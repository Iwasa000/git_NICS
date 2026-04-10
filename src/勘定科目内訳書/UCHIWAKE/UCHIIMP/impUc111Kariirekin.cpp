#include "StdAfx.h"
#include "impUc111Kariirekin.h"

// midori 190301 0415 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\prtConstData.h"

extern	BOOL	bG_Kanso;
// midori 190301 0415 add <--
// 24/02/20_ScoreLink add -->
extern BOOL	bG_Sl;
// 24/02/20_ScoreLink add <--

// midori 156188 add -->
//extern	int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw);				// 改良No.21-0086,21-0529 del
extern	int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);	// 改良No.21-0086,21-0529 add
// midori 156188 add <--

// コンストラクタ
CimpUc111Kariirekin::CimpUc111Kariirekin(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
// midori 157046 add -->
	m_KmkRowSgn = 0;
// midori 157046 add <--
}

// デストラクタ
CimpUc111Kariirekin::~CimpUc111Kariirekin(void)
{
}


//**********************************************************************
// 初期処理
// 
//	引数
//			pDB						[IN] DBポインタ
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
//virtual
int CimpUc111Kariirekin::Init(CDatabase* pDB)
{
	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 共通で使用するテーブルとの接続を開始
	ConnectCmnTables(pDB);


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳票固有のコード
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 様式シーケンスを指定（ID_FORMNO_XXX）
	m_nFormSeq = ID_FORMNO_111;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_111_ITEM_CNT;

// 210113_5301_1 add -->
// 24/02/20_ScoreLink cor -->
	//if(m_IsExport == TRUE && bG_Kanso == TRUE)	{
// --------------------------
	if(m_IsExport == TRUE && bG_Kanso == TRUE && bG_Sl == FALSE)	{
// 24/02/20_ScoreLink cor <--
		// エクスポート時、新様式なら項目数を減らしておく
		// (インポートは新旧両方のレイアウトに対応するため、ここでは決定できない)
		m_nItemCount = m_nItemCount - 2;
	}
// 210113_5301_1 add <--

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc111Kariirekin(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );

// midori 156189,156190,156191 add -->
	m_pDB = pDB;
	// 科目名称のクリア
	m_strKmName.Empty();
	// 科目行サインのクリア
	m_KmkRowSw = 0;
// midori 156189,156190,156191 add <--
// midori 157046 add -->
	//m_KmkRowSgn = ImpKamokuRowEnableSgn(m_pDB,0);				// 改良No.21-0086,21-0529 del
	m_KmkRowSgn = ImpKamokuRowEnableSgn(m_pDB, 0, m_nFormSeq);	// 改良No.21-0086,21-0529 add
// midori 157046 add <--

	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_111;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_111 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_111;


	return IMP_ERRCODE_SUCCESS;
}

//**********************************************************************
// midori 180404 del -->
//// CSVの1行分のデータをテーブルに登録（インポート用）
////		・データの正当性をチェック
////		・データをテーブルに格納する形式に変換
////		・テーブルに登録
// midori 180404 del <	--
// midori 180404 add -->
// CSV1行分データの正当性をチェック
// midori 180404 add <--
// 
//	引数
//			pstrCsvDataAry			[IN] CSVの1行分のデータ
// 
//	戻値
//			レコード（行）の種類（DBUchiImpDEF.h参照）
//**********************************************************************
//virtual
int CimpUc111Kariirekin::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
// midori 180404 del -->
//	BYTE			cFgFunc			= 0;			// 特殊行フラグ
// midori 180404 del <--
// midori 156189,156190,156191 add -->
	int				kt=0;
// midori 156189,156190,156191 add <--
// No.200903 add -->
	int				len=0;
// No.200903 add <--

// midori 157046 del -->
//// midori 156936 del -->
////// midori 180404 del -->
//////	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &cFgFunc);
////// midori 180404 del <--
////// midori 180404 add -->
////	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc);
////// midori 180404 add <--
//// midori 156936 del <--
//// midori 156936 add -->
//	int kmkuRowSgn = ImpKamokuRowEnableSgn(m_pDB,0);
//
//	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc, kmkuRowSgn);
//// midori 156936 add <--
// midori 157046 del <--
// midori 157046 add -->
	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc, m_KmkRowSgn);
// midori 157046 add <--
	// エラーなら終了
	if (nRet != ID_FGFUNC_NULL) {
		return nRet;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc111Kariirekin*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

// midori 156936 add -->
	CsvInfoClr(&m_uImpCsvInfo);
// midori 156936 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 科目
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_111_KAMOKU*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_01);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_111_KAMOKU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

// midori 156936 del -->
//// midori 156189,156190,156191 del -->
////	// 02 借入先１
////	if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_111_ADNAME1) != TRUE) {
////		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_02);
////// midori 150899 cor -->
////		//return IMP_REC_NG;
////// ---------------------
////		m_uImpCsvInfo.nLength = MAX_KETA_111_ADNAME1/2;
////		m_uImpCsvInfo.nKind = 0;	// 0:文字
////		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
////		CsvInfoClr(&m_uImpCsvInfo);
////		nRet = IMP_REC_NG;
////// midori 150899 cor <--
////	}
////
////	// 03 借入先２
////	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_111_ADNAME1) != TRUE) {
////		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_03);
////// midori 150899 cor -->
////		//return IMP_REC_NG;
////// ---------------------
////		m_uImpCsvInfo.nLength = MAX_KETA_111_ADNAME1/2;
////		m_uImpCsvInfo.nKind = 0;	// 0:文字
////		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
////		CsvInfoClr(&m_uImpCsvInfo);
////		nRet = IMP_REC_NG;
////// midori 150899 cor <--
////	}
//// midori 156189,156190,156191 del <--
//// midori 156189,156190,156191 add -->
//	if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0) {
//		kt = MAX_KETA_111_SYOKEI;
//	}
//	else {
//		kt = MAX_KETA_111_ADNAME1;
//	}
//	if((pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_KAMOKU) == 0) && ImpKamokuRowEnableSgn(m_pDB,1) == 1) {
//		// 02 借入先１
//		if (CheckImportStr(pstrCsvDataAry->GetAt(2), kt) != TRUE) {
//			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_02);
//			m_uImpCsvInfo.nLength = kt / 2;
//			m_uImpCsvInfo.nKind = 0;	// 0:文字
//			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//			CsvInfoClr(&m_uImpCsvInfo);
//			nRet = IMP_REC_NG;
//		}
//
//		// 03 借入先２
//		if (CheckImportStr(pstrCsvDataAry->GetAt(3), kt) != TRUE) {
//			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_03);
//			m_uImpCsvInfo.nLength = kt / 2;
//			m_uImpCsvInfo.nKind = 0;	// 0:文字
//			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//			CsvInfoClr(&m_uImpCsvInfo);
//			nRet = IMP_REC_NG;
//		}
//	}
//// midori 156189,156190,156191 add <--
// midori 156936 del <--
// midori 156936 add -->
// midori 157046 del -->
	//if(kmkuRowSgn == 1) {
