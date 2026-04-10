#include "StdAfx.h"
#include "impUc071Koteisisan.h"

// No.200903 add -->
extern BOOL	bG_Kanso;
// No.200903 add <--
// 登録番号（法人番号）_23/11/21 add -->
extern BOOL	bG_InvNo;
// 登録番号（法人番号）_23/11/21 add <--
// 24/02/20_ScoreLink add -->
extern BOOL	bG_Sl;
// 24/02/20_ScoreLink add <--

// コンストラクタ
CimpUc071Koteisisan::CimpUc071Koteisisan(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc071Koteisisan::~CimpUc071Koteisisan(void)
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
int CimpUc071Koteisisan::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_071;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_071_ITEM_CNT;

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
	m_pUcCmn = (CdbUc000Common*)(new CdbUc071Koteishisan(pDB));

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
	m_RowMax		= ID_ROWKOBETU_071;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_071 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_071;


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
int CimpUc071Koteisisan::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc071Koteishisan*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++

// 登録番号（法人番号）_23/11/21 cor -->
	int	ofset=1;
// 登録番号（法人番号）_23/11/21 cor <--

	// 01 種類・構造１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_071_SYURUI) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_SYURUI) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_01);
		m_uImpCsvInfo.nLength = MAX_KETA_071_SYURUI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 02 種類・構造２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_071_SYURUI) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_SYURUI) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_02);
		m_uImpCsvInfo.nLength = MAX_KETA_071_SYURUI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 用途１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_071_YOUTO) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_YOUTO) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_03);
		m_uImpCsvInfo.nLength = MAX_KETA_071_YOUTO/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 04 用途２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_071_YOUTO) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_YOUTO) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_04);
		m_uImpCsvInfo.nLength = MAX_KETA_071_YOUTO/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 05 面積
	if(bG_Kanso == TRUE)	len = 8;
	else					len = 6;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(5), len, 2, FALSE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, 2, FALSE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_05);
		m_uImpCsvInfo.nLength	= len;	// 整数最大桁数
		m_uImpCsvInfo.nLength2	= 2;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 06 物件の所在地１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_071_AD2ADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_AD2ADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_06);
		m_uImpCsvInfo.nLength = MAX_KETA_071_AD2ADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 07 物件の所在地２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(7), MAX_KETA_071_AD2ADD2*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_AD2ADD2*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_07);
		m_uImpCsvInfo.nLength = MAX_KETA_071_AD2ADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 08 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_071_K_DATA;
	else					len = ID_VAL_071_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(8), len, TRUE) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_08);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
// 登録番号（法人番号）_23/11/21 cor -->
		//if(pstrCsvDataAry->GetAt(8).Left(1) == '-')	{
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

	// 09 異動年月日
	long nSpDate = 0;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(9) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(9), &nSpDate) != TRUE) {
// -------------------------------------
	ofset++;
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nSpDate) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_09);
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}
	}

	// 10 異動事由１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(10), MAX_KETA_071_RIYUU) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_RIYUU) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_10);
		m_uImpCsvInfo.nLength = MAX_KETA_071_RIYUU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 11 異動事由２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(11), MAX_KETA_071_RIYUU) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_RIYUU) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_11);
		m_uImpCsvInfo.nLength = MAX_KETA_071_RIYUU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 12 取得（処分）価額
	if(bG_Kanso == TRUE)	len = ID_MSVAL_071_K_DATA;
	else					len = ID_MSVAL_071_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(12), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_12);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
// 登録番号（法人番号）_23/11/21 cor -->
		//if(pstrCsvDataAry->GetAt(12).Left(1) == '-')	{
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

	// 13 異動直前の帳簿価額
	if(bG_Kanso == TRUE)	len = ID_MSVAL2_071_K_DATA;
	else					len = ID_MSVAL2_071_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(13), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_13);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
// 登録番号（法人番号）_23/11/21 cor -->
		//if(pstrCsvDataAry->GetAt(13).Left(1) == '-')	{
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

