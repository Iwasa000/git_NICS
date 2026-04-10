#include "StdAfx.h"
#include "impUc081Siharaitegata.h"

// midori 190301 0415 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\prtConstData.h"

extern	BOOL	bG_Kanso;
// midori 190301 0415 add <--
// 登録番号（法人番号）_23/11/21 add -->
extern BOOL	bG_InvNo;
// 登録番号（法人番号）_23/11/21 add <--
// 24/02/20_ScoreLink add -->
extern BOOL	bG_Sl;
// 24/02/20_ScoreLink add <--

// 改良No.21-0086,21-0529 add -->
extern	int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 add <--


// コンストラクタ
CimpUc081Siharaitegata::CimpUc081Siharaitegata(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
	m_KmkRowSgn = 0;	// 改良No.21-0086,21-0529 add
}

// デストラクタ
CimpUc081Siharaitegata::~CimpUc081Siharaitegata(void)
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
int CimpUc081Siharaitegata::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_081;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
// 改良No.21-0086,21-0529 cor -->
//	m_nItemCount = CSV_081_ITEM_CNT;
//
//// 24/05/31_ScoreLink add -->
//	// Export + ScoreLink ならカウントを１アップ
//	// (科目追加対応を行ったときは CSV_081_ITEM_CNT を 11 にする)
//	if(m_IsExport == TRUE && bG_Sl == TRUE)	{
//		m_nItemCount = m_nItemCount + 1;
//	}
//// 24/05/31_ScoreLink add <--
// ------------------------------
	if(m_KamokuVer2 == 0 || (m_IsExport == TRUE && bG_Sl == TRUE))
		m_nItemCount = CSV_081_ITEM_CNT_K;
	else
		m_nItemCount = CSV_081_ITEM_CNT;
// 改良No.21-0086,21-0529 cor <--

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
	m_pUcCmn = (CdbUc000Common*)(new CdbUc081Siharaitegata(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );

// 改良No.21-0086,21-0529 add -->
	m_pDB = pDB;
	// 科目名称のクリア
	m_strKmName.Empty();
	// 科目行サインのクリア
	m_KmkRowSw = 0;
	// 「科目行を挿入する」有効/無効のサイン
	m_KmkRowSgn = ImpKamokuRowEnableSgn(m_pDB, 0, m_nFormSeq);
// 改良No.21-0086,21-0529 add <--

	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_081;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_081 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_081;


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
int CimpUc081Siharaitegata::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// No.200903 add -->
	int		len=0;
// No.200903 add <--
	int		kt=0;		// 改良No.21-0086,21-0529 add

	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
// midori 180404 del -->
//	BYTE			cFgFunc			= 0;			// 特殊行フラグ
// midori 180404 del <--

// midori 180404 del -->
//	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &cFgFunc);
// midori 180404 del <--
// midori 180404 add -->
	//int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc);				// 改良No.21-0086,21-0529 del
	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc, m_KmkRowSgn);	// 改良No.21-0086,21-0529 add
