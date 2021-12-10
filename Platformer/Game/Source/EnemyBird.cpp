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


EnemyBird::EnemyBird(b2Vec2 startPosition, int health) : Module()
{
	name.Create("enemyBird");

	spawnPosition = startPosition;
	iPoint temp((int)fabs(spawnPosition.x), (int)fabs(spawnPosition.y));
	spawnPos = temp;
	spawnPosMap = app->map->WorldToMap(temp.x, temp.y);
	Hitbox = app->physics->CreateCircle(spawnPosition.x, spawnPosition.y, 10);
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
	maxDistanceAgroActive = 14;

	agroSpeed.x = 3.f;
	agroSpeed.y = 3.f;
	calmSpeed.x = 1.f;
	calmSpeed.x = 1.f;
	startPosMargin = 48;
	currentSpeed.x = 0;
	currentSpeed.y = 0;

	//initial values
	checkTimer = 0;
	checkTimerAgro = 0;
	maxDistanceAgro = 0;


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
	LOG("agro distance %i", maxDistanceAgro);

	//LOG("agro %i", agroTowardsPlayer);
	//LOG("distance: %i", CheckDistanceToPhysBody(app->player->GetColHitbox()));
	iPoint myPosMap(app->map->WorldToMap(myPosWorld.x, myPosWorld.y));
	iPoint playerPosMap(app->map->WorldToMap(playerPosWorld.x, playerPosWorld.y));

	if (agroTowardsPlayer)
	{
		if (checkTimerAgro == posCheckTimeAgro)
		{
			LOG("agro timer working");
			checkTimer = 0;
			app->pathfinding->CreatePath(myPosMap, playerPosMap);

			LOG("mypos: %i, %i", myPosMap.x, myPosMap.y);
			LOG("playerpos: %i, %i", playerPosMap.x, playerPosMap.y);



			

			checkTimerAgro = 0;
		}
		checkTimerAgro++;

		//LOG("target pos: %i, %i", nextMovePos.x, nextMovePos.y);

	}
	else
	{
		if (checkTimer == posCheckTime)
		{
			LOG("timer working");
			checkTimerAgro = 0;




			if (!Between(METERS_TO_PIXELS(Hitbox->body->GetPosition().x),spawnPosition.x - startPosMargin, spawnPosition.x + startPosMargin)
				&& !Between(METERS_TO_PIXELS(Hitbox->body->GetPosition().y), spawnPosition.y - startPosMargin, spawnPosition.y + startPosMargin))
			{
				app->pathfinding->CreatePath(myPosMap, spawnPosMap);
			}



			LOG("mypos: %i, %i", myPosMap.x, myPosMap.y);
			
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
		currentSpeed.x = calmSpeed.x * direction.x;
		currentSpeed.y = calmSpeed.y * direction.y;
	}
	LOG("spawn: %i, %i", nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().x), nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	Hitbox->body->SetLinearVelocity(currentSpeed);

	//LOG("spawn phys: %f, %f", spawnPosition.x, spawnPosition.y);
	//LOG("spawn: %i, %i", spawnPos.x, spawnPos.y);
	//LOG("spawn map: %i, %i", spawnPosMap.x, spawnPosMap.y);


	//draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	SDL_Rect pathRect = { 48 * 2,0,48,48 };

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(app->enemyMaster->texturePath, pos.x, pos.y, &pathRect);
	}
	
	app->render->DrawTexture(
		app->enemyMaster->textureBird,
		METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 8,
		METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 8,
		&app->enemyMaster->birdTemp
	);


	return true;
}

bool EnemyBird::LoadState(pugi::xml_node& data)
{

	//startPosX = data.child("startPos").attribute("x").as_float(0);
	//startPosY = data.child("startPos").attribute("y").as_float(0);

	//b2Vec2 v = { PIXEL_TO_METERS(startPosX), PIXEL_TO_METERS(startPosY) };
	//ColHitbox->body->SetTransform(v, 0);

	return true;
}


bool EnemyBird::SaveState(pugi::xml_node& data) const
{

	//LOG("saving camera pos");
	//data.child("startPos").attribute("x").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x));
	//data.child("startPos").attribute("y").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().y));
	return true;
}
