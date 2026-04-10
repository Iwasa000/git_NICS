// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

#include <H26HyoViewRec.h>

class CDBSyzShinMainView;
class CBaseHyoView;
class CBaseTaxHyoView;
class CMainFrame : public ICSMainFrame
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:
#ifdef _DEBUG
	ICSMon* pMon;
#endif //_DEBUG

private:
	CDBNpSub		*m_pZmSub;			// 公益クラス
	CCoinfCtl		*m_pCoinfCtl;		// 会社情報クラス
	CSzzanList		*m_pSzzanList;		// 消費税集計クラス（基礎データ）
	CSyzSyukei		*m_pSyzSyukei;		// 消費税集計クラス（申告書メイン）
	CSyzSyukei		*m_pSyzSubSyukei;	// 消費税集計クラス（申告書サブ）
	CArithEx		m_Arith;			// 倍長演算
	CTblhdl			m_Tblhdl;			// テーブルハンドリングクラス
	CVolDateDB		m_VolDate;			// 期間モジュール
	CSyzUtil		m_Util;				// ユーティリティクラス
	int				m_swFork;			// フォークスイッチ
										//  0：呼び出し無し
										//  1：初期起動時に税率５％プロの呼び出し
										//  2：履歴で税率５％プロの呼び出し
	HINSTANCE		m_MainInstance;		// メインアプリケーション側のリソースインスタンス

	int				m_swBeForked;		// フォーク呼び出されスイッチ
										//  0：呼び出し無し
										//  1：一括印刷（詳細設定）
										//  2：一括印刷（印刷）
										//  3：一括印刷（ＣＤＲ印刷）
	int				m_swForkErr;		// フォーク時エラー
	SH_BPPRINT_INFO	m_BpPrintInfo;		// 一括印刷情報
	BOOL			m_isKeyoff;			// フォーク中のキー制御
	BOOL			m_isShownHonpyo;	// 本表画面が表示されたかどうか？	('15.02.03)
// 24/06/13_税務代理書面 del -->
	//bool			m_isHirendoCall;	// 非連動dbからの呼び出し？			('15.12.23)
// 24/06/13_税務代理書面 del <--
	int				m_isScoreLinkCall;	// ScoreLinkからの呼び出し？		  20240116
										//  0：ScoreLinkからの呼び出し
										// -1：一括印刷からの呼び出し、または通常業務からの印刷
	CString			m_ScoreLinkPdfNam;	// ScoreLinkで使用するpdf名			　20240116

	CString			m_PrgPath;			// プログラムのパス
	CString			m_SysPath;			// システムファイルのパス
	CString			m_SysGrPath;		// システムグループファイルのパス
	CString			m_GrPath;			// グループNo.
	CString			m_PrgMdev;			// プログラムデバイス
	mctl_usertbl	m_usertbl;			// ユーザ情報
	int				m_sgDotPrn;			// ドットプリンタサイン

//	int				m_UnConnect;		// 非連動サイン（0:通常, 1:非連動）
	BOOL			m_isNonCorp;		// 非営利法人？
	BOOL			m_isKndMster;		// 過年度マスター？
	BOOL			m_isNewMake;		// 新規作成？
	BOOL			m_isAfterH24Reform;	// 95％ルール改正対応期間？
	BOOL			m_isExistOffice;	// 事務所名あり？
	BOOL			m_isAbleProgress;	// 進捗処理可能？
	BOOL			m_isPrintEtaxFinish;// 電子申告済を印刷する？
	BOOL			m_isEndStart;		// 終了処理開始？
	int				m_Over500MillionSw;	// 課税売上高が５億円超えサイン
	BOOL			m_isCompChange;		// 強制書き換え？
	BOOL			m_isInitEnd;		// 初期化完了？
	BOOL			m_isCompReInit;		// 再初期化必要？（履歴→現在申告に切り換えた時に再読込み必要かどうか）
	BOOL			m_isNeedChkTkkz;	// 終了時に特定課税仕入の確認が必要？('15.11.17)

	char			m_sgIkat;			// 一括税抜きサイン
