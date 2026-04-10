#include "StdAfx.h"
#include "impUc152Kenrikin.h"

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
CimpUc152Kenrikin::CimpUc152Kenrikin(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc152Kenrikin::~CimpUc152Kenrikin(void)
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
int CimpUc152Kenrikin::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_152;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_152_ITEM_CNT;

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
	m_pUcCmn = (CdbUc000Common*)(new CdbUc152Kenrikin(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );

	// inf_form_typeテーブルの初期化
	m_pUcInfFormType->Init( ID_FORMNO_15 );


	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_152;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_152 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_152;

	// 出力設定が通常の場合
	if( m_pUcInfFormType->m_FormType == 0 ){
		m_RowMax		= ID_ROWNORMAL_152;
		m_RowMaxData	= ID_ROWNORMAL_152 - 1;
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
int CimpUc152Kenrikin::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc152Kenrikin*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++

// 登録番号（法人番号）_23/11/21 cor -->
	int	ofset=1;

	// 11 登録番号（法人番号）
	if(m_NewVer2 == TRUE)	{
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
// 登録番号（法人番号）_23/11/21 cor <--

	// 01 支払先の名称（氏名）
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_152_ADNAME1*2) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_ADNAME1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_01);
		m_uImpCsvInfo.nLength = MAX_KETA_152_ADNAME1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 02 支払先の所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_152_ADADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_ADADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_02);
		m_uImpCsvInfo.nLength = MAX_KETA_152_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 支払先の所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_152_ADADD2*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_ADADD2*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_03);
		m_uImpCsvInfo.nLength = MAX_KETA_152_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 04 支払年月日
	long nSpDate = 0;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(4) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(4), &nSpDate) != TRUE) {
// -------------------------------------
	ofset++;
	if (pstrCsvDataAry->GetAt(ofset) != _T("")) {
		if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(ofset), &nSpDate) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
			strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_04);
			m_uImpCsvInfo.nKind = 3;	// 3:日付
			m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
			CsvInfoClr(&m_uImpCsvInfo);
			nRet = IMP_REC_NG;
		}
	}

	// 05 支払金額
	if(bG_Kanso == TRUE)	len = ID_VAL_152_K_DATA;
	else					len = ID_VAL_152_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(5), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
// -------------------------------------
	ofset++;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_05);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
// 登録番号（法人番号）_23/11/21 cor -->
		//if(pstrCsvDataAry->GetAt(5).Left(1) == '-')	{
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

	// 06 権利金等の内容１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_152_NAIYOU) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_NAIYOU) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_06);
		m_uImpCsvInfo.nLength = MAX_KETA_152_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 07 権利金等の内容２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(7), MAX_KETA_152_NAIYOU) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_NAIYOU) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_07);
		m_uImpCsvInfo.nLength = MAX_KETA_152_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 08 摘要１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(8), MAX_KETA_152_TEKI/3*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_TEKI/3*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_08);
		m_uImpCsvInfo.nLength = MAX_KETA_152_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 09 摘要２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(9), MAX_KETA_152_TEKI/3*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_TEKI/3*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_09);
		m_uImpCsvInfo.nLength = MAX_KETA_152_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 10 摘要３
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(10), MAX_KETA_152_TEKI/3*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_152_TEKI/3*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_152_ITEM_10);
		m_uImpCsvInfo.nLength = MAX_KETA_152_TEKI/3;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}

// midori 180404 del -->
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 名称テーブルへ登録
	////++++++++++++++++++++++++++++++++++++++++++++++++++
	//// 名称テーブル登録無し

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
	//	// 01 支払先の名称（氏名）
	//	m_pUcData->m_AdSeq		= 0;
	//	m_pUcData->m_AdOrder	= 0;
	//	m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
	//	m_pUcData->m_AdName2	= "";

	//	// 02 支払先の所在地（住所）１
	//	m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(2);

	//	// 03 支払先の所在地（住所）２
	//	m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(3);

	//	// 04 支払年月日
	//	m_pUcData->m_SpDate		= nSpDate;

	//	// 05 支払金額
	//	m_pUcData->m_Val		= pstrCsvDataAry->GetAt(5);
	//	RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa

	//	// 06 権利金等の内容１
	//	// 07 権利金等の内容２
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(6), pstrCsvDataAry->GetAt(7));
	//	m_pUcData->m_Naiyou		= strTemp;

	//	// 08 摘要１
	//	// 09 摘要２
	//	// 10 摘要３
	//	strTemp = _T("");
	//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
	//	strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(10));
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
int CimpUc152Kenrikin::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// No.200903 add -->
	int		len=0;
// No.200903 add <--

	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// 登録番号（法人番号）_23/11/21 add -->
	int				ofset=1;
	int				save=0;
	CString			cs=_T("");
// 登録番号（法人番号）_23/11/21 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

// 登録番号（法人番号）_23/11/21 add -->
	// 11 登録番号（法人番号）
	if(m_NewVer2 == TRUE)	{
		ofset++;
	}
