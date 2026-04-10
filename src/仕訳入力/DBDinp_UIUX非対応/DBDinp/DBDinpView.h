// DBDinpView.h : CDBDinpView クラスのインターフェイス
//


#pragma once
#include "afxwin.h"

#include "icssel.h"
#include "icsdbedt.h"
#include "icsdisp.h"
//}}AFX_INCLUDES
#ifdef _ICSPEN_	// 01.20 /01
//#include "ICSPen.h"
#endif

#include "icsdisp.h"

#include "DataListCtrl.h"
#include "DtClmItem.h"
#include "DataListLine.h"
#include "rdtatbl.h"
#include "icsdateselect.h"
#include "DBbaseview.h"


#define FOCUS_DEBUG (0)   //0:Release / 1:Debug
#define INPLINE_DEBUG (0)   //0:Release / 1:Debug


#ifdef _SLIP_ACCOUNTS_
// 原票ボタンの状態
typedef enum {
	ID_DISPSLIP_BSTATE	=	0,	// 原票表示
	ID_LINKON_BSTATE,			// リンクON
	ID_LINKOFF_BSTATE			// リンクOFF
} EnumIdSlipdispBtnstate;

// 原票表示ダイアログの状態
typedef enum {
	ID_DBDINP_NODISPDLG_TYPE	=	-1,	// 表示無し
	ID_DBDINP_INPUTDLG_TYPE		=	0,	// 入力補助
	ID_DBDINP_CONFIRMDLG_TYPE,			// リンク確認
	ID_DBDINP_ADDLINKDLG_TYPE			// リンク追加
} EnumIdDispImgDlgType;

//== 製品コード ==
static	char	*ICS_IMGMNG_OCR_PACKAGE_CODE	=	_T("554");	// 新ＯＣＲでの伝票読み取り（コクヨ伝票を含む）
static	char	*ICS_IMGMNG_SLIP_PACKAGE_CODE	=	_T("555");	// 新ＯＣＲで原票会計の読み取り
#endif

class CDBDinpView : public CDBbaseView
{
protected: // シリアル化からのみ作成します。

	CDBDinpView();
	DECLARE_DYNCREATE(CDBDinpView)

public:
	enum{ IDD = IDD_DBDINP_FORM };
//	CICSSel	m_selval32;
//	CICSSel	m_selfix8;
	CICSDisp	m_GEN1;
	CICSDisp	m_GEN2;
	//}}AFX_DATA
	CDataListCtrl	m_DtList;
	CDataListLine	m_DtLine;
	CICSDBEDT	m_Input;
	CICSDBEDT	m_Inp2;
	ICSDateSelect m_selDate;

// 属性
public:
	CDBDinpDoc* GetDocument() const;

	int		m_NOBRNCHK_CND;
	int		m_BMKJICHK_CND;	// 下位から 1 バイト目 -- 工事未入力チェック
							// 下位から 2 バイト目 -- 完成工事チェック
							// 下位から 3 バイト目 -- 部門未入力チェック

	//会社切替時の画面フラグ
	int		m_reCoDispType;

protected:
	BOOL	m_AutoInp_use;
	short	m_TEK_MODE;

	BOOL	m_ToSkbnDlg;
	BOOL	m_bSyzYunyu;		// 消費税が輸入仕入に変更されたか？
	BOOL	m_bSyzSotoMen;		// 外税で免税事業者からの課税仕入れ
	BOOL	m_pnZeiSotoMen;		// 外税で免税事業者からの課税仕入れで税額欄へ フォーカスした前ポジション
//	int		m_SET_SKBNIN_PN;	// 消費税ダイアログフォーカスポジション
	BOOL	m_bHusenFocus;
	int		m_OneHusen;			// 1行時の付箋か？ その場合は、ONEHUSEN_PN をセット

	int		m_TabKeyStop;

	BOOL	m_bSwkCopy;
	BOOL	m_bSwkPaste;

	BOOL	m_ScanWndMove;
	BOOL	m_bScanMode;

	int		m_SlipPakageCd;

	int		m_InsLine;		//挿入の行位置
	int		m_InsMode;		//挿入時の入力モード [_MODEFY or _KEYSCAN]
	int		m_InsSeq;		//挿入の行の前後のSEQ

	CDBINPDataRec m_InsSREC;
	CArray<CDBINPDataRec, CDBINPDataRec&>	m_InsRecArray;	//画面表示用

	int		m_DenpSeq, m_DenpICNO;

#ifdef CLOSE_MAINFRAME
	CoSel cosel;				//会社確認構築
#ifdef _ICSPEN_	// 01.20 /01
	CoSel coselpen;				//会社確認構築
#endif
#endif

	int		prevImgSeq;
	BOOL	IsImgDlgChanged( int setDtseq );
	//
	BOOL	ddata_dspFlg;

	BOOL		m_CalcFlg; /*電卓表示サイン*/
	ULONG		m_CalcOFF; /*電卓表示OFF の時間*/

public:

