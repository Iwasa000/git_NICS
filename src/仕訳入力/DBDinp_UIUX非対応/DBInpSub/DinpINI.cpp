// DinpINI.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

//#include "SETUPTBL.h"
#include "BrnTkyDef.h"

#include "DinpINI.h"
#include "../DBDinp/AP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// キー定義
#define DATE_OPTN		0
#define DENP_OPTN		1
#define BMON_OPTN		2
#define DEBT_OPTN		3
#define CRED_OPTN		4
#define BILL_OPTN		5
#define HUSEN_OPTN		6

#define BRN_OPTN		7
#define BRTEK_OPTN		8
#define BRTOTKY_OPTN	9
#define SYGBRN_OPTN		10
//#define SYIMPORT_OPTN	11
#define HJIYU_OPTN		11

// 08.02 /02
#define	BRCPY_OPTN		12	// 枝番のコピー
// 08.28 /02		
#define	BMNCHK_OPTN		13	// 部門チェック
// 12.09 /02
#define	SWKTEK_OPTN		14	// 仕訳対応摘要
// 04.03 /03
#define	SYOBUZ_OPTN		15	// 諸口ブザー
// 08.30 /04
#define	BMNDSP_OPTN		16	// 部門表示
#define	BMNSEL_OPTN		17	// 部門選択

#define	RDDATE_OPTN		18	// 定型仕訳 日付を入力
#define	RDBRN_OPTN		19	// 定型仕訳 枝番を入力
// 02.20 /02
#define	KMPG_OPTN		20	// 科目選択ﾍﾟｰｼﾞ設定
#define	KMNO_OPTN		21	// 科目コード設定

#define DCKM_OPTN		22

// 仕訳表示関係
#define DSPLINE_OPTN	23
#define	DSPDENP_OPTN	24
#define DSPBMON_OPTN	25
#define DSPKOJI_OPTN	26
#define DSPTGNO_OPTN	27
#define DSPTGDT_OPTN	28

// 項目幅関係
#define RESOL_OPTN		29
#define	WDSEQ_OPTN		30
#define WDDATE_OPTN		31
#define WDDENP_OPTN		32
#define WDBMON_OPTN		33
#define WDKOJI_OPTN		34
#define	WDDEBT_OPTN		35
#define WDCRED_OPTN		36
#define WDVAL_OPTN		37
#define WDZEI_OPTN		38
#define WDSYZ_OPTN		39
#define WDTKY_OPTN		40

#define KANADSP_OPTN	41
#define KOJI_OPTN		42
#define KJTOTKY_OPTN	43
#define SCNCD_OPTN		44

#define DSPHJIYU_OPTN	45	// 変動事由の表示
#define DSPZEI_OPTN		46	// 税額の表示
#define ZEIMDFY_OPTN	47	// 税額を修正可能
#define DELSWK_OPTN		48	// 取消仕訳処理

#define INP2BK_OPTN		49	// 部門･工事を借方・貸方とも入力
#define INDSGN_OPTN		50	// 個人兼業科目選択

//#define BMNBRN_OPTN		43	// 部門枝番自動登録 --- CUT

// '08.12.01
#define	NPJGY_OPTN		51	// 公益事業費選択
#define	NPKNR_OPTN		52	// 公益管理費選択

// 06.22 /09
#define	SCNCLR_OPTN		53

// 01.21 /10
#define KJIINP_OPTN		54
#define KANKJI_OPTN		55
// 03.14 /11
#define KJIKMK_OPTN		56
// 04.27 /12
#define BRNKAK_OPTN		57

#ifdef _SLIP_ACCOUNTS_
// 09.28 /12
#define NOWSLIP_SEQ		58
#endif
// 11.01 /12
#define BLKDISP_OPTN	59
// 11.13 /12
#define SWKTKYD_OPTN	60
// 12.14 /12
#define DOCEVI_OPTN		61

// 02.08 /13	部門摘要枝番を使用する。
#define BMNTKBR_OPTN	62
// 04.01 /15	枝番カナ順
#define BRNKANA_OPTN	63
// 06.23 /17
#define RTSWKKANA_OPTN	64
// 06.07 /18
#define BMNTOTKY_OPTN	65
// 11.05 /20
#define DSPSYZ_OPTN		66	// 消費税欄の表示

//変動事由項目幅
#define	WDHJYU_OPTN		67
//出納帳の項目幅
#define	AWSEQ_OPTN		68
#define AWDATE_OPTN		69
#define AWDENP_OPTN		70
#define AWBMON_OPTN		71
#define AWKOJI_OPTN		72
#define	AWKMK_OPTN		73
#define AWNKN_OPTN		74
#define AWSKN_OPTN		75
#define AWZEI_OPTN		76
#define AWSYZ_OPTN		77
#define AWTKY_OPTN		78
#define AWHJYU_OPTN		79
//証憑番号項目幅
#define WDSYON_OPTN		80
#define AWSYON_OPTN		81


// キーMax
#define Key_Max			(AWSYON_OPTN+1)


// '08.12.01：科目コード
const	unsigned long	NP_JGYKCD		= 0x00000009;	// 公益：事業費
const	unsigned long	NP_KNRKCD		= 0x0000000a;	// 公益：管理費
const	unsigned long	NP_ALLINIKCD	= 0xffffffff;	// 公益：全出力初期化用コード

//
// key name get
static char *get_keyname( int key )
{
	char *p;

	switch( key )
	{
	case DATE_OPTN: p = "DATE"; break;
	case DENP_OPTN: p = "DENP"; break;
	case BMON_OPTN: p = "BMON"; break;
	case DEBT_OPTN: p = "DEBT"; break;
	case CRED_OPTN: p = "CRED"; break;
	case BILL_OPTN: p = "BILL"; break;
	case HUSEN_OPTN: p = "HUSEN"; break;
	
	case BRN_OPTN:	p = "BRN"; break;
	case BRTEK_OPTN:	p = "BRTEK"; break;
//	case SYIMPORT_OPTN: p = "SYIMPORT"; break;
	case HJIYU_OPTN:	p = "HJIYU";	break;
	case BRTOTKY_OPTN:	p = "BRTOTKY"; break;

	case SYGBRN_OPTN: p = "SYGBRN"; break;
	case BRCPY_OPTN:  p = "BRCPY"; break;
	case BMNCHK_OPTN: p = "BMNCHK"; break;
	case SWKTEK_OPTN: p = "SWKTEK"; break;
	case SYOBUZ_OPTN: p = "SYOBUZ"; break;
	case BMNDSP_OPTN: p = "BMNDSP"; break;
	case BMNSEL_OPTN: p = "BMNSEL"; break;

	case RDDATE_OPTN: p = "RDDATE"; break;
	case RDBRN_OPTN:  p = "RDBRN";  break;
	// 科目選択画面
	case KMPG_OPTN:	p = "KMPG";	break;
	case KMNO_OPTN:	p = "KMNO";	break;
	// 現金貸借の設定
	case DCKM_OPTN:		p = "DCKM";	break;
	// 表示設定
	case DSPLINE_OPTN:	p = "DSPLINE";	break;
	case DSPDENP_OPTN:	p = "DSPDENP";	break;
	case DSPBMON_OPTN:	p = "DSPBMON";	break;
	case DSPKOJI_OPTN:	p = "DSPKOJI";	break;
	case DSPTGNO_OPTN:	p = "DSPTGNO";	break;
	case DSPTGDT_OPTN:	p = "DSPTGDT";	break;
	// 項目幅関係
	case RESOL_OPTN:	p = "RESOL";	break;
	case WDSEQ_OPTN:	p = "WSEQ";		break;
	case WDDATE_OPTN:	p = "WDATE";	break;
	case WDDENP_OPTN:	p = "WDENP";	break;
	case WDBMON_OPTN:	p = "WBMON";	break;
	case WDKOJI_OPTN:	p = "WKOJI";	break;
	case WDDEBT_OPTN:	p = "WDEBT";	break;
	case WDCRED_OPTN:	p = "WCRED";	break;
	case WDVAL_OPTN:	p = "WVAL";	break;
	case WDZEI_OPTN:	p = "WZEI";	break;
	case WDSYZ_OPTN:	p = "WSYZ";	break;
	case WDTKY_OPTN:	p = "WTKY";	break;
	case KANADSP_OPTN:	p = "KANADISP";	break;
	case KOJI_OPTN:		p = "KOJI";		break;
	case KJTOTKY_OPTN:	p = "KJTOTKY";	break;
	case SCNCD_OPTN:	p = "SCNCD";	break;
	case DSPHJIYU_OPTN:	p = "DSPHJIYU";	break;
	case DSPZEI_OPTN:	p = "DSPZEI";	break;
	case ZEIMDFY_OPTN:	p = "ZEIMDFY";	break;
	case DELSWK_OPTN:	p = "DELSWK";	break;

	case INP2BK_OPTN:	p = "INP2BK";	break;
	case INDSGN_OPTN:	p = "INDSGN";	break;
	// '08.12.01
	case NPJGY_OPTN:	p = "NPJGY";	break;
	case NPKNR_OPTN:	p = "NPKNR";	break;
	// 06.22 /09
	case SCNCLR_OPTN:	p = "SCNCLR";	break;
	// 01.21 /10
	case KJIINP_OPTN:	p = "KJIINP";	break;
	case KANKJI_OPTN:	p = "KANKJI";	break;
	// 03.14 /11
	case KJIKMK_OPTN:	p = "KJIKMK";	break;
	// 04.27 /12
	case BRNKAK_OPTN:	p = "BRNKAK";	break;	
#ifdef _SLIP_ACCOUNTS_
	// 09.28 /12
	case NOWSLIP_SEQ:	p = "SLIPSEQ";	break;	
#endif
	// 11.01 /12
	case BLKDISP_OPTN:	p = "BLKD";	break;
	// 11.13 /12
	case SWKTKYD_OPTN:	p = "STKYD";	break;
	// 12.14 /12
	case DOCEVI_OPTN:	p = "DOCEVI";	break;
	// 02.08 /13
	case BMNTKBR_OPTN:	p = "BMNTKBR";	break;
	// 04.01 /15
	case BRNKANA_OPTN:	p = "BRNKANA";	break;
	// 06.23 /17
	case RTSWKKANA_OPTN:	p = "RTSWK";	break;
	// 06.07 /18
	case BMNTOTKY_OPTN:		p = "BMNTOTKY";	break;
	// 11.05 /20
	case DSPSYZ_OPTN:		p = "DSPSYZ";	break;
	// 出納帳項目幅
	case WDHJYU_OPTN:	p = "WDHJYU";	break;
	case AWSEQ_OPTN:	p = "AWSEQ";	break;
	case AWDATE_OPTN:	p = "AWDATE";	break;
	case AWDENP_OPTN:	p = "AWDENP";	break;
	case AWBMON_OPTN:	p = "AWBMON";	break;
	case AWKOJI_OPTN:	p = "AWKOJI";	break;
	case AWKMK_OPTN:	p = "AWKMK";	break;
	case AWNKN_OPTN:	p = "AWNKN";	break;
	case AWSKN_OPTN:	p = "AWSKN";	break;
	case AWZEI_OPTN:	p = "AWZEI";	break;
	case AWSYZ_OPTN:	p = "AWSYZ";	break;
	case AWTKY_OPTN:	p = "AWTKY";	break;
	case AWHJYU_OPTN:	p = "AWHJYU";	break;

	case WDSYON_OPTN:	p = "WDSYON";	break;
	case AWSYON_OPTN:	p = "AWSYON";	break;
	default: return 0;
	}

	return p;
}