//	char			m_sgBasic;			// 個人決算書対応マスターサイン
	int				m_sgSpc;			// 特定収入処理サイン
	int				m_Specnt;			// 特定収入 登録数
	int				m_SplistCnt;		// 特定収入計算表 登録数
	int				m_SnSeq;			// 現在処理中の履歴シーケンス

	int				m_TaxCalcusw;		// ９０％ルール変更時、警告サイン [12'05.11]

	int				m_symd;				//150107

	//----->特定課税仕入対応150713
	int				m_eymd;				
	//<------------------------------

	// タブの総数
//	int				m_nowTabIdx;		// 現表示タブインデックス

	char			m_sgMnthExcept[20];	// 決算集計 除外サイン
	char			m_sgSyzsyk[20];		// 消費税集計サイン 

	
	ESELECT			m_Eselect;			// 電子申告用　税務署コード選択モジュール
	BOOL			m_isOpenEselect;	// 税務署コードモジュールをオープンした？

	//== 事務所情報ファイル用 ==
	OFC_HEAD			m_O_HEAD;			// 事務所情報ヘッダ
	OFC_DATA			m_O_DATA;			// 事務所情報データ
	ZEI_DATA			m_Z_DATA;			// 税理士個人情報
	COP_DATA			m_C_DATA;			// 
	Old_TaxInf8			m_oTax;				// 
	New_TaxInf8			m_nTax;				// 
//	int					m_SubInf;
//	char				Zname[128];
//	char				ZHnam[128];
//	char				Ztel[128];
//	char				ZtelP[128];
//	int					m_nTaxSubInf;		// どの税理士情報？
	struct	ProgDataEx	m_Progdata;			// 進捗情報
	SH_ZEIRI_INFO		m_ZeiriInfo;		// 税理士個人情報(新様式)


	int					m_Progress;
	int					m_FinishSw;
	int					m_Finish;
//	PRCT_BPINFO			Bpinfo;
//	KMNCTL_GETSKINFPAC	Gkspac;
	char				m_FinishPrn[1024];

	//== 会社情報取込用 ==
	CTime				m_old_tansyuku;
	BYTE				m_old_Sn_SKKBN;
	BYTE				m_old_Sn_KZTNSK;
	BYTE				m_old_Sn_MCOUNT;



	//== 申告書データ ==
	CSnHeadData		m_SnHeadData;			// 消費税ヘッダ情報
	_TB_PAR			m_SkjCnvtbl;			// 仕入控除用テーブル
	VOL2			m_Vol2;					// 月別情報（旧形式）・・・本当は消したい！！
	char			m_Month26[16];			// 消費税申告書用月管理テーブル
	char			m_SYsw[14];				// 消費税集計ｻｲﾝ 
	SH_DAYPAC		m_dtl[14];				// 
	int				m_svSVmzsw;				// 履歴過年度プロ呼出時データ保存用('14.05.15)
	BYTE			m_svRealSvmzsw;			// 複数税率データ本体

	SH_PAYMENT		m_SvPayment;			// 国税納付書データ（一次保存用）

	NOWCOINF		NowCoInf;				// 顧問先情報（現在）
	
	MoneyBasejagArray	m_SvRevMoneyArray;	// リバースチャージを加味した試算表データ保存('15.09.30)

	int				m_rtIsRegistMcode;		//	-999:取得未了、-999以外：該当の戻り値
	bool			m_bAlterIndex;			// インデックスの更新完了？

	int				m_JcContinuePrint;		// 和暦継続出力？

	bool			m_isKoketsuKingaku;		// 個人決算書金額の取得可能？
	bool			m_isSkjUsrUnconfirmed;	// VerUpで還付申告明細用連動情報設定を採用する利用者が未確定か // TISW修正[22-0676] 2026/03/19

// 操作
public:
	CoSel				m_cosel;		// 会社確認
	SYC_SHININFO		m_ShinInfo;		// 共通情報
	TgPackData			m_TgPDat;		// 顧問先情報()

