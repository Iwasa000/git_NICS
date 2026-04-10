// ShinFhyo2_12.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo2_12.h"
#include "ShinFhyo2_12idx.h"
#include "H31HyoView.h"
#include "ChangeTax.h"
#include "ChangeTaxEx.h"


// ShinFhyo2_12 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo2_12, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;


CShinFhyo2_12::CShinFhyo2_12(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo2_12::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo2_02Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pShinInfo ( NULL )
	, m_pSnTanaData ( NULL )
	, m_pSyzSyukei ( NULL )
	, m_SpcSgn ( FALSE )
{
	memset ( F212_1A, '\0', sizeof ( F212_1A ) );
	memset ( F212_1B, '\0', sizeof ( F212_1B ) );
	memset ( F212_1C, '\0', sizeof ( F212_1C ) );
	memset ( F212_1X, '\0', sizeof ( F212_1X ) );

	memset ( F212_4X, '\0', sizeof ( F212_4X ) );

	memset ( F212_7X, '\0', sizeof ( F212_7X ) );

	memset ( F212_9A, '\0', sizeof ( F212_9A ) );
	memset ( F212_9B, '\0', sizeof ( F212_9B ) );
	memset ( F212_9C, '\0', sizeof ( F212_9C ) );
	memset ( F212_9X, '\0', sizeof ( F212_9X ) );

	memset ( F212_10A, '\0', sizeof ( F212_10A ) );
	memset ( F212_10B, '\0', sizeof ( F212_10B ) );
	memset ( F212_10C, '\0', sizeof ( F212_10C ) );
	memset ( F212_10X, '\0', sizeof ( F212_10X ) );

	memset ( F212_11C, '\0', sizeof ( F212_11C ) );
	memset ( F212_11X, '\0', sizeof ( F212_11X ) );

	memset ( F212_12C, '\0', sizeof ( F212_12C ) );
	memset ( F212_12X, '\0', sizeof ( F212_12X ) );

	memset ( F212_13C, '\0', sizeof ( F212_13C ) );
	memset ( F212_13X, '\0', sizeof ( F212_13X ) );

	memset ( F212_14A, '\0', sizeof ( F212_14A ) );
	memset ( F212_14B, '\0', sizeof ( F212_14B ) );
	memset ( F212_14C, '\0', sizeof ( F212_14C ) );
	memset ( F212_14X, '\0', sizeof ( F212_14X ) );

	memset ( F212_15A, '\0', sizeof ( F212_15A ) );
	memset ( F212_15B, '\0', sizeof ( F212_15B ) );
	memset ( F212_15C, '\0', sizeof ( F212_15C ) );
	memset ( F212_15X, '\0', sizeof ( F212_15X ) );

	memset ( F212_16A, '\0', sizeof ( F212_16A ) );
	memset ( F212_16B, '\0', sizeof ( F212_16B ) );
	memset ( F212_16C, '\0', sizeof ( F212_16C ) );
	memset ( F212_16X, '\0', sizeof ( F212_16X ) );

	memset ( F212_17A, '\0', sizeof ( F212_17A ) );
	memset ( F212_17B, '\0', sizeof ( F212_17B ) );
	memset ( F212_17C, '\0', sizeof ( F212_17C ) );
	memset ( F212_17X, '\0', sizeof ( F212_17X ) );

	memset ( F212_18A, '\0', sizeof ( F212_18A ) );
	memset ( F212_18B, '\0', sizeof ( F212_18B ) );
	memset ( F212_18C, '\0', sizeof ( F212_18C ) );
	memset ( F212_18X, '\0', sizeof ( F212_18X ) );

	memset ( F212_19A, '\0', sizeof ( F212_19A ) );
	memset ( F212_19B, '\0', sizeof ( F212_19B ) );
	memset ( F212_19C, '\0', sizeof ( F212_19C ) );
	memset ( F212_19X, '\0', sizeof ( F212_19X ) );

	memset ( F212_20A, '\0', sizeof ( F212_20A ) );
	memset ( F212_20B, '\0', sizeof ( F212_20B ) );
	memset ( F212_20C, '\0', sizeof ( F212_20C ) );
	memset ( F212_20X, '\0', sizeof ( F212_20X ) );

	memset ( F212_21A, '\0', sizeof ( F212_21A ) );
	memset ( F212_21B, '\0', sizeof ( F212_21B ) );
	memset ( F212_21C, '\0', sizeof ( F212_21C ) );
	memset ( F212_21X, '\0', sizeof ( F212_21X ) );

	memset ( F212_22A, '\0', sizeof ( F212_22A ) );
	memset ( F212_22B, '\0', sizeof ( F212_22B ) );
	memset ( F212_22C, '\0', sizeof ( F212_22C ) );
	memset ( F212_22X, '\0', sizeof ( F212_22X ) );
	
	memset ( F212_26A, '\0', sizeof ( F212_26A ) );
	memset ( F212_26B, '\0', sizeof ( F212_26B ) );
	memset ( F212_26C, '\0', sizeof ( F212_26C ) );
	memset ( F212_26X, '\0', sizeof ( F212_26X ) );

	memset ( F212_23A, '\0', sizeof ( F212_23A ) );
	memset ( F212_23B, '\0', sizeof ( F212_23B ) );
	memset ( F212_23C, '\0', sizeof ( F212_23C ) );
	memset ( F212_23X, '\0', sizeof ( F212_23X ) );

	memset ( F212_24A, '\0', sizeof ( F212_24A ) );
	memset ( F212_24B, '\0', sizeof ( F212_24B ) );
	memset ( F212_24C, '\0', sizeof ( F212_24C ) );

	memset ( F212_25A, '\0', sizeof ( F212_25A ) );
	memset ( F212_25B, '\0', sizeof ( F212_25B ) );
	memset ( F212_25C, '\0', sizeof ( F212_25C ) );
	memset ( F212_25X, '\0', sizeof ( F212_25X ) );

	memset ( F211_1D, '\0', sizeof ( F211_1D ) );
	memset ( F211_1E, '\0', sizeof ( F211_1E ) );
	memset ( F211_2F, '\0', sizeof ( F211_2F ) );

	F212_8X = 0;
	F212_8EX = 0;

	F212_10c = F212_12c = F212_19c = F212_20c = F212_23c = 0;

	eymd = 0;

	m_curwnd = -1;
}

CShinFhyo2_12::~CShinFhyo2_12()
{
}

void CShinFhyo2_12::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo2_12Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo2_12Diag02);
	DDX_Control(pDX, IDC_BUTTON1, m_tanaoroshi);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo2_12, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo2_12::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo2_12::OnBnClickedButton2)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo2_12, CSyzBaseDlg)
	ON_EVENT(CShinFhyo2_12, IDC_ICSDIAG8CTRL1, 1, CShinFhyo2_12::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_12, IDC_ICSDIAG8CTRL1, 2, CShinFhyo2_12::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_12, IDC_ICSDIAG8CTRL1, 3, CShinFhyo2_12::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinFhyo2_12, IDC_ICSDIAG8CTRL1, 9, CShinFhyo2_12::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()
// ShinFhyo2_12 メッセージ ハンドラー


BOOL CShinFhyo2_12::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL1 );
	m_BottomIdx = 210;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo2_12::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo2_12::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo2_12::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


BOOL CShinFhyo2_12::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	CWnd *pWnd = GetFocus();

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 付表２－２更新
			GetAllDiagData();
			WriteData();

			if ( m_pParent ) {
				m_pParent->PostMessageA ( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}

		if ( pMsg->wParam == VK_F2 ) {
			DIAGRAM_DATA  dd;
			if ( ( m_curwnd != -1 ) && ( pWnd != GetDlgItem ( IDC_BUTTON1 ) ) ) {
				short result = ( ( CIcsdiagctrl* )GetDlgItem ( m_curwnd ) ) ->GetPosition();
				diag_getdata ( m_curwnd, result, &dd, CIcsdiagctrl );
				if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
					TerminationIcsdiag8ctrl2 ( result, ( short  )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}
				return TRUE;
			}
		}

		if ( pWnd == GetDlgItem ( IDC_BUTTON1 ) ) {
			char VK_FLG = 0x00;
			VK_FLG = ( char )::GetKeyState ( VK_SHIFT );

			if ( ( pMsg->wParam == VK_F3 ) || ( pMsg->wParam == VK_RIGHT ) || ( pMsg->wParam == VK_DOWN ) || ( ( pMsg->wParam == VK_TAB ) && !( VK_FLG & 0x80 ) ) ) {
				if( *m_pShinInfo->pOver500MillionSw != 1 && F212_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){	//5億円かつ課税割合が95％以上の場合
					diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_21A, CIcsdiagctrl );
				}
				else {
					if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
						if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
							diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_17A, CIcsdiagctrl );
						}
						else {
							diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_20A, CIcsdiagctrl );
						}
					}
					else {
						diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_21A, CIcsdiagctrl );
					}
				}
				return TRUE;
			}

			// 前項目へ
			if ( ( pMsg->wParam == VK_LEFT ) || ( pMsg->wParam == VK_F2 ) || ( pMsg->wParam == VK_UP ) || ( ( pMsg->wParam == VK_TAB ) && ( VK_FLG & 0x80 ) ) ) {
				if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
					diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_13C, CIcsdiagctrl );
				}
				else {
					if ( eymd >= ICS_SH_KYOJU_INPUT_DAY ) {
						diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_26C, CIcsdiagctrl );
					}
					else {
						diag_setposition ( IDC_ICSDIAG8CTRL1, ID212_22C, CIcsdiagctrl );
					}
				}
				return TRUE;
			}
		}

	}
	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

void CShinFhyo2_12::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;

}

void CShinFhyo2_12::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char CkMy[MONY_BUF_SIZE] = { 0 };
	char buf[512] = { 0 };
	int length = 0;
	int	tmpUWexp = 0;

	char VAL[MONY_BUF_SIZE] = {0};
	char ValCnv[MONY_BUF_SIZE] = {0};

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
	case ID212_1A:
	case ID212_1B:
	case ID212_1C:
		SyukeiDataSet ( index );

		// 課税売上高の５億超えチェック
		CheckOver500Million();
		// 各入力の表示
		F212data_all ( 1 );
		break;

	case ID212_9A:
	case ID212_9B:
	case ID212_9C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 9 );
		break;

	case ID212_10A:
	case ID212_10B:
	case ID212_10C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 10 );
		break;

	case ID212_11C:
		SyukeiDataSet  (index );

		// 各入力の表示
		F212data_all ( 11 );
		break;

	case ID212_12C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 12 );
		break;

	case ID212_13C:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F212_13C, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F212_13C, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F212_13C, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) 
				&& ( F212_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
				// 個別対応で95％未満の場合
				CString		Msg;
				Msg.Empty();
				Msg = _T( "課税貨物に係る消費税額が変更されました。\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 7 );
		break;


	case ID212_14A:
	case ID212_14B:
	case ID212_14C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 7 );
		break;

	case ID212_17A:
	case ID212_17B:
	case ID212_17C:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F212_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 8 );
		break;

	case ID212_18A:
	case ID212_18B:
	case ID212_18C:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F212_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 8 );
		break;

	case ID212_19A:
	case ID212_19B:
	case ID212_19C:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F212_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 8 );
		break;

	case ID212_20A:
	case ID212_20B:
	case ID212_20C:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F212_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 8 );
		break;

	case ID212_21A:
	case ID212_21B:
	case ID212_21C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 12 );
		break;

	case ID212_22A:
	case ID212_22B:
	case ID212_22C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 13 );
		break;

	case ID212_26A:
	case ID212_26B:
	case ID212_26C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 13 );
		break;

	case ID212_23A:
	case ID212_23B:
	case ID212_23C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 13 );
		break;

	case ID212_24A:
	case ID212_24B:
	case ID212_24C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 13 );
		break;

	case ID212_25A:
	case ID212_25B:
	case ID212_25C:
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 14 );
		break;

	case ID212_8EX:			// 課税売上割合の変更チェック
		( *m_pSnHeadData )->Sn_UPERS &= 0xef;
		diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		if ( DiagData.data_check ) {
			( *m_pSnHeadData )->Sn_UPERS |= 0x10;
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "320800" ) );
		}

		F212data_8();
		SyukeiDataSet ( index );

		// 各入力の表示
		F212data_all ( 4 );
		break;

	case ID212_8X:			// 課税売上割合　合計 F
		diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_8X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		length = DiagData.data_edit.GetLength();
		memset ( buf, '\0', sizeof ( buf ) ); 
		tmpUWexp = 0;
		if ( length ) {
			memmove ( buf, DiagData.data_edit, length );
		}
		m_Util.perinp ( 3, 2, buf, ( short* )&tmpUWexp );
		if ( tmpUWexp <= 10000 ) {
			( *m_pSnFhyo2_02Data )->Sn_F202_8X_UW = tmpUWexp;
		}

		// 個別対応で95％未満の場合
		if ( ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) && ( F212_8X != ( *m_pSnFhyo2_02Data )->Sn_F202_8X_UW ) ) {
			CString		Msg;
			Msg.Empty();
			Msg = _T( "課税売上割合が変更されました。\n" );
			Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
			ICSMessageBox ( Msg, MB_OK, 0 );
			// 各入力の計算表示
			F212data_all ( 4 );
			break;
		}
		else {
			// 個別対応で95％未満の場合
			if ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) && ( F212_8X != ( *m_pSnFhyo2_02Data )->Sn_F202_8X_UW )
				&& ( ( *m_pSnFhyo2_02Data )->Sn_F202_8X_UW < ( *m_pSnHeadData )->m_UriRatioBorder ) ) {
				CString		Msg;
				Msg.Empty();
				Msg = _T( "課税売上割合が変更されました。\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
		}

		SyukeiDataSet ( index );

		// 各入力の計算表示
		F212data_all ( 4 );

		break;

	default:
		break;

	}
}

