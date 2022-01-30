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
#include "EntityHandler.h"

EntityHandler::EntityHandler() : Module()
{
	name.Create("EntityHandler");
}

EntityHandler::~EntityHandler()
{}

bool EntityHandler::Awake()
{
	return true;
}

bool EntityHandler::Start()
{
	textureMushroom = app->tex->Load("Assets/textures/mushroom_walk.png"); 
	textureSnake = app->tex->Load("Assets/textures/snake.png");
	textureBird = app->tex->Load("Assets/textures/bird.png");
	attention = app->tex->Load("Assets/textures/attention_sign.png");
	texturePath = app->tex->Load("Assets/maps/meta.png");

	all_ids = 0;

	return true;
}
bool EntityHandler::CleanUp()
{
	LOG("Unloading eHandler");

	for (int i = 0; i < allEntities.count(); i++)
	{

		Entity* entity;
		allEntities.at(i, entity);

		entity->CleanUp();
	}

	/*for (int i = 0; i < enemiesMushroom.count(); i++)
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
	for (int i = 0; i < rockets.count(); i++)
	{

		RocketBanana* iteratorRocket;
		rockets.at(i, iteratorRocket);

		iteratorRocket->CleanUp();
	}*/


	return true;
}

bool EntityHandler::Update(float dt)
{

	for (int i = 0; i < allEntities.count(); i++)
	{

		Entity* entitiy;
		allEntities.at(i, entitiy);

		entitiy->Update(dt);
	}

	//for (int i = 0; i < enemiesMushroom.count(); i++)
	//{
	//
	//	EnemyMushroom* iteratorMushroom;
	//	enemiesMushroom.at(i, iteratorMushroom);
	//
	//	iteratorMushroom->Update(dt);
	//}
	//
	//for (int i = 0; i < enemiesBird.count(); i++)
	//{
	//
	//	EnemyBird* iteratorBird;
	//	enemiesBird.at(i, iteratorBird);
	//
	//	iteratorBird->Update(dt);
	//}
	//
	//for (int i = 0; i < enemiesSnake.count(); i++)
	//{
	//
	//	EnemySnake* iteratorSnake;
	//	enemiesSnake.at(i, iteratorSnake);
	//
	//	iteratorSnake->Update(dt);
	//}
	//
	//for (int i = 0; i < items.count(); i++)
	//{
	//
	//	Item* iteratorItem;
	//	items.at(i, iteratorItem);
	//
	//	iteratorItem->Update(dt);
	//}
	//
	//for (int i = 0; i < rockets.count(); i++)
	//{
	//
	//	RocketBanana* iteratorRocket;
	//	rockets.at(i, iteratorRocket);
	//
	//	iteratorRocket->Update(dt);
	//}

	return true;
}

PhysBody EntityHandler::GetNearestEnemy(PhysBody* Character)
{
	PhysBody* NearEnemy = nullptr;
	int temp = 1000;
	
	for (int i = 0; i < enemiesSnake.count(); i++)
	{
		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		int j = iteratorSnake->CheckDistanceToPhysBody(Character);
		if (j<temp)
		{
			temp = j;
			NearEnemy = iteratorSnake->GetPhysBody();
		}
	}
	for (int i = 0; i < enemiesBird.count(); i++)
	{
		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		int j = iteratorBird->CheckDistanceToPhysBody(Character);
		if (j < temp)
		{
			temp = j;
			NearEnemy = iteratorBird->GetPhysBody();
		}
	}
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{
		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		int j = iteratorMushroom->CheckDistanceToPhysBody(Character);
		if (j < temp)
		{
			temp = j;
			NearEnemy = iteratorMushroom->GetPhysBody();
		}
	}
	return *NearEnemy;
}

