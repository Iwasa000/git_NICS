// SelImgDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#ifdef _CPYIMG	//#* 05.10 /01

#include "DBDinp.h"

#include "mstrw.h"

#include "Input1.h"
#include "external.h"

#include "SelImgDlg.h"

#include "ICSDisp.h"

#include "scan.h"
#include "DinpSubFnc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelImgDlg ダイアログ

static int _imgdata_read( DWORD dwSelTyp, int vect, int rno, CDBINPDataRec* data );
static int imgdata_read( DWORD dwSelTyp, int vect, int rno, CDBINPDataRec* data, int nl );

#include "SEQ.h"
extern CSEQ	INSPOS_SEQ;		//挿入仕訳対応ＳＥＱ ポジション格納テーブル
extern CSEQ	SEQ_INSPOS;		//挿入仕訳対応ＳＥＱ 格納テーブル
extern CSEQ	SCMDFY_SEQ;		//カラムソート順データＳＥＱ管理テーブル
extern CSEQ	SEQ_SCMDFY;


CSelImgDlg::CSelImgDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSelImgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelImgDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	for( int i = 0; i < SIMG_DATA_MAX; i++ ) {
		m_IMGDATA[i].Reset();
	}	
	m_BaseData.Reset();
	m_CImgSeq = 0;

	m_pZm = NULL;
	m_Strno = m_Edrno = 0;
	m_SIMG_LN = 0;
	m_PREV_IDX = -1;
	// 現在フォーカス位置初期化 06.29 /01
	set_nowpos(-1);
	// 関数内部のスタティック変数の初期化 06.29 /01
	line_colordsp( -1, FALSE, TRUE );

#ifdef _SELIMG_NUMINP_
	m_bShow = FALSE;
#endif
}


void CSelImgDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelImgDlg)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_Linp);
	DDX_Control(pDX, IDC_ICSDISP_SELIMG, m_DispImg);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


// 初期化
void CSelImgDlg::Initial( CDipZmSub* pZm, CDBINPDataRec* pData, RECT Rect )
{
	m_pZm = pZm;
	m_BaseData = *pData;
	m_RECT = Rect;
}


BEGIN_MESSAGE_MAP(CSelImgDlg, ICSDialog)
	//{{AFX_MSG_MAP(CSelImgDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_CBN_SELENDOK(IDC_COMBO1, OnSelendokCombo1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	//}}AFX_MSG_MAP
#ifdef _SELIMG_NUMINP_
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelImgDlg メッセージ ハンドラ

