// ShinFhyo2_11.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo2_11.h"
#include "ShinFhyo2_11idx.h"
#include "H31HyoView.h"
#include "ChangeTax2.h"
#include "ChangeTaxEx2.h"


// ShinFhyo2_11 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo2_11, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;


CShinFhyo2_11::CShinFhyo2_11(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo2_11::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo2_01Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pShinInfo ( NULL )
	, m_pSnTanaData ( NULL )
	, m_pSyzSyukei ( NULL )
	, m_SpcSgn ( FALSE )
	, m_UwSgn ( TRUE )
{
	memset ( F211_1X, '\0', sizeof ( F211_1X ) );
	memset ( F211_1D, '\0', sizeof ( F211_1D ) );
	memset ( F211_1E, '\0', sizeof ( F211_1E ) );
	memset ( F211_1F, '\0', sizeof ( F211_1F ) );

	memset ( F211_2F, '\0', sizeof ( F211_2F ) );

	memset ( F211_3F, '\0', sizeof ( F211_3F ) );

	memset ( F211_4F, '\0', sizeof ( F211_4F ) );

	memset ( F211_5F, '\0', sizeof ( F211_5F ) );

	memset ( F211_6F, '\0', sizeof ( F211_6F ) );

	memset ( F211_7F, '\0', sizeof ( F211_7F ) );

	memset ( F211_9X, '\0', sizeof ( F211_9X ) );
	memset ( F211_9D, '\0', sizeof ( F211_9D ) );
	memset ( F211_9E, '\0', sizeof ( F211_9E ) );
	memset ( F211_9F, '\0', sizeof ( F211_9F ) );

	memset ( F211_10X, '\0', sizeof ( F211_10X ) );
	memset ( F211_10D, '\0', sizeof ( F211_10D ) );
	memset ( F211_10E, '\0', sizeof ( F211_10E ) );
	memset ( F211_10F, '\0', sizeof ( F211_10F ) );

	memset ( F211_11X, '\0', sizeof ( F211_11X ) );
	memset ( F211_11E, '\0', sizeof ( F211_11E ) );
	memset ( F211_11F, '\0', sizeof ( F211_11F ) );
	 
	memset ( F211_12X, '\0', sizeof ( F211_12X ) );
	memset ( F211_12E, '\0', sizeof ( F211_12E ) );
	memset ( F211_12F, '\0', sizeof ( F211_12F ) );

	memset ( F211_13X, '\0', sizeof ( F211_13X ) );
	memset ( F211_13D, '\0', sizeof ( F211_13D ) );
	memset ( F211_13E, '\0', sizeof ( F211_13E ) );
	memset ( F211_13F, '\0', sizeof ( F211_13F ) );

	memset ( F211_14X, '\0', sizeof ( F211_14X ) );
	memset ( F211_14D, '\0', sizeof ( F211_14D ) );
	memset ( F211_14E, '\0', sizeof ( F211_14E ) );
	memset ( F211_14F, '\0', sizeof ( F211_14F ) );

	memset ( F211_15X, '\0', sizeof ( F211_15X ) );
	memset ( F211_15D, '\0', sizeof ( F211_15D ) );
	memset ( F211_15E, '\0', sizeof ( F211_15E ) );
	memset ( F211_15F, '\0', sizeof ( F211_15F ) );

	memset ( F211_16X, '\0', sizeof ( F211_16X ) );
	memset ( F211_16D, '\0', sizeof ( F211_16D ) );
	memset ( F211_16E, '\0', sizeof ( F211_16E ) );
	memset ( F211_16F, '\0', sizeof ( F211_16F ) );

	memset ( F211_17X, '\0', sizeof ( F211_17X ) );
	memset ( F211_17D, '\0', sizeof ( F211_17D ) );
	memset ( F211_17E, '\0', sizeof ( F211_17E ) );
	memset ( F211_17F, '\0', sizeof ( F211_17F ) );

	memset ( F211_18X, '\0', sizeof ( F211_18X ) );
	memset ( F211_18D, '\0', sizeof ( F211_18D ) );
	memset ( F211_18E, '\0', sizeof ( F211_18E ) );
	memset ( F211_18F, '\0', sizeof ( F211_18F ) );

	memset ( F211_19X, '\0', sizeof ( F211_19X ) );
	memset ( F211_19D, '\0', sizeof ( F211_19D ) );
	memset ( F211_19E, '\0', sizeof ( F211_19E ) );
	memset ( F211_19F, '\0', sizeof ( F211_19F ) );

	memset ( F211_20X, '\0', sizeof ( F211_20X ) );
	memset ( F211_20D, '\0', sizeof ( F211_20D ) );
	memset ( F211_20E, '\0', sizeof ( F211_20E ) );
	memset ( F211_20F, '\0', sizeof ( F211_20F ) );

	memset ( F211_21X, '\0', sizeof ( F211_21X ) );
	memset ( F211_21D, '\0', sizeof ( F211_21D ) );
	memset ( F211_21E, '\0', sizeof ( F211_21E ) );
	memset ( F211_21F, '\0', sizeof ( F211_21F ) );

	memset ( F211_22X, '\0', sizeof ( F211_22X ) );
	memset ( F211_22D, '\0', sizeof ( F211_22D ) );
	memset ( F211_22E, '\0', sizeof ( F211_22E ) );
	memset ( F211_22F, '\0', sizeof ( F211_22F ) );

	memset ( F211_26X, '\0', sizeof ( F211_26X ) );
	memset ( F211_26D, '\0', sizeof ( F211_26D ) );
	memset ( F211_26E, '\0', sizeof ( F211_26E ) );
	memset ( F211_26F, '\0', sizeof ( F211_26F ) );

	memset ( F211_23X, '\0', sizeof ( F211_23X ) );
	memset ( F211_23D, '\0', sizeof ( F211_23D ) );
	memset ( F211_23E, '\0', sizeof ( F211_23E ) );
	memset ( F211_23F, '\0', sizeof ( F211_23F ) );

	memset ( F211_24X, '\0', sizeof ( F211_24X ) );
	memset ( F211_24D, '\0', sizeof ( F211_24D ) );
	memset ( F211_24E, '\0', sizeof ( F211_24E ) );
	memset ( F211_24F, '\0', sizeof ( F211_24F ) );

	memset ( F211_25X, '\0', sizeof ( F211_25X ) );
	memset ( F211_25D, '\0', sizeof ( F211_25D ) );
	memset ( F211_25E, '\0', sizeof ( F211_25E ) );
	memset ( F211_25F, '\0', sizeof ( F211_25F ) );


	F211_8F = 0;
	F211_8EX = 0;

	F211_10c = F211_12c = F211_19c = F211_20c = F211_23c = 0;

	eymd = 0;

	m_curwnd = -1;
}

CShinFhyo2_11::~CShinFhyo2_11()
{
}

void CShinFhyo2_11::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo2_11Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo2_11Diag02);
	DDX_Control(pDX, IDC_BUTTON1, m_tanaoroshi);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo2_11, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo2_11::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo2_11::OnBnClickedButton2)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo2_11, CSyzBaseDlg)
	ON_EVENT(CShinFhyo2_11, IDC_ICSDIAG8CTRL2, 1, CShinFhyo2_11::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_11, IDC_ICSDIAG8CTRL2, 2, CShinFhyo2_11::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_11, IDC_ICSDIAG8CTRL2, 3, CShinFhyo2_11::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinFhyo2_11, IDC_ICSDIAG8CTRL2, 9, CShinFhyo2_11::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()
// ShinFhyo2_11 メッセージ ハンドラー


BOOL CShinFhyo2_11::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL2 );
	m_BottomIdx = 202;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo2_11::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo2_11::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo2_11::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


BOOL CShinFhyo2_11::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	CWnd *pWnd = GetFocus();

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 付表２－１更新
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
				if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2 ( result, ( short  )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}
				return TRUE;
			}
		}

		if ( pWnd == GetDlgItem ( IDC_BUTTON1 ) ) {
			char VK_FLG = 0x00;
			VK_FLG = ( char )::GetKeyState ( VK_SHIFT );

			if ( ( pMsg->wParam == VK_F3 ) || ( pMsg->wParam == VK_RIGHT ) || ( pMsg->wParam == VK_DOWN ) || ( ( pMsg->wParam == VK_TAB ) && !( VK_FLG & 0x80 ) ) ) {
				if( *m_pShinInfo->pOver500MillionSw != 1 && F211_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){	//5億円かつ課税割合が95％以上の場合
					diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_21D, CIcsdiagctrl );
				}
				else {
					if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
						if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
							diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_17D, CIcsdiagctrl );
						}
						else {
							diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_20D, CIcsdiagctrl );
						}
					}
					else {
						diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_21D, CIcsdiagctrl );
					}
				}
				return TRUE;
			}

			// 前項目へ
			if ( ( pMsg->wParam == VK_LEFT ) || ( pMsg->wParam == VK_F2 ) || ( pMsg->wParam == VK_UP ) || ( ( pMsg->wParam == VK_TAB ) && ( VK_FLG & 0x80 ) ) ) {
				if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
					diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_13E, CIcsdiagctrl );
				}
				else {
					if ( eymd >= ICS_SH_KYOJU_INPUT_DAY ) {
						diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_26E, CIcsdiagctrl );
					}
					else {
						diag_setposition ( IDC_ICSDIAG8CTRL2, ID211_22E, CIcsdiagctrl );
					}
				}
				return TRUE;
			}
		}

	}
	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

void CShinFhyo2_11::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;

}

void CShinFhyo2_11::EditOFFIcsdiag8ctrl2(short index)
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
	case ID211_1D:
	case ID211_1E:
		SyukeiDataSet ( index );

		// 課税売上高の５億超えチェック
		CheckOver500Million();
		// 各入力の表示
		f211data_all ( 1 );
		break;

	case ID211_2F:
		SyukeiDataSet ( index );

		// 課税売上高の５億超えチェック
		CheckOver500Million();
		// 各入力の表示
		f211data_all ( 2 );
		break;

	case ID211_3F:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 3 );
		break;

	case ID211_6F:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 4 );
		break;

	case ID211_9D:
	case ID211_9E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 5 );
		break;

	case ID211_10D:
	case ID211_10E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 6 );
		break;

	case ID211_11E:
		SyukeiDataSet (index );

		// 各入力の表示
		f211data_all ( 11 );
		break;

	case ID211_12E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 12 );
		break;

	case ID211_13D:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F211_13D, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_13D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F211_13D, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F211_13D, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) 
				&& ( F211_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
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
		f211data_all ( 7 );
		break;

	case ID211_13E:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F211_13E, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_13E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F211_13E, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F211_13E, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE )
				&& ( F211_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE )  ) {
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
		f211data_all ( 7 );
		break;

	case ID211_14D:
	case ID211_14E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 7 );
		break;

	case ID211_17D:
	case ID211_17E:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F211_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 8 );
		break;

	case ID211_18D:
	case ID211_18E:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F211_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 8 );
		break;

	case ID211_19D:
	case ID211_19E:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F211_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 8 );
		break;

	case ID211_20D:
	case ID211_20E:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F211_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 8 );
		break;

	case ID211_21D:
	case ID211_21E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 12 );
		break;

	case ID211_22D:
	case ID211_22E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 13 );
		break;

	case ID211_26D:
	case ID211_26E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 13 );
		break;

	case ID211_23D:
	case ID211_23E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 13 );
		break;

	case ID211_24D:
	case ID211_24E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 13 );
		break;

	case ID211_25D:
	case ID211_25E:
		SyukeiDataSet ( index );

		// 各入力の表示
		f211data_all ( 14 );
		break;

	case ID211_8EX:			// 課税売上割合の変更チェック
		if ( m_UwSgn ) {
			( *m_pSnHeadData )->Sn_UPERS &= 0xef;
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			if ( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_UPERS |= 0x10;
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323300" ) );
			}

			f211data_8();
			SyukeiDataSet ( index );

			// 各入力の表示
			f211data_all ( 4 );
		}
		break;

	case ID211_8F:			// 課税売上割合　合計 F
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_8F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		length = DiagData.data_edit.GetLength();
		memset ( buf, '\0', sizeof ( buf ) ); 
		tmpUWexp = 0;
		if ( length ) {
			memmove ( buf, DiagData.data_edit, length );
		}
		m_Util.perinp ( 3, 2, buf, ( short* )&tmpUWexp );
		if ( tmpUWexp <= 10000 ) {
			( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW = tmpUWexp;
		}

		// 個別対応で95％未満の場合
		if ( ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) && ( F211_8F != ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW ) ) {
			CString		Msg;
			Msg.Empty();
			Msg = _T( "課税売上割合が変更されました。\n" );
			Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
			ICSMessageBox ( Msg, MB_OK, 0 );
			F211_8F = ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW;
			f211data_8();
			MemoryReget();
			// 各入力の計算表示
			f211data_all ( 4 );
			break;
		}
		else {
			// 個別対応で95％未満の場合
			if ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) && ( F211_8F != ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW )
				&& ( ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW < ( *m_pSnHeadData )->m_UriRatioBorder ) ) {
				CString		Msg;
				Msg.Empty();
				Msg = _T( "課税売上割合が変更されました。\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
			F211_8F = ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW;
		}

		f211data_8();
		SyukeiDataSet ( index );

		// 各入力の計算表示
		f211data_all ( 4 );

		break;

	default:
		break;

	}
}

