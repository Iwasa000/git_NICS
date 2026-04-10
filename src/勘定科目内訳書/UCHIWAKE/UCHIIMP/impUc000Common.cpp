#include "StdAfx.h"
#include "impUc000Common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc000Common.h"

// midori 180404 add -->
#define	AS_SPLIT_MAX	2048
// midori 180404 add <--
BOOL CimpUc000Common::m_RegAutoRef;

// 改良No.21-0086,21-0529 cor -->
//// midori 157044 add -->
//extern int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 157044 add <--
// ------------------------------
extern BOOL		bG_InvNo;				// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）

extern int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

// コンストラクタ
CimpUc000Common::CimpUc000Common(void)
{
	// 初期化
	ZeroMemory(&m_uImpCsvInfo, sizeof(m_uImpCsvInfo));

	m_pUcCmn				= NULL;
	m_pUcInfSub				= NULL;
	m_pUcListBank			= NULL;
	m_pUcListAddress		= NULL;
	m_pUcListKamoku			= NULL;
	m_pUcInfFormType		= NULL;
	m_NumPage				= 0;
	m_NumRow				= 0;
	m_nFormSeq				= 0;
// midori 160609 add -->
	m_nKaikakeRangai		= 0;
// midori 160609 add <--
	m_strSyokeiName			= _T("");
	m_strChukeiName			= _T("");
	m_nGroupNum				= 0;
	m_nItemCount			= 0;
	m_nImportCount			= 0;
	m_mapIkkatuData		= NULL;

	m_RowMax				= 0;
	m_RowMaxData			= 0;
	m_RowKei				= 0;
	m_FormType				= 0;
	m_KamokuVer				= FALSE;
	m_KamokuVer2			= 0;			// 改良No.21-0086,21-0529 add
// 210113_5301_1 add -->
	m_NewVer				= FALSE;
// 登録番号（法人番号）_23/11/21 add -->
	m_NewVer2				= FALSE;
// 登録番号（法人番号）_23/11/21 add <--
	m_IsExport				= FALSE;
// 210113_5301_1 add <--
	m_RegAutoRef			= FALSE;
}

// デストラクタ
CimpUc000Common::~CimpUc000Common(void)
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
int CimpUc000Common::Init(CDatabase* pDB)
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳表のクラスで処理を実装する
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// テーブル接続開始（オブジェクト生成）
	m_pUcCmn			= new CdbUc000Common(pDB);

	return IMP_ERRCODE_SUCCESS;
}

//**********************************************************************
// midori 180404 del -->
//// CSVの1行分のデータをテーブルに登録
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
int CimpUc000Common::SetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳表のクラスで処理を実装する
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	return IMP_REC_OK;
}

// midori 156935 add -->
//**********************************************************************
// CSV1行分データの特殊行属性種別を取得
// 
//	引数	なし
//	戻値	レコード（行）の種類
//**********************************************************************
//virtual
BYTE CimpUc000Common::GetOneLineFgFunc(void)
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳表のクラスで処理を実装する
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	return m_FgFunc;
}
// midori 156935 add <--

// midori 180404 add -->
//**********************************************************************
// CSVの1行分のデータをテーブルに登録
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
int CimpUc000Common::ImpCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳表のクラスで処理を実装する
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	return IMP_REC_OK;
}
// midori 180404 add <--

// midori 150954 add -->
//**********************************************************************
// 買掛金の欄外登録用
// CSVでインポートしなかった行のデータをテーブルに登録
//**********************************************************************
int CimpUc000Common::SetCsvNullLineData091(void)
{
	return IMP_REC_OK;
}
// midori 150954 add <--
// midori 150905 add -->
//**********************************************************************
// 人件費の内訳書用
// CSVでインポートしなかった行のデータをテーブルに登録
//**********************************************************************
int CimpUc000Common::SetCsvNullLineData142(void)
{
	return IMP_REC_OK;
}
// midori 150905 add <--

//**********************************************************************
// CSVの1行分のデータをテーブルから取得
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
int CimpUc000Common::GetCsvOneLineData(CStringArray* pstrCsvDataAry)
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各帳表のクラスで処理を実装する
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	return IMP_REC_OK;
}

//************************************************************
// 全データ取得
// 
//  引数
//			なし
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//************************************************************
int CimpUc000Common::GetAllData(void)
{
	// 頁番号/行で昇順番号して全レコード抽出
	if (m_pUcCmn->RequerySortPageRow() != DB_ERR_OK) {
		return IMP_ERRCODE_DB;
	}

	return IMP_ERRCODE_SUCCESS;
}

//************************************************************
//　一括集計金額データ取得
// 
//  引数
//			なし
// 
//	戻値
//			エラーコード（TRUE/FALSE）
//************************************************************
int CimpUc000Common::GetIkkatuData(void)
{
	// 一括集計金額データを取得する
	int nRetValue = m_pUcCmn->RequeryFgShowIkkatu();
	if (nRetValue != DB_ERR_OK) {
		return IMP_ERRCODE_DB;
	}

	return IMP_ERRCODE_SUCCESS;
}

//************************************************************
//　一括集計金額欄のデータをCSVへ設定する（エクスポート用）
// 
//  引数
//			なし
// 
//	戻値
//			なし
//************************************************************
void CimpUc000Common::SetIkkatuData(void)
{
	if ( m_mapIkkatuData == NULL ) {
		m_mapIkkatuData = new CMap<long, long, IKKATU_CSV_DATA, IKKATU_CSV_DATA>();
	}

	int nRet = 0;
	CStringArray csaValue;
	// CSVデータ格納用
	IKKATU_CSV_DATA udIkkatuData;

	int nSeq = 0;
	while ( (nRet = GetCsvOneLineData(&csaValue)) != IMP_REC_EOF ) {
		// エクスポート対象外の行？
		if ( nRet == IMP_REC_NG ) 
		{ 
			continue; 
		}

		// CSVデータを初期化する
		ZeroMemory(&udIkkatuData, sizeof(&udIkkatuData));

		// シーケンス番号を加算する
		nSeq++;

		// グループ番号をコピーする
		udIkkatuData.nGroup = m_nGroupNum;

		// 項目データをコピーする
		for(int i = 0; i < m_nItemCount; i++) {
			udIkkatuData.strItem[i] = csaValue.GetAt(i);
		}

		// CSVデータをコピーする
		m_mapIkkatuData->SetAt(nSeq, udIkkatuData);
	}
}

//************************************************************
//　一括集計金額欄のデータをCSVへ設定する（エクスポート用）
// 
//  引数
//			csvUcFile				[IN] CSVファイル
// 
//	戻値
//			エラーコード（TRUE/FALSE）
//************************************************************
//virtual
int CimpUc000Common::OutIkkatuData(CCsv* csvUcFile)
{
	// CSVデータを列挙する
	IKKATU_CSV_DATA udTemp;
	for(int i = 1; i <= m_mapIkkatuData->GetCount(); i++){
		ZeroMemory(&udTemp, sizeof(&udTemp));

		// CSVデータを取得する
		int nRet = m_mapIkkatuData->Lookup(i, udTemp);

		if ( nRet == FALSE ) {
			continue;
		}

		// 指定グループ番号でない場合
		if( udTemp.nGroup != m_nGroupNum ) {
			continue;
		}

		CStringArray csaValue;
		csaValue.RemoveAll();
		csaValue.SetSize(m_nItemCount);

		for (int j = 0; j < m_nItemCount; j++ ){
			csaValue.SetAt(j, udTemp.strItem[j]);
		}
		
		// CSVへデータを書き込む
		nRet = csvUcFile->CsvWrite(&csaValue);
		if( nRet == FALSE ) {
			return IMP_ERRCODE_EXPORT;
		}
	}

	return IMP_ERRCODE_SUCCESS;
}

//************************************************************
// 一括集計金額欄のデータを削除する
// 
//  引数
//			なし
// 
//	戻値
//			なし
//************************************************************
void CimpUc000Common::DeleteIkkatuData(void)
{
	if ( m_mapIkkatuData != NULL) {
		m_mapIkkatuData->RemoveAll();
		delete m_mapIkkatuData;
	}
}

//************************************************************
// 様式シーケンス取得
// 
//  引数
//			なし
// 
//	戻値
//			様式シーケンス
//************************************************************
int CimpUc000Common::GetFormSeq()
{
	return m_nFormSeq;
}

//**********************************************************************
// 全データ削除
// 
//	引数
//			なし
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
int CimpUc000Common::DeleteAllData(void)
{
// midori 152713 del -->
	//// テーブル内の全レコードを削除
	//if (m_pUcCmn->DeleteAllRecord() != DB_ERR_OK) {
	//	return IMP_ERRCODE_DB;
	//}
// midori 152713 del -->
// midori 152713 add -->
	// テーブル内の保管以外のレコードを削除
	if(m_pUcCmn->DeleteVisualRecord() != DB_ERR_OK)	{
		return IMP_ERRCODE_DB;
	}
// midori 152713 add -->

	return IMP_ERRCODE_SUCCESS;
}

//**********************************************************************
// 終了処理
// 
//	引数
//			なし
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
int CimpUc000Common::Fin(void)
{
	// 一括集計金額データを解放する
	DeleteIkkatuData();

	// テーブル接続終了（オブジェクト解放）
	if (m_pUcCmn != NULL) {
		m_pUcCmn->Fin();
		delete m_pUcCmn;
	}
	if (m_pUcInfSub != NULL) {
		m_pUcInfSub->Fin();
		delete m_pUcInfSub;
	}
	if (m_pUcListBank != NULL) {
		m_pUcListBank->Fin();
		delete m_pUcListBank;
	}
	if (m_pUcListAddress != NULL) {
		m_pUcListAddress->Fin();
		delete m_pUcListAddress;
	}
	if (m_pUcListKamoku != NULL) {
		m_pUcListKamoku->Fin();
		delete m_pUcListKamoku;
	}

	if(m_pUcInfFormType != NULL) {
		m_pUcInfFormType->Fin();
		delete m_pUcInfFormType;
	}

	return IMP_ERRCODE_SUCCESS;
}


//**********************************************************************
//（サブ関数）
// CSVの1行分のデータをテーブルに登録
//		※SetCsvOneLineData() 内の各帳票共通の部分を外出しにした
// 
//	引数
//			pstrCsvDataAry			[IN, OUT] CSVの1行分のデータ
//			pcFgFunc				[OUT]特殊行フラグ
// 
//	戻値
//			レコード（行）の種類（DBUchiImpDEF.h参照）
//**********************************************************************
// midori 156936 del -->
//int CimpUc000Common::SetCsvOneLineDataSub(CStringArray* pstrCsvDataAry, BYTE* pcFgFunc)
// midori 156936 del <--
// midori 156936 add -->
int CimpUc000Common::SetCsvOneLineDataSub(CStringArray* pstrCsvDataAry, BYTE* pcFgFunc, int pSw)
// midori 156936 add <--
{
	CString			strCsvId		= _T("");		// CSVの識別子
	int				nItemCount		= 0;			// 項目数
// 157766 add -->
	int				nMaxCount		= 0;			// 規定の項目数
// 157766 add <--
	BOOL			fIsData			= FALSE;		// TRUE=データあり, FALSE=データなし

// midori 150954 del -->
//	// インポートしようとした回数をカウント
//	m_nImportCount++;
// midori 150954 del <--

	//--------------------------------------------------
	// レコードチェック
	//--------------------------------------------------

// midori 150898 cor -->
//	// 項目数が正しいか？
//	nItemCount = (int)pstrCsvDataAry->GetSize();
//	if( m_KamokuVer != FALSE ){
//		m_nItemCount--;
//	}
//
////	if (nItemCount != m_nItemCount) {				// [2009/07/23 Fix]
//	if (nItemCount < m_nItemCount) {
//		return IMP_REC_NG;
//	}
//
//	// インポート対象レコードか？
//	strCsvId = pstrCsvDataAry->GetAt(0);
//	if (ConvCsvDataIdStrToDbVal(strCsvId, pcFgFunc) != TRUE) {
//		return IMP_REC_NG;
//	}
// -------------------------------
	// 先頭の識別子を先にチェックする
	// 棚卸資産の内訳書等の "[欄外]" の識別子がおかしなとき、"項目数が一致していません" のエラーになるので
	// 
	// インポート対象レコードか？
	strCsvId = pstrCsvDataAry->GetAt(0);
// midori 156936 del -->
	//if (ConvCsvDataIdStrToDbVal(strCsvId, pcFgFunc) != TRUE) {
// midori 156936 del <--
// midori 156936 add -->
	if (ConvCsvDataIdStrToDbVal(strCsvId, pcFgFunc, pSw) != TRUE) {
// midori 156936 add <--
		return IMP_REC_NG;
	}

// 157766 del -->
//	// 項目数が正しいか？
//	nItemCount = (int)pstrCsvDataAry->GetSize();
//	if( m_KamokuVer != FALSE ){
//		m_nItemCount--;
//	}
//// 210113_5301_1 add -->
//	if(m_NewVer == TRUE)	{
//		if(m_nFormSeq == ID_FORMNO_042 || m_nFormSeq == ID_FORMNO_111)	{
//			m_nItemCount = m_nItemCount - 2;
//		}
//		else if(m_nFormSeq == ID_FORMNO_131)	{
//			m_nItemCount = m_nItemCount - 1;
//		}
//	}
//// 210113_5301_1 add <--
//
//	if (nItemCount < m_nItemCount) {
//		m_uImpCsvInfo.nErr = 2;
//		return IMP_REC_NG;
//	}
// 157766 del <--
// 157766 add -->
	// 項目数が正しいかのチェックは行わない
	// その代わりに現在の行がタイトル行(3行目)の列数未満の場合は pstrCsvDataAry に空白を追加する
	// (メモ帳で編集した場合等に起こりえる)
	nMaxCount = m_nItemCount;
	if(m_KamokuVer == TRUE)	{
		if(m_nFormSeq == ID_FORMNO_021)	{
			nMaxCount = nMaxCount - 1;
		}
	}
// 登録番号（法人番号）_23/11/21 cor -->
	//if(m_NewVer == TRUE)	{
// -------------------------------------
	// 登録番号（法人番号）追加様式
	if(m_NewVer2 == TRUE)	{
		// ④-2だけはデフォルトが簡素化様式前の列数なので2列多い
		// 2列減らして登録番号（法人番号）の1列増やすので、-1
		switch(m_nFormSeq)	{
			case	ID_FORMNO_042:
				nMaxCount = nMaxCount - 1;
				break;
			case	ID_FORMNO_021:
			case	ID_FORMNO_031:
			case	ID_FORMNO_041:
			case	ID_FORMNO_071:
			case	ID_FORMNO_081:
			case	ID_FORMNO_091:
			case	ID_FORMNO_101:
			case	ID_FORMNO_121:
			case	ID_FORMNO_151:
			case	ID_FORMNO_152:
			case	ID_FORMNO_153:
			case	ID_FORMNO_161:
			case	ID_FORMNO_162:
			case	ID_FORMNO_171:
			case	ID_FORMNO_172:
			case	ID_FORMNO_173:
			case	ID_FORMNO_174:
			case	ID_FORMNO_175:
			case	ID_FORMNO_176:
			case	ID_FORMNO_177:
			case	ID_FORMNO_178:
			case	ID_FORMNO_179:
			case	ID_FORMNO_1710:
			case	ID_FORMNO_1711:
			case	ID_FORMNO_1712:
			case	ID_FORMNO_1713:
			case	ID_FORMNO_1714:
			case	ID_FORMNO_1715:
			case	ID_FORMNO_1716:
			case	ID_FORMNO_1717:
			case	ID_FORMNO_1718:
			case	ID_FORMNO_1719:
			case	ID_FORMNO_1720:
				nMaxCount = nMaxCount + 1;
				break;
		}
	}
	// 簡素化様式
	else if(m_NewVer == TRUE)	{
// 登録番号（法人番号）_23/11/21 cor <--
		if(m_nFormSeq == ID_FORMNO_042 || m_nFormSeq == ID_FORMNO_111)	{
			nMaxCount = nMaxCount - 2;
		}
		else if(m_nFormSeq == ID_FORMNO_131)	{
			nMaxCount = nMaxCount - 1;
		}
	}
	nItemCount = (int)pstrCsvDataAry->GetSize();
	for(int ii=nItemCount; ii<nMaxCount; ii++)	{
		pstrCsvDataAry->InsertAt(ii,_T(""));
	}
// 157766 add <--
// midori 150898 cor <--

	// 空行か？
	// CSV上で、「"[明細]","","","",""」（データがない）となっていた場合、
	// その行は空行とみなす。
	if ((strCsvId.Compare(CSV_ID_DATA)        == 0) ||
		(strCsvId.Compare(CSV_ID_UCHIWAKE)    == 0) ||
		(strCsvId.Compare(CSV_ID_HAITOKIN)    == 0) ||
		(strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0)) {
		// データがあるか？
		for (int i = 0; i <nItemCount; i ++) {
			// 先頭は項目名称が格納されているはずなので無視
			if (i == 0) {
				continue;
			}
			// 1項目でもデータがあればデータ行とみなす
			if (pstrCsvDataAry->GetAt(i).GetLength() > 0){
				fIsData = TRUE;
				break;
			}
		}

		// データが1項目もない（空行）
		if (fIsData != TRUE) {
			*pcFgFunc = ID_FGFUNC_NULL;
		}
	}

	return IMP_REC_OK;
}

//**********************************************************************
//（サブ関数）
// CSVの1行分のデータをテーブルから取得
//		※GetCsvOneLineData() 内の各帳票共通の部分を外出しにした
// 
//	引数
//			pstrCsvDataAry			[IN, OUT] CSVの1行分のデータ
//			pstrCsvId				[IN, OUT] CSVの識別子
// 
//	戻値
//			レコード（行）の種類（DBUchiImpDEF.h参照）
//**********************************************************************
int CimpUc000Common::GetCsvOneLineDataSub(CStringArray* pstrCsvDataAry, CString* pstrCsvId)
{
	// CSV 1行バッファをクリア
	pstrCsvDataAry->RemoveAll();

	// バッファサイズを指定
	pstrCsvDataAry->SetSize(m_nItemCount);

	//--------------------------------------------------
	// レコードチェック
	//--------------------------------------------------
	// EOFか？
	if (m_pUcCmn->IsEOF() == TRUE) {
		return IMP_REC_EOF;
	}

	// グループ番号を保持する
	m_nGroupNum = m_pUcCmn->m_NumGroup;

	// 一括集計金額レコード？
	if (m_pUcCmn->m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
		// 次のレコードに移動しておく
		m_pUcCmn->MoveNext();

		return IMP_REC_IKKATU;
	}

	// 戻り値
	int nRetValue = IMP_REC_OK;

	// エクスポート対象レコードか？
	if (ConvCsvDataIdDbValToStr(m_pUcCmn->m_FgFunc, m_pUcCmn->m_FgShow, pstrCsvId) != TRUE) {
		// 非表示行の場合
		if (m_pUcCmn->m_FgShow == ID_FGSHOW_IKKATU ) {
			nRetValue = IMP_REC_HIDE;
		}
		else {
			nRetValue = IMP_REC_NG;
		}

		// 次のレコードに移動しておく
		m_pUcCmn->MoveNext();

		return nRetValue;
	}

	// 非表示行の場合
	if (m_pUcCmn->m_FgShow == ID_FGSHOW_IKKATU ) {
		nRetValue = IMP_REC_HIDE;
	}

	return nRetValue;
}

//**********************************************************************
// 共通で使用するテーブルとの接続を開始
// 
//	引数
//			pDB						[IN] DBポインタ
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
void CimpUc000Common::ConnectCmnTables(CDatabase* pDB)
{
	// テーブル接続開始（オブジェクト生成）
	m_pUcInfSub			= new CdbUcInfSub(pDB);
	m_pUcListBank		= new CdbUcLstBank(pDB);
	m_pUcListAddress	= new CdbUcLstAddress(pDB);
	m_pUcListKamoku		= new CdbUcLstKamoku(pDB);
	m_pUcInfFormType	= new CdbUcInfFormType(pDB);
}

