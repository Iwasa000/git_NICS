// SpecPassOffEx.cpp : é¿ëïÉtÉ@ÉCÉã
//

#include "stdafx.h"
#include "SpecPassOffEx.h"

//== íËêî ==
const	int	IdTop		=	64;			// ã‡äzï\é¶ÇÃèâä˙à íu
const	int	IdZselCmb	=	IdTop+30;	// ê≈ó¶ëIëÉRÉìÉ{É{ÉbÉNÉX

// CSpecPassOffEx É_ÉCÉAÉçÉO

IMPLEMENT_DYNAMIC(CSpecPassOffEx, ICSDialog)

CSpecPassOffEx::CSpecPassOffEx(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSpecPassOffEx::IDD, pParent)
	, m_pParent(pParent)
	, m_pSnHeadData(NULL)
	, m_pSpcListData(NULL)
	, m_pArith(NULL)
{
}

CSpecPassOffEx::~CSpecPassOffEx()
{
}

void CSpecPassOffEx::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Diag1);
}


BEGIN_MESSAGE_MAP(CSpecPassOffEx, ICSDialog)
END_MESSAGE_MAP()


// CSpecPassOffEx ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ

BOOL CSpecPassOffEx::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  Ç±Ç±Ç…èâä˙âªÇí«â¡ÇµÇƒÇ≠ÇæÇ≥Ç¢

	InitControl();
	ChgDispByZeiritsu();

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ó·äO : OCX ÉvÉçÉpÉeÉB ÉyÅ[ÉWÇÕïKÇ∏ FALSE Çï‘ÇµÇ‹Ç∑ÅB
}

//-----------------------------------------------------------------------------
// ê≈ó¶ÉRÉìÉ{É{ÉbÉNÉXèâä˙âª
//-----------------------------------------------------------------------------
void CSpecPassOffEx::InitZrtCombo()
{
	CString	zrtTitle;

	if( (*m_pSnHeadData)->SVmzsw == 1 ){
		zrtTitle = _T(" ÇTÅì\r\n ÇWÅì\r\n ÇWÅìÅiåyå∏ê≈ó¶Åj\r\n ÇPÇOÅì\r\n");
	}
	else{
		zrtTitle = _T(" ÇWÅìÅiåyå∏ê≈ó¶Åj\r\n ÇPÇOÅì\r\n");
	}

	// ã∆éÌñºÉZÉbÉg
	DIAGRAM_DATA data;
	data.data_imgdata = NULL;
	data.text_combo = zrtTitle;
	m_Diag1.SetData( IdZselCmb, (LPUNKNOWN)&data );
}

int CSpecPassOffEx::Disp_10per()
{
	char			CBF[80]={0}, BUF[80]={0}, KBF[80]={0}, MBF[80]={0},
		WORK0[6]={0}, WORK1[6]={0}, WORK2[6]={0};
	char			moji[64]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// á@.â€ê≈îÑè„äÑçá
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuws, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuwb, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áA.í≤êÆëOÇÃédì¸çTèúê≈äz
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz78, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áB.ì¡íËäÑçá	
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGsyu, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpGsyu, (*m_pSpcListData)->stval.SpTuwb );
	m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTswr, _T("SSS9.9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_input( WORK0, _T("50") );
	if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpTswr, WORK0 ) > 0 ){	// ì¡ó·é˚ì¸äÑçáÇTÅìà»è„	

		// áC.ì¡íËé˚ì¸Ç…åWÇÈê≈äz
		memset( WORK0, '\0', sizeof(WORK0) );
		wsprintf( moji, _T("%d"), (*m_pSnHeadData)->m_UriRatioBorder );
		m_pArith->l_input( WORK0, moji );
		//if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ) {	// â€ê≈îÑè„äÑçáÇXÇTÅìà»è„
		if ( ( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ) && ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) ) {	// â€ê≈îÑè„äÑçáÇXÇTÅìà»è„
			memset( WORK0, '\0', sizeof(WORK0) );
			m_pArith->l_add( WORK0, WORK0, (*m_pSpcListData)->stval.SpTsyu78 );
			m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg78, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+8, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
				// î‰ó·îzï™
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu78, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg78, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// å¬ï ëŒâû
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSsyu78, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg78, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsyu78, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+15, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKszg78, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
		}
		// áD.í≤êÆäÑçá
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpFsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+17, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memset( WORK0, '\0', 6 );
		m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpFsyu, (*m_pSpcListData)->stval.SpTuwb );
		m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+19, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áE.í≤êÆäz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+22, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+23, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áFÅDì¡íËé˚ì¸Ç…åWÇÈí≤êÆê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+25, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+26, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// áGÅDçTèúëŒè€édì¸ê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+27, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+28, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz78, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+29, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	return 0;
}

