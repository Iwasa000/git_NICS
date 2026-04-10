#include "StdAfx.h"
#include "impUc131Uriagedaka.h"

// midori 190301 0415 add -->
extern	BOOL	bG_Kanso;
// midori 190301 0415 add <--
// 24/02/20_ScoreLink add -->
extern BOOL	bG_Sl;
// 24/02/20_ScoreLink add <--

// コンストラクタ
CimpUc131Uriagedaka::CimpUc131Uriagedaka(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc131Uriagedaka::~CimpUc131Uriagedaka(void)
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
int CimpUc131Uriagedaka::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_131;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_131_ITEM_CNT;

// 210113_5301_1 add -->
// 24/02/20_ScoreLink cor -->
	//if(m_IsExport == TRUE && bG_Kanso == TRUE)	{
// --------------------------
	if(m_IsExport == TRUE && bG_Kanso == TRUE && bG_Sl == FALSE)	{
// 24/02/20_ScoreLink cor <--
		// エクスポート時、新様式なら項目数を減らしておく
		// (インポートは新旧両方のレイアウトに対応するため、ここでは決定できない)
		m_nItemCount = m_nItemCount - 1;
	}
// 210113_5301_1 add <--

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc131Uriagedaka(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );


	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_131;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_131 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_131;


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
int CimpUc131Uriagedaka::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// No.200903 add -->
	int		len=0;
// No.200903 add <--

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
	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc);
// midori 180404 add <--
	// エラーなら終了
	if (nRet != ID_FGFUNC_NULL) {
		return nRet;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc131Uriagedaka*)m_pUcCmn;


// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 事業所の名称
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_131_ADNAME1*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_01);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_ADNAME1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 02 所在地１
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_131_ADADD1*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_02);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 03 所在地２
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_131_ADADD1*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_03);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 04 責任者氏名１
	if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_131_HDNAME) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_04);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_HDNAME/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 05 責任者氏名２
	if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_131_HDNAME) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_05);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_HDNAME/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 06 代表者との関係
	if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_131_KANKEI*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_06);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_KANKEI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 07 事業等の内容１
	if (CheckImportStr(pstrCsvDataAry->GetAt(7), MAX_KETA_131_NAIYOU*2/3) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_07);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_NAIYOU/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 08 事業等の内容２
	if (CheckImportStr(pstrCsvDataAry->GetAt(8), MAX_KETA_131_NAIYOU*2/3) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_08);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_NAIYOU/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 09 事業等の内容３
	if (CheckImportStr(pstrCsvDataAry->GetAt(9), MAX_KETA_131_NAIYOU*2/3) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_09);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_NAIYOU/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

// No.200903 del -->
//	// 10 売上高
////	if (CheckImportNum(pstrCsvDataAry->GetAt(10), ID_VAL_131_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(10), ID_VAL_131_DATA, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(10), ID_VAL_131_DATA, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_10);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL_131_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(10).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
//
//	// 11 期末棚卸高
////	if (CheckImportNum(pstrCsvDataAry->GetAt(11), ID_VAL2_131_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(11), ID_VAL2_131_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), ID_VAL2_131_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_11);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL2_131_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(11).Left(1) == '-')	{
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
	// 10 売上高
	if(bG_Kanso == TRUE)	len = ID_VAL_131_K_DATA;
	else					len = ID_VAL_131_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(10), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_10);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(10).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 11 期末棚卸高
	if(bG_Kanso == TRUE)	len = ID_VAL2_131_K_DATA;
	else					len = ID_VAL2_131_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(11), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_11);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(11).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

	// 12 期末従業員数
