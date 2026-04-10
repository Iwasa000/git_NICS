// DlgToolBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgToolBar.h"
#include "afxpriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar

CDlgToolBar::CDlgToolBar()
{
}

CDlgToolBar::~CDlgToolBar()
{
}


BEGIN_MESSAGE_MAP(CDlgToolBar, CToolBar)
	//{{AFX_MSG_MAP(CDlgToolBar)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar メッセージ ハンドラ
LRESULT
CDlgToolBar::OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam)
{
    if(IsWindowVisible()){
        CFrameWnd*  pParent = (CFrameWnd*)GetParent();
        if(pParent){
//TRACE( "CDlgToolBar::OnIdleUpdateCmdUI wParam = %08x\n", wParam );
            OnUpdateCmdUI(pParent,(BOOL)wParam);
        }
    }
    return 0L;
}
