#include "Pacman.h"

Pacman::Pacman(int row, int col)
{
	setRow(row);
	setCol(col);
}

void Pacman::setSpeed(int value)
{
	mSpeed = value;
}

int Pacman::getSpeed()
{
	return mSpeed;
}

void Pacman::setDir(int value)
{
	mDir = value;
}

int Pacman::getDir()
{
	return mDir;
}

void Pacman::draw(HDC & dest, HDC & src, HBITMAP & image)
{
	//1.先把图片放到兼容DC中
	SelectObject(src, image);
	//2.开始贴图
	BitBlt(dest, getX(), getY(), Constants::Grid_Width, Constants::Grid_Height, src, 320 + mAniIndex*32, mDir * 32, SRCCOPY);
}

void Pacman::update()
{
	++mCount;
	if (mCount >=5)
	{
		mAniIndex = mAniIndex ? 0 : 1;
		mCount = 0;
	}

	move();
}

void Pacman::move()
{
	int&& dir = getDir();
	switch (dir)
	{
	case 0:
		setX(getX() + mSpeed);
		break;
	case 1:
		setY(getY() + mSpeed);
		break;
	case 2:
		setX(getX() - mSpeed);
		break;
	case 3:
		setY(getY() - mSpeed);
		break;
	}
}


