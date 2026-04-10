// WaitDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WaitDialog.h"
#if _MSC_VER >= 1900
#include "afxdialogex.h"
#endif


// CWaitDialog ダイアログ

IMPLEMENT_DYNAMIC(CWaitDialog, ICSDialog)

CWaitDialog::CWaitDialog(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_WAIT_DIALOG, pParent)
{

}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WAIT, m_StaticWait);
}


BEGIN_MESSAGE_MAP(CWaitDialog, ICSDialog)
END_MESSAGE_MAP()


// CWaitDialog メッセージ ハンドラー


BOOL CWaitDialog::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	// TODO: ここに初期化を追加してください
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