// midori 180404 add <--
	// エラーなら終了
	if (nRet != ID_FGFUNC_NULL) {
		return nRet;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc081Siharaitegata*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

// 改良No.21-0086,21-0529 add -->
	CsvInfoClr(&m_uImpCsvInfo);
// 改良No.21-0086,21-0529 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 日付は、ここでデータ加工も行う
	long			nTemp1			= 0;			// 数値テンポラリ
	long			nTemp2			= 0;			// 数値テンポラリ

// 改良No.21-0086,21-0529 cor -->
//// 登録番号（法人番号）_23/11/21 cor -->
//	//if(pstrCsvDataAry->GetAt(1).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(2) != TRUE)	{
//	//	pstrCsvDataAry->SetAt(1,pstrCsvDataAry->GetAt(2));
//	//	pstrCsvDataAry->SetAt(2,_T(""));
//	//}
//// -------------------------------------
//	int	ofset=1;
//
//	// 10 登録番号（法人番号）
//	if(m_NewVer2 == TRUE)	{
//		if(bG_InvNo == TRUE)	{
//			CString	cs=_T("");
//			cs = pstrCsvDataAry->GetAt(ofset);
//			if(CheckImportTno(cs) != TRUE)	{
//				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_042_ITEM_13);
//				m_uImpCsvInfo.nLength = 0;
//				m_uImpCsvInfo.nKind = 0;	// 0:文字
//				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//				CsvInfoClr(&m_uImpCsvInfo);
//				nRet = IMP_REC_NG;
//				// 登録番号（法人番号）は桁数までのみ取り込むことをしないので
//				// ここでクリアする
//				pstrCsvDataAry->SetAt(ofset,_T(""));
//			}
//			else	{
//				// 先頭が "T" ではない場合、半角スペースが挿入されているので再セット
//				// "T6210001025986" → "T6210001025986"
//				// "6210001025986"  → " 6210001025986"
//				pstrCsvDataAry->SetAt(ofset,cs);
//			}
//		}
//		ofset++;
//	}
//
//	if(pstrCsvDataAry->GetAt(ofset).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(ofset+1) != TRUE)	{
//		pstrCsvDataAry->SetAt(ofset,pstrCsvDataAry->GetAt(ofset+1));
//		pstrCsvDataAry->SetAt(ofset+1,_T(""));
//	}
//
//	// 01 支払先１
//// 登録番号（法人番号）_23/11/21 cor -->
//	//if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_081_ADNAME1) != TRUE) {
//// -------------------------------------
//	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_ADNAME1) != TRUE) {
//// 登録番号（法人番号）_23/11/21 cor <--
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_01);
//		m_uImpCsvInfo.nLength = MAX_KETA_081_ADNAME1/2;
//		m_uImpCsvInfo.nKind = 0;	// 0:文字
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//	}
//
//	// 02 支払先２
//// 登録番号（法人番号）_23/11/21 cor -->
//	//if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_081_ADNAME1) != TRUE) {
//// -------------------------------------
//	ofset++;
//	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_ADNAME1) != TRUE) {
//// 登録番号（法人番号）_23/11/21 cor <--
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_02);
//		m_uImpCsvInfo.nLength = MAX_KETA_081_ADNAME1/2;
//		m_uImpCsvInfo.nKind = 0;	// 0:文字
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//	}
//// 登録番号（法人番号）_23/11/21 cor <--
// ------------------------------
	int		ofset=1;
	int		sw=0;

	if(m_KamokuVer2 == 0)	{
		if(bG_InvNo == TRUE)	{
			// 11 科目
			if(CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_KAMOKU * 2) != TRUE)	{
				strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_11);
				m_uImpCsvInfo.nLength = MAX_KETA_081_KAMOKU;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
			}
		}
		ofset++;
	}

	// 10 登録番号（法人番号）
	if(m_NewVer2 == TRUE)	{
		if(bG_InvNo == TRUE)	{
			CString	cs=_T("");
			cs = pstrCsvDataAry->GetAt(ofset);
			if(CheckImportTno(cs) != TRUE)	{
				strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_10);
				m_uImpCsvInfo.nLength = 0;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
				// 登録番号（法人番号）は桁数までのみ取り込むことをしないので
				// ここでクリアする
				pstrCsvDataAry->SetAt(ofset, _T(""));
			}
			else	{
				// 先頭が "T" ではない場合、半角スペースが挿入されているので再セット
				// "T6210001025986" → "T6210001025986"
				// "6210001025986"  → " 6210001025986"
				pstrCsvDataAry->SetAt(ofset, cs);
			}
		}
		ofset++;
	}

	if(m_KmkRowSgn == 1)	{
		// 科目行
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_KAMOKU) == 0)	{
			// 支払先１、支払先２
			if(pstrCsvDataAry->GetAt(ofset) == _T("") && pstrCsvDataAry->GetAt(1+ofset) == _T(""))	{
				m_uImpCsvInfo.nErr = 4;
				return(IMP_REC_NG);
			}
		}
	}

	// 支払先１が空欄で、支払先２に文字列が入力されている場合は、２の内容を１に入れ直す
	if(pstrCsvDataAry->GetAt(ofset).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(1+ofset) != TRUE)	{
		pstrCsvDataAry->SetAt(ofset, pstrCsvDataAry->GetAt(1+ofset));
		pstrCsvDataAry->SetAt(1+ofset, _T(""));
	}

	if(m_KmkRowSgn == 0)	{
		kt = MAX_KETA_081_ADNAME1;
	}
	else	{
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_CHUKEI) == 0)	{
			kt = MAX_KETA_081_SYOKEI;
		}
		else	{
			kt = MAX_KETA_081_ADNAME1;
		}
	}

	// 01 支払先１
	if(CheckImportStr(pstrCsvDataAry->GetAt(ofset), kt) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_01);
		m_uImpCsvInfo.nLength = kt / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 02 支払先２
	ofset++;
	if(CheckImportStr(pstrCsvDataAry->GetAt(ofset), kt) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_02);
		m_uImpCsvInfo.nLength = kt / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// 改良No.21-0086,21-0529 cor <--

	// 03 振出年月日
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(3) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(3), &nTemp1) != TRUE) {
// -------------------------------------
	ofset++;
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nTemp1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_03);
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}
	}

	// 04 支払期日
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(4) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(4), &nTemp2) != TRUE) {
// -------------------------------------
	ofset++;
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nTemp2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_04);
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}
	}

	// 05 支払銀行名
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_081_BKNAME1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_BKNAME1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_05_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_05);
		m_uImpCsvInfo.nLength = MAX_KETA_081_BKNAME1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 06 支店名
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_081_BKNAME2*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_BKNAME2*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_06_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_06);
		m_uImpCsvInfo.nLength = MAX_KETA_081_BKNAME2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 07 金額
	if(bG_Kanso == TRUE)	len = ID_VAL_081_K_DATA;
	else					len = ID_VAL_081_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_07);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
