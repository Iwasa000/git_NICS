#include "StdAfx.h"
#include "impUc042Kasituke.h"

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

// コンストラクタ
CimpUc042Kasituke::CimpUc042Kasituke(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc042Kasituke::~CimpUc042Kasituke(void)
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
int CimpUc042Kasituke::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_042;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_042_ITEM_CNT;

// 登録番号（法人番号）_23/11/21 cor -->
//// 210113_5301_1 add -->
//	if(m_IsExport == TRUE && bG_Kanso == TRUE)	{
//		// エクスポート時、新様式なら項目数を減らしておく
//		// (インポートは新旧両方のレイアウトに対応するため、ここでは決定できない)
//		m_nItemCount = m_nItemCount - 2;
//	}
//// 210113_5301_1 add <--
// -------------------------------------
	if(m_IsExport == TRUE)	{
		// エクスポート時、新様式なら項目数を減らしておく
		// (インポートは新旧両方のレイアウトに対応するため、ここでは決定できない)
		// 旧様式　						13列
		// 簡素化様式　					11列
		// 登録番号（法人番号）追加様式	12列
// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_Sl == TRUE)	{
			m_nItemCount = m_nItemCount + 1;
		}
		else if(bG_InvNo == TRUE)	{
// 24/02/20_ScoreLink cor <--
			m_nItemCount = m_nItemCount - 1;
		}
		else if(bG_Kanso == TRUE)	{
			m_nItemCount = m_nItemCount - 2;
		}
	}
// 登録番号（法人番号）_23/11/21 cor <--

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc042Kasituke(pDB));

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
	m_RowMax		= ID_ROWKOBETU_042;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_042 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_042;

	// 出力設定が通常の場合
	if( m_pUcInfFormType->m_FormType == 0 ){
		m_RowMax		= ID_ROWNORMAL_042;
		m_RowMaxData	= ID_ROWNORMAL_042 - 1;
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
int CimpUc042Kasituke::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	int			len=0;

	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc);
	// エラーなら終了
	if (nRet != ID_FGFUNC_NULL) {
		return nRet;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc042Kasituke*)m_pUcCmn;

	nRet = IMP_REC_OK;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++

// 登録番号（法人番号）_23/11/21 cor -->
	//if(pstrCsvDataAry->GetAt(1).IsEmpty() == TRUE && pstrCsvDataAry->GetAt(2) != TRUE)	{
	//	pstrCsvDataAry->SetAt(1,pstrCsvDataAry->GetAt(2));
	//	pstrCsvDataAry->SetAt(2,_T(""));
	//}