bool EntityHandler::LoadState(pugi::xml_node& data)
{
	//DestroyAllEnemies();
	//pugi::xml_node myself = data.child("EntityHandler");

	DestroyAllEnemies();


	if (data != NULL)
	{
		pugi::xml_node prop;

		int i = 0;
		//prop = myself.child("EnemyMushroom");
		//while (prop.type() == pugi::node_element)
		//{
		//	CreateEntity(EntityType::ENEMY_MUSHROOM, 0, 0);
		//	EnemyMushroom* temp;
		//	enemiesMushroom.at(i, temp);
		//	temp->LoadState(myself);
		//	i++;
		//
		//	prop = prop.next_sibling("EnemyMushroom");
		//}

		for (prop = data.child("Item"); prop.type() == pugi::node_element; prop = prop.next_sibling("Item"))
		{
			CreateEntity(ITEM_BANANA, 0, 0);
			Item* ent = nullptr;
			items.at(i, ent);
			ent->LoadState(prop);

			i++;

		}
		i = 0;

		for (prop = data.child("EnemySnake"); prop.type() == pugi::node_element; prop = prop.next_sibling("EnemySnake"))
		{
			CreateEntity(ENEMY_SNAKE, 0, 0);
			EnemySnake* ent = nullptr;
			enemiesSnake.at(i, ent);
			ent->LoadState(prop);

			i++;

		}
		i = 0;

		for (prop = data.child("EnemyBird"); prop.type() == pugi::node_element; prop = prop.next_sibling("EnemyBird"))
		{
			CreateEntity(ENEMY_BIRD, 0, 0);
			EnemyBird* ent = nullptr;
			enemiesBird.at(i, ent);
			ent->LoadState(prop);

			i++;

		}
		i = 0;

		for (prop = data.child("EnemyMushroom"); prop.type() == pugi::node_element; prop = prop.next_sibling("EnemyMushroom"))
		{
			CreateEntity(ENEMY_MUSHROOM, 0, 0);
			EnemyMushroom* ent = nullptr;
			enemiesMushroom.at(i,ent);
			ent->LoadState(prop);

			i++;
		
		}
		i = 0;

		/*for (prop = data.child("EnemyBird"); prop.type() == pugi::node_element; prop = prop.next_sibling("EnemyBird"))
		{
			EnemyBird* temp;
			enemiesBird.at(i, temp);
			if (temp == nullptr || enemiesBird.count() == 0)
			{
				CreateEntity(EntityType::ENEMY_BIRD, 0, 0);
				enemiesBird.at(i, temp);
			}


			temp->LoadState(prop);
			i++;

		}
		

		for (prop = data.child("EnemySnake"); prop.type() == pugi::node_element; prop = prop.next_sibling("EnemySnake"))
		{
			EnemySnake* temp;
			enemiesSnake.at(i, temp);
			if (temp == nullptr || enemiesSnake.count() == 0)
			{
				CreateEntity(EntityType::ENEMY_SNAKE, 0, 0);
				enemiesSnake.at(i, temp);
			}

			temp->LoadState(prop);
			i++;

		}*/
		
	}

	return true;
}


bool EntityHandler::SaveState(pugi::xml_node& data) const
{
	//pugi::xml_node myself = data.child("EntityHandler");
	//if (myself.type() != pugi::node_element)
	//{
	//	pugi::xml_node myself = data.append_child("EntityHandler");
	//
	//}

	//myself.append_attribute("MushroomCount").set_value(enemiesMushroom.count());
	//myself.append_attribute("BirdCount").set_value(enemiesBird.count());
	//myself.append_attribute("SnakeCount").set_value(enemiesSnake.count());

	pugi::xml_node iteratorRemove = data.first_child();

	while (iteratorRemove.next_sibling())
	{
		pugi::xml_node toRemove = iteratorRemove.next_sibling();
		data.remove_child(toRemove);
	}
	data.remove_child(iteratorRemove);

	//for (pugi::xml_node iteratorRemove = data.first_child(); iteratorRemove; iteratorRemove = iteratorRemove.next_sibling())
	//{
	//	pugi::xml_node toRemove = iteratorRemove;
	//	data.remove_child(toRemove);
	//	data.remove_child(toRemove.next_sibling());
	//}



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

	for (int i = 0; i < items.count(); i++)
	{

		Item* it;
		items.at(i, it);

		it->SaveState(data);
	}
	return true;
}