// 24/06/13_税務代理書面 add -->
	bool			m_isHirendoCall;	// 非連動dbからの呼び出し？			('15.12.23)
// 24/06/13_税務代理書面 add <--

	int				m_nowTabIdx;		// 現表示タブインデックス
	int				m_nowTabmax;		// 現表示タブ数（'15.03.18）
	int				m_nowTabver;		// 原表示のタブバージョン 1:Syz31 ('19.07.17)

	char			m_sgBasic;			// 個人マスターサイン


	int				m_ActViewVer;		// 起動中のビューのバージョン　EnumIdKaiseiVer
	int				m_ActedViewFlg;		// 起動したビューのバージョン　□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□■■
										// D0:26年改正(8%改正), D1:31年改正(10%改正)

	// 起動中のビューのバージョンを設定
	int SetActViewVer( int ver );
	// 起動中のビューのバージョンを設定（ヘッドデータを使用）
	int SetActViewVer(CSnHeadData *pSnHeadData);
	// 起動中のビューのバージョンを設定（ビュー情報を使用）
	int SetActViewVer(CSnset *pSnset);
	// 起動中のビューのバージョンを設定
	int SetActViewVerByDay( long eymd );

	// 起動中のビューのバージョンの取得
	int GetActViewVer();

	// メインアプリで管理している改正バージョンの設定
	int SetKaiseiVer( int ver );
	// メインアプリで管理している改正バージョンの取得
	int GetKaiseiVer();


	// ウィンドウ管理
	CDBSyzShinMainView	*m_pMainView;	// 基本情報ビュー

	// フォーク情報の取得
	int GetForkInfo();
	// 年度に合わせたプログラムの呼び出し
	int CallSyzProgram( BOOL isFirst );
	// 履歴に合わせたプログラムの呼び出し
	int CallSyzHistoryProgram( int seq );
	// 通年のプログラム呼出
	int CallSyzAllYearProgram();
	// 一括印刷情報の取得
	int GetBatchPrintInfo( EnumIdShBatchPrintState *pBpState, SH_BPPRINT_INFO *pBpPrintInfo );
	// ScoreLink情報の取得  20240116
	void GetScoreLinkInfo( int& isScoreLinkCall, CString& ScoreLinkPdfNam );
	// 強制変更フラグをセット
	void SetCompChangeFlg( BOOL compChange ){ m_isCompChange = compChange; };
	// 過年度プログラム呼出マスタ？
	BOOL IsBeforeH25Master( int chktype );
	// フォーク呼出？
	int IsCalledFork(){ return m_swFork; };
	// 一括印刷のフォーク
	int IsCalledBeFork(){ return m_swBeForked; };
	// アプリケーション初期化
	int InitApl( BOOL isFirst );
	// DB接続
	int ConnectDB();
	// テーブルオープン
	int OpenZmtbl();
	// 環境情報の取得
	int GetEnvInfo();
	// 消費税情報のチェック
	int CheckSyzInfo();
	// 消費税マスターバージョンのチェック
	int CheckSyzVersion( CString &msg );
	// 決算期間内 消費税５％改正 チェック
	int Chk_5per_ReviseDayIn();
	// 決算期間内 消費税８％改正 チェック
	int Chk_8per_ReviseDayIn();
	// 決算期間内 標準税率チェック
	int ChkDefaultZritu();
	// 現在の選択マスターは過年度マスタ？
	int ChkKanendoMaster();
// 改良依頼No.20-0560 add -->
	int GetZenSyz(char *pArith);