// 登録番号（法人番号）_23/11/21 add -->
	ofset++;
	if(m_NewVer2 == TRUE)	{
		// 19 登録番号（法人番号）
		if(bG_InvNo == TRUE)	{
			CString	cs=_T("");
			cs = pstrCsvDataAry->GetAt(ofset);
			if(CheckImportTno(cs) != TRUE)	{
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_042_ITEM_13);
				m_uImpCsvInfo.nLength = 0;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
				// 登録番号（法人番号）は桁数までのみ取り込むことをしないので
				// ここでクリアする
				pstrCsvDataAry->SetAt(ofset,_T(""));
			}
			else	{
				// 先頭が "T" ではない場合、半角スペースが挿入されているので再セット
				// "T6210001025986" → "T6210001025986"
				// "6210001025986"  → " 6210001025986"
				pstrCsvDataAry->SetAt(ofset,cs);
			}
		}
		ofset++;
	}
// 登録番号（法人番号）_23/11/21 add <--

	// 14 売却（購入）先の名称（氏名）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(14), MAX_KETA_071_ADNAME1) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_14);
		m_uImpCsvInfo.nLength = MAX_KETA_071_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 15 売却（購入）先の名称（氏名）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(15), MAX_KETA_071_ADNAME1) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_15);
		m_uImpCsvInfo.nLength = MAX_KETA_071_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 16 売却（購入）先の所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(16), MAX_KETA_071_ADADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_ADADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_16);
		m_uImpCsvInfo.nLength = MAX_KETA_071_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 17 売却（購入）先の所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(17), MAX_KETA_071_ADADD2*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_071_ADADD2*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_17);
		m_uImpCsvInfo.nLength = MAX_KETA_071_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 18 売却物件の取得年月
	long nSpDate2 = 0;
// 登録番号（法人番号）_23/11/21 cor -->
	//if ( pstrCsvDataAry->GetAt(18) != _T("")){
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_C, pstrCsvDataAry->GetAt(18), &nSpDate2) != TRUE) {
// -------------------------------------
	ofset++;
	if ( pstrCsvDataAry->GetAt(ofset) != _T("")){
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_C, pstrCsvDataAry->GetAt(ofset), &nSpDate2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_071_ITEM_18);
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}
	}

	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}

// midori 180404 del -->
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 名称テーブルへ登録
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 
	//// TODO: ここに各帳票固有の処理を記述！
	//// 


	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// データ加工
	//// テーブルへデータ登録
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//CString			strTemp			= _T("");		// 文字列テンポラリ

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
	//	// 01 種類・構造１
	//	// 02 種類・構造２
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1), pstrCsvDataAry->GetAt(2));
	//	m_pUcData->m_Syurui		= strTemp;
	//	
	//	// 03 用途１
	//	// 04 用途２
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(3), pstrCsvDataAry->GetAt(4));
	//	m_pUcData->m_Youto		= strTemp;

	//	// 05 面積
	//	strTemp = _T("");
	//	strTemp = CalcStrMul(pstrCsvDataAry->GetAt(5), 100, 0);	// ×100
	//	m_pUcData->m_Area		= strTemp;

	//	// 06 物件の所在地１
	//	m_pUcData->m_Ad2Add1	= pstrCsvDataAry->GetAt(6);
	//	
	//	// 07 物件の所在地２
	//	m_pUcData->m_Ad2Add2	= pstrCsvDataAry->GetAt(7);

	//	// 08 期末現在高
	//	m_pUcData->m_Val		= pstrCsvDataAry->GetAt(8);
	//	RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

	//	// 09 異動年月日
	//	m_pUcData->m_SpDate		= nSpDate;

	//	// 10 異動事由１
	//	// 11 異動事由２		
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(10), pstrCsvDataAry->GetAt(11));
	//	m_pUcData->m_Riyuu		= strTemp;

	//	// 12 取得（処分）価額
	//	m_pUcData->m_MsVal		= pstrCsvDataAry->GetAt(12);
	//	RemoveCommaImportNum( m_pUcData->m_MsVal );				// カンマを除く　2011.07.15 h.ukawa

	//	// 13 異動直前の帳簿価額
	//	m_pUcData->m_MsVal2			= pstrCsvDataAry->GetAt(13);
	//	RemoveCommaImportNum( m_pUcData->m_MsVal2 );				// カンマを除く　2011.07.15 h.ukawa

	//	// 14 売却（購入）先の名称（氏名）１
	//	m_pUcData->m_AdName1		= pstrCsvDataAry->GetAt(14);

	//	// 15 売却（購入）先の名称（氏名）２
	//	m_pUcData->m_AdName2		= pstrCsvDataAry->GetAt(15);

	//	// 16 売却（購入）先の所在地（住所）１
	//	m_pUcData->m_AdAdd1			= pstrCsvDataAry->GetAt(16);

	//	// 17 売却（購入）先の所在地（住所）２
	//	m_pUcData->m_AdAdd2			= pstrCsvDataAry->GetAt(17);

	//	// 18 売却物件の取得年月
	//	m_pUcData->m_SpDate2		= nSpDate2;
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
int CimpUc071Koteisisan::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// No.200903 add -->
	int				len=0;
