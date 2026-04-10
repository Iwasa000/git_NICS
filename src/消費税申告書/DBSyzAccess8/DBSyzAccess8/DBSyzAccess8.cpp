// DBSyzAccess8.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _MANAGED
#error /clr と共にコンパイルするには DBSyzAccess8.cpp 内の操作手順を参照してください。
// /clr をプロジェクトに追加したい場合は、次の操作を行ってください:
//	1. 上記の afxdllx.h のインクルードを削除する
//	2. .cpp ファイルを、プリコンパイル済みヘッダーが無効にされていて次のテキストを含み、
//	   /clr がスローされていないプロジェクトに追加します:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE DBSyzAccess8DLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DBSyzAccess8.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(DBSyzAccess8DLL, hInstance))
			return 0;

		// この DLL をリソース チェーンへ挿入します。
		// メモ: 拡張 DLL が MFC アプリケーションではなく
		//  MFC 標準 DLL (ActiveX コントロールのような)
		//  に暗黙的にリンクされる場合、この行を DllMain
		//  から削除して、この拡張 DLL からエクスポート
		//  された別の関数内へ追加してください。
		//  この拡張 DLL を使用する標準 DLL はこの拡張 DLL
		//  を初期化するために明示的にその関数を呼び出します。
		//  それ以外の場合は、CDynLinkLibrary オブジェクトは
		//  標準 DLL のリソース チェーンへアタッチされず、
		//  その結果重大な問題となります。

		new CDynLinkLibrary(DBSyzAccess8DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DBSyzAccess8.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(DBSyzAccess8DLL);
	}
	return 1;   // OK
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

CDBSyzAccess::CDBSyzAccess()
{
	m_pBmeszsyk = NULL;
	m_pBmespsyk = NULL;
	m_pShinPayment = NULL;
	m_pRefundvol = NULL;
	m_pRefundrec = NULL;
	m_pSkjrec = NULL;
	m_pReask = NULL;
	m_pKjsnk = NULL;
//--> '14.01.06 INS START
	m_pSH_LST_VAL = NULL;
	m_pSH_LST_RATE = NULL;
//<-- '14.01.06 INS START
//2018.03.13 INSERT START
	m_pFhyo60rec = NULL;
	m_pSyz_Sozoku = NULL;
    m_pSyz_Tokurei = NULL;
	m_pSzsyksub = NULL;
//2018.03.13 INSERT END
}

CDBSyzAccess::~CDBSyzAccess()
{
	if( m_pBmeszsyk ){
		BmeSzsykClose();
		m_pBmeszsyk = NULL;
	}
	if( m_pBmespsyk ){
		BmeSpsykClose();
		m_pBmespsyk = NULL;
	}
	if( m_pShinPayment ){
		ShinPayMentClose();
		m_pShinPayment = NULL;
	}
	if( m_pRefundvol ){
		ReFundVolClose();
		m_pRefundvol = NULL;
	}
	if( m_pRefundrec ){
		ReFundRecClose();
		m_pRefundrec = NULL;
	}
	if( m_pSkjrec ){
		SkjrecClose();
		m_pSkjrec = NULL;
	}
	if( m_pReask ){
		ReaskClose();
		m_pReask = NULL;
	}
	if( m_pKjsnk ){
		KjsnkClose();
		m_pKjsnk = NULL;
	}
//--> '14.01.06 INS START
	if( m_pSH_LST_VAL ){
		CloseShLstVal();
		m_pSH_LST_VAL = NULL;
	}
	if( m_pSH_LST_RATE ){
		CloseShLstRate();
		m_pSH_LST_RATE = NULL;
	}
//<-- '14.01.06 INS START
//2018.03.13 INSERT START
	if( m_pFhyo60rec ){
		Fhyo60RecClose();
		m_pFhyo60rec = NULL;
	}
	if( m_pSyz_Sozoku ){
		Syz_SozokuClose();
		m_pSyz_Sozoku = NULL;
	}
    if( m_pSyz_Tokurei ) {
        Syz_TokureiClose();
        m_pSyz_Tokurei = NULL;
	}
//2018.03.13 INSERT END
	if (m_pSzsyksub) {
		SzsyksubClose();
		m_pSzsyksub = NULL;
	}

}

