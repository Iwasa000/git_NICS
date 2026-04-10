// ShinFhyo2_03.cpp : 実装ファイル
//

//#include "pch.h"
#include "stdafx.h"
#include "ShinFhyo2_03.h"
#include "ShinFhyo2_03idx.h"
#include "H31HyoView.h"
#include "ChangeTax2.h"
#include "ChangeTaxEx2.h"
//#include "afxdialogex.h"


// ShinFhyo2_03 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo2_03, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;


CShinFhyo2_03::CShinFhyo2_03(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo2_03::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo2_01Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pShinInfo ( NULL )
	, m_pSnTanaData ( NULL )
	, m_pSyzSyukei ( NULL )
	, m_SpcSgn ( FALSE )
	, m_UwSgn ( TRUE )
{
	memset ( F203_1A, '\0', sizeof ( F203_1A ) );
	memset ( F203_1B, '\0', sizeof ( F203_1B ) );
	memset ( F203_1C, '\0', sizeof ( F203_1C ) );

	memset ( F203_2C, '\0', sizeof ( F203_2C ) );

	memset ( F203_3C, '\0', sizeof ( F203_3C ) );

	memset ( F203_4C, '\0', sizeof ( F203_4C ) );

	memset ( F203_5C, '\0', sizeof ( F203_5C ) );

	memset ( F203_6C, '\0', sizeof ( F203_6C ) );

	memset ( F203_7C, '\0', sizeof ( F203_7C ) );

	memset ( F203_9A, '\0', sizeof ( F203_9A ) );
	memset ( F203_9B, '\0', sizeof ( F203_9B ) );
	memset ( F203_9C, '\0', sizeof ( F203_9C ) );

	memset ( F203_10A, '\0', sizeof ( F203_10A ) );
	memset ( F203_10B, '\0', sizeof ( F203_10B ) );
	memset ( F203_10C, '\0', sizeof ( F203_10C ) );

	memset ( F203_11B, '\0', sizeof ( F203_11B ) );
	memset ( F203_11C, '\0', sizeof ( F203_11C ) );

	memset ( F203_12B, '\0', sizeof ( F203_12B ) );
	memset ( F203_12C, '\0', sizeof ( F203_12C ) );

	memset ( F203_13A, '\0', sizeof ( F203_13A ) );
	memset ( F203_13B, '\0', sizeof ( F203_13B ) );
	memset ( F203_13C, '\0', sizeof ( F203_13C ) );

	memset ( F203_14A, '\0', sizeof ( F203_14A ) );
	memset ( F203_14B, '\0', sizeof ( F203_14B ) );
	memset ( F203_14C, '\0', sizeof ( F203_14C ) );

	memset ( F203_15A, '\0', sizeof ( F203_15A ) );
	memset ( F203_15B, '\0', sizeof ( F203_15B ) );
	memset ( F203_15C, '\0', sizeof ( F203_15C ) );

	memset ( F203_16A, '\0', sizeof ( F203_16A ) );
	memset ( F203_16B, '\0', sizeof ( F203_16B ) );
	memset ( F203_16C, '\0', sizeof ( F203_16C ) );

	memset ( F203_17A, '\0', sizeof ( F203_17A ) );
	memset ( F203_17B, '\0', sizeof ( F203_17B ) );
	memset ( F203_17C, '\0', sizeof ( F203_17C ) );

	memset ( F203_18A, '\0', sizeof ( F203_18A ) );
	memset ( F203_18B, '\0', sizeof ( F203_18B ) );
	memset ( F203_18C, '\0', sizeof ( F203_18C ) );

	memset ( F203_19A, '\0', sizeof ( F203_19A ) );
	memset ( F203_19B, '\0', sizeof ( F203_19B ) );
	memset ( F203_19C, '\0', sizeof ( F203_19C ) );

	memset ( F203_20A, '\0', sizeof ( F203_20A ) );
	memset ( F203_20B, '\0', sizeof ( F203_20B ) );
	memset ( F203_20C, '\0', sizeof ( F203_20C ) );

	memset ( F203_21A, '\0', sizeof ( F203_21A ) );
	memset ( F203_21B, '\0', sizeof ( F203_21B ) );
	memset ( F203_21C, '\0', sizeof ( F203_21C ) );

	memset ( F203_22A, '\0', sizeof ( F203_22A ) );
	memset ( F203_22B, '\0', sizeof ( F203_22B ) );
	memset ( F203_22C, '\0', sizeof ( F203_22C ) );

	memset ( F203_26A, '\0', sizeof ( F203_26A ) );
	memset ( F203_26B, '\0', sizeof ( F203_26B ) );
	memset ( F203_26C, '\0', sizeof ( F203_26C ) );

	memset ( F203_23A, '\0', sizeof ( F203_23A ) );
	memset ( F203_23B, '\0', sizeof ( F203_23B ) );
	memset ( F203_23C, '\0', sizeof ( F203_23C ) );

	memset ( F203_24A, '\0', sizeof ( F203_24A ) );
	memset ( F203_24B, '\0', sizeof ( F203_24B ) );
	memset ( F203_24C, '\0', sizeof ( F203_24C ) );

	memset ( F203_25A, '\0', sizeof ( F203_25A ) );
	memset ( F203_25B, '\0', sizeof ( F203_25B ) );
	memset ( F203_25C, '\0', sizeof ( F203_25C ) );


	F203_8C = 0;
	F203_8EX = 0;

	F203_10c = F203_12c = F203_19c = F203_20c = F203_23c = 0;

	eymd = 0;

	m_curwnd = -1;
}

CShinFhyo2_03::~CShinFhyo2_03()
{
}

void CShinFhyo2_03::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo2_03Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo2_03Diag02);
	DDX_Control(pDX, IDC_BUTTON1, m_tanaoroshi);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo2_03, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo2_03::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo2_03::OnBnClickedButton2)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo2_03, CSyzBaseDlg)
	ON_EVENT(CShinFhyo2_03, IDC_ICSDIAG8CTRL2, 1, CShinFhyo2_03::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_03, IDC_ICSDIAG8CTRL2, 2, CShinFhyo2_03::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_03, IDC_ICSDIAG8CTRL2, 3, CShinFhyo2_03::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinFhyo2_03, IDC_ICSDIAG8CTRL2, 9, CShinFhyo2_03::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()
// ShinFhyo2_03 メッセージ ハンドラー


BOOL CShinFhyo2_03::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL2 );
	m_BottomIdx = 170;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo2_03::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo2_03::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo2_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


BOOL CShinFhyo2_03::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	CWnd *pWnd = GetFocus();

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 付表２－３更新
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
				if( *m_pShinInfo->pOver500MillionSw != 1 && F203_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){	//5億円かつ課税割合が95％以上の場合
					diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_21A, CIcsdiagctrl );
				}
				else {
					if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
						if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
							diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_17A, CIcsdiagctrl );
						}
						else {
							diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_20A, CIcsdiagctrl );
						}
					}
					else {
						diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_21A, CIcsdiagctrl );
					}
				}
				return TRUE;
			}

			// 前項目へ
			if ( ( pMsg->wParam == VK_LEFT ) || ( pMsg->wParam == VK_F2 ) || ( pMsg->wParam == VK_UP ) || ( ( pMsg->wParam == VK_TAB ) && ( VK_FLG & 0x80 ) ) ) {
				if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
					diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_13B, CIcsdiagctrl );
				}
				else {
					if ( eymd >= ICS_SH_KYOJU_INPUT_DAY ) {
						diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_26B, CIcsdiagctrl );
					}
					else {
						diag_setposition ( IDC_ICSDIAG8CTRL2, ID203_22B, CIcsdiagctrl );
					}
				}
				return TRUE;
			}
		}

	}
	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

void CShinFhyo2_03::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;

}

