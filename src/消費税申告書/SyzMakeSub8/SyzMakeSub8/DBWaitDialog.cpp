// DBWaitDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DBWaitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBWaitDialog ダイアログ


CDBWaitDialog::CDBWaitDialog(CWnd* pParent /*=NULL*/)
	: ICSDialog(CDBWaitDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBWaitDialog)
	//}}AFX_DATA_INIT

	m_pParent = NULL;
}


void CDBWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBWaitDialog)
	DDX_Control(pDX, IDC_ANIMATE_DBWAIT, m_ani);
	DDX_Control(pDX, IDC_STATIC_DBWAIT, m_static1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBWaitDialog, ICSDialog)
	//{{AFX_MSG_MAP(CDBWaitDialog)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBWaitDialog メッセージ ハンドラ

BOOL CDBWaitDialog::OnInitDialog() 
{
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

//	m_ani.Open( IDR_AVI_DBWAIT );
//	m_ani.Play( 0, -1, -1 );

	m_ani.ShowWindow( SW_HIDE );

	ICSDialog::OnInitDialogEX();

	return FALSE;
//	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

int CDBWaitDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ICSDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ここに特定な作成コードを追加してください。

	CenterWindow();
	ShowWindow( SW_SHOW );

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------
// 表示文字列のセット
//		LPCTSTR		pStr	表示文字列
//-------------------------------------------------------
void CDBWaitDialog::SetWaitText(LPCTSTR pStr)
{
	m_static1.SetWindowText( pStr );
	UpdateWindow();
}

