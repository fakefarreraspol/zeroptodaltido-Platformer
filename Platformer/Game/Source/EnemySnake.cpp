#include "Player.h"
#include "Input.h"
#include "SDL/include/SDL.h"
#include "p2List.h"
#include "Scene.h"
#include "Physics.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h";
#include "EnemySnake.h"

EnemySnake::EnemySnake(b2Vec2 startPosition, int health) : Entity()
{
	spawnPosition = startPosition;
	name.Create("enemySnake");
	Hitbox = app->physics->CreateStaticCircle(spawnPosition.x, spawnPosition.y, 20);
	Hitbox->body->SetSleepingAllowed(false);
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
	//app->render->DrawTexture(app->entityMaster->textureSnake, METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x), METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y), &r_snakeIdle[0]);
	if (!app->GameIsPaused())
	{
		currentTime = SDL_GetTicks();
	}
	
	if (((app->player->GetColHitbox()->body->GetPosition().x) - (Hitbox->body->GetPosition().x)) < 0)
	{
		snakeDirection = true;
	}
	if (((app->player->GetColHitbox()->body->GetPosition().x) - (Hitbox->body->GetPosition().x)) > 0)
	{
		snakeDirection = false;
	}

	if (snakeAgro)
	{
		if (CheckDistanceToPhysBody(app->player->GetColHitbox()) <= maxDistanceAgroBase)
		{
			snakeAgro = true;
			
		}
		else {
			snakeAgro = false;
			
		}
	}
	else
	{
		if (CheckDistanceToPhysBody(app->player->GetColHitbox()) <= maxDistanceAgroBase)
		{
			snakeAgro = true;
			

		}
		else {
			snakeAgro = false;
		
		}
	}

	if (!snakeAgro)
	{
		snakeAttackAnim = 0;
		if ((lastTime + 400 > currentTime) && (snakeAnim < 5))
		{
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
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
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
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
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 30,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeIdle[snakeAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
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
		if ((snakeAttackTime + 400 > currentTime) && (snakeAttackAnim < 4))
		{
			if (snakeDirection)
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) -60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) - 30,
					&r_snakeAttack[snakeAttackAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x)-20 ,
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
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30,
					&r_snakeAttack[snakeAttackAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x)-20 ,
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
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x) - 60,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30 ,
					&r_snakeAttack[snakeAttackAnim], SDL_FLIP_HORIZONTAL
				);
			}
			else
			{
				app->render->DrawTexture(
					app->entityMaster->textureSnake,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().x)-20,
					METERS_TO_PIXELS(this->Hitbox->body->GetPosition().y) -30,
					&r_snakeAttack[snakeAttackAnim]
				);

			}
			
			snakeAgro = false;
			snakeAnim = 0;

			snakeAttackAnim = 0;
		}
	}
	if ((snakeAttackAnim == 2)&&(attackCooldown<currentTime))
	{
		attackCooldown = currentTime + 400;
		acidOnMap = true;
		if (snakeDirection) {
			acidBox = app->physics->CreateCircle(METERS_TO_PIXELS(Hitbox->body->GetPosition().x) - 24, METERS_TO_PIXELS(Hitbox->body->GetPosition().y)-10, 8);
			lastAcidDirection = false;
		}
		else
		{
			acidBox = app->physics->CreateCircle(METERS_TO_PIXELS(Hitbox->body->GetPosition().x) + 24, METERS_TO_PIXELS(Hitbox->body->GetPosition().y)-10, 8);
			lastAcidDirection = true;
		}
		acidBox->body->SetType(b2_dynamicBody);
		acidBox->type = TYPE_BULLET;
		b2Vec2 acidMovement{ 8, 0 };
		if (lastAcidDirection)
		{


			acidBox->body->SetLinearVelocity(acidMovement);
		}
		else
		{

			acidBox->body->SetLinearVelocity(-acidMovement);
		}
		acidThrown.add(acidBox);
		//acidBox->body->GetFixtureList()->SetSensor(true);
		//acidBox->body->SetSleepingAllowed(false);
	}
	
	p2List_item<PhysBody*>* currentAcid = acidThrown.getFirst();
	while (currentAcid != NULL)
	{

		if (!PhysBodyIsInMap(currentAcid->data))
		{
			app->physics->GetWorld()->DestroyBody(currentAcid->data->body);
			acidThrown.del(currentAcid);
		}

		if (acidOnMap)
		{
			b2Vec2 v(currentAcid->data->body->GetLinearVelocity().x, -0.408f);
			currentAcid->data->body->SetLinearVelocity(v);

		}
		
		b2Body* playerBox = nullptr;

		if (Hitbox->body->GetContactList() != nullptr)
		{
			playerBox = Hitbox->body->GetContactList()->contact->GetFixtureB()->GetBody();
		}

		if ( playerBox != nullptr && playerBox == app->player->GetColHitbox()->body)
		{
			float margin = PIXEL_TO_METERS(40);

			if (playerBox->GetPosition().x > Hitbox->body->GetPosition().x - margin &&
				playerBox->GetPosition().x < Hitbox->body->GetPosition().x + margin &&
				playerBox->GetPosition().y < Hitbox->body->GetPosition().y)
			{
				app->entityMaster->DestroyEnemy(Hitbox->body);
				app->player->healingCooldown -= 1000;
				app->player->RestartGorilaIdle();

				b2Vec2 jumpForce(0, -10.f);
				b2Vec2 yVel = { playerBox->GetLinearVelocity().x,0 };
				playerBox->SetLinearVelocity(yVel);
				playerBox->ApplyLinearImpulse(jumpForce, playerBox->GetPosition(), true);
				playerBox->SetLinearDamping(0);
				app->audio->PlayFx(app->player->enemy_death);
			}
		}

		b2Body* acidHit = nullptr;
		if (currentAcid->data->body->GetContactList() != nullptr)
		{
			//LOG("true");
			acidHit = currentAcid->data->body->GetContactList()->contact->GetFixtureB()->GetBody();

			if (acidHit != nullptr && acidHit != Hitbox->body && acidHit != currentAcid->data->body)
			{
				LOG("hit");
				LOG("type: %i", acidHit->GetType());
				LOG("type dynamic: %i", b2_dynamicBody);
				//LOG("type stat: %i", b2_staticBody);
				//LOG("type kin: %i", b2_kinematicBody);

				if (acidHit == app->player->GetColHitbox()->body) app->player->HurtGorila(1);

				b2Vec2 hitForce(10.f, 0);
				b2Vec2 hitDir = acidHit->GetLinearVelocity();
				hitDir.Normalize();
				hitForce.x *= hitDir.x;
				LOG("force %f", hitForce.x);
				LOG("dir %f", hitDir.x);
				b2Vec2 xVel = { 0,acidHit->GetLinearVelocity().y };
				acidHit->SetLinearVelocity(xVel);
				acidHit->ApplyLinearImpulse(hitForce, acidHit->GetPosition(), true);
				acidHit->SetLinearDamping(0);

				app->physics->GetWorld()->DestroyBody(currentAcid->data->body);

				acidThrown.del(currentAcid);

			}

			
		}
		if (currentAcid->data->body->GetContactList() != nullptr)
		{
			acidHit = currentAcid->data->body->GetContactList()->contact->GetFixtureA()->GetBody();

			if (acidHit != nullptr && acidHit != Hitbox->body && acidHit != currentAcid->data->body)
			{
				LOG("hit");
				LOG("type: %i", acidHit->GetType());
				LOG("type dynamic: %i", b2_dynamicBody);
				//LOG("type stat: %i", b2_staticBody);
				//LOG("type kin: %i", b2_kinematicBody);

				b2Vec2 hitForce(10.f, 0);
				b2Vec2 hitDir = acidHit->GetLinearVelocity();
				hitDir.Normalize();
				hitForce.x *= hitDir.x;
				LOG("force %f", hitForce.x);
				LOG("dir %f", hitDir.x);
				b2Vec2 xVel = { 0,acidHit->GetLinearVelocity().y };
				acidHit->SetLinearVelocity(xVel);
				acidHit->ApplyLinearImpulse(hitForce, acidHit->GetPosition(), true);
				acidHit->SetLinearDamping(0);

				app->physics->GetWorld()->DestroyBody(currentAcid->data->body);

				acidThrown.del(currentAcid);

			}
		}


		int x, y;
		currentAcid->data->GetPosition(x, y);
		SDL_Rect rec_acid = { 167, 214,24,9 };

		if (snakeDirection)
		{
			app->render->DrawTexture(app->entityMaster->textureSnake, x / app->win->GetScale() - 5, y / app->win->GetScale() - 2, &rec_acid, SDL_FLIP_HORIZONTAL);
		}
		else 
		{
			app->render->DrawTexture(app->entityMaster->textureSnake, x / app->win->GetScale() - 5, y / app->win->GetScale() - 2, &rec_acid);

		}

		currentAcid = currentAcid->next;
	}
	


	return true;
}

