#pragma once
#include <cstdlib>

#include "GameObject.h"
#include "IDrawable.h"
#include "IMoveable.h"

class Ghost : public GameObject, public IDrawable, public IMoveable
{
public:
	Ghost(const int r, const int c, const int type);
	virtual ~Ghost() = default;

	virtual void draw(HDC& dest, HDC& src, HBITMAP& image) override;
	virtual void setSpeed(int value) override;
	virtual int getSpeed() override;
	virtual void setDir(int value) override;
	virtual int getDir() override;
	void update();
	int getState() const;
	void setState(int value);
private:
	int mSpeed = Constants::Ghost_Speed;
	int mDir = 0;
	int mAniIndex = 0;//每副动画有两帧
	int mCount = 0;
	int mState = 0;// 0 是追逐模式， 1是绕路模式
	int mType = 0;
};
