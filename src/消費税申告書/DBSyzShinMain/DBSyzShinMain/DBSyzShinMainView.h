// DBSyzShinMainView.h : CDBSyzShinMainView クラスのインターフェイス
//


#pragma once
#include "afxwin.h"
#include "groupstatic.h"
#include "historyreport.h"
#include "colorcombobox.h"
#include "transstatic.h"

class CDBSyzShinMainView : public ICSFormView
{
protected: // シリアル化からのみ作成します。
	CDBSyzShinMainView();
	DECLARE_DYNCREATE(CDBSyzShinMainView)

public:
	enum{ IDD = IDD_DBSYZSHINMAIN_FORM };

// 属性
public:
	CDBSyzShinMainDoc* GetDocument() const;

// 操作
public:

	void SetDefFocus();
	// 初期化
	int Init( CDBNpSub *pZmSub, CMainFrame *pMainFrame, CSnHeadData *pSnHeadData, SYC_SHININFO *pShinInfo, CTblhdl *pTblhdl, _TB_PAR *pTbPar );
	// 画面の初期化
	int InitDisp();
	//-------------------->2019.12.19.yokono
	void NyuryokuLock();
	//<-------------------------------------
	// フォーク終わり
	int EndProcess( WPARAM wParam, LPARAM lParam );
	// 終了処理
	int EndJob( int swBeForked, int swForkErr, mctl_usertbl &usertbl, CString &Grpath );
	// マイナンバー関連情報の破棄 ('16.01.05)
	void FinalizeMyno( bool isYotei );
	// 変数初期化
	int InitVariable();
	// 仕入控除税額に関する明細書の変換テーブルの保存
	int SaveSkjCnvInfo();
	// 第２画面作成時の比較情報を保存
	void SaveTabMakeInfo();

	//== 申告選択 ==
	// 呼出プログラム選択
	int ChkCallProgramType(int mode = 0);//引数追加　mode = 0 : 履歴関係セット処理通す　１：履歴関係セット処理通さない
	//== 印刷 ==
	// 印刷共有情報のクリア
	void ClearPrintCmInfo();
	// 印刷情報の初期化
	int InitPrintInfo();
	// 印刷微調整情報の取得
	void GetPrAdjustInfo();
	// 印刷微調整情報の保存
	void SetPrAdjustInfo();
	// 印刷する帳票が課税取引金額計算表？
	BOOL IsPrintTaxationList( char sign );
	// 出力情報取得
	BOOL GetPrInitInfo();
	BOOL GetPrInitInfo2();		// 部数
	// 印刷前処理
	int CheckSyzShinPrint();
	// 課税売上の金額チェック
	int CheckKzeiUri();
	// 中間納付税額の金額チェック
	int CheckMiddleTax();
	// 印刷中？
	BOOL IsPrinting();
	// ユーザー情報保存
	void SetInitInfo( mctl_usertbl &usertbl );
	void SetInitInfo2( void );	// 部数
	// 顧問先名称及びコード印刷情報の取得
	void SetNprtParInf( NPRT_PAR *pPrtSet );
	// 一括印刷をエラー状態にセット
	void SetBpPrintErr();
	// 出力情報選択
	int PrntBasisSelect();
	// 出力キャンセル
	void SetOptionMode();
	// 印刷する帳票が特定収入計算表？ ('15.02.13)
	BOOL IsPrintSpcList( char sign );
	// 還付申告の要・不要の切り替え ('15.02.24)
	int ChangeRefundOut( int sw );

	// 印刷スケジュールの読み込み
	int ReadXYSPrintSch();
	// 印刷スケジュールの読み込み
	int ReadXYSPrintSch_H26( EnumIdIcsShRevType shrevtype, CString& PrgPath );
	// 印刷スケジュールの読み込み
	int ReadXYSPrintSch_R01( EnumIdIcsShRevType shrevtype, CString& PrgPath );

	// 年度に合わせたプログラムの呼び出し
	int CallSyzProgram();

// 24/02/19wd_特定収入対応 add -->
	// 特定収入計算表 個別出力設定
	BOOL GetPrnExSubInfo(void);		// 出力情報の取得
	BOOL GetPrnExSubInfo2(void);
	BOOL SetPrnExSubInfo(void);		// 出力情報を保存
// 24/02/19wd_特定収入対応 add <--

//// 24/03/07wd_個別対応（課税取引金額計算表＋特定収入計算表＋税務代理書） add -->
//	// 特定収入計算表 個別出力設定
//	BOOL GetPrInitInfoSub(void);		// 出力情報の取得
//	BOOL SetPrInitInfoSub(void);		// 出力情報を保存
//// 24/03/07wd_個別対応 add <--

