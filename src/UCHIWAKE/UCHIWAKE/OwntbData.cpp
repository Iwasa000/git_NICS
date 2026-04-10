//----------------------------------------------------------------
//	勘定科目内訳書アプリで使用するowntbデータクラス
//
//
//	2010.07.XX 一括印刷
//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
#include	"StdAfx.h"
#include	"OwntbData.h"

// コンストラクタ
COwntbData::COwntbData(CDBZmSub* pZmSub, CBatchPrt* pBatchPrt)
{
	const CString	FN = _T("COwntbData::COwntbData(CDBZmSub*, CBatchPrt* )");
	CString			strLog;

	DbgWriteLog( FN, _T("START") );

	// 2010/07/XX 一括印刷対応
	// 一括印刷管理クラスのポインタ
	m_pBatchPrt	= pBatchPrt;

	// COwntbDataクラス初期処理（既存コンストラクタ処理）
	// → 通常用のレコードが無い場合は作成
	COwntbDataInit( pZmSub );

	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	// COwntbDataInit() 失敗時 終了
	if ( FALSE == m_fInit ) {
		DbgWriteLog( FN, _T("COwntbDataInit() Error → END") );
		return ;
	}

	// 2010/08/02 一括印刷対応 新規レコード作成救済処置対応
	// 一括印刷用 レコード初期処理
	// → 一括用のレコードが無い場合は作成
	if (( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchExec() )) {
		COwntbDataInitBprt();
	}

	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	//-------------------------------------------------------------------------
	// 一括印刷 詳細設定時 owntb テーブルデータチェック(作成) 正常終了時、
	// 共通ID(=0) 以外のレコードを削除
	//-------------------------------------------------------------------------
	if ( m_fInit && m_pBatchPrt->IsBatchModeSet() ) {
		CString		strItmName = m_pBatchPrt->GetBatchKey();
		CString		strFilter;
		strFilter.Format( _T("apl_name = '%s' AND itm_name = '%s' AND u_id <> %d"), 
								OWNTB_APLNAME, 
								strItmName, 
								TG_COMMON_USER_ID );

		pZmSub->owntb->Requery( strFilter );
		if( pZmSub->owntb->st != -1){
			// 一括印刷用 owntb ユーザID≠0 レコード削除
			if ( FALSE == ClrOwnRecByUID( pZmSub, strItmName ) ) {
				// レコード削除失敗 → 処理継続可能。 → 処理無
				DbgWriteLog( FN, _T("一括印刷用 owntb ユーザID≠0 レコード削除 失敗 → 処理継続"));
			}
		}
	}

	DbgWriteLog( FN, _T("END") );
}

//----------------------------------------------------------------
//	COwntbDataクラス初期処理
//
//	IN:		なし
//
//	RET:	なし
//
//	2010/07/XX	一括印刷対応（既存コンストラクタ処理を関数化）
//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
void COwntbData::COwntbDataInit(CDBZmSub* pZmSub)
{
	// DB財務クラスのポインタ
	m_pZmSub	= pZmSub;

	m_nUserId	= 0;
	m_fInit		= TRUE;

	//--------------------------------
	//	データ構造の初期化
	//--------------------------------
	ZeroMemory(&m_uInfo.uPrtCmn,	sizeof(m_uInfo.uPrtCmn));		// 1000.一括印刷情報（共通）
	ZeroMemory(&m_uInfo.uBPrtCmn,	sizeof(m_uInfo.uBPrtCmn));		// 1001～1004.一括印刷情報（帳表別）

	//--------------------------------
	//	初期処理
	//--------------------------------
	// ユーザID取得
	if (GetUserId() == FALSE) {
		// 初期処理エラー
		m_fInit = FALSE;
		return;
	}

	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	//-------------------------------------------------------------------------
	// 一括印刷 出力/CDRモード起動時
	// owntb の一括印刷用レコードに、下記何れかの ユーザID データの有無チェック
	//	　・共通ユーザID(=0)
	//	　・現在ログイン中の ユーザID 
	//
	// 無い場合、出力処理を中止し、「一括印刷詳細設定の実行誘導メッセージ」を表示する。
	// 運用中マスタの場合、該当ユーザIDレコードが無い場合、一括印刷メイン画面表示と異なる帳表が出力される。
	//-------------------------------------------------------------------------
// 24/01/16_ScoreLink cor -->
	//if (( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchModePrt() || m_pBatchPrt->IsBatchModeCDR() )) {
// --------------------------
	if(m_pBatchPrt && (m_pBatchPrt->IsBatchModePrt() || m_pBatchPrt->IsBatchModeCDR()) && m_pBatchPrt->IsScoreLinkExec() == FALSE) {
// 24/01/16_ScoreLink cor <--
		m_pBatchPrt->ClrDoSetMode();
		// owntb 一括印刷用 ユーザIDレコードチェック（共通ID、ログインID）
		// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
		//BOOL	fRet = CheckRecordBprtUserID();
		//if ( FALSE == fRet ) {
		//	// 該当レコード無し フラグ セット
		//	m_pBatchPrt->SetDoSetMode();
		//	// 初期処理エラー
		//	m_fInit = FALSE;
		//	return;
		//}
		int	nRet = CheckRecordBprtUserID();
		switch (nRet) {
		case OWNTB_CHKREC_NG_DOSET01:
		case OWNTB_CHKREC_NG_DOSET02:
		case OWNTB_CHKREC_NG_DOSET03:
			// 該当レコード無し フラグ セット
			m_pBatchPrt->SetDoSetMode( nRet );
			// 初期処理エラー
			m_fInit = FALSE;
			return;
		default:
			break;
		}
	}
}

