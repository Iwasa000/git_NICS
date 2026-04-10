// ShinFhyo20EX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo20EX.h"
#include "ShinFhyo20EXIdx.h"
#include "H26HyoView.h"	// '15.02.16

#include "ChangeTax.h"
#include "ChangeTaxEx.h"


// CShinFhyo20EX ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo20EX, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinFhyo20EX::CShinFhyo20EX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo20EX::IDD, pParent)
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
	memset( F214A, '\0', sizeof(F214A) );
	memset( F214B, '\0', sizeof(F214B) );
	memset( F214C, '\0', sizeof(F214C) );
	memset( F215A, '\0', sizeof(F215A) );
	memset( F215B, '\0', sizeof(F215B) );
	memset( F215C, '\0', sizeof(F215C) );
	memset( F217A, '\0', sizeof(F217A) );
	memset( F217B, '\0', sizeof(F217B) );
	memset( F217C, '\0', sizeof(F217C) );
	memset( F218A, '\0', sizeof(F218A) );
	memset( F218B, '\0', sizeof(F218B) );
	memset( F218C, '\0', sizeof(F218C) );
	memset( F217AEX, '\0', sizeof(F217AEX) );
	memset( F217BEX, '\0', sizeof(F217BEX) );
	memset( F217CEX, '\0', sizeof(F217CEX) );
	memset( F218AEX, '\0', sizeof(F218AEX) );
	memset( F218BEX, '\0', sizeof(F218BEX) );
	memset( F218CEX, '\0', sizeof(F218CEX) );
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
	memset( F224A, '\0', sizeof(F224A) );
	memset( F224B, '\0', sizeof(F224B) );
	memset( F224C, '\0', sizeof(F224C) );
	memset( F225A, '\0', sizeof(F225A) );
	memset( F225B, '\0', sizeof(F225B) );
	memset( F225C, '\0', sizeof(F225C) );
	memset( FS25A, '\0', sizeof(FS25A) );
	memset( FS25B, '\0', sizeof(FS25B) );
	memset( FS25C, '\0', sizeof(FS25C) );

	F2UWS = 0;
	F2UWSEX = 0;
	F219c = F220c = F223c = 0;
	F29c = F211c = F213c = 0;

	m_curwnd = -1;
}

CShinFhyo20EX::~CShinFhyo20EX()
{
}

void CShinFhyo20EX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo20Diag02);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo20Diag01);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo20EX, CSyzBaseDlg)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo20EX::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo20EX::OnBnClickedButton2)
END_MESSAGE_MAP()


// CShinFhyo20EX メッセージ ハンドラ

