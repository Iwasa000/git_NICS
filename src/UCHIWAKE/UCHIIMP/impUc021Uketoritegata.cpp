#include "StdAfx.h"
#include "impUc021Uketoritegata.h"

// midori 190301 0415 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\prtConstData.h"

extern	BOOL	bG_Kanso;
// midori 190301 0415 add <--
// 登録番号（法人番号）_23/11/21 add -->
extern	BOOL	bG_InvNo;
// 登録番号（法人番号）_23/11/21 add <--
// 24/02/20_ScoreLink add -->
extern	BOOL	bG_Sl;
// 24/02/20_ScoreLink add <--

// 改良No.21-0086,21-0529 cor -->
//// midori 156188 add -->
//extern	int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156188 add <--
// ------------------------------
extern	int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--


// コンストラクタ
CimpUc021Uketoritegata::CimpUc021Uketoritegata(void)
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
CimpUc021Uketoritegata::~CimpUc021Uketoritegata(void)
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
int CimpUc021Uketoritegata::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_021;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_021_ITEM_CNT;	// 科目追加以前のCSVを読み込む為

// 登録番号（法人番号）_23/11/21 add -->
// 24/02/20_ScoreLink cor -->
	//if(m_IsExport == TRUE && bG_InvNo == TRUE)	{
// --------------------------
	if(m_IsExport == TRUE && (bG_InvNo == TRUE || bG_Sl == TRUE))	{
// 24/02/20_ScoreLink cor <--
		// エクスポート時、新様式なら項目数を増やしておく
		// (インポートは新旧両方のレイアウトに対応するため、ここでは決定できない)
		m_nItemCount = m_nItemCount + 1;
	}
// 登録番号（法人番号）_23/11/21 add <--

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc021Uketoritegata(pDB));

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
	m_RowMax		= ID_ROWKOBETU_021;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_021 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_021;


	return IMP_ERRCODE_SUCCESS;
}

//**********************************************************************
// midori 180404 del -->
//// CSVの1行分のデータをテーブルに登録（インポート用）
////		・データの正当性をチェック
////		・データをテーブルに格納する形式に変換
////		・テーブルに登録
// midori 180404 del <--
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
int CimpUc021Uketoritegata::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc021Uketoritegata*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

// midori 156936 add -->
	CsvInfoClr(&m_uImpCsvInfo);
// midori 156936 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	int Offset = 0;	// 科目が追加されている場合は1
// 登録番号（法人番号）_23/11/21 cor -->
	//if( pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT ){
// -------------------------------------
	int	sw=0;
	if(m_NewVer2 == TRUE)	{
// 231227_168519 cor -->
		//if(pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT + 1)	{
// ---------------------
		if(pstrCsvDataAry->GetCount() >= CSV_021_ITEM_CNT + 1)	{
// 231227_168519 cor <--
			sw=1;
		}
	}
	else	{
		if(pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT)	{
			sw=1;
		}
	}
	if(sw != 0)	{
// 登録番号（法人番号）_23/11/21 cor <--
		// 01 科目
		if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_021_KAMOKU*2) != TRUE) {
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_01);
			m_uImpCsvInfo.nLength = MAX_KETA_021_KAMOKU;
			m_uImpCsvInfo.nKind = 0;	// 0:文字
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}

		Offset++;
	}

// 登録番号（法人番号）_23/11/21 add -->
	if(m_NewVer2 == TRUE)	{
		// 13 登録番号（法人番号）
		if(bG_InvNo == TRUE)	{
			CString	cs=_T("");
			cs = pstrCsvDataAry->GetAt(1+Offset);
			if(CheckImportTno(cs) != TRUE)	{
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_021_ITEM_13);
				m_uImpCsvInfo.nLength = 0;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
				// 登録番号（法人番号）は桁数までのみ取り込むことをしないので
				// ここでクリアする
				pstrCsvDataAry->SetAt(1+Offset,_T(""));
			}
			else	{
				// 先頭が "T" ではない場合、半角スペースが挿入されているので再セット
				// "T6210001025986" → "T6210001025986"
				// "6210001025986"  → " 6210001025986"
				pstrCsvDataAry->SetAt(1+Offset,cs);
			}
		}
		Offset++;
	}
// 登録番号（法人番号）_23/11/21 add <--

	if(m_KmkRowSgn == 1) {
		// 科目行
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_KAMOKU) == 0) {
			// 振出人１、振出人２
			if ( pstrCsvDataAry->GetAt(1 + Offset) == _T("") && pstrCsvDataAry->GetAt(2 + Offset) == _T("")){
				m_uImpCsvInfo.nErr = 4;
				return(IMP_REC_NG);
			}
		}
	}

	if(m_KmkRowSgn == 0) {
		kt = MAX_KETA_021_ADNAME1;
	}
	else {
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_CHUKEI) == 0) {
			kt = MAX_KETA_021_SYOKEI;
		}
		else {
			kt = MAX_KETA_021_ADNAME1;
		}
	}
	if(pstrCsvDataAry->GetAt(1 + Offset).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(2 + Offset) != TRUE)	{
		pstrCsvDataAry->SetAt(1 + Offset,pstrCsvDataAry->GetAt(2 + Offset));
		pstrCsvDataAry->SetAt(2 + Offset,_T(""));
	}
	// 02 振出人１
	if (CheckImportStr(pstrCsvDataAry->GetAt(1 + Offset), kt) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_02);
		m_uImpCsvInfo.nLength = kt / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 振出人２
	if (CheckImportStr(pstrCsvDataAry->GetAt(2 + Offset), kt) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_03);
		m_uImpCsvInfo.nLength = kt / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// midori 156936 add <--

	// 04 振出年月日
	long nSpDate = 0;
	if ( pstrCsvDataAry->GetAt(3 + Offset) != _T("")){
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(3 + Offset), &nSpDate) != TRUE) {
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_04);
// midori 150899 cor -->
			//return IMP_REC_NG;