// -------------------------------------
	int	ofset=1;

	if(m_NewVer2 == TRUE)	{
		// 13 登録番号（法人番号）
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
// 登録番号（法人番号）_23/11/21 add <--

	// 01 貸付先１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_042_ADNAME1) != TRUE) {
// -------------------------------------
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_01_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_01);
		m_uImpCsvInfo.nLength = MAX_KETA_042_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 02 貸付先２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_042_ADNAME1) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_ADNAME1) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_02_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_02);
		m_uImpCsvInfo.nLength = MAX_KETA_042_ADNAME1/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 03 所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_042_ADADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_ADADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_03_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_03);
		m_uImpCsvInfo.nLength = MAX_KETA_042_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 04 所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_042_ADADD1*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_ADADD1*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		if(bG_Kanso == TRUE)	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_04_NEW);
		else					strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_04);
		m_uImpCsvInfo.nLength = MAX_KETA_042_ADADD1;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 05 法人・代表者との関係
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_042_KANKEI*2) != TRUE) {
// -------------------------------------
	ofset++;
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_KANKEI*2) != TRUE) {
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_05);
		m_uImpCsvInfo.nLength = MAX_KETA_042_KANKEI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 06 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_042_K_DATA;
	else					len = ID_VAL_042_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if(CheckImportNum2(pstrCsvDataAry->GetAt(6), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_06);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(6).Left(1) == '-')	{
			// 金額がマイナス
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 07 期中の受取利息額
	if(bG_Kanso == TRUE)	len = ID_RISOKU_042_K_DATA;
	else					len = ID_RISOKU_042_DATA;
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE)	{
// -------------------------------------
	ofset++;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(ofset), len, TRUE) != TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_07);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(7).Left(1) == '-')	{
			// 金額がマイナス
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	// 08 利率
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum(pstrCsvDataAry->GetAt(8), 2, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
// -------------------------------------
	ofset++;
	if (CheckImportNum(pstrCsvDataAry->GetAt(ofset), 2, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
// 登録番号（法人番号）_23/11/21 cor <--
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_08);
		m_uImpCsvInfo.nLength	= 2;	// 整数最大桁数
		m_uImpCsvInfo.nLength2	= 3;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

// 登録番号（法人番号）_23/11/21 cor -->
	//int	ofset=9;
// -------------------------------------
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--
	if(m_NewVer != TRUE) {
		// 09 貸付理由１
		if(bG_Kanso != TRUE)	{
			if(CheckImportStr(pstrCsvDataAry->GetAt(ofset),MAX_KETA_042_RIYUU) != TRUE) {
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_042_ITEM_09);
				m_uImpCsvInfo.nLength = MAX_KETA_042_RIYUU/2;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
			}
		}
		ofset++;
		// 10 貸付理由２
		if(bG_Kanso != TRUE)	{
			if(CheckImportStr(pstrCsvDataAry->GetAt(ofset),MAX_KETA_042_RIYUU) != TRUE) {
				strcpy_s(m_uImpCsvInfo.szColumn,sizeof(m_uImpCsvInfo.szColumn),CSV_042_ITEM_10);
				m_uImpCsvInfo.nLength = MAX_KETA_042_RIYUU/2;
				m_uImpCsvInfo.nKind = 0;	// 0:文字
				m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
				CsvInfoClr(&m_uImpCsvInfo);
				nRet = IMP_REC_NG;
			}
		}
		ofset++;
	}

	// 11 担保の内容（物件の種類、数量、所在地等）１
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_NAIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_11);
		m_uImpCsvInfo.nLength = MAX_KETA_042_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
	ofset++;

	// 12 担保の内容（物件の種類、数量、所在地等）２
	if (CheckImportStr(pstrCsvDataAry->GetAt(ofset), MAX_KETA_042_NAIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_042_ITEM_12);
		m_uImpCsvInfo.nLength = MAX_KETA_042_NAIYOU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}

	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}

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
int CimpUc042Kasituke::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
	int				len=0;
// 登録番号（法人番号）_23/11/21 add -->
	int				ofset=1;
	int				save=0;
// 登録番号（法人番号）_23/11/21 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
	CString			cs=_T("");
	CString			cs1=_T("");
	CString			cs2=_T("");
	CString			cs3=_T("");
	CString			strCsvId = _T("");
	CUcFunctionCommon	ufc;

// 登録番号（法人番号）_23/11/21 add -->
	if(m_NewVer2 == TRUE)	{
		// 13 登録番号（法人番号）
		ofset++;
	}
// 登録番号（法人番号）_23/11/21 add <--

	// 文字数をオーバーしていればカットする
	// 01 貸付先１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_042_ADNAME1);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(1,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_ADNAME1);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 02 貸付先２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_042_ADNAME1);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//pstrCsvDataAry->SetAt(2,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_ADNAME1);
	strTemp = ufc.DeleteRightSpace(strTemp);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 03 所在地（住所）１
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_042_ADADD1*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//if(bG_Kanso == TRUE)	{
	//	// 全角１０文字を超えていれば改行マークを付けて取り込む
	//	if(strTemp.GetLength() > 20)	{
	//		cs2 = GetSpritString(strTemp,&cs1,20);
	//		cs = cs2;
	//		// 前半に半角文字が奇数バイト数分、入力されている場合
	//		// 後半に半角文字が１バイト余分に付いているのでカットする
	//		cs3 = GetSpritString(cs,&cs2,20);
	//		strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
	//	}
	//}
	//pstrCsvDataAry->SetAt(3,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_ADADD1*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
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
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 04 所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(4),&strTemp,MAX_KETA_042_ADADD1*2);
	//strTemp = ufc.DeleteRightSpace(strTemp);
	//if(bG_Kanso == TRUE)	{
	//	// 全角１０文字を超えていれば改行マークを付けて取り込む
	//	if(strTemp.GetLength() > 20)	{
	//		cs2 = GetSpritString(strTemp,&cs1,20);
	//		cs = cs2;
	//		// 前半に半角文字が奇数バイト数分、入力されている場合
	//		// 後半に半角文字が１バイト余分に付いているのでカットする
	//		cs3 = GetSpritString(cs,&cs2,20);
	//		strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
	//	}
	//}
	//pstrCsvDataAry->SetAt(4,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_ADADD1*2);
	strTemp = ufc.DeleteRightSpace(strTemp);
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
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 05 法人・代表者との関係
// 登録番号（法人番号）_23/11/21 cor -->
	//GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_042_KANKEI*2);
	//pstrCsvDataAry->SetAt(5,strTemp);
