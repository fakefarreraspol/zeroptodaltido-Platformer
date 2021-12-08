#ifndef __PLAYER_H__
#define __PLAYER_H__

#define PLAYER_MAX_HP 5
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
	
	void PlayerDeath()
	{
		playerLifes--;
		playerHP = PLAYER_MAX_HP;
	}

	void RestartPlayer()
	{
		app->render->camera.x = 0;
		app->render->camera.y = -48 * 14;

		b2Vec2 v = { PIXEL_TO_METERS(gorilaStartX), PIXEL_TO_METERS(gorilaStartY) };
		ColHitbox->body->SetTransform(v, 0);
		playerHP = PLAYER_MAX_HP;
		playerLifes = 3;
	}

	void HurtGorila(int damage)
	{
		iFramesActive = true;
		currentIFrameTime = 0;
		playerHP -= damage;
	}
	/*enum GameState
	{
		INTRO = 0,
		GAMEPLAY,
		END
	};
	GameState state;
	*/
private:


	void HitAnimation();
	p2List<PhysBody*> bananasThrown;
	bool goLeft, goRight;

	float startPosX;
	float startPosY;
	int currentIFrameTime = 0;
	int iFrameReference = 120;
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
	
	uint bananaThrow = 0;
	uint playerDeath = 0;
	uint playerHurt = 0;
	uint playerHurt2 = 0;
	uint playerHurt3 = 0;
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