	//== 計算 ==
	// 申告書金額計算
// CMainFram.CalqSyzForKsk() へ
//	void ShzBillValSet( int idx, int mode );
	// 金額取得
	int GetMony( CString &tag, CString &chtag, char *pArith );
	// 金額取得
	int GetMony( CString &tag, CString &chtag, char *pArith, int kver );
	// 金額セット
	int SetMony( CString &tag, CString &chtag, char *pArith );
	// 金額セット
	int SetMony( CString &tag, CString &chtag, char *pArith, int kver );
	// 入力年月　開始・最終設定
	int	GetMofs();
	// 月日よりオフセット返送
	short GetShinOfst( unsigned char *date, int *ofst, struct _VOL1 *Vol1, struct _VOL2 *Vol2 );
	// 閏年？
	BOOL IsLeapYear( int year );
	// 末日チェック
	int ChkLastDayEntry( int month, int day, int sw );

	// 履歴関係 まとめ
	CSnset curset, pastset;

	//== 履歴関係 ==
	// 履歴関係の情報セット
	void SetSnHeadToCSnset( const CSnHeadData& Snh );
	// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
	void AlreadyDecisionCalc( int type );
	// 中間及び期間短縮の更新
	int RenewSyzShin();
	// 消費税申告書集計
	int SyzShinWrite();
	// 履歴クラスの取得
	CSnset& GetSnset(){ return curset; };
	// 申告不可能期間？
	BOOL IsNotOutKikan( CSnset &snset );
	// 過去データ更新
	int SetPastData();
	// 既確定税額のクリア('15.01.28)
	int ClearAlreadyDecision();
	// 申告不可能期間？('15.02.23)
	BOOL IsImpossibleKikan( CSnset &snset, CString &msg );
	// 予定申告の月数クリア('17.02.09)
	int ClearYoteiMonth();

	// 消費税申告書集計
	int SyzShinCalq();
	// 申告書集計
	int ShinCalq( int middle_calq=FALSE );
	// 入力項目チェック
	int CheckEntry( int sign );
	// ９５％ルール改正対応期間チェック
	int CheckTaxReforms( int type );
	// 課税売上高５億円チェック（連動用）
	int CheckConnectTaxationSales();
	// 課税売上高５億円チェック 自己計算用
	int CheckUnconnectTaxationSales();
	// 課税売上高５億円チェック処理まとめ
	void CheckOver500Million();
	// 仕入控除税額との連動
	int ConnectedSkj();

	void SetOldData();

	CTime old_tansyuku;
	BYTE old_Sn_SKKBN;
	BYTE old_Sn_KZTNSK;
	BYTE old_Sn_MCOUNT;

	int VerCheck();
	void VerUpExec();

	void HistoryReportInit( void );							// 履歴情報の表示
	void HistoryReportInitCur( void );
	int NextShinkokuCheck(CString& mes);
	int NextShinkoku( int save );
	CSnset* GetRirekiSnset(int (CSnset::*)());

//2016.06.22 INSERT START
	int Tyukan_Kitei( int mcount ); //中間申告の規定回数取得
	int Message_BlankKinyu_Honpyo();
//2016.06.22 INSERT END

	CHistoryReport::VectorType HistoryVec;
	void CreateShininfo(CDatabase* db);
	int SavePastShinkokuData(int delflg);
	int	MakeJikaiShinkokuData(int save);

	CTime FindTansyukuKaishi();
	void SetTansyukuKaishi();

	int CheckShinMinfo();
	TimePairVector GetMinfovecOrg();
	TimePairVector GetShinMininfovec();
	void CleateShinMininfo();

	int IsMinashi();
	void HistoryModeOff();
	int IsHistoryMode();
	int IsExistsHistory();
//-- '14.09.10 --
//	int DropHistoryAll();
//---------------
//--- '15.01.13 ---
//	int DropHistoryAll( CString& msg );
//-----------------
	int DropHistoryAll( CString& msg, BOOL isExistHistory );
//-----------------
//---------------

//	int	DeleteOldKjsnkData();