void CShinFhyo2_03::EditOFFIcsdiag8ctrl2(short index)
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
	case ID203_1A:
	case ID203_1B:
		SyukeiDataSet ( index );

		// 課税売上高の５億超えチェック
		CheckOver500Million();
		// 各入力の表示
		f203data_all ( 1 );
		break;

	case ID203_2C:
		SyukeiDataSet ( index );

		// 課税売上高の５億超えチェック
		CheckOver500Million();
		// 各入力の表示
		f203data_all ( 2 );
		break;

	case ID203_3C:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 3 );
		break;

	case ID203_6C:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 4 );
		break;

	case ID203_9A:
	case ID203_9B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 5 );
		break;

	case ID203_10A:
	case ID203_10B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 6 );
		break;

	case ID203_11B:
		SyukeiDataSet (index );

		// 各入力の表示
		f203data_all ( 11 );
		break;

	case ID203_12B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 12 );
		break;

	case ID203_13A:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F203_13A, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_13A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F203_13A, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F203_13A, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) 
				&& ( F203_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
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
		f203data_all ( 7 );
		break;

	case ID203_13B:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F203_13B, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_13B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F203_13B, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F203_13B, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE )
				&& ( F203_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE )  ) {
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
		f203data_all ( 7 );
		break;

	case ID203_14A:
	case ID203_14B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 7 );
		break;

	case ID203_17A:
	case ID203_17B:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F203_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 8 );
		break;

	case ID203_18A:
	case ID203_18B:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F203_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 8 );
		break;

	case ID203_19A:
	case ID203_19B:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F203_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 8 );
		break;

	case ID203_20A:
	case ID203_20B:
		if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) {
			if ( ( F203_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder) || ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) ) {
				break;
			}
		}

		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 8 );
		break;

	case ID203_21A:
	case ID203_21B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 12 );
		break;

	case ID203_22A:
	case ID203_22B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 13 );
		break;

	case ID203_26A:
	case ID203_26B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 13 );
		break;

	case ID203_23A:
	case ID203_23B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 13 );
		break;

	case ID203_24A:
	case ID203_24B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 13 );
		break;

	case ID203_25A:
	case ID203_25B:
		SyukeiDataSet ( index );

		// 各入力の表示
		f203data_all ( 14 );
		break;

	case ID203_8EX:			// 課税売上割合の変更チェック
		if ( m_UwSgn ) {
			( *m_pSnHeadData )->Sn_UPERS &= 0xef;
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			if ( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_UPERS |= 0x10;
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323300" ) );
			}

			f203data_8();
			SyukeiDataSet ( index );

			// 各入力の表示
			f203data_all ( 4 );
		}
		break;

	case ID203_8C:			// 課税売上割合　合計 F
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
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
		if ( ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) && ( F203_8C != ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW ) ) {
			CString		Msg;
			Msg.Empty();
			Msg = _T( "課税売上割合が変更されました。\n" );
			Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
			ICSMessageBox ( Msg, MB_OK, 0 );
			F203_8C = ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW;
			f203data_8();
			MemoryReget();
			// 各入力の計算表示
			f203data_all ( 4 );
			break;
		}
		else {
			// 個別対応で95％未満の場合
			if ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) && ( F203_8C != ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW )
				&& ( ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW < ( *m_pSnHeadData )->m_UriRatioBorder ) ) {
				CString		Msg;
				Msg.Empty();
				Msg = _T( "課税売上割合が変更されました。\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
			F203_8C = ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW;
		}

		f203data_8();
		SyukeiDataSet ( index );

		// 各入力の計算表示
		f203data_all ( 4 );

		break;

	default:
		break;

	}
}

void CShinFhyo2_03::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Fhyo2_03Diag02.SetPosition ( pos );
	}
	if ( wTerm==VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index , 0 );
		m_Fhyo2_03Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) ){
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
			if ( index == ID203_13B ) {
				c_set ( IDC_BUTTON1 );
			}
			else {
				m_Fhyo2_03Diag02.SetNextPosition();
			}
		}
		else {
			if ( index == ID203_20B ) {
				c_set ( IDC_BUTTON1 );
			}
			else{
				m_Fhyo2_03Diag02.SetNextPosition();
			}

		}

	}

	// 前項目へ
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		BOOL IsPrevPosition = TRUE;
		if ( ( *m_pShinInfo->pOver500MillionSw != 1 ) && ( F203_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) ) {	//5億円以下かつ 課税売上割合が９５％以上の場合
			if ( index == ID203_19A ) {
				c_set ( IDC_BUTTON1 );
				IsPrevPosition = FALSE;
			}
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
					if ( index == ID203_17A ) {
						c_set ( IDC_BUTTON1 );
						IsPrevPosition = FALSE;
					}
				}
				else {
					if ( index == ID203_20A ) {
						c_set ( IDC_BUTTON1 );
						IsPrevPosition = FALSE;
					}
				}
			}
			else {
				if ( index == ID203_21A ) {
					c_set ( IDC_BUTTON1 );
					IsPrevPosition = FALSE;
				}
			}
		}
		if ( IsPrevPosition ) {
			m_Fhyo2_03Diag02.SetPrevPosition();
		}

	}
	if ( wTerm == VK_DELETE ) {
		switch( index ){
		case ID203_23A:
			F203_23c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );
			// 各入力の表示
			f203data_all ( 13 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_23A );
			break;

		case ID203_23B:
			F203_23c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );
			// 各入力の表示
			f203data_all ( 13 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_23B );
			break;

		case ID203_10A:
			F203_10c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			// 各入力の表示
			f203data_all ( 6 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_10A );
			break;

		case ID203_10B:
			F203_10c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			// 各入力の表示
			f203data_all ( 6 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_10B );
			break;

		case ID203_12B:
			F203_12c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			// 各入力の表示
			f203data_all ( 16 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_12B );
			break;

		case ID203_19A:
			F203_19c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324402" ) );
			// 各入力の表示
			f203data_all ( 8 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_19A );
			break;

		case ID203_19B:
			F203_19c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324403" ) );
			// 各入力の表示
			f203data_all ( 8 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_19B );
			break;

		case ID203_20A:
			F203_20c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324502" ) );
			// 各入力の表示
			f203data_all( 11 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_20A );
			break;
		case ID203_20B:
			F203_20c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324503" ) );
			// 各入力の表示
			f203data_all ( 11 );
			m_Fhyo2_03Diag02.SetPosition ( ID203_20B );
			break;
		case ID203_8EX:		// 課税売上割合変更チェック
			break;
		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2 , index );
			break;
		}

	}

	int nowpos = 0;
	CWnd* pWnd = this->GetFocus();
	if ( pWnd->GetDlgCtrlID() == IDC_BUTTON1 ) {
		nowpos = ID203_14C;	// 14Cのポジションをセット
	}
	else {
		nowpos = m_Fhyo2_03Diag02.GetPosition();	// 移動先または自身のポジション
	}

	RECT	rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int	cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

