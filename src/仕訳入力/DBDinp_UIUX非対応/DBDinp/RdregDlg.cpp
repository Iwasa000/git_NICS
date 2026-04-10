// RdregDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DataListCtrl.h"
#include "RdregDlg.h"

#include "Input1.h"
#include "external.h"

#include "MstRw.h"
#include "DinpSub.h"

#include "RdgrpAddDlg.h"
#include "DBAbookView.h"

extern CMKRDSub	Mkrd;

extern struct _AUTOSEL *pAUTOSEL;

/////////////////////////////////////////////////////////////////////////

// CRdregDlg ダイアログ

IMPLEMENT_DYNAMIC(CRdregDlg, ICSDialog)

CRdregDlg::CRdregDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CRdregDlg::IDD, pParent)
{
	m_CmnId = -1;
	m_chkVal = FALSE;
	m_LbCnt = 0;

	m_bShowFlag = FALSE;

	m_Type = 0;
	m_pParent = pParent;

	m_TopIndex = 0;

	m_bOnOKFlag = FALSE;
}

CRdregDlg::~CRdregDlg()
{
}

void CRdregDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDLBL_CMBO, m_RDLBL);
	DDX_Control(pDX, IDC_NOWDATA, m_DtData);
	DDX_Check(pDX, IDC_RD_VALCHK, m_chkVal);
}


BEGIN_MESSAGE_MAP(CRdregDlg, ICSDialog)
	ON_BN_CLICKED(IDC_RD_VALCHK, &CRdregDlg::OnBnClickedRdValchk)
	ON_BN_CLICKED(IDC_BTN_GRPADD, &CRdregDlg::OnBnClickedBtnGrpadd)
	ON_CBN_SELCHANGE(IDC_RDLBL_CMBO, &CRdregDlg::OnCbnSelchangeRdlblCmbo)
	ON_WM_SHOWWINDOW()

	ON_NOTIFY( DLN_SELCHANGED,	IDC_NOWDATA, OnLineChanged )
	ON_NOTIFY( DLN_GETDISPINFO,	IDC_NOWDATA, OnGetDispInfo )
	ON_MESSAGE(WM_USER_SELCHG_MSG, OnSelChgMsg)
END_MESSAGE_MAP()


void CRdregDlg::MakeMkrdLabelCombo()
{
	int lbmax = Mkrd.label->GetRecordCount();
	int cnt = 0;
	CString str;
	if (lbmax > 0) {

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
				m_LbCnt = idx;
			} while (Mkrd.label->MoveNext() == 0);
		}
		m_RDLBL.MakeComboList(text, data);
	}
}


// CRdregDlg メッセージ ハンドラ

BOOL CRdregDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_RDLBL.SetSheetTypeMode(FALSE);
	MakeMkrdLabelCombo();

	m_RDLBL.SetCurSel(0);

	ICSDialog::OnInitDialogEX();

	if( ! m_Type ) {
		SetListColumn( 2 );
	}
	else {
		ACbookListColumn();
	}

	m_DtData.SetReadOnly( TRUE );
	m_DtData.EnforceLayout();
	m_DtData.SetConfirmMode( Confirm_Mode() );

	for( int n = 0; n < m_recCnt; n++ ) {
		//イメージサインは落とす
		m_RegData[n].m_dsign[0] &= (~0x80);
		//取消
		m_RegData[n].m_dsign[0] &= (~0x01);
		//原票リンクサイン
		m_RegData[n].m_dsign[6] &= (~0x01);

		for( int i = 7; i < 16; i++ )
			m_RegData[n].m_dsign[i] = 0;
	}

	m_DtData.SetCurrentLine(1);
	m_DtData.SetDataTotalCount( m_recCnt, TRUE );

	CString txt;
	txt.Format( "下記%d仕訳(最大%d)を定型仕訳として追加します。", m_recCnt, RDADD_MAX );
	GetDlgItem(IDC_STATIC24)->SetWindowText( txt );

