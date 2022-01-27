#include "UI_Button.h"

UI_Button::UI_Button(int id) : UI_Element(id)
{
	name.Create("UI_Button");
}

UI_Button::~UI_Button()
{

}

bool UI_Button::Start()
{
	return true;
}

bool UI_Button::CleanUp()
{
	return true;

}

bool UI_Button::PreUpdate()
{
	return true;

}

bool UI_Button::Update(float dt)
{
	Draw();
	return true;

}

bool UI_Button::PostUpdate()
{
	return true;

}

void UI_Button::OnMouseOver()
{

}

void UI_Button::OnMouseNotOver()
{

}

void UI_Button::OnMouseHold()
{

}

void UI_Button::OnMouseRelease()
{

}

