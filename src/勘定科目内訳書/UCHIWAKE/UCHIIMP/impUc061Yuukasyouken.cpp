#include "StdAfx.h"
#include "impUc061Yuukasyouken.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

// コンストラクタ
CimpUc061Yuukasyouken::CimpUc061Yuukasyouken(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc061Yuukasyouken::~CimpUc061Yuukasyouken(void)
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
int CimpUc061Yuukasyouken::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_061;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_061_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc061Yuukasyouken(pDB));

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
	m_RowMax		= ID_ROWKOBETU_061;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_061 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_061;


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
int CimpUc061Yuukasyouken::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc061Yuukasyouken*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 区分
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_061_KUBUN*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_01);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_KUBUN;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 02 種類・銘柄１
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_061_SYURUI) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_02);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_02_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_02);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_SYURUI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 03 種類・銘柄２
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_061_SYURUI) != TRUE) {
// 210113_5301_1 del -->
		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_03);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_03_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_03);
// 210113_5301_1 add <--
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_SYURUI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 04 期末数量
//	if (CheckImportNum(pstrCsvDataAry->GetAt(4), 8, 3) != TRUE) {
//	if (CheckImportNum(pstrCsvDataAry->GetAt(4), 8, 3, FALSE) != TRUE) {				// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(4), 8, 3, FALSE) != TRUE) {				// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(4), 9, 3, FALSE) != TRUE) {				// [2012/07/20 桁数8→9]
	if (CheckImportNum2(pstrCsvDataAry->GetAt(4), 9, 3, TRUE) != TRUE) {				// [2012/08/01 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_04);
// midori 150895 cor -->
//		return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength	= 9;	// 整数最大桁数
		if(pstrCsvDataAry->GetAt(4).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_uImpCsvInfo.nLength2	= 3;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 05 （単位）
	if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_061_TANNI*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_05);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_TANNI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

// No.200903 del -->
//	// 06 期末現在高（時価評価前）
////	if (CheckImportNum(pstrCsvDataAry->GetAt(6), ID_VAL_061_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(6), ID_VAL_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(6), ID_VAL_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_06);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL_061_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(6).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
//
//	// 07 期末現在高（時価評価後）
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL2_061_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL2_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), ID_VAL2_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_07);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL2_061_DATA;
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
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_061_K_DATA;
	else					len = ID_VAL_061_DATA;
	// 06 期末現在高（時価評価前）
	if (CheckImportNum2(pstrCsvDataAry->GetAt(6), len, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_06);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(6).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 07 期末現在高（時価評価後）
	if(bG_Kanso == TRUE)	len = ID_VAL2_061_K_DATA;
	else					len = ID_VAL2_061_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_07);
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
// No.200903 add <--

	// 08 異動年月日
	long nSpDate = 0;
// midori 150902 cor -->
	//if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(8), &nSpDate) != TRUE && nSpDate != 0) {
	//	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_08);
	//	return IMP_REC_NG;
	//}
// ---------------------
	if (pstrCsvDataAry->GetAt(8) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(8), &nSpDate) != TRUE) {
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_08);
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}
	}
