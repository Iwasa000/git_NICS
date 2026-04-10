// DlgTkdic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "icslistinp.h"

#include "icsdbedt.h"

#include "DBInpSubFunc.h"

#include "TkdicDlg.h"
#include "ksrcstr.h"
#include "Swkseldlg.h"
#include "tksrchdlg.h"

#include "CStRecordSet.h"
#include "CTkyDelRepodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern
int SyzZeiritsuType( CDBZmSub* pZm );

/////////////////////////////////////////////////////////////////////////////

#define		KANA_LEN	4
#define		KANA_FMT	"%-4.4s"

//------------>摘要全削除対応2014.11.20
//摘要コード削除、変更時に使用
#define DELMODE		0	//削除モード
#define UPMODE		1	//更新モード
//<----------------------

#define EDIT_ON_FIRST 1
#define EDIT_IN		  2

#define KIND_A  ICS_EDS_ZENKAKU | ICS_EDS_KANA | ICS_EDS_ASCII | ICS_EDS_ALPHABET | ICS_EDS_NUMERIC


COLORREF	BKCOLOR = RGB(255, 255, 255);
COLORREF	BKCOLOR2 = RGB(255, 255, 240);
COLORREF	OFFCOLOR = RGB(0, 128, 0);
COLORREF	ONSETCOLOR = RGB(255, 255, 220);

CTkdicDlg::CTkdicDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CTkdicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTkdicDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	CBaseDlg::m_Xdl = 492;
	CBaseDlg::m_Ydl = 260;

	m_White.CreateSolidBrush( CL_WHITE );

	m_bInitOK = FALSE;

	m_NowIdx = m_NowClm = -1;
	m_InputSign = 0;
	m_SelDlgWnd = NULL;
	m_TkMax = 20000;
	m_InsIndex = -1;
	m_TkNumber = -1;
	insNewLine = -1;
	addNewLine = -1;

	::ZeroMemory( &m_Ctkrec, sizeof m_Ctkrec );
	::ZeroMemory( &m_Kana, sizeof m_Kana );

	m_SyzType = 5;
	m_bSyzInp = TRUE;

	m_iniz = 0;
	Tk_UseItem = 0;

	edit_keydown_sw = 0;

	post_row = -1;
	post_col = -1;

	pTkjour = NULL;
	if (pTkjour == NULL) {
		pTkjour = new CZIM_TkjourUtil();
	}	

	m_bInvMaster = false;
	m_bInvUse = false;
	m_bInvDisp = false;

	m_syzKubn = 0;

	::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
	m_bIgnoreFocus = FALSE;
	m_Menchk = 0;
}

CTkdicDlg::~CTkdicDlg()//('15.12.08)
{
	if (pTkjour) {

		pTkjour->End();
		delete pTkjour;
		pTkjour = NULL;
	}
}


void CTkdicDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTkdicDlg)
	//DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_listtkdic);
	DDX_Control(pDX, IDC_LEFTBUTTON, m_EBLeft);
	DDX_Control(pDX, IDC_RIGHTBUTTON, m_EBRight);
	DDX_Control(pDX, IDC_ADDBUTTON, m_EBAdd);
	DDX_Control(pDX, IDC_SUBBUTTON, m_EBSub);
	DDX_Control(pDX, IDC_HOMEBUTTON, m_EBHome);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_DBEkana);
	DDX_Control(pDX, IDC_ICSDBEDT_NUM, m_DBEnum);

	DDX_Control(pDX, IDC_BUTTON_TOROK, m_RBtorok);
	DDX_Control(pDX, IDC_BUTTON_DEBKMK, m_RBdebkmk);
	DDX_Control(pDX, IDC_BUTTON_CREKMK, m_RBcrekmk);
	DDX_Control(pDX, IDC_BUTTON_DEBCRE, m_RBdebcre);
	DDX_Control(pDX, IDC_BUTTON_SWK, m_RBswk);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT1, m_Edit);

	DDX_Control(pDX, IDC_REPORT_TKDIC, m_report_tkdic);
	DDX_Control(pDX, IDC_ICSDBEDT_INV, m_DBEinv);
	DDX_Control(pDX, IDC_COMBO_MEN, m_ComboMen);
}


// 仮想関数
// 画面表示
void CTkdicDlg::WndShow(BOOL bShow)
{
	CBaseDlg::WndShow( bShow );
	
	// 科目対応摘要ボタンのチェック
	TKY_REGPAR par = {0};
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	int cnt = get_tknum( m_pZm );

	// 摘要があり まだ追加登録できる時のみチェック
	if( par.tr_tky[0] != '\0' && cnt < m_TkMax ) {
		BOOL bDebBtn, bCreBtn;
		bDebBtn = bCreBtn = FALSE;

		DBKNREC* pKn;
		if( par.tr_debt[0] ) {
			pKn = DB_PjisToKnrec( par.tr_debt, m_pZm );

			if( pKn && pKn->knrtb > 0 )
				bDebBtn = TRUE;		
		}

		if( par.tr_cred[0] ) {
			pKn = DB_PjisToKnrec( par.tr_cred, m_pZm );

			if( pKn && pKn->knrtb > 0 )
				bCreBtn = TRUE;		
		}
		
		GetDlgItem(IDC_BUTTON_DEBKMK)->EnableWindow( bDebBtn );
		GetDlgItem(IDC_BUTTON_CREKMK)->EnableWindow( bCreBtn );

		GetDlgItem(IDC_BUTTON_DEBCRE)->EnableWindow( (bDebBtn || bCreBtn ) );

		// 借・貸 どちらの科目もない時は 仕訳摘要登録ボタンを不可
		if( (!par.tr_debt[0] && !par.tr_cred[0] ) ) {

			GetDlgItem(IDC_BUTTON_SWK)->EnableWindow( FALSE );
		}
	}
}

void CTkdicDlg::FocusMove( int direction ) 
{
}

void CTkdicDlg::SetFirstPosition() 
{
}


// 仮想関数
void CTkdicDlg::InitWork( double x, double y )
{
	CBaseDlg::InitWork( x, y );

	TorokDisp();
	CString strMsg;
	strMsg.Format( _T( "登録摘要指定（最大全角%s文字）"), m_TKYO_LEN == 20 ? "１０" : "２０" );
	GetDlgItem( IDC_STATIC1 )->SetWindowText( strMsg );

	// 貸借 の 科目表示
	CString debstr, crestr;	
	TKY_REGPAR par = {0};
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	if( par.tr_tky[0] != '\0' && 
		(par.tr_debt[0] != 0 || par.tr_cred[0] != 0) ) {

		DBKNREC* pKn;
		pKn = DB_PjisToKnrec( par.tr_debt, m_pZm );
		if( pKn )
			debstr.Format( "借 方『%.14s』", pKn->knnam );
		else
			debstr = ( "借 方『－－－－－－－』" );

		pKn = DB_PjisToKnrec( par.tr_cred, m_pZm );
		if( pKn )
			crestr.Format( "貸 方『%.14s』", pKn->knnam );
		else
			crestr = ( "貸 方『－－－－－－－』" );
	}
	else {
		debstr = "";	// 貸借 科目表示しない
		crestr = "";
	}

	GetDlgItem(IDC_STATIC_DT)->SetWindowText( debstr );
	GetDlgItem(IDC_STATIC_CT)->SetWindowText( crestr );

	if( m_Kana[0] != '\0' ) {
		VARIANT var;
		var.pbVal = (BYTE*)m_Kana;
		DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else {
		if( par.tr_tky[0] != '\0' ) {
			CRevConv rv;
			DWORD opt;
			opt = RVF_KATAKANA | RVF_HANKAKU;
			CString kana;

			int idx = 0;

			if( par.tr_pos < strlen( par.tr_tky ) )
				idx = par.tr_pos;

			char buf[128] = {0};
			strcpy_s( buf, sizeof buf, &par.tr_tky[ idx ] );

			if( rv.GetFurigana( opt, CString(buf), kana ) == 0 ) {
				kana_chk( &kana, kana.GetLength() );
				furikana_chk( m_Kana, sizeof m_Kana, kana );
				VARIANT var;
				var.pbVal = (BYTE*)m_Kana;
				DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
		}
	}

	if( m_bInvMaster ) {
		m_ComboMen.SetCurSel(m_Menchk);
	}

	//インボイス登録番号
	if( m_bInvDisp ) {
		VARIANT var;
		var.pbVal = (BYTE*)m_TkyInvno;

		CString str, strWork;
		if( m_TkyInvno[0] ) {
			if( m_TkyInvno[0] == 'T' ) {
				str = m_TkyInvno;
				strWork = m_TkyInvno;

				strWork.Replace("T", "");
				var.pbVal = (BYTE*)strWork.GetBuffer(128);
				strWork.ReleaseBuffer(-1);
			}
			else {
				str.Format(_T("T%s"), m_TkyInvno);
				var.pbVal = (BYTE*)m_TkyInvno;
			}
		}
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, str);
	}
	else {
		m_DBEinv.EnableWindow(FALSE);
		m_DBEinv.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC18)->ShowWindow(SW_HIDE);
		//免税を移動する
		CRect kanaRect, rect;
		m_DBEkana.GetWindowRect(&kanaRect);
		GetDlgItem(IDC_STATIC20)->GetWindowRect(&rect);

		if( m_bInvMaster && m_bInvUse ) {
			int nWd = rect.Width();
			rect.right = kanaRect.right;
			rect.left = rect.right - nWd;
			int nLeft = rect.right;
			ScreenToClient(rect);
			GetDlgItem(IDC_STATIC20)->MoveWindow(&rect);

			m_ComboMen.GetWindowRect(&rect);
			nWd = rect.Width();
			rect.left = nLeft;
			rect.right = rect.left + nWd;
			ScreenToClient(rect);
			m_ComboMen.MoveWindow(&rect);
		}
		else {
			GetDlgItem(IDC_STATIC20)->ShowWindow(SW_HIDE);
			m_ComboMen.ShowWindow(SW_HIDE);
			m_ComboMen.EnableWindow(FALSE);
		}
	}

	m_bInitOK = TRUE;
}

// 仮想関数
// ツールバー状態
BOOL CTkdicDlg::IsEnableToolBar( int nID )
{
	BOOL bEnable = FALSE;
	int id;
	CWnd* pwnd;
	pwnd = GetFocus();

	switch( nID ) {
	case ID_TLBAR_F8:
	case ID_TLBAR_F6:
		if( pwnd ) {
			id = pwnd->GetDlgCtrlID();
			if( id == IDC_ICSLISTINPCTRL1 || m_listtkdic.IsChild( pwnd ) ) 
				bEnable = TRUE;
		}
		break;
		break;
	default:
		break;

	}

	return bEnable;
}


// 登録数表示
void CTkdicDlg::TorokDisp()
{
	int cnt = get_tknum( m_pZm );

	if( cnt >= m_TkMax ) {
		GetDlgItem(IDC_BUTTON_TOROK)->EnableWindow(FALSE);
	//	m_DspMax.ShowWindow( SW_SHOW );

		GetDlgItem(IDC_BUTTON_DEBKMK)->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_CREKMK)->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_DEBCRE)->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_SWK)->EnableWindow( FALSE );

		GetDlgItem(IDC_STATIC_DT)->EnableWindow( FALSE );
		GetDlgItem(IDC_STATIC_CT)->EnableWindow( FALSE );
	}
	else {
		// 入力文字数が 0 のときは 登録不可
		if( !m_Edit.IsWindowEnabled() ) {
			GetDlgItem(IDC_BUTTON_TOROK)->EnableWindow(FALSE);
			GetDlgItem(IDC_FURIBUTTON)->EnableWindow(FALSE);
		}
	
	//	m_DspMax.ShowWindow( SW_HIDE );
	}

	char bf[64];

	sprintf_s( bf, sizeof bf, "%5d", cnt );
	GetDlgItem(IDC_STDSP_RGCNT)->SetWindowText( bf );
	sprintf_s( bf, sizeof bf, "%5d", m_TkMax );		
	GetDlgItem(IDC_STDSP_RGMAX)->SetWindowText( bf );
}



BEGIN_MESSAGE_MAP(CTkdicDlg, CBaseDlg)
	//{{AFX_MSG_MAP(CTkdicDlg)
	ON_BN_CLICKED(IDC_HOMEBUTTON, OnHomebutton)
	ON_BN_CLICKED(IDC_LEFTBUTTON, OnLeftbutton)
	ON_BN_CLICKED(IDC_RIGHTBUTTON, OnRightbutton)
	ON_BN_CLICKED(IDC_SUBBUTTON, OnSubbutton)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TOROK, OnButtonTorok)
	ON_BN_CLICKED(IDC_BUTTON_DEBKMK, OnButtonDebkmk)
	ON_BN_CLICKED(IDC_BUTTON_CREKMK, OnButtonCrekmk)
	ON_BN_CLICKED(IDC_BUTTON_DEBCRE, OnButtonDebcre)
	ON_BN_CLICKED(IDC_BUTTON_SWK, OnButtonSwk)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_TKDICMESSAGE, OnTKDICMessage )
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus )
	ON_MESSAGE( WM_USER_1, OnUser_1 )
	ON_MESSAGE(WM_FONT_SIZE, FontSize)

	ON_MESSAGE(WM_MYMESSAGE, OnMyMessage)
	//WM_MYMESSAGE 使用済みだったので、WM_INVNO_MESSAGE 作成
	ON_MESSAGE(WM_INVNO_MESSAGE, OnInvnoMessage)

	ON_BN_CLICKED(IDC_FURIBUTTON, OnButtonFuri)

	ON_NOTIFY(ICS_NM_REPORT_EDITSETFOCUS, IDC_REPORT_TKDIC, OnNMReportEditsetfocusREPORT_TKDIC)

	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_REPORT_TKDIC, OnNMReportEditkeydownREPORT_TKDIC)
	//ON_NOTIFY(ICS_NM_REPORT_EDITSETFOCUS, IDC_REPORT_TKDIC, OnNMReportEditsetfocusREPORT_TKDIC)
	ON_NOTIFY(ICS_NM_REPORT_EDITIMEEND, IDC_REPORT_TKDIC, OnNMReportEditimeendREPORT_TKDIC)
	ON_NOTIFY(ICS_NM_REPORT_EDITKILLFOCUS, IDC_REPORT_TKDIC, OnNMReportEditkillfocusREPORT_TKDIC)

	// インボイス対応
	// 免税事業者からの課税仕入れサイン
	ON_MESSAGE( WM_MENZEI_MESSAGE, OnMenzeiMessage )

	// 仕入/売上区分
	ON_MESSAGE( WM_KUBUN_MESSAGE, OnKubunMessage )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTkdicDlg メッセージ ハンドラ

BOOL CTkdicDlg::OnInitDialog() 
{
	// インボイス対応
	CZmGen8 zmGen;

	// バージョンアップマスター判定
	m_bInvMaster = zmGen.IsInvoiceVupMaster( m_pZm );
	if( m_bInvMaster ) {
		// インボイス期間か
		m_bInvUse = zmGen.IsAbleToUseInvoice( m_pZm );
		// 登録番号を表示するか
		m_bInvDisp = zmGen.IsDispInvno( m_pZm );

		// 課税方式取得
		m_syzKubn = CheckSyzKubun( m_pZm->zvol );
	}
	else {
		m_bInvUse = false;
		m_bInvDisp = false;
		m_syzKubn = 0;
	}

	syzInvoice.SetDBZmSub( m_pZm );


	int len = GetTekiyoLength( m_pZm );
	if( len > 0 )	m_TKYO_LEN = len;

	m_SyzType = SyzZeiritsuType( m_pZm );
	//科目設定の摘要消費税設定サインをチェック
	KamokuOwnTblCheck();

	MakeTKdicFrm();

	CBaseDlg::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	UINT btnID[] = {
		IDC_LEFTBUTTON,IDC_RIGHTBUTTON,IDC_ADDBUTTON,IDC_SUBBUTTON,IDC_HOMEBUTTON
	};

	TKY_REGPAR par = {0};
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	// コントロール初期化
	//m_listtkdic.EnableDelete(0);
	m_DBEkana.EnableDelete(0);
	m_DBEnum.EnableDelete(0);
	m_DBEinv.EnableDelete(0);

	VARIANT var;
	var.pbVal = NULL;
	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	TKY_VER2* ptv = get_tkyver2();

	if( par.tr_tky[0] != '\0' ) {
		int sel1 = LOWORD( ptv->t2_sel );
		int sel2 = HIWORD( ptv->t2_sel );

		int tky_pos = par.tr_pos;
		char tkyTmp[128] = { 0 };

		if(	m_svTky.GetLength() ) {
			strcpy_s(tkyTmp, sizeof tkyTmp, m_svTky);
			sel1 = 0; 
			sel2 = sel1 + m_svTky.GetLength();
			tky_pos = 0;
		}
		else {
			strcpy_s(tkyTmp, sizeof tkyTmp, ptv->t2_tky);
			CString stmp = ptv->t2_tky;
			int idx = stmp.Find(m_svTky);
			if( idx != -1 ) {
				tky_pos = idx;
				sel1 = idx;
				sel2 = sel1 + m_svTky.GetLength();
			}
		}

		// エディット 初期化
		m_Edit.OnInitial(tky_pos, tkyTmp, m_TKYO_LEN, sel1, sel2 );
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
		GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE); // 文字数表示 static
		m_Edit.EnableWindow(FALSE);
		((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDTCTRL1))->SetEnabled(FALSE);

		GetDlgItem(IDC_BUTTON_DEBKMK)->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_CREKMK)->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_DEBCRE)->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_SWK)->EnableWindow( FALSE );

	}

	m_DBEkana.ImeEndMode( FALSE );
#ifndef _DBEDT_FRAME_
	m_DBEkana.SetFrame( -1, RGB_DARK_GRAY );
	m_DBEnum.SetFrame( -1, RGB_DARK_GRAY );
	m_DBEinv.SetFrame(-1, RGB_DARK_GRAY);
#endif

#ifdef TEST_CLOSE
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FURIBUTTON)->ShowWindow(SW_HIDE);
	m_DBEkana.SetEnabled(FALSE);
	m_DBEkana.ShowWindow(SW_HIDE);
#endif

	//追加用摘要コードを セットする
	tkdb_init( m_pZm );	
	m_pZm->tkrec->MoveLast();
	int cnt = m_pZm->tkrec->GetRecordCount();

	char bf[64];

	if( cnt <= 0 )	m_Tkno = 0;
	else			m_Tkno = m_pZm->tkrec->tkcod;

	if( cnt < m_TkMax ) {
		if( (m_Tkno+1) < 999999 )	// 6桁まで入力
			m_Tkno++;
		else {
			m_pZm->tkrec->MovePrev();
			int tknum = m_Tkno;
			if( m_pZm->tkrec->st != -1 ) {
				do {
					if( (tknum - m_pZm->tkrec->tkcod) > 1 ) {
						m_Tkno = (m_pZm->tkrec->tkcod + 1);
						break;
					}
					tknum = m_pZm->tkrec->tkcod;
				} while( m_pZm->tkrec->MovePrev() == 0 );
			}
		}
	}
	sprintf_s( bf, sizeof bf, "%d", m_Tkno );

	var.pbVal = (BYTE*)bf;
	DBdata_set( this, IDC_ICSDBEDT_NUM, &var, ICSDBEDT_TYPE_STRING, 0 );

	SetItemListTkyo();

	if( m_bInvMaster ) {
		m_ComboMen.SetCurSel(m_Menchk);
	}

	//インボイス登録番号
	if( m_bInvDisp ) {
		var.pbVal = (BYTE*)m_TkyInvno;
		CString str, strWork;
		str.Empty();
		if( m_TkyInvno[0] ) {
			if( m_TkyInvno[0] == 'T' ) {
				str = m_TkyInvno;
				strWork = m_TkyInvno;

				strWork.Replace("T", "");
				var.pbVal = (BYTE*)strWork.GetBuffer(128);
				strWork.ReleaseBuffer(-1);
			}
			else {
				str.Format(_T("T%s"), m_TkyInvno);
				var.pbVal = (BYTE*)m_TkyInvno;
			}
		}
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, str);
	}
	else {
		m_DBEinv.EnableWindow(FALSE);
		m_DBEinv.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC18)->ShowWindow(SW_HIDE);
		//免税を移動する
		CRect kanaRect, rect;
		m_DBEkana.GetWindowRect(&kanaRect);
		GetDlgItem(IDC_STATIC20)->GetWindowRect(&rect);

		if( m_bInvMaster && m_bInvUse ) {
			int nWd = rect.Width();
			rect.right = kanaRect.right;
			rect.left = rect.right - nWd;
			int nLeft = rect.right;
			ScreenToClient(rect);
			GetDlgItem(IDC_STATIC20)->MoveWindow(&rect);

			m_ComboMen.GetWindowRect(&rect);
			nWd = rect.Width();
			rect.left = nLeft;
			rect.right = rect.left + nWd;
			ScreenToClient(rect);
			m_ComboMen.MoveWindow(&rect);
		}
		else {
			GetDlgItem(IDC_STATIC20)->ShowWindow(SW_HIDE);
			m_ComboMen.ShowWindow(SW_HIDE);
			m_ComboMen.EnableWindow(FALSE);
		}
	}

	m_bListTermOK = TRUE;
	
	ICSDialog::OnInitDialogEX();

	PostMessage(WM_FONT_SIZE,0, 0); //OnSizeで行うため削除


	m_iniz = 1;

	ICS_TKJOURUTIL_PAC paket;

	ZeroMemory(&paket, sizeof(ICS_TKJOURUTIL_PAC));
	paket.zmsub = m_pZm;
	paket.CallFlg = 5;


	pTkjour->Init(paket);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


LRESULT CTkdicDlg::FontSize(WPARAM wParam, LPARAM lParam)
{
	LOGFONT		lf;
	CFont *pFont = GetDlgItem(IDC_BUTTON_TOROK)->GetFont();
	pFont->GetLogFont(&lf);
	m_report_tkdic.SetTextFont(lf);
	m_report_tkdic.SetHeaderFont(lf);
	return 1;

}


/*
//	表形式入力　設定bk
void CTkdicDlg::MakeTKdicFrm()
{
	LINP_COLUMN LC[8];
	LINP_VCOLORINFO	LVCOL[8];
	int i;

	//　番号
	LC[0].clm_kind = LINP_KIND_NUMERIC;
	LC[0].clm_max = 6;
	LC[0].clm_attr = LINP_ATTR_CENTER| LINP_ATTR_BOTTOM;
	LC[0].clm_format = NUM_FMT;
//	LC[0].clm_width = 30;
	LC[0].clm_width = 35;
	LC[0].clm_title = "番号";
	LC[0].clm_tbcolor = -1;
	LC[0].clm_ttcolor = -1;

	// 摘要名称
	LC[1].clm_kind = LINP_KIND_STRING;
	LC[1].clm_max = (m_TKYO_LEN/2);
	LC[1].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_CMODE;
	LC[1].clm_attr |= LINP_ATTR_ZENHAN;

	LC[1].clm_width = 172; //140
	LC[1].clm_title = "　摘要名称";
	LC[1].clm_tbcolor = -1;
	LC[1].clm_ttcolor = -1;

	// カナ
	LC[2].clm_kind = LINP_KIND_STRING;
	LC[2].clm_max = KANA_LEN;
	LC[2].clm_attr = LINP_ATTR_ANK | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	LC[2].clm_width = 22; //25 <- 40
	LC[2].clm_title = "　カナ";
	LC[2].clm_tbcolor = -1;
	LC[2].clm_ttcolor = -1;
	// 消費税
	LC[3].clm_kind = LINP_KIND_STRING;
	LC[3].clm_max = 6;
	LC[3].clm_attr = LINP_ATTR_ANK | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	if( !m_bSyzInp )	LC[3].clm_attr |= LINP_ATTR_READONLY;

	LC[3].clm_width = 46; //50 <- 60

	LC[3].clm_title = "消費税";
	LC[3].clm_tbcolor = -1;
	LC[3].clm_ttcolor = -1;

	// 自動仕訳なしのコントロール
//	m_listtkdic.SetColumn( 4, (LPUNKNOWN)LC );
	linp_setclm( IDC_ICSLISTINPCTRL1, 4, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
//	m_listtkdic.SetHorzGrid( 5, CL_RED, LINP_GRID_SOLID );
	linp_horzgrid( IDC_ICSLISTINPCTRL1, 5, CL_RED, LINP_GRID_SOLID, CICSListInp );

	for( i=0; i < 4; i++ ) {
		LVCOL[i].pos = i;
		LVCOL[i].color = CL_GREEN;
		LVCOL[i].type = LINP_GRID_SOLID;
	}
//	m_listtkdic.SetVartGrid( 4, (LPUNKNOWN)LVCOL );
	linp_vartgrid( IDC_ICSLISTINPCTRL1, 4, (LPUNKNOWN)LVCOL, CICSListInp );
}

*/

