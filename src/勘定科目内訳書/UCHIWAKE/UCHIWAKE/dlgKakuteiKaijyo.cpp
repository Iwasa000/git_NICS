// midori 152718 add -->
// dlgKakuteiKaijyo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgKakuteiKaijyo.h"


// CdlgKakuteiKaijyo ダイアログ

IMPLEMENT_DYNAMIC(CdlgKakuteiKaijyo, ICSDialog)

CdlgKakuteiKaijyo::CdlgKakuteiKaijyo(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgKakuteiKaijyo::IDD, pParent)
{
}

CdlgKakuteiKaijyo::~CdlgKakuteiKaijyo()
{
}

void CdlgKakuteiKaijyo::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DATA_KAKUTEI, m_chkDataKakutei);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CdlgKakuteiKaijyo, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CdlgKakuteiKaijyo::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CdlgKakuteiKaijyo::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CdlgKakuteiKaijyo メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgKakuteiKaijyo::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	int state = BST_UNCHECKED;

	// 入力データを確定する
	if(m_Kakutei)	state = BST_CHECKED;
	else			state = BST_UNCHECKED;
	m_chkDataKakutei.SetCheck(state);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//**************************************************
//	ダイアログ表示
//	【引数】	bKakutei			…	「入力データを確定する」サイン
//				pDB					…	データベースハンドル
//	【戻値】	ID_DLG_OK			…	OKボタンが押された
//				ID_DLG_CANCEL		…	キャンセルボタンが押された
//**************************************************
int CdlgKakuteiKaijyo::ShowDialog(BOOL* bKakutei, CDatabase* pDB /*=NULL*/ )
{
	m_nRet	= ID_DLG_CANCEL;				//	戻値を初期化
	m_Kakutei = *bKakutei;

	if( pDB != NULL )	m_pDB = pDB;

	DoModal();

	*bKakutei = m_Kakutei;

	return m_nRet;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OKボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKakuteiKaijyo::OnBnClickedButtonOk()
{
	m_Kakutei = m_chkDataKakutei.GetCheck();

	m_nRet = ID_DLG_OK;	//	戻値にOKをセット

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKakuteiKaijyo::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	キャンセルボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKakuteiKaijyo::OnBnClickedButtonCancel()
{
	ICSDialog::OnCancel();
}