void CShinFhyo2_03::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char flg[MONY_BUF_SIZE] = { 0 };

	// ICSMessageBoxによってEditOffへはしるのを防ぐ
	m_UwSgn = FALSE;

	// 課税売上割合の変更チェック
	if ( index == ID203_8EX ) {
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
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
		m_Fhyo2_03Diag02.SetPosition ( index );

		( *m_pSnHeadData )->Sn_UPERS &= 0xef;
		if ( DiagData.data_check ) {
			( *m_pSnHeadData )->Sn_UPERS |= 0x10;
		}

		// 各入力の計算表示
		f203data_all ( 4 );
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
int CShinFhyo2_03::InitInfo ( CH31SnFhyo2_01Data **pSnFhyo2_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH31SnTanaData **pSnTanaData, CSyzSyukei **pSyzSyukei )
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
int CShinFhyo2_03::Init()
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
	m_Fhyo2_03Diag02.ModifyItem ( ID203_TS, DIAG_MDFY_TRANSPARENT );

	// 集計
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();


	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo2_03Diag01.ModifyItem ( 4, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh2_03();
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
int CShinFhyo2_03::ReadData()
{
	// 1.課税売上額(税抜き)
	memmove ( F203_1A, ( *m_pSnFhyo2_01Data )->Sn_F201_1D, MONY_BUF_SIZE );
	memmove ( F203_1B, ( *m_pSnFhyo2_01Data )->Sn_F201_1E, MONY_BUF_SIZE );
	memmove ( F203_1C, ( *m_pSnFhyo2_01Data )->Sn_F201_1F, MONY_BUF_SIZE );

	// 2.免税売上額
	memmove ( F203_2C, ( *m_pSnFhyo2_01Data )->Sn_F201_2F, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額…
	memmove ( F203_3C, ( *m_pSnFhyo2_01Data )->Sn_F201_3F, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove ( F203_4C, ( *m_pSnFhyo2_01Data )->Sn_F201_4F, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	memmove ( F203_5C, ( *m_pSnFhyo2_01Data )->Sn_F201_5F, MONY_BUF_SIZE );

	// 6.非課税売上額
	memmove ( F203_6C, ( *m_pSnFhyo2_01Data )->Sn_F201_6F, MONY_BUF_SIZE );

	// 7.課税資産の譲渡等の対価の額
	memmove ( F203_7C, ( *m_pSnFhyo2_01Data )->Sn_F201_7F, MONY_BUF_SIZE );

	// 8.課税売上割合
	F203_8EX = ( *m_pSnFhyo2_01Data )->Sn_F201_8EX;
	F203_8C = ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove ( F203_9A, ( *m_pSnFhyo2_01Data )->Sn_F201_9D, MONY_BUF_SIZE );
	memmove ( F203_9B, ( *m_pSnFhyo2_01Data )->Sn_F201_9E, MONY_BUF_SIZE );
	memmove ( F203_9C, ( *m_pSnFhyo2_01Data )->Sn_F201_9F, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove ( F203_10A, ( *m_pSnFhyo2_01Data )->Sn_F201_10D, MONY_BUF_SIZE );
	memmove ( F203_10B, ( *m_pSnFhyo2_01Data )->Sn_F201_10E, MONY_BUF_SIZE );
	memmove ( F203_10C, ( *m_pSnFhyo2_01Data )->Sn_F201_10F, MONY_BUF_SIZE );
	F203_10c = ( *m_pSnFhyo2_01Data )->Sn_F201_10sw;

	// 11.特定課税仕入れに係る支払対価の額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F203_11B, ( *m_pSnFhyo2_01Data )->Sn_F201_11E, MONY_BUF_SIZE );
		memmove ( F203_11C, ( *m_pSnFhyo2_01Data )->Sn_F201_11F, MONY_BUF_SIZE );
	}
	else {
		memset ( F203_11B, '\0', MONY_BUF_SIZE );
		memset ( F203_11C, '\0', MONY_BUF_SIZE );
	}

	// 12.特定課税仕入れに係る消費税額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F203_12B, ( *m_pSnFhyo2_01Data )->Sn_F201_12E, MONY_BUF_SIZE );
		memmove ( F203_12C, ( *m_pSnFhyo2_01Data )->Sn_F201_12F, MONY_BUF_SIZE );
		F203_12c = ( *m_pSnFhyo2_01Data )->Sn_F201_12sw;
	}
	else {
		memset ( F203_12B, '\0', MONY_BUF_SIZE );
		memset ( F203_12C, '\0', MONY_BUF_SIZE );
		F203_12c = 0;
	}

	// 13.課税貨物に係る消費税額
	memmove ( F203_13A, ( *m_pSnFhyo2_01Data )->Sn_F201_13D, MONY_BUF_SIZE );
	memmove ( F203_13B, ( *m_pSnFhyo2_01Data )->Sn_F201_13E, MONY_BUF_SIZE );
	memmove ( F203_13C, ( *m_pSnFhyo2_01Data )->Sn_F201_13F, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove ( F203_14A, ( *m_pSnFhyo2_01Data )->Sn_F201_14D, MONY_BUF_SIZE );
	memmove ( F203_14B, ( *m_pSnFhyo2_01Data )->Sn_F201_14E, MONY_BUF_SIZE );
	memmove ( F203_14C, ( *m_pSnFhyo2_01Data )->Sn_F201_14F, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove ( F203_15A, ( *m_pSnFhyo2_01Data )->Sn_F201_15D, MONY_BUF_SIZE );
	memmove ( F203_15B, ( *m_pSnFhyo2_01Data )->Sn_F201_15E, MONY_BUF_SIZE );
	memmove ( F203_15C, ( *m_pSnFhyo2_01Data )->Sn_F201_15F, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove ( F203_16A, ( *m_pSnFhyo2_01Data )->Sn_F201_16D, MONY_BUF_SIZE );
	memmove ( F203_16B, ( *m_pSnFhyo2_01Data )->Sn_F201_16E, MONY_BUF_SIZE );
	memmove ( F203_16C, ( *m_pSnFhyo2_01Data )->Sn_F201_16F, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove ( F203_17A, ( *m_pSnFhyo2_01Data )->Sn_F201_17D, MONY_BUF_SIZE );
	memmove ( F203_17B, ( *m_pSnFhyo2_01Data )->Sn_F201_17E, MONY_BUF_SIZE );
	memmove ( F203_17C, ( *m_pSnFhyo2_01Data )->Sn_F201_17F, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove ( F203_18A, ( *m_pSnFhyo2_01Data )->Sn_F201_18D, MONY_BUF_SIZE );
	memmove ( F203_18B, ( *m_pSnFhyo2_01Data )->Sn_F201_18E, MONY_BUF_SIZE );
	memmove ( F203_18C, ( *m_pSnFhyo2_01Data )->Sn_F201_18F, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove ( F203_19A, ( *m_pSnFhyo2_01Data )->Sn_F201_19D, MONY_BUF_SIZE );
	memmove ( F203_19B, ( *m_pSnFhyo2_01Data )->Sn_F201_19E, MONY_BUF_SIZE );
	memmove ( F203_19C, ( *m_pSnFhyo2_01Data )->Sn_F201_19F, MONY_BUF_SIZE );
	F203_19c = ( *m_pSnFhyo2_01Data )->Sn_F201_19sw;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove ( F203_20A, ( *m_pSnFhyo2_01Data )->Sn_F201_20D, MONY_BUF_SIZE );
	memmove ( F203_20B, ( *m_pSnFhyo2_01Data )->Sn_F201_20E, MONY_BUF_SIZE );
	memmove ( F203_20C, ( *m_pSnFhyo2_01Data )->Sn_F201_20F, MONY_BUF_SIZE );
	F203_20c = ( *m_pSnFhyo2_01Data )->Sn_F201_20sw;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove ( F203_21A, ( *m_pSnFhyo2_01Data )->Sn_F201_21D, MONY_BUF_SIZE );
	memmove ( F203_21B, ( *m_pSnFhyo2_01Data )->Sn_F201_21E, MONY_BUF_SIZE );
	memmove ( F203_21C, ( *m_pSnFhyo2_01Data )->Sn_F201_21F, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove ( F203_22A, ( *m_pSnFhyo2_01Data )->Sn_F201_22D, MONY_BUF_SIZE );
	memmove ( F203_22B, ( *m_pSnFhyo2_01Data )->Sn_F201_22E, MONY_BUF_SIZE );
	memmove ( F203_22C, ( *m_pSnFhyo2_01Data )->Sn_F201_22F, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	memmove ( F203_26A, ( *m_pSnFhyo2_01Data )->Sn_F201_26D, MONY_BUF_SIZE );
	memmove ( F203_26B, ( *m_pSnFhyo2_01Data )->Sn_F201_26E, MONY_BUF_SIZE );
	memmove ( F203_26C, ( *m_pSnFhyo2_01Data )->Sn_F201_26F, MONY_BUF_SIZE );

	// 24.控除対象仕入税額
	memmove ( F203_23A, ( *m_pSnFhyo2_01Data )->Sn_F201_23D, MONY_BUF_SIZE );
	memmove ( F203_23B, ( *m_pSnFhyo2_01Data )->Sn_F201_23E, MONY_BUF_SIZE );
	memmove ( F203_23C, ( *m_pSnFhyo2_01Data )->Sn_F201_23F, MONY_BUF_SIZE );
	F203_23c = ( *m_pSnFhyo2_01Data )->Sn_F201_23sw;

	// 25.控除過大調整税額
	memmove ( F203_24A, ( *m_pSnFhyo2_01Data )->Sn_F201_24D, MONY_BUF_SIZE );
	memmove ( F203_24B, ( *m_pSnFhyo2_01Data )->Sn_F201_24E, MONY_BUF_SIZE );
	memmove ( F203_24C, ( *m_pSnFhyo2_01Data )->Sn_F201_24F, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	memmove ( F203_25A, ( *m_pSnFhyo2_01Data )->Sn_F201_25D, MONY_BUF_SIZE );
	memmove ( F203_25B, ( *m_pSnFhyo2_01Data )->Sn_F201_25E, MONY_BUF_SIZE );
	memmove ( F203_25C, ( *m_pSnFhyo2_01Data )->Sn_F201_25F, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo2_03::ReadData2()
{
	// 旧税率は全てクリア
	char mony[MONY_BUF_SIZE] = { 0 }, flg[MONY_BUF_SIZE] = { 0 };;
	memset ( mony, '\0', MONY_BUF_SIZE );
	memset ( flg, '\0', MONY_BUF_SIZE );

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
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "327101" ), mony );


	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911104" ), F203_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322602" ), F203_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911105" ), F203_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322603" ), F203_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322700" ), F203_2C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322800" ), F203_3C );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323100" ), F203_6C );

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

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323402" ), F203_9A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323403" ), F203_9B );

			if ( F203_10c & 0x80 ) {
				( *m_pSyzSyukei)->SetShinkokuData ( _T( "323502" ), F203_10A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			}
			if ( F203_10c & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "323503" ), F203_10B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			}

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323603" ), F203_11B );

			if ( F203_12c & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "323703" ), F203_12B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			}

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323802" ), F203_13A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323803" ), F203_13B );

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F203_14A);
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F203_14B);
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F203_17A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F203_17B );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F203_18A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F203_18B );
		
		if ( F203_19c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324402" ), F203_19A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324402" ) );
		}

		if ( F203_19c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324403" ), F203_19B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324403" ) );
		}

		if ( F203_20c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324502" ), F203_20A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324502" ) );
		}

		if ( F203_20c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324503" ), F203_20B);
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324503" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324602") , F203_21A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324603") , F203_21B );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324702") , F203_22A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324703") , F203_22B );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327102") , F203_26A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327103") , F203_26B );
		
		if ( F203_23c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), F203_23A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), F203_24A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );
		}

		if ( F203_23c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), F203_23B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), F203_24B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F203_25A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F203_25B );

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

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324602" ), F203_21A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324603" ), F203_21B );
			
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324702" ), F203_22A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324703" ), F203_22B );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327102") , F203_26A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327103") , F203_26B );
	}
}