// midori 157046 del <--
// midori 157046 add -->
	if(m_KmkRowSgn == 1) {
// midori 157046 add <--
		// 科目行
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_KAMOKU) == 0) {
			// 借入先１、借入先２
			if ( pstrCsvDataAry->GetAt(2) == _T("") && pstrCsvDataAry->GetAt(3) == _T("")){
				m_uImpCsvInfo.nErr = 4;
				return(IMP_REC_NG);
			}
		}
	}

// midori 157046 del -->
	//if(kmkuRowSgn == 0) {
// midori 157046 del <--
// midori 157046 add -->
	if(m_KmkRowSgn == 0) {
// midori 157046 add <--
		kt = MAX_KETA_111_ADNAME1;
	}
	else {
// midori 157046 del -->
		//if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0) {
// midori 157046 del <--
// midori 157046 add -->
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_CHUKEI) == 0) {
// midori 157046 add <--
			kt = MAX_KETA_111_SYOKEI;
		}
		else {
			kt = MAX_KETA_111_ADNAME1;
		}
	}

	// 02 借入先１
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), kt) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_02);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_02_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_02);
// 210113_5301_1 add <--
		m_uImpCsvInfo.nLength = kt / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 借入先２
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), kt) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_03);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_03_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_03);
// 210113_5301_1 add <--
		m_uImpCsvInfo.nLength = kt / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// midori 156936 add <--

	// 04 所在地（住所）１
	if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_111_ADADD1*2) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_04);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_04_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_04);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_111_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 05 所在地（住所）２
	if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_111_ADADD2*2) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_05);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_05_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_05);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_111_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 06 法人・代表者との関係
	if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_111_KANKEI*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_06);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_111_KANKEI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

// No.200903 del -->
//	// 07 期末現在高
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL_111_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL_111_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), ID_VAL_111_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_07);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL_111_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(7).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
//
//	// 08 期中の支払利子額
////	if (CheckImportNum(pstrCsvDataAry->GetAt(8), ID_RISOKU_111_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(8), ID_RISOKU_111_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(8), ID_RISOKU_111_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_08);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_RISOKU_111_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(8).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
// No.200903 del <--
// No.200903 add -->
	// 07 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_111_K_DATA;
	else					len = ID_VAL_111_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_07);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(7).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 08 期中の支払利子額
	if(bG_Kanso == TRUE)	len = ID_RISOKU_111_K_DATA;
	else					len = ID_RISOKU_111_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(8), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_08);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(8).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

	// 09 利率
//	if (CheckImportNum(pstrCsvDataAry->GetAt(9), 3, 3) != TRUE) {
//	if (CheckImportNum(pstrCsvDataAry->GetAt(9), 3, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
	if (CheckImportNum2(pstrCsvDataAry->GetAt(9), 3, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_09);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength	= 3;	// 整数最大桁数
		m_uImpCsvInfo.nLength2	= 3;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

// 210113_5301_1 del -->
//// midori 190301 0415 del -->
////	// 10 借入理由１
////	if (CheckImportStr(pstrCsvDataAry->GetAt(10), MAX_KETA_111_RIYUU) != TRUE) {
////		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_10);
////// midori 150899 cor -->
////		//return IMP_REC_NG;
////// ---------------------
////		m_uImpCsvInfo.nLength = MAX_KETA_111_RIYUU/2;
////		m_uImpCsvInfo.nKind = 0;	// 0:文字
////		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
////		CsvInfoClr(&m_uImpCsvInfo);
////		nRet = IMP_REC_NG;
////// midori 150899 cor <--
////	}
////
////	// 11 借入理由２
////	if (CheckImportStr(pstrCsvDataAry->GetAt(11), MAX_KETA_111_RIYUU) != TRUE) {
////		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_11);
////// midori 150899 cor -->
////		//return IMP_REC_NG;
////// ---------------------
////		m_uImpCsvInfo.nLength = MAX_KETA_111_RIYUU/2;
////		m_uImpCsvInfo.nKind = 0;	// 0:文字
////		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
////		CsvInfoClr(&m_uImpCsvInfo);
////		nRet = IMP_REC_NG;
////// midori 150899 cor <--
////	}
//// midori 190301 0415 del <--
//// midori 190301 0415 del -->
//	if(bG_Kanso != TRUE) {
//		// 10 借入理由１
//		if(CheckImportStr(pstrCsvDataAry->GetAt(10),MAX_KETA_111_RIYUU) != TRUE) {
//			strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_111_ITEM_10);
//			m_uImpCsvInfo.nLength = MAX_KETA_111_RIYUU/2;
//			m_uImpCsvInfo.nKind = 0;	// 0:文字
//			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//			CsvInfoClr(&m_uImpCsvInfo);
//			nRet = IMP_REC_NG;
//		}
//
//		// 11 借入理由２
//		if(CheckImportStr(pstrCsvDataAry->GetAt(11),MAX_KETA_111_RIYUU) != TRUE) {
//			strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_111_ITEM_11);
//			m_uImpCsvInfo.nLength = MAX_KETA_111_RIYUU/2;
//			m_uImpCsvInfo.nKind = 0;	// 0:文字
//			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//			CsvInfoClr(&m_uImpCsvInfo);
//			nRet = IMP_REC_NG;
//		}
//	}
//// midori 190301 0415 add <--
//
//	// 12 担保の内容（物件の種類、数量、所在地等）１
//	if (CheckImportStr(pstrCsvDataAry->GetAt(12), MAX_KETA_111_NAIYOU) != TRUE) {
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_12);
//// midori 150899 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = MAX_KETA_111_NAIYOU/2;
//		m_uImpCsvInfo.nKind = 0;	// 0:文字
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150899 cor <--
//	}
//
//	// 13 担保の内容（物件の種類、数量、所在地等）２
//	if (CheckImportStr(pstrCsvDataAry->GetAt(13), MAX_KETA_111_NAIYOU) != TRUE) {
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_13);
//// midori 150899 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = MAX_KETA_111_NAIYOU/2;
//		m_uImpCsvInfo.nKind = 0;	// 0:文字
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150899 cor <--
//	}
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	int	ofset=10;
	if(m_NewVer != TRUE) {
		// 10 借入理由１
		if(bG_Kanso != TRUE)	{
			if(CheckImportStr(pstrCsvDataAry->GetAt(ofset),MAX_KETA_111_RIYUU) != TRUE) {
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_111_ITEM_10);
				m_uImpCsvInfo.nLength = MAX_KETA_111_RIYUU/2;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
			}
		}
		ofset++;
		// 11 借入理由２
		if(bG_Kanso != TRUE)	{
			if(CheckImportStr(pstrCsvDataAry->GetAt(ofset),MAX_KETA_111_RIYUU) != TRUE) {
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_111_ITEM_11);
				m_uImpCsvInfo.nLength = MAX_KETA_111_RIYUU/2;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
			}
		}
		ofset++;
	}

	// 12 担保の内容（物件の種類、数量、所在地等）１
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_111_NAIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_12);
		m_uImpCsvInfo.nLength = MAX_KETA_111_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
	ofset++;
	// 13 担保の内容（物件の種類、数量、所在地等）２
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_111_NAIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_111_ITEM_13);
		m_uImpCsvInfo.nLength = MAX_KETA_111_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// 210113_5301_1 add <--