BOOL CSelImgDlg::OnInitDialog() 
{
//	ControlInit( 500, 90, -1 );

	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	char* ptr[] = {
		"イメージ摘要のみ", "文字摘要のみ", "すべての摘要" 
	};
	m_combo.ResetContent();

	if( IMG_master != ERR ) {
		int idx = 0;
		m_combo.AddString( ptr[idx] );
		m_combo.SetItemData( idx++, SELTK_IMG_MODE );
		m_combo.AddString( ptr[idx] );
		m_combo.SetItemData( idx++, SELTK_STR_MODE );
		m_combo.AddString( ptr[idx] );
		m_combo.SetItemData( idx, SELTK_NORMAL_MODE );
		// 摘要選択モード
		m_dwTekSelectMode = SELTK_IMG_MODE;
	}
	else {
		m_combo.AddString( ptr[2] );
		m_combo.SetItemData( 0, SELTK_NORMAL_MODE );
		m_combo.EnableWindow(FALSE);
		// 摘要選択モード
		m_dwTekSelectMode = SELTK_NORMAL_MODE;
	}
	m_combo.SetCurSel(0);

	SetLinpDef();

	// 大きなｲﾒｰｼﾞの色を穏やかな青に
	CWnd* org;
	org = GetDlgItem(IDC_ICSDISP_SELIMG);
	((CICSDisp*)org)->SetBackColor( SELECT_BACK_COL );

	// 初期イメージデータ表示
	if( init_dsp() == 0 ) {
		if( IMG_master != ERR ) {
			if( m_dwTekSelectMode == SELTK_IMG_MODE ) {
				m_combo.SetCurSel(2);
				m_combo.EnableWindow(FALSE);
				m_dwTekSelectMode = SELTK_NORMAL_MODE;
				if( init_dsp() == 0 ) {
					myICSMessageBox("摘要複写画面を表示できません！",0,0,0,this);
					EndDialog(IDCANCEL);		
				}
			}
		}
		else {
			myICSMessageBox("摘要複写画面を表示できません！",0,0,0,this);
			EndDialog(IDCANCEL);		
		}
	}

	ICSDialog::OnInitDialogEX();

	CRect rect;
	GetWindowRect( rect );	
	int height, width;
	height = rect.Height();
	width = rect.Width();
	MoveWindow( m_RECT.left, (m_RECT.bottom-height), width, height );

#ifdef _SELIMG_NUMINP_
	// インバースラインセット
	int seq;
	seq = m_BaseData.m_seq;

	PostMessage( WM_RETROFOCUS, 0, seq );
#endif

//	SetWindowPos( 0, m_RECT.left, (m_RECT.bottom-height), width, height, SWP_NOZORDER );
//	SetWindowPos( &wndTopMost, m_RECT.left, m_RECT.top, width, height, SWP_SHOWWINDOW );

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


// ﾘｽﾄｲﾝﾌﾟの行をｾｯﾄ
void CSelImgDlg::SetLinpDef()
{
	int i;
	LINP_COLUMN *def;
	
	for( i = SIMG_SEQ ; i <= SIMG_TKY ; ++i )
	{
		def = &m_SIMG_LDEF[i];

		//color
		def->clm_tbcolor = SCAN_TITLE_BackCOL;
		def->clm_ttcolor = SCAN_TITLE_ForeCOL;

		switch( i )
		{
		case SIMG_SEQ:
				def->clm_kind = LINP_KIND_STRING;
				def->clm_max = 7;
			//	def->clm_attr = LINP_ATTR_RIGHT|LINP_ATTR_BOTTOM|LINP_ATTR_READONLY;
			//	def->clm_attr = LINP_ATTR_CENTER|LINP_ATTR_BOTTOM;
				def->clm_attr = LINP_ATTR_CENTER|LINP_ATTR_BOTTOM|LINP_ATTR_READONLY;
				def->clm_format = "";
				def->clm_width = 30;
				def->clm_lf = NULL;
				def->clm_title = "番号";
				break;

		case SIMG_TKY:
				def->clm_kind = LINP_KIND_TEKIYO;
				def->clm_max = 40;
		//		def->clm_attr = LINP_ATTR_LEFT|LINP_ATTR_BOTTOM|LINP_ATTR_TEKIMODE;
				def->clm_attr = LINP_ATTR_LEFT|LINP_ATTR_BOTTOM|LINP_ATTR_TEKIMODE|LINP_ATTR_READONLY;
				def->clm_format = "";
				def->clm_width = (WORD)230;
				def->clm_lf = NULL;
				def->clm_title = "　　　　摘　　要";
				break;

		}
	}	
	m_Linp.SetColumn( SIMG_TKY + 1, (LPUNKNOWN)m_SIMG_LDEF );	//登録 
}



// ﾘｽﾄｲﾝﾌﾟﾃﾞｰﾀｾｯﾄ
void CSelImgDlg::SetLinpData(SELIMG_LINE* pLN, CDBINPDataRec* pDt )
{
	pLN->SIMG_SEQ_DATA.dat_initflg = FALSE;
	pLN->SIMG_SEQ_DATA.dat_attr = 0;
	pLN->SIMG_SEQ_DATA.dat_string.Format( "%d", pDt->m_seq );

	if( pDt->m_dsign[0] & 0x80 ) {
		pLN->ImgSeq = pDt->m_imgsq;
		pLN->bImage = TRUE;
	}
	else {
		::ZeroMemory( &pLN->SIMG_TKY_DATA.dat_tek.tek_data, sizeof pLN->SIMG_TKY_DATA.dat_tek.tek_data );
	
		int n = get_strcnt( (char*)(LPCTSTR)pDt->m_tekiyo, sizeof pLN->SIMG_TKY_DATA.dat_tek.tek_data );
		memcpy( pLN->SIMG_TKY_DATA.dat_tek.tek_data, (LPCTSTR)pDt->m_tekiyo, n );

		pLN->bImage = FALSE;
	}
}


// イメージデータのセット( SetLinpData 後 )
void CSelImgDlg::set_imgtky( SELIMG_LINE* pLN )
{
	IMREC* pIMREC = NULL;

	if( pLN->bImage ) {
		// イメージ摘要
		if( pDBzm->image->SetAbsolutePosition( pLN->ImgSeq ) == 0 ) {
			pIMREC = pDBzm->image->DecodeImage();
		}

		if( pIMREC )
		{
			pLN->SIMG_TKY_DATA.dat_attr = 0;
			pLN->SIMG_TKY_DATA.dat_initflg = FALSE;
			::ZeroMemory( pLN->SIMG_TKY_DATA.dat_tek.tek_data, sizeof(pLN->SIMG_TKY_DATA.dat_tek.tek_data));
			pLN->SIMG_TKY_DATA.dat_tek.tek_width = pIMREC->dotxs;
			pLN->SIMG_TKY_DATA.dat_tek.tek_height = pIMREC->dotys;
			pLN->SIMG_TKY_DATA.dat_tek.tek_image = (char*)pIMREC->imdata;
		}
		else {
			pLN->SIMG_TKY_DATA.dat_attr = 0;
			pLN->SIMG_TKY_DATA.dat_initflg = TRUE;
			::ZeroMemory( pLN->SIMG_TKY_DATA.dat_tek.tek_data, sizeof(pLN->SIMG_TKY_DATA.dat_tek.tek_data));
			pLN->SIMG_TKY_DATA.dat_tek.tek_width = 0;
			pLN->SIMG_TKY_DATA.dat_tek.tek_height = 0;
			pLN->SIMG_TKY_DATA.dat_tek.tek_image = NULL;
		}
	}
	else {
		pLN->SIMG_TKY_DATA.dat_attr = 0;
		pLN->SIMG_TKY_DATA.dat_initflg = FALSE;
		// dat_tek.tek_data には セット済み
		pLN->SIMG_TKY_DATA.dat_tek.tek_width = 0;
		pLN->SIMG_TKY_DATA.dat_tek.tek_height = 0;
		pLN->SIMG_TKY_DATA.dat_tek.tek_image = NULL;
	}
}


// ｲﾒｰｼﾞﾃﾞｰﾀの読みこみ
int CSelImgDlg::img_read(int vect, int rno, CDBINPDataRec* pImgdata, int imgmax)
{
	CDBINPDataRec data[40];
	CDBINPDataRec* pd;

	IMREC *pIMREC = NULL;
	int n, dsiz, rno_x, imgcnt, vect_n;
	
	dsiz = sizeof(data)/sizeof(CDBINPDataRec);

	vect_n = vect;
	imgcnt = 0;
//	rno_x = rno;	// 開始ﾚｺｰﾄﾞ

//	int (*pfunc)( int, struct _DATAREC*, int ) = (vect_n < 0) ? data_back : data_for;

	// ｳｴｲﾄｶｰｿﾙ
	CWaitCursor wait;

	if( ! pDBzm->insseq_make() ) {
		insseqpos_make( NULL );
	}
	//挿入対応
	rno_x = INSPOS_SEQ.ngetseq( rno );




	do {
		for( n = 0; n < dsiz; n++ )	data[n].Reset();
		// データの読みこみ開始位置
		pd = (vect_n < 0 ) ? &data[dsiz-1] : &data[0];

		// データの読みこみ
//		n = ( *pfunc )( rno_x, pd, dsiz);
		n = imgdata_read( m_dwTekSelectMode, vect_n, rno_x, pd, imgmax );

		// イメージ摘要仕訳かどうかのチェック
		for( short k = 0; k < n && pd->m_seq != 0; k++, pd += vect_n ) {
			// 現在検索の終了ﾚｺｰﾄﾞ
			rno_x = pd->m_seq - 1;

//TRACE( "pd SEQ = %d\n", hl_lev( pd->darec._SEQ ) );
			if( pd->m_dsign[0] & 0x80 ) {

				if( pd->m_imgsq > 0 )
				{
					// イメージ摘要
					if( pDBzm->image->SetAbsolutePosition( pd->m_imgsq ) == 0 ) {
						pIMREC = pDBzm->image->DecodeImage();
					}
					if( pIMREC )
					{
						*pImgdata = *pd;
						pImgdata += vect_n;

						imgcnt++;
						if( imgcnt >= imgmax )
							break;
					}
				}
			}
			else {
				if( m_dwTekSelectMode == SELTK_NORMAL_MODE || 
					m_dwTekSelectMode == SELTK_STR_MODE ) {
					*pImgdata = *pd;
					pImgdata += vect_n;

					imgcnt++;
					if( imgcnt >= imgmax )
						break;
				}
			}
		}
		
		if( vect_n < 0 ) {
			int ins_pos		= INSPOS_SEQ.ngetseq( --rno_x );
			if( ins_pos < 0 )	break;
		//	if( --rno_x < 0 )
		//		break;
		}
		else {
			int ins_pos		= INSPOS_SEQ.ngetseq( ++rno_x );

			rno_x = SEQ_INSPOS.ngetseq( ins_pos - 1 );
		//	++rno_x;
		}
//TRACE( "rno_x = %d, n = %d, imgcnt %d\n", rno_x, n, imgcnt );
	} while( imgcnt < imgmax && n == imgmax );

	return imgcnt;

}


//初期表示
int CSelImgDlg::init_dsp( int seq_no/*=-1*/ )
{
	int i,n, k, j;
	CDBINPDataRec tmp[SIMG_DATA_NL];
	int seq;
	m_SIMG_LN = 0;

	for( int n = 0; n < SIMG_DATA_NL; n++ ) {
		mk_zerodata( &m_SIMG_LINE[n] );	//ゼロデータセット
	}

	m_Linp.RemoveAll();
	if( seq_no == -1 )
		seq = m_BaseData.m_seq;
	else
		seq = seq_no;

	// 底から
	n = img_read( -1, seq - 1, &tmp[SIMG_DATA_NL-1], SIMG_DATA_NL );
	for( i = 0; i < SIMG_DATA_MAX; i++ ) {
		m_IMGDATA[i].Reset();
	}

	for( i = 0; i < SIMG_DATA_NL; i++ ) {
		m_SIMG_LINE[i].ZREC_NO = 0;
	}

	if( n ) {
		for( i = 0 ; i < n; i++ ) {
			m_IMGDATA[n-1-i] = tmp[SIMG_DATA_NL-1-i];
		}
	}

	// ｲﾒｰｼﾞﾃﾞｰﾀが表示行数より少ない時
	if( n < SIMG_DATA_NL ) {

		//挿入対応
		int nPos = INSPOS_SEQ.ngetseq( seq - 1 );
		// nPos は １ ～
		int nSeqNum = SEQ_INSPOS.ngetseq( nPos );

		if( k = img_read( 1, nSeqNum-1, tmp, SIMG_DATA_NL - n )) {
			for( i = n, j = 0; j < k; i++, j++ ) {
				m_IMGDATA[i] = tmp[j];
			}
			n += k;
		}
	}

	if( n ) {
		//表示データ作成
		mk_data( &m_SIMG_LINE[ m_SIMG_LN ], &m_IMGDATA[ m_SIMG_LN ], m_SIMG_LN, n );

		//データ表示
		for( i = 0 ; i < n ; ++i )
		{	
			set_imgtky( &m_SIMG_LINE[ m_SIMG_LN+i ] );
			SetData( m_SIMG_LN+i, m_SIMG_LINE[ m_SIMG_LN+i ]._SIMGDTA, FALSE );
		}
		m_Linp.RedrawControl();	// 再描画

#ifdef KEEP
		// インバースラインを２行目に
		if( n > 1 ) {
			m_Linp.SetInputPositionEX( 1, 0 );
		}
#endif
		m_PREV_IDX = 0;	// 表示済みのｲﾝﾃﾞｯｸｽ
		disp_image(0);

		set_nowpos( SIMG_LINP );
	}

	m_SIMG_LN += n;

	return m_SIMG_LN;

}


//
// １行セット
void CSelImgDlg::SetData( int idx, LPVOID data, BOOL sw )
{
	m_Linp.SetData( idx, (LPUNKNOWN)data, sw );
}


//	data table N line scroll down
int CSelImgDlg::simg_dscroll_down( int scrol_nl )
{
	int loop;
	int seq,seq2,n;
	CDBINPDataRec dta;

	for( loop = 0 ; loop < scrol_nl ; ++loop )
	{
		n = m_SIMG_LINE[0].ZREC_NO - 1;	//m_IMGDATA[n]

		seq = m_IMGDATA[n].m_seq;	//そのシーク番号

		if( n > 0 )
			n--;
		else	// n == 0
			n = SIMG_DATA_MAX - 1;
		seq2 = m_IMGDATA[n].m_seq;	//一つ前のシーク番号
//		if( seq2 <= 0 || seq2 > seq )	//まだ読み込まれていないZREC[n]か，リングバッファの底か
		if( seq2 <= 0 || datarec_seqcmp( 0, seq2, seq ) > 0 )
		{
			int inspos = INSPOS_SEQ.ngetseq( seq-1 );
			if( --inspos <= 0 ) {
				break;
			}
		//	if( --seq <= 0 )
		//	{
		//		break;
		//	}
			seq = SEQ_INSPOS.ngetseq( inspos-1 );

			if( !img_read( -1, seq - 1, &dta, 1 ) )
			{
				break;
			}
			m_IMGDATA[n] = dta;
		}

		//仕訳表示データエリア１行ダウン
		simg_scroll_down( m_SIMG_LINE, SIMG_DATA_NL, 1 );

		//１行表示
		m_Linp.RemoveData( m_SIMG_LN - 1 );	//最終行を削除
		mk_data( &m_SIMG_LINE[ 0 ], &m_IMGDATA[n], n, 1 );	//表示データを作成

		set_imgtky( &m_SIMG_LINE[ 0 ] );
		m_Linp.InsertData( 0, (LPUNKNOWN)&m_SIMG_LINE[ 0 ]._SIMGDTA, TRUE );	//先頭行を表示

	}
	if( !loop )
		return(0);

	return(loop);
}

//	data table N line scroll up
int CSelImgDlg::simg_dscroll_up( int scrol_nl )
{
	int loop;
	int seq,seq2,n;
	CDBINPDataRec dta;

	for ( loop = 0 ; loop < scrol_nl ; ++loop )
	{
		n = m_SIMG_LINE[m_SIMG_LN-1].ZREC_NO - 1;	//m_IMGDATA[n]

		seq = m_IMGDATA[n].m_seq;	//そのシーク番号
		if( n < SIMG_DATA_MAX - 1 )
			n++;
		else	// n == SIMG_DATA_MAX - 1
			n = 0;
		seq2 = m_IMGDATA[n].m_seq;	//一つ先のシーク番号
	//	if( seq2 <= 0 || seq2 < seq )	//まだ読み込まれていないZREC[n]か，リングバッファのTOPか
		if( seq2 <= 0 || datarec_seqcmp( 0, seq2, seq ) < 0 )
		{
			int inspos = INSPOS_SEQ.ngetseq( seq-1 );
			++inspos;
			seq = SEQ_INSPOS.ngetseq( inspos-1 );
		//	++seq;
			if( !img_read( 1, seq - 1, &dta, 1 ) )
			{
				break;
			}
			m_IMGDATA[n] = dta;
		}
		//仕訳表示データエリア１行アップ
		simg_scroll_up( m_SIMG_LINE, SIMG_DATA_NL, 1 );

		//１行表示
		m_Linp.RemoveData( 0 );	//先頭行を削除
		mk_data( &m_SIMG_LINE[ m_SIMG_LN - 1 ], &m_IMGDATA[n], n, 1 );	//表示データを作成

		set_imgtky( &m_SIMG_LINE[ m_SIMG_LN - 1 ] );
		SetData( m_SIMG_LN - 1, &m_SIMG_LINE[ m_SIMG_LN - 1]._SIMGDTA, TRUE );	//最終行を表示

	}
	if( !loop )
		return(0);

	return(loop);
}

//表示 N 行分ダウン
void CSelImgDlg::simg_scroll_down( SELIMG_LINE *ldta, int max, int ln )
{
	int start_pn, end_pn;
	int i;

	start_pn = 0;		//スクロール開始ポジション
	end_pn = max - 1;	//スクロール終了ポジション
	while( ln-- )		//スクロール行数分
	{
		for( i = end_pn ; i > start_pn ; --i )
		{
			*(ldta + i ) = *(ldta + i - 1);
		}

		mk_zerodata( ldta + i );	//ゼロデータセット
	}

}

//表示 N 行分アップ
void CSelImgDlg::simg_scroll_up( SELIMG_LINE *ldta, int max, int ln )
{
	int start_pn, end_pn;
	int i;

	start_pn = 0;		//スクロール開始ポジション
	end_pn = max - 1;	//スクロール終了ポジション
	while( ln-- )		//スクロール行数分
	{
		for( i = start_pn ; i < end_pn ; ++i )
		{
			*(ldta + i ) = *(ldta + i + 1);

		}
		mk_zerodata( ldta + i );	//ゼロデータセット
	}
}



//表示データを作成
void CSelImgDlg::mk_data( SELIMG_LINE* ldta, CDBINPDataRec* data, int data_n, int nline )
{
	int i;

	for( i = 0 ; i < nline ; ++i )
	{
		SetLinpData(ldta + i, data + i );
		// m_IMGDATA[] ポジション
		(ldta + i)->ZREC_NO = ++data_n;
	}	
}


//ゼロデータを作成
void CSelImgDlg::mk_zerodata( SELIMG_LINE* pLN )
{
	pLN->ZREC_NO = 0;
	
	pLN->SIMG_SEQ_DATA.dat_attr = 0;
	pLN->SIMG_SEQ_DATA.dat_initflg = TRUE;
	pLN->SIMG_SEQ_DATA.dat_seq = 0;

	pLN->SIMG_TKY_DATA.dat_attr = 0;
	pLN->SIMG_TKY_DATA.dat_initflg = TRUE;
	::ZeroMemory( pLN->SIMG_TKY_DATA.dat_tek.tek_data, sizeof(pLN->SIMG_TKY_DATA.dat_tek.tek_data));
	pLN->SIMG_TKY_DATA.dat_tek.tek_width = 0;
	pLN->SIMG_TKY_DATA.dat_tek.tek_height = 0;
	pLN->SIMG_TKY_DATA.dat_tek.tek_image = NULL;

	pLN->ImgSeq = 0;
	pLN->bImage = FALSE;

}


// 選択されいるｲﾒｰｼﾞをICSDispに表示
void CSelImgDlg::disp_image( int idx )
{
	IMREC* pIMREC = NULL;

	m_DispImg.ClearDisp();

	if( m_SIMG_LINE[idx].bImage ) {
		m_DispImg.SetBackColor( SELECT_BACK_COL );
		m_bSelTekImage = TRUE;

		if( m_SIMG_LINE[idx].ImgSeq > 0 ) {
			// 選択されているイメージの情報をセット
			// イメージ摘要
			if( pDBzm->image->SetAbsolutePosition( m_SIMG_LINE[idx].ImgSeq ) == 0 ) {
				pIMREC = pDBzm->image->DecodeImage();
			}
			m_CImgSeq = m_SIMG_LINE[idx].ImgSeq;
		}

		if( pIMREC ) {
			m_DispImg.ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
		}
	}
	else {
		// 文字摘要
		m_DispImg.SetBackColor( ::GetSysColor( COLOR_BTNFACE ) );
		m_bSelTekImage = FALSE;
		::ZeroMemory( m_Tekiyo, sizeof m_Tekiyo );
		CDBINPDataRec* pDt;

		int index = m_SIMG_LINE[idx].ZREC_NO-1;

		if( index >= 0 ) {
			pDt = &m_IMGDATA[ index ];
			strncpy_s( m_Tekiyo, sizeof m_Tekiyo, pDt->m_tekiyo, Voln1->tk_ln*2 );
		}
	}

	disp_seq( idx );
}

// SEQ を表示
void CSelImgDlg::disp_seq( int idx )
{
	int n;
	CWnd* dspwnd;

	n = m_SIMG_LINE[idx].ZREC_NO -1;

	if( n < 0 )		return;

	dspwnd = GetDlgItem(IDC_ICSDISP_SEQ);
	char seqbf[48];

	((CICSDisp*)dspwnd)->ClearDisp();

#ifdef _SELIMG_NUMINP_
	VARIANT var;

	if( n >= 0 ) {
		wsprintf( seqbf, "%d", m_IMGDATA[n].m_seq );
		((CICSDisp*)dspwnd)->ColmDisp(1, seqbf);

		var.pbVal = (BYTE*)seqbf;
		DBdata_set( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	}
	else {
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
#endif
}

//	データ行の移動
int CSelImgDlg::_dline_chg( int idx, int focs_sw /*= 0*/ )
{
	int i, ret;

	ret = 0;

	if( idx >= m_SIMG_LN )	//オーバーフロー
	{
		idx = m_PREV_IDX;

//		i = simg_dscroll_up(2);
		i = simg_dscroll_up(1);
		if( !i )
		{
			ret = -1;
		}
		else if( i == 1 )
		{
		}
		else	//i == 2
		{
//			SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYDOWN, VK_UP, 0 );
//			SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYUP, VK_UP, 0 );
		}
	}
	else if( idx < 0 )	//アンダーフロー
	{
		idx = m_PREV_IDX;

//		i = simg_dscroll_down(2);
		i = simg_dscroll_down(1);
		if( !i )
		{
			ret = -1;
		}
		else if( i == 1 )
		{
		}
		else	//i == 2
		{
//			SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYDOWN, VK_DOWN, 0 );
//			SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYUP, VK_DOWN, 0 );
		}
	}
	else 
	{
		if( idx == 0 && !focs_sw && m_PREV_IDX == 0 )
		{
			if( simg_dscroll_down(1) )
			{
//				SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYDOWN, VK_DOWN, 0 );
//				SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYUP, VK_DOWN, 0 );
			}
		}
		else if( (idx == m_SIMG_LN - 1) && !focs_sw && (m_PREV_IDX == m_SIMG_LN -1) )
		{
			if( simg_dscroll_up(1) )
			{
//				SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYDOWN, VK_UP, 0 );
//				SendDlgItemMessage( IDC_ICSLISTINPCTRL1, WM_KEYUP, VK_UP, 0 );
			}
		}
		else
		{
		}
	}
	disp_image( idx );
	m_PREV_IDX = idx;

	return(0);
}

BEGIN_EVENTSINK_MAP(CSelImgDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CSelImgDlg)
	ON_EVENT(CSelImgDlg, IDC_ICSLISTINPCTRL1, 4 /* KeyIn */, OnKeyInIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSelImgDlg, IDC_ICSLISTINPCTRL1, 7 /* LButtonDblClick */, OnLButtonDblClickIcslistinpctrl1, VTS_I2)
	ON_EVENT(CSelImgDlg, IDC_ICSLISTINPCTRL1, 6 /* LButtonClick */, OnLButtonClickIcslistinpctrl1, VTS_I2)
	ON_EVENT(CSelImgDlg, IDC_ICSLISTINPCTRL1, 9 /* Focused */, OnFocusedIcslistinpctrl1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
#ifdef _SELIMG_NUMINP_
	ON_EVENT(CSelImgDlg, IDC_ICSDBEDT8CTRL1, 3, CSelImgDlg::SetFocusIcsdbedt8ctrl1, VTS_NONE)
	ON_EVENT(CSelImgDlg, IDC_ICSDBEDT8CTRL1, 1, CSelImgDlg::TerminationIcsdbedt8ctrl1, VTS_I4 VTS_I4 VTS_I4)
#endif
	ON_EVENT(CSelImgDlg, IDC_ICSDBEDT8CTRL1, 2, CSelImgDlg::KillFocusIcsdbedt8ctrl1, VTS_I4)
END_EVENTSINK_MAP()



void CSelImgDlg::OnFocusedIcslistinpctrl1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HWND hwnd = GetDlgItem(IDC_ICSLISTINPCTRL1)->m_hWnd;
	if( ::IsWindow( hwnd ) == FALSE )
		return;
	
TRACE( "focus\n" );
	// 選択ラインの色をOFF
	set_nowpos( SIMG_LINP );
	line_colordsp( -1, FALSE );
	
}


void CSelImgDlg::OnKeyInIcslistinpctrl1(short nChar, short previndex, short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HWND hwnd = GetDlgItem(IDC_ICSLISTINPCTRL1)->m_hWnd;
	if( ::IsWindow( hwnd ) == FALSE )
		return;

	int st;

/*
	try
	{
*/
		if( nChar == VK_RETURN )
		{
			img_focus(SIMG_OK);

		//	PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED),
		//							(LPARAM)GetDlgItem(IDOK)->m_hWnd);
			return;
		}
		else if( nChar == VK_ESCAPE ) {
			PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED),
									(LPARAM)GetDlgItem(IDCANCEL)->m_hWnd);
			return;
		}
//		else if( nChar == VK_F3 ) {
//			img_focus(SIMG_OK);
//			return;
//		}
		else if( nChar == VK_F2 || nChar == VK_LEFT ) {
			img_focus(SIMG_TKSEL);
			return;
		}
		else
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				st = _dline_chg( index );
				if( st == -1 )
				{
					return;
				}
				else if( !st )
				{
					return;
				}
			}
			else if( nChar == VK_PRIOR || nChar == VK_NEXT ) {
				int i;
				if( nChar == VK_PRIOR ) {
					i = simg_dscroll_down(SIMG_DATA_NL);
				}
				else {
					i = simg_dscroll_up(SIMG_DATA_NL);
				}
				if( i ) {
					disp_image( index );
					m_PREV_IDX = index;
				}
			}
		}
