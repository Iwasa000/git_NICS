// MainFrm.h : CMainFrame クラスのインターフェイス
//


#pragma once
class CMainFrame : public ICSMainFrame
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:
#ifdef _DEBUG
	ICSMon* pMon;
#endif
	CoSel m_cosel;
	CDBZmSub* m_Zmsub;
	CString strErr;

// 操作
public:

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
	int Start();
	BOOL ZmInit();
	BOOL ConnectDB();
	int CheckSetMasterDB(CDBZmSub* pZmsub, CString sqlsvr, int apno, int cocode, int kikan, CString & strErr);

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	LRESULT		OnCall( WPARAM wParam, LPARAM lParam );
	LRESULT		OnFromForkChain( WPARAM wParam, LPARAM lParam );
	LRESULT		OnFromParentCosel( WPARAM wParam, LPARAM lParam );
	LRESULT		OnMessageClose( WPARAM w, LPARAM l );

public:	
	void ChildFrameActive( CWnd* pChild );
	virtual int BeginApp();
	virtual int	TabRButtonDown();
	virtual int	TabLButtonDown();
};