//	if (CheckImportNum(pstrCsvDataAry->GetAt(12), ID_WORKER_131_DATA) != TRUE) {
//	if (CheckImportNum(pstrCsvDataAry->GetAt(12), ID_WORKER_131_DATA, FALSE) != TRUE) {			// [2009/07/09 Minus Value Support]
	if (CheckImportNum2(pstrCsvDataAry->GetAt(12), ID_WORKER_131_DATA, FALSE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_12);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = ID_WORKER_131_DATA;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(12).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

// 210113_5301_1 del -->
//// midori 190301 0415 del -->
////	// 13 使用建物の延面積
//////	if (CheckImportNum(pstrCsvDataAry->GetAt(13), ID_AREA_131_DATA, 2) != TRUE) {
//////	if (CheckImportNum(pstrCsvDataAry->GetAt(13), ID_AREA_131_DATA, 2, FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
////	if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_AREA_131_DATA, 2, FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
////		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_13);
////// midori 150895 cor -->
////		//return IMP_REC_NG;
////// ---------------------
////		m_uImpCsvInfo.nLength	= ID_AREA_131_DATA;	// 整数最大桁数
////		m_uImpCsvInfo.nLength2	= 2;	// 小数点以下最大桁数
////		m_uImpCsvInfo.nKind = 2;		// 2:面積等
////		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
////		CsvInfoClr(&m_uImpCsvInfo);
////		nRet = IMP_REC_NG;
////// midori 150895 cor <--
////	}
//// midori 190301 0415 del <--
//// midori 190301 0415 add -->
//	if(bG_Kanso != TRUE) {
//		// 13 使用建物の延面積
//		if(CheckImportNum2(pstrCsvDataAry->GetAt(13),ID_AREA_131_DATA,2,FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
//			strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_131_ITEM_13);
//			m_uImpCsvInfo.nLength	= ID_AREA_131_DATA;	// 整数最大桁数
//			m_uImpCsvInfo.nLength2	= 2;	// 小数点以下最大桁数
//			m_uImpCsvInfo.nKind = 2;		// 2:面積等
//			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//			CsvInfoClr(&m_uImpCsvInfo);
//			nRet = IMP_REC_NG;
//		}
//	}
//// midori 190301 0415 add <--
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	int	ofset=13;
	if(m_NewVer != TRUE) {
		// 13 使用建物の延面積
		if(bG_Kanso != TRUE)	{
			if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset),ID_AREA_131_DATA,2,FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_131_ITEM_13);
				m_uImpCsvInfo.nLength	= ID_AREA_131_DATA;	// 整数最大桁数
				m_uImpCsvInfo.nLength2	= 2;	// 小数点以下最大桁数
				m_uImpCsvInfo.nKind = 2;		// 2:面積等
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
			}
			ofset++;
		}
	}
// 210113_5301_1 add <--

	// 14 源泉所得税納付署１
