#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Physics.h"

struct SDL_Texture;

class Scene2:public Module
{
public:

	Scene2();

	// Destructor
	virtual ~Scene2();

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
	void createMushroom(int x, int y, int size)
	{
		startMushroom temp;
		temp.x = x;
		temp.y = y;
		temp.body = app->physics->CreateCircle(x, y, size);
		mushrooms.add(temp.body);
	}
	int playerX = 0;
	int playerY = 0;
	bool freeCam = false;
	enum GameState
	{
		INTRO = 0,
		GAMEPLAY,
		END
	};

	GameState state = GAMEPLAY;
	//GameState state = GAMEPLAY;
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
	int currentTime = 0;
	int lastTime = 0;
	bool loadingScreenActive = false;
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

#endif // __SCENE2_H__
