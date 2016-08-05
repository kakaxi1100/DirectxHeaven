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
	//1.�Ȱ�ͼƬ�ŵ�����DC��
	SelectObject(src, image);
	//2.��ʼ��ͼ
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

	if(mState == 0)
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
	else if (mState == 1)
	{

	}
}
