#include "StdAfx.h"
#include "impUc011Yotyokin.h"

// midori 190301 0415 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\prtConstData.h"

extern	BOOL	bG_Kanso;
// midori 190301 0415 add <--

extern BOOL RemoveCommaImportNum(CString &strVal );

// コンストラクタ
CimpUc011Yotyokin::CimpUc011Yotyokin(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc011Yotyokin::~CimpUc011Yotyokin(void)
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
int CimpUc011Yotyokin::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_011;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_011_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc011Yotyokin(pDB));

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
	m_RowMax		= ID_ROWKOBETU_011;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_011 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_011;

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
int CimpUc011Yotyokin::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// No.200903 add -->
	int			len=0;
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
	m_pUcData = (CdbUc011Yotyokin*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 金融機関名
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_011_KINYU*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_01);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_011_KINYU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 02 支店名
	if (CheckImportStr(pstrCsvDataAry->GetAt(2)	, MAX_KETA_011_KINYUSUB*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_02);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_011_KINYUSUB;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 03 種類
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_011_KAMOKU*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_03);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_011_KAMOKU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 04 口座番号
	if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_011_KOUZA, TRUE) != TRUE) {			// 半角文字入力
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_04);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_011_KOUZA;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor -->
	}

	// 05 期末現在高
// No.200903 del -->
////	if (CheckImportNum(pstrCsvDataAry->GetAt(5), ID_VAL_011_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(5), ID_VAL_011_DATA, TRUE) != TRUE) {	// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(5), ID_VAL_011_DATA, TRUE) != TRUE) {	// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_05);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL_011_DATA;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(5).Left(1) == '-')	{
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
	if(bG_Kanso == TRUE)	len = ID_VAL_011_K_DATA;
	else					len = ID_VAL_011_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(5), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_05);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(5).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

	// 06 摘要１
	if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_011_TEKIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_06);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_011_TEKIYOU / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 07 摘要２
	if (CheckImportStr(pstrCsvDataAry->GetAt(7), MAX_KETA_011_TEKIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_011_ITEM_07);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_011_TEKIYOU / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
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
//	long			nBkSeq			= 0;				// 金融機関シーケンス番号
//	long			nBkOrder		= 0;				// 金融機関並び順
//	long			nKnSeq			= 0;				// 科目シーケンス番号
//	long			nKnOrder		= 0;				// 科目並び順
//
//	// 01 金融機関名
//	// 02 支店名
//	if (CheckEntryBank(pstrCsvDataAry->GetAt(1), pstrCsvDataAry->GetAt(2), &nBkSeq, &nBkOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//	// 03 種類
//	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(3), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
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
//		// 01 金融機関名
//		// 02 支店名
//		m_pUcData->m_BkSeq		= nBkSeq;
//		m_pUcData->m_BkOrder	= nBkOrder;
//		m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(1);
//		m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(2);
//// midori 152137 add -->
//		m_pUcData->m_BkKana1	= fc.Convert(pstrCsvDataAry->GetAt(1));
//		m_pUcData->m_BkKana2	= fc.Convert(pstrCsvDataAry->GetAt(2));
//// midori 152137 add <--
//
//		// 03 種類
//		m_pUcData->m_KnSeq		= nKnSeq;
//		m_pUcData->m_KnOrder	= nKnOrder;
//		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(3);
//// midori 152137 add -->
//		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(3));
//// midori 152137 add <--
//
//		// 04 口座番号
//		m_pUcData->m_AcNum		= pstrCsvDataAry->GetAt(4);
//
//		// 05 期末現在高
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(5);
//		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//
//		// 06 摘要１
//		// 07 摘要２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(6), pstrCsvDataAry->GetAt(7));
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
int CimpUc011Yotyokin::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
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
// midori 190504 add -->
	CUcFunctionCommon	ufc;
// midori 190504 add <--

	// 文字数をオーバーしていればカットする
	// 01 金融機関名
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_011_KINYU*2);
// midori 190504 add -->
	strTemp = ufc.DeleteRightSpace(strTemp);
// midori 190504 add <--
	pstrCsvDataAry->SetAt(1,strTemp);
	// 02 支店名
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_011_KINYUSUB*2);
// midori 190504 add -->
	strTemp = ufc.DeleteRightSpace(strTemp);
// midori 190504 add <--
// midori 190301 0415 add -->
	if(bG_Kanso == TRUE)	{
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
// midori 190301 0415 add <--
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 種類
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_011_KAMOKU*2);
	pstrCsvDataAry->SetAt(3,strTemp);
	// 04 口座番号
	// エラーの場合、クリアーする
	if (CheckImportStr(pstrCsvDataAry->GetAt(4), MAX_KETA_011_KOUZA, TRUE) != TRUE) {			// 半角文字入力
		strTemp.Empty();
		pstrCsvDataAry->SetAt(4,strTemp);
	}
	// 05 期末現在高
	// エラーの場合、クリアーする