// ---------------------
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
// midori 150899 cor <--
		}
	}

	// 05 支払期日
	long nPmDate = 0;
	if ( pstrCsvDataAry->GetAt(4 + Offset) != _T("")){
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(4 + Offset), &nPmDate) != TRUE) {
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_05);
// midori 150899 cor -->
			//return IMP_REC_NG;
// ---------------------
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
// midori 150899 cor <--
		}
	}

	// 06 支払銀行名
	if (CheckImportStr(pstrCsvDataAry->GetAt(5 + Offset), MAX_KETA_021_BKNAME1*2) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_06);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_06_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_06);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_021_BKNAME1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 07 支店名
	if (CheckImportStr(pstrCsvDataAry->GetAt(6 + Offset), MAX_KETA_021_BKNAME2*2) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_07);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_07_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_07);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_021_BKNAME2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 08 金額
// No.200903 del -->
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL_021_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL_021_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(7 + Offset), ID_VAL_021_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_08);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL_021_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(7 + Offset).Left(1) == '-')	{
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
	if(bG_Kanso == TRUE)	len = ID_VAL_021_K_DATA;
	else					len = ID_VAL_021_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(7 + Offset), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_08);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(7 + Offset).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

	// 09 割引銀行名等
	if (CheckImportStr(pstrCsvDataAry->GetAt(8 + Offset), MAX_KETA_021_BK2NAME1*2) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_09);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_09_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_09);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_021_BK2NAME1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 10 支店名
	if (CheckImportStr(pstrCsvDataAry->GetAt(9 + Offset), MAX_KETA_021_BK2NAME2*2) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_10);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_10_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_10);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_021_BK2NAME2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 11 摘要１
	if (CheckImportStr(pstrCsvDataAry->GetAt(10 + Offset), MAX_KETA_021_TEKI) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_11);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_021_TEKI / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 12 摘要２
	if (CheckImportStr(pstrCsvDataAry->GetAt(11 + Offset), MAX_KETA_021_TEKI) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_021_ITEM_12);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_021_TEKI / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

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
//	long			nAdSeq		= 0;					// 取引先シーケンス番号
//	long			nAdOrder	= 0;					// 取引先並び順
//	int				nAdRessGr	= ID_ADDRESSGR_URI;		// 取引先グループ（売掛金）
//	long			nBkSeq1		= 0;					// 金融機関シーケンス番号
//	long            nBkOrder1	= 0;					// 金融機関並び順
//	long			nBkSeq2		= 0;					// 金融機関シーケンス番号
//	long            nBkOrder2	= 0;					// 金融機関並び順
//
//	// 01 科目
//	if( pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT ){
//		if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
//			return IMP_REC_NG;
//		}
//	}
//
//	// 01 振出人１
//	// 02 振出人２
//	// 取引先（得意先）
//	if (CheckEntryAddress(	pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset),
//							nAdRessGr, &nAdSeq, &nAdOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	// 05 支払銀行名
//	// 06 支店名
//	// 金融機関
//	if (CheckEntryBank(pstrCsvDataAry->GetAt(5 + Offset), pstrCsvDataAry->GetAt(6 + Offset), &nBkSeq1, &nBkOrder1) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	// 08 割引銀行名等
//	// 09 支店名
//	// 金融機関
//	if (CheckEntryBank(pstrCsvDataAry->GetAt(8 + Offset), pstrCsvDataAry->GetAt(9 + Offset), &nBkSeq2, &nBkOrder2) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// データ加工
//	// テーブルへデータ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	CString strTemp = _T("");
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
//		if( pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT ){
//			m_pUcData->m_KnSeq		= nKnSeq;
//			m_pUcData->m_KnOrder	= nKnOrder;
//			m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
//// midori 152137 add -->
//			m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//// midori 152137 add <--
//		}
//
//		// 02 振出人１
//		// 03 振出人２
//		m_pUcData->m_AdSeq		= nAdSeq;
//		m_pUcData->m_AdOrder	= nAdOrder;
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1 + Offset);
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(2 + Offset);
//// midori 152137 add -->
//		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(1 + Offset));
//// midori 152137 add <--
//
//		// 04 振出年月日
//		m_pUcData->m_SpDate		= nSpDate;
//
//		// 05 支払期日
//		m_pUcData->m_PmDate		= nPmDate;
//
//		// 06 支払銀行名
//		// 07 支店名
//		m_pUcData->m_BkSeq		= nBkSeq1;
//		m_pUcData->m_BkOrder	= nBkOrder1;
//		m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(5 + Offset);
//		m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(6 + Offset);
//
//		// 08 金額
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7 + Offset);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 09 割引銀行名等
//		// 10 支店名
//		m_pUcData->m_Bk2Seq		= nBkSeq2;
//		m_pUcData->m_Bk2Order	= nBkOrder2;
//		m_pUcData->m_Bk2Name1	= pstrCsvDataAry->GetAt(8 + Offset);
//		m_pUcData->m_Bk2Name2	= pstrCsvDataAry->GetAt(9 + Offset);
//
//		// 11 摘要１
//		// 12 摘要２
//		strTemp = _T("");			// [2009/09/11 Add]
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(10 + Offset), pstrCsvDataAry->GetAt(11 + Offset));
//		m_pUcData->m_Teki		= strTemp; 
//
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
int CimpUc021Uketoritegata::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
	int				Offset = 0;						// 科目が追加されている場合は1