void CShinFhyo2_12::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	short wTerm;
	char VK_FLG = 0x00;
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}
	int pos = 0;
	if ( wTerm==VK_UP ) {
		pos = UpDownFocusMoveCheck ( index , 1);
		m_Fhyo2_12Diag01.SetPosition ( pos );
	}
	if ( wTerm==VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index , 0 );
		m_Fhyo2_12Diag01.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) ){
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
			if ( index == ID212_13C ) {
				c_set ( IDC_BUTTON1 );
			}
			else {
				m_Fhyo2_12Diag01.SetNextPosition();
			}
		}
		else {
			if ( index == ID212_20C ) {
				c_set ( IDC_BUTTON1 );
			}
			else{
				m_Fhyo2_12Diag01.SetNextPosition();
			}

		}

	}

	// 前項目へ
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		BOOL IsPrevPosition = TRUE;
		if ( ( *m_pShinInfo->pOver500MillionSw != 1 ) && ( F212_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) ) {	//5億円以下かつ 課税売上割合が９５％以上の場合
			if ( index == ID212_19A ) {
				c_set ( IDC_BUTTON1 );
				IsPrevPosition = FALSE;
			}
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
					if ( index == ID212_17A ) {
						c_set ( IDC_BUTTON1 );
						IsPrevPosition = FALSE;
					}
				}
				else {
					if ( index == ID212_20A ) {
						c_set ( IDC_BUTTON1 );
						IsPrevPosition = FALSE;
					}
				}
			}
			else {
				if ( index == ID212_21A ) {
					c_set ( IDC_BUTTON1 );
					IsPrevPosition = FALSE;
				}
			}
		}
		if ( IsPrevPosition ) {
			m_Fhyo2_12Diag01.SetPrevPosition();
		}

	}
	if ( wTerm == VK_DELETE ) {
		switch( index ){
		case ID212_23A:
			F212_23c &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322301" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322401" ) );
			// 各入力の表示
			F212data_all ( 13 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_23A );
			break;

		case ID212_23B:
			F212_23c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322302" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322402" ) );
			// 各入力の表示
			F212data_all ( 13 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_23B );
			break;

		case ID212_23C:
			F212_23c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322303" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322403" ) );
			// 各入力の表示
			F212data_all ( 13 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_23C );
			break;

		case ID212_10A:
			F212_10c &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321001" ) );
			// 各入力の表示
			F212data_all ( 6 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_10A );
			break;

		case ID212_10B:
			F212_10c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321002" ) );
			// 各入力の表示
			F212data_all ( 6 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_10B );
			break;

		case ID212_10C:
			F212_10c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321003" ) );
			// 各入力の表示
			F212data_all ( 6 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_10C );
			break;

		case ID212_12C:
			F212_12c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321203" ) );
			// 各入力の表示
			F212data_all ( 16 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_12C );
			break;

		case ID212_19A:
			F212_19c &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321901" ) );
			// 各入力の表示
			F212data_all ( 8 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_19A );
			break;

		case ID212_19B:
			F212_19c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321902" ) );
			// 各入力の表示
			F212data_all ( 8 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_19B );
			break;

		case ID212_19C:
			F212_19c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321903" ) );
			// 各入力の表示
			F212data_all ( 8 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_19C );
			break;

		case ID212_20A:
			F212_20c &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322001" ) );
			// 各入力の表示
			F212data_all( 8 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_20A );
			break;

		case ID212_20B:
			F212_20c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322002" ) );
			// 各入力の表示
			F212data_all ( 8 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_20B );
			break;

		case ID212_20C:
			F212_20c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322003" ) );
			// 各入力の表示
			F212data_all ( 8 );
			m_Fhyo2_12Diag01.SetPosition ( ID212_20C );
			break;

		default:
			dsp_cls ( IDC_ICSDIAG8CTRL1 , index );
			break;
		}

	}

	int nowpos = 0;
	CWnd* pWnd = this->GetFocus();
	if ( pWnd->GetDlgCtrlID() == IDC_BUTTON1 ) {
		nowpos = ID212_14X;	// 14Xのポジションをセット
	}
	else {
		nowpos = m_Fhyo2_12Diag01.GetPosition();	// 移動先または自身のポジション
	}

	RECT	rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int	cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

void CShinFhyo2_12::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char flg[MONY_BUF_SIZE] = { 0 };

	// 課税売上割合の変更チェック
	if ( index == ID212_8EX ) {
		diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) {
			CString	Msg;
			Msg.Empty();
			if ( DiagData.data_check ) {
				Msg = _T( "課税売上割合に準ずる割合を適用する為に、\n" );
				Msg += _T( "課税売上割合を変更される場合は・・・[はい]を、\n" );
				Msg += _T( "それ以外の理由で変更される場合は・・・[いいえ]を、\n" );
				Msg += _T( "選択してください。" );
				int st = ICSMessageBox ( Msg, MB_YESNO, 0 );
				if ( st == IDYES ) {
					( *m_pSnHeadData )->Sn_UPERS |= 0x02;
				}
			}
			else {
				if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) {
					Msg = _T( "課税売上割合に準ずる割合を無効にします。" );
					ICSMessageBox ( Msg, MB_OK, 0 );
					( *m_pSnHeadData )->Sn_UPERS &= 0xfd;

				}
			}
			if ( ( DiagData.data_check && ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) == 0x00 ) ) ||
				( ( DiagData.data_check == 0 ) && ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) ) {
				CString	 Msg;
				Msg.Empty();
				Msg = _T( "課税売上割合を変更する場合は、\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
		}
	}
}


//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo2_02Data	：	付表2-2データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSnTanaData		：	棚卸データクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSyzSyukei		：	集計クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo2_12::InitInfo ( CH31SnFhyo2_02Data **pSnFhyo2_02Data, CSnHeadData **pSnHeadData, CH31SnTanaData **pSnTanaData , CArithEx *pArith, SYC_SHININFO *pShinInfo, CSyzSyukei **pSyzSyukei )
{
	ASSERT ( pSnFhyo2_02Data );
	if ( pSnFhyo2_02Data == NULL ) {
		return -1;
	}
	m_pSnFhyo2_02Data = pSnFhyo2_02Data;


	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;


	ASSERT ( pShinInfo );
	if ( pShinInfo == NULL ) {
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT ( pArith );
	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT ( pSnTanaData );
	if ( pSnTanaData == NULL ) {
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	return 0;
}

//=========================================================
// 初期化
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo2_12::Init()
{
	eymd = 0;
	if ( !( ( *m_pSnHeadData )->Sn_SKKBN % 2 ) ) {
		eymd = ( *m_pSnHeadData )->Sn_MDAYE;
	}
	else {
		eymd = ( *m_pSnHeadData )->Sn_KDAYE;
	}


	// データ読込み
	ReadData();
	ReadData2();

	// 特定収入連動メッセージ非表示
	m_Fhyo2_12Diag01.ModifyItem ( ID212_TS, DIAG_MDFY_TRANSPARENT );

	// 集計
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();
	diag_modify ( IDC_ICSDIAG8CTRL1, ID212_8EX, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo2_12Diag02.ModifyItem ( 5, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh2_12();
	ChangeBtnDisp();

	// 入力ポジションセット
	SetItemPosition();

	return 0;
}

//=========================================================
// データ読込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo2_12::ReadData()
{
		// 1.課税売上額(税抜き)
	memmove ( F212_1A, ( *m_pSnFhyo2_02Data )->Sn_F202_1A, MONY_BUF_SIZE );
	memmove ( F212_1B, ( *m_pSnFhyo2_02Data )->Sn_F202_1B, MONY_BUF_SIZE );
	memmove ( F212_1C, ( *m_pSnFhyo2_02Data )->Sn_F202_1C, MONY_BUF_SIZE );
	memmove ( F212_1X, ( *m_pSnFhyo2_02Data )->Sn_F202_1X, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove ( F212_4X, ( *m_pSnFhyo2_02Data )->Sn_F202_4X, MONY_BUF_SIZE );

	// 7.資産の譲渡等の対価の額
	memmove ( F212_7X, ( *m_pSnFhyo2_02Data )->Sn_F202_7X, MONY_BUF_SIZE );

	// 8.課税売上割合
	F212_8EX = ( unsigned short )( *m_pSnFhyo2_02Data )->Sn_F202_8EX;
	F212_8X = ( *m_pSnFhyo2_02Data )->Sn_F202_8X_UW;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove ( F212_9A, ( *m_pSnFhyo2_02Data )->Sn_F202_9A, MONY_BUF_SIZE );
	memmove ( F212_9B, ( *m_pSnFhyo2_02Data )->Sn_F202_9B, MONY_BUF_SIZE );
	memmove ( F212_9C, ( *m_pSnFhyo2_02Data )->Sn_F202_9C, MONY_BUF_SIZE );
	memmove ( F212_9X, ( *m_pSnFhyo2_02Data )->Sn_F202_9X, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove ( F212_10A, ( *m_pSnFhyo2_02Data )->Sn_F202_10A, MONY_BUF_SIZE );
	memmove ( F212_10B, ( *m_pSnFhyo2_02Data )->Sn_F202_10B, MONY_BUF_SIZE );
	memmove ( F212_10C, ( *m_pSnFhyo2_02Data )->Sn_F202_10C, MONY_BUF_SIZE );
	memmove ( F212_10X, ( *m_pSnFhyo2_02Data )->Sn_F202_10X, MONY_BUF_SIZE );
	F212_10c = ( *m_pSnFhyo2_02Data )->Sn_F202_10sw;

	// 11.特定課税仕入れに係る支払対価の額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F212_11C, ( *m_pSnFhyo2_02Data )->Sn_F202_11C, MONY_BUF_SIZE );
		memmove ( F212_11X, ( *m_pSnFhyo2_02Data )->Sn_F202_11X, MONY_BUF_SIZE );
	}
	else {
		memset ( F212_11C, '\0', MONY_BUF_SIZE );
		memset ( F212_11X, '\0', MONY_BUF_SIZE );
	}

	// 12.特定課税仕入れに係る消費税額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F212_12C, ( *m_pSnFhyo2_02Data )->Sn_F202_12C, MONY_BUF_SIZE );
		memmove ( F212_12X, ( *m_pSnFhyo2_02Data )->Sn_F202_12X, MONY_BUF_SIZE );
		F212_12c = ( *m_pSnFhyo2_02Data )->Sn_F202_12sw;
	}
	else {
		memset ( F212_12C, '\0', MONY_BUF_SIZE );
		memset ( F212_12X, '\0', MONY_BUF_SIZE );
		F212_12c = 0;
	}

	// 13.課税貨物に係る消費税額
	memmove ( F212_13C, ( *m_pSnFhyo2_02Data )->Sn_F202_13C, MONY_BUF_SIZE );
	memmove ( F212_13X, ( *m_pSnFhyo2_02Data )->Sn_F202_13X, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove ( F212_14A, ( *m_pSnFhyo2_02Data )->Sn_F202_14A, MONY_BUF_SIZE );
	memmove ( F212_14B, ( *m_pSnFhyo2_02Data )->Sn_F202_14B, MONY_BUF_SIZE );
	memmove ( F212_14C, ( *m_pSnFhyo2_02Data )->Sn_F202_14C, MONY_BUF_SIZE );
	memmove ( F212_14X, ( *m_pSnFhyo2_02Data )->Sn_F202_14X, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove ( F212_15A, ( *m_pSnFhyo2_02Data )->Sn_F202_15A, MONY_BUF_SIZE );
	memmove ( F212_15B, ( *m_pSnFhyo2_02Data )->Sn_F202_15B, MONY_BUF_SIZE );
	memmove ( F212_15C, ( *m_pSnFhyo2_02Data )->Sn_F202_15C, MONY_BUF_SIZE );
	memmove ( F212_15X, ( *m_pSnFhyo2_02Data )->Sn_F202_15X, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove ( F212_16A, ( *m_pSnFhyo2_02Data )->Sn_F202_16A, MONY_BUF_SIZE );
	memmove ( F212_16B, ( *m_pSnFhyo2_02Data )->Sn_F202_16B, MONY_BUF_SIZE );
	memmove ( F212_16C, ( *m_pSnFhyo2_02Data )->Sn_F202_16C, MONY_BUF_SIZE );
	memmove ( F212_16X, ( *m_pSnFhyo2_02Data )->Sn_F202_16X, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove ( F212_17A, ( *m_pSnFhyo2_02Data )->Sn_F202_17A, MONY_BUF_SIZE );
	memmove ( F212_17B, ( *m_pSnFhyo2_02Data )->Sn_F202_17B, MONY_BUF_SIZE );
	memmove ( F212_17C, ( *m_pSnFhyo2_02Data )->Sn_F202_17C, MONY_BUF_SIZE );
	memmove ( F212_17X, ( *m_pSnFhyo2_02Data )->Sn_F202_17X, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove ( F212_18A, ( *m_pSnFhyo2_02Data )->Sn_F202_18A, MONY_BUF_SIZE );
	memmove ( F212_18B, ( *m_pSnFhyo2_02Data )->Sn_F202_18B, MONY_BUF_SIZE );
	memmove ( F212_18C, ( *m_pSnFhyo2_02Data )->Sn_F202_18C, MONY_BUF_SIZE );
	memmove ( F212_18X, ( *m_pSnFhyo2_02Data )->Sn_F202_18X, MONY_BUF_SIZE );

	F212_19c = F212_20c = 0x00;
	// 19.個別対応方式により控除する課税仕入れ等の税額
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_19A ) ) {
		memmove ( F212_19A, ( *m_pSnFhyo2_02Data )->Sn_F202_19A, MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_19B ) ) {
		memmove ( F212_19B, ( *m_pSnFhyo2_02Data )->Sn_F202_19B, MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_19C ) ) {
		memmove ( F212_19C, ( *m_pSnFhyo2_02Data )->Sn_F202_19C, MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_19X ) ) {
		memmove ( F212_19X, ( *m_pSnFhyo2_02Data )->Sn_F202_19X, MONY_BUF_SIZE );
	}
	F212_19c = ( *m_pSnFhyo2_02Data )->Sn_F202_19sw;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_20A ) ) {
		memmove ( F212_20A, ( *m_pSnFhyo2_02Data )->Sn_F202_20A, MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_20B ) ) {
		memmove ( F212_20B, ( *m_pSnFhyo2_02Data )->Sn_F202_20B, MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_20C ) ) {
		memmove ( F212_20C, ( *m_pSnFhyo2_02Data )->Sn_F202_20C, MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( ( *m_pSnFhyo2_02Data )->Sn_F202_20X ) ) {
		memmove ( F212_20X, ( *m_pSnFhyo2_02Data )->Sn_F202_20X, MONY_BUF_SIZE );
	}
	F212_20c = ( *m_pSnFhyo2_02Data )->Sn_F202_20sw;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove ( F212_21A, ( *m_pSnFhyo2_02Data )->Sn_F202_21A, MONY_BUF_SIZE );
	memmove ( F212_21B, ( *m_pSnFhyo2_02Data )->Sn_F202_21B, MONY_BUF_SIZE );
	memmove ( F212_21C, ( *m_pSnFhyo2_02Data )->Sn_F202_21C, MONY_BUF_SIZE );
	memmove ( F212_21X, ( *m_pSnFhyo2_02Data )->Sn_F202_21X, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove ( F212_22A, ( *m_pSnFhyo2_02Data )->Sn_F202_22A, MONY_BUF_SIZE );
	memmove ( F212_22B, ( *m_pSnFhyo2_02Data )->Sn_F202_22B, MONY_BUF_SIZE );
	memmove ( F212_22C, ( *m_pSnFhyo2_02Data )->Sn_F202_22C, MONY_BUF_SIZE );
	memmove ( F212_22X, ( *m_pSnFhyo2_02Data )->Sn_F202_22X, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	memmove ( F212_26A, ( *m_pSnFhyo2_02Data )->Sn_F202_26A, MONY_BUF_SIZE );
	memmove ( F212_26B, ( *m_pSnFhyo2_02Data )->Sn_F202_26B, MONY_BUF_SIZE );
	memmove ( F212_26C, ( *m_pSnFhyo2_02Data )->Sn_F202_26C, MONY_BUF_SIZE );
	memmove ( F212_26X, ( *m_pSnFhyo2_02Data )->Sn_F202_26X, MONY_BUF_SIZE );

	// 24.控除対象仕入税額
	F212_23c = ( *m_pSnFhyo2_02Data )->Sn_F202_23sw;
	memmove ( F212_23A, ( *m_pSnFhyo2_02Data )->Sn_F202_23A, MONY_BUF_SIZE );
	memmove ( F212_23B, ( *m_pSnFhyo2_02Data )->Sn_F202_23B, MONY_BUF_SIZE );
	memmove ( F212_23C, ( *m_pSnFhyo2_02Data )->Sn_F202_23C, MONY_BUF_SIZE );
	memmove ( F212_23X, ( *m_pSnFhyo2_02Data )->Sn_F202_23X, MONY_BUF_SIZE );

	// 25.控除過大調整税額
	memmove ( F212_24A, ( *m_pSnFhyo2_02Data )->Sn_F202_24A, MONY_BUF_SIZE );
	memmove ( F212_24B, ( *m_pSnFhyo2_02Data )->Sn_F202_24B, MONY_BUF_SIZE );
	memmove ( F212_24C, ( *m_pSnFhyo2_02Data )->Sn_F202_24C, MONY_BUF_SIZE );
	memmove ( F212_24X, ( *m_pSnFhyo2_02Data )->Sn_F202_24X, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	memmove ( F212_25A, ( *m_pSnFhyo2_02Data )->Sn_F202_25A, MONY_BUF_SIZE );
	memmove ( F212_25B, ( *m_pSnFhyo2_02Data )->Sn_F202_25B, MONY_BUF_SIZE );
	memmove ( F212_25C, ( *m_pSnFhyo2_02Data )->Sn_F202_25C, MONY_BUF_SIZE );
	memmove ( F212_25X, ( *m_pSnFhyo2_02Data )->Sn_F202_25X, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo2_12::ReadData2()
{
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {//非連動の場合
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911101" ), F212_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911102" ), F212_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911103" ), F212_1C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320101" ), F212_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320102" ), F212_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320103" ), F212_1C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320901" ), F212_9A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320902" ), F212_9B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320903" ), F212_9C );

		if ( F212_10c & 0x08 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321001" ), F212_10A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321001" ) );
		}

		if ( F212_10c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321002" ), F212_10B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321002" ) );
		}

		if ( F212_10c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321003" ), F212_10C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321003" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321103" ), F212_11C );

		if ( F212_12c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321203" ), F212_12C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321203" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321303" ), F212_13C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F212_14A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F212_14B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F212_14C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), F212_17A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), F212_17B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), F212_17C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), F212_18A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), F212_18B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), F212_18C );

		if ( F212_19c & 0x08 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321901" ), F212_19A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321901" ) );
		}

		if ( F212_19c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321902" ), F212_19B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321902" ) );
		}

		if ( F212_19c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData (_T( "321903" ), F212_19C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321903" ) );
		}

		if ( F212_20c & 0x08 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322001" ), F212_20A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322001" ) );
		}

		if ( F212_20c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322002" ), F212_20B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322002" ) );
		}

		if ( F212_20c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322003" ), F212_20C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322003" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322101" ), F212_21A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322102" ), F212_21B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322103" ), F212_21C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322201" ), F212_22A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322202" ), F212_22B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322203" ), F212_22C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326101" ), F212_26A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326102" ), F212_26B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326103" ), F212_26C );

		if( F212_23c&0x08 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322301" ), F212_23A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322301" ) );
		}

		if ( F212_23c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), F212_23B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322302" ) );
		}

		if ( F212_23c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), F212_23C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322303" ) );
		}

		if ( F212_23c & 0x08 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322401" ), F212_24A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322401" ) );
		}

		if ( F212_23c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), F212_24B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322402" ) );
		}

		if ( F212_23c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403"), F212_24C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322403" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322501" ), F212_25A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322502" ), F212_25B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322503" ), F212_25C );
	}
	else {
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322101" ), F212_21A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322102" ), F212_21B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322103" ), F212_21C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322201" ), F212_22A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322202" ), F212_22B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322203" ), F212_22C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326101" ), F212_26A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326102" ), F212_26B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326103" ), F212_26C );
	}
}

