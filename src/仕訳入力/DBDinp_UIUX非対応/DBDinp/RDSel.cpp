// RDSel.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"

#include "Input1.h"
#include "external.h"

#include "MstRw.h"

#include "MainFrm.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"
#include "DBAbookView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDBSyohi* pDBsy;
extern CMKRDSub	Mkrd;
extern struct _AUTOSEL *pAUTOSEL;
extern CDBDinpView *p_DBDinpView;

/////////////////////////////////////////////////////////////////////////////
// CRDSel ダイアログ

// バックカラー
#define SELECT_BKCOL		RGB_GREEN2			// 選択背景色
#define NORMAL_BKCOL		RGB_PAPER_WHITE		// 通常背景色

// アクションモード
#define ACT_FOCUS_ON			1
#define ACT_FOCUS_OFF			2
#define ACT_KEYIN_ONLY			3
#define ACT_KEYIN_VAL			4
#define ACT_LBUTTON_CLICK		5
#define ACT_LBUTTON_DBLCLICK	6
#define ACT_SELCHANGE			7

void MakeShortString(CDC* pDC, CString &cs, int nColumnLen, int nOffset);


CRDSel::CRDSel(CRect rect, CWnd* pParent /*=NULL*/)
	: ICSDialog(CRDSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRDSel)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_TopIndex = 0;
	m_cbIdx = -1;
	m_CurIndex = 0;
	m_CurDataIdx = 0;
	m_bDIALOG_OK = FALSE;

	m_NowIDC = 0;

	::ZeroMemory( m_Kbsgn, sizeof m_Kbsgn );
	selection_iniz();

	m_DispRect	= rect;
	m_CmnId		= -1;

	m_bShowFlag = FALSE;

	m_Type		= 0;
	m_pParent	= pParent;

	m_RDdate = m_RDmofs = -1;
}


void CRDSel::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRDSel)
	DDX_Control(pDX, IDC_RDLBL_CMBO, m_RDLBL);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_BUTTON1, m_Btn1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RDLIST, m_List);
	DDX_Control( pDX, IDC_CHK_ZEIRITSU, m_Zritu );
}


BEGIN_MESSAGE_MAP(CRDSel, ICSDialog)
	//{{AFX_MSG_MAP(CRDSel)
	ON_CBN_SELCHANGE(IDC_RDLBL_CMBO, OnSelchangeCombo1)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
	ON_NOTIFY( DLN_SETDISPINFO,	IDC_RDLIST, OnSetDispInfo )
	ON_NOTIFY( DLN_SELCHANGED,	IDC_RDLIST, OnLineChanged )
	ON_NOTIFY( DLN_GETDISPINFO,	IDC_RDLIST, OnGetDispInfo )

	ON_MESSAGE( WM_REGDATA, OnMyViewJob )
	ON_BN_CLICKED( IDC_CHK_ZEIRITSU, OnBnClickedChkZeiritsu )
	ON_MESSAGE(WM_USER_SELCHG_MSG, OnSelChgMsg)

	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRDSel メッセージ ハンドラ

BOOL CRDSel::OnInitDialog() 
{
//	ControlInit(500,112,-1);

	ICSDialog::OnInitDialog();
	// TODO: この位置に初期化の補足処理を追加してください
	m_RDLBL.SetSheetTypeMode(FALSE);

	int lbmax =  Mkrd.label->GetRecordCount();
	int cnt = 0;
	CString str;
	if( lbmax > 0 ) {

		CString str;
		CStringArray text;
		CDWordArray	data;

		Mkrd.label->MoveFirst();
		int idx = 0;
		if (Mkrd.label->st != -1) {
			do {
				str.Format(_T("%3d.%s"), (idx + 1), Mkrd.label->dealname);

				text.Add(str);
				data.Add(idx);
				idx++;
			} while (Mkrd.label->MoveNext() == 0);
		}
		m_RDLBL.MakeComboList(text, data);
	}

	m_RDLBL.SetCurSel(0);
	m_CurIndex = 0;
	m_CurDataIdx = 0;

	int btnSt = BST_CHECKED;
	if (!RDsiwakeZeirituMode())	btnSt = BST_UNCHECKED;
	m_Zritu.SetCheck( btnSt );

	TitleBarTextChange();

#ifdef CLOSE
	pDBzm->minfo->MoveLast();
	do {
		if( pDBzm->minfo->insw & 0x01 ) {
			m_RDdate = pDBzm->minfo->emd;
			m_RDmofs = pDBzm->minfo->ofset;
			break;
		}
	} while( pDBzm->minfo->MovePrev() == 0 );
#endif

	m_bDIALOG_OK = TRUE;

	ICSDialog::OnInitDialogEX();

	// 表示位置の調整
	CRect rect;
	GetWindowRect( rect );
	int h, w, h2;
	h = rect.Height();
	h2 = m_DispRect.Height();
	w = rect.Width();

	MoveWindow( m_DispRect.left, m_DispRect.top, w, h2 );

	m_List.GetWindowRect( rect );
	ScreenToClient( rect );

	int list_height; /*= rect.Height() + (h2 - h)*/;
	CRect chkRect;
	m_Zritu.GetWindowRect( chkRect );
	ScreenToClient( chkRect );

	int ht_adj = m_Type ? 3 : 2;

	//税率自動判定のチェックボックス
	//if (! IsMasterType( MST_SYZ8_10 )) {
	//}

	list_height = h2 - rect.top - (chkRect.Height() * ht_adj);

	m_List.MoveWindow( rect.left, rect.top, rect.Width(), list_height );

	StaticWndMove();

	if( ! m_Type ) {
		m_List.SetLineHeight( -1 );
		SetListColumn( m_LinNum );

		DT_ITEMMAX	im;
		im.im_seq = 8;
		im.im_denp = 7;
		im.im_eda = Voln1->edcol;
		im.im_bmn = Voln1->bmcol;
		im.im_koji= Voln1->kjcol;
		im.im_tky = (Voln1->tk_ln * 2);
		im.im_sno = Voln1->snocol;
		m_List.SetImax( &im ); 

		m_List.SetReadOnly(TRUE);
		m_List.EnforceLayout();
	}
	else {
		ACbookListColumn();

		CString	bmn, koji, kmk, brn;
		((CDBAbookView*)m_pParent)->GetACRdselBaseStr(bmn, koji, kmk, brn);

		GetDlgItem(IDC_ST_BUMON)->SetWindowTextA( bmn );
		GetDlgItem(IDC_ST_KOJI)->SetWindowTextA( koji );
		GetDlgItem(IDC_ST_KMKNAM)->SetWindowTextA( kmk );
		GetDlgItem(IDC_ST_BRN)->SetWindowTextA( brn );
	}

	mkrd_data_requery( 1 );

	m_NowIDC = IDC_RDLBL_CMBO;
	PostMessage( WM_REGDATA, defRSL_InizListInp, (LPARAM)&m_RDLBL );

	TitleBarTextChange();

	m_bDIALOG_OK = TRUE;

	// 表示位置の調整
	GetWindowRect( rect );
	h = rect.Height();
	w = rect.Width();

	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	int left = m_DispRect.left;

	CRect mainRect;
	GetDBDinpMainFrame()->GetWindowRect(mainRect);

//MyTrace( "scrX = %d, left = %d, mainRect(%d,%d,%d,%d)\n", scrX, left, mainRect.left, mainRect.top, mainRect.right, mainRect.bottom );
	if( mainRect.right <= scrX || (left <= scrX && mainRect.right > scrX) ) {
		if( (left + w) > scrX ) {
			int ntmp = (left + w) - scrX;
			left -= ntmp;
		}
	}
//MyTrace( "new left = %d\n", left );

	MoveWindow( left, m_DispRect.top, w, h );

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


LRESULT CRDSel::OnMyViewJob( WPARAM wParam, LPARAM lParam )
{
	ICSComboBox *pCOMB = (ICSComboBox*)lParam;
	int oldidx, sel;

	switch( wParam )
	{
	case defRSL_InizListInp:
		oldidx = m_cbIdx;
		sel = m_RDLBL.GetCurSel();
		m_cbIdx = m_RDLBL.GetItemData(sel);

		// データ表示
		if( m_cbIdx == CB_ERR )
			return 0;

		if( oldidx != m_cbIdx ) {
			int cnt = 0;
			if( mkrd_data_requery(m_cbIdx+1) == 0 ) {
				cnt = RecordDataCount();
			}
			// 11.27 /13
			m_List.RemoveAllDtListData();
			m_List.SetDataTotalCount( 0, TRUE );
			LineCursorOFF();

			// ON_NOTIFY( DLN_GETDISPINFO ) が通知される
			m_List.SetCurrentLine(1);
			m_List.SetDataTotalCount( cnt, TRUE );

			m_CurIndex = 0;
			m_CurDataIdx = 0;
		}
		else {
			ListDataSet( 1 );
		}
		break;

	case defRSL_ListClick:
		// リストクリック時の処理
		break;
	}

	return 1;
}


void CRDSel::OnSelchangeCombo1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	Kbt_ActionFncs( IDC_RDLBL_CMBO, ACT_SELCHANGE );
}


// 定型仕訳データリクエリー
int CRDSel::mkrd_data_requery( int num )
{
	CString filter;
	int	owner;

	if( m_CmnId > 0 )	owner = m_CmnId;
	else				owner = 0;

	m_ACdata.RemoveAll();
	m_ACpos.RemoveAll();

	if( Mkrd.label->SetAbsolutePosition( num ) == 0 ) {
		filter.Format( "rd_lbnum = %d AND rd_owner = %d", Mkrd.label->lbnum, owner );

		int st = Mkrd.data->Requery( filter );

		if( m_Type ) {	//出納帳形式
			CDBINPDataRec	rec;

			if( Mkrd.data->st != -1 ) {
				int abs_pos = 1;
				do {
					dbdata_set( FALSE );
					pDBzm->GetCDBData( &rec );

					if( ((CDBAbookView*)m_pParent)->IsACRdData( &rec ) ) {
						rec.m_seq = abs_pos;
						m_ACdata.Add( rec );
						m_ACpos.Add( abs_pos );
					}

					abs_pos++;
				} while( Mkrd.data->MoveNext() == 0 );
			}
		}
		return st;
	}
	else {
		filter.Format( "rd_lbnum = -1 AND rd_owner = %d", owner );
		Mkrd.data->Requery( filter );
	}

	return -1;
}


