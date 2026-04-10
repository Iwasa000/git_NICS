// TkyEdaDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "DBInpSubFunc.h"

#include "icsdbedt.h"
#include "TkyEdaDlg.h"

#include "BrnTkyDef.h"


// CTkyEdaDlg ダイアログ

extern
int get_strcnt(char* str, int byte);

#define		KANA_LEN	4
#define		KANA_FMT	"%-4.4s"


int NewEdabanSearch( CDBZmSub* pzm, LPCTSTR kcod, int eda, int ketachk = 0 );



IMPLEMENT_DYNAMIC(CTkyEdaDlg, ICSDialog)

CTkyEdaDlg::CTkyEdaDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CTkyEdaDlg::IDD, pParent)
{
	m_MkEda = -1;
}

CTkyEdaDlg::~CTkyEdaDlg()
{
}


// 財務クラスのセット
void CTkyEdaDlg::Set_Class( CDBZmSub* pZm, CDBSyohi* pSy, DBKNREC* pKn, int tkcod )
{
	m_pZm = pZm;
	m_pSy = pSy;
	memcpy( &m_knrec, pKn, sizeof DBKNREC );

	m_tkcod = tkcod;
}

void	CTkyEdaDlg::Set_Info( CString& tky, CString& kana, int inieda, char* zan )
{
	m_svTky		= tky;
	m_svKana	= kana;
	m_inieda	= inieda;
	memcpy( m_Zan, zan, 6 );
}

void CTkyEdaDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSLINP8CTRL1, m_list);
}


BEGIN_MESSAGE_MAP(CTkyEdaDlg, ICSDialog)
	ON_MESSAGE( WM_RETROFOCUS, &CTkyEdaDlg::OnRetroFocus)
END_MESSAGE_MAP()


// CTkyEdaDlg メッセージ ハンドラ

BOOL CTkyEdaDlg::OnInitDialog()
{
	MakeEdaListInp();

	char tmp[32];

	sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( m_pZm->zvol->edcol ) );

	CWnd* pwnd = GetDlgItem( IDC_ICSDBEDT8CTRL1 );

	((CICSDBEDT *)pwnd)->SetMinval( "0" );
	((CICSDBEDT *)pwnd)->SetMaxval( tmp );
	((CICSDBEDT *)pwnd)->SetMaxLen( 0 );	//桁数設定(数字のときは 0)


	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	ICSDialog::OnInitDialogEX();

	SetEdabanData();
	InitEdabanCode();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//リスト入力設定