void EntityHandler::CreateEntity(enum EntityType type, int x, int y)
{
	
	switch (type)
	{
	case ENEMY_MUSHROOM:
	{
		b2Vec2 pos(x, y);
		EnemyMushroom* newMushroom = new EnemyMushroom(pos, 2);
		enemiesMushroom.add(newMushroom);
		allEntities.add(newMushroom);
		newMushroom->Start();
		newMushroom->SetID(all_ids);
		all_ids++;
	}
		break;
	case ENEMY_SNAKE:
	{
		b2Vec2 pos(x, y);
		EnemySnake* newSnake = new EnemySnake(pos, 2);
		enemiesSnake.add(newSnake);
		allEntities.add(newSnake);
		newSnake->Start();
		newSnake->SetID(all_ids);
		all_ids++;

		break;
	}
	case ENEMY_BIRD:
	{
		b2Vec2 pos(x, y);
		EnemyBird* newBird = new EnemyBird(pos, 1);
		enemiesBird.add(newBird);
		allEntities.add(newBird);
		newBird->Start();
		newBird->SetID(all_ids);
		all_ids++;

		break;
	}
	case ITEM_BANANA:
	{
		b2Vec2 pos(x, y);
		Item* newItem = new Item(ItemType::BANANA, pos);
		items.add(newItem);
		allEntities.add(newItem);
		newItem->Start();
		newItem->SetID(all_ids);
		all_ids++;

		break;
	} 
	case ROCKET_BANANA:
	{
		b2Vec2 pos(x, y);
		RocketBanana* newRocket = new RocketBanana(pos, 1);
		
		rockets.add(newRocket);
		allEntities.add(newRocket);
		newRocket->Start();
		newRocket->SetID(all_ids);
		all_ids++;
		break;
	} 
		
	default:
		break;
	}

	//LOG("Entity Created");
}

void EntityHandler::DestroyEnemy(b2Body* body)
{

	bool cont = false; 




	p2List_item<EnemyMushroom*>* mushroom = enemiesMushroom.getFirst();
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{
		if (cont) break;
		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);
		


		if (iteratorMushroom->GetPhysBody()->body == body)
		{
			enemiesMushroom.del(mushroom);
			p2List_item<Entity*>* eToDelete = allEntities.findNode(iteratorMushroom);
			allEntities.del(eToDelete);

			app->physics->GetWorld()->DestroyBody(body);
			
			cont = true;
		}
		else
			mushroom = mushroom->next;
	}

	p2List_item<EnemyBird*>* bird = enemiesBird.getFirst();

	for (int i = 0; i < enemiesBird.count(); i++)
	{
		if (cont) break;

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);


		if (iteratorBird->GetPhysBody()->body == body)
		{
   			enemiesBird.del(bird);
			p2List_item<Entity*>* eToDelete = allEntities.findNode(iteratorBird);
			allEntities.del(eToDelete);

			app->physics->GetWorld()->DestroyBody(body);
			cont = true;
		}
		else
			bird = bird->next;
	}


	p2List_item<EnemySnake*>* snake = enemiesSnake.getFirst();
	for (int i = 0; i < enemiesSnake.count(); i++)
	{
		if (cont) break;

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		if (iteratorSnake->GetPhysBody()->body == body)
		{
			enemiesSnake.del(snake);
			p2List_item<Entity*>* eToDelete = allEntities.findNode(iteratorSnake);
			allEntities.del(eToDelete);

			app->physics->GetWorld()->DestroyBody(body);
			cont = true;
		}
		else
			snake = snake->next;
	}

	p2List_item<RocketBanana*>* rocket = rockets.getFirst();
	for (int i = 0; i < rockets.count(); i++)
	{
		if (cont) break;

		RocketBanana* iteratorRocket;
		rockets.at(i, iteratorRocket);


		if (iteratorRocket->GetPhysBody()->body == body)
		{
			rockets.del(rocket);
			p2List_item<Entity*>* eToDelete = allEntities.findNode(iteratorRocket);
			allEntities.del(eToDelete);

			app->physics->GetWorld()->DestroyBody(body);
			cont = true;
		}
		else
			rocket = rocket->next;
	}

	p2List_item<Item*>* it = items.getFirst();
	for (int i = 0; i < items.count(); i++)
	{
		if (cont) break;

		Item* item;
		items.at(i, item);


		if (item->GetPhysBody()->body == body)
		{
			items.del(it);
			p2List_item<Entity*>* eToDelete = allEntities.findNode(item);
			allEntities.del(eToDelete);

			app->physics->GetWorld()->DestroyBody(body);
			cont = true;
		}
		else
			it = it->next;
	}

	if (cont) 
		LOG("Enemy destroyed");
}