BOOL CShinFhyo20EX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo20Diag02;
	m_BottomIdx = 62;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinFhyo20EX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CWnd *pWnd = GetFocus();

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			GetAllDiagData();
			f2calq_all( 1 );
			WriteData( 1 );

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
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

		if(pWnd == GetDlgItem(IDC_BUTTON1)){
			if( (pMsg->wParam==VK_F3) || (pMsg->wParam==VK_RIGHT) || (pMsg->wParam==VK_DOWN) || (pMsg->wParam==VK_TAB)){
				if(  *m_pShinInfo->pOver500MillionSw != 1 &&  F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	//[Y]5億円以下かつ 課税売上割合が９５％以上の場合
					diag_setposition( IDC_ICSDIAGCTRL2, ID221A, CIcsdiagctrl );
				}
				else{
					if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//非連動の場合
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID217A, CIcsdiagctrl );
						}
						else{				
							diag_setposition( IDC_ICSDIAGCTRL2, ID220A, CIcsdiagctrl );
						}
					}
					else{
						diag_setposition( IDC_ICSDIAGCTRL2, ID221A, CIcsdiagctrl );
					}
				}
				return TRUE;

			}
			// 前項目へ
			if( (pMsg->wParam==VK_LEFT) || (pMsg->wParam==VK_F2) || (pMsg->wParam==VK_UP) ){
				//[Y]
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					diag_setposition( IDC_ICSDIAGCTRL2, ID213A, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL2, ID222A, CIcsdiagctrl );
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

HBRUSH CShinFhyo20EX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinFhyo20EX::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CShinFhyo20EX::OnSize(UINT nType, int cx, int cy)
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
int CShinFhyo20EX::InitInfo( CH28SnFhyo20Data **pSnFhyo20Data, CSnHeadData **pSnHeadData, CH26SnTanaData **pSnTanaData, CH28SnFhyo10Data **pSnFhyo10Data, CH28SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
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
int CShinFhyo20EX::Init()
{
	if( ((*m_pSnFhyo20Data)==NULL) || ((*m_pSnTanaData)==NULL) || ((*m_pSnFhyo10Data)==NULL) ){
		return -1;
	}

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
void CShinFhyo20EX::ChangeDispByApno()
{
	if( ((*m_pSnHeadData)->m_apno&0xf0) >= 0x50 ){	// 非営利法人
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_Button1.EnableWindow( FALSE );
		}
		else	{
			m_Button1.EnableWindow( TRUE );
			if( *m_pShinInfo->pSgSpc && !((*m_pSnHeadData)->Sn_Sign2&0x02) ){
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
		m_Button2.EnableWindow( FALSE );
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
int CShinFhyo20EX::ReadData( int pas )
{
	ASSERT( m_pSnFhyo20Data );
	ASSERT( *m_pSnFhyo20Data );

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
	
	// 特定課税仕入れに係る支払対価の額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F210A, (*m_pSnFhyo20Data)->Sn_2F10A, MONY_BUF_SIZE );
		memmove( F210B, (*m_pSnFhyo20Data)->Sn_2F10B, MONY_BUF_SIZE );
		memmove( F210C, (*m_pSnFhyo20Data)->Sn_2F10C, MONY_BUF_SIZE );
	}
	else{
		memset( F210A, '\0', MONY_BUF_SIZE );
		memset( F210B, '\0', MONY_BUF_SIZE );
		memset( F210C, '\0', MONY_BUF_SIZE );
	}
	
	// 特定課税仕入れに係る消費税額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F211A, (*m_pSnFhyo20Data)->Sn_2F11A, MONY_BUF_SIZE );
		memmove( F211B, (*m_pSnFhyo20Data)->Sn_2F11B, MONY_BUF_SIZE );
		memmove( F211C, (*m_pSnFhyo20Data)->Sn_2F11C, MONY_BUF_SIZE );
		F211c = (*m_pSnFhyo20Data)->Sn_2F11sw;
	}
	else{
		memset( F211A, '\0', MONY_BUF_SIZE );
		memset( F211B, '\0', MONY_BUF_SIZE );
		memset( F211C, '\0', MONY_BUF_SIZE );
		F211c = 0;
	}

	// 課税貨物に係る支払対価の額
//	memmove( F212A, (*m_pSnFhyo20Data)->Sn_2F12A, MONY_BUF_SIZE );
//	memmove( F212B, (*m_pSnFhyo20Data)->Sn_2F12B, MONY_BUF_SIZE );
//	memmove( F212C, (*m_pSnFhyo20Data)->Sn_2F12C, MONY_BUF_SIZE );
	// 課税貨物に係る消費税額
	memmove( F213A, (*m_pSnFhyo20Data)->Sn_2F12A, MONY_BUF_SIZE );	
	memmove( F213B, (*m_pSnFhyo20Data)->Sn_2F12B, MONY_BUF_SIZE );
	memmove( F213C, (*m_pSnFhyo20Data)->Sn_2F12C, MONY_BUF_SIZE );

	//(免税)控除対象調整税額
	if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){//個別対応時
			char	WORK0[MONY_BUF_SIZE] = {0};
			m_pArith->l_add( F214A , (*m_pSnTanaData)->Sn_menzz[0] , (*m_pSnTanaData)->Sn_menzz[1] );
			m_pArith->l_add( F214A , F214A , (*m_pSnTanaData)->Sn_menzz[2] );
			m_pArith->l_add( F214B , (*m_pSnTanaData)->Sn_mnz40[0] , (*m_pSnTanaData)->Sn_mnz40[1] );
			m_pArith->l_add( F214B , F214B , (*m_pSnTanaData)->Sn_mnz40[2] );
			m_pArith->l_add( F214C , (*m_pSnTanaData)->Sn_mnz63[0] , (*m_pSnTanaData)->Sn_mnz63[1] );
			m_pArith->l_add( F214C , F214C , (*m_pSnTanaData)->Sn_mnz63[2] );

		}
		else{//比例配分
			memmove( F214A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
			memmove( F214B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
			memmove( F214C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
		}

		if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
			m_pArith->l_neg( F214A );
			m_pArith->l_neg( F214B );
			m_pArith->l_neg( F214C );//[Y]
		}
	}
	else{
		memset( F214A, '\0', MONY_BUF_SIZE );
		memset( F214B, '\0', MONY_BUF_SIZE );
		memset( F214C, '\0', MONY_BUF_SIZE );
	}
	// 課税売上に係る仕入税額
	memmove( F217A, (*m_pSnFhyo20Data)->Sn_2F16A, MONY_BUF_SIZE );
	memmove( F217B, (*m_pSnFhyo20Data)->Sn_2F16B, MONY_BUF_SIZE );
	memmove( F217C, (*m_pSnFhyo20Data)->Sn_2F16C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( F218A, (*m_pSnFhyo20Data)->Sn_2F17A, MONY_BUF_SIZE );
	memmove( F218B, (*m_pSnFhyo20Data)->Sn_2F17B, MONY_BUF_SIZE );
	memmove( F218C, (*m_pSnFhyo20Data)->Sn_2F17C, MONY_BUF_SIZE );

	// 課税売上に係る仕入税額
	memmove( F217AEX, (*m_pSnFhyo20Data)->Sn_2F16A, MONY_BUF_SIZE );
	memmove( F217BEX, (*m_pSnFhyo20Data)->Sn_2F16B, MONY_BUF_SIZE );
	memmove( F217CEX, (*m_pSnFhyo20Data)->Sn_2F16C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( F218AEX, (*m_pSnFhyo20Data)->Sn_2F17A, MONY_BUF_SIZE );
	memmove( F218BEX, (*m_pSnFhyo20Data)->Sn_2F17B, MONY_BUF_SIZE );
	memmove( F218CEX, (*m_pSnFhyo20Data)->Sn_2F17C, MONY_BUF_SIZE );

	F219c = F220c = F223c = 0x00;
	// 個別対応方式控除仕入税額
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F18A ) ){
		memmove( F219A, (*m_pSnFhyo20Data)->Sn_2F18A, MONY_BUF_SIZE );
		F219c |= 0x10;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F18B ) ){
		memmove( F219B, (*m_pSnFhyo20Data)->Sn_2F18B, MONY_BUF_SIZE );
		F219c |= 0x01;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F18C ) ){
		memmove( F219C, (*m_pSnFhyo20Data)->Sn_2F18C, MONY_BUF_SIZE );
		F219c |= 0x100;
	}
	// 比例配分方式控除仕入税額
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F19A ) ){
		memmove( F220A, (*m_pSnFhyo20Data)->Sn_2F19A, MONY_BUF_SIZE );
		F220c |= 0x10;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F19B ) ){
		memmove( F220B, (*m_pSnFhyo20Data)->Sn_2F19B, MONY_BUF_SIZE );
		F220c |= 0x01;
	}
	if( m_pArith->l_test( (*m_pSnFhyo20Data)->Sn_2F19C ) ){
		memmove( F220C, (*m_pSnFhyo20Data)->Sn_2F19C, MONY_BUF_SIZE );
		F220c |= 0x100;
	}
	// 調整対象固定資産に係る調整税額
	memmove( F221A, (*m_pSnFhyo20Data)->Sn_2F20A, MONY_BUF_SIZE );
	memmove( F221B, (*m_pSnFhyo20Data)->Sn_2F20B, MONY_BUF_SIZE );
	memmove( F221C, (*m_pSnFhyo20Data)->Sn_2F20C, MONY_BUF_SIZE );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( F222A, (*m_pSnFhyo20Data)->Sn_2F21A, MONY_BUF_SIZE );
	memmove( F222B, (*m_pSnFhyo20Data)->Sn_2F21B, MONY_BUF_SIZE );
	memmove( F222C, (*m_pSnFhyo20Data)->Sn_2F21C, MONY_BUF_SIZE );

	// 控除対象仕入税額
	F223c = (*m_pSnFhyo20Data)->Sn_2F22sw;
	if( F223c & 0x01 ){			// 手入力有り
		memmove( F223A, (*m_pSnFhyo20Data)->Sn_2F22A, MONY_BUF_SIZE );
	}
	else{
		memmove( F223A, (*m_pSnFhyo10Data)->Sn_1F4A, MONY_BUF_SIZE );	// 手入力無し
	}

	if( F223c & 0x10 ){			// 手入力有り
		memmove( F223B, (*m_pSnFhyo20Data)->Sn_2F22B, MONY_BUF_SIZE );
	}
	else{
		memmove( F223B, (*m_pSnFhyo10Data)->Sn_1F4B, MONY_BUF_SIZE );	// 手入力無し
	}
	if( F223c & 0x100 ){			// 手入力有り
		memmove( F223C, (*m_pSnFhyo20Data)->Sn_2F22C, MONY_BUF_SIZE );
	}
	else{
		memmove( F223C, (*m_pSnFhyo10Data)->Sn_1F4C, MONY_BUF_SIZE );	// 手入力無し
	}
	// 貸倒回収に係る税額
	if( pas == 1 ){
		memmove( FS25A, (*m_pSnFhyo20Data)->Sn_2F24A, MONY_BUF_SIZE );
		memmove( FS25B, (*m_pSnFhyo20Data)->Sn_2F24B, MONY_BUF_SIZE );
		memmove( FS25C, (*m_pSnFhyo20Data)->Sn_2F24C, MONY_BUF_SIZE );
	}
	memmove( F225A, (*m_pSnFhyo20Data)->Sn_2F24A, MONY_BUF_SIZE );
	memmove( F225B, (*m_pSnFhyo20Data)->Sn_2F24B, MONY_BUF_SIZE );
	memmove( F225C, (*m_pSnFhyo20Data)->Sn_2F24C, MONY_BUF_SIZE );

	// 控除過大調整税額	･ 貸倒回収に係る消費税額
	m_pArith->l_sub( F224A, (*m_pSnFhyo10Data)->Sn_1F3A, F225A );
	m_pArith->l_sub( F224B, (*m_pSnFhyo10Data)->Sn_1F3B, F225B );
	m_pArith->l_sub( F224C, (*m_pSnFhyo10Data)->Sn_1F3C, F225C );

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
int CShinFhyo20EX::WriteData( int pas )
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

	// 特定課税仕入れに係る支払対価の額
	memmove( (*m_pSnFhyo20Data)->Sn_2F10A, F210A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F10B, F210B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F10C, F210C, MONY_BUF_SIZE );

	// 特定課税仕入れに係る消費税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F11A, F211A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F11B, F211B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F11C, F211C, MONY_BUF_SIZE );

	(*m_pSnFhyo20Data)->Sn_2F11sw = F211c;

	// 課税貨物に係る支払対価の額
