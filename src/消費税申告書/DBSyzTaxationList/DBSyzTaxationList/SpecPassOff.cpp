// SpecPassOff.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SpecPassOff.h"


//== 定数 ==
const	int	IdTop	=	65;

// CSpecPassOff ダイアログ

IMPLEMENT_DYNAMIC(CSpecPassOff, ICSDialog)

CSpecPassOff::CSpecPassOff(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSpecPassOff::IDD, pParent)
	, m_pParent(pParent)
	, m_pSnHeadData(NULL)
	, m_pSpcListData(NULL)
	, m_pArith(NULL)
{
}

CSpecPassOff::~CSpecPassOff()
{
}

void CSpecPassOff::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpecPassOff, ICSDialog)
END_MESSAGE_MAP()


// CSpecPassOff メッセージ ハンドラ

BOOL CSpecPassOff::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	InitDisp();

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期表示
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSpecPassOff::InitDisp()
{
	char			CBF[80]={0}, BUF[80]={0}, KBF[80]={0}, MBF[80]={0},
					WORK0[6]={0}, WORK1[6]={0}, WORK2[6]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// ①.課税売上割合
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuws, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuwb, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// ②.調整前の仕入控除税額
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgzg, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// ③.特定割合	
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGsyu, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpGsyu, (*m_pSpcListData)->stval.SpTuwb );
	m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// [04'07.29]
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTswr, _T("SSS9.9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( WORK0, '\0', sizeof(WORK0) );
	// [04'07.29]
	m_pArith->l_input( WORK0, _T("50") );
	if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpTswr, WORK0 ) > 0 ){	// 特例収入割合５％以上	

		// ④.特定収入に係る税額
		memset( WORK0, '\0', sizeof(WORK0) );
		m_pArith->l_input( WORK0, &(*m_pSnHeadData)->m_UriRatioBorder );
		if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ) {	// 課税売上割合９５％以上
			memset( WORK0, '\0', sizeof(WORK0) );
			m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpTsyu, (*m_pSpcListData)->stval.SpKsyu );
			m_pArith->l_add( WORK0, WORK0, (*m_pSpcListData)->stval.SpHsyu );
			m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+8, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
				// 比例配分
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// 個別対応
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsyu, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+15, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKszg, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
		}
		// ⑤.調整割合
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
		// ⑥.調整額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgzg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+22, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+23, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// ⑦．特定収入に係る調整税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+25, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgtg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+26, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// ⑧．控除対象仕入税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgzg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+27, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgtg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+28, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSizg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+29, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 初期値セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData		：	ヘッダ情報
//		pSpcListData	：	特定収入計算表データ
//		pArith			：	倍長演算
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CSpecPassOff::Init( CSnHeadData **pSnHeadData, CH26SpcListData **pSpcListData, CArithEx *pArith )
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


