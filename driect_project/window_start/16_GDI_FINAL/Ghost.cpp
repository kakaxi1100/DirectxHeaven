#include "Ghost.h"

Ghost::Ghost(const int r, const int c, const int type) : mType(type)
{
	setCol(c);
	setRow(r);
}

void Ghost::setSpeed(int value)
{
	mSpeed = value;
}

int Ghost::getSpeed()
{
	return mSpeed;
}

void Ghost::setDir(int value)
{
	mDir = value;
}

int Ghost::getDir()
{
	return mDir;
}

void Ghost::draw(HDC & dest, HDC & src, HBITMAP & image)
{
	//1.先把图片放到兼容DC中
	SelectObject(src, image);
	//2.开始贴图
	BitBlt(dest, getX(), getY(), Constants::Grid_Width, Constants::Grid_Height, 
		src, mType * 64 + mAniIndex * 32, mDir * 32, SRCCOPY);
}

void Ghost::update()
{
	++mCount;
	if (mCount >= 5)
	{
		mAniIndex = mAniIndex ? 0 : 1;
		mCount = 0;
	}
	if (mState == 0)
	{
		
	}
	else if (mState == 1)
	{

	}
}
