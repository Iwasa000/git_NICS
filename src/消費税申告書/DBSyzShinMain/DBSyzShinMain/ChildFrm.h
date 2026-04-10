// ChildFrm.h : CChildFrame クラスのインターフェイス
//


#pragma once


class CChildFrame : public ICSChildFrame
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 属性
public:

// 操作
public:

// オーバーライド
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CChildFrame();
	virtual int	TabRButtonDown();
	virtual int	TabLButtonDown();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChildActivate();
};
