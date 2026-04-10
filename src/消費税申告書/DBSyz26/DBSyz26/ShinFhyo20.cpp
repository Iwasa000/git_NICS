// ShinFhyo20.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo20.h"
#include "ShinFhyo20Idx.h"
#include "H26HyoView.h"	// '15.02.16

#include "ChangeTax.h"
#include "ChangeTaxEx.h"


// CShinFhyo20 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo20, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinFhyo20::CShinFhyo20(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo20::IDD, pParent)
	, m_pSnFhyo20Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pSnTanaData(NULL)
	, m_pSnFhyo10Data(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
	, m_DlSw(0)
	, m_UWexp(0)
{
	memset( F21A, '\0', sizeof(F21A) );
	memset( F21B, '\0', sizeof(F21B) );
	memset( F21C, '\0', sizeof(F21C) );
	memset( F22A, '\0', sizeof(F22A) );
	memset( F22B, '\0', sizeof(F22B) );
	memset( F22C, '\0', sizeof(F22C) );
	memset( F23A, '\0', sizeof(F23A) );
	memset( F23B, '\0', sizeof(F23B) );
	memset( F23C, '\0', sizeof(F23C) );
	memset( F24C, '\0', sizeof(F24C) );
	memset( F26A, '\0', sizeof(F26A) );
	memset( F26B, '\0', sizeof(F26B) );
	memset( F26C, '\0', sizeof(F26C) );
	memset( F27C, '\0', sizeof(F27C) );
	memset( F28A, '\0', sizeof(F28A) );
	memset( F28B, '\0', sizeof(F28B) );
	memset( F28C, '\0', sizeof(F28C) );
	memset( F29A, '\0', sizeof(F29A) );
	memset( F29B, '\0', sizeof(F29B) );
	memset( F29C, '\0', sizeof(F29C) );
	memset( F210A, '\0', sizeof(F210A) );
	memset( F210B, '\0', sizeof(F210B) );
	memset( F210C, '\0', sizeof(F210C) );
	memset( F211A, '\0', sizeof(F211A) );
	memset( F211B, '\0', sizeof(F211B) );
	memset( F211C, '\0', sizeof(F211C) );
	memset( F212A, '\0', sizeof(F212A) );
	memset( F212B, '\0', sizeof(F212B) );
	memset( F212C, '\0', sizeof(F212C) );
	memset( F213A, '\0', sizeof(F213A) );
	memset( F213B, '\0', sizeof(F213B) );
	memset( F213C, '\0', sizeof(F213C) );
	memset( F215A, '\0', sizeof(F215A) );
	memset( F215B, '\0', sizeof(F215B) );
	memset( F215C, '\0', sizeof(F215C) );
	memset( F216A, '\0', sizeof(F216A) );
	memset( F216B, '\0', sizeof(F216B) );
	memset( F216C, '\0', sizeof(F216C) );
	memset( F215AEX, '\0', sizeof(F215AEX) );
	memset( F215BEX, '\0', sizeof(F215BEX) );
	memset( F215CEX, '\0', sizeof(F215CEX) );
	memset( F216AEX, '\0', sizeof(F216AEX) );
	memset( F216BEX, '\0', sizeof(F216BEX) );
	memset( F216CEX, '\0', sizeof(F216CEX) );
	memset( F217A, '\0', sizeof(F217A) );
	memset( F217B, '\0', sizeof(F217B) );
	memset( F217C, '\0', sizeof(F217C) );
	memset( F218A, '\0', sizeof(F218A) );
	memset( F218B, '\0', sizeof(F218B) );
	memset( F218C, '\0', sizeof(F218C) );
	memset( F219A, '\0', sizeof(F219A) );
	memset( F219B, '\0', sizeof(F219B) );
	memset( F219C, '\0', sizeof(F219C) );
	memset( F220A, '\0', sizeof(F220A) );
	memset( F220B, '\0', sizeof(F220B) );
	memset( F220C, '\0', sizeof(F220C) );
	memset( F221A, '\0', sizeof(F221A) );
	memset( F221B, '\0', sizeof(F221B) );
	memset( F221C, '\0', sizeof(F221C) );
	memset( F222A, '\0', sizeof(F222A) );
	memset( F222B, '\0', sizeof(F222B) );
	memset( F222C, '\0', sizeof(F222C) );
	memset( F223A, '\0', sizeof(F223A) );
	memset( F223B, '\0', sizeof(F223B) );
	memset( F223C, '\0', sizeof(F223C) );
	memset( FS23A, '\0', sizeof(FS23A) );
	memset( FS23B, '\0', sizeof(FS23B) );
	memset( FS23C, '\0', sizeof(FS23C) );

	F2UWS = 0;
	F2UWSEX = 0;
	F217c = F218c = F221c = 0;
	F29c = F211c = 0;

	m_curwnd = -1;
}

CShinFhyo20::~CShinFhyo20()
{
}

void CShinFhyo20::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo20Diag02);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo20Diag01);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo20, CSyzBaseDlg)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo20::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo20::OnBnClickedButton2)
END_MESSAGE_MAP()


// CShinFhyo20 メッセージ ハンドラ

BOOL CShinFhyo20::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo20Diag02;
	m_BottomIdx = 58;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinFhyo20::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CWnd *pWnd = GetFocus();

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			GetAllDiagData();
			f2calq_all( 1 );
			WriteData( 1 );

			// 付表１のクラス作成