void CRDSel::ListDataSet( int seq )
{
	int i, newsgn;
	newsgn = 1;

	//出納帳
	if( m_Type ) {
		ACBK_ListDataSet( seq );
		return;
	}

	int keepseq = pDBzm->dbdata->seq;
	BOOL	bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	if( Mkrd.data->GetRecordCount() > 0 ) {
		Mkrd.data->MoveFirst();
		newsgn = 0;
		if( Mkrd.data->st == -1 )	newsgn = 1;
		else {
			for( i = 1; i < seq; i++ ) {
				if( Mkrd.data->MoveNext() != 0 ) {
					// 新規行
					newsgn = 1;
					break;
				}
			}
		}
	}

	DTLIST_DATA	dt;
	COLORREF back_col;
	int ln = m_List.GetCurrentLine();

	m_List.RemoveAllDtListData();
	DBKNREC* pKn;
	int lcnt = m_List.GetDispDataCount();

	if( newsgn ) {
		m_List.RedrawDataListCtrl();
		return;
	}

	for( i = 0; i < lcnt; i++ ) {
		if( Mkrd.data->st != 0 )
			break;

		dt.dd_seq = Mkrd.data->seq;
		CString cstr;

		int dt_idx = i + m_TopIndex;
		if( m_Kbsgn[dt_idx] ) {
			cstr.Format("○%3d", dt.dd_seq);
		} 
		else {
			cstr.Format("  %3d", dt.dd_seq);
		}
		dt.dd_udtbl[0].userdata = (BYTE*)(LPCTSTR)cstr;

		dt.dd_ksign = -1;
		dt.dd_ddate = -1;
		dt.dd_mofs = -1;
		dt.dd_cno = -1;

		dt.dd_dbmn = Mkrd.data->dbmn;

		struct _SET_KMKETC sk;
		sk.bmn = dt.dd_dbmn;
		sk.brn = -1;
		sk.code.Empty();
		sk.kno.Empty();

		if (bDisp && _set_bmnetc(&sk)) {
			dt.dd_dbmn_etc = sk.etc;
			dt.dd_ctbl[DATA_DEB_BMON].text = sk.etc_col;
		}
		else {
		dt.dd_dbmn_etc.Empty();
			dt.dd_ctbl[DATA_DEB_BMON].text = (DWORD)-1;
		}

		dt.dd_dkno = Mkrd.data->dkno;
		sk.bmn = -1;
		sk.brn = -1;
		sk.code.Empty();
		sk.kno = dt.dd_dkno;
		int bflen = 128;
		int len = sk.kno.GetLength();
		if (len > 0) {
			LPSTR p = sk.kno.GetBuffer(bflen);
			trim_zero(p, bflen, len);
			sk.kno.ReleaseBuffer();
		}

		if (bDisp && _set_kojietc(&sk)) {
			dt.dd_dkno_etc = sk.etc;
			dt.dd_ctbl[DATA_DEB_KNO].text = sk.etc_col;
		}
		else {
		dt.dd_dkno_etc.Empty();
			dt.dd_ctbl[DATA_DEB_KNO].text = (DWORD)-1;
		}

		dt.dd_dbt = Mkrd.data->dbt;
		pKn = pDBzm->DB_PjisToKnrec( dt.dd_dbt );
		if( pKn ) {
			KamokuString(pKn, dt.dd_deb_name);
		}
		else {
			dt.dd_deb_name.Empty();
		}
		dt.dd_dbr = Mkrd.data->dbr;

		sk.bmn = dt.dd_dbmn;
		sk.brn = dt.dd_dbr;
		sk.code = dt.dd_dbt;
		if (_set_kmketc(&sk, TRUE)) {
			dt.dd_deb_etc = sk.etc;
			dt.dd_ctbl[DATA_DEBT].text = sk.etc_col;
		}
		else {
			dt.dd_deb_etc.Empty();
			dt.dd_ctbl[DATA_DEBT].text = (DWORD)-1;
		}

		//貸方部門
		dt.dd_cbmn = Mkrd.data->cbmn;
		sk.bmn = dt.dd_cbmn;
		sk.brn = -1;
		sk.code.Empty();
		sk.kno.Empty();

		if (bDisp && _set_bmnetc(&sk)) {
			dt.dd_cbmn_etc = sk.etc;
			dt.dd_ctbl[DATA_CRE_BMON].text = sk.etc_col;
		}
		else {
		dt.dd_cbmn_etc.Empty();
			dt.dd_ctbl[DATA_CRE_BMON].text = (DWORD)-1;
		}
		//貸方工事
		dt.dd_ckno = Mkrd.data->ckno;

		sk.bmn = -1;
		sk.brn = -1;
		sk.code.Empty();
		sk.kno = dt.dd_ckno;
		len = sk.kno.GetLength();
		if (len > 0) {
			LPSTR p = sk.kno.GetBuffer(bflen);
			trim_zero(p, bflen, len);
			sk.kno.ReleaseBuffer();
		}

		if (bDisp && _set_kojietc(&sk)) {
			dt.dd_ckno_etc = sk.etc;
			dt.dd_ctbl[DATA_CRE_KNO].text = sk.etc_col;
		}
		else {
		dt.dd_ckno_etc.Empty();
			dt.dd_ctbl[DATA_CRE_KNO].text = (DWORD)-1;
		}

		dt.dd_cre = Mkrd.data->cre;
		pKn = pDBzm->DB_PjisToKnrec( dt.dd_cre );
		if( pKn ) {
			KamokuString(pKn, dt.dd_cre_name);
		}
		else	dt.dd_cre_name.Empty();
		dt.dd_cbr = Mkrd.data->cbr;

		sk.bmn = dt.dd_cbmn;
		sk.brn = dt.dd_cbr;
		sk.code = dt.dd_cre;
		if (_set_kmketc(&sk, TRUE)) {
			dt.dd_cre_etc = sk.etc;
			dt.dd_ctbl[DATA_CRED].text = sk.etc_col;
		}
		else {
			dt.dd_cre_etc.Empty();
			dt.dd_ctbl[DATA_CRED].text = (DWORD)-1;
		}

		l_input( dt.dd_val, (char*)(LPCTSTR)Mkrd.data->val );
		dt.val_flag = Mkrd.data->valflag;
		l_input( dt.dd_zei, (char*)(LPCTSTR)Mkrd.data->zei );
		dt.zei_flag = Mkrd.data->zeiflag;

		dbdata_set( TRUE );
		dt.dd_syz_str = rd_syline_txt();
		dt.dd_dsign = 0;

		if ( pDBsy->IsTaikaData( pDBzm->dbdata ) == 2 ) {
			dt.zei_flag |= 0x10;
		}
		if( ! is_syohizeisiwake( pDBzm->dbdata ) ) {
			dt.zei_flag = 0;
		}
		dt.dd_tek.tekiyo = Mkrd.data->tekiyo;
		dt.dd_tek.width = 0;
		dt.dd_tek.height = 0;
		dt.dd_tek.image = NULL;

		dt.dd_snumber = Mkrd.data->snumber;
		dt.dd_tdate = Mkrd.data->tdate;
		dt.dd_tdstr.Empty();
		dt.dd_husen = 0;
		dt.dd_hscomment.Empty();

		back_col = GetRdLineBackColor( i );

		for( int k = 0; k < DATA_CNT; k++ ) {
			dt.dd_ctbl[k].back = back_col;
		}

		m_List.SetDtListData( i, &dt, FALSE );

		if( Mkrd.data->MoveNext() != 0 )
			break;
	}

//	if( keepseq > 0 ) {
//		pDBzm->DBDATA_SetPosition( keepseq );
//	}

	if( ln != -1 ) 
		m_List.SetCurrentLine( ln );

	m_List.RedrawDataListCtrl();

}


COLORREF CRDSel::GetRdLineBackColor( int list_idx )
{
	COLORREF back_col = NORMAL_BKCOL;

	int max_count = RecordDataCount();
	int ar_max;
	int index = list_idx + m_TopIndex;

	ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
ASSERT( list_idx < ar_max );

	if( index < max_count && index < ar_max )
	{
		if( m_Kbsgn[index] ) 
		{
			back_col = SELECT_BKCOL;
		}
		else
		{
			back_col = NORMAL_BKCOL;
		}
	}	

	return back_col;
}



// 消費税モジュールのSyIniz 呼び出しため CDBDATA にデータをセット
void CRDSel::dbdata_set( BOOL syinit )
{

	pDBzm->dbdata->seq = 1;
	pDBzm->dbdata->ksign = 0;

	if( m_RDdate == -1 ) {
		pDBzm->dbdata->ddate	= pDBzm->zvol->ss_ymd;
		pDBzm->dbdata->mofs		= 0;
	}
	else {
		pDBzm->dbdata->ddate	= m_RDdate;
		pDBzm->dbdata->mofs		= m_RDmofs;
	}

	pDBzm->dbdata->cno = -1;
	pDBzm->dbdata->dbmn	= Mkrd.data->dbmn;
	pDBzm->dbdata->dkno	= Mkrd.data->dkno;
	pDBzm->dbdata->dbt	= Mkrd.data->dbt;
	pDBzm->dbdata->dbr	= Mkrd.data->dbr;
	pDBzm->dbdata->cbmn	= Mkrd.data->cbmn;
	pDBzm->dbdata->ckno	= Mkrd.data->ckno;
	pDBzm->dbdata->cre	= Mkrd.data->cre;
	pDBzm->dbdata->cbr	= Mkrd.data->cbr;
	pDBzm->dbdata->val	= Mkrd.data->val;
	if( pDBzm->dbdata->val.IsEmpty() )	pDBzm->dbdata->val = "0";
	pDBzm->dbdata->zei	= Mkrd.data->zei;
	pDBzm->dbdata->dsign.Copy( Mkrd.data->dsign );
	pDBzm->dbdata->owner	= -1;
	pDBzm->dbdata->dtype	= -1;
	pDBzm->dbdata->tekiyo	= Mkrd.data->tekiyo;
	pDBzm->dbdata->imgsq	= -1;
	pDBzm->dbdata->snumber	= Mkrd.data->snumber;
	pDBzm->dbdata->tdate	= Mkrd.data->tdate;
	pDBzm->dbdata->edate	= -1;
	pDBzm->dbdata->udate	= -1;

	pDBzm->dbdata->hjiyu	= Mkrd.data->hjiyu;
	pDBzm->dbdata->taika	= Mkrd.data->taika;

	if( syinit )	pDBsy->SyIniz( pDBzm->dbdata );
}

// 消費税文字列作成
LPCTSTR CRDSel::rd_syline_txt()
{
	struct _SY_MSG_PACK2 smpk;
	static CString txt;
	
	pDBsy->SyMsg( &smpk );

	// 10%の範囲外
#ifdef CLOSE //定型仕訳選択時は、登録の表示を優先し、日付から自動で税率を変更しない
	if( IsMasterType( MST_SYZ8_10 ) ) {
		CDBINPDataRec	tmpRec;
		pDBzm->GetCDBData( &tmpRec );
		DWORD	syCode;
		if( ((CDBbaseView*)p_DBDinpView)->rd_reccheck( &tmpRec, &tmpRec, syCode ) ) {
			if( pDBsy->SyFnc( syCode ) == 0 ) {
				struct _SY_MSG_PACK2 smpk;
				pDBsy->SyMsg( &smpk );
			}
		}
	}
#endif
	txt = pDBsy->Sy_get_message( pDBzm->dbdata, SYGET_TYPE );
	return txt;
}


