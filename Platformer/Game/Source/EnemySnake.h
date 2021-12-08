#ifndef __ENEMY_SNAKE_H__
#define __ENEMY_SNAKE_H__

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
#include "EnemyHandler.h"


class EnemySnake : public Module
{
public:
	EnemySnake();
	virtual ~EnemySnake();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();

private:

	SDL_Texture* enemyTexture;
	List<PhysBody*> allEnemies;

};

#endif // __ENEMY_SNAKE_H__