bool EnemySnake::LoadState(pugi::xml_node& data)
{
	id = data.attribute("id").as_int();
	b2Vec2 currentPos(data.attribute("currentPos.x").as_float(), data.attribute("currentPos.y").as_float());
	Hitbox->body->SetTransform(currentPos, 0);


	return true;
}


bool EnemySnake::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node myself = data.append_child("EnemySnake");

	myself.append_attribute("id").set_value(id);
	myself.append_attribute("currentPos.x").set_value(Hitbox->body->GetPosition().x);
	myself.append_attribute("currentPos.y").set_value(Hitbox->body->GetPosition().y);

	//myself.append_attribute("checkTimer").set_value(checkTimer);
	//myself.append_attribute("maxDistanceAgro").set_value(maxDistanceAgro);
	//
	//myself.append_attribute("currentSpeed.x").set_value(currentSpeed.x);
	//myself.append_attribute("currentSpeed.y").set_value(currentSpeed.y);
	//
	//myself.append_attribute("agroTowardsPlayer").set_value(agroTowardsPlayer);
	//myself.append_attribute("health").set_value(health);
	//myself.append_attribute("direction").set_value(direction);

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
		app->entityMaster->DestroyEnemy(Hitbox->body);

		


		app->audio->PlayFx(app->player->enemy_death);

	}
}

