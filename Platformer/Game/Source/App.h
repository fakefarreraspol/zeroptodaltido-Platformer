#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;

class Map;
class PathFinding;
class Physics;
class ModuleFonts;
class Player;
class EntityHandler;
class EnemySnake;
class EnemyMushroom;
class EnemyBird;
class RocketBanana;
class UI_Handler;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;
	float DEGTORAD() const
	{
		return degtorad;
	}

	float RADTODEG() const
	{
		return radtodeg;
	}
private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

	

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	
	Map* map;
	Physics* physics;
	ModuleFonts* fonts;
	Player* player;
	EntityHandler* entityMaster;
	UI_Handler* UI_handler;
	PathFinding* pathfinding;


	float DeltaTime()
	{
		return dt;
	}

	void SetMaxFrames(int maxFrames)
	{
		limitFrames = maxFrames;
	}

	int GetMaxFrames() const
	{
		return limitFrames;
	}

	void SetPause(bool pause)
	{
		gamePaused = pause;
	}

	bool GameIsPaused() const
	{
		return gamePaused;
	}


private:

	
	bool gamePaused;
	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	float degtorad = 0.0174532925199432957f;
	float radtodeg = 57.295779513082320876f;

	float limitFrames = 60;
	float init;
	float start = 0;
	float end;
	float currentFPS;
	double totalFrames = 0;

	// L01: DONE 2: Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	//pugi::xml_document configFile;
	//pugi::xml_node config;
	//pugi::xml_node configApp;

	uint frames;
	float dt;

	// L02: DONE 1: Create variables to control when to execute the request load / save
	mutable bool saveGameRequested;
	bool loadGameRequested;
};

extern App* app;

#endif	// __APP_H__