// 210113_5301_1 del -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(14), MAX_KETA_131_HDNAME2) != TRUE) {
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_131_HDNAME2) != TRUE) {
// 210113_5301_1 add <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_14);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_HDNAME2/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}
// 210113_5301_1 add -->
	ofset++;
// 210113_5301_1 add <--

	// 15 源泉所得税納付署２
// 210113_5301_1 del -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(15), MAX_KETA_131_HDNAME2) != TRUE) {
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_131_HDNAME2) != TRUE) {
// 210113_5301_1 add <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_15);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_HDNAME2/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}
// 210113_5301_1 add -->
	ofset++;
// 210113_5301_1 add <--

	// 16 摘要１
// 210113_5301_1 del -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(16), MAX_KETA_131_TEKI*2/3) != TRUE) {
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_131_TEKI*2/3) != TRUE) {
// 210113_5301_1 add <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_16);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}
// 210113_5301_1 add -->
	ofset++;
// 210113_5301_1 add <--

	// 17 摘要２
// 210113_5301_1 del -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(17), MAX_KETA_131_TEKI*2/3) != TRUE) {
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_131_TEKI*2/3) != TRUE) {
// 210113_5301_1 add <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_17);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}
// 210113_5301_1 add -->
	ofset++;
// 210113_5301_1 add <--

	// 18 摘要３
// 210113_5301_1 del -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(18), MAX_KETA_131_TEKI*2/3) != TRUE) {
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_131_TEKI*2/3) != TRUE) {
// 210113_5301_1 add <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_131_ITEM_18);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_131_TEKI/3;
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
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 名称テーブルへ登録
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 
	//// なし
	//// 


	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// データ加工
	//// テーブルへデータ登録
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//CString			strTemp			= _T("");		// 文字列テンポラリ
	//double			nTemp			= 0;			// 数値テンポラリ１

	//// 新規レコード作成
	//m_pUcData->CreateNewRecord();

	//// レコード編集開始
	//m_pUcData->Edit();

	//// XX 頁番号
	//// XX 行番号
	//CheckNumPageRow(&m_NumPage, &m_NumRow);
	//m_pUcData->m_NumPage		= m_NumPage;
	//m_pUcData->m_NumRow			= m_NumRow;

	//// 00 特殊行フラグ
	//m_pUcData->m_FgFunc			= cFgFunc;

	//// [明細]の場合のみ各項目のデータを登録
	//// [小計]、[中計]は、計文言と頁番号、行番号、特殊行フラグのみ
	//// 空行は、頁番号、行番号、特殊行フラグのみ
	//switch (cFgFunc) {
	//case	ID_FGFUNC_DATA:			// データ行
	//	// 01 事業所の名称
	//	// 02 所在地１
	//	// 03 所在地２
	//	//m_pUcData->m_AdSeq	= nAdSeq;		// 未使用
	//	//m_pUcData->m_AdOrder	= AdOrder;		// 未使用
	//	m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
	//	//m_pUcData->m_AdName2	= _T("");		// 未使用
	//	m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(2);
	//	m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(3);

	//	// 04 責任者氏名１
	//	// 05 責任者氏名２
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(4), pstrCsvDataAry->GetAt(5));
	//	m_pUcData->m_HdName		= strTemp;

	//	// 06 代表者との関係
	//	m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(6);

	//	// 07 事業等の内容１
	//	// 08 事業等の内容２
	//	// 09 事業等の内容３
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(7), pstrCsvDataAry->GetAt(8));
	//	strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(9));
	//	m_pUcData->m_Naiyou		= strTemp;

	//	// 10 売上高
	//	m_pUcData->m_Val		= pstrCsvDataAry->GetAt(10);
	//	RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

	//	// 11 期末棚卸高
	//	m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(11);
	//	RemoveCommaImportNum( m_pUcData->m_Val2 );				// カンマを除く　2011.07.15 h.ukawa

	//	// 12 期末従業員数
	//	m_pUcData->m_Worker		=pstrCsvDataAry->GetAt(12);
	//	RemoveCommaImportNum( m_pUcData->m_Worker );			// カンマを除く　2011.07.15 h.ukawa

	//	// 13 使用建物の延面積
	//	nTemp = 0;
	//	strTemp = _T("");
	//	strTemp = CalcStrMul(pstrCsvDataAry->GetAt(13), 100, 0, &nTemp);	// ×100
	//	m_pUcData->m_Area		= strTemp;

	//	// 14 源泉所得税納付署１
	//	// 15 源泉所得税納付署２
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(14), pstrCsvDataAry->GetAt(15));
	//	m_pUcData->m_HdName2	= strTemp;

	//	// 16 摘要１
	//	// 17 摘要２
	//	// 18 摘要３
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(16), pstrCsvDataAry->GetAt(17));
	//	strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(18));
	//	m_pUcData->m_Teki		= strTemp;
	//	break;
	//case	ID_FGFUNC_SYOKEI:		// 小計行
	//	m_pUcData->m_KeiStr		= m_strSyokeiName;
	//	break;
	//case	ID_FGFUNC_CHUKEI:		// 中計行
	//	m_pUcData->m_KeiStr		= m_strChukeiName;
	//	break;
	//}

	//// レコード登録
	//m_pUcData->Update();
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
int CimpUc131Uriagedaka::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// No.200903 add -->
	int				len=0;
// No.200903 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori 190301 0415 add -->
	CString			cs=_T("");
	CString			cs1=_T("");
	CString			cs2=_T("");
	CString			cs3=_T("");
// midori 190301 0415 add <--
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

	// 文字数をオーバーしていればカットする
	// 01 事業所の名称
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_131_ADNAME1*2);
	pstrCsvDataAry->SetAt(1,strTemp);
	// 02 所在地１
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_131_ADADD1*2);
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 所在地２
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_131_ADADD1*2);
	pstrCsvDataAry->SetAt(3,strTemp);
	// 04 責任者氏名１
	GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_131_HDNAME);
	pstrCsvDataAry->SetAt(4,strTemp);
	// 05 責任者氏名２
	GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_131_HDNAME);
	pstrCsvDataAry->SetAt(5,strTemp);
	// 06 代表者との関係
	GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_131_KANKEI*2);
	pstrCsvDataAry->SetAt(6,strTemp);
	// 07 事業等の内容１
	GetSpritString(pstrCsvDataAry->GetAt(7),&strTemp,MAX_KETA_131_NAIYOU*2/3);
	pstrCsvDataAry->SetAt(7,strTemp);
	// 08 事業等の内容２
	GetSpritString(pstrCsvDataAry->GetAt(8),&strTemp,MAX_KETA_131_NAIYOU*2/3);
	pstrCsvDataAry->SetAt(8,strTemp);
	// 09 事業等の内容３
	GetSpritString(pstrCsvDataAry->GetAt(9),&strTemp,MAX_KETA_131_NAIYOU*2/3);
	pstrCsvDataAry->SetAt(9,strTemp);