int CSpecPassOffEx::Disp_8perK()
{
	char			CBF[80]={0}, BUF[80]={0}, KBF[80]={0}, MBF[80]={0},
		WORK0[6]={0}, WORK1[6]={0}, WORK2[6]={0};
	char			moji[64]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// á@.â€ê≈îÑè„äÑçá
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuws, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuwb, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áA.í≤êÆëOÇÃédì¸çTèúê≈äz
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz624, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áB.ì¡íËäÑçá	
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGsyu, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpGsyu, (*m_pSpcListData)->stval.SpTuwb );
	m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTswr, _T("SSS9.9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_input( WORK0, _T("50") );
	if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpTswr, WORK0 ) > 0 ){	// ì¡ó·é˚ì¸äÑçáÇTÅìà»è„	

																			// áC.ì¡íËé˚ì¸Ç…åWÇÈê≈äz
		memset( WORK0, '\0', sizeof(WORK0) );
		wsprintf( moji, _T("%d"), (*m_pSnHeadData)->m_UriRatioBorder );
		m_pArith->l_input( WORK0, moji );
		if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ) {	// â€ê≈îÑè„äÑçáÇXÇTÅìà»è„
			memset( WORK0, '\0', sizeof(WORK0) );
			m_pArith->l_add( WORK0, WORK0, (*m_pSpcListData)->stval.SpTsyu624 );
			m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg624, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+8, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
				// î‰ó·îzï™
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu624, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg624, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// å¬ï ëŒâû
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSsyu624, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg624, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsyu624, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+15, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKszg624, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
		}
		// áD.í≤êÆäÑçá
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpFsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+17, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memset( WORK0, '\0', 6 );
		m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpFsyu, (*m_pSpcListData)->stval.SpTuwb );
		m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+19, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áE.í≤êÆäz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+22, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+23, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áFÅDì¡íËé˚ì¸Ç…åWÇÈí≤êÆê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+25, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+26, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// áGÅDçTèúëŒè€édì¸ê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+27, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+28, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz624, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+29, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// èâä˙ï\é¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//			-1	ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CSpecPassOffEx::Disp_8per()
{
	char			CBF[80]={0}, BUF[80]={0}, KBF[80]={0}, MBF[80]={0},
					WORK0[6]={0}, WORK1[6]={0}, WORK2[6]={0};
	char			moji[64]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// á@.â€ê≈îÑè„äÑçá
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuws, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuwb, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áA.í≤êÆëOÇÃédì¸çTèúê≈äz
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz63, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áB.ì¡íËäÑçá	
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGsyu, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpGsyu, (*m_pSpcListData)->stval.SpTuwb );
	m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTswr, _T("SSS9.9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_input( WORK0, _T("50") );
	if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpTswr, WORK0 ) > 0 ){	// ì¡ó·é˚ì¸äÑçáÇTÅìà»è„	

		// áC.ì¡íËé˚ì¸Ç…åWÇÈê≈äz
		memset( WORK0, '\0', sizeof(WORK0) );
		wsprintf( moji, _T("%d"), (*m_pSnHeadData)->m_UriRatioBorder );
		m_pArith->l_input( WORK0, moji );
		if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ) {	// â€ê≈îÑè„äÑçáÇXÇTÅìà»è„
			memset( WORK0, '\0', sizeof(WORK0) );
			m_pArith->l_add( WORK0, WORK0, (*m_pSpcListData)->stval.SpTsyu63 );
			m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg63, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+8, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
				// î‰ó·îzï™
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu63, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg63, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// å¬ï ëŒâû
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSsyu63, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg63, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsyu63, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+15, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKszg63, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
		}
		// áD.í≤êÆäÑçá
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpFsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+17, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memset( WORK0, '\0', 6 );
		m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpFsyu, (*m_pSpcListData)->stval.SpTuwb );
		m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+19, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áE.í≤êÆäz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+22, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+23, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áFÅDì¡íËé˚ì¸Ç…åWÇÈí≤êÆê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+25, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+26, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// áGÅDçTèúëŒè€édì¸ê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+27, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+28, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz63, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+29, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// èâä˙ï\é¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//			-1	ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CSpecPassOffEx::Disp_5per()
{
	char			CBF[80]={0}, BUF[80]={0}, KBF[80]={0}, MBF[80]={0},
					WORK0[6]={0}, WORK1[6]={0}, WORK2[6]={0};
	char			moji[64]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// á@.â€ê≈îÑè„äÑçá
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuws, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuwb, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áA.í≤êÆëOÇÃédì¸çTèúê≈äz
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz4, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// áB.ì¡íËäÑçá	
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGsyu, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpGsyu, (*m_pSpcListData)->stval.SpTuwb );
	m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTswr, _T("SSS9.9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_input( WORK0, _T("50") );
	if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpTswr, WORK0 ) > 0 ){	// ì¡ó·é˚ì¸äÑçáÇTÅìà»è„	

		// áC.ì¡íËé˚ì¸Ç…åWÇÈê≈äz
		memset( WORK0, '\0', sizeof(WORK0) );
		wsprintf( moji, _T("%d"), (*m_pSnHeadData)->m_UriRatioBorder );
		m_pArith->l_input( WORK0, moji );
		if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ) {	// â€ê≈îÑè„äÑçáÇXÇTÅìà»è„
			memset( WORK0, '\0', sizeof(WORK0) );
			m_pArith->l_add( WORK0, WORK0, (*m_pSpcListData)->stval.SpTsyu4 );
			m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg4, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+8, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
				// î‰ó·îzï™
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu4, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg4, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// å¬ï ëŒâû
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSsyu4, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg4, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsyu4, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+15, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKszg4, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
		}
		// áD.í≤êÆäÑçá
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpFsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+17, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memset( WORK0, '\0', 6 );
		m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpFsyu, (*m_pSpcListData)->stval.SpTuwb );
		m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+19, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áE.í≤êÆäz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+22, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+23, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// áFÅDì¡íËé˚ì¸Ç…åWÇÈí≤êÆê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+25, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+26, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// áGÅDçTèúëŒè€édì¸ê≈äz
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+27, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+28, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+29, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	return 0;
}


