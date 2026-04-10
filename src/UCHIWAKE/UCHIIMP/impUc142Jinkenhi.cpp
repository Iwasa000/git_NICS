#include "StdAfx.h"
#include "impUc142Jinkenhi.h"

// No.200903 add -->
extern BOOL	bG_Kanso;
// No.200903 add <--

// コンストラクタ
CimpUc142Jinkenhi::CimpUc142Jinkenhi(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
	fYakuinHousyu			= 0;
	fJugyoinKyuyo			= 0;
	fJugyoinChingin			= 0;
// midori 180404 add -->
	m_FgFunc = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc142Jinkenhi::~CimpUc142Jinkenhi(void)
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
int CimpUc142Jinkenhi::Init(CDatabase* pDB)
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
	m_nFormSeq = ID_FORMNO_142;

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_142_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc142Jinkenhi(pDB));

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
	m_RowMax		= ID_ROWKOBETU_142;
	// １頁内のデータ行数（合計行は含まず）
	m_RowMaxData	= ID_ROWKOBETU_142 - 1;
	// 帳表固定の合計行数（1:合計あり，0:なし）
	m_RowKei		= ID_ROWKEI_142;

	// 出力設定が通常の場合
	if( m_pUcInfFormType->m_FormType == 0 ){
		m_RowMax		= ID_ROWNORMAL_142;
		m_RowMaxData	= ID_ROWNORMAL_142 - 1;
	}


	return IMP_ERRCODE_SUCCESS;
}

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
int CimpUc142Jinkenhi::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
// midori 180404 del -->
//	BYTE			cFgFunc			= 0;			// 特殊行フラグ
// midori 180404 del <--
// midori 150973 add -->
	int				nErr=0;
	int				nRow=0;
// midori 150973 add <--
// No.200903 add -->
	int				len=0;
// No.200903 add <--

// midori 180404 del -->
//	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &cFgFunc);
// midori 180404 del <--
// midori 180404 add -->
	// CSVエラーの情報をクリアする
	CsvInfoClr(&m_uImpCsvInfo);
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
	m_pUcData = (CdbUc142Jinkenhi*)m_pUcCmn;

	// 1データのみインポート可能	[2009/07/10 Delete for In any order]
/*	if (m_nImportCount > 3) {
		return IMP_REC_OK;
	}
*/

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	nRet = IMP_REC_OK;

	// 01 区分
/*	if (CheckImportStr(pstrCsvDataAry->GetAt(1), 7*2) != TRUE) {
		nRet = IMP_REC_NG;
	}
*/

	// 02 総額
/*	if (CheckImportNum(pstrCsvDataAry->GetAt(2), 11) != TRUE) {
		nRet = IMP_REC_NG;
	}
*/

// midori 150973 cor -->
//// midori 150946,150947 add -->
//	// 01 区分
//	// 行の振り分け				// [2009/07/10 Add for In any order]
//	int nRow = 0;
//	CString strTemp = pstrCsvDataAry->GetAt(1);
//	if ( (strTemp.Compare(CSV_142_SEQ_01) == 0) && (fYakuinHousyu   == 0) ) {	// 役員報酬手当員
//		nRow = 1;
//		fYakuinHousyu = 1;
//	} else 
//	if ( (strTemp.Compare(CSV_142_SEQ_02) == 0) && (fJugyoinKyuyo   == 0) ) {	// 従業員給料手当
//		nRow = 2;
//		fJugyoinKyuyo = 1;
//	} else 
//	if ( (strTemp.Compare(CSV_142_SEQ_03) == 0) && (fJugyoinChingin == 0) ) {	// 従業員賃金手当
//		nRow = 3;
//		fJugyoinChingin = 1;
//	} else {
//		// このエラーの場合、項目名は不要
//		memset(m_uImpCsvInfo.szColumn,0, sizeof(m_uImpCsvInfo.szColumn));
//		if( strTemp.Compare(CSV_142_SEQ_01) != 0 && 
//			strTemp.Compare(CSV_142_SEQ_02) != 0 && 
//			strTemp.Compare(CSV_142_SEQ_03) != 0 )	{
//			// 区分が正しくありません。
//			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_142_KUBUN);
//		}
//		else	{
//			// 「役員報酬手当」が重複しています。
//			sprintf_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),_T("「%s」が重複しています。"),strTemp);
//		}
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//		return(nRet);
//	}
//// midori 150946,150947 add <--
// ---------------------
	// 01 区分
	// 行の振り分け				// [2009/07/10 Add for In any order]
	CString strTemp = pstrCsvDataAry->GetAt(1);
	if ( (strTemp.Compare(CSV_142_SEQ_01) == 0) && (fYakuinHousyu   == 0) ) {	// 役員報酬手当員
		nErr = 0;
	}
	else if ( (strTemp.Compare(CSV_142_SEQ_02) == 0) && (fJugyoinKyuyo   == 0) ) {	// 従業員給料手当
		nErr = 0;
	}
	else if ( (strTemp.Compare(CSV_142_SEQ_03) == 0) && (fJugyoinChingin == 0) ) {	// 従業員賃金手当
		nErr = 0;
	}
	else	{
		nErr = 1;
	}
	if(nErr == 1)	{
		// このエラーの場合、項目名は不要
		memset(m_uImpCsvInfo.szColumn,0, sizeof(m_uImpCsvInfo.szColumn));
		if( strTemp.Compare(CSV_142_SEQ_01) != 0 && 
			strTemp.Compare(CSV_142_SEQ_02) != 0 && 
			strTemp.Compare(CSV_142_SEQ_03) != 0 )	{
			// 区分が正しくありません。
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_142_KUBUN);
		}
		else	{
			// 「役員報酬手当」が重複しています。
			sprintf_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),_T("「%s」が重複しています。"),strTemp);
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
// midori 180404 del -->
//		CsvInfoClr(&m_uImpCsvInfo);
// midori 180404 del <--
// midori 180404 add -->
		m_uImpCsvInfo.nErr = 3;