// 改良依頼No.20-0560 add <--
	// 起動時の９５％ルール改正対応期間チェック
	void Chk_H24Reform_InInit();
	// 一括印刷をエラー状態にセット
	void SetBpPrintErr();
	// 履歴番号の取得
	int GetSnSeq(){ return m_SnSeq; };
	// テーブル再オープン
	int ReOpenZmtbl();
	// テーブル再オープン必要？
	BOOL IsNeedReOpenZmtbl(){ return m_isCompReInit; };
	// テーブル再オープン終了
	void SetReOpenFlg( BOOL bReInit ){ m_isCompReInit = bReInit; }
	// キーオフ中？
	BOOL IsKeyoff(){ return m_isKeyoff; };
	// szvol バージョンチェック
	int ChkSzvolVersion( CDBNpSub *pZmsub );
	// フォーカスを入力画面に戻す('15.04.13)
	int SetFocusToInputPos();
	// 終了処理フラグの切り替え('18.06.22)
	void SetEndStartFlg( BOOL flg );

	// 履歴情報の初期化
	int InitHistoryReport();
	// 履歴があるかどうかのチェック
	int IsExistHistoryFromDB();

	// 消費税申告書の読込み
	int OpenSzsnk( int number, CString &errmsg, bool isFirst=false );
	// マスターオープン
	int OpenSyzShin( int number, CString &errmsg );
	// 消費税申告書の書込み
	int CloseSzsnk( int number, CString &errmsg, BOOL doClose );
	// 履歴バージョンチェック
	int CheckHistoryVersion();
	// 消費税申告書情報の書き込み
	int UpdateSyzShin();
	// 仕入控除税額に関する明細書の変換テーブルの取得
	int GetSkjCnvtbl();
	// 仕入控除税額に関する明細書(個人用)　財務連動 
	int SkjPersonalConvert();
	// 消費税の還付申告に関する明細書 オープン
//	int OpenRefund( int number, CString& errmes );
	// 異マスターの再構築
	int ReConstructMaster();
	// 変数初期設定
	int InitVariable();
	// 表示帳票の最終データ取得
	void GetDispHyoData();
	// 仕入控除税額に関する明細書の変換テーブルの再取得
	int GetPastSkjData();
	// ユーザ情報の取得
	mctl_usertbl& GetUserTbl(){ return m_usertbl; };

	// マスター起動時の総合会社情報の取り込み
	int InitCoinfo( CString &errmsg );
	// 会社情報から取り込み
	int GetCoInfo( int type, CWnd *pWnd=NULL );
	// 税務署コードのセット
	int SetTxationOffice( int sign );
	// 事務所情報の読込み
	int ReadOfficeInfo( BOOL isFirst );
	// 事務所情報の読込み
	int ReadOfficeInfo( int seq,int subinf );
	// 進捗情報の初期化
	int InitProgressInfo();
	// 国税納付書
	int AccessShinPayment( CString	&errmes );
//----> 納付書・相続人対応
	// 納付書情報のセット
	void SetNOHFUSHO_INFO( NOHFUSHO_INFO &nf_info, int setflg );
	// 納付書データのセット
	void SetNOHFUSHO_DATA( NOHFUSHO_DATA &nf_data, CDBSyzAccess *pDBSyzAcc, int sozoku_no = -1 );
	// 相続人情報のセット
	void SetSozokuInfo( NOHFUSHO_INFO &nf_info, NOHFUSHO_DATA &nf_data, NOHFUSHO_INFO &sozokuinfo, NOHFUSHO_DATA &sozokudata );
	// 国税納付書　ShinPaymentテーブルの更新
	void UpdateShinPayment( CDBSyzAccess *pDBSyzAcc, NOHFUSHO_DATA &nf_data, int update_type, int sozoku_no = -1 );
