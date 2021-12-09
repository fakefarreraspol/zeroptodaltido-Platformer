#ifndef __ENEMY_HANDLER_H__
#define __ENEMY_HANDLER_H__

#include "Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "App.h"
#include "Physics.h"
#include <math.h>
#include "SDL/include/SDL.h"
#include "Render.h"
#include "Animation.h"
#include "Log.h"

//enemies
#include "EnemySnake.h"
#include "EnemyBird.h"
#include "EnemyMushroom.h"

enum EnemyType
{
	ENEMY_MUSHROOM = 0,
	ENEMY_SNAKE,
	ENEMY_BIRD
};

class EnemyHandler : public Module
{
public:
	EnemyHandler();
	virtual ~EnemyHandler();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();
	void CreateEnemy(EnemyType type, int x, int y);

	p2List<EnemyMushroom*> enemiesMushroom;
	p2List<EnemySnake*> enemiesSnake;
	p2List<EnemyBird*> enemiesBird;



	SDL_Rect mushroomTemp = { 0,0,48,48 };
	SDL_Rect birdTemp = { 48,16,16,16};

	SDL_Texture* attention;
	SDL_Texture* textureMushroom;
	SDL_Texture* textureBird;
	SDL_Texture* texturePath;
private:

	bool spawnEnemyRequest = false;
	EnemyType requestType;
	iPoint requestPoint;

	
	

};

#endif // __ENEMY_HANDLER_H__