//----------------------------------------------------------------
//	一括印刷用レコード作成
//
//	IN:		なし
//
//	RET:	なし
//
//	2010/08/02	一括印刷対応 新規レコード作成救済処置対応
//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
void COwntbData::COwntbDataInitBprt( void )
{
	const CString	FN = _T("COwntbData::COwntbDataInitBprt()");
	DbgWriteLog( FN, _T("START") );
	CString		strFilter = GetFilterString( -1, TG_COMMON_USER_ID );		// 一括印刷用バージョンレコード参照条件
																			// 2011/01/11 修正依頼No.N-037 共通ID のレコード検索
	// テーブルオープンチェック
	if(m_pZmSub->owntb == NULL){
		if ( m_pZmSub->OwnTblOpen( strFilter ) != 0) {						// バージョン情報を取得
			DbgWriteLog( FN, _T("OwnTblOpen() 失敗 抽出条件[") + strFilter + _T("]") );
			DbgWriteLog( FN, _T("m_fInit FALSE セット → return]") );
			m_fInit = FALSE;												// 初期処理失敗をセット
			return;
		}
	}else{
		m_pZmSub->owntb->Requery(strFilter);								// バージョン情報を取得
	}

	// 一括印刷の owntb データが無い場合、作成する。
	// 初めて実行？
	if (m_pZmSub->owntb->st == -1) {
		//--------------------------------------
		// itm_seq = 1000：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN )) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1001：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN1 )) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1002：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN2 )) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1003：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN3 )) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1004：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN4 )) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1005：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN5)) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1006：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN6)) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1007：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN7)) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1008：一括印刷専用 作成
		//--------------------------------------
		if ( FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN8)) {
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

// midori 190505 add -->
		//--------------------------------------
		// itm_seq = 1009：一括印刷専用 作成
		//--------------------------------------
		if(FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN9))	{
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1010：一括印刷専用 作成
		//--------------------------------------
		if(FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN10))	{
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1011：一括印刷専用 作成
		//--------------------------------------
		if(FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN11))	{
			m_fInit = FALSE;												// 初期処理失敗をセット
		}

		//--------------------------------------
		// itm_seq = 1012：一括印刷専用 作成
		//--------------------------------------
		if(FALSE == CreateNewRecord( OWNTB_SEQ_BPRTCMN12))	{
			m_fInit = FALSE;												// 初期処理失敗をセット
		}
// midori 190505 add <--

		// F6項目登録は、通常用の設定値を参照する（設定値 共有 2010/07/30 仕様変更 From ICS）

		if ( FALSE == m_fInit ) {
			DbgWriteLog( FN, _T("一括印刷用 レコード作成失敗  m_fInit FALSE セット"));
		}
	}//はじめて実行？

	DbgWriteLog( FN, _T("END") );
}

// デストラクタ
COwntbData::~COwntbData(void)
{
}

