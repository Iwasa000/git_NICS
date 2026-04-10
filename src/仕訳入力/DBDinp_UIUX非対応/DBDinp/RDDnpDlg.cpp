// RDDnpDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "RDDnpDlg.h"

// 定型仕訳
extern CMKRDSub Mkrd;

#include "input1.h"
#include "external.h"

#include "DnpInView.h"

extern CDnpInView* p_DnpView;

#include "RDnpNSdlg.h"
#include "RDnpFuriDlg.h"
#include "MstRw.h"

#include "RDDnpAddDlg.h"

#include "DBDinpDoc.h"
#include "DBDinpView.h"
extern class CDBDinpView *p_DBDinpView;

#include "CStRecordSet.h"

// CRDDnpDlg ダイアログ

IMPLEMENT_DYNAMIC(CRDDnpDlg, ICSDialog)

CRDDnpDlg::CRDDnpDlg( BYTE dtype, int cmnId, CWnd* pParent /*=NULL*/)
	: ICSDialog(CRDDnpDlg::IDD, pParent)
{
	m_Dtype = dtype;
	m_CmnId = cmnId;

	m_bAddJob	= FALSE;
	m_dnpDataID = 0;

	m_chkVal	= FALSE;
	p_DnpView->RDDnpSetChkVal( m_chkVal );
}

CRDDnpDlg::~CRDDnpDlg()
{
}

void CRDDnpDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Check(pDX, IDC_RD_VALCHK, m_chkVal);
	DDX_Control( pDX, IDC_CHK_ZEIRITSU, m_Zritu );
}


BEGIN_MESSAGE_MAP(CRDDnpDlg, ICSDialog)
	ON_BN_CLICKED(IDC_BTN_REGDNP, &CRDDnpDlg::OnBnClickedBtnRegdnp)
	ON_BN_CLICKED(IDC_BTN_DNPSET, &CRDDnpDlg::OnBnClickedBtnDnpset)
	ON_BN_CLICKED(IDC_BTN_DNPDEL, &CRDDnpDlg::OnBnClickedBtnDnpdel)
	ON_LBN_DBLCLK(IDC_LIST1, &CRDDnpDlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BTN_REGOVW, &CRDDnpDlg::OnBnClickedBtnRegOVW)
	ON_BN_CLICKED(IDC_BTN_CHGNAM, &CRDDnpDlg::OnBnClickedBtnChgNam)
	ON_BN_CLICKED(IDC_BTN_CONTXT, &CRDDnpDlg::OnBnClickedBtnContxt)

	ON_BN_CLICKED(IDC_RD_VALCHK, &CRDDnpDlg::OnBnClickedRdValchk)
	ON_BN_CLICKED(IDC_CHK_ZEIRITSU, &CRDDnpDlg::OnBnClickedChkZeiritsu )
END_MESSAGE_MAP()


// CRDDnpDlg メッセージ ハンドラ

BOOL CRDDnpDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	DnpLabelSet();

	int lastline = p_DnpView->check_dnplndata();
	if( lastline <= 0 ) {
		GetDlgItem( IDC_BTN_REGDNP )->EnableWindow( FALSE );
		GetDlgItem( IDC_BTN_REGOVW )->EnableWindow( FALSE );
	}
	if( m_List.GetCount() <= 0 ) {
		GetDlgItem( IDC_BTN_DNPDEL )->EnableWindow( FALSE );
		GetDlgItem( IDC_BTN_REGOVW )->EnableWindow( FALSE );
		GetDlgItem( IDC_BTN_CONTXT )->EnableWindow( FALSE );
		GetDlgItem( IDC_BTN_CHGNAM )->EnableWindow( FALSE );
	}

	if (IsMasterType( MST_SYZ8_10 )) {
		GetDlgItem( IDC_CHK_ZEIRITSU )->ShowWindow( SW_SHOW );
	}
	else {
		GetDlgItem( IDC_CHK_ZEIRITSU )->ShowWindow( SW_HIDE );
	}
	int btnSt = BST_CHECKED;
	if (!RDsiwakeZeirituMode())	btnSt = BST_UNCHECKED;
	m_Zritu.SetCheck( btnSt );

	CString	str;
	char buf[64];

	char* pFuri = "振替伝票";
	char* pSkin = "出金伝票";
	char* pNkin = "入金伝票";

	switch( m_Dtype ) {
	case 0x30:	strcpy_s( buf, sizeof buf, pFuri );	break;
	case 0x20:	strcpy_s( buf, sizeof buf, pSkin );	break;
	case 0x10:	strcpy_s( buf, sizeof buf, pNkin );	break;
	}

	str.Format( "登録済み定型伝票[%s]", buf );
	GetDlgItem( IDC_ST_TOROK )->SetWindowText( str );
	str.Format( "%sに入力[F12]", buf );
	GetDlgItem( IDC_BTN_DNPSET )->SetWindowText( str );

	m_List.SetCurSel(0);

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//定型伝票に新規登録
void CRDDnpDlg::OnBnClickedBtnRegdnp()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_bAddJob = TRUE;

	CRDDnpAddDlg	dlg( m_Dtype, FALSE, this );
	int abs_pos = ListSelDataQuery(TRUE);
	dlg.m_AbsPos = abs_pos;

	if( dlg.DoModal() == IDOK ) {
		int sel, cnt;
		sel = m_List.GetCurSel();

		DnpLabelSet();
		cnt = m_List.GetCount();
		if( cnt == -1 )	sel = 0;
		else if( ! dlg.m_bOverWrite ) {
			sel = cnt-1;
		}
		else {
			Mkrd.dnpLabel->MoveFirst();
			sel = 0;
			if( Mkrd.dnpLabel->st != -1 ) {
				do {
					if( dlg.m_strOverName.Compare( Mkrd.dnpLabel->rdp_name ) == 0 ) {
						break;
					}
					sel++;
				}while( Mkrd.dnpLabel->MoveNext()==0);
			}
		}
		m_List.SetCurSel(sel);

		if( cnt > 0 ) {
			GetDlgItem( IDC_BTN_DNPDEL )->EnableWindow( TRUE );
			GetDlgItem( IDC_BTN_REGOVW )->EnableWindow( TRUE );
			GetDlgItem( IDC_BTN_DNPSET )->EnableWindow( TRUE );

			GetDlgItem( IDC_BTN_CONTXT )->EnableWindow( TRUE );
			GetDlgItem( IDC_BTN_CHGNAM )->EnableWindow( TRUE );
		}

		ListSelDataQuery(TRUE);
	}
}

