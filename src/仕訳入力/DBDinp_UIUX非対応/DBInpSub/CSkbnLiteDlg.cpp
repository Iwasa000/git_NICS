// CSkbnLiteDlg.cpp : 実装ファイル
//

#include "stdafx.h"

#include "resource.h"
#include "DBInpSubFunc.h"

#include "CSkbnLiteDlg.h"
#include "../DBDinp/ocxdef.h"


// CSkbnLiteDlg ダイアログ
#define TAIKA_VER

int SYZLITE_IDX[] = { SY_ZRITU_PN, SY_SZKBN_PN,SY_INV_PN,SY_MENZEI_PN, SY_MENWARI_PN, -1 };

#define PN_MAX		((sizeof(SYZLITE_IDX)/sizeof(int))-1)


static struct _InpCtl INP_CTL2[PN_MAX] = {
	{IDC_ICSDBEDTCTRL2,0,0,0,-1},	// 税率
	{IDC_ICSDBEDTCTRL3,0,0,0,-1},	// 税区分
	{IDC_ICSDBEDT_INV,0,0,0,-1},	// インボイス対応 登録番号
	{IDC_CHECK_MENZEI,0,0,0,-1},	// インボイス対応 免税事業者からの課税仕入れ
	{IDC_ICSDBEDT_MEN,0,0,0,-1}	// インボイス対応 免税事業者　控除割合
};

static int STATIC_CTL[PN_MAX] = {
	IDC_STATIC2,IDC_STATIC3,IDC_STATIC8,IDC_STATIC18,IDC_STATIC20
};

// Now ポジション
static int NOW_PN = 0;
//直前のポジション
static int PREV_PN = 0;

// 消費税アイテムコードよりインデックス
static int PNitemToIndex(int PNitm)
{
	int idx = -1;
	for( int i = 0; i < PN_MAX; i++ ) {
		if( SYZLITE_IDX[i] == PNitm ) {
			idx = i;
			break;
		}
	}

	return idx;
}

IMPLEMENT_DYNAMIC(CSkbnLiteDlg, CDialog)

CSkbnLiteDlg::CSkbnLiteDlg(CWnd* pParent /*=NULL*/, BOOL bToku /*=FALSE*/, BOOL bResize/*=TRUE*/, bool bInvMaster /*=false*/, 
							bool bInvUse/*=false*/, bool bInvDisp/*=false*/, int koujo8050/* = SYZ_MEN_NOT*/)
	: ICSDialog(CSkbnLiteDlg::IDD, pParent)
{
	m_bResize = bResize;

	m_THIS_DIALOG_OK = FALSE;
	m_pParent = pParent;
	m_bResize = bResize;

	m_Dispflg = FALSE;
	m_DlgRect.SetRectEmpty();

	m_pDBDta = NULL;
	m_pLineTxt = NULL;
	m_pSmpk = NULL;

	m_bInvMaster = bInvMaster;
	m_bInvUse = bInvUse;
	m_bInvDisp = bInvDisp;

	// インボイス対応
	m_bMsgDsp = FALSE;
	m_Koujo80_50 = koujo8050;

	m_bIgnoreFocus = FALSE;

	m_brBk.CreateSolidBrush(RGB_YELLOW);	// 背景(黄)
	m_brDefBk.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));

	Create(CSkbnLiteDlg::IDD, pParent);

	//アクセラレータテーブルロード
//	m_hAccelTable = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CSkbnLiteDlg::~CSkbnLiteDlg()
{
}

void CSkbnLiteDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSSELCTRL1, m_Select);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_DBedt2);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL3, m_DBedt3);
	DDX_Control(pDX, IDC_ICSDBEDT_INV, m_DBedtInv);
	DDX_Control(pDX, IDC_CHECK_MENZEI, m_MenzeiCheck);
	DDX_Control(pDX, IDC_ICSDBEDT_MEN, m_MenWari);
	DDX_Control(pDX, IDC_ICSDBEDT_INVDISP, m_DBedtInvDisp);
}


BEGIN_MESSAGE_MAP(CSkbnLiteDlg, ICSDialog)
	ON_MESSAGE(WM_RETROFOCUS, RetroFocus)
	ON_MESSAGE(WM_MYMESSAGE, OnMyMessage)
	ON_BN_CLICKED(IDC_CHECK_MENZEI, &CSkbnLiteDlg::OnBnClickedMenzei)
	ON_BN_SETFOCUS(IDC_CHECK_MENZEI, &CSkbnLiteDlg::OnSetfocusCheckMenzei)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSkbnLiteDlg メッセージ ハンドラー

BOOL CSkbnLiteDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	MoveInvCtrl();
	//	ControlInit(147,53,-1);
	ICSColorInfo ci;
	((ICSWinApp*)AfxGetApp())->GetStanderdColor(ci);

	if( ci.m_swOnOff > 0 ) {
		// 科目選択画面背景色の設定
		m_Select.SetBackColor(ci.PageKmkBackColor());
		// 科目選択画面文字色の設定
		m_Select.SetForeColor(ci.PageKmkTextColor());
		// 科目選択画面枠の色
		m_Select.SetLineColor(RGB_GRAY);
	}
	else {
		// 科目選択画面背景色の設定
		m_Select.SetBackColor(RGB_VAL32_BACK);
		// 科目選択画面文字色の設定
		m_Select.SetForeColor(RGB_VAL32_FORE);
		// 科目選択画面枠の色
		m_Select.SetLineColor(RGB_GRAY);
	}
	m_Select.SetNumber(TRUE);

	// TODO: この位置に初期化の補足処理を追加してください
	m_DBedt2.ImeEndMode(FALSE);
	m_DBedt3.ImeEndMode(FALSE);
	m_DBedtInv.ImeEndMode( TRUE );

	m_DBedt2.EnableDelete(0);
	m_DBedt3.EnableDelete(0);
	m_DBedtInv.EnableDelete(0);
	m_DBedtInv.EnableHome(1);
	m_DBedtInv.SetKanaTerm(TRUE, INVKANA_MAX, ICSDBEDT_KTYPE_KANA);

	// １文字単位の入力対応
	CRect rect;
	GetDlgItem(IDC_ICSDBEDT_INV)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_ICSDBEDT_INVDISP)->SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	// インボイス対応
	ShowInvCtrl();

	// リサイズ前を取得
	GetWindowRect(&m_orgRect);

	ModifyStyle(0, WS_CLIPSIBLINGS);

	invmng.m_hWnd = m_DBedtInv.m_hWnd;
	invmng.m_nID = IDC_ICSDBEDT_INV;
	invmng.m_pDlg = this;

	if( m_bResize ) {
		ICSDialog::OnInitDialogEX();
	}
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSkbnLiteDlg::PostNcDestroy()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	ICSDialog::PostNcDestroy();

	delete this;
}


