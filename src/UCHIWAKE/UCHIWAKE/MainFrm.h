// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
class CMainFrame : public ICSMainFrame
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// PCOM_GetString()使用するための処理
protected:
#ifdef	_DEBUG
	ICSMon*	m_pMon;
#endif

// 属性
public:
// 操作
private:
	//void		OnCallInsert();
	//void		OnCallDelete();

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
	LRESULT		OnMessageClose( WPARAM w, LPARAM l );

	void		OnUpdateButtonShincyoku(CCmdUI* pCmdUI);

public:	
// 修正No.158301 add -->
	// 確定のカーソル位置保存
	int 		Kt_rowsv;
	int 		Kt_colmsv;
// 修正No.158301 add <--

	void ChildFrameActive( CWnd* pChild );
	virtual int BeginApp();
	virtual int	TabRButtonDown();
	virtual int	TabLButtonDown();

// 2009.09.08 TS自動ログオフ対応
public:
	afx_msg BOOL OnQueryEndSession();
//protected:
	BOOL	CloseView(UINT message, WPARAM wParam);
	//int		ProgTuuti_Dialog(CDBZmSub* pZmSub);
	void ProgTuuti_Dialog_Toolbar(/*CDBZmSub* pZmSub*/);
	int ProgTuuti_Dialog_End(CDBZmSub* pZmSub);
// midori 160612 add -->
	//== ツールバー ==
	// 
// midori 152766 del -->
//	BOOL UpdateLockIndicator( int flg );
// midori 152766 del <--
// midori 152766 add -->
	BOOL UpdateLockIndicator( BOOL flg, BOOL flg2 );
// midori 152766 add <--
// midori 160612 add <--

// 2009.10.08 Windows7対応
public:
	CView*	m_pActivePreview;	// 表示中の印刷プレビューのクラスポインタ
// midori 152710 del -->
//// midori 151403 add -->
//	// 業務内の会社選択ボタンを押下して会社を切り替えた場合
//	// 次の会社が確定するまでの間、TRUEになる
//	BOOL	m_Init;
//// midori 151403 add <--
// midori 152710 del <--

	BOOL	m_fShowdlgPrintAllFlg;		// 2010/07/XX 一括印刷対応　一括印刷ダイアログ表示済フラグ

	BOOL	EndProcBprt(void);			// 2010/08/02 一括印刷時、終了処理（UCOM への情報返却）

// midori 160612 add -->
	LRESULT OnUserUpdateIndicate(WPARAM wParam, LPARAM lParam);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
// midori 160612 add <--

// midori 152718 add -->
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnKakuteiKaijyo();
// midori 152718 add <--
};
