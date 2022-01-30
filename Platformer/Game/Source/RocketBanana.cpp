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
#include "RocketBanana.h"


RocketBanana::RocketBanana(b2Vec2 startPosition, int health) : Entity()
{
	name.Create("rocket");

	spawnPosition = startPosition;
	iPoint temp((int)fabs(spawnPosition.x), (int)fabs(spawnPosition.y));
	spawnPos = temp;
	spawnPosMap = app->map->WorldToMap(temp.x, temp.y);
	Hitbox = app->physics->CreateCircle(spawnPosition.x, spawnPosition.y, 5);
	Hitbox->body->SetGravityScale(0);
	this->health = health;

}

RocketBanana::~RocketBanana()
{}

bool RocketBanana::Awake()
{
	return true;
}

bool RocketBanana::Start()
{
	Hitbox->body->ResetMassData();
	
	rocketsbody.add(Hitbox);
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


	

	//initial values
	checkTimer = 0;
	checkTimerAgro = 0;
	maxDistanceAgro = 0;
	currentSpeed.x = 0;
	currentSpeed.y = 0;


	return true;
}
bool RocketBanana::CleanUp()
{
	LOG("Unloading player");

	return true;
}

bool RocketBanana::Update(float dt)
{
	iPoint myPosWorld(
		METERS_TO_PIXELS(Hitbox->body->GetPosition().x),
		METERS_TO_PIXELS(Hitbox->body->GetPosition().y)
	);
	

	PhysBody* nearestEnemy = &app->entityMaster->GetNearestEnemy(Hitbox);


	iPoint enemyPosWorld(
		METERS_TO_PIXELS(nearestEnemy->body->GetPosition().y),
		METERS_TO_PIXELS(nearestEnemy->body->GetPosition().y)
	);

	currentTime = SDL_GetTicks();

	

	agroTowardsPlayer = true;

	//LOG("agro distance %i", maxDistanceAgro);

	//LOG("agro %i", agroTowardsPlayer);
	//LOG("distance: %i", CheckDistanceToPhysBody(app->player->GetColHitbox()));
	iPoint myPosMap(app->map->WorldToMap(myPosWorld.x, myPosWorld.y));
	iPoint enemyPosMap(app->map->WorldToMap(enemyPosWorld.x, enemyPosWorld.y));

	app->pathfinding->CreatePath(myPosMap, enemyPosMap);

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

	if (Hitbox->body->GetContactList() != nullptr)
	{
		b2Body* playerB = Hitbox->body->GetContactList()->contact->GetFixtureB()->GetBody();
		if (playerB == app->player->GetColHitbox()->body)
		{
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

	p2List_item<PhysBody*>* currentRocket = rocketsbody.getFirst();
	while (currentRocket != NULL)
	{

		if (!PhysBodyIsInMap(currentRocket->data))
		{
			app->physics->GetWorld()->DestroyBody(currentRocket->data->body);
			rocketsbody.del(currentRocket);
		}

		
		b2Body* bananaHit;
		if (currentRocket->data->body->GetContactList() != nullptr)
		{
			//LOG("true");
			bananaHit = currentRocket->data->body->GetContactList()->contact->GetFixtureA()->GetBody();

			if (bananaHit != nullptr && bananaHit != app->player->GetColHitbox()->body && bananaHit != currentRocket->data->body)
			{
				LOG("hit");
				LOG("type: %i", bananaHit->GetType());
				LOG("type dynamic: %i", b2_dynamicBody);
				//LOG("type stat: %i", b2_staticBody);
				//LOG("type kin: %i", b2_kinematicBody);

				app->entityMaster->DamageEnemy(bananaHit, 1);

				b2Vec2 hitForce(10.f, 0);
				b2Vec2 hitDir = bananaHit->GetLinearVelocity();
				hitDir.Normalize();
				hitForce.x *= hitDir.x;
				LOG("force %f", hitForce.x);
				LOG("dir %f", hitDir.x);
				b2Vec2 xVel = { 0,bananaHit->GetLinearVelocity().y };
				bananaHit->SetLinearVelocity(xVel);
				bananaHit->ApplyLinearImpulse(hitForce, bananaHit->GetPosition(), true);
				bananaHit->SetLinearDamping(0);

				app->physics->GetWorld()->DestroyBody(currentRocket->data->body);

				rocketsbody.del(currentRocket);
				
				app->entityMaster->rockets.del(app->entityMaster->rockets.getFirst());
				//switch (bananaHit->type)
				//{
				//
				//case TYPE_SOLID_TILE:
				//	app->physics->GetWorld()->DestroyBody(currentBanana->data->body);
				//	//banana hit sound
				//	break;
				//
				//	/*
				//	case ENEMY:
				//		damage enemy
				//		destroy banana
				//		sound
				//	
				//	*/
				//
				//default:
				//	app->physics->GetWorld()->DestroyBody(currentBanana->data->body);
				//	break;
				//}


			}
		}


		int x, y;
		currentRocket->data->GetPosition(x, y);
		

		currentRocket = currentRocket->next;
	}
		
	

	return true;
}

bool RocketBanana::LoadState(pugi::xml_node& data)
{

	spawnPos.x = data.attribute("spawnPos.x").as_int();
	spawnPos.y = data.attribute("spawnPos.y").as_int();

	b2Vec2 currentPos(data.attribute("currentPos.x").as_float(), data.attribute("currentPos.y").as_float());
	Hitbox->body->SetTransform(currentPos, 0);

	LOG("pos, %i %i", METERS_TO_PIXELS(Hitbox->body->GetPosition().x), METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	LOG("pos, %i %i", METERS_TO_PIXELS(currentPos.x), METERS_TO_PIXELS(currentPos.y));

	checkTimer = data.attribute("checkTimer").as_int();
	checkTimerAgro = data.attribute("checkTimerAgro").as_int();

	maxDistanceAgro = data.attribute("maxDistanceAgro").as_int();

	currentSpeed.x = data.attribute("currentSpeed.x").as_float();
	currentSpeed.y = data.attribute("currentSpeed.y").as_float();

	agroTowardsPlayer = data.attribute("agroTowardsPlayer").as_bool();
	health = data.attribute("health").as_int();

	return true;
}


bool RocketBanana::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node myself = data.append_child("RocketBanana");

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

void RocketBanana::DoDamage(int damage)
{
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