BEGIN_EVENTSINK_MAP(CSkbnLiteDlg, ICSDialog)
	//{{AFX_EVENTSINK_MAP(CSkbnIn)
	ON_EVENT(CSkbnLiteDlg, IDC_ICSSELCTRL1, 1 /* Selected */, OnSelectedIcsselctrl, VTS_I2 VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CSkbnLiteDlg, IDC_ICSDBEDTCTRL2, IDC_ICSDBEDT_INVDISP, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl, VTS_I4)
	ON_EVENT_RANGE(CSkbnLiteDlg, IDC_ICSDBEDTCTRL2, IDC_ICSDBEDT_INVDISP, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CSkbnLiteDlg, IDC_ICSDBEDTCTRL2, IDC_ICSDBEDT_MEN, 1 /* Termination */, OnTerminationIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CSkbnLiteDlg, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDT_MEN, 5, ImeEndCompositionIcsdbedtctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()


void CSkbnLiteDlg::OnSelectedIcsselctrl(short position, long Number)
{
	SetSkbnSelNo(Number);
}


LRESULT	CSkbnLiteDlg::RetroFocus(WPARAM wParam, LPARAM lParam)
{
	switch( wParam ) {
	case SY_ZRITU_PN:
	case SY_SZKBN_PN:
	case SY_INV_PN:
	case SY_MENZEI_PN:
	case SY_MENWARI_PN:
		c_set(getINP_CTL(wParam)->IDC_X);
		break;
	}
	return 1;
}

void CSkbnLiteDlg::OnSetFocusIcsdbedtctrl(UINT ID)
{
	if( m_THIS_DIALOG_OK == FALSE )
		return;

	struct _SY_MSG_TBL *psmt;
	int pn = -1;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL2:	// 税率
		pn = SY_ZRITU_PN;		psmt = m_pSmpk->SY_MSG_RITU;
		break;
	case IDC_ICSDBEDTCTRL3:	// 税区分
		pn = SY_SZKBN_PN;		psmt = m_pSmpk->SY_MSG_ZEI;
		break;
	case IDC_ICSDBEDT_INV:	// 登録番号
		m_DBedtInvDisp.ShowWindow(SW_HIDE);
		pn = SY_INV_PN;
		psmt = NULL;
		break;
	case IDC_ICSDBEDT_MEN:
		pn = SY_MENWARI_PN;
		psmt = m_pSmpk->SY_MSG_EXEMPT;
		break;

	case IDC_ICSDBEDT_INVDISP:
		m_DBedtInvDisp.ShowWindow(SW_HIDE);
		set_focus(SY_INV_PN);
		return;
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	if( pn != SY_INV_PN ) {
		invmng.InvNoSearch(NULL);
	}

	if( pn == -1 )
		return;

	if( getINP_CTL(pn)->INP_ena == FALSE )
	{
		set_focus(get_nowpn());
		return;
	}

	set_nowpn(pn);
	kbn_select_make(psmt, get_nowpn());
}

void CSkbnLiteDlg::OnKillFocusIcsdbedtctrl(UINT ID, long inplen)
{
	if( m_bIgnoreFocus ) {
		ID = 0;
	}
	switch( ID ) {
	case IDC_ICSDBEDTCTRL2:
		OnTerminationDBEdt2(0, inplen, 0);
		break;
	case IDC_ICSDBEDTCTRL3:
		OnTerminationDBEdt3(0, inplen, 0);
		break;
	case IDC_ICSDBEDT_INV:	// 登録番号
		OnTerminationDBEdtInv( 0, inplen, 0 );
		WINDOWPLACEMENT place;
		m_DBedtInvDisp.GetWindowPlacement(&place);
		m_DBedtInvDisp.SetWindowPos(&CWnd::wndTop, place.rcNormalPosition.left, place.rcNormalPosition.top, place.rcNormalPosition.right - place.rcNormalPosition.left, place.rcNormalPosition.bottom - place.rcNormalPosition.top, SWP_SHOWWINDOW);
		break;
	case IDC_ICSDBEDT_MEN:	// 免税事業者控除割合
		OnTerminationDBEdtMen(0, inplen, 0);
		break;
	default:
		ASSERT(FALSE);
		return;
	}
}

void CSkbnLiteDlg::OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst)
{
	switch( ID ) {
	case IDC_ICSDBEDTCTRL2:
		OnTerminationDBEdt2(nChar, inplen, kst);
		break;
	case IDC_ICSDBEDTCTRL3:
		OnTerminationDBEdt3(nChar, inplen, kst);
		break;
	case IDC_ICSDBEDT_INV:	// 登録番号
		OnTerminationDBEdtInv( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDT_MEN:	// 免税事業者　控除割合
		OnTerminationDBEdtMen(nChar, inplen, kst);
		break;
	default:
		ASSERT(FALSE);
		return;
	}

}

int	CSkbnLiteDlg::OnTerminationDBEdt2(long nChar, long inplen, long kst)
{
	int i, pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
		//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get(this, getINP_CTL(SY_ZRITU_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				strcpy_s(pbuf, sizeof pbuf, (char*)var.pbVal);

				for( i = 0; pbuf[i]; ++i )
				{
					if( isdigit(pbuf[i]) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo(atoi(pbuf));
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn(get_nowpn())) != -1 ) {
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, VK_LEFT);
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		if( (pn = get_prevpn(get_nowpn())) == SY_SYZSWKBN_PN )
		{
			set_focus(pn);
			return 0;
		}
		to_parent(m_pParent, nChar);
		return 0;
	case VK_RIGHT:
		if( (pn = get_nextpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		to_parent(m_pParent, nChar);
		return 0;
	case VK_DOWN:	// 下 矢印で 売上／仕入区分 へ
		if( (pn = get_nextpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		to_parent(m_pParent, nChar);
		return 0;
	case VK_RETURN:
		if( (pn = get_prevpn(get_nowpn())) == SY_SZKBN_PN )
		{
			set_focus(pn);
			return 0;
		}
	case VK_TAB:
		//	case VK_F3:
		if( (pn = get_nextpn(get_nowpn(), nChar)) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else
		{
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;

	case VK_UP:
	case VK_F2:
		if( (pn = get_prevpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else
		{
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}


	return 0;
}

int	CSkbnLiteDlg::OnTerminationDBEdt3(long nChar, long inplen, long kst)
{
	int i, pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
		//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get(this, getINP_CTL(SY_SZKBN_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				strcpy_s(pbuf, sizeof pbuf, (char*)var.pbVal);

				for( i = 0; pbuf[i]; ++i )
				{
					if( isdigit(pbuf[i]) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo(atoi(pbuf));
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn(get_nowpn())) != -1 ) {
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, VK_LEFT);
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_RIGHT:
		if( (pn = get_nextpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, nChar);
		}
		return 0;

	case VK_RETURN:
		if( (pn = get_prevpn(get_nowpn())) == SY_SZKBN_PN )
		{
			set_focus(pn);
			return 0;
		}
	case VK_TAB:
	case VK_DOWN:
		//	case VK_F3:
		if( (pn = get_nextpn(get_nowpn(), nChar)) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else
		{
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;
	case VK_UP:
		if( (pn = get_prevpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else
		{
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( (pn = get_prevpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else
		{
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}

	return 0;
}


// IDC get
struct _InpCtl *CSkbnLiteDlg::getINP_CTL(int pn)
{
	int idx = PNitemToIndex(pn);

	if( idx >= 0 && idx < PN_MAX )
		return(&INP_CTL2[idx]);
	else
		return(&INP_CTL2[0]);	//ERROR
}

void CSkbnLiteDlg::set_fastfocus(BOOL modefy_sg, int pn)
{
	m_MODEFY_OK = modefy_sg;
	m_THIS_DIALOG_OK = TRUE;

	PostMessage(WM_RETROFOCUS, pn);

	for( int n = 0; n < PN_MAX; n++ ) {
		if( GetDlgItem(STATIC_CTL[n]) ) {
			GetDlgItem(STATIC_CTL[n])->Invalidate();
		}
	}
}

BOOL CSkbnLiteDlg::skbn_dataset(int *set_pn)
{
	BOOL ret;
	ret = sydata_set(set_pn);

	return ret;
}


//
//	消費税データセット
//		int* set_pn	.... 入力可能な項目をビットで知らせる
//
//
BOOL CSkbnLiteDlg::sydata_set(int *set_pn)
{
	DBGFUNCTION;

	struct _InpCtl* p_inp;
	VARIANT var;
	char	tmp[64];
	int idc, typ;
	int ritu_sw, zeikbn_sw, uri_sw, zei_sw, swk_sw, taika_sw, toku_sw, menzei_sw;

	ritu_sw = zeikbn_sw = uri_sw = zei_sw = swk_sw = taika_sw = toku_sw = menzei_sw = 0;

	DSIGN_TRACE(m_Litesub->m_dbdata.dsign);

	m_pSy->SyMsg(m_pSmpk);
	struct _SY_MSG_TBL* psmt;

	//エディット初期化
	var.pbVal = NULL;

	int idx;
	for( idx = 0; idx < PN_MAX; idx++ ) {
		int pn = SYZLITE_IDX[idx];

		p_inp = getINP_CTL(pn);
		idc = p_inp->IDC_X;
		var.pbVal = NULL;
		typ = ICSDBEDT_TYPE_STRING;

		// インボイス対応
		if( pn != SY_MENZEI_PN ) {
			DBdata_set( this, idc, &var, typ, 0 );
		}
		//if( pn != SY_TAIKA_PN )
		if( /*( pn != SY_INV_PN ) &&*/ ( pn != SY_MENZEI_PN )) {
			DBSetDispString(this, idc, NULL);
		}
		//表示のみの登録番号
		if( pn == SY_INV_PN ) {
			idc = IDC_ICSDBEDT_INVDISP;
			DBdata_set(this, idc, &var, typ, 0);
			DBSetDispString(this, idc, NULL);
		}
	}

	swk_sw = m_pSmpk->SY_MSG_SWKBN->sel_no;
	ritu_sw = m_pSmpk->SY_MSG_RITU->sel_no;
	zeikbn_sw = m_pSmpk->SY_MSG_ZEI->sel_no;
	uri_sw = m_pSmpk->SY_MSG_URISIRE->sel_no;
#ifdef TAIKA_VER
	taika_sw = m_pSmpk->SY_MSG_TAIKAKBN->sel_no;
#endif
	toku_sw = m_pSmpk->SY_MSG_TOKTEI->sel_no;		// 特定収入
	menzei_sw = m_pSmpk->SY_MSG_EXEMPT->sel_no;

//MyTrace("-------> CSkbnLiteDlg::sydata_set zeikbn_sw(%d), menzei_sw = %d\n", zeikbn_sw, menzei_sw );


	if( !ritu_sw && !zeikbn_sw && !uri_sw && !swk_sw && !taika_sw && !toku_sw ) {
		if( set_pn != NULL )	*set_pn = 0;
		return FALSE;
	}
	//
	if( ritu_sw && !zeikbn_sw ) {
		//どちらかが仮受・仮払の時は、表示しない
		if( m_Litesub->karisyz_check(m_pDBDta) ) {
			if( set_pn != NULL )	*set_pn = 0;
			return FALSE;
		}
	}

	//消費税仕訳区分
#ifdef CLOSE
	p_inp = getINP_CTL(SY_SYZSWKBN_PN);
	idc = p_inp->IDC_X;
	GetDlgItem(idc)->EnableWindow(TRUE);
	psmt = m_pSmpk->SY_MSG_SWKBN;
	if( swk_sw != 0 )
	{
		GetDlgItem(IDC_STATIC1)->EnableWindow(TRUE);
		sprintf_s(tmp, sizeof tmp, "%d", psmt->sel_no);
		var.pbVal = (BYTE*)tmp;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, idc, psmt->msg_tbl[psmt->sel_no - 1]->msg);
		p_inp->INP_ena = TRUE;

		// 最初の 消費税仕訳区分 サイン
		if( set_pn != NULL ) {
			m_defSwkKbn = psmt->msg_tbl[psmt->sel_no - 1]->code;
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);
		var.pbVal = NULL;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		GetDlgItem(idc)->EnableWindow(FALSE);
		p_inp->INP_ena = FALSE;
	}
#endif
	// 税率
	GetDlgItem(IDC_STATIC2)->SetWindowText("税 率");
	p_inp = getINP_CTL(SY_ZRITU_PN);
	idc = p_inp->IDC_X;
	GetDlgItem(idc)->EnableWindow(TRUE);
	psmt = m_pSmpk->SY_MSG_RITU;
	if( ritu_sw != 0 )
	{
		GetDlgItem(IDC_STATIC2)->EnableWindow(TRUE);
		sprintf_s(tmp, sizeof tmp, "%d", psmt->sel_no);
		var.pbVal = (BYTE*)tmp;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, idc, psmt->msg_tbl[psmt->sel_no - 1]->msg);
		p_inp->INP_ena = TRUE;
	}
	else
	{
		GetDlgItem(IDC_STATIC2)->EnableWindow(FALSE);

		var.pbVal = NULL;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		GetDlgItem(idc)->EnableWindow(FALSE);
		p_inp->INP_ena = FALSE;
	}
	// 税区分
	p_inp = getINP_CTL(SY_SZKBN_PN);
	idc = p_inp->IDC_X;
	GetDlgItem(idc)->EnableWindow(TRUE);
	psmt = m_pSmpk->SY_MSG_ZEI;
	if( zeikbn_sw != 0 )
	{
		GetDlgItem(IDC_STATIC3)->EnableWindow(TRUE);
		sprintf_s(tmp, sizeof tmp, "%d", psmt->sel_no);
		var.pbVal = (BYTE*)tmp;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, idc, psmt->msg_tbl[psmt->sel_no - 1]->msg);
		p_inp->INP_ena = TRUE;
	}
	else
	{
		GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);

		var.pbVal = NULL;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		GetDlgItem(idc)->EnableWindow(FALSE);
		p_inp->INP_ena = FALSE;
	}
#ifdef CLOSE
	//売上／仕入区分
	p_inp = getINP_CTL(SY_URISIRE_PN);
	idc = p_inp->IDC_X;
	GetDlgItem(idc)->EnableWindow(TRUE);
	psmt = m_pSmpk->SY_MSG_URISIRE;

	if( uri_sw )
	{

		if( m_pSy->IsKobetsuMaster() ) {
			GetDlgItem(IDC_STATIC4)->SetWindowText("仕入区分");
		}
		else {
			GetDlgItem(IDC_STATIC4)->SetWindowText("売上区分");
		}

		GetDlgItem(IDC_STATIC4)->EnableWindow(TRUE);
		sprintf_s(tmp, sizeof tmp, "%d", psmt->sel_no);
		var.pbVal = (BYTE*)tmp;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, idc, psmt->msg_tbl[psmt->sel_no - 1]->msg);
		p_inp->INP_ena = TRUE;
	}
	else
	{
		GetDlgItem(IDC_STATIC4)->EnableWindow(FALSE);
		var.pbVal = NULL;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		GetDlgItem(idc)->EnableWindow(FALSE);
		p_inp->INP_ena = FALSE;
	}
#endif

	m_THIS_DIALOG_OK = TRUE;

	if( set_pn != NULL ) {
		*set_pn = 0;
		if( swk_sw )	*set_pn |= 0x01 << SY_SYZSWKBN_PN;
		if( ritu_sw )	*set_pn |= 0x01 << SY_ZRITU_PN;
		if( zeikbn_sw ) *set_pn |= 0x01 << SY_SZKBN_PN;
		if( uri_sw )	*set_pn |= 0x01 << SY_URISIRE_PN;
		if( taika_sw ) {
			if( getINP_CTL(SY_TAIKA_PN)->INP_ena )
				*set_pn |= 0x01 << SY_TAIKA_PN;
			else
				*set_pn |= 0x01 << SY_TAISEL_PN;
		}
		if( toku_sw ) {
			*set_pn |= 0x01 << SY_TOKUS_PN;
		}
	}
	// インボイス対応
	if( m_bInvMaster && m_bInvUse ) {
		if( m_bInvDisp ) {
			// 登録番号
			p_inp = getINP_CTL( SY_INV_PN );
			idc = p_inp->IDC_X;

			int idc2 = IDC_ICSDBEDT_INVDISP;

			if( menzei_sw ) {
				GetDlgItem(idc)->EnableWindow(TRUE);
				GetDlgItem(idc2)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC18)->EnableWindow(TRUE);

				CString tmpInvno;
				tmpInvno = m_Litesub->m_dbdata.invno;
				if( !tmpInvno.IsEmpty() ) {
					tmpInvno.Replace(_T("T"), _T(""));
					sprintf_s(tmp, sizeof(tmp), _T("%s"), tmpInvno);
					var.pbVal = (BYTE*)tmp;
					DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
					sprintf_s(tmp, sizeof(tmp), _T("%s"), m_Litesub->m_dbdata.invno);
					var.pbVal = (BYTE*)tmp;
					DBSetDispString(this, IDC_ICSDBEDT_INVDISP, tmp);
				}
				else {
					memset(tmp, '\0', sizeof(tmp));
					var.pbVal = (BYTE*)tmp;
					DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
					DBSetDispString(this, IDC_ICSDBEDT_INVDISP, tmp);
				}
				p_inp->INP_ena = TRUE;
			}
			else {
				GetDlgItem(idc)->EnableWindow(FALSE);
				GetDlgItem(idc2)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC18)->EnableWindow(FALSE);
				memset(tmp, '\0', sizeof(tmp));
				var.pbVal = (BYTE*)tmp;
				DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				DBSetDispString(this, IDC_ICSDBEDT_INVDISP, tmp);
				p_inp->INP_ena = FALSE;
			}
		}
		else {
			p_inp = getINP_CTL(SY_INV_PN);
			p_inp->INP_ena = FALSE;
		}

		// 免税事業者からの課税仕入れ
		p_inp = getINP_CTL( SY_MENZEI_PN );
		idc = p_inp->IDC_X;

		if( menzei_sw ) {
			GetDlgItem( idc )->EnableWindow( TRUE );
			int menCheck = BST_UNCHECKED;
			if( menzei_sw == 2 || menzei_sw == 3 ) {
				menCheck = BST_CHECKED;
			}
			m_MenzeiCheck.SetCheck( menCheck );
			MenWariDBEdit(menCheck == BST_CHECKED ? TRUE : FALSE);

			p_inp->INP_ena = TRUE;
		}
		else {
			GetDlgItem( idc )->EnableWindow( FALSE );
			m_MenzeiCheck.SetCheck( BST_UNCHECKED );
			MenWariDBEdit(FALSE);
			p_inp->INP_ena = FALSE;
		}
	}
	else {
		int ctrlId = 0;
		for( int i = 0; SYZLITE_IDX[i] != -1; i++ ) {
			ctrlId = SYZLITE_IDX[i];
			if( ctrlId >= SY_INV_PN && ctrlId <= SY_MENWARI_PN ) {
				p_inp = getINP_CTL(ctrlId);
				p_inp->INP_ena = FALSE;
			}
		}
	}

	return TRUE;
}

//
//区分選択作成
void CSkbnLiteDlg::kbn_select_make(struct _SY_MSG_TBL *sel, int pn)
{
	int i;
	static CString str[MaxRec];

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 1 /*skbn_sw == SYFNCRET_SEL_SET */ )
	{
		switch( pn )
		{
		case SY_SYZSWKBN_PN:
		case SY_ZRITU_PN:
		case SY_SZKBN_PN:
		case SY_URISIRE_PN:
		case SY_TAISEL_PN:
		case SY_TAIKA_PN:
		case SY_TOKUS_PN:
		case SY_MENWARI_PN:
			if( sel ) {
				if( pn == SY_MENWARI_PN ) {
					int idx = 0;
					for( i = 0; i < MaxRec; ++i ) {
						if( i < sel->trn ) {
							if( sel->msg_tbl[i]->code == CD_SP_NO_EXEMPT )
								continue;
							if( m_Koujo80_50 == SYZ_MEN_80 || m_Koujo80_50 == SYZ_MEN_8050 || m_Koujo80_50 == SYZ_MEN_50 ) {
								if( sel->msg_tbl[i]->code == CD_SP_EXEMPT ) {
									str[idx++] = "80% 控除";
								}
							}
							if( m_Koujo80_50 == SYZ_MEN_8050 || m_Koujo80_50 == SYZ_MEN_50 ) {
								if( sel->msg_tbl[i]->code == CD_SP_EXEMPT_50 ) {
									str[idx++] = "50% 控除";
								}
							}
						}
						else {
							break;
						}
					}
					for( i = idx; i < MaxRec; i++ ) {
						str[i].Empty();
					}
				}
				else {
					for( i = 0; i < sel->trn && i < MaxRec; ++i ) {
						if( pn == SY_SZKBN_PN ) {
							if( sel->msg_tbl[i]->code == CD_BEKKI ) {
								str[i].Empty();
								continue;
							}
							else if( sel->msg_tbl[i]->code == CD_SOTOZ ) {
								//外税は基本、非表示。 SetDispMode()を呼び出したプロのみ表示
								if( !m_Litesub->SotoNotCut() ) {
									str[i].Empty();
									continue;
								}
							}
						}
						str[i] = sel->msg_tbl[i]->msg;
					}
				}
			}
			else {
				for( i = 0; i < MaxRec; ++i )
					str[i].Empty();
			}
			DispSyzIchiran(i, str);
			m_selbox_user = pn;
			invmng.m_invPG = 0;
			break;
		//インボイス対応
		case SY_INV_PN:
			int invCount = 0;
			int sidx;

			invCount = (int)invmng.m_pInvAry->GetCount();
			sidx = (invmng.m_invPG * invmng.m_invPGCnt);
			
			for( i = 0; i < invmng.m_invPGCnt; ++i ) {
				if( sidx < invCount ) {
					str[i] = invmng.m_pInvAry->GetAt(sidx).inv_tkNam;
					sidx++;
				}
				else {
					str[i].Empty();
				}
			}
			DispSyzIchiran(i, str);
			m_selbox_user = pn;
			break;
		}
	}
	else
	{
		DispSyzIchiran(0, str);
		m_selbox_user = pn;
	}
}


// 消費税名称を画面にセット
void CSkbnLiteDlg::DispSyzIchiran(int max, CString* str)
{
	DWORD i;

#define SYZSEL_MAX	6

	static struct _SelTable SelVal32[SYZSEL_MAX];

	for( i = 0; i < SYZSEL_MAX; ++i )
	{
		// 表示
		SelVal32[i].Data = i < max ? *(str + i) : "";
		//	SelVal32[i].number = i + (isEntrySys() ? 9 : 1);
		SelVal32[i].number = i + 1;
	}

	m_Select.SetData((long)SelVal32);
}



int	 CSkbnLiteDlg::SetSkbnSelNo(int no)
{
	int ret = 0;
	DWORD code;
	int invCount = 0;
	_SY_TKREC_ syTkrec = { 0 };
	INVOICE_INFO invInfo = { 0 };
	struct _SY_MSG_TBL* psmt;
	int noMax, pn, flg;

	if( m_MODEFY_OK )
	{
		pn = get_nowpn();
		switch( pn )
		{
			//消費税仕訳区分
		case SY_SYZSWKBN_PN:
			psmt = m_pSmpk->SY_MSG_SWKBN;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no - 1]->code;

					if( m_pSy->SyFnc(code) != 0 )
						ret = -1;
					else {
						m_pSy->SyMsg(m_pSmpk);
						*m_pLineTxt = m_pSy->Sy_get_message_short(m_pDBDta, 0x07);
						m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
					}
				}
			break;
			//税率
		case SY_ZRITU_PN:
			psmt = m_pSmpk->SY_MSG_RITU;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no - 1]->code;
					if( m_pSy->SyFnc(code) != 0 )
						ret = -1;
					else {
						m_pSy->SyMsg(m_pSmpk);
						*m_pLineTxt = m_pSy->Sy_get_message_short(m_pDBDta, 0x01);
						m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
					}
				}
			break;

			//税区分
		case SY_SZKBN_PN:
			psmt = m_pSmpk->SY_MSG_ZEI;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no - 1]->code;

					if( CD_BEKKI == code || (CD_SOTOZ == code && ! m_Litesub->SotoNotCut()) ) {
						ret = -1;
					}
					else {
						if( m_pSy->SyFnc(code) != 0 )
							ret = -1;
						else {
							m_pSy->SyMsg(m_pSmpk);
							*m_pLineTxt = m_pSy->Sy_get_message_short(m_pDBDta, 0x01);
							m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
						}
					}
				}
			break;
			//売上/仕入区分
		case SY_URISIRE_PN:
			psmt = m_pSmpk->SY_MSG_URISIRE;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no - 1]->code;
					if( m_pSy->SyFnc(code) != 0 )
						ret = -1;
					else {
						m_pSy->SyMsg(m_pSmpk);
						*m_pLineTxt = m_pSy->Sy_get_message_short(m_pDBDta, 0x03);
						m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
					}
				}
			break;

		// インボイス対応
		// 登録番号
		case SY_INV_PN:
			invCount = (int)invmng.m_pInvAry->GetCount();
			if( (no > 0) && (no <= invmng.m_invPGCnt) ) {
				// 科目設定で登録番号のチェックは行われているはずだが、一応仕訳入力でも行っておく
				CString checkInvno;
				int sidx = (invmng.m_invPG * invmng.m_invPGCnt);
				int aryIdx = sidx + (no - 1);

				if( aryIdx < invCount ) {
					checkInvno = invmng.m_pInvAry->GetAt(aryIdx).inv_invNo;

					if( syzInvoice.CheckInvoiceNumber(checkInvno) == -1 ) {
						m_bIgnoreFocus = TRUE;
					//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP);
					//	m_bIgnoreFocus = FALSE;
						m_ErrMsg = syzInvoice.GetErrorMessage();
						PostMessage(WM_MYMESSAGE, INV_ERRMSG);
						ret = -1;
					}
					else {
						m_Litesub->m_dbdata.invno = checkInvno;
						char tmp[128];
						checkInvno.Replace(_T("T"), _T(""));
						VARIANT var;
						var.pbVal = (BYTE*)(LPCTSTR)checkInvno;
						DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);

						sprintf_s(tmp, sizeof(tmp), _T("%s"), m_Litesub->m_dbdata.invno);
						DBSetDispString(this, IDC_ICSDBEDT_INVDISP/*getINP_CTL(SY_INV_PN)->IDC_X*/, tmp);
						m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
					}
				}
			}
			break;

		case SY_MENZEI_PN:
		case SY_MENWARI_PN:
			set_nowpn(m_selbox_user);
			flg = 0;
			code = 0;
			noMax = 0;

			if( m_Koujo80_50 == SYZ_MEN_80) {
				noMax = 1;

				if( m_Litesub->m_dbdata.ddate > SYZMEN4_YMD ) {
					noMax = 0;
				}
				else if( m_Litesub->m_dbdata.ddate < SYZMEN1_YMD ) {
					noMax = 0;
				}
			}
			else if( m_Koujo80_50 == SYZ_MEN_8050 || m_Koujo80_50 == SYZ_MEN_50 ) {
				noMax = 2;
			}
			if( noMax ) {
				if( no > 0 && no <= noMax )
				{
					//1番目除く(課税事業者からの課税仕入れ)
					//code = CD_SP_EXEMPT;		80%
					//code = CD_SP_EXEMPT_50;	50%
					//code = psmt->msg_tbl[no - 0]->code;
				//	if( m_Koujo80_50 == SYZ_MEN_50 ) {
				//		code = CD_SP_EXEMPT_50;	//50 %
				//	}
				//	else {
						if( no == 1 ) {
							code = CD_SP_EXEMPT;	//80 %
						}
						else {
							code = CD_SP_EXEMPT_50;	//50 %
						}
				//	}
				}
			}
			if( code != 0 ) {
				if( m_pSy->SyFnc(code) != 0 )
					ret = -1;
				else {
					*m_pLineTxt = m_pSy->Sy_get_message_short(m_pDBDta, 0x01);
					m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
					flg = 1; //消費税文字列表示ずみ
				}
			}
			if( !flg ) {
				if( pn == SY_MENZEI_PN && no == 0 ) {
					*m_pLineTxt = m_pSy->Sy_get_message_short(m_pDBDta, 0x01);
					m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, no);
				}
			}
//MyTrace("no = %d, --- code = %08x", no, code);
			DSIGN_TRACE(m_pDBDta->dsign);

			break;

		default:
			set_nowpn(m_selbox_user);
			break;
		}
		sydata_set(NULL);
	}

	return ret;
	return 0;
}