// -------------------------------------
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_KANKEI*2);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

	// 06 期末現在高
	if(bG_Kanso == TRUE)	len = ID_VAL_042_K_DATA;
	else					len = ID_VAL_042_DATA;
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

	// 07 期中の受取利息額
	if(bG_Kanso == TRUE)	len = ID_RISOKU_042_K_DATA;
	else					len = ID_RISOKU_042_DATA;
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

	// 08 利率
// 登録番号（法人番号）_23/11/21 cor -->
	//if (CheckImportNum(pstrCsvDataAry->GetAt(8), 2, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(8,strTemp);
	//}
// -------------------------------------
	if (CheckImportNum(pstrCsvDataAry->GetAt(ofset), 2, 3, FALSE) != TRUE) {					// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(ofset,strTemp);
	}
	ofset++;
// 登録番号（法人番号）_23/11/21 cor <--

// 登録番号（法人番号）_23/11/21 del -->
	//int	ofset=9;
// 登録番号（法人番号）_23/11/21 del <--

	if(m_NewVer != TRUE)	{
		// 09 貸付理由１
		GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_RIYUU);
		pstrCsvDataAry->SetAt(ofset,strTemp);
		ofset++;
		// 10 貸付理由２
		GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_RIYUU);
		pstrCsvDataAry->SetAt(ofset,strTemp);
		ofset++;
	}
	// 11 担保の内容（物件の種類、数量、所在地等）１
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_NAIYOU);
	pstrCsvDataAry->SetAt(ofset,strTemp);
	ofset++;
	// 12 担保の内容（物件の種類、数量、所在地等）２
	GetSpritString(pstrCsvDataAry->GetAt(ofset),&strTemp,MAX_KETA_042_NAIYOU);
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

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc042Kasituke*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nAdSeq			= 0;				// 取引先シーケンス番号
	long			nAdOrder		= 0;				// 取引先並び順
	int				nAdRessGr		= ID_ADDRESSGR_URI;	// 取引先グループ（売掛金）
	// 01 振出人１
	// 02 振出人２
	// 03 所在地（住所）１
	// 04 所在地（住所）２
// 登録番号（法人番号）_23/11/21 cor -->
	//cs1 = pstrCsvDataAry->GetAt(3);
	//cs1.Replace(PRT_FORMAT_CHCODE,_T(""));
	//cs2 = pstrCsvDataAry->GetAt(4);
	//cs2.Replace(PRT_FORMAT_CHCODE,_T(""));
	//if (CheckEntryAddress(pstrCsvDataAry->GetAt(1),pstrCsvDataAry->GetAt(2),cs1,cs2,nAdRessGr,&nAdSeq,&nAdOrder) != TRUE)	{
	//	return IMP_REC_NG;
	//}
