#ifndef CLOSE	// 08.05 /08
# ifndef _CRC_H_
# define _CRC_H_

#pragma once
class CCRC
{
public:
	CCRC(void);
	~CCRC(void);

	// CRCテーブル作成
	int BuildCRCTable(void);
	//	DESCRIPTION: CRC を算出します．
	DWORD Calculate(LPCVOID lpBuf, UINT nCount);
	//	DESCRIPTION: CRC-Table を解体します．
	void ReleaseCRCTable();

protected:
	// CRCテーブル
	DWORD* s_dwCRCTable;
};
# endif
#endif