	void StartBlock();
	void EndBlock( int );
	// 仕訳フル時のメッセージ
	char *get_Dfullmsg();
	void set_firstfocus( int ReCosel = 0 );

	void Recosel_Job();
	void ReCoselCheckOtherPro();

	void SetICSSel8( long selval, BOOL bNumber = TRUE );
	void SetICSSel32( long selval, BOOL bNumber = TRUE );

	BOOL is_AutoInp_use();
	void AutoInp( int pn, BOOL bNXT = FALSE );
	void AutoSelIndicator( struct _AUTOSEL *sel, int inp_mode );

//	long SelItemDsp( long nChar );
	void Dsp_TkyToItem(BOOL bUpdate =FALSE );
	int rentky_srdsp( CString deb, CString cre );
//	int rentkydsp( void );
//	void rentkysr( int, struct _KNREC * );
//	void _rentky_str( CString debt, CString cred ); // 03.06 /02
	int RTpmax_Get();

	BOOL dinp_modefy_ok( struct _DATA_LINE *dtline );
	BOOL isdeldata();
	BOOL _isdeldata( int ln );

	void tabchg_saveinfo( BOOL bZanUp );
	void to_scankey();
	void to_inpmode();
	void to_scanmode( int seq_send = 0 );
	void to_scanmode2( int seq, int now_seq );

	void to_appendmode();
	BOOL& ScanModeSign() { return m_bScanMode; };

	void SaveDinpDataNOW_PN( int inp_mode );
#ifdef CLOSE
	void SaveDinpDataSREC( int inp_mode );
#endif
	void SaveDinpParaSREC( int inp_mode );

	void _save_inpcondition();

	BOOL brnitm_to_tky( int ln, int bmn, CString& code, int brn );

	int totaldisp( int mode );
	void save_scanview( CView *pn );
//	BOOL IsEnableHjiyu( CDBINPDataRec *pcrec );

	BOOL GetDtListEnable( int ln, int pn );

	// パラメータセーブ
	void SaveDinpPara( int inp_mode );
	void ReSetDinpPara( int inp_mode );
	void InizDinpPara( int inp_mode );

	virtual void LastDataGet();
	virtual void ForkUcomSet();
	virtual int  ReloadView();
	virtual void ReCoselSetFocus( BOOL bNewCo );
	virtual void OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual void CalcMouseEvent();
	virtual BOOL EnableCallPro();

	BOOL bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw, CString* bmnname );
	void ReCosel_DinpData();
	// MainFrame Call 用 --- 05.13 /11
	void CDBDinpView::SaveDinpParaFunc();

	// サイズ変更処理
	void OnSizeJob();
	// DBedit OnSize
	void DBeditOnSizeJob();

#ifdef _SLIP_ACCOUNTS_
	// サブウィンドウ移動
	int MoveSubWindow( int sw );
#endif
	void DispBmpMake( int mode );

	BOOL IsBlkInpGnkinDevide();
	void BlkWndMove( int nKeyScanFlg = -1 );

	void BlkMoveOnSizeJob( int beginApp = 0, int bOnSize = 0 );

	void SelWndShow( BOOL bShow );
	void SelWndMoveJob( BOOL bShow );

	void fix8disp( BOOL sg );
	int	 SelWndDispJob();

	int	 get_InsLine() { return m_InsLine; };
	void insert_clear();

	void BmnIMEkeydownJob(long nChar);
	void SetDtListItemWidth();
	void GetDtListItemWidth();

	void bmneda_update(CDBINPDataRec* rec, int dc_sw);


protected:
	void set_etcdsp( int ln, int pn );
	BOOL IsSCAN_TKYONLY();

	void FocusIcsSeq(CWnd* pwnd, int ln, short click );
	void FocusIcsDate(CWnd* pwnd, int ln, short click);
	void FocusIcsDenp(CWnd* pwnd, int ln, short click);
	void FocusIcsBmon(CWnd* pwnd, int ln, int dc_sw, short click );
	void FocusIcsKoji(CWnd* pwnd, int ln, int dc_sw, short click );
	void FocusIcsDebt(CWnd* pwnd, int ln, short click);
	void FocusIcsCred(CWnd* pwnd, int ln, short click);
	void FocusIcsVal(CWnd* pwnd, int ln, short click);
	void FocusIcsZei(CWnd* pwnd, int ln, short click );
	void FocusIcsSyz(CWnd* pwnd, int ln, short click, int focs_sw );
	void FocusIcsTky(CWnd* pwnd, int ln, short click);
	void FocusIcsSyono( CWnd* pwnd, int ln, short click );
	void FocusIcsTgDate( CWnd* pwnd, int ln, short click );
	void FocusHusenInput( CWnd* pwnd, int ln, short click );
	void FocusIcsHjiyu( CWnd* pwnd, int ln, short click );
	void FocusIcsDocEviNo( CWnd* pwnd, int ln, short click );