//	memmove( (*m_pSnFhyo20Data)->Sn_2F10A, F212A, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo20Data)->Sn_2F10B, F212B, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo20Data)->Sn_2F10C, F212C, MONY_BUF_SIZE );
	// 課税貨物に係る消費税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F12A, F213A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F12B, F213B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F12C, F213C, MONY_BUF_SIZE );
	// 課税売上に係る仕入税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F16A, F217A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F16B, F217B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F16C, F217C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F17A, F218A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F17B, F218B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F17C, F218C, MONY_BUF_SIZE );
	// 個別対応方式控除仕入税額
	memset( (*m_pSnFhyo20Data)->Sn_2F18A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F18B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F18C, '\0', MONY_BUF_SIZE );
	if( F219c & 0x10 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F18A, F219A, MONY_BUF_SIZE );
	}
	if( F219c & 0x01 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F18B, F219B, MONY_BUF_SIZE );
	}
	if( F219c & 0x100 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F18C, F219C, MONY_BUF_SIZE );
	}
	// 比例配分方式控除仕入税額
	memset( (*m_pSnFhyo20Data)->Sn_2F19A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F19B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo20Data)->Sn_2F19C, '\0', MONY_BUF_SIZE );
	if( F220c & 0x10 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F19A, F220A, MONY_BUF_SIZE );
	}
	if( F220c & 0x01 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F19B, F220B, MONY_BUF_SIZE );
	}
	if( F220c & 0x100 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F19C, F220C, MONY_BUF_SIZE );
	}
	// 調整対象固定資産に係る調整税額
	memmove( (*m_pSnFhyo20Data)->Sn_2F20A, F221A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F20B, F221B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F20C, F221C, MONY_BUF_SIZE );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( (*m_pSnFhyo20Data)->Sn_2F21A, F222A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F21B, F222B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F21C, F222C, MONY_BUF_SIZE );
	// 控除対象仕入税額
	if( F223c & 0x01 ){
		memmove( (*m_pSnFhyo20Data)->Sn_2F22A, F223A, MONY_BUF_SIZE );
	}
	else{
		memset( (*m_pSnFhyo20Data)->Sn_2F22A, '\0', MONY_BUF_SIZE );
	}
	if( F223c & 0x10 ){
		if( m_pArith->l_test( F223B ) ){
			memmove( (*m_pSnFhyo20Data)->Sn_2F22B, F223B, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo20Data)->Sn_2F22B, '\0', MONY_BUF_SIZE );
	}
	if( F223c & 0x100 ){
		if( m_pArith->l_test( F223C ) ){
			memmove( (*m_pSnFhyo20Data)->Sn_2F22C, F223C, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo20Data)->Sn_2F22C, '\0', MONY_BUF_SIZE );
	}
	(*m_pSnFhyo20Data)->Sn_2F22sw = F223c;

	memmove( (*m_pSnFhyo10Data)->Sn_1F4A, F223A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4B, F223B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4C, F223C, MONY_BUF_SIZE );
	// 控除過大調整税額	･ 貸倒回収に係る消費税額
	if( (pas==1) && (m_pArith->l_cmp(FS25A, F223A)||m_pArith->l_cmp(FS25B, F223B)) ){
		st = 1;
	}
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3A, F224A, F225A );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3B, F224B, F225B );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3C, F224C, F225C );
	memmove( (*m_pSnFhyo20Data)->Sn_2F24A, F225A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F24B, F225B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo20Data)->Sn_2F24C, F225C, MONY_BUF_SIZE );

	return st;
}

