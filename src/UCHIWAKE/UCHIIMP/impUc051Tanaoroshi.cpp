#include "StdAfx.h"
#include "impUc051Tanaoroshi.h"

// 修正No.157647 add -->
extern BOOL	bG_Kanso;
// 修正No.157647 add <--

// コンストラクタ
CimpUc051Tanaoroshi::CimpUc051Tanaoroshi(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc051Tanaoroshi::~CimpUc051Tanaoroshi(void)
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
int CimpUc051Tanaoroshi::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_051;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_051_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc051Tanaoroshi(pDB));

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
	m_RowMax		= ID_ROWKOBETU_051;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_051 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_051;


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
int CimpUc051Tanaoroshi::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
// 修正No.157647 add -->
	int			len=0;
// 修正No.157647 add <--

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
	m_pUcData = (CdbUc051Tanaoroshi*)m_pUcCmn;

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 01 科目
	if (CheckImportStr(pstrCsvDataAry->GetAt(1), MAX_KETA_051_KAMOKU*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_01);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_051_KAMOKU;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 02 品目１
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), MAX_KETA_051_HINNMOKU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_02);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_051_HINNMOKU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 03 品目２
	if (CheckImportStr(pstrCsvDataAry->GetAt(3), MAX_KETA_051_HINNMOKU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_03);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_051_HINNMOKU/2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 04 数量
//	if (CheckImportNum(pstrCsvDataAry->GetAt(4), 7, 2) != TRUE) {
//	if (CheckImportNum(pstrCsvDataAry->GetAt(4), 7, 2, FALSE) != TRUE) {				// [2009/07/09 Minus Value Support]
	if (CheckImportNum2(pstrCsvDataAry->GetAt(4), 7, 2, FALSE) != TRUE) {				// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_04);
// midori 150895 cor -->
//		return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength	= 7;	// 整数最大桁数
		m_uImpCsvInfo.nLength2	= 2;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 05 単位
	if (CheckImportStr(pstrCsvDataAry->GetAt(5), MAX_KETA_051_TANNI*2) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_05);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_051_TANNI;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
}

	// 06 単価
//	if (CheckImportNum(pstrCsvDataAry->GetAt(6), 8, 2) != TRUE) {
//	if (CheckImportNum(pstrCsvDataAry->GetAt(6), 8, 2, TRUE) != TRUE) {					// [2009/07/09 Minus Value Support]
	if (CheckImportNum2(pstrCsvDataAry->GetAt(6), 8, 2, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_06);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength	= 8;	// 整数最大桁数
		m_uImpCsvInfo.nLength2	= 2;	// 小数点以下最大桁数
		m_uImpCsvInfo.nKind = 2;		// 2:面積等
		if(pstrCsvDataAry->GetAt(6).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 07 期末現在高
// 修正No.157647 del -->
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL_051_DATA) != TRUE) {
////	if (CheckImportNum(pstrCsvDataAry->GetAt(7), ID_VAL_051_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), ID_VAL_051_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_07);
//// midori 150895 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_uImpCsvInfo.nLength = ID_VAL_051_DATA;
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
// 修正No.157647 del <--
// 修正No.157647 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_051_K_DATA;
	else					len = ID_VAL_051_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_07);
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
// 修正No.157647 add <--

	// 08 摘要１
	if (CheckImportStr(pstrCsvDataAry->GetAt(8), MAX_KETA_051_TEKIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_08);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_051_TEKIYOU / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150895 cor <--
	}

	// 09 摘要２
	if (CheckImportStr(pstrCsvDataAry->GetAt(9), MAX_KETA_051_TEKIYOU) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_051_ITEM_09);
// midori 150895 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = MAX_KETA_051_TEKIYOU / 2;
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
//	long			nKnSeq			= 0;				// 科目シーケンス番号
//	long			nKnOrder		= 0;				// 科目並び順
//
//	// 01 科目
//	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
//		return IMP_REC_NG;
//	}
//
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// データ加工
//	// テーブルへデータ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	CString			strTemp			= _T("");		// 文字列テンポラリ
//	double			nTemp1			= 0;			// 数値テンポラリ１
//	double			nTemp2			= 0;			// 数値テンポラリ２
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
//		// 02 品目１
//		// 03 品目２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
//		m_pUcData->m_HdName		= strTemp;
//
//		// 04 数量
//		nTemp1 = 0;		// 「07 期末現在高」で使用
//		strTemp = _T("");
//		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(4), 100, 0, &nTemp1);	// ×100
//		m_pUcData->m_Suuryou	= strTemp;
//
//		// 05 単位
//		m_pUcData->m_Tanni		= pstrCsvDataAry->GetAt(5);
//
//		// 06 単価
//		nTemp2 = 0;		// 「07 期末現在高」で使用
//		strTemp = _T("");
//		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(6), 100, 0, &nTemp2);	// ×100
//		m_pUcData->m_Price		= strTemp;
//
//		// 07 期末現在高
//		if (pstrCsvDataAry->GetAt(7).IsEmpty() == TRUE) {						// データがない（空文字）の場合は自動計算とする
//			// 自動計算
//			m_pUcData->m_FgInput	= 0;
//			// 「数量」と「単価」ともに入力がある場合のみ計算する
//			strTemp = _T("");
//			if ((m_pUcData->m_Suuryou.IsEmpty()	!= TRUE) && 
//				(m_pUcData->m_Price.IsEmpty()	!= TRUE)) {
//				strTemp.Format(_T("%.0f"), ((nTemp1 / 100) * (nTemp2 / 100)));	// 数値（数量×単価）→文字列（小数第一位で四捨五入）
//				// DB上の有効桁数は14桁までなので、自動計算によりそれを超える場合は補正する
//				if (CheckImportStr(strTemp, 14) != TRUE) {		// この時点で数字以外の文字が入っていることはあり得ない（チェック済み）
//					strTemp = _T("99999999999999");
//				}
//			}
//			m_pUcData->m_Val		= strTemp;
//		}
//		else {
//			// 手入力
//			m_pUcData->m_FgInput	= 1;
//			m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
//			RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
//		}
//
//		// 08 摘要１
//		// 09 摘要２
//		strTemp = _T("");
//		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
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
int CimpUc051Tanaoroshi::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
// 修正No.157647 add -->
	int				len=0;
// 修正No.157647 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--


	// 文字数をオーバーしていればカットする
	// 01 科目
	GetSpritString(pstrCsvDataAry->GetAt(1),&strTemp,MAX_KETA_051_KAMOKU*2);
	pstrCsvDataAry->SetAt(1,strTemp);
	// 02 品目１
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,MAX_KETA_051_HINNMOKU);
	pstrCsvDataAry->SetAt(2,strTemp);
	// 03 品目２
	GetSpritString(pstrCsvDataAry->GetAt(3),&strTemp,MAX_KETA_051_HINNMOKU);
	pstrCsvDataAry->SetAt(3,strTemp);
	// 04 数量
	if (CheckImportNum2(pstrCsvDataAry->GetAt(4), 7, 2, FALSE) != TRUE) {				// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(4,strTemp);
	}
	// 05 単位
	GetSpritString(pstrCsvDataAry->GetAt(5),&strTemp,MAX_KETA_051_TANNI*2);
	pstrCsvDataAry->SetAt(5,strTemp);
	// 06 単価
	if (CheckImportNum2(pstrCsvDataAry->GetAt(6), 8, 2, TRUE) != TRUE) {				// [2009/07/09 Minus Value Support]
		strTemp.Empty();
		pstrCsvDataAry->SetAt(6,strTemp);
	}
	// 07 期末現在高