void CTkyEdaDlg::MakeEdaListInp()
{
	LINP_COLUMN LC[8];
	LINP_VCOLORINFO	LVCOL[8];
	int i;

	// 枝番
	LC[0].clm_kind = LINP_KIND_NUMERIC;
	LC[0].clm_max = m_pZm->zvol->edcol;
	LC[0].clm_attr = LINP_ATTR_CENTER| LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	switch( m_pZm->zvol->edcol ) {
	case 2:
		LC[0].clm_format = NUM_FMTZ2;		break;
	case 3:
		LC[0].clm_format = NUM_FMTZ3;		break;
	case 4:
		LC[0].clm_format = NUM_FMTZ4;		break;
	case 5:
		LC[0].clm_format = NUM_FMTZ5;		break;
	default:
		LC[0].clm_format = NUM_FMTZ;
		break;
	}

	LC[0].clm_width = 30;
	LC[0].clm_title = "枝番";
	LC[0].clm_tbcolor = -1;
	LC[0].clm_ttcolor = -1;

	LC[1].clm_kind = LINP_KIND_NUMERIC;
	LC[1].clm_max = 6;
	LC[1].clm_attr = LINP_ATTR_CENTER| LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	LC[1].clm_format = NUM_FMT;
	LC[1].clm_width = 30;
	LC[1].clm_title = "摘要";
	LC[1].clm_tbcolor = -1;
	LC[1].clm_ttcolor = -1;

	// 摘要
	LC[2].clm_kind = LINP_KIND_STRING;
	LC[2].clm_max = (m_TKYO_LEN/2);
	LC[2].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_CMODE | LINP_ATTR_READONLY;
	LC[2].clm_attr |= LINP_ATTR_ZENHAN;

	LC[2].clm_width = 95;
	LC[2].clm_title = "　枝番名称";
	LC[2].clm_tbcolor = -1;
	LC[2].clm_ttcolor = -1;

	// カナ
	LC[3].clm_kind = LINP_KIND_STRING;
	LC[3].clm_max = 4;
	LC[3].clm_attr = LINP_ATTR_ANK | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	LC[3].clm_width = 25;
	LC[3].clm_title = "　カナ";
	LC[3].clm_tbcolor = -1;
	LC[3].clm_ttcolor = -1;
	// スペース
	LC[4].clm_kind = LINP_KIND_STRING;
	LC[4].clm_max = 0;
	LC[4].clm_attr = LINP_ATTR_ANK | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	LC[4].clm_width = 8;

	LC[4].clm_title = "";
	LC[4].clm_tbcolor = -1;
	LC[4].clm_ttcolor = -1;

	//開始残高
	LC[5].clm_kind = LINP_KIND_KINGAKU;
	LC[5].clm_max = 0;
	LC[5].clm_attr = LINP_ATTR_BOTTOM | LINP_ATTR_RIGHT | LINP_ATTR_READONLY;;
	LC[5].clm_width = 90;

	LC[5].clm_title = "開始残高";
	LC[5].clm_format = "S,SSS,SSS,SSS,SS9";
	LC[5].clm_tbcolor = -1;
	LC[5].clm_ttcolor = -1;

#ifdef CLOSE
	//貸方金額
	LC[6].clm_kind = LINP_KIND_KINGAKU;
	LC[6].clm_max = 0;
	LC[6].clm_attr = LINP_ATTR_BOTTOM | LINP_ATTR_RIGHT | LINP_ATTR_READONLY;;
	LC[6].clm_width = 75;

	LC[6].clm_title = "貸方金額";
	LC[6].clm_format = "S,SSS,SSS,SSS,SS9";
	LC[6].clm_tbcolor = -1;
	LC[6].clm_ttcolor = -1;
#endif

	linp_setclm( IDC_ICSLINP8CTRL1, 6, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
	linp_horzgrid( IDC_ICSLINP8CTRL1, 5, CL_GRAY, LINP_GRID_SOLID, CICSListInp );

	for( i=0; i < 6; i++ ) {
		LVCOL[i].pos = i;
		LVCOL[i].color = CL_RED;
		LVCOL[i].type = LINP_GRID_SOLID;
	}

	linp_vartgrid( IDC_ICSLINP8CTRL1, 5, (LPUNKNOWN)LVCOL, CICSListInp );
}


// 枝番データをセット
void CTkyEdaDlg::SetEdabanData()
{
	DWORD tmp;
	tmp = m_knrec.kncod;
	CString kcod;

	CString str;
	char inpcd[16];

	if( m_knrec.knicod < 0 ) {
		strcpy_s( inpcd, sizeof inpcd, "(    )" );
	}
	else {
		sprintf_s( inpcd, sizeof inpcd, "(%04d)", m_knrec.knicod );
	}

	str.Format( "『%.14s%s』 登録枝番リスト", m_knrec.knnam, inpcd );
	GetDlgItem( IDC_STATIC_KMK )->SetWindowText( str );

	hl_rev( &tmp, 4 );
	kcod.Format( "%08x", tmp );

	ezdb_init(m_pZm);

	int cnt = m_pZm->ezrec->Find( kcod );

	m_daEda.RemoveAll();

	int j = 0;
	if( cnt > 0 ) {
		do {
			MakeEdabanLine( m_pZm->ezrec );
			m_daEda.Add( m_pZm->ezrec->ezecd );

		} while( ++j < cnt && m_pZm->ezrec->MoveNext() == 0 );
	}
}


// 一行データ 作成
void CTkyEdaDlg::MakeEdabanLine( CEZREC* pez )
{
	LINP_DATA	LD[10];
	COLORREF	ONCOLOR, OFFCOLOR, SY_ONCOLOR, SY_OFFCOLOR;
	char	pbuf[64], fmt[24];
	CString Symsg, Data;

	ONCOLOR = CL_BLACK;
	OFFCOLOR = CL_TKBACK;

	SY_ONCOLOR = ONCOLOR;
	SY_OFFCOLOR = OFFCOLOR;

	// 枝番コード
	LD[0].dat_attr = LINP_DISP_COLOR;
	LD[0].dat_fc = ONCOLOR;
	LD[0].dat_bc = OFFCOLOR;

	l_defn(0x16);

	set_codestr( pbuf, sizeof pbuf, pez->ezecd, m_pZm->zvol->edcol );
	l_input( LD[0].dat_numeric, pbuf );
	LD[0].dat_initflg = FALSE;

	// 摘要コード
	LD[1].dat_attr = LINP_DISP_COLOR;
	LD[1].dat_fc = ONCOLOR;
	LD[1].dat_bc = OFFCOLOR;

	if( pez->eztcd <= 0 )
		::ZeroMemory( pbuf, sizeof pbuf );
	else {
		sprintf_s( pbuf, sizeof pbuf, "%6d", pez->eztcd );
	}

	l_input( LD[1].dat_numeric, pbuf );
	LD[1].dat_initflg = (pez->eztcd <= 0) ? TRUE : FALSE;

	// 摘要名称
	::ZeroMemory( pbuf, sizeof pbuf );
	CTKREC* ptk, *tkrec;

	tkrec = NULL;

	if( pez->eztcd > 0 ) {
		tkdb_init( m_pZm );

		ptk = m_pZm->tkrec;

		ptk->MoveFirst();

		LD[2].dat_initflg = TRUE;

		if( ptk->st != -1 ) {
			do {
				if( ptk->tkcod == pez->eztcd ) {

					int n = get_strcnt( (char*)(LPCTSTR)ptk->tkname, 20 );
					sprintf_s( fmt, sizeof fmt, "%%.%ds", n );
					sprintf_s( pbuf, sizeof pbuf, fmt, ptk->tkname );
					LD[2].dat_initflg = FALSE;

					tkrec = ptk;
					break;
				}
			} while( ptk->MoveNext() == 0 );
		}
	}
	else {

		if( ! pez->ezname.IsEmpty() ) {
			int n = get_strcnt( (char*)(LPCTSTR) pez->ezname, 20 );

			sprintf_s( fmt, sizeof fmt, "%%.%ds", n );
			sprintf_s( pbuf, sizeof pbuf, fmt,  pez->ezname );

			LD[2].dat_initflg = FALSE;
		}
		else {
			LD[2].dat_initflg = TRUE;
		}
	}
	LD[2].dat_attr = LINP_DISP_COLOR;
	LD[2].dat_fc = ONCOLOR;
	LD[2].dat_bc = OFFCOLOR;
	LD[2].dat_string.Format( "%s", pbuf );

	// カナ50音
	LD[3].dat_attr = LINP_DISP_COLOR;
	LD[3].dat_fc = ONCOLOR;
	LD[3].dat_bc = OFFCOLOR;
	if( tkrec != NULL )	LD[3].dat_string.Format( KANA_FMT, tkrec->tkana );
	else				LD[3].dat_string = "";
	LD[3].dat_initflg = tkrec ? FALSE : TRUE;

	// 空白
	LD[4].dat_attr = LINP_DISP_COLOR;
	LD[4].dat_fc = ONCOLOR;
	LD[4].dat_bc = OFFCOLOR;
	LD[4].dat_initflg = TRUE;

	// 開始残
	char val[6];
	l_input( val, (char*)(LPCTSTR)pez->ezzan );

	LD[5].dat_attr = LINP_DISP_COLOR;
	LD[5].dat_fc = SY_ONCOLOR;
	LD[5].dat_bc = SY_OFFCOLOR;
	memcpy( LD[5].dat_numeric, val, 6 );
	LD[5].dat_initflg = FALSE;

	m_list.AddData( (LPUNKNOWN)LD, FALSE );
}

#ifdef OLD_CLOSE
// 枝番コードのサーチ
int CTkyEdaDlg::NewEdabanSearch( LPCTSTR kcod, int eda, int ketachk/*=0*/)
{
	CEZREC* pez;
	pez = m_pZm->ezrec;

	int kchk = 10;
	
	for( int i = 0; i < (m_pZm->zvol->edcol-1); i++ ) {
		kchk *= 10;
	}

	int neweda = eda;
	int result = -1;
	int cnt;

	cnt = 0;
	do {
		if( ! ketachk )	neweda++;

		//桁数オーバー
		if( (neweda / kchk) > 0 ) {
			break;
		}

		cnt = pez->Find( kcod, neweda );
		if( cnt <= 0 ) {
			result = neweda;
			break;
		}

	} while( cnt > 0 );

	if( result == -1 ) {
		//桁数オーバー
		if( (neweda / kchk) > 0 ) {
			// 1 から
			neweda = 1;

			for( ; neweda < eda; neweda++ ) {

				cnt = pez->Find( kcod, neweda );
				if( cnt <= 0 ) {
					result = neweda;
					break;
				}
			}
		}
	}

	return result;
}
#endif


//
// 枝番名称入力と一致するものを検索
//
int CTkyEdaDlg::EdaNameInpSearch( LPCTSTR kcod )
{
	CEZREC* pez;
	pez = m_pZm->ezrec;

	int eda = -1;
	int j = 0;
	int cnt = pez->Find( kcod );

	if( cnt > 0 ) {
		do
		{
			if( pez->ezkcd.CompareNoCase( kcod ) == 0 ) {
				if( m_svTky.Compare( pez->ezname ) == 0 ) {
					eda = pez->ezecd;
					break;
				}
			}
		} while( ++j < cnt && pez->MoveNext() == 0 );
	}

	return eda;
}


//枝番コードチェック
void CTkyEdaDlg::InitEdabanCode()
{
	CEZREC* pez;
	DWORD tmp;
	tmp = m_knrec.kncod;
	hl_rev( &tmp, 4 );
	CString kcod;
	kcod.Format( "%08x", tmp );
	int eda;
	VARIANT var;

	pez = m_pZm->ezrec;

	char buf[64];

	//初期枝番あり
	if( m_inieda >= 0 ) {
		m_MkEda = m_inieda;
		set_codestr( buf, sizeof buf, m_MkEda, m_pZm->zvol->edcol );
		var.pbVal = (BYTE*)buf;
	}
	else {
		//摘要セットの枝番がある
		if( pez->Find2( kcod, m_tkcod ) > 0 ) {
			eda = pez->ezecd;

			m_MkEda = eda;
			set_codestr( buf, sizeof buf, eda, m_pZm->zvol->edcol );
			var.pbVal = (BYTE*)buf;

			//開始残高を既存枝番
			l_input( m_Zan, (char*)(LPCTSTR)pez->ezzan );
		}
		else {
			//摘要コードの枝番が登録済み
			if( pez->Find( kcod, m_tkcod ) > 0 ) {
				//空き枝番をサーチ
				eda = NewEdabanSearch( m_pZm, kcod, m_tkcod );

				if( eda == -1 ) {
					//この科目に割り当て可能な枝番がありません。
					var.pbVal = NULL;
					m_MkEda = -1;
				}
				else {
					m_MkEda = eda;
					set_codestr( buf, sizeof buf, eda, m_pZm->zvol->edcol );
					var.pbVal = (BYTE*)buf;
				}
			}
			else {
				//枝番名称入力と一致しているものを検索する
				eda = EdaNameInpSearch(kcod);

				if( eda < 0 ) {
					//摘要コードでは、枝番桁数をオーバーしているかも
					eda = NewEdabanSearch( m_pZm, kcod, m_tkcod, 1 );
				}
				else {
					//既存枝番の開始残高
					l_input( m_Zan, (char*)(LPCTSTR)pez->ezzan );
				}

				if( eda == -1 ) {
					//この科目に割り当て可能な枝番がありません。
					var.pbVal = NULL;
					m_MkEda = -1;
				}
				else {
					m_MkEda = eda;
					set_codestr( buf, sizeof buf, eda, m_pZm->zvol->edcol );
					var.pbVal = (BYTE*)buf;
				}
			}
		}
	}

	DBdata_set( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	//枝番名称のセット
	sprintf_s( buf, sizeof buf, "%6d", m_tkcod );

	GetDlgItem( IDC_STATIC_TKCOD )->SetWindowText( buf );

	int n = get_strcnt( (char*)(LPCTSTR)m_svTky, 20 );
	char fmt[16];
	sprintf_s( fmt, sizeof fmt, "%%.%ds", n );
	sprintf_s( buf, sizeof buf, fmt, m_svTky );
	GetDlgItem( IDC_STATIC_EDNAM )->SetWindowText(buf);
//	GetDlgItem( IDC_STATIC_EDNAM )->SetWindowText(m_svTky);

	GetDlgItem( IDC_STATIC_KANA )->SetWindowText(m_svKana);

	//開始残高のセット
	var.pbVal = (BYTE*)m_Zan;
	DBdata_set( this, IDC_ICSDBEDT8CTRL2, &var, ICSDBEDT_TYPE_ARITH, 0 );
}


BEGIN_EVENTSINK_MAP(CTkyEdaDlg, ICSDialog)
ON_EVENT(CTkyEdaDlg, IDC_ICSDBEDT8CTRL1, 1, CTkyEdaDlg::TerminationIcsdbedt8ctrl1, VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CTkyEdaDlg, IDC_ICSDBEDT8CTRL2, 1, CTkyEdaDlg::TerminationIcsdbedt8ctrl2, VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CTkyEdaDlg, IDC_ICSLINP8CTRL1, 4, CTkyEdaDlg::KeyInIcslinp8ctrl1, VTS_I2 VTS_I2 VTS_I2)
ON_EVENT(CTkyEdaDlg, IDC_ICSLINP8CTRL1, 7, CTkyEdaDlg::LButtonDblClickIcslinp8ctrl1, VTS_I2)
ON_EVENT(CTkyEdaDlg, IDC_ICSDBEDT8CTRL1, 2, CTkyEdaDlg::KillFocusIcsdbedt8ctrl1, VTS_I4)
END_EVENTSINK_MAP()


//枝番ターミネーション
void CTkyEdaDlg::TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst)
{
	VARIANT var;

	if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			set_focus( IDCANCEL );
		}
		else {
			set_focus( IDC_ICSDBEDT8CTRL2 );
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_RIGHT:
			set_focus( IDC_ICSDBEDT8CTRL2 );
			break;
		case VK_LEFT :
		case VK_F2 :
			set_focus( IDCANCEL );
			break;

		case VK_UP:
			m_list.LineMove(-1);
			set_focus( IDC_ICSDBEDT8CTRL1 );
			break;
		case VK_DOWN:
			m_list.LineMove(1);
			set_focus( IDC_ICSDBEDT8CTRL1 );
			break;

		case VK_PRIOR:
			m_list.PageMove(-1);
			set_focus( IDC_ICSDBEDT8CTRL1 );
			break;
		case VK_NEXT:
			m_list.PageMove(1);
			set_focus( IDC_ICSDBEDT8CTRL1 );
			break;
		}
	}

}

