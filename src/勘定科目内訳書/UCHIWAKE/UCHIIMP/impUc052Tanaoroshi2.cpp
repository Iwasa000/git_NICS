#include "StdAfx.h"
#include "impUc052Tanaoroshi2.h"

// midori 190301 0415 add -->
extern	BOOL	bG_Kanso;
// midori 190301 0415 add <--

// コンストラクタ
CimpUc052Tanaoroshi2::CimpUc052Tanaoroshi2(void)
	: CimpUc000Common()
{
	m_pUcData				= NULL;
// midori 180404 add -->
	m_FgFunc = 0;
	m_Method = 0;
// midori 180404 add <--
}

// デストラクタ
CimpUc052Tanaoroshi2::~CimpUc052Tanaoroshi2(void)
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
int CimpUc052Tanaoroshi2::Init(CDatabase* pDB)
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
	m_nFormSeq = 0;		// [欄外]なので様式シーケンスはない

	// 項目数を指定（CSV_XXX_ITEM_CNT）
	m_nItemCount = CSV_052_ITEM_CNT;

	// 内訳書テーブルとの接続を開始
	m_pUcCmn = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(pDB));

	// InfSubテーブルの初期化
	m_pUcInfSub->Init( m_nFormSeq );

	m_RowMax = CSV_052_ROW_MAX;


	//--------------------------------------------------
	// 全帳票共通のコード
	//--------------------------------------------------
	// 小計、中計文言の取得
	//GetKeiStr(m_nFormSeq, &m_strSyokeiName, &m_strChukeiName);		// [欄外]なので計行はない


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
int CimpUc052Tanaoroshi2::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
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
	// CSVエラーの情報をクリアする
	CsvInfoClr(&m_uImpCsvInfo);
	int nRet = SetCsvOneLineDataSub(pstrCsvDataAry, &m_FgFunc);
// midori 180404 add <--
	// エラーなら終了
	if (nRet != ID_FGFUNC_NULL) {
		return nRet;
	}

// midori 190301 0415 add -->
	if(bG_Kanso == TRUE) {
		return(IMP_REC_OK);
	}
// midori 190301 0415 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル操作の準備
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 一時的にテーブルアクセス領域を拡張
	m_pUcData = (CdbUc052Tanaoroshi2*)m_pUcCmn;

	// 2014/09/01 欄外登録の複数ページ対応
	// 1データのみインポート可能（2つ目以降は無視）
	//if (m_nImportCount > 1) {
	//	return IMP_REC_OK;
	//}

// midori 150899 add -->
	nRet = IMP_REC_OK;
// midori 150899 add <--

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// データチェック
	//++++++++++++++++++++++++++++++++++++++++++++++++++
// midori 180404 del -->
//	// 期末棚卸の方法と日付は、ここでデータ加工も行う
//	BYTE			cTemp			= 0;			// フラグテンポラリ
//	long			nTemp			= 0;			// 数値テンポラリ
//
//	// 01 期末棚卸の方法
//	if (ConvMethodStrToDbVal(pstrCsvDataAry->GetAt(1), &cTemp) != TRUE) {
//		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_052_ITEM_01);
//// midori 150899 cor -->
//		//return IMP_REC_NG;
//// ---------------------
//		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
//		CsvInfoClr(&m_uImpCsvInfo);
//		nRet = IMP_REC_NG;
//// midori 150899 cor <--
//	}
// midori 180404 del <--
// midori 180404 add -->
	long			nTemp			= 0;			// 数値テンポラリ

	// 期末棚卸の方法は、ここでデータ加工も行う
	// 01 期末棚卸の方法
	if (ConvMethodStrToDbVal(pstrCsvDataAry->GetAt(1), &m_Method) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_052_ITEM_01);
		m_uImpCsvInfo.nErr = 3;
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		nRet = IMP_REC_NG;
		return(nRet);
	}