void CSkbnLiteDlg::set_nextpos()
{
}

void CSkbnLiteDlg::DlgRectCheck(BOOL& bVert, BOOL& bHorz)
{
	int typ = GetMultiWindowType();

	//MyTrace( "@CSkbnLiteDlg::DlgRectCheck TOP (%d)\n", typ );

	bVert = FALSE;
	bHorz = FALSE;

	if( typ == MULTIWINDOW_NOP || typ == MULTIWINDOW_NONE ) {
		return;
	}

	CRect rc1, rc2, rcWnd;
	GetDlgItem(IDC_ICSDBEDTCTRL2)->GetWindowRect(rc1);
	m_pParent->ScreenToClient(rc1);

	GetDlgItem(IDC_ICSSELCTRL1)->GetWindowRect(rc2);
	m_pParent->ScreenToClient(rc2);

	GetWindowRect(rcWnd);
	m_pParent->ScreenToClient(rcWnd);

#ifdef TEST_CLOSE
	CRect rcTmp;
	rcTmp = rc1;
	MyTrace("@CSkbnLiteDlg::DlgRectCheck[ Ctrl(1) ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
		rcTmp.Width(), rcTmp.Height());
	rcTmp = rc2;
	MyTrace("@CSkbnLiteDlg::DlgRectCheck[ Ctrl(2) ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
		rcTmp.Width(), rcTmp.Height());
	rcTmp = rcWnd;
	MyTrace("@CSkbnLiteDlg::DlgRectCheck[ DlgWnd ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
		rcTmp.Width(), rcTmp.Height());
#endif

	int chkwd = rc1.Width();
	int chkht = rc2.Height();

	if( (rcWnd.right - rc1.right) > chkwd ) {
		bHorz = TRUE;
	}

	if( (rcWnd.bottom - rc2.bottom) > chkht ) {
		bVert = TRUE;
	}
}