static
char seps[]   = ",";

int _get_dinpsetup( int key, LPCTSTR tblstring, int* par, CString* parstr)
{
	char* p, *token, *next_token1;
	char buf[128];
	CString str1;
	CString tmp1, tmp2;
	int i, ok;
	
	p = get_keyname( key );

	ok = 0;
	str1 = tblstring;
	token = strtok_s( (char*)(LPCTSTR)str1, seps, &next_token1 );
	while( token != NULL )
	{
		::ZeroMemory( buf, sizeof buf );
		for( i = 0; token[i] != 0; i++ ) {
			if( token[i] == '=' ) {
				if( strcmp( p, buf ) == 0 ) {
					strcpy_s( buf, sizeof buf, &token[i+1] );
					ok = 1;
				}
				break;
			}
			else {
				buf[i] = token[i];
			}
		}
		if( ok ) {
			tmp1.Empty();
			tmp2.Empty();
			tmp1 = buf;
			int n = tmp1.Find( '+' );
			if( n != -1 ) {
				LPCTSTR ptr = (LPCTSTR)tmp1;

				tmp2.Format( "%s", &ptr[n+1] );
				tmp1.SetAt( n, '\0' );
			}

			*par = atoi( tmp1 );
			*parstr = tmp2;
			break;
		}
		/* 次のトークンを取得します。 */
		token = strtok_s( NULL, seps, &next_token1 );
	}

	return ok ? 0 : -1;
}