// No.200903 add <--
// 登録番号（法人番号）_23/11/21 add -->
	int				ofset=1;
	int				save=0;
	CString			cs=_T("");
// 登録番号（法人番号）_23/11/21 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

	// 文字数をオーバーしていればカットする

	// 01 種類・構造１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_071_SYURUI);
	//pstrCsvDataAry->SetAt(1,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_SYURUI);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 02 種類・構造２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_071_SYURUI);
	//pstrCsvDataAry->SetAt(2,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_SYURUI);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 03 用途１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_071_YOUTO);
	//pstrCsvDataAry->SetAt(3,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_YOUTO);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 04 用途２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_071_YOUTO);
	//pstrCsvDataAry->SetAt(4,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_YOUTO);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 05 面積
	if(bG_Kanso == TRUE)	len = 8;
	else					len = 6;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(5), len, 2, FALSE) != TRUE)	{
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(5,strTemp);
	//}
// -------------------------------------
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, 2, FALSE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 06 物件の所在地１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_071_AD2ADD1*2);
	//pstrCsvDataAry->SetAt(6,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_AD2ADD1*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 07 物件の所在地２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(7),&strTemp,MAX_KETA_071_AD2ADD2*2);
	//pstrCsvDataAry->SetAt(7,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_AD2ADD2*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 08 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_071_K_DATA;
	else					len = ID_VAL_071_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(8), len, TRUE) != TRUE)	{
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(8,strTemp);
	//}
// -------------------------------------
	if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 09 異動年月日
	long nSpDate = 0;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(9) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(9), &nSpDate) != TRUE) {
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(9,strTemp);
	//	}
	//}
// -------------------------------------
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nSpDate) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
		}
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 10 異動事由１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(10),&strTemp,MAX_KETA_071_RIYUU);
	//pstrCsvDataAry->SetAt(10,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_RIYUU);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 11 異動事由２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(11),&strTemp,MAX_KETA_071_RIYUU);
	//pstrCsvDataAry->SetAt(11,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_RIYUU);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 12 取得（処分）価額
	if(bG_Kanso == TRUE)	len = ID_MSVAL_071_K_DATA;
	else					len = ID_MSVAL_071_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(12), len, TRUE) != TRUE)	{
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(12,strTemp);
	//}
// -------------------------------------
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 13 異動直前の帳簿価額
	if(bG_Kanso == TRUE)	len = ID_MSVAL2_071_K_DATA;
	else					len = ID_MSVAL2_071_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(13), len, TRUE) != TRUE)	{
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(13,strTemp);
	//}
// -------------------------------------
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;

	// 19 登録番号（法人番号）
	if(m_NewVer2 == TRUE)	{
		ofset++;
	}
// 登録番号（法人番号）_23/11/21 cor <--

	// 14 売却（購入）先の名称（氏名）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(14),&strTemp,MAX_KETA_071_ADNAME1);
	//pstrCsvDataAry->SetAt(14,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_ADNAME1);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 15 売却（購入）先の名称（氏名）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(15),&strTemp,MAX_KETA_071_ADNAME1);
	//pstrCsvDataAry->SetAt(15,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_ADNAME1);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 16 売却（購入）先の所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(16),&strTemp,MAX_KETA_071_ADADD1*2);
	//pstrCsvDataAry->SetAt(16,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_ADADD1*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 17 売却（購入）先の所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(17),&strTemp,MAX_KETA_071_ADADD2*2);
	//pstrCsvDataAry->SetAt(17,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_071_ADADD2*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 18 売却物件の取得年月
	long nSpDate2 = 0;
// 登録番号（法人番号）_23/11/21 cor -->
	//if ( pstrCsvDataAry->GetAt(18) != _T("")){
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_C, pstrCsvDataAry->GetAt(18), &nSpDate2) != TRUE) {
	//		strTemp.Empty();
	//		pstrCsvDataAry->SetAt(18,strTemp);
	//	}
	//}
// -------------------------------------
	if ( pstrCsvDataAry->GetAt(ofset) != _T("")){
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_C, pstrCsvDataAry->GetAt(ofset), &nSpDate2) != TRUE) {
			strTemp.Empty();
			pstrCsvDataAry->SetAt(ofset,strTemp);
		}
	}
	ofset++;
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
	m_pUcData = (CdbUc071Koteishisan*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 
	// TODO: ここに各帳票固有の処理を記述！
	// 

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
		//// 01 種類・構造１
		//// 02 種類・構造２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(1), pstrCsvDataAry->GetAt(2));
		//m_pUcData->m_Syurui		= strTemp;
		//
		//// 03 用途１
		//// 04 用途２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(3), pstrCsvDataAry->GetAt(4));
		//m_pUcData->m_Youto		= strTemp;

		//// 05 面積
		//strTemp = _T("");
		//strTemp = CalcStrMul(pstrCsvDataAry->GetAt(5), 100, 0);	// ×100
		//m_pUcData->m_Area		= strTemp;

		//// 06 物件の所在地１
		//m_pUcData->m_Ad2Add1	= pstrCsvDataAry->GetAt(6);
		//
		//// 07 物件の所在地２
		//m_pUcData->m_Ad2Add2	= pstrCsvDataAry->GetAt(7);

		//// 08 期末現在高
		//m_pUcData->m_Val		= pstrCsvDataAry->GetAt(8);
		//RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		//// 09 異動年月日
		//m_pUcData->m_SpDate		= nSpDate;

		//// 10 異動事由１
		//// 11 異動事由２		
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(10), pstrCsvDataAry->GetAt(11));
		//m_pUcData->m_Riyuu		= strTemp;

		//// 12 取得（処分）価額
		//m_pUcData->m_MsVal		= pstrCsvDataAry->GetAt(12);
		//RemoveCommaImportNum( m_pUcData->m_MsVal );				// カンマを除く　2011.07.15 h.ukawa

		//// 13 異動直前の帳簿価額
		//m_pUcData->m_MsVal2			= pstrCsvDataAry->GetAt(13);
		//RemoveCommaImportNum( m_pUcData->m_MsVal2 );				// カンマを除く　2011.07.15 h.ukawa

		//// 14 売却（購入）先の名称（氏名）１
		//m_pUcData->m_AdName1		= pstrCsvDataAry->GetAt(14);

		//// 15 売却（購入）先の名称（氏名）２
		//m_pUcData->m_AdName2		= pstrCsvDataAry->GetAt(15);

		//// 16 売却（購入）先の所在地（住所）１
		//m_pUcData->m_AdAdd1			= pstrCsvDataAry->GetAt(16);

		//// 17 売却（購入）先の所在地（住所）２
		//m_pUcData->m_AdAdd2			= pstrCsvDataAry->GetAt(17);

		//// 18 売却物件の取得年月
		//m_pUcData->m_SpDate2		= nSpDate2;
