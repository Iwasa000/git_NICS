// AutoInp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DBDinpRec.h"

#include "AutoInp.h"

#include "MstRw.h"
#include "input1.h"
#include "external.h"


static int Mst_KOJI;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoInp ダイアログ


CAutoInp::CAutoInp(CWnd* pParent /*=NULL*/)
	: ICSDialog(CAutoInp::IDD, pParent)
	, m_BMNtkbr(FALSE)
{
	//{{AFX_DATA_INIT(CAutoInp)	
	m_KMPG = -1;
	m_KMNO = -1;
	m_BMNSEL = -1;
	m_SWKTEK = -1;
	m_BMON = FALSE;
	m_CRED = FALSE;
	m_DATE = FALSE;
	m_DEBT = FALSE;
	m_BRN = FALSE;
	m_BRTEK = FALSE;
	m_BRTOTKY = FALSE;
	m_SYGBRN = FALSE;
	m_SYIMP = FALSE;
	m_BRCPY = FALSE;
	m_BMNDSP = FALSE;
	m_KOJI = FALSE;
	m_SYOBUZ = FALSE;
	m_RDDATE = FALSE;
	m_RDBRN = FALSE;
	m_BMNCHK = FALSE;
	m_DCKMchk = FALSE;
	m_KanaDisp = FALSE;
	m_Hjiyu = FALSE;
//	m_BMNBrn = FALSE;
	//}}AFX_DATA_INIT

	m_ScnCode	=	FALSE;
	m_ZeiChk	=	FALSE;
	m_DelSwk	=	FALSE;
	m_ScnClr	=	FALSE;

	m_KjiInp	=	FALSE;
	m_KanKji	=	FALSE;
	m_KjiKmk	=	FALSE;
#ifdef CENTURY_CLOSE
	m_BRNKak	=	FALSE;
#endif
	m_BMNtkbr	=	FALSE;
	m_BrnKana	=	FALSE;
	m_RTSWK		=	FALSE;
	m_BmnTky	=	FALSE;
	::ZeroMemory( &m_Sel, sizeof m_Sel );
}


void CAutoInp::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoInp)
	DDX_Control(pDX, IDC_DENP_COMBO, m_DENP);
	DDX_Radio(pDX, IDC_RADIOKMKEEP, m_KMPG);
	DDX_Radio(pDX, IDC_RADIOKMCODE, m_KMNO);
	DDX_Radio(pDX, IDC_RADIOBMNSELYES, m_BMNSEL);
	DDX_Radio(pDX, IDC_RADIOSWKTEKYES, m_SWKTEK);
	DDX_Radio(pDX, IDC_RADIOSWKTKY1,	m_SWTKYD);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Check(pDX, IDC_BMNFIX, m_BMON);
	DDX_Check(pDX, IDC_CREDFIX, m_CRED);
	DDX_Check(pDX, IDC_DATEFIX, m_DATE);
	DDX_Check(pDX, IDC_DEBTFIX, m_DEBT);
	DDX_Check(pDX, IDC_BRN_INPCHK, m_BRN);
	DDX_Check(pDX, IDC_BRNNAM_DSP, m_BRTEK);
	DDX_Check(pDX, IDC_BRNTOSWK, m_BRTOTKY);
	DDX_Check(pDX, IDC_SYOBRN_DSP, m_SYGBRN);
	DDX_Check(pDX, IDC_IMPORT_CHK, m_SYIMP);
	DDX_Check(pDX, IDC_BRN_CPY, m_BRCPY);
	DDX_Check(pDX, IDC_BMNNAM_DSP, m_BMNDSP);
	DDX_Check(pDX, IDC_KOJIFIX, m_KOJI);
	DDX_Check(pDX, IDC_SYO_BUZZER, m_SYOBUZ);
	DDX_Check(pDX, IDC_RD_DATE, m_RDDATE);
	DDX_Check(pDX, IDC_RD_BRN, m_RDBRN);
	DDX_Check(pDX, IDC_BMN_CHECK, m_BMNCHK);
	DDX_Control(pDX, IDC_ICSDBEDT_BSKMK, m_DBEdtKmk);
	DDX_Check(pDX, IDC_BS_CHK, m_DCKMchk);
	//}}AFX_DATA_MAP
	//	DDX_Control(pDX, IDC_MNTHSEL, m_selDate);
	DDX_Check(pDX, IDC_KANADISP_CHK, m_KanaDisp);
	DDX_Check(pDX, IDC_HJIYUTOSWK, m_Hjiyu);
	DDX_Check(pDX, IDC_KOJITOSWK, m_KojiTky);
	//	DDX_Check(pDX, IDC_BMNBRN_REG, m_BMNBrn);
	DDX_Check(pDX, IDC_SCAN_KMCD, m_ScnCode);
	DDX_Check(pDX, IDC_ZEIMDFY_CHK, m_ZeiChk);
	DDX_Check(pDX, IDC_DELSWK_CHK, m_DelSwk);
	DDX_Check(pDX, IDC_SCNCLR_CHK, m_ScnClr);

	DDX_Check(pDX, IDC_KJI_INPCHK, m_KjiInp);
	DDX_Check(pDX, IDC_KANKJI_CHK, m_KanKji);
	DDX_Check(pDX, IDC_KJIKMK_CHK, m_KjiKmk);
#ifdef CENTURY_CLOSE
	DDX_Check(pDX, IDC_BRN_INPKAKU, m_BRNKak);
#endif
	DDX_Control(pDX, IDC_SNO_COMBO, m_DEvi);
	DDX_Check(pDX, IDC_BMNTKBR_CHK, m_BMNtkbr);
	DDX_Check(pDX, IDC_BRNSEL_KANA, m_BrnKana);
	DDX_Check(pDX, IDC_RTSWKKANA_CHK, m_RTSWK);
	DDX_Check(pDX, IDC_BMNTOSWK, m_BmnTky);
}

void CAutoInp::OnInitial( int koji_sw, struct _AUTOSEL *sel, int no, int nChar, BOOL APPEND_MODE )
{

	m_DATE = (sel->DATE_OPT == OPT_FIX ? TRUE : FALSE);
	// 伝票番号の設定は 後で、
	
	m_BMON = (sel->BMON_OPT == OPT_FIX ? TRUE : FALSE);
	m_KOJI = (sel->KOJI_OPT == OPT_FIX ? TRUE : FALSE);
	m_DEBT = (sel->DEBT_OPT == OPT_FIX ? TRUE : FALSE);
	m_CRED = (sel->CRED_OPT == OPT_FIX ? TRUE : FALSE);
	m_BRN = (sel->BRN_OPT == OPT_RGSCHK ? TRUE : FALSE); // 未入力チェック
	m_BRTEK = (sel->BRTEK_OPT == OPT_DSP ? TRUE : FALSE);

	m_BRTOTKY	= (sel->BRTOTKY_OPT == OPT_EXEC ? TRUE : FALSE);
	m_BRCPY = (sel->BRCPY_OPT == OPT_BRCPY ? TRUE : FALSE );

	m_SYGBRN = (sel->SYGBRN_OPT == OPT_DSP ? TRUE : FALSE);
	m_SYOBUZ = (sel->SYOBUZ_OPT == OPT_SYOBUZ ? TRUE : FALSE);

	m_BMNCHK = (sel->BMNCHK_OPT == OPT_BMNCHK ? TRUE : FALSE);
	m_BMNDSP = (sel->BMNDSP_OPT == OPT_BMNDSP ? TRUE : FALSE);
	m_KojiTky = (sel->KJTOTKY_OPT == OPT_KJTOTKY ? TRUE : FALSE );

	m_BMNSEL = sel->BMNSEL_OPT == -1 ? -1 : (sel->BMNSEL_OPT == OPT_BMNSEL ? 0 :1 ); // 部門選択
//	m_BMNBrn = (sel->BMNBRN_OPT == OPT_BMNBRN ? TRUE : FALSE);

#ifdef DB_CLOSE
	m_SYIMP	= (sel->SYIMPORT_OPT == OPT_AUTO ? TRUE : FALSE);
#endif
	m_Hjiyu = (sel->HJIYU_OPT == OPT_HJIYU ? TRUE : FALSE);

	// 科目選択関係
	m_KMPG = sel->KMPG_OPT == -1 ? -1 : (sel->KMPG_OPT == OPT_KMPG ? 0 :1 );
	m_KMNO = sel->KMNO_OPT == -1 ? -1 : (sel->KMNO_OPT == OPT_KMNO ? 0 :1 );

	// 対応摘要の表示順
	m_SWKTEK = sel->SWKTEK_OPT == -1 ? -1 : (sel->SWKTEK_OPT == OPT_SWKTEK ? 0 :1 );

	m_RDDATE = (sel->RDDATE_OPT == OPT_RDDATE ? TRUE : FALSE); // 定型仕訳 日付
	m_RDBRN  = (sel->RDBRN_OPT  == OPT_RDBRN  ? TRUE : FALSE); // 定型仕訳 枝番

	m_DCKMchk	= (sel->DCKM_OPT == OPT_NODSP ? TRUE : FALSE );
	m_KanaDisp	= (sel->KANADSP_OPT == OPT_KANAJUN ? TRUE : FALSE );
	// 検索一覧での科目コード表示
	m_ScnCode	= (sel->SCNCD_OPT == OPT_SCNCD ? TRUE : FALSE );
	// 税額修正可
	m_ZeiChk	= (sel->ZEIMDFY_OPT == OPT_ZEIMDFY ? TRUE : FALSE );
	// 取消仕訳処理
	m_DelSwk	= (sel->DELSWK_OPT == OPT_DELSWK ? TRUE : FALSE );
	// 検索項目クリア
	m_ScnClr	= (sel->SCNCLR_OPT == OPT_SCNCLR ? TRUE : FALSE );
	// 工事関係チェック
	m_KjiInp	= (sel->KJIINP_OPT == OPT_KJIINP ? TRUE : FALSE );
	m_KanKji	= (sel->KANKJI_OPT == OPT_KANKJI ? TRUE : FALSE );
	// 3.14 /11
	m_KjiKmk	= (sel->KJIKMK_OPT == OPT_KJIKMK ? TRUE : FALSE );
#ifdef CENTURY_CLOSE
	// 4.27 /12
	m_BRNKak	= (sel->BRNKAK_OPT == OPT_BRNKAK ? TRUE : FALSE );
#endif

	//項目の 表示チェック用に 保存 
	m_Sel = *sel;

	if( ! m_Sel.DCKM_CODE[0] ) {
		strcpy_s( m_Sel.DCKM_CODE, sizeof m_Sel.DCKM_CODE, GNKINCode(pDBzm->zvol->apno) );
	}

	// 仕訳対応摘要の表示
	m_SWTKYD = sel->SWTKYD_OPT == -1 ? -1 : (sel->SWTKYD_OPT == OPT_NONOPT ? 0 :1 );

	// 02.08 /13
	m_BMNtkbr = sel->BMNTKBR_OPT == -1 ? -1 : (sel->BMNTKBR_OPT == OPT_NONOPT ? FALSE : TRUE );

	// 04.01 /15
	m_BrnKana = sel->BRNKANA_OPT == -1 ? -1 : (sel->BRNKANA_OPT == OPT_NONOPT ? FALSE : TRUE );

	// 06.23 /17
	m_RTSWK = sel->RTSWKKANA_OPT == -1 ? -1 : (sel->RTSWKKANA_OPT == OPT_NONOPT ? FALSE : TRUE );
	// 06.07 /18
	m_BmnTky = (sel->BMNTOTKY_OPT == OPT_CHKON ? TRUE : FALSE );

	m_APPEND_MODE = APPEND_MODE;

	Mst_KOJI = koji_sw;

	m_grpno = no;

	alt_key = nChar;
}

