// DBAbookView.cpp : CDBAbookView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DBDinp.h"

#include "MainFrm.h"

#include "icsocx.h"
#include "ICSDisp.h"
#include "DinpCol.h"

#include "input1.h"
#include "external.h"
#include "ACSelDlg.h"

#include "DBAbookView.h"
#include "DinpINI.h"
#include "subfnc.h"
#include "dinpsub.h"
#include "MstRw.h"

#include "zinpsub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBAbookView
extern CDipZmSub* pDBzm;
extern CDBSyohi*  pDBsy;
extern CBrnTky m_BrTek;
extern struct _AUTOSEL* pAUTOSEL;

extern BOOL	PROGRAM_OK;
//BOOL	FNC_BUTTON;

// 仕訳状態ビットテーブル
#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル


IMPLEMENT_DYNCREATE(CDBAbookView, CDBbaseView)

BEGIN_MESSAGE_MAP(CDBAbookView, CDBbaseView)
	//{{AFX_MSG_MAP(CDBAbookView)
	ON_COMMAND(ID_BUTTON_END, OnButtonEnd)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
//	ON_COMMAND(ID_BUTTON_F5, CDBbaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CDBbaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CDBbaseView::OnFilePrintPreview)

	ON_NOTIFY( DLN_BEGINLABELEDIT, IDC_ACBOOKLIST, OnBeginLabelEdit )
	ON_NOTIFY( DLN_ENDLABELEDIT,   IDC_ACBOOKLIST, OnEndLabelEdit )
	ON_NOTIFY( DLN_SETDISPINFO,	IDC_ACBOOKLIST, OnSetDispInfo )
	
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus)
	ON_COMMAND(ID_BUTTON_F9, &CDBAbookView::OnButtonF9)

	ON_MESSAGE( WM_SKBN_INEND, OnSkbnInend)
	ON_MESSAGE( WM_SKBN_ICHIRN, DspSkbnIchiran)
	ON_MESSAGE( WM_SKBN_LINEDISP, OnSkbnLineDisp)
	ON_COMMAND(ID_BUTTON_F8, &CDBAbookView::OnButtonF8)
	ON_COMMAND(ID_BUTTON_F7, &CDBAbookView::OnButtonF7)
//	ON_COMMAND(ID_BUTTON_COSEL, &CDBAbookView::OnButtonCosel)
//ON_COMMAND(ID_BUTTON_F3, &CDBAbookView::OnButtonF3)
ON_COMMAND(ID_BUTTON_F12, &CDBAbookView::OnButtonF12)
ON_BN_CLICKED(IDC_BUTTON_DISP, &CDBAbookView::OnBnClickedButtonDisp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBAbookView クラスの構築/消滅

CDBAbookView::CDBAbookView()
	: CDBbaseView(CDBAbookView::IDD), m_List(105)
{
	//{{AFX_DATA_INIT(CDBAbookView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
	
	//注意！この関数を変更してはいけません
	FirstTime()	=	TRUE;

	m_BaseCode = "01010101";
	m_BaseEda = -1;
	m_BaseBmn = -1;
	m_BaseKoji.Empty();

	m_BaseDate = -1;
	m_BaseOfst = -1;
	m_BaseKsign = 0;

	PROGRAM_OK = FALSE;
//	FNC_BUTTON = FALSE;
	m_bMnthFix = FALSE;

	m_NowPN = m_NowLine = -1;

	m_bIgnoreDateTerm = FALSE;
	// 基本項目設定時は、このサインをセットする
	bINP_mode = _RETREV;

	m_TKsyz = m_TKsyz2 = 0;
	m_InsLine = 0;
	m_LASTSEQ = 0;
	m_bAutoSwk = FALSE;
}

CDBAbookView::~CDBAbookView()
{
}

void CDBAbookView::DoDataExchange(CDataExchange* pDX)
{
	CDBbaseView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBAbookView)
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_Input);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_BmnInp);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL3, m_KojiInp);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL4, m_KamokInp);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL5, m_BrnInp);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ACBOOKLIST, m_List);
	DDX_Control(pDX, IDC_MNTHSEL, m_selDate);
}

BOOL CDBAbookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CDBbaseView::PreCreateWindow(cs);
}

void CDBAbookView::BeginApp()
{
	//二回目以降は処理を行いません。注意！これより以下の二行は変更してはいけません。
//	if( !FirstTime() )	return;
	FirstTime()	=	FALSE;

//	UpdateAllChild();

	//この位置にアプリケーションの初期化処理を記述して下さい。
	enable_fixval( TRUE );
	BaseWndMove();

	try {
		// 選択初期化 & 消費税画面作成
		CDBbaseView::InitWork(TRUE);

		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob( &par, -1 );

		if( BMON_MST )	m_BmnInp.EnableWindow( TRUE );
		else			m_BmnInp.EnableWindow( FALSE );

		if( M_KOJI )	m_KojiInp.EnableWindow( TRUE );
		else			m_KojiInp.EnableWindow( FALSE );

		m_BaseCode = "01010101";
		m_BaseEda = -1;
		m_BaseBmn = -1;
		m_BaseKoji.Empty();

		m_BaseDate = -1;
		m_BaseOfst = -1;
		m_BaseKsign = 0;

		GetDlgItem(IDC_STATIC5)->SetWindowText("");

		m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF, 0, 0 );

		int sofs, eofs, sdate, edate;
		m_selDate.GetMonthOffset( sofs, eofs, sdate, edate );

//		if( sofs == eofs ) {
//
//
//		}

		// 初期科目名セット
		m_KamokInp.ImeEndMode( TRUE );
		m_KamokInp.SetKanaTerm( TRUE, 1, ICSDBEDT_KTYPE_KANA );

		struct _KamokuData check = {0};
		struct _DBKNREC* pKn;
			
		strncpy_s( check.kd_code, sizeof check.kd_code, m_BaseCode, m_BaseCode.GetLength() );
		check.kd_eda = m_BaseEda;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			memcpy( check.kd_name, pKn->knnam, sizeof pKn->knnam );
		}

		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &check, pDBzm );

		m_Input.ShowWindow( SW_HIDE );
	}
	catch( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;	
	}

	ListClmSet(0);

//	PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_F11, 0) );

}


// 基本項目 のウインドウ 移動
void CDBAbookView::BaseWndMove()
{
	CRect rect, rc;
	int wd, left;
	int kjsgn = 0;

	if( pDBzm->zvol->apno == 0x20 ) {
		if( (pDBzm->zvol->sub_sw&0x04) ) kjsgn = 1;
	}

	// 部門なし
	if( !(pDBzm->zvol->sub_sw&0x02) ) {
		GetDlgItem( IDC_STATIC1 )->ShowWindow(SW_HIDE);
		m_BmnInp.ShowWindow(SW_HIDE);
	}
	// 工事なし
	if( ! kjsgn ) {
		GetDlgItem( IDC_STATIC2 )->ShowWindow(SW_HIDE);
		m_KojiInp.ShowWindow(SW_HIDE);
	}

	// 開始位置
	m_selfix8.GetWindowRect( rect );
	ScreenToClient( rect );
	left = rect.left;
	CWnd* pwnd;

	for( int i = 0; ; i++ ) {
		switch(i) {
		case 0:
			if( !(pDBzm->zvol->sub_sw&0x02) )	continue;
			else	pwnd = GetDlgItem(IDC_STATIC1);
			break;
		case 1:
			if( !(pDBzm->zvol->sub_sw&0x02) )	continue;
			else	pwnd = (CWnd*)&m_BmnInp;
			break;
		case 2:
			if( ! kjsgn )	continue;
			else	pwnd = GetDlgItem(IDC_STATIC2);
			break;
		case 3:
			if( ! kjsgn )	continue;
			else	pwnd = (CWnd*)&m_KojiInp;
			break;
		case 4:		pwnd = GetDlgItem(IDC_STATIC3);	break;
		case 5:		pwnd = &m_KamokInp;	break;
		case 6:		pwnd = GetDlgItem(IDC_STATIC4);	break;
		case 7:		pwnd = &m_BrnInp;	break;
		case 8:		pwnd = GetDlgItem(IDC_STATIC5);	break;

		default:
			pwnd = NULL;
			break;
		}

		if( ! pwnd )	break;

		pwnd->GetWindowRect( rc );
		ScreenToClient( rc );
		wd = rc.Width();

		rc.left = left;
		rc.right = rc.left + wd;
		pwnd->MoveWindow( rc );

		left = rc.right;
	}
}


// リストの設定
void CDBAbookView::ListClmSet( int wd )
{
	DT_COLUMN	data[10];

	data[0].ln_ofs = 0;
	data[0].clm_title = "";
	data[0].clm_id = DATA_SEQ;
	data[0].clm_type = 0;
	data[0].clm_back = -1;
	data[0].clm_text = -1;
	data[0].clm_inpsz = 0;
	data[0].clm_size = 7;
	data[0].clm_szadj = 0;
	data[0].clm_spos = 0;
	data[0].clm_adj = 0;
	data[0].clm_sidx = 0;

	data[1].ln_ofs = 0;
	data[1].clm_title = "";
	data[1].clm_id = DATA_DATE;
	data[1].clm_type = 0;
	data[1].clm_back = -1;
	data[1].clm_text = -1;
	data[1].clm_inpsz = 0;
	data[1].clm_size = 6;
	data[1].clm_szadj = 0;
	data[1].clm_spos = -1;
	data[1].clm_adj = 0;
	data[1].clm_sidx = -1;

	data[2].ln_ofs = 0;
	data[2].clm_title = "相 手 科 目";
	data[2].clm_id = DATA_DEBT;
	data[2].clm_type = 0;
	data[2].clm_back = -1;
	data[2].clm_text = -1;
	data[2].clm_inpsz = 0;
	data[2].clm_size = 20;
	data[2].clm_szadj = 0;
	data[2].clm_spos = -1;
	data[2].clm_adj = 0;
	data[2].clm_sidx = -1;

	int tksz = 52;
	int tkinp;
	if( (pDBzm->zvol->tk_ln * 2) > tksz )
			tkinp = (pDBzm->zvol->tk_ln * 2);
	else	tkinp = tksz;
	data[3].ln_ofs = 0;
	data[3].clm_title = "";
	data[3].clm_id = DATA_TEKIYO;
	data[3].clm_type = DT_INP_BOTTOMLEFT;
	data[3].clm_back = -1;
	data[3].clm_text = -1;
	data[3].clm_inpsz = tkinp;
	data[3].clm_size = tksz;
	data[3].clm_szadj = 0;
	data[3].clm_spos = -1;
	data[3].clm_adj = 0;
	data[3].clm_sidx = -1;

	data[4].ln_ofs = 0;
	data[4].clm_title = "入　金";
	data[4].clm_id = DATA_VAL;
	data[4].clm_type = 0;
	data[4].clm_back = -1;
	data[4].clm_text = -1;
	data[4].clm_inpsz = 0;
	data[4].clm_size = 15;
	data[4].clm_szadj = 0;
	data[4].clm_spos = -1;
	data[4].clm_adj = 0;
	data[4].clm_sidx = -1;

	data[5].ln_ofs = 0;
	data[5].clm_title = "出　金";
	data[5].clm_id = DATA_USER1;
	data[5].clm_type = DT_KIND_VAL | DT_FMT_RIGHT;
	data[5].clm_fmt = "sss,sss,sss,ss9";
	data[5].clm_back = -1;
	data[5].clm_text = -1;
	data[5].clm_inpsz = 0;
	data[5].clm_size = 15;
	data[5].clm_szadj = 0;
	data[5].clm_spos = -1;
	data[5].clm_adj = 0;
	data[5].clm_sidx = -1;
/*
	data[6].ln_ofs = 0;
	data[6].clm_title = "税　額";
	data[6].clm_id = DATA_ZEI;
	data[6].clm_type = 0;
	data[6].clm_back = -1;
	data[6].clm_text = -1;
	data[6].clm_inpsz = 0;
	data[6].clm_size = 13;
	data[6].clm_szadj = 0;
	data[6].clm_spos = -1;
	data[6].clm_adj = 0;
	data[6].clm_sidx = -1;
*/
	data[6].ln_ofs = 0;
	data[6].clm_title = "";
	data[6].clm_id = DATA_SYZ;
	data[6].clm_type = 0;
	data[6].clm_back = -1;
	data[6].clm_text = -1;
	data[6].clm_inpsz = 0;
	data[6].clm_size =  8;
	data[6].clm_szadj = 0;
	data[6].clm_spos = -1;
	data[6].clm_adj = 0;
	data[6].clm_sidx = -1;

	data[7].ln_ofs = 0;
	data[7].clm_title = "差引残高";
	data[7].clm_id = DATA_USER2;
	data[7].clm_type = DT_KIND_VAL | DT_FMT_RIGHT;
	data[7].clm_fmt = "sss,sss,sss,ss9";
	data[7].clm_back = -1;
	data[7].clm_text = -1;
	data[7].clm_inpsz = 0;
	data[7].clm_size = 15;
	data[7].clm_szadj = 0;
	data[7].clm_spos = -1;
	data[7].clm_adj = 0;
	data[7].clm_sidx = -1;

	m_List.SetItemColumn( &data[0], 8 );
	m_List.SetFocusedSign( TRUE );
	m_List.EnforceLayout();
	m_List.RedrawDataListCtrl();

}


BOOL CDBAbookView::EndApp( WPARAM wParam, BOOL bCosel )
{
	//通常の終了処理を記述して下さい。
	//終了を許可する場合はTRUE、許可しない場合はFALSEを返します。
	if( (wParam & 0x01) ) {
		//ICSExit()でエラー終了した場合はこちらに記述します。
//				mst_close();
	}
	else {
		//それ以外はこちらに記述します。

		int st	=	ICSJobEndBox( _T("処理を終了してもよろしいですか？") );
		if( st != IDYES )	return FALSE;

		// 追加仕訳のチェック
		for( int i = 0; i < ACREC_MAX; i++ ) {
			if( m_ALine[i].abs_pos == 0 ) {
				if( AppendDataCheck( (i+1) ) ) {
					PROGRAM_OK = FALSE;
					
					int st = ICSMessageBox( "最後の仕訳が不完全です。仕訳を完成させますか？\r\n「いいえ」の場合この仕訳は破棄されます。", MB_YESNO );
				
					if( st == IDYES ) {
						PROGRAM_OK = TRUE;

						_set_focus( i+1, AC_DATE_PN );
						return FALSE;
					}
				}
				break;
			}
		}
	}

	return	TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDBAbookViewクラスの印刷

BOOL CDBAbookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CDBAbookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CDBAbookView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: OnPrepareDCこの位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	CDBbaseView::OnPrepareDC(pDC, pInfo);
}

void CDBAbookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CDBAbookView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
}

/////////////////////////////////////////////////////////////////////////////
// CDBAbookViewクラスの診断

#ifdef _DEBUG
void CDBAbookView::AssertValid() const
{
	CDBbaseView::AssertValid();
}

void CDBAbookView::Dump(CDumpContext& dc) const
{
	CDBbaseView::Dump(dc);
}
/*
CDBAbookDoc* CDBAbookView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBAbookDoc)));
	return (CDBAbookDoc*)m_pDocument;
}
*/

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBAbookViewクラスのメッセージ ハンドラ

void CDBAbookView::OnInitialUpdate() 
{
	CDBbaseView::OnInitialUpdate();

	//フォームの大きさにウィンドウの大きさを合わせます。
	//この機能が不要な場合は、以下の二行を削除するか
	//ResizeParentToFit()のパラメータをTRUEにして下さい。
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit( FALSE );

//	((CMainFrame*)GetParentFrame())->MinMaxFlg()	=	TRUE;
	// TODO: OnInitialUpdate() 固有の処理を追加するか、または基本クラスを呼び出してください

	// 縦の長さ変更
	SetResize( IDC_ACBOOKLIST, ICS_RESIZE_BOTTOM );

	int i;
	int clipID[] = {
		IDC_BUTTON_DISP, IDC_MNTHSEL
	};
	// 兄弟ウィンドウをクリップする(ウィンドウを重ねる場合)
	for( i = 0; i < (sizeof clipID/ sizeof clipID[0]); i++ ) {
		GetDlgItem( clipID[i] )->ModifyStyle(0, WS_CLIPSIBLINGS);
	}

	m_List.SetInputHwnd(0, m_Input.m_hWnd );	
}

void CDBAbookView::OnDraw(CDC* pDC) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	//注意！この関数は変更してはいけません。呼出し位置は必ずOnDraw()の最終行です。
//	BeginApp();
}

void CDBAbookView::OnButtonEnd()
{
	//この関数は、キーボードからEndキーを押した時と
	//ツールバーのEndボタンを押した時に呼び出されます。

	((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_CLOSE );
}

#ifdef CLOSE
void CDBAbookView::OnButtonF5()
{
	//この関数は、キーボードからF5キーを押した時と
	//ツールバーのF5ボタンを押した時、
	//及びメニューから印刷が選択された時に呼び出されます。
	CDBbaseView::OnFilePrint();
}
#endif


void CDBAbookView::OnButtonF11() 
{

	set_focus( AC_SELKAMOK_PN );

#ifdef OLD_CLOSE
	CACSelDlg dlg( this );
	dlg.m_pZm	= pDBzm;
	dlg.m_pSy	= pDBsy;
	dlg.m_code	= m_BaseCode;
	dlg.m_koji	= m_BaseKoji;
	dlg.m_brn 	= m_BaseEda;
	dlg.m_bmn	= m_BaseBmn;
	dlg.m_date	= m_BaseDate;
	dlg.m_ofset	= m_BaseOfst;
	dlg.m_ksign	= m_BaseKsign;

	dlg.BRNNAM_Disp = (pAUTOSEL->BRTEK_OPT == OPT_DSP);
	dlg.KMCD_Disp = (pAUTOSEL->KMNO_OPT == OPT_KMNO);

	if( dlg.DoModal() == IDOK ) {
		m_BaseCode = dlg.m_code;
		m_BaseEda = dlg.m_brn;
		m_BaseBmn = dlg.m_bmn;
		m_BaseKoji = dlg.m_koji;

		m_BaseDate = dlg.m_date;
		m_BaseOfst = dlg.m_ofset;
		m_BaseKsign = dlg.m_ksign;

		DBKNREC* pKn;
		CString str, s2;
		LPSTR p;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			str.Format( "%.14s", pKn->knnam );
			m_DSPkmd.SetCaption( str );

			if( m_BaseEda != -1 ) {
				p = str.GetBuffer(64);
				pDBzm->EdabanToStr( p, 64, m_BaseEda );
				str.ReleaseBuffer();

				int tno;
				if( (tno = m_BrTek.get_brtek( m_BaseBmn, m_BaseCode, m_BaseEda )) >= 0 ) {
				/*	if( tno > 0 ) {
						if( pDBzm->DB_TekiyoNoRead( &tkrec, tno ) == ERR )
							return FALSE;
						//摘要toデータ
						dc_sw = tky_select_to_drec( &CREC, &tkrec );
					}
					else {
						// 枝番残高本体に登録された枝番名称
						if( pDBzm->DB_EdabanZanSearch( code, brn ) != 0 )
							return FALSE;

						tkrec.Reset();
						tkrec.m_tkname = pDBzm->ezrec->ezname;
						dc_sw = 0;
					}
					*/
				}
				s2.Empty();
			}
			else {
				str.Empty();
			}

			m_DSPedad.SetCaption( str );
			m_DSPenamd.SetCaption( str );

			if( m_BaseBmn != -1 ) {
				p = str.GetBuffer(64);
				pDBzm->BumonCodeToStr( p, 64, m_BaseBmn );
				str.ReleaseBuffer();
			}
			else	str.Empty();
			m_DSPbmd.SetCaption( str );

			if( ! m_BaseKoji.IsEmpty() ) {
				p = str.GetBuffer(64);
				pDBzm->KojiCodeToStr( p, 64, m_BaseKoji );
				str.ReleaseBuffer();
			}
			else	str.Empty();

			m_DSPkjd.SetCaption( str );

			m_LASTSEQ = 0;	// 最終仕訳番号を セットするため
			int n;
			if( ( n= InitDataSet() ) < 0 )
				return;

			ListDataSet();
			_set_focus( n+1, AC_DATE_PN );

		}
		else {
			ICSExit(0, "科目名称コードが正しくありません！");
			return;
		}
	}
#endif

}


// 表示ボタン
void CDBAbookView::OnBnClickedButtonDisp()
{
	if( ! PROGRAM_OK )	return;

	m_LASTSEQ = 0;	// 最終仕訳番号を セットするため
	int n;
	if( ( n= InitDataSet() ) < 0 )
		return;

	ListDataSet();
	_set_focus( n+1, AC_DATE_PN );

}



//	表にデータをセットする
//		CDATA クラスに 先頭より表示するデータをセットしておく
//
void CDBAbookView::ListDataSet()
{
	DTLIST_DATA data;

	m_List.RemoveAllDtListData();
	int dispcnt = m_List.GetDispDataCount();

	DBKNREC* pKn;
	BOOL bNyuKin;
	ACBOOK_LINE*	al;
	al = m_ALine;

	for( int i = 0; i < dispcnt; i++ ) {
		data.dd_seq = al[i].drec.m_seq;

		data.dd_ddate = al[i].drec.m_ddate;
		data.dd_ksign = al[i].drec.m_ksign;
		data.dd_mofs = al[i].drec.m_mofs;

		bNyuKin = al[i].lcnd[AC_NKIN_PN].INP_sg;

		DWORD tmp;
		if( al[i].drec.m_dbt == m_BaseCode ) {
			tmp = _atoh( (char*)(LPCTSTR)al[i].drec.m_cre );
			hl_rev( &tmp, 4 );
			pKn = pDBzm->PjisToKnrec( tmp );
			if( pKn )	data.dd_deb_name.Format( "%.14s", pKn->knnam );
			else		data.dd_deb_name.Empty();
			data.dd_dbr = al[i].drec.m_cbr;
		}
		else {
			tmp = _atoh( (char*)(LPCTSTR)al[i].drec.m_dbt );
			hl_rev( &tmp, 4 );
			pKn = pDBzm->PjisToKnrec( tmp );
			if( pKn )	data.dd_deb_name.Format( "%.14s", pKn->knnam );
			else		data.dd_deb_name.Empty();
			data.dd_dbr = al[i].drec.m_dbr;
		}

		BYTE kin[6], kin2[6];

		::ZeroMemory( data.dd_udtbl, sizeof data.dd_udtbl );
		::ZeroMemory( data.dd_val, sizeof data.dd_val );
		data.val_flag = 0;

		// 追加行は 金額欄を空白で表示
		if( bNyuKin ) {
			memcpy( data.dd_val, al[i].drec.m_val, 6 );
			data.val_flag = al[i].abs_pos > 0 ? 1 : 0;
		}
		else {
			memcpy( kin, al[i].drec.m_val, 6 );
			data.dd_udtbl[0].userdata = kin;
			data.dd_udtbl[0].initflag = al[i].abs_pos > 0 ? 1 : 0;
		}

		memcpy( data.dd_zei, al[i].drec.m_zei, 6 );
		data.zei_flag = al[i].abs_pos > 0 ? 1 : 0;

		data.dd_syz_str = al[i].syzstr;

		data.dd_tek.tekiyo = al[i].drec.m_tekiyo;
		// イメージ摘要
		if( al[i].drec.m_imgsq > 0 ) {
			if( pDBzm->image->SetAbsolutePosition( al[i].drec.m_imgsq ) == 0 ) {
				IMREC* pIMREC = pDBzm->image->DecodeImage();

				data.dd_tek.width = pIMREC->dotxs;
				data.dd_tek.height = pIMREC->dotys;
				data.dd_tek.image = (char*)pIMREC->imdata;
			}
		}

		data.dd_husen = (al[i].drec.m_dsign[7] & 0x0f);

		data.dd_dsign = 0;
		// 転送済みか？
		if( _tline_chk( al[i].drec.m_seq )) {
			data.dd_dsign |= 0x02;
		}
		// 取消し
		if( al[i].drec.m_dsign[0] & 0x01 ) {
			data.dd_dsign |= 0x01;
		}

		memcpy( kin2, al[i].szan, 6 );
		data.dd_udtbl[1].userdata = kin2;
		data.dd_udtbl[1].initflag = al[i].abs_pos > 0 ? 1 : 0;

		m_List.SetDtListData( i, &data, FALSE );

		if( al[i].abs_pos < 0 )
			continue;
	}

	m_List.RedrawDataListCtrl();

}



//	表の1行分データクリア
//
void CDBAbookView::LineDataClear( int ln )
{
	DTLIST_DATA data;
	int i, sv_seq, sv_ddate;
	BYTE sv_ksign, sv_mofs;
	ACBOOK_LINE* al;
	i = ln-1;

	al = m_ALine;

	sv_seq	 = al[i].drec.m_seq;
	sv_ddate = al[i].drec.m_ddate;
	sv_ksign = al[i].drec.m_ksign;
	sv_mofs  = al[i].drec.m_mofs;

	acline_init( &al[i], 1 );
	al[i].abs_pos = 0;

	al[i].drec.m_seq = sv_seq;
	al[i].drec.m_ddate = sv_ddate;
	al[i].drec.m_ksign = sv_ksign;
	al[i].drec.m_mofs = sv_mofs;

	// BCD 日付をセット
	pDBzm->DateConv( al[i].bcddate, &al[i].drec, CV2_1 );

	al[i].lcnd[AC_SEQ_PN].INP_sg = TRUE;
	al[i].lcnd[AC_DATE_PN].INP_sg = TRUE;

	data.dd_seq = al[i].drec.m_seq;
	data.dd_ddate = al[i].drec.m_ddate;
	data.dd_ksign = al[i].drec.m_ksign;
	data.dd_mofs  = al[i].drec.m_mofs;

	data.dd_cno	= -1;
	data.dd_dbmn = -1;
	data.dd_dbmn_etc.Empty();
	data.dd_dkno.Empty();
	data.dd_dkno_etc.Empty();
	data.dd_dbt.Empty();
	data.dd_deb_name.Empty();

	data.dd_dbr = -1;
	data.dd_deb_etc.Empty();
	data.dd_cbmn = -1;
	data.dd_cbmn_etc.Empty();
	data.dd_ckno.Empty();
	data.dd_ckno_etc.Empty();
	data.dd_cre.Empty();
	data.dd_cre_name.Empty();
	data.dd_cbr = -1;
	data.dd_cre_etc.Empty();
	::ZeroMemory( data.dd_val, sizeof data.dd_val );
	data.val_flag = 0;
	::ZeroMemory( data.dd_zei, sizeof data.dd_zei );
	data.zei_flag = 0;
	data.dd_syz_str.Empty();
	data.dd_dsign = 0;

	data.dd_tek.tekiyo.Empty();
	data.dd_tek.width = 0;
	data.dd_tek.height = 0;
	data.dd_tek.image = NULL;
	data.dd_snumber.Empty();
	data.dd_tdate = -1;
	data.dd_tdstr.Empty();
	data.dd_husen = -1;
	data.dd_hscomment.Empty();

	m_List.SetDtListData( i, &data, TRUE );
}


