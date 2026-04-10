// dlgImportMessage.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgImportMessage.h"


// CdlgImportMessage ダイアログ

IMPLEMENT_DYNAMIC(CdlgImportMessage, ICSDialog)

CdlgImportMessage::CdlgImportMessage(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgImportMessage::IDD, pParent)
{

}

CdlgImportMessage::~CdlgImportMessage()
{
}

void CdlgImportMessage::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_MESSAGE, m_lblMessage);
}


BEGIN_MESSAGE_MAP(CdlgImportMessage, ICSDialog)
	ON_WM_SIZE()	// 2010/08/30 業務統合メニュー マルチウィンドウ対応
					// 本メッセージハンドルが無いため、デスクトップ中央に表示される現象対応
END_MESSAGE_MAP()


// CdlgImportMessage メッセージ ハンドラ

BOOL CdlgImportMessage::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	m_lblMessage.UpdateCaption( "データを再構成しています．．．", 0 );
	m_lblMessage.UpdateWindow();

	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 2010/08/30 業務統合メニュー マルチウィンドウ対応
// 本メッセージハンドルが無いため、デスクトップ中央に表示される現象対応
void CdlgImportMessage::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}