//**********************************************************************
// CSVの1行分のデータをテーブルから取得
//		※GetCsvOneLineData() 内の各帳票共通の部分を外出しにした
// 
//	引数
//			nFormSeq				[IN] 様式シーケンス（ID_FORMNO_XXX）
//			pstrSyokeiName			[OUT]小計文言
//			pstrChukeiName			[OUT]中計文言
// 
//	戻値
//			レコード（行）の種類（DBUchiImpDEF.h参照）
//**********************************************************************
void CimpUc000Common::GetKeiStr(int nFormSeq, CString* pstrSyokeiName, CString* pstrChukeiName)
{
	// 指定した帳票の情報を取得
	m_pUcInfSub->Init(nFormSeq);

	// 小計文言
	pstrSyokeiName->Format(	_T("%s%s%s"), 
							BRACKET_KIND[m_pUcInfSub->m_OpSykeiKakko].szLeft,
							m_pUcInfSub->m_OpSykeiName,
							BRACKET_KIND[m_pUcInfSub->m_OpSykeiKakko].szRight);

	// 中計文言
	pstrChukeiName->Format(	_T("%s%s%s"), 
							BRACKET_KIND[m_pUcInfSub->m_OpCykeiKakko].szLeft,
							m_pUcInfSub->m_OpCykeiName,
							BRACKET_KIND[m_pUcInfSub->m_OpCykeiKakko].szRight);
}



//**********************************************************************
// 金融機関名称テーブルに名称を登録する
// 
//	引数
//			strBkName1				[IN] 金融機関名
//			strBkName2				[IN] 支店名
//			pnBkSeq					[OUT]金融機関シーケンス番号
//			pnBkOrder				[OUT]金融機関並び順
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::CheckEntryBank(CString strBkName1, CString strBkName2, long* pnBkSeq, long* pnBkOrder)
{
	// 自動登録設定なしなら、何もせず帰る
	if(m_RegAutoRef == FALSE)	return TRUE;

	long			nSeq			= 0;				// シーケンス番号
	long			nOrder			= 0;				// 並び順

	// 名称テーブルに登録
	// シーケンス番号を取得
	nSeq = m_pUcListBank->CheckEntry(strBkName1, strBkName2);
	if (nSeq == 0) {
		return TRUE;		// NULL文字列なので登録しない（正常終了）
	}
	if (nSeq < 0) {
		return FALSE;		// 登録失敗
	}


	// 並び順を取得
	if (m_pUcListBank->RequerySeq(nSeq) != DB_ERR_OK) {
		return FALSE;
	}
	nOrder = m_pUcListBank->m_OrderNum;

	// シーケンス番号、並び順を返送
	*pnBkSeq		= nSeq;
	*pnBkOrder		= nOrder;

	return TRUE;
}

//**********************************************************************
// 取引先名称テーブルに名称を登録する
//		・本関数の呼び出し方法により、動作が異なる。
//			(1) 住所の指定なし（帳票②、⑧で使用）
//			(2) 住所の指定あり
// 
//	引数
//			strAdName1				[IN] 取引先名称１ 
//			strAdName2				[IN] 取引先名称２ 
//			strAdAdd1				[IN] 取引先住所１
//			strAdAdd2				[IN] 取引先住所２
//			nGrSeq					[IN] グループ番号
//			pnAdSeq					[OUT]取引先シーケンス番号
//			pnAdOrder				[OUT]取引先並び順
//			strInvNo				[IN] 登録番号（法人番号）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
// (1) 住所の指定なし（帳票②、⑧で使用）
// 登録番号（法人番号）_23/11/21 cor -->
//BOOL CimpUc000Common::CheckEntryAddress(CString strAdName1, CString strAdName2, int nGrSeq, long* pnAdSeq, long* pnAdOrder)
// -------------------------------------
BOOL CimpUc000Common::CheckEntryAddress(CString strAdName1, CString strAdName2, int nGrSeq, long* pnAdSeq, long* pnAdOrder, CString strInvNo/*=NULL*/)
// 登録番号（法人番号）_23/11/21 cor <--
{
	// 自動登録設定なしなら、何もせず帰る
	if(m_RegAutoRef == FALSE)	return TRUE;

	long			nSeq			= 0;				// シーケンス番号
	long			nOrder			= 0;				// 並び順

	// 名称テーブルに登録
	// シーケンス番号を取得
// 登録番号（法人番号）_23/11/21 cor -->
	//nSeq = m_pUcListAddress->CheckEntryAddNone(strAdName1, strAdName2, _T(""), _T(""), nGrSeq, FALSE, TRUE);
// -------------------------------------
	if(strInvNo.IsEmpty() == TRUE)	{
		nSeq = m_pUcListAddress->CheckEntryAddNone(strAdName1, strAdName2, _T(""), _T(""), nGrSeq, FALSE, TRUE);
	}
	else	{
		nSeq = m_pUcListAddress->CheckEntryAddNone_Inv(strAdName1, strAdName2, _T(""), _T(""), strInvNo, nGrSeq, FALSE, TRUE);
	}
// 登録番号（法人番号）_23/11/21 cor <--
	if (nSeq == 0) {
		return TRUE;		// NULL文字列なので登録しない（正常終了）
	}
	if (nSeq < 0) {
		return FALSE;		// 登録失敗
	}

	// 並び順を取得
	if (CheckEntryAddressSub(nGrSeq, nSeq, &nOrder) != TRUE) {
		return FALSE;
	}

	// シーケンス番号、並び順を返送
	*pnAdSeq		= nSeq;
	*pnAdOrder		= nOrder;

	return TRUE;
}
// (2) 住所の指定あり
// 登録番号（法人番号）_23/11/21 cor -->
//BOOL CimpUc000Common::CheckEntryAddress(CString strAdName1, CString strAdName2, CString strAdAdd1, CString strAdAdd2, int nGrSeq, long* pnAdSeq, long* pnAdOrder)
// -------------------------------------
BOOL CimpUc000Common::CheckEntryAddress(CString strAdName1, CString strAdName2, CString strAdAdd1, CString strAdAdd2, int nGrSeq, long* pnAdSeq, long* pnAdOrder, CString strInvNo/*=NULL*/)
// 登録番号（法人番号）_23/11/21 cor <--
{
	// 自動登録設定なしなら、何もせず帰る
	if(m_RegAutoRef == FALSE)	return TRUE;

	long			nSeq			= 0;				// シーケンス番号
	long			nOrder			= 0;				// 並び順

	// 名称テーブルに登録
	// シーケンス番号を取得
// 登録番号（法人番号）_23/11/21 cor -->
	//nSeq = m_pUcListAddress->CheckEntry(strAdName1, strAdName2, strAdAdd1, strAdAdd2, nGrSeq, TRUE);
// -------------------------------------
	if(strInvNo.IsEmpty() == TRUE)	{
		nSeq = m_pUcListAddress->CheckEntry(strAdName1, strAdName2, strAdAdd1, strAdAdd2, nGrSeq, TRUE);
	}
	else	{
		nSeq = m_pUcListAddress->CheckEntry_Inv(strAdName1, strAdName2, strAdAdd1, strAdAdd2, strInvNo, nGrSeq, TRUE );
	}
// 登録番号（法人番号）_23/11/21 cor <--
	if (nSeq == 0) {
		return TRUE;		// NULL文字列なので登録しない（正常終了）
	}
	if (nSeq < 0) {
		return FALSE;		// 登録失敗
	}

	// 並び順を取得
	if (CheckEntryAddressSub(nGrSeq, nSeq, &nOrder) != TRUE) {
		return FALSE;
	}

	// シーケンス番号、並び順を返送
	*pnAdSeq		= nSeq;
	*pnAdOrder		= nOrder;

	return TRUE;
}
//（サブ関数）
//		・CheckEntryAddress()で使用。
//		・本関数を、直接呼び出すことは禁止。
BOOL CimpUc000Common::CheckEntryAddressSub(int nGrSeq, long nAdSeq, long* pnAdOrder)
{
	long			nOrder			= 0;				// 並び順

	// 並び順を取得
	if (m_pUcListAddress->RequeryAdSeq(nGrSeq, nAdSeq) != DB_ERR_OK) {
		return FALSE;
	}
	nOrder = m_pUcListAddress->m_OrderNum;

	// 並び順を返送
	*pnAdOrder		= nOrder;

	return TRUE;
}

//**********************************************************************
// 科目名称テーブルに名称を登録する
// 
//	引数
//			strKnName				[IN] 科目名
//			nFormSeq				[IN] 様式シーケンス
//			pnKnSeq					[OUT]科目シーケンス番号
//			pnKnOrder				[OUT]科目並び順
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::CheckEntryKamoku(CString strKnName, int nFormSeq, long* pnKnSeq, long* pnKnOrder)
{
	long			nSeq			= 0;				// シーケンス番号
	long			nOrder			= 0;				// 並び順
// midori 152200 add -->
	CString				cst=_T("");							// CString型バッファ
	CFuriganaConverter	fc;
// midori 152200 add <--

	// 名称テーブルに登録
	// シーケンス番号を取得
// midori 152200 del -->
//	nSeq = m_pUcListKamoku->CheckEntry(strKnName, m_nFormSeq);
// midori 152200 del <--
// midori 152200 add -->
	if(strKnName.IsEmpty() == FALSE)	{
		cst = fc.Convert(strKnName);
	}
	nSeq = m_pUcListKamoku->CheckEntry(strKnName, m_nFormSeq,cst);
// midori 152200 add <--
	if (nSeq == 0) {
		return TRUE;		// NULL文字列なので登録しない（正常終了）
	}
	if (nSeq < 0) {
		return FALSE;		// 登録失敗
	}

	// 並び順を取得
	if (m_pUcListKamoku->RequeryKnSeq(m_nFormSeq, nSeq) != DB_ERR_OK) {
		return FALSE;
	}
	nOrder = m_pUcListKamoku->m_KnOrder;

	// シーケンス番号、並び順を返送
	*pnKnSeq		= nSeq;
	*pnKnOrder		= nOrder;

	return TRUE;
}

//-------------------------------------------
// カンマ、スペースを除く　2011.07.15 h.ukawa
//-------------------------------------------
BOOL RemoveCommaImportNum(CString &strVal )
{
	int	i, ln;

	ln = strVal.GetLength();


	for( i = 0; i < ln; ) {
		if( strVal[i] == 0 ) break;
		else if( strVal[i] == ',' || strVal[i] == ' ' ) {
			strVal.Delete(i,1);
			ln--;
		}
		else i++;
	}

	return TRUE;
}

