#ifndef __UI_PANEL__
#define __UI_PANEL__

#include "UIElements.h"



class UI_Panel : public UI_Element
{
public:
	UI_Panel(int id);
	~UI_Panel();
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

	float angle;

private:

};



#endif // !__UI_PANEL__