//	表形式入力　設定
void CTkdicDlg::MakeTKdicFrm(){

	//CRect	m_Report;
	ICSReportRecords	*pRecords = m_report_tkdic.GetRecords();
	//m_report_tkdic.GetWindowRect(m_Report);
	pRecords->RemoveAll();

//	ICSReportRecordItem*	pItem;

	m_report_tkdic.RemoveColumn(-1);

//	m_ProgressCtrl.SetRange( 1, cnt );

	/*
	LOGFONT		lf;
	CFont *pFont = GetDlgItem(IDC_BUTTON_TOROK)->GetFont();
	pFont->GetLogFont(&lf);
	m_report_tkdic.SetTextFont(lf);
	m_report_tkdic.SetHeaderFont(lf);

	*/
	
	CString strTitle;
	
	m_report_tkdic.AddColumn(new ICSReportColumn(0, _T("番号"), 140,TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_RIGHT));
	m_report_tkdic.AddColumn(new ICSReportColumn(1, _T("摘要名称"), 510,TRUE , ICS_REPORT_NOICON, FALSE, TRUE, DT_LEFT));
	m_report_tkdic.AddColumn(new ICSReportColumn(2, _T("カナ"), 140, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));

	//m_report_tkdic.AddColumn(new ICSReportColumn(3, _T("消費税"), 200, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
	if(( m_syzKubn == 2 ) || ( m_syzKubn == 3 )) {
		if( m_syzKubn == 2 ) {
			strTitle.Format( _T( "仕入区分" ));
		}
		else {
			strTitle.Format( _T( "売上区分" ));
		}
		m_report_tkdic.AddColumn( new ICSReportColumn( TK_KUBUN, strTitle,  200, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER ));
	}
	else {
		strTitle.Empty();
		m_report_tkdic.AddColumn( new ICSReportColumn( TK_KUBUN, strTitle, -1, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER ));
	}

	m_report_tkdic.AddColumn( new ICSReportColumn( TK_SYOHI, _T( "消費税" ), 200, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER ));

	// インボイス対応
	if( m_bInvMaster && m_bInvUse ) {
		if( m_bInvDisp ) {
			//　登録番号
			m_report_tkdic.AddColumn( new ICSReportColumn( TK_INVNO, _T( "登録番号" ), 275 , TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_LEFT ));
		}
		else {
			//　登録番号
			m_report_tkdic.AddColumn( new ICSReportColumn( TK_INVNO, _T( "登録番号" ), -1 , FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_LEFT ));
		}

		// 免税事業者からの課税仕入れ
		m_report_tkdic.AddColumn( new ICSReportColumn( TK_MENZEI, _T( "免税" ), 95 , TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_LEFT ));
	}

	return;
}

// カナ検索時カーソル位置を指定 
void CTkdicDlg::SetItemStateTkyo(int tkcod /*= 0*/, int clm /*=TK_CODE*/)

{
	int		i, cnt, pos_idx;


	m_pZm->tkrec->MoveFirst();
	i = 0;
	pos_idx = -1;	// 12.12 /11

	if (m_pZm->tkrec->st != -1) {
		do {
			//MakeTkyoLine(m_pZm->tkrec);
			//		m_ProgressCtrl.SetPos( i+1 );

			if (tkcod > 0 && tkcod == m_pZm->tkrec->tkcod) {
				pos_idx = i;
			}
			i++;
		} while (m_pZm->tkrec->MoveNext() == 0);
	}

	if (pos_idx >= 0) {	// 12.12 /11
		//m_listtkdic.SetInputPositionEX(pos_idx, clm);
		// スクロール
		int cnt = get_tknum(m_pZm);
		if( (pos_idx + 1) == cnt ) {
			// 最下行の場合、消費税カラムをクリックしても反応しない 162240
			// マウスホイールの下方向をソフト的に実行し、リポートをスクロールさせる。
			CPoint nowPt;
			::GetCursorPos(&nowPt);
			CRect rect;
			m_report_tkdic.GetWindowRect(&rect);
			int x = (rect.left + rect.Width() / 2);
			int y = (rect.top + rect.Height() / 2);
			//	'実際にマウスを移動させる為の補正値を求める
			//	'画面の解像度を取得
			int smx, smy, posx, posy;
			//	'画面解像度の取得
			smx = GetSystemMetrics(SM_CXSCREEN);
			smy = GetSystemMetrics(SM_CYSCREEN);
			//	'マウスの移動量を計算(絶対位置)
			posx = x * (65535 / smx);
			posy = y * (65535 / smy);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
			mouse_event(MOUSEEVENTF_WHEEL, 0, 0, (DWORD)-256, 0);

			//元の位置に戻す
			posx = nowPt.x * (65535 / smx);
			posy = nowPt.y * (65535 / smy);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
		}

		m_report_tkdic.EditItem(pos_idx, clm, TRUE);
		// カレントレコードセット
		///Current_Recset(pos_idx);
	}
}


void CTkdicDlg::SetReportRecord(ICSReportRecord* pRecord)
{
	CString str_set_f;
	str_set_f.Empty();
	int digit = 40; // 最大入力バイト数
	ICSReportRecordItem*	pItem;
	

	//番号
	pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, ICS_EDS_NUMERIC, TRUE, FALSE, 6, ICS_EDS_CENTER, FALSE));
	//摘要名称
	pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, KIND_A /*ICS_EDS_ZENKAKU*/, TRUE, TRUE, digit, ICS_EDS_LEFT, FALSE));
	//カナ
	pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, ICS_EDS_KANA, TRUE, FALSE, 4, ICS_EDS_CENTER, FALSE));
	// 仕入/売上区分
	pItem = pRecord->AddItem( new ICSReportRecordItemText( str_set_f, ( ICS_EDS_ZENKAKU | ICS_EDS_KANA ), TRUE, FALSE, digit, ICS_EDS_LEFT, FALSE ));
	//消費税
	pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, ICS_EDS_ZENKAKU | ICS_EDS_KANA, TRUE, FALSE, digit, ICS_EDS_LEFT, FALSE));

	// インボイス対応
	if( m_bInvMaster && m_bInvUse ) {
		// 登録番号
		pItem = pRecord->AddItem( new ICSReportRecordItemText( str_set_f, ICS_EDS_NUMERIC, TRUE, FALSE, 14, ICS_EDS_CENTER, FALSE ));
		
		// 免税事業者からの課税仕入れ
		pItem = pRecord->AddItem( new ICSReportRecordItemText( str_set_f, ICS_EDS_NUMERIC, TRUE, FALSE, 6, ICS_EDS_CENTER, FALSE ));
	}

}

// レポートコントロールに 摘要セット
void CTkdicDlg::SetItemListTkyo(int tkcod /*= 0*/, int clm /*=TK_CODE*/)
{
	TorokDisp();
	ICSReportRecordItem*	pItem;
	
	ICSReportRecords	*pRecords = m_report_tkdic.GetRecords();
	
	pRecords->RemoveAll();
	
	tkdb_init(m_pZm);


	int maxbyte = 40; // 最大入力バイト数
	CString str_set_f;
	str_set_f.Empty();

	insNewLine = -1;
	int cnt = get_tknum(m_pZm);

	MakeTkyoLine(cnt);//摘要辞書登録されている摘要データを作成

	for (map<long, CTKDATA_REC>::iterator it = m_TbCust.begin(); it != m_TbCust.end(); it++) {

		ICSReportRecord*	pRecord = m_report_tkdic.AddRecord(new ICSReportRecord());

		//番号
		str_set_f.Format("%6d", it->second.tkcod);
		pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, ICS_EDS_NUMERIC, TRUE, FALSE, 6, ICS_EDS_CENTER, FALSE));
		//摘要名称
		str_set_f.Format("%s", it->second.tkname);
		pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, KIND_A, FALSE, TRUE, maxbyte, ICS_EDS_LEFT, FALSE));
		//カナ
		str_set_f.Format("%-4.4s", it->second.tkana);
		pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, ICS_EDS_KANA, TRUE, TRUE, 4, ICS_EDS_CENTER, FALSE));

		// 仕入/売上区分
		if(( m_syzKubn == 2 ) || ( m_syzKubn == 3  )) {
			str_set_f.Format( _T( "%s" ), it->second.dsign );
		}
		else {
			str_set_f.Empty();
		}
		pItem = pRecord->AddItem( new ICSReportRecordItemText( str_set_f, ( ICS_EDS_ZENKAKU | ICS_EDS_KANA ), TRUE, FALSE, 40, ICS_EDS_LEFT, FALSE ));

		str_set_f.Format("%s", it->second.tksgn);
		pItem = pRecord->AddItem(new ICSReportRecordItemText(str_set_f, ICS_EDS_ZENKAKU | ICS_EDS_KANA, TRUE, FALSE, maxbyte, ICS_EDS_LEFT, FALSE));

		// インボイス対応
		if( m_bInvMaster && m_bInvUse ) {
			// 登録番号
			str_set_f.Format( _T( "%s" ), it->second.invno );
			pItem = pRecord->AddItem( new ICSReportRecordItemText( str_set_f, ICS_EDS_NUMERIC, TRUE, FALSE, 13, ICS_EDS_CENTER, FALSE ));

			// 免税事業者からの課税仕入れ
			str_set_f.Format( _T( "%s" ), it->second.taxfree );
			pItem = pRecord->AddItem( new ICSReportRecordItemText( str_set_f, ICS_EDS_NUMERIC, TRUE, FALSE, 6, ICS_EDS_CENTER, FALSE ));
		}


		// 新規追加行
		//if( cnt < m_TkMax ) {	
		//	MakeTkyoLine( NULL );
		//}
	}

	//最終行
	ICSReportRecord*	pRecord = m_report_tkdic.AddRecord(new ICSReportRecord());
	SetReportRecord(pRecord);
	addNewLine = m_report_tkdic.GetRecords()->GetCount() - 1;

	//	垂直・水平グリッド
	COLORREF	m_colorRed, m_colorRBlack, m_colorRwhite, m_colorBack, m_colorLINE, m_colorLINE2;
	m_colorRed = RGB(255, 0, 0);
	m_colorRwhite = RGB(255, 255, 255);
	m_colorRBlack = RGB(0, 0, 0);
	m_colorBack = RGB(204, 255, 204);
	//	m_colorLINE		=	RGB(210,255,210);
	m_colorLINE = RGB(0, 128, 0);
	m_colorLINE2 = RGB(221, 255, 255);
	ICSReportGridStyle	m_styleHorPer;
	m_styleHorPer = (ICSReportGridStyle)4;	// ４：直線

	m_report_tkdic.SetGridPerCount(FALSE, 1, m_colorLINE2, m_styleHorPer);
	m_report_tkdic.SetGridPerCount(FALSE, 5, m_colorRed, m_styleHorPer);
	m_report_tkdic.SetGridPerCount(TRUE, 1, m_colorLINE, m_styleHorPer);

	int ht =25;
	int itemH = 16;

	m_report_tkdic.SetHeaderHeight(ht);

	m_report_tkdic.SetItemHeight(itemH + (itemH / 2));
	
	if (!(m_pZm->zvol->tl_cor & 0x10)) {
		m_report_tkdic.AllowEdit(FALSE);
	}
	else {
		m_report_tkdic.AllowEdit(TRUE);
	}

	TKY_REGPAR par = {0};
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	m_report_tkdic.SetDraggable(FALSE);
	m_report_tkdic.EnableResetColumnWidth(0);


	//	更新
	SetMapColor();
	m_report_tkdic.Populate();

	SetItemStateTkyo(tkcod, clm); // カーソル位置を指定する。
	return;
	
}



void CTkdicDlg::MakeTkyoLine(int entry_cnt) {


	//CArray < ICS_TKJOUR_PAC, ICS_TKJOUR_PAC& > tkjourAry;
	
	//int result = 0;

	int tkari_sw = 0;
	int tkasi_sw = 0;
	int nBuf = 0;
	CString Symsg;

	m_TbCust.clear();//再描画する為にマップデータ削除2015.01.13

	m_pZm->tkrec->Requery("", 0);
	m_pZm->tkrec->MoveFirst();

	int test = sizeof(CTKDATA_REC);

	
	for (int i = 0; i < entry_cnt; i++) {
		CTKDATA_REC tkdata_rec;

		// 初期化
		map_struct_clear( &tkdata_rec );

		tkdata_rec.tkcod = m_pZm->tkrec->tkcod;
		tkdata_rec.tkname.Format("%s", m_pZm->tkrec->tkname);

		tkdata_rec.tkana.Format("%s", m_pZm->tkrec->tkana);
		
		skbn_msg(Symsg, m_pZm->tkrec->tksgn.GetAt(1), m_pZm, m_pSy);
		tkdata_rec.tksgn.Format("%s", Symsg);

		//tkjourAry.RemoveAll();
		//result = pTkjour->SelectALLTkjour(m_pZm->tkrec->tkcod, tkjourAry);

		// インボイス対応
		tkdata_rec.invno = m_pZm->tkrec->invno;	// 登録番号

		// 免税事業者からの課税仕入れ
		if( m_pZm->tkrec->dsign.GetSize() != SIZE_DSIGN ) {
			m_pZm->tkrec->dsign.SetSize( SIZE_DSIGN );

			m_pZm->tkrec->Edit();
			for (int idx = 0; idx < SIZE_DSIGN; idx++) {
				m_pZm->tkrec->dsign.SetAt( idx, 0 );
			}
			m_pZm->tkrec->Update();
		}

		if( m_pZm->tkrec->dsign[4] & BIT_MENZEI ) {
			tkdata_rec.taxfree = _T("○");
		}
		else {
			tkdata_rec.taxfree = _T("");
		}

		//
		int selno;
		if( m_syzKubn == 2 ) {
			selno = m_pZm->tkrec->dsign.GetAt(4) & 0x0f;
			SkbnMsg(tkdata_rec.dsign, selno, m_syzKubn, 1);
		}
		else if( m_syzKubn == 3 ) {
			selno = m_pZm->tkrec->dsign.GetAt(3);
			SkbnMsg(tkdata_rec.dsign, selno, m_syzKubn, 1);
		}

		
		m_TbCust.insert(pair<long, CTKDATA_REC>(m_pZm->tkrec->tkcod, tkdata_rec));
		if (m_pZm->tkrec->MoveNext() == -1) break;
	}
}


// 一行データ 作成

/*
void CTkdicDlg::MakeTkyoLine( CTKREC * tkrec )
{

	LINP_DATA	LD[10];
	COLORREF	ONCOLOR, OFFCOLOR, SY_ONCOLOR, SY_OFFCOLOR;
	char	pbuf[64];
	int		r, tkno;
	CString Symsg, Data;

	ONCOLOR = CL_BLACK;
	OFFCOLOR = CL_TKBACK;

	SY_ONCOLOR = ONCOLOR;
	SY_OFFCOLOR = OFFCOLOR;

	// 摘要コード
	if( tkrec != NULL )	tkno = tkrec->tkcod;
	else				tkno = 0;

	LD[0].dat_attr = LINP_DISP_COLOR;
	LD[0].dat_fc = ONCOLOR;
	LD[0].dat_bc = OFFCOLOR;

	l_defn(0x16);
	sprintf_s( pbuf, sizeof pbuf, "%6d", tkno );
	l_input( LD[0].dat_numeric, pbuf );
//	LD[0].dat_string.Format( "%4d", tkno );
	LD[0].dat_initflg = tkno ? FALSE : TRUE;

	// 摘要名称
	memset( pbuf, 0, sizeof pbuf );
	if( tkrec != NULL ) {
		r = kjlen( (void*)(LPCTSTR)tkrec->tkname, m_TKYO_LEN );
		memmove( pbuf, tkrec->tkname, r );
	}

	LD[1].dat_attr = LINP_DISP_COLOR;
	LD[1].dat_fc = ONCOLOR;
	LD[1].dat_bc = OFFCOLOR;
	LD[1].dat_string.Format( "%s", pbuf );
	LD[1].dat_initflg = tkrec ? FALSE : TRUE;

	// カナ50音
	LD[2].dat_attr = LINP_DISP_COLOR;
	LD[2].dat_fc = ONCOLOR;
	LD[2].dat_bc = OFFCOLOR;
	if( tkrec != NULL )	LD[2].dat_string.Format( KANA_FMT, tkrec->tkana );
	else				LD[2].dat_string = "";
	LD[2].dat_initflg = tkrec ? FALSE : TRUE;

	// 消費税
	if( tkrec != NULL )	skbn_msg( Symsg, tkrec->tksgn[1], m_pZm, m_pSy );
	else				Symsg = "";

	LD[3].dat_attr = LINP_DISP_COLOR;
	LD[3].dat_fc = SY_ONCOLOR;
	LD[3].dat_bc = m_bSyzInp ? SY_OFFCOLOR : CL_GRAY;
	LD[3].dat_string = Symsg;
	LD[3].dat_initflg = tkrec ? FALSE : TRUE;

//	m_listtkdic.SetData( tkno-1, (LPUNKNOWN)LD, FALSE );

	m_listtkdic.AddData( (LPUNKNOWN)LD, FALSE );
}


*/

void CTkdicDlg::OnLeftbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.LeftFunc();	
}

void CTkdicDlg::OnRightbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.RightFunc();	
}

void CTkdicDlg::OnAddbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.AddFunc();	
}

void CTkdicDlg::OnSubbutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.SubFunc();	
}

void CTkdicDlg::OnHomebutton() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_Edit.HomeFunc();	
}


HBRUSH CTkdicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください
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
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

// 摘要登録リスト状態を クリアする
void CTkdicDlg::ListInsertClear() 
{
	if( m_InsIndex >= 0 ) {
		m_listtkdic.RemoveData( m_InsIndex );
		m_InsIndex = -1;
	}
}


void CTkdicDlg::OnButtonTorok() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	tkdb_init( m_pZm );

	int tkcod, st;
	tkcod = m_Tkno;
	if( ! tkcod ) return;

	char teki[60] = {0};
	m_Edit.get_dictky( teki, sizeof teki );

	ListInsertClear();

	get_kanadata();

	// 同一の摘要があるか
	m_TkNumber = -1;
	int ret = SrchTKname( teki, &m_TkNumber, FALSE );

	//インボイス番号チェック
	if( !CheckInvoiceNo() )
		return;

	char pbuf[128];
	CString checkInvno;
	int ncnt = 0;
	if( m_bInvDisp ) {
		ncnt = GetInvnoString(pbuf, sizeof pbuf);
		if( ncnt == 13 ) {
			checkInvno.Format(_T("T%s"), pbuf);
		}
	}
	BYTE menchk = 0;
	if( m_bInvMaster ) {
		menchk = m_ComboMen.GetCurSel();
	}

	if( ret == CKsrcStr::EV_ADD || ! ret ) {
		if( ! tkyngetr( tkcod, m_pZm) ) {
			st = ICSMessageBox("指定された番号は登録されています。\r\n摘要の置換をしますか？", MB_YESNO, 0,0,this );
			if( st == IDYES ) {
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname = teki;
				m_pZm->tkrec->tkana = m_Kana;
				if( m_bInvDisp ) {
					m_pZm->tkrec->invno = checkInvno;
				}
				if( m_bInvMaster ) {
					if( menchk ) {
						m_pZm->tkrec->dsign[4] |= BIT_MENZEI;
					}
					else {
						m_pZm->tkrec->dsign[4] &= ~BIT_MENZEI;
					}
				}
				m_pZm->tkrec->Update();
				m_bDBupdate = TRUE;

				m_pZm->tkrec->Requery( "", 0 );
				PostMessage( WM_USER_1, 0, tkcod );
			}
		}
		else if( m_pZm->tkrec->GetRecordCount() < m_TkMax ) 
		{
			// 新規に追加
			m_pZm->tkrec->AddNew();
			m_pZm->tkrec->tkcod = tkcod;
			m_pZm->tkrec->tkname = teki;
			m_pZm->tkrec->tkana = m_Kana;
			m_pZm->tkrec->dsign.SetSize( SIZE_DSIGN );
			for( int idx = 0; idx < SIZE_DSIGN; idx++ ) {
				m_pZm->tkrec->dsign.SetAt( idx, 0 );
			}
			if( m_bInvDisp ) {
				m_pZm->tkrec->invno = checkInvno;
			}
			if( m_bInvMaster ) {
				if( menchk ) {
					m_pZm->tkrec->dsign[4] = BIT_MENZEI;
				}
			}
			m_pZm->tkrec->Update();
			m_bDBupdate = TRUE;

			m_pZm->tkrec->Requery( "", 0 );

			// リスト再表示
			PostMessage( WM_USER_1, 0, tkcod );
		}		
		else {
			st = ICSMessageBox("最大登録数まで登録されいるため登録できません！",0,0,0,this);	
		}
	}
	else if( ret == CKsrcStr::EV_REP ) {

		if( ! tkyngetr( m_TkNumber, m_pZm) ) {
			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkname = teki;
			m_pZm->tkrec->tkana = m_Kana;
			if( m_bInvDisp ) {
				m_pZm->tkrec->invno = checkInvno;
			}
			if( m_bInvMaster ) {
				if( menchk ) {
					m_pZm->tkrec->dsign[4] |= BIT_MENZEI;
				}
				else {
					m_pZm->tkrec->dsign[4] &= ~BIT_MENZEI;
				}
			}
			m_pZm->tkrec->Update();
			m_bDBupdate = TRUE;

			m_pZm->tkrec->Requery( "", 0 );
			PostMessage( WM_USER_1, 0, m_TkNumber );
		}
	}
	else if( ret == CKsrcStr::EV_CANCEL ) {
		c_set( IDC_ICSDBEDT_NUM );
	}

	m_report_tkdic.Populate();
}


BEGIN_EVENTSINK_MAP(CTkdicDlg, CBaseDlg)
    //{{AFX_EVENTSINK_MAP(CTkdicDlg)
	//ON_EVENT(CTkdicDlg, IDC_ICSLISTINPCTRL1, 1 /* EditON */, OnEditONIcslistinpctrl1, VTS_I2 VTS_I2)
	//ON_EVENT(CTkdicDlg, IDC_ICSLISTINPCTRL1, 2 /* Termination */, OnTerminationIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CTkdicDlg, IDC_ICSDBEDTCTRL1, 1 /* Termination */, OnTerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkdicDlg, IDC_ICSDBEDT_NUM, 1 /* Termination */, OnTerminationIcsdbedtNum, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkdicDlg, IDC_ICSDBEDTCTRL1, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl1, VTS_I4)
	ON_EVENT(CTkdicDlg, IDC_ICSDBEDT_NUM, 2 /* KillFocus */, OnKillFocusIcsdbedtNum, VTS_I4)
	//ON_EVENT(CTkdicDlg, IDC_ICSLISTINPCTRL1, 5 /* EditOFF */, OnEditOFFIcslistinpctrl1, VTS_I2)
	//ON_EVENT(CTkdicDlg, IDC_ICSLISTINPCTRL1, 9 /* Focused */, OnFocusedIcslistinpctrl1, VTS_NONE)
	//ON_EVENT(CTkdicDlg, IDC_ICSLISTINPCTRL1, 4 /* KeyIn */, OnKeyInIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
	//ON_EVENT(CTkdicDlg, IDC_ICSLISTINPCTRL1, 12, CTkdicDlg::ReadStringIcslistinpctrl1, VTS_I4 VTS_BSTR)
	//ON_EVENT(CTkdicDlg, IDC_REPORT_TKDIC, 1 /* Termination */, OnTerminationIcslinpTkdic, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CTkdicDlg, IDC_ICSDBEDT_INV, 1, CTkdicDlg::TerminationIcsdbedt8Invno, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkdicDlg, IDC_ICSDBEDT_INV, 2 /* KillFocus */, CTkdicDlg::OnKillFocusIcsdbedt8Invno, VTS_I4)
	
END_EVENTSINK_MAP()



void CTkdicDlg::OnFocusedIcslistinpctrl1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

//	if( m_NowIdx != -1 ) {
//		int clm;
//		clm = m_NowClm != -1 ? m_NowClm : TK_CODE;
//		m_listtkdic.SetInputPositionEX( m_NowIdx, clm );
//	}
}


void CTkdicDlg::OnKeyInIcslistinpctrl1(short nChar, short previndex, short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( nChar == VK_RETURN ) {
		m_listtkdic.SetInputPosition( index, TK_CODE );	
	}
}