void CDBAbookView::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult )
{
	if( PROGRAM_OK == FALSE )
		return;
	// 基本項目設定時

	NM_DLCTVIEW* dv;
	dv = (NM_DLCTVIEW*)pNMHDR;
TRACE( "++Abook++ OnBeginLabelEdit *** nItem %d nLine %d ItemRect %d,%d,%d,%d\n", dv->iItem, dv->iLine, 
							dv->rect.left, dv->rect.top, dv->rect.right, dv->rect.bottom );
//TRACE( "++Abook++ **OnBeginLabelEdit nItem = %d\n", dv->iItem );
	CRect inprc;
	inprc = dv->rect;
	m_List.ClientToScreen( inprc );
	ScreenToClient( inprc );

	char tmp[30];
	CString koji;
	struct _KamokuData check = {0};
	struct _DBKNREC* pKn;

	CString kno;
	int line, edit_id, i;
	INT_PTR pos;
	line = dv->iLine;
/*
	if( dv->iItem == DATA_HUSEN ) {
		FocusHusenInput( this, line, dv->iClick );	
		return;
	}
	else {
		// 付箋フォーカス サインを 戻す。
		if( ! m_bHusenFocus )	m_bHusenFocus = TRUE;
	}
*/

	ACBOOK_LINE* al;
	if( dv->iItem == DATA_SEQ || dv->iItem == DATA_USER2 ) {
		// 番号・差引残高
		int nowpn = get_nowpn();
		set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
		_set_focus( line, nowpn );
		return;
	}
	else if( dv->iItem == DATA_DEBT ) {
		al = _getLINE_DATA( line );
		if( al->abs_pos == 0 ) {
			if( m_bAutoSwk ) {
				if( ! al->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
					set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
					_set_focus( line, AC_TKY_PN );
					return;
				}
			}
		}
	}
	else if( dv->iItem == DATA_TEKIYO || dv->iItem == DATA_VAL || dv->iItem == DATA_ZEI || dv->iItem == DATA_USER1 ) {
		al = _getLINE_DATA( line );
		if( al->abs_pos == 0 ) {
			if( ! m_bAutoSwk ) {
				if( ! al->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
					set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
					_set_focus( line, AC_KMK_PN );
					return;
				}
			}
			else {	// 自動仕訳入力
				if( dv->iItem != DATA_TEKIYO ) {
					if( ! al->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
						set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
						_set_focus( line, AC_TKY_PN );
						return;
					}
				}
			}
		}
	}

	CICSDBEDT* pDBed;
	CDataListCtrl* pListCtrl;
	edit_id = IDC_ICSDBEDTCTRL1;
	pDBed = &m_Input;
	pListCtrl = &m_List;
	
	DBeditCtrlSet( dv->iItem );

	BYTE buf[100] = {0};
//	char fmt[20];
	VARIANT var;

	pDBed->ShowWindow(SW_SHOW);

	if( dv->iItem == DATA_DATE && m_bMnthFix ) {
		i = inprc.Width() / 2;
		pDBed->MoveWindow( inprc.left + i, inprc.top, i, inprc.Height() );
	}
	else {
		pDBed->MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );
	}
//	pDBed->MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );

	c_set( edit_id );

	switch( dv->iItem ) {
	case DATA_SEQ:

		break;
	case DATA_DATE:
		FocusIcsDate( line, dv->iClick );

		if( ! m_bMnthFix ) {
			memcpy( buf, getLINE_DATA()->bcddate, 2 );
		}
		else {
			buf[0] = getLINE_DATA()->bcddate[1];
		}
		var.pbVal = buf;
		pDBed->SetData( &var, ICSDBEDT_TYPE_BCD, m_bMnthFix ? 2 : 4 );
		pDBzm->BcdDateToString( tmp, sizeof tmp, getLINE_DATA()->bcddate, m_bMnthFix );
		pDBed->SetDisplayString( tmp );
		break;

	case DATA_DEB_BMON:
	case DATA_DEB_KNO:

		break;
	case DATA_DEBT:
		FocusIcsKamoku( line, dv->iClick );
		
		if( getLINE_DATA()->drec.m_dbt != m_BaseCode ) {
			memcpy( check.kd_code, getLINE_DATA()->drec.m_dbt, KMKCD_LN );
			check.kd_eda = getLINE_DATA()->drec.m_dbr;
			pKn = pDBzm->DB_PjisToKnrec( getLINE_DATA()->drec.m_dbt );
		}		
		else {
			memcpy( check.kd_code, getLINE_DATA()->drec.m_cre, KMKCD_LN );
			check.kd_eda = getLINE_DATA()->drec.m_cbr;
			pKn = pDBzm->DB_PjisToKnrec( getLINE_DATA()->drec.m_cre );
		}

		if( pKn ) sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		else {
			::ZeroMemory( &check, sizeof check );
		}
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

		if( ! getLINE_DATA()->lcnd[AC_KMK_PN].Etcmsg.IsEmpty() )
			pListCtrl->EtcStaticDisp( dv->rect, getLINE_DATA()->lcnd[AC_KMK_PN].Etcmsg, getLINE_DATA()->lcnd[AC_KMK_PN].ETC_TXT_forecol );
		break;	

	case DATA_TEKIYO:
		FocusIcsTekiyo( line, dv->iClick );

		::ZeroMemory( buf, sizeof buf );
		strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->drec.m_tekiyo );
		var.pbVal = buf; 
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		pos = strlen( (char*)buf );
		pDBed->SetCaretPosition(pos);
		break;

	case DATA_VAL:	// 入金
		FocusIcsVal( line, 0, dv->iClick );

		if( getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = buf; 
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

		break;

	case DATA_USER1:	// 出金
		FocusIcsVal( line, 1, dv->iClick );

		if( getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = buf; 
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

		break;

	case DATA_ZEI:

		FocusIcsZei( line, dv->iClick );

		if( getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->drec.m_zei, 6 );
			var.pbVal = buf; 
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );
		break;

		break;
	case DATA_SYZ:
		FocusIcsSyz( line, dv->iClick );
		break;

	default:
		break;
	}
}

void CDBAbookView::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
TRACE( "++Abook++ **OnEndLabelEdit\n" );
	m_Input.ShowWindow(SW_HIDE);
}

void CDBAbookView::OnSetDispInfo( NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_DLCTVIEW* dv;
	dv = (NM_DLCTVIEW*)pNMHDR;
//TRACE( "*** nItem %d nLine %d ItemRect %d,%d,%d,%d\n", dv->iItem, dv->iLine, 
//							dv->rect.left, dv->rect.top, dv->rect.right, dv->rect.bottom );
//TRACE( "++Abook++ **OnSetDispInfo %d\n" );
	CRect inprc;
	inprc = dv->rect;
	m_List.ClientToScreen( inprc );
	ScreenToClient( inprc );

	if( dv->iItem == DATA_HUSEN ) {
	}
	else {
		m_Input.MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );
	}
}


// エディットコントロールの セット
void CDBAbookView::DBeditCtrlSet( int id_pn )
{
	char tmp[60];
	BYTE buf[100] = {0};
	VARIANT var;
	var.pbVal = NULL;
	CICSDBEDT* pDBed;
	pDBed = &m_Input;

	if( id_pn == DATA_DATE ) {
		pDBed->SetData( &var, ICSDBEDT_TYPE_BCD, 4 );
	}
	if( id_pn == DATA_VAL || id_pn == DATA_ZEI || id_pn == DATA_USER1 ) {
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );	
	}
	else {
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	if( id_pn == DATA_DEBT || id_pn == DATA_CRED )
		pDBed->SetDisplayString( "         " );
	else
		pDBed->SetDisplayString( NULL );
	pDBed->SetCaretPosition(0);

	// 左詰 右詰
	switch( id_pn ) {
	case DATA_DEBT:
	case DATA_CRED:
	case DATA_TEKIYO:
		pDBed->SetVector( FALSE );
		break;
	default:
		pDBed->SetVector( TRUE );
		break;
	}

	int kmklen;
	kmklen = KMKCD_LN + 1 + pDBzm->zvol->edcol;

	pDBed->SetKanaTerm(FALSE, 1, ICSDBEDT_KTYPE_KANA );
	pDBed->ImeEndMode( TRUE );
	pDBed->SetCmove( TRUE );

	switch( id_pn ) {
	case DATA_SEQ:
	case DATA_SYZ:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "1" );
		pDBed->SetMaxval( "999999" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;
	case DATA_DATE:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		if( m_bMnthFix )	pDBed->SetMaxval( "31" );
		else				pDBed->SetMaxval( "9931" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;

	case DATA_DENP:	
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "99999" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;	
	case DATA_DEB_BMON:
	case DATA_CRE_BMON:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( pDBzm->zvol->bmcol ) );
		pDBed->SetMaxval( tmp );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;	
	case DATA_DEB_KNO:
	case DATA_CRE_KNO:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( pDBzm->zvol->kjcol ) );
		pDBed->SetMaxval( tmp );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;	
	case DATA_DEBT:
	case DATA_CRED:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( kmklen );
		pDBed->SetInverse( TRUE );
		pDBed->SetKanaTerm(TRUE, 1, ICSDBEDT_KTYPE_KANA );
		break;
	case DATA_VAL:		// 入金
	case DATA_USER1:	// 出金
	case DATA_ZEI:		// 税額
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( TRUE ); // カンマ編集
		pDBed->SetMinval( "-99999999999" );
		pDBed->SetMaxval( "999999999999" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		break;
	case DATA_TEKIYO:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( TRUE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( (pDBzm->zvol->tk_ln*2) );
		pDBed->SetInverse( FALSE );
	//	pDBed->ImeEndMode( FALSE );
		break;

	case DATA_SYONO:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( 10 );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		break;

	case DATA_TGDATE:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "21001231" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		
		break;
	}
}


//---------------------------------------------------------------------------------
// 先頭データから表示
//   返送値は 底に表示しているデータ位置(１からはじまる)
//
//---------------------------------------------------------------------------------
int CDBAbookView::TopDataSet()
{
	//検索開始
	Abook_Requery();

	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};

	int dispcnt = m_List.GetDispDataCount();
	int i, lcnt, abpos;

	acline_init( m_ALine, ACREC_MAX );

	lcnt = dispcnt;

	pDBzm->ledger->MoveFirst();
	abpos = 1;

	if( pDBzm->DBDATA_SetPosition( pDBzm->ledger->seq ) == 0 ) {
		pDBzm->GetCDBData( &SREC );
	}
	else {
		ICSExit( 0, "データを取得できません！" );
		return -1;
	}

	for( i = 0; i < lcnt; i++ ) {
		acline_set( m_ALine, i, abpos );

		memcpy( m_ALine[i].szan, work, sizeof szan );
		// 差引残高を 計算
		if( pDBzm->ledger->val1.IsEmpty() ) {
			l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val2 );
			l_add( work, work, tmp );
		}
		else {
			l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val1 );
			l_sub( work, work, tmp );
		}

		abpos++;

		pDBzm->ledger->MoveNext();
		if( pDBzm->ledger->st == -1 ) {
			pDBzm->ledger->MovePrev();
			break;
		}
	}

	return (i+1);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 初期データ表示
// 返送値は 底に表示しているデータ位置(１からはじまる)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CDBAbookView::InitDataSet()
{
	CString filter;
	// 検索を開始
	Abook_Requery();

	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};

	int dcnt = 0;
	int i, lcnt, abpos, n;
	int reccnt = pDBzm->ledger->GetRecordCount();
	AC_SZAN aszan = {0};

	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize( reccnt );

	if( reccnt > 0 ) {
		do {

			if( pDBzm->ledger->val1.IsEmpty() ) {
				l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val2 );
				l_add( aszan.ac_szan, aszan.ac_szan, tmp );
			}
			else {
				l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val1 );
				l_sub(aszan.ac_szan, aszan.ac_szan, tmp );
			}

			m_SZan[dcnt] = aszan;
			dcnt++;

		} while( pDBzm->ledger->MoveNext() == 0 );
	}
	// 差引残高最終インデックス
	n = (dcnt-1);

	int dispcnt = m_List.GetDispDataCount();

	acline_init( m_ALine, ACREC_MAX );

	lcnt = (dispcnt / 2);
	if( dcnt < lcnt )	lcnt = dcnt;

	pDBzm->ledger->MoveLast();
	abpos = dcnt;
	memcpy( work, szan, sizeof work );

	if( reccnt > 0 ) {
		if( pDBzm->DBDATA_SetPosition( pDBzm->ledger->seq ) == 0 ) {
			pDBzm->GetCDBData( &SREC );
		}
		else {
			ICSExit( 0, "データを取得できません！" );
			return -1;
		}
	}

	for( i = lcnt; i > 0; i--, n-- ) {
		acline_set( m_ALine, (i-1), abpos );

		memcpy( m_ALine[i-1].szan, m_SZan[n].ac_szan, sizeof m_SZan[n].ac_szan );

		abpos--;

		pDBzm->ledger->MovePrev();
		if( pDBzm->ledger->st == -1 ) {
			pDBzm->ledger->MoveNext();
			break;
		}
	}

	// 新規行 を追加
	m_ALine[lcnt].abs_pos = 0;

	filter = _T( "seq = 1" );
	pDBzm->dcntl->Requery( filter ); 
	int newseq;
	if( pDBzm->dcntl->st != -1 ) {
		newseq = pDBzm->dcntl->cnt;
		if( m_LASTSEQ == 0 )	m_LASTSEQ = newseq;
		newseq++;
	}

	if( lcnt > 0 ) {
		m_ALine[lcnt].drec.m_seq = newseq;
		m_ALine[lcnt].drec.m_ddate = m_ALine[lcnt-1].drec.m_ddate;
		m_ALine[lcnt].drec.m_mofs = m_ALine[lcnt-1].drec.m_mofs;
		m_ALine[lcnt].drec.m_ksign = m_ALine[lcnt-1].drec.m_ksign;
	}
	else {
		m_ALine[lcnt].drec.m_seq = newseq;
		// 入力日付をセット
		set_inpdate( &m_ALine[lcnt].drec );
	}

	// BCD 日付をセット
	pDBzm->DateConv( m_ALine[lcnt].bcddate, &m_ALine[lcnt].drec, CV2_1 );

	return lcnt;
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// データ追加時の再SET
// CDBipDataRec* pcrec ---- 追加したレコード
//
// 返送値は 底に表示しているデータ位置(１からはじまる)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CDBAbookView::DataAppendReset( CDBipDataRec* pcrec )
{
	CString filter;

	//検索開始
	Abook_Requery();

	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};

	int n, dcnt = 0;
	int dispcnt = m_List.GetDispDataCount();
	int i, abpos, reccnt;
	AC_SZAN aszan = {0};

	reccnt = pDBzm->ledger->GetRecordCount();

	// 先頭で表示している仕訳を覚える
	int sv_seq = m_ALine[0].drec.m_seq;

	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize( reccnt );

	n = 0;
	do {
		// 差引残高を配列にセット
		if( pDBzm->ledger->val1.IsEmpty() ) {
			l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val2 );
			l_add( aszan.ac_szan, aszan.ac_szan, tmp );
		}
		else {
			l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val1 );
			l_sub(aszan.ac_szan, aszan.ac_szan, tmp );
		}

		m_SZan[n] = aszan;
		n++;
		if( sv_seq == pDBzm->ledger->seq ) {
			dcnt = n;
		}

	} while( pDBzm->ledger->MoveNext() == 0 );

	// 該当仕訳位置まで検索
	pDBzm->ledger->SetAbsolutePosition( dcnt );

	acline_init( m_ALine, ACREC_MAX );

	abpos = dcnt;
	memcpy( work, szan, sizeof work );

/*	if( pDBzm->DBDATA_SetPosition( pDBzm->ledger->seq ) == 0 ) {
		pDBzm->GetCDBData( &SREC );
	}
	else {
		ICSExit( 0, "データを取得できません！" );
		return;
	}
*/

	i = 0;
	do {
		acline_set( m_ALine, i, abpos );

		memcpy( m_ALine[i].szan, m_SZan[abpos-1].ac_szan, sizeof m_SZan[abpos-1].ac_szan );

		i++;
		abpos++;

	} while( pDBzm->ledger->MoveNext() == 0 );

	// 新規行 を追加
	m_ALine[i].abs_pos = 0;

	int newseq;
	newseq = CREC.m_seq;
	newseq++;

	if( i > 0 ) {
		m_ALine[i].drec.m_seq	= newseq;
		m_ALine[i].drec.m_ddate	= pcrec->m_ddate;
		m_ALine[i].drec.m_mofs	= pcrec->m_mofs;
		m_ALine[i].drec.m_ksign	= pcrec->m_ksign;
	}
	else {
		m_ALine[i].drec.m_seq	= newseq;
		// 入力日付をセット
		set_inpdate( &m_ALine[i].drec );
	}

	// 先頭データセット
	if( m_ALine[0].abs_pos > 0 ) {
		pDBzm->ledger->SetAbsolutePosition( m_ALine[0].abs_pos );
	}

	return i+1;
}


// 出納帳 ページレコードクリア
void CDBAbookView::acline_init( ACBOOK_LINE* aline, int size )
{
	int i, n;
	int ofs;

	ofs = offsetof( ABLN_CND, Etcmsg );

	for( i = 0; i < size; i++ ) {
		(aline + i)->abs_pos = -1;
		(aline + i)->dtype	 = -1;
		(aline + i)->drec.Reset();

		::ZeroMemory( (aline + i)->bcddate, sizeof (aline + i)->bcddate );

		for( n = 0; n < AC_ICNT; n++ ) {
			::ZeroMemory( &(aline + i)->lcnd[n], ofs );
			(aline + i)->lcnd[n].Etcmsg.Empty();
		}

		::ZeroMemory( (aline + i)->syzstr, sizeof (aline + i)->syzstr );
		(aline + i)->imgseq = 0;

		::ZeroMemory( (aline + i)->szan, sizeof (aline + i)->szan );
	}
}


//-------------------------------------------------------------
//	消費税ボリュームに登録されている科目か？
//		消費税ボリュームは SyOpen() で 開かれている
//
//-------------------------------------------------------------
BOOL CDBAbookView::syzvol_kmkchk( CString& dbt, CString& cre )
{
	if( pDBzm->szvol->SVkcod1 == dbt || pDBzm->szvol->SVkcod1 == cre )
		return TRUE;

	if( pDBzm->szvol->SVkcod2 == dbt || pDBzm->szvol->SVkcod2 == cre )
		return TRUE;

	return FALSE;
}

// 出納帳 ページレコードにセット
void CDBAbookView::acline_set( ACBOOK_LINE* aline, int idx, int abpos )
{
	CDBipDataRec* rec;

	(aline + idx)->abs_pos = abpos;
	rec = &(aline + idx)->drec;

	pDBzm->DBDATA_SetPosition( pDBzm->ledger->seq );
	pDBzm->GetCDBData( rec );

	int keep_seq;
	CString kcd1, kcd2;
	keep_seq = pDBzm->ledger->seq;
	kcd1 = pDBzm->ledger->kcd1;
	kcd2 = pDBzm->ledger->kcd2;

	// 元帳クラスで生成された仕訳かどうか？
	(aline + idx)->dtype = 0;
	pDBzm->ledger->MovePrev();

	if( pDBzm->ledger->st != -1 ) {
		// 直前の仕訳が同じSEQの場合
		if( pDBzm->ledger->seq == keep_seq ) {
			// 消費税ボリュームに該当する科目コードか？
			if( syzvol_kmkchk( kcd1, kcd2 ) ) {
				(aline + idx)->dtype = 1;
				::ZeroMemory( rec->m_zei, sizeof rec->m_zei );
			}
		}
	}
	pDBzm->ledger->MoveNext();

	rec->m_seq		= pDBzm->ledger->seq;
	rec->m_ksign	= pDBzm->ledger->ksign;
	rec->m_ddate	= pDBzm->ledger->ddate;
	rec->m_mofs		= pDBzm->ledger->mofs;
	rec->m_cno		= pDBzm->ledger->cno;

	// 入金
	if( pDBzm->ledger->val1.IsEmpty() ) {
		rec->m_dbmn		= pDBzm->ledger->bmn2;
		rec->m_dkno		= pDBzm->ledger->kno2;
		rec->m_dkno.TrimRight();		 
		rec->m_dbt		= pDBzm->ledger->kcd2;
		rec->m_dbt.TrimRight();			 
		rec->m_dbr		= pDBzm->ledger->br2;

		rec->m_cbmn		= pDBzm->ledger->bmn1;
		rec->m_ckno		= pDBzm->ledger->kno1;
		rec->m_ckno.TrimRight();		 
		rec->m_cre		= pDBzm->ledger->kcd1;
		rec->m_cre.TrimRight();			 
		rec->m_cbr		= pDBzm->ledger->br1;

		l_input( rec->m_val, (void*)(LPCTSTR)pDBzm->ledger->val2 );
	}
	else {
		rec->m_dbmn		= pDBzm->ledger->bmn1;
		rec->m_dkno		= pDBzm->ledger->kno1;
		rec->m_dkno.TrimRight();		 
		rec->m_dbt		= pDBzm->ledger->kcd1;
		rec->m_dbt.TrimRight();			 
		rec->m_dbr		= pDBzm->ledger->br1;

		rec->m_cbmn		= pDBzm->ledger->bmn2;
		rec->m_ckno		= pDBzm->ledger->kno2;
		rec->m_ckno.TrimRight();		 
		rec->m_cre		= pDBzm->ledger->kcd2;
		rec->m_cre.TrimRight();			 
		rec->m_cbr		= pDBzm->ledger->br2;

		l_input( rec->m_val, (void*)(LPCTSTR)pDBzm->ledger->val1 );
	}

	rec->m_dsign.Copy( pDBzm->ledger->dsign );
	rec->m_owner	= pDBzm->ledger->owner;
	rec->m_tekiyo	= pDBzm->ledger->tekiyo;
	rec->m_imgsq	= pDBzm->ledger->imgsq;
	rec->m_tag		= pDBzm->ledger->tag;

	// 消費税 属性取得

	pDBsy->SyIniz( pDBzm->ledger );
	sprintf_s( (aline + idx)->syzstr, sizeof (aline + idx)->syzstr, "%s", pDBsy->Sy_get_message( pDBzm->ledger, 0x03 ) );

	// BCD 日付をセット
	pDBzm->DateConv( (aline + idx)->bcddate, rec, CV2_1 );

	// 入力サインセット
	int dc_sw;
	if( rec->m_dbt == m_BaseCode ) {
		// 入金
		(aline + idx)->lcnd[AC_NKIN_PN].INP_sg = TRUE; 
		(aline + idx)->lcnd[AC_SKIN_PN].INP_sg = FALSE;
		dc_sw = 0;
	}
	else {
		// 出金	
		(aline + idx)->lcnd[AC_NKIN_PN].INP_sg = FALSE;
		(aline + idx)->lcnd[AC_SKIN_PN].INP_sg = TRUE;
		dc_sw = 1;
	}
	(aline + idx)->lcnd[AC_ZEI_PN].INP_sg = FALSE;

	(aline + idx)->lcnd[AC_SEQ_PN].INP_sg	= TRUE;
	(aline + idx)->lcnd[AC_DATE_PN].INP_sg	= TRUE;

	BOOL bInpSgn = FALSE;
	if( ! dc_sw ) {
		if( rec->m_cbmn != -1 ) bInpSgn = TRUE;
	}
	else {
		if( rec->m_dbmn != -1 ) bInpSgn = TRUE;
	}
	(aline + idx)->lcnd[AC_BMON_PN].INP_sg	= bInpSgn;

	bInpSgn = FALSE;
	if( pDBzm->zvol->apno == 0x20 ) {
		if( ! dc_sw ) {
			if( ! rec->m_dkno.IsEmpty() ) bInpSgn = TRUE;
		}
		else {
			if( ! rec->m_ckno.IsEmpty() ) bInpSgn = TRUE;
		}
	}
	(aline + idx)->lcnd[AC_KOJI_PN].INP_sg	= bInpSgn;

	(aline + idx)->lcnd[AC_TKY_PN].INP_sg	= TRUE;
	if( rec->m_imgsq > 0 )
		(aline + idx)->lcnd[AC_TKY_PN].IMG_sg = 1;
	
	(aline + idx)->lcnd[AC_SYZ_PN].INP_sg	= TRUE;

	(aline + idx)->lcnd[AC_SZAN_PN].INP_sg	= TRUE;



}

