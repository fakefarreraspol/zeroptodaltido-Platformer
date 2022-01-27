//#include "Globals.h"
#include "App.h"
#include "Input.h"
#include "Input.h"
#include "Window.h"
#include "Physics.h"
#include "p2Point.h"
#include "math.h"
#include "Render.h"
#include "Log.h"

#ifdef _DEBUG
#pragma comment( lib, "../Game/Source/External/Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "../Game/Source/External/Box2D/libx86/Release/Box2D.lib" )
#endif

Physics::Physics() : Module()
{
	name.Create("physics");
	world = NULL;
	debug = false;
}

// Destructor
Physics::~Physics()
{
}

bool Physics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	/*b2BodyDef groundBD;
	groundBD.type = b2_staticBody;
	groundBD.position.Set(100, 100);
	world->CreateBody(&groundBD);*/
	// TODO 3: You need to make Physics class a contact listener

	// big static circle as "ground" in the middle of the screen
	/*int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);*/

	return true;
}

// 
bool Physics::PreUpdate()
{
	if (!app->GameIsPaused())
	{
		world->Step(app->DeltaTime(), 6, 2);
	}
	
	//world->Step(1 / 60.0f, 6, 2);

	// TODO: HomeWork
	/*
	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
	}
	*/

	return true;
}

b2RevoluteJoint* Physics::CreateFlipperJoint(b2Body* ground, b2Vec2 groundAnchor, b2Body* flipper, b2Vec2 flipperAnchor)
{
	b2RevoluteJointDef FlipperJoint;
	FlipperJoint.bodyA = ground;
	FlipperJoint.bodyB = flipper;
	FlipperJoint.collideConnected = false;
	FlipperJoint.enableLimit = false;
	FlipperJoint.enableMotor = true;
	FlipperJoint.localAnchorA.Set(PIXEL_TO_METERS(groundAnchor.x), PIXEL_TO_METERS(groundAnchor.y));
	FlipperJoint.localAnchorB.Set(PIXEL_TO_METERS(flipperAnchor.x), PIXEL_TO_METERS(flipperAnchor.y));

	b2RevoluteJoint* Flippjoint = (b2RevoluteJoint*)world->CreateJoint(&FlipperJoint);
	return Flippjoint;
}


PhysBody* Physics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.allowSleep = false;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0;
	//fixture.restitution = 0.25f;

	b->CreateFixture(&fixture);

	// TODO 4: add a pointer to PhysBody as UserData to the body
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* Physics::CreateSensorCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 5.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	// TODO 4: add a pointer to PhysBody as UserData to the body
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* Physics::CreateStaticCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 5.0f;

	b->CreateFixture(&fixture);

	// TODO 4: add a pointer to PhysBody as UserData to the body
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = radius;

	return pbody;
}


PhysBody* Physics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* Physics::CreateKinematicRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x) * app->win->GetScale(), PIXEL_TO_METERS(y) * app->win->GetScale());

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f * app->win->GetScale(), PIXEL_TO_METERS(height) * 0.5f * app->win->GetScale());

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = width * 0.5f * app->win->GetScale();
	pbody->height = height * 0.5f * app->win->GetScale();

	return pbody;
}

PhysBody* Physics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* Physics::CreateStaticChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.friction = 0.2f;
	

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* Physics::CreateSensorChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* Physics::CreateKinematicChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x) * app->win->GetScale(), PIXEL_TO_METERS(y) * app->win->GetScale());

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;

	return pbody;
}



