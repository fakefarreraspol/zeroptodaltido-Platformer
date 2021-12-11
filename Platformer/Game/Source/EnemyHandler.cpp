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
#include "EnemyHandler.h"

EnemyHandler::EnemyHandler() : Module()
{
	name.Create("enemyHandler");
}

EnemyHandler::~EnemyHandler()
{}

bool EnemyHandler::Awake()
{
	return true;
}

bool EnemyHandler::Start()
{
	textureMushroom = app->tex->Load("Assets/textures/mushroom_walk.png");
	textureBird = app->tex->Load("Assets/textures/bird.png");
	attention = app->tex->Load("Assets/textures/attention_sign.png");
	texturePath = app->tex->Load("Assets/maps/meta.png");
	return true;
}
bool EnemyHandler::CleanUp()
{
	LOG("Unloading eHandler");

	for (int i = 0; i < enemiesMushroom.count(); i++)
	{

		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		iteratorMushroom->CleanUp();
	}

	for (int i = 0; i < enemiesBird.count(); i++)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		iteratorBird->CleanUp();
	}

	for (int i = 0; i < enemiesSnake.count(); i++)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		iteratorSnake->CleanUp();
	}


	return true;
}

bool EnemyHandler::Update(float dt)
{

	for (int i = 0; i < enemiesMushroom.count(); i++)
	{

		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		iteratorMushroom->Update(dt);
	}

	for (int i = 0; i < enemiesBird.count(); i++)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		iteratorBird->Update(dt);
	}

	for (int i = 0; i < enemiesSnake.count(); i++)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		iteratorSnake->Update(dt);
	}


	return true;
}

bool EnemyHandler::LoadState(pugi::xml_node& data)
{
	
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{

		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		iteratorMushroom->LoadState(data);
	}

	for (int i = 0; i < enemiesBird.count(); i++)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		iteratorBird->LoadState(data);
	}

	for (int i = 0; i < enemiesSnake.count(); i++)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		iteratorSnake->LoadState(data);
	}

	return true;
}


bool EnemyHandler::SaveState(pugi::xml_node& data) const
{

	for (int i = 0; i < enemiesMushroom.count(); i++)
	{

		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		iteratorMushroom->SaveState(data);
	}

	for (int i = 0; i < enemiesBird.count(); i++)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		iteratorBird->SaveState(data);
	}

	for (int i = 0; i < enemiesSnake.count(); i++)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		iteratorSnake->SaveState(data);
	}
	return true;
}

void EnemyHandler::CreateEnemy(EnemyType type, int x, int y)
{
	spawnEnemyRequest = true;
	switch (type)
	{
	case ENEMY_MUSHROOM:
	{
		b2Vec2 pos(x, y);
		EnemyMushroom* newMushroom = new EnemyMushroom(pos, 5);
		enemiesMushroom.add(newMushroom);
		newMushroom->Start();
	}
		break;
	case ENEMY_SNAKE:

		
		break;
	case ENEMY_BIRD:
	{
		b2Vec2 pos(x, y);
		EnemyBird* newBird = new EnemyBird(pos, 2);
		enemiesBird.add(newBird);
		newBird->Start();
	}
		break;
	default:
		break;
	}
}

void EnemyHandler::DestroyEnemy(PhysBody* body)
{

	bool cont = false; 

	p2List_item<EnemyMushroom*>* mushroom = enemiesMushroom.getFirst();
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{
		LOG("n: %i", enemiesMushroom.count());
		if (cont) break;
		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);
		


		if (iteratorMushroom->GetPhysBody() == body)
		{
			enemiesMushroom.del(mushroom);
			app->physics->GetWorld()->DestroyBody(body->body);
			
			delete body;
			cont = true;
		}
		mushroom = mushroom->next;
	}

	p2List_item<EnemyBird*>* bird = enemiesBird.getFirst();
	for (int i = 0; i < enemiesBird.count(); i++)
	{
		if (cont) break;

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);


		if (iteratorBird->GetPhysBody() == body)
		{
			enemiesBird.del(bird);
			app->physics->GetWorld()->DestroyBody(body->body);
			delete body;
			cont = true;
		}
		bird = bird->next;
	}


	p2List_item<EnemySnake*>* snake = enemiesSnake.getFirst();
	for (int i = 0; i < enemiesSnake.count(); i++)
	{
		if (cont) break;

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		if (iteratorSnake->GetPhysBody() == body)
		{
			enemiesSnake.del(snake);
			app->physics->GetWorld()->DestroyBody(body->body);
			delete body;
			cont = true;
		}
		bird = bird->next;
	}

	if (cont) 
		LOG("Enemy destroyed");
}

void EnemyHandler::DamageEnemy(b2Body* body, int damage)
{

	bool cont = false;

	
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{
		if (cont) break;
		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);



		if (iteratorMushroom->GetPhysBody()->body == body)
		{
			LOG("found type");	iteratorMushroom->DoDamage(damage);
			cont = true;
		}
	}


	for (int i = 0; i < enemiesBird.count(); i++)
	{
		if (cont) break;

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);



		if (iteratorBird->GetPhysBody()->body == body)
		{
			LOG("found type"); 
			iteratorBird->DoDamage(damage);
			cont = true;
		}
	}



	for (int i = 0; i < enemiesSnake.count(); i++)
	{
		if (cont) break;

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		if (iteratorSnake->GetPhysBody()->body == body)
		{
			LOG("found type");
			iteratorSnake->DoDamage(damage);
			cont = true;
		}
	}
}

void EnemyHandler::HandleEnemyDespawn()
{
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{

		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		if (PhysBodyIsInMap(iteratorMushroom->GetPhysBody())) 
			DestroyEnemy(iteratorMushroom->GetPhysBody());
	}

	for (int i = 0; i < enemiesBird.count(); i++)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		if (PhysBodyIsInMap(iteratorBird->GetPhysBody()))
			DestroyEnemy(iteratorBird->GetPhysBody());
	}

	for (int i = 0; i < enemiesSnake.count(); i++)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		if (PhysBodyIsInMap(iteratorSnake->GetPhysBody()))
			DestroyEnemy(iteratorSnake->GetPhysBody());
	}
}