//**********************************************************************
// インポート数値チェック
//		・以下のチェックを行う。
//			(0) 正/負の判断
//			(1) 文字列に数値以外が含まれていないか
//			(2) 数値が有効桁数内か
// 
//	引数
//			strVal					[IN] 数値文字列
//			nDigit					[IN] 有効桁数
//			fMinus					[IN] 負の値の対応
//											TRUE ：許可する
//											FALSE：許可しない
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
//BOOL CimpUc000Common::CheckImportNum(CString strVal, int nDigit)
BOOL CimpUc000Common::CheckImportNum(CString strVal, int nDigit, BOOL fMinus)	// [2009/07/09 Minus Value Support]
{
	//int	i;

	//--------------------------------------------------
	// (0) 文字列先頭が'-'(マイナス)か						[2009/07/09 Add for Minus Value Support]
	//--------------------------------------------------
	if ( (fMinus == TRUE) && (strVal[0] == '-') ) {		// 文字列先頭がマイナス記号
		nDigit--;										// 有効桁数を-1
		strVal.Delete(0, 1);							// マイナス記号をカット
	}

	//--------------------------------------------------
	// (1) 文字列に数値以外が含まれていないか
	//--------------------------------------------------
	// スペースも数値以外

	CString		strTemp			= 0;		// 部分文字列

	// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
	strTemp = strVal.SpanIncluding(_T("0123456789"));

	// 元の文字列と抽出した部分文字列を比較
	// 一致しなければ数位以外の文字列があったことになる
	if (strTemp.Compare(strVal) != 0) {
		strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
		return FALSE;
	}


	//--------------------------------------------------
	// (2) 数値が有効桁数内か
	//--------------------------------------------------
	// 例えば文字列=“0123”で、有効桁数が3桁の場合、エラーとみなす

	// バイト数チェック
	if (CheckImportStr(strVal, nDigit) != TRUE) {
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************
// インポート数値チェック  カンマremoveバージョン
//		・以下のチェックを行う。
//			(0) 正/負の判断
//			(1) 文字列に数値以外が含まれていないか
//			(2) 数値が有効桁数内か
// 
//	引数
//			strVal					[IN] 数値文字列
//			nDigit					[IN] 有効桁数
//			fMinus					[IN] 負の値の対応
//											TRUE ：許可する
//											FALSE：許可しない
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
//BOOL CimpUc000Common::CheckImportNum(CString strVal, int nDigit)
BOOL CimpUc000Common::CheckImportNum2(CString strVal, int nDigit, BOOL fMinus)	// [2009/07/09 Minus Value Support]
{
	//int	i;

	//--------------------------------------------------
	// (0) 文字列先頭が'-'(マイナス)か						[2009/07/09 Add for Minus Value Support]
	//--------------------------------------------------
	if ( (fMinus == TRUE) && (strVal[0] == '-') ) {		// 文字列先頭がマイナス記号
		nDigit--;										// 有効桁数を-1
		strVal.Delete(0, 1);							// マイナス記号をカット
	}


	RemoveCommaImportNum( strVal );				// カンマを除く　2011.07.15 h.ukawa

	//--------------------------------------------------
	// (1) 文字列に数値以外が含まれていないか
	//--------------------------------------------------
	// スペースも数値以外

	CString		strTemp			= 0;		// 部分文字列

	// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
	strTemp = strVal.SpanIncluding(_T("0123456789"));

	// 元の文字列と抽出した部分文字列を比較
	// 一致しなければ数位以外の文字列があったことになる
	if (strTemp.Compare(strVal) != 0) {
		strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
		return FALSE;
	}


	//--------------------------------------------------
	// (2) 数値が有効桁数内か
	//--------------------------------------------------
	// 例えば文字列=“0123”で、有効桁数が3桁の場合、エラーとみなす

	// バイト数チェック
	if (CheckImportStr(strVal, nDigit) != TRUE) {
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************
// インポート数値チェック
//		・整数部および小数部について以下のチェックを行う。
//			(1) 文字列に数値以外が含まれていないか
//			(2) 数値が有効桁数内か
// 
//	引数
//			strVal					[IN] 数値文字列
//			nIntegerDigit			[IN] 整数部有効桁数
//			nDecimalDigit			[IN] 小数部有効桁数
//			fMinus					[IN] 負の値の対応
//											TRUE ：許可する
//											FALSE：許可しない
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
//BOOL CimpUc000Common::CheckImportNum(CString strVal, int nIntegerDigit, int nDecimalDigit)
// midori 150901 cor -->
//BOOL CimpUc000Common::CheckImportNum(CString strVal, int nIntegerDigit, int nDecimalDigit, BOOL fMinus)		// [2009/07/09 Minus Value Support]
//{
//	//--------------------------------------------------
//	// 小数点(ピリオド)までの文字列を取得する
//	//--------------------------------------------------
//	int nPriodIndex = strVal.Find('.');
//
//	//--------------------------------------------------
//	// 小数点有無を判断する
//	//--------------------------------------------------
//	if (nPriodIndex != -1) {
//		// 小数点が含まれ場合
//
//		//--------------------------------------------------
//		// 1.整数部のチェックを行う
//		//--------------------------------------------------
//		// 整数部の文字列を取得する(小数点を除く部分先頭からの文字列)
//		CString strInteger = 0;
//		strInteger = strVal.Left(nPriodIndex);
//
//		//--------------------------------------------------
//		// 1-0.文字列先頭が'-'(マイナス)か						[2009/07/09 Add for Minus Value Support]
//		//--------------------------------------------------
//		if ( (fMinus == TRUE) && (strVal[0] == '-') ) {		// 文字列先頭がマイナス記号
//			nIntegerDigit--;								// 有効桁数を-1
//			strInteger.Delete(0, 1);						// マイナス記号をカット
//		}
//
//		//--------------------------------------------------
//		// 1-1.文字列に数値以外が含まれていないか
//		//--------------------------------------------------
//		// “数値以外”は、スペースも含む
//		// チェック用
//		CString strTemp = 0;
//		
//		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
//		strTemp = strInteger.SpanIncluding(_T("0123456789"));
//
//		// 元の文字列と抽出した部分文字列を比較
//		// 一致しなければ数位以外の文字列があったことになる
//		if (strTemp.Compare(strInteger) != 0) {
//			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
//			return FALSE;
//		}
//
//		//--------------------------------------------------
//		// 1-2.数値が有効桁数内か
//		//--------------------------------------------------
//		// 例えば文字列=“0123”で、有効桁数が3桁の場合、エラーとみなす
//
//		// バイト数チェック
//		if (CheckImportStr(strTemp, nIntegerDigit) != TRUE) {
//			return FALSE;
//		}
//		
//		//--------------------------------------------------
//		// 2.小数部のチェックを行う
//		//--------------------------------------------------
//		// 小数部文字列を取得する(小数点を除く部分、小数点以降の文字列)
//		CString strDecimal = 0;
//		strDecimal = strVal.Mid(nPriodIndex + 1);
//
//		//--------------------------------------------------
//		// 2-1.文字列に数値以外が含まれていないか
//		//--------------------------------------------------
//		// “数値以外”は、スペースも含む
//		// チェック用
//		strTemp = "";
//		
//		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
//		strTemp = strDecimal.SpanIncluding(_T("0123456789"));
//
//		// 元の文字列と抽出した部分文字列を比較
//		// 一致しなければ数位以外の文字列があったことになる
//		if (strTemp.Compare(strDecimal) != 0) {
//			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
//			return FALSE;
//		}
//
//		//--------------------------------------------------
//		// 2-2.数値が有効桁数内か
//		//--------------------------------------------------
//		// 例えば文字列=“0123”で、有効桁数が3桁の場合、エラーとみなす
//
//		// バイト数チェック
//		if (CheckImportStr(strTemp, nDecimalDigit) != TRUE) {
//			return FALSE;
//		}
//
//	}
//	else {
//		// 小数点が含まれない場合
////		return CheckImportNum(strVal, nIntegerDigit);
//		return CheckImportNum(strVal, nIntegerDigit, fMinus);	// [2009/07/10 Minus Value Support]
//
//	}
//
//	return TRUE;
//}
// ---------------------
// 整数・小数点以下のどちらでエラーが発生したかを把握するためチェックの順序を変更しました
// old) 整数文字種 → 整数桁数 → 小数点以下文字種 → 小数点以下桁数
// new) 整数文字種 → 小数点以下文字種 → 整数桁数 → 小数点以下桁数
BOOL CimpUc000Common::CheckImportNum(CString strVal, int nIntegerDigit, int nDecimalDigit, BOOL fMinus)		// [2009/07/09 Minus Value Support]
{
	BOOL		brv1,brv2;
	CString		strTemp;
	CString		strInteger;
	CString		strDecimal;

	//--------------------------------------------------
	// 小数点(ピリオド)までの文字列を取得する
	//--------------------------------------------------
	int nPriodIndex = strVal.Find('.');

	//--------------------------------------------------
	// 小数点有無を判断する
	//--------------------------------------------------
	if (nPriodIndex != -1) {
		// 小数点が含まれる場合

		//--------------------------------------------------
		// 1.文字列に数値以外が含まれていないか
		//--------------------------------------------------

		//--------------------------------------------------
		// 1-0.整数部のチェックを行う
		//--------------------------------------------------
		// 整数部の文字列を取得する(小数点を除く部分先頭からの文字列)
		strInteger = strVal.Left(nPriodIndex);

		// 文字列先頭が'-'(マイナス)か
		if ( (fMinus == TRUE) && (strVal[0] == '-') ) {		// 文字列先頭がマイナス記号
			nIntegerDigit--;								// 有効桁数を-1
			strInteger.Delete(0, 1);						// マイナス記号をカット
		}

		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
		strTemp = strInteger.SpanIncluding(_T("0123456789"));

		// 元の文字列と抽出した部分文字列を比較
		// 一致しなければ数位以外の文字列があったことになる
		if (strTemp.Compare(strInteger) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
			return FALSE;
		}

		//--------------------------------------------------
		// 1-1.小数部のチェックを行う
		//--------------------------------------------------
		// 小数部文字列を取得する(小数点を除く部分、小数点以降の文字列)
		strDecimal = strVal.Mid(nPriodIndex + 1);

		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
		strTemp = strDecimal.SpanIncluding(_T("0123456789"));

		// 元の文字列と抽出した部分文字列を比較
		// 一致しなければ数位以外の文字列があったことになる
		if (strTemp.Compare(strDecimal) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
			return FALSE;
		}

		//--------------------------------------------------
		// 2.数値が有効桁数内か
		//--------------------------------------------------

		//--------------------------------------------------
		// 2-0.整数部のチェックを行う
		//--------------------------------------------------
		brv1 = CheckImportStr(strInteger, nIntegerDigit);

		//--------------------------------------------------
		// 2-1.小数部のチェックを行う
		//--------------------------------------------------
		brv2 = CheckImportStr(strDecimal, nDecimalDigit);

		if(brv1 != TRUE || brv2 != TRUE)	{
			if(brv1 != TRUE && brv2 != TRUE)	m_uImpCsvInfo.nType2 = 2;	// 整数、小数点以下ともにエラー
			else if(brv1 != TRUE)				m_uImpCsvInfo.nType2 = 0;	// 整数のみエラー
			else								m_uImpCsvInfo.nType2 = 1;	// 小数点以下のみエラー
			return FALSE;
		}
	}
	else {
		// 小数点が含まれない場合
		return CheckImportNum(strVal, nIntegerDigit, fMinus);	// [2009/07/10 Minus Value Support]

	}

	return TRUE;
}
// midori 150901 cor <--

//**********************************************************************
// インポート数値チェック  connma remove 
//		・整数部および小数部について以下のチェックを行う。
//			(1) 文字列に数値以外が含まれていないか
//			(2) 数値が有効桁数内か
// 
//	引数
//			strVal					[IN] 数値文字列
//			nIntegerDigit			[IN] 整数部有効桁数
//			nDecimalDigit			[IN] 小数部有効桁数
//			fMinus					[IN] 負の値の対応
//											TRUE ：許可する
//											FALSE：許可しない
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
//BOOL CimpUc000Common::CheckImportNum(CString strVal, int nIntegerDigit, int nDecimalDigit)
// midori 150901 cor -->
//BOOL CimpUc000Common::CheckImportNum2(CString strVal, int nIntegerDigit, int nDecimalDigit, BOOL fMinus)		// [2009/07/09 Minus Value Support]
//{
//	RemoveCommaImportNum( strVal );				// カンマを除く　2011.07.15 h.ukawa
//
//	//--------------------------------------------------
//	// 小数点(ピリオド)までの文字列を取得する
//	//--------------------------------------------------
//	int nPriodIndex = strVal.Find('.');
//
//	//--------------------------------------------------
//	// 小数点有無を判断する
//	//--------------------------------------------------
//	if (nPriodIndex != -1) {
//		// 小数点が含まれ場合
//
//		//--------------------------------------------------
//		// 1.整数部のチェックを行う
//		//--------------------------------------------------
//		// 整数部の文字列を取得する(小数点を除く部分先頭からの文字列)
//		CString strInteger = 0;
//		strInteger = strVal.Left(nPriodIndex);
//
//		//--------------------------------------------------
//		// 1-0.文字列先頭が'-'(マイナス)か						[2009/07/09 Add for Minus Value Support]
//		//--------------------------------------------------
//		if ( (fMinus == TRUE) && (strVal[0] == '-') ) {		// 文字列先頭がマイナス記号
//			nIntegerDigit--;								// 有効桁数を-1
//			strInteger.Delete(0, 1);						// マイナス記号をカット
//		}
//
//		//--------------------------------------------------
//		// 1-1.文字列に数値以外が含まれていないか
//		//--------------------------------------------------
//		// “数値以外”は、スペースも含む
//		// チェック用
//		CString strTemp = 0;
//		
//		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
//		strTemp = strInteger.SpanIncluding(_T("0123456789"));
//
//		// 元の文字列と抽出した部分文字列を比較
//		// 一致しなければ数位以外の文字列があったことになる
//		if (strTemp.Compare(strInteger) != 0) {
//			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
//			return FALSE;
//		}
//
//		//--------------------------------------------------
//		// 1-2.数値が有効桁数内か
//		//--------------------------------------------------
//		// 例えば文字列=“0123”で、有効桁数が3桁の場合、エラーとみなす
//
//		// バイト数チェック
//		if (CheckImportStr(strTemp, nIntegerDigit) != TRUE) {
//			return FALSE;
//		}
//		
//		//--------------------------------------------------
//		// 2.小数部のチェックを行う
//		//--------------------------------------------------
//		// 小数部文字列を取得する(小数点を除く部分、小数点以降の文字列)
//		CString strDecimal = 0;
//		strDecimal = strVal.Mid(nPriodIndex + 1);
//
//		//--------------------------------------------------
//		// 2-1.文字列に数値以外が含まれていないか
//		//--------------------------------------------------
//		// “数値以外”は、スペースも含む
//		// チェック用
//		strTemp = "";
//		
//		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
//		strTemp = strDecimal.SpanIncluding(_T("0123456789"));
//
//		// 元の文字列と抽出した部分文字列を比較
//		// 一致しなければ数位以外の文字列があったことになる
//		if (strTemp.Compare(strDecimal) != 0) {
//			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
//			return FALSE;
//		}
//
//		//--------------------------------------------------
//		// 2-2.数値が有効桁数内か
//		//--------------------------------------------------
//		// 例えば文字列=“0123”で、有効桁数が3桁の場合、エラーとみなす
//
//		// バイト数チェック
//		if (CheckImportStr(strTemp, nDecimalDigit) != TRUE) {
//			return FALSE;
//		}
//
//	}
//	else {
//		// 小数点が含まれない場合
////		return CheckImportNum(strVal, nIntegerDigit);
//		return CheckImportNum(strVal, nIntegerDigit, fMinus);	// [2009/07/10 Minus Value Support]
//
//	}
//
//	return TRUE;
//}
// ---------------------
// 整数・小数点以下のどちらでエラーが発生したかを把握するためチェックの順序を変更しました
// old) 整数文字種 → 整数桁数 → 小数点以下文字種 → 小数点以下桁数
// new) 整数文字種 → 小数点以下文字種 → 整数桁数 → 小数点以下桁数
BOOL CimpUc000Common::CheckImportNum2(CString strVal, int nIntegerDigit, int nDecimalDigit, BOOL fMinus)		// [2009/07/09 Minus Value Support]
{
	BOOL		brv1,brv2;
	CString		strTemp;
	CString		strInteger;
	CString		strDecimal;

	RemoveCommaImportNum( strVal );				// カンマを除く　2011.07.15 h.ukawa

	//--------------------------------------------------
	// 小数点(ピリオド)までの文字列を取得する
	//--------------------------------------------------
	int nPriodIndex = strVal.Find('.');

	//--------------------------------------------------
	// 小数点有無を判断する
	//--------------------------------------------------
	if (nPriodIndex != -1) {
		// 小数点が含まれ場合

		//--------------------------------------------------
		// 1.文字列に数値以外が含まれていないか
		//--------------------------------------------------

		//--------------------------------------------------
		// 1-0.整数部のチェックを行う
		//--------------------------------------------------
		// 整数部の文字列を取得する(小数点を除く部分先頭からの文字列)
		strInteger = strVal.Left(nPriodIndex);

		// 文字列先頭が'-'(マイナス)か
		if ( (fMinus == TRUE) && (strVal[0] == '-') ) {		// 文字列先頭がマイナス記号
			nIntegerDigit--;								// 有効桁数を-1
			strInteger.Delete(0, 1);						// マイナス記号をカット
		}

		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
		strTemp = strInteger.SpanIncluding(_T("0123456789"));

		// 元の文字列と抽出した部分文字列を比較
		// 一致しなければ数位以外の文字列があったことになる
		if (strTemp.Compare(strInteger) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
			return FALSE;
		}

		//--------------------------------------------------
		// 1-1.小数部のチェックを行う
		//--------------------------------------------------
		// 小数部文字列を取得する(小数点を除く部分、小数点以降の文字列)
		strDecimal = strVal.Mid(nPriodIndex + 1);

		// 文字列の最初から、数値以外の文字が出現するまでの部分文字列を抽出
		strTemp = strDecimal.SpanIncluding(_T("0123456789"));

		// 元の文字列と抽出した部分文字列を比較
		// 一致しなければ数位以外の文字列があったことになる
		if (strTemp.Compare(strDecimal) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_NUM);
			return FALSE;
		}

		//--------------------------------------------------
		// 2.数値が有効桁数内か
		//--------------------------------------------------

		//--------------------------------------------------
		// 2-0.整数部のチェックを行う
		//--------------------------------------------------
		brv1 = CheckImportStr(strInteger, nIntegerDigit);

		//--------------------------------------------------
		// 2-1.小数部のチェックを行う
		//--------------------------------------------------
		brv2 = CheckImportStr(strDecimal, nDecimalDigit);

		if(brv1 != TRUE || brv2 != TRUE)	{
			if(brv1 != TRUE && brv2 != TRUE)	m_uImpCsvInfo.nType2 = 2;	// 整数、小数点以下ともにエラー
			else if(brv1 != TRUE)				m_uImpCsvInfo.nType2 = 0;	// 整数のみエラー
			else								m_uImpCsvInfo.nType2 = 1;	// 小数点以下のみエラー
			return FALSE;
		}
	}
	else {
		// 小数点が含まれない場合
		return CheckImportNum(strVal, nIntegerDigit, fMinus);	// [2009/07/10 Minus Value Support]

	}

	return TRUE;
}
// midori 150901 cor <--

//**********************************************************************
// インポート文字列チェック
// 
//	引数
//		・以下のチェックを行う。
//			(1) 文字列の長さが有効バイト数内か
//			strVal					[IN] 数値文字列
//			nSize					[IN] 有効バイト数
//			fCheckHankaku			[IN] 半角チェック(TRUE:すべて半角)
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::CheckImportStr(CString strVal, int nSize, BOOL fCheckHankaku /* = FALSE */)
{
	//--------------------------------------------------
	// (1) 文字列が全て半角文字か
	//--------------------------------------------------
	if(fCheckHankaku == TRUE) {
		// 文字列全てが半角文字かを判断する
		if (CheckStrIsHankaku(strVal) != TRUE) {
			return FALSE;
		}
	}

	//--------------------------------------------------
	// (2) 文字列の長さが有効バイト数内か
	//--------------------------------------------------

	// バイト数チェック
	if (CheckStrLength(strVal, nSize) != TRUE) {
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************
//（サブ関数）
// 文字列の長さ（バイト数）が有効範囲内かをチェック
// 
//	引数
//			strVal					[IN] 数値文字列
//			nSize					[IN] 有効バイト数
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::CheckStrLength(CString strVal, int nSize)
{
	int			nLength			= strVal.GetLength();	// 文字列のバイト数

	// バイト数？
	if (nLength > nSize) {
		strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_OVER_LENGTH);
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************
//（サブ関数）
// 文字列がすべて半角文字かをチェック
// 
//	引数
//			strVal					[IN] 文字列
// 
//	戻値
//			TRUE ：成功(すべて半角文字)
//			FALSE：失敗(半角文字以外が含まれる)
//**********************************************************************
int CimpUc000Common::CheckStrIsHankaku(CString strVal)
{
	TCHAR szTemp[64];
	_tcscpy_s (szTemp, sizeof(szTemp), strVal);

	char*	p = szTemp;
	int		nLength = (int)strlen(p);

	for( int i = 0; i < nLength; i++){
		//	指定された文字が先行バイトである場合、つまり全角の場合
		if( IsDBCSLeadByte( (BYTE)p[i]) != 0){
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_HANKAKU);
			return FALSE;
		}
	}

	return TRUE;
}

// 登録番号（法人番号）_23/11/21 add -->
//**********************************************************************
// インポート文字列チェック（登録番号（法人番号））
// 
//	引数
//			strVal					[IN/OUT] 文字列
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::CheckImportTno( CString& strVal )
{
	CString	strTemp=_T("");
	CString	chkVal=_T("");

	strVal.TrimLeft();
	if(strVal.IsEmpty() == TRUE)	{
		return(TRUE);
	}

	// ----------------------------------------------------------------------------------
	// ① T または数字以外の文字が含まれていないかのチェック
	// ----------------------------------------------------------------------------------
	// 文字列全てが半角文字かを判断する
	if(CheckStrIsHankaku(strVal) != TRUE)	{
		strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_THNO_ERR3);
		return(FALSE);
	}

	// 小文字を大文字に変換する
	strVal.MakeUpper();

	// 文字列の最初から、T,数値以外の文字が出現するまでの部分文字列を抽出
	strTemp = strVal.SpanIncluding(_T("T0123456789"));
	if(strTemp.Compare(strVal) != 0)	{
		strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_THNO_ERR3);
		return(FALSE);
	}

	// ----------------------------------------------------------------------------------
	// ② 数字の桁数が13桁未満ではないかのチェック
	// ----------------------------------------------------------------------------------
	if(strVal.Left(1) == "T")	{
		chkVal = strVal.Mid(1,strVal.GetLength() - 1);
	}
	else	{
		chkVal = strVal;
	}
	if(chkVal.GetLength() < 13)	{
		strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_THNO_ERR1);
		return(FALSE);
	}

	// ----------------------------------------------------------------------------------
	// ③ 数字の桁数が13桁を超えていないかのチェック
	// ----------------------------------------------------------------------------------
	if(chkVal.GetLength() > 13)	{
		strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_THNO_ERR2);
		return(FALSE);
	}

	// ----------------------------------------------------------------------------------
	// ④ チェックディジットが正しいかチェック
	// ----------------------------------------------------------------------------------
	if(strVal.Left(1) == "T")	{
		chkVal = strVal;
	}
	else	{
		chkVal.Format(_T("T%s"),strVal);
	}
	CSyzInvoice si;
	if(si.CheckInvoiceNumber(chkVal) == -1)	{
		strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_THNO_ERR4);
		return(FALSE);
	}

	// ----------------------------------------------------------------------------------
	// ⑤ "T"が付いていなければ先頭に半角スペースを付加して返す
	// ----------------------------------------------------------------------------------
	if(strVal.Left(1) != "T")	{
		strVal.Format(_T(" %s"),strVal);
	}

	return(TRUE);
}
// 登録番号（法人番号）_23/11/21 add <--

//**********************************************************************
// CSVのDataBlock識別子を文字列→DBデータに変換
//		・データインポートで使用。
//		・文字列とDBデータの対応。
//			--------------------------------------------------
//			No.	文字列			DBデータ	備考
//			--------------------------------------------------
//			1	“[明細]”		1			通常（データ）行
//			2	“[小計]”		10			小計行
//			3	“[中計]”		12			中計行
//			4	“内訳”		201			⑤棚卸資産下欄で利用
//			5	その他の文字列	0			関数失敗
//			--------------------------------------------------
// 
//	引数
//			strCsvId				[IN] 文字列
//			pcFgFunc				[OUT]DBデータ（特殊行フラグ）
// midori 156936 add -->
//			pSw						0:項目「科目」無効　1:項目「科目」有効		様式②、様式⑪のみで使用
// midori 156936 add <--
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
// midori 156936 del -->
//int CimpUc000Common::ConvCsvDataIdStrToDbVal(CString strCsvId, BYTE* pcFgFunc)
// midori 156936 del <--
// midori 156936 add -->
int CimpUc000Common::ConvCsvDataIdStrToDbVal(CString strCsvId, BYTE* pcFgFunc, int pSw)
// midori 156936 add <--
{
	// 通常（データ）行？
	if (strCsvId.Compare(CSV_ID_DATA) == 0) {
		*pcFgFunc = ID_FGFUNC_DATA;
		return TRUE;
	}

	// 小計行？
	if (strCsvId.Compare(CSV_ID_SYOKEI) == 0) {
		*pcFgFunc = ID_FGFUNC_SYOKEI;
		return TRUE;
	}

	// 中計行？
	if (strCsvId.Compare(CSV_ID_CHUKEI) == 0) {
		*pcFgFunc = ID_FGFUNC_CHUKEI;
		return TRUE;
	}

	// ⑤棚卸資産下欄で利用？
	if (strCsvId.Compare(CSV_ID_UCHIWAKE) == 0) {
		*pcFgFunc = ID_FGFUNC_TANAOROSHI;
		return TRUE;
	}

	// ⑨買掛金下欄で利用？
	if (strCsvId.Compare(CSV_ID_HAITOKIN) == 0) {
		*pcFgFunc = ID_FGFUNC_DATA;
		return TRUE;
	}

	// ⑨買掛金下欄で利用？
	if (strCsvId.Compare(CSV_ID_YAKUINSYOYO) == 0) {
		*pcFgFunc = ID_FGFUNC_DATA;
		return TRUE;
	}

// midori 156936 del -->
//// midori 190505 add -->
//	// ②受取手形の内訳書、⑪借入金及び支払利子の内訳書
//	if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111) {
//		if (strCsvId.Compare(CSV_ID_KAMOKU) == 0) {
//			*pcFgFunc = ID_FGFUNC_KAMOKU;
//			return TRUE;
//		}
//	}
//// midori 190505 add <--
// midori 156936 del <--
// midori 156936 add -->
	if(pSw == 1) {
// 改良No.21-0086,21-0529 cor -->
		//// ②受取手形の内訳書、⑪借入金及び支払利子の内訳書
		//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111) {
// ------------------------------
		// ②受取手形の内訳書、登録番号追加版の⑧支払手形の内訳書、⑪借入金及び支払利子の内訳書
		if(m_nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_nFormSeq == ID_FORMNO_081) || m_nFormSeq == ID_FORMNO_111) {
// 改良No.21-0086,21-0529 cor <--
			if (strCsvId.Compare(CSV_ID_KAMOKU) == 0) {
				*pcFgFunc = ID_FGFUNC_KAMOKU;
				return TRUE;
			}
		}
	}
// midori 156936 add <--

	// インポート対象外
	*pcFgFunc = ID_FGFUNC_NULL;
	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_NOT_TARGET_ROW);
// midori 150898 add -->
	m_uImpCsvInfo.nErr = 1;
// midori 150898 add <--
	return FALSE;
}

