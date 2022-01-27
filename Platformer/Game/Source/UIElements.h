#ifndef __UI_ELEMENT__
#define	__UI_ELEMENT__

#include "p2Point.h"
#include "Module.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"

class UI_Button;
class UI_Panel;

class UI_Element : public Module
{
public:
	UI_Element();
	UI_Element(int id);
	~UI_Element();


	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual int Id() const
	{
		return id;
	}

	int x, y, w, h;
	SDL_Texture* sprite;
	SDL_Rect rec_sprite;
	bool active;
protected:

	
	
	int id;

};






#endif // !__UI_ELEMENT__
