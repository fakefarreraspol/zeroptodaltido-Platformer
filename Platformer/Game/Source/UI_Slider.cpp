#include "UI_Slider.h"
#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "Scene.h"
#include "List.h"

UI_Slider::UI_Slider(int id, int maxValue)
{
	displacement = 0;
	this->maxValue = maxValue;
	value = maxValue;
	holdingSlider = false;
	this->id = id;
}
UI_Slider::~UI_Slider()
{

}

void UI_Slider::OnMouseOver()
{
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		clicked_inside = true;
	}


	if (clicked_inside)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			OnMouseHold();
		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			clicked_inside = false;
			holdingSlider = false;
		}

	}
	
}

void UI_Slider::OnMouseHold()
{

	int x_, y_;
	app->input->GetMousePosition(x_, y_);

	displacement = x_ - (x + rec_curr_fill.w);

	if (abs(displacement) > 2)
		value = (((float)rec_curr_fill.w + displacement) / (float)rec_body_fill.w) * maxValue;

	holdingSlider = true;
	if (abs(displacement) < 40)
	app->render->DrawTexture(sprite, slider_x - rec_slider.w / 2 + 20, y - rec_slider.h / 4, &rec_slider_hold, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);
}


bool UI_Slider::Start()
{
	return true;
}

bool UI_Slider::PreUpdate()
{

	if (active)
	{
		switch (set)
		{
		case SET_NOTHING:
			break;
		case SET_GLOBAL_VOLUME:
		{

			app->audio->audio_multiplier = (float)value / 100;

		}
		break;
		case SET_MUSIC_VOLUME:
		{

			int volume = (int)((float)value * app->audio->audio_multiplier);
			if (app->scene->titleMusic != 1000)
				Mix_VolumeChunk(app->audio->fx[app->scene->titleMusic - 1], volume);

			if (app->scene->jungleMusic != 1000)
				Mix_VolumeChunk(app->audio->fx[app->scene->jungleMusic - 1], volume);

		}
		break;
		case SET_SFX_VOLUME:
		{
			ListItem<Mix_Chunk*>* curCunk = app->audio->fx.start;
			for (size_t i = 0; i < app->audio->fx.count(); i++)
			{
				if (app->audio->fx[i] != nullptr)
				{
					Mix_Chunk* chunk = app->audio->fx[i];

					int volume = (int)((float)value * app->audio->audio_multiplier);
					Mix_VolumeChunk(chunk, volume);
				}

				curCunk = curCunk->next;
			}

			int volume = (int)((float)app->scene->UI_slider_music_vol->value * app->audio->audio_multiplier);
			if (app->scene->titleMusic != 1000)
				Mix_VolumeChunk(app->audio->fx[app->scene->titleMusic - 1], volume);

			if (app->scene->jungleMusic != 1000)
				Mix_VolumeChunk(app->audio->fx[app->scene->jungleMusic - 1], volume);

		}
		break;
		default:
			break;
		}
	}

	

	return true;

}

bool UI_Slider::Update(float dt)
{
	if (value <= 0)
		value = 0;

	if (value >= maxValue)
		value = maxValue;

	

	return true;
}

bool UI_Slider::PostUpdate()
{

	if (active)
	{
		DrawBody();
		int mouse_x, mouse_y;
		app->input->GetMousePosition(mouse_x, mouse_y);

		slider_x = x - rec_slider.w / 2 + (int)((float)value / maxValue * (float)rec_body_fill.w) + 40 + displacement;
		slider_y = y - rec_slider.h / 4 + 8;
		slider_w = rec_slider.w - 40 * 2;
		slider_h = rec_slider.h - 8 - 12;



		if (mouse_x > x && mouse_x < x + w &&
			mouse_y > y && mouse_y < y + h)
		{
			OnMouseOver();
			if (!holdingSlider)
			{
				app->render->DrawTexture(sprite, slider_x - rec_slider.w / 2 + 20, y - rec_slider.h / 4, &rec_slider, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);
			}
		}
		else
		{
			app->render->DrawTexture(sprite, slider_x - rec_slider.w/2 + 20, y - rec_slider.h/4, &rec_slider, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);
			holdingSlider = false;
		}
		
	}
	//LOG("id: %i", id);
	//LOG("value: %i", value);

	return true;
}

bool UI_Slider::CleanUp()
{
	return true;

}

void UI_Slider::SetSlider(SDL_Texture* sprite, SDL_Rect defaultRect , SDL_Rect holdRect, int x, int y, int w, int h)
{
	SetSprite(sprite);

	rec_slider = defaultRect;
	rec_slider_hold = holdRect;

	slider_x = x;
	slider_y = y;
	slider_w = w;
	slider_h = h;
}


iPoint UI_Slider::GetPosition() const
{
	iPoint temp = { x,y };
	return temp;
}

iPoint UI_Slider::GetPositionCenter() const
{
	iPoint res(x + w / 2, y + h / 2);

	return res;
}

void UI_Slider::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void UI_Slider::SetPositionCenter(int x, int y)
{
	this->x = x - w / 2;
	this->y = y - h / 2;
}

void UI_Slider::DrawBody()
{
	

	float slider_fill_f = (float)value / maxValue * (float)rec_body_fill.w;
	int slider_fill_i = (int)slider_fill_f;
	
	rec_curr_fill = rec_body_fill;
	rec_curr_fill.w = slider_fill_i;
	app->render->DrawTexture(sprite, x, y, &rec_sprite, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);
	app->render->DrawTexture(sprite, x + 4, y + 4, &rec_curr_fill, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);

}

void UI_Slider::DrawSlider()
{

}

void UI_Slider::SetActive(bool isActive)
{
	active = isActive;
}

bool UI_Slider::IsActive() const
{
	return active;
}

void UI_Slider::SetSprite(SDL_Texture* newSprite)
{
	sprite = newSprite;
}
