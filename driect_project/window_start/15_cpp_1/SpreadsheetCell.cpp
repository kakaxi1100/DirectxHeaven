#include "SpreadsheetCell.h"
#include <iostream>
#include <sstream>

using namespace std;
SpreadsheetCell::SpreadsheetCell(const SpreadsheetCell & rsh)
{
	rsh.mValue;
}
void SpreadsheetCell::setValue(double inValue)
{
	mValue = inValue;
	mString = doubleToString(mValue);
}

double SpreadsheetCell::getValue() const
{
	return mValue;
}

void SpreadsheetCell::setString(const std::string inString)
{
	mString = inString;

}

const std::string SpreadsheetCell::getString() const
{
	return mString;
}

std::string SpreadsheetCell::doubleToString(double inValue) const
{
	ostringstream ostr;
	ostr << inValue;
	return ostr.str();
}

double SpreadsheetCell::stringToDouble(const std::string inString) const
{
	double temp;
	istringstream istr(inString);
	istr >> temp;
	if (istr.fail() || !istr.eof())
	{
		return 0;
	}
	return temp;
}
