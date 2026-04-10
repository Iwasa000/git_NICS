#include "StdAfx.h"
#include "impUc141Yakuin.h"

// midori 154710 add -->
extern	BOOL	bG_Kanso;
// midori 154710 add <--

// コンストラクタ
CimpUc141Yakuin::CimpUc141Yakuin(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc141Yakuin::~CimpUc141Yakuin(void)
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
int CimpUc141Yakuin::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_141;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_141_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc141Yakuin(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );

	// inf_form_typeテーブルの初期化
	m_pUcInfFormType->Init( ID_FORMNO_14 );


	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_141;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_141 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_141;

	// 出力設定が通常の場合
	if( m_pUcInfFormType->m_FormType == 0 ){
		m_RowMax		= ID_ROWNORMAL_141;
		m_RowMaxData	= ID_ROWNORMAL_141 - 1;
	}


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
int CimpUc141Yakuin::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// midori 154710 add -->
	int		keta=0;
// midori 154710 add <--
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
// midori 180404 del -->
//	BYTE			cFgFunc			= 0;			// 特殊行フラグ
// midori 180404 del -->

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
	m_pUcData = (CdbUc141Yakuin*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 役職名
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_141_NAMEYAKU*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_01);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_NAMEYAKU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 02 担当職務
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_141_NAMETANT*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_02);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_NAMETANT;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 03 氏名１
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_141_ADNAME1) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_03);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 04 氏名２
	if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_141_ADNAME1) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_04);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 05 住所１
	if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_141_ADADD1*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_05);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 06 住所２
	if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_141_ADADD2*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_06);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 07 代表者との関係
	if (CheckImportStr(pstrCsvDataAry->GetAt(7), MAX_KETA_141_KANKEI*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_07);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_141_KANKEI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 08 常勤・非常勤の別
	BYTE bTemp;
	if (ConvFpTimeStrToDbVal(pstrCsvDataAry->GetAt(8), &bTemp) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_08);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		// 「区分が正しくありません。」のエラーしかないので、長さと種類をセットする必要なし
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

// 改良No.23-0961 del -->
//	// 09 使用人職務分
////	if (CheckImportNum(pstrCsvDataAry->GetAt(9), ID_EMVAL_141_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(9), ID_EMVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(9), ID_EMVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_09);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_EMVAL_141_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(9).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
//
//	// 10 定期同額給与
////	if (CheckImportNum(pstrCsvDataAry->GetAt(10), ID_PRVAL_141_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(10), ID_PRVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(10), ID_PRVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_10);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_PRVAL_141_DATA;
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
//	// 11 事前確定届出給与
////	if (CheckImportNum(pstrCsvDataAry->GetAt(11), ID_BFVAL_141_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(11), ID_BFVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(11), ID_BFVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_11);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_BFVAL_141_DATA;
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
//
//	// 12 利益連動給与
////	if (CheckImportNum(pstrCsvDataAry->GetAt(12), ID_PFVAL_141_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(12), ID_PFVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(12), ID_PFVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//// 210113_5301_1 del -->
//		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_12);
//// 210113_5301_1 del <--
//// 210113_5301_1 add -->
//		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_12_NEW);
//		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_12);
//// 210113_5301_1 add <--
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_PFVAL_141_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(12).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
//
//	// 13 その他
////	if (CheckImportNum(pstrCsvDataAry->GetAt(13), ID_ETVAL_141_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(13), ID_ETVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_ETVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_13);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_ETVAL_141_DATA;
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
//
//	// 14 退職給与
////	if (CheckImportNum(pstrCsvDataAry->GetAt(14), ID_RTVAL_141_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(14), ID_RTVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(14), ID_RTVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_14);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_RTVAL_141_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(14).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150895 cor <--
//	}
//
//	// 15 役員給与計
//// midori 154710 del -->
//////	if (CheckImportNum(pstrCsvDataAry->GetAt(15), ID_VAL_141_DATA) != TRUE) {
//////	if (CheckImportNum(pstrCsvDataAry->GetAt(15), ID_VAL_141_DATA, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
////	if (CheckImportNum2(pstrCsvDataAry->GetAt(15), ID_VAL_141_DATA, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
////		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_15);
////// midori 150895 cor -->
////		//return IMP_REC_NG;
////// ---------------------
////		m_uImpCsvInfo.nLength = ID_VAL_141_DATA;
////		m_uImpCsvInfo.nKind = 1;	// 1:金額
////		if(pstrCsvDataAry->GetAt(15).Left(1) == '-')	{
////			m_uImpCsvInfo.nLength--;
////			m_uImpCsvInfo.nType = 1;
////		}
////		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
////		CsvInfoClr(&m_uImpCsvInfo);
////		nRet = IMP_REC_NG;
////// midori 150895 cor <--
////	}
//// midori 154710 del <--
//// midori 154710 add -->
//	if(bG_Kanso == TRUE)	keta = ID_VAL_141_DATA_K;
//	else					keta = ID_VAL_141_DATA;
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(15),keta,TRUE) != TRUE)	{
//		strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_141_ITEM_15);
//		m_uImpCsvInfo.nLength = keta;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(15).Left(1) == '-')	{
//			m_uImpCsvInfo.nLength--;
//			m_uImpCsvInfo.nType = 1;
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//	}
//// midori 154710 add <--
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
	// 09 使用人職務分
	if(bG_Kanso == TRUE)	keta = ID_EMVAL_141_DATA_K;
	else					keta = ID_EMVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(9), keta, TRUE) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_09);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(9).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 10 定期同額給与
	if(bG_Kanso == TRUE)	keta = ID_PRVAL_141_DATA_K;
	else					keta = ID_PRVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(10), keta, TRUE) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_10);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(10).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 11 事前確定届出給与
	if(bG_Kanso == TRUE)	keta = ID_BFVAL_141_DATA_K;
	else					keta = ID_BFVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(11), keta, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_11);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(11).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 12 利益連動給与
	if(bG_Kanso == TRUE)	keta = ID_PFVAL_141_DATA_K;
	else					keta = ID_PFVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(12), keta, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_12_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_12);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(12).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 13 その他
	if(bG_Kanso == TRUE)	keta = ID_ETVAL_141_DATA_K;
	else					keta = ID_ETVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(13), keta, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_13);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(13).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 14 退職給与
	if(bG_Kanso == TRUE)	keta = ID_RTVAL_141_DATA_K;
	else					keta = ID_RTVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(14), keta, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_14);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(14).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 15 役員給与計
	if(bG_Kanso == TRUE)	keta = ID_VAL_141_DATA_K;
	else					keta = ID_VAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(15), keta, TRUE) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_141_ITEM_15);
		m_uImpCsvInfo.nLength = keta;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(15).Left(1) == '-') {
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(), m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// 改良No.23-0961 add <--

// midori 150895 add -->
	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}