// 登録番号（法人番号）_23/11/21 add -->
	int				save=0;
// 登録番号（法人番号）_23/11/21 add <--
// midori 156189,156190,156191 add -->
	int				kt=0;
// midori 156189,156190,156191 add <--
// No.200903 add -->
	int				len=0;
// No.200903 add <--
	long			nSpDate = 0;
	long			nPmDate = 0;
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori 190301 0415 add -->
	CString			strTemp1=_T("");
	CString			strTemp2=_T("");
	CString			strTemp3=_T("");
	CString			strTemp4=_T("");
// midori 190301 0415 add <--
// 登録番号（法人番号）_23/11/21 add -->
	CString			cs=_T(""),cs1=_T(""),cs2=_T(""),cs3=_T("");
// 登録番号（法人番号）_23/11/21 add <--
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
// 157766 add -->
	// 新様式マスターで科目列が無いファイルを取り込んだ時に、科目名に振出人名が入る不具合
	// 新様式は摘要３の列が追加されるので、追加前の列数を保存しておく
	int				clmcnt = pstrCsvDataAry->GetCount();
// 157766 add <--

	// 文字数をオーバーしていればカットする
	// 01 科目
// 登録番号（法人番号）_23/11/21 cor <--
	//if( pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT ){
// -------------------------------------
	int	sw2=0;
	if(m_NewVer2 == TRUE)	{
// 231227_168519 cor -->
		//if(pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT + 1)	{
// ---------------------
		if(pstrCsvDataAry->GetCount() >= CSV_021_ITEM_CNT + 1)	{
// 231227_168519 cor <--
			sw2=1;
		}
	}
	else	{
		if(pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT)	{
			sw2=1;
		}
	}
	if(sw2 != 0)	{
// 登録番号（法人番号）_23/11/21 cor <--
		GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_021_KAMOKU*2);
		pstrCsvDataAry->SetAt(1,strTemp);
		Offset++;
	}

// 登録番号（法人番号）_23/11/21 add -->
	if(m_NewVer2 == TRUE)	{
		// 13 登録番号（法人番号）
		Offset++;
	}
// 登録番号（法人番号）_23/11/21 add <--

	if(m_KmkRowSgn == 0) {
		kt = MAX_KETA_021_ADNAME1;
	}
	else {
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_CHUKEI) == 0) {
			kt = MAX_KETA_021_SYOKEI;
		}
		else {
			kt = MAX_KETA_021_ADNAME1;
		}
	}
	// 02 振出人１
	GetSpritString(pstrCsvDataAry->GetAt(1 + Offset),&strTemp,kt);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(1 + Offset,strTemp);
	// 03 振出人２
	GetSpritString(pstrCsvDataAry->GetAt(2 + Offset),&strTemp,kt);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(2 + Offset,strTemp);
// midori 156936 add <--
	// 04 振出年月日
	// エラーの場合、クリアーする
	if ( pstrCsvDataAry->GetAt(3 + Offset) != _T("")){
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(3 + Offset), &nSpDate) != TRUE) {
			strTemp.Empty();
			pstrCsvDataAry->SetAt(3 + Offset,strTemp);
		}
	}
	// 05 支払期日
	// エラーの場合、クリアーする
	if ( pstrCsvDataAry->GetAt(4 + Offset) != _T("")){
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(4 + Offset), &nPmDate) != TRUE) {
			strTemp.Empty();
			pstrCsvDataAry->SetAt(4 + Offset,strTemp);
		}
	}

	// 06 支払銀行名
	GetSpritString(pstrCsvDataAry->GetAt(5 + Offset),&strTemp,MAX_KETA_021_BKNAME1*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
// 登録番号（法人番号）_23/11/21 add -->
	if(bG_InvNo == TRUE)	{
		// 全角５文字を超えていれば改行マークを付けて取り込む
		if(strTemp.GetLength() > 10)	{
			cs2 = GetSpritString(strTemp,&cs1,10);
			cs = cs2;
			// 前半に半角文字が奇数バイト数分、入力されている場合
			// 後半に半角文字が１バイト余分に付いているのでカットする
			cs3 = GetSpritString(cs,&cs2,10);
			strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
		}
	}
// 登録番号（法人番号）_23/11/21 add <--
	pstrCsvDataAry->SetAt(5 + Offset,strTemp);

	// 07 支店名
	GetSpritString(pstrCsvDataAry->GetAt(6 + Offset),&strTemp,MAX_KETA_021_BKNAME2*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
// 登録番号（法人番号）_23/11/21 add -->
	if(bG_InvNo == TRUE)	{
		// 全角５文字を超えていれば改行マークを付けて取り込む
		if(strTemp.GetLength() > 10)	{
			cs2 = GetSpritString(strTemp,&cs1,10);
			cs = cs2;
			// 前半に半角文字が奇数バイト数分、入力されている場合
			// 後半に半角文字が１バイト余分に付いているのでカットする
			cs3 = GetSpritString(cs,&cs2,10);
			strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
		}
	}
// 登録番号（法人番号）_23/11/21 add <--
	pstrCsvDataAry->SetAt(6 + Offset,strTemp);

	// 08 金額
	// エラーの場合、クリアーする
// No.200903 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(7 + Offset), ID_VAL_021_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_021_K_DATA;
	else 					len = ID_VAL_021_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(7 + Offset), len, TRUE) != TRUE)	{
// No.200903 add <--
		strTemp.Empty();
		pstrCsvDataAry->SetAt(7 + Offset,strTemp);
	}
	// 09 割引銀行名等
	GetSpritString(pstrCsvDataAry->GetAt(8 + Offset),&strTemp,MAX_KETA_021_BK2NAME1*2);