//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo20EX::DispFh2()
{
	char				buf[512] = {0};
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	// 画面の初期化
	for( int i = 90;i <= 118; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
		//手入力サイン初期化
		F29c = 0;
		F219c = 0;
		F220c = 0;
		if( ((CH26HyoView*)m_pParent)->IsSpcDataLinked() == FALSE ){
			F223c = 0;
		}
	}

	// 期間・氏名の表示
	char	yy=0, mm=0, dd=0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof( buf ), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 90, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 91, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( buf, '\0', sizeof( buf ) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, &(*m_pSnHeadData)->Sn_DIHYO[0], sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, &(*m_pSnHeadData)->Sn_CONAM[0], sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 92, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税売上高の５億超えチェック
	char	ValKur[6] = {0};
	char	ValCnv[6] = {0};
	m_pArith->l_add( ValKur, F21C, F22C );
	(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
	ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
	ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
	diag_clear( IDC_ICSDIAGCTRL1, 3, TRUE, CIcsdiagctrl );
	*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
	if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
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
	// 特定課税仕入れに係る支払対価の額
	dsp_prs( IDC_ICSDIAGCTRL2, ID210A, F210C );
	// 特定課税仕入れに係る消費税額
	f2calq_11();
	// 課税貨物に係る消費税額
//	f2calq_11();
	f2calq_13();
	// (免税)控除対象調整税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID214A, F214C );
	// 課税仕入等の税額の合計額
//	f2calq_13();
	f2calq_15();
	if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
		// 課税売上割合に準ずる割合を有効サイン
		f2calq_ko();		// 課税売上割合が９５％未満
	}
	else{
		// 課税売上割合が９５％以上
		if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
//			f2calq_14();
			f2calq_16();
		}
		// 課税売上割合が９５％未満
		else{
			f2calq_ko();
		}
	}
	// 特定収入を加味した控除対象仕入税額の計算
	f2calq_spc();
	// 調整対象固定資産に係る調整税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID221A, F221C );
	// 調整対象固定資産を課税業務用に転用した調整額
	dsp_prs( IDC_ICSDIAGCTRL2, ID222A, F222C );
	// 差引控除税額
//	f2calq_2122();
	f2calq_2324();
	// 貸倒回収に係る消費税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID225A, F225C );
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo20EX::dsp_prs( unsigned short Id, short Index, char *Mony )
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
		case ID210A :		// 10. 特定課税仕入れに係る支払対価の額	
		case ID211A :		// 11. 特定課税仕入れに係る消費税額

		case ID213A:		// 12. 課税貨物に係る消費税額
		case ID214A:		// 13. 調整税額
		case ID217A:		// 16. 課税売上に係る仕入税額
		case ID218A:		// 17. 共通の売上に係る仕入税額
		case ID219A:		// 18. 個別対応方式控除仕入税額
		case ID220A:		// 19. 比例配分方式控除仕入税額
		case ID221A:		// 20. 調整対象固定資産に係る調整税額
		case ID222A:		// 20. 調整対象固定資産を課税業務用に転用した調整額
		case ID223A:		// 22. 差引控除対象仕入税額
		case ID225A:		// 24. 貸倒回収に係る消費税額
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
void CShinFhyo20EX::dsp_cls( unsigned short Id, short Index )
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
void CShinFhyo20EX::GetAllDiagData()
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
	// 10. 特定課税仕入れに係る支払対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID210A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F210C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 11. 特定課税仕入れに係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID211A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F211C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 12. 課税貨物に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID213A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F213C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 16. 課税売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID217A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F217C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 17. 共通の売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID218A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F218C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 18. 個別対応方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID219A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F219C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 19. 比例配分方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID220A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F220C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 20. 調整対象固定資産に係る調整税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F221C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 21. 調整対象固定資産を課税業務用に転用した調整額
	diag_getdata( IDC_ICSDIAGCTRL2, ID222A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F222C, DiagData.data_val, MONY_BUF_SIZE ); 
	// 24. 貸倒回収に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID225A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F225C, DiagData.data_val, MONY_BUF_SIZE ); 
}

