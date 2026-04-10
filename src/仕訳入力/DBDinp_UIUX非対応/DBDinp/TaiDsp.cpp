// TaiTtl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "icsdisp.h"

#include "MstRw.h"
#include "TaiDsp.h"

#include "icslistinp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CBrnTky		m_BrTek;
extern CDipZmSub*	pDBzm;

/////////////////////////////////////////////////////////////////////////////
// CTaiDsp ダイアログ


CTaiDsp::CTaiDsp(CWnd* pParent /*=NULL*/)
	: ICSDialog(CTaiDsp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiDsp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_Active = FALSE;
	m_pTaiData = 0;
	m_IDALT = 0;
	m_pAltTxt = 0;
	m_pKmkDspData = 0;
}


void CTaiDsp::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiDsp)
	DDX_Control(pDX, IDC_KM_COMBO, m_KmCbo);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_ALT, m_Btn1);
	DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiDsp, ICSDialog)
	ON_MESSAGE(WM_TAIDSP, TaiDSP)
	//{{AFX_MSG_MAP(CTaiDsp)
	ON_BN_CLICKED(IDC_ALT, OnAlt)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_KM_COMBO, OnSelchangeKmCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiDsp メッセージ ハンドラ

//
// 貸借合計表示（メッセージ駆動）
LRESULT CTaiDsp::TaiDSP( WPARAM wParam, LPARAM lParam)
{
//MyTrace( "TaiDSP top wParam = %d, lParam = %d\n", wParam, lParam );

	int	l_r;
	char work[3][10];
	STaiData *tdata = (STaiData*)lParam;

	CWnd	*pNORMAL_DEB = GetDlgItem(IDC_NORMALTOTAL_DEB),	// 通常借方合計
			*pNORMAL_CRE = GetDlgItem(IDC_NORMALTOTAL_CRE),	// 通常貸方合計
			*pSYOGT_DEB = GetDlgItem(IDC_SYOGTOTAL_DEB),	// 諸口借方合計
			*pSYOGT_CRE = GetDlgItem(IDC_SYOGTOTAL_CRE),	// 諸口貸方合計
			*pSYOG_SAGK = GetDlgItem(IDC_SYOG_SAGAK),		// 諸口差額
			*pBS_DEB = GetDlgItem(IDC_BSTOTAL_DEB),			// 貸借借方合計
			*pBS_CRE = GetDlgItem(IDC_BSTOTAL_CRE),			// 貸借貸方合計
	//		*pSYOGBS = GetDlgItem(IDC_SYOGBS),				// 諸口貸借

			*pBRNUM = GetDlgItem(IDC_BRNUM),				// 枝番件数
			*pDATANUM = GetDlgItem(IDC_DATANUM);			// 仕訳件数

	// 借方 貸方表示用
	m_pKmkDspData = tdata;

	char txt[128] = {0};
	char tmp[128] = {0};

	// 元文字色セーブ
	// 元文字色セーブ
	static OLE_COLOR sv_fore1 = ((CICSDisp*)pNORMAL_DEB)->GetForeColor();
	static OLE_COLOR sv_fore2 = ((CICSDisp*)pNORMAL_CRE)->GetForeColor();
	static OLE_COLOR sv_fore3 = ((CICSDisp*)pSYOGT_DEB )->GetForeColor();
	static OLE_COLOR sv_fore4 = ((CICSDisp*)pSYOGT_CRE )->GetForeColor();
	static OLE_COLOR sv_fore5 = ((CICSDisp*)pSYOG_SAGK )->GetForeColor(); 
	static OLE_COLOR sv_fore6 = ((CICSDisp*)pBS_DEB )->GetForeColor();
	static OLE_COLOR sv_fore7 = ((CICSDisp*)pBS_CRE )->GetForeColor();

	l_r = l_retn();
	l_defn( _BIN );

	if( tdata->clq_sw == TAICLQ_NON )
	{
		pNORMAL_DEB->SetWindowText( "" );
		pNORMAL_CRE->SetWindowText( "" );
		pSYOGT_DEB->SetWindowText( "" );
		pSYOGT_CRE->SetWindowText( "" );
		pSYOG_SAGK->SetWindowText( "" );
		pBS_DEB->SetWindowText( "" );
		pBS_CRE->SetWindowText( "" );

		pBRNUM->SetWindowText( "件 " );		// 枝番件数
		pDATANUM->SetWindowText( "件 " );	// 仕訳件数
	}
	else
	{
		// 通常
		val_disp( pNORMAL_DEB, (char*)tdata->comm_deb, sv_fore1 );
		val_disp( pNORMAL_CRE, (char*)tdata->comm_cre, sv_fore2 );

		// 諸口
//		l_add( work[0], tdata->syog_ksi, tdata->syog_kri );	// work[0] = 借貸諸口合計
//		val_disp( pSYOGTOTAL, work[0], sv_fore2 );
		val_disp( pSYOGT_DEB, (char*)tdata->syog_deb, sv_fore3 );
		val_disp( pSYOGT_CRE, (char*)tdata->syog_cre, sv_fore4 );
		// 諸口差額
		l_sub( work[0], tdata->syog_deb, tdata->syog_cre );
		val_disp( pSYOG_SAGK, work[0], sv_fore5 );

		// 貸借
		l_clear( work[1] );
		l_clear( work[2] );
#ifdef CLSOE
		l_input( work[2], "2" );
		l_div( work[1], work[0], work[2] ); 
		l_add( work[1], work[1], tdata->comm_ttl );	// work[1] = 借貸諸口合計 / 2 + 通常合計
#endif
		l_add( work[1], tdata->comm_deb, tdata->syog_deb );
		l_add( work[2], tdata->comm_cre, tdata->syog_cre );
		val_disp( pBS_DEB, work[1], sv_fore6 );
		val_disp( pBS_CRE, work[2], sv_fore7 );

		// 諸口貸借
//		l_sub( work[0], tdata->syog_ksi, tdata->syog_kri );	//
//		val_disp( pSYOGBS, work[0], sv_fore4 );

		// 枝番件数
		sprintf_s( tmp, sizeof tmp, "%5d", tdata->ezdt_cnt );
		l_input( work[0], tmp );
		sprintf_s( txt, sizeof txt, "%s 件 ", val_txt( tmp, sizeof tmp, work[0]) );
		pBRNUM->SetWindowText( txt );

		// 仕訳件数
		sprintf_s( tmp, sizeof tmp, "%5d", tdata->data_cnt ); 
		l_input( work[0], tmp );
		sprintf_s( txt, sizeof txt, "%s 件 ", val_txt( tmp, sizeof tmp, work[0]) );
		pDATANUM->SetWindowText( txt );

	}

	if( tdata->clq_sw == TAICLQ_END )
	{
		GetDlgItem( IDC_WANING )->ShowWindow( SW_HIDE );
		// コンボボックスに科目をセット
		CComboBox* pCb;
		pCb = ((CComboBox*)GetDlgItem(IDC_KM_COMBO));
		pCb->ResetContent();
		DBKNREC* pKn;
		STaiKmkData* pKmd;
		pKmd = tdata->pkm_ttl;
		for( int i = 0; i < tdata->km_cnt && (pKmd+i)->kmcod[0]; i++ ) {
			pKn = pjs_knrec( (pKmd+i)->kmcod );
			if( pKn != NULL ) {
				sprintf_s( tmp, sizeof tmp, "%.14s", pKn->knnam );
				pCb->AddString( tmp );
			}
		}
		pCb->SetCurSel(0);
	}
	// 借方 貸方表示
	Kari_Kasi_Dsp( tdata, 0 );
	brn_disp( tdata, 0 );

	l_defn( l_r );

	this->UpdateWindow();

//MyTrace( "TaiDSP end\n" );

	return 1;
}