// midori 150895 add -->
	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}
// midori 150895 add <--

// midori 180404 del -->
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 名称テーブルへ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	long			nKnSeq			= 0;				// 科目シーケンス番号
//	long			nKnOrder		= 0;				// 科目並び順
//	long			nAdSeq			= 0;				// 金融機関シーケンス番号
//	long			nAdOrder		= 0;				// 金融機関並び順
//
//	// 01 科目
//	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	// 02 借入先１
//	// 03 借入先２
//	if (CheckEntryBank(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3), &nAdSeq, &nAdOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// データ加工
//	// テーブルへデータ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	CString			strTemp			= _T("");		// 文字列テンポラリ
//
//	// 新規レコード作成
//	m_pUcData->CreateNewRecord();
//
//	// レコード編集開始
//	m_pUcData->Edit();
//
//	// XX 頁番号
//	// XX 行番号
//	CheckNumPageRow(&m_NumPage, &m_NumRow);
//	m_pUcData->m_NumPage		= m_NumPage;
//	m_pUcData->m_NumRow			= m_NumRow;
//
//	// 00 特殊行フラグ
//	m_pUcData->m_FgFunc			= cFgFunc;
//
//	// [明細]の場合のみ各項目のデータを登録
//	// [小計]、[中計]は、計文言と頁番号、行番号、特殊行フラグのみ
//	// 空行は、頁番号、行番号、特殊行フラグのみ
//	switch (cFgFunc) {
//	case	ID_FGFUNC_DATA:			// データ行
//		// 01 科目
//		m_pUcData->m_KnSeq		= nKnSeq;
//		m_pUcData->m_KnOrder	= nKnOrder;
//		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
//// midori 152137 add -->
//		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//// midori 152137 add <--
//
//		// 02 借入先１
//		// 03 借入先２
//		m_pUcData->m_AdSeq		= nAdSeq;
//		m_pUcData->m_AdOrder	= nAdOrder;
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(2);
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(3);
//// midori 152137 add -->
//		m_pUcData->m_AdKana1	= fc.Convert(pstrCsvDataAry->GetAt(2));
//		m_pUcData->m_AdKana2	= fc.Convert(pstrCsvDataAry->GetAt(3));
//// midori 152137 add <--
//
//		// 04 所在地（住所）１
//		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(4);
//
//		// 05 所在地（住所）２
//		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(5);
//
//		// 06 法人・代表者との関係
//		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(6);
//
//		// 07 期末現在高
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 08 期中の支払利子額
//		m_pUcData->m_Risoku		= pstrCsvDataAry->GetAt(8);
//		RemoveCommaImportNum( m_pUcData->m_Risoku );			// カンマを除く　2011.07.15 h.ukawa
//
//		// 09 利率
//		strTemp = _T("");
//		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(9), 1000, 0);	// ×1000
//		m_pUcData->m_Rate		= strTemp;
//
//		// 10 借入理由１
//		// 11 借入理由２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(10), pstrCsvDataAry->GetAt(11));
//		m_pUcData->m_Riyuu		= strTemp;
//
//		// 12 担保の内容（物件の種類、数量、所在地等）１
//		// 13 担保の内容（物件の種類、数量、所在地等）２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(12), pstrCsvDataAry->GetAt(13));
//		m_pUcData->m_Naiyou		= strTemp;
//		break;
//	case	ID_FGFUNC_SYOKEI:		// 小計行
//		m_pUcData->m_KeiStr		= m_strSyokeiName;
//		break;
//	case	ID_FGFUNC_CHUKEI:		// 中計行
//		m_pUcData->m_KeiStr		= m_strChukeiName;
//		break;
//	}
//
//	// レコード登録
//	m_pUcData->Update();
// midori 180404 del <--

	return IMP_REC_OK;
}

// midori 180404 add -->
//**********************************************************************
// CSVの1行分のデータをテーブルに登録（インポート用）
//		・データをテーブルに格納する形式に変換
//		・テーブルに登録
// 
//	引数
//			pstrCsvDataAry			[IN] CSVの1行分のデータ
// 
//	戻値
//			レコード（行）の種類（DBUchiImpDEF.h参照）
//**********************************************************************
//virtual
int CimpUc111Kariirekin::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// midori 156189,156190,156191 add -->
	int				kt=0;
// midori 156189,156190,156191 add <--
// No.200903 add -->
	int				len=0;
// No.200903 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori 190301 0415 add -->
	CString			cs=_T("");
	CString			cs1=_T("");
	CString			cs2=_T("");
	CString			cs3=_T("");
	CString			cs4=_T("");
// midori 190301 0415 add <--
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--
// midori 156189,156190,156191 add -->
	CString			strkei1= _T("");		// CSVに入力された小計名称
	CString			strkei2= _T("");		// 自動作成の小計名称
// midori 156189,156190,156191 add <--
// midori 190504 add -->
	CUcFunctionCommon	ufc;
// midori 190504 add <--

	// 文字数をオーバーしていればカットする
	// 01 科目
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_111_KAMOKU*2);
	pstrCsvDataAry->SetAt(1,strTemp);
// midori 156936 del -->
//// midori 156189,156190,156191 del -->
////	// 02 借入先１
////	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_111_ADNAME1);
////// midori 190504 add -->
////	strTemp = ufc.DeleteRightSpace(strTemp);
////// midori 190504 add <--
////	pstrCsvDataAry->SetAt(2,strTemp);
////	// 03 借入先２
////	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_111_ADNAME1);
////// midori 190504 add -->
////	strTemp = ufc.DeleteRightSpace(strTemp);
////// midori 190504 add <--
////	pstrCsvDataAry->SetAt(3,strTemp);
//// midori 156189,156190,156191 del <--
//// midori 156189,156190,156191 add -->
//	if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0) {
//		kt = MAX_KETA_111_SYOKEI;
//	}
//	else {
//		kt = MAX_KETA_111_ADNAME1;
//	}
//	if((pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_KAMOKU) == 0) && ImpKamokuRowEnableSgn(m_pDB,1) == 1) {
//		// 02 借入先１
//		GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,kt);
//		strTemp = ufc.DeleteRightSpace(strTemp);
//		pstrCsvDataAry->SetAt(2,strTemp);
//		// 03 借入先２
//		GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,kt);
//		strTemp = ufc.DeleteRightSpace(strTemp);
//		pstrCsvDataAry->SetAt(3,strTemp);
//	}
//// midori 156189,156190,156191 add <--
// midori 156936 del <--
// midori 156936 add -->
// midori 157046 del -->
	//if(ImpKamokuRowEnableSgn(m_pDB,0) == 0) {
