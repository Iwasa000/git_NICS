// ACDspOptDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "ACDspOptDlg.h"


#include "input1.h"
#include "external.h"
#include "mstrw.h"

// CACDspOptDlg ダイアログ

IMPLEMENT_DYNAMIC(CACDspOptDlg, ICSDialog)

CACDspOptDlg::CACDspOptDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CACDspOptDlg::IDD, pParent)
	, m_chkBumon(FALSE)
	, m_chkKoji(FALSE)
	, m_chkEda(FALSE)
	, m_bEnableBmn(TRUE)
	, m_chkKikan(FALSE)
{
	m_BmnYes = -1;
	m_KojiYes = -1;
	m_Hjiyu = -1;
	m_Zei = -1;
	m_DispDate = -1;
	m_Syz = -1;
}

CACDspOptDlg::~CACDspOptDlg()
{
}

void CACDspOptDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RD_DENPYES, m_DenpYes);
	DDX_Radio(pDX, IDC_RD_BMNYES, m_BmnYes);
	DDX_Radio(pDX, IDC_RD_KOJIYES, m_KojiYes);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Radio(pDX, IDC_RD_HJIYUYES, m_Hjiyu);
	DDX_Radio(pDX, IDC_RD_ZEIYES, m_Zei);
	DDX_Radio(pDX, IDC_RD_SYZYES, m_Syz);
	DDX_Check(pDX, IDC_CHECK1, m_chkBumon);
	DDX_Check(pDX, IDC_CHECK2, m_chkKoji);
	DDX_Check(pDX, IDC_CHECK3, m_chkEda);

	DDX_Radio(pDX, IDC_RD_DISPDATE, m_DispDate);
	DDX_Check(pDX, IDC_CHECK_KIKAN, m_chkKikan);
}


BEGIN_MESSAGE_MAP(CACDspOptDlg, ICSDialog)
END_MESSAGE_MAP()


// CACDspOptDlg メッセージ ハンドラ

BOOL CACDspOptDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	GetDlgItem(IDC_ST_BMN)->EnableWindow( BMON_MST );
	GetDlgItem(IDC_STATIC_WAK4)->EnableWindow( BMON_MST );
	GetDlgItem(IDC_RD_BMNYES)->EnableWindow( BMON_MST );
	GetDlgItem(IDC_RD_BMNNO)->EnableWindow( BMON_MST );

	// 建設以外は 工事をDisable
	GetDlgItem(IDC_ST_KOJI)->EnableWindow( M_KOJI );
	GetDlgItem(IDC_STATIC_WAK5)->EnableWindow( M_KOJI );
	GetDlgItem(IDC_RD_KOJIYES)->EnableWindow( M_KOJI );
	GetDlgItem(IDC_RD_KOJINO)->EnableWindow( M_KOJI );

	// TODO:  ここに初期化を追加してください

	if( ! IsHjiyuItemGyosyu() ) {
//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO) /*== 0x52*/ ) {
		GetDlgItem(IDC_ST_HJIYU)->EnableWindow( FALSE );
		GetDlgItem(IDC_STATIC_WAK9)->EnableWindow( FALSE );
		GetDlgItem(IDC_RD_HJIYUYES)->EnableWindow( FALSE );
		GetDlgItem(IDC_RD_HJIYUNO)->EnableWindow( FALSE );
	}

	// 基本科目 仕訳検索条件
	if( ! m_bChkQuery ) {
		GetDlgItem(IDC_CHECK1)->EnableWindow( FALSE );
		GetDlgItem(IDC_CHECK2)->EnableWindow( FALSE );
		GetDlgItem(IDC_CHECK3)->EnableWindow( FALSE );
		GetDlgItem(IDC_STATIC_WAK6)->EnableWindow( FALSE );

		GetDlgItem(IDC_STATIC_WAK7)->EnableWindow( FALSE );
		GetDlgItem(IDC_RD_DISPDATE)->EnableWindow( FALSE );
		GetDlgItem(IDC_RD_DISPSEQ)->EnableWindow( FALSE );

		GetDlgItem(IDC_STATIC_WAK8)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_KIKAN)->EnableWindow(FALSE);
	}
	else {
		if( BMON_MST ) {
			GetDlgItem(IDC_CHECK1)->EnableWindow( m_bEnableBmn );
		}
		else {
			GetDlgItem(IDC_CHECK1)->EnableWindow( FALSE );
		}
		GetDlgItem(IDC_CHECK2)->EnableWindow( M_KOJI );
		GetDlgItem(IDC_CHECK3)->EnableWindow( BRmst );
		GetDlgItem(IDC_STATIC_WAK6)->EnableWindow( TRUE );

		GetDlgItem(IDC_STATIC_WAK8)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_KIKAN)->EnableWindow(TRUE);
	}

	CString str = pDBzm->get_hjiyutitle(2);
	str += ":";
	GetDlgItem(IDC_ST_HJIYU)->SetWindowText( str );

	str.Format( "基本科目[%s]の仕訳表示条件", m_BaseKnam );
	GetDlgItem(IDC_STATIC_WAK6)->SetWindowText( str );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CACDspOptDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd ) {
			int id = pwnd->GetDlgCtrlID();
		
			switch( id ) {
			case IDC_CHECK1: case IDC_CHECK2: case IDC_CHECK3:
			case IDC_RD_BMNYES:		case IDC_RD_BMNNO:
			case IDC_RD_KOJIYES:	case IDC_RD_KOJINO:
			case IDC_RD_HJIYUYES:	case IDC_RD_HJIYUNO:
			case IDC_RD_ZEIYES:		case IDC_RD_ZEINO:
			case IDC_RD_SYZYES:		case IDC_RD_SYZNO:
			case IDC_RD_DISPDATE:	case IDC_RD_DISPSEQ:
			case IDC_RD_DENPYES:	case IDC_RD_DENPNO:

				switch( pMsg->wParam ) {
				case VK_RETURN : case VK_DOWN :
					NextDlgCtrl();	return TRUE;
				case VK_UP : 
					PrevDlgCtrl();	return TRUE;
				}
				break;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


void CACDspOptDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	ICSDialog::OnOK();
}
