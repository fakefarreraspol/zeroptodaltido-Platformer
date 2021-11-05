#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	texBackground = app->tex->Load("Assets/maps/bg.png");
	app->map->Load("platform.tmx");
	jungleMusic = app->audio->LoadFx("Assets/audio/music/videoplayback.ogg");
	//app->audio->PlayMusic("Assets/audio/music/videoplayback.ogg");    Destroy ears
	app->audio->PlayFx(jungleMusic, 0);
	//app->physics->CreateKinematicChain(0, 0,		rightSlope_30_1, 6);
	//app->physics->CreateKinematicChain(48, 0,		rightSlope_30_2, 6);
	//app->physics->CreateKinematicChain(48 * 2, 0,	rightSlope_30_3, 6);
	//app->physics->CreateKinematicChain(48 * 3, 0,		leftSlope_30_1, 6);
	//app->physics->CreateKinematicChain(48 * 4, 0,		leftSlope_30_2, 6);
	//app->physics->CreateKinematicChain(48 * 5, 0,	leftSlope_30_3, 6);
	//
	//app->physics->CreateKinematicChain(0, 48, rightSlope_45, 6);
	//app->physics->CreateKinematicChain(48, 48, leftSlope_45, 6);


	for (int x = 0; x < app->map->mapData.maplayers.start->data->width; x++)
	{
		for (int y = 0; y < app->map->mapData.maplayers.start->data->height; y++)
		{
	
			int gid = app->map->mapData.maplayers.start->data->Get(x, y);
	
			SDL_Rect rect = app->map->mapData.tilesets.start->data->GetTileRect(gid);
			iPoint screenPos = app->map->MapToWorld(x, y);
	
			app->render->DrawTexture(app->map->mapData.tilesets.start->data->texture, screenPos.x, screenPos.y, &rect);
			

			for (size_t i = 0; i < 44; i++)
			{
				if (gid == squareGround[i])
				{
					app->physics->CreateKinematicRectangle(screenPos.x + 24, screenPos.y + 24, 48, 48);
				}
			}
			PhysBody* temp;

			switch (gid)
			{
			case 5:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, rightSlope_45, 6);
				break;

			case 8:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, leftSlope_45, 6);

				break;

			case 22:

				temp = app->physics->CreateKinematicChain(screenPos.x, screenPos.y + 48, leftSlope_45, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), 90.f * app->DEGTORAD());
				break;
			case 23:
				temp = app->physics->CreateKinematicChain(screenPos.x, screenPos.y + 48, rightSlope_45, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), -90.f * app->DEGTORAD());

				break;

			case 47:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, rightSlope_30_1, 6);
				break;

			case 48:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, rightSlope_30_2, 6);

				break;
			case 49:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, rightSlope_30_3, 6);

				break;
			case 50:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, leftSlope_30_1, 6);

				break;
			case 51:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, leftSlope_30_2, 6);

				break;
			case 52:
				app->physics->CreateKinematicChain(screenPos.x, screenPos.y, leftSlope_30_3, 6);

				break;
			case 28:

				temp = app->physics->CreateKinematicChain(screenPos.x, screenPos.y + 48, leftSlope_45, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), 90.f * app->DEGTORAD());
				break;
			case 29:
				temp = app->physics->CreateKinematicChain(screenPos.x, screenPos.y + 48, rightSlope_45, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), -90.f * app->DEGTORAD());

				break;
			case 30:
				temp = app->physics->CreateKinematicChain(screenPos.x, screenPos.y + 48, rightSlope_45, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), -90.f * app->DEGTORAD());

				break;
			case 31:

				temp = app->physics->CreateKinematicChain(screenPos.x + 48, screenPos.y + 16, leftSlope_30_1, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());
				break;
			case 32:
				temp = app->physics->CreateKinematicChain(screenPos.x + 48, screenPos.y + 48, leftSlope_30_2, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());

				break;
			case 33:
				temp = app->physics->CreateKinematicChain(screenPos.x + 48, screenPos.y + 48 + 32, leftSlope_30_3, 6);
				temp->body->SetFixedRotation(true);
				temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());

				break;
			default:
				break;
			}

	
		}
	}


	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	int cameraSpeed = 10;

    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += cameraSpeed;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= cameraSpeed;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += cameraSpeed;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= cameraSpeed;

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_UP)
	{
		app->physics->CreateCircle(app->input->GetMouseX() - app->render->camera.x, app->input->GetMouseY() - app->render->camera.y, 25);
		LOG("circle created");
	}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->render->DrawTexture(texBackground, 0, 0, NULL, 0.4f);
	app->map->Draw();

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());
	
	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