// 借方 貸方表示
void CTaiDsp::Kari_Kasi_Dsp( STaiData *tdata, int index )
{
	CWnd *pKARI = GetDlgItem(IDC_KARITOTAL),				// 借方
		 *pKASI = GetDlgItem(IDC_KASITOTAL);				// 貸方
	static OLE_COLOR sv_fore = ((CICSDisp*)pKARI)->GetForeColor();

	if( tdata->clq_sw != TAICLQ_END ) {
		pKARI->SetWindowText( "" );		// 借方
		pKASI->SetWindowText( "" );		// 貸方
	}
	else {	
		STaiKmkData* pTdt;
		pTdt = (tdata->pkm_ttl+index);

		val_disp( pKARI, (char*)pTdt->kmk_kri, sv_fore );	// 借方
		val_disp( pKASI, (char*)pTdt->kmk_ksi, sv_fore );
	}	
}


int CTaiDsp::DoModal() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	return ICSDialog::DoModal();
}

void CTaiDsp::set_alt( int alt_msg, char *alt_txt)
{
	m_IDALT = alt_msg;
	m_pAltTxt = alt_txt;
}

void CTaiDsp::dsp_data( STaiData *tdata )
{
	if( !m_Active )
		m_pTaiData = tdata;
	else
		PostMessage( WM_TAIDSP, 0, (LPARAM)tdata );
}

