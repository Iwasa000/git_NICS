#pragma once

const int clmIndex = 4;	// seq,NumPage,NumRow,FgFuncˆÈŠO

typedef struct _PRINT_TEMP_DATA{
	int nFormSeq;
	BOOL nRowKei;
	int nKeiType;
}PRINT_TEMP_DATA;

class CPrintTemp
{
public:
	CPrintTemp(const PRINT_TEMP_DATA& ptd, CDatabase* _db, const CString table_name);
	~CPrintTemp(void);
	int GetNumPage();

private:
	int GetColumnName(vector<CString>& vec_clm_name);

private:
	PRINT_TEMP_DATA m_ptd;
	CDatabase*	m_db;
	CString m_TableName;
};
