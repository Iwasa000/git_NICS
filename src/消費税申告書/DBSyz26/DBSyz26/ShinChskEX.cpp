// ShinChskEX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinChskEX.h"

#include "ShinChskEXIdx.h"
#include "H26HyoView.h"

// CShinChskEX ダイアログ
// バックカラーコントロール

#define	SIROIROCOLOR	PALETTERGB( 255, 251, 240 )

CBrush	Ibrsh_ChskEX( SIROIROCOLOR );


//=============================================================================
// 期間チェック関数
//=============================================================================
static	int  	Date_KymdCheck(BYTE* sp, BYTE* ep);
static	int  	Date_YMDcomp(BYTE* ymd1, BYTE* ymd2);
static	void  	Date_GetSymd(BYTE* sp, BYTE* ep);
static	void  	Date_GetEymd(BYTE* ep, BYTE* sp);
static	void  	Date_SetFebEday(BYTE* ymd);
static	int  	Date_DateCheck(BYTE* date);
static	int  	Date_DtrmCheck(BYTE* smd, BYTE* emd, BYTE* dat);
static	int  	Date_SetNextDate(BYTE* ndp, BYTE* sdp);
static	int  	Date_SetPrevDate(BYTE* pdp, BYTE* sdp);
static	BYTE  	Date_GetEndDay(BYTE mth);
static	int  	Date_IsLeap(BYTE year);
static	BYTE  	Date_CalcBCD(BYTE val, BYTE dat, BYTE sg);
static	int		val_cmp(BYTE* val1, BYTE* val2, int len);




IMPLEMENT_DYNAMIC(CShinChskEX, CSyzBaseDlg)

CShinChskEX::CShinChskEX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinChskEX::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pSnChskData(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_Crsw(0)
	, m_Index(0)
	, m_pParent(pParent)	// '15.12.07
{
	m_curwnd = -1;
//2016.06.22 INSERT START
	m_bunshi_sw = 0;
//2016.06.22 INSERT END
}

CShinChskEX::~CShinChskEX()
{
}

void CShinChskEX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_ChskDiag);
	DDX_Control(pDX, IDC_EDIT_MYNUM1, m_edit_mynum1);
	DDX_Control(pDX, IDC_EDIT_MYNUM2, m_edit_mynum2);
	DDX_Control(pDX, IDC_EDIT_MYNUM3, m_edit_mynum3);
	DDX_Control(pDX, IDC_EDIT_MYNUM4, m_edit_mynum4);
}


BEGIN_MESSAGE_MAP(CShinChskEX, CSyzBaseDlg)
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShinChskEX メッセージ ハンドラ

//-----------------------------------------------------------------------------
// フリガナのコピー
//-----------------------------------------------------------------------------
void CopyFuriganaEX(UCHAR* buf,CString edit)
{
	char Str[128] ={};
	edit.Replace("\r\n","");
	int Length = edit.GetLength();
	memset( Str, '\0', sizeof( Str ) );
	memmove( Str, edit, Length );
	memmove( buf, Str, 34*2 );
}

//2016.10.20 INSERT START
void CopyFuriganaEX2(UCHAR* buf,CString edit)
{
	char Str[128] ={};
	edit.Replace("\r\n","");
	int Length = edit.GetLength();
	memset( Str, '\0', sizeof( Str ) );
	memmove( Str, edit, Length );
	memmove( buf, Str, 40*2 );
}
//2016.10.20 INSERT END

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData		：	本表ヘッダデータ
//		pSnChskData		：	中間予定申告データ
//		pSnHonpyoData	：	本表データ
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinChskEX::InitInfo( CSnHeadData *pSnHeadData, CH26SnChskData *pSnChskData, CH26SnHonpyoData *pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSnChskData );
	if( pSnChskData == NULL ){
		return -1;
	}
	m_pSnChskData = pSnChskData;

	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