//<---- 納付書・相続人対応
	// 納付書年度判定関数
	unsigned char GetNohusyoNendo( int ADD, unsigned char EYMD[] );
	// 進捗記録
	void ProgressAdminister();
	// 会社読込み時の状態保存
	void SetOldData();
	// 終了時の進捗保存
	void ProgressEnd();
	// 顧問先情報取り込み後、全ての保存情報に書き込む必要があるのか？
	int AdviseInfoRenew( int type, CString& errmes );
	// 関連づいている顧問先情報の保存('15.09.14)
	int SaveRelCoInfo();

	// 現状顧問先情報のコピー処理
	void Copy_NowCoInfo();

	//顧問先情報書き込み
	int SetCoInformation( void );

	// 顧問先情報へ郵便番号の変換
	int PostcodeConvert( char *SetCode, unsigned char *GetCode );

	// 顧問先情報モジュールにマスター側のデータをセット
	void SetMasterInfo( CINF_REC &CoRec );

	// 消費税申告書データの新規作成
	int AddNewSzsnkData( CString &errmsg );

	// テーブルクローズ
	int CloseZmtbl();
	// 消費税関連後処理
	int SyzEndJob();
	// エラー終了
	void ErrorExit( const char *pErrStr );
	// マスター情報の保存
	int SaveNowSyzInfo();

	// 集計クラス初期化
	int InitSyzSyukei();
	// 集計クラス再初期化
	int ReSyzSyukei();
	// 集計
	int StartSyzSyukei( char *pUwriai );
	// 再集計
	int ReStartSyzSyukei( BOOL bMiddleCalq );
	// 金額再取得
	int RegetCalqedMony();
	// 仕入控除税額との連動
	int ConnectedSkj( bool isUpdDisp );
	// 仕入控除税額のみとの連動
	int ConnectedSkjZeigaku( bool isUpdDisp );
//2016.02.23 INSERT START
	// 仕入金額等の明細のクリア
	int ClearShiire();
//2016.02.23 INSERT END
	// 更正の請求用　申告書金額計算
	int CalqSyzForKsk( int mode );
	// 中間及び期間短縮の更新
	int RenewSyzShin( SH_PASTINFO *pPastInfo );
	// 印刷可能？
	BOOL IsAblePrint();
	// 棚卸調整額の連動
	int CalqStatckSyz();
	// 現申告データを消去して、データの再読込み
	int ReReadData();
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
	// 不要になった帳票のデータをクリア ('19.04.16)
	void ClearHyoData();
	// 仮受・仮払消費税チェック
	int CheckKariukeKaribarai();
	// 集計済み金額の取得	('14.06.30)
	int GetCalqedMony();
	// 課税取引金額計算書との連動（'14.08.08）
	int ConnectedTaxList();
	// 特定収入計算書との連動（'15.02.13）
	int ConnectedSpcList();
	// 簡易課税第六種フラグをセット('15.02.20)
	void SetSixKindFlg( BOOL SixKindFlg );

	// プログラムパスの取得
	CString& GetPrgPath(){ return m_PrgPath; };
	// プログラムデバイスの取得
	CString& GetPrgMdev(){ return m_PrgMdev; }


	//== 年月日 ==
	// 月日情報の変換
	int CnvZvolMinfo();
	// 入力年月　開始・最終設定
	//int	CDBSyzShinView::ofs_get( void )
	int	GetMofs();
	// 月日よりオフセット返送
	short GetShinofst( unsigned char *date, int *ofst );
	// 集計済みチェック
	int ChekCalqed( int sm, int km );
	// 期間処理モジュール	初期設定
	int InizShinDate();
	// 平成～西暦　変換
//	int shin_datecnv( unsigned char year, unsigned char *date, int sign );
	// 閏年計算
	short shin_uday( int eymd );
	// 閏年計算
//	short shin_uday( char *eymd );
	// 閏年計算用モジュール
//	short shin_Feb( short yr );
	// 月日よりデータ期間チェック
//	short shin_mdchk( char mnth, char day, char *symd, char *eymd, char *eeymd );
	short shin_mdchk(char mnth, char day, char *symd, char *eymd, int eeymd);
	// 期末月計算
//	short shin_mday( char month, char *eymd );
	short shin_mday(char month, int eymd);
	// 課税期間オフセットの取得
	int GetKzkikanOfs();
	// 月日が指定範囲内？('14.06.07)
	BOOL IsInMonthRange( int mofs, unsigned char mm, unsigned char dd );

	//== ツールバー ==
	// 
	BOOL UpdateLockIndicator( int flg );
	// 
	BOOL UpdateRirekiIndicator( int flg );
	// ツールバーの表示変更
	void ChangeToolbar();
	// 会社選択ボタンの切替
	void ChangeCoselButton( BOOL onof );

	// 非連動？
