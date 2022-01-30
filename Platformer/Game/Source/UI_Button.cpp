#include "UI_Button.h"
#include "Input.h"
#include "Scene.h"
#include "Player.h"
#include "Audio.h"

UI_Button::UI_Button(int id) : UI_Element(id)
{
	name.Create("UI_Button");
}

UI_Button::~UI_Button()
{

}

bool UI_Button::Start()
{
	rec_over = { 612, 112, 56,56 };
	rec_hold = { 616, 172, 48, 48 };

	clicked_inside = false;
	action = ACTION_NOTHING;
	ret = true;
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


	return true;

}

bool UI_Button::PostUpdate()
{
	if (active)
	{
		Draw();
		int mouse_x, mouse_y;
		app->input->GetMousePosition(mouse_x, mouse_y);

		//LOG("my x: %i", x);
		//LOG("my y: %i", y);
		//LOG("my w: %i", x + w);
		//LOG("my h: %i", y + h);
		//
		//LOG("mouse x: %i", mouse_x);
		//LOG("mouse y: %i", mouse_y);


		if (mouse_x > x && mouse_x < x + w &&
			mouse_y > y && mouse_y < y + h)
		{
			OnMouseOver();
		}
		else
		{
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				clicked_inside = false;
			}
		}
	}

	return ret;

}

void UI_Button::OnMouseOver()
{
	//LOG("mouse over");
	
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
			OnMouseRelease();
			clicked_inside = false;
		}
		
	}
	else
	{
		app->render->DrawTexture(sprite, x - 4, y - 4, &rec_over, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);

	}
	
}

void UI_Button::OnMouseHold()
{
	app->render->DrawTexture(sprite, x, y, &rec_hold, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);

	//LOG("mouse hold");

}

void UI_Button::OnMouseRelease()
{
	//LOG("mouse released");

	switch (action)
	{
	case ACTION_NOTHING:
		break;
	case ACTION_START_GAME:
	{
		app->scene->lastTime = app->scene->currentTime;

		//app->audio->clearAudio();
		//app->scene->state= GAMEPLAY;
		//app->scene->Start();
		//app->player->Start();

		app->scene->UI_button_start_game->SetActive(false);
		app->scene->UI_button_quit_game->SetActive(false);
		app->scene->UI_panel_title->SetActive(false);
		app->scene->loadingScreenActive = true;

		
	}
		break;
	case ACTION_PAUSE_OPEN:
	{
		app->SetPause(true);
		active = !active;
	}
		break;
	case ACTION_PAUSE_CLOSE:
	{
		
		if (app->scene->state == INTRO)
		{
			app->scene->UI_button_settings->SetActive(true);
			app->scene->UI_panel_pause_menu->SetActive(false);
			app->scene->UI_button_settings->SetActive(true);

			app->scene->UI_button_start_game->SetActive(true);
			app->scene->UI_button_quit_game->SetActive(true);

			

		}
		else if (app->scene->state == GAMEPLAY)
		{
			app->SetPause(false);
			app->scene->UI_button_open_pause_menu->SetActive(true);
		}
	}
	break;
	case ACTION_SETTINGS_OPEN:
	{
		app->scene->menuOpen = true;
		app->scene->UI_panel_pause_menu->SetActive(true);
		app->scene->UI_button_start_game->SetActive(false);
		app->scene->UI_button_quit_game->SetActive(false);
		active = !active;
	}
	break;
	case ACTION_SETTINGS_CLOSE:
	{
		
	}
		break;
	case ACTION_TOGGLE_VSYNC:
		break;
	case ACTION_TOGGLE_FULLSCREEN:
		break;
	case ACTION_SAVE_GAME:
	{
		app->SaveGameRequest();
	}
		break;
	case ACTION_LOAD_GAME:
	{
		app->LoadGameRequest();
		
	}
		break;
	case ACTION_EXIT_GAME:
	{
		app->scene->UI_panel_pause_menu_exit_game->SetActive(true);
		app->scene->UI_button_exit_game_confirm->SetActive(true);
		app->scene->UI_button_exit_game_deny->SetActive(true);
	}
		break;
	case ACTION_EXIT_GAME_CONFIRM:
	{
		ret = false;
	}
		break;
	case ACTION_EXIT_GAME_DENY:
	{
		app->scene->UI_panel_pause_menu_exit_game->SetActive(false);
		app->scene->UI_button_exit_game_confirm->SetActive(false);
		app->scene->UI_button_exit_game_deny->SetActive(false);
	}
		break;
	default:
		break;
	}


}

iPoint UI_Button::GetPosition() const
{
	iPoint res(x, y);

	return res;
}

iPoint UI_Button::GetPositionCenter() const
{
	iPoint res(x + w / 2, y + h / 2);

	return res;
}

void UI_Button::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;

}

void UI_Button::SetPositionCenter(int x, int y)
{
	this->x = x - w / 2;
	this->y = y - h / 2;

}

void UI_Button::Draw()
{
	app->render->DrawTexture(sprite, x, y, &rec_sprite, SDL_FLIP_NONE, 0, 0, x + w / 2, y + h / 2);
}

void UI_Button::SetActive(bool isActive)
{
	active = isActive;
}

bool UI_Button::IsActive() const
{
	return active;
}

int UI_Button::Id() const
{
	return id;
}

void UI_Button::SetSprite(SDL_Texture* newSprite)
{
	sprite = newSprite;
}

void UI_Button::SetRectR(SDL_Rect newRect)
{
	rec_sprite = newRect;
}

void UI_Button::SetRect(int x, int y, int w, int h)
{
	SDL_Rect newRect = { x,y,w,h };
	rec_sprite = newRect;
}