int CShinChskEX::Init()
{
	InitDisp();
	if( !(m_pSnHeadData->Sn_Sign4&0x01) ){
		Tuki_Set();
	}
	if( !(m_pSnHeadData->Sn_Sign4&0x80) || (m_pSnHonpyoData->Sn_ZNOsw==1) ){
		m_Crsw = 1;
	}
	if( m_pSnHeadData->Sn_Sign4&0x01 ){//入力ロック中はしない
		m_Crsw = 0;
	}
	SetDiagData();

	InitDiagAttr();

	diag_setposition( IDC_ICSDIAGCTRL1, 0, CIcsdiagctrl );

	m_pSnHonpyoData->Sn_ZNOsw = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinChskEX::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( TyuukanEX[cnt].Index == -1 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( m_pSnHeadData->Sn_Sign4&0x01 ){
			Flg = TyuukanEX[cnt].AllUnConnect;
		}
		else{
			if( m_pSnHeadData->Sn_Sign4&0x80 ){
				Flg = TyuukanEX[cnt].UnConnect;
			}
			else{
				Flg = TyuukanEX[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, TyuukanEX[cnt].Index, Flg, m_pSnHeadData->Sn_Sign4, TyuukanEX[cnt].Connect );

		if( (TyuukanEX[cnt].Index==CHUMEIFRI) || (TyuukanEX[cnt].Index==CHUDAIFRI) ){//フリガナの部分
			m_ChskDiag.ModifyItem( TyuukanEX[cnt].Index, ( Flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY ) | DIAG_MODIFY_IMEKANA );
		}

		cnt++;
	}

//2015.12.08 INSERT START
//	// 個人番号又は法人番号
//	if( ((CH26HyoView*)m_pParent)->GetShRevType() >= ID_ICSSH_MYNUMBER_REVTYPE ){
//
//		int kubun;
//		HWND edit[10] = {NULL};
//		int diag_index[2][10] = {{-1},-1};
//		int nKojinDispModify	= 0;		// 表示属性
//		int nHojinDispModify	= 0;		// 表示属性
//		
//		if(m_pSnHeadData->IsSoleProprietor() == TRUE ){ //個人
//			kubun = MN_KOJIN;
//
//			edit[0] = m_edit_mynum1.GetSafeHwnd();
//			edit[1] = m_edit_mynum2.GetSafeHwnd();
//			edit[2] = m_edit_mynum3.GetSafeHwnd();
//			m_edit_mynum4.ShowWindow(SW_HIDE);
//
//			diag_index[0][0] = CHSKEX_NUM_KOJIN1;
//			diag_index[0][1] = CHSKEX_NUM_KOJIN2;
//			diag_index[0][2] = CHSKEX_NUM_KOJIN3;
//			diag_index[0][3] = -1;
//
//			nKojinDispModify = DIAG_MDFY_OPAQUE|DIAG_MDFY_READONLY;
//			nHojinDispModify = DIAG_MDFY_TRANSPARENT;
//
//		}else{ //法人
//			kubun = MN_HOJIN;
//
//			edit[0] = m_edit_mynum1.GetSafeHwnd();
//			edit[1] = m_edit_mynum2.GetSafeHwnd();
//			edit[2] = m_edit_mynum3.GetSafeHwnd();
//			edit[3] = m_edit_mynum4.GetSafeHwnd();
//
//			diag_index[0][0] = CHSKEX_NUM_HOJIN1;		
//			diag_index[0][1] = CHSKEX_NUM_HOJIN2;
//			diag_index[0][2] = CHSKEX_NUM_HOJIN3;
//			diag_index[0][3] = CHSKEX_NUM_HOJIN4;
//			diag_index[0][4] = -1;
//
//			nKojinDispModify = DIAG_MDFY_TRANSPARENT;
//			nHojinDispModify = DIAG_MDFY_OPAQUE|DIAG_MDFY_READONLY;
//		}
//
//		//法人番号
//		int idx_cnt = 0;
//		for( idx_cnt = CHSKEX_NUM_HOJIN1; idx_cnt <= CHSKEX_NUM_HOJINHYPHEN3; idx_cnt++ ){
//			m_ChskDiag.ModifyItem( idx_cnt, nHojinDispModify );
//		}
//		//個人番号
//		for( idx_cnt = CHSKEX_NUM_KOJIN1; idx_cnt <= CHSKEX_NUM_KOJINHYPHEN2; idx_cnt++ ){
//			m_ChskDiag.ModifyItem( idx_cnt, nKojinDispModify );
//		}
//
//		m_ctrl_mynum.Init( this, kubun , m_pSnHeadData->m_apno, TRUE, edit, m_ChskDiag.GetSafeHwnd(), diag_index, MN_NO_PICKUP_DIAGDATA );
//
//		if(m_pSnHeadData->IsSoleProprietor() == TRUE ){ //個人
//			m_ctrl_mynum.MoveDiagMNEdit(CHSKEX_NUM_KOJIN1);
//		}else{
//			m_ctrl_mynum.MoveDiagMNEdit(CHSKEX_NUM_HOJIN1);
//		}
//
////		m_ctrl_mynum.SetHeightMargin(MN_HEIGHT_BOTTOM);
//		m_ctrl_mynum.SetHeightMargin(MN_HEIGHT_CENTER);
//		m_ctrl_mynum.EnableWindow(TRUE);
//		m_ctrl_mynum.ShowWindow(SW_SHOW);
//	}
//2015.12.08 INSERT END

//	m_ChskDiag.EnableDelete(0);//DELETEキー１回で削除モード設定//[Y]

	return 0;
}

//-----------------------------------------------------------------------------
// 初期表示
//-----------------------------------------------------------------------------
int CShinChskEX::InitDisp()
{
	int			i, x;
	DIAGRAM_DATA	dgda;
	char		buf[256];
	int			len;
	CString		str;

	// ANK交じり対応
	DIAGRAM_ATTRIBUTE	DA;
	// 納税地　住所(上)
	m_ChskDiag.GetAttribute( CHUNOU1, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_ChskDiag.SetAttribute( CHUNOU1, (LPUNKNOWN)&DA, TRUE );
	// 納税地　住所(下)
	m_ChskDiag.GetAttribute( CHUNOU2, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_ChskDiag.SetAttribute( CHUNOU2, (LPUNKNOWN)&DA, TRUE );
	// 名称又は屋号
	m_ChskDiag.GetAttribute( CHUMEI, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_ChskDiag.SetAttribute( CHUMEI, (LPUNKNOWN)&DA, TRUE );
	// 代表者氏名又は氏名
	m_ChskDiag.GetAttribute( CHUDAI, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_ChskDiag.SetAttribute( CHUDAI, (LPUNKNOWN)&DA, TRUE );

	diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );

	//初期化
	diag_clear( IDC_ICSDIAGCTRL1, CHUNOU1, TRUE, CIcsdiagctrl );// 納税地　上段
	diag_clear( IDC_ICSDIAGCTRL1, CHUNOU2, TRUE, CIcsdiagctrl );// 納税地　下段
	diag_clear( IDC_ICSDIAGCTRL1, CHUTEL1, TRUE, CIcsdiagctrl );// 電話番号
	diag_clear( IDC_ICSDIAGCTRL1, CHUTEL2, TRUE, CIcsdiagctrl );// 電話番号
	diag_clear( IDC_ICSDIAGCTRL1, CHUTEL3, TRUE, CIcsdiagctrl );// 電話番号
	diag_clear( IDC_ICSDIAGCTRL1, CHUMEIFRI, TRUE, CIcsdiagctrl );// フリガナ（名称部分）
	diag_clear( IDC_ICSDIAGCTRL1, CHUMEI, TRUE, CIcsdiagctrl );// 名称又は屋号
	diag_clear( IDC_ICSDIAGCTRL1, CHUDAIFRI, TRUE, CIcsdiagctrl );// フリガナ（代表者氏名部分）
	diag_clear( IDC_ICSDIAGCTRL1, CHUDAI, TRUE, CIcsdiagctrl );// 代表者氏名又は氏名
	
	
	//税務署部分の初期化
	diag_clear( IDC_ICSDIAGCTRL1, CHUZEINEN, TRUE, CIcsdiagctrl );// 平成○○
	diag_clear( IDC_ICSDIAGCTRL1, CHUZEISYO, TRUE, CIcsdiagctrl );// ○○税務署長殿

	//初期化
	diag_clear( IDC_ICSDIAGCTRL1, CHUCHUKJI, TRUE, CIcsdiagctrl );// ○年○月○日　課税期間分の中間申告書（自）
	diag_clear( IDC_ICSDIAGCTRL1, CHUCHUKITA, TRUE, CIcsdiagctrl );// ○年○月○日　課税期間分の中間申告書（至）
	diag_clear( IDC_ICSDIAGCTRL1, CHUTAIJI, TRUE, CIcsdiagctrl );// 中間申告対象期間 自
	diag_clear( IDC_ICSDIAGCTRL1, CHUTAIITA, TRUE, CIcsdiagctrl );// 中間申告対象期間 至


	diag_clear( IDC_ICSDIAGCTRL1, CHUZRJMEI, TRUE, CIcsdiagctrl );	//税理士署名 (事務所名)
	diag_clear( IDC_ICSDIAGCTRL1, CHUZRMEI, TRUE, CIcsdiagctrl );	//税理士署名(税理士氏名)
	diag_clear( IDC_ICSDIAGCTRL1, CHUZRTEL, TRUE, CIcsdiagctrl ); //税理士署名(税理士 TEL)
	diag_clear( IDC_ICSDIAGCTRL1, CHUSYONO, TRUE, CIcsdiagctrl ); //税務署検索の下 (書番号)

	// 納税地
	x = CHUNOU1;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_NOZEI, 40 );
	memmove( buf, &m_pSnHeadData->Sn_NOZEI[0], len );
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	x = CHUNOU2;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf) );
	len = m_Util.MixSpaceCutLength( (char *)&m_pSnHeadData->Sn_NOZEI[40], 40 );
	memmove( buf, &m_pSnHeadData->Sn_NOZEI[40], len );
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	char bf1[8], bf2[8], bf3[8];
	memset( bf1, '\0', sizeof( bf1 ));
	memset( bf2, '\0', sizeof( bf2 ));
	memset( bf3, '\0', sizeof( bf3 ));

	// TEL
	// 電話番号　市外
	x = CHUTEL1;
	m_Util.DiagOcxIniz( &dgda );
	memset( bf1, '\0', sizeof( bf1 ));
	len = kjlen( m_pSnHeadData->Sn_TELNO1, 6 );
	memmove( bf1, m_pSnHeadData->Sn_TELNO1, len );
	dgda.data_edit = bf1;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	// 電話番号　市内
	x = CHUTEL2;
	m_Util.DiagOcxIniz( &dgda );
	memset( bf2, '\0', sizeof( bf2 ));
	len = kjlen( m_pSnHeadData->Sn_TELNO2, 4 );
	memmove( bf2, m_pSnHeadData->Sn_TELNO2, len );
	dgda.data_edit = bf2;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	// 電話番号　己心
	x = CHUTEL3;
	m_Util.DiagOcxIniz( &dgda );
	memset( bf3, '\0', sizeof( bf3 ));
	len = kjlen( m_pSnHeadData->Sn_TELNO3, 4 );
	memmove( bf3, m_pSnHeadData->Sn_TELNO3, len );
	dgda.data_edit = bf3;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 名称又は屋号 フリガナ
	x = CHUMEIFRI;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	len = kjlen( m_pSnHeadData->Sn_COFRI, sizeof( m_pSnHeadData->Sn_COFRI ));
	{
		char buf1[64] ={};
		char buf2[64] ={};
//2016.10.20 UPDATE START
		//memmove(buf1,m_pSnHeadData->Sn_COFRI,34);
		//memmove(buf2,&m_pSnHeadData->Sn_COFRI[34],34);
		memmove(buf1,m_pSnHeadData->Sn_COFRI,40);
		memmove(buf2,&m_pSnHeadData->Sn_COFRI[40],40);
//2016.10.20 UPDATE END
		sprintf_s(buf,sizeof(buf),"%s\r\n%s",buf1,buf2);
	}
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	// 名称又は屋号
	x = CHUMEI;
	memset( buf, '\0', sizeof( buf ));
	memmove( buf, m_pSnHeadData->Sn_CONAM, 40 );
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

//-- '15.12.07 --
//	//---->個人番号又は法人番号
////2015.10.20 INSERT START
//	x = CHUKNO;
//	memset( buf, '\0', sizeof( buf ));
//	str.Empty();
//	if( m_pSnHeadData->GetMyNumber_hyphen(str) == 0 ){
//		memmove( buf, str, str.GetLength());
//	}
//	dgda.data_disp = buf;
//	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
////2015.10.20 INSERT END
//	//<-------------------------------
//---------------
	// 個人番号又は法人番号
//2015.12.08 UPDATE START
//	if( ((CH26HyoView*)m_pParent)->GetShRevType() >= ID_ICSSH_MYNUMBER_REVTYPE ){
		//x = CHUKNO;
		//memset( buf, '\0', sizeof( buf ));
		//str.Empty();
		//if( m_pSnHeadData->GetMyNumber_hyphen(str) == 0 ){
		//	memmove( buf, str, str.GetLength());
		//}
		//dgda.data_disp = buf;
		//diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
//	}

	if( ((CH26HyoView*)m_pParent)->GetShRevType() >= ID_ICSSH_MYNUMBER_REVTYPE ){

		int kubun;
		HWND edit[10] = {NULL};
		int diag_index[2][10] = {{-1},-1};
		int nKojinDispModify	= 0;		// 表示属性
		int nHojinDispModify	= 0;		// 表示属性
		
		if(m_pSnHeadData->IsSoleProprietor() == TRUE ){ //個人
			kubun = MN_KOJIN;

			edit[0] = m_edit_mynum1.GetSafeHwnd();
			edit[1] = m_edit_mynum2.GetSafeHwnd();
			edit[2] = m_edit_mynum3.GetSafeHwnd();
			m_edit_mynum4.ShowWindow(SW_HIDE);

			diag_index[0][0] = CHSKEX_NUM_KOJIN1;
			diag_index[0][1] = CHSKEX_NUM_KOJIN2;
			diag_index[0][2] = CHSKEX_NUM_KOJIN3;
			diag_index[0][3] = -1;

			nKojinDispModify = DIAG_MDFY_OPAQUE|DIAG_MDFY_READONLY;
			nHojinDispModify = DIAG_MDFY_TRANSPARENT;

		}else{ //法人
			kubun = MN_HOJIN;

			edit[0] = m_edit_mynum1.GetSafeHwnd();
			edit[1] = m_edit_mynum2.GetSafeHwnd();
			edit[2] = m_edit_mynum3.GetSafeHwnd();
			edit[3] = m_edit_mynum4.GetSafeHwnd();

			diag_index[0][0] = CHSKEX_NUM_HOJIN1;		
			diag_index[0][1] = CHSKEX_NUM_HOJIN2;
			diag_index[0][2] = CHSKEX_NUM_HOJIN3;
			diag_index[0][3] = CHSKEX_NUM_HOJIN4;
			diag_index[0][4] = -1;

			nKojinDispModify = DIAG_MDFY_TRANSPARENT;
			nHojinDispModify = DIAG_MDFY_OPAQUE|DIAG_MDFY_READONLY;
		}

		//法人番号
		int idx_cnt = 0;
		for( idx_cnt = CHSKEX_NUM_HOJIN1; idx_cnt <= CHSKEX_NUM_HOJINHYPHEN3; idx_cnt++ ){
			m_ChskDiag.ModifyItem( idx_cnt, nHojinDispModify );
		}
		//個人番号
		for( idx_cnt = CHSKEX_NUM_KOJIN1; idx_cnt <= CHSKEX_NUM_KOJINHYPHEN2; idx_cnt++ ){
			m_ChskDiag.ModifyItem( idx_cnt, nKojinDispModify );
		}

		m_ctrl_mynum.Init( this, kubun , m_pSnHeadData->m_apno, TRUE, edit, m_ChskDiag.GetSafeHwnd(), diag_index, MN_NO_PICKUP_DIAGDATA );

		int ret;
		if( ((CH26HyoView*)m_pParent)->GetShRevType() >= ID_ICSSH_MYNUMBER_REVTYPE ){
			CString str_myno;

			m_pSnHeadData->GetMyNumber(str_myno);

			ret = m_ctrl_mynum.SetData(str_myno);
		}

		MoveDiagMNEdit();

//		m_ctrl_mynum.SetHeightMargin(MN_HEIGHT_BOTTOM);
		m_ctrl_mynum.SetHeightMargin(MN_HEIGHT_CENTER);
		m_ctrl_mynum.EnableWindow(FALSE);
		m_ctrl_mynum.ShowWindow(SW_SHOW);
	}
//2015.12.08 UPDATE END
//---------------

	// 代表者氏名 フリガナ
	x = CHUDAIFRI;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	len = kjlen( m_pSnHeadData->Sn_DIFRI, sizeof( m_pSnHeadData->Sn_DIFRI ));
	{
		char buf1[64] ={};
		char buf2[64] ={};
		memmove(buf1,m_pSnHeadData->Sn_DIFRI,34);
		memmove(buf2,&m_pSnHeadData->Sn_DIFRI[34],34);
		sprintf_s(buf,sizeof(buf),"%s\r\n%s",buf1,buf2);
	}
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	// 代表者氏名
	x = CHUDAI;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	memmove( buf, m_pSnHeadData->Sn_DIHYO, 40 );
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	if( m_pShinInfo->isExistOffice ){
		// 事務所名
		x = CHUZRJMEI;
		m_Util.DiagOcxIniz( &dgda );
		memset( buf, '\0', sizeof( buf ));
		memmove( buf, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam) );
		dgda.data_disp = buf;
		diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
		// 税理士氏名
		x = CHUZRMEI;
		m_Util.DiagOcxIniz( &dgda );
		memset( buf, '\0', sizeof( buf ));
		memmove( buf, m_pShinInfo->pZ_DATA->z_zeinam, sizeof( m_pShinInfo->pZ_DATA->z_zeinam ));
		dgda.data_disp = buf;
		diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
		// 税理士 TEL
		x = CHUZRTEL;
		m_Util.DiagOcxIniz( &dgda );
		memset( buf, '\0', sizeof( buf ));
		m_Util.CnvTelno( buf, m_pShinInfo->pZ_DATA->ji_denwa, 16, 30 );
		dgda.data_disp = buf;
		diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	}

	bool isReiwaPrnt = false;
	if( m_Util.GetPrePrintGengo(m_pSnHeadData, false) == 2 ){
		isReiwaPrnt = true;
	}

	// 提出年月日 平成○○
	int		gengo=0;
	char	yymmdd[3]={0};
	x = CHUZEINEN;
	m_Util.DiagOcxIniz( &dgda );
//-- '19.04.02 --
//	m_pSnHeadData->GetYmdData( ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
//---------------
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
//---------------
	if( yymmdd[0] && yymmdd[1] ){
		memmove(dgda.data_day, yymmdd, 3);
	}
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt ){
		sprintf_s(buf, sizeof(buf), _T("%s"), SYZ26_REIWA_G_STR);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHTEISYUTU, &dgda, CIcsdiagctrl);

	// 税務署名 ○○税務署長殿
	x = CHUZEISYO;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	memmove( buf, m_pSnHeadData->Sn_ZMSYO, 12 );
	int	Length = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_ZMSYO, 12 );
	Length = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_ZMSYO, 12 );
	memmove( buf, m_pSnHeadData->Sn_ZMSYO, Length );
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	// 署番号
	x = 145;
	memset( buf, '\0', sizeof( buf ) );
	if( (m_pSnHeadData->Sn_TAXNO[0]&0xff) != 0x00 && (m_pSnHeadData->Sn_TAXNO[0]&0xff) != 'F' ){
		sprintf_s( buf, sizeof( buf ), "署番号 %-5.5s", m_pSnHeadData->Sn_TAXNO );
	}
	else{
		sprintf_s( buf, sizeof( buf ), "署番号未登録" );
	}
	dgda.data_disp = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 整理番号
	x = CHUSEIRI;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
	len = (int)strlen( buf );
	str.Empty();
	for( i=8; i>0; i-- ){
		if( len == i ){
			str += buf;
			break;
		}
		str += ' ';
	}
	dgda.data_edit = str;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 課税期間
	x = CHUCHUKJI;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	gengo = 0;
//-- '19.04.02 --
//	m_pSnHeadData->GetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
//---------------
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
//---------------
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt && (gengo==ID_ICSSH_REIWA_GENGO) ){
		sprintf_s(buf, sizeof(buf), _T("%s"), SYZ26_REIWA_G_STR);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUCHUKJI_G, &dgda, CIcsdiagctrl);

	x = CHUCHUKITA;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	gengo = 0;
//-- '19.04.02 --
//	m_pSnHeadData->GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
//---------------
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
//---------------
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt && (gengo==ID_ICSSH_REIWA_GENGO) ){
		sprintf_s(buf, sizeof(buf), _T("%s"), SYZ26_REIWA_G_STR);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUCHUKITA_G, &dgda, CIcsdiagctrl);

	// 中間申告の計算期間
	x = CHUTAIJI;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	gengo = 0;