	TimePairVector GetTimeVector(int mcount);
	TimePair GetCurrentTyukanKikan();
	TimePair GetCurrentKessanKikan();
	TimePairVector GetTyukanTimeVector();
	TimePairVector GetKessanTimeVector();
	TimePairVector GetMinashiKazeiKikan();
	TimePairVector GetMinashiTyuukanKikan();

	UINT GetKikanOfs(TimePairVector& timevector, TimePair t );

	CHistoryReport::RecType* GetZenkaiShinkoku();
	vector<pair<int,CString>>  GetVectorSkbnName();
	map<int,int> GetMapKubun(int swap_sw = FALSE);

	int SyzEdit();
	int IsEnableNextShinkoku();
	CString GetNextShinkokuMessage();

	void ChangeComboTyukanKikan();
	void ChangeComboKessanKikan();
	void ChangeComboSkkbn();
	void ChangeRirekiSel();
	
	void SetComboTyukanAri();
/*- '14.02.19 -*/
//	void SetComboSkkbn();
/*-------------*/
/*-- '14.02.22 --*/
//	void SetComboSkkbn( CSnHeadData& Snh );
/*---------------*/
	void SetComboSkkbn();
/*---------------*/
/*-------------*/
	void SetComboTyukanKikan();
	void SetComboKessanKikan();
	void SetComboKikanTansyuku();
	int SetKikan();

	int SetCtrl();
	void SetHistoryCtrl(int);
	void SetCurCtrl(int);


	int SyokaiCheck();
	int CheckChange();
	int IsShinkokuChanging();
	int IsKikanTansyuku();
	int	IsTyukanAri();
	int IsKityuuKakutei();
//2018.03.13 INSERT START
	int IsKojin();
	int IsMynumber();
	int IsJunkakutei( int sgn );
//2018.03.13 INSERT END
	int CheckKaiseiVER( unsigned char& newver );

	int DropHistory();
	int ReportGetCount();
	int GetHistoryCurSel();
	int GetHistorySeq();
	int SetCurHistoryCheck();
	int ChangeLockMode(CWnd* wnd);
/*- '14.02.22 -*/
//	void GetComboTyukanKikan( CSnHeadData& Snh );
/*-------------*/
	void GetComboTyukanKikan();
/*-------------*/
	int GetMonthCnt();
	int GetMonthSel();
	int GetSkkbnSel();

/*- '14.02.22 -*/
//	void SetCurrentSkkbn( CSnHeadData& Snh );
//	void SetCurrentMcount( CSnHeadData& Snh );
/*-------------*/
	void SetCurrentSkkbn();
	void SetCurrentMcount();
/*-------------*/
	map<int,int> GetMapOfsKubun( int swap_sw = FALSE );
	map<int,int> GetMapMcount( int swap_sw = FALSE );

/*- '14.02.22 -*/
//	void ChangeTyukanKikanTansyuku( CSnHeadData& Snh );
/*-------------*/
	void ChangeTyukanKikanTansyuku();
/*-------------*/
	void ChangeMonthCnt();
/*- '14.02.22 -*/
//	void SetMonthCnt( CSnHeadData& Snh );
/*-------------*/
	void SetMonthCnt();
/*-------------*/
//shimizu追加/////
	//還付明細書印刷関係
	int SkjCheckList( int pgno );

//////////////////
	int GetTimeToJcount(int mcount, TimePair time);
	int ShinKinfoWrite(vector<MPAYMENTVAL>& vec);
	vector<MPAYMENTVAL> ShinKinfoRead();
	vector<MPAYMENTVAL> OldKinfoRead();
	vector<MPAYMENTVAL> MiddlePaymentRead(int old_dataread = FALSE);
	int WriteNofu();
	int KanendoZei();
	int GetZei();
	void ClearZei(int flg);
	int BasPerideTaxSold( int sign );//
	//一括印刷
	int ForkJob();
	int SumUpPrintIntinitial( void );
	void GetElectronicReport();

