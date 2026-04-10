// RdgrpAddDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "RdgrpAddDlg.h"

#include "MkrdSub.h"

#include "input1.h"
#include "external.h"

#include "MstRw.h"



extern CMKRDSub	Mkrd;

// CRdgrpAddDlg ダイアログ


IMPLEMENT_DYNAMIC(CRdgrpAddDlg, ICSDialog)

CRdgrpAddDlg::CRdgrpAddDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CRdgrpAddDlg::IDD, pParent)
{

}

CRdgrpAddDlg::~CRdgrpAddDlg()
{
}

void CRdgrpAddDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRdgrpAddDlg, ICSDialog)
END_MESSAGE_MAP()


// CRdgrpAddDlg メッセージ ハンドラ

BOOL CRdgrpAddDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	// TODO:  ここに初期化を追加してください


	ICSDialog::OnInitDialogEX();

	VARIANT var;
	var.pbVal = NULL;

	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	c_set( IDC_ICSDBEDTCTRL1 );

	CString msg;
	msg.Format( "追加仕訳グループ: %2d.", m_LbCnt+1 );
	GetDlgItem(IDC_STATIC1)->SetWindowText( msg );

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CRdgrpAddDlg::OnOK()
{
	VARIANT var;
	char bf[80] = {0};

	DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	strcpy_s( bf, sizeof bf, (const char*)var.pbVal );

	int	owner;

	if( m_CmnId > 0 )	owner = m_CmnId;
	else				owner = 0;

	Mkrd.label->dealname	= bf;
	Mkrd.label->owner		= owner;

	int st = myICSMessageBox("定型仕訳グループを追加しますか？", MB_YESNO, 0,0,this);

	if( st == IDNO )
		return;

	CString filter;
	filter.Format( "rl_owner = %d", owner );

	if( Mkrd.label->GetRecordCount() >= GetRDLabelMax() ) {
		CString msg;
		msg.Format( "既に最大登録数(＝%d)まで登録されています。", GetRDLabelMax() ); 
		ICSMessageBox( msg, 0,0,0,this);
	}
	else {
		if( Mkrd.label->Append() != 0 ) {	// ラベル追加
			ICSMessageBox( "定型仕訳グループの追加ができません。" );
			return;
		}

		filter.Format( "rl_owner = %d", owner );	// ユーザごと
		Mkrd.label->Requery( filter );
	}

	ICSDialog::OnOK();
}


BEGIN_EVENTSINK_MAP(CRdgrpAddDlg, ICSDialog)
	ON_EVENT(CRdgrpAddDlg, IDC_ICSDBEDTCTRL1, 1, CRdgrpAddDlg::TerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CRdgrpAddDlg::TerminationIcsdbedtctrl1(long nChar, long inplen, long kst)
{
	BOOL	shift	=	FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		shift	=	TRUE;
	}

	if( nChar == VK_RETURN || (nChar == VK_TAB && ! shift) || nChar == VK_DOWN ||
		nChar == VK_RIGHT /*|| nChar == VK_F3*/ ) {
		c_set( IDOK );
	}
	else if( nChar == VK_UP || (nChar == VK_TAB && shift) || nChar == VK_F2 ||
		nChar == VK_LEFT ) {
		c_set( IDCANCEL );
	}
}