// No.200903 del -->
	//// 10 売上高
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(10), ID_VAL_131_DATA, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(10,strTemp);
	//}
	//// 11 期末棚卸高
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(11), ID_VAL2_131_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(11,strTemp);
	//}
// No.200903 del <--
// No.200903 add -->
	// 10 売上高
	if(bG_Kanso == TRUE)	len = ID_VAL_131_K_DATA;
	else					len = ID_VAL_131_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(10), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(10,strTemp);
	}
	// 11 期末棚卸高
	if(bG_Kanso == TRUE)	len = ID_VAL2_131_K_DATA;
	else					len = ID_VAL2_131_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(11,strTemp);
	}
// No.200903 add <--
	// 12 期末従業員数
	if (CheckImportNum2(pstrCsvDataAry->GetAt(12), ID_WORKER_131_DATA, FALSE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(12,strTemp);
	}

// 210113_5301_1 del -->
//	// 13 使用建物の延面積
//// midori 190301 0415 del -->
//	//if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_AREA_131_DATA, 2, FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	//	strTemp.Empty();
//	//	pstrCsvDataAry->SetAt(13,strTemp);
//	//}
//// midori 190301 0415 del <--
//// midori 190301 0415 del -->
//	if(bG_Kanso == TRUE) {
//		strTemp.Empty();
//		pstrCsvDataAry->SetAt(13,strTemp);
//	}
//	else	{
//		if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_AREA_131_DATA, 2, FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
//			strTemp.Empty();
//			pstrCsvDataAry->SetAt(13,strTemp);
//		}
//	}
//// midori 190301 0415 del <--
//	// 14 源泉所得税納付署１
//	GetSpritString(pstrCsvDataAry->GetAt(14),&strTemp,MAX_KETA_131_HDNAME2);
//	pstrCsvDataAry->SetAt(14,strTemp);
//	// 15 源泉所得税納付署２
//	GetSpritString(pstrCsvDataAry->GetAt(15),&strTemp,MAX_KETA_131_HDNAME2);
//	pstrCsvDataAry->SetAt(15,strTemp);
//	// 16 摘要１
//	GetSpritString(pstrCsvDataAry->GetAt(16),&strTemp,MAX_KETA_131_TEKI*2/3);
//	pstrCsvDataAry->SetAt(16,strTemp);
//	// 17 摘要２
//	GetSpritString(pstrCsvDataAry->GetAt(17),&strTemp,MAX_KETA_131_TEKI*2/3);
//	pstrCsvDataAry->SetAt(17,strTemp);
//	// 18 摘要３
//	GetSpritString(pstrCsvDataAry->GetAt(18),&strTemp,MAX_KETA_131_TEKI*2/3);
//	pstrCsvDataAry->SetAt(18,strTemp);
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
//		for (ii = 0; ii <(int)pstrCsvDataAry->GetSize(); ii++) {
//			// 先頭は項目名称が格納されているはずなので無視
//			if (ii == 0) {
//				continue;
//			}
//			// 1項目でもデータがあればデータ行とみなす
//			if (pstrCsvDataAry->GetAt(ii).GetLength() > 0){
//				sw = 1;
//				break;
//			}
//		}
//		if(sw == 0)	{
//			return IMP_REC_OK;
//		}
//// midori UC_0054 add -->
//	}
//// midori UC_0054 add <--
// 210113_5301_1 del <--
// 210113_5301_1 add -->
	int	ofset=13;
	// 13 使用建物の延面積
	if(m_NewVer != TRUE)	{
		if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), ID_AREA_131_DATA, 2, FALSE) != TRUE) {		// [2009/07/09 Minus Value Support]
			strTemp.Empty();
			pstrCsvDataAry->SetAt(13,strTemp);
		}
		ofset++;
	}
	// 14 源泉所得税納付署１
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_131_HDNAME2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
	// 15 源泉所得税納付署２
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_131_HDNAME2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
	// 16 摘要１
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_131_TEKI*2/3);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
	// 17 摘要２
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_131_TEKI*2/3);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
	// 18 摘要３
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_131_TEKI*2/3);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
	//
	strCsvId = pstrCsvDataAry->GetAt(0);
	if (((strCsvId.Compare(CSV_ID_DATA) == 0) ||
		(strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) ||
		(strCsvId.Compare(CSV_ID_HAITOKIN) == 0) ||
		(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) && m_ErrMap.GetCount() > 0)	{
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
	}
// 210113_5301_1 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc131Uriagedaka*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 
	// なし
	// 


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データ加工
	// テーブルへデータ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	double			nTemp			= 0;			// 数値テンポラリ１

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
		// 01 事業所の名称
		// 02 所在地１
		// 03 所在地２
		//m_pUcData->m_AdSeq	= nAdSeq;		// 未使用
		//m_pUcData->m_AdOrder	= AdOrder;		// 未使用
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
		//m_pUcData->m_AdName2	= _T("");		// 未使用
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(2);
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(3);

		// 04 責任者氏名１
		// 05 責任者氏名２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(4), pstrCsvDataAry->GetAt(5));
		m_pUcData->m_HdName		= strTemp;

		// 06 代表者との関係
		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(6);

		// 07 事業等の内容１
		// 08 事業等の内容２
		// 09 事業等の内容３
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(7), pstrCsvDataAry->GetAt(8));
		strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(9));
		m_pUcData->m_Naiyou		= strTemp;

		// 10 売上高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(10);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		// 11 期末棚卸高
		m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(11);
		RemoveCommaImportNum( m_pUcData->m_Val2 );				// カンマを除く　2011.07.15 h.ukawa

		// 12 期末従業員数
		m_pUcData->m_Worker		=pstrCsvDataAry->GetAt(12);
		RemoveCommaImportNum( m_pUcData->m_Worker );			// カンマを除く　2011.07.15 h.ukawa