// 非表示・表示
void CAutoInp::dsp_onoff()
{
	if( m_Sel.DATE_OPT == -1 || !m_APPEND_MODE )
	{
		GetDlgItem( IDC_DATEFIX )->EnableWindow( FALSE );
	}
	//if( m_Sel.DENP_OPT == -1 || !m_APPEND_MODE )
	//[改良依頼No.22-0710]
	//仕訳入力モード以外でも伝票番号の設定を可能に
	if (m_Sel.DENP_OPT == -1)
	{
		GetDlgItem( IDC_DENPMODEGRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_DENP_COMBO )->EnableWindow( FALSE );
	}

	if( m_Sel.KOJI_OPT == -1 || ! Mst_KOJI || !m_APPEND_MODE ) {
		GetDlgItem( IDC_KOJIFIX )->EnableWindow( FALSE );
	}

	if( m_Sel.BMON_OPT == -1 || !m_APPEND_MODE )
	{
		GetDlgItem( IDC_BMNFIX )->EnableWindow( FALSE );
	}
	if( m_Sel.DEBT_OPT == -1 || !m_APPEND_MODE )
	{
		GetDlgItem( IDC_DEBTFIX )->EnableWindow( FALSE );
	}
	if( m_Sel.CRED_OPT == -1 || !m_APPEND_MODE )
	{
		GetDlgItem( IDC_CREDFIX )->EnableWindow( FALSE );
	}

	if( m_Sel.BRN_OPT == -1 )	// 枝番未入力
	{
		GetDlgItem( IDC_BRN_INPCHK )->EnableWindow( FALSE );
	}
	if( m_Sel.BRTEK_OPT == -1 )	// 枝番名称表示
	{
		GetDlgItem( IDC_BRNNAM_DSP )->EnableWindow( FALSE );
	}

	if( m_Sel.BRTOTKY_OPT == -1 )
	{
		GetDlgItem( IDC_BRNTOSWK )->EnableWindow( FALSE );
	}
	if( m_Sel.BRCPY_OPT == -1 ) {	// 08.02 /02
		GetDlgItem( IDC_BRN_CPY )->EnableWindow( FALSE );
	}

	if( m_Sel.SYOBUZ_OPT == -1 ) {
		GetDlgItem( IDC_SYO_BUZZER )->EnableWindow( FALSE );
	}
	if( m_Sel.SYGBRN_OPT == -1 ) {
		GetDlgItem( IDC_SYOBRN_DSP )->EnableWindow( FALSE );
	}
#ifdef DB_CLOSE
	if( m_Sel.SYIMPORT_OPT == -1 || !m_APPEND_MODE ) {
		GetDlgItem( IDC_IMPORT_CHK )->EnableWindow( FALSE );
	}
#endif
	if( m_Sel.HJIYU_OPT == -1 ) {
		GetDlgItem( IDC_HJIYUTOSWK )->EnableWindow( FALSE );
	}

	if( m_Sel.KMPG_OPT == -1 ) {
		GetDlgItem( IDC_KMKSELGRP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOKMKEEP )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOKMTOP )->EnableWindow( FALSE );
	}
	if( m_Sel.KMNO_OPT == -1 ) {
		GetDlgItem( IDC_KMKSELGRP2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOKMCODE )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOKMNUM )->EnableWindow( FALSE );
	}
	if( m_Sel.BMNCHK_OPT == -1 ) {
		GetDlgItem( IDC_BMN_CHECK )->EnableWindow( FALSE );
	}
	if( m_Sel.BMNDSP_OPT == -1 ) {
		GetDlgItem( IDC_BMNNAM_DSP )->EnableWindow( FALSE );
	}
	if( m_Sel.BMNSEL_OPT == -1 ) {
		GetDlgItem( IDC_BMNSELGRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOBMNSELYES )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOBMNSELNO )->EnableWindow( FALSE );
	}
	if( m_Sel.BMNBRN_OPT == -1 ) {
		GetDlgItem( IDC_BMNBRN_REG )->EnableWindow( FALSE );
	}
	// 工事番号を仕訳摘要に
	if( m_Sel.KJTOTKY_OPT == -1 || ! Mst_KOJI ) {
		GetDlgItem( IDC_KOJITOSWK )->EnableWindow( FALSE );
	}

	if( m_Sel.SWKTEK_OPT == -1 ) {
		GetDlgItem( IDC_SWKTEKGRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOSWKTEKYES )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOSWKTEKNO )->EnableWindow( FALSE );
	}
	
	if( m_Sel.RDDATE_OPT == -1 ) {
		GetDlgItem( IDC_RD_DATE )->EnableWindow( FALSE );
	}
	if( m_Sel.RDBRN_OPT == -1 ) {
		GetDlgItem( IDC_RD_BRN )->EnableWindow( FALSE );
	}

	// '08.12.01
	if( pDBzm && pDBzm->zvol && ((pDBzm->zvol->apno&0xff)>=0x50) ){
		GetDlgItem( IDC_HJIYUTOSWK )->EnableWindow( FALSE );
	}
	// 工事未入力
	if( m_Sel.KJIINP_OPT == -1 /*|| ! Mst_KOJI &&*/ ) {
		GetDlgItem( IDC_KJI_INPCHK )->EnableWindow( FALSE );
	}
	if( m_Sel.KANKJI_OPT == -1 || ! Mst_KOJI ) {
		GetDlgItem( IDC_KANKJI_CHK )->EnableWindow( FALSE );
	}
	// 03.14 /11
	if( m_Sel.KJIKMK_OPT == -1 || ! Mst_KOJI ) {
		GetDlgItem( IDC_KJIKMK_CHK )->EnableWindow( FALSE );
	}
#ifdef CENTURY_CLOSE
	// 枝番未入力時 移動しない。
	if( m_Sel.BRNKAK_OPT == -1 ) {
		GetDlgItem( IDC_BRN_INPKAKU )->EnableWindow( FALSE );
	}
#endif
	// 11.13 /12
	if( m_Sel.SWTKYD_OPT == -1 ) {
		GetDlgItem( IDC_SWTKYDGRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOSWKTKY1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIOSWKTKY2 )->EnableWindow( FALSE );
	}

	// 02.08 /13
	if( m_Sel.BMNTKBR_OPT == -1 ) {
		GetDlgItem( IDC_ST_TKBRNGRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_BMNTKBR_CHK )->EnableWindow( FALSE );
	}

	// 02.08 /13
	if( ! TekiyoKeyMode() ) {
		GetDlgItem( IDC_TKYTAB_GRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHK_TKYTAB )->EnableWindow( FALSE );
		GetDlgItem( IDC_ST_TKYTAB )->EnableWindow( FALSE );
	}

	// 04.01 /15
	if( m_Sel.BRNKANA_OPT == -1 ) {
		GetDlgItem( IDC_BRNSEL_KANA )->EnableWindow( FALSE );
	}

	// 06.23 /17
	if( m_Sel.RTSWKKANA_OPT == -1 ) {
		GetDlgItem( IDC_RTSWKKANA_CHK )->EnableWindow( FALSE );
	}

	// 11.14 /16
	if( ! IsCloudSetupEnv() ) {
		GetDlgItem( IDC_CHK_ENDKAKU )->ShowWindow( SW_HIDE );
	}
	// 06.07 /18
	if( m_Sel.BMNTOTKY_OPT == -1 ) {
		GetDlgItem( IDC_BMNTOSWK )->EnableWindow( FALSE );
	}
}

