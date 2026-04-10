// ZeroPercent.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#include "ZeroPercent.h"


// CZeroPercent ダイアログ

IMPLEMENT_DYNAMIC(CZeroPercent, ICSDialog)

CZeroPercent::CZeroPercent(CWnd* pParent /*=NULL*/)
	: ICSDialog(CZeroPercent::IDD, pParent)
{

}

CZeroPercent::~CZeroPercent()
{
}

void CZeroPercent::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CZeroPercent, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CZeroPercent::OnBnClickedButton1)
END_MESSAGE_MAP()


// CZeroPercent メッセージ ハンドラ

BOOL CZeroPercent::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	c_set( IDOK );

	ICSDialog::OnInitDialogEX();
	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります

}

void CZeroPercent::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	ICSDialog::OnOK();
}

void CZeroPercent::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	ICSDialog::OnCancel();
}

void CZeroPercent::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ICSDialog::EndDialog( 0 );
}

BOOL CZeroPercent::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	return ICSDialog::PreTranslateMessage(pMsg);
}