void CRDSel::OnSetDispInfo( NMHDR* pNMHDR, LRESULT* pResult)
{

}

void CRDSel::OnLineChanged( NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;

	NM_DTLNCHG* nmdt;
	nmdt = (NM_DTLNCHG*)pNMHDR;

	int	index, oldidx;
	long nChar;

	int lcnt = m_List.GetDispDataCount();
	nChar = nmdt->nChar;
	oldidx = m_CurIndex;
	index = (nmdt->iLine - 1);
TRACE("□□OnLineChanged--- oldidx, index (%d, %d)\n", oldidx, index);
TRACE("□□OnLineChanged--- nFlags(%04x) nChar = %04x\n", nmdt->nFlags, nChar);

	// クリックでも選択する
	if( nmdt->nChar == 0 && nmdt->iClick ) {
		long nChar = 0;
		if( nmdt->iClick & 0x10 )	nChar = VK_SHIFT;
		if( nmdt->iClick & 0x20 )	nChar = VK_CONTROL;

		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd && pwnd->m_hWnd != m_List.m_hWnd ) {
			FocusFncs(IDC_RDLIST, ACT_FOCUS_ON);
		}
		click_seljob(nmdt->iLine, nChar);
	}
	else {
		if( nChar == VK_UP || nChar == VK_DOWN ) {
			// KeyUp からのメッセージの場合、現在行のみ処理して、return
			// KeyDownで処理していることは何もしない。(WM_VSCROLLなど)
			if( (nmdt->nFlags & 0x02) || (nmdt->nFlags & 0x04) ) {
				sel_shiftjob(index, nmdt->nChar, is_SHIFT());
				sel_lineupdate();
				return;
			}
			else {
				//m_CurIndex の行
				sel_shiftjob((nmdt->nFlags &0x01) ? -2 : -1, nmdt->nChar, is_SHIFT());
			}
		}
		else if( nChar == VK_SPACE ) {
			// KEYUPのみ処理する
			if( (nmdt->nFlags & 0x06) ) {
			}
			else {
				nChar = 0;
			}
		}
	}

	m_CurIndex = index;
	m_CurDataIdx = m_CurIndex + m_TopIndex;

//MyTrace( "@OnLineChanged nChar = %d, iLine = %d, nFlags = %08x\n", nChar, nmdt->iLine, nmdt->nFlags );

	switch( nChar ){
	case VK_UP:
		if( oldidx == 0 && index == 0 && m_TopIndex > 0 ) {
			m_List.SendMessage( WM_VSCROLL, MAKELONG(SB_LINEUP, 0 ) );
		}
		break;
	case VK_DOWN:
		if( index == (lcnt-1) && oldidx == (lcnt-1) ) {
			if( (m_TopIndex + lcnt) < RecordDataCount() )
				m_List.SendMessage( WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0 ) );
		}
		break;

	case VK_SPACE:
		Kbt_ActionFncs( IDC_RDLIST, ACT_KEYIN_ONLY, nChar, index );
		break;

	case VK_TAB:
	case VK_RETURN:
		Kbt_ActionFncs( IDC_RDLIST, ACT_KEYIN_ONLY, nChar, index );
		break;
		
	default:
		break;
	}

	sel_lineupdate();
}


// 表示要求 CDataListCtrl より
void CRDSel::OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult)
{
	
	DLCT_DISPINFO* di;
	di = (DLCT_DISPINFO*)pNMHDR;

	m_TopIndex = di->TopIndex;
	ListDataSet( (m_TopIndex+1) );

	*pResult = 0;

TRACE( "CRDSel::OnGetDispInfo = %d\n", m_TopIndex );
}



// ラインカーソル関係
//
void CRDSel::LineCursorOFF()
{
	int line;
	DTLIST_DATA	dt;

	// イニシャライズ
	::ZeroMemory( m_Kbsgn, sizeof m_Kbsgn );
	selection_iniz();

	int lcnt, dcnt, max;
	lcnt = m_List.GetDispDataCount();
	dcnt = RecordDataCount();

	max = __min( lcnt, dcnt );

	// 選択ＯＦＦ
	for(  line = 0 ; line < max; ++line )
	{
		m_List.GetDtListData( line, &dt );

		// 同背景色の場合は、スキップ
		if( dt.dd_ctbl[0].back == NORMAL_BKCOL )
			continue;

		// 背景色セット
		for( int pn = 0; pn < DATA_CNT; pn++ )
		{
			dt.dd_ctbl[pn].back = NORMAL_BKCOL;
		}

		m_List.SetDtListData( line, &dt, FALSE );
	}

	// 表示
	Kbn_Rngdsp();
}


BOOL CRDSel::SetLineCursor( short index )
{
	DTLIST_DATA	dt;
//TRACE( "@SetLineCursor top = %d\n", index );

	if( index < 0 )	return FALSE;

	int max_count = RecordDataCount();	// 登録数
	int ar_max;
	ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
	ASSERT(ar_max > max_count);

	BOOL bData = FALSE;
	COLORREF back_col;

	for( int list_idx = 0; list_idx < max_count; list_idx++ ) {
		back_col = NORMAL_BKCOL;
		if( m_List.GetDtListData(list_idx, &dt) != 0 )
			continue;

		CString cstr;

		if( m_Kbsgn[list_idx] ) {
			back_col = SELECT_BKCOL;
			cstr.Format("○%3d", dt.dd_seq);
		}
		else {
			cstr.Format("  %3d", dt.dd_seq);
		}
		dt.dd_udtbl[0].userdata = (BYTE*)(LPCTSTR)cstr;
		// 同背景色の場合は、スキップ
//		if( dt.dd_ctbl[0].back == back_col )
//			continue;

		// 背景色セット
		for( int pn = 0; pn < DATA_CNT; pn++ )
		{
			dt.dd_ctbl[pn].back = back_col;
		}

		m_List.SetDtListData(list_idx, &dt, FALSE);
	}
	m_List.RedrawDataListCtrl();

	return TRUE;
}


//
// 選択番号を表示
void CRDSel::Kbn_Rngdsp()
{
#ifdef CLOSE
	int max_count = RecordDataCount();	// 登録数
	int ar_max;
	ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
ASSERT( ar_max > max_count );

	CString msg;
	char	bf[20];
	int sg = 0;

	for( int i = 0; i < max_count; i++ ) {
		if( m_Kbsgn[i] ) {
			if( sg )	sprintf_s( bf, sizeof bf, ",%d", (i+1) );
			else		sprintf_s( bf, sizeof bf, "%d", (i+1) );
			msg += bf;
			sg++;
		}
	}

	CWnd* pwnd;
	pwnd = GetDlgItem(IDC_STATIC4);

	if( pwnd ) {
		CClientDC dc( pwnd );
		CFont* font = pwnd->GetFont();
//		font->GetLogFont(&lf);
		dc.SelectObject( font );
		
		CRect rect;
		pwnd->GetWindowRect( &rect );
		ScreenToClient( rect );
		rect.DeflateRect( 2, 0 );

		MakeShortString( &dc, msg, rect.Width(), 0 ); 
		pwnd->SetWindowText( msg );
	}
#endif
}


//
// フォーカス
//
void CRDSel::FocusFncs( UINT IDC, int FocusMode )
{
	BOOL _1line = FALSE;

	switch( IDC )
	{
		// 仕訳グループ
	case IDC_RDLBL_CMBO:
		switch( FocusMode )
		{
		case ACT_FOCUS_ON:

			LineCursorOFF();
		//	GetDlgItem( IDC_BUTTON1 )->ShowWindow( SW_HIDE );
			
			m_NowIDC = IDC;
			break;

		case ACT_FOCUS_OFF:
			break;
		}		
		break;


		// 書込み確認
	case IDC_BUTTON1:
		switch( FocusMode )
		{
		case ACT_FOCUS_ON:
			
			
			m_NowIDC = IDC_BUTTON1;
			break;

		case ACT_FOCUS_OFF:
			break;
		}
		break;

		// 仕訳表示
	case IDC_RDLIST:
		switch( FocusMode )
		{
		case ACT_FOCUS_ON:
			if( m_RDLBL.GetDroppedState( ) )
				m_RDLBL.ShowDropDown( FALSE );

				m_List.SetFocus();
			break;

		case ACT_FOCUS_OFF:
			break;
		}
		break;
	}
}

// Kbt_ActionFncs を使用
// アクション(未使用)
//
int CRDSel::ActionFncs( UINT IDC, int actionmode, long nChar, int val, VOID *pval )
{
	// Delete ＪＯＢ
	switch( actionmode )
	{
	case ACT_KEYIN_ONLY:
		switch( nChar )
		{
		case VK_DELETE:
			switch( IDC )
			{
			case IDC_BUTTON1:
				LineCursorOFF();
				m_NowIDC = 0;
				GotoDlgCtrl( GetDlgItem( IDC_RDLIST ) );
				return 1;
			}
			break;
		}
		break;
	}

	//
	switch( IDC )
	{
		// 仕訳グループ
	case IDC_RDLBL_CMBO:
		switch( actionmode )
		{
		case ACT_KEYIN_ONLY:
			break;
		case ACT_KEYIN_VAL:
			break;
		case ACT_LBUTTON_CLICK:
			break;
		case ACT_LBUTTON_DBLCLICK:
			m_RDLBL.PostMessage( WM_KEYDOWN, VK_RETURN );
			return 1;

		case ACT_SELCHANGE:
			PostMessage( WM_REGDATA, defRSL_InizListInp, (LPARAM)&m_RDLBL );
			return 1;

		}
		break;
		

		// 書込み確認
	case IDC_BUTTON1:
		switch( actionmode )
		{
		case ACT_KEYIN_ONLY:
			switch( nChar )
			{
			case VK_TAB:
				if( is_SHIFT() )
				{
					// シフトＴＡＢ
					GotoDlgCtrl( &m_RDLBL );
					return 1;
				}
				else {
					GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
					return 1;
				}
				break;
			case VK_LEFT:
			case VK_F2:
			case VK_UP:
				GotoDlgCtrl( &m_RDLBL );
				return 1;

			case VK_RIGHT:
				GotoDlgCtrl(GetDlgItem(IDCANCEL));
				return 1;

			case VK_DOWN:
				GotoDlgCtrl(GetDlgItem(IDC_RDLIST));
				return 1;

			case VK_NEXT:
			case VK_PRIOR:
				break;
			}
			break;

		case ACT_KEYIN_VAL:
			break;
		case ACT_LBUTTON_CLICK:
			break;
		case ACT_LBUTTON_DBLCLICK:
			break;
		case ACT_SELCHANGE:
			break;
		}
		break;

		// 仕訳表示
	case IDC_RDLIST:
		switch( actionmode )
		{
		case ACT_KEYIN_ONLY:
			switch( nChar )
			{
//			case VK_RETURN:
//			case VK_SPACE:
//				SendData( Mkrd.data->GetTopIndex() + index );	// 親元にデータ送出
//				PostMessage( WM_KEYDOWN, VK_ESCAPE );
//				break;
			case VK_RIGHT:
//				Mkrd.data->SendMessage( WM_HSCROLL, SB_PAGERIGHT );
				return 1;

			case VK_LEFT:
//				Mkrd.data->SendMessage( WM_HSCROLL, SB_PAGELEFT );
				return 1;
			}
			break;

		case ACT_KEYIN_VAL:
			break;
		case ACT_LBUTTON_CLICK:
			if( m_NowIDC == IDC_BUTTON1 )
			{
				LineCursorOFF();
				Kbt_SetLineCursor( val );
			}
			else
				Kbt_SetLineCursor( val );

			return 1;

		case ACT_LBUTTON_DBLCLICK:
			LineCursorOFF();
			FocusFncs( IDC_BUTTON1, ACT_FOCUS_ON );
			return 1;

		case ACT_SELCHANGE:
			if( val > -1 )
			{
				SetLineCursor( val );
			}
			return 1;
		}
		break;
	}
	return 0;
}

