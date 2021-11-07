#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Physics.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();
	uint jungleMusic = 0;
	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	SDL_Texture* bTexture = nullptr;

	float playerX = 0.0f;
	float playerY = 0.0f;
	bool freeCam = false;
private:

	SDL_Texture* texBackground;
	p2List<PhysBody*> trespasableElements;

	int squareGround[44] = {
		6,7,39,40,42,43,45,46,
		58,59,60,61,62,63,64,65,
		77,78,79,80,81,82,83,84,
		85,86,87,88,96,97,100,101,
		102,103,104,105,206,107,
		108,109,116,117,119,120 
	};

	int rightSlope_45[6] = 
	{
		0,48,
		48,48,
		48,0
	};
	int leftSlope_45[6] =
	{
		0,0,
		0,48,
		48,48
	};

	int rightSlope_30_1[6] =
	{
		0,48,
		48,48,
		48,32

	};
	int leftSlope_30_1[6] =
	{
		48,16,
		0,0,
		0,16
	};
		

	int rightSlope_30_2[6] =
	{
		0,32,
		48,32,
		48,16
	};

	int leftSlope_30_2[6] =
	{
		48,32,
		0,16,
		0,32
	};

	int rightSlope_30_3[6] =
	{
		0,16,
		48,16,
		48,0
	};

	int leftSlope_30_3[6] =
	{
		0,48,
		0,32,
		48,48
	};

};

#endif // __SCENE_H__