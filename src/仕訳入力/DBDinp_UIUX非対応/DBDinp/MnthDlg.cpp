// MnthDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "MnthDlg.h"

#include "input1.h"
#include "external.h"


// CMnthDlg ダイアログ
int CMnthDlg::m_TanOfst = -1;


IMPLEMENT_DYNAMIC(CMnthDlg, ICSDialog)

CMnthDlg::CMnthDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CMnthDlg::IDD, pParent)
{
	m_bTan = FALSE;
	m_bKikan = FALSE;
}

CMnthDlg::~CMnthDlg()
{
}

void CMnthDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MNTHSEL, m_selDate);
}


BEGIN_MESSAGE_MAP(CMnthDlg, ICSDialog)
END_MESSAGE_MAP()


// CMnthDlg メッセージ ハンドラ

BOOL CMnthDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_selDate.SetMode( ICS_DATESELECT_MODE_NONE );

	DWORD dwRange;

	if( m_bTan )	{
		dwRange = ICS_DATESELECT_RANGE_MONTH | ICS_DATESELECT_EDIT_OFF;
	}
	else {
		dwRange = ICS_DATESELECT_RANGE_PERIOD | ICS_DATESELECT_EDIT_ON;
	}
	GetDlgItem(IDC_STATIC1)->SetWindowText( (LPCTSTR)m_st1 );

	m_selDate.Iniz( pDBzm, dwRange|/*ICS_DATESELECT_EDIT_ON|*/ICS_DATESELECT_EDIT_M|ICS_DATESELECT_INPOK_ONLY, 0, 0 );

	if( m_bKikan ) {
	}
	else {
		pDBzm->get_datelimit(m_Sofs, m_Eofs, m_Sdate, m_Edate);
	}

	if( m_bTan ) {
		if( m_TanOfst == -1 ) {
			m_TanOfst = m_Sofs;
		}
		m_selDate.SetMonthOffset( m_TanOfst, m_TanOfst, m_Sdate, m_Edate ); 
	}
	else {
		m_selDate.SetMonthOffset( m_Sofs, m_Eofs, m_Sdate, m_Edate ); 
	}

	ICSDialog::OnInitDialogEX();

	if( m_bTan )	{
		CRect rect, mnthRC;

		GetDlgItem(IDC_MNTHSEL)->GetWindowRect( mnthRC );
		ScreenToClient(mnthRC);

		GetDlgItem(IDC_STATIC1)->GetWindowRect( rect );
		ScreenToClient(rect);

		int h = rect.Height();

		rect.top	= 	mnthRC.top;
		rect.bottom =	rect.top + h;
		GetDlgItem(IDC_STATIC1)->MoveWindow( rect );

		h = mnthRC.Height();
		mnthRC.top		= rect.bottom + 4;
		mnthRC.bottom	= mnthRC.top + h;
		GetDlgItem(IDC_MNTHSEL)->MoveWindow( mnthRC );

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CMnthDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	m_selDate.GetMonthOffset( m_Sofs, m_Eofs, m_Sdate, m_Edate );
	if( m_bTan ) {
		m_TanOfst = m_Sofs;
	}

	ICSDialog::OnOK();
}

BOOL CMnthDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_ESCAPE ) {
			OnCancel();
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