//**********************************************************************
// CSVのDataBlock識別子をDBデータ→文字列に変換
//		・データエクスポートで使用。
//		・DBデータと文字列の対応。
//			--------------------------------------------------
//			No.	DBデータ		文字列		備考
//				FgFunc	FgShow		
//			--------------------------------------------------
//			1	1		*		“[明細]”	通常（データ）行
//			2	1		1		“”		一時保管(関数失敗)
//			3	10		*		“[小計]”	小計行
//			4	11		*		“[小計]”	小計行（空行）
//			5	12		*		“[中計]”	中計行
//			6	13		*		“[中計]”	中計行（空行）
//			7	21		*		“[明細]”	一括集計金額行（手動）
//			8	201		*		“内訳”	⑤棚卸資産下欄で利用
//			9	*		2		“[明細]”	非表示（一括）
//			10	その他の組合せ	“”		関数失敗
//			--------------------------------------------------
// 
//	引数
//			cFgFunc					[IN] DBデータ（特殊行フラグ）
//			cFgShow					[IN] DBデータ（表示フラグ）
//			pstrCsvId				[OUT]文字列
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
int CimpUc000Common::ConvCsvDataIdDbValToStr(BYTE cFgFunc, BYTE cFgShow, CString* pstrCsvId)
{
	// 特殊行フラグ？
	switch (cFgFunc) {
	case	ID_FGFUNC_DATA:			// 通常（データ）行
		// 一時保管
		if (cFgShow == ID_FGSHOW_HOKAN) {
			*pstrCsvId = _T("");
			return FALSE;
		}
		*pstrCsvId = CSV_ID_DATA;
		return TRUE;
	case	ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
		*pstrCsvId = CSV_ID_DATA;
		return TRUE;
	case	ID_FGFUNC_SYOKEI:		// 小計行
	case	ID_FGFUNC_SYOKEINULL:	// 小計行（空行）
		*pstrCsvId = CSV_ID_SYOKEI;
		return TRUE;
	case	ID_FGFUNC_CHUKEI:		// 中計行
	case	ID_FGFUNC_CHUKEINULL:	// 中計行（空行）
		*pstrCsvId = CSV_ID_CHUKEI;
		return TRUE;
	case	ID_FGFUNC_TANAOROSHI:	// ⑤棚卸資産下欄で利用
		*pstrCsvId = CSV_ID_UCHIWAKE;
		return TRUE;
// midori 160609 add -->
	case	ID_FGFUNC_NULL:			// 空行
		// 買掛金の内訳書(欄外)は、空行をエクスポートする
		if(m_nKaikakeRangai == 1)	{
			*pstrCsvId = CSV_ID_DATA;
			return TRUE;
		}
		break;
// midori 160609 add <--
// midori 190505 add -->
	case	ID_FGFUNC_KAMOKU:			// 科目行
// 改良No.21-0086,21-0529 cor -->
		//// ②受取手形の内訳書、⑪借入金及び支払利子の内訳書
		//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111) {
// ------------------------------
		// ②受取手形の内訳書、登録番号追加版の⑧支払手形の内訳書、⑪借入金及び支払利子の内訳書
		if(m_nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_nFormSeq == ID_FORMNO_081) || m_nFormSeq == ID_FORMNO_111) {
// 改良No.21-0086,21-0529 cor <--
			*pstrCsvId = CSV_ID_KAMOKU;
			return TRUE;
		}
// midori 190505 add <--
	}

	// 表示フラグ？
	switch (cFgShow) {
	case	ID_FGSHOW_IKKATU:		// 非表示（一括）
		*pstrCsvId = CSV_ID_DATA;
		return TRUE;
	}

	// エクスポート対象外
	*pstrCsvId = _T("");
	return FALSE;
}

