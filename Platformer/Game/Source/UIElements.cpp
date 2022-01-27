#include "UIElements.h"


UI_Element::UI_Element() : Module()
{
	name.Create("UI_Element");
	x = y = w = h = 0;
	active = true;
	id = 0;
	sprite = nullptr;
	rec_sprite = { 0,0,0,0 };
}



UI_Element::UI_Element(int id) : Module()
{
	name.Create("UI_Element");
	x = y = w = h = 0;
	active = true;
	this->id = id;
	sprite = nullptr;
	rec_sprite = { 0,0,0,0 };
}

UI_Element::~UI_Element()
{

}





bool UI_Element::PreUpdate()
{
	return true;

}

bool UI_Element::Update(float dt)
{

	return true;

}

bool UI_Element::PostUpdate()
{
	return true;
}