void CShinFhyo2_11::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Fhyo2_11Diag02.SetPosition ( pos );
	}
	if ( wTerm==VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index , 0 );
		m_Fhyo2_11Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) ){
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
			if ( index == ID211_13E ) {
				c_set ( IDC_BUTTON1 );
			}
			else {
				m_Fhyo2_11Diag02.SetNextPosition();
			}
		}
		else {
			if ( index == ID211_20E) {
				c_set ( IDC_BUTTON1 );
			}
			else{
				m_Fhyo2_11Diag02.SetNextPosition();
			}

		}

	}

	// 前項目へ
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		BOOL IsPrevPosition = TRUE;
		if ( ( *m_pShinInfo->pOver500MillionSw != 1 ) && ( F211_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) ) {	//5億円以下かつ 課税売上割合が９５％以上の場合
			if ( index == ID211_19D ) {
				c_set ( IDC_BUTTON1 );
				IsPrevPosition = FALSE;
			}
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
					if ( index == ID211_17D ) {
						c_set ( IDC_BUTTON1 );
						IsPrevPosition = FALSE;
					}
				}
				else {
					if ( index == ID211_20D ) {
						c_set ( IDC_BUTTON1 );
						IsPrevPosition = FALSE;
					}
				}
			}
			else {
				if ( index == ID211_21D ) {
					c_set ( IDC_BUTTON1 );
					IsPrevPosition = FALSE;
				}
			}
		}
		if ( IsPrevPosition ) {
			m_Fhyo2_11Diag02.SetPrevPosition();
		}

	}
	if ( wTerm == VK_DELETE ) {
		switch( index ){
		case ID211_23D:
			F211_23c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );
			// 各入力の表示
			f211data_all ( 13 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_23D );
			break;

		case ID211_23E:
			F211_23c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );
			// 各入力の表示
			f211data_all ( 13 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_23E );
			break;

		case ID211_10D:
			F211_10c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			// 各入力の表示
			f211data_all ( 6 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_10D );
			break;

		case ID211_10E:
			F211_10c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			// 各入力の表示
			f211data_all ( 6 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_10E );
			break;

		case ID211_12E:
			F211_12c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			// 各入力の表示
			f211data_all ( 16 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_12E );
			break;

		case ID211_19D:
			F211_19c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324402" ) );
			// 各入力の表示
			f211data_all ( 8 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_19D );
			break;

		case ID211_19E:
			F211_19c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324403" ) );
			// 各入力の表示
			f211data_all ( 8 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_19E );
			break;

		case ID211_20D:
			F211_20c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324502" ) );
			// 各入力の表示
			f211data_all( 8 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_20D );
			break;
		case ID211_20E:
			F211_20c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324503" ) );
			// 各入力の表示
			f211data_all ( 8 );
			m_Fhyo2_11Diag02.SetPosition ( ID211_20E );
			break;
		case ID211_8EX:		// 課税売上割合変更チェック
			break;
		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2 , index );
			break;
		}

	}

	int nowpos = 0;
	CWnd* pWnd = this->GetFocus();
	if ( pWnd->GetDlgCtrlID() == IDC_BUTTON1 ) {
		nowpos = ID211_14F;	// 14Fのポジションをセット
	}
	else {
		nowpos = m_Fhyo2_11Diag02.GetPosition();	// 移動先または自身のポジション
	}

	RECT	rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int	cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

void CShinFhyo2_11::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char flg[MONY_BUF_SIZE] = { 0 };

	// ICSMessageBoxによってEditOffへはしるのを防ぐ
	m_UwSgn = FALSE;

	// 課税売上割合の変更チェック
	if ( index == ID211_8EX ) {
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
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
					m_pArith->l_input ( flg, _T( "1" ) );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "FF3002" ), flg );
				}
			}
			else {
				if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) {
					Msg = _T( "課税売上割合に準ずる割合を無効にします。" );
					ICSMessageBox ( Msg, MB_OK, 0 );
					( *m_pSnHeadData )->Sn_UPERS &= 0xfd;
					m_pArith->l_input ( flg, _T( "0" ) );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "FF3002" ), flg );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323300" ) );
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

		m_UwSgn = TRUE;

		m_Fhyo2_11Diag02.SetPosition ( index );

		( *m_pSnHeadData )->Sn_UPERS &= 0xef;
		if ( DiagData.data_check ) {
			( *m_pSnHeadData )->Sn_UPERS |= 0x10;
		}

		// 各入力の計算表示
		f211data_all ( 4 );
	}
}