//=========================================================
// データ書込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo2_03::WriteData()
{
	// 1.課税売上額(税抜き)
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1D, F203_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1E, F203_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_1F, F203_1C, MONY_BUF_SIZE );

	// 2.免税売上額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_2F, F203_2C, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額…
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_3F, F203_3C, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_4F, F203_4C, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_5F, F203_5C, MONY_BUF_SIZE );

	// 6.非課税売上額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_6F, F203_6C, MONY_BUF_SIZE );

	// 7.課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_7F, F203_7C, MONY_BUF_SIZE );

	// 8.課税売上割合
	( *m_pSnFhyo2_01Data )->Sn_F201_8EX = F203_8EX;
	( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW = F203_8C;
	( *m_pSnHeadData )->Sn_UPERS &= 0xfe;
	if ( ( ( *m_pSnFhyo2_01Data )->Sn_F201_8EX ) < ( *m_pSnHeadData )->m_UriRatioBorder ) {
		( *m_pSnHeadData )->Sn_UPERS |= 0x01;
	}

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_9X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9D, F203_9A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9E, F203_9B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_9F, F203_9C, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_10X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10D, F203_10A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10E, F203_10B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_10F, F203_10C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_10sw = F203_10c;

	// 11.特定課税仕入れに係る支払対価の額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_11X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_11E, F203_11B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_11F, F203_11C, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_12X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_12E, F203_12B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_12F, F203_12C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_12sw = F203_12c;

	// 13.課税貨物に係る消費税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_13X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13D, F203_13A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13E, F203_13B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_13F, F203_13C, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_14X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14D, F203_14A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14E, F203_14B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_14F, F203_14C, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_15X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15D, F203_15A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15E, F203_15B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_15F, F203_15C, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_16X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16D, F203_16A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16E, F203_16B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_16F, F203_16C, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_17X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17D, F203_17A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17E, F203_17B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_17F, F203_17C, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_18X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18D, F203_18A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18E, F203_18B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_18F, F203_18C, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_19X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19D, F203_19A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19E, F203_19B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_19F, F203_19C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_19sw = F203_19c;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_20X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20D, F203_20A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20E, F203_20B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_20F, F203_20C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_20sw = F203_20c;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_21X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21D, F203_21A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21E, F203_21B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_21F, F203_21C, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_22X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22D, F203_22A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22E, F203_22B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_22F, F203_22C, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_26X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26D, F203_26A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26E, F203_26B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_26F, F203_26C, MONY_BUF_SIZE );

	// 24.控除対象仕入税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_23X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23D, F203_23A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23E, F203_23B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_23F, F203_23C, MONY_BUF_SIZE );
	( *m_pSnFhyo2_01Data )->Sn_F201_23sw = F203_23c;

	// 25.控除過大調整税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_24X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24D, F203_24A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24E, F203_24B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_24F, F203_24C, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_25X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25D, F203_25A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25E, F203_25B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo2_01Data )->Sn_F201_25F, F203_25C, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo2_03::MemoryReget()
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
			if ( money[l][m].code == _T( "322602" ) ) {
				memmove ( F203_1A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322603" ) ) {
				memmove ( F203_1B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322600" ) ) {
				memmove ( F203_1C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322700" ) ) {
				memmove ( F203_2C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322800" ) ) {
				memmove ( F203_3C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "322900" ) ) {
				memmove ( F203_4C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323000" ) ) {
				memmove ( F203_5C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323100" ) ) {
				memmove ( F203_6C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323200" ) ) {
				memmove ( F203_7C, money[l][m].arith, MONY_BUF_SIZE ); 
			}
			else if ( money[l][m].code == _T( "323300" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai,  money[l][m].arith, _T( "SSSSSSSSSSSSS9" ) );
				F203_8EX = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "323402" ) ) {
				memmove ( F203_9A, money[l][m].arith, MONY_BUF_SIZE);
			}
			else if ( money[l][m].code == _T( "323403" ) ) {
				memmove ( F203_9B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323400" ) ) {
				memmove ( F203_9C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323502" ) ) {
				memmove ( F203_10A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323503" ) ) {
				memmove ( F203_10B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323500" ) ) {
				memmove ( F203_10C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323603" ) ) {
				memmove ( F203_11B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323600" ) ) {
				memmove ( F203_11C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323703" ) ) {
				memmove ( F203_12B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323700" ) ) {
				memmove ( F203_12C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323802" ) ) {
				memmove ( F203_13A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323803" ) ) {
				memmove ( F203_13B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323800" ) ) {
				memmove ( F203_13C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323902" ) ) {
				memmove ( F203_14A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323903" ) ) {
				memmove ( F203_14B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "323900" ) ) {
				memmove ( F203_14C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324002" ) ) {
				memmove ( F203_15A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324003" ) ) {
				memmove ( F203_15B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324000" ) ) {
				memmove ( F203_15C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324102" ) ) {
				memmove ( F203_16A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324103" ) ) {
				memmove ( F203_16B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324100" ) ) {
				memmove ( F203_16C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324202" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_17A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_17A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324203" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_17B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_17B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324200" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_17C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_17C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324302" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_18A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_18A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324303" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_18B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_18B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324300" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_18C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_18C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324402" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_19A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_19A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324403" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_19B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_19B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324400" ) ) {
				if ( Kobetsu && ( Flg || semiuri ) ) {
					memmove ( F203_19C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_19C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324502" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F203_20A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_20A, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324503" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F203_20B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_20B, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324500" ) ) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove ( F203_20C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else {
					memset ( F203_20C, '\0', MONY_BUF_SIZE );
				}
			}
			else if ( money[l][m].code == _T( "324602" ) ) {
				memmove ( F203_21A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324603" ) ) {
				memmove ( F203_21B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324600" ) ) {
				memmove ( F203_21C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324702" ) ) {
				memmove ( F203_22A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324703" ) ) {
				memmove ( F203_22B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324700" ) ) {
				memmove ( F203_22C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327102" ) ) {
				memmove ( F203_26A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327103" ) ) {
				memmove ( F203_26B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "327100" ) ) {
				memmove ( F203_26C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324802" ) ) {
				memmove ( F203_23A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324803" ) ) {
				memmove ( F203_23B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324800" ) ) {
				memmove ( F203_23C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324902" ) ) {
				memmove ( F203_24A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324903" ) ) {
				memmove ( F203_24B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "324900" ) ) {
				memmove ( F203_24C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325002" ) ) {
				memmove ( F203_25A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325003" ) ) {
				memmove ( F203_25B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "325000" ) ) {
				memmove ( F203_25C, money[l][m].arith, MONY_BUF_SIZE );
			}
		}
	}

	// 特定収入計算表対応
	if ( *( m_pShinInfo )->pSgSpc == 1 ) {
		f203calq_spc ( SyzToSpc );
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
int CShinFhyo2_03::SyukeiDataSet ( int cID )
{
	char val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID203_1A:
		memmove ( F203_1A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322602" ), F203_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911104" ), F203_1A );
		break;
	case ID203_1B:
		memmove ( F203_1B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322603" ), F203_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "911105" ), F203_1B );
		break;
	case ID203_2C:
		memmove ( F203_2C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322700" ), F203_2C );
		break;
	case ID203_3C:
		memmove ( F203_3C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "322800" ), F203_3C );
		break;
	case ID203_6C:
		memmove ( F203_6C, val, MONY_BUF_SIZE);
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323100" ), F203_6C );
		break;
	case ID203_9A:
		memmove ( F203_9A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323402" ), F203_9A );
		break;
	case ID203_9B:
		memmove ( F203_9B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323403" ), F203_9B );
		break;
	case ID203_10A:
		if ( m_pArith->l_cmp ( F203_10A, val ) ) {
			F203_10c |= 0x80;
			memmove ( F203_10A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323502" ), F203_10A );
		}
		break;
	case ID203_10B:
		if ( m_pArith->l_cmp ( F203_10B, val ) ) {
			F203_10c |= 0x800;
			memmove ( F203_10B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323503" ), F203_10B);
		}
		break;
	case ID203_11B:
		memmove ( F203_11B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323603" ), F203_11B );
		break;
	case ID203_12B:
		if ( m_pArith->l_cmp ( F203_12B, val ) ) {
			F203_12c |= 0x800;
			memmove ( F203_12B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "323703" ), F203_12B );
		}
		break;
	case ID203_13A:
		memmove ( F203_13A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323802" ), F203_13A );
		break;
	case ID203_13B:
		memmove ( F203_13B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323803" ), F203_13B );
		break;
	case ID203_14A:
		memmove ( F203_14A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F203_14A );
		break;
	case ID203_14B:
		memmove ( F203_14B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F203_14B );
		break;
	case ID203_17A:
		memmove ( F203_17A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F203_17A );
		break;
	case ID203_17B:
		memmove ( F203_17B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F203_17B );
		break;
	case ID203_18A:
		memmove ( F203_18A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F203_18A );
		break;
	case ID203_18B:
		memmove ( F203_18B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData (_T( "324303" ), F203_18B );
		break;
	case ID203_19A:
		if ( m_pArith->l_cmp ( F203_19A, val ) ) {
			F203_19c |= 0x80;
			memmove ( F203_19A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324402" ), F203_19A );
		}
		break;
	case ID203_19B:
		if ( m_pArith->l_cmp ( F203_19B, val)) {
			F203_19c |= 0x800;
			memmove ( F203_19B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324403" ), F203_19B );
		}
		break;
	case ID203_20A:
		if ( m_pArith->l_cmp ( F203_20A, val ) ) {
			F203_20c |= 0x80;
			memmove ( F203_20A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324502" ), F203_20A );
		}
		break;
	case ID203_20B:
		if ( m_pArith->l_cmp ( F203_20B, val ) ) {
			F203_20c |= 0x800;
			memmove ( F203_20B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324503" ), F203_20B );
		}
		break;
	case ID203_21A:
		memmove ( F203_21A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324602" ), F203_21A );
		break;
	case ID203_21B:
		memmove ( F203_21B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324603" ), F203_21B );
		break;
	case ID203_22A:
		memmove ( F203_22A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324702" ), F203_22A );
		break;
	case ID203_22B:
		memmove ( F203_22B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324703" ), F203_22B );
		break;
	case ID203_26A:
		memmove ( F203_26A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327102" ), F203_26A );
		break;
	case ID203_26B:
		memmove ( F203_26B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "327103" ), F203_26B );
		break;
	case ID203_23A:
		if ( m_pArith->l_cmp ( F203_23A, val ) ) {
			F203_23c |= 0x80;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F203_24A, val, MONY_BUF_SIZE );
				memset ( F203_23A, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F203_23A, val, MONY_BUF_SIZE );
				memset ( F203_24A, '\0', MONY_BUF_SIZE);
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), F203_23A);
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), F203_24A);
		}
		break;
	case ID203_23B:
		if ( m_pArith->l_cmp ( F203_23B, val ) ) {
			F203_23c |= 0x800;
			if ( m_pArith->l_test ( val ) < 0 ) {
				m_pArith->l_neg ( val );
				memmove ( F203_24B, val, MONY_BUF_SIZE );
				memset ( F203_23B, '\0', MONY_BUF_SIZE );
			}
			else {
				memmove ( F203_23B, val, MONY_BUF_SIZE );
				memset ( F203_24B, '\0', MONY_BUF_SIZE );
			}
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), F203_23B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), F203_24B );
		}
		break;
	case ID203_24A:
		memmove ( F203_24A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), F203_24A );
		break;
	case ID203_24B:
		memmove ( F203_24B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), F203_24B );
		break;
	case ID203_25A:
		memmove ( F203_25A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F203_25A );
		break;
	case ID203_25B:
		memmove ( F203_25B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F203_25B );
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
int CShinFhyo2_03::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo2_03[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo2_03[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo2_03[cnt].UnConnect;

				int st = ChkTokureiIdx ( Fhyo2_03[cnt].Index );
				if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) && ( st == 1 ) ) {
					Flg = 0;
				}
				else {
					st = ChkTkkzIdx ( Fhyo2_03[cnt].Index );
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
				Flg = Fhyo2_03[cnt].Connect;
			}
		}
		if ( ( ( *m_pSnHeadData )->Sn_EXP100 & 0x01 ) && *( m_pShinInfo->pSgSpc ) && ( Fhyo2_03[cnt].Index == ID203_8EX ) ) {
			Flg = 0;
		}

		if ( m_SpcSgn ) {
			//if ( ( Fhyo2_03[cnt].Index == ID203_24A ) || ( Fhyo2_03[cnt].Index == ID203_24B ) ) {
			if ( ( Fhyo2_03[cnt].Index == ID203_23A ) || ( Fhyo2_03[cnt].Index == ID203_23B ) ) {
				Flg = 0;
			}
		}
		if ( ( Fhyo2_03[cnt].Index == ID203_26A ) || ( Fhyo2_03[cnt].Index == ID203_26B ) ) {
			if ( eymd < ICS_SH_KYOJU_INPUT_DAY ) {
				Flg = 0;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo2_03[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo2_03[cnt].Connect );
		cnt++;
	}

	m_Fhyo2_03Diag02.EnableDelete ( 0 );

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
int CShinFhyo2_03::ChkTkkzIdx ( short idx )
{
	int rSt = 0;

	if ( ( idx == ID203_11B ) || ( idx == ID203_12B ) ) {
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
int CShinFhyo2_03::ChkTokureiIdx ( short idx )
{
	int rSt = 0;

	if ( ( ID203_9A <= idx ) && ( idx <= ID203_13B ) ) {
		rSt = 1;
	}

	return rSt;
}

//=========================================================
// 画面出力
//=========================================================
void CShinFhyo2_03::DispFh2_03()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 4; i <= 6; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	for( int i = ID203_1A; i <= ID203_25C; i++ ){
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		//手入力サイン初期化
		F203_10c = 0;
		F203_19c = 0;
		F203_20c = 0;
		F203_23c = 0;
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
	ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION + 8, 1 );
	diag_clear ( IDC_ICSDIAG8CTRL1, 3, TRUE, CIcsdiagctrl );
	CheckOver500Million();

	// 全項目表示
	f203data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo2_03::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo2_03::dsp_cls ( unsigned short Id, short Index )
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
void CShinFhyo2_03::CheckOver500Million()
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
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION + 8, 5 );
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
			diag_setdata ( IDC_ICSDIAG8CTRL1, 3, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}
	}
	else {			// ４億５千万以上で５億以下又は４億５千万未満
		( *m_pSnHeadData )->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION, 1 );
			//ChangeColor ( IDC_ICSDIAG8CTRL2, MILLION + 8, 1 );
			color = BC_WHITE;
			diag_clear ( IDC_ICSDIAG8CTRL1, 3, TRUE, CIcsdiagctrl );
		}
	}

	// m_Fhyo2_03Diag02.Refresh()を呼ぶと解像度が1024×768のときに正常に表示されなくなる
	if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
		diag_getattr ( IDC_ICSDIAG8CTRL2, MILLION, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL2, MILLION, &DA, TRUE, CIcsdiagctrl );

		diag_getattr ( IDC_ICSDIAG8CTRL2, MILLION + 8, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL2, MILLION + 8, &DA, TRUE, CIcsdiagctrl );
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
int CShinFhyo2_03::CheckOver500MillionSub ( int &MilSw, char *pVAL, char *pValCnv )
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
				// 付表２-３データ
				if ( money[l][m].code == _T( "322602" ) ) {
					memmove ( F203_1A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322603" ) ) {
					memmove ( F203_1B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322700" ) ) {
					memmove ( F203_2C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "323300" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "SSSSSSSSSSSSS9" ) );
					F203_8EX = atoi ( wariai );
				}
			}
		}
	}

	m_pArith->l_add ( tmpVAL, F203_1A, F203_1B );
	m_pArith->l_add ( tmpVAL, tmpVAL, F203_2C );
	int sw = ( *m_pSnHeadData )->CheckTaxationSales ( tmpVAL, tmpValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );

	if ( sw == 1 ) {
		m_pArith->l_input ( flg, _T( "0" ) );
		nflg = 0;
	}
	else {
		if ( F203_8EX >= 9500 ) {
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
void CShinFhyo2_03::f203data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f203data_1();

	case 2:
		f203data_2();

	case 3:
		f203data_3();

	case 4:
	case 5:
		f203data_4_5();

	case 6:	
		f203data_6();

	case 7:
		f203data_7();

	case 8:
		f203data_8();
		Changef203data_8();

	case 9:
		f203data_9();

	case 10:
		f203data_10();

	case 11:
		f203data_11();

	case 12:
		f203data_12();

	case 13:
		f203data_13();

	case 14:
		f203data_14();

	case 15:
		f203data_15();

	case 16:
		f203data_16();

	case 17:
		f203data_17();

	case 18:
		f203data_18();

	case 19:
		f203data_19();

	case 20:
		f203data_20();

	case 21:
		f203data_21();

	case 22:
		f203data_22();

	case 23:
		f203data_26();

	case 24:
		f203data_23();

	case 25:
		f203data_24();

	case 26:
		f203data_25();

		break;
	default:
		break;
	}
}

//=========================================================
// 課税売上(税抜き)表示
//=========================================================
void CShinFhyo2_03::f203data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_1A, F203_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_1B, F203_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_1C, F203_1C );
}

//=========================================================
// 免税売上額
//=========================================================
void CShinFhyo2_03::f203data_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_2C, F203_2C );
}

//=========================================================
// 非課税資産の輸出等の金額、海外支店等へ移送した資産の価額
//=========================================================
void CShinFhyo2_03::f203data_3()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_3C, F203_3C );
}

//=========================================================
// 課税資産の譲渡等の対価の額
//=========================================================
void CShinFhyo2_03::f203data_4_5()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_4C, F203_4C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_5C, F203_5C );
}

//=========================================================
// 非課税売上額
//=========================================================
void CShinFhyo2_03::f203data_6()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_6C, F203_6C );
}

//=========================================================
// 資産の譲渡等の対価の額
//=========================================================
void CShinFhyo2_03::f203data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_7C, F203_7C );
}

//=========================================================
// 課税売上割合
//=========================================================
void CShinFhyo2_03::f203data_8()
{
	char WORK0[MONY_BUF_SIZE] = { 0 };
	char buf[512] = { 0 };

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 課税売上高割合
	DiagData.data_check = 0;
	diag_setdata ( IDC_ICSDIAG8CTRL2, ID203_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8C, 1 );
	diag_modify ( IDC_ICSDIAG8CTRL2, ID203_8C, DIAG_MDFY_READONLY, CIcsdiagctrl );
	memset ( buf, '\0', sizeof ( buf ) );
	memset ( WORK0, '\0', MONY_BUF_SIZE );
	if ( !( ( *m_pSnHeadData )->Sn_EXP100 & 0x01) ) {
		/*if( *m_pShinInfo->pSgSpc ){
			sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), F203_8EX );
			m_pArith->l_input ( WORK0, buf );

			// 特定収入有り
			ChangeColor( IDC_ICSDIAG8CTRL2, ID203_8EX, 1 );
			diag_modify( IDC_ICSDIAG8CTRL2, ID203_8EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
		}*/
		//else{
			if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
				// 課税売上割合　手入力
				DiagData.data_check = 1;
				diag_setdata ( IDC_ICSDIAG8CTRL2, ID203_8EX, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), ( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW );
				m_pArith->l_input ( WORK0, buf );
				( *m_pSyzSyukei )->SetShinkokuData (_T( "323300" ), WORK0 );

				if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() ) {
					memset ( buf, '\0', sizeof ( buf ) );
					strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)\n【課税売上割合に準ずる割合を適用】" ) );
					DiagData.data_disp = buf;
					diag_setdata ( IDC_ICSDIAG8CTRL2, 63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8C, 1 );
				}
				else {
					memset ( buf, '\0', sizeof ( buf ) );
					strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)" ) );
					DiagData.data_disp = buf;
					diag_setdata ( IDC_ICSDIAG8CTRL2, 63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8C, 1 );
				}

				if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8C, 0 );
					diag_modify ( IDC_ICSDIAG8CTRL2, ID203_8C, DIAG_MDFY_EDIT, CIcsdiagctrl );
				}
				else {
					ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8C, 2 );
				}
			}
			else {
				sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), F203_8EX );
				m_pArith->l_input ( WORK0, buf );

				memset ( buf, '\0', sizeof ( buf ) );
				strcpy_s ( buf, sizeof ( buf ), _T( "課　税　売　上　割　合　　(4/7)" ) );
				DiagData.data_disp = buf;
				diag_setdata ( IDC_ICSDIAG8CTRL2, 63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8C, 1 );
			}
		//}
	}
	else{
		m_pArith->l_input ( WORK0, _T( "10000" ) );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323300" ), WORK0 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_8EX, 1 );
		diag_modify ( IDC_ICSDIAG8CTRL2, ID203_8EX, DIAG_MDFY_READONLY, CIcsdiagctrl );

	}

	memset ( buf, '\0', sizeof ( buf ) );
	m_pArith->l_print ( buf, WORK0, _T( "SSSSS" ) );
	F203_8EX = atoi ( buf );
	m_pArith->l_print ( buf, WORK0, _T( "SS9.99" ) );
	DiagData.data_edit = buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, ID203_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 課税売上割合を無効にするとリセットされてしまうため、別関数にする
/*	// 課税売上割合に準ずる割合を有効
	// 課税売上割合が９５％未満
	if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() || ( F203_8EX < (*m_pSnHeadData)->m_UriRatioBorder ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 )== 0x00 ) ) {
				ModifyDiagInput ( ID203_17A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_17B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17B, 0 );

				ModifyDiagInput ( ID203_18A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_18B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18B, 0 );

				ModifyDiagInput ( ID203_19A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_19B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, ( F203_19c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, ( F203_19c & 0x800 ) ? 3 : 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, 1 );
				ModifyDiagInput ( ID203_20A, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID203_20B, DIAG_MDFY_READONLY );
			}
		}
		else {
			ModifyDiagInput ( ID203_17A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID203_17B, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17B, 1 );

			ModifyDiagInput ( ID203_18A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID203_18B, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18B, 1 );

			F203_19c = 0;
			ModifyDiagInput ( ID203_19A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID203_19B, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, 1 );

			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
				ModifyDiagInput ( ID203_20A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_20B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, ( F203_20c & 0x80 )  ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, ( F203_20c & 0x800 ) ? 3 : 0 );
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

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17B, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18B, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, 1 );

		ModifyDiagInput ( ID203_17A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_17B, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID203_18A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_18B, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID203_19A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_19B, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID203_20A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_20B, DIAG_MDFY_READONLY );
	}
*/
}

//=========================================================
// 課税仕入れに係る支払対価の額(税込み)
//=========================================================
void CShinFhyo2_03::f203data_9()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_9A, F203_9A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_9B, F203_9B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_9C, F203_9C );
}

//=========================================================
// 課税仕入れに係る消費税額
//=========================================================
void CShinFhyo2_03::f203data_10()
{
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_10A, ( F203_10c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_10B, ( F203_10c & 0x800 ) ? 3 : 0 );
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_10A, F203_10A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_10B, F203_10B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_10C, F203_10C );
}

//=========================================================
// 特定課税仕入れに係る支払対価の額
//=========================================================
void CShinFhyo2_03::f203data_11()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_11B, F203_11B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_11C, F203_11C );
}

//=========================================================
// 特定課税仕入れに係る消費税額
//=========================================================
void CShinFhyo2_03::f203data_12()
{
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_12B, ( F203_12c & 0x800 ) ? 3 : 0 );
			}
			else {
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_12B, 1 );
			}
		}
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_12B, F203_12B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_12C, F203_12C );
}

//=========================================================
// 課税貨物に係る消費税額
//=========================================================
void CShinFhyo2_03::f203data_13()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_13A, F203_13A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_13B, F203_13B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_13C, F203_13C );
}

//=========================================================
// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_03::f203data_14()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_14A, F203_14A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_14B, F203_14B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_14C, F203_14C );
}

//=========================================================
// 課税仕入れ等の税額の合計額
//=========================================================
void CShinFhyo2_03::f203data_15()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_15A, F203_15A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_15B, F203_15B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_15C, F203_15C );
}

//=========================================================
// 課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合
//=========================================================
void CShinFhyo2_03::f203data_16()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_16A, F203_16A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_16B, F203_16B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_16C, F203_16C );
}

//=========================================================
// 15のうち、課税売上げにのみ要するもの
//=========================================================
void CShinFhyo2_03::f203data_17()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_17A, F203_17A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_17B, F203_17B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_17C, F203_17C );
}

//=========================================================
// 15のうち、課税売上げと非課税売上げに共通して要するもの
//=========================================================
void CShinFhyo2_03::f203data_18()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_18A, F203_18A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_18B, F203_18B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_18C, F203_18C );
}

//=========================================================
// 個別対応方式により控除する課税仕入れ等の税額
//=========================================================
void CShinFhyo2_03::f203data_19()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_19A, F203_19A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_19B, F203_19B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_19C, F203_19C );
}