	//== マイナンバー ==
	// マイナンバーコントロールの初期化
	int InitMynumberControl();
	// データの取得
	int GetData( int kubun, int sn_seq, CString& data );
	// 区分に合わせたコントロールの再配置
	void MoveEditControl( int kubun, int sn_seq );
	// マイナンバーの最終インデックスを取得
	int GetMNLastIndex();
	// マイナンバー入力コントロールから他のコントロールへ移動する処理
	int MNEditTermination( int index, MSG* pMsg );
	// マイナンバー：コントロールEditOff
	int MNEditEditOff( int index );
	// マイナンバーのチェック
	BOOL CheckMNumber( BOOL isZeroOK, int isShowMes );
	// 入力ロック中？
	BOOL InLockInput();
	// 個人番号削除ボタンの表示／非表示を切り替える
	void ChangeDispKjdelBtn();
	// マイナンバー入力コントロールにデータをセットする
	int SetMNData();
	// コントロールから実データへセット
	int SetHeadDataFromControl();
	// 個人番号の登録
	int RegistKojinMN();
	// 個人番号履歴情報セット
	void SetNoHistory();
	// 個人番号履歴情報セット（顧問先情報）
	void SetCoNoHistory();
	// 空の個人ハッシュ？
	BOOL IsEmptyKjhash( char *phash, int size );
	// 個人番号を全体から削除
	int DeleteKjhashFromNoDB();
	// 個人番号ハッシュ値を削除
	int ClearKjhash( bool isWriteHistory );
	// マイナンバーデータテーブルに個人番号ハッシュ値をセット
	int SetKjhashToNdtbl();
	// 終了時に影響を受けないコントロールにフォーカスを移動
	void EndJobFocus();
	// 個人番号削除
	int DeleteKjno();
	// 法人番号から情報を取得
	int GetHjInfoFromHjno();
	// 印刷情報のクリア('15.12.21)
	void ClearPsetInfo();
	// 空の個人ハッシュ？
	bool IsEmptyKjhash();

//2015.12.03 INSERT START
	// マイナンバー履歴を書き込む
	void WriteNoHistory(int write_sgn);
	// 個人番号を出力するか
	BOOL IsPrintKojinNo();
//2015.12.03 INSERT END
//2016.02.23 INSERT START
//2018.08.21 UPDATE START
//	BOOL IsPrintableKojinNo();
	BOOL IsPrintableKojinNo(char *print_sgn = NULL);
//2018.08.21 UPDATE END
//2016.02.23 INSERT END

	//== 特定課税仕入 ==
	// 改正様式は？('15.08.27)
	EnumIdIcsShRevType GetShRevType();
	// 改正様式は？('15.08.28)
	EnumIdIcsShRevType GetShRevTypeFromSnset( CSnset& snset );
	// 特定課税仕入の状態をセット('15.08.27)
	void SetTkkzSign( long eymd );
	// 特定課税仕入有りチェックの状態をセット('15.09.09)
	void SetTkkzChk( int onoff );
	// 特定課税仕入有りチェックの状態をセット('15.11.14)
	void SetTkkzChk();
	// 文字列データをサインに分解（'15.11.16）
	int SepStrToSign( long *pDispTabSgn, CString &srcStr );

//2015.08.04 INSERT START
	// KSK印刷時のメッセージ作成
//	void GetPrintMessage( char PRtable, CString& msg );
//2015.08.04 INSERT END

//2016.02.23 INSERT START
	//「入力ロック」のフォントを太字に
	int Make_StaticFont();
	//「入力ロック」の表示・非表示
	void Static_Update( unsigned char Sn_Sign4 ); //CSnHeadDataのSn_Sign4
//2016.02.23 INSERT END

	// 法人番号付き用紙での印刷？
	bool IsPrintWithMynoHonpyo();
	// グループテキスト再描画
	void RedrawGroupText();


	// 特例計算チェックの状態をチェック
	void SetTkreiChk();
	// 特例計算チェックの状態をセット
	void SetTkreiChk(int onoff);
	// 令和5年9月30日をまたぐ期間かどうか
	bool IsCrossOverR050930( long smd, long emd );

	// ８％データの削除
	int DeleteH26LstVal();

	// 令和2年4月1日改正様式を使用するチェックの状態をチェック
	void SetR02Chk();
	void SetR02Chk ( int onoff );
	void SetR02Sign ( long eymd );

	// 2割特例のチェックの状態をセット
	void Set2wariChk();

	// 簡易計算のチェックの状態をセット
	void SetKaniKeisanChk();

	// 積上げ計算を行うのチェックの状態をセット
	void SetTumiageChk();

	// 特定収入計算表未対応。期間を見て対象期間の場合処理制御 yoshida190728
	BOOL IsSpc31OutKikan(CSnset &snset);

