//  ProgDlg.cpp : インプリメンテーション ファイル
// CG: このファイルは「プログレス ダイアログ」コンポーネントにより追加されています。

#include "stdafx.h"
#include "resource.h"
#include "MsgDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg ダイアログ

CMsgDlg::CMsgDlg(UINT nCaptionID)
{
	m_nCaptionID = IDS_MSG_STRING;
	if (nCaptionID != 0)
		m_nCaptionID = nCaptionID;

    m_bCancel=FALSE;

    //{{AFX_DATA_INIT(CMsgDlg)
    // NOTE: ClassWizard は、この位置にメンバの初期化コードを追加します。
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;

	m_Xrate = -1;
	m_Yrate = -1;
	m_bHideCancel = FALSE;
}

CMsgDlg::~CMsgDlg()
{
    if(m_hWnd!=NULL)
      DestroyWindow();
}

BOOL CMsgDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CMsgDlg::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CMsgDlg::Create(CWnd *pParent)
{
    // ダイアログの実際の親ウィンドウを取得します。
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled は、このダイアログが破棄された時に、親ウィンドウを
	// 再び有効にするために使用します。従って、この時点で親ウィンドウがすでに
    // 有効な場合のみ、この変数に TRUE を設定します。

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);	// 親窓 無効
      m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CMsgDlg::IDD,pParent))
    {
      ReEnableParent();
      return FALSE;
    }

    return TRUE;
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgDlg)
    //}}AFX_DATA_MAP
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}

BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
    //{{AFX_MSG_MAP(CMsgDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CMsgDlg::OnCancel()
{

    m_bCancel=TRUE;

}


void CMsgDlg::PumpMessages()
{
    // ダイアログを使用する前に Create() を呼んでください。
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // ダイアログ メッセージの処理
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}


BOOL CMsgDlg::CheckCancelButton()
{
    // ペンディング中のメッセージの処理
    PumpMessages();

    // m_bCancel を FALSE に戻すことにより
    // ユーザがキャンセル ボタンを押すまで、CheckCancelButton は
    // FALSE を返します。このため、CheckCancelButton を呼び出した
    // 後にオペレーションを継続することが可能になります。
    // もし m_bCancel が TRUE に設定されたままだと、次回以降の
    // CheckCancelButton の呼び出しは常に TRUE を返してしまいます。

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}



    
/////////////////////////////////////////////////////////////////////////////
// CMsgDlg クラスのメッセージ ハンドラ

BOOL CMsgDlg::OnInitDialog() 
{
//	ControlInit(165,50,-1);
    
	CDialog::OnInitDialog();

	CString strCaption, strTitle;
	VERIFY(strTitle.LoadString(AFX_IDS_APP_TITLE));
    SetWindowText(strTitle);

	VERIFY(strCaption.LoadString(m_nCaptionID));
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(strCaption);

	if( m_bHideCancel ) {
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}

	// ﾀﾞｲｱﾛｸﾞｻｲｽﾞ変更
	if( m_Xrate != -1 && m_Yrate != -1 ) {
		RECT	newrect;

		GetWindowRect( &newrect );
		// 高さ
		newrect.bottom = newrect.top + ((newrect.bottom - newrect.top ) * m_Yrate);
		// 幅
		newrect.right = newrect.left + ((newrect.right - newrect.left ) * m_Xrate);

		SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER );
	}

    return TRUE;  
}


int CMsgDlg::CaptionChange( int captionID )
{
	CString strCaption;
	int old_capID;
	old_capID = m_nCaptionID;
	m_nCaptionID = captionID;

	VERIFY(strCaption.LoadString(m_nCaptionID));
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(strCaption);

	return old_capID;
}


void CMsgDlg::HideCancelButton()
{
	m_bHideCancel = TRUE;
}