int _get_blkpara( BYTE* pBlk, int blCnt, CString parstr)
{
	char* p, *token, *next_token1;
	char buf[128];
	CString str1;
	CString tmp1, tmp2;
	int i, n, ok, ci;

	for( n = 0; n < blCnt; n++ ) {
		*(pBlk+n) = 0;
	}

	::ZeroMemory( buf, sizeof buf );

	ci = n = 0;

	for( i = 0; parstr[i] != 0; i++ ) {
		if( parstr[i] == '_' ) {
			*(pBlk+n) = atoi( buf );
			::ZeroMemory( buf, sizeof buf );
			ci = 0;
			n++;
			if( n >= blCnt )	break;
		}
		else {
			buf[ci++] = parstr[i];
		}
	}

	if( n < blCnt ) {
		if( ci > 0 ) {
			*(pBlk+n) = atoi( buf );
		}
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CDinpINI

CDinpINI::CDinpINI()
{
	memset( &m_AUTOSEL, '\0', sizeof(struct _AUTOSEL) );
	m_pZm	= NULL;
//	m_pMsu	= NULL;

	m_User_id	= -1;
//TRACE( "@CDinpINI %d\n", sizeof m_AUTOSEL );
}

CDinpINI::~CDinpINI()
{
}

void CDinpINI::Init( CDBZmSub *pZm/*, CMastSetUp *msu*/ )
{
	m_pZm	= pZm;
	// ユーザー固有情報を検索
	CString filter;
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;
	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl( utbl );
		m_User_id = utbl.user_id;
	}

//	m_pMsu	= msu;
}


//
//			syogbrn_ok()
//		資金繰諸口枝番ＯＫマスタ？
//			->	TRUE ... ok
//				FALSE .. No
//
//BOOL CDinpINI::syogbrn_ok( unsigned char apno )
BOOL CDinpINI::syogbrn_ok( CZVOLUME* zvol )
{
	switch( zvol->apno )
	{
	case AP_HOJIN:		// 法人
	case AP_KENSETU:	// 建設
	case AP_IRYO:		// 医療
	case AP_UNSO:		// 運送業 ... 12.15 /03
	//宗教除く非営利
	case AP_PUB:
	case AP_PUB3:
	case AP_SOCIAL:
	case AP_SOCIAL2:
	case AP_SCHOOL:
		return TRUE;
	case AP_KOJIN:		// 個人兼業対応
		if( (zvol->ind_table&0x01) )
			return FALSE;
		else
			return TRUE;
	default:
		return FALSE;
	}
}



int CDinpINI::get_dinpsetup( char* key, int* par, CString* parstr )
{
//old function
ASSERT( FALSE );
	return -1;
//odl function

#ifdef OLD_CLOSE
	CSETUPTBL* sup;
	sup = m_pMsu->setup;
	int ret = -1;

	sup->MoveFirst();

	if( sup->st != -1 ) {
		do {
			if( sup->setupname == key ) {
				ret		= 0;
				*par	= sup->param;
				*parstr = sup->parastr;
			}
		} while( sup->MoveNext() == 0 );
	}

	return ret;
#endif

}

#ifdef _20081201_NP_EXTEND_
int CDinpINI::get_dinpsetup( int key, int* par, CString* parstr )
{
	COWNTB* otb;
	otb = m_pZm->owntb;
	int ret = -1;

	if( otb == NULL )
		return -1;
	if( otb->st != 0 )
		return -1;

	if( key <= RDBRN_OPTN ) {
		*par = otb->vl[key];
		ret = 0;
	}
	else if( key <= DSPTGDT_OPTN ) {
		ret = _get_dinpsetup( key, otb->str[0], par, parstr );
	}
	else if( key <= WDTKY_OPTN ) {
		ret = _get_dinpsetup( key, otb->str[1], par, parstr );
	}
	else {
	//	if( key != BMNBRN_OPTN )
			ret = _get_dinpsetup( key, otb->str[2], par, parstr );
	//	else {
	//		ret = 0;
	//		*par = (m_pZm->zvol->bm_isw&0x02) ? 1 : 0;
	//	}
	}

	return ret;
}
#endif

int CDinpINI::get_dinpsetup( int key, int* par, CString* parstr )
{
	COWNTB* otb;
	otb = m_pZm->owntb;
	int ret = -1;

	if( otb == NULL )
		return -1;
	if( otb->st != 0 )
		return -1;

	if( key <= RDBRN_OPTN ) {
		*par = otb->vl[key];
		ret = 0;
	}
	else if( key <= DSPTGDT_OPTN ) {
		ret = _get_dinpsetup( key, otb->str[0], par, parstr );
	}
	else if( key <= WDTKY_OPTN ) {
		ret = _get_dinpsetup( key, otb->str[1], par, parstr );
	}
	else if( key <= INDSGN_OPTN ){
		ret = _get_dinpsetup( key, otb->str[2], par, parstr );
	}
	else if( key <= SWKTKYD_OPTN ) {
		ret = _get_dinpsetup( key, otb->str[3], par, parstr );
	}
	else if( key <= AWSEQ_OPTN ) {
		ret = _get_dinpsetup( key, otb->str[4], par, parstr );
	}
	else if( key <= AWHJYU_OPTN ) {
		ret = _get_dinpsetup(key, otb->str[5], par, parstr);
	}
	else {
		ret = _get_dinpsetup(key, otb->str[6], par, parstr);
	}

	return ret;
}

//
// 入力パラメータのゲット
//	->	struct _AUTOSEL *
struct _AUTOSEL *CDinpINI::GetDinpSW()
{
	struct _AUTOSEL *psel;
	int key, param;
	CString	parastr, filter;
	CString tblstr, tmp;
	char *pkey;
	BOOL bOwnOpen	= FALSE;
	BOOL bZvolOpen	= FALSE;

	// 会社 再選択のためクリア
	memset( &m_AUTOSEL, '\0', sizeof(struct _AUTOSEL) );
	
	psel = &m_AUTOSEL;

	if( m_pZm == NULL /*|| m_pMsu == NULL*/ )
		return psel;

	tblstr.Empty();

	filter.Format( "apl_name = '%s' AND itm_name = '%s' AND u_id = %d", INI_AppName, INI_ItmName, m_User_id );

	// COWNTB OpenされていなければOPEN
	if( m_pZm->owntb == NULL ) {
		if( m_pZm->OwnTblOpen( filter ) == ERR )
			return NULL;
		bOwnOpen = TRUE;
	}
	else {
		m_pZm->owntb->Requery( filter );
	}
	// ZVOLUME OpenされていなければOPEN
	if( m_pZm->zvol == NULL ) {
		if( m_pZm->VolumeOpen() )
			return NULL;
		bZvolOpen = TRUE;
	}

	for( key = DATE_OPTN ; key < Key_Max ; ++key )
	{
		// キーNameGet
		if( pkey = get_keyname( key ) )
		{
			param = 0;

			if( get_dinpsetup( key, &param, &parastr ) != 0 ) 
			{
#ifdef KEEP
				if( key == BRN_OPTN )		// 枝番チェックをデフォルト 03.19 /02
					strcpy( txt, "OPT_RGSCHK" );
				else if( key == BRCPY_OPTN)	// 枝番コピーをデフォルト	08.02 /02
					strcpy( txt, "OPT_BRCPY" );
				else if( key == SYGBRN_OPTN )
					strcpy( txt, "OPT_DSP" );
				else if( key == KMPG_OPTN )		// ページ保持をデフォルトにする
					strcpy( txt, "OPT_KMPG" );
				else if( key == BRTEK_OPTN )	// 枝番名称表示をデフォルト 03.19 /02
					strcpy( txt, "OPT_DSP" );
				else
					strcpy( txt, "OPT_NONOPT" );
#endif
				switch( key ) {
				case BRN_OPTN:	// 枝番チェックをデフォルト 03.19 /02
				case BRCPY_OPTN:// 枝番コピーをデフォルト	08.02 /02
				case SYGBRN_OPTN://諸口枝番表示をデフォルト
				case KMPG_OPTN:	// ページ保持をデフォルト
				case BRTEK_OPTN:// 枝番名称表示をデフォルト 03.19 /02
					param = 1;
					break;
#ifdef _SLIP_ACCOUNTS_
				case NOWSLIP_SEQ:	// ドキュメントシーケンス 09.28 /12
					param = -1;
					break;
#endif
				case BMNCHK_OPTN:	// 部門登録チェック	01.25 /13
					param = 1;
					break;

				case KMNO_OPTN:		//科目コード出力サインより [06/10 13]
				case SCNCD_OPTN:
					if( m_pZm->zvol->kc_psw&0x01 ) param = 1;
					break;
				default:
					param = 0;
					break;
				}
			}

			switch( key )
			{
			case DATE_OPTN:
				if( psel->DATE_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_FIX" )*/ )
					{
						// 日付固定
						psel->DATE_OPT = OPT_FIX;
					}
					else
					{
						// オプションなし
						psel->DATE_OPT = OPT_NONOPT;
					}
				break;

			case DENP_OPTN:
				// ボリュームラベルから
				if( psel->DENP_OPT != -1 )
				{
					if( param == CNO_AUTO /*!strcmp( txt, "OPT_AUTO" )*/ )
					{
						psel->DENP_OPT = OPT_AUTO;
					}
					else if( param == CNO_EQU /*!strcmp( txt, "OPT_EQU" )*/ )
					{
						// =SEQ
						psel->DENP_OPT = OPT_EQU;
					}
					else if( param == CNO_NON /*!strcmp( txt, "OPT_NON" )*/ )
					{
						// 伝票番号なし
						psel->DENP_OPT = OPT_NON;
					}
					else
					{
						// オプションなし
						psel->DENP_OPT = OPT_NONOPT;
					}
				}
				break;

			case BMON_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x02) )
					psel->BMON_OPT = -1;	// 部門なし
				else if( psel->BMON_OPT != -1 )
				{
					if( param == 1 /*!strcmp( txt, "OPT_FIX" )*/ )
					{
						// 部門番号固定
						psel->BMON_OPT = OPT_FIX;
					}
					else
					{
						// オプションなし
						psel->BMON_OPT = OPT_NONOPT;
					}
				}
				break;

			case DEBT_OPTN:
				if( psel->DEBT_OPT != -1 )	
					if( param == 1 /*!strcmp( txt, "OPT_FIX" )*/ )
					{
						// 借方固定
						psel->DEBT_OPT = OPT_FIX;
					}
					else
					{
						// オプションなし
						psel->DEBT_OPT = OPT_NONOPT;
					}
				break;

			case CRED_OPTN:
				if( psel->CRED_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_FIX" )*/ )
					{
						// 貸方固定
						psel->CRED_OPT = OPT_FIX;
					}
					else
					{
						// オプションなし
						psel->CRED_OPT = OPT_NONOPT;
					}
				break;

			case BRN_OPTN:
				if( !(m_pZm->zvol->br_sw & 0x01) )
					psel->BRN_OPT = -1;	// 枝番なし
				else if( psel->BRN_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_RGSCHK" )*/ )
					{
						// 未入力チェック
						psel->BRN_OPT = OPT_RGSCHK;
					}
					else
					{
						// オプションなし
						psel->BRN_OPT = OPT_NONOPT;
					}
				break;

			case BRTEK_OPTN:
				if( !(m_pZm->zvol->br_sw & 0x01) )
					psel->BRTEK_OPT = -1;	// 枝番なし
				else if( psel->BRTEK_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_DSP" )*/ )
					{
						// 枝番摘要表示
						psel->BRTEK_OPT = OPT_DSP;
					}
					else
					{
						// オプションなし
						psel->BRTEK_OPT = OPT_NONOPT;
					}
				break;

			case BRTOTKY_OPTN:
				if( !(m_pZm->zvol->br_sw & 0x01) )
					psel->BRTOTKY_OPT = -1;	// 枝番なし
				else if( psel->BRTOTKY_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_EXEC" )*/ )
					{
						// 枝番摘要実行
						psel->BRTOTKY_OPT = OPT_EXEC;
					}
					else
					{
						// オプションなし
						psel->BRTOTKY_OPT = OPT_NONOPT;
					}
				break;

			case SYGBRN_OPTN:
				if( !syogbrn_ok( m_pZm->zvol ) )
					psel->SYGBRN_OPT = -1;	// 枝番なし
				else if( psel->SYGBRN_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_DSP" )*/ )
					{
						// 資金繰諸口枝番名称表示
						psel->SYGBRN_OPT = OPT_DSP;
					}
					else
					{
						// オプションなし
						psel->SYGBRN_OPT = OPT_NONOPT;
					}
				break;

#ifdef DB_CLOSE
			case SYIMPORT_OPTN:
				if( psel->SYIMPORT_OPT != -1 )
					if( param == 1/*!strcmp( txt, "OPT_AUTO" )*/ )
					{
						// 輸入仕訳自動生成
						psel->SYIMPORT_OPT = OPT_AUTO;
					}
					else
					{
						// オプションなし
						psel->SYIMPORT_OPT = OPT_NONOPT;
					}
				break;
#endif
			case HJIYU_OPTN:
				if( psel->HJIYU_OPT != -1 )
					if( param == 1 ){
						// 変動事由 -> 仕訳摘要
						psel->HJIYU_OPT = OPT_HJIYU;
					}
					else{
						// オプションなし
						psel->HJIYU_OPT = OPT_NONOPT;
					}
				break;


			case BRCPY_OPTN:	// 08.02 /02
				if( !(m_pZm->zvol->br_sw & 0x01) )
					psel->BRCPY_OPT = -1;	// 枝番なし
				else if( psel->BRCPY_OPT != -1 )
					if( param == 1/*!strcmp( txt, "OPT_BRCPY" )*/ )
					{
						// 枝番コピー する
						psel->BRCPY_OPT = OPT_BRCPY;
					}
					else
					{
						// オプションなし
						psel->BRCPY_OPT = OPT_NONOPT;
					}
				break;

			case BMNCHK_OPTN:	// 08.28 /02
				if( !(m_pZm->zvol->sub_sw&0x02) && !(m_pZm->zvol->sub_sw&0x04) )
					psel->BMNCHK_OPT = -1;	// 部門なし

				if( psel->BMNCHK_OPT != -1 )
					if( param == 1/*!strcmp( txt, "OPT_BMNCHK" )*/ )
					{
						// 部門チェックする
						psel->BMNCHK_OPT = OPT_BMNCHK;
					}
					else
					{
						// オプションなし
						psel->BMNCHK_OPT = OPT_NONOPT;
					}
				break;

			case SWKTEK_OPTN:	// 12.09 /02
				if( psel->SWKTEK_OPT != -1 ) {
					if( param == 1/*!strcmp( txt, "OPT_SWKTEK" )*/ )
					{
						// 仕訳摘要 を優先
						psel->SWKTEK_OPT = OPT_SWKTEK;
					}
					else
					{
						// オプションなし
						psel->SWKTEK_OPT = OPT_NONOPT;
					}
				}
				break;

			case SYOBUZ_OPTN:	// 04.03 /03
				
				if( psel->SYOBUZ_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_SYOBUZ" )*/ )
					{
						// 諸口ブザー 鳴らす
						psel->SYOBUZ_OPT = OPT_SYOBUZ;
					}
					else
					{
						// 鳴らさない
						psel->SYOBUZ_OPT = OPT_NONOPT;
					}
				break;

			case BMNDSP_OPTN:	// 08.30 /04
				if( !(m_pZm->zvol->sub_sw&0x02) && !(m_pZm->zvol->sub_sw&0x04) )
					psel->BMNDSP_OPT = -1;

				if( psel->BMNDSP_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_BMNDSP" )*/ )
					{
						// 部門名称表示
						psel->BMNDSP_OPT = OPT_BMNDSP;
					}
					else
					{
						// しない
						psel->BMNDSP_OPT = OPT_NONOPT;
					}
				break;

			case BMNSEL_OPTN:	// 08.30 /04
				if( !(m_pZm->zvol->sub_sw&0x02) && !(m_pZm->zvol->sub_sw&0x04) )
					psel->BMNSEL_OPT = -1;

				if( psel->BMNSEL_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_BMNSEL" )*/ )
					{
						psel->BMNSEL_OPT = OPT_BMNSEL;	//画面番号順
					}
					else
					{
						psel->BMNSEL_OPT = OPT_NONOPT;	//部門番号順
					}
				break;
#ifdef OLD_CLOSE
			case BMNBRN_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x02)  )
					psel->BMNBRN_OPT = -1;

				if( psel->BMNBRN_OPT != -1 )
					if( param == 1 ){
						// 部門枝番残高自動登録
						psel->BMNBRN_OPT = OPT_BMNBRN;
					}
					else {
						// しない
						psel->BMNBRN_OPT = OPT_NONOPT;
					}
				break;
#endif

			case RDDATE_OPTN:	// 08.30 /04
				
				if( psel->RDDATE_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_RDDATE" )*/ )
					{
						// 定型仕訳 日付を入力する
						psel->RDDATE_OPT = OPT_RDDATE;
					}
					else
					{
						// しない
						psel->RDDATE_OPT = OPT_NONOPT;
					}
				break;

			case RDBRN_OPTN:	// 08.30 /04
				
				if( psel->RDBRN_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_RDBRN" )*/ )
					{
						// 定型仕訳 枝番を入力する
						psel->RDBRN_OPT = OPT_RDBRN;
					}
					else
					{
						// しない
						psel->RDBRN_OPT = OPT_NONOPT;
					}
				break;

			case KMPG_OPTN:		// 02.20 /02
				if( psel->KMPG_OPT != -1 ) {
					if( param == 1 /*!strcmp( txt, "OPT_KMPG" )*/ ) {
						// 科目ﾍﾟｰｼﾞ保持
						psel->KMPG_OPT = OPT_KMPG;
					}
					else {
						// 毎回先頭ﾍﾟｰｼﾞ
						psel->KMPG_OPT = OPT_NONOPT;
					}
				}
				break;

			case KMNO_OPTN:		// 02.20 /02
				if( psel->KMNO_OPT != -1 ) {
					if( param == 1/*!strcmp( txt, "OPT_KMNO" )*/ ) {
						// 科目コードを表示
						psel->KMNO_OPT = OPT_KMNO;
					}
					else {
						// 通常の番号表示
						psel->KMNO_OPT = OPT_NONOPT;
					}
				}
				break;

			case DCKM_OPTN:	// 現金貸借の設定
				if( psel->DCKM_OPT != -1 ) {
					if( param == 1 ) {
						// 現金貸借を表示しない
						psel->DCKM_OPT = OPT_NODSP;
					}
					else {
						// 現金貸借を表示する
						psel->DCKM_OPT = OPT_NONOPT;
					}
					if( parastr.IsEmpty() ) {
						strcpy_s( psel->DCKM_CODE, sizeof psel->DCKM_CODE, GNKINCode(m_pZm->zvol->apno) );
					}
					else {
						strncpy_s( psel->DCKM_CODE, sizeof psel->DCKM_CODE, parastr, KMKCD_LN );
					}
				}
				break;

			case DSPLINE_OPTN:
				if( param == 1 ) {
					psel->DSPLINE_OPT = 2;	// 仕訳２行表示
				}
				else {
					psel->DSPLINE_OPT = 1;	// 仕訳1行表示
				}
				// 仕訳表示幅
				if( parastr.IsEmpty() ) {
					psel->DSPLNWD_OPT = OPT_WIDE;
				}
				else {
					parastr.MakeUpper();
					if( parastr == "WIDE" )
						psel->DSPLNWD_OPT = OPT_WIDE;
					else
						psel->DSPLNWD_OPT = OPT_NARROW;
				}
				break;	

			case DSPDENP_OPTN:
				if( psel->DSPDENP_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPDENP_OPT = OPT_NODSP;	// 伝票番号を表示しない
					}
					else {
						psel->DSPDENP_OPT = OPT_NONOPT;
					}
				}
				break;

			case DSPBMON_OPTN:
				if( psel->DSPBMON_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPBMON_OPT = OPT_NODSP;	// 部門を表示しない
					}
					else {
						psel->DSPBMON_OPT = OPT_NONOPT;
					}
				}
				break;

			case DSPKOJI_OPTN:
				if( psel->DSPKOJI_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPKOJI_OPT = OPT_NODSP;	// 工事を表示しない
					}
					else {
						psel->DSPKOJI_OPT = OPT_NONOPT;
					}
				}
				break;

			case DSPTGNO_OPTN:
				if( psel->DSPTGNO_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPTGNO_OPT = OPT_NODSP;	// 手形番号を表示しない
					}
					else {
						psel->DSPTGNO_OPT = OPT_NONOPT;
					}
				}
				break;

			case DSPTGDT_OPTN:
				if( psel->DSPTGDT_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPTGDT_OPT = OPT_NODSP;	// 手形期日を表示しない
					}
					else {
						psel->DSPTGDT_OPT = OPT_NONOPT;
					}
				}
				break;
			// 画面幅関係
			case RESOL_OPTN:
				psel->RESOL_OPT = param;	break;
			case WDSEQ_OPTN:
				psel->WDSEQ_OPT = param;	break;
			case WDDATE_OPTN:
				psel->WDDATE_OPT = param;	break;
			case WDDENP_OPTN:
				psel->WDDENP_OPT = param;	break;
			case WDBMON_OPTN:
				psel->WDBMON_OPT = param;	break;
			case WDKOJI_OPTN:
				psel->WDKOJI_OPT = param;	break;
			case WDDEBT_OPTN:
				psel->WDDEBT_OPT = param;	break;
			case WDCRED_OPTN:
				psel->WDCRED_OPT = param;	break;
			case WDVAL_OPTN:
				psel->WDVAL_OPT = param;	break;
			case WDZEI_OPTN:
				psel->WDZEI_OPT = param;	break;
			case WDSYZ_OPTN:
				psel->WDSYZ_OPT = param;	break;
			case WDTKY_OPTN:
				psel->WDTKY_OPT = param;	break;

			case KANADSP_OPTN:	// カナ順表示
				if( psel->KANADSP_OPT != -1 ) {
					if( param == 1 ) {
						psel->KANADSP_OPT = OPT_KANAJUN;	//カナ順で表示
					}
					else {
						psel->KANADSP_OPT = OPT_NONOPT;
					}
				}
				break;
			case KOJI_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x04) )
					psel->KOJI_OPT = -1;

				if( psel->KOJI_OPT != -1 ) {
					if( param == 1 )
							psel->KOJI_OPT = OPT_FIX;
					else	psel->KOJI_OPT = OPT_NONOPT;
				}
				break;
			case KJTOTKY_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x04) )
					psel->KJTOTKY_OPT = -1;

				if( psel->KJTOTKY_OPT != -1 ) {
					if( param == 1 )
							psel->KJTOTKY_OPT = OPT_KJTOTKY;
					else	psel->KJTOTKY_OPT = OPT_NONOPT;
				}
				break;
			// 検索一覧での 科目コード表示
			case SCNCD_OPTN:
				if( psel->SCNCD_OPT != -1 ) {
					if( param == 1 )
							psel->SCNCD_OPT = OPT_SCNCD;
					else	psel->SCNCD_OPT = OPT_NONOPT;
				}
				break;

			case DSPHJIYU_OPTN:
				if( psel->DSPHJIYU_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPHJIYU_OPT = OPT_NODSP;	// 変動事由を表示しない
					}
					else {
						psel->DSPHJIYU_OPT = OPT_NONOPT;
					}
				}
				break;
			case DSPZEI_OPTN:
				if( psel->DSPZEI_OPT != -1 ) {
					if( param == 1 ) {
						psel->DSPZEI_OPT = OPT_NODSP;	// 税額を表示しない
					}
					else {
						psel->DSPZEI_OPT = OPT_NONOPT;
					}
				}
				break;

			case ZEIMDFY_OPTN:
				if( psel->ZEIMDFY_OPT != -1 ) {
					if( param == 1 ) {
						psel->ZEIMDFY_OPT = OPT_ZEIMDFY;	// 税額を修正可能にする
					}
					else {
						psel->ZEIMDFY_OPT = OPT_NONOPT;
					}
				}
				break;

			case DELSWK_OPTN:
				if( psel->DELSWK_OPT != -1 ) {
					if( param == 1 ) {
						psel->DELSWK_OPT = OPT_DELSWK;	// 取消仕訳処理を行わない
					}
					else {
						psel->DELSWK_OPT = OPT_NONOPT;
					}
				}
				break;

			case INP2BK_OPTN:	// 部門･工事を借方・貸方とも入力
				psel->INP2BK_OPT = (param == 1) ? OPT_INP2BK : OPT_NONOPT;
				break;

			case INDSGN_OPTN:	// 個人科目情報
				psel->INDSGN_OPT = param;
				break;

			case NPJGY_OPTN:	// 公益事業費情報
				if( parastr.IsEmpty() ) {
					psel->NPJGY_OPT = NP_ALLINIKCD;
				}
				else{
					DWORD tmp;
					tmp = _atoh( parastr.GetBuffer() );
					psel->NPJGY_OPT = tmp;
				}
				break;

			case NPKNR_OPTN:	// 公益管理費情報
				if( parastr.IsEmpty() ) {
					psel->NPKNR_OPT = NP_ALLINIKCD;
				}
				else{
					DWORD tmp;
					tmp = _atoh( parastr.GetBuffer() );
					psel->NPKNR_OPT = tmp;
				}
				break;

			case SCNCLR_OPTN:
				if( psel->SCNCLR_OPT != -1 ) {
					if( param == 1 ) {
						psel->SCNCLR_OPT = OPT_SCNCLR;	// 検索項目クリア
					}
					else {
						psel->SCNCLR_OPT = OPT_NONOPT;
					}
				}
				break;

			case KJIINP_OPTN: 
				if( !(m_pZm->zvol->sub_sw&0x02) && !(m_pZm->zvol->sub_sw&0x04) )
					psel->KJIINP_OPT = -1;

				if( psel->KJIINP_OPT != -1 ) {
					if( param == 1 ) {
						psel->KJIINP_OPT = OPT_KJIINP;	// 部門／工事未入力チェック
					}
					else {
						psel->KJIINP_OPT = OPT_NONOPT;
					}
				}
				break;
			case KANKJI_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x04) )
					psel->KANKJI_OPT = -1;

				if( psel->KANKJI_OPT != -1 ) {
					if( param == 1 ) {
						psel->KANKJI_OPT = OPT_KANKJI;	// 完成工事チェック
					}
					else {
						psel->KANKJI_OPT = OPT_NONOPT;
					}
				}
				break;

			case KJIKMK_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x04) )
					psel->KJIKMK_OPT = -1;

				if( psel->KJIKMK_OPT != -1 ) {
					if( param == 1 ) {
						psel->KJIKMK_OPT = OPT_KJIKMK;	// 工事番号科目チェック
					}
					else {
						psel->KJIKMK_OPT = OPT_NONOPT;
					}
				}
				break;

			case BRNKAK_OPTN:
				if( !(m_pZm->zvol->br_sw & 0x01) )
					psel->BRNKAK_OPT = -1;	// 枝番なし

				if( psel->BRNKAK_OPT != -1 ) {
					if( param == 1 ) {
						psel->BRNKAK_OPT = OPT_BRNKAK;	// 枝番未入力時 確認する。
					}
					else {
						psel->BRNKAK_OPT = OPT_NONOPT;
					}
				}
				break;