// midori 150902 cor <--

	// 09 異動事由１
	if (CheckImportStr(pstrCsvDataAry->GetAt(9), MAX_KETA_061_RIYUU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_09);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_RIYUU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 10 異動事由２
	if (CheckImportStr(pstrCsvDataAry->GetAt(10), MAX_KETA_061_RIYUU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_10);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_RIYUU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 11 異動数量
//	if (CheckImportNum(pstrCsvDataAry->GetAt(11), 8, 3) != TRUE) {
//	if (CheckImportNum(pstrCsvDataAry->GetAt(11), 8, 3, TRUE) != TRUE) {					// [2009/07/09 Minus Value Support]
//	if (CheckImportNum(pstrCsvDataAry->GetAt(11), 8, 3, FALSE) != TRUE) {					// [2009/09/01 Fix]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), 8, 3, FALSE) != TRUE) {					// [2009/09/01 Fix]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), 9, 3, FALSE) != TRUE) {					// [2012/07/20 桁数8→9]
	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), 9, 3, TRUE) != TRUE) {					// [2013/08/01 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_11);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength	= 9;	// 整数最大桁数
		if(pstrCsvDataAry->GetAt(11).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_uImpCsvInfo.nLength2	= 3;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 12 （単位）
	if (CheckImportStr(pstrCsvDataAry->GetAt(12), MAX_KETA_061_TANNI2*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_12);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_TANNI2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
}

	// 13 期末増（減）額
// No.200903 del -->
////	if (CheckImportNum(pstrCsvDataAry->GetAt(13), ID_VAL3_061_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(13), ID_VAL3_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_VAL3_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_13);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL3_061_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(13).Left(1) == '-')	{
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
	if(bG_Kanso == TRUE)	len = ID_VAL3_061_K_DATA;
	else					len = ID_VAL3_061_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(13), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_13);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(13).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

	// 14 売却（買入）先の名称（氏名）１
	if (CheckImportStr(pstrCsvDataAry->GetAt(14), MAX_KETA_061_ADNAME1) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_14);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_ADNAME1 / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 15 売却（買入）先の名称（氏名）２
	if (CheckImportStr(pstrCsvDataAry->GetAt(15), MAX_KETA_061_ADNAME1) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_15);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_ADNAME1 / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 16 売却（買入）先の所在地（住所）１
	if (CheckImportStr(pstrCsvDataAry->GetAt(16), MAX_KETA_061_ADADD1*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_16);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 17 売却（買入）先の所在地（住所）２
	if (CheckImportStr(pstrCsvDataAry->GetAt(17), MAX_KETA_061_ADADD2*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_17);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_061_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 18 摘要１
// midori 150895 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(18), MAX_KETA_061_TEKI) != TRUE) {
	//	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_18);
	//	return IMP_REC_NG;
	//}
// ---------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(18), MAX_KETA_061_TEKI/3*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_18);
		m_uImpCsvInfo.nLength = MAX_KETA_061_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// midori 150895 cor <--

	// 19 摘要２
// midori 150895 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(19), MAX_KETA_061_TEKI) != TRUE) {
	//	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_19);
	//	return IMP_REC_NG;
	//}
// ---------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(19), MAX_KETA_061_TEKI/3*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_19);
		m_uImpCsvInfo.nLength = MAX_KETA_061_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// midori 150895 cor <--

	// 20 摘要３
// midori 150895 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(20), MAX_KETA_061_TEKI) != TRUE) {
	//	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_20);
	//	return IMP_REC_NG;
	//}
// ---------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(20), MAX_KETA_061_TEKI/3*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_061_ITEM_20);
		m_uImpCsvInfo.nLength = MAX_KETA_061_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// midori 150895 cor <--

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
//
//	// 01 区分
//	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
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
//		// 01 区分
//		m_pUcData->m_KnSeq		= nKnSeq;
//		m_pUcData->m_KnOrder	= nKnOrder;
//		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
//// midori 152137 add -->
//		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//// midori 152137 add <--
//
//		// 02 種類・銘柄１
//		// 03 種類・銘柄２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
//		m_pUcData->m_Syurui		= strTemp;
//
//		// 04 期末数量
//		strTemp = _T("");
//		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(4), 1000, 0);	// ×1000
//		m_pUcData->m_Suuryou	= strTemp;
//
//		// 05 （単位）
//		m_pUcData->m_Tanni		= pstrCsvDataAry->GetAt(5);
//
//		// 06 期末現在高（時価評価前）
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(6);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 07 期末現在高（時価評価後）
//		m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(7);
//		RemoveCommaImportNum( m_pUcData->m_Val2 );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 08 異動年月日
//		m_pUcData->m_SpDate		= nSpDate;
//
//		// 09 異動事由１
//		// 10 異動事由２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(9), pstrCsvDataAry->GetAt(10));
//		m_pUcData->m_Riyuu		= strTemp;
//
//		// 11 異動数量
//		strTemp = _T("");
//		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(11), 1000, 0);	// ×1000
//		m_pUcData->m_Suuryou2	= strTemp;
//
//		// 12 （単位）
//		m_pUcData->m_Tanni2		= pstrCsvDataAry->GetAt(12);
//
//		// 13 期末増（減）額
//		m_pUcData->m_Val3		= pstrCsvDataAry->GetAt(13);
//		RemoveCommaImportNum( m_pUcData->m_Val3 );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 14 売却（買入）先の名称（氏名）１
//		m_pUcData->m_AdSeq		= 0;
//		m_pUcData->m_AdOrder	= 0;
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(14);
//		// 15 売却（買入）先の名称（氏名）２
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(15);
//
//		// 16 売却（買入）先の所在地（住所）１
//		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(16);
//
//		// 17 売却（買入）先の所在地（住所）２
//		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(17);
//
//		// 18 摘要１
//		// 19 摘要２
//		// 20 摘要３
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(18), pstrCsvDataAry->GetAt(19));
//		strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(20));
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
int CimpUc061Yuukasyouken::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
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
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

	// 文字数をオーバーしていればカットする

	// 01 区分
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_061_KUBUN*2);
	pstrCsvDataAry->SetAt(1,strTemp);
	// 02 種類・銘柄１
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_061_SYURUI);
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 種類・銘柄２
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_061_SYURUI);
	pstrCsvDataAry->SetAt(3,strTemp);
	// 04 期末数量
	if (CheckImportNum2(pstrCsvDataAry->GetAt(4), 9, 3, TRUE) != TRUE) {				// [2012/08/01 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(4,strTemp);
	}
	// 05 （単位）
	GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_061_TANNI*2);
	pstrCsvDataAry->SetAt(5,strTemp);
