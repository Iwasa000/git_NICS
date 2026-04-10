// FuriDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DenpRec.h"
#include "DInputSub.h"
#include "DnpBaseClass.h"

#include "icsdbedt.h"
#include "FuriDlg.h"


// 振替伝票 項目ポジション

static DNP_POS FuriHeadPos[] = {
	// ヘッド部分
	{ 0, 0, DP_DENP_PN,	1 },
	{ 0, 1, DP_MM_PN,	1 },
	{ 0, 2, DP_DD_PN,	1 },
	{ 0, 15,DP_SYONUM_PN,1 },
	{ 0, 3, DP_BMN_PN,	1 },
	{ 0, 4, DP_KOJI_PN,	1 },

	{ 0, 9, DP_KESYU_PN },
	{ 0,11, DP_SYOGT_PN },

	{ -1, -1, -1, 0 }
};


static DNP_POS FuriDataPos[] = {
	// 仕訳部分
	{ 1, 0, FD_DBVAL_PN,	1},
	{ 1, 1, FD_DBZEI_PN,	1},
	{ 1, 2, FD_DSKBN_PN,	1},
	{ 1, 3, FD_DBMN_PN,		1},
	{ 1, 4, FD_DKOJI_PN,	1},
	{ 1, 5, FD_DEBT_PN,		1},
	{ 1, 6, FD_DSYOG_PN,	1},
	{ 1, 7, FD_TKY_PN,		1},
	{ 1, 8, FD_CBMN_PN,		1},
	{ 1, 9, FD_CKOJI_PN,	1},
	{ 1,10, FD_CRED_PN,		1},
	{ 1,11, FD_CSYOG_PN,	1},
	{ 1,12, FD_CSKBN_PN,	1},
	{ 1,13, FD_CRVAL_PN,	1},
	{ 1,14, FD_CRZEI_PN,	1},

	{ 1,15, FD_TGNUM_PN,	1},
	{ 1,16, FD_TGDATE_PN,	1},
	{ 1,17, FD_HJIYU_PN,	1},
	{ 1,18, FD_HUSEN_PN,	1},

	{ -1,-1, -1,	0},

};

static DNP_POS FuriKeiPos[] = {

	{ 1, 133, FD_DBVAL_PN,	0},
	{ 1, 134, FD_DBZEI_PN,	0},
	{ 1, 135, FD_CRVAL_PN,	0},
	{ 1, 136, FD_CRZEI_PN,	0},

	{ -1,-1, -1,	0},

};
// CFuriDlg ダイアログ

IMPLEMENT_DYNAMIC(CFuriDlg, CDnpBaseDlg)

CFuriDlg::CFuriDlg(CWnd* pParent /*=NULL*/)
: CDnpBaseDlg(CFuriDlg::IDD, pParent)
{
	SetDataLineCount(7);
	InitDnpBase( FuriHeadPos, FuriDataPos, FuriKeiPos );

	m_pParent = pParent;

	m_FuriMode = FURI_TANBMON;
}

CFuriDlg::~CFuriDlg()
{
}

void CFuriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDnpBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_diag[0]);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_diag[1]);
	DDX_Control(pDX, IDC_ICSDISPJOB, m_Djob );
	DDX_Control(pDX, IDC_IMAGE1, m_image1);
	DDX_Control(pDX, IDC_IMAGE2, m_image2);
	DDX_Control(pDX, IDC_IMAGE3, m_image3);
	DDX_Control(pDX, IDC_IMAGE4, m_image4);
	DDX_Control(pDX, IDC_IMAGE5, m_image5);
	DDX_Control(pDX, IDC_IMAGE6, m_image6);
	DDX_Control(pDX, IDC_IMAGE7, m_image7);
	DDX_Control(pDX, IDC_IMAGE8, m_image8);

	DDX_Control(pDX, IDC_HUSEN1, m_Husen1);
	DDX_Control(pDX, IDC_HUSEN2, m_Husen2);
	DDX_Control(pDX, IDC_HUSEN3, m_Husen3);
	DDX_Control(pDX, IDC_HUSEN4, m_Husen4);
	DDX_Control(pDX, IDC_HUSEN5, m_Husen5);
	DDX_Control(pDX, IDC_HUSEN6, m_Husen6);
	DDX_Control(pDX, IDC_HUSEN7, m_Husen7);

	DDX_Control(pDX, IDC_SCROLLBAR1, m_scBar);
}


