#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Pathfinding.h"
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
		//loadingScreen = app->tex->Load("Assets/maps/Loading_screen.png");
		loadingScreen = app->tex->Load("Assets/maps/mini_Loading_screen.png");

		app->audio->PlayFx(titleMusic, 0);
	}break;
	case GAMEPLAY:
	{
		//state == INTRO;
	// L03: DONE: Load map
		texBackground = app->tex->Load("Assets/maps/bg.png");
		if (app->map->Load("platform_test.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data))
				app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}

		jungleMusic = app->audio->LoadFx("Assets/audio/music/videoplayback.ogg");

		//app->audio->PlayMusic("Assets/audio/music/videoplayback.ogg");    Destroy ears
		app->audio->PlayFx(jungleMusic, 0);

		for (int x = 0; x < app->map->data.layers.start->data->width; x++)
		{
			for (int y = 0; y < app->map->data.layers.start->data->height; y++)
			{

				int gid = app->map->data.layers.start->data->Get(x, y);

				SDL_Rect rect = app->map->data.tilesets.start->data->GetTileRect(gid);
				iPoint screenPos = app->map->MapToWorld(x, y);

				app->render->DrawTexture(app->map->data.tilesets.start->data->texture, screenPos.x, screenPos.y, &rect);
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

		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_BIRD, 48 * 10 - 24, 48 * 10 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_BIRD, 48 * 28 - 24, 48 * 7 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_BIRD, 48 * 43 - 24, 48 * 13 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_BIRD, 48 * 73 - 24, 48 * 25 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_BIRD, 48 * 74 - 24, 48 * 6 - 24);

		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 12 - 24, 48 * 21 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 15 - 22, 48 * 2 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 70 - 22, 48 * 20 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 45 - 45, 48 * 22 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 70 - 67, 48 * 27 - 24);
		app->enemyMaster->CreateEnemy(EnemyType::ENEMY_MUSHROOM, 48 * 70 - 67, 48 * 7 - 24);
		marginX = 5;
		marginX = 5;


	}break;
	}
	goodEndingScreen = app->tex->Load("Assets/maps/good_ending_screen.png");
	badEndingScreen = app->tex->Load("Assets/maps/bad_ending_screen.png");
	badEnding = app->audio->LoadFx("Assets/audio/fx/bad_ending.wav");
	goodEnding = app->audio->LoadFx("Assets/audio/fx/good_ending.wav");
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
		if ((lastTime + 300 > currentTime)&&(!loadingScreenActive))
		{
			app->render->DrawTexture(intro01, -50, 500);
		}
		else if ((lastTime + 300 < currentTime) && (lastTime + 600 > currentTime) && (!loadingScreenActive))
		{
			app->render->DrawTexture(intro02, -50, 500);
		}
		else
		{
			if (!loadingScreenActive)
			{
				app->render->DrawTexture(intro02, -50, 500);
				lastTime = currentTime;
			}
			else
			{
				app->render->DrawTexture(loadingScreen, 55, 800);
				if (lastTime + 300 <= currentTime)
				{
					app->audio->clearAudio();
					state = GAMEPLAY;
					Start();
					app->player->Start();

				}

			}
		}
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			loadingScreenActive = true;
			lastTime = currentTime;
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
			app->SaveGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (!freeCam) freeCam = true;
			else freeCam = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			if (app->GetMaxFrames() == 30) app->SetMaxFrames(60);
			else if (app->GetMaxFrames() == 60) app->SetMaxFrames(30);
			//LOG("current: %i", app->GetMaxFrames());
			
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
				if (app->render->camera.y > (-48 * 15 + 16))
				{
					app->render->camera.y -= cameraSpeed;
				}



			if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
				if (app->render->camera.x < (0))
				{
					app->render->camera.x += cameraSpeed;
				}



			if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
				if (app->render->camera.x > (-48 * 78 + 24))
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

		//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

		// Draw map
		app->render->DrawTexture(texBackground, 0, 0, NULL, SDL_FLIP_NONE, 0.6f);
		app->render->DrawTexture(texBackground, 2304, 0, NULL, SDL_FLIP_NONE, 0.6f);

		app->map->Draw();

		// L03: DONE 7: Set the window title with map/tileset info
		SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
			app->map->data.width, app->map->data.height,
			app->map->data.tileWidth, app->map->data.tileHeight,
			app->map->data.tilesets.count());

		app->win->SetTitle(title.GetString());

		if ((app->player->GetColHitbox()->body->GetPosition().x > 90)||(app->player->GetPlayerLifes()<=0))
		{
			if (app->player->GetPlayerLifes() <= 0) whichEnding = false;
			app->audio->clearAudio();
			state = END;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			//Start();
			
			

		}


	}break;
	case END:
	{
		//app->render->DrawTexture(loadingScreen, 55, 800,NULL,SDL_FLIP_NONE,0);
		if (whichEnding)
		{
			if (sound)
			{
				app->audio->PlayFx(goodEnding);
				sound = false;
			}
			app->render->DrawTexture(goodEndingScreen, 55, 100);
			
		}
		else
		{
			if (sound)
			{
				app->audio->PlayFx(badEnding);
				sound = false;
			}
			app->render->DrawTexture(badEndingScreen, 0, 20);
		}
		app->enemyMaster->DestroyAllEnemies();
	
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

// L02: TODO 8: create a method to save the state of the renderer
// Save Game State
bool Scene::SaveState(pugi::xml_node& data) const
{
	//...

	return true;
}