// データの取り出し
void CAutoInp::get_data( struct _AUTOSEL *sel)
{
	if( m_Sel.DATE_OPT != -1 )
		sel->DATE_OPT = m_DATE ? OPT_FIX : OPT_NONOPT;
	if( m_Sel.DENP_OPT != -1 ) {
		int nSelect = m_DenpSel;

		switch( nSelect ) {
		case 0: sel->DENP_OPT = OPT_NONOPT;	break;
		case 1: sel->DENP_OPT = OPT_AUTO;	break;
		case 2: sel->DENP_OPT = OPT_EQU;	break;
		case 3: sel->DENP_OPT = OPT_NON;	break;
		default:
			sel->DENP_OPT = OPT_NONOPT;	break;
		}
	}
	if( m_Sel.BMON_OPT != -1 )
		sel->BMON_OPT = m_BMON ? OPT_FIX : OPT_NONOPT;
	if( m_Sel.KOJI_OPT != -1 )
		sel->KOJI_OPT = m_KOJI ? OPT_FIX : OPT_NONOPT;

	if( m_Sel.DEBT_OPT != -1 )
		sel->DEBT_OPT = m_DEBT ? OPT_FIX : OPT_NONOPT;
	if( m_Sel.CRED_OPT != -1 )
		sel->CRED_OPT = m_CRED ? OPT_FIX : OPT_NONOPT;
	if( m_Sel.BRN_OPT != -1 )
		sel->BRN_OPT = m_BRN ? OPT_RGSCHK : OPT_NONOPT;
	if( m_Sel.BRTEK_OPT != -1 )
		sel->BRTEK_OPT = m_BRTEK ? OPT_DSP : OPT_NONOPT;

	if( m_Sel.BRTOTKY_OPT != -1 )
		sel->BRTOTKY_OPT = m_BRTOTKY ? OPT_EXEC : OPT_NONOPT;
	if( m_Sel.BRCPY_OPT != -1 )
		sel->BRCPY_OPT = m_BRCPY ? OPT_BRCPY : OPT_NONOPT;

	if( m_Sel.SYGBRN_OPT != -1 )
		sel->SYGBRN_OPT = m_SYGBRN ? OPT_DSP : OPT_NONOPT;
	if( m_Sel.SYOBUZ_OPT != -1 )
		sel->SYOBUZ_OPT = m_SYOBUZ ? OPT_SYOBUZ : OPT_NONOPT;

#ifdef DB_CLOSE
	if( m_Sel.SYIMPORT_OPT != -1 )
		sel->SYIMPORT_OPT = m_SYIMP ? OPT_AUTO : OPT_NONOPT;
#endif
	if( m_Sel.HJIYU_OPT != -1 )
		sel->HJIYU_OPT = m_Hjiyu ? OPT_HJIYU : OPT_NONOPT;

	if( m_Sel.KMPG_OPT != -1 )
		sel->KMPG_OPT = !m_KMPG ? OPT_KMPG : OPT_NONOPT;
	if( m_Sel.KMNO_OPT != -1 )
		sel->KMNO_OPT = !m_KMNO ? OPT_KMNO : OPT_NONOPT;

	if( m_Sel.BMNCHK_OPT != -1 )
		sel->BMNCHK_OPT = m_BMNCHK ? OPT_BMNCHK : OPT_NONOPT;
	if( m_Sel.BMNDSP_OPT != -1 )
		sel->BMNDSP_OPT = m_BMNDSP ? OPT_BMNDSP : OPT_NONOPT;
	if( m_Sel.KJTOTKY_OPT != -1 )
		sel->KJTOTKY_OPT = m_KojiTky ? OPT_KJTOTKY : OPT_NONOPT;

	if( m_Sel.BMNSEL_OPT != -1 )
		sel->BMNSEL_OPT = ! m_BMNSEL ? OPT_BMNSEL : OPT_NONOPT;
//	if( m_Sel.BMNBRN_OPT != -1 )
//		sel->BMNBRN_OPT = m_BMNBrn ? OPT_BMNBRN : OPT_NONOPT;

	// 12.09 /02
	if( m_Sel.SWKTEK_OPT != -1 )
		sel->SWKTEK_OPT = !m_SWKTEK ? OPT_SWKTEK : OPT_NONOPT;

	if( m_Sel.RDDATE_OPT != -1 )
		sel->RDDATE_OPT = m_RDDATE ? OPT_RDDATE : OPT_NONOPT;
	if( m_Sel.RDBRN_OPT != -1 )
		sel->RDBRN_OPT  = m_RDBRN  ? OPT_RDBRN : OPT_NONOPT;
	// 貸借科目の設定
	strcpy_s( sel->DCKM_CODE, sizeof sel->DCKM_CODE, m_Sel.DCKM_CODE );

	if( m_Sel.DCKM_OPT != -1 )
		sel->DCKM_OPT  = m_DCKMchk  ? OPT_NODSP : OPT_NONOPT;

	if( m_Sel.KANADSP_OPT != -1 )
		sel->KANADSP_OPT = m_KanaDisp ? OPT_KANAJUN : OPT_NONOPT;

	// 検索一覧での科目コード表示
	if( m_Sel.SCNCD_OPT != -1 )
		sel->SCNCD_OPT = m_ScnCode ? OPT_SCNCD : OPT_NONOPT;
	// 税額修正可
	if( m_Sel.ZEIMDFY_OPT != -1 )
		sel->ZEIMDFY_OPT = m_ZeiChk ? OPT_ZEIMDFY : OPT_NONOPT;
	// 取消仕訳処理
	if( m_Sel.DELSWK_OPT != -1 )
		sel->DELSWK_OPT = m_DelSwk ? OPT_DELSWK : OPT_NONOPT;
	// 検索項目クリア
	if( m_Sel.SCNCLR_OPT != -1 )
		sel->SCNCLR_OPT = m_ScnClr ? OPT_SCNCLR : OPT_NONOPT;

	// 工事番号未入力チェック
	if( m_Sel.KJIINP_OPT != -1 )
		sel->KJIINP_OPT = m_KjiInp ? OPT_KJIINP : OPT_NONOPT;
	// 完成工事チェック
	if( m_Sel.KANKJI_OPT != -1 )
		sel->KANKJI_OPT = m_KanKji ? OPT_KANKJI : OPT_NONOPT;
	// 03.14 /11
	if( m_Sel.KJIKMK_OPT != -1 )
		sel->KJIKMK_OPT = m_KjiKmk ? OPT_KJIKMK : OPT_NONOPT;
#ifdef CENTURY_CLOSE
	// 04.27 /12
	if( m_Sel.BRNKAK_OPT != -1 )
		sel->BRNKAK_OPT = m_BRNKak ? OPT_BRNKAK : OPT_NONOPT;
#endif

	// 11.13 /12
	if( m_Sel.SWTKYD_OPT != -1 )
		sel->SWTKYD_OPT = !m_SWTKYD ? OPT_NONOPT : OPT_SWTKYD;

	// 12.14 /12
	if( m_Sel.DOCEVI_OPT != -1 ) {
		int nSelect = m_DEviSel;

		switch( nSelect ) {
		case 1: sel->DOCEVI_OPT = SNO_AUTO;
				break;
		case 2: sel->DOCEVI_OPT = SNO_EQCNO;
				break;
		default:
			sel->DOCEVI_OPT = OPT_NONOPT;
			break;
		}
	}
	// 11.13 /12
	if( m_Sel.BMNTKBR_OPT != -1 )
		sel->BMNTKBR_OPT = !m_BMNtkbr ? OPT_NONOPT : OPT_CHKON;

	// 04.01 /15
	if( m_Sel.BRNKANA_OPT != -1 )
		sel->BRNKANA_OPT = !m_BrnKana ? OPT_NONOPT : OPT_KANAJUN;

	// 06.23 /17
	if( m_Sel.RTSWKKANA_OPT != -1 )
		sel->RTSWKKANA_OPT = !m_RTSWK ? OPT_NONOPT : OPT_KANAJUN;
	// 06.07 /18
	if( m_Sel.BMNTOTKY_OPT != -1 )
		sel->BMNTOTKY_OPT = m_BmnTky ? OPT_CHKON : OPT_NONOPT;
}

/*****************************************************
	現在のビュー拡大比率に従って表示サイズを変える
******************************************************/
//void CAutoInp::kakudai(CWnd* wnd)
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


BEGIN_MESSAGE_MAP(CAutoInp, ICSDialog)
	//{{AFX_MSG_MAP(CAutoInp)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_VALCOPY, &CAutoInp::OnBnClickedChkValcopy)
	ON_BN_CLICKED(IDC_CHK_VALNOT, &CAutoInp::OnBnClickedChkValnot)
	ON_BN_CLICKED(IDC_CHK_TKYTYPE1, &CAutoInp::OnBnClickedChkTkytype1)
	ON_BN_CLICKED(IDC_CHK_TKYTYPE2, &CAutoInp::OnBnClickedChkTkytype2)
	ON_BN_CLICKED(IDC_CHK_TKYTYPE3, &CAutoInp::OnBnClickedChkTkytype3)
	ON_BN_CLICKED(IDC_CHK_ZMVDATE, &CAutoInp::OnBnClickedChkZmvdate)
	ON_BN_CLICKED(IDC_CHK_ZMVZANC, &CAutoInp::OnBnClickedChkZmvmnth)
	ON_BN_CLICKED(IDC_CHK_KMKANA, &CAutoInp::OnBnClickedChkKmkana)
	ON_MESSAGE(WM_RETROFOCUS, &CAutoInp::OnRetroFocus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoInp メッセージ ハンドラ

BOOL CAutoInp::OnInitDialog() 
{	
// 部門と工事
	CWnd *pwnd;
	char* strtbl[] = {
		"","番号チェック","名称を表示","選択の優先順", "番号", "未入力チェック"
	};
	int ids[] = {
		IDC_ST_BMNGRP,IDC_BMN_CHECK,IDC_BMNNAM_DSP,IDC_BMNSELGRP,IDC_RADIOBMNSELNO, IDC_KJI_INPCHK
	};
	CString str;

	// 部門・工事番号
	for( int i = 0; i < (sizeof strtbl / sizeof strtbl[0]); i++ ) {
		pwnd = GetDlgItem( ids[i] );
		str.Format( "%s%s", Mst_KOJI ? "部門／工事" : "部門" , strtbl[i] );
		pwnd->SetWindowText( str );
	}

	//諸口枝番文言
	if (pDBzm->zvol->apno >= 0x50) {
		GetDlgItem(IDC_SYOBRN_DSP)->SetWindowText("資金収支諸口枝番名称を表示");
	}
	//インボイス登録番号
	if( bInvMaster && bInvUse && bInvDisp ) {
		GetDlgItem(IDC_RTSWKKANA_CHK)->SetWindowText("対応摘要・登録番号を50音順で表示する");
	}
	else {
		GetDlgItem(IDC_RTSWKKANA_CHK)->SetWindowText("対応摘要を50音順で表示する");
	}
//	ControlInit( 404, 210, -1 );
//	kakudai(this);

	// 12.14 /12
	//[改良依頼220633]証憑番号オプションを入力モード以外でも変更可能に,if文をまとめる
	//if( m_Sel.DOCEVI_OPT == -1 || !m_APPEND_MODE )
	if (m_Sel.DOCEVI_OPT == -1)
	{
		GetDlgItem( IDC_SNO_GRP )->EnableWindow( FALSE );
		GetDlgItem( IDC_SNO_COMBO )->EnableWindow( FALSE );

		//if( m_Sel.DOCEVI_OPT == -1 ) {
			GetDlgItem( IDC_SNO_GRP )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_SNO_COMBO )->ShowWindow( SW_HIDE );

			CRect baseRect, rect;
			GetDlgItem( IDC_DENPMODEGRP )->GetWindowRect( &baseRect );
			ScreenToClient( baseRect );

			GetDlgItem(IDC_RGDATA_GRP )->GetWindowRect( &rect );
			ScreenToClient( rect );

			int mv = rect.top - (baseRect.bottom + 3);
			int nID[] = {
				IDC_RGDATA_GRP, IDC_RD_DATE, IDC_RD_BRN, IDC_SYZ_GRP, IDC_ZEIMDFY_CHK, 0
			};

			for( int n = 0; nID[n] != 0; n++ ) {
				GetDlgItem( nID[n] )->GetWindowRect( &rect );
				ScreenToClient( rect );
				rect.top	-= mv;
				rect.bottom -= mv;
				GetDlgItem( nID[n] )->MoveWindow( &rect );
	
			}
		//}
	}

	ICSDialog::OnInitDialog();
	
	// 伝票番号の項目設定
	int select;

	switch( m_Sel.DENP_OPT ) {
	case OPT_AUTO:	select = 1; break;
	case OPT_EQU:	select = 2; break;
	case OPT_NON:	select = 3; break;
	default: 
		select = 0;
		break;
	}

	m_DENP.SetCurSel( select );

	// 表示・非表示
	dsp_onoff();

	// 指定グループの選択へフォーカス
	goto_grp( m_grpno );

	m_DBEdtKmk.ImeEndMode( TRUE );

	// 貸借科目の表示
	struct _KamokuData ans;
	struct _DBKNREC	*pKn;
	::ZeroMemory( &ans, sizeof(struct _KamokuData) );
	CString km_cod;

	km_cod = m_Sel.DCKM_CODE;
	pKn = pDBzm->DB_PjisToKnrec( km_cod );
	if( pKn != NULL ) {
		memcpy( (char *)ans.kd_name, (LPCTSTR)pKn->knnam, KMNLEN );
	//	memcpy( ans.kd_code, pKn->kncod, KMKCD_LN );
		DWORDcodeToSTRcode( pKn->kncod, ans.kd_code, sizeof ans.kd_code );
	}
	ans.kd_eda = -1;
	DBkamokudata_set( this, IDC_ICSDBEDT_BSKMK, &ans, pDBzm );

	int valsgn = ValEnterCopyMode();

	if( ! valsgn ) {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_CHECKED);
	}
	//10.01 /08
	valsgn = TkyRegMode();

	if( ! valsgn ) {
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2))->SetCheck(BST_UNCHECKED);
	}
	else if( valsgn == 1 ) {
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2))->SetCheck(BST_UNCHECKED);
	}
	// 06.30 /11
	valsgn = ZmoveMode();
	if( ! valsgn ) {
		((CButton*)GetDlgItem(IDC_CHK_ZMVDATE))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_ZMVZANC))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_ZMVDATE))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_ZMVZANC))->SetCheck(BST_CHECKED);
	}

	// 02.07 /13
	valsgn = TKtabMode();
	if( ! valsgn ) {
		((CButton*)GetDlgItem(IDC_CHK_TKYTAB))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_TKYTAB))->SetCheck(BST_CHECKED);
	}

	// 11.14 /16
	if( ! JZcloudEndKakutei() ) {
		((CButton*)GetDlgItem(IDC_CHK_ENDKAKU))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_ENDKAKU))->SetCheck(BST_CHECKED);
	}

	if( ! DenpTypeMode() ) {
		((CButton*)GetDlgItem(IDC_CHK_DNPTYPE))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_DNPTYPE))->SetCheck(BST_CHECKED);
	}

	//1文字検索=0 でチェックON
	if( !KanaSearchType() ) {
		((CButton*)GetDlgItem(IDC_CHK_KMKANA))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_KMKANA))->SetCheck(BST_UNCHECKED);
	}
	OnBnClickedChkKmkana();

	//消費税欄 毎回確認
	if( !SyzStopModeValue() ) {
		((CButton*)GetDlgItem(IDC_CHK_SYZSTOP))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_SYZSTOP))->SetCheck(BST_CHECKED);
	}


