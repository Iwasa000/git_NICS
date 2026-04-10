#pragma once

#include "DBSubFnc8.h"				// Š¿Žš‹t•ÏŠ·ƒNƒ‰ƒX

#define LENGTH_FURIGANA		6

class CFuriganaConverter
{
public:
	CRevConv	StrConv;

public:
	CFuriganaConverter();
	~CFuriganaConverter();
	CString Convert(CString strBase);
};
