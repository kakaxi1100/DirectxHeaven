#pragma once

class IMoveable
{
public:
	IMoveable() = default;
	virtual ~IMoveable() = default;
	virtual void setSpeed(int value) = 0;
	virtual int getSpeed() = 0;
	virtual void setDir(int value) = 0;
	virtual int getDir() = 0;
};