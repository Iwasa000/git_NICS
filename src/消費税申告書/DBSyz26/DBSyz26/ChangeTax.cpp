// ChangeTax.cpp : é¿ëïÉtÉ@ÉCÉã
//

#include "stdafx.h"
#include "ChangeTax.h"
#include "ChangeTaxIdx.h"

// CChangeTax É_ÉCÉAÉçÉO

IMPLEMENT_DYNAMIC(CChangeTax, ICSDialog)

CChangeTax::CChangeTax(CWnd* pParent /*=NULL*/)
	: ICSDialog(CChangeTax::IDD, pParent)
	, m_pSnFhyo20Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pSnTanaData(NULL)
	, m_CbSw(0)
	, m_ChSw(0)
	, m_CbSt(0)
	, m_pArith(NULL)
{
	memset( MNK40, '\0', sizeof(MNK40) );
	memset( MNZ40, '\0', sizeof(MNZ40) );
	memset( MENZK, '\0', sizeof(MENZK) );
	memset( MENZZ, '\0', sizeof(MENZZ) );

	tansw = NULL;

	tmpChSw = 0;

}

CChangeTax::~CChangeTax()
{
}

void CChangeTax::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_ICSDIAG1);
	DDX_Control(pDX, IDC_STA1, m_txt1);
}


BEGIN_MESSAGE_MAP(CChangeTax, ICSDialog)
END_MESSAGE_MAP()


// CChangeTax ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ

BOOL CChangeTax::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	//	TODO:  Ç±Ç±Ç…èâä˙âªÇí«â¡ÇµÇƒÇ≠ÇæÇ≥Ç¢
	Init();

	ICSDialog::OnInitDialogEX();

	return FALSE;  // ÉRÉìÉgÉçÅ[ÉãÇ…ÉtÉHÅ[ÉJÉXÇê›íËÇµÇ»Ç¢Ç∆Ç´ÅAñﬂÇËílÇÕ TRUE Ç∆Ç»ÇËÇ‹Ç∑
	              // ó·äO: OCX ÉvÉçÉpÉeÉB ÉyÅ[ÉWÇÃñﬂÇËílÇÕ FALSE Ç∆Ç»ÇËÇ‹Ç∑
}

//-----------------------------------------------------------------------------
// èâä˙èÓïÒÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSnFhyo20Data	ÅF	ïtï\ÇQÉfÅ[É^ì«çûÇ›ÉNÉâÉX
//		pSnHeadData		ÅF	ñ{ï\ÉwÉbÉ_ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
//		pSnTanaData		ÅF	íIâµí≤êÆÉfÅ[É^ì«çûÇ›ÉNÉâÉX
//		pArith			ÅF	î{í∑ââéZÉNÉâÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0			ÅF	ê≥èÌèIóπ
//			-1			ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CChangeTax::InitInfo( CH26SnFhyo20Data *pSnFhyo20Data, CSnHeadData *pSnHeadData, CH26SnTanaData *pSnTanaData, CArithEx *pArith )
{
	ASSERT( pSnFhyo20Data );
	if( pSnFhyo20Data == NULL ){
		return -1;
	}
	m_pSnFhyo20Data = pSnFhyo20Data;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSnTanaData );
	if( pSnTanaData == NULL ){
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	return 0;
}

