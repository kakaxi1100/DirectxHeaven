#pragma once
#include "Map.h"
#include "Beans.h"
#include "Pacman.h"

class HitTest
{
public:
	HitTest() = default;
	~HitTest() = default;

	static int HitObject(Pacman& p, Map& m, Beans& b)
	{
		const int col = p.getCol();
		const int row = p.getRow();
		const int dir = p.getDir();
		const int left = p.getX();
		const int right = left + Constants::Grid_Width - 1;
		const int up = p.getY();
		const int down = up + Constants::Grid_Height - 1;
		const int rightCol = static_cast<int>(right / Constants::Grid_Width);
		const int upRow = static_cast<int>(up / Constants::Grid_Height);
		const int leftCol = static_cast<int>(left / Constants::Grid_Width);
		const int downRow = static_cast<int>(down / Constants::Grid_Width);

		const int rightUpType = m.map.at(upRow*m.getCols() + rightCol)->getType();
		const int rightDownType = m.map.at(downRow*m.getCols() + rightCol)->getType();
		const int leftUpType = m.map.at(upRow*m.getCols() + leftCol)->getType();
		const int leftDownType = m.map.at(downRow*m.getCols() + leftCol)->getType();

		//hit wall test
		if (dir == 0) // right
		{
			if (rightUpType == 1 || rightDownType == 1)
			{
				p.setCol(p.getCol());
				return 1;
			}
		}
		else if (dir == 1)// down
		{
			if (rightDownType == 1 || leftDownType == 1)
			{
				p.setRow(p.getRow());
				return 1;
			}
		}
		else if (dir == 2)//left
		{
			if (leftUpType == 1 || leftDownType == 1)
			{
				p.setCol(p.getCol() + 1);
				return 1;
			}
		}
		else if (dir == 3)//up
		{
			if (rightUpType == 1 || leftUpType == 1)
			{
				p.setRow(p.getRow() + 1);
				return 1;
			}
		}

		//hit test bean
		for (auto it = b.beanList.begin(); it != b.beanList.end();)
		{
			int bx =  (*it)->getCenterX();
			int by = (*it)->getCenterY();
			if (bx >= left && bx <= right && by >= up && by <= down)
			{
				it = b.beanList.erase(it);
			}
			else
			{
				++it;
			}
		}

		return 0;
	}
};
