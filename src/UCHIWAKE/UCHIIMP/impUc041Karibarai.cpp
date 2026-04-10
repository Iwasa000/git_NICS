#include "StdAfx.h"
#include "impUc041Karibarai.h"

// 210113_5301_1 add -->
extern BOOL	bG_Kanso;
// 210113_5301_1 add <--
// 登録番号（法人番号）_23/11/21 add -->
extern BOOL	bG_InvNo;
// 登録番号（法人番号）_23/11/21 add <--
// 24/02/20_ScoreLink add -->
extern BOOL	bG_Sl;
// 24/02/20_ScoreLink add <--

// コンストラクタ
CimpUc041Karibarai::CimpUc041Karibarai(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc041Karibarai::~CimpUc041Karibarai(void)
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
int CimpUc041Karibarai::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_041;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_041_ITEM_CNT;

// 登録番号（法人番号）_23/11/21 add -->
	if(m_IsExport == TRUE && bG_InvNo == TRUE)	{
		// エクスポート時、新様式なら項目数を増やしておく
		// (インポートは新旧両方のレイアウトに対応するため、ここでは決定できない)
		m_nItemCount = m_nItemCount + 1;
	}
// 登録番号（法人番号）_23/11/21 add <--

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc041Karibarai(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );

	// inf_form_typeテーブルの初期化
	m_pUcInfFormType->Init( ID_FORMNO_04 );


	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_041;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_041 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_041;

	// 出力設定が通常の場合
	if( m_pUcInfFormType->m_FormType == 0 ){
		m_RowMax		= ID_ROWNORMAL_041;
		m_RowMaxData	= ID_ROWNORMAL_041 - 1;
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
int CimpUc041Karibarai::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc041Karibarai*)m_pUcCmn;

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
	//if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_041_KAMOKU*2) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_KAMOKU*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_01);
		m_uImpCsvInfo.nLength = MAX_KETA_041_KAMOKU;
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
		// 10 登録番号（法人番号）
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
	//if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_041_ADNAME1) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_02);
		m_uImpCsvInfo.nLength = MAX_KETA_041_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 相手先名称（氏名）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_041_ADNAME1) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_03);
		m_uImpCsvInfo.nLength = MAX_KETA_041_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 04 相手先所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_041_ADADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_ADADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_04);
		m_uImpCsvInfo.nLength = MAX_KETA_041_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 05 相手先所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_041_ADADD2*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_ADADD2*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_05);
		m_uImpCsvInfo.nLength = MAX_KETA_041_ADADD2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 06 法人・代表者との関係
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_041_KANKEI*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_KANKEI*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_06);
		m_uImpCsvInfo.nLength = MAX_KETA_041_KANKEI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 07 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_041_K_DATA;
	else					len = ID_VAL_041_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_07);
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

	// 08 取引の内容１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(8), MAX_KETA_041_NAIYOU) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_NAIYOU) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_08_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_08);
		m_uImpCsvInfo.nLength = MAX_KETA_041_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 09 取引の内容２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(9), MAX_KETA_041_NAIYOU) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_041_NAIYOU) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_09_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_041_ITEM_09);
		m_uImpCsvInfo.nLength = MAX_KETA_041_NAIYOU/2;
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
//	long			nKnSeq			= 0;				// 科目シーケンス番号
//	long			nKnOrder		= 0;				// 科目並び順
//	long			nAdSeq			= 0;				// 取引先シーケンス番号
//	long			nAdOrder		= 0;				// 取引先並び順
//	int				nAdRessGr		= ID_ADDRESSGR_KAI;	// 取引先グループ（買掛金）
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
//		m_pUcData->m_AdSeq		= nAdSeq;
//		m_pUcData->m_AdOrder	= nAdOrder;
//		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(2);
//		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(3);
//// midori 152137 add -->
//		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(2));
//// midori 152137 add <--
//
//		// 04 相手先所在地（住所）１
//		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(4);
//
//		// 05 相手先所在地（住所）２
//		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(5);
//
//		// 06 法人・代表者との関係
//		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(6);
//		
//		// 07 期末現在高
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 08 取引の内容１
//		// 09 取引の内容２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
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
int CimpUc041Karibarai::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
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
// 登録番号（法人番号）_23/11/21 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--
// midori 190504 add -->
	CUcFunctionCommon	ufc;
// midori 190504 add <--

	// 文字数をオーバーしていればカットする
	// 01 科目
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_041_KAMOKU*2);
	//pstrCsvDataAry->SetAt(1,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_KAMOKU*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;

	// 10 登録番号（法人番号）
	if(m_NewVer2 == TRUE)	{
		ofset++;
	}
// 登録番号（法人番号）_23/11/21 cor <--

	// 02 相手先名称（氏名）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_041_ADNAME1);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(2,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_ADNAME1);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 03 相手先名称（氏名）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_041_ADNAME1);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(3,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_ADNAME1);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 04 相手先所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_041_ADADD1*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(4,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_ADADD1*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 05 相手先所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_041_ADADD2*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(5,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_ADADD2*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 06 法人・代表者との関係
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_041_KANKEI*2);
	//pstrCsvDataAry->SetAt(6,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_KANKEI*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 07 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_041_K_DATA;
	else					len = ID_VAL_041_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(7,strTemp);
	//}