//=========================================================
// 一括比例配分方式により控除する課税仕入れ等の税額
//=========================================================
void CShinFhyo2_03::f203data_20()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_20A, F203_20A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_20B, F203_20B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_20C, F203_20C );
}

//=========================================================
// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_03::f203data_21()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_21A, F203_21A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_21B, F203_21B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_21C, F203_21C );
}

//=========================================================
// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額
//=========================================================
void CShinFhyo2_03::f203data_22()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_22A, F203_22A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_22B, F203_22B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_22C, F203_22C );
}

//=========================================================
// 控除対象仕入税額がプラスの時
//=========================================================
void CShinFhyo2_03::f203data_23()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00) && ( m_SpcSgn == FALSE ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_23A, ( F203_23c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_23B, ( F203_23c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_23A, F203_23A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_23B, F203_23B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_23C, F203_23C );
}

//=========================================================
// 控除対象仕入税額がマイナスの時
//=========================================================
void CShinFhyo2_03::f203data_24()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_24A, F203_24A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_24B, F203_24B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_24C, F203_24C );
}

//=========================================================
// 貸倒回収に係る消費税額
//=========================================================
void CShinFhyo2_03::f203data_25()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_25A, F203_25A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_25B, F203_25B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_25C, F203_25C );
}

//=========================================================
// 居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
//=========================================================
void CShinFhyo2_03::f203data_26()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_26A, F203_26A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_26B, F203_26B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID203_26C, F203_26C );
}