#ifdef CLOSE
	m_selDate.SetMode( ICS_DATESELECT_MODE_NONE );
	m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_ON|ICS_DATESELECT_EDIT_M|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
	pDBzm->get_datelimit( m_Sofs, m_Eofs, m_Sdate, m_Edate );
	m_selDate.SetMonthOffset( m_Sofs, m_Eofs, m_Sdate, m_Edate ); 
#endif

	if( m_Sel.DENP_OPT != -1 ) {
		m_DEvi.SetCurSel( m_Sel.DOCEVI_OPT );
	}

	ICSDialog::OnInitDialogEX();

	CRect mainRc;
	GetDBDinpMainFrame()->GetWindowRect(&mainRc);
	CRect dlgRect;
	GetWindowRect(dlgRect);

	//ダイアログ底位置チェック
	if( dlgRect.bottom > mainRc.bottom ) {
		int dlgHt = dlgRect.Height();
		int top = (mainRc.bottom - dlgHt);
		top -= 50;
		if( top < 0 )	top = 0;
		MoveWindow(dlgRect.left, top, dlgRect.Width(), dlgHt );
	}
	return FALSE; //TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// グループチェック
BOOL CAutoInp::ok_grp( int grpno )
{
BOOL ret;

	switch( grpno )
	{
	case GR_DATE:	ret = m_Sel.DATE_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;
	case GR_BMON:	ret = m_Sel.BMON_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;
	case GR_KOJI:	
		if( Mst_KOJI )
			ret = m_Sel.KOJI_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE;
		else
			ret = FALSE;
		 break;
	case GR_DEBT: ret = m_Sel.DEBT_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;
	case GR_CRED: ret = m_Sel.CRED_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;

	case GR_BMNCHK: ret = m_Sel.BMNCHK_OPT == -1 ? FALSE : TRUE; break;
	case GR_BMNDSP: ret = m_Sel.BMNDSP_OPT == -1 ? FALSE : TRUE; break;
	case GR_KJTKY : ret = m_Sel.KJTOTKY_OPT == -1 ? FALSE : TRUE;	break;
	case GR_BMNSEL: ret = m_Sel.BMNSEL_OPT == -1 ? FALSE : TRUE; break;
	case GR_BMNTKY: ret = m_Sel.BMNTOTKY_OPT == -1 ? FALSE : TRUE; break;
//	case GR_BMNBRN: ret = m_Sel.BMNBRN_OPT == -1 ? FALSE : TRUE; break;

	case GR_KJIINP: ret = m_Sel.KJIINP_OPT == -1 ? FALSE : TRUE; break;
	case GR_KANKJI: ret = m_Sel.KANKJI_OPT == -1 ? FALSE : TRUE; break;
	case GR_KJIKMK: ret = m_Sel.KJIKMK_OPT == -1 ? FALSE : TRUE; break;

	case GR_BRN:	ret = m_Sel.BRN_OPT == -1 ? FALSE : TRUE; break;		//枝番未入力
	case GR_BRTEK:	ret = m_Sel.BRTEK_OPT == -1 ? FALSE : TRUE; break;
	case GR_BRCPY:	ret = m_Sel.BRCPY_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;
	case GR_BRTOTKY: ret = m_Sel.BRTOTKY_OPT == -1 ? FALSE : TRUE; break;
	case GR_SYGBRN:	ret = m_Sel.SYGBRN_OPT == -1 ? FALSE : TRUE; break;
#ifdef CENTURY_CLOSE
	case GR_BRNKAK:	ret = m_Sel.BRNKAK_OPT == -1 ? FALSE : TRUE; break;
#endif

	case GR_DENP:	ret = m_Sel.DENP_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;
	case GR_SNO:	ret = m_Sel.DOCEVI_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;	//12.14 /12

	case GR_RDDATE: ret = m_Sel.RDDATE_OPT == -1 ? FALSE : TRUE; break;
	case GR_RDBRN: ret = m_Sel.RDBRN_OPT  == -1 ? FALSE : TRUE; break;

	case GR_SYOBUZ:		ret = m_Sel.SYOBUZ_OPT == -1 ? FALSE : TRUE; break;
#ifdef DB_CLOSE
	case GR_SYIMPORT:	ret = m_Sel.SYIMPORT_OPT == -1 || !m_APPEND_MODE ? FALSE : TRUE; break;
#endif
	case GR_HJIYU:	
		if( pDBzm->zvol->apno >= 0x50 )	ret = FALSE;
		else							ret = m_Sel.HJIYU_OPT == -1 ? FALSE : TRUE; 
		break;

	case GR_SWKTEK:		ret = m_Sel.SWKTEK_OPT == -1 ? FALSE : TRUE; break;

	case GR_KMPAGE: ret = m_Sel.KMPG_OPT == -1 ? FALSE : TRUE; break;
	case GR_KMCODE: ret = m_Sel.KMNO_OPT == -1 ? FALSE : TRUE; break;

	case GR_DCKMCOD: ret = m_Sel.DCKM_CODE[0] == '\0' ? FALSE : TRUE; break;
	case GR_DCKMCHK: ret = m_Sel.DCKM_OPT == -1 ? FALSE : TRUE; break;

	case GR_SCNCODE: ret = m_Sel.SCNCD_OPT == -1 ? FALSE : TRUE; 
		 break;
	case GR_ZEIMDFY: ret = m_Sel.ZEIMDFY_OPT == -1 ? FALSE : TRUE; 
		 break;
	case GR_DELSWK: ret = m_Sel.DELSWK_OPT == -1 ? FALSE : TRUE; 
		 break;
	case GR_SCNCLR: ret = m_Sel.SCNCLR_OPT == -1 ? FALSE : TRUE; 
		 break;

	case GR_KANADISP: ret = m_Sel.KANADSP_OPT == -1 ? FALSE : TRUE; 
		 break;

	case GR_VALCOPY:	ret = TRUE;
						break;

	case GR_TKYREG:		ret = TRUE;
						break;
	case GR_ZMOVE:		ret = TRUE;
						break;

	case GR_SWTKYD:		ret = m_Sel.SWTKYD_OPT == -1 ? FALSE : TRUE; break;

	case GR_BMNTKBR:	ret = m_Sel.BMNTKBR_OPT == -1 ? FALSE : TRUE; break;

	case GR_BRNKANA:	ret = m_Sel.BRNKANA_OPT == -1 ? FALSE : TRUE; break;

	case GR_RTSWK:		ret = m_Sel.RTSWKKANA_OPT == -1 ? FALSE : TRUE; break;

	case GR_TKTAB:		if( TekiyoKeyMode() )	ret = TRUE;
						else					ret = FALSE;
						break;
	case GR_ENDKAKU:	if( IsCloudSetupEnv() )	ret = TRUE;
						else					ret = FALSE;
						break;

	case GR_DENPTYPE:	ret = TRUE;
						break;

	case GR_KANASEARCH:	ret = TRUE;
						break;
	case GR_SYZSTOP:	ret = TRUE;
		break;

	case 0: ret = TRUE; break;
	default: ret = FALSE; break;
	}
	return ret;
}