//リストコントロール処理のため不使用
void CTkdicDlg::OnEditONIcslistinpctrl1(short index, short column) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int tidx = m_listtkdic.GetTopIndex();
	int	real_index = index + tidx;


	///*

	if( m_InsIndex >= 0 && real_index != m_InsIndex ) {
		m_listtkdic.DeleteInput();
		m_listtkdic.RemoveData( m_InsIndex );
		m_InsIndex = -1;
		m_listtkdic.SetInputPosition( index, column );
		return;
	}
	
	dline_chg( real_index );
	if( m_Ctkrec.tk_code == 0 ) {
		if( column != TK_CODE ) {
			m_listtkdic.DeleteInput();
			m_listtkdic.SetInputPosition( index, TK_CODE );
			column = TK_CODE;
		}
	}

	//*/
	m_NowClm = column;

	// 摘要辞書　消費税選択
	if( column == TK_SYOHI ) {
		// 消費税選択ダイアログボックスの設定
		RECT	rc;
		CWnd	*p;
	
		p = GetFocus();
		p->GetWindowRect( &rc );
		
		//m_listtkdic.DeleteInput();

		SELDLG	*seld = new SELDLG( this );
		m_SelDlgWnd = seld;

		seld->sx_pos = rc.left;
//		seld->sy_pos = rc.top;
		seld->sy_pos = rc.bottom;

		seld->nx_width = rc.right - rc.left;
		seld->ny_width = rc.bottom - rc.top;

		seld->SelItem = "消費税選択";

		// 消費税選択メッセージの初期設定
		int i;
		for( i=0; i < 5; i++ ) {
			skbn_msg( seld->SelMsg[i], i, m_pZm, m_pSy );
		}
		// 8% 対応分
		if( m_SyzType >= 8 ) {
			skbn_msg( seld->SelMsg[i], i, m_pZm, m_pSy );
			i++;

			if (m_SyzType >= 10) {
				skbn_msg( seld->SelMsg[i], i, m_pZm, m_pSy );
				i++;
				skbn_msg( seld->SelMsg[i], i, m_pZm, m_pSy );
				i++;
				skbn_msg( seld->SelMsg[i], i, m_pZm, m_pSy); //標準ー10％
				i++;
			}
		}

		seld->SelMsg[i].Empty();
		seld->SelMsg[0] = "登録なし";

		// 選択番号
		if( m_Ctkrec.tk_sign[1] ) {
			seld->Selno = m_Ctkrec.tk_sign[1];
		}
		else seld->Selno = 0;

		// インボイス対応
		seld->SetColumn( SEL_SYOHI );

		seld->SetDlgData();
	}

}


void CTkdicDlg::OnEditOFFIcslistinpctrl1(short nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください


}

// 振り仮名
void CTkdicDlg::ReadStringIcslistinpctrl1(long index, LPCTSTR string)
{
//TRACE( "CTkdicDlg::ReadString %d, %s\n", index, string );
	LINP_DATA	ldata[10];

	m_listtkdic.GetData( index, (LPUNKNOWN)ldata );

	ldata[2].dat_string.Trim();
	int len = ldata[2].dat_string.GetLength();

	if( ldata[2].dat_initflg || len == 0 ) {
		ldata[2].dat_string = string;
		if( strlen( string ) > 4 ) {
			LPSTR p = ldata[2].dat_string.GetBuffer(48);
			p[4] = '\0';
			ldata[2].dat_string.ReleaseBuffer(-1);
		}

		ldata[2].dat_initflg = FALSE;

		::ZeroMemory( m_Ctkrec.tk_kana, sizeof m_Ctkrec.tk_kana );
		memcpy( m_Ctkrec.tk_kana, ldata[2].dat_string, ldata[2].dat_string.GetLength() );
		m_InputSign = 1;

		m_listtkdic.SetData( index, (LPUNKNOWN)ldata, TRUE );
	}
}


void CTkdicDlg::OnTerminationIcslistinpctrl1(short nChar, short len, short index, short clm, LPUNKNOWN data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください	
//	m_listtkdic.GetData();

	IcsLinpTermination( nChar, len, index, clm, data );
}


//--------------------------------------------------------------
//	摘要入力ターミネーション
//
//--------------------------------------------------------------
void CTkdicDlg::IcsLinpTermination(short nChar, short len, short index, short clm, LPUNKNOWN data) 
{
	long	ret;
	LINP_DATA	*LD = (LINP_DATA *)data;
	LINP_DATA	ldata[10];
	int		cnt;
	CString str;

	ret = LINP_REPLY_OK;

	switch( nChar ) {
	case	VK_F6:	// 摘要複写
					ret |= LINP_REPLY_STAY;
					SendMessage( WM_USER_1, WM_COMMAND, ID_TLBAR_F6 );
					break;
//	case	VK_F8:
//					ret |= LINP_REPLY_STAY;
//					tlbtn = ID_TLBAR_F8;
//					break;
//	case	VK_F9:
//					ret |= LINP_REPLY_STAY;
//					tlbtn = ID_TLBAR_F9;
//					break;
//	case	VK_F12:
//					ret |= LINP_REPLY_STAY;
//					break;

	case	VK_LEFT:
					ret |= LINP_REPLY_BACK;
					break;

	case	VK_RIGHT:
					ret |= LINP_REPLY_NEXT;
					break;

	case	VK_DOWN:
					ret |= LINP_REPLY_DOWN;
					break;

	case	VK_UP:
					ret |= LINP_REPLY_UP;
					break;

	case	VK_PRIOR:
					m_listtkdic.PageMove( -1 );
					break;

	case	VK_NEXT:
					m_listtkdic.PageMove( 1 );
					break;

	case	VK_INSERT:
					m_listtkdic.GetData( index, (LPUNKNOWN)ldata );

					cnt = m_listtkdic.GetCount();

					ret |= LINP_REPLY_STAY;

					if( cnt > (index+1) ) {
						if( ldata[0].dat_initflg || ! l_test( ldata[0].dat_numeric ) ) {
							m_listtkdic.RemoveData( index );	// 空行を元に戻す
							Current_Recset( index );
							m_InsIndex = -1;
						}
						else if( m_InsIndex < 0 ) {
							ldata[0].dat_initflg = TRUE;
							ldata[1].dat_initflg = TRUE;
							ldata[2].dat_initflg = TRUE;
							ldata[3].dat_initflg = TRUE;
							m_listtkdic.InsertData( index, (LPUNKNOWN)ldata, TRUE );
							TkyoNewlineData();
							m_InsIndex = index;

							if( clm != TK_CODE ) {
								ret &= ~LINP_REPLY_STAY;
								ret |= LINP_REPLY_BACK;
							}
						}
					}

					break;
	case	VK_RETURN:
		#ifdef OLD_CLOSE ///////////////////////////////////////////////////////////
					if( !len ) {
						ret |= LINP_REPLY_NEXT;
						break;
					}
					if( CheckTKrecData( index, clm, (LPUNKNOWN)LD ) == TRUE ) {
						ret = LINP_REPLY_NEW | LINP_REPLY_NEXT;
						LD->dat_initflg = FALSE;
					}
					else	{
						ret = LINP_REPLY_ERR;
					}	
		#endif //////////////////////////////////////////////////////////////////////
					ret |= LINP_REPLY_NEXT;
					break;

	case	VK_DELETE:
					if( clm == TK_CODE ) {	// 摘要を削除
						if( m_Ctkrec.tk_code != 0 ) {
							// 摘要を削除できるか？チェック
							m_bListTermOK = FALSE;

							if( ! IsUsedTKcode(m_Ctkrec.tk_code) ) {
								str.Format( "摘要[%6d] を削除します。", m_Ctkrec.tk_code );

								if( ICSMessageBox( str, MB_YESNO, 0,0,this ) == IDYES ) {;
									DeleteTkrec( m_Ctkrec.tk_code );
									m_listtkdic.RemoveData( index );
									Current_Recset( index );
									m_listtkdic.SetInputPositionEX( index, clm );
									ret |= LINP_REPLY_STAY;
									TorokDisp();
								}
							}

							m_bListTermOK = TRUE;
						}
					}
					else {
						if( LD != NULL ) {
							LD->dat_string.Empty();
							CheckTKrecData( index, clm, (LPUNKNOWN)LD );
						}
						ret = LINP_REPLY_NEW;
					}
					break;

	case	0:
				if( LD != NULL && m_bListTermOK ) {
					if( clm != TK_CODE || (clm == TK_CODE && len > 0) ) {
						if( CheckTKrecData( index, clm, (LPUNKNOWN)LD ) == TRUE ) {
							ret = LINP_REPLY_NEW;

							LD->dat_initflg = FALSE;
							ldata[clm] = *LD;
							m_listtkdic.SetColumnData(index, clm, (LPUNKNOWN)ldata, TRUE );
						}
					}
				}
				ret |= LINP_REPLY_OK;
				break;

	case VK_END:
				ret = LINP_REPLY_OK|LINP_REPLY_STAY;
				break;
	default	:	ret = LINP_REPLY_ERR;
				break;
	}

	m_listtkdic.ReplyTermination( ret, (LPUNKNOWN)LD );

//	if( ret & LINP_REPLY_NEXT ) TkyoNewlineCheck( index, clm  );
}

//------------------------------------------------------
// 摘要を削除できるか？使用チェック
//		枝番残
//		連想摘要・仕訳対応摘要
//		int tkcode	消す予定の摘要コード
//
//------------------------------------------------------
BOOL CTkdicDlg::IsUsedTKcode( int tkcode )
{
	CString filter;
	CString tmp, msg, strError;
	int sgn = 0;

	//枝番残高より
	CStRecordSet st_rec( m_pZm->m_database );
	int eda_kmk = 0;

	TRY {
		st_rec.m_sql.Format("select count(*) as st from rtrec where rtcod = %d", tkcode);
		if( !st_rec.Open() ) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;
		}
		if( st_rec.m_st > 0 ) {
			tmp += _T("【科目対応摘要】");
			sgn++;
		}
		st_rec.Close();

		st_rec.m_sql.Format("select count(*) as st from strec where stcod = %d", tkcode);
		if( !st_rec.Open() ) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;
		}
		if( st_rec.m_st > 0 ) {
			tmp += _T("【仕訳対応摘要】");
			sgn++;
		}
		st_rec.Close();

		st_rec.m_sql.Format("select count(*) as st from ezrec where eztcd = %d", tkcode);
		if( !st_rec.Open() ) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;
		}
		if( (m_pZm->zvol->br_sw & 0x01) ) {
			if( st_rec.m_st > 0 ) {
				if( sgn > 0 ) {
					tmp += "\r\n";
				}
				tmp += _T("【科目枝番名称】");
				sgn++;
				eda_kmk |= 0x01;
			}
		}
		st_rec.Close();

		// 部門枝番残高
		if( (m_pZm->zvol->sub_sw&0x02) ) {
			st_rec.m_sql.Format( "select count(*) as st from bmezrec where eztcd = %d", tkcode );
			if( ! st_rec.Open() ) {
				ICSMessageBox( _T( "摘要の使用チェックが行えません！" ) );
				return -1;
			}
			if( st_rec.m_st > 0 ) {
				if( !(eda_kmk & 0x01) ) {
					if( sgn > 0 ) {
						tmp += "\r\n";
					}
				}
				tmp += _T("【部門枝番名称】");
				sgn++;
				eda_kmk |= 0x02;
			}
			st_rec.Close();
		}

		//非営利収支枝番
		if( m_pZm->zvol->apno >= 0x50 ) {
			st_rec.m_sql.Format("select count(*) as st from eszrec where eztcd = %d", tkcode);
			if( !st_rec.Open() ) {
				ICSMessageBox(_T("摘要の使用チェックが行えません！"));
				return -1;
			}
			if( (m_pZm->zvol->br_sw & 0x01) ) {
				if( st_rec.m_st > 0 ) {
					if( (eda_kmk & 0x03) ) {
						tmp += "\r\n";
					}
					else {
						if( sgn > 0 ) {
							tmp += "\r\n";
						}
					}

					tmp += _T("【収支科目枝番名称】");
					sgn++;
					eda_kmk |= 0x04;
				}
			}
			st_rec.Close();

			// 収支部門枝番残高
			if( (m_pZm->zvol->sub_sw & 0x02) ) {
				st_rec.m_sql.Format("select count(*) as st from beszrec where eztcd = %d", tkcode);
				if( !st_rec.Open() ) {
					ICSMessageBox(_T("摘要の使用チェックが行えません！"));
					return -1;
				}
				if( st_rec.m_st > 0 ) {
					if( (eda_kmk & 0x03) && !(eda_kmk & 0x04) ) {
						tmp += "\r\n";
					}
					else {
						if( sgn > 0 && !(eda_kmk & 0x04) ) {
							tmp += "\r\n";
						}
					}
					tmp += _T("【収支部門枝番名称】");
					sgn++;
					eda_kmk |= 0x08;
				}
				st_rec.Close();
			}
		}
	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( strError.GetBuffer(_MAX_PATH), _MAX_PATH );
		strError.ReleaseBuffer();
		ICSMessageBox( strError );
		return FALSE;	
	}
	CATCH( CDBException, de ) {
		strError = de->m_strError;
		ICSMessageBox( strError );
		return FALSE;
	}
	END_CATCH

	CRecordset	rset(m_pZm->m_database);
	CString sqltmp, str2;

	CArray <KMK_REC, KMK_REC> kmkArray;

	if( eda_kmk ) {
		filter.Empty();
		if( eda_kmk & 0x01 ) {
			sqltmp.Format("select -1, ezkcd, ezecd, 0 from ezrec where eztcd = %d", tkcode);
			filter += sqltmp;
		}
		
		if( eda_kmk & 0x02 ) {
			sqltmp.Format("select ezbmn, ezkcd, ezecd, 0 from bmezrec where eztcd = %d", tkcode);

			if( !filter.IsEmpty() ) {
				filter += " union ";
				filter += sqltmp;
			}
			else {
				filter += sqltmp;
			}
		}
		if( eda_kmk & 0x04 ) {
			sqltmp.Format("select -1, ezkcd, ezecd, 1 from eszrec where eztcd = %d", tkcode);
			if( !filter.IsEmpty() ) {
				filter += " union ";
				filter += sqltmp;
			}
			else {
				filter += sqltmp;
			}
		}
		if( eda_kmk & 0x08 ) {
			sqltmp.Format("select ezbmn, ezkcd, ezecd, 1 from beszrec where eztcd = %d", tkcode);
			if( !filter.IsEmpty() ) {
				filter += " union ";
				filter += sqltmp;
			}
			else {
				filter += sqltmp;
			}
		}
		try {
			rset.Open(CRecordset::forwardOnly, filter, CRecordset::readOnly);
		}
		catch( CDBException* dbe ) {	// SQL自体のエラー
			strError = dbe->m_strError;
			ICSMessageBox(strError);
			return FALSE;
		}

		KMK_REC kmrec;

		for( int i = 0; ; i++ ) {
			if( i )
				rset.MoveNext();
			if( rset.IsEOF() )
				break;

			short nIndex = 0;

			rset.GetFieldValue((SHORT)nIndex++, sqltmp);
			kmrec.ezbmn = atoi(sqltmp);

			rset.GetFieldValue((SHORT)nIndex++, sqltmp);
			kmrec.kncod = sqltmp;

			kmrec.knnam.Empty();
			kmrec.knicod = -1;
			DBKNREC* pKn;
			if( (pKn = DB_PjisToKnrec(kmrec.kncod, m_pZm)) != NULL ) {
				kmrec.knnam.Format("%.14s", pKn->knnam);
				kmrec.knicod = pKn->knicod;
			}

			rset.GetFieldValue((SHORT)nIndex++, sqltmp);
			kmrec.ezecd = atoi(sqltmp);

			rset.GetFieldValue((SHORT)nIndex++, sqltmp);
			kmrec.type = atoi(sqltmp);

			kmkArray.Add(kmrec);
		}

		rset.Close();

		//int cnt = kmkArray.GetCount();
		//for( int n = 0; n < cnt; n++ ) {
		//	MyTrace("kmk [%d], '%s', %s, [%d], type(%d)\n", kmkArray[n].ezbmn,
		//			kmkArray[n].kncod, kmkArray[n].knnam, kmkArray[n].ezecd, kmkArray[n].type);
		//}
	}

	BOOL bRet = FALSE;

	if( eda_kmk ) {
		msg.Format(_T("指定された摘要は以下で使用されているため\r\n削除できません！\r\n\r\n%s\r\n"), tmp);

		CTkyDelRepoDlg	dlg;
		dlg.m_pKmAry = &kmkArray;
		dlg.m_pZm = m_pZm;
		dlg.m_str = msg;

		dlg.DoModal();
		bRet = TRUE;
	}
	else {
		if( sgn ) {
			msg.Format(_T("指定された摘要は以下で使用されているため削除できません！\r\n\r\n%s\r\n"), tmp);
			if( !str2.IsEmpty() )	msg += str2;
			msg += "\r\n";

			ICSMessageBox(msg, MB_OK, 0, 0, this);
			bRet = TRUE;
		}
	}

	return bRet;
}

// 摘要コードが使用されているかをチェックする。

//メッセージを出すためにどの摘要項目で使用されているか調べているが、そもそもそも必要か
//

//複数選択削除用に作成　
int CTkdicDlg::CheckUseTkcod(int tkcode)
{

	CString cmd;
	CString filter;
	CString tmp, msg, strError;
	int sgn = 0;

	

	//枝番残高より
	CStRecordSet st_rec(m_pZm->m_database);
	int eda_kmk = 0;


	Tk_UseItem = 0;

	TRY{
		st_rec.m_sql.Format("select count(*) as st from rtrec where rtcod = %d", tkcode);
		if (!st_rec.Open()) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;
		}
		if (st_rec.m_st > 0) {

			Tk_UseItem |= 0x01; // 科目対応摘要
			//tmp += _T("【科目対応摘要】");
			sgn++;

			
		}
		st_rec.Close();

		st_rec.m_sql.Format("select count(*) as st from strec where stcod = %d", tkcode);
		if (!st_rec.Open()) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;
		}
		if (st_rec.m_st > 0) {


			Tk_UseItem |= 0x02; //仕訳対応摘要
			//tmp += _T("【仕訳対応摘要】");
			sgn++;
			
		}
		st_rec.Close();

		st_rec.m_sql.Format("select count(*) as st from ezrec where eztcd = %d", tkcode);
		if (!st_rec.Open()) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;
		}
		if ((m_pZm->zvol->br_sw & 0x01)) {
			if (st_rec.m_st > 0) {
				if (sgn > 0) {
					//tmp += "\r\n";
				}
				//tmp += _T("【科目枝番名称】");
				sgn++;
				eda_kmk |= 0x01;
				Tk_UseItem |= 0x04;

			}
		}
		st_rec.Close();

		// 部門枝番残高
		if ((m_pZm->zvol->sub_sw & 0x02)) {
			st_rec.m_sql.Format("select count(*) as st from bmezrec where eztcd = %d", tkcode);
			if (!st_rec.Open()) {
				ICSMessageBox(_T("摘要の使用チェックが行えません！"));
				return -1;
			}
			if (st_rec.m_st > 0) {
				if (!(eda_kmk & 0x01)) {
					if (sgn > 0) {
						//tmp += "\r\n";
					}
				}
				//tmp += _T("【部門枝番名称】");
				sgn++;
				eda_kmk |= 0x02;

				Tk_UseItem |= 0x10;
			}
			st_rec.Close();
		}

		//非営利収支枝番
		if (m_pZm->zvol->apno >= 0x50) {
			st_rec.m_sql.Format("select count(*) as st from eszrec where eztcd = %d", tkcode);
			if (!st_rec.Open()) {
				ICSMessageBox(_T("摘要の使用チェックが行えません！"));
				return -1;
			}
			if ((m_pZm->zvol->br_sw & 0x01)) {
				if (st_rec.m_st > 0) {
					if ((eda_kmk & 0x03)) {
						//tmp += "\r\n";
					}
					else {
						if (sgn > 0) {
							//tmp += "\r\n";
						}
					}

					//tmp += _T("【収支科目枝番名称】");
					sgn++;
					eda_kmk |= 0x04;

					Tk_UseItem |= 0x08;
				}
			}
			st_rec.Close();

			// 収支部門枝番残高
			if ((m_pZm->zvol->sub_sw & 0x02)) {
				st_rec.m_sql.Format("select count(*) as st from beszrec where eztcd = %d", tkcode);
				if (!st_rec.Open()) {
					ICSMessageBox(_T("摘要の使用チェックが行えません！"));
					return -1;
				}
				if (st_rec.m_st > 0) {
					if ((eda_kmk & 0x03) && !(eda_kmk & 0x04)) {
						//tmp += "\r\n";
					}
					else {
						if (sgn > 0 && !(eda_kmk & 0x04)) {
							//tmp += "\r\n";
						}
					}
					//tmp += _T("【収支部門枝番名称】");
					sgn++;
					eda_kmk |= 0x08;

					Tk_UseItem |= 0x20;
				}
				st_rec.Close();
			}
		}
	
		// 摘要対応 自動仕訳が登録されているか？
		CArray < ICS_TKJOUR_PAC, ICS_TKJOUR_PAC& > tkjourAry;
	
		int result = 0;
	
		tkjourAry.RemoveAll();

		result = pTkjour->SelectALLTkjour(tkcode, tkjourAry); // 2回目のこの箇所で確保したメモリ以上に書き込もうとしている？

		if (result == 1) {	// 摘要仕訳有り
			Tk_UseItem |= 0x40;
			sgn++;
		}
		else if (result == -1) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return -1;//-1がかえっても摘要チェックIF文内を通ってしまっている。
		}
	}
	CATCH(CMemoryException, me) {
		me->GetErrorMessage(strError.GetBuffer(_MAX_PATH), _MAX_PATH);
		strError.ReleaseBuffer();
		ICSMessageBox(strError);
		return FALSE;
	}
	CATCH(CDBException, de) {
		strError = de->m_strError;
		ICSMessageBox(strError);
		return FALSE;
	}
	END_CATCH


	BOOL bRet = FALSE;

	 
	if(sgn) {

		
		bRet = TRUE;
	}
	
	return bRet;
}


//
//	摘要追加 チェック等
//
BOOL CTkdicDlg::CheckTKrecData( short index, short colm, LPUNKNOWN data )
{
	LINP_DATA	*LD = (LINP_DATA*)data;
	LINP_DATA	input_ldata[10];

	int			r, i, tkcod;
	char		pbuf[64];
	CString		Work, Work2;
	LPCTSTR		p;
	BOOL		ret;
	COLORREF ONCOLOR, OFFCOLOR;
	ONCOLOR = CL_BLACK;
	OFFCOLOR = CL_TKBACK;

	LD->dat_bc		= OFFCOLOR;
	LD->dat_fc		= ONCOLOR;
	LD->dat_attr	= LINP_DISP_COLOR;

	ret = FALSE;
	switch( colm )	{
	case	TK_CODE:
			// すでに登録されているかのチェック
			l_print( pbuf, LD->dat_numeric, NUM_FMT );
			tkcod = atoi( pbuf );
			if( ! tkcod )
				break;

			if( ! tkyngetr( tkcod, m_pZm) ) {
				ICSMessageBox("指定された番号は登録されています。", 0,0,0,this);
			}
			else {
				if( m_InsIndex == index || m_Ctkrec.tk_code == 0 ) {
					// 新規に追加
					m_pZm->tkrec->AddNew();
					m_pZm->tkrec->tkcod = tkcod;
					m_pZm->tkrec->dsign.SetSize( SIZE_DSIGN );
					for( int idx = 0; idx < SIZE_DSIGN; idx++ ) {
						m_pZm->tkrec->dsign.SetAt( idx, 0 );
					}
					m_pZm->tkrec->Update();
					m_bDBupdate = TRUE;

					m_pZm->tkrec->Requery( "", 0 );
					m_InsIndex = -1;
					ret = TRUE;
					m_Ctkrec.tk_code = tkcod;
					// リスト再表示
					PostMessage( WM_USER_1, MAKEWPARAM(1, TK_NAME), tkcod );
				}
			}
			break;
	case	TK_NAME:
			ret = TRUE;
			memset( pbuf, 0, sizeof pbuf );

			r = kjlen( (void *)m_Ctkrec.tk_name, m_TKYO_LEN );
			memmove( pbuf, m_Ctkrec.tk_name, r );
			Work.Format( "%s", pbuf );

			if( LD->dat_string != Work ) {
				p = LD->dat_string;
				r = kjlen( (void *)p, m_TKYO_LEN );

				memset( m_Ctkrec.tk_name, 0, m_TKYO_LEN );
				memmove( m_Ctkrec.tk_name, p, r );
				m_InputSign = 1;
			}
			break;

	case	TK_KANA:
					
			r = LD->dat_string.GetLength();
			if( r > KANA_LEN ) r = KANA_LEN;

			// カナ文字　チェック
			for( i=0; i < r; i++ ) {

				if( LD->dat_string[i] == ' ' );
				else if( LD->dat_string[i] == '-' );
				else if( _cclass( LD->dat_string.GetAt( i )) ) break;

				if( LD->dat_string[i] == 'ｰ' )
					LD->dat_string.SetAt( i, '-' );
			}

			if( i == r ) {
				ret = TRUE;

				Work.Format( KANA_FMT, m_Ctkrec.tk_kana );
				Work2.Format( KANA_FMT, LD->dat_string );
				LD->dat_string = Work2;

				if( Work != LD->dat_string ) {
					p = LD->dat_string;
					memset( m_Ctkrec.tk_kana, 0x20, KANA_LEN );
					memmove( m_Ctkrec.tk_kana, p, r );
					m_Ctkrec.tk_kana[r] = '\0';
					m_InputSign = 1;
				}
			}
			break;

	case	TK_SYOHI:
			ret = TRUE;
			m_listtkdic.GetData( index, (LPUNKNOWN)input_ldata );
			//m_report_tkdic.GetItemData(index, (LPUNKNOWN)input_ldata);
			input_ldata[TK_SYOHI].dat_string = LD->dat_string;
			m_listtkdic.SetData( index, (LPUNKNOWN)input_ldata, TRUE );
			m_InputSign = 1;
			break;

	}

	return ret;
}