// midori 180404 add <--
		nRet = IMP_REC_NG;
		return(nRet);
	}
// midori 150973 cor <--

// No.200903 del -->
////	if (CheckImportNum(pstrCsvDataAry->GetAt(2), 11, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(2), 11, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_142_ITEM_02);
//// midori 150899 cor -->
//		//return IMP_REC_NG;														// [2009/07/10 Fix]
//// ---------------------
//		m_uImpCsvInfo.nLength = 11;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(2).Left(1) == '-')	{
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
	if(bG_Kanso == TRUE)	len = 13;
	else					len = 11;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(2), len, TRUE) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_142_ITEM_02);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(2).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

	// 03 総額のうち代表者及びその家族分
/*	if (CheckImportNum(pstrCsvDataAry->GetAt(3), 11) != TRUE) {
		nRet = IMP_REC_NG;
	}
*/

// No.200903 del -->
////	if (CheckImportNum(pstrCsvDataAry->GetAt(3), 11, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//	if (CheckImportNum2(pstrCsvDataAry->GetAt(3), 11, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_142_ITEM_03);
//// midori 150899 cor -->
//		//return IMP_REC_NG;														// [2009/07/10 Fix]
//// ---------------------
//		m_uImpCsvInfo.nLength = 11;
//		m_uImpCsvInfo.nKind = 1;	// 1:金額
//		if(pstrCsvDataAry->GetAt(3).Left(1) == '-')	{
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
	if(bG_Kanso == TRUE)	len = 13;
	else					len = 11;
	if (CheckImportNum2(pstrCsvDataAry->GetAt(3), len, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_142_ITEM_03);
		m_uImpCsvInfo.nLength = len;
		m_uImpCsvInfo.nKind = 1;	// 1:金額
		if(pstrCsvDataAry->GetAt(3).Left(1) == '-')	{
			m_uImpCsvInfo.nLength--;
			m_uImpCsvInfo.nType = 1;
		}
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
	}
// No.200903 add <--

// midori 150946,150947 del -->
//	// 行の振り分け				// [2009/07/10 Add for In any order]
//	int nRow = 0;
//	CString strTemp = pstrCsvDataAry->GetAt(1);
//	if ( (strTemp.Compare(CSV_142_SEQ_01) == 0) && (fYakuinHousyu   == 0) ) {	// 役員報酬手当員
//		nRow = 1;
//		fYakuinHousyu = 1;
//	} else 
//	if ( (strTemp.Compare(CSV_142_SEQ_02) == 0) && (fJugyoinKyuyo   == 0) ) {	// 従業員給料手当
//		nRow = 2;
//		fJugyoinKyuyo = 1;
//	} else 
//	if ( (strTemp.Compare(CSV_142_SEQ_03) == 0) && (fJugyoinChingin == 0) ) {	// 従業員賃金手当
//		nRow = 3;
//		fJugyoinChingin = 1;
//	} else {
//		//strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_142_ITEM_01);
//		strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_142_KUBUN);
//		return IMP_REC_NG;
//	}
// midori 150946,150947 del <--

// midori 150895 add -->
	if(nRet == IMP_REC_NG)	{
		return(nRet);
	}