	// 起動中のビューのバージョンの取得
	int GetActViewVer();
	// 起動中のビューのバージョンを設定（ビュー情報を使用）
	int SetActViewVer(CSnset *pSnset);
	// 起動中のビューのバージョンを設定（ヘッドデータを使用）
	int SetActViewVer(CSnHeadData *pSnHeadData);
	// メインアプリで管理している改正バージョンの取得
	int GetKaiseiVer();
	// 本表ビューの取得
	CBaseHyoView* GetHonHyoView(int vtype=-1);

	// 課税期間の取得
	void GetKazeiKikan( long &symd, long &eymd );
	void GetKazeiKikanfromSnset(CSnset &snset, int &symd, int &eymd);

	// 課税期間の開始日を取得
	long GetKazeiKaishibi();

	// インボイスの期間マスター？
	bool IsInvoiceMaster();

	// 課税期間における課税売上高の入力が可能か？
	bool IsInputKazeiUriage();

	// 課税期間における課税売上高の入力が必要？
	bool IsNeedInputKazeiUriage();

	// 課税期間における課税売上高の保存コード取得
	CString GetKazeiUriageCode();

	// 課税期間における課税売上高を取得
	void GetKazeiUriagedaka( char *pArith );

	// 選択中の課税期間の取得
	void GetSelectKazeiKikan( long &sYmd, long &eYmd );

	// 仕訳に紐づいている登録番号のチェック
	int CheckWrongInvoiceData( CString &errMsg );

//----->('23.09.14 Tanaka Add)
	//転記処理
	void TenkiData();		//課税期間末日が令和5年10月1日以前の転記処理
	void TenkiInvData();	//課税期間末日が令和5年10月1日以降の転記処理
//<-----

	// 部数関連
	CString GetBusuItmName( int pSw1, int pSw2, CString pKeyName );
	int		m_BusuSave[50];
	BOOL	IsColorSelect( SH_PRINT_CMINFO* pPrInfo, CSnHeadData* pSnHead );
	BOOL	IsBusuSelect( SH_PRINT_CMINFO* pPrInfo );
	char	GetPRtable( SH_PRINT_CMINFO* pPrInfo, int pNo );
	int		GetPGtable( int pNo );
	int		GetPrOfs( char pNo );
	CArray<char>	m_PRtableEx;
	CArray<int>		m_PGtable;		// 複数枚出力される帳票用	
									// 複数部数を指定された場合も現在のページ番号が分かるようにする
									// (Fhy6Ct 等の代わり)
	CArray<int>		m_PRtableEx1;	// 1部目の並び順を格納


	//汎用
	int IsBitFlg(int target, int BitFlg);
	void SetBitFlg(int& target, int BitFlg);

	int Kazei_DButton_sw;
	int Toku_DButton_sw;
	int Zeiri_DButton_sw;

// 24/03/26_税務代理書面 add -->
	int	m_Fnt108gt;
	int	m_Fnt090gt;
// 24/03/26_税務代理書面 add <--
// 24/04/01_246 add -->
	int					m_GetSki;
	KMNCTL_KANRYO_DATA	m_Kanryo;
// 24/04/01_246 add <--

// 24/06/13_税務代理書面 add -->
	void CallWriteTaxAgentDB( CDBNpSub* pZms, CSnHeadData* pSh, CWnd* pWnd );
// 24/06/13_税務代理書面 add <--

private:
	CMainFrame	*m_pMainFrame;	// 親ウィンドウ
	CDBNpSub	*m_pZmSub;		// 公益クラス
	CVPrint		m_Vprn;			// 出力クラス
	CSyzUtil	m_Util;			// ユーティリティクラス
	CArithEx	m_Arith;		// 倍長演算
	CTblhdl		*m_pTblhdl;		// テーブルハンドリングクラス
	CBrush		m_hBrushBkgnd;	// 背景ブラシ

//2016.02.23 INSERT START
//	CBrush		m_hBrushLock;
	CFont		m_font;
	ICSStatic m_InputLock;
//2016.02.23 INSERT END

	CRect		m_rcHrep;		// 履歴レポート移動位置
	BOOL		m_IsInitFlg;	// 初期化完了フラグ
	BOOL		m_IsDispedFlg;	// 初期描画完了フラグ
	BYTE		m_SvSn_Sign4;	// 消費税サイン
	BYTE		m_SvSVmzsw;		// 複数税率サイン
	long		m_SvDispTabSgn;	// タブ表示サイン('15.08.21)

