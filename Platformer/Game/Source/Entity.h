#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"

class Entity : public Module
{
public:
	Entity() 
	{}

	~Entity()
	{}

	void SetID(int id)
	{
		this->id = id;
	}

	int Id() const
	{
		return id;
	}

private:

protected:

	int id;

};






#endif // !__ENTITY_H__