// 登録番号（法人番号）_23/11/21 cor -->
		//if(pstrCsvDataAry->GetAt(7).Left(1) == '-')	{
// -------------------------------------
		if(pstrCsvDataAry->GetAt(ofset).Left(1) == '-')	{
// 登録番号（法人番号）_23/11/21 cor <--
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 08 摘要１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(8), MAX_KETA_081_TEKI) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_TEKI) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_08);
		m_uImpCsvInfo.nLength = MAX_KETA_081_TEKI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 09 摘要２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(9), MAX_KETA_081_TEKI) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_081_TEKI) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_081_ITEM_09);
		m_uImpCsvInfo.nLength = MAX_KETA_081_TEKI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}


// midori 180404 del -->
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 名称テーブルへ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	long			nAdSeq			= 0;				// 取引先シーケンス番号
//	long			nAdOrder		= 0;				// 取引先並び順
//	int				nAdRessGr		= ID_ADDRESSGR_KAI;	// 取引先グループ（買掛金）
//	long			nBkSeq			= 0;				// 金融機関シーケンス番号
//	long			nBkOrder		= 0;				// 金融機関並び順
//
//	// 01 支払先１
//	// 02 支払先２
//	if (CheckEntryAddress(	pstrCsvDataAry->GetAt(1), pstrCsvDataAry->GetAt(2),
//							nAdRessGr, &nAdSeq, &nAdOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	// 05 支払銀行名
//	// 06 支店名
//	if (CheckEntryBank(pstrCsvDataAry->GetAt(5), pstrCsvDataAry->GetAt(6), &nBkSeq, &nBkOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
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
//		// 01 支払先１
//		// 02 支払先２
//		m_pUcData->m_AdSeq		= nAdSeq;
//		m_pUcData->m_AdOrder	= nAdOrder;
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(2);
//// midori 152137 add -->
//		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//// midori 152137 add <--
//
//		// 03 振出年月日
//		m_pUcData->m_SpDate		= nTemp1;
//
//		// 04 支払期日
//		m_pUcData->m_PmDate		= nTemp2;
//
//		// 05 支払銀行名
//		// 06 支店名
//		m_pUcData->m_BkSeq		= nBkSeq;
//		m_pUcData->m_BkOrder	= nBkOrder;
//		m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(5);
//		m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(6);
//
//		// 07 金額
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 08 摘要１
//		// 09 摘要２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
//		m_pUcData->m_Teki		= strTemp;
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
int CimpUc081Siharaitegata::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// No.200903 add -->
	int				len=0;
// No.200903 add <--
	int				kt=0;					// 改良No.21-0086,21-0529 add
	CString			strTemp= _T("");		// 文字列テンポラリ
// midori 190301 0415 add -->
	CString			strTemp1=_T("");
	CString			strTemp2=_T("");
	CString			strTemp3=_T("");
	CString			strTemp4=_T("");
// midori 190301 0415 add <--
// 登録番号（法人番号）_23/11/21 add -->
	int				ofset=1;
	int				save=0;
	CString			cs=_T(""),cs1=_T(""),cs2=_T(""),cs3=_T("");
// 登録番号（法人番号）_23/11/21 add <--
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--
// 改良No.21-0086,21-0529 add -->
	CString			strkei1=_T("");		// CSVに入力された小計名称
	CString			strkei2=_T("");		// 自動作成の小計名称
// 改良No.21-0086,21-0529 add <--
// midori 190504 add -->
	CUcFunctionCommon	ufc;
// midori 190504 add <--

	// 日付は、ここでデータ加工も行う
	long			nTemp1			= 0;			// 数値テンポラリ
	long			nTemp2			= 0;			// 数値テンポラリ

	// 文字数をオーバーしていればカットする
// 改良No.21-0086,21-0529 add -->
	// 11 科目
	if(m_KamokuVer2 == 0)	{
		if(bG_InvNo == TRUE)	{
			GetSpritString(pstrCsvDataAry->GetAt(ofset), &strTemp, MAX_KETA_081_KAMOKU*2);
			pstrCsvDataAry->SetAt(ofset, strTemp);
		}
		ofset++;
	}