//	BOOL IsUnConnect(){ return m_UnConnect; };

	// 金額取得
	int GetMony( CString &tag, CString &chtag, char *pArith );
	// 金額取得
	int GetMony( CString &tag, CString &chtag, char *pArith, int kver );
	// 金額セット
	int SetMony( CString &tag, CString &chtag, char *pArith );
	// 金額セット
	int SetMony( CString &tag, CString &chtag, char *pArith, int kver );
	// 本表の計算(1)～(26)迄
	// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
	void AlreadyDecisionCalc( int type );
	// 部分データ書込み
	int WriteSepData( int flg );
	// 部分データ読込み
	int ReadSepData( int flg );
	// 履歴番号の切替（他表）
	int ChangeViewSnSeq( int snseq );
	int CMainFrame::ChangeViewSnSeq( int snseq, int actVer );
	// 次回申告データ作成に必要になるデータの読込み
	int ReadDataForNextShinkoku();
	// 前年の消費税額取得（過年度マスターから）
	int GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith );
	// 過年度マスターの読み込み
	int SetKanendoData();
	// ボリュームコントロールによるマスターチェック('17.12.14)
	int ChkKanendoByVctl( CO_CTL_Array& coarray, int& rtZ_kikan, bool& rtHasKMst );
	// 既確定税額のクリア('15.01.28)
	int ClearAlreadyDecision();
	// 税務署入力にフォーカス誘導('15.02.03)
	int GuideTaxOfficeInput();
	// 特定収入処理サインの取得('15.02.11)
//-- '15.04.04 --
//	int GetsgSpc(){ return m_sgSpc; };
//---------------
	int* GetsgSpc(){ return &m_sgSpc; };
//---------------
	// 還付申告？('15.02.24)
	BOOL IsKanpuShinkoku();
//2016.06.22 INSERT START
	// 還付申告？(中間納付税額、中間納付譲渡割額加味)
	BOOL IsKanpuShinkoku2();
//2016.06.22 INSERT END
	// 税理士タイプの取得('15.02.25)
	int GetZeirishiType();
	// 仕入額変更による特定収入の再計算('15.04.03)
	int RecalqSpcByChgSkj();
	// ５％の特定収入アリ？('15.04.27)
	int Chk5PerSpc( int sofs, int eofs, int endYymmdd );
	// ５％の特定収入の詳細チェック('15.04.27)
	//int Chk5PerSpcDetail( int sofs, int eofs );
	// ５％と８％の特定収入の詳細チェック('15.04.27)
	int Chk5PerSpcDetail( int sofs, int eofs, int endYymmdd );
	// 簡易課税第六種？('15.05.01)
	BOOL IsSixKindKani( long symd );
	// 還付金融機関入力にフォーカス誘導('16.12.26)
	int GuideKanpuKinyu( int dsInput );
	// 予定申告の月数クリア('17.02.09)
	int ClearYoteiMonth();

	//== マイナンバー ==
	// 終了処理開始？
	BOOL IsEndStart(){ return m_isEndStart; };
	// 関連づいている顧問先情報の保存
	int SaveLinkedCoInfo( CINF_REC *pCoRec );
	// 関連づいている顧問先情報の取得
	TgPackData* GetTgData(){ return &m_TgPDat;};
	// 既存のハッシュ値と等しい？('15.08.04)
	BOOL IsEqualNowKjhash( CINF_REC *pCoRec );
	// 個人番号ハッシュ値の取込('15.08.04)
	int TakeInMynumberFromCoinfo( CINF_REC *pCoRec );
	// 削除履歴を登録
	int RegistDelHistory();
	// 個人番号履歴情報セット
	void SetNoHistory( NOHISTTBL &nhtbl );
	// 個人番号履歴情報セット（顧問先情報）
	void SetCoNoHistory( NOHISTTBL &nhtbl );

	//== 特定課税仕入 ==
	//------>特定課税仕入対応150713
	// 特定収入チェックボックス表示期間かどうかの判定
	BOOL IsTokutei( long eymd );
	// 改正様式をセット('15.08.27)
	void SetShRevType( EnumIdIcsShRevType shrevtype );
	//<---------------------------

