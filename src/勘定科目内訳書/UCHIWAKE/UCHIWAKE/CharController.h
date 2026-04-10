#pragma once

class CharController
{
public:
	CharController(){};
	~CharController(){};
	int Divide(CString source, CString& result, int length);
	int Divide(CString source, CString& result1, CString& result2, int length);
	int GetDivideLength(const CString& source, int length);
};