/*			CShinFhy10 *pShinFhy10	= new CShinFhy10;
			pShinFhy10->fh1_calq( 1 );
			delete( pShinFhy10 );
			CWnd*	p1	=	GetParent();
			CWnd*	p2	=	p1->GetParent();
			((CSyzShinSheet*)p2)->Ippan_calq( 0 );*/

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		//-->追加 harino ('14.03.04)
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1 && (pWnd != GetDlgItem(IDC_BUTTON1)) && (pWnd != GetDlgItem(IDC_BUTTON2))) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
					TerminationIcsdiagctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
		//<--追加 harino ('14.03.04)

		if(pWnd == GetDlgItem(IDC_BUTTON1)){
			if( (pMsg->wParam==VK_F3) || (pMsg->wParam==VK_RIGHT) || (pMsg->wParam==VK_DOWN) || (pMsg->wParam==VK_TAB)){
				if(  *m_pShinInfo->pOver500MillionSw != 1 &&  F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	//[Y]5億円以下かつ 課税売上割合が９５％以上の場合
					diag_setposition( IDC_ICSDIAGCTRL2, ID219A, CIcsdiagctrl );
				}
				else{
					if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//非連動の場合
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID215A, CIcsdiagctrl );
						}
						else{				
							diag_setposition( IDC_ICSDIAGCTRL2, ID218A, CIcsdiagctrl );
						}
					}
					else{
						diag_setposition( IDC_ICSDIAGCTRL2, ID219A, CIcsdiagctrl );
					}
				}
				return TRUE;

			}
			// 前項目へ
			if( (pMsg->wParam==VK_LEFT) || (pMsg->wParam==VK_F2) || (pMsg->wParam==VK_UP) ){
				//[Y]
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					diag_setposition( IDC_ICSDIAGCTRL2, ID211A, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL2, ID220A, CIcsdiagctrl );
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

HBRUSH CShinFhyo20::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinFhyo20::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CShinFhyo20::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo20Data	：	付表２データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSnTanaData		：	棚卸調整データ読込みクラス
//		pSnFhyo10Data	：	付表１データ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo20::InitInfo( CH26SnFhyo20Data **pSnFhyo20Data, CSnHeadData **pSnHeadData, CH26SnTanaData **pSnTanaData, CH26SnFhyo10Data **pSnFhyo10Data, CH26SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
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

	ASSERT( pSnFhyo10Data );
	if( pSnFhyo10Data == NULL ){
		return -1;
	}
	m_pSnFhyo10Data = pSnFhyo10Data;

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
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo20::Init()
{
//--> '14.05.16 INS START
	if( ((*m_pSnFhyo20Data)==NULL) || ((*m_pSnTanaData)==NULL) || ((*m_pSnFhyo10Data)==NULL) ){
		return -1;
	}
//<-- '14.05.16 INS END

	ReadData( m_FrSw );
	m_FrSw = 1;

	InitDiagAttr();

	DispFh2();

	ChangeDispByApno();

	// 課税標準率Ｂ欄へ
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//		diag_setposition( IDC_ICSDIAGCTRL2, ID21A, CIcsdiagctrl );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 業種による画面表示の変更
//-----------------------------------------------------------------------------
void CShinFhyo20::ChangeDispByApno()
{
	if( ((*m_pSnHeadData)->m_apno&0xf0) >= 0x50 ){	// 非営利法人
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_Button1.EnableWindow( FALSE );
//--> '15.04.25 CUT START
//			m_Button2.EnableWindow( FALSE );
//<-- '15.04.25 CUT END
		}
		else	{
			m_Button1.EnableWindow( TRUE );
//-- '15.04.04 --
//			if( m_pShinInfo->sgSpc && !((*m_pSnHeadData)->Sn_Sign2&0x02) ){
//---------------
			if( *m_pShinInfo->pSgSpc && !((*m_pSnHeadData)->Sn_Sign2&0x02) ){
//---------------
				m_Button2.ShowWindow( SW_SHOW );
			}
			else{
				m_Button2.ShowWindow( SW_HIDE );
			}
		}
	}
	else{
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_Button1.EnableWindow( FALSE );
		}
		else{
			m_Button1.EnableWindow( TRUE );
		}
//--> '15.04.25 INS START
		m_Button2.EnableWindow( FALSE );
//<-- '15.04.25 INS END
		m_Button2.ShowWindow( SW_HIDE );
	}
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数	pas		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo20::ReadData( int pas )
{
	ASSERT( m_pSnFhyo20Data );

	F29c = 0;

	// 課税標準額
	memmove( F21A,  (*m_pSnFhyo20Data)->Sn_2F1A, MONY_BUF_SIZE );
	memmove( F21B,  (*m_pSnFhyo20Data)->Sn_2F1B, MONY_BUF_SIZE );
	memmove( F21C,  (*m_pSnFhyo20Data)->Sn_2F1C, MONY_BUF_SIZE );
	// 免税売上額
	memmove( F22C,  (*m_pSnFhyo20Data)->Sn_2F2C, MONY_BUF_SIZE );
	// 非課税資産の輸出
	memmove( F23C,  (*m_pSnFhyo20Data)->Sn_2F3C, MONY_BUF_SIZE );
	// 非課税売上額
	memmove( F26C,  (*m_pSnFhyo20Data)->Sn_2F6C, MONY_BUF_SIZE );
	// 課税売上割合
	F2UWS = (*m_pSnFhyo20Data)->Sn_2FUWS;
	// 課税仕入に係る支払対価の額
	memmove( F28A,  (*m_pSnFhyo20Data)->Sn_2F8A, MONY_BUF_SIZE );
	memmove( F28B,  (*m_pSnFhyo20Data)->Sn_2F8B, MONY_BUF_SIZE );
	memmove( F28C,  (*m_pSnFhyo20Data)->Sn_2F8C, MONY_BUF_SIZE );
	// 課税仕入に係る消費税額
	memmove( F29A,  (*m_pSnFhyo20Data)->Sn_2F9A, MONY_BUF_SIZE );
	memmove( F29B,  (*m_pSnFhyo20Data)->Sn_2F9B, MONY_BUF_SIZE );
	memmove( F29C,  (*m_pSnFhyo20Data)->Sn_2F9C, MONY_BUF_SIZE );

	F29c = (*m_pSnFhyo20Data)->Sn_2F9sw;//[Y]

	// 課税貨物に係る支払対価の額
//	memmove( F210A, (*m_pSnFhyo20Data)->Sn_2F10A, MONY_BUF_SIZE );
//	memmove( F210B, (*m_pSnFhyo20Data)->Sn_2F10B, MONY_BUF_SIZE );
//	memmove( F210C, (*m_pSnFhyo20Data)->Sn_2F10C, MONY_BUF_SIZE );
	// 課税貨物に係る消費税額
	memmove( F211A, (*m_pSnFhyo20Data)->Sn_2F10A, MONY_BUF_SIZE );	
	memmove( F211B, (*m_pSnFhyo20Data)->Sn_2F10B, MONY_BUF_SIZE );
	memmove( F211C, (*m_pSnFhyo20Data)->Sn_2F10C, MONY_BUF_SIZE );

	//(免税)控除対象調整税額
	if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){//個別対応時
			char	WORK0[MONY_BUF_SIZE] = {0};
			m_pArith->l_add( F212A , (*m_pSnTanaData)->Sn_menzz[0] , (*m_pSnTanaData)->Sn_menzz[1] );
			m_pArith->l_add( F212A , F212A , (*m_pSnTanaData)->Sn_menzz[2] );
			m_pArith->l_add( F212B , (*m_pSnTanaData)->Sn_mnz40[0] , (*m_pSnTanaData)->Sn_mnz40[1] );
			m_pArith->l_add( F212B , F212B , (*m_pSnTanaData)->Sn_mnz40[2] );
			m_pArith->l_add( F212C , (*m_pSnTanaData)->Sn_mnz63[0] , (*m_pSnTanaData)->Sn_mnz63[1] );
			m_pArith->l_add( F212C , F212C , (*m_pSnTanaData)->Sn_mnz63[2] );

		}
		else{//比例配分
			memmove( F212A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
			memmove( F212B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
			memmove( F212C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
		}

		if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
			m_pArith->l_neg( F212A );
			m_pArith->l_neg( F212B );
			m_pArith->l_neg( F212C );//[Y]
		}
	}
	else{
		memset( F212A, '\0', MONY_BUF_SIZE );
		memset( F212B, '\0', MONY_BUF_SIZE );
		memset( F212C, '\0', MONY_BUF_SIZE );
	}
	// 課税売上に係る仕入税額
	memmove( F215A, (*m_pSnFhyo20Data)->Sn_2F15A, MONY_BUF_SIZE );
	memmove( F215B, (*m_pSnFhyo20Data)->Sn_2F15B, MONY_BUF_SIZE );
	memmove( F215C, (*m_pSnFhyo20Data)->Sn_2F15C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( F216A, (*m_pSnFhyo20Data)->Sn_2F16A, MONY_BUF_SIZE );
	memmove( F216B, (*m_pSnFhyo20Data)->Sn_2F16B, MONY_BUF_SIZE );
	memmove( F216C, (*m_pSnFhyo20Data)->Sn_2F16C, MONY_BUF_SIZE );

	// 課税売上に係る仕入税額
	memmove( F215AEX, (*m_pSnFhyo20Data)->Sn_2F15A, MONY_BUF_SIZE );
	memmove( F215BEX, (*m_pSnFhyo20Data)->Sn_2F15B, MONY_BUF_SIZE );
	memmove( F215CEX, (*m_pSnFhyo20Data)->Sn_2F15C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( F216AEX, (*m_pSnFhyo20Data)->Sn_2F16A, MONY_BUF_SIZE );
	memmove( F216BEX, (*m_pSnFhyo20Data)->Sn_2F16B, MONY_BUF_SIZE );
	memmove( F216CEX, (*m_pSnFhyo20Data)->Sn_2F16C, MONY_BUF_SIZE );

	F217c = F218c = F221c = 0x00;
	// 個別対応方式控除仕入税額
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F17A ) ){
		memmove( F217A, (*m_pSnFhyo20Data)->Sn_2F17A, MONY_BUF_SIZE );
		F217c |= 0x10;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F17B ) ){
		memmove( F217B, (*m_pSnFhyo20Data)->Sn_2F17B, MONY_BUF_SIZE );
		F217c |= 0x01;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F17C ) ){
		memmove( F217C, (*m_pSnFhyo20Data)->Sn_2F17C, MONY_BUF_SIZE );
		F217c |= 0x100;
	}
	// 比例配分方式控除仕入税額
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F18A ) ){
		memmove( F218A, (*m_pSnFhyo20Data)->Sn_2F18A, MONY_BUF_SIZE );
		F218c |= 0x10;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F18B ) ){
		memmove( F218B, (*m_pSnFhyo20Data)->Sn_2F18B, MONY_BUF_SIZE );
		F218c |= 0x01;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F18C ) ){
		memmove( F218C, (*m_pSnFhyo20Data)->Sn_2F18C, MONY_BUF_SIZE );
		F218c |= 0x100;
	}
	// 調整対象固定資産に係る調整税額
	memmove( F219A, (*m_pSnFhyo20Data)->Sn_2F19A, MONY_BUF_SIZE );
	memmove( F219B, (*m_pSnFhyo20Data)->Sn_2F19B, MONY_BUF_SIZE );
	memmove( F219C, (*m_pSnFhyo20Data)->Sn_2F19C, MONY_BUF_SIZE );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( F220A, (*m_pSnFhyo20Data)->Sn_2F20A, MONY_BUF_SIZE );
	memmove( F220B, (*m_pSnFhyo20Data)->Sn_2F20B, MONY_BUF_SIZE );
	memmove( F220C, (*m_pSnFhyo20Data)->Sn_2F20C, MONY_BUF_SIZE );

	// 控除対象仕入税額
	F221c = (*m_pSnFhyo20Data)->Sn_2F21sw;
	if( F221c & 0x01 ){			// 手入力有り
		memmove( F221A, (*m_pSnFhyo20Data)->Sn_2F21A, MONY_BUF_SIZE );
	}
	else{
		memmove( F221A, (*m_pSnFhyo10Data)->Sn_1F4A, MONY_BUF_SIZE );	// 手入力無し
	}

	if( F221c & 0x10 ){			// 手入力有り
		memmove( F221B, (*m_pSnFhyo20Data)->Sn_2F21B, MONY_BUF_SIZE );
	}
	else{
		memmove( F221B, (*m_pSnFhyo10Data)->Sn_1F4B, MONY_BUF_SIZE );	// 手入力無し
	}
	if( F221c & 0x100 ){			// 手入力有り
		memmove( F221C, (*m_pSnFhyo20Data)->Sn_2F21C, MONY_BUF_SIZE );
	}
	else{
		memmove( F221C, (*m_pSnFhyo10Data)->Sn_1F4C, MONY_BUF_SIZE );	// 手入力無し
	}
	// 貸倒回収に係る税額
	if( pas == 1 ){
		memmove( FS23A, (*m_pSnFhyo20Data)->Sn_2F23A, MONY_BUF_SIZE );
		memmove( FS23B, (*m_pSnFhyo20Data)->Sn_2F23B, MONY_BUF_SIZE );
		memmove( FS23C, (*m_pSnFhyo20Data)->Sn_2F23C, MONY_BUF_SIZE );
	}
	memmove( F223A, (*m_pSnFhyo20Data)->Sn_2F23A, MONY_BUF_SIZE );
	memmove( F223B, (*m_pSnFhyo20Data)->Sn_2F23B, MONY_BUF_SIZE );
	memmove( F223C, (*m_pSnFhyo20Data)->Sn_2F23C, MONY_BUF_SIZE );

	// 控除過大調整税額	･ 貸倒回収に係る消費税額
	m_pArith->l_sub( F222A, (*m_pSnFhyo10Data)->Sn_1F3A, F223A );
	m_pArith->l_sub( F222B, (*m_pSnFhyo10Data)->Sn_1F3B, F223B );
	m_pArith->l_sub( F222C, (*m_pSnFhyo10Data)->Sn_1F3C, F223C );

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 引数	pas		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo20::WriteData( int pas )
{
	int	st = 0;

	// 課税標準額
	memmove( (*m_pSnFhyo20Data)->Sn_2F1A, F21A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F1B, F21B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F1C, F21C,  MONY_BUF_SIZE );
	// 免税売上額
	memmove( (*m_pSnFhyo20Data)->Sn_2F2A, F22A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F2B, F22B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F2C, F22C,  MONY_BUF_SIZE );
	// 非課税資産の輸出
	memmove( (*m_pSnFhyo20Data)->Sn_2F3A, F23A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F3B, F23B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F3C, F23C,  MONY_BUF_SIZE );
	// 課税資産の譲渡等の対価の額
	memmove( (*m_pSnHonpyoData)->Sn_KZURI, F24C, MONY_BUF_SIZE );
	// 非課税売上額
	memmove( (*m_pSnFhyo20Data)->Sn_2F6A, F26A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F6B, F26B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F6C, F26C,  MONY_BUF_SIZE );
	// 資産の譲渡等の対価の額
	memmove( (*m_pSnHonpyoData)->Sn_SOURI, F27C, MONY_BUF_SIZE );
	// 課税売上割合
	(*m_pSnHeadData)->Sn_UPERS &= 0xfe;
	if( ((*m_pSnFhyo20Data)->Sn_2FUWS = F2UWS) < (*m_pSnHeadData)->m_UriRatioBorder ){
		(*m_pSnHeadData)->Sn_UPERS |= 0x01;
	}
	// 課税仕入に係る支払対価の額
	memmove( (*m_pSnFhyo20Data)->Sn_2F8A, F28A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F8B, F28B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F8C, F28C,  MONY_BUF_SIZE );
	// 課税仕入に係る消費税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F9A, F29A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F9B, F29B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F9C, F29C,  MONY_BUF_SIZE );

	(*m_pSnFhyo20Data)->Sn_2F9sw = F29c;//[Y]

	// 課税貨物に係る支払対価の額
//	memmove( (*m_pSnFhyo20Data)->Sn_2F10A, F210A, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo20Data)->Sn_2F10B, F210B, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo20Data)->Sn_2F10C, F210C, MONY_BUF_SIZE );
	// 課税貨物に係る消費税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F10A, F211A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F10B, F211B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F10C, F211C, MONY_BUF_SIZE );
	// 課税売上に係る仕入税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F15A, F215A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F15B, F215B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F15C, F215C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F16A, F216A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F16B, F216B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F16C, F216C, MONY_BUF_SIZE );
	// 個別対応方式控除仕入税額
	memset( (*m_pSnFhyo20Data)->Sn_2F17A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F17B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F17C, '\0', MONY_BUF_SIZE );
	if( F217c & 0x10 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F17A, F217A, MONY_BUF_SIZE );
	}
	if( F217c & 0x01 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F17B, F217B, MONY_BUF_SIZE );
	}
	if( F217c & 0x100 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F17C, F217C, MONY_BUF_SIZE );
	}
	// 比例配分方式控除仕入税額
	memset( (*m_pSnFhyo20Data)->Sn_2F18A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F18B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F18C, '\0', MONY_BUF_SIZE );
	if( F218c & 0x10 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F18A, F218A, MONY_BUF_SIZE );
	}
	if( F218c & 0x01 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F18B, F218B, MONY_BUF_SIZE );
	}
	if( F218c & 0x100 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F18C, F218C, MONY_BUF_SIZE );
	}
	// 調整対象固定資産に係る調整税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F19A, F219A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F19B, F219B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F19C, F219C, MONY_BUF_SIZE );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( (*m_pSnFhyo20Data)->Sn_2F20A, F220A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F20B, F220B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F20C, F220C, MONY_BUF_SIZE );
	// 控除対象仕入税額
	if( F221c & 0x01 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F21A, F221A, MONY_BUF_SIZE );
	}
	else{
		memset( (*m_pSnFhyo20Data)->Sn_2F21A, '\0', MONY_BUF_SIZE );
	}
	if( F221c & 0x10 ){
		if( m_pArith->l_test( F221B ) ){
			memmove( (*m_pSnFhyo20Data)->Sn_2F21B, F221B, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo20Data)->Sn_2F21B, '\0', MONY_BUF_SIZE );
	}
	if( F221c & 0x100 ){
		if( m_pArith->l_test( F221C ) ){
			memmove( (*m_pSnFhyo20Data)->Sn_2F21C, F221C, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo20Data)->Sn_2F21C, '\0', MONY_BUF_SIZE );
	}
	(*m_pSnFhyo20Data)->Sn_2F21sw = F221c;

	memmove( (*m_pSnFhyo10Data)->Sn_1F4A, F221A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4B, F221B, MONY_BUF_SIZE );