	int			m_PrintType;	// 印刷タイプ	0:申告書 1:仕入控除税額に関する明細書チェックリスト
	NPRT_PAR	m_Pset;			// プリンタ情報
    PCOL_EXT	m_Cset;			// カラー定義パケット

// -> m_PrintCmInfo
//	BOOL		m_isColorPrn;	// カラープリンタ？
	long		m_PrFlg;		// プリント初期設定フラグ

	PBUF		PF[30];


	short		m_PagePrn;		// ページ番号印刷サイン
	short		m_CntPage;
	short		m_MaxPage;
	short		m_TopPage;
	
	int			m_swBeFork;		//一括印刷フラグ
	SH_PRINT_CMINFO	m_PrintCmInfo;	// 印刷情報

// 240308_h_160357 add -->
	int				m_Tokutei;
// 240308_h_160357 add <--

	int			m_isScoreLinkCall;	// ScoreLinkからの呼び出しか 20240116
	CString		m_ScoreLinkPdfNam;	// ScoreLinkで使用するpdf名  20240116

	int				PageNo;
// -> m_PrintCmInfo
//	int				PgMx;		// 出力枚数サイン（複写は含まない）

// -> m_PrintCmInfo
//	char			PRtype;		// |･| | |･|･|･|･|･|
								// ON 　  | | | | ON:申告書本表出力
								// 版     | | | ON:付表１又は４出力	
								// 下     | | ON:付表２又は５出力
								// 出     | ON:付表２-(2)又は５-(2)出力
								// 力     ON:付表３出力
// -> m_PrintCmInfo
//	char			PRtable[50];	// 仕入控除税額に関する明細書（次葉）

	int				HP, VP, DHP, DVP;
	int				PcSw;
	int				m_TaxReforms;	// ON:改正以降
	unsigned char	AdSw;
	unsigned char	KdNo;
//	CString			m_PrinterName;			// プリンタ名

	int				nList[128];							//
	char			Hpath[64];							// 消費税申告書スケジュール

	//== 消費税の還付申告に関する明細書 ==
	char			m_KindSg;							// D0:主な課税資産の譲渡等 D1:主な輸出取引等の明細 D2:主な棚卸資産・原材料等の取得 D3:主な固定資産等の取得
	char			m_KindPr;							// リザーブ
	short			m_KindSw;							//  1:主な課税資産の譲渡等  2:主な輸出取引等の明細  3:主な棚卸資産・原材料等の取得  4:主な固定資産等の取得
	int				TkList;								// 作表ハンドル(課税期間)
	int				m_KindPg;							// 次葉ページ番号（申告選択で次葉を選択時）

	int				m_Refrnum;
	int				m_RefJotTop;							// 主な課税資産の譲渡等
	int				m_RefJotMax;							// 〃
	int				m_RefExpTop;							// 主な輸出取引等の明細
	int				m_RefExpMax;							// 〃
	int				m_RefTnaTop;							// 主な棚卸資産・原材料等の取得
	int				m_RefTnaMax;							// 〃
	int				m_RefKotTop;							// 主な固定資産等の取得
	int				m_RefKotMax;							// 〃
	_TB_PAR			*m_pSkjCnvtbl;							// 仕入控除用テーブル

	//== 微調整用テーブル ==
	SH_MvTbl	m_MvRec[30];	// 仕入控除税額に関する明細書（次葉）
	SH_PRMOVE	m_PrMv[30];
	SH_PRMOVE	m_PrMove[30];
//	int			m_OPEla;		// 伸縮数

	// 特定収入
	// 消費税集計テーブル
	struct TB_PAR	SpTbl;
	struct TB_PAR	TsTbl;

	int				m_Specnt;							// 特定収入マスター登録数
	int				m_Class;							// クラスサイン
	int				m_SpcSg;							// 特定収入処理サイン
	int				m_SpcPr;							// 計算表印刷サイン
	int				m_Splist;							// 計算表マスター登録数
	int				m_SpcCq;
	int				m_SpcTk;
	int				m_SpcCp;							// 会社名・コード印刷サイン
	int				m_SpcCt1;							// 計算表21
	int				m_SpcMx1;							// 〃 
	int				m_SpcTp1;							// 〃
	int				m_SpcCt2;							// 計算表22
	int				m_SpcMx2;							// 〃 
	int				m_SpcTp2;							// 〃
//	SPCLIST			SpcList;							// 計算表マスター

//	struct	_SPCVAL	Spval;								// 特定収入計算表 [09'04.14]


