#ifndef __PLAYER_H__
#define __PLAYER_H__

#define PLAYER_MAX_HP 3
#include "Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "App.h"
#include "Physics.h"
#include <math.h>
#include "SDL/include/SDL.h"
#include "Render.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Map.h"


class Player : public Module
{
public:
	Player();
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();


	PhysBody* GetColHitbox() const
	{
		return ColHitbox;
	}


	const float getX() const
	{
		return x;
	}
	const float getY() const
	{
		return y;
	}
	const b2Vec2 GetSpeed() const
	{
		return speed;
	}

	const int GetDirection() const
	{
		return goRight - goLeft;
	}

	Animation* currentAnimation = nullptr;
	void RestartGorilaIdle()
	{
		currentGorilaIdle = 0;
	}
	
	void PlayerDeath()
	{
		playerLifes--;
	}
	int GetPlayerLifes() const
	{
		return playerLifes;
	}
	void RestartPlayer()
	{

		b2Vec2 v = { PIXEL_TO_METERS(gorilaStartX), PIXEL_TO_METERS(gorilaStartY) };
		ColHitbox->body->SetTransform(v, 0);
		playerHP = PLAYER_MAX_HP;
		lastPlayerHP = playerHP;
		
		b2Vec2 s(0, 0);
		ColHitbox->body->SetLinearVelocity(s);

		//quit when adding endings
		//playerLifes = 3;
	}

	

	void HurtGorila(int damage)
	{
		if (!AdminMode)
		{
			if (!iFramesActive)
			{
				iFramesActive = true;
				currentIFrameTime = 0;
				playerHP -= damage;
			}
		}
	}
	/*enum GameState
	{
		INTRO = 0,
		GAMEPLAY,
		END
	};
	GameState state;
	*/

	bool PhysBodyIsInMap(PhysBody* phys)
	{
		iPoint positionInMap(
			app->map->WorldToMap(
				METERS_TO_PIXELS(phys->body->GetPosition().x),
				METERS_TO_PIXELS(phys->body->GetPosition().y)
			)
		);

		return app->pathfinding->CheckBoundaries(positionInMap);

	}
	uint kick = 0;
	uint enemy_death = 0;
	int healingCooldown = 0;

	int bananasCollected;
private:

	bool AdminMode;

	void HitAnimation();
	p2List<PhysBody*> bananasThrown;
	bool goLeft, goRight;
	bool healingUsed = false;
	
	int healingCooldownMax = 5000;
	float startPosX;
	float startPosY;
	int currentIFrameTime = 0;
	int iFrameReference = 180;
	bool iFramesActive = false;
	bool drawGorila = true;
	int currentTime = 0;
	int lastTime = 0;
	int airTime = 0;
	float x, y;
	b2Vec2 speed;
	float maxXspeed;
	b2Vec2 jumpForce;

	int gorilaStartX = 48 * 4;
	int gorilaStartY = 48 * 22;
	uint healingSound = 0;
	uint bananaThrow = 0;
	uint playerDeath = 0;
	uint playerHurt = 0;
	uint playerHurt2 = 0;
	uint playerHurt3 = 0;
	uint itemPickup = 0;
	
	unsigned int audioHurt= 0;
	PhysBody* ColHitbox;
	PhysBody* BananaBox;
	SDL_Texture* mango;
	SDL_Texture* panel;
	SDL_Texture* gorila;
	SDL_Texture* gorilaFace;
	SDL_Texture* throwBanana;
	SDL_Texture* sleep;
	SDL_Rect r_gorilaWalk[4];
	SDL_Rect r_gorilaIdle[5];
	SDL_Rect r_gorilaJump[4];
	SDL_Rect r_gorilaPunch[2];
	int currentGorilaWalk = -1;
	int currentGorilaIdle = -1;
	int currentGorilaJump = -1;
	int currentGorilaHit = 0;
	unsigned int playerHP = PLAYER_MAX_HP;
	unsigned int lastPlayerHP = PLAYER_MAX_HP;
	unsigned int playerLifes = 3;
	bool lastDirection=true;
	bool bananaOnMap = false;
	bool characterWalking = false;
	bool onAir = false;
	bool playerHit = false;
	bool lastBananaDirection = true;
};

#endif // !__PLAYER_H__