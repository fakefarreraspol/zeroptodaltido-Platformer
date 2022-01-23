#ifndef __ENEMY_BIRD_H__
#define __ENEMY_BIRD_H__


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
#include "Pathfinding.h"


class EnemyBird : public Entity
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

	int CheckDistanceToPhysBody(PhysBody* PhysPos)
	{
		b2Vec2 dist = PhysPos->body->GetPosition() - Hitbox->body->GetPosition();

		return (abs(dist.x) + abs(dist.y));
	}

	void DoDamage(int damage);

private:

	int RandomRange(int value01, int value02) {
		if (value01 > value02) {

			int i = value01;
			value01 = value02;
			value02 = i;

		}
		return(rand() % (value02 - value01 + 1) + value01);
	}

	bool Between(int value, int a, int b)
	{
		return (value >= a && value <= b);
	}

	PhysBody* Hitbox;
	b2Vec2 spawnPosition;
	iPoint spawnPos;
	iPoint spawnPosMap;
	SDL_Rect r_bird[8];
	unsigned int birdAnim = 0;
	bool birdDirection = true;
	int health;
	int lastTime = 0;
	int currentTime = 0;
	b2Vec2 agroSpeed;
	b2Vec2 calmSpeed;
	b2Vec2 currentSpeed;

	//navegation AI

	iPoint nextMovePos;

	iPoint currentMapTilePosition;
	iPoint lastMapTilePosition;
	bool direction;

	int posCheckTime;
	int checkTimer;
	int posCheckTimeAgro;
	int checkTimerAgro;
	int startPosMargin;


	bool agroTowardsPlayer;
	int maxDistanceAgroBase;
	int maxDistanceAgroActive;
	int maxDistanceAgro;
	bool inSpawnPos = true;


};

#endif // __ENEMY_BIRD_H__