//	m_pArith->l_add( (*m_pSnFhyo20Data)->Sn_1F8C, F221A, F221B );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4C, F221C, MONY_BUF_SIZE );
	// 控除過大調整税額	･ 貸倒回収に係る消費税額
	if( (pas==1) && (l_cmp(FS23A, F223A)||l_cmp(FS23B, F223B)) ){
		st = 1;
	}
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3A, F222A, F223A );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3B, F222B, F223B );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3C, F222C, F223C );
	memmove( (*m_pSnFhyo20Data)->Sn_2F23A, F223A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F23B, F223B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F23C, F223C, MONY_BUF_SIZE );
	
	return st;
}

//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo20::DispFh2()
{
	char				buf[512] = {0};
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	// 画面の初期化
	for( int i = 84;i <= 110; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
		//手入力サイン初期化
		F29c = 0;
		F217c = 0;
		F218c = 0;
//-- '15.03.09 --
//		F221c = 0;
//---------------
		if( ((CH26HyoView*)m_pParent)->IsSpcDataLinked() == FALSE ){
			F221c = 0;
		}
//---------------
	}

	// 期間・氏名の表示
	char	yy=0, mm=0, dd=0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof( buf ), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 84, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 85, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( buf, '\0', sizeof( buf ) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, &(*m_pSnHeadData)->Sn_DIHYO[0], sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, &(*m_pSnHeadData)->Sn_CONAM[0], sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 86, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税売上高の５億超えチェック
	char	ValKur[6] = {0};
	char	ValCnv[6] = {0};
	m_pArith->l_add( ValKur, F21C, F22C );
	(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
	ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
	ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
	diag_clear( IDC_ICSDIAGCTRL1, 3, TRUE, CIcsdiagctrl );
/*- '14.06.10 -*/
//	*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
//	CString	hoge;
//	hoge.Format( _T("stno:%d, dtno:%d"), m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
//	AfxMessageBox( hoge );
	*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
//	hoge.Format( _T("500Million：%d"), *m_pShinInfo->pOver500MillionSw );
//	AfxMessageBox( hoge );
//	char	moji[64]={0};
//	m_pArith->l_print( moji, ValCnv, _T("sss,sss,sss,sss,ss9") );
//	AfxMessageBox( moji );
/*-------------*/
	if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//		memset( pSyzShin->RatioAsc, '\0', 6 );
//		strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "11000" );
		(*m_pSnHeadData)->m_UriRatioBorder = 11000;
		(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );			//4 );
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );			//4 );
			memset( buf, '\0', sizeof( buf ) );
			if( m_pArith->l_cmp( ValKur, ValCnv ) == 0 ){ 
				strcpy_s( buf, sizeof( buf ), _T("課税売上高が、５億円を超えています。\n") );
			}
			else{
				strcpy_s( buf, sizeof( buf ), _T("課税売上高が、年換算で５億円を超えています。\n") );
			}
			strcat_s( buf, sizeof( buf ), _T("比例配分方式の選択も含めて注意してください。") );
			DiagData.data_disp = (char *)buf;
			diag_setdata( IDC_ICSDIAGCTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
	}
	else{			// ４億５千万以上で５億以下又は４億５千万未満
//		memset( pSyzShin->RatioAsc, '\0', 6 );
//		strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
		(*m_pSnHeadData)->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
			diag_clear( IDC_ICSDIAGCTRL1, 3, TRUE, CIcsdiagctrl );
		}
	}
	m_Fhyo20Diag01.Refresh();
	m_Fhyo20Diag02.Refresh();

	// 課税標準額
	dsp_prs( IDC_ICSDIAGCTRL2,  ID21A, F21C );
	// 免税売上額
	dsp_prs( IDC_ICSDIAGCTRL2,  ID22A, F22C );
	// 非課税資産の輸出
	dsp_prs( IDC_ICSDIAGCTRL2,  ID23A, F23C );
	// 課税資産の譲渡等の対価の額
	f2calq_45();
	// 非課税売上額
	dsp_prs( IDC_ICSDIAGCTRL2,  ID26A,  F26C );
	// 資産の譲渡等の対価の額 ･ 課税売上割合
	f2calq_7();
	// 課税仕入に係る支払対価の額
	dsp_prs( IDC_ICSDIAGCTRL2, ID28A, F28C );
	// 課税仕入に係る消費税額
	f2calq_9();
	// 課税貨物に係る消費税額
	f2calq_11();
	// (免税)控除対象調整税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID212A, F212C );
	// 課税仕入等の税額の合計額
	f2calq_13();
	if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
		// 課税売上割合に準ずる割合を有効サイン
		f2calq_ko();		// 課税売上割合が９５％未満
	}
	else{
		// 課税売上割合が９５％以上
		if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
			f2calq_14();
		}
		// 課税売上割合が９５％未満
		else{
			f2calq_ko();
		}
	}