int CSkbnLiteDlg::GetDlgWidth()
{
	return 0;
}

int CSkbnLiteDlg::GetDlgHeight()
{
	return 0;
}



// 現在ポジションセット
int CSkbnLiteDlg::set_nowpn(int pn)
{
	PREV_PN = NOW_PN;
	NOW_PN = pn;

	return(PREV_PN);	//直前のポジション
}

// 現在ポジションゲット
int CSkbnLiteDlg::get_nowpn(void)
{
	return(NOW_PN);
}

void CSkbnLiteDlg::set_nowfocus()
{
	set_focus(get_nowpn());
}

// カーソルセット
void CSkbnLiteDlg::set_focus(int pn)
{
	if( pn >= 0 )
	{
		PostMessage(WM_RETROFOCUS, pn);
	}
}


//次のポジション検索
int CSkbnLiteDlg::get_nextpn(int now, int vk/*= 0*/)
{
	int pn;

	if( m_MODEFY_OK ) {
		int i, sgn = 0;
		for( i = 0; SYZLITE_IDX[i] != -1; i++ ) {
			if( now == SYZLITE_IDX[i] ) {
				sgn = 1;
				continue;
			}

			if( sgn ) {
				pn = SYZLITE_IDX[i];
				if( getINP_CTL(pn)->INP_ena )
				{
					return pn;
				}
			}
		}
	}

	return -1;
}