// midori 157046 del <--
// midori 157046 add -->
	if(m_KmkRowSgn == 0) {
// midori 157046 add <--
		kt = MAX_KETA_111_ADNAME1;
	}
	else {
// midori 157046 del -->
		//if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0) {
// midori 157046 del <--
// midori 157046 add -->
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_CHUKEI) == 0) {
// midori 157046 add <--
			kt = MAX_KETA_111_SYOKEI;
		}
		else {
			kt = MAX_KETA_111_ADNAME1;
		}
	}

	// 02 借入先１
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,kt);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 借入先２
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,kt);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(3,strTemp);
// midori 156936 add <--
	// 04 所在地（住所）１
	GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_111_ADADD1*2);
// midori 190301 0415 add -->
	if(bG_Kanso == TRUE)	{
		// 全角１０文字を超えていれば改行マークを付けて取り込む
		if(strTemp.GetLength() > 20)	{
			cs2 = GetSpritString(strTemp,&cs1,20);
			cs = cs2;
			// 前半に半角文字が奇数バイト数分、入力されている場合
			// 後半に半角文字が１バイト余分に付いているのでカットする
			cs3 = GetSpritString(cs,&cs2,20);
			strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
		}
	}
// midori 190301 0415 add <--
	pstrCsvDataAry->SetAt(4,strTemp);
	// 05 所在地（住所）２
	GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_111_ADADD2*2);
// midori 190301 0415 add -->
	if(bG_Kanso == TRUE)	{
		// 全角１０文字を超えていれば改行マークを付けて取り込む
		if(strTemp.GetLength() > 20)	{
			cs2 = GetSpritString(strTemp,&cs1,20);
			cs = cs2;
			// 前半に半角文字が奇数バイト数分、入力されている場合
			// 後半に半角文字が１バイト余分に付いているのでカットする
			cs3 = GetSpritString(cs,&cs2,20);
			strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
		}
	}
// midori 190301 0415 add <--
	pstrCsvDataAry->SetAt(5,strTemp);
	// 06 法人・代表者との関係
	GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_111_KANKEI*2);
// midori 154640 del -->
//// midori 190301 0415 add -->
//	if(bG_Kanso == TRUE)	{
//		// 全角３文字を超えていれば改行マークを付けて取り込む
//		if(strTemp.GetLength() > 6)	{
//			cs2 = GetSpritString(strTemp,&cs1,6);
//			cs = cs2;
//			// 前半に半角文字が奇数バイト数分、入力されている場合
//			// 後半に半角文字が１バイト余分に付いているのでカットする
//			cs3 = GetSpritString(cs,&cs2,6);
//			strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
//		}
//	}
//// midori 190301 0415 add <--
// midori 154640 del <--
	pstrCsvDataAry->SetAt(6,strTemp);
// No.200903 del -->
	//// 07 期末現在高
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(7), ID_VAL_111_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(7,strTemp);
	//}
	//// 08 期中の支払利子額
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(8), ID_RISOKU_111_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(8,strTemp);
	//}
// No.200903 del <--
// No.200903 add -->
	// 07 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_111_K_DATA;
	else					len = ID_VAL_111_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(7,strTemp);
	}
	// 08 期中の支払利子額
	if(bG_Kanso == TRUE)	len = ID_RISOKU_111_K_DATA;
	else					len = ID_RISOKU_111_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(8), len, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(8,strTemp);
	}
