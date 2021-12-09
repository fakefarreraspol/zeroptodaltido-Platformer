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
#include "EnemyMushroom.h"
#include "App.h"

EnemyMushroom::EnemyMushroom(b2Vec2 startPosition, int health) : Module()
{
	spawnPosition = startPosition;
	name.Create("enemyMushroom");
	Hitbox = app->physics->CreateCircle(spawnPosition.x, spawnPosition.y, 20);
	Hitbox->body->SetGravityScale(10);
	
	this->health = health;
	lastMapTilePosition = app->map->WorldToMap(startPosition.x, startPosition.y);

}


EnemyMushroom::~EnemyMushroom()
{}

bool EnemyMushroom::Awake()
{
	return true;
}

bool EnemyMushroom::Start()
{
	Hitbox->body->ResetMassData();

	//permanent values
	posCheckTime = 15;
	speed.x = 3.f;
	speed.y = 0.f;
	maxDistanceAgro = 5;


	//initial values
	currentSpeed = speed;
	direction = true;
	Hitbox->body->SetLinearVelocity(currentSpeed);
	agroTowardsPlayer = false;
	checkTimer = 0;

	return true;
}
bool EnemyMushroom::CleanUp()
{
	LOG("Unloading player");

	return true;
}

bool EnemyMushroom::Update(float dt)
{
	//navegation AI


	if (checkTimer == posCheckTime)
	{
		currentMapTilePosition = app->map->WorldToMap(
			METERS_TO_PIXELS(Hitbox->body->GetPosition().x),
			METERS_TO_PIXELS(Hitbox->body->GetPosition().y)
		);

		if (currentMapTilePosition != lastMapTilePosition)
		{
			lastMapTilePosition = currentMapTilePosition;
		}
		iPoint worldPosIpoint(app->map->MapToWorld(lastMapTilePosition.x, lastMapTilePosition.y));

		uint leftCheckPos = app->map->mapData.maplayers.start->data->Get(lastMapTilePosition.x - 1, lastMapTilePosition.y);
		uint leftDownCheckPos = app->map->mapData.maplayers.start->data->Get(lastMapTilePosition.x - 1, lastMapTilePosition.y + 1);

		uint rightCheckPos = app->map->mapData.maplayers.start->data->Get(lastMapTilePosition.x + 1, lastMapTilePosition.y);
		uint rightDownCheckPos = app->map->mapData.maplayers.start->data->Get(lastMapTilePosition.x + 1, lastMapTilePosition.y + 1);

		bool leftCheck = false;
		bool leftDownCheck = false;

		bool rightCheck = false;
		bool rightDownCheck = false;

		for (int i = 0; i < 60; i++)
		{
			if (leftCheckPos == arTilesToCheck[i]) leftCheck = true;
			if (rightCheckPos == arTilesToCheck[i]) rightCheck = true;
			if (leftDownCheckPos == arTilesToCheck[i]) leftDownCheck = true;
			if (rightDownCheckPos == arTilesToCheck[i]) rightDownCheck= true;
		}

		if (CheckDistanceToPhysBody(app->player->GetColHitbox()) <= maxDistanceAgro)
		{
			agroTowardsPlayer = true;
		}
		else {
			agroTowardsPlayer = false;
		}

		if (!agroTowardsPlayer)
		{
			if ((leftCheck || !leftDownCheck) && direction == false)
			{

				currentSpeed = speed;
				direction = true;

			}


			if ((rightCheck || !rightDownCheck) && direction == true)
			{
				currentSpeed = -speed;
				direction = false;

			}
		}
		else
		{

			b2Vec2 v0(0,0);
			if (leftCheck && direction == false || rightCheck && direction == true)
			{
				currentSpeed = v0;
			}
			else {
				if (app->player->GetColHitbox()->body->GetPosition().x < Hitbox->body->GetPosition().x)
				{
					currentSpeed = -speed;
					direction = false;
				}

				if (app->player->GetColHitbox()->body->GetPosition().x >= Hitbox->body->GetPosition().x)
				{
					currentSpeed = speed;
					direction = true;
				}
			}
		}
		

		//LOG("distance agro: %i", CheckDistanceToPhysBody(app->player->GetColHitbox()));
		//LOG("agro: %i", CheckDistanceToPhysBody(app->player->GetColHitbox()) <= maxDistanceAgro);
		//LOG("map: %i, %i", lastMapTilePosition.x, lastMapTilePosition.y);
		//LOG("left check pos: %i", leftCheck);
		//LOG("left down check pos: %i", leftDownCheck);
		//LOG("right check pos: %i", rightCheck);
		//LOG("right down check pos: %i", rightDownCheck);


		checkTimer = 0;
	}
	checkTimer++;

	Hitbox->body->SetLinearVelocity(currentSpeed);
	//Draw
	int Yoffset = -28 + 6;
	int Xoffset = -24;

	app->render->DrawTexture(app->enemyMaster->textureMushroom,
		METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
		METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
		&app->enemyMaster->mushroomTemp);
	
	if (agroTowardsPlayer)
	{
		app->render->DrawTexture(app->enemyMaster->attention,
			METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
			METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
			NULL);
	}

	return true;
}

bool EnemyMushroom::LoadState(pugi::xml_node& data)
{
	
	//startPosX = data.child("startPos").attribute("x").as_float(0);
	//startPosY = data.child("startPos").attribute("y").as_float(0);

	//b2Vec2 v = { PIXEL_TO_METERS(startPosX), PIXEL_TO_METERS(startPosY) };
	//ColHitbox->body->SetTransform(v, 0);

	return true;
}


bool EnemyMushroom::SaveState(pugi::xml_node& data) const
{

	//LOG("saving camera pos");
	//data.child("startPos").attribute("x").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x));
	//data.child("startPos").attribute("y").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().y));
	return true;
}
