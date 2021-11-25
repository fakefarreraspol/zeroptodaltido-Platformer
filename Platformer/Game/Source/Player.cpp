#include "App.h"
#include "Log.h"
#include "Player.h"
#include "Input.h"
#include "SDL/include/SDL.h"
#include "p2List.h"
#include "Scene.h"
#include "Physics.h"
#include "Textures.h"
#include"Render.h"

Player::Player() : Module()
{
	name.Create("player");	
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
	//textures
	gorila = app->tex->Load("Assets/textures/gorila.png");
	r_gorilaWalk[0] = { 4, 4, 62, 60};
	r_gorilaWalk[1] = {80,4,72,60};
	r_gorilaWalk[2] = {166,4,62,60};
	r_gorilaWalk[3] = {246,4,76,60};
	
	r_gorilaPunch[0] = { 4, 116, 72, 64};
	r_gorilaPunch[1] = {92,116,86,72};
	
	r_gorilaJump[0] = {628,248,62,64};
	r_gorilaJump[1] = {538,248,76,64};
	r_gorilaJump[2] = {412,248,110,64};
	r_gorilaJump[3] = { 344,236,54,76 };

	r_gorilaIdle[0] = { 348,0,60,64 };
	r_gorilaIdle[1] = { 408,0,60,64 };
	r_gorilaIdle[2] = { 471,0,60,64 };
	r_gorilaIdle[3] = { 540,0,74,64 };
	r_gorilaIdle[4] = { 622,0,104,64};
	//player stats
	startPosX = 48 * 4;
	startPosY = 48 * 22;
	speed = { 7.f,0 };
	jumpForce = { 0,-25.f };

	
	ColHitbox = app->physics->CreateCircle(startPosX, startPosY, 23);
	
	

	int x_ = (int)x;
	int y_ = (int)y;
	ColHitbox->GetPosition(x_, y_);

	//ColSensor = app->physics->CreateRectangle(x, y, 46, 50);
	//ColSensor->body->GetFixtureList()->SetSensor(true);
	//ColSensor->body->SetType(b2_kinematicBody);

	goLeft = false;
	goRight = false;

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
	currentTime = SDL_GetTicks();
	b2Vec2 pos = { x,y };

	goLeft = (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT);
	goRight = (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT);


	b2Vec2 movement = { (goRight - goLeft) * speed.x, ColHitbox->body->GetLinearVelocity().y};
	ColHitbox->body->SetLinearVelocity(movement);

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



	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) characterWalking = true;
	else characterWalking = false;

	
	/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		inAir = true;
		airTime = currentTime + 1000;
		if (airTime > currentTime)
		{
			if (lastTime + 200 > currentTime)
			{
				if (!lastDirection)
				{
					app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaJump[currentGorilaJump]);
				}
				else app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaJump[currentGorilaJump], SDL_FLIP_HORIZONTAL);
			}
			else 
			{
				if (currentGorilaJump < 4)
				{
					currentGorilaJump++;
					lastTime = currentTime;
				}
				else
				{
					app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaJump[currentGorilaJump]);
				}
			}
		}
		else
		{
			inAir = false;
		}
		
		
	}*/
	
	LOG("current time %i", currentTime);
	if (!inAir)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			RestartGorilaIdle();
			if (lastTime + 300 > currentTime)
			{
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk]);
			}
			else
			{
				currentGorilaWalk++;
				lastTime = currentTime;

			}
			lastDirection = false;

			if ((currentGorilaWalk <= -1) || (currentGorilaWalk >= 4))
			{
				currentGorilaWalk = 0;
			}
		}


		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			RestartGorilaIdle();
			if (lastTime + 300 > currentTime)
			{
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk], SDL_FLIP_HORIZONTAL);
			}
			else
			{
				currentGorilaWalk++;
				lastTime = currentTime;

			}
			lastDirection = true;

			if ((currentGorilaWalk <= -1) || (currentGorilaWalk >= 4))
			{
				currentGorilaWalk = 0;
			}

		}
		if (!characterWalking)
		{
			if (lastDirection)
			{
				if (lastTime + 1000 > currentTime)
				{
					app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
				}
				else
				{
					if (currentGorilaIdle < 4)
					{
						currentGorilaIdle++;
						lastTime = currentTime;
					}
					else
					{
						app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
					}


				}


			}
			else if (!lastDirection)
			{
				if (lastTime + 1000 > currentTime)
				{
					app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);
				}
				else
				{
					if (currentGorilaIdle < 4)
					{
						currentGorilaIdle++;
						lastTime = currentTime;
					}
					else
					{
						app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);
					}


				}
			}
		}

	}

	

	

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	startPosX = data.child("startPos").attribute("x").as_float(0);
	startPosY = data.child("startPos").attribute("y").as_float(0);

	b2Vec2 v = { PIXEL_TO_METERS( startPosX), PIXEL_TO_METERS(startPosY )};
	ColHitbox->body->SetTransform(v, 0);

	return true;
}


bool Player::SaveState(pugi::xml_node& data) const
{

	LOG("saving camera pos");
	data.child("startPos").attribute("x").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x));
	data.child("startPos").attribute("y").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().y));
	return true;
}