// No.200903 add <--
	// 09 利率
	if (CheckImportNum2(pstrCsvDataAry->GetAt(9), 3, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(9,strTemp);
	}

// 210113_5301_1 del -->
//// midori 190301 0415 del -->
//	//// 10 借入理由１
//	//GetSpritString(pstrCsvDataAry->GetAt(10),&strTemp,MAX_KETA_111_RIYUU);
//	//pstrCsvDataAry->SetAt(10,strTemp);
//	//// 11 借入理由２
//	//GetSpritString(pstrCsvDataAry->GetAt(11),&strTemp,MAX_KETA_111_RIYUU);
//	//pstrCsvDataAry->SetAt(11,strTemp);
//	//// 12 担保の内容（物件の種類、数量、所在地等）１
//	//GetSpritString(pstrCsvDataAry->GetAt(12),&strTemp,MAX_KETA_111_NAIYOU);
//	//pstrCsvDataAry->SetAt(12,strTemp);
//	//// 13 担保の内容（物件の種類、数量、所在地等）２
//	//GetSpritString(pstrCsvDataAry->GetAt(13),&strTemp,MAX_KETA_111_NAIYOU);
//	//pstrCsvDataAry->SetAt(13,strTemp);
//// midori 190301 0415 del <--
//// midori 190301 0415 add -->
//	// 10 借入理由１
//	if(bG_Kanso == TRUE) {
//		strTemp = _T("");
//	}
//	else {
//		GetSpritString(pstrCsvDataAry->GetAt(10),&strTemp,MAX_KETA_111_RIYUU);
//	}
//	pstrCsvDataAry->SetAt(10,strTemp);
//	// 11 借入理由２
//	if(bG_Kanso == TRUE) {
//		strTemp = _T("");
//	}
//	else {
//		GetSpritString(pstrCsvDataAry->GetAt(11),&strTemp,MAX_KETA_111_RIYUU);
//	}
//	pstrCsvDataAry->SetAt(11,strTemp);
//
//	cs1 = pstrCsvDataAry->GetAt(12);
//	cs2 = pstrCsvDataAry->GetAt(13);
//	cs3 = _T("");
//	cs4 = _T("");
//	if(bG_Kanso == TRUE)	{
//		// ２行のうちいずれかが全角１０文字を超えている場合３行にする
//		if(cs1.GetLength() > 20 || cs2.GetLength() > 20)	{
//			strTemp = cs1 + cs2;
//			cs2 = GetSpritString(strTemp,&cs1,20);
//			strTemp = cs2;
//			cs3 = GetSpritString(strTemp,&cs2,20);
//			// １段目、２段目に半角文字が奇数バイト数分、入力されている場合
//			// ３段目に半角文字が１バイト余分に付いているのでカットする
//			strTemp = cs3;
//			cs4 = GetSpritString(strTemp,&cs3,12);
//		}
//		// 12 担保の内容（物件の種類、数量、所在地等）１
//		pstrCsvDataAry->SetAt(12,cs1);
//		// 13 担保の内容（物件の種類、数量、所在地等）２
//		pstrCsvDataAry->SetAt(13,cs2);
//		// 14 担保の内容（物件の種類、数量、所在地等）３
//		pstrCsvDataAry->InsertAt(14,cs3);
//	}
//	else	{
//		// 12 担保の内容（物件の種類、数量、所在地等）１
//		GetSpritString(cs1,&strTemp,MAX_KETA_111_NAIYOU);
//		pstrCsvDataAry->SetAt(12,strTemp);
//		// 13 担保の内容（物件の種類、数量、所在地等）２
//		GetSpritString(cs2,&strTemp,MAX_KETA_111_NAIYOU);
//		pstrCsvDataAry->SetAt(13,strTemp);
//	}
//// midori 190301 0415 add <--
//
//// midori UC_0054 add -->
//	strCsvId = pstrCsvDataAry->GetAt(0);
//// midori 155543 del -->
//	//if ((strCsvId.Compare(CSV_ID_DATA) == 0) ||
//	//	(strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) ||
//	//	(strCsvId.Compare(CSV_ID_HAITOKIN) == 0) ||
//	//	(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) {
//// midori 155543 del <--
//// midori 155543 add -->
//	if (((strCsvId.Compare(CSV_ID_DATA) == 0) ||
//		(strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) ||
//		(strCsvId.Compare(CSV_ID_HAITOKIN) == 0) ||
//		(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) && m_ErrMap.GetCount() > 0)	{
//// midori 155543 add <--
//// midori UC_0054 add <--
//		// エラー項目クリア後、全ての項目が空白になった場合、処理を行わない
//		// データがあるか？
//		for (ii = 0; ii < (int)pstrCsvDataAry->GetSize(); ii++) {
//			// 先頭は項目名称が格納されているはずなので無視
//			if (ii == 0) {
//				continue;
//			}
//			// 1項目でもデータがあればデータ行とみなす
//			if (pstrCsvDataAry->GetAt(ii).GetLength() > 0) {
//				sw = 1;
//				break;
//			}
//		}
//		if (sw == 0) {
//			return IMP_REC_OK;
//		}
//// midori UC_0054 add -->
//	}
//// midori UC_0054 add <--
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	int	ofset=10;
	if(m_NewVer != TRUE)	{
		// 10 借入理由１
		GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_111_RIYUU);
		pstrCsvDataAry->SetAt(ofset,strTemp);
		ofset++;
		// 11 借入理由２
		GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_111_RIYUU);
		pstrCsvDataAry->SetAt(ofset,strTemp);
		ofset++;
	}
	cs1 = pstrCsvDataAry->GetAt(ofset);
	cs2 = pstrCsvDataAry->GetAt(ofset+1);
	cs3 = _T("");
	cs4 = _T("");
	if(bG_Kanso == TRUE)	{
		// ２行のうちいずれかが全角１０文字を超えている場合３行にする
		if(cs1.GetLength() > 20 || cs2.GetLength() > 20)	{
			strTemp = cs1 + cs2;
			cs2 = GetSpritString(strTemp,&cs1,20);
			strTemp = cs2;
			cs3 = GetSpritString(strTemp,&cs2,20);
			// １段目、２段目に半角文字が奇数バイト数分、入力されている場合
			// ３段目に半角文字が１バイト余分に付いているのでカットする
			strTemp = cs3;
			cs4 = GetSpritString(strTemp,&cs3,12);
		}
		// 12 担保の内容（物件の種類、数量、所在地等）１
		pstrCsvDataAry->SetAt(ofset,cs1);
		// 13 担保の内容（物件の種類、数量、所在地等）２
		pstrCsvDataAry->SetAt(ofset+1,cs2);
		// 14 担保の内容（物件の種類、数量、所在地等）３
		pstrCsvDataAry->InsertAt(ofset+2,cs3);
	}
	else	{
		// 12 担保の内容（物件の種類、数量、所在地等）１
		GetSpritString(cs1,&strTemp,MAX_KETA_111_NAIYOU);
		pstrCsvDataAry->SetAt(ofset,strTemp);
		// 13 担保の内容（物件の種類、数量、所在地等）２
		GetSpritString(cs2,&strTemp,MAX_KETA_111_NAIYOU);
		pstrCsvDataAry->SetAt(ofset+1,strTemp);
	}
	//
	strCsvId = pstrCsvDataAry->GetAt(0);
	if (((strCsvId.Compare(CSV_ID_DATA) == 0) ||
		(strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) ||
		(strCsvId.Compare(CSV_ID_HAITOKIN) == 0) ||
		(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) && m_ErrMap.GetCount() > 0)	{
		// エラー項目クリア後、全ての項目が空白になった場合、処理を行わない
		// データがあるか？
		for (ii = 0; ii < (int)pstrCsvDataAry->GetSize(); ii++) {
			// 先頭は項目名称が格納されているはずなので無視
			if (ii == 0) {
				continue;
			}
			// 1項目でもデータがあればデータ行とみなす
			if (pstrCsvDataAry->GetAt(ii).GetLength() > 0) {
				sw = 1;
				break;
			}
		}
		if (sw == 0) {
			return IMP_REC_OK;
		}
	}
// 210113_5301_1 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc111Kariirekin*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順
	long			nAdSeq			= 0;				// 金融機関シーケンス番号
	long			nAdOrder		= 0;				// 金融機関並び順

	// 01 科目
	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
		return IMP_REC_NG;
	}

// midori 156189,156190,156191 del -->
	//// 02 借入先１
	//// 03 借入先２
	//if (CheckEntryBank(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3), &nAdSeq, &nAdOrder) != TRUE) {
	//	return IMP_REC_NG;
	//}
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	// 02 借入先１
	// 03 借入先２
// midori 157046 del -->
	//if ((strCsvId.Compare(CSV_ID_SYOKEI) != 0) && (strCsvId.Compare(CSV_ID_KAMOKU) != 0)) {
// midori 157046 del <--
// midori 157046 add -->
	if ((strCsvId.Compare(CSV_ID_SYOKEI) != 0) && (strCsvId.Compare(CSV_ID_CHUKEI) != 0)  && (strCsvId.Compare(CSV_ID_KAMOKU) != 0)) {
// midori 157046 add <--
		if (CheckEntryBank(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3), &nAdSeq, &nAdOrder) != TRUE) {
			return IMP_REC_NG;
		}
	}