// midori 150895 add <--

// midori 180404 del -->
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 名称テーブルへ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 名称テーブル登録無し
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// データ加工
//	// テーブルへデータ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	char			strTemp[15];					// 文字列テンポラリ
//	__int64			calcTemp		= 0;			// 自動計算テンポラリ
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
//		// 01 役職名
//		m_pUcData->m_NameYaku	= pstrCsvDataAry->GetAt(1);
//
//		// 02 担当職務
//		m_pUcData->m_NameTant	= pstrCsvDataAry->GetAt(2);
//
//		// 03 氏名１
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(3);
//
//		// 04 氏名２
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(4);
//
//		// 05 住所１
//		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(5);
//
//		// 06 住所２
//		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(6);
//
//		// 07 代表者との関係
//		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(7);
//
//		// 08 常勤・非常勤の別
//		m_pUcData->m_FpTime		= bTemp;
//
//		// 09 使用人職務分
//		if (pstrCsvDataAry->GetAt(9).IsEmpty() != TRUE) {
//			m_pUcData->m_EmVal = pstrCsvDataAry->GetAt(9);
//			RemoveCommaImportNum( m_pUcData->m_EmVal );			// カンマを除く　2011.07.15 h.ukawa
////			calcTemp += atoi(pstrCsvDataAry->GetAt(9));
//			calcTemp += atoi(m_pUcData->m_EmVal);				//2011.07.15
//		}
//		
//		// 10 定期同額給与
//		if (pstrCsvDataAry->GetAt(10).IsEmpty() != TRUE) {
//			m_pUcData->m_PrVal = pstrCsvDataAry->GetAt(10);
//			RemoveCommaImportNum( m_pUcData->m_PrVal );			// カンマを除く　2011.07.15 h.ukawa
////			calcTemp += atoi(pstrCsvDataAry->GetAt(10));
//			calcTemp += atoi(m_pUcData->m_PrVal);				//2011.07.15
//		}
//
//		// 11 事前確定届出給与
//		if (pstrCsvDataAry->GetAt(11).IsEmpty() != TRUE) {
//			m_pUcData->m_BfVal = pstrCsvDataAry->GetAt(11);
//			RemoveCommaImportNum( m_pUcData->m_BfVal );			// カンマを除く　2011.07.15 h.ukawa
////			calcTemp += atoi(pstrCsvDataAry->GetAt(11));
//			calcTemp += atoi(m_pUcData->m_BfVal);				//2011.07.15
//		}
//
//		// 12 利益連動給与
//		if (pstrCsvDataAry->GetAt(12).IsEmpty() != TRUE) {
//			m_pUcData->m_PfVal = pstrCsvDataAry->GetAt(12);
//			RemoveCommaImportNum( m_pUcData->m_PfVal );			// カンマを除く　2011.07.15 h.ukawa
////			calcTemp += atoi(pstrCsvDataAry->GetAt(12));
//			calcTemp += atoi(m_pUcData->m_PfVal);				//2011.07.15
//		}
//
//		// 13 その他
//		if (pstrCsvDataAry->GetAt(13).IsEmpty() != TRUE) {
//			m_pUcData->m_EtVal = pstrCsvDataAry->GetAt(13);
//			RemoveCommaImportNum( m_pUcData->m_EtVal );			// カンマを除く　2011.07.15 h.ukawa
////			calcTemp += atoi(pstrCsvDataAry->GetAt(13));
//			calcTemp += atoi(m_pUcData->m_EtVal);				//2011.07.15
//		}
//
//		// 14 退職給与
//		m_pUcData->m_RtVal = pstrCsvDataAry->GetAt(14);
//		RemoveCommaImportNum( m_pUcData->m_RtVal );			// カンマを除く　2011.07.15 h.ukawa
//
//		// 15 役員給与計
//		if (pstrCsvDataAry->GetAt(15).IsEmpty() != TRUE) {
//			m_pUcData->m_FgInput = 1;			// 手入力
//			m_pUcData->m_Val = pstrCsvDataAry->GetAt(15);
//			RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa
//		} else {
//			m_pUcData->m_FgInput = 0;			// 自動計算
//			_i64toa_s(calcTemp, strTemp, sizeof(strTemp), 10);
//			m_pUcData->m_Val = strTemp;
//		}
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
int CimpUc141Yakuin::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// midori 154710 add -->
	int				keta=0;
