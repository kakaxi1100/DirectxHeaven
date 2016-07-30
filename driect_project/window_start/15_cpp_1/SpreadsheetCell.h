#pragma once
#include <string>
class SpreadsheetCell
{
public:
	SpreadsheetCell(const SpreadsheetCell& rsh);
	void setValue(double inValue);
	double getValue() const;
	void setString(const std::string inString);
	const std::string getString() const;
private:
	std::string doubleToString(double inValue) const;
	double stringToDouble(const std::string inString) const;
	double mValue;
	std::string mString;
};