// -------------------------------------
	cs3=_T("");
	ofset=1;
	if(m_NewVer2 == TRUE)	{
		if(bG_InvNo == TRUE)	{
			cs3 = pstrCsvDataAry->GetAt(ofset);
// 231222_168474 add -->
			if(CheckImportTno(cs3) != TRUE)	{
				cs3 = _T("");
				pstrCsvDataAry->SetAt(ofset,cs3);
			}
// 231222_168474 add <--
		}
		ofset++;
	}
	cs1 = pstrCsvDataAry->GetAt(ofset+2);
	cs1.Replace(PRT_FORMAT_CHCODE,_T(""));
	cs2 = pstrCsvDataAry->GetAt(ofset+3);
	cs2.Replace(PRT_FORMAT_CHCODE,_T(""));
	if (CheckEntryAddress(pstrCsvDataAry->GetAt(ofset),pstrCsvDataAry->GetAt(ofset+1),cs1,cs2,nAdRessGr,&nAdSeq,&nAdOrder,cs3) != TRUE)	{
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
		//// 01 振出人１
		//// 02 振出人２
		//// 03 所在地（住所）１
		//// 04 所在地（住所）２
		//m_pUcData->m_AdSeq		= nAdSeq;
		//m_pUcData->m_AdOrder	= nAdOrder;
		//m_pUcData->m_AdName1	= pstrCsvDataAry->GetAt(1);
		//m_pUcData->m_AdName2	= pstrCsvDataAry->GetAt(2);
		//m_pUcData->m_AdAdd1		= pstrCsvDataAry->GetAt(3);
		//m_pUcData->m_AdAdd2		= pstrCsvDataAry->GetAt(4);
		//m_pUcData->m_AdKana		= fc.Convert(pstrCsvDataAry->GetAt(1));

		//// 05 法人・代表者との関係
		//m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(5);

		//// 06 期末現在高
		//m_pUcData->m_Val		= pstrCsvDataAry->GetAt(6);
		//RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		//// 07 期中の受取利息額
		//m_pUcData->m_Risoku		= pstrCsvDataAry->GetAt(7);
		//RemoveCommaImportNum( m_pUcData->m_Risoku );			// カンマを除く　2011.07.15 h.ukawa

		//// 08 利率
		//strTemp = _T("");
		//strTemp = CalcStrMul(pstrCsvDataAry->GetAt(8), 1000, 0);	// ×1000
		//m_pUcData->m_Rate		= strTemp;

		//ofset=9;
		//if(m_NewVer != TRUE)	{
		//	// 09 貸付理由１
		//	// 10 貸付理由２
		//	strTemp = _T("");
		//	strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		//	m_pUcData->m_Riyuu		= strTemp;
		//	ofset=11;
		//}
		//// 11 担保の内容（物件の種類、数量、所在地等）１
		//// 12 担保の内容（物件の種類、数量、所在地等）２ 
		//strTemp = _T("");
		//strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
		//m_pUcData->m_Naiyou		= strTemp;
// -------------------------------------
		ofset=1;
		if(m_NewVer2 == TRUE)	{
			// 13 登録番号
			if(bG_InvNo == TRUE)	{
				m_pUcData->m_InvNo = pstrCsvDataAry->GetAt(ofset);
			}
			ofset++;
		}
		save=ofset;
		// 01 振出人１
		// 02 振出人２
		// 03 所在地（住所）１
		// 04 所在地（住所）２
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

		// 05 法人・代表者との関係
		m_pUcData->m_Kankei		= pstrCsvDataAry->GetAt(ofset);
		ofset++;

		// 06 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		// 07 期中の受取利息額
		m_pUcData->m_Risoku		= pstrCsvDataAry->GetAt(ofset);
		ofset++;
		RemoveCommaImportNum( m_pUcData->m_Risoku );			// カンマを除く　2011.07.15 h.ukawa

		// 08 利率
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(ofset), 1000, 0);	// ×1000
		ofset++;
		m_pUcData->m_Rate		= strTemp;

		if(m_NewVer != TRUE)	{
			// 09 貸付理由１
			// 10 貸付理由２
			strTemp = _T("");
			strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(ofset), pstrCsvDataAry->GetAt(ofset+1));
			m_pUcData->m_Riyuu		= strTemp;
			ofset=ofset+2;
		}
		// 11 担保の内容（物件の種類、数量、所在地等）１
		// 12 担保の内容（物件の種類、数量、所在地等）２ 
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
int CimpUc042Kasituke::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	CString			strCsvId		= _T("");		// CSVの識別子
// midori 190301 0415 add -->
	CString			cs=_T("");
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
	m_pUcData = (CdbUc042Kasituke*)m_pUcCmn;


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
//		// 01 貸付先１
//		pstrCsvDataAry->SetAt(1, m_pUcData->m_AdName1);
//
//		// 02 貸付先２
//		pstrCsvDataAry->SetAt(2, m_pUcData->m_AdName2);
//
//// midori 190301 0415 del -->
//		//// 03 所在地（住所）１
//		//pstrCsvDataAry->SetAt(3, m_pUcData->m_AdAdd1);
//
//		//// 04 所在地（住所）２
//		//pstrCsvDataAry->SetAt(4, m_pUcData->m_AdAdd2);
//
//		//// 05 法人・代表者との関係
//		//pstrCsvDataAry->SetAt(5, m_pUcData->m_Kankei);
//// midori 190301 0415 del <--
//// midori 190301 0415 add -->
//		// 03 所在地（住所）１
//		cs = m_pUcData->m_AdAdd1;
//		if(bG_Kanso == TRUE)	{
//			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
//		}
//		pstrCsvDataAry->SetAt(3,cs);
//
//		// 04 所在地（住所）２
//		cs = m_pUcData->m_AdAdd2;
//		if(bG_Kanso == TRUE)	{
//			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
//		}
//		pstrCsvDataAry->SetAt(4,cs);
//
//		// 05 法人・代表者との関係
//		cs = m_pUcData->m_Kankei;
//// midori 154640 del -->
//		//if(bG_Kanso == TRUE)	{
//		//	cs.Replace(PRT_FORMAT_CHCODE,_T(""));
//		//}
//// midori 154640 del <--
//		pstrCsvDataAry->SetAt(5,cs);
//// midori 190301 0415 add <--
//
//		// 06 期末現在高
//		pstrCsvDataAry->SetAt(6, m_pUcData->m_Val);
//
//		// 07 期中の受取利息額
//		pstrCsvDataAry->SetAt(7, m_pUcData->m_Risoku);
//
//		// 08 利率
//		strTemp = _T("");
//		strTemp = CalcStrMul(m_pUcData->m_Rate, 0.001, 3);	// ÷1000
//		pstrCsvDataAry->SetAt(8, strTemp);
//
//// 210113_5301_1 del -->
//		//// 09 貸付理由１
//		//// 10 貸付理由２
//		//strTempAry.RemoveAll();
//		//if(bG_Kanso == TRUE) {
//		//	strTempAry.SetSize(2);
//		//	strTempAry.SetAt(0,_T(""));
//		//	strTempAry.SetAt(1,_T(""));
//		//}
//		//else	{
//		//	m_clsFunc.StrDivision(m_pUcData->m_Riyuu,&strTempAry,2);
//		//}
//		//pstrCsvDataAry->SetAt(9, strTempAry.GetAt(0));
//		//pstrCsvDataAry->SetAt(10, strTempAry.GetAt(1));
//
//		//// 11 担保の内容（物件の種類、数量、所在地等）１
//		//// 12 担保の内容（物件の種類、数量、所在地等）２
//		//strTempAry.RemoveAll();
//		//m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 2);
//		//pstrCsvDataAry->SetAt(11, strTempAry.GetAt(0));
//		//pstrCsvDataAry->SetAt(12, strTempAry.GetAt(1));
//// 210113_5301_1 del <--
//// 210113_5301_1 add -->
//		int	ofset=9;
//		if(bG_Kanso != TRUE)	{
//			// 09 貸付理由１
//			// 10 貸付理由２
//			strTempAry.RemoveAll();
//			m_clsFunc.StrDivision(m_pUcData->m_Riyuu,&strTempAry,2);
//			pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(0));
//			ofset++;
//			pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(1));
//			ofset++;
//		}
//		// 11 担保の内容（物件の種類、数量、所在地等）１
//		// 12 担保の内容（物件の種類、数量、所在地等）２
//		strTempAry.RemoveAll();
//		m_clsFunc.StrDivision(m_pUcData->m_Naiyou, &strTempAry, 2);
//		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(0));
//		ofset++;
//		pstrCsvDataAry->SetAt(ofset, strTempAry.GetAt(1));
//// 210113_5301_1 add <--
// -------------------------------------
		rno=1;

