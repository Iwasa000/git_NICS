
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
// 消費税申告書（金額）テーブルオープン
//-----------------------------------------------------------------------------
// 引数	pdatebase	：	オープン対象DB
//		filter		：	条件フィルタ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::OpenShLstVal( CDatabase* pdatebase, LPCTSTR filter/*=_T("")*/ )
{
	ASSERT( pdatebase );
	if( pdatebase == NULL ){
		return -1;
	}

	if( m_pSH_LST_VAL ){	// オープン済み？
		CloseShLstVal();
	}
	else{
		if( !IsExist(SH_LST_VAL_TBL, pdatebase) ){
			//if( MakePrfetaxTbl(MK_SH_LST_VAL_SQL, SH_LST_VAL_TBL, pdatebase, 2) ){
			if( MakePrfetaxTbl(MK_SH_LST_VAL_SQL, SH_LST_VAL_TBL, pdatebase, 3) ){
				return -1;
			}
		}
		else{
			CString	sysExName, sysExVer, Sql;
			Sql.Empty();
			Sql.Format( GET_VERSION_SQL, SH_LST_VAL_TBL );

			CRecordset	mstExDbrec( pdatebase );
			try{
				mstExDbrec.Open( CRecordset::forwardOnly, Sql, CRecordset::readOnly );
			}
			catch( CDBException* dbe ){
//				SetErrMsg( DB_FATAL_ERROR, dbe->m_strError.GetBuffer(), 0 );
				dbe->Delete();
				return -1;
			}

			BOOL fndFlg = FALSE;
			for( int i=0;; i++ ){
				if( i != 0 ){
					mstExDbrec.MoveNext();
				}
				if( mstExDbrec.IsEOF() ){
					break;
				}
				// バージョン取得
				mstExDbrec.GetFieldValue( (SHORT)0, sysExName );
				mstExDbrec.GetFieldValue( (SHORT)1, sysExVer );
				if( sysExName == _T("version") ){
					fndFlg = TRUE;
					break;
				}
			}
			mstExDbrec.Close();

			int	nVer = atoi( sysExVer );
			if( nVer == 1 ){
				// データをクリアしてバージョンを更新
				CString	Sql1, Sql2;
				Sql1.Format( _T("delete from %s"), SH_LST_VAL_TBL );
				Sql2.Format( UPD_SVER_SQL, 2, SH_LST_VAL_TBL );

				pdatebase->BeginTrans();
				try{
					pdatebase->ExecuteSQL( Sql1 );
					pdatebase->ExecuteSQL( Sql2 );
				}
				catch( CDBException* dbe ){
					pdatebase->Rollback();
					dbe->Delete();
					return -1;
				}
				pdatebase->CommitTrans();
				nVer = 2;
			}
			else if( fndFlg == FALSE ){
				// データをクリアしてバージョンを更新
				CString	Sql1;
				Sql1.Format( SET_VERSION_SQL, 2, SH_LST_VAL_TBL );

				pdatebase->BeginTrans();
				try{
					pdatebase->ExecuteSQL( Sql1 );
				}
				catch( CDBException* dbe ){
					m_ErrMsg = dbe->m_strError;
					pdatebase->Rollback();
					dbe->Delete();
					return -1;
				}
				pdatebase->CommitTrans();
				nVer = 2;
			}
// 230718Aw add -->
		//	if(nVer == 2)	{
			if (nVer < 3) {	//23.11.1 nVer == 0の場合に不正終了するため変更
				// カラムを拡張してバージョンを更新
				CString	Sql1,Sql2,Sql3;
				Sql1 = _T("if not exists ( SELECT name FROM sys.columns WHERE OBJECT_ID IN (SELECT OBJECT_ID ('SYZ_SNK_LST_VAL')) and name = 'ksname' ) begin ");
				Sql1 += _T("alter table SYZ_SNK_LST_VAL add ksname varchar(40) ");
				Sql1 += _T("end ");

				Sql2 = _T("if not exists ( SELECT name FROM sys.columns WHERE OBJECT_ID IN (SELECT OBJECT_ID ('SYZ_SNK_LST_VAL')) and name = 'kssgn1' ) begin ");
				Sql2 += _T("alter table SYZ_SNK_LST_VAL add kssgn1 tinyint ");
				Sql2 += _T("end ");

				Sql3.Format( UPD_SVER_SQL, 3, SH_LST_VAL_TBL );

				pdatebase->BeginTrans();
				try{
					pdatebase->ExecuteSQL( Sql1 );
					pdatebase->ExecuteSQL( Sql2 );
					pdatebase->ExecuteSQL( Sql3 );
				}
				catch( CDBException* dbe ){
					pdatebase->Rollback();
					dbe->Delete();
					return -1;
				}
				pdatebase->CommitTrans();
			}
// 230718Aw add <--
		}
	}

	CSh_lst_val	*pSh_lst_val = NULL;
	if( (pSh_lst_val = new CSh_lst_val(pdatebase)) == NULL ){
		return -1;
	}
	m_pSH_LST_VAL = &pSh_lst_val->sh;
	m_pSH_LST_VAL->m_pSh_lst_val = pSh_lst_val;

	pSh_lst_val->m_strSort = SH_LST_VAL_SORT;
	pSh_lst_val->m_strFilter = filter;

	char	buf[256] = {0};
	int		rt = 0;
	try{
		rt = pSh_lst_val->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_ErrMsg = _T("消費税申告書（金額）テーブルにアクセス出来ません。");
		return -1;
	}

	if( pSh_lst_val->IsBOF() ){
		m_pSH_LST_VAL->sn_seq = -1;
		m_pSH_LST_VAL->nodata = 1;
	}
	m_pSH_LST_VAL->data();

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税申告書（金額）テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::CloseShLstVal()
{
	if( m_pSH_LST_VAL ){
		m_pSH_LST_VAL->m_pSh_lst_val->Close();
		delete m_pSH_LST_VAL->m_pSh_lst_val;
		m_pSH_LST_VAL = NULL;
	}
}

//-----------------------------------------------------------------------------
// 消費税申告書（割合）テーブルオープン
//-----------------------------------------------------------------------------
// 引数	pdatebase	：	オープン対象DB
//		filter		：	条件フィルタ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::OpenShLstRate( CDatabase* pdatebase, LPCTSTR filter/*=_T("")*/ )
{
	ASSERT( pdatebase );
	if( pdatebase == NULL ){
		return -1;
	}

	if( m_pSH_LST_RATE ){	// オープン済み？
		CloseShLstRate();
	}
	else{
		if( !IsExist(SH_LST_RATE_TBL, pdatebase) ){
			if( MakePrfetaxTbl(MK_SH_LST_RATE_SQL, SH_LST_RATE_TBL, pdatebase, 1) ){
				return -1;
			}
		}
	}

	CSh_lst_rate	*pSh_lst_rate = NULL;
	if( (pSh_lst_rate = new CSh_lst_rate(pdatebase)) == NULL ){
		return -1;
	}
	m_pSH_LST_RATE = &pSh_lst_rate->sh;
	m_pSH_LST_RATE->m_pSh_lst_rate = pSh_lst_rate;

	pSh_lst_rate->m_strSort = SH_LST_RATE_SORT;
	pSh_lst_rate->m_strFilter = filter;

	char	buf[256] = {0};
	int		rt = 0;
	try{
		rt = pSh_lst_rate->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_ErrMsg = _T("消費税申告書（割合）テーブルにアクセス出来ません。");
		return -1;
	}

	if( pSh_lst_rate->IsBOF() ){
		m_pSH_LST_RATE->sn_seq = -1;
		m_pSH_LST_RATE->nodata = 1;
	}
	m_pSH_LST_RATE->data();

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税申告書（割合）テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::CloseShLstRate()
{
	if( m_pSH_LST_RATE ){
		m_pSH_LST_RATE->m_pSh_lst_rate->Close();
		delete m_pSH_LST_RATE->m_pSh_lst_rate;
		m_pSH_LST_RATE = NULL;
	}
}

//-----------------------------------------------------------------------------
// テーブルのバージョン取得
//-----------------------------------------------------------------------------
// 引数	pDatabase	：	取得対象DB
//		pTbname		：	テーブル名
//-----------------------------------------------------------------------------
// 返送値	1以上	：	バージョン
//			0		：	バージョン無し
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::GetTblVersion(CDatabase* pDatabase, LPCTSTR pTbname)
{
	ASSERT(pDatabase);

	CString	sysExName, sysExVer, Sql;
	Sql.Empty();
	Sql.Format(GET_VERSION_SQL, pTbname);

	CRecordset	mstExDbrec(pDatabase);
	try {
		mstExDbrec.Open(CRecordset::forwardOnly, Sql, CRecordset::readOnly);
	}
	catch (CDBException* dbe) {
		dbe->Delete();
		return -1;
	}

	BOOL fndFlg = FALSE;
	for (int i = 0;; i++) {
		if (i != 0) {
			mstExDbrec.MoveNext();
		}
		if (mstExDbrec.IsEOF()) {
			break;
		}
		// バージョン取得
		mstExDbrec.GetFieldValue((SHORT)0, sysExName);
		mstExDbrec.GetFieldValue((SHORT)1, sysExVer);
		if (sysExName == _T("version")) {
			fndFlg = TRUE;
			break;
		}
	}
	mstExDbrec.Close();

	if (fndFlg) {
		int	nVer = atoi(sysExVer);
		return nVer;
	}
	else {
		return -1;
	}
}