void CSpecPassOffEx::Edit_Static_10per()
{
	DIAGRAM_DATA	DiagData;

	int st_top = 17;

	m_Diag1.GetData( st_top, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇVÅDÇW");
	m_Diag1.SetData( st_top, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+1, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇPÇO");
	m_Diag1.SetData( st_top+1, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+9, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇVÅDÇW");
	m_Diag1.SetData( st_top+9, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+10, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇPÇO");
	m_Diag1.SetData( st_top+10, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+18, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇVÅDÇW");
	m_Diag1.SetData( st_top+18, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+19, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇPÇO");
	m_Diag1.SetData( st_top+19, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+24, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇVÅDÇW");
	m_Diag1.SetData( st_top+24, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+25, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇPÇO");
	m_Diag1.SetData( st_top+25, (LPUNKNOWN)&DiagData );
}


void CSpecPassOffEx::Edit_Static_8perK()
{
	DIAGRAM_DATA	DiagData;

	int st_top = 17;

	m_Diag1.GetData( st_top, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇQÇS");
	m_Diag1.SetData( st_top, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+1, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+1, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+9, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇQÇS");
	m_Diag1.SetData( st_top+9, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+10, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+10, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+18, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇQÇS");
	m_Diag1.SetData( st_top+18, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+19, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+19, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+24, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇQÇS");
	m_Diag1.SetData( st_top+24, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+25, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+25, (LPUNKNOWN)&DiagData );
}


void CSpecPassOffEx::Edit_Static_8per()
{
	DIAGRAM_DATA	DiagData;

	int st_top = 17;

	m_Diag1.GetData( st_top, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇR");
	m_Diag1.SetData( st_top, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+1, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+1, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+9, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇR");
	m_Diag1.SetData( st_top+9, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+10, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+10, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+18, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇR");
	m_Diag1.SetData( st_top+18, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+19, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+19, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+24, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇUÅDÇR");
	m_Diag1.SetData( st_top+24, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+25, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇW");
	m_Diag1.SetData( st_top+25, (LPUNKNOWN)&DiagData );
}

void CSpecPassOffEx::Edit_Static_5per()
{
	DIAGRAM_DATA	DiagData;

	int st_top = 17;

	m_Diag1.GetData( st_top, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇS");
	m_Diag1.SetData( st_top, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+1, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇT");
	m_Diag1.SetData( st_top+1, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+9, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇS");
	m_Diag1.SetData( st_top+9, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+10, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇT");
	m_Diag1.SetData( st_top+10, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+18, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇS");
	m_Diag1.SetData( st_top+18, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+19, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇT");
	m_Diag1.SetData( st_top+19, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+24, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇS");
	m_Diag1.SetData( st_top+24, (LPUNKNOWN)&DiagData );

	m_Diag1.GetData( st_top+25, (LPUNKNOWN)&DiagData );
	DiagData.data_disp.Format("ÇPÇOÇT");
	m_Diag1.SetData( st_top+25, (LPUNKNOWN)&DiagData );
}

void CSpecPassOffEx::Disp_AllClear()
{
	for( int cnt = 0; cnt < 30; cnt++ ){
		diag_clear( IDC_ICSDIAGCTRL1, IdTop+cnt, TRUE, CIcsdiagctrl );
	}
}

int CSpecPassOffEx::GetZeiritsu()
{
	int ret = -1; 

	DIAGRAM_DATA	DiagData;
	m_Diag1.GetData( IdZselCmb, (LPUNKNOWN)&DiagData );

	if( (*m_pSnHeadData)->SVmzsw == 1 ){
		switch( DiagData.data_combo ){
			case 0:
				ret = zeiritsu_5per;
				break;
			case 1:
				ret = zeiritsu_8per;
				break;
			case 2:
				ret = zeiritsu_8perK;
				break;
			case 3:
				ret = zeiritsu_10per;
				break;
			default:
				break;
		}
	}
	else{
		switch( DiagData.data_combo ){
			case 0:
				ret = zeiritsu_8perK;
				break;
			case 1:
				ret = zeiritsu_10per;
				break;
			default:
				break;
		}
	}

	return ret;
}

//-----------------------------------------------------------------------------
// èâä˙ílÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSnHeadData		ÅF	ÉwÉbÉ_èÓïÒ
//		pSpcListData	ÅF	ì¡íËé˚ì¸åvéZï\ÉfÅ[É^
//		pArith			ÅF	î{í∑ââéZ
//-----------------------------------------------------------------------------
// ï‘ëóíl	0			ÅF	ê≥èÌèIóπ
//			-1			ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CSpecPassOffEx::Init( CSnHeadData **pSnHeadData, CH31SpcListData **pSpcListData, CArithEx *pArith )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSpcListData );
	if( pSpcListData == NULL ){
		return -1;
	}
	m_pSpcListData = pSpcListData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	return 0;
}

int CSpecPassOffEx::InitControl()
{
	// ê≈ó¶ÉRÉìÉ{èâä˙âª
	InitZrtCombo();

	// ÉRÉìÉ{É{ÉbÉNÉXèâä˙ílÉZÉbÉg
	DIAGRAM_DATA	DiagData;
	m_Diag1.GetData( IdZselCmb, (LPUNKNOWN)&DiagData );
	if( (*m_pSnHeadData)->SVmzsw == 1 ){
		DiagData.data_combo = 3;
	}
	else{
		DiagData.data_combo = 1;
	}
	m_Diag1.SetData( IdZselCmb, (LPUNKNOWN)&DiagData );

	return 0;	
}
BEGIN_EVENTSINK_MAP(CSpecPassOffEx, ICSDialog)
	ON_EVENT(CSpecPassOffEx, IDC_ICSDIAGCTRL1, 8, CSpecPassOffEx::ComboSelIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CSpecPassOffEx::ComboSelIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	if( index == IdZselCmb ){
		Disp_AllClear();
		ChgDispByZeiritsu();
	}
}

//-----------------------------------------------------------------------------
// ê≈ó¶Ç…çáÇÌÇπÇΩâÊñ êÿë÷
//-----------------------------------------------------------------------------
void CSpecPassOffEx::ChgDispByZeiritsu()
{
	int zrt = GetZeiritsu();
	if( zrt == zeiritsu_10per ){
		Edit_Static_10per();
		Disp_10per();
	}
	else if( zrt == zeiritsu_8perK ){
		Edit_Static_8perK();
		Disp_8perK();
	}
	else if( zrt == zeiritsu_8per ){
		Edit_Static_8per();
		Disp_8per();
	}
	else if( zrt == zeiritsu_5per ){
		Edit_Static_5per();
		Disp_5per();
	}
}

