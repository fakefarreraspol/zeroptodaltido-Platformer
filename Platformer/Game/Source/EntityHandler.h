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
#include "Map.h"
#include "Pathfinding.h"

//entities
#include "Entity.h"
#include "EnemySnake.h"
#include "EnemyBird.h"
#include "RocketBanana.h"
#include "EnemyMushroom.h"
#include "Item.h"


enum EntityType
{
	ENTITY_NULL = -1,
	ENEMY_MUSHROOM,
	ENEMY_SNAKE,
	ENEMY_BIRD,
	ITEM_BANANA,
	ROCKET_BANANA
};

class Item;

class EntityHandler : public Module
{
public:
	EntityHandler();
	virtual ~EntityHandler();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();
	void CreateEntity(enum EntityType type, int x, int y);
	void DestroyEnemy(b2Body* body);
	void DamageEnemy(b2Body* body, int damage);
	void HandleEnemyDespawn();
	void DestroyAllEnemies();

	EntityType GetEntityType(b2Body*) const;
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

	
	
	p2List<Entity*> allEntities;

	p2List<EnemyMushroom*> enemiesMushroom;
	p2List<EnemySnake*> enemiesSnake;
	p2List<EnemyBird*> enemiesBird;
	p2List<RocketBanana*> rockets;
	
	p2List<Item*> items;


	SDL_Texture* attention;
	SDL_Texture* textureMushroom;
	SDL_Texture* textureBird;
	SDL_Texture* textureSnake;
	SDL_Texture* texturePath;

	PhysBody  GetNearestEnemy(PhysBody* Character);
	
private:

	
	int all_ids;
	

	
	

};

#endif // __ENEMY_HANDLER_H__