// midori 156189,156190,156191 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データ加工
	// テーブルへデータ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++

	// 新規レコード作成
	m_pUcData->CreateNewRecord();

	// レコード編集開始
	m_pUcData->Edit();

	// XX 頁番号
	// XX 行番号
	CheckNumPageRow(&m_NumPage, &m_NumRow);
	m_pUcData->m_NumPage		= m_NumPage;
	m_pUcData->m_NumRow			= m_NumRow;

	// 00 特殊行フラグ
	m_pUcData->m_FgFunc			= m_FgFunc;

	// [明細]の場合のみ各項目のデータを登録
	// [小計]、[中計]は、計文言と頁番号、行番号、特殊行フラグのみ
	// 空行は、頁番号、行番号、特殊行フラグのみ
	switch (m_FgFunc) {
	case	ID_FGFUNC_DATA:			// データ行
		// 01 科目
		m_pUcData->m_KnSeq		= nKnSeq;
		m_pUcData->m_KnOrder	= nKnOrder;
		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));

		// 02 借入先１
		// 03 借入先２
		m_pUcData->m_AdSeq		= nAdSeq;
		m_pUcData->m_AdOrder	= nAdOrder;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(2);
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(3);
		m_pUcData->m_AdKana1	= fc.Convert(pstrCsvDataAry->GetAt(2));
		m_pUcData->m_AdKana2	= fc.Convert(pstrCsvDataAry->GetAt(3));

		// 04 所在地（住所）１
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(4);

		// 05 所在地（住所）２
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(5);

		// 06 法人・代表者との関係
		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(6);

		// 07 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		// 08 期中の支払利子額
		m_pUcData->m_Risoku		= pstrCsvDataAry->GetAt(8);
		RemoveCommaImportNum( m_pUcData->m_Risoku );			// カンマを除く　2011.07.15 h.ukawa

		// 09 利率
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(9), 1000, 0);	// ×1000
		m_pUcData->m_Rate		= strTemp;

// 210113_5301_1 del -->
//		// 10 借入理由１
//		// 11 借入理由２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(10), pstrCsvDataAry->GetAt(11));
//		m_pUcData->m_Riyuu		= strTemp;
//
//		// 12 担保の内容（物件の種類、数量、所在地等）１
//		// 13 担保の内容（物件の種類、数量、所在地等）２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(12), pstrCsvDataAry->GetAt(13));
//// midori 190301 0415 add -->
//		if(bG_Kanso == TRUE)	{
//			if(pstrCsvDataAry->GetAt(14).IsEmpty() == FALSE)	{
//				strTemp = m_clsFunc.StrDocking(strTemp,pstrCsvDataAry->GetAt(14));
//			}
//		}
//// midori 190301 0415 add <--
//		m_pUcData->m_Naiyou		= strTemp;
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		ofset=10;
		if(m_NewVer != TRUE)	{
			// 10 借入理由１
			// 11 借入理由２
			strTemp = _T("");
			strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
			m_pUcData->m_Riyuu		= strTemp;
			ofset=12;
		}
		// 12 担保の内容（物件の種類、数量、所在地等）１
		// 13 担保の内容（物件の種類、数量、所在地等）２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		if(bG_Kanso == TRUE)	{
			if(pstrCsvDataAry->GetAt(ofset+2).IsEmpty() == FALSE)	{
				strTemp = m_clsFunc.StrDocking(strTemp,pstrCsvDataAry->GetAt(ofset+2));
			}
		}
		m_pUcData->m_Naiyou		= strTemp;
// 210113_5301_1 add <--
		break;
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	case	ID_FGFUNC_KAMOKU:		// 科目行
//		if( pstrCsvDataAry->GetCount() >= CSV_021_ITEM_CNT )	{
//			m_pUcData->m_KnSeq		= nKnSeq;
//			m_pUcData->m_KnOrder	= nKnOrder;
//			m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
//			m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//		}
//		break;
//// midori 190505 add <--
//	case	ID_FGFUNC_SYOKEI:		// 小計行
//		m_pUcData->m_KeiStr		= m_strSyokeiName;
//		break;
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	case	ID_FGFUNC_KAMOKU:		// 科目行
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,1) == 1) {
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
// midori 157046 add <--
			m_pUcData->m_KeiStr		= m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
		}
		break;
	case	ID_FGFUNC_SYOKEI:		// 小計行
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,1) == 1) {
// midori 157046 del <--
// midori 157046 add -->
// midori 157046 add <--
		if(m_KmkRowSgn == 1) {
// 201224 del -->
//// midori 156937 del -->
//			//if(pstrCsvDataAry->GetAt(2).IsEmpty() == FALSE || pstrCsvDataAry->GetAt(3).IsEmpty() == FALSE) {
//			//	m_pUcData->m_KeiStr		= m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
//			//	strkei1 = pstrCsvDataAry->GetAt(2) + pstrCsvDataAry->GetAt(3);
//			//	strkei2 = ImpGetSyokeiName();
//			//	if(strkei1.Compare(strkei2) != 0) {
//			//		m_pUcData->m_ShowKeiZero |= BIT_D7;
//			//	}
//			//}
//			//else {
//			//	m_pUcData->m_KeiStr		= ImpGetSyokeiName();
//			//}
//// midori 156937 del <--
//// midori 156937 add -->
//			strkei1 = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
//// 157154 del -->
//			//if(GetKamokuKeiStrMain(m_nFormSeq,m_KmkRowSw,strkei1,m_strSyokeiName,m_strKmName,&strkei2) != 0) {
//			//	m_pUcData->m_ShowKeiZero |= BIT_D7;
//			//}
//// 157154 del <--
//// 157154 add -->
//			GetKamokuKeiStrMain(m_nFormSeq,m_KmkRowSw,strkei1,m_strSyokeiName,m_strKmName,&strkei2);
//// 157154 add <--
//			m_pUcData->m_KeiStr = strkei2;
//// midori 156937 add <--
// 201224 del <--
// 201224 add -->
			// 【小計】もそのまま取り込む
			m_pUcData->m_KeiStr = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
			if(m_pUcData->m_KeiStr.IsEmpty() == TRUE)	{
				// 何も入力されていなければ
				// 科目行あり　→　【小計】
				// 科目行なし　→　【小計(科目名)】
				// を自動でセットする
				GetKamokuKeiStrMain(m_nFormSeq,m_KmkRowSw,_T(""),m_strSyokeiName,m_strKmName,&strkei2);
				m_pUcData->m_KeiStr = strkei2;
			}
// 201224 add <--
		}
		else {
			m_pUcData->m_KeiStr		= m_strSyokeiName;
		}
		break;
// midori 156189,156190,156191 add <--
	case	ID_FGFUNC_CHUKEI:		// 中計行
// midori 157046 del --> 
		//m_pUcData->m_KeiStr		= m_strChukeiName;
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
			strkei1=m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
			strkei2.Empty();
			if(strkei1.IsEmpty() == TRUE) {
				strkei2=m_strChukeiName;
			}
			else {
				if(strkei1.Compare(m_strChukeiName) == 0) {
					strkei2=m_strChukeiName;
				}
				else {
					strkei2 = strkei1;
// 157154 del -->
					//m_pUcData->m_ShowKeiZero |= BIT_D7;
// 157154 del <--
				}
			}
			m_pUcData->m_KeiStr=strkei2;
		}
// midori 157046 add <--
		break;
	}

	// レコード登録
	m_pUcData->Update();