//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo2_01Data	：	付表2-1データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSnTanaData		：	棚卸データクラス
//		pSyzSyukei		：	集計クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo2_11::InitInfo ( CH31SnFhyo2_01Data **pSnFhyo2_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH31SnTanaData **pSnTanaData, CSyzSyukei **pSyzSyukei )
{
	ASSERT ( pSnFhyo2_01Data );
	if ( pSnFhyo2_01Data == NULL ) {
		return -1;
	}
	m_pSnFhyo2_01Data = pSnFhyo2_01Data;


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
int CShinFhyo2_11::Init()
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
	m_Fhyo2_11Diag02.ModifyItem ( ID211_TS, DIAG_MDFY_TRANSPARENT );

	// 集計
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo2_11Diag01.ModifyItem ( 5, DIAG_MDFY_TRANSPARENT );
	}


	// 画面出力
	DispFh2_11();
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
int CShinFhyo2_11::ReadData()
{
	// 1.課税売上額(税抜き)
	memmove ( F211_1X, ( *m_pSnFhyo2_01Data )->Sn_F201_1X, MONY_BUF_SIZE );
	memmove ( F211_1D, ( *m_pSnFhyo2_01Data )->Sn_F201_1D, MONY_BUF_SIZE );
	memmove ( F211_1E, ( *m_pSnFhyo2_01Data )->Sn_F201_1E, MONY_BUF_SIZE );
	memmove ( F211_1F, ( *m_pSnFhyo2_01Data )->Sn_F201_1F, MONY_BUF_SIZE );

	// 2.免税売上額
	memmove ( F211_2F, ( *m_pSnFhyo2_01Data )->Sn_F201_2F, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額…
	memmove ( F211_3F, ( *m_pSnFhyo2_01Data )->Sn_F201_3F, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove ( F211_4F, ( *m_pSnFhyo2_01Data )->Sn_F201_4F, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	memmove ( F211_5F, ( *m_pSnFhyo2_01Data )->Sn_F201_5F, MONY_BUF_SIZE );

	// 6.非課税売上額
	memmove ( F211_6F, ( *m_pSnFhyo2_01Data )->Sn_F201_6F, MONY_BUF_SIZE );

	// 7.課税資産の譲渡等の対価の額
	memmove ( F211_7F, ( *m_pSnFhyo2_01Data )->Sn_F201_7F, MONY_BUF_SIZE );

	// 8.課税売上割合
	F211_8EX = ( *m_pSnFhyo2_01Data )->Sn_F201_8EX;
	F211_8F = ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove ( F211_9X, ( *m_pSnFhyo2_01Data )->Sn_F201_9X, MONY_BUF_SIZE );
	memmove ( F211_9D, ( *m_pSnFhyo2_01Data )->Sn_F201_9D, MONY_BUF_SIZE );
	memmove ( F211_9E, ( *m_pSnFhyo2_01Data )->Sn_F201_9E, MONY_BUF_SIZE );
	memmove ( F211_9F, ( *m_pSnFhyo2_01Data )->Sn_F201_9F, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove ( F211_10X, ( *m_pSnFhyo2_01Data )->Sn_F201_10X, MONY_BUF_SIZE );
	memmove ( F211_10D, ( *m_pSnFhyo2_01Data )->Sn_F201_10D, MONY_BUF_SIZE );
	memmove ( F211_10E, ( *m_pSnFhyo2_01Data )->Sn_F201_10E, MONY_BUF_SIZE );
	memmove ( F211_10F, ( *m_pSnFhyo2_01Data )->Sn_F201_10F, MONY_BUF_SIZE );
	F211_10c = ( *m_pSnFhyo2_01Data )->Sn_F201_10sw;

	// 11.特定課税仕入れに係る支払対価の額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F211_11X, ( *m_pSnFhyo2_01Data )->Sn_F201_11X, MONY_BUF_SIZE );
		memmove ( F211_11E, ( *m_pSnFhyo2_01Data )->Sn_F201_11E, MONY_BUF_SIZE );
		memmove ( F211_11F, ( *m_pSnFhyo2_01Data )->Sn_F201_11F, MONY_BUF_SIZE );
	}
	else {
		memset( F211_11X, '\0', MONY_BUF_SIZE );
		memset( F211_11E, '\0', MONY_BUF_SIZE );
		memset( F211_11F, '\0', MONY_BUF_SIZE );
	}

	// 12.特定課税仕入れに係る消費税額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F211_12X, ( *m_pSnFhyo2_01Data )->Sn_F201_12X, MONY_BUF_SIZE );
		memmove ( F211_12E, ( *m_pSnFhyo2_01Data )->Sn_F201_12E, MONY_BUF_SIZE );
		memmove ( F211_12F, ( *m_pSnFhyo2_01Data )->Sn_F201_12F, MONY_BUF_SIZE );
		F211_12c = ( *m_pSnFhyo2_01Data )->Sn_F201_12sw;
	}
	else {
		memset( F211_12X, '\0', MONY_BUF_SIZE );
		memset( F211_12E, '\0', MONY_BUF_SIZE );
		memset( F211_12F, '\0', MONY_BUF_SIZE );
		F211_12c = 0;
	}

	// 13.課税貨物に係る消費税額
	memmove ( F211_13X, ( *m_pSnFhyo2_01Data )->Sn_F201_13X, MONY_BUF_SIZE );
	memmove ( F211_13D, ( *m_pSnFhyo2_01Data )->Sn_F201_13D, MONY_BUF_SIZE );
	memmove ( F211_13E, ( *m_pSnFhyo2_01Data )->Sn_F201_13E, MONY_BUF_SIZE );
	memmove ( F211_13F, ( *m_pSnFhyo2_01Data )->Sn_F201_13F, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove ( F211_14X, ( *m_pSnFhyo2_01Data )->Sn_F201_14X, MONY_BUF_SIZE );
	memmove ( F211_14D, ( *m_pSnFhyo2_01Data )->Sn_F201_14D, MONY_BUF_SIZE );
	memmove ( F211_14E, ( *m_pSnFhyo2_01Data )->Sn_F201_14E, MONY_BUF_SIZE );
	memmove ( F211_14F, ( *m_pSnFhyo2_01Data )->Sn_F201_14F, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove ( F211_15X, ( *m_pSnFhyo2_01Data )->Sn_F201_15X, MONY_BUF_SIZE );
	memmove ( F211_15D, ( *m_pSnFhyo2_01Data )->Sn_F201_15D, MONY_BUF_SIZE );
	memmove ( F211_15E, ( *m_pSnFhyo2_01Data )->Sn_F201_15E, MONY_BUF_SIZE );
	memmove ( F211_15F, ( *m_pSnFhyo2_01Data )->Sn_F201_15F, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove ( F211_16X, ( *m_pSnFhyo2_01Data )->Sn_F201_16X, MONY_BUF_SIZE );
	memmove ( F211_16D, ( *m_pSnFhyo2_01Data )->Sn_F201_16D, MONY_BUF_SIZE );
	memmove ( F211_16E, ( *m_pSnFhyo2_01Data )->Sn_F201_16E, MONY_BUF_SIZE );
	memmove ( F211_16F, ( *m_pSnFhyo2_01Data )->Sn_F201_16F, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove ( F211_17X, ( *m_pSnFhyo2_01Data )->Sn_F201_17X, MONY_BUF_SIZE );
	memmove ( F211_17D, ( *m_pSnFhyo2_01Data )->Sn_F201_17D, MONY_BUF_SIZE );
	memmove ( F211_17E, ( *m_pSnFhyo2_01Data )->Sn_F201_17E, MONY_BUF_SIZE );
	memmove ( F211_17F, ( *m_pSnFhyo2_01Data )->Sn_F201_17F, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove ( F211_18X, ( *m_pSnFhyo2_01Data )->Sn_F201_18X, MONY_BUF_SIZE );
	memmove ( F211_18D, ( *m_pSnFhyo2_01Data )->Sn_F201_18D, MONY_BUF_SIZE );
	memmove ( F211_18E, ( *m_pSnFhyo2_01Data )->Sn_F201_18E, MONY_BUF_SIZE );
	memmove ( F211_18F, ( *m_pSnFhyo2_01Data )->Sn_F201_18F, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove ( F211_19X, ( *m_pSnFhyo2_01Data )->Sn_F201_19X, MONY_BUF_SIZE );
	memmove ( F211_19D, ( *m_pSnFhyo2_01Data )->Sn_F201_19D, MONY_BUF_SIZE );
	memmove ( F211_19E, ( *m_pSnFhyo2_01Data )->Sn_F201_19E, MONY_BUF_SIZE );
	memmove ( F211_19F, ( *m_pSnFhyo2_01Data )->Sn_F201_19F, MONY_BUF_SIZE );
	F211_19c = ( *m_pSnFhyo2_01Data )->Sn_F201_19sw;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove ( F211_20X, ( *m_pSnFhyo2_01Data )->Sn_F201_20X, MONY_BUF_SIZE );
	memmove ( F211_20D, ( *m_pSnFhyo2_01Data )->Sn_F201_20D, MONY_BUF_SIZE );
	memmove ( F211_20E, ( *m_pSnFhyo2_01Data )->Sn_F201_20E, MONY_BUF_SIZE );
	memmove ( F211_20F, ( *m_pSnFhyo2_01Data )->Sn_F201_20F, MONY_BUF_SIZE );
	F211_20c = ( *m_pSnFhyo2_01Data )->Sn_F201_20sw;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove ( F211_21X, ( *m_pSnFhyo2_01Data )->Sn_F201_21X, MONY_BUF_SIZE );
	memmove ( F211_21D, ( *m_pSnFhyo2_01Data )->Sn_F201_21D, MONY_BUF_SIZE );
	memmove ( F211_21E, ( *m_pSnFhyo2_01Data )->Sn_F201_21E, MONY_BUF_SIZE );
	memmove ( F211_21F, ( *m_pSnFhyo2_01Data )->Sn_F201_21F, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove ( F211_22X, ( *m_pSnFhyo2_01Data )->Sn_F201_22X, MONY_BUF_SIZE );
	memmove ( F211_22D, ( *m_pSnFhyo2_01Data )->Sn_F201_22D, MONY_BUF_SIZE );
	memmove ( F211_22E, ( *m_pSnFhyo2_01Data )->Sn_F201_22E, MONY_BUF_SIZE );
	memmove ( F211_22F, ( *m_pSnFhyo2_01Data )->Sn_F201_22F, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	memmove ( F211_26X, ( *m_pSnFhyo2_01Data )->Sn_F201_26X, MONY_BUF_SIZE );
	memmove ( F211_26D, ( *m_pSnFhyo2_01Data )->Sn_F201_26D, MONY_BUF_SIZE );
	memmove ( F211_26E, ( *m_pSnFhyo2_01Data )->Sn_F201_26E, MONY_BUF_SIZE );
	memmove ( F211_26F, ( *m_pSnFhyo2_01Data )->Sn_F201_26F, MONY_BUF_SIZE );

	// 24.控除対象仕入税額
	memmove ( F211_23X, ( *m_pSnFhyo2_01Data )->Sn_F201_23X, MONY_BUF_SIZE );
	memmove ( F211_23D, ( *m_pSnFhyo2_01Data )->Sn_F201_23D, MONY_BUF_SIZE );
	memmove ( F211_23E, ( *m_pSnFhyo2_01Data )->Sn_F201_23E, MONY_BUF_SIZE );
	memmove ( F211_23F, ( *m_pSnFhyo2_01Data )->Sn_F201_23F, MONY_BUF_SIZE );
	F211_23c = ( *m_pSnFhyo2_01Data )->Sn_F201_23sw;

	// 25.控除過大調整税額
	memmove ( F211_24X, ( *m_pSnFhyo2_01Data )->Sn_F201_24X, MONY_BUF_SIZE );
	memmove ( F211_24D, ( *m_pSnFhyo2_01Data )->Sn_F201_24D, MONY_BUF_SIZE );
	memmove ( F211_24E, ( *m_pSnFhyo2_01Data )->Sn_F201_24E, MONY_BUF_SIZE );
	memmove ( F211_24F, ( *m_pSnFhyo2_01Data )->Sn_F201_24F, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	memmove ( F211_25X, ( *m_pSnFhyo2_01Data )->Sn_F201_25X, MONY_BUF_SIZE );
	memmove ( F211_25D, ( *m_pSnFhyo2_01Data )->Sn_F201_25D, MONY_BUF_SIZE );
	memmove ( F211_25E, ( *m_pSnFhyo2_01Data )->Sn_F201_25E, MONY_BUF_SIZE );
	memmove ( F211_25F, ( *m_pSnFhyo2_01Data )->Sn_F201_25F, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo2_11::ReadData2()
{
	char mony[MONY_BUF_SIZE] = { 0 }, flg[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );
	memset ( flg, '\0', MONY_BUF_SIZE );

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911104" ), F211_1D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322602" ), F211_1D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911105" ), F211_1E );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322603" ), F211_1E );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322700" ), F211_2F );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322800" ), F211_3F );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323100" ), F211_6F );

		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323300" ) );

		// 特例計算を行う場合は、9～14の6.24%と7.8%と合計は0とする
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			memset ( mony, '\0', MONY_BUF_SIZE);

			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323400" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323402" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323403" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323500" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323502" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323503" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323600" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323603" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323700" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323703" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323800" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323802" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323803" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323900" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323902" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323903" ), mony );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323400" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323500" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323600" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323603" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323700" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323800" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323900" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324002" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324003" ) );

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323402" ), F211_9D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323403" ), F211_9E );

			if ( F211_10c & 0x80 ) {
				( *m_pSyzSyukei)->SetShinkokuData ( _T( "323502" ), F211_10D );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			}
			if ( F211_10c & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "323503" ), F211_10E );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			}

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323603" ), F211_11E );

			if ( F211_12c & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "323703" ), F211_12E );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			}

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323802" ), F211_13D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323803" ), F211_13E );

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F211_14D);
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F211_14E);
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F211_17D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F211_17E );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F211_18D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F211_18E );

		if ( F211_19c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324402" ), F211_19D );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324402" ) );
		}

		if ( F211_19c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324403" ), F211_19E );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324403" ) );
		}

		if ( F211_20c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324502" ), F211_20D );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324502" ) );
		}

		if ( F211_20c & 0x800 ) {
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "324503" ), F211_20D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324503" ), F211_20E );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324503" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324602") , F211_21D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324603") , F211_21E );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324702") , F211_22D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324703") , F211_22E );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327102") , F211_26D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327103") , F211_26E );

		if ( F211_23c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), F211_23D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), F211_24D );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );
		}

		if ( F211_23c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), F211_23E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), F211_24E );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F211_25D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F211_25E );

		if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322601" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323401" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323501" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323601" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323701" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323801" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323901" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324001" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324101" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324201" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324301" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324401" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324501" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324601" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324701" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324801" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324901" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "325001" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "327101" ) );
		}
		else {
			// 経過措置なし　旧税率合計を０クリア
			memset(mony, '\0', MONY_BUF_SIZE);

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "322601" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323401" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323501" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323601" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323701" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323801" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323901" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324001" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324101" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324201" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324301" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324401" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324501" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324601" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324701" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324801" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324901" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "325001" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "327101") , mony );
		}

		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x02 ) {
			m_pArith->l_input ( flg, _T( "1" ) );
		}
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "FF3002" ), flg );
	}
	else {

		// 特例計算を行う場合は、9～14の6.24%と7.8%と合計は0とする
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			memset ( mony, '\0', MONY_BUF_SIZE);

			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323400" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323402" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323403" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323500" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323502" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323503" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323600" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323603" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323700" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323703" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323800" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323802" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323803" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323900" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323902" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323903" ), mony );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324602" ), F211_21D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324603" ), F211_21E );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324702" ), F211_22D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324703" ), F211_22E );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327102") , F211_26D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327103") , F211_26E );
	}
}