//-----------------------------------------------------------------------------
// èâä˙âª
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//			-1	ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CChangeTax::Init()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	char				buf[512] = {0};

	tansw = m_pSnTanaData->Sn_MENZsw;//[Y]

	switch( m_pSnTanaData->Sn_MENZsw & 0xff ){
		case 0x10 :	// ñ∆ê≈Ç©ÇÁâ€ê≈
			m_CbSw = 0;

			memset( buf, '\0', sizeof( buf ) );
			strcpy_s( buf, sizeof( buf ), _T("ìñä˙ÇÊÇËÅAñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃ\n") );
			strcat_s( buf, sizeof( buf ), _T("â€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz") );
			DiagData.data_disp = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			memset( buf, '\0', sizeof( buf ) );
			strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€\n") );
			strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
			DiagData.data_disp = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;

		case 0x11 : // â€ê≈Ç©ÇÁñ∆ê≈
			m_CbSw = 1;

			memset( buf, '\0', sizeof( buf ) );
			strcpy_s( buf, sizeof( buf ), _T("óÇä˙ÇÊÇËÅAâ€ê≈ã∆é“Ç©ÇÁñ∆ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃ\n") );
			strcat_s( buf, sizeof( buf ), _T("â€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz") );
			DiagData.data_disp = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			memset( buf, '\0', sizeof( buf ) );
			strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€äO\n") );
			strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
			DiagData.data_disp = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;

		default	  : // â€ê≈ï˚éÆêÿÇËë÷Ç¶âèú
			m_pSnTanaData->Sn_MENZsw = 0x00;
			m_CbSw = 2;

			diag_clear( IDC_ICSDIAGCTRL1, IDTITLE, TRUE, CIcsdiagctrl );

			memset( buf, '\0', sizeof( buf ) );
			strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€\n") );
			strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
			DiagData.data_disp = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			memset( m_pSnTanaData->Sn_MNK63, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
			memset( m_pSnTanaData->Sn_MNZ63, '\0', MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
			memset( m_pSnTanaData->Sn_MNK40, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
			memset( m_pSnTanaData->Sn_MNZ40, '\0', MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
			memset( m_pSnTanaData->Sn_MENZK, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)
			memset( m_pSnTanaData->Sn_MENZZ, '\0', MONY_BUF_SIZE );	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )

			break;
	}
	DiagData.data_combo = m_CbSw;
	diag_setdata( IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( MNK63, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
	memset( MNZ63, '\0', MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
	memset( MNK40, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
	memset( MNZ40, '\0', MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
	memset( MENZK, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)
	memset( MENZZ, '\0', MONY_BUF_SIZE );	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )

	memmove( MNK63, m_pSnTanaData->Sn_MNK63, MONY_BUF_SIZE );		// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)//à⁄ìÆ
	memmove( MNZ63, m_pSnTanaData->Sn_MNZ63, MONY_BUF_SIZE );		//				 ÅV	   è¡îÔê≈äz   (ÅV      )
	
	if( m_pSnHeadData->SVmzsw == 1 ){
		memmove( MNK40, m_pSnTanaData->Sn_MNK40, MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)//à⁄ìÆ
		memmove( MNZ40, m_pSnTanaData->Sn_MNZ40, MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
		memmove( MENZK, m_pSnTanaData->Sn_MENZK, MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)
		memmove( MENZZ, m_pSnTanaData->Sn_MENZZ, MONY_BUF_SIZE );	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
	}

	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
	memmove( DiagData.data_val, MNK63, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAGCTRL1, IDMNK63, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(ê≈ó¶6.3%ï™)
	memmove( DiagData.data_val, MNZ63, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAGCTRL1, IDMNZ63, (struct IUnknown *)&DiagData, CIcsdiagctrl );


	if( m_pSnHeadData->SVmzsw == 1 ){
		// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
		memmove( DiagData.data_val, MNK40, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMNK40, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		
		// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(ê≈ó¶4%ï™)
		memmove( DiagData.data_val, MNZ40, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMNZ40, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)
		memmove( DiagData.data_val, MENZK, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMENZK, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		
		// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(ãåê≈ó¶ï™)
		memmove( DiagData.data_val, MENZZ, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMENZZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	// çTèúëŒè€äOédì¸ê≈äz
	if( m_pSnHeadData->SVsign&0x10 ){	// ê≈çûÇ›
		m_ChSw = 1;
		tmpChSw = 1;//[Y]
		DiagData.data_check = 0;
		diag_setdata( IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		DiagData.data_check = 1;
		diag_setdata( IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
	else{						// ê≈î≤Ç´
		m_ChSw = 0;
		tmpChSw = 0;//[Y]
		DiagData.data_check = 1;
		diag_setdata( IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		DiagData.data_check = 0;
		diag_setdata( IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
	


	/*-----> ä÷êîâª

	int		flg = 0;
	short	index = 0;
	switch( m_pSnHeadData->Sn_Sign4&0x81 ){
		case 0x01 :	// ÉçÉbÉN
		case 0x81 : // îÒòAìÆÅïÉçÉbÉN
			// édñÛÇ…ÇÊÇÈíIâµí≤êÆóLÇËéûÇÕï\é¶ÇÃÇ› 
			if( m_pSnTanaData->Sn_tansw & 0x01 ){
				flg = 1;
			}
			else{
				flg = 2;
			}

			diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, flg );
			diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, flg );
			diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, flg );
			if( m_pSnHeadData->SVmzsw == 1 ){
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, flg );
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, flg );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, flg );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, flg );
			}
			else{
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
			}
			c_set( IDOK );
			break;

		case 0x80 :	// îÒòAìÆ
			diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );
			diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 0 );
			diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 0 );
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
			diag_modify( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDMNK63, 0 );
			//				 ÅV	   è¡îÔê≈äz   (ÅV      )
			diag_modify( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ63, 0 );

			if( m_pSnHeadData->SVmzsw == 1 ){
				// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 0 );
				// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 0 );
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 0 );
				//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 0 );
			}
			else{
				// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
				// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
				//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
			}
			c_cls( IDC_ICSDISPCTRL1,CIcsdisp8ctrl1 );
			::ZeroMemory(buf , sizeof(buf));
			m_txt1.SetWindowTextA(buf);
			diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
			break;

		default   : // òAìÆ
			if( m_pSnTanaData->Sn_tansw & 0x01 ){
				// édñÛÇ…ÇÊÇÈíIâµí≤êÆóLÇËéûÇÕï\é¶ÇÃÇ›
				diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );
				for( index=IDMNK63; index<=IDMENZZ; index++ ){
					diag_modify( IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, index, 1 );
				}
				strcpy_s( buf, sizeof( buf ), _T(" íIâµí≤êÆédñÛÇ™ì¸óÕÇ≥ÇÍÇƒÇ¢Ç‹Ç∑ÅB\n ê≈äzÇèCê≥Ç∑ÇÈèÍçáÇÕÅAäYìñédñÛÇïœçXÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB") );
				puts( IDC_ICSDISPCTRL1, buf, CIcsdisp8ctrl1 );
				m_txt1.SetWindowTextA(buf);
				c_set( IDOK );
			}
			else{
				diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );
				diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 0 );
				diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 0 );
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK63, 0 );
				//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ63, 0 );
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 0 );
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 0 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 0 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 0 );
				}
				else{
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );//[Y]
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );//[Y]
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );//[Y]
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );//[Y]
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
				}
				c_cls( IDC_ICSDISPCTRL1,CIcsdisp8ctrl1 );
				::ZeroMemory(buf , sizeof(buf));
				m_txt1.SetWindowTextA(buf);
				diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
			}
			break;
	}

	*///<--------------------

	
	ComboCheck_AttrSet();//[Y]


	return 0;
}

void CChangeTax::OnOK()
{
	// TODO: Ç±Ç±Ç…ì¡íËÇ»ÉRÅ[ÉhÇí«â¡Ç∑ÇÈÇ©ÅAÇ‡ÇµÇ≠ÇÕäÓñ{ÉNÉâÉXÇåƒÇ—èoÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB

	char	buf[512] = {0};

	if( !(m_pSnTanaData->Sn_tansw&0x01) || (m_pSnHeadData->Sn_Sign4&0x80) ){
		if( m_CbSw == 2 ){ 
			// åxçêÉÅÉbÉZÅ[ÉW
			sprintf_s( buf, sizeof( buf ), _T("â€ê≈ï˚éÆÇÃêÿë÷Ç¶ÇâèúÇµÇ‹Ç∑Ç©ÅH") );
			if( ICSMessageBox( buf, MB_YESNO, 0, 0, this ) == IDYES ){
				m_pSnTanaData->Sn_MENZsw = 0x00;
				tansw = 0x00;
				memset( MNK40, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (4%ï™)
				memset( MNZ40, '\0', MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				memset( MENZK, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (3%ï™)
				memset( MENZZ, '\0', MONY_BUF_SIZE );	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
				memset( MNK63, '\0', MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (6.3%ï™)
				memset( MNZ63, '\0', MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
			}
			else{
				diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
				return;
			}
		}
		memmove( m_pSnTanaData->Sn_MNK63, MNK63, MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (6.3%ï™)
		memmove( m_pSnTanaData->Sn_MNZ63, MNZ63, MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
		memmove( m_pSnTanaData->Sn_MNK40, MNK40, MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (4%ï™)
		memmove( m_pSnTanaData->Sn_MNZ40, MNZ40, MONY_BUF_SIZE );	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
		memmove( m_pSnTanaData->Sn_MENZK, MENZK, MONY_BUF_SIZE );	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (3%ï™)
		memmove( m_pSnTanaData->Sn_MENZZ, MENZZ, MONY_BUF_SIZE );	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )

		m_pSnTanaData->Sn_MENZsw = tansw;

		//[Y]OKÇ®Ç≥ÇÍÇΩéûÇ…ê≈çûÇ›ÅAê≈î≤Ç´ÉTÉCÉìÇï€ë∂Ç∑ÇÈÇÊÇ§Ç…Ç∑ÇÈÅBÉLÉÉÉìÉZÉãÇ®Ç≥ÇÍÇΩèÍçáÇ‡ïœÇÌÇ¡ÇƒÇ¢ÇΩà◊
		if(m_ChSw == 1){//ê≈çûÇ›
			m_pSnHeadData->SVsign |= 0x10;
		}
		else{//ê≈î≤Ç´
			m_pSnHeadData->SVsign &= 0xef;
		}
	}

	ICSDialog::OnOK();
}

void CChangeTax::OnCancel()
{
	// TODO: Ç±Ç±Ç…ì¡íËÇ»ÉRÅ[ÉhÇí«â¡Ç∑ÇÈÇ©ÅAÇ‡ÇµÇ≠ÇÕäÓñ{ÉNÉâÉXÇåƒÇ—èoÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB

	ICSDialog::OnCancel();
}

//-----------------------------------------------------------------------------
// ì¸óÕçÄñ⁄Å@êFïœÇ¶
//-----------------------------------------------------------------------------
// à¯êî	id		ÅG	É_ÉCÉAÉOÉâÉÄID
//		index	ÅF	É_ÉCÉAÉOÉâÉÄì‡ÉCÉìÉfÉbÉNÉX
//		sign	ÅF	ïœçXÉTÉCÉì
//-----------------------------------------------------------------------------
void CChangeTax::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :		// ì¸óÕïsâ¬	ÅiîíêFÅj
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 :		// ÉçÉbÉN	ÅiäDêFÅj
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3 :		// ì¡éÍêFá@	Åiê¬êFÅj
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4 :		// ì¡éÍêFáA	ÅióŒêFÅj	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5 : 		// ì¡éÍêFáB	ÅiîñóŒêFÅj	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:		// é¿äzì¸óÕ	ÅiÉNÉäÅ[ÉÄêFÅj 
			DA.attr_bcolor = BC_CREAM;
			break;
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}

BOOL CChangeTax::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Ç±Ç±Ç…ì¡íËÇ»ÉRÅ[ÉhÇí«â¡Ç∑ÇÈÇ©ÅAÇ‡ÇµÇ≠ÇÕäÓñ{ÉNÉâÉXÇåƒÇ—èoÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB

	CWnd *pWnd = GetFocus();


	if ( pMsg->message == WM_KEYDOWN ){

		if( GetCombineKeyDown(VK_SHIFT) && (VK_F1<=pMsg->wParam) && (pMsg->wParam<=VK_F12) ){
			return TRUE;
		}

		if( pMsg->wParam == VK_TAB  || pMsg->wParam == VK_RIGHT) {
			if(pWnd == GetDlgItem(IDOK)){
				c_set( IDCANCEL );
				return TRUE;	
			}
			else if(pWnd == GetDlgItem(IDCANCEL)){
				diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );	
				return TRUE;
			}
		}

		else if( pMsg->wParam == VK_UP ) {
			if(pWnd == GetDlgItem(IDOK) || pWnd == GetDlgItem(IDCANCEL)){
				diag_setposition( IDC_ICSDIAGCTRL1, IDMENZZ, CIcsdiagctrl );
				return TRUE;	
			}
		}

		else if( pMsg->wParam == VK_DOWN ) {
			if(pWnd == GetDlgItem(IDOK) || pWnd == GetDlgItem(IDCANCEL)){
				diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
				return TRUE;	
			}
		}

		else if( pMsg->wParam == VK_LEFT ) {
			if(pWnd == GetDlgItem(IDOK)){
				diag_setposition( IDC_ICSDIAGCTRL1, IDMENZZ, CIcsdiagctrl );
				return TRUE;	
			}
		}

	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CChangeTax, ICSDialog)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 1, CChangeTax::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 2, CChangeTax::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 3, CChangeTax::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 8, CChangeTax::ComboSelIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CChangeTax::EditONIcsdiagctrl1(short index)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){

		case IDNUKIZ	:	// çTèúëŒè€édì¸äzÅ@ê≈î≤Ç´
			if(m_ChSw == 1){//[Y]
				m_ChSw = 0;
			//	m_pSnHeadData->SVsign &= 0xef;	// ê≈î≤Ç´	//[Y]OKÇ®Ç≥ÇÍÇΩéûÇ…ï€ë∂Ç∑ÇÈÇÊÇ§ëŒâû
				diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
				DiagData.data_check = 1;
				diag_setdata( IDC_ICSDIAGCTRL1,IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				DiagData.data_check = 0;
				diag_setdata( IDC_ICSDIAGCTRL1,IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				ZeiChangeCalq();//[Y]åvéZ
				
				diag_setposition( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );
			}
			break;

		case IDKOMIZ	:	// çTèúëŒè€édì¸äzÅ@ê≈çûÇ›

			if(m_ChSw == 0){//[Y]
				m_ChSw = 1;
//				m_pSnHeadData->SVsign |= 0x10;	// ê≈çûÇ›	//[Y]OKÇ®Ç≥ÇÍÇΩéûÇ…ï€ë∂Ç∑ÇÈÇÊÇ§ëŒâû
				diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
				DiagData.data_check = 0;
				diag_setdata( IDC_ICSDIAGCTRL1,IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				DiagData.data_check = 1;
				diag_setdata( IDC_ICSDIAGCTRL1,IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				ZeiChangeCalq();//[Y]åvéZ
				diag_setposition( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );
			}
			break;

		default :
			ZeiChangeCalq();//[Y]åvéZ

			break;
	}
}

//-----------------------------------------------------------------------------
//[Y] ê≈ïœçXéûÇÃåvéZèàóùä÷êîâª
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CChangeTax::ZeiChangeCalq()
{

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( tmpChSw != m_ChSw ){
		if( m_ChSw == 1 ){
			m_Util.percent( MNZ40, MNK40, 4, 40, 0 );	// ê≈çûÇ›
			m_Util.percent( MENZZ, MENZK, 0, 30, 0 );
			m_Util.percent( MNZ63, MNK63, 9, 63, 0 );
			
		}
		else{
			m_Util.percent( MNZ40, MNK40, 1, 40, 0 );	// ê≈î≤Ç´
			m_Util.percent( MENZZ, MENZK, 1, 30, 0 );
			m_Util.percent( MNZ63, MNK63, 1, 63, 0 );
		}
		memmove( DiagData.data_val, MNZ63, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMNZ63, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memmove( DiagData.data_val, MNZ40, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMNZ40, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memmove( DiagData.data_val, MENZZ, MONY_BUF_SIZE );
		diag_setdata( IDC_ICSDIAGCTRL1, IDMENZZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
	if( m_ChSw == 1 ){
		tmpChSw = 1;
	}
	else{
		tmpChSw = 0;
	}
}

void CChangeTax::EditOFFIcsdiagctrl1(short index)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	char			CkMy[MONY_BUF_SIZE];
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};

	switch( index ) {

		case IDCOMBO	:	// â€ê≈ï˚éÆëIë

			/*
			diag_getdata( IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_CbSt = 	DiagData.data_combo;
			switch( m_CbSt ) {

				case 0 :	// ñ∆ê≈ÇÊÇËâ€ê≈
					m_CbSw = 0;

					m_pSnTanaData->Sn_MENZsw = 0x10;
					memset( buf, '\0', sizeof( buf ) );
					strcpy_s( buf, sizeof( buf ), _T("ìñä˙ÇÊÇËÅAñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃ\n") );
					strcat_s( buf, sizeof( buf ), _T("â€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl );

					memset( buf, '\0', sizeof( buf ) );
					strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€\n") );
					strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					break;

				case 1 :	// â€ê≈ÇÊÇËñ∆ê≈
					m_CbSw = 1;

					m_pSnTanaData->Sn_MENZsw = 0x11;
					memset( buf, '\0', sizeof( buf ) );
					strcpy_s( buf, sizeof( buf ), _T("óÇä˙ÇÊÇËÅAâ€ê≈ã∆é“Ç©ÇÁñ∆ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃ\n") );
					strcat_s( buf, sizeof( buf ), _T("â€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl );

					memset( buf, '\0', sizeof( buf ) );
					strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€äO\n") );
					strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					break;

				default:
					m_CbSw = 2;
					break;
			}
			*/

			break;
			
		case IDMNK63	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3Åìï™)
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, MNK63, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, IDMNK63, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( MNK63, CkMy ) ){
				memmove( MNK63, CkMy, MONY_BUF_SIZE );
				if( m_ChSw == 1 ){//[Y]
					m_Util.percent( MNZ63, MNK63, 9, 63, 0 );
				}
				else{
					m_Util.percent( MNZ63, MNK63, 1, 63, 0 );
				}
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)
				memmove( DiagData.data_val, MNZ63, MONY_BUF_SIZE );
				diag_setdata( IDC_ICSDIAGCTRL1, IDMNZ63, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}	
			break;

		case IDMNK40	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4Åìï™)
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, MNK40, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, IDMNK40, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( MNK40, CkMy ) ){
				memmove( MNK40, CkMy, MONY_BUF_SIZE );
				if( m_ChSw == 1 ){//[Y]
					m_Util.percent( MNZ40, MNK40, 4, 40, 0 );
				}
				else{
					m_Util.percent( MNZ40, MNK40, 1, 40, 0 );
				}
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)
				memmove( DiagData.data_val, MNZ40, MONY_BUF_SIZE );
				diag_setdata( IDC_ICSDIAGCTRL1, IDMNZ40, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}	
			break;

		case IDMENZK	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3Åìï™)
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, MENZK, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, IDMENZK, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( MENZK, CkMy ) ){
				memmove( MENZK, CkMy, MONY_BUF_SIZE );
				if( m_ChSw == 1 ){//[Y]
					m_Util.percent( MENZZ, MENZK, 0, 30, 0 );
				}
				else{
					m_Util.percent( MENZZ, MENZK, 1, 30, 0 );
				}
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)
				memmove( DiagData.data_val, MENZZ, MONY_BUF_SIZE );
				diag_setdata( IDC_ICSDIAGCTRL1, IDMENZZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}	
			break;

		case IDMNZ63	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz   (6.3%ï™)
			diag_getdata( IDC_ICSDIAGCTRL1, IDMNZ63, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( MNZ63, DiagData.data_val, MONY_BUF_SIZE ); 
			break;

		case IDMNZ40	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz   (4%ï™)
			diag_getdata( IDC_ICSDIAGCTRL1, IDMNZ40, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( MNZ40, DiagData.data_val, MONY_BUF_SIZE ); 
			break;

		case IDMENZZ	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz   (3%ï™)
			diag_getdata( IDC_ICSDIAGCTRL1, IDMENZZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( MENZZ, DiagData.data_val, MONY_BUF_SIZE ); 
			break;

		default			:
			break;
	}
}

void CChangeTax::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	char			VK_FLG, VK_DMM;
	short			wTerm;
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	if( nChar == 0 ){
		return;
	}
	
	// Shift+TABÇTABÇ…ÅATABÇENTERÇ…ï‘ä“
	VK_FLG = VK_DMM = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}
	
	switch( index ){

		case IDCOMBO	:	// â€ê≈ï˚éÆëIë

			if( wTerm == VK_UP ){
				return;
			}

			if( wTerm == VK_ESCAPE ){
				OnCancel();
				return;
			}

			if( wTerm == VK_DOWN ){
				m_ICSDIAG1.ComboDropDown( IDCOMBO, TRUE );
				return;
			}
			diag_getdata( IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( DiagData.data_combo == 2 ){
				c_set( IDOK );
				return;
			}
			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				if( m_ChSw == 1 ){//[Y]
					diag_setposition( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );	// ê≈çûÇ›
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );	// ê≈î≤Ç´
				}
			}

			break;

		case IDKOMIZ	:	// çTèúëŒè€äOédì¸äzÅ@ê≈çûÇ›
		case IDNUKIZ	:	// çTèúëŒè€äOédì¸äzÅ@ê≈î≤Ç´

			if((wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){//[Y]
				if( m_ChSw == 1 ){//[Y]
					m_ChSw = 0;
		//			m_pSnHeadData->SVsign &= 0xef;	// ê≈î≤Ç´
					DiagData.data_check = 1;
					diag_setdata( IDC_ICSDIAGCTRL1,IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					DiagData.data_check = 0;
					diag_setdata( IDC_ICSDIAGCTRL1,IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ZeiChangeCalq();//[Y]åvéZ
					diag_setposition( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );
				}
				else{
					m_ChSw = 1;//[Y]
//					m_pSnHeadData->SVsign |= 0x10;	// ê≈çûÇ›
					DiagData.data_check = 0;
					diag_setdata( IDC_ICSDIAGCTRL1,IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					DiagData.data_check = 1;
					diag_setdata( IDC_ICSDIAGCTRL1,IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ZeiChangeCalq();//[Y]åvéZ
					diag_setposition( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );
				}
			}
			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN )){
				diag_setposition( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_F2)  || (wTerm == VK_UP )){
				diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
			}
			break;				

		case IDMNK40	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4ï™)

			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN ) || (wTerm == VK_RIGHT ) ){
				if( m_pSnHeadData->SVmzsw == 1 ){
					diag_setposition( IDC_ICSDIAGCTRL1, IDMENZK, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ40, CIcsdiagctrl );
				}
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm == VK_UP )){
				if( m_ChSw == 1 ){//[Y]
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );	// ê≈çûÇ›
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );	// ê≈î≤Ç´
				}
			}
			break;

		case IDMNK63	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)

			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN ) || (wTerm == VK_RIGHT ) ){
				if( m_pSnHeadData->SVmzsw == 1 ){
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNK40, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
				}
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2)  || (wTerm == VK_UP )){
				if( m_ChSw == 1 ){//[Y]
					diag_setposition( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );	// ê≈çûÇ›
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );	// ê≈î≤Ç´
				}
			}
			break;

		case IDMENZK	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)

			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN ) || (wTerm == VK_RIGHT ) ){
				diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
				//DiagData.movepos = IDMNZ40;
				//m_ICSDIAG1.ReplyTermination( DIAG_REPLY_MOVE, (struct IUnknown *)&DiagData );
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2)  || (wTerm == VK_UP )){
				diag_setposition( IDC_ICSDIAGCTRL1, IDMNK40, CIcsdiagctrl );
			}
			break;

		case IDMNZ40	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz   (ê≈ó¶4%ï™)

			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN ) || (wTerm == VK_RIGHT ) ){
				if( m_pSnHeadData->SVmzsw == 1 ){
					diag_setposition( IDC_ICSDIAGCTRL1, IDMENZZ, CIcsdiagctrl );
				}
				else{
					c_set( IDOK );
				}
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2)  || (wTerm == VK_UP )){
				if( m_pSnHeadData->SVmzsw == 1 ){
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
				}
			}
			break;

		case IDMNZ63	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz   (ê≈ó¶6.3%ï™)
			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN ) || (wTerm == VK_RIGHT ) ){
				if( m_pSnHeadData->SVmzsw == 1 ){
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ40, CIcsdiagctrl );
				}
				else{
					c_set( IDOK );
				}
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm == VK_UP )){
				if( m_pSnHeadData->SVmzsw == 1 ){
					diag_setposition( IDC_ICSDIAGCTRL1, IDMENZK, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );
				}
			}
			break;
		case IDMENZZ	:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz   (ãåê≈ó¶ï™)

			// éüçÄñ⁄
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm == VK_DOWN ) || (wTerm == VK_RIGHT ) ){
				c_set( IDOK );
			}
			// ëOçÄñ⁄Ç÷
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm == VK_UP ) ){
				diag_setposition( IDC_ICSDIAGCTRL1, IDMNZ40, CIcsdiagctrl );
			}
			break;

		default			:
			break;
	}

}