//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::BmeSzsykOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	CBmeszsyk *bmeszsyk;

	if( (bmeszsyk = new CBmeszsyk(pdatebase)) == NULL ){
//		m_pBmeszsyk->ErrMsg = _T("消費税拡張クラスの構築が出来ません。");
		return -1;
	}
	m_pBmeszsyk = &bmeszsyk->pr;
	m_pBmeszsyk->m_pbmeszsyk = bmeszsyk;

	bmeszsyk->m_strSort = _T("SZtype,SZmnth,SZbumn,SZkcod,SZedbn,SZgrp1,SZgrp2,SZzkbn,SZitem,SZsjsg,SZrvsg");
	bmeszsyk->m_strFilter = filter;

	int rt;
	try{
		rt = bmeszsyk->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pBmeszsyk->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pBmeszsyk->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_pBmeszsyk->ErrMsg = _T("消費税拡張マスターにアクセスできません。");
		return -1;
	}

	if( bmeszsyk->IsBOF() ){
		m_pBmeszsyk->SZtype = -1;
		m_pBmeszsyk->nodata = 1;
	}
	m_pBmeszsyk->data();

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::BmeSzsykClose( void )
{
	if( m_pBmeszsyk ){
		m_pBmeszsyk->m_pbmeszsyk->Close();
		delete m_pBmeszsyk->m_pbmeszsyk;
		m_pBmeszsyk = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetBmeSzsykErrMsg()
{
	m_ErrMsg.Empty();
	if( m_pBmeszsyk )	m_ErrMsg = m_pBmeszsyk->ErrMsg;
	return m_ErrMsg;
}

//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::BmeSpsykOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	CBmespsyk *bmespsyk;

	if( (bmespsyk = new CBmespsyk(pdatebase)) == NULL ){
//		m_pBmespsyk->ErrMsg = _T("拡張特定収入クラスの構築が出来ません。");
		return -1;
	}
	m_pBmespsyk = &bmespsyk->pr;
	m_pBmespsyk->m_pbmespsyk = bmespsyk;

	bmespsyk->m_strSort = _T("SPtype,SPmnth,SPbumn,SPkcod,SPedbn,SPgrp1,SPgrp2,SPzkbn,SPitem,SPrvsg");
	bmespsyk->m_strFilter = filter;

	int rt;
	try{
		rt = bmespsyk->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pBmespsyk->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pBmespsyk->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_pBmespsyk->ErrMsg = _T("拡張特定収入マスターにアクセスできません。");
		return -1;
	}

	if( bmespsyk->IsBOF() ){
		m_pBmespsyk->SPtype = -1;
		m_pBmespsyk->nodata = 1;
	}
	m_pBmespsyk->data();

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::BmeSpsykClose( void )
{
	if( m_pBmespsyk ){
		m_pBmespsyk->m_pbmespsyk->Close();
		delete m_pBmespsyk->m_pbmespsyk;
		m_pBmespsyk = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetBmeSpsykErrMsg()
{
	m_ErrMsg.Empty();
	if( m_pBmespsyk )	m_ErrMsg = m_pBmespsyk->ErrMsg;
	return m_ErrMsg;
}

////////////////////////////
// 消費税改正追加マスター //
////////////////////////////

// 納付書（領収済通知書）
//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::ShinPayMentOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	CShinPayment *shinpayment;

	if( !IsExist( SHINPAYMENT_TBL, pdatebase ) ) {
		if( MakePrfetaxTbl( SHINPAYMENT_SQL, SHINPAYMENT_TBL, pdatebase, 1 ) )	{
			return(-1);
		}
	}

	if( (shinpayment = new CShinPayment(pdatebase)) == NULL ){
//		m_pShinPayment->ErrMsg = _T("納付書クラスの構築が出来ません。");
		return -1;
	}
	m_pShinPayment = &shinpayment->pr;
	m_pShinPayment->m_pshinpayment = shinpayment;

	shinpayment->m_strSort = _T("NFItem,NFLine,NFResv,NFSequ");
	shinpayment->m_strFilter = filter;

	int rt;
	try{
		rt = shinpayment->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pShinPayment->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pShinPayment->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_pShinPayment->ErrMsg = _T("納付書マスターにアクセスできません。");
		return -1;
	}

	if( shinpayment->IsBOF() ){
		m_pShinPayment->NFItem = -1;
		m_pShinPayment->nodata = 1;
	}
	m_pShinPayment->data();

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::ShinPayMentClose( void )
{
	if( m_pShinPayment ){
		m_pShinPayment->m_pshinpayment->Close();
		delete m_pShinPayment->m_pshinpayment;
		m_pShinPayment = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetShinPayMentErrMsg()
{
	m_ErrMsg.Empty();
	if( m_pShinPayment )	m_ErrMsg = m_pShinPayment->ErrMsg;
	return m_ErrMsg;
}

// 消費税の還付申告に関する明細書（基本情報）
//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::ReFundVolOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	CRefundvol *refundvol;

	if( !IsExist( REFUNDVOL_TBL, pdatebase ) ) {
		if( MakePrfetaxTbl( REFUNDVOL_SQL, REFUNDVOL_TBL, pdatebase, 1 ) )	{
			return(-1);
		}
	}

	if( (refundvol = new CRefundvol(pdatebase)) == NULL ){
//		m_pRefundvol->ErrMsg = _T("消費税の還付申告に関する明細書（基本）クラスの構築が出来ません。");
		return -1;
	}
	m_pRefundvol = &refundvol->pr;
	m_pRefundvol->m_prefundvol = refundvol;

	refundvol->m_strSort = _T("KPItem,KPLine,KPResv,KPSequ");
	refundvol->m_strFilter = filter;

	int rt;
	try{
		rt = refundvol->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pRefundvol->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pRefundvol->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_pRefundvol->ErrMsg = _T("消費税の還付申告に関する明細書（基本）にアクセスできません。");
		return -1;
	}

	if( refundvol->IsBOF() ){
		m_pRefundvol->KPItem = -1;
		m_pRefundvol->nodata = 1;
	}
	m_pRefundvol->data();

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::ReFundVolClose( void )
{
	if( m_pRefundvol ){
		m_pRefundvol->m_prefundvol->Close();
		delete m_pRefundvol->m_prefundvol;
		m_pRefundvol = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetReFundVolErrMsg()
{
	m_ErrMsg.Empty();
	if( m_pRefundvol )	m_ErrMsg = m_pRefundvol->ErrMsg;
	return m_ErrMsg;
}

// 消費税の還付申告に関する明細書（次葉）
//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::ReFundRecOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	CRefundrec *refundrec;

	if( !IsExist( REFUNDREC_TBL, pdatebase ) ) {
//		if( MakePrfetaxTbl( REFUNDREC_SQL, REFUNDREC_TBL, pdatebase, 1 ) )	{
		if( MakePrfetaxTbl( REFUNDREC_SQL, REFUNDREC_TBL, pdatebase, 2 ) )	{	// 20230704
			return(-1);
		}
	}
	//----> 20230704
	else{
		// テーブルが存在する時にはバージョンを見て、バージョンアップ
		if (VupRefundRec(pdatebase)) {
			ASSERT(!_T("テーブル（還付付表）のバージョンアップに失敗"));
			return -1;
		}		
	}
	//<---- 20230704

	if( (refundrec = new CRefundrec(pdatebase)) == NULL ){
//		m_pRefundrec->ErrMsg = _T("消費税の還付申告に関する明細書（次葉）クラスの構築が出来ません。");
		return -1;
	}
	m_pRefundrec = &refundrec->pr;
	m_pRefundrec->m_prefundrec = refundrec;

	refundrec->m_strSort = _T("KPItem,KPLine,KPResv,KPSequ");
	refundrec->m_strFilter = filter;

	int rt;
	try{
		rt = refundrec->Open();
	}
	catch( CDBException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pRefundrec->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch( CMemoryException* dbe ){
		dbe->GetErrorMessage( buf, sizeof(buf) );
		m_pRefundrec->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if( rt == 0 ) {
		m_pRefundrec->ErrMsg = _T("消費税の還付申告に関する明細書（次葉）にアクセスできません。");
		return -1;
	}

	if( refundrec->IsBOF() ){
		m_pRefundrec->KPItem = -1;
		m_pRefundrec->nodata = 1;
	}
	m_pRefundrec->data();

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルバージョンアップ（還付付表：refundrec）20230704
//-----------------------------------------------------------------------------
// 引数	pdatebase	：	走査対象DB
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::VupRefundRec( CDatabase* pdatebase )
{
	ASSERT(pdatebase);

	// バージョンの取得
	int nVer = GetTblVersion(pdatebase, REFUNDREC_TBL);
	//if (nVer == -1) {
	//	return -1;
	//}
	//else if (nVer == 0) {
	//	// バージョンが付与されていない時には、どうする？？
	//	;
	//}

	// 拡張プロパティが存在していない場合、GetTblVersion()は-1を返送するため nVer <= 0　とする
	if( nVer <= 0 ) {

		// バージョンが付与されていない場合には、現在のバージョンをセットする
		if( IsExistColumn( REFUNDREC_TBL, ADDREFUNDRECCOLUMN, pdatebase ) == 1 ) {
			// テーブルが拡張済みであれば最新バージョンの2をセット
			nVer = 2;
		}
		else {
			// テーブルが未拡張であれば旧バージョンの1をセットし、下記バージョンアップを行う
			nVer = 1;
		}

		SetTableVersion( pdatebase, REFUNDREC_TBL, nVer );
	}

	if (nVer == 1) {
		// バージョン１から２へのバージョンアップ
		CString Sql;
		Sql.Format(UPD_SVER_SQL, 2, REFUNDREC_TBL);

		pdatebase->BeginTrans();
		try {
			pdatebase->ExecuteSQL(VUP1_REFUNDREC_SQL);
			pdatebase->ExecuteSQL(Sql);
		}
		catch (CDBException *pDbe) {
			pdatebase->Rollback();
			pDbe->Delete();
			return -1;
		}
		pdatebase->CommitTrans();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::ReFundRecClose( void )
{
	if( m_pRefundrec ){
		m_pRefundrec->m_prefundrec->Close();
		delete m_pRefundrec->m_prefundrec;
		m_pRefundrec = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetReFundRecErrMsg()
{
	m_ErrMsg.Empty();
	if( m_pRefundrec )	m_ErrMsg = m_pRefundrec->ErrMsg;
	return m_ErrMsg;
}

// 仕入控除税額に関する明細書次葉
//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::SkjrecOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	m_ErrMsg.Empty();

	CSkjrec *skjrec;

	if( !IsExist( SKJREC_TBL, pdatebase ) ) {
		if( MakePrfetaxTbl( SKJREC_SQL, SKJREC_TBL, pdatebase, 1 ) )	{
//			m_ErrMsg = _T("仕入控除税額に関する明細書(明細情報)クラスの構築が出来ません。");
			return(-1);
		}
	}

	if( (skjrec = new CSkjrec(pdatebase)) == NULL ){
		return -1;
	}
	m_pSkjrec = &skjrec->pr;
	m_pSkjrec->m_pskjrec = skjrec;

	skjrec->m_strSort = _T("SkjLine,SkjColm,SkjSaveDay");
	skjrec->m_strFilter = filter;

	int rt;
	try{
		rt = skjrec->Open();
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
		m_ErrMsg = _T("仕入控除税額に関する明細書(明細情報)にアクセスできません。");
		return -1;
	}

	if( skjrec->IsBOF() ){
		m_pSkjrec->SkjLine = -1;
		m_pSkjrec->nodata = 1;
	}
	m_pSkjrec->data();

	return 0;
}
//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::SkjrecClose( void )
{
	if( m_pSkjrec ){
		m_pSkjrec->m_pskjrec->Close();
		delete m_pSkjrec->m_pskjrec;
		m_pSkjrec = NULL;
	}
}

//-----------------------------------------------------------------------------
// テーブルバージョンアップ（更正の請求：reask）更正の請求/新様式対応
//-----------------------------------------------------------------------------
// 引数	pdatebase	：	走査対象DB
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::VupReaskRec( CDatabase* pdatebase )
{
	ASSERT(pdatebase);

	// バージョンの取得
	int nVer = GetTblVersion(pdatebase, REASK_TBL);
	//if (nVer == -1) {
	//	return -1;
	//}
	//else if (nVer == 0) {
	//	// バージョンが付与されていない時には、どうする？？
	//	;
	//}

	// 拡張プロパティが存在していない場合、GetTblVersion()は-1を返送するため nVer <= 0　とする
	if( nVer <= 0 ) {

		// バージョンが付与されていない場合には、現在のバージョンをセットする
		if( IsExistColumn( REASK_TBL, ADDREASKCOLUMN, pdatebase ) == 1 ) {
			// テーブルが拡張済みであれば最新バージョンの2をセット
			nVer = 2;
		}
		else {
			// テーブルが未拡張であれば旧バージョンの1をセットし、下記バージョンアップを行う
			nVer = 1;
		}

		SetTableVersion( pdatebase, REASK_TBL, nVer );
	}

	if (nVer == 1) {
		// バージョン１から２へのバージョンアップ
		CString Sql;
		Sql.Format(UPD_SVER_SQL, 2, REASK_TBL);

		pdatebase->BeginTrans();
		try {
			pdatebase->ExecuteSQL(VUP1_REASK_SQL);
			pdatebase->ExecuteSQL(Sql);
		}
		catch (CDBException *pDbe) {
			pdatebase->Rollback();
			pDbe->Delete();
			return -1;
		}
		pdatebase->CommitTrans();
	}

	return 0;
}

// 更正の請求書
//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::ReaskOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	m_ErrMsg.Empty();

	CReask *preask;

	if( !IsExist( REASK_TBL, pdatebase ) ) {
		if( MakePrfetaxTbl( REASK_SQL, REASK_TBL, pdatebase, 2 ) )	{	// 更正の請求/新様式対応
//		if( MakePrfetaxTbl( REASK_SQL, REASK_TBL, pdatebase, 1 ) )	{
//			m_ErrMsg = _T("更正の請求書クラスの構築が出来ません。");
			return -1;
		}
	}
	else {
		// 更正の請求/新様式対応
		// テーブルが存在する時にはバージョンを見て、バージョンアップ
		if (VupReaskRec(pdatebase)) {
			ASSERT(!_T("テーブル（更正の請求）のバージョンアップに失敗"));
			return -1;
		}
	}

	if( (preask = new CReask(pdatebase)) == NULL ){
		return -1;
	}
	m_pReask = &preask->pr;
	m_pReask->m_preask = preask;

	preask->m_strSort = _T("KStype");
	preask->m_strFilter = filter;

	int rt;
	try{
		rt = preask->Open();
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
		m_ErrMsg = _T("更正の請求書クラスにアクセス出来ません。");
		return -1;
	}

	if( preask->IsBOF() ){
		m_pReask->KStype = -1;
		m_pReask->nodata = 1;
	}
	m_pReask->data();

	return 0;
}
//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::ReaskClose( void )
{
	if( m_pReask ){
		m_pReask->m_preask->Close();
		delete m_pReask->m_preask;
		m_pReask = NULL;
	}
}

// 課税取引金額計算表
//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::KjsnkOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	CKjsnk *pkjsnk;

	if( !IsExist( KJSNK_TBL, pdatebase ) ) {
		if( MakePrfetaxTbl( KJSNK_SQL, KJSNK_TBL, pdatebase, 1 ) )	{
//			m_ErrMsg = _T("課税取引金額計算表クラスの構築が出来ません。");
			return -1;
		}
	}

	if( (pkjsnk = new CKjsnk(pdatebase)) == NULL ){
		return -1;
	}
	m_pKjsnk = &pkjsnk->pr;
	m_pKjsnk->m_pkjsnk = pkjsnk;

	pkjsnk->m_strSort = _T("kslist,ksline,ksitem");
	pkjsnk->m_strFilter = filter;

	int rt;
	try{
		rt = pkjsnk->Open();
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
		m_ErrMsg = _T("課税取引金額計算表クラスにアクセス出来ません。");
		return -1;
	}

	if( pkjsnk->IsBOF() ){
		m_pKjsnk->kslist = -1;
		m_pKjsnk->nodata = 1;
	}
	m_pKjsnk->data();

	return 0;
}
//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::KjsnkClose( void )
{
	if( m_pKjsnk ){
		m_pKjsnk->m_pkjsnk->Close();
		delete m_pKjsnk->m_pkjsnk;
		m_pKjsnk = NULL;
	}
}

//-----------------------------------------------------------------------------
// テーブルバージョンアップ（付表６：fhyo60rec）
//-----------------------------------------------------------------------------
// 引数	pdatebase	：	走査対象DB
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::VupFhyo60Rec( CDatabase* pdatebase )
{
	ASSERT(pdatebase);

	// バージョンの取得
	int nVer = GetTblVersion(pdatebase, FHYO60REC_TBL);
	if (nVer == -1) {
		return -1;
	}
	else if (nVer == 0) {
		// バージョンが付与されていない時には、どうする？？
		;
	}

	if (nVer == 1) {
		// バージョン１から２へのバージョンアップ
		CString Sql;
		Sql.Format(UPD_SVER_SQL, 2, FHYO60REC_TBL);

		pdatebase->BeginTrans();
		try {
			pdatebase->ExecuteSQL(VUP1_FHYO60REC_SQL);
			pdatebase->ExecuteSQL(Sql);
		}
		catch (CDBException *pDbe) {
			pdatebase->Rollback();
			pDbe->Delete();
			return -1;
		}
		pdatebase->CommitTrans();
	}

	return 0;
}

//2018.03.13 INSERT START
int CDBSyzAccess::Fhyo60RecOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	m_ErrMsg.Empty();

	CFhyo60Rec *pfhyo60rec;

	if( !IsExist( FHYO60REC_TBL, pdatebase ) ) {
//		if( MakePrfetaxTbl( FHYO60REC_SQL, FHYO60REC_TBL, pdatebase, 1 ) )	{
		if (MakePrfetaxTbl(FHYO60REC_SQL, FHYO60REC_TBL, pdatebase, 2)) {
			//			m_ErrMsg = _T("付表６クラスの構築が出来ません。");
			return -1;
		}
	}
	else {
		// テーブルが存在する時にはバージョンを見て、バージョンアップ
		if (VupFhyo60Rec(pdatebase)) {
			ASSERT(!_T("テーブル（付表６）のバージョンアップに失敗"));
			return -1;
		}
	}

	if( (pfhyo60rec = new CFhyo60Rec(pdatebase)) == NULL ){
		return -1;
	}
	m_pFhyo60rec = &pfhyo60rec->pr;
	m_pFhyo60rec->m_pfhyo60rec = pfhyo60rec;

	pfhyo60rec->m_strSort = _T("Sn_SEQ");
	pfhyo60rec->m_strFilter = filter;

	int rt;
	try{
		rt = pfhyo60rec->Open();
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
		m_ErrMsg = _T("付表６クラスにアクセス出来ません。");
		return -1;
	}

	if( pfhyo60rec->IsBOF() ){
		m_pFhyo60rec->Sn_SEQ = -1;
		m_pFhyo60rec->nodata = 1;
	}
	m_pFhyo60rec->data();

	return 0;
}

void CDBSyzAccess::Fhyo60RecClose()
{
	if( m_pFhyo60rec ){
		m_pFhyo60rec->m_pfhyo60rec->Close();
		delete m_pFhyo60rec->m_pfhyo60rec;
		m_pFhyo60rec = NULL;
	}
}

int CDBSyzAccess::Syz_SozokuOpen( CDatabase* pdatebase, LPCTSTR filter )
{
	char	buf[256];

	m_ErrMsg.Empty();

	CSyz_Sozoku *psyz_sozoku;

	if( !IsExist( SYZ_SOZOKU_TBL, pdatebase ) ) {
		if( MakePrfetaxTbl( SYZ_SOZOKU_SQL, SYZ_SOZOKU_TBL, pdatebase, 1 ) )	{
//			m_ErrMsg = _T("付表６相続人クラスの構築が出来ません。");
			return -1;
		}
	}

	if( (psyz_sozoku = new CSyz_Sozoku(pdatebase)) == NULL ){
		return -1;
	}
	m_pSyz_Sozoku = &psyz_sozoku->pr;
	m_pSyz_Sozoku->m_psyz_sozoku =psyz_sozoku;

	psyz_sozoku->m_strSort = _T("Sn_SEQ,Sn_NO");
	psyz_sozoku->m_strFilter = filter;

	int rt;
	try{
		rt = psyz_sozoku->Open();
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
		m_ErrMsg = _T("付表６相続人クラスにアクセス出来ません。");
		return -1;
	}

	if( psyz_sozoku->IsBOF() ){
		m_pSyz_Sozoku->Sn_SEQ = -1;
		m_pSyz_Sozoku->nodata = 1;
	}
	m_pSyz_Sozoku->data();

	return 0;
}

void CDBSyzAccess::Syz_SozokuClose()
{
	if( m_pSyz_Sozoku ){
		m_pSyz_Sozoku->m_psyz_sozoku->Close();
		delete m_pSyz_Sozoku->m_psyz_sozoku;
		m_pSyz_Sozoku = NULL;
	}
}
//2018.03.13 INSERT END

int CDBSyzAccess::Syz_TokureiOpen(CDatabase* pdatebase, LPCTSTR filter)
{
    char buf[256];
	m_ErrMsg.Empty();

	CSyz_Tokurei *psyz_tokurei;

	if (!IsExist(SYZ_TOKUREI_TBL, pdatebase)) {
		if (MakePrfetaxTbl(SYZ_TOKUREI_SQL, SYZ_TOKUREI_TBL, pdatebase, 1)) {
			return -1;
		}
	}

	if ((psyz_tokurei = new CSyz_Tokurei(pdatebase)) == NULL) {
		return -1;
	}
	m_pSyz_Tokurei = &psyz_tokurei->pr;
	m_pSyz_Tokurei->m_psyz_tokurei = psyz_tokurei;

	psyz_tokurei->m_strSort = SYZ_TOKUREI_VAL_SORT;
	psyz_tokurei->m_strFilter = filter;

	int rt;
	try {
		rt = psyz_tokurei->Open();
	}
	catch (CDBException* dbe) {
		dbe->GetErrorMessage(buf, sizeof(buf));
		m_ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch (CMemoryException* dbe) {
		dbe->GetErrorMessage(buf, sizeof(buf));
		m_ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if (rt == 0) {
		m_ErrMsg = _T("特例帳票クラスにアクセス出来ません。");
		return -1;
	}

	if (psyz_tokurei->IsBOF()) {
		m_pSyz_Tokurei->sn_seq = -1;
		m_pSyz_Tokurei->nodata = 1;
	}
	m_pSyz_Tokurei->data();

    return 0;
}

void CDBSyzAccess::Syz_TokureiClose()
{
	if (m_pSyz_Tokurei) {
		m_pSyz_Tokurei->m_psyz_tokurei->Close();
		delete m_pSyz_Tokurei->m_psyz_tokurei;
		m_pSyz_Tokurei = NULL;
	}
}


//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
int CDBSyzAccess::SzsyksubOpen(CDatabase* pdatebase, LPCTSTR filter)
{
	char buf[256];

	CSzsyksub *pSzsyksub;

	if (!IsExist(SYZ_SZSYKSUB_TBL, pdatebase)) {
		if (MakePrfetaxTbl(SYZ_SZSYKSUB_SQL, SYZ_SZSYKSUB_TBL, pdatebase, 1)) {
			return -1;
		}
	}

	if ((pSzsyksub = new CSzsyksub(pdatebase)) == NULL) {
		return -1;
	}
	m_pSzsyksub = &pSzsyksub->pr;
	m_pSzsyksub->m_pSzsyksub = pSzsyksub;

	pSzsyksub->m_strSort = SYZ_SZSYKSUB_VAL_SORT;
	pSzsyksub->m_strFilter = filter;

	int rt;
	try {
		rt = pSzsyksub->Open();
	}
	catch (CDBException* dbe) {
		dbe->GetErrorMessage(buf, sizeof(buf));
		m_pSzsyksub->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}
	catch (CMemoryException* dbe) {
		dbe->GetErrorMessage(buf, sizeof(buf));
		m_pSzsyksub->ErrMsg = buf;
		dbe->Delete();
		return -1;
	}

	if (rt == 0) {
		m_pSzsyksub->ErrMsg = _T("消費税集計サブテーブルクラスにアクセス出来ません。");
		return -1;
	}

	if (pSzsyksub->IsBOF()) {
		m_pSzsyksub->Syksubofs = UCHAR_MAX;
		m_pSzsyksub->nodata = 1;
	}

	m_pSzsyksub->data();

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
void CDBSyzAccess::SzsyksubClose()
{
	if (m_pSzsyksub) {
		m_pSzsyksub->m_pSzsyksub->Close();
		delete m_pSzsyksub->m_pSzsyksub;
		m_pSzsyksub = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetSzsyksubErrMsg()
{
	m_ErrMsg.Empty();
	if (m_pSzsyksub) {
		m_ErrMsg = m_pSzsyksub->ErrMsg;
	}
	return m_ErrMsg;
}

//-----------------------------------------------------------------------------
// 月別情報毎の税額計算種別取得関数 
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	財務クラス
//      ZeiCalqEachMonth：	月毎情報格納用構造体
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::GetSyzZeiCalqForEachMonth(CDBZmSub* pZmsub, CArray<ZeiCalqEachMonth, ZeiCalqEachMonth&>* pArray)
{
	BOOL bMonthInfoOpen = FALSE;
	BOOL bZvolOpen = FALSE;

    if( pZmsub->minfo == NULL){
		if( pZmsub->MinfoOpen() < 0) {
			return -1; 
		}
		bMonthInfoOpen = TRUE;
	}
	if (pZmsub->zvol == NULL) {
		if (pZmsub->VolumeOpen() < 0) {
			return -1;
		}
		bZvolOpen = TRUE;
	}

    if( pZmsub->SyzcalqmthOpen() < 0){
		return -1;
	}

    pArray->RemoveAll();

    //month_infoのオフセットごとに税額計算種別情報を格納.
	ZeiCalqEachMonth ListItem = {0};
    int zeicalqmth_smd = 0, zeicalqmth_emd = 0, pre_syzcalqmth_emd = 0, pre_syzcalqmth_zeicalq = 0;
    for(int i = 0; i < pZmsub->minfo->GetRecordCount(); i++){

		if (i == 0) {
			pZmsub->minfo->MoveFirst();
		}
		else {
			pZmsub->minfo->MoveNext();
		}

        //売上区分の税額計算種別
		memset(&ListItem, -1, sizeof(ListItem));
		ListItem.month_info_ofs = pZmsub->minfo->ofset;
		//ListItem.calqkbn = 0; //売上区分

        //SYZ_ZEICALQ_MTHテーブルの情報を元に月オフセット毎の税額計算種別を取得.
        for(int j = 0; j < pZmsub->syzcalqmth->GetRecordCount(); j++){
			if (j == 0) {
				pZmsub->syzcalqmth->MoveFirst();
			}
			else {
				pZmsub->syzcalqmth->MoveNext();
			}

            //期首日、期末日は0になっているのでzvolumeから取得.
			if (pZmsub->syzcalqmth->smd == 0) {
				zeicalqmth_smd = pZmsub->zvol->ss_ymd;
			}
			else {
				zeicalqmth_smd = pZmsub->syzcalqmth->smd;
			}
			if (pZmsub->syzcalqmth->emd == 0) {
				zeicalqmth_emd = pZmsub->zvol->ee_ymd;
			}
			else {
				zeicalqmth_emd = pZmsub->syzcalqmth->emd;
			}

			//DbgViewTrace(_T("【DEBUG】%d %s() [minfo->ofset:%d] month_smd:%d month_emd:%d syzcalqmthofset:%d zeicalqmth_smd:%d zeicalqmth_emd:%d\n"), 
			//	__LINE__, __FUNCTION__, pZmsub->minfo->ofset, pZmsub->minfo->smd, pZmsub->minfo->emd, j,zeicalqmth_smd, zeicalqmth_emd);

            if( 
                //SYZ_ZEICALQ_MTH開始日 - 該当月開始日 - SYZ_ZEICALQ_MTH終了日 - 該当月終了日				
                ((zeicalqmth_smd <= pZmsub->minfo->smd) && (pZmsub->minfo->smd <= zeicalqmth_emd) && (zeicalqmth_emd <= pZmsub->minfo->emd))
				//該当月開始日 - SYZ_ZEICALQ_MTH開始日 - 該当月終了日 - SYZ_ZEICALQ_MTH終了日
				|| ((pZmsub->minfo->smd <= zeicalqmth_smd) && (zeicalqmth_smd <= pZmsub->minfo->emd) && (pZmsub->minfo->emd <= zeicalqmth_emd))
				//SYZ_ZEICALQ_MTH開始日 - 該当月開始日 - 該当月終了日 - SYZ_ZEICALQ_MTH終了日
				|| ((zeicalqmth_smd <= pZmsub->minfo->smd) && (pZmsub->minfo->smd <= pZmsub->minfo->emd) && (pZmsub->minfo->emd <= zeicalqmth_emd))
			) {
                //該当月開始日 - 前行の終了日,現在行の開始日 - 該当月終了日
                if( (pZmsub->minfo->smd <= pre_syzcalqmth_emd) && (pZmsub->minfo->smd <= zeicalqmth_smd)
                    && (pre_syzcalqmth_emd <= pZmsub->minfo->emd) && (zeicalqmth_smd <= pZmsub->minfo->emd)){
                    //前行の計算種別と違えば混合
                    if(pre_syzcalqmth_zeicalq != pZmsub->syzcalqmth->zeicalq ) {
						if (pZmsub->syzcalqmth->calqkbn == 0) {
							ListItem.zeicalq_uri = 2;
						}
                        else {
							ListItem.zeicalq_sire = 2;
						}
					}
				}
                else {
					if (pZmsub->syzcalqmth->calqkbn == 0) {
						ListItem.zeicalq_uri = pZmsub->syzcalqmth->zeicalq;
					}
					else {
						ListItem.zeicalq_sire = pZmsub->syzcalqmth->zeicalq;
					}
				}

				//DbgViewTrace(_T("【DEBUG】%d %s() [minfo->ofset:%d] syzcalqmthofset:%d ListItem.zeicalq_uri:%d ListItem.zeicalq_sire:%d\n"),
				//	__LINE__, __FUNCTION__, pZmsub->minfo->ofset, j, ListItem.zeicalq_uri, ListItem.zeicalq_sire);

			}
			pre_syzcalqmth_emd = pZmsub->syzcalqmth->emd;
			pre_syzcalqmth_zeicalq = pZmsub->syzcalqmth->zeicalq;
		}

		pArray->Add(ListItem);
	}

    if( bMonthInfoOpen ) {
		pZmsub->MinfoClose();
	}

    if( bZvolOpen ) {
		pZmsub->VolumeClose();
	}

    pZmsub->SyzcalqmthClose();

    return 0;
}

//-----------------------------------------------------------------------------
// 月別情報毎の税額計算種別取得関数 
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	財務クラス
//      ZeiCalqEachMonth：	月毎情報格納用構造体
//      Sn_TUMIAG:消費税積上げ設定サイン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::GetSyzZeiCalqForEachMonth2(CDBZmSub* pZmsub, CArray<ZeiCalqEachMonth, ZeiCalqEachMonth&>* pArray, const unsigned char Sn_TUMIAG)
{
	BOOL bMonthInfoOpen = FALSE;
	BOOL bZvolOpen = FALSE;
	BOOL bSyzszsyksubmthOpen = FALSE;
	if (pZmsub->minfo == NULL) {
		if (pZmsub->MinfoOpen() < 0) {
			return -1;
		}
		bMonthInfoOpen = TRUE;
	}

	if (pZmsub->zvol == NULL) {
		if (pZmsub->VolumeOpen() < 0) {
			return -1;
		}
		bZvolOpen = TRUE;
	}

    if( pZmsub->syzszsyksubmth == NULL){
		if (pZmsub->SyzSzSyksubmthOpen() < 0) {
			if (bMonthInfoOpen) {
				pZmsub->MinfoClose();
			}
			if (bZvolOpen) {
				pZmsub->VolumeClose();
			}
			return -1;
		}
		bSyzszsyksubmthOpen = TRUE;
	}

	if (pZmsub->SyzcalqmthOpen() < 0) {
		if (bMonthInfoOpen) {
			pZmsub->MinfoClose();
		}
		if (bZvolOpen) {
			pZmsub->VolumeClose();
		}
        if (bSyzszsyksubmthOpen){
			pZmsub->SyzSzSyksubmthClose();
		}
		return -1;
	}

    //syzszsyksubmthのemd列に20230930があるかどうか
    bool bBunkatsu = FALSE;
	for(int i = 0; i < pZmsub->syzszsyksubmth->GetRecordCount(); i++){
		if (i == 0) {
			pZmsub->syzszsyksubmth->MoveFirst();
		}
		else {
			pZmsub->syzszsyksubmth->MoveNext();
		}
        if(pZmsub->syzszsyksubmth->emd == 20230930){
			bBunkatsu = TRUE;
		}
	}

	pArray->RemoveAll();

	//month_infoのオフセットごとに税額計算種別情報を格納.
	ZeiCalqEachMonth ListItem = { 0 };
	int zeicalqmth_smd = 0, zeicalqmth_emd = 0, pre_syzcalqmth_emd = 0, pre_syzcalqmth_zeicalq = 0;
	for (int i = 0; i < pZmsub->minfo->GetRecordCount(); i++) {

		if (i == 0) {
			pZmsub->minfo->MoveFirst();
		}
		else {
			pZmsub->minfo->MoveNext();
		}

		//売上区分の税額計算種別
		memset(&ListItem, -1, sizeof(ListItem));
		ListItem.month_info_ofs = pZmsub->minfo->ofset;
		//ListItem.calqkbn = 0; //売上区分

		//SYZ_ZEICALQ_MTHテーブルの情報を元に月オフセット毎の税額計算種別を取得.
		for (int j = 0; j < pZmsub->syzcalqmth->GetRecordCount(); j++) {
			if (j == 0) {
				pZmsub->syzcalqmth->MoveFirst();
			}
			else {
				pZmsub->syzcalqmth->MoveNext();
			}

			//期首日、期末日は0になっているのでzvolumeから取得.
			if (pZmsub->syzcalqmth->smd == 0) {
				zeicalqmth_smd = pZmsub->zvol->ss_ymd;
			}
			else {
				zeicalqmth_smd = pZmsub->syzcalqmth->smd;
			}
			if (pZmsub->syzcalqmth->emd == 0) {
				zeicalqmth_emd = pZmsub->zvol->ee_ymd;
			}
			else {
				zeicalqmth_emd = pZmsub->syzcalqmth->emd;
			}

			//DbgViewTrace(_T("【DEBUG】%d %s() [minfo->ofset:%d] month_smd:%d month_emd:%d syzcalqmthofset:%d zeicalqmth_smd:%d zeicalqmth_emd:%d\n"), 
			//	__LINE__, __FUNCTION__, pZmsub->minfo->ofset, pZmsub->minfo->smd, pZmsub->minfo->emd, j,zeicalqmth_smd, zeicalqmth_emd);

			if (
				//SYZ_ZEICALQ_MTH開始日 - 該当月開始日 - SYZ_ZEICALQ_MTH終了日 - 該当月終了日				
				((zeicalqmth_smd <= pZmsub->minfo->smd) && (pZmsub->minfo->smd <= zeicalqmth_emd) && (zeicalqmth_emd <= pZmsub->minfo->emd))
				//該当月開始日 - SYZ_ZEICALQ_MTH開始日 - 該当月終了日 - SYZ_ZEICALQ_MTH終了日
				|| ((pZmsub->minfo->smd <= zeicalqmth_smd) && (zeicalqmth_smd <= pZmsub->minfo->emd) && (pZmsub->minfo->emd <= zeicalqmth_emd))
				//SYZ_ZEICALQ_MTH開始日 - 該当月開始日 - 該当月終了日 - SYZ_ZEICALQ_MTH終了日
				|| ((zeicalqmth_smd <= pZmsub->minfo->smd) && (pZmsub->minfo->smd <= pZmsub->minfo->emd) && (pZmsub->minfo->emd <= zeicalqmth_emd))
				) {

                //該当月開始日 - INVOICE_SYZ_YMD - 該当月終了日 且つ 20231001跨ぎで分割
				if( (pZmsub->minfo->smd <= INVOICE_SYZ_YMD) && (INVOICE_SYZ_YMD <= pZmsub->minfo->emd) && bBunkatsu) {

					//DbgViewTrace(_T("【DEBUG】%d %s() pZmsub->syzcalqmth->calqkbn:%d  pZmsub->syzcalqmth->zeicalq:%d \n"), 
					//	__LINE__, __FUNCTION__, pZmsub->syzcalqmth->calqkbn, pZmsub->syzcalqmth->zeicalq);

					//決算月の場合,混合月とせずそのまま
					if (pZmsub->minfo->kes_sgn) {
						if (pZmsub->syzcalqmth->calqkbn == 0) {
							ListItem.zeicalq_uri = pZmsub->syzcalqmth->zeicalq;
						}
						else {
							ListItem.zeicalq_sire = pZmsub->syzcalqmth->zeicalq;
						}
					}
                    else {
						//20231001跨ぎはパターン別に戻り値3-6で返す.
						if (pZmsub->syzcalqmth->calqkbn == 0) {
							if (Sn_TUMIAG & 0x01) { //消費税申告書 積上げ設定(課税標準額の消費税額)
								if (pZmsub->syzcalqmth->zeicalq == 0) {
									ListItem.zeicalq_uri = 3;
								}
								else {
									ListItem.zeicalq_uri = 4;
								}
							}
							else {
								if (pZmsub->syzcalqmth->zeicalq == 0) {
									ListItem.zeicalq_uri = 5;
								}
								else {
									ListItem.zeicalq_uri = 6;
								}
							}
						}
						else {
							if (Sn_TUMIAG & 0x02) {//消費税申告書 積上げ設定(控除対象仕入税額)
								if (pZmsub->syzcalqmth->zeicalq == 0) {
									ListItem.zeicalq_sire = 3;
								}
								else {
									ListItem.zeicalq_sire = 4;
								}
							}
							else {
								if (pZmsub->syzcalqmth->zeicalq == 0) {
									ListItem.zeicalq_sire = 5;
								}
								else {
									ListItem.zeicalq_sire = 6;
								}
							}
						}
					}
				}
				//該当月開始日 - 前行の終了日,現在行の開始日 - 該当月終了日				{
				else if ((pZmsub->minfo->smd <= pre_syzcalqmth_emd) && (pZmsub->minfo->smd <= zeicalqmth_smd)
					&& (pre_syzcalqmth_emd <= pZmsub->minfo->emd) && (zeicalqmth_smd <= pZmsub->minfo->emd)) {

                    //決算月の場合,混合月とせずそのまま
                    if( pZmsub->minfo->kes_sgn ) {
						if (pZmsub->syzcalqmth->calqkbn == 0) {
							ListItem.zeicalq_uri = pZmsub->syzcalqmth->zeicalq;
						}
						else {
							ListItem.zeicalq_sire = pZmsub->syzcalqmth->zeicalq; 
						}
					}
                    else {
						//前行の計算種別と違えば混合
						if (pre_syzcalqmth_zeicalq != pZmsub->syzcalqmth->zeicalq) {
							if (pZmsub->syzcalqmth->calqkbn == 0) {
								ListItem.zeicalq_uri = 2;
							}
							else {
								ListItem.zeicalq_sire = 2;
							}
						}
					}
				}
				else {
					if (pZmsub->syzcalqmth->calqkbn == 0) {
						ListItem.zeicalq_uri = pZmsub->syzcalqmth->zeicalq;
					}
					else {
						ListItem.zeicalq_sire = pZmsub->syzcalqmth->zeicalq;
					}
				}

				//DbgViewTrace(_T("【DEBUG】%d %s() [minfo->ofset:%d] syzcalqmthofset:%d ListItem.zeicalq_uri:%d ListItem.zeicalq_sire:%d\n"),
				//	__LINE__, __FUNCTION__, pZmsub->minfo->ofset, j, ListItem.zeicalq_uri, ListItem.zeicalq_sire);

			}
			pre_syzcalqmth_emd = pZmsub->syzcalqmth->emd;
			pre_syzcalqmth_zeicalq = pZmsub->syzcalqmth->zeicalq;

			//DbgViewTrace(_T("【DEBUG】%d %s() [minfo->ofset:%d] pre_syzcalqmth_emd:%d pre_syzcalqmth_zeicalq:%d\n"),
			//	__LINE__, __FUNCTION__, pZmsub->minfo->ofset, pre_syzcalqmth_emd, pre_syzcalqmth_zeicalq);

		}

		pArray->Add(ListItem);
	}

	if (bMonthInfoOpen) {
		pZmsub->MinfoClose();
	}

	if (bZvolOpen) {
		pZmsub->VolumeClose();
	}

	if (bSyzszsyksubmthOpen) {
		pZmsub->SyzSzSyksubmthClose();
	}

	pZmsub->SyzcalqmthClose();

	return 0;
}
//-----------------------------------------------------------------------------
// 開始日から終了日に税額計算の「積上げ計算」「割戻し計算」が混在しているかチェック.
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	財務クラス
// 	       smd	：	判定開始日
// 	       ymd	：	判定終了日
// 	       bMixUriage	：	税額計算(売上) false:混合なし true:混合あり
// 	         bMixSire	：	税額計算(仕入) false:混合なし true:混合あり
//-----------------------------------------------------------------------------
// 返送値	 0：	正常終了
//			-1：	エラー
//			-2：	エラー(期間外)
//-----------------------------------------------------------------------------
int CDBSyzAccess::CheckSyzZeiCalqSireKonzai(CDBZmSub* pZmsub, const int smd, const int emd, bool* bMixUriage, bool* bMixSire)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->m_database);

    BOOL bHantei = FALSE;
	BOOL bZeicalqUW = FALSE; //割戻し計算(売上)
	BOOL bZeicalqUT = FALSE; //積上げ計算(売上)
	BOOL bZeicalqSW = FALSE; //割戻し計算(仕入)
	BOOL bZeicalqST = FALSE; //積上げ計算(仕入)
	int line_smd = 0, line_emd = 0;

	//SYZ_ZEICALQ_MTHのデータを元に判定
	if (pZmsub->SyzcalqmthOpen() < 0) {
		return -1;
	}
	if (pZmsub->minfo == NULL) {
		if (pZmsub->MinfoOpen() < 0) {
			return -1;
		}
	}
	if (pZmsub->zvol == NULL) {
		if (pZmsub->VolumeOpen() < 0) {
			return -1;
		}
	}

	*bMixUriage = false;
	*bMixSire = false;

    //決算期間外だったら判定しない.
    if( (smd < pZmsub->zvol->ss_ymd) || (pZmsub->zvol->ee_ymd < emd)){
        return -2;
	}

    //SYZ_ZEICALQ_MTHテーブルから仕入税額が混合計算になっているか判定.
    int nSyzCalqMthRecord = pZmsub->syzcalqmth->GetRecordCount();
    for(int i = 0; i < nSyzCalqMthRecord; i++){

		if (i == 0) {
			pZmsub->syzcalqmth->MoveFirst();
		}
		else {
			pZmsub->syzcalqmth->MoveNext();
		}

		if (nSyzCalqMthRecord == 2) {
			//SYZ_ZEICALQ_MTHのレコード数が2なら売上、仕入の期間分割は行われていないので抜ける.(混合なし)
			break;
		}

		//SYZ_ZEICALQ_MTHの開始日が期首日の場合,0なのでzvolから取得.
		if (pZmsub->syzcalqmth->smd == 0) {
			line_smd = pZmsub->zvol->ss_ymd;
		}
		else {
			line_smd = pZmsub->syzcalqmth->smd;
		}

		//該当行の終了日が期末日の場合,0なのでzvolから取得.
		if (pZmsub->syzcalqmth->emd == 0) {
			if (pZmsub->syzcalqmth->smd == INVOICE_SYZ_YMD) {
				//開始日がインボイス施工日であれば期間分割されていない.
				continue;
			}
			line_emd = pZmsub->zvol->ee_ymd;
		}
		else {
			line_emd = pZmsub->syzcalqmth->emd;
		}

	    //DbgViewTrace(_T("【DEBUG】%d %s() line_smd:%d line_emd:%d smd:%d emd:%d\n"), __LINE__, __FUNCTION__, line_smd, line_emd, smd, emd);

        //SYZ_ZEICALQ_MTH該当行のsmd <= 開始日 <= SYZ_ZEICALQ_MTHであれば判定開始
        if( (line_smd <= smd) && (smd <= line_emd) ){
            bHantei = TRUE;
			//DbgViewTrace(_T("【DEBUG】%d %s() 判定開始\n"), __LINE__, __FUNCTION__);
		}

        //判定中は税額計算状態を取得.
        if( bHantei ) {

            if( pZmsub->syzcalqmth->calqkbn == 0 ) {
				if (pZmsub->syzcalqmth->zeicalq == 0) {
					bZeicalqUW = TRUE;
				}
				else {
					bZeicalqUT = TRUE;
				}
			}
			if (pZmsub->syzcalqmth->calqkbn == 1) {
				if (pZmsub->syzcalqmth->zeicalq == 0) {
					bZeicalqSW = TRUE;
				}
				else {
					bZeicalqST = TRUE;
				}
			}

			//DbgViewTrace(_T("【DEBUG】%d %s() 判定中 bZeicalqUW:%d bZeicalqUT:%d bZeicalqSW:%d bZeicalqST:%d\n"), 
			//	__LINE__, __FUNCTION__,
			//	bZeicalqUW, bZeicalqUT, bZeicalqSW, bZeicalqST);

            //取得した状態が割戻しと積上げ計算、どちらもあれば混合
            if( bZeicalqUW && bZeicalqUT ) {
                *bMixUriage = true;
			}
			if (bZeicalqSW && bZeicalqST) {
				*bMixSire = true;
			}
		}

		//SYZ_SZSYKSUB_MTH該当行のsmd <= 終了日 <= emdであれば判定終了
		if ( (line_smd <= emd) && (emd <= line_emd)) {
			bHantei = FALSE;
			//DbgViewTrace(_T("【DEBUG】%d %s() 判定終了\n"), __LINE__, __FUNCTION__);
		}

	}

    return 0;
}

//5億円超え判定 指定期間内の仕訳データを集計し,課税売上高が5億円を超えているかどうか判定する
//またその集計金額を返す.
//引数 pZmsub:財務クラス smd:指定期間開始日 emd:指定期間終了日(課税開始日が設定されている場合は課税開始日の前日までの集計) money:集計金額
bool CDBSyzAccess::IsOver5okuYen(CDBZmSub* pZmsub, long sddate, long eddate, char* syukei_money)
{

	bool bOver = false;
	CArithEx ArithEx;

	ArithEx.l_defn(0x16);

	BOOL bOpenFlg = FALSE;
	if (pZmsub != NULL) {
		if (pZmsub->zvol == NULL) {
			pZmsub->VolumeOpen();
			bOpenFlg = TRUE;
		}
	}

	static char* KaribaraiSyohizei = _T("01071101");
	static char* KariukeSyohizei = _T("04051601");

	//年次マスター
	if (!(pZmsub->zvol->m_type & 0x01)) {
		//集計用一時テーブル作成.
		if (CreateTmpTable2(pZmsub, sddate, eddate, KaribaraiSyohizei, KariukeSyohizei) < 0) {
			return bOver;
		}

		//SiwakeRecListへデータを格納.
		std::vector<SiwakeRec> SiwakeRecList;

		//SiwakeRecListへデータを格納.
		GetDbDataSub(pZmsub, SiwakeRecList, sddate, eddate);

		//SiwakeRecList内のデータから集計&判定.
		bOver = Syukei5okuHantei(pZmsub, SiwakeRecList, syukei_money, KaribaraiSyohizei, KariukeSyohizei);
	}
	//合併マスターは免税期間の課税売上高のみをSYZ_SNL_LST_VALから読み出す
	else {
		char money[6] = { 0 };
		//課税開始日が設定されている
		if (pZmsub->zvol->to_kz_ymd != 0) {
			GetMenzeiUriage(pZmsub, money);
			//5億円を超えているかどうか
			char money_5oku[6] = { 0 };
			ArithEx.l_input(&money_5oku[0], _T("500000000"));
			if (ArithEx.l_cmp(&money[0], &money_5oku[0]) > 0) {
				bOver = true;
			}
		}
		memcpy(&syukei_money[0], &money[0], sizeof(money));
		
	}

	if (bOpenFlg) {
		pZmsub->VolumeClose();
	}

	return bOver;
}



//--> 2024.07.12 h.wada add
//-----------------------------------------------------------------------------
// 免税期間の課税売上高の集計額をSYZ_SNK_LST_VALに書き込み(合併マスター用)
//-----------------------------------------------------------------------------
// 引数	dest	：	集計額を書き込むマスターの財務クラス
// 	    src	　　：	集計元にするマスタ―の財務クラス
// 	    srcCnt	：	集計元マスターのカウンタ(0...n)
//-----------------------------------------------------------------------------
// 返送値	 0：	書き込み成功、または書き込み不要
//			-1：	エラー
//-----------------------------------------------------------------------------
// 備考	 
//       財務クラスはボリュームラベルオープン済みの前提
//       destの財務クラスを使用してテーブルに接続する
//       →SYZ_SNK_LST_VALが未openの消費税拡張マスターアクセスモジュールからの呼び出しが前提
//       内部でトランザクション使うので注意
//-----------------------------------------------------------------------------


int CDBSyzAccess::SaveMenzeiUriage(CDBZmSub* dest, CDBZmSub* src, int srcCnt){

	//書き込み先パラメータ
	const int seq = 0;
	const int version = 2;
	const CString hyoId = _T("FFFFFF");
	const CString itmId = _T("FFF00000");
	const CString errMsg = _T("免税期間の課税売上高の集計中に障害が発生しました。");

	//入力チェック
	if (dest == NULL       || src == NULL)        { m_ErrMsg = errMsg; return -1; }
	if (dest->zvol == NULL || src->zvol  == NULL) { m_ErrMsg = errMsg; return -1; }
	if (m_pSH_LST_VAL != NULL)                    { m_ErrMsg = errMsg; return -1; } //m_pSH_LST_VALがすでに開いている場合、destのテーブルに接続できないのでエラー扱い
	if ((dest->zvol->to_kz_ymd == 0) != (src->zvol->to_kz_ymd == 0)) { return 0; } //課税開始日の設定が一致しない場合

	//集計不要なパターン
	if (!(dest->zvol->m_type & 0x01)) { return 0; } //書き込み先が年次マスター

	bool openLstVal = false;
	bool clearRecord = (dest->zvol->to_kz_ymd == 0) && (src->zvol->to_kz_ymd == 0);

	try {
		CString filter = _T("");
		filter.Format(_T("sn_seq = %d and version = %d and hyoId = \'%s\' and itmId = \'%s\'"), 0, version, hyoId, itmId);
		//テーブルオープン
		if (m_pSH_LST_VAL == NULL) {
			if (OpenShLstVal(dest->m_database,filter) == -1) { 
				m_ErrMsg = errMsg;
				if (openLstVal) { CloseShLstVal(); }
				return -1;
			}
			openLstVal = true;
		}
		else {
			m_pSH_LST_VAL->Requery(filter);
		}
		int cnt = m_pSH_LST_VAL->GetRecordCount();
		
		//金額クリア
		if (clearRecord) {
			if (cnt != 0) {
				m_pSH_LST_VAL->Edit();
				m_pSH_LST_VAL->val = _T("0");
				m_pSH_LST_VAL->Update();
			}
		}
		//金額書き込み
		else {
			//集計元マスターの免税期間の課税売上高を取得
			CArithEx ArithEx;
			ArithEx.l_defn(0x16);
			char data[6] = { 0 };
			IsOver5okuYen(src, src->zvol->ss_ymd, src->zvol->ee_ymd, data);

			if (cnt == 0) {
				m_pSH_LST_VAL->AddNew();
				m_pSH_LST_VAL->sn_seq = seq;
				m_pSH_LST_VAL->version = version;
				m_pSH_LST_VAL->hyoId = hyoId;
				m_pSH_LST_VAL->itmId = itmId;
			}
			else {
				m_pSH_LST_VAL->Edit();
			}

			//金額セット
			char str[64] = { 0 };
			char curValue[6] = { 0 };
			CString result = _T("");
			//AddNew()、1社目の場合元の金額を無視する
			if (cnt != 0 && srcCnt != 0) {
				strcpy_s(str, m_pSH_LST_VAL->val);
				ArithEx.l_input(curValue, str);
				ArithEx.l_add(data, data, curValue);
				::ZeroMemory(str, 64);
			}

			ArithEx.l_print(str, data, _T("SSSSSSSSSSSSS9"));
			result = str;
			result.TrimLeft();
			m_pSH_LST_VAL->val = result;
			m_pSH_LST_VAL->Update();
		}
	}
	catch (CDBException* dbe) {
		char	buf[256] = {0};
		dbe->GetErrorMessage(buf, sizeof(buf));
		CString msg = buf;
		dbe->Delete();
		m_ErrMsg = msg;
		if (openLstVal) { CloseShLstVal(); }
		return -1;
	}

	
	if (openLstVal) { CloseShLstVal(); }
	return 0;
}

//-----------------------------------------------------------------------------
// エラーメッセージを取得
//-----------------------------------------------------------------------------
// 返送値	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CDBSyzAccess::GetErrMsg()
{
	return m_ErrMsg;
}
//<-- 2024.07.12 h.wada add

//集計用一時テーブル2(5億円判定専用)
//返り値 0:正常 -1:指定期間エラー
int CDBSyzAccess::CreateTmpTable2(CDBZmSub* pZmsub, long sddate, long eddate, char* KaribaraiSyohizei, char* KariukeSyohizei)
{
	//指定期間範囲チェック
	if ((sddate < pZmsub->zvol->ss_ymd) || (eddate < pZmsub->zvol->ss_ymd)
		|| (pZmsub->zvol->ee_ymd < sddate) || (pZmsub->zvol->ee_ymd < eddate)) {
		m_ErrMsg = _T("指定期間が決算期間外です。");
		return -1;
	}

    //指定期間終了日が課税開始日を超える場合, 課税開始日の一日前までの集計とする.
    bool bOverTO_KZ_YMD = false;
    if( pZmsub->zvol->to_kz_ymd <= eddate) {
        eddate = pZmsub->zvol->to_kz_ymd;
		bOverTO_KZ_YMD = true;
	}

	CString SVKCOD1; //仮払消費税コード 
	CString SVKCOD2; //仮受消費税コード 

	CString sq, subsq;
	sq.Format("%s", _T("SELECT COUNT(*) FROM sysobjects where id = object_id('SZVOL')"));
	CRecordset dbrec(pZmsub->m_database);
	CString value;
	try {
		dbrec.Open(CRecordset::forwardOnly, sq, CRecordset::readOnly);
		if (!dbrec.IsEOF()) {
			dbrec.GetFieldValue((SHORT)0, value);
		}
		dbrec.Close();
	}
	catch (CDBException* dbe) {
		dbe->Delete();
		value.Empty();
	}

	int nExist = atoi(value.GetBuffer());
	if (nExist) {
		sq.Empty();
		sq.Format("%s", _T("SELECT SVKCOD1, SVKCOD2 FROM SZVOL"));
		try {
			dbrec.Open(CRecordset::forwardOnly, sq, CRecordset::readOnly);
			if (!dbrec.IsEOF()) {
				dbrec.GetFieldValue((SHORT)0, SVKCOD1);
				dbrec.GetFieldValue((SHORT)1, SVKCOD2);
			}
			dbrec.Close();
		}
		catch (CDBException* dbe) {
			dbe->Delete();
			SVKCOD1.Empty();
			SVKCOD2.Empty();
		}
	}
	else {
		SVKCOD1.Empty();
		SVKCOD2.Empty();
	}

	//仮払消費税コード, 仮受消費税コードをSZVOLから取得.
	if (!SVKCOD1.IsEmpty()) {
		KaribaraiSyohizei = SVKCOD1.GetBuffer();
	}
	if (!SVKCOD2.IsEmpty()) {
		KariukeSyohizei = SVKCOD2.GetBuffer();
	}

	sq.Empty();
	sq += "IF OBJECT_ID('tempdb..#knrec_kubun', 'u') IS NOT NULL DROP TABLE #knrec_kubun ";

	sq += "CREATE TABLE #knrec_kubun( ";
	sq += "kncod CHAR(8) NOT NULL, knnam VARCHAR(14), ";
	sq += "kmgroup CHAR(4) NOT NULL, kmgroupmeisai CHAR(4) NOT NULL, ";
	sq += ") ";

	sq += "INSERT INTO #knrec_kubun ";
	sq += "SELECT kncod, knnam, ";
	sq += "(CAST(CAST(knsgn AS binary(1)) AS int) & 0xFF) AS kmgroup, ";      //消費税 処理グループ区分
	sq += "(CAST(CAST(knsgn AS binary(2)) AS int) & 0xFF) AS kmgroupmeisai "; //消費税 処理グループ区分明細
	sq += "FROM knrec ";

	// 集計データ作成(仕訳区分有り).
	sq += "IF OBJECT_ID('tempdb..#datarec_SiwakeKubunAri','u') IS NOT NULL DROP TABLE #datarec_SiwakeKubunAri ";

	sq += "CREATE TABLE #datarec_SiwakeKubunAri( ";
	sq += "seq INT, ddate INT, dbt CHAR(8), dbtgroup CHAR(4), dbtgroupmeisai CHAR(4), ";
	sq += "cre CHAR(8), cregroup CHAR(4), cregroupmeisai CHAR(4), ";
	sq += "val DECIMAL(12), zei DECIMAL(12), taika DECIMAL(12), zeikubun CHAR(4), zeiritsu CHAR(4), ";
	sq += "syutokujoto CHAR(4), kazeikubun CHAR(4), jigyokubun CHAR(4), siwakekubun CHAR(4) ";
	sq += ") ";

	sq += "INSERT INTO #datarec_SiwakeKubunAri ";
	sq += "SELECT DISTINCT seq, datarec.ddate, dbt, K1.kmgroup, K1.kmgroupmeisai, cre, K2.kmgroup, K2.kmgroupmeisai, val, zei, taika, ";
	sq += "(CAST(CAST(dsign AS binary(2)) AS int) & 0xF0) AS zeikubun, ";    //税区分サイン
	sq += "(CAST(CAST(dsign AS binary(2)) AS int) & 0x0F) AS zeiritsu, ";    //税率サイン
	sq += "(CAST(CAST(dsign AS binary(3)) AS int) & 0x20) AS syutokujoto, "; //取得, 譲渡(対価)サイン
	sq += "(CAST(CAST(dsign AS binary(3)) AS int) & 0x0F) AS kazeikubun, ";  //課税区分サイン
	sq += "(CAST(CAST(dsign AS binary(4)) AS int) & 0x0F) AS jigyokubun, ";  //簡易課税の売上業種区分
	sq += "(CAST(CAST(dsign AS binary(5)) AS int) & 0xFF) AS siwakekubun ";  //消費税仕訳区分
	sq += "FROM datarec ";
	sq += "LEFT OUTER JOIN #knrec_kubun AS K1 ON dbt = K1.kncod ";
	sq += "LEFT OUTER JOIN #knrec_kubun AS K2 ON cre = K2.kncod ";

	sq += "WHERE NOT (CAST (CAST(dsign AS binary(1)) AS int) & 0x01) = 1  "; //取消仕訳ではない
	sq += "AND (K1.kmgroup IS NOT NULL OR K2.kmgroup IS NOT NULL) ";         //消費税処理グループ
	subsq.Empty();
    if(bOverTO_KZ_YMD == true) {
		subsq.Format(_T("AND (%d <= ddate AND ddate < %d) "), sddate, eddate);  //指定期間内のデータ(課税開始日の前日までに調整)
	}
    else {
		subsq.Format(_T("AND (%d <= ddate AND ddate <= %d) "), sddate, eddate); //指定期間内のデータ
	}
	sq += subsq;
	sq += "DELETE FROM #datarec_SiwakeKubunAri WHERE NOT siwakekubun > 0 ";  //消費税仕訳区分が入っていない(=0)のデータを削除

	pZmsub->m_database->ExecuteSQL(sq);
	sq.Empty();

	// 集計データ作成(仕訳区分無し).
	sq += "IF OBJECT_ID('tempdb..#datarec_SiwakeKubunNasi','u') IS NOT NULL DROP TABLE #datarec_SiwakeKubunNasi ";

	sq += "CREATE TABLE #datarec_SiwakeKubunNasi( ";
	sq += "seq INT, ddate INT, dbt CHAR(8), dbtgroup CHAR(4), dbtgroupmeisai CHAR(4), ";
	sq += "cre CHAR(8), cregroup CHAR(4), cregroupmeisai CHAR(4), ";
	sq += "val DECIMAL(12), zei DECIMAL(12), taika DECIMAL(12), zeikubun CHAR(4), zeiritsu CHAR(4), ";
	sq += "syutokujoto CHAR(4), kazeikubun CHAR(4), jigyokubun CHAR(4), siwakekubun CHAR(4) ";
	sq += ") ";

	sq += "INSERT INTO #datarec_SiwakeKubunNasi ";
	sq += "SELECT DISTINCT seq, datarec.ddate, dbt, K1.kmgroup, K1.kmgroupmeisai, cre, K2.kmgroup, K2.kmgroupmeisai, val, zei, taika, ";
	sq += "(CAST(CAST(dsign AS binary(2)) AS int) & 0xF0) AS zeikubun,  ";   //税区分サイン
	sq += "(CAST(CAST(dsign AS binary(2)) AS int) & 0x0F) AS zeiritsu, ";	 //税率サイン
	sq += "(CAST(CAST(dsign AS binary(3)) AS int) & 0x20) AS syutokujoto, "; //取得, 譲渡(対価)サイン
	sq += "(CAST(CAST(dsign AS binary(3)) AS int) & 0x0F) AS kazeikubun, ";	 //課税区分サイン
	sq += "(CAST(CAST(dsign AS binary(4)) AS int) & 0x0F) AS jigyokubun, ";	 //簡易課税の売上業種区分
	sq += "(CAST(CAST(dsign AS binary(5)) AS int) & 0xFF) AS siwakekubun ";	 //消費税仕訳区分
	sq += "FROM datarec ";
	sq += "LEFT OUTER JOIN #knrec_kubun AS K1 ON dbt = K1.kncod ";
	sq += "LEFT OUTER JOIN #knrec_kubun AS K2 ON cre = K2.kncod ";
	sq += "WHERE NOT (CAST (CAST(dsign AS binary(1)) AS int) & 0x01) = 1 "; //取消仕訳ではない
	sq += "AND (K1.kmgroup IS NOT NULL OR K2.kmgroup IS NOT NULL) ";
	subsq.Empty();
	if (bOverTO_KZ_YMD == true) {
		subsq.Format(_T("AND (%d <= ddate AND ddate < %d) "), sddate, eddate);  //指定期間内のデータ(課税開始日の前日までに調整)
	}
	else {
		subsq.Format(_T("AND (%d <= ddate AND ddate <= %d) "), sddate, eddate);        //指定期間内のデータ
	}
	sq += subsq;
	sq += "DELETE FROM #datarec_SiwakeKubunNasi ";
	sq += "WHERE siwakekubun > 0 ";                //消費税仕訳区分が入っている(>0)のデータを削除

	pZmsub->m_database->ExecuteSQL(sq);
	sq.Empty();

	//作成した一時テーブル#datarec_SiwakeKubunAriと#datarec_SiwakeKubunNasi内のデータを#datarec_SiwakeKubunAriNasiにまとめる.
	sq += "IF OBJECT_ID('tempdb..#datarec_SiwakeKubunAriNasi','u') IS NOT NULL DROP TABLE #datarec_SiwakeKubunAriNasi ";

	sq += "CREATE TABLE #datarec_SiwakeKubunAriNasi( ";
	sq += "ddate INT, dbt CHAR(8), dbtgroup CHAR(4), dbtgroupmeisai CHAR(4), ";
	sq += "cre CHAR(8), cregroup CHAR(4), cregroupmeisai CHAR(4), ";
	sq += "val DECIMAL(12), zei DECIMAL(12), taika DECIMAL(12), zeikubun CHAR(4), zeiritsu CHAR(4), ";
	sq += "syutokujoto CHAR(4), kazeikubun CHAR(4), jigyokubun CHAR(4), siwakekubun CHAR(4) ";
	sq += ") ";

	sq += "INSERT INTO #datarec_SiwakeKubunAriNasi ";
	sq += "SELECT ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, SUM(val), SUM(zei), SUM(taika), zeikubun, zeiritsu, syutokujoto, kazeikubun, jigyokubun, siwakekubun ";
	sq += "FROM #datarec_SiwakeKubunAri ";  //消費税仕訳区分ありで
	sq += "WHERE kazeikubun = 1 ";          //課税区分が課税
	subsq.Empty();

	//subsq.Format(_T("OR (dbt = '%s' OR dbt = '%s' OR cre = '%s' OR cre = '%s') "), KaribaraiSyohizei, KariukeSyohizei, KaribaraiSyohizei, KariukeSyohizei); //又は仮受消費税か仮払消費税
	subsq.Format(_T("OR (dbt LIKE '%.6s__' OR dbt LIKE '%.6s__' OR cre LIKE '%.6s__' OR cre LIKE '%.6s__') "), KaribaraiSyohizei, KariukeSyohizei, KaribaraiSyohizei, KariukeSyohizei);	// 明細拡張も含める
	sq += subsq;
	sq += "GROUP BY ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, zeikubun, zeiritsu, syutokujoto, kazeikubun, jigyokubun, siwakekubun ";

	sq += "INSERT INTO #datarec_SiwakeKubunAriNasi ";
	sq += "SELECT ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, SUM(val), SUM(zei), SUM(taika), zeikubun, zeiritsu, syutokujoto, kazeikubun, jigyokubun, siwakekubun ";
	sq += "FROM #datarec_SiwakeKubunNasi "; //消費税仕訳区分なしで
	sq += "WHERE kazeikubun = 1 ";			//課税区分が課税
	subsq.Empty();
	//subsq.Format(_T("OR (dbt = '%s' OR dbt = '%s' OR cre = '%s' OR cre = '%s') "), KaribaraiSyohizei, KariukeSyohizei, KaribaraiSyohizei, KariukeSyohizei); //又は仮受消費税か仮払消費税
	subsq.Format(_T("OR (dbt LIKE '%.6s__' OR dbt LIKE '%.6s__' OR cre LIKE '%.6s__' OR cre LIKE '%.6s__') "), KaribaraiSyohizei, KariukeSyohizei, KaribaraiSyohizei, KariukeSyohizei);	// 明細拡張も含める
	sq += subsq;
	sq += "GROUP BY ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, zeikubun, zeiritsu, syutokujoto, kazeikubun, jigyokubun, siwakekubun ";

	if ((pZmsub->zvol->s_sgn2 & 0x0f) > 0x01) {
		//簡易課税マスターの場合、仕訳データで業種区分が設定されていないものは基本業種とする.
		int gyosyu = (pZmsub->zvol->s_sgn2 & 0x0f) - 1;
		subsq.Empty();
		subsq.Format(_T("UPDATE #datarec_SiwakeKubunAriNasi SET jigyokubun = %d WHERE jigyokubun = 0"), gyosyu);
		sq += subsq;
	}
	else {
		//原則課税マスターの場合、業種区分が設定されていても業種なしとする.
		sq += "UPDATE #datarec_SiwakeKubunAriNasi SET jigyokubun = 0 ";
	}

	pZmsub->m_database->ExecuteSQL(sq);

	return 0;
}

void CDBSyzAccess::GetDbDataSub(CDBZmSub* pZmsub, std::vector<SiwakeRec>& pSiwakeRecList, long sddate, long eddate)
{
	pSiwakeRecList.clear();

	CRecordset mstDbrec(pZmsub->m_database);
	CString getVal;

	SiwakeRec Rec = { 0 };

	CString sq, subsq;
	sq.Empty();
	subsq.Empty();

	sq += "SELECT ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, val, zei, taika, zeikubun, zeiritsu, syutokujoto, jigyokubun, siwakekubun ";
	sq += "FROM #datarec_SiwakeKubunAriNasi ";
	subsq.Format(_T("WHERE (jigyokubun = %d OR jigyokubun IS NULL) "), 0);
	subsq.Empty();
	sq += "ORDER BY ddate ";

	try {
		mstDbrec.Open(CRecordset::forwardOnly, sq);
		for (int i = 0;; i++) {
			if (i != 0) {
				mstDbrec.MoveNext();
			}
			if (mstDbrec.IsEOF()) {
				break;
			}
			for (int i = 0; i < mstDbrec.m_nResultCols; i++) {
				mstDbrec.GetFieldValue(i, getVal);
				switch (i) {
				case 0:
					Rec.ddate = atol(getVal.Trim());
					break;
				case 1:
					Rec.dbt = getVal.Trim();
					break;
				case 2:
					Rec.dbtgroup = atoi(getVal.Trim());
					break;
				case 3:
					Rec.dbtgroupmeisai = atoi(getVal.Trim());
					break;
				case 4:
					Rec.cre = getVal.Trim();
					break;
				case 5:
					Rec.cregroup = atoi(getVal.Trim());
					break;
				case 6:
					Rec.cregroupmeisai = atoi(getVal.Trim());
					break;
				case 7:
					Rec.val = getVal.Trim();
					break;
				case 8:
					Rec.zei = getVal.Trim();
					break;
				case 9:
					Rec.taika = getVal.Trim();
					break;
				case 10:
					Rec.zeikubun = atoi(getVal.Trim());
					break;
				case 11:
					Rec.zeiritsu = atoi(getVal.Trim());
					break;
				case 12:
					Rec.syutokujoto = atoi(getVal.Trim());
					break;
				case 13:
					Rec.jigyokubun = 0;
					break;
				case 14:
					Rec.siwakekubun = atoi(getVal.Trim());
					break;
				default:
					break;
				}
			}

			if (sddate && eddate) {
				if ((Rec.ddate < sddate) || (Rec.ddate > eddate)) {
					continue;
				}
			}

#if _DEBUG
			//DbgViewTrace(_T("【DEBUG】%d %s [%02d] Rec.ddate:%d Rec.jigyokubun:%d dbt:%s dbtgroup:0x%02x dbtgroupmeisai:0x%02x cre:%s cregroup:0x%02x cregroupmeisai:0x%02x val:%s zei:%s\n"),
			//	__LINE__, __FUNCTION__,
			//	0,
			//	Rec.ddate,
			//	Rec.jigyokubun,
			//	Rec.dbt,
			//	Rec.dbtgroup,
			//	Rec.dbtgroupmeisai,
			//	Rec.cre,
			//	Rec.cregroup,
			//	Rec.cregroupmeisai,
			//	Rec.val,
			//	Rec.zei
			//);
#endif
			pSiwakeRecList.push_back(Rec);
		}

	}
	catch (CDBException* dbe) {
		dbe->Delete();
		return;
	}

	mstDbrec.Close();

}

//5億円判定対象データを集計し、5億円を超えているかどうか判定する.
bool CDBSyzAccess::Syukei5okuHantei(CDBZmSub* pZmsub, std::vector<SiwakeRec>& pSiwakeRecList, char* syukei_money, char* KaribaraiSyohizei, char* KariukeSyohizei)
{
	bool bOver = false;
    bool bWariModoshi = false;

	char money_val[6] = { 0 };
	char money_zei[6] = { 0 };
	char money_sum[6] = { 0 };

	memset(&money_val[0], 0, sizeof(money_val));
	memset(&money_zei[0], 0, sizeof(money_zei));
	memset(&money_sum[0], 0, sizeof(money_sum));

	std::vector<SiwakeRec> kariuke_10List;
	std::vector<SiwakeRec> kariuke_8keiList;
	std::vector<SiwakeRec> kariuke_8List;

	CArithEx ArithEx;
	ArithEx.l_defn(0x16);

    int to_kz_ymd;
    if( pZmsub->zvol->to_kz_ymd != 0 ) {
		to_kz_ymd = pZmsub->zvol->to_kz_ymd;
	}
    else {
		to_kz_ymd = INVOICE_SYZ_YMD;
	}

#if _DEBUG
	//char strmoney_sum[64] = { 0 };
	//char strmoney_val[64] = { 0 };
	//char strmoney_zei[64] = { 0 };
	//char strmoney_kariuke[64] = { 0 };
#endif

	//m_SiwakeRecListの5億円判定対象データから金額を集計.
	for (std::vector<SiwakeRec>::iterator itr = pSiwakeRecList.begin(); itr != pSiwakeRecList.end(); itr++) {

#if _DEBUG
		   //  DbgViewTrace(_T("【DEBUG】%d %s() dbt:%s dbtgroup:0x%02x dbtgroupmeisai:0x%02x cre:%s cregroup:0x%02x cregroupmeisai:0x%02x val:%s zei:%s\n"), 
				 //__LINE__, __FUNCTION__, itr->dbt, itr->dbtgroup, itr->dbtgroupmeisai, itr->cre, itr->cregroup, itr->cregroupmeisai, itr->val, itr->zei);
#endif

		if (itr->syutokujoto == SISAN_JOTO) {
			ArithEx.l_input(&money_val[0], itr->taika.GetBuffer());
		}
		else {
			ArithEx.l_input(&money_val[0], itr->val.GetBuffer());
		}
		ArithEx.l_input(&money_zei[0], itr->zei.GetBuffer());

		//貸方科目
		if (Is5okuHanteiData(itr, ID_KASHIKATA, KaribaraiSyohizei, KariukeSyohizei)) {
#if _DEBUG
			//DbgViewTrace(_T("【DEBUG】%d %s() money_sum:%s - money_val:%s = "),
			//	__LINE__, __FUNCTION__,
			//	ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
			//	ArithEx.l_print(strmoney_val, &money_val[0], _T("s,sss,sss,sss,ss9"))
			//);
#endif
			//if (itr->dbt.Compare(KariukeSyohizei) == 0){
			if (memcmp(itr->dbt, KariukeSyohizei, 6) == 0){	// 明細拡張も含める
                //仮受消費税
				if(itr->ddate < to_kz_ymd) {
					//課税開始日以前なら引く.
					ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_val[0]);
				}
                else {
					//課税開始日以降は考慮しない.
                    ;
				}
			}
			else {
				ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_val[0]);
			}
#if _DEBUG
			//DbgViewTrace(_T("money_sum:%s \n"), ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif

			//内税 課税開始日以降は税抜き.
			if ((itr->zeikubun == ZEIKUBUN_UTIZEI) && (itr->ddate >= to_kz_ymd)) { // TISW修正[24-0381] 2026/02/16
#if _DEBUG
				//DbgViewTrace(_T("【DEBUG】%d %s() [内税] money_sum:%s + money_zei:%s = "),
				//	__LINE__, __FUNCTION__,
				//	ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				ArithEx.l_add(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}

			//外税 課税開始日より前は税込み
			if ((itr->zeikubun == ZEIKUBUN_SOTOZEI) && (itr->ddate < to_kz_ymd)) { // TISW修正[24-0381] 2026/02/16
#if _DEBUG
				//DbgViewTrace(_T("【DEBUG】%d %s() [外税] money_sum:%s - money_zei:%s = "),
				//	__LINE__, __FUNCTION__,
				//	ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}
		}

		//借方科目
		if (Is5okuHanteiData(itr, ID_KARIKATA, KaribaraiSyohizei, KariukeSyohizei)) {
#if _DEBUG
			//DbgViewTrace(_T("【DEBUG】%d %s() money_sum:%s + money_val:%s = "),
			//	__LINE__, __FUNCTION__,
			//	ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
			//	ArithEx.l_print(strmoney_val, &money_val[0], _T("s,sss,sss,sss,ss9"))
			//);
#endif
			//if (itr->cre.Compare(KariukeSyohizei) == 0){
			if (memcmp(itr->cre, KariukeSyohizei, 6) == 0){	// 明細拡張も含める
                //仮受消費税
				if (itr->ddate < to_kz_ymd) {
					//課税開始日以前は税込み集計.
					ArithEx.l_add(&money_sum[0], &money_sum[0], &money_val[0]);
				}
				else {
					//課税開始日以降は考慮しない.
                    ;
				}
			}
			else {
				ArithEx.l_add(&money_sum[0], &money_sum[0], &money_val[0]);
			}
#if _DEBUG
			//DbgViewTrace(_T("money_sum:%s \n"), ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif

			//外税 課税開始日より前は税込み
			if ((itr->zeikubun == ZEIKUBUN_SOTOZEI) && (itr->ddate < to_kz_ymd)) { // TISW修正[24-0381] 2026/02/16
#if _DEBUG
				//DbgViewTrace(_T("【DEBUG】%d %s() [外税] money_sum:%s + money_zei:%s = "),
				//	__LINE__, __FUNCTION__,
				//	ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				ArithEx.l_add(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}
		}
	}

	//5億円を超えているかどうか
	char money_5oku[6] = { 0 };
	ArithEx.l_input(&money_5oku[0], _T("500000000"));
	if (ArithEx.l_cmp(&money_sum[0], &money_5oku[0]) > 0) {
		bOver = true;
	}

	memcpy(&syukei_money[0], &money_sum[0], sizeof(money_sum));

	return bOver;
}

bool CDBSyzAccess::Is5okuHanteiData(std::vector<SiwakeRec>::iterator itr, EnumIdKariKashiKbn type, char* KaribaraiSyohizei, char* KariukeSyohizei)
{
	bool bSyukei = false;
	int group, groupmeisai;
	CString kncod;

	if (type == ID_KASHIKATA) {
		group = itr->dbtgroup;
		groupmeisai = itr->dbtgroupmeisai;
		kncod = itr->dbt;
	}
	else {
		group = itr->cregroup;
		groupmeisai = itr->cregroupmeisai;
		kncod = itr->cre;
	}

	//仮受消費税
	//if (kncod.Compare(KariukeSyohizei) == 0) {
	if (memcmp(kncod, KariukeSyohizei, 6) == 0){	// 明細拡張も含める
		//消費税仕訳区分あり
		switch (itr->siwakekubun) {
		case ID_SKA_KASHIDAORESON:		// 貸　倒　損  失
		case ID_SKA_KASHIDAOREKAISYU:	// 貸　倒　回　収
		case ID_SKA_SYOUHIZEISEISAN:	// 消 費 税 清 算
			break;
		default:
			bSyukei = true;
			return bSyukei;
		}
	}

	if (itr->siwakekubun) {
		//消費税仕訳区分あり.
		switch (itr->siwakekubun) {
		case ID_SKA_URIAGE:               //売上
		case ID_SKA_URIAGENEBIKIMODORI:   //売上値引・戻り
		case ID_SKA_URIAGEWARIBIKI:       //売上割引
		case ID_SKA_YUSYUTSUMENZEIURIAGE: //輸出　免税売上 
		case ID_SKA_HANBAISYOUREIKINHIYO: //販売奨励金費用
			if (group) {
				bSyukei = true;
			}
			break;
		}
	}
	else {
		//仕訳区分なし.
		switch (group) {
		case KAMOKUGROUP_SISAN: //資産
			switch (groupmeisai) {
			case KAMOKUMEISAI_SISAN_JOTO: //資産譲渡
				bSyukei = true;
				break;
			}
			break;
		case KAMOKUGROUP_TAIKASISAN: //対価&資産
			switch (groupmeisai) {
			case KAMOKUMEISAI_TAIKASISAN_SYUTOKUJOTO: //譲渡
				if (itr->syutokujoto == SISAN_JOTO) {
					bSyukei = true;
				}
				break;
			}
			break;
		case KAMOKUGROUP_URIAGE: //売上
			switch (groupmeisai) {
			case KAMOKUMEISAI_URIAGE_URIAGE:              //売上
			case KAMOKUMEISAI_URIAGE_URIAGEHENPIN:        //売上返品
			case KAMOKUMEISAI_URIAGE_EIGYOGAISYUEKI:      //営業外収益 
			case KAMOKUMEISAI_URIAGE_TOKUBETSURIEKI:      //特別利益
			case KAMOKUMEISAI_URIAGE_UKETORIRISOKU:       //受取利息
			case KAMOKUMEISAI_URIAGE_URIAGEWARIBIKI:      //売上割引
			case KAMOKUMEISAI_URIAGE_YUSYUTUMENZEIURIAGE: //輸出免税売上
			case KAMOKUMEISAI_URIAGE_HANBAISYOREIKINHIYO: //販売奨励金費用
				bSyukei = true;
				break;
			}
			break;
		}
	}

	return bSyukei;

}

//指定日付の税額計算方法を取得する.
//ddate:指定日付 CalqKbn: 0：売上 1:仕入
//戻り値: 割戻し:0 積上げ:1
int CDBSyzAccess::GetZeiCalqByDate(CDBZmSub* pZmsub, long ddate, int CalqKbn)
{
    int Rtkbn = 0; //割戻し:0 積上げ:1

	BOOL bZvolOpen = FALSE;

	if (pZmsub->zvol == NULL) {
		if (pZmsub->VolumeOpen() < 0) {
			return -1;
		}
		bZvolOpen = TRUE;
	}

	if (pZmsub->SyzcalqmthOpen() < 0) {
		return -1;
	}

	int zeicalqmth_smd = 0, zeicalqmth_emd = 0;

	for (int j = 0; j < pZmsub->syzcalqmth->GetRecordCount(); j++) {

		if (j == 0) {
			pZmsub->syzcalqmth->MoveFirst();
		}
		else {
			pZmsub->syzcalqmth->MoveNext();
		}

		//検索区分と違えば飛ばす.
		if (CalqKbn != pZmsub->syzcalqmth->calqkbn) {
            continue;
		}

		//期首日、期末日は0になっているのでzvolumeから取得.
		if (pZmsub->syzcalqmth->smd == 0) {
			zeicalqmth_smd = pZmsub->zvol->ss_ymd;
		}
		else {
			zeicalqmth_smd = pZmsub->syzcalqmth->smd;
		}
		if (pZmsub->syzcalqmth->emd == 0) {
			zeicalqmth_emd = pZmsub->zvol->ee_ymd;
		}
		else {
			zeicalqmth_emd = pZmsub->syzcalqmth->emd;
		}

        if( (zeicalqmth_smd <= ddate) && (ddate <= zeicalqmth_smd) ){
			Rtkbn = pZmsub->syzcalqmth->zeicalq;
            break;
		}

	}

	if (bZvolOpen) {
		pZmsub->VolumeClose();
	}
	pZmsub->SyzcalqmthClose();

    return Rtkbn;
}

//--> 2024.07.16 h.wada add
//-----------------------------------------------------------------------------
// 免税期間の課税売上高の集計額をSYZ_SNK_LST_VALから読み出す
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	財務クラス
// 	    dest	：	読み出し金額の返送先
//-----------------------------------------------------------------------------
// 返送値	 0：	読み出し成功、または読み出し不要
//			-1：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::GetMenzeiUriage(CDBZmSub* pZmsub, char* dest)
{
	CArithEx ArithEx;
	ArithEx.l_defn(0x16);

	//読み出し先パラメータ
	const int seq = 0;
	const int version = 2;
	const CString hyoId = _T("FFFFFF");
	const CString itmId = _T("FFF00000");
	CString filter = _T("");
	filter.Format(_T("sn_seq = %d and version = %d and hyoId = \'%s\' and itmId = \'%s\'"), 0, version, hyoId, itmId);

	char money[6] = { 0 };
	memset(money, 0, sizeof(money));
	bool openLstVal = false;

	try {
		//テーブルオープン
		if (m_pSH_LST_VAL == NULL) {
			if (OpenShLstVal(pZmsub->m_database) == -1) { return -1; }
			openLstVal = true;
		}

		//金額読み出し
		if (m_pSH_LST_VAL->Requery(filter) != 0) {
			if (openLstVal) { CloseShLstVal(); }
			return -1;
		}

		if (m_pSH_LST_VAL->GetRecordCount() != 0) {
			char str[64] = { 0 };
			strcpy_s(str, m_pSH_LST_VAL->val);
			ArithEx.l_input(money, str);
		}
	}
	catch (CDBException* dbe) {
		if (openLstVal) { CloseShLstVal(); }
		dbe->Delete();
		return -1;
	}

	if (openLstVal) { CloseShLstVal(); }
	memcpy(&dest[0], &money[0], sizeof(money));

	return 0;
}
//<-- 2024.07.16 h.wada add

//////////////
// 共通関数 //
//////////////

//-----------------------------------------------------------------------------
// テーブルの存在確認
//-----------------------------------------------------------------------------
// 引数	pTbname	：	チェック対象テーブル名
//		pdb		：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	存在する
//			0	：	存在しない
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::IsExist( const char *pTbname, CDatabase *pdb )
{
	ASSERT( pTbname );
	ASSERT( pdb );

	int rt = 0;

	CString Sql, msg;
	Sql.Format( ISEXIST_SQL, pTbname );
	CString	tblCnt;

	CRecordset	tmpDbrec( pdb );
	try{
		tmpDbrec.Open( CRecordset::forwardOnly, Sql, CRecordset::readOnly );
		if ( !tmpDbrec.IsEOF() ) {
			tmpDbrec.GetFieldValue( (SHORT)0, tblCnt );
		}
		else {
			tmpDbrec.Close();
			return -1;
		}
	}
	catch( CDBException* dbe ){
		dbe->Delete();
		return -1;
	}
	tmpDbrec.Close();

	int imCnt = atoi( tblCnt.GetBuffer() );
	if( imCnt > 0 ){	// テーブル存在
		rt = 1;
	}

	return rt;
}

//=============================================================================
// カラム存在チェック
//-----------------------------------------------------------------------------
// 引数
//		pTblName			:			テーブル名
//		pClmName			:			カラム名
//		pdb					:			チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値
//		0					:			存在しない
//		1					:			存在する
//		-1					:			エラー
//=============================================================================
int CDBSyzAccess::IsExistColumn( const char *pTblName, const char *pClmName, CDatabase *pdb )
{
	// 指定されたテーブルに、指定されたカラムが存在するかチェック
	ASSERT( pTblName );
	ASSERT( pClmName );
	ASSERT( pdb );

	if(( pTblName == NULL ) || ( pClmName == NULL ) || ( pdb == NULL )) {
		return -1;
	}

	int ret = 0, clmCount = 0;
	CString sql, strCount;
	CRecordset rs( pdb );

	sql.Format( ISEXISTCOLUMN, pClmName, pTblName );
	try {
		rs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !rs.IsEOF() ) {
			rs.GetFieldValue( ( short )0, strCount );
		}
		rs.Close();
	}
	catch( CDBException *dbe ) {
		rs.Close();
		dbe->Delete();
		return -1;
	}

	clmCount = atoi( strCount.GetBuffer() );
	if( clmCount > 0 ) {
		ret = 1;
	}

	return ret;
}

//-----------------------------------------------------------------------------
// テーブル作成
//-----------------------------------------------------------------------------
// 引数	pdb		：	作成先データベース
//		ver		：	テーブルバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzAccess::MakePrfetaxTbl( const char *pSqname, const char *pTbname, CDatabase *pdb, long ver )
{
	ASSERT( pdb );

	CString	Sql;
	
	pdb->BeginTrans();

	// テーブル作成
	Sql.Format( pSqname, pTbname );
	try{
		pdb->ExecuteSQL( Sql );
	}
	catch( CDBException* dbe ){
//--> '14.01.06 INS START
		m_ErrMsg = dbe->m_strError;
//<-- '14.01.06 INS END
		pdb->Rollback();
		dbe->Delete();
		return -1;
	}

	// バージョン付加
	Sql.Format( SET_VERSION_SQL, ver, pTbname );
	try{
		pdb->ExecuteSQL( Sql );
	}
	catch( CDBException* dbe ){
//--> '14.01.06 INS START
		m_ErrMsg = dbe->m_strError;
//<-- '14.01.06 INS END
		pdb->Rollback();
		dbe->Delete();
		return -1;
	}

	pdb->CommitTrans();

	return 0;
}

//=============================================================================
// テーブルバージョンセット
//-----------------------------------------------------------------------------
// 引数
//		pdb					:			チェック対象データベース
//		pTblName			:			テーブル名
//		ver					:			テーブルバージョン
//-----------------------------------------------------------------------------
// 返送値
//		0					:			正常終了
//		-1					:			エラー
//=============================================================================
int CDBSyzAccess::SetTableVersion( CDatabase *pdb, const char *pTblName, long ver )
{
	// 指定されたテーブルにバージョンをセット
	ASSERT( pdb );
	ASSERT( pTblName );
	if(( pdb == NULL ) || ( pTblName == NULL )) {
		return -1;
	}

	if( ver < 0 ) {
		return -1;
	}

	CString sql;

	pdb->BeginTrans();

	// バージョン付加
	sql.Format( SET_VERSION_SQL, ver, pTblName );
	try{
		pdb->ExecuteSQL( sql );
	}
	catch( CDBException* dbe ){
		m_ErrMsg = dbe->m_strError;
		pdb->Rollback();
		dbe->Delete();
		return -1;
	}
	pdb->CommitTrans();

	return 0;
}