//=========================================================
// データ書込み
//---------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo2_11::WriteData()
{
	// 1.課税売上額(税抜き)
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1X, F211_1X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1D, F211_1D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1E, F211_1E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1F, F211_1F, MONY_BUF_SIZE );

	// 2.免税売上額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_2F, F211_2F, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額…
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_3F, F211_3F, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_4F, F211_4F, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_5F, F211_5F, MONY_BUF_SIZE );

	// 6.非課税売上額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_6F, F211_6F, MONY_BUF_SIZE );

	// 7.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_7F, F211_7F, MONY_BUF_SIZE );

	// 8.課税売上割合
	( *m_pSnFhyo2_01Data )->Sn_F201_8EX = F211_8EX;
	( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW = F211_8F;
	( *m_pSnHeadData )->Sn_UPERS &= 0xfe;
	if ( ( ( *m_pSnFhyo2_01Data )->Sn_F201_8EX ) < ( *m_pSnHeadData )->m_UriRatioBorder ) {
		( *m_pSnHeadData )->Sn_UPERS |= 0x01;
	}

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9X, F211_9X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9D, F211_9D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9E, F211_9E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9F, F211_9F, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10X, F211_10X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10D, F211_10D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10E, F211_10E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10F, F211_10F, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_10sw = F211_10c;

	// 11.特定課税仕入れに係る支払対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_11X, F211_11X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_11E, F211_11E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_11F, F211_11F, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_12X, F211_12X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_12E, F211_12E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_12F, F211_12F, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_12sw = F211_12c;

	// 13.課税貨物に係る消費税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13X, F211_13X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13D, F211_13D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13E, F211_13E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13F, F211_13F, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14X, F211_14X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14D, F211_14D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14E, F211_14E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14F, F211_14F, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15X, F211_15X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15D, F211_15D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15E, F211_15E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15F, F211_15F, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16X, F211_16X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16D, F211_16D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16E, F211_16E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16F, F211_16F, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17X, F211_17X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17D, F211_17D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17E, F211_17E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17F, F211_17F, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18X, F211_18X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18D, F211_18D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18E, F211_18E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18F, F211_18F, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19X, F211_19X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19D, F211_19D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19E, F211_19E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19F, F211_19F, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_19sw = F211_19c;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20X, F211_20X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20D, F211_20D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20E, F211_20E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20F, F211_20F, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_20sw = F211_20c;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21X, F211_21X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21D, F211_21D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21E, F211_21E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21F, F211_21F, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22X, F211_22X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22D, F211_22D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22E, F211_22E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22F, F211_22F, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26X, F211_26X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26D, F211_26D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26E, F211_26E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26F, F211_26F, MONY_BUF_SIZE );

	// 24.控除対象仕入税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23X, F211_23X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23D, F211_23D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23E, F211_23E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23F, F211_23F, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_23sw = F211_23c;

	// 25.控除過大調整税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24X, F211_24X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24D, F211_24D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24E, F211_24E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24F, F211_24F, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25X, F211_25X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25D, F211_25D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25E, F211_25E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25F, F211_25F, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo2_11::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目を全て再取得
	MoneyBasejagArray money;
	SYZTOSPC_DATA SyzToSpc = { 0 };

	// 個別対応方式か
	BOOL Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();
	// 課税売上割合に準ずる割合が有効か
	int semiuri = ( *m_pSnHeadData )->IsUseSemiUriRatio();

	TanaDataAdd();

	int Flg = 0;
	int	dmysw = 0;
	int	tmpFlg = 0;
	char VAL[MONY_BUF_SIZE] = { 0 };
	char ValCnv[MONY_BUF_SIZE] = { 0 };
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
			if ( money[l][m].code == _T( "322601" ) ) {
				memmove ( F211_1X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322602" ) ) {
				memmove ( F211_1D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322603" ) ) {
				memmove ( F211_1E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322600" ) ) {
				memmove ( F211_1F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322700" ) ) {
				memmove ( F211_2F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322800" ) ) {
				memmove ( F211_3F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322900" ) ) {
				memmove ( F211_4F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323000" ) ) {
				memmove ( F211_5F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323100" ) ) {
				memmove ( F211_6F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323200" ) ) {
				memmove ( F211_7F, money[l][m].arith, MONY_BUF_SIZE ); 
			}
			else if ( money[l][m].code == _T( "323300" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai,  money[l][m].arith, _T( "SSSSSSSSSSSSS9" ) );
				F211_8EX = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "323401" ) ) {
				memmove ( F211_9X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323402" ) ) {
				memmove ( F211_9D, money[l][m].arith, MONY_BUF_SIZE);
			}
			else if ( money[l][m].code == _T( "323403" ) ) {
				memmove ( F211_9E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323400" ) ) {
				memmove ( F211_9F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323501" ) ) {
				memmove ( F211_10X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323502" ) ) {
				memmove ( F211_10D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323503" ) ) {
				memmove ( F211_10E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323500" ) ) {
				memmove ( F211_10F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323601" ) ) {
				memmove ( F211_11X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323603" ) ) {
				memmove ( F211_11E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323600" ) ) {
				memmove ( F211_11F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323701" ) ) {
				memmove ( F211_12X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323703" ) ) {
				memmove ( F211_12E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323700" ) ) {
				memmove ( F211_12F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323801" ) ) {
				memmove ( F211_13X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323802" ) ) {
				memmove ( F211_13D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323803" ) ) {
				memmove ( F211_13E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323800" ) ) {
				memmove ( F211_13F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323901" ) ) {
				memmove ( F211_14X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323902" ) ) {
				memmove ( F211_14D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323903" ) ) {
				memmove ( F211_14E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323900" ) ) {
				memmove ( F211_14F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324001" ) ) {
				memmove ( F211_15X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324002" ) ) {
				memmove ( F211_15D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324003" ) ) {
				memmove ( F211_15E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324000" ) ) {
				memmove ( F211_15F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324101" ) ) {
				memmove ( F211_16X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324102" ) ) {
				memmove ( F211_16D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324103" ) ) {
				memmove ( F211_16E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324100" ) ) {
				memmove ( F211_16F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324201" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_17X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_17X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324202" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_17D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_17D, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324203" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_17E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_17E, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324200" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_17F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_17F, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324301" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_18X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_18X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324302" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_18D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_18D, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324303" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_18E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_18E, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324300" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_18F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_18F, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324401" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_19X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_19X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324402" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_19D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_19D, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324403" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_19E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_19E, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324400" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F211_19F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_19F, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324501" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F211_20X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_20X, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324502" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F211_20D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_20D, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324503" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F211_20E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_20E, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324500" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F211_20F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F211_20F, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324601" ) ) {
				memmove ( F211_21X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324602" ) ) {
				memmove ( F211_21D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324603" ) ) {
				memmove ( F211_21E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324600" ) ) {
				memmove ( F211_21F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324701" ) ) {
				memmove ( F211_22X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324702" ) ) {
				memmove ( F211_22D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324703" ) ) {
				memmove ( F211_22E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324700" ) ) {
				memmove ( F211_22F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327101" ) ) {
				memmove ( F211_26X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327102" ) ) {
				memmove ( F211_26D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327103" ) ) {
				memmove ( F211_26E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327100" ) ) {
				memmove ( F211_26F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324801" ) ) {
				memmove ( F211_23X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324802" ) ) {
				memmove ( F211_23D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324803" ) ) {
				memmove ( F211_23E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324800" ) ) {
				memmove ( F211_23F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324901" ) ) {
				memmove ( F211_24X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324902" ) ) {
				memmove ( F211_24D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324903" ) ) {
				memmove ( F211_24E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324900" ) ) {
				memmove ( F211_24F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325001" ) ) {
				memmove ( F211_25X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325002" ) ) {
				memmove ( F211_25D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325003" ) ) {
				memmove ( F211_25E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325000" ) ) {
				memmove ( F211_25F, money[l][m].arith, MONY_BUF_SIZE );
			}

			if ( ( *( m_pShinInfo )->pSgSpc == 1 ) && ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// ( !Flg ) {
				if ( !Flg && !semiuri ) {
					if ( money[l][m].code == _T( "321602" ) ) {
						memmove ( SyzToSpc.SpKgz4, money[l][m].arith, sizeof ( SyzToSpc.SpKgz4 ) );
					}
					else if ( money[l][m].code == _T( "321603" ) ) {
						memmove ( SyzToSpc.SpKgz63, money[l][m].arith, sizeof ( SyzToSpc.SpKgz63 ) );
					}
				}
				else {
					if ( Kobetsu ) {
						if ( Flg || semiuri ) {
							if ( money[l][m].code == _T( "321902" ) ) {
								memmove ( SyzToSpc.SpKgz4, money[l][m].arith, sizeof ( SyzToSpc.SpKgz4 ) );
							}
							else if ( money[l][m].code == _T( "321903" ) ) {
								memmove ( SyzToSpc.SpKgz63, money[l][m].arith, sizeof ( SyzToSpc.SpKgz63 ) );
							}
						}
					}
					else {
						if ( Flg ) {
							if ( money[l][m].code == _T( "322002" ) ) {
								memmove ( SyzToSpc.SpKgz4, money[l][m].arith, sizeof ( SyzToSpc.SpKgz4 ) );
							}
							else if ( money[l][m].code == _T( "322003" ) ) {
								memmove ( SyzToSpc.SpKgz63, money[l][m].arith, sizeof ( SyzToSpc.SpKgz63 ) );
							}
						}
					}
				}
			}
		}
	}

	// 特定収入計算表対応
	if ( *( m_pShinInfo )->pSgSpc == 1 ) {
		f211calq_spc ( SyzToSpc );
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
int CShinFhyo2_11::SyukeiDataSet ( int cID )
{
	char val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID211_1D:
		memmove ( F211_1D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322602" ), F211_1D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911104" ), F211_1D );
		break;
	case ID211_1E:
		memmove ( F211_1E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322603" ), F211_1E );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911105" ), F211_1E );
		break;
	case ID211_2F:
		memmove ( F211_2F, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322700" ), F211_2F );
		break;
	case ID211_3F:
		memmove ( F211_3F, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322800" ), F211_3F );
		break;
	case ID211_6F:
		memmove ( F211_6F, val, MONY_BUF_SIZE);
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323100" ), F211_6F );
		break;
	case ID211_9D:
		memmove ( F211_9D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323402" ), F211_9D );
		break;
	case ID211_9E:
		memmove ( F211_9E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323403" ), F211_9E );
		break;
	case ID211_10D:
		if ( m_pArith->l_cmp ( F211_10D, val ) ) {
			F211_10c |= 0x80;
			memmove ( F211_10D, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323502" ), F211_10D );
		}
		break;
	case ID211_10E:
		if ( m_pArith->l_cmp ( F211_10E, val ) ) {
			F211_10c |= 0x800;
			memmove ( F211_10E, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323503" ), F211_10E );
		}
		break;
	case ID211_11E:
		memmove ( F211_11E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323603" ), F211_11E );
		break;
	case ID211_12E:
		if ( m_pArith->l_cmp ( F211_12E, val ) ) {
			F211_12c |= 0x800;
			memmove ( F211_12E, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323703" ), F211_12E );
		}
		break;
	case ID211_13D:
		memmove ( F211_13D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323802" ), F211_13D );
		break;
	case ID211_13E:
		memmove ( F211_13E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323803" ), F211_13E );
		break;
	case ID211_14D:
		memmove ( F211_14D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F211_14D );
		break;
	case ID211_14E:
		memmove ( F211_14E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F211_14E );
		break;
	case ID211_17D:
		memmove ( F211_17D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F211_17D );
		break;
	case ID211_17E:
		memmove ( F211_17E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F211_17E );
		break;
	case ID211_18D:
		memmove ( F211_18D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F211_18D );
		break;
	case ID211_18E:
		memmove ( F211_18E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData (_T( "324303" ), F211_18E );
		break;
	case ID211_19D:
		if ( m_pArith->l_cmp ( F211_19D, val ) ) {
			F211_19c |= 0x80;
			memmove ( F211_19D, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324402" ), F211_19D );
		}
		break;
	case ID211_19E:
		if ( m_pArith->l_cmp ( F211_19E, val)) {
			F211_19c |= 0x800;
			memmove ( F211_19E, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324403" ), F211_19E );
		}
		break;
	case ID211_20D:
		if ( m_pArith->l_cmp ( F211_20D, val ) ) {
			F211_20c |= 0x80;
			memmove ( F211_20D, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324502" ), F211_20D );
		}
		break;
	case ID211_20E:
		if ( m_pArith->l_cmp ( F211_20E, val ) ) {
			F211_20c |= 0x800;
			memmove ( F211_20E, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324503" ), F211_20E );
		}
		break;
	case ID211_21D:
		memmove ( F211_21D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324602" ), F211_21D );
		break;
	case ID211_21E:
		memmove ( F211_21E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324603" ), F211_21E );
		break;
	case ID211_22D:
		memmove ( F211_22D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324702" ), F211_22D );
		break;
	case ID211_22E:
		memmove ( F211_22E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324703" ), F211_22E );
		break;
	case ID211_26D:
		memmove ( F211_26D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327102" ), F211_26D );
		break;
	case ID211_26E:
		memmove ( F211_26E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327103" ), F211_26E );
		break;
	case ID211_23D:
		if ( m_pArith->l_cmp ( F211_23D, val ) ) {
			F211_23c |= 0x80;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F211_24D, val, MONY_BUF_SIZE );
				memset ( F211_23D, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F211_23D, val, MONY_BUF_SIZE );
				memset ( F211_24D, '\0', MONY_BUF_SIZE);
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), F211_23D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), F211_24D );
		}
		break;
	case ID211_23E:
		if ( m_pArith->l_cmp ( F211_23E, val ) ) {
			F211_23c |= 0x800;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F211_24E, val, MONY_BUF_SIZE );
				memset ( F211_23E, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F211_23E, val, MONY_BUF_SIZE );
				memset ( F211_24E, '\0', MONY_BUF_SIZE );
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), F211_23E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), F211_24E );
		}
		break;
	case ID211_24D:
		memmove ( F211_24D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), F211_24D );
		break;
	case ID211_24E:
		memmove ( F211_24E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), F211_24E );
		break;
	case ID211_25D:
		memmove ( F211_25D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F211_25D );
		break;
	case ID211_25E:
		memmove ( F211_25E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F211_25E );
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
int CShinFhyo2_11::InitDiagAttr()
{

	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo2_11[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo2_11[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo2_11[cnt].UnConnect;

				int st = ChkTokureiIdx ( Fhyo2_11[cnt].Index );
				if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) && ( st == 1 ) ) {
					Flg = 0;
				}
				else {
					st = ChkTkkzIdx ( Fhyo2_11[cnt].Index );
					if ( st == 1 ) {
						if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
							Flg = 1;
						}
						else {
							Flg = 0;
						}
					}
				}
			}
			else {
				Flg = Fhyo2_11[cnt].Connect;
			}
		}
		if ( ( ( *m_pSnHeadData )->Sn_EXP100 & 0x01 ) && *( m_pShinInfo->pSgSpc ) && ( Fhyo2_11[cnt].Index == ID211_8EX ) ) {
			Flg = 0;
		}

		if ( m_SpcSgn ) {
			//if ( ( Fhyo2_11[cnt].Index == ID211_24D ) || ( Fhyo2_11[cnt].Index == ID211_24E ) ) {
			if ( ( Fhyo2_11[cnt].Index == ID211_23D ) || ( Fhyo2_11[cnt].Index == ID211_23E ) ) {
				Flg = 0;
			}
		}
		if ( ( Fhyo2_11[cnt].Index == ID211_26D ) || ( Fhyo2_11[cnt].Index == ID211_26E ) ) {
			if ( eymd < ICS_SH_KYOJU_INPUT_DAY ) {
				Flg = 0;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo2_11[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo2_11[cnt].Connect );
		cnt++;
	}

	m_Fhyo2_11Diag02.EnableDelete ( 0 );

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
int CShinFhyo2_11::ChkTkkzIdx ( short idx )
{
	int rSt = 0;

	if ( ( idx == ID211_11E ) || ( idx == ID211_12E ) ) {
		rSt = 1;
	}

	return rSt;
}

//============================================================
// 特例計算に関連するインデックス
//------------------------------------------------------------
// 引数
//		idx		:	ダイアグラム内インデックス
//------------------------------------------------------------
// 返送値
//		0		:	特例計算に関連しない
//		1		:	特例計算で入力可能になるインデックス
//============================================================
int CShinFhyo2_11::ChkTokureiIdx ( short idx )
{
	int rSt = 0;

	if ( ( ID211_9D <= idx ) && ( idx <= ID211_13E ) ) {
		rSt = 1;
	}

	return rSt;
}

//=========================================================
// 画面出力
//=========================================================
void CShinFhyo2_11::DispFh2_11()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 4; i <= 6; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	for( int i = ID211_1D; i <= ID211_25F; i++ ){
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		//手入力サイン初期化
		F211_10c = 0;
		F211_19c = 0;
		F211_20c = 0;
		F211_23c = 0;
	}

	// 期間の表示
	char	buf[512] = { 0 };
	char	yy, mm, dd;
	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 4, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 5, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 氏名の表示
	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( *m_pSnHeadData )->IsSoleProprietor() ) {
		memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, sizeof ( ( *m_pSnHeadData )->Sn_DIHYO ) );
	}
	else {
		memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, sizeof ( ( *m_pSnHeadData )->Sn_CONAM ) );
	}
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 6, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 課税売上高の5億円超えチェック
	ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION, 1 );
	ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION + 10, 1 );
	diag_clear ( IDC_ICSDIAG8CTRL1, 4, TRUE, CIcsdiagctrl );
	CheckOver500Million();

	// 全項目表示
	f211data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo2_11::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo2_11::dsp_cls ( unsigned short Id, short Index )
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
void CShinFhyo2_11::CheckOver500Million()
{
	DIAGRAM_ATTRIBUTE DA;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	COLORREF color;

	char	VAL[MONY_BUF_SIZE] ={ 0 };
	char	ValCnv[MONY_BUF_SIZE] ={ 0 };
	( *m_pSnHeadData )->Sn_Sign4 &= 0xef;
	CheckOver500MillionSub( *( m_pShinInfo->pOver500MillionSw ), VAL, ValCnv );

	char	buf[256] ={ 0 };
	if ( *m_pShinInfo->pOver500MillionSw == 1 ) {	// ON:９５％ルール改正対応期間で５億円超え
		( *m_pSnHeadData )->m_UriRatioBorder = 11000;
		( *m_pSnHeadData )->Sn_Sign4 |= 0x10;
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION, 5 );
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION + 10, 5 );
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
			diag_setdata ( IDC_ICSDIAG8CTRL1, 4, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}
	}
	else {			// ４億５千万以上で５億以下又は４億５千万未満
		( *m_pSnHeadData )->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION, 1 );
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION + 10, 1 );
			color = BC_WHITE;
			diag_clear ( IDC_ICSDIAG8CTRL1, 4, TRUE, CIcsdiagctrl );
		}
	}

	// m_Fhyo2_11Diag02.Refresh()を呼ぶと解像度が1024×768のときに正常に表示されなくなる
	if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
		diag_getattr ( IDC_ICSDIAG8CTRL2, MILLION, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL2, MILLION, &DA, TRUE, CIcsdiagctrl );

		diag_getattr ( IDC_ICSDIAG8CTRL2, MILLION + 10, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL2, MILLION + 10, &DA, TRUE, CIcsdiagctrl );
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
int CShinFhyo2_11::CheckOver500MillionSub ( int &MilSw, char *pVAL, char *pValCnv )
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
				if ( money[l][m].code == _T( "322601" ) ) {
					memmove ( F211_1X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322602" ) ) {
					memmove ( F211_1D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322603" ) ) {
					memmove ( F211_1E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322700" ) ) {
					memmove ( F211_2F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "323300" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "SSSSSSSSSSSSS9" ) );
					F211_8EX = atoi ( wariai );
				}
			}
		}
	}

	m_pArith->l_add ( tmpVAL, F211_1X, F211_1D );
	m_pArith->l_add ( tmpVAL, tmpVAL, F211_1E );
	m_pArith->l_add ( tmpVAL, tmpVAL, F211_2F );
	int sw = ( *m_pSnHeadData )->CheckTaxationSales ( tmpVAL, tmpValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );

	if ( sw == 1 ) {
		m_pArith->l_input ( flg, _T( "0" ) );
		nflg = 0;
	}
	else {
		if ( F211_8EX >= 9500 ) {
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
void CShinFhyo2_11::f211data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f211data_1();

	case 2:
		f211data_2();

	case 3:
		f211data_3();

	case 4:
	case 5:
		f211data_4_5();

	case 6:	
		f211data_6();

	case 7:
		f211data_7();

	case 8:
		f211data_8();
		Changef211data_8();

	case 9:
		f211data_9();

	case 10:
		f211data_10();

	case 11:
		f211data_11();

	case 12:
		f211data_12();

	case 13:
		f211data_13();

	case 14:
		f211data_14();

	case 15:
		f211data_15();

	case 16:
		f211data_16();

	case 17:
		f211data_17();

	case 18:
		f211data_18();

	case 19:
		f211data_19();

	case 20:
		f211data_20();

	case 21:
		f211data_21();

	case 22:
		f211data_22();

	case 23:
		f211data_26();

	case 24:
		f211data_23();

	case 25:
		f211data_24();

	case 26:
		f211data_25();

		break;
	default:
		break;
	}
}

//=========================================================
// 課税売上(税抜き)表示
//=========================================================
void CShinFhyo2_11::f211data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_1X, F211_1X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_1D, F211_1D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_1E, F211_1E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_1F, F211_1F );
}

//=========================================================
// 免税売上額
//=========================================================
void CShinFhyo2_11::f211data_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_2F, F211_2F );
}

//=========================================================
// 非課税資産の輸出等の金額、海外支店等へ移送した資産の価額
//=========================================================
void CShinFhyo2_11::f211data_3()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_3F, F211_3F );
}

//=========================================================
// 課税資産の譲渡等の対価の額
//=========================================================
void CShinFhyo2_11::f211data_4_5()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_4F, F211_4F );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_5F, F211_5F );
}

//=========================================================
// 非課税売上額
//=========================================================
void CShinFhyo2_11::f211data_6()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_6F, F211_6F );
}

//=========================================================
// 資産の譲渡等の対価の額
//=========================================================
void CShinFhyo2_11::f211data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_7F, F211_7F );
}

//=========================================================
// 課税売上割合
//=========================================================
void CShinFhyo2_11::f211data_8()
{
	char WORK0[MONY_BUF_SIZE] = { 0 };
	char buf[512] = { 0 };

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 課税売上高割合
	DiagData.data_check = 0;
	diag_setdata ( IDC_ICSDIAG8CTRL2, ID211_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8F, 1 );
	diag_modify ( IDC_ICSDIAG8CTRL2, ID211_8F, DIAG_MDFY_READONLY, CIcsdiagctrl );
	memset ( buf, '\0', sizeof ( buf ) );
	memset ( WORK0, '\0', MONY_BUF_SIZE );
	if ( !( ( *m_pSnHeadData )->Sn_EXP100 & 0x01) ) {
		/*if( *m_pShinInfo->pSgSpc ){
			sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), F211_8EX );
			m_pArith->l_input ( WORK0, buf );

			// 特定収入有り
			ChangeColor( IDC_ICSDIAG8CTRL2, ID211_8EX, 1 );
			diag_modify( IDC_ICSDIAG8CTRL2, ID211_8EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
		}*/
		//else{
			if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
				// 課税売上割合　手入力
				DiagData.data_check = 1;
				diag_setdata ( IDC_ICSDIAG8CTRL2, ID211_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW );
				m_pArith->l_input ( WORK0, buf );
				( *m_pSyzSyukei )->SetShinkokuData (_T( "323300" ), WORK0 );

				if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() ) {
					memset ( buf, '\0', sizeof ( buf ) );
					strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)\n【課税売上割合に準ずる割合を適用】" ) );
					DiagData.data_disp = buf;
					diag_setdata ( IDC_ICSDIAG8CTRL2, 78, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8F, 1 );
				}
				else {
					memset ( buf, '\0', sizeof ( buf ) );
					strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)" ) );
					DiagData.data_disp = buf;
					diag_setdata ( IDC_ICSDIAG8CTRL2, 78, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8F, 1 );
				}

				if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8F, 0 );
					diag_modify ( IDC_ICSDIAG8CTRL2, ID211_8F, DIAG_MDFY_EDIT, CIcsdiagctrl );
				}
				else {
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8F, 2 );
				}
			}
			else {
				sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), F211_8EX );
				m_pArith->l_input ( WORK0, buf );

				memset ( buf, '\0', sizeof ( buf ) );
				strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)" ) );
				DiagData.data_disp = buf;
				diag_setdata ( IDC_ICSDIAG8CTRL2, 78, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8F, 1 );
			}
		//}
	}
	else{
		m_pArith->l_input ( WORK0, _T( "10000" ) );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323300" ), WORK0 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_8EX, 1 );
		diag_modify ( IDC_ICSDIAG8CTRL2, ID211_8EX, DIAG_MDFY_READONLY, CIcsdiagctrl );

	}

	memset ( buf, '\0', sizeof ( buf ) );
	m_pArith->l_print ( buf, WORK0, _T( "SSSSS" ) );
	F211_8EX = atoi ( buf );
	m_pArith->l_print ( buf, WORK0, _T( "SS9.99" ) );
	DiagData.data_edit = buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, ID211_8F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );


	// 課税売上割合を無効にするとリセットされてしまうため、別関数にする
/*	// 課税売上割合に準ずる割合を有効
	// 課税売上割合が９５％未満
	if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() || ( F211_8EX < (*m_pSnHeadData)->m_UriRatioBorder ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 )== 0x00 ) ) {
				ModifyDiagInput ( ID211_17D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_17E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17E, 0 );

				ModifyDiagInput ( ID211_18D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_18E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18E, 0 );

				ModifyDiagInput ( ID211_19D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_19E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, ( F211_19c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, ( F211_19c & 0x800 ) ? 3 : 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, 1 );
				ModifyDiagInput ( ID211_20D, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID211_20E, DIAG_MDFY_READONLY );
			}
		}
		else {
			ModifyDiagInput ( ID211_17D, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID211_17E, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17D, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17E, 1 );

			ModifyDiagInput ( ID211_18D, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID211_18E, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18D, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18E, 1 );

			F211_19c = 0;
			ModifyDiagInput ( ID211_19D, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID211_19E, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, 1 );

			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
				ModifyDiagInput ( ID211_20D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_20E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, ( F211_20c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, ( F211_20c & 0x800 ) ? 3 : 0 );
			}
		}
	}
	else {
		char mony[MONY_BUF_SIZE] = { 0 };
		memset ( mony, '\0', MONY_BUF_SIZE) ;

		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321702" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321703" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321802" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321803" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324202" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324203" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324302" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324303" ), mony );

		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321902" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321903" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324402" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324403" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322002" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322003" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324502" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324503" ) );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17E, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18E, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, 1 );

		ModifyDiagInput ( ID211_17D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_17E, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID211_18D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_18E, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID211_19D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_19E, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID211_20D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_20E, DIAG_MDFY_READONLY );
	}
*/
}

//=========================================================
// 課税仕入れに係る支払対価の額(税込み)
//=========================================================
void CShinFhyo2_11::f211data_9()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_9X, F211_9X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_9D, F211_9D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_9E, F211_9E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_9F, F211_9F );
}

//=========================================================
// 課税仕入れに係る消費税額
//=========================================================
void CShinFhyo2_11::f211data_10()
{
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_10D, ( F211_10c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_10E, ( F211_10c & 0x800 ) ? 3 : 0 );
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_10X, F211_10X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_10D, F211_10D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_10E, F211_10E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_10F, F211_10F );
}

//=========================================================
// 特定課税仕入れに係る支払対価の額
//=========================================================
void CShinFhyo2_11::f211data_11()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_11X, F211_11X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_11E, F211_11E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_11F, F211_11F );
}

//=========================================================
// 特定課税仕入れに係る消費税額
//=========================================================
void CShinFhyo2_11::f211data_12()
{
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_12E, ( F211_12c & 0x800 ) ? 3 : 0 );
			}
			else {
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_12E, 1 );
			}
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_12X, F211_12X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_12E, F211_12E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_12F, F211_12F );
}

//=========================================================
// 課税貨物に係る消費税額
//=========================================================
void CShinFhyo2_11::f211data_13()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_13X, F211_13X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_13D, F211_13D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_13E, F211_13E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_13F, F211_13F );
}

//=========================================================
// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_11::f211data_14()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_14X, F211_14X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_14D, F211_14D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_14E, F211_14E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_14F, F211_14F );
}

//=========================================================
// 課税仕入れ等の税額の合計額
//=========================================================
void CShinFhyo2_11::f211data_15()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_15X, F211_15X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_15D, F211_15D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_15E, F211_15E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_15F, F211_15F );
}

//=========================================================
// 課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合
//=========================================================
void CShinFhyo2_11::f211data_16()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_16X, F211_16X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_16D, F211_16D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_16E, F211_16E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_16F, F211_16F );
}

//=========================================================
// 15のうち、課税売上げにのみ要するもの
//=========================================================
void CShinFhyo2_11::f211data_17()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_17X, F211_17X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_17D, F211_17D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_17E, F211_17E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_17F, F211_17F );
}

//=========================================================
// 15のうち、課税売上げと非課税売上げに共通して要するもの
//=========================================================
void CShinFhyo2_11::f211data_18()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_18X, F211_18X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_18D, F211_18D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_18E, F211_18E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_18F, F211_18F );
}

//=========================================================
// 個別対応方式により控除する課税仕入れ等の税額
//=========================================================
void CShinFhyo2_11::f211data_19()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_19X, F211_19X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_19D, F211_19D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_19E, F211_19E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_19F, F211_19F );
}

//=========================================================
// 一括比例配分方式により控除する課税仕入れ等の税額
//=========================================================
void CShinFhyo2_11::f211data_20()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_20X, F211_20X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_20D, F211_20D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_20E, F211_20E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_20F, F211_20F );
}

//=========================================================
// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_11::f211data_21()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_21X, F211_21X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_21D, F211_21D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_21E, F211_21E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_21F, F211_21F );
}

//=========================================================
// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_11::f211data_22()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_22X, F211_22X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_22D, F211_22D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_22E, F211_22E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_22F, F211_22F );
}

//=========================================================
// 控除対象仕入税額がプラスの時
//=========================================================
void CShinFhyo2_11::f211data_23()
{
	if ( ( ( *m_pSnHeadData)->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) && ( m_SpcSgn == FALSE ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_23D, ( F211_23c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_23E, ( F211_23c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_23X, F211_23X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_23D, F211_23D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_23E, F211_23E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_23F, F211_23F );
}

//=========================================================
// 控除対象仕入税額がマイナスの時
//=========================================================
void CShinFhyo2_11::f211data_24()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_24X, F211_24X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_24D, F211_24D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_24E, F211_24E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_24F, F211_24F );
}

//=========================================================
// 貸倒回収に係る消費税額
//=========================================================
void CShinFhyo2_11::f211data_25()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_25X, F211_25X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_25D, F211_25D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_25E, F211_25E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_25F, F211_25F );
}

//=========================================================
// 居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
//=========================================================
void CShinFhyo2_11::f211data_26()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_26X, F211_26X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_26D, F211_26D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_26E, F211_26E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID211_26F, F211_26F );
}

//=========================================================
// 課税売上割合を変更
//=========================================================
void CShinFhyo2_11::Changef211data_8()
{
	// 課税売上割合に準ずる割合を有効
	// 課税売上割合が９５％未満
	if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() || ( F211_8EX < (*m_pSnHeadData)->m_UriRatioBorder ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 )== 0x00 ) ) {
				ModifyDiagInput ( ID211_17D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_17E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17E, 0 );

				ModifyDiagInput ( ID211_18D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_18E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18E, 0 );

				ModifyDiagInput ( ID211_19D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_19E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, ( F211_19c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, ( F211_19c & 0x800 ) ? 3 : 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, 1 );
				ModifyDiagInput ( ID211_20D, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID211_20E, DIAG_MDFY_READONLY );

				F211_20c = 0;
			}
		}
		else {
			ModifyDiagInput ( ID211_17D, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID211_17E, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17D, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17E, 1 );

			ModifyDiagInput ( ID211_18D, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID211_18E, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18D, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18E, 1 );

			F211_19c = 0;
			ModifyDiagInput ( ID211_19D, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID211_19E, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, 1 );

			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
				ModifyDiagInput ( ID211_20D, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID211_20E, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, ( F211_20c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, ( F211_20c & 0x800 ) ? 3 : 0 );
			}
		}
	}
	else {
		char mony[MONY_BUF_SIZE] = { 0 };
		memset ( mony, '\0', MONY_BUF_SIZE) ;

		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321701" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321702" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321703" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321801" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321802" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321803" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324202" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324203" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324302" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324303" ), mony );

		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321901" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321902" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321903" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324402" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324403" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322001" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322002" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322003" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324502" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324503" ) );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_17E, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_18E, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_19E, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20D, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID211_20E, 1 );

		ModifyDiagInput ( ID211_17D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_17E, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID211_18D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_18E, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID211_19D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_19E, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID211_20D, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID211_20E, DIAG_MDFY_READONLY );

		F211_19c = 0;
		F211_20c = 0;
	}
}