// No.200903 del -->
	//// 06 期末現在高（時価評価前）
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(6), ID_VAL_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(6,strTemp);
	//}
	//// 07 期末現在高（時価評価後）
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(7), ID_VAL2_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(7,strTemp);
	//}
// No.200903 del <--
// No.200903 add -->
	// 06 期末現在高（時価評価前）
	if(bG_Kanso == TRUE)	len = ID_VAL_061_K_DATA;
	else					len = ID_VAL_061_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(6), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(6,strTemp);
	}
	// 07 期末現在高（時価評価後）
	if(bG_Kanso == TRUE)	len = ID_VAL2_061_K_DATA;
	else					len = ID_VAL2_061_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(7,strTemp);
	}
// No.200903 add <--
	// 08 異動年月日
	long nSpDate = 0;
	if (pstrCsvDataAry->GetAt(8) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(8), &nSpDate) != TRUE) {
			strTemp.Empty();
			pstrCsvDataAry->SetAt(8,strTemp);
		}
	}
	// 09 異動事由１
	GetSpritString(pstrCsvDataAry->GetAt(9),&strTemp,MAX_KETA_061_RIYUU);
	pstrCsvDataAry->SetAt(9,strTemp);
	// 10 異動事由２
	GetSpritString(pstrCsvDataAry->GetAt(10),&strTemp,MAX_KETA_061_RIYUU);
	pstrCsvDataAry->SetAt(10,strTemp);
	// 11 異動数量
	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), 9, 3, TRUE) != TRUE) {					// [2013/08/01 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(11,strTemp);
	}
	// 12 （単位）
	GetSpritString(pstrCsvDataAry->GetAt(12),&strTemp,MAX_KETA_061_TANNI2*2);
	pstrCsvDataAry->SetAt(12,strTemp);
	// 13 期末増（減）額
