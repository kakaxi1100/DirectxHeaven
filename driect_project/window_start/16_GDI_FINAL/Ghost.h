#pragma once
#include "GameObject.h"
#include "IDrawable.h"
#include "IMoveable.h"

class Ghost : public GameObject, public IDrawable, public IMoveable
{
public:

private:
	int state = 0;
	int type = 0;
};