/*	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
*/
}
	

void CSelImgDlg::OnLButtonDblClickIcslistinpctrl1(short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HWND hwnd = GetDlgItem(IDC_ICSLISTINPCTRL1)->m_hWnd;
	if( ::IsWindow( hwnd ) == FALSE )
		return;

	disp_image( index );
	PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)GetDlgItem(IDOK)->m_hWnd);
}


void CSelImgDlg::OnLButtonClickIcslistinpctrl1(short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// イメージの表示
	
	HWND hwnd = GetDlgItem(IDC_ICSLISTINPCTRL1)->m_hWnd;
	if( ::IsWindow( hwnd ) == FALSE )
		return;

	disp_image( index );

#ifdef CLOSE // ダブルクリックでの選択を有効にするため、行移動機能をカット
	if( GetFocus()->GetDlgCtrlID() != IDC_ICSLISTINPCTRL1 ) {
		img_focus(SIMG_LINP);
	}

	int st = _dline_chg( index );

	if( st == -1 )
	{
	//	Buzzer();
		return;
	}
	else if( !st )
	{
		return;
	}
#endif
}


// 選択された行の表示

void CSelImgDlg::line_colordsp( int idx, BOOL sgn, BOOL initflg /* = FALSE*/)
{
	static int prev_idx;
	int pn;
	SELIMG_LINE* pLN;

	// 以前の選択行のインデックス変数の初期化のみ(ダイアログ表示前に必ず)
	// 06.29 /01
	if( initflg ) {
		prev_idx = -1;	return;
	}

	if( prev_idx != -1 && prev_idx != idx ) {
		// 選択行の 色のOFF
		if( m_SIMG_LN >= (prev_idx+1) ) {
			pLN = &m_SIMG_LINE[ prev_idx ];
			for( pn = SIMG_SEQ; pn <= SIMG_TKY; pn++ ) {
				pLN->_SIMGDTA[pn].dat_attr = 0;
				pLN->_SIMGDTA[pn].dat_fc = 0;
				pLN->_SIMGDTA[pn].dat_bc = 0;
				pLN->_SIMGDTA[pn].dat_initflg = FALSE;

				SetData( prev_idx, pLN->_SIMGDTA, TRUE );		
			}
		}
	}
	prev_idx = idx;

	// 選択されている行の色を表示
	if( sgn ) {
		pLN = &m_SIMG_LINE[ idx ];
		for( pn = SIMG_SEQ; pn <= SIMG_TKY; pn++ ) {
			pLN->_SIMGDTA[pn].dat_attr = LINP_DISP_COLOR;
			pLN->_SIMGDTA[pn].dat_fc = RGB_BLACK;
			pLN->_SIMGDTA[pn].dat_bc = SELECT_BACK_COL;
			pLN->_SIMGDTA[pn].dat_initflg = FALSE;

			SetData( idx, pLN->_SIMGDTA, TRUE );		
		}
	}
}