// midori 156189,156190,156191 add -->
	if(m_FgFunc == ID_FGFUNC_DATA) {
// 201224 del -->
		//m_strKmName = pstrCsvDataAry->GetAt(1);
// 201224 del <--
// 201224 add -->
		// 同一グループ内の先頭名称を使用する
		if(m_strKmName.IsEmpty() == TRUE)	{
			m_strKmName = pstrCsvDataAry->GetAt(1);
		}
// 201224 add <--
	}
	else if(m_FgFunc == ID_FGFUNC_KAMOKU) {
		m_KmkRowSw = 1;
		m_strKmName.Empty();
	}
	else if(m_FgFunc == ID_FGFUNC_SYOKEI) {
		m_KmkRowSw = 0;
		m_strKmName.Empty();
	}
	else {
		m_strKmName.Empty();
	}
// midori 156189,156190,156191 add <--

	return IMP_REC_OK;
}
// midori 180404 add <--

//**********************************************************************
// CSVの1行分のデータをテーブルから取得（エクスポート用）
//		・データの正当性をチェック
//		・データをCSVに記述する形式に変換
// 
//	引数
//			pstrCsvDataAry			[OUT]CSVの1行分のデータ
// 
//	戻値
//			レコード（行）の種類（DBUchiImpDEF.h参照）
//**********************************************************************
//virtual
int CimpUc111Kariirekin::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	CString			strCsvId		= _T("");		// CSVの識別子
	// midori 190301 0415 add -->
	CString			cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
// midori 156189,156190,156191 add -->
	CString				strkei=_T("");
	CString				strkei2=_T("");
	CStringArray		strArray;
// midori 156189,156190,156191 add <--
	CUcFunctionCommon	ufc;
// midori 190301 0415 add <--

	int nRet = GetCsvOneLineDataSub(pstrCsvDataAry, &strCsvId);
	// エラーもしくはEOFもしくは一括金額の場合終了
	if (nRet == IMP_REC_NG || 
		nRet == IMP_REC_EOF || 
		nRet == IMP_REC_IKKATU) {
		return nRet;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc111Kariirekin*)m_pUcCmn;


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CStringArray	strTempAry;						// 文字列配列テンポラリ
	CString			strTemp			= _T("");		// 文字列テンポラリ

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// [明細]の場合のみ各項目のデータを取得
// midori 156189,156190,156191 del -->
	//// [小計]、[中計]は識別子のみ
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	// [中計]は識別子のみ
// midori 156189,156190,156191 add <--
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
		// 01 科目
		pstrCsvDataAry->SetAt( 1, m_pUcData->m_KnName);

		// 02 借入先１
		pstrCsvDataAry->SetAt( 2, m_pUcData->m_AdName1);

		// 03 借入先２
		pstrCsvDataAry->SetAt( 3, m_pUcData->m_AdName2);

// midori 190301 0415 del -->
		//// 04 所在地（住所）１
		//pstrCsvDataAry->SetAt( 4, m_pUcData->m_AdAdd1);

		//// 05 所在地（住所）２
		//pstrCsvDataAry->SetAt( 5, m_pUcData->m_AdAdd2);

		//// 06 法人・代表者との関係
		//pstrCsvDataAry->SetAt( 6, m_pUcData->m_Kankei);
// midori 190301 0415 del <--
// midori 190301 0415 add -->
		// 04 所在地（住所）１
		cs =  m_pUcData->m_AdAdd1;
		if(bG_Kanso == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(4,cs);

		// 05 所在地（住所）２
		cs =  m_pUcData->m_AdAdd2;
		if(bG_Kanso == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(5,cs);

		// 06 法人・代表者との関係
		cs =  m_pUcData->m_Kankei;
// midori 154640 del -->
		//if(bG_Kanso == TRUE)	{
		//	cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		//}
// midori 154640 del <--
		pstrCsvDataAry->SetAt(6,cs);
// midori 190301 0415 add <--

		// 07 期末現在高
		pstrCsvDataAry->SetAt( 7, m_pUcData->m_Val);

		// 08 期中の支払利子額
		pstrCsvDataAry->SetAt( 8, m_pUcData->m_Risoku);

		// 09 利率
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Rate, 0.001, 3);		// ÷1000
		pstrCsvDataAry->SetAt( 9, strTemp);

// 210113_5301_1 del -->
		//// 10 借入理由１
		//// 11 借入理由２
		//strTempAry.RemoveAll();
		//if(bG_Kanso == TRUE) {
		//	strTempAry.SetSize(2);
		//	strTempAry.SetAt(0,_T(""));
		//	strTempAry.SetAt(1,_T(""));
		//}
		//else {
		//	m_clsFunc.StrDivision(m_pUcData->m_Riyuu,&strTempAry,2);
		//}
		//pstrCsvDataAry->SetAt(10, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(11, strTempAry.GetAt(1));

		//// 12 担保の内容（物件の種類、数量、所在地等）１
		//// 13 担保の内容（物件の種類、数量、所在地等）２
		//strTempAry.RemoveAll();
		//cs = m_pUcData->m_Naiyou;
		//if(bG_Kanso == TRUE)	{
		//	// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
		//	if(ufc.GetKaigyoCount(cs) >= 2)	{
		//		strTempAry.RemoveAll();
		//		ufc.StrDivision(cs,&strTempAry,3);
		//		// ３行入力されていても２行目が空白の場合は１行目と３行目をくっつけるだけにする
		//		if(strTempAry.GetAt(1).IsEmpty() == TRUE)	{
		//			cs = strTempAry.GetAt(0) + PRT_FORMAT_CHCODE + strTempAry.GetAt(2);
		//		}
		//		else	{
		//			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		//			cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_111_NAIYOU);
		//			cs = cs2;
		//			cs3 = ufc.GetSpritString(cs,&cs2,MAX_KETA_111_NAIYOU);
		//			cs = cs1 + PRT_FORMAT_CHCODE + cs2;
		//		}
		//		strTempAry.RemoveAll();
		//	}
		//}
		//m_clsFunc.StrDivision(cs,&strTempAry,2);
		//pstrCsvDataAry->SetAt(12,strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(13,strTempAry.GetAt(1));
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		int	ofset=10;
// 24/02/20_ScoreLink cor -->
		//if(bG_Kanso != TRUE)	{
// --------------------------
		if(bG_Kanso != TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 10 借入理由１
			// 11 借入理由２
			strTempAry.RemoveAll();
			if(bG_Kanso == TRUE) {
				strTempAry.SetSize(2);
				strTempAry.SetAt(0,_T(""));
				strTempAry.SetAt(1,_T(""));
			}
			else {
				m_clsFunc.StrDivision(m_pUcData->m_Riyuu,&strTempAry,2);
			}
			pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(0));
			ofset++;
			pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(1));
			ofset++;
		}
		// 12 担保の内容（物件の種類、数量、所在地等）１
		// 13 担保の内容（物件の種類、数量、所在地等）２
		strTempAry.RemoveAll();
		cs = m_pUcData->m_Naiyou;
		if(bG_Kanso == TRUE)	{
			// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
			if(ufc.GetKaigyoCount(cs) >= 2)	{
				strTempAry.RemoveAll();
				ufc.StrDivision(cs,&strTempAry,3);
				// ３行入力されていても２行目が空白の場合は１行目と３行目をくっつけるだけにする
				if(strTempAry.GetAt(1).IsEmpty() == TRUE)	{
					cs = strTempAry.GetAt(0) + PRT_FORMAT_CHCODE + strTempAry.GetAt(2);
				}
				else	{
					cs.Replace(PRT_FORMAT_CHCODE,_T(""));
					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_111_NAIYOU);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,MAX_KETA_111_NAIYOU);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
				}
				strTempAry.RemoveAll();
			}
		}
		m_clsFunc.StrDivision(cs,&strTempAry,2);
		pstrCsvDataAry->SetAt(ofset,strTempAry.GetAt(0));
		ofset++;
		pstrCsvDataAry->SetAt(ofset,strTempAry.GetAt(1));