// midori 154710 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

	// 文字数をオーバーしていればカットする
	// 01 役職名
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_141_NAMEYAKU*2);
	pstrCsvDataAry->SetAt(1,strTemp);
	// 02 担当職務
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_141_NAMETANT*2);
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 氏名１
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_141_ADNAME1);
	pstrCsvDataAry->SetAt(3,strTemp);
	// 04 氏名２
	GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_141_ADNAME1);
	pstrCsvDataAry->SetAt(4,strTemp);
	// 05 住所１
	GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_141_ADADD1*2);
	pstrCsvDataAry->SetAt(5,strTemp);
	// 06 住所２
	GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_141_ADADD2*2);
	pstrCsvDataAry->SetAt(6,strTemp);
	// 07 代表者との関係
	GetSpritString(pstrCsvDataAry->GetAt(7),&strTemp,MAX_KETA_141_KANKEI*2);
	pstrCsvDataAry->SetAt(7,strTemp);

	// 08 常勤・非常勤の別
	BYTE bTemp;
	if (ConvFpTimeStrToDbVal(pstrCsvDataAry->GetAt(8), &bTemp) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(8,strTemp);
	}
// 改良No.23-0961 del -->
	//// 09 使用人職務分
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(9), ID_EMVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(9,strTemp);
	//}
	//// 10 定期同額給与
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(10), ID_PRVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(10,strTemp);
	//}
	//// 11 事前確定届出給与
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(11), ID_BFVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(11,strTemp);
	//}
	//// 12 利益連動給与
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(12), ID_PFVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(12,strTemp);
	//}
	//// 13 その他
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(13), ID_ETVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(13,strTemp);
	//}
	//// 14 退職給与
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(14), ID_RTVAL_141_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(14,strTemp);
	//}
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
	// 09 使用人職務分
	if(bG_Kanso == TRUE)	keta = ID_EMVAL_141_DATA_K;
	else					keta = ID_EMVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(9), keta, TRUE) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(9, strTemp);
	}
	// 10 定期同額給与
	if(bG_Kanso == TRUE)	keta = ID_PRVAL_141_DATA_K;
	else					keta = ID_PRVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(10), keta, TRUE) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(10, strTemp);
	}
	// 11 事前確定届出給与
	if(bG_Kanso == TRUE)	keta = ID_BFVAL_141_DATA_K;
	else					keta = ID_BFVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(11), keta, TRUE) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(11, strTemp);
	}
	// 12 利益連動給与
	if(bG_Kanso == TRUE)	keta = ID_PFVAL_141_DATA_K;
	else					keta = ID_PFVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(12), keta, TRUE) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(12, strTemp);
	}
	// 13 その他
	if(bG_Kanso == TRUE)	keta = ID_ETVAL_141_DATA_K;
	else					keta = ID_ETVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(13), keta, TRUE) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(13, strTemp);
	}
	// 14 退職給与
	if(bG_Kanso == TRUE)	keta = ID_RTVAL_141_DATA_K;
	else					keta = ID_RTVAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(14), keta, TRUE) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(14, strTemp);
	}
