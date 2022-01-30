#include "Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "App.h"
#include "Physics.h"
#include <math.h>
#include "SDL/include/SDL.h"
#include "Render.h"
#include "Animation.h"
#include "Log.h"
#include "Item.h"
#include "Textures.h"


Item::Item(ItemType type, b2Vec2 startPosition) : Entity()
{
	spawnPosition = startPosition;
	name.Create("item");
	this->type = type;

	spawnPosition = startPosition;
	iPoint temp((int)fabs(spawnPosition.x), (int)fabs(spawnPosition.y));

	spawnPosIP = temp;
	spawnPosMapIP = app->map->WorldToMap(temp.x, temp.y);

	Hitbox = app->physics->CreateCircle(spawnPosition.x, spawnPosition.y, 20);
	Hitbox->body->GetFixtureList()->SetSensor(true);
	Hitbox->body->SetGravityScale(0);

}

Item::~Item()
{}

bool Item::Awake()
{
	return true;
}

bool Item::Start()
{
	TexBanana = app->tex->Load("Assets/textures/banana.png");


	switch (type)
	{
	case BANANA:
	{


	}
		break;
	default:
		break;
	}
	return true;
}
bool Item::CleanUp()
{
	switch (type)
	{
	case BANANA:
	{


	}
	break;
	default:
		break;
	}
	return true;
}
bool Item::Update(float dt)
{
	currentTime++;

	switch (type)
	{
	case BANANA:
	{

		app->render->DrawTexture(TexBanana,
			METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x - 10),
			METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y - 15) + 5 * sin(currentTime * 0.05f),
			NULL);
	}
	break;
	default:
		break;
	}
	return true;
}

bool Item::LoadState(pugi::xml_node& data)
{
	switch (type)
	{
	case BANANA:
	{
		id = data.attribute("id").as_int();
		b2Vec2 currentPos(data.attribute("currentPos.x").as_float(), data.attribute("currentPos.y").as_float());
		Hitbox->body->SetTransform(currentPos, 0);

	}
	break;
	default:
		break;
	}
	return true;
}
bool Item::SaveState(pugi::xml_node& data) const
{
	switch (type)
	{
	case BANANA:
	{
		pugi::xml_node myself = data.append_child("Item");

		myself.append_attribute("id").set_value(id);
		myself.append_attribute("currentPos.x").set_value(Hitbox->body->GetPosition().x);
		myself.append_attribute("currentPos.y").set_value(Hitbox->body->GetPosition().y);


	}
	break;
	default:
		break;
	}
	return true;
}
