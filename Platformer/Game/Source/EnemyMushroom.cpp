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

EnemyMushroom::EnemyMushroom(b2Vec2 startPosition, int health) : Entity()
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
	speed.x = 2.f ;
	speed.y = 0.f ;
	maxDistanceAgro = 5;


	//initial values
	//currentSpeed = { speed.x * (60 * app->DeltaTime()), speed.y * (60 * app->DeltaTime()) };
	currentSpeed = speed;
	direction = true;
	Hitbox->body->SetLinearVelocity(currentSpeed);
	agroTowardsPlayer = false;
	checkTimer = 0;

	r_mushroomWalk[0] = { 0,0,48,48 };
	r_mushroomWalk[1] = { 48,0,48,48 };
	r_mushroomWalk[2] = { 48 * 2,0,48,48 };
	r_mushroomWalk[3] = { 48 * 3,0,48,48 };

	//Mushroom 01
	

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
	if (!app->GameIsPaused())
	{
		currentTime += 16;
	}
	iPoint worldPosIpoint(app->map->MapToWorld(lastMapTilePosition.x, lastMapTilePosition.y));

	//float multiplier = (60 * dt);

	//b2Vec2 newSpeed(speed.x * multiplier, speed.y * multiplier);

	//LOG("time: %f", posCheckTime * multiplier);

	if (checkTimer > posCheckTime)
	{
		currentMapTilePosition = app->map->WorldToMap(
			METERS_TO_PIXELS(Hitbox->body->GetPosition().x),
			METERS_TO_PIXELS(Hitbox->body->GetPosition().y)
		);

		if (currentMapTilePosition != lastMapTilePosition)
		{
			lastMapTilePosition = currentMapTilePosition;
		}
		

		uint leftCheckPos = 0;
		uint leftDownCheckPos = 0;

		uint rightCheckPos = 0;
		uint rightDownCheckPos = 0;

		
		if (lastMapTilePosition.y < 30)
		{
			leftCheckPos = app->map->data.layers.start->data->Get(lastMapTilePosition.x - 1, lastMapTilePosition.y);
			leftDownCheckPos = app->map->data.layers.start->data->Get(lastMapTilePosition.x - 1, lastMapTilePosition.y + 1);

			rightCheckPos = app->map->data.layers.start->data->Get(lastMapTilePosition.x + 1, lastMapTilePosition.y);
			rightDownCheckPos = app->map->data.layers.start->data->Get(lastMapTilePosition.x + 1, lastMapTilePosition.y + 1);
		}

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
		//float interval = (1 / dt);
		//float interval2 = (1 / app->DeltaTime());
		//currentSpeed.x *= interval2;
		//currentSpeed.y *= interval;
		
	}
	checkTimer++;










	

	if (worldPosIpoint.y > 30 * 48) app->entityMaster->DestroyEnemy(Hitbox->body);

	//b2Vec2 spd = { currentSpeed.x * multiplier, currentSpeed.y * multiplier };

	Hitbox->body->SetLinearVelocity(currentSpeed);
	
	//Draw
	int Yoffset = -28 + 6;
	int Xoffset = -24;

	

	if ((lastTime + 500 > currentTime)&&(currentMushroomWalk<3))
	{
		if (direction)
		{
			app->render->DrawTexture(app->entityMaster->textureMushroom,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
				&r_mushroomWalk[currentMushroomWalk]);

			if (agroTowardsPlayer)
			{
				app->render->DrawTexture(app->entityMaster->attention,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
					NULL);
			}
		}
		else
		{
			app->render->DrawTexture(app->entityMaster->textureMushroom,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
				&r_mushroomWalk[currentMushroomWalk],SDL_FLIP_HORIZONTAL);

			if (agroTowardsPlayer)
			{
				app->render->DrawTexture(app->entityMaster->attention,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
					NULL);
			}
		}
	}
	else if ((lastTime + 100 < currentTime) && (currentMushroomWalk < 3))
	{
		
		if (direction)
		{
			app->render->DrawTexture(app->entityMaster->textureMushroom,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
				&r_mushroomWalk[currentMushroomWalk]);

			if (agroTowardsPlayer)
			{
				app->render->DrawTexture(app->entityMaster->attention,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
					NULL);
			}
		}
		else
		{
			app->render->DrawTexture(app->entityMaster->textureMushroom,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
				&r_mushroomWalk[currentMushroomWalk],SDL_FLIP_HORIZONTAL);

			if (agroTowardsPlayer)
			{
				app->render->DrawTexture(app->entityMaster->attention,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
					NULL);
			}
		}
		lastTime = currentTime;
		currentMushroomWalk++;
	}
	else
	{
		if (direction)
		{
			app->render->DrawTexture(app->entityMaster->textureMushroom,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
				&r_mushroomWalk[currentMushroomWalk]);

			if (agroTowardsPlayer)
			{
				app->render->DrawTexture(app->entityMaster->attention,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
					NULL);
			}
		}
		else
		{
			app->render->DrawTexture(app->entityMaster->textureMushroom,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset,
				METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset,
				&r_mushroomWalk[currentMushroomWalk],SDL_FLIP_HORIZONTAL);

			if (agroTowardsPlayer)
			{
				app->render->DrawTexture(app->entityMaster->attention,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) + Xoffset + 25,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) + Yoffset - 5,
					NULL);
			}
		}
		currentMushroomWalk=0;
	}

	if (Hitbox->body->GetContactList() != nullptr)
	{
		b2Body* playerBox = Hitbox->body->GetContactList()->contact->GetFixtureB()->GetBody();
		if (playerBox == app->player->GetColHitbox()->body)
		{
			float margin = PIXEL_TO_METERS(40);

			if (playerBox->GetPosition().x > Hitbox->body->GetPosition().x - margin &&
				playerBox->GetPosition().x < Hitbox->body->GetPosition().x + margin &&
				playerBox->GetPosition().y < Hitbox->body->GetPosition().y)
			{
				app->entityMaster->DestroyEnemy(Hitbox->body);
				app->player->healingCooldown -= 1000;
				app->player->RestartGorilaIdle();

				b2Vec2 jumpForce(0, -10.f);
				b2Vec2 yVel = { playerBox->GetLinearVelocity().x,0 };
				playerBox->SetLinearVelocity(yVel);
				playerBox->ApplyLinearImpulse(jumpForce, playerBox->GetPosition(), true);
				playerBox->SetLinearDamping(0);
				app->audio->PlayFx(app->player->enemy_death);
			}
			else
			{
				if (!app->GameIsPaused())
				app->player->HurtGorila(1);
			}
		}



	}


	return true;
}

