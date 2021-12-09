#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "EnemyHandler.h"
#include "EnemyMushroom.h"
#include "EnemySnake.h"
#include "EnemyBird.h"


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
	
	switch (state)
	{
	case INTRO:
	{
		intro01 = app->tex->Load("Assets/maps/intro_1.png");
		intro02 = app->tex->Load("Assets/maps/intro_2.png");
		titleMusic = app->audio->LoadFx("Assets/audio/music/title.wav");
		
		app->audio->PlayFx(titleMusic, 0);
	}break;
	case GAMEPLAY:
	{
		//state == INTRO;
	// L03: DONE: Load map
		texBackground = app->tex->Load("Assets/maps/bg.png");
		app->map->Load("platform_test.tmx");
		jungleMusic = app->audio->LoadFx("Assets/audio/music/videoplayback.ogg");
		
		//app->audio->PlayMusic("Assets/audio/music/videoplayback.ogg");    Destroy ears
		app->audio->PlayFx(jungleMusic, 0);

		for (int x = 0; x < app->map->mapData.maplayers.start->data->width; x++)
		{
			for (int y = 0; y < app->map->mapData.maplayers.start->data->height; y++)
			{

				int gid = app->map->mapData.maplayers.start->data->Get(x, y);

				SDL_Rect rect = app->map->mapData.tilesets.start->data->GetTileRect(gid);
				iPoint screenPos = app->map->MapToWorld(x, y);

				app->render->DrawTexture(app->map->mapData.tilesets.start->data->texture, screenPos.x, screenPos.y, &rect);
				PhysBody* temp;

				for (size_t i = 0; i < 44; i++)
				{
					if (gid == squareGround[i])
					{
						temp = app->physics->CreateRectangle(screenPos.x + 24, screenPos.y + 24, 48, 48);
						temp->body->SetType(b2_staticBody);
						temp->type = TYPE_SOLID_TILE;
						temp->body->SetUserData((void*)temp);

					}
				}


				switch (gid)
				{
				case 5:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, rightSlope_45, 6);
					break;

				case 8:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, leftSlope_45, 6);

					break;

				case 22:

					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y, leftSlope_45, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 90.f * app->DEGTORAD());
					break;
				case 23:
					temp = app->physics->CreateStaticChain(screenPos.x, screenPos.y + 48, rightSlope_45, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), -90.f * app->DEGTORAD());

					break;

				case 47:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, rightSlope_30_1, 6);
					break;

				case 48:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, rightSlope_30_2, 6);

					break;
				case 49:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, rightSlope_30_3, 6);

					break;
				case 50:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, leftSlope_30_1, 6);

					break;
				case 51:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, leftSlope_30_2, 6);

					break;
				case 52:
					app->physics->CreateStaticChain(screenPos.x, screenPos.y, leftSlope_30_3, 6);

					break;
				case 15:

					temp = app->physics->CreateKinematicRectangle(screenPos.x + 24, screenPos.y + 10, 48, 16);
					temp->type = TYPE_SOLID_TILE;
					temp->body->SetUserData((void*)temp);
					trespasableElements.add(temp);
					break;


				case 28:

					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y + 16 * 5, rightSlope_30_1, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());
					break;
				case 29:
					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y + 16 * 3, rightSlope_30_2, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());

					break;
				case 30:
					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y + 16, rightSlope_30_3, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());

					break;
				case 31:

					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y + 16, leftSlope_30_1, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());
					break;
				case 32:
					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y + 48, leftSlope_30_2, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());

					break;
				case 33:
					temp = app->physics->CreateStaticChain(screenPos.x + 48, screenPos.y + 48 + 32, leftSlope_30_3, 6);
					temp->body->SetFixedRotation(true);
					temp->body->SetTransform(temp->body->GetPosition(), 180.f * app->DEGTORAD());

					break;
				default:
					break;
				}

				temp = nullptr;
				delete temp;
			}


		}
		
		
		/*
			r_characterRun[0]= { 0,37, 50, 37*2 };
			r_characterRun[1] = { 50,37, 100, 37 * 2 };
			r_characterRun[2] = { 100,37, 150, 37 * 2 };
			r_characterRun[3] = { 150,37, 200, 37 * 2 };
			r_characterRun[4] = { 200,37, 250, 37 * 2 };
			r_characterRun[5] = { 250,37, 300, 37 * 2 };
			r_characterRun[6] = { 300,37, 350, 37 * 2 };
			r_characterJump[0] = { 0,37 * 3, 50, 37 * 4 };
			r_characterJump[1] = { 50,37 * 3, 100, 37 * 4 };
			r_characterJump[2] = { 100,37 * 3, 150, 37 * 4 };
			r_characterJump[3] = { 150,37 * 3, 200, 37 * 4 };
			r_characterJump[4] = { 200,37 * 3, 250, 37 * 4 };
			r_characterJump[5] = { 250,37 * 3, 300, 37 * 4 };
			r_characterJump[6] = { 300,37 * 3, 350, 37 * 4 };
			*/
		r_mushroomWalk[0] = { 0,0,48,48 };
		r_mushroomWalk[1] = { 48,0,48,48 };
		r_mushroomWalk[2] = { 48 * 2,0,48,48 };
		r_mushroomWalk[3] = { 48 * 3,0,48,48 };

		//Mushroom 01
		mushroom = app->tex->Load("Assets/textures/mushroom_walk.png");
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 12 - 24, 48 * 21 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 15 - 24, 48 * 14 - 24);
		marginX = 5;
		marginX = 5;

		
	}break;
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
	currentTime = SDL_GetTicks();
	switch (state)
	{
	case INTRO:
	{
		if (lastTime + 300 > currentTime)
		{
			app->render->DrawTexture(intro01, -50, 500);
		}
		else if ((lastTime + 300 < currentTime) && (lastTime + 600 > currentTime))
		{
			app->render->DrawTexture(intro02, -50, 500);
		}
		else
		{
			app->render->DrawTexture(intro02, -50, 500);
			lastTime = currentTime;
		}
		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			app->audio->clearAudio();
			state = GAMEPLAY;
			Start();
			app->player->Start();

		}
	}break;
	case GAMEPLAY:
	{
		
		int cameraSpeed = 10;



		//int mushroomIdleFrameSpeed = 500;
		//
		//if (!isMushroomWalking)
		//{
		//	EnemyMushroomHitbox = mushrooms.getFirst()->data;
		//	if (lastMushroomDirection)
		//	{
		//		if (lastTime + mushroomIdleFrameSpeed > currentTime)
		//		{
		//			app->render->DrawTexture(mushroom, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().y) - 19 * 2, &r_mushroomIdle[currentMushroomIdle], SDL_FLIP_HORIZONTAL);
		//		}
		//		else
		//		{
		//			if (currentMushroomIdle < 3)
		//			{
		//
		//				lastTime = currentTime;
		//				app->render->DrawTexture(mushroom, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().y) - 19 * 2, &r_mushroomIdle[currentMushroomIdle], SDL_FLIP_HORIZONTAL);
		//				currentMushroomIdle++;
		//			}
		//			else currentMushroomIdle = 0;
		//
		//
		//		}
		//
		//
		//	}
		//	else if (!lastMushroomDirection)
		//	{
		//		if (lastTime + mushroomIdleFrameSpeed > currentTime)
		//		{
		//			app->render->DrawTexture(mushroom, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().y) - 19 * 2, &r_mushroomIdle[currentMushroomIdle]);
		//
		//		}
		//		else
		//		{
		//			if (currentMushroomIdle < 3)
		//			{
		//				currentMushroomIdle++;
		//				lastTime = currentTime;
		//				app->render->DrawTexture(mushroom, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(EnemyMushroomHitbox->body->GetPosition().y) - 19 * 2, &r_mushroomIdle[currentMushroomIdle]);
		//
		//			}
		//			else currentMushroomIdle = 0;
		//
		//
		//
		//
		//		}
		//	}
		//}








		uint screnWidth, screenHeight;
		app->win->GetWindowSize(screnWidth, screenHeight);

		// L02: DONE 3: Request Load / Save when pressing L/S
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->LoadGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->SaveGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (!freeCam) freeCam = true;
			else freeCam = false;
		}
		playerX = app->player->GetColHitbox()->body->GetPosition().x;
		playerY = app->player->GetColHitbox()->body->GetPosition().y;

		if (!freeCam)
		{
			//app->render->camera.x = -(playerX );
			if ((playerY < 24) && (playerY > 1))
			{
				app->render->camera.y = -(playerY * 30);
			}


			if (playerX > marginX && playerX < 100 * 48)
			{
				app->render->camera.x = -playerX * 48 * 1.042f + marginX * 48;
			}



		}
		else
		{
			if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT))
				if (app->render->camera.y < (-5))
				{
					app->render->camera.y += cameraSpeed;
				}




			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
				if (app->render->camera.y > (-48 * 14))
				{
					app->render->camera.y -= cameraSpeed;
				}



			if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
				if (app->render->camera.x < (0))
				{
					app->render->camera.x += cameraSpeed;
				}



			if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
				if (app->render->camera.x > (-48 * 33))
				{
					app->render->camera.x -= cameraSpeed;
				}

		}

		//app->render->camera.x -= cameraSpeed;
		//LOG("player x %f", playerX * 48);


		//LOG("player position %f", playerX);
		//LOG("player y %f", playerY);
		//LOG("Camera position %i", app->render->camera.x);
		//LOG("difference: %f", (float)app->render->camera.x - playerX * 48);

		/*if ((playerX > 10)&& (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
		{
			app->render->camera.x -= 2;
		}*/


		p2List_item <PhysBody*>* trespasableCounter = trespasableElements.getFirst();
		while (trespasableCounter != nullptr)
		{

			if (trespasableCounter->data->body->GetContactList() != nullptr)
			{
				b2Body* playerHitbox = trespasableCounter->data->body->GetContactList()->contact->GetFixtureB()->GetBody();




				if (playerHitbox == app->player->GetColHitbox()->body &&
					playerHitbox->GetPosition().y + PIXEL_TO_METERS(30) > trespasableCounter->data->body->GetPosition().y)
				{
					trespasableCounter->data->body->GetFixtureList()->SetSensor(true);

				}
				else
				{
					if (trespasableCounter->data->body->GetFixtureList()->IsSensor())
					{

						trespasableCounter->data->body->GetFixtureList()->SetSensor(false);
					}
				}
			}

			trespasableCounter = trespasableCounter->next;
		}

		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_UP)
		{
			app->physics->CreateCircle(app->input->GetMouseX() - app->render->camera.x, app->input->GetMouseY() - app->render->camera.y, 23);
			LOG("circle created");
		}

		//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

		// Draw map
		app->render->DrawTexture(texBackground, 0, 0, NULL, SDL_FLIP_NONE, 0.6f);
		app->render->DrawTexture(texBackground, 2304, 0, NULL, SDL_FLIP_NONE, 0.6f);

		app->map->Draw();

		// L03: DONE 7: Set the window title with map/tileset info
		SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
			app->map->mapData.width, app->map->mapData.height,
			app->map->mapData.tileWidth, app->map->mapData.tileHeight,
			app->map->mapData.tilesets.count());

		app->win->SetTitle(title.GetString());




	}break;
	}
	
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	


	

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node& data)
{

	return true;
}

// L02: TODO 8: Create a method to save the state of the renderer
// Save Game State
bool Scene::SaveState(pugi::xml_node& data) const
{
	//...

	return true;
}
