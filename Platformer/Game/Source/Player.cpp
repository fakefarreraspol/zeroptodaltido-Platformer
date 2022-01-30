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
#include "Window.h"
#include "Audio.h";
#include "EntityHandler.h"
#include <string.h>

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
	if (app->scene->state == GAMEPLAY)
	{
		playerDeath = app->audio->LoadFx("Assets/audio/fx/player_death.wav");
		healingSound = app->audio->LoadFx("Assets/audio/fx/healing.wav");
		bananaThrow = app->audio->LoadFx("Assets/audio/fx/player_shot.wav");
		playerHurt = app->audio->LoadFx("Assets/audio/fx/monkey_hurt.wav");
		playerHurt2 = app->audio->LoadFx("Assets/audio/fx/monkey_hurt_2.wav");
		playerHurt3 = app->audio->LoadFx("Assets/audio/fx/monkey_hurt_3.wav");
		kick = app->audio->LoadFx("Assets/audio/fx/kick.wav");
		enemy_death = app->audio->LoadFx("Assets/audio/fx/enemy_death.wav");
		itemPickup = app->audio->LoadFx("Assets/audio/fx/item_pickup.wav");
		//textures
		gorila = app->tex->Load("Assets/textures/gorila.png");
		panel = app->tex->Load("Assets/textures/transparent_black_square_50.png");
		gorilaFace = app->tex->Load("Assets/textures/gorila_face_icon.png");
		sleep = app->tex->Load("Assets/textures/mini_zzz.png");
		throwBanana = app->tex->Load("Assets/textures/throw_banana.png");
		mango = app->tex->Load("Assets/textures/mango.png");
		r_gorilaWalk[0] = { 2, 4, 62, 60 };
		r_gorilaWalk[1] = { 74,4,78,60 };
		r_gorilaWalk[2] = { 166,4,62,60 };
		r_gorilaWalk[3] = { 242,4,82,60 };

		r_gorilaPunch[0] = { 4, 116, 72, 64 };
		r_gorilaPunch[1] = { 92,116,86,72 };

		r_gorilaJump[0] = { 628,248,62,64 };
		r_gorilaJump[1] = { 538,248,76,64 };
		r_gorilaJump[2] = { 412,248,110,64 };
		r_gorilaJump[3] = { 344,236,54,76 };

		r_gorilaIdle[0] = { 348,0,60,64 };
		r_gorilaIdle[1] = { 408,0,60,64 };
		r_gorilaIdle[2] = { 471,0,60,64 };
		r_gorilaIdle[3] = { 540,0,74,64 };
		r_gorilaIdle[4] = { 622,0,104,64 };
		//player stats
		startPosX = 48 * 4;
		startPosY = 48 * 22;
		speed = { 8.f,8.f };
		jumpForce = { 0,-27.f };


		ColHitbox = app->physics->CreateCircle(startPosX, startPosY, 23);
		ColHitbox->type = TYPE_PLAYER;
		ColHitbox->body->GetFixtureList()->SetDensity(3.0f);
		ColHitbox->body->ResetMassData();



		int x_ = (int)x;
		int y_ = (int)y;
		ColHitbox->GetPosition(x_, y_);

		//ColSensor = app->physics->CreateRectangle(x, y, 46, 50);
		//ColSensor->body->GetFixtureList()->SetSensor(true);
		//ColSensor->body->SetType(b2_kinematicBody);

		goLeft = false;
		goRight = false;

		healingCooldown = 0;

		LOG("Loading player");
	}
	
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
	//LOG("attempts: %i", playerLifes);

	if (app->scene->state == GAMEPLAY) 
	{

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->SetPause(!app->GameIsPaused());
		}

		if (!app->GameIsPaused())
		{
			currentTime += 16;
		}
		
		b2Vec2 pos = { x,y };

		
			goLeft = (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT);
			goRight = (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT);

		
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			RestartPlayer();
			app->render->camera.x = 0;
			app->render->camera.y = -48 * 14;
		
		}

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			AdminMode = !AdminMode;
			ColHitbox->body->ResetMassData();
			if (!ColHitbox->body->IsAwake())
				ColHitbox->body->SetAwake(true);
		}


		if (!AdminMode)
		{
			ColHitbox->body->SetGravityScale(1);

			b2Vec2 movement = { (goRight - goLeft) * speed.x, ColHitbox->body->GetLinearVelocity().y };
			if (!playerHit) ColHitbox->body->SetLinearVelocity(movement);
			else {
				b2Vec2 v = { 0, ColHitbox->body->GetLinearVelocity().y };
				ColHitbox->body->SetLinearVelocity(v);

			}

			if (ColHitbox->body->GetFixtureList()->IsSensor())
			{
				ColHitbox->body->GetFixtureList()->SetSensor(false);
			}
		}
		else
		{

			if (!ColHitbox->body->GetFixtureList()->IsSensor())
			{
				ColHitbox->body->GetFixtureList()->SetSensor(true);
			}
			bool goUp = false;
			bool goDown = false;

			
				goUp = (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT);
				goDown = (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT);
			
			b2Vec2 movement = { (goRight - goLeft) * speed.x, (goDown - goUp) * speed.y};
			if (!playerHit) ColHitbox->body->SetLinearVelocity(movement);
			ColHitbox->body->SetGravityScale(0);
			
		}

		b2Body* ground;
		if (ColHitbox->body->GetContactList() != nullptr)
		{
			ground = ColHitbox->body->GetContactList()->contact->GetFixtureA()->GetBody();

			if (ground != nullptr)
			{

				b2Vec2 xVel = { 0,ColHitbox->body->GetLinearVelocity().y };
				if (!goLeft && !goRight) ColHitbox->body->SetLinearVelocity(xVel);

				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->GameIsPaused())
				{
					RestartGorilaIdle();
					b2Vec2 yVel = { ColHitbox->body->GetLinearVelocity().x,0 };
					ColHitbox->body->SetLinearVelocity(yVel);
					ColHitbox->body->ApplyLinearImpulse(jumpForce, ColHitbox->body->GetPosition(), true);
					ColHitbox->body->SetLinearDamping(0);
				}
			}
		}
		if (lastPlayerHP > playerHP)
		{
			audioHurt = currentTime % 2;
			if (audioHurt == 0)
			{
				app->audio->PlayFx(playerHurt3);
			}
			else if (audioHurt == 1)
			{
				app->audio->PlayFx(playerHurt2);
			}
			/*else
			{
				app->audio->PlayFx(playerHurt3);
			}*/
			lastPlayerHP = playerHP;
		}

		//LOG("x: %f", ColHitbox->body->GetPosition().x);


		if (!app->GameIsPaused())
		{
			if (iFramesActive)
			{
				currentIFrameTime++;
				int blinkTime = 20;

				if (currentIFrameTime % blinkTime >= 0 && currentIFrameTime % blinkTime <= blinkTime / 2)
				{
					drawGorila = true;
				}
				else
				{
					drawGorila = false;
				}

				if (currentIFrameTime >= iFrameReference)
				{
					drawGorila = true;
					iFramesActive = false;
				}
			}
		}


		if (!app->GameIsPaused())
		{
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ^ (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) characterWalking = true;
			else
			{
				characterWalking = false;
			}
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
						if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaJump[currentGorilaJump]);
					}
					else if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaJump[currentGorilaJump], SDL_FLIP_HORIZONTAL);
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
						if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaJump[currentGorilaJump]);
					}
				}
			}
			else
			{
				inAir = false;
			}


		}*/

		
			if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
			{
				if (!app->GameIsPaused() && bananasCollected > 0)
				{
					playerHit = true;
					bananasCollected--;
				}

			}
			if (playerHit)
			{
				HitAnimation();
			}
		
		////LOG("current time %i", currentTime);
		//if ((app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN))
		//{
		//	HurtGorila(1);
		//}


		if (!app->GameIsPaused())
		{
			if ((app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) && (playerHP < 3) && healingCooldown <= 0)
			{
				playerHP++;
				lastPlayerHP++;
				app->audio->PlayFx(healingSound);
				healingCooldown = healingCooldownMax;
			}
			else
			{
				healingCooldown--;
			}


			if (healingCooldown <= 0) healingCooldown = 0;
		}




		//if ((!healingUsed) && (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)&&(playerHP<3))
		//{
		//	healingUsed = true;
		//	healingCooldown = currentTime;
		//	playerHP++;
		//	lastPlayerHP++;
		//	app->audio->PlayFx(healingSound);
		//}
		//if ((healingUsed) && (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) && (playerHP < 3) && (healingCooldown + healingCooldownMax < currentTime))
		//{
		//	healingCooldown = currentTime;
		//	playerHP++;
		//	lastPlayerHP++;
		//	app->audio->PlayFx(healingSound);
		//}

		
		int gorilaWalkFrameSpeed = 150;
		if ((!onAir) && (!playerHit))
		{

			

			if ((playerHP <= 0) || (ColHitbox->body->GetPosition().y > 35))
			{
				
				app->audio->PlayFx(playerDeath);
				app->LoadGameRequest();
				PlayerDeath();
				
				app->render->camera.x = 0;
				app->render->camera.y = -48 * 14;
			}

			if (!app->GameIsPaused())
			{
				if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE))
				{

					if (!((lastTime + gorilaWalkFrameSpeed > currentTime) && (currentGorilaWalk >= 0)))
					{
						currentGorilaWalk++;
						lastTime = currentTime;
					}

					if (currentGorilaWalk <= 3)
						if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk]);
						else
							if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk - 1]);

					lastDirection = false;

					if ((currentGorilaWalk <= -1) || (currentGorilaWalk >= 4))
					{
						currentGorilaWalk = -1;
					}
					RestartGorilaIdle();
				}



				if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE))
				{

					if (!((lastTime + gorilaWalkFrameSpeed > currentTime) && (currentGorilaWalk >= 0)))

					{
						//if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk], SDL_FLIP_HORIZONTAL);
						currentGorilaWalk++;
						lastTime = currentTime;

					}
					if (currentGorilaWalk <= 3)
						if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 24 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk], SDL_FLIP_HORIZONTAL);
						else
							if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 24 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaWalk[currentGorilaWalk - 1], SDL_FLIP_HORIZONTAL);

					lastDirection = true;

					if ((currentGorilaWalk <= -1) || (currentGorilaWalk >= 4))
					{
						currentGorilaWalk = -1;
					}
					RestartGorilaIdle();
				}
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
						if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
					}
					else
					{
						if (currentGorilaIdle < 4)
						{

							lastTime = currentTime;
							if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
							currentGorilaIdle++;
						}
						else
						{
							if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle], SDL_FLIP_HORIZONTAL);
							app->render->DrawTexture(sleep, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 19 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 4);
						}


					}


				}
				else if (!lastDirection)
				{
					if (lastTime + gorilaSleepFrameSpeed > currentTime)
					{
						if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);

					}
					else
					{
						if (currentGorilaIdle < 4)
						{
							currentGorilaIdle++;
							lastTime = currentTime;
							if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);

						}
						else
						{
							if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - 20 * 2, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaIdle[currentGorilaIdle]);
							app->render->DrawTexture(sleep, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) + 10, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 4);
						}


					}
				}
			}

		}

		b2Body* item = nullptr;
		if (ColHitbox->body != nullptr && ColHitbox->body->GetContactList() != nullptr)
			item = ColHitbox->body->GetContactList()->contact->GetFixtureA()->GetBody();

		if (app->entityMaster->GetEntityType(item) == ITEM_BANANA)
		{
			bananasCollected += 3;
			app->audio->PlayFx(itemPickup);
			app->entityMaster->DestroyEnemy(item);
			
		}

		p2List_item<PhysBody*>* currentBanana = bananasThrown.getFirst();
		while (currentBanana != NULL)
		{

			if (!PhysBodyIsInMap(currentBanana->data))
			{
				app->physics->GetWorld()->DestroyBody(currentBanana->data->body);
				bananasThrown.del(currentBanana);
			}

			if (bananaOnMap)
			{
				b2Vec2 v(currentBanana->data->body->GetLinearVelocity().x, -0.408f);
				currentBanana->data->body->SetLinearVelocity(v);

			}
			

			b2Body* bananaHit;
			if (currentBanana->data->body->GetContactList() != nullptr)
			{
				//LOG("true");
				bananaHit = currentBanana->data->body->GetContactList()->contact->GetFixtureA()->GetBody();
				
				if (bananaHit != nullptr && bananaHit != ColHitbox->body && bananaHit != currentBanana->data->body)
				{
					LOG("hit");
					LOG("type: %i", bananaHit->GetType());
					LOG("type dynamic: %i", b2_dynamicBody);
					//LOG("type stat: %i", b2_staticBody);
					//LOG("type kin: %i", b2_kinematicBody);
					
						app->entityMaster->DamageEnemy(bananaHit, 1);

						b2Vec2 hitForce(10.f, 0);
						b2Vec2 hitDir = bananaHit->GetLinearVelocity();
						hitDir.Normalize();
						hitForce.x *= hitDir.x;
						LOG("force %f", hitForce.x);
						LOG("dir %f", hitDir.x); 
						b2Vec2 xVel = { 0,bananaHit->GetLinearVelocity().y };
						bananaHit->SetLinearVelocity(xVel);
						bananaHit->ApplyLinearImpulse(hitForce, bananaHit->GetPosition(), true);
						bananaHit->SetLinearDamping(0);

						app->physics->GetWorld()->DestroyBody(currentBanana->data->body);
						
						bananasThrown.del(currentBanana);
					
					//switch (bananaHit->type)
					//{
					//
					//case TYPE_SOLID_TILE:
					//	app->physics->GetWorld()->DestroyBody(currentBanana->data->body);
					//	//banana hit sound
					//	break;
					//
					//	/*
					//	case ENEMY:
					//		damage enemy
					//		destroy banana
					//		sound
					//	
					//	*/
					//
					//default:
					//	app->physics->GetWorld()->DestroyBody(currentBanana->data->body);
					//	break;
					//}
			
					
				}
			}


			int x, y;
			currentBanana->data->GetPosition(x, y);
			app->render->DrawTexture(throwBanana, x / app->win->GetScale() - 17, y / app->win->GetScale() - 17, NULL);
			

			currentBanana = currentBanana->next;
		}

		

	}
	

	

	return true;
}

