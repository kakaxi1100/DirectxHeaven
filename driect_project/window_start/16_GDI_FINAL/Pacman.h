#pragma once
#include "GameObject.h"
#include "IMoveable.h"
#include "IDrawable.h"

class Pacman : public GameObject , public IMoveable, public IDrawable
{
public:
	Pacman(int row, int col);
	virtual void setSpeed(int value);
	virtual int getSpeed();
	virtual void setDir(int value);
	virtual int getDir();
	virtual void draw(HDC& dest, HDC& src, HBITMAP& image);
	void update();
private:
	int mSpeed = Constants::Player_Speed;
	int mDir = 0;
	int mAniIndex = 0;//每副动画有两帧
	int mCount = 0;
	void move();
};