// フォーカスのチェック
void CSelImgDlg::focus_chk()
{
	CWnd* pwnd;

	int prev_pos;
	if( pwnd = GetFocus() ) {
		prev_pos = get_nowpos();

		if( prev_pos != SIMG_OK	&& pwnd->m_hWnd == GetDlgItem(IDOK)->m_hWnd )
			set_nowpos( SIMG_OK );
		else if( prev_pos != SIMG_CANCEL && pwnd->m_hWnd == GetDlgItem(IDCANCEL)->m_hWnd )
			set_nowpos( SIMG_CANCEL );
		else if( prev_pos != SIMG_TKSEL && pwnd->m_hWnd == GetDlgItem(IDC_COMBO1)->m_hWnd )
			set_nowpos( SIMG_TKSEL );

		// リストインプットの選択ラインの色の表示
		if( prev_pos == SIMG_LINP && get_nowpos() != SIMG_LINP ) {
			line_colordsp(m_PREV_IDX, TRUE);		
		}
	}

}

// 現在フォーカス位置セット
void CSelImgDlg::set_nowpos(int pos)
{
	m_SIMG_POS = pos;
}

// 現在フォーカス位置取得
int  CSelImgDlg::get_nowpos()
{
	return m_SIMG_POS;
}

// OKボタンキーダウン
int CSelImgDlg::ButtonOkKeydown(MSG* pMsg)
{
	int rval = 0;

	switch(pMsg->wParam) {
	case VK_F2:
	case VK_LEFT:
	case VK_UP:
		img_focus(SIMG_LINP);
		rval = 1;
		break;
//	case VK_F3:
	case VK_DOWN:
	case VK_RIGHT:
		img_focus(SIMG_CANCEL);
		rval = 1;
		break;
	case VK_RETURN:
		EndDialog( IDOK );
		rval = 1;
		break;
	default:
		break;
	}

	return rval;
}