//-- '19.04.02 --
//	m_pSnHeadData->GetYmdData( ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
//---------------
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
//---------------
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt && (gengo==ID_ICSSH_REIWA_GENGO) ){
		sprintf_s(buf, sizeof(buf), _T("自%s"), SYZ26_REIWA_G_STR);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("自%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUTAIJI_G, &dgda, CIcsdiagctrl);

	x = CHUTAIITA;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	gengo = 0;
//-- '19.04.02 --
//	m_pSnHeadData->GetYmdData( ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
//---------------
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
//---------------
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt && (gengo==ID_ICSSH_REIWA_GENGO) ){
		sprintf_s(buf, sizeof(buf), _T("至%s"), SYZ26_REIWA_G_STR);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("至%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUTAIITA_G, &dgda, CIcsdiagctrl);

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラムデータセット
//-----------------------------------------------------------------------------
void CShinChskEX::SetDiagData()
{
	int		i, x;
	DIAGRAM_DATA	dgda;
	char	buf[128] = {0};
	char	val[MONY_BUF_SIZE], nm[MONY_BUF_SIZE];

	m_pArith->l_input( nm, _T("100") );

	diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );

	bool isReiwaPrnt = false;
	if( m_Util.GetPrePrintGengo(m_pSnHeadData, false) == 2 ){
		isReiwaPrnt = true;
	}

	//初期化
	diag_clear( IDC_ICSDIAGCTRL1, CHUZKJI, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUZKITA, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUSNMD, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUZZEI, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUTU1, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUTU2, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUNZEI, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUNTZEI, TRUE, CIcsdiagctrl );
	diag_clear( IDC_ICSDIAGCTRL1, CHUZEIGOU, TRUE, CIcsdiagctrl );

	// 前課税期間
	char	yymmdd[4]={0};
	int		gengo = 0;
	x = CHUZKJI;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	m_Util.DiagOcxIniz(&dgda);
	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt ){
		if( gengo==ID_ICSSH_REIWA_GENGO ){
			sprintf_s(buf, sizeof(buf), _T("自%s"), SYZ26_REIWA_G_STR);
		}
		else if( gengo==ID_ICSSH_HEISEI_GENGO ){
			sprintf_s(buf, sizeof(buf), _T("自%s"), SYZ26_HEISEI_G_STR);
		}
		else{
			sprintf_s(buf, sizeof(buf), _T("自%s"), SYZ26_EMPTY_G_STR);
		}
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("自%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUZKJI_G, &dgda, CIcsdiagctrl);

	x = CHUZKITA;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	gengo = 0;
	m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	m_Util.DiagOcxIniz(&dgda);
	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt ){
		if( gengo==ID_ICSSH_REIWA_GENGO ){
			sprintf_s(buf, sizeof(buf), _T("至%s"), SYZ26_REIWA_G_STR);
		}
		else if( gengo==ID_ICSSH_HEISEI_GENGO ){
			sprintf_s(buf, sizeof(buf), _T("至%s"), SYZ26_HEISEI_G_STR);
		}
		else{
			sprintf_s(buf, sizeof(buf), _T("至%s"), SYZ26_EMPTY_G_STR);
		}
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("至%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUZKITA_G, &dgda, CIcsdiagctrl);

	// 修正・更正・決定の年月日
	x = CHUSNMD;
	m_Util.DiagOcxIniz( &dgda );
	memset( yymmdd, '\0', sizeof(yymmdd) );
	gengo = 0;
	m_pSnChskData->GetYmdDataGen( ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
	memmove( dgda.data_day, yymmdd, 3 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	m_Util.DiagOcxIniz(&dgda);
	memset(buf, '\0', sizeof(buf));
	if( isReiwaPrnt ){
		if( gengo==ID_ICSSH_REIWA_GENGO ){
			sprintf_s(buf, sizeof(buf), _T("　%s"), SYZ26_REIWA_G_STR);
		}
		else if( gengo==ID_ICSSH_HEISEI_GENGO ){
			sprintf_s(buf, sizeof(buf), _T("　%s"), SYZ26_HEISEI_G_STR);
		}
		else{
			sprintf_s(buf, sizeof(buf), _T("　%s"), SYZ26_EMPTY_G_STR);
		}
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("　%s"), SYZ26_HEISEI_G_STR);
	}
	dgda.data_disp = buf;
	diag_setdata(IDC_ICSDIAGCTRL1, CHUSNMD_G, &dgda, CIcsdiagctrl);

	// 前課税期間の消費税額
	x = CHUZZEI;
	m_Util.DiagOcxIniz( &dgda );
	memmove( val, m_pSnHonpyoData->Sn_ZNOFZ, MONY_BUF_SIZE );
	m_pArith->l_div( val, val, nm );
	memmove( dgda.data_val, val, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 月数換算
//	Tuki_Set();
	x = CHUTU1;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	if( m_pSnChskData->Sn_BSMON ){
		wsprintf( buf, _T("%2d"), m_pSnChskData->Sn_BSMON );
	}
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );
	x = CHUTU2;
	m_Util.DiagOcxIniz( &dgda );
	memset( buf, '\0', sizeof( buf ));
	if( m_pSnChskData->Sn_BBMON ){
		wsprintf( buf, _T("%2d"), m_pSnChskData->Sn_BBMON );
	}
	dgda.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 修正申告
	for( i=0, x=27; i<5; i++, x++ ){
		diag_modify( IDC_ICSDIAGCTRL1, x, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
	}

	// 納付すべき消費税額
	x = CHUNZEI;
	m_Util.DiagOcxIniz( &dgda );
	memmove( val, m_pSnHonpyoData->Sn_EDNOFZ, 6 );
	m_pArith->l_div( val, val, nm );
	memmove( dgda.data_val, val, 6 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 納付すべき地方消費税額
	x = CHUNTZEI;
	m_Util.DiagOcxIniz( &dgda );
	memmove( val, m_pSnHonpyoData->Sn_TEDNOF, 6 );
	m_pArith->l_div( val, val, nm );
	memmove( dgda.data_val, val, 6 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	// 合計納付税額
	x = CHUZEIGOU;
	m_Util.DiagOcxIniz( &dgda );
	memmove( val, m_pSnChskData->Sn_KTNFZ, 6 );
	m_pArith->l_div( val, val, nm );
	memmove( dgda.data_val, val, 6 );
	diag_setdata( IDC_ICSDIAGCTRL1, x, &dgda, CIcsdiagctrl );

	if( m_Crsw ){
		SetCalq( 0x01 );
	}
	else{
		Disp_Calq();
	}
}

//-----------------------------------------------------------------------------
// 計算セット
//-----------------------------------------------------------------------------
// 引数	flg	：	
//-----------------------------------------------------------------------------
void CShinChskEX::SetCalq( char flg )
{
	if( flg&0x0f ){
		CalqSyz( flg&0x0f );
	}
	else if( flg&0xf0 ){
		CalqSyzRevised();
	}
}

//-----------------------------------------------------------------------------
// 消費税計算
//-----------------------------------------------------------------------------
// 引数	flg	：	
//-----------------------------------------------------------------------------
void CShinChskEX::CalqSyz( char flg )
{
	DIAGRAM_DATA	data;
	char ans[MONY_BUF_SIZE]={0}, num1[MONY_BUF_SIZE]={0}, num2[MONY_BUF_SIZE]={0};
	char n100[MONY_BUF_SIZE]={0};
	char buf[128]={0};

	m_pArith->l_input( n100, _T("100") );

	if( flg == 0x01 ){
		// 納付すべき消費税額
		m_pArith->l_clear( num1 );
		m_pArith->l_clear( num2 );
		m_pArith->l_clear( ans );

		if( m_pSnChskData->Sn_BBMON && m_pSnChskData->Sn_BSMON ){
			memmove( num1, m_pSnHonpyoData->Sn_ZNOFZ, MONY_BUF_SIZE );
			sprintf_s( buf, sizeof( buf ), _T("%d"), m_pSnChskData->Sn_BBMON );
			m_pArith->l_input( num2, buf );
			m_pArith->l_div( ans, num1, num2 );
			m_pArith->l_clear( num1 );
			m_pArith->l_clear( num2 );
			memmove( num1, ans, MONY_BUF_SIZE );
			sprintf_s( buf, sizeof( buf ), _T("%d"), m_pSnChskData->Sn_BSMON );
			m_pArith->l_input( num2, buf );
			m_pArith->l_mul( ans, num1, num2 );
		}
		m_Util.DiagOcxIniz( &data );
		diag_getdata( IDC_ICSDIAGCTRL1, CHUNZEI, &data, CIcsdiagctrl );
		if( m_pArith->l_test( ans ) ){
			m_pArith->l_div( ans, ans, n100 );
		}
		memmove( data.data_val, ans, MONY_BUF_SIZE );
		if( m_pArith->l_test( ans ) ){
			m_pArith->l_mul( ans, ans, n100 );
		}
		memmove( m_pSnHonpyoData->Sn_EDNOFZ, ans, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, CHUNZEI, &data, CIcsdiagctrl );
		disp_clr( 0, CHUNZEI );
	}

	// 納付すべき地方消費税額
	if( (flg==0x01) || (flg==0x02) ){
		m_pArith->l_clear( num1 );
		m_pArith->l_clear( num2 );
		m_pArith->l_clear( ans );
		memmove( num1, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
		if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_8PER_REFORM_DAY ){
			m_Util.percent( ans, num1, 8, 63, 0 );
		}
		else{
			m_pArith->l_input( num2, _T("25") );
			m_pArith->l_mul100((BYTE*)ans, (BYTE*)num1, (BYTE*)num2, 0, 0 );
		}

		m_Util.DiagOcxIniz( &data );
		diag_getdata( IDC_ICSDIAGCTRL1, CHUNTZEI, &data, CIcsdiagctrl );
		m_pArith->l_div( ans, ans, n100 );
		memmove( data.data_val, ans, MONY_BUF_SIZE );
		m_pArith->l_mul( ans, ans, n100 );
		memmove( m_pSnHonpyoData->Sn_TEDNOF, ans, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, CHUNTZEI, &data, CIcsdiagctrl );
		disp_clr( 0, CHUNTZEI );
	}

	// 合計
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	m_pArith->l_clear( ans );
	memmove( num1, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
	memmove( num2, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );
	m_pArith->l_add( ans, num1, num2 );
	m_Util.DiagOcxIniz( &data );
	m_pArith->l_div( ans, ans, n100 );
	diag_getdata( IDC_ICSDIAGCTRL1, CHUZEIGOU, &data, CIcsdiagctrl );
	memmove( data.data_val, ans, MONY_BUF_SIZE );
	m_pArith->l_mul( ans, ans, n100 );
	memmove( m_pSnChskData->Sn_KTNFZ, ans, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAGCTRL1, CHUZEIGOU, &data, CIcsdiagctrl );
	disp_clr( 0, CHUZEIGOU );

	CalqSyzDivide();
}

//-----------------------------------------------------------------------------
// 分割納付税額の計算
//-----------------------------------------------------------------------------
void CShinChskEX::CalqSyzDivide()
{
	switch( m_pSnHeadData->Sn_MCOUNT ){
		case 3:
			switch( m_pSnHeadData->Sn_JCOUNT ){
				case 1:
					memmove( m_pSnHeadData->Sn_TYNF1, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
					memmove( m_pSnHeadData->Sn_TTYNF1, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );
					break;
				case 2:
					memmove( m_pSnHeadData->Sn_TYNF2, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
					memmove( m_pSnHeadData->Sn_TTYNF2, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );
					break;
				case 3:
					memmove( m_pSnHeadData->Sn_TYNF3, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
					memmove( m_pSnHeadData->Sn_TTYNF3, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );
					break;
				case 4:
					memmove( m_pSnHeadData->Sn_TYNF4, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
					memmove( m_pSnHeadData->Sn_TTYNF4, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );
					break;
				default:	break;
			}
		case 11:
			if( m_pSnHeadData->Sn_JCOUNT < 12 ){
				memmove( m_pSnHeadData->Sn_TYNF[m_pSnHeadData->Sn_JCOUNT-1], m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );
				memmove( m_pSnHeadData->Sn_TTNF[m_pSnHeadData->Sn_JCOUNT-1], m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );
			}
			break;
		default:
			break;
	}
}

//-----------------------------------------------------------------------------
// 修正申告
//-----------------------------------------------------------------------------
void CShinChskEX::CalqSyzRevised()
{
	DIAGRAM_DATA	data;
	char ans[MONY_BUF_SIZE]={0}, num1[MONY_BUF_SIZE]={0}, num2[MONY_BUF_SIZE]={0};
	char n100[MONY_BUF_SIZE]={0};

	m_pArith->l_input( n100, _T("100") );

	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	m_pArith->l_clear( ans );
	memmove( num1, m_pSnChskData->Sn_ZSKZN, MONY_BUF_SIZE );
	memmove( num2, m_pSnChskData->Sn_ZSKAD, MONY_BUF_SIZE );
	m_pArith->l_add( ans, num1, num2 );

	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	memmove( num1, ans, MONY_BUF_SIZE );
	memmove( num2, m_pSnChskData->Sn_ZSTZN, MONY_BUF_SIZE );
	m_pArith->l_clear( ans );
	m_pArith->l_add( ans, num1, num2 );

	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	memmove( num1, ans, MONY_BUF_SIZE );
	memmove( num2, m_pSnChskData->Sn_ZSTAD, MONY_BUF_SIZE );
	m_pArith->l_clear( ans );
	m_pArith->l_add( ans, num1, num2 );

	m_Util.DiagOcxIniz( &data );
	diag_getdata( IDC_ICSDIAGCTRL1, CHUSYUKEI, &data, CIcsdiagctrl );
	m_pArith->l_div( ans, ans, n100 );
	memmove( data.data_val, ans, MONY_BUF_SIZE );
	m_pArith->l_mul( ans, ans, n100 );
	memmove( m_pSnChskData->Sn_ZSKTZ, ans, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAGCTRL1, CHUSYUKEI, &data, CIcsdiagctrl );
	disp_clr( 0, CHUSYUKEI );
}

//-----------------------------------------------------------------------------
// 計算
//-----------------------------------------------------------------------------
void CShinChskEX::Disp_Calq()
{
	char ans[6], num1[6], num2[6];
	char n100[6];
	char buf[128];
	char sw = 0;

	m_pArith->l_input( n100, _T("100") );

	// 6, 7
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	m_pArith->l_clear( ans );
	memcpy( num1, m_pSnChskData->Sn_ZSKZN, 6 );
	memcpy( num2, m_pSnChskData->Sn_ZSKAD, 6 );
	m_pArith->l_add( ans, num1, num2 );
	// 8
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	memcpy( num1, ans, 6 );
	memcpy( num2, m_pSnChskData->Sn_ZSTZN, 6 );
	m_pArith->l_clear( ans );
	m_pArith->l_add( ans, num1, num2 );
	// 9
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	memcpy( num1, ans, 6 );
	memcpy( num2, m_pSnChskData->Sn_ZSTAD, 6 );
	m_pArith->l_clear( ans );
	m_pArith->l_add( ans, num1, num2 );
	// 10
	m_pArith->l_div( ans, ans, n100 );
	m_pArith->l_mul( ans, ans, n100 );
	if( m_pArith->l_cmp( m_pSnChskData->Sn_ZSKTZ, ans )){
		sw = 1;
	}
	else{
		sw = 0;
	}
	disp_clr( sw, CHUSYUKEI );

	// 納付すべき消費税額
// [前課税期間の消費税額*分子/分母]の順で計算していたのを
// [前課税期間の消費税額/分母*分子]の順で計算するとように変更。
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	m_pArith->l_clear( ans );

	if( m_pSnChskData->Sn_BBMON && m_pSnChskData->Sn_BSMON ){
		memcpy( num1, m_pSnHonpyoData->Sn_ZNOFZ, 6 );
		sprintf_s( buf, sizeof( buf ), "%d", m_pSnChskData->Sn_BBMON );
		m_pArith->l_input( num2, buf );
		m_pArith->l_div( ans, num1, num2 );
		m_pArith->l_clear( num1 );
		m_pArith->l_clear( num2 );
		memcpy( num1, ans, 6 );
		sprintf_s( buf, sizeof( buf ), "%d", m_pSnChskData->Sn_BSMON );
		m_pArith->l_input( num2, buf );
		m_pArith->l_clear( ans );
		m_pArith->l_mul( ans, num1, num2 );

		m_pArith->l_div( ans, ans, n100 );
		m_pArith->l_mul( ans, ans, n100 );
	}

	if( m_pArith->l_cmp( m_pSnHonpyoData->Sn_EDNOFZ, ans )){
		sw = 1;
	}
	else{
		sw = 0;
	}
	disp_clr( sw, CHUNZEI );

	// 納付すべき地方消費税額
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	m_pArith->l_clear( ans );
	memcpy( num1, m_pSnHonpyoData->Sn_EDNOFZ, 6 );
	if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_8PER_REFORM_DAY ){
		m_Util.percent( ans, num1, 8, 63, 0 );
	}
	else{
		m_pArith->l_input( num2, "25" );
		m_pArith->l_mul100((BYTE*)ans, (BYTE*)num1, (BYTE*)num2, 0, 0 );
	}

	m_pArith->l_div( ans, ans, n100 );
	m_pArith->l_mul( ans, ans, n100 );
	if( m_pArith->l_cmp( m_pSnHonpyoData->Sn_TEDNOF, ans )){
		sw = 1;
	}
	else{
		sw = 0;
	}
	disp_clr( sw, CHUNTZEI );

	// 合計
	m_pArith->l_clear( num1 );
	m_pArith->l_clear( num2 );
	m_pArith->l_clear( ans );
	memcpy( num1, m_pSnHonpyoData->Sn_EDNOFZ, 6 );
	memcpy( num2, m_pSnHonpyoData->Sn_TEDNOF, 6 );
	m_pArith->l_add( ans, num1, num2 );
	m_pArith->l_div( ans, ans, n100 );
	m_pArith->l_mul( ans, ans, n100 );
	if( m_pArith->l_cmp( m_pSnChskData->Sn_KTNFZ, ans )){
		sw = 1;
	}
	else{
		sw = 0;
	}
	disp_clr( sw, CHUZEIGOU );
}

BOOL CShinChskEX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 背景色の変更
//-----------------------------------------------------------------------------
void CShinChskEX::disp_clr( char sw, short idx )
{
	DIAGRAM_ATTRIBUTE	dattr;
	diag_getattr( IDC_ICSDIAGCTRL1, idx, &dattr,CIcsdiagctrl );
//	if( sw )	dattr.attr_bcolor = BC_BLUE;	//入力後背景色
//	else		dattr.attr_bcolor = BC_WHITE;	//デフォルト背景色
	diag_setattr( IDC_ICSDIAGCTRL1, idx, &dattr, TRUE,CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 月をセットする
//-----------------------------------------------------------------------------
void CShinChskEX::Tuki_Set()
{
//2016.06.22 UPDATE START
	// 分子
	//if( m_pSnHeadData->Sn_MCOUNT == 1 ){
	//	if( m_pSnChskData->Sn_BSMON !=  6 ){
	//		m_Crsw = 1;
	//	}
	//	m_pSnChskData->Sn_BSMON = 6;
	//}
	//else if( m_pSnHeadData->Sn_MCOUNT ==  3 ){
	//	if( m_pSnChskData->Sn_BSMON !=  3 ){
	//		m_Crsw = 1;
	//	}
	//	m_pSnChskData->Sn_BSMON = 3;
	//}
	//else if( m_pSnHeadData->Sn_MCOUNT ==  11 ){
	//	if( m_pSnChskData->Sn_BSMON !=  1 ){
	//		m_Crsw = 1;
	//	}
	//	m_pSnChskData->Sn_BSMON = 1;
	//}

	// 分子
	if((m_pSnHeadData->Sn_Sign4&0x80)&&( m_pSnChskData->Sn_BSMON != 0 )){
		m_bunshi_sw = 0x01;
	}

	if( m_pSnHeadData->Sn_MCOUNT == 1 ){
		if( m_pSnChskData->Sn_BSMON !=  6 ){
			m_Crsw = 1;
		}
		if( !(m_bunshi_sw&0x01)){
			m_pSnChskData->Sn_BSMON = 6;
		}
	}
	else if( m_pSnHeadData->Sn_MCOUNT ==  3 ){
		if( m_pSnChskData->Sn_BSMON !=  3 ){
			m_Crsw = 1;
		}
		if( !(m_bunshi_sw&0x01)){
			m_pSnChskData->Sn_BSMON = 3;
		}
	}
	else if( m_pSnHeadData->Sn_MCOUNT ==  11 ){
		if( m_pSnChskData->Sn_BSMON !=  1 ){
			m_Crsw = 1;
		}
		if( !(m_bunshi_sw&0x01)){
			m_pSnChskData->Sn_BSMON = 1;
		}
	}
//2016.06.22 UPDATE END

	// 分母
//--- '15.12.10 ---
//	if( !m_pSnChskData->Sn_BBMON ){
////-- '15.08.28 --
////		m_pSnChskData->Sn_BBMON = 12;
////---------------
//		CalqMonthSpan();
////---------------
//		m_Crsw = 1;
//	}
//-----------------
	if( !m_pSnChskData->Sn_BBMON || (m_pSnChskData->Sn_BBMON>12) ){
		CalqMonthSpan();
		m_Crsw = 1;
	}
//-----------------
}

void CShinChskEX::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	CSyzBaseDlg::OnOK();
}

void CShinChskEX::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	CSyzBaseDlg::OnCancel();
}

BEGIN_EVENTSINK_MAP(CShinChskEX, CSyzBaseDlg)
	ON_EVENT(CShinChskEX, IDC_ICSDIAGCTRL1, 1, CShinChskEX::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinChskEX, IDC_ICSDIAGCTRL1, 2, CShinChskEX::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinChskEX, IDC_ICSDIAGCTRL1, 3, CShinChskEX::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinChskEX::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAGCTRL1;
	m_Index = index;
}

void CShinChskEX::EditOFFIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	xdata;
	long	flg = 0;
	char	dbuf[128]={0};
	char	*buff;
	char	val[6]={0}, num[6]={0}, sday[4]={0}, eday[4]={0}, tmpday[4]={0};
	int		len, tuki;
	int		st = 0x00;
	char	Str[128] = {0};
	int		Length = 0;
	char	yymmdd[3]={0};
	int		sday_gengo=0, eday_gengo=0, tmp_gengo=0;

	int	setYmdGengo = 0;
	bool isReiwaPrnt = false;
	if( m_Util.GetPrePrintGengo(m_pSnHeadData, false) == 2 ){
		isReiwaPrnt = true;
		setYmdGengo = ID_ICSSH_REIWA_GENGO;
	}

	// 変換
	CVolDateDB	voldate;
	char	seSday[4]={ 0 }, seEday[4]={ 0 };
	int		se_gengo=0;
	int		tmpGengo=0;

	m_Util.DiagOcxIniz( &xdata );

	diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	diag_getdata( IDC_ICSDIAGCTRL1, index, &xdata, CIcsdiagctrl );

	len = xdata.data_edit.GetLength();
	memset( dbuf, '\0', sizeof( dbuf ));
	buff = (char *)((LPCTSTR)(xdata.data_edit));
	memset( val, '\0', 6 );
	memset( sday, '\0', 4 );
	memset( eday, '\0', 4 );

	m_pArith->l_input( num, _T("100") );

	memcpy( val, xdata.data_val, 6 );
	m_pArith->l_mul( val, val, num );
	switch( index ){
	case CHUSEIRI:	// 整理番号
		Length = xdata.data_edit.GetLength();
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, xdata.data_edit, Length );
		m_pArith->l_pack( m_pSnHeadData->Sn_SEIRI, Str, 8 );
		break;
	case CHUZKJI:		// 前課税期間 自
		memmove( sday, xdata.data_day, 3 );
		memset( eday, '\0', sizeof(eday) );
		m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &eday[0], &eday[1], &eday[2], &eday_gengo );

		// 和暦→西暦変換
		memmove(tmpday, sday, sizeof(sday));
		if( isReiwaPrnt ){
			tmpGengo = ID_ICSSH_REIWA_GENGO;
		}
		else{
			m_Util.CnvNoGengoToGengoWareki((UCHAR*)tmpday, &tmpGengo, (UCHAR*)sday);
		}
		voldate.db_datecnvGen(0, (UCHAR*)sday, &se_gengo, (UCHAR*)seSday, 1, 0);
		voldate.db_datecnvGen(eday_gengo, (UCHAR*)eday, &se_gengo, (UCHAR*)seEday, 1, 0);

//		if( _CheckYYMMDD(sday, eday, 0) == 0 ){
		if( _CheckYYMMDD(seSday, seEday, 0 ) == 0 ){
			// 西暦→和暦変換
			voldate.db_datecnvGen(0, (UCHAR*)seSday, &sday_gengo, (UCHAR*)sday, 0, 0);
			voldate.db_datecnvGen(0, (UCHAR*)seEday, &eday_gengo, (UCHAR*)eday, 0, 0);

			memmove( xdata.data_day, sday, 3 );
//			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 0, eday[0] );
//			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 1, eday[1] );
//			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 2, eday[2] );
//			m_pSnChskData->SetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, -1, eday[0], eday[1], eday[2], eday_gengo);
			m_pSnChskData->SetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, -1, sday[0], sday[1], sday[2], sday_gengo);
		}
		else{
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &sday_gengo);
			memmove( xdata.data_day, yymmdd, 3 );
		}
		break;
	case CHUZKITA:		// 前課税期間 至
		m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &sday[0], &sday[1], &sday[2], &sday_gengo );
		memmove( eday, xdata.data_day, 3 );

		// 和暦→西暦変換
		memmove(tmpday, sday, sizeof(sday));
		if( isReiwaPrnt ){
			tmpGengo = ID_ICSSH_REIWA_GENGO;
		}
		else{
			m_Util.CnvNoGengoToGengoWareki((UCHAR*)tmpday, &tmpGengo, (UCHAR*)sday);
		}
		voldate.db_datecnvGen(sday_gengo, (UCHAR*)sday, &se_gengo, (UCHAR*)seSday, 1, 0);
		voldate.db_datecnvGen(0, (UCHAR*)eday, &se_gengo, (UCHAR*)seEday, 1, 0);

//		if( _CheckYYMMDD( sday, eday, 1 ) == 0 ){
		if( _CheckYYMMDD(seSday, seEday, 1) == 0 ){
			// 西暦→和暦変換
			voldate.db_datecnvGen(0, (UCHAR*)seSday, &sday_gengo, (UCHAR*)sday, 0, 0);
			voldate.db_datecnvGen(0, (UCHAR*)seEday, &eday_gengo, (UCHAR*)eday, 0, 0);

			memmove( xdata.data_day, eday, 3 );
//			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 0, eday[0] );
//			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 1, eday[1] );
//			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 2, eday[2] );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, -1, eday[0], eday[1], eday[2], eday_gengo );
		}
		else{
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &eday_gengo );
			memmove( xdata.data_day, yymmdd, 3 );
		}
		break;
	case CHUSNMD:		// 修正・更正・決定の年月日
		if( (xdata.data_day[0]&0xff) == 0xff ){
			memset( xdata.data_day, '\0', 3 );
		}
		memset( yymmdd, '\0', sizeof(yymmdd) );
		m_pSnChskData->GetYmdDataGen( ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2], &sday_gengo );
		memmove( xdata.data_day, yymmdd, 3 );						
		break;
	case CHUZZEI:		// 前課税期間の消費税額
		if( xdata.data_check ){
			memmove( m_pSnHonpyoData->Sn_ZNOFZ, val, 6 );
			st = 0x01;
		}
		break;
	case CHUTU1:		// 月数換算 分子
		memmove( dbuf, buff, len );
		m_pSnChskData->Sn_BSMON = (unsigned char)atoi( dbuf );
		st = 0x01;
		break;
	case CHUTU2:		// 月数換算 分母
		memmove( dbuf, buff, len );
		tuki = (unsigned char)atoi( dbuf );
		if(( tuki >= 1 )&&( tuki <= 12 )){
			m_pSnChskData->Sn_BBMON = tuki;
		}
		else{
			wsprintf( dbuf, "%2d", m_pSnChskData->Sn_BBMON );
			xdata.data_edit = dbuf;
		}
		st = 0x01;
		break;
	case  CHUNZEI:	// 納付すべき消費税額
		if( xdata.data_check ){
			memmove( m_pSnHonpyoData->Sn_EDNOFZ, val, 6 );
			disp_clr( 1, index );
			st = 0x02;
		}
		break;
	case  CHUNTZEI:	// 納付すべき消費税額
		if( xdata.data_check ){
			memmove( m_pSnHonpyoData->Sn_TEDNOF, val, 6 );
			disp_clr( 1, index );
			st = 0x03;
		}
		break;
	case CHUZEIGOU:	// 合計納付税額
		if( xdata.data_check ){
			memmove( dbuf, buff, len );
			if( strchr( dbuf, '*' ) == NULL ){
				memmove( m_pSnChskData->Sn_KTNFZ, val, 6 );
				disp_clr( 1, index );
			}
		}
		else	return;
		break;
	case CHUNOU1:	// 納税地　上段
		memset( Str, '\0', sizeof( Str ) );
		Length = xdata.data_edit.GetLength();
		memmove( Str, xdata.data_edit, Length );
		memmove( m_pSnHeadData->Sn_NOZEI, Str, 40 );
		break;
	case CHUNOU2:	// 納税地　下段
		memset( Str, '\0', sizeof( Str ) );
		Length = xdata.data_edit.GetLength();
		memmove( Str, xdata.data_edit, Length );
		memmove( &m_pSnHeadData->Sn_NOZEI[40], Str, 40 );
		break;
	case CHUTEL1:	// 電話番号　市外
		Length = xdata.data_edit.GetLength();
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, xdata.data_edit, Length );
		memmove( m_pSnHeadData->Sn_TELNO1, Str, 6 );
		break;
	case CHUTEL2:	// 電話番号　市内
		Length = xdata.data_edit.GetLength();
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, xdata.data_edit, Length );
		memmove( m_pSnHeadData->Sn_TELNO2, Str, 4 );
		break;
	case CHUTEL3:	// 電話番号　個人
		Length = xdata.data_edit.GetLength();
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, xdata.data_edit, Length );
		memmove( m_pSnHeadData->Sn_TELNO3, Str, 4 );
		break;
	case CHUMEIFRI:	// 名称又は屋号フリガナ
//2016.10.20 UPDATE START
		CopyFuriganaEX( m_pSnHeadData->Sn_COFRI, xdata.data_edit );
//2016.10.20 UPDATE END
		break;
	case CHUMEI:	// 名称又は屋号
		Length = xdata.data_edit.GetLength();
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, xdata.data_edit, Length );
		memmove( m_pSnHeadData->Sn_CONAM, Str, 40 );
		break;

	//-------->個人番号処理ここに追加する

	//<----------------

	case CHUDAIFRI:	// 代表者氏名又は氏名フリガナ
		CopyFuriganaEX(m_pSnHeadData->Sn_DIFRI,xdata.data_edit);
		break;
	case CHUDAI:	// 代表者氏名又は氏名
		Length = xdata.data_edit.GetLength();
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, xdata.data_edit, Length );
		memmove( m_pSnHeadData->Sn_DIHYO, Str, 40 );
		break;
	case CHUZEINEN:	// 提出年月日
		if( (xdata.data_day[0]&0xff) == 0xff || (xdata.data_day[0]&0xff) == 0x00 )	memset( xdata.data_day, '\0', 3 );
//		m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 0, xdata.data_day[0] );
//		m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 1, xdata.data_day[1] );
//		m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 2, xdata.data_day[2] );
		m_pSnHeadData->SetYmdDataGen(ID_ICSSH_PRESENT_DATE, -1, xdata.data_day[0], xdata.data_day[1], xdata.data_day[2], setYmdGengo);
		if( ((xdata.data_day[0]&0xff)!=0xff) && ((xdata.data_day[0]&0xff)!=0x00) ){
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &xdata.data_day[0], &xdata.data_day[1], &xdata.data_day[2] );
		}
		break;
	case CHUZEISYO:	// 税務署名
		memset( Str, '\0', sizeof( Str ) );
		memmove( Str, m_pSnHeadData->Sn_ZMSYO, 12 );
		Length = (int)xdata.data_edit.GetLength();
		memset( m_pSnHeadData->Sn_ZMSYO, '\0', 12 );
		memmove( m_pSnHeadData->Sn_ZMSYO, xdata.data_edit, Length );
		if( Length ){
			char	name1[20], name2[20];
			char	buf[256];
			memset( name1, '\0', sizeof( name1 ) );
			memset( name2, '\0', sizeof( name2 ) );
			memset( buf, '\0', sizeof( buf ) );
			memmove( name1, m_pSnHeadData->Sn_ZMSYO, 12 );
			memmove( name2, Str, 12 );
			if( Esel_CompareTaxOffice( name1, name2 ) ){
				memset( m_pSnHeadData->Sn_TAXNO, '\0', 5 );
				// 税務署コードの取得
				if( *m_pShinInfo->pIsOpenEselect ){
					if( m_pSnHeadData->GetTaxationOffice(this, m_pShinInfo->pEselect) ){
						memset( buf, '\0', sizeof(buf) );
						if( ((m_pSnHeadData->Sn_TAXNO[0]&0xff)!=0x00) && ((m_pSnHeadData->Sn_TAXNO[0]&0xff)!='F') ){
							sprintf_s( buf, sizeof(buf), "署番号 %-5.5s", m_pSnHeadData->Sn_TAXNO );
						}
						else{
							sprintf_s( buf, sizeof(buf), "署番号未登録" );
						}
						xdata.data_disp = buf;
						diag_setdata( IDC_ICSDIAGCTRL1, CHUSYONO, (LPUNKNOWN)&xdata, CIcsdiagctrl );

						diag_setposition( IDC_ICSDIAGCTRL1, CHUZEISYO , CIcsdiagctrl );//キャンセルの場合フォーカスそのまま

						break;
					}
					memset( buf, '\0', sizeof(buf) );
					len = m_Util.MixSpaceCutLength( (char*)m_pSnHeadData->Sn_ZMSYO, sizeof(m_pSnHeadData->Sn_ZMSYO) );
					memmove( buf, m_pSnHeadData->Sn_ZMSYO, len );
					xdata.data_edit = buf;
					diag_setdata( IDC_ICSDIAGCTRL1, CHUZEISYO, (LPUNKNOWN)&xdata, CIcsdiagctrl );

					diag_setposition( IDC_ICSDIAGCTRL1, CHUSEIRI, CIcsdiagctrl );//フォーカス次へ

//					return;
				}
			}
			// 税務署コード
			memset( buf, '\0', sizeof(buf) );
			if( ((m_pSnHeadData->Sn_TAXNO[0]&0xff)!=0x00) && ((m_pSnHeadData->Sn_TAXNO[0]&0xff)!='F') ){
				sprintf_s( buf, sizeof(buf), "署番号 %-5.5s", m_pSnHeadData->Sn_TAXNO );
			}
			else{
				sprintf_s( buf, sizeof(buf), "署番号未登録" );
			}
			xdata.data_disp = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, CHUSYONO, (LPUNKNOWN)&xdata, CIcsdiagctrl );

		}
		else{
			memset( m_pSnHeadData->Sn_TAXNO, '\0', 5 );
			xdata.data_disp = "署番号未登録";
			diag_setdata( IDC_ICSDIAGCTRL1, CHUSYONO, (LPUNKNOWN)&xdata, CIcsdiagctrl );
		}
		return;
		break;