// 全選択処理
int CRDSel::AllSelJob()
{
	int sgn = 0;

	if( m_CurDataIdx < 0 ) {
		sgn = 1;
	}
	else {
		if( m_Kbsgn[m_CurDataIdx] )
			sgn = 0;
		else
			sgn = 1;

		if( sgn == 0 ) {
			::ZeroMemory(m_Kbsgn, sizeof m_Kbsgn);
		}
		else {
			int max = RecordDataCount();

			for( int n = 0; n < max; n++ ) {
				m_Kbsgn[n] = sgn;
			}
		}
		m_SelUpdate = TRUE;
		sel_lineupdate();
	}
	return 0;
}

//
// 個別指定 アクション
//
int CRDSel::Kbt_ActionFncs( UINT IDC, int actionmode, long nChar, int val, VOID *pval )
{

	// Delete ＪＯＢ
	switch( actionmode )
	{
	case ACT_KEYIN_ONLY:
		switch( nChar )
		{
		case VK_DELETE:
			switch( IDC )
			{
			case IDC_RDLIST:
			case IDC_BUTTON1:
				LineCursorOFF();
				m_NowIDC = 0;
				GotoDlgCtrl( &m_RDLBL );
				return 1;
			}
			break;
		}
		break;
	}

	switch( IDC ) {
		// 仕訳グループ
	case IDC_RDLBL_CMBO:
		switch( actionmode )
		{
		case ACT_KEYIN_ONLY:
			break;
		case ACT_KEYIN_VAL:
			break;
		case ACT_LBUTTON_CLICK:
			break;
		case ACT_LBUTTON_DBLCLICK:
			m_RDLBL.PostMessage(WM_KEYDOWN, VK_RETURN);
			return 1;

		case ACT_SELCHANGE:
			PostMessage(WM_REGDATA, defRSL_InizListInp, (LPARAM)&m_RDLBL);
			return 1;

		}
		break;

		// 書込み確認
	case IDC_BUTTON1:
		switch( actionmode )
		{

		case ACT_KEYIN_ONLY:
			switch( nChar )
			{
			case VK_TAB:
				if( is_SHIFT() )
				{
					// シフトＴＡＢ
					GotoDlgCtrl(&m_RDLBL);
					return 1;
				}
				// else は 下へ
			case VK_RIGHT:
				GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
				return 1;			
			case VK_LEFT:
			case VK_F2:
				GotoDlgCtrl( GetDlgItem( IDC_RDLBL_CMBO ) );
				return 1;

			case VK_DOWN:
				GotoDlgCtrl(GetDlgItem(IDC_RDLIST));
				return 1;

			case VK_NEXT:
			case VK_PRIOR:
				break;
			}
			break;

		case ACT_KEYIN_VAL:
			break;
		case ACT_LBUTTON_CLICK:
			break;
		case ACT_LBUTTON_DBLCLICK:
			break;
		case ACT_SELCHANGE:
			break;
		}
		break;

		// 仕訳表示
	case IDC_RDLIST:
		switch( actionmode )
		{

		case ACT_KEYIN_ONLY:
			switch( nChar )
			{			
			case VK_SPACE:
				Kbt_SetLineCursor( m_TopIndex + val );
				break;

			case VK_RIGHT:
//				Mkrd.data->SendMessage( WM_HSCROLL, SB_PAGERIGHT );
				return 1;

			case VK_LEFT:
//				Mkrd.data->SendMessage( WM_HSCROLL, SB_PAGELEFT );
				return 1;

			case VK_RETURN:
				if( Kbn_DataCheck() )
					GotoDlgCtrl(GetDlgItem(IDC_BUTTON1));
				return 1;
			case VK_TAB:
				if( is_SHIFT() )
				{
					// シフトＴＡＢ
					GotoDlgCtrl(GetDlgItem(IDC_RDLBL_CMBO));
				}
				else {
					GotoDlgCtrl(GetDlgItem(IDC_BUTTON1));
				}
				return 1;

			}
			break;

		case ACT_KEYIN_VAL:
			break;
		case ACT_LBUTTON_CLICK:
			Kbt_SetLineCursor( val );
			return 1;

		case ACT_LBUTTON_DBLCLICK:
			Kbt_SetLineCursor( val );
			return 1;

		case ACT_SELCHANGE:
			return 1;
		}
		break;
	}

	return 0;
}


BOOL CRDSel::Kbt_SetLineCursor( short index, int shift_sel/*=0*/ )
{
	DTLIST_DATA dt;
	int list_idx;

	if( index >= 0 )
	{
		int max_count = RecordDataCount();
		int ar_max;
		ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
ASSERT( index < ar_max );

		list_idx = index - m_TopIndex;

		if( index < max_count && index < ar_max )
		{
			int sgn;

			if( shift_sel ) {
				m_Kbsgn[index] = 1;
			}
			else {
				sgn = m_Kbsgn[index];
				m_Kbsgn[index] = !sgn;
			}

			COLORREF back_col;

			m_List.GetDtListData( list_idx, &dt );
			CString cstr;

			if( m_Kbsgn[index] ) 
			{
				back_col = SELECT_BKCOL;
				cstr.Format("○%3d", dt.dd_seq);
			}
			else
			{
				back_col = NORMAL_BKCOL;
				cstr.Format("  %3d", dt.dd_seq);
			}
			dt.dd_udtbl[0].userdata = (BYTE*)(LPCTSTR)cstr;
			// 背景色セット
			for( int k = 0; k < DATA_CNT; k++ ) {
				dt.dd_ctbl[k].back = back_col;
			}

			m_List.SetDtListData( list_idx, &dt, TRUE );

			// 表示
			Kbn_Rngdsp();

			return TRUE;
		}
	}

	// 表示
	Kbn_Rngdsp();

	return FALSE;
}


BOOL CRDSel::Kbn_DataCheck()
{
	int max_count = RecordDataCount();	// 登録数
	int ar_max;
	ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
ASSERT( ar_max > max_count );

	BOOL bData = FALSE;
	
	for( int i = 0; i < max_count; i++ ) {
		if( m_Kbsgn[i] ) {
			bData = TRUE;
			break;
		}
	}
	return bData;
}


//
// 該当インデックスページ表示
//
BOOL CRDSel::FindPageDisp( int num )
{
	int line = m_List.GetDispDataCount();	// データ表示行
	BOOL ret = FALSE;

	// ページ移動
	while( num < m_TopIndex || num >= m_TopIndex + line )
	{
		if( num < m_TopIndex )
		{
			m_List.SendMessage( WM_VSCROLL, SB_PAGEUP );
			ret = TRUE;
		}
		else
		{
			m_List.SendMessage( WM_VSCROLL, SB_PAGEDOWN );
			ret = TRUE;
		}
	}
	// 行移動
	int idx = m_TopIndex;
	int lcnt = m_List.GetDispDataCount();

	if( num > (m_TopIndex+lcnt) || num < idx ) {
		while( idx != num )
		{
			if( idx > num )
			{
				m_List.SendMessage( WM_VSCROLL, SB_LINEUP );
				idx--;
				ret = TRUE;
			}
			else
			{
				m_List.SendMessage( WM_VSCROLL, SB_LINEDOWN );
				idx++;
				ret = TRUE;
			}
		}
	}

	return ret;
}



BEGIN_EVENTSINK_MAP(CRDSel, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CRDSel)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void CRDSel::SendData( void )
{
	int n;

	m_pTBL->RD_lbnum = -1;
	m_pTBL->RD_cnt = 0;
	m_pTBL->RD_array.RemoveAll();

	Mkrd.data->MoveFirst();
	if( Mkrd.data->st == -1 )
		return;

	m_pTBL->RD_lbnum = Mkrd.data->lbnum;
	n = 0;

	// 開始番号＆転送数
	int ar_max;
	ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
	n = 0;
	for( int i = 0; i < ar_max; i++ ) {
		if( m_Kbsgn[i] ) {
			n++;
			if( ! m_Type ) {
				m_pTBL->RD_array.Add( i+1 );
			}
			else {
				m_pTBL->RD_array.Add( m_ACpos[i] );
			}
		}
	}

	m_pTBL->RD_cnt = n;
}



/**************************
	Shift key check
***************************/
BOOL CRDSel::is_SHIFT()
{
	return( GetKeyState( VK_SHIFT ) < 0 );
}

/**************************
	Ctrl key check
***************************/
BOOL CRDSel::is_CTRL()
{
	return( GetKeyState( VK_CONTROL ) < 0 );
}



void CRDSel::OnButton1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SendData();

	OnOK();
}

void CRDSel::OnButton2() 
{
	TitleBarTextChange();	
}


// タイトルバーに ガイドを出す
void CRDSel::TitleBarTextChange()
{
	char* title = "定型仕訳の選択";
	CString txt;

	txt.Format("%s    [Shift+↑↓矢印キー]:範囲選択 [スペース]:個別選択", title);

	SetWindowText( txt );
}


void CRDSel::OnShowWindow(BOOL bShow, UINT nStatus) 
{
//	static int flag = 0;
	
	ICSDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
//	if( flag == 0 ) {
//		flag = 1;

	if( ! m_bShowFlag ) {
		m_bShowFlag = TRUE;
		GotoDlgCtrl( &m_RDLBL );
		//if( !m_RDLBL.GetDroppedState( ) )
		//	m_RDLBL.ShowDropDown( TRUE );
	}
}