//=========================================================
// 課税売上割合を変更
//=========================================================
void CShinFhyo2_03::Changef203data_8()
{
	// 課税売上割合に準ずる割合を有効
	// 課税売上割合が９５％未満
	if ( ( *m_pSnHeadData )->IsUseSemiUriRatio() || ( F203_8EX < (*m_pSnHeadData)->m_UriRatioBorder ) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() ) {
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 )== 0x00 ) ) {
				ModifyDiagInput ( ID203_17A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_17B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17B, 0 );

				ModifyDiagInput ( ID203_18A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_18B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18B, 0 );

				ModifyDiagInput ( ID203_19A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_19B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, ( F203_19c & 0x80 ) ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, ( F203_19c & 0x800 ) ? 3 : 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, 1 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, 1 );
				ModifyDiagInput ( ID203_20A, DIAG_MDFY_READONLY );
				ModifyDiagInput ( ID203_20B, DIAG_MDFY_READONLY );

				F203_20c = 0;
			}
		}
		else {
			ModifyDiagInput ( ID203_17A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID203_17B, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17B, 1 );

			ModifyDiagInput ( ID203_18A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID203_18B, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18B, 1 );

			F203_19c = 0;
			ModifyDiagInput ( ID203_19A, DIAG_MDFY_READONLY );
			ModifyDiagInput ( ID203_19B, DIAG_MDFY_READONLY );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, 1 );
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, 1 );

			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
				ModifyDiagInput ( ID203_20A, DIAG_MDFY_EDIT );
				ModifyDiagInput ( ID203_20B, DIAG_MDFY_EDIT );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, 0 );

				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, ( F203_20c & 0x80 )  ? 3 : 0 );
				ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, ( F203_20c & 0x800 ) ? 3 : 0 );
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

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_17B, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_18B, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_19B, 1 );

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20A, 1 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID203_20B, 1 );

		ModifyDiagInput ( ID203_17A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_17B, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID203_18A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_18B, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID203_19A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_19B, DIAG_MDFY_READONLY );

		ModifyDiagInput ( ID203_20A, DIAG_MDFY_READONLY );
		ModifyDiagInput ( ID203_20B, DIAG_MDFY_READONLY );

		F203_19c = 0;
		F203_20c = 0;
	}
}