// 210113_5301_1 del -->
		//// 13 使用建物の延面積
		//nTemp = 0;
		//strTemp = _T("");
		//strTemp = CalcStrMul(pstrCsvDataAry->GetAt(13), 100, 0, &nTemp);	// ×100
		//m_pUcData->m_Area		= strTemp;

		//// 14 源泉所得税納付署１
		//// 15 源泉所得税納付署２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(14), pstrCsvDataAry->GetAt(15));
		//m_pUcData->m_HdName2	= strTemp;

		//// 16 摘要１
		//// 17 摘要２
		//// 18 摘要３
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(16), pstrCsvDataAry->GetAt(17));
		//strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(18));
		//m_pUcData->m_Teki		= strTemp;
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		ofset=13;
		if(m_NewVer != TRUE)	{
			// 13 使用建物の延面積
			nTemp = 0;
			strTemp = _T("");
			strTemp = CalcStrMul(pstrCsvDataAry->GetAt(ofset), 100, 0, &nTemp);	// ×100
			m_pUcData->m_Area		= strTemp;
			ofset++;
		}
		// 14 源泉所得税納付署１
		// 15 源泉所得税納付署２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		m_pUcData->m_HdName2	= strTemp;
		ofset = ofset + 2;

		// 16 摘要１
		// 17 摘要２
		// 18 摘要３
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(ofset+2));
		m_pUcData->m_Teki		= strTemp;