EntityType EntityHandler::GetEntityType(b2Body* body) const
{
	bool cont = false;


	p2List_item<EnemyMushroom*>* mushroom = enemiesMushroom.getFirst();
	for (int i = 0; i < enemiesMushroom.count(); i++)
	{
		//LOG("n: %i", enemiesMushroom.count());
		if (cont) break;
		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);



		if (iteratorMushroom->GetPhysBody()->body == body)
		{
			return ENEMY_MUSHROOM;
			cont = true;
		}
		else
			mushroom = mushroom->next;
	}

	p2List_item<EnemyBird*>* bird = enemiesBird.getFirst();

	for (int i = 0; i < enemiesBird.count(); i++)
	{
		if (cont) break;

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);


		if (iteratorBird->GetPhysBody()->body == body)
		{
			return ENEMY_BIRD;
			cont = true;

		}
		else
			bird = bird->next;
	}


	p2List_item<EnemySnake*>* snake = enemiesSnake.getFirst();
	for (int i = 0; i < enemiesSnake.count(); i++)
	{
		if (cont) break;

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		if (iteratorSnake->GetPhysBody()->body == body)
		{
			return ENEMY_SNAKE;
			cont = true;

		}
		else
			snake = snake->next;
	}

	p2List_item<RocketBanana*>* rocket = rockets.getFirst();
	for (int i = 0; i < rockets.count(); i++)
	{
		if (cont) break;

		RocketBanana* iteratorRocket;
		rockets.at(i, iteratorRocket);


		if (iteratorRocket->GetPhysBody()->body == body)
		{
			return ROCKET_BANANA;
			cont = true;
		}
		else
			rocket = rocket->next;
	}

	p2List_item<Item*>* it = items.getFirst();
	for (int i = 0; i < items.count(); i++)
	{
		if (cont) break;

		Item* item;
		items.at(i, item);


		if (item->GetPhysBody()->body == body)
		{
			return ITEM_BANANA;
			cont = true;
		}
		else
			it = it->next;
	}

	return ENTITY_NULL;
}

void EntityHandler::DamageEnemy(b2Body* body, int damage)
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

	for (int i = 0; i < rockets.count(); i++)
	{
		if (cont) break;

		RocketBanana* iteratorRocket;
		rockets.at(i, iteratorRocket);

		if (iteratorRocket->GetPhysBody()->body == body)
		{
			LOG("found type");
			iteratorRocket->DoDamage(damage);
			cont = true;
		}
	}
}

void EntityHandler::HandleEnemyDespawn()
{

	for (int i = 0; i < enemiesMushroom.count(); i++)
	{

		EnemyMushroom* iteratorMushroom;
		enemiesMushroom.at(i, iteratorMushroom);

		if (PhysBodyIsInMap(iteratorMushroom->GetPhysBody())) 
			DestroyEnemy(iteratorMushroom->GetPhysBody()->body);
	}

	for (int i = 0; i < enemiesBird.count(); i++)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i, iteratorBird);

		if (PhysBodyIsInMap(iteratorBird->GetPhysBody()))
			DestroyEnemy(iteratorBird->GetPhysBody()->body);
	}

	for (int i = 0; i < enemiesSnake.count(); i++)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i, iteratorSnake);

		if (PhysBodyIsInMap(iteratorSnake->GetPhysBody()))
			DestroyEnemy(iteratorSnake->GetPhysBody()->body);
	}
	for (int i = 0; i < rockets.count(); i++)
	{

		RocketBanana* iteratorRocket;
		rockets.at(i, iteratorRocket);

		if (PhysBodyIsInMap(iteratorRocket->GetPhysBody()))
			DestroyEnemy(iteratorRocket->GetPhysBody()->body);
	}
}

void EntityHandler::DestroyAllEnemies()
{
	int a = enemiesMushroom.count();
	for (int i = a; i > 0; i--)
	{

		EnemyMushroom* iteratorMushroom;
		bool ret = enemiesMushroom.at(i - 1, iteratorMushroom);
		
		DestroyEnemy(iteratorMushroom->GetPhysBody()->body);
	}

	a = enemiesBird.count();
	for (int i = a; i > 0; i--)
	{

		EnemyBird* iteratorBird;
		enemiesBird.at(i - 1, iteratorBird);
		
		DestroyEnemy(iteratorBird->GetPhysBody()->body);
	}

	a = enemiesSnake.count();
	for (int i = a; i > 0; i--)
	{

		EnemySnake* iteratorSnake;
		enemiesSnake.at(i - 1, iteratorSnake);
		
		DestroyEnemy(iteratorSnake->GetPhysBody()->body);
	}

	a = rockets.count();
	for (int i = a; i > 0; i--)
	{

		RocketBanana* iteratorRocket;
		rockets.at(i - 1, iteratorRocket);

		DestroyEnemy(iteratorRocket->GetPhysBody()->body);
	}

	a = items.count();
	for (int i = a; i > 0; i--)
	{

		Item* it;
		items.at(i - 1, it);

		DestroyEnemy(it->GetPhysBody()->body);
	}

}