//**********************************************************************
// 日付を文字列→DBデータに変換
//		・データインポートで使用。
//		・文字列とDBデータの対応。
//			------------------------------------------------------------
//			Type		文字列			DBデータ		例
//			------------------------------------------------------------
//			A			“gee.mm.dd”	yyyymmdd		19890203
//			B			“gee.mm”		yyyymm01		19890200	※1,※2
//			C			“Gee.mm”		yyyymm01		19890200	※1,※2
//			D			“Gee”			yyyy1231		19891231	※3
//			その他の値	“”			0				0（関数失敗）
//			------------------------------------------------------------
//			※1.和暦年がオーバーしても、正常に西暦変換される（日付変換クラスの仕様）。
//			※2.日は指定した年月により異なる（日付変換クラスの仕様）。
//				各和暦の元年の開始月　　 → 初日（例：平成01.01 → 19890108）
//				各和暦の元年のその他の月 → 「00」
//				各和暦の２年以降　　　　 → 「01」
//			※3.月日は「1231」（年の最終日）固定（本関数内で指定）。
//				これは、本関数内で指定する。
// 
//		・本関数では、和暦→西暦変換時にBCD値を使用する。
//		　（日付変換クラスの仕様）
// 
//	引数
//			nConvType				[IN] 日付変換タイプ
//			strDate					[IN] 文字列
//			pnDate					[OUT]DBデータ（西暦年）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::ConvDateStrToDbVal(int nConvType, CString strDate, long* pnDate)
{
	// 空文字の場合は処理しない（正常終了）
	if (strDate.IsEmpty() == TRUE) {
		return TRUE;
	}

	*pnDate		= 0;

	//--------------------------------------------------
	// (1) 全体の文字数チェック
	// (2) 部品ごとに文字列分割（例：｜g｜ee｜.｜mm｜.｜dd｜）
	//--------------------------------------------------
	CString		strGengou		= _T("");	// g or G	：和暦元号	（1 Byte or 4 Byte）
	CString		strWareki		= _T("");	// ee		：和暦年	（2 Byte）→ (1～2 Byte) [2009/07/10 Change]
	CString		strPeriod1		= _T("");	// .		：ピリオド	（1 Byte）
	CString		strMonth		= _T("");	// mm		：月		（2 Byte）→ (1～2 Byte) [2009/07/10 Change]
	CString		strPeriod2		= _T("");	// .		：ピリオド	（1 Byte）
	CString		strDay			= _T("");	// dd		：日		（2 Byte）→ (1～2 Byte) [2009/07/10 Change]
	CString		strEra			= _T("");	// 文字列加工用(元号)	[2009/07/10 Add]
// midori 150895 add -->
	CString		strTemp			= _T("");
// midori 150895 add <--
	int			iYear			= 0;		// 文字列加工用(年)		[2009/07/10 Add]
	int			iMonth			= 0;		// 文字列加工用(月)		[2009/07/10 Add]
	int			iDay			= 0;		// 文字列加工用(日)		[2009/07/10 Add]

	// タイプ別に文字数チェック＆文字列分割
	switch (nConvType) {
	case	IMP_DATE_CONV_TYPE_A:	// gee.mm.dd
		// 年月日の桁数を2桁に変換	[2009/07/10 Add]
		strEra = strDate.Mid(0, 1);	
		strDate.Delete(0, 1);
		if ( sscanf_s( strDate, "%d.%d.%d", &iYear, &iMonth, &iDay ) == 0 ) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add -->
		strTemp = strDate.SpanIncluding(_T("0123456789."));
		if (strTemp.Compare(strDate) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add <--
		strDate.Format( "%s%02d.%02d.%02d", strEra, iYear, iMonth, iDay );

		// 文字数チェック
		if (CheckImportStr(strDate, 9, TRUE) != TRUE) {
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}

		// 文字列分割
		strGengou		= strDate.Mid(0, 1);	// g ：和暦元号（M/T/S/H）
		strWareki		= strDate.Mid(1, 2);	// ee：和暦年
		strPeriod1		= strDate.Mid(3, 1);	// . ：ピリオド
		strMonth		= strDate.Mid(4, 2);	// mm：月
		strPeriod2		= strDate.Mid(6, 1);	// . ：ピリオド
		strDay			= strDate.Mid(7, 2);	// dd：日
		break;
	case	IMP_DATE_CONV_TYPE_B:	// gee.mm
		// 年月の桁数を2桁に変換	[2009/07/10 Add]
		strEra = strDate.Mid(0, 1);	
		strDate.Delete(0, 1);
		if ( sscanf_s( strDate, "%d.%d", &iYear, &iMonth ) == 0 ) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add -->
		strTemp = strDate.SpanIncluding(_T("0123456789."));
		if (strTemp.Compare(strDate) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add <--
		strDate.Format( "%s%02d.%02d", strEra, iYear, iMonth );

		// 文字数チェック
		if (CheckImportStr(strDate, 6, TRUE) != TRUE) {
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}
		// 文字列分割
		strGengou		= strDate.Mid(0, 1);	// g ：和暦元号（M/T/S/H）
		strWareki		= strDate.Mid(1, 2);	// ee：和暦年
		strPeriod1		= strDate.Mid(3, 1);	// . ：ピリオド
		strMonth		= strDate.Mid(4, 2);	// mm：月
		break;
	case	IMP_DATE_CONV_TYPE_C:	// Gee.mm
		// 年月の桁数を2桁に変換	[2009/07/10 Add]
		strEra = strDate.Mid(0, 4);	
		strDate.Delete(0, 4);
		if ( sscanf_s( strDate, "%d.%d", &iYear, &iMonth ) == 0 ) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add -->
		strTemp = strDate.SpanIncluding(_T("0123456789."));
		if (strTemp.Compare(strDate) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add <--
		strDate.Format( "%s%02d.%02d", strEra, iYear, iMonth );

		// 文字数チェック
		if (CheckImportStr(strDate, 9) != TRUE) {
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}
		// 文字列分割
		strGengou		= strDate.Mid(0, 4);	// G ：和暦元号（明治/大正/昭和/平成）
		strWareki		= strDate.Mid(4, 2);	// ee：和暦年
		strPeriod1		= strDate.Mid(6, 1);	// . ：ピリオド
		strMonth		= strDate.Mid(7, 2);	// mm：月
		break;
	case	IMP_DATE_CONV_TYPE_D:	// Gee
		// 年月日の桁数を2桁に変換	[2009/07/10 Add]
		strEra = strDate.Mid(0, 4);	
		strDate.Delete(0, 4);
		if ( sscanf_s( strDate, "%d", &iYear ) == 0 ) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add -->
		strTemp = strDate.SpanIncluding(_T("0123456789"));
		if (strTemp.Compare(strDate) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 150895 add <--
		strDate.Format( "%s%02d", strEra, iYear );

		// 文字数チェック
		if (CheckImportStr(strDate, 6) != TRUE) {
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}
		// 文字列分割
		strGengou		= strDate.Mid(0, 4);	// G ：和暦元号（明治/大正/昭和/平成）
		strWareki		= strDate.Mid(4, 2);	// ee：和暦年
		break;
	default:
		strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
		return FALSE;	// タイプが不正
	}


	//--------------------------------------------------
	// (3) 分割文字ごとに値チェック
	//--------------------------------------------------
	int			nGengou			= 0;		// 和暦元号（数値）
	int			nWareki			= 0;		// 和暦年
	int			nMonth			= 0;		// 月
	int			nDay			= 0;		// 日
// midori 97 add -->
	int			seireki			= 0;		// 西暦
// midori 97 add <--
	// 日付変換クラスの都合上、変換するときはBCD値を使う
	BYTE		cGengou			= 0x00;		// 和暦元号（数値）
	BYTE		cWareki			= 0x00;		// 和暦年
	BYTE		cMonth			= 0x00;		// 月
	BYTE		cDay			= 0x00;		// 日
// midori 97 add -->
	CString		strGengo		= "";		// 元号名
// midori 97 add <--

	// タイプA、Bのみでチェック
	if ((nConvType == IMP_DATE_CONV_TYPE_A) || (nConvType == IMP_DATE_CONV_TYPE_B)) {
		//==================================================
		// g ：和暦元号（M/T/S/H）
		//==================================================
		// 文字列→整数
		if (ConvRomanGgengouStrToInt(strGengou, &nGengou) != TRUE) {
			return FALSE;
		}
		// 整数→BCD
		int_bcd(&cGengou, nGengou, 1);
	}

	// タイプC、Dのみでチェック
	if ((nConvType == IMP_DATE_CONV_TYPE_C) || (nConvType == IMP_DATE_CONV_TYPE_D)) {
		//==================================================
		// G ：和暦元号（明治/大正/昭和/平成）
		//==================================================
		// 文字列→整数
		if (ConvGgengouStrToInt(strGengou, &nGengou) != TRUE) {
			return FALSE;
		}
		// 整数→BCD
		int_bcd(&cGengou, nGengou, 1);
	}

	// 全タイプでチェック
	{
		//==================================================
		// ee：和暦年
		//==================================================
		// 数値チェック
//		if (CheckImportNum(strWareki, 2) != TRUE) {
		if (CheckImportNum(strWareki, 2, FALSE) != TRUE) {			// [2009/07/09 Minus Value Support]
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}
		// 文字列→整数
		nWareki	= atoi(strWareki);
		// 整数→BCD
		int_bcd(&cWareki, nWareki, 1);
		// 範囲チェック
		if ((nWareki < 1) || (99 < nWareki)) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
	}

	// タイプA、B、Cのみでチェック
	if ((nConvType == IMP_DATE_CONV_TYPE_A) || (nConvType == IMP_DATE_CONV_TYPE_B) || (nConvType == IMP_DATE_CONV_TYPE_C)) {
		//==================================================
		// . ：ピリオド
		//==================================================
		// 文字列比較
		if (strPeriod1.Compare(_T(".")) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}

		//==================================================
		// mm：月
		//==================================================
		// 数値チェック
//		if (CheckImportNum(strMonth, 2) != TRUE) {
		if (CheckImportNum(strMonth, 2, FALSE) != TRUE) {			// [2009/07/09 Minus Value Support]
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}
		// 文字列→整数
		nMonth	= atoi(strMonth);
		// 整数→BCD
		int_bcd(&cMonth, nMonth, 1);
		// 範囲チェック
		if ((nMonth < 1) || (12 < nMonth)) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
	}

	// タイプAのみでチェック
	if (nConvType == IMP_DATE_CONV_TYPE_A) {
		//==================================================
		// . ：ピリオド
		//==================================================
		// 文字列比較
		if (strPeriod2.Compare(_T(".")) != 0) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}

		//==================================================
		// dd：日
		//==================================================
		// 数値チェック
//		if (CheckImportNum(strDay, 2) != TRUE) {
		if (CheckImportNum(strDay, 2, FALSE) != TRUE) {			// [2009/07/09 Minus Value Support]
// midori 150895 add -->
			// "日付が正しくありません。" に書き換える
			memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
			strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
// midori 150895 add <--
			return FALSE;
		}
		// 文字列→整数
		nDay	= atoi(strDay);
		// 整数→BCD
		int_bcd(&cDay, nDay, 1);
		// 末日を取得
		BYTE		cLastDay		= 0x00;		// BCD
		int			nLastDay		= 0;		// 整数
		cLastDay	= m_clsDate.GetLastDay(cGengou, cWareki, cMonth);
		nLastDay	= bcd_int(&cLastDay, 1);
		// 範囲チェック
		if ((nDay < 1) || (nLastDay < nDay)) {
			strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_TYPE);
			return FALSE;
		}
// midori 181101 add -->
		if(IsValidKaigen() == TRUE)	{
			// １年１月１日 ～ １年１月７日はエラーとする
			if( atoi(strWareki) == 1 && atoi(strMonth) == 1 && 
				(atoi(strDay) >= 1 && atoi(strDay) <= 7) )	{
				memset(m_uImpCsvInfo.szErrType,0,sizeof(m_uImpCsvInfo.szErrType));
				strcpy_s(m_uImpCsvInfo.szErrType,sizeof(m_uImpCsvInfo.szErrType),IMP_ERRMSG_ITM_DATE_TYPE);
				return FALSE;
			}
		}
// midori 181101 add <--
	}

// midori 97 add -->
	// タイプB、タイプCの対応
	if(IsValidKaigen() == TRUE) {
		if (nConvType == IMP_DATE_CONV_TYPE_B || nConvType == IMP_DATE_CONV_TYPE_C) {
			// 日付を取得
			m_clsDate.GetYmDay(cGengou,cWareki,cMonth,&cDay);
			nDay = bcd_int(&cDay,1);
			// 和暦 → 西暦
			WarekiToSeireki(seireki,nGengou,nWareki,nMonth,nDay);
			// 西暦 → 和暦
			SeirekiToWareki(seireki,nGengou,nWareki,nMonth,nDay,strGengo);

			// 整数→BCD
			int_bcd(&cGengou, nGengou, 1);
			int_bcd(&cWareki, nWareki, 1);
			int_bcd(&nMonth, nMonth, 1);
			int_bcd(&nDay, nDay, 1);
		}
	}
// midori 97 add <--

	// タイプDの対応
	if (nConvType == IMP_DATE_CONV_TYPE_D) {
		// タイプDでは、月日を年の最終月（12月31日）にしてDBに格納する
// midori 181101 del -->
		//cMonth			= 0x12;		// 月
		//cDay			= 0x31;		// 日
// midori 181101 del <--
// midori 181101 add -->
		if(nWareki == 1) {
			cMonth			= 0x12;		// 月
			cDay			= 0x31;		// 日
		}
		else {
			cMonth			= 0x01;		// 月
			cDay			= 0x01;		// 日
		}
// midori 181101 add <--
	}

	//--------------------------------------------------
	// 日付変換
	//--------------------------------------------------
	// 和暦→西暦
	*pnDate = m_clsDate.Convert(cGengou, cWareki, cMonth, cDay);

	return TRUE;
}

//**********************************************************************
// 日付をDBデータ→文字列に変換
//		・データエクスポートで使用。
//		・DBデータと文字列の対応。
//			------------------------------------------------------------
//			Type		DBデータ		文字列			例
//			------------------------------------------------------------
//			A			yyyymmdd		“gee.mm.dd”	“H01.02.03”
//			B			   〃			“gee.mm”		“H01.02”
//			C			   〃			“Gee.mm”		“平成01.02”
//			D			   〃			“Gee”			“平成01”
//			その他の値	   〃			“”			“”（関数失敗）
//			------------------------------------------------------------
// 
//	引数
//			nConvType				[IN] 日付変換タイプ
//			nDate					[IN] DBデータ（西暦年）
//			pstrDate				[OUT]文字列
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::ConvDateDbValToStr(int nConvType, long nDate, CString* pstrDate)
{
	// データがない場合は処理しない（正常終了）
	if (nDate <= 0) {
		return TRUE;
	}

	CString		strRomanGengou	= _T("");	// g ：和暦元号（M/T/S/H）

	// 西暦→和暦
	if (m_clsDate.Convert(nDate) != 0) {
		return FALSE;		// 変換失敗（ありえない）
	}

	// ローマ字元号を取得（タイプA、Bで使用）
	ConvRomanGgengouIntToStr(m_clsDate.m_nGengou, &strRomanGengou);

	// タイプ別に文字列を生成
	switch (nConvType) {
	case	IMP_DATE_CONV_TYPE_A:	// gee.mm.dd
		pstrDate->Format(	_T("%s%02d.%02d.%02d"),
							strRomanGengou,				// g ：和暦元号（M/T/S/H）
							m_clsDate.m_nWareki,		// ee：和暦年
							m_clsDate.m_nMonth,			// mm：月
							m_clsDate.m_nDay);			// dd：日
		return TRUE;
	case	IMP_DATE_CONV_TYPE_B:	// gee.mm
		pstrDate->Format(	_T("%s%02d.%02d"),
							strRomanGengou,				// g ：和暦元号（M/T/S/H）
							m_clsDate.m_nWareki,		// ee：和暦年
							m_clsDate.m_nMonth);		// mm：月
		return TRUE;
	case	IMP_DATE_CONV_TYPE_C:	// Gee.mm
		pstrDate->Format(	_T("%s%02d.%02d"),
							m_clsDate.m_szGengou,		// G ：和暦元号（明治/大正/昭和/平成）
							m_clsDate.m_nWareki,		// ee：和暦年
							m_clsDate.m_nMonth);		// mm：月
		return TRUE;
	case	IMP_DATE_CONV_TYPE_D:	// Gee
		pstrDate->Format(	_T("%s%02d"),
							m_clsDate.m_szGengou,		// G ：和暦元号（明治/大正/昭和/平成）
							m_clsDate.m_nWareki);		// ee：和暦年
		return TRUE;
	}

	// エクスポート対象外
	*pstrDate = _T("");
	return FALSE;
}

//**********************************************************************
//（サブ関数）
// 和暦元号をローマ字→数値に変換
//		・データインポートで使用。
//		・ローマと数値の対応。
//			--------------------------------------------------
//			No.	ローマ字		数値		備考
//			--------------------------------------------------
//			1	“M”			1			明治
//			2	“T”			2			大正
//			3	“S”			3			昭和
//			4	“H”			4			平成
//			5	その他の文字列	0			関数失敗
//			--------------------------------------------------
// 
//	引数
//			strGengou				[IN] 和暦元号（ローマ字）
//			pnGengou				[OUT]和暦元号（数値）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::ConvRomanGgengouStrToInt(CString strGengou, int* pnGengou)
{
// midori 181101 add -->
	CVolDateDB			vd;
	CString				strGengo=_T("");
// midori 181101 add <--

	// 明治？
	if (strGengou.Compare(CSV_ROMAN_GENGOUW_MEIJ) == 0) {
		*pnGengou = GT_MEIJI;
		return TRUE;
	}

	// 大正？
	if (strGengou.Compare(CSV_ROMAN_GENGOUW_TAISYOU) == 0) {
		*pnGengou = GT_TAISYOU;
		return TRUE;
	}

	// 昭和？
	if (strGengou.Compare(CSV_ROMAN_GENGOUW_SYOUWA) == 0) {
		*pnGengou = GT_SYOUWA;
		return TRUE;
	}

	// 平成？
	if (strGengou.Compare(CSV_ROMAN_GENGOUW_HEISEI) == 0) {
		*pnGengou = GT_HEISEI;
		return TRUE;
	}

// midori 181101 add -->
	if(IsValidKaigen() == TRUE) {
		if(vd.db_vd_NoToAlpGen(5,strGengo,1) == 0) {
			// 改元？
			if (strGengou.Compare(strGengo) == 0) {
				*pnGengou = GT_KAIGEN;
				return TRUE;
			}
		}
	}
// midori 181101 add <--
	// インポート対象外
	*pnGengou = GT_REIGAI;
	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_GENGO);
	return FALSE;
}

//**********************************************************************
//（サブ関数）
// 和暦元号を数値→ローマ字に変換
//		・データエクスポートで使用。
//		・数値とローマ字の対応。
//			--------------------------------------------------
//			No.	数値			ローマ字	備考
//			--------------------------------------------------
//			1	1				“M”		明治
//			2	2				“T”		大正
//			3	3				“S”		昭和
//			4	4				“H”		平成
//			5	その他の値		“”		関数失敗
//			--------------------------------------------------
// 
//	引数
//			nGengou					[IN] 和暦元号（数値）
//			pstrGengou				[OUT]和暦元号（ローマ字）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::ConvRomanGgengouIntToStr(int nGengou, CString* pstrGengou)
{
// midori 181101 del -->
	//// 和暦元号？
	//switch (nGengou){
	//case	GT_MEIJI:	// 明治
	//	*pstrGengou = CSV_ROMAN_GENGOUW_MEIJ;
	//	return TRUE;
	//case	GT_TAISYOU:	// 大正
	//	*pstrGengou = CSV_ROMAN_GENGOUW_TAISYOU;
	//	return TRUE;
	//case	GT_SYOUWA:	// 昭和
	//	*pstrGengou = CSV_ROMAN_GENGOUW_SYOUWA;
	//	return TRUE;
	//case	GT_HEISEI:	// 平成
	//	*pstrGengou = CSV_ROMAN_GENGOUW_HEISEI;
	//	return TRUE;
	//}
// midori 181101 del <--
// midori 181101 add -->
	CVolDateDB			vd;
	CString				strGengo = _T("");

	// 和暦元号？
	switch (nGengou){
	case	GT_MEIJI:	// 明治
		*pstrGengou = CSV_ROMAN_GENGOUW_MEIJ;
		return TRUE;
	case	GT_TAISYOU:	// 大正
		*pstrGengou = CSV_ROMAN_GENGOUW_TAISYOU;
		return TRUE;
	case	GT_SYOUWA:	// 昭和
		*pstrGengou = CSV_ROMAN_GENGOUW_SYOUWA;
		return TRUE;
	case	GT_HEISEI:	// 平成
		*pstrGengou = CSV_ROMAN_GENGOUW_HEISEI;
		return TRUE;
	case	GT_KAIGEN:	// 改元
		if(IsValidKaigen() == TRUE) {
			if(vd.db_vd_NoToAlpGen(nGengou,strGengo,1) == 0) {
				*pstrGengou = strGengo;
				return TRUE;
			}
		}
		break;
	}
// midori 181101 add <--

	// エクスポート対象外
	*pstrGengou = CSV_ROMAN_GENGOUW_REIGAI;
	return FALSE;
}

//**********************************************************************
//（サブ関数）
// 和暦元号を漢字→数値に変換
//		・データインポートで使用。
//		・漢字と数値の対応。
//			--------------------------------------------------
//			No.	漢字			数値		備考
//			--------------------------------------------------
//			1	“明治”		1			明治
//			2	“大正”		2			大正
//			3	“昭和”		3			昭和
//			4	“平成”		4			平成
//			5	その他の文字列	0			関数失敗
//			--------------------------------------------------
// 
//	引数
//			strGengou				[IN] 和暦元号（漢字）
//			pnGengou				[OUT]和暦元号（数値）
// 
//	戻値
//			TRUE ：成功
//			FALSE：失敗
//**********************************************************************
BOOL CimpUc000Common::ConvGgengouStrToInt(CString strGengou, int* pnGengou)
{
// midori 181101 add -->
	CVolDateDB			vd;
	CString				strGengo = _T("");
// midori 181101 add <--

	// 明治？
	if (strGengou.Compare(DC_GENGOU_MEIJI) == 0) {
		*pnGengou = GT_MEIJI;
		return TRUE;
	}

	// 大正？
	if (strGengou.Compare(DC_GENGOU_TAISYOU) == 0) {
		*pnGengou = GT_TAISYOU;
		return TRUE;
	}

	// 昭和？
	if (strGengou.Compare(DC_GENGOU_SYOUWA) == 0) {
		*pnGengou = GT_SYOUWA;
		return TRUE;
	}

	// 平成？
	if (strGengou.Compare(DC_GENGOU_HEISEI) == 0) {
		*pnGengou = GT_HEISEI;
		return TRUE;
	}

// midori 181101 add -->
	// 改元？
	if(vd.db_vd_NoToStrGen(5,strGengo) == 0) {
		if (strGengou.Compare(strGengo) == 0) {
			*pnGengou = GT_KAIGEN;
			return TRUE;
		}
	}
// midori 181101 add <--

	// インポート対象外
	*pnGengou = GT_REIGAI;
	strcpy_s(m_uImpCsvInfo.szErrType, sizeof(m_uImpCsvInfo.szErrType), IMP_ERRMSG_ITM_DATE_GENGO);
	return FALSE;
}


//**********************************************************************
// 指定した数値文字列の乗算
//		・主に小数点ありの項目で使用
//		※本関数内では文字列チェックを行わない。
//		　指定した数値文字列が妥当な値かは、事前にCheckImportNum()で
//		　チェックすること。
// 
//	引数
//			strTarget				[IN] 計算対象の文字列
//			nMagnify				[IN] 倍率
//			nDecimalDigit			[IN] 小数部有効桁数
//			pnResult				[OUT]乗算結果（数値）
// 
//	戻値
//			乗算結果（文字列）
//**********************************************************************
CString CimpUc000Common::CalcStrMul(CString strTarget, double nMagnify , int nDecimalDigit/* = 0*/, double* pnResult/* = NULL*/)
{
	// 計算対象文字列が空文字の場合はそのまま返す
	if (strTarget.IsEmpty() == TRUE) {
		return strTarget;
	}
	
	RemoveCommaImportNum( strTarget );				// カンマを除く　2011.07.15 h.ukawa

	CString			strFormat		= _T("");		// 文字列フォーマット（小数部有効桁数の指定）
	CString			strTemp			= _T("");		// 文字列テンポラリ
	double			nTemp			= 0;			// 数値テンポラリ

	// 小数部有効桁数の指定
	strFormat.Format(_T("%%.0%df"), nDecimalDigit);

	// 文字列生成
	nTemp	= atof(strTarget);						// 文字列→数値
	nTemp	= nTemp * nMagnify;						// 掛け算
	strTemp.Format(strFormat, nTemp);				// 数値→文字列（四捨五入）

	// 乗算結果（数値）を返送
	if (pnResult != NULL) {
		*pnResult = nTemp;
	}

	return strTemp;
}


//**********************************************************************
// インポート時に格納する頁番号、行番号をインクリメントする
//		・頁番号、行番号の初期値は「0」とする。
//		・インポート時には、テーブルの頁構成を気にせず、機械的に行を追加していく。
//		　テーブル上では行番号に限界があるので、行数制限を超えたる場合は、
//		　頁番号のインクリメントで対処する。
//		・正しい頁番号、行番号にするには、頁の再構成処理が必要。
// 
//	引数
//			pnNumPage				[IN, OUT]頁番号
//			pnNumRow				[IN, OUT]行番号
// 
//	戻値
//			なし
//**********************************************************************
void CimpUc000Common::CheckNumPageRow(int* pnNumPage, int* pnNumRow)
{
	// 頁がない場合は、頁番号をインクリメント
	if (*pnNumPage < 1) {
		*pnNumPage	+= 1;
	}

	// 行番号をインクリメント
	*pnNumRow += 1;

	// 行数制限を超えた場合は、頁番号をインクリメント
	if (*pnNumRow > IMP_MAX_NUM_ROW) {
		*pnNumPage	+= 1;
		*pnNumRow	= 1;
	}
}


//**********************************************************************
// メッセージボックスを表示
// 
//	引数
//			nErrorCode				[IN] エラーコード
//			pWnd					[IN] 親ウィンドウのポインタ
// 
//	戻値
//			ICSMessageBoxの仕様に準じる
//**********************************************************************
// midori 150894 cor -->
//int CimpUc000Common::ShowMessageBox(int nErrorCode, CWnd* pWnd /*= NULL*/)
// ---------------------
int CimpUc000Common::ShowMessageBox(int nErrorCode, CWnd* pWnd /*= NULL*/, CDBZmSub* pZmSub)
// midori 150894 cor <--
{
	CString		strMsgONE		= _T("");		// メッセージ１
	CString		strMsgALL		= _T("");		// メッセージ３
	UINT		nType			= 0;			// メッセージボックスのスタイル
	CString		strMsgClm;
// midori 150894 add -->
	int				ii;
	CdbUcInfSub*	pUcInfSub;
// midori 150894 add <--
// midori 150895 add -->
	IMP_CSV_INFO	csvinfo;					// CSVの情報
// midori 150895 add <--

	// エラーコード判定
	switch (nErrorCode) {
	case	IMP_ERRCODE_SUCCESS:	// 正常終了
		if (m_uImpCsvInfo.fImport == TRUE) {
			// インポート
			strMsgALL.Format(_T("%s"), IMP_INFMSG_IMPORT);
		}
		else {
			// エクスポート
			strMsgALL.Format(_T("%s"), IMP_INFMSG_EXPORT);
		}
		nType	= (MB_ICONINFORMATION | MB_OK);
		break;
// midori 160603 del -->
	//case	IMP_ERRCODE_FINDFOLDER:		// エラー（フォルダがない）
	//	strMsgONE.Format(IMP_ERRMSG_FINDFOLDER, m_uImpCsvInfo.szFolderPath);
	//	strMsgALL.Format(_T("%s"), strMsgONE);
	//	nType	= (MB_ICONWARNING | MB_OK);
	//	break;
// midori 160603 del <--
	case	IMP_ERRCODE_CREATEDFOLDER:	// エラー（ファイル作成失敗）
		strMsgONE.Format(IMP_ERRMSG_CREATEDFOLDER, m_uImpCsvInfo.szFolderPath);
		strMsgALL.Format(_T("%s"), strMsgONE);
		nType	= (MB_ICONWARNING | MB_OK);
		break;
	case	IMP_ERRCODE_CSVOPEN:		// エラー（CSVファイルのオープン失敗）
//		strMsgONE.Format(IMP_ERRMSG_CSVOPEN, m_uImpCsvInfo.szFileName);
		strMsgONE.Format(IMP_ERRMSG_CSVOPEN, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.szSheetName);	// [2009/07/08 Add for Error Message]
		strMsgALL.Format(_T("%s\r\n\r\n%s"), strMsgONE, IMP_ERRMSG_SKIP_SHEET);
		nType	= (MB_ICONWARNING | MB_YESNO);
		break;
	case	IMP_ERRCODE_FIXDATA:		// エラー（固定データが見つからない）
//		strMsgONE.Format(IMP_ERRMSG_FIXDATA, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.nLine, m_uImpCsvInfo.szId);
		strMsgONE.Format(IMP_ERRMSG_FIXDATA, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.szSheetName, m_uImpCsvInfo.nLine, m_uImpCsvInfo.szId);	// [2009/07/08 Add for Error Message]
		strMsgALL.Format(_T("%s\r\n\r\n%s"), strMsgONE, IMP_ERRMSG_SKIP_SHEET);
		nType	= (MB_ICONWARNING | MB_YESNO);
		break;
// 157766 add -->
	case	IMP_ERRCODE_NUMOFITEM:		// エラー（項目数が一致していません）
// 157785 del -->
		//strMsgONE.Format(IMP_ERRMSG_NUMOFITEM,m_uImpCsvInfo.szFileName,m_uImpCsvInfo.szSheetName,m_uImpCsvInfo.nLine);
// 157785 del <--
// 157785 add -->
		strMsgONE.Format(IMP_ERRMSG_NUMOFITEM,m_uImpCsvInfo.szFileName,m_uImpCsvInfo.szSheetName);
// 157785 add <--
		strMsgALL.Format(_T("%s\r\n\r\n%s"),strMsgONE,IMP_ERRMSG_SKIP_SHEET);
		nType = (MB_ICONWARNING | MB_YESNO);
		break;
// 157766 add <--
	case	IMP_ERRCODE_CONAME:			// エラー（会社名が異なる）
//		strMsgONE.Format(IMP_ERRMSG_CONAME, m_uImpCsvInfo.szFileName);
		strMsgONE.Format(IMP_ERRMSG_CONAME, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.szSheetName);	// [2009/07/08 Add for Error Message]
		strMsgALL.Format(_T("%s\r\n\r\n%s"), strMsgONE, IMP_ERRMSG_DO_SHEET);
		nType	= (MB_ICONWARNING | MB_YESNO);
		break;
// midori 150894 add -->
	case	IMP_ERRCODE_IMPORT_TITLE:	// エラー（データインポート失敗「様式名」）
		pUcInfSub = new CdbUcInfSub(pZmSub->m_database);
		strMsgALL = IMP_ERRMSG_TITLE1;
		strMsgALL = strMsgALL + _T("\r\n\r\n");
		for(ii=0; ii<SHEET_NUM; ii++)	{
			if(m_uImpCsvInfo.szErr[ii] == 1)	{
				pUcInfSub->Init(ii+1);
				strMsgALL = strMsgALL + _T("　") + pUcInfSub->m_Title + _T("\r\n");
			}
		}
		strMsgALL = strMsgALL + _T("\r\n");
		if(m_uImpCsvInfo.nAllErr == 1)	{
			strMsgALL = strMsgALL + IMP_ERRMSG_TITLE3;
			nType	= (MB_ICONWARNING | MB_OK);
		}
		else	{
			strMsgALL = strMsgALL + IMP_ERRMSG_TITLE2;
			nType	= (MB_ICONWARNING | MB_YESNO);
		}
		if(pUcInfSub != NULL)	{
			pUcInfSub->Fin();
			delete pUcInfSub;
		}
		break;
// midori 150894 add <--
// midori 170504 add -->
	case	IMP_ERRCODE_IMPORT_ITEM:	// エラー（データインポート失敗「項目タイトル」）
		strMsgALL.Format(_T("'%s' (%s)\r\n%d 行目のデータインポートに失敗しました。\r\n\r\n"),m_uImpCsvInfo.szFileName,m_uImpCsvInfo.szSheetName,m_uImpCsvInfo.nLine);
		strMsgONE.Format(_T("　'項目タイトル'%s（最大 8 文字）\r\n"),IMP_ERRMSG_ITM_OVER_LENGTH);
		strMsgALL = strMsgALL + strMsgONE + _T("\r\n\r\n") + IMP_ERRMSG_SKIP_LINE;
		nType	= (MB_ICONWARNING | MB_YESNO);
		break;
// midori 170504 add <--
// midori 150895 cor -->
//	case	IMP_ERRCODE_IMPORT:			// エラー（データインポート失敗）
////		strMsgONE.Format(IMP_ERRMSG_PART1, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.nLine);
//		strMsgClm = m_uImpCsvInfo.szColumn;
//		if(!strMsgClm.IsEmpty())	strMsgClm = _T("'") + strMsgClm + _T("'の");
//		strMsgONE.Format(IMP_ERRMSG_PART1, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.szSheetName, m_uImpCsvInfo.nLine, strMsgClm);	// [2009/07/08 Add for Error Message]	[2016/01/13 Add column info]
//		strMsgALL.Format(_T("%s%s\r\n%s\r\n\r\n%s"), strMsgONE, IMP_ERRMSG_IMPORT, m_uImpCsvInfo.szErrType, IMP_ERRMSG_SKIP_LINE);
//		nType	= (MB_ICONWARNING | MB_YESNO);
//		break;
// ---------------------
	case	IMP_ERRCODE_IMPORT:			// エラー（データインポート失敗）
		strMsgONE.Format(_T("'%s' (%s)\r\n%d 行目のデータインポートに失敗しました。"),m_uImpCsvInfo.szFileName,m_uImpCsvInfo.szSheetName,m_uImpCsvInfo.nLine);
		strMsgALL = strMsgONE + _T("\r\n");
		if(m_uImpCsvInfo.nErr == 1)	{
// midori 150966,150967,150969 cor -->
//// midori 150945 cor -->
//			//strMsgONE.Format(_T("　'%s'"),m_uImpCsvInfo.szId);
//			//strMsgONE = strMsgONE + _T(" がインポートの対象になっていません。");
//// ---------------------
//			strMsgONE.Format(_T("　項目名 %s が正しくありません。"),m_uImpCsvInfo.szId);
//// midori 150945 cor <--
// -----------------------------------
			strMsgONE = _T("　'項目名' が正しくありません。");
// midori 150966,150967,150969 cor <--
			strMsgALL = strMsgALL + _T("\r\n") + strMsgONE;
		}
		else if(m_uImpCsvInfo.nErr == 2)	{
			strMsgONE = _T("　項目数が一致していません。");
			strMsgALL = strMsgALL + _T("\r\n") + strMsgONE;
		}
		else	{
			for(ii=0; ii<(int)m_ErrMap.GetCount(); ii++)	{
				m_ErrMap.Lookup(ii,csvinfo);
				strMsgClm = csvinfo.szColumn;
				if(!strMsgClm.IsEmpty())	strMsgClm = _T("'") + strMsgClm + _T("'");
				// nKind → 種類(0:文字 1:金額 2:面積等)
				strMsgONE.Format(_T("%s"),csvinfo.szErrType);
				if(strMsgONE == IMP_ERRMSG_ITM_OVER_LENGTH && csvinfo.nKind != 0)	{
					// 文字以外はメッセージを変える
					strcpy_s(csvinfo.szErrType,sizeof(csvinfo.szErrType),IMP_ERRMSG_ITM_OVER_LENGTH_N);
				}
				strMsgONE.Format(_T("　%s %s"),strMsgClm,csvinfo.szErrType);
				strMsgALL = strMsgALL + _T("\r\n") + strMsgONE;
				strMsgONE.Format(_T("%s"),csvinfo.szErrType);
				if(strMsgONE == IMP_ERRMSG_ITM_OVER_LENGTH || strMsgONE == IMP_ERRMSG_ITM_OVER_LENGTH_N)	{
					// 「最大文字(桁)数を超えています。」の場合は、文字(桁)数を表示する
					// 0:文字
					if(csvinfo.nKind == 0)		{
						strMsgONE.Format(_T("（最大 %d 文字）"),csvinfo.nLength);
					}
					// 1:金額
					else if(csvinfo.nKind == 1)	{
						if(csvinfo.nType == 0)	{
							strMsgONE.Format(_T("（正数最大 %d 桁）"),csvinfo.nLength);
						}
						else	{
							strMsgONE.Format(_T("（負数最大 %d 桁）"),csvinfo.nLength);
						}
					}
					// 2:面積等
					else								{
						if(csvinfo.nType2 == 0)	{
							if(csvinfo.nType == 0)	{
								strMsgONE.Format(_T("（整数最大 %d 桁）"),csvinfo.nLength);
							}
							else	{
								strMsgONE.Format(_T("（整数[負]最大 %d 桁）"),csvinfo.nLength);
							}
						}
						else if(csvinfo.nType2 == 1)	{
							strMsgONE.Format(_T("（小数点以下最大 %d 桁）"),csvinfo.nLength2);
						}
						else	{
							if(csvinfo.nType == 0)	{
								strMsgONE.Format(_T("（整数最大 %d 桁）、（小数点以下最大 %d 桁）"),csvinfo.nLength,csvinfo.nLength2);
							}
							else	{
								strMsgONE.Format(_T("（整数[負]最大 %d 桁）、（小数点以下最大 %d 桁）"),csvinfo.nLength,csvinfo.nLength2);
							}
						}
					}
					strMsgALL = strMsgALL + strMsgONE;
				}
			}
		}
		strMsgALL = strMsgALL + _T("\r\n\r\n") + IMP_ERRMSG_SKIP_LINE;
		nType	= (MB_ICONWARNING | MB_YESNO);
		break;
// midori 150895 cor <--
	case	IMP_ERRCODE_EXPORT:			// エラー（データエクスポート失敗）
//		strMsgONE.Format(IMP_ERRMSG_PART2, m_uImpCsvInfo.szFileName);
		strMsgONE.Format(IMP_ERRMSG_PART2, m_uImpCsvInfo.szFileName, m_uImpCsvInfo.szSheetName);	// [2009/07/08 Add for Error Message]
		strMsgALL.Format(_T("%s%s\r\n\r\n%s"), strMsgONE, IMP_ERRMSG_EXPORT, IMP_ERRMSG_SKIP_SHEET);
		nType	= (MB_ICONWARNING | MB_YESNO);
		break;
	case	IMP_ERRCODE_DB:				// エラー（DBエラー）
		strMsgALL.Format(_T("%s"), IMP_ERRMSG_DB_ACCESS);
		nType	= (MB_ICONWARNING | MB_OK);
		break;
	case	IMP_ERRCODE_DRIVE_EMPTY:	// エラー（ドライブ容量不足）
		strMsgONE.Format(IMP_ERRMSG_DRIVE_EMPTY, m_uImpCsvInfo.szSheetName);
		strMsgALL.Format(_T("%s"), strMsgONE);
		nType	= (MB_ICONWARNING | MB_OK);
		break;
	default:							// エラー（その他）
		if (m_uImpCsvInfo.fImport == TRUE) {
			// インポート
			strMsgALL.Format(_T("%s"), IMP_ERRMSG_IMPORT);
		}
		else {
			// エクスポート
			strMsgALL.Format(_T("%s"), IMP_ERRMSG_EXPORT);
		}
		nType	= (MB_ICONWARNING | MB_OK);
		break;
	}
	// メッセージボックスの表示
	return ICSMessageBox(strMsgALL, nType, 0, 0, pWnd);
}

// ------------------------------------------------------------------------------------------------------------------------
// 項目取り込みのエラーメッセージを表示
// 
//	引数
//			nErrorCode				[IN] エラーコード
//			pWnd					[IN] 親ウィンドウのポインタ
// 
//	戻値
//			ICSMessageBoxの戻り値
//			※項目エラーの場合、はい→IDNO、いいえ→IDCANCELを戻り値をして返す
// ------------------------------------------------------------------------------------------------------------------------
int CimpUc000Common::ShowMessageBox2(int nErrorCode, CWnd* pWnd /*= NULL*/)
{
	int				sw=0;							// 0:項目数、名称エラー 2:取り込めない項目に対するエラー
	int				nRet=0;							// 戻り値（メッセージの戻り値を返す）
													// ※項目エラーの場合、はい→IDNO、いいえ→IDCANCELを戻り値をして返す
	CString			strMsg			= _T("");		// 各項目のメッセージ
	CString			strMsgALL		= _T("");		// 表示するメッセージ

	// ⑱-1～⑱-20 その他科目の内訳書 タイトル取込エラー
	if(nErrorCode == IMP_ERRCODE_IMPORT_ITEM)	{
		sw = 2;
	}
	else	{
		// 項目名取込エラー
		// nErr 1:項目名
		// nErr 2:項目数
		// nErr 3:⑭-2 人件費の内訳書の区分エラー
		if(m_uImpCsvInfo.nErr == 1 || m_uImpCsvInfo.nErr == 2)	{
			sw = 0;
		}
		else if(m_uImpCsvInfo.nErr == 3)	{
			sw = 1;
		}
// midori 156936 add -->
		else if(m_uImpCsvInfo.nErr == 4)	{
			sw = 3;
		}
// midori 156936 add <--
		else	{
			sw = 2;
		}
	}
	// CSV名称、帳票名をセット
	strMsgALL.Format(_T("\"%s\" 【%s】\r\n"),m_uImpCsvInfo.szFileName,m_uImpCsvInfo.szSheetName);
	// 取り込みエラーの種類によりメッセージを分岐
	if(sw == 0)	{
		strMsgALL.Format(strMsgALL + _T("%d 行目のデータインポートに失敗しました。\r\n\r\n"),m_uImpCsvInfo.nLine);
		if(m_uImpCsvInfo.nErr == 1)	strMsgALL = strMsgALL+ _T("　'項目名' が正しくありません。\r\n\r\n");
// 210113_5301_1 del -->
		//else						strMsgALL = strMsgALL+ _T("　'項目数が一致していません。\r\n\r\n");
// 210113_5301_1 del <--
// 210113_5301_1 add -->
		else						strMsgALL = strMsgALL+ _T("　項目数が一致していません。\r\n\r\n");
// 210113_5301_1 add <--
		strMsgALL = strMsgALL + IMP_ERRMSG_SKIP_LINE;
		// メッセージボックスの表示
		nRet = ICSMessageBox(strMsgALL, (MB_ICONWARNING | MB_YESNO), 0, 0, pWnd);
		if(nRet == IDYES)	nRet = IDNO;
		else				nRet = IDCANCEL;
	}
	else if(sw == 1)	{
		strMsgALL.Format(strMsgALL + _T("%d 行目のデータインポートに失敗しました。\r\n\r\n"),m_uImpCsvInfo.nLine);
		// 項目名、エラー内容をセット
		strMsgALL = strMsgALL + GetErrorMessage();
		strMsgALL = strMsgALL + _T("\r\n");
		strMsgALL = strMsgALL + IMP_ERRMSG_SKIP_LINE;
		// メッセージボックスの表示
		nRet = ICSMessageBox(strMsgALL, (MB_ICONWARNING | MB_YESNO), 0, 0, pWnd);
		if(nRet == IDYES)	nRet = IDNO;
		else				nRet = IDCANCEL;
	}
// midori 156936 add -->
	else if(sw == 3) {
		// 1行分スペースを空ける
		strMsgALL = strMsgALL + _T("\r\n");

		if(m_nFormSeq == ID_FORMNO_021) {
			strMsg = _T("\"振出人１\"、\"振出人２\" ");
		}
// 改良No.21-0086,21-0529 add -->
		else if(m_nFormSeq == ID_FORMNO_081) {
			strMsg = _T("\"支払先１\"、\"支払先２\" ");
		}
// 改良No.21-0086,21-0529 add <--
		else {
			strMsg = _T("\"借入先１\"、\"借入先２\" ");
		}

		strMsgALL.Format(strMsgALL + _T("%d 行目のデータは科目行ですが、%sがともに空白のため\r\n科目名称が取り込まれません。\r\n"),m_uImpCsvInfo.nLine,strMsg);
		strMsgALL = strMsgALL + _T("\r\n");
		strMsgALL = strMsgALL + _T("科目名称を空白でインポートしますか？\r\n");
		strMsgALL = strMsgALL + _T("　「はい」　　　科目名称を空白でインポートする\r\n");
		strMsgALL = strMsgALL + _T("　「いいえ」　　現在の行をスキップする\r\n");
		strMsgALL = strMsgALL + _T("　「キャンセル」処理を中断する\r\n");

		// メッセージボックスの表示
		nRet = ICSMessageBox(strMsgALL, (MB_ICONWARNING | MB_YESNOCANCEL), 0, 0, pWnd);
	}
// midori 156936 add <--
	else	{
		// 各項目のエラーメッセージを取得
		if(nErrorCode == IMP_ERRCODE_IMPORT_ITEM)	{
			strMsg.Format(_T("　\"項目タイトル\"%s（最大 8 文字）※\r\n"),IMP_ERRMSG_ITM_OVER_LENGTH);
		}
		else	{
			strMsg = GetErrorMessage();
		}

		// 1行分スペースを空ける
		strMsgALL = strMsgALL + _T("\r\n");
		strMsgALL.Format(strMsgALL + _T("%d 行目のデータに取り込めない項目があります。\r\n"),m_uImpCsvInfo.nLine);
		strMsgALL = strMsgALL + strMsg + _T("\r\n");
		strMsgALL = strMsgALL + _T("取り込めない項目を除いてインポートしますか？\r\n");
		strMsgALL = strMsgALL + _T("　「はい」　　　取り込めない項目を除いてインポートする\r\n");
		strMsgALL = strMsgALL + _T("　「いいえ」　　現在の行をスキップする\r\n");
		strMsgALL = strMsgALL + _T("　「キャンセル」処理を中断する\r\n");
		if(strMsg.Find(_T("※")) != -1)	{
			strMsgALL = strMsgALL + _T("「はい」を選択時、※印の項目は最大文字数までを取り込みます。\r\n");
		}
		// メッセージボックスの表示
		nRet = ICSMessageBox(strMsgALL, (MB_ICONWARNING | MB_YESNOCANCEL), 0, 0, pWnd);
	}
	return(nRet);
}

// ------------------------------------------------------------------------------------------------------------------------
//	項目のエラーマップ(m_ErrMap)からデータを取得し、エラーメッセージを作成
// 
//	引数	無し
//	戻値
//			ICSMessageBoxの戻り値
//			※項目エラーの場合、はい→IDNO、いいえ→IDCANCELを戻り値をして返す
// ------------------------------------------------------------------------------------------------------------------------
CString CimpUc000Common::GetErrorMessage(void)
{
	int				ii;
	CString			strMsgONE		= _T("");		// メッセージ用バッファ
	CString			strMsg			= _T("");		// 各項目のメッセージ
	CString			strMsgClm;						// 項目名
	IMP_CSV_INFO	csvinfo;						// CSVの情報

	// 各項目のエラーメッセージを取得
	for(ii=0; ii<(int)m_ErrMap.GetCount(); ii++)	{
		m_ErrMap.Lookup(ii,csvinfo);
		// 項目名の取得
		strMsgClm = csvinfo.szColumn;
		if(!strMsgClm.IsEmpty())	strMsgClm = _T("\"") + strMsgClm + _T("\"");
		// nKind → 種類(0:文字 1:金額 2:面積等)
		strMsgONE.Format(_T("%s"),csvinfo.szErrType);
		// 最大文字数のエラーの場合
		if(strMsgONE == IMP_ERRMSG_ITM_OVER_LENGTH && csvinfo.nKind != 0)	{
			// 文字以外はメッセージを変える
			strcpy_s(csvinfo.szErrType,sizeof(csvinfo.szErrType),IMP_ERRMSG_ITM_OVER_LENGTH_N);
		}
		// 項目名、エラー内容をセット
		strMsg.Format(strMsg + _T("　%s %s"),strMsgClm,csvinfo.szErrType);
		// エラー内容が「最大文字(桁)数を超えています。」の場合は、文字(桁)数を表示する
		strMsgONE.Format(_T("%s"),csvinfo.szErrType);
		if(strMsgONE == IMP_ERRMSG_ITM_OVER_LENGTH || strMsgONE == IMP_ERRMSG_ITM_OVER_LENGTH_N)	{
			// 0:文字
			if(csvinfo.nKind == 0)		{
				strMsgONE.Format(_T("（最大 %d 文字）"),csvinfo.nLength);
				// 口座番号は取り込み不可の場合、文字列をカットして取り込まないため「※」を付けない
				if(memcmp(csvinfo.szColumn,CSV_011_ITEM_04,strlen(csvinfo.szColumn)) != 0 )	{
					strMsgONE = strMsgONE + _T("※");
				}
			}
			// 1:金額
			else if(csvinfo.nKind == 1)	{
				if(csvinfo.nType == 0)	{
					strMsgONE.Format(_T("（正数最大 %d 桁）"),csvinfo.nLength);
				}
				else	{
					strMsgONE.Format(_T("（負数最大 %d 桁）"),csvinfo.nLength);
				}
			}
			// 2:面積等
			else								{
				if(csvinfo.nType2 == 0)	{
					if(csvinfo.nType == 0)	{
						strMsgONE.Format(_T("（整数最大 %d 桁）"),csvinfo.nLength);
					}
					else	{
						strMsgONE.Format(_T("（整数[負]最大 %d 桁）"),csvinfo.nLength);
					}
				}
				else if(csvinfo.nType2 == 1)	{
					strMsgONE.Format(_T("（小数点以下最大 %d 桁）"),csvinfo.nLength2);
				}
				else	{
					if(csvinfo.nType == 0)	{
						strMsgONE.Format(_T("（整数最大 %d 桁）、（小数点以下最大 %d 桁）"),csvinfo.nLength,csvinfo.nLength2);
					}
					else	{
						strMsgONE.Format(_T("（整数[負]最大 %d 桁）、（小数点以下最大 %d 桁）"),csvinfo.nLength,csvinfo.nLength2);
					}
				}
			}
			strMsg = strMsg + strMsgONE;
		}
		strMsg = strMsg + _T("\r\n");
	}

	return(strMsg);
}
// midori 180404 add <--

//インポートオプション時における改頁処理
//処理はCfrmUc000Common::OnPageInsert()とほぼ同じ
int CimpUc000Common::ImpPageNew()
{
	int nTargetPage, nTargetRow, nTargetCount;

	if ( m_pUcCmn->GetNumPage() > 0 ) {
		nTargetPage = m_pUcCmn->GetNumPage();
	}
	else {
		nTargetPage = 1;		// 帳表の初期表示でレコード無しなら１頁目挿入する対応
	}
	nTargetRow = 1;
	nTargetCount = GetRowMaxData();

	m_NumPage = nTargetPage + 1;

	return 0;
}

//インポートオプション時における改行処理
//改頁を行わない場合はこちら
int CimpUc000Common::ImpRowNew(int nFormSeq, CDatabase* pDB)
{
	// 頁計の削除、累計の削除
	m_pUcCmn->DeleteFgFunc(ID_FGFUNC_PAGEKEI);
	m_pUcCmn->DeleteFgFunc(ID_FGFUNC_RUIKEI);

	// 最大頁、最大行の取得
	m_pUcCmn->RequeryLastNotNullLine();

	if( m_pUcCmn->GetRecordCount() == 0 ){ // その様式が全て空行の場合
//		m_pUcCmn->Requery();
		m_pUcCmn->DeleteVisualRecord();

		m_NumPage = 1;
		m_NumRow  = 1;
	}
	else{
		CDBVariant	val;
		m_pUcCmn->GetFieldValue( _T("NumPage"), val);
		m_NumPage = val.m_iVal;

		val.Clear();

		m_pUcCmn->GetFieldValue( _T("NumRow"), val);
		m_NumRow = val.m_iVal;

		// データ行の間の空行を削除しないようにするため、頁、行で昇順ソート
		m_pUcCmn->RequerySortPageRow();

		// 指定頁(ここでは最大頁)の最大行以降の空行削除
		m_pUcCmn->DeleteNullLinePageRowAfterAll(m_NumPage, m_NumRow);
	}

	return 0;
}

int CimpUc000Common::ImpUpdatePageNumRowNumAdd(int inPage, int inRow, int inAdd)
{
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = TableCountMaxRow();

	// 頁番、行番振り直し
	m_pUcCmn->UpdatePageNumRowNumAdd(inPage, inRow, inAdd, nBaseRow);

	return retVal;
}

int CimpUc000Common::TableCountMaxRow()
{
	int		nBaseRow = m_RowMaxData;

	// ベースになる行数を決める
	// 「計なし」帳表で、計設定が「オフ」または「累計のみ」の場合は、「計なし」の場合の行数をセットする
	if ((m_RowKei == 0) && ((m_pUcInfSub->m_OutKei == ID_OUTKEI_OFF) || (m_pUcInfSub->m_OutKei == ID_OUTKEI_RUIKEI))) {
		nBaseRow = m_RowMax;
	}

	return nBaseRow;
}

int CimpUc000Common::TableInsertNullLine(int nPage, int nRow, int nLine)
{
	int					i = 0;
	int					nCountPage = nPage;
	int					nCountRow = nRow;

	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0 || nLine <= 0) {
		return FUNCTION_NG;
	}

	// 帳表毎のテーブルクラスを"New"する
	//CdbUc000Common*		rsData;
	//rsData = virGetDataTableObject();

	// ｎ件分のレコード追加
	for (i = 0; i < nLine; i++){
		// 新規レコード追加
		m_pUcCmn->CreateNewRecord();				// レコード初期化済み

		// 空行データとする
		m_pUcCmn->Edit();
		m_pUcCmn->m_NumPage	= nCountPage;			// ページ番号
		m_pUcCmn->m_NumRow	= nCountRow;			// 行番号
		m_pUcCmn->m_FgFunc	= ID_FGFUNC_NULL;		// 特殊行フラグ
		m_pUcCmn->m_FgShow	= ID_FGSHOW_OFF;		// 表示フラグ

		// レコード更新
		m_pUcCmn->Update();

		// 次の頁と行を取得
		GetNextPageRow(&nCountPage, &nCountRow);
	}

	return FUNCTION_OK;
}