// 210113_5301_1 add <--
		break;
	case	ID_FGFUNC_SYOKEI:		// 小計行
		m_pUcData->m_KeiStr		= m_strSyokeiName;
		break;
	case	ID_FGFUNC_CHUKEI:		// 中計行
		m_pUcData->m_KeiStr		= m_strChukeiName;
		break;
	}

	// レコード登録
	m_pUcData->Update();

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
int CimpUc131Uriagedaka::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	CString			strCsvId		= _T("");		// CSVの識別子

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
	m_pUcData = (CdbUc131Uriagedaka*)m_pUcCmn;


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CStringArray	strTempAry;						// 文字列配列テンポラリ
	CString			strTemp			= _T("");		// 文字列テンポラリ

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// [明細]の場合のみ各項目のデータを取得
	// [小計]、[中計]は識別子のみ
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
		// 01 事業所の名称
		pstrCsvDataAry->SetAt(1, m_pUcData->m_AdName1);

		// 02 所在地１
		pstrCsvDataAry->SetAt(2, m_pUcData->m_AdAdd1);

		// 03 所在地２
		pstrCsvDataAry->SetAt(3, m_pUcData->m_AdAdd2);

		// 04 責任者氏名１
		// 05 責任者氏名２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_HdName, &strTempAry, 2);
		pstrCsvDataAry->SetAt(4, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(5, strTempAry.GetAt(1));

		// 06 代表者との関係
		pstrCsvDataAry->SetAt(6, m_pUcData->m_Kankei);

		// 07 事業等の内容１
		// 08 事業等の内容２
		// 09 事業等の内容３
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 3);
		pstrCsvDataAry->SetAt(7, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(8, strTempAry.GetAt(1));
		pstrCsvDataAry->SetAt(9, strTempAry.GetAt(2));

		// 10 売上高
		pstrCsvDataAry->SetAt(10, m_pUcData->m_Val);

		// 11 期末棚卸高
		pstrCsvDataAry->SetAt(11, m_pUcData->m_Val2);

		// 12 期末従業員数
		pstrCsvDataAry->SetAt(12, m_pUcData->m_Worker);

// 210113_5301_1 del -->
		//// 13 使用建物の延面積
		//strTemp = _T("");
		//if(bG_Kanso != TRUE) {
		//	strTemp = CalcStrMul(m_pUcData->m_Area,0.01,2);	// ÷100
		//}
		//pstrCsvDataAry->SetAt(13, strTemp);

		//// 14 源泉所得税納付署１
		//// 15 源泉所得税納付署２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_HdName2, &strTempAry, 2);
		//pstrCsvDataAry->SetAt(14, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(15, strTempAry.GetAt(1));

		//// 16 摘要１
		//// 17 摘要２
		//// 18 摘要３
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 3);
		//pstrCsvDataAry->SetAt(16, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(17, strTempAry.GetAt(1));
		//pstrCsvDataAry->SetAt(18, strTempAry.GetAt(2));
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		int	ofset=13;
// 24/02/20_ScoreLink cor -->
		//if(bG_Kanso != TRUE)	{
		//	// 13 使用建物の延面積
		//	strTemp = _T("");
		//	if(bG_Kanso != TRUE) {
		//		strTemp = CalcStrMul(m_pUcData->m_Area,0.01,2);	// ÷100
		//	}
// --------------------------
		if(bG_Kanso != TRUE || bG_Sl == TRUE)	{
			// 13 使用建物の延面積
			strTemp = _T("");
			strTemp = CalcStrMul(m_pUcData->m_Area,0.01,2);	// ÷100
// 24/02/20_ScoreLink cor <--
			pstrCsvDataAry->SetAt(ofset, strTemp);
			ofset++;
		}
		// 14 源泉所得税納付署１
		// 15 源泉所得税納付署２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_HdName2, &strTempAry, 2);
		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(0));
		ofset++;
		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(1));
		ofset++;
		// 16 摘要１
		// 17 摘要２
		// 18 摘要３
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 3);
		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(0));
		ofset++;
		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(1));
		ofset++;
		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(2));
// 210113_5301_1 add <--
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}

// 157766 del -->
//// 210113_5301_1 add -->
////********************************************************************************
////	新様式のCSVデータかチェックする
////		IN		pstrCsvDataAry			CSVの1行分のデータ
////
////		RET		TRUE  : 新様式
////				FALSE : 上記以外
////********************************************************************************
//BOOL CimpUc131Uriagedaka::virFormatedIsNewVer(CStringArray* pstrCsvDataAry)
//{
//	BOOL	nRet=TRUE;
//	CString strItem=_T("");
//	int		ii=0;
//
//	for(ii=1/*[項目]読み飛ばし*/; ii<pstrCsvDataAry->GetCount(); ii++)	{
//		strItem.Empty();
//		switch(ii)	{
//			case	1:	strItem = CSV_131_ITEM_01;	break;
//			case	2:	strItem = CSV_131_ITEM_02;	break;
//			case	3:	strItem = CSV_131_ITEM_03;	break;
//			case	4:	strItem = CSV_131_ITEM_04;	break;
//			case	5:	strItem = CSV_131_ITEM_05;	break;
//			case	6:	strItem = CSV_131_ITEM_06;	break;
//			case	7:	strItem = CSV_131_ITEM_07;	break;
//			case	8:	strItem = CSV_131_ITEM_08;	break;
//			case	9:	strItem = CSV_131_ITEM_09;	break;
//			case	10:	strItem = CSV_131_ITEM_10;	break;
//			case	11:	strItem = CSV_131_ITEM_11;	break;
//			case	12:	strItem = CSV_131_ITEM_12;	break;
//			case	13:	strItem = CSV_131_ITEM_14;	break;
//			case	14:	strItem = CSV_131_ITEM_15;	break;
//			case	15:	strItem = CSV_131_ITEM_16;	break;
//			case	16:	strItem = CSV_131_ITEM_17;	break;
//			case	17:	strItem = CSV_131_ITEM_18;	break;
//			default:								break;
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