// リストに定型伝票をセット
void CRDDnpDlg::DnpLabelSet()
{
	CString filter;
	int owner;
	if( p_DBDinpView->m_CmnId > 0 )		owner = p_DBDinpView->m_CmnId;
	else								owner = 0;

	filter.Format( "rdp_dtype = %d and rdp_owner = %d", m_Dtype, owner );

	Mkrd.dnpLabel->Requery( filter );
	int cnt = Mkrd.dnpLabel->GetRecordCount();

	m_List.ResetContent();

	if( cnt > 0 ) {
		int num = 1;
		do {
			CString	str;
			str.Format( "%3d.%s", num, Mkrd.dnpLabel->rdp_name );
			m_List.AddString( str );
			num++;
		} while( Mkrd.dnpLabel->MoveNext() == 0 );
	}
	else {
		GetDlgItem(IDC_BTN_DNPSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DNPDEL)->EnableWindow(FALSE);
	}
}


//定型伝票を使う
void CRDDnpDlg::OnBnClickedBtnDnpset()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int sel = m_List.GetCurSel();
	Mkrd.dnpLabel->SetAbsolutePosition( sel+1 );
	m_dnpDataID = Mkrd.dnpLabel->rdp_dataid;

	EndDialog( IDOK );
}

//定型伝票の削除
void CRDDnpDlg::OnBnClickedBtnDnpdel()
{
	ListSelDataQuery();

	CString	msg;
	msg.Format( "定型伝票『%s』を削除しますか？", Mkrd.dnpLabel->rdp_name );

	int st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );

	if( st == IDYES ) {

		CStRecordSet st_rec(pDBzm->m_database);
		CString tmp;
		tmp = Mkrd.dnpLabel->rdp_name;
		AdjustSQLsearch(tmp);

		int type = Mkrd.dnpLabel->rdp_dtype;
		int lbseq = Mkrd.dnpLabel->rdp_lbseq;

		int owner;
		if( p_DBDinpView->m_CmnId > 0 )		owner = p_DBDinpView->m_CmnId;
		else								owner = 0;

		st_rec.m_sql.Format("select count(*) from rdnplabel where rdp_name = '%s' COLLATE Japanese_CI_AS_KS_WS and rdp_dtype = %d and rdp_lbseq = %d and rdp_owner = %d", tmp, type, lbseq, owner);
		BOOL bEnaDel = TRUE;

		TRY{
			if( !st_rec.Open() ) {
				ICSMessageBox("定型伝票の名称チェックが行えません！", 0, 0, 0, this);
				return;
			}
			if( st_rec.m_st != 1 ) {
				ICSMessageBox("削除対象の定型伝票データがありません！", 0, 0, 0, this);
				bEnaDel = FALSE;
			}
			st_rec.Close();
		}
		CATCH(CMemoryException, me) {
			me->GetErrorMessage(tmp.GetBuffer(_MAX_PATH), _MAX_PATH);
			tmp.ReleaseBuffer();
			ICSMessageBox(tmp, 0, 0, 0, this);
			return;
		}
		CATCH(CDBException, de) {
			tmp = de->m_strError;
			ICSMessageBox(tmp, 0, 0, 0, this);
			return;
		}
		END_CATCH;

		if( bEnaDel ) {
			if( Mkrd.dnpData->st == 0 ) {
				int cnt = Mkrd.dnpData->GetRecordCount();

				for( int n = 0; n < cnt; n++ ) {
					Mkrd.dnpData->Delete();
				}
			}

			int seq = Mkrd.dnpLabel->rdp_lbseq;
			Mkrd.dnpLabel->Delete(seq);
		}

		DnpLabelSet();

		if( m_List.GetCount() > 0 ) {
			m_List.SetCurSel(0);
		}
		else {
			GetDlgItem( IDC_BTN_REGOVW )->EnableWindow( FALSE );
			GetDlgItem( IDC_BTN_CONTXT )->EnableWindow( FALSE );
			GetDlgItem( IDC_BTN_CHGNAM )->EnableWindow( FALSE );
		}
	}
}