// グループ内のチェックボックス移動振り分け
int CAutoInp::goto_grpcheck( int grpno, int nChar )
{
	int ret = 0;
	CWnd* pwnd = GetFocus();

	if( !pwnd )	return -1;

	switch( grpno )
	{
	case GR_VALCOPY:
		if( GetDlgItem(IDC_CHK_VALCOPY) == pwnd )
			GotoDlgCtrl( GetDlgItem(IDC_CHK_VALNOT) );
		else
			GotoDlgCtrl( GetDlgItem(IDC_CHK_VALCOPY) );
		break;

	case GR_TKYREG:
		if( nChar == VK_DOWN ) {
			if( GetDlgItem(IDC_CHK_TKYTYPE1) == pwnd )
				GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE3) );
			else if( GetDlgItem(IDC_CHK_TKYTYPE3) == pwnd )
				GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE2) );
			else
				GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE1) );
		}
		else {
			if( GetDlgItem(IDC_CHK_TKYTYPE1) == pwnd )
				GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE2) );
			else if( GetDlgItem(IDC_CHK_TKYTYPE3) == pwnd )
				GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE1) );
			else
				GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE3) );
		}
		break;

	case GR_ZMOVE:
		if( GetDlgItem(IDC_CHK_ZMVDATE) == pwnd )
			GotoDlgCtrl( GetDlgItem(IDC_CHK_ZMVZANC) );
		else
			GotoDlgCtrl( GetDlgItem(IDC_CHK_ZMVDATE) );
		break;

	default:
		ret = -1;
		break;
	}

	return ret;
}

// グループ振り分け
void CAutoInp::goto_grp( int grpno )
{
	for( ; grpno <= GRP_MAX ; ++grpno )
		if( ok_grp( grpno ) )
			break;

	switch( grpno )
	{
	case GR_DATE: GotoDlgCtrl( GetDlgItem( IDC_DATEFIX )); break;
	case GR_BMON: GotoDlgCtrl( GetDlgItem( IDC_BMNFIX )); break;
	case GR_KOJI: GotoDlgCtrl( GetDlgItem( IDC_KOJIFIX )); break;
	case GR_DEBT: GotoDlgCtrl( GetDlgItem( IDC_DEBTFIX )); break;
	case GR_CRED: GotoDlgCtrl( GetDlgItem( IDC_CREDFIX )); break;

	case GR_BMNCHK: GotoDlgCtrl( GetDlgItem( IDC_BMN_CHECK )); break; 
	case GR_BMNDSP: GotoDlgCtrl( GetDlgItem( IDC_BMNNAM_DSP )); break;
	case GR_BMNTKY:  GotoDlgCtrl( GetDlgItem( IDC_BMNTOSWK )); break;
	case GR_KJTKY:  GotoDlgCtrl( GetDlgItem( IDC_KOJITOSWK )); break;
	case GR_BMNSEL: GotoDlgCtrl( GetDlgItem( GetCheckedRadioButton( IDC_RADIOBMNSELYES, IDC_RADIOBMNSELNO))); break;
//	case GR_BMNBRN: GotoDlgCtrl( GetDlgItem( IDC_BMNBRN_REG )); break; 
	case GR_KJIINP:  GotoDlgCtrl( GetDlgItem( IDC_KJI_INPCHK )); break;
	case GR_KANKJI:  GotoDlgCtrl( GetDlgItem( IDC_KANKJI_CHK )); break;
	case GR_KJIKMK:  GotoDlgCtrl( GetDlgItem( IDC_KJIKMK_CHK )); break;

	case GR_BRN	:	GotoDlgCtrl( GetDlgItem( IDC_BRN_INPCHK )); break;
	case GR_BRTEK:	GotoDlgCtrl( GetDlgItem( IDC_BRNNAM_DSP )); break;
	case GR_BRCPY:	GotoDlgCtrl( GetDlgItem( IDC_BRN_CPY )); break;
	case GR_BRTOTKY: GotoDlgCtrl( GetDlgItem( IDC_BRNTOSWK )); break;
	case GR_SYGBRN:	GotoDlgCtrl( GetDlgItem( IDC_SYOBRN_DSP )); break;
#ifdef CENTURY_CLOSE
	case GR_BRNKAK:	GotoDlgCtrl( GetDlgItem( IDC_BRN_INPKAKU )); break;
#endif

	case GR_DENP:	GotoDlgCtrl( GetDlgItem( IDC_DENP_COMBO )); break;
	case GR_SNO:	GotoDlgCtrl( GetDlgItem( IDC_SNO_COMBO )); break;	//12.14 /12
	case GR_RDDATE:	GotoDlgCtrl( GetDlgItem( IDC_RD_DATE )); break;
	case GR_RDBRN:	GotoDlgCtrl( GetDlgItem( IDC_RD_BRN )); break;
	
	case GR_SYOBUZ:	GotoDlgCtrl( GetDlgItem( IDC_SYO_BUZZER )); break;
#ifdef DB_CLOSE
	case GR_SYIMPORT: GotoDlgCtrl( GetDlgItem( IDC_IMPORT_CHK )); break;
#endif
	case GR_HJIYU: GotoDlgCtrl( GetDlgItem( IDC_HJIYUTOSWK )); break;

	case GR_SWKTEK:	GotoDlgCtrl( GetDlgItem( GetCheckedRadioButton( IDC_RADIOSWKTEKYES, IDC_RADIOSWKTEKNO))); break;

	case GR_KMPAGE: GotoDlgCtrl( GetDlgItem( GetCheckedRadioButton( IDC_RADIOKMKEEP, IDC_RADIOKMTOP))); break;	//02.20/02
	case GR_KMCODE: GotoDlgCtrl( GetDlgItem( GetCheckedRadioButton( IDC_RADIOKMCODE, IDC_RADIOKMNUM))); break;	//02.20/02

	case GR_DCKMCOD: GotoDlgCtrl( &m_DBEdtKmk ); break;
	case GR_DCKMCHK: GotoDlgCtrl( GetDlgItem(IDC_BS_CHK) );	break;
	case GR_SCNCODE: GotoDlgCtrl( GetDlgItem(IDC_SCAN_KMCD) );	break;
	case GR_ZEIMDFY: GotoDlgCtrl( GetDlgItem(IDC_ZEIMDFY_CHK) );	break;
	case GR_DELSWK: GotoDlgCtrl( GetDlgItem(IDC_DELSWK_CHK) );	break;
	case GR_SCNCLR: GotoDlgCtrl( GetDlgItem(IDC_SCNCLR_CHK) );	break;	// 06.22 /09

	case GR_KANADISP:GotoDlgCtrl( GetDlgItem(IDC_KANADISP_CHK) );	break;

	case GR_VALCOPY:	// 09.08 /08
		if( ((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->GetCheck() == BST_CHECKED )
			GotoDlgCtrl( GetDlgItem(IDC_CHK_VALCOPY) );
		else
			GotoDlgCtrl( GetDlgItem(IDC_CHK_VALNOT) );
		break;

	case GR_TKYREG:		// 10.01 /08
		if( ((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->GetCheck() == BST_CHECKED )
			GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE1) );
		else if( ((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2))->GetCheck() == BST_CHECKED )
			GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE2) );
		else
			GotoDlgCtrl( GetDlgItem(IDC_CHK_TKYTYPE3) );
		break;

	case GR_ZMOVE:	// 06.30 /11
		if( ((CButton*)GetDlgItem(IDC_CHK_ZMVDATE))->GetCheck() == BST_CHECKED )
			GotoDlgCtrl( GetDlgItem(IDC_CHK_ZMVDATE) );
		else
			GotoDlgCtrl( GetDlgItem(IDC_CHK_ZMVZANC) );
		break;
	// 11.13 /12
	case GR_SWTKYD:	GotoDlgCtrl( GetDlgItem( GetCheckedRadioButton( IDC_RADIOSWKTKY1, IDC_RADIOSWKTKY2))); break;

	case GR_BMNTKBR:	GotoDlgCtrl( GetDlgItem( IDC_BMNTKBR_CHK )); break;
	case GR_TKTAB:		GotoDlgCtrl( GetDlgItem( IDC_CHK_TKYTAB )); break;
	case GR_BRNKANA:	GotoDlgCtrl( GetDlgItem( IDC_BRNSEL_KANA )); break;
	case GR_RTSWK:		GotoDlgCtrl( GetDlgItem( IDC_RTSWKKANA_CHK )); break;

	case GR_ENDKAKU:	GotoDlgCtrl( GetDlgItem( IDC_CHK_ENDKAKU )); break;
	case GR_DENPTYPE:	GotoDlgCtrl( GetDlgItem( IDC_CHK_DNPTYPE )); break;

	case GR_KANASEARCH:	GotoDlgCtrl(GetDlgItem(IDC_CHK_KMKANA)); break;
	case GR_SYZSTOP:	GotoDlgCtrl(GetDlgItem(IDC_CHK_SYZSTOP)); break;

	default:
		GotoDlgCtrl( GetDlgItem( IDOK ) ); break;
	}
}