void CTkdicDlg::DeleteTkrec( int tkcode )
{
	CTKREC* ptk;

	tkdb_init( m_pZm );
	ptk = m_pZm->tkrec;

	int del_sw = 0;
	ptk->MoveFirst();

	if( ptk->st != -1 ) {
		do {
			if( ptk->tkcod == tkcode ) {
				ptk->Delete();
				del_sw = 1;
			}
		} while ( ptk->MoveNext() == 0 );
	}

	if( del_sw ) {
		m_bDBupdate = TRUE;
		tkquery_flag() = TRUE;
	}

}


void CTkdicDlg::Update_Tkrec( int index )
{
	if( m_NowIdx != index /*m_Ctkrec.tk_code != (index+1)*/ && m_InputSign ) {
		CTKREC* ptk;

		CString filter;
		filter.Format( "tkcod = %d", m_Ctkrec.tk_code );
		ptk = m_pZm->tkrec;
		tkquery_flag() = TRUE;
		ptk->Requery(filter, 0);

		if( ptk->st != -1 ) {
			if( ptk->tkname != m_Ctkrec.tk_name || ptk->tkana != m_Ctkrec.tk_kana ||
				memcmp( &ptk->tksgn[0], m_Ctkrec.tk_sign, 2) ) {
				ptk->Edit();
				ptk->tkname	= m_Ctkrec.tk_name;
				ptk->tkana	= m_Ctkrec.tk_kana;
				ptk->tksgn.SetSize(2);
				memcpy( &ptk->tksgn[0], m_Ctkrec.tk_sign, 2);

				ptk->Update();
				m_bDBupdate = TRUE;
			}
		}
		else if( m_Ctkrec.tk_code > 0 ) {
			ptk->AddNew();
			ptk->tkcod = m_Ctkrec.tk_code;
			ptk->tkname = m_Ctkrec.tk_name;
			ptk->tkana	= m_Ctkrec.tk_kana;
			ptk->tksgn.SetSize(2);
			memcpy( &ptk->tksgn[0], m_Ctkrec.tk_sign, 2);
			ptk->Update();
			m_bDBupdate = TRUE;

			PostMessage( WM_USER_1, 0, ptk->tkcod );
		}
	}
}

// 摘要データをニューライン用にセット
void CTkdicDlg::TkyoNewlineData()
{
	::ZeroMemory( &m_Ctkrec, sizeof m_Ctkrec );
}


void CTkdicDlg::dline_chg( int index )
{
//	if( m_NowIdx == -1 )
//		goto FNC_END;

	if( index == m_NowIdx )
		return;

	if( m_NowIdx != -1 )
		Update_Tkrec(index);

	Current_Recset( index );
//FNC_END:
	m_NowIdx = index;
}


void CTkdicDlg::Current_Recset( int index )
{
	// カレントレコードをデータベースにセットする
	LINP_DATA	LD[10];
	char	buf[128];

	if( ! m_listtkdic.GetData( index, (LPUNKNOWN)LD ) ) {
		l_print( buf, LD[0].dat_numeric, NUM_FMT );

		::ZeroMemory( &m_Ctkrec, sizeof m_Ctkrec );

		if( ! tkyngetr( atoi( buf ), m_pZm ) ) {
			m_Ctkrec.tk_code = atoi( buf );
			strncpy_s( m_Ctkrec.tk_name, sizeof m_Ctkrec.tk_name,  m_pZm->tkrec->tkname, m_TKYO_LEN );
			strncpy_s( m_Ctkrec.tk_kana, sizeof m_Ctkrec.tk_kana, m_pZm->tkrec->tkana, KANA_LEN );
			memcpy( &m_Ctkrec.tk_sign[0], &m_pZm->tkrec->tksgn[0], 2 );
		}
	}
	else {
		::ZeroMemory( &m_Ctkrec, sizeof m_Ctkrec );
	}

	m_InputSign = 0;

}




// カナ用 ICSDBEDT
void CTkdicDlg::OnKillFocusIcsdbedtctrl1(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( inplen > 0 ) {
		get_kanadata();
	}	
}


void CTkdicDlg::get_kanadata()
{
	VARIANT var;

	DBdata_get(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	CString org, kana;
	kana = var.pbVal;
	org = kana;

	kana_chk( &kana, kana.GetLength() );
	var.pbVal = (BYTE*)kana.GetBuffer(16);
	kana.ReleaseBuffer();

	if( org != kana ) {
		DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	strcpy_s( m_Kana, sizeof m_Kana, (char*)(LPCTSTR)kana );
//	strcpy_s( m_Kana, sizeof m_Kana, (char*)var.pbVal );
}



void CTkdicDlg::OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	VARIANT var;

	if( inplen > 0 ) {
		get_kanadata();
	}

	if( nChar == VK_DELETE ) {
		::ZeroMemory( m_Kana, sizeof m_Kana );
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			set_focus( IDC_EDIT1 );
		}
		else {
			set_focus( IDC_FURIBUTTON );
		}
	}
	else if( nChar == VK_INSERT) {
	//	SendMessage( WM_COMMAND, MAKELONG(IDC_FURIBUTTON, BN_CLICKED), (LPARAM)GetDlgItem(IDC_FURIBUTTON)->m_hWnd);
	}
	else {
		switch( nChar ) {
		case VK_RETURN:
		case VK_DOWN:
			if( m_bInvDisp ) {
				set_focus(IDC_ICSDBEDT_INV);
			}
			else if( m_bInvMaster ) {
				set_focus(IDC_COMBO_MEN);
			}
			else {
				set_focus(IDC_ICSDBEDT_NUM);
			}
			break;
		case VK_RIGHT:
			set_focus( IDC_FURIBUTTON );
			break;
		case VK_LEFT :
		case VK_F2 :
			set_focus( IDC_EDIT1 );
			break;
		}
	}
}

// 番号用 ICSDBEDT
void CTkdicDlg::OnKillFocusIcsdbedtNum(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( inplen > 0 ) {
		TknoInp( IDC_ICSDBEDT_NUM ); 
	}
}

void CTkdicDlg::OnTerminationIcsdbedtNum(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	VARIANT var;

	if( inplen > 0 ) {
		TknoInp( IDC_ICSDBEDT_NUM ); 
	}

	if( nChar == VK_DELETE ) {
		::ZeroMemory( m_Kana, sizeof m_Kana );
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT_NUM, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			if( m_bInvMaster && m_bInvUse ) {
				set_focus(IDC_COMBO_MEN);
			}
			else if( m_bInvDisp ) {
				set_focus(IDC_ICSDBEDT_INV);
			}
			else {
				set_focus(IDC_ICSDBEDTCTRL1);
			}
		}
		else {
			set_focus( IDC_BUTTON_TOROK );		
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_DOWN :
			set_focus( IDC_BUTTON_TOROK );
			break;
		case VK_RIGHT:

			break;
		case VK_UP:
		case VK_LEFT :
		case VK_F2 :
			if( m_bInvMaster && m_bInvUse ) {
				set_focus(IDC_COMBO_MEN);
			}
			else if( m_bInvDisp ) {
				set_focus(IDC_ICSDBEDT_INV);
			}
			else {
				set_focus(IDC_ICSDBEDTCTRL1);
			}
			break;
		}
	}
}


// 摘要番号入力
void CTkdicDlg::TknoInp( int id )
{
	VARIANT var;
	DBdata_get( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
	int n = 0;
	if( var.pbVal[0] ) {
		n = atoi( (char*)var.pbVal );
	}

	if( ! n ) {
		char bf[20];
		sprintf_s( bf, sizeof bf, "%d", m_Tkno );
		var.pbVal = (BYTE*)bf;
		DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else {
		m_Tkno = n;
	}
}

//消費税選択ダイアログ　ターミネーション
LRESULT CTkdicDlg::OnTKDICMessage(WPARAM wparam, LPARAM lparam)
{

	LRESULT	 return_sw = 0;

	ICSReportRow *focus_row = m_report_tkdic.GetFocusedRow();
	ICSReportColumn* focus_column = m_report_tkdic.GetFocusedColumn();

	if (focus_column == NULL) {
		focus_column = m_report_tkdic.GetColumns()->GetAt(TK_SYOHI);
	}

	if (focus_column == NULL)	return return_sw;
	if (focus_row == NULL)		return return_sw;
	int focus_index = focus_row->GetIndex();
	int focus_col = focus_column->GetIndex();

	CString Kbnbf;
	//ptk = m_pZm->tkrec;

	UINT	Term = m_SelDlgWnd->SelTerm;
	int		Selno = m_SelDlgWnd->Selno;

	//if (Term == VK_LEFT) {
	//	m_report_tkdic.EditItem(focus_index, TK_KANA, TRUE);
	//	return return_sw;
	//}

	if (wparam == 0) {	// 消費税選択

		//UINT	Term = m_SelDlgWnd->SelTerm;
		//int		Selno = m_SelDlgWnd->Selno;

		m_SelDlgWnd->DeleteSelDlg();

		if (Term == VK_RETURN) {

			m_pZm->tkrec->SetAbsolutePosition(focus_index + 1);
			int sgn = m_pZm->tkrec->tksgn.GetAt(1);

			if (sgn != Selno) {
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tksgn.SetAt(1, Selno);
				if(( Selno == 2 ) || ( Selno ==3 )) {
					m_pZm->tkrec->dsign[4] &= ~BIT_MENZEI;
					m_report_tkdic.SetCaption( focus_index, TK_MENZEI, _T( "" ));

					int kbnClear = 0;
					if( m_syzKubn == 2 ) {
						m_pZm->tkrec->dsign[4] &= ~(0x0f);
						kbnClear = 1;
					}
					else if( m_syzKubn == 3 ) {
						m_pZm->tkrec->dsign[3] &= ~(0x0f);
						kbnClear = 1;
					}
					if( kbnClear ) {
						m_report_tkdic.SetCaption(focus_index, TK_KUBUN, _T(""));
					}
				}
				m_pZm->tkrec->Update();
		
				m_pZm->tkrec->Requery("", 0);
				skbn_msg(Kbnbf, Selno , m_pZm, m_pSy);
				m_report_tkdic.SetCaption(focus_index, focus_col, Kbnbf);
		
			}
		}
		SetNexControl( focus_index, TK_SYOHI, TRUE );
	}
	//m_report_tkdic.Populate();
	
	// インボイス対応
	/*ICSReportRows *pRows = m_report_tkdic.GetRows();
	if (pRows) {
		ICSReportRow *pRow = pRows->GetAt(focus_index + 1);
		if (pRow) {
			m_report_tkdic.EditItem(focus_index + 1, TK_CODE, TRUE);
		}
	}*/
	//SetNexControl( focus_index, TK_SYOHI, TRUE );
	else if( wparam == 1 ) {
		SetPrevControl( focus_index, TK_SYOHI, TRUE );
	}
	else if( wparam == 2 ) {
		m_report_tkdic.SetFocus();
		m_report_tkdic.SetFocusedRow( m_report_tkdic.GetRows()->GetAt( focus_index )); 
	}

	return return_sw;
}

		

/*

//	消費税選択ダイアログ　ターミネーション
LRESULT CTkdicDlg::OnTKDICMessage( WPARAM wparam, LPARAM lparam )
{

	LRESULT	 r = 0;
	LINP_DATA	LD;

	short	index = m_listtkdic.GetIndex() + m_listtkdic.GetTopIndex();
	short	max = m_listtkdic.GetCount();

	if( wparam == 0 )	{	// 消費税選択

		UINT	Term = m_SelDlgWnd->SelTerm;
		int		Selno = m_SelDlgWnd->Selno;

		m_SelDlgWnd->DeleteSelDlg();

		if( Term == VK_RETURN ) {

			if( m_Ctkrec.tk_sign[1] != Selno ) {
				m_listtkdic.GetColumnData( index, TK_SYOHI, (LPUNKNOWN)&LD );
				m_Ctkrec.tk_sign[1] = Selno;
				skbn_msg( LD.dat_string, Selno, m_pZm, m_pSy );
				CheckTKrecData( index, TK_SYOHI, (LPUNKNOWN)&LD  );
			}
		}
		if( (index+1) < max ) index++;
		else {
		//	TkyoNewlineCheck( index, TK_SYOHI );
			max = m_listtkdic.GetCount();
			if( ( index +1 ) < max ) index++;
		}
		m_listtkdic.SetInputPositionEX( index, TK_NAME );
	
	}
	else	if( wparam == 1 ) {
		m_listtkdic.SetInputPositionEX( index, TK_KANA );
	}

	return r;

}
*/

// フォーカスセット
void CTkdicDlg::set_focus( int id )
{
	PostMessage( WM_RETROFOCUS, id );
}

// フォーカスセット
LRESULT CTkdicDlg::OnRetroFocus(WPARAM wParam, LPARAM lParam)
{
	c_set( wParam );

	if( wParam == IDC_EDIT1 )
		m_Edit.UpdateSelect();

	return 1;
}

int CTkdicDlg::GetTableOfset(int tkcod)
{

	m_pZm->dcntl->Requery("seq = 7");

	int cnt = m_pZm->dcntl->cnt;
	int		rno = -1;

	m_pZm->tkrec->MoveFirst();
	for (int i = 0; i < cnt; i++) {
		if (m_pZm->tkrec->tkcod == tkcod) {
			rno = i;
			break;
		}
		m_pZm->tkrec->MoveNext();
	}
	return rno;
}



// ユーザーメッセージ
LRESULT CTkdicDlg::OnUser_1(WPARAM wParam, LPARAM lParam)
{
	WORD w		= LOWORD( wParam );
	WORD hiword	= HIWORD( wParam );

	if( w == WM_COMMAND ) {
		switch( lParam ) {
		// 摘要置換
		case ID_TLBAR_F6:
			{
				char teki[60] = {0};
				LINP_DATA	ldata[10];

				m_Edit.get_dictky( teki, sizeof teki );
#ifdef CLOSE // 09.27 /11
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname = teki;
				m_pZm->tkrec->tkana = m_Kana;
				m_pZm->tkrec->Update();
				m_bDBupdate = TRUE;
#endif
//MyTrace( "ID_TLBAR_F6 --> m_Ctkrec, Zm tkcod, %d, %d, ", m_Ctkrec.tk_code, m_pZm->tkrec->tkcod );
//----------------------
				if( m_Ctkrec.tk_code != m_pZm->tkrec->tkcod ) {
					if( ! tkyngetr( m_Ctkrec.tk_code, m_pZm ) ) {
						m_pZm->tkrec->Edit();
						m_pZm->tkrec->tkname = teki;
						m_pZm->tkrec->tkana = m_Kana;
						m_pZm->tkrec->Update();
						m_bDBupdate = TRUE;
					}
				}
				else {
					m_pZm->tkrec->Edit();
					m_pZm->tkrec->tkname = teki;
					m_pZm->tkrec->tkana = m_Kana;
					m_pZm->tkrec->Update();
					m_bDBupdate = TRUE;
				}
				// 09.27 /11
				strncpy_s( m_Ctkrec.tk_name, sizeof m_Ctkrec.tk_name, teki, m_TKYO_LEN );
				strncpy_s( m_Ctkrec.tk_kana, sizeof m_Ctkrec.tk_kana, m_Kana, KANA_LEN );
//----------------------
				m_pZm->tkrec->Requery( "", 0 );

				//int	index = m_listtkdic.GetIndex() + m_listtkdic.GetTopIndex();

				//m_listtkdic.GetData( index, (LPUNKNOWN)ldata );
				//ldata[TK_NAME].dat_string = teki;
				//ldata[TK_KANA].dat_string = m_Kana;
				//m_listtkdic.SetData( index, (LPUNKNOWN)ldata, TRUE );
			}
			break;
		// カナ検索ダイアログ表示
		case ID_TLBAR_F8:

			
			if (m_report_tkdic.GetFocusedRow() == NULL)	post_row = 0;
			else									post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

			if (m_report_tkdic.GetFocusedColumn() == NULL)	post_col = 0;
			else									post_col = m_report_tkdic.GetFocusedColumn()->GetIndex();
			

			
			
			// 摘要リストマーク ON
//			TkrenListPosMark( 1 );

			/*
			// TODO: ここにコントロール通知ハンドラ コードを追加します。
			CWnd *pWnd = GetDlgItem(IDC_BUTTON_SWK);
			if (pWnd == NULL) return 0;
			RECT rc;
			pWnd->GetWindowRect(&rc);

			KSEL_PAR	Kpar;
			ZeroMemory(&Kpar, sizeof(KSEL_PAR));
			Kpar.ksPsgn = KSP_RIGHTBOTTOM;		// ポジション・サイン
			Kpar.ksFsgn = KSF_ABSDISP;		// ファンクション・サイン
			Kpar.ksLnum = 10;				// 表示科目数
			Kpar.ksKana;		// 呼び出し科目の頭文字（ASCII）
			Kpar.ksKrng;		// 対象科目（０：全科目／１：貸借／２：損益／他：（下位：開始／上位：終了））
			Kpar.ksPos.x = rc.right;
			Kpar.ksPos.y = rc.top;			// ダイアログ表示ポジション（スクリーン座標）
			Kpar.ksCwnd;		// ポジションチェック対象ウィンドウ
		//	Kpar.ksKrec;		// 選択された科目のレコード格納バッファ
			Kpar.ksStat;		// ステータス・フラグ
			Kpar.ksFhwd;		// フォントサイズ設定対象ウィンドウハンドル（'98.9.28）

			if (Tekiyo50onSelect(m_pZm, &Kpar, this) != FALSE) {
				if (Kpar.ksStat == 0) {

					int ofs = GetTableOfset(m_pZm->tkrec->tkcod); //m_pZm->tkrec->tkcod 検索した後に表示される分しか入ってない　20220823
					m_report_tkdic.SetFocus();
					//m_report_tkdic.SetFocusedRow(ofs); //引数で選択行の選択状態を指定できる
					//m_report_tkdic.SetTopRow(ofs); // 指定行を最上段へ

					m_report_tkdic.Populate();
					m_report_tkdic.EditItem(ofs, TK_CODE, FALSE);
				}


				break;
			}
			*/
			//if (m_report_tkdic.GetEnabled()) {
			//	{
					CPoint pt;
					CRect rc;
					m_report_tkdic.GetWindowRect(&rc);
					pt.x = rc.left;
					pt.y = rc.top;

					CTksrchDlg dlg(m_pZm, pt, 0, this);

					m_report_tkdic.SetFocus();

					if (dlg.DoModal() == IDOK) {

						PostMessage(WM_USER_1, 0, dlg.m_TKcod);
					}

					else {
						
						//PostMessage(WM_USER_1, 2, dlg.m_TKcod);
						//m_report_tkdic.EditItem(post_row, post_col, TRUE);
						PostMessage(WM_MYMESSAGE, EDIT_IN, 0);
						//ここに処理を追加？カナ検索を×で閉じるとここを通るが、、、、
					}
			//	}
				//c_set(IDC_ICSLISTINPCTRL1);
			//}
			break;
		}
	}
	else if( w == 0 ) {
		//m_TbCust.clear();//再描画する為にマップデータ削除2015.01.13
		MakeTKdicFrm();
		SetItemListTkyo( lParam);
		//m_report_tkdic.Populate();
		TorokDisp();
	}
	else if( w == 1 ) {
		MakeTKdicFrm();
		SetItemListTkyo( lParam, hiword );
		TorokDisp();
	}
	/**
	else if (w == 2) {

		m_report_tkdic.SetFocus();
		//m_report_tkdic.EditItem()
	}//*/

	return 1;
}


// 摘要名称サーチ
//	char* teki ..... チェック用 摘要名称
//	int* tkno ...... 置換が選ばれた場合の、その摘要番号
//  BOOL bOther .... 他の登録処理の ﾎﾞﾀﾝ Enable / Disable
//	-------------------------------------------------------------
//	返送値は KsrcStr.h の enum KEnd_Val
//			EV_ADD ..... 追加
//			EV_REP ..... 置換
//			EV_OTHER ... 他登録
//			EV_CANCEL... キャンセル	
//			0 .......... 同一なし 
//
int CTkdicDlg::SrchTKname( char* teki, int* tkno, BOOL bOther )
{
	int tkcnt;
	CStringArray	strArray;

	tkdb_init( m_pZm );
	tkcnt = get_tknum( m_pZm );

	CTKREC* ptk;
	ptk = m_pZm->tkrec;
	ptk->MoveFirst();

	if( ptk->st != -1 ) {
		do {
			
			CString strName;
			strName.Format( "%.40s", ptk->tkname );

			// 摘要名称文字列の完全なマッチングをとる
			if( strcmp( teki, strName )) continue;

			CString str;
			str.Format( "%6d  %-40.40s [%-4.4s]", ptk->tkcod, strName, ptk->tkana );
			strArray.Add( str );
				
		} while( ptk->MoveNext() == 0 );
	}

	int	maxflg = 0;
	if( tkcnt >= m_TkMax ) maxflg = 1;

	// すでに登録済みの番号のとき
	if( tkyngetr( m_Tkno, m_pZm) == 0 )
		maxflg = 1;

	int	st = 0;
	int	cnt = strArray.GetSize();
	if( cnt ) {

		ICSMessageBox( _T( "同様な摘要辞書が見つかりました！\r\n\r\n一覧を表示します。" ), MB_ICONINFORMATION, 0,0,this );

		// 摘要表示
		CKsrcStr	Tkstr( this );

		Tkstr.strArray.Copy( strArray );
		Tkstr.Maxflg = maxflg;
		Tkstr.bOther = bOther;
		RECT	rc1, rc2;
		GetDlgItem( IDC_EDIT1 )->GetWindowRect( &rc1 );
		GetDlgItem( IDC_SUBBUTTON )->GetWindowRect( &rc2 );

		Tkstr.posx = rc2.left;
		Tkstr.posy = rc1.top;

		// 返送値は KsrcStr.h の enum KEnd_Val
		st = Tkstr.DoModal();

		*tkno = Tkstr.m_RecNo;
	}

	return st;
}


//------------------------------------------------
//	摘要を追加して 他の 登録処理へ
//
//------------------------------------------------
int CTkdicDlg::AddTkrec_OtherReg()
{
	int cnt, number, tkcod, st;
	cnt = number = 0;
	tkdb_init( m_pZm );
	cnt = get_tknum( m_pZm );

	tkcod = m_Tkno;
	if( ! tkcod ) return 0;

	char teki[60] = {0};
	m_Edit.get_dictky( teki, sizeof teki );
	get_kanadata();

	// リスト挿入状態をクリア
	ListInsertClear();
	// 同一の摘要があるか
	m_TkNumber = -1;
	int ret = SrchTKname( teki, &m_TkNumber, TRUE );

	//インボイス番号チェック
	if( !CheckInvoiceNo() )
		return 0;

	char pbuf[128];
	CString checkInvno;
	int ncnt;
	if( m_bInvDisp ) {
		ncnt = GetInvnoString(pbuf, sizeof pbuf);
		if( ncnt == 13 ) {
			checkInvno.Format(_T("T%s"), pbuf);
		}
	}
	BYTE menchk = 0;
	if( m_bInvMaster ) {
		menchk = m_ComboMen.GetCurSel();
	}

	if( ret == CKsrcStr::EV_ADD || ret == 0 ) {
		if( ! tkyngetr( tkcod, m_pZm) ) {
			st = ICSMessageBox("指定された番号は登録されています。\r\n摘要の置換をしますか？", MB_YESNO, 0,0,this );
			if( st == IDYES ) {
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname = teki;
				m_pZm->tkrec->tkana = m_Kana;
				if( m_bInvDisp ) {
					m_pZm->tkrec->invno = checkInvno;
				}
				if( m_bInvMaster ) {
					if( menchk ) {
						m_pZm->tkrec->dsign[4] |= BIT_MENZEI;
					}
					else {
						m_pZm->tkrec->dsign[4] &= ~BIT_MENZEI;
					}
				}
				m_pZm->tkrec->Update();
				m_bDBupdate = TRUE;

				m_pZm->tkrec->Requery( "", 0 );
				SendMessage( WM_USER_1, 0, tkcod );
			}
			else {
				return -1;
			}
		}
		else if( m_pZm->tkrec->GetRecordCount() < m_TkMax ) 
		{
			// 新規に追加
			m_pZm->tkrec->AddNew();
			m_pZm->tkrec->tkcod = tkcod;
			m_pZm->tkrec->tkname = teki;
			m_pZm->tkrec->tkana = m_Kana;
			m_pZm->tkrec->dsign.SetSize( SIZE_DSIGN );
			for( int idx = 0; idx < SIZE_DSIGN; idx++ ) {
				m_pZm->tkrec->dsign.SetAt( idx, 0 );
			}
			if( m_bInvDisp ) {
				m_pZm->tkrec->invno = checkInvno;
			}
			if( m_bInvMaster ) {
				if( menchk ) {
					m_pZm->tkrec->dsign[4] = BIT_MENZEI;
				}
			}
			m_pZm->tkrec->Update();
			m_bDBupdate = TRUE;

			m_pZm->tkrec->Requery( "", 0 );

		}		
		else {
			st = ICSMessageBox("最大登録数まで登録されいるため登録できません！", 0,0,0,this);	
			return -1;
		}

		// リスト再表示
		SendMessage( WM_USER_1, 0, tkcod );

		m_report_tkdic.Populate();
		number = tkcod;
	}
	else if( ret == CKsrcStr::EV_REP ) {
		// 置換
		number = m_TkNumber;
		if( ! tkyngetr( number, m_pZm) ) {
			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkname = teki;
			m_pZm->tkrec->tkana = m_Kana;
			if( m_bInvDisp ) {
				m_pZm->tkrec->invno = checkInvno;
			}
			if( m_bInvMaster ) {
				if( menchk ) {
					m_pZm->tkrec->dsign[4] |= BIT_MENZEI;
				}
				else {
					m_pZm->tkrec->dsign[4] &= ~BIT_MENZEI;
				}
			}
			m_pZm->tkrec->Update();
			m_bDBupdate = TRUE;

			m_pZm->tkrec->Requery( "", 0 );
			SendMessage( WM_USER_1, 0, tkcod );
		}
	}
	else if( ret == CKsrcStr::EV_OTHER ) {	// 他登録へ
		number = m_TkNumber;
	}
	else if( ret == CKsrcStr::EV_CANCEL ) {
		c_set( IDC_ICSDBEDT_NUM );
	}

	return number;
}


void CTkdicDlg::OnButtonDebkmk() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int cod = AddTkrec_OtherReg();

	if( cod > 0 ) {
		::ZeroMemory( &m_TkAddPar, sizeof(m_TkAddPar));
		m_TkAddPar.ta_cod = cod;
		m_TkAddPar.ta_typ = TP_KAMOK;
		m_TkAddPar.ta_kmktyp = 0;
		GetParentOwner()->PostMessage( WM_TEKIADDJOB, 0, (LPARAM)&m_TkAddPar);
	}
	
}

// 貸借科目対応摘要にも追加
void CTkdicDlg::OnButtonCrekmk() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int cod = AddTkrec_OtherReg();

	if( cod > 0 ) {
		::ZeroMemory( &m_TkAddPar, sizeof(m_TkAddPar));
		m_TkAddPar.ta_cod = cod;
		m_TkAddPar.ta_typ = TP_KAMOK;
		m_TkAddPar.ta_kmktyp = 1;
		GetParentOwner()->PostMessage( WM_TEKIADDJOB, 0, (LPARAM)&m_TkAddPar);
	}	
}

