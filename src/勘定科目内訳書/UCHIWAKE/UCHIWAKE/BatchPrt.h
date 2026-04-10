// *****************************************************************************************
// BatchPrt.h
// 一括印刷処理 ヘッダ情報
//
//
//	ICS様受領資料： 一括印刷仕様_100710.doc
// *****************************************************************************************
#pragma once

//-------------------------------------------------------------
// 定数
//-------------------------------------------------------------
#define		LPARAM_BPRT_EXITAPP		(9999)				// 一括印刷からの終了通知 WM_CLOSE LPARAM

// UCOM I/F キー情報定義
#define		UCOMKEY_CALLPRG		_T( "呼出プログラム" )
#define		UCOMKEY_BATCHPRG	_T( "依頼プログラム" )
#define		UCOMKEY_BATCHMODE	_T( "依頼項目" )
#define		UCOMKEY_BATCHKEY	_T( "依頼キー" )

#define		UCOMKEY_SETYMD		_T( "設定年月日" )
#define		UCOMKEY_PRTSIZE		_T( "用紙サイズ" )

#define		UCOMKEY_PRTINFO		_T( "依頼詳細" )		// 印刷部数、プリンタ名 取得キー
#define		UCOMKEY_PRTYMD		_T( "出力年月日" )		// 印刷開始・終了年月日, 印刷開始・終了月オフセット 取得キー
#define		UCOMKEY_PRTRET		_T( "印刷状態" )		// 印刷結果 セットキー（0：正常終了／-1：エラー終了）

#define		UCOMKEY_CDRPATH		_T( "出力パス" )		// PDF出力先 パス文字列

#define		UCOMKEY_SETDETAIL	_T( "設定詳細" )		// PDFファイル名	2010/07/31 追加 仕様変更（7/30）

#define		UCOMKEY_		_T( "" )

// UCOM I/F アイテム情報定義
#define		UCOMITM_BATCHMODE	_T( "一括印刷" )		// UCIM "呼出プログラム"	に、一括印刷アプリからの起動時にセットされるアイテム
#define		UCOMITM_BATCHSET	_T( "詳細設定" )		// UCOM "依頼項目"			に、一括印刷の設定モード の場合にセットされるアイテム
#define		UCOMITM_BATCHPRT	_T( "印刷" )			// UCOM "依頼項目"			に、一括印刷の印刷モード の場合にセットされるアイテム
#define		UCOMITM_BATCHCDR	_T( "CDR出力" )			// UCOM "依頼項目"			に、一括印刷のCDR出力モード の場合にセットされるアイテム

// 印刷結果定義
#define		UCOMITM_BATCHPRT_OK		( 0)				// UCOM 印刷出力結果 正常終了
#define		UCOMITM_BATCHPRT_NG		(-1)				// UCOM 印刷出力結果 異常終了

// 印刷方向定義
#define		UCOMITM_BATCHPRT_V		(0)					// UCOM 印刷出力方向 縦
#define		UCOMITM_BATCHPRT_H		(1)					// UCOM 印刷出力方向 横
#define		UCOMITM_BATCHPRT_V_STR	_T("PORTRAIT")		// UCOM 印刷出力方向 縦
#define		UCOMITM_BATCHPRT_H_STR	_T("LANDSCAPE")		// UCOM 印刷出力方向 横

// 印刷サイズ定義
#define	UCOMITM_BATCHPRT_PAPERA5	_T("PC_PAPERA5")	// A5
#define	UCOMITM_BATCHPRT_PAPERA4	_T("PC_PAPERA4")	// A4
#define	UCOMITM_BATCHPRT_PAPERA3	_T("PC_PAPERA3")	// A3
#define	UCOMITM_BATCHPRT_PAPERB5	_T("PC_PAPERB5")	// B5
#define	UCOMITM_BATCHPRT_PAPERB4	_T("PC_PAPERB4")	// B4

// 一括印刷用 メッセージ
// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
//#define		BATCHPRT_MSG_DOSETMODE	_T("現ユーザで保存された「詳細設定」の情報が見つかりません。\n設定を保存したユーザで印刷されるか、再度「詳細設定」で印刷情報の登録を行ってください。")	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
#define		BATCHPRT_MSG_DOSETMODE01	_T("一括印刷用に保存された情報が複数存在します。\n再度「詳細設定」で印刷情報の登録を行ってください。")	// 2010/12/17 修正依頼No.N-035 一括印刷ユーザ情報共通化（運用中マスタ対応）
#define		BATCHPRT_MSG_DOSETMODE02	_T("現ユーザで保存された「詳細設定」の情報が見つかりません。\n設定を保存したユーザで印刷されるか、再度「詳細設定」で印刷情報の登録を行ってください。")	// 2010/12/17 修正依頼No.N-035 一括印刷ユーザ情報共通化（運用中マスタ対応）
#define		BATCHPRT_MSG_DOSETMODE03	_T("現ユーザで保存された「詳細設定」の情報が見つかりません。\n再度「詳細設定」で印刷情報の登録を行ってください。")	// 2010/12/17 修正依頼No.N-035 一括印刷ユーザ情報共通化（運用中マスタ対応）