//=========================================================
// 棚卸データの加減算を行う
//=========================================================
void CShinFhyo2_11::TanaDataAdd()
{
	char	VAL_14D[6] = { 0 }, VAL_14E[6] = { 0 }, VAL_17D[6] = { 0 }, VAL_17E[6] = { 0 }, VAL_18D[6] = { 0 }, VAL_18E[6] = { 0 };

	char	VAL_14A[6] = { 0 }, VAL_14B[6] = { 0 }, VAL_14C[6] = { 0 };
	char	VAL_17A[6] = { 0 }, VAL_17B[6] = { 0 }, VAL_17C[6] = { 0 };
	char	VAL_18A[6] = { 0 }, VAL_18B[6] = { 0 }, VAL_18C[6] = { 0 };

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動は保存データで使用するので、再計算不要
		return;
	}
	else {
		// 連動時でも特例計算(5-(3)号様式)を摘要する場合は再計算しない
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			return;
		}
	}

	if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01 ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) {
			MoneyBasejagArray	money;
			char wariai[32] = { 0 };

			// 連動データを取得
			money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "323300" ) );
			m_pArith->l_print ( wariai, money[0][0].arith, _T( "SSSSSSSSSSSSS9" ) );
			F211_8EX = atoi ( wariai );
			CheckOver500Million();

			if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
				m_pArith->l_add ( VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[0], ( *m_pSnTanaData )->Sn_mnz78[1] );
				m_pArith->l_add ( VAL_14E, VAL_14E, ( *m_pSnTanaData )->Sn_mnz78[2]);
				m_pArith->l_add ( VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[0], ( *m_pSnTanaData )->Sn_mnz624[1] );
				m_pArith->l_add ( VAL_14D, VAL_14D, ( *m_pSnTanaData )->Sn_mnz624[2] );

				m_pArith->l_add ( VAL_14C, ( *m_pSnTanaData)->Sn_mnz63[0], ( *m_pSnTanaData )->Sn_mnz63[1] );
				m_pArith->l_add ( VAL_14C, VAL_14C, ( *m_pSnTanaData )->Sn_mnz63[2] );
				m_pArith->l_add ( VAL_14B, ( *m_pSnTanaData)->Sn_mnz40[0], ( *m_pSnTanaData )->Sn_mnz40[1] );
				m_pArith->l_add ( VAL_14B, VAL_14B, ( *m_pSnTanaData )->Sn_mnz40[2] );
				m_pArith->l_add ( VAL_14A, ( *m_pSnTanaData)->Sn_menzz[0], ( *m_pSnTanaData )->Sn_menzz[1] );
				m_pArith->l_add ( VAL_14A, VAL_14A, ( *m_pSnTanaData )->Sn_menzz[2] );

				// 旧格納場所へ格納
				memmove ( ( *m_pSnTanaData )->Sn_MNZ78, VAL_14E, MONY_BUF_SIZE );
				memmove ( ( *m_pSnTanaData )->Sn_MNZ624, VAL_14D, MONY_BUF_SIZE );

				memmove ( ( *m_pSnTanaData )->Sn_MNZ63, VAL_14C, MONY_BUF_SIZE );
				memmove ( ( *m_pSnTanaData )->Sn_MNZ40, VAL_14B, MONY_BUF_SIZE );
				memmove ( ( *m_pSnTanaData )->Sn_MENZZ, VAL_14A, MONY_BUF_SIZE );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( VAL_14D );
					m_pArith->l_neg ( VAL_14E );

					m_pArith->l_neg ( VAL_14A );
					m_pArith->l_neg ( VAL_14B );
					m_pArith->l_neg ( VAL_14C );
				}

				// 個別対応で95％未満の場合
				if ( ( F211_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
					memmove ( VAL_17E, ( *m_pSnTanaData )->Sn_mnz78[0], MONY_BUF_SIZE );
					memmove ( VAL_18E, ( *m_pSnTanaData )->Sn_mnz78[2], MONY_BUF_SIZE );
					memmove ( VAL_17D, ( *m_pSnTanaData )->Sn_mnz624[0], MONY_BUF_SIZE );
					memmove ( VAL_18D, ( *m_pSnTanaData )->Sn_mnz624[2], MONY_BUF_SIZE );

					memmove ( VAL_17C, ( *m_pSnTanaData )->Sn_mnz63[0], MONY_BUF_SIZE );
					memmove ( VAL_18C, ( *m_pSnTanaData )->Sn_mnz63[2], MONY_BUF_SIZE );
					memmove ( VAL_17B, ( *m_pSnTanaData )->Sn_mnz40[0], MONY_BUF_SIZE );
					memmove ( VAL_18B, ( *m_pSnTanaData )->Sn_mnz40[2], MONY_BUF_SIZE );
					memmove ( VAL_17A, ( *m_pSnTanaData )->Sn_menzz[0], MONY_BUF_SIZE );
					memmove ( VAL_18A, ( *m_pSnTanaData )->Sn_menzz[2], MONY_BUF_SIZE );

					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( VAL_17E );
						m_pArith->l_neg ( VAL_18E );
						m_pArith->l_neg ( VAL_17D );
						m_pArith->l_neg ( VAL_18D );

						m_pArith->l_neg ( VAL_17C );
						m_pArith->l_neg ( VAL_18C );
						m_pArith->l_neg ( VAL_17B );
						m_pArith->l_neg ( VAL_18B );
						m_pArith->l_neg ( VAL_17A );
						m_pArith->l_neg ( VAL_18A );
					}
				}
			}
		}
		else {
			if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
				memmove ( VAL_14D, ( *m_pSnTanaData )->Sn_MNZ624, MONY_BUF_SIZE );
				memmove ( VAL_14E, ( *m_pSnTanaData )->Sn_MNZ78, MONY_BUF_SIZE );

				memmove ( VAL_14A, ( *m_pSnTanaData )->Sn_MENZZ, MONY_BUF_SIZE );
				memmove ( VAL_14B, ( *m_pSnTanaData )->Sn_MNZ40, MONY_BUF_SIZE );
				memmove ( VAL_14C, ( *m_pSnTanaData )->Sn_MNZ63, MONY_BUF_SIZE );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( VAL_14D );
					m_pArith->l_neg ( VAL_14E );

					m_pArith->l_neg ( VAL_14A );
					m_pArith->l_neg ( VAL_14B );
					m_pArith->l_neg ( VAL_14C );
				}
			}
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), VAL_14D );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), VAL_14E );

		if ( ( *m_pSnHeadData )->SVmzsw & 0x01 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), VAL_14A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), VAL_14B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), VAL_14C );
		}
	}

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A602" ), VAL_17D );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A603" ), VAL_17E );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A702" ), VAL_18D );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A703" ), VAL_18E );

	if ( ( *m_pSnHeadData )->SVmzsw & 0x01 ) {
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A201" ), VAL_17A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A202" ), VAL_17B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A203" ), VAL_17C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A301" ), VAL_18A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A302" ), VAL_18B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A303" ), VAL_18C );
	}
}

