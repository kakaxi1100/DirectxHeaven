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
private:
	const int mCols;
	const int mRows;
	std::vector<std::unique_ptr<Grid>> map;
};