//=========================================================
// データ書込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo2_12::WriteData()
{
	// 1.課税売上額(税抜き)
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_1A, F212_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_1B, F212_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_1C, F212_1C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_1X, F212_1X, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_4X, F212_4X, MONY_BUF_SIZE );

	// 7.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_7X, F212_7X, MONY_BUF_SIZE );

	// 8.課税売上割合
	( unsigned short )( *m_pSnFhyo2_02Data )->Sn_F202_8EX = F212_8EX;
	( *m_pSnHeadData )->Sn_UPERS &= 0xfe;
	if ( ( ( *m_pSnFhyo2_02Data )->Sn_F202_8EX ) < ( *m_pSnHeadData )->m_UriRatioBorder ) {
		( *m_pSnHeadData )->Sn_UPERS |= 0x01;
	}

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_9A, F212_9A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_9B, F212_9B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_9C, F212_9C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_9X, F212_9X, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_10A, F212_10A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_10B, F212_10B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_10C, F212_10C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_10X, F212_10X, MONY_BUF_SIZE );
	( *m_pSnFhyo2_02Data )->Sn_F202_10sw = F212_10c;

	// 11.特定課税仕入れに係る支払対価の額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_11C, F212_11C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_11X, F212_11X, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_12C, F212_12C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_12X, F212_12X, MONY_BUF_SIZE );
	( *m_pSnFhyo2_02Data )->Sn_F202_12sw = F212_12c;

	// 13.課税貨物に係る消費税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_13C, F212_13C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_13X, F212_13X, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_14A, F212_14A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_14B, F212_14B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_14C, F212_14C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_14X, F212_14X, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_15A, F212_15A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_15B, F212_15B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_15C, F212_15C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_15X, F212_15X, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_16A, F212_16A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_16B, F212_16B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_16C, F212_16C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_16X, F212_16X, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_17A, F212_17A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_17B, F212_17B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_17C, F212_17C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_17X, F212_17X, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_18A, F212_18A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_18B, F212_18B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_18C, F212_18C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_18X, F212_18X, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_19X, F212_19X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_19A, F212_19A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_19B, F212_19B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_19C, F212_19C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_02Data )->Sn_F202_19sw = F212_19c;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_20X, F212_20X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_20A, F212_20A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_20B, F212_20B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_20C, F212_20C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_02Data )->Sn_F202_20sw = F212_20c;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_21A, F212_21A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_21B, F212_21B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_21C, F212_21C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_21X, F212_21X, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_22A, F212_22A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_22B, F212_22B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_22C, F212_22C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_22X, F212_22X, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_26A, F212_26A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_26B, F212_26B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_26C, F212_26C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_26X, F212_26X, MONY_BUF_SIZE );

	// 24.控除対象仕入税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_23A, F212_23A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_23B, F212_23B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_23C, F212_23C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_23X, F212_23X, MONY_BUF_SIZE );
	( *m_pSnFhyo2_02Data )->Sn_F202_23sw = F212_23c;

	// 25.控除過大調整税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_24A, F212_24A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_24B, F212_24B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_24C, F212_24C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_24X, F212_24X, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_25A, F212_25A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_25B, F212_25B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_25C, F212_25C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_02Data )->Sn_F202_25X, F212_25X, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo2_12::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目を全て再取得
	MoneyBasejagArray	money;

	SYZTOSPC_DATA SyzToSpc = { 0 };

	// 個別対応方式か
	BOOL Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();

	int Flg = 0;

	// 課税売上割合に準ずる割合が有効か
	int semiuri = (*m_pSnHeadData)->IsUseSemiUriRatio();

	TanaDataAdd();

	int		dmysw = 0;
	int		tmpFlg = 0;
	char	VAL[MONY_BUF_SIZE] = { 0 };
	char	ValCnv[MONY_BUF_SIZE] = { 0 };
	tmpFlg = CheckOver500MillionSub ( dmysw, VAL, ValCnv );
	if ( tmpFlg == 0 ) {
		Flg = 1;
	}

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "32" ) );
	int cnt = ( int )money.GetCount();
	int	incnt = 0;

	for ( int l = 0; l < cnt; l++ ) {
		incnt = ( int )money[l].GetCount();
		for ( int m = 0; m < incnt; m++ ) {
			if ( money[l][m].code == _T( "320100" ) ) {
				memmove ( F212_1X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "320400" ) ) {
				memmove ( F212_4X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "320700" ) ) {
				memmove ( F212_7X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "320800" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai,  money[l][m].arith, _T("SSSSSSSSSSSSS9") );
				F212_8EX = atoi(wariai);
				F212_8X = atoi(wariai);
			}
			else if ( money[l][m].code == _T( "320900" ) ) {
				memmove ( F212_9X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "320901" ) ) {
				memmove ( F212_9A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "320902" ) ) {
				memmove ( F212_9B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "320903" ) ) {
				memmove ( F212_9C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321001" ) ) {
				memmove ( F212_10A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321002" ) ) {
				memmove ( F212_10B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321003" ) ) {
				memmove ( F212_10C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321000" ) ) {
				memmove ( F212_10X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321103" ) ) {
				memmove ( F212_11C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321100" ) ) {
				memmove ( F212_11X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321203" ) ) {
				memmove ( F212_12C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321200" ) ) {
				memmove ( F212_12X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321303" ) ) {
				memmove ( F212_13C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321300" ) ) {
				memmove ( F212_13X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321401" ) ) {
				memmove ( F212_14A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321402" ) ) {
				memmove ( F212_14B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321403" ) ) {
				memmove ( F212_14C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321400" ) ) {
				memmove ( F212_14X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321501" ) ) {
				memmove ( F212_15A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321502" ) ) {
				memmove ( F212_15B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321503" ) ) {
				memmove ( F212_15C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321500" ) ) {
				memmove ( F212_15X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321601" ) ) {
				memmove ( F212_16A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321602" ) ) {
				memmove ( F212_16B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321603" ) ) {
				memmove ( F212_16C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321600" ) ) {
				memmove ( F212_16X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "321701" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_17A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_17A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321702" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_17B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_17B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321703" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_17C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_17C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321700" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_17X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_17X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321801" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_18A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else  {
					memset ( F212_18A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321802" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_18B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_18B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321803" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_18C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_18C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321800" ) ) {
				if ( Kobetsu && ( Flg || semiuri) ) {
					memmove ( F212_18X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_18X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321901" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_19A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_19A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321902" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_19B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_19B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321903" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_19C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_19C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "321900" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F212_19X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_19X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "322001" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F212_20A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_20A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "322002" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F212_20B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_20B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "322003" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F212_20C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_20C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "322000" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F212_20X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F212_20X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "322101" ) ) {
				memmove ( F212_21A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322102" ) ) {
				memmove ( F212_21B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322103" ) ) {
				memmove ( F212_21C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322100" ) ) {
				memmove ( F212_21X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322201" ) ) {
				memmove ( F212_22A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322202" ) ) {
				memmove ( F212_22B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322203" ) ) {
				memmove ( F212_22C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322200" ) ) {
				memmove ( F212_22X, money[l][m].arith, MONY_BUF_SIZE );
			}	
			else if ( money[l][m].code == _T( "326101" ) ) {
				memmove ( F212_26A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "326102" ) ) {
				memmove ( F212_26B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "326103" ) ) {
				memmove ( F212_26C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "326100" ) ) {
				memmove ( F212_26X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322301" ) ) {
				memmove ( F212_23A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322302" ) ) {
				memmove ( F212_23B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322303" ) ) {
				memmove ( F212_23C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322300" ) ) {
				memmove ( F212_23X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322401" ) ) {
				memmove ( F212_24A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322402" ) ) {
				memmove ( F212_24B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322403" ) ) {
				memmove ( F212_24C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322400" ) ) {
				memmove ( F212_24X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322501" ) ) {
				memmove ( F212_25A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322502" ) ) {
				memmove ( F212_25B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322503" ) ) {
				memmove ( F212_25C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322500" ) ) {
				memmove ( F212_25X, money[l][m].arith, MONY_BUF_SIZE );
			}
	
			// 特定収入計算表対応
			if ( *( m_pShinInfo )->pSgSpc == 1 ) {
				if ( money[l][m].code == _T( "322700" ) ) {
					m_pArith->l_add ( SyzToSpc.SpMzur, SyzToSpc.SpMzur, money[l][m].arith );
				}
				else if ( money[l][m].code == _T( "322800" ) ) {
					m_pArith->l_add ( SyzToSpc.SpMzur, SyzToSpc.SpMzur, money[l][m].arith );
				}				
				else if ( money[l][m].code == _T( "323100" ) ) {
					memmove ( SyzToSpc.SpHkur, money[l][m].arith, MONY_BUF_SIZE );
				}

				else if ( money[l][m].code == _T( "322900" ) ) {
					memmove ( SyzToSpc.SpKuws, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "323200" ) ) {
					memmove ( SyzToSpc.SpKuwb, money[l][m].arith, MONY_BUF_SIZE );
				}

				//else if ( !Flg ) {
				if ( !Flg && !semiuri ) {
					if ( money[l][m].code == _T( "324102" ) ) {
						memmove ( SyzToSpc.SpKgz624, money[l][m].arith, MONY_BUF_SIZE );
					}
					else if ( money[l][m].code == _T( "324103" ) ) {
						memmove ( SyzToSpc.SpKgz78, money[l][m].arith, MONY_BUF_SIZE );
					}
				}
				else {
					if ( Kobetsu ) {
						if ( Flg || semiuri ) {
							if ( money[l][m].code == _T( "324402" ) ) {
								memmove ( SyzToSpc.SpKgz624, money[l][m].arith, MONY_BUF_SIZE );
							}
							else if ( money[l][m].code == _T( "324403" ) ) {
								memmove ( SyzToSpc.SpKgz78, money[l][m].arith, MONY_BUF_SIZE );
							}
						}
					}
					else {
						if ( Flg ) {
							if ( money[l][m].code == _T( "324502" ) ) {
								memmove ( SyzToSpc.SpKgz624, money[l][m].arith, MONY_BUF_SIZE );
							}
							else if ( money[l][m].code == _T( "324503" ) ) {
								memmove ( SyzToSpc.SpKgz78, money[l][m].arith, MONY_BUF_SIZE );
							}
						}
					}
				}
			}
		}
	}


	// 特定収入計算表対応
	if ( *( m_pShinInfo )->pSgSpc == 1 ) {
		F212calq_spc ( SyzToSpc );
	}

	return 0;
}

//=========================================================
// 集計モジュール更新
//---------------------------------------------------------
//	引数
//			cID			：	ダイアグラム内インデックス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo2_12::SyukeiDataSet ( int cID )
{
	char val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL1, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID212_1A:
		memmove ( F212_1A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320101" ), F212_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911101" ), F212_1A );
		break;
	case ID212_1B:
		memmove ( F212_1B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320102" ), F212_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911102" ), F212_1B );
		break;
	case ID212_1C:
		memmove ( F212_1C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320103" ), F212_1C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911103" ), F212_1C );
		break;
	case ID212_9A:
		memmove ( F212_9A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320901" ), F212_9A );
		break;
	case ID212_9B:
		memmove ( F212_9B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320902" ), F212_9B );
		break;
	case ID212_9C:
		memmove ( F212_9C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "320903" ), F212_9C );
		break;
	case ID212_10A:
		if ( m_pArith->l_cmp ( F212_10A, val ) ) {
			F212_10c |= 0x08;
			memmove ( F212_10A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321001" ), F212_10A );
		}
		break;
	case ID212_10B:
		if ( m_pArith->l_cmp ( F212_10B, val ) ) {
			F212_10c |= 0x80;
			memmove ( F212_10B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321002" ), F212_10B );
		}
		break;
	case ID212_10C:
		if ( m_pArith->l_cmp ( F212_10C, val ) ) {
			F212_10c |= 0x800;
			memmove ( F212_10C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321003" ), F212_10C );
		}
		break;
	case ID212_11C:
		memmove ( F212_11C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321103" ), F212_11C );
		break;
	case ID212_12C:
		if ( m_pArith->l_cmp ( F212_12C, val ) ) {
			F212_12c |= 0x800;
			memmove ( F212_12C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321203" ), F212_12C );
		}
		break;
	case ID212_13C:
		memmove ( F212_13C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321303" ), F212_13C );
		break;
	case ID212_17A:
		memmove ( F212_17A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), F212_17A );
		break;
	case ID212_17B:
		memmove ( F212_17B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), F212_17B );
		break;
	case ID212_17C:
		memmove ( F212_17C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), F212_17C );
		break;
	case ID212_18A:
		memmove ( F212_18A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), F212_18A );
		break;
	case ID212_18B:
		memmove ( F212_18B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData (_T( "321802" ), F212_18B );
		break;
	case ID212_18C:
		memmove ( F212_18C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), F212_18C );
		break;
	case ID212_19A:
		if ( m_pArith->l_cmp ( F212_19A, val ) ) {
			F212_19c |= 0x08;
			memmove ( F212_19A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321901" ), F212_19A );
		}
		break;
	case ID212_19B:
		if ( m_pArith->l_cmp ( F212_19B, val ) ) {
			F212_19c |= 0x80;
			memmove ( F212_19B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321902" ), F212_19B );
		}
		break;
	case ID212_19C:
		if ( m_pArith->l_cmp ( F212_19C, val ) ) {
			F212_19c |= 0x800;
			memmove ( F212_19C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321903" ), F212_19C );
		}
		break;
	case ID212_20A:
		if ( m_pArith->l_cmp ( F212_20A, val ) ) {
			F212_20c |= 0x08;
			memmove ( F212_20A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322001" ), F212_20A );
		}
		break;
	case ID212_20B:
		if ( m_pArith->l_cmp ( F212_20B, val ) ) {
			F212_20c |= 0x80;
			memmove ( F212_20B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322002" ), F212_20B );
		}
		break;
	case ID212_20C:
		if ( m_pArith->l_cmp ( F212_20C, val ) ) {
			F212_20c |= 0x800;
			memmove ( F212_20C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322003" ), F212_20C );
		}
		break;
	case ID212_21A:
		memmove ( F212_21A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322101" ), F212_21A );
		break;
	case ID212_21B:
		memmove ( F212_21B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322102" ), F212_21B );
		break;
	case ID212_21C:
		memmove ( F212_21C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322103" ), F212_21C );
		break;
	case ID212_22A:
		memmove ( F212_22A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322201" ), F212_22A );
		break;
	case ID212_22B:
		memmove ( F212_22B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322202" ), F212_22B );
		break;
	case ID212_22C:
		memmove ( F212_22C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322203" ), F212_22C );
		break;
	case ID212_26A:
		memmove ( F212_26A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326101" ), F212_26A );
		break;
	case ID212_26B:
		memmove ( F212_26B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326102" ), F212_26B );
		break;
	case ID212_26C:
		memmove ( F212_26C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "326103" ), F212_26C );
		break;
	case ID212_23A:
		if ( m_pArith->l_cmp ( F212_23A, val ) ) {
			F212_23c |= 0x08;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F212_24A, val, MONY_BUF_SIZE );
				memset ( F212_23A, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F212_23A, val, MONY_BUF_SIZE );
				memset ( F212_24A, '\0', MONY_BUF_SIZE);
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322301" ), F212_23A);
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322401" ), F212_24A);
		}
		break;
	case ID212_23B:
		if ( m_pArith->l_cmp ( F212_23B, val ) ) {
			F212_23c |= 0x80;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F212_24B, val, MONY_BUF_SIZE );
				memset ( F212_23B, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F212_23B, val, MONY_BUF_SIZE );
				memset ( F212_24B, '\0', MONY_BUF_SIZE );
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), F212_23B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), F212_24B );
		}
		break;
	case ID212_23C:
		if ( m_pArith->l_cmp ( F212_23C, val ) ) {
			F212_23c |= 0x800;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F212_24C, val, MONY_BUF_SIZE );
				memset ( F212_23C, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F212_23C, val, MONY_BUF_SIZE );
				memset ( F212_24C, '\0', MONY_BUF_SIZE );
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), F212_23C );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), F212_24C );
		}
		break;
	case ID212_24A:
		memmove ( F212_24A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322401" ), F212_24A );
		break;
	case ID212_24B:
		memmove ( F212_24B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), F212_24B );
		break;
	case ID212_24C:
		memmove ( F212_24C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), F212_24C );
		break;
	case ID212_25A:
		memmove ( F212_25A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322501" ), F212_25A );
		break;
	case ID212_25B:
		memmove ( F212_25B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322502" ), F212_25B );
		break;
	case ID212_25C:
		memmove ( F212_25C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322503" ), F212_25C );
		break;
	}

	return MemoryReget();
}

//=========================================================
// ダイアグラム初期属性セット
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo2_12::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo2_12[cnt].Index == 0 ) {
			break;
		}

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo2_12[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo2_12[cnt].UnConnect;

				int st = ChkTkkzIdx ( Fhyo2_12[cnt].Index );
				if ( st == 1 ) {
					if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
				}
			}
			else {
				Flg = Fhyo2_12[cnt].Connect;
			}
		}
		if ( ( ( *m_pSnHeadData)->Sn_EXP100 & 0x01 ) && *m_pShinInfo->pSgSpc && ( Fhyo2_12[cnt].Index == ID212_8EX ) ) {
			Flg = 0;
		}

		if ( m_SpcSgn ) {
			if ( ( Fhyo2_12[cnt].Index == ID212_23B ) || ( Fhyo2_12[cnt].Index == ID212_23C ) ) {
				Flg = 0;
			}
		}
		if ( ( Fhyo2_12[cnt].Index == ID212_26A ) || ( Fhyo2_12[cnt].Index == ID212_26B ) || ( Fhyo2_12[cnt].Index == ID212_26C ) ) {
			if ( eymd < ICS_SH_KYOJU_INPUT_DAY ) {
				Flg = 0;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Fhyo2_12[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo2_12[cnt].Connect );
		cnt++;

	}

	m_Fhyo2_12Diag01.EnableDelete ( 0 );

	return 0;
}

//============================================================
// 特定課税仕入に関連するインデックス
//------------------------------------------------------------
// 引数
//		idx		:	チェック対象
//------------------------------------------------------------
// 返送値
//		0		:	特定課税仕入に関連しない
//		1		:	特定課税仕入で入力可能になるインデックス
//============================================================
int CShinFhyo2_12::ChkTkkzIdx ( short idx )
{
	int rSt = 0;

	if ( ( idx >= ID212_11C ) && ( idx <= ID212_12X ) ) {
		rSt = 1;
	}

	return rSt;
}

//=========================================================
// 画面出力
//=========================================================
void CShinFhyo2_12::DispFh2_12()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 4; i <= 6; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ( short )i );
	}

	for( int i = ID212_1A; i <= ID212_25X; i++ ){
		dsp_cls ( IDC_ICSDIAG8CTRL1, ( short )i );
	}

	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		//手入力サイン初期化
		F212_10c = 0;
		F212_19c = 0;
		F212_20c = 0;
		F212_23c = 0;
	}

	// 期間の表示
	char	buf[512] = { 0 };
	char	yy, mm, dd;
	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, 4, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, 5, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 氏名の表示
	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( *m_pSnHeadData )->IsSoleProprietor() ) {
		memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, sizeof ( ( *m_pSnHeadData )->Sn_DIHYO ) );
	}
	else {
		memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, sizeof ( ( *m_pSnHeadData )->Sn_CONAM ) );
	}
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, 6, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 課税売上高の5億円超えチェック
	ChangeColor ( IDC_ICSDIAG8CTRL1, MILLION, 1 );
	ChangeColor ( IDC_ICSDIAG8CTRL1, MILLION + 1, 1 );
	diag_clear ( IDC_ICSDIAG8CTRL2, 4, TRUE, CIcsdiagctrl );
	CheckOver500Million();

	// 全項目表示
	F212data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo2_12::dsp_prs ( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
	diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
}

//=========================================================
// 表示金額クリア
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo2_12::dsp_cls ( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
	case 3 :	// 氏名又は､名称
	case 4 :	// 課税期間　自
	case 5 :	// 課税期間　至
		diag_clear ( Id, Index, TRUE, CIcsdiagctrl );
		break;
	default :	// 金額
		memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
		diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		break;
	}
}

//=========================================================
// 課税売上高5億円越えチェック
//=========================================================
void CShinFhyo2_12::CheckOver500Million()
{
	DIAGRAM_ATTRIBUTE DA;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	COLORREF color;

	char	VAL[MONY_BUF_SIZE] ={ 0 };
	char	ValCnv[MONY_BUF_SIZE] ={ 0 };
	( *m_pSnHeadData )->Sn_Sign4 &= 0xef;
	CheckOver500MillionSub ( *( m_pShinInfo->pOver500MillionSw ), VAL, ValCnv );

	char	buf[256] ={ 0 };
	if ( *m_pShinInfo->pOver500MillionSw == 1 ) {	// ON:９５％ルール改正対応期間で５億円超え
		( *m_pSnHeadData )->m_UriRatioBorder = 11000;
		( *m_pSnHeadData )->Sn_Sign4 |= 0x10;
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
			//ChangeColor ( IDC_ICSDIAG8CTRL1, MILLION, 5 );
			//ChangeColor ( IDC_ICSDIAG8CTRL1, MILLION + 1, 5 );
			color = BC_GREEN_L;
			memset ( buf, '\0', sizeof ( buf ) );

			if ( m_pArith->l_cmp ( VAL, ValCnv ) == 0 ) {
				strcpy_s ( buf, sizeof ( buf ), _T( "課税売上高が、５億円を超えています。\n" ) );
			}
			else {
				strcpy_s ( buf, sizeof ( buf ), _T( "課税売上高が、年換算で５億円を超えています。\n" ) );
			}
			strcat_s ( buf, sizeof ( buf ), _T( "比例配分方式の選択も含めて注意してください。" ) );
			DiagData.data_disp = ( char* )buf;
			diag_setdata ( IDC_ICSDIAG8CTRL2, 4, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}
	}
	else {			// ４億５千万以上で５億以下又は４億５千万未満
		( *m_pSnHeadData )->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
			//ChangeColor ( IDC_ICSDIAG8CTRL1, MILLION, 1 );
			//ChangeColor ( IDC_ICSDIAG8CTRL1, MILLION + 1, 1 );
			color = BC_WHITE;
			diag_clear ( IDC_ICSDIAG8CTRL2, 4, TRUE, CIcsdiagctrl );
		}
	}

	// m_Fhyo2_12Diag02.Refresh()を呼ぶと解像度が1024×768のときに正常に表示されなくなる
	if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
		diag_getattr ( IDC_ICSDIAG8CTRL1, MILLION, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL1, MILLION, &DA, TRUE, CIcsdiagctrl );

		diag_getattr ( IDC_ICSDIAG8CTRL1, MILLION + 1, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL1, MILLION + 1, &DA, TRUE, CIcsdiagctrl );
	}
}

//=========================================================
// ５億円チェック（金額チェック部分）
//---------------------------------------------------------
// 引数
//		MilSw		：	５億円チェックサインの保存先
//		pVAL		：	元金額
//		pValCnv		：	変換後金額
//---------------------------------------------------------
// 返送値		
//						FF3000　にセットした値
//=========================================================
int CShinFhyo2_12::CheckOver500MillionSub ( int &MilSw, char *pVAL, char *pValCnv )
{
	int		nflg = 0;
	char	flg[MONY_BUF_SIZE] ={ 0 };
	char	tmpVAL[MONY_BUF_SIZE]={ 0 }, 
		tmpValCnv[MONY_BUF_SIZE]={ 0 };

	// 入力ロック時は集計しない
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) {
		MoneyBasejagArray	money;
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "32" ) );
		int cnt = ( int )money.GetCount();
		int	incnt = 0;

		for ( int l = 0; l < cnt; l++ ) {
			incnt = ( int )money[l].GetCount();
			for ( int m = 0; m < incnt; m++ ) {
				// 付表２-１データ
				if ( money[l][m].code == _T( "322602" ) ) {
					memmove ( F211_1D, money[l][m].arith, MONY_BUF_SIZE);
				}
				else if ( money[l][m].code == _T( "322603" ) ) {
					memmove ( F211_1E, money[l][m].arith, MONY_BUF_SIZE);
				}
				else if ( money[l][m].code == _T("322700" ) ) {
					memmove ( F211_2F, money[l][m].arith, MONY_BUF_SIZE);
				}
				else if ( money[l][m].code == _T( "320101" ) ) {
					memmove ( F212_1A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "320102" ) ) {
					memmove ( F212_1B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "320103" ) ) {
					memmove ( F212_1C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "320800" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "SSSSSSSSSSSSS9" ) );
					F212_8EX = atoi ( wariai );
					F212_8X = atoi ( wariai );
				}
			}
		}
	}
	else {
		// 入力ロック時は付表2-1のデータクラスから金額を取得する
		CString tag, chtag;
		tag = _T( "CRB00030" );
		( ( CH31HyoView* )m_pParent )->GetMony ( tag, chtag, F211_1D );

		tag = _T( "CRB00040" );
		( ( CH31HyoView* )m_pParent )->GetMony ( tag, chtag, F211_1E );

		tag = _T( "CRB00060" );
		( ( CH31HyoView* )m_pParent )->GetMony ( tag, chtag, F211_2F );
	}

	m_pArith->l_add ( tmpVAL, F212_1A, F212_1B );
	m_pArith->l_add ( tmpVAL, tmpVAL, F212_1C );
	m_pArith->l_add ( tmpVAL, tmpVAL, F211_1D );
	m_pArith->l_add ( tmpVAL, tmpVAL, F211_1E );
	m_pArith->l_add ( tmpVAL, tmpVAL, F211_2F );
	int sw = ( *m_pSnHeadData )->CheckTaxationSales ( tmpVAL, tmpValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );

	if ( sw == 1 ) {
		m_pArith->l_input ( flg, _T( "0" ) );
		nflg = 0;
	}
	else {
		if ( F212_8EX >= 9500 ) {
			m_pArith->l_input ( flg, _T( "1" ) );
			nflg = 1;
		}
		else {
			m_pArith->l_input ( flg, _T( "0" ) );
			nflg = 0;
		}
	}

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "FF3000" ), flg );
	memmove ( pVAL, tmpVAL, MONY_BUF_SIZE );
	memmove ( pValCnv, tmpValCnv, MONY_BUF_SIZE );
	MilSw = sw;

	return nflg;
}

//=========================================================
// 項目表示
//---------------------------------------------------------
// 引数
//		atno		：		項目番号
//=========================================================
void CShinFhyo2_12::F212data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		F212data_1();

	case 4:
		F212data_4();

	case 7:
		F212data_7();

	case 8:
		F212data_8();
		ChangeF212data_8();

	case 9:
		F212data_9();

	case 10:
		F212data_10();

	case 11:
		F212data_11();

	case 12:
		F212data_12();

	case 13:
		F212data_13();

	case 14:
		F212data_14();

	case 15:
		F212data_15();

	case 16:
		F212data_16();

	case 17:
		F212data_17();

	case 18:
		F212data_18();

	case 19:
		F212data_19();

	case 20:
		F212data_20();

	case 21:
		F212data_21();

	case 22:
		F212data_22();

	case 23:
		F212data_26();

	case 24:
		F212data_23();

	case 25:
		F212data_24();

	case 26:
		F212data_25();

		break;
	default:
		break;
	}
}

//=========================================================
// 課税売上(税抜き)表示
//=========================================================
void CShinFhyo2_12::F212data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_1A, F212_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_1B, F212_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_1C, F212_1C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_1X, F212_1X );
}

//=========================================================
// 課税資産の譲渡等の対価の額
//=========================================================
void CShinFhyo2_12::F212data_4()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_4X, F212_4X );
}

//=========================================================
// 資産の譲渡等の対価の額
//=========================================================
void CShinFhyo2_12::F212data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_7X, F212_7X );
}

//=========================================================
// 課税売上割合
//=========================================================
void CShinFhyo2_12::F212data_8()
{
	char WORK0[MONY_BUF_SIZE] = { 0 };
	char buf[512] = { 0 };

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 課税売上高割合
	DiagData.data_check = 0;
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID212_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_8X, 1 );
	diag_modify ( IDC_ICSDIAG8CTRL1, ID212_8X, DIAG_MDFY_READONLY, CIcsdiagctrl );
	memset ( buf, '\0', sizeof ( buf ) );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	if ( !( ( *m_pSnHeadData )->Sn_EXP100 & 0x01 ) ) {
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			// 課税売上割合　手入力
			if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() ) {
				memset ( buf, '\0', sizeof ( buf ) );
				strcpy_s (  buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)\n【課税売上割合に準ずる割合を適用】" ) );
				DiagData.data_disp = buf;
				diag_setdata ( IDC_ICSDIAG8CTRL1, 24, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID212_8X, 1 );
			}
			else{
				memset ( buf, '\0', sizeof ( buf ) );
				strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)" ) );
				DiagData.data_disp = buf;
				diag_setdata ( IDC_ICSDIAG8CTRL1, 24, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_8X, 1 );
			}

			if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_8X, 1 );
			}
			else {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_8X, 1 );
			}
		}
		else {
			strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)" ) );
			DiagData.data_disp = buf;
			diag_setdata ( IDC_ICSDIAG8CTRL1, 24, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_8X, 1 );
		}
	}
	wsprintf ( buf, _T( "%d.%02d" ), ( F212_8EX / 100 ), ( F212_8EX % 100 ) );
	DiagData.data_edit = buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID212_8X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 課税売上割合を無効にするとリセットされてしまうため、別関数にする
/*	// 課税売上割合に準ずる割合を有効
	// 課税売上割合が９５％未満
	if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() || ( F212_8EX < (*m_pSnHeadData)->m_UriRatioBorder ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 )== 0x00 ) ) {
				ModifyDiagInput ( ID212_17A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_17B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_17C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17C, 0 );

				ModifyDiagInput ( ID212_18A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_18B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_18C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18C, 0 );

				ModifyDiagInput ( ID212_19A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_19B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_19C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, ( F212_19c & 0x08 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, ( F212_19c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, ( F212_19c & 0x800 ) ? 3 : 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, 1 );
				ModifyDiagInput ( ID212_20A, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID212_20B, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID212_20C, DIAG_MDFY_READONLY );
			}
		}
		else {
			ModifyDiagInput ( ID212_17A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_17B, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_17C, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17B, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17C, 1 );

			ModifyDiagInput ( ID212_18A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_18B, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_18C, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18B, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18C, 1 );

			F212_19c = 0;
			ModifyDiagInput ( ID212_19A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_19B, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_19C, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, 1 );

			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
				ModifyDiagInput ( ID212_20A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_20B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_20C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, ( F212_20c & 0x08 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, ( F212_20c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, ( F212_20c & 0x800 ) ? 3 : 0 );
			}
		}
	}
	else {
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17C, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18C, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, 1 );

		ModifyDiagInput ( ID212_17A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_17B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_17C, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID212_18A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_18B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_18C, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID212_19A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_19B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_19C, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID212_20A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_20B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_20C, DIAG_MDFY_READONLY );
	}
*/
}

//=========================================================
// 課税仕入れに係る支払対価の額(税込み)
//=========================================================
void CShinFhyo2_12::F212data_9()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_9A, F212_9A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_9B, F212_9B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_9C, F212_9C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_9X, F212_9X );
}

//=========================================================
// 課税仕入れに係る消費税額
//=========================================================
void CShinFhyo2_12::F212data_10()
{
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_10A, ( F212_10c & 0x08 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_10B, ( F212_10c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_10C, ( F212_10c & 0x800 ) ? 3 : 0 );
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_10A, F212_10A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_10B, F212_10B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_10C, F212_10C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_10X, F212_10X );
}

//=========================================================
// 特定課税仕入れに係る支払対価の額
//=========================================================
void CShinFhyo2_12::F212data_11()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_11C, F212_11C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_11X, F212_11X );
}

//=========================================================
// 特定課税仕入れに係る消費税額
//=========================================================
void CShinFhyo2_12::F212data_12()
{
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_12C, ( F212_12c & 0x800 ) ? 3 : 0 );
			}
			else {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_12C, 1 );
			}
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_12C, F212_12C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_12X, F212_12X );
}

//=========================================================
// 課税貨物に係る消費税額
//=========================================================
void CShinFhyo2_12::F212data_13()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_13C, F212_13C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_13X, F212_13X );
}

//=========================================================
// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_12::F212data_14()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_14A, F212_14A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_14B, F212_14B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_14C, F212_14C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_14X, F212_14X );
}

//=========================================================
// 課税仕入れ等の税額の合計額
//=========================================================
void CShinFhyo2_12::F212data_15()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_15A, F212_15A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_15B, F212_15B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_15C, F212_15C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_15X, F212_15X );
}

//=========================================================
// 課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合
//=========================================================
void CShinFhyo2_12::F212data_16()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_16A, F212_16A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_16B, F212_16B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_16C, F212_16C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_16X, F212_16X );
}

//=========================================================
// 15のうち、課税売上げにのみ要するもの
//=========================================================
void CShinFhyo2_12::F212data_17()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_17A, F212_17A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_17B, F212_17B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_17C, F212_17C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_17X, F212_17X );
}

