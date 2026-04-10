// DispOptionDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"
#include "DispOptionDlg.h"

#include "DBDinpRec.h"

#include "input1.h"
#include "external.h"
#include "mstrw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispOptionDlg ダイアログ


CDispOptionDlg::CDispOptionDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CDispOptionDlg::IDD, pParent)

{
	//{{AFX_DATA_INIT(CDispOptionDlg)
	m_Sw1 = -1;
	m_BmnYes = -1;
	m_DenpYes = -1;
	m_KojiYes = -1;
	m_SyoYes = -1;
	m_TgdtYes = -1;
	m_Hjiyu = -1;
	m_Zei = -1;
	//}}AFX_DATA_INIT

	m_bShow = FALSE;

	m_bEnaMove = FALSE;
}


void CDispOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispOptionDlg)
	DDX_Radio(pDX, IDC_RADIO_SW1, m_Sw1);
	DDX_Radio(pDX, IDC_RD_BMNYES, m_BmnYes);
	DDX_Radio(pDX, IDC_RD_DENPYES, m_DenpYes);
	DDX_Radio(pDX, IDC_RD_KOJIYES, m_KojiYes);
	DDX_Radio(pDX, IDC_RD_SYOYES, m_SyoYes);
	DDX_Radio(pDX, IDC_RD_TGDTYES, m_TgdtYes);
	DDX_Radio(pDX, IDC_RD_LNHT1, m_LnHt1);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RD_HJIYUYES, m_Hjiyu);
	DDX_Radio(pDX, IDC_RD_ZEIYES, m_Zei);
	DDX_Radio(pDX, IDC_RD_SYZYES, m_Syz);
	DDX_Control(pDX, IDC_CUSTOM1, m_Blwnd);
}


BEGIN_MESSAGE_MAP(CDispOptionDlg, ICSDialog)
	//{{AFX_MSG_MAP(CDispOptionDlg)
	ON_BN_CLICKED(IDC_RADIO_SW1, OnRadioSw1)
	ON_BN_CLICKED(IDC_RADIO_SW2, OnRadioSw2)
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SELDSP_CHK, &CDispOptionDlg::OnBnClickedSeldspChk)
	ON_BN_CLICKED(IDC_BTN_INIT, &CDispOptionDlg::OnBnClickedBtnInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispOptionDlg メッセージ ハンドラ

BOOL CDispOptionDlg::OnInitDialog() 
{
//	ControlInit( 233, 189, -1 );
//	kakudai(this);

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


	if( ! IsHjiyuItemGyosyu() ) {
	//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {
			GetDlgItem(IDC_ST_HJIYU)->EnableWindow( FALSE );
			GetDlgItem(IDC_STATIC_WAK9)->EnableWindow( FALSE );
			GetDlgItem(IDC_RD_HJIYUYES)->EnableWindow( FALSE );
			GetDlgItem(IDC_RD_HJIYUNO)->EnableWindow( FALSE );
	//	}
	}

	SetDoptControlState( m_Sw1+1 ); 

	CString str = pDBzm->get_hjiyutitle(2);
	str += ":";
	GetDlgItem(IDC_ST_HJIYU)->SetWindowText( str );

	if( ! m_bEnaMove ) {
		m_Blwnd.ShowWindow( SW_HIDE );

		GetDlgItem(IDC_ST_BLKMOVE)->EnableWindow( FALSE );
		GetDlgItem(IDC_SELDSP_CHK)->EnableWindow( FALSE );
		GetDlgItem(IDC_BTN_INIT)->EnableWindow( FALSE );
	}

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDispOptionDlg::OnRadioSw1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDoptControlState(1);
}

void CDispOptionDlg::OnRadioSw2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDoptControlState(2);
}


void CDispOptionDlg::SetDoptControlState( int line )
{
	BOOL bEnable;
	if( line == 1 )	bEnable = FALSE;
	else			bEnable = TRUE;

	GetDlgItem(IDC_ST_LNHT )->EnableWindow( ! bEnable );
	GetDlgItem(IDC_RD_LNHT1)->EnableWindow( ! bEnable );
	GetDlgItem(IDC_RD_LNHT2)->EnableWindow( ! bEnable );

	GetDlgItem(IDC_ST_SYONO)->EnableWindow( bEnable );
	GetDlgItem(IDC_RD_SYOYES)->EnableWindow( bEnable );
	GetDlgItem(IDC_RD_SYONO)->EnableWindow( bEnable );
	GetDlgItem(IDC_ST_TGDT)->EnableWindow( bEnable );
	GetDlgItem(IDC_RD_TGDTYES)->EnableWindow( bEnable );
	GetDlgItem(IDC_RD_TGDTNO)->EnableWindow( bEnable );
}



BOOL CDispOptionDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd ) {
			int id = pwnd->GetDlgCtrlID();
		
			switch( id ) {
			case IDC_RADIO_SW1:		case IDC_RADIO_SW2:
			case IDC_RD_LNHT1:		case IDC_RD_LNHT2:
			case IDC_RD_DENPYES:	case IDC_RD_DENPNO:
			case IDC_RD_BMNYES:		case IDC_RD_BMNNO:
			case IDC_RD_KOJIYES:	case IDC_RD_KOJINO:
			case IDC_RD_SYOYES:		case IDC_RD_SYONO:
			case IDC_RD_TGDTYES:	case IDC_RD_TGDTNO:
			case IDC_RD_HJIYUYES:	case IDC_RD_HJIYUNO:
			case IDC_RD_ZEIYES:		case IDC_RD_ZEINO:
			case IDC_RD_SYZYES:		case IDC_RD_SYZNO:
			case IDC_SELDSP_CHK:

				switch( pMsg->wParam ) {
				case VK_RETURN : case VK_DOWN :
					NextDlgCtrl();	return TRUE;
				case VK_UP : 
					PrevDlgCtrl();	return TRUE;
				}
				break;
#ifdef TEST_CUT
			case IDC_CHK_DEFCOLOR:
				switch( pMsg->wParam ) {
				case VK_RETURN : 
				case VK_DOWN : case VK_RIGHT :
					NextDlgCtrl();	return TRUE;
				case VK_UP : case VK_LEFT :
					PrevDlgCtrl();	return TRUE;
				}
				break;
#endif
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

void CDispOptionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ICSDialog::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( bShow ) {
		if( ! m_bShow ) {
			m_bShow = TRUE;

			m_Blwnd.Initialize();

			if( !m_Blwnd.GetSelDispShow() ) {
				((CButton*)GetDlgItem(IDC_SELDSP_CHK))->SetCheck(BST_CHECKED); 
			}
		}
	}
}


// ブロック単位の画面情報をセット
void CDispOptionDlg::SetBlockInfo( BlockInfo* pBlk )
{
	m_Blwnd.SetBlockInfo( pBlk );
}


int CDispOptionDlg::GetBlockIdx( int* pBlkIdx )
{
	return m_Blwnd.GetBlockIdx( pBlkIdx );
}

// 選択画面の表示をしない
void CDispOptionDlg::OnBnClickedSeldspChk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	BOOL bShow = TRUE;

	if( ((CButton*)GetDlgItem(IDC_SELDSP_CHK))->GetCheck() == BST_CHECKED ) {
		bShow = FALSE;
	}

	m_Blwnd.SetSelDispShow( bShow );
}


BOOL CDispOptionDlg::GetSelDispShow()
{
	return m_Blwnd.GetSelDispShow();
}

// 画面を初期位置へ
void CDispOptionDlg::OnBnClickedBtnInit()
{
	m_Blwnd.InitDispBlk();
}