bool Player::PostUpdate()
{

	switch (app->scene->state)
	{
	case INTRO:
	{

	}
	break;
	case GAMEPLAY:
	{

		SDL_Rect leaf = { 0,748,213,126 };
		app->render->DrawTexture(app->UI_handler->spritesheet, 0, 720 - 140, &leaf, SDL_FLIP_NONE, 0);
		app->render->DrawTexture(throwBanana, 48, 720 - 90, NULL, SDL_FLIP_NONE, 0);

		char val[4] = { 0 };

		sprintf_s(val, "%i", bananasCollected);

		app->fonts->DrawText(37 + 48 , 720 - 80, app->scene->font1_gold_2, ":");
		app->fonts->DrawText(37 + 48 + 2 * 16, 720 - 80, app->scene->font1_gold_2, val);

		for (int i = 0; i < playerHP; i++)
		{
			app->render->DrawTexture(mango, 96 + (48 + 10) * (i), 32, NULL, SDL_FLIP_NONE, 0);

		}
		//app->render->DrawTexture(mango, 100, 20, NULL, SDL_FLIP_NONE, 0);
		app->render->DrawTexture(gorilaFace, 8, 32, NULL, SDL_FLIP_NONE, 0);

		//LOG("c : %i", healingCooldown);
		float skill_time = (float)healingCooldown;

		float skill_fill_f = (healingCooldownMax - skill_time) / healingCooldownMax * 132;
		int skill_fill_i = (int)skill_fill_f;

		SDL_Rect r = { 264, 92, skill_fill_i, 20 };

		if (app->scene->UI_player_skill_bar_fill != nullptr)
			app->scene->UI_player_skill_bar_fill->rec_sprite = r;
		//LOG("fill: %i", skill_fill_i);
		//LOG("fill: %f", (healingCooldownMax - skill_time) / healingCooldownMax);
		//LOG("skill: %f", skill_fill_f);

	}
	break;
	case END:
	{
		app->scene->UI_player_skill_bar_fill->SetActive(false);
		app->scene->UI_player_skill_bar_bg->SetActive(false);
		app->scene->UI_player_lifes->SetActive(false);
		app->scene->UI_player_skill_icon->SetActive(false);
		app->scene->UI_button_open_pause_menu->SetActive(false);

	}
	break; 
	default:
		break;
	}
	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	data = data.child("Player");

	b2Vec2 currentPos(data.attribute("x").as_float(), data.attribute("y").as_float());
	ColHitbox->body->SetTransform(currentPos, 0);

	//LOG("pos, %i %i", METERS_TO_PIXELS(Hitbox->body->GetPosition().x), METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("pos, %i %i", METERS_TO_PIXELS(currentPos.x), METERS_TO_PIXELS(currentPos.y));

	playerHP = data.attribute("health").as_int();
	healingCooldown = data.attribute("cooldown").as_int();
	bananasCollected = data.attribute("bananas").as_int();
	//playerLifes = data.attribute("attempts").as_int();


	/*startPosX = data.child("startPos").attribute("x").as_float(0);
	startPosY = data.child("startPos").attribute("y").as_float(0);

	b2Vec2 v = { PIXEL_TO_METERS( startPosX), PIXEL_TO_METERS(startPosY )};
	RestartPlayer();

	ColHitbox->body->SetTransform(v, 0);
	playerHP = data.child("health").attribute("value").as_int(3);
	healingCooldown = data.child("cooldown").attribute("value").as_int(20000);
	lastPlayerHP = playerHP;*/
	//playerLifes = data.child("attempts").attribute("value").as_int(3);

	return true;
}


bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node iteratorRemove = data.first_child();

	while (iteratorRemove.next_sibling())
	{
		pugi::xml_node toRemove = iteratorRemove.next_sibling();
		data.remove_child(toRemove);
	}
	data.remove_child(iteratorRemove);

	pugi::xml_node myself = data.append_child("Player");

	myself.append_attribute("x").set_value(ColHitbox->body->GetPosition().x);
	myself.append_attribute("y").set_value(ColHitbox->body->GetPosition().y);
	myself.append_attribute("health").set_value(playerHP);
	myself.append_attribute("cooldown").set_value(healingCooldown);
	myself.append_attribute("bananas").set_value(bananasCollected);
	myself.append_attribute("attempts").set_value(playerLifes);


	LOG("saving player");
	//data.child("startPos").attribute("x").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x));
	//data.child("startPos").attribute("y").set_value(METERS_TO_PIXELS(ColHitbox->body->GetPosition().y));
	//data.child("health").attribute("value").set_value(playerHP);
	//data.child("cooldown").attribute("value").set_value(healingCooldown);
	//data.child("attempts").attribute("value").set_value(playerLifes);
	return true;
}


void Player::HitAnimation()
{
	RestartGorilaIdle();
	int dist = 24;
	if (currentGorilaHit < 2)
	{
		
		if (!lastDirection)
		{
			if (lastTime + 200 > currentTime)
			{
				if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - dist, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit]);
				
			}
			else
			{
				currentGorilaHit++;
				lastTime = currentTime;
				if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - dist, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit]);

			}

		}
		else
		{
			if (lastTime + 200 > currentTime)
			{
				if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - dist, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit], SDL_FLIP_HORIZONTAL);
			}
			else
			{
				currentGorilaHit++;
				lastTime = currentTime;
				if (drawGorila) app->render->DrawTexture(gorila, METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - dist, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y) - 19 * 2, &r_gorilaPunch[currentGorilaHit], SDL_FLIP_HORIZONTAL);

			}
		}

	}
	else
	{
		if (!lastDirection) {
			BananaBox = app->physics->CreateCircle(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) - dist, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y), 10);
			lastBananaDirection = false;
		}
		else
		{
			BananaBox = app->physics->CreateCircle(METERS_TO_PIXELS(ColHitbox->body->GetPosition().x) + dist, METERS_TO_PIXELS(ColHitbox->body->GetPosition().y), 10);
			lastBananaDirection = true;
		}
		BananaBox->body->SetType(b2_dynamicBody);
		BananaBox->type = TYPE_BULLET;
		b2Vec2 bananaMovement{ 12, 0 };
		if (lastBananaDirection)
		{


			BananaBox->body->SetLinearVelocity(bananaMovement);
		}
		else
		{

			BananaBox->body->SetLinearVelocity(-bananaMovement);
		}
		bananasThrown.add(BananaBox);
		BananaBox->body->GetFixtureList()->SetSensor(true);
		app->audio->PlayFx(bananaThrow);
		currentGorilaHit = 0;
		bananaOnMap = true;
		playerHit = false;
	}

}