//-----------------------------------------------------------------------------
// 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo20EX::ChangeColor( unsigned short id, short Index, int sign )
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
int CShinFhyo20EX::InitDiagAttr()
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
//--> '15.08.20 INS START
				int st = ChkTkkzIdx( Fhyo2[cnt].Index );
				if( st == 1 ){
					if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
						Flg = 1;
					}
					else{
						Flg = 0;
					}
				}
//<-- '15.08.20 INS END
			}
			else{
				Flg = Fhyo2[cnt].Connect;
			}
		}
		if( ((*m_pSnHeadData)->Sn_EXP100&0x01) && *m_pShinInfo->pSgSpc && (Fhyo2[cnt].Index==ID27EX) ){
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
void CShinFhyo20EX::SetItemPosition( int Index, BOOL isScroll/*=FALSE*/ )
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
			setIdx = ID221A;
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

BEGIN_EVENTSINK_MAP(CShinFhyo20EX, CSyzBaseDlg)
	ON_EVENT(CShinFhyo20EX, IDC_ICSDIAGCTRL2, 1, CShinFhyo20EX::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo20EX, IDC_ICSDIAGCTRL2, 2, CShinFhyo20EX::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo20EX, IDC_ICSDIAGCTRL2, 3, CShinFhyo20EX::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinFhyo20EX, IDC_ICSDIAGCTRL2, 9, CShinFhyo20EX::CheckButtonIcsdiagctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo20EX::EditONIcsdiagctrl2(short index)
{
	m_curwnd = IDC_ICSDIAGCTRL2;

	switch( index ){
		case ID214A :
			if( m_DlSw ){
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					// 課税売上割合に準ずる割合を有効
					diag_setposition( IDC_ICSDIAGCTRL2, ID217A, CIcsdiagctrl );
				}
				else{
					if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){		// 課税売上高割合が95%以上の場合
						diag_setposition( IDC_ICSDIAGCTRL2, ID221A, CIcsdiagctrl );
					}
					else{				// 課税売上高割合が95%未満の場合	
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID217A, CIcsdiagctrl );
						}
						else{										// 比例配分方式
							diag_setposition( IDC_ICSDIAGCTRL2, ID220A, CIcsdiagctrl );
						}
					}
				}
			}
			else{
				SetItemPosition( ID213A );
			}

			// 次入力項目　設定
			if( !m_DlSw ){
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					// 課税売上割合に準ずる割合を有効
					diag_setposition( IDC_ICSDIAGCTRL2, ID217A, CIcsdiagctrl );
				}
				else{
					if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){		// 課税売上高割合が95%以上の場合
						diag_setposition( IDC_ICSDIAGCTRL2, ID221A, CIcsdiagctrl );
					}
					else{												// 課税売上高割合が95%未満の場合	
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){		// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID217A, CIcsdiagctrl );
						}
						else{			// 比例配分方式
							diag_setposition( IDC_ICSDIAGCTRL2, ID220A, CIcsdiagctrl );
						}
					}
				}
			}
			else	{
				SetItemPosition( ID213A );
			}
			break;
		case ID221A :
		case ID217A :
		case ID220A :
			break;
		default      :
			break;

	}
}

