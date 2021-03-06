#include "Map.h"

using namespace std;

Map::Map(const int cols, const int rows, vector<int> lst):mCols(cols), mRows(rows)
{
	mCols = cols;
	mRows = rows;
	int len = static_cast<int>(lst.size());
	for (int i = 0; i < len; i++)
	{
		int r = i / cols;
		int c = i % cols;
		int type = lst.at(i);
		map.push_back(make_unique<Grid>(type, r, c));
	}
}

void Map::draw(HDC & dest, HDC & src, HBITMAP & image)
{
	for (const auto& grid : map)
	{
		if (grid->getType() == Constants::WALL) {
			grid->draw(dest, src, image);
		}
	}
}

const int Map::getCols() const
{
	return mCols;
}

const int Map::getRows() const
{
	return mRows;
}