//★//[12'10.24]///現在未使用
	// 修正申告
	case CHUSYUSZEI1:		// 消費税　この申告前の税額
		if( xdata.data_check ){
			memmove( m_pSnChskData->Sn_ZSKZN, val, 6 );
			st = 0x10;
		}
		break;
	case CHUSYUSZEI2:		// 消費税　この申告により増加する税額
		if( xdata.data_check ){
			memmove( m_pSnChskData->Sn_ZSKAD, val, 6 );
			st = 0x10;
		}
		break;
	case CHUSYUTZEI1:		// 地方消費税　この申告前の税額
		if( xdata.data_check ){
			memmove( m_pSnChskData->Sn_ZSTZN, val, 6 );
			st = 0x10;
		}
		break;
	case CHUSYUTZEI2:		// 地方消費税　この申告により増加する税額
		if( xdata.data_check ){
			memmove( m_pSnChskData->Sn_ZSTAD, val, 6 );
			st = 0x10;
		}
		break;
	case CHUSYUKEI:			// 合計納付税額
		if( xdata.data_check ){
			memmove( dbuf, buff, len );
			if( strchr( dbuf, '*' ) == NULL ){
				memmove( m_pSnChskData->Sn_ZSKTZ, val, 6 );
				disp_clr( 1, index );
			}
		}
		else	return;
		break;
	}
	diag_setdata( IDC_ICSDIAGCTRL1, index, (LPUNKNOWN)&xdata, CIcsdiagctrl );
	SetCalq( st );

	// 元号変更
	if( (index==CHUZKJI) || (index==CHUZKITA) || (index==CHUSNMD) ){
		// 元号決定
		int	tmpgengo = sday_gengo;
		if( index==CHUZKITA ){
			tmpgengo = eday_gengo;
		}
		char	*pGengoStr = SYZ26_EMPTY_G_STR;
		if( isReiwaPrnt ){
			if( tmpgengo==ID_ICSSH_REIWA_GENGO ){
				pGengoStr = SYZ26_REIWA_G_STR;
			}
			else if( tmpgengo==ID_ICSSH_HEISEI_GENGO ){
				pGengoStr = SYZ26_HEISEI_G_STR;
			}
		}
		else{
			pGengoStr = SYZ26_HEISEI_G_STR;
		}

		char			buf[128]={ 0 };
		DIAGRAM_DATA	dgda;
		m_Util.DiagOcxIniz(&dgda);
		if( index==CHUZKJI ){
			sprintf_s(buf, sizeof(buf), _T("自%s"), pGengoStr);
			dgda.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, CHUZKJI_G, &dgda, CIcsdiagctrl);
		}
		else if( index==CHUZKITA ){
			sprintf_s(buf, sizeof(buf), _T("至%s"), pGengoStr);
			dgda.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, CHUZKITA_G, &dgda, CIcsdiagctrl);
		}
		else if( index==CHUSNMD ){
			sprintf_s(buf, sizeof(buf), _T("　%s"), pGengoStr);
			dgda.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, CHUSNMD_G, &dgda, CIcsdiagctrl);
		}
	}
}