// No.200903 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(5), ID_VAL_011_DATA, TRUE) != TRUE) {	// [2009/07/09 Minus Value Support]
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_011_K_DATA;
	else					len = ID_VAL_011_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(5), len, TRUE) != TRUE)	{
// No.200903 add <--
		strTemp.Empty();
		pstrCsvDataAry->SetAt(5,strTemp);
	}
	// 06 摘要
	// 適用１（全角20、半角40文字　40byte）
	GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_011_TEKIYOU);
	pstrCsvDataAry->SetAt(6,strTemp);
	// 適用２（全角20、半角40文字　40byte）
	GetSpritString(pstrCsvDataAry->GetAt(7),&strTemp,MAX_KETA_011_TEKIYOU);
	pstrCsvDataAry->SetAt(7,strTemp);

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
	m_pUcData = (CdbUc011Yotyokin*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nBkSeq			= 0;				// 金融機関シーケンス番号
	long			nBkOrder		= 0;				// 金融機関並び順
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順

	// 01 金融機関名
	// 02 支店名
// midori 154689 del -->
	//if (CheckEntryBank(pstrCsvDataAry->GetAt(1), pstrCsvDataAry->GetAt(2), &nBkSeq, &nBkOrder) != TRUE) {
	//	return IMP_REC_NG;
	//}
// midori 154689 del <--
// midori 154689 add -->
	cs = pstrCsvDataAry->GetAt(2);
	cs.Replace(PRT_FORMAT_CHCODE,_T(""));
	if (CheckEntryBank(pstrCsvDataAry->GetAt(1), cs, &nBkSeq, &nBkOrder) != TRUE) {
		return IMP_REC_NG;
	}
// midori 154689 add <--

	// 03 種類
	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(3), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
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
		// 01 金融機関名
		// 02 支店名
		m_pUcData->m_BkSeq		= nBkSeq;
		m_pUcData->m_BkOrder	= nBkOrder;
		m_pUcData->m_BkName1	= pstrCsvDataAry->GetAt(1);
		m_pUcData->m_BkName2	= pstrCsvDataAry->GetAt(2);
		m_pUcData->m_BkKana1	= fc.Convert(pstrCsvDataAry->GetAt(1));
		m_pUcData->m_BkKana2	= fc.Convert(pstrCsvDataAry->GetAt(2));

		// 03 種類
		m_pUcData->m_KnSeq		= nKnSeq;
		m_pUcData->m_KnOrder	= nKnOrder;
		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(3);
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(3));

		// 04 口座番号
		m_pUcData->m_AcNum		= pstrCsvDataAry->GetAt(4);

		// 05 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(5);
		RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa

		// 06 摘要１
		// 07 摘要２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(6), pstrCsvDataAry->GetAt(7));
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
int CimpUc011Yotyokin::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc011Yotyokin*)m_pUcCmn;


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CStringArray	strTempAry;						// 文字列配列テンポラリ

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// [明細]の場合のみ各項目のデータを取得
	// [小計]、[中計]は識別子のみ
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
		// 01 金融機関名
		pstrCsvDataAry->SetAt(1, m_pUcData->m_BkName1);

		// 02 支店名
// midori 190301 0415 del -->
		//pstrCsvDataAry->SetAt(2, m_pUcData->m_BkName2);
// midori 190301 0415 del <--
// midori 190301 0415 add -->
		cs = m_pUcData->m_BkName2;
		if(bG_Kanso == TRUE)	{
			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		}
		pstrCsvDataAry->SetAt(2,cs);
// midori 190301 0415 add <--

		// 03 種類
		pstrCsvDataAry->SetAt(3, m_pUcData->m_KnName);

		// 04 口座番号
		pstrCsvDataAry->SetAt(4, m_pUcData->m_AcNum);

		// 05 期末現在高
		pstrCsvDataAry->SetAt(5, m_pUcData->m_Val);

		// 06 摘要１
		// 07 摘要２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
		pstrCsvDataAry->SetAt(6, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(7, strTempAry.GetAt(1));
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}

//int CimpUc011Yotyokin::virSpecialLineToNull(int nPage, int MaxRow)
//{
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// テーブル操作の準備
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 一時的にテーブルアクセス領域を拡張
//	m_pUcData = (CdbUc011Yotyokin*)m_pUcCmn;
//	
//	m_pUcData->MoveFirst();
//	while( !m_pUcData->IsEOF()){
//		if(( m_pUcData->m_NumPage == nPage ) && (m_pUcData->m_NumRow == MaxRow )){
//			break;
//		}
//		m_pUcData->MoveNext();
//	}
//
//	m_pUcData->Edit();
//	//m_pUcData->m_Seq = retVal;			// シーケンス番号を自動付番
//	//m_pUcData->m_NumPage = 0;			// (0)  ページ番号
//	//m_pUcData->m_NumRow = 0;			// (0)  行番号
//	m_pUcData->m_FgFunc = 0;			// (0)  空行
//	m_pUcData->m_FgShow = 0;			// (0)  表示
//	m_pUcData->m_KeiStr = _T("");		// ("") 計名称
//	m_pUcData->m_NumGroup = 0;			// (0)  グループ番号
//	m_pUcData->m_RenKcd = _T("");		// ("") 連動元情報(科目)
//	m_pUcData->m_RenEcd = 0;			// (0)  連動元情報(枝番)
//	m_pUcData->m_RenFgTemp = 0;		// (0)  連動テンポラリ情報
//
//	m_pUcData->m_BkSeq = 0;			// (0)  金融機関：(金融)シーケンス番号
//	m_pUcData->m_BkOrder = 0;			// (0)  金融機関：(金融)順序番号
//	m_pUcData->m_BkName1 = _T("");		// ("") 金融機関：(金融)銀行名
//	m_pUcData->m_BkName2 = _T("");		// ("") 金融機関：(金融)支店名
//	m_pUcData->m_KnSeq = 0;			// (0)  種類：(科目)シーケンス番号
//	m_pUcData->m_KnOrder = 0;			// (0)  種類：(科目)順序番号
//	m_pUcData->m_KnName = _T("");		// ("") 種類：(科目)名称
//	m_pUcData->m_AcNum = _T("");		// ("") 口座番号
//	m_pUcData->m_Val = _T("");			// ("") 期末現在高（金額）
//	m_pUcData->m_Teki = _T("");		// ("") 摘要
//	m_pUcData->Update();
//
//	return 0;
//}