//枝番 kill focus
void CTkyEdaDlg::KillFocusIcsdbedt8ctrl1(long inplen)
{
	EdaInpCheck();
}


//開始残ターミネーション
void CTkyEdaDlg::TerminationIcsdbedt8ctrl2(long nChar, long inplen, long kst)
{
	VARIANT var;

	if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT8CTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			set_focus( IDC_ICSDBEDT8CTRL1 );
		}
		else {
			set_focus( IDOK );
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_DOWN :
		case VK_RIGHT:
			set_focus( IDOK );
			break;
		case VK_LEFT :
		case VK_F2 :
			set_focus( IDC_ICSDBEDT8CTRL1 );
			break;
		}
	}
}



// フォーカスセット
void CTkyEdaDlg::set_focus( int id )
{
	PostMessage( WM_RETROFOCUS, id );
}

// フォーカスセット
LRESULT CTkyEdaDlg::OnRetroFocus(WPARAM wParam, LPARAM lParam)
{
	c_set( wParam );

	return 1;
}

BOOL CTkyEdaDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		switch( pMsg->wParam ) {
		case VK_ESCAPE:
			EndDialog( IDCANCEL );
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}



void CTkyEdaDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	VARIANT var;
	char buf[64] = {0};

	DBdata_get(this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	strcpy_s( buf, sizeof buf, (const char*)var.pbVal );
	if( ! buf[0] ) {
		ICSMessageBox( "枝番が指定されていません！",0,0,0,this );
		set_focus(IDC_ICSDBEDT8CTRL1);
		return;
	}

	int n;
	n = atoi( buf );
	m_MkEda = n;

	DBdata_get(this, IDC_ICSDBEDT8CTRL2, &var, ICSDBEDT_TYPE_ARITH, 0 );
	memcpy( m_Zan, var.pbVal, 6 );


	ICSDialog::OnOK();
}