//=========================================================
// 15のうち、課税売上げと非課税売上げに共通して要するもの
//=========================================================
void CShinFhyo2_12::F212data_18()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_18A, F212_18A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_18B, F212_18B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_18C, F212_18C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_18X, F212_18X );
}

//=========================================================
// 個別対応方式により控除する課税仕入れ等の税額
//=========================================================
void CShinFhyo2_12::F212data_19()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_19A, F212_19A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_19B, F212_19B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_19C, F212_19C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_19X, F212_19X );
}

//=========================================================
// 一括比例配分方式により控除する課税仕入れ等の税額
//=========================================================
void CShinFhyo2_12::F212data_20()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_20A, F212_20A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_20B, F212_20B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_20C, F212_20C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_20X, F212_20X );
}

//=========================================================
// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_12::F212data_21()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_21A, F212_21A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_21B, F212_21B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_21C, F212_21C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_21X, F212_21X );
}

//=========================================================
// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_12::F212data_22()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_22A, F212_22A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_22B, F212_22B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_22C, F212_22C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_22X, F212_22X );
}

//=========================================================
// 控除対象仕入税額がプラスの時
//=========================================================
void CShinFhyo2_12::F212data_23()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_23A, ( F212_23c & 0x08 ) ? 3 : 0 );
		if ( m_SpcSgn == FALSE ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_23B, ( F212_23c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_23C, ( F212_23c & 0x800 ) ? 3 : 0 );
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_23A, F212_23A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_23B, F212_23B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_23C, F212_23C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_23X, F212_23X );
}