//--------------------------------------------------
// 最上段で上を押された場合のデータ移動
//	return 0 データ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_scrolup()
{
	int set_abs = 0;
	int i;

	if( m_ALine[0].abs_pos <= 0 ) {
		if( m_ALine[1].abs_pos <= 0 ) {
			set_abs = pDBzm->ledger->GetRecordCount();
			if( set_abs <= 0 )
				return 0;
		}
		else
			set_abs = m_ALine[1].abs_pos;
	}
	else {
		set_abs = m_ALine[0].abs_pos;
	}

	pDBzm->ledger->SetAbsolutePosition(set_abs);

	if( pDBzm->ledger->st == -1 ) {
		ermset( -1, "データポジションを取得できません！" );
		return 0;
	}
	pDBzm->ledger->MovePrev();
	if( pDBzm->ledger->st == -1 )	return 0;

	// データを一つずつずらす
	for( i = ACREC_MAX-1; i > 0; i-- ) {
		m_ALine[i] = m_ALine[i-1];
	}

	set_abs--;
	acline_set( &m_ALine[0], 0, set_abs );

	// 差引残高を セット
	memcpy( m_ALine[0].szan, m_SZan[set_abs].ac_szan, sizeof m_SZan[set_abs].ac_szan );

	return 1;
}


//--------------------------------------------------
// 最下段で下を押された場合のデータ移動
//	return 0 データ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_scroldown()
{
	int set_abs = 0;
	int i;

	int lcnt = m_List.GetDispDataCount();

	if( m_ALine[lcnt-1].abs_pos <= 0 ) {
		if( m_InsLine == 0 )	return 0;
		else {
			if( m_ALine[lcnt-2].abs_pos <= 0 )
				return 0;
			else
				set_abs = m_ALine[lcnt-2].abs_pos;
		}
	}
	else {
		set_abs = m_ALine[lcnt-1].abs_pos;
	}

	pDBzm->ledger->SetAbsolutePosition(set_abs);

	if( pDBzm->ledger->st == -1 ) {
		ermset( -1, "データポジションを取得できません！" );
		return 0;
	}

	pDBzm->ledger->MoveNext();
	// データを一つずつずらす
	for( i = 0; i < ACREC_MAX-1; i++ ) {
		m_ALine[i] = m_ALine[i+1];
	}
	
	if( pDBzm->ledger->st == -1 ) {
		// すでに 新規ラインを作成ずみ
		if( m_ALine[lcnt-2].abs_pos == 0 )
			return 0;

		// 新規ラインを追加作成
		CDBipDataRec* smp;
		smp = &m_ALine[lcnt-2].drec;

		acline_init( &m_ALine[lcnt-1], 1 );

		m_ALine[lcnt-1].drec.m_seq = m_LASTSEQ + 1;
		m_ALine[lcnt-1].drec.m_ddate = smp->m_ddate;
		m_ALine[lcnt-1].drec.m_ksign = smp->m_ksign;
		m_ALine[lcnt-1].drec.m_mofs  = smp->m_mofs;
		
		pDBzm->DateConv( m_ALine[lcnt-1].bcddate, smp, CV2_1 );

		m_ALine[lcnt-1].abs_pos = 0;
		m_ALine[lcnt-1].dtype = 0;

		pDBzm->ledger->SetAbsolutePosition(m_ALine[0].abs_pos);

		return 1;
	}
	else {
		set_abs++;
		acline_set( &m_ALine[0], lcnt-1, set_abs );
	}

	char work[6], tmp[6];
	memcpy( work, m_ALine[lcnt-2].szan, sizeof work );
	
	// 差引残高を 計算
	if( pDBzm->ledger->val1.IsEmpty() ) {
		l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val2 );
		l_add( work, work, tmp );
	}
	else {
		l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val1 );
		l_sub( work, work, tmp );
	}

	memcpy( m_ALine[lcnt-1].szan, work, sizeof work );
	pDBzm->ledger->SetAbsolutePosition(m_ALine[0].abs_pos);

	return 1;

}



//--------------------------------------------------
// PageUp が押された場合のページ移動
//	return 0 ページ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_pageup()
{
	// 挿入時は 移動しない
	if( m_InsLine != 0 )
		return 0;

	// データの変更があるか？
	int append = 0;
	int i, ln, lcnt, pos, reccnt;
	ln = get_nowln();

	if( ! PageMoveDataUpdate( ln ) ) {
		return 0;
	}

	lcnt = m_List.GetDispDataCount();

	pos = m_ALine[0].abs_pos;
	if( pos > 1 ) {
		for( i = 0; i < lcnt; i++ ) {
			pos--;
			if( pos == 1 )	break;
		}
		// 先頭行にセット
		if( i < lcnt )
			ln = 1;
	}
	else {
		reccnt = pDBzm->ledger->GetRecordCount();
		if( reccnt > 0 ) { 
			pDBzm->ledger->MoveLast();	// 先頭が追加行のときは最終データ
		}
		else  {
			return 0;
		}
	}

TRACE( "ac pgup %d, %d\n", pos, m_ALine[0].abs_pos );

	pDBzm->ledger->SetAbsolutePosition(pos);

	if( pDBzm->ledger->st == -1 ) {
		ermset( -1, "データポジションを取得できません！" );
		return 0;
	}

	// ページデータセット
	acline_init( m_ALine, ACREC_MAX );
	for( i = 0; i < lcnt; i++ ) {
		acline_set( m_ALine, i, pos );

		memcpy( m_ALine[i].szan, m_SZan[pos-1].ac_szan, sizeof m_SZan[pos-1].ac_szan );
		pos++;

		pDBzm->ledger->MoveNext();
		if( pDBzm->ledger->st == -1 ) {
			if( (i+1) < lcnt ) {
				append = (i+1);	// 追加行
			}
			break;
		}
	}

	// 新規行 を追加
	if( append ) {
		i = (append-1);
		m_ALine[i].abs_pos = 0;
		int newseq = m_LASTSEQ+1;

		if( i > 0 ) {
			m_ALine[i].drec.m_seq = newseq;
			m_ALine[i].drec.m_ddate = m_ALine[i-1].drec.m_ddate;
			m_ALine[i].drec.m_mofs = m_ALine[i-1].drec.m_mofs;
			m_ALine[i].drec.m_ksign = m_ALine[i-1].drec.m_ksign;
		}
		else {
			m_ALine[i].drec.m_seq = newseq;
			// 入力日付をセット
			set_inpdate( &m_ALine[i].drec );
		}

		// BCD 日付をセット
		pDBzm->DateConv( m_ALine[i].bcddate, &m_ALine[i].drec, CV2_1 );

	}

	return 1;
}


//--------------------------------------------------
// PageDown が押された場合のページ移動
//	return 0 ページ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_pagedown()
{
	// 挿入時は 移動しない
	if( m_InsLine != 0 )
		return 0;

	int pos, append;
	int i, ln;

	ln = get_nowln();

	if( ! PageMoveDataUpdate( ln ) ) {
		return 0;
	}

	int lcnt = m_List.GetDispDataCount();

	pos = m_ALine[lcnt-1].abs_pos;
	pDBzm->ledger->SetAbsolutePosition( pos );
	if( pDBzm->ledger->st == -1 ) {
		ermset( -1, "データポジションを取得できません！" );
		return -1;
	}
	// 次ページ開始
	pos++;
	pDBzm->ledger->SetAbsolutePosition( pos );

	if( pDBzm->ledger->st == -1 ) {
		pos = 0;
	}

	acline_init( m_ALine, ACREC_MAX );
	if( pos > 0 ) {
		// ページデータセット
		append = 0;
		for( i = 0; i < lcnt; i++ ) {
			acline_set( m_ALine, i, pos );

			memcpy( m_ALine[i].szan, m_SZan[pos-1].ac_szan, sizeof m_SZan[pos-1].ac_szan );
			pos++;

			pDBzm->ledger->MoveNext();
			if( pDBzm->ledger->st == -1 ) {
				if( (i+2) < lcnt ) {	// 追加行 が表示行数内
					append = (i+2);
				}
				break;
			}
		}
	}
	else {
		append = 1;	// 先頭が新規行
	}
	
	// 新規行 を追加
	if( append ) {
		i = (append-1);
		m_ALine[i].abs_pos = 0;
		int newseq;
		newseq = (m_LASTSEQ+1);

		if( i > 0 ) {
			m_ALine[i].drec.m_seq = newseq;
			m_ALine[i].drec.m_ddate = m_ALine[i-1].drec.m_ddate;
			m_ALine[i].drec.m_mofs = m_ALine[i-1].drec.m_mofs;
			m_ALine[i].drec.m_ksign = m_ALine[i-1].drec.m_ksign;
		}
		else {
			m_ALine[i].drec.m_seq = newseq;
			// 入力日付をセット
			set_inpdate( &m_ALine[i].drec );
		}

		// BCD 日付をセット
		pDBzm->DateConv( m_ALine[i].bcddate, &m_ALine[i].drec, CV2_1 );
	}

	return 1;

}


//--------------------------------------------------
// ページ移動の前に行う 現行のデータチェック
//	return	TRUE --- check OK
//			FALSE -- ページ移動しない
//
//--------------------------------------------------
BOOL CDBAbookView::PageMoveDataUpdate(int nowln )
{
	// 現在の行のKillFocus 呼び出す(データの取得＆バッファにセット)
	m_List.SetFocus(); 
	
	if( m_ALine[nowln-1].abs_pos > 0 ) {
		if( CREC != SCAN_rec ) {
//			pDBzm->DB_DataCorrect( &CREC );
			if( memcmp( CREC.m_val, SCAN_rec.m_val, sizeof CREC.m_val ) ||
				CREC.m_dbt != SCAN_rec.m_dbt ) {
				SzanUpdate( nowln, CREC.m_val, SCAN_rec.m_val, 1 );
			}
		}
	}
	else if( m_ALine[nowln-1].abs_pos == 0 ) {
		if( AppendDataCheck(nowln) ) {
			PROGRAM_OK = FALSE;
			
			int st = ICSMessageBox( "最後の仕訳が不完全です。仕訳を完成させますか？\r\n「いいえ」の場合この仕訳は破棄されます。", MB_YESNO );
		
			PROGRAM_OK = TRUE;

			if( st == IDYES ) {
				_set_focus( nowln, AC_DATE_PN );
				return FALSE;
			}
		}
	}

	return TRUE;
}


//ポジションによる表示切替
void CDBAbookView::disp_change()
{
BOOL sg;
int pn = get_nowpn();

	sg = TRUE;
	// 選択画面の表示色
	if( ! fixvalcnd() )
	{
		enable_fixval( sg );
	}

	// 固定科目選択のディセーブル／イネーブル
	switch( pn ) {
	case AC_BMON_PN:	case AC_KOJI_PN :
	case AC_SYZ_PN:		case AC_TKY_PN :
	case AC_SELBMON_PN:
	case AC_SELKOJI_PN:
		sg = FALSE;	
		break;
	default:
		sg = TRUE;
		break;
	}

	if( sg != fix8cnd() )
	{
		fix8disp( sg );
	}
}


//------------------------------------
//	選択画面を初期表示にする
//
//------------------------------------
void CDBAbookView::SelectReset()
{
	int pn = get_nowpn();
	BOOL bReset = FALSE;

	switch( pn ) {
	case AC_SEQ_PN:
	case AC_DATE_PN:
	case AC_NKIN_PN:
	case AC_SKIN_PN:
		if( m_SelDispPN != SL_KAMOKU_PN )
			bReset = TRUE;
		break;
	}

	if( bReset ) {
		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob(&par, -1);
	}
}


void CDBAbookView::set_nowpn( int pn )
{
	int prev_pn;

	prev_pn = m_NowPN;
	m_NowPN = pn;

	if( pn >= AC_SELBMON_PN && pn <= AC_SELDISP_PN ) {
		// 出納帳入力部分から、基本項目に移動
		if( prev_pn < AC_SELBMON_PN ) {
			acline_init( m_ALine, ACREC_MAX );
			m_List.RemoveAllDtListData();
			m_List.RedrawDataListCtrl();
			set_nowln( -1 );
			acINP_mode = _RETREV;
		}
	}

	disp_change();
	SelectReset();
}

int CDBAbookView::get_nowpn()
{
	return m_NowPN;
}

void CDBAbookView::set_nowln( int ln )
{
	m_NowLine = ln;
}

int CDBAbookView::get_nowln()
{
	return m_NowLine;
}


ACBOOK_LINE* CDBAbookView::getLINE_DATA()
{
	int ln = get_nowln();
ASSERT( ln != -1 );
	return &m_ALine[ln-1];
}


ACBOOK_LINE* CDBAbookView::_getLINE_DATA( int ln )
{
	return &m_ALine[ln-1];
}


void CDBAbookView::Destroy_ModeLessDlg()
{

	Hide_SyzInp();
}


BOOL CDBAbookView::isdeldata()
{
	int nowln;
	nowln = get_nowln();

	return _isdeldata( nowln );
}

BOOL CDBAbookView::_isdeldata( int ln )
{
	if( m_ALine[ln-1].abs_pos > 0 ) {
		if( m_ALine[ln-1].drec.m_dsign[0] & 0x01 )
			return TRUE;
	}

	return FALSE;
}

void CDBAbookView::set_focus( int pn, int iClick/*=0*/ )
{
	_set_focus( get_nowln(), pn );
}

void CDBAbookView::_set_focus( int ln, int pn, int iClick/*=0*/  )
{
	if( pn >= AC_SELBMON_PN ) {
		PostMessage( WM_RETROFOCUS, -1, MAKELONG( pn, 0) );
	}
	else {
		int id = get_dctrl_id( pn );
		PostMessage( WM_RETROFOCUS, ln, MAKELONG( id, iClick) );
	}
}


LRESULT CDBAbookView::OnRetroFocus( WPARAM wParam, LPARAM lParam )
{
	WORD low;
	low = LOWORD( lParam );

	// リストにフォーカスをセットして、DBEDTのフォーカスOFFを発生させる。
	// そのフォーカスOFFでデータを取得する
	m_List.SetFocus();

	if( low >= AC_SELBMON_PN ) {
		switch( low ) {
		case AC_SELBMON_PN:		c_set( IDC_ICSDBEDTCTRL2 );	break;
		case AC_SELKOJI_PN:		c_set( IDC_ICSDBEDTCTRL3 );	break;
		case AC_SELKAMOK_PN:	c_set( IDC_ICSDBEDTCTRL4 );	break;
		case AC_SELBRN_PN:		c_set( IDC_ICSDBEDTCTRL5 );	break;
		case AC_SELDISP_PN:		c_set( IDC_BUTTON_DISP );	break;
		default:
			ASSERT( FALSE );
			break;
		}
	}
	else {
	//	m_List.SetWindowPos( &m_Input, 0,0,0,0,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSIZE );
		m_List.SetPosition( wParam, LOWORD( lParam ), HIWORD( lParam ) );
	}

	return 1;
}


// データコントロール ID を取得
int CDBAbookView::get_dctrl_id( int pn )
{
	int did = -1;

	switch( pn ) {
	case AC_SEQ_PN:		did = DATA_SEQ;		break;
	case AC_DATE_PN:	did = DATA_DATE;	break;
	case AC_BMON_PN:	did = DATA_DEB_BMON;	break;
	case AC_KOJI_PN:	did = DATA_DEB_KNO;	break;
	case AC_KMK_PN:		did = DATA_DEBT;	break;
	case AC_TKY_PN:		did = DATA_TEKIYO;	break;
	case AC_NKIN_PN:	did = DATA_VAL;		break;
	case AC_SKIN_PN:	did = DATA_USER1;	break;
	case AC_ZEI_PN:		did = DATA_ZEI;		break;
	case AC_SYZ_PN:		did = DATA_SYZ;		break;
	case AC_SZAN_PN:	did = DATA_USER2;	break;
	default:
		ASSERT( FALSE );
		break;
	}

	return did;
}


// データコントロール ID を取得
int CDBAbookView::dctrlid_toacpn( int id )
{
	int pn = -1;

	switch( id ) {
	case DATA_SEQ:		pn = AC_SEQ_PN;		break;
	case DATA_DATE:		pn = AC_DATE_PN;	break;
	case DATA_DEB_BMON:	pn = AC_BMON_PN;	break;
	case DATA_DEB_KNO:	pn = AC_KOJI_PN;	break;
	case DATA_DEBT:		pn = AC_KMK_PN;		break;
	case DATA_TEKIYO:	pn = AC_TKY_PN;		break;
	case DATA_VAL:		pn = AC_NKIN_PN;	break;
	case DATA_USER1:	pn = AC_SKIN_PN;	break;
	case DATA_ZEI:		pn = AC_ZEI_PN;		break;
	case DATA_SYZ:		pn = AC_SYZ_PN;		break;
	case DATA_USER2:	pn = AC_SZAN_PN;	break;
	default:
		ASSERT( FALSE );
		break;;
	}

	return pn;
}

//--------------------------------------------------------------------
// ターミネーション時のフォーカス処理
//	int ln ........ 現在ライン
//	int acpn ...... 現在カーソルポジション
//	int vect_n .... 1 = 次項目, -1 = 全項目
//
//--------------------------------------------------------------------
void CDBAbookView::set_termfocus( int ln, int acpn, int vect_n )
{
	DT_RECT* pdt;
	CDataListCtrl* pCtrl;
	DBKNREC* pKn;

	pdt = m_List.GetDtRect();
	pCtrl = &m_List;

	if( pdt == NULL )
		return;

	int pos, nxt, pn;
	pn = get_dctrl_id( acpn );

LOOP:
	pos = -1;
	nxt = 0;

	for( int i = 0; i < pdt->Dtcnt; i++ ) {
		if( pn == pdt->DtID[i] ) {
			if( vect_n < 0 ) break;
			else	nxt = 1;
		}
		else if( nxt ) {
			if( pdt->DtID[i] != DATA_NONE && pCtrl->GetListItemEnable( 0, pdt->DtID[i] ) ) {
				pos = pdt->DtID[i];
				break;
			}
		}

		if( pdt->DtID[i] != DATA_NONE && pCtrl->GetListItemEnable( 0, pdt->DtID[i] ) )
			pos = pdt->DtID[i];
	}

	ACBOOK_LINE* al;
	al = _getLINE_DATA( ln );

	if( pos != -1 ) {
		switch( pos ) {

		case DATA_ZEI:
			if( vect_n > 0 ) {	// 税額へは、ターミネーションで行かせない
				pn = DATA_ZEI;
				goto LOOP;
			}
			break;
		// 入金
		case DATA_VAL:
			if( acINP_mode == _APPEND ) {
				// まだ金額未入力
				if( ! al->lcnd[AC_NKIN_PN].INP_sg && ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					if( al->drec.m_dbt == m_BaseCode ) {
						pKn = pDBzm->DB_PjisToKnrec( al->drec.m_cre );
					}
					else {
						pKn = pDBzm->DB_PjisToKnrec( al->drec.m_dbt );
					}
					// 出金科目
					if( ! (pKn->knatt & 0x01) ) {
						pn = DATA_VAL;
						goto LOOP;
					}
				}
				else if( ! al->lcnd[AC_NKIN_PN].INP_sg ) {
					pn = DATA_VAL;
					goto LOOP;
				}
			}
			else {
				// 入金ではなく 出金
				if( ! al->lcnd[AC_NKIN_PN].INP_sg ) {
					pn = DATA_VAL;
					goto LOOP;
				}
			}
			break;
		// 出金
		case DATA_USER1:
			if( acINP_mode == _APPEND ) {
				if( ! al->lcnd[AC_NKIN_PN].INP_sg && ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					if( al->drec.m_dbt == m_BaseCode ) {
						pKn = pDBzm->DB_PjisToKnrec( al->drec.m_cre );
					}
					else {
						pKn = pDBzm->DB_PjisToKnrec( al->drec.m_dbt );
					}
					// 入金科目
					if( (pKn->knatt & 0x01) ) {
						pn = DATA_USER1;
						goto LOOP;
					}
				}
				else if( ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					pn = DATA_USER1;
					goto LOOP;
				}
			}
			else {
				// 入金ではなく 出金
				if( ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					pn = DATA_USER1;
					goto LOOP;
				}
			}
			break;

		case DATA_SYZ:
			break;

//		case SYON_PN:	// 手形科目でない仕訳は、FOCUS なし
//		case TGDT_PN:
//			if( ! IsBillData(getLINE_DATA()->drec.m_dbt, getLINE_DATA()->drec.m_cre ) ) {
//				pn = pos;
//				goto LOOP;			
//			}
			break;
		}

		pn = dctrlid_toacpn( pos );
		_set_focus( ln, pn );
	}
}

//----------------------------------------------------------------
//	データ行の移動
//	int ln ---- 1 ～ は 移動する行番号(画面先頭行を １とする)
//				画面データをスクロールする場合は 
//				ACD_SCROLLUP
//				ACD_SCROLLDOWN
//				ACD_PAGEUP
//				ACD_PAGEDOWN
//
//	int pn ---- 項目ポジション
//	int focs_sw	フォーカスサイン
//				マウスクリックでの行移動は、"0", キーの時は "1"
//
//	返送値	-1 ... 正常処理ができなかった
//			 0 ... 正常終了(行の変更の有無は関係なく)
//
//		エラーの場合 例外(CErrBlk)をthrow します
//----------------------------------------------------------------
int CDBAbookView::_dline_chg( int ln, int pn, int focs_sw/* = 0 */ )
{
	int i, pos, nowln, linemax, st;
	BOOL bSzanUpdate = FALSE;
	BOOL bRequery = FALSE;

	linemax = m_List.GetDispDataCount();
	nowln = get_nowln();

TRACE( "++Abook++ _dline_chg ln = %d, nowln = %d, fcsw %d\n", ln, nowln, focs_sw );

	if( nowln != -1 ) {
		if( nowln == ln )	return 0;
		else {
			// 現在の行のKillFocus 呼び出す(データの取得＆バッファにセット)
			if( focs_sw ) m_List.SetFocus(); 
			if( m_ALine[nowln-1].abs_pos > 0 && m_ALine[nowln-1].dtype == 0 ) {
				if( CREC != SCAN_rec ) {
					pDBzm->DB_DataCorrect( &CREC );
					if( memcmp( CREC.m_val, SCAN_rec.m_val, sizeof CREC.m_val ) ||
						CREC.m_dbt != SCAN_rec.m_dbt ) {
						SzanUpdate( nowln, CREC.m_val, SCAN_rec.m_val, 1 );
						bSzanUpdate = TRUE;
					}
					if( CREC.m_ddate != SCAN_rec.m_ddate || 
						(CREC.m_ddate == SCAN_rec.m_ddate && CREC.m_mofs != SCAN_rec.m_mofs ) ) {
						bRequery = TRUE;
					}
				}
			}
			else if( m_ALine[nowln-1].abs_pos == 0 ) {
				// 挿入モード時は 行移動を許さない
				if( m_InsLine > 0 ) {
					_set_focus( nowln, get_nowpn() );
					return 0;
				}

			//	if( ! AppendDataCheck(nowln) ) {
			//		pDBzm->DB_DataAppend( &CREC );
			//		m_ALine[nowln-1].drec.m_seq = CREC.m_seq;

			//		SzanUpdate( nowln, CREC.m_val, NULL, 0 );
			//		bSzanUpdate = TRUE;
			//	}
				if( AppendDataCheck(nowln) ) {
					PROGRAM_OK = FALSE;
					
					st = ICSMessageBox( "最後の仕訳が不完全です。仕訳を完成させますか？\r\n「いいえ」の場合この仕訳は破棄されます。", MB_YESNO );
				
					PROGRAM_OK = TRUE;

					if( st == IDYES ) {
						_set_focus( nowln, AC_DATE_PN );
						return 0;
					}
					else {
						// カレント行の表示をクリア
						CREC.Reset();
						LineDataClear( nowln );
					}
				}
			}
		}
	}

	// 画面データスクロール
	if( ln < 0 ) {
		switch( ln ) {
		case ACD_SCROLLUP:
			if( acline_scrolup() ) {
				if( ! bRequery )	ListDataSet();
			}
			ln = 1;
			break;
		case ACD_SCROLLDOWN:
			if( acline_scroldown() ) {
				if( ! bRequery )	ListDataSet();
			}
			ln = linemax;
			break;
		case ACD_PAGEUP:
			if( acline_pageup() ) {
				if( ! bRequery )	ListDataSet();
			}
			ln = 1;
			break;
		case ACD_PAGEDOWN:
			if( acline_pagedown() ) {
				if( ! bRequery )	ListDataSet();
			}
			ln = 1;
			break;
		default:
			ln = 1;
			break;
		}
	}
	else if( bSzanUpdate ) {
		// 差引残高更新
		SzanDataSet(nowln);
	}

	// 仕訳リクエリー
	if( bRequery ) {
		int nxtseq = -1;
		if( m_ALine[ln-1].abs_pos < 0 ) {
			ln = 1;
		}
		else {
			nxtseq = m_ALine[ln-1].drec.m_seq;
		}

		ln = RequeryACLine( nxtseq, ln );
	}

	if( m_ALine[ln-1].abs_pos < 0 ) {	// 未入力部分
		if( focs_sw )	_set_focus( nowln, pn );
		return 0;
	}
	else if( m_ALine[ln-1].abs_pos == 0 ) {	//追加
		acINP_mode = _APPEND;
	}
	else {
		acINP_mode = _MODEFY;	// 修正
	}
 
	CREC = m_ALine[ln-1].drec;
	DB_SyIniz( &CREC );
	SCAN_rec = CREC;

	pos = m_ALine[ln-1].abs_pos;

	if( pos <= 0 ) {
		for( i = ln-1; i > 0; i-- ) {
			pos = m_ALine[i-1].abs_pos;
			if( pos > 0 )	break;
		}
	}
	else	pos--;

	if( pos <= 0 ) {
		SREC.Reset();
	}
	else {
		pDBzm->ledger->SetAbsolutePosition( pos );
		if( pDBzm->DBDATA_SetPosition( pDBzm->ledger->seq ) == 0 )
			pDBzm->GetCDBData( &SREC );
		else {
			SREC.Reset();
			// 入力日付をセット
			set_inpdate( &SREC );
		}
	}

	if( acINP_mode == _APPEND && m_bAutoSwk )
		m_List.SetCurBkColor( RGB_WINE_GREEN );
	else
		m_List.SetCurBkColor( RGB_WINE_BLUE );

	set_nowln( ln );
	if( focs_sw ) {
		if( pn == AC_NKIN_PN || pn == AC_SKIN_PN ) {
			if( getLINE_DATA()->drec.m_dbt == m_BaseCode ) {
				if( pn == AC_SKIN_PN )	pn = AC_NKIN_PN;
			}
			else {
				if( pn == AC_NKIN_PN )	pn = AC_SKIN_PN;
			}
		}
		_set_focus( ln, pn );
	}
	else {
		m_List.RedrawDataListCtrl();
	}

	return 0;
}