int CimpUc000Common::TableUpdatePageNumRowNumReset(int inPage, int inRow)
{
	int					intNumPage;
	int					intNumRow;
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = TableCountMaxRow();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	m_pUcCmn->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	intNumPage = inPage;
	intNumRow = inRow;

	// 頁番、行番振り直し
	while (!m_pUcCmn->IsEOF()) {

		// パラメータの頁番号、行番号以上のレコードから番号を振り直す
		if ((m_pUcCmn->m_NumPage > inPage) || ((m_pUcCmn->m_NumPage == inPage) && (m_pUcCmn->m_NumRow >= inRow))) {
			m_pUcCmn->Edit();
			m_pUcCmn->m_NumPage = intNumPage;		// 頁番号をセット
			m_pUcCmn->m_NumRow = intNumRow;			// 行番号をセット
			m_pUcCmn->Update();
		}

		// 頁番号と行番号をインクリメント
		if (intNumRow >= nBaseRow) {
			intNumPage++;
			intNumRow = 1;
		}
		else {
			intNumRow++;
		}

		// 次のレコードへ
		m_pUcCmn->MoveNext();
	}

	return retVal;
}

//int CimpUc000Common::TableInsertPageKeiRuiKei()
//{
//	//CdbUc000Common*		rs;					// 全レコード取得用
//	int			nSetNumPage;				// 頁番号保存変数
//	int			nSetNumRow;					// 行番号保存変数
//	int			retVal = FUNCTION_NG;
//
//	retVal = FUNCTION_OK;
//
//	//rs = virGetDataTableObject();
//
//	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
//	m_pUcCmn->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
//
//	// 1頁目の1行目からチェックする。
//	nSetNumPage = 1;
//	nSetNumRow = 1;
//
//	// Step1：先頭頁の先頭行から順に参照して、頁計/累計行を挿入する
//	switch (m_pUcInfSub->m_OutKei) {
//	case ID_OUTKEI_OFF:		// なし
//	case ID_OUTKEI_PAGEKEI:	// 頁計
//	case ID_OUTKEI_RUIKEI:	// 累計
//	case ID_OUTKEI_BOTH:	// 累計と累計
//
//		// 計行、頁計行、累計行を挿入していく。
//		while (!m_pUcCmn->IsEOF()) {
//			// 頁計を挿入する条件式
//			if (nSetNumRow > m_RowMaxData) {
//				if (m_RowKei == 1) {
//					// 計あり帳表
//					switch (m_pUcInfSub->m_OutKei) {
//					case ID_OUTKEI_OFF:		// なし
//					case ID_OUTKEI_RUIKEI:	// 累計のみを挿入する
//						// 頁計なしの場合、"計"文字をセット
//						TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
//						break;
//
//					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
//					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
//						// 頁計をセット
//						TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
//						break;
//					}
//				}
//				else {
//					// 計なし帳表
//					switch (m_pUcInfSub->m_OutKei) {
//					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
//					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
//						// 頁計をセット
//						TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
//						break;
//					}
//				}
//				nSetNumPage++;		// 頁番号をインクリメント
//				nSetNumRow = 1;		// 行番号を先頭に戻す
//			}
//			else {
//				m_pUcCmn->MoveNext();
//				nSetNumRow++;		// 行番号をインクリメント
//			}
//		}
//		break;
//	}
//
//	// Step2：最終頁の空行挿入＋頁計/累計作成
//	switch (m_pUcInfSub->m_OutKei) {
//	case ID_OUTKEI_OFF:		// 計なし
//		// 計あり帳表：最終行は合計行とする
//		// 計なし帳表：最終行まで空行で埋める
//		while (nSetNumRow <= m_RowMax) {
//			if ((nSetNumRow == m_RowMax) && (m_RowKei == 1)) {
//				// 最終行に合計行を挿入
//				TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
//			}
//			else {
//				// 空行挿入
//				TableInsertNullLine(nSetNumPage, nSetNumRow, 1);
//			}
//			nSetNumRow++;		// 行番号をインクリメント
//		}
//		break;
//
//	case ID_OUTKEI_PAGEKEI:	// 頁計
//		// 計あり帳表，計なし帳表共に、最終行は合計行とする
//		while (nSetNumRow <= m_RowMax) {
//			if (nSetNumRow == m_RowMax) {
//				// 最終行に合計行を挿入
//				TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
//			}
//			else {
//				// 空行挿入
//				TableInsertNullLine(nSetNumPage, nSetNumRow, 1);
//			}
//			nSetNumRow++;		// 行番号をインクリメント
//		}
//		break;
//
//	case ID_OUTKEI_RUIKEI:	// 累計
//		// 計あり帳表：最終行は合計行とする
//		// 計なし帳表：最終行は合計行とする。また最終行がデータ行なら次頁を空行で埋め最終行を累計とする
//		if (m_RowKei == 1) {
//			// 計あり帳表
//			while (nSetNumRow <= m_RowMax) {
//				if (nSetNumRow == m_RowMax) {
//					// 最終行に合計行を挿入
//					TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
//				}
//				else {
//					// 空行挿入
//					TableInsertNullLine(nSetNumPage, nSetNumRow, 1);
//				}
//				nSetNumRow++;		// 行番号をインクリメント
//			}
//		}
//		else {
//			// 計なし帳表：最終データが空行なら削除する（最終行の条件を付けるべきかも）
//			if (m_pUcCmn->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
//				m_pUcCmn->MovePrev();
//				if (!m_pUcCmn->IsEOF()) {
//					if ((m_pUcCmn->m_FgFunc == 0) && (m_pUcCmn->m_FgShow == 0)) {
//						//rs->Delete();
//						nSetNumRow--;
//					}
//				}
//			}
//			// すでに最終行がデータで埋まっている場合は次頁に累計行を入れる
//			if (nSetNumRow > m_RowMax) {
//				nSetNumPage++;			// 頁番号をインクリメント
//				nSetNumRow = 1;			// 行番号をクリア
//			}
//			// あとは空行で埋め、最終行に合計行を挿入
//			if (nSetNumRow <= m_RowMax) {
//				while (nSetNumRow <= m_RowMax) {
//					if (nSetNumRow == m_RowMax) {
//						// 最終行に合計行を挿入
//						TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
//					}
//					else {
//						// 空行挿入
//						TableInsertNullLine(nSetNumPage, nSetNumRow, 1);
//					}
//					nSetNumRow++;		// 行番号をインクリメント
//				}
//			}
//		}
//		break;
//
//	case ID_OUTKEI_BOTH:	// 頁計と累計
//		// 計あり帳表，計なし帳表共に、データの最後を頁計と累計とする。
//		//	１頁しかない場合は、累計行のみ追加する
//		//	合計行を１行しか挿入できない場合、該当頁は頁計として、次頁に頁計と累計行とする
//		if (m_pUcCmn->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
//			m_pUcCmn->MovePrev();
//			if (!m_pUcCmn->IsEOF()) {
//				if ((m_pUcCmn->m_FgFunc == 0) && (m_pUcCmn->m_FgShow == 0)) {
//					//m_pUcCmn->Delete();
//					nSetNumRow--;
//				}
//			}
//		}
//		// 既に最終行－１がデータで埋まっている場合
//		if (nSetNumRow >= m_RowMax) {
//			if (nSetNumPage == 1) {
//				// 累計を挿入して終了
//				TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
//				nSetNumRow++;			// 行番号をインクリメント
//			}
//			else {
//				// 頁計を挿入して次頁に頁計と累計を出す
//				TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
//				nSetNumPage++;			// 頁番号をインクリメント
//				nSetNumRow = 1;			// 行番号をクリア
//			}
//		}
//		// あとは空行で埋め、最終行－１と最終行に合計行を挿入
//		while (nSetNumRow <= m_RowMax) {
//			if (nSetNumRow == m_RowMax) {
//				// 最終行に累計行を挿入
//				TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
//			}
//			else if ((nSetNumRow == m_RowMax - 1) && (nSetNumPage >= 2)) {
//				// 最終行－１に頁計行を挿入
//				TableInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
//			}
//			else {
//				// 空行挿入
//				TableInsertNullLine(nSetNumPage, nSetNumRow, 1);
//			}
//			nSetNumRow++;		// 行番号をインクリメント
//		}
//	}
//	//delete rs;
//
//	return retVal;
//}