//=========================================================
// 棚卸データの加減算を行う
//=========================================================
void CShinFhyo2_03::TanaDataAdd()
{
	char VAL_14A[6] = { 0 }, VAL_14B[6] = { 0 },
		 VAL_17A[6] = { 0 }, VAL_17B[6] = { 0 },
		 VAL_18A[6] = { 0 }, VAL_18B[6] = { 0 };

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		// 非連動は保存データで使用するので、再計算不要
		return;
	}
	else {
		// 連動時でも特例計算(5-(3)号様式)を摘要する場合は再計算しない
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			return;
		}
	}

	if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01) ) {
		if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) {
			MoneyBasejagArray money;
			char wariai[32] = { 0 };

			// 連動データを取得
			money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "323300" ) );
			m_pArith->l_print ( wariai, money[0][0].arith, _T( "SSSSSSSSSSSSS9" ) );
			F203_8EX = atoi ( wariai );
			CheckOver500Million();

			if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
				m_pArith->l_add ( VAL_14B, ( *m_pSnTanaData )->Sn_mnz78[0], ( *m_pSnTanaData )->Sn_mnz78[1] );
				m_pArith->l_add ( VAL_14B, VAL_14B, ( *m_pSnTanaData )->Sn_mnz78[2] );
				m_pArith->l_add ( VAL_14A, ( *m_pSnTanaData )->Sn_mnz624[0], ( *m_pSnTanaData )->Sn_mnz624[1] );
				m_pArith->l_add ( VAL_14A, VAL_14A, ( *m_pSnTanaData )->Sn_mnz624[2] );
			

				// 旧格納場所へ格納
				memmove ( ( *m_pSnTanaData )->Sn_MNZ78, VAL_14B, MONY_BUF_SIZE );
				memmove ( ( *m_pSnTanaData )->Sn_MNZ624, VAL_14A, MONY_BUF_SIZE );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( VAL_14A );
					m_pArith->l_neg ( VAL_14B );
				}

				// 個別対応で95％未満の場合
				if ( ( F203_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
					memmove ( VAL_17B, ( *m_pSnTanaData )->Sn_mnz78[0], MONY_BUF_SIZE );
					memmove ( VAL_18B, ( *m_pSnTanaData )->Sn_mnz78[2], MONY_BUF_SIZE );
					memmove ( VAL_17A, ( *m_pSnTanaData )->Sn_mnz624[0], MONY_BUF_SIZE );
					memmove ( VAL_18A, ( *m_pSnTanaData )->Sn_mnz624[2], MONY_BUF_SIZE );

					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
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
				memmove ( VAL_14A, ( *m_pSnTanaData )->Sn_MNZ624, MONY_BUF_SIZE );
				memmove ( VAL_14B, ( *m_pSnTanaData )->Sn_MNZ78, MONY_BUF_SIZE );

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg ( VAL_14A );
					m_pArith->l_neg ( VAL_14B );
				}
			}
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), VAL_14A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), VAL_14B );
	}

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A602" ), VAL_17A );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A603" ), VAL_17B );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A702" ), VAL_18A );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "32A703" ), VAL_18B );
}

//=========================================================
// 表示金額取得
//=========================================================
void CShinFhyo2_03::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1.課税売上額(税抜き)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 2.免税売上額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額、海外支店等へ移送した資産の価額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_3C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_3C, DiagData.data_val, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_4C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_4C, DiagData.data_val, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_5C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_5C, DiagData.data_val, MONY_BUF_SIZE );

	// 6.非課税売上額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_6C, DiagData.data_val, MONY_BUF_SIZE );

	// 7.資産譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_7C, DiagData.data_val, MONY_BUF_SIZE );

	// 9.課税仕入れに係る支払い対価の額(税込み)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_9A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_9A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_9B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_9B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_9C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_9C, DiagData.data_val, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_10A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_10A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_10B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_10B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_10C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_10C, DiagData.data_val, MONY_BUF_SIZE );

	// 11.特定課税仕入れに係る支払対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_11B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_11B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_11C, DiagData.data_val, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_12B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_12B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_12C, DiagData.data_val, MONY_BUF_SIZE );

	// 13.課税貨物に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_13A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_13A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_13B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_13B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_13C, DiagData.data_val, MONY_BUF_SIZE );

	// 14.納税義務の免除を受ける(受けない)こととなった場合における消費税額の調整額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_14A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_14A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_14B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_14B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_14C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_14C, DiagData.data_val, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_15A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_15A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_15B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_15B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_15C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_15C, DiagData.data_val, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、割合が95％以上の場合
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_16A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_16A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_16B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_16B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_16C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_16C, DiagData.data_val, MONY_BUF_SIZE );

	// 17.15のうち、課税売上げにのみ要するもの
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_17A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_17A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_17B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_17B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_17C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_17C, DiagData.data_val, MONY_BUF_SIZE );

	// 18.15のうち、共通して要するもの
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_18A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_18A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_18B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_18B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_18C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_18C, DiagData.data_val, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_19A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_19A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_19B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_19B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_19C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_19C, DiagData.data_val, MONY_BUF_SIZE );

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_20A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_20A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_20B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_20B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_20C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_20C, DiagData.data_val, MONY_BUF_SIZE );

	// 21.課税売上割合変動時の…消費税額の調整額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_21A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_21A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_21B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_21B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_21C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_21C, DiagData.data_val, MONY_BUF_SIZE );

	// 22.調整対象固定資産を…調整額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_22A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_22A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_22B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_22B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_22C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_22C, DiagData.data_val, MONY_BUF_SIZE );

	// 23.居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_26A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_26A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_26B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_26B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_26C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_26C, DiagData.data_val, MONY_BUF_SIZE );

	// 24.控除対象仕入税額がプラスの時
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_23A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_23A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_23B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove( F203_23B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_23C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_23C, DiagData.data_val, MONY_BUF_SIZE );

	// 25.控除過大調整額がマイナスの時
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_24A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_24A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_24B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_24B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_24C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_24C, DiagData.data_val, MONY_BUF_SIZE );

	// 26.貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_25A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_25A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_25B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_25B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID203_25C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F203_25C, DiagData.data_val, MONY_BUF_SIZE );
}