//前のポジション検索
int CSkbnLiteDlg::get_prevpn(int now)
{
	int pn;
	int i, max;

	max = sizeof SYZLITE_IDX / sizeof SYZLITE_IDX[0];
	int sgn = 0;

	for( i = max - 1; i >= 0; i-- ) {
		if( now == SYZLITE_IDX[i] ) {
			sgn = 1;
			continue;
		}

		if( sgn ) {
			pn = SYZLITE_IDX[i];
			if( getINP_CTL(pn)->INP_ena )
			{
				return pn;
			}
		}
	}

	return -1;
}

//親の現在のフォーカスポジションへ
void CSkbnLiteDlg::to_parent(CWnd *parent, long nChar)
{
	//BYTE dsign[4];
	//memcpy(dsign, &m_pZm->dbdata->dsign[0], 4);

	m_pParent->SendMessage(WM_SKBNLITE_KEYDOWN, nChar);
}

// 消費税文字列を取得
void CSkbnLiteDlg::SyGetMessage()
{
	m_pLineTxt->Empty();
	if( m_pDBDta == NULL )
		return;

	m_Litesub->SyGetMessage();
}


void CSkbnLiteDlg::SyMsgJob()
{
	if( m_pDBDta == NULL )
		return;

	m_pSy->SyMsg(m_pSmpk);
}


