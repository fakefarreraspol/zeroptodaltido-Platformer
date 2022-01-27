#include "UIHandler.h"


UI_Handler::UI_Handler() : Module() , total_ids(0), spritesheet(nullptr)
{
	name.Create("UI_Handler");
}

UI_Handler::~UI_Handler()
{

}

bool UI_Handler::Start()
{
	total_ids = 0;
	spritesheet = app->tex->Load("Assets/textures/UI/UI_elements.png");

	return true;
}

bool UI_Handler::CleanUp()
{


	return true;
}

bool UI_Handler::PreUpdate()
{

	for (int i = 0; i < allUI_Elements.count(); i++)
	{

		UI_Element* element;
		allUI_Elements.at(i, element);

		element->PreUpdate();
	}
	return true;
}

bool UI_Handler::Update(float dt)
{
	//LOG("added ui handler");

	for (int i = 0; i < allUI_Elements.count(); i++)
	{

		UI_Element* element;
		allUI_Elements.at(i, element);

		element->Update(dt);
	}

	return true;
}

bool UI_Handler::PostUpdate()
{
	for (int i = 0; i < allUI_Elements.count(); i++)
	{

		UI_Element* element;
		allUI_Elements.at(i, element);

		element->PostUpdate();
	}
	return true;
}

UI_Button* UI_Handler::CreateButton(SDL_Texture* sprite, int x, int y, int width, int height)
{
	UI_Button* button = new UI_Button(total_ids);
	allUI_Elements.add(button);
	total_ids++;


	button->SetSprite(sprite);
	button->SetPosition(x, y);
	button->SetWidth(width);
	button->SetHeight(height);
	LOG("created button");
	return button;

}

UI_Panel* UI_Handler::CreatePanel(SDL_Texture* sprite, int x, int y, int width, int height)
{
	LOG("created panel");
	UI_Panel* panel = nullptr;
	panel = new UI_Panel(total_ids);
	LOG("panel %i", panel);
	allUI_Elements.add(panel);
	total_ids++;


	panel->SetSprite(sprite);
	panel->SetPosition(x, y);
	panel->SetWidth(width);
	panel->SetHeight(height);
	

	return panel;
}

 

void UI_Handler::DeleteElement(UI_Element* element)
{
	bool cont = true;
	p2List_item<UI_Element*>* elem = allUI_Elements.getFirst();
	for (int i = 0; i < allUI_Elements.count(); i++)
	{
		LOG("n: %i", allUI_Elements.count());
		if (!cont) break;
		UI_Element* iterator;
		allUI_Elements.at(i, iterator);



		if (iterator->Id() == element->Id())
		{
			allUI_Elements.del(elem);
			p2List_item<UI_Element*>* eToDelete = allUI_Elements.findNode(iterator);
			allUI_Elements.del(eToDelete);

			delete element;
			cont = true;
		}
		else
			elem = elem->next;
	}
}