BEGIN_MESSAGE_MAP(CFuriDlg, CDnpBaseDlg)
	ON_BN_CLICKED(IDC_BMNINP_CHK, &CFuriDlg::OnBnClickedBmninpChk)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CFuriDlg メッセージ ハンドラ

BOOL CFuriDlg::OnInitDialog()
{
	GetDlgItem( IDC_BMNINP_CHK )->ModifyStyle( 0, WS_CLIPSIBLINGS);
//	GetDlgItem( IDC_BMNINP_CHK )->ModifyStyle( 0, BS_DEFPUSHBUTTON);

	CDnpBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	InitBaseDiag( m_diag, sizeof m_diag / sizeof m_diag[0] );

	CRect rect, taskrc;
	GetClientRect(&rect);

	HWND hwnd = GetTaskWndID();

	if( hwnd != NULL ) {
		taskrc.SetRectEmpty();
		::GetClientRect( hwnd, &taskrc );

		CSize* scrn = GetScreenSize();
//MyTrace( "TaskWnd hwnd = %08x, scrn  cx = %d, cy = %d\n", hwnd, scrn->cx, scrn->cy );
		if( scrn->cx <= 1024 ) {
//MyTrace( "taskrc %d,%d, %d,%d\n", taskrc.left, taskrc.top, taskrc.right, taskrc.bottom );
			rect.right -= taskrc.Width();
			MoveWindow( rect );
		}
	}

	ICSDialog::OnInitDialogEX();

	int max = m_pBaseCls->get_MaxLineEX();

	set_scrollbar( &m_scBar, -1, max, DP_SCROLL_NL_F );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BEGIN_EVENTSINK_MAP(CFuriDlg, CDnpBaseDlg)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL2, 3, CFuriDlg::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL2, 1, CFuriDlg::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL2, 2, CFuriDlg::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL1, 1, CFuriDlg::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL1, 2, CFuriDlg::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL1, 3, CFuriDlg::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CFuriDlg, IDC_ICSDIAG8CTRL2, 7, CFuriDlg::KanaTermIcsdiag8ctrl2, VTS_I2 VTS_BSTR)

	ON_EVENT_RANGE(CFuriDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 5, CFuriDlg::ImeEndCompositionIcsdbedt8ctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CFuriDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 1, CFuriDlg::TerminationIcsdbedt8ctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CFuriDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 3, CFuriDlg::SetFocusIcsdbedt8ctrl, VTS_I4)
	ON_EVENT_RANGE(CFuriDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 2, CFuriDlg::KillFocusIcsdbedt8ctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CFuriDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 4, CFuriDlg::KanaTerminationIcsdbedtctrl, VTS_I4 VTS_BSTR)
END_EVENTSINK_MAP()

void CFuriDlg::EditONIcsdiag8ctrl2(short index)
{
	CDnpBaseDlg::DiagEditON( IDC_ICSDIAG8CTRL2, index );
}

void CFuriDlg::EditOFFIcsdiag8ctrl2(short index)
{
	DIAGRAM_DATA		dd;
	int st;
	st = diag_getdata( IDC_ICSDIAG8CTRL2, index, &dd, CIcsdiag8ctrl );

	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL2, index, 0, 0, (LPUNKNOWN)&dd);
}

void CFuriDlg::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL2, index, nChar, length, data);
}

void CFuriDlg::KanaTermIcsdiag8ctrl2(short index, LPCTSTR kana)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	CDnpBaseDlg::DiagKanaTermination(IDC_ICSDIAG8CTRL2, index, kana);
}

void CFuriDlg::EditONIcsdiag8ctrl1(short index)
{
	CDnpBaseDlg::DiagEditON( IDC_ICSDIAG8CTRL1, index );
}

void CFuriDlg::EditOFFIcsdiag8ctrl1(short index)
{
	DIAGRAM_DATA		dd;
	diag_getdata( IDC_ICSDIAG8CTRL1, index, &dd, CIcsdiag8ctrl );

	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL1, index, 0, 0, (LPUNKNOWN)&dd);
}

void CFuriDlg::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL1, index, nChar, length, data);
}


void CFuriDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;

//	CDnpBaseDlg::PostNcDestroy();
}