// キャンセルボタンキーダウン
int CSelImgDlg::ButtonCancelKeydown(MSG* pMsg)
{
	int rval = 0;

	switch(pMsg->wParam) {
	case VK_F2:
	case VK_LEFT:
	case VK_UP:
		img_focus(SIMG_OK);
		rval = 1;
		break;
//	case VK_F3:
	case VK_DOWN:
	case VK_RIGHT:	// フォーカスは動かさない
		rval = 1;
		break;
	case VK_RETURN:
		EndDialog( IDCANCEL );
		rval = 1;
		break;
	default:
		break;
	}

	return rval;
}

// コンボボックスキーダウン
int CSelImgDlg::ComboSelectKeydown(MSG* pMsg)
{
	int rval = 0;

	switch(pMsg->wParam) {
	case VK_DOWN:
		if( ! m_combo.GetDroppedState() ) {
			m_combo.ShowDropDown( TRUE );
			rval = 1;
		}
		break;
	case VK_RETURN:
//	case VK_F3:
	case VK_RIGHT:
	case VK_TAB:
		if( m_combo.GetDroppedState() ) {
			m_combo.ShowDropDown( FALSE );
			// 設定取得
			SendMessage(WM_COMMAND, MAKELONG(IDC_COMBO1,CBN_SELENDOK), (LPARAM)m_combo.m_hWnd );
		}
		img_focus(SIMG_LINP);
		rval = 1;
		break;
	case VK_F2:
	case VK_LEFT:
		if( m_combo.GetDroppedState() ) {
			m_combo.ShowDropDown( FALSE );
			// 設定取得
			SendMessage(WM_COMMAND, MAKELONG(IDC_COMBO1,CBN_SELENDOK), (LPARAM)m_combo.m_hWnd );
		}
		img_focus(SIMG_CANCEL);
		rval = 1;
		break;
	default:
		break;
	}

	return rval;
}

