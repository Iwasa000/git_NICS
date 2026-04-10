// EnvDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "EnvDialog.h"

#include "input1.h"
#include "external.h"


// EnvDialog ダイアログ

IMPLEMENT_DYNAMIC(EnvDialog, ICSDialog)

EnvDialog::EnvDialog(CWnd* pParent /*=NULL*/)
	: ICSDialog(EnvDialog::IDD, pParent)
{
	hAccel	=	NULL;
}

EnvDialog::~EnvDialog()
{
}

void EnvDialog::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC2, m_text);
	DDX_Control(pDX, IDC_RADIO1, m_fukugo);
	DDX_Control(pDX, IDC_RADIO2, m_tanitu);

	DDX_Control(pDX, IDC_RADIO_SKN1, m_SknInp);
	DDX_Control(pDX, IDC_RADIO_SKN2, m_SknNot);

	DDX_Control(pDX, IDC_RADIO_FURI1, m_FuriNormal);
	DDX_Control(pDX, IDC_RADIO_FURI2, m_FuriSyog);

	DDX_Control(pDX, IDC_RADIO_DNPCHK, m_DnpNoChk);
	DDX_Control(pDX, IDC_RADIO_DNPCHK2, m_DnpNoNone);
	DDX_Control(pDX, IDC_RADIO_DNPCHK3, m_DnpNoAuto);

	DDX_Control(pDX, IDC_RADIO_BMN1, m_BmnClr);
	DDX_Control(pDX, IDC_RADIO_BMN2, m_BmnNotClr);
}


void EnvDialog::SetExplain( int mode )
{
	CString	text;

	switch( mode ) {
		case	0:
			text	=	_T("プログラム開始時や仕訳登録の後等、新しい振替伝票を\r\n\r\n入力する時 『単一仕訳』で入力します。");
			break;
		case	1:
			text	=	_T("プログラム開始時や仕訳登録の後等、新しい振替伝票を\r\n\r\n入力する時 『複合仕訳』で入力します。");
			break;
		default:
			text.Empty();
			break;
	}
	m_text.SetWindowText( text );
}



BEGIN_MESSAGE_MAP(EnvDialog, ICSDialog)
	ON_BN_CLICKED(IDC_RADIO1, &EnvDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &EnvDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO_SKN1, &EnvDialog::OnBnClickedRadioSkn1)
	ON_BN_CLICKED(IDC_RADIO_SKN2, &EnvDialog::OnBnClickedRadioSkn2)
	ON_BN_CLICKED(IDC_RADIO_FURI1, &EnvDialog::OnBnClickedRadioFURI1)
	ON_BN_CLICKED(IDC_RADIO_FURI2, &EnvDialog::OnBnClickedRadioFURI2)
	ON_BN_CLICKED(IDC_RADIO_DNPCHK, &EnvDialog::OnBnClickedRadioDenpNoChk)
	ON_BN_CLICKED(IDC_RADIO_DNPCHK2, &EnvDialog::OnBnClickedRadioDenpNoChk2)
	ON_BN_CLICKED(IDC_RADIO_DNPCHK3, &EnvDialog::OnBnClickedRadioDenpNoAuto)
	ON_COMMAND(IDA_RETURN, &EnvDialog::OnReturn)
	ON_BN_CLICKED(IDC_RADIO_BMN1, &EnvDialog::OnBnClickedRadioBmn1)
	ON_BN_CLICKED(IDC_RADIO_BMN2, &EnvDialog::OnBnClickedRadioBmn2)
END_MESSAGE_MAP()


// EnvDialog メッセージ ハンドラ