	//== チェックボックス関連 ==
	int				m_K221sw;							// 規則２２条１項の適用（積み上げ方式）
	int				m_SimpSw;							// 簡易計算サイン
	int				m_H301sw;							// 法第３０条第１項の適用（積み上げ方式）
	char			m_toku;								// 特例計算適用ＳＷ
	char			m_Kenei;							// 兼営事業選択ＳＷ

	int				m_MatuDay;//Matu					// 末日

//shimizu追加/////
	//還付明細書印刷関係
	COLORREF	Lcol[3];								// 罫線
	COLORREF	Scol[3];								// 網掛

	unsigned char	Save_Head[5];
	unsigned long	Save_Headlong;
	unsigned long	Save_Headlong2;
//////////////////

	bool			m_SvIsPrintableKojinNo;				// 個人番号を出力可能かの前回情報を保存（'16.12.08）

//2018.08.21 INSERT START
	BOOL			m_MyNumBrowse;						// マイナンバーを閲覧したか記憶(画面表示のみ)
//2018.08.21 INSERT END

// 211203h add -->
	int				m_PnoNougyou;
	int				m_PnoFudousan;
// 211203h add <--
// kasai add -->
	int				m_PnoJigyo;
	int				m_PnoSell;
	int				m_PnoBuy;
// kasai add <--
// 220412 add -->
	int				m_PnoKuchiTC;	// 提出用カラー
	int				m_PnoKuchiTM;	// 提出用モノクロ
	int				m_PnoKuchiHC;	// 控　用カラー
	int				m_PnoKuchiHM;	// 控　用モノクロ
// 220412 add <--

public:
	// 消費税マスターSVhead
/*
	SH_SVHED		TBhead;
	SH_SVHED		SVhead;
//	H9SUM		SVsum;
	SH_SN_HEAD		Snh;
	SH_SN_DATA		Snd;	
//	SN_PPRT		Snp;*/

	//== 帳票データ ==
	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報

	SYC_SHININFO	*m_pShinInfo;		// 共通情報

	// 総合＆一括
	// 消費税マスター
//	SH_SVHED		SVHeadEx;
//	SH_SN_HEAD		SNHeadEx;
//	SH_SN_DATA		SNDataEx;	

//	int				m_Middle;	// 前年実績による中間申告サイン
								// 本当は、別のクラス内に保存すべき？
//	int				Anbun;		// ﾎﾞﾘｭｰﾑﾗﾍﾞﾙ  1=個別対応  0=比例配分
	int				m_Anbun;	// ﾎﾞﾘｭｰﾑﾗﾍﾞﾙ  1=個別対応  0=比例配分

	int				m_Printsw;	// ON:プレビュー、印刷中

	BOOL			m_SixKindFlg;		// SixKindFlg = TRUE・・第六事業種あり	150106

	//== マイナンバー('15.07.21) ==
	CICSMNEditController8*	m_pMnec;		// マイナンバーエディットコントロール
	SYZ_MYNO_SVINF			m_svMynoInf;	// マイナンバー情報保存
	NODATATBL				m_ndtbl;		// マイナンバーデータテーブル
	NOHISTTBL				m_nhtbl;		// マイナンバー履歴データテーブル

	//----->特定課税仕入対応150713
//	BOOL					m_TokuteiFlg;		// m_TokuteiFlg = TRUE・・特定収入チェックボックス表示（期間で判定）　FALSE・・非表示
	BOOL					m_IsAbleTkkz;		// 特定課税仕入の選択が有効？
	EnumIdIcsShRevType		m_ShRevType;		// 改正様式('15.08.27)
	//<------------------------------

	BOOL					m_IsAbleTkrei;		// 特例計算の選択が有効

	BOOL					m_IsAbleR02Reform;	// 令和2年4月1日改正が有効

	//19.11.07
	BOOL					m_Drophistorysw;	// 前回申告戻り時SWがTRUEとさせる。税額情報のコンバージョンを行わない為のサイン

	BOOL					m_msgdisp;			//TRUE:メッセージ表示　FALSE：メッセージ非表示　（消費税インボイス制御メッセージの部分で使用。）'23.04.04

	//takemura
//	int				m_Tokurei_USign;					//特例計算　売上税額計算特例　サイン
//	int				m_Tokurei_SSign;					//特例計算　仕入税額計算特例　サイン

// オーバーライド
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後にはじめて呼び出されます。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);