void CShinChskEX::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	long	flg = 0;
	char	dbuf[128]={ 0 };
	char	*buff;
	char	val[6]={ 0 }, num[6]={ 0 }, sday[4]={ 0 }, eday[4]={ 0 }, yymmdd[4]={ 0 }, tmpday[4]={ 0 };
	int		len, tuki;
	short	xpos;
	char	sw = 0x00, st = 0x00, sw2 = 0x00;
	char	Str[128] = {0};
	int		Length = 0;
	CString	ZipBf, AddBf;

	int setYmdGengo = 0;
	bool isReiwaPrnt = false;
	if( m_Util.GetPrePrintGengo(m_pSnHeadData, false) == 2 ){
		isReiwaPrnt = true;
		setYmdGengo = ID_ICSSH_REIWA_GENGO;
	}

	// 変換用
	CVolDateDB	voldate;
	char	seSday[4]={ 0 }, seEday[4]={ 0 };
	int		se_gengo=0;
	int		sday_gengo=0, eday_gengo=0, tmp_gengo=0;
	int		tmpGengo=0;
	int		chgSt=0;

	if( (nChar==VK_NEXT) || (nChar==VK_PRIOR) ){
		return;
	}

	char	VK_FLG = 0x00;
	short	wTerm = 0;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			wTerm = VK_TAB;
		}
	}

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );

	len = xdata.data_edit.GetLength();
	memset( dbuf, '\0', sizeof( dbuf ));
	buff = (char *)((LPCTSTR)(xdata.data_edit));
	memset( val, '\0', 6 );
	memset( sday, '\0', 4 );
	memset( eday, '\0', 4 );

	m_pArith->l_input( num, "100" );

	flg = REPLY_STAY;
	memcpy( val, xdata.data_val, 6 );
	m_pArith->l_mul( val, val, num );
	switch( index ){
	case CHUSEIRI:		// 整理番号
		if( nChar == VK_RETURN ){
			Length = xdata.data_edit.GetLength();
			memset( Str, '\0', sizeof( Str ) );
			memmove( Str, xdata.data_edit, Length );
			m_pArith->l_pack( m_pSnHeadData->Sn_SEIRI, Str, 8 );
		}
		else if( nChar == VK_DELETE ){
			flg = REPLY_OK;
			memset( m_pSnHeadData->Sn_SEIRI, '\0', 4 );
			m_pSnHeadData->Sn_SEIRI[0] = 0xff;
	 		diag_clear( IDC_ICSDIAGCTRL1, index, TRUE, CIcsdiagctrl );	
		}
		break;
	case CHUZKJI:		// 前課税期間 自
		if( nChar == VK_RETURN ){
			memmove( sday, xdata.data_day, 3 );
			memset( eday, '\0', sizeof(eday) );
			m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &eday[0], &eday[1], &eday[2], &eday_gengo );

			// 和暦→西暦変換
			memmove(tmpday, sday, sizeof(sday));
			if( isReiwaPrnt ){
				tmpGengo = ID_ICSSH_REIWA_GENGO;
			}
			else{
				m_Util.CnvNoGengoToGengoWareki((UCHAR*)tmpday, &tmpGengo, (UCHAR*)sday);
			}
			voldate.db_datecnvGen(0, (UCHAR*)sday, &se_gengo, (UCHAR*)seSday, 1, 0);
			voldate.db_datecnvGen(eday_gengo, (UCHAR*)eday, &se_gengo, (UCHAR*)seEday, 1, 0);

//			if( _CheckYYMMDD( sday, eday, 0 ) == 0 ){
			if( tmpGengo && (_CheckYYMMDD(seSday, seEday, 0)==0) ){

				// 西暦→和暦変換
				voldate.db_datecnvGen(0, (UCHAR*)seSday, &sday_gengo, (UCHAR*)sday, 0, 0);
				voldate.db_datecnvGen(0, (UCHAR*)seEday, &eday_gengo, (UCHAR*)eday, 0, 0);

				memmove( xdata.data_day, sday, 3 );
//				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 0, sday[0] );
//				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 1, sday[1] );
//				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 2, sday[2] );
				m_pSnChskData->SetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, -1, sday[0], sday[1], sday[2], sday_gengo);
			}
			else{
				m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &xdata.data_day[0], &xdata.data_day[1], &xdata.data_day[2] );
			}
		}
		else if( nChar == VK_DELETE ){
			flg = REPLY_OK;
			memset( xdata.data_day, '\0', 3 );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 0, xdata.data_day[0] );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 1, xdata.data_day[1] );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 2, xdata.data_day[2] );
		}
		break;
	case CHUZKITA:		// 前課税期間 至
		if( nChar == VK_RETURN ){
			m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &sday[0], &sday[1], &sday[2], &sday_gengo );
			memmove( eday, xdata.data_day, 3 );

			// 和暦→西暦変換
			memmove(tmpday, eday, sizeof(eday));
			if( isReiwaPrnt ){
				tmpGengo = ID_ICSSH_REIWA_GENGO;
			}
			else{
				m_Util.CnvNoGengoToGengoWareki((UCHAR*)tmpday, &tmpGengo, (UCHAR*)eday);
			}
			voldate.db_datecnvGen(sday_gengo, (UCHAR*)sday, &se_gengo, (UCHAR*)seSday, 1, 0);
			voldate.db_datecnvGen(0, (UCHAR*)eday, &se_gengo, (UCHAR*)seEday, 1, 0);