// midori 190504 add -->
	strTemp = ufc.DeleteRightSpace(strTemp);
// midori 190504 add <--
	pstrCsvDataAry->SetAt(8 + Offset,strTemp);
	// 10 支店名
	GetSpritString(pstrCsvDataAry->GetAt(9 + Offset),&strTemp,MAX_KETA_021_BK2NAME2*2);
// midori 190504 add -->
	strTemp = ufc.DeleteRightSpace(strTemp);
// midori 190504 add <--
	pstrCsvDataAry->SetAt(9 + Offset,strTemp);
// midori 190301 0415 del -->
	//// 11 摘要１
	//GetSpritString(pstrCsvDataAry->GetAt(10 + Offset),&strTemp,MAX_KETA_021_TEKI);
	//pstrCsvDataAry->SetAt(10 + Offset,strTemp);
	//// 12 摘要２
	//GetSpritString(pstrCsvDataAry->GetAt(11 + Offset),&strTemp,MAX_KETA_021_TEKI);
	//pstrCsvDataAry->SetAt(11 + Offset,strTemp);
// midori 190301 0415 del <--
// midori 190301 0415 add -->
	strTemp1 = pstrCsvDataAry->GetAt(10 + Offset);
	strTemp2 = pstrCsvDataAry->GetAt(11 + Offset);
	strTemp3 = _T("");
	strTemp4 = _T("");
	if(bG_Kanso == TRUE)	{
		// ２行のうちいずれかが全角１１文字を超えている場合３行にする
		if(strTemp1.GetLength() > 22 || strTemp2.GetLength() > 22)	{
			strTemp = strTemp1 + strTemp2;
			strTemp2 = GetSpritString(strTemp,&strTemp1,22);
			strTemp = strTemp2;
			strTemp3 = GetSpritString(strTemp,&strTemp2,22);
			// １段目、２段目に半角文字が奇数バイト数分、入力されている場合
			// ３段目に半角文字が１バイト余分に付いているのでカットする
			strTemp = strTemp3;
			strTemp4 = GetSpritString(strTemp,&strTemp3,20);
		}
		// 11 摘要１
		pstrCsvDataAry->SetAt(10 + Offset,strTemp1);
		// 12 摘要２
		pstrCsvDataAry->SetAt(11 + Offset,strTemp2);
		// 13 摘要３(配列が作成されていないので拡張する)
		pstrCsvDataAry->InsertAt(12 + Offset,strTemp3);
	}
	else	{
		// 11 摘要１
		GetSpritString(strTemp1,&strTemp,MAX_KETA_021_TEKI);
		pstrCsvDataAry->SetAt(10 + Offset,strTemp);
		// 12 摘要２
		GetSpritString(strTemp2,&strTemp,MAX_KETA_021_TEKI);
		pstrCsvDataAry->SetAt(11 + Offset,strTemp);
	}
// midori 190301 0415 add <--

// midori UC_0054 add -->
	strCsvId = pstrCsvDataAry->GetAt(0);
// midori 155543 del -->
	//if ((strCsvId.Compare(CSV_ID_DATA) == 0) ||
	//	(strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) ||
	//	(strCsvId.Compare(CSV_ID_HAITOKIN) == 0) ||
	//	(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) {
// midori 155543 del <--
// midori 155543 add -->
	if (((strCsvId.Compare(CSV_ID_DATA) == 0) ||
		(strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) ||
		(strCsvId.Compare(CSV_ID_HAITOKIN) == 0) ||
		(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) && m_ErrMap.GetCount() > 0)	{
// midori 155543 add <--
// midori UC_0054 add <--
		// エラー項目クリア後、全ての項目が空白になった場合、処理を行わない
		// データがあるか？
		for (ii = 0; ii <(int)pstrCsvDataAry->GetSize(); ii++) {
			// 先頭は項目名称が格納されているはずなので無視
			if (ii == 0) {
				continue;
			}
			// 1項目でもデータがあればデータ行とみなす
			if (pstrCsvDataAry->GetAt(ii).GetLength() > 0){
				sw = 1;
				break;
			}
		}
		if(sw == 0)	{
			return IMP_REC_OK;
		}
// midori UC_0054 add -->
	}
// midori UC_0054 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc021Uketoritegata*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順
	long			nAdSeq		= 0;					// 取引先シーケンス番号
	long			nAdOrder	= 0;					// 取引先並び順
	int				nAdRessGr	= ID_ADDRESSGR_URI;		// 取引先グループ（売掛金）
	long			nBkSeq1		= 0;					// 金融機関シーケンス番号
	long            nBkOrder1	= 0;					// 金融機関並び順
	long			nBkSeq2		= 0;					// 金融機関シーケンス番号
	long            nBkOrder2	= 0;					// 金融機関並び順

	// 01 科目
// 登録番号（法人番号）_23/11/21 cor <--
	//if(clmcnt == CSV_021_ITEM_CNT)	{
// -------------------------------------
	sw2=0;
	if(m_NewVer2 == TRUE)	{
// 231227_168519 cor -->
		//if(clmcnt == CSV_021_ITEM_CNT + 1)	{
// ---------------------
		if(clmcnt >= CSV_021_ITEM_CNT + 1)	{
// 231227_168519 cor <--
			sw2=1;
		}
	}
	else	{
		if(clmcnt == CSV_021_ITEM_CNT)	{
			sw2=1;
		}
	}
	if(sw2 != 0)	{
// 登録番号（法人番号）_23/11/21 cor <--
		if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
			return IMP_REC_NG;
		}
	}

	// 01 振出人１
	// 02 振出人２
	// 取引先（得意先）