//-----------------------------------------------------
//	キーによる行移動
//	long nChar ---- キーコード
//
//	エラーの場合 例外(CErrBlk)を throw します
//-----------------------------------------------------
int CDBAbookView::dline_chg( long nChar )
{
	int ln, linemax;
TRACE( "++Abook++ dline_chg\n" );

	linemax = m_List.GetDispDataCount();

	// 追加行は キーによる移動を 許さない
	if( m_InsLine == 0 ) {
		if( acINP_mode == _APPEND && get_nowpn() != AC_DATE_PN ) {
			if( nChar == VK_UP || nChar == VK_PRIOR ) {
				return 0;
			}
			if( nChar == VK_DOWN || nChar == VK_NEXT ) {
				return 0;
			}
		}
	}

	switch( nChar )
	{
/*		case VK_END:	//修正の終了
			if( acINP_mode != c_KEYSCAN && get_nowln() != APPEND_LINE )
				ln = APPEND_LINE;	//入力行に戻る
			else
				return( nChar );
			break;
*/
		case VK_UP:	//１行戻る
			ln = get_nowln();
			if( ln == 1 )
				ln = ACD_SCROLLUP;
			else
				ln--;

			break;

		case VK_DOWN:	//１行進む
			ln = get_nowln() + 1;
			if( ln > linemax )	
				ln = ACD_SCROLLDOWN;
			break;

		case VK_PRIOR:
			ln = ACD_PAGEUP;
			break;
		case VK_NEXT:
			ln = ACD_PAGEDOWN;
			break;

		default:
			return( nChar );
			break;
	}

	return( _dline_chg( ln, get_nowpn(), 1 ) );

}

//-------------------------------------------
// 追加用仕訳データのチェック
//	int ln ... 追加した 行
//	return	TRUE ---- 日付以外の入力があった
//			FALSE --- 日付以外入力がない
//----------------------------------------------
int CDBAbookView::AppendDataCheck( int ln ) 
{
	ACBOOK_LINE* al;
	BOOL bRet = FALSE;

	al = _getLINE_DATA( ln );

	if( al->abs_pos > 0 )
		return TRUE;

	if( al->lcnd[AC_BMON_PN].INP_sg ||
		al->lcnd[AC_KOJI_PN].INP_sg ||
		al->lcnd[AC_KMK_PN].INP_sg ||
		al->lcnd[AC_TKY_PN].INP_sg ||
		al->lcnd[AC_NKIN_PN].INP_sg || al->lcnd[AC_SKIN_PN].INP_sg ) {
		bRet = TRUE;
	}
	else {
		bRet = FALSE;
	}

	return bRet;
}

//-------------------------------------------
// 仕訳をクエリーする
//	int nxtseq ... 今回 選択ぎょうになる仕訳番号
//  int ln	...... セットする行(１から)
//  
//	セットする行番号を返送する(１から)
//----------------------------------------------
int CDBAbookView::RequeryACLine( int nxtseq, int ln )
{
	CString filter;

	//検索開始
	Abook_Requery();

	if( pDBzm->ledger->st == -1 ) {
		ermset( -1, "仕訳データクエリーエラー！" );
		return -1;
	}

	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};
	int dcnt = 0;
	int i, lcnt, n, abpos;
	int reccnt = pDBzm->ledger->GetRecordCount();
	AC_SZAN aszan = {0};

	lcnt = m_List.GetDispDataCount();

	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize( reccnt );

	dcnt = n = 0;
	do {
		// 差引残高を配列にセット
		if( pDBzm->ledger->val1.IsEmpty() ) {
			l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val2 );
			l_add( aszan.ac_szan, aszan.ac_szan, tmp );
		}
		else {
			l_input( tmp, (char*)(LPCTSTR)pDBzm->ledger->val1 );
			l_sub(aszan.ac_szan, aszan.ac_szan, tmp );
		}

		m_SZan[n] = aszan;
		n++;
		if( pDBzm->ledger->seq == nxtseq ) {
			dcnt = n;
		}

	} while( pDBzm->ledger->MoveNext() == 0 );

	if( pDBzm->ledger->SetAbsolutePosition( dcnt ) != 0 ) {
		ermset( -1, "仕訳データクエリーエラー！" );
		return -1;
	}

	acline_init( m_ALine, ACREC_MAX );
	abpos = dcnt;

	// 指定行に表示できるかチェック
	for( i = (ln-1); i > 0; i-- ) {
		pDBzm->ledger->MovePrev();
		abpos--;
		if( pDBzm->ledger->st == -1 ) {
			abpos++;
			pDBzm->ledger->MoveNext();
			break;
		}
	}
TRACE( "org - ln = %d\n", ln );

	// 表示行が変更
	if( i > 0 )	ln -= i;

	for( i = 0; i < lcnt; i++ ) {
		acline_set( m_ALine, i, abpos );

TRACE( "nxtseq = %d, data->seq = %d, ln %d\n", nxtseq, pDBzm->ledger->seq, ln );

		//差引残高
		memcpy( m_ALine[i].szan, &m_SZan[abpos-1], 6 );
		abpos++;

		pDBzm->ledger->MoveNext();
		if( pDBzm->ledger->st == -1 )
			break;
	} 

	// 新規行を追加
	if( (i+1) < lcnt ) {
		m_ALine[i+1].abs_pos = 0;
		m_ALine[i+1].drec.m_seq = m_LASTSEQ+1;
		if( i >= 0 ) {
			m_ALine[i+1].drec.m_ddate	= m_ALine[i].drec.m_ddate;
			m_ALine[i+1].drec.m_mofs	= m_ALine[i].drec.m_mofs;
			m_ALine[i+1].drec.m_ksign	= m_ALine[i].drec.m_ksign;
		}
		else {
			// 入力日付をセット
			set_inpdate( &m_ALine[i+1].drec );
		}

		// BCD 日付をセット
		pDBzm->DateConv( m_ALine[i+1].bcddate, &m_ALine[i+1].drec, CV2_1 );
	}

	ListDataSet();

	return ln;
}


//-------------------------------------------
// 差引残高を更新
//	int ln ... 修正・追加した 行
//  char* nowval ... 現在の金額
//  char* oldval ... 修正前の金額
//  int amsgn ...... 0 = append, 1 = modify
//----------------------------------------------
int CDBAbookView::SzanUpdate( int ln, char* nowval, char* oldval, int amsgn ) 
{
	char work[6] = {0};
	CDBipDataRec*	rec;

	memcpy( work, m_ALine[ln-1].szan, sizeof work );
	rec = &m_ALine[ln-1].drec;

	int n = m_ALine[ln-1].abs_pos - 1;
	if( n > 0 ) {
		n--;
		l_add( work, m_SZan[n].ac_szan, nowval );
	}
	else {
		memcpy( work, nowval, 6 );
	}

	// 差引残高より 計算
	if( rec->m_dbt != rec->m_cre ) {
		if( rec->m_dbt == m_BaseCode ) {	// 入金
			l_add( work, work, nowval );
		}
		else {								// 出金
			l_sub( work, work, nowval );
		}
	}

	memcpy( m_ALine[ln-1].szan, work, sizeof work );

	// 現行以降 の差引残高を更新
	for( int i = ln; i < ACREC_MAX; i++ ) {
		rec = &m_ALine[i].drec;

		if( m_ALine[i].abs_pos > 0 ) {
			if( rec->m_dbt != rec->m_cre ) {
				if( rec->m_dbt == m_BaseCode ) {	// 入金
					l_add( work, work, rec->m_val );
				}
				else {								// 出金
					l_sub( work, work, rec->m_val );
				}
			}
			memcpy( &m_SZan[n].ac_szan, work, sizeof work );
			memcpy( m_ALine[i].szan, work, sizeof work );
		}
	}

	return 0;
}


//-------------------------------------------
// 差引残高を画面に再表示
//	int ln ... 再表示開始 行
//
//----------------------------------------------
void CDBAbookView::SzanDataSet( int ln ) 
{
	DTLIST_DATA data;

	int lcnt;
	lcnt = m_List.GetDispDataCount();
	int pn = AC_SZAN_PN;

	for( int line = ln; line <= lcnt; line++ ) {
		m_List.GetDtListData( line-1, &data );
		
		ACBOOK_LINE *dtline;
		dtline = _getLINE_DATA( line );

		_TerminationDataSet( &data, dtline, &pn, 1 );

		m_List.UpdateDtListData( line-1, &data, FALSE );
	//	m_List.SetDtListData( line-1, &data, FALSE );
	}

	m_List.RedrawDataListCtrl();
}


