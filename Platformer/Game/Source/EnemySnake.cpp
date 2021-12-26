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

	r_snakeIdle[0] = { 113,0,50,47 };
	r_snakeIdle[1] = { 113 + 50 * 1,0,50,47 };
	r_snakeIdle[2] = { 113 + 50 * 2,0,50,47 };
	r_snakeIdle[3] = { 113 + 50 * 3,0,50,47 };
	r_snakeIdle[4] = { 113 + 50 * 4,0,50,47 };
	r_snakeIdle[5] = { 113 + 50 * 5,0,50,47 };
	
	r_snakeAttack[0] = { 113,132,80,47 };
	r_snakeAttack[1] = { 113+80,132,80,47 };
	r_snakeAttack[2] = { 113+80*2,132,80,47 };
	r_snakeAttack[3] = { 113+80*3,132,80,47 };
	
	return true;
}
bool EnemySnake::CleanUp()
{
	LOG("Unloading player");

	return true;
}

bool EnemySnake::Update(float dt)
{
	//app->render->DrawTexture(app->enemyMaster->textureSnake, METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x), METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y), &r_snakeIdle[0]);
	currentTime = SDL_GetTicks();
	
	if (((app->player->GetColHitbox()->body->GetPosition().x) - (Hitbox->body->GetPosition().x)) < 0)
	{
		snakeDirection = true;
	}
	if (((app->player->GetColHitbox()->body->GetPosition().x) - (Hitbox->body->GetPosition().x)) > 0)
	{
		snakeDirection = false;
	}
	if (!snakeAgro)
	{
		if ((lastTime + 400 > currentTime) && (snakeAnim < 5))
		{
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim]
				);

			}


		}
		else if ((lastTime + 400 < currentTime) && (snakeAnim < 5))
		{
			lastTime = currentTime;
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim]
				);

			}

			snakeAnim++;
		}
		else
		{
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim]
				);

			}
			snakeAnim = 0;
		}
	}
	else if(snakeAgro == true)
	{
		if ((snakeAttackTime + 400 > currentTime) && (snakeAttackAnim < 3))
		{
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) -60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeAttack[snakeAttackAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeAttack[snakeAttackAnim]
				);

			}


		}
		else if ((snakeAttackTime + 400 < currentTime) && (snakeAttackAnim < 4))
		{
			snakeAttackTime = currentTime;
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30,
					&r_snakeAttack[snakeAttackAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30,
					&r_snakeAttack[snakeAttackAnim]
				);

			}

			snakeAttackAnim++;
		}
		else
		{
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30 ,
					&r_snakeAttack[snakeAttackAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->enemyMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30,
					&r_snakeAttack[snakeAttackAnim]
				);

			}

			snakeAgro = false;
			snakeAnim = 0;

			snakeAttackAnim = 0;
		}
	}
	
	
	
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

void EnemySnake::DoDamage(int damage)
{
	if (health > 0)
	{
		health -= damage;
		app->audio->PlayFx(app->player->kick);
	}
	if (health <= 0)
	{
		app->enemyMaster->DestroyEnemy(Hitbox);
		app->audio->PlayFx(app->player->enemy_death);

	}
}

void EnemySnake::SnakeAttack()
{
	
}