void CRDDnpDlg::OnLbnDblclkList1()
{
	ListSelDataQuery();

	if( Mkrd.dnpData->st == 0 ) {

		if( m_Dtype == 0x30 ) {
			CRDnpFuriDlg	dlg(this);
			dlg.SetRDNPLabel( Mkrd.dnpLabel->rdp_name );
			dlg.DoModal();
		}
		else {
			CRDnpNSdlg	ndDlg(this);
			int type = (m_Dtype == 0x10) ? KIND_NYUKIN : KIND_SYUKKIN;
			ndDlg.SetDenpyoType( type );
			ndDlg.SetRDNPLabel( Mkrd.dnpLabel->rdp_name );
			ndDlg.DoModal();
		}
	}
}


int CRDDnpDlg::ListSelDataQuery( BOOL bDataNoquery/*=FALSE*/ )
{
	int abs_pos = 0;;
	
	int sel = m_List.GetCurSel();
	abs_pos = (sel + 1);

	Mkrd.dnpLabel->SetAbsolutePosition( abs_pos );

	if( ! bDataNoquery ) {
		CString filter;
		filter.Format( "rdp_dataid = %d", Mkrd.dnpLabel->rdp_dataid );

		Mkrd.dnpData->Requery( filter );
	}

	return abs_pos;
}


//上書き
void CRDDnpDlg::OnBnClickedBtnRegOVW()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ListSelDataQuery();

	CString sMsg;
	sMsg.Format( "現在、表示中の伝票を『%s』に上書登録しますか？", Mkrd.dnpLabel->rdp_name );

	int st = myICSMessageBox( sMsg, MB_YESNO|MB_DEFBUTTON2 );

	if( st == IDYES ) {
		p_DnpView->RDDnpUpdateJob( Mkrd.dnpLabel->rdp_name, m_Dtype, TRUE, FALSE );
	}
}

BOOL CRDDnpDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = CWnd::FromHandle( pMsg->hwnd );
		int id = pwnd->GetDlgCtrlID();

		if( id == IDC_LIST1 || id == IDC_CHK_ZEIRITSU ) {
			switch( pMsg->wParam ) {
			case VK_TAB:
				if( is_SHIFT() )	break;
				// else なら ↓
			case VK_RETURN:
				NextDlgCtrl();
				return TRUE;
				break;
			}
		}

		switch( pMsg->wParam ) {

		case VK_F2:
			PrevDlgCtrl();
			return TRUE;
			break;
		case VK_F9:
			KeyDownClickedJob(IDC_BTN_CHGNAM);
			break;
		case VK_F7:
			KeyDownClickedJob(IDC_BTN_REGDNP);
			break;
		case VK_F8:
			KeyDownClickedJob(IDC_BTN_REGOVW);
			break;
		case VK_DELETE:
			KeyDownClickedJob(IDC_BTN_DNPDEL);
			break;
		case VK_F12:
			KeyDownClickedJob(IDC_BTN_DNPSET);
			break;
		case VK_END:
			KeyDownClickedJob(IDCANCEL);
			return TRUE;
			break;
		case VK_F11:
			if( m_List.GetCount() > 0 ) {
				OnLbnDblclkList1();
			}
			return TRUE;
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}



void CRDDnpDlg::KeyDownClickedJob( int nID )
{
	if( GetDlgItem(nID)->IsWindowEnabled() ) {
		PostMessage(WM_COMMAND, MAKELONG(nID, BN_CLICKED), (LPARAM)GetDlgItem(nID)->m_hWnd);
	}
}


//名称変更
void CRDDnpDlg::OnBnClickedBtnChgNam()
{
	int abs_pos = ListSelDataQuery(TRUE);

	CRDDnpAddDlg	dlg( m_Dtype, TRUE, this );
	dlg.m_LabelName = Mkrd.dnpLabel->rdp_name;
	dlg.m_AbsPos	= abs_pos;

	if( dlg.DoModal() == IDOK ) {
		ListSelDataQuery(TRUE);
		int sel;
		sel = m_List.GetCurSel();
		if( sel == -1 )	sel = 0;

		DnpLabelSet();
		m_List.SetCurSel(sel);
	}
}



//内容表示
void CRDDnpDlg::OnBnClickedBtnContxt()
{

	PostMessage( WM_KEYDOWN, VK_F11 );
}



void CRDDnpDlg::OnBnClickedRdValchk()
{
	UpdateData();
	p_DnpView->RDDnpSetChkVal( m_chkVal );
}


void CRDDnpDlg::OnBnClickedChkZeiritsu()
{
	int st = m_Zritu.GetCheck();
	int chk_val = 1;

	if (st == BST_UNCHECKED)
		chk_val = 0;

	RDsiwakeZeirituMode() = chk_val;
}