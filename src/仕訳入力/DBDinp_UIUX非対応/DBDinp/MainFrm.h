// MainFrm.h : CMainFrame クラスのインターフェイス
//
#define NON_VIEW	-1
#define INP_VIEW	0	//Main View
#define SKEY_VIEW	1	//検索項目 View
#define SCAN_VIEW	2	//検索 View
#define DNP_VIEW	3
#define ACBK_VIEW	4	//出納帳 View


#pragma once
class CMainFrame : public ICSMainFrame
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:
	CView *GetCView( int viewno );
	//現在のビュー問い合わせ
	int		NowView(){ return m_nowvw_flg;	}
	BOOL	GetKyuyoDBsign() { return m_bKyuyoDB; };
	BOOL	GetGensyoDBsign();

	HBRUSH	GetViewColorBrush() { return (HBRUSH)m_brBack.GetSafeHandle(); };
	BOOL	IsReCOSEL() { return bReCOSEL; };
	BOOL	IsForkChain() { return m_bForkChain; };
	void	set_DocShtypCombo();
	BOOL	ScanDenpCheck();
	//Wiz連携マスターサイン
	BOOL	GetWizMasterSign() { return m_bWIZMaster; };
	void	SetWizMasterOFF();

protected:
#ifdef _DEBUG
	ICSMon* mon;
#endif

	CoSel cosel;				//会社確認構築

	CView		*m_inpview, *m_skeyview, *m_scanview, *m_acbkview, *m_dnpview;
	int			m_INPview_id;	//Main View == 0
	int			m_SKEYview_id;
	int			m_SCANview_id;
	int			m_ACBKview_id;
	int			m_DNPview_id;
	int			m_nowvw_flg, m_prevw_flg;
	// スキャン項目画面に行く前に開いていた画面ID
	int			m_SKEYprev_flg;

	int		m_NowBarType;
	BOOL	bTabClickChange;

	BOOL	bSysClose;
	BOOL	bTKY_ONLY;
	BOOL	bSCANMDFY;
	int		m_ScmdfySeq;
	BOOL	bReCOSEL;
	BOOL	m_bChgView;
	BOOL	m_bBeginApp;

	int		m_SkeyKmkType;	//検索項目 科目タイプ[0=単一, 1=範囲]
	int		m_AutoSwkType;	//自動仕訳タイプ
	int		m_AcbkOption;	//出納帳ｵﾌﾟｼｮﾝ
							/*======================================
								D0:	部門
								D1:	工事番号
								D2:	枝番
								D8 OFF: 日付順, ON:SEQ順
							======================================*/
	int		m_AcbkKikanSwk; //出納帳 1:月指定の指定期間で仕訳表示
	int		m_DupliSwk;		//重複仕訳のチェックボックス

	int		m_AcbkKojiWd;	//出納帳工事番号幅
	BOOL	m_bKyuyoDB;
	BOOL	m_bGensyoDB;
	BOOL	m_bWIZMaster;	//販売管理連携マスター

	BOOL	m_bMsgBox;		//メッセージボックス表示サイン
	BOOL	m_bMsgBoxOnSize;//メッセージボックス表示中サイン(OnSize制御用)
	BOOL	m_bIgnoreOnSize;//OnSize処理無視フラグ

	BOOL	m_bBrushFlg;
	CBrush	m_brBack;

	int		m_IsShincyoku;

	int		m_FuriMode;
	BOOL	m_bReload;
	BOOL	m_bForkChain;

	CString	m_ObjsPath;
	int		m_ActivateFlag;
	BOOL	m_bOnCloud;
	BOOL	m_bKeiriJzEB;

	UINT	m_MouseRouting;
	ICSProgressWnd*	m_prog;

// 操作
public:
	void OnInitialize();
	void OnInitialize( LPCTSTR seq_path );

	int StartBlock();
	void JobEndOperation(BOOL bCosel);

	void ChgToScanView( int sw, int ofs, int scan_sgn );
	void ChgToInpView( int sw, int seq = 0, int now_seq = 0 );
	void ChgToSkeyView();
	void ChgToACBKView();
	void ChgToDenpView(	BOOL bScan =TRUE );
	void ChgToScanViewEx();

	void SkeyOnButtonEnd();
	void ScanKeyClear();

	void DirectScanView();
	void DirectInpView();	
	void DirectSkeyView();
	void DirectACBKView();
	void DirectDenpView();

	void ChangeToolBar(int chg_sg, int sub = 0 );	// ﾂｰﾙﾊﾞｰ切替
	BOOL GetToolBarEnabled( UINT nID );
	void ChangeToolBarText( int index, char* text );
	
	void ReCoselSetFocus( BOOL bNewCo );
	BOOL ReCoselEndCheck();
	BOOL ReCoselCheckOtherPro();

	void totaldisp();
	void SetViewID( int* id );
	void CallOtherPro( int prosgn );
	void SetNormalCheckUCOM();

	BOOL&	IsTkyOnly() { return bTKY_ONLY; }
	BOOL&	IsSCANMDFY() { return bSCANMDFY; }