// 改良No.21-0086,21-0529 add <--

// 登録番号（法人番号）_23/11/21 add -->
	// 10 登録番号（法人番号）
	if(m_NewVer2 == TRUE)	{
		ofset++;
	}
// 登録番号（法人番号）_23/11/21 add <--

// 改良No.21-0086,21-0529 cor -->
//	// 01 支払先１
//// 登録番号（法人番号）_23/11/21 cor -->
//	//GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_081_ADNAME1);
//	//strTemp = ufc.DeleteRightSpace(strTemp);
//	//pstrCsvDataAry->SetAt(1,strTemp);
//// -------------------------------------
//	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_081_ADNAME1);
//	strTemp = ufc.DeleteRightSpace(strTemp);
//	pstrCsvDataAry->SetAt(ofset,strTemp);
//	ofset++;
//// 登録番号（法人番号）_23/11/21 cor <--
//
//	// 02 支払先２
//// 登録番号（法人番号）_23/11/21 cor -->
//	//GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_081_ADNAME1);
//	//strTemp = ufc.DeleteRightSpace(strTemp);
//	//pstrCsvDataAry->SetAt(2,strTemp);
//// -------------------------------------
//	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_081_ADNAME1);
//	strTemp = ufc.DeleteRightSpace(strTemp);
//	pstrCsvDataAry->SetAt(ofset,strTemp);
//	ofset++;
//// 登録番号（法人番号）_23/11/21 cor <--
// ------------------------------
	//if(m_KmkRowSgn == 0) {
	if(m_KmkRowSgn == 0 || m_KamokuVer2 == 1)	{
		kt = MAX_KETA_081_ADNAME1;
	}
	else	{
		if(pstrCsvDataAry->GetAt(0).Compare(CSV_ID_SYOKEI) == 0 || pstrCsvDataAry->GetAt(0).Compare(CSV_ID_CHUKEI) == 0)	{
			kt = MAX_KETA_081_SYOKEI;
		}
		else {
			kt = MAX_KETA_081_ADNAME1;
		}
	}

	// 01 支払先１
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,kt);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;

	// 02 支払先２
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,kt);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 改良No.21-0086,21-0529 cor <--

	// 03 振出年月日
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(3) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(3), &nTemp1) != TRUE) {
	//		strTemp.Empty();
	//		pstrCsvDataAry->SetAt(3,strTemp);
	//	}
	//}
// -------------------------------------
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nTemp1) != TRUE) {
			strTemp.Empty();
			pstrCsvDataAry->SetAt(ofset,strTemp);
		}
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 04 支払期日
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(4) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(4), &nTemp2) != TRUE) {
	//		strTemp.Empty();
	//		pstrCsvDataAry->SetAt(4,strTemp);
	//	}
	//}
// -------------------------------------
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nTemp2) != TRUE) {
			strTemp.Empty();
			pstrCsvDataAry->SetAt(ofset,strTemp);
		}
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 05 支払銀行名
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_081_BKNAME1*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(5,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_081_BKNAME1*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
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
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 06 支店名
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_081_BKNAME2*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(6,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_081_BKNAME2*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
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
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 07 金額
	if(bG_Kanso == TRUE)	len = ID_VAL_081_K_DATA;
	else					len = ID_VAL_081_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE)	{
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(7,strTemp);
	//}
// -------------------------------------
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 08 摘要１
	// 09 摘要２
// 登録番号（法人番号）_23/11/21 cor -->
	//strTemp1 = pstrCsvDataAry->GetAt(8);
	//strTemp2 = pstrCsvDataAry->GetAt(9);
	//strTemp3 = _T("");
	//strTemp4 = _T("");
	//if(bG_Kanso == TRUE)	{
	//	// ２行のうちいずれかが全角１５文字を超えている場合３行にする
	//	if(strTemp1.GetLength() > 30 || strTemp2.GetLength() > 30)	{
	//		strTemp = strTemp1 + strTemp2;
	//		strTemp2 = GetSpritString(strTemp,&strTemp1,30);
	//		strTemp = strTemp2;
	//		strTemp3 = GetSpritString(strTemp,&strTemp2,30);
	//		// １段目、２段目に半角文字が奇数バイト数分、入力されている場合
	//		// ３段目に半角文字が１バイト余分に付いているのでカットする
	//		strTemp = strTemp3;
	//		strTemp4 = GetSpritString(strTemp,&strTemp3,20);
	//	}
	//	// 11 摘要１
	//	pstrCsvDataAry->SetAt(8,strTemp1);
	//	// 12 摘要２
	//	pstrCsvDataAry->SetAt(9,strTemp2);
	//	// 13 摘要３(配列が作成されていないので拡張する)
	//	pstrCsvDataAry->InsertAt(10,strTemp3);
	//}
	//else	{
	//	// 11 摘要１
	//	GetSpritString(strTemp1,&strTemp,MAX_KETA_081_TEKI);
	//	pstrCsvDataAry->SetAt(8,strTemp);
	//	// 12 摘要２
	//	GetSpritString(strTemp2,&strTemp,MAX_KETA_081_TEKI);
	//	pstrCsvDataAry->SetAt(9,strTemp);
	//}
// -------------------------------------
	strTemp1 = pstrCsvDataAry->GetAt(ofset);
	strTemp2 = pstrCsvDataAry->GetAt(ofset+1);
	strTemp3 = _T("");
	strTemp4 = _T("");
	if(bG_Kanso == TRUE)	{
		// ２行のうちいずれかが全角１５文字を超えている場合３行にする
		if(strTemp1.GetLength() > 30 || strTemp2.GetLength() > 30)	{
			strTemp = strTemp1 + strTemp2;
			strTemp2 = GetSpritString(strTemp,&strTemp1,30);
			strTemp = strTemp2;
			strTemp3 = GetSpritString(strTemp,&strTemp2,30);
			// １段目、２段目に半角文字が奇数バイト数分、入力されている場合
			// ３段目に半角文字が１バイト余分に付いているのでカットする
			strTemp = strTemp3;
			strTemp4 = GetSpritString(strTemp,&strTemp3,20);
		}
		// 11 摘要１
		pstrCsvDataAry->SetAt(ofset,strTemp1);
		// 12 摘要２
		pstrCsvDataAry->SetAt(ofset+1,strTemp2);
		// 13 摘要３(配列が作成されていないので拡張する)
		pstrCsvDataAry->InsertAt(ofset+2,strTemp3);
	}
	else	{
		// 11 摘要１
		GetSpritString(strTemp1,&strTemp,MAX_KETA_081_TEKI);
		pstrCsvDataAry->SetAt(ofset,strTemp);
		// 12 摘要２
		GetSpritString(strTemp2,&strTemp,MAX_KETA_081_TEKI);
		pstrCsvDataAry->SetAt(ofset+1,strTemp);
	}
// 登録番号（法人番号）_23/11/21 cor <--


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
	m_pUcData = (CdbUc081Siharaitegata*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
// 改良No.21-0086,21-0529 add -->
	long			nKnSeq		= 0;				// 科目シーケンス番号
	long			nKnOrder	= 0;				// 科目並び順
// 改良No.21-0086,21-0529 add <--
	long			nAdSeq		= 0;				// 取引先シーケンス番号
	long			nAdOrder	= 0;				// 取引先並び順
	int				nAdRessGr	= ID_ADDRESSGR_KAI;	// 取引先グループ（買掛金）
	long			nBkSeq		= 0;				// 金融機関シーケンス番号
	long			nBkOrder	= 0;				// 金融機関並び順

// 改良No.21-0086,21-0529 cor -->
//	// 01 支払先１
//	// 02 支払先２
//// 登録番号（法人番号）_23/11/21 cor -->
//	//if (CheckEntryAddress(	pstrCsvDataAry->GetAt(1), pstrCsvDataAry->GetAt(2),
//	//						nAdRessGr, &nAdSeq, &nAdOrder) != TRUE) {
//	//	return IMP_REC_NG;
//	//}
//// -------------------------------------
//	ofset=1;
//	cs3 = _T("");
//	if(m_NewVer2 == TRUE)	{
//		if(bG_InvNo == TRUE)	{
//			cs3 = pstrCsvDataAry->GetAt(ofset);
//// 231222_168474 add -->
//			if(CheckImportTno(cs3) != TRUE)	{
//				cs3 = _T("");
//				pstrCsvDataAry->SetAt(ofset,cs3);
//			}
//// 231222_168474 add <--
//		}
//		ofset++;
//	}
//	cs1 = pstrCsvDataAry->GetAt(ofset);
//	cs2 = pstrCsvDataAry->GetAt(ofset+1);
//	if (CheckEntryAddress(cs1,cs2,
//							nAdRessGr,&nAdSeq,&nAdOrder,cs3) != TRUE) {
//		return IMP_REC_NG;
//	}
//// 登録番号（法人番号）_23/11/21 cor <--
// ------------------------------
	ofset = 1;

	// 11 科目
	if(m_KamokuVer2 == 0)	{
		if(bG_InvNo == TRUE)	{
			if(CheckEntryKamoku(pstrCsvDataAry->GetAt(ofset), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE)	{
				return IMP_REC_NG;
			}
		}
		ofset++;
	}

	// 10 登録番号（法人番号）
	cs3 = _T("");
	if(m_NewVer2 == TRUE)	{
		if(bG_InvNo == TRUE)	{
			cs3 = pstrCsvDataAry->GetAt(ofset);
			if(CheckImportTno(cs3) != TRUE)	{
				cs3 = _T("");
				pstrCsvDataAry->SetAt(ofset, cs3);
			}
		}
		ofset++;
	}

	// 01 支払先１
	// 02 支払先２
	cs1 = pstrCsvDataAry->GetAt(ofset);
	cs2 = pstrCsvDataAry->GetAt(ofset+1);
	if((strCsvId.Compare(CSV_ID_SYOKEI) != 0) && (strCsvId.Compare(CSV_ID_CHUKEI) != 0) && (strCsvId.Compare(CSV_ID_KAMOKU) != 0))	{
		if(CheckEntryAddress(cs1, cs2, nAdRessGr, &nAdSeq, &nAdOrder, cs3) != TRUE)	{
			return IMP_REC_NG;
		}
	}
// 改良No.21-0086,21-0529 cor <--

	// 05 支払銀行名
	// 06 支店名
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckEntryBank(pstrCsvDataAry->GetAt(5), pstrCsvDataAry->GetAt(6), &nBkSeq, &nBkOrder) != TRUE) {
	//	return IMP_REC_NG;
	//}
// -------------------------------------
	cs1 = pstrCsvDataAry->GetAt(ofset+4);
	cs1.Replace(PRT_FORMAT_CHCODE,_T(""));
	cs2 = pstrCsvDataAry->GetAt(ofset+5);
	cs2.Replace(PRT_FORMAT_CHCODE,_T(""));
	if (CheckEntryBank(cs1, cs2, &nBkSeq, &nBkOrder) != TRUE) {
		return IMP_REC_NG;
	}
// 登録番号（法人番号）_23/11/21 cor <--

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
// 登録番号（法人番号）_23/11/21 cor -->
		//// 01 支払先１
		//// 02 支払先２
		//m_pUcData->m_AdSeq		= nAdSeq;
		//m_pUcData->m_AdOrder	= nAdOrder;
		//m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
		//m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(2);
		//m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(1));

		//// 03 振出年月日
		//m_pUcData->m_SpDate		= nTemp1;

		//// 04 支払期日
		//m_pUcData->m_PmDate		= nTemp2;

		//// 05 支払銀行名
		//// 06 支店名
		//m_pUcData->m_BkSeq		= nBkSeq;
		//m_pUcData->m_BkOrder	= nBkOrder;
		//m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(5);
		//m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(6);

		//// 07 金額
		//m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
		//RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		//// 08 摘要１
		//// 09 摘要２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
		//if(bG_Kanso == TRUE)	{
		//	if(pstrCsvDataAry->GetAt(10).IsEmpty() == FALSE)	{
		//		strTemp = m_clsFunc.StrDocking(strTemp,pstrCsvDataAry->GetAt(10));
		//	}
		//}
		//m_pUcData->m_Teki		= strTemp;
// -------------------------------------
		ofset=1;

// 改良No.21-0086,21-0529 add -->
		// 11 科目
		if(m_KamokuVer2 == 0)	{
			if(bG_InvNo == TRUE)	{
				m_pUcData->m_KnSeq		= nKnSeq;
				m_pUcData->m_KnOrder	= nKnOrder;
				m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(ofset);
				m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(ofset));
			}
			ofset++;
		}
// 改良No.21-0086,21-0529 add <--

		// 10 登録番号（法人番号）
		if(m_NewVer2 == TRUE)	{
			if(bG_InvNo == TRUE)	{
				m_pUcData->m_InvNo = pstrCsvDataAry->GetAt(ofset);
			}
			ofset++;
		}
		save=ofset;

		// 01 支払先１
		// 02 支払先２
		m_pUcData->m_AdSeq		= nAdSeq;
		m_pUcData->m_AdOrder	= nAdOrder;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(ofset);
		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(save));
		ofset++;

		// 03 振出年月日
		m_pUcData->m_SpDate		= nTemp1;
		ofset++;

		// 04 支払期日
		m_pUcData->m_PmDate		= nTemp2;
		ofset++;

		// 05 支払銀行名
		// 06 支店名
		m_pUcData->m_BkSeq		= nBkSeq;
		m_pUcData->m_BkOrder	= nBkOrder;
		m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(ofset);
		ofset++;

		// 07 金額
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
		ofset++;

		// 08 摘要１
		// 09 摘要２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		if(bG_Kanso == TRUE)	{
			//if(pstrCsvDataAry->GetAt(10).IsEmpty() == FALSE)	{		// 改良No.21-0086,21-0529 del
			if(pstrCsvDataAry->GetAt(ofset+2).IsEmpty() == FALSE)	{	// 改良No.21-0086,21-0529 add
				strTemp = m_clsFunc.StrDocking(strTemp,pstrCsvDataAry->GetAt(ofset+2));
			}
		}
		m_pUcData->m_Teki		= strTemp;
// 登録番号（法人番号）_23/11/21 cor <--
		break;

// 改良No.21-0086,21-0529 add -->
	case	ID_FGFUNC_KAMOKU:		// 科目行
		//if(bG_InvNo == TRUE && m_KmkRowSgn == 1) {
		if(m_KamokuVer2 == 0 && bG_InvNo == TRUE && m_KmkRowSgn == 1) {
			ofset = 1;
			m_pUcData->m_KeiStr = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2 + ofset), pstrCsvDataAry->GetAt(3 + ofset));
		}
		break;