//int CimpUc000Common::TableInsertSpecialLine(int nPage, int nRow, int nType, CString strName)
//{
//	BOOL		bIkkatuLine = FALSE;
//	
//	// パラメータのチェック
//	if (nPage <= 0 || nRow <= 0) {
//		return FUNCTION_NG;
//	}
//	switch ( nType ) {
//	case ID_FGFUNC_SYOKEI:		// 小計行
//	case ID_FGFUNC_SYOKEINULL:	// 小計行（空行）
//	case ID_FGFUNC_CHUKEI:		// 中計行
//	case ID_FGFUNC_CHUKEINULL:	// 中計行（空行）
//	case ID_FGFUNC_RUIKEI:		// 累計行
//	case ID_FGFUNC_PAGEKEI:		// 頁計行
//		bIkkatuLine = FALSE;
//		break;
//	case ID_FGFUNC_IKKATUMANUAL:// 一括集計金額行（手動）
//	case ID_FGFUNC_IKKATUAUTO:	// 一括集計金額行（自動）
//		bIkkatuLine = TRUE;
//		break;
//	default:
//		return FUNCTION_NG;
//	}
//
//	// 帳表毎のテーブルクラスを"New"する
//	//CdbUc000Common*		rsData;
//	//rsData = virGetDataTableObject();
//
//	// 新規レコード追加
//	m_pUcCmn->CreateNewRecord();				// レコード初期化済み
//
//	// 特殊行データ編集（共通データ）
//	m_pUcCmn->Edit();
//	m_pUcCmn->m_NumPage = nPage;				// ページ番号
//	m_pUcCmn->m_NumRow	= nRow;					// 行番号
//	m_pUcCmn->m_FgFunc	= nType;				// 特殊行フラグ
//	m_pUcCmn->m_FgShow	= ID_FGSHOW_OFF;		// 表示フラグ
//
//	// 特殊行の名称
//	if (bIkkatuLine == TRUE) {
//		CALCKEI_INFO		uCalcKei;		// 実は未使用
//		SORTKAMOKU_INFO		uKamoku;		// 実は未使用
//		virTableEditIkkatuLine(2, m_pUcCmn, strName, uCalcKei, uKamoku);
//	}
//	else {
//		m_pUcCmn->m_KeiStr = strName;
//	}
//
//	// レコード更新
//	m_pUcCmn->Update();
//
//	return FUNCTION_OK;
//}

int CimpUc000Common::GetNextPageRow(int* pPage, int* pRow)
{
	// 次の行は改行が必要？
	if (*pRow + 1 > m_RowMaxData) {
		*pPage = *pPage + 1;
		*pRow = 1;
	}
	else {
		*pRow = *pRow + 1;
	}
	
	return	FUNCTION_OK;
}

int CimpUc000Common::TableCalcKei()
{
	//CALCKEI_INFO	uCalcInfo;
	//CString		strSyoKei[CALCKEI_MAXCOUNT];		// 小計計算用
	//CString		strChuKei[CALCKEI_MAXCOUNT];		// 中計計算用
	//CString		strPageKei[CALCKEI_MAXCOUNT];		// 頁計計算用
	//CString		strRuiKei[CALCKEI_MAXCOUNT];		// 累計計算用
	//CString		strTemp = "";
	//int			intMax;
	//int			retVal = FUNCTION_OK;
	//int			i = 0;

	//uCalcInfo = m_uInfo.uCalcInfo;
	//intMax = uCalcInfo.intMaxCount;

	//// 帳表毎のテーブルクラスを"New"する
	////CdbUc000Common*		rs;
	////rs = virGetDataTableObject();				// 全レコード取得用
	//CdbUc000Common*		rsData;
	//rsData = virGetDataTableObject();			// 集計値更新用：Field名称可変で集計値を更新するため

	//// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	////rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	//m_pUcCmn->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);


	//// 初期化
	//for (int i = 0; i < CALCKEI_MAXCOUNT; i++) {
	//	uCalcInfo.strKei[i] = "";

	//	strSyoKei[i].Format("%d", 0);
	//	strChuKei[i].Format("%d", 0);
	//	strPageKei[i].Format("%d", 0);
	//	strRuiKei[i].Format("%d", 0);
	//}

	//// 全レコードに対し１行目から計算していく
	//while (!m_pUcData->IsEOF()) {
	//	switch (m_pUcData->m_FgFunc) {
	//	case ID_FGFUNC_SYOKEI:		// 小計値をセット
	//		// 金額欄へ登録＋小計値を初期化
	//		for (int i = 0; i < intMax; i++) {
	//			uCalcInfo.strKei[i] = strSyoKei[i].TrimLeft();		// 集計値で更新
	//			strSyoKei[i].Format("%d", 0);						// クリア
	//		}
	//		rsData->UpdateCalcKei(m_pUcData->m_Seq, uCalcInfo);			// テーブルへ登録
	//		break;

	//	case ID_FGFUNC_SYOKEINULL:
	//		// 小計値を初期化
	//		for (int i = 0; i < intMax; i++) {
	//			strSyoKei[i].Format("%d", 0);						// クリア
	//		}
	//		break;

	//	case ID_FGFUNC_CHUKEI:		// 中計値をセット
	//		// 金額欄へ登録＋中計値を初期化
	//		for (int i = 0; i < intMax; i++) {
	//			uCalcInfo.strKei[i] = strChuKei[i].TrimLeft();		// 集計値で更新
	//			strChuKei[i].Format("%d", 0);						// クリア
	//		}
	//		rsData->UpdateCalcKei(m_pUcData->m_Seq, uCalcInfo);			// テーブルへ登録
	//		break;

	//	case ID_FGFUNC_CHUKEINULL:
	//		// 中計値を初期化
	//		for (int i = 0; i < intMax; i++) {
	//			strChuKei[i].Format("%d", 0);						// クリア
	//		}
	//		break;

	//	case ID_FGFUNC_PAGEKEI:		// 頁計値をセット
	//		// 金額欄へ登録＋頁計値を初期化
	//		for (int i = 0; i < intMax; i++) {
	//			switch (m_uInfo.intOutKei) {
	//			case ID_OUTKEI_OFF:			// なし
	//			case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
	//				uCalcInfo.strKei[i] = _T("");
	//				break;
	//			case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
	//			case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
	//				uCalcInfo.strKei[i] = strPageKei[i].TrimLeft();;	// 集計値で更新
	//				break;
	//			}
	//			strPageKei[i].Format("%d", 0);						// クリア
	//		}
	//		rsData->UpdateCalcKei(m_pUcData->m_Seq, uCalcInfo);			// テーブルへ登録
	//		break;

	//	case ID_FGFUNC_RUIKEI:		// 累計値をセット
	//		// 金額欄へ登録＋クリアしない
	//		for (int i = 0; i < intMax; i++) {
	//			switch (m_uInfo.intOutKei) {
	//			case ID_OUTKEI_OFF:			// なし
	//			case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
	//				uCalcInfo.strKei[i] = _T("");
	//				break;
	//			case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
	//			case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
	//				uCalcInfo.strKei[i] = strRuiKei[i].TrimLeft();;		// 集計値で更新
	//				break;
	//			}
	//		}
	//		rsData->UpdateCalcKei(m_pUcData->m_Seq, uCalcInfo);			// テーブルへ登録
	//		break;

	//	default:
	//		// 小計、中計、頁計、累計を足しこむ
	//		for (int i = 0; i < intMax; i++) {
	//			// フィールド名称から値を取得
	//			strTemp = m_pUcData->GetFieldValueString(uCalcInfo.strField[i]);
	//			strTemp = strTemp.TrimLeft();

	//			// 加算
	//			CmnKingakuAddFunc(&strSyoKei[i], strTemp);
	//			CmnKingakuAddFunc(&strChuKei[i], strTemp);
	//			CmnKingakuAddFunc(&strPageKei[i], strTemp);
	//			CmnKingakuAddFunc(&strRuiKei[i], strTemp);
	//		}
	//		break;
	//	}

	//	m_pUcData->MoveNext();
	//}

	////rs->Fin();
	//rsData->Fin();

	////delete rs;
	//delete rsData;

	//return retVal;
	return 0;
}

int CimpUc000Common::GetRowMax()
{
	return m_RowMax;
}

int CimpUc000Common::GetRowMaxData()
{
	return m_RowMaxData;
}

int CimpUc000Common::GetRowKei()
{
	return m_RowKei;
}

// virtual関数：各様式で実装する処理
void CimpUc000Common::virTableEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO strSum, SORTKAMOKU_INFO uKamoku)
{
	return;
}

// virtual関数：各様式で実装する処理
int CimpUc000Common::virSpecialLineToNull(int nPage, int MaxRow)
{
	return FUNCTION_OK;
}

// 157766 del -->
//// virtual関数：impUc21Uketoritegataのみで実装
//int CimpUc000Common::virFormatedIsNoKamokuVer(CStringArray* pstrCsvDataAry)
//{
//	return 0;
//}
//
//// 210113_5301_1 add -->
//// virtual関数：CimpUc042Kasituke,CimpUc111Kariirekin,CimpUc131Uriagedakaで実装
//BOOL CimpUc000Common::virFormatedIsNewVer(CStringArray* pstrCsvDataAry)
//{
//	return 0;
//}
//// 210113_5301_1 add <--
// 157766 del <--

// 210113_5301_1 del -->
// ※未使用のため削除
//// virtual関数：impUc21Uketoritegataのみで実装
//int CimpUc000Common::virFormatedIsKamokuAddedVer(CStringArray* pstrCsvDataAry)
//{
//	return 0;
//}
// 210113_5301_1 del <--

int CimpUc000Common::GetRow(int import_count)
{
	return (import_count % m_RowMax) + 1;
}

int CimpUc000Common::GetPage(int row)
{
	return (row / m_RowMax) + 1;
}

// midori 16/05/20 add -->
void CimpUc000Common::CsvInfoClr(IMP_CSV_INFO* pCi)
{
	pCi->nErr		= 0;	// 1:識別子のエラー 2:項目数のエラー
	pCi->nKind		= 0;	// 種類(0:文字 1:金額 2:面積等 3:日付)
	pCi->nLength	= 0;	// 最大文字(桁)数
	pCi->nLength2	= 0;	// 小数点以下の最大桁数(nKind=2のみ)
	pCi->nType		= 0;	// エラーの状態１(0:正数 1:負数)
	pCi->nType2		= 0;	// エラーの状態２(0:整数部 1:小数部 2:両方 nKind=2のみ)
}
// midori 16/05/20 add <--

// midori 180404 add -->
//**************************************************
//	文字列の先頭から指定位置まで取得（２５６バイトまで対応）
//	【引数】	szSrc		…	文字列（元）
//				szDst		…	文字列（先）
//				nPos		…	分割位置までのバイト数
//	【戻値】	指定位置以降の文字列を返す
//**************************************************
CString CimpUc000Common::GetSpritString( CString szSrc, CString* szDst, int nPos )
{
	CString					szRet;					//	戻値
	CString					cs;						//	戻値
	char					szBuf[AS_SPLIT_MAX];	//	バッファ
	char					szBuf1[AS_SPLIT_MAX];	//	バッファ
	char					szBuf2[AS_SPLIT_MAX];	//	バッファ
	const unsigned char*	pBuf = 0;				//	バッファ
	int						nLen;					//	文字列長
	int						nSplitPos;				//	分割位置
	
	//	初期化
	ZeroMemory( szBuf, sizeof( szBuf ) );
	ZeroMemory( szBuf1, sizeof( szBuf1 ) );
	ZeroMemory( szBuf2, sizeof( szBuf2 ) );
	szDst->Empty();
	szRet.Empty(); 

	//	文字列（元）長取得
	nLen = szSrc.GetLength();
	
	//	文字列（元）長か分割位置が最大文字数を超えている？
	if (( nLen > AS_SPLIT_MAX ) ||
		( nPos > AS_SPLIT_MAX )){
			//	抜ける
			return( szRet );
	}

	//	文字列長が分割位置に満たない？
	if ( nLen <= nPos ){
		//	文字列（元）をそのまま取得
		*szDst = szSrc;
	}
	else{
		//	分割位置を取得
		nSplitPos = nPos;
		
		//	分割位置までの文字列を取得
		strcpy_s( szBuf, sizeof( szBuf ), szSrc );

		if(_mbsbtype((LPBYTE)szBuf,nSplitPos) == _MBC_TRAIL)	nSplitPos--;

		memcpy(szBuf1,szBuf,nSplitPos);
		if((int)strlen(szBuf) > nSplitPos)	{
			memcpy(szBuf2,&szBuf[nSplitPos],strlen(szBuf) - nSplitPos);
		}

		//	文字列の先頭から指定位置まで取得
		cs.Format(_T("%s"),szBuf1);
		*szDst = cs;
		//	指定位置以降の文字列を取得
		szRet.Format(_T("%s"),szBuf2);
	}

	//	戻値を返す
	return( szRet );
}
// midori 180404 add <--

