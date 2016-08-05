#pragma once
#include "Map.h"
#include "Beans.h"
#include "Pacman.h"
#include "Ghost.h"

class HitTest
{
public:
	HitTest() = default;
	~HitTest() = default;

	static void HitObject(Pacman& p, Ghost& g, Map& m, Beans& b)
	{
		/*const int col = p.getCol();
		const int row = p.getRow();*/
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
			}
		}
		else if (dir == 1)// down
		{
			if (rightDownType == 1 || leftDownType == 1)
			{
				p.setRow(p.getRow());
			}
		}
		else if (dir == 2)//left
		{
			if (leftUpType == 1 || leftDownType == 1)
			{
				p.setCol(p.getCol() + 1);
			}
		}
		else if (dir == 3)//up
		{
			if (rightUpType == 1 || leftUpType == 1)
			{
				p.setRow(p.getRow() + 1);
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

		//hit test Ghost
		/*const int g_col = g.getCol();
		const int g_row = g.getRow();*/
		const int g_dir = g.getDir();
		const int g_left = g.getX();
		const int g_right = g_left + Constants::Grid_Width - 1;
		const int g_up = g.getY();
		const int g_down = g_up + Constants::Grid_Height - 1;
		const int g_rightCol = static_cast<int>(g_right / Constants::Grid_Width);
		const int g_upRow = static_cast<int>(g_up / Constants::Grid_Height);
		const int g_leftCol = static_cast<int>(g_left / Constants::Grid_Width);
		const int g_downRow = static_cast<int>(g_down / Constants::Grid_Width);

		const int g_rightUpType = m.map.at(g_upRow*m.getCols() + g_rightCol)->getType();
		const int g_rightDownType = m.map.at(g_downRow*m.getCols() + g_rightCol)->getType();
		const int g_leftUpType = m.map.at(g_upRow*m.getCols() + g_leftCol)->getType();
		const int g_leftDownType = m.map.at(g_downRow*m.getCols() + g_leftCol)->getType();


		//hit wall test
		if (g_dir == 0) // right
		{
			if (g_rightUpType == 1 || g_rightDownType == 1)
			{
				g.setCol(g.getCol());
				g.setDir(3);
			}
			if (p.getRow() <= g.getRow())
			{
				if (m.map.at((g.getRow() - 1)*m.getCols() + g.getCol())->getType() == 0)
				{
					g.setDir(3);
				}
			}
			else if (p.getRow() > g.getRow())
			{
				if (m.map.at((g.getRow() + 1)*m.getCols() + g.getCol())->getType() == 0)
				{
					g.setDir(1);
				}
			}
		}
		else if (g_dir == 1)// down
		{
			if (g_rightDownType == 1 || g_leftDownType == 1)
			{
				g.setRow(g.getRow());
				g.setDir(0);
			}
			if (p.getCol() <= g.getCol())
			{
				if (m.map.at((g.getRow())*m.getCols() + g.getCol() - 1)->getType() == 0)
				{
					g.setDir(2);
				}
			}
			else if (p.getCol() > g.getCol())
			{
				if (m.map.at((g.getRow())*m.getCols() + g.getCol() + 1)->getType() == 0)
				{
					g.setDir(0);
				}
			}
		}
		else if (g_dir == 2)//left
		{
			if (g_leftUpType == 1 || g_leftDownType == 1)
			{
				g.setCol(g.getCol() + 1);
				g.setDir(1);
			}
			if (p.getRow() <= g.getRow())
			{
				if (m.map.at((g.getRow() - 1)*m.getCols() + g.getCol())->getType() == 0)
				{
					g.setDir(3);
				}
			}
			else if (p.getRow() > g.getRow())
			{
				if (m.map.at((g.getRow() + 1)*m.getCols() + g.getCol())->getType() == 0)
				{
					g.setDir(1);
				}
			}
				
			
		}
		else if (g_dir == 3)//up
		{
			if (g_rightUpType == 1 || g_leftUpType == 1)
			{
				g.setRow(g.getRow() + 1);
				g.setDir(2);
			}
			if (p.getCol() <= g.getCol())
			{
				if (m.map.at((g.getRow())*m.getCols() + g.getCol() - 1)->getType() == 0)
				{
					g.setDir(2);
				}
			}
			else if (p.getCol() > g.getCol())
			{
				if (m.map.at((g.getRow())*m.getCols() + g.getCol() + 1)->getType() == 0)
				{
					g.setDir(0);
				}
			}
		}
	}
};