// 登録番号（法人番号）_23/11/21 cor -->
	//if ((strCsvId.Compare(CSV_ID_SYOKEI) != 0) && (strCsvId.Compare(CSV_ID_CHUKEI) != 0) && (strCsvId.Compare(CSV_ID_KAMOKU) != 0)) {
	//	if (CheckEntryAddress(	pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset),
	//							nAdRessGr, &nAdSeq, &nAdOrder) != TRUE) {
	//		return IMP_REC_NG;
	//	}
	//}
// -------------------------------------
	cs1 = pstrCsvDataAry->GetAt(1 + Offset);
	cs2 = pstrCsvDataAry->GetAt(2 + Offset);
	cs3 = _T("");
	if(m_NewVer2 == TRUE)	{
		if(bG_InvNo == TRUE)	{
			cs3 = pstrCsvDataAry->GetAt(2);
// 231222_168474 add -->
			if(CheckImportTno(cs3) != TRUE)	{
				cs3 = _T("");
				pstrCsvDataAry->SetAt(2,cs3);
			}
// 231222_168474 add <--
		}
	}
	if ((strCsvId.Compare(CSV_ID_SYOKEI) != 0) && (strCsvId.Compare(CSV_ID_CHUKEI) != 0) && (strCsvId.Compare(CSV_ID_KAMOKU) != 0)) {
		if (CheckEntryAddress(	cs1, cs2,
								nAdRessGr, &nAdSeq, &nAdOrder, cs3) != TRUE) {
			return IMP_REC_NG;
		}
	}
// 登録番号（法人番号）_23/11/21 cor <--

	// 05 支払銀行名
	// 06 支店名
	// 金融機関
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckEntryBank(pstrCsvDataAry->GetAt(5 + Offset), pstrCsvDataAry->GetAt(6 + Offset), &nBkSeq1, &nBkOrder1) != TRUE) {
// -------------------------------------
	cs1 = pstrCsvDataAry->GetAt(5 + Offset);
	cs1.Replace(PRT_FORMAT_CHCODE,_T(""));
	cs2 = pstrCsvDataAry->GetAt(6 + Offset);
	cs2.Replace(PRT_FORMAT_CHCODE,_T(""));
	if (CheckEntryBank(cs1, cs2, &nBkSeq1, &nBkOrder1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		return IMP_REC_NG;
	}

	// 08 割引銀行名等
	// 09 支店名
	// 金融機関
	if (CheckEntryBank(pstrCsvDataAry->GetAt(8 + Offset), pstrCsvDataAry->GetAt(9 + Offset), &nBkSeq2, &nBkOrder2) != TRUE) {
		return IMP_REC_NG;
	}

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
// 登録番号（法人番号）_23/11/21 cor -->
		//if(clmcnt == CSV_021_ITEM_CNT)	{
// -------------------------------------
		sw2=0;
		if(m_NewVer2 == TRUE)	{
// 231227_168519 cor -->
			//if(clmcnt == CSV_021_ITEM_CNT + 1)	{
// ---------------------
			if(clmcnt >= CSV_021_ITEM_CNT + 1)	{
// 231227_168519 cor <--
				sw2=1;
			}
		}
		else	{
			if(clmcnt == CSV_021_ITEM_CNT)	{
				sw2=1;
			}
		}
		if(sw2 != 0)	{
// 登録番号（法人番号）_23/11/21 cor <--
			m_pUcData->m_KnSeq		= nKnSeq;
			m_pUcData->m_KnOrder	= nKnOrder;
			m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
			m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
		}

		if(m_NewVer2 == TRUE)	{
			// 13 登録番号
			if(bG_InvNo == TRUE)	{
				m_pUcData->m_InvNo = pstrCsvDataAry->GetAt(2);
			}
		}

		// 02 振出人１
		// 03 振出人２
		m_pUcData->m_AdSeq		= nAdSeq;
		m_pUcData->m_AdOrder	= nAdOrder;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1 + Offset);
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(2 + Offset);
		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(1 + Offset));

		// 04 振出年月日
		m_pUcData->m_SpDate		= nSpDate;

		// 05 支払期日
		m_pUcData->m_PmDate		= nPmDate;

		// 06 支払銀行名
		// 07 支店名
		m_pUcData->m_BkSeq		= nBkSeq1;
		m_pUcData->m_BkOrder	= nBkOrder1;
		m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(5 + Offset);
		m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(6 + Offset);

		// 08 金額
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7 + Offset);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		// 09 割引銀行名等
		// 10 支店名
		m_pUcData->m_Bk2Seq		= nBkSeq2;
		m_pUcData->m_Bk2Order	= nBkOrder2;
		m_pUcData->m_Bk2Name1	= pstrCsvDataAry->GetAt(8 + Offset);
		m_pUcData->m_Bk2Name2	= pstrCsvDataAry->GetAt(9 + Offset);

		// 11 摘要１
		// 12 摘要２
		strTemp = _T("");			// [2009/09/11 Add]
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(10 + Offset), pstrCsvDataAry->GetAt(11 + Offset));
// midori 190301 0415 add -->
		if(bG_Kanso == TRUE)	{
			if(pstrCsvDataAry->GetAt(12 + Offset).IsEmpty() == FALSE)	{
				strTemp = m_clsFunc.StrDocking(strTemp,pstrCsvDataAry->GetAt(12 + Offset));
			}
		}