// 24/02/20_ScoreLink cor -->
		//if(bG_InvNo == TRUE)	{
// --------------------------
		if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 13 登録番号（法人番号）
			pstrCsvDataAry->SetAt(rno, m_pUcData->m_InvNo.TrimLeft());
			rno++;
		}

		// 01 貸付先１
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName1);
		rno++;
		// 02 貸付先２
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_AdName2);
		rno++;
		// 03 所在地（住所）１
		cs = m_pUcData->m_AdAdd1;
		if(bG_Kanso == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(rno,cs);
		rno++;
		// 04 所在地（住所）２
		cs = m_pUcData->m_AdAdd2;
		if(bG_Kanso == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(rno,cs);
		rno++;
		// 05 法人・代表者との関係
		cs = m_pUcData->m_Kankei;
		pstrCsvDataAry->SetAt(rno,cs);
		rno++;
		// 06 期末現在高
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Val);
		rno++;
		// 07 期中の受取利息額
		pstrCsvDataAry->SetAt(rno, m_pUcData->m_Risoku);
		rno++;
		// 08 利率
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Rate, 0.001, 3);	// ÷1000
		pstrCsvDataAry->SetAt(rno, strTemp);
		rno++;
		//int	ofset=9;
// 24/02/20_ScoreLink cor -->
		//if(bG_Kanso != TRUE)	{