// 貸借科目対応摘要にも追加
void CTkdicDlg::OnButtonDebcre() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int cod = AddTkrec_OtherReg();

	if( cod > 0 ) {
		::ZeroMemory( &m_TkAddPar, sizeof(m_TkAddPar));
		m_TkAddPar.ta_cod = cod;
		m_TkAddPar.ta_typ = TP_KAMOK;
		m_TkAddPar.ta_kmktyp = 2;
		GetParentOwner()->PostMessage( WM_TEKIADDJOB, 0, (LPARAM)&m_TkAddPar);
	}	
}


// 仕訳科目対応摘要にも追加
void CTkdicDlg::OnButtonSwk() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int cod = AddTkrec_OtherReg();

	if( cod > 0 ) {
	
		CSWKselDlg dlg(this, TRUE );
		CSWKselDlg::bSwkDlgShow = TRUE;

		dlg.SetZmClass( m_pZm );

		if( dlg.DoModal() == IDOK ) {
			::ZeroMemory( &m_TkAddPar, sizeof(m_TkAddPar));
		
			m_TkAddPar.ta_cod = cod;
			m_TkAddPar.ta_typ = TP_SIWAKE;

			if( dlg.m_Selected.type == RSW_BOTH ) {
				// 借貸 どちらにも登録
				strcpy_s( m_TkAddPar.ta_swk[0].debcd, sizeof m_TkAddPar.ta_swk[0].debcd, dlg.m_Selected.debcode );
				strcpy_s( m_TkAddPar.ta_swk[0].crecd, sizeof m_TkAddPar.ta_swk[0].crecd, dlg.m_Selected.crecode );
			//	m_TkAddPar.ta_swk[1].debcd = 0;
			//	m_TkAddPar.ta_swk[1].crecd = 0;
			}
			else {
				strcpy_s( m_TkAddPar.ta_swk[0].debcd, sizeof m_TkAddPar.ta_swk[0].debcd, dlg.m_Selected.debcode );
				strcpy_s( m_TkAddPar.ta_swk[0].crecd, sizeof m_TkAddPar.ta_swk[0].crecd, dlg.m_Selected.crecode );
			//	m_TkAddPar.ta_swk[1].debcd = 0;
			//	m_TkAddPar.ta_swk[1].crecd = 0;
			}

			GetParentOwner()->PostMessage( WM_TEKIADDJOB, 0, (LPARAM)&m_TkAddPar);
		}
	}	
}

void CTkdicDlg::OnNMReportEditsetfocusREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn = pItemNotify->pColumn;
	int	indexColumn = pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow = pItemNotify->pRow;
	int	indexRow = pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem = pItemNotify->pItem;
	int	indexItem = pItem->GetIndex();

	int newrow = 0;
	
	if ((indexColumn == TK_CODE || indexColumn == TK_NAME) && m_report_tkdic.GetleftOffset() != 0) {
		m_report_tkdic.SetLeftOffset(0);
		m_report_tkdic.SetFocus();
		ICSReportRows *pRows = m_report_tkdic.GetRows();
		if (pRows) {
			ICSReportRow *pRow2 = pRows->GetAt(indexRow);
			if (pRow2) {
				m_report_tkdic.SetFocusedRow(pRow2);
				m_report_tkdic.EditItem(indexRow, indexColumn, TRUE);
			}
		}
		//m_report.EditItem( indexRow, indexColumn, TRUE );
	}



	//最下行のedit可不可?//


	
	if ((insNewLine == indexRow || addNewLine == indexRow)) {


		if (indexColumn == TK_CODE) {

			return;

		}
		else {

			m_report_tkdic.SetFocus();
			post_row = indexRow;
			post_col = 0;
			m_report_tkdic.SetFocusedRow(pRow);

			//m_report_tkdic.EditItem(indexRow, TK_CODE, TRUE);

		    PostMessage(WM_MYMESSAGE, EDIT_ON_FIRST, 0);
			return;
		}
	}
	

	
	if (insNewLine != -1 && insNewLine != indexRow) {

		//SetItemListTkyo();
		m_report_tkdic.RemoveRecordAt(insNewLine);
		
		insNewLine = -1;

		//ここでaddNewLine=-1なら　挿入行ありの状態で挿入が押された場合　!=-1なら　挿入行有りの状態で追加を押された場合

		//	focus_index = insNewLine;
		ICSReportRecord*		pRecord;
		//ICSReportRecordItem*	pItem;

		addNewLine = m_report_tkdic.GetRecords()->GetCount();

		pRecord = new ICSReportRecord();
		m_report_tkdic.InsertRecordAt(addNewLine, pRecord);
		SetReportRecord(pRecord);

		m_report_tkdic.Populate();


		
		m_report_tkdic.SetFocus();
		ICSReportRows *pRows = m_report_tkdic.GetRows();
		if (pRows) {
			ICSReportRow *pRow = pRows->GetAt(indexRow);
			if (pRow) {
				m_report_tkdic.SetFocusedRow(pRow);
				m_report_tkdic.EditItem(indexRow, indexItem, TRUE);
			}
		}

		return;
	}

	if( indexColumn == TK_KUBUN ) {
		// 区分選択ダイアログボックスの設定
		RECT rect;
		CWnd *pWnd = NULL;

		// 売上・仕入区分なし
		if( !(m_syzKubn == 2 || m_syzKubn == 3) ) {
			//次の項目にフォーカスセット
			post_row = indexRow;
			post_col = TK_SYOHI;
			PostMessage(WM_MYMESSAGE, EDIT_IN, 0);
			return;
		}

		pWnd = GetFocus();
		if( pWnd == NULL ) {
			return;
		}
		pWnd->GetWindowRect( &rect );

		SELDLG *pSeld = new SELDLG( this );
		m_SelDlgWnd = pSeld;

		pSeld->sx_pos = rect.left;
		pSeld->sy_pos = rect.bottom;

		pSeld->nx_width = rect.right - rect.left;
		pSeld->ny_width = rect.bottom - rect.top;

		pSeld->SelItem = "区分選択";
		// 区分選択メッセージの初期設定
		int i;
		int cnt = 1;

		if( m_syzKubn == 2 ) {
			for( i = 0; i < 4; i++ ) {
				SkbnMsg( pSeld->SelMsg[i], i, 2 );
			}
		}
		else if( m_syzKubn == 3 ) {
			for( i = 1; i < 8; i++ ) {
				SkbnMsg( pSeld->SelMsg[cnt], i, 3 );
				if( pSeld->SelMsg[cnt].IsEmpty() == 1 ) {
					;
				}
				else {
					cnt++;
				}
			}

		}

		if( m_syzKubn == 3 ) {
			pSeld->SelMsg[cnt].Empty();
		}
		else {
			pSeld->SelMsg[i].Empty();
		}
		pSeld->SelMsg[0] = _T( "登録なし" );

		CString str;
		m_pZm->tkrec->SetAbsolutePosition( indexRow + 1 );
		if( m_syzKubn == 2) {
			pSeld->Selno = m_pZm->tkrec->dsign.GetAt( 4 ) & 0x0f;
		}
		else if( m_syzKubn == 3 ) {
			pSeld->Selno = SkbnMsg(str, m_pZm->tkrec->dsign.GetAt( 3 ), 3, 1 );
		}

		pSeld->SetColumn( SEL_KUBUN );
		pSeld->SetDlgData();
	}


	if (indexColumn == TK_SYOHI) {

		// 消費税選択ダイアログボックスの設定
		RECT	rc;
		CWnd	*p;

		p = GetFocus();
		p->GetWindowRect(&rc);

		//m_listtkdic.DeleteInput();

		SELDLG	*seld = new SELDLG(this);
		m_SelDlgWnd = seld;

		seld->sx_pos = rc.left;
		//		seld->sy_pos = rc.top;
		seld->sy_pos = rc.bottom;

		seld->nx_width = rc.right - rc.left;
		seld->ny_width = rc.bottom - rc.top;

		seld->SelItem = "消費税選択";

		// 消費税選択メッセージの初期設定
		int i;
		for (i = 0; i < 5; i++) {
			skbn_msg(seld->SelMsg[i], i, m_pZm, m_pSy);
		}
		// 8% 対応分
		if (m_SyzType >= 8) {
			skbn_msg(seld->SelMsg[i], i, m_pZm, m_pSy);
			i++;

			if (m_SyzType >= 10) {
				skbn_msg(seld->SelMsg[i], i, m_pZm, m_pSy);
				i++;
				skbn_msg(seld->SelMsg[i], i, m_pZm, m_pSy);
				i++;
				skbn_msg(seld->SelMsg[i], i, m_pZm, m_pSy); //標準ー10％
				i++;
			}
		}

		seld->SelMsg[i].Empty();
		seld->SelMsg[0] = "登録なし";

		/*// 選択番号
		if (m_Ctkrec.tk_sign[1]) {
			seld->Selno = m_Ctkrec.tk_sign[1];
		}

		*/

		m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);
		seld->Selno = m_pZm->tkrec->tksgn.GetAt(1);
		//else seld->Selno = 0;

		// インボイス対応
		seld->SetColumn( SEL_SYOHI );

		seld->SetDlgData();

		//delete seld;
	}

	if (indexColumn == TK_NAME) {//摘要名称欄にて入力カーソル位置を末尾に指定
		CString list_str;
		list_str = m_report_tkdic.GetCaption(indexRow, indexColumn);
		int len = list_str.GetLength();
		m_report_tkdic.SetCaretPosition(len);

	}

	// インボイス対応
	//　登録番号
	if( indexColumn == TK_INVNO ) {
		CString list_str;
		int len = 0;

		list_str = m_report_tkdic.GetCaption( indexRow, indexColumn );
		list_str.Replace( _T( "T" ), _T( "" ));
		m_report_tkdic.SetCaption( indexRow, indexColumn, list_str );
	}

	// 免税事業者からの課税仕入れ
	if( indexColumn == TK_MENZEI ) {
		CString tkCode;
		int tkSgn = 0;

		tkCode = m_report_tkdic.GetCaption( indexRow, TK_CODE );
		m_pZm->tkrec->Find( atoi( tkCode ));
		tkSgn = m_pZm->tkrec->tksgn.GetAt( 1 );
		if(( tkSgn == 2 ) || ( tkSgn == 3 )) {
			m_report_tkdic.SetFocus();
			post_row = indexRow;
			post_col = 0;
			PostMessage( WM_MYMESSAGE, EDIT_ON_FIRST, 0 );
			return;
		}

		// 区分選択ダイアログの設定
		RECT rc = { 0 };
		CWnd *pWnd = NULL;

		pWnd = GetFocus();
		if( pWnd == NULL ) {
			return;
		}
		pWnd->GetWindowRect( &rc );

		SELDLG *pSelDlg = NULL;
		pSelDlg = new SELDLG( this );

		m_SelDlgWnd = pSelDlg;

		// サイズ
		pSelDlg->sx_pos = rc.left;
		pSelDlg->sy_pos = rc.bottom;
		pSelDlg->nx_width = ( rc.right - rc.left );
		pSelDlg->ny_width = ( rc.bottom - rc.top );

		// タイトル
		pSelDlg->SelItem = _T( "免税事業者選択" );

		// 項目
		pSelDlg->SelMsg[0] = _T( "登録なし" );
		pSelDlg->SelMsg[1] = _T( "○" );

		m_pZm->tkrec->SetAbsolutePosition( indexRow + 1 );
		pSelDlg->Selno = ( m_pZm->tkrec->dsign[4] & BIT_MENZEI ) ? 1 : 0;
		pSelDlg->SetColumn( SEL_MENZEI );
		pSelDlg->SetDlgData();
	}
}


void CTkdicDlg::OnNMReportEditkillfocusREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result)
{

	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn = pItemNotify->pColumn;
	int	indexColumn = pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow = pItemNotify->pRow;
	int	indexRow = pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem = pItemNotify->pItem;
	int	indexItem = pItem->GetIndex();

	if (edit_keydown_sw) {
		//edit_keydown_sw = 0;
		return;
	}

	///////////////////
	char dmmy[512] = { 0 }, kk[128] = { 0 };
	int i = 0, k = 0, set_c = 0, str_len = 0, st = 0;
	CString cod_str;
	long	cod = 0;

	CString kstr;

	CString list_str, strch, Clear_str;
	list_str.Empty();
	Clear_str.Empty();
	DWORD_PTR f_code;
	int select = 0, check_cod = 0;
	long ny_cod = 0, report_tk_cod = 0;

	int kana_flg = 0, rdw_flg = 0;


	list_str = pItem->GetCaption(pColumn);	//リスト内選択文字の取得
	sprintf_s(kk, sizeof(kk), "%s", list_str);
	str_len = list_str.GetLength();

	cod_str = m_report_tkdic.GetCaption(indexRow, TK_CODE);
	report_tk_cod = atoi(cod_str);


	

	//	m_listtkdic.GetData( index, (LPUNKNOWN)li_data );
	l_defn(0x16);

	map<long, CTKDATA_REC >::iterator it = m_TbCust.find(report_tk_cod);

	m_pZm->tkrec->Requery("", 0); //('15.12.08) リクエリ順が変わったままの場合も考慮する
	m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);

	kstr.Format("%s", list_str.Left(str_len));



	if (indexColumn == TK_CODE) {//　番号をbackspece等で消した後他のカラムをクリックしても元の摘要番号を表示
		if (indexRow != insNewLine && indexRow != addNewLine) {
			m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);
			CString tcstr;
			tcstr.Format("%6d", m_pZm->tkrec->tkcod);
			m_report_tkdic.SetCaption(indexRow, indexColumn, tcstr);
		}
		return;
	}

	if (indexColumn == TK_NAME) {
	
		if (m_pZm->tkrec->tkname.Compare(kstr)
			|| (str_len != list_str.GetLength())) {// 入力後enter押さずに他のカラムをクリックしたときにも入力を反映

			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkname.Format("%s", kstr);
			m_pZm->tkrec->Update();

		}
	}

	if (indexColumn == TK_KANA) {
		if (m_pZm->tkrec->tkana.Compare(kstr)
			|| (str_len != list_str.GetLength())) {// 入力後enter押さずに他のカラムをクリックしたときにも入力を反映

			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkana.Format("%s", kstr);
			m_pZm->tkrec->Update();

		}
	}

	// インボイス対応
	if( indexColumn == TK_INVNO ) {
		if( PosCngChk ) {
			GetTableOfset( report_tk_cod );
		}

		// 前に「T」をセット
		if( !list_str.IsEmpty() ) {
			list_str.Format( _T( "T%s" ), pItem->GetCaption( pColumn ) );
		}

		int len = list_str.GetLength();
		if( len > 14 ) {
			len = 14;
		}

		if( len > 0 ) {
			// 登録番号のチェック
			if( syzInvoice.CheckInvoiceNumber( list_str ) == -1 ) {
			//	list_str.Empty();
			//	m_report_tkdic.SetCaption( indexRow, indexColumn, list_str );
				ICSMessageBox( syzInvoice.GetErrorMessage(), MB_ICONSTOP );
				m_report_tkdic.SetFocus();
				return;
			}
		}

		CString kstr, check_str;
		kstr = list_str;
		if(( m_pZm->tkrec->invno.Compare( kstr )) || ( len != m_pZm->tkrec->invno.GetLength() )) {
			// 変更あり
			kspTrim( kstr );
			check_str = kstr;
			check_str.Replace( _T( "T" ), _T( "" ));

			if( check_str.IsEmpty() ) {
				m_pZm->tkrec->invno.Empty();
				it->second.invno.Empty();
			}
			else {
				m_pZm->tkrec->invno = kstr;
				it->second.invno = kstr;

				m_report_tkdic.SetCaption( indexRow, indexColumn, kstr );
				st = UpdateTkrecEx( m_pZm->tkrec );

				m_pZm->tkrec->Requery( _T( "" ), 0 );
			}
		}
		else {
			// 変更なし
			m_report_tkdic.SetCaption( indexRow, indexColumn, list_str );
		}
	}
	
}


void CTkdicDlg::SetMapColor()
{
	m_report_tkdic.Populate();
	//常に新規行か挿入行がある為＋１とする



	for (int i = 0; i < m_TbCust.size() + 1; i++) {
		if (i % 2)	m_report_tkdic.SetRowBackColor(i, BKCOLOR, TRUE);
		else		m_report_tkdic.SetRowBackColor(i, BKCOLOR2, TRUE);
	}

	ICSColorInfo info;
	((ICSWinApp *)AfxGetApp())->GetStanderdColor(info);
	COLORREF NO_SET = info.ViewColor();
	COLORREF ON_SET = info.ViewColor();


	if (m_bSyzInp) {
		if (!(m_pZm->zvol->tl_cor & 0x10)) {// マスター確定（修正不可）2014.12.11
			m_report_tkdic.AllowEditColumn(TK_SYOHI, FALSE);
			m_report_tkdic.AllowEditColumn( TK_KUBUN, FALSE );
			m_report_tkdic.AllowEditColumn( TK_INVNO, FALSE );
			m_report_tkdic.AllowEditColumn( TK_MENZEI, FALSE );
		}
		else {
			m_report_tkdic.AllowEditColumn(TK_SYOHI, TRUE);
			m_report_tkdic.AllowEditColumn( TK_KUBUN, TRUE );
			m_report_tkdic.AllowEditColumn( TK_INVNO, TRUE );
			m_report_tkdic.AllowEditColumn( TK_MENZEI, TRUE );
		}
	}
	else {
		m_report_tkdic.SetColumnBackColor(TK_SYOHI, NO_SET, FALSE);
		m_report_tkdic.AllowEditColumn(TK_SYOHI, FALSE);

		m_report_tkdic.SetColumnBackColor( TK_KUBUN, NO_SET, FALSE );
		m_report_tkdic.AllowEditColumn( TK_KUBUN, FALSE );
		m_report_tkdic.SetColumnBackColor( TK_INVNO, NO_SET, FALSE );
		m_report_tkdic.AllowEditColumn( TK_INVNO, FALSE );
		m_report_tkdic.SetColumnBackColor( TK_MENZEI, NO_SET, FALSE );
		m_report_tkdic.AllowEditColumn( TK_MENZEI, FALSE );
	}


	
}


LRESULT CTkdicDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{


	WORD	MsgID = LOWORD(wParam);
	WORD	WndID = HIWORD(wParam);

	m_report_tkdic.SetFocus();

	if (post_row < 0) return TRUE;
	if (post_col < 0) return TRUE;

	//=====
	ICSReportRows	*pRows = m_report_tkdic.GetRows();
	if (pRows == NULL) {
		return TRUE;
	}
	ICSReportRow	*pRow = pRows->GetAt(post_row);
	if (pRow == NULL) {
		return TRUE;
	}
	//=====


	switch (MsgID) {
	case EDIT_ON_FIRST:
		m_report_tkdic.SetFocusedRow(pRow);
		m_report_tkdic.EditItem(post_row, TK_CODE, TRUE);
		break;
	case EDIT_IN:
		m_report_tkdic.SetFocusedRow(pRow);
		m_report_tkdic.EditItem(post_row, post_col, TRUE);
		break;
	default:
		break;

	}
	//edit_keydown_sw = 0;


	return	TRUE;
}




/*
BOOL CTkdicDlg::PreTranslateMessage(MSG* pMsg)_bk
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		long key = pMsg->wParam;
	//	int id = ::GetDlgCtrlID( pMsg->hwnd );
		int id = 0;

		CWnd* pwnd = FromHandle( pMsg->hwnd );
		if( pwnd )	id = pwnd->GetDlgCtrlID();
		BOOL bFlag = TRUE;

		if( m_listtkdic.IsChild( pwnd ) || m_listtkdic.m_hWnd == pMsg->hwnd || 
			m_Edit.m_hWnd == pMsg->hwnd )
			bFlag = FALSE;

		if( bFlag ) {
			if( key == VK_TAB ) {
				if( is_SHIFT() )	key = VK_F2;
			}
			if( key == VK_RETURN ) {
				switch( id ) {
				case IDC_BUTTON_TOROK:
				case IDC_BUTTON_DEBKMK:
				case IDC_BUTTON_CREKMK:
				case IDC_BUTTON_DEBCRE:
				case IDC_BUTTON_SWK:
				case IDC_FURIBUTTON:
					key = 0;
					break;
				}
			}

			if( id == IDC_ICSDBEDTCTRL1 ) {

				switch( key ) {
				case VK_RETURN:
				case VK_DOWN:
					set_focus( IDC_ICSDBEDT_NUM );
					break;
				case VK_TAB:
					set_focus( IDC_FURIBUTTON );
					return TRUE;
				case VK_F2:
				case VK_UP:
					PrevDlgCtrl();
					return TRUE;
				default:
					key = 0;
					break;
				}
			}

			switch( key ) {
			case VK_RETURN:
			case VK_TAB:
			case VK_DOWN:
			case VK_RIGHT:
				if( id == IDC_BUTTON_SWK )
					m_listtkdic.SetFocus();
				else
					NextDlgCtrl();
				return TRUE;
				break;
			case VK_F2:
			case VK_LEFT:
			case VK_UP:
				PrevDlgCtrl();
				return TRUE;
				break;
			}
		}
	}

	return CBaseDlg::PreTranslateMessage(pMsg);
}
*/