// midori 180404 add <--

	// 02 登録名称
	if (CheckImportStr(pstrCsvDataAry->GetAt(2), 30) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_052_ITEM_02);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nLength = 30 / 2;
		m_uImpCsvInfo.nKind = 0;	// 0:文字
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
	}

	// 03 棚卸を行った時期
	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(3), &nTemp) != TRUE) {
		strcpy_s(m_uImpCsvInfo.szColumn, sizeof(m_uImpCsvInfo.szColumn), CSV_052_ITEM_03);
// midori 150899 cor -->
		//return IMP_REC_NG;
// ---------------------
		m_uImpCsvInfo.nKind = 3;	// 3:日付
		m_ErrMap.SetAt((int)m_ErrMap.GetCount(),m_uImpCsvInfo);
		CsvInfoClr(&m_uImpCsvInfo);
		nRet = IMP_REC_NG;
// midori 150899 cor <--
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
//	// 
//	// なし
//	// 
//
//// midori 150954 add -->
//	// インポートしようとした回数をカウント
//	m_nImportCount++;
//// midori 150954 add <--
//
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// データ加工
//	// テーブルへデータ登録
//	//++++++++++++++++++++++++++++++++++++++++++++++++++
//	// 新規レコード作成
//	m_pUcData->CreateNewRecord();
//
//	// レコード編集開始
//	m_pUcData->Edit();
//
//	// XX 頁番号
//	// XX 行番号
//	//CheckNumPageRow(&m_NumPage, &m_NumRow);
//	m_pUcData->m_NumRow			= GetRow(m_nImportCount);
//	m_pUcData->m_NumPage		= GetPage(m_nImportCount - 1);
//	
//
//	// 00 特殊行フラグ
//	m_pUcData->m_FgFunc			= cFgFunc;
//
//	// 内訳の場合のみ各項目のデータを登録
//	switch (cFgFunc) {
//	case	ID_FGFUNC_TANAOROSHI:		// データ行
//		// 01 期末棚卸の方法
//		m_pUcData->m_Method		= cTemp;
//
//		// 02 登録名称
////		m_pUcData->m_HdName		= pstrCsvDataAry->GetAt(2);
//		if ( cTemp == 4 ) {				// [2009/07/13 Fix]
//			m_pUcData->m_HdName		= pstrCsvDataAry->GetAt(2);
//		} else {
//			m_pUcData->m_HdName		= _T("");
//		}
//
//		// 03 棚卸を行った時期
//		m_pUcData->m_SpDate		= nTemp;
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
int CimpUc052Tanaoroshi2::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//--------------------------------------------------
	// 前処理（全帳票共通のコード）
	//--------------------------------------------------
	int				ii=0;
	int				sw=0;
	long			nTemp = 0;						// 数値テンポラリ
	CString			strTemp			= _T("");		// 文字列テンポラリ
// midori UC_0054 add -->
	CString			strCsvId = _T("");
// midori UC_0054 add <--

// midori 190301 0415 add -->
	if(bG_Kanso == TRUE)	{
		return IMP_REC_OK;
	}
// midori 190301 0415 add <--

	// 文字数をオーバーしていればカットする

	// 02 登録名称
	GetSpritString(pstrCsvDataAry->GetAt(2),&strTemp,30);
	pstrCsvDataAry->SetAt(2,strTemp);

	// 03 棚卸を行った時期
	// エラーの場合、クリアーする
	if (ConvDateStrToDbVal(IMP_DATE_CONV_TYPE_A, pstrCsvDataAry->GetAt(3), &nTemp) != TRUE) {
		strTemp.Empty();
		pstrCsvDataAry->SetAt(3,strTemp);
		nTemp = 0;
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
		for (ii = 2; ii <(int)pstrCsvDataAry->GetSize(); ii++) {
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
	m_pUcData = (CdbUc052Tanaoroshi2*)m_pUcCmn;

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 名称テーブルへ登録
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 
	// なし
	// 

	// インポートしようとした回数をカウント
	m_nImportCount++;

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
	//CheckNumPageRow(&m_NumPage, &m_NumRow);
	m_pUcData->m_NumRow			= GetRow(m_nImportCount);
	m_pUcData->m_NumPage		= GetPage(m_nImportCount - 1);
	

	// 00 特殊行フラグ
	m_pUcData->m_FgFunc			= m_FgFunc;

	// 内訳の場合のみ各項目のデータを登録
	switch (m_FgFunc) {
	case	ID_FGFUNC_TANAOROSHI:		// データ行
		// 01 期末棚卸の方法
		m_pUcData->m_Method		= m_Method;

		// 02 登録名称
//		m_pUcData->m_HdName		= pstrCsvDataAry->GetAt(2);
		if ( m_Method == 4 ) {				// [2009/07/13 Fix]
			m_pUcData->m_HdName		= pstrCsvDataAry->GetAt(2);
		} else {
			m_pUcData->m_HdName		= _T("");
		}

		// 03 棚卸を行った時期
		m_pUcData->m_SpDate		= nTemp;
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
int CimpUc052Tanaoroshi2::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
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
	m_pUcData = (CdbUc052Tanaoroshi2*)m_pUcCmn;

	// 2014/09/01 欄外登録の複数ページ対応
	// 1頁目の1行目のデータのみ許可（1データしか存在しない）
	//if ((m_pUcData->m_NumPage != 1) || (m_pUcData->m_NumRow != 1)) {
	//	// 次のレコードに移動しておく
	//	m_pUcData->MoveNext();

	//	return IMP_REC_OK;
	//}


	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブルからデータ取得
	// データ加工
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	CString			strTemp			= _T("");		// 文字列テンポラリ

	// 00 項目名称
	pstrCsvDataAry->SetAt(0, strCsvId);

	// 内訳の場合のみ各項目のデータを取得
	if (strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) {
// midori 190301 0415 del -->
		//// 01 期末棚卸の方法
		//strTemp = _T("");
		//ConvMethodDbValToStr(m_pUcData->m_Method, &strTemp);
		//pstrCsvDataAry->SetAt(1, strTemp);

		//// 02 登録名称
		//strTemp = _T("");
		//if ( m_pUcData->m_Method == 4 ) {		// 期末棚卸の方法＝"Ｄ"
		//	strTemp = m_pUcData->m_HdName;
		//} else {								// 期末棚卸の方法＝"Ａ"～"Ｃ"
		//	ConvRegNameDbValToStr(m_pUcData->m_Method, &strTemp);
		//}
		//pstrCsvDataAry->SetAt(2, strTemp);

		//// 03 棚卸を行った時期
		//strTemp = _T("");
		//ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A, m_pUcData->m_SpDate, &strTemp);
		//pstrCsvDataAry->SetAt(3, strTemp);
// midori 190301 0415 del <--
// midori 190301 0415 add -->
		// 01 期末棚卸の方法
		strTemp = _T("");
		if(bG_Kanso != TRUE) {
			ConvMethodDbValToStr(m_pUcData->m_Method,&strTemp);
		}
		pstrCsvDataAry->SetAt(1, strTemp);

		// 02 登録名称
		strTemp = _T("");
		if(bG_Kanso != TRUE) {
			if(m_pUcData->m_Method == 4) {		// 期末棚卸の方法＝"Ｄ"
				strTemp = m_pUcData->m_HdName;
			} else {								// 期末棚卸の方法＝"Ａ"～"Ｃ"
				ConvRegNameDbValToStr(m_pUcData->m_Method,&strTemp);
			}
		}
		pstrCsvDataAry->SetAt(2, strTemp);

		// 03 棚卸を行った時期
		strTemp = _T("");
		if(bG_Kanso != TRUE) {
			ConvDateDbValToStr(IMP_DATE_CONV_TYPE_A,m_pUcData->m_SpDate,&strTemp);
		}
		pstrCsvDataAry->SetAt(3, strTemp);
// midori 190301 0415 add <--
	}


	//--------------------------------------------------
	// 後処理（全帳票共通のコード）
	//--------------------------------------------------
	// 次のレコードに移動しておく
	m_pUcData->MoveNext();

	return nRet;
}


//**********************************************************************
// 期末棚卸の方法を文字列→DBデータに変換
//		・データインポートで使用。
//		・文字列とDBデータの対応。
//			--------------------------------------------------
//			No.	文字列			DBデータ	備考
//			--------------------------------------------------
//			1	“Ａ”			1			「実施棚卸」
//			2	“Ｂ”			2			「帳簿棚卸」
//			3	“Ｃ”			3			「AとBとの併用」
//			4	“Ｄ”			4			登録名称を印字
//			5	その他の文字列	0			関数失敗
//			--------------------------------------------------
// 
//	引数
//			strMethod				[IN] 文字列
//			pcMethod				[OUT]DBデータ（期末棚卸の方法）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc052Tanaoroshi2::ConvMethodStrToDbVal(CString strMethod, BYTE* pcMethod)
{
	// 「実施棚卸」？
	if (strMethod.Compare(CSV_052_METHOD_A) == 0) {
		*pcMethod = DT_METHOD_A;
		return TRUE;
	}

	// 「帳簿棚卸」？
	if (strMethod.Compare(CSV_052_METHOD_B) == 0) {
		*pcMethod = DT_METHOD_B;
		return TRUE;
	}

	// 「AとBとの併用」？
	if (strMethod.Compare(CSV_052_METHOD_C) == 0) {
		*pcMethod = DT_METHOD_C;
		return TRUE;
	}

	// 登録名称を印字？
	if (strMethod.Compare(CSV_052_METHOD_D) == 0) {
		*pcMethod = DT_METHOD_D;
		return TRUE;
	}

	// インポート対象外
	*pcMethod = DT_METHOD_NOTHING;
// midori 150972 cor -->
//// midori 150904 add -->
//	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_142_KUBUN);
//// midori 150904 add <--
// ---------------------
	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_052_KUBUN);
// midori 150972 cor <--
	return FALSE;
}

//**********************************************************************
// 期末棚卸の方法をDBデータ→文字列に変換
//		・データエクスポートで使用。
//		・DBデータと文字列の対応。
//			--------------------------------------------------
//			No.	DBデータ		文字列		備考
//			--------------------------------------------------
//			1	1				“Ａ”		「実施棚卸」
//			2	2				“Ｂ”		「帳簿棚卸」
//			3	3				“Ｃ”		「AとBとの併用」
//			4	4				“Ｄ”		登録名称を印字
//			5	その他の値		“”		関数失敗
//			--------------------------------------------------
// 
//	引数
//			cMethod					[IN] DBデータ（期末棚卸の方法）
//			pstrMethod				[OUT]文字列
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc052Tanaoroshi2::ConvMethodDbValToStr(BYTE cMethod, CString* pstrMethod)
{
	// 期末棚卸の方法？
	switch (cMethod) {
	case	DT_METHOD_A:		// 「実施棚卸」
		*pstrMethod = CSV_052_METHOD_A;
		return TRUE;
	case	DT_METHOD_B:		// 「帳簿棚卸」
		*pstrMethod = CSV_052_METHOD_B;
		return TRUE;
	case	DT_METHOD_C:		// 「AとBとの併用」
		*pstrMethod = CSV_052_METHOD_C;
		return TRUE;
	case	DT_METHOD_D:		// 登録名称を印字
		*pstrMethod = CSV_052_METHOD_D;
		return TRUE;
	}

	// エクスポート対象外
	*pstrMethod = _T("");
	return FALSE;
}

//**********************************************************************
// 登録名称をDBデータ→文字列に変換
//		・データエクスポートで使用。
//		・DBデータと文字列の対応。
//			--------------------------------------------------
//			No.	DBデータ		文字列		備考
//			--------------------------------------------------
//			1	1				“実施棚卸”
//			2	2				“帳簿棚卸”
//			3	3				“ＡとＢとの併用”
//			4	その他の値		“”		関数失敗
//			--------------------------------------------------
// 
//	引数
//			cMethod					[IN] DBデータ（期末棚卸の方法）
//			pstrMethod				[OUT]文字列
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc052Tanaoroshi2::ConvRegNameDbValToStr(BYTE cMethod, CString* pstrMethod)
{
	// 期末棚卸の方法？
	switch (cMethod) {
	case	DT_METHOD_A:		// 「実施棚卸」
		*pstrMethod = CSV_052_REGNAME_A;
		return TRUE;
	case	DT_METHOD_B:		// 「帳簿棚卸」
		*pstrMethod = CSV_052_REGNAME_B;
		return TRUE;
	case	DT_METHOD_C:		// 「AとBとの併用」
		*pstrMethod = CSV_052_REGNAME_C;
		return TRUE;
	}

	// エクスポート対象外
	*pstrMethod = _T("");
	return FALSE;
}