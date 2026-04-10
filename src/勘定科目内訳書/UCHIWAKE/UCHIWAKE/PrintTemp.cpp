#include "StdAfx.h"
#include "PrintTemp.h"

CPrintTemp::CPrintTemp(const PRINT_TEMP_DATA& ptd, CDatabase* _db, const CString table_name):
m_ptd(ptd), 
m_db(_db),
m_TableName(table_name)
{
}

CPrintTemp::~CPrintTemp(void)
{
}

int CPrintTemp::GetNumPage()
{
	// カラム名取得
	std::vector<CString> clm;
	int st = GetColumnName(clm);
	if(st != 0)	return DB_ERR_OPEN;

	// SELECT句のカラム名作成
	CString strColumn;
	size_t max = clm.size();
	for(int i=0; i<max; i++){
		strColumn = strColumn + _T(", ") + clm[i];
	}

	// 一時テーブル名作成
	CString strTempRowIdTbl;
// midori 160610 cor -->
	//strTempRowIdTbl.Format( _T("#row_id_tbl_%d"), m_ptd.nFormSeq );
// ---------------------
#ifdef _DEBUG
	strTempRowIdTbl.Format( _T("##row_id_tbl_%d"), m_ptd.nFormSeq );
#else
	strTempRowIdTbl.Format( _T("#row_id_tbl_%d"), m_ptd.nFormSeq );
#endif
// midori 160610 cor <--

	// 科目出力
	int is_kamoku = 1;
	if(is_kamoku){
	}

	// ０円空欄カット処理
	if(1){
		// ０円カット
		// 空欄カット
	}

	// 科目行追加
	if(1){
	}

	return DB_ERR_OK;
}

//カラム名をアレイに追加する
int CPrintTemp::GetColumnName(std::vector<CString> &vec)
{
	CString		strSQL;
	CdbBase		db_base(m_db);
	CRecordset	rs(m_db);

	strSQL.Format(_T("SELECT name FROM sys.columns WHERE object_id = object_id('%s', 'u') AND column_id > %d"), m_TableName, clmIndex);

	if(!db_base.OpenEx( &rs , strSQL , m_TableName))	return DB_ERR_OPEN;

	while (!rs.IsEOF()) {
		CString strBuff;
		rs.GetFieldValue(_T("name"), strBuff);
		vec.push_back(strBuff);

		rs.MoveNext();
	}

	rs.Close();

	return DB_ERR_OK;
}