//=========================================================
// 控除対象仕入税額がマイナスの時
//=========================================================
void CShinFhyo2_12::F212data_24()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_24A, F212_24A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_24B, F212_24B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_24C, F212_24C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_24X, F212_24X );
}

//=========================================================
// 貸倒回収に係る消費税額
//=========================================================
void CShinFhyo2_12::F212data_25()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_25A, F212_25A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_25B, F212_25B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_25C, F212_25C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_25X, F212_25X );
}

//=========================================================
// 居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
//=========================================================
void CShinFhyo2_12::F212data_26()
{
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_26A, F212_26A );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_26B, F212_26B );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_26C, F212_26C );
	dsp_prs ( IDC_ICSDIAG8CTRL1, ID212_26X, F212_26X );
}

//=========================================================
// 課税売上割合を変更
//=========================================================
void CShinFhyo2_12::ChangeF212data_8()
{
	// 課税売上割合に準ずる割合を有効
	// 課税売上割合が９５％未満
	if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() || ( F212_8EX < (*m_pSnHeadData)->m_UriRatioBorder ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 )== 0x00 ) ) {
				ModifyDiagInput ( ID212_17A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_17B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_17C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17C, 0 );

				ModifyDiagInput ( ID212_18A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_18B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_18C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18C, 0 );

				ModifyDiagInput ( ID212_19A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_19B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_19C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, ( F212_19c & 0x08 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, ( F212_19c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, ( F212_19c & 0x800 ) ? 3 : 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, 1 );
				ModifyDiagInput ( ID212_20A, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID212_20B, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID212_20C, DIAG_MDFY_READONLY );

				F212_20c = 0;
			}
		}
		else {
			ModifyDiagInput ( ID212_17A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_17B, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_17C, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17B, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17C, 1 );

			ModifyDiagInput ( ID212_18A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_18B, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_18C, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18B, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18C, 1 );

			F212_19c = 0;
			ModifyDiagInput ( ID212_19A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_19B, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID212_19C, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, 1 );

			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
				ModifyDiagInput ( ID212_20A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_20B, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID212_20C, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, ( F212_20c & 0x08 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, ( F212_20c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, ( F212_20c & 0x800 ) ? 3 : 0 );
			}
		}
	}
	else {
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_17C, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_18C, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_19C, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20B, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID212_20C, 1 );

		ModifyDiagInput ( ID212_17A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_17B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_17C, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID212_18A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_18B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_18C, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID212_19A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_19B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_19C, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID212_20A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_20B, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID212_20C, DIAG_MDFY_READONLY );

		F212_20c = 0;
	}
}