bool EnemyMushroom::LoadState(pugi::xml_node& data)
{
	id = data.attribute("id").as_int();
	b2Vec2 currentPos(data.attribute("currentPos.x").as_float(), data.attribute("currentPos.y").as_float());
	Hitbox->body->SetTransform(currentPos, 0);

	//LOG("pos, %i %i", METERS_TO_PIXELS(Hitbox->body->GetPosition().x), METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("pos, %i %i", METERS_TO_PIXELS(currentPos.x), METERS_TO_PIXELS(currentPos.y));

	checkTimer = data.attribute("checkTimer").as_int();
	maxDistanceAgro = data.attribute("maxDistanceAgro").as_int();

	currentSpeed.x = data.attribute("currentSpeed.x").as_float();
	currentSpeed.y = data.attribute("currentSpeed.y").as_float();

	agroTowardsPlayer = data.attribute("agroTowardsPlayer").as_bool();
	health = data.attribute("health").as_int();
	direction = data.attribute("direction").as_bool();
	

	return true;
}


bool EnemyMushroom::SaveState(pugi::xml_node& data) const
{

	pugi::xml_node myself = data.append_child("EnemyMushroom");

	myself.append_attribute("id").set_value(id);
	myself.append_attribute("currentPos.x").set_value(Hitbox->body->GetPosition().x);
	myself.append_attribute("currentPos.y").set_value(Hitbox->body->GetPosition().y);

	myself.append_attribute("checkTimer").set_value(checkTimer);
	myself.append_attribute("maxDistanceAgro").set_value(maxDistanceAgro);

	myself.append_attribute("currentSpeed.x").set_value(currentSpeed.x);
	myself.append_attribute("currentSpeed.y").set_value(currentSpeed.y);

	myself.append_attribute("agroTowardsPlayer").set_value(agroTowardsPlayer);
	myself.append_attribute("health").set_value(health);
	myself.append_attribute("direction").set_value(direction);
	return true;
}

void EnemyMushroom::DoDamage(int damage)
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