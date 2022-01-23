#ifndef __ENEMY_SNAKE_H__
#define __ENEMY_SNAKE_H__

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


class EnemySnake : public Entity
{
public:
	EnemySnake(b2Vec2 startPosition, int health);
	virtual ~EnemySnake();

	// Called before render is available
	bool Awake();
	
	int CheckDistanceToPhysBody(PhysBody* PhysPos)
	{
		b2Vec2 dist = PhysPos->body->GetPosition() - Hitbox->body->GetPosition();

		return (abs(dist.x) + abs(dist.y));
	}

	void SnakeAttack();
	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();

	int lastTime = 0;
	int currentTime = 0;
	bool snakeDirection = true;
	int snakeAnim = 0;
	bool snakeAgro = false;
	int snakeAttackAnim = 0;
	int snakeAttackTime = 0;

	PhysBody* GetPhysBody() const
	{
		return Hitbox;
	}

	void setPosition(int x, int y)
	{
		b2Vec2 v(x, y);
		Hitbox->body->SetTransform(v, 0);
	}

	void DoDamage(int damage);

private:
	p2List<PhysBody*> acidThrown;
	SDL_Rect r_snakeIdle[6];
	SDL_Rect r_snakeAttack[4];
	PhysBody* Hitbox;
	PhysBody* acidBox;
	b2Vec2 spawnPosition;
	const int maxDistanceAgroBase = 13;
	bool lastAcidDirection = true;
	int health;
	int attackCooldown = 0;
	bool acidOnMap = false;
	bool PhysBodyIsInMap(PhysBody* phys)
	{
		iPoint positionInMap(
			app->map->WorldToMap(
				METERS_TO_PIXELS(phys->body->GetPosition().x),
				METERS_TO_PIXELS(phys->body->GetPosition().y)
			)
		);

		return app->pathfinding->CheckBoundaries(positionInMap);

	}
};

#endif // __ENEMY_SNAKE_H__