//----------------------------------------------------------------
//	データ読み込み(全て)
//
//	IN:		なし
//
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//----------------------------------------------------------------
BOOL COwntbData::LoadAllData(void)
{
	// 2010/07/XX 一括印刷対応 ログ追加
	const CString	FN = _T("COwntbData::LoadAllData()");
	DbgWriteLog( FN, _T("START"));

	//--------------------------------------------------------
	//	itm_seq=1～999を順番に読み込む
	//	事前チェックは、各Load処理で実施するので省略する
	//--------------------------------------------------------
	// 2010/07/XX 一括印刷対応
	if ( ( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchExec() )) {
		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN"));
		// 1000：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN1"));
		// 1001：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN1) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN2"));
		// 1002：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN2) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN3"));
		// 1003：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN3) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN4"));
		// 1004：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN4) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN5"));
		// 1005：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN5) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN6"));
		// 1006：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN6) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN7"));
		// 1007：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN7) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN8"));
		// 1008：一括印刷用 情報
		if (LoadData(OWNTB_SEQ_BPRTCMN8) == FALSE) {
			return FALSE;
		}

// midori 190505 add -->
		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN9"));
		// 1009：一括印刷用 情報
		if(LoadData(OWNTB_SEQ_BPRTCMN9) == FALSE)	{
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN10"));
		// 1010：一括印刷用 情報
		if(LoadData(OWNTB_SEQ_BPRTCMN10) == FALSE)	{
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN11"));
		// 1011：一括印刷用 情報
		if(LoadData(OWNTB_SEQ_BPRTCMN11) == FALSE)	{
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード取得 OWNTB_SEQ_BPRTCMN12"));
		// 1012：一括印刷用 情報
		if(LoadData(OWNTB_SEQ_BPRTCMN12) == FALSE)	{
			return FALSE;
		}
// midori 190505 add <--

		// その他の内訳書追加により、スワップする（２－①～２－⑤を１－⑳の後ろに差し込む）
		SwapSonotaLoad();
	}

	DbgWriteLog( FN, _T("END"));
	return TRUE;
}

//----------------------------------------------------------------
//	データ読み込み(個別)
//
//	IN:		int				データ種別（itm_seq）
//
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//
//
// 2010.01.XX 社会福祉　資金収支計算書で貸借発生を出力する
// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
BOOL COwntbData::LoadData(int nItmSeq)
{
	// 2010/07/XX 一括印刷対応 ログ追加
	const CString	FN = _T("COwntbData::LoadData()");
	CString			strLog;

	strLog.Format( _T("START  nItmSeq=[%d]"), nItmSeq );
	DbgWriteLog( FN, strLog);

	// 事前チェック
	if (StartCheck(nItmSeq) == FALSE) {
		DbgWriteLog( FN, _T("事前チェックエラー"));
		return FALSE;
	}

	// リクエリー実行
	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化 （共通ID引数追加）
	CString		strFilter = GetFilterString(nItmSeq, TG_COMMON_USER_ID );
	m_pZmSub->owntb->Requery(strFilter);

	// データが見つからない場合は新規レコード登録
	if (m_pZmSub->owntb->st == -1) {

		DbgWriteLog( FN, _T("データ無 → レコード作成"));

		// 2010/07/XX  一括印刷対応
		// 一括印刷時は、itm_seq を指定してコピーする
		if (( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchExec() )) {
			// 一括印刷用 owntb レコード作成
			CString	strBatchKey = m_pBatchPrt->GetBatchKey();				// 依頼キー取得
			BOOL	fRet = FALSE;
			switch ( nItmSeq ) {
			case	OWNTB_SEQ_BPRTCMN:
			case	OWNTB_SEQ_BPRTCMN1:
			case	OWNTB_SEQ_BPRTCMN2:
			case	OWNTB_SEQ_BPRTCMN3:
			case	OWNTB_SEQ_BPRTCMN4:
			case	OWNTB_SEQ_BPRTCMN5:
			case	OWNTB_SEQ_BPRTCMN6:
			case	OWNTB_SEQ_BPRTCMN7:
			case	OWNTB_SEQ_BPRTCMN8:
// midori 190505 add -->
			case	OWNTB_SEQ_BPRTCMN9:
			case	OWNTB_SEQ_BPRTCMN10:
			case	OWNTB_SEQ_BPRTCMN11:
			case	OWNTB_SEQ_BPRTCMN12:
// midori 190505 add <--
				fRet = CreateNewRecord( nItmSeq );
				break;
			}
			// 失敗？
			if ( FALSE == fRet ) {
				return FALSE;
			}
		}

		// 再度リクエリー
		m_pZmSub->owntb->Requery(strFilter);
	}

	// テーブル→変数
	switch (nItmSeq) {
	// 2010/07/XX 一括印刷対応
	case	OWNTB_SEQ_BPRTCMN:
		// 1000.一括印刷用 情報
		m_uInfo.uPrtCmn.nCoverSw				= m_pZmSub->owntb->vl[0];			// 表紙を出力する					→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
		m_uInfo.uPrtCmn.nKsnAddNow				= m_pZmSub->owntb->vl[1];			// 決算期間に「現在」を出力する		→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
// midori 160608 add -->
		m_uInfo.uPrtCmn.nZeroSw					= m_pZmSub->owntb->vl[2];			// 帳票イメージのみ出力する			→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
		// vl[2]の初期値にNULLが入っているので
		if(m_uInfo.uPrtCmn.nZeroSw != 0 && m_uInfo.uPrtCmn.nZeroSw != 1)	{
			m_uInfo.uPrtCmn.nZeroSw = 0;
		}
// midori 160608 add <--
		break;
	case	OWNTB_SEQ_BPRTCMN1:
		// 1001.一括印刷用 情報
		for (int n=0, i=0; n<20; n++, i++) {
			m_uInfo.uBPrtCmn.nOutPut[i]			= atoi(m_pZmSub->owntb->code[n]);	// 各帳表の出力ON/OFF				→ 2010/07/XX 一括印刷対応 itm_seq(=1001) にて保存
			m_uInfo.uBPrtCmn.nOPRange_From[i]	= m_pZmSub->owntb->vl[n];			// 各帳表の出力ページ範囲（From）	→ 2010/07/XX 一括印刷対応 itm_seq(=1001) にて保存
		}
		break;
	case	OWNTB_SEQ_BPRTCMN2:
		// 1002.一括印刷用 情報
		for (int n=0, i=0; n<20; n++, i++) {
			m_uInfo.uBPrtCmn.nOPRange_To[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力ページ範囲（To）		→ 2010/07/XX 一括印刷対応 itm_seq(=1002) にて保存
		}
		break;
	case	OWNTB_SEQ_BPRTCMN3:
		// 1003.一括印刷用 情報
		for (int n=0, i=20; n<12; n++, i++) {
			m_uInfo.uBPrtCmn.nOutPut[i]			= atoi(m_pZmSub->owntb->code[n]);	// 各帳表の出力ON/OFF				→ 2010/07/XX 一括印刷対応 itm_seq(=1003) にて保存
			m_uInfo.uBPrtCmn.nOPRange_From[i]	= m_pZmSub->owntb->vl[n];			// 各帳表の出力ページ範囲（From）	→ 2010/07/XX 一括印刷対応 itm_seq(=1003) にて保存
		}
		break;
	case	OWNTB_SEQ_BPRTCMN4:
		// 1004.一括印刷用 情報
		for (int n=0, i=20; n<12; n++, i++) {
			m_uInfo.uBPrtCmn.nOPRange_To[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力ページ範囲（To）		→ 2010/07/XX 一括印刷対応 itm_seq(=1004) にて保存
		}
		break;
	case	OWNTB_SEQ_BPRTCMN5:
		// 1005.一括印刷用 情報　その他の内訳書１－⑥～１－⑳
		for (int n=0, i=32; n<15; n++, i++) {
			m_uInfo.uBPrtCmn.nOutPut[i]			= atoi(m_pZmSub->owntb->code[n]);
			m_uInfo.uBPrtCmn.nOPRange_From[i]		= m_pZmSub->owntb->vl[n];		// 各帳表の出力ページ範囲（From）	→ 2014/06/XX　その他の内訳書追加対応
		}
		break;
	case	OWNTB_SEQ_BPRTCMN6:
		// 1006.一括印刷用 情報　その他の内訳書１－⑥～１－⑳
		for (int n=0, i=32; n<15; n++, i++) {
			m_uInfo.uBPrtCmn.nOPRange_To[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力ページ範囲（To）		→ 2014/06/XX　その他の内訳書追加対応
		}
		break;
	case	OWNTB_SEQ_BPRTCMN7:
		// 1007.一括印刷用 情報　その他の内訳書２－⑥～２－⑳
		for (int n=0, i=47; n<15; n++, i++) {
			m_uInfo.uBPrtCmn.nOutPut[i]			= atoi(m_pZmSub->owntb->code[n]);
			m_uInfo.uBPrtCmn.nOPRange_From[i]		= m_pZmSub->owntb->vl[n];		// 各帳表の出力ページ範囲（From）	→ 2014/06/XX　その他の内訳書追加対応
		}
		break;
	case	OWNTB_SEQ_BPRTCMN8:
		// 1008.一括印刷用 情報　その他の内訳書２－⑥～２－⑳
		for (int n=0, i=47; n<15; n++, i++) {
			m_uInfo.uBPrtCmn.nOPRange_To[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力ページ範囲（To）		→ 2014/06/XX　その他の内訳書追加対応
		}
		break;

// midori 190505 add -->
	case	OWNTB_SEQ_BPRTCMN9:
		// 1009.一括印刷用 情報
		for (int n=0, i=0; n<20; n++, i++) {
			m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力方法
		}
		break;

	case	OWNTB_SEQ_BPRTCMN10:
		// 1010.一括印刷用 情報
		for (int n=0, i=20; n<12; n++, i++) {
			m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力方法
		}
		break;

	case	OWNTB_SEQ_BPRTCMN11:
		// 1011.一括印刷用 情報
		for (int n=0, i=32; n<15; n++, i++) {
			m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力方法
		}
		break;

	case	OWNTB_SEQ_BPRTCMN12:
		// 1012.一括印刷用 情報
		for (int n=0, i=47; n<15; n++, i++) {
			m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= m_pZmSub->owntb->vl[n];			// 各帳表の出力方法
		}
// midori 190505 add <--
	}

	DbgWriteLog( FN, _T("END"));
	return TRUE;
}

//----------------------------------------------------------------
//	事前チェック処理
//
//	IN:		int				データ種別（itm_seq）
//
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//----------------------------------------------------------------
BOOL COwntbData::StartCheck(int nItmSeq)
{
	try {
		// owntb，zvolがオープン済みである事
		if ((m_pZmSub->owntb == NULL)/* || (m_pZmSub->zvol == NULL)*/) {
			return FALSE;
		}

		// 初期処理が成功している事 ･･･ 但し初期処理中は"TRUE"とする
		if (m_fInit == FALSE) {
			return FALSE;
		}

		// ユーザIDが取得できている事
		if (m_nUserId == 0) {
			return FALSE;
		}

		// itm_seqが正しい値である事
		switch (nItmSeq) {
		case	OWNTB_SEQ_BPRTCMN:			// 1000.一括印刷用 情報	2010/07/XX 一括印刷対応
		case	OWNTB_SEQ_BPRTCMN1:			// 1001.一括印刷用 情報	2010/07/XX 一括印刷対応
		case	OWNTB_SEQ_BPRTCMN2:			// 1002.一括印刷用 情報	2010/07/XX 一括印刷対応
		case	OWNTB_SEQ_BPRTCMN3:			// 1003.一括印刷用 情報	2010/07/XX 一括印刷対応
		case	OWNTB_SEQ_BPRTCMN4:			// 1004.一括印刷用 情報	2010/07/XX 一括印刷対応
		case	OWNTB_SEQ_BPRTCMN5:			// 1005.その他の内訳書追加対応 2014/06/XX
		case	OWNTB_SEQ_BPRTCMN6:			// 1006.その他の内訳書追加対応 2014/06/XX
		case	OWNTB_SEQ_BPRTCMN7:			// 1007.その他の内訳書追加対応 2014/06/XX
		case	OWNTB_SEQ_BPRTCMN8:			// 1008.その他の内訳書追加対応 2014/06/XX
// midori 190505 add -->
		case	OWNTB_SEQ_BPRTCMN9:			// 1009.一括印刷用 情報
		case	OWNTB_SEQ_BPRTCMN10:		// 1010.一括印刷用 情報
		case	OWNTB_SEQ_BPRTCMN11:		// 1011.一括印刷用 情報
		case	OWNTB_SEQ_BPRTCMN12:		// 1012.一括印刷用 情報
// midori 190505 add <--
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}
	catch (CDBException *e) {
		// テーブルオープンに失敗時
		e->Delete();
		return FALSE;
	}
}

//----------------------------------------------------------------
//	ICSMCTLからユーザID取得
//
//	IN:		なし
//
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//----------------------------------------------------------------
BOOL COwntbData::GetUserId(void)
{
	// ICSMCTLからユーザID取得
	ICSMCTL			ics_mctl;
	mctl_usertbl	utbl;

	if (ics_mctl.SetDatabase() == 0) {
		ics_mctl.get_usertbl(utbl);
		m_nUserId = utbl.user_id;
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------
//	owntb抽出条件の取得
//
//	IN:		int				0～:データ種別（itm_seq）
//							-1 :データ種別なし
//			int				ユーザID
//			CString			itm_name 文字列 指定なしの場合は、
//							起動モード（通常 or 一括印刷）確認を行う
//
//	RET:	CString			抽出条件
//
//  2010/07/19	一括印刷対応  引数追加
//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
//CString	COwntbData::GetFilterString(int nItmSeq, CString strItmName /*=""*/)
CString	COwntbData::GetFilterString(int nItmSeq, int nUserId, CString strItmName /*=""*/)
{
	const CString	FN = _T("COwntbData::GetFilterString()");
	CString strLog;

	strLog.Format( _T("START  nItmSeq=[%d], nUserId = [%d], strItmName=[%s]") , nItmSeq, nUserId, strItmName );
	DbgWriteLog( FN, strLog);

	// 2010/07/XX 一括印刷対応
	CString strItm = strItmName;		// owntb itm_name フィールド値

	//---------------------------------------------------------------
	// itm_name の 一括印刷時の場合 切り替え
	//---------------------------------------------------------------
	// 起動モード（通常 or 一括印刷）確認を行う場合
	if ( "" == strItm ) {
		// 一括印刷アプリからの起動？
		if ( ( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchExec() )) {
			// 一括印刷 依頼キー 情報取得（itm_name フィールドへ登録する値）
			strItm = m_pBatchPrt->GetBatchKey();
		}
	}

	//---------------------------------------------------------------
	// owntb抽出条件文字列の作成
	//---------------------------------------------------------------
	CString		strFilter;
	if (nItmSeq >= 0) {
		strFilter.Format("(apl_name='%s') AND (itm_name='%s') AND (itm_seq=%d) AND (u_id=%d)"
							, OWNTB_APLNAME
							//, OWNTB_ITMNAME
							, strItm
							, nItmSeq
							, nUserId );		//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
							//, m_nUserId);
	}
	else {
		strFilter.Format("(apl_name='%s') AND (itm_name='%s') AND (u_id=%d)"
							, OWNTB_APLNAME
							//, OWNTB_ITMNAME
							, strItm
							, nUserId );		//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
							//, m_nUserId);
	}

	DbgWriteLog( FN, _T("END  strFilter=[") + strFilter + _T("]") );

	return strFilter;
}

//----------------------------------------------------------------
//	新規レコード作成
//
//	IN:		int				データ種別（itm_seq）
//			BOOL			通常起動＝TRUE／一括印刷起動＝FALSE
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//
//	2010/08/02 一括印刷対応 引数追加 （新規レコード作成救済処置対応）
//	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
BOOL COwntbData::CreateNewRecord(int nItmSeq, BOOL fNormal/*= TRUE*/)
{
	// 2010/07/16	itm_name を 一括印刷時の場合 切り替える事！　OWNTB_ITMNAME
	// 2010/07/21	→ 上記記述の対応は不要。 → 初期データを作成するのは、通常起動時のみ。
	//				   一括印刷は、通常起動用のowntb データをコピーするため、不要。
	// 2010/08/02	→ 通常、レコードのコピーに失敗した場合、一括印刷用レコードを本関数にて作成する
	const CString	FN = _T("COwntbData::CreateNewRecord()");
	CString			strItmName,
					strLog;

	strLog.Format( _T("START  nItmSeq = [%d]､ fNormal = [%d]"), nItmSeq, fNormal);
	DbgWriteLog( FN, strLog );

	// 2010/07/XX 一括印刷対応	
	//----------------------------------------------------------------
	// 一括印刷用 情報の場合は、itm_name に依頼キーをセット
	//----------------------------------------------------------------
	if ( ( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchExec() )) {
		strItmName = m_pBatchPrt->GetBatchKey();
	} else {
		DbgWriteLog( FN, _T("パラメータエラー → END") );
		return FALSE;
	}

	try {
		// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
		//----------------------------------------------------------------
		// 共通IDのレコードが無い場合、自IDのレコードを検索。
		// 存在する場合、共通ID(=0) に更新する。
		//----------------------------------------------------------------
		BOOL		fUpdateID = FALSE;
		CString		strFilter = GetFilterString( nItmSeq, m_nUserId );		// 自IDの検索
		m_pZmSub->owntb->Requery( strFilter );					// リクエリ

		// 自IDレコードが無い場合 → 新規作成　　 (FALSE)
		// 自IDレコードがある場合 → 共通IDに更新 (TRUE)
		fUpdateID = ( (m_pZmSub->owntb->st == -1)? FALSE : TRUE );

		//----------------------------------------------------------------
		// 更新
		//----------------------------------------------------------------
		if ( fUpdateID ) {
			// 自ID を 共通IDに更新
			m_pZmSub->owntb->Edit();
			m_pZmSub->owntb->u_id = TG_COMMON_USER_ID;
			m_pZmSub->owntb->Update();
		}
		//----------------------------------------------------------------
		// 新規作成
		//----------------------------------------------------------------
		else {
			// 各レコード共通の設定＋初期化
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name	= OWNTB_APLNAME;
			//m_pZmSub->owntb->itm_name	= OWNTB_ITMNAME;	// 2010/07/XX 一括印刷対応
			m_pZmSub->owntb->itm_name	= strItmName;
			m_pZmSub->owntb->itm_seq	= nItmSeq;
			// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
			//m_pZmSub->owntb->u_id		= m_nUserId;
			m_pZmSub->owntb->u_id		= TG_COMMON_USER_ID;

			//----------------------------------------------------------------
			//	レコード別に初期値を設定
			//	　・使用するフィールド全てに、必ず値をセットする
			//	　・未使用フィールドはNULLなので、項目追加時に対応可能
			//----------------------------------------------------------------
			switch (nItmSeq) {
			// 2010/07/XX 一括印刷対応
			case	OWNTB_SEQ_BPRTCMN:
				m_pZmSub->owntb->vl[0]	= 0;				// 表紙を出力する					→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
				m_pZmSub->owntb->vl[1]	= 0;				// 決算期間に「現在」を出力する		→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
				break;
			case	OWNTB_SEQ_BPRTCMN1:
				for (int n=0; n<20; n++) {
					m_pZmSub->owntb->code[n]	= "0";		// 各帳表の出力ON/OFF				→ 2010/07/XX 一括印刷対応 itm_seq(=1001) にて保存
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（From）	→ 2010/07/XX 一括印刷対応 itm_seq(=1001) にて保存
				}
				break;
			case	OWNTB_SEQ_BPRTCMN2:
				for (int n=0; n<20; n++) {
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（To）		→ 2010/07/XX 一括印刷対応 itm_seq(=1002) にて保存
				}
				break;
			case	OWNTB_SEQ_BPRTCMN3:
				for (int n=0; n<12; n++) {
					m_pZmSub->owntb->code[n]	= "0";		// 各帳表の出力ON/OFF				→ 2010/07/XX 一括印刷対応 itm_seq(=1003) にて保存
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（From）	→ 2010/07/XX 一括印刷対応 itm_seq(=1003) にて保存
				}
				break;
			case	OWNTB_SEQ_BPRTCMN4:
				for (int n=0; n<12; n++) {
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（To）		→ 2010/07/XX 一括印刷対応 itm_seq(=1004) にて保存
				}
				break;
			case	OWNTB_SEQ_BPRTCMN5:
				for (int n=0; n<15; n++) {
					m_pZmSub->owntb->code[n]	= "0";		// 各帳表の出力ON/OFF				→ 2010/06/XX その他の内訳書追加対応
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（From
				}
				break;
			case	OWNTB_SEQ_BPRTCMN6:
				for (int n=0; n<15; n++) {
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（To）		→ 2010/06/XX その他の内訳書追加対応
				}
				break;
			case	OWNTB_SEQ_BPRTCMN7:
				for (int n=0; n<15; n++) {
					m_pZmSub->owntb->code[n]	= "0";		// 各帳表の出力ON/OFF				→ 2010/06/XX その他の内訳書追加対応
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（From）
				}
				break;
			case	OWNTB_SEQ_BPRTCMN8:
				for (int n=0; n<15; n++) {
					m_pZmSub->owntb->vl[n]		= 0;		// 各帳表の出力ページ範囲（To）		→ 2010/06/XX その他の内訳書追加対応
				}
				break;
// midori 190505 add -->
			case	OWNTB_SEQ_BPRTCMN9:
				for(int n=0; n<20; n++)	{
					m_pZmSub->owntb->vl[n]		= -1;		// 各帳表の出力方法
				}
				break;

			case	OWNTB_SEQ_BPRTCMN10:
				for(int n=0; n<12; n++)	{
					m_pZmSub->owntb->vl[n]		= -1;		// 各帳表の出力方法
				}
				break;

			case	OWNTB_SEQ_BPRTCMN11:
				for(int n=0; n<15; n++)	{
					m_pZmSub->owntb->vl[n]		= -1;		// 各帳表の出力方法
				}
				break;

			case	OWNTB_SEQ_BPRTCMN12:
				for(int n=0; n<15; n++)	{
					m_pZmSub->owntb->vl[n]		= -1;		// 各帳表の出力方法
				}
// midori 190505 add <--
			}

			// 更新
			m_pZmSub->owntb->Update();
		}
	}
	catch (CDBException *e) {

		DbgWriteLog( FN, _T("例外発生 → END") );
		DbgWriteLog( FN, e->m_strError );

		e->Delete();
		return FALSE;
	}

	DbgWriteLog( FN, _T("END") );
	return TRUE;
}

//----------------------------------------------------------------
//	データ書き込み(全て)
//
//	IN:		なし
//
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//----------------------------------------------------------------
BOOL COwntbData::SaveAllData(void)
{
	// 2010/07/XX 一括印刷対応 ログ追加
	const CString	FN = _T("COwntbData::SaveAllData()");
	DbgWriteLog( FN, _T("START"));

	//--------------------------------------------------------
	//	itm_seq=1～999を順番に読み込む
	//	事前チェックは、各Load処理で実施するので省略する
	//--------------------------------------------------------
	// 2010/07/XX 一括印刷対応
	if ( ( m_pBatchPrt ) && ( m_pBatchPrt->IsBatchExec() )) {
		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN"));
		// 1000：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN) == FALSE) {
			return FALSE;
		}

		// その他の内訳書追加により、スワップする（２－①～２－⑤を１－⑳の後ろに差し込む）
		SwapSonotaSave();

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN1"));
		// 1001：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN1) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN2"));
		// 1002：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN2) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN3"));
		// 1003：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN3) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN4"));
		// 1004：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN4) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN5"));
		// 1005：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN5) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN6"));
		// 1006：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN6) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN7"));
		// 1007：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN7) == FALSE) {
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN8"));
		// 1008：一括印刷用 情報
		if (SaveData(OWNTB_SEQ_BPRTCMN8) == FALSE) {
			return FALSE;
		}

// midori 190505 add -->
		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN9"));
		// 1009：一括印刷用 情報
		if(SaveData(OWNTB_SEQ_BPRTCMN9) == FALSE)	{
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN10"));
		// 1010：一括印刷用 情報
		if(SaveData(OWNTB_SEQ_BPRTCMN10) == FALSE)	{
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN11"));
		// 1011：一括印刷用 情報
		if(SaveData(OWNTB_SEQ_BPRTCMN11) == FALSE)	{
			return FALSE;
		}

		DbgWriteLog( FN, _T("一括印刷専用 レコード保存 OWNTB_SEQ_BPRTCMN12"));
		// 1012：一括印刷用 情報
		if(SaveData(OWNTB_SEQ_BPRTCMN12) == FALSE)	{
			return FALSE;
		}
// midori 190505 add <--
	}

	DbgWriteLog( FN, _T("END"));
	return TRUE;
}

//----------------------------------------------------------------
//	データ書き込み
//
//	IN:		int				データ種別（itm_seq）
//
//	RET:	BOOL			TRUE=正常終了，FALSE=エラー
//
// 2010.01.XX 社会福祉　資金収支計算書で貸借発生を出力する
// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
BOOL COwntbData::SaveData(int nItmSeq)
{
	const CString	FN = _T("COwntbData::SaveData()");
	CString			strLog;

	strLog.Format( _T("START  nItmSeq = [%d]"), nItmSeq);
	DbgWriteLog( FN, strLog );

	// 事前チェック
	if (StartCheck(nItmSeq) == FALSE) {
		return FALSE;
	}

	// リクエリー実行
	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化 （共通ID引数追加）
	CString		strFilter = GetFilterString(nItmSeq, TG_COMMON_USER_ID);
	m_pZmSub->owntb->Requery(strFilter);

	// データが見つからない場合はエラーとする
	if (m_pZmSub->owntb->st == -1) {
		return FALSE;
	}

	// 変数→テーブル更新
	try {
		m_pZmSub->owntb->Edit();
		// レコード別に値を設定
		switch (nItmSeq) {
		// 2010/07/XX 一括印刷対応
		case	OWNTB_SEQ_BPRTCMN:
			// 1000.一括印刷用 情報
			m_pZmSub->owntb->vl[0]			= m_uInfo.uPrtCmn.nCoverSw;						// 表紙を出力する					→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
			m_pZmSub->owntb->vl[1]			= m_uInfo.uPrtCmn.nKsnAddNow;					// 決算期間に「現在」を出力する		→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
// midori 160608 add -->
			m_pZmSub->owntb->vl[2]			= m_uInfo.uPrtCmn.nZeroSw;						// 帳票イメージのみ出力する			→ 2010/07/XX 一括印刷対応 itm_seq(=1000) にて保存
// midori 160608 add <--

			strLog.Format( _T("START  表紙を出力する = [%d]"), m_uInfo.uPrtCmn.nCoverSw);
			DbgWriteLog( FN, strLog );
			strLog.Format( _T("START  決算期間に「現在」を出力する = [%d]"), m_uInfo.uPrtCmn.nKsnAddNow);
			DbgWriteLog( FN, strLog );
// midori 160608 add -->
			strLog.Format( _T("START  帳票イメージのみ出力する = [%d]"), m_uInfo.uPrtCmn.nZeroSw);
			DbgWriteLog( FN, strLog );
// midori 160608 add <--

			break;
		case	OWNTB_SEQ_BPRTCMN1:
			// 1001.一括印刷用 情報
			for (int n=0, i=0; n<20; n++, i++) {
				m_pZmSub->owntb->code[n].Format( _T("%d"), m_uInfo.uBPrtCmn.nOutPut[i] ); 	// 各帳表の出力ON/OFF				→ 2010/07/XX 一括印刷対応 itm_seq(=1001) にて保存
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_From[i];			// 各帳表の出力ページ範囲（From）	→ 2010/07/XX 一括印刷対応 itm_seq(=1001) にて保存

				strLog.Format( _T("START  各帳表の出力ON/OFF(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOutPut[i]);
				DbgWriteLog( FN, strLog );
				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（From） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_From[i]);
				DbgWriteLog( FN, strLog );
			}

			break;
		case	OWNTB_SEQ_BPRTCMN2:
			// 1002.一括印刷用 情報
			for (int n=0, i=0; n<20; n++, i++) {
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_To[i];				// 各帳表の出力ページ範囲（To）		→ 2010/07/XX 一括印刷対応 itm_seq(=1002) にて保存

				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（To） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_To[i]);
				DbgWriteLog( FN, strLog );
			}

			break;
		case	OWNTB_SEQ_BPRTCMN3:
			// 1001.一括印刷用 情報
			for (int n=0, i=20; n<12; n++, i++) {
				m_pZmSub->owntb->code[n].Format( _T("%d"), m_uInfo.uBPrtCmn.nOutPut[i] ); 	// 各帳表の出力ON/OFF				→ 2010/07/XX 一括印刷対応 itm_seq(=1003) にて保存
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_From[i];			// 各帳表の出力ページ範囲（From）	→ 2010/07/XX 一括印刷対応 itm_seq(=1003) にて保存

				strLog.Format( _T("START  各帳表の出力ON/OFF(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOutPut[i]);
				DbgWriteLog( FN, strLog );
				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（From） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_From[i]);
				DbgWriteLog( FN, strLog );
			}

			break;
		case	OWNTB_SEQ_BPRTCMN4:
			// 1002.一括印刷用 情報
			for (int n=0, i=20; n<12; n++, i++) {
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_To[i];				// 各帳表の出力ページ範囲（To）		→ 2010/07/XX 一括印刷対応 itm_seq(=1004) にて保存

				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（To） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_To[i]);
				DbgWriteLog( FN, strLog );
			}

			break;

		case	OWNTB_SEQ_BPRTCMN5:
			// 1005.一括印刷用 情報
			for (int n=0, i=32; n<15; n++, i++) {
				m_pZmSub->owntb->code[n].Format( _T("%d"), m_uInfo.uBPrtCmn.nOutPut[i] ); 	// 各帳表の出力ON/OFF				→ 2010/06/XX　その他の内訳書追加対応
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_From[i];			// 各帳表の出力ページ範囲（From）

				strLog.Format( _T("START  各帳表の出力ON/OFF(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOutPut[i]);
				DbgWriteLog( FN, strLog );
				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（From） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_From[i]);
				DbgWriteLog( FN, strLog );
			}

			break;
		case	OWNTB_SEQ_BPRTCMN6:
			// 1006.一括印刷用 情報
			for (int n=0, i=32; n<15; n++, i++) {
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_To[i];				// 各帳表の出力ページ範囲（To）		→ 2010/06/XX　その他の内訳書追加対応

				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（To） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_To[i]);
				DbgWriteLog( FN, strLog );
			}

			break;
		case	OWNTB_SEQ_BPRTCMN7:
			// 1007.一括印刷用 情報
			for (int n=0, i=47; n<15; n++, i++) {
				m_pZmSub->owntb->code[n].Format( _T("%d"), m_uInfo.uBPrtCmn.nOutPut[i] ); 	// 各帳表の出力ON/OFF				→ 2010/06/XX　その他の内訳書追加対応
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_From[i];			// 各帳表の出力ページ範囲（From）

				strLog.Format( _T("START  各帳表の出力ON/OFF(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOutPut[i]);
				DbgWriteLog( FN, strLog );
				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（From） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_From[i]);
				DbgWriteLog( FN, strLog );
			}

			break;
		case	OWNTB_SEQ_BPRTCMN8:
			// 1008.一括印刷用 情報
			for (int n=0, i=47; n<15; n++, i++) {
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPRange_To[i];				// 各帳表の出力ページ範囲（To）		→ 2010/06/XX　その他の内訳書追加対応

				strLog.Format( _T("START  各帳表の出力ページ範囲(%02d)（To） = [%d]"), i, m_uInfo.uBPrtCmn.nOPRange_To[i]);
				DbgWriteLog( FN, strLog );
			}

			break;

// midori 190505 add -->
		case	OWNTB_SEQ_BPRTCMN9:
			// 1009.一括印刷用 情報
			for(int n=0, i=0; n<20; n++, i++)	{
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPPrnHoho[i];			// 各帳表の出力方法

				strLog.Format(_T("START  各帳表の出力方法(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOPPrnHoho[i]);
				DbgWriteLog( FN, strLog );
			}
			break;

		case	OWNTB_SEQ_BPRTCMN10:
			// 1010.一括印刷用 情報
			for(int n=0, i=20; n<12; n++, i++)	{
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPPrnHoho[i];			// 各帳表の出力方法

				strLog.Format(_T("START  各帳表の出力方法(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOPPrnHoho[i]);
				DbgWriteLog( FN, strLog );
			}
			break;

		case	OWNTB_SEQ_BPRTCMN11:
			// 1011.一括印刷用 情報
			for(int n=0, i=32; n<15; n++, i++)	{
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPPrnHoho[i];			// 各帳表の出力方法

				strLog.Format(_T("START  各帳表の出力方法(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOPPrnHoho[i]);
				DbgWriteLog( FN, strLog );
			}
			break;

		case	OWNTB_SEQ_BPRTCMN12:
			// 1012.一括印刷用 情報
			for(int n=0, i=47; n<15; n++, i++)	{
				m_pZmSub->owntb->vl[n]		= m_uInfo.uBPrtCmn.nOPPrnHoho[i];			// 各帳表の出力方法

				strLog.Format(_T("START  各帳表の出力方法(%02d) = [%d]"), i, m_uInfo.uBPrtCmn.nOPPrnHoho[i]);
				DbgWriteLog( FN, strLog );
			}
// midori 190505 add <--
		}

		// 更新
		m_pZmSub->owntb->Update();

		DbgWriteLog( FN, _T("END") );
		return TRUE;
	}
	catch (CDBException *e) {
		// ログ
		DbgWriteLog( FN, _T("例外発生 END") );
		DbgWriteLog( FN, e->m_strError );
		e->Delete();
		return FALSE;
	}
}

// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
// 一括印刷用 owntbレコードから、ログインユーザID、共通ユーザID(=0)を検索
//
// IN:	CDBNpMod*		公益法人db 非営利共用集計クラスポインタ
// 
// RET:	BOOL			TRUE :レコード有り
//						FALSE:レコード無し → 一括印刷 詳細設定モードを実行し設定実行誘導 出力しない。
//
// 2010/12/17 修正依頼No.N-035 一括印刷ユーザ情報共通化
//	・共通ユーザID=0 を検索
//		→ あり：処理終了
//		→ なし：ログインユーザIDを検索
//	・ログインユーザID検索
//		→ あり：ID=0に更新
//		→ なし：詳細設定モード → 通常用のowntb ID=0 をコピーしデータ作成
//			　	 印刷/CDRモード → 詳細設定実行の誘導メッセージを表示 → 正常終了とする（出力行わない）
//
// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
//----------------------------------------------------------------
BOOL	COwntbData::CheckRecordBprtUserID( void )
{
	// ログ
	const CString	FN = _T("COwntbData::CheckRecordBprtUserID()");
	CString			filter;
	DbgWriteLog( FN, _T("START") );

	// 一括印刷以外
	if ( ( !m_pBatchPrt ) || ( FALSE == m_pBatchPrt->IsBatchExec())) {
		// 正常終了
		DbgWriteLog( FN, _T("通常起動 → END"));
		return OWNTB_CHKREC_OK_EXIST;
	}

	//--------------------------------------------------
	// 依頼キー取得
	//--------------------------------------------------
	CString	strBKey = m_pBatchPrt->GetBatchKey();

	//--------------------------------------------------
	// ユーザID = 0 レコードチェック
	//--------------------------------------------------
	filter = GetFilterString( OWNTB_SEQ_BPRTCMN, TG_COMMON_USER_ID, strBKey );
	m_pZmSub->owntb->Requery( filter );
	if( m_pZmSub->owntb->st != -1){
		// レコードがある場合は何もしない
		DbgWriteLog( FN, _T("END - 共通ユーザID レコード有り"));
		return OWNTB_CHKREC_OK_EXIST;
	}

	//--------------------------------------------------
	// ユーザID = ログインユーザID レコードチェック
	//--------------------------------------------------
	filter = GetFilterString( OWNTB_SEQ_BPRTCMN, m_nUserId, strBKey );
	m_pZmSub->owntb->Requery( filter );
	if( m_pZmSub->owntb->st != -1){
		//// レコードがある場合は何もしない
		//DbgWriteLog( FN, _T("END - ログインユーザID レコード有り"));
		//return TRUE;
		//--------------------------------------------------
		// ユーザID = ログインユーザID レコードあり
		// 　→ログインユーザ以外のユーザ数チェック
		//--------------------------------------------------
		filter.Format(SQLFILTER_BPRT_USRIDCHK, OWNTB_APLNAME, strBKey, OWNTB_SEQ_BPRTCMN, TG_COMMON_USER_ID, m_nUserId);
		m_pZmSub->owntb->Requery( filter );
		if( m_pZmSub->owntb->st != -1){
			// ログインユーザ以外のユーザ数 > 0
			DbgWriteLog( FN, _T("END - 共通ID(=0), ログインユーザID レコード有り, ログインユーザ以外ID レコード有り"));
			return OWNTB_CHKREC_NG_DOSET01;
		}

		// ログインユーザ以外のユーザ数 = 0
		DbgWriteLog( FN, _T("END - 共通ID(=0), ログインユーザID レコード有り, ログインユーザ以外ID レコード無し"));
		return OWNTB_CHKREC_OK_EXIST;
	}

	//--------------------------------------------------
	// ユーザID = ログインユーザID レコードなし
	// 　→ログインユーザ以外のユーザ数チェック
	//--------------------------------------------------
	filter.Format(SQLFILTER_BPRT_USRIDCHK, OWNTB_APLNAME, strBKey, OWNTB_SEQ_BPRTCMN, TG_COMMON_USER_ID, m_nUserId);
	m_pZmSub->owntb->Requery( filter );
	// マップを使用して、登録済みのユーザをカウントする
	// 【補足】…CMapクラスを使用する理由
	// 　CDBZmSubクラスは、CRecordset::m_strSortが使用出来ない（定義していない？使用を抑制している？）ため、ソート不可。
	// 　CDatabase::ExecuteSQLでの“ORDER BY”または“GROUP BY”は、結果が取得出来ない。
	// 　以上の事から、アプリ側でユーザIDを重複チェックしない仕組みが必要 → CMapクラスのLookup、SetAtを使用するのが確実と判断。
	int		nUserCnt	= 0;					// ユーザID=ログインユーザID以外のユーザ数
	long	lUserID		= 0;					// ユーザID以外参照
	BOOL	fPrsFlg		= FALSE;				// 有無フラグ（処理には使用しない。マップに必要なので定義しているだけ）
	CMap<int, int, BOOL, BOOL> mapPrsUserID;	// ログインユーザ以外のユーザマップ（VALUE定義=BOOLは処理には使用しない。CMapに必要なので定義しているだけ）
	// ログインユーザ以外のユーザマップ クリア
	mapPrsUserID.RemoveAll();
	// ログインユーザ以外のユーザを検出する
	if (m_pZmSub->owntb->st != -1) {
		while (m_pZmSub->owntb->st != -1) {
			// ユーザIDを参照する
			lUserID = m_pZmSub->owntb->u_id;

			// ユーザIDでマップに検索をかける
			if (mapPrsUserID.Lookup( lUserID, fPrsFlg ) != TRUE) {
				// マップに登録がなければ、新規追加する
				mapPrsUserID.SetAt( lUserID, TRUE );
			}

			// 次のレコードを参照する
			m_pZmSub->owntb->MoveNext();
		}
		// 念のため最初のレコードを参照しておく
		m_pZmSub->owntb->MoveFirst();

		// ログインユーザー以外のユーザ数を取得する
		nUserCnt = (int)mapPrsUserID.GetCount();
	}
	// ログインユーザ以外のユーザ数チェック
	if (nUserCnt > 1) {
		// ログインユーザ以外のユーザ数 > 1
		DbgWriteLog( FN, _T("END - 共通ID(=0), ログインユーザID レコード無し, ログインユーザ以外のユーザ > 1人"));
		return OWNTB_CHKREC_NG_DOSET03;
	}

	//// 該当レコード無し
	//// → 上位で、出力モード時、詳細設定実行誘導メッセージを表示し、直ちに終了（出力を行わない）とする
	//DbgWriteLog( FN, _T("END - 共通ID(=0), ログインユーザID レコード無し"));

	//return FALSE;
	// ログインユーザ以外のユーザ数 = 1
	DbgWriteLog( FN, _T("END - 共通ID(=0), ログインユーザID レコード無し, ログインユーザ以外のユーザ = 1人"));
	return OWNTB_CHKREC_NG_DOSET02;
}//COwntbData::CheckRecordBprtUserID

// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
//----------------------------------------------------------------
// 引数指定の itn_name レコードを削除（ユーザID≠0）
//
// IN:	CDBNpMod*		公益法人db 非営利共用集計クラスポインタ
//		CString			owntbテーブル itm_name 
// 
// RET:	BOOL			TRUE：正常終了
//----------------------------------------------------------------
BOOL	COwntbData::ClrOwnRecByUID( CDBZmSub* pZmSub, CString strItmName )
{
	// ログ
	const CString	FN = _T("COwntbData::ClrOwnRecByUID()");
	CString			strSQL,
					strFilter;
	DbgWriteLog( FN, _T("START") );

	// パラメータチェック
	if ( !pZmSub ) {
		DbgWriteLog( FN, _T("パラメータエラー") );
		return FALSE;
	}

	strSQL		= _T("DELETE owntb WHERE ");

	strFilter.Format( _T("apl_name = '%s' AND itm_name = '%s' AND u_id <> %d"), 
							OWNTB_APLNAME, 
							strItmName, 
							TG_COMMON_USER_ID );
	strSQL += strFilter;

	// DBオブジェクト
	CDatabase	*pDB = pZmSub->m_database;

	try {
		pDB->ExecuteSQL( strSQL );
	} catch ( CDBException* e ) {
		DbgWriteLog( FN, strSQL);
		DbgWriteLog( FN, e->m_strError );
		e->Delete();
		return FALSE;
	}

	DbgWriteLog( FN, _T("END") );
	return TRUE;

}//COwntbData::ClrOwnRecByUID


void COwntbData::SwapSonotaLoad()
{
	int nCheck[5] = {0};
	int nFrom[5] = {0};
	int nTo[5] = {0};
// midori 190505 add -->
	int nHoho[5] = {0};
// midori 190505 add <--

	// まず上書きされる２－①～２－⑤を退避
	for(int i=0; i<5; i++){
		nCheck[i]	= m_uInfo.uBPrtCmn.nOutPut[i+27];
		nFrom[i]	= m_uInfo.uBPrtCmn.nOPRange_From[i+27];
		nTo[i]		= m_uInfo.uBPrtCmn.nOPRange_To[i+27];
// midori 190505 add -->
		nHoho[i]	= m_uInfo.uBPrtCmn.nOPPrnHoho[i+27];
// midori 190505 add <--
	}

	// １－⑥～１－⑳を１－⑤の後ろに上書き
	for(int i=27; i<42; i++){
		m_uInfo.uBPrtCmn.nOutPut[i]			= m_uInfo.uBPrtCmn.nOutPut[i+5];
		m_uInfo.uBPrtCmn.nOPRange_From[i]	= m_uInfo.uBPrtCmn.nOPRange_From[i+5];
		m_uInfo.uBPrtCmn.nOPRange_To[i]		= m_uInfo.uBPrtCmn.nOPRange_To[i+5];
// midori 190505 add -->
		m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= m_uInfo.uBPrtCmn.nOPPrnHoho[i+5];
// midori 190505 add <--
	}

	// ２－①～２－⑤を１－⑳の後ろに上書き
	for(int i=42; i<47; i++){
		m_uInfo.uBPrtCmn.nOutPut[i]			= nCheck[i-42];
		m_uInfo.uBPrtCmn.nOPRange_From[i]	= nFrom[i-42];
		m_uInfo.uBPrtCmn.nOPRange_To[i]		= nTo[i-42];
// midori 190505 add -->
		m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= nHoho[i-42];
// midori 190505 add <--
	}
}

void COwntbData::SwapSonotaSave()
{
	int nCheck[5] = {0};
	int nFrom[5] = {0};
	int nTo[5] = {0};
// midori 190505 add -->
	int nHoho[5] = {0};
// midori 190505 add <--

	// まず上書きされる２－①～２－⑤を退避
	for(int i=0; i<5; i++){
		nCheck[i]	= m_uInfo.uBPrtCmn.nOutPut[i+42];
		nFrom[i]	= m_uInfo.uBPrtCmn.nOPRange_From[i+42];
		nTo[i]		= m_uInfo.uBPrtCmn.nOPRange_To[i+42];
// midori 190505 add -->
		nHoho[i]	= m_uInfo.uBPrtCmn.nOPPrnHoho[i+42];
// midori 190505 add <--
	}

	// １－⑥～１－⑳を２－⑥の前に上書き
	for(int i=41; i>26; i--){
		m_uInfo.uBPrtCmn.nOutPut[i+5]		= m_uInfo.uBPrtCmn.nOutPut[i];
		m_uInfo.uBPrtCmn.nOPRange_From[i+5] = m_uInfo.uBPrtCmn.nOPRange_From[i];
		m_uInfo.uBPrtCmn.nOPRange_To[i+5]	=  m_uInfo.uBPrtCmn.nOPRange_To[i];
// midori 190505 add -->
		m_uInfo.uBPrtCmn.nOPPrnHoho[i+5]		= m_uInfo.uBPrtCmn.nOPPrnHoho[i];
// midori 190505 add <--
	}

	// ２－①～２－⑤を１－⑤の後ろに上書き
	for(int i=27; i<32; i++){
		m_uInfo.uBPrtCmn.nOutPut[i]			= nCheck[i-27];
		m_uInfo.uBPrtCmn.nOPRange_From[i]	= nFrom[i-27];
		m_uInfo.uBPrtCmn.nOPRange_To[i]		= nTo[i-27];
// midori 190505 add -->
		m_uInfo.uBPrtCmn.nOPPrnHoho[i]		= nHoho[i-27];
// midori 190505 add <--
	}
}