// Gyousyu.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"
#include "Gyousyu.h"


// CGyousyu ダイアログ

IMPLEMENT_DYNAMIC(CGyousyu, ICSDialog)

CGyousyu::CGyousyu(CWnd* pParent /*=NULL*/)
	: ICSDialog(CGyousyu::IDD, pParent)
{

}

CGyousyu::~CGyousyu()
{
}

void CGyousyu::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_RADIO1);
	DDX_Control(pDX, IDC_RADIO2, m_RADIO2);
	DDX_Control(pDX, IDC_RADIO3, m_RADIO3);
	DDX_Control(pDX, IDC_STATIC_TYPE, m_STATICTYPE);
}


BEGIN_MESSAGE_MAP(CGyousyu, ICSDialog)
	ON_BN_CLICKED(IDOK, &CGyousyu::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGyousyu::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CGyousyu::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CGyousyu::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CGyousyu::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CGyousyu メッセージ ハンドラ

BOOL CGyousyu::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	switch( m_Gyousyu ) {
		case 2 : m_RADIO2.SetCheck(1);	c_set( IDC_RADIO2 );	break;
		case 3 : m_RADIO3.SetCheck(1);	c_set( IDC_RADIO3 );	break;
		default: m_RADIO1.SetCheck(1);	c_set( IDC_RADIO1 );	break;
	}

//★//[12'03.12]///
	// キャプション変更
	if( m_Refund )	m_STATICTYPE.SetWindowTextA(_T("還付申告に関する明細書の連動情報を初期化します。") );
	else			m_STATICTYPE.SetWindowTextA(_T("仕入控除税額に関する明細書の連動情報を初期化します。") );
///////////////////

	ICSDialog::OnInitDialogEX();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CGyousyu::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::OnOK();
}

void CGyousyu::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ICSDialog::OnCancel();
}

void CGyousyu::OnBnClickedRadio1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_Gyousyu = 1;
}

void CGyousyu::OnBnClickedRadio2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_Gyousyu = 2;
}

void CGyousyu::OnBnClickedRadio3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_Gyousyu = 3;
}