//--> '15.04.04 INS START
	// 特定収入を加味した控除対象仕入税額の計算
	f2calq_spc();
//<-- '15.04.04 INS END
	// 調整対象固定資産に係る調整税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID219A, F219C );
	// 調整対象固定資産を課税業務用に転用した調整額
	dsp_prs( IDC_ICSDIAGCTRL2, ID220A, F220C );
	// 差引控除税額
	f2calq_2122();
	// 貸倒回収に係る消費税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID223A, F223C );
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo20::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ){
		case ID21A :		//  1. 課税売上額
		case ID22A :		//  2. 免税売上額
		case ID23A :		//  3. 非課税資産の輸出金額	
		case ID26A :		//	6. 非課税売上額
		case ID28A :		//	8. 課税仕入に係る支払対価の額
		case ID29A :		//	9. 課税仕入に係る消費税額
		case ID211A:		// 11. 課税貨物に係る消費税額
		case ID212A:		// 12. 調整税額
		case ID215A:		// 15. 課税売上に係る仕入税額
		case ID216A:		// 16. 共通の売上に係る仕入税額
		case ID217A:		// 17. 個別対応方式控除仕入税額
		case ID218A:		// 18. 比例配分方式控除仕入税額
		case ID219A:		// 19. 調整対象固定資産に係る調整税額
		case ID220A:		// 20. 調整対象固定資産を課税業務用に転用した調整額
		case ID221A:		// 21. 差引控除対象仕入税額
		case ID223A:		// 23. 貸倒回収に係る消費税額
			memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
		default		:
			memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinFhyo20::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ){
		case 84 :		// 課税期間　自
		case 85 :		// 課税期間　至
		case 86 :		// 氏名又は､名称
		case ID2EX :	// 課税売上割合
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default :
			memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo20::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;

	if( m_Fhyo20Diag02.GetPosition() == ID28A ){
		m_Fhyo20Diag01.SetFocus();
	}

	//  1. 課税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID21A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F21C, DiagData.data_val, MONY_BUF_SIZE ); 
	//  2. 免税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID22A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F22C, DiagData.data_val, MONY_BUF_SIZE ); 
	//  3. 非課税資産の輸出金額
	diag_getdata( IDC_ICSDIAGCTRL2, ID23A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F23C, DiagData.data_val, MONY_BUF_SIZE ); 
	//	6. 非課税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID26A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F26C, DiagData.data_val, MONY_BUF_SIZE ); 
	//	8. 課税仕入に係る支払対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID28A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F28C, DiagData.data_val, MONY_BUF_SIZE ); 
	//	9. 課税仕入に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID29A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F29C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 11. 課税貨物に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID211A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F211C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 15. 課税売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID215A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F215C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 16. 共通の売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID216A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F216C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 17. 個別対応方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID217A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F217C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 18. 比例配分方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID218A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F218C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 19. 調整対象固定資産に係る調整税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID219A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F219C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 20. 調整対象固定資産を課税業務用に転用した調整額
	diag_getdata( IDC_ICSDIAGCTRL2, ID220A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F220C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 23. 貸倒回収に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID223A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F223C, DiagData.data_val, MONY_BUF_SIZE ); 
}

