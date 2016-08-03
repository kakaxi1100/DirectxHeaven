#pragma once
#include <vector>
#include <memory>
#include "Grid.h"
class Map : public IDrawable
{
public:
	Map(const int cols, const int rows, std::vector<int> lst);
	~Map() = default;
	virtual void draw(HDC& dest, HDC& src, HBITMAP& image);
	const int getCols() const;
	const int getRows() const;
	
	std::vector<std::shared_ptr<Grid>> map;
private:
	int mCols;
	int mRows;
};