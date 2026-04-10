// CKoketsuKingaku.cpp : 実装ファイル
//

#include "stdafx.h"
#include "KoketsuKingaku.h"
#include "StzRendoDef.h"

// KoketsuKingaku ダイアログ

IMPLEMENT_DYNAMIC(CKoketsuKingaku, ICSDialog)

CKoketsuKingaku::CKoketsuKingaku(CDBZmSub *pZmSub, CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_KOKETSUKINGAKU, pParent)
{
	m_pZmSub = pZmSub;
	m_chkIdx = 0;
}

CKoketsuKingaku::~CKoketsuKingaku()
{
}

void CKoketsuKingaku::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_IppanCheck);
	DDX_Control(pDX, IDC_CHECK2, m_FudousanCheck);
	DDX_Control(pDX, IDC_CHECK3, m_NougyouCheck);
}


BEGIN_MESSAGE_MAP(CKoketsuKingaku, ICSDialog)
	ON_BN_CLICKED(IDOK, &CKoketsuKingaku::OnBnClickedOk)
END_MESSAGE_MAP()


// KoketsuKingaku メッセージ ハンドラー


BOOL CKoketsuKingaku::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//========================================================================
// 初期化
//========================================================================
void CKoketsuKingaku::Init()
{
	// 一般
	if( m_pZmSub->zvol->ind_type & 0x01 ) {
		m_IppanCheck.SetCheck( BST_CHECKED );
	}
	else {
		m_IppanCheck.SetCheck( BST_UNCHECKED );
		m_IppanCheck.EnableWindow( FALSE );
	}

	// 不動産
	if( m_pZmSub->zvol->ind_type & 0x02 ) {
		m_FudousanCheck.SetCheck( BST_CHECKED );
	}
	else {
		m_FudousanCheck.SetCheck( BST_UNCHECKED );
		m_FudousanCheck.EnableWindow( FALSE );
	}

	// 農業
	if( m_pZmSub->zvol->ind_type & 0x04 ) {
		m_NougyouCheck.SetCheck( BST_CHECKED );
	}
	else {
		m_NougyouCheck.SetCheck( BST_UNCHECKED );
		m_NougyouCheck.EnableWindow( FALSE );
	}
}

//========================================================================
// 選択された業種を取得
//------------------------------------------------------------------------
// 引数
//		取得した業種
//========================================================================
void CKoketsuKingaku::GetKoketsuKingauItem( int &idx )
{
	idx = m_chkIdx;
}

void CKoketsuKingaku::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// 一般
	if( m_IppanCheck.GetCheck() ) {
		m_chkIdx |= GYOSYU_IPPAN;
	}
	else {
		m_chkIdx &= ~GYOSYU_IPPAN;
	}

	// 不動産
	if( m_FudousanCheck.GetCheck() ) {
		m_chkIdx |= GYOSYU_FUDOUSAN;
	}
	else {
		m_chkIdx &= ~GYOSYU_FUDOUSAN;
	}

	// 農業
	if( m_NougyouCheck.GetCheck() ) {
		m_chkIdx |= GYOSYU_NOUGYOU;
	}
	else {
		m_chkIdx &= ~GYOSYU_NOUGYOU;
	}

	OnOK();
}