//=========================================================
// 表示金額取得
//=========================================================
void CShinFhyo2_11::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1.課税売上額(税抜き)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_1X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_1X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_1D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_1D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_1E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_1E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_1F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_1F, DiagData.data_val, MONY_BUF_SIZE );

	// 2.免税売上額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_2F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_2F, DiagData.data_val, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額、海外支店等へ移送した資産の価額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_3F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_3F, DiagData.data_val, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_4F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_4F, DiagData.data_val, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_5F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_5F, DiagData.data_val, MONY_BUF_SIZE );

	// 6.非課税売上額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_6F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_6F, DiagData.data_val, MONY_BUF_SIZE );

	// 7.資産譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_7F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_7F, DiagData.data_val, MONY_BUF_SIZE );

	// 9.課税仕入れに係る支払い対価の額(税込み)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_9X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_9X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_9D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_9D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_9E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_9E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_9F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_9F, DiagData.data_val, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_10X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_10X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_10D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_10D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_10E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_10E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_10F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_10F, DiagData.data_val, MONY_BUF_SIZE );

	// 11.特定課税仕入れに係る支払対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_11X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_11X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_11E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_11E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_11F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_11F, DiagData.data_val, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_12X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_12X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_12E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_12E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_12F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_12F, DiagData.data_val, MONY_BUF_SIZE );

	// 13.課税貨物に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_13X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_13X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_13D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_13D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_13E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_13E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_13F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_13F, DiagData.data_val, MONY_BUF_SIZE );

	// 14.納税義務の免除を受ける(受けない)こととなった場合における消費税額の調整額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_14X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_14X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_14D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_14D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_14E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_14E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_14F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_14F, DiagData.data_val, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_15X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_15X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_15D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_15D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_15E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_15E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_15F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_15F, DiagData.data_val, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、割合が95％以上の場合
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_16X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_16X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_16D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_16D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_16E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_16E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_16F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_16F, DiagData.data_val, MONY_BUF_SIZE );

	// 17.15のうち、課税売上げにのみ要するもの
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_17X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_17X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_17D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_17D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_17E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_17E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_17F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_17F, DiagData.data_val, MONY_BUF_SIZE );

	// 18.15のうち、共通して要するもの
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_18X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_18X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_18D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_18D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_18E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_18E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_18F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_18F, DiagData.data_val, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_19X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_19X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_19D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_19D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_19E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_19E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_19F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_19F, DiagData.data_val, MONY_BUF_SIZE );

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_20X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_20X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_20D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_20D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_20E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_20E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_20F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_20F, DiagData.data_val, MONY_BUF_SIZE );

	// 21.課税売上割合変動時の…消費税額の調整額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_21X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_21X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_21D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_21D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_21E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_21E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_21F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_21F, DiagData.data_val, MONY_BUF_SIZE );

	// 22.調整対象固定資産を…調整額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_22X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_22X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_22D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_22D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_22E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_22E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_22F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_22F, DiagData.data_val, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_26X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_26X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_26D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_26D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_26E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_26E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_26F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_26F, DiagData.data_val, MONY_BUF_SIZE );

	// 24.控除対象仕入税額がプラスの時
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_23X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_23X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_23D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_23D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_23E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_23E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_23F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_23F, DiagData.data_val, MONY_BUF_SIZE );

	// 25.控除過大調整額がマイナスの時
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_24X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_24X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_24D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_24D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_24E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_24E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_24F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_24F, DiagData.data_val, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_25X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_25X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_25D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_25D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_25E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_25E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID211_25F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F211_25F, DiagData.data_val, MONY_BUF_SIZE );
}