// midori 150895 add <--

// midori 180404 del -->
//// midori 150973 add -->
//	else	{
//		// 行の振り分け
//		CString strTemp = pstrCsvDataAry->GetAt(1);
//		if((strTemp.Compare(CSV_142_SEQ_01) == 0) && (fYakuinHousyu == 0))			{	// 役員報酬手当員
//			nRow = 1;
//			fYakuinHousyu = 1;
//		}
//		else if((strTemp.Compare(CSV_142_SEQ_02) == 0) && (fJugyoinKyuyo == 0))		{	// 従業員給料手当
//			nRow = 2;
//			fJugyoinKyuyo = 1;
//		}
//		else if((strTemp.Compare(CSV_142_SEQ_03) == 0) && (fJugyoinChingin == 0))	{	// 従業員賃金手当
//			nRow = 3;
//			fJugyoinChingin = 1;
//		}
//	}
//// midori 150973 add <--
//
///*	[2009/07/10 Delete]
//	// 行数にて登録する項目を判断する
//	switch (m_nImportCount){
//		case 1:
//			if (pstrCsvDataAry->GetAt(1) != CSV_142_SEQ_01) {
//				nRet = IMP_REC_NG;
//			}
//			break;
//		case 2:
//			if (pstrCsvDataAry->GetAt(1) != CSV_142_SEQ_02) {
//				nRet = IMP_REC_NG;
//			}
//			break;
//		case 3:
//			if (pstrCsvDataAry->GetAt(1) != CSV_142_SEQ_03) {
//				nRet = IMP_REC_NG;
//			}
//			break;
//	}
//*/
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 名称テーブルへ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 
//	// TODO: ここに各帳票固有の処理を記述！
//	// 
//
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// データ加工
//	// テーブルへデータ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
////	CString			strTemp			= _T("");		// 文字列テンポラリ
//
//	// 新規レコード作成
//	m_pUcData->CreateNewRecord();
//
//	// レコード編集開始
//	m_pUcData->Edit();
//
//	// XX 頁番号
//	// XX 行番号
///*	CheckNumPageRow(&m_NumPage, &m_NumRow);
//	m_pUcData->m_NumPage		= m_NumPage;
//	m_pUcData->m_NumRow			= m_NumRow;
//*/
//	m_pUcData->m_NumPage		= 1;							// [2009/07/10 Change for In any order
//	m_pUcData->m_NumRow			= nRow;
//
//	// 00 特殊行フラグ
//	m_pUcData->m_FgFunc			= cFgFunc;
//
//	// [明細]の場合のみ各項目のデータを登録
//	// [小計]、[中計]は、計文言と頁番号、行番号、特殊行フラグのみ
//	// 空行は、頁番号、行番号、特殊行フラグのみ
//	switch (cFgFunc) {
//	case	ID_FGFUNC_DATA:			// データ行
///*		m_pUcData->m_Seq = m_nImportCount;						// [2009/07/10 Delete for In any order]
//		m_pUcData->m_NumRow = (int)m_nImportCount;				// [2009/07/10 Delete for In any order]
//		// 01 区分
//		
//		// 02 総額
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(2);		// [2009/07/10 Change for In any order]
//		if ( nRet == IMP_REC_OK ){
//			m_pUcData->m_Val		= pstrCsvDataAry->GetAt(2);
//		}
//		else {
//			m_pUcData->m_Val		= _T("");
//		}
//
//		// 03 総額のうち代表者及びその家族分
//		if ( nRet == IMP_REC_OK ){
//			m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(3);
//		}
//		else {
//			m_pUcData->m_Val2		= _T("");
//		}
//*/
//		// 01 区分
//
//		// 02 総額
//		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(2);		// [2009/07/10 Change for In any order]
//		RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa
//
//		// 03 総額のうち代表者及びその家族分
//		m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(3);		// [2009/07/10 Change for In any order]
//		RemoveCommaImportNum( m_pUcData->m_Val2 );			// カンマを除く　2011.07.15 h.ukawa
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
//
////	return nRet;
// midori 180404 del <--
	return IMP_REC_OK;		// [2009/07/10 Change for In any order]
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
int CimpUc142Jinkenhi::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
	int				nRow=0;							// 保存する行
// No.200903 add -->
	int				len=0;
// No.200903 add <--
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