void MakeShortString(CDC* pDC, CString &cs, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[]=_T("...");

	int nStringLen = cs.GetLength();

	if (nStringLen == 0 || pDC->GetTextExtent(cs).cx + nOffset <= nColumnLen) return;

	static _TCHAR szShort[MAX_PATH];
	if (nStringLen >= MAX_PATH) {
		strncpy_s(szShort, sizeof szShort, cs, MAX_PATH - 1);
		nStringLen = MAX_PATH - 1;
	}
	else {
		lstrcpy(szShort, cs);
	}


	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;
 
	for (int i = nStringLen-1; i > 0; i--)
	{
		szShort[i]=0;
		if(pDC->GetTextExtent(szShort,i).cx+nOffset+nAddLen<=nColumnLen)
			break;
	}

	lstrcat(szShort,szThreeDots);
	cs = szShort;
}


////////////////////////////////////////////////////////////////////////////////////////////
// 表示行設定
//
#include "DtClmItem.h"


DT_CLMITEM rdLINE1[] = {
	{ 0, "番号", DATA_USER1, DT_KIND_STRING | DT_FMT_RIGHT, "", -1,-1,	7, 7,	0,	0, 0, -1 },
//	{ 0, "", DATA_DATE,	0, "", -1,-1,	6, 6,	0, -1, 0, -1 },
//	{ 0, "", DATA_DENP,	0, "", -1,-1,	6, 6,	0, -1, 0, -1 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	 7,	7, 0, -1, 0, -1 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,	12,12, 0, -1, 0, -1 },

	{ 0, "", DATA_DEBT,		0, "", -1,-1, 14+5,14+5, 0, -1, 0, -1 },
	{ 0, "", DATA_CRED,		0, "", -1,-1, 14+5,14+5, 0, -1, 0, -1 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,15, 0, -1, 0, -1 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	13,13, 0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8, 0, -1, 0, -1 },

	{ 0, "", DATA_TEKIYO,	DT_INP_BOTTOMRIGHT, "", -1,-1,	80,80, 0, -1, 0,  -1 },
};

// 伝票 + 部門(2) + 工事(2)
DT_CLMITEM rdLN2_BK2[] = {

	{ 0, "番号", DATA_USER1, DT_KIND_STRING| DT_FMT_RIGHT, "", -1,-1,	6,	6,	0,	0, 0, -1 },
//	{ 0, "", DATA_DATE,	0, "", -1,-1,	6,	6,	0, -1, 0, -1 },
//	{ 1, "", DATA_DENP,	0, "", -1,-1,	6,	6,	0,  0, 0,  0 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	 8,   8, 0, -1, 0, -1 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,	12,  12, 0, -1, 0, -1 },
	{ 1, "", DATA_DEBT,		0, "", -1,-1, 14+6,14+5, 1, -1, 0,  0 },

	{ 0, "", DATA_CRE_BMON, 0, "", -1,-1,	8,    8, 0, -1, 0,  2 },
	{ 0, "", DATA_CRE_KNO,	0, "", -1,-1,	12,  12, 0, -1, 0, -1 },
	{ 1, "", DATA_CRED,		0, "", -1,-1, 14+6,14+6, 1, -1, 0,  3 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	15,	0, -1, 0,  5 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	13,	13,	0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,	0, -1, 0, -1 },

	{ 1, "", DATA_TEKIYO,	DT_INP_TOPRIGHT, "", -1,-1,	80,80,	0, -1, 0,  6 },

//	{ 0, "", DATA_SYONO,	0, "", -1,-1,	8,	8,	0, -1, 0, 11 },
//	{ 0, "", DATA_TGDATE,	0, "", -1,-1,	9,	9,	0, -1, 0, -1 },
//	{ 0, "", DATA_HUSEN,	0, "", -1,-1,	4,	4,	0, -1, 0, -1 },
};





void CRDSel::SetListColumn( int line )
{
	CArray<DT_COLUMN, DT_COLUMN&> clm;
	int i, max, cnt, kmk_lng;
	DT_COLUMN dtcol;
	DT_CLMITEM* pDisp;
	kmk_lng		= 14 + Voln1->edcol;

	if( line == 1 ) {
		max = (sizeof rdLINE1 / sizeof rdLINE1[0]);
		clm.SetSize( max );

		pDisp = rdLINE1;

		for( i = cnt = 0; i < max; i++ ) {
			if( pDisp[i].id == DATA_DENP ) {
//				if( pAUTOSEL->DSPDENP_OPT == OPT_NODSP )
//					continue;
			}
			if( pDisp[i].id == DATA_DEB_BMON ) {
//				if( pAUTOSEL->DSPBMON_OPT == OPT_NODSP )
//					continue;

				if( pDBzm->zvol->bmcol <= 0 )
					continue;
				pDisp[i].inpsize = pDBzm->zvol->bmcol;
				pDisp[i].dispsize= pDBzm->zvol->bmcol;
			}
			if( pDisp[i].id == DATA_DEB_KNO ) {
				if( pDBzm->zvol->apno == 0x20 ) {
//					if( pAUTOSEL->DSPKOJI_OPT == OPT_NODSP )
//						continue;

					if( !(pDBzm->zvol->sub_sw&0x04) )
						continue;

					pDisp[i].inpsize = pDBzm->zvol->kjcol;
					pDisp[i].dispsize= pDBzm->zvol->kjcol;
				}
				else {
					continue;
				}
			}
			if( pDisp[i].id == DATA_DEBT || pDisp[i].id == DATA_CRED ) {
				pDisp[i].inpsize	= kmk_lng;
				pDisp[i].dispsize	= kmk_lng;
			}

			dtcol.ln_ofs	= pDisp[i].ln_ofs;
			dtcol.clm_title	= pDisp[i].title;
			dtcol.clm_id	= pDisp[i].id;
			dtcol.clm_type	= pDisp[i].type;
			dtcol.clm_fmt	= pDisp[i].format;
			dtcol.clm_back	= pDisp[i].back;
			dtcol.clm_text	= pDisp[i].text;
			dtcol.clm_inpsz	= pDisp[i].inpsize;
			dtcol.clm_size	= pDisp[i].dispsize;
			dtcol.clm_szadj	= pDisp[i].size_adj;
			dtcol.clm_spos	= pDisp[i].spos;
			dtcol.clm_adj	= pDisp[i].spos_adj;
			dtcol.clm_sidx	= pDisp[i].clm_sidx;

			clm.SetAt( cnt, dtcol );
			cnt++;
		}
		m_List.SetItemColumn( &clm[0], cnt );
	}
	else {
		// 2行うち
		max = (sizeof rdLN2_BK2 / sizeof rdLN2_BK2[0]);
		clm.SetSize( max );

		pDisp = rdLN2_BK2;
		int id;
		int bmnkoji_lng, kmk_lng, nMax;
		bmnkoji_lng	= pDBzm->zvol->bmcol + 12;
		kmk_lng		= 14 + pDBzm->zvol->edcol;

		nMax = __max( bmnkoji_lng, kmk_lng );

		for( i = cnt = 0; i < max; i++ ) {
			id = pDisp[i].id;
			if( id == DATA_DENP ) {
//				if( pAUTOSEL->DSPDENP_OPT == OPT_NODSP ) {
//					id = DATA_NONE;
//				}
			}
			if( id == DATA_DEB_BMON || id == DATA_CRE_BMON ) {
				if( !(pDBzm->zvol->sub_sw&0x02) ) {
					id = DATA_NONE;
				}
				
				pDisp[i].inpsize = nMax - 12;
				pDisp[i].dispsize = pDisp[i].inpsize;
			}
			if( id == DATA_DEB_KNO || id == DATA_CRE_KNO ) {
				if( pDBzm->zvol->apno == 0x20 ) {
					if( !(pDBzm->zvol->sub_sw&0x04) ) {
						id = DATA_NONE;	
					}
				}
				else {
					id = DATA_NONE;	
				}
			}
			if( id == DATA_DEBT || id == DATA_CRED ) {
				pDisp[i].inpsize	= nMax;
				pDisp[i].dispsize	= nMax;
			}
			if( id == DATA_SYONO ) {
//				if( pAUTOSEL->DSPTGNO_OPT == OPT_NODSP ) {
//					id = DATA_NONE;	
//				}
			}
			if( id == DATA_TGDATE ) {
//				if( pAUTOSEL->DSPTGDT_OPT == OPT_NODSP ) {
//					id = DATA_NONE;	
//				}
			}

			dtcol.ln_ofs	= pDisp[i].ln_ofs;
			dtcol.clm_title	= pDisp[i].title;
			dtcol.clm_id	= id;
			dtcol.clm_type	= pDisp[i].type;
			dtcol.clm_fmt	= pDisp[i].format;
			dtcol.clm_back	= pDisp[i].back;
			dtcol.clm_text	= pDisp[i].text;
			dtcol.clm_inpsz	= pDisp[i].inpsize;
			dtcol.clm_size	= pDisp[i].dispsize;
			dtcol.clm_szadj	= pDisp[i].size_adj;
			dtcol.clm_spos	= pDisp[i].spos;
			dtcol.clm_adj	= pDisp[i].spos_adj;
			dtcol.clm_sidx	= pDisp[i].clm_sidx;

			clm.SetAt( cnt, dtcol );
			cnt++;
		}
		m_List.SetItemColumn( &clm[0], cnt );
	}

}



void CRDSel::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	ICSDialog::OnCancel();
}

void CRDSel::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	ICSDialog::OnOK();
}


