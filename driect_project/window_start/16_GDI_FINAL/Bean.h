#pragma once
#include "GameObject.h"
#include "IDrawable.h"

class Bean : public GameObject, public IDrawable
{
public:

	Bean(const int r, const int c) 
	{
		setCol(c);
		setRow(r);
	}

	virtual ~Bean() = default;

	virtual void draw(HDC& dest, HDC& src, HBITMAP& image) override
	{
		//1.先把图片放到兼容DC中
		SelectObject(src, image);
		//2.开始贴图
		BitBlt(dest, getCol() * Constants::Grid_Width, getRow() * Constants::Grid_Height, Constants::Grid_Width, Constants::Grid_Height,
			src, 64, 0, SRCCOPY);
	}
	
	const int getCenterX() 
	{
		return static_cast<const int>(getX() + Constants::Grid_Width * 0.5);
	}

	const int getCenterY()
	{
		return static_cast<const int>(getY() + Constants::Grid_Height * 0.5);
	}
};