BOOL CTaiDsp::OnInitDialog() 
{
	CWnd	*pALT_BUTTON = GetDlgItem( IDC_ALT );

	MakeEdaListInp();
//	MakeTkrenFrm();

	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	kakudai(this);
	
	if( m_pAltTxt )
	{
		pALT_BUTTON->SetWindowText( m_pAltTxt );
		pALT_BUTTON->ShowWindow( SW_SHOW );
	}
	else
	{
		pALT_BUTTON->ShowWindow( SW_HIDE );
	}

	m_Active = TRUE;
	if( m_pTaiData )
	{
		dsp_data( m_pTaiData );
		m_pTaiData = 0;
	}

	if( m_Mode == MODE_NORMAL )
		GetDlgItem( IDOK )->SetWindowText( "閉じる" );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*****************************************************
	現在のビュー拡大比率に従って表示サイズを変える
******************************************************/
//void CTaiDsp::kakudai(CWnd* wnd)
//{
//	RECT	newrect;
//	double rate_w, rate_h;
//
//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
//	wnd->GetWindowRect( &newrect );
//	newrect.bottom = newrect.top + ((newrect.bottom - newrect.top ) * rate_h);
//	newrect.right = newrect.left + ((newrect.right - newrect.left ) * rate_w);
//
//	wnd->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOZORDER|SWP_NOMOVE );
//}

void CTaiDsp::OnDestroy() 
{
	ICSDialog::OnDestroy();

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	m_Active = FALSE;
}

void CTaiDsp::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	ICSDialog::OnOK();
}

void CTaiDsp::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	ICSDialog::OnCancel();
}

void CTaiDsp::OnAlt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	EndDialog( m_IDALT );
}


BOOL CTaiDsp::PreTranslateMessage(struct tagMSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 )
	{
		GetDlgItem( IDC_ALT )->PostMessage( WM_LBUTTONDOWN, 0, 0 );
		GetDlgItem( IDC_ALT )->PostMessage( WM_LBUTTONUP, 0, 0 );
		return 1;
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// mode は "TaiData.h" の MODE_XXXX
//
void CTaiDsp::tai_dsp( int mode )
{
	m_Mode = mode;
}


void CTaiDsp::OnSelchangeKmCombo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int index = ((CComboBox*)GetDlgItem(IDC_KM_COMBO))->GetCurSel();

ASSERT( m_pKmkDspData != NULL );
ASSERT( index >= 0 );

	Kari_Kasi_Dsp( m_pKmkDspData, index );

	brn_disp( m_pKmkDspData, index );
}


