#include "StdAfx.h"
#include "impUc031Urikakekin.h"

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
CimpUc031Urikakekin::CimpUc031Urikakekin(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc031Urikakekin::~CimpUc031Urikakekin(void)
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
int CimpUc031Urikakekin::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_031;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_031_ITEM_CNT;

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
	m_pUcCmn = (CdbUc000Common*)(new CdbUc031Urikakekin(pDB));

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
	m_RowMax		= ID_ROWKOBETU_031;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_031 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_031;


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
int CimpUc031Urikakekin::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc031Urikakekin*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++

// 登録番号（法人番号）_23/11/21 cor -->
	int	ofset=1;
// 登録番号（法人番号）_23/11/21 cor <--

	// 01 科目
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_031_KAMOKU*2) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_KAMOKU*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_01);
		m_uImpCsvInfo.nLength = MAX_KETA_031_KAMOKU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

// 登録番号（法人番号）_23/11/21 cor -->
	//if(pstrCsvDataAry->GetAt(2).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(3) != TRUE)	{
	//	pstrCsvDataAry->SetAt(2,pstrCsvDataAry->GetAt(3));
	//	pstrCsvDataAry->SetAt(3,_T(""));
	//}
// -------------------------------------
	ofset++;
	if(m_NewVer2 == TRUE)	{
		// 09 登録番号（法人番号）
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
	if(pstrCsvDataAry->GetAt(ofset).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(ofset+1) != TRUE)	{
		pstrCsvDataAry->SetAt(ofset,pstrCsvDataAry->GetAt(ofset+1));
		pstrCsvDataAry->SetAt(ofset+1,_T(""));
	}
// 登録番号（法人番号）_23/11/21 cor <--

	// 02 相手先名称（氏名）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_031_ADNAME1) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_02);
		m_uImpCsvInfo.nLength = MAX_KETA_031_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 相手先名称（氏名）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_031_ADNAME1) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_03);
		m_uImpCsvInfo.nLength = MAX_KETA_031_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 04 相手先所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_031_ADADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_ADADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_04);
		m_uImpCsvInfo.nLength = MAX_KETA_031_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 05 相手先所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_031_ADADD2*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_ADADD2*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_05);
		m_uImpCsvInfo.nLength = MAX_KETA_031_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 06 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_031_K_DATA;
	else					len = ID_VAL_031_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(6), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_06);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
// 登録番号（法人番号）_23/11/21 cor -->
		//if(pstrCsvDataAry->GetAt(6).Left(1) == '-')	{
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

	// 07 摘要１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(7), MAX_KETA_031_TEKI) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_TEKI) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_07);
		m_uImpCsvInfo.nLength = MAX_KETA_031_TEKI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 08 摘要２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(8), MAX_KETA_031_TEKI) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_031_TEKI) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_031_ITEM_08);
		m_uImpCsvInfo.nLength = MAX_KETA_031_TEKI/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
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
//	long			nAdSeq			= 0;				// 取引先シーケンス番号
//	long			nAdOrder		= 0;				// 取引先並び順
//	int				nAdRessGr		= ID_ADDRESSGR_URI;	// 取引先グループ（売掛金）
//
//	// 01 科目
//	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	// 02 相手先名称（氏名）１
//	// 03 相手先名称（氏名）２
//	// 04 相手先所在地（住所）１
//	// 05 相手先所在地（住所）２
//	if (CheckEntryAddress(	pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3),
//							pstrCsvDataAry->GetAt(4), pstrCsvDataAry->GetAt(5),
//							nAdRessGr, &nAdSeq, &nAdOrder) != TRUE) {
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
//		// 01 科目
//		m_pUcData->m_KnSeq		= nKnSeq;
//		m_pUcData->m_KnOrder	= nKnOrder;
//		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
//// midori 152137 add -->
//		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//// midori 152137 add <--
//
//		// 02 相手先名称（氏名）１
//		// 03 相手先名称（氏名）２
//		// 04 相手先所在地（住所）１
//		// 05 相手先所在地（住所）２
//		m_pUcData->m_AdSeq		= nAdSeq;
//		m_pUcData->m_AdOrder	= nAdOrder;
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(2);
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(3);
//		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(4);
//		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(5);
//// midori 152137 add -->
//		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(2));
//// midori 152137 add <--
//
//		// 06 期末現在高
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(6);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 07 摘要１
//		// 08 摘要２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(7), pstrCsvDataAry->GetAt(8));
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
int CimpUc031Urikakekin::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// No.200903 add -->
	int		len=0;