//****debug リリースのデバッグ用にステータスバーに文字列表示  ――――――――――
	void err_statusdsp( LPCTSTR msg ) {
//		m_wndStatusBar.SetWindowText( msg );
	}
//****debug ―――――――――――――――――――――――――――――――――――
	void GetFrameRect( CRect& rect );
	BOOL& TabClickChange(void) { return bTabClickChange; }
	void AutoInpMonthChange( int sofs, int eofs, int sy, int ey );

	void	RedrawStatusBar( CString& str );
	void	draw_kengyobar();

	//ビュー切り替え中フラグサイン
	// ビュー切り替え中で、前のビューのフォーカス処理等
	// 処理したくないもの用
	//
	BOOL& ChgViewFlag() {	return m_bChgView; }

	int& SkeyKmkType()  {	return m_SkeyKmkType; }
	int& AutoSwkType()  {	return m_AutoSwkType; }

	int& AcbkOption()  {	return m_AcbkOption; }
	int& AcbkKojiWd()  {	return m_AcbkKojiWd; }

	int& FuriMode()  {	return m_FuriMode; }

	int& AcbkKikanSwk() { return m_AcbkKikanSwk; }
	int& DupliSwk() { return m_DupliSwk; }

	void StatusGuideDisp( int strid );
	void div_toolstr( char* str, CString& descript );

	BOOL& BeginAppFlag() {	return m_bBeginApp; }

	BOOL& MsgBoxFlag() { return m_bMsgBox; };

	BOOL& MsgBoxOnSizeFlag() { return m_bMsgBoxOnSize; };

	BOOL& IgnoreOnSizeFlag() { return m_bIgnoreOnSize; };

	void  SwkListSortSign( int id, int type );
	void  GetSwkListSortSign( int& id, int& type );

#ifdef _SLIP_ACCOUNTS_
	// 設定変更を各ビューに伝える ('12.09.26)
	int SendChangeSlipOption();
#endif

	void ShicyokuKiroku( int kind );
	void GetShincyokuFlag();

	void end_toolsub( int tkyo, int type = 0 );
	BOOL DenpDataExistCheck();

	int  IsCloudEndMsg();
	void ChainProcess();
	BOOL CloudEndChainJob();
	BOOL bCloudEnd;

	void ToolBarClear();

	LPCTSTR GetObjsPath() { return m_ObjsPath;  }
	void CurrentDirCheck();
	int	GetActivateFlag() { return m_ActivateFlag; }

	BOOL IsOnCloudEnv() { return m_bOnCloud; }
	//経理上手くん（電子帳簿版）.
	BOOL& IsKeiriJzEBEnv() { return m_bKeiriJzEB; }

	void GetMouseWheelRouting();
	int  GetMouseRoutingInfo() { return m_MouseRouting; }

	void fnc_fromReadOCR();

	BOOL m_bGenpyoScan;

protected:
	BOOL OnCloseSub(WPARAM w, BOOL bCosel = FALSE);

	void CheckListUcom( CString& pro );
	void ResetCheckUCOM();

	int DispInfoWork( int& type, BOOL bRead );
	void fnc_scanmdfy();

	void ToolBarClearFunc();

	int  WizDataDownLoadJob();
	void ChgToScanViewWizData();
	int  WizRenkeiMasterCheck();

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

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	LRESULT		OnCall( WPARAM wParam, LPARAM lParam );
	LRESULT		OnFromForkChain( WPARAM wParam, LPARAM lParam );
	LRESULT		OnFromParentCosel( WPARAM wParam, LPARAM lParam );
	LRESULT		OnChgToolBar( WPARAM wParam, LPARAM lParam );
	LRESULT		OnBunki( WPARAM wParam, LPARAM lParam );
#ifdef _SLIP_ACCOUNTS_
	LRESULT		OnMessageMultipos( WPARAM wParam, LPARAM lParam );
#endif

public:	
	void ChildFrameActive( CWnd* pChild );
	virtual int BeginApp();
	virtual int	TabRButtonDown();
	virtual int	TabLButtonDown();
public:
	LRESULT	OnMessageClose( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnIndkmk();
	afx_msg void OnUpdateIndkmk(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateJgykmk(CCmdUI *pCmdUI);
	afx_msg void OnJgykmk();

	afx_msg void OnButtonShicyoku();
	afx_msg void OnUpdateShicyoku(CCmdUI *pCmdUI);
	afx_msg void OnHelpDisp();

	LRESULT	OnWmAddInvMsg(WPARAM wParam, LPARAM lParam);
};


