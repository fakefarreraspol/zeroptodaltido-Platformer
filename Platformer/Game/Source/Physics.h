#pragma once
#include "Module.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -24.0f

#define PIXELS_PER_METER 48.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.020833333333333f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
enum BodyType
{
	TYPE_NULL = 0,
	TYPE_ENEMY,
	TYPE_BULLET,
	TYPE_PLAYER,
	TYPE_SOLID_TILE

};
class PhysBody
{
public:
	PhysBody() : body(NULL), type(TYPE_NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	bool playAnimation = false;
	int width, height;
	b2Body* body;
	BodyType type;
	// TODO 6: add a pointer to a module that might want to listen to a collision from this body
};

// Module --------------------------------------
// TODO 3: Make module physics inherit from b2ContactListener
// then override void BeginContact(b2Contact* contact)
class Physics : public Module
{
public:
	Physics();
	~Physics();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateStaticCircle(int x, int y, int radius);
	PhysBody* CreateSensorCircle(int x, int y, int radius);

	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateKinematicRectangle(int x, int y, int width, int height);
	//PhysBody* CreateKinematicRectangle(int x, int y, int width, int height);

	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreateSensorChain(int x, int y, int* points, int size);
	PhysBody* CreateStaticChain(int x, int y, int* points, int size);
	PhysBody* CreateKinematicChain(int x, int y, int* points, int size);

	b2RevoluteJoint* CreateFlipperJoint(b2Body* round, b2Vec2 groundAnchor, b2Body* flipper, b2Vec2 flipperAnchor);
	

	b2World* GetWorld() const
	{
		return world;
	}

	bool GetDebug() const
	{
		return debug;
	}

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouseJoint;
	b2Body* ground;
	b2Vec2 clickedBodyPos;
	b2Body* mouseBody;
};