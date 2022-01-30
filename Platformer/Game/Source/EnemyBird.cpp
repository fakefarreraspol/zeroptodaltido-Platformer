#include "Player.h"
#include "Input.h"
#include "SDL/include/SDL.h"
#include "p2List.h"
#include "Scene.h"

#include "Physics.h"
#include "Textures.h"
#include"Render.h"
#include "Window.h"
#include "Audio.h";
#include "EnemyBird.h"


EnemyBird::EnemyBird(b2Vec2 startPosition, int health) : Entity()
{
	name.Create("enemyBird");

	spawnPosition = startPosition;
	iPoint temp((int)fabs(spawnPosition.x), (int)fabs(spawnPosition.y));
	spawnPos = temp;
	spawnPosMap = app->map->WorldToMap(temp.x, temp.y);
	Hitbox = app->physics->CreateCircle(spawnPosition.x, spawnPosition.y, 20);
	Hitbox->body->SetGravityScale(0);
	this->health = health;

}

EnemyBird::~EnemyBird()
{}

bool EnemyBird::Awake()
{
	return true;
}

bool EnemyBird::Start()
{
	Hitbox->body->ResetMassData();

	//permanent values
	posCheckTime = 8;
	posCheckTimeAgro = 15;
	maxDistanceAgroBase = 8;
	maxDistanceAgroActive = 11;

	agroSpeed.x = 3.f;
	agroSpeed.y = 3.f;
	calmSpeed.x = 3.f;
	calmSpeed.y = 3.f;
	startPosMargin = 48 + 24;
	

	r_bird[0] = { 0,16,16,16 };
	r_bird[1] = { 16,16,16,16 };
	r_bird[2] = { 16 * 2,16,16,16 };
	r_bird[3] = { 16 * 3,16,16,16 };
	r_bird[4] = { 16 * 4,16,16,16 };
	r_bird[5] = { 16 * 5,16,16,16 };
	r_bird[6] = { 16 * 6,16,16,16 };
	r_bird[7] = { 16 * 7,16,16,16 };
	
	//initial values
	checkTimer = 0;
	checkTimerAgro = 0;
	maxDistanceAgro = 0;
	currentSpeed.x = 0;
	currentSpeed.y = 0;
	

	return true;
}
bool EnemyBird::CleanUp()
{
	LOG("Unloading player");

	return true;
}

