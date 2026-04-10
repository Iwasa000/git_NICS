// SyzMakeTestView.h : CSyzMakeTestView クラスのインターフェイス
//


#pragma once
#include "afxwin.h"


class CSyzMakeTestView : public ICSFormView
{
protected: // シリアル化からのみ作成します。
	CSyzMakeTestView();
	DECLARE_DYNCREATE(CSyzMakeTestView)

public:
	enum{ IDD = IDD_SYZMAKETEST_FORM };

// 属性
public:
	CSyzMakeTestDoc* GetDocument() const;

// 操作
public:

	// 初期化
	int InitApl();
	// 画面初期化
	void InitDisp();
	// コントロールから会社情報の取得
	int GetInfoFromControl( SYZMAKE_MKCOINFO &mkcoinfo );

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
	virtual ~CSyzMakeTestView();
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
	CComboBox m_ApnoCmb;
	CEdit m_CodeEdit;
	CEdit m_KisyuEdit;
	CEdit m_KimatsuEdit;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};

#ifndef _DEBUG  // SyzMakeTestView.cpp のデバッグ バージョン
inline CSyzMakeTestDoc* CSyzMakeTestView::GetDocument() const
   { return reinterpret_cast<CSyzMakeTestDoc*>(m_pDocument); }
#endif

