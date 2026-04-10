// ICS Application1View.h : CSimulateKuriwariDriverView クラスのインターフェイス
//


#pragma once
#include "afxwin.h"


class CSimulateKuriwariDriverView : public ICSFormView
{
protected: // シリアル化からのみ作成します。
	CSimulateKuriwariDriverView();
	DECLARE_DYNCREATE(CSimulateKuriwariDriverView)

public:
	enum{ IDD = IDD_SimulateKuriwariDriver_FORM };

// 属性
public:
	CSimulateKuriwariDriverDoc* GetDocument() const;

// 操作
public:

// オーバーライド
	public:
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
	virtual ~CSimulateKuriwariDriverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	LRESULT OnMessageFromPrint( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};

#ifndef _DEBUG  // ICS Application1View.cpp のデバッグ バージョン
inline CSimulateKuriwariDriverDoc* CSimulateKuriwariDriverView::GetDocument() const
   { return reinterpret_cast<CSimulateKuriwariDriverDoc*>(m_pDocument); }
#endif