bool EnemyBird::Update(float dt)
{
	iPoint myPosWorld(
		METERS_TO_PIXELS(Hitbox->body->GetPosition().x),
		METERS_TO_PIXELS(Hitbox->body->GetPosition().y)
	);
	iPoint playerPosWorld(
		METERS_TO_PIXELS(app->player->GetColHitbox()->body->GetPosition().x),
		METERS_TO_PIXELS(app->player->GetColHitbox()->body->GetPosition().y)
	);
	
	if (!app->GameIsPaused())
	{
		currentTime += 16;
	}

	

	if (agroTowardsPlayer)
	{
		if (CheckDistanceToPhysBody(app->player->GetColHitbox()) <= maxDistanceAgroBase)
		{
			agroTowardsPlayer = true;
			maxDistanceAgro = maxDistanceAgroActive;
		}
		else {
			agroTowardsPlayer = false;
			maxDistanceAgro = maxDistanceAgroBase;
		}
	}
	else
	{
		if (CheckDistanceToPhysBody(app->player->GetColHitbox()) <= maxDistanceAgroBase)
		{
			agroTowardsPlayer = true;
			maxDistanceAgro = maxDistanceAgroActive;

		}
		else {
			agroTowardsPlayer = false;
			maxDistanceAgro = maxDistanceAgroBase;

		}
	}
	//LOG("agro distance %i", maxDistanceAgro);

	//LOG("agro %i", agroTowardsPlayer);
	//LOG("distance: %i", CheckDistanceToPhysBody(app->player->GetColHitbox()));
	iPoint myPosMap(app->map->WorldToMap(myPosWorld.x, myPosWorld.y));
	iPoint playerPosMap(app->map->WorldToMap(playerPosWorld.x, playerPosWorld.y));

	if (agroTowardsPlayer)
	{
		if (checkTimerAgro == posCheckTimeAgro)
		{
			//LOG("agro timer working");
			checkTimer = 0;
			app->pathfinding->CreatePath(myPosMap, playerPosMap);

			//LOG("mypos: %i, %i", myPosMap.x, myPosMap.y);
			//LOG("playerpos: %i, %i", playerPosMap.x, playerPosMap.y);



			

			checkTimerAgro = 0;
		}
		checkTimerAgro++;

		//LOG("target pos: %i, %i", nextMovePos.x, nextMovePos.y);

	}
	else
	{
		if (checkTimer == posCheckTime)
		{
			//LOG("timer working");
			checkTimerAgro = 0;


			if (!Between(METERS_TO_PIXELS(Hitbox->body->GetPosition().x),spawnPosition.x - startPosMargin, spawnPosition.x + startPosMargin)
				|| !Between(METERS_TO_PIXELS(Hitbox->body->GetPosition().y), spawnPosition.y - startPosMargin, spawnPosition.y + startPosMargin))
			{
				app->pathfinding->CreatePath(myPosMap, spawnPosMap);
				inSpawnPos = false;
			}
			else {
				inSpawnPos = true;

			}



			//LOG("mypos: %i, %i", myPosMap.x, myPosMap.y);
			
			checkTimer = 0;
		}
		checkTimer++;
	}

	const DynArray<iPoint>* tempPath = app->pathfinding->GetLastPath();
	if (tempPath->Count() > 2)
	{
		iPoint temp(tempPath->At(1)->x, tempPath->At(1)->y);
		nextMovePos = temp;


	}


	nextMovePos = app->map->MapToWorld(nextMovePos.x, nextMovePos.y);

	b2Vec2 direction(
		nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().x),
		nextMovePos.y + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().y)
	);

	direction.Normalize();
	if (agroTowardsPlayer)
	{
		
		currentSpeed.x = agroSpeed.x * direction.x;
		currentSpeed.y = agroSpeed.y * direction.y;
	}
	else {
		if (!inSpawnPos)
		{
			currentSpeed.x = calmSpeed.x * direction.x;
			currentSpeed.y = calmSpeed.y * direction.y;
		}
		else {
			currentSpeed.x = 0;
			currentSpeed.y = 0;
		}
	}
	//LOG("next pos world %i", nextMovePos.y + 24);
	//LOG("pos, %i", METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("res: %i", nextMovePos.y + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("res: %i", METERS_TO_PIXELS(direction.y));
	//LOG("res: %i", METERS_TO_PIXELS(calmSpeed.y));
	//LOG("res: %i", METERS_TO_PIXELS(currentSpeed.y));
	//LOG("spawn: %i, %i", nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().x), nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	Hitbox->body->SetLinearVelocity(currentSpeed);

	//LOG("spawn phys: %f, %f", spawnPosition.x, spawnPosition.y);
	//LOG("spawn: %i, %i", spawnPos.x, spawnPos.y);
	//LOG("spawn map: %i, %i", spawnPosMap.x, spawnPosMap.y);

	if (Hitbox->body->GetContactList() != NULL)
	{
		b2Body* playerB = Hitbox->body->GetContactList()->contact->GetFixtureB()->GetBody();
		if (playerB == app->player->GetColHitbox()->body)
		{
			if (!app->GameIsPaused())
				app->player->HurtGorila(1);
		}
	}

	if (currentSpeed.x >= 0)
	{
		birdDirection = true;
	}
	else
	{
		birdDirection = false;
	}

	//draw
	if (app->physics->GetDebug())
	{
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		SDL_Rect pathRect = { 48 * 2,0,48,48 };

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(app->entityMaster->texturePath, pos.x, pos.y, &pathRect);
		}
	}
	

	if ((lastTime + 100 > currentTime)&&(birdAnim<7))
	{
		if (birdDirection)
		{
			app->render->DrawTexture(
				app->entityMaster->textureBird,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
				&r_bird[birdAnim], SDL_FLIP_HORIZONTAL
			);
		}
		else
		{
			app->render->DrawTexture(
				app->entityMaster->textureBird,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
				&r_bird[birdAnim]
			);

		}
		
		
	}
	else if((lastTime + 100 < currentTime) && (birdAnim < 7))
	{
		lastTime = currentTime;
		if (birdDirection)
		{
			app->render->DrawTexture(
				app->entityMaster->textureBird,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
				&r_bird[birdAnim], SDL_FLIP_HORIZONTAL
			);
		}
		else
		{
			app->render->DrawTexture(
				app->entityMaster->textureBird,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
				&r_bird[birdAnim]
			);

		}

		birdAnim++;
	}
	else
	{
		if (birdDirection)
		{
			app->render->DrawTexture(
				app->entityMaster->textureBird,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
				&r_bird[birdAnim],SDL_FLIP_HORIZONTAL
			);
		}
		else
		{
			app->render->DrawTexture(
				app->entityMaster->textureBird,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
				&r_bird[birdAnim]
			);

		}
		birdAnim = 0;
	}
	if (agroTowardsPlayer)
	{
		app->render->DrawTexture(app->entityMaster->attention,
			METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x+10),
			METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y-20),
			NULL);
	}

	return true;
}

