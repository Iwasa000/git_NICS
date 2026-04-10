#include "StdAfx.h"
#ifndef CLOSE	// 08.05 /08
#include "CRC.h"

CCRC::CCRC(void)
: s_dwCRCTable(NULL)
{
	s_dwCRCTable = NULL;
}

CCRC::~CCRC(void)
{
	ReleaseCRCTable();
}

// CRCテーブル作成
int CCRC::BuildCRCTable(void)
{
	if(!s_dwCRCTable)
	{
		s_dwCRCTable = new DWORD[256];
		if( !s_dwCRCTable )
			return -1;

		for(DWORD dw = 0; dw < 256; dw++)
		{
			DWORD dwCRC = dw;

			for(int i = 0; i < 8; i++)
			{
				if(dwCRC & 1)
				{
					dwCRC = (dwCRC >> 1) ^ 0xEDB88320;
				}
				else
				{
					dwCRC >>= 1;
				}
			}

			s_dwCRCTable[dw] = dwCRC;
		}
	}
	return 0;
}

//	CFileCRC::Calculate()
//	DESCRIPTION: CRC を算出します．
DWORD CCRC::Calculate(LPCVOID lpBuf, UINT nCount)
{
	ASSERT(s_dwCRCTable);
	ASSERT(lpBuf);

	const BYTE* lpbBuf = static_cast<const BYTE*>(lpBuf);

	DWORD dw = 0 ^ 0xFFFFFFFF;

	for(; nCount > 0; nCount--)
	{
		dw = (dw >> 8) ^ s_dwCRCTable[(dw ^ *lpbBuf++) & 0xFF];
	}

	return (dw ^ 0xFFFFFFFF);
}

//	CFileCRC::ReleaseCRCTable()
//	DESCRIPTION: CRC-Table を解体します．
void CCRC::ReleaseCRCTable()
{
	if( s_dwCRCTable ) {
		delete [] s_dwCRCTable;

		s_dwCRCTable = NULL;
	}
}
#endif