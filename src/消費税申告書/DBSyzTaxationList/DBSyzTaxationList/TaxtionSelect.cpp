// TaxtionSelect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TaxtionSelect.h"


// CTaxtionSelect ダイアログ

IMPLEMENT_DYNAMIC(CTaxtionSelect, CDialog)

CTaxtionSelect::CTaxtionSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CTaxtionSelect::IDD, pParent)
{

}

CTaxtionSelect::~CTaxtionSelect()
{
}

void CTaxtionSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
}


BEGIN_MESSAGE_MAP(CTaxtionSelect, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CTaxtionSelect::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTaxtionSelect::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CTaxtionSelect メッセージ ハンドラ

void CTaxtionSelect::OnBnClickedRadio1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_KHstax = 0;
}

void CTaxtionSelect::OnBnClickedRadio2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_KHstax = 1;
}

BOOL CTaxtionSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxtionSelect::Init()
{
	if( m_KHstax ){
		m_Radio2.SetCheck( 1 );
	}
	else{
		m_Radio1.SetCheck( 2 );
	}

	return 0;
}
