// UCHIWAKEView.h : CUCHIWAKEView クラスのインターフェイス
//


#pragma once
#include "afxwin.h"


class CUCHIWAKEView : public ICSFormView
{
protected: // シリアル化からのみ作成します。
	CUCHIWAKEView();
	DECLARE_DYNCREATE(CUCHIWAKEView)

public:
	enum{ IDD = IDD_UCHIWAKE_FORM };

// 属性
public:
	CUCHIWAKEDoc* GetDocument() const;

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
	virtual ~CUCHIWAKEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // UCHIWAKEView.cpp のデバッグ バージョン
inline CUCHIWAKEDoc* CUCHIWAKEView::GetDocument() const
   { return reinterpret_cast<CUCHIWAKEDoc*>(m_pDocument); }
#endif

