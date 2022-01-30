#ifndef __UI_HANDLER__
#define __UI_HANDLER__

#include "App.h"
#include "Module.h"
#include "UIElements.h"
#include "Textures.h"
#include "p2List.h"
#include "Textures.h"

#include "UI_Button.h"
#include "UI_Panel.h"
#include "UI_Slider.h"



class UI_Handler : public Module
{
public:
	UI_Handler();
	~UI_Handler();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void DeleteElement(UI_Element* element);

	UI_Button* CreateButton(SDL_Texture* sprite, int x, int y, int width, int height);
	UI_Panel* CreatePanel(SDL_Texture* sprite, int x, int y, int width, int height, float angle = 0);
	UI_Slider* CreateSlider(int x, int y, int maxValue, SDL_Texture* sprite, SDL_Rect slider_bg, SDL_Rect slider_fill, SDL_Rect slider_button, SDL_Rect slider_button_hold);
	
	SDL_Texture* spritesheet;
private:


	p2List<UI_Element*> allUI_Elements;
	int total_ids;
	bool ret;

};

#endif // !__UI_HANDLER__