// No.200903 add <--
// 登録番号（法人番号）_23/11/21 add -->
	int				ofset=1;
	int				save=0;
// 登録番号（法人番号）_23/11/21 add <--

	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--
// 登録番号（法人番号）_23/11/21 add -->
	CString			cs=_T("");
// 登録番号（法人番号）_23/11/21 add <--
// midori 190504 add -->
	CUcFunctionCommon	ufc;
// midori 190504 add <--

	// 文字数をオーバーしていればカットする
	// 01 科目
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_031_KAMOKU*2);
	//pstrCsvDataAry->SetAt(1,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_KAMOKU*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;

	if(m_NewVer2 == TRUE)	{
		// 09 登録番号（法人番号）
		ofset++;
	}

// 登録番号（法人番号）_23/11/21 cor <--

	// 02 相手先名称（氏名）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_031_ADNAME1);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(2,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_ADNAME1);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 03 相手先名称（氏名）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_031_ADNAME1);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(3,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_ADNAME1);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 04 相手先所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_031_ADADD1*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(4,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_ADADD1*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 05 相手先所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_031_ADADD2*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(5,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_ADADD2*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 06 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_031_K_DATA;
	else					len = ID_VAL_031_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(6), len, TRUE) != TRUE)	{
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(6,strTemp);
	//}
// -------------------------------------
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 07 摘要１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(7),&strTemp,MAX_KETA_031_TEKI);
	//pstrCsvDataAry->SetAt(7,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_TEKI);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 08 摘要２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(8),&strTemp,MAX_KETA_031_TEKI);
	//pstrCsvDataAry->SetAt(8,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_031_TEKI);
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
	m_pUcData = (CdbUc031Urikakekin*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順
	long			nAdSeq			= 0;				// 取引先シーケンス番号
	long			nAdOrder		= 0;				// 取引先並び順
	int				nAdRessGr		= ID_ADDRESSGR_URI;	// 取引先グループ（売掛金）

	// 01 科目
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
// -------------------------------------
	ofset=1;
	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(ofset), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		return IMP_REC_NG;
	}

	// 02 相手先名称（氏名）１
	// 03 相手先名称（氏名）２
	// 04 相手先所在地（住所）１
	// 05 相手先所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckEntryAddress(	pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3),
	//						pstrCsvDataAry->GetAt(4), pstrCsvDataAry->GetAt(5),
	//						nAdRessGr, &nAdSeq, &nAdOrder) != TRUE) {
	//	return IMP_REC_NG;
	//}
