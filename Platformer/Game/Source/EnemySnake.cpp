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
#include "EnemySnake.h"

EnemySnake::EnemySnake(b2Vec2 startPosition, int health) : Module()
{
	spawnPosition = startPosition;
	name.Create("enemySnake");
	Hitbox = app->physics->CreateCircle(spawnPosition.x, spawnPosition.y, 10);
	this->health = health;

}


EnemySnake::~EnemySnake()
{}

bool EnemySnake::Awake()
{
	return true;
}

bool EnemySnake::Start()
{



	return true;
}
bool EnemySnake::CleanUp()
{
	LOG("Unloading player");

	return true;
}

bool EnemySnake::Update(float dt)
{



	app->render->DrawTexture(app->enemyMaster->textureMushroom, METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x), METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y), NULL);


	return true;
}

bool EnemySnake::LoadState(pugi::xml_node& data)
{

	//startPosX = data.child("startPos").attribute("x").as_float(0);
	//startPosY = data.child("startPos").attribute("y").as_float(0);

	//b2Vec2 v = { PIXEL_TO_METERS(startPosX), PIXEL_TO_METERS(startPosY) };
	//ColHitbox->body->SetTransform(v, 0);

	return true;
}


bool EnemySnake::SaveState(pugi::xml_node& data) const
{

	//LOG("saving camera pos");
	//data.child("startPos").attribute("x").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x));
	//data.child("startPos").attribute("y").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().y));
	return true;
}
