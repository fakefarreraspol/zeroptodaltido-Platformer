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


UI_Element::~UI_Element()
{

}

iPoint UI_Element::GetPosition() const
{
	iPoint res(x, y);

	return res;
}

iPoint UI_Element::GetPositionCenter() const
{
	iPoint res(x + w / 2, y + h / 2);

	return res;
}

void UI_Element::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;

}

void UI_Element::SetPositionCenter(int x, int y)
{
	this->x = x - w / 2;
	this->y = y - h / 2;

}

int UI_Element::GetWidth() const
{
	return w;
}

void UI_Element::SetWidth(int width)
{
	w = width;
}

int UI_Element::GetHeight() const
{
	return h;
}

void UI_Element::SetHeight(int height)
{
	h = height;
}

void UI_Element::Draw()
{
	app->render->DrawTexture(sprite, x, y, &rec_sprite, SDL_FLIP_NONE, 0, 0,x + w/2, y + h/2);
}

void UI_Element::SetActive(bool isActive)
{
	active = isActive;
}

bool UI_Element::IsActive() const
{
	return active;
}

int UI_Element::Id() const
{
	return id;
}

void UI_Element::SetSprite(SDL_Texture* newSprite)
{
	sprite = newSprite;
}

void UI_Element::SetRectR(SDL_Rect newRect)
{
	rec_sprite = newRect;
}

void UI_Element::SetRect(int x, int y, int w, int h)
{
	SDL_Rect newRect = { x,y,w,h };
	rec_sprite = newRect;
}

void UI_Element::Delete()
{

}