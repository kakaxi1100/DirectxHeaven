#pragma once
#include "Constants.h"

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;
	
	inline void setX(int value) { mX = value; }
	inline int getX() { return mX; }
	inline void setY(int value) { mY = value; }
	inline int getY() { return mY; }
	inline void setCol(int value) { mX = value * Constants::Grid_Width; }
	inline int getCol() { return static_cast<int>( mX / Constants::Grid_Width ); }
	inline void setRow(int value) { mY = value * Constants::Grid_Height; }
	inline int getRow() { return static_cast<int>( mY / Constants::Grid_Height); }
private:
	int mX;
	int mY;
};