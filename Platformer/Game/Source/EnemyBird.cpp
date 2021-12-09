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
	spawnPosition = startPosition;
	name.Create("enemyBird");
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
	posCheckTime = 15;
	
	
	
	

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
	
	iPoint myPosMap(app->map->WorldToMap(myPosWorld.x,myPosWorld.y));
	iPoint playerPosMap(app->map->WorldToMap(playerPosWorld.x, playerPosWorld.y));

	int ret = app->pathfinding->CreatePath(myPosMap, playerPosMap);

	if (checkTimer == posCheckTime)
	{
		LOG("timer working");



		checkTimer = 0;
	}
	checkTimer++;



	//draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	SDL_Rect pathRect = { 48 * 2,0,48,48 };

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(app->enemyMaster->texturePath, pos.x, pos.y, &pathRect);
	}

	LOG("paths: %i", ret);

	app->render->DrawTexture(app->enemyMaster->texturePath, myPosWorld.x, myPosWorld.y, &pathRect);
	
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