// グループ番号ゲット
int CAutoInp::get_grpno()
{
CWnd *now = GetFocus();

	if( m_Sel.DATE_OPT != -1 && GetDlgItem( IDC_DATEFIX ) == now )
		return GR_DATE;
	else if( m_Sel.BMON_OPT != -1 && GetDlgItem( IDC_BMNFIX ) == now )
		return GR_BMON;
	else if( m_Sel.KOJI_OPT != -1 && GetDlgItem( IDC_KOJIFIX ) == now )
		return GR_KOJI;
	else if( m_Sel.DEBT_OPT != -1 && GetDlgItem( IDC_DEBTFIX ) == now )
		return GR_DEBT;
	else if( m_Sel.CRED_OPT != -1 && GetDlgItem( IDC_CREDFIX ) == now )
		return GR_CRED;

	else if( m_Sel.BMNCHK_OPT != -1 && GetDlgItem( IDC_BMN_CHECK ) == now ) 
		return GR_BMNCHK;
	else if( m_Sel.BMNDSP_OPT != -1 && GetDlgItem( IDC_BMNNAM_DSP ) == now )
		return GR_BMNDSP;
	else if( m_Sel.BMNTOTKY_OPT != -1 && GetDlgItem( IDC_BMNTOSWK ) == now )
		return GR_BMNTKY;
	else if( m_Sel.KJTOTKY_OPT != -1 && GetDlgItem( IDC_KOJITOSWK ) == now )
		return GR_KJTKY;
	else if( m_Sel.BMNSEL_OPT != -1 && GetDlgItem( GetCheckedRadioButton( IDC_RADIOBMNSELYES, IDC_RADIOBMNSELNO )) == now ) 
		return GR_BMNSEL;
//	else if( m_Sel.BMNBRN_OPT != -1 && GetDlgItem( IDC_BMNBRN_REG ) == now ) 
//		return GR_BMNBRN;
	else if( m_Sel.KJIINP_OPT != -1 && GetDlgItem( IDC_KJI_INPCHK ) == now )	// 01.21 /10
		return GR_KJIINP;
	else if( m_Sel.KANKJI_OPT != -1 && GetDlgItem( IDC_KANKJI_CHK ) == now )	// 01.21 /10
		return GR_KANKJI;
	else if( m_Sel.KJIKMK_OPT != -1 && GetDlgItem( IDC_KJIKMK_CHK ) == now )	// 03.11 /11
		return GR_KJIKMK;

	else if( m_Sel.BRN_OPT != -1 && GetDlgItem( IDC_BRN_INPCHK ) == now )
		return GR_BRN;
	else if( m_Sel.BRTEK_OPT != -1 && GetDlgItem( IDC_BRNNAM_DSP ) == now )
		return GR_BRTEK;
	else if( m_Sel.BRCPY_OPT != -1 && GetDlgItem( IDC_BRN_CPY ) == now ) // 08.02 /02
		return GR_BRCPY;
	else if( m_Sel.BRTOTKY_OPT != -1 && GetDlgItem( IDC_BRNTOSWK ) == now )
		return GR_BRTOTKY;
	else if( m_Sel.SYGBRN_OPT != -1 && GetDlgItem( IDC_SYOBRN_DSP ) == now )
		return GR_SYGBRN;
#ifdef CENTURY_CLOSE
	else if( m_Sel.BRNKAK_OPT != -1 && GetDlgItem( IDC_BRN_INPKAKU ) == now )	// 04.27 /12
		return GR_BRNKAK;
#endif
	else if( m_Sel.DENP_OPT != -1 && GetDlgItem( IDC_DENP_COMBO ) == now )
		return GR_DENP;
	else if( m_Sel.DOCEVI_OPT != -1 && GetDlgItem( IDC_SNO_COMBO ) == now )
		return GR_SNO;
	else if( m_Sel.RDDATE_OPT != -1 && GetDlgItem( IDC_RD_DATE ) == now ) 
		return GR_RDDATE;
	else if( m_Sel.RDBRN_OPT  != -1 && GetDlgItem( IDC_RD_BRN ) == now )
		return GR_RDBRN;

	else if( m_Sel.SYOBUZ_OPT != -1 && GetDlgItem( IDC_SYO_BUZZER ) == now ) // 04.03 /03
		return GR_SYOBUZ;
#ifdef DB_CLOSE
	else if( m_Sel.SYIMPORT_OPT != -1 && GetDlgItem( IDC_IMPORT_CHK ) == now )
		return GR_SYIMPORT;
#endif
	else if( m_Sel.HJIYU_OPT != -1 && GetDlgItem( IDC_HJIYUTOSWK ) == now )
		return GR_HJIYU;

	else if( m_Sel.SWKTEK_OPT != -1 && GetDlgItem( GetCheckedRadioButton( IDC_RADIOSWKTEKYES, IDC_RADIOSWKTEKNO )) == now ) // 12.09 /02
		return GR_SWKTEK;

	else if( m_Sel.KMPG_OPT != -1 && GetDlgItem( GetCheckedRadioButton( IDC_RADIOKMKEEP, IDC_RADIOKMTOP )) == now ) // 02.20 /02
		return GR_KMPAGE;
	else if( m_Sel.KMNO_OPT != -1 && GetDlgItem( GetCheckedRadioButton( IDC_RADIOKMCODE, IDC_RADIOKMNUM )) == now ) // 02.20 /02
		return GR_KMCODE;

	else if( m_Sel.DCKM_CODE[0] && &m_DBEdtKmk == now )
		return GR_DCKMCOD;
	else if( m_Sel.DCKM_OPT != -1 && GetDlgItem( IDC_BS_CHK ) == now ) // 02.20 /02
		return GR_DCKMCHK;
	else if( m_Sel.SCNCD_OPT != -1 && GetDlgItem( IDC_SCAN_KMCD ) == now )
		return GR_SCNCODE;
	else if( m_Sel.ZEIMDFY_OPT != -1 && GetDlgItem( IDC_ZEIMDFY_CHK ) == now )
		return GR_ZEIMDFY;
	else if( m_Sel.DELSWK_OPT != -1 && GetDlgItem( IDC_DELSWK_CHK ) == now )
		return GR_DELSWK;
	else if( m_Sel.SCNCLR_OPT != -1 && GetDlgItem( IDC_SCNCLR_CHK ) == now )	// 06.22 /09
		return GR_SCNCLR;

	else if( m_Sel.KANADSP_OPT != -1 && GetDlgItem( IDC_KANADISP_CHK ) == now ) 
		return GR_KANADISP;

	else if( GetDlgItem(IDC_CHK_VALCOPY) == now || GetDlgItem(IDC_CHK_VALNOT) == now )
		return GR_VALCOPY;	// 09.08 /08

	else if( GetDlgItem(IDC_CHK_TKYTYPE1) == now || GetDlgItem(IDC_CHK_TKYTYPE2) == now ||
			 GetDlgItem(IDC_CHK_TKYTYPE3) == now )
		return GR_TKYREG;	// 10.01 /08

	else if( GetDlgItem(IDC_CHK_ZMVDATE) == now || GetDlgItem(IDC_CHK_ZMVZANC) == now )
		return GR_ZMOVE;	// 06.30 /11

	else if( m_Sel.SWTKYD_OPT != -1 && GetDlgItem( GetCheckedRadioButton( IDC_RADIOSWKTKY1, IDC_RADIOSWKTKY2 )) == now ) // 11.13 /12
		return GR_SWTKYD;

	else if( m_Sel.BMNTKBR_OPT != -1 && GetDlgItem( IDC_BMNTKBR_CHK ) == now ) // 02.08 /13
		return GR_BMNTKBR;

	else if( m_Sel.BRNKANA_OPT != -1 && GetDlgItem( IDC_BRNSEL_KANA ) == now ) // 04.01 /15
		return GR_BRNKANA;

	else if( m_Sel.RTSWKKANA_OPT != -1 && GetDlgItem( IDC_RTSWKKANA_CHK ) == now ) // 06.23 /17
		return GR_RTSWK;

	else if( GetDlgItem( IDC_CHK_TKYTAB ) == now ) // 04.30 /13
		return GR_TKTAB;
	else if( GetDlgItem( IDC_CHK_ENDKAKU ) == now ) // 04.30 /13
		return GR_ENDKAKU;
	else if( GetDlgItem( IDC_CHK_DNPTYPE ) == now ) // 04.30 /13
		return GR_DENPTYPE;
	else if( GetDlgItem(IDC_CHK_KMKANA) == now )
		return GR_KANASEARCH;
	else if( GetDlgItem(IDC_CHK_SYZSTOP) == now )
		return GR_SYZSTOP;
	else
		return 0;
}	

// キーバック
int CAutoInp::grp_back( int grpno )
{
int i;

	for( i = grpno ? grpno-1 : GRP_MAX /*5*/ ; i >= 1 ; --i )
	{
		if( ok_grp( i ) )
			break;
	}
	if( i < 1 )
		return -1;
	else
		return i;
}

// キーフォア
int CAutoInp::grp_for( int grpno )
{
int i;
	
	if( grpno == 0 )
		return -1;

	for( i = grpno+1 ; i <= GRP_MAX/*5*/ ; ++i )
	{
		if( ok_grp( i ) )
			break;
	}
	if( i > GRP_MAX/*5*/ )
		return 0;
	else
		return i;
}

// 右キー
int CAutoInp::grp_right( int grpno )
{
int i, sno;
	
	if( grpno == 0 )
		return -1;

	switch( grpno ) {
	case GR_DATE : 	case GR_BMON : 
	case GR_KOJI : 
		sno = grpno + 5;
		break;
//	case GR_KOJI : 
//		sno = GR_BMNBRN;
//		break;
	case GR_DEBT : 	case GR_CRED :
		sno = GR_BMNSEL;
		break;
	case GR_DENP : 
		sno = GR_SYOBUZ;
		break;
	case GR_SNO :
		sno = GR_HJIYU;
		break;

	case GR_SYOBUZ : case GR_HJIYU /*GR_SYIMPORT*/ : 
		sno = GR_KMPAGE;
		break;

	case GR_BMNCHK : case GR_BMNDSP :
//	case GR_BMNBRN :
		sno = grpno + 7;
		break;
	case GR_RDDATE : case GR_RDBRN :
		sno = GR_DCKMCOD;
		break;

	case GR_BRN : case GR_BRTEK : case GR_BRCPY :
	case GR_BRTOTKY :
		sno = grpno - 11;
		break;
	case GR_SYGBRN :
#ifdef CENTURY_CLOSE
	case GR_BRNKAK :
#endif
//		sno = GR_DENP;
		sno = GR_BMNTKBR;
		break;
	
	case GR_BMNTKBR:
		sno = GR_KMPAGE;
		break;

	case GR_BMNTKY:
		sno = GR_BRCPY;
		break;
	case GR_KJTKY:
		sno = GR_BRTOTKY;
		break;
	case GR_KJIINP: case GR_KANKJI: case GR_KJIKMK:
#ifdef CENTURY_CLOSE
		sno = GR_BRNKAK;
#endif
		sno = GR_SYGBRN;
		break;

	case GR_DCKMCOD :
	case GR_VALCOPY: case GR_TKYREG	: case GR_ZMOVE:
 		sno = grpno + 1;
		break;
	case GR_DCKMCHK :
	case GR_SCNCODE : case GR_DELSWK: case GR_SCNCLR:
		sno = GR_KANADISP;
		break;

	case GR_ZEIMDFY:
		sno = GR_SCNCODE;
		break;

	default:
		return -1;
	}

	for( i = sno ; i <= GRP_MAX/*5*/ ; ++i )
	{
		if( ok_grp( i ) )
			break;
	}
	if( i > GRP_MAX/*5*/ )
		return 0;
	else
		return i;
}


