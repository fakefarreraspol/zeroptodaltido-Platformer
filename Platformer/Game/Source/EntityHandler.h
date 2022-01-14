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
#include "EnemySnake.h"
#include "EnemyBird.h"
#include "EnemyMushroom.h"
#include "Item.h"

enum EntityType
{
	ENEMY_MUSHROOM = 0,
	ENEMY_SNAKE,
	ENEMY_BIRD,
	ITEM_BANANA
};

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
	void CreateEnemy(enum EntityType type, int x, int y);
	void DestroyEnemy(PhysBody* body);
	void DamageEnemy(b2Body* body, int damage);
	void HandleEnemyDespawn();
	void DestroyAllEnemies();

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
	
	p2List<EnemyMushroom*> enemiesMushroom;
	p2List<EnemySnake*> enemiesSnake;
	p2List<EnemyBird*> enemiesBird;
	p2List<Item*> items;


	SDL_Texture* attention;
	SDL_Texture* textureMushroom;
	SDL_Texture* textureBird;
	SDL_Texture* textureSnake;
	SDL_Texture* texturePath;


	
private:

	
	
	

	
	

};

#endif // __ENEMY_HANDLER_H__