//=========================================================
// 棚卸データの加減算を行う
//=========================================================
void CShinFhyo2_12::TanaDataAdd()
{
	char	VAL_14A[6] = { 0 }, VAL_14B[6] = { 0 }, VAL_14C[6] = { 0 }, VAL_14D[6] = { 0 }, VAL_14E[6] = { 0 };	// 全体
	char	VAL_17A[6] = { 0 }, VAL_17B[6] = { 0 }, VAL_17C[6] = { 0 }, VAL_17D[6] = { 0 }, VAL_17E[6] = { 0 };	// 課税売上に要するもの
	char	VAL_18A[6] = { 0 }, VAL_18B[6] = { 0 }, VAL_18C[6] = { 0 }, VAL_18D[6] = { 0 }, VAL_18E[6] = { 0 };	// 課税売上と非課税売上に共通して要するもの

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動は保存データで使用するので、再計算不要

		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) {
			if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
				memset ( F212_14C, '\0', MONY_BUF_SIZE );
				m_pArith->l_add ( F212_14C, F212_14C, ( *m_pSnTanaData )->Sn_mnz63[0] );
				m_pArith->l_add ( F212_14C, F212_14C, ( *m_pSnTanaData )->Sn_mnz63[1] );
				m_pArith->l_add ( F212_14C, F212_14C, ( *m_pSnTanaData )->Sn_mnz63[2] );

				memset ( F212_14B, '\0', MONY_BUF_SIZE );
				m_pArith->l_add ( F212_14B, F212_14B, ( *m_pSnTanaData) ->Sn_mnz40[0] );
				m_pArith->l_add ( F212_14B, F212_14B, ( *m_pSnTanaData )->Sn_mnz40[1] );
				m_pArith->l_add ( F212_14B, F212_14B, ( *m_pSnTanaData )->Sn_mnz40[2] );

				memset ( F212_14A, '\0', MONY_BUF_SIZE );
				m_pArith->l_add ( F212_14A, F212_14A, ( *m_pSnTanaData )->Sn_menzz[0] );
				m_pArith->l_add ( F212_14A, F212_14A, ( *m_pSnTanaData )->Sn_menzz[1] );
				m_pArith->l_add ( F212_14A, F212_14A, ( *m_pSnTanaData )->Sn_menzz[2] );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( F212_14A );
					m_pArith->l_neg ( F212_14B );
					m_pArith->l_neg ( F212_14C );
				}

				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F212_14A );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F212_14B );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F212_14C );
			}
			else {
				memset ( F212_14A, '\0', MONY_BUF_SIZE );
				memset ( F212_14B, '\0', MONY_BUF_SIZE );
				memset ( F212_14C, '\0', MONY_BUF_SIZE );

				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
			}
		}
		else {
			if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
				memmove ( F212_14A, ( *m_pSnTanaData )->Sn_MENZZ, MONY_BUF_SIZE );
				memmove ( F212_14B, ( *m_pSnTanaData )->Sn_MNZ40, MONY_BUF_SIZE );
				memmove ( F212_14C, ( *m_pSnTanaData )->Sn_MNZ63, MONY_BUF_SIZE );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( F212_14A );
					m_pArith->l_neg ( F212_14B );
					m_pArith->l_neg ( F212_14C );
				}

				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F212_14A );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F212_14B );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F212_14C );
			}
			else {
				memset ( F212_14A, '\0', MONY_BUF_SIZE );
				memset ( F212_14B, '\0', MONY_BUF_SIZE );
				memset ( F212_14C, '\0', MONY_BUF_SIZE );

				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
			}
		}
		return;
	}

	if( !((*m_pSnTanaData)->Sn_tansw&0x01) && ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) ){

		MoneyBasejagArray	money;
		char wariai[32] = { 0 };

		// 連動データを取得
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "320800" ) );
		m_pArith->l_print ( wariai, money[0][0].arith, _T( "SSSSSSSSSSSSS9" ) );
		F212_8EX = atoi ( wariai );
		CheckOver500Million();

		if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
			m_pArith->l_add ( VAL_14C, ( *m_pSnTanaData )->Sn_mnz63[0], ( *m_pSnTanaData )->Sn_mnz63[1] );
			m_pArith->l_add ( VAL_14C, VAL_14C, ( *m_pSnTanaData )->Sn_mnz63[2] );
			m_pArith->l_add ( VAL_14B, ( *m_pSnTanaData )->Sn_mnz40[0], ( *m_pSnTanaData )->Sn_mnz40[1] );
			m_pArith->l_add ( VAL_14B, VAL_14B, ( *m_pSnTanaData )->Sn_mnz40[2] );
			m_pArith->l_add ( VAL_14A, ( *m_pSnTanaData )->Sn_menzz[0], ( *m_pSnTanaData )->Sn_menzz[1] );
			m_pArith->l_add ( VAL_14A, VAL_14A, ( *m_pSnTanaData )->Sn_menzz[2] );

			m_pArith->l_add ( VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[0], ( *m_pSnTanaData )->Sn_mnz78[1] );
			m_pArith->l_add ( VAL_14E, VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[2] );
			m_pArith->l_add ( VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[0], ( *m_pSnTanaData )->Sn_mnz624[1] );
			m_pArith->l_add ( VAL_14D, VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[2] );

			// 比例配分用格納場所へ格納
			memmove ( ( *m_pSnTanaData )->Sn_MNZ63, VAL_14C, MONY_BUF_SIZE );
			memmove ( ( *m_pSnTanaData )->Sn_MNZ40, VAL_14B, MONY_BUF_SIZE );
			memmove ( ( *m_pSnTanaData )->Sn_MENZZ, VAL_14A, MONY_BUF_SIZE );

			memmove ( ( *m_pSnTanaData )->Sn_MNZ78, VAL_14E, MONY_BUF_SIZE );
			memmove ( ( *m_pSnTanaData )->Sn_MNZ624, VAL_14D, MONY_BUF_SIZE );

			if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
				m_pArith->l_neg ( VAL_14A );
				m_pArith->l_neg ( VAL_14B );
				m_pArith->l_neg ( VAL_14C );

				m_pArith->l_neg ( VAL_14D );
				m_pArith->l_neg ( VAL_14E );
			}

			// 個別対応で95％未満の場合
			if ( ( F212_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
				memmove ( VAL_17C, ( *m_pSnTanaData )->Sn_mnz63[0], MONY_BUF_SIZE );
				memmove ( VAL_18C, ( *m_pSnTanaData )->Sn_mnz63[2], MONY_BUF_SIZE );
				memmove ( VAL_17B, ( *m_pSnTanaData )->Sn_mnz40[0], MONY_BUF_SIZE );
				memmove ( VAL_18B, ( *m_pSnTanaData )->Sn_mnz40[2], MONY_BUF_SIZE );
				memmove ( VAL_17A, ( *m_pSnTanaData )->Sn_menzz[0], MONY_BUF_SIZE );
				memmove ( VAL_18A, ( *m_pSnTanaData )->Sn_menzz[2], MONY_BUF_SIZE );

				memmove ( VAL_17E, ( *m_pSnTanaData )->Sn_mnz78[0], MONY_BUF_SIZE );
				memmove ( VAL_18E, ( *m_pSnTanaData )->Sn_mnz78[2], MONY_BUF_SIZE );
				memmove ( VAL_17D, ( *m_pSnTanaData )->Sn_mnz624[0], MONY_BUF_SIZE );
				memmove ( VAL_18D, ( *m_pSnTanaData )->Sn_mnz624[2], MONY_BUF_SIZE );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( VAL_17C );
					m_pArith->l_neg ( VAL_18C );
					m_pArith->l_neg ( VAL_17B );
					m_pArith->l_neg ( VAL_18B );
					m_pArith->l_neg ( VAL_17A );
					m_pArith->l_neg ( VAL_18A );

					m_pArith->l_neg ( VAL_17E );
					m_pArith->l_neg ( VAL_18E );
					m_pArith->l_neg ( VAL_17D );
					m_pArith->l_neg ( VAL_18D );
				}
			}
		}
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), VAL_14A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), VAL_14B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), VAL_14C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), VAL_14D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), VAL_14E );
	}
	else if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01 ) && ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
		if( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
			memmove ( VAL_14A, ( *m_pSnTanaData )->Sn_MENZZ, MONY_BUF_SIZE );
			memmove ( VAL_14B, ( *m_pSnTanaData )->Sn_MNZ40, MONY_BUF_SIZE );
			memmove ( VAL_14C, ( *m_pSnTanaData )->Sn_MNZ63, MONY_BUF_SIZE );

			memmove ( VAL_14D, ( *m_pSnTanaData )->Sn_MNZ624, MONY_BUF_SIZE );
			memmove ( VAL_14E, ( *m_pSnTanaData )->Sn_MNZ78, MONY_BUF_SIZE );

			if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
				m_pArith->l_neg ( VAL_14A );
				m_pArith->l_neg ( VAL_14B );
				m_pArith->l_neg ( VAL_14C );

				m_pArith->l_neg ( VAL_14D );
				m_pArith->l_neg ( VAL_14E );
			}
		}
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), VAL_14A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), VAL_14B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), VAL_14C );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), VAL_14D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), VAL_14E );
	}

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A201" ), VAL_17A );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A202" ), VAL_17B );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A203" ), VAL_17C );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A301" ), VAL_18A );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A302" ), VAL_18B );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A303" ), VAL_18C );

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A602" ), VAL_17D );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A603" ), VAL_17E );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A702" ), VAL_18D );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A703" ), VAL_18E );
}