void CDBAbookView::FocusIcsDate( int ln, short click )
{
TRACE( "++Abook++ FocusIcsDate ln = %d, click = %d\n", ln, click );

	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( _dline_chg( ln, AC_DATE_PN, click ? 0 : 1  ) == -1 )
			return;

/*
		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			set_focus( AC_SEQ_PN );
			return;
		}
*/
		set_nowpn( AC_DATE_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}



// 日付チェックデータ
long CDBAbookView::CheckDataIcsDate( long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;

	//
	DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	ACBOOK_LINE *ldata;
	int len;

	if( ! m_bMnthFix ) {
		if( var.pbVal != NULL )	{
			len = strlen( (char*)var.pbVal );
			if( len > 2 ) {
				strncpy_s( data, sizeof data, (char*)var.pbVal, 4 );
			}
			else {
				int sgn = 0;
				if( len == 2 ) {
					if( var.pbVal[0] == '6' || (var.pbVal[0] == '0' && var.pbVal[1] == '0') ) {
						strncpy_s( data, sizeof data, (char*)var.pbVal, len );
						sgn = 1;
					}
				}

				if( !sgn ) {
					ldata = getLINE_DATA();
					int sgn = KESY_SG_GET( ldata->bcddate[0] );
					switch( sgn ) {
					case 1 :
						strcpy_s( data, sizeof data, "61" );	break;
					case 2 :
						strcpy_s( data, sizeof data, "62" );	break;
					case 3 :
						strcpy_s( data, sizeof data, "63" );	break;
					case 4 :
						strcpy_s( data, sizeof data, "00" );	break;
					default:
						sprintf_s( data, sizeof data, "%02x", ldata->bcddate[0] );
						break;
					}
					if( len == 1 ) {
						data[2] = '0';
						data[3] = var.pbVal[0];
					}
					else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
				}
			}
		}
	}
	else {
		ldata = getLINE_DATA();
		int sgn = KESY_SG_GET( ldata->bcddate[0] );
		switch( sgn ) {
		case 1 :
			strcpy_s( data, sizeof data, "61" );	break;
		case 2 :
			strcpy_s( data, sizeof data, "62" );	break;
		case 3 :
			strcpy_s( data, sizeof data, "63" );	break;
		case 4 :
			strcpy_s( data, sizeof data, "00" );	break;
		default:
			sprintf_s( data, sizeof data, "%02x", ldata->bcddate[0] );
			break;
		}
		if( var.pbVal != NULL ) {
			len = strlen( (char*)var.pbVal );
			if( len == 1 ) {
				data[2] = '0';
				data[3] = var.pbVal[0];
			}
			else	strncpy_s( &data[2], sizeof(data) -2, (char*)var.pbVal, 2 );
		}
		else {
			data[2] = data[3] = '0';
		}
	}

	if( DateInputCheck( data, bcd ) != 0 ) {
		// 不正データが入力された
		ldata = getLINE_DATA();
		if( ! m_bMnthFix ) {
			var.pbVal = ldata->bcddate;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
		}
		else  {
			var.pbVal = &ldata->bcddate[1];
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
		}
		pDBzm->BcdDateToString( str, ldata->bcddate, m_bMnthFix );
		DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
		return -1;
	}
	else {
		memcpy( check, bcd, 3 );
	}

/*	if( !check[1] )
	{
		check[0] = 0;
		check[1] = (char)0xff;
	}
*/
	CVolDateDB vd;
	BOOL bCheck = FALSE;
//	if( _modefy_ok( getLINE_DATA()->drec.m_seq ) && !vd.db_vd_check( (BYTE*)check, pDBzm ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
	if( _modefy_ok( getLINE_DATA()->drec.m_seq ) && 
		!pDBzm->Myvd_chek( (BYTE*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
	{
		if( bCheck = AcbookDateCheck( (BYTE*)check ) ) {
			memmove( bcd, &ymd[0], 2 );

			if( ! m_bMnthFix ) {
				var.pbVal = bcd;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &bcd[1];
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
			}
		}
//*DEBUG
//VARIANT dbgvar;
//DBdata_get( this, IDC_ICSDBEDTCTRL1, &dbgvar, ICSDBEDT_TYPE_BCD, 4 );
//TRACE( "date %02x%02x", dbgvar.pbVal[0], dbgvar.pbVal[1] );
	}

	if( ! bCheck ) {
		Buzzer();
		memset( bcd, 0, sizeof bcd );
		var.pbVal = bcd;
		ldata = getLINE_DATA();

		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, m_bMnthFix ? 2 : 4 );
		pDBzm->BcdDateToString( str, ldata->bcddate, m_bMnthFix );
		DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
		return -1;
	}

	return nChar;
}


//-------------------------------------------------------
//	仕訳挿入時 の 日付をチェック
//	BYTE* bcddate ... bcd 日付
//	return TRUE = チェックOK, FALSE = 不正な日付
//
//-------------------------------------------------------
BOOL CDBAbookView::AcbookDateCheck( BYTE* bcddate )
{
	if( m_InsLine == 0 )	return TRUE;

	int ln, lcnt;
	ln = get_nowln();

	if( ln != m_InsLine ) {
		ermset( -1, "挿入時の日付チェックエラー！" );
		ASSERT( FALSE );
		return FALSE;
	}

	lcnt = m_List.GetDispDataCount();

	int date, pos;
	char buf[64];
	BOOL bRet = FALSE;
	CVolDateDB vd;
	VDPAC pac;

	sprintf_s( buf, sizeof buf, "%02x%02x", bcddate[0], bcddate[1] );
	date = atoi( buf );

	if( vd.db_vd_dpac( 1, date, &pac, pDBzm ) != 0 )
		return FALSE;

	db_datecnv( pac.Dymd, &date, 1, 0 );

	if( ln == 1 ) {
		// 現行 より 1つ後の行
		if( m_ALine[ln].abs_pos > 1 ) {
			pos = m_ALine[ln].abs_pos - 1;	// 上の行の ポジション

			pDBzm->ledger->SetAbsolutePosition(pos);
			pDBzm->dbdata->SetAbsolutePosition( pDBzm->ledger->seq );

			if( pDBzm->dbdata->st == -1 ) {
				ermset( -1, "挿入時の日付チェックエラー！" );
				return FALSE;
			}

			if( ( date >= pDBzm->dbdata->ddate && pac.Doffset >= pDBzm->dbdata->mofs ) && 
				( date < m_ALine[ln].drec.m_ddate && pac.Doffset <= m_ALine[ln].drec.m_mofs )) {
				bRet = TRUE;
			}
		}
		else {
			// 入力開始日付と比較
			if( ( date >= pDBzm->zvol->ip_ymd ) && 
				( date < m_ALine[ln].drec.m_ddate && pac.Doffset <= m_ALine[ln].drec.m_mofs )) {
				bRet = TRUE;
			}
		}
	}
	else if( ln == lcnt ) {	// 画面上の最終行で挿入
		pos = m_ALine[ln-2].abs_pos;	// 現在行より1行上

		pDBzm->ledger->SetAbsolutePosition(pos);
		pDBzm->ledger->MoveNext();
		if( pDBzm->ledger->st == -1 )
			return FALSE;
		pDBzm->dbdata->SetAbsolutePosition( pDBzm->ledger->seq );

		if( pDBzm->dbdata->st == -1 ) {
			ermset( -1, "挿入時の日付チェックエラー！" );
			return FALSE;
		}

		if( ( date >= m_ALine[ln-2].drec.m_ddate && pac.Doffset >= m_ALine[ln-2].drec.m_mofs ) && 
			( date < pDBzm->dbdata->ddate && pac.Doffset <= pDBzm->dbdata->mofs ))  {
			bRet = TRUE;
		}
	}
	else {
		if( ( date >= m_ALine[ln-2].drec.m_ddate && pac.Doffset >= m_ALine[ln-2].drec.m_mofs ) && 
			( date < m_ALine[ln].drec.m_ddate && pac.Doffset <= m_ALine[ln].drec.m_mofs ))  {
			bRet = TRUE;
		}
	}

	return bRet;
}


void CDBAbookView::TerminationIcsDate( long nChar, long inplen, long kst ) 
{
//	struct _ItemData check = {0};
//	INPUTDATA data;
	BYTE bcd[10] = {0};
	BYTE tmp[10] = {0};
	int date, ksgn, offset;
	VARIANT var;
	CString str;
	CVolDateDB vd;

	if( PROGRAM_OK == FALSE )
		return;

	if( ! nChar && m_bIgnoreDateTerm ) {
		m_bIgnoreDateTerm = FALSE;
		return;
	}

	try {

//		if( !nChar )	//フォーカスＯＦＦ
//		{
			if( !_modefy_ok( getLINE_DATA()->drec.m_seq ) )
			{
				goto NEXT;
			}

			if( ! m_bMnthFix ) {
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
				if( var.pbVal != NULL )	memcpy( bcd, var.pbVal, 3 );
			}
			else {
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
				
				if( var.pbVal != NULL )	memcpy( &bcd[1], var.pbVal, 2 );
				bcd[0] = getLINE_DATA()->bcddate[0];
			}

			if( ! bcd[0] ) {
				pDBzm->DateConv( bcd, &SREC, CV2_1 );
			}
			else pDBzm->DateConv( bcd, &SREC, CV1_2 );
		
			if( SREC.m_ddate > 0 && SREC.m_mofs >= 0 /*!date_chk( nChar )*/ )
			{
				CREC.m_ddate = SREC.m_ddate;
				CREC.m_ksign = SREC.m_ksign;
				CREC.m_mofs  = SREC.m_mofs;
				
				//消費税
				sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt( &CREC, NULL, acINP_mode) );
				//消費税表示
				TerminationDataSet( AC_SYZ_PN );
			}
			else
			{
				Buzzer();
				getLINE_DATA()->drec.m_ddate = 0;
				getLINE_DATA()->drec.m_mofs = 0;
				getLINE_DATA()->drec.m_ksign = 0;
				memset( getLINE_DATA()->bcddate, '\0', sizeof getLINE_DATA()->bcddate );

				TerminationDataSet( AC_DATE_PN );
				return;
			}

			pDBzm->BcdDateToYmd( bcd, &date );
			ksgn = KESY_SG_GET (bcd[0]);
			memcpy( getLINE_DATA()->bcddate, bcd, 2 );
			memcpy( tmp, bcd, 2 );
//			if( ksgn > 0 )	tmp[0] = (ksgn==1) ? 0x00 : 0x60;
//			if( vd.db_vd_offset( tmp, &offset, pDBzm ) != 0 ) {
			switch( ksgn ) {
			case 1: tmp[0] = 0x61;	break;
			case 2: tmp[0] = 0x62;	break;
			case 3: tmp[0] = 0x63;	break;
			case 4: tmp[0] = 0x00;	break;
			default:
				break;
			}
			if( vd.db_vd_offset(tmp, &offset, pDBzm) == -1 ) {
				ICSMessageBox( "月のオフセットを取得できません！ ");
				return;
			}

			getLINE_DATA()->drec.m_ddate = date;
			getLINE_DATA()->drec.m_mofs = offset;
			getLINE_DATA()->drec.m_ksign = (KESY_SG_GET (bcd[0]) == 0) ? 0 : 1;

			getLINE_DATA()->lcnd[AC_DATE_PN].INP_sg = TRUE;
			if( ! m_bMnthFix ) {
				var.pbVal = bcd;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &bcd[1];
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
			}

			pDBzm->BcdDateToString( str, getLINE_DATA()->bcddate, m_bMnthFix );
			DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
			TerminationDataSet( AC_DATE_PN );

//			return;
//		}

NEXT:

		if( !nChar ) {
			// 10.16 /03 ... 諸口ブザー
			if( acINP_mode == _APPEND ) {
#ifdef LATER_CLOSE
				chk_syobz( &CREC, AC_DATE_PN );	
				if( pAUTOSEL->SYOBUZ_OPT == OPT_SYOBUZ && is_syobuzzer_on(AC_DATE_PN) )
					Buzzer();
#endif
			}
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			nChar = dline_chg( nChar );
			if(	nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合
			/*	if( isdeldata() )
				{
					set_focus( AC_SEQ_PN );
					return;
				}
			*/
//				set_focus( AC_DATE_PN );
				return;
			}
		}

		if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_F3 )
		{
//			if( !is_AutoInp_use() )
//			{
				set_termfocus( get_nowln(), AC_DATE_PN, 1 );
//			}
		}
		else if( nChar == VK_TAB ) {	// 日付固定切り替え
			// つぎのkillfocus は無視する
			m_bIgnoreDateTerm = TRUE;
			DateInputChange();
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
		{
			if( acINP_mode != _APPEND )
				set_termfocus( get_nowln(), AC_DATE_PN, -1 );
		}
		else if( nChar == VK_DELETE ) {
			// 仕訳取消
			acbook_datadelete();
		}
		else if( nChar == VK_INSERT ) {
			// 仕訳挿入
			if( m_InsLine == 0 ) {
				InsertSiwake( get_nowln() );
			}
			else {
				BackNormalAppendMode( get_nowln() );
			}
		}
		else
		{
			Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}

// 仕訳取消し
void CDBAbookView::acbook_datadelete()
{
	int ln = get_nowln();
	int lcnt, other_ln;
	lcnt = m_List.GetDispDataCount();

	if( ln <= 0 )	return;

	ACBOOK_LINE* ldata;
	ldata = getLINE_DATA();

	// 仕訳取消
	if( _modefy_ok( ldata->drec.m_seq ) ) 
	{
		other_ln = -1;	//取消しを行う 他の行番号

		// 実データの取消しを行う
		pDBzm->DBDATA_SetPosition( ldata->drec.m_seq );

		if( pDBzm->dbdata->st == -1 ) {
			ermset( 0, "データを取得できません！" );
			return;
		}

		if( pDBzm->dbdata->dsign[0] & 0x01 )	// 取消し済み
		{
			if( !isDataDelOK( 1 ) )
				return;
			// 取消サインＯＦＦ
			pDBzm->dbdata->dsign[0] &= 0xfe;
		}
		else
		{
			if( !isDataDelOK( 0 ) )
				return;
			// 取消サインＯＮ

			pDBzm->dbdata->dsign[0] |= 0x01;
		}

		CREC.m_dsign[0] = pDBzm->dbdata->dsign[0];
		ldata->drec.m_dsign[0] = pDBzm->dbdata->dsign[0];

		if( pDBzm->DB_DataCorrect() == -1 ) {
			ermset( 0, "取消データの更新ができません！" );
			return;
		}

		// 前後の仕訳をチェック
		// 元帳クラスの生成仕訳
		if( ldata->dtype == 1 ) {
			if( ln > 1 ) {
				_getLINE_DATA( ln-1 )->drec.m_dsign[0] = CREC.m_dsign[0];
				other_ln = ln-1;
			}
		}
		else if( ln < lcnt ) {
			if( _getLINE_DATA( ln+1 )->drec.m_seq == CREC.m_seq &&
				syzvol_kmkchk( _getLINE_DATA( ln+1 )->drec.m_dbt, _getLINE_DATA( ln+1 )->drec.m_cre ) ) {
				_getLINE_DATA( ln+1 )->drec.m_dsign[0] = CREC.m_dsign[0];
				other_ln = ln+1;
			}
		}
		TerminationDataSet(AC_SEQ_PN);

		if( other_ln != -1 )
			TerminationDataSet(AC_SEQ_PN, other_ln );

	}

}

// 日付入力の変更
void CDBAbookView::DateInputChange()
{
	// モードの変更
	m_bMnthFix = ! m_bMnthFix;
	// 日付表示エリアを更新
	c_set( IDC_ICSDISPCTRL10 );
	
	set_focus( AC_DATE_PN );
}


// 科目 フォーカス
void CDBAbookView::FocusIcsKamoku( int ln, short click )
{

	if( PROGRAM_OK == FALSE )
		return;

/*	if( BRNCHK_PN == AC_KMK_PN )
	{
		if( !BRNTKY_sel || getLINE_DATA()->drec.m_dbt != TKYBRN_SEL.BRN_KCOD ) {
			CString tmp;
			tmp = getLINE_DATA()->drec.m_dbt;
			BRNTKY_sel_set( brntky_sch( &TKYBRN_SEL, SelVal32, &m_selval32, tmp ));
		}
		BRNCHK_PN = -1;
		return;
	}
*/

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( _dline_chg( ln, AC_KMK_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}

		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob( &par, -1 );

		set_nowpn( AC_KMK_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 科目チェックデータ
long CDBAbookView::CheckDataKamoku( long nChar, LPCTSTR string ) 
{
	CString kmkstr, ldata_code;
	int edaban = -1, ldata_brn = -1;
	struct _KamokuData check = {0}, ans = {0};
	DBKNREC* pKn;

//_DEBUG_FILEOUT( "CheckDataIcsinputDebt %s\n", string );

	KamokuStr_Chk( string, &check );
	if( acINP_mode != _APPEND && (check.kd_sgn == 2 || check.kd_sgn == 3) ) {
		if( getLINE_DATA()->drec.m_dbt == m_BaseCode )
			memcpy( check.kd_code, getLINE_DATA()->drec.m_cre, KMKCD_LN );
		else
			memcpy( check.kd_code, getLINE_DATA()->drec.m_dbt, KMKCD_LN );
	}

	if( _modefy_ok( getLINE_DATA()->drec.m_seq ) && m_Seldata.InputItemCheck( ans, check, 0 ) == TRUE )
	{
//		if( BRNTKY_sel && ans.kd_eda != -1 ) {
//			set_brn( ans.kd_eda );
//		}
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );

#ifndef NEWBRTKCLOSE	// 07.13 /00
		ACBOOK_LINE *ldata = getLINE_DATA();

		ldata->lcnd[ AC_KMK_PN ].KEY_inp = TRUE;

		if( acINP_mode == _APPEND )
		{
			if( ldata->lcnd[ AC_KMK_PN ].INP_sg )
				m_TKsyz = 0;
			kmkstr = ans.kd_code;
			edaban = ans.kd_eda;

			if( ldata->drec.m_dbt == m_BaseCode ) {
				ldata_code = ldata->drec.m_cre;
				ldata_brn = ldata->drec.m_cbr;
			}
			else {
				ldata_code = ldata->drec.m_dbt;
				ldata_brn  = ldata->drec.m_dbr;
			}

			if( is_mdfybrnitem( ldata_code, ldata_brn, kmkstr, edaban ) )
				m_TKsyz2 = 0;
		}
#endif
	}
	else
	{
		Buzzer();

		::ZeroMemory( &ans, sizeof(ans) );
		ans.kd_eda = -1;
		if( _modefy_ok( getLINE_DATA()->drec.m_seq ) )
		{
			if( getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg )
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
			else {
				Buzzer();
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
			}

		//	set_focus( AC_KMK_PN );
		}
		else
		{
			ACBOOK_LINE *ldata = getLINE_DATA();

			if( ldata->drec.m_dbt == m_BaseCode ) {
				ldata_code = ldata->drec.m_cre;
				ldata_brn = ldata->drec.m_cbr;
			}
			else {
				ldata_code = ldata->drec.m_dbt;
				ldata_brn  = ldata->drec.m_dbr;
			}

			strcpy_s( ans.kd_code, sizeof ans.kd_code, ldata_code );
			ans.kd_eda = ldata_brn;

			pKn = pDBzm->DB_PjisToKnrec( ldata_code );
			if( pKn )	sprintf_s( ans.kd_name, sizeof ans.kd_name,  "%.14s", pKn->knnam );
			else		ZeroMemory( ans.kd_name, sizeof ans.kd_name );
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
		}
		return 0;
	}

	return nChar;
}


// 科目ターミネーション
void CDBAbookView::TerminationKamoku( int ln, long nChar, long inplen, long kst )
{
int st;
#ifndef NEWBRTKCLOSE	// 03.21 /00
ACBOOK_LINE *ldata;
#endif

BOOL set_brtek = FALSE;
CString kmkcd_str, ld_kmk;
int eda_code, ld_eda;
struct _KamokuData check = {0};
DBKNREC* pKn;
int dc_sgn = -1;


	if( PROGRAM_OK == FALSE )
		return;

//	if( !nChar )	//フォーカスＯＦＦ
//	{
#ifndef NEWBRTKCLOSE	// 07.13 /00
		if( !nChar )
			getLINE_DATA()->lcnd[ AC_KMK_PN ].KEY_inp = FALSE;
#endif

		if( !_modefy_ok( getLINE_DATA()->drec.m_seq ) )
			goto NEXT;

		DBkamokudata_get( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

/*
		// 枝番セット
		if( BRNTKY_sel && get_brn() != -1 )
		{
//			check.ip_kmk.edaban = hl_lev( get_brn() );
			eda_code = get_brn();
			check.kd_eda = eda_code;
			set_brn( -1 );	// リセット
		}
*/
		if( !check.kd_code[0] /*check.CdNum == 0xffff*/ && getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg == FALSE )
		{
			if( getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY == FALSE )
//				return;
				goto NEXT;

			if( pAUTOSEL->BRCPY_OPT == OPT_NONOPT ) {	// 08.02 /02
				if( SREC.m_dbt == m_BaseCode )
					memcpy( check.kd_code, SREC.m_cre, KMKCD_LN );
				else
					memcpy( check.kd_code, SREC.m_dbt, KMKCD_LN );

				check.kd_eda = -1;
			}
			else {
				if( SREC.m_dbt == m_BaseCode ) {
					memcpy( check.kd_code, SREC.m_cre, KMKCD_LN );
					check.kd_eda = SREC.m_cbr;
				}
				else {
					memcpy( check.kd_code, SREC.m_dbt, KMKCD_LN );
					check.kd_eda = SREC.m_dbr;
				}
			}

			struct	_DBKNREC* pKn;
			if( (pKn = pDBzm->DB_PjisToKnrec( check.kd_code ) ) != NULL ) {
				memmove( check.kd_name, pKn->knnam, 14 );
			}

#ifndef NEWBRTKCLOSE	// 06.27 /00
			set_brtek = FALSE;

			// 入力枝番タイプセット
			ldata = getLINE_DATA();
			kmkcd_str = check.kd_code;
			eda_code = check.kd_eda;
			ld_kmk = "";

			ldata->lcnd[ AC_KMK_PN ].INP_type = brn_inptype( ldata->lcnd[ AC_KMK_PN ].INP_type, ld_kmk, -1, kmkcd_str, eda_code );
#endif
		}
#ifndef NEWBRTKCLOSE	// 06.27 /00
		else	// すでに科目が入力されている
		{
			set_brtek = FALSE;

			// 入力枝番タイプセット
			ldata = getLINE_DATA();
			kmkcd_str = check.kd_code;
			eda_code = check.kd_eda;
			int brntyp;

			if( ldata->drec.m_dbt == m_BaseCode ) {
				ld_kmk = ldata->drec.m_cre;
				ld_eda = ldata->drec.m_cbr;
				brntyp = brn_inptype( ldata->lcnd[ AC_KMK_PN ].INP_type,  ldata->drec.m_cre, ldata->drec.m_cbr, kmkcd_str, eda_code );
			}
			else {
				ld_kmk = ldata->drec.m_dbt;
				ld_eda = ldata->drec.m_dbr;
				brntyp = brn_inptype( ldata->lcnd[ AC_KMK_PN ].INP_type,  ldata->drec.m_dbt, ldata->drec.m_dbr, kmkcd_str, eda_code );
			}

			ldata->lcnd[ AC_KMK_PN ].INP_type = brntyp;

			// 枝番摘要セット
			if( ldata->lcnd[ AC_KMK_PN ].INP_type == 1 )
			{
				// 科目.枝番 に変更があったか？
				if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC ) {
					kmkcd_str = check.kd_code;
					eda_code = check.kd_eda;

					if( ldata->lcnd[ AC_KMK_PN ].KEY_inp || is_mdfybrnitem( ld_kmk, ld_eda, kmkcd_str, eda_code ) )
						set_brtek = TRUE;
				}
			}
		}
#endif

#ifndef NEWBRTKCLOSE	// 09.08 /00
		ldata->lcnd[ AC_KMK_PN ].KEY_inp = FALSE;
#endif

		if( check.kd_code[0] != '\0' )
		{
			if( SREC.m_dbt == m_BaseCode ) {
				SREC.m_cre = check.kd_code;
				SREC.m_cbr = check.kd_eda;
			}
			else {
				SREC.m_dbt = check.kd_code;
				SREC.m_dbr = check.kd_eda;
			}

			ldata = getLINE_DATA();

			switch( GetKamokuSign() ) {
			case 0:
				CREC.m_dbt = check.kd_code;
				CREC.m_dbr = check.kd_eda;
				ldata->drec.m_dbt = check.kd_code;
				ldata->drec.m_dbr = check.kd_eda;
				break;
			case 1:
				CREC.m_cre = check.kd_code;
				CREC.m_cbr = check.kd_eda;
				ldata->drec.m_cre = check.kd_code;
				ldata->drec.m_cbr = check.kd_eda;
				break;
			default:
				pKn = pDBzm->DB_PjisToKnrec( check.kd_code );
				if( pKn ) {
					if( (pKn->knatt & 0x01) ) {
						CREC.m_cre = check.kd_code;
						CREC.m_cbr = check.kd_eda;
						CREC.m_dbt = m_BaseCode;
						CREC.m_dbr = m_BaseEda;
						CREC.m_dbmn = m_BaseBmn;
						CREC.m_dkno = m_BaseKoji;

						ldata->drec.m_cre = check.kd_code;
						ldata->drec.m_cbr = check.kd_eda;
						ldata->drec.m_dbt = m_BaseCode;
						ldata->drec.m_dbr = m_BaseEda;
						ldata->drec.m_dbmn = m_BaseBmn;
						ldata->drec.m_dkno = m_BaseKoji;
					}
					else {
						CREC.m_dbt = check.kd_code;
						CREC.m_dbr = check.kd_eda;
						CREC.m_cre = m_BaseCode;
						CREC.m_cbr = m_BaseEda;
						CREC.m_cbmn = m_BaseBmn;
						CREC.m_ckno = m_BaseKoji;

						ldata->drec.m_dbt = check.kd_code;
						ldata->drec.m_dbr = check.kd_eda;
						ldata->drec.m_cre = m_BaseCode;
						ldata->drec.m_cbr = m_BaseEda;
						ldata->drec.m_cbmn = m_BaseBmn;
						ldata->drec.m_ckno = m_BaseKoji;
					}
				}
				else {
					::ZeroMemory( &check.kd_name, sizeof check.kd_name );
				}
			}

			getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;
		}
		else
		{
			ldata = getLINE_DATA();
			if( ldata->drec.m_dbt == m_BaseCode ) {
				memcpy( check.kd_code, ldata->drec.m_cre, KMKCD_LN );
				check.kd_eda = ldata->drec.m_cbr;
			}
			else {
				memcpy( check.kd_code, ldata->drec.m_dbt, KMKCD_LN );
				check.kd_eda = ldata->drec.m_dbr;
			}

			pKn = pDBzm->DB_PjisToKnrec( check.kd_code );
			if( pKn )	sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
			else		::ZeroMemory( &check.kd_name, sizeof check.kd_name );

			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

			// 資金繰諸口枝番等セット
			set_etcdsp( ln, AC_KMK_PN );

			TerminationDataSet( AC_KMK_PN );
			return;
		}

		//消費税
		sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt( &CREC, NULL, acINP_mode) );
		//消費税再表示
		TerminationDataSet( AC_SYZ_PN );

		dc_sgn = (CREC.m_dbt == m_BaseCode) ? 1 : 0;

#ifndef NEWBRTKCLOSE	// 06.27 /00
		if( set_brtek )
		{
			// 枝番摘要を摘要に
			if( dc_sgn ) {
				if( ! brnitm_to_tky( ln, CREC.m_cbmn, CREC.m_cre, CREC.m_cbr ) )
					ldata->lcnd[ AC_KMK_PN ].INP_type = 3;	// 枝番登録に期待
			}
			else {			
				if( ! brnitm_to_tky( ln, CREC.m_dbmn, CREC.m_dbt, CREC.m_dbr ) )
					ldata->lcnd[ AC_KMK_PN ].INP_type = 3;	// 枝番登録に期待
			}
		}
#endif

		if( !nChar )
		{
			PROGRAM_OK = FALSE;
			{

//				if( !FNC_BUTTON )
				{
					//枝番チェック
					st = ac_brchk1( &CREC, dc_sgn );

					if( st == 99 )
					{
						//枝番登録選択
						if( ! dc_sgn ) 
							st = brzan_torok( this, CREC.m_dbmn, CREC.m_dbt, CREC.m_dbr, 0 );
						else
							st = brzan_torok( this, CREC.m_cbmn, CREC.m_cre, CREC.m_cbr, 1 );

						if( st == -1 )				//キャンセル
						{
							del_dtaeda( &CREC, dc_sgn );			//仕訳枝番消去
						}
					}
					else if( st == 2 || st == 3 ) {	
						// 枝番無しか登録MAXなので枝番名称を表示しない
					}

					if( st )
					{
						if( ! dc_sgn ) {
#ifndef NEWBRTKCLOSE	// 03.21 /00
							if( getLINE_DATA()->drec.m_dbr != CREC.m_dbr ) {
								// 入力枝番タイプOFFセット
								getLINE_DATA()->lcnd[ AC_KMK_PN ].INP_type = brn_dfltinptype( CREC.m_dbr );
							}
#endif
							getLINE_DATA()->drec.m_dbr = SREC.m_dbr = CREC.m_dbr;	// 0xffff
						}
						else {
							if( getLINE_DATA()->drec.m_cbr != CREC.m_cbr ) {
								// 入力枝番タイプOFFセット
								getLINE_DATA()->lcnd[ AC_KMK_PN ].INP_type = brn_dfltinptype( CREC.m_cbr );
							}
							getLINE_DATA()->drec.m_cbr = SREC.m_cbr = CREC.m_cbr;
						}
					}

				}
			}
			PROGRAM_OK = TRUE;

		}


		::ZeroMemory( &check, sizeof check );
		ldata = getLINE_DATA();

		if( ! dc_sgn ) {
			memcpy( check.kd_code, ldata->drec.m_dbt, KMKCD_LN );
			check.kd_eda = ldata->drec.m_dbr;
			pKn = pDBzm->DB_PjisToKnrec( ldata->drec.m_dbt );
		}
		else {
			memcpy( check.kd_code, ldata->drec.m_cre, KMKCD_LN );
			check.kd_eda = ldata->drec.m_cbr;
			pKn = pDBzm->DB_PjisToKnrec( ldata->drec.m_cre );
		}
		if( pKn )	sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		else		::ZeroMemory( check.kd_name, sizeof check.kd_name );
		
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );
		// 資金繰諸口枝番等セット
		set_etcdsp( ln, AC_KMK_PN );

		TerminationDataSet( AC_KMK_PN );

//		return;
//	}

NEXT:
	getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY = FALSE;

	if( !nChar )
	{
		return;
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		return;

	if( nChar )
	{
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_KAMOKU_PN:
		case SL_KMKANA_PN:
		case SL_BRNTKY_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
		if( !nChar )
			return;

		try {
			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
	//			MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合は
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}

	//			set_focus( AC_KMK_PN );
				return;
			}
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
	{
		DBkamokudata_get( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

		if( dc_sgn == -1 )
			dc_sgn = (SREC.m_dbt == m_BaseCode) ? 1 : 0;

		if( ! dc_sgn ) {
			eda_code = getLINE_DATA()->drec.m_dbr;
		}
		else {
			eda_code = getLINE_DATA()->drec.m_cbr;
		}
		
		// 枝番摘要検索＆表示
		if( (nChar == VK_TAB || (nChar == VK_RETURN && eda_code == -1 )) && _modefy_ok( getLINE_DATA()->drec.m_seq ) )
		{
			if( check.kd_code[0] != 0 ) {
				kmkcd_str = check.kd_code;
			}
			else {
				if( SREC.m_dbt == m_BaseCode ) {
					kmkcd_str = SREC.m_cre;
					check.kd_eda = SREC.m_cbr;
				}
				else {
					kmkcd_str = SREC.m_dbt;
					check.kd_eda = SREC.m_dbr;
				}

				memcpy( check.kd_code, kmkcd_str, KMKCD_LN );
				check.kd_eda;
				pKn = pDBzm->DB_PjisToKnrec( check.kd_code );
				if( pKn )	sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
				else		::ZeroMemory( check.kd_name, sizeof check.kd_name );
	
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );
			}

			BRNTKY_PAR bpar;
			bpar = m_Seldata.GetBrnTkyPar();
			if( !BRNTKY_sel || kmkcd_str != bpar.bt_code ) {
				SelPar par;
				par.sel_pn = SL_BRNTKY_PN;
				par.tkybrn = kmkcd_str;
				par.brn_bmn = getLINE_DATA()->drec.m_dbmn;

				m_Seldata.SelectJob( &par, -1);
			}

#ifdef BRN_KEEP_CLOSE
//			code = (check.ip_kmk.kamoku != 0 ? (unsigned short)check.ip_kmk.kamoku : (unsigned short)SREC.m_dbt);
			if( !BRNTKY_sel || kmkcd_str != TKYBRN_SEL.BRN_KCOD )
				if( BRNTKY_sel_set( brntky_sch( &TKYBRN_SEL, SelVal32, &m_selval32, kmkcd_str, 0, nChar == VK_TAB ? TRUE : FALSE )) )
				{
					getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY = TRUE;
					m_DtList.SetFocus();
				//	PostMessage( WM_RETROFOCUS, CRED_PN );
					PostMessage( WM_RETROFOCUS, AC_KMK_PN );

					if( nChar == VK_RETURN )	nChar = VK_TAB;	//#* 03.11 /02

				}
				else
				{
					//科目・摘要選択画面リセット
					BRNTKY_sel = 1;
					SelItemDsp( VK_HOME );
				}
#endif
		
		}
#ifdef DB_KEEP
		if( check.kd_code[0] == 0/*0xffff*/ )
		{
			::ZeroMemory( &check, sizeof check );
			ldata = getLINE_DATA();
			memcpy( check.kd_code, ldata->drec.m_dbt, KMKCD_LN );
			check.kd_eda = ldata->drec.m_dbr;
			strcpy( check.kd_name, ldata->drec.m_deb_name );

			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

			// 資金繰諸口枝番等セット
			set_etcdsp( ln, AC_KMK_PN );

			TerminationDataSet( AC_KMK_PN );
			return;
		}
		else if( !check.kd_code[0] && !BOTOM_SEQ )
#endif
		if( ! check.kd_code[0] )
		{
			Buzzer();
			return;
		}
		getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY = TRUE;

		if( /*!is_AutoInp_use() &&*/ nChar != VK_TAB ) {
			set_termfocus( get_nowln(), AC_KMK_PN, 1 );
		}
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 )
	{
		set_termfocus( get_nowln(), AC_KMK_PN, -1 );
	}	
	else
		Buzzer();

}


//---------------------------------------------------------
//	科目が借／貸どちらかを取得
//	return 0 = 借方, 1 = 貸方, -1 = 科目属性に従う
//
//---------------------------------------------------------
int CDBAbookView::GetKamokuSign() 
{
	int ret = -1;
	ACBOOK_LINE* al;
	al = getLINE_DATA();

	if( acINP_mode == _APPEND ) {
		// 新規追加
		if( al->lcnd[AC_KMK_PN].INP_sg ) {
			if( al->lcnd[AC_NKIN_PN].INP_sg ) {
				ret = 1;
			}
			else if( al->lcnd[AC_SKIN_PN].INP_sg ) {
				ret = 0;
			}
		}
	}
	else {
		// 修正
		if( al->drec.m_dbt == m_BaseCode ) {
			ret = 1;
		}
		else {
			ret = 0;
		}
	}

	return ret;
}



/***************************
	カナチェック
****************************/
void CDBAbookView::KanaTermKamoku(LPCTSTR data) 
{
	char kana[20] = {0};

	strcpy_s( kana, sizeof kana, (char *)data );

	SelPar par;
	if( BRNTKY_sel ) {
		par.sel_pn = SL_BRNTKY_PN;

		BRNTKY_PAR brp;
		brp = m_Seldata.GetBrnTkyPar();

		par.tkybrn	= brp.bt_code;
		par.brn_bmn = brp.bt_bmn;
		// 絞込み
		sprintf_s( par.brn_kana, sizeof par.brn_kana, "%s%s", saveITEM50kana, data );
	}
	else if( m_SelDispPN == SL_KMKANA_PN ) {
		par.sel_pn = SL_KMKANA_PN;
		sprintf_s( kana, sizeof kana, "%s%s", saveITEM50kana, data );
		memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
	}
	else {
		par.sel_pn = SL_KMKANA_PN;
		memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
	}

TRACE( ">> kanaterm %s\n", kana );

	int st = m_Seldata.SelectJob( &par, -1 );

	if( m_SelDispPN == SL_KMKANA_PN ) {
		strcpy_s( saveITEM50kana, sizeof saveITEM50kana, kana);	//save
	}
	else {
		::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
		Buzzer();
	}

}


void CDBAbookView::FocusIcsTekiyo( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	//別画面入力消去
	try {
		Destroy_ModeLessDlg();

		if( _dline_chg( ln, AC_TKY_PN, click ? 0 : 1 ) == -1 )
			return;

		//当項目まで自動入力
	//	if( click ) {
	//		if( acINP_mode == _APPEND && !is_AutoInp_use() )
	//			AutoInp( AC_TKY_PN );
	//	}
		// 取消データの場合
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}

		//直前のポジションが当ポジションより前で、金額が入力されていない場合は直前のポジションに返す。
	#ifdef CLOSE
		int set_pn;
		if( inp_ok( get_nowpn(), AC_TKY_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}
	#endif

	#ifdef CLOSE	// 06.28 /00
		m_TKsyz = 0;	//適用レコード消費税コード
	#endif

		if( get_nowpn() != AC_TKY_PN ) //#* 06.06 /03
		{
			SelPar sel;
			sel.sel_pn = SL_TKY_PN;
			sel.dbt = getLINE_DATA()->drec.m_dbt;
			sel.cre = getLINE_DATA()->drec.m_cre;
			::ZeroMemory( sel.tky_kana, sizeof sel.tky_kana );

			// 摘要選択を表示
			m_Seldata.SelectJob( &sel, -1);
		}
		
		set_nowpn( AC_TKY_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


void CDBAbookView::TerminationTekiyo( int ln, long nChar, long inplen, long kst )
{

	VARIANT var;
	char	buf[128];

	if( PROGRAM_OK == FALSE )
		return;

//	if( !nChar )
//	{

		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal != NULL ) {
			strcpy_s( buf, sizeof buf, (const char*)var.pbVal );
		}
		else {
			ZeroMemory( buf, sizeof buf );
		}
	
		if( !_modefy_ok( getLINE_DATA()->drec.m_seq ) )
		{

			if( strcmp( getLINE_DATA()->drec.m_val, buf ) )
			{
				Buzzer();
			}

			strcpy_s( buf, sizeof buf, getLINE_DATA()->drec.m_tekiyo );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
			TerminationDataSet( AC_TKY_PN );
		}
		else
		{
			if( getLINE_DATA()->lcnd[AC_TKY_PN].INP_sg == FALSE && ! buf[0] )
			{
				if( getLINE_DATA()->lcnd[AC_TKY_PN].NEXT_MOVE_KEY == FALSE )
//					return;
					goto NEXT;
				
				strcpy_s( buf, sizeof buf, SREC.m_tekiyo );
			}

			SREC.m_tekiyo = buf;
			CREC.m_tekiyo = buf;

			getLINE_DATA()->drec.m_tekiyo = buf;
			getLINE_DATA()->lcnd[AC_TKY_PN].INP_sg = TRUE;

			//消費税
			struct _SY_MSG_PACK smp;
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &CREC, &smp, acINP_mode) );
			//消費税再表示
			TerminationDataSet( AC_SYZ_PN );
#ifdef CLOSEKEEP
			char zei[6] = {0};
			l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
			memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
			memmove( SREC.m_zei, zei, sizeof(SREC.m_zei) );
			memmove( CREC.m_zei, SREC.m_zei, sizeof(SREC.m_zei) );

			getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
			TerminationDataSet( AC_ZEI_PN );
#endif

			::ZeroMemory( buf, sizeof buf );
			strcpy_s( buf, sizeof buf, getLINE_DATA()->drec.m_tekiyo );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
			TerminationDataSet( AC_TKY_PN );

		}
//		return;
//	}
NEXT:
	getLINE_DATA()->lcnd[AC_TKY_PN].NEXT_MOVE_KEY = FALSE;
		
	if( !nChar )
		return;

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		return;

	if( nChar )
	{
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_TKY_PN:
		case SL_TKYKANA_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
		if( !nChar )
			return;

		try {
			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
	//			MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}
			}
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
	{
		getLINE_DATA()->lcnd[AC_TKY_PN].NEXT_MOVE_KEY = TRUE;

		set_termfocus( get_nowln(), AC_TKY_PN, 1 );
	//	set_focus( AC_SYZ_PN );
	//	set_focus( AC_TKY_PN );
	}
	else if( nChar == VK_DELETE ) {
		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		int linelength = 0;
		int del_pos;
		if( var.pbVal != NULL )	{
			linelength = strlen( (const char*)var.pbVal );
		}

		// イメージ削除
		if(_modefy_ok( getLINE_DATA()->drec.m_seq ) )
		{
			switch( getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg )
			{
			case 1:	// イメージ摘要の取消
				memset( buf, '\0', sizeof(buf));
				var.pbVal = (BYTE*)buf;
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
				SREC.m_dsign[0] &= ~0x80;
				CREC.m_dsign[0] &= ~0x80;
				SREC.m_imgsq = 0;
				CREC.m_imgsq = 0;

				getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg = 2;
				TerminationDataSet( AC_TKY_PN );
				
			//	goto NEXT_POS1;
				break;

			case 2: // イメージ摘要の取消の取消
				if( ! linelength )
				{
					SREC.m_tekiyo.Empty();
					CREC.m_tekiyo.Empty();
					SREC.m_imgsq = getLINE_DATA()->imgseq;
					CREC.m_imgsq = getLINE_DATA()->imgseq;
					SREC.m_dsign[0] |= 0x80;
					CREC.m_dsign[0] |= 0x80;

					getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg = 1;
					TerminationDataSet( AC_TKY_PN );

			//		goto NEXT_POS2;
				}
			default:	// 通常の摘要の取消
				//カーソルポジション以降の文字列取り消し
				del_pos = m_Input.GetCaretPosition();
				buf[del_pos] = '\0';
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
				break;
			}
		}
		else
		{
			Buzzer();
			return;
		}
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_termfocus( get_nowln(), AC_TKY_PN, -1 );
	}
	else
		Buzzer();


}


//	摘要カナターミネーション
//   LPCTSTR data  カナ文字列
//   BOOL bTkcut   TRUE で摘要をセットしない
//
void CDBAbookView::_KanaTermIcsTekiyo(LPCTSTR data, BOOL bTkcut)
{
	int dc_sw;
	CDBipTKREC tkrec;
	struct _ItemData dmy_data = {0};


	if( tky_50kana_chk((char*)data) ) {
		//５０音摘要選択画面に表示
		SelPar par;
		par.sel_pn = SL_TKYKANA_PN;
		strncpy_s( par.tky_kana, sizeof par.tky_kana, m_tky_50kana, sizeof par.tky_kana );
		m_Seldata.SelectJob( &par, -1 );
		//５０音摘要があった
		if( m_SelDispPN == SL_TKYKANA_PN )
			return;
	}

	//摘要選択
	if( _modefy_ok( getLINE_DATA()->drec.m_seq ) && tky_select( &tkrec, (char *)data ) != -1 )
	{
		//摘要toデータ
		dc_sw = tky_select_to_drec( &CREC, &tkrec );
		//表示
		tky_select_dsp( &CREC, dc_sw, &tkrec, TRUE, bTkcut );
	}
	else
	{
#ifdef CLOSE	// 10.18 /99
		Buzzer();
#endif
//		data_reply( getINP_CTL( AC_TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
	}

}

//-----------------------------------------------------------
// 選択摘要toデータ
//   CDBipDataRec *CREC  仕訳レコード
//   CDBipTKREC *ptk     摘要レコード
//
//-----------------------------------------------------------
int CDBAbookView::tky_select_to_drec( CDBipDataRec *CREC, CDBipTKREC *ptk )
{
	int dc_sw = 0;

	if( acINP_mode == _APPEND )
	{
		//消費税コードチェック
		if( ptk->m_tksgn[1] )
		{
			m_TKsyz = getLINE_DATA()->lcnd[ AC_TKY_PN ].TKattr = ptk->m_tksgn[1];	//消費税コード
		}
		//自動仕訳
		if( m_bAutoSwk ) {
			if( ! getLINE_DATA()->lcnd[ AC_KMK_PN ].INP_sg ) {
				CString code;
				if( ! ptk->m_tkari.IsEmpty() || ! ptk->m_tkasi.IsEmpty() ) {
					if( ! ptk->m_tkari.IsEmpty() && ! ptk->m_tkasi.IsEmpty() ) {
						if( _stricmp( ptk->m_tkari, m_BaseCode ) == 0 ) {
							code = ptk->m_tkasi;
						}
						else if( _stricmp( ptk->m_tkasi, m_BaseCode ) == 0 ) {
							code = ptk->m_tkari;
						}
					}
					else {
						if( ! ptk->m_tkari.IsEmpty() )	code = ptk->m_tkari;
						else							code = ptk->m_tkasi;
					}
				}
				if( ! code.IsEmpty() ) {
					DBKNREC* pKn;
					pKn = pDBzm->DB_PjisToKnrec( code );
					if( pKn != NULL ) {
						if( (pKn->knatt & 0x01) ) {
							CREC->m_dbt = m_BaseCode;
							CREC->m_dbr = m_BaseEda;
							CREC->m_cre = code;
							getLINE_DATA()->drec.m_dbt = m_BaseCode;
							getLINE_DATA()->drec.m_dbr = m_BaseEda;
							getLINE_DATA()->drec.m_cre = code;
						}
						else {
							CREC->m_dbt = code;
							CREC->m_cre = m_BaseCode;
							CREC->m_cbr = m_BaseEda;
							getLINE_DATA()->drec.m_dbt = code;
							getLINE_DATA()->drec.m_cre = m_BaseCode;
							getLINE_DATA()->drec.m_cbr = m_BaseEda;
						}
						getLINE_DATA()->lcnd[ AC_KMK_PN ].INP_sg = TRUE;
						TerminationDataSet( AC_KMK_PN );
					}
				}
			}
		}

		m_TKsyz2 = 0;	// 前行分消費税コードクリア
	}

	//摘要枝番を検索
#ifndef CLOSE	// 07.14 /00
	if( Voln1->tk_br && BRmst && (get_nowpn() == AC_TKY_PN) )
#else
	if( Voln1->tk_br && BRmst )
#endif
	{
		dc_sw = tkybrn_search( CREC, ptk->m_tkcod, &getLINE_DATA()->lcnd[0] );
	}

// TODO 摘要消費税属性処理
#ifdef LATER_CLOSE
		if( acINP_mode == _APPEND && m_TKsyz && pDBsy->IsSyohizeiMaster()
											&& (Voln1->s_sgn3 & 0x02) )
		{
			pDBsy->Sy_tkyo_inp( CREC, m_TKsyz, 1 );
		}
#endif

	return dc_sw;
}

//-----------------------------------------------------------
// 摘要セットデータ表示
//   CDBipDataRec *CREC  仕訳レコード
//	 int dc_sw           借貸 サイン
//   CDBipTKREC *ptk     摘要レコード
//   BOOL F12_KEY        IME のターミネーション
//   BOOL bTkcut         摘要をセットしない(=TRUE)
//-----------------------------------------------------------
void CDBAbookView::tky_select_dsp( CDBipDataRec *CREC, int dc_sw, CDBipTKREC *ptk, BOOL F12_KEY, BOOL bTkcut/*=FALSE*/ )
{

	//摘要枝番表示
	if( dc_sw & 0x01 )
	{
		//借方枝番セット＆表示
		SREC.m_dbr = CREC->m_dbr;
		getLINE_DATA()->drec.m_dbr = CREC->m_dbr;
		// 資金繰諸口枝番等セット
		set_etcdsp( get_nowln(), AC_KMK_PN );
		TerminationDataSet( AC_KMK_PN );
	}
	if( dc_sw & 0x02 )
	{
		//貸方枝番セット＆表示
		SREC.m_cbr = CREC->m_cbr;
		getLINE_DATA()->drec.m_cbr = CREC->m_cbr;
		// 資金繰諸口枝番等セット
		set_etcdsp( get_nowln(), AC_KMK_PN );
		TerminationDataSet( AC_KMK_PN );
	}

	//#* 03.14 /02 ... 摘要追加
	if( !bTkcut ) {
		//仕訳摘要表示
		char tkstr[64];
		strcpy_s( tkstr, sizeof tkstr, ptk->m_tkname );

#ifdef DB_OLD_CLOSE
		sprintf( fmt, "%%.%ds", sizeof( ptk->TKnam ) );
		sprintf( (char *)ins_data.IP_TKY, fmt, ptk->TKnam );

		if( F12_KEY ) 
			;
			data_reply( getINP_CTL( AC_TKY_PN )->IDC_X, 1, &ins_data, CICSInput );
		else
		{
			((CICSInput*)GetDlgItem( getINP_CTL( AC_TKY_PN )->IDC_X ))->TekiInsert( (char*)ins_data.IP_TKY );	// 04.08 /99 移動
	#ifndef CLOSE	// 09.07 /98
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationIcsinputTky( this, get_nowln(), 0, 0, 0 );
	#endif
#endif
		
		if( get_nowpn() == AC_TKY_PN ) {
			VARIANT var;
			var.pbVal = (BYTE*)tkstr;
			short pos = m_Input.GetCaretPosition();
			m_Input.InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationTekiyo( get_nowln(), 0, 0, 0 );
		}
		else {
			// 現レコードにセット
			int len = CREC->m_tekiyo.GetLength();
			int byte = (Voln1->tk_ln*2);
			if( len < byte ) {
				CREC->m_tekiyo += tkstr;
				len = CREC->m_tekiyo.GetLength();
				if( len > byte ) {
					int n = get_strcnt( (char*)(LPCTSTR)CREC->m_tekiyo, byte );
					CREC->m_tekiyo.SetAt( n+1, '\0' );
				}
			}
		}
	}
	else {
		// 摘要追加せず
	//	if( F12_KEY )
	//		;
	//		data_reply( getINP_CTL( AC_TKY_PN )->IDC_X, 0, &ins_data, CICSInput );
	}

	//消費税
	strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( CREC, NULL, acINP_mode) );
	//消費税再表示
	TerminationDataSet( AC_SYZ_PN );

}


/* -----------------------------
	摘要枝番　サーチ＆セット
------------------------------- */
int CDBAbookView::tkybrn_search( CDBipDataRec *data, int tkcd, ABLN_CND* cnd )
{
int ret = 0;
CDBipDataRec sv;
int sv_dbr, sv_cbr, dcsgn;

	sv = *data;

	if( m_BaseCode == data->m_dbt )	dcsgn = 1;
	else							dcsgn = 0;

	if( (cnd+AC_KMK_PN)->INP_type != 1 )
	{
		if( m_BaseCode == data->m_dbt ) {
			dcsgn = 1;	// 相手科目が貸方
			sv_cbr = data->m_cbr;
			data->m_cbr = -1;
		}
		else {
			dcsgn = 0;	// 相手科目が借方
			sv_dbr = data->m_dbr;
			data->m_dbr = -1;		
		}

	}

	pDBzm->DB_TekiyoEdabanSet( data, tkcd );

	if( (cnd+AC_KMK_PN)->INP_type != 1 ) {
		if( ! dcsgn ) {
			if( data->m_dbr != -1 && data->m_dbr != sv.m_dbr )
			{
				(cnd+AC_KMK_PN)->INP_type = 2;	// 自動

				ret |= 0x01;	//借方にセット
			}
			else
				data->m_dbr = sv_dbr;
		}
		else {
			if( data->m_cbr != -1 && data->m_cbr != sv.m_cbr )
			{
				(cnd+AC_KMK_PN)->INP_type = 2;	// 自動

				ret |= 0x02;	//貸方にセット
			}
			else
				data->m_cbr = sv_cbr;
		}
	}

	return ret;
}



int CDBAbookView::FocusIcsVal( int ln, int ns_sw, short click )
{
	if( PROGRAM_OK == FALSE )
		return -1;

	//別画面入力消去
	try {
		Destroy_ModeLessDlg();
		int setpn, pn;
		pn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;

		if( _dline_chg( ln, pn, click ? 0 : 1 ) == -1 )
			return -1;

	/*	if( click ) {
			//当項目まで自動入力
			if( acINP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( pn );
		}
	*/
		// 取消データの場合
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return -1;
		}
		else {
			// 入金 および 出金 チェック
			ACBOOK_LINE* al;
			al = getLINE_DATA();

			char* pNKIN = "入金";
			char* pSKIN = "出金";
			CString str;

			int chg = 0;
			if( ns_sw == 0 ) {
				if( al->lcnd[AC_SKIN_PN].INP_sg ) {	// 出金 → 入金
					chg = 1;
					str.Format( "金額を%sから%sに変更します。", pSKIN, pNKIN );
				}
			}
			else {
				if( al->lcnd[AC_NKIN_PN].INP_sg ) { // 入金 → 出金
					chg = 2;
					str.Format( "金額を%sから%sに変更します。", pNKIN, pSKIN);
				}		
			}

			setpn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;
			if( chg && (acINP_mode != _APPEND) ) {
				PROGRAM_OK = FALSE;

	TRACE("!!++Abook FocusVal 01 -setpn %d\n", setpn);

				int st = ICSMessageBox( str, MB_OKCANCEL );
				
				PROGRAM_OK = TRUE;

				if( st == IDOK ) {	// 入金・出金 の変更
					setpn = (chg == 1) ? AC_NKIN_PN : AC_SKIN_PN;
					ChangeDebtCred();
					set_focus( setpn );
				}
				else {
					// TerminationValの先頭で return させるため、 現在ポジションを今回のフォーカスにセット
					setpn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;
					set_nowpn( setpn );
					// 現在の 入・出金にフォーカスを戻す
					setpn = (chg == 1) ? AC_SKIN_PN : AC_NKIN_PN;
					set_focus( setpn );
					return 0;
				}
TRACE("!!++Abook FocusVal 02 -setpn %d\n", setpn );

			}

			set_nowpn( setpn );
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return 0;
}


void CDBAbookView::TerminationVal( int ln, int ns_sw, long nChar, long inplen, long kst )
{
	VARIANT var;
	char	ip_val[6];
	int pn;
	pn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;

TRACE("!!++Abook TerminationVal, dbt='%s', cre='%s'\n", getLINE_DATA()->drec.m_dbt, getLINE_DATA()->drec.m_cre );

	if( PROGRAM_OK == FALSE )
		return;

	// 入金・出金とは 反対のターミネーションは無視
	if( getLINE_DATA()->drec.m_dbt != getLINE_DATA()->drec.m_cre ) {
		if( m_BaseCode == getLINE_DATA()->drec.m_dbt ) {
			if( pn == AC_SKIN_PN )	return;
		}
		else {
			if( pn == AC_NKIN_PN )	return;	
		}
	}

//	if( !nChar )
//	{

		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
		if( var.pbVal != NULL )		memcpy( ip_val, var.pbVal, 6 );
		else						ZeroMemory( ip_val, sizeof ip_val );
	
		if( !_modefy_ok( getLINE_DATA()->drec.m_seq ) )
		{
			if( l_jsgn( getLINE_DATA()->drec.m_val, ip_val, _BIN & 0x0f ) )
			{
				Buzzer();
			}
			memcpy( ip_val, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = (BYTE*)ip_val;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			TerminationDataSet( pn );
		}
		else
		{
			if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && !l_test( ip_val ) /*&& !((CEdit*)pwnd->GetDlgItem( getINP_CTL( pwnd, pn )->IDC_X ))->LineLength()*/ )
			{
				if( getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY == FALSE )
//					return;
					goto NEXT;
					memmove( ip_val, SREC.m_val, 6 );
			}

			memmove( SREC.m_val, ip_val, sizeof(SREC.m_val) );
			memmove( CREC.m_val, SREC.m_val, sizeof(SREC.m_val) );

			memmove( getLINE_DATA()->drec.m_val, ip_val, sizeof(ip_val) );

			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

			//消費税
			struct _SY_MSG_PACK smp;
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &CREC, &smp, acINP_mode) );
			//消費税再表示
			TerminationDataSet( AC_SYZ_PN );
#ifndef KEEP
			char zei[6] = {0};
			l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
			memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
			memmove( SREC.m_zei, zei, sizeof(SREC.m_zei) );
			memmove( CREC.m_zei, SREC.m_zei, sizeof(SREC.m_zei) );

			getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
			TerminationDataSet( AC_ZEI_PN );
#endif

			::ZeroMemory( ip_val, sizeof ip_val );
			memcpy( ip_val, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = (BYTE*)ip_val;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			TerminationDataSet( pn );
		}
//		return;
//	}
NEXT:
	getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = FALSE;
		
	if( !nChar )
		return;

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		return;

	if( nChar )
	{
		try {
			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
	//			MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}

				return;
			}
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
	{
		getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = TRUE;

		int vect;
		vect = (nChar == VK_RETURN ) ? 1 : 2;
		//消費税項目カーソル位置チェック
		m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(vect);
		if( m_SET_SYZINP_PN >= 0 )
			set_termfocus( get_nowln(), AC_SYZ_PN, 1 );
#ifdef KEEP_CLOSE
		//変動事由チェック
		else if( get_khkbntype( CREC.m_dbt, CREC.m_cre, pDBzm ) >= 0 ) {
			set_termfocus( get_nowln(), SKBN_PN, 1 );
		}
#endif
		else {
			


			set_focus( AC_TKY_PN );
		}
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_termfocus( get_nowln(), pn, -1 );
	}
	else
		Buzzer();
}


// フォーカス 税額
int CDBAbookView::FocusIcsZei( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return -1;

	//別画面入力消去
	try {
		Destroy_ModeLessDlg();

		if( _dline_chg( ln, AC_ZEI_PN, click ? 0 : 1 ) == -1 )
			return -1;

	/*	if( click ) {
			//当項目まで自動入力
			if( acINP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( pn );
		}
	*/
		// 取消データの場合
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return -1;
		}
		else {
			//直前のポジションが当ポジションより前で、貸方科目が入力されていない場合は直前のポジションに返す。
	//		int set_pn;
	//		if( inp_ok( get_nowpn(), ZEI_PN, &set_pn ) == FALSE )
	//		{
	//			set_nowpn( set_pn );
	//			set_focus( /*get_nowpn()*/set_pn );
	//			return;
	//		}

			set_nowpn( AC_ZEI_PN );
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return 0;
}

// ターミネーション 税額
void CDBAbookView::TerminationZei( int ln, long nChar, long inplen, long kst )
{
	VARIANT var;
	char	ip_val[6];
	char	buf[64];

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
//		if( !nChar )
//		{
			l_defn( _BIN );

			DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			if( var.pbVal != NULL )	memcpy( ip_val, var.pbVal, 6 );
			else					memset( ip_val, '\0', sizeof ip_val );
		
			if( !_modefy_ok( getLINE_DATA()->drec.m_seq ) )
			{
				if( l_jsgn( getLINE_DATA()->drec.m_zei, ip_val, _BIN & 0x0f ) )
				{
					Buzzer();
				}
				memcpy( ip_val, getLINE_DATA()->drec.m_zei, 6 );
				var.pbVal = (BYTE*)ip_val;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
				TerminationDataSet( AC_ZEI_PN );
			}
			else
			{
				if( getLINE_DATA()->lcnd[ZEI_PN].INP_sg == FALSE && !l_test( ip_val ) && ! inplen  )
				{
					if( getLINE_DATA()->lcnd[VAL_PN].NEXT_MOVE_KEY == FALSE )
//						return;
						goto NEXT;

//					memmove( ip_val, SREC.m_zei, 6 );
				}

				memmove( getLINE_DATA()->drec.m_zei, ip_val, sizeof(ip_val) );
				getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;

				//消費税
			//	getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, INP_mode);
				//消費税再表示
			//	TerminationDataSet( SKBN_PN );
			//	::ZeroMemory( ip_val, sizeof ip_val );
				
				l_print( buf, ip_val, FMT15 );
				if( pDBsy->SyZei( buf ) == 0 ) {
					struct _SY_MSG_PACK smpk;
					pDBsy->SyMsg( &smpk );
					CDBipDataRec rec;
					pDBzm->GetCDBData( &rec );

					memmove( SREC.m_zei, ip_val, sizeof(SREC.m_zei) );
					memmove( CREC.m_zei, SREC.m_zei, sizeof(SREC.m_zei) );

					memcpy( getLINE_DATA()->drec.m_zei, rec.m_zei, 6 );
					memcpy( ip_val, rec.m_zei, 6 );
					memcpy( getLINE_DATA()->drec.m_val, rec.m_val, 6 );

					var.pbVal = (BYTE*)ip_val;
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
					TerminationDataSet( AC_ZEI_PN );
				//	TerminationDataSet( AC_VAL_PN );
				}
				else {
					memmove( ip_val, CREC.m_zei, sizeof(CREC.m_zei) );
					var.pbVal = (BYTE*)ip_val;
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
				
				}
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->lcnd[VAL_PN].NEXT_MOVE_KEY = FALSE;
			
		if( !nChar )
			return;

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			try {
				nChar = dline_chg( nChar );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return;
				}
				else if( !nChar )
				{
					// 取消データの場合
					if( isdeldata() )
					{
						set_focus( AC_DATE_PN );
						return;
					}

					set_focus( AC_ZEI_PN );
					return;
				}
			}
			catch ( CErrBlk E )
			{
				ErrExit( E.ErrCode, E.ErrMsg );
				return;
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
		{
			getLINE_DATA()->lcnd[AC_ZEI_PN].NEXT_MOVE_KEY = TRUE;
			
		//	if( !is_AutoInp_use() )
		//	{
				set_termfocus( get_nowln(), AC_ZEI_PN, 1 );
		//	}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 ) {
			set_termfocus( get_nowln(), AC_ZEI_PN, -1 );
		}
		else
			Buzzer();
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}

void CDBAbookView::FocusIcsSyz( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	//別画面入力消去
	Destroy_ModeLessDlg();

	try {

		if( _dline_chg( ln, AC_SYZ_PN, click ? 0 : 1 ) == -1 )
			return;

	/*	if( click ) {
			//当項目まで自動入力
			if( acINP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( VAL_PN );
		}
	*/
		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		else {
			//消費税
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &CREC, NULL, acINP_mode) );
			//消費税再表示
			TerminationDataSet( AC_SYZ_PN );

			set_nowpn( AC_SYZ_PN );
		}

		if( SyzInpDisp() != 0 )
		{
			PROGRAM_OK = FALSE;
			Destroy_SyzInp();
			PROGRAM_OK = TRUE;

			// 空の消費税区分欄表示
			PostMessage( WM_SKBN_ICHIRN, (WPARAM)0, (LPARAM)0 );
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

	return;
}


// 消費税欄 ターミネーション
int CDBAbookView::TerminationSyz( int ln, long nChar, long inplen, long kst )
{
	int sv_ln = get_nowln();
//MSG msg = {0};

	if( PROGRAM_OK == FALSE )
		return -1;

	ACBOOK_LINE* al;

	try
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return 0;

		if( nChar == VK_RETURN || nChar == VK_TAB ) {
			// 仕訳書き込み処理
			if( acINP_mode == _APPEND ) {
				WriteAndNextData();
				nChar = 0;
			}
			else {
				nChar = VK_DOWN;
			}
		}

		if( nChar )
		{
			try {
				nChar = dline_chg( nChar );
				if( nChar == -1 )
				{
					return -1;
				}
				else if( !nChar )
				{
#ifndef CLOSE	// 08.25 /98
				/*	if( sv_ln == get_nowln() )
						FocusIcsSyz( get_nowln(), 0 );
					else
					{
						_set_focus( get_nowln(), AC_SYZ_PN );
					}
				*/
					if( sv_ln != get_nowln() )
						_set_focus( get_nowln(), AC_SYZ_PN );
#else
					set_focus( VAL_PN );
					set_focus( SKBN_PN );
#endif
					return -1;
				}
			}
			catch ( CErrBlk E )
			{
				ErrExit( E.ErrCode, E.ErrMsg );
				return -1;
			}
		}

		if( !nChar )
		{
			return 0;
		}

		switch( nChar )
		{
		case VK_END:
			break;
		case VK_TAB:
		case VK_RETURN:
			// TODO 仕訳書き込み および 行変更
//		case VK_RIGHT:
//		case VK_F3:
			break;
		case VK_LEFT:
		case VK_F2:
			// 金額に戻る
			al = getLINE_DATA();
			if( al->drec.m_dbt == m_BaseCode ) {
				set_focus( AC_NKIN_PN );
			}
			else {
				set_focus( AC_SKIN_PN );
			}
			break;
		default:
			Buzzer();
			return -1;
			break;

		}
		return 0;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}
}


//---------------------------------------------
//	データ追加 ＆ 次データ 作成
//
//
//---------------------------------------------
int CDBAbookView::WriteAndNextData()
{
	int nowln;
	nowln = get_nowln();
	int lcnt;
	lcnt = m_List.GetDispDataCount();

	if( pDBzm->DB_DataAppend( &CREC ) != 0 )
		return -1;

	// 次の SEQ 番号
	int seq = CREC.m_seq;
	m_LASTSEQ = seq;
	seq++;

	m_ALine[nowln-1].drec.m_seq = CREC.m_seq;

	if( lcnt <= nowln ) {
		// 一行スクロールする
		acline_scroldown();
	}
	else {
		nowln++;
	}

	// 新しいデータをセット
	SREC = CREC;
	DataAppendReset( &CREC );
	ListDataSet();

	CREC.Reset();
	CREC.m_seq		= seq;
	CREC.m_ddate	= SREC.m_ddate;
	CREC.m_mofs		= SREC.m_mofs;
	CREC.m_ksign	= SREC.m_ksign;

	m_InsLine = 0;

//	SzanUpdate( nowln, CREC.m_val, NULL, 0 );
	// 差引残高更新
//	SzanDataSet(nowln);
	
	// _dline_chg の DB_DataCorrect() 関数を 呼ばないように 現在行を初期化
	set_nowln(-1);

	_set_focus( nowln, DATE_PN );

	return 0;
}




//消費税入力画面表示
int CDBAbookView::SyzInpDisp()
{
	if (m_pSyzInp == NULL)
	{
		ermset( ERROR_ENV, "消費税入力オブジェクトがありません。");
		return -1;
	}
	else
	{
//		CWnd *org;
		int sw;
		double rate_w, rate_h;
		double vrate = (flgPen ? 84 : 100);
//		get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
		rate_w = rate_h = 1.0;

		CSize size;
		m_pSyzInp->GetCrntRect( size );
		CRect	irect;

		m_List.GetItemScreenRect( 0, DATA_SYZ, irect );

		m_Input.GetWindowRect( irect );
		this->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK = _modefy_ok( getLINE_DATA()->drec.m_seq);
		sw = m_pSyzInp->DialogON( bOK, irect, SYP_TOPLEFT, m_SET_SYZINP_PN );

		return sw;
	}
}


void CDBAbookView::Hide_SyzInp()
{
	CDBbaseView::Destroy_SyzInp();

	if( get_nowln() == -1 )	return;

	//消費税
	strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &CREC, NULL, acINP_mode) );
	//消費税再表示
	TerminationDataSet( AC_SYZ_PN );
}



// 消費税区分一覧表示
LRESULT CDBAbookView::DspSkbnIchiran( WPARAM wParam, LPARAM lParam )
{
	DWORD i;
	CString *str = (CString*)lParam;

#define SYZSEL_MAX	8*4

static struct _SelTable SelVal32[SYZSEL_MAX];
static SelSetProcPar	spar;

	spar.sp_prevpn = m_SelDispPN;
	spar.sp_nowpn = BVW_SYZ_PN;
	strcpy_s( spar.seljob, sizeof spar.seljob, "《消費税》" );

	// 現在内容のセーブ と　表示
	for( i = 0 ; i < SYZSEL_MAX ; ++i )
	{
		// 表示
		SelVal32[i].Data = i < wParam ? *(str+i) : "";
	//	SelVal32[i].number = i + (isEntrySys() ? 9 : 1);
		SelVal32[i].number = i + 1;
	}

	SetSelectSel32( SelVal32, &spar, SYZSEL_MAX, this );

	return TRUE;
}


// キーによる 消費税ダイアログEND
LRESULT CDBAbookView::OnSkbnInend( WPARAM wParam, LPARAM lParam )
{
TRACE( "--++ CDBAbookView syohi KeyEnd %d\n", wParam );

	if( TerminationSyz( get_nowln(), wParam, 0, 0 ) != -1 )
	{
		TerminationSyz( get_nowln(), 0, 0, 0 );
	}
	else if( m_pSyzInp )
	{
		m_pSyzInp->set_nowfocus();
	}

	return 1;
}

// 消費税 の属性を再表示
LRESULT CDBAbookView::OnSkbnLineDisp( WPARAM wParam, LPARAM lParam )
{
TRACE( "--++ CDBAbookView syohi OnSkbnLineDisp\n" );

	struct _SY_MSG_PACK smpk;
	pDBsy->SyMsg( &smpk );

	pDBzm->GetCDBData( &CREC );
	sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_line_txt( &CREC ) );
	memmove( getLINE_DATA()->drec.m_zei, CREC.m_zei, 6 );
	getLINE_DATA()->lcnd[AC_SYZ_PN].INP_sg = TRUE;
	getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
	TerminationDataSet( AC_SYZ_PN );
	TerminationDataSet( AC_ZEI_PN );

	return 1;
}


void CDBAbookView::set_etcdsp( int ln, int pn )
{

}

// ターミネーション時のデータセット
void CDBAbookView::TerminationDataSet( int pn, int line/*=-1*/ ) 
{
	DTLIST_DATA data;
	int ln;
	
	if( line == -1 )	ln = get_nowln();
	else				ln = line;

	if( ln <= 0 )	return;

	m_List.GetDtListData( ln-1, &data );
	
	ACBOOK_LINE *dtline;
	dtline = _getLINE_DATA( ln );

	_TerminationDataSet( &data, dtline, &pn, 1 );

	m_List.SetDtListData( ln-1, &data );
}

// ターミネーション時のデータセット 複数項目
void CDBAbookView::TerminationDataSet( int pn[], int pncnt, int line/*=-1*/ ) 
{
	DTLIST_DATA data;
	int ln;
	
	if( line == -1 )	ln = get_nowln();
	else				ln = line;

	m_List.GetDtListData( ln-1, &data );
	
	ACBOOK_LINE *dtline;
	dtline = _getLINE_DATA( ln );

	_TerminationDataSet( &data, dtline, pn, pncnt );

	m_List.SetDtListData( ln-1, &data );
}


void CDBAbookView::_TerminationDataSet( DTLIST_DATA* data, ACBOOK_LINE *dtline, int pntbl[], int tblcnt ) 
{
	BYTE ksgn;
	int pn;
	DBKNREC* pKn;

	data->dd_dsign = 0;

	// 転送済みか？
	if( _tline_chk( dtline->drec.m_seq )) {
		data->dd_dsign |= 0x02;
	}
	// 取消し
	if( dtline->drec.m_dsign[0] & 0x01 ) {
		data->dd_dsign |= 0x01;
	}

	for( int i = 0; i < tblcnt; i++ ) {
		pn = pntbl[i];
//		data->dd_ctbl[pn] = dtline->drec.m_ctbl[pn];

		switch( pn ) {
		case AC_SEQ_PN:
		case AC_DATE_PN:
			data->dd_ksign = 0;
			data->dd_mofs = 0;
			if( dtline->lcnd[AC_DATE_PN].INP_sg ) {
				data->dd_ddate = dtline->drec.m_ddate;
				if( data->dd_ddate > 0 ) {
					ksgn = KESY_SG_GET( dtline->bcddate[0] );
					data->dd_ksign = !ksgn ? 0 : 1;
					BYTE tmp[10] = {0};
					memcpy( tmp, dtline->bcddate, 2 );

					CVolDateDB vd;
					int offset;
				//	if( ksgn > 0 )	tmp[0] = (ksgn==1) ? 0x00 : 0x60;
					switch( ksgn ) {
					case 1: tmp[0] = 0x61;	break;
					case 2: tmp[0] = 0x62;	break;
					case 3: tmp[0] = 0x63;	break;
					case 4: tmp[0] = 0x00;	break;
					default:
						break;
					}
					if( vd.db_vd_offset( tmp, &offset, pDBzm ) != 0 ) {
						ICSMessageBox( "月のオフセットを取得できません！ ");
						return;
					}
					data->dd_mofs = offset;
				}
			}
			else {
				data->dd_ddate = 0;
			}
			break;
//		case AC_DENP_PN:
//			data->dd_cno = dtline->drec.m_cno;
//			break;
		case AC_BMON_PN:
			data->dd_dbmn = dtline->drec.m_dbmn;
			data->dd_dbmn_etc = dtline->lcnd[AC_BMON_PN].Etcmsg;
			break;
		case AC_KOJI_PN:
			data->dd_dkno = dtline->drec.m_dkno;
			data->dd_dkno_etc = dtline->lcnd[AC_KOJI_PN].Etcmsg;
			break;
		case AC_KMK_PN:
			if( dtline->drec.m_dbt == m_BaseCode ) {
				data->dd_dbt = dtline->drec.m_cre;
				data->dd_dbr = dtline->drec.m_cbr;

				pKn = pDBzm->DB_PjisToKnrec( data->dd_dbt );
				if( pKn )	data->dd_deb_name.Format( "%.14s", pKn->knnam );
			}
			else {
				data->dd_dbt = dtline->drec.m_dbt;
				data->dd_dbr = dtline->drec.m_dbr;

				pKn = pDBzm->DB_PjisToKnrec( data->dd_dbt );
				if( pKn )	data->dd_deb_name.Format( "%.14s", pKn->knnam );			
			}

			data->dd_deb_etc = dtline->lcnd[AC_KMK_PN].Etcmsg;
			break;
/*		case AC_CBMN_PN:
			data->dd_cbmn = dtline->drec.m_cbmn;
			data->dd_cbmn_etc = dtline->drec.m_cbmn_etc;
			break;
		case AC_CKJI_PN:
			data->dd_ckno = dtline->drec.m_ckno;
			data->dd_ckno_etc = dtline->drec.m_ckno_etc;
			break;
		case AC_CRED_PN:
			data->dd_cre = dtline->drec.m_cre;
			data->dd_cbr = dtline->drec.m_cbr;
			data->dd_cre_name.Format( "%.14s", dtline->drec.m_cre_name );
			data->dd_cre_etc = dtline->drec.m_cre_etc;
			break;
*/
		case AC_NKIN_PN:
			memcpy( data->dd_val, dtline->drec.m_val, 6 );
			data->val_flag = dtline->lcnd[AC_NKIN_PN].INP_sg;
			break;

		case AC_SKIN_PN:
			data->dd_udtbl[0].userdata = (BYTE*)dtline->drec.m_val;
			data->dd_udtbl[0].initflag = dtline->lcnd[AC_SKIN_PN].INP_sg;
			break;

		case AC_ZEI_PN:
			memcpy( data->dd_zei, dtline->drec.m_zei, 6 );
			data->zei_flag = dtline->lcnd[AC_ZEI_PN].INP_sg;
			break;
		
		case AC_SYZ_PN:
			data->dd_syz_str = dtline->syzstr;
			break;
		case AC_TKY_PN:
			if( dtline->lcnd[ AC_TKY_PN ].IMG_sg == 1 && dtline->imgseq > 0 ) {
				data->dd_tek.tekiyo = "";

			//	if( pDBzm->image->SetAbsolutePosition( dtline->imgseq ) == 0 )
			//	{
			//		IMREC* pIMREC = pDBzm->image->DecodeImage();
			//		data->dd_tek.width = pIMREC->dotxs;
			//		data->dd_tek.height = pIMREC->dotys;
			//		data->dd_tek.image = (char*)pIMREC->imdata;
			//	}
			//	else {
					data->dd_tek.image = 0;
					data->dd_tek.width = 0;
					data->dd_tek.height = 0;
			//	}
			}
			else {
				data->dd_tek.tekiyo = dtline->drec.m_tekiyo;
				data->dd_tek.image = 0;
				data->dd_tek.width = 0;
				data->dd_tek.height = 0;
			}
			break;
		case AC_SZAN_PN:
L_PRINT( "!! termdataset szan", dtline->szan );

			data->dd_udtbl[1].userdata = (BYTE*)dtline->szan;
			data->dd_udtbl[1].initflag = dtline->lcnd[AC_SZAN_PN].INP_sg;
			break;

/*		case AC_SYON_PN:
			data->dd_snumber = dtline->drec.m_snumber;
			break;
		case AC_TGDT_PN:
			data->dd_tdate = dtline->drec.m_tdate;
			pDBzm->IntsrekitoGenstr( data->dd_tdstr.GetBuffer(30), data->dd_tdate );
			data->dd_tdstr.ReleaseBuffer();
			break;

		case SKIN_PN:

			break;
		case HSEN_PN:
			data->dd_husen = dtline->drec.m_husen;
			break;
*/		}
	}
}


BOOL CDBAbookView::modefy_ok( CDBipDataRec *drec  )
{
	return _modefy_ok( drec->m_seq );
}

BOOL CDBAbookView::_modefy_ok( int darec_seq )
{
	if( acINP_mode != _APPEND && _tline_chk( darec_seq ) && !pDBzm->zvol->tl_cor )
		return FALSE;
	else
		return TRUE;
}


int* CDBAbookView::acfnINP_mode()
{
	return &bINP_mode;
}


// 消費税イニシャライズ
int CDBAbookView::DB_SyIniz( CDBipDataRec* data )
{
	pDBzm->SetCDBData( data );
	int st = pDBsy->SyIniz( pDBzm->dbdata );
//TRACE( "pDBsy->SyIniz st = %d\n", st );
	return st;
}

char *CDBAbookView::sy_chk_txt( CDBipDataRec *data,  struct _SY_MSG_PACK* smp, int inp_mode )
{
//ASSERT( data->m_seq == pDBzm->dbdata->seq );

	static CString txt;

	struct _SY_MSG_PACK smpk, *pSmp;
	pSmp = (smp == NULL) ? &smpk : smp;

	pDBzm->SetCDBData( data );
	if( pDBsy->SyMsg( pSmp ) == 0) {
	//	txt = pSmp->SY_LIN_MSG;
		txt = pDBsy->Sy_get_message( pDBzm->dbdata, 0x03 );
		data->m_dsign.Copy( pDBzm->dbdata->dsign );
	}
	else	txt.Empty();

#ifdef OLD_CLOSE
	pDBsy->Sy_chk_sign( data, (inp_mode != _APPEND ? 1 : 0 ) );

	if( inp_mode == _APPEND )
		if( m_TKsyz )
			pDBsy->Sy_tkyo_inp( data, m_TKsyz, 1 );	// 摘要属性
			
	txt = pDBsy->getSYLINETXT();
#endif

	return (char*)(LPCTSTR)txt;

}

// Sy_line_dsp() text get
char *CDBAbookView::sy_line_txt( CDBipDataRec *data )
{
//ASSERT( data->m_seq == pDBzm->dbdata->seq );

	static CString txt;
	if( data->m_seq != pDBzm->dbdata->seq ) {
		pDBzm->SetCDBData( data );
	}
	txt = pDBsy->Sy_get_message( pDBzm->dbdata, 0x03 );

#ifdef KEEP
	struct _SY_MSG_PACK smpk;
	if( pDBsy->SyMsg( &smpk ) == 0 ) {
		txt = smpk.SY_LIN_MSG;
	}
	else	txt.Empty();
#endif

	return (char*)(LPCTSTR)txt;

}


/*===============================
	枝番摘要toデータ摘要
================================*/
BOOL CDBAbookView::brnitm_to_tky( int ln, int bmn, CString& code, int brn )
{
	int dc_sw, tno;
	CDBipTKREC tkrec;

	// 枝番摘要を検索
	if( pDBzm->zvol->tk_br /*&& BRmst*/ )
	{
		// 枝番to摘要レコードリード
		if( tno = m_BrTek.get_brtek( bmn, code, brn ) )
		{

// TODO 枝番摘要を 仕訳摘要に
//#ifdef KEEP_CLOSE
			
			if( tno > 0 ) {
				if( pDBzm->DB_TekiyoNoRead( &tkrec, tno ) == ERR )
					return FALSE;
				//摘要toデータ
				dc_sw = tky_select_to_drec( &CREC, &tkrec );
			}
			else {
				// 枝番残高本体に登録された枝番名称
				if( pDBzm->DB_EdabanZanSearch( code, brn ) != 0 )
					return FALSE;

				tkrec.Reset();
				tkrec.m_tkname = pDBzm->ezrec->ezname;
				dc_sw = 0;
			}
			//表示
			tky_select_dsp( &CREC, dc_sw, &tkrec, FALSE );
			//データセットのためのダミーフォーカスＯＦＦ

			_getLINE_DATA( ln )->drec.m_tekiyo = CREC.m_tekiyo;
			TerminationDataSet(AC_TKY_PN);
		//	TerminationTekiyo( this, ln, 0, 0, 0 );

			return TRUE;

//#endif
		}
	}
	return FALSE;
}



BEGIN_EVENTSINK_MAP(CDBAbookView, CDBbaseView)
    //{{AFX_EVENTSINK_MAP(CDBAbookView)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl, VTS_I4)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 1 /* Termination */, OnTerminationIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 4 /* KanaTermination */, OnKanaTerminationIcsdbedtctrl, VTS_I4 VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CDBAbookView, IDC_ICSSELCTRL1, 1, CDBAbookView::SelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CDBAbookView, IDC_ICSSELCTRL2, 1, CDBAbookView::SelectedIcsselctrl2, VTS_I2 VTS_I4)
END_EVENTSINK_MAP()


// DBEDIT セットフォーカス	のまとまり
//
void CDBAbookView::OnSetFocusIcsdbedtctrl(UINT ID)
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1: 
		OnSetFocusIcsdbedtctrl1();	break;
	case IDC_ICSDBEDTCTRL2:
		OnSetFocusBaseBumon();		break;
	case IDC_ICSDBEDTCTRL3:
		OnSetFocusBaseKoji();		break;
	case IDC_ICSDBEDTCTRL4:
		OnSetFocusBaseKamok();		break;
	case IDC_ICSDBEDTCTRL5:
		OnSetFocusBaseBrn();		break;
	}
}

// DBEDIT KILLフォーカス	のまとまり
//
void CDBAbookView::OnKillFocusIcsdbedtctrl(UINT ID, long inplen) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1: 
		OnKillFocusIcsdbedtctrl1( inplen );	break;
	case IDC_ICSDBEDTCTRL2:
		OnKillFocusBaseBumon( inplen );		break;
	case IDC_ICSDBEDTCTRL3:
		OnKillFocusBaseKoji( inplen );		break;
	case IDC_ICSDBEDTCTRL4:
		OnKillFocusBaseKamok( inplen );		break;
	case IDC_ICSDBEDTCTRL5:
		OnKillFocusBaseBrn( inplen );		break;
	}
}

// DBEDIT ターミネーション	のまとまり
//
void CDBAbookView::OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
		OnTerminationIcsdbedtctrl1( nChar, inplen, kst );	break;
	case IDC_ICSDBEDTCTRL2:
		OnTerminationBaseBumon( nChar, inplen, kst );		break;
	case IDC_ICSDBEDTCTRL3:
		OnTerminationBaseKoji( nChar, inplen, kst );		break;
	case IDC_ICSDBEDTCTRL4:
		OnTerminationBaseKamok( nChar, inplen, kst );		break;
	case IDC_ICSDBEDTCTRL5:
		OnTerminationBaseBrn( nChar, inplen, kst );			break;
	}
}

