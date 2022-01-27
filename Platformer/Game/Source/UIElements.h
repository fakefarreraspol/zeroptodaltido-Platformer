#ifndef __UI_ELEMENT__
#define	__UI_ELEMENT__

#include "p2Point.h"
#include "Module.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"



class UI_Element : public Module
{
public:
	UI_Element();
	UI_Element(int id);
	~UI_Element();

	iPoint GetPosition() const;
	
	iPoint GetPositionCenter() const;

	void SetPosition(int x, int y);

	void SetPositionCenter(int x, int y);

	int GetWidth() const;

	void SetWidth(int width);

	int GetHeight() const;

	void SetHeight(int height);

	virtual void Draw();

	void SetActive(bool isActive);

	bool IsActive() const;

	int Id() const;

	void SetSprite(SDL_Texture* newSprite);

	void SetRectR(SDL_Rect newRect);

	void SetRect(int x, int y, int w, int h);

	void Delete();

	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();

protected:

	int x, y, w, h;
	SDL_Texture* sprite;
	SDL_Rect rec_sprite;
	bool active;
	int id;

};






#endif // !__UI_ELEMENT__