// midori 190301 0415 add <--
		m_pUcData->m_Teki		= strTemp; 

		break;
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	case	ID_FGFUNC_KAMOKU:		// 科目行
//		if( pstrCsvDataAry->GetCount() >= CSV_021_ITEM_CNT ){
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
			m_pUcData->m_KeiStr		= m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset));
		}
		break;
	case	ID_FGFUNC_SYOKEI:		// 小計行
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,1) == 1) {
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
// midori 157046 add <--
// 201224 del -->
//// midori 156937 del -->
//			//if(pstrCsvDataAry->GetAt(1 + Offset).IsEmpty() == FALSE || pstrCsvDataAry->GetAt(2 + Offset).IsEmpty() == FALSE) {
//			//	m_pUcData->m_KeiStr		= m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset));
//			//	strkei1 = pstrCsvDataAry->GetAt(1 + Offset) + pstrCsvDataAry->GetAt(2 + Offset);
//			//	strkei2 = ImpGetSyokeiName();
//			//	if(strkei1.Compare(strkei2) != 0) {
//			//		m_pUcData->m_ShowKeiZero |= BIT_D7;
//			//	}
//			//}
//			//else {
//			//	m_pUcData->m_KeiStr = ImpGetSyokeiName();
//			//}
//// midori 156937 del <--
//// midori 156937 add -->
//			strkei1 = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset));
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
			m_pUcData->m_KeiStr = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset));
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
			strkei1=m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1 + Offset), pstrCsvDataAry->GetAt(2 + Offset));
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
int CimpUc021Uketoritegata::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	CString			strCsvId		= _T("");		// CSVの識別子
// midori 190301 0415 add -->
	CString				cs=_T("");
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

	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
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
	m_pUcData = (CdbUc021Uketoritegata*) m_pUcCmn;


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CStringArray	strTempAry;						// 文字列配列テンポラリ
// 登録番号（法人番号）_23/11/21 cor -->
	int				rno=0;
// 登録番号（法人番号）_23/11/21 cor <--

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

// 登録番号（法人番号）_23/11/21 cor -->
//		// 01 科目
//		pstrCsvDataAry->SetAt(1, m_pUcData->m_KnName);
//
//		// 02 振出人１
//		pstrCsvDataAry->SetAt(2, m_pUcData->m_AdName1);
//
//		// 03 振出人２
//		pstrCsvDataAry->SetAt(3, m_pUcData->m_AdName2);
//
//		// 04 振出年月日
//		CString strSpDate;
//		strSpDate.Empty();
//		if (m_pUcData->m_SpDate > 0) {
//			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strSpDate);
//		}
//		pstrCsvDataAry->SetAt(4, strSpDate);
//
//		// 05 支払期日
//		CString strPmDate;
//		strPmDate.Empty();
//		if (m_pUcData->m_PmDate > 0) {
//			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_PmDate, &strPmDate);
//		}
//		pstrCsvDataAry->SetAt(5, strPmDate);
//
//		// 06 支払銀行名
//		pstrCsvDataAry->SetAt(6, m_pUcData->m_BkName1);
//
//		// 07 支店名
//		pstrCsvDataAry->SetAt(7, m_pUcData->m_BkName2);
//
//		// 08 金額
//		pstrCsvDataAry->SetAt(8, m_pUcData->m_Val);
//
//		// 09 割引銀行名等
//		pstrCsvDataAry->SetAt(9, m_pUcData->m_Bk2Name1);
//
//		// 10 支店名
//		pstrCsvDataAry->SetAt(10, m_pUcData->m_Bk2Name2);
//
//		// 11 摘要１
//		// 12 摘要２
//		strTempAry.RemoveAll();
//// midori 190301 0415 del -->
//		//m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
//		//pstrCsvDataAry->SetAt(11, strTempAry.GetAt(0));
//		//pstrCsvDataAry->SetAt(12, strTempAry.GetAt(1));
//// midori 190301 0415 del <--
//// midori 190301 0415 add -->
//		cs = m_pUcData->m_Teki;
//		if(bG_Kanso == TRUE)	{
//			// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
//			if(ufc.GetKaigyoCount(cs) >= 2)	{
//				strTempAry.RemoveAll();
//				ufc.StrDivision(cs,&strTempAry,3);
//				// ３行入力されていても２行目が空白の場合は１行目と３行目をくっつけるだけにする
//				if(strTempAry.GetAt(1).IsEmpty() == TRUE)	{
//					cs = strTempAry.GetAt(0) + PRT_FORMAT_CHCODE + strTempAry.GetAt(2);
//				}
//				else	{
//					cs.Replace(PRT_FORMAT_CHCODE,_T(""));
//					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_021_TEKI);
//					cs = cs2;
//					cs3 = ufc.GetSpritString(cs,&cs2,MAX_KETA_021_TEKI);
//					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
//				}
//				strTempAry.RemoveAll();
//			}
//		}
//		m_clsFunc.StrDivision(cs,&strTempAry,2);
//		pstrCsvDataAry->SetAt(11,strTempAry.GetAt(0));
//		pstrCsvDataAry->SetAt(12,strTempAry.GetAt(1));
//// midori 190301 0415 add <--
// -------------------------------------
		rno=1;

		// 01 科目
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_KnName);
		rno++;

// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 13 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}

		// 02 振出人１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName1);
		rno++;
		// 03 振出人２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName2);
		rno++;
		// 04 振出年月日
		CString strSpDate;
		strSpDate.Empty();
		if (m_pUcData->m_SpDate > 0) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strSpDate);
		}
		pstrCsvDataAry->SetAt(rno, strSpDate);
		rno++;
		// 05 支払期日
		CString strPmDate;
		strPmDate.Empty();
		if (m_pUcData->m_PmDate > 0) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_PmDate, &strPmDate);
		}
		pstrCsvDataAry->SetAt(rno, strPmDate);
		rno++;
		// 06 支払銀行名
		cs = m_pUcData->m_BkName1;
		if(bG_InvNo == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(rno, cs);
		rno++;
		// 07 支店名
		cs = m_pUcData->m_BkName2;
		if(bG_InvNo == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(rno, cs);
		rno++;
		// 08 金額
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Val);
		rno++;
		// 09 割引銀行名等
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Bk2Name1);
		rno++;
		// 10 支店名
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Bk2Name2);
		rno++;
		// 11 摘要１
		// 12 摘要２
		strTempAry.RemoveAll();
		cs = m_pUcData->m_Teki;
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
					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_021_TEKI);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,MAX_KETA_021_TEKI);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
				}
				strTempAry.RemoveAll();
			}
		}
		m_clsFunc.StrDivision(cs,&strTempAry,2);
		pstrCsvDataAry->SetAt(rno,strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt(rno,strTempAry.GetAt(1));
		rno++;
// 登録番号（法人番号）_23/11/21 cor <--
	}
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	// [科目]の場合、科目のみデータを取得
//	else if (strCsvId.Compare(CSV_ID_KAMOKU) == 0) {
//		// 01 科目
//		pstrCsvDataAry->SetAt(1, m_pUcData->m_KnName);
//	}
//// midori 190505 add <--
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	// [小計]の場合、名称のみデータを取得
	else if (strCsvId.Compare(CSV_ID_SYOKEI) == 0) {
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,0) == 1) {
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
// midori 157046 add <--
			m_clsFunc.StrDivision( m_pUcData->m_KeiStr, &strArray, 2 );
			strkei = strArray.GetAt( 0 );
			strkei2 = strArray.GetAt( 1 );
// 登録番号（法人番号）_23/11/21 cor -->
			//if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(2, strkei);
			//if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(3, strkei2);
// -------------------------------------
			rno=2;
// 24/03/04_ScoreLink cor -->
			//if(bG_InvNo == TRUE)	{
// --------------------------
			if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/03/04_ScoreLink cor <--
				rno++;
			}
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei);
			rno++;
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei2);
// 登録番号（法人番号）_23/11/21 cor <--
		}
	}
// midori 157046 add -->
	// [中計]の場合、名称のみデータを取得
	else if (strCsvId.Compare(CSV_ID_CHUKEI) == 0) {
		if(m_KmkRowSgn == 1) {
			m_clsFunc.StrDivision( m_pUcData->m_KeiStr, &strArray, 2 );
			strkei = strArray.GetAt( 0 );
			strkei2 = strArray.GetAt( 1 );
// 登録番号（法人番号）_23/11/21 cor -->
			//if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(2, strkei);
			//if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(3, strkei2);
// -------------------------------------
			rno=2;
// 24/03/04_ScoreLink cor -->
			//if(bG_InvNo == TRUE)	{
// --------------------------
			if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/03/04_ScoreLink cor <--
				rno++;
			}
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei);
			rno++;
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei2);
// 登録番号（法人番号）_23/11/21 cor <--
		}
	}
