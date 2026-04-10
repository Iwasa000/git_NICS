// TkySel.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DBInpSubFunc.h"
#include "TkySel.h"


// CTkySel ダイアログ
CString	CTkySel::m_orgTky;
int		CTkySel::m_orgPos = 0;


IMPLEMENT_DYNAMIC(CTkySel, ICSDialog)

CTkySel::CTkySel(CWnd* pParent /*=NULL*/)
	: ICSDialog(CTkySel::IDD, pParent)
{
	m_White.CreateSolidBrush( CL_WHITE );
}

CTkySel::~CTkySel()
{
}

void CTkySel::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_LEFTBUTTON, m_EBLeft);
	DDX_Control(pDX, IDC_RIGHTBUTTON, m_EBRight);
	DDX_Control(pDX, IDC_ADDBUTTON, m_EBAdd);
	DDX_Control(pDX, IDC_SUBBUTTON, m_EBSub);
	DDX_Control(pDX, IDC_HOMEBUTTON, m_EBHome);
}


BEGIN_MESSAGE_MAP(CTkySel, ICSDialog)
	ON_BN_CLICKED(IDC_HOMEBUTTON, OnHomebutton)
	ON_BN_CLICKED(IDC_LEFTBUTTON, OnLeftbutton)
	ON_BN_CLICKED(IDC_RIGHTBUTTON, OnRightbutton)
	ON_BN_CLICKED(IDC_SUBBUTTON, OnSubbutton)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTkySel メッセージ ハンドラ

BOOL CTkySel::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	UINT btnID[] = {
		IDC_LEFTBUTTON,IDC_RIGHTBUTTON,IDC_ADDBUTTON,IDC_SUBBUTTON,IDC_HOMEBUTTON
	};

	CString strMsg;
	strMsg.Format( _T( "登録摘要指定（最大全角%s文字）"), m_TKYO_LEN == 20 ? "１０" : "２０" );
	GetDlgItem( IDC_STATIC1 )->SetWindowText( strMsg );

	if( m_orgTky.GetLength() > 0 ) {
		// エディット 初期化
		TKY_VER2* ptv = get_tkyver2();
		int sel1 = LOWORD( ptv->t2_sel );
		int sel2 = HIWORD( ptv->t2_sel );

		m_Edit.OnInitial( m_orgPos, (char*)(LPCTSTR)m_orgTky, m_TKYO_LEN, sel1, sel2 );
		m_Edit.InitCtrl(this, btnID, IDC_STATIC2);
		GotoDlgCtrl( (CWnd*)&m_Edit );
		//初期状態 (Home)
		PostMessage( WM_COMMAND, MAKELONG(IDC_HOMEBUTTON, BN_CLICKED),
									(LPARAM)GetDlgItem(IDC_HOMEBUTTON)->m_hWnd );
	}
	else {
		// 文字列がないため 文字列選択 ＆ 追加 は なし
		for( int i = 0; i < (sizeof(btnID)/sizeof(btnID[0])); i++ ) {
			GetDlgItem( btnID[i] )->EnableWindow(FALSE );
		}
		GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE); // 文字数表示 static
		m_Edit.EnableWindow(FALSE);

	}
	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



void CTkySel::OnLeftbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.LeftFunc();	
}

void CTkySel::OnRightbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.RightFunc();	
}

void CTkySel::OnAddbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.AddFunc();	
}

void CTkySel::OnSubbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.SubFunc();	
}

void CTkySel::OnHomebutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.HomeFunc();	
}


void CTkySel::OnOK()
{
	::ZeroMemory( m_Teki, sizeof m_Teki );;
	m_Edit.get_dictky( m_Teki, sizeof m_Teki );

	ICSDialog::OnOK();
}

HBRUSH CTkySel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_STATIC ) {
		int id = pWnd->GetDlgCtrlID();

		if( id == IDC_STATIC2 ) {
			// ｴﾃﾞｨｯﾄに ﾌｫｰｶｽがあるとき
			if( m_Edit.IsSetFocus() ) {
				hbr = (HBRUSH)m_White.GetSafeHandle();
				pDC->SetTextColor( CL_BLACK );
				pDC->SetBkColor( CL_WHITE );
			}
		}
	}	

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}
