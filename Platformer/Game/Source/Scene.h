#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Physics.h"
#include "ModuleFonts.h"
#include "UIHandler.h"

struct SDL_Texture;


enum GameState
{
	INTRO = 0,
	GAMEPLAY,
	END,
	NONE
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();
	uint jungleMusic = 0;
	uint titleMusic = 0;
	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	SDL_Texture* bTexture = nullptr;

	int playerX = 0;
	int playerY = 0;
	bool freeCam = false;
	

	GameState state = INTRO;
	//GameState state = GAMEPLAY;

	int font1_gold_1;
	int font1_black_1;
	int font1_white_1;

	int font1_black_2;
	int font1_gold_2;
	int font1_white_2;

	int font1_gold_3;
	int font1_black_3;
	int font1_white_3;

	//start
	UI_Panel* UI_panel_title;

	UI_Button* UI_button_start_game;
	UI_Button* UI_button_quit_game;

	UI_Button* UI_button_settings;

	//gameplay
	UI_Panel* UI_player_lifes;
	UI_Panel* UI_player_skill_icon;
	UI_Panel* UI_player_skill_bar_bg;
	UI_Panel* UI_player_skill_bar_fill;

	UI_Panel* UI_panel_pause_menu;
	UI_Panel* UI_panel_pause_menu_exit_game;

	UI_Button* UI_button_open_pause_menu;
	UI_Button* UI_button_close_pause_menu;
	UI_Button* UI_button_exit_game;
	UI_Button* UI_button_exit_game_confirm;
	UI_Button* UI_button_exit_game_deny;

	UI_Button* UI_button_save_game;
	UI_Button* UI_button_load_game;

	UI_Slider* UI_slider_global_audio;
	UI_Slider* UI_slider_music_vol;
	UI_Slider* UI_slider_sfx_vol;

	bool loadingScreenActive = false;
	int currentTime = 0;
	int lastTime = 0;

	bool menuOpen = false;

private:

	


	p2List<PhysBody*> mushrooms;
	
	
	int marginX;
	int marginY;
	
	struct startMushroom
	{
		PhysBody* body;
		int x;
		int y;
	};
	PhysBody* EnemyMushroomHitbox;
	SDL_Texture* texBackground;
	SDL_Texture* loadingScreen;
	SDL_Texture* flag;
	SDL_Rect r_flag[2];
	SDL_Texture* goodEndingScreen;
	SDL_Texture* badEndingScreen;
	SDL_Texture* intro01;
	SDL_Texture* intro02;
	
	uint checkpoint = 0;
	bool gameCheckpoint = false;
	bool lastIntro = false;
	bool flagPast = false;
	p2List<PhysBody*> trespasableElements;
	
	uint goodEnding = 0;
	uint badEnding = 0;
	bool whichEnding = true;
	bool sound = true;
	
	

	


	int squareGround[44] = {
		6,7,39,40,42,43,45,46,
		58,59,60,61,62,63,64,65,
		77,78,79,80,81,82,83,84,
		85,86,87,88,96,97,100,101,
		102,103,104,105,206,107,
		108,109,116,117,119,120 
	};

	int rightSlope_45[6] = 
	{
		0,48,
		48,48,
		48,0
	};
	int leftSlope_45[6] =
	{
		0,0,
		0,48,
		48,48
	};

	int rightSlope_30_1[6] =
	{
		0,48,
		48,48,
		48,32

	};
	int leftSlope_30_1[6] =
	{
		0,0,
		0,16,
		48,16
	};
		

	int rightSlope_30_2[6] =
	{
		0,32,
		48,32,
		48,16
	};

	int leftSlope_30_2[6] =
	{
		0,16,
		48,32,
		0,32
	};

	int rightSlope_30_3[6] =
	{
		0,16,
		48,16,
		48,0
	};

	int leftSlope_30_3[6] =
	{
		0,32,
		48,48,
		0,48
	};

	

};

#endif // __SCENE_H__