//2016.06.22 INSERT START
	// 「還付を受けようとする金融機関等」が空欄のエラーメッセージ
	int	Message_BlankKinyu( int &erst );
//2016.06.22 INSERT END
//2017.12.04 INSERT START
	// 基準期間の課税売上高が入力されているか
	int Message_BlankKijunKazei();
	// 「損益計算書の収入金額」「損益計算書の仕入金額～」が入力されているか
	int Message_BlankSoneki();
//2017.12.04 INSERT END
//2018.03.13 INSERT START
	// 付表６　個人番号のエラー
	int Message_MyNumber_error();
	// 付表６　個人番号の重複
	int Message_MyNumber_overlap();
	// 付表６　相続分の割合の合計
	int Message_SozokuWariai();
	// 付表６　各人の納付還付税額の合計
	int Message_NofuKanpuGokei();
	// 付表６　強制エディットオフ
	void EditOff_Fuhyo6();
//2018.03.13 INSERT END

//----> 2022/12/27
	// 公金受取口座にチェックがついているが個人番号がない場合のメッセージ
	int Message_KoukinUketori();
//<---- 2022/12/27

	// 還付付表2　インボイス登録番号のチェック
	bool CheckInvno( int sgn );

//---->yoshida200925
	// // zvol subswチェック＆修復
	int Chkzvolsubsw(CDBNpSub *pZmsub);
//<------

	// 特例テーブルチェック&修復
	int ChkTkreiTable ( CDBNpSub *pZmSub );
	// 10日間営業日チェック&修復
	int ChkEigyobiTable ( int symd, CDatabase *pdb, CRecordset *prs );
	// 休日設定チェック&修復
	int ChkHolidayTable ( CDatabase *pdb, CRecordset *prs );
	// 10日間営業日修復
	int RepairEigyobi ( CDatabase *pdb, CRecordset *prs );

	// 10%様式：手入力項目注意メッセージ
	int Message_H31ManualInputSign();

	// 顧問先情報データのメモリアップを切り替える
	void _TgtLimit_Kmn( bool bSw );
	// 過年度の関連情報をチェック
	int ChkKndMasterInfo();
	// 前年の消費税額を取得
	int GetLastYearSyz();

	// 平成継続出力
	bool IsJcContinuePrint(CSnHeadData *pSnHeadData);

	void KikanChange( unsigned char newver );

	// データの引き継ぎ
	int TakeOverData( int now_Kaisei_VER, int dst_Kaisei_VER );
	// 本表ビューの取得
	CBaseHyoView* GetHonHyoView( int vtype=-1 );
	// 課税取引ビューの取得
	CBaseTaxHyoView* GetTaxListView( int vtype=-1 );

// 24/04/04_モジュール仕様変更 add -->
	void	GetZeiriInfo( R_HEAD* rHead, R_ZEIRI* rData );
// 24/04/04_モジュール仕様変更 add <--

	// 前年マスター情報
	ZEN_MASTERINF	m_ZMasterinfo;


	int		m_KndFlg;										// 過年度マスター有りサイン
	char	m_ZenkTax[6];
	char	m_ZenkSymd[4];
	char	m_ZenkEymd[4];

	int		m_ZenkSg;		//元号
	int		m_ZenkEg;

	// 課税期間の取得
	void GetKazeiKikan( long &symd, long &eymd );
	// 課税取引金額計算表　経理方式の選択可能？
	bool IsChangeKeiriHoushiki();

	// 還付付表２　フォーカス移動
	void MoveKanpuFocus();

	// 個人決算書金額の取得が可能
	void IsGetKoketsuKingaku();

// 24/03/26_税務代理書面 add -->
	void SetTaxagInfoPar( struct _TAXAGENT_INFO* pTi, CSnHeadData* pSh );
	void CallWriteTaxAgentDB( CDBNpSub* pZms, CSnHeadData* pSh, CWnd* pWnd );
	BOOL IsUsedZeimud( void );