bool EnemyBird::LoadState(pugi::xml_node& data)
{
	
	spawnPos.x = data.attribute("spawnPos.x").as_int();
	spawnPos.y = data.attribute("spawnPos.y").as_int();

	b2Vec2 currentPos(data.attribute("currentPos.x").as_float(), data.attribute("currentPos.y").as_float());
	Hitbox->body->SetTransform(currentPos, 0);

	//LOG("pos, %i %i", METERS_TO_PIXELS(Hitbox->body->GetPosition().x), METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("pos, %i %i", METERS_TO_PIXELS(currentPos.x), METERS_TO_PIXELS(currentPos.y));

	checkTimer = data.attribute("checkTimer").as_int();
	checkTimerAgro = data.attribute("checkTimerAgro").as_int();

	maxDistanceAgro = data.attribute("maxDistanceAgro").as_int();

	currentSpeed.x = data.attribute("currentSpeed.x").as_float();
	currentSpeed.y = data.attribute("currentSpeed.y").as_float();

	agroTowardsPlayer = data.attribute("agroTowardsPlayer").as_bool();
	health = data.attribute("health").as_int();

	return true;
}


bool EnemyBird::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node myself = data.append_child("EnemyBird");

	myself.append_attribute("id").set_value(id);
	myself.append_attribute("currentSpeed.x").set_value(currentSpeed.x);
	myself.append_attribute("currentSpeed.y").set_value(currentSpeed.y);

	myself.append_attribute("spawnPos.x").set_value(spawnPos.x);
	myself.append_attribute("spawnPos.y").set_value(spawnPos.y);

	myself.append_attribute("currentPos.x").set_value(Hitbox->body->GetPosition().x);
	myself.append_attribute("currentPos.y").set_value(Hitbox->body->GetPosition().y);

	myself.append_attribute("checkTimer").set_value(checkTimer);
	myself.append_attribute("checkTimerAgro").set_value(checkTimerAgro);
	myself.append_attribute("maxDistanceAgro").set_value(maxDistanceAgro);
	
	myself.append_attribute("agroTowardsPlayer").set_value(agroTowardsPlayer);
	myself.append_attribute("health").set_value(health);
	
	
	return true;
}

void EnemyBird::DoDamage(int damage)
{
	app->player->healingCooldown -= 1000;
	if (health > 0)
	{
		health -= damage;
		app->audio->PlayFx(app->player->kick);
	}
	if (health <= 0)
	{
		app->entityMaster->DestroyEnemy(Hitbox->body);
		app->audio->PlayFx(app->player->enemy_death);
		

	}
	
}