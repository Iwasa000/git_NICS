#pragma once

class CConvOutRangai
{
public:
	CConvOutRangai(){}

	BYTE ToDB(int PrintPage, int PrintType)
	{
		BYTE rangai = 0;
		// 出力ページ
		switch(PrintPage){
			case 3:
				rangai |= BIT_D0;
				rangai |= BIT_D1;
				break;
			case 2:
				rangai &= ~BIT_D0;
				rangai |= BIT_D1;
				break;
			case 1:
				rangai |= BIT_D0;
				rangai &= ~BIT_D1;
				break;
			default:
				rangai &= ~BIT_D0;
				rangai &= ~BIT_D1;
				break;
		}
		// 出力形式
		if(PrintType)	rangai |= BIT_D2;
		else			rangai &= ~BIT_D2;

		return rangai;
	}

	void ToMemory(BYTE rangai, int& PrintPage, int& PrintType)
	{
		// 出力ページ
		if(rangai & 0x03)	PrintPage = rangai & 0x03;
		else				PrintPage = 0;

		// 出力形式
		if(rangai & BIT_D2)	PrintType = 1;
		else				PrintType = 0;
	}
};