//			if( _CheckYYMMDD(sday, eday, 1) == 0 ){
			if( tmpGengo && (_CheckYYMMDD(seSday, seEday, 1 )==0) ){
				// 西暦→和暦変換
				voldate.db_datecnvGen(0, (UCHAR*)seSday, &sday_gengo, (UCHAR*)sday, 0, 0);
				voldate.db_datecnvGen(0, (UCHAR*)seEday, &eday_gengo, (UCHAR*)eday, 0, 0);

				memmove( xdata.data_day, eday, 3 );
//				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 0, eday[0] );
//				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 1, eday[1] );
//				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 2, eday[2] );
				m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, -1, eday[0], eday[1], eday[2], eday_gengo );
			}
			else{
				memset( yymmdd, '\0', sizeof(yymmdd) );
				m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
				if(  yymmdd[0] || yymmdd[1] || yymmdd[2] ){
					sw2 = 0x01;
				}
				m_pSnChskData->GetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, &xdata.data_day[0], &xdata.data_day[1], &xdata.data_day[2] );
				xpos = CHUZKITA;
			}
		}
		else if( nChar == VK_DELETE ){
			flg = REPLY_OK;
			memset( xdata.data_day, '\0', 3 );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 0, xdata.data_day[0] );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 1, xdata.data_day[1] );
			m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO, 2, xdata.data_day[2] );
		}
		break;
	case CHUSNMD:		// 修正・更正・決定の年月日
		if( nChar == VK_RETURN ){
//			m_pSnChskData->SetYmdDataGen( ID_SYUZEI_DECIDE_DAY, 0, xdata.data_day[0] );
//			m_pSnChskData->SetYmdDataGen( ID_SYUZEI_DECIDE_DAY, 1, xdata.data_day[1] );
//			m_pSnChskData->SetYmdDataGen( ID_SYUZEI_DECIDE_DAY, 2, xdata.data_day[2] );
			memmove(tmpday, xdata.data_day, sizeof(xdata.data_day));

			m_Util.CnvNoGengoToGengoWareki((UCHAR*)tmpday, &tmpGengo, (UCHAR*)xdata.data_day);
			if( tmpGengo ){
				m_pSnChskData->SetYmdDataGen(ID_SYUZEI_DECIDE_DAY, -1, xdata.data_day[0], xdata.data_day[1], xdata.data_day[2], 0);
			}
			else{
				if( isReiwaPrnt ){
					memset(xdata.data_day, '\0', sizeof(xdata.data_day));
					m_pSnChskData->SetYmdDataGen(ID_SYUZEI_DECIDE_DAY, -1, xdata.data_day[0], xdata.data_day[1], xdata.data_day[2], 0);
				}
				else{
					m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &xdata.data_day[0], &xdata.data_day[1], &xdata.data_day[2]);
				}
			}
		}
		else if( nChar == VK_DELETE ){
			flg = REPLY_OK;
			memset( xdata.data_day, '\0', 3 );
			m_pSnChskData->SetYmdDataGen( ID_SYUZEI_DECIDE_DAY, 0, xdata.data_day[0] );
			m_pSnChskData->SetYmdDataGen( ID_SYUZEI_DECIDE_DAY, 1, xdata.data_day[1] );
			m_pSnChskData->SetYmdDataGen( ID_SYUZEI_DECIDE_DAY, 2, xdata.data_day[2] );
		}
		break;
	case CHUZZEI:		// 前課税期間の消費税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnHonpyoData->Sn_ZNOFZ, val, 6 );
				st = 0x01;
			}
		}
		break;
	case CHUTU1:		// 月数換算 分子
		if( nChar == VK_RETURN ){
			memmove( dbuf, buff, len );
			m_pSnChskData->Sn_BSMON = (unsigned char)atoi( dbuf );
			st = 0x01;
		}
		else if( nChar == VK_DELETE ) {
			flg = REPLY_DELT;
			m_pSnChskData->Sn_BSMON = 0;
			xdata.data_edit.Empty();
			st = 0x01;
		}
		break;
	case CHUTU2:		// 月数換算 分母
		if( nChar == VK_RETURN ){
			memmove( dbuf, buff, len );
			tuki = (unsigned char)atoi( dbuf );
			if(( tuki >= 1 )&&( tuki <= 12 )){
				m_pSnChskData->Sn_BBMON = tuki;
			}
			else{
				wsprintf( dbuf, "%2d", m_pSnChskData->Sn_BBMON );
				xdata.data_edit = dbuf;
				sw = 0x01;
			}
			st = 0x01;
		}
		else if( nChar == VK_DELETE ) {
			flg = REPLY_DELT;
			m_pSnChskData->Sn_BBMON = 0;
			xdata.data_edit.Empty();
			st = 0x01;
		}
		break;
	case CHUNZEI:	// 納付すべき消費税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnHonpyoData->Sn_EDNOFZ, val, 6 );
				disp_clr( 1, index );
				st = 0x02;
			}
		}
		break;
	case CHUNTZEI:	// 納付すべき地方消費税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnHonpyoData->Sn_TEDNOF, val, 6 );
				disp_clr( 1, index );
				st = 0x03;
			}
		}
		break;
	case CHUZEIGOU:	// 合計納付税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( dbuf, buff, len );
				if( strchr( dbuf, '*' ) == NULL ){
					memmove( m_pSnChskData->Sn_KTNFZ, val, 6 );
					disp_clr( 1, index );
				}
			}
			else	sw = 0x10;
		}
		break;
	case CHUNOU1:	// 納税地　上段
		if( nChar == VK_RETURN ){
			memset( Str, '\0', sizeof( Str ) );
			Length = xdata.data_edit.GetLength();
			memmove( Str, xdata.data_edit, Length );
			memmove( m_pSnHeadData->Sn_NOZEI, Str, 40 );
			// 納税地
			int x = 11;
			m_Util.DiagOcxIniz( &xdata );	 // 表形式 初期化	
			memset( dbuf, '\0', sizeof( dbuf ));
			memmove( dbuf, m_pSnHeadData->Sn_NOZEI, 40 );
			xdata.data_edit = dbuf;
			diag_setdata( IDC_ICSDIAGCTRL1, x, &xdata, CIcsdiagctrl );
		}
		if( nChar == VK_HOME ) {
			ZipBf = m_pSnHeadData->Sn_ZIP;
			if( ZipToAddress( ZipBf, AddBf ) == TRUE )	{
				memset( Str, '\0', sizeof( Str ) );
				Length = AddBf.GetLength();
				memmove( Str, AddBf, Length );
				memmove( m_pSnHeadData->Sn_NOZEI, Str, 40 );
				// 納税地
				int x = CHUNOU1;
				m_Util.DiagOcxIniz( &xdata );	 // 表形式 初期化	
				memset( dbuf, '\0', sizeof( dbuf ));
				memmove( dbuf, m_pSnHeadData->Sn_NOZEI, 40 );
				xdata.data_edit = dbuf;
				diag_setdata( IDC_ICSDIAGCTRL1, x, &xdata, CIcsdiagctrl );
				xpos = x;
			}
		}
		break;
	case CHUNOU2:	// 納税地　下段
		if( nChar == VK_RETURN ){
			memset( Str, '\0', sizeof( Str ) );
			Length = xdata.data_edit.GetLength();
			memmove( Str, xdata.data_edit, Length );
			memmove( &m_pSnHeadData->Sn_NOZEI[40], Str, 40 );
			// 納税地
			int x = CHUNOU2;
			m_Util.DiagOcxIniz( &xdata );	 // 表形式 初期化	
			memset( dbuf, '\0', sizeof( dbuf ));
			memmove( dbuf, &m_pSnHeadData->Sn_NOZEI[40], 40 );
			xdata.data_edit = dbuf;
			diag_setdata( IDC_ICSDIAGCTRL1, x, &xdata, CIcsdiagctrl );
		}
		if( nChar == VK_HOME ){
			ZipBf =  m_pSnHeadData->Sn_ZIP;
			if( ZipToAddress( ZipBf, AddBf ) == TRUE )	{
				memset( Str, '\0', sizeof( Str ) );
				Length = AddBf.GetLength();
				memmove( Str, AddBf, Length );
				memmove( &m_pSnHeadData->Sn_NOZEI[40], Str, 40 );
				// 納税地
				int x = CHUNOU2;
				m_Util.DiagOcxIniz( &xdata );	 // 表形式 初期化	
				memset( dbuf, '\0', sizeof( dbuf ));
				memmove( dbuf, &m_pSnHeadData->Sn_NOZEI[40], 40 );
				xdata.data_edit = dbuf;
				diag_setdata( IDC_ICSDIAGCTRL1, x, &xdata, CIcsdiagctrl );
				xpos = x;
			}
		}
		break;
	case CHUTEL1:	// 電話番号　市外
		if( nChar == VK_RETURN ){
			Length = xdata.data_edit.GetLength();
			memset( Str, '\0', sizeof( Str ) );
			memmove( Str, xdata.data_edit, Length );
			memmove( m_pSnHeadData->Sn_TELNO1, Str, 6 );
		}
		break;
	case CHUTEL2:	// 電話番号　市内
		if( nChar == VK_RETURN ){
			Length = xdata.data_edit.GetLength();
			memset( Str, '\0', sizeof( Str ) );
			memmove( Str, xdata.data_edit, Length );
			memmove( m_pSnHeadData->Sn_TELNO2, Str, 4 );
		}
		break;
	case CHUTEL3:	// 電話番号　個人
		if( nChar == VK_RETURN ){
			Length = xdata.data_edit.GetLength();
			memset( Str, '\0', sizeof( Str ) );
			memmove( Str, xdata.data_edit, Length );
			memmove( m_pSnHeadData->Sn_TELNO3, Str, 4 );
		}
		break;
	case CHUMEIFRI:	// 名称又は屋号フリガナ
		if( nChar == VK_RETURN ){
//2016.10.20 UPDATE START
//			CopyFuriganaEX(m_pSnHeadData->Sn_COFRI,xdata.data_edit);
			CopyFuriganaEX2(m_pSnHeadData->Sn_COFRI,xdata.data_edit);
//2016.10.20 UPDATE END
		}
		break;
	case CHUMEI:	// 名称又は屋号
		if( nChar == VK_RETURN ){
			Length = xdata.data_edit.GetLength();
			memset( Str, '\0', sizeof( Str ) );
			memmove( Str, xdata.data_edit, Length );
			memmove( m_pSnHeadData->Sn_CONAM, Str, 40 );
		}
		break;

	//---->個人番号処理追加する
	case CHUKNO:	// 個人番号
		break;
	//<------------------------

	case CHUDAIFRI:	// 代表者氏名又は氏名フリガナ
		if( nChar == VK_RETURN ){
			CopyFuriganaEX(m_pSnHeadData->Sn_DIFRI,xdata.data_edit);
		}
		break;
	case CHUDAI:	// 代表者氏名又は氏名
		if( nChar == VK_RETURN ){
			Length = xdata.data_edit.GetLength();
			memset( Str, '\0', sizeof( Str ) );
			memmove( Str, xdata.data_edit, Length );
			memmove( m_pSnHeadData->Sn_DIHYO, Str, 40 );
		}
		break;
	case CHUZEINEN:	// 提出年月日
		if( nChar == VK_RETURN ){
			if( (xdata.data_day[0]&0xff) == 0xff ){
				memset( xdata.data_day, '\0', 3 );
			}
//			m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 0, xdata.data_day[0] );
//			m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 1, xdata.data_day[1] );
//			m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 2, xdata.data_day[2] );
			if( isReiwaPrnt && (m_Util.IsValidReiwaYmd(setYmdGengo, xdata.data_day[0], xdata.data_day[1], xdata.data_day[2])==false) ){
				memset(xdata.data_day,'\0',sizeof(xdata.data_day));
			}
			m_pSnHeadData->SetYmdDataGen(ID_ICSSH_PRESENT_DATE, -1, xdata.data_day[0], xdata.data_day[1], xdata.data_day[2], setYmdGengo );
			m_pSnHeadData->GetYmdDataGen( ID_ICSSH_PRESENT_DATE, &xdata.data_day[0], &xdata.data_day[1], &xdata.data_day[2] );
		}
		else if( nChar == VK_DELETE ) {
			flg = REPLY_OK;
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 0, yymmdd[0] );
			m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 1, yymmdd[1] );
			m_pSnHeadData->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 2, yymmdd[2] );
	 		diag_clear( IDC_ICSDIAGCTRL1, index, TRUE, CIcsdiagctrl );	
		}
		break;
	case CHUZEISYO:	// 税務署名
		 if( nChar == VK_HOME ) {
			if( *(m_pShinInfo->pIsOpenEselect) == TRUE ){
				memset( m_pShinInfo->pEselect, '\0', sizeof(ESELECT) );
				m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
				if( (m_pSnHeadData->Sn_TAXNO[0]&0xff) != 0x00 && (m_pSnHeadData->Sn_TAXNO[0]&0xff) != 'F' ){
					memmove( m_pShinInfo->pEselect->cd_office, m_pSnHeadData->Sn_TAXNO, 5 );
				}
				m_Util.DiagOcxIniz(&xdata);
				m_ChskDiag.GetData( index, (struct IUnknown *)&xdata );
				len = (int)xdata.data_edit.GetLength();
				memset( m_pSnHeadData->Sn_ZMSYO, '\0', sizeof(m_pSnHeadData->Sn_ZMSYO) );
				memmove( m_pSnHeadData->Sn_ZMSYO, xdata.data_edit, len );
				if( Esel_SelectTaxOffice( m_pShinInfo->pEselect ) == IDOK )	{
					memmove( m_pSnHeadData->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, 5 );
					memset( Str, '\0', sizeof( Str ) );
					kspclr( Str, 6 );
					Length = (int)strlen( m_pShinInfo->pEselect->nm_office );
					memmove( Str, m_pShinInfo->pEselect->nm_office, Length );
					memmove( m_pSnHeadData->Sn_ZMSYO, Str, 12 );
					// 税務署名
					memset( Str, '\0', sizeof( Str ) );
					Length = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_ZMSYO, 12 );
					memmove( Str, m_pSnHeadData->Sn_ZMSYO, Length );
					index = CHUZEISYO;
					diag_clear( IDC_ICSDIAGCTRL1, index, TRUE, CIcsdiagctrl );
					m_Util.DiagOcxIniz( &xdata );
					xdata.data_edit = Str;
					m_ChskDiag.OffControl( TRUE );
					diag_setdata( IDC_ICSDIAGCTRL1, index, &xdata, CIcsdiagctrl );
					m_ChskDiag.OffControl( FALSE );
					index = CHUSYONO;
					diag_clear( IDC_ICSDIAGCTRL1, index, TRUE, CIcsdiagctrl );
					memset( Str, '\0', sizeof( Str ) );
					sprintf_s( Str, sizeof( Str ), "署番号 %-5.5s", m_pSnHeadData->Sn_TAXNO );
					xdata.data_disp = Str;
					diag_setdata( IDC_ICSDIAGCTRL1, index, &xdata, CIcsdiagctrl );
					xpos = CHUSEIRI;
				}
				else{
					diag_setdata( IDC_ICSDIAGCTRL1, index, &xdata, CIcsdiagctrl );
					xpos = CHUZEISYO;
				}
			}
		}
		break;

	// 修正申告
	case CHUSYUSZEI1:		// 消費税　この申告前の税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnChskData->Sn_ZSKZN, val, 6 );
				st = 0x10;
			}
		}
		break;
	case CHUSYUSZEI2:		// 消費税　この申告により増加する税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnChskData->Sn_ZSKAD, val, 6 );
				st = 0x10;
			}
		}
		break;
	case CHUSYUTZEI1:		// 地方消費税　この申告前の税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnChskData->Sn_ZSTZN, val, 6 );
				st = 0x10;
			}
		}
		break;
	case CHUSYUTZEI2:		// 地方消費税　この申告により増加する税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( m_pSnChskData->Sn_ZSTAD, val, 6 );
				st = 0x10;
			}
		}
		break;
	case CHUSYUKEI:	// 合計納付税額
		if( nChar == VK_RETURN ){
			if( length ){
				memmove( dbuf, buff, len );
				if( strchr( dbuf, '*' ) == NULL ){
					memmove( m_pSnChskData->Sn_ZSKTZ, val, 6 );
					disp_clr( 1, index );
				}
			}
			else	sw = 0x10;
		}
		break;
	}

	if( nChar == VK_RETURN || ( nChar == VK_F3 ) ){
		flg = REPLY_OK;
		if( index == CHUZEISYO ){
			xpos = CHUSEIRI;
		}
		else{
			if( !sw2 )	flg |= REPLY_NEXT;
		}
	}
	else if(( nChar == VK_LEFT ) || ( nChar == VK_UP ) || ( nChar == VK_F2 ) || ( wTerm == VK_TAB ) ){
		if( index == CHUSEIRI )	xpos = CHUZEISYO;
		else	flg = REPLY_BACK;
	}
	else if(( nChar == VK_RIGHT ) || ( nChar == VK_DOWN ) || ( nChar == VK_TAB ))
		flg = REPLY_NEXT;

	if(( sw&0x0f ) == 0x01 )	flg = REPLY_OK;

	if(( sw&0xf0 ) == 0x00 ){
		if( flg&REPLY_OK )
			diag_setdata( IDC_ICSDIAGCTRL1, index, (LPUNKNOWN)&xdata, CIcsdiagctrl );
	}

	SetCalq( st );

	if( flg & REPLY_NEXT ){
		diag_setnext( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	}
	else if( flg & REPLY_BACK ){
		diag_setprev( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	}
	else if( flg & REPLY_DELT ){
		diag_clear( IDC_ICSDIAGCTRL1, index, TRUE, CIcsdiagctrl );	
	}
	else{
		diag_setposition( IDC_ICSDIAGCTRL1, xpos, CIcsdiagctrl );
	}
}


//////////////////////////////////////////////////////////////////////////
//						 期間チェック関数								//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------
	決算期間 チェック
		int  ymd_chk(BYTE* sp, BYTE* ep)
		int  Date_KymdCheck(BYTE* sp, BYTE* ep)
			unsigned char	*sp;	自決算日 buffa pointer
			unsigned char	*ep;	至決算日 buffa pointer
 --------------------------------------------------------------------
		返送値：	 0 ..... ＯＫ（１年間）
					 1 ..... ＯＫ（１年未満）
					-1 ..... 期間エラー
 ---------------------------------------------------------------------*/
static
int Date_KymdCheck(BYTE* sp, BYTE* ep)
{
BYTE	symd[4], eymd[4], tymd[4];
int		sg;

	/*--- 日付 チェック ---*/
	if (Date_DateCheck( sp+1 ) != 0)  return -1;
	if (Date_DateCheck( ep+1 ) != 0)  return -1;

	memmove( symd, sp, 3 );		/*-- 自決算日 セーブ --*/
	memmove( eymd, ep, 3 );		/*-- 至決算日 セーブ --*/
	Date_GetEymd( tymd, symd );		/*-- 計算上の至決算日 --*/

	if (eymd[0] < symd[0])  {	/*-- 00年 < 99年 --*/
		if (eymd[0] == 0)  eymd[0] = 0xa0;
	}
	if (tymd[0] < symd[0])  {	/*-- 00年 < 99年 --*/
		if (tymd[0] == 0)  tymd[0] = 0xa0;
	}

	/*--- 期間 チェック ---*/
	if (val_cmp( symd, eymd, 3 ) > 0)  return -1;	/*-- 自 ＞ 至 --*/
	switch ( val_cmp( eymd, tymd, 3 ) )  {
		case  -1:	/*-- １年未満 --*/
					sg = 1;
					break;
		case   0:	/*-- １年間 --*/
					sg = 0;
					break;
		default :	/*-- 期間エラー --*/
					sg = -1;
					break;
	}
	return sg;
}

/*---------------------------------------------------------------------
	期間(BCD:３桁) 比較
		int  ymd_comp( ymd1, ymd2 )
		int  Date_YMDcomp(BYTE* ymd1, BYTE* ymd2)
			unsigned char	*ymd1;	期間１ buffa pointer
			unsigned char	*ymd2;	期間２ buffa pointer
	-----------------------------------------------------------------
		返送値：	-1: ymd1 < ymd2
					 0: ymd1 = ymd2
					 1: ymd1 > ymd2
  ---------------------------------------------------------------------*/
static
int Date_YMDcomp(BYTE* ymd1, BYTE* ymd2)
{
BYTE	*bp, *sp, ymd_1[4], ymd_2[4];

	ymd_1[0] = ymd_2[0] = 0x19;		//-- 1900年
	memmove( &ymd_1[1], ymd1, 3 );
	memmove( &ymd_2[1], ymd2, 3 );
	if (*ymd1 != *ymd2)  {
		if (*ymd1 > *ymd2)  {
			bp = (BYTE*)ymd_1;
			sp = (BYTE*)ymd_2;
		}
		else  {
			bp = (BYTE*)ymd_2;
			sp = (BYTE*)ymd_1;
		}
		if ((*(bp+1) - *(sp+1)) > 0x50)  {
			*sp = Date_CalcBCD( *sp, 0x01, 0 );		/* ＋１年 */
		}
	}
	return( val_cmp( ymd_1, ymd_2, 4 ) );
}

/*---------------------------------------------------------------------
	至決算年月日 ---> 自決算年月日
		void  Date_GetSymd(BYTE* sp, BYTE* ep)
		void  symd_get( sp, ep )
			unsigned char	*sp;	自決算日 buffa pointer
			unsigned char	*ep;	至決算日 buffa pointer
  ---------------------------------------------------------------------*/
static
void Date_GetSymd(BYTE* sp, BYTE* ep)
{
BYTE	tmp;

	/*-- 末日 ｇｅｔ --*/
	tmp = Date_GetEndDay( *(ep+1) );
	/*---------------- 閏年 チェック カット -----------------
	if (*(ep+1) == 0x02)  {
		if (isleap( *ep ))  tmp = bcd_clc( tmp, 0x01, 0 );
	}
	  -------------------------------------------------------*/
	
	if (*(ep+2) >= tmp)  {		/*-- 至日＝ 末日 --*/
		if (*(ep+1) == 0x12)  {
			*sp = *ep;
			*(sp+1) = 0x01;
		}
		else  {
			*sp = Date_CalcBCD( *ep, 0x01, 1 );		/* －１年 */
			*(sp+1) = Date_CalcBCD( *(ep+1), 0x01, 0 );		/* ＋１月 */
		}
		*(sp+2) = 0x01;
	}
	else  {
		*sp = Date_CalcBCD( *ep, 0x01, 1 );		/* －１年 */
		*(sp+1) = *(ep+1);
		*(sp+2) = Date_CalcBCD( *(ep+2), 0x01, 0 );		/* ＋１日 */
	}
	/*-- 00年 <--> 99年 --*/
	if ((*sp & 0xf0) == 0xf0)  *sp = (*sp & 0x0f) | 0x90;
}

/*---------------------------------------------------------------------
	自決算年月日 ---> 至決算年月日
		void  Date_GetEymd(BYTE* ep, BYTE* sp)
		void  eymd_get( ep, sp )
			unsigned char	*ep;	至決算日 buffa pointer
			unsigned char	*sp;	自決算日 buffa pointer
  ---------------------------------------------------------------------*/
static
void Date_GetEymd(BYTE* ep, BYTE* sp)
{
	if (*(sp+2) == 0x01)  {		/*-- 開始日＝ １日 --*/
		if (*(sp+1) == 0x01)  {
			*ep = *sp;
			*(ep+1) = 0x12;
		}
		else  {
			*ep = Date_CalcBCD( *sp, 0x01, 0 );		/* ＋１年 */
			*(ep+1) = Date_CalcBCD( *(sp+1), 0x01, 1 );		/* －１月 */
		}
		*(ep+2) = Date_GetEndDay( *(ep+1) );
		/*-- 閏年 チェック & セット --*/
		Date_SetFebEday( ep );
	}
	else  {
		*ep = Date_CalcBCD( *sp, 0x01, 0 );		/* ＋１年 */
		*(ep+1) = *(sp+1);
		*(ep+2) = Date_CalcBCD( *(sp+2), 0x01, 1 );		/* －１日 */
	}
}

/*---------------------------------------------------------------------
	２月末日 セット ( xx.02.28 <--> xx.02.29 )
		void  Date_SetFebEday(BYTE* ymd)
		void  feb_date( ymd )
			unsigned char	*ymd;	年月日(BCD) buffa pointer
  ---------------------------------------------------------------------*/
static
void Date_SetFebEday(BYTE* ymd)
{
BYTE	day;

	if (*(ymd+1) == 0x02)  {
		day = Date_GetEndDay( *(ymd+1) );		/* ２月末日 */
		if (*(ymd+2) >= day)  {
			if (Date_IsLeap( *ymd ))  day = Date_CalcBCD( day, 0x01, 0 );	/* ＋１日 */
			*(ymd+2) = day;
		}
	}
}

/*---------------------------------------------------------------------
	日付 チェック
		int  Date_DateCheck(BYTE* date)
		int  date_chk( date )
			unsigned char	*date;	日付（月日） buffa pointer
	-----------------------------------------------------------------
		返送値：	 0 ..... ｏｋ
					-1 ..... エラー
  ---------------------------------------------------------------------*/
static
int Date_DateCheck(BYTE* date)
{
BYTE	mm, dd, eday;

	mm = *date;
	dd = *(date+1);
	if ((mm >= 0x01) && (mm <= 0x12) && ((mm & 0x0f) <= 0x09))  {
		eday = Date_GetEndDay( mm );	/*-- 月末日 --*/
		if (mm == 0x02)
			eday = Date_CalcBCD( eday, 0x01, 0 );	/* 閏年： ＋１日 */
		if ((dd >= 0x01) && (dd <= eday) && ((dd & 0x0f) <= 0x09))
			return 0;
	}
	return -1;
}

/*---------------------------------------------------------------------
	日付 期間内チェック
		int  Date_DtrmCheck(BYTE* smd, BYTE* emd, BYTE* dat)
		int  dtrm_chk( smd, emd, dat )
			unsigned char	*smd;	開始日付（月日） buffa pointer
			unsigned char	*emd;	最終日付（月日） buffa pointer
			unsigned char	*dat;	日付（月日） buffa pointer
	-----------------------------------------------------------------
		返送値：	 0 ..... ｏｋ
					-1 ..... エラー
  ---------------------------------------------------------------------*/
static
int Date_DtrmCheck(BYTE* smd, BYTE* emd, BYTE* dat)
{
BYTE	tbuf[4], tbf2[4];

	if (Date_DateCheck( dat ) == 0)  {
		if (val_cmp( smd, emd, 2 ) > 0)  {
			memmove( tbuf, emd, 2 );
			tbuf[0] |= 0x20;
			emd = (BYTE*)tbuf;
			if (val_cmp( smd, dat, 2 ) > 0)  {
				memmove( tbf2, dat, 2 );
				tbf2[0] |= 0x20;
				dat = (BYTE*)tbf2;
			}
		}
		if ((val_cmp( dat, smd, 2 ) >= 0) && (val_cmp( dat, emd, 2 ) <= 0))
			return 0;
	}
	return -1;
}

/*---------------------------------------------------------------------
	翌日付 セット
		int  Date_SetNextDate(BYTE* ndp, BYTE* sdp)
		int  next_date( ndp, sdp )
			unsigned char	*ndp;	翌日付（月日） buffa pointer
			unsigned char	*sdp;	該当日付（月日） buffa pointer
	-----------------------------------------------------------------
		返送値：	 0 ..... ｏｋ
					-1 ..... エラー
  ---------------------------------------------------------------------*/
static
int Date_SetNextDate(BYTE* ndp, BYTE* sdp)
{
	if (ndp != sdp)  memmove( ndp, sdp, 2 );
	if (Date_DateCheck( ndp ) == 0)  {
		*(ndp+1) = Date_CalcBCD( *(ndp+1), 0x01, 0 );		/* ＋１日 */
		if (*(ndp+1) > Date_GetEndDay( *ndp ))  {
			*(ndp+1) = 0x01;
			*ndp = Date_CalcBCD( *ndp, 0x01, 0 );		/* ＋１月 */
			if (*ndp > 0x12)  *ndp = 0x01;
		}
		return 0;
	}
	return -1;
}

/*---------------------------------------------------------------------
	前日付 セット
		int  Date_SetPrevDate(BYTE* pdp, BYTE* sdp)
		int  prev_date( pdp, sdp )
			unsigned char	*pdp;	前日付（月日） buffa pointer
			unsigned char	*sdp;	該当日付（月日） buffa pointer
	-----------------------------------------------------------------
		返送値：	 0 ..... ｏｋ
					-1 ..... エラー
  ---------------------------------------------------------------------*/
static
int Date_SetPrevDate(BYTE* pdp, BYTE* sdp)
{
	if (pdp != sdp)  memmove( pdp, sdp, 2 );
	if (Date_DateCheck( pdp ) == 0)  {
		*(pdp+1) = Date_CalcBCD( *(pdp+1), 0x01, 1 );		/* －１日 */
		if (*(pdp+1) == 0)  {
			*pdp = Date_CalcBCD( *pdp, 0x01, 1 );		/* －１月 */
			if (*pdp == 0)  *pdp = 0x12;
			*(pdp+1) = Date_GetEndDay( *pdp );
		}
		return 0;
	}
	return -1;
}

/*---------------------------------------------------------------------
	該当月末日 ｇｅｔ
		BYTE  Date_GetEndDay(BYTE mth)
		unsigned char  end_day( mth )
				char	mth;	指定月 (BCD)
	-----------------------------------------------------------------
		返送値：	末日
  ---------------------------------------------------------------------*/
static
BYTE Date_GetEndDay(BYTE mth)
{
BYTE	day;

	switch (mth)  {
		case  0x01:
		case  0x03:
		case  0x05:
		case  0x07:
		case  0x08:
		case  0x10:
		case  0x12:	day = 0x31;
					break;
		case  0x04:
		case  0x06:
		case  0x09:
		case  0x11:	day = 0x30;
					break;
		case  0x02:	day = 0x28;
					break;
	}
	return day;
}

/*---------------------------------------------------------------------
	閏年 チェック
		int  Date_IsLeap(BYTE year)
		int  isleap( year )
			unsigned char	year;	（西暦）年
	-----------------------------------------------------------------
		返送値：	1 ..... 閏年
					0 ..... 
  ---------------------------------------------------------------------*/
static
int Date_IsLeap(BYTE year)
{
char	buf[10];
int		sg, ad_year;

	sprintf_s( buf, sizeof( buf ), "%02x", year );
	ad_year = atoi(buf);
	if (ad_year >= 80)  ad_year += 1900;
	else  ad_year += 2000;
	sg = ((!(ad_year % 4) && (ad_year % 100)) || !(ad_year % 400)) ? 1 : 0;
	return sg;
}

/*---------------------------------------------------------------------
	ＢＣＤ計算
		BYTE  Date_CalcBCD(BYTE val, BYTE dat, BYTE sg)
		unsigned char  bcd_clc( val, dat, sg )
			unsigned char	val;	計算元データ
			unsigned char	dat;	被計算値
			unsigned char	 sg;	0: 加算 , 1: 減算
	-----------------------------------------------------------------
		返送値：	計算結果
  ---------------------------------------------------------------------*/
static
BYTE Date_CalcBCD(BYTE val, BYTE dat, BYTE sg)
{
BYTE	tmp;

	if (sg == 0)  {
		tmp = (val & 0x0f) + (dat & 0x0f);
		val = (val & 0xf0) + (dat & 0xf0);
		if (tmp > 0x09)  {
			val += 0x10;
			tmp -= 0x0a;
		}
		if (val > 0x90)  val -= 0xa0;
		val += tmp;
	}
	else  {
		if ((val & 0x0f) < (dat & 0x0f))  {
			tmp = (val & 0x0f) + 0x0a;
			val = (val & 0xf0) - 0x10;
			val = (val - (dat & 0xf0)) + (tmp - (dat & 0x0f));
		}
		else  {
			tmp = (val & 0x0f) - (dat & 0x0f);
			val = (val & 0xf0) - (dat & 0xf0) + tmp;
		}
	}
	return val;
}

/*---------------------------------------------------------------------
	データ比較
		int  val_cmp( val1, val2, len )
			unsigned char	*val1;	データ１ buffa pointer
			unsigned char	*val2;	データ２ buffa pointer
			int				  len;	比較レングス
	-----------------------------------------------------------------
		返送値：	-1 ..... データ１ ＜ データ２
					 0 ..... データ１ ＝ データ２
					 1 ..... データ１ ＞ データ２
  ---------------------------------------------------------------------*/
static
int  val_cmp(BYTE* val1, BYTE* val2, int len)
{
int		i, sg;

	sg = 0;
	for ( i = 0; i < len; ++i )  {
		if ((*(val1+i) & 0xff) > (*(val2+i) & 0xff))  {
			sg = 1;  break;
		}
		else if ((*(val1+i) & 0xff) < (*(val2+i) & 0xff))  {
			sg = -1;  break;
		}
	}
	return sg;
}



int CShinChskEX::_CheckYYMMDD( char *sday, char *eday, char sw )
{
	int flg = -1;

	if( sw == 0 ){
		flg = Date_DateCheck(( BYTE* )&sday[1] );
	}
	else{
		if(( !eday[0] ) && ( !eday[1] ) && ( !eday[2] )){
			Date_GetEymd(( BYTE* )eday, ( BYTE* )sday );
		}
		if(( flg = Date_DateCheck(( BYTE* )&eday[1] )) == 0 ){
			flg = -1;
			if( Date_KymdCheck(( BYTE* )sday, ( BYTE* )eday ) >= 0 ){
				if(( eday[1] == 0x2 ) && ( eday[2] >= 0x28 )){
					BYTE	ymd[4];
					memmove( ymd, eday, 3 );
					shin_datecnv( ymd[0], ymd, 1 );
					Date_SetFebEday( ymd );
					eday[2] = ymd[2];
				}
				flg =0;
			}
		}
	}
	return flg;
};

/*---------------------------------------------------------------------
		平成～西暦　変換
  ---------------------------------------------------------------------
	shin_datecnv( year, date, sign ) 
	char	year;	年  変換前
	char	*date;  年　変換後
	char	sign;   変換方向
		Status	 0 = OK!
				-1 = ERROR
  ---------------------------------------------------------------------*/
int CShinChskEX::shin_datecnv( unsigned char year, unsigned char *date, int sign )
{
	unsigned char	ymd[4], dchk[10];
	int				yy, y1;

	sprintf_s( (char *)dchk,  sizeof( dchk ),"%02x", year );
	y1 = atoi( (char *)dchk );
	if( (y1<0) || (y1>99) ){
		return -1;
	}

	if( sign ){	/*  平成→西暦  */
		if( y1 >= 12 ){
			yy = y1 - 12;	/*  西暦２０００年以上  */
		}
		else{
			yy = y1 + 88;	/*  　　　　〃　　未満  */
		}
		sprintf_s( (char *)dchk, sizeof( dchk ), "%02d",yy);
		m_pArith->l_pack( ymd, dchk, 2 );
		*date = ymd[0];
	}
	else{		/*  西暦→平成  */
		if( y1 < 89 ){
			yy = y1 + 12;	/*  西暦２０００年以上  */
		}
		else{
			yy = y1 - 88;	/*  　　　　〃　　未満  */
		}
		sprintf_s( (char *)dchk, sizeof( dchk ), "%02d",yy);
		m_pArith->l_pack( ymd, dchk, 2 );
		*date = ymd[0];
	}

	return 0;
}


BOOL CShinChskEX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN )	{
		if ( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR )	{
		}
		if( pMsg->wParam == VK_END ){
			// フォーカス移動
			MoveFocusInEnd();

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
		
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
					TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
		
		if( pMsg->wParam == VK_ESCAPE ) {
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F5 );
			}
			return TRUE;
		}
		if( pMsg->wParam == VK_F4 ) {
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F4 );
			}
			return TRUE;
		}
		if( pMsg->wParam == VK_F9 ) {
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F9 );
			}
			return TRUE;
		}
		if( pMsg->wParam == VK_F11 ) {
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F11 );
			}
			return TRUE;
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// ポジションセット
//-----------------------------------------------------------------------------
void CShinChskEX::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndexEX;
	}

	m_ChskDiag.SetFocus();

	// 確定時は全入力不可
	if( (m_pSnHeadData->Sn_Sign4&0x01) ){
		return;
	}

	m_ChskDiag.SetPosition( Index );

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );
}