//	MakeDspData( m_RegData, m_recCnt );

	DispCount(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//定型仕訳に追加する
void CRdregDlg::OnOK()
{
	int num = m_RDLBL.GetCurSel();
	int cnt = 0;

	if( m_bOnOKFlag )	return;

	m_bOnOKFlag = TRUE;

	if( mkrd_data_requery(num+1) == 0 ) {
		cnt = Mkrd.data->GetRecordCount();
	}

	CString	msg;

	for( int n = 0; n < m_recCnt; n++ ) {
		// 08.10 /00
		if( (cnt+n) >= GetRDDataMax() )
		{
			if( n == 0 ) {
				msg.Format( _T("既に最大登録数(＝%d)まで登録されています。"), GetRDDataMax());
			}
			else {
				msg.Format( _T("最大登録数（＝%d）まで登録されました。"), GetRDDataMax());
				DispCount( num );
			}
			ICSMessageBox( msg, MB_OK,0,0,this );
			m_bOnOKFlag = FALSE;
			return;
		}

		SetRddata( cnt+n+1, n );
		Mkrd.data->Append();
	}

	ICSDialog::OnOK();
	m_bOnOKFlag = FALSE;
}


// DBに データをセット
void CRdregDlg::SetRddata( int seq, int idx )
{
	int	owner;
	if( m_CmnId > 0 )	owner = m_CmnId;
	else				owner = 0;

	Mkrd.data->lbnum	= 	Mkrd.label->lbnum;
	Mkrd.data->seq	= 	seq;
	Mkrd.data->dbmn	= 	m_RegData[idx].m_dbmn;
	Mkrd.data->dkno	= 	m_RegData[idx].m_dkno;
	Mkrd.data->dbt	= 	m_RegData[idx].m_dbt;
	Mkrd.data->dbr	= 	m_RegData[idx].m_dbr;
	Mkrd.data->cbmn	= 	m_RegData[idx].m_cbmn;
	Mkrd.data->ckno	= 	m_RegData[idx].m_ckno;
	Mkrd.data->cre	= 	m_RegData[idx].m_cre;
	Mkrd.data->cbr	= 	m_RegData[idx].m_cbr;
	
	UpdateData();

	if( m_chkVal ) {
		l_print( Mkrd.data->val.GetBuffer(100), m_RegData[idx].m_val, "sssssssssss9" );
		Mkrd.data->val.ReleaseBuffer();
		Mkrd.data->valflag	= 	1;

		if( l_test( m_RegData[idx].m_zei ) ||  (m_RegData[idx].m_dsign[0] & 0x40) ) {
			l_print( Mkrd.data->zei.GetBuffer(100), m_RegData[idx].m_zei, "sssssssssss9" );
			Mkrd.data->zei.ReleaseBuffer();
			Mkrd.data->zeiflag	= 	1;
		}
		else {
			Mkrd.data->zei.Empty();
			Mkrd.data->zeiflag	= 	0;
		}

		if( l_test( m_RegData[idx].m_taika ) ) {
			l_print( Mkrd.data->taika.GetBuffer(100), m_RegData[idx].m_taika, "sssssssssss9" );
			Mkrd.data->taika.ReleaseBuffer();
		}
		else {
			Mkrd.data->taika.Empty();
		}
	}
	else {
		Mkrd.data->val.Empty();
		Mkrd.data->valflag	= 	0;
		Mkrd.data->zei.Empty();
		Mkrd.data->zeiflag	= 	0;
		Mkrd.data->taika.Empty();
	}

	//出力済みサインは落とす
	m_RegData[idx].m_dsign[0] &= (~0x02);

	Mkrd.data->dsign.Copy( m_RegData[idx].m_dsign );

	Mkrd.data->owner	= 	owner;
	Mkrd.data->dtype	= 	-1;
	Mkrd.data->tekiyo	=	m_RegData[idx].m_tekiyo;
	Mkrd.data->snumber	=	_T("");
	Mkrd.data->tdate	=	-1;
	Mkrd.data->icno		= 	-1;
	Mkrd.data->skn		=	_T("");

	Mkrd.data->hjiyu	= 0;

	// インボイス対応
	Mkrd.data->invno	=	m_RegData[idx].m_invno;
}



// 定型仕訳データリクエリー
int CRdregDlg::mkrd_data_requery( int num )
{
	CString filter;
	int	owner;

	if( m_CmnId > 0 )	owner = m_CmnId;
	else				owner = 0;

	if( Mkrd.label->SetAbsolutePosition( num ) == 0 ) {
		filter.Format( "rd_lbnum = %d AND rd_owner = %d", Mkrd.label->lbnum, owner  );
		return Mkrd.data->Requery( filter );
	}
	else {
		filter.Format( "rd_lbnum = -1 AND rd_owner = %d", owner );
		Mkrd.data->Requery( filter );
	}

	return -1;
}



#include "DtClmItem.h"

static
DT_CLMITEM ctLINE1[] = {
	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7, 7,	0,	0, 0, -1 },
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
static
DT_CLMITEM ctLN2_BK2[] = {

	{ 0, "", DATA_SEQ,	0, "", -1,-1,	6,	6,	0,	0, 0, -1 },
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




void CRdregDlg::SetListColumn( int line )
{
	CArray<DT_COLUMN, DT_COLUMN&> clm;
	int i, max, cnt, edlen;
	DT_COLUMN dtcol;
	DT_CLMITEM* pDisp;

	edlen = pDBzm->zvol->edcol;
	if( edlen < 4 )	edlen = 4;

	if( line == 1 ) {
		max = (sizeof ctLINE1 / sizeof ctLINE1[0]);
		clm.SetSize( max );

		pDisp = ctLINE1;

		for( i = cnt = 0; i < max; i++ ) {
			if( pDisp[i].id == DATA_DENP ) {
//				if( pAUTOSEL->DSPDENP_OPT == OPT_NODSP )
//					continue;
			}
			if( pDisp[i].id == DATA_DEB_BMON ) {
//				if( pAUTOSEL->DSPBMON_OPT == OPT_NODSP )
//					continue;

				// 部門なし
				if( ! (pDBzm->zvol->sub_sw & 0x02) )
					continue;
				pDisp[i].inpsize = pDBzm->zvol->bmcol;
				pDisp[i].dispsize= pDBzm->zvol->bmcol;
			}
			if( pDisp[i].id == DATA_DEB_KNO ) {
				if( pDBzm->zvol->apno == 0x20 ) {
//					if( pAUTOSEL->DSPKOJI_OPT == OPT_NODSP )
//						continue;
					// 工事なし
					if( ! (pDBzm->zvol->sub_sw & 0x04) )
						continue;

					pDisp[i].inpsize = pDBzm->zvol->kjcol;
					pDisp[i].dispsize= pDBzm->zvol->kjcol;
				}
				else {
					continue;
				}
			}
			if( pDisp[i].id == DATA_DEBT || pDisp[i].id == DATA_CRED ) {
				pDisp[i].inpsize = 14+edlen;
				pDisp[i].dispsize = 14+edlen;
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
		m_DtData.SetItemColumn( &clm[0], cnt );
	}
	else {
		// 2行うち
		max = (sizeof ctLN2_BK2 / sizeof ctLN2_BK2[0]);
		clm.SetSize( max );

		pDisp = ctLN2_BK2;
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
		m_DtData.SetItemColumn( &clm[0], cnt );
	}

	DT_ITEMMAX im;
	im.im_seq = 8;
	im.im_denp = 7;
	im.im_eda = pDBzm->zvol->edcol;
	im.im_bmn = pDBzm->zvol->bmcol;
	im.im_koji= pDBzm->zvol->kjcol;
	im.im_tky = (pDBzm->zvol->tk_ln * 2);
	im.im_sno = Voln1->snocol;

	m_DtData.SetImax( &im );
}



void CRdregDlg::ACbookListColumn()
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
	pFont = m_DtData.GetFont();

	// 摘要幅を調整する
	CClientDC dc(&m_DtData);
	CSize sz, *scrn;
	pOld = dc.SelectObject( pFont );

	sz = dc.GetTextExtent( "A", 1 );

	dc.SelectObject( pOld );

	scrn = GetScreenSize();

	CRect rect;
	m_DtData.GetClientRect( rect );
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
	data[idx].clm_title = "";
	data[idx].clm_id = DATA_SEQ;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
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
	data[idx].clm_id = DATA_USER1;
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

	m_DtData.SetItemColumn( &data[0], idx );

//	if( bOnSize ) {
//		for( int n = 0; n < idx; n++ ) {
//			m_DtData.SetListItemWidth( data[n].clm_id, 0 );
//		}
//	}

	// 工事幅 保存
	m_DtData.SetListItemWidth( DATA_DEB_KNO, kjwd );

	m_DtData.SetFocusedSign( TRUE );
	m_DtData.EnforceLayout();

	DT_ITEMMAX im;
	im.im_seq = 8;
	im.im_denp = 7;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno= Voln1->snocol;

	m_DtData.SetImax( &im );
	m_DtData.SetConfirmMode( Confirm_Mode() );

	m_DtData.RedrawDataListCtrl();

}



// データを表示する
void CRdregDlg::ListDataSet( int topidx )
{
	CDBINPDataRec* pRec;
	pRec = &m_RegData[m_TopIndex];

	int dcnt = m_recCnt - m_TopIndex;

	int lcnt = m_DtData.GetDispDataCount();

	int dsp_cnt = 0;

	if( lcnt > dcnt )	dsp_cnt = dcnt;
	else				dsp_cnt = lcnt;

	MakeDspData( pRec, dsp_cnt );
}



// データを表示する
void CRdregDlg::MakeDspData( CDBINPDataRec* pRec, int cnt )
{
	if( m_Type ) {
		ACBK_DspData( pRec, cnt );
		return;
	}
	CDataListCtrl* pLst;
	struct _DATA_LINE dline[10];

	DTLIST_DATA* pLdata;
	CDBINPDataRec* pdt;
	
	pLst = &m_DtData;

//test
//	return;

	for( int n = 0; n < cnt; n++ ) {
		pLdata = &dline[n]._XXDTA;
		pdt = &pRec[n];

		//SEQ調整
		CDBINPDataRec data;
		data = *pdt;
		data.m_seq = pDBzm->m_lastdata.m_seq + n + 1;

		DB_SyIniz( &data );
		pLdata->dd_syz_str = sy_line_txt( &data/*, NULL, INP_mode*/ );

		inpline_inz( &dline[n] );

		dline[n]._XXDTA.dd_seq = pdt->m_seq;

		UpdateData();
		if( ! m_chkVal ) {
			::ZeroMemory( data.m_val, sizeof data.m_val );
			::ZeroMemory( data.m_zei, sizeof data.m_zei );
		}

		_recdsp( &data, &dline[n], FALSE );
		// 金額・税額を空白
		if( ! m_chkVal ) {
			pLdata->val_flag = 0;
			pLdata->zei_flag = 0;
		}

		pLdata->dd_tek.image = 0;
		pLdata->dd_tek.width = 0;
		pLdata->dd_tek.height = 0;

		pLst->SetDtListData( n, pLdata );
	}
}


void CRdregDlg::ACBK_DspData( CDBINPDataRec* pRec, int cnt )
{
	CDataListCtrl* pLst;
	DTLIST_DATA	data;
	CDBINPDataRec drec;
	int dcsw;
	DBKNREC* pKn;
	BOOL	bNyuKin = FALSE;
	BOOL	bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	pLst = &m_DtData;

	for( int n = 0; n < cnt; n++ ) {
		drec = pRec[n];

		DB_SyIniz( &drec );
		data.dd_syz_str = sy_line_txt( &drec/*, NULL, INP_mode*/ );

		data.dd_seq = drec.m_seq;

		data.dd_ddate = drec.m_ddate;
		data.dd_ksign = drec.m_ksign;
		data.dd_mofs = drec.m_mofs;

		dcsw = ((CDBAbookView*)m_pParent)->GetRecordDCsgn( &drec );

		// 基本科目が 借方か、借方／貸方 同じ場合
		if( dcsw == 0 || dcsw == -1 ) {
			bNyuKin = TRUE;
			pKn = pDBzm->DB_PjisToKnrec( drec.m_cre, TRUE );

			if( pKn ) {
				KamokuString( pKn, data.dd_deb_name );
			}
			else {
				data.dd_deb_name.Empty();
			}
			data.dd_dbr = drec.m_cbr;

			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn	= -1;
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
			pKn = pDBzm->DB_PjisToKnrec( drec.m_dbt, TRUE );

			if( pKn ) {
				KamokuString( pKn, data.dd_deb_name );
			}
			else	data.dd_deb_name.Empty();
			data.dd_dbr = drec.m_dbr;

			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn	= -1;
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

		if( ! m_chkVal ) {
			::ZeroMemory( drec.m_val, sizeof drec.m_val );
			::ZeroMemory( drec.m_zei, sizeof drec.m_zei );
		}

		::ZeroMemory( data.dd_udtbl, sizeof data.dd_udtbl );
		::ZeroMemory( data.dd_val, sizeof data.dd_val );
		data.val_flag = 0;

		// 追加行は 金額欄を空白で表示
		if( bNyuKin ) {
			memcpy( data.dd_val, drec.m_val, 6 );
			data.val_flag = 1;
		}
		else {
			memcpy( kin, drec.m_val, 6 );
			data.dd_udtbl[0].userdata = kin;
			data.dd_udtbl[0].initflag = 1;
		}

		// 金額・税額を空白
		if( ! m_chkVal ) {
			data.val_flag = 0;
			data.zei_flag = 0;
			data.dd_udtbl[0].initflag = 0;
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

		m_DtData.SetDtListData( n, &data, TRUE );

	}

}


BOOL CRdregDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd ) {
			int id = pwnd->GetDlgCtrlID();

			if( pMsg->wParam == VK_F7 ) {
				if( m_bShowFlag && ! m_bOnOKFlag ) {
					OnOK();
				}
				return TRUE;
			}

			switch( id ) {
			case IDC_RDLBL_CMBO: 
				switch( pMsg->wParam ) {
				case VK_RETURN : case VK_RIGHT:
					NextDlgCtrl();	return TRUE;
				case VK_LEFT : 
					PrevDlgCtrl();	return TRUE;
				case VK_DOWN:
					if( ! m_RDLBL.GetDroppedState() ) {
						m_RDLBL.ShowDropDown(TRUE);
						return TRUE;
					}
				}
				break;
			case IDC_RD_VALCHK: 
			case IDOK: case IDCANCEL:
				switch( pMsg->wParam ) {
				case VK_RETURN :
					if( id == IDOK || id == IDCANCEL )
						break;
					//else は 下へ
				case VK_DOWN:	case VK_RIGHT:
					NextDlgCtrl();	return TRUE;
				case VK_LEFT : case VK_UP:
					PrevDlgCtrl();	return TRUE;
				break;
				}
				break;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


// 金額を定型仕訳に入れるか？
void CRdregDlg::OnBnClickedRdValchk()
{
	UpdateData();

	ListDataSet( m_TopIndex );
}


// グループ追加
void CRdregDlg::OnBnClickedBtnGrpadd()
{
	CRdgrpAddDlg	dlg( this );

	dlg.m_CmnId = m_CmnId;
	dlg.m_LbCnt = m_LbCnt;


	if( Mkrd.label->GetRecordCount() >= GetRDLabelMax() ) {
		CString msg;
		msg.Format( "既に最大登録数(＝%d)まで登録されています。", GetRDLabelMax() ); 
		ICSMessageBox( msg, 0,0,0,this);
		return;
	}

	if( dlg.DoModal() == IDOK ) {
		m_RDLBL.ResetContent();
		MakeMkrdLabelCombo();
		m_RDLBL.SetCurSel(m_LbCnt -1);
		DispCount(m_LbCnt -1);
	}
}


// グループ選択
void CRdregDlg::OnCbnSelchangeRdlblCmbo()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int sel;
	sel = m_RDLBL.GetCurSel();
	int cbIdx = m_RDLBL.GetItemData(sel);

	DispCount(cbIdx);
}

//
void CRdregDlg::DispCount( int sel )
{
	CString msg;

	int num = m_RDLBL.GetCountComboList();

	msg.Format( " %3d ／ %3d", num, GetRDLabelMax() );
	GetDlgItem(IDC_ST_GRPCNT)->SetWindowText(msg);

	int cnt = 0;

	if( mkrd_data_requery(sel+1) == 0 ) {
		cnt = Mkrd.data->GetRecordCount();
	}
	msg.Format( " %3d ／ %3d", cnt, GetRDDataMax() );
	GetDlgItem(IDC_ST_DATACNT)->SetWindowText(msg); 
}



void CRdregDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ICSDialog::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( ! m_bShowFlag ) {
		m_bShowFlag = TRUE;
		GotoDlgCtrl( &m_RDLBL );
		if( !m_RDLBL.GetDroppedState( ) )
			m_RDLBL.ShowDropDown( TRUE );
	}
}


void CRdregDlg::OnLineChanged( NMHDR* pNMHDR, LRESULT* pResult)
{
}


// 表示要求 CDataListCtrl より
void CRdregDlg::OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult)
{
	
	DLCT_DISPINFO* di;
	di = (DLCT_DISPINFO*)pNMHDR;

	m_TopIndex = di->TopIndex;
	ListDataSet( (m_TopIndex+1) );

	*pResult = 0;
}


LRESULT CRdregDlg::OnSelChgMsg(WPARAM wp, LPARAM lp)
{
	if (wp == WM_KEYDOWN) {
		if (lp == VK_RETURN) {
			NextDlgCtrl();
		}
	}
	else {
		OnCbnSelchangeRdlblCmbo();
	}
	return 1;
}