//-----------------------------------------------------------------------------
// 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo20::ChangeColor( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, Index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :	// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 :	// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3 :	// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4 :	// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5 :	// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:	// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;
			break;
	}
	diag_setattr( id, Index, &DA, FALSE, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo20::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo2[cnt].Index == 0 ){
			break;
		}
	
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo2[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo2[cnt].UnConnect;
			}
			else{
				Flg = Fhyo2[cnt].Connect;
			}
		}
//-- '15.04.04 --
//		if( ((*m_pSnHeadData)->Sn_EXP100&0x01) && m_pShinInfo->sgSpc && (Fhyo2[cnt].Index==ID27EX) ){
//---------------
		if( ((*m_pSnHeadData)->Sn_EXP100&0x01) && *m_pShinInfo->pSgSpc && (Fhyo2[cnt].Index==ID27EX) ){
//---------------
			Flg = 0;
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Fhyo2[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo2[cnt].Connect );
		cnt++;
	}


	m_Fhyo20Diag02.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定


	return 0;
}

//-----------------------------------------------------------------------------
// ポジションセット
//-----------------------------------------------------------------------------
// 引数	Index		：	ダイアグラム内インデックス
//		isScroll	：	スクロールするかどうか
//-----------------------------------------------------------------------------
void CShinFhyo20::SetItemPosition( int Index, BOOL isScroll/*=FALSE*/ )
{
	int	setIdx;

	if( Index ){
		setIdx = Index;
	}
	else{
		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
			setIdx = ID21A;
		}
		else{
			setIdx = ID219A;
		}
	}

	m_Fhyo20Diag01.SetFocus();
	// 確定時は全入力不可
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return;
	}

	m_Fhyo20Diag01.SetPosition( setIdx );

	if( isScroll ){
		CRect	rc;
		this->GetClientRect( &rc );
		this->InvalidateRect( &rc );
		SetRedrawScroll( 0 );
	}
}

BEGIN_EVENTSINK_MAP(CShinFhyo20, CSyzBaseDlg)
	ON_EVENT(CShinFhyo20, IDC_ICSDIAGCTRL2, 1, CShinFhyo20::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo20, IDC_ICSDIAGCTRL2, 2, CShinFhyo20::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo20, IDC_ICSDIAGCTRL2, 3, CShinFhyo20::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinFhyo20, IDC_ICSDIAGCTRL2, 9, CShinFhyo20::CheckButtonIcsdiagctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo20::EditONIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	RECT	a, b;
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rc1, rc2;
	//GetWindowRect( &rc1 );
	//diag_getrect( IDC_ICSDIAGCTRL2, index, &rc2, CIcsdiagctrl );

	//if( (rc1.top>rc2.top) || (rc1.bottom<rc2.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rc2.top-rc1.top)-((rc1.bottom-rc1.top)/2) );
	//}
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL2;

	switch( index ){
		case ID212A :
			if( m_DlSw ){
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					// 課税売上割合に準ずる割合を有効
					diag_setposition( IDC_ICSDIAGCTRL2, ID215A, CIcsdiagctrl );
				}
				else{
					if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){		// 課税売上高割合が95%以上の場合
						diag_setposition( IDC_ICSDIAGCTRL2, ID219A, CIcsdiagctrl );
					}
					else{				// 課税売上高割合が95%未満の場合	
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID215A, CIcsdiagctrl );
						}
						else{										// 比例配分方式
							diag_setposition( IDC_ICSDIAGCTRL2, ID218A, CIcsdiagctrl );
						}
					}
				}
			}
			else{
				SetItemPosition( ID211A );
			}

			// 次入力項目　設定
			if( !m_DlSw ){
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					// 課税売上割合に準ずる割合を有効
					diag_setposition( IDC_ICSDIAGCTRL2, ID215A, CIcsdiagctrl );
				}
				else{
					if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){		// 課税売上高割合が95%以上の場合
						diag_setposition( IDC_ICSDIAGCTRL2, ID219A, CIcsdiagctrl );
					}
					else{												// 課税売上高割合が95%未満の場合	
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){		// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID215A, CIcsdiagctrl );
						}
						else{			// 比例配分方式
							diag_setposition( IDC_ICSDIAGCTRL2, ID218A, CIcsdiagctrl );
						}
					}
				}
			}
			else	{
				SetItemPosition( ID211A );
			}
			break;
		case ID219A :
		case ID215A :
		case ID218A :
			break;
		default      :
			break;

	}
}