// midori 156937 add -->
// -------------------------------------------------------------------------------------------------------
//	引数							int			フォームシーケンス
//									int			0:科目行挿入無し		1:科目行挿入済み
//									CString		小計行の名称
//									CString		小計行のデフォルト名称（科目名称無し）
//									CString		科目名称
//									CString		返送値（小計名称）
// 157154 del -->
//// 戻り値							int			0:デフォルトの小計名称と同じ	1:デフォルトの小計名称と異なる
// 157154 del <--
// 157154 add -->
// 戻り値							int			0固定		未使用
// 157154 add <--
// -------------------------------------------------------------------------------------------------------
int CimpUc000Common::GetKamokuKeiStrMain(int nFormSeq,int pKmkRowSw,CString szSrc,CString strDefSyokei, CString sKamoku, CString* szDst)
{
	int		ret = 0;
	int		pos = 0;
	int		index=0;
	int		nLParen=0;
	int		nRParen=0;
	CString strTmpSy=_T("");
	CString strBuf=_T("");
	CString strBuf2=_T("");

	szDst->Empty();

	// 科目が挿入されていない
	if(pKmkRowSw == 0) {
		// デフォルトの小計名称（科目名付き）を取得する
		GetKamokuKeiStr(strDefSyokei,sKamoku,&strBuf);
// 210113_5301_1 add -->
// 【小計】名称を手入力できるようにしたときのバグ
// 科目名称を含めた小計名称が１３文字を超える場合、改行されずに取り込まれる
		strTmpSy = strBuf;
		m_clsFunc.KeiStrSprit(strTmpSy,&strBuf,&strBuf2);
		if(strBuf2.IsEmpty() == FALSE)	strBuf = strBuf + _T("\r\n") + strBuf2;
// 210113_5301_1 add <--
// 157154 del -->
		//// 小計名称が入力されていない
		//if(szSrc.IsEmpty() == TRUE) {
		//	// デフォルトの小計名称（科目名付き）をセットする
		//	*szDst = strBuf;
		//	ret = 0;
		//}
		//// 小計名称が入力されている
		//else {
		//	// デフォルトの小計名称（科目名無し）とCSVファイルに入力されている小計名称を比較
		//	// 科目名称が一致
		//	if(szSrc.Compare(strDefSyokei) == 0) {
		//		// デフォルトの小計名称（科目名付き）をセットする
		//		*szDst = strBuf;
		//		ret = 0;
		//	}
		//	else {
		//		// デフォルトの小計名称（科目名付き）とCSVファイルに入力されている小計名称を比較
		//		// 科目名称が一致
		//		if(szSrc.Compare(strBuf) == 0) {
		//			// デフォルトの小計名称（科目名付き）をセットする
		//			*szDst = strBuf;
		//			ret = 0;
		//		}
		//		// 科目名称が不一致
		//		else {
		//			// CSVファイルに入力された小計名称をセットする
		//			*szDst = szSrc;
		//			ret = 1;
		//		}
		//	}
		//}
// 157154 del <--
// 157154 add -->
		// デフォルトの小計名称（科目名付き）をセットする
		*szDst = strBuf;
		ret = 0;
// 157154 add <--
	}
	// 科目が挿入されている
	else {
		// 小計名称にデフォルトの科目名称をセットする
		*szDst = strDefSyokei;
		ret = 0;
	}
	return(ret);
}
// midori 156937 add <--

// midori 156189,156190,156191 add -->
// -------------------------------------------------------------------------------------------------------
// 引数で渡された小計名に科目名称を追加する
// -------------------------------------------------------------------------------------------------------
void CimpUc000Common::GetKamokuKeiStr(CString szSrc,CString sKamoku, CString* szDst)
{
	int				tp = 0;
	int				ii = 0;
	int				nbf = 0;
	int				nbf2 = 0;
	int				nLParen=0;
	int				nRParen=0;
	char			buf[128]={0};
	CString			sykakkoname = _T("");
	CString			strTmpSy;		// 小計名称

	szDst->Empty();

	// 小計名称が未入力の場合処理を行わない
	if(szSrc.IsEmpty() == TRUE)	{
		return;
	}

	*szDst = szSrc;
	// 科目名称が未入力の場合処理を行わない
	if(sKamoku.IsEmpty() == TRUE)	{
		return;
	}

	// 既に科目名称が入力されている場合、科目名称の追加を行わない
	strTmpSy = szSrc;
	// １文字目が "(" の場合、"(" と ")" を削除する
	if(strTmpSy.Left(2) == _T("("))	{
		strTmpSy.Delete(0,1);
		strTmpSy.Delete(strTmpSy.GetLength() - 1,1);
	}

	// "(" と ")" の位置を検索する
	nLParen = strTmpSy.Find(_T("("));
	nRParen = strTmpSy.Find(_T(")"));
	// 存在する場合、科目名称ありとして処理を行わない
	if(nLParen != -1 && nRParen != -1)	{
		return;
	}

	// 小計に科目名称を追加する
	memset(buf,0,128);
	strcpy_s(buf,128,szSrc);
	// 右の括弧を取得
	tp = 1;
	if (_mbsbtype((LPBYTE)buf, (szSrc.GetLength() - 1)) == _MBC_TRAIL) {
		tp++;
	}
	sykakkoname = szSrc.Right(tp);

	nbf = szSrc.GetLength();
	nbf2 = 0;
	for (ii = 0; ii < ID_BRACKET_MAX; ii++) {
		// 右括弧を比較
		if (sykakkoname.Compare(BRACKET_KIND[ii].szRight) == 0) {
			nbf2 = sykakkoname.GetLength();
		}
		if (nbf2 > 0) break;
	}

	// 小計の括弧あり
	if(nbf2 > 0) {
		// 小計名をセット
		strTmpSy = szSrc;
		// 小計名称の後ろの位置を取得
		nbf = nbf - nbf2;
		// 小計名称後ろの括弧をカット
		strTmpSy.Delete(nbf,nbf2);
		// 小計名称に科目名を追加
		*szDst = strTmpSy + _T("(") + sKamoku + _T(")") + sykakkoname;
	}
	return;
}
// midori 156189,156190,156191 add <--

// midori 157044 add -->
// -------------------------------------------------------------------------------------------------
// 小計行、または中計行のデータをクリアする
// （科目行、小計行の名称変更が可能の場合は、様式②、⑪は計名称欄もクリアしない)
//※現在未使用
// -------------------------------------------------------------------------------------------------
void CimpUc000Common::CheckKeiStr(CDatabase* pDB,int nFormSeq,CStringArray* pstrCsvDataAry)
{
	int		ii=0;
	int		keicol1=0;
	int		keicol2=0;
	int		sw=0;
	CString strID = _T("");
	CString cst = _T("");

	// 項目が無い場合は処理を行わない
	if(pstrCsvDataAry->GetSize() <= 0) return;

	strID = pstrCsvDataAry->GetAt(0);

	// 小計行、中計行、科目行でない場合は処理を行わない
	if(strID != CSV_ID_SYOKEI && strID != CSV_ID_CHUKEI && strID != CSV_ID_KAMOKU)	return;

	if(nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111) {
		// 科目行、小計行の名称変更が可能
		//sw = ImpKamokuRowEnableSgn(pDB,0);		// 改良No.21-0086,21-0529 del
		sw = ImpKamokuRowEnableSgn(pDB,0,nFormSeq);	// 改良No.21-0086,21-0529 add
		if(sw > 0) {
// 157154 del -->
//// midori 157046 del -->
//			//if(strID == CSV_ID_SYOKEI || strID == CSV_ID_KAMOKU) {
//// midori 157046 del <--
//// midori 157046 add -->
//			if(strID == CSV_ID_SYOKEI || strID == CSV_ID_CHUKEI || strID == CSV_ID_KAMOKU) {
//// midori 157046 add <--
// 157154 del <--
// 157154 add -->
			if(strID == CSV_ID_CHUKEI || strID == CSV_ID_KAMOKU) {
// 157154 add <--
				if (nFormSeq == ID_FORMNO_021) {
					if (pstrCsvDataAry->GetCount() == CSV_021_ITEM_CNT) {
						keicol1 = 2;
						keicol2 = 3;
					}
					else {
						keicol1 = 1;
						keicol2 = 2;
					}
				}
				else {
					keicol1 = 2;
					keicol2 = 3;
				}
			}
		}
	}

	for(ii=1; ii<(int)pstrCsvDataAry->GetSize(); ii++) {
		cst.Empty();
		if(ii != keicol1 && ii != keicol2) {
			pstrCsvDataAry->SetAt(ii,cst);
		}
	}
}
// midori 157044 add <--

// 157766 add -->
// ------------------------------------------------------------------------------------------
//	「科目」が追加されておらず、かつ正しい項目のCSVデータかチェックする
// 　（CimpUc021Uketoritegata から持ってきた）
// ------------------------------------------------------------------------------------------
//		IN		CStringArray*		CSV1行分のデータ
//	戻り値		BOOL				TRUE	科目が追加されておらず、かつ項目が正しい
//									FALSE	上記以外
// ------------------------------------------------------------------------------------------
BOOL CimpUc000Common::FormatedIsNoKamokuVer( CStringArray* pstrCsvDataAry )
{
	BOOL	nRet = TRUE;
	CString strItem;

	for(int i=1/*[項目]読み飛ばし*/; i<pstrCsvDataAry->GetCount(); i++){
		strItem.Empty();
		switch(i){
			case 0:
				break;
			case 1:
				strItem		+= CSV_021_ITEM_02;	break;
			case 2:
				strItem		+= CSV_021_ITEM_03;	break;
			case 3:
				strItem		+= CSV_021_ITEM_04;	break;
			case 4:
				strItem		+= CSV_021_ITEM_05;	break;
			case 5:
				strItem		+= CSV_021_ITEM_06;	break;
			case 6:
				strItem		+= CSV_021_ITEM_07;	break;
			case 7:
				strItem		+= CSV_021_ITEM_08;	break;
			case 8:
				strItem		+= CSV_021_ITEM_09;	break;
			case 9:
				strItem		+= CSV_021_ITEM_10;	break;
			case 10:
				strItem		+= CSV_021_ITEM_11;	break;
			case 11:
				strItem		+= CSV_021_ITEM_12;	break;
		}
		CString hoge = pstrCsvDataAry->GetAt(i);

		if( pstrCsvDataAry->GetAt(i).Compare(strItem) != 0 ){
			nRet = FALSE;
			break;
		}
	}

	return nRet;
}

// 改良No.21-0086,21-0529 add -->
// ------------------------------------------------------------------------------------------
//	「科目」が追加されておらず、かつ正しい項目のCSVデータかチェックする
// 　（CimpUc081Siharaitegata）
// ------------------------------------------------------------------------------------------
//		IN		CStringArray*		CSV1行分のデータ
//	戻り値		int					1	科目が追加されておらず、かつ項目が正しい
//									-1	科目が追加されておらず、かつ項目が正しくない
//									0	上記以外
// ------------------------------------------------------------------------------------------
int CimpUc000Common::FormatedIsNoKamokuVer2(CStringArray* pstrCsvDataAry)
{
	int			ii = 0;
	int			nRet = 1;
	CString		strItem;
	CString		hoge;

	for(ii = 1/*[項目]読み飛ばし*/; ii<pstrCsvDataAry->GetCount(); ii++)	{
		strItem.Empty();
		switch(ii) {
			case 0:
				break;
			case 1:
				strItem = CSV_081_ITEM_10;		break;
			case 2:
				strItem = CSV_081_ITEM_01;		break;
			case 3:
				strItem = CSV_081_ITEM_02;		break;
			case 4:
				strItem = CSV_081_ITEM_03;		break;
			case 5:
				strItem = CSV_081_ITEM_04;		break;
			case 6:
				strItem = CSV_081_ITEM_05_NEW;	break;
			case 7:
				strItem = CSV_081_ITEM_06_NEW;	break;
			case 8:
				strItem = CSV_081_ITEM_07;		break;
			case 9:
				strItem = CSV_081_ITEM_08;		break;
			case 10:
				strItem = CSV_081_ITEM_09;		break;
		}
		hoge.Empty();
		hoge = pstrCsvDataAry->GetAt(ii);

		if(pstrCsvDataAry->GetAt(ii).Compare(strItem) != 0) {
			nRet = 0;
		}
	}

	if(nRet == 0 && ii <= CSV_081_ITEM_CNT) {
		nRet = -1;
	}

	return nRet;
}
// 改良No.21-0086,21-0529 add <--

// ------------------------------------------------------------------------------------------
//	項目数のチェック
// ------------------------------------------------------------------------------------------
//	引数			BOOL			TRUE:登録番号（法人番号）追加マスター	FALSE:簡素化マスター
//					BOOL			TRUE:簡素化マスター						FALSE:旧様式マスター
//					int				フォームシーケンス
//					CStringArray*	CSV1行分のデータ
//	戻り値			int				IMP_ERRCODE_SUCCESS		正常データ
//									IMP_ERRCODE_NUMOFITEM	エラーデータ
// ------------------------------------------------------------------------------------------
// 登録番号（法人番号）_23/11/21 cor -->
//int	CimpUc000Common::NumberOfItemCheck( BOOL bKanso, int nFormSeq, CStringArray* pstrCsvDataAry )
// -------------------------------------
int	CimpUc000Common::NumberOfItemCheck( BOOL bThno, BOOL bKanso, int nFormSeq, CStringArray* pstrCsvDataAry )
// 登録番号（法人番号）_23/11/21 cor <--
{
	int		max=0,ii=0;
	int		csvcnt=0,regcnt=0;
	int		ret=IMP_ERRCODE_SUCCESS;

	max = (int)pstrCsvDataAry->GetSize();
	for(ii=max-1; ii>=0; ii--)	{
		if(pstrCsvDataAry->GetAt(ii).IsEmpty() == FALSE)	{
			break;
		}
	}
	csvcnt = ii + 1;
	if(nFormSeq == ID_FORMNO_011)		regcnt=CSV_011_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_021)	{
		regcnt=CSV_021_ITEM_CNT;
		m_KamokuVer = FormatedIsNoKamokuVer(pstrCsvDataAry);
		if(m_KamokuVer != FALSE)	{
			regcnt--;
		}
	}
	else if(nFormSeq == ID_FORMNO_031)	regcnt=CSV_031_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_041)	regcnt=CSV_041_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_042)	regcnt=CSV_042_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_051)	regcnt=CSV_051_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_061)	regcnt=CSV_061_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_071)	regcnt=CSV_071_ITEM_CNT;
// 改良No.21-0086,21-0529 cor -->
	//else if(nFormSeq == ID_FORMNO_081)	regcnt=CSV_081_ITEM_CNT;
// ------------------------------
	else if(nFormSeq == ID_FORMNO_081)	{
		m_KamokuVer2 = FormatedIsNoKamokuVer2(pstrCsvDataAry);
		if(bG_InvNo == TRUE && m_KamokuVer2 == 0)	regcnt = CSV_081_ITEM_CNT_K;
		else if(m_KamokuVer2 == -1)					regcnt = CSV_081_ITEM_CNT - 1;
		else										regcnt = CSV_081_ITEM_CNT;
	}
// 改良No.21-0086,21-0529 cor <--
	else if(nFormSeq == ID_FORMNO_091)	regcnt=CSV_091_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_101)	regcnt=CSV_101_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_102)	regcnt=CSV_102_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_111)	regcnt=CSV_111_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_121)	regcnt=CSV_121_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_131)	regcnt=CSV_131_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_141)	regcnt=CSV_141_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_142)	regcnt=CSV_142_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_151)	regcnt=CSV_151_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_152)	regcnt=CSV_152_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_153)	regcnt=CSV_153_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_161)	regcnt=CSV_161_ITEM_CNT;
	else if(nFormSeq == ID_FORMNO_162)	regcnt=CSV_162_ITEM_CNT;
	else if(nFormSeq >= ID_FORMNO_171 && nFormSeq <= ID_FORMNO_1720)	{
		regcnt=CSV_171_ITEM_CNT;
	}
	else	{
		regcnt=CSV_181_ITEM_CNT;
	}
	m_NewVer = bKanso;
// 登録番号（法人番号）_23/11/21 add -->
	m_NewVer2 = bThno;
// 登録番号（法人番号）_23/11/21 add <--
	if(csvcnt < regcnt)	{
		// 既定の列数より少ない場合はエラー
		ret = IMP_ERRCODE_NUMOFITEM;
		if(bKanso != FALSE)	{
			// 但し、新様式マスターの④-2,⑪,⑬に限り新旧どちらのファイル形式にも対応する
// 登録番号（法人番号）_23/11/21 cor -->
			//if(nFormSeq == ID_FORMNO_042 || nFormSeq == ID_FORMNO_111)	{
			//	// ④-2、⑪は2列少ない場合に新様式とみなす
			//	if(csvcnt >= (regcnt - 2))	{
			//		ret = IMP_ERRCODE_SUCCESS;
			//		m_NewVer = TRUE;
			//	}
			//}
// -------------------------------------
			if(nFormSeq == ID_FORMNO_042)	{
				// ④-2は1列少ない場合に登録番号（法人番号）追加様式とみなし
				if(csvcnt >= (regcnt - 1))	{
					ret = IMP_ERRCODE_SUCCESS;
					m_NewVer2 = TRUE;
				}
				// 2列少ない場合に簡素化様式とみなす
				else if(csvcnt >= (regcnt - 2))	{
					ret = IMP_ERRCODE_SUCCESS;
					m_NewVer = TRUE;
					m_NewVer2 = FALSE;
				}
			}
			else if(nFormSeq == ID_FORMNO_111)	{
				// ⑪は2列少ない場合に新様式とみなす
				if(csvcnt >= (regcnt - 2))	{
					ret = IMP_ERRCODE_SUCCESS;
					m_NewVer = TRUE;
				}
			}
// 登録番号（法人番号）_23/11/21 cor <--
			else if(nFormSeq == ID_FORMNO_131)	{
				// ⑬は1列少ない場合に新様式とみなす
				if(csvcnt >= (regcnt - 1))	{
					ret = IMP_ERRCODE_SUCCESS;
					m_NewVer = TRUE;
				}
			}
		}
	}
	else	{
		// 既定の列数より多い場合、定義されている既定の列数は旧様式のものなので
		// ④-2,⑪,⑬はファイル形式サインを旧様式にする
// 231225_168496 cor -->
		//if(nFormSeq == ID_FORMNO_042 || nFormSeq == ID_FORMNO_111 || nFormSeq == ID_FORMNO_131)	{
// ---------------------
		if(nFormSeq == ID_FORMNO_042)	{
			if(bThno == TRUE)	{
				m_NewVer = TRUE;
				m_NewVer2 = TRUE;
			}
			else	{
				m_NewVer = FALSE;
				m_NewVer2 = FALSE;
			}
		}
		else if(nFormSeq == ID_FORMNO_111 || nFormSeq == ID_FORMNO_131)	{
// 231225_168496 cor <--
			m_NewVer = FALSE;
// 登録番号（法人番号）_23/11/21 add -->
			m_NewVer2 = FALSE;
// 登録番号（法人番号）_23/11/21 add <--
		}
// 改良No.21-0086,21-0529 add -->
		else if(nFormSeq == ID_FORMNO_081) {
			if(csvcnt > regcnt) {
				if(m_KamokuVer2 == -1)	m_NewVer2 = FALSE;
				else					m_NewVer2 = TRUE;
			}
			else {
				m_NewVer2 = FALSE;
			}
		}
// 改良No.21-0086,21-0529 add <--
// 登録番号（法人番号）_23/11/21 add -->
		// 以下の様式は1列多い場合に登録番号（法人番号）追加様式とみなす
		switch(nFormSeq)	{
			case	ID_FORMNO_021:
			case	ID_FORMNO_031:
			case	ID_FORMNO_041:
			case	ID_FORMNO_071:
			//case	ID_FORMNO_081:	// 改良No.21-0086,21-0529 del
			case	ID_FORMNO_091:
			case	ID_FORMNO_101:
			case	ID_FORMNO_121:
			case	ID_FORMNO_151:
			case	ID_FORMNO_152:
			case	ID_FORMNO_153:
			case	ID_FORMNO_161:
			case	ID_FORMNO_162:
			case	ID_FORMNO_171:
			case	ID_FORMNO_172:
			case	ID_FORMNO_173:
			case	ID_FORMNO_174:
			case	ID_FORMNO_175:
			case	ID_FORMNO_176:
			case	ID_FORMNO_177:
			case	ID_FORMNO_178:
			case	ID_FORMNO_179:
			case	ID_FORMNO_1710:
			case	ID_FORMNO_1711:
			case	ID_FORMNO_1712:
			case	ID_FORMNO_1713:
			case	ID_FORMNO_1714:
			case	ID_FORMNO_1715:
			case	ID_FORMNO_1716:
			case	ID_FORMNO_1717:
			case	ID_FORMNO_1718:
			case	ID_FORMNO_1719:
			case	ID_FORMNO_1720:
				if(csvcnt > regcnt)	{
					m_NewVer2 = TRUE;
				}
				else	{
					m_NewVer2 = FALSE;
				}
				break;
		}
// 登録番号（法人番号）_23/11/21 add <--
	}

	return(ret);
}
// 157766 add <--