// -------------------------------------
	if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 08 取引の内容１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(8),&strTemp,MAX_KETA_041_NAIYOU);
	//pstrCsvDataAry->SetAt(8,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_NAIYOU);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 09 取引の内容２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(9),&strTemp,MAX_KETA_041_NAIYOU);
	//pstrCsvDataAry->SetAt(9,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_041_NAIYOU);
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
	m_pUcData = (CdbUc041Karibarai*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順
	long			nAdSeq			= 0;				// 取引先シーケンス番号
	long			nAdOrder		= 0;				// 取引先並び順
	int				nAdRessGr		= ID_ADDRESSGR_KAI;	// 取引先グループ（買掛金）

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
	CString	cs=_T("");
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
		//m_pUcData->m_AdSeq		= nAdSeq;
		//m_pUcData->m_AdOrder	= nAdOrder;
		//m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(2);
		//m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(3);
		//m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(2));

		//// 04 相手先所在地（住所）１
		//m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(4);

		//// 05 相手先所在地（住所）２
		//m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(5);

		//// 06 法人・代表者との関係
		//m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(6);
		//
		//// 07 期末現在高
		//m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
		//RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		//// 08 取引の内容１
		//// 09 取引の内容２
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
		//m_pUcData->m_Naiyou		= strTemp;
// -------------------------------------
		ofset=1;
		// 01 科目
		m_pUcData->m_KnSeq		= nKnSeq;
		m_pUcData->m_KnOrder	= nKnOrder;
		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(ofset);
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(ofset));
		ofset++;
		// 10 登録番号（法人番号）
		if(m_NewVer2 == TRUE)	{
			if(bG_InvNo == TRUE)	{
				m_pUcData->m_InvNo = pstrCsvDataAry->GetAt(ofset);
			}
			ofset++;
		}
		save=ofset;
		// 02 相手先名称（氏名）１
		// 03 相手先名称（氏名）２
		m_pUcData->m_AdSeq		= nAdSeq;
		m_pUcData->m_AdOrder	= nAdOrder;
		m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(save));
		// 04 相手先所在地（住所）１
		m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		// 05 相手先所在地（住所）２
		m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		// 06 法人・代表者との関係
		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		// 07 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(ofset);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
		ofset++;
		// 08 取引の内容１
		// 09 取引の内容２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		m_pUcData->m_Naiyou		= strTemp;
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
int CimpUc041Karibarai::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc041Karibarai*)m_pUcCmn;


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CStringArray	strTempAry;						// 文字列配列テンポラリ
// 登録番号（法人番号）_23/11/21 cor -->
	int				rno=0;
// 登録番号（法人番号）_23/11/21 cor <--

	// バッファサイズを指定
// 登録番号（法人番号）_23/11/21 cor -->
	//pstrCsvDataAry->SetSize(CSV_041_ITEM_CNT);
// -------------------------------------
	// 何故か④-1のみここでサイズをセットし直している
// 24/02/20_ScoreLink cor -->
	//if(bG_InvNo == TRUE)	{
// --------------------------
	if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
		pstrCsvDataAry->SetSize(CSV_041_ITEM_CNT+1);
	}
	else	{
		pstrCsvDataAry->SetSize(CSV_041_ITEM_CNT);
	}
// 登録番号（法人番号）_23/11/21 cor <--

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// [明細]の場合のみ各項目のデータを取得
	// [小計]、[中計]は識別子のみ
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
// 登録番号（法人番号）_23/11/21 cor -->
		//// 01 科目
		//pstrCsvDataAry->SetAt( 1, m_pUcData->m_KnName);

		//// 02 相手先名称（氏名）１
		//pstrCsvDataAry->SetAt( 2, m_pUcData->m_AdName1);

		//// 03 相手先名称（氏名）２
		//pstrCsvDataAry->SetAt( 3, m_pUcData->m_AdName2);

		//// 04 相手先所在地（住所）１
		//pstrCsvDataAry->SetAt( 4, m_pUcData->m_AdAdd1);

		//// 05 相手先所在地（住所）２
		//pstrCsvDataAry->SetAt( 5, m_pUcData->m_AdAdd2);

		//// 06 法人・代表者との関係
		//pstrCsvDataAry->SetAt( 6, m_pUcData->m_Kankei);

		//// 07 期末現在高
		//pstrCsvDataAry->SetAt( 7, m_pUcData->m_Val);

		//// 08 取引の内容１
		//// 09 取引の内容２
		//strTempAry.RemoveAll();
		//m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 2);
		//pstrCsvDataAry->SetAt(8, strTempAry.GetAt(0));
		//pstrCsvDataAry->SetAt(9, strTempAry.GetAt(1));
// -------------------------------------
		rno=1;

		// 01 科目
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_KnName);
		rno++;

// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 10 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}

		// 02 相手先名称（氏名）１
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdName1);
		rno++;

		// 03 相手先名称（氏名）２
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdName2);
		rno++;

		// 04 相手先所在地（住所）１
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdAdd1);
		rno++;

		// 05 相手先所在地（住所）２
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_AdAdd2);
		rno++;

		// 06 法人・代表者との関係
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_Kankei);
		rno++;

		// 07 期末現在高
		pstrCsvDataAry->SetAt( rno, m_pUcData->m_Val);
		rno++;

		// 08 取引の内容１
		// 09 取引の内容２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 2);
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
void	CimpUc041Karibarai::virTableEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	//CdbUc041Karibarai*	mfcRec = (CdbUc041Karibarai*)rsData;

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