// 24/03/26_税務代理書面 add <--

	// TISW修正[22-0676] 2026/03/19 START
	/// <summary>VerUpで還付申告明細用連動情報設定を採用する利用者が未確定か</summary>
	bool IsSkjUsrUnconfirmed() { return m_isSkjUsrUnconfirmed; };
	// TISW修正[22-0676] 2026/03/19 END

protected:
	// 会社情報クラスの初期化
	int InitCCoinfCtl( CString &errmsg, int type );
	// 会社情報取り出し条件パケットの初期化
//-- '15.08.11 --
//	int InitCoinfPacket( NCOF_PAR &NCpar, CINF_REC &CoRec );
//---------------
	int InitCoinfPacket( NCOF_PAR &NCpar, CINF_REC &CoRec, BOOL IsDispKjno );
//---------------

	// 会社コードの変換
	void CnvCono( char *pCod );
	// 漢字スペースカット
	int CutKanjiSpace( char *pDsKnji, char *pSrKnji, int len );
	// 半角スペースカット
	void CutAnkSpace( char *pBuf, int len );

	// 企業環境？
	BOOL IsCompanyPackageInSyz();

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 各付表の初期化
	int InitOtherView( int type, CDBNpSub *pZmSub );
	// 各付表の終了処理
	int EndOtherView( int wflg = 1 );
	// タブタイトルのセット
	int SetTabTitle( int change = 0 );
	// 各付表の終了処理フォーカスセット
	int EndViewFocus();
// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	LRESULT		OnCall( WPARAM wParam, LPARAM lParam );
	LRESULT		OnFromForkChain( WPARAM wParam, LPARAM lParam );
	LRESULT		OnFromParentCosel( WPARAM wParam, LPARAM lParam );
	LRESULT		OnMessageClose( WPARAM w, LPARAM l );
	LRESULT		OnEndProcess( WPARAM wParam, LPARAM lParam );

public:
	LRESULT OnUserSetFocus(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserUpdateIndicate(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserReadZeirishi(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserRecalq(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserDispSpecpass(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserSpcpostRecalq(WPARAM wParam, LPARAM lParam);	// '15.03.09
	LRESULT OnUserSkjpostRecalq(WPARAM wParam, LPARAM lParam);	// '15.04.03
	LRESULT OnUserEndStart(WPARAM wParam, LPARAM lParam);		// '15.07.31
	LRESULT OnUserResetTkkz(WPARAM wParam, LPARAM lParam);		// '15.09.09
	LRESULT OnUserRefocusPrint(WPARAM wParam, LPARAM lParam);	// '16.01.07
	LRESULT OnUserChgLockDisp(WPARAM wParam, LPARAM lParam);	// '16.07.22
	LRESULT OnUserResetRefund(WPARAM wParam, LPARAM lParam);	// '19.09.09
	void ChildFrameActive( CWnd* pChild );
	virtual int BeginApp();
	virtual int	TabRButtonDown();
	virtual int	TabLButtonDown();
	afx_msg void OnTaxtionList();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	afx_msg void OnShinPayment();
	afx_msg void OnButtonF0();
	afx_msg void OnUpdateButtonF0(CCmdUI *pCmdUI);
	afx_msg void OnKomonsakiInf();
	afx_msg void OnUpdateTaxtionList(CCmdUI *pCmdUI);
	afx_msg void OnHistoryRev();
	afx_msg void OnUpdateHistoryRev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShinPayment(CCmdUI *pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnZennenTax();
	afx_msg void OnUpdateZennenTax(CCmdUI *pCmdUI);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnKoketsuMeisyo();
	afx_msg void OnUpdateKoketsuMeisyo(CCmdUI *pCmdUI);
	afx_msg void OnKoketsuKingaku();
	afx_msg void OnUpdateKoketsuKingaku( CCmdUI *pCmdUI );
// 24/03/18_税務代理書面 add -->
	afx_msg void OnShinZeimud();
	afx_msg void OnUpdateShinZeimud(CCmdUI *pCmdUI);
// 24/03/18_税務代理書面 add <--
	//afx_msg void OnUpdateKoujoSelect(CCmdUI *pCmdUI);
	//afx_msg void OnKoujoSelect();
};