void CShinFhyo20::EditOFFIcsdiagctrl2( short index )
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	int				length;
	char			CkMy[MONY_BUF_SIZE] = {0};
	char			buf[512] = {0};

	//m_curwnd = -1;	// 削除 harino ('14.03.07)

	// 12. 調整税額
	if( index != ID212A ){
		if( index > ID212A ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}
	
	char	ValKur[6] = {0};
	char	ValCnv[6] = {0};
	switch( index ){

		case ID21A	:		//  1. 課税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID21A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, F21C, MONY_BUF_SIZE );
			memmove( F21C, DiagData.data_val, MONY_BUF_SIZE );
			//if( m_pArith->l_cmp(CkMy, F21C) ){	// End時に正しく判定できないため削除 harino ('14.03.12)
				// 課税売上高の５億超えチェック
				m_pArith->l_add( ValKur, F21C, F22C );
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
/*- '14.06.10 -*/
//				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//					memset( pSyzShin->RatioAsc, '\0', 6 );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "11000" );
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F215A, F215AEX, MONY_BUF_SIZE );
						memmove( F215B, F215BEX, MONY_BUF_SIZE );
						memmove( F215C, F215CEX, MONY_BUF_SIZE );
						// 共通の売上に係る仕入税額
						memmove( F216A, F216AEX, MONY_BUF_SIZE );
						memmove( F216B, F216BEX, MONY_BUF_SIZE );
						memmove( F216C, F216CEX, MONY_BUF_SIZE );
					}
					(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );
						memset( buf, '\0', sizeof( buf ) );
						if( m_pArith->l_cmp(ValKur, ValCnv) == 0 ){
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、５億円を超えています。\n") );
						}
						else{
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、年換算で５億円を超えています。\n") );
						}
						strcat_s( buf, sizeof( buf ), "比例配分方式の選択も含めて注意してください。" );
						DiagData.data_disp = (char *)buf;
						diag_setdata( IDC_ICSDIAGCTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					}
				}
				else{			// ４億５千万以上で５億以下又は４億５千万未満
//					memset( pSyzShin->RatioAsc, '\0', 6 );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
					(*m_pSnHeadData)->m_UriRatioBorder = 9500;
					*m_pShinInfo->pOver500MillionSw = 0;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );
						diag_clear( IDC_ICSDIAGCTRL1, 3, TRUE, CIcsdiagctrl );
					}

				}
				m_Fhyo20Diag01.Refresh();
				m_Fhyo20Diag02.Refresh();
			//}
			// 各入力の計算表示
			f2calq_all( 1 );
			break;

		case ID22A	:		//  2. 免税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID22A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, F22C, MONY_BUF_SIZE );
			memmove( F22C, DiagData.data_val, MONY_BUF_SIZE );
			//if( m_pArith->l_cmp(CkMy, F22C) ){	// End時に正しく判定できないため削除 harino ('14.03.12)
				// 課税売上高の５億超えチェック
				m_pArith->l_add( ValKur, F21C, F22C );
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
/*- '14.06.10 -*/
//				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//					memset( pSyzShin->RatioAsc, '\0', 6 );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "11000" );
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F215A, F215AEX, MONY_BUF_SIZE );
						memmove( F215B, F215BEX, MONY_BUF_SIZE );
						memmove( F215C, F215CEX, MONY_BUF_SIZE );
						// 共通の売上に係る仕入税額
						memmove( F216A, F216AEX, MONY_BUF_SIZE );
						memmove( F216B, F216BEX, MONY_BUF_SIZE );
						memmove( F216C, F216CEX, MONY_BUF_SIZE );
					}
					(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );
						memset( buf, '\0', sizeof( buf ) );
						if( m_pArith->l_cmp(ValKur, ValCnv) == 0 ){
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、５億円を超えています。\n") );
						}
						else{
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、年換算で５億円を超えています。\n") );
						}
						strcat_s( buf, sizeof( buf ), _T("比例配分方式の選択も含めて注意してください。") );
						DiagData.data_disp = (char *)buf;
						diag_setdata( IDC_ICSDIAGCTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					}
				}
				else{			// ４億５千万以上で５億以下又は４億５千万未満
//					memset( pSyzShin->RatioAsc, '\0', 6 );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
					(*m_pSnHeadData)->m_UriRatioBorder = 9500;
					*m_pShinInfo->pOver500MillionSw = 0;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );
						diag_clear( IDC_ICSDIAGCTRL1, 3, TRUE, CIcsdiagctrl );
					}

				}
				m_Fhyo20Diag01.Refresh();
				m_Fhyo20Diag02.Refresh();
			//}
			// 各入力の計算表示
			f2calq_all( 2 );
			break;

		case ID23A	:		//  3. 非課税資産の輸出金額
			diag_getdata( IDC_ICSDIAGCTRL2, ID23A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F23C, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 3 );
			break;

		case ID26A	:		//	6. 非課税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID26A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F26C, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 4 );
			break;

		case ID28A	:		//	8. 課税仕入に係る支払対価の額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F28C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID28A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F28C, CkMy) ){
				if(!(F29c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F29c |= 0x100;
				}
//				F29c |= 0x01;
			}
			memmove( F28C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 5 );
			break;

		case ID29A	:		//	9. 課税仕入に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F29C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID29A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F29C, CkMy) ){
				F29c |= 0x800;
				F29c &= ~0x100;
			}
			memmove( F29C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 6 );
			break;

		case ID211A :		// 10. 課税貨物に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F211C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID211A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F211C, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F211C, CkMy) ){
				if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()&&(F2UWS<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
					CString		Msg;
					Msg.Empty();
					Msg =  "課税貨物に係る消費税額が変更されました。\n";
					Msg += "個別対応方式欄の⑭～⑯の金額を見直してください。";
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}
			// 各入力の計算表示
			f2calq_all( 7 );
			break;

		case ID212A	:		// 11. 調整税額
			break;

		case ID215A :		// 14. 課税売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID215A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F215C, DiagData.data_val, MONY_BUF_SIZE );
			memmove( F215CEX, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 8 );
			break;

		case ID216A :		// 15. 共通の売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID216A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F216C, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F216CEX, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 9 );
			break;

		case ID217A :		// 16. 個別対応方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F217C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID217A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_test(CkMy) ){
				if( m_pArith->l_cmp(F217C, CkMy) ){
					F217c |= 0x100;
				}
			}
			memmove( F217C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 10 );
			break;

		case ID218A:		// 17. 比例配分方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F218C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID218A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_test(CkMy) ){
				if( m_pArith->l_cmp(F218C, CkMy) ){
					F218c |= 0x100;
				}
			}
			memmove( F218C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 11 );
			break;

		case ID219A :		// 18. 調整対象固定資産に係る調整税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID219A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F219C, DiagData.data_val, 6 ); 
			// 各入力の計算表示
			f2calq_all( 12 );
			break;

		case ID220A :		// 19. 調整対象固定資産を課税業務用に転用した調整額
			diag_getdata( IDC_ICSDIAGCTRL2, ID220A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F220C, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 13 );
			break;

		case ID221A:		// 20. 差引控除対象仕入税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F221C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F221C, CkMy) ){
//				F221c |= 0x10;
				F221c |= 0x100;
				memmove( F221C, CkMy, MONY_BUF_SIZE );
				memmove( (*m_pSnFhyo20Data)->Sn_2F21C, F221C, MONY_BUF_SIZE );
				if( m_pArith->l_test(F221C) < 0 ) {
					// 控除過大調整額
					m_pArith->l_neg( F221C );
					memmove( F222C, F221C, MONY_BUF_SIZE );
					memset( F221C, '\0', MONY_BUF_SIZE );	// 控除対象仕入税額０円クリア
				}
				else{
					// 控除対象仕入税額
					memset( F222C, '\0', MONY_BUF_SIZE );	// 控除過大調整額０円クリア
				}