//--------------------------------------------------
//	ダイアグの設定
//	int bmon	部門ありか？
//	int koji	工事ありか？
//--------------------------------------------------
void CFuriDlg::diag_inz()
{
	CDnpBaseDlg::diag_inz();

	if( ! m_BmonLNG && ! m_KojiLNG ) {
		GetDlgItem( IDC_BMNINP_CHK )->ShowWindow( SW_HIDE );
	}
	else {
		GetDlgItem( IDC_BMNINP_CHK )->ShowWindow( SW_SHOW );

		CString str;

		if( m_BmonLNG && m_KojiLNG ) {
			str = "1仕訳ごとに部門・工事番号を入力";
		}
		else if( m_BmonLNG ) {
			str = "1仕訳ごとに部門を入力";
		}
		else {
			str = "1仕訳ごとに工事番号を入力";
		}

		GetDlgItem( IDC_BMNINP_CHK )->SetWindowText( str );
	}
	if (m_SyoNumLNG) {
		DIAGRAM_ATTRIBUTE da;
		m_diag[0].ModifyItem(14, DIAG_MDFY_OPAQUE);
		m_diag[0].ModifyItem(15, DIAG_MDFY_EDIT);

		m_diag[0].GetAttribute(15, (LPUNKNOWN)&da);
		da.attr_inpmax = m_SyoNumLNG;
		m_diag[0].SetAttribute(15, (LPUNKNOWN)&da, TRUE);
	}
	else {
		m_diag[0].ModifyItem(14, DIAG_MDFY_TRANSPARENT);
		m_diag[0].ModifyItem(15, DIAG_MDFY_READONLY);
	}
	
	// 変動事由
	DIAGRAM_DATA		dd;
	dd.data_imgdata  = NULL;
	dd.data_disp.Format( " %s 付箋", m_pBaseCls->get_hjnptitle() );
	m_diag[1].SetData( 147, (LPUNKNOWN)&dd );

	BmkjTitleChange();

	// 諸口枝番
	long flg = DIAG_MDFY_READONLY;
	if( ! m_pBaseCls->IsFuriSyogBrn() )	flg = DIAG_MDFY_TRANSPARENT;

	m_diag[1].ModifyItem( 148, flg );
	m_diag[1].ModifyItem( 149, flg );

	char	tmp[32] = { 0 };
	int edamax;
	edamax = m_EdaLNG;
	if (edamax == 0)	edamax = 4;
	sprintf_s(tmp, sizeof tmp, "%d", _op_max_calq(edamax));

	CWnd* pwnd;
	pwnd = GetDlgItem(IDC_ICSDBEDT_SYOBRN);

	((CICSDBEDT*)pwnd)->SetMinval("0");
	((CICSDBEDT*)pwnd)->SetMaxval(tmp);
	((CICSDBEDT*)pwnd)->SetMaxLen(0);	//桁数設定(数字のときは 0)

	((CICSDBEDT*)pwnd)->ImeEndMode(TRUE);
	((CICSDBEDT*)pwnd)->SetKana(FALSE);
	((CICSDBEDT*)pwnd)->SetKanaTerm(FALSE, 1, ICSDBEDT_KTYPE_KANA);
	((CICSDBEDT*)pwnd)->EnableDelete(0);
	((CICSDBEDT *)pwnd)->EnableWindow(TRUE);
	((CICSDBEDT *)pwnd)->SetFrame(1, RGB_BLACK);
}

// 部門・工事 タイトル部の変更
void CFuriDlg::BmkjTitleChange()
{
	// タイトル部の更新
	DIAGRAM_DATA		dd;

	int debt_idx, cred_idx;

	debt_idx = 140;
	cred_idx = 143;

	if( m_FuriMode == FURI_DATABMON ) {
		if( m_BmonLNG ) {
			if( m_KojiLNG ) {
				dd.data_disp = "部門　工事番号\r\n借　　　　　方";
				m_diag[1].SetData(debt_idx, (LPUNKNOWN)&dd );
				dd.data_disp = "部門　工事番号\r\n貸　　　　　方";
				m_diag[1].SetData(cred_idx, (LPUNKNOWN)&dd );
			}
			else {
				dd.data_disp = "部門　　　　　\r\n借　　　　　方";
				m_diag[1].SetData(debt_idx, (LPUNKNOWN)&dd );
				dd.data_disp = "部門　　　　　\r\n貸　　　　　方";
				m_diag[1].SetData(cred_idx, (LPUNKNOWN)&dd );
			}
		}
		else {
			if( m_KojiLNG ) {
				dd.data_disp = "　　　工事番号\r\n借　　　　　方";
				m_diag[1].SetData(debt_idx, (LPUNKNOWN)&dd );
				dd.data_disp = "　　　工事番号\r\n貸　　　　　方";
				m_diag[1].SetData(cred_idx, (LPUNKNOWN)&dd );
			}
			else {
				dd.data_disp = "\r\n借　　　　　方";
				m_diag[1].SetData(debt_idx, (LPUNKNOWN)&dd );
				dd.data_disp = "\r\n貸　　　　　方";
				m_diag[1].SetData(cred_idx, (LPUNKNOWN)&dd );
			}
		}
	}
	else {
		// 単一部門入力
		dd.data_disp = "\r\n借　　　　　方";
		m_diag[1].SetData(debt_idx, (LPUNKNOWN)&dd );
		dd.data_disp = "\r\n貸　　　　　方";
		m_diag[1].SetData(cred_idx, (LPUNKNOWN)&dd );
	}
}