void	CTkyEdaDlg::GetTkyEdaData(int& mk_eda, char* zan )
{
	mk_eda = m_MkEda;
	memcpy( zan, m_Zan, 6 );
}


//
//	LPCTSTR kcod		科目コード
//	int		eda			作成する基本枝番コード(この次より探す。)
//	int		ketachk		桁チェック(1)のときは、 eda をインクリメントしない。
//
//
int NewEdabanSearch( CDBZmSub* pzm, LPCTSTR kcod, int eda, int ketachk/*=0*/)
{
	CEZREC* pez;
	pez = pzm->ezrec;

	int kchk = 10;
	
	for( int i = 0; i < (pzm->zvol->edcol-1); i++ ) {
		kchk *= 10;
	}

	int neweda = eda;
	int result = -1;
	int cnt;

	cnt = 0;
	do {
		if( ! ketachk )	neweda++;

		//桁数オーバー
		if( (neweda / kchk) > 0 ) {
			break;
		}

		cnt = pez->Find( kcod, neweda );
		if( cnt <= 0 ) {
			result = neweda;
			break;
		}

	} while( cnt > 0 );

	if( result == -1 ) {
		//桁数オーバー
		if( (neweda / kchk) > 0 ) {
			// 0 から
			neweda = 0;
			if( eda > kchk )	eda = kchk;

			for( ; neweda < eda; neweda++ ) {

				cnt = pez->Find( kcod, neweda );
				if( cnt <= 0 ) {
					result = neweda;
					break;
				}
			}
		}
	}

	return result;
}

