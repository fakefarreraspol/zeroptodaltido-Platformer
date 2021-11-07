#include "App.h"
#include "Log.h"
#include "Player.h"
#include "Input.h"
#include "SDL/include/SDL.h"
#include "p2List.h"
#include "Scene.h"
#include "Physics.h"

Player::Player() : Module()
{
}

Player::~Player()
{}

bool Player::Awake()
{
	return true;
}

// Load assets
bool Player::Start()
{
	//player stats
	int startPosX = 48 * 4;
	int startPosY = 48 * 27;
	speed = { 2,0 };
	jumpForce = { 0,-25.f };


	ColHitbox = app->physics->CreateCircle(startPosX, startPosY, 23);
	
	

	int x_ = (int)x;
	int y_ = (int)y;
	ColHitbox->GetPosition(x_, y_);

	//ColSensor = app->physics->CreateRectangle(x, y, 46, 50);
	//ColSensor->body->GetFixtureList()->SetSensor(true);
	//ColSensor->body->SetType(b2_kinematicBody);

	LOG("Loading player");
	return true;
}

// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
bool Player::Update(float dt)
{
	//ColSensor->body->SetTransform(ColHitbox->body->GetPosition(), 0);

	b2Vec2 pos = { x,y };

	bool goLeft = (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT);
	bool goRight = (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT);



	//LOG("v: %f", ColHitbox->body->GetLinearVelocity().x);
	//ColHitbox->body->ApplyLinearImpulse(goRight * speed, ColHitbox->body->GetPosition(), true);

	if (ColHitbox->body->GetLinearVelocity().x < 5.f)
		ColHitbox->body->ApplyLinearImpulse(goRight * speed, ColHitbox->body->GetPosition(), true);
	
	if (ColHitbox->body->GetLinearVelocity().x > -5.f)
		ColHitbox->body->ApplyLinearImpulse(-goLeft* speed, ColHitbox->body->GetPosition(), true);

	

	

	

	b2Body* ground;
	if (ColHitbox->body->GetContactList() != nullptr)
	{
		ground = ColHitbox->body->GetContactList()->contact->GetFixtureA()->GetBody();
		
		if (ground != nullptr)
		{

			b2Vec2 xVel = { 0,ColHitbox->body->GetLinearVelocity().y };
			if (!goLeft && !goRight) ColHitbox->body->SetLinearVelocity(xVel);

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				b2Vec2 yVel = { ColHitbox->body->GetLinearVelocity().y,0 };
				ColHitbox->body->SetLinearVelocity(yVel);
				ColHitbox->body->ApplyLinearImpulse(jumpForce, ColHitbox->body->GetPosition(), true);
				ColHitbox->body->SetLinearDamping(0);
			}
		}
		
	}
	else
	{
		LOG("no ground");
		


	}


	



	return true;
}