BOOL CRDSel::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	static int down_key = 0;
	// 全体 エスケープ END処理
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam ) {
		case VK_ESCAPE:
			if( m_RDLBL.GetDroppedState( ) )
				m_RDLBL.ShowDropDown( FALSE );
			EndDialog( IDCANCEL );
			return 1;

		case VK_END:
			if( m_RDLBL.GetDroppedState( ) )
				m_RDLBL.ShowDropDown( FALSE );
			GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
			return 1;
		}
	}

	// データ部ページ移動
	//
	if( pMsg->hwnd != m_RDLBL.m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			//if( is_SHIFT() )
			//{
			//	switch( pMsg->wParam )
			//	{
			//	case VK_DOWN:
			//		m_List.SendMessage( WM_VSCROLL, SB_LINEDOWN );
			//		return 1;
			//	case VK_UP:
			//		m_List.SendMessage( WM_VSCROLL, SB_LINEUP );
			//		return 1;
			//	case VK_LEFT:
			//		m_List.SendMessage( WM_HSCROLL, SB_PAGEUP );
			//		return 1;
			//	case VK_RIGHT:
			//		m_List.SendMessage( WM_HSCROLL, SB_PAGEDOWN );
			//		return 1;
			//	}
			//}
		}
	}

	// ラベル部
	if( pMsg->hwnd == m_RDLBL.m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			UINT key;
			key = pMsg->wParam;

			if( key == VK_TAB && is_SHIFT() ) 
				key = VK_F2;

			switch( key )
			{
			case VK_F2:
				if( m_RDLBL.GetDroppedState( ) )
					m_RDLBL.ShowDropDown( FALSE );
				GotoDlgCtrl( GetDlgItem( IDCANCEL) );
				return 1;
				break;
			case VK_TAB:
				if (IsMasterType( MST_SYZ8_10 )) {
					GotoDlgCtrl( &m_Zritu );
					return 1;
				}
			case VK_RETURN:
//			case VK_LEFT:
//			case VK_F3:
				FocusFncs( IDC_RDLIST, ACT_FOCUS_ON );
				return 1;
			case VK_DOWN:
				if( !m_RDLBL.GetDroppedState( ) )
					m_RDLBL.ShowDropDown( TRUE );
				break;
			case VK_END:
				down_key = pMsg->wParam;

				if( m_RDLBL.GetDroppedState( ) )
					m_RDLBL.ShowDropDown( FALSE );
				GetDlgItem( IDCANCEL )->PostMessage( WM_LBUTTONDOWN );
				return 1;
				break;
			default:
				break;
			}
		}
	}
	// データ部
	else if( pMsg->hwnd == m_List.m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			switch( pMsg->wParam )
			{
			case VK_END:
			case VK_F2:
				m_RDLBL.SetFocus();
				return 1;
			case VK_NEXT:
				m_List.SendMessage( WM_VSCROLL, SB_PAGEDOWN );
				return 1;
			case VK_PRIOR:
				m_List.SendMessage( WM_VSCROLL, SB_PAGEUP );
				return 1;
			}
		}
		else if( pMsg->message == WM_LBUTTONDOWN )
		{
		}
	}
	// return to ラベル部 check
	else if( pMsg->hwnd == GetDlgItem(IDC_BUTTON1)->m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			switch( pMsg->wParam )
			{
			case VK_END:
				m_RDLBL.SetFocus();
				return 1;
			case VK_RETURN:
				break;
			}
		}
	}
	// return to ラベル部 check
	else if( pMsg->hwnd == GetDlgItem(IDCANCEL)->m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			switch( pMsg->wParam )
			{
			case VK_LEFT: case VK_UP:
				GotoDlgCtrl(GetDlgItem(IDC_BUTTON1));
				return 1;

			case VK_RIGHT: case VK_DOWN:
				FocusFncs(IDC_RDLIST, ACT_FOCUS_ON);
				return 1;
			}
		}
	}

	// 確認 部
	if( pMsg->hwnd == ((CWnd*)GetDlgItem( IDC_BUTTON1 ))->m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			if( pMsg->wParam == VK_RETURN )
			{
				OnButton1();
				return 1;
			}
			else if( Kbt_ActionFncs( IDC_BUTTON1, ACT_KEYIN_ONLY, pMsg->wParam ) )
				return 1;
		}
		else if( pMsg->message == WM_KEYUP )
		{
		//	if( down_key == pMsg->wParam )
		//	{
		//		down_key = 0;
		//		GetDlgItem( IDC_BUTTON1 )->PostMessage( WM_LBUTTONUP );
		//		return 1;
		//	}
		}
	}
	// 全体
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_HOME:
			//if( m_RDLBL.GetDroppedState( ) )
			//	m_RDLBL.ShowDropDown( FALSE );
			//return 1;
			break;
		case VK_F12:
			if( m_RDLBL.GetDroppedState( ) )
				m_RDLBL.ShowDropDown( FALSE );

			AllSelJob();
			FocusFncs(IDC_RDLIST, ACT_FOCUS_ON);
			return 1;
			break;
		case VK_ESCAPE:
		case VK_INSERT:
		case VK_END:
			down_key = pMsg->wParam;

			if( m_RDLBL.GetDroppedState( ) )
				m_RDLBL.ShowDropDown( FALSE );
			if( pMsg->wParam != VK_ESCAPE ) {
				if( pMsg->hwnd == GetDlgItem( IDCANCEL )->m_hWnd )
					EndDialog( IDCANCEL );
				else
					GetDlgItem( IDCANCEL )->PostMessage( WM_LBUTTONDOWN );
				return 1;
			}
		}
	}
	if (pMsg->hwnd == m_Zritu.m_hWnd)
	{
		if (pMsg->message == WM_KEYDOWN) {
			UINT key = pMsg->wParam;
			if (key == VK_TAB && is_SHIFT())
				key = VK_F2;

			if (key == VK_RETURN || key == VK_TAB ) {
				NextDlgCtrl();
				return 1;
			}
			else if (key == VK_F2 ) {
				PrevDlgCtrl();
				return 1;
			}
			else if (key == VK_DOWN) {
				FocusFncs( IDC_RDLIST, ACT_FOCUS_ON );
				return 1;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}



void CRDSel::ACbookListColumn()
{
	DT_COLUMN	data[16];

	int total, width;
	int max, tksz, tkinp, idx;

	BOOL bBmon, bKoji, bZei;
	bBmon = bKoji = bZei = FALSE;

	if( BMON_MST ) {
		if( pAUTOSEL->DSPBMON_OPT == OPT_NONOPT)
			bBmon = TRUE;
	}
	if( M_KOJI ) {
		if( pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT)
			bKoji = TRUE;
	}

	if( pAUTOSEL->DSPZEI_OPT == OPT_NONOPT)
		bZei = TRUE;

	int komok_num = 5;
	
	if( bBmon )		komok_num++;
	if( bKoji )		komok_num++;
	if( bZei )		komok_num++;

	//工事幅
	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	int kjwd = 0;

	CFont* pFont, *pOld;
	pFont = m_List.GetFont();

	// 摘要幅を調整する
	CClientDC dc(&m_List);
	CSize sz, *scrn;
	pOld = dc.SelectObject( pFont );

	sz = dc.GetTextExtent( "A", 1 );

	dc.SelectObject( pOld );

	scrn = GetScreenSize();

	CRect rect;
	m_List.GetClientRect( rect );
	width = rect.Width();
	// (項目個数*調整ピクセル数)
	max = (width-(komok_num*4)) / sz.cx;

	total = 7+20+15+15+8; //番号＋科目＋入金＋出金＋消費税

	if( bBmon)		total += 4 /*Voln1->bmcol*/;
	if( bKoji ) {
		//工事幅保存分 調整する
		int kjcnt = 4;
//		if( pAUTOSEL->RESOL_OPT == scrX ) {
//			kjwd = ((CMainFrame*)GetDBDinpMainFrame())->AcbkKojiWd();
//		}
		if( kjwd > 0 ) {
			kjcnt = (kjwd / sz.cx);
		}

		total += kjcnt/*Voln1->kjcol*/;
	}
	if( bZei )		total += 13;

	tksz = max - total;
	if( tksz <= 0 )
		tksz = 2;

//CString msg;
//msg.Format( "w %d, tksz = %d", sz.cx, tksz );
//AfxMessageBox( msg );

	idx = 0;

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "番号";
	data[idx].clm_id = DATA_USER1;
	data[idx].clm_type = DT_KIND_STRING | DT_FMT_RIGHT;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 7;
	data[idx].clm_size = 7;
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = 0;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = 0;
	idx++;


	if( bBmon ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "部門";
		data[idx].clm_id = DATA_DEB_BMON;
		data[idx].clm_type = DT_FMT_LEFT;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = Voln1->bmcol;
		data[idx].clm_size = 4 /*Voln1->bmcol*/;
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}
	if( bKoji ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "工事";
		data[idx].clm_id = DATA_DEB_KNO;
		data[idx].clm_type = DT_FMT_LEFT;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = Voln1->kjcol;
		data[idx].clm_size = 4 /*Voln1->kjcol*/;
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "相 手 科 目";
	data[idx].clm_id = DATA_DEBT;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = 20;
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	// 摘要
	if( (pDBzm->zvol->tk_ln * 2) > tksz )
			tkinp = (pDBzm->zvol->tk_ln * 2);
	else	tkinp = tksz;
	data[idx].ln_ofs = 0;
	data[idx].clm_title = "";
	data[idx].clm_id = DATA_TEKIYO;
	data[idx].clm_type = DT_INP_BOTTOMLEFT;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz	= tkinp;
	data[idx].clm_size	= tksz;
	data[idx].clm_szadj 	= 0;
	data[idx].clm_spos 	= -1;
	data[idx].clm_adj 	= 0;
	data[idx].clm_sidx	= -1;
	idx++;

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "入　金";
	data[idx].clm_id = DATA_VAL;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = 15;
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "出　金";
	data[idx].clm_id = DATA_USER2;
	data[idx].clm_type = DT_KIND_VAL | DT_FMT_RIGHT;
	data[idx].clm_fmt = "sss,sss,sss,ss9";
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = 15;
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	if( bZei ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "税　額";
		data[idx].clm_id = DATA_ZEI;
		data[idx].clm_type = 0;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = 0;
		data[idx].clm_size = 13;
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "";
	data[idx].clm_id = DATA_SYZ;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	if( CheckWidthSyohi() ) data[idx].clm_size = 8+2;
	else					data[idx].clm_size = 8;

	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	m_List.SetItemColumn( &data[0], idx );

//	if( bOnSize ) {
//		for( int n = 0; n < idx; n++ ) {
//			m_List.SetListItemWidth( data[n].clm_id, 0 );
//		}
//	}

	// 工事幅 保存
	m_List.SetListItemWidth( DATA_DEB_KNO, kjwd );

	m_List.SetReadOnly(TRUE);
	m_List.EnforceLayout();

	DT_ITEMMAX im;
	im.im_seq = 8;
	im.im_denp = 7;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno= Voln1->snocol;

	m_List.SetImax( &im );
	m_List.RedrawDataListCtrl();

}

void CRDSel::ACBK_ListDataSet( int seq )
{
	CDataListCtrl* pLst;
	DTLIST_DATA	data;
	CDBINPDataRec drec;
	int dcsw;
	DBKNREC* pKn;
	BOOL	bNyuKin = FALSE;
	BOOL	bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	pLst = &m_List;

	int size = m_ACdata.GetCount();

	int ln = m_List.GetCurrentLine();
	m_List.RemoveAllDtListData();

	if( seq > size ) {
		m_List.RedrawDataListCtrl();
		return;
	}
	int lcnt = m_List.GetDispDataCount();

	int idx = seq -1;

	for( int i = 0; i < lcnt; i++, idx++ ) {
		bNyuKin = FALSE;
		if( idx >= size )	break;

		drec = m_ACdata[idx];

		DB_SyIniz( &drec );
		data.dd_syz_str = sy_line_txt( &drec/*, NULL, INP_mode*/ );

		data.dd_seq = i+1+m_TopIndex; /*drec.m_seq;*/

		data.dd_ddate = drec.m_ddate;
		data.dd_ksign = drec.m_ksign;
		data.dd_mofs = drec.m_mofs;

		dcsw = ((CDBAbookView*)m_pParent)->GetRecordDCsgn( &drec );

		// 基本科目が 借方か、借方／貸方 同じ場合
		if( dcsw == 0 || dcsw == -1 ) {
			bNyuKin = TRUE;
			pKn = pDBzm->DB_PjisToKnrec( drec.m_cre );

			if( pKn ) {
				KamokuString( pKn, data.dd_deb_name );
			}
			else {
				data.dd_deb_name.Empty();
			}
			data.dd_dbr = drec.m_cbr;

			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn	= drec.m_cbmn;
			sk.brn	= drec.m_cbr;
			sk.code	= drec.m_cre;

			if( _set_kmketc( &sk, TRUE ) ) {
				data.dd_deb_etc = sk.etc;
				data.dd_ctbl[DATA_DEBT].text = sk.etc_col;
			}
			else {
				data.dd_deb_etc.Empty();
				data.dd_ctbl[DATA_DEBT].text = (DWORD)-1;
			}

			// 部門
			data.dd_dbmn = drec.m_cbmn;
			data.dd_cbmn = data.dd_dbmn;

			sk.bmn	= drec.m_cbmn;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno.Empty();

			if( bDisp && _set_bmnetc( &sk ) ) {
				data.dd_dbmn_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_BMON].text = sk.etc_col;
			}
			else {
				data.dd_dbmn_etc.Empty();
				data.dd_ctbl[DATA_DEB_BMON].text = (DWORD)-1;
			}

			// 工事
			data.dd_dkno = drec.m_ckno;
			data.dd_ckno = data.dd_dkno;

			sk.bmn	= -1;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno	= drec.m_ckno;
			int bflen = 128;
			int len = sk.kno.GetLength();
			if (len > 0) {
				LPSTR p = sk.kno.GetBuffer(bflen);
				trim_zero(p, bflen, len);
				sk.kno.ReleaseBuffer();
			}

			if( bDisp && _set_kojietc( &sk ) ) {
				data.dd_dkno_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_KNO].text = sk.etc_col;
			}
			else {
				data.dd_dkno_etc.Empty();
				data.dd_ctbl[DATA_DEB_KNO].text = (DWORD)-1;
			}
		}
		else {
			pKn = pDBzm->DB_PjisToKnrec( drec.m_dbt );

			if( pKn ) {
				KamokuString( pKn, data.dd_deb_name );
			}
			else	data.dd_deb_name.Empty();
			data.dd_dbr = drec.m_dbr;

			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn	= drec.m_dbmn;
			sk.brn	= drec.m_dbr;
			sk.code	= drec.m_dbt;

			if( _set_kmketc( &sk, TRUE ) ) {
				data.dd_deb_etc = sk.etc;
				data.dd_ctbl[DATA_DEBT].text = sk.etc_col;
			}
			else {
				data.dd_deb_etc.Empty();
				data.dd_ctbl[DATA_DEBT].text = (DWORD)-1;
			}

			// 部門
			data.dd_dbmn = drec.m_dbmn;
			data.dd_cbmn = data.dd_dbmn;

			sk.bmn	= drec.m_dbmn;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno.Empty();

			if( bDisp && _set_bmnetc( &sk ) ) {
				data.dd_dbmn_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_BMON].text = sk.etc_col;
			}
			else {
				data.dd_dbmn_etc.Empty();
				data.dd_ctbl[DATA_DEB_BMON].text = (DWORD)-1;
			}

			// 工事
			data.dd_dkno = drec.m_dkno;
			data.dd_ckno = data.dd_dkno;

			sk.bmn	= -1;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno	= drec.m_dkno;
			int bflen = 128;
			int len = sk.kno.GetLength();
			if (len > 0) {
				LPSTR p = sk.kno.GetBuffer(bflen);
				trim_zero(p, bflen, len);
				sk.kno.ReleaseBuffer();
			}

			if( bDisp && _set_kojietc( &sk ) ) {
				data.dd_dkno_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_KNO].text = sk.etc_col;
			}
			else {
				data.dd_dkno_etc.Empty();
				data.dd_ctbl[DATA_DEB_KNO].text = (DWORD)-1;
			}
		}

		BYTE kin[6];

		::ZeroMemory( data.dd_udtbl, sizeof data.dd_udtbl );
		::ZeroMemory( data.dd_val, sizeof data.dd_val );
		data.val_flag = 0;

		//番号
		int dt_idx = i + m_TopIndex;
		CString cstr;
		if( m_Kbsgn[dt_idx] ) {
			cstr.Format("○%3d", data.dd_seq);
		}
		else {
			cstr.Format("  %3d", data.dd_seq);
		}
		data.dd_udtbl[0].userdata = (BYTE*)(LPCTSTR)cstr;

		// 追加行は 金額欄を空白で表示
		if( bNyuKin ) {
			memcpy( data.dd_val, drec.m_val, 6 );
			data.val_flag = 1;
		}
		else {
			memcpy( kin, drec.m_val, 6 );
			data.dd_udtbl[1].userdata = kin;
			data.dd_udtbl[1].initflag = 1;
		}

		// 金額・税額を空白
		if( l_test( drec.m_val ) == 0 ) {
			data.val_flag = 0;
			data.zei_flag = 0;
			data.dd_udtbl[1].initflag = 0;
		}

		memcpy( data.dd_zei, drec.m_zei, 6 );
		if( is_syohizeisiwake( &drec ) ) {
			data.zei_flag = 1;
			/* 非課税有価証券譲渡仕訳 */
			if( pDBsy->IsTaikaData( pDBzm->dbdata )==2 )
				data.zei_flag |= 0x10;
		}
		else {
			// 消費税税仕訳でない
			data.zei_flag = 0;
		}

		data.dd_tek.tekiyo = drec.m_tekiyo;

		data.dd_tek.width	= 0;
		data.dd_tek.height	= 0;
		data.dd_tek.image	= NULL;

		// 変動事由 or 振替区分[公益]
		data.dd_hjiyu.Empty();
		data.dd_dsign = 0;

		data.dd_tdate = -1;
		data.dd_husen = 0;

		COLORREF back_col = GetRdLineBackColor(i);

		for (int k = 0; k < DATA_CNT; k++) {
			data.dd_ctbl[k].back = back_col;
		//	data.dd_ctbl[k].text = (DWORD)-1;
		}

		m_List.SetDtListData( i, &data, FALSE );
	}

	if( ln != -1 ) 
		m_List.SetCurrentLine( ln );

	m_List.RedrawDataListCtrl();
}