//				memmove( pSyzShin->Snd.Sn_2F21B, F221B, 6 );		//★[13'11.21]
			}
			// 各入力の計算表示
			f2calq_all( 13 );
			break;

		case ID223A :		// 22. 貸倒回収に係る消費税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID223A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F223C, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 14 );
			break;

		case ID27EX :	// 課税売上割合の変更チェック
			(*m_pSnHeadData)->Sn_UPERS &= 0xef;
			diag_getdata( IDC_ICSDIAGCTRL2, ID27EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_UPERS |= 0x10;
			}
			// 各入力の計算表示
			f2calq_all( 4 );
			break;

		case ID2EX :	// 課税売上割合
			F2UWSEX = (*m_pSnFhyo20Data)->Sn_2FUWSEX;
			diag_getdata( IDC_ICSDIAGCTRL2, ID2EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			length = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) ); 
			m_UWexp = 0;
			if( length ){
				memmove( buf, DiagData.data_edit, length );
			}
			m_Util.perinp( 3, 2, buf, (short *)&m_UWexp );
			if( m_UWexp <= 10000 ){
				(*m_pSnFhyo20Data)->Sn_2FUWSEX = m_UWexp;
			}
			// 個別対応で95％未満の場合
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() && (F2UWSEX!=(*m_pSnFhyo20Data)->Sn_2FUWSEX) ){
				CString		Msg;
				Msg.Empty();
				Msg =  "課税売上割合が変更されました。\n";
				Msg += "個別対応方式欄の⑭～⑯の金額を見直してください。";
				ICSMessageBox( Msg, MB_OK, 0 );
				// 各入力の計算表示
				f2calq_all( 4 );
				break;
			}
			else{
				// 個別対応で95％未満の場合
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() && (F2UWS!=(*m_pSnFhyo20Data)->Sn_2FUWSEX) && ((*m_pSnFhyo20Data)->Sn_2FUWSEX<(*m_pSnHeadData)->m_UriRatioBorder) ){
					CString		Msg;
					Msg.Empty();
					Msg =  "課税売上割合が変更されました。\n";
					Msg += "個別対応方式欄の⑭～⑯の金額を見直してください。";
					ICSMessageBox( Msg, MB_OK, 0 );
				}
			}
			// 各入力の計算表示
			f2calq_all( 4 );
			if( m_UWexp > 10000 ){
				SetItemPosition( ID2EX );
			}
			break;

		default		:
			break;
	}
}

void CShinFhyo20::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}
	
	// Shift+TABをTABに、TABをENTERに返還
	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG&0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}

	// 12. 調整税額
	if( index != ID212A ){
		if( index > ID212A ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
		m_Fhyo20Diag02.SetNextPosition();

		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
			if(index == ID211A){//[Y]
				c_set(IDC_BUTTON1);
			}
		}
		else{
			if(index == ID220A){
				c_set(IDC_BUTTON1);
			}
		}

	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
		m_Fhyo20Diag02.SetPrevPosition();

		if( *m_pShinInfo->pOver500MillionSw != 1 &&  F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	//[Y]5億円以下かつ 課税売上割合が９５％以上の場合
			if(index == ID219A){
				c_set(IDC_BUTTON1);
			}
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
					if(index == ID215A){
						c_set(IDC_BUTTON1);
					}
				}
				else{
					if(index == ID218A){
						c_set(IDC_BUTTON1);
					}
				}
			}
			else{
				if(index == ID219A){
					c_set(IDC_BUTTON1);
				}
			}
		}
	}

	if( wTerm == VK_DELETE ){	
		switch( index ){
			case ID221A :		// 21.控除対象仕入税額
				F221c &= 0xeff;
				// 各入力の計算表示
				f2calq_all( 13 );
				m_Fhyo20Diag02.SetPosition( ID221A );
				break;

			//[Y]
			case ID29A:		//	9. 課税仕入に係る消費税額
				F29c &= ~0x800;
				F29c |= 0x100;
				// 各入力の計算表示
				f2calq_all( 6 );
				m_Fhyo20Diag02.SetPosition( ID29A );
				break;

			//[Y]
			case ID217A:		// 16. 個別対応方式控除仕入税額
				F217c &= ~0x100; 
				f2calq_all( 10 );
				m_Fhyo20Diag02.SetPosition( ID217A );
				break;

			//[Y]
			case ID218A:		// 17. 比例配分方式控除仕入税額
				F218c &= ~0x100; 
				f2calq_all( 11 );
				m_Fhyo20Diag02.SetPosition( ID218A );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);//[Y]
				break;
		}
	}

	//-->Editonから移動 harino ('14.03.07)
	int nowpos = m_Fhyo20Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

void CShinFhyo20::CheckButtonIcsdiagctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	CString		Msg;
	Msg.Empty();

	// 課税売上割合の変更チェック
	if( index == ID27EX ){
		diag_getdata( IDC_ICSDIAGCTRL2, ID27EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
			if( DiagData.data_check ){
				Msg  = _T("課税売上割合に準ずる割合を適用する為に、\n");
				Msg += _T("課税売上割合を変更される場合は・・・[はい]を、\n");
				Msg += _T("それ以外の理由で変更される場合は・・・[いいえ]を、\n");
				Msg += _T("選択してください。" );
//				Msg += _T("選択してください。\n" );
//				Msg += _T("※　申告書集計を行うと、全ての変更は無効になります。");
//				Msg += _T("※　いずれの場合も⑭～⑯の金額を再計算します。");
				int st = ICSMessageBox( Msg, MB_YESNO, 0 );
				if( st == IDYES ){
/*- nakaG -*/
//					if( pSyzShin->RATIO&0x02 ){
//						pSyzShin->RATIO &= 0xfd;
//					}
//					else{
//						pSyzShin->RATIO |= 0x02;
//					}
/*---------*/
					(*m_pSnHeadData)->Sn_UPERS |= 0x02;
/*---------*/
				}
			}
			else{
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					Msg  = "課税売上割合に準ずる割合を無効にします。";
					ICSMessageBox( Msg, MB_OK, 0 );
/*- nakaG -*/
//					pSyzShin->RATIO &= 0xfd;
/*---------*/
					(*m_pSnHeadData)->Sn_UPERS &= 0xfd;
/*---------*/
				}
			}
			if( ( DiagData.data_check && (((*m_pSnHeadData)->Sn_UPERS&0x10)==0x00) ) ||
				( (DiagData.data_check==0) && ((*m_pSnHeadData)->Sn_UPERS&0x10) ) ){
				Msg.Empty();
				Msg =  "課税売上割合を変更する場合は、\n";
				Msg += "個別対応方式欄の⑭～⑯の金額を見直してください。";
				ICSMessageBox( Msg, MB_OK, 0 );
			}
		}
		(*m_pSnFhyo20Data)->Sn_2FUWSEX = F2UWS;	
		if( (*m_pSnFhyo20Data)->Sn_2FUWS < (*m_pSnHeadData)->m_UriRatioBorder ){
			// 課税売上に係る仕入税額
			memmove( F215A, F215AEX, MONY_BUF_SIZE );
			memmove( F215B, F215BEX, MONY_BUF_SIZE );
			memmove( F215C, F215CEX, MONY_BUF_SIZE );
			// 共通の売上に係る仕入税額
			memmove( F216A, F216AEX, MONY_BUF_SIZE );
			memmove( F216B, F216BEX, MONY_BUF_SIZE );
			memmove( F216C, F216CEX, MONY_BUF_SIZE );
		}
		(*m_pSnHeadData)->Sn_UPERS &= 0xef;
		if( DiagData.data_check ){
			(*m_pSnHeadData)->Sn_UPERS |= 0x10;
		}
		// 各入力の計算表示
		f2calq_all( 4 );
	}
}