BOOL EnvDialog::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	hAccel	=	::LoadAccelerators( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

	if( typeSiwake == 1 ) {
		m_fukugo.SetCheck( 1 );
		m_tanitu.SetCheck( 0 );
		GotoDlgCtrl( &m_fukugo );
	}
	else {
		m_fukugo.SetCheck( 0 );
		m_tanitu.SetCheck( 1 );
		GotoDlgCtrl( &m_tanitu );
	}

	SetExplain( typeSiwake );

	if (m_APno >= 0x50) {
		GetDlgItem(IDC_STATIC24)->SetWindowText("振替伝票で資金収支枝番入力");
	}

	if( m_APno < 0x50 || m_APno != 0x59 ) {
		if( sgnSknBrn == 1 ) {
			m_SknInp.SetCheck( 1 );
			m_SknNot.SetCheck( 0 );
		}
		else {
			m_SknInp.SetCheck( 0 );
			m_SknNot.SetCheck( 1 );
		}
	}
	else {
		m_SknInp.EnableWindow(FALSE);
		m_SknNot.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC24)->EnableWindow(FALSE);
	}

	if( m_bScan ) {
		m_FuriNormal.EnableWindow(FALSE);
		m_FuriSyog.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC28)->EnableWindow(FALSE);

		m_DnpNoNone.EnableWindow(FALSE);
		m_DnpNoChk.EnableWindow(FALSE);
		m_DnpNoAuto.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC30)->EnableWindow(FALSE);

		m_BmnClr.EnableWindow(FALSE);
		m_BmnNotClr.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC31)->EnableWindow(FALSE);
	}
	else {
		int sgn1, sgn2;
		if( syogMode ) {
			sgn1 = 0;	sgn2 = 1;
		}
		else {
			sgn1 = 1;	sgn2 = 0;
		}

		m_FuriNormal.SetCheck( sgn1 );
		m_FuriSyog.SetCheck( sgn2 );

		if( sgnDenpNo == 1 ) {
			m_DnpNoNone.SetCheck( 0 );
			m_DnpNoAuto.SetCheck( 0 );
			m_DnpNoChk.SetCheck( 1 );
		}
		else if (sgnDenpNo == 2) {
			m_DnpNoNone.SetCheck(0);
			m_DnpNoAuto.SetCheck(1);
			m_DnpNoChk.SetCheck(0);
		}
		else {
			m_DnpNoNone.SetCheck( 1 );
			m_DnpNoAuto.SetCheck(0);
			m_DnpNoChk.SetCheck( 0 );
		}

		//部門・工事ともになし
		if( sgnBmnClr == -1 || ! m_bBmnTan  ) {
			m_BmnClr.EnableWindow(FALSE);
			m_BmnNotClr.EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC31)->EnableWindow(FALSE);
		}
		else {
			CString tmp;
			if( BMON_MST ) {
				if( M_KOJI ) tmp = _T("部門／工事番号");
				else		 tmp = _T("部門番号");
			}
			else {
				if( M_KOJI ) tmp = _T("工事番号");
			}
			CString text;
			text.Format("伝票登録後の新規伝票の%s", tmp);
			GetDlgItem(IDC_STATIC31)->SetWindowText(text);

			if( sgnBmnClr ) {
				sgn1 = 1;	sgn2 = 0;
			}
			else {
				sgn1 = 0;	sgn2 = 1;
			}
			m_BmnClr.SetCheck(sgn1);
			m_BmnNotClr.SetCheck(sgn2);
		}
	}

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL EnvDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( hAccel ) {
		if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST ) {
			if( pMsg->wParam == VK_RETURN ) {
				return	::TranslateAccelerator( m_hWnd, hAccel, pMsg );
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


void EnvDialog::OnReturn()
{
	CWnd*	pWnd	=	GetFocus();
	if( pWnd ) {
		if( pWnd->GetSafeHwnd() == m_fukugo.m_hWnd || pWnd->GetSafeHwnd() == m_tanitu.m_hWnd ||
			pWnd->GetSafeHwnd() == m_SknInp.m_hWnd || pWnd->GetSafeHwnd() == m_SknNot.m_hWnd ||
			pWnd->GetSafeHwnd() == m_FuriNormal.m_hWnd || pWnd->GetSafeHwnd() == m_FuriSyog.m_hWnd ||
			pWnd->GetSafeHwnd() == m_DnpNoNone.m_hWnd || pWnd->GetSafeHwnd() == m_DnpNoAuto.m_hWnd ||
			pWnd->GetSafeHwnd() == m_DnpNoChk.m_hWnd ) {
	
			NextDlgCtrl();
		//	GotoDlgCtrl( GetDlgItem( IDOK ) );
		}
		else if( pWnd->GetSafeHwnd() == m_BmnClr.m_hWnd || pWnd->GetSafeHwnd() == m_BmnNotClr.m_hWnd ) {
			GotoDlgCtrl( GetDlgItem( IDOK ) );
		}
		else if( pWnd->GetSafeHwnd() == GetDlgItem( IDOK )->GetSafeHwnd() ) {
			OnOK();
		}
		else if( pWnd->GetSafeHwnd() == GetDlgItem( IDCANCEL )->GetSafeHwnd() ) {
			OnCancel();
		}
	}
}


void EnvDialog::OnBnClickedRadio1()
{
	typeSiwake	=	1;
	SetExplain( typeSiwake );
}

void EnvDialog::OnBnClickedRadio2()
{
	typeSiwake	=	0;
	SetExplain( typeSiwake );
}

void EnvDialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	ICSDialog::OnOK();
}


void EnvDialog::OnBnClickedRadioSkn1()
{
	sgnSknBrn	=	1;
}

void EnvDialog::OnBnClickedRadioSkn2()
{
	sgnSknBrn	=	0;
}

//伝票修正時の諸口仕訳 詰める
void EnvDialog::OnBnClickedRadioFURI1()
{
	syogMode	=	0;
}
//伝票修正時の諸口仕訳 詰めない
void EnvDialog::OnBnClickedRadioFURI2()
{
	syogMode	=	1;
}

//伝票番号チェック
void EnvDialog::OnBnClickedRadioDenpNoChk()
{
	sgnDenpNo	=	1;
}

void EnvDialog::OnBnClickedRadioDenpNoChk2()
{
	sgnDenpNo	=	0;
}

void EnvDialog::OnBnClickedRadioDenpNoAuto()
{
	sgnDenpNo = 2;
}


void EnvDialog::OnBnClickedRadioBmn1()
{
	sgnBmnClr = 1;
}

void EnvDialog::OnBnClickedRadioBmn2()
{
	sgnBmnClr = 0;
}