// 改良No.23-0961 add <--

	// 15 役員給与計
// midori 154710 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(15), ID_VAL_141_DATA, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
// midori 154710 del <--
// midori 154710 add -->
	if(bG_Kanso == TRUE)	keta = ID_VAL_141_DATA_K;
	else					keta = ID_VAL_141_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(15),keta,TRUE) != TRUE)	{
// midori 154710 add <--
		strTemp.Empty();
		pstrCsvDataAry->SetAt(15,strTemp);
	}

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
	m_pUcData = (CdbUc141Yakuin*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブル登録無し

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データ加工
	// テーブルへデータ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	char			chrTemp[15];					// 文字列テンポラリ
	__int64			calcTemp		= 0;			// 自動計算テンポラリ

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
		// 01 役職名
		m_pUcData->m_NameYaku	= pstrCsvDataAry->GetAt(1);

		// 02 担当職務
		m_pUcData->m_NameTant	= pstrCsvDataAry->GetAt(2);

		// 03 氏名１
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(3);

		// 04 氏名２
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(4);

		// 05 住所１
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(5);

		// 06 住所２
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(6);

		// 07 代表者との関係
		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(7);

		// 08 常勤・非常勤の別
		m_pUcData->m_FpTime		= bTemp;

		// 09 使用人職務分
		if (pstrCsvDataAry->GetAt(9).IsEmpty() != TRUE) {
			m_pUcData->m_EmVal = pstrCsvDataAry->GetAt(9);
			RemoveCommaImportNum( m_pUcData->m_EmVal );			// カンマを除く　2011.07.15 h.ukawa
//			calcTemp += atoi(pstrCsvDataAry->GetAt(9));
			calcTemp += atoi(m_pUcData->m_EmVal);				//2011.07.15
		}
		
		// 10 定期同額給与
		if (pstrCsvDataAry->GetAt(10).IsEmpty() != TRUE) {
			m_pUcData->m_PrVal = pstrCsvDataAry->GetAt(10);
			RemoveCommaImportNum( m_pUcData->m_PrVal );			// カンマを除く　2011.07.15 h.ukawa
//			calcTemp += atoi(pstrCsvDataAry->GetAt(10));
			calcTemp += atoi(m_pUcData->m_PrVal);				//2011.07.15
		}

		// 11 事前確定届出給与
		if (pstrCsvDataAry->GetAt(11).IsEmpty() != TRUE) {
			m_pUcData->m_BfVal = pstrCsvDataAry->GetAt(11);
			RemoveCommaImportNum( m_pUcData->m_BfVal );			// カンマを除く　2011.07.15 h.ukawa
//			calcTemp += atoi(pstrCsvDataAry->GetAt(11));
			calcTemp += atoi(m_pUcData->m_BfVal);				//2011.07.15
		}

		// 12 利益連動給与
		if (pstrCsvDataAry->GetAt(12).IsEmpty() != TRUE) {
			m_pUcData->m_PfVal = pstrCsvDataAry->GetAt(12);
			RemoveCommaImportNum( m_pUcData->m_PfVal );			// カンマを除く　2011.07.15 h.ukawa
//			calcTemp += atoi(pstrCsvDataAry->GetAt(12));
			calcTemp += atoi(m_pUcData->m_PfVal);				//2011.07.15
		}

		// 13 その他
		if (pstrCsvDataAry->GetAt(13).IsEmpty() != TRUE) {
			m_pUcData->m_EtVal = pstrCsvDataAry->GetAt(13);
			RemoveCommaImportNum( m_pUcData->m_EtVal );			// カンマを除く　2011.07.15 h.ukawa
//			calcTemp += atoi(pstrCsvDataAry->GetAt(13));
			calcTemp += atoi(m_pUcData->m_EtVal);				//2011.07.15
		}

		// 14 退職給与
		m_pUcData->m_RtVal = pstrCsvDataAry->GetAt(14);
		RemoveCommaImportNum( m_pUcData->m_RtVal );			// カンマを除く　2011.07.15 h.ukawa

		// 15 役員給与計
		if (pstrCsvDataAry->GetAt(15).IsEmpty() != TRUE) {
// midori 154673 del -->
			//m_pUcData->m_FgInput = 1;			// 手入力
// midori 154673 del <--
// midori 154673 add -->
			_i64toa_s(calcTemp,chrTemp,sizeof(chrTemp),10);
			strTemp.Format(_T("%s"),chrTemp);
			if(pstrCsvDataAry->GetAt(15) == strTemp)	{
				// 自動計算
				m_pUcData->m_FgInput = 0;
			}
			else	{
				// 実額入力
				m_pUcData->m_FgInput = 1;
			}
// midori 154673 add <--
			m_pUcData->m_Val = pstrCsvDataAry->GetAt(15);
			RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa
		} else {
			m_pUcData->m_FgInput = 0;			// 自動計算
			_i64toa_s(calcTemp, chrTemp, sizeof(chrTemp), 10);
			m_pUcData->m_Val = chrTemp;
		}
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
int CimpUc141Yakuin::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc141Yakuin*)m_pUcCmn;


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CString			strTemp			= _T("");		// 文字列テンポラリ

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// [明細]の場合のみ各項目のデータを取得
	// [小計]、[中計]は識別子のみ
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
		// 01 役職名
		pstrCsvDataAry->SetAt( 1, m_pUcData->m_NameYaku);

		// 02 担当職務
		pstrCsvDataAry->SetAt( 2, m_pUcData->m_NameTant);

		// 03 氏名１
		pstrCsvDataAry->SetAt( 3, m_pUcData->m_AdName1);

		// 04 氏名２
		pstrCsvDataAry->SetAt( 4, m_pUcData->m_AdName2);

		// 05 住所１
		pstrCsvDataAry->SetAt( 5, m_pUcData->m_AdAdd1);

		// 06 住所２
		pstrCsvDataAry->SetAt( 6, m_pUcData->m_AdAdd2);

		// 07 代表者との関係
		pstrCsvDataAry->SetAt( 7, m_pUcData->m_Kankei);

		// 08 常勤・非常勤の別
		strTemp = _T("");
		ConvFpTimeDbValToStr(m_pUcData->m_FpTime, &strTemp);
		pstrCsvDataAry->SetAt( 8, strTemp);

		// 09 使用人職務分
		pstrCsvDataAry->SetAt( 9, m_pUcData->m_EmVal);

		// 10 定期同額給与
		pstrCsvDataAry->SetAt(10, m_pUcData->m_PrVal);

		// 11 事前確定届出給与
		pstrCsvDataAry->SetAt(11, m_pUcData->m_BfVal);

		// 12 利益連動給与
		pstrCsvDataAry->SetAt(12, m_pUcData->m_PfVal);

		// 13 その他
		pstrCsvDataAry->SetAt(13, m_pUcData->m_EtVal);

		// 14 退職給与
		pstrCsvDataAry->SetAt(14, m_pUcData->m_RtVal);

		// 15 役員給与計
		pstrCsvDataAry->SetAt(15, m_pUcData->m_Val);
	}

	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}