// イメージのフォーカス
int CSelImgDlg::img_focus( int pn )
{
	int idc;
	
	switch(pn) {
	case SIMG_TKSEL :		// 08.31 /04
		idc = IDC_COMBO1;
		break;
	case SIMG_LINP:
		idc = IDC_ICSLISTINPCTRL1;
		break;
	case SIMG_OK:
		idc = IDOK;
		break;
	case SIMG_CANCEL:
		idc = IDCANCEL;
		break;
#ifdef _SELIMG_NUMINP_
	case SIMG_NUM:
		idc = IDC_ICSDBEDT8CTRL1;
		break;
#endif
	default:
		return 0;
	}

	if( pn == SIMG_LINP )
		GetDlgItem(idc)->SetFocus();
	else
		((ICSDialog*)this)->GotoDlgCtrl( GetDlgItem(idc) );

	return 0;
}



BOOL CSelImgDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	focus_chk();

	if( pMsg->message == WM_LBUTTONDOWN ) {
		if( pMsg->hwnd == GetDlgItem(IDC_ICSDISP_SELIMG)->m_hWnd ) {
			// 大イメージをクリック
			if( GetFocus()->GetDlgCtrlID() != IDOK ) {
				img_focus(SIMG_OK);
				return TRUE;
			}
		}
	}
	else if( pMsg->message == WM_KEYDOWN ) {
		int nChar = -1;

		if( pMsg->hwnd == GetDlgItem(IDC_ICSDBEDT8CTRL1)->m_hWnd ) {
			if( pMsg->wParam == VK_TAB ) {
				int idx = m_Linp.GetIndex();
				disp_seq( idx );

				if( is_SHIFT() ) {
					PostMessage( WM_RETROFOCUS, 1, SIMG_LINP );
					return TRUE;
				}
				else {
					PostMessage( WM_RETROFOCUS, 1, SIMG_OK );
					return TRUE;
				}
			}
		}

		if( pMsg->hwnd == GetDlgItem(IDOK)->m_hWnd ) {
			if( ButtonOkKeydown( pMsg ) )
				return TRUE;
		}
		else if( pMsg->hwnd == GetDlgItem(IDCANCEL)->m_hWnd  ) {
			if( ButtonCancelKeydown( pMsg ) )
				return TRUE;		
		}
		else if( pMsg->hwnd == GetDlgItem(IDC_COMBO1)->m_hWnd  ) {
			if( ComboSelectKeydown( pMsg ) )
				return TRUE;
		}
	}
	
	return ICSDialog::PreTranslateMessage(pMsg);

}