void CShinFhyo20EX::EditOFFIcsdiagctrl2( short index )
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	int				length;
	char			CkMy[MONY_BUF_SIZE] = {0};
	char			buf[512] = {0};

	//m_curwnd = -1;	// 削除 harino ('14.03.07)

	// 12. 調整税額
	if( index != ID214A ){
		if( index > ID214A ){
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
				// 課税売上高の５億超えチェック
				m_pArith->l_add( ValKur, F21C, F22C );
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F217A, F217AEX, MONY_BUF_SIZE );
						memmove( F217B, F217BEX, MONY_BUF_SIZE );
						memmove( F217C, F217CEX, MONY_BUF_SIZE );
						// 共通の売上に係る仕入税額
						memmove( F218A, F218AEX, MONY_BUF_SIZE );
						memmove( F218B, F218BEX, MONY_BUF_SIZE );
						memmove( F218C, F218CEX, MONY_BUF_SIZE );
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
				// 課税売上高の５億超えチェック
				m_pArith->l_add( ValKur, F21C, F22C );
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( ValKur, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F217A, F217AEX, MONY_BUF_SIZE );
						memmove( F217B, F217BEX, MONY_BUF_SIZE );
						memmove( F217C, F217CEX, MONY_BUF_SIZE );
						// 共通の売上に係る仕入税額
						memmove( F218A, F218AEX, MONY_BUF_SIZE );
						memmove( F218B, F218BEX, MONY_BUF_SIZE );
						memmove( F218C, F218CEX, MONY_BUF_SIZE );
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

		case ID210A :		// 10. 特定課税仕入れに係る支払対価の額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F210C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID210A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F210C, CkMy) ){
				if(!(F211c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F211c |= 0x100;
				}
			}
			memmove( F210C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 15 );
			break;

		case ID211A :		// 11. 特定課税仕入れに係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F211C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID211A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F211C, CkMy) ){
				F211c |= 0x800;
				F211c &= ~0x100;
			}
			memmove( F211C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 16 );
			break;

		case ID213A :		// 12. 課税貨物に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F213C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID213A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F213C, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F213C, CkMy) ){
				if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()&&(F2UWS<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
					CString		Msg;
					Msg.Empty();
					Msg =  "課税貨物に係る消費税額が変更されました。\n";
					Msg += "個別対応方式欄の⑯～⑱の金額を見直してください。";
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}
			// 各入力の計算表示
			f2calq_all( 7 );
			break;

		case ID214A	:		// 13. 調整税額
			break;

		case ID217A :		// 16. 課税売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID217A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F217C, DiagData.data_val, MONY_BUF_SIZE );
			memmove( F217CEX, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 8 );
			break;

		case ID218A :		// 17. 共通の売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID218A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F218C, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F218CEX, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 9 );
			break;

		case ID219A :		// 18. 個別対応方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F219C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID219A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_test(CkMy) ){
				if( m_pArith->l_cmp(F219C, CkMy) ){
					F219c |= 0x100;
				}
			}
			memmove( F219C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 10 );
			break;

		case ID220A:		// 19. 比例配分方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F2UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F220C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID220A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_test(CkMy) ){
				if( m_pArith->l_cmp(F220C, CkMy) ){
					F220c |= 0x100;
				}
			}
			memmove( F220C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f2calq_all( 11 );
			break;

		case ID221A :		// 20. 調整対象固定資産に係る調整税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F221C, DiagData.data_val, 6 ); 
			// 各入力の計算表示
			f2calq_all( 12 );
			break;

		case ID222A :		// 21. 調整対象固定資産を課税業務用に転用した調整額
			diag_getdata( IDC_ICSDIAGCTRL2, ID222A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F222C, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f2calq_all( 13 );
			break;

		case ID223A:		// 22. 差引控除対象仕入税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
//-- '15.12.28 --
//			memmove( CkMy, F221C, MONY_BUF_SIZE );
//---------------
			memmove( CkMy, F223C, MONY_BUF_SIZE );
//---------------
			diag_getdata( IDC_ICSDIAGCTRL2, ID223A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F223C, CkMy) ){
				F223c |= 0x100;
				memmove( F223C, CkMy, MONY_BUF_SIZE );
//-- '15.12.28 --
//				memmove( (*m_pSnFhyo20Data)->Sn_2F21C, F223C, MONY_BUF_SIZE );
//---------------
				memmove( (*m_pSnFhyo20Data)->Sn_2F22C, F223C, MONY_BUF_SIZE );
//---------------
				if( m_pArith->l_test(F223C) < 0 ) {
					// 控除過大調整額
					m_pArith->l_neg( F223C );
					memmove( F224C, F223C, MONY_BUF_SIZE );
					memset( F223C, '\0', MONY_BUF_SIZE );	// 控除対象仕入税額０円クリア
				}
				else{
					// 控除対象仕入税額
					memset( F224C, '\0', MONY_BUF_SIZE );	// 控除過大調整額０円クリア
				}
			}
			// 各入力の計算表示
			f2calq_all( 13 );
			break;

		case ID225A :		// 24. 貸倒回収に係る消費税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID225A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F225C, DiagData.data_val, MONY_BUF_SIZE ); 
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
				Msg += "個別対応方式欄の⑯～⑱の金額を見直してください。";
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
					Msg += "個別対応方式欄の⑯～⑱の金額を見直してください。";
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

void CShinFhyo20EX::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
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

	// 13. 調整税額
	if( index != ID214A ){
		if( index > ID214A ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
		m_Fhyo20Diag02.SetNextPosition();

		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
			if(index == ID213A){//[Y]
				c_set(IDC_BUTTON1);
			}
		}
		else{
			if(index == ID222A){
				c_set(IDC_BUTTON1);
			}
		}

	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
		m_Fhyo20Diag02.SetPrevPosition();

		if( *m_pShinInfo->pOver500MillionSw != 1 &&  F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	//[Y]5億円以下かつ 課税売上割合が９５％以上の場合
			if(index == ID221A){
				c_set(IDC_BUTTON1);
			}
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
					if(index == ID217A){
						c_set(IDC_BUTTON1);
					}
				}
				else{
					if(index == ID220A){
						c_set(IDC_BUTTON1);
					}
				}
			}
			else{
				if(index == ID221A){
					c_set(IDC_BUTTON1);
				}
			}
		}
	}

	if( wTerm == VK_DELETE ){	
		switch( index ){
			case ID223A :		// 22.控除対象仕入税額
				F223c &= 0xeff;
				// 各入力の計算表示
				f2calq_all( 13 );
				m_Fhyo20Diag02.SetPosition( ID223A );
				break;

			//[Y]
			case ID29A:		//	9. 課税仕入に係る消費税額
				F29c &= ~0x800;
				F29c |= 0x100;
				// 各入力の計算表示
				f2calq_all( 6 );
				m_Fhyo20Diag02.SetPosition( ID29A );
				break;

//--> '15.08.21 INS START
			case ID211A:	//	11. 特定課税仕入れに係る消費税額
				F211c &= ~0x800;
				F211c |= 0x100;
				// 各入力の計算表示
				f2calq_all( 15 );
				m_Fhyo20Diag02.SetPosition( ID211A );
				break;
//<-- '15.08.21 INS END

			//[Y]
			case ID219A:		// 18. 個別対応方式控除仕入税額
				F219c &= ~0x100; 
				f2calq_all( 10 );
				m_Fhyo20Diag02.SetPosition( ID219A );
				break;

			//[Y]
			case ID220A:		// 19. 比例配分方式控除仕入税額
				F220c &= ~0x100; 
				f2calq_all( 11 );
				m_Fhyo20Diag02.SetPosition( ID220A );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);//[Y]
				break;
		}
	}

	int nowpos = m_Fhyo20Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}