// 一括印刷モード 
typedef enum {
	ID_BATCHMODE_NONE = 0,			// 通常起動
	ID_BATCHMODE_PRT,				// 一括印刷 出力モード
	ID_BATCHMODE_CDR,				// 一括印刷 CDR出力モード
	ID_BATCHMODE_SET				// 一括印刷 設定モード
} EnumIdBatchMode;


//-------------------------------------------------------------
// 構造体
//-------------------------------------------------------------
// UCOM 情報　設定 結果 返却情報
typedef struct _tagBATCHPRTUCOMINF_RETSET {
	int			nPrtSize;			// 設定・用紙選択 コンボの選択結果の値をセット（例：試算表 OwntbData.h EnumIdInfoPrtSize）

	int			nPrtOffSetS;		// 設定・出力月オフセット 開始
	int			nPrtOffSetE;		// 設定・出力月オフセット 終了

	// 勘定科目内訳書アプリ不要 情報：本クラスを共通して使用するため、定義
	int			nPrtYmdS;			// 設定・出力年月日 開始
	int			nPrtYmdE;			// 設定・出力年月日 終了
	int			nFlgYmd;			// 設定・出力年月日情報に 日 まで必要か？（1=必要／0=不要））

	CString		strPdfName;			// 設定・PDF出力ファイル名	2010/07/31 追加（仕様変更 7/30）

} BATCHPRTUCOMINF_RETSET;


// UCOM 情報　出力 結果 返却情報
typedef struct _tagBATCHPRTUCOMINF_RETPRT {
	int			nPrtRet;			// 印刷結果	（正常：0，失敗：-1  定数管理）
} BATCHPRTUCOMINF_RETPRT;


// UCOM 情報　起動時 取得情報
typedef struct _tagBATCHPRTUCOMINF {
	CString		strCallPrg;			// 呼出プログラム 名 
	CString		strBatchPrg;		// 依頼プログラム 名（一括印刷プログラムから実行されるアプリ名）
	CString		strBatchMode;		// 起動モード（ 印刷、設定、CDR出力 ）
	CString		strBatchKey;		// 依頼キー  （一括印刷プログラムで一意に識別するためのキー情報 owntb itm_name フィールド値に使用）

	CString		strPrtCnt;			// 依頼詳細 印刷部数
	CString		strPrtName;			// 依頼詳細 プリンタ名
	CString		strPrtYmdS;			// 設定・出力年月日 開始（NDS担当アプリ不要）
	CString		strPrtYmdE;			// 設定・出力年月日 終了（NDS担当アプリ不要）
	CString		strPrtOffSetS;		// 設定・出力月オフセット 開始
	CString		strPrtOffSetE;		// 設定・出力月オフセット 終了
// 24/01/16_ScoreLink add -->
	CString		strSlIraiId;		// ScoreLink 依頼ID
	CString		strSlPdfName;		// ScoreLink 出力PDFファイル名
// 24/01/16_ScoreLink add <--

	//CString		strCdrPath;			// 出力パス	  PDF出力先 パス文字列	2010/07/31 削除（仕様変更 7/30）
} BATCHPRTUCOMINF;


//-------------------------------------------------------------
// プロトタイプ
//-------------------------------------------------------------
// ログ出力
extern void DbgWriteLog( CString strFN, LPCSTR strLog);

//-------------------------------------------------------------
// 一括印刷 クラス
//-------------------------------------------------------------
class CBatchPrt
{
public:
	CBatchPrt(void);

	~CBatchPrt(void);

public:

	BOOL	GetUcomInf( void );				// 一括印刷 コモン情報(UCOM) 情報 取得 

	BOOL	IsBatchExec( void );			// 一括印刷アプリからの起動    確認
// 24/01/16_ScoreLink add -->
	BOOL	IsScoreLinkExec( void );		// ScoreLinkからの起動    確認
	CString	GetPdfName( void );
// 24/01/16_ScoreLink add <--

	BOOL	IsBatchModePrt( void );			// 一括印刷 出力モードでの起動 確認
	BOOL	IsBatchModeSet( void );			// 一括印刷 設定モードでの起動 確認
	BOOL	IsBatchModeCDR( void );			// 一括印刷 出力モードでの起動 確認
	int		GetBatchMode( void );			// 一括印刷モード取得

	CString	GetBatchKey( void );			// 一括印刷依頼キー取得

