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
	ICSMon*		m_mon;
#endif

private:
	CoSel		m_cosel;		// 会社確認(財務)


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