// 改良No.21-0086,21-0529 add <--

	case	ID_FGFUNC_SYOKEI:		// 小計行
// 改良No.21-0086,21-0529 cor -->
		//m_pUcData->m_KeiStr		= m_strSyokeiName;
// ------------------------------
		if(bG_InvNo == TRUE && m_KmkRowSgn == 1)	{
			ofset = 1;
			m_pUcData->m_KeiStr = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2 + ofset), pstrCsvDataAry->GetAt(3 + ofset));
			if(m_pUcData->m_KeiStr.IsEmpty() == TRUE) {
				// 何も入力されていなければ
				// 科目行あり　→　【小計】
				// 科目行なし　→　【小計(科目名)】
				// を自動でセットする
				GetKamokuKeiStrMain(m_nFormSeq, m_KmkRowSw, _T(""), m_strSyokeiName, m_strKmName, &strkei2);
				m_pUcData->m_KeiStr = strkei2;
			}
		}
		else {
			m_pUcData->m_KeiStr = m_strSyokeiName;
		}
// 改良No.21-0086,21-0529 cor <--
		break;

	case	ID_FGFUNC_CHUKEI:		// 中計行
// 改良No.21-0086,21-0529 cor -->
		//m_pUcData->m_KeiStr		= m_strChukeiName;