// -------------------------------------
	ofset++;
	cs=_T("");
	if(m_NewVer2 == TRUE)	{
		if(bG_InvNo == TRUE)	{
			cs=pstrCsvDataAry->GetAt(ofset);
// 231222_168474 add -->
			if(CheckImportTno(cs) != TRUE)	{
				cs = _T("");
				pstrCsvDataAry->SetAt(ofset,cs);
			}
// 231222_168474 add <--
		}
		ofset++;
	}
	if (CheckEntryAddress(	pstrCsvDataAry->GetAt(ofset),
							pstrCsvDataAry->GetAt(ofset+1),
							pstrCsvDataAry->GetAt(ofset+2),
							pstrCsvDataAry->GetAt(ofset+3),
							nAdRessGr,&nAdSeq,&nAdOrder,cs) != TRUE) {
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
		//// 01 科目
		//m_pUcData->m_KnSeq		= nKnSeq;
		//m_pUcData->m_KnOrder	= nKnOrder;
		//m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
		//m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));

		//// 02 相手先名称（氏名）１
		//// 03 相手先名称（氏名）２
		//// 04 相手先所在地（住所）１
		//// 05 相手先所在地（住所）２
		//m_pUcData->m_AdSeq		= nAdSeq;
		//m_pUcData->m_AdOrder	= nAdOrder;
		//m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(2);
		//m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(3);
		//m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(4);
		//m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(5);
		//m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(2));

		//// 06 期末現在高
		//m_pUcData->m_Val		= pstrCsvDataAry->GetAt(6);
		//RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		//// 07 摘要１
		//// 08 摘要２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(7), pstrCsvDataAry->GetAt(8));
		//m_pUcData->m_Teki		= strTemp;
// -------------------------------------
		ofset=1;
		// 01 科目
		m_pUcData->m_KnSeq		= nKnSeq;
		m_pUcData->m_KnOrder	= nKnOrder;
		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(ofset);
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(ofset));
		ofset++;
		// 09 登録番号（法人番号）
		if(m_NewVer2 == TRUE)	{
			if(bG_InvNo == TRUE)	{
				m_pUcData->m_InvNo = pstrCsvDataAry->GetAt(ofset);
			}
			ofset++;
		}
		save=ofset;
		// 02 相手先名称（氏名）１
		// 03 相手先名称（氏名）２
		// 04 相手先所在地（住所）１
		// 05 相手先所在地（住所）２
		m_pUcData->m_AdSeq		= nAdSeq;
		m_pUcData->m_AdOrder	= nAdOrder;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(save));
		// 06 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
		ofset++;
		// 07 摘要１
		// 08 摘要２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
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
int CimpUc031Urikakekin::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc031Urikakekin*)m_pUcCmn;


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
		//// 01 科目
		//pstrCsvDataAry->SetAt(1, m_pUcData->m_KnName);

		//// 02 相手先名称（氏名）１
		//pstrCsvDataAry->SetAt(2, m_pUcData->m_AdName1);

		//// 03 相手先名称（氏名）２
		//pstrCsvDataAry->SetAt(3, m_pUcData->m_AdName2);

		//// 04 相手先所在地（住所）１
		//pstrCsvDataAry->SetAt(4, m_pUcData->m_AdAdd1);

		//// 05 相手先所在地（住所）２
		//pstrCsvDataAry->SetAt(5, m_pUcData->m_AdAdd2);

		//// 06 期末現在高
		//pstrCsvDataAry->SetAt(6, m_pUcData->m_Val);

		//// 07 摘要１
		//// 08 摘要２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
		//pstrCsvDataAry->SetAt(7, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(8, strTempAry.GetAt(1));
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
			// 09 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}

		// 02 相手先名称（氏名）１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName1);
		rno++;
		// 03 相手先名称（氏名）２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName2);
		rno++;
		// 04 相手先所在地（住所）１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdAdd1);
		rno++;
		// 05 相手先所在地（住所）２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdAdd2);
		rno++;
		// 06 期末現在高
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Val);
		rno++;
		// 07 摘要１
		// 08 摘要２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(0));
		rno++;
		pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(1));
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

//********************************************************************************
//	一括集計金額行のデータ設定
//		IN		int					呼び出し元（0:ソート，1:一括金額参照，2:特殊行挿入）
//				CfrmUc000Common*	テーブル情報
//				CString				名称
//				CALCKEI_INFO		一括集計金額情報
//				SORTKAMOKU_INFO		科目情報
//		RET		なし
//********************************************************************************
void	CimpUc031Urikakekin::virTableEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	//CdbUc031Urikakekin*	mfcRec = (CdbUc031Urikakekin*)rsData;

	switch (nType) {
	case	0:
		// ソートの一括集計処理 ･･･ 科目の無い帳表はいらない
		m_pUcData->m_AdName1 = strName;				// 名称：名称２
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定

		m_pUcData->m_KnSeq = uKamoku.intKnSeq;			// 科目情報
		m_pUcData->m_KnOrder = uKamoku.intKnOrder;		// 科目情報
		m_pUcData->m_KnName = uKamoku.strKnName;		// 科目情報
		break;
	case	1:
		// 一括金額参照の戻す処理
		m_pUcData->m_AdName1 = strName;				// 名称：名称２
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case	2:
		// 特殊行挿入の一括金額選択時
		m_pUcData->m_AdName1 = strName;				// 名称：名称２
		break;
	}
}