//------>ÉCÉxÉìÉgí«â¡
void CChangeTax::ComboSelIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB

	char			buf[512] = {0};
	DIAGRAM_DATA	diag_data;

	if(index == IDCOMBO){
		diag_getdata( IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&diag_data, CIcsdiagctrl );
		m_CbSt = 	diag_data.data_combo;
		switch( m_CbSt ) {

			case 0 :	// ñ∆ê≈ÇÊÇËâ€ê≈
				m_CbSw = 0;
				tansw = 0x10;//[Y]
//				m_pSnTanaData->Sn_MENZsw = 0x10;
				memset( buf, '\0', sizeof( buf ) );
				strcpy_s( buf, sizeof( buf ), _T("ìñä˙ÇÊÇËÅAñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃ\n") );
				strcat_s( buf, sizeof( buf ), _T("â€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz") );
				diag_data.data_disp = buf;
				diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl );

				memset( buf, '\0', sizeof( buf ) );
				strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€\n") );
				strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
				diag_data.data_disp = buf;
				diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&diag_data, CIcsdiagctrl );
				break;

			case 1 :	// â€ê≈ÇÊÇËñ∆ê≈
				m_CbSw = 1;
				tansw = 0x11;//[Y]
//				m_pSnTanaData->Sn_MENZsw = 0x11;
				memset( buf, '\0', sizeof( buf ) );
				strcpy_s( buf, sizeof( buf ), _T("óÇä˙ÇÊÇËÅAâ€ê≈ã∆é“Ç©ÇÁñ∆ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃ\n") );
				strcat_s( buf, sizeof( buf ), _T("â€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz") );
				diag_data.data_disp = buf;
				diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl );

				memset( buf, '\0', sizeof( buf ) );
				strcpy_s( buf, sizeof( buf ), _T("çTèúëŒè€äO\n") );
				strcat_s( buf, sizeof( buf ), _T("édì¸äz") );
				diag_data.data_disp = buf;
				diag_setdata( IDC_ICSDIAGCTRL1, IDSUBTL, (struct IUnknown *)&diag_data, CIcsdiagctrl );
				break;

			default:
				m_CbSw = 2;
				tansw = 0x00;
				memset( buf, '\0', sizeof( buf ) );
				strcpy_s( buf, sizeof( buf ), _T(" ") );
				diag_data.data_disp = buf;
				diag_setdata( IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl );

				break;
		}

		ComboCheck_AttrSet();

	}

}