// ------------------------------
		if(bG_InvNo == TRUE && m_KmkRowSgn == 1) {
			ofset = 1;
			strkei1 = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2 + ofset), pstrCsvDataAry->GetAt(3 + ofset));
			strkei2.Empty();
			if(strkei1.IsEmpty() == TRUE) {
				strkei2 = m_strChukeiName;
			}
			else {
				if(strkei1.Compare(m_strChukeiName) == 0) {
					strkei2 = m_strChukeiName;
				}
				else {
					strkei2 = strkei1;
				}
			}
			m_pUcData->m_KeiStr = strkei2;
		}
		else	{
			m_pUcData->m_KeiStr = m_strChukeiName;
		}
// 改良No.21-0086,21-0529 cor <--
		break;
	}

	// レコード登録
	m_pUcData->Update();

// 改良No.21-0086,21-0529 add -->
	if(bG_InvNo == TRUE && m_KamokuVer2 == 0)	{
		if(m_FgFunc == ID_FGFUNC_DATA) {
			// 同一グループ内の先頭名称を使用する
			if(m_strKmName.IsEmpty() == TRUE) {
				m_strKmName = pstrCsvDataAry->GetAt(1);
			}
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
	}
// 改良No.21-0086,21-0529 add <--

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
int CimpUc081Siharaitegata::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	CString			strCsvId		= _T("");		// CSVの識別子
// midori 190301 0415 add -->
	CString				cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
// 改良No.21-0086,21-0529 add -->
	CString				strkei=_T("");
	CString				strkei2=_T("");
	CStringArray		strArray;
// 改良No.21-0086,21-0529 add <--
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
	m_pUcData = (CdbUc081Siharaitegata*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CStringArray	strTempAry;						// 文字列配列テンポラリ
	CString			strTemp			= _T("");		// 文字列テンポラリ
// 登録番号（法人番号）_23/11/21 cor -->
	int				rno=0;
// 登録番号（法人番号）_23/11/21 cor <--

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// [明細]の場合は、各項目のデータを取得
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
// 登録番号（法人番号）_23/11/21 cor -->
//		// 01 支払先１
//		pstrCsvDataAry->SetAt(1, m_pUcData->m_AdName1);
//
//		// 02 支払先２
//		pstrCsvDataAry->SetAt(2, m_pUcData->m_AdName2);
//
//		// 03 振出年月日
//		strTemp = _T("");
//		ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strTemp);
//		pstrCsvDataAry->SetAt(3, strTemp);
//
//		// 04 支払期日
//		strTemp = _T("");
//		ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_PmDate, &strTemp);
//		pstrCsvDataAry->SetAt(4, strTemp);
//
//		// 05 支払銀行名
//		pstrCsvDataAry->SetAt(5, m_pUcData->m_BkName1);
//
//		// 06 支店名
//		pstrCsvDataAry->SetAt(6, m_pUcData->m_BkName2);
//
//		// 07 金額
//		pstrCsvDataAry->SetAt(7, m_pUcData->m_Val);
//
//		// 08 摘要１
//		// 09 摘要２
//		strTempAry.RemoveAll();
//// midori 190301 0415 del -->
//		//m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
//		//pstrCsvDataAry->SetAt(8, strTempAry.GetAt(0));
//		//pstrCsvDataAry->SetAt(9, strTempAry.GetAt(1));
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
//					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_081_TEKI);
//					cs = cs2;
//					cs3 = ufc.GetSpritString(cs,&cs2,MAX_KETA_081_TEKI);
//					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
//				}
//				strTempAry.RemoveAll();
//			}
//		}
//		m_clsFunc.StrDivision(cs,&strTempAry,2);
//		pstrCsvDataAry->SetAt(8,strTempAry.GetAt(0));
//		pstrCsvDataAry->SetAt(9,strTempAry.GetAt(1));
//// midori 190301 0415 add <--
// -------------------------------------
		rno=1;

// 24/05/31_ScoreLink add -->
		// 科目
// 改良No.21-0086,21-0529 cor -->
		//if(bG_Sl == TRUE)	{
		//	pstrCsvDataAry->SetAt(rno, _T(""));
// ------------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_KnName);
// 改良No.21-0086,21-0529 cor <--
			rno++;
		}
// 24/05/31_ScoreLink add <--

// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 10 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}
		// 01 支払先１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName1);
		rno++;
		// 02 支払先２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName2);
		rno++;
		// 03 振出年月日
		strTemp = _T("");
		ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strTemp);
		pstrCsvDataAry->SetAt(rno, strTemp);
		rno++;
		// 04 支払期日
		strTemp = _T("");
		ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_PmDate, &strTemp);
		pstrCsvDataAry->SetAt(rno, strTemp);
		rno++;
		// 05 支払銀行名
		cs = m_pUcData->m_BkName1;
		if(bG_InvNo == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(rno, cs);
		rno++;
		// 06 支店名
		cs = m_pUcData->m_BkName2;
		if(bG_InvNo == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(rno, cs);
		rno++;
		// 07 金額
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Val);
		rno++;
		// 08 摘要１
		// 09 摘要２
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
					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_081_TEKI);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,MAX_KETA_081_TEKI);
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
// 改良No.21-0086,21-0529 add -->
	// [小計]、[中計]の場合は、名称のみデータを取得
	// [科目]の場合は、科目のみデータを取得
	else if(strCsvId.Compare(CSV_ID_SYOKEI) == 0 || strCsvId.Compare(CSV_ID_CHUKEI) == 0 || strCsvId.Compare(CSV_ID_KAMOKU) == 0)	{
		if((bG_InvNo == TRUE || bG_Sl == TRUE) && m_KmkRowSgn == 1)	{
			m_clsFunc.StrDivision(m_pUcData->m_KeiStr, &strArray, 2);
			strkei = strArray.GetAt(0);
			strkei2 = strArray.GetAt(1);

			rno = 3;
			if(strkei.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei);
			rno++;
			if(strkei2.IsEmpty() == FALSE)		pstrCsvDataAry->SetAt(rno, strkei2);
		}
	}
// 改良No.21-0086,21-0529 add <--

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
void	CimpUc081Siharaitegata::virTableEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	//CdbUc081Siharaitegata*	rs = (CdbUc081Siharaitegata*)rsData;

	switch (nType) {
	case 0:
		// ソートの一括集計処理
		m_pUcData->m_AdName1 = strName;				// 名称：支払先(下)
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case 1:
		// 一括金額参照の戻す処理
		m_pUcData->m_AdName1 = strName;				// 名称：支払先(下)
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case 2:
		// 特殊行挿入の一括金額選択時
		m_pUcData->m_AdName1 = strName;				// 名称：金融機関名
		break;
	}
}