void CFuriDlg::SetFocusIcsdbedt8ctrl( UINT ID )
{
	CDnpBaseDlg::SetFocusDBEDT( ID );
}

void CFuriDlg::KillFocusIcsdbedt8ctrl( UINT ID, long inplen)
{
	CDnpBaseDlg::KillFocusDBEDT( ID, inplen );
}

void CFuriDlg::TerminationIcsdbedt8ctrl(UINT ID, long nChar, long inplen, long kst)
{
	CDnpBaseDlg::TerminationDBEDT( ID, nChar, inplen, kst );
}

void CFuriDlg::ImeEndCompositionIcsdbedt8ctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring)
{
	CDnpBaseDlg::ImeEndDBEDT( ID, nChar, string, ystring );
}

void CFuriDlg::KanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana)
{
	CDnpBaseDlg::KanaTermDBEDT(ID, Kana);
}

//----------------------------------------------------
//	伝票モード変更による表示
//----------------------------------------------------
void CFuriDlg::ChangeJob( int job )
{
	if( job == JOB_INPUT )
		m_Djob.ShowWindow(SW_HIDE);
	else {
		DispScanTitle( &m_Djob );
		m_Djob.ShowWindow(SW_SHOW);
	}
}


//----------------------------------------------------
//	取消表示用の赤ラインインデックスを返す
//		取り消しラインは2本だが、各行の先頭を返す
//----------------------------------------------------
int	 CFuriDlg::del_idx( int ln ) 
{ 
	const int top_del = 164;
	int add = 0;
	
	add = (ln-1) * 2;

	return top_del + add; 
}


//----------------------------------------------------
//	取消表示用の矩形を取得 スクリーン座標
//----------------------------------------------------
void CFuriDlg::GetDeleteRect( int ln, CRect& delrect )
{
	CRect rect, rc, tmp;

	GetRect( ln, FD_DBVAL_PN, rc );
	rect.left = rc.left;
	rect.top = rc.bottom;
	rect.bottom = rect.top + 3;

	GetRect( ln, FD_CRVAL_PN, tmp );
	rect.right = tmp.right;

	delrect = rect;
}


//----------------------------------------------------
//	1行の矩形を取得 スクリーン座標
//----------------------------------------------------
void CFuriDlg::GetLineRect( int ln, CRect& rect )
{
	CRect rc;

	GetRect( ln, FD_DBVAL_PN, rc );
	rect.top = rc.top;
	rect.left = rc.left;

	GetRect( ln, FD_CRZEI_PN, rc );
	rect.bottom = rc.bottom;
	rect.right = rc.right;
}



//-------------------------------------------------------
//	自動仕訳状態を 摘要タイトル部分の色変更で表現
//
//-------------------------------------------------------
void CFuriDlg::AutoSiwake( int mode ) 
{	
	// 色の変更
	DIAGRAM_ATTRIBUTE	da;
	short index = 141;

	m_diag[1].GetAttribute( index, (LPUNKNOWN)&da );

	da.attr_bcolor = (mode == MODE_AUTO) ? RGB_WINE_GREEN : RGB_WHITE;

	m_diag[1].SetAttribute( index, (LPUNKNOWN)&da, TRUE );
}


