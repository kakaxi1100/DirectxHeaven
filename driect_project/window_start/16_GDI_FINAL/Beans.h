#pragma once
#include <vector>
#include <memory>
#include "Map.h"
#include "Bean.h"
#include "IDrawable.h"

class Beans : public IDrawable
{
public:
	void init(const Map& m);
	virtual void draw(HDC& dest, HDC& src, HBITMAP& image) override;
	std::vector<std::shared_ptr<Bean>> beanList;
};