//=========================================================
// 表示金額取得
//=========================================================
void CShinFhyo2_12::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1.課税売上額(税抜き)
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_1C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_1X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_1X, DiagData.data_val, MONY_BUF_SIZE );

	// 7.資産譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_7X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_7X, DiagData.data_val, MONY_BUF_SIZE );

	// 9.課税仕入れに係る支払い対価の額(税込み)
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_9A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_9A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_9B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_9B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_9C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_9C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_9X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_9X, DiagData.data_val, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_10A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_10A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_10B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_10B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_10C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_10C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_10X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_10X, DiagData.data_val, MONY_BUF_SIZE );

	// 11.特定課税仕入れに係る支払対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_11C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_11X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_11X, DiagData.data_val, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_12C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_12X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_12X, DiagData.data_val, MONY_BUF_SIZE );

	// 13.課税貨物に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_13C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_13X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_13X, DiagData.data_val, MONY_BUF_SIZE );

	// 14.納税義務の免除を受ける(受けない)こととなった場合における消費税額の調整額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_14A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_14A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_14B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_14B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_14C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_14C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_14X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_14X, DiagData.data_val, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_15A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_15A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_15B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_15B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_15C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_15C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_15X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_15X, DiagData.data_val, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、割合が95％以上の場合
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_16A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_16A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_16B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_16B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_16C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_16C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_16X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_16X, DiagData.data_val, MONY_BUF_SIZE );

	// 17.15のうち、課税売上げにのみ要するもの
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_17A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_17A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_17B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_17B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_17C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_17C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_17X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_17X, DiagData.data_val, MONY_BUF_SIZE );

	// 18.15のうち、共通して要するもの
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_18A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_18A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_18B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_18B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_18C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_18C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_18X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_18X, DiagData.data_val, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_19A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_19A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_19B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_19B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_19C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_19C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_19X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_19X, DiagData.data_val, MONY_BUF_SIZE );

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_20A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_20A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_20B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_20B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_20C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_20C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_20X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_20X, DiagData.data_val, MONY_BUF_SIZE );

	// 21.課税売上割合変動時の…消費税額の調整額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_21A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_21A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_21B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_21B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_21C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_21C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_21X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_21X, DiagData.data_val, MONY_BUF_SIZE );

	// 22.調整対象固定資産を…調整額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_22A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_22A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_22B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_22B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_22C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_22C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_22X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_22X, DiagData.data_val, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_26A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_26A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_26B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_26B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_26C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_26C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_26X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_26X, DiagData.data_val, MONY_BUF_SIZE );

	// 24.控除対象仕入税額がプラスの時
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_23A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_23A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_23B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove( F212_23B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_23C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_23C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_23X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_23X, DiagData.data_val, MONY_BUF_SIZE );

	// 25.控除過大調整額がマイナスの時
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_24A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_24A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_24B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_24B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_24C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_24C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_24X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_24X, DiagData.data_val, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_25A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_25A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_25B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_25B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_25C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_25C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID212_25X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F212_25X, DiagData.data_val, MONY_BUF_SIZE );
}

//=========================================================
// ボタン表示変更
//=========================================================
void CShinFhyo2_12::ChangeBtnDisp()
{

	if ( ( ( *m_pSnHeadData )->m_apno & 0xf0) >= 0x50 ) {
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			m_tanaoroshi.EnableWindow ( FALSE );

			if ( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) {
				// 入力ロック時には簡易計算式のボタンを非表示
				m_Button2.EnableWindow ( FALSE );
				m_Button2.ShowWindow ( SW_HIDE );
			}
		}
		else {
			m_tanaoroshi.EnableWindow ( TRUE );
			if ( ( *m_pShinInfo->pSgSpc ) && !( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) ) {
				m_Button2.ShowWindow ( SW_SHOW );
			}
			else {
				m_Button2.ShowWindow ( SW_HIDE );
			}
		}
	}
	else {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) ) {
			m_tanaoroshi.EnableWindow ( FALSE );
		}
		else {
			m_tanaoroshi.EnableWindow ( TRUE );
		}
		m_Button2.EnableWindow ( FALSE );
		m_Button2.ShowWindow ( SW_HIDE );
	}
}

//===================================================
// 初期ポジションセット
//===================================================
void CShinFhyo2_12::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( (index = m_Fhyo2_12Diag01.GetPosition()) != -1 ) {
			m_Fhyo2_12Diag01.SetPosition ( index );
			m_Fhyo2_12Diag01.SetFocus();
			return;
		}
	}

	m_Fhyo2_12Diag02.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Fhyo2_12Diag01.SetFocus();
	m_Fhyo2_12Diag01.SetPosition ( ID212_1A );
}

//=========================================================
// フォーカス移動先取得
//---------------------------------------------------------
// 引数
//			Index		：	ダイアグラム内インデックス
//			Move		：	0 = 下方向, 1 = 上方向
//=========================================================
int CShinFhyo2_12::UpDownFocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY2_12CNT; i++ ) {
		if ( Index == FCS_Fhyo2_12[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo2_12[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY2_12CNT; j++ ) {
			m_Fhyo2_12Diag01.GetAttribute ( FCS_Fhyo2_12[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_12[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo2_12Diag01.GetAttribute ( FCS_Fhyo2_12[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_12[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//=========================================================
// 入力許可・不許可 切替
//---------------------------------------------------------
// 引数
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinFhyo2_12::ModifyDiagInput ( short index, int sign )
{
	diag_modify( IDC_ICSDIAG8CTRL1, index, sign, CIcsdiagctrl );
}


//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinFhyo2_12::ChangeColor ( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr ( id, Index, &DA, CIcsdiagctrl );
	switch ( sign ) {
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
	diag_setattr ( id, Index, &DA, FALSE, CIcsdiagctrl );

}

//=========================================================
// 棚卸調整額入力
//=========================================================
void CShinFhyo2_12::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) {

		// 前回の情報をセーブ
		char			SV_mtype = ( *m_pSnTanaData )->Sn_MENZsw;
		unsigned short	SV_F212_8EX = F212_8EX;
		char			SV_mnk40[3][MONY_BUF_SIZE];
		char			SV_mnz40[3][MONY_BUF_SIZE];
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];
		char			SV_menzk[3][MONY_BUF_SIZE];
		char			SV_menzz[3][MONY_BUF_SIZE];
		char			zerobuf[MONY_BUF_SIZE];

		char			SV_mnk624[3][MONY_BUF_SIZE];
		char			SV_mnz624[3][MONY_BUF_SIZE];
		char			SV_mnk78[3][MONY_BUF_SIZE];
		char			SV_mnz78[3][MONY_BUF_SIZE];
		char			VAL_14D[MONY_BUF_SIZE];
		char			VAL_14E[MONY_BUF_SIZE];
		char			VAL_17D[MONY_BUF_SIZE];
		char			VAL_17E[MONY_BUF_SIZE];
		char			VAL_18D[MONY_BUF_SIZE];
		char			VAL_18E[MONY_BUF_SIZE];

		memmove ( SV_mnk63[0], ( *m_pSnTanaData )->Sn_mnk63[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
		memmove ( SV_mnz63[0], ( *m_pSnTanaData )->Sn_mnz63[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
		memmove ( SV_mnk63[1], ( *m_pSnTanaData )->Sn_mnk63[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
		memmove ( SV_mnz63[1], ( *m_pSnTanaData )->Sn_mnz63[1], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
		memmove ( SV_mnk63[2], ( *m_pSnTanaData )->Sn_mnk63[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
		memmove ( SV_mnz63[2], ( *m_pSnTanaData )->Sn_mnz63[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃

		if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
			memmove ( SV_mnk40[0], ( *m_pSnTanaData )->Sn_mnk40[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
			memmove ( SV_mnz40[0], ( *m_pSnTanaData )->Sn_mnz40[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove ( SV_mnk40[1], ( *m_pSnTanaData )->Sn_mnk40[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
			memmove ( SV_mnz40[1], ( *m_pSnTanaData )->Sn_mnz40[1], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove ( SV_mnk40[2], ( *m_pSnTanaData )->Sn_mnk40[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
			memmove ( SV_mnz40[2], ( *m_pSnTanaData )->Sn_mnz40[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃

			memmove ( SV_menzk[0], ( *m_pSnTanaData )->Sn_menzk[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
			memmove ( SV_menzz[0], ( *m_pSnTanaData )->Sn_menzz[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove ( SV_menzk[1], ( *m_pSnTanaData )->Sn_menzk[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
			memmove ( SV_menzz[1], ( *m_pSnTanaData )->Sn_menzz[1], MONY_BUF_SIZE );	//消費税額   (〃      )　〃
			memmove ( SV_menzk[2], ( *m_pSnTanaData )->Sn_menzk[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
			memmove ( SV_menzz[2], ( *m_pSnTanaData )->Sn_menzz[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
		}

		memmove ( SV_mnk78[0], ( *m_pSnTanaData )->Sn_mnk78[0], MONY_BUF_SIZE );
		memmove ( SV_mnz78[0], ( *m_pSnTanaData )->Sn_mnz78[0], MONY_BUF_SIZE );
		memmove ( SV_mnk78[1], ( *m_pSnTanaData )->Sn_mnk78[1], MONY_BUF_SIZE );
		memmove ( SV_mnz78[1], ( *m_pSnTanaData )->Sn_mnz78[1], MONY_BUF_SIZE );
		memmove ( SV_mnk78[2], ( *m_pSnTanaData )->Sn_mnk78[2], MONY_BUF_SIZE );
		memmove ( SV_mnz78[2], ( *m_pSnTanaData )->Sn_mnz78[2], MONY_BUF_SIZE );

		memmove ( SV_mnk624[0], ( *m_pSnTanaData )->Sn_mnk624[0], MONY_BUF_SIZE );
		memmove ( SV_mnz624[0], ( *m_pSnTanaData )->Sn_mnz624[0], MONY_BUF_SIZE );
		memmove ( SV_mnk624[1], ( *m_pSnTanaData )->Sn_mnk624[1], MONY_BUF_SIZE );
		memmove ( SV_mnz624[1], ( *m_pSnTanaData )->Sn_mnz624[1], MONY_BUF_SIZE );
		memmove ( SV_mnk624[2], ( *m_pSnTanaData )->Sn_mnk624[2], MONY_BUF_SIZE );
		memmove ( SV_mnz624[2], ( *m_pSnTanaData )->Sn_mnz624[2], MONY_BUF_SIZE );

		// 個別対応
		CChangeTaxEx	CTDiagEx;
		CTDiagEx.InitInfo ( ( *m_pSnFhyo2_02Data ), ( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );


		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle ( g_hInstance );
		int st = ( int )CTDiagEx.DoModal();
		AfxSetResourceHandle ( svhInstResource );

		if ( st == IDOK ) {

			// 手入力
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動
				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					MoneyBasejagArray	money;

					// 付表２－２のデータを取得
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "324202" ) );
					memmove ( VAL_17D, money[0][0].arith, MONY_BUF_SIZE );
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "324203" ) );
					memmove ( VAL_17E, money[0][0].arith, MONY_BUF_SIZE );
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "324302" ) );
					memmove ( VAL_18D, money[0][0].arith, MONY_BUF_SIZE );
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "324303" ) );
					memmove ( VAL_18E, money[0][0].arith, MONY_BUF_SIZE );
				}

				// 前回が個別対応で95％未満の場合、前回分を削除
				if ( ( SV_mtype & 0x10 ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
					if ( SV_mtype & 0x01 ) {
						m_pArith->l_add ( F212_17C, F212_17C, SV_mnz63[0] );			// 課税売上に係る仕入税額
						m_pArith->l_add ( F212_18C, F212_18C, SV_mnz63[2] );			// 共通の売上に係る仕入税額

						if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
							m_pArith->l_add ( F212_17B, F212_17B, SV_mnz40[0] );		// 課税売上に係る仕入税額
							m_pArith->l_add ( F212_18B, F212_18B, SV_mnz40[2] );		// 共通の売上に係る仕入税額
							m_pArith->l_add ( F212_17A, F212_17A, SV_menzz[0] );		// 課税売上に係る仕入税額
							m_pArith->l_add ( F212_18A, F212_18A, SV_menzz[2] );		// 共通の売上に係る仕入税額
						}

						m_pArith->l_add ( VAL_17E, VAL_17E, SV_mnz78[0] );				// 課税売上に係る仕入税額
						m_pArith->l_add ( VAL_18E, VAL_18E, SV_mnz78[2] );				// 共通の売上に係る仕入税額
						m_pArith->l_add ( VAL_17D, VAL_17D, SV_mnz624[0] );				// 課税売上に係る仕入税額
						m_pArith->l_add ( VAL_18D, VAL_18D, SV_mnz624[2] );				// 共通の売上に係る仕入税額
					}
					else {
						m_pArith->l_sub ( F212_17C, F212_17C, SV_mnz63[0] );			// 15. 課税売上に係る仕入税額
						m_pArith->l_sub ( F212_18C, F212_18C, SV_mnz63[2] );			// 16. 共通の売上に係る仕入税額
						if  ( (*m_pSnHeadData )->SVmzsw == 1 ) {
							m_pArith->l_sub ( F212_17B, F212_17B, SV_mnz40[0] );		// 15. 課税売上に係る仕入税額
							m_pArith->l_sub ( F212_18B, F212_18B, SV_mnz40[2] );		// 16. 共通の売上に係る仕入税額
							m_pArith->l_sub ( F212_17A, F212_17A, SV_menzz[0] );		// 15. 課税売上に係る仕入税額
							m_pArith->l_sub ( F212_18A, F212_18A, SV_menzz[2] );		// 16. 共通の売上に係る仕入税額
						}

						m_pArith->l_sub ( VAL_17E, VAL_17E, SV_mnz78[0] );				// 課税売上に係る仕入税額
						m_pArith->l_sub ( VAL_18E, VAL_18E, SV_mnz78[2] );				// 共通の売上に係る仕入税額
						m_pArith->l_sub ( VAL_17D, VAL_17D, SV_mnz624[0] );				// 課税売上に係る仕入税額
						m_pArith->l_sub ( VAL_18D, VAL_18D, SV_mnz624[2] );				// 共通の売上に係る仕入税額
					}
				}

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
					memset ( F212_14C, '\0', MONY_BUF_SIZE );
					m_pArith->l_add ( F212_14C, F212_14C, ( *m_pSnTanaData )->Sn_mnz63[0] );
					m_pArith->l_add ( F212_14C, F212_14C, ( *m_pSnTanaData )->Sn_mnz63[1] );
					m_pArith->l_add ( F212_14C, F212_14C, ( *m_pSnTanaData )->Sn_mnz63[2] );
					if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
						memset ( F212_14B, '\0', MONY_BUF_SIZE );
						m_pArith->l_add ( F212_14B, F212_14B, ( *m_pSnTanaData )->Sn_mnz40[0] );
						m_pArith->l_add ( F212_14B, F212_14B, ( *m_pSnTanaData )->Sn_mnz40[1] );
						m_pArith->l_add ( F212_14B, F212_14B, ( *m_pSnTanaData )->Sn_mnz40[2] );

						memset ( F212_14A, '\0', MONY_BUF_SIZE );
						m_pArith->l_add ( F212_14A, F212_14A, ( *m_pSnTanaData )->Sn_menzz[0] );	//				 〃	   消費税額   (〃      )　〃
						m_pArith->l_add ( F212_14A, F212_14A, ( *m_pSnTanaData )->Sn_menzz[1] );	//				 〃	   消費税額   (〃      )　〃
						m_pArith->l_add ( F212_14A, F212_14A, ( *m_pSnTanaData )->Sn_menzz[2] );	//				 〃	   消費税額   (〃      )　〃
					}

					memset ( VAL_14E, '\0', MONY_BUF_SIZE );
					memset ( VAL_14D, '\0', MONY_BUF_SIZE );
					m_pArith->l_add ( VAL_14E, VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[0] );
					m_pArith->l_add ( VAL_14E, VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[1] );
					m_pArith->l_add ( VAL_14E, VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[2] );
					m_pArith->l_add ( VAL_14D, VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[0] );
					m_pArith->l_add ( VAL_14D, VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[1] );
					m_pArith->l_add ( VAL_14D, VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[2] );

					// 旧格納場所へ格納
					memmove ( ( *m_pSnTanaData )->Sn_MNZ63, F212_14C, MONY_BUF_SIZE );
					if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
						memmove ( ( *m_pSnTanaData )->Sn_MNZ40, F212_14B, MONY_BUF_SIZE );
						memmove ( ( *m_pSnTanaData )->Sn_MENZZ, F212_14A, MONY_BUF_SIZE );
					}
					memmove ( ( *m_pSnTanaData )->Sn_MNZ78, VAL_14E, MONY_BUF_SIZE );
					memmove ( ( *m_pSnTanaData )->Sn_MNZ624, VAL_14D, MONY_BUF_SIZE );
					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( F212_14A );
						m_pArith->l_neg ( F212_14B );
						m_pArith->l_neg ( F212_14C );
						m_pArith->l_neg ( VAL_14D );
						m_pArith->l_neg ( VAL_14E );
					}
					// 個別対応で95％未満の場合
					if ( ( F212_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
						if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
							m_pArith->l_sub ( F212_17C, F212_17C, ( *m_pSnTanaData )->Sn_mnz63[0] );
							m_pArith->l_sub ( F212_18C, F212_18C, ( *m_pSnTanaData )->Sn_mnz63[2] );
							if ( ( *m_pSnHeadData)->SVmzsw == 1 ) {
								m_pArith->l_sub ( F212_17B, F212_17B, ( *m_pSnTanaData )->Sn_mnz40[0] );
								m_pArith->l_sub ( F212_18B, F212_18B, ( *m_pSnTanaData )->Sn_mnz40[2] );
								m_pArith->l_sub ( F212_17A, F212_17A, ( *m_pSnTanaData )->Sn_menzz[0] );
								m_pArith->l_sub ( F212_18A, F212_18A, ( *m_pSnTanaData )->Sn_menzz[2] );
							}
							m_pArith->l_sub ( VAL_17E, VAL_17E, ( *m_pSnTanaData )->Sn_mnz78[0] );
							m_pArith->l_sub ( VAL_18E, VAL_18E, ( *m_pSnTanaData )->Sn_mnz78[2] );
							m_pArith->l_sub (  VAL_17D, VAL_17D, ( *m_pSnTanaData )->Sn_mnz624[0] );
							m_pArith->l_sub (VAL_18D, VAL_18D, ( *m_pSnTanaData )->Sn_mnz624[2] );
						}
						else {
							m_pArith->l_add ( F212_17C, F212_17C, ( *m_pSnTanaData )->Sn_mnz63[0] );
							m_pArith->l_add ( F212_18C, F212_18C, ( *m_pSnTanaData )->Sn_mnz63[2] );
							if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
								m_pArith->l_add ( F212_17B, F212_17B, ( *m_pSnTanaData )->Sn_mnz40[0] );
								m_pArith->l_add ( F212_18B, F212_18B, ( *m_pSnTanaData )->Sn_mnz40[2] );
								m_pArith->l_add ( F212_17A, F212_17A, ( *m_pSnTanaData )->Sn_menzz[0] );
								m_pArith->l_add ( F212_18A, F212_18A, ( *m_pSnTanaData )->Sn_menzz[2] );
							}
							m_pArith->l_add ( VAL_17E, VAL_17E, ( *m_pSnTanaData )->Sn_mnz78[0] );
							m_pArith->l_add ( VAL_18E, VAL_18E, ( *m_pSnTanaData )->Sn_mnz78[2] );
							m_pArith->l_add ( VAL_17D, VAL_17D, ( *m_pSnTanaData )->Sn_mnz624[0] );
							m_pArith->l_add ( VAL_18D, VAL_18D, ( *m_pSnTanaData )->Sn_mnz624[2] );
						}
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F212_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F212_14B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F212_14C );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), F212_17A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), F212_17B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), F212_17C );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), F212_18A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), F212_18B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), F212_18C );

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), VAL_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), VAL_14E );
					if ( ( F212_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), VAL_17D );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), VAL_17E );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), VAL_18D );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), VAL_18E );
					}

				}
				else {
					memset ( F212_14A, '\0', MONY_BUF_SIZE );
					memset ( F212_14B, '\0', MONY_BUF_SIZE );
					memset ( F212_14C, '\0', MONY_BUF_SIZE );

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F212_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F212_14B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F212_14C );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), F212_17A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), F212_17B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), F212_17C );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), F212_18A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), F212_18B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), F212_18C );

					// 付表２－１の金額を更新
					memset ( zerobuf, '\0', MONY_BUF_SIZE );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), zerobuf );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), zerobuf );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), VAL_17D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), VAL_17E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), VAL_18D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), VAL_18E );
				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_12();
				SetItemPosition();
			}
			else if ( !( (  *m_pSnTanaData )->Sn_tansw & 0x01 ) ) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_12();
				SetItemPosition();
			}
		}
	}
	else {
		CChangeTax	CTDiag;
		CTDiag.InitInfo ( ( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );

		// 比例配分
		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle ( g_hInstance );
		int st = ( int )CTDiag.DoModal();
		AfxSetResourceHandle ( svhInstResource );

		if ( st == IDOK ) {

			if( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動
				// (免税)控除対象調整税額
				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
					memmove ( F212_14A, ( *m_pSnTanaData )->Sn_MENZZ, MONY_BUF_SIZE );
					memmove ( F212_14B, ( *m_pSnTanaData )->Sn_MNZ40, MONY_BUF_SIZE );
					memmove ( F212_14C, ( *m_pSnTanaData )->Sn_MNZ63, MONY_BUF_SIZE );
					if (( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( F212_14A );
						m_pArith->l_neg ( F212_14B );
						m_pArith->l_neg ( F212_14C );
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F212_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F212_14B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F212_14C );
				}
				else {
					memset ( F212_14A, '\0', MONY_BUF_SIZE );
					memset ( F212_14B, '\0', MONY_BUF_SIZE );
					memset ( F212_14C, '\0', MONY_BUF_SIZE );

					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
					// 付表２－１もリセット
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
				}


				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_12();
				SetItemPosition();
			}
			else if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01 ) ) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_12();
				SetItemPosition();
			}
		}
	}
}

