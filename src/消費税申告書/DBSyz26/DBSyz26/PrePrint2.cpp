// PrePrint2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PrePrint2.h"


// CPrePrint2 ダイアログ

IMPLEMENT_DYNAMIC(CPrePrint2, CDialog)

CPrePrint2::CPrePrint2(CWnd* pParent /*=NULL*/)
	: CDialog(CPrePrint2::IDD, pParent)
{

}

CPrePrint2::~CPrePrint2()
{
}

void CPrePrint2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MEMO, m_Edit_Memo);
	DDX_Control(pDX, IDC_CHECK_KAIGYO, m_Check_Kaigyo);
}


BEGIN_MESSAGE_MAP(CPrePrint2, CDialog)
	ON_BN_CLICKED(IDOK, &CPrePrint2::OnBnClickedOk)
END_MESSAGE_MAP()


// CPrePrint2 メッセージ ハンドラ

BOOL CPrePrint2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	char chr[512];
	memset( chr, '\0', sizeof( chr ) );
	//memmove( chr, m_pSnHeadData->Sn_MEMO, 183 );

	CString str;
	str.Format( chr );

	m_Edit_Memo.Make_Edit( 3, 60, str, ZEN_ST );

	//memmove( MEMO, m_pSnHeadData->Sn_MEMO, sizeof(MEMO));
	m_Edit_Memo.SetWindowTextA(MEMO);

	//KAIGYO = m_pSnHeadData->Sn_KAIGYO;
	if( KAIGYO == 0x00 ){
		m_Check_Kaigyo.SetCheck(BST_UNCHECKED);
	}else{
		m_Check_Kaigyo.SetCheck(BST_CHECKED);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CPrePrint2::InitInfo( CSnHeadData *pSnHeadData )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	return 0;
}


void CPrePrint2::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	m_Edit_Memo.GetWindowTextA( MEMO, sizeof(MEMO));
	//memmove( m_pSnHeadData->Sn_MEMO, MEMO, sizeof(MEMO));

	if( m_Check_Kaigyo.GetCheck() == BST_UNCHECKED ){
		KAIGYO = 0x00;
	}else{
		KAIGYO = 0x01;
	}
	//m_pSnHeadData->Sn_KAIGYO = KAIGYO;

	CDialog::OnOK();
}

void CPrePrint2::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//memmove( m_pSnHeadData->Sn_MEMO, MEMO, sizeof(MEMO));

	//m_pSnHeadData->Sn_KAIGYO = KAIGYO;

	CDialog::OnCancel();
}


void CPrePrint2::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。


	OnOK();
}
