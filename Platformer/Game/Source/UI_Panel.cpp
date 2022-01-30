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
	if (active)
		Draw();
	return true;
}


iPoint UI_Panel::GetPosition() const
{
	iPoint res(x, y);

	return res;
}

iPoint UI_Panel::GetPositionCenter() const
{
	iPoint res(x + w / 2, y + h / 2);

	return res;
}

void UI_Panel::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;

}

void UI_Panel::SetPositionCenter(int x, int y)
{
	this->x = x - w / 2;
	this->y = y - h / 2;

}


void UI_Panel::Draw()
{
	app->render->DrawTexture(sprite, x, y, &rec_sprite, SDL_FLIP_NONE, 0, angle);
}

void UI_Panel::SetActive(bool isActive)
{
	active = isActive;
}

bool UI_Panel::IsActive() const
{
	return active;
}

int UI_Panel::Id() const
{
	return id;
}

void UI_Panel::SetSprite(SDL_Texture* newSprite)
{
	sprite = newSprite;
}

void UI_Panel::SetRectR(SDL_Rect newRect)
{
	rec_sprite = newRect;
}

void UI_Panel::SetRect(int x, int y, int w, int h)
{
	SDL_Rect newRect = { x,y,w,h };
	rec_sprite = newRect;
}