BOOL CTkdicDlg::PreTranslateMessage(MSG* pMsg)
{
	int clm = 0;
	int row = 0;
	int toprow = 0;
	BOOL tkok = 0;//摘要入力判定
	//CWnd *pWnd = GetFocus();

	//ICSReportRows	*pRows = m_report_tkdic.GetRows();
	//ICSReportRow	*pRow = pRows->GetAt(post_row);
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		long key = pMsg->wParam;
		//	int id = ::GetDlgCtrlID( pMsg->hwnd );
		int id = 0;
		
		CWnd* pwnd = FromHandle(pMsg->hwnd);
		if (pwnd)	id = pwnd->GetDlgCtrlID();
		BOOL bFlag = TRUE;

		tkok = m_Edit.IsWindowEnabled();
	
		if (m_report_tkdic.IsChild(pwnd) || m_report_tkdic.m_hWnd == pMsg->hwnd ||
			m_Edit.m_hWnd == pMsg->hwnd )
			bFlag = FALSE;

		if( m_DBEkana.m_hWnd == pMsg->hwnd || m_DBEnum.m_hWnd == pMsg->hwnd || m_DBEinv.m_hWnd == pMsg->hwnd ) {
			return CBaseDlg::PreTranslateMessage(pMsg);
		}

		if (m_report_tkdic.GetFocusedRow() == NULL)	row = -1;
		else {
			row = m_report_tkdic.GetFocusedRow()->GetIndex();
		}
		
		if (bFlag) {
			if (key == VK_TAB) {
				if (is_SHIFT())	key = VK_F2;
			}
			if (key == VK_RETURN) {
				switch (id) {
				case IDC_BUTTON_TOROK:
				case IDC_BUTTON_DEBKMK:
				case IDC_BUTTON_CREKMK:
				case IDC_BUTTON_DEBCRE:
				case IDC_BUTTON_SWK:
				case IDC_FURIBUTTON:
					key = 0;
					break;
				case IDC_COMBO_MEN:
					NextDlgCtrl();
					return TRUE;
					break;
				}
			}
			key = ComboBoxJob(id, key);

			if (id == IDC_ICSDBEDTCTRL1) {

				switch (key) {
				case VK_RETURN:
				case VK_DOWN:
					if( m_bInvDisp ) {
						set_focus(IDC_ICSDBEDT_INV);
					}
					else if( m_bInvMaster ) {
						set_focus(IDC_COMBO_MEN);
					}
					else {
						set_focus(IDC_ICSDBEDT_NUM);
					}
					break;
				case VK_TAB:
					set_focus(IDC_FURIBUTTON);
					return TRUE;
				case VK_F2:
				case VK_UP:
					PrevDlgCtrl();
					return TRUE;
				default:
					key = 0;
					break;
				}
			}

			switch (key) {
			case VK_RETURN:
			//case VK_TAB:
			case VK_DOWN:
			case VK_RIGHT:
				if (tkok == FALSE) {
					//m_report_tkdic.Populate();
					//m_report_tkdic.SetFocus();

					if (m_report_tkdic.GetFocusedRow() == NULL)	post_row = 0;
					else									post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

					post_col = TK_CODE;
					PostMessage(WM_MYMESSAGE, EDIT_IN, 0);
					/*
					if (row == -1) {

						m_report_tkdic.EditItem(0, TK_CODE, TRUE);
					}
					
					m_report_tkdic.EditItem(row, TK_CODE, TRUE);
					*/
					//tab押下でレポートコントロールから登録番号欄へ戻れるようにする。1208
				}
				else if (id == IDC_BUTTON_SWK) {
					m_report_tkdic.SetFocus();//ボタンTab押下でここに入ってくるその後ブレイクポイントを置いた状態で動かすと白くはならないがこの後の処理を見直す1208
					if (m_report_tkdic.GetFocusedRow() == NULL)	post_row = 0;
					else									post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

					m_report_tkdic.SetFocusedRow(post_row);
				}
				else {
					NextDlgCtrl();
				}
				return TRUE;
				break;

			//1209検証
			case VK_TAB:
				if (tkok == FALSE) {
					if (id != IDC_REPORT_TKDIC) {
						m_report_tkdic.SetFocus();
						if (m_report_tkdic.GetFocusedRow() == NULL)	post_row = 0;
						else									post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

						m_report_tkdic.SetFocusedRow(post_row);
						//PostMessage(WM_MYMESSAGE, EDIT_IN, 0);
					}
					else {
						set_focus(IDC_BUTTON_TOROK);
					}
				}
				else {
					if (id == IDC_BUTTON_SWK) {
						m_report_tkdic.SetFocus();

						if (m_report_tkdic.GetFocusedRow() == NULL)	post_row = 0;
						else									post_row = m_report_tkdic.GetFocusedRow()->GetIndex();
						m_report_tkdic.SetFocusedRow(post_row);
					}
					else {
						NextDlgCtrl();
					}
				}
				return TRUE;

			case VK_F2:
			case VK_LEFT:
			case VK_UP:
				if (tkok == FALSE) {
					set_focus(IDC_ICSDBEDT_NUM);
				}
				else {
					PrevDlgCtrl();
				}
				return TRUE;
				break;
			}
		}

		if (row >= 0) {

			switch (key) {

			case VK_TAB:
				if (m_report_tkdic.GetFocusedColumn() == NULL) {
					NextDlgCtrl();
					return TRUE;
				}
				else {
					edit_keydown_sw = 0;
				}
				break;

			case VK_RETURN:
				if (m_report_tkdic.GetFocusedColumn() == NULL) {

					if (m_report_tkdic.GetFocusedRow() == NULL)	post_row = 0;
					else									post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

					if (m_report_tkdic.GetFocusedColumn() == NULL)	post_col = 0;
				    else									post_col = m_report_tkdic.GetFocusedColumn()->GetIndex();
		

					//m_report_tkdic.EditItem(post_row, post_col, TRUE);

					PostMessage(WM_MYMESSAGE, EDIT_IN, 0);

					edit_keydown_sw = 0;

					return TRUE;
				}
				break;
			
			case VK_DELETE:
				if (m_report_tkdic.GetFocusedColumn() != NULL && m_report_tkdic.GetSelectedRowsCount() == 1) {//単行選択削除
					clm = m_report_tkdic.GetFocusedColumn()->GetIndex();
					if (clm == TK_CODE) {
						/*
						post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

						m_pZm->tkrec->SetAbsolutePosition(post_row + 1);
						CString tcstr;
						tcstr.Format("%6d", m_pZm->tkrec->tkcod);
						m_report_tkdic.SetCaption(post_row, clm, tcstr);

						*/
						//SendMessage(WM_MYMESSAGE, EDIT_IN, 0);
						OnBnDel();
						return TRUE;
					}
				}
				else if (m_report_tkdic.GetSelectedRowsCount() != 0) {//複数行選択削除
					OnBnDel();
					return TRUE;
				}
				break;

			case VK_INSERT:
				OnBnIns();

				return TRUE;
				//break;

			case VK_F6:
				char teki[60] = { 0 };
				LINP_DATA	ldata[10];

				m_Edit.get_dictky(teki, sizeof teki);
#ifdef CLOSE // 09.27 /11
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname = teki;
				m_pZm->tkrec->tkana = m_Kana;
				m_pZm->tkrec->Update();
				m_bDBupdate = TRUE;
#endif
				if (m_report_tkdic.GetFocusedColumn() == NULL)	post_col = 0;
				else									post_col = m_report_tkdic.GetFocusedColumn()->GetIndex();

					//----------------------
				ICSReportRow *focus_row = m_report_tkdic.GetFocusedRow();

				int focus_line = focus_row->GetIndex();
				int endline = m_report_tkdic.GetRecords()->GetCount() - 1;
				/*
				int ofs = GetTableOfset(m_pZm->tkrec->tkcod);
				m_report_tkdic.SetFocus();
				m_report_tkdic.SetFocusedRow(ofs); //引数で選択行の選択状態を指定できる
				//m_report_tkdic.SetTopRow(ofs); // 指定行を最上段へ

				//m_report_tkdic.Populate();
				//m_report_tkdic.EditItem(ofs, TK_CODE, FALSE);
				*/
				//最下行と挿入行でF6摘要置換をできないようにする。
				if ((focus_line == insNewLine) || (focus_line == endline)) {
					break;
				}

				m_pZm->tkrec->SetAbsolutePosition(focus_line + 1);
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname = teki;
				m_pZm->tkrec->tkana = m_Kana;
				m_pZm->tkrec->Update();

				m_pZm->tkrec->Requery("", 0);

				//PostMessage(WM_USER_1, 0, tkcod );

				if (focus_row == NULL) break; // 選択がない状態の場合

				toprow = m_report_tkdic.GetTopRowIndex();

				int indexRow = focus_row->GetIndex();

				//m_report_tkdic.SetCaption(indexRow, TK_NAME, teki);

				//m_report_tkdic.SetCaption(indexRow, TK_KANA, m_Kana);

				SetItemListTkyo();//レポートコントロールを作り直しているためF6置換後にスクロールしてしまう。これを通ったのちどこにもフォーカスがない状態になる

				//m_report_tkdic.SetFocus();
				//m_report_tkdic.SetFocusedRow(pRow);
				//m_report_tkdic.EditItem(indexRow, TK_NAME, TRUE);
				m_report_tkdic.SetFocus();
				m_report_tkdic.SetTopRow(toprow);// 削除時にスクロールしないよう対応。
				m_report_tkdic.EditItem(focus_line, post_col, TRUE);

				return TRUE;
			//}
			}
		}
		/*
		else {
			m_report_tkdic.SetFocus();
			m_report_tkdic.EditItem(0, TK_CODE, TRUE);
			return TRUE;
		}
		*/
	}
	return CBaseDlg::PreTranslateMessage(pMsg);
}


// wp はキーコード
long CTkdicDlg::ComboBoxJob(UINT id, WPARAM wp)
{
	long ret = wp;

	if( id == IDC_COMBO_MEN ) {
		CComboBox* pCmb = &m_ComboMen;

		switch( wp ) {
		case VK_DOWN:
			if( !pCmb->GetDroppedState() ) {
				pCmb->ShowDropDown(TRUE);
			}
			ret = 0;
			break;

		case VK_UP:
			ret = 0;
			break;
		case VK_RETURN:
		case VK_RIGHT:
		case VK_F2:
		case VK_LEFT:
			if( pCmb->GetDroppedState() )
				pCmb->ShowDropDown(FALSE);
			break;
		}
	}

	return ret;
}


// 最終データ取得
void CTkdicDlg::LastDataGet()
{
	Update_Tkrec(-1);
}