// DBEDIT IME End	のまとまり
//
void CDBAbookView::OnImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
		OnImeEndCompositionIcsdbedtctrl1( nChar, string, ystring );	break;
	case IDC_ICSDBEDTCTRL4:
		OnImeEndCompositionBaseKamok( nChar, string, ystring );		break;
	}
}

// DBEDIT カナターミネーション	のまとまり
void CDBAbookView::OnKanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
	case IDC_ICSDBEDTCTRL4:
		KanaTermKamoku( Kana );	break;
//	case IDC_ICSDBEDTCTRL4:
//		OnImeEndCompositionBaseKamok( Kana );		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////
//	基本項目 部門 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseBumon()
{
	set_nowpn( AC_SELBMON_PN );

	SelPar par;
	par.sel_pn = SL_BUMON_PN;
	par.sel_flg = 0;

	m_Seldata.SelectJob(&par, -1);
}

//	基本項目 部門 キルフォーカス
void CDBAbookView::OnKillFocusBaseBumon(long inplen)
{
	OnTerminationBaseBumon( 0, inplen, 0);
}

//	基本項目 部門 ターミネーション
void CDBAbookView::OnTerminationBaseBumon(long nChar, long inplen, long kst)
{
	char bf[64];
	int bmon, prev, len, number, n;
	CDBipDataRec	rec;
	VARIANT var;

	// 直前のデータを保存
	prev = m_BaseBmn;

	DBdata_get( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( var.pbVal != NULL )
		strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
	len = 0;

	len = strlen( (const char*)bf );
	bmon = -1;

	if( len > 0 ) {
		bmon = atoi( (LPCTSTR)bf );
	}

#ifdef _2KT_BSEL	// 2桁で選択画面より番号選択
	if( inplen != 0 && (len  > 0 && len <= 2) ) {
		// 選択画面より番号選択
		number = atoi( bf );
		if( bmncode_todata( &rec, number, 0 ) != FALSE ) {
			bmon = rec.m_dbmn;
		}
		else {
			bmon = m_BaseBmn;
			Buzzer();
		}
	}
	else if( inplen != 0 && len > 2 ) {
#else
	else {
#endif

		// 部門を手入力
		n = BMON_LNG-len;
			
		if( n >= 0 ) {
			// 部門最大桁数まで入力されていない
			bmon = atoi( bf );
		}
		else {
			bmon = -1;	//桁数オーバー
		}
	}

	pDBzm->BumonCodeToStr( bf, sizeof bf, bmon );
	var.pbVal = (BYTE*)bf;
	DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
	if( inplen ) {
		check_basebrn();
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_BUMON_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		if( M_KOJI )	set_focus( AC_SELKOJI_PN );
		else			set_focus( AC_SELKAMOK_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_focus( AC_SELDISP_PN );
	}
}

//////////////////////////////////////////////////////////////////////////////////
//	基本項目 工事番号 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseKoji()
{
	set_nowpn( AC_SELKOJI_PN );

	SelPar par;
	par.sel_pn = SL_KOJI_PN;
	par.sel_flg = 0;

	m_Seldata.SelectJob(&par, -1);
}


//	基本項目 工事番号 キルフォーカス
void CDBAbookView::OnKillFocusBaseKoji(long inplen)
{
	OnTerminationBaseKoji( 0, inplen, 0 );
}

//	基本項目 工事番号 ターミネーション
void CDBAbookView::OnTerminationBaseKoji(long nChar, long inplen, long kst)
{
	char bf[64];
	CString koji, prev;
	int len, number, n;
	CDBipDataRec	rec;
	VARIANT var;

	// 直前のデータを保存
	prev = m_BaseKoji;

	DBdata_get( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( var.pbVal != NULL )
		strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
	len = 0;

	len = strlen( (const char*)bf );
	koji.Empty();

	if( len > 0 ) {
		koji = bf;
	}

#ifdef _2KT_BSEL	// 2桁で選択画面より番号選択
	if( inplen != 0 && (len  > 0 && len <= 2) && isdigitstr( bf ) ) {
		// 選択画面より番号選択
		number = atoi( bf );
		if( kojicode_todata( &rec, number, 0 ) != FALSE ) {
			koji = rec.m_dkno;
		}
		else {
			koji = m_BaseKoji;
			Buzzer();
		}
	}
	else if( inplen != 0 && len > 2 ) {
#else
	else {
#endif

		// 工事を手入力
		n = Voln1->kjcol - len;
			
		if( n >= 0 ) {
			// 工事最大桁数まで入力されていない
			koji = bf;
		}
		else {
			koji.Empty();	//桁数オーバー
		}
	}

	pDBzm->KojiCodeToStr( bf, sizeof bf, koji );
	var.pbVal = (BYTE*)bf;
	DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_KOJI_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		set_focus( AC_SELKAMOK_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		if( BMON_MST )	set_focus( AC_SELBMON_PN );
		else			set_focus( AC_SELDISP_PN );
	}

}






//////////////////////////////////////////////////////////////////////////////////
//	基本項目 科目 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseKamok()
{
	set_nowpn( AC_SELKAMOK_PN );

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob(&par, -1);
}

//	基本項目 科目 キルフォーカス
void CDBAbookView::OnKillFocusBaseKamok(long inplen)
{
}

//	基本項目 科目 ターミネーション
void CDBAbookView::OnTerminationBaseKamok(long nChar, long inplen, long kst)
{
	if( ! PROGRAM_OK )	return;

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_KAMOKU_PN:
		case SL_KMKANA_PN:
		case SL_BRNTKY_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		set_focus( AC_SELBRN_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		if( M_KOJI )		set_focus( AC_SELKOJI_PN );
		else if( BMON_MST )	set_focus( AC_SELBMON_PN );
		else {
			set_focus( AC_SELDISP_PN );
		}
	}
}

//	基本項目 科目 IME End
void CDBAbookView::OnImeEndCompositionBaseKamok(short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ! PROGRAM_OK )	return;

	if( CheckDataBaseKamoku( nChar, string ) <= 0 )
		return;
	OnTerminationBaseKamok( nChar, 1, 0 );
}

// 基本科目チェックデータ
long CDBAbookView::CheckDataBaseKamoku( long nChar, LPCTSTR string ) 
{
	CString kmkstr, ldata_code;
	int edaban = -1, ldata_brn = -1;
	struct _KamokuData check = {0}, ans = {0};
	DBKNREC* pKn;

	KamokuStr_Chk( string, &check );

	if( m_Seldata.InputItemCheck( ans, check, 0 ) == TRUE )
	{
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
		m_BaseCode = ans.kd_code;

		check_basebrn();
	}
	else {
		Buzzer();

		::ZeroMemory( (char *)&ans, sizeof(ans), 0 );
		memcpy( ans.kd_code, m_BaseCode, KMKCD_LN );
		ans.kd_eda = -1;
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
		return 0;
	}

	return nChar;
}

//////////////////////////////////////////////////////////////////////////////////
//	基本項目 枝番 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseBrn()
{
	set_nowpn( AC_SELBRN_PN );

	SelPar par;
	par.sel_pn	= SL_BRNTKY_PN;
	par.tkybrn	= m_BaseCode;
	par.brn_bmn	= m_BaseBmn;
	par.brn_cd	= -1;
	::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
	
	m_Seldata.SelectJob(&par, -1);
}


//	基本項目 枝番 キルフォーカス
void CDBAbookView::OnKillFocusBaseBrn(long inplen)
{
	OnTerminationBaseBrn( 0, inplen, 0 );
}

//	基本項目 枝番 ターミネーション
void CDBAbookView::OnTerminationBaseBrn(long nChar, long inplen, long kst)
{
	int n, num;
	VARIANT var;
	BOOL	bSetNew = FALSE;
	char str[128] = {0};
	n = num = 0;

	if( inplen ) {
		DBdata_get( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal != NULL )	
			n = strlen( (const char*)var.pbVal );
	}

	if( n && n < 3 )
	{
		num = atoi( (const char*)var.pbVal );

		if( num > 8 ) {
			SelResult srt;
			sprintf_s( str, sizeof str, "%d", num );

			srt.sel_pn = SL_BRNTKY_PN;
			if( m_Seldata.GetSelResult( &srt, str ) ) {

				m_BaseEda = srt.brn_no;
				//コントロールへ再セット
				pDBzm->EdabanToStr( str, sizeof str,m_BaseEda );
				var.pbVal = (BYTE*)str;
				DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
				bSetNew = TRUE;
			}
		}
		// 元の枝番をセット
		if( ! bSetNew ) {
			pDBzm->EdabanToStr( str, sizeof str,m_BaseEda );
			var.pbVal = (BYTE*)str;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else if( n != 0 ) {
		// 枝番手入力
		num = atoi( (const char*)var.pbVal );
		m_BaseEda = num;
		pDBzm->EdabanToStr( str, sizeof str,m_BaseEda );
		var.pbVal = (BYTE*)str;
		DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	// 枝番名称を表示
	if( inplen ) {
		struct _SET_KMKETC sk;
		sk.bmn = m_BaseBmn;
		sk.brn = m_BaseEda;
		sk.code = m_BaseCode;

		if( _set_kmketc( &sk, FALSE ) ) {
			GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
		}
		else {
			GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
		}
	}

	if( nChar ) {
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_BRNTKY_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		set_focus( AC_SELDISP_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_focus( AC_SELKAMOK_PN );
	}
}


// リストの中身の DBEDIT セットフォーカス
void CDBAbookView::OnSetFocusIcsdbedtctrl1()
{
	if( PROGRAM_OK == FALSE )
		return;

	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

TRACE( "++Abook++ **SetFocus\n" );	
	DT_EDIT edit;

	edit.hdr.hwndFrom = m_Input.m_hWnd;
	edit.hdr.idFrom   = IDC_ICSDBEDTCTRL1;
	edit.hdr.code     = DLUM_EDITFOCUS;

	m_List.SendMessage(WM_DTLISTUSER, edit.hdr.code, (LPARAM)&edit );

}

// リストの中身の DBEDIT キルフォーカス
void CDBAbookView::OnKillFocusIcsdbedtctrl1(long inplen) 
{
TRACE( "++Abook++ **killFocus , pok %d\n", PROGRAM_OK );	
	if( PROGRAM_OK == FALSE )
		return;

	_OnTerminationIcsdbedtctrl( 0, inplen, 0 );
	
}

// リストの中身の DBEDIT ターミネーション
void CDBAbookView::OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst) 
{
	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

	_OnTerminationIcsdbedtctrl( nChar, inplen, kst );	
}

// リストの中身の DBEDIT IME END
void CDBAbookView::OnImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

	if( PROGRAM_OK == FALSE )
		return;
	int st, term, pos, ln;
	VARIANT var;
	char tmp[128];

	ln = get_nowln();
	if( ln > 0 ) {
		// 元帳クラス自動作成仕訳
		if( m_ALine[ln-1].dtype == 1 )
			return;
	}

	switch( get_nowpn() ) {
	case AC_KMK_PN:
		if( CheckDataKamoku( nChar, string ) <= 0 )
			return;
		TerminationKamoku( get_nowln(), nChar, 1, 0 );
		break;

	case AC_TKY_PN:
		term = 1;
		if( (st = TekiyoStr_Chk( string )) > 0 ) {
			if( GetTekiMode() ) {	// 摘要コード＋Enter で 摘要をセット
				if(	st == 1 ) {
					if( nChar == VK_RETURN ) {
						_KanaTermIcsTekiyo( ystring, FALSE );
						term = 0;
					}
				}
				if( nChar == VK_TAB ) {
					if( ! isdigitstr( (char*)ystring ) )
						_KanaTermIcsTekiyo( ystring, FALSE );
					else
						_KanaTermIcsTekiyo( ystring, TRUE );	
					term = 0;
				}
				else if( nChar == VK_F12 ) {
					_KanaTermIcsTekiyo( ystring, FALSE );
					term = 0;
				}
			}
			else {
				if( nChar == VK_TAB || nChar == VK_F12 ) {
					_KanaTermIcsTekiyo( ystring, FALSE );
					term = 0;
				}
			}
		}
		if( term ) {
			strcpy_s( tmp, sizeof tmp, string );
			var.pbVal = (BYTE*)tmp;
			pos = m_Input.GetCaretPosition();
			m_Input.InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		//	TerminationIcsinputTky( this, get_nowln(), nChar );
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}	
}


// リストの中身の カナターミネーション
void CDBAbookView::OnKanaTerminationIcsdbedtctrl1(LPCTSTR Kana) 
{
	// TODO: OnKanaTermination この位置にコントロール通知ハンドラ用のコードを追加してください
	
}


void CDBAbookView::_OnTerminationIcsdbedtctrl( long nChar, long inplen, long kst )
{
	if( PROGRAM_OK == FALSE )
		return;

//_DEBUG_FILEOUT( "**_OnTerminationIcsdbedtctrl1 nChar %04x, inplen %d, ln %d, pn %d\n", nChar, inplen, get_nowln(), get_nowpn() );
TRACE( "++Abook++ **_OnTerminationIcsdbedtctrl1 nChar %04x, inplen %d, ln %d, pn %d\n", nChar, inplen, get_nowln(), get_nowpn() );
	int ln = get_nowln();
	if( ln == -1 )	return;

	int pn = get_nowpn();
	switch( pn )
	{
	case AC_SEQ_PN:
//		TerminationIcsinputSeq( nChar, inplen, kst ); 
		break;
	case AC_DATE_PN:
		if( inplen ) {
			if( CheckDataIcsDate( nChar ) < 0 )
				return;
		}
		TerminationIcsDate( nChar, inplen, kst ); 
		break;
	case AC_BMON_PN:
//		TerminationBumon( get_nowln(), nChar, inplen, kst );
		break;
	case AC_KOJI_PN:
//		TerminationKoji( get_nowln(), nChar, inplen, kst );
		break;

	case AC_KMK_PN:
		if( nChar != VK_DELETE )
			TerminationKamoku( get_nowln(), nChar, inplen, kst );
		break;
	case AC_NKIN_PN:
		TerminationVal( get_nowln(), 0, nChar, inplen, kst );
		break;

	case AC_SKIN_PN:
		TerminationVal( get_nowln(), 1, nChar, inplen, kst );
		break;
	
	case AC_ZEI_PN:
		TerminationZei( get_nowln(), nChar, inplen, kst );
		break;
	case AC_SYZ_PN:
		TerminationSyz( get_nowln(), nChar, inplen, kst );
		break;
	case AC_TKY_PN:
		TerminationTekiyo( get_nowln(), nChar, inplen, kst );
		break;

/*	case SYON_PN:
		TerminationIcsinputSyono( get_nowln(), nChar, inplen, kst );
		break;
	case TGDT_PN:
		if( inplen ) {
			if( CheckDataIcsinputTgDate( nChar ) <= 0 )
				return;
		}
		TerminationIcsinputTgDate( get_nowln(), nChar, inplen, kst );
		break;
*/	default:
//		ASSERT( FALSE );
		break;
	}

	if( nChar == 0 ) {
//TRACE( "m_Input.ShowWindow(SW_HIDE)\n" );
//		m_Input.ShowWindow(SW_HIDE);
//		m_Input.PostMessage( WM_SHOWWINDOW, FALSE, 0 );
	}
	DT_EDIT edit;

	edit.hdr.hwndFrom = m_Input.GetSafeHwnd();
	edit.hdr.idFrom   = IDC_ICSDBEDTCTRL1;
	edit.hdr.code     = DLN_ENDLABELEDIT;

	edit.nChar = (short)nChar;

	m_List.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );

}


// 
void CDBAbookView::to_acbkmode()
{
	static int init_sgn = 0;

	if( ! init_sgn ) {
		int date = pDBzm->zvol->ip_ymd;
		date %= 10000;

		VDPAC pac;
		// 年度は 平成年
		if( db_vd_dpac( 1, date, &pac, pDBzm ) == 0 ) {
			m_BaseDate = pac.Dymd;
			m_BaseOfst = pac.Doffset;
			m_BaseKsign = (pac.Dsgn & 0x80) ? 1 : 0;
		}
		else {
			ICSExit( 0, "期間を取得できません" );
			return;
		}

		init_sgn = 1;
		PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_F11, 0) );
	}
}


// 借／貸 変更
void CDBAbookView::OnButtonF9()
{
	int ret = ChangeDebtCred();
	
	int pn = get_nowpn();
	if( pn == AC_NKIN_PN || pn == AC_SKIN_PN ) {
		if( ret == 0 && pn == AC_SKIN_PN )
			set_focus( AC_NKIN_PN );
		else if( ret == 1 && pn == AC_NKIN_PN )
			set_focus( AC_SKIN_PN );
	}
}


// 借／貸 変更
int CDBAbookView::ChangeDebtCred()
{
	ACBOOK_LINE* al = getLINE_DATA();
	int ret, nTmp;

	CString tmp;
	tmp = al->drec.m_dbt;
	// 科目コード
	al->drec.m_dbt = al->drec.m_cre;
	al->drec.m_cre = tmp;
	// 枝番
	nTmp = al->drec.m_dbr;
	al->drec.m_dbr = al->drec.m_cbr;
	al->drec.m_cbr = nTmp;
	// 部門
	nTmp = al->drec.m_dbmn;
	al->drec.m_dbmn = al->drec.m_cbmn;
	al->drec.m_cbmn = nTmp;
	// 工事番号
	tmp = al->drec.m_dkno;
	al->drec.m_dkno = al->drec.m_ckno;
	al->drec.m_ckno = tmp;

	CREC.m_dbt = al->drec.m_dbt;
	CREC.m_dbr = al->drec.m_dbr;
	CREC.m_dbmn = al->drec.m_dbmn;
	CREC.m_dkno = al->drec.m_dkno;

	CREC.m_cre = al->drec.m_cre;
	CREC.m_cbr = al->drec.m_cbr;
	CREC.m_cbmn = al->drec.m_cbmn;
	CREC.m_ckno = al->drec.m_ckno;

	// 差引残高
	if( al->drec.m_dbt == m_BaseCode ) {	
		l_sub( al->szan, al->szan, al->drec.m_val );	// 入金
		l_add( al->szan, al->szan, al->drec.m_val );
	}
	else {
		l_add( al->szan, al->szan, al->drec.m_val );	// 出金	
		l_sub( al->szan, al->szan, al->drec.m_val );
	}

	// リスト再表示
	int pn[] = { AC_NKIN_PN, AC_SKIN_PN };

	if( m_BaseCode == al->drec.m_dbt ) {
		al->lcnd[AC_NKIN_PN].INP_sg = TRUE;
		al->lcnd[AC_SKIN_PN].INP_sg = FALSE;

		ret = 0;	// 入金
	}
	else {
		al->lcnd[AC_NKIN_PN].INP_sg = FALSE;
		al->lcnd[AC_SKIN_PN].INP_sg = TRUE;	

		ret = 1;	// 出金
	}

	TerminationDataSet( pn, 2 );

	return ret;
}


// 先頭仕訳 に
void CDBAbookView::OnButtonF8()
{
	// TODO: 先頭仕訳 ここにコマンド ハンドラ コードを追加します。
	if( ! PageMoveDataUpdate( get_nowln() ) ) {
		return;
	}
	// _dline_chg で DataCorrect()しないように
	set_nowln(-1);

	if( TopDataSet() > 0 ) {
		ListDataSet();
		_set_focus( 1, AC_DATE_PN );
	}
}


// 登録処理
void CDBAbookView::OnButtonF7()
{
	// TODO: 追加 ここにコマンド ハンドラ コードを追加します。
}




//--------------------------------------------------------
// 仕訳挿入
//	返送値	-1 = エラー,
//			 1 = 仕訳挿入モードに
//			 0 = なにもしない
//	エラー時 例外をthrow
//--------------------------------------------------------
int CDBAbookView::InsertSiwake( int ln )
{
	// 現在 仕訳挿入モード
	if( ! PROGRAM_OK )	return 0;

	if( ln < 1 )	return 0;

	if( m_InsLine > 0 )
		return 0;

	if( acINP_mode == _APPEND )
		return 0;

	int pos = m_ALine[ln-1].abs_pos;
	if( pos <= 0 )
		return 0;

	CDBipDataRec*	prec;

	if( pos > 1 ) {
		pos--;
		prec = &m_ALine[ln-1].drec;

		pDBzm->ledger->SetAbsolutePosition( pos );
		if( pDBzm->ledger->st == -1 ) {
			ermset( -1, "データポジションを取得できません！" );
			return -1;
		}

		pDBzm->dbdata->SetAbsolutePosition( pDBzm->ledger->seq );
		if( pDBzm->dbdata->st == -1 ) {
			ermset( -1, "データポジションを取得できません！" );
			return -1;
		}
		// 挿入モードへ
		if( prec->m_ddate != pDBzm->dbdata->ddate ||
			(prec->m_ddate == pDBzm->dbdata->ddate && prec->m_mofs != pDBzm->dbdata->mofs) ) {
			m_InsLine = ln;
		}
	}
	else {
		// 挿入モードへ
		m_InsLine = ln;
	}

	// 表示を更新
	if( m_InsLine != 0 ) {
		PROGRAM_OK = FALSE;
		m_List.SetFocus();
		int n = (ln-1);
		// データを一つずつずらす
		for( int i = ACREC_MAX-1; i >= n; i-- ) {
			m_ALine[i] = m_ALine[i-1];
			// 現在の追加行は空行にする
			if( m_ALine[i].abs_pos == 0 ) {
				acline_init( &m_ALine[i], 1 );
			}
		}
		// 新規行をセット
		acline_init( &m_ALine[n], 1 );
		m_ALine[n].abs_pos = 0;
		m_ALine[n].drec.m_seq = (m_LASTSEQ+1);
		m_ALine[n].drec.m_ddate	= -1;
		m_ALine[n].drec.m_mofs	= -1;
		m_ALine[n].drec.m_ksign	= -1;

		ListDataSet();
		PROGRAM_OK = TRUE;
		set_nowln(-1);	// _dline_chg で 現在レコードをセットするため
		_set_focus( ln, AC_DATE_PN );
	}

	return ( m_InsLine > 0 ) ? 1 : 0;
}


//--------------------------------------------------------
// 仕訳挿入 から 最終行に入力ラインがあるモードへ戻す
//	返送値	-1 = エラー,
//			 1 = 仕訳挿入モードに
//			 0 = なにもしない
//
//--------------------------------------------------------
int CDBAbookView::BackNormalAppendMode( int ln )
{
	// 現在 仕訳挿入モード
	if( ! PROGRAM_OK )	return 0;

	if( m_InsLine == 0 )
		return 0;

ASSERT( m_InsLine == ln );

	int i, lcnt, pos, apnd_idx;
	lcnt = m_List.GetDispDataCount();

	// 次のページの最初の仕訳を読み込む
	if( ln == lcnt ) {
		pos = m_ALine[ln-2].abs_pos;
		pos++;
	}
	else {
		pos = m_ALine[lcnt-1].abs_pos;
	}

	if( pos > 1 ) {
		pDBzm->ledger->SetAbsolutePosition( pos );
		if( pDBzm->ledger->st == -1 ) {
			ermset( -1, "データポジションを取得できません！" );
			return -1;
		}
	}

	apnd_idx = -1;
	for( i = (ln-1); i < lcnt; i++ ) {

		if( i < (lcnt-1) ) {
			m_ALine[i] = m_ALine[i+1];
		}
		else {
			acline_init( &m_ALine[i], 1 );

			if( pos > 0 ) {
				acline_set( m_ALine, i, pos );
			}
		}
		// 追加データセット用インデックス
		if( m_ALine[i].abs_pos < 0 ) {
			if( apnd_idx == -1 )	apnd_idx = i;
		}
	}

	m_InsLine = 0;

	if( apnd_idx != -1 ) {
		// 新規行をセット
		acline_init( &m_ALine[apnd_idx], 1 );
		m_ALine[apnd_idx].abs_pos = 0;
		m_ALine[apnd_idx].drec.m_seq = (m_LASTSEQ+1);

		if( apnd_idx > 0 ) {
			m_ALine[apnd_idx].drec.m_ddate	= m_ALine[apnd_idx-1].drec.m_ddate;
			m_ALine[apnd_idx].drec.m_mofs	= m_ALine[apnd_idx-1].drec.m_mofs;
			m_ALine[apnd_idx].drec.m_ksign	= m_ALine[apnd_idx-1].drec.m_ksign;
		}
		else {
			set_inpdate( &m_ALine[apnd_idx].drec );
		}
	}

	ListDataSet();
	set_nowln(-1);	// _dline_chg で 現在レコードをセットするため
	_set_focus( ln, AC_DATE_PN );

	return 0;
}



// F12 自動仕訳
void CDBAbookView::OnButtonF12()
{
	AutoSiwake( get_nowln(), m_bAutoSwk ? TRUE : FALSE );
}

//--------------------------------------------------------
// 自動仕訳
//	int ln ...... 自動仕訳行
//	BOOL bAuto .. 自動仕訳モードへ(TRUE)
//
//--------------------------------------------------------
void CDBAbookView::AutoSiwake( int ln, BOOL bAuto )
{
	if( ! PROGRAM_OK )
		return;

	if( acINP_mode != _APPEND )
		return;

	// 行の背景色を変更
	DTLIST_DATA data;
/*
	m_List.GetDtListData( ln-1, &data );
	for( int i = 0; i < sizeof(data.dd_ctbl) / sizeof (data.dd_ctbl[0]); i++ ) {
		data.dd_ctbl[i].back = RGB_WINE_GREEN;
	}
	m_List.SetDtListData( ln-1, &data, TRUE );
*/
	m_List.SetCurBkColor( RGB_WINE_GREEN );

	m_bAutoSwk = TRUE;

	// 摘要欄にフォーカスセット
	_set_focus( ln, AC_TKY_PN );
}


// 固定選択画面クリック
void CDBAbookView::SelectedIcsselctrl1(short position, long Number)
{
	SelectedIcsselctrl( position, Number, 0 );
}

// 選択画面クリック
void CDBAbookView::SelectedIcsselctrl2(short position, long Number)
{
	SelectedIcsselctrl( position, Number, 1 );
}


/*=======================
	FIX 8科目選択
	科目・摘要32項目選択
		本体
========================*/
void CDBAbookView::SelectedIcsselctrl(short position, long Number, int place ) 
{
	int pn, tmp;
	CDBipTKREC tkrec;
	CDBipDataRec tmpdt;	// 部門コード取得用
	struct _ItemData dmy_data = {0};
	int dcsw;
	struct	_KamokuData check={0}, ans={0};
	char temp[10];
	char str[128];
	DBKNREC* pKn;

	pn = get_nowpn();

	// 基本項目エリアでの選択
	if( base_selected( pn, Number ) ) {
		return;
	}

	if( acINP_mode == _APPEND ) {
		if( ! getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg && 
			! getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
			dcsw = -1;
		}
		else {
			if( getLINE_DATA()->drec.m_dbt == m_BaseCode ) {
				dcsw = 1;
			}
			else {
				dcsw = 0;
			}
		}
	}
	else {
		if( getLINE_DATA()->drec.m_dbt == m_BaseCode ) {
			dcsw = 1;
		}
		else {
			dcsw = 0;
		}
	}

	if( pn == AC_KMK_PN )
	{
	//	check.IP_CHKCdNum = (int)Number;
	//	check.IP_CHKCdNumSg = 1;
		// 科目ｺｰﾄﾞ or 番号 02.20/02
		if( bKamokuCode ) {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );
			check.kd_sgn = 0;
		}
		else {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );	// 番号
			check.kd_sgn = 1;
		}
		check.kd_eda = -1;

		if( m_Seldata.InputItemCheck( ans, check, dcsw, TRUE ) )
		{
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
			//データセット
			getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;
			//次に移動
			//CREC などには、Termination 0 で データセット
			set_termfocus( get_nowln(), AC_KMK_PN, 1 );
		}
	}
	else if( BRNTKY_sel && pn == AC_KMK_PN )
	{
		if( Number > 8 )
		{
			SelResult srt;
			sprintf_s( str, sizeof str, "%d", Number );

			srt.sel_pn = SL_BRNTKY_PN;
			if( m_Seldata.GetSelResult( &srt, str ) ) {

				strcpy_s( ans.kd_code, sizeof ans.kd_code, srt.km_code );
				ans.kd_eda = srt.brn_no;
				if( (pKn = pDBzm->DB_PjisToKnrec( ans.kd_code ) ) ) {
					sprintf_s( ans.kd_name, sizeof ans.kd_name, "%.14s", pKn->knnam );
				}
				//データセット
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
				getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;

				//次に移動
				//CREC などには、Termination 0 で データセット
				set_termfocus( get_nowln(), AC_KMK_PN, 1 );

			}
		}
	}
	else if( pn == AC_TKY_PN && place )
	{
		//摘要選択
		sprintf_s( temp, sizeof temp, "%d", Number );
		if( tky_select( &tkrec, (char *)temp ) != -1 )
		{
			//表示
			tky_select_dsp( &CREC,  0, &tkrec,FALSE );
		}
	}
	else if( pn == AC_BMON_PN || pn == AC_KOJI_PN ) {
		// 部門・工事選択処理
		switch( pn ) {
		case AC_BMON_PN:
			break;
		case AC_KOJI_PN:
			break;
		}
//		int bmn;
//		if( bmncode_todata( &bmn, Number, 0 ) != FALSE ) {
//			pKey->IP_BMN = bmn;
//			bmon_select_dsp( pn, pKey, TRUE );
//		}
	}
	else if( pn == AC_SYZ_PN ) {
		sprintf_s( str, sizeof str, "%d", Number );
		tmp = atoi( str );

		m_pSyzInp->SetSkbnSelNo( tmp );
		return;
	}

	set_focus( pn );

}


//--------------------------------------------------------------
//	基本項目の選択欄クリックによる選択
//
//--------------------------------------------------------------
BOOL CDBAbookView::base_selected( int pn, int Number )
{
	struct	_KamokuData check={0}, ans={0};
	BOOL bRet = TRUE;
	SelResult	srt;
	char	tmp[128] = {0};
	VARIANT var;

	switch( pn ) {
	case AC_SELBMON_PN:
		srt.sel_pn = SL_BUMON_PN;
		sprintf_s( tmp, sizeof tmp, "%d", Number );

		if( m_Seldata.GetSelResult( &srt, tmp ) ) {
			m_BaseBmn = srt.bmn;
			//コントロールへ再セット
			pDBzm->BumonCodeToStr( tmp, sizeof tmp, m_BaseBmn );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
			check_basebrn();
		}
		break;

	case AC_SELKOJI_PN:
		srt.sel_pn = SL_KOJI_PN;
		sprintf_s( tmp, sizeof tmp, "%d", Number );

		if( m_Seldata.GetSelResult( &srt, tmp ) ) {
			m_BaseKoji = srt.kjcode;
			//コントロールへ再セット
			pDBzm->KojiCodeToStr( tmp, sizeof tmp, m_BaseKoji );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		break;

	case AC_SELKAMOK_PN:
		// 科目ｺｰﾄﾞ or 番号
		if( bKamokuCode ) {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );
			check.kd_sgn = 0;
		}
		else {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );	// 番号
			check.kd_sgn = 1;
		}
		check.kd_eda = -1;

		if( m_Seldata.InputItemCheck( ans, check, 0, TRUE ) )
		{
			m_BaseCode = ans.kd_code;
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
			check_basebrn();
		}
		else {
			::ZeroMemory( &ans, sizeof ans );
			memcpy( ans.kd_code, m_BaseCode, KMKCD_LN );
			ans.kd_eda = -1;

			DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
		}
		break;

	case AC_SELBRN_PN:
		if( ! BRNTKY_sel )
			break;

		srt.sel_pn = SL_BRNTKY_PN;
		sprintf_s( tmp, sizeof tmp, "%d", Number );

		if( m_Seldata.GetSelResult( &srt, tmp ) ) {
			m_BaseEda = srt.brn_no;
			//コントロールへ再セット
			pDBzm->EdabanToStr( tmp, sizeof tmp, m_BaseEda );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn = m_BaseBmn;
			sk.brn = m_BaseEda;
			sk.code = m_BaseCode;

			if( _set_kmketc( &sk, FALSE ) ) {
				GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
			}
			else {
				GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
			}
		}
		break;

	case AC_SELDISP_PN:
		break;
	default:
		bRet = FALSE;
		break;
	}

	return bRet;
}

//
//	科目・部門変わりの枝番チェック
//		return FALSE	該当枝番がない
//
BOOL CDBAbookView::check_basebrn()
{
	CDBipDataRec tmp;

	if( m_BaseEda == -1 )
		return FALSE;

	tmp.m_dbmn	= m_BaseBmn;
	tmp.m_dbt	= m_BaseCode;
	tmp.m_dbr	= m_BaseEda;

	if( pDBzm->DB_EdabanMasterSearch( &tmp, 0 ) != 0 ) {
		m_BaseEda = -1;

		VARIANT var;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
		return FALSE;
	}

	// 枝番名称表示
	struct _SET_KMKETC sk;
	sk.bmn = m_BaseBmn;
	sk.code = m_BaseCode;
	sk.brn = m_BaseEda;

	if( _set_kmketc( &sk, FALSE ) ) {
		GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
	}
	else {
		GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
	}

	return TRUE;
}


BOOL CDBAbookView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: pretranslate ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	set_nonICSpn( pMsg );

	if( pMsg->message == WM_KEYDOWN ) {
		if( GetDlgItem( IDC_BUTTON_DISP )->m_hWnd == pMsg->hwnd ){
			if( ! OnTermBaseDispBtn(pMsg->wParam) )
				return TRUE;
		}
	}
	if( pMsg->message == WM_CHAR ) {
		if( PROGRAM_OK ) {
			int ln = get_nowln();
			if( ln != -1 && get_nowpn() != -1 ) {
				//元帳クラス自動生成仕訳
				if( m_ALine[ln-1].dtype == 1 ) {
					return TRUE;
				}
			}
		}
	}

	return CDBbaseView::PreTranslateMessage(pMsg);
}