//=========================================================
// 課税売上割合の取得
//---------------------------------------------------------
// 返送値
//		計算した課税売上割合
//=========================================================
unsigned short CShinFhyo2_11::GetCalqedKzwari()
{
	unsigned short	rtKzwari = F211_8EX;

	if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
		// 課税売上割合を変更

		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x02 ) {
			// 課税売上割合に準ずる割合を使用
			// 計算
			char WORK0[MONY_BUF_SIZE] = { 0 };
			char PW6[MONY_BUF_SIZE]   = { 0 };
			char PER100BUF[MONY_BUF_SIZE] = { 0 };

			m_pArith->l_input ( PER100BUF, _T( "10000" ) );
			m_pArith->l_par100 ( ( unsigned char* )WORK0, ( unsigned char* )F211_4F, ( unsigned char* )F211_7F, 2, 0 );
			if ( m_pArith->l_cmp ( F211_4F, PW6 ) < 0 ) {
				memset ( WORK0, '\0', MONY_BUF_SIZE );
			}
			if ( m_pArith->l_cmp ( F211_7F, PW6 ) < 0 ) {
				memset ( WORK0, '\0', MONY_BUF_SIZE );
			}
			if ( m_pArith->l_cmp ( WORK0, PER100BUF ) > 0 ) {
				memmove ( WORK0, PER100BUF, sizeof ( WORK0 ) );
			}
			char buf[64] = { 0 };
			m_pArith->l_print ( buf, WORK0, _T( "SSSSS" ) );
			rtKzwari = atoi ( buf );
		}
	}

	return rtKzwari;
}


//=========================================================
// ボタン表示変更
//=========================================================
void CShinFhyo2_11::ChangeBtnDisp()
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
void CShinFhyo2_11::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( (index = m_Fhyo2_11Diag02.GetPosition()) != -1 ) {
			m_Fhyo2_11Diag02.SetPosition ( index );
			m_Fhyo2_11Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo2_11Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Fhyo2_11Diag02.SetFocus();

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) ) {
			// 非課税売上額
			m_Fhyo2_11Diag02.SetPosition ( ID211_2F );
		}
		else {
			// 課税売上額
			m_Fhyo2_11Diag02.SetPosition( ID211_1D );
		}
	}
}

//=========================================================
// フォーカス移動先取得
//---------------------------------------------------------
// 引数
//			Index		：	ダイアグラム内インデックス
//			Move		：	0 = 下方向, 1 = 上方向
//=========================================================
int CShinFhyo2_11::UpDownFocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY2_01CNT; i++ ) {
		if ( Index == FCS_Fhyo2_11[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo2_11[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY2_01CNT; j++ ) {
			m_Fhyo2_11Diag02.GetAttribute ( FCS_Fhyo2_11[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_11[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo2_11Diag02.GetAttribute ( FCS_Fhyo2_11[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_11[j].IDNo;
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
void CShinFhyo2_11::ModifyDiagInput ( short index, int sign )
{
	diag_modify( IDC_ICSDIAG8CTRL2, index, sign, CIcsdiagctrl );
}


//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinFhyo2_11::ChangeColor ( unsigned short id, short Index, int sign )
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
void CShinFhyo2_11::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {

		// 前回の情報をセーブ
		char			SV_mtype = ( *m_pSnTanaData )->Sn_MENZsw;
		unsigned short	SV_F211_8E = F211_8EX;
		char			SV_mnk624[3][MONY_BUF_SIZE];
		char			SV_mnz624[3][MONY_BUF_SIZE];
		char			SV_mnk78[3][MONY_BUF_SIZE];
		char			SV_mnz78[3][MONY_BUF_SIZE];
		char			zerobuf[MONY_BUF_SIZE];

		char			SV_mnk40[3][MONY_BUF_SIZE];
		char			SV_mnz40[3][MONY_BUF_SIZE];
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];
		char			SV_menzk[3][MONY_BUF_SIZE];
		char			SV_menzz[3][MONY_BUF_SIZE];
		char			VAL_14A[MONY_BUF_SIZE];
		char			VAL_14B[MONY_BUF_SIZE];
		char			VAL_14C[MONY_BUF_SIZE];
		char			VAL_17A[MONY_BUF_SIZE];
		char			VAL_17B[MONY_BUF_SIZE];
		char			VAL_17C[MONY_BUF_SIZE];
		char			VAL_18A[MONY_BUF_SIZE];
		char			VAL_18B[MONY_BUF_SIZE];
		char			VAL_18C[MONY_BUF_SIZE];

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

		if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
			memmove ( SV_mnk63[0], ( *m_pSnTanaData )->Sn_mnk63[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
			memmove ( SV_mnz63[0], ( *m_pSnTanaData )->Sn_mnz63[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove ( SV_mnk63[1], ( *m_pSnTanaData )->Sn_mnk63[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
			memmove ( SV_mnz63[1], ( *m_pSnTanaData )->Sn_mnz63[1], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove ( SV_mnk63[2], ( *m_pSnTanaData )->Sn_mnk63[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
			memmove ( SV_mnz63[2], ( *m_pSnTanaData )->Sn_mnz63[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃

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

		// 個別対応
		CChangeTaxEx2		CTDiag;
		CTDiag.InitInfo ( ( *m_pSnFhyo2_01Data ), ( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );

		HINSTANCE	svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle ( g_hInstance );
		int st = ( int )CTDiag.DoModal();
		AfxSetResourceHandle ( svhInstResource );

		if (st == IDOK) {
			// 手入力
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動
				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					MoneyBasejagArray	money;

					// 付表２－２のデータを取得
					money =  (*m_pSyzSyukei )->GetShinkokuData ( _T( "321701" ) );
					memmove ( VAL_17A, money[0][0].arith, MONY_BUF_SIZE);
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "321702" ) );
					memmove ( VAL_17B, money[0][0].arith, MONY_BUF_SIZE);
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "321703" ) );
					memmove ( VAL_17C, money[0][0].arith, MONY_BUF_SIZE);
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "321801" ) );
					memmove ( VAL_18A, money[0][0].arith, MONY_BUF_SIZE);
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "321802" ) );
					memmove ( VAL_18B, money[0][0].arith, MONY_BUF_SIZE);
					money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "321803" ) );
					memmove ( VAL_18C, money[0][0].arith, MONY_BUF_SIZE);
				}

				// 前回が個別対応で95％未満の場合、前回分を削除
				if ( ( SV_mtype & 0x10 ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
					if (SV_mtype & 0x01) {
						m_pArith->l_add ( F211_17E, F211_17E, SV_mnz78[0] );		// 課税売上に係る仕入税額
						m_pArith->l_add ( F211_18E, F211_18E, SV_mnz78[2] );		// 共通の売上に係る仕入税額
						m_pArith->l_add ( F211_17D, F211_17D, SV_mnz624[0] );		// 課税売上に係る仕入税額
						m_pArith->l_add ( F211_18D, F211_18D, SV_mnz624[2] );		// 共通の売上に係る仕入税額
						if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
							m_pArith->l_add ( VAL_17C, VAL_17C, SV_mnz63[0] );		// 課税売上に係る仕入税額
							m_pArith->l_add ( VAL_18C, VAL_18C, SV_mnz63[2] );		// 共通の売上に係る仕入税額
							m_pArith->l_add ( VAL_17B, VAL_17B, SV_mnz40[0] );		// 課税売上に係る仕入税額
							m_pArith->l_add ( VAL_18B, VAL_18B, SV_mnz40[2] );		// 共通の売上に係る仕入税額
							m_pArith->l_add ( VAL_17A, VAL_17A, SV_menzz[0] );		// 課税売上に係る仕入税額
							m_pArith->l_add ( VAL_18A, VAL_18A, SV_menzz[2] );		// 共通の売上に係る仕入税額
						}
					}
					else {
						m_pArith->l_sub ( F211_17E, F211_17E, SV_mnz78[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub ( F211_18E, F211_18E, SV_mnz78[2] );		// 16. 共通の売上に係る仕入税額
						m_pArith->l_sub ( F211_17D, F211_17D, SV_mnz624[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub ( F211_18D, F211_18D, SV_mnz624[2] );		// 16. 共通の売上に係る仕入税額
						if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
							m_pArith->l_sub ( VAL_17C, VAL_17C, SV_mnz63[0] );		// 課税売上に係る仕入税額
							m_pArith->l_sub ( VAL_18C, VAL_18C, SV_mnz63[2] ) ;		// 共通の売上に係る仕入税額
							m_pArith->l_sub ( VAL_17B, VAL_17B, SV_mnz40[0] );		// 課税売上に係る仕入税額
							m_pArith->l_sub ( VAL_18B, VAL_18B, SV_mnz40[2] );		// 共通の売上に係る仕入税額
							m_pArith->l_sub ( VAL_17A, VAL_17A, SV_menzz[0] );		// 課税売上に係る仕入税額
							m_pArith->l_sub ( VAL_18A, VAL_18A, SV_menzz[2] );		// 共通の売上に係る仕入税額
						}
					}
				}

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
					memset ( F211_14E, '\0', MONY_BUF_SIZE );
					m_pArith->l_add ( F211_14E, F211_14E, ( *m_pSnTanaData )->Sn_mnz78[0] );
					m_pArith->l_add ( F211_14E, F211_14E, ( *m_pSnTanaData )->Sn_mnz78[1] );
					m_pArith->l_add ( F211_14E, F211_14E, ( *m_pSnTanaData )->Sn_mnz78[2] );
					memset(F211_14D, '\0', MONY_BUF_SIZE);
					m_pArith->l_add ( F211_14D, F211_14D, ( *m_pSnTanaData )->Sn_mnz624[0] );
					m_pArith->l_add ( F211_14D, F211_14D, ( *m_pSnTanaData )->Sn_mnz624[1] );
					m_pArith->l_add ( F211_14D, F211_14D, ( *m_pSnTanaData )->Sn_mnz624[2] );

					memset ( VAL_14C, '\0', MONY_BUF_SIZE );
					memset ( VAL_14B, '\0', MONY_BUF_SIZE );
					memset ( VAL_14A, '\0', MONY_BUF_SIZE );
					if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
						m_pArith->l_add ( VAL_14C, VAL_14C, ( *m_pSnTanaData )->Sn_mnz63[0] );
						m_pArith->l_add ( VAL_14C, VAL_14C, ( *m_pSnTanaData )->Sn_mnz63[1] );
						m_pArith->l_add ( VAL_14C, VAL_14C, ( *m_pSnTanaData )->Sn_mnz63[2] );
						m_pArith->l_add ( VAL_14B, VAL_14B, ( *m_pSnTanaData )->Sn_mnz40[0] );
						m_pArith->l_add ( VAL_14B, VAL_14B, ( *m_pSnTanaData )->Sn_mnz40[1] );
						m_pArith->l_add ( VAL_14B, VAL_14B, ( *m_pSnTanaData )->Sn_mnz40[2] );
						m_pArith->l_add ( VAL_14A, VAL_14A, ( *m_pSnTanaData )->Sn_menzz[0] );
						m_pArith->l_add ( VAL_14A, VAL_14A, ( *m_pSnTanaData )->Sn_menzz[1] );
						m_pArith->l_add ( VAL_14A, VAL_14A, ( *m_pSnTanaData )->Sn_menzz[2] );
					}

					// 旧格納場所へ格納
					memmove ( ( *m_pSnTanaData )->Sn_MNZ78, F211_14E, MONY_BUF_SIZE );
					memmove ( ( *m_pSnTanaData )->Sn_MNZ624, F211_14D, MONY_BUF_SIZE );
					if ((*m_pSnHeadData)->SVmzsw == 1) {
						memmove ( ( *m_pSnTanaData )->Sn_MNZ63, VAL_14C, MONY_BUF_SIZE );
						memmove ( ( *m_pSnTanaData )->Sn_MNZ40, VAL_14B, MONY_BUF_SIZE );
						memmove ( ( *m_pSnTanaData )->Sn_MENZZ, VAL_14A, MONY_BUF_SIZE );
					}

					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( F211_14X );
						m_pArith->l_neg ( F211_14D );
						m_pArith->l_neg ( F211_14E );
						if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
							m_pArith->l_neg ( VAL_14A );
							m_pArith->l_neg ( VAL_14B );
							m_pArith->l_neg ( VAL_14C );
						}
					}
					// 個別対応で95％未満の場合
					if ( ( F211_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
						if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
							m_pArith->l_sub ( F211_17E, F211_17E, ( *m_pSnTanaData )->Sn_mnz78[0]);
							m_pArith->l_sub ( F211_18E, F211_18E, ( *m_pSnTanaData )->Sn_mnz78[2] );
							m_pArith->l_sub ( F211_17D, F211_17D, ( *m_pSnTanaData )->Sn_mnz624[0] );
							m_pArith->l_sub ( F211_18D, F211_18D, ( *m_pSnTanaData )->Sn_mnz624[2] );
							if ((*m_pSnHeadData)->SVmzsw == 1) {
								m_pArith->l_sub ( VAL_17C, VAL_17C, ( *m_pSnTanaData )->Sn_mnz63[0] );		// 課税売上に係る仕入税額
								m_pArith->l_sub ( VAL_18C, VAL_18C, ( *m_pSnTanaData )->Sn_mnz63[2] );		// 共通の売上に係る仕入税額
								m_pArith->l_sub ( VAL_17B, VAL_17B, ( *m_pSnTanaData )->Sn_mnz40[0] );		// 課税売上に係る仕入税額
								m_pArith->l_sub ( VAL_18B, VAL_18B, ( *m_pSnTanaData )->Sn_mnz40[2] );		// 共通の売上に係る仕入税額
								m_pArith->l_sub ( VAL_17A, VAL_17A, ( *m_pSnTanaData )->Sn_menzz[0] );		// 課税売上に係る仕入税額
								m_pArith->l_sub ( VAL_18A, VAL_18A, ( *m_pSnTanaData )->Sn_menzz[2] );		// 共通の売上に係る仕入税額
							}
						}
						else {
							m_pArith->l_add ( F211_17E, F211_17E, ( *m_pSnTanaData )->Sn_mnz78[0] );
							m_pArith->l_add ( F211_18E, F211_18E, ( *m_pSnTanaData )->Sn_mnz78[2] );
							m_pArith->l_add ( F211_17D, F211_17D, ( *m_pSnTanaData )->Sn_mnz624[0] );
							m_pArith->l_add ( F211_18D, F211_18D, ( *m_pSnTanaData )->Sn_mnz624[2] );
							if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
								m_pArith->l_add ( VAL_17C, VAL_17C, ( *m_pSnTanaData )->Sn_mnz63[0] );		// 課税売上に係る仕入税額
								m_pArith->l_add ( VAL_18C, VAL_18C, ( *m_pSnTanaData )->Sn_mnz63[2] );		// 共通の売上に係る仕入税額
								m_pArith->l_add ( VAL_17B, VAL_17B, ( *m_pSnTanaData )->Sn_mnz40[0] );		// 課税売上に係る仕入税額
								m_pArith->l_add ( VAL_18B, VAL_18B, ( *m_pSnTanaData )->Sn_mnz40[2] );		// 共通の売上に係る仕入税額
								m_pArith->l_add ( VAL_17A, VAL_17A, ( *m_pSnTanaData )->Sn_menzz[0] );		// 課税売上に係る仕入税額
								m_pArith->l_add ( VAL_18A, VAL_18A, ( *m_pSnTanaData )->Sn_menzz[2] );		// 共通の売上に係る仕入税額
							}
						}
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F211_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F211_14E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F211_17D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F211_17E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F211_18D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F211_18E );

					if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
						// 付表２－２の金額を更新
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), VAL_14A );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), VAL_14B );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), VAL_14C );
						if ( ( F211_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), VAL_17A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), VAL_17B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), VAL_17C );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), VAL_18A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), VAL_18B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), VAL_18C );
						}
					}
				}
				else {
					memset ( F211_14X, '\0', MONY_BUF_SIZE );
					memset ( F211_14D, '\0', MONY_BUF_SIZE );
					memset ( F211_14E, '\0', MONY_BUF_SIZE );

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F211_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F211_14E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F211_17D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F211_17E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F211_18D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F211_18E );

					if ((*m_pSnHeadData)->SVmzsw == 1) {
						// 付表２－２の金額を更新
						memset(zerobuf, '\0', MONY_BUF_SIZE);
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), zerobuf );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), zerobuf );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), zerobuf );
						if ( ( F211_8EX < ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), VAL_17A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), VAL_17B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), VAL_17C );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), VAL_18A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), VAL_18B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), VAL_18C );
						}
					}

				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_11();
				SetItemPosition();
			}
			else if (!((*m_pSnTanaData)->Sn_tansw & 0x01)) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_11();
				SetItemPosition();
			}
		}

	}
	else {
		CChangeTax2	CTDiag;
		CTDiag.InitInfo ( ( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );

		// 比例配分
		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		int st = ( int )CTDiag.DoModal();
		AfxSetResourceHandle ( svhInstResource );

		if ( st == IDOK ) {

			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動

				// (免税)控除対象調整税額
				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10) {
					memmove ( F211_14D, ( *m_pSnTanaData )->Sn_MNZ624, MONY_BUF_SIZE );
					memmove ( F211_14E, ( *m_pSnTanaData )->Sn_MNZ78, MONY_BUF_SIZE );
					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( F211_14D );
						m_pArith->l_neg ( F211_14E );
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F211_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F211_14E );
				}
				else {
					memset ( F211_14X, '\0', MONY_BUF_SIZE );
					memset ( F211_14D, '\0', MONY_BUF_SIZE );
					memset ( F211_14E, '\0', MONY_BUF_SIZE );

					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
					// 付表２－２もリセット
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_11();
				SetItemPosition();
			}
			else if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01 ) ) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_11();
				SetItemPosition();

			}
		}
	}
}