//=========================================================
// 課税売上割合の取得
//---------------------------------------------------------
// 返送値
//		計算した課税売上割合
//=========================================================
unsigned short CShinFhyo2_03::GetCalqedKzwari()
{
	unsigned short	rtKzwari = F203_8EX;

	if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
		// 課税売上割合を変更

		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x02 ) {
			// 課税売上割合に準ずる割合を使用
												// 計算
			char WORK0[MONY_BUF_SIZE] = { 0 };
			char PW6[MONY_BUF_SIZE]   = { 0 };
			char PER100BUF[MONY_BUF_SIZE] = { 0 };

			m_pArith->l_input ( PER100BUF, _T( "10000" ) );
			m_pArith->l_par100 ( ( unsigned char* )WORK0, ( unsigned char* )F203_4C, ( unsigned char* )F203_7C, 2, 0 );
			if ( m_pArith->l_cmp ( F203_4C, PW6 ) < 0 ) {
				memset ( WORK0, '\0', MONY_BUF_SIZE );
			}
			if ( m_pArith->l_cmp ( F203_7C, PW6 ) < 0 ) {
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
void CShinFhyo2_03::ChangeBtnDisp()
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
void CShinFhyo2_03::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( (index = m_Fhyo2_03Diag02.GetPosition()) != -1 ) {
			m_Fhyo2_03Diag02.SetPosition ( index );
			m_Fhyo2_03Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo2_03Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Fhyo2_03Diag02.SetFocus();

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) ) {
			// 非課税売上額
			m_Fhyo2_03Diag02.SetPosition ( ID203_2C );
		}
		else {
			// 課税売上額
			m_Fhyo2_03Diag02.SetPosition( ID203_1A );
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
int CShinFhyo2_03::UpDownFocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY2_01CNT; i++ ) {
		if ( Index == FCS_Fhyo2_03[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo2_03[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY2_01CNT; j++ ) {
			m_Fhyo2_03Diag02.GetAttribute ( FCS_Fhyo2_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_03[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo2_03Diag02.GetAttribute ( FCS_Fhyo2_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_03[j].IDNo;
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
void CShinFhyo2_03::ModifyDiagInput ( short index, int sign )
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
void CShinFhyo2_03::ChangeColor ( unsigned short id, short Index, int sign )
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
void CShinFhyo2_03::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {

		// 前回の情報をセーブ
		char			SV_mtype = ( *m_pSnTanaData )->Sn_MENZsw;
		unsigned short	SV_F203_8E = F203_8EX;
		char			SV_mnz624[3][MONY_BUF_SIZE];
		char			SV_mnz78[3][MONY_BUF_SIZE];

		memset ( SV_mnz624, '\0', sizeof ( SV_mnz624 ) );
		memset ( SV_mnz78, '\0', sizeof ( SV_mnz78 ) );

		memmove ( SV_mnz78[0], ( *m_pSnTanaData )->Sn_mnz78[0], MONY_BUF_SIZE );
		memmove ( SV_mnz78[1], ( *m_pSnTanaData )->Sn_mnz78[1], MONY_BUF_SIZE );
		memmove ( SV_mnz78[2], ( *m_pSnTanaData )->Sn_mnz78[2], MONY_BUF_SIZE );

		memmove ( SV_mnz624[0], ( *m_pSnTanaData )->Sn_mnz624[0], MONY_BUF_SIZE );
		memmove ( SV_mnz624[1], ( *m_pSnTanaData )->Sn_mnz624[1], MONY_BUF_SIZE );
		memmove ( SV_mnz624[2], ( *m_pSnTanaData )->Sn_mnz624[2], MONY_BUF_SIZE );

		// 個別対応
		CChangeTaxEx2 CTDiag;
		CTDiag.InitInfo ( ( *m_pSnFhyo2_01Data ),( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );

		HINSTANCE	svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle ( g_hInstance );
		int st = ( int )CTDiag.DoModal();
		AfxSetResourceHandle ( svhInstResource );

		if ( st == IDOK ) {
			// 手入力
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80) {	// 非連動
				// 前回が個別対応で95％未満の場合、前回分を削除
				if ( ( SV_mtype & 0x10 ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
					if ( SV_mtype & 0x01 ) {
						m_pArith->l_add ( F203_17B, F203_17B, SV_mnz78[0] );		// 課税売上に係る仕入税額
						m_pArith->l_add ( F203_18B, F203_18B, SV_mnz78[2] );		// 共通の売上に係る仕入税額
						m_pArith->l_add ( F203_17A, F203_17A, SV_mnz624[0] );		// 課税売上に係る仕入税額
						m_pArith->l_add ( F203_18A, F203_18A, SV_mnz624[2] );		// 共通の売上に係る仕入税額
					}
					else {
						m_pArith->l_sub ( F203_17B, F203_17B, SV_mnz78[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub ( F203_18B, F203_18B, SV_mnz78[2] );		// 16. 共通の売上に係る仕入税額
						m_pArith->l_sub ( F203_17A, F203_17A, SV_mnz624[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub ( F203_18A, F203_18A, SV_mnz624[2] );		// 16. 共通の売上に係る仕入税額
					}
				}

				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
					memset ( F203_14B, '\0', MONY_BUF_SIZE );
					m_pArith->l_add ( F203_14B, F203_14B, ( *m_pSnTanaData )->Sn_mnz78[0] );
					m_pArith->l_add ( F203_14B, F203_14B, ( *m_pSnTanaData )->Sn_mnz78[1] );
					m_pArith->l_add ( F203_14B, F203_14B, ( *m_pSnTanaData )->Sn_mnz78[2] );

					memset ( F203_14A, '\0', MONY_BUF_SIZE );
					m_pArith->l_add ( F203_14A, F203_14A, ( *m_pSnTanaData )->Sn_mnz624[0] );
					m_pArith->l_add ( F203_14A, F203_14A, ( *m_pSnTanaData )->Sn_mnz624[1] );
					m_pArith->l_add ( F203_14A, F203_14A, ( *m_pSnTanaData )->Sn_mnz624[2] );

					// 旧格納場所へ格納
					memmove ( ( *m_pSnTanaData )->Sn_MNZ78, F203_14B, MONY_BUF_SIZE);
					memmove ( ( *m_pSnTanaData )->Sn_MNZ624, F203_14A, MONY_BUF_SIZE);

					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( F203_14A);
						m_pArith->l_neg ( F203_14B);
					}

					// 個別対応で95％未満の場合
					if ( ( F203_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {
						if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
							m_pArith->l_sub ( F203_17B, F203_17B, ( *m_pSnTanaData )->Sn_mnz78[0] );
							m_pArith->l_sub ( F203_18B, F203_18B, ( *m_pSnTanaData )->Sn_mnz78[2] );
							m_pArith->l_sub ( F203_17A, F203_17A, ( *m_pSnTanaData )->Sn_mnz624[0] );
							m_pArith->l_sub ( F203_18A, F203_18A, ( *m_pSnTanaData )->Sn_mnz624[2] );
						}
						else {
							m_pArith->l_add ( F203_17B, F203_17B, ( *m_pSnTanaData )->Sn_mnz78[0] );
							m_pArith->l_add ( F203_18B, F203_18B, ( *m_pSnTanaData )->Sn_mnz78[2] );
							m_pArith->l_add ( F203_17A, F203_17A, ( *m_pSnTanaData )->Sn_mnz624[0] );
							m_pArith->l_add ( F203_18A, F203_18A, ( *m_pSnTanaData )->Sn_mnz624[2] );
						}
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F203_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F203_14B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F203_17A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F203_17B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F203_18A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F203_18B );
				}
				else {
					memset ( F203_14A, '\0', MONY_BUF_SIZE );
					memset ( F203_14B, '\0', MONY_BUF_SIZE );
					
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F203_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F203_14B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F203_17A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F203_17B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F203_18A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F203_18B );
				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_03();
				SetItemPosition();
			}
			else if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01 ) ) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_03();
				SetItemPosition();
			}
		}
	}
	else {
		CChangeTax2	CTDiag;
		CTDiag.InitInfo ( ( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );

		// 比例配分
		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle ( g_hInstance);
		int st = ( int )CTDiag.DoModal();
		AfxSetResourceHandle ( svhInstResource );

		if ( st == IDOK ) {

			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80) {	// 非連動

				// (免税)控除対象調整税額
				if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x10 ) {
					memmove ( F203_14A, ( *m_pSnTanaData )->Sn_MNZ624, MONY_BUF_SIZE );
					memmove ( F203_14B, ( *m_pSnTanaData )->Sn_MNZ78, MONY_BUF_SIZE );
					if ( ( *m_pSnTanaData )->Sn_MENZsw & 0x01 ) {
						m_pArith->l_neg ( F203_14A );
						m_pArith->l_neg ( F203_14B );
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F203_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F203_14B );
				}
				else {
					memset ( F203_14A, '\0', MONY_BUF_SIZE );
					memset ( F203_14B, '\0', MONY_BUF_SIZE );

					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_03();
				SetItemPosition();
			}
			else if ( !( ( *m_pSnTanaData )->Sn_tansw & 0x01 ) ) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_03();
				SetItemPosition();

			}
		}
	}
}

//=========================================================
// 特定収入の簡易計算式表示
//=========================================================
void CShinFhyo2_03::OnBnClickedButton2()
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
void CShinFhyo2_03::f203calq_spc ( SYZTOSPC_DATA SpcData )
{
	if ( *( m_pShinInfo->pSgSpc ) == 1 ) {
		unsigned char sign = ( *m_pSnHeadData )->Sn_Sign2;
		MoneyBasejagArray	money;
		int cnt = 0;
		int incnt = 0;
		unsigned short uwri = 0;

		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			uwri = F203_8C;
		}
		else {
			uwri = F203_8EX;
		}

		SYZTOSPC_DATA SyzToSpc = { 0 };
		memmove ( &SyzToSpc, &SpcData, sizeof ( SYZTOSPC_DATA ) );

		BOOL IsRatioOver95 = FALSE;
		BOOL Kobetsu = FALSE;

		//if ( F203_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
		if ( uwri >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
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

		m_pArith->l_add ( SyzToSpc.SpMzur, F203_2C, F203_3C );
		memmove ( SyzToSpc.SpHkur, F203_6C, sizeof ( SyzToSpc.SpHkur ) );

		memmove ( SyzToSpc.SpKuws, F203_4C, MONY_BUF_SIZE );
		memmove ( SyzToSpc.SpKuwb, F203_7C, MONY_BUF_SIZE );

		// 課税売上割合	
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "323300" ) );
		memmove ( SyzToSpc.SpUwri, money[0][0].arith, MONY_BUF_SIZE );


		// 控除対象仕入税額セット
		//if ( IsRatioOver95 ) {
		// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
		if ( IsRatioOver95 && ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) ) {
			// 計算表５（１）
			memmove ( SyzToSpc.SpKgz624, F203_16A, sizeof ( SyzToSpc.SpKgz624 ) );
			memmove ( SyzToSpc.SpKgz78, F203_16B, sizeof ( SyzToSpc.SpKgz78 ) );
		}
		else {
			if ( Kobetsu ) {
				// 計算表５（２）
				memmove ( SyzToSpc.SpKgz624, F203_19A, sizeof ( SyzToSpc.SpKgz624 ) );
				memmove ( SyzToSpc.SpKgz78, F203_19B, sizeof ( SyzToSpc.SpKgz78 ) );
			}
			else {
				// 計算表５（３）
				memmove ( SyzToSpc.SpKgz624, F203_20A, sizeof ( SyzToSpc.SpKgz624 ) );
				memmove ( SyzToSpc.SpKgz78, F203_20B, sizeof ( SyzToSpc.SpKgz78 ) );
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
				// 付表1-3 4E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), zero );
			}
			else {
				// 25E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), zero );
				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), mony );
				// 付表1-3 4E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), mony );
			}

			// 特定収入連動メッセージ表示
			m_Fhyo2_03Diag02.ModifyItem ( ID203_TS, DIAG_MDFY_OPAQUE );

			// 簡易計算式ボタン表示
			m_Button2.ShowWindow ( SW_SHOW );

			for ( int index = ID203_23A; index <= ID203_23B; index++ ) {
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

				F203_23c &= ~0x80;
				F203_23c &= ~0x800;

				// 特定収入連動メッセージ非表示
				m_Fhyo2_03Diag02.ModifyItem ( ID203_TS, DIAG_MDFY_TRANSPARENT );

				// 簡易計算式ボタン非表示
				m_Button2.ShowWindow ( SW_HIDE );

				m_SpcSgn = FALSE;

				for ( int index = ID203_23A; index <= ID203_23B; index++ ) {
					ATRIB_MOD ( IDC_ICSDIAG8CTRL2, index, 1, ( *m_pSnHeadData )->Sn_Sign4, 0 );
				}
			}
			else {
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
				if ( money[l][m].code == _T( "324802" ) ) {
					memmove ( F203_23A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324803" ) ) {
					memmove ( F203_23B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324800" ) ) {
					memmove ( F203_23C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324902" ) ) {
					memmove ( F203_24A, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324903" ) ) {
					memmove ( F203_24B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324900" ) ) {
					memmove ( F203_24C, money[l][m].arith, MONY_BUF_SIZE );
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
BOOL CShinFhyo2_03::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F203_10c & 0x880 ) || ( F203_12c & 0x800 ) || ( F203_19c & 0x880 ) ||
			 ( F203_20c & 0x880 ) || ( F203_23c & 0x880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}