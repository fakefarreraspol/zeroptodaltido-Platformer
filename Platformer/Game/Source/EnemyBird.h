#ifndef __ENEMY_BIRD_H__
#define __ENEMY_BIRD_H__


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
#include "Pathfinding.h"


class EnemyBird : public Module
{
public:
	EnemyBird(b2Vec2 startPosition, int health);
	virtual ~EnemyBird();

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



private:

	


	PhysBody* Hitbox;
	b2Vec2 spawnPosition;

	int health;

	//navegation AI
	iPoint currentMapTilePosition;
	iPoint lastMapTilePosition;
	bool direction;

	int posCheckTime;
	int checkTimer;

	PathFinding* pathToPlayer;
	int pathIndex;


};

#endif // __ENEMY_BIRD_H__