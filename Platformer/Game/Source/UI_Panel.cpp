#include "UI_Panel.h"

UI_Panel::UI_Panel(int id) : UI_Element(id)
{
	name.Create("UI_Panel");
}

UI_Panel::~UI_Panel()
{

}

bool UI_Panel::Start()
{
	return true;

}

bool UI_Panel::CleanUp()
{
	return true;

}

bool UI_Panel::PreUpdate()
{
	return true;

}

bool UI_Panel::Update(float dt)
{
	
	return true;

}

bool UI_Panel::PostUpdate()
{
	Draw();
	return true;
}