// 定型仕訳レコード を返送
int CRDSel::RecordDataCount()
{
	int cnt;

	if( ! m_Type ) {
		cnt = Mkrd.data->GetRecordCount();
	}
	else {
		cnt = m_ACdata.GetCount();
	}

	return cnt;
}


// staitc window 移動
void CRDSel::StaticWndMove()
{
	int nID[] = {
		IDC_ST_LINE, 
		IDC_ST_TLBMON,IDC_ST_BUMON,IDC_ST_TLKOJI,IDC_ST_KOJI, IDC_ST_TLKMK,
		IDC_ST_KMKNAM,IDC_ST_TLBRN,IDC_ST_BRN, 0
	};

	if (IsMasterType( MST_SYZ8_10 )) {
		GetDlgItem( IDC_CHK_ZEIRITSU )->ShowWindow( SW_SHOW );
	}
	else {
		GetDlgItem( IDC_CHK_ZEIRITSU )->ShowWindow( SW_HIDE );
		CRect base, rc;
		GetDlgItem(IDC_CHK_ZEIRITSU)->GetWindowRect(&base);
		ScreenToClient(&base);
	}

	//通常形式
	if( ! m_Type ) {
		for( int i = 0; nID[i] != 0; i++ ) {
			GetDlgItem( nID[i] )->ShowWindow( SW_HIDE );
		}
		return;
	}

	
	CRect rect, rc;
	int left, top, wd, height, ListTop;
	int kjsgn = 0;

	if( pDBzm->zvol->apno == 0x20 ) {
		if( (pDBzm->zvol->sub_sw&0x04) ) kjsgn = 1;
	}

	// 部門なし
	if( !(pDBzm->zvol->sub_sw&0x02) ) {
		GetDlgItem( IDC_ST_TLBMON )->ShowWindow(SW_HIDE);
		GetDlgItem( IDC_ST_BUMON )->ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem( IDC_ST_TLBMON )->ShowWindow(SW_SHOW);
		GetDlgItem( IDC_ST_BUMON )->ShowWindow(SW_SHOW);
	}
	// 工事なし
	if( ! kjsgn ) {
		GetDlgItem( IDC_ST_TLKOJI )->ShowWindow(SW_HIDE);
		GetDlgItem( IDC_ST_KOJI )->ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem( IDC_ST_TLKOJI )->ShowWindow(SW_SHOW);
		GetDlgItem( IDC_ST_KOJI )->ShowWindow(SW_SHOW);
	}

	if( ! BRmst ) {
		GetDlgItem(IDC_ST_TLBRN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ST_BRN)->ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem(IDC_ST_TLBRN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ST_BRN)->ShowWindow(SW_SHOW);
	}

	// 開始位置
	if (IsMasterType( MST_SYZ8_10 )) {
		GetDlgItem( IDC_CHK_ZEIRITSU )->GetWindowRect( rect );
	}
	else {
		GetDlgItem( IDC_BUTTON_ALL )->GetWindowRect( rect );
	}
	ScreenToClient( rect );

	left	= rect.left;
	top		= rect.bottom + 9;

	CWnd* pwnd;

	ListTop = 0;

	for( int i = 1; nID[i] != 0; i++ ) {
		switch(i) {
		case 1:
		case 2:
			if( !(pDBzm->zvol->sub_sw&0x02) )	continue;
			break;
		case 3:
		case 4:
			if( ! kjsgn )	continue;
			break;
		case 5:		//相手科目
		case 6:
			break;
		case 7:		//枝番
		case 8:		
			if( !BRmst )	continue;
			if( !BRmst )	continue;
			break;
		default:
			pwnd = NULL;
			break;
		}

		pwnd = GetDlgItem( nID[i] );
		if( ! pwnd )	break;

		pwnd->GetWindowRect( rc );
		ScreenToClient( rc );
		wd		= rc.Width();
		height	= rc.Height();

		rc.top		= top;
		rc.bottom	= rc.top + height;
		rc.left = left;
		rc.right = rc.left + wd;

		pwnd->MoveWindow( rc );
		pwnd->Invalidate();
		pwnd->UpdateWindow();

		left = rc.right;

		if( ! ListTop )	ListTop = rc.bottom + 4;
	}

	CRect dlgRect;
	GetClientRect( dlgRect );
	m_List.GetWindowRect( rect );
	ScreenToClient( rect );

	int htList = rect.Height();

	if( ( ListTop + htList ) > dlgRect.Height() ) {
		htList = dlgRect.Height() - ListTop;
	}

	m_List.MoveWindow( rect.left, ListTop, rect.Width(), htList );

	// 線のスタティック
	top		-= 4;

	GetDlgItem(nID[0])->GetWindowRect( rc );
	GetDlgItem(nID[0])->MoveWindow(rect.left, top, rect.Width(), 1 );
}