// -------------------------------------
		ofset=1;

		// 01 種類・構造１
		// 02 種類・構造２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		m_pUcData->m_Syurui		= strTemp;

		ofset++;
		ofset++;
		
		// 03 用途１
		// 04 用途２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		m_pUcData->m_Youto		= strTemp;

		ofset++;
		ofset++;

		// 05 面積
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(ofset), 100, 0);	// ×100
		m_pUcData->m_Area		= strTemp;

		ofset++;

		// 06 物件の所在地１
		m_pUcData->m_Ad2Add1	= pstrCsvDataAry->GetAt(ofset);

		ofset++;
		
		// 07 物件の所在地２
		m_pUcData->m_Ad2Add2	= pstrCsvDataAry->GetAt(ofset);

		ofset++;

		// 08 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		ofset++;

		// 09 異動年月日
		m_pUcData->m_SpDate		= nSpDate;

		ofset++;

		// 10 異動事由１
		// 11 異動事由２		
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		m_pUcData->m_Riyuu		= strTemp;

		ofset++;
		ofset++;

		// 12 取得（処分）価額
		m_pUcData->m_MsVal		= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_MsVal );				// カンマを除く　2011.07.15 h.ukawa

		ofset++;

		// 13 異動直前の帳簿価額
		m_pUcData->m_MsVal2			= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_MsVal2 );				// カンマを除く　2011.07.15 h.ukawa

		ofset++;

		// 19 登録番号（法人番号）
		if(m_NewVer2 == TRUE)	{
			if(bG_InvNo == TRUE)	{
// 231222_168474 add -->
				cs=pstrCsvDataAry->GetAt(ofset);
				if(CheckImportTno(cs) != TRUE)	{
					cs = _T("");
					pstrCsvDataAry->SetAt(ofset,cs);
				}
// 231222_168474 add <--
				m_pUcData->m_InvNo = pstrCsvDataAry->GetAt(ofset);
			}
			ofset++;
		}

		// 14 売却（購入）先の名称（氏名）１
		m_pUcData->m_AdName1		= pstrCsvDataAry->GetAt(ofset);

		ofset++;

		// 15 売却（購入）先の名称（氏名）２
		m_pUcData->m_AdName2		= pstrCsvDataAry->GetAt(ofset);

		ofset++;

		// 16 売却（購入）先の所在地（住所）１
		m_pUcData->m_AdAdd1			= pstrCsvDataAry->GetAt(ofset);

		ofset++;

		// 17 売却（購入）先の所在地（住所）２
		m_pUcData->m_AdAdd2			= pstrCsvDataAry->GetAt(ofset);

		ofset++;

		// 18 売却物件の取得年月
		m_pUcData->m_SpDate2		= nSpDate2;

		ofset++;
