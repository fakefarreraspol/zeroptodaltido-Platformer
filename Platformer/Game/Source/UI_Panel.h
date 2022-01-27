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

private:

};



#endif // !__UI_PANEL__