BOOL CSkbnLiteDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	//if( (m_hAccelTable != NULL) && ::TranslateAccelerator(m_pParent->m_hWnd, m_hAccelTable, pMsg) )
	//{
	//	return TRUE;
	//}
	//
	if( pMsg->message == WM_KEYDOWN ) {
		CWnd *pWnd = GetFocus();
		if( pWnd ) {
			int id = pWnd->GetDlgCtrlID();
			if( pWnd->GetDlgCtrlID() == IDC_CHECK_MENZEI ) {
				set_nowpn( SY_MENZEI_PN );
				OnTerminationCheckMenzei( pMsg->wParam );
				return TRUE;
			}
			if( id == IDC_ICSDBEDT_INV ) {
				invmng.KanaCheckVK_BACK(pMsg);
				//if( invmng.KanaCheckVK_BACK(pMsg) )
				//	return 1;
			}
		}
		switch( pMsg->wParam ) {
		case VK_F2:	case VK_F3:
		case VK_F9:	case VK_F11: case VK_F12:
		case VK_END:
		case VK_F5: case VK_F8:
			to_parent(m_pParent, pMsg->wParam);
			return TRUE;
			break;
		case VK_HOME:
			CWnd *pWnd = GetFocus();
			if( pWnd ) {
				int id = pWnd->GetDlgCtrlID();
				if( id != IDC_ICSDBEDT_INV ) {
					to_parent(m_pParent, pMsg->wParam);
					return TRUE;
				}
			}
			break;
		}
	}
	else if( pMsg->message == WM_KEYUP ) {
		CWnd *pWnd = GetFocus();
		if( pWnd ) {
			int id = pWnd->GetDlgCtrlID();
			if( id == IDC_ICSDBEDT_INV ) {
				if( invmng.FukuKanaSearch(pMsg) )
					return 1;
			}
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}


//=============================================================================
// インボイスバージョンアップマスターでなければ、ダイアログを縮小する
//=============================================================================
void CSkbnLiteDlg::MoveInvCtrl()
{
	// インボイス対応

	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		return;
	}

	CRect rc1, rc2, rect;

	// 移動する基準となるコントロール
	GetDlgItem( IDC_ICSDBEDTCTRL3 )->GetWindowRect( rc1 );
	ScreenToClient( rc1 );

	if( !m_bInvMaster ) {
		// 未インボイスバージョンアップマスター
		// 登録番号、免税事業者からの課税仕入れを表示しない
		// ダイアログの底を登録番号のトップの位置に
		GetWindowRect( rc2 );
//		MoveWindow( rc2.left, rc2.top, rc1.right-rc2.left, rc2.Height() );
	}
	else {
		if( !m_bInvDisp ) {
			// インボイス登録番号を表示しない
			// 登録番号のみ表示しない

			// チェックボックスを登録番号の位置に
			GetDlgItem( IDC_CHECK_MENZEI )->GetWindowRect( rc2 );
			ScreenToClient( rc2 );
			
			GetDlgItem( IDC_CHECK_MENZEI )->MoveWindow( rc2.left, rc1.bottom + 1, rc2.Width(), rc2.Height() );

			int nxtYpos = rc1.bottom + 1 + rc2.Height() + 1;

			int nID[] = {
				IDC_STATIC20, IDC_ICSDBEDT_MEN, -1
			};
			for( int n = 0; nID[n] != -1; n++ ) {
				GetDlgItem(nID[n])->GetWindowRect(rc2);
				ScreenToClient(rc2);
				GetDlgItem(nID[n])->MoveWindow(rc2.left, nxtYpos, rc2.Width(), rc2.Height());
			}
			// ダイアログの底をチェックボックスの底の位置に
		//	GetWindowRect( rc2 );
		//	MoveWindow( rc2.left, rc2.top, rc2.Width(), rc2.Height() - mv );
		}
	}
}

//=============================================================================
// インボイスコントロール表示非表示切り替え
//=============================================================================
void CSkbnLiteDlg::ShowInvCtrl()
{
	// インボイス対応
	if( !m_bInvMaster || !m_bInvUse ) {
		// バージョンアップマスターでない場合、登録番号と免税事業者からの課税仕入れを非表示
		int ctrlId[] = { IDC_STATIC18, IDC_ICSDBEDT_INV, IDC_ICSDBEDT_INVDISP, IDC_CHECK_MENZEI, IDC_STATIC20, IDC_ICSDBEDT_MEN, 0 };
		for( int i= 0; ctrlId[i] != 0; i++ ) {
			GetDlgItem( ctrlId[i] )->ShowWindow( SW_HIDE );
		}
	}
	else {
		if( !m_bInvDisp ) {
			// 登録番号を表示しないマスターの場合、登録番号を非表示
			int ctrlId[] = { IDC_STATIC18, IDC_ICSDBEDT_INV, IDC_ICSDBEDT_INVDISP, 0 };
			for( int i= 0; ctrlId[i] != 0; i++ ) {
				GetDlgItem( ctrlId[i] )->ShowWindow( SW_HIDE );
			}
		}
	}
}