void CTkyEdaDlg::KeyInIcslinp8ctrl1(short nChar, short previndex, short index)
{

	switch( nChar ) {
	case VK_RETURN:
		set_focus(IDC_ICSDBEDT8CTRL1); 
		break;
	case VK_TAB:
		if( GetKeyState( VK_SHIFT ) < 0 ) {
			set_focus(IDCANCEL); 
		}
		else {
			set_focus(IDC_ICSDBEDT8CTRL1); 
		}
		break;

	case VK_F2:
		set_focus(IDCANCEL); 
		break;
	}
}

void CTkyEdaDlg::LButtonDblClickIcslinp8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DWORD tmp;
	tmp = m_knrec.kncod;
	CString kcod;

	hl_rev( &tmp, 4 );
	kcod.Format( "%08x", tmp );

	if( m_pZm->ezrec->Find( kcod, m_daEda[index] ) > 0 ){
		VARIANT var;
		char	buf[64];

		m_MkEda = m_pZm->ezrec->ezecd;
		set_codestr( buf, sizeof buf, m_MkEda, m_pZm->zvol->edcol );
		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		//開始残高セット
		l_input( m_Zan, (char*)(LPCTSTR)m_pZm->ezrec->ezzan );
		var.pbVal = (BYTE*)m_Zan;
		DBdata_set( this, IDC_ICSDBEDT8CTRL2, &var, ICSDBEDT_TYPE_ARITH, 0 );
	}
}



void CTkyEdaDlg::EdaInpCheck()
{
	DWORD tmp;
	tmp = m_knrec.kncod;
	CString kcod;

	hl_rev( &tmp, 4 );
	kcod.Format( "%08x", tmp );
	VARIANT var;
	char	buf[64];

	var.pbVal = (BYTE*)buf;
	DBdata_get( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	strcpy_s( buf, sizeof buf, (const char*)var.pbVal );

	if( buf[0] != 0 ) {
		int eda = atoi( buf );
		if( m_MkEda != eda ) {
			if( m_pZm->ezrec->Find( kcod, eda ) > 0 ){
				m_MkEda = m_pZm->ezrec->ezecd;
				set_codestr( buf, sizeof buf, m_MkEda, m_pZm->zvol->edcol );
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

				//開始残高セット
				l_input( m_Zan, (char*)(LPCTSTR)m_pZm->ezrec->ezzan );
				var.pbVal = (BYTE*)m_Zan;
				DBdata_set( this, IDC_ICSDBEDT8CTRL2, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
		}
	}
}

