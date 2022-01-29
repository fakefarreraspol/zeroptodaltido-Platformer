#ifndef __UI_SLIDER__
#define __UI_SLIDER__

#include "UIElements.h"


enum setType
{
	SET_NOTHING = 0,
	SET_GLOBAL_VOLUME,
	SET_MUSIC_VOLUME,
	SET_SFX_VOLUME
};

class UI_Slider : public UI_Element
{
public:
	UI_Slider(int id, int maxValue);
	~UI_Slider();

	void OnMouseOver();

	void OnMouseHold();


	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	iPoint GetPosition() const;

	iPoint GetPositionCenter() const;

	void SetPosition(int x, int y);

	void SetPositionCenter(int x, int y);

	void DrawBody();
	void DrawSlider();

	void SetActive(bool isActive);

	bool IsActive() const;

	void SetSprite(SDL_Texture* newSprite);


	void SetSlider(SDL_Texture* defaultSprite, SDL_Rect newRect, SDL_Rect holdRect, int x, int y, int w, int h);

	void SetMaxValue(int value)
	{
		maxValue = value;
	}

	int GetValue() const
	{
		return value;
	}


	bool clicked_inside;


	SDL_Rect rec_body_fill;
	SDL_Rect rec_slider;
	SDL_Rect rec_slider_hold;
	setType set;

private:

	

	int value;
	int maxValue;

	SDL_Rect rec_curr_fill;

	int displacement;

	int slider_x;
	int slider_y;
	int slider_h;
	int slider_w;

	bool holdingSlider;
	

};



#endif // !__UI_BUTTON__
