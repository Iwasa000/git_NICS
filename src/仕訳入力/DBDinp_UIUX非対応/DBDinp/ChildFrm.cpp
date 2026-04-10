// ChildFrm.cpp : CChildFrame クラスの実装
//
#include "stdafx.h"
#include "DBDinp.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, ICSChildFrame)

BEGIN_MESSAGE_MAP(CChildFrame, ICSChildFrame)
	ON_WM_CHILDACTIVATE()
END_MESSAGE_MAP()

// CChildFrame コンストラクション/デストラクション

CChildFrame::CChildFrame()
{
	// TODO: メンバ初期化コードをここに追加してください。
}

CChildFrame::~CChildFrame()
{
}

int CChildFrame::TabLButtonDown()
{
	ICSChildFrame::TabLButtonDown();
	return	0;
}

int CChildFrame::TabRButtonDown()
{
	ICSChildFrame::TabRButtonDown();
	return	0;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。
	if( !ICSChildFrame::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame 診断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	ICSChildFrame::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	ICSChildFrame::Dump(dc);
}

#endif //_DEBUG


// CChildFrame メッセージ ハンドラ

void CChildFrame::OnChildActivate()
{
	ICSChildFrame::OnChildActivate();
}
