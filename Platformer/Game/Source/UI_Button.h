#ifndef __UI_BUTTON__
#define __UI_BUTTON__

#include "UIElements.h"



class UI_Button : public UI_Element
{
public:
	UI_Button(int id);
	~UI_Button();

	void OnMouseOver();
	void OnMouseNotOver();

	void OnMouseHold();
	void OnMouseRelease();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:

};



#endif // !__UI_BUTTON__