void CShinFhyo20EX::CheckButtonIcsdiagctrl2(short index, LPUNKNOWN data)
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
				int st = ICSMessageBox( Msg, MB_YESNO, 0 );
				if( st == IDYES ){
					(*m_pSnHeadData)->Sn_UPERS |= 0x02;
				}
			}
			else{
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					Msg  = "課税売上割合に準ずる割合を無効にします。";
					ICSMessageBox( Msg, MB_OK, 0 );
					(*m_pSnHeadData)->Sn_UPERS &= 0xfd;
				}
			}
			if( ( DiagData.data_check && (((*m_pSnHeadData)->Sn_UPERS&0x10)==0x00) ) ||
				( (DiagData.data_check==0) && ((*m_pSnHeadData)->Sn_UPERS&0x10) ) ){
				Msg.Empty();
				Msg =  "課税売上割合を変更する場合は、\n";
				Msg += "個別対応方式欄の⑯～⑱の金額を見直してください。";
				ICSMessageBox( Msg, MB_OK, 0 );
			}
		}
		(*m_pSnFhyo20Data)->Sn_2FUWSEX = F2UWS;	
		if( (*m_pSnFhyo20Data)->Sn_2FUWS < (*m_pSnHeadData)->m_UriRatioBorder ){
			// 課税売上に係る仕入税額
			memmove( F217A, F217AEX, MONY_BUF_SIZE );
			memmove( F217B, F217BEX, MONY_BUF_SIZE );
			memmove( F217C, F217CEX, MONY_BUF_SIZE );
			// 共通の売上に係る仕入税額
			memmove( F218A, F218AEX, MONY_BUF_SIZE );
			memmove( F218B, F218BEX, MONY_BUF_SIZE );
			memmove( F218C, F218CEX, MONY_BUF_SIZE );
		}
		(*m_pSnHeadData)->Sn_UPERS &= 0xef;
		if( DiagData.data_check ){
			(*m_pSnHeadData)->Sn_UPERS |= 0x10;
		}
		// 各入力の計算表示
		f2calq_all( 4 );
	}
}