//=============================================================================
// 登録番号ターミネーション
//=============================================================================
int CSkbnLiteDlg::OnTerminationDBEdtInv( long nChar, long inplen, long kst )
{
	int pn = 0, ncnt = 0;
	char pbuf[50] = { 0 };
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE ) {
		return 1;
	}

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
		if( m_MODEFY_OK ) {
			if( inplen == 0 ) { // 貼り付けの場合はinplenが0になる
				CString dbInvno, edtInvno;
				dbInvno = m_Litesub->m_dbdata.invno;
				if( !dbInvno.IsEmpty() ) {
					dbInvno.Replace(_T("T"), _T(""));
				}

				DBdata_get(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);

				edtInvno = var.pbVal;
				if( dbInvno.CompareNoCase(edtInvno) ) {
					inplen = 1;
				}
			}

			if( inplen > 0 || inplen < 0 ) {

				pbuf[0] = '\0';
				DBdata_get(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				sprintf_s(pbuf, sizeof(pbuf), _T("%s"), (char*)var.pbVal);

				if( inplen < 0 ) {
					//全部消した場合
					if( pbuf[0] == 0 ) {
						// Delete, Back Spaceで削除すると-1となる
						m_bMsgDsp = TRUE;
						//メッセージ出さない
						m_Litesub->m_dbdata.invno.Empty();
						var.pbVal = NULL;
						DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
						sydata_set(NULL);
						m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, 0);
						m_bMsgDsp = FALSE;
						goto LABEL_KEY;
					}
				}

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
				int pn = get_nowpn();

				if( (ncnt >= 1) && (ncnt <= 2) ) {
					// 摘要情報から登録番号を取得
					SetSkbnSelNo(atoi(pbuf));
				}
				else {
					CString checkInvno;
					checkInvno.Format(_T("T%s"), pbuf);
					if( syzInvoice.CheckInvoiceNumber(checkInvno) == -1 ) {
						//m_Litesub->m_dbdata.invno.Empty();
						//var.pbVal = (BYTE*)0;
						//DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
						m_bIgnoreFocus = TRUE;
					//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP);
						m_ErrMsg = syzInvoice.GetErrorMessage();
						PostMessage(WM_MYMESSAGE, INV_ERRMSG);
						var.pbVal = (BYTE*)pbuf;
						DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
						DBSetDispString(this, IDC_ICSDBEDT_INVDISP/*getINP_CTL(SY_INV_PN)->IDC_X*/, "");
					//	set_focus(SY_INV_PN);
					//	m_bIgnoreFocus = FALSE;
						return 0;
					}
					else {
						m_Litesub->m_dbdata.invno = checkInvno;
						CString stmp;
						stmp = checkInvno;
						stmp.Replace(_T("T"), _T(""));
						VARIANT var;
						var.pbVal = (BYTE*)(LPCTSTR)stmp;
						DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);

						DBSetDispString(this, IDC_ICSDBEDT_INVDISP/*getINP_CTL(SY_INV_PN)->IDC_X*/, checkInvno);
						m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, 0);
					}
				}
			}
			else {
#ifdef CLOSE
				if( inplen < 0 ) {
					// Delete, Back Spaceで削除すると-1となる
				//	CString dmyInvno;
				//	syzInvoice.CheckInvoiceNumber(dmyInvno);
					m_bMsgDsp = TRUE;
				//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP); //必ずエラーになる

					m_Litesub->m_dbdata.invno.Empty();
					var.pbVal = NULL;
					DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
					sydata_set(NULL);
					m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, 0);
					set_focus(SY_INV_PN);
					nChar = 0;
					m_bMsgDsp = FALSE;
				}
#endif
			}
		}
		else {
			sydata_set(NULL);
		}
		break;

	case VK_DELETE:
		m_Litesub->m_dbdata.invno.Empty();
		var.pbVal = NULL;
		DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
		m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, 0);
		sydata_set(NULL);
		break;
	case VK_HOME:
		invmng.InvNoSearch(NULL);
		break;
	default:
		break;
	}