// No.200903 del -->
	//// 02 総額
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(2), 11, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(2,strTemp);
	//}
	//// 03 総額のうち代表者及びその家族分
	//if (CheckImportNum2(pstrCsvDataAry->GetAt(3), 11, TRUE) != TRUE) {			// [2009/07/09 Minus Value Support]
	//	strTemp.Empty();
	//	pstrCsvDataAry->SetAt(3,strTemp);
	//}
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	len = 13;
	else					len = 11;

	// 02 総額
	if(CheckImportNum2(pstrCsvDataAry->GetAt(2), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(2,strTemp);
	}
	// 03 総額のうち代表者及びその家族分
	if(CheckImportNum2(pstrCsvDataAry->GetAt(3), len, TRUE) != TRUE)	{
		strTemp.Empty();
		pstrCsvDataAry->SetAt(3,strTemp);
	}
// No.200903 add <--

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
		for (ii = 1; ii <(int)pstrCsvDataAry->GetSize(); ii++) {
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

	// 保存する行を取得
	// 区分重複チェック用のサインをONにする
	strTemp = pstrCsvDataAry->GetAt(1);
	if(strTemp.Compare(CSV_142_SEQ_01) == 0)			{	// 役員報酬手当員
		nRow = 1;
		fYakuinHousyu = 1;
	}
	else if(strTemp.Compare(CSV_142_SEQ_02) == 0)		{	// 従業員給料手当
		nRow = 2;
		fJugyoinKyuyo = 1;
	}
	else if(strTemp.Compare(CSV_142_SEQ_03) == 0)		{	// 従業員賃金手当
		nRow = 3;
		fJugyoinChingin = 1;
	}

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
	m_pUcData->m_NumPage		= 1;							// [2009/07/10 Change for In any order
	m_pUcData->m_NumRow			= nRow;

	// 00 特殊行フラグ
	m_pUcData->m_FgFunc			= m_FgFunc;

	// [明細]の場合のみ各項目のデータを登録
	// [小計]、[中計]は、計文言と頁番号、行番号、特殊行フラグのみ
	// 空行は、頁番号、行番号、特殊行フラグのみ
	switch (m_FgFunc) {
	case	ID_FGFUNC_DATA:			// データ行
		// 01 区分

		// 02 総額
		m_pUcData->m_Val		= pstrCsvDataAry->GetAt(2);		// [2009/07/10 Change for In any order]
		RemoveCommaImportNum( m_pUcData->m_Val );			// カンマを除く　2011.07.15 h.ukawa

		// 03 総額のうち代表者及びその家族分
		m_pUcData->m_Val2		= pstrCsvDataAry->GetAt(3);		// [2009/07/10 Change for In any order]
		RemoveCommaImportNum( m_pUcData->m_Val2 );			// カンマを除く　2011.07.15 h.ukawa
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

	return IMP_REC_OK;		// [2009/07/10 Change for In any order]
}
// midori 180404 add <--

// midori 150905 add -->
//**********************************************************************
// 人件費の内訳書用
// CSVでインポートしなかった行のデータをテーブルに登録
//**********************************************************************
int CimpUc142Jinkenhi::SetCsvNullLineData142(void)
{
	int		nRow = 0;
	CString	m_strFilter = _T("");

// midori 150955 add -->
	if(m_pUcData == NULL)	return IMP_REC_OK;
// midori 150955 add <--
	for(nRow=0; nRow<3; nRow++)	{
		m_strFilter.Format(_T("NumRow=%d"),nRow + 1);
		m_pUcData->m_strFilter = m_strFilter;
		m_pUcData->Requery();
		if(m_pUcData->IsEOF())	{
			// レコード作成
			m_pUcData->CreateNewRecord();
			m_pUcData->Edit();
			m_pUcData->m_NumPage	= 1;
			m_pUcData->m_NumRow		= nRow + 1;
			m_pUcData->m_FgFunc		= 0;
			m_pUcData->Update();
		}
	}

	return IMP_REC_OK;
}
// midori 150905 add <--

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
int CimpUc142Jinkenhi::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc142Jinkenhi*)m_pUcCmn;


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
		switch (m_pUcData->m_NumRow){
			case 1:
				strTemp	= CSV_142_SEQ_01;
				break;
			case 2:
				strTemp	= CSV_142_SEQ_02;
				break;
			case 3:
				strTemp	= CSV_142_SEQ_03;
				break;
			default:
				strTemp	= _T("");
				break;
		}

		// 01 区分
		pstrCsvDataAry->SetAt(1, strTemp);		
		
		// 02 総額
		pstrCsvDataAry->SetAt(2, m_pUcData->m_Val);	
		
		// 03 総額のうち代表者及びその家族分
		pstrCsvDataAry->SetAt(3, m_pUcData->m_Val2);	
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}