//-----------------------------------------------------------------------------
//	åªç›ÇÃÉRÉìÉ{É{ÉbÉNÉXÇÃèÓïÒÇÉ`ÉFÉbÉNÇµì¸óÕëÆê´ÅAîwåiêFÇÉZÉbÉg
//-----------------------------------------------------------------------------
void CChangeTax::ComboCheck_AttrSet()
{
	int		flg = 0;
	short	index = 0;

	char	buf[512] = {0};

	switch( m_pSnHeadData->Sn_Sign4&0x81 ){
		case 0x01 :	// ÉçÉbÉN
		case 0x81 : // îÒòAìÆÅïÉçÉbÉN
			// édñÛÇ…ÇÊÇÈíIâµí≤êÆóLÇËéûÇÕï\é¶ÇÃÇ› 
			if( m_pSnTanaData->Sn_tansw & 0x01 ){
				flg = 1;
			}
			else{
				flg = 2;
			}
			diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, flg );
			diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, flg );
			diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, flg );
			if( m_pSnHeadData->SVmzsw == 1 ){
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, flg );
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, flg );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, flg );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, flg );
			}
			else{
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
			}
			c_set( IDOK );
			break;

		case 0x80 :	// îÒòAìÆ
			diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );


			if( tansw != 0x10 && tansw != 0x11 ){

				diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK63, 1 );
				//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ63, 1 );

				if( m_pSnHeadData->SVmzsw == 1 ){
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
				}
				else{
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
				}
			}
			else{
				diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 0 );
				diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 0 );
				// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
				diag_modify( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNK63, 0 );
				//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ63, 0 );

				if( m_pSnHeadData->SVmzsw == 1 ){
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 0 );
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 0 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 0 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 0 );
				}
				else{
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
				}
			}

			c_cls( IDC_ICSDISPCTRL1,CIcsdisp8ctrl1 );
			::ZeroMemory(buf , sizeof(buf));
			m_txt1.SetWindowTextA(buf);
			diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
			break;

		default   : // òAìÆ
			if( m_pSnTanaData->Sn_tansw & 0x01 ){
				// édñÛÇ…ÇÊÇÈíIâµí≤êÆóLÇËéûÇÕï\é¶ÇÃÇ›
				diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );
				diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );
				for( index=IDMNK63; index<=IDMENZZ; index++ ){
					diag_modify( IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, index, 1 );
				}
				strcpy_s( buf, sizeof( buf ), _T(" íIâµí≤êÆédñÛÇ™ì¸óÕÇ≥ÇÍÇƒÇ¢Ç‹Ç∑ÅB\n ê≈äzÇèCê≥Ç∑ÇÈèÍçáÇÕÅAäYìñédñÛÇïœçXÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB") );
				puts( IDC_ICSDISPCTRL1, buf, CIcsdisp8ctrl1 );
				m_txt1.SetWindowTextA(buf);
				c_set( IDOK );
			}
			else{

				if( tansw != 0x10 && tansw != 0x11 ){
					diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );
					
					diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );
					
					diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );
					
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK63, 1 );
					
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ63, 1 );
					
					// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
					
					// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
					
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
					
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
					
				}
				else{
					diag_modify( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );
					diag_modify( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDNUKIZ, 0 );
					diag_modify( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDKOMIZ, 0 );
					// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
					diag_modify( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNK63, 0 );
					//				 ÅV	   è¡îÔê≈äz   (ÅV      )
					diag_modify( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ63, 0 );
					if( m_pSnHeadData->SVmzsw == 1 ){
						// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
						diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_EDIT, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 0 );
						// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
						diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 0 );
						// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
						diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_EDIT, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 0 );
						//				 ÅV	   è¡îÔê≈äz   (ÅV      )
						diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_EDIT, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 0 );
					}
					else{
						// 				 ÅV	   íIâµéëéYäz (ê≈ó¶3%ï™)
						diag_modify( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMENZK, 1 );
						// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
						diag_modify( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );
						// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
						diag_modify( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMNK40, 1 );
						//				 ÅV	   è¡îÔê≈äz   (ÅV      )
						diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
					}
				}
				c_cls( IDC_ICSDISPCTRL1,CIcsdisp8ctrl1 );
				::ZeroMemory(buf , sizeof(buf));
				m_txt1.SetWindowTextA(buf);
				diag_setposition( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
			}
			break;
	}


	diag_redraw(IDC_ICSDIAGCTRL1 , CIcsdiagctrl);//çƒï`âÊ


	return;
}