	CString	GetPrtName( void );				// UCOMで指定された出力プリンタ名
	int		GetPrtCnt( void );				// UCOMで指定された印刷部数取得
	int		GetPrtOfsetS( void );			// 開始月 オフセット 取得
	int		GetPrtOfsetE( void );			// 終了月 オフセット 取得


	BOOL	DoBatchPrint( void );			// 一括印刷 出力 指示
	BOOL	ExitBatchMode( void );			// 一括印刷 モードでの起動終了 指示（アプリ終了）

	BOOL	SetUcomInfSet( BATCHPRTUCOMINF_RETSET *pRetSetInf );
	BOOL	SetUcomInfPrt( BATCHPRTUCOMINF_RETPRT *pRetPrtInf );

	void	SetErrorExit( void );			// CMainFrame::OnMessageClose() エラー終了(LPARAM=1) の時に使用 ／ CVPrint::ErrorAbort() 時使用
	BOOL	IsErrorExit( void );			// 終了モード取得（TRUE=異常発生による終了時／正常終了時）
											// 2010/08/02 一括印刷 異常終了対応

	BOOL	GetMsgShowFlg( void );			// 2010/08/12 一括印刷 印刷時メッセージ非表示対応

	void	SetWmExitMsg( UINT message, WPARAM param );	//	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
	UINT	GetWmExitMsg( void );						//	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
	WPARAM	GetWmExitParam( void );						// 	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応

	BOOL	SetPrtSchYmd( CDBZmSub* pZmSub );			// 2010/08/25 税務用年月日セット

	void	ICSMessageBoxEx( CString strMsg );			// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
														// 2010/08/30 実装クラス変更 → UCHIIMP ビルド対応

	void	SetPrtMdlOnErr( void );			//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
	void	ClrPrtMdlOnErr( void );			//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
	BOOL	IsPrtMdlOnErr( void );			//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応

	//void	SetDoSetMode( void );			// 詳細設定 実行誘導 フラグ ON		2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	void	SetDoSetMode( int nDoSetMode );	// 詳細設定 実行誘導 フラグ ON		2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
	void	ClrDoSetMode( void );			// 詳細設定 実行誘導 フラグ クリア	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	//BOOL	IsDoSetMode( void );			// 詳細設定 実行誘導 フラグ 取得	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	int	IsDoSetMode( void );				// 詳細設定 実行誘導 フラグ 取得	2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）

protected:
	void	InitBatchPrgInf(void);			// コモン情報(UCOM) 初期化

	BOOL	GetUcomInfYMD( CString );		// 一括印刷 コモン情報(UCOM) “設定年月日”or “出力年月日” 取得 
	BOOL	GetUcomInfSet( void );			// 一括印刷 コモン情報(UCOM) 設定	 情報 取得 
	BOOL	GetUcomInfPrt( void );			// 一括印刷 コモン情報(UCOM) 印刷	 情報 取得 
	BOOL	GetUcomInfCDR( void );			// 一括印刷 コモン情報(UCOM) CDR出力 情報 取得 

	CString	CnvPrtSize_Ap2Ucom( int nPinf );	// UCOM用 用紙サイズ 変換	(AP情報→UCOM情報）
	CString	CnvPrtVH_Ap2Ucom( int nPinf );		// UCOM用 印刷方向 変換		(AP情報→UCOM情報）

	int		CnvPrtSize_Ucom2Ap( int nPinf );	// UCOM用 用紙サイズ 変換	(UCOM情報→AP情報）
	int		CnvPrtVH_Ucom2Ap( int nPinf );		// UCOM用 印刷方向 変換		(UCOM情報→AP情報）


protected:
	int					m_nBatchMode;		// 一括印刷モード ： EnumIdBatchMode 参照
	BATCHPRTUCOMINF		m_udUcomInf;		// UCOM I/F 情報

	BOOL				m_fErrorEnd;		// TRUE=異常終了／FALSE正常終了  2010/08/02 一括印刷 異常終了対応 
											// CMainFrame::OnMessageClose() エラー終了(LPARAM=1) の時 TRUEセット

	UINT				m_uiWmExitMsg;		// 終了通知メッセージ WM_CLOSE, WM_QUERYENDSESSION など
	WPARAM				m_wpWmExitParam;	// 終了通知メッセージ 受信時 パラメータ
											// 2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応

	BOOL				m_fPrtMdlErr;		// 印刷モジュール異常終了処理中フラグ   CVPrint::ErrorAbort() 実行 ～ エラー後処理終了まで TRUE
											// 詳細：CBatchPrt::ClrPrtMdlOnErr() 関数のコメント参照

	//BOOL				m_fDoSetMode;		// 詳細設定モード実行誘導フラグ  TRUE=誘導が必要(一括印刷 出力不可) // 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	int					m_nDoSetMode;		// 詳細設定モード実行誘導フラグ  0以外=誘導が必要(一括印刷 出力不可) 2011/02/08
};
