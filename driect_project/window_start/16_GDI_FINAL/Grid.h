#pragma once
#include "IDrawable.h"
#include "Constants.h"

class Grid : public IDrawable
{
public:
	Grid(int type, int row, int col):mType(type), mCol(col), mRow(row) {};
	~Grid() = default;

	inline const int getType() const 
	{
		return mType;
	}

	inline const int getCol() const
	{
		return mCol;
	}
	
	inline const int getRow() const
	{
		return mRow;
	}
	
	virtual void draw(HDC& dest, HDC& src, HBITMAP& image)
	{
		//1.先把图片放到兼容DC中
		SelectObject(src, image);
		//2.开始贴图
		BitBlt(dest, mCol * Constants::Grid_Width, mRow * Constants::Grid_Height, Constants::Grid_Width, Constants::Grid_Height,
			src, 416, 96, SRCCOPY);
	};
private:
	int mCol = -1;
	int mRow = -1;
	int mType = -1;
};