// 実装
public:
	virtual ~CDBSyzShinMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	LRESULT OnUserUnlock(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserSyzInit(WPARAM wParam, LPARAM lParam);//'14.10.08
	LRESULT OnUserTenkiData(WPARAM wParam, LPARAM lParam);//'14.10.08
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	LRESULT OnMessageFromPrint( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
public:
	CGroupStatic m_BaseGroup;
	CGroupStatic m_CalqGroup;
	CGroupStatic m_InfoGroup;
	CHistoryReport m_PastReport;
	CComboBox m_ComboMonthCnt;
	CComboBox m_ComboSkkbn;
	CComboBox m_ComboKikanTansyuku;
	CComboBox m_ComboTyukanAri;
	CComboBox m_ComboTyukanKikan;
	CColorComboBox m_ComboKessanKikan;
	CButton m_CheckTokusyu;
	CButton m_ButtonRirekiSel;
	CButton m_CheckMinashi;
	CTransStatic m_StaticB01;
	CTransStatic m_StaticB02;
	CTransStatic m_StaticB03;
	CTransStatic m_StaticB04;
	CTransStatic m_StaticB05;
	CTransStatic m_StaticB06;
	CTransStatic m_StaticB07;
	CTransStatic m_StaticB08;
	CTransStatic m_StaticB09;
	CTransStatic m_StaticB14;
	CTransStatic m_StaticB51;
	CTransStatic m_StaticB52;
	CTransStatic m_StaticB53;
	CTransStatic m_StaticB54;
	CTransStatic m_StaticB55;
	CTransStatic m_StaticB56;
	CTransStatic m_StaticB57;
	CTransStatic m_StaticB58;
	CTransStatic m_StaticB59;
	CTransStatic m_StaticI01;
	CTransStatic m_StaticI02;
	CTransStatic m_StaticI03;
	CTransStatic m_StaticI04;
	CTransStatic m_StaticI05;
	CTransStatic m_StaticI06;
	afx_msg void OnCbnSelchangeSkkbnCmb();
	afx_msg void OnCbnSelchangeMonthCmb();
	afx_msg void OnCbnSelchangeKikantCmb();
	afx_msg void OnBnClickedCheckTokusyu();
	afx_msg void OnBnClickedCheckRirekisel();
	afx_msg void OnCbnSelchangeTyukanKikanCmb();
	afx_msg void OnBnClickedInputBtn();
	afx_msg void OnBnClickedCheck21();
	afx_msg void OnBnClickedMinashiChk();
	CButton m_Check21;
	CGroupStatic m_RInfoGroup;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_Check1;
	CButton m_Check2;
	CButton m_Check3;
	CButton m_Check4;
	CButton m_Check5;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedTnozeBtn();
	CTransStatic m_StaticHelp;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonF9();
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
//	afx_msg void OnUpdateShinPayment(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZeirishiInf(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
//	afx_msg void OnKomonsakiInf();
	afx_msg void OnCbnSelchangeTcountCmb();
	afx_msg void OnUpdateKomonsakiInf(CCmdUI *pCmdUI);
	void OnNMReportSelchangedPastReport(NMHDR * pNotifyStruct, LRESULT * result);
	CICSMNEdit8 m_MnEdit1;
	CICSMNEdit8 m_MnEdit2;
	CICSMNEdit8 m_MnEdit3;
	CICSMNEdit8 m_MnEdit4;
	CTransStatic m_StaticB10;
	CTransStatic m_StaticB11;
	CTransStatic m_StaticB12;
	CTransStatic m_StaticB13;
	afx_msg void OnBnClickedButtonKjdel();
	CButton m_KjdelBtn;
	CButton m_Check6;
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedDenshiInfo();
	CButton m_Check7;
	afx_msg void OnClickedCheck7();
	afx_msg void OnBnClickedButton1();
	CButton m_btn1;
	afx_msg void OnCbnSelchangeKessanKikanCmb();
	CButton m_Check9;
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnUpdateKoujoSelect(CCmdUI *pCmdUI);
	CButton m_Check10;
	afx_msg void OnBnClickedCheck10();
	CTransStatic m_StaticC01;
};

#ifndef _DEBUG  // DBSyzShinMainView.cpp のデバッグ バージョン
inline CDBSyzShinMainDoc* CDBSyzShinMainView::GetDocument() const
   { return reinterpret_cast<CDBSyzShinMainDoc*>(m_pDocument); }
#endif