// 210113_5301_1 add <--
	}
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	// [科目]の場合、科目のみデータを取得
//	else if (strCsvId.Compare(CSV_ID_KAMOKU) == 0) {
//		// 01 科目
//		pstrCsvDataAry->SetAt( 1, m_pUcData->m_KnName);
//	}
//// midori 190505 add <--
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	// [小計]の場合、名称のみデータを取得
	else if (strCsvId.Compare(CSV_ID_SYOKEI) == 0) {
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,0) == 1)	{
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
// midori 157046 add <--
			m_clsFunc.StrDivision( m_pUcData->m_KeiStr, &strArray, 2 );
			strkei = strArray.GetAt( 0 );
			strkei2 = strArray.GetAt( 1 );
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(2, strkei);
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(3, strkei2);
		}
	}
// midori 157046 add -->
	// [中計]の場合、名称のみデータを取得
	else if (strCsvId.Compare(CSV_ID_CHUKEI) == 0) {
		if(m_KmkRowSgn == 1) {
			m_clsFunc.StrDivision( m_pUcData->m_KeiStr, &strArray, 2 );
			strkei = strArray.GetAt( 0 );
			strkei2 = strArray.GetAt( 1 );
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(2, strkei);
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(3, strkei2);
		}
	}
// midori 157046 add <--
	// [科目]の場合、科目のみデータを取得
	else if (strCsvId.Compare(CSV_ID_KAMOKU) == 0) {
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,0) == 1)	{
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
// midori 157046 add <--
			// 科目行名称
			m_clsFunc.StrDivision( m_pUcData->m_KeiStr, &strArray, 2 );
			strkei = strArray.GetAt( 0 );
			strkei2 = strArray.GetAt( 1 );
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(2, strkei);
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(3, strkei2);
		}
	}
// midori 156189,156190,156191 add <--


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}

//********************************************************************************
//	一括集計金額行のデータ設定
//		IN		int					呼び出し元（0:ソート，1:一括金額参照，2:特殊行挿入）
//				CfrmUc000Common*	テーブル情報
//				CString				名称
//				CALCKEI_INFO		一括集計金額情報
//				SORTKAMOKU_INFO		科目情報
//		RET		なし
//********************************************************************************
void	CimpUc111Kariirekin::virTableEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	//CdbUc111Kariirekin*	rs = (CdbUc111Kariirekin*)rsData;

	switch (nType) {
	case 0:
		// ソートの一括集計処理
		m_pUcData->m_AdAdd2 = strName;					// 名称：借入先所在地名
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		m_pUcData->m_Risoku = uCalcKei.strKei[1];		// 利息："1:Risoku"固定

		m_pUcData->m_KnSeq = uKamoku.intKnSeq;			// 科目情報
		m_pUcData->m_KnOrder = uKamoku.intKnOrder;		// 科目情報
		m_pUcData->m_KnName = uKamoku.strKnName;		// 科目情報
		break;
	case 1:
		// 一括金額参照の戻す処理
		m_pUcData->m_AdAdd2 = strName;				// 名称：借入先所在地名
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		m_pUcData->m_Risoku = uCalcKei.strKei[1];		// 利息："1:Risoku"固定
		break;
	case 2:
		// 特殊行挿入の一括金額選択時
		m_pUcData->m_AdAdd2 = strName;				// 名称：借入先所在地名
		break;
	}
}

// midori 156937 del -->
//// midori 156189,156190,156191 add -->
//CString CimpUc111Kariirekin::ImpGetSyokeiName(void)
//{
//	CString syokeinm = _T("");
//
//	syokeinm = m_strSyokeiName;
//	if(m_KmkRowSw == 0 ) {
//		GetKamokuKeiStr(m_strSyokeiName,m_strKmName,&syokeinm);
//	}
//	return(syokeinm);
//}
//// midori 156189,156190,156191 add <--
// midori 156937 del <--

// 157766 del -->
//// 210113_5301_1 add -->
////********************************************************************************
////	新様式のCSVデータかチェックする
////		IN		pstrCsvDataAry			CSVの1行分のデータ
////
////		RET		TRUE  : 新様式
////				FALSE : 上記以外
////********************************************************************************
//BOOL CimpUc111Kariirekin::virFormatedIsNewVer(CStringArray* pstrCsvDataAry)
//{
//	BOOL	nRet=TRUE;
//	CString strItem=_T("");
//	int		ii=0;
//
//	for(ii=1/*[項目]読み飛ばし*/; ii<pstrCsvDataAry->GetCount(); ii++)	{
//		strItem.Empty();
//		switch(ii)	{
//			case	1:	strItem = CSV_111_ITEM_01;		break;
//			case	2:	strItem = CSV_111_ITEM_02_NEW;	break;
//			case	3:	strItem = CSV_111_ITEM_03_NEW;	break;
//			case	4:	strItem = CSV_111_ITEM_04_NEW;	break;
//			case	5:	strItem = CSV_111_ITEM_05_NEW;	break;
//			case	6:	strItem = CSV_111_ITEM_06;		break;
//			case	7:	strItem = CSV_111_ITEM_07;		break;
//			case	8:	strItem = CSV_111_ITEM_08;		break;
//			case	9:	strItem = CSV_111_ITEM_09;		break;
//			case	10:	strItem = CSV_111_ITEM_12;		break;
//			case	11:	strItem = CSV_111_ITEM_13;		break;
//			default:									break;
//		}
//		if(pstrCsvDataAry->GetAt(ii).Compare(strItem) != 0)	{
//			nRet = FALSE;
//			break;
//		}
//	}
//
//	return(nRet);
//}
//// 210113_5301_1 add <--
// 157766 del <--