HBRUSH CShinChskEX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	if( nCtlColor == CTLCOLOR_DLG )	hbr = Ibrsh_ChskEX;

	return hbr;
}


//-----------------------------------------------------------------------------
// 税務署入力にフォーカスセット('15.02.03)
//-----------------------------------------------------------------------------
void CShinChskEX::SetTaxOfficeInput()
{
	if( m_pSnHeadData->Sn_Sign4&0x01 ){
		return;
	}

	m_ChskDiag.SetPosition( CHUZEISYO );
}

//-----------------------------------------------------------------------------
// 終了時のフォーカス移動 ('15.03.02)
//-----------------------------------------------------------------------------
void CShinChskEX::MoveFocusInEnd()
{
	// データを確定させる為にフォーカス移動
	if( m_curwnd == IDC_ICSDIAGCTRL1 ){
		if( m_ChskDiag.GetPosition() == CHUZEISYO ){
			m_ChskDiag.SetPosition( CHUSEIRI );
		}
	}
}

//-----------------------------------------------------------------------------
// 前課税期間の月数を計算('15.08.28)
//-----------------------------------------------------------------------------
void CShinChskEX::CalqMonthSpan()
{
	// デフォルトは12カ月
	m_pSnChskData->Sn_BBMON = 12;

	long sYmd = m_pSnChskData->Sn_ZKDYS;
	int sMd = sYmd % 10000;
	int sY = sYmd / 10000;	  // 年
	int sM = sMd / 100;		  // 月
	int sD = sMd % 100;		  // 日

	// 正しいデータ？
	if( (sM<1) || (12<sM) ){
		return;
	}
	if( (sD<1) || (31<sD) ){
		return;
	}

	long eYmd = m_pSnChskData->Sn_ZKDYE;
	int eMd = eYmd % 10000;
	int eY = eYmd / 10000;	  // 年
	int eM = eMd / 100;		  // 月
	int eD = eMd % 100;		  // 日

	// 正しいデータ？
	if( (eM<1) || (12<eM) ){
		return;
	}
	if( (eD<1) || (31<eD) ){
		return;
	}
//--> '15.12.10 INS START
	if( sYmd >= eYmd ){
		return;
	}
//<-- '15.12.10 INS END

	// 初期値
	int tmpY = sY,
		tmpM = sM,
		tmpD = sD;
	long tmpYmd = (tmpY*10000) + (tmpM*100) + tmpD;
	int	span = 0;
	for( int i=0; i<12; i++ ){
		// 翌月日計算
		if( tmpM == 12 ){
			tmpY += 1;
			tmpM = 1;
		}
		else{
			tmpM += 1;
		}
		tmpYmd = (tmpY*10000) + (tmpM*100) + tmpD;

		span++;
		if( eYmd <= tmpYmd ){
			break;
		}
	}
	// 12カ月を超える事は無いのでストッパ
	if( (0<span) && (span<=12) ){
		m_pSnChskData->Sn_BBMON = span;
	}
}


