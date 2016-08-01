#pragma once
#include <Windows.h>

class IDrawable
{
public:
	IDrawable() = default;
	virtual ~IDrawable() = default;
	virtual void draw(HDC& dest, HDC& src, HBITMAP& image) = 0;
};