// 登録番号（法人番号）_23/11/21 cor <--
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
int CimpUc071Koteisisan::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc071Koteishisan*)m_pUcCmn;


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

	// [明細]の場合のみ各項目のデータを取得
	// [小計]、[中計]は識別子のみ
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
// 登録番号（法人番号）_23/11/21 cor -->
		//// 01 種類・構造１
		//// 02 種類・構造２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Syurui, &strTempAry, 2);
		//pstrCsvDataAry->SetAt(1, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(2, strTempAry.GetAt(1));
		//		
		//// 03 用途１
		//// 04 用途２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Youto, &strTempAry, 2);
		//pstrCsvDataAry->SetAt(3, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(4, strTempAry.GetAt(1));

		//// 05 面積
		//strTemp = _T("");
		//strTemp = CalcStrMul(m_pUcData->m_Area, 0.01, 2);	// ÷100
		//pstrCsvDataAry->SetAt(5, strTemp);

		//// 06 物件の所在地１
		//pstrCsvDataAry->SetAt(6, m_pUcData->m_Ad2Add1);
		//
		//// 07 物件の所在地２
		//pstrCsvDataAry->SetAt(7, m_pUcData->m_Ad2Add2);

		//// 08 期末現在高
		//pstrCsvDataAry->SetAt(8, m_pUcData->m_Val);

		//// 09 異動年月日
		//strTemp = _T("");
		//if (m_pUcData->m_SpDate > 0) {
		//	ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strTemp);
		//}
		//pstrCsvDataAry->SetAt(9, strTemp);

		//// 10 異動事由１
		//// 11 異動事由２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Riyuu, &strTempAry, 2);
		//pstrCsvDataAry->SetAt(10, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(11, strTempAry.GetAt(1));

		//// 12 取得（処分）価額
		//pstrCsvDataAry->SetAt(12, m_pUcData->m_MsVal);

		//// 13 異動直前の帳簿価額
		//pstrCsvDataAry->SetAt(13, m_pUcData->m_MsVal2);

		//// 14 売却（購入）先の名称（氏名）１
		//pstrCsvDataAry->SetAt(14, m_pUcData->m_AdName1);

		//// 15 売却（購入）先の名称（氏名）２
		//pstrCsvDataAry->SetAt(15, m_pUcData->m_AdName2);

		//// 16 売却（購入）先の所在地（住所）１
		//pstrCsvDataAry->SetAt(16, m_pUcData->m_AdAdd1);

		//// 17 売却（購入）先の所在地（住所）２
		//pstrCsvDataAry->SetAt(17, m_pUcData->m_AdAdd2);

		//// 18 売却物件の取得年月
		//strTemp = _T("");
		//if (m_pUcData->m_SpDate2 > 0) {
		//	ConvDateDbValToStr(IMP_DATE_CONV_TYPE_C, m_pUcData->m_SpDate2, &strTemp);
		//}
		//pstrCsvDataAry->SetAt(18, strTemp);
