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
#include "Audio.h"
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
	playerDeath = app->audio->LoadFx("Assets/audio/fx/player_death.wav");
	bananaThrow = app->audio->LoadFx("Assets/audio/fx/player_shot.wav");
	//textures
	
	gorila = app->tex->Load("Assets/textures/gorila.png");
	sleep = app->tex->Load("Assets/textures/mini_zzz.png");
	throwBanana = app->tex->Load("Assets/textures/throw_banana.png");
	r_gorilaWalk[0] = { 2, 4, 62, 60};
	r_gorilaWalk[1] = {74,4,78,60};
	r_gorilaWalk[2] = {166,4,62,60};
	r_gorilaWalk[3] = {242,4,82,60};
	
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

	if (bananaOnMap) app->render->DrawTexture(throwBanana, METERS_TO_PIXELS(BananaBox->body->GetPosition().x)-15, METERS_TO_PIXELS(BananaBox->body->GetPosition().y)-25, NULL, SDL_FLIP_HORIZONTAL);;
	
	b2Vec2 movement = { (goRight - goLeft) * speed.x, ColHitbox->body->GetLinearVelocity().y};
	if (!playerHit) ColHitbox->body->SetLinearVelocity(movement);
	else {
		b2Vec2 v = { 0, ColHitbox->body->GetLinearVelocity().y };
		ColHitbox->body->SetLinearVelocity(v);

	}
	if ((playerHP <= 0) || (ColHitbox->body->GetPosition().y > 35))
	{
		PlayerDeath();
		app->audio->PlayFx(playerDeath);
		app->LoadGameRequest();
	}
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
	LOG("player Y %2.2f", ColHitbox->body->GetPosition().y);
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		RestartPlayer();

	}

	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ^ (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) characterWalking = true;
	else
	{
		
		characterWalking = false;
	}
	
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
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		playerHit = true;
		
		
	}
	if (playerHit)
	{
		HitAnimation();
		
	}
	LOG("current time %i", currentTime);

	int gorilaWalkFrameSpeed = 180;
	if ((!onAir)&& (!playerHit))
	{
		if ((app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN))
		{
			playerHP = playerHP - 50;
		}

		if ((playerHP <= 0) || (ColHitbox->body->GetPosition().y > 35))
		{
			PlayerDeath();
			app->LoadGameRequest();
			app->audio->PlayFx(playerDeath);
		}

		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE))
		{
			
			if (!((lastTime + gorilaWalkFrameSpeed > currentTime) && (currentGorilaWalk >= 0)))
			{
				 currentGorilaWalk++;
				lastTime = currentTime;
			}

			if (currentGorilaWalk <= 3)
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk]);
			else
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk - 1]);

			lastDirection = false;

			if ((currentGorilaWalk <= -1) || (currentGorilaWalk >= 4))
			{
				currentGorilaWalk = -1;
			}
			RestartGorilaIdle();
		}


		if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)&& (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE))
		{
			
			if (!((lastTime + gorilaWalkFrameSpeed > currentTime)&&(currentGorilaWalk>=0)))
			
			{
				//app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk], SDL_FLIP_HORIZONTAL);
				currentGorilaWalk++;
				lastTime = currentTime;

			}
			if (currentGorilaWalk <= 3)
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 24 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk], SDL_FLIP_HORIZONTAL);
			else
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 24 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk - 1], SDL_FLIP_HORIZONTAL);

			lastDirection = true;

			if ((currentGorilaWalk <= -1) || (currentGorilaWalk >= 4))
			{
				currentGorilaWalk = -1;
			}
			RestartGorilaIdle();
		}

		//if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
		//{
		//	characterWalking = false;
		//}

		int gorilaSleepFrameSpeed = 2000;

		if (!characterWalking)
		{
			
			if (lastDirection)
			{
				if (lastTime + gorilaSleepFrameSpeed > currentTime)
				{
					app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
				}
				else
				{
					if (currentGorilaIdle < 4)
					{
						currentGorilaIdle++;
						lastTime = currentTime;
						app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
					}
					else
					{
						app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
						app->render->DrawTexture(sleep, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x)-19*2 , METERS_TO_PIXELS(ColHitbox->body->GetPosition().y)-19*4 );
					}


				}


			}
			else if (!lastDirection)
			{
				if (lastTime + gorilaSleepFrameSpeed > currentTime)
				{
					app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);
					
				}
				else
				{
					if (currentGorilaIdle < 4)
					{
						currentGorilaIdle++;
						lastTime = currentTime;
						app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);

					}
					else
					{
						app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);
						app->render->DrawTexture(sleep, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x)+10, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y)-19*4);
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


void Player::HitAnimation()
{
	if (currentGorilaHit < 2)
	{
		if (!lastDirection)
		{
			if (lastTime + 200 > currentTime)
			{
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit]);
			}
			else
			{
				currentGorilaHit++;
				lastTime = currentTime;
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit]);

			}

		}
		else
		{
			if (lastTime + 200 > currentTime)
			{
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit], SDL_FLIP_HORIZONTAL);
			}
			else
			{
				currentGorilaHit++;
				lastTime = currentTime;
				app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit], SDL_FLIP_HORIZONTAL);

			}
		}

	}
	else
	{
		currentGorilaHit = -1;
		app->audio->PlayFx(bananaThrow);
		playerHit = false;
	}

}