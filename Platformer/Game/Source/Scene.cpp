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
#include "EntityHandler.h"
#include "EnemyMushroom.h"
#include "EnemySnake.h"
#include "EnemyBird.h"
#include "RocketBanana.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include <stdio.h>


#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
	titleMusic = 1000;
	jungleMusic = 1000;
}

// Destructor
Scene::~Scene()
{
}

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
		char lookupTable1[] = { "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[£]çç€!ççç%&'()*+,-.^0123456789:;<=>?/abcdefghijklmnopqrstuvwxyz ççççççç" };

		font1_gold_1 = app->fonts->Load("Assets/textures/UI/fonts/font1_gold_1.png", lookupTable1, 6);
		font1_black_1 = app->fonts->Load("Assets/textures/UI/fonts/font1_black_1.png", lookupTable1, 6);
		font1_white_1 = app->fonts->Load("Assets/textures/UI/fonts/font1_white_1.png", lookupTable1, 6);

		font1_black_2 = app->fonts->Load("Assets/textures/UI/fonts/font1_black_2.png", lookupTable1, 6);
		font1_gold_2 = app->fonts->Load("Assets/textures/UI/fonts/font1_gold_2.png", lookupTable1, 6);
		font1_white_2 = app->fonts->Load("Assets/textures/UI/fonts/font1_white_2.png", lookupTable1, 6);

		font1_gold_3 = app->fonts->Load("Assets/textures/UI/fonts/font1_gold_2.png", lookupTable1, 6);
		font1_black_3 = app->fonts->Load("Assets/textures/UI/fonts/font1_black_3.png", lookupTable1, 6);
		font1_white_3 = app->fonts->Load("Assets/textures/UI/fonts/font1_white_3.png", lookupTable1, 6);

		intro01 = app->tex->Load("Assets/maps/intro_1.png");
		intro02 = app->tex->Load("Assets/maps/intro_2.png");
		titleMusic = app->audio->LoadFx("Assets/audio/music/title.wav");
		//loadingScreen = app->tex->Load("Assets/maps/Loading_screen.png");
		loadingScreen = app->tex->Load("Assets/maps/mini_Loading_screen.png");

		app->audio->PlayFx(titleMusic, 0);

		//title
		SDL_Rect rec_panel = { 674,60,200,580};
		UI_panel_title = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 580/2 - 80, -180, 0, 0, 90);
		UI_panel_title->sprite = app->UI_handler->spritesheet;
		UI_panel_title->rec_sprite = rec_panel;

		
		rec_panel = { 679,654,327,72 };
		UI_button_start_game = app->UI_handler->CreateButton(app->UI_handler->spritesheet, 1080 - 327 - 48 , 720 - 72*2 - 48 - 12, 327, 72);
		UI_button_start_game->sprite = app->UI_handler->spritesheet;
		UI_button_start_game->rec_sprite = rec_panel;
		UI_button_start_game->rec_over = { 676,801,334,79 };
		UI_button_start_game->rec_hold = { 679,882,327,72 };
		UI_button_start_game->action = ACTION_START_GAME;

		rec_panel = { 679,729,327,72 };
		UI_button_quit_game = app->UI_handler->CreateButton(app->UI_handler->spritesheet, 1080 - 327 - 48, 720 - 72 - 48 , 327, 72);
		UI_button_quit_game->sprite = app->UI_handler->spritesheet;
		UI_button_quit_game->rec_sprite = rec_panel;
		UI_button_quit_game->rec_over = { 676,801,334,79 };
		UI_button_quit_game->rec_hold = { 679,882,327,72 };
		UI_button_quit_game->action = ACTION_EXIT_GAME_CONFIRM;

		//settings
		rec_panel = { 636,4,48,48 };
		UI_button_settings = app->UI_handler->CreateButton(app->UI_handler->spritesheet, 1080 - 48 - 12, 12, 48, 48);
		UI_button_settings->sprite = app->UI_handler->spritesheet;
		UI_button_settings->rec_sprite = rec_panel;
		UI_button_settings->action = ACTION_SETTINGS_OPEN;

		//panel pause
		rec_panel = { 0, 312, 672, 432 };
		UI_panel_pause_menu = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 1080 / 2 - 672 / 2, 720 / 2 - 432 / 2, 672, 432);
		UI_panel_pause_menu->sprite = app->UI_handler->spritesheet;
		UI_panel_pause_menu->rec_sprite = rec_panel;

		//panel exit game
		rec_panel = { 692, 4, 268, 48 };
		UI_panel_pause_menu_exit_game = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 1080 / 2 + 14, 720 / 2 + 432 / 4 + 128, 268, 48);
		UI_panel_pause_menu_exit_game->sprite = app->UI_handler->spritesheet;
		UI_panel_pause_menu_exit_game->rec_sprite = rec_panel;

		////pause open button
		//rec_panel = { 468, 4, 48, 48 };
		//UI_button_open_pause_menu = app->UI_handler->CreateButton(app->UI_handler->spritesheet, 1080 - 48 - 14, 14, 48, 48);
		//UI_button_open_pause_menu->sprite = app->UI_handler->spritesheet;
		//UI_button_open_pause_menu->rec_sprite = rec_panel;
		//UI_button_open_pause_menu->action = ACTION_PAUSE_OPEN;

		//pause close button
		rec_panel = { 412, 60, 48, 48 };
		UI_button_close_pause_menu = app->UI_handler->CreateButton(
			app->UI_handler->spritesheet,
			1080 / 2 + 584 / 2 - 28,
			584 / 2 - 432 / 4,
			48,
			48);
		UI_button_close_pause_menu->sprite = app->UI_handler->spritesheet;
		UI_button_close_pause_menu->rec_sprite = rec_panel;
		UI_button_close_pause_menu->action = ACTION_PAUSE_CLOSE;



		//exit button
		rec_panel = { 468, 60, 48, 48 };
		UI_button_exit_game = app->UI_handler->CreateButton(
			app->UI_handler->spritesheet,
			1080 / 2 + 584 / 2 - 28,
			720 / 2 + 432 / 2 - 64,
			48,
			48);
		UI_button_exit_game->sprite = app->UI_handler->spritesheet;
		UI_button_exit_game->rec_sprite = rec_panel;
		UI_button_exit_game->action = ACTION_EXIT_GAME;

		//exit button confirm
		rec_panel = { 412, 4, 48, 48 };
		UI_button_exit_game_confirm = app->UI_handler->CreateButton(
			app->UI_handler->spritesheet,
			1080 / 2 - 584 / 2 + 584 + 4,
			720 / 2 + 432 / 4 + 128,
			48,
			48
		);
		UI_button_exit_game_confirm->sprite = app->UI_handler->spritesheet;
		UI_button_exit_game_confirm->rec_sprite = rec_panel;
		UI_button_exit_game_confirm->action = ACTION_EXIT_GAME_CONFIRM;

		//exit button deny
		rec_panel = { 468, 60, 48, 48 };
		UI_button_exit_game_deny = app->UI_handler->CreateButton(
			app->UI_handler->spritesheet,
			1080 / 2 - 584 / 2 + 584 + 48 + 16,
			720 / 2 + 432 / 4 + 128,
			48,
			48
		);
		UI_button_exit_game_deny->sprite = app->UI_handler->spritesheet;
		UI_button_exit_game_deny->rec_sprite = rec_panel;
		UI_button_exit_game_deny->action = ACTION_EXIT_GAME_DENY;

		//save button
		rec_panel = { 320, 116, 132, 48 };
		UI_button_save_game = app->UI_handler->CreateButton(
			app->UI_handler->spritesheet,
			1080 / 2 - 584 / 2 + 584 / 4,
			720 / 2 + 432 / 4 + 24,
			132,
			48
		);
		UI_button_save_game->sprite = app->UI_handler->spritesheet;
		UI_button_save_game->rec_sprite = rec_panel;
		UI_button_save_game->rec_over = { 468,112,140,56 };
		UI_button_save_game->rec_hold = { 472,172,132,48 };
		UI_button_save_game->action = ACTION_SAVE_GAME;

		//load button
		rec_panel = { 180, 116, 132, 48 };
		UI_button_load_game = app->UI_handler->CreateButton(
			app->UI_handler->spritesheet,
			1080 / 2 + 584 / 2 - 584 / 4 - 132,
			720 / 2 + 432 / 4 + 24,
			132,
			48
		);
		UI_button_load_game->sprite = app->UI_handler->spritesheet;
		UI_button_load_game->rec_sprite = rec_panel;
		UI_button_load_game->rec_over = { 468,112,140,56 };
		UI_button_load_game->rec_hold = { 472,172,132,48 };
		UI_button_load_game->action = ACTION_LOAD_GAME;

		//sfx volume slider
		rec_panel = { 0, 60, 256, 24 };
		UI_slider_sfx_vol = app->UI_handler->CreateSlider(450, 420, 128, app->UI_handler->spritesheet, { 0 }, { 0 }, { 0 }, { 0 });
		UI_slider_sfx_vol->rec_sprite = rec_panel;
		rec_panel = { 4, 92, 244, 16 };
		UI_slider_sfx_vol->rec_body_fill = rec_panel;
		rec_panel = { 0, 0, 116, 60 };
		UI_slider_sfx_vol->rec_slider_hold = rec_panel;
		rec_panel = { 120, 0, 120, 60 };
		UI_slider_sfx_vol->rec_slider = rec_panel;
		UI_slider_sfx_vol->w = 256;
		UI_slider_sfx_vol->h = 24;
		UI_slider_sfx_vol->set = SET_SFX_VOLUME;

		//global audio slider
		rec_panel = { 0, 60, 256, 24 };
		UI_slider_global_audio = app->UI_handler->CreateSlider(450, 320, 100, app->UI_handler->spritesheet, { 0 }, { 0 }, { 0 }, { 0 });
		UI_slider_global_audio->rec_sprite = rec_panel;
		rec_panel = { 4, 92, 244, 16 };
		UI_slider_global_audio->rec_body_fill = rec_panel;
		rec_panel = { 0, 0, 116, 60 };
		UI_slider_global_audio->rec_slider_hold = rec_panel;
		rec_panel = { 120, 0, 120, 60 };
		UI_slider_global_audio->rec_slider = rec_panel;
		UI_slider_global_audio->w = 256;
		UI_slider_global_audio->h = 24;
		UI_slider_global_audio->set = SET_GLOBAL_VOLUME;


		//music volume slider
		rec_panel = { 0, 60, 256, 24 };
		UI_slider_music_vol = app->UI_handler->CreateSlider(450, 370, 128, app->UI_handler->spritesheet, { 0 }, { 0 }, { 0 }, { 0 });
		UI_slider_music_vol->rec_sprite = rec_panel;
		rec_panel = { 4, 92, 244, 16 };
		UI_slider_music_vol->rec_body_fill = rec_panel;
		rec_panel = { 0, 0, 116, 60 };
		UI_slider_music_vol->rec_slider_hold = rec_panel;
		rec_panel = { 120, 0, 120, 60 };
		UI_slider_music_vol->rec_slider = rec_panel;
		UI_slider_music_vol->w = 256;
		UI_slider_music_vol->h = 24;
		UI_slider_music_vol->set = SET_MUSIC_VOLUME;

	}break;
	case GAMEPLAY:
	{
		//state == INTRO;
		flag = app->tex->Load("Assets/maps/Nature_environment_01.png");
		
		r_flag[0] = { 672,48,48,48 };
		r_flag[1] = { 672+(48*3),48,48,48 };
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
		//jungleMusic = app->audio->PlayMusic("Assets/audio/music/cyber.ogg");

		//Mix_Music* music = Mix_LoadMUS("Assets/audio/music/mission_1_repeat.ogg");
		//Mix_PlayMusic(music, -1);

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



		app->entityMaster->CreateEntity(EntityType::ENEMY_BIRD, 48 * 10 - 24, 48 * 10 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_BIRD, 48 * 28 - 24, 48 * 7 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_BIRD, 48 * 43 - 24, 48 * 13 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_BIRD, 48 * 73 - 24, 48 * 25 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_BIRD, 48 * 74 - 24, 48 * 6 - 24);

		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, 48 * 12 - 24, 48 * 22 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, 48 * 15 - 22, 48 * 2 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, 48 * 70 - 22, 48 * 20 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, 48 * 45 - 45, 48 * 22 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, 48 * 70 - 67, 48 * 27 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, 48 * 70 - 67, 48 * 7 - 24);
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, METERS_TO_PIXELS(25), METERS_TO_PIXELS(25));
		app->entityMaster->CreateEntity(EntityType::ENEMY_MUSHROOM, METERS_TO_PIXELS(53.6f), METERS_TO_PIXELS(9.5f));

		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, METERS_TO_PIXELS(39), METERS_TO_PIXELS(21.5f) + 8 );
		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, METERS_TO_PIXELS(54.f), METERS_TO_PIXELS(28.5f) + 8);
		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, METERS_TO_PIXELS(79.5f), METERS_TO_PIXELS(25.5f) + 8);
		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, METERS_TO_PIXELS(67.5f), METERS_TO_PIXELS(3.5f) + 8);
		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, METERS_TO_PIXELS(36.5f), METERS_TO_PIXELS(27.5f) + 8);

		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, 48 * 13 - 24, 48 * 22 - 24);
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(33.52f), METERS_TO_PIXELS(24.52f));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(3.52f), METERS_TO_PIXELS(3.52f));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(39.52f), METERS_TO_PIXELS(14.52f));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(37), METERS_TO_PIXELS(21.5f));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(37), METERS_TO_PIXELS(22.5f));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(47), METERS_TO_PIXELS(28));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(47), METERS_TO_PIXELS(28));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(71), METERS_TO_PIXELS(20));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(6), METERS_TO_PIXELS(28.11f));
		app->entityMaster->CreateEntity(EntityType::ITEM_BANANA, METERS_TO_PIXELS(15.7f), METERS_TO_PIXELS(28.4f));
		
		

		//app->entityMaster->CreateEntity(EntityType::ROCKET_BANANA, 48 * 10 - 24, 48 * 15 - 24);

		char lookupTable1[] = { "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[£]çç€!ççç%&'()*+,-.^0123456789:;<=>?/abcdefghijklmnopqrstuvwxyz ççççççç" };


		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, 48 *20 + 25, 48*25+35 );
		app->entityMaster->CreateEntity(EntityType::ENEMY_SNAKE, 48 * 30 , 48 * 13+35);
		marginX = 48 * 11;
		marginY = 48 * 7;
		checkpoint = app->audio->LoadFx("Assets/audio/fx/checkpoint.wav");

		//player lifes
		SDL_Rect rec_panel = { 0,188,340,124 };
		UI_player_lifes = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 0, 5, 0, 0);
		UI_player_lifes->sprite = app->UI_handler->spritesheet;
		UI_player_lifes->rec_sprite = rec_panel;

		//skill icon
		rec_panel = { 368, 200, 80, 80 };
		UI_player_skill_icon = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 16, 150, 0, 0);
		UI_player_skill_icon->sprite = app->UI_handler->spritesheet;
		UI_player_skill_icon->rec_sprite = rec_panel;

		//player skill_bg
		rec_panel = { 260, 60, 140, 24 };
		UI_player_skill_bar_bg = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 100, 200, 0, 0);
		UI_player_skill_bar_bg->sprite = app->UI_handler->spritesheet;
		UI_player_skill_bar_bg->rec_sprite = rec_panel;

		//player skill fill
		rec_panel = { 264, 92, 132, 20 };
		UI_player_skill_bar_fill = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 100 + 4, 200 + 4, 0, 0);
		UI_player_skill_bar_fill->sprite = app->UI_handler->spritesheet;
		UI_player_skill_bar_fill->rec_sprite = rec_panel;

		////panel pause
		//rec_panel = { 0, 312, 672, 432 };
		//UI_panel_pause_menu = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 1080/2 - 672 /2, 720/2 - 432/2, 672, 432);
		//UI_panel_pause_menu->sprite = app->UI_handler->spritesheet;
		//UI_panel_pause_menu->rec_sprite = rec_panel;

		////panel exit game
		//rec_panel = { 692, 4, 268, 48 };
		//UI_panel_pause_menu_exit_game = app->UI_handler->CreatePanel(app->UI_handler->spritesheet, 1080 / 2 + 14, 720 / 2 + 432/4 + 128, 268, 48);
		//UI_panel_pause_menu_exit_game->sprite = app->UI_handler->spritesheet;
		//UI_panel_pause_menu_exit_game->rec_sprite = rec_panel;

		//pause open button
		rec_panel = { 468, 4, 48, 48 };
		UI_button_open_pause_menu = app->UI_handler->CreateButton(app->UI_handler->spritesheet, 1080 - 48 - 14, 14, 48, 48);
		UI_button_open_pause_menu->sprite = app->UI_handler->spritesheet;
		UI_button_open_pause_menu->rec_sprite = rec_panel;
		UI_button_open_pause_menu->action = ACTION_PAUSE_OPEN;

		////pause close button
		//rec_panel = { 412, 60, 48, 48 };
		//UI_button_close_pause_menu = app->UI_handler->CreateButton(
		//	app->UI_handler->spritesheet,
		//	1080 / 2 + 584/2 -28,
		//	584/2 - 432/4,
		//	48,
		//	48);
		//UI_button_close_pause_menu->sprite = app->UI_handler->spritesheet;
		//UI_button_close_pause_menu->rec_sprite = rec_panel;
		//UI_button_close_pause_menu->action = ACTION_PAUSE_CLOSE;

		//
		//
		////exit button
		//rec_panel = { 468, 60, 48, 48 };
		//UI_button_exit_game = app->UI_handler->CreateButton(
		//	app->UI_handler->spritesheet,
		//	1080 / 2 + 584 / 2 - 28,
		//	720 / 2 + 432 / 2 - 64,
		//	48,
		//	48);
		//UI_button_exit_game->sprite = app->UI_handler->spritesheet;
		//UI_button_exit_game->rec_sprite = rec_panel;
		//UI_button_exit_game->action = ACTION_EXIT_GAME;

		////exit button confirm
		//rec_panel = { 412, 4, 48, 48 };
		//UI_button_exit_game_confirm = app->UI_handler->CreateButton(
		//	app->UI_handler->spritesheet,
		//	1080 / 2 - 584 / 2 + 584 + 4,
		//	720 / 2 + 432 / 4 + 128,
		//	48,
		//	48
		//);
		//UI_button_exit_game_confirm->sprite = app->UI_handler->spritesheet;
		//UI_button_exit_game_confirm->rec_sprite = rec_panel;
		//UI_button_exit_game_confirm->action = ACTION_EXIT_GAME_CONFIRM;

		////exit button deny
		//rec_panel = { 468, 60, 48, 48 };
		//UI_button_exit_game_deny = app->UI_handler->CreateButton(
		//	app->UI_handler->spritesheet,
		//	1080 / 2 - 584 / 2 + 584 + 48 + 16,
		//	720 / 2 + 432 / 4 + 128,
		//	48,
		//	48
		//);
		//UI_button_exit_game_deny->sprite = app->UI_handler->spritesheet;
		//UI_button_exit_game_deny->rec_sprite = rec_panel;
		//UI_button_exit_game_deny->action = ACTION_EXIT_GAME_DENY;

		////save button
		//rec_panel = { 320, 116, 132, 48 };
		//UI_button_save_game = app->UI_handler->CreateButton(
		//	app->UI_handler->spritesheet,
		//	 1080 / 2 - 584 / 2 + 584 / 4,
		//	720 / 2 + 432 / 4 + 24,
		//	132,
		//	48
		//);
		//UI_button_save_game->sprite = app->UI_handler->spritesheet;
		//UI_button_save_game->rec_sprite = rec_panel;
		//UI_button_save_game->rec_over = {468,112,140,56};
		//UI_button_save_game->rec_hold = { 472,172,132,48 };
		//UI_button_save_game->action = ACTION_SAVE_GAME;

		////load button
		//rec_panel = { 180, 116, 132, 48 };
		//UI_button_load_game = app->UI_handler->CreateButton(
		//	app->UI_handler->spritesheet,
		//	1080 / 2 + 584 / 2 - 584 / 4 - 132,
		//	720 / 2 + 432 / 4 + 24,
		//	132,
		//	48
		//);
		//UI_button_load_game->sprite = app->UI_handler->spritesheet;
		//UI_button_load_game->rec_sprite = rec_panel;
		//UI_button_load_game->rec_over = { 468,112,140,56 };
		//UI_button_load_game->rec_hold = { 472,172,132,48 };
		//UI_button_load_game->action = ACTION_LOAD_GAME;

		////sfx volume slider
		//rec_panel = { 0, 60, 256, 24 };
		//UI_slider_sfx_vol = app->UI_handler->CreateSlider(450, 420, 128, app->UI_handler->spritesheet, { 0 }, { 0 }, { 0 }, { 0 });
		//UI_slider_sfx_vol->rec_sprite = rec_panel;
		//rec_panel = { 4, 92, 244, 16 };
		//UI_slider_sfx_vol->rec_body_fill = rec_panel;
		//rec_panel = { 0, 0, 116, 60 };
		//UI_slider_sfx_vol->rec_slider_hold = rec_panel;
		//rec_panel = { 120, 0, 120, 60 };
		//UI_slider_sfx_vol->rec_slider = rec_panel;
		//UI_slider_sfx_vol->w = 256;
		//UI_slider_sfx_vol->h = 24;
		//UI_slider_sfx_vol->set = SET_SFX_VOLUME;

		////global audio slider
		//rec_panel = { 0, 60, 256, 24 };
		//UI_slider_global_audio = app->UI_handler->CreateSlider(450, 320, 100, app->UI_handler->spritesheet, { 0 }, { 0 }, { 0 }, { 0 });
		//UI_slider_global_audio->rec_sprite = rec_panel;
		//rec_panel = { 4, 92, 244, 16 };
		//UI_slider_global_audio->rec_body_fill = rec_panel;
		//rec_panel = { 0, 0, 116, 60 };
		//UI_slider_global_audio->rec_slider_hold = rec_panel;
		//rec_panel = { 120, 0, 120, 60 };
		//UI_slider_global_audio->rec_slider = rec_panel;
		//UI_slider_global_audio->w = 256;
		//UI_slider_global_audio->h = 24;
		//UI_slider_global_audio->set = SET_GLOBAL_VOLUME;


		////music volume slider
		//rec_panel = { 0, 60, 256, 24 };
		//UI_slider_music_vol = app->UI_handler->CreateSlider(450, 370, 128, app->UI_handler->spritesheet, { 0 }, { 0 }, { 0 }, { 0 });
		//UI_slider_music_vol->rec_sprite = rec_panel;
		//rec_panel = { 4, 92, 244, 16 };
		//UI_slider_music_vol->rec_body_fill = rec_panel;
		//rec_panel = { 0, 0, 116, 60 };
		//UI_slider_music_vol->rec_slider_hold = rec_panel;
		//rec_panel = { 120, 0, 120, 60 };
		//UI_slider_music_vol->rec_slider = rec_panel;
		//UI_slider_music_vol->w = 256;
		//UI_slider_music_vol->h = 24;
		//UI_slider_music_vol->set = SET_MUSIC_VOLUME;

		//176, 120, 140, 24
		//180, 124, 132, 20

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
	currentTime += 16;

	
	
	

	switch (state)
	{
	case INTRO:
	{
		/*if ((lastTime + 300 > currentTime)&&(!loadingScreenActive))
		{
			app->render->DrawTexture(intro01, -50, 500);
		}
		else if ((lastTime + 300 < currentTime) && (lastTime + 600 > currentTime) && (!loadingScreenActive))
		{
			app->render->DrawTexture(intro02, -50, 500);
		}*/

		if (!loadingScreenActive)
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
					app->scene->UI_button_settings->SetActive(false);
					app->audio->clearAudio();
					state = GAMEPLAY;
					Start();
					app->player->Start();

				}

			}
		}
		app->fonts->DrawText(14, 720 - 24 - 20, font1_white_1, "Made by Pol Farreras");
		app->fonts->DrawText(14, 720 - 24, font1_white_1, "& Brandon Arandia");


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

		playerX = METERS_TO_PIXELS(app->player->GetColHitbox()->body->GetPosition().x);
		playerY = METERS_TO_PIXELS(app->player->GetColHitbox()->body->GetPosition().y);

		//LOG("pos: %i, %i", playerX, playerY);

		if (!freeCam)
		{
			uint w, h;
			app->win->GetWindowSize(w, h);
			//app->render->camera.x = -(playerX );

			if (playerX > marginX)
			{
				app->render->camera.x = -(playerX - marginX);
			}
			

			if (playerX >= 48 * 100 - w + marginX)
			{
				
				app->render->camera.x = -1*(48 * 100 - (w));
			}

			if (playerY > marginY)
			{
				app->render->camera.y = -(playerY - marginY);
			}


			if (playerY >= 30 * 48 - h + marginY)
			{

				app->render->camera.y = -1*(30 * 48 - (h));
			}


			//if (playerX > marginX && playerX < 100 * 48)
			//{
			//	app->render->camera.x = -playerX * 48 * 1.042f + marginX * 48;
			//}



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


		
		//LOG("PLAYER POS = %f", app->player->GetColHitbox()->body->GetPosition().y);
		//LOG("PLAYER POS = %f", app->player->GetColHitbox()->body->GetPosition().x);

		if ((flagPast == true)&&(!gameCheckpoint))
		{
			app->audio->PlayFx(checkpoint);
			app->SaveGameRequest();
			
			gameCheckpoint = true;
			
		}
		if (((app->player->GetColHitbox()->body->GetPosition().x > 59) && (app->player->GetColHitbox()->body->GetPosition().x < 63)) && ((app->player->GetColHitbox()->body->GetPosition().y > 15)) && (app->player->GetColHitbox()->body->GetPosition().y < 16))
		{
			
			flagPast = true;

		}
		if ((playerX > 99 * 48)||(app->player->GetPlayerLifes()<=0))
		{
			if (app->player->GetPlayerLifes() <= 0) whichEnding = false;
			
					
			state = END;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			//Start();

			if (whichEnding)
			{
				if (sound)
				{
					Mix_CloseAudio();
					Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
					app->audio->PlayFx(goodEnding);
					sound = false;
				}

			}
			else
			{
				if (sound)
				{
					Mix_CloseAudio();
					Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
					app->audio->PlayFx(badEnding);
					sound = false;
				}
			}
			
			

		}
		if (flagPast)
		{
			app->render->DrawTexture(flag, 62 * 48, 15 * 48, &r_flag[0]);
		}
		else app->render->DrawTexture(flag, 62*48, 15*48, &r_flag[1]);

		

		//app->fonts->DrawText(20, 100, font1_gold_1, "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[£]↑→€!çç#$%&'%()*+,-.^0123456789:;<=>?/abcdefghijklmnopqrstuvwxyz çççç");
		//app->fonts->DrawText(20, 150, font1_gold_1, "[£]↑→€!çç$%&'()*+,-.");
		//app->fonts->DrawText(20, 200, font1_gold_1, "^0123456789:;<=>abcdef");
		//app->fonts->DrawText(20, 250, font1_gold_1, "ghijklmnopqrstuvwxyz");
		//app->fonts->DrawText(20, 300, font1_gold_1, "3 Attempts left!");

			
	}break;
	case END:
	{
		app->entityMaster->DestroyAllEnemies();

		//app->render->DrawTexture(loadingScreen, 55, 800,NULL,SDL_FLIP_NONE,0);
		if (whichEnding)
		{
			
			app->render->DrawTexture(goodEndingScreen, 55, 100);
			
		}
		else
		{
			
			app->render->DrawTexture(badEndingScreen, 0, 20);
		}
	
	}break;
	}
	
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	switch (state)
	{
	case INTRO:
	{


		if (!menuOpen)
		{
			UI_panel_pause_menu->SetActive(false);
		}

		if (UI_panel_pause_menu->IsActive())
		{
			app->fonts->DrawText(1080 / 3 + 50, 200 - 24, font1_gold_2, "Settings");


			char val[4];

			app->fonts->DrawText(310, 320 - 8, font1_black_1, "Global");
			app->fonts->DrawText(310, 320 + 8, font1_black_1, "Vol:");

			sprintf_s(val, "%i", UI_slider_global_audio->GetValue());

			app->fonts->DrawText(310 + 4 * 16, 320 + 8, font1_black_1, val);
			app->fonts->DrawText(310 + (4 + strlen(val)) * 16, 320 + 8, font1_black_1, "%");

			app->fonts->DrawText(310, 370 - 8, font1_black_1, "Music");
			app->fonts->DrawText(310, 370 + 8, font1_black_1, "Vol:");

			sprintf_s(val, "%i", (int)((float)UI_slider_music_vol->GetValue() / 128.f * 100));

			app->fonts->DrawText(310 + 4 * 16, 370 + 8, font1_black_1, val);
			app->fonts->DrawText(310 + (4 + strlen(val)) * 16, 370 + 8, font1_black_1, "%");

			app->fonts->DrawText(310, 420 - 8, font1_black_1, "SFX");
			app->fonts->DrawText(310, 420 + 8, font1_black_1, "Vol:");

			sprintf_s(val, "%i", (int)((float)UI_slider_sfx_vol->GetValue() / 128.f * 100));

			app->fonts->DrawText(310 + 4 * 16, 420 + 8, font1_black_1, val);
			app->fonts->DrawText(310 + (4 + strlen(val)) * 16, 420 + 8, font1_black_1, "%");



			UI_button_close_pause_menu->SetActive(true);
			//UI_button_exit_game->SetActive(true);
			//UI_button_load_game->SetActive(true);
			//UI_button_save_game->SetActive(true);
			UI_slider_global_audio->SetActive(true);
			UI_slider_music_vol->SetActive(true);
			UI_slider_sfx_vol->SetActive(true);

		}
		else
		{
			UI_panel_pause_menu->SetActive(false);
			UI_button_close_pause_menu->SetActive(false);
			
			if (!loadingScreenActive)
				UI_button_settings->SetActive(true);
			else {
				UI_button_settings->SetActive(false);

			}

			UI_panel_pause_menu_exit_game->SetActive(false);
			UI_button_exit_game->SetActive(false);
			UI_button_exit_game_confirm->SetActive(false);
			UI_button_exit_game_deny->SetActive(false);
			UI_button_load_game->SetActive(false);
			UI_button_save_game->SetActive(false);
			UI_slider_global_audio->SetActive(false);
			UI_slider_music_vol->SetActive(false);
			UI_slider_sfx_vol->SetActive(false);



		}
	}
		break;
	case GAMEPLAY:
	{




		if (app->GameIsPaused())
		{
			app->fonts->DrawText(1080 / 3, 200 - 24, font1_gold_2, "Game Paused");


			char val[4];

			app->fonts->DrawText(310, 320 - 8, font1_black_1, "Global");
			app->fonts->DrawText(310, 320 + 8, font1_black_1, "Vol:");
			
			sprintf_s(val, "%i", UI_slider_global_audio->GetValue());

			app->fonts->DrawText(310 + 4 * 16, 320 + 8, font1_black_1, val);
			app->fonts->DrawText(310 + (4 + strlen(val))*16, 320 + 8, font1_black_1, "%");

			app->fonts->DrawText(310, 370 - 8, font1_black_1, "Music");
			app->fonts->DrawText(310, 370 + 8, font1_black_1, "Vol:");

			sprintf_s(val, "%i", (int)((float)UI_slider_music_vol->GetValue() / 128.f * 100));

			app->fonts->DrawText(310 + 4 * 16, 370 + 8, font1_black_1, val);
			app->fonts->DrawText(310 + (4 + strlen(val)) * 16, 370 + 8, font1_black_1, "%");

			app->fonts->DrawText(310, 420 - 8, font1_black_1, "SFX");
			app->fonts->DrawText(310, 420 + 8, font1_black_1, "Vol:");
			
			sprintf_s(val, "%i", (int)((float)UI_slider_sfx_vol->GetValue() / 128.f * 100));

			app->fonts->DrawText(310 + 4 * 16, 420 + 8, font1_black_1, val);
			app->fonts->DrawText(310 + (4 + strlen(val)) * 16, 420 + 8, font1_black_1, "%");


			UI_panel_pause_menu->SetActive(true);
			UI_button_close_pause_menu->SetActive(true);
			UI_button_open_pause_menu->SetActive(false);
			UI_button_exit_game->SetActive(true);
			UI_button_load_game->SetActive(true);
			UI_button_save_game->SetActive(true);
			UI_slider_global_audio->SetActive(true);
			UI_slider_music_vol->SetActive(true);
			UI_slider_sfx_vol->SetActive(true);
			 
		}
		else
		{
			UI_panel_pause_menu->SetActive(false);
			UI_button_close_pause_menu->SetActive(false);
			UI_button_open_pause_menu->SetActive(true);
			UI_panel_pause_menu_exit_game->SetActive(false);
			UI_button_exit_game->SetActive(false);
			UI_button_exit_game_confirm->SetActive(false);
			UI_button_exit_game_deny->SetActive(false);
			UI_button_load_game->SetActive(false);
			UI_button_save_game->SetActive(false);
			UI_slider_global_audio->SetActive(false);
			UI_slider_music_vol->SetActive(false);
			UI_slider_sfx_vol->SetActive(false);


		}
	}
		break;
	case END:
		break;
	case NONE:
		break;
	default:
		break;
	}

	

	//
	//

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
