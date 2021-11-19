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

	//void Player::SetAnimation(Animation &toChange)
	//{
		
	//}

private:

	bool goLeft, goRight;

	float startPosX;
	float startPosY;

	float x, y;
	b2Vec2 speed;
	float maxXspeed;
	b2Vec2 jumpForce;

	PhysBody* ColHitbox;
	PhysBody* ColSensor;
	SDL_Texture* texture;
	

};