//-------------------------------------------------------
//	振替伝票の 単一部門入力 切替
//
//-------------------------------------------------------
void CFuriDlg::FuriModeChange( int mode )
{
	m_FuriMode = mode;

	int i, idx;
	DNP_POS* pos;
	pos = FuriHeadPos;
	BOOL ena;
	DIAGRAM_ATTRIBUTE da;
	CString kno;
	kno.Empty();

	// ヘッド部
	for( i = 0; pos[i].dp_pn != -1; i++ ) {
		switch( pos[i].dp_pn ) {
		case DP_BMN_PN:
			if( m_FuriMode == FURI_TANBMON ) {
				if( m_BmonLNG )	ena = TRUE;
				else			ena = FALSE;
			}
			else {
				ena = FALSE;
				_SetBmon( pos[i].dpno, pos[i].dpindex, -1, NULL );
			}

			pos[i].dp_enable = ena;
			idx = 12;
			if( ! ena ) {
				m_diag[0].ModifyItem( pos[i].dpindex, DIAG_MDFY_READONLY );
				m_diag[0].ModifyItem( idx, DIAG_MDFY_TRANSPARENT );
			}
			else {
				m_diag[0].ModifyItem( pos[i].dpindex, DIAG_MDFY_EDIT );
				m_diag[0].ModifyItem( idx, DIAG_MDFY_OPAQUE );
			}

			m_diag[0].GetAttribute( pos[i].dpindex, (LPUNKNOWN)&da );
			da.attr_inpmax = m_BmonLNG;
			m_diag[0].SetAttribute( pos[i].dpindex, (LPUNKNOWN)&da, FALSE );

			break;

		case DP_KOJI_PN:
			if( m_FuriMode == FURI_TANBMON ) {
				if( m_KojiLNG )	ena = TRUE;
				else			ena = FALSE;
			}
			else {
				ena = FALSE;
				_SetKoji( pos[i].dpno, pos[i].dpindex, kno, NULL );
			}
			pos[i].dp_enable = ena;
			idx = 13;
			if( ! ena ) {
				m_diag[0].ModifyItem( pos[i].dpindex, DIAG_MDFY_READONLY );
				m_diag[0].ModifyItem( idx, DIAG_MDFY_TRANSPARENT );
			}
			else {
				m_diag[0].ModifyItem( pos[i].dpindex, DIAG_MDFY_EDIT );
				m_diag[0].ModifyItem( idx, DIAG_MDFY_OPAQUE );
			}

			m_diag[0].GetAttribute( pos[i].dpindex, (LPUNKNOWN)&da );
			da.attr_inpmax = m_KojiLNG;
			m_diag[0].SetAttribute( pos[i].dpindex, (LPUNKNOWN)&da, FALSE );

			break;
		default:
			continue;
			break;
		}

		pos[i].dp_enable = ena;
	}

	// 仕訳データ部
	pos = FuriDataPos;

	for( i = 0; pos[i].dp_pn != -1; i++ ) {
		switch( pos[i].dp_pn ) {
		case FD_DBMN_PN:
		case FD_CBMN_PN:
			if( m_FuriMode == FURI_DATABMON ) {
				if( m_BmonLNG )	ena = TRUE;
				else			ena = FALSE;
			}
			else {
				ena = FALSE;
			}
			break;

		case FD_DKOJI_PN:
		case FD_CKOJI_PN:
			if( m_FuriMode == FURI_DATABMON ) {
				if( m_KojiLNG )	ena = TRUE;
				else			ena = FALSE;
			}
			else {
				ena = FALSE;
			}
			break;
		default:
			continue;
			break;
		}

		pos[i].dp_enable = ena;
	}

	((CButton*)GetDlgItem( IDC_BMNINP_CHK ))->SetCheck( (m_FuriMode == FURI_TANBMON) ? 0 : 1 );

	// 仕訳データ部
	// タイトル、入力可・不可の変更
	diag_tanbmninz( (m_FuriMode == FURI_TANBMON) );
	BmkjTitleChange();
}

//
// 1仕訳ごとに、部門を入力するか？
//
void CFuriDlg::OnBnClickedBmninpChk()
{

	CButton* p;
	p = ((CButton*)GetDlgItem( IDC_BMNINP_CHK ) );
	UINT st = p->GetState();

	int mode = (st & 0x0003) ? FURI_DATABMON : FURI_TANBMON;

	m_pBaseCls->FuriModeChange( mode );
}