// 左キー
int CAutoInp::grp_left( int grpno )
{
int i, sno;

	switch( grpno ) {
	case GR_BMON : case GR_KOJI : case GR_DEBT : case GR_CRED : 
		sno = grpno + 9;
		break;

	case GR_BMNCHK : case GR_BMNDSP :
//	case GR_BMNBRN :
		sno = grpno - 5;
		break;
	case GR_BRN : case GR_BRTEK : case GR_BRCPY : 
		sno = grpno - 7;
		break;
#ifdef CENTRUY_CLOSE
	case GR_BRTOTKY : case GR_SYGBRN : case GR_BRNKAK :
#endif
	case GR_BRTOTKY : case GR_SYGBRN :
		sno = GR_BMNSEL;
		break;
	case GR_DENP : case GR_RDDATE : case GR_RDBRN : case GR_DCKMCHK :
	case GR_ZEIMDFY:
	case GR_VALCOPY: case GR_TKYREG	: case GR_ZMOVE:
	case GR_SNO : 
		sno = grpno - 1;
		break;

	case GR_KJTKY:
		sno = GR_KOJI;
		break;
	case GR_KJIINP: case GR_KANKJI:	case GR_KJIKMK:
		sno = GR_CRED;
		break;
	case GR_KANADISP:
		sno = GR_DCKMCHK;
		break;

	case GR_SYOBUZ : case GR_HJIYU /*GR_SYIMPORT*/ :
		sno = GR_DENP;
		break;
	case GR_DCKMCOD :
		sno = grpno - 4;
		break;
	case GR_SCNCODE : case GR_DELSWK : case GR_SCNCLR:
		sno = GR_ZEIMDFY ;
		break;

	case GR_BMNTKBR:
		sno = GR_KJIINP;
		break;

	default:
		return -1;
	}

	for( i = sno ; i >= 1 ; --i )
	{
		if( ok_grp( i ) )
			break;
	}
	if( i < 1 )
		return -1;
	else
		return i;
}


// UP キー
int CAutoInp::grp_up( int grpno )
{
int i, sno;
	
	if( grpno == 0 )
		return -1;

	switch( grpno ) {
	case GR_DATE : case GR_BMNCHK : case GR_BRN :
		return -1;
		break;
	case GR_KMPAGE :	
		sno = GR_BMNTKBR;
		break;
	case GR_SYOBUZ :
		sno = GR_KJIKMK /*GR_BMNSEL*/;
		break;
	case GR_DCKMCOD: case GR_DCKMCHK:
		sno = GR_HJIYU;
		break;
	case GR_DENP : case GR_SNO :
		return -1;
		break;
	case GR_VALCOPY:
		sno = GR_ZEIMDFY;
		break;

	case GR_TKYREG:
		sno = GR_SCNCLR;
		break;

	case GR_ZMOVE:
		sno = GR_KANADISP;
		break;

	case GR_BMNTKBR:
		sno = GR_SYGBRN;
		break;

	default:
		sno = grpno - 1;
		break;
	}

	for( i = sno ; i >= 1 ; --i )
	{
		if( ok_grp( i ) )
			break;
	}
	if( i < 1 )
		return -1;
	else
		return i;

}


// DOWN キー
int CAutoInp::grp_down( int grpno )
{
int i, sno;

	switch( grpno ) {
	case GR_RDBRN : case GR_DCKMCOD :
		return 0;
		break;
	case GR_ZEIMDFY : case GR_SCNCLR :
		sno = GR_VALCOPY;
		break;
	case GR_CRED:
		sno = GR_DENP;	break;
	case GR_KJIKMK:
		sno = GR_SYOBUZ;	break;
	case GR_SYGBRN : 
	//	sno = GR_KMPAGE;	break;
		sno	= GR_BMNTKBR;	break;
	case GR_KMCODE:
		sno = grpno + 1;
		break;
	case GR_DENP : case GR_SNO :
		return -1;
		break;

	case GR_BMNTKBR:
		sno = GR_KMPAGE;	break;

	default:
		sno = grpno + 1;
		break;
	}

	for( i = sno ; i <= GRP_MAX/*5*/ ; ++i )
	{
		if( ok_grp( i ) )
			break;
	}
	if( i > GRP_MAX/*5*/ )
		return 0;
	else
		return i;

}

// Ｆ２　Ｆ３　キー処理
short CAutoInp::F2F3( short nChar )
{
int grpno = get_grpno();
int i;

	switch( nChar )
	{
/*	case VK_F3:
		if( grpno == 0 )
		{
			if( GetFocus() == GetDlgItem( IDOK ) )
			{
				GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
				return 0;
			}
			else
				return 0;
		}
		else
		{
			i = grp_for( grpno );
			if( i != -1 )
				goto_grp( i );
			return 0;
		}
*/
	case VK_F2:
		if( grpno == 0 )
		{
			if( GetFocus() == GetDlgItem( IDCANCEL ) )
			{
				GotoDlgCtrl( GetDlgItem( IDOK ) );
				return 0;
			}
		}

		if( grpno == GR_DATE ) {
#ifdef CLOSE
			m_selDate.SetFocus();
#endif
		}
		else {
			i = grp_back( grpno );
			if( i != -1 )
				goto_grp( i );
		}
		return 0;
	}
	return nChar;
}

// キー処理
short CAutoInp::OnTerm( short nChar )
{
int grpno = get_grpno();
int i;

	switch( nChar )
	{
	case VK_RETURN:
	case VK_TAB:
		if( grpno == 0 )
		{
			return nChar;
		}
		else if( nChar == VK_RETURN || nChar == VK_TAB )
		{
			i = grp_for( grpno );
			if( i != -1 )
				goto_grp( i );
			return 0;
		}

		return nChar;
		break;
	case VK_RIGHT:
		if( GetFocus() == GetDlgItem( IDCANCEL ) )
			return 0;

		i = grp_right( grpno );
		if( i != -1 ) {
			goto_grp( i );
			return 0;
		}
		break;
	case VK_LEFT:
		if( grpno == 0 )
		{
			if( GetFocus() == GetDlgItem( IDOK ) )
			{
				;
			}
			else	// == IDCANCEL
				return nChar;
		}
		else {
			i = grp_left( grpno );
			if( i != -1 ) {
				goto_grp( i );
				return 0;
			}
		}
		return nChar;
	case VK_UP:
		if( grpno == 0 && GetFocus() == GetDlgItem( IDOK ) )
		{
			i = grp_back( grpno );
			if( i != -1 )
				goto_grp( i );
			return 0;
		}
		else if( grpno > 0 && grpno != GR_DENP && grpno != GR_SNO ) {
			if( ! goto_grpcheck( grpno, VK_UP ) )
				return 0;

			i = grp_up( grpno );
			if( i != -1 ) {
				goto_grp( i );
			}
			return 0;
		}
		return nChar;

	case VK_DOWN:
		if( grpno == 0 )
		{
			if( GetFocus() == GetDlgItem( IDCANCEL ) )
				return 0;
		}
		else {
			if( ! goto_grpcheck( grpno, VK_DOWN ) )
				return 0;

			i = grp_down( grpno );
			if( i != -1 ) {
				goto_grp( i );
				return 0;
			}
		}
		return nChar;
	case VK_END:
		if( grpno )
		{
			goto_grp( 0 );
			return 0;
		}
		return nChar;
	}
	return nChar;
}

BOOL CAutoInp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->hwnd != m_DBEdtKmk.m_hWnd ) {
			if(  pMsg->wParam == alt_key )
			{
				GetDlgItem( IDCANCEL )->PostMessage( WM_LBUTTONDOWN, 0, 0 );
				GetDlgItem( IDCANCEL )->PostMessage( WM_LBUTTONUP, 0, 0 );
				return 1;
			}
			else if( GetKeyState( VK_SHIFT ) < 0 ) {
				if( pMsg->wParam == VK_TAB )
					pMsg->wParam = VK_F2;
			}
			
			if( F2F3( pMsg->wParam ) == 0 )
				return 1;
			else if( OnTerm( pMsg->wParam ) == 0 )
				return 1;
		}
		else {
			if( pMsg->wParam == VK_ESCAPE ) {
				OnCancel();
				return TRUE;
			}
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}