LABEL_KEY:
	if(( nChar == VK_TAB ) && ( kst & ICSDBEDT_KST_SHIFT )) {
		if(( pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar ) {
	case VK_LEFT:
		if(( pn = get_prevpn( get_nowpn() )) == SY_INV_PN ) {
			set_focus( pn );
			return 0;
		}
		break;
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_DOWN:
	case VK_RETURN:
	case VK_TAB:
		if(( pn = get_nextpn( get_nowpn(), nChar )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_UP:
	case VK_F2:
		if(( pn = get_prevpn( get_nowpn() )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;
	case VK_HOME:
		invmng.InvDispPage(0);
		break;
	case VK_PRIOR:
		invmng.InvDispPage(-1);
		break;
	case VK_NEXT:
	{
		invmng.InvDispPage(1);
	}
	break;

	default:
		return 1;
		break;
	}

	return 0;
}

//=============================================================================
// 免税事業者からの課税仕入れターミネーション
//=============================================================================
int CSkbnLiteDlg::OnTerminationCheckMenzei( DWORD nChar )
{
	DBGFUNCTION;
	int pn = 0;
	int st;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	if( ::GetKeyState(VK_SHIFT) < 0 ) {
		if( nChar == VK_TAB ) {
			nChar = VK_LEFT;
		}
	}

	switch( nChar )	{
	case VK_LEFT:
		if(( pn = get_prevpn( get_nowpn() )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		break;
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_DOWN:
	case VK_RETURN:
	case VK_TAB:
		if(( pn = get_nextpn( get_nowpn(), nChar )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		return 0;
	case VK_UP:
	case VK_F2:
		if(( pn = get_prevpn( get_nowpn() )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;
	case VK_SPACE:
		SetMenzeiCheck();
		MenCheckButtonJob(0);
		break;

	case '1': case VK_NUMPAD1:
	case '2': case VK_NUMPAD2:
		st = m_MenzeiCheck.GetCheck();
		if( st == BST_UNCHECKED ) {
			SetMenzeiCheck();
		}
		MenCheckButtonJob(nChar);
		break;

	default:
		return 1;
		break;
	}

	return 0;
}

//=============================================================================
// 免税事業者からの課税仕入れのチェック切り替え
//=============================================================================
void CSkbnLiteDlg::SetMenzeiCheck()
{
	DBGFUNCTION;

	int check = 0, st = 0;
	st = m_MenzeiCheck.GetCheck();

	if( st == BST_CHECKED ) {
		check = BST_UNCHECKED;
	}
	else {
		check = BST_CHECKED;
	}

	m_MenzeiCheck.SetCheck( check );
}

void CSkbnLiteDlg::OnBnClickedMenzei()
{
	DBGFUNCTION;
	MenCheckButtonJob(0);
}


void CSkbnLiteDlg::MenCheckButtonJob(long nChar)
{
	if( m_MODEFY_OK ) {
		int st = m_MenzeiCheck.GetCheck();

		if( st == BST_CHECKED ) {
			// 免税事業者からの仕入れ
			DWORD fnc = 0;
			if( nChar == 0 ) {
				if( m_Litesub->m_dbdata.ddate > SYZMEN4_YMD ) {
					fnc = CD_SP_EXEMPT_00;
				}
				else if( m_Litesub->m_dbdata.ddate >= SYZMEN3_YMD ) {
					fnc = CD_SP_EXEMPT_50;
				}
				else {
					fnc = CD_SP_EXEMPT;
				}
			}
			else {
				if( m_Koujo80_50 == SYZ_MEN_80 ) {
					if( nChar == '1' || nChar == VK_NUMPAD1 ) {
						fnc = CD_SP_EXEMPT;
					}
				}
				else if( m_Koujo80_50 == SYZ_MEN_8050 ) {
					if( nChar == '2' || nChar == VK_NUMPAD2 ) {
						fnc = CD_SP_EXEMPT_50;
					}
					if( nChar == '1' || nChar == VK_NUMPAD1 ) {
						fnc = CD_SP_EXEMPT;
					}
				}
				else if( m_Koujo80_50 == SYZ_MEN_50 ) {
					if( nChar == '2' || nChar == VK_NUMPAD2 ) {
						fnc = CD_SP_EXEMPT_50;
					}
					if( nChar == '1' || nChar == VK_NUMPAD1 ) {
						fnc = CD_SP_EXEMPT;
					}
				}
				if( m_Litesub->m_dbdata.ddate > SYZMEN4_YMD ) {
					fnc = CD_SP_EXEMPT_00;
				}
				else if( m_Litesub->m_dbdata.ddate < SYZMEN1_YMD ) {
					fnc = CD_SP_NO_EXEMPT;
				}
			}
			if( fnc != 0 ) {
				m_pSy->SyFnc(fnc);
				MenWariDBEdit(TRUE);
			}
		}
		else {
			// 課税事業者からの仕入
			m_pSy->SyFnc(CD_SP_NO_EXEMPT);
			MenWariDBEdit(FALSE);
		}
		set_nowpn(SY_MENZEI_PN);
		SetSkbnSelNo(NULL);
	//	m_pParent->SendMessage(WM_SKBNLITE_DISPTXT, NULL);
	}
	else {
		sydata_set(NULL);
	}
}



//免税事業者から仕入 控除割合
void CSkbnLiteDlg::MenWariDBEdit(BOOL bEnable)
{
	DBGFUNCTION;
	GetDlgItem(IDC_STATIC20)->EnableWindow(bEnable);
	m_MenWari.EnableWindow(bEnable);

	struct _InpCtl* p_inp;
	p_inp = getINP_CTL(SY_MENWARI_PN);
	p_inp->INP_ena = bEnable;

	if( bEnable ) {
		MenWariSetDBedtData();
	}
	else {
		//エディット初期化
		VARIANT var;
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT_MEN, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_MEN, NULL);
	}
}

//コンボボックス 割合セット
void CSkbnLiteDlg::MenWariSetDBedtData()
{
	int sel = -1;

	int tmp = (m_pDBDta->dsign[6] & 0xc0);

	if( tmp == 0 ) {
		//80%
		sel = 0;
	}
	tmp >>= 6;

	if( tmp == 1 ) {
		sel = 1;	//50%
	}
	else if( tmp == 2 ) {
		//0%
		sel = -1;
	}
	//struct _SY_MSG_TBL* psmt;
	//psmt = m_pSmpk->SY_MSG_EXEMPT;

	VARIANT var;
	char tmpbf[32];
	sprintf_s(tmpbf, sizeof tmpbf, "%d", sel);
	var.pbVal = (BYTE*)tmpbf;
	DBdata_set(this, IDC_ICSDBEDT_MEN, &var, ICSDBEDT_TYPE_STRING, 0);
	DBSetDispString(this, IDC_ICSDBEDT_MEN, GetMenzeiWariaiString(sel));
}


//=============================================================================
// 控除割合ターミネーション
//=============================================================================
int CSkbnLiteDlg::OnTerminationDBEdtMen(long nChar, long inplen, long kst)
{
	DBGFUNCTION;
	int i, pn = 0;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	if( ::GetKeyState(VK_SHIFT) < 0 ) {
		if( nChar == VK_TAB ) {
			nChar = VK_LEFT;
		}
	}

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
		//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get(this, getINP_CTL(SY_MENWARI_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				strcpy_s(pbuf, sizeof pbuf, (char*)var.pbVal);

				for( i = 0; pbuf[i]; ++i )
				{
					if( isdigit(pbuf[i]) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo(atoi(pbuf));
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	switch( nChar ) {
	case VK_RIGHT:
	case VK_DOWN:
		to_parent(m_pParent, nChar);
		return 0;
		break;

	case VK_RETURN:
	case VK_TAB:
		if( (pn = get_nextpn(get_nowpn(), nChar)) != -1 ) {
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, nChar);
			return 0;
		}
		return 0;
	case VK_UP:
	case VK_LEFT:
	case VK_F2:
		if( (pn = get_prevpn(get_nowpn())) != -1 ) {
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;
	default:
		return 1;
		break;
	}

	return 0;
}


void CSkbnLiteDlg::OnSetfocusCheckMenzei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	set_nowpn(SY_MENZEI_PN);
	struct _SY_MSG_TBL* psmt;
	psmt = m_pSmpk->SY_MSG_EXEMPT;
	int pn = SY_MENWARI_PN;

	kbn_select_make(psmt, pn);
}


HBRUSH CSkbnLiteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
#ifdef CLOSE
	if( pWnd->GetSafeHwnd() == m_MenzeiCheck.GetSafeHwnd() ) {
		HWND hwnd = ::GetFocus();
		if( hwnd == m_MenzeiCheck.m_hWnd ) {
			return (HBRUSH)m_brBk;
		}
		else {
			return (HBRUSH)m_brDefBk;
		}
	}
#endif
	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void CSkbnLiteDlg::ImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ID == IDC_ICSDBEDT_INV ) {
		char	tmp[128] = { 0 };

		if( !lastBrnInpStr(string, tmp, sizeof tmp) )
			return;
		if( CheckDataDBeditInv(nChar, tmp) <= 0 )
			return;

		OnTerminationDBEdtInv(nChar, 1, 0);
	}
}


long CSkbnLiteDlg::CheckDataDBeditInv(long nChar, LPCTSTR string)
{
	if( m_MODEFY_OK )
	{
		VARIANT var;
		int no = atoi(string);
		int idc = IDC_ICSDBEDT_INV;
		//選択欄より
		if( (no > 0) && (no <= invmng.m_invPGCnt) ) {
			func_invketachk(idc, this, string);
		}
		else {
			//登録番号チェック は Terminationで
			CICSDBEDT* pDedt;
			pDedt = (CICSDBEDT*)(GetDlgItem(idc));
			int pos = pDedt->GetCaretPosition();
			var.pbVal = (BYTE*)string;

			if( strlen(string) == 13 || pos == 13 ) {
				DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
			}
			else {
				pDedt->InsertData(&var, ICSDBEDT_TYPE_STRING, 0, pos);
			}
		}
	}
	else {
		return 0;
	}

	return nChar;
}


//インボイス番号より
LRESULT CSkbnLiteDlg::OnMyMessage(WPARAM wp, LPARAM lp)
{
	if( wp == INV_KBNMAKE ) {
		kbn_select_make(NULL, SY_INV_PN);
	}
	else if( wp == INV_ONTERM ) {
		WORD vkcode = LOWORD(lp);
		OnTerminationDBEdtInv(vkcode, 0, 0);
	}
	else if( wp == INV_ERRMSG) {
		if( IsWindowVisible() ) {
			ICSMessageBox(m_ErrMsg, MB_ICONSTOP,0,0,this);
			m_DBedtInvDisp.ShowWindow(SW_HIDE);
			set_focus(SY_INV_PN);
		}
		m_bIgnoreFocus = FALSE;
		m_ErrMsg.Empty();
	}
	return 1;
}
