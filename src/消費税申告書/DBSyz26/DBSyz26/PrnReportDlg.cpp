// PrnReportDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PrnReportDlg.h"


// CPrnReportDlg ダイアログ

IMPLEMENT_DYNAMIC(CPrnReportDlg, ICSDialog)

CPrnReportDlg::CPrnReportDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnReportDlg::IDD, pParent)
{

}

CPrnReportDlg::~CPrnReportDlg()
{
}

void CPrnReportDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_PRINT, m_PrnMainTab);
}


BEGIN_MESSAGE_MAP(CPrnReportDlg, ICSDialog)
END_MESSAGE_MAP()


// CPrnReportDlg メッセージ ハンドラ

BOOL CPrnReportDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