// イメージ仕訳の読みこみ(スレーブ)
// rno は INSPOS( 1～ )
static int _imgdata_read( DWORD dwSelTyp, int vect, int rno, CDBINPDataRec* data )
{

CDBINPDataRec dta;
CDBINPDataRec key;
int vect_n, okdata;

	vect_n = (vect < 0 ) ? -1 : 1;

BOOL	bRead;

	for( ; ; rno += vect_n )
	{
		if( rno >= 0 ) {
			int seq = SEQ_INSPOS.ngetseq( rno-1 );
			if( seq > 0 ) {
				if( cdata_read( seq-1, &dta ) == ERR )
					return -1;	//over access
			}
			else {
				return -1;	//over access
			}
		}
		else	return -1;	//over access

		// イメージ仕訳 or 文字のみ or すべて
		bRead = FALSE;

		switch( dwSelTyp ) {
		case SELTK_IMG_MODE :
			if( dta.m_dsign[0] & 0x80 )	bRead = TRUE;
			break;
		case SELTK_STR_MODE :
			if( !(dta.m_dsign[0] & 0x80) )	bRead = TRUE;
			break;
		default:
			bRead = TRUE;
			break;
		}

		okdata = 0;
		// 該当仕訳のユーザーチェック
		if( ! pDBzm->data_authority() ) {
			// カレントユーザ
			int owner = pDBzm->DBUserNo();

			if( owner == dta.m_owner ) {
				okdata = 1;
			}
		}
		else {
			okdata = 1;
		}

		if( okdata ) {
			if( !(dta.m_dsign[0] & 0x01) && bRead ) 
			{
				*data = dta;
				return rno;		//OK
			}
		}
	}
}


// イメージ仕訳の読みこみ
static int imgdata_read( DWORD dwSelTyp, int vect, int rno, CDBINPDataRec* data, int nl )
{
CDBINPDataRec dta;
int n,vect_n;

	vect_n = (vect < 0 ) ? -1 : 1;

	for( n = 0 ; n < nl ; rno += vect_n )
	{
		if( rno >= 0 ) {
			rno = _imgdata_read( dwSelTyp, vect, rno, &dta );
		}

		if( rno == -1 )	//over access
		{
			break;
		}
		//データセット
		*data = dta;

		data += vect_n;
		n++;
	}

	return( n );
}



// スピンコントロール
void CSelImgDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	CWnd *pwnd = GetDlgItem( IDC_ICSLISTINPCTRL1 );

	if( get_nowpos() != SIMG_LINP )
		img_focus( SIMG_LINP );

	if( pNMUpDown->iDelta < 0 )
	{
		pwnd->PostMessage( WM_KEYDOWN, VK_UP );
		pwnd->PostMessage( WM_KEYUP, VK_UP );
	}
	else
	{
		pwnd->PostMessage( WM_KEYDOWN, VK_DOWN );
		pwnd->PostMessage( WM_KEYUP, VK_DOWN );
	}

	*pResult = 0;
}



void CSelImgDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CWnd *pwnd = GetDlgItem( IDC_ICSLISTINPCTRL1 );

	if( get_nowpos() != SIMG_LINP )
		img_focus( SIMG_LINP );

	if( pNMUpDown->iDelta < 0 )
	{
		pwnd->PostMessage( WM_KEYDOWN, VK_PRIOR );
		pwnd->PostMessage( WM_KEYUP, VK_PRIOR );
	}
	else
	{
		pwnd->PostMessage( WM_KEYDOWN, VK_NEXT );
		pwnd->PostMessage( WM_KEYUP, VK_NEXT );
	}

	*pResult = 0;
}



void CSelImgDlg::OnSelendokCombo1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_combo.GetCurSel();
	DWORD dwTyp;
	if( sel != CB_ERR ) {
		dwTyp = m_combo.GetItemData( sel );
	}
	if( dwTyp != m_dwTekSelectMode ) {
		m_dwTekSelectMode = dwTyp;

		int seq = -1;
		if( m_PREV_IDX >= 0 ) {
			int n;
			for( int i = (SIMG_DATA_NL-1); i >= 0; i-- ) {
				n = m_SIMG_LINE[ i ].ZREC_NO -1;
				if( n >= 0 ) {
					seq = m_IMGDATA[n].m_seq;
					break;
				}
			}
		}
TRACE( "init_dsp seq %d @OnSelendokCombo1\n", seq );
		init_dsp( seq );
	}
}


