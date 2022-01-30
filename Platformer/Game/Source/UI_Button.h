#ifndef __UI_BUTTON__
#define __UI_BUTTON__

#include "UIElements.h"


enum ButtonAction
{
	ACTION_NOTHING = 0,
	ACTION_START_GAME,
	ACTION_PAUSE_OPEN,
	ACTION_PAUSE_CLOSE,
	ACTION_SETTINGS_OPEN,
	ACTION_SETTINGS_CLOSE,
	ACTION_TOGGLE_VSYNC,
	ACTION_TOGGLE_FULLSCREEN,
	ACTION_SAVE_GAME,
	ACTION_LOAD_GAME,
	ACTION_EXIT_GAME,
	ACTION_EXIT_GAME_CONFIRM,
	ACTION_EXIT_GAME_DENY
};



class UI_Button : public UI_Element
{
public:
	UI_Button(int id);
	~UI_Button();

	void OnMouseOver();


	void OnMouseHold();
	void OnMouseRelease();

	
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	iPoint GetPosition() const;

	iPoint GetPositionCenter() const;

	void SetPosition(int x, int y);

	void SetPositionCenter(int x, int y);


	void Draw();

	void SetActive(bool isActive);

	bool IsActive() const;

	int Id() const;

	void SetSprite(SDL_Texture* newSprite);

	void SetRectR(SDL_Rect newRect);

	void SetRect(int x, int y, int w, int h);

	bool clicked_inside;
	ButtonAction action;

	SDL_Rect rec_over;
	SDL_Rect rec_hold;

private:

	bool ret;

};



#endif // !__UI_BUTTON__
