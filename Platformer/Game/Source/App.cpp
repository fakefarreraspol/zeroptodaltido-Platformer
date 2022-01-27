#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"

#include "Map.h"
#include "Pathfinding.h"
#include "Physics.h"
#include "ModuleFonts.h"
#include "Player.h"
#include "EntityHandler.h"
#include "UIHandler.h"
#include "EnemySnake.h"
#include "EnemyMushroom.h"
#include "EnemyBird.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();

	map = new Map();
	pathfinding = new PathFinding();
	physics = new Physics();
	fonts = new ModuleFonts();
	player = new Player();
	entityMaster = new EntityHandler();
	UI_handler = new UI_Handler();


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(fonts);
	AddModule(UI_handler);
	AddModule(scene);
	
	AddModule(map);
	AddModule(pathfinding);
	AddModule(player);
	AddModule(entityMaster);
	

	
	

	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{

	totalFrames++;
	init = SDL_GetTicks();
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();
	end = SDL_GetTicks();


	long elapsedTime = (float)(end - init);
	(float)SDL_GetPerformanceFrequency();

	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//{
	//	limitFrames /= 2;
	//}
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_UP)
	//{
	//	limitFrames *= 2;
	//}


	currentFPS = limitFrames - (float)elapsedTime;
	float frameSpeed = 1000 / currentFPS;
	dt = 1.f / currentFPS;

	//LOG("Current FPS: %f", currentFPS);

	//LOG("Current FPS: %f", currentFPS);
	//LOG("time left: %f", frameSpeed - elapsedTime);
	
	

	if (win->GetVsync())
	{
		SString title("FPS: %f / average FPS: % / Last-frame MS: %f, Vsync: On",
			limitFrames,
			currentFPS,
			(float)(1000.0 * (double(SDL_GetPerformanceCounter() - init) / double(SDL_GetPerformanceFrequency())))
		);
		app->win->SetTitle(title.GetString());
	}
	if (!win->GetVsync())
	{
		SString title("FPS: %.3f / average FPS: %.3f / Last-frame MS: %.3f / Total time MS: %.2f / Vsync: Off",
			limitFrames,
			currentFPS,
			(float)frameSpeed - elapsedTime,
			SDL_GetTicks() - start
		);
		app->win->SetTitle(title.GetString());
	}


	if ((frameSpeed - elapsedTime) > 0.0f)
	{
		SDL_Delay(fabs(floor((long)frameSpeed - elapsedTime)));
	}


	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{


}

// ---------------------------------------------
void App::FinishUpdate()
{

	//SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
	//	app->map->data.width, app->map->data.height,
	//	app->map->data.tileWidth, app->map->data.tileHeight,
	//	app->map->data.tilesets.count());
	
	


	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) 
		LoadGame();
	if (saveGameRequested == true) 
		SaveGame();
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = false;

	pugi::xml_document configFile;
	pugi::xml_node config;
	config = LoadConfig(configFile);

	ListItem<Module*>* item;
	item = modules.start;
	//LOG("name: %s", item->data->name.GetString());
	while (item != NULL)
	{
		SString name = item->data->name;
		LOG("name: %s", name.GetString());
		ret = item->data->LoadState(config.child(name.GetString()));
		item = item->next;
	}
	

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document configFile;
	pugi::xml_node config;
	config = LoadConfig(configFile);

	ListItem<Module*>* item;
	item = modules.start;
	//LOG("name: %s", item->data->name.GetString());
	while (item != NULL)
	{
		SString name = item->data->name;
		LOG("name: %s", name.GetString());
		ret = item->data->SaveState(config.child(name.GetString()));
		item = item->next;
	}

	ret = configFile.save_file("config.xml");

	saveGameRequested = false;

	return ret;
}