// -------------------------------------
		rno=1;

		// 01 種類・構造１
		// 02 種類・構造２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Syurui, &strTempAry, 2);
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(1));
		rno++;
		// 03 用途１
		// 04 用途２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Youto, &strTempAry, 2);
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(1));
		rno++;
		// 05 面積
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Area, 0.01, 2);	// ÷100
		pstrCsvDataAry->SetAt(rno, strTemp);
		rno++;
		// 06 物件の所在地１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Ad2Add1);
		rno++;
		// 07 物件の所在地２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Ad2Add2);
		rno++;
		// 08 期末現在高
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Val);
		rno++;
		// 09 異動年月日
		strTemp = _T("");
		if (m_pUcData->m_SpDate > 0) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strTemp);
		}
		pstrCsvDataAry->SetAt(rno, strTemp);
		rno++;
		// 10 異動事由１
		// 11 異動事由２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Riyuu, &strTempAry, 2);
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(1));
		rno++;
		// 12 取得（処分）価額
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_MsVal);
		rno++;
		// 13 異動直前の帳簿価額
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_MsVal2);
		rno++;
// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 19 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}
		// 14 売却（購入）先の名称（氏名）１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName1);
		rno++;
		// 15 売却（購入）先の名称（氏名）２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName2);
		rno++;
		// 16 売却（購入）先の所在地（住所）１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdAdd1);
		rno++;
		// 17 売却（購入）先の所在地（住所）２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdAdd2);
		rno++;
		// 18 売却物件の取得年月
		strTemp = _T("");
		if (m_pUcData->m_SpDate2 > 0) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_C, m_pUcData->m_SpDate2, &strTemp);
		}
		pstrCsvDataAry->SetAt(rno, strTemp);
		rno++;
// 登録番号（法人番号）_23/11/21 cor <--
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}