//**********************************************************************
// 常勤・非常勤の別を文字列→DBデータに変換
//		・データインポートで使用。
//		・文字列とDBデータの対応。
//			--------------------------------------------------
//			No.	文字列			DBデータ	備考
//			--------------------------------------------------
//			1	“”			0			未選択
//			2	“常”			1			常勤
//			3	“非”			2			非常勤
//			4	その他の文字列	0			関数失敗
//			--------------------------------------------------
// 
//	引数
//			strFpTime				[IN] 文字列
//			pcFpTime				[OUT]DBデータ（常勤・非常勤の別）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
int CimpUc141Yakuin::ConvFpTimeStrToDbVal(CString strFpTime, BYTE* pcFpTime)
{
	// 未選択？
	if (strFpTime.Compare(CSV_141_FPTIME_NULL) == 0) {
		*pcFpTime = 0;
		return TRUE;
	}

	// 常勤？
	if (strFpTime.Compare(CSV_141_FPTIME_FULL) == 0) {
		*pcFpTime = 1;
		return TRUE;
	}

	// 非常勤？
	if (strFpTime.Compare(CSV_141_FPTIME_PART) == 0) {
		*pcFpTime = 2;
		return TRUE;
	}

	// インポート対象外
	*pcFpTime = 0;
// midori 150972 cor -->
//// midori 150904 add -->
//	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_142_KUBUN);
//// midori 150904 add <--
// ---------------------
	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_141_KUBUN);