int CDBAbookView::set_nonICSpn( tagMSG* pMsg )
{
	CWnd *pwnd;

	if( pwnd = GetFocus() )
	{
		if( get_nowpn() != AC_SELDISP_PN && pwnd->m_hWnd == GetDlgItem( IDC_BUTTON_DISP )->m_hWnd )
		{
			set_nowpn( AC_SELDISP_PN );
			return 1;
		}
	}
	return 0;
}


long	CDBAbookView::OnTermBaseDispBtn( long nChar )
{

	if( ! PROGRAM_OK ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_UP:
	case VK_LEFT:
	case VK_F2:
		set_focus( AC_SELBRN_PN );
		nChar = 0;
		break;

	case VK_TAB:
		if( BMON_MST )		set_focus( AC_SELBMON_PN );
		else if( M_KOJI )	set_focus( AC_SELKOJI_PN );
		else				set_focus( AC_SELKAMOK_PN );
		nChar = 0;
		break;
	}

	return nChar;

}


//----------------------------------------------------------------------------
//	元帳クラス クエリー
//
//----------------------------------------------------------------------------
int CDBAbookView::Abook_Requery()
{
	CString filter, tmp;
	// 取消は除く
	filter.Format( "(cast(cast(dsign as binary(1)) as int) & 0x01) = 0 and kcd1 = '%s'", m_BaseCode );

	if( m_BaseBmn != -1 ) {
		tmp.Format( "bmn1 = %d", m_BaseBmn );
		filter += tmp;
	}
	if( m_BaseEda != -1 ) {
		tmp.Format( "br1 = %d", m_BaseEda );
		filter += tmp;
	}
	if( M_KOJI && ! m_BaseKoji.IsEmpty() ) {
		tmp.Format( "kno1 = %s", m_BaseKoji );
		filter += tmp;
	}

	int type;

	if( m_BaseBmn == -1 ) {
		if( m_BaseEda == -1 )	type = 0;
		else					type = 0x1;
	}
	else {
		if( m_BaseEda == -1 )	type = 0x2;
		else					type = 0x3;	
	}

	return pDBzm->ledger->Requery( type, filter, 0 );

}



//pDBzm->DB_DataCorrect( &CREC );