#ifdef _SLIP_ACCOUNTS_
			case NOWSLIP_SEQ:
				if( !(m_pZm->zvol->sub_sw & 0x40) )
					psel->SLIP_SEQ = -1;	// 原票無し

				if( psel->SLIP_SEQ != -1 ) {
					psel->SLIP_SEQ = param;
				}
				break;
#endif
			// 画面ブロック移動
			case BLKDISP_OPTN:
				if( parastr.IsEmpty() ) {
					::ZeroMemory( psel->BLK_OPT, sizeof psel->BLK_OPT );
					psel->BLKSEL_OPT = OPT_NONOPT;
				}
				else {
					_get_blkpara( psel->BLK_OPT, sizeof psel->BLK_OPT, parastr);
					psel->BLKSEL_OPT = param;
				}
				break;
			// 仕訳対応摘要 表示サイン
			case SWKTKYD_OPTN:
				if( param == 1 ) {
					psel->SWTKYD_OPT = OPT_SWTKYD;
				}
				else {
					psel->SWTKYD_OPT = OPT_NONOPT;
				}
				break;

			// 証憑
			case DOCEVI_OPTN:
				if( ! (m_pZm->zvol->g_hsw & 0x04) )
					psel->DOCEVI_OPT = -1;

				if( psel->DOCEVI_OPT != -1 )
				{
					psel->DOCEVI_OPT = param;
				}
				break;

			// 部門摘要枝番
			case BMNTKBR_OPTN:
				if( !(m_pZm->zvol->br_sw & 0x01) || !(m_pZm->zvol->br_sw & 0x02) || !(m_pZm->zvol->sub_sw & 0x02) )
					psel->BMNTKBR_OPT = -1;

				if( psel->BMNTKBR_OPT != -1 )
				{
					if( param == 1 ) {
						psel->BMNTKBR_OPT = OPT_CHKON;
					}
					else {
						psel->BMNTKBR_OPT = OPT_NONOPT;
					}
				}
				break;

			// 枝番選択 50音順
			case BRNKANA_OPTN:
				if( !(m_pZm->zvol->br_sw & 0x01) )
					psel->BRNKANA_OPT = -1;	// 枝番なし
				else if( psel->BRNKANA_OPT != -1 )
					if( param == 1 /*!strcmp( txt, "OPT_DSP" )*/ )
					{
						// 枝番選択 50順
						psel->BRNKANA_OPT = OPT_KANAJUN;
					}
					else
					{
						// オプションなし
						psel->BRNKANA_OPT = OPT_NONOPT;
					}
				break;

			// 科目・仕訳対応摘要 50音順
			case RTSWKKANA_OPTN:
				if( psel->RTSWKKANA_OPT != -1 )
					if( param == 1 )
					{
						// 枝番選択 50順
						psel->RTSWKKANA_OPT = OPT_KANAJUN;
					}
					else
					{
						// オプションなし
						psel->RTSWKKANA_OPT = OPT_NONOPT;
					}
				break;

			case BMNTOTKY_OPTN:
				if( !(m_pZm->zvol->sub_sw&0x02) )
					psel->BMNTOTKY_OPT = -1;

				if( psel->BMNTOTKY_OPT != -1 ) {
					if( param == 1 )
							psel->BMNTOTKY_OPT = OPT_CHKON;
					else	psel->BMNTOTKY_OPT = OPT_NONOPT;
				}
				break;

			case DSPSYZ_OPTN:
				if (psel->DSPSYZ_OPT != -1) {
					if (param == 1) {
						psel->DSPSYZ_OPT = OPT_NODSP;	// 税額を表示しない
					}
					else {
						psel->DSPSYZ_OPT = OPT_NONOPT;
					}
				}
				break;

			case WDHJYU_OPTN:
				psel->WDHJYU_OPT = param;	break;

			// 出納帳 画面幅関係
			case AWSEQ_OPTN:
				psel->AWSEQ_OPT = param;	break;
			case AWDATE_OPTN:
				psel->AWDATE_OPT = param;	break;
			case AWDENP_OPTN:
				psel->AWDENP_OPT = param;	break;
			case AWBMON_OPTN:
				psel->AWBMON_OPT = param;	break;
			case AWKOJI_OPTN:
				psel->AWKOJI_OPT = param;	break;
			case AWKMK_OPTN:
				psel->AWKMK_OPT = param;	break;
			case AWNKN_OPTN:
				psel->AWNKN_OPT = param;	break;
			case AWSKN_OPTN:
				psel->AWSKN_OPT = param;	break;
			case AWZEI_OPTN:
				psel->AWZEI_OPT = param;	break;
			case AWSYZ_OPTN:
				psel->AWSYZ_OPT = param;	break;
			case AWTKY_OPTN:
				psel->AWTKY_OPT = param;	break;
			case AWHJYU_OPTN:
				psel->AWHJYU_OPT = param;	break;

			case WDSYON_OPTN:
				psel->WDSYON_OPT = param;	break;
			case AWSYON_OPTN:
				psel->AWSYON_OPT = param;	break;

			default:
				break;
			}
		}
	}
	// 
	if( ! psel->DCKM_CODE[0] )
		strcpy_s( psel->DCKM_CODE, sizeof psel->DCKM_CODE, GNKINCode(m_pZm->zvol->apno) );

	if( bOwnOpen ) {
		m_pZm->OwnTblClose();
	}
	if( bZvolOpen ) {
		m_pZm->VolumeClose();
	}

	return psel;
}



