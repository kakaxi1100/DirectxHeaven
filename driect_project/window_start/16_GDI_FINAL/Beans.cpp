#include "Beans.h"

void Beans::init(const Map& m)
{
	for (auto g : m.map)
	{
		if (g->getType() == 0)
		{
			beanList.push_back(std::make_shared<Bean>(g->getRow(), g->getCol()));
		}
	}
}

void Beans::draw(HDC & dest, HDC & src, HBITMAP & image)
{
	for (const auto& bean : beanList)
	{
		bean->draw(dest, src, image);
	}
}