void CShinFhyo20EX::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
		CChangeTaxEx	CTDiagEx;

		// 前回の情報をセーブ
		char			SV_mtype = (*m_pSnTanaData)->Sn_MENZsw;
		unsigned short	SV_F2UWS = F2UWS;
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];

		memmove( SV_mnk63[0], (*m_pSnTanaData)->Sn_mnk63[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
		memmove( SV_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
		memmove( SV_mnk63[1], (*m_pSnTanaData)->Sn_mnk63[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
		memmove( SV_mnz63[1], (*m_pSnTanaData)->Sn_mnz63[1], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃
		memmove( SV_mnk63[2], (*m_pSnTanaData)->Sn_mnk63[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
		memmove( SV_mnz63[2], (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE );	// 消費税額    (〃      )　〃*/


		CTDiagEx.InitInfo( (*m_pSnFhyo20Data), (*m_pSnHeadData), (*m_pSnTanaData), m_pArith );

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( g_hInstance );
		int st = (int)CTDiagEx.DoModal();
		AfxSetResourceHandle( svhInstResource );

		if( st == IDOK ){
			// 手入力
			if( !((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]非連動の場合は常に更新させる
				// 前回が個別対応で95％未満の場合、前回分を削除
				if( (SV_mtype&0x10) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){ 
					if( SV_mtype&0x01 ){
						m_pArith->l_add( F217C, F217C, SV_mnz63[0] );		// 16. 課税売上に係る仕入税額
						m_pArith->l_add( F218C, F218C, SV_mnz63[2] );		// 17. 共通の売上に係る仕入税額
					}
					else{
						m_pArith->l_sub( F217C, F217C, SV_mnz63[0] );		// 16. 課税売上に係る仕入税額
						m_pArith->l_sub( F218C, F218C, SV_mnz63[2] );		// 17. 共通の売上に係る仕入税額
					}
				}
				
				// 今回分を変更
				char	CkMy[MONY_BUF_SIZE] = {0};
				memmove( CkMy, F214C, MONY_BUF_SIZE );
				if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
					memset( F214C, '\0', MONY_BUF_SIZE );
					m_pArith->l_add( F214C, F214C, (*m_pSnTanaData)->Sn_mnz63[0] );
					m_pArith->l_add( F214C, F214C, (*m_pSnTanaData)->Sn_mnz63[1] );
					m_pArith->l_add( F214C, F214C, (*m_pSnTanaData)->Sn_mnz63[2] );
					// 旧格納場所へ格納
					memmove( (*m_pSnTanaData)->Sn_MNZ63, F214C, MONY_BUF_SIZE );
					if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
						m_pArith->l_neg( F214C );
					}
					// 個別対応で95％未満の場合
					if( (F2UWS<(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
						if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
							// 今回分を変更
							m_pArith->l_sub( F217C, F217C, (*m_pSnTanaData)->Sn_mnz63[0] );
							m_pArith->l_sub( F218C, F218C, (*m_pSnTanaData)->Sn_mnz63[2] );
						}
						else{
							// 今回分を変更
							m_pArith->l_add( F217C, F217C, (*m_pSnTanaData)->Sn_mnz63[0] );
							m_pArith->l_add( F218C, F218C, (*m_pSnTanaData)->Sn_mnz63[2] );
						}
//--> '17.11.20 INS START
//						memmove( F217CEX, F217C, MONY_BUF_SIZE );
//						memmove( F218CEX, F218C, MONY_BUF_SIZE );
//<-- '17.11.20 INS END
					}
				}
				else{
					memset( F214A, '\0', MONY_BUF_SIZE );
					memset( F214B, '\0', MONY_BUF_SIZE );
					memset( F214C, '\0', MONY_BUF_SIZE );
				}

				// 個別対応で95%未満の場合
				if( (F2UWS<(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
					memmove( F217CEX, F217C, MONY_BUF_SIZE );
					memmove( F218CEX, F218C, MONY_BUF_SIZE );
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


		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( g_hInstance );
		int st = (int)CTDiag.DoModal();
		AfxSetResourceHandle( svhInstResource );

		if( st == IDOK ){

			if( !((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]非連動の場合は常に更新させる

				char	CkMy[MONY_BUF_SIZE] = {0};
				memmove( CkMy, F214C, MONY_BUF_SIZE );

				// (免税)控除対象調整税額
				if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
					memmove( F214A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
					memmove( F214B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
					memmove( F214C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
					if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
						m_pArith->l_neg( F214A );
						m_pArith->l_neg( F214B );
						m_pArith->l_neg( F214C );
					}
				}
				else{
					memset( F214A, '\0', MONY_BUF_SIZE );
					memset( F214B, '\0', MONY_BUF_SIZE );
					memset( F214C, '\0', MONY_BUF_SIZE );
				}

				// 再表示
				DispFh2();
				SetItemPosition();	// ('14.03.13)
			}
		}
	}
}

void CShinFhyo20EX::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	((CH26HyoView*)m_pParent)->SpecPassDlg( 0 );

	// 画面出力
	DispFh2();
	SetItemPosition();
}

//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo20EX::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo20Diag02.GetPosition();
		if( (index = m_Fhyo20Diag02.GetPosition()) != -1) {
			m_Fhyo20Diag02.SetPosition(index);
			m_Fhyo20Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo20Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo20Diag02.SetFocus();

	//入力可能なインデックスを探す処理が必要？

	//[Y]
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Fhyo20Diag02.SetPosition(ID21A);
	}
	else{
		m_Fhyo20Diag02.SetPosition(ID221A);
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
int CShinFhyo20EX::SetCalqSw( int sw )
{
//-- '15.11.14 --
//	F213c = sw;
//---------------
	F223c = sw;
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// 特定課税仕入に関連するインデックス？
//-----------------------------------------------------------------------------
// 引数		idx	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	0	：	特定課税仕入に関連しない
//			1	：	特定課税仕入で入力可能になるインデックス
//			2	：	特定課税仕入で入力不可になるインデックス
//-----------------------------------------------------------------------------
int CShinFhyo20EX::ChkTkkzIdx( short idx )
{
	int rSt = 0;

	if( (ID210A<=idx) && (idx<=ID211A) ){
		rSt = 1;
	}

	return rSt;
}

//-----------------------------------------------------------------------------
// 課税売上割合の取得
//-----------------------------------------------------------------------------
// 返送値	計算した課税売上割合
//-----------------------------------------------------------------------------
unsigned short CShinFhyo20EX::GetCalqedKzwari()
{
	unsigned short	rtKzwari = F2UWS;

	if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){		// 課税売上割合を変更
		if( (*m_pSnHeadData)->Sn_UPERS&0x02 ){	// 課税売上割合に準ずる割合を使用
			// 計算
			char	WORK0[MONY_BUF_SIZE] = {0};
			char	PW6[MONY_BUF_SIZE] = {0};
			char	PER100BUF[MONY_BUF_SIZE] = {0};
			m_pArith->l_input( PER100BUF, _T("10000") );
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F24C, (unsigned char *)F27C, 2, 0 );
			if( m_pArith->l_cmp(F24C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F27C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
				memmove( WORK0, PER100BUF, sizeof(WORK0) );
			}
			char	buf[64]={0};
			m_pArith->l_print( buf, WORK0, "SSSSS" );
			rtKzwari = atoi( buf );
		}
	}

	return rtKzwari;
}

//-----------------------------------------------------------------------------
// 特定課税仕入に関する項目の金額クリア('15.11.13)
//-----------------------------------------------------------------------------
void CShinFhyo20EX::ClearTkkzMony()
{
	m_pArith->l_clear( F210A );
	m_pArith->l_clear( F210B );
	m_pArith->l_clear( F210C );

	m_pArith->l_clear( F211A );
	m_pArith->l_clear( F211B );
	m_pArith->l_clear( F211C );

	DispFh2();

	WriteData( 1 );
}