// 
bool Physics::PostUpdate()
{
	if(app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return true;

	b2Body* clickedBody = NULL;
	b2Vec2 mousePos = { PIXEL_TO_METERS(app->input->GetMouseX()), PIXEL_TO_METERS(app->input->GetMouseY()) };
	

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					app->render->DrawCircle(METERS_TO_PIXELS(pos.x + app->render->camera.x) , METERS_TO_PIXELS(pos.y + app->render->camera.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				b2Vec2 p = { PIXEL_TO_METERS(app->input->GetMouseX()), PIXEL_TO_METERS(app->input->GetMouseY()) };
				if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
				{
					mouseBody = b;

					b2Vec2 mousePosition;
					mousePosition.x = p.x;
					mousePosition.y = p.y;

					b2BodyDef groundBD;
					groundBD.type = b2_staticBody;
					groundBD.position.Set(100, 100);
					ground = world->CreateBody(&groundBD);

					b2MouseJointDef def;
					def.bodyA = ground; // First body must be a static ground
					def.bodyB = mouseBody; // Second body will be the body to attach to the mouse
					def.target = mousePosition; // The second body will be pulled towards this location
					def.dampingRatio = 0.5f; // Play with this value
					def.frequencyHz = 2.0f; // Play with this value
					def.maxForce = 200.0f * mouseBody->GetMass(); // Play with this value
					
					// add the new mouse joint into the World
					mouseJoint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}
		}
		
	}
	//if (mouseBody != nullptr && mouseJoint != nullptr)
	//{
	//	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	//	{
	//		// Get new mouse position and re-target mouse_joint there
	//		b2Vec2 mousePosition;
	//		mousePosition.x = PIXEL_TO_METERS(app->input->GetMouseX());
	//		mousePosition.y = PIXEL_TO_METERS(app->input->GetMouseY());
	//		mouseJoint->SetTarget(mousePosition);
	//
	//		// Draw a red line between both anchor points
	//		app->render->DrawLine(METERS_TO_PIXELS(mouseBody->GetPosition().x), METERS_TO_PIXELS(mouseBody->GetPosition().y), app->input->GetMouseX(), app->input->GetMouseY(), 255, 0, 0);
	//	}
	//}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (mouseBody != nullptr && mouseJoint != nullptr)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			// Tell Box2D to destroy the mouse_joint
			world->DestroyJoint(mouseJoint);

			// DO NOT FORGET THIS! We need it for the "if (mouse_body != nullptr && mouse_joint != nullptr)"
			mouseJoint = nullptr;
			mouseBody = nullptr;
		}
	}
	// Draw a red line between both anchor points
		
	/*if (clickedBody != NULL)
	{
		b2MouseJointDef mJoint;
		ground = Physics::CreateStaticCircle(400, 800, 10);
		mJoint.bodyA = ground->body;
		
		mJoint.bodyB = clickedBody;
		mJoint.target = mousePos;
		mJoint.dampingRatio = 0.5f;
		mJoint.frequencyHz = 2.0f;
		mJoint.maxForce = 100.0f * clickedBody->GetMass();
		mouseJoint = (b2MouseJoint*)world->CreateJoint(&mJoint);

		clickedBodyPos.x = clickedBody->GetPosition().x;
		clickedBodyPos.y = clickedBody->GetPosition().y;




	}*/
	// TODO 3: If the player keeps pressing the mouse button, update
// target position and draw a red line between both anchor points
	//if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	//{
	//	mouseJoint->SetTarget(mousePos);
	//	if (clickedBody != NULL)
	//	{
	//		app->render->DrawLine(mousePos.x, mousePos.y, clickedBodyPos.x, clickedBodyPos.y, 255, 0, 0, 255);
	//	}




	//}
	//// TODO 4: If the player releases the mouse button, destroy the joint
	//if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	//{
	//	//delete mouseJoint
	//	world->DestroyJoint(mouseJoint);
	//	mouseJoint = NULL;
	//	clickedBody = NULL;
	//}
	//if (app->input->GetKey(SDL_SCANCODE_2) == KEY_UP)
	//{
	//	//delete mouseJoint;
	//	world->DestroyJoint(mouseJoint);
	//	mouseJoint = NULL;
	//	clickedBody = NULL;
	//}
	
	return true;
}


// Called before quitting
bool Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	if (body != nullptr)
	{
		b2Vec2 pos = body->GetPosition();
		x = METERS_TO_PIXELS(pos.x) - (width);
		y = METERS_TO_PIXELS(pos.y) - (height);
	}
	else
	{
		LOG("body does not exist!");
	}
	
}

float PhysBody::GetRotation() const
{
	return app->RADTODEG() * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	// TODO 1: Write the code to return true in case the point
	// is inside ANY of the shapes contained by this body
	b2Vec2  point;
	point.x = x;
	point.y = y;
	if (body->GetFixtureList()->GetShape()->TestPoint(body->GetTransform(), point)) return true;
	
	else return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	// TODO 2: Write code to test a ray cast between both points provided. If not hit return -1
	// if hit, fill normal_x and normal_y and return the distance between x1,y1 and it's colliding point
	/*b2RayCastOutput output;
	b2RayCastInput input;
    if(body->GetFixtureList()->RayCast(&output,input, ))*/
	int ret = -1;

	return ret;
}

// TODO 3

// TODO 7: Call the listeners that are not NULL