void CAutoInp::OnOK() 
{
	if( m_Sel.DENP_OPT != -1 ) {
		m_DenpSel = m_DENP.GetCurSel();
	}
	else	m_DenpSel = -1;

	// 09.08 /08
	int chk = ((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->GetCheck();
	ValEnterCopyMode() = (chk == BST_CHECKED) ? 0 : 1;
	// 10.01 /08
	chk = ((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->GetCheck();
	
	if( (chk == BST_CHECKED) ) {
		TkyRegMode() = 0;
	}
	else {
		chk = ((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3))->GetCheck();
		if( (chk == BST_CHECKED) ) {
			TkyRegMode() = 2;
		}
		else {
			TkyRegMode() = 1;
		}
	}
	
//	TkyRegMode() = (chk == BST_CHECKED) ? 0 : 1;

	// 06.30 /11
	chk = ((CButton*)GetDlgItem(IDC_CHK_ZMVDATE))->GetCheck();
	ZmoveMode() = (chk == BST_CHECKED) ? 0 : 1;

	// 02.07 /13
	chk = ((CButton*)GetDlgItem(IDC_CHK_TKYTAB))->GetCheck();
	TKtabMode() = (chk == BST_CHECKED) ? 1 : 0;

	// 11.14 /16
	if( IsCloudSetupEnv() ) {
		chk = ((CButton*)GetDlgItem(IDC_CHK_ENDKAKU))->GetCheck();
		JZcloudEndKakutei() = (chk == BST_CHECKED) ? 1 : 0;
	}
	// 08.21 /18
	chk = ((CButton*)GetDlgItem(IDC_CHK_DNPTYPE))->GetCheck();
	DenpTypeMode() = (chk == BST_CHECKED) ? 1 : 0;

	chk = ((CButton*)GetDlgItem(IDC_CHK_KMKANA))->GetCheck();
	KanaSearchType() = (chk == BST_CHECKED) ? 0 : 1;

	chk = ((CButton*)GetDlgItem(IDC_CHK_SYZSTOP))->GetCheck();
	SyzStopModeValue() = (chk == BST_CHECKED) ? 1 : 0;

#ifdef CLOSE
	m_selDate.GetMonthOffset( m_Sofs, m_Eofs, m_Sdate, m_Edate );
#endif

	if( m_Sel.DOCEVI_OPT != -1 ) {
		m_DEviSel = m_DEvi.GetCurSel();
	}
	else	m_DEviSel = -1;


	ICSDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CAutoInp, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CAutoInp)
	ON_EVENT(CAutoInp, IDC_ICSDBEDT_BSKMK, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtBskmk, VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT(CAutoInp, IDC_ICSDBEDT_BSKMK, 4 /* KanaTermination */, OnKanaTerminationIcsdbedtBskmk, VTS_BSTR)
	ON_EVENT(CAutoInp, IDC_ICSDBEDT_BSKMK, 1 /* Termination */, OnTerminationIcsdbedtBskmk, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CAutoInp::OnImeEndCompositionIcsdbedtBskmk(short nChar, LPCTSTR string, LPCTSTR ystring) 
{
#ifdef _20090307_INPCD_CHG_

	struct _KamokuData check, ans;
//	DBkamokudata_get( pwnd, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &tmp );
	
	KamokuStr_ChkLength( string, &check, Voln1->edcol );
	
	CString km_cod;
	km_cod = check.kd_code;

	struct _DBKNREC	*pKn;

	int ipc;
	ipc = atoi( km_cod );

	DWORD pjis = pDBzm->InpcToPjis( ipc );
	pKn = pDBzm->DB_PjisToKnrec( pjis );

	::ZeroMemory( &ans, sizeof(struct _KamokuData) );

	if( ! pKn  )
	{
		if( m_Sel.DCKM_CODE[0] ) {
			km_cod = m_Sel.DCKM_CODE;
		}
		else {
			km_cod = GNKIN;
		}

		pKn = pDBzm->DB_PjisToKnrec( km_cod );

		memcpy( (char *)ans.kd_name, (LPCTSTR)pKn->knnam, KMNLEN );
//		memcpy( ans.kd_code, pKn->kncod, KMKCD_LN );
		DWORDcodeToSTRcode( pKn->kncod, ans.kd_code, sizeof ans.kd_code );
		::ZeroMemory( m_Sel.DCKM_CODE, sizeof m_Sel.DCKM_CODE );
		memcpy( m_Sel.DCKM_CODE, ans.kd_code, KMKCD_LN );
		ans.kd_eda = -1;
		DBkamokudata_set( this, IDC_ICSDBEDT_BSKMK, &ans, pDBzm );

		return;
	}
	else
	{

		memcpy( (char *)ans.kd_name, (LPCTSTR)pKn->knnam, KMNLEN );
	//	memcpy( ans.kd_code, pKn->kncod, KMKCD_LN );
		DWORDcodeToSTRcode( pKn->kncod, ans.kd_code, sizeof ans.kd_code );
		::ZeroMemory( m_Sel.DCKM_CODE, sizeof m_Sel.DCKM_CODE );
		memcpy( m_Sel.DCKM_CODE, ans.kd_code, KMKCD_LN );
		ans.kd_eda = -1;

		DBkamokudata_set( this, IDC_ICSDBEDT_BSKMK, &ans, pDBzm );
	}
#else

	m_imeStr = string;

	HIMC	hImc;
	hImc = ImmGetContext(GetDlgItem(IDC_ICSDBEDT_BSKMK)->m_hWnd);
	ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext(GetDlgItem(IDC_ICSDBEDT_BSKMK)->m_hWnd, hImc);
	PostMessage(WM_RETROFOCUS);

#endif
}


LRESULT CAutoInp::OnRetroFocus(WPARAM wp, LPARAM lp)
{
	struct _KamokuData ans;
	CString km_cod;

	KSEL_PAR kpar;
	CRect rc;
	GetDlgItem(IDC_ICSDBEDT_BSKMK)->GetWindowRect(rc);

	pDBzm->knrec->Requery("", 0);

	kpar.ksPsgn = KSP_LEFTBOTTOM;
	kpar.ksFsgn = 0;
	kpar.ksLnum = 10;

	::ZeroMemory(kpar.ksKana, sizeof kpar.ksKana);
	strncpy_s(kpar.ksKana, sizeof kpar.ksKana, m_imeStr, sizeof kpar.ksKana - 1);
	if( m_imeStr== " " )
		strncpy_s(kpar.ksKana, sizeof kpar.ksKana, "0", sizeof kpar.ksKana - 1);

	kpar.ksKrng = 0;
	kpar.ksPos.x = rc.left;
	kpar.ksPos.y = rc.top;
	kpar.ksCwnd = this;
	kpar.ksStat = 0;
	kpar.ksFhwd = /*this->m_hWnd*/m_OK.m_hWnd;

	if( Kmk50onSelect(pDBzm, &kpar, this) ) {
		memmove((char *)ans.kd_name, (LPCTSTR)pDBzm->knrec->knnam, KMNLEN);
		strcpy_s(ans.kd_code, sizeof ans.kd_code, pDBzm->knrec->kncod);
		//		DWORDcodeToSTRcode( pDBzm->knrec->kncod, ans.kd_code );
		::ZeroMemory(m_Sel.DCKM_CODE, sizeof m_Sel.DCKM_CODE);
		memcpy(m_Sel.DCKM_CODE, ans.kd_code, KMKCD_LN);
		ans.kd_eda = -1;

		DBkamokudata_set(this, IDC_ICSDBEDT_BSKMK, &ans, pDBzm);
	}
	else {
		if( m_Sel.DCKM_CODE[0] ) {
			km_cod = m_Sel.DCKM_CODE;
		}
		else {
			km_cod = GNKINCode(pDBzm->zvol->apno);
		}

		struct _DBKNREC	*pKn;
		pKn = pDBzm->DB_PjisToKnrec(km_cod);

		memcpy((char *)ans.kd_name, (LPCTSTR)pKn->knnam, KMNLEN);
		//		memcpy( ans.kd_code, pKn->kncod, KMKCD_LN );
		DWORDcodeToSTRcode(pKn->kncod, ans.kd_code, sizeof ans.kd_code);
		::ZeroMemory(m_Sel.DCKM_CODE, sizeof m_Sel.DCKM_CODE);
		memcpy(m_Sel.DCKM_CODE, ans.kd_code, KMKCD_LN);
		ans.kd_eda = -1;
		DBkamokudata_set(this, IDC_ICSDBEDT_BSKMK, &ans, pDBzm);
	}
	return 1;
}


// 科目 ５０音検索
void CAutoInp::OnKanaTerminationIcsdbedtBskmk(LPCTSTR Kana) 
{
	KSEL_PAR kpar;
	CRect rc;
	GetDlgItem(IDC_ICSDBEDT_BSKMK)->GetWindowRect( rc );

	struct _KamokuData ans = {0};

	pDBzm->knrec->Requery("", 0);

	kpar.ksPsgn	= KSP_LEFTBOTTOM;
	kpar.ksFsgn = 0;
	kpar.ksLnum = 10;

	::ZeroMemory( kpar.ksKana, sizeof kpar.ksKana );
	strncpy_s( kpar.ksKana, sizeof kpar.ksKana, Kana, sizeof kpar.ksKana -1 );
//	kpar.ksKana[0] = Kana[0];

	kpar.ksKrng = 0;
	kpar.ksPos.x	= rc.left;
	kpar.ksPos.y	= rc.top;
	kpar.ksCwnd = this;
	kpar.ksStat = 0;
	kpar.ksFhwd = /*this->m_hWnd*/m_OK.m_hWnd;

	if( Kmk50onSelect(pDBzm, &kpar, this) ) {
		memmove( (char *)ans.kd_name, (LPCTSTR)pDBzm->knrec->knnam, KMNLEN );
		strcpy_s( ans.kd_code, sizeof ans.kd_code, pDBzm->knrec->kncod );
//		DWORDcodeToSTRcode( pDBzm->knrec->kncod, ans.kd_code );
		::ZeroMemory( m_Sel.DCKM_CODE, sizeof m_Sel.DCKM_CODE );
		memcpy( m_Sel.DCKM_CODE, ans.kd_code, KMKCD_LN );
		ans.kd_eda = -1;

		DBkamokudata_set( this, IDC_ICSDBEDT_BSKMK, &ans, pDBzm );
	}
}

void CAutoInp::OnTerminationIcsdbedtBskmk(long nChar, long inplen, long kst) 
{
	BOOL	shift	=	FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		shift	=	TRUE;
	}

	if( nChar == VK_RETURN || (nChar == VK_TAB && ! shift) || nChar == VK_DOWN ||
		nChar == VK_RIGHT /*|| nChar == VK_F3*/ ) {
		goto_grp( GR_DCKMCHK );
	}
	else if( nChar == VK_UP || (nChar == VK_TAB && shift) || nChar == VK_F2) {
		goto_grp( GR_HJIYU );
	}
	else if( nChar == VK_LEFT ) {
		goto_grp( GR_RDBRN );
	}
}


void CAutoInp::OnBnClickedChkValcopy()
{
	ValCheckCtrl(IDC_CHK_VALCOPY);
}

void CAutoInp::OnBnClickedChkValnot()
{
	ValCheckCtrl(IDC_CHK_VALNOT);
}


// 金額チェックボックスコントロール
void CAutoInp::ValCheckCtrl( UINT ID )
{
	if( ID == IDC_CHK_VALCOPY ) {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_UNCHECKED);
	}
	else if( ID == IDC_CHK_VALNOT ) {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_CHECKED);
	}
}

void CAutoInp::OnBnClickedChkTkytype1()
{
	TkyCheckCtrl( IDC_CHK_TKYTYPE1 );
}

void CAutoInp::OnBnClickedChkTkytype2()
{
	TkyCheckCtrl( IDC_CHK_TKYTYPE2 );
}

// 摘要登録チェックボックスコントロール
void CAutoInp::TkyCheckCtrl( UINT ID )
{
	// IDC_CHK_TKYTYPE3; --- 簡易＋枝番

	if( ID == IDC_CHK_TKYTYPE1 ) {
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3 ))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2 ))->SetCheck(BST_UNCHECKED);
	}
	else if( ID == IDC_CHK_TKYTYPE3 ) {
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3 ))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2 ))->SetCheck(BST_UNCHECKED);
	}
	else if( ID == IDC_CHK_TKYTYPE2 ) {
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE3 ))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_TKYTYPE2 ))->SetCheck(BST_CHECKED);
	}
}


void CAutoInp::OnBnClickedChkTkytype3()
{
	TkyCheckCtrl( IDC_CHK_TKYTYPE3 );
}

// 残高問合せ
void CAutoInp::OnBnClickedChkZmvdate()
{
	ZmoveCheckCtrl( IDC_CHK_ZMVDATE );
}

void CAutoInp::OnBnClickedChkZmvmnth()
{
	ZmoveCheckCtrl( IDC_CHK_ZMVZANC );
}

// 残高問合せ移行チェックボックスコントロール
void CAutoInp::ZmoveCheckCtrl( UINT ID )
{
	if( ID == IDC_CHK_ZMVDATE ) {
		((CButton*)GetDlgItem(IDC_CHK_ZMVDATE))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_ZMVZANC))->SetCheck(BST_UNCHECKED);
	}
	else if( ID == IDC_CHK_ZMVZANC ) {
		((CButton*)GetDlgItem(IDC_CHK_ZMVDATE))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_ZMVZANC))->SetCheck(BST_CHECKED);
	}
}


void CAutoInp::OnBnClickedChkKmkana()
{
	int chk = ((CButton*)GetDlgItem(IDC_CHK_KMKANA))->GetCheck();

	if( chk ) {
		m_DBEdtKmk.SetKanaTerm(TRUE, 1, ICSDBEDT_KTYPE_KANA);
	}
	else {
		m_DBEdtKmk.SetKanaTerm(TRUE, 10, ICSDBEDT_KTYPE_KANA);
	}
}