//	long TerminationHusenCombo( long nChar );

	void TerminationIcsinputSeq( CWnd* pwnd, long nChar, long inplen, long kst );
	BOOL seq_to_data( int ln, long nChar );
	void SeqDataJob( int seq, int now_seq );
	BOOL seq_to_data_move( int ln, int seq, int now_seq );
	BOOL seq_to_modefy_data( int ln, int seq, int now_seq, int vect );
	BOOL seq_to_keyscan_data( int ln, int seq, int now_seq, int vect );

	long CheckDataIcsinputDate( CWnd* pwnd, long nChar );
	void TerminationIcsinputDate( CWnd* pwnd, long nChar, long inplen, long kst );

	void TerminationIcsinputDenp( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	BOOL auto_cno( int *next_cno, CDBINPDataRec *data, struct _AUTOSEL *mode );

	int  CheckImeEndBumon( CWnd* pwnd, int pn, long nChar, LPCTSTR string );
	void TerminationIcsinputBumon( CWnd* pwnd, int dc_sw, int ln, long nChar, long inplen, long kst );
	
	int  CheckImeEndKoji( CWnd* pwnd, int pn, long nChar, LPCTSTR string );
	void TerminationIcsinputKoji( CWnd* pwnd, int dc_sw, int ln, long nChar, long inplen, long kst );

	long CheckDataIcsinputDebt( CWnd* pwnd, long nChar, LPCTSTR string );
	void TerminationIcsinputDebt( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	void KanaTermIcsinputDebt(LPCTSTR data);
	BOOL TerminationCodeCheckDebt( CString& kmkcd );

	long CheckDataIcsinputCred( CWnd* pwnd, long nChar, LPCTSTR string );
	void TerminationIcsinputCred( CWnd* pwnd, int ln, long nChar, long inplen, long kst);
	void KanaTermIcsinputCred(LPCTSTR data);
	BOOL TerminationCodeCheckCred( CString& kmkcd );

	void TerminationIcsinputVal( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	void TerminationIcsinputZei( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	int TerminationIcsinputSyz( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	void TerminationIcsinputTky( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	void _KanaTermIcsinputTky(LPCTSTR data, BOOL bTkcut);

	void TerminationIcsinputSyono( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	long CheckDataIcsinputTgDate( CWnd* pwnd, long nChar );
	void TerminationIcsinputTgDate( CWnd* pwnd, int ln, long nChar, long inplen, long kst );

	void SelectedIcsselctrl(short position, long Number, int place );
	void SelectedEtcUpdate(int pn);

	void TerminationIcsinputDocEviNo( CWnd* pwnd, int ln, long nChar, long inplen, long kst );
	BOOL auto_docevi( CString& next, CDBINPDataRec *data, struct _AUTOSEL *mode );

protected:
//	RECT m_selfix8_rect;
//	RECT m_valtitle_rect;
//	void valtitle( BOOL sg );

	void disp_change( int oldpn, int pn );
	void _disp_change();

	void sel_inpmode( int pn );
	void SelKmkUpdate( int inp_mode );
	BOOL isUSE_AUTOSEL( struct _AUTOSEL *sel, BOOL APPEND_MODE );

	void dtlist_dispopt();

	int	 swkline_dispchg( int oldln, int nowln, int* pFocusSgn );
	int	 swkline_keyscan_dispchg( int oldln, int nowln, int* pFocusSgn );

//	void ScanKeyMake();
//	void ScanKeyDisp();
	void SkbnInMake();
	int	 SkbnInDisp(long idc);
	void Destroy_SkbnIn(int back_color=FALSE);	//消費税画面消去
	void Destroy_SkbnInX();

	void BkInpDisp( int pn );
	void BkInpMakeX();
	void BkInpMake();
	void Destroy_BkInp(); // 部門／工事入力画面消去

	void HusenInpDisp( BOOL bOneHusen = FALSE );
	void Destroy_HusenInp();
	void HjiyuInpDisp();
	void Destroy_HjiyuInp();

	void SubDataInpDisp( int pn );
	void Destroy_SubDataInp();

	void Destroy_ModeLessDlg();
	void DateInputChange();

	void _hjiyu_select( int selno );
	void HendJiyuCheck( CDBINPDataRec *pcrec, int clear = 0 );

	long exDBdata_set( int pn, int id, VARIANT* data, int type, int bcdlen );
	long exDBdata_get( int pn, int id, VARIANT* data, int type, int bcdlen );
	void exDBSetDispString( int pn, int id, LPCTSTR string );
	int	exDBkamokudata_get( int pn, int id, struct _KamokuData* data );
	int	exDBkamokudata_set( int pn, int id, struct _KamokuData* data );

	int _GetDataList_Data( int ln, DTLIST_DATA* pData );
	void _SetDataList_Data( int ln, DTLIST_DATA* pData, BOOL bRedraw = TRUE );
	void _UpdateDataList_Data( int ln, DTLIST_DATA* pData, BOOL bRedraw = TRUE );

	void TerminationDataSet( int pn );
	void TerminationDataSet( int pn[], int pncnt );
	void _TerminationDataSet( DTLIST_DATA* data, struct _DATA_LINE *dtline, int pntbl[], int tblcnt );

	void OnLineattr_dsp();
	void TermDistribute( long nChar, int off_type = 0 );

	void ddata_dsp( int vect );
	void _ddata_dsp( int vect );

	void inpline_update();
#if (INPLINE_DEBUG == 1)
	void inpline_dspTrace(struct _DATA_LINE *dtline, int ln, int line = 0, char* func = NULL, char* fname = NULL);
#else
	void inpline_dsp(struct _DATA_LINE *dtline, int ln);
#endif

	void SlipWndMove( BOOL bKeyScan );
	void ScanMode_WndMove(BOOL bKeyScan, BOOL bOnSize = FALSE);
	int	GetZengyoBrcode( int dc_sw );

	BOOL isNoBRNInpChk();
	BOOL tab_copy( CWnd* pwnd, int idc, char *tks );

#if (FOCUS_DEBUG == 1)
	void set_focusTrace( int pn, int iClick = 0, int line = 0, char* func = NULL, char* fname = NULL );
	void _set_focusTrace( int ln, int pn, int iClick = 0, int line = 0, char* func = NULL, char* fname = NULL );
#else
	void set_focus( int pn, int iClick = 0 );
	void _set_focus( int ln, int pn, int iClick = 0 );
#endif
	void __set_focus( int ln, int pn, int iClick = 0 );
	void set_termfocus( int ln, int pn, int vect_n );
	BOOL check_lastitem( int ln, int pn, long nChar  );

	void set_nowln(int ln);

	int set_nowpn( int pn );
	int get_nowpn( void );
	int _get_nowpn( int ln );

	int is_linechk( int ln );
	int _dline_chg( CWnd* pwnd, int, int, int focs_sw = 0 );
	int __dline_chg( CWnd* pwnd, int, int, int focs_sw = 0 );
	long dline_chg( CWnd* pwnd, long );
	int dscroll_down( int, int scrl_bar = 0 );
	int dscroll_up( int );

	int Wdata_ok();
	int AppendWdata_ok();

	int WandNextData( int endapp = 0 );

	int rewupdat( CDBINPDataRec *dta, CDBINPDataRec *moto, int mode, BOOL brtorok = FALSE, BOOL bmchk = TRUE );
	int rewupdat2( CDBINPDataRec *dta, CDBINPDataRec *moto, int mode, BOOL brtorok = FALSE, BOOL bmchk = TRUE );

	void DB_AppendOperation( CWnd* pwnd, int pn );
	void DATAandDOC_AppendOperation( int pn );
	BOOL BmonKoji_DebCre_Inp( int ln );

	void *getDinpData( int inp_mode );
	void SaveDinpData( int inp_mode );

	void SetDinpData( int inp_mode );
	void SetDinpDataLite( int inp_mode );
	void SaveDinpData_SEQ( int inp_mode, int seq );

	void *getINPtype( int inp_mode );
	void SaveInpType( int inp_mode, int ln );
	void SetInpType( int inp_mode );

	void big_img( struct _DATA_LINE *dtline, struct _InpCtl *ctl );
	void dsp_img( CWnd *imgorg, struct _DATA_LINE *dtline, RECT* newrect = NULL, BOOL real = FALSE, RECT* lmt_width = NULL );

	void _OnTerminationIcsdbedtctrl( UINT ID, long nChar, long inplen, long kst );
	void SendEndLabelEdit( UINT ID, long nChar );

	void SetListTitle();
	void SetListColumn( int line );
	BOOL IsBillData( CString& dbt, CString& cre );

	void UpdateDataLineDisp( int* set_pn );

	int	CheckSelectMonthData();
	int	KiriSwk( CDBINPDataRec* pKrec, CDBINPDataRec* crec );
	int ChkMotoSwk();

	BOOL GetKriMode();
	void SetKriMode( BOOL bMode );
	void GetKriSiwake( CDBINPDataRec* data );
	void KeepKriSiwake( CDBINPDataRec* data );

	BOOL isBmnHaniChk();
	int  CheckKamokuInput( int dc_sw, struct _KamokuData* data );

	BOOL rewupdat_job( int ret );
	void rewupdat_disp();
	void kmkbmn_change( int dc_sw, CDBINPDataRec* data, CString& chgcd );

	void TraceWndRect(int num);
	void BmnKjSelDispRst();
	void rewupdat_kojidisp();
	void sel_shiftjob( int ln, long nChar, BOOL bPress );
	void sel_lineupdate( BOOL bDoUpdate = FALSE );
	void click_seljob( int line, long nChar );

	int	 BRNTKY_select( int bmn, CString& code, char* kana = NULL, BOOL bDisp = FALSE );

	int	 pn_movecheck( int pn );
	void rewupdat_syzdisp();

	BOOL CheckAppendSansyo();

private:
	int NOW_PN, EDIT_PN;
	int SAVE_PN;
	int DDATA_NOW_PN[ DDATA_LNMAX ];
	BOOL	m_bMnthFix, m_bIgnoreDateTerm;

	int	m_BKINP_PN;

//	BOOL make_50tky();
	//#* 12.09 /02	
//	BOOL make_autotky( BOOL bLastDisp = FALSE );

	WORD*	 m_ASTKCX;		// 仕訳摘要コードエリアポインタ
	int	 swktky_alloc();	// 　〃　　メモリ確保
	void swktky_clear();	// 　〃　　メモリゼロクリア
	
	int get_rentk_sw();
	int get_swktky_sw();

	BOOL inp_ok( int prev_pn, int now_pn, int* set_pn );
	int	getAUTOSELpn( int ln, int nowpn );

	int tky_select_to_drec( CDBINPDataRec *CREC, CDBipTKREC *ptk );
	void tky_select_dsp( CDBINPDataRec *CREC, int dc_sw, CDBipTKREC *ptk, BOOL F12_KEY, BOOL bTkcut = FALSE );
	void remove_edabandsp( CDBINPDataRec *crec, int pn );

	void DBeditCtrlSet( UINT ID, int id_pn );

	//前行
	int		m_prevln;
	void	set_prevln(int ln);
	int		get_prevln();

	CDBINPDataRec	m_DCrct;

	BOOL		m_bBmpMake;
	CBitmap		m_ScrnBmp;
	CRect		m_blkRect[10];
	int			m_blkIdx[10];

	//部門範囲(損益のみ)
	int		m_sdbtBmn, m_screBmn;

	//ソート種
	int		m_sortID, m_sortType;

	BOOL	m_bActiveIgnore;

// 定型仕訳関係
#ifdef REGDATA

public:
//	void RGDTA_BUTTON_DISP( BOOL ena = TRUE );

private:
//	BOOL m_bRGDTA_BUTTON_DISP;
	RECT RDRemainRECT;
//	int	 m_CmnId;

	BOOL isRGDTDLG();
	RECT GetRDDlgRECT( );
	LRESULT OnRGDTsel( WPARAM wParam, LPARAM lParam );

	void prevdata( CDBINPDataRec *data, int ln );

	int	 GetCmnOwntb();
	int	 InitRdata();

public:
	BOOL m_bRGDTA_BUTTON_DISP;
	int	 m_CmnId;
private:
	void RdataSaveSrec( CDBINPDataRec *srec );
	void RdataRstrSrec( CDBINPDataRec *srec );
	BOOL RdataToInpData( CDBINPDataRec *crec, CDBINPDataRec *srec );
// 定型残り仕訳数欄関係
	void RDRemainDisp( BOOL sw );
	void RDRemainIniz( void );
	virtual void RDRemainDispJOB( void );

	void Rdadd_job( int line );
	void RdselCancelJob();
	BOOL Rdta_autocheck( int pn );
#endif // REGDATA

// 原票会計関係
#ifdef _SLIP_ACCOUNTS_
private:
	// 初期化
	int InitSldata();
	// 原票：表示中？
	BOOL IsDispSlip();
public:
	// リンクボタン状態変更
/*- '12.10.03 -*/
//	int ChgLinkBtnState( EnumIdSlipdispBtnstate btnSt );
/*-------------*/
	int ChgLinkBtnState( EnumIdSlipdispBtnstate btnSt, BOOL isChgDepressState=TRUE );
/*-------------*/
	// フォーカス再セット
	int ResetFocusFormImg();
	// 摘要再セット
	int ResetTekiyoFromImg();
	// イメージダイアログ表示切替
	int ChangeImgDlgDisp( int pn, int &tooltype );
	// イメージダイアログ表示切替 ('12.10.06)
	int ChangeImgDlgDisp( int pn );
	// タブクリックによる切替時の チェック
	int TabLbuttonDownJob();
	// イメージダイアログ表示
	int DispImgDlg( BOOL bKeyDown );
	// イメージダイアログ非表示
	void HideImgDlg(int prn = 0 );
	// 関連付けるイメージから表示中のイメージを削除する
	int DelDocseq();
	// 表示中のイメージが関連付けられたもの？
	int IsConnectedDoc();
	// 関連付けるイメージを全て削除する
	int DelAllDocseq();
	// リンクボタンの状態を取得する ('12.09.11)
	EnumIdSlipdispBtnstate GetLinkBtnState(){ return m_SlipBtnState; };
	// イメージダイアログの表示タイプを変更する ('12.09.18)
	int ChangeImgdlgType( EnumIdDispImgDlgType dtype );
	// アクティブになった時にフォーカスを再セットして良い状態？ ('12.09.21)
	BOOL IsAbleToResetFocusState();
	// イメージ表示オプション変更 ('12.09.26)
	int ImageOptionChanged();
	// 最新表示ドキュメントシーケンス保存 ('12.09.28)
	void SaveDocseq( int svDocseq );
	// リンクボタン状態変更 ('12.10.01)
	int ChgOnlyLinkBtnState( EnumIdSlipdispBtnstate btnSt );
	// 原票表示モードをオフにする ('12.10.02)
	int TurnOffDispImgDlg();
	// 現在位置に従ってツールバーを変更する ('12.10.02)
	int ChangeToolbarSub( int pn );
	// 現在位置に従ってツールバーを変更する[摘要欄] ('12.10.03)
	int ChangeToolbarSubInTkypn( int pn );
	// 表示中のイメージを消す ('12.10.03)
	int HideDispImgDlg( BOOL bScanViewEx = FALSE );

public:
	BOOL m_bSLACC_BUTTON_DISP;
	BOOL m_bSLIP_MASTER;						// 原票保存マスター？

protected:
	CImgMng					*m_pImgmng;			// イメージ表示用クラス
	int						m_Imgcnt;			// イメージカウンタ
	IMG_DISPOPTION			m_ImgDispOption;	// イメージ表示オプション
	EnumIdSlipdispBtnstate	m_SlipBtnState;		// 原票ボタンの状態
	CToolTipCtrl			m_tooltip;			// ツールチップ
	int						m_ImgDtseq;			// イメージ表示中の仕訳シーケンス
	CArray<int>				m_ContDocseqAry;	// 関連付けるイメージデータシーケンス
	int						m_NowImgSeq;		// 表示中のイメージデータシーケンス('12.08.10)
	EnumIdDispImgDlgType	m_DImgdlgType;		// イメージダイアログ表示タイプ('12.09.07)

#endif	// _SLIP_ACCOUNTS_

#ifdef _SCANNER_SV_
	// 原票との関連付け状態を変更する('16.01.24)
	int ChgLink();
	CArray<int>		m_SRECdseqAry;	//変更前イメージデータシーケンス
public:
	// 原票表示ダイアログでリンクが切り替えられた時の後処理('16.02.03)
	void DoAfterLinkCall( int msg );
	// 原票表示ダイアログでリンクが解除された時の後処理('16.02.03)
	void DoAfterDropLinkCall( int msg, int leftCnt );

	BOOL IsChangeLinkDocSeq();
#endif	// _SCANNER_SV_

	BOOL	m_bDscroll;
	BOOL	m_bDWP_LINECOUNT;
	BOOL	m_bForkUCOM; //ForkUCOM 処理をするか？

public:
//	void fix8disp( BOOL sg );
//	void enable_fixval( BOOL sg );
//	void enable_fix8( BOOL sg );
//	void set_valtitle( char *msg );

	void set_inpmsg( int mode, char *msg );
	void inpn_dsp( int nline );
	void genk_dsp( char *buf );
	void syog_dsp( char *buf );

	void genk_ctrlshow( BOOL bShow );
	void genk_strchg( char* kmcd );

//	int	InputItemCheck(struct _ItemData& ans, struct _ItemData& check, int _dcsw, BOOL bSelClick/*=FALSE*/ );
//	int InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick =FALSE );

	void SelDspRst( int inp_mode );
	void SelDspParaRst();
	void brntky_rst();
//	long SelTkyDsp( long nChar );

	void gotokeyscan(void);
	void gotoscankey(void);
	void endkeyscan(void);

	void directscankey();
	void directacbook();

//	int tky_select( CDBipTKREC *tkrec, char *data );
//	BOOL tky_select50_dsp( char *data, BOOL bDISP =TRUE );
//	int tky_atswk_dsp( CString deb, CString cre, BOOL bDISP = TRUE );

	void Dsp_TkytoBmnSelect();
	void bmon_select_dsp( CWnd* pwnd, int dc_sw, CDBINPDataRec *CREC, BOOL bTerm );
	BOOL BmnCodeChk( CWnd* pwnd, int bmn, int sw, int pn );

	void koji_select_dsp( CWnd* pwnd, int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm );
	BOOL KojiCodeChk( CWnd* pwnd, CString koji, int sw, int pn );
	void bmnkoji_to_tekiyo( CDBINPDataRec* rec, CString* meisyo, int type = 0 );
	BOOL isKojiChk();

	BOOL isNoBmnInpChk();
	BOOL isKojiDataChk();

	void CtrlDispJob();
	BOOL KojinKamokuAutoCheck( int dc_sw );
	
	void kyuyocsv_append( LPCTSTR fpath, BOOL bGenDB = FALSE );
	void RButtonMenuJob( UINT nID, CPoint point );
	BOOL CheckCopySeq( int line );

	void ColumnSizeAdjust( int& bmon, int& koji, int& syonum );
	int  get_insseq();
	int  CRECtoZREC();

	int  keyscan_insline_dispchg( int oldln, int nowln, int* pFocusSgn );
	int  KeyscanInsertJob();

	int  modefy_insline_dispchg();
	void ins_back_modefy();
	void ins_back_keyscan();
	int  get_insrec( int seq, CDBINPDataRec* pdata );
	void set_insrec(CDBINPDataRec* data, int bseq );
	void disp_insswklist( int oldln, int nowln );
	void disp_keyscan_insswklist( int oldln, int nowln );

	void dbedit_clear();

	void SwkListSortSign( int data_id, int type );
	void SwkListSortClear();

	void GetSwkListSortSign( int& data_id, int& type );
	BOOL IsKeyScanMode();

	int  Rddata_Check();
	void set_rddatasyz();
	void dbdata_set( CDBDATA* dbdata, CDBSyohi* pSy, BOOL syinit = TRUE );

	BOOL check_cpydata( CDBINPDataRec* data, DWORD& syCode );
	void CheckCopySwkData( CDBINPDataRec* pcpy );

	int  SetNprtParam();

	int  data_denpdialog(int ln);
	int  denpend_swkdisp( int ln );
	int  denpend_keyscandisp( int ln );
	BOOL m_bAppendUpkey;
	int  SetCRECfromDPend( CDBINPDataRec *pCREC );
	int  GetNowLineSetPN( int ln = -1 );
	BOOL IsDenpyoDataLine( int ln );

	BOOL ChangeZeiritsu();
	BOOL ChangeSotoMenZei();
	void disp_keyscan_insswkcopy();

	void GnpSiwakeDlgJob();
	void GnpRecogDataSet();

	void datetekiyo_check_tkrec(CDBINPDataRec *CREC);

// 操作
public:

// オーバーライド
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後にはじめて呼び出されます。

// 実装
public:
	virtual ~CDBDinpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void	OnButtonEnd();
	afx_msg void OnButtonF12();
	afx_msg void OnButtonF8();
	afx_msg void OnUpdateButtonF8(CCmdUI* pCmdUI);
	afx_msg void OnButtonF11();
	afx_msg void OnSetFocusIcsdbedtctrl( UINT ID );
	afx_msg void OnKillFocusIcsdbedtctrl( UINT ID, long inplen );
	afx_msg void OnTerminationIcsdbedtctrl( UINT ID, long nChar, long inplen, long kst);
	afx_msg void OnImeEndCompositionIcsdbedtctrl( UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	afx_msg void OnKanaTerminationIcsdbedtctrl( UINT ID, LPCTSTR Kana);
			void RButtonUpIcsdbedtctrl( UINT nID, long nFlags, long pointx, long pointy);
	afx_msg void OnSelectedIcsselctrl1(short position, long Number);
	afx_msg void OnSelectedIcsselctrl2(short position, long Number);
	afx_msg void OnButtonF3();
	afx_msg void OnButtonF4();
	afx_msg void OnUpdateButtonF4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonF12(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonF3(CCmdUI* pCmdUI);
	afx_msg void OnButtonF9();
	afx_msg void OnUpdateButtonF9(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonF11(CCmdUI* pCmdUI);
	afx_msg void OnButtonF7();
	afx_msg void OnUpdateButtonF7(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

	void	OnBeginLabelEdit(UINT ID, NMHDR* pNMHDR, LRESULT* pResult );
	void	OnEndLabelEdit(UINT ID, NMHDR* pNMHDR, LRESULT* pResult);
	void	OnSetDispInfo( UINT ID, NMHDR* pNMHDR, LRESULT* pResult);
	void	OnLineChanged( NMHDR* pNMHDR, LRESULT* pResult);

	BOOL	IsYunyuMove();
	LRESULT SetSkbnLineDisp( WPARAM wParam, LPARAM lParam );
	LRESULT OnSkbnEnd( WPARAM wParam, LPARAM lParam );
	LRESULT DspSkbnIchiran( WPARAM wParam, LPARAM lParam );

	LRESULT OnBkinpEnd( WPARAM wParam, LPARAM lParam );
	LRESULT OnHusenInpMsg( WPARAM wParam, LPARAM lParam );
	LRESULT OnHjiyuInEnd( WPARAM wParam, LPARAM lParam );
	LRESULT OnHjiyuSel( WPARAM wParam, LPARAM lParam );
	LRESULT OnSubDataInEnd( WPARAM wParam, LPARAM lParam );

	LRESULT	RetroFocus2( WPARAM wParam, LPARAM lParam );
	LRESULT	RetroFocus( WPARAM wParam, LPARAM lParam );
	LRESULT OnSendSeq( WPARAM wParam, LPARAM lParam );
	// 09.11 /09
	LRESULT OnCorrectMsg( WPARAM wParam, LPARAM lParam );
	LRESULT OnDBDinpMsg( WPARAM wParam, LPARAM lParam );

	LRESULT OnSelGuideMsg( WPARAM wParam, LPARAM lParam );

#ifdef _ICSPEN_	// 01.20 /01
private:
	BOOL			fPen;
	short			KindInput;
	short			SiwakeType;

	BOOL		flgPen;
//	ICSPen		icspen;
//	ICSPen*		p_icspen;

	void _FocusIcsSeq(CWnd* pwnd, int ln, short click );
	void _FocusIcsDate(CWnd* pwnd, int ln, short click );
	void _FocusIcsDenp(CWnd* pwnd, int ln, short click );
	void _FocusIcsBmon(CWnd* pwnd, int ln, int dc_sw, short click );
	void _FocusIcsKoji(CWnd* pwnd, int ln, int dc_sw, short click );
	void _FocusIcsDebt(CWnd* pwnd, int ln, short click );
	void _FocusIcsCred(CWnd* pwnd, int ln, short click );
	void _FocusIcsVal(CWnd* pwnd, int ln, short click );
	void _FocusIcsZei(CWnd* pwnd, int ln, short click );
	void _FocusIcsSyz(CWnd* pwnd, int ln, short click, int focs_sw );
	void _FocusIcsTky(CWnd* pwnd, int ln, short click );
	void _FocusIcsSyono( CWnd* pwnd, int ln, short click );
	void _FocusIcsTgDate( CWnd* pwnd, int ln, short click );
	void _FocusIcsHjiyu( CWnd* pwnd, int ln, short click );
	void _FocusIcsDocEviNo( CWnd* pwnd, int ln, short click );

protected:
	BOOL&	IsPen() { return fPen; }
	short&	IsKind() { return KindInput; }
	short&	IsType() { return SiwakeType; }

	void SelVrtResize( long topercent );
	void MakePenDisplay( void );

	short SkbnSetPen( BOOL flg );
	void SkbnSetPenRect( CRect& rect );
	short ScanKeySetPen( BOOL flg );
	void OnICSAnkFocus();

	void OnICSInputFocus( int pn );

public:
	CRect GetPenRect();
	CRect GetICSAnkRect();
	short SetPen( BOOL flg );
//	ICSPen* GetPenObject() { return	&icspen; }
//	ICSPen* GetPenObject() { return	p_icspen; }

#endif

private:
	BOOL	m_checkfirst;
	BOOL&	FirstTime() { return	m_checkfirst; }

public:
	int		BeginApp(int recosel = 0);
	BOOL	EndApp( WPARAM wParam = 0, BOOL bCosel = FALSE  );
	BOOL	EndAppChkRDdata( WPARAM wParam );
	BOOL	EndChk();

	void	CloseWork();

	void	TabClickChangeWork();
	int		TabClickChangedPrevView();

	int		GetWorkDirFunc(LPSTR wkPath, int szWkPath);
	void	WizRenkeiEndJob();

protected:
	virtual void OnDraw(CDC* /*pDC*/);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo );
	virtual void OnPrint(CDC* pDC, CPrintInfo*);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	LRESULT OnDateSelectMessageChangeMNTHSEL(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonF2();
	afx_msg void OnUpdateButtonF2(CCmdUI *pCmdUI);
	afx_msg void OnButtonInsert();
	afx_msg void OnUpdateButtonInsert(CCmdUI *pCmdUI);

public:
	afx_msg void OnButtonF6();
public:
	afx_msg void OnUpdateButtonF6(CCmdUI *pCmdUI);
	afx_msg void OnButtonF5();
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
	afx_msg void OnMenuKyucsv();
	afx_msg void OnUpdateMenuKyucsv(CCmdUI *pCmdUI);
	afx_msg void OnMenuGencsv();
	afx_msg void OnUpdateMenuGencsv(CCmdUI *pCmdUI);
	afx_msg void OnMenuWiz();
	afx_msg void OnUpdateMenuWiz(CCmdUI *pCmdUI);

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnSwkCopy();
	afx_msg void OnSwkPaste();
	afx_msg void OnUpdateSwkCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSwkPaste(CCmdUI *pCmdUI);
#ifdef _SLIP_ACCOUNTS_
	ICSButton m_DispSlipBtn;
	CIconOwnerButton m_LinkSlipBtn;
	CIconOwnerButton m_DispSlipChk;
	CIconOwnerButton m_GnpScanBtn;
	afx_msg void OnBnClickedDispslipChk();
	afx_msg void OnBnClickedLinkslipBtn();
	afx_msg void OnBnClickedDispslipBtn();
	afx_msg void OnMenuSlip();
	afx_msg void OnUpdateMenuSlip(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedGnpScanBtn();

	afx_msg void OnTimer(UINT nIDEvent);

#endif
	virtual HWND GetKamokuCtrlHWND();
	virtual void SelectWindowShow(BOOL bShow);

};

#ifndef _DEBUG  // DBDinpView.cpp のデバッグ バージョン
inline CDBDinpDoc* CDBDinpView::GetDocument() const
   { return reinterpret_cast<CDBDinpDoc*>(m_pDocument); }
#endif