// midori 157046 add <--
	// [科目]の場合、科目のみデータを取得
	else if (strCsvId.Compare(CSV_ID_KAMOKU) == 0) {
// midori 157046 del -->
		//if(ImpKamokuRowEnableSgn(m_pDB,0) == 1) {
// midori 157046 del <--
// midori 157046 add -->
		if(m_KmkRowSgn == 1) {
// midori 157046 add <--
			// 科目行名称
			m_clsFunc.StrDivision( m_pUcData->m_KeiStr, &strArray, 2 );
			strkei = strArray.GetAt( 0 );
			strkei2 = strArray.GetAt( 1 );
// 登録番号（法人番号）_23/11/21 cor -->
			//if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(2, strkei);
			//if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(3, strkei2);
// -------------------------------------
			rno=2;
// 24/03/04_ScoreLink cor -->
			//if(bG_InvNo == TRUE)	{
// --------------------------
			if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/03/04_ScoreLink cor <--
				rno++;
			}
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei);
			rno++;
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei2);
// 登録番号（法人番号）_23/11/21 cor <--
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
void	CimpUc021Uketoritegata::virTableEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	//CdbUc021Uketoritegata*	rs = (CdbUc021Uketoritegata*)rsData;
	
	switch (nType) {
	case 0:
		// ソートの一括集計処理 ･･･ 科目の無い帳表はいらない
		m_pUcData->m_AdName1 = strName;			// 名称：金融機関名
		m_pUcData->m_Val = uCalcKei.strKei[0];		// 金額："0:Val"固定
		break;
	case 1:
		// 一括金額参照の戻す処理
		m_pUcData->m_AdName1 = strName;			// 名称：金融機関名
		m_pUcData->m_Val = uCalcKei.strKei[0];		// 金額："0:Val"固定
		break;
	case 2:
		// 特殊行挿入の一括金額選択時
		m_pUcData->m_AdName1 = strName;			// 名称：金融機関名
		break;
	}
}

// 157766 del -->
////********************************************************************************
////	「科目」が追加されておらず、かつ正しい項目のCSVデータかチェックする
////		IN		pstrCsvDataAry			CSVの1行分のデータ
////
////		RET		TRUE : 科目が追加されておらず、かつ項目が正しくない
////				FALSE : 上記以外
////********************************************************************************
//int CimpUc021Uketoritegata::virFormatedIsNoKamokuVer(CStringArray* pstrCsvDataAry)
//{
//	BOOL	nRet = TRUE;
//	CString strItem;
//
//	for(int i=1/*[項目]読み飛ばし*/; i<pstrCsvDataAry->GetCount(); i++){
//		strItem.Empty();
//		switch(i){
//			case 0:
//				break;
//			case 1:
//				strItem		+= CSV_021_ITEM_02;	break;
//			case 2:
//				strItem		+= CSV_021_ITEM_03;	break;
//			case 3:
//				strItem		+= CSV_021_ITEM_04;	break;
//			case 4:
//				strItem		+= CSV_021_ITEM_05;	break;
//			case 5:
//				strItem		+= CSV_021_ITEM_06;	break;
//			case 6:
//				strItem		+= CSV_021_ITEM_07;	break;
//			case 7:
//				strItem		+= CSV_021_ITEM_08;	break;
//			case 8:
//				strItem		+= CSV_021_ITEM_09;	break;
//			case 9:
//				strItem		+= CSV_021_ITEM_10;	break;
//			case 10:
//				strItem		+= CSV_021_ITEM_11;	break;
//			case 11:
//				strItem		+= CSV_021_ITEM_12;	break;
//		}
//		CString hoge = pstrCsvDataAry->GetAt(i);
//
//		if( pstrCsvDataAry->GetAt(i).Compare(strItem) != 0 ){
//			nRet = FALSE;
//			break;
//		}
//	}
//
//	return nRet;
//}
// 157766 del <--

// 210113_5301_1 del -->
// ※未使用のため削除
////********************************************************************************
////	「科目」が追加おり、かつ正しい項目のCSVデータかチェックする
////		IN		pstrCsvDataAry			CSVの1行分のデータ
////
////		RET		TRUE : 科目が追加されており、かつ項目が正しくない
////				FALSE : 上記以外
////********************************************************************************
//int CimpUc021Uketoritegata::virFormatedIsKamokuAddedVer(CStringArray* pstrCsvDataAry)
//{
//	BOOL	nRet = TRUE;
//	CString strItem;
//
//	for(int i=1/*[項目]読み飛ばし*/; i<pstrCsvDataAry->GetCount(); i++){
//		strItem.Empty();
//		switch(i){
//			case 0:
//				break;
//			case 1:
//				strItem		+= CSV_021_ITEM_01;	break;
//			case 2:
//				strItem		+= CSV_021_ITEM_02;	break;
//			case 3:
//				strItem		+= CSV_021_ITEM_03;	break;
//			case 4:
//				strItem		+= CSV_021_ITEM_04;	break;
//			case 5:
//				strItem		+= CSV_021_ITEM_05;	break;
//			case 6:
//				strItem		+= CSV_021_ITEM_06;	break;
//			case 7:
//				strItem		+= CSV_021_ITEM_07;	break;
//			case 8:
//				strItem		+= CSV_021_ITEM_08;	break;
//			case 9:
//				strItem		+= CSV_021_ITEM_09;	break;
//			case 10:
//				strItem		+= CSV_021_ITEM_10;	break;
//			case 11:
//				strItem		+= CSV_021_ITEM_11;	break;
//			case 12:
//				strItem		+= CSV_021_ITEM_12;	break;
//		}
//		CString hoge = pstrCsvDataAry->GetAt(i);
//
//		if( pstrCsvDataAry->GetAt(i).Compare(strItem) != 0 ){
//			nRet = FALSE;
//			break;
//		}
//	}
//
//	return nRet;
//}
// 210113_5301_1 del <--

// midori 156937 del -->
//// midori 156189,156190,156191 add -->
//CString CimpUc021Uketoritegata::ImpGetSyokeiName(void)
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