//=========================================================
// 特定収入の簡易計算式表示
//=========================================================
void CShinFhyo2_11::OnBnClickedButton2()
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
void CShinFhyo2_11::f211calq_spc ( SYZTOSPC_DATA SpcData )
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

		unsigned short uwri = 0;

		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			uwri = F211_8F;
		}
		else {
			uwri = F211_8EX;
		}

		if ( uwri >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();

		// 課税売上セット
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "310202" ) );
		memmove ( SyzToSpc.SpKzur4, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "310203" ) );
		memmove ( SyzToSpc.SpKzur63, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "312200" ) );
		memmove ( SyzToSpc.SpKzur, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "312202" ) );
		memmove ( SyzToSpc.SpKzur624, money[0][0].arith, MONY_BUF_SIZE );

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "312203" ) );
		memmove ( SyzToSpc.SpKzur78, money[0][0].arith, MONY_BUF_SIZE );

		m_pArith->l_add ( SyzToSpc.SpMzur, F211_2F, F211_3F );
		memmove ( SyzToSpc.SpHkur, F211_6F, sizeof ( SyzToSpc.SpHkur ) );

		memmove ( SyzToSpc.SpKuws, F211_4F, MONY_BUF_SIZE );
		memmove ( SyzToSpc.SpKuwb, F211_7F, MONY_BUF_SIZE );

		// 課税売上割合	
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "323300" ) );
		memmove ( SyzToSpc.SpUwri, money[0][0].arith, MONY_BUF_SIZE );

		// 控除対象仕入税額セット
		//if ( IsRatioOver95 ) {
		// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
		if ( IsRatioOver95 && ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) ) {
			// 計算表５（１）
			memmove ( SyzToSpc.SpKgz624, F211_16D, sizeof ( SyzToSpc.SpKgz624 ) );
			memmove ( SyzToSpc.SpKgz78, F211_16E, sizeof ( SyzToSpc.SpKgz78 ) );
		}
		else {
			if ( Kobetsu ) {
				// 計算表５（２）
				memmove ( SyzToSpc.SpKgz624, F211_19D, sizeof ( SyzToSpc.SpKgz624 ) );
				memmove ( SyzToSpc.SpKgz78, F211_19E, sizeof ( SyzToSpc.SpKgz78 ) );
			}
			else {
				// 計算表５（３）
				memmove ( SyzToSpc.SpKgz624, F211_20D, sizeof ( SyzToSpc.SpKgz624 ) );
				memmove ( SyzToSpc.SpKgz78, F211_20E, sizeof ( SyzToSpc.SpKgz78 ) );
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
			memmove ( mony, SyzToSpc.SpSiz624, sizeof ( mony ) );

			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 25D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), mony );
				// 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), zero );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), zero );
			}
			else {
				// 25D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), zero );
				// 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), mony );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), mony );
			}

			memmove ( mony, SyzToSpc.SpSiz78, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 25E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), mony );
				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), zero );
				// 付表1-1 4E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), zero );
			}
			else {
				// 25E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), zero );
				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), mony );
				// 付表1-1 4E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), mony );
			}

			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				memmove ( mony, SyzToSpc.SpSiz4, sizeof ( mony ) );
				if ( m_pArith->l_test ( mony ) < 0 ) {
					// マイナスの場合符号反転
					m_pArith->l_neg ( mony );

					// 付表2-2 25B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), mony );
					// 付表2-2 24B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), zero );
					// 付表1-2 4B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), zero );
				}
				else {
					// 付表2-2 25B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), zero );
					// 付表2-2 24B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), mony );
					// 付表1-2 4B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), mony );
				}

				memmove ( mony, SyzToSpc.SpSiz63, sizeof ( mony ) );
				if ( m_pArith->l_test ( mony ) < 0 ) {
					// マイナスの場合符号反転
					m_pArith->l_neg ( mony );

					// 付表2-2 25C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), mony );
					// 付表2-2 24C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), zero );
					// 付表1-2 4C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), zero );
				}
				else {
					// 付表2-2 25C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), zero );
					// 付表2-2 24C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), mony );
					// 付表1-2 4C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), mony );
				}
			}

			// 特定収入連動メッセージ表示
			m_Fhyo2_11Diag02.ModifyItem ( ID211_TS, DIAG_MDFY_OPAQUE );

			// 簡易計算式ボタン表示
			m_Button2.ShowWindow ( SW_SHOW );

			for ( int index = ID211_23D; index <= ID211_23E; index++ ) {
				ATRIB_MOD ( IDC_ICSDIAG8CTRL2, index, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}
		}
		else {
			// 特定収入割合5%未満
			if ( m_SpcSgn ) {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );	// 23D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );	// 23E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );	// 24D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );	// 24E欄

				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322302" ) );	// 付表2-2 23B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322303" ) );	// 付表2-2 23C欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322402" ) );	// 付表2-2 24B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322403" ) );	// 付表2-2 24C欄
				}

				F211_23c &= ~0x80;
				F211_23c &= ~0x800;

				// 特定収入連動メッセージ非表示
				m_Fhyo2_11Diag02.ModifyItem ( ID211_TS, DIAG_MDFY_TRANSPARENT );

				// 簡易計算式ボタン非表示
				m_Button2.ShowWindow ( SW_HIDE );

				m_SpcSgn = FALSE;

				for ( int index = ID211_23D; index <= ID211_23E; index++ ) {
					ATRIB_MOD ( IDC_ICSDIAG8CTRL2, index, 1, ( *m_pSnHeadData )->Sn_Sign4, 0 );
				}
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄


				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
				}
			}
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "32" ) );
		cnt = ( int )money.GetCount();
		incnt = 0;

		for ( int l = 0; l < cnt; l++ ) {
			incnt = ( int )money[l].GetCount();
			for ( int m = 0; m < incnt; m++ ) {
				if ( money[l][m].code == _T( "324801" ) ) {
					memmove ( F211_23X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324802" ) ) {
					memmove ( F211_23D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324803" ) ) {
					memmove ( F211_23E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324800" ) ) {
					memmove ( F211_23F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324901" ) ) {
					memmove ( F211_24X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324902" ) ) {
					memmove ( F211_24D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324903" ) ) {
					memmove ( F211_24E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324900" ) ) {
					memmove ( F211_24F, money[l][m].arith, MONY_BUF_SIZE );
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
BOOL CShinFhyo2_11::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F211_10c & 0x880 ) || ( F211_12c & 0x800 ) || ( F211_19c & 0x880 ) ||
			 ( F211_20c & 0x880 ) || ( F211_23c & 0x880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}