void CRDSel::OnBnClickedChkZeiritsu()
{
	int st = m_Zritu.GetCheck();
	int chk_val = 1;

	if (st == BST_UNCHECKED)
		chk_val = 0;

	RDsiwakeZeirituMode() = chk_val;
}

LRESULT CRDSel::OnSelChgMsg(WPARAM wp, LPARAM lp)
{
	if( wp == WM_KEYDOWN ) {
		if( lp == VK_RETURN ) {
			FocusFncs(IDC_RDLIST, ACT_FOCUS_ON);
		}
	}
	else {
		OnSelchangeCombo1();
	}
	return 1;
}


//全選択・全解除
void CRDSel::OnButtonAll()
{
	AllSelJob();
	FocusFncs(IDC_RDLIST, ACT_FOCUS_ON);
}

//---------------------------------------------------------------------------------------
// 定型仕訳 選択処理(Shift)
//	int  curIndex	現在行インデックス( -1: KeyDown[1回], -2 KeyDown(キー押しっぱなし)
//									    >= 0: KeyUp)
//	long nChar		キーコード
//	BOOL bPress		TRUE:Shift キー押下中
//
//---------------------------------------------------------------------------------------
void CRDSel::sel_shiftjob(int curIndex, long nChar, BOOL bPress)
{
	// SHIFT 押下し
	if( bPress ) {
		int index;
		if( curIndex <= -1 )	index = m_CurIndex + m_TopIndex;
		else {
			index = curIndex + m_TopIndex;
		}

		int oldsgn = m_Kbsgn[index];

#ifdef DBG_TRACE
		char* p1 = "VK_UP";
		char* p2 = "VK_DOWN";
		char* cpy1 = "SELCPY_UP";
		char* cpy2 = "SELCPY_DOWN";
MyTrace("curIndex = %d, index = %d, oldsgn(%d), nChar = %s\n", curIndex, index, oldsgn, (nChar == VK_UP) ? p1 : p2 );
MyTrace("m_SelIndex = %d, m_SelSign = %d, m_SelSign2 = %d, Direc = %s\n", m_SelIndex, m_SelSign, m_SelSign2, 
		m_SelDirection == 0 ? "SELCPY_NONE" : m_SelDirection == SELCPY_UP ? cpy1 : cpy2 );
#endif
		// KEYUP
		if( curIndex >= 0 ) {
			if( m_SelDirection == SELCPY_UP ) {
				//当初の↑方向から↓になった
				if( (nChar != VK_UP) ) {
					if( m_SelIndex != -1 ) {
						if( index == m_SelIndex ) {
							m_Kbsgn[index] = m_SelSign;
						}
						else if( index > m_SelIndex ) {
							int nochg = 0;
							int dcnt = RecordDataCount();
							if( index == (dcnt-1) ) {
								if( m_Kbsgn[index] )//○の場合はOFFにしない
									nochg++;
							}
							if( !nochg ) {
								//基準行より下側
								m_Kbsgn[index] = oldsgn ? 0 : 1;
							}
						}
						else {
						}
						m_SelUpdate = TRUE;
					}
					return;
				}
			}
			else {
				//当初の↓方向から↑になった
				if( (nChar != VK_DOWN) ) {
					if( m_SelIndex != -1 ) {
						if( index == m_SelIndex ) {
							m_Kbsgn[index] = m_SelSign;
						}
						if( index < m_SelIndex ) {
							int nochg = 0;
							if( index == 0 ) {
								if( m_Kbsgn[index] )//○の場合はOFFにしない
									nochg++;
							}
							if( !nochg ) {
								//基準行より上側
								m_Kbsgn[index] = oldsgn ? 0 : 1;
							}
						}
						else {
						}
						m_SelUpdate = TRUE;
					}
					return;
				}
			}
			//○の場合はOFFにしない
			if( m_Kbsgn[index] )
				return;
		}
		if( index >= 0 ) {

			m_SelType = SEL_SHIFT;
			if( m_SelIndex == -1 ) {
				m_SelIndex = index;

				m_SelSign = oldsgn ? 0 : 1;
				m_Kbsgn[m_SelIndex] = m_SelSign;
				m_SelDirection = (nChar == VK_UP) ? SELCPY_UP : SELCPY_DOWN;
			}
			else {
				//click 後の 矢印移動
				if( m_SelDirection == SELCPY_NONE ) {
					m_SelDirection = (nChar == VK_UP) ? SELCPY_UP : SELCPY_DOWN;
				}

				if( m_SelDirection == SELCPY_UP ) {
					if( (nChar == VK_UP) ) {
						m_Kbsgn[index] = m_SelSign;
						if( index > m_SelIndex ) {
							if( curIndex <= -1 ) {
								m_Kbsgn[index] = oldsgn ? 0 : 1;
							}
						}
					}
					else {
						// 選択方向が変わった。その行のサインを設定
						if( m_SelSign2 == -1 ) {
							m_SelSign2 = oldsgn ? 0 : 1;
						}

						if( index == m_SelIndex ) {
							m_Kbsgn[index] = m_SelSign;
						}
						else if( index > m_SelIndex ) {
							if( curIndex == -2 ) {
								m_Kbsgn[index] = oldsgn ? 0 : 1;
							}
						}
						else {
							m_Kbsgn[index] = m_SelSign2;
						}
					}
				}
				else {
					if( (nChar == VK_DOWN) ) {
						m_Kbsgn[index] = m_SelSign;
						if( index < m_SelIndex ) {
							if( curIndex <= -1 ) {
								m_Kbsgn[index] = oldsgn ? 0 : 1;
							}
						}
					}
					else {
						// 選択方向が変わった。その行のサインを設定
						if( m_SelSign2 == -1 ) {
							m_SelSign2 = oldsgn ? 0 : 1;
						}
						if( index == m_SelIndex ) {
							m_Kbsgn[index] = m_SelSign;
						}
						else if( index < m_SelIndex ) {
							if( curIndex == -2 ) {
								m_Kbsgn[index] = oldsgn ? 0 : 1;
							}
						}
						else {
							m_Kbsgn[index] = m_SelSign2;
						}
					}
				}
			}
//MyTrace("---> m_Kbsgn[index] = %d, oldsgn = %d\n", m_Kbsgn[index], oldsgn );

			m_SelUpdate = TRUE;
		}
	}
	else {
		// SHIFT 離した。
	//	if( m_SelDirection != SELCPY_NONE ) {
		//	::ZeroMemory(m_Kbsgn, sizeof m_Kbsgn);
			selection_iniz();
	//	}
	}
}


/***********************************************
	int line	クリックして変更したライン


 ***********************************************/
void CRDSel::click_seljob(int line, long nChar)
{
	int sgn;
	int ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];

	int curidx = m_CurIndex;
	int dt_idx = -1;	//直前行のデータインデックス
	BOOL curAdd = FALSE;

	//クリックした行のデータインデックス
	int now_idx = line - 1 + m_TopIndex;

	if( (nChar == VK_SHIFT || nChar == VK_CONTROL) && curidx >= 0 ) {

		//現在行も選択範囲に入れる。
		if( curidx >= 0 ) {
		//	dt_idx = curidx + m_TopIndex;
			dt_idx = m_CurDataIdx;
			if( m_SelIndex == -1 ) {
				m_SelIndex = dt_idx;
				m_Kbsgn[dt_idx] = 1;
				m_SelUpdate = TRUE;
				m_SelDirection = SELCPY_UP;
			}
		}

		if( nChar == VK_SHIFT ) {
			if( now_idx < dt_idx ) {
				m_SelDirection = SELCPY_UP;
				for( int n = now_idx; n < ar_max; n++ ) {
					if( n >= dt_idx + 1 ) {
						m_Kbsgn[n] = 0;
					}
					else {
						m_Kbsgn[n] = 1;
					}
				}
			}
			else {
				m_SelDirection = SELCPY_DOWN;
				for( int n = 0; n < ar_max; n++ ) {
					if( n < dt_idx ) {
						m_Kbsgn[n] = 0;
					}
					else if( n <= now_idx ) {
						m_Kbsgn[n] = 1;
					}
					else {
						m_Kbsgn[n] = 0;
					}
				}
			}
			m_SelUpdate = TRUE;
		}
		else { //Ctrl + クリック
			sgn = m_Kbsgn[now_idx];
			m_Kbsgn[now_idx] = !sgn;
			m_SelUpdate = TRUE;
		}
	}
	else {
		//SHIFT, CTRL が押されていないただのクリック選択として扱う
		m_SelIndex = now_idx;

		sgn = m_Kbsgn[now_idx];
		m_Kbsgn[now_idx] = !sgn;
		m_SelUpdate = TRUE;

#ifdef CLOSE
		// SHIFT, CTRL 離した。
		if( m_SelDirection != SELCPY_NONE ) {
			::ZeroMemory(m_Kbsgn, sizeof m_Kbsgn);
			selection_iniz();
		}
#endif
	}

}


// 選択ライン再描画
void CRDSel::sel_lineupdate()
{
	if( m_SelUpdate ) {
		int curidx = m_List.GetCurrentLine() - 1;
		int dt_idx = curidx + m_TopIndex;

		int disp_cnt = m_List.GetDispDataCount();
		int max_count = RecordDataCount();
		int ar_max;
		ar_max = sizeof m_Kbsgn / sizeof m_Kbsgn[0];
		ASSERT(dt_idx < ar_max);

		int drawcnt;
		if( max_count < disp_cnt ) {
			drawcnt = max_count;
		}
		else {
			drawcnt = max_count;
		}

		DTLIST_DATA dt;
		int list_idx;

		for( list_idx = 0; list_idx < drawcnt; list_idx++ ) {
			dt_idx = list_idx + m_TopIndex;

			COLORREF back_col;

			m_List.GetDtListData(list_idx, &dt);
			CString cstr;

			if( m_Kbsgn[dt_idx] )
			{
				cstr.Format("○%3d", dt.dd_seq);
				back_col = SELECT_BKCOL;
			}
			else
			{
				cstr.Format("  %3d", dt.dd_seq);
				back_col = NORMAL_BKCOL;
			}
			dt.dd_udtbl[0].userdata = (BYTE*)(LPCTSTR)cstr;
			// 背景色セット
			for( int k = 0; k < DATA_CNT; k++ ) {
				dt.dd_ctbl[k].back = back_col;
			}

			m_List.SetDtListData(list_idx, &dt, FALSE);
		}
		m_List.RedrawDataListCtrl();
		// 表示
		Kbn_Rngdsp();
	}

	m_SelUpdate = FALSE;
}


//行選択の変数を初期化
void CRDSel::selection_iniz()
{
	m_SelDirection = SELCPY_NONE;
	m_SelType = SEL_NONE;
	m_SelIndex = -1;
	m_SelUpdate = TRUE;
	m_SelSign = m_SelSign2 = -1;
}
