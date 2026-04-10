// UCHIWAKEView.cpp : CUCHIWAKEView クラスの実装
//

#include "stdafx.h"
#include "UCHIWAKE.h"

#include "MainFrm.h"
#include "UCHIWAKEDoc.h"
#include "UCHIWAKEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUCHIWAKEView

IMPLEMENT_DYNCREATE(CUCHIWAKEView, ICSFormView)

BEGIN_MESSAGE_MAP(CUCHIWAKEView, ICSFormView)
END_MESSAGE_MAP()


// CUCHIWAKEView コンストラクション/デストラクション

CUCHIWAKEView::CUCHIWAKEView()
	: ICSFormView(CUCHIWAKEView::IDD)
{
	// TODO: 構築コードをここに追加します。
}

CUCHIWAKEView::~CUCHIWAKEView()
{
}

void CUCHIWAKEView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
}

BOOL CUCHIWAKEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return ICSFormView::PreCreateWindow(cs);
}

void CUCHIWAKEView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();
}


// CUCHIWAKEView 診断

#ifdef _DEBUG
void CUCHIWAKEView::AssertValid() const
{
	ICSFormView::AssertValid();
}

void CUCHIWAKEView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}

CUCHIWAKEDoc* CUCHIWAKEView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUCHIWAKEDoc)));
	return (CUCHIWAKEDoc*)m_pDocument;
}
#endif //_DEBUG


// CUCHIWAKEView メッセージ ハンドラ