//リスト入力設定
void CTaiDsp::MakeEdaListInp()
{
	LINP_COLUMN LC[8];
	LINP_VCOLORINFO	LVCOL[8];
	int i;

	// 枝番
	LC[0].clm_kind = LINP_KIND_STRING;
	LC[0].clm_max = 16;
	LC[0].clm_attr = LINP_ATTR_CENTER| LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	LC[0].clm_format = "";
	LC[0].clm_width = 116;
	LC[0].clm_title = "枝番";
	LC[0].clm_tbcolor = -1;
	LC[0].clm_ttcolor = -1;

	//開始残高
	LC[1].clm_kind = LINP_KIND_KINGAKU;
	LC[1].clm_max = 0;
	LC[1].clm_attr = LINP_ATTR_BOTTOM | LINP_ATTR_RIGHT | LINP_ATTR_READONLY;;
	LC[1].clm_width = 72;

	LC[1].clm_title = "借方金額";
	LC[1].clm_format = "S,SSS,SSS,SSS,SS9";
	LC[1].clm_tbcolor = -1;
	LC[1].clm_ttcolor = -1;

	//貸方金額
	LC[2].clm_kind = LINP_KIND_KINGAKU;
	LC[2].clm_max = 0;
	LC[2].clm_attr = LINP_ATTR_BOTTOM | LINP_ATTR_RIGHT | LINP_ATTR_READONLY;;
	LC[2].clm_width = 72;

	LC[2].clm_title = "貸方金額";
	LC[2].clm_format = "S,SSS,SSS,SSS,SS9";
	LC[2].clm_tbcolor = -1;
	LC[2].clm_ttcolor = -1;

	linp_setclm( IDC_ICSLISTINPCTRL1, 3, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
	linp_horzgrid( IDC_ICSLISTINPCTRL1, 5, RGB_GRAY, LINP_GRID_SOLID, CICSListInp );

	for( i=0; i < 3; i++ ) {
		LVCOL[i].pos = i;
		LVCOL[i].color = RGB_RED;
		LVCOL[i].type = LINP_GRID_SOLID;
	}

	linp_vartgrid( IDC_ICSLISTINPCTRL1, 3, (LPUNKNOWN)LVCOL, CICSListInp );
}



//枝番表示
void CTaiDsp::brn_disp( STaiData *tdata, int index )
{
	STaiKmkData* pTdt;
	pTdt = (tdata->pkm_ttl+index);

	m_List.RemoveAll();

	int cnt = tdata->tbary.GetCount();
	int i, add;

	BRN_REC	brec;
	char	edbf[32], bf[64] = {0};

	LINP_DATA	LD[6];
	COLORREF	ONCOLOR, OFFCOLOR;
	ONCOLOR		= RGB_BLACK;
	OFFCOLOR	= RGB_PAPER_WHITE;

	for( i = add = 0; i < cnt; i++ ) {
		if( strcmp( tdata->tbary[i].kmk_cd, pTdt->kmcod ) == 0 ) {

			LD[0].dat_attr = LINP_DISP_COLOR;
			LD[0].dat_fc = ONCOLOR;
			LD[0].dat_bc = OFFCOLOR;

			pDBzm->EdabanToStr( edbf, sizeof edbf, tdata->tbary[i].brn_no );

			if( m_BrTek.get_brtek( &brec, -1, CString(pTdt->kmcod), tdata->tbary[i].brn_no ) ) {

				// 摘要名称
				char pbuf[128];
				memset( pbuf, 0, sizeof pbuf );
				int r;
				r = kjlen( (void*)(LPCTSTR) brec.brmsg, 20 );
				memmove( pbuf,  brec.brmsg, r );

				LD[0].dat_string.Format( "%-20s-%s", pbuf, edbf );
			}
			else {
				LD[0].dat_string.Format( "%20s-%s", "", edbf );
			}
			LD[0].dat_initflg = FALSE;


			LD[1].dat_attr = LINP_DISP_COLOR;
			LD[1].dat_fc = ONCOLOR;
			LD[1].dat_bc = OFFCOLOR;
			memcpy( LD[1].dat_numeric, tdata->tbary[i].brn_kari, 6 );
			LD[1].dat_initflg = FALSE;

			LD[2].dat_attr = LINP_DISP_COLOR;
			LD[2].dat_fc = ONCOLOR;
			LD[2].dat_bc = OFFCOLOR;
			memcpy( LD[2].dat_numeric, tdata->tbary[i].brn_kasi, 6 );
			LD[2].dat_initflg = FALSE;

			m_List.AddData( (LPUNKNOWN)LD, FALSE );
			add = 1;
		}
	}

	if( add ) {
		m_List.SetBackColor( RGB_WHITE );
		m_List.RedrawControl();
		CRect rc;
		m_List.GetRect( 0,2, (LPUNKNOWN)&rc);

		SCROLLINFO	sf;
		int n = m_List.GetScrollInfo( SB_VERT, &sf );

#ifdef CLOSE
CString msg;
msg.Format( "spos = %d, max %d, min %d, nPos = %d", n, sf.nMax, sf.nMin, sf.nPos );
//msg.Format( "top = %d, left = %d, bottom = %d, right = %d", rc.top, rc.left,
//				rc.bottom, rc.right );
AfxMessageBox( msg );
#endif
	}
	else {
		m_List.SetBackColor( RGB_GRAY );
		m_List.RedrawControl();
	}
	
}