#ifdef _SELIMG_NUMINP_


void CSelImgDlg::SetFocusIcsdbedt8ctrl1()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	img_focus( SIMG_NUM );
	set_nowpos( SIMG_NUM );
}

void CSelImgDlg::TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == VK_RETURN /*|| nChar == VK_TAB*/ ) {
		char	buf[128];
		VARIANT var;

		DBdata_get( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal != NULL ) {
			int no;
			strcpy_s( buf, sizeof buf, (const char*)var.pbVal );
			no = atoi( buf );
			search_tky( no );
		}
	}
	else if( nChar == 0 ) {
		int idx = m_Linp.GetIndex();

		disp_seq( idx );
	}
}

void CSelImgDlg::KillFocusIcsdbedt8ctrl1(long inplen)
{
	TerminationIcsdbedt8ctrl1(0,inplen,0);
}

#ifdef OLD_CLOSE
// NUMEDIT Focus
void CSelImgDlg::OnFocusIcsnumedit() 
{
	img_focus( SIMG_NUM );
	set_nowpos( SIMG_NUM );
}

// NUMEDIT Termination
void CSelImgDlg::OnTerminationIcsnumedit(short nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	if( nChar == VK_RETURN ) {
		CString str;
		short len;
		int no;
		_nkrd( IDC_ICSNUMEDIT, str, len, CICSNUMEdit );

		no = atoi( str );

		search_tky( no );
	}
	else if( nChar == 0 ) {
		int idx = m_Linp.GetIndex();

		disp_seq( idx );
	}
}
#endif


int CSelImgDlg::search_tky( int seq )
{
	int vect = -1, lidx = -1;
	int	i, n, seqnum;

	for( i = (SIMG_DATA_NL-1); i >= 0; i-- ) {
		n = m_SIMG_LINE[ i ].ZREC_NO -1;
		if( n >= 0 ) {
			seqnum = m_IMGDATA[n].m_seq;

		//	if( seq > seqnum ) {	// 最下行より 大きい場合
			if( datarec_seqcmp( 0, seq, seqnum ) > 0 ) {
				vect = 1;
				break;
			}
			else {
			//	if( seqnum <= seq ) {
			//	if( datarec_seqcmp( 0, seq, seqnum ) <= 0 ) {
				if( seq == seqnum ) {
					lidx = i;
					break;
				}
			}
		}
	}


	if( lidx != -1 ) {
	//	CRect rect;
	//	m_Linp.GetRect( lidx, 0, (LPUNKNOWN)&rect );
	//	ScreenToClient( rect );
	//	rect.DeflateRect( 1,1 );

	//	m_Linp.SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( rect.left, rect.top ) );

		if( m_PREV_IDX < lidx ) {
			int n = lidx - m_PREV_IDX;

			m_Linp.SetFocus();

			for( i = 0; i < n; i++ ) {
				keybd_event( VK_DOWN, 0,0,0 );
				keybd_event( VK_DOWN, 0,KEYEVENTF_KEYUP,0 );
			}
		}
		else {
			int n = m_PREV_IDX - lidx;

			m_Linp.SetFocus();

			for( i = 0; i < n; i++ ) {
				keybd_event( VK_UP, 0,0,0 );
				keybd_event( VK_UP, 0,KEYEVENTF_KEYUP,0 );
			}
		}
		m_Linp.RedrawControl();	// 再描画

		return 0;
	}
	else {
		m_PREV_IDX = -1;
		set_nowpos(-1);
		line_colordsp( -1, FALSE, TRUE );
		
//		int max = (unsigned int)IDXENT( Ndate );
		int max = pDBzm->m_dacnt;

		if( seq > max )	seq = max;

		init_dsp( seq );

		lidx = -1;
		for( i = (SIMG_DATA_NL-1); i >= 0; i-- ) {
			n = m_SIMG_LINE[ i ].ZREC_NO -1;
			if( n >= 0 ) {
				seqnum = m_IMGDATA[n].m_seq;

			//	if( seqnum > seq )
				if( datarec_seqcmp( 0, seqnum, seq ) > 0 )
					continue;

			//	if( seqnum <= seq ) {
				if( datarec_seqcmp( 0, seqnum, seq ) <= 0 ) {
					lidx = i;
					break;
				}
			}
		}
		if( lidx != -1 ) {
			m_Linp.SetFocus();

			for( i = 0; i < lidx; i++ ) {
				keybd_event( VK_DOWN, 0,0,0 );
				keybd_event( VK_DOWN, 0,KEYEVENTF_KEYUP,0 );
			}
		}

		m_Linp.RedrawControl();	// 再描画
	}

	return 0;
}


// インバースラインをセットする。
LRESULT CSelImgDlg::OnRetroFocus( WPARAM w, LPARAM l )
{
	int i, n, seq, seqnum, lidx = -1;

	if( w == 0 ) {
	
		seq = l;

		for( i = (SIMG_DATA_NL-1); i >= 0; i-- ) {
			n = m_SIMG_LINE[ i ].ZREC_NO -1;
			if( n >= 0 ) {

				seqnum = m_IMGDATA[n].m_seq;

			//	if( seqnum > seq )
				if( datarec_seqcmp( 0, seqnum, seq ) > 0 )
					continue;

			//	if( seqnum < seq ) {
				if( datarec_seqcmp( 0, seqnum, seq ) < 0 ) {
					lidx = i;
					break;
				}
			}
		}

		m_Linp.SetFocus();

		for( i = 0; i < lidx; i++ ) {
			keybd_event( VK_DOWN, 0,0,0 );
			keybd_event( VK_DOWN, 0,KEYEVENTF_KEYUP,0 );
		}
	}
	else {
		img_focus( l );
	}

	return 1;
}


void CSelImgDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	ICSDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	//	if( ! m_bShow ) {
	//		m_bShow = TRUE;
	//		PostMessage( WM_RETROFOCUS, 1, SIMG_TKSEL );
	//	}
}

void CSelImgDlg::OnSize(UINT nType, int cx, int cy) 
{
	ICSDialog::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( ! m_bShow ) {
		m_bShow = TRUE;
		PostMessage( WM_RETROFOCUS, 1, SIMG_TKSEL );
	}	
}
#endif


#endif // end of _CPYIMG