void CShinChskEX::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MoveDiagMNEdit();

	CSyzBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CShinChskEX::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MoveDiagMNEdit();

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CShinChskEX::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSyzBaseDlg::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	MoveDiagMNEdit();
}

void CShinChskEX::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	MoveDiagMNEdit();
}

void CShinChskEX::MoveDiagMNEdit()
{
	if( m_pParent && m_pParent->GetSafeHwnd() ){
		if( ((CH26HyoView*)m_pParent)->GetShRevType() >= ID_ICSSH_MYNUMBER_REVTYPE ){
			if(m_pSnHeadData->IsSoleProprietor() == TRUE ){ //個人
				m_ctrl_mynum.MoveDiagMNEdit(CHSKEX_NUM_KOJIN1,FALSE);
			}else{
				m_ctrl_mynum.MoveDiagMNEdit(CHSKEX_NUM_HOJIN1,FALSE);
			}
			if( m_pSnHeadData->Sn_Sign4&0x01 ){
				m_ctrl_mynum.SetDisableColor( BC_GRAY );
			}
			else{
				m_ctrl_mynum.SetDisableColor( BC_WHITE );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// マイナンバーコントロールの表示を消す
//-----------------------------------------------------------------------------
void CShinChskEX::HideMNEdit()
{
	m_edit_mynum1.EnableWindow( FALSE );
	m_edit_mynum2.EnableWindow( FALSE );
	m_edit_mynum3.EnableWindow( FALSE );
	m_edit_mynum4.EnableWindow( FALSE );
	m_edit_mynum1.ShowWindow( SW_HIDE );
	m_edit_mynum2.ShowWindow( SW_HIDE );
	m_edit_mynum3.ShowWindow( SW_HIDE );
	m_edit_mynum4.ShowWindow( SW_HIDE );
}