int CDinpINI::set_dinpsetup( char* key, int* par, CString* parstr )
{
//old function
ASSERT( FALSE );
	return -1;
//odl function

#ifdef OLD_CLOSE
	CSETUPTBL* sup;
	sup = m_pMsu->setup;
	int ret = -1;

	sup->MoveFirst();

	if( sup->st != -1 ) {
		do {
			if( sup->setupname == key ) {
				ret		= 0;
				sup->param = *par;
				sup->parastr = *parstr;
				sup->Correct();
			}
		} while( sup->MoveNext() == 0 );
	}

	if( ret ) {
		sup->setupname = key;
		sup->param = *par;
		sup->parastr = *parstr;
		sup->Append();
	}

	return ret;
#endif
}


//
// 入力パラメータのセット
//	0 ... ok
//	-1 .. Error
int CDinpINI::SetDinpSW( struct _AUTOSEL *psel, BOOL close )
{
	int key, param;
	char *pkey;
	CString parastr, filter;
	CString tmp, tblstr, tblwd_str, tbl2_str, tbl3_str, tbl4_str, tbl5_str, tbl6_str;
	BOOL bOwnOpen = FALSE;
	BOOL bZvolOpen = FALSE;

	if( m_pZm == NULL || psel == NULL /*|| m_pMsu == NULL*/ )
		return -1;

	filter.Format( "apl_name = '%s' AND itm_name = '%s' AND u_id = %d", INI_AppName, INI_ItmName, m_User_id );

	// COWNTB OpenされていなければOPEN
	if( m_pZm->owntb == NULL ) {
		if( m_pZm->OwnTblOpen( filter ) != 0 )
			return -1;
		bOwnOpen = TRUE;
	}
	else {
		m_pZm->owntb->Requery( filter );
	}

	if( m_pZm->owntb->st == -1 ) {
		// レコード追加
		m_pZm->owntb->AddNew();
		m_pZm->owntb->apl_name = INI_AppName;
		m_pZm->owntb->itm_name = INI_ItmName; 
		m_pZm->owntb->itm_seq = 1;

		ICSMCTL	mctrl;
		mctl_usertbl	utbl;

		if( mctrl.SetDatabase() == 0 ) {
			mctrl.get_usertbl( utbl );
			m_pZm->owntb->u_id = utbl.user_id;
		}

		m_pZm->owntb->Update();
		m_pZm->owntb->Requery( filter );
	}

	// 財務ボリューム Open
	if( m_pZm->zvol == NULL ) {
		m_pZm->VolumeOpen();
		bZvolOpen = TRUE;
	}
	//新規会社登録からの呼び出しで zvol->Edit(),zvol->Update()で例外
	// sql実行に変更
	if( psel->DENP_OPT != -1 ) {
		int in_opt = 0;
		switch( psel->DENP_OPT )
		{
		case -1:
			break;
		case OPT_AUTO:	// 自動加算
			in_opt = CNO_AUTO;
			break;
		case OPT_EQU:	// =SEQ
			in_opt = CNO_EQU;
			break;
		case OPT_NON:	// 伝票番号なし
			in_opt = CNO_NON;
			break;
		default:		// オプションなし
			in_opt = 0;
			break;
		}
		if( in_opt != m_pZm->zvol->in_opt ) {
			CString sql;
			sql.Format( "update zvolume set in_opt = %d", in_opt);
			m_pZm->m_database->ExecuteSQL(sql);
		}
	}

	tblstr.Empty();
	tblwd_str.Empty();
	tbl2_str.Empty();
	m_pZm->owntb->Edit();

	for( key = DATE_OPTN ; key < Key_Max ; ++key )
	{
		// キーNameGet
		if( pkey = get_keyname( key ) )
		{
			param = 0;
			parastr.Empty();

			switch( key )
			{
			case DATE_OPTN:
				switch( psel->DATE_OPT )
				{
				case -1:
					break;
				case OPT_FIX:	// 日付固定
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case DENP_OPTN:
				switch( psel->DENP_OPT )
				{
				case -1:
					break;
				case OPT_AUTO:	// 自動加算
					param = CNO_AUTO;
					break;
				case OPT_EQU:	// =SEQ
					param = CNO_EQU;
					break;
				case OPT_NON:	// 伝票番号なし
					param = CNO_NON;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;
			case BMON_OPTN:
				switch( psel->BMON_OPT )
				{
				case -1:
					break;
				case OPT_FIX:	// 部門番号固定
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case DEBT_OPTN:
				switch( psel->DEBT_OPT )
				{
				case -1:
					break;
				case OPT_FIX:	// 借方固定
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case CRED_OPTN:
				switch( psel->CRED_OPT )
				{
				case -1:
					break;
				case OPT_FIX:	// 貸方固定
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case BRN_OPTN:
				switch( psel->BRN_OPT )
				{
				case -1:
					break;
				case OPT_RGSCHK:	// 未入力チェック
					param = 1;
					break;
				default:			// オプションなし
					param = 0;
					break;
				}
				break;

			case BRTEK_OPTN:
				switch( psel->BRTEK_OPT )
				{
				case -1:
					break;
				case OPT_DSP:	// 枝番摘要表示
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case BRTOTKY_OPTN:
				switch( psel->BRTOTKY_OPT )
				{
				case -1:
					break;
				case OPT_EXEC:	// 枝番摘要実行
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case SYGBRN_OPTN:
				switch( psel->SYGBRN_OPT )
				{
				case -1:
					break;
				case OPT_DSP:	// 資金繰諸口枝番名称表示
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

#ifdef DB_CLOSE
			case SYIMPORT_OPTN:
				switch( psel->SYIMPORT_OPT )
				{
				case -1:
					break;
				case OPT_AUTO:	// 輸入仕訳自動生成
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;
#endif

			case HJIYU_OPTN:
				switch( psel->HJIYU_OPT )
				{
				case -1:
					break;
				case OPT_HJIYU:	// 変動事由 -> 仕訳摘要
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case BRCPY_OPTN:	// 08.02 /02
				switch( psel->BRCPY_OPT )
				{
				case -1:
					break;
				case OPT_BRCPY:	// 枝番をコピー する
					param = 1;
					break;
				default:		// オプションなし
					param = 0;
					break;
				}
				break;

			case BMNCHK_OPTN:	// 08.28 /02
				switch( psel->BMNCHK_OPT )
				{
				case -1:
					break;
				case OPT_BMNCHK:	// 部門チェックをする
					param = 1;
					break;
				default:			// オプションなし
					param = 0;
					break;
				}
				break;

			case SWKTEK_OPTN:	// 12.09 /02
				switch( psel->SWKTEK_OPT )
				{
				case -1:
					break;
				case OPT_SWKTEK:	// 仕訳対応摘要を使用
					param = 1;
					break;
				default:			// オプションなし
					param = 0;
					break;
				}
				break;

			case SYOBUZ_OPTN:	// 04.03 /03
				switch( psel->SYOBUZ_OPT )
				{
				case -1:
					break;
				case OPT_SYOBUZ:	// 諸口ブザー 鳴らす
					param = 1;
					break;
				default:			// 鳴らさない
					param = 0;
					break;
				}
				break;

			case BMNDSP_OPTN:	// 08.30 /04
				switch( psel->BMNDSP_OPT )
				{
				case -1:
					break;
				case OPT_BMNDSP:// 部門名称 表示する
					param = 1;
					break;
				default:		// しない
					param = 0;
					break;
				}
				break;

			case BMNSEL_OPTN:	// 08.30 /04
				switch( psel->BMNSEL_OPT )
				{
				case -1:
					break;
				case OPT_BMNSEL:	// 画面番号優先
					param = 1;
					break;
				default:			// 部門コード優先
					param = 0;
					break;
				}
				break;

#ifdef OLD_CLOSE
			case BMNBRN_OPTN:
				switch( psel->BMNBRN_OPT )
				{
				case -1:
					break;
				case OPT_BMNBRN:	// 部門枝番自動登録
					m_pZm->zvol->bm_isw |= 0x02;
					break;
				default:			// しない
					m_pZm->zvol->bm_isw &= ~0x02;
					break;
				}
				break;
#endif

			case RDDATE_OPTN:	// 08.30 /04
				switch( psel->RDDATE_OPT )
				{
				case -1:
					break;
				case OPT_RDDATE:	// 定型仕訳 日付を入力する
					param = 1;
					break;
				default:			// しない
					param = 0;
					break;
				}
				break;

			case RDBRN_OPTN:	// 08.30 /04
				switch( psel->RDBRN_OPT )
				{
				case -1:
					break;
				case OPT_RDBRN:	// 定型仕訳 枝番を入力する
					param = 1;
					break;
				default:		// しない
					param = 0;
					break;
				}
				break;
			/*----- ここからは文字列で -----*/
			case KMPG_OPTN:		// 02.20 /02
				switch( psel->KMPG_OPT ) {
				case -1:
					break;
				case OPT_KMPG:	// 科目ﾍﾟｰｼﾞ保持
					param = 1;
					break;
				default:		// 毎回先頭ﾍﾟｰｼﾞ
					param = 0;
					break;
				}
				break;

			case KMNO_OPTN:		// 02.20 /02
				switch( psel->KMNO_OPT ) {
				case -1:
					break;
				case OPT_KMNO:	// 科目コードを表示
					param = 1;
					break;				
				default:		// 通常の番号表示
					param = 0;
					break;
				}
				break;

			case DCKM_OPTN:		// 02.20 /02
				switch( psel->DCKM_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 現金貸借を表示しない
					param = 1;
					break;				
				default:		// 現金貸借を表示する
					param = 0;
					break;
				}
				if( psel->DCKM_OPT != -1 ) {
					parastr = psel->DCKM_CODE;
				}
				break;

			case DSPLINE_OPTN:
				if( psel->DSPLINE_OPT == 2 ) {
					param = 1;	// 仕訳２行表示
				}
				else {
					param = 0;	// 仕訳1行表示
				}
				// 仕訳表示幅
				if( psel->DSPLNWD_OPT == OPT_WIDE ) {
					parastr = _T( "WIDE" );
				}
				else {
					parastr = _T( "NARROW" );
				}
				break;	

			case DSPDENP_OPTN:
				switch( psel->DSPDENP_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 伝票番号を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			case DSPBMON_OPTN:
				switch( psel->DSPBMON_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 部門を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			case DSPKOJI_OPTN:
				switch( psel->DSPKOJI_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 工事を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			case DSPTGNO_OPTN:
				switch( psel->DSPTGNO_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 手形番号を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			case DSPTGDT_OPTN:
				switch( psel->DSPTGDT_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 手形期日を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			// 画面幅関係
			case RESOL_OPTN:
				param = psel->RESOL_OPT;	break;
			case WDSEQ_OPTN:
				param = psel->WDSEQ_OPT;	break;
			case WDDATE_OPTN:
				param = psel->WDDATE_OPT;	break;
			case WDDENP_OPTN:
				param = psel->WDDENP_OPT;	break;
			case WDBMON_OPTN:
				param = psel->WDBMON_OPT;	break;
			case WDKOJI_OPTN:
				param = psel->WDKOJI_OPT;	break;
			case WDDEBT_OPTN:
				param = psel->WDDEBT_OPT;	break;
			case WDCRED_OPTN:
				param = psel->WDCRED_OPT;	break;
			case WDVAL_OPTN:
				param = psel->WDVAL_OPT;	break;
			case WDZEI_OPTN:
				param = psel->WDZEI_OPT;	break;
			case WDSYZ_OPTN:
				param = psel->WDSYZ_OPT;	break;
			case WDTKY_OPTN:
				param = psel->WDTKY_OPT;	break;
			case KANADSP_OPTN:
				switch( psel->KANADSP_OPT ) {
				case -1:
					break;
				case OPT_KANAJUN:	// カナ順で表示する
					param = 1;
					break;
				default:
					param = 0;
					break;
				}
				break;

			case KOJI_OPTN:
				switch( psel->KOJI_OPT ) {
				case -1:	break;
				case OPT_FIX:		// 工事固定
					param = 1;	break;
				default:
					param = 0;	break;
				}
				break;

			case KJTOTKY_OPTN:
				switch( psel->KJTOTKY_OPT ) {
				case -1:	break;
				case OPT_KJTOTKY:		// 工事名称を仕訳摘要に
					param = 1;	break;
				default:
					param = 0;	break;
				}
				break;

			// 検索一覧での 科目コード表示
			case SCNCD_OPTN:
				switch( psel->SCNCD_OPT ) {
				case -1:	break;
				case OPT_SCNCD:		// 科目コード表示
					param = 1;	break;
				default:
					param = 0;	break;
				}
				break;
			// 変動事由の表示
			case DSPHJIYU_OPTN:
				switch( psel->DSPHJIYU_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 変動事由を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			// 税額の表示
			case DSPZEI_OPTN:
				switch( psel->DSPZEI_OPT ) {
				case -1:
					break;
				case OPT_NODSP:	// 税額を表示しない
					param = 1;
					break;				
				default:		// 表示する
					param = 0;
					break;
				}
				break;

			// 税額を修正可能に
			case ZEIMDFY_OPTN:
				switch( psel->ZEIMDFY_OPT ) {
				case -1:
					break;
				case OPT_ZEIMDFY:	// 税額を修正可能
					param = 1;
					break;				
				default:			// 修正不可[デフォルト]
					param = 0;
					break;
				}
				break;

			// 税額を修正可能に
			case DELSWK_OPTN:
				switch( psel->DELSWK_OPT ) {
				case -1:
					break;
				case OPT_DELSWK:	// 取消仕訳を処理しない
					param = 1;
					break;				
				default:			// 処理する
					param = 0;
					break;
				}
				break;

			case INP2BK_OPTN:	// 部門･工事を借方・貸方とも入力
				if( psel->INP2BK_OPT == OPT_INP2BK )
					param = 1;
				else
					param = 0;
				break;

			case INDSGN_OPTN:	// 個人科目選択
				param = psel->INDSGN_OPT;
				break;

			case NPJGY_OPTN:	// 公益事業費情報
				param = 1;
				parastr.Format( "%08x", psel->NPJGY_OPT );
				break;

			case NPKNR_OPTN:	// 公益管理費情報
				param = 1;
				parastr.Format( "%08x", psel->NPKNR_OPT );
				break;

			// 検索項目クリア
			case SCNCLR_OPTN:
				switch( psel->SCNCLR_OPT ) {
				case -1:
					break;
				case OPT_SCNCLR:	// 検索項目クリア
					param = 1;
					break;				
				default:			// しない
					param = 0;
					break;
				}
				break;

			// 工事未入力チェック
			case KJIINP_OPTN:
				switch( psel->KJIINP_OPT ) {
				case -1:
					break;
				case OPT_KJIINP:	// 工事未入力チェックする
					param = 1;
					break;				
				default:			// しない
					param = 0;
					break;
				}
				break;

			// 完成工事チェック
			case KANKJI_OPTN:
				switch( psel->KANKJI_OPT ) {
				case -1:
					break;
				case OPT_KANKJI:	// 完成工事チェックする
					param = 1;
					break;				
				default:			// しない
					param = 0;
					break;
				}
				break;

			// 工事番号科目チェック
			case KJIKMK_OPTN:
				switch( psel->KJIKMK_OPT ) {
				case -1:
					break;
				case OPT_KJIKMK:	// 工事番号科目チェックする
					param = 1;
					break;				
				default:			// しない
					param = 0;
					break;
				}
				break;

			// 枝番未入力時確認する
			case BRNKAK_OPTN:
				switch( psel->BRNKAK_OPT ) {
				case -1:
					break;
				case OPT_BRNKAK:	// 枝番未入力時確認する
					param = 1;
					break;				
				default:			// しない
					param = 0;
					break;
				}
				break;

#ifdef _SLIP_ACCOUNTS_
			// ドキュメントシーケンス
			case NOWSLIP_SEQ:
				param = psel->SLIP_SEQ;
				break;
#endif

			// 画面ブロック移動
			case BLKDISP_OPTN:
				if( psel->BLKSEL_OPT == OPT_NONOPT ) {
					param = 0;
				}
				else {
					param = 1;
				}
			{
				for( int n = 0; n < (sizeof psel->BLK_OPT); n++ ) { 
					if( psel->BLK_OPT[n] > 0 ) {
						if( n > 0 ) {
							parastr += "_";
						}
						tmp.Format( "%d", psel->BLK_OPT[n] );
						parastr += tmp;
					}
					else {
						break;
					}
				}
			}
				break;

			// 仕訳対応摘要 表示
			case SWKTKYD_OPTN:
				switch( psel->SWTKYD_OPT ) {
				case -1:	break;
				case OPT_SWTKYD:		// 登録通り 単独として使用しない
					param = 1;	break;
				default:
					param = 0;	break;
				}
				break;

			// 証憑
			case DOCEVI_OPTN:
				switch( psel->DOCEVI_OPT ) {
				case -1:
					break;
				default:
					param = psel->DOCEVI_OPT;
					break;
				}

				break;

			// 部門摘要枝番
			case BMNTKBR_OPTN:
				switch( psel->BMNTKBR_OPT ) {
				case -1:
					break;
				default:
					if( psel->BMNTKBR_OPT == OPT_CHKON )
						param = 1;
					else
						param = 0;
					break;
				}
				break;

			// 部門摘要枝番
			case BRNKANA_OPTN:
				switch( psel->BRNKANA_OPT ) {
				case -1:
					break;
				default:
					if( psel->BRNKANA_OPT == OPT_KANAJUN )
						param = 1;
					else
						param = 0;
					break;
				}
				break;

			// 科目・仕訳対応摘要 50音順
			case RTSWKKANA_OPTN:
				switch( psel->RTSWKKANA_OPT ) {
				case -1:
					break;
				default:
					if( psel->RTSWKKANA_OPT == OPT_KANAJUN )
						param = 1;
					else
						param = 0;
					break;
				}
				break;

			case BMNTOTKY_OPTN:
				switch( psel->BMNTOTKY_OPT ) {
				case -1:	break;
				case OPT_CHKON:		// 部門名称を仕訳摘要に
					param = 1;	break;
				default:
					param = 0;	break;
				}
				break;
				// 消費税欄の表示
			case DSPSYZ_OPTN:
				switch (psel->DSPSYZ_OPT) {
				case -1:		break;
				case OPT_NODSP:	// 表示しない
					param = 1;	break;
				default:		// 表示する
					param = 0;	break;
				}
				break;
			case WDHJYU_OPTN:
				param = psel->WDHJYU_OPT;	break;

			case AWSEQ_OPTN:
				param = psel->AWSEQ_OPT;	break;
			case AWDATE_OPTN:
				param = psel->AWDATE_OPT;	break;
			case AWDENP_OPTN:
				param = psel->AWDENP_OPT;	break;
			case AWBMON_OPTN:
				param = psel->AWBMON_OPT;	break;
			case AWKOJI_OPTN:
				param = psel->AWKOJI_OPT;	break;
			case AWKMK_OPTN:
				param = psel->AWKMK_OPT;	break;
			case AWNKN_OPTN:
				param = psel->AWNKN_OPT;	break;
			case AWSKN_OPTN:
				param = psel->AWSKN_OPT;	break;
			case AWZEI_OPTN:
				param = psel->AWZEI_OPT;	break;
			case AWSYZ_OPTN:
				param = psel->AWSYZ_OPT;	break;
			case AWTKY_OPTN:
				param = psel->AWTKY_OPT;	break;
			case AWHJYU_OPTN:
				param = psel->AWHJYU_OPT;	break;

			case WDSYON_OPTN:
				param = psel->WDSYON_OPT;	break;
			case AWSYON_OPTN:
				param = psel->AWSYON_OPT;	break;
			default:
				break;
			}

#ifdef _20081201_NP_EXTEND_
			if( key <= RDBRN_OPTN ) {
				m_pZm->owntb->vl[key] = param;
			}
			else if( key <= DSPTGDT_OPTN ) {
				if( parastr.IsEmpty() )
					tmp.Format( "%s=%d", pkey, param );
				else {
					tmp.Format( "%s=%d+%s", pkey, param, parastr );
				}

				if( ! tblstr.IsEmpty() )	tblstr += ",";
				tblstr += tmp;
			}
			else if( key <= WDTKY_OPTN ) {
				tmp.Format( "%s=%d", pkey, param );
				if( ! tblwd_str.IsEmpty() )	tblwd_str += ",";
				tblwd_str += tmp;
			}
			else {
			//	if( key != BMNBRN_OPTN ) {
					tmp.Format( "%s=%d", pkey, param );
					if( ! tbl2_str.IsEmpty() )	tbl2_str += ",";
					tbl2_str += tmp;
			//	}
			}
#endif
			if( key <= RDBRN_OPTN ) {
				m_pZm->owntb->vl[key] = param;
			}
			else if( key <= DSPTGDT_OPTN ) {
				if( parastr.IsEmpty() )
					tmp.Format( "%s=%d", pkey, param );
				else {
					tmp.Format( "%s=%d+%s", pkey, param, parastr );
				}

				if( ! tblstr.IsEmpty() )	tblstr += ",";
				tblstr += tmp;
			}
			else if( key <= WDTKY_OPTN ) {
				tmp.Format( "%s=%d", pkey, param );
				if( ! tblwd_str.IsEmpty() )	tblwd_str += ",";
				tblwd_str += tmp;
			}
			else if( key <= INDSGN_OPTN ) {
				tmp.Format( "%s=%d", pkey, param );
				if( ! tbl2_str.IsEmpty() )	tbl2_str += ",";
				tbl2_str += tmp;
			}
			else if( key <= SWKTKYD_OPTN ) { 
				if( key == NPJGY_OPTN || key == NPKNR_OPTN || key == BLKDISP_OPTN ) {
					tmp.Format( "%s=%d+%s", pkey, param, parastr );
				}
				else {
					tmp.Format( "%s=%d", pkey, param );
				}
				if( ! tbl3_str.IsEmpty() )	tbl3_str += ",";
				tbl3_str += tmp;
			}
			else if( key <= AWSEQ_OPTN ) {
				tmp.Format( "%s=%d", pkey, param );
				if( ! tbl4_str.IsEmpty() )	tbl4_str += ",";
				tbl4_str += tmp;
			}
			else if( key <= AWHJYU_OPTN ) {
				tmp.Format("%s=%d", pkey, param);
				if( !tbl5_str.IsEmpty() )	tbl5_str += ",";
				tbl5_str += tmp;
			}
			else {
				tmp.Format("%s=%d", pkey, param);
				if( !tbl6_str.IsEmpty() )	tbl6_str += ",";
				tbl6_str += tmp;
			}
		}
	}

	if( ! tblstr.IsEmpty() ) {
		m_pZm->owntb->str[0] = tblstr;
	}
	if( ! tblwd_str.IsEmpty() ) {
		m_pZm->owntb->str[1] = tblwd_str;
	}
	if( ! tbl2_str.IsEmpty() ) {
		m_pZm->owntb->str[2] = tbl2_str;
	}
	if( ! tbl3_str.IsEmpty() ) {
		m_pZm->owntb->str[3] = tbl3_str;
	}
	if( ! tbl4_str.IsEmpty() ) {
		m_pZm->owntb->str[4] = tbl4_str;
	}
	if( !tbl5_str.IsEmpty() ) {
		m_pZm->owntb->str[5] = tbl5_str;
	}
	if( !tbl6_str.IsEmpty() ) {
		m_pZm->owntb->str[6] = tbl6_str;
	}

//AfxMessageBox(" owntb->Update 00" );

	m_pZm->owntb->Update();

	if( bOwnOpen ) {
		m_pZm->OwnTblClose();
	}
	if( bZvolOpen ) {
		m_pZm->VolumeClose();
	}

//AfxMessageBox(" owntb->Update 01" );

//		if( close )
//		inz();	// ICSParam class delete
	return 0;
}

//
// 入力モードＳＷゲット
struct _AUTOSEL *CDinpINI::get_inpswtbl()
{
	struct _AUTOSEL *psel;

	if( m_pZm == NULL /*|| m_pMsu == NULL*/ )
		return NULL;
//	if( !m_pParam )
//	{
//		if( !(psel = GetDinpSW( m_pZm )) )
//			return NULL;
//	}
	else
		psel = &m_AUTOSEL;

	return psel;
}
	
//
// 輸入仕訳自動生成のみチェック
BOOL CDinpINI::isMkSyImpDta()
{
	struct _AUTOSEL *psel;

	if( psel = get_inpswtbl() )
	{
//		if( psel->SYIMPORT_OPT == OPT_AUTO )
//			return TRUE;
	}
	return FALSE;
}

//
// 枝番名称表示サインゲット
//
int CDinpINI::isBrTekDisp()
{
	struct _AUTOSEL *psel;
	BOOL sw1,sw2;

	if( psel = get_inpswtbl() )
	{
		sw1 = (psel->BRTEK_OPT == OPT_DSP);
		sw2 = (psel->SYGBRN_OPT == OPT_DSP);

		if( sw1 && !sw2 )
			return 0x01;	// 枝番
		else if( !sw1 && sw2 )
			return 0x02;	// 諸口枝番
		else if( sw1 && sw2 )
			return 0x03;	// 枝番＋諸口枝番
	}
	return 0;
}


// 部門名称表示 08.30 /04
int CDinpINI::IsBumonDisp()
{
	struct _AUTOSEL *psel;
	BOOL sw1 = FALSE;

	if( psel = get_inpswtbl() )
	{
		sw1 = (psel->BMNDSP_OPT == OPT_BMNDSP);
	}

	return sw1;
}