// --------------------------
		if(bG_Kanso != TRUE || bG_Sl == TRUE)	{
// 24/02/20_ScoreLink cor <--
			// 09 貸付理由１
			// 10 貸付理由２
			strTempAry.RemoveAll();
			m_clsFunc.StrDivision(m_pUcData->m_Riyuu,&strTempAry,2);
			pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(0));
			rno++;
			pstrCsvDataAry->SetAt(rno, strTempAry.GetAt(1));
			rno++;
		}
		// 11 担保の内容（物件の種類、数量、所在地等）１
		// 12 担保の内容（物件の種類、数量、所在地等）２
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
void	CimpUc042Kasituke::virTableEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	//CdbUc042Kasituke*	rs = (CdbUc042Kasituke*)rsData;

	switch (nType) {
	case 0:
		// ソートの一括集計処理
		m_pUcData->m_AdAdd2 = strName;					// 名称：金融機関名
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		m_pUcData->m_Risoku = uCalcKei.strKei[1];		// 利息："1:Risoku"固定
		break;
	case 1:
		// 一括金額参照の戻す処理
		m_pUcData->m_AdAdd2 = strName;					// 名称：金融機関名
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		m_pUcData->m_Risoku = uCalcKei.strKei[1];		// 利息："1:Risoku"固定
		break;
	case 2:
		// 特殊行挿入の一括金額選択時
		m_pUcData->m_AdAdd2 = strName;					// 名称：金融機関名
		break;
	}
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
//BOOL CimpUc042Kasituke::virFormatedIsNewVer(CStringArray* pstrCsvDataAry)
//{
//	BOOL	nRet=TRUE;
//	CString strItem=_T("");
//	int		ii=0;
//
//	for(ii=1/*[項目]読み飛ばし*/; ii<pstrCsvDataAry->GetCount(); ii++)	{
//		strItem.Empty();
//		switch(ii)	{
//			case	1:	strItem = CSV_042_ITEM_01_NEW;	break;
//			case	2:	strItem = CSV_042_ITEM_02_NEW;	break;
//			case	3:	strItem = CSV_042_ITEM_03_NEW;	break;
//			case	4:	strItem = CSV_042_ITEM_04_NEW;	break;
//			case	5:	strItem = CSV_042_ITEM_05;		break;
//			case	6:	strItem = CSV_042_ITEM_06;		break;
//			case	7:	strItem = CSV_042_ITEM_07;		break;
//			case	8:	strItem = CSV_042_ITEM_08;		break;
//			case	9:	strItem = CSV_042_ITEM_11;		break;
//			case	10:	strItem = CSV_042_ITEM_12;		break;
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