// No.200903 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_VAL3_061_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL3_061_K_DATA;
	else					len = ID_VAL3_061_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(13), len, TRUE) != TRUE)	{
// No.200903 add <--
		strTemp.Empty();
		pstrCsvDataAry->SetAt(13,strTemp);
	}
	// 14 売却（買入）先の名称（氏名）１
	GetSpritString(pstrCsvDataAry->GetAt(14),&strTemp,MAX_KETA_061_ADNAME1);
	pstrCsvDataAry->SetAt(14,strTemp);
	// 15 売却（買入）先の名称（氏名）２
	GetSpritString(pstrCsvDataAry->GetAt(15),&strTemp,MAX_KETA_061_ADNAME1);
	pstrCsvDataAry->SetAt(15,strTemp);
	// 16 売却（買入）先の所在地（住所）１
	GetSpritString(pstrCsvDataAry->GetAt(16),&strTemp,MAX_KETA_061_ADADD1*2);
	pstrCsvDataAry->SetAt(16,strTemp);
	// 17 売却（買入）先の所在地（住所）２
	GetSpritString(pstrCsvDataAry->GetAt(17),&strTemp,MAX_KETA_061_ADADD2*2);
	pstrCsvDataAry->SetAt(17,strTemp);
	// 18 摘要１
	GetSpritString(pstrCsvDataAry->GetAt(18),&strTemp,MAX_KETA_061_TEKI/3*2);
	pstrCsvDataAry->SetAt(18,strTemp);
	// 19 摘要２
	GetSpritString(pstrCsvDataAry->GetAt(19),&strTemp,MAX_KETA_061_TEKI/3*2);
	pstrCsvDataAry->SetAt(19,strTemp);
	// 20 摘要３
	GetSpritString(pstrCsvDataAry->GetAt(20),&strTemp,MAX_KETA_061_TEKI/3*2);
	pstrCsvDataAry->SetAt(20,strTemp);

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
	m_pUcData = (CdbUc061Yuukasyouken*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順

	// 01 区分
	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
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
		// 01 区分
		m_pUcData->m_KnSeq		= nKnSeq;
		m_pUcData->m_KnOrder	= nKnOrder;
		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));

		// 02 種類・銘柄１
		// 03 種類・銘柄２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
		m_pUcData->m_Syurui		= strTemp;

		// 04 期末数量
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(4), 1000, 0);	// ×1000
		m_pUcData->m_Suuryou	= strTemp;

		// 05 （単位）
		m_pUcData->m_Tanni		= pstrCsvDataAry->GetAt(5);

		// 06 期末現在高（時価評価前）
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(6);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		// 07 期末現在高（時価評価後）
		m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(7);
		RemoveCommaImportNum( m_pUcData->m_Val2 );				// カンマを除く　2011.07.15 h.ukawa

		// 08 異動年月日
		m_pUcData->m_SpDate		= nSpDate;

		// 09 異動事由１
		// 10 異動事由２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(9), pstrCsvDataAry->GetAt(10));
		m_pUcData->m_Riyuu		= strTemp;

		// 11 異動数量
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(11), 1000, 0);	// ×1000
		m_pUcData->m_Suuryou2	= strTemp;

		// 12 （単位）
		m_pUcData->m_Tanni2		= pstrCsvDataAry->GetAt(12);

		// 13 期末増（減）額
		m_pUcData->m_Val3		= pstrCsvDataAry->GetAt(13);
		RemoveCommaImportNum( m_pUcData->m_Val3 );				// カンマを除く　2011.07.15 h.ukawa

		// 14 売却（買入）先の名称（氏名）１
		m_pUcData->m_AdSeq		= 0;
		m_pUcData->m_AdOrder	= 0;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(14);
		// 15 売却（買入）先の名称（氏名）２
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(15);

		// 16 売却（買入）先の所在地（住所）１
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(16);

		// 17 売却（買入）先の所在地（住所）２
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(17);

		// 18 摘要１
		// 19 摘要２
		// 20 摘要３
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(18), pstrCsvDataAry->GetAt(19));
		strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(20));
		m_pUcData->m_Teki		= strTemp;
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
int CimpUc061Yuukasyouken::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc061Yuukasyouken*)m_pUcCmn;


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
		// 01 区分
		pstrCsvDataAry->SetAt( 1, m_pUcData->m_KnName);

		// 02 種類・銘柄１
		// 03 種類・銘柄２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Syurui, &strTempAry, 2);
		pstrCsvDataAry->SetAt( 2, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt( 3, strTempAry.GetAt(1));

		// 04 期末数量
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Suuryou, 0.001, 3);		// ÷1000
		pstrCsvDataAry->SetAt( 4, strTemp);

		// 05 （単位）
		pstrCsvDataAry->SetAt( 5, m_pUcData->m_Tanni);

		// 06 期末現在高（時価評価前）
		pstrCsvDataAry->SetAt( 6, m_pUcData->m_Val);

		// 07 期末現在高（時価評価後）
		pstrCsvDataAry->SetAt( 7, m_pUcData->m_Val2);

		// 08 異動年月日
		strTemp = _T("");											// [2009/09/11 Fix]
		if (m_pUcData->m_SpDate > 0) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strTemp);
		}
		pstrCsvDataAry->SetAt( 8, strTemp);

		// 09 異動事由１
		// 10 異動事由２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Riyuu, &strTempAry, 2);
		pstrCsvDataAry->SetAt( 9, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(10, strTempAry.GetAt(1));

		// 11 異動数量
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Suuryou2, 0.001, 3);		// ÷1000
		pstrCsvDataAry->SetAt(11, strTemp);

		// 12 （単位）
		pstrCsvDataAry->SetAt(12, m_pUcData->m_Tanni2);

		// 13 期末増（減）額
		pstrCsvDataAry->SetAt(13, m_pUcData->m_Val3);

		// 14 売却（買入）先の名称（氏名）１
		pstrCsvDataAry->SetAt(14, m_pUcData->m_AdName1);

		// 15 売却（買入）先の名称（氏名）２
		pstrCsvDataAry->SetAt(15, m_pUcData->m_AdName2);

		// 16 売却（買入）先の所在地（住所）１
		pstrCsvDataAry->SetAt(16, m_pUcData->m_AdAdd1);

		// 17 売却（買入）先の所在地（住所）２
		pstrCsvDataAry->SetAt(17, m_pUcData->m_AdAdd2);

		// 18 摘要１
		// 19 摘要２
		// 20 摘要３
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 3);
		pstrCsvDataAry->SetAt(18, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(19, strTempAry.GetAt(1));
		pstrCsvDataAry->SetAt(20, strTempAry.GetAt(2));
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}