// 修正No.157647 del -->
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(7), ID_VAL_051_DATA, TRUE) != TRUE) {		// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(7,strTemp);
	//}
// 修正No.157647 del <--
// 修正No.157647 add -->
	if(bG_Kanso == TRUE)	len = ID_VAL_051_K_DATA;
	else					len = ID_VAL_051_DATA;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(7), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(7,strTemp);
	}
// 修正No.157647 add <--

	// 08 摘要１
	GetSpritString(pstrCsvDataAry->GetAt(8),&strTemp,MAX_KETA_051_TEKIYOU);
	pstrCsvDataAry->SetAt(8,strTemp);
	// 09 摘要２
	GetSpritString(pstrCsvDataAry->GetAt(9),&strTemp,MAX_KETA_051_TEKIYOU);
	pstrCsvDataAry->SetAt(9,strTemp);

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
	m_pUcData = (CdbUc051Tanaoroshi*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	long			nKnSeq			= 0;				// 科目シーケンス番号
	long			nKnOrder		= 0;				// 科目並び順

	// 01 科目
	if (CheckEntryKamoku(pstrCsvDataAry->GetAt(1), m_nFormSeq, &nKnSeq, &nKnOrder) != TRUE) {
		return IMP_REC_NG;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データ加工
	// テーブルへデータ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	double			nTemp1			= 0;			// 数値テンポラリ１
	double			nTemp2			= 0;			// 数値テンポラリ２

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
		m_pUcData->m_KnKana		= fc.Convert(pstrCsvDataAry->GetAt(1));

		// 02 品目１
		// 03 品目２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(2), pstrCsvDataAry->GetAt(3));
		m_pUcData->m_HdName		= strTemp;

		// 04 数量
		nTemp1 = 0;		// 「07 期末現在高」で使用
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(4), 100, 0, &nTemp1);	// ×100
		m_pUcData->m_Suuryou	= strTemp;

		// 05 単位
		m_pUcData->m_Tanni		= pstrCsvDataAry->GetAt(5);

		// 06 単価
		nTemp2 = 0;		// 「07 期末現在高」で使用
		strTemp = _T("");
		strTemp = CalcStrMul(pstrCsvDataAry->GetAt(6), 100, 0, &nTemp2);	// ×100
		m_pUcData->m_Price		= strTemp;

		// 07 期末現在高
// midori 154673 del -->
		//if (pstrCsvDataAry->GetAt(7).IsEmpty() == TRUE) {						// データがない（空文字）の場合は自動計算とする
		//	// 自動計算
		//	m_pUcData->m_FgInput	= 0;
		//	// 「数量」と「単価」ともに入力がある場合のみ計算する
		//	strTemp = _T("");
		//	if ((m_pUcData->m_Suuryou.IsEmpty()	!= TRUE) && 
		//		(m_pUcData->m_Price.IsEmpty()	!= TRUE)) {
		//		strTemp.Format(_T("%.0f"), ((nTemp1 / 100) * (nTemp2 / 100)));	// 数値（数量×単価）→文字列（小数第一位で四捨五入）
		//		// DB上の有効桁数は14桁までなので、自動計算によりそれを超える場合は補正する
		//		if (CheckImportStr(strTemp, 14) != TRUE) {		// この時点で数字以外の文字が入っていることはあり得ない（チェック済み）
		//			strTemp = _T("99999999999999");
		//		}
		//	}
		//	m_pUcData->m_Val		= strTemp;
		//}
		//else {
		//	// 手入力
		//	m_pUcData->m_FgInput	= 1;
		//	m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
		//	RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
		//}
// midori 154673 del <--
// midori 154673 add -->
		strTemp = _T("");
		if ((m_pUcData->m_Suuryou.IsEmpty()	!= TRUE) && 
			(m_pUcData->m_Price.IsEmpty()	!= TRUE)) {
			strTemp.Format(_T("%.0f"), ((nTemp1 / 100) * (nTemp2 / 100)));	// 数値（数量×単価）→文字列（小数第一位で四捨五入）
			// DB上の有効桁数は14桁までなので、自動計算によりそれを超える場合は補正する
			if (CheckImportStr(strTemp, 14) != TRUE) {		// この時点で数字以外の文字が入っていることはあり得ない（チェック済み）
				strTemp = _T("99999999999999");
			}
		}
		if (pstrCsvDataAry->GetAt(7).IsEmpty() == TRUE) {						// データがない（空文字）の場合は自動計算とする
			// 自動計算
			m_pUcData->m_FgInput	= 0;
			// 「数量」と「単価」ともに入力がある場合のみ計算する
			m_pUcData->m_Val		= strTemp;
		}
		else	{
			// 手入力
			if(pstrCsvDataAry->GetAt(7) == strTemp)	{
				// 自動計算
				m_pUcData->m_FgInput = 0;
			}
			else	{
				// 実額入力
				m_pUcData->m_FgInput = 1;
			}
			m_pUcData->m_Val		= pstrCsvDataAry->GetAt(7);
			RemoveCommaImportNum( m_pUcData->m_Val );				// カンマを除く　2011.07.15 h.ukawa
		}
// midori 154673 add <--

		// 08 摘要１
		// 09 摘要２
		strTemp = _T("");
		strTemp = m_clsFunc.StrDocking(pstrCsvDataAry->GetAt(8), pstrCsvDataAry->GetAt(9));
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
int CimpUc051Tanaoroshi::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc051Tanaoroshi*)m_pUcCmn;


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
		// 01 科目
		pstrCsvDataAry->SetAt(1, m_pUcData->m_KnName);

		// 02 品目１
		// 03 品目２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_HdName, &strTempAry, 2);
		pstrCsvDataAry->SetAt(2, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(3, strTempAry.GetAt(1));

		// 04 数量
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Suuryou, 0.01, 2);	// ÷100
		pstrCsvDataAry->SetAt(4, strTemp);

		// 05 単位
		pstrCsvDataAry->SetAt(5, m_pUcData->m_Tanni);

		// 06 単価
		strTemp = _T("");
		strTemp = CalcStrMul(m_pUcData->m_Price, 0.01, 2);		// ÷100
		pstrCsvDataAry->SetAt(6, strTemp);

		// 07 期末現在高
		pstrCsvDataAry->SetAt(7, m_pUcData->m_Val);

		// 08 摘要１
		// 09 摘要２
		strTempAry.RemoveAll();
		m_clsFunc.StrDivision(m_pUcData->m_Teki, &strTempAry, 2);
		pstrCsvDataAry->SetAt(8, strTempAry.GetAt(0));
		pstrCsvDataAry->SetAt(9, strTempAry.GetAt(1));
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
void CimpUc051Tanaoroshi::virTableEditIkkatuLine(int nType, CdbUc000Common* rsData, 
											  CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku)
{
	//CdbUc051Tanaoroshi*	mfcRec = (CdbUc051Tanaoroshi*)rsData;

	switch (nType) {
	case	0:
		// ソートの一括集計処理 ･･･ 科目の無い帳表はいらない
		m_pUcData->m_HdName = strName;					// 名称：品目
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定

		m_pUcData->m_KnSeq = uKamoku.intKnSeq;			// 科目情報
		m_pUcData->m_KnOrder = uKamoku.intKnOrder;		// 科目情報
		m_pUcData->m_KnName = uKamoku.strKnName;		// 科目情報
		break;
	case	1:
		// 一括金額参照の戻す処理
		m_pUcData->m_HdName = strName;					// 名称：品目
		m_pUcData->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case	2:
		// 特殊行挿入の一括金額選択時
		m_pUcData->m_HdName = strName;					// 名称：品目
		break;
	}
}