// 登録番号（法人番号）_23/11/21 add <--

	// 文字数をオーバーしていればカットする
	// 01 支払先の名称（氏名）
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_152_ADNAME1*2);
	//pstrCsvDataAry->SetAt(1,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_ADNAME1*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 02 支払先の所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_152_ADADD1*2);
	//pstrCsvDataAry->SetAt(2,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_ADADD1*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 03 支払先の所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_152_ADADD2*2);
	//pstrCsvDataAry->SetAt(3,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_ADADD2*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 04 支払年月日
	long nSpDate = 0;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (pstrCsvDataAry->GetAt(4) != _T("")) {
	//	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(4), &nSpDate) != TRUE) {
	//		strTemp.Empty();
	//		pstrCsvDataAry->SetAt(4,strTemp);
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

	// 05 支払金額
	if(bG_Kanso == TRUE)	len = ID_VAL_152_K_DATA;
	else					len = ID_VAL_152_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(5), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(5,strTemp);
	//}
// -------------------------------------
	if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 06 権利金等の内容１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_152_NAIYOU);
	//pstrCsvDataAry->SetAt(6,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_NAIYOU);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 07 権利金等の内容２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(7),&strTemp,MAX_KETA_152_NAIYOU);
	//pstrCsvDataAry->SetAt(7,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_NAIYOU);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 08 摘要１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(8),&strTemp,MAX_KETA_152_TEKI/3*2);
	//pstrCsvDataAry->SetAt(8,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_TEKI/3*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 09 摘要２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(9),&strTemp,MAX_KETA_152_TEKI/3*2);
	//pstrCsvDataAry->SetAt(9,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_TEKI/3*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 10 摘要３
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(10),&strTemp,MAX_KETA_152_TEKI/3*2);
	//pstrCsvDataAry->SetAt(10,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_152_TEKI/3*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
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
	m_pUcData = (CdbUc152Kenrikin*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブル登録無し

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
		//// 01 支払先の名称（氏名）
		//m_pUcData->m_AdSeq		= 0;
		//m_pUcData->m_AdOrder	= 0;
		//m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
		//m_pUcData->m_AdName2	= "";

		//// 02 支払先の所在地（住所）１
		//m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(2);

		//// 03 支払先の所在地（住所）２
		//m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(3);

		//// 04 支払年月日
		//m_pUcData->m_SpDate		= nSpDate;

		//// 05 支払金額
		//m_pUcData->m_Val		= pstrCsvDataAry->GetAt(5);
		//RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa

		//// 06 権利金等の内容１
		//// 07 権利金等の内容２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(6), pstrCsvDataAry->GetAt(7));
		//m_pUcData->m_Naiyou		= strTemp;

		//// 08 摘要１
		//// 09 摘要２
		//// 10 摘要３
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
		//strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(10));
		//m_pUcData->m_Teki		= strTemp;
// -------------------------------------
		ofset=1;
		// 11 登録番号（法人番号）
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
		// 01 支払先の名称（氏名）
		m_pUcData->m_AdSeq		= 0;
		m_pUcData->m_AdOrder	= 0;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(ofset);
		m_pUcData->m_AdName2	= "";
		ofset++;
		// 02 支払先の所在地（住所）１
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		// 03 支払先の所在地（住所）２
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		// 04 支払年月日
		m_pUcData->m_SpDate		= nSpDate;
		ofset++;
		// 05 支払金額
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa
		ofset++;
		// 06 権利金等の内容１
		// 07 権利金等の内容２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		m_pUcData->m_Naiyou		= strTemp;
		ofset++;
		ofset++;
		// 08 摘要１
		// 09 摘要２
		// 10 摘要３
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		strTemp = m_clsFunc.StrDocking(strTemp, pstrCsvDataAry->GetAt(ofset+2));
		m_pUcData->m_Teki		= strTemp;
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
int CimpUc152Kenrikin::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc152Kenrikin*)m_pUcCmn;


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
	// [小計]、[中計]は識別子のみ
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
// 登録番号（法人番号）_23/11/21 cor -->
		//// 01 支払先の名称（氏名）
		//pstrCsvDataAry->SetAt( 1, m_pUcData->m_AdName1);

		//// 02 支払先の所在地（住所）１
		//pstrCsvDataAry->SetAt( 2, m_pUcData->m_AdAdd1);

		//// 03 支払先の所在地（住所）２
		//pstrCsvDataAry->SetAt( 3, m_pUcData->m_AdAdd2);

		//// 04 支払年月日
		//CString csSpDate = _T("");
		//if (m_pUcData->m_SpDate > 0) {
		//	ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &csSpDate);
		//}
		//pstrCsvDataAry->SetAt( 4, csSpDate);

		//// 05 支払金額
		//pstrCsvDataAry->SetAt( 5, m_pUcData->m_Val);

		//// 06 権利金等の内容１
		//// 07 権利金等の内容２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 2);
		//pstrCsvDataAry->SetAt( 6, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt( 7, strTempAry.GetAt(1));

		//// 08 摘要１
		//// 09 摘要２
		//// 10 摘要３
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 3);
		//pstrCsvDataAry->SetAt( 8, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt( 9, strTempAry.GetAt(1));
		//pstrCsvDataAry->SetAt(10, strTempAry.GetAt(2));
// -------------------------------------
		rno=1;
// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 11 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}
		// 01 支払先の名称（氏名）
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdName1);
		rno++;
		// 02 支払先の所在地（住所）１
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdAdd1);
		rno++;
		// 03 支払先の所在地（住所）２
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdAdd2);
		rno++;
		// 04 支払年月日
		CString csSpDate = _T("");
		if (m_pUcData->m_SpDate > 0) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &csSpDate);
		}
		pstrCsvDataAry->SetAt( rno, csSpDate);
		rno++;
		// 05 支払金額
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_Val);
		rno++;
		// 06 権利金等の内容１
		// 07 権利金等の内容２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 2);
		pstrCsvDataAry->SetAt( rno, strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt( rno, strTempAry.GetAt(1));
		rno++;
		// 08 摘要１
		// 09 摘要２
		// 10 摘要３
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 3);
		pstrCsvDataAry->SetAt( rno, strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt( rno, strTempAry.GetAt(1));
		rno++;
		pstrCsvDataAry->SetAt( rno, strTempAry.GetAt(2));
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