void CTkdicDlg::OnButtonFuri()
{
	char teki[60] = {0};
	m_Edit.get_dictky( teki, sizeof teki );

	if( teki[0] != '\0' ) {
		CRevConv rv;
		DWORD opt;
		opt = RVF_KATAKANA | RVF_HANKAKU;
		CString kana;

		int idx = 0;

		char buf[128] = {0};
		strcpy_s( buf, sizeof buf, teki );

		if( rv.GetFurigana( opt, CString(buf), kana ) == 0 ) {
			kana_chk( &kana, kana.GetLength() );
			furikana_chk( m_Kana, sizeof m_Kana, kana );
			VARIANT var;
			var.pbVal = (BYTE*)m_Kana;
			DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
}

//	科目設定のサインを取得する
//
//
//
int CTkdicDlg::KamokuOwnTblCheck()
{
	BOOL bOwnOpen = FALSE;
	CString filter;

	filter = _T("apl_name = 'KMKAPP8' AND itm_name = 'KMKAPP8' AND u_id = 0" );

	// COWNTB OpenされていなければOPEN
	if( m_pZm->owntb == NULL ) {
		if( m_pZm->OwnTblOpen(filter) == ERR )
			return -1;
		bOwnOpen = TRUE;
	}
	else {
		m_pZm->owntb->Requery(filter);
	}

	if( m_pZm->owntb->st != -1 ) {

		long sign = m_pZm->owntb->vl[19];

		if( sign != -1 ) {
			//科目設定で設定OFF
			if( !(sign & 0x04)) {
				m_bSyzInp = FALSE;
			}
		}
	}
	if( bOwnOpen ) {
		m_pZm->OwnTblClose();
	}

	return 0;
}
//----------->摘要全削除対応2014.11.20
//=============================================================
// 指定の摘要コードの関連する情報を変更又は削除を行う
//		int		OldCode		摘要コード(変更前)
//		int		NewCode		摘要コード(変更後)
//		int		mode		変更モード	0:削除　1:コード変更
//	-----------------------------------------------------
//		返送値		0		正常
//					-1		エラー
//=============================================================
int CTkdicDlg::Change_AllTKInfo(int OldCode, int NewCode, int mode)
{

	int st = 0;
	CString cmd;
	m_pZm->dcntl->Requery("itm = 'rtrec'");
	int rt_num = m_pZm->dcntl->cnt;//削除前の科目対応摘要登録数

	m_pZm->m_database->BeginTrans();//新規トランザクションを開始

	//++++++++++++++++++++++++++++++++削除モード+++++++++++++++++++++++++++++++++++++++//
	if (mode == 0) {
		try {
			// 科目対応摘要
			if (Tk_UseItem & 0x01) {
				cmd.Format("delete from dbo.rtrec where rtcod = '%d'", OldCode);
				m_pZm->m_database->ExecuteSQL(cmd);

				//登録数更新
				cmd.Empty();
				cmd.Format("update dbo.dcntl set cnt = (select count(*) from rtrec) where seq = '15'");
				m_pZm->m_database->ExecuteSQL(cmd);


			}

			
			// 仕訳対応摘要
			if (Tk_UseItem & 0x02) {
				cmd.Format("delete from dbo.strec where stcod = '%d'", OldCode);
				m_pZm->m_database->ExecuteSQL(cmd);

				//登録数更新
				cmd.Empty();
				cmd.Format("update dbo.dcntl set cnt = (select count(*) from strec) where seq = '26'");
				m_pZm->m_database->ExecuteSQL(cmd);

			}


			
		}
		catch (CDBException* dbe) {
			st = -1;
			ErrMsg = dbe->m_strError;
			m_pZm->m_database->Rollback();
			dbe->Delete();
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//


	/*//++++++++++++++++++++++++++++++++コード変更モード++++++++++++++++++++++++++++++++++++++++//
	else if (mode == 1) {
		try {
			// 科目対応摘要
			if (Tk_UseItem & 0x01) {
				cmd.Format("update dbo.rtrec set rtcod = '%d' where rtcod = '%d'", NewCode, OldCode);
				m_pZm->m_database->ExecuteSQL(cmd);
			}

			// 仕訳対応摘要
			if (Tk_UseItem & 0x02) {
				cmd.Format("update dbo.strec set stcod = '%d' where stcod = '%d'", NewCode, OldCode);
				m_pZm->m_database->ExecuteSQL(cmd);
			}

		}
		catch (CDBException* dbe) {
			st = -1;
			ErrMsg = dbe->m_strError;
			m_pZm->m_database->Rollback();
			dbe->Delete();
		}
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//*/

		m_pZm->m_database->CommitTrans();

		//pZmSub->m_database->CommitTrans();

		//159577 159582
		if (mode == 0 && Tk_UseItem & 0x01) {
			int del_num = 0;
			m_pZm->dcntl->Requery("itm = 'rtrec'");
			int cnt = m_pZm->dcntl->cnt;

			m_pZm->rtrec->Requery("");
			int rpg = -1;
			int rseq = -1;
			for (int i = 0; i < m_pZm->dcntl->cnt; i++) {
				if (m_pZm->rtrec->SetAbsolutePosition(i + 1) == -1) break;

				if (m_pZm->rtrec->rtseq == -1) break;
				if (m_pZm->rtrec->rtpgn != rpg) {
					rpg = m_pZm->rtrec->rtpgn;
					rseq = 1;
				}
				if (m_pZm->rtrec->rtseq != rseq) {
					long rcd = m_pZm->rtrec->rtcod;
					m_pZm->rtrec->Delete(rpg, m_pZm->rtrec->rtseq);
					m_pZm->rtrec->Append(rpg, rseq, rcd);
					m_pZm->rtrec->Requery("");
					del_num++;
					if (del_num == (rt_num - cnt)) break;
				}
				rseq++;
			}
		}
		return st;
	//	func();
	return st;
}


void CTkdicDlg::OnBnDel()
{
	if (!(m_pZm->zvol->tl_cor & 0x10))	return;

	ICSReportRecords*		ICSRRs = m_report_tkdic.GetRecords();
	ICSReportRecord*		ICSRR;

	ICSReportRows* rows = m_report_tkdic.GetRows();
	ICSReportRow* row;

	ICSReportRow *focus_row = m_report_tkdic.GetFocusedRow();
	int		cnt;
	CString str;
	CString list_str;
	int code;

	int index = 0;

	str.Format("");

	if (focus_row == NULL) {
		return; // 選択がない状態の場合
	}
	int focus_index = focus_row->GetIndex();
	ICSReportColumn* focus_column = m_report_tkdic.GetFocusedColumn();

	int focus_col = 0;

	if (focus_column == NULL)	focus_col = 0;
	else						focus_col = focus_column->GetIndex();


	int total = m_report_tkdic.GetSelectedRowsCount();
	if (total < 1) return;

	BOOL add_selected = m_report_tkdic.IsContainSelectedRow(addNewLine);
	BOOL ins_selected = m_report_tkdic.IsContainSelectedRow(insNewLine);

	if ((add_selected || ins_selected) && total == 1) {
		post_row = focus_index;
		post_col = focus_col;
		return;
	}

	post_row = m_report_tkdic.GetFocusedRow()->GetIndex();

	m_pZm->tkrec->SetAbsolutePosition(post_row + 1);
	CString tcstr;
	tcstr.Format("%6d", m_pZm->tkrec->tkcod);
	m_report_tkdic.SetCaption(post_row, TK_CODE, tcstr);

	if (add_selected) {
		ICSReportRow* add_row = rows->GetAt(addNewLine);
		ICSReportSelectedRows *selected_rows = m_report_tkdic.GetSelectedRows();
		selected_rows->Remove(add_row);
		total = m_report_tkdic.GetSelectedRowsCount();
	}
	if (ins_selected) {
		ICSReportRow* ins_row = rows->GetAt(insNewLine);
		ICSReportSelectedRows *selected_rows = m_report_tkdic.GetSelectedRows();
		selected_rows->Remove(ins_row);
		total = m_report_tkdic.GetSelectedRowsCount();
	}


	int st = 0;
	int x = 0;
	int totalcount = 0;

	long toprow = 0;

	BOOL remake = FALSE;

	toprow = m_report_tkdic.GetTopRowIndex();

	if (total > 1) {
		//str.Format("選択された%d個の摘要を削除します。", total);
		str.Format("選択された%d個の摘要を削除します。科目対応摘要、仕訳対応摘要、枝番摘要名称が削除されます。\n処理を継続しますか？\n(削除後は元に戻すことはできませんので注意してください。)", total);
		st = ICSMessageBox(str, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2, 0, 0, this);
		//if (ICSMessageBox(str, MB_YESNO, 0, 0, this) == IDYES) {

		if (st == IDYES || st == IDOK) {

			remake = TRUE;

			for (x = 0; x < rows->GetCount(); x++) {

				list_str.Empty();

				if (m_report_tkdic.IsContainSelectedRow(x) == TRUE){

					list_str = m_report_tkdic.GetCaption(x, TK_CODE);

					code = atoi(list_str);
					//DeleteTkrec(code);
					
					//削除判定条件を修正
					
					if (CheckUseTkcod(code)) {

				        pTkjour->DeleteTklnkjour(code);

						Change_AllTKInfo(code, code, DELMODE);
					}

					DeleteTkrec(code);

					m_pZm->tkrec->Requery("", 0);//DeleteTkrec後仕訳摘要モジュール内でFind()する際に必ず必要

					totalcount++;
				}

				if (totalcount == total) {
					index = x;
					break;
				}
			}

			focus_index = x - totalcount + 1;
		}
	}

	else /*if (focus_col == TK_CODE )*/ {	// 摘要を削除// カナ　摘要名称欄で削除できるように修正。
		list_str = m_report_tkdic.GetCaption(focus_index, TK_CODE);
		code = atoi(list_str);

		if (code == 0) {
			/*
			m_pZm->tkrec->SetAbsolutePosition(focus_index + 1);
			CString tcstr;
			tcstr.Format("%6d", m_pZm->tkrec->tkcod);
			m_report_tkdic.SetCaption(focus_index, TK_CODE, tcstr);
			*/
			return;
		}

		BOOL bUsed = FALSE;

		if (CheckUseTkcod(code) == TRUE) {

			str.Empty();
			//科目対応摘要
			str.Format("摘要[%s]は、", list_str);

			if (Tk_UseItem & 0x01) {
				str += "【科目対応摘要】";
			}
			//仕訳対応摘要
			if (Tk_UseItem & 0x02) {
				str += "【仕訳対応摘要】";
			}

			//科目枝番
			if (Tk_UseItem & 0x04) {
				str += "【科目枝番名称】";
			}
			//部門枝番
			if (Tk_UseItem & 0x10) {
				str += "【部門枝番名称】";
			}
			//if (CKmkBaseDlg::SyusiMasterFlag != FALSE) {//収支の場合
			if (Tk_UseItem & 0x08) {//収支科目枝番
				str += "【収支科目枝番名称】";
			}
			if (Tk_UseItem & 0x20) {//収支部門枝番
				str += "【収支部門枝番名称】";
			}
			if (Tk_UseItem & 0x40) {
				str += "【自動仕訳】";
			}
			//}
			bUsed = TRUE;

			str += "で使用されています。\n削除処理を継続しますか？";

			st = ICSMessageBox(str, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2, 0, 0, this);
		}

		else {
			str.Format("摘要[%s]を削除します", list_str);
			st = ICSMessageBox(str, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2, 0, 0, this);
		}

		if (st == IDYES || st == IDOK) {
			remake = TRUE;
		//	m_pZm->tkrec->SetAbsolutePosition(focus_index + 1);
		//	if (CheckUseTkcod(code)) {// 2回通す意味ある？
			if( bUsed ) {
				if( Tk_UseItem & 0x40 ) {
					DeleteTklnkjour(code);
				//	pTkjour->DeleteTklnkjour(code);
				}
				Change_AllTKInfo(code, code, DELMODE);
			}
			//toprow = m_report_tkdic.GetTopRowIndex();
			DeleteTkrec(code);
			m_report_tkdic.RemoveRecordAt(focus_index);
		}
		
	}

	m_TbCust.clear();
	m_pZm->tkrec->Requery("", 0);
	//MakeTKdicFrm();

	if (remake) {// いいえ選択時はレポートコントロールを作り直さない。
		SetItemListTkyo();
		
		m_report_tkdic.SetTopRow(toprow);// 削除時にスクロールしないよう対応。
		
		TorokDisp();
	}
	m_report_tkdic.SetFocus();
	m_report_tkdic.EditItem(focus_index, TK_CODE, TRUE);
	
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


//モジュールだとエラーになった。
int CTkdicDlg::DeleteTklnkjour(int tkcod)
{
	if( m_pZm->tkrec->tkcod != tkcod ) {
		if( m_pZm->tkrec->Find(tkcod) <= 0 ) {
			return -2;
		}
	}
	int tklnkseq = -1;
	if( (tklnkseq = m_pZm->tkrec->tklnkseq) == -1 ) {
		return 0;//リンクなし
	}
	CString filter, jourfil;
	filter.Format("tklnkseq = %d", tklnkseq);
	if( m_pZm->tklnk->Requery(filter) == -1 ) {
		return 0;
	}
	if( m_pZm->tklnk->MoveLast() == -1 ) {
		return 0;
	}
	while( 1 ) {
		int tkjourseq = m_pZm->tklnk->tkjourseq;
		jourfil.Format("tkjourseq = %d", tkjourseq);

		if( m_pZm->tkjour->Requery(jourfil) != -1 ) {
			if( m_pZm->tkjour->MoveLast() != -1 ) {
				while( 1 ) {
					m_pZm->tkjour->Delete();
					m_pZm->tkjour->Requery(jourfil);
					if( m_pZm->tkjour->st == -1 )
						break;
					if( m_pZm->tkjour->MovePrev() == -1 ) {
						break;
					}
				}
			}
		}
		m_pZm->tklnk->Delete();
		if( m_pZm->tklnk->Requery(filter) == -1 ) {
			break;
		}
		if( m_pZm->tklnk->MovePrev() == -1 ) {
			break;
		}
	}

	m_pZm->tkrec->Edit();
	m_pZm->tkrec->tklnkseq = -1;
	m_pZm->tkrec->Update();

	return 0;
}



void CTkdicDlg::OnBnIns()
{
	
	//挿入 insert 押下

	//addNewLine = get_tknum(m_pZm);
	if (!(m_pZm->zvol->tl_cor & 0x10))		return;

	ICSReportRows* rows = m_report_tkdic.GetRows();

	ICSReportRow *focus_row = m_report_tkdic.GetFocusedRow();
	if (focus_row == NULL) return; // 選択がない状態の場合

	int focus_index = focus_row->GetIndex();

	ICSReportRecord*		pRecord;
//	ICSReportRecordItem*	pItem;
	pRecord = new ICSReportRecord();

	if (insNewLine == -1) {
		//最終行で追加を押されて場合
		if (addNewLine == focus_index) {
			m_report_tkdic.SetFocus();
			m_report_tkdic.EditItem(addNewLine, TK_CODE, TRUE);
			return;
		}
		//最終行の新規行を消す
		m_report_tkdic.RemoveRecordAt(addNewLine);
		addNewLine = -1;

		// リストに追加
		m_report_tkdic.InsertRecordAt(focus_index, pRecord);
		insNewLine = focus_index;
	}
	else {
		//挿入を終わり　新規行を作る
		
		m_report_tkdic.RemoveRecordAt(insNewLine);

		//ここでaddNewLine=-1なら　挿入行ありの状態で挿入が押された場合　!=-1なら　挿入行有りの状態で追加を押された場合
		if (insNewLine == -1 && addNewLine != -1)	focus_index = m_report_tkdic.GetRecords()->GetCount();
		else										focus_index = insNewLine;

		insNewLine = -1;
		addNewLine = m_report_tkdic.GetRecords()->GetCount();
		m_report_tkdic.InsertRecordAt(addNewLine, pRecord);
		//		focus_index = insNewLine; 
	}
	

	SetReportRecord(pRecord);// 行の追加　新たに関数を追加するべき


	ICSColorInfo info;
	((ICSWinApp *)AfxGetApp())->GetStanderdColor(info);
	COLORREF ONCOLOR = info.ViewColor();

	m_report_tkdic.Populate();
	m_report_tkdic.SetFocus();
	//m_report_tkdic.SetFocusedRow(focus_index);
	m_report_tkdic.EditItem(focus_index, TK_CODE, TRUE);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CTkdicDlg::OnNMReportEditkeydownREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn = pItemNotify->pColumn;
	int	indexColumn = pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow = pItemNotify->pRow;
	int	indexRow = pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem = pItemNotify->pItem;
	int	indexItem = pItem->GetIndex();
	//ターミネーションキーコード
	UINT	nChar = pItemNotify->nChar;


	//--->先頭インデックス取得2015.10.
	long toprow = 0;
	toprow = m_report_tkdic.GetTopRowIndex();
	//<---------------------------------
	

	char dmmy[512] = { 0 }, kk[128] = { 0 };
	int i = 0, k = 0, sel_num = 0, set_c = 0, len = 0, ch_at = 0;
	CString cod_str, cod_str2;
	long	cod = 0;

	CString list_str, strch, Clear_str;
	list_str.Empty();
	Clear_str.Empty();
	DWORD_PTR f_code;
	int select = 0, check_cod = 0, str_len = 0;
	long ny_cod = 0, kmk_cod = 0;

	int kana_flg = 0, rdw_flg = 0;

	int clm = 0;

	PosCngChk = 0;

	int ofs = 0;
	   
	//editkeydown中にメッセージを表示させると
	edit_keydown_sw = 1;

	switch (pItemNotify->nChar) {
	case VK_RETURN:
	case VK_TAB:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_DOWN:
	case VK_UP:
		list_str = pItem->GetCaption(pColumn);	//リスト内選択文字の取得
		sprintf_s(kk, sizeof(kk), "%s", list_str);
		str_len = list_str.GetLength();

		CString tkstr;
		//摘要コード

		if (indexColumn == TK_CODE) {
			ny_cod = atoi(list_str);

			//ny_cod = m_pZm->tkrec->tkcod;
			m_pZm->tkrec->Requery("", 0);

			//空行への追加
			if ((indexRow == insNewLine) || (indexRow == addNewLine)) {

				if (str_len == 0) {									//番号欄が空白+Enterキー押下
					if (nChar == VK_RETURN) {
						if (m_pZm->tkrec->st == -1)	ny_cod = 1;		//摘要レコードにデータがないとき１を自動付番
						else {
							if (m_pZm->tkrec->MoveLast() == ERR) break;
							ny_cod = m_pZm->tkrec->tkcod + 1;
							if (ny_cod > 999999) break;
						}
					}
					else break;
				}


				m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);
				long tk_org = m_pZm->tkrec->tkcod;
				tkstr.Format("%6d", tk_org);


				if (ny_cod <= 0) {
					m_report_tkdic.SetCaption(indexRow, TK_CODE, Clear_str);
					post_row = indexRow;
					post_col = TK_CODE;
					m_report_tkdic.EditItem(indexRow, post_col, TRUE);

					return;
				}

				if (!tkyngetr(ny_cod, m_pZm)) {
					ICSMessageBox("指定された番号は登録されています。", 0, 0, 0, this);

					//tkstr.Format("");

					m_report_tkdic.SetFocus();

					if (insNewLine != -1){ //|| (indexRow == addNewLine)) {
						m_report_tkdic.SetCaption(indexRow, TK_CODE, Clear_str);
					}
					else {
						//160476対応検証
						SetItemListTkyo();

					}
					//m_report_tkdic.SetCaption(indexRow, TK_CODE, Clear_str); // 検証のため一時的に削除
					m_report_tkdic.EditItem(indexRow, TK_CODE, TRUE);
					return;

					//m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);
				}
				else {

					map<long, CTKDATA_REC >::iterator it = m_TbCust.find(ny_cod);

					
					CTKDATA_REC tkdata_rec;
					map_struct_clear(&tkdata_rec);
					tkdata_rec.tkana.Empty();
					tkdata_rec.tksgn.Empty();
					tkdata_rec.tkname.Empty();
					tkdata_rec.tkcod = ny_cod;


					m_TbCust.insert(pair<long, CTKDATA_REC>(ny_cod, tkdata_rec));
				}
			}
			else {
				m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);
				long tk_org = m_pZm->tkrec->tkcod;
				tkstr.Format("%6d", tk_org);

				m_report_tkdic.SetCaption(indexRow, TK_CODE, tkstr);

				m_report_tkdic.EditItem(indexRow, indexColumn, TRUE);


				/*摘要コードを変更できないようにする*/ // 220823
				if (str_len == 0 || ny_cod <= 0) {
					m_report_tkdic.SetCaption(indexRow, TK_CODE, tkstr);
					post_row = indexRow;
					post_col = TK_CODE;
					m_report_tkdic.EditItem(indexRow, post_col, TRUE);
					return;
				}
				
				
				else if (tk_org == ny_cod) break;

			}
			
			if ((insNewLine != -1) || (indexRow == addNewLine)) {
				//m_pZm->tkrec->Requery("", 0);
				m_pZm->tkrec->AddNew();
				m_pZm->tkrec->tkname.Format("%s", "");
				m_pZm->tkrec->tkana.Format("%s", "");
				m_pZm->tkrec->dsign.SetSize( SIZE_DSIGN );
				for( int idx = 0; idx < SIZE_DSIGN; idx++ ) {
					m_pZm->tkrec->dsign.SetAt( idx, 0 );
				}

				m_pZm->tkrec->tkcod = ny_cod;
				if (m_pZm->tkrec->Update() == -1) {
				}

				m_pZm->tkrec->Requery("", 0);

				ofs = GetTableOfset(ny_cod);

				if (ofs == -1) {//2022/11/08

					ofs = indexRow;
				}
				m_bDBupdate = TRUE;

				PosCngChk = 1;//追加時ポジション変更サインON
			}
			
			if (PosCngChk) {
				
				//MakeTKdicFrm();
				SetItemListTkyo();

				//int ofs = GetTableOfset(m_pZm->tkrec->tkcod); //m_pZm->tkrec->tkcod 検索した後に表示される分しか入ってない　20220823
				m_report_tkdic.SetFocus();

				//m_report_tkdic.SetFocusedRow(ofs); //引数で選択行の選択状態を指定できる
				m_report_tkdic.SetTopRow(ofs); // 指定行を最上段へ

				//
				//m_report_tkdic.EditItem(ofs, TK_CODE, FALSE);

				//----->保存しておいた先頭インデックスのポジションにセット2015.10.21
				//m_report_tkdic.SetTopRow(toprow);
				m_report_tkdic.EditItem(ofs, TK_NAME, TRUE);


				//<-----------------------------------------------------------------
				//m_report_tkdic.Populate();
				
			//m_report_tkdic.SetFocus();
			}
			//}

		}
		else if (indexColumn == TK_NAME) {
			list_str = pItem->GetCaption(pColumn);	//リスト内選択文字の取得
			sprintf_s(kk, sizeof(kk), "%s", list_str);
			str_len = list_str.GetLength();

			m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);

			CString kstr, check_str;

			kstr.Format("%s", list_str);
			if (indexColumn == TK_NAME) check_str.Format("%s", m_pZm->tkrec->tkname);

			if (check_str.Compare(kstr) || (str_len != list_str.GetLength())) {

				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname.Format("%s", list_str);
				m_pZm->tkrec->Update();
				m_pZm->tkrec->Requery("", 0);

			}
		}

		else if (indexColumn == TK_KANA) {

			//if (len == 0) break;
			sprintf_s(kk, sizeof(kk), "%s", list_str);

			m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);
			if (m_pZm->tkrec->tkana.Compare(kk) == 0) {
				break;
			}

			m_report_tkdic.SetCaption(indexRow, TK_KANA, list_str);
			//				SetCTKDATA_REC(pZmSub->tkrec, TKDIC_KANA);

			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkana.Format("%s", list_str);
			m_pZm->tkrec->Update();
			m_pZm->tkrec->Requery("", 0);

		}
		// インボイス対応
		else if( indexColumn == TK_INVNO ) {
			// 登録番号
			list_str = pItem->GetCaption( pColumn );
			if( !list_str.IsEmpty() ) {
				// 前に「T」をセット
				list_str.Format( _T( "T%s" ), pItem->GetCaption( pColumn ));
			}

			m_pZm->tkrec->SetAbsolutePosition( indexRow + 1 );
			str_len = list_str.GetLength();
			if( str_len > 14 ) {
				str_len = 14;
			}

			if( str_len > 0 ) {
				// 登録番号のチェック
				if( syzInvoice.CheckInvoiceNumber( list_str ) == -1 ) {
				//	list_str.Empty();
				//	m_report_tkdic.SetCaption( indexRow, indexColumn, list_str );
					ICSMessageBox( syzInvoice.GetErrorMessage(), MB_ICONSTOP );
					m_report_tkdic.SetFocus();
					m_report_tkdic.EditItem( indexRow, indexColumn, TRUE );
					return;
				}
			}

			CString kstr, check_str;
			kstr = list_str;
			check_str = m_pZm->tkrec->invno;

			if(( check_str.Compare( kstr )) || ( str_len != list_str.GetLength() )) {
				// 変更あり
				kspTrim( kstr );
				kstr.Replace( _T( "T" ), _T( "" ));

				if( kstr.IsEmpty() ) {
					list_str.Empty();
				}

				// 更新
				m_pZm->tkrec->invno = list_str;
				int st = UpdateTkrecEx( m_pZm->tkrec );

				if( st == -1 ) {
					SetReportPara();
				}
				else {
					SetCTKDATA_REC( m_pZm->tkrec, TK_INVNO );
					m_report_tkdic.SetCaption( indexRow, TK_INVNO, list_str );

					m_pZm->tkrec->Requery( _T( "" ), 0 );
					m_report_tkdic.SetFocus();
				}
			}
			else {
				// 変更なし
				if(( pItemNotify->nChar == VK_UP ) || ( pItemNotify->nChar == VK_DOWN )) {
					m_report_tkdic.SetCaption( indexRow, TK_INVNO, list_str );
				}
			}
		}
		break;
	}

	switch (pItemNotify->nChar) {
	case VK_RETURN:
	case VK_RIGHT:

		if (PosCngChk) {

			PosCngChk = 0;
			break;
		}

		if (GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_TAB) < 0) {
			SetPrevControl(indexRow, indexColumn, TRUE);
		}

		SetNexControl(indexRow, indexColumn, TRUE);// 品質アップ時点での処理

		//else {
			//SetNexControl(indexRow, indexColumn, FALSE);
		//}

		break;

	case VK_TAB:

		SetNexControl(indexRow, indexColumn, TRUE);// 品質アップ時点での処理
		//一行選択状態にし、タブをきかないようにする2015.02.03
		//m_report_tkdic.EditItem(indexRow, indexColumn, FALSE);
		break;

	case VK_LEFT:      
		SetPrevControl(indexRow, indexColumn, TRUE);
		break;
	case VK_DOWN:
		//行全体だけでなくカラムにフォーカスがある場合もshift+↓で複数範囲を指定できるよう修正
		//m_report_tkdic.SetFocus();
	
		if ((indexRow + 1) >= m_report_tkdic.GetRecords()->GetCount()) {
			m_report_tkdic.EditItem(indexRow, indexColumn, TRUE);
		}
		else {
			if (GetKeyState(VK_SHIFT) < 0) {

				m_report_tkdic.EditItem(indexRow , indexColumn, FALSE);

				/*
				ICSReportRows* rows = m_report_tkdic.GetRows();
				ICSReportRow* row = rows->GetAt(indexRow +1 );


				///shift + ↓で落ちる検証　科目設定より追加/////
				if ((indexRow + 1) == addNewLine) {
					int cnt = m_report_tkdic.GetSelectedRowsCount();
					cnt -= 1;
					if (m_report_tkdic.IsContainSelectedRow(addNewLine))
						cnt -= 1;
					m_report_tkdic.SetSelectedRows(indexRow - cnt, indexRow);
					m_report_tkdic.EditItem(indexRow, indexColumn, TRUE);
					break;
				}
				else if ((indexRow + 1) > addNewLine) {
					break;
				}
				////ここまで追加分//////



				if (row->IsSelected() == TRUE) {//次の行がすでに選択されていた場合
					ICSReportRow* row2 = rows->GetAt(indexRow);
					row2->SetSelected(FALSE);
				}
				row->SetSelected(TRUE);
				*/
				//m_report_tkdic.SetFocusedRow(indexRow + 1, TRUE);
				
	
			}
			else {
				m_report_tkdic.EditItem(indexRow + 1, indexColumn, TRUE);
			}
		}

		break;

	case VK_UP:
		//行全体だけでなくカラムにフォーカスがある場合もshift+↑で複数範囲を指定できるよう修正
		m_report_tkdic.SetFocus();
		if (indexRow == 0)	m_report_tkdic.EditItem(indexRow, indexColumn, TRUE);
		else {
			if (GetKeyState(VK_SHIFT) < 0) {
				m_report_tkdic.EditItem(indexRow, indexColumn, FALSE);
				/*
				ICSReportRows* rows = m_report_tkdic.GetRows();
				ICSReportRow* row = rows->GetAt(indexRow - 1);
				if (row->IsSelected() == TRUE) {//次の行がすでに選択されていた場合
					ICSReportRow* row2 = rows->GetAt(indexRow);
					row2->SetSelected(FALSE);
				}
				row->SetSelected(TRUE);
				m_report_tkdic.SetFocusedRow(indexRow - 1, TRUE);

				*/
			}

			else {

				m_report_tkdic.EditItem(indexRow - 1, indexColumn, TRUE);
			}
		}
		break;
	case VK_NEXT:
		m_report_tkdic.MovePageDown(FALSE, FALSE);
		break;
	case VK_PRIOR:
		m_report_tkdic.MovePageUp(FALSE, FALSE);
		break;


	case VK_DELETE:
		m_report_tkdic.SetCaption(indexRow, indexColumn, "");
		break;

	default:
		break;

	}

	edit_keydown_sw = 0;
	
}



void CTkdicDlg::SetNexControl(int s_row, int s_clm, BOOL s_edit)
{
	int set_c = 0;
	set_c = s_clm;

	if (set_c == TK_CODE) {
		post_row = s_row;
		post_col = TK_NAME;
	}

	if (set_c == TK_NAME) {
		//CSV摘要ありなら
		post_row = s_row;
		post_col = TK_KANA;
	}

	if (set_c == TK_KANA) {
		if( m_bInvMaster && m_bInvUse && m_syzKubn && m_bSyzInp ) {
			int sgn = m_pZm->tkrec->tksgn.GetAt( 1 );
			if(( sgn == 2 ) || ( sgn == 3 )) {
				post_row = s_row;
				post_col = TK_SYOHI;
			}
			else {
			post_row = s_row;
			post_col = TK_KUBUN;
		}
		}
		else if (m_bSyzInp) {
			post_row = s_row;
			post_col = TK_SYOHI;
		}
		else if ((s_row + 1) > m_report_tkdic.GetRecords()->GetCount()) {
			post_row = s_row;
			post_col = TK_CODE;
		} 

		else {
			post_row = s_row + 1;
			post_col = TK_CODE;
		}

	}

	if( set_c == TK_KUBUN ) {
		post_row = s_row;
		post_col = TK_SYOHI;
	}

	if (set_c == TK_SYOHI) {
		if ((s_row + 1) > m_report_tkdic.GetRecords()->GetCount()) {
			post_row = s_row;
			//post_col = TK_CODE;

			// インボイス対応
			if( m_bInvMaster && m_bInvUse ) {
				if( m_bInvDisp ) {
					post_col = TK_INVNO;
				}
				else {
					int sgn = m_pZm->tkrec->tksgn.GetAt( 1 );
					if(( sgn == 2 ) || ( sgn == 3 )) {
						post_row = s_row + 1;
						post_col = TK_CODE;
					}
					else {
						post_col = TK_MENZEI;
					}
				}
			}
			else {
				post_col = TK_CODE;
			}
		}
		else {
			// インボイス対応
			if( m_bInvMaster && m_bInvUse ) {
				post_row = s_row;
				if( m_bInvDisp ) {
					post_col = TK_INVNO;
				}
				else {
					int sgn = m_pZm->tkrec->tksgn.GetAt( 1 );
					if(( sgn == 2 ) || ( sgn == 3 )) {
						post_row = s_row + 1;
						post_col = TK_CODE;
					}
					else {
						post_col = TK_MENZEI;
					}
				}
			}
			else {
				post_row = s_row + 1;
				post_col = TK_CODE;
			}
		}
	}

	// インボイス対応
	if( set_c == TK_INVNO ) {
		// 登録番号
		int sgn = m_pZm->tkrec->tksgn.GetAt( 1 );
		if(( sgn == 2 ) || ( sgn == 3 )) {
			post_row = s_row + 1;
			post_col = TK_CODE;
		}
		else {
			post_row = s_row;
			post_col = TK_MENZEI;
		}
	}

	if( set_c == TK_MENZEI ) {
		// 免税事業者からの課税仕入れサイン
		post_row = s_row + 1;
		post_col = TK_CODE;
	}



	//m_report_tkdic.EditItem(post_row, post_col, TRUE); //品質アップ時点での処理
	PostMessage( WM_MYMESSAGE, EDIT_IN, 0 );


	/*
	if(s_edit == FALSE){

		m_report_tkdic.EditItem(post_row, post_col, FALSE);
	}
	else {
		m_report_tkdic.EditItem(post_row, post_col, TRUE);
	}
	*/
	
}


void CTkdicDlg::SetPrevControl(int s_row, int s_clm, BOOL s_edit)
{
	int set_c = 0;
	set_c = s_clm;

	if (set_c == TK_CODE) {
		if (s_row == 0) {
			//post_row = s_row;
			post_col = TK_CODE;
		}
		else {
			s_row--;
			//set_c = TK_SYOHI;
			// インボイス対応
			//post_col = TK_SYOHI;//11/30左キーを押したときの動作がおかしいのを修正。
			//post_row = s_row;
			//m_report_tkdic.EditItem(s_row, post_col, s_edit);

			if( m_bInvMaster && m_bInvUse && m_bSyzInp ) {
				m_pZm->tkrec->SetAbsolutePosition( s_row + 1 );
				if( m_pZm->tkrec->st != -1 ) {
					int sgn = m_pZm->tkrec->tksgn.GetAt( 1 );
					if(( sgn == 2 ) || ( sgn == 3 )) {
						post_col = TK_INVNO;
					}
					else {
						post_col = TK_MENZEI;
					}
				}
				else {
					post_col = TK_CODE;
				}
			}
			else if( m_bSyzInp ) {
				post_col = TK_SYOHI;
			}
			else {
				post_col = TK_KANA;
			}
		}
	}

	if( set_c == TK_SYOHI ) {
		if(( m_syzKubn == 2 ) || ( m_syzKubn == 3 )) {
			post_col = TK_KUBUN;
		}
		else {
		post_col = TK_KANA;
		}
	}
	
	if (set_c == TK_KANA) {
		//post_row = s_row;
		post_col = TK_NAME;
	}

	if (set_c == TK_NAME) {
		//post_row = s_row;
		post_col = TK_CODE;
		//m_report_tkdic.EditItem(s_row, TK_CODE, s_edit);
	}

	if( set_c == TK_KUBUN ) {
		post_col = TK_KANA;
	}

	// インボイス対応
	if( set_c == TK_INVNO ) {
		post_col = TK_SYOHI;
	}

	if( set_c == TK_MENZEI ) {
		if( m_bInvDisp ) {
			post_col = TK_INVNO;
		}
		else {
			post_col = TK_SYOHI;
		}
	}

	post_row = s_row;
	//m_report_tkdic.SetFocusedRow(s_row);
	//m_report_tkdic.EditItem(post_row, post_col, TRUE);
	PostMessage( WM_MYMESSAGE, EDIT_IN, 0 );
}


//map操作
void CTkdicDlg::map_struct_clear(CTKDATA_REC* tkdata_rec)
{
	ASSERT( tkdata_rec );
	if( tkdata_rec == NULL ) {
		return;
	}

	tkdata_rec->tkcod = 0;
	tkdata_rec->tkana.Empty();
	tkdata_rec->tksgn.Empty();
	tkdata_rec->tkname.Empty();
	tkdata_rec->tkari.Empty();
	tkdata_rec->tkasi.Empty();
	tkdata_rec->bktkwd.Empty();
	tkdata_rec->karicod = 0;
	tkdata_rec->kasicod = 0;
	tkdata_rec->tel.Empty();
	tkdata_rec->multi.Empty();
	tkdata_rec->invno.Empty();
	tkdata_rec->taxfree.Empty();
	tkdata_rec->dsign.Empty();
}

void CTkdicDlg::OnNMReportEditimeendREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn = pItemNotify->pColumn;
	int	indexColumn = pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow = pItemNotify->pRow;
	int	indexRow = pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem = pItemNotify->pItem;
	int	indexItem = pItem->GetIndex();
	//ターミネーションキー
	UINT	nChar = pItemNotify->nChar;
	//IME文字列
	CString	strIME = pItemNotify->strIME;
	//IME読み文字列
	CString	strRead = pItemNotify->strYomi;


	CString list_str;
	int ln = 0;

	//名称を打たれた時カナになにもなければ書く
	if (indexItem == TK_NAME) {
		list_str = m_report_tkdic.GetCaption(indexRow, TK_KANA);

		if ((list_str.Compare(_T("    ")) == 0) || list_str.IsEmpty()) {

			CString str;
			str.Format("%s", strRead);
			ln = str.GetLength();
			if (ln > 4) ln = 4;

			TCHAR c;
			TCHAR ank[6];
			int i, j;
			for (i = j = 0; i < ln; i++) {
				c = str.GetAt(i);
				if (c != '-') {
					if (_cclass(c)) continue;
				}
				if (c == 'ｰ')	c = '-';	// カナ欄に環境依存文字'ｰ'が入っていたのを修正 ('20.07.10 修正依頼No.156416)
				ank[j] = c;
				j++;
			}
			if (j == 0) return;

			ln = j;
			ank[j] = 0;
			str.Format("%s", ank);
			m_report_tkdic.SetCaption(indexRow, TK_KANA, str);

			m_pZm->tkrec->SetAbsolutePosition(indexRow + 1);


			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkana.Format("%s", str);
			m_pZm->tkrec->Update();
			m_pZm->tkrec->Requery("", 0);

		}
	}
}

//===================================================================
// mapの更新
//-------------------------------------------------------------------
// 引数
//		tkrec			:			CTKRECのポインタ
//		SetNo			:			更新項目
//===================================================================
void CTkdicDlg::SetCTKDATA_REC( CTKREC *tkrec, int SetNo )
{
	ASSERT( tkrec );
	if( tkrec == NULL ) {
		return;
	}

	int cnt = 0;
	map<long, CTKDATA_REC >::iterator it;
	for( it = m_TbCust.begin(), cnt = 0; it != m_TbCust.end(); ++it, cnt++ ) {
		if( tkrec->tkcod == it->first ) {
			if( SetNo == TK_NAME ) {
				it->second.tkname.Format( _T( "%s" ), tkrec->tkname  );
			}
			else if(SetNo == TK_KANA ){
				it->second.tkana.Format( _T( "%s" ), tkrec->tkana );
			}
			// インボイス対応
			else if (SetNo == TK_INVNO) {
				it->second.invno.Format( _T( "%s" ), tkrec->invno );
			}
			break;
		}		
	}
}