//
//	他のウィンドウからのチェックボックス制御
//
void CFuriDlg::SetTanbmnCheck( int nCheck )
{
	CButton* p;
	p = ((CButton*)GetDlgItem( IDC_BMNINP_CHK ) );
	p->SetCheck( nCheck );
}



// 単一・複合 入力表示
void CFuriDlg::SiwakeTypeDisp( LPCTSTR type )
{
	GetDlgItem(IDC_ST_TYPE)->SetWindowText( type );
}



BOOL CFuriDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = CWnd::FromHandle( pMsg->hwnd );
		int id = pwnd->GetDlgCtrlID();

		if( id == IDC_BMNINP_CHK ) {
			switch( pMsg->wParam ) {
			case VK_F2:
				return TRUE;
				break;
			case VK_TAB:
				if( is_SHIFT() )	break;
				// else なら ↓
			case VK_RETURN:
			case VK_UP:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_DOWN:
				set_focus( -1, DP_DENP_PN );
				return TRUE;
				break;
			}
		}
	}

	return CDnpBaseDlg::PreTranslateMessage(pMsg);
}



//-------------------------------------------------------
//	諸口枝番 部分 入力サイン
//
//-------------------------------------------------------
void CFuriDlg::syog_iniz()
{
	int i;
	DNP_POS* pos;
	BOOL ena;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳データ部
	pos = FuriDataPos;

	for( i = 0; pos[i].dp_pn != -1; i++ ) {
		switch( pos[i].dp_pn ) {
		case FD_DSYOG_PN:
		case FD_CSYOG_PN:
			if( m_pBaseCls->IsFuriSyogBrn() ) {
				ena = TRUE;
			}
			else {
				ena = FALSE;
			}
			break;
		default:
			continue;
			break;
		}

		pos[i].dp_enable = ena;
	}
}


void CFuriDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( pScrollBar && ::IsWindow(m_scBar.m_hWnd) ) {
		int curPos = m_scBar.GetScrollPos();
TRACE("\nnPos = %d, curPos = %d\n", nPos, curPos );
		switch( nSBCode ) {
		case SB_LINEDOWN:
			m_pBaseCls->Vs_Linemove( 1 );	TRACE( "SB_LINEDOWN\n" );	
			break;
		case SB_LINEUP:
			m_pBaseCls->Vs_Linemove( -1 );	TRACE( "SB_LINEUP\n" );	
			break;
		case SB_PAGEDOWN:
			m_pBaseCls->Vs_Linemove( DP_SCROLL_NL_F );	TRACE( "SB_PAGEDOWN\n" );
			break;
		case SB_PAGEUP:
			m_pBaseCls->Vs_Linemove( -DP_SCROLL_NL_F );	TRACE( "SB_PAGEUP\n");
			break;
		case SB_THUMBTRACK:
			m_pBaseCls->Vs_Linemove( nPos - curPos );	TRACE( "SB_THUMBTRACK n-cur %d\n", nPos-curPos );	
			break;
		case SB_THUMBPOSITION:
			TRACE( "SB_THUMBPOSITION\n" );	break;
		case SB_BOTTOM:
			TRACE( "SB_BOTTOM\n" );	break;
		case SB_ENDSCROLL:
			TRACE( "SB_ENDSCROLL\n" );	break;
		case SB_TOP:
			TRACE( "SB_TOP\n" );	break;		
		}
	}


	CDnpBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}


//スクロールバーメッセージ
int CFuriDlg::ScrollbarMsgFunc( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	PostMessage(Msg, wParam, (LPARAM)m_scBar.m_hWnd);
	return 0;
}


BOOL CFuriDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( zDelta > 0 ) {
		ScrollbarMsgFunc( WM_VSCROLL, MAKELONG(SB_LINEUP, 0), 0 );
	}
	else {
		ScrollbarMsgFunc( WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), 0 );
	}

	return CDnpBaseDlg::OnMouseWheel(nFlags, zDelta, pt);
}

//
void CFuriDlg::DispLineInfo( int nowln )
{
	DIAGRAM_DATA		dd;
	dd.data_imgdata  = NULL;

	int max = m_pBaseCls->get_MaxLineEX();

	if( nowln != -1 )
		dd.data_disp.Format( "最大%3d行 [%3d/%3d]", max, nowln, max );
	else
		dd.data_disp.Format( "最大%3d行", max );

	m_diag[1].SetData( 168, (LPUNKNOWN)&dd );
}