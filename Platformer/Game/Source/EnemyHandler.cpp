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
	textureMushroom = app->tex->Load("Assets/textures/mushroom_idle.png");
	return true;
}
bool EnemyHandler::CleanUp()
{
	LOG("Unloading eHandler");

	p2List_item<EnemyMushroom*>* iteratorMushroom = enemiesMushroom.getFirst();
	if (iteratorMushroom != NULL)
	{
		iteratorMushroom->data->CleanUp();
	}

	p2List_item<EnemyBird*>* iteratorBird = enemiesBird.getFirst();
	if (iteratorBird != NULL)
	{
		iteratorBird->data->CleanUp();

	}

	p2List_item<EnemySnake*>* iteratorSnake = enemiesSnake.getFirst();
	if (iteratorSnake != NULL)
	{
		iteratorSnake->data->CleanUp();

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
	
	p2List_item<EnemyMushroom*>* iteratorMushroom = enemiesMushroom.getFirst();
	if (iteratorMushroom != NULL)
	{
		iteratorMushroom->data->LoadState(data);
	}

	p2List_item<EnemyBird*>* iteratorBird = enemiesBird.getFirst();
	if (iteratorBird != NULL)
	{
		iteratorBird->data->LoadState(data);
	}

	p2List_item<EnemySnake*>* iteratorSnake = enemiesSnake.getFirst();
	if (iteratorSnake != NULL)
	{
		iteratorSnake->data->LoadState(data);
	}

	return true;
}


bool EnemyHandler::SaveState(pugi::xml_node& data) const
{

	p2List_item<EnemyMushroom*>* iteratorMushroom = enemiesMushroom.getFirst();
	if (iteratorMushroom != NULL)
	{
		iteratorMushroom->data->SaveState(data);
	}

	p2List_item<EnemyBird*>* iteratorBird = enemiesBird.getFirst();
	if (iteratorBird != NULL)
	{
		iteratorBird->data->SaveState(data);
	}

	p2List_item<EnemySnake*>* iteratorSnake = enemiesSnake.getFirst();
	if (iteratorSnake != NULL)
	{
		iteratorSnake->data->SaveState(data);
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
		EnemyMushroom* newMushroom = new EnemyMushroom(pos,30);
		enemiesMushroom.add(newMushroom);
		newMushroom->Start();
	}
		break;
	case ENEMY_SNAKE:
		break;
	case ENEMY_BIRD:
		break;
	default:
		break;
	}
}

