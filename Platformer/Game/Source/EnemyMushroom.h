#ifndef __ENEMY_MUSHROOM_H__
#define __ENEMY_MUSHROOM_H__

#include "Entity.h"
#include "p2Point.h"
#include "p2List.h"
#include "App.h"
#include "Physics.h"
#include <math.h>
#include "SDL/include/SDL.h"
#include "Render.h"
#include "Animation.h"
#include "Log.h"
#include "EntityHandler.h"
#include "Map.h"



class EnemyMushroom : public Entity
{
public:
	EnemyMushroom(b2Vec2 startPosition, int health);

	virtual ~EnemyMushroom();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();
	
	PhysBody* GetPhysBody() const
	{
		return Hitbox;
	}

	void setPosition(int x, int y)
	{
		b2Vec2 v(x, y);
		Hitbox->body->SetTransform(v, 0);
	}


	int CheckDistanceToPhysBody(PhysBody* PhysPos)
	{
		b2Vec2 dist = PhysPos->body->GetPosition() - Hitbox->body->GetPosition();

		return (abs(dist.x) + abs(dist.y));
	}

	void DoDamage(int damage);
	

private:

	int lastTime = 0;
	int currentTime = 0;
	SDL_Rect r_mushroomIdle[9];
	SDL_Rect r_mushroomWalk[4];
	bool isMushroomWalking = false;
	bool lastMushroomDirection = true;
	unsigned int currentMushroomWalk = 0;
	unsigned int currentMushroomIdle = 0;
	PhysBody* Hitbox;
	b2Vec2 spawnPosition;

	int health;
	SDL_Rect mushroomTemp;
	b2Vec2 speed;
	b2Vec2 currentSpeed;

	//navegation AI

	iPoint currentMapTilePosition;
	iPoint lastMapTilePosition;
	bool direction;

	int posCheckTime;
	int checkTimer;


	int arTilesToCheck[60] = {

		6,7,39,40,42,43,45,46,
		58,59,60,61,62,63,64,65,
		77,78,79,80,81,82,83,84,
		85,86,87,88,96,97,100,101,
		102,103,104,105,206,107,
		108,109,116,117,119,120,

		5,8,22,23,28,29,30,31,32,
		33,47,48,49,50,51,52
	};

	bool agroTowardsPlayer;
	int maxDistanceAgro;


};

#endif // __ENEMY_MUSHROOM_H__