// midori 150972 cor <--
	return FALSE;
}

//**********************************************************************
// 常勤・非常勤の別をDBデータ→文字列に変換
//		・データエクスポートで使用。
//		・DBデータと文字列の対応。
//			--------------------------------------------------
//			No.	DBデータ		文字列		備考
//			--------------------------------------------------
//			1	0				“”		未選択
//			2	1				“常”		常勤
//			3	2				“非”		非常勤
//			4	その他の値		“”		関数失敗
//			--------------------------------------------------
// 
//	引数
//			cFpTime					[IN] DBデータ（常勤・非常勤の別）
//			pstrFpTime				[OUT]文字列
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
int CimpUc141Yakuin::ConvFpTimeDbValToStr(BYTE cFpTime, CString* pstrFpTime)
{
	// 常勤・非常勤の別？
	switch (cFpTime) {
	case	0:		// 未選択
		*pstrFpTime = CSV_141_FPTIME_NULL;
		return TRUE;
	case	1:		// 常勤
		*pstrFpTime = CSV_141_FPTIME_FULL;
		return TRUE;
	case	2:		// 非常勤
		*pstrFpTime = CSV_141_FPTIME_PART;
		return TRUE;
	}

	// エクスポート対象外
	*pstrFpTime = CSV_141_FPTIME_NULL;
	return FALSE;
}