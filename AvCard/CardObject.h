#pragma once

#include "typedef.h";

class CardObject
{
private:
	bool active;
public:
	void setActive(bool active){
		this->active=active;
	}
	CardObject(void)
	{
		active=false;
	}

	~CardObject(void)
	{
	}
};

