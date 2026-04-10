// midori 170504 add -->
#include "StdAfx.h"
#include "impUc181Sonota2.h"

// No.200903 add -->
extern BOOL	bG_Kanso;
// No.200903 add <--

// コンストラクタ
CimpUc181Sonota2::CimpUc181Sonota2(int formSeq1,int formSeq2,CString csv_181_item_02,CString csv_181_item_03)
	: CimpUc000Common()
{
	m_pUcData = NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
	// 様式シーケンスを指定
	m_nFormSeq = formSeq1;
	// 様式並び順番号(表示インデックス) を指定
	m_nFormSeq2 = formSeq2;
	// 項目１
	m_CSV_181_ITEM_02 = csv_181_item_02;
	// 項目２
	m_CSV_181_ITEM_03 = csv_181_item_03;
}

// デストラクタ
CimpUc181Sonota2::~CimpUc181Sonota2(void)
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
int CimpUc181Sonota2::Init(CDatabase* pDB)
{
	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 共通で使用するテーブルとの接続を開始
	ConnectCmnTables(pDB);

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳票固有のコード
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_181_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc181Sonota2(pDB,m_nFormSeq));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq2 );

	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	GetKeiStr(m_nFormSeq2, &m_strSyokeiName, &m_strChukeiName);

	//--------------------------------------------------
	// 共通データの設定
	//--------------------------------------------------
	// １頁内の合計行を含む全体の行数
	m_RowMax		= ID_ROWKOBETU_181;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_181 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_181;

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
int CimpUc181Sonota2::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc181Sonota2*)m_pUcCmn;

	nRet = IMP_REC_OK;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 科目
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_171_KAMOKU*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_181_ITEM_01);
		m_uImpCsvInfo.nLength = MAX_KETA_171_KAMOKU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
	// 02 項目１
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_181_ITEM) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), m_CSV_181_ITEM_02);
		m_uImpCsvInfo.nLength = MAX_KETA_181_ITEM / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
	// 03 項目２
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_181_ITEM) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), m_CSV_181_ITEM_03);
		m_uImpCsvInfo.nLength = MAX_KETA_181_ITEM / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
	// 04 期末現在高
// No.200903 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(4), ID_VAL_181_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_181_ITEM_04);
	//	m_uImpCsvInfo.nLength = ID_VAL_181_DATA;
	//	m_uImpCsvInfo.nKind = 1;	// 1:金額
	//	if(pstrCsvDataAry->GetAt(4).Left(1) == '-')	{
	//		m_uImpCsvInfo.nLength--;
	//		m_uImpCsvInfo.nType = 1;
	//	}
	//	m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
	//	CsvInfoClr(&m_uImpCsvInfo);
	//	nRet = IMP_REC_NG;
	//}
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_181_K_DATA;
	else					len = ID_VAL_181_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(4), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_181_ITEM_04);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(4).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--
	// 05 摘要１
	if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_181_TEKI) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_181_ITEM_05);
		m_uImpCsvInfo.nLength = MAX_KETA_181_TEKI / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
	// 06 摘要２
	if (CheckImportStr(pstrCsvDataAry->GetAt(6), MAX_KETA_181_TEKI) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_181_ITEM_06);
		m_uImpCsvInfo.nLength = MAX_KETA_181_TEKI / 2;
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
//
//	// 科目
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
//		// 01 科目
//		m_pUcData->m_KnSeq		= nKnSeq;
//		m_pUcData->m_KnOrder	= nKnOrder;
//		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
//// midori 152137 add -->
//		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
//// midori 152137 add <--
//		// 02 項目１
//		// 03 項目２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
//		m_pUcData->m_Item		= strTemp;
//		// 04 期末現在高
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(4);
//		RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa
//		// 05 摘要１
//		// 06 摘要２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(5), pstrCsvDataAry->GetAt(6));
//		m_pUcData->m_Teki			= strTemp;
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
//		・データの正当性をチェック
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
int CimpUc181Sonota2::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	// 01 科目
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_171_KAMOKU*2);
	pstrCsvDataAry->SetAt(1,strTemp);
	// 02 項目１
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_181_ITEM);
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 項目２
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_181_ITEM);
	pstrCsvDataAry->SetAt(3,strTemp);
	// 04 期末現在高
// No.200903 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(4), ID_VAL_181_DATA, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_181_K_DATA;
	else					len = ID_VAL_181_DATA;
	if(CheckImportNum2(pstrCsvDataAry->GetAt(4), len, TRUE) != TRUE)	{
// No.200903 add <--
		strTemp.Empty();
		pstrCsvDataAry->SetAt(4,strTemp);
	}
	// 05 摘要１
	GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_181_TEKI);
	pstrCsvDataAry->SetAt(5,strTemp);
	// 06 摘要２
	GetSpritString(pstrCsvDataAry->GetAt(6),&strTemp,MAX_KETA_181_TEKI);
	pstrCsvDataAry->SetAt(6,strTemp);

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
	m_pUcData = (CdbUc181Sonota2*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順

	// 科目
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
		// 01 科目
		m_pUcData->m_KnSeq		= nKnSeq;
		m_pUcData->m_KnOrder	= nKnOrder;
		m_pUcData->m_KnName		= pstrCsvDataAry->GetAt(1);
// midori 152137 add -->
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));
// midori 152137 add <--
		// 02 項目１
		// 03 項目２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
		m_pUcData->m_Item		= strTemp;
		// 04 期末現在高
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(4);
		RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa
		// 05 摘要１
		// 06 摘要２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(5), pstrCsvDataAry->GetAt(6));
		m_pUcData->m_Teki			= strTemp;
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
int CimpUc181Sonota2::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc181Sonota2*)m_pUcCmn;

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

		// 01 科目
		pstrCsvDataAry->SetAt(1, m_pUcData->m_KnName);
		// 02 項目１
		// 03 項目２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Item, &strTempAry, 2);
		pstrCsvDataAry->SetAt(2, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(3, strTempAry.GetAt(1));
		// 04 期末現在高
		pstrCsvDataAry->SetAt(4, m_pUcData->m_Val);
		// 05 摘要１
		// 06 摘要２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
		pstrCsvDataAry->SetAt(5, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(6, strTempAry.GetAt(1));
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}
// midori 170504 add <--
