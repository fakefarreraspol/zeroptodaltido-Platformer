#ifndef __ITEM_H__
#define __ITEM_H__

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


enum ItemType
{
	BANANA = 0,
};


class Item : public Entity
{
public:
	Item(ItemType type, b2Vec2 startPosition);
	virtual ~Item();

	// Called before render is available
	bool Awake();
	
	int CheckDistanceToPhysBody(PhysBody* PhysPos)
	{
		b2Vec2 dist = PhysPos->body->GetPosition() - Hitbox->body->GetPosition();

		return (abs(dist.x) + abs(dist.y));
	}
	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool LoadState(pugi::xml_node& data);
	bool SaveState(pugi::xml_node& data) const;
	bool CleanUp();

	int lastTime = 0;
	int currentTime = 0;

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

	ItemType type;
	iPoint spawnPosIP;
	iPoint spawnPosMapIP;

	SDL_Texture* TexBanana;
};



#endif // __ENEMY_SNAKE_H__
