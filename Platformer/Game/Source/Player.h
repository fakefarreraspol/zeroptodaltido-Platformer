#pragma once
#include "Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "App.h"
#include "Physics.h"
#include <math.h>
#include "SDL/include/SDL.h"
#include "Render.h"
#include "Animation.h"

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
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool CleanUp();


	PhysBody* GetColHitbox() const
	{
		return ColHitbox;
	}

	PhysBody* GetColSensor() const
	{
		return ColSensor;
	}
	const float getX()
	{
		return x;
	}
	const float getY()
	{
		return y;
	}
	const b2Vec2 GetSpeed()
	{
		return speed;
	}

	const int GetDirection()
	{
		return goRight - goLeft;
	}

	Animation* currentAnimation = nullptr;
	void RestartGorilaIdle()
	{
		currentGorilaIdle = 0;
	}
	
	//void Player::SetAnimation(Animation &toChange)
	//{
		
	//}

private:


	void HitAnimation();
	p2List<PhysBody*> bananasThrown;
	bool goLeft, goRight;

	float startPosX;
	float startPosY;
	int currentTime = 0;
	int lastTime = 0;
	int airTime = 0;
	float x, y;
	b2Vec2 speed;
	float maxXspeed;
	b2Vec2 jumpForce;

	PhysBody* ColHitbox;
	PhysBody* ColSensor;
	PhysBody* BananaBox;
	SDL_Texture* gorila;
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
	bool lastDirection=true;
	bool bananaOnMap = false;
	bool characterWalking = false;
	bool onAir = false;
	bool playerHit = false;
	bool lastBananaDirection = true;
};