void CShinFhyo20::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
		CChangeTaxEx	CTDiagEx;

		// 前回の情報をセーブ
		char			SV_mtype = (*m_pSnTanaData)->Sn_MENZsw;
		unsigned short	SV_F2UWS = F2UWS;
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];

//		char			SV_menzk[3][6];
//		char			SV_menzz[3][6];
/*		memmove( SV_mnk40[0], (*m_pSnTanaData)->Sn_mnk40[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
		memmove( SV_mnz40[0], (*m_pSnTanaData)->Sn_mnz40[0], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
		memmove( SV_mnk40[1], (*m_pSnTanaData)->Sn_mnk40[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
		memmove( SV_mnz40[1], (*m_pSnTanaData)->Sn_mnz40[1], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
		memmove( SV_mnk40[2], (*m_pSnTanaData)->Sn_mnk40[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
		memmove( SV_mnz40[2], (*m_pSnTanaData)->Sn_mnz40[2], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
*/	
		memmove( SV_mnk63[0], (*m_pSnTanaData)->Sn_mnk63[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
		memmove( SV_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
		memmove( SV_mnk63[1], (*m_pSnTanaData)->Sn_mnk63[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
		memmove( SV_mnz63[1], (*m_pSnTanaData)->Sn_mnz63[1], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
		memmove( SV_mnk63[2], (*m_pSnTanaData)->Sn_mnk63[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
		memmove( SV_mnz63[2], (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃*/


		CTDiagEx.InitInfo( (*m_pSnFhyo20Data), (*m_pSnHeadData), (*m_pSnTanaData), m_pArith );

//		m_ItSw = 0;

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( g_hInstance );
		int st = (int)CTDiagEx.DoModal();
		AfxSetResourceHandle( svhInstResource );

//		m_ItSw = 1;
		if( st == IDOK ){
			// 手入力
			if( !((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]非連動の場合は常に更新させる
				// 前回が個別対応で95％未満の場合、前回分を削除
				if( (SV_mtype&0x10) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){ 
					if( SV_mtype&0x01 ){
						m_pArith->l_add( F215C, F215C, SV_mnz63[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_add( F216C, F216C, SV_mnz63[2] );		// 16. 共通の売上に係る仕入税額
//						m_pArith->l_add( F216C, F216C, SV_mnz40[2] );		// 16. 共通の売上に係る仕入税額
					}
					else{
						m_pArith->l_sub( F215C, F215C, SV_mnz63[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub( F216C, F216C, SV_mnz63[2] );		// 16. 共通の売上に係る仕入税額
//						m_pArith->l_sub( F216C, F216C, SV_mnz40[2] );		// 16. 共通の売上に係る仕入税額
					}
				}
				
				// 今回分を変更
				char	CkMy[MONY_BUF_SIZE] = {0};
				memmove( CkMy, F212C, MONY_BUF_SIZE );
				if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
					memset( F212C, '\0', MONY_BUF_SIZE );
					m_pArith->l_add( F212C, F212C, (*m_pSnTanaData)->Sn_mnz63[0] );
					m_pArith->l_add( F212C, F212C, (*m_pSnTanaData)->Sn_mnz63[1] );
					m_pArith->l_add( F212C, F212C, (*m_pSnTanaData)->Sn_mnz63[2] );
					// 旧格納場所へ格納
					memmove( (*m_pSnTanaData)->Sn_MNZ63, F212C, MONY_BUF_SIZE );
					if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
						m_pArith->l_neg( F212C );
					}
					// 個別対応で95％未満の場合
					if( (F2UWS<(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
						if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
							// 今回分を変更
							m_pArith->l_sub( F215C, F215C, (*m_pSnTanaData)->Sn_mnz63[0] );
							m_pArith->l_sub( F216C, F216C, (*m_pSnTanaData)->Sn_mnz63[2] );
						}
						else{
							// 今回分を変更
							m_pArith->l_add( F215C, F215C, (*m_pSnTanaData)->Sn_mnz63[0] );
							m_pArith->l_add( F216C, F216C, (*m_pSnTanaData)->Sn_mnz63[2] );
						}
//--> '17.11.20 INS START
						memmove( F215CEX, F215C, MONY_BUF_SIZE );
						memmove( F216CEX, F216C, MONY_BUF_SIZE );
//<-- '17.11.20 INS END
					}
				}
				else{
					memset( F212A, '\0', MONY_BUF_SIZE );
					memset( F212B, '\0', MONY_BUF_SIZE );
					memset( F212C, '\0', MONY_BUF_SIZE );
				}
				// 再表示
				DispFh2();
				SetItemPosition();	// ('14.03.13)
			}
		}
	}
	else{		// 比例配分
		CChangeTax		CTDiag;

		CTDiag.InitInfo( (*m_pSnFhyo20Data), (*m_pSnHeadData), (*m_pSnTanaData), m_pArith );

//		ItSw = 0;

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( g_hInstance );
		int st = (int)CTDiag.DoModal();
		AfxSetResourceHandle( svhInstResource );

//		ItSw = 1;
		if( st == IDOK ){

			if( !((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]非連動の場合は常に更新させる

				char	CkMy[MONY_BUF_SIZE] = {0};
				memmove( CkMy, F212C, MONY_BUF_SIZE );

				// (免税)控除対象調整税額
				if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
					memmove( F212A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
					memmove( F212B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
					memmove( F212C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
					if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
						m_pArith->l_neg( F212A );
						m_pArith->l_neg( F212B );
						m_pArith->l_neg( F212C );
					}
				}
				else{
					memset( F212A, '\0', MONY_BUF_SIZE );
					memset( F212B, '\0', MONY_BUF_SIZE );
					memset( F212C, '\0', MONY_BUF_SIZE );
				}

				// 再表示
				DispFh2();
				SetItemPosition();	// ('14.03.13)
			}
		}
	}
}

void CShinFhyo20::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	((CH26HyoView*)m_pParent)->SpecPassDlg( 0 );

	// 画面出力
	DispFh2();
//--> '15.04.04 INS START
	SetItemPosition();
//<-- '15.04.04 INS END
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo20::SetItemPosition()
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo20Diag02.GetPosition();
		if( (index = m_Fhyo20Diag02.GetPosition()) != -1) {
			m_Fhyo20Diag02.SetPosition(index);
			m_Fhyo20Diag02.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)

	m_Fhyo20Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo20Diag02.SetFocus();

	//入力可能なインデックスを探す処理が必要？

	//[Y]
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Fhyo20Diag02.SetPosition(ID21A);
	}
	else{
		m_Fhyo20Diag02.SetPosition(ID219A);
	}
	SetRedrawScroll( 0 );

}

//-----------------------------------------------------------------------------
// 特定収入関連の手入力サイン連動('15.05.20)
//-----------------------------------------------------------------------------
// 引数		sw	：	セット対象サイン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo20::SetCalqSw( int sw )
{
	F221c = sw;

	return 0;
}