//===================================================================
// レポートコントロールパラメータ設定
//===================================================================
void CTkdicDlg::SetReportPara()
{
	MakeTKdicFrm();
	SetItemListTkyo();
}

//===================================================================
// CTKRECの更新
//-------------------------------------------------------------------
// 引数
//		pTkrec			:			CTKRECのポインタ
//-------------------------------------------------------------------
// 返送値
//		0				:			正常終了
//		-1				:			エラー
//===================================================================
int CTkdicDlg::UpdateTkrecEx( CTKREC *pTkrec )
{
	ASSERT( pTkrec );
	if( pTkrec == NULL ) {
		return -1;
	}

	long tkcod;
	CString tkana, tkname, invno;
	CByteArray tksgn, dsign;	// 免税・区分

	tkcod = pTkrec->tkcod;
	tkana.Format( _T( "%s" ), pTkrec->tkana );
	tksgn.Add( pTkrec->tksgn.GetAt(0) );
	tksgn.Add( pTkrec->tksgn.GetAt(1) );
	tkname.Format( _T( "%s" ), pTkrec->tkname );
	kspTrim( tkname );


	// インボイス対応
	if( m_bInvMaster && m_bInvUse ) {
		for( int idx = 0; idx < SIZE_DSIGN; idx++) {
			dsign.Add( pTkrec->dsign.GetAt(idx) );
		}

		invno.Format( _T( "%s" ), pTkrec->invno);
	}


	m_pZm->dcntl->Requery( _T( "seq = 7" ));
	m_pZm->tkrec->Requery( _T( "" ), 0 );

	if( GetTableOfset( tkcod ) != -1 ) {

		m_pZm->tkrec->Edit();

		m_pZm->tkrec->tkcod = tkcod;
		m_pZm->tkrec->tkana = tkana;

		m_pZm->tkrec->tksgn.SetAt( 0, tksgn.GetAt( 0 ) );
		m_pZm->tkrec->tksgn.SetAt( 1, tksgn.GetAt( 1 ) );
		m_pZm->tkrec->tkname = tkname;

		if( m_bInvMaster && m_bInvUse ) {
			for( int idx = 0; idx < SIZE_DSIGN; idx++ ) {
				m_pZm->tkrec->dsign.SetAt( idx, dsign.GetAt( idx ));
			}

			m_pZm->tkrec->invno = invno;
		}

		if( m_pZm->tkrec->Update() == -1 ) {
			ICSMessageBox( m_pZm->GetZmErrMsg() );
			return -1;
		}
	}

	return 0;
}

//===================================================================
// スペースカット
//-------------------------------------------------------------------
// 引数
//		OrgStr			:			対象文字列
//===================================================================
void CTkdicDlg::kspTrim( CString &OrgStr )
{
	CString work;
	work.Format( _T( "%s" ), OrgStr );
	int ln = work.GetLength();

	while( ln ) {
		CString wk;
		int cln =0;

		wk.Format( _T( "%s" ), work.Right( 1 ) );
		if( wk.Compare( _T( " " )) == 0 ) {
			cln = 1;
		}
		else if( ln > 1 ) {
			wk.Format( _T( "%s" ), work.Right( 2 ));
			if( wk.Compare( _T( "　" )) == 0 ) {
				cln = 2;
			}
		}

		if( cln ) {
			work.Format( _T( "%s" ), work.Left( ln - cln ) );
			ln -= cln;
		}
		else {
			break;
		}
	}

	OrgStr.Format( _T( "%s" ), work );
}

//===================================================================
// 免税事業者からの課税仕入れサイン
//===================================================================
LRESULT CTkdicDlg::OnMenzeiMessage( WPARAM wParam, LPARAM lParam )
{
	ICSReportRow *pFocusRow = m_report_tkdic.GetFocusedRow();
	ICSReportColumn *pFocusColumn = m_report_tkdic.GetFocusedColumn();

	if( pFocusColumn == NULL ) {
		pFocusColumn = m_report_tkdic.GetColumns()->GetAt( TK_MENZEI );
	}

	if(( pFocusRow == NULL ) || ( pFocusColumn == NULL )) {
		return 0L;
	}

	// 選択行取得
	int focusIndex = pFocusRow->GetIndex();


	CString list_str = m_report_tkdic.GetCaption( focusIndex, TK_CODE );
	int report_tk_cod = atoi( list_str.GetBuffer() );
	map<long, CTKDATA_REC >::iterator it = m_TbCust.find( report_tk_cod );
	if( it == m_TbCust.end() ) {
		return 0;
	}

	if( wParam == 0 ) {
		UINT term = m_SelDlgWnd->SelTerm;
		int selNo = m_SelDlgWnd->Selno;
		int sgn = 0;

		m_SelDlgWnd->DeleteSelDlg();

		if( term == VK_RETURN ) {
			m_pZm->tkrec->SetAbsolutePosition( focusIndex + 1 );

			if( m_pZm->tkrec->dsign[4] & BIT_MENZEI ) {
				sgn = 1;
			}
			else {
				sgn = 0;
			}

			if( sgn != selNo ) {
				int dsign = 0;

				if( selNo ) {
					dsign |= 0x10;
				}

				if( m_pZm->tkrec->dsign.GetAt( 4 ) & 0x0f ) {
					dsign += m_pZm->tkrec->dsign.GetAt( 4 ) & 0x0f;
				}

				m_pZm->tkrec->dsign.SetAt( 4, dsign );
				int st = UpdateTkrecEx( m_pZm->tkrec );
				m_pZm->tkrec->Requery( _T( "" ), 0 );

				if( st != -1 ) {
					CString msg;
					if( selNo ) {
						msg = _T( "○" );
					}
					else {
						msg = _T( "" );
					}
					m_report_tkdic.SetCaption( focusIndex, TK_MENZEI, msg );
					it->second.taxfree = msg;
				}
			}
		}
		SetNexControl( focusIndex, TK_MENZEI, TRUE );
	}
	else if( wParam == 1 ) {
		SetPrevControl( focusIndex, TK_MENZEI, TRUE );
	}
	else if( wParam == 2 ) {
		m_report_tkdic.SetFocus();
		m_report_tkdic.SetFocusedRow( m_report_tkdic.GetRows()->GetAt( focusIndex )); 
	}

	return 0L;
}

//===================================================================
// 仕入/売上区分
//===================================================================
LRESULT CTkdicDlg::OnKubunMessage( WPARAM wParam, LPARAM lParam )
{
	ICSReportRow *pFocusRow = m_report_tkdic.GetFocusedRow();
	ICSReportColumn *pFocusColumn = m_report_tkdic.GetFocusedColumn();

	if( pFocusColumn == NULL ) {
		pFocusColumn = m_report_tkdic.GetColumns()->GetAt( TK_MENZEI );
	}

	if(( pFocusRow == NULL ) || ( pFocusColumn == NULL )) {
		return 0L;
	}

	// 選択行取得
	int focusIndex = pFocusRow->GetIndex();

	CString listStr = m_report_tkdic.GetCaption( focusIndex, TK_CODE );
	int tkCod = atoi( listStr );
	map<long, CTKDATA_REC >::iterator it = m_TbCust.find( tkCod );
	if( it == m_TbCust.end() ) {
		return 0;
	}

	if( wParam == 0 ) {
		UINT term = m_SelDlgWnd->SelTerm;
		int selNo = m_SelDlgWnd->Selno;
		int sgn = 0, i = 0, c = 0;
		int tbl[10] = { 0 };
		CString kbnBf;

		m_SelDlgWnd->DeleteSelDlg();

		if( term == VK_RETURN ) {
			m_pZm->tkrec->SetAbsolutePosition( focusIndex + 1 );

			if( m_syzKubn == 2 ) {
				sgn = m_pZm->tkrec->dsign.GetAt( 4 ) & 0x0f;
			}
			else if( m_syzKubn == 3 ) {
				sgn = m_pZm->tkrec->dsign.GetAt( 3 );

				i = 1;
				if( m_pZm->zvol->s_sgn3 & 0x10 ) {
					tbl[i] = 1;
					i++;
				}

				if( m_pZm->zvol->s_sgn3 & 0x20 ) {
					tbl[i] = 2;
					i++;
				}

				if( m_pZm->zvol->s_sgn3 & 0x40 ) {
					tbl[i] = 3;
					i++;
				}

				tbl[i] = 4;
				i++;

				if( m_pZm->zvol->s_sgn3 & 0x80 ) {
					tbl[i] = 5;
					i++;
				}

				if(( m_pZm->zvol->ss_ymd >= 20150401 ) && ( m_pZm->zvol->s_sgn3 & 0x08 )) {
					tbl[i] = 6;
					i++;
				}

				tbl[i] = 0;
				i++;

				for( c = 0; c < i; c++ ) {
					if( tbl[c] == sgn ) {
						break;
					}
				}
				sgn = c;
			}

			int syzFlg = 0;
			if( sgn != selNo ) {
				if( m_syzKubn == 2 ) {
					if( m_pZm->tkrec->dsign.GetAt( 4 ) & BIT_MENZEI ) {
						sgn = m_pZm->tkrec->dsign.GetAt( 4 ) & 0xf0;
						selNo += sgn;
					}
					m_pZm->tkrec->dsign.SetAt( 4, selNo );

					int tkSgn = m_pZm->tkrec->tksgn.GetAt( 1 );
					if(( selNo != 0 ) && (( tkSgn == 2 ) || ( tkSgn == 3 ))) {
						m_pZm->tkrec->tksgn.SetAt( 1, 0 );
						syzFlg = 1;
					}
				}
				else if( m_syzKubn == 3 ) {
					m_pZm->tkrec->dsign.SetAt( 3, tbl[selNo] );

					int tkSgn = m_pZm->tkrec->tksgn.GetAt( 1 );
					if(( selNo != 0 ) && (( tkSgn == 2 ) || ( tkSgn == 3 ))) {
						m_pZm->tkrec->tksgn.SetAt( 1, 0 );
						syzFlg = 1;
					}
				}

				int st = UpdateTkrecEx( m_pZm->tkrec );
				m_pZm->tkrec->Requery( _T( "" ), 0 );

				if( st != -1 ) {
					if( m_syzKubn == 2 ) {
						SkbnMsg( kbnBf, selNo & 0x0f, 2 );
					}
					else if( m_syzKubn == 3 ) {
						SkbnMsg( kbnBf, selNo, 3 );
					}

					m_report_tkdic.SetCaption( focusIndex, TK_KUBUN, kbnBf );
					it->second.dsign.Format( _T( "%s" ), kbnBf);

					if( syzFlg ) {
						m_report_tkdic.SetCaption( focusIndex, TK_SYOHI, _T( "" ));
						it->second.tksgn.Empty();
				}
			}
		}
	}

	SetNexControl( focusIndex, TK_KUBUN, TRUE );
	}
	else if( wParam == 1 ) {
		SetPrevControl( focusIndex, TK_KUBUN, TRUE );
	}
	else if( wParam == 2 ) {
		m_report_tkdic.SetFocus();
		m_report_tkdic.SetFocusedRow( m_report_tkdic.GetRows()->GetAt( focusIndex )); 
	}

	return 0L;
}

//==================================================================
// 課税方式のチェック
//------------------------------------------------------------------
// 引数
//		pZvol			:			ボリュームラベル
//------------------------------------------------------------------
// 返送値
//		0				:			免税
//		1				:			原則課税（比例配分）
//		2				:			原則課税（個別対応）
//		3				:			簡易課税
//==================================================================
int CTkdicDlg::CheckSyzKubun( CZVOLUME *pZvol )
{
	ASSERT( pZvol );
	if( pZvol == NULL ) {
		return -1;
	}

	int sg = 0;

	if(( pZvol->s_sgn2 & 0x0f ) == 0 ) {
		// 原則課税
		sg = 1;
		if( pZvol->s_sgn4 & 0x80 ) {
			// 個別対応
			sg++;
		}
	}
	else if(( pZvol->s_sgn2 & 0x0f ) >= 2 ) {
		sg = 3;
	}

	return sg;
}

//==================================================================
// 課税方式のチェック
//------------------------------------------------------------------
// 引数
//		Kbnbf			:			表示文字列格納用
//		TKszk			:			摘要のdsign
//		type			:			
//		int flg			:			
//------------------------------------------------------------------
// 返送値
//		0				:			正常終了
//		-1				:			エラー
//==================================================================
int CTkdicDlg::SkbnMsg( CString& Kbnbf, unsigned char TKszk, int type, int flg )
{
	int i = 0, c = 0;
	int num = -1;
	int tbl[10] = { 0 };

	CZmGen8 zmGen;
	::ZeroMemory( tbl, sizeof( tbl ));

	if( type == 3 ) {	// 簡易課税
		i = 1;
		if( m_pZm->zvol->s_sgn3 & 0x10 ) {
			tbl[i] = 1;
			i++;
		}
		if( m_pZm->zvol->s_sgn3 & 0x20 ) {
			tbl[i] = 2;
			i++;
		}
		if( m_pZm->zvol->s_sgn3 & 0x40 ) {
			tbl[i] = 3;
			i++;
		}

		tbl[i] = 4;
		i++;

		if( m_pZm->zvol->s_sgn3 & 0x80 ) {
			tbl[i] = 5;
			i++;
		}
		if(( m_pZm->zvol->ss_ymd >= 20150401 ) && ( m_pZm->zvol->s_sgn3 & 0x08 )) {
			tbl[i] = 6;
			i++;
		}
		tbl[i] = 0;
		i++;

		for( c = 0; c < i; c++ ) {
			if( tbl[c] == TKszk ) {
				break;
			}
		}
		num = c;

		// 売上区分
		switch( flg ? TKszk : tbl[TKszk] ) {
		case	1:
			Kbnbf.Format( _T( "%-14.14s" ), _T( "卸売業(第1)" ));
			break;
		case	2:
			Kbnbf.Format( _T( "%-14.14s" ), _T( "小売業(第2)" ));
			break;
		case	3:
			Kbnbf.Format("%-14.14s", "製造業(第3)");
			break;
		case	4:
			if( is5perm() >= 0 ) {
				Kbnbf.Format( _T( "%-14.14s" ), _T( "その他(第4)" ));
			}
			else {
				Kbnbf.Format( _T( "%-14.14s" ), _T( "その他(第4)" ));
			}
			break;
		case	5:
			if( zmGen.CheckValiableVersion( m_pZm->m_database, ID_H26SYZ_VUP_CHK ) == 1 ) {
				if( is8perm() >= 0 ) {
					Kbnbf.Format( _T( "%s" ), _T( "サービス業(第5)" ));
				}
			}
			break;
		case	6:
			if( zmGen.CheckValiableVersion( m_pZm->m_database, ID_H31SYZ_VUP_CHK ) == 1 ) {
				if( is10perm() == 2 ) {
					Kbnbf.Format( _T( "%-14.14s" ), _T( "不動産(第6)" ));
				}
			}
			break;

		default:
			Kbnbf.Empty();
			break;

		}
	}
	else if( type == 2 ) {	// 原則課税／個別対応
		// 仕入区分
		switch( TKszk ) {
		case	1:
			Kbnbf.Format( _T( "%-14.14s" ), _T( "課税に係る" ));
			break;
		case	2:
			Kbnbf.Format( _T( "%-14.14s" ), _T( "非課税に係る" ));
			break;
		case	3:
			Kbnbf.Format( _T( "%-14.14s" ), _T( "共通に係る" ));
			break;

		default:
			Kbnbf.Empty();
			break;

		}
	}

	return num;
}

int	CTkdicDlg::is5perm()
{

	unsigned char	enday[3] = { 0 };
	const unsigned char SEIREKI_CHK_NEN = 0x80;

	/* 消費税 5% 改正年月日の１年前 */
	enday[0] = 0x96;
	enday[1] = 0x04;
	enday[2] = 0x01;

	/* 西暦２０００年以降の判定　*/
	if(( m_pZm->zvol->ss_ymd >= 20000101 ) || ( m_pZm->zvol->ee_ymd >= 20000101 )) {
		return ( 1 );
	}

	/* ３％消費税期間	*/
	if( m_pZm->zvol->ss_ymd <= 19960401 ) {
		return ( -1 );
	}

	/* ３％消費税期間	*/
	if( m_pZm->zvol->ee_ymd <= 19970401 ) {
		return ( -1 );
	}

	/* ５％消費税期間	*/
	if( m_pZm->zvol->ss_ymd >= 19970401 ) {
		return ( 1 );
	}

	/* 決算期間中に３％から５％への改正あり	*/
	return( 0 );

}

int	CTkdicDlg::is8perm()
{
	/* 西暦２０１４年4月１日以降の判定　*/
	if(( m_pZm->zvol->ss_ymd >= 20140101 ) || ( m_pZm->zvol->ee_ymd >= 20140401 )) {
		return ( 1 );
	}

	/* ５％消費税期間	*/
	if( m_pZm->zvol->ee_ymd < 20140401 ) {
		return ( -1 );
	}

	return( 0 );
}

int	CTkdicDlg::is10perm()
{
	/* 西暦２０１９年１０月１日以降の判定　*/
	if(( m_pZm->zvol->ss_ymd >= 20191001 ) || ( m_pZm->zvol->ee_ymd >= 20191001 )) {
		return ( 2 );
	}

	/* 西暦２０１４年4月１日以降の判定　*/
	if(( m_pZm->zvol->ss_ymd >= 20140101 ) || ( m_pZm->zvol->ee_ymd >= 20140401 )) {
		return ( 1 );
	}

	/* ５％消費税期間	*/
	if( m_pZm->zvol->ee_ymd < 20140401 ) {
		return ( -1 );
	}

	return( 0 );
}



//インボイス登録番号取得
int CTkdicDlg::GetInvnoString(char* pbuf, int szpbuf)
{
	int ncnt = 0;
	VARIANT var;

	pbuf[0] = '\0';
	DBdata_get(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
	sprintf_s(pbuf, szpbuf, _T("%s"), (char*)var.pbVal);
	ncnt = 0;
	for( int i = 0; pbuf[i] != '\0'; i++ ) {
		if( isdigit(pbuf[i]) ) {
			ncnt++;
			continue;
		}
		else {
			pbuf[i] = '\0';
			break;
		}
	}

	return ncnt;
}



//	キー移動処理
//		int nID		移動元のコントロール
//		int vect	> 0 -> Next, < 0 -> Prev 
//
int CTkdicDlg::TerminationJob(int nID, int vect)
{
	int idtbl[] = {
		IDC_EDIT1, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDT_INV, IDC_COMBO_MEN,
		IDC_ICSDBEDT_NUM,
		/*IDC_BTN_REG,*/ IDOK, IDCANCEL
	};

	int	idsz = sizeof idtbl / sizeof idtbl[0];
	int	move_id = 0;
	int loop = 1;
	int	repeat = 0;

	do {

		for( int i = 0; i < idsz; i++ ) {
			if( nID == idtbl[i] ) {
				if( vect < 0 ) {
					if( i > 0 )	move_id = idtbl[(i - 1)];
				}
				else {
					if( (i + 1) < idsz )	move_id = idtbl[(i + 1)];
					else {
						if( repeat == 0 ) {
							repeat++;
							move_id = IDC_EDIT1;
						}
					}
				}
				break;
			}
		}

		if( move_id ) {
			CWnd* pwnd = GetDlgItem(move_id);

			if( pwnd && !pwnd->IsWindowEnabled() ) {
				nID = move_id;
				move_id = 0;
			}
			else {
				loop = 0;
			}
		}
		else {
			loop = 0;
		}

	} while( loop );

	int ret = 0;

	if( move_id ) {
		set_focus(move_id);
		ret = 1;
	}

	return ret;
}


//インボイス番号
void CTkdicDlg::TerminationIcsdbedt8Invno(long nChar, long inplen, long kst)
{
	VARIANT var;
	CString strWork, edtInvno;
	char pbuf[128] = { 0 };
	int ncnt;

	switch( nChar ) {
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
	case VK_INSERT:
	case 0x16:	//貼り付け
		if( inplen == 0 ) { // 貼り付けの場合はinplenが0になる
			strWork.Format("%s", m_TkyInvno);
			strWork.Replace("T", "");
			DBdata_get(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);

			edtInvno = var.pbVal;
			if( strWork.CompareNoCase(edtInvno) ) {
				inplen = 1;
			}
		}

		if( inplen > 0 ) {
			ncnt = GetInvnoString(pbuf, sizeof pbuf);
			CString checkInvno;
			checkInvno.Format(_T("T%s"), pbuf);

			if( CheckInvoiceNo() ) {
				int len = checkInvno.GetLength();
				::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
				if( len > sizeof m_TkyInvno ) len = sizeof m_TkyInvno;
				memcpy(m_TkyInvno, checkInvno, len);

				CString stmp;
				stmp = checkInvno;
				stmp.Replace(_T("T"), _T(""));
				VARIANT var;
				var.pbVal = (BYTE*)(LPCTSTR)stmp;
				DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
				DBSetDispString(this, IDC_ICSDBEDT_INV, checkInvno);
			}
		}
		else {
			if( inplen < 0 ) {
				// Delete, Back Spaceで削除すると-1となる
			//	CString dmyInvno;
			//	syzInvoice.CheckInvoiceNumber(dmyInvno);
			//	m_bMsgDsp = TRUE;
			//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP); //必ずエラーになる

				::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
				var.pbVal = NULL;
				DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
				DBSetDispString(this, IDC_ICSDBEDT_INV, NULL);
				//	set_focus();
				nChar = 0;
				//	m_bMsgDsp = FALSE;
			}
		}
		break;

	case VK_DELETE:
		var.pbVal = NULL;
		::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, NULL);
		break;
	}

	if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			TerminationJob(IDC_ICSDBEDT_INV, -1);
		}
		else {
			TerminationJob(IDC_ICSDBEDT_INV, 1);
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN:
		case VK_DOWN:
		case VK_RIGHT:
			TerminationJob(IDC_ICSDBEDT_INV, 1);
			break;
		case VK_UP:
		case VK_LEFT:
		case VK_F2:
			TerminationJob(IDC_ICSDBEDT_INV, -1);
			break;
		}
	}
}


afx_msg void CTkdicDlg::OnKillFocusIcsdbedt8Invno(long inplen)
{
	if( !m_bIgnoreFocus ) {
		TerminationIcsdbedt8Invno(0, inplen, 0);
	}
	else {
		m_bIgnoreFocus = FALSE;
	}
}


BOOL CTkdicDlg::CheckInvoiceNo()
{
	char pbuf[128];
	VARIANT var;
	int ncnt;

	if( !m_bInvDisp )
		return TRUE;

	ncnt = GetInvnoString(pbuf, sizeof pbuf);

	if( ncnt == 0 )
		return TRUE;

	CString checkInvno;
	checkInvno.Format(_T("T%s"), pbuf);
	if( syzInvoice.CheckInvoiceNumber(checkInvno) == -1 ) {
		m_bIgnoreFocus = TRUE;
		m_ErrMsg = syzInvoice.GetErrorMessage();
		PostMessage(WM_INVNO_MESSAGE, INV_ERRMSG);
		//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP);
		var.pbVal = (BYTE*)pbuf;
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, "");
		//	set_focus(IDC_ICSDBEDT_INV);
		//	m_bIgnoreFocus = FALSE;
		return FALSE;
	}

	return TRUE;
}


//インボイス番号より
LRESULT CTkdicDlg::OnInvnoMessage(WPARAM wp, LPARAM lp)
{
	if( wp == INV_ERRMSG ) {
		if( IsWindowVisible() ) {
			ICSMessageBox(m_ErrMsg, MB_ICONSTOP, 0, 0, this);
			set_focus(IDC_ICSDBEDT_INV);
		}
		m_bIgnoreFocus = FALSE;
		m_ErrMsg.Empty();
	}

	return 1;
}




void CTkdicDlg::SaveTkyData(TKYV2_SAVE* psv, BOOL bClear/*=FALSE*/)
{

	char teki[60] = { 0 };
	m_Edit.get_dictky(teki, sizeof teki);

	get_kanadata();

	//インボイス番号チェック
	if( !CheckInvoiceNo() )
		return;

	char pbuf[128];
	CString checkInvno;
	int ncnt = 0;
	if( m_bInvDisp ) {
		ncnt = GetInvnoString(pbuf, sizeof pbuf);
		if( ncnt == 13 ) {
			checkInvno.Format(_T("T%s"), pbuf);
		}
	}
	BYTE menchk = 0;
	if( m_bInvMaster ) {
		menchk = m_ComboMen.GetCurSel();
	}

	strcpy_s(psv->sv_tky, sizeof psv->sv_tky, teki);
	strcpy_s(psv->sv_seltky, sizeof psv->sv_seltky, teki);
	strcpy_s(psv->sv_kana, sizeof psv->sv_kana, m_Kana);
	memcpy(psv->sv_invno, checkInvno, checkInvno.GetLength() );
	psv->sv_menchk = menchk;
}