//=========================================================
// 特定収入の簡易計算式表示
//=========================================================
void CShinFhyo2_12::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	( ( CH31HyoView* )m_pParent )->SpecPassDlg ( 1 );
}

//===================================================
// 特定収入計算表を加味した控除対象仕入税額の計算
//---------------------------------------------------
// 引数
//		SpcData	:	 控除対象仕入税額 特定収入計算表転記用データ
//===================================================
void CShinFhyo2_12::F212calq_spc ( SYZTOSPC_DATA SpcData )
{
	if ( *( m_pShinInfo->pSgSpc ) == 1 ) {
		unsigned char sign = ( *m_pSnHeadData )->Sn_Sign2;
		MoneyBasejagArray	money;
		int cnt = 0;
		int incnt = 0;

		SYZTOSPC_DATA SyzToSpc = { 0 };
		memmove ( &SyzToSpc, &SpcData, sizeof ( SYZTOSPC_DATA ) );

		BOOL IsRatioOver95 = FALSE;
		BOOL Kobetsu = FALSE;

		if ( F212_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();

		// 課税売上セット
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "312200" ) );
		memmove ( SyzToSpc.SpKzur, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "312202" ) );
		memmove ( SyzToSpc.SpKzur624, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "312203" ) );
		memmove ( SyzToSpc.SpKzur78, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "310202" ) );
		memmove ( SyzToSpc.SpKzur4, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "310203" ) );
		memmove ( SyzToSpc.SpKzur63, money[0][0].arith, MONY_BUF_SIZE );

		// 課税売上割合	
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "320800" ) );
		memmove ( SyzToSpc.SpUwri, money[0][0].arith, MONY_BUF_SIZE );

		// 控除対象仕入税額セット
		//if ( IsRatioOver95 ) {
		// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
		if ( IsRatioOver95 && ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) ) {
			// 計算表５（１）
			memmove ( SyzToSpc.SpKgz4, F212_16B, MONY_BUF_SIZE );
			memmove ( SyzToSpc.SpKgz63, F212_16C, MONY_BUF_SIZE );
		}
		else {
			if ( Kobetsu ) {
				// 計算表５（２）
				memmove ( SyzToSpc.SpKgz4, F212_19B, MONY_BUF_SIZE );
				memmove ( SyzToSpc.SpKgz63, F212_19C, MONY_BUF_SIZE );
			}
			else {
				// 計算表５（３）
				memmove ( SyzToSpc.SpKgz4, F212_20B, MONY_BUF_SIZE );
				memmove ( SyzToSpc.SpKgz63, F212_20C, MONY_BUF_SIZE );
			}
		}

		// 再計算
		( ( CH31HyoView* )m_pParent )->RecalqSkj ( &SyzToSpc );

		// 金額再セット
		char mony[MONY_BUF_SIZE] = { 0 };
		char zero[MONY_BUF_SIZE] = { 0 };
		if ( ( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) == 0x00 ) {
			m_SpcSgn = TRUE;

			memset ( zero, '\0', MONY_BUF_SIZE );
			memmove ( mony, SyzToSpc.SpSiz4, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 25B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), mony );
				// 24B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), zero );
				// 付表1-2 4B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), zero );
			}
			else {
				// 25B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), zero );
				// 24B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), mony );
				// 付表1-2 4B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), mony );
			}

			memmove ( mony, SyzToSpc.SpSiz63, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 25C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), mony );
				// 24C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), zero );
				// 付表1-2 4C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), zero );
			}
			else {
				// 25C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), zero );
				// 24C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), mony );
				// 付表1-2 4C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), mony );
			}

			memmove ( mony, SyzToSpc.SpSiz624, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 付表2-1 25D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), mony );
				// 付表2-1 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), zero );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), zero );
			}
			else {
				// 付表2-1 25D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), zero );
				// 付表2-1 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), mony );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), mony );
			}

			memmove ( mony, SyzToSpc.SpSiz78, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 付表2-1 25E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), mony );
				// 付表2-1 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), zero );
				// 付表1-1 4Ｅ欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), zero );
			}
			else {
				// 25E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), zero );
				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), mony );
				// 付表1-1 4Ｅ欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), mony );
			}

			// 特定収入連動メッセージ表示
			m_Fhyo2_12Diag01.ModifyItem ( ID212_TS, DIAG_MDFY_OPAQUE );

			// 簡易計算式ボタン表示
			m_Button2.ShowWindow ( SW_SHOW );

			for ( int index = ID212_23B; index <= ID212_23C; index++ ) {
				ATRIB_MOD ( IDC_ICSDIAG8CTRL1, index, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}
		}
		else {
			// 特定収入割合5%未満
			if ( m_SpcSgn ) {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322302" ) );	// 24B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322303" ) );	// 24C欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322402" ) );	// 25B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322403" ) );	// 25C欄

				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );	// 付表2-1 24D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );	// 付表2-1 24E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );	// 付表2-1 25D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );	// 付表2-1 25E欄

				F212_23c &= ~0x80;
				F212_23c &= ~0x800;

				// 特定収入連動メッセージ非表示
				m_Fhyo2_12Diag01.ModifyItem ( ID212_TS, DIAG_MDFY_TRANSPARENT );

				// 簡易計算式ボタン非表示
				m_Button2.ShowWindow ( SW_HIDE );

				m_SpcSgn = FALSE;

				for ( int index = ID212_23B; index <= ID212_23C; index++ ) {
					ATRIB_MOD ( IDC_ICSDIAG8CTRL1, index, 1, ( *m_pSnHeadData )->Sn_Sign4, 0 );
				}
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄
			}
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "32" ) );
		cnt = ( int )money.GetCount();
		incnt = 0;

		for ( int l = 0; l < cnt; l++ ) {
			incnt = ( int )money[l].GetCount();
			for ( int m = 0; m < incnt; m++ ) {
				if ( money[l][m].code == _T( "322302" ) ) {
					memmove ( F212_23B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322303" ) ) {
					memmove ( F212_23C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322300" ) ) {
					memmove ( F212_23X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322402" ) ) {
					memmove ( F212_24B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322403" ) ) {
					memmove ( F212_24C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322400" ) ) {
					memmove ( F212_24X, money[l][m].arith, MONY_BUF_SIZE );
				}
			}
		}
	}
}

//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo2_12::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F212_10c & 0x888 ) || ( F212_12c & 0x800 ) || ( F212_19c & 0x888 ) ||
			 ( F212_20c & 0x888 ) || ( F212_23c & 0x888 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}