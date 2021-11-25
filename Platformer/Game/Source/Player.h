#pragma once
#include "Module.h"
#include "p2Point.h"
#include "App.h"
#include "Physics.h"
#include <math.h>
#include "SDL/include/SDL.h"

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
	SDL_Texture* gorila;
	SDL_Texture* sleep;
	SDL_Rect r_gorilaWalk[4];
	SDL_Rect r_gorilaIdle[5];
	SDL_Rect r_gorilaJump[4];
	SDL_Rect r_gorilaPunch[2];
	int currentGorilaWalk = 0;
	int currentGorilaIdle = 0;
	int currentGorilaJump = 0;
	bool lastDirection=true;
	bool characterWalking = false;
	bool inAir = false;
	
};