// RDDnpAddDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "RDDnpAddDlg.h"

#include "input1.h"
#include "external.h"

// CRDDnpAddDlg ダイアログ

// 定型仕訳
extern CMKRDSub Mkrd;

#include "DnpInView.h"
//#include "DinpSubFnc.h"
extern CDnpInView* p_DnpView;

#include "DBDinpDoc.h"
#include "DBDinpView.h"
extern class CDBDinpView *p_DBDinpView;


IMPLEMENT_DYNAMIC(CRDDnpAddDlg, ICSDialog)

CRDDnpAddDlg::CRDDnpAddDlg(BYTE dtype, BOOL bNameChg/*=FALSE*/, CWnd* pParent /*=NULL*/)
	: ICSDialog(CRDDnpAddDlg::IDD, pParent)
{
	m_Dtype	= dtype;
	m_bNameChg	= bNameChg;

	m_bOverWrite = FALSE;
}

CRDDnpAddDlg::~CRDDnpAddDlg()
{
}

void CRDDnpAddDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRDDnpAddDlg, ICSDialog)
	ON_BN_CLICKED(IDOK, &CRDDnpAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRDDnpAddDlg メッセージ ハンドラ

BOOL CRDDnpAddDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	ICSDialog::OnInitDialogEX();

	//名前の変更
	if( m_bNameChg ) {
		SetWindowText("定型伝票 名称変更");

		CWnd* pwnd;
		pwnd = GetDlgItem(IDC_STATIC_GUID);
		pwnd->SetWindowText( "定型伝票の名称を変更してください。" );

		CRect baseRect;
		pwnd->GetWindowRect( &baseRect );
		ScreenToClient( &baseRect );

		GetDlgItem(IDC_ST_GRPNAME)->ShowWindow( SW_HIDE );

		CRect rect;

		pwnd = GetDlgItem(IDC_ICSDBEDTCTRL1);
		pwnd->GetWindowRect( &rect );
		ScreenToClient( &rect );

		int hMove = rect.top - baseRect.bottom;
		pwnd->MoveWindow( rect.left, rect.top - hMove, rect.Width(), rect.Height() );

		pwnd = GetDlgItem(IDOK);
		pwnd->GetWindowRect( &rect );
		ScreenToClient( &rect );
		pwnd->MoveWindow( rect.left, rect.top - hMove, rect.Width(), rect.Height() );
		pwnd->SetWindowText( "変更[F9]" );

		pwnd = GetDlgItem(IDCANCEL);
		pwnd->GetWindowRect( &rect );
		ScreenToClient( &rect );
		pwnd->MoveWindow( rect.left, rect.top - hMove, rect.Width(), rect.Height() );

		VARIANT var;
		char bf[80] = {0};

		strcpy_s( bf, sizeof bf, m_LabelName );
		var.pbVal = (BYTE*)bf;
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		GetWindowRect(&rect);
		MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() - hMove );
	}

//	c_set( IDC_ICSDBEDTCTRL1 );
//	return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//
void CRDDnpAddDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	VARIANT var;
	char bf[80] = {0};

	DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	strcpy_s( bf, sizeof bf, (const char*)var.pbVal );

	if( bf[0] == 0 ) {
		myICSMessageBox( "定型伝票の名称が入力されていません！" );
		GotoDlgCtrl( GetDlgItem(IDC_ICSDBEDTCTRL1) );
		return;
	}

	CString filter;
	int lbseq = Mkrd.dnpLabel->rdp_lbseq;

	CString tmp;
	tmp = bf;
	AdjustSQLsearch( tmp );

	int owner;
	if( p_DBDinpView->m_CmnId > 0 )		owner = p_DBDinpView->m_CmnId;
	else								owner = 0;

	filter.Format( "rdp_name = '%s' COLLATE Japanese_CI_AS_KS_WS and rdp_dtype = %d and rdp_owner = %d", tmp, m_Dtype, owner );

	Mkrd.dnpLabel->Requery( filter );

	BOOL bOverWrite = FALSE;

	if( m_bNameChg ) {
		if( Mkrd.dnpLabel->st == 0 ) {
			if( lbseq == Mkrd.dnpLabel->rdp_lbseq ) {
				myICSMessageBox( "名称が変更されていません！" );
				BackDnpLabel( lbseq );
				return;
			}
			int st = myICSMessageBox( "同じ名称の定型伝票が登録されているので、変更できません！" );
			GotoDlgCtrl( GetDlgItem(IDC_ICSDBEDTCTRL1) );
			BackDnpLabel( lbseq );
			return;
		}
	}
	else {
		if( Mkrd.dnpLabel->st == 0 ) {
			//上書き保存
			int st = myICSMessageBox( "同じ名称の定型伝票が登録されています。上書きしますか？", MB_YESNO|MB_DEFBUTTON2 );
			if( st == IDYES ) {
				bOverWrite = TRUE;
				m_strOverName = bf;
			}
			else {
				BackDnpLabel( lbseq );
				return;
			}
		}
	}

	m_bOverWrite = bOverWrite;
	//元の位置に戻す
	if( ! bOverWrite ) {
		BackDnpLabel( lbseq );
	}

	p_DnpView->RDDnpUpdateJob( bf, m_Dtype, bOverWrite, m_bNameChg );

	EndDialog( IDOK );
}


// 定型伝票ラベルを元に戻す
void CRDDnpAddDlg::BackDnpLabel( int back_lbseq )
{
	CString filter;
	//元の位置に戻す

	int owner;
	if( p_DBDinpView->m_CmnId > 0 )		owner = p_DBDinpView->m_CmnId;
	else								owner = 0;

	filter.Format("rdp_dtype = %d and rdp_owner = %d", m_Dtype, owner);
	Mkrd.dnpLabel->Requery( filter );

	if( Mkrd.dnpLabel->st == 0 ) {
		do {
			if( Mkrd.dnpLabel->rdp_lbseq == back_lbseq )
				break;
		} while( Mkrd.dnpLabel->MoveNext() == 0 );
	}

	if( back_lbseq == -1 ) {
		Mkrd.dnpLabel->SetAbsolutePosition( m_AbsPos );
	}
}


BEGIN_EVENTSINK_MAP(CRDDnpAddDlg, ICSDialog)
	ON_EVENT(CRDDnpAddDlg, IDC_ICSDBEDTCTRL1, 1, CRDDnpAddDlg::TerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CRDDnpAddDlg::TerminationIcsdbedtctrl1(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == VK_RETURN ) {
		GotoDlgCtrl( GetDlgItem(IDOK) );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		PrevDlgCtrl();
	}
	else if( nChar == VK_RIGHT ) {
		NextDlgCtrl();
	}

}


BOOL CRDDnpAddDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_ESCAPE ) {
			EndDialog(IDCANCEL);
			return TRUE;
		}

		switch( pMsg->wParam ) {
		case VK_F7:
			if( ! m_bNameChg ) {
				PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)GetDlgItem(IDOK)->m_hWnd);
			}
			break;
		case VK_F9:
			if( m_bNameChg ) {
				PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)GetDlgItem(IDOK)->m_hWnd);
			}
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
