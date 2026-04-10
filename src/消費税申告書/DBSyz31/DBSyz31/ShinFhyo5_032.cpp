// ShinFhyo5_032.cpp : 実装ファイル
//

//#include "pch.h"
#include "stdafx.h"
#include "ShinFhyo5_032.h"
#include "ShinFhyo5_032idx.h"
#include "H31HyoView.h"
//#include "afxdialogex.h"


// ShinFhyo5_032 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo5_032, CSyzBaseDlg)

CShinFhyo5_032::CShinFhyo5_032(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo5_032::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo5_012Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pSyzSyukei( NULL )
{
	memset ( F5032_20A, '\0', sizeof ( F5032_20A ) );
	memset ( F5032_20B, '\0', sizeof ( F5032_20B ) );
	memset ( F5032_20C, '\0', sizeof ( F5032_20C ) );

	memset ( F5032_21A, '\0', sizeof ( F5032_21A ) );
	memset ( F5032_21B, '\0', sizeof ( F5032_21B ) );
	memset ( F5032_21C, '\0', sizeof ( F5032_21C ) );

	memset ( F5032_22A, '\0', sizeof ( F5032_22A ) );
	memset ( F5032_22B, '\0', sizeof ( F5032_22B ) );
	memset ( F5032_22C, '\0', sizeof ( F5032_22C ) );

	memset ( F5032_23A, '\0', sizeof ( F5032_23A ) );
	memset ( F5032_23B, '\0', sizeof ( F5032_23B ) );
	memset ( F5032_23C, '\0', sizeof ( F5032_23C ) );

	memset ( F5032_24A, '\0', sizeof ( F5032_24A ) );
	memset ( F5032_24B, '\0', sizeof ( F5032_24B ) );
	memset ( F5032_24C, '\0', sizeof ( F5032_24C ) );

	memset ( F5032_25A, '\0', sizeof ( F5032_25A ) );
	memset ( F5032_25B, '\0', sizeof ( F5032_25B ) );
	memset ( F5032_25C, '\0', sizeof ( F5032_25C ) );

	memset ( F5032_26A, '\0', sizeof ( F5032_26A ) );
	memset ( F5032_26B, '\0', sizeof ( F5032_26B ) );
	memset ( F5032_26C, '\0', sizeof ( F5032_26C ) );

	memset ( F5032_27A, '\0', sizeof ( F5032_27A ) );
	memset ( F5032_27B, '\0', sizeof ( F5032_27B ) );
	memset ( F5032_27C, '\0', sizeof ( F5032_27C ) );

	memset ( F5032_28A, '\0', sizeof ( F5032_28A ) );
	memset ( F5032_28B, '\0', sizeof ( F5032_28B ) );
	memset ( F5032_28C, '\0', sizeof ( F5032_28C ) );

	memset ( F5032_29A, '\0', sizeof ( F5032_29A ) );
	memset ( F5032_29B, '\0', sizeof ( F5032_29B ) );
	memset ( F5032_29C, '\0', sizeof ( F5032_29C ) );

	memset ( F5032_30A, '\0', sizeof ( F5032_30A ) );
	memset ( F5032_30B, '\0', sizeof ( F5032_30B ) );
	memset ( F5032_30C, '\0', sizeof ( F5032_30C ) );

	memset ( F5032_31A, '\0', sizeof ( F5032_31A ) );
	memset ( F5032_31B, '\0', sizeof ( F5032_31B ) );
	memset ( F5032_31C, '\0', sizeof ( F5032_31C ) );

	memset ( F5032_32A, '\0', sizeof ( F5032_32A ) );
	memset ( F5032_32B, '\0', sizeof ( F5032_32B ) );
	memset ( F5032_32C, '\0', sizeof ( F5032_32C ) );

	memset ( F5032_33A, '\0', sizeof ( F5032_33A ) );
	memset ( F5032_33B, '\0', sizeof ( F5032_33B ) );
	memset ( F5032_33C, '\0', sizeof ( F5032_33C ) );

	memset ( F5032_34A, '\0', sizeof ( F5032_34A ) );
	memset ( F5032_34B, '\0', sizeof ( F5032_34B ) );
	memset ( F5032_34C, '\0', sizeof ( F5032_34C ) );

	memset ( F5032_35A, '\0', sizeof ( F5032_35A ) );
	memset ( F5032_35B, '\0', sizeof ( F5032_35B ) );
	memset ( F5032_35C, '\0', sizeof ( F5032_35C ) );

	memset ( F5032_36A, '\0', sizeof ( F5032_36A ) );
	memset ( F5032_36B, '\0', sizeof ( F5032_36B ) );
	memset ( F5032_36C, '\0', sizeof ( F5032_36C ) );

	memset ( F5032_37A, '\0', sizeof ( F5032_37A ) );
	memset ( F5032_37B, '\0', sizeof ( F5032_37B ) );
	memset ( F5032_37C, '\0', sizeof ( F5032_37C ) );
	F5032_21kbn = 0;

	F5032_20c = F5032_21c = F5032_22c = F5032_23c = F5032_24c = F5032_25c = F5032_26c = F5032_27c = F5032_28c = F5032_29c = 0;
	F5032_30c = F5032_31c = F5032_32c = F5032_32c = F5032_33c = F5032_34c = F5032_35c = F5032_36c = 0;

	F5032_21Disp = F5032_22Disp = F5032_23Disp = F5032_24Disp = F5032_25Disp = F5032_26Disp = F5032_27Disp = F5032_28Disp = F5032_29Disp = 0;
	F5032_30Disp = F5032_31Disp = F5032_32Disp = F5032_33Disp = F5032_34Disp = F5032_35Disp = F5032_36Disp = 0;

	m_id = 0;
	m_typecnt = 0;
	m_flg = 0;
	m_4val = 0;
	m_PrintId = 0;

	m_curwnd = -1;

}

CShinFhyo5_032::~CShinFhyo5_032()
{
}

void CShinFhyo5_032::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo5_032Diag01);
}


BEGIN_MESSAGE_MAP(CShinFhyo5_032, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo5_032, CSyzBaseDlg)
	ON_EVENT(CShinFhyo5_032, IDC_ICSDIAG8CTRL1, 1, CShinFhyo5_032::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinFhyo5_032, IDC_ICSDIAG8CTRL1, 2, CShinFhyo5_032::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinFhyo5_032, IDC_ICSDIAG8CTRL1, 3, CShinFhyo5_032::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// ShinFhyo5_032 メッセージ ハンドラー


BOOL CShinFhyo5_032::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL1 );
	m_BottomIdx = 206;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo5_032::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo5_032::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo5_032::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BOOL CShinFhyo5_032::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			GetAllDiagData();
			WriteData();
			if ( m_pParent ) {
				m_pParent->PostMessageA ( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}

		if ( pMsg->wParam == VK_F2 ) {
			DIAGRAM_DATA dd;
			if ( m_curwnd != -1 ) {
				short result = ( ( CIcsdiagctrl* )GetDlgItem ( m_curwnd ) )->GetPosition();
				diag_getdata ( m_curwnd, result, &dd, CIcsdiagctrl );
				if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
					TerminationIcsdiag8ctrl1 ( result, ( short )pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}

				return TRUE;
			}
		}
	}
	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


void CShinFhyo5_032::EditONIcsdiag8ctrl1(short index)
{
	m_curwnd = IDC_ICSDIAG8CTRL1;
}

void CShinFhyo5_032::EditOFFIcsdiag8ctrl1(short index)
{
	switch ( index ) {
		// 原則計算を適用する場合
	case ID5032_20A:
	case ID5032_20B:
	case ID5032_20C:
		SyukeiDataSet ( index );
		f5032data_all ( 1 );
		break;

		// 特例計算を適用する場合
	case ID5032_21A:
	case ID5032_21B:
	case ID5032_21C:
		SyukeiDataSet ( index );
		f5032data_all ( 2 );
		break;


	case ID5032_22A:
	case ID5032_22B:
	case ID5032_22C:
		SyukeiDataSet ( index );
		f5032data_all ( 3 );
		break;

	case ID5032_23A:
	case ID5032_23B:
	case ID5032_23C:
		SyukeiDataSet ( index );
		f5032data_all ( 4 );
		break;

	case ID5032_24A:
	case ID5032_24B:
	case ID5032_24C:
		SyukeiDataSet ( index );
		f5032data_all ( 5 );
		break;

	case ID5032_25A:
	case ID5032_25B:
	case ID5032_25C:
		SyukeiDataSet ( index );
		f5032data_all ( 6 );
		break;

	case ID5032_26A:
	case ID5032_26B:
	case ID5032_26C:
		SyukeiDataSet ( index );
		f5032data_all ( 7 );
		break;

	case ID5032_27A:
	case ID5032_27B:
	case ID5032_27C:
		SyukeiDataSet ( index );
		f5032data_all ( 8 );
		break;

	case ID5032_28A:
	case ID5032_28B:
	case ID5032_28C:
		SyukeiDataSet ( index );
		f5032data_all ( 9 );
		break;

	case ID5032_29A:
	case ID5032_29B:
	case ID5032_29C:
		SyukeiDataSet ( index );
		f5032data_all ( 10 );
		break;

	case ID5032_30A:
	case ID5032_30B:
	case ID5032_30C:
		SyukeiDataSet ( index );
		f5032data_all ( 11 );
		break;

	case ID5032_31A:
	case ID5032_31B:
	case ID5032_31C:
		SyukeiDataSet ( index );
		f5032data_all ( 12 );
		break;

	case ID5032_32A:
	case ID5032_32B:
	case ID5032_32C:
		SyukeiDataSet ( index );
		f5032data_all ( 13 );
		break;

	case ID5032_33A:
	case ID5032_33B:
	case ID5032_33C:
		SyukeiDataSet ( index );
		f5032data_all ( 14 );
		break;

	case ID5032_34A:
	case ID5032_34B:
	case ID5032_34C:
		SyukeiDataSet ( index );
		f5032data_all ( 15 );
		break;

	case ID5032_35A:
	case ID5032_35B:
	case ID5032_35C:
		SyukeiDataSet ( index );
		f5032data_all ( 16 );
		break;

	case ID5032_36A:
	case ID5032_36B:
	case ID5032_36C:
		SyukeiDataSet ( index );
		f5032data_all ( 17 );
		break;

	default:
		break;
	}
}

void CShinFhyo5_032::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	if ( nChar == 0 ) {
		return;
	}

	short wTerm = 0;
	char VK_FLG = 0x00;

	// Shift+TabをTabに、TabをEnterに変換
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	int pos = 0;
	if ( wTerm == VK_UP ) {
		pos = UpDownFocusMoveCheck( index, 1 );
		m_Fhyo5_032Diag01.SetPosition( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck( index, 0 );
		m_Fhyo5_032Diag01.SetPosition( pos );
	}

	if ( (wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3) ) {
		m_Fhyo5_032Diag01.SetNextPosition();
	}
	if ( (wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2) ) {
		m_Fhyo5_032Diag01.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
		case ID5032_20A:
			F5032_20c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359002" ) );
			// 各入力の表示
			f5032data_all ( 1 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_20A );
			break;
		case ID5032_20B:
			F5032_20c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359003" ) );
			// 各入力の表示
			f5032data_all ( 1 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_20B );
			break;
		case ID5032_20C:
			F5032_20c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359000" ) );
			// 各入力の表示
			f5032data_all ( 1 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_20C );
			break;

		case ID5032_21A:
			F5032_21c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359102" ) );
			// 各入力の表示
			f5032data_all ( 2 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_21A );
			break;
		case ID5032_21B:
			F5032_21c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359103" ) );
			// 各入力の表示
			f5032data_all ( 2 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_21B );
			break;
		case ID5032_21C:
			F5032_21c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359100" ) );
			// 各入力の表示
			f5032data_all ( 2 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_21C );
			break;

		case ID5032_22A:
			F5032_22c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359202" ) );
			// 各入力の表示
			f5032data_all ( 3 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_22A );
			break;
		case ID5032_22B:
			F5032_22c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359203" ) );
			// 各入力の表示
			f5032data_all ( 3 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_22B );
			break;
		case ID5032_22C:
			F5032_22c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359200" ) );
			
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E001" ) );
			// 各入力の表示
			f5032data_all ( 3 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_22C );
			break;

		case ID5032_23A:
			F5032_23c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359302" ) );
			// 各入力の表示
			f5032data_all ( 4 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_23A );
			break;
		case ID5032_23B:
			F5032_23c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359303" ) );
			// 各入力の表示
			f5032data_all ( 4 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_23B );
			break;
		case ID5032_23C:
			F5032_23c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359300" ) );
			
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E002" ) );
			// 各入力の表示
			f5032data_all ( 4 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_23C );
			break;

		case ID5032_24A:
			F5032_24c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359402" ) );
			// 各入力の表示
			f5032data_all ( 5 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_24A );
			break;
		case ID5032_24B:
			F5032_24c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359403" ) );
			// 各入力の表示
			f5032data_all ( 5 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_24B );
			break;
		case ID5032_24C:
			F5032_24c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359400" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E003" ) );
			// 各入力の表示
			f5032data_all ( 5 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_24C );
			break;

		case ID5032_25A:
			F5032_25c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359502" ) );
			// 各入力の表示
			f5032data_all ( 6 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_25A );
			break;
		case ID5032_25B:
			F5032_25c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359503" ) );
			// 各入力の表示
			f5032data_all ( 6 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_25B );
			break;
		case ID5032_25C:
			F5032_25c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359500" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E004" ) );
			// 各入力の表示
			f5032data_all ( 6 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_25C );
			break;

		case ID5032_26A:
			F5032_26c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359602" ) );
			// 各入力の表示
			f5032data_all ( 7 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_26A );
			break;
		case ID5032_26B:
			F5032_26c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359603" ) );
			// 各入力の表示
			f5032data_all ( 7 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_26B );
			break;
		case ID5032_26C:
			F5032_26c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359600" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E005" ) );
			// 各入力の表示
			f5032data_all ( 7 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_26C );
			break;

		case ID5032_27A:
			F5032_27c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359702" ) );
			// 各入力の表示
			f5032data_all ( 8 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_27A );
			break;
		case ID5032_27B:
			F5032_27c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359703" ) );
			// 各入力の表示
			f5032data_all ( 8 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_27B );
			break;
		case ID5032_27C:
			F5032_27c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359700" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E006" ) );
			// 各入力の表示
			f5032data_all ( 8 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_27C );
			break;

		case ID5032_28A:
			F5032_28c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359802" ) );
			// 各入力の表示
			f5032data_all ( 9 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_28A );
			break;
		case ID5032_28B:
			F5032_28c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359803" ) );
			// 各入力の表示
			f5032data_all ( 9 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_28B );
			break;
		case ID5032_28C:
			F5032_28c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359800" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E007" ) );
			// 各入力の表示
			f5032data_all ( 9 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_28C );
			break;

		case ID5032_29A:
			F5032_29c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359902" ) );
			// 各入力の表示
			f5032data_all ( 10 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_29A );
			break;
		case ID5032_29B:
			F5032_29c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359903" ) );
			// 各入力の表示
			f5032data_all ( 10 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_29B );
			break;
		case ID5032_29C:
			F5032_29c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359900" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E008" ) );
			// 各入力の表示
			f5032data_all ( 10 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_29C );
			break;

		case ID5032_30A:
			F5032_30c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A002" ) );
			// 各入力の表示
			f5032data_all ( 11 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_30A );
			break;
		case ID5032_30B:
			F5032_30c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A003" ) );
			// 各入力の表示
			f5032data_all ( 11 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_30B );
			break;
		case ID5032_30C:
			F5032_30c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A000" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E009" ) );
			// 各入力の表示
			f5032data_all ( 11 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_30C );
			break;

		case ID5032_31A:
			F5032_31c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A102" ) );
			// 各入力の表示
			f5032data_all ( 12 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_31A );
			break;
		case ID5032_31B:
			F5032_31c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A103" ) );
			// 各入力の表示
			f5032data_all ( 12 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_31B );
			break;
		case ID5032_31C:
			F5032_31c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A100" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00A" ) );
			// 各入力の表示
			f5032data_all ( 12 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_31C );
			break;

		case ID5032_32A:
			F5032_32c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A202" ) );
			// 各入力の表示
			f5032data_all ( 13 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_32A );
			break;
		case ID5032_32B:
			//F5032_20c &= ~0x800;
			F5032_32c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A203" ) );
			// 各入力の表示
			f5032data_all ( 13 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_32B );
			break;
		case ID5032_32C:
			F5032_32c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A200" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00B" ) );
			// 各入力の表示
			f5032data_all ( 13 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_32C );
			break;

		case ID5032_33A:
			F5032_33c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A302" ) );
			// 各入力の表示
			f5032data_all ( 14 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_33A );
			break;
		case ID5032_33B:
			F5032_33c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A303" ) );
			// 各入力の表示
			f5032data_all ( 14 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_33B );
			break;
		case ID5032_33C:
			F5032_33c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A300" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00C" ) );
			// 各入力の表示
			f5032data_all ( 14 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_33C );
			break;

		case ID5032_34A:
			F5032_34c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A402" ) );
			// 各入力の表示
			f5032data_all ( 15 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_34A );
			break;
		case ID5032_34B:
			F5032_34c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A403" ) );
			// 各入力の表示
			f5032data_all ( 15 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_34B );
			break;
		case ID5032_34C:
			F5032_34c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A400" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00D" ) );
			// 各入力の表示
			f5032data_all ( 15 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_34C );
			break;

		case ID5032_35A:
			F5032_35c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A502" ) );
			// 各入力の表示
			f5032data_all ( 16 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_35A );
			break;
		case ID5032_35B:
			F5032_35c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A503" ) );
			// 各入力の表示
			f5032data_all ( 16 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_35B );
			break;
		case ID5032_35C:
			F5032_35c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A500" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00E" ) );
			// 各入力の表示
			f5032data_all ( 16 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_35C );
			break;

		case ID5032_36A:
			F5032_36c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A602" ) );
			// 各入力の表示
			f5032data_all ( 17 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_36A );
			break;
		case ID5032_36B:
			F5032_36c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A603" ) );
			// 各入力の表示
			f5032data_all ( 17 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_36B );
			break;
		case ID5032_36C:
			F5032_36c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A600" ) );
			//( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00F" ) );
			// 各入力の表示
			f5032data_all ( 17 );
			m_Fhyo5_032Diag01.SetPosition ( ID5032_36C );
			break;

		default :
			dsp_cls ( IDC_ICSDIAG8CTRL1, index );
			break;
		}

		ClearInput ( index );
		m_Fhyo5_032Diag01.SetPosition( index );
	}


	int nowpos = m_Fhyo5_032Diag01.GetPosition();

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}


//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo5_012Data：	付表5-1(2)データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSyzSyukei		：	集計クラス
//		pArith			：	倍長演算クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_032::InitInfo ( CH31SnFhyo5_012Data **pSnFhyo5_012Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith )
{
	if ( pSnFhyo5_012Data == NULL ) {
		return -1;
	}
	m_pSnFhyo5_012Data = pSnFhyo5_012Data;

	if ( pSnHeadData == NULL ) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

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
int CShinFhyo5_032::Init()
{
	// データ読込み
	ReadData();
	ReadData2();

	// 集計
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo5_032Diag01.ModifyItem ( 264, DIAG_MDFY_TRANSPARENT );
	}

	// 画面表示
	DispFh5_032();

	// ポジションセット
	SetItemPosition();

	return 0;
}

//=========================================================
// データ読込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo5_032::ReadData()
{
	// 4×みなし仕入率
	memmove ( F5032_20A, ( *m_pSnFhyo5_012Data )->Sn_F5012_20D, MONY_BUF_SIZE );
	memmove ( F5032_20B, ( *m_pSnFhyo5_012Data )->Sn_F5012_20E, MONY_BUF_SIZE );
	memmove ( F5032_20C, ( *m_pSnFhyo5_012Data )->Sn_F5012_20F, MONY_BUF_SIZE );
	F5032_20c = ( *m_pSnFhyo5_012Data )->Sn_F5012_20sw;

	// 4×みなし仕入率
	memmove ( F5032_21A, ( *m_pSnFhyo5_012Data )->Sn_F5012_21D, MONY_BUF_SIZE );
	memmove ( F5032_21B, ( *m_pSnFhyo5_012Data )->Sn_F5012_21E, MONY_BUF_SIZE );
	memmove ( F5032_21C, ( *m_pSnFhyo5_012Data )->Sn_F5012_21F, MONY_BUF_SIZE );
	F5032_21c = ( *m_pSnFhyo5_012Data )->Sn_F5012_21sw;

	// 第一種事業及び第二種事業
	memmove ( F5032_22A, ( *m_pSnFhyo5_012Data )->Sn_F5012_22D, MONY_BUF_SIZE );
	memmove ( F5032_22B, ( *m_pSnFhyo5_012Data )->Sn_F5012_22E, MONY_BUF_SIZE );
	memmove ( F5032_22C, ( *m_pSnFhyo5_012Data )->Sn_F5012_22F, MONY_BUF_SIZE );
	F5032_22c = ( *m_pSnFhyo5_012Data )->Sn_F5012_22sw;

	// 第一種事業及び第三種事業
	memmove ( F5032_23A, ( *m_pSnFhyo5_012Data )->Sn_F5012_23D, MONY_BUF_SIZE );
	memmove ( F5032_23B, ( *m_pSnFhyo5_012Data )->Sn_F5012_23E, MONY_BUF_SIZE );
	memmove ( F5032_23C, ( *m_pSnFhyo5_012Data )->Sn_F5012_23F, MONY_BUF_SIZE );
	F5032_23c = ( *m_pSnFhyo5_012Data )->Sn_F5012_23sw;

	// 第一種事業及び第四種事業
	memmove ( F5032_24A, ( *m_pSnFhyo5_012Data )->Sn_F5012_24D, MONY_BUF_SIZE );
	memmove ( F5032_24B, ( *m_pSnFhyo5_012Data )->Sn_F5012_24E, MONY_BUF_SIZE );
	memmove ( F5032_24C, ( *m_pSnFhyo5_012Data )->Sn_F5012_24F, MONY_BUF_SIZE );
	F5032_24c = ( *m_pSnFhyo5_012Data )->Sn_F5012_24sw;

	// 第一種事業及び第五種事業
	memmove ( F5032_25A, ( *m_pSnFhyo5_012Data )->Sn_F5012_25D, MONY_BUF_SIZE );
	memmove ( F5032_25B, ( *m_pSnFhyo5_012Data )->Sn_F5012_25E, MONY_BUF_SIZE );
	memmove ( F5032_25C, ( *m_pSnFhyo5_012Data )->Sn_F5012_25F, MONY_BUF_SIZE );
	F5032_25c = ( *m_pSnFhyo5_012Data )->Sn_F5012_25sw;

	// 第一種事業及び第六種事業
	memmove ( F5032_26A, ( *m_pSnFhyo5_012Data )->Sn_F5012_26D, MONY_BUF_SIZE );
	memmove ( F5032_26B, ( *m_pSnFhyo5_012Data )->Sn_F5012_26E, MONY_BUF_SIZE );
	memmove ( F5032_26C, ( *m_pSnFhyo5_012Data )->Sn_F5012_26F, MONY_BUF_SIZE );
	F5032_26c = ( *m_pSnFhyo5_012Data )->Sn_F5012_26sw;

	// 第二種事業及び第三種事業
	memmove ( F5032_27A, ( *m_pSnFhyo5_012Data )->Sn_F5012_27D, MONY_BUF_SIZE );
	memmove ( F5032_27B, ( *m_pSnFhyo5_012Data )->Sn_F5012_27E, MONY_BUF_SIZE );
	memmove ( F5032_27C, ( *m_pSnFhyo5_012Data )->Sn_F5012_27F, MONY_BUF_SIZE );
	F5032_27c = ( *m_pSnFhyo5_012Data )->Sn_F5012_27sw;

	// 第二種事業及び第四種事業
	memmove ( F5032_28A, ( *m_pSnFhyo5_012Data )->Sn_F5012_28D, MONY_BUF_SIZE );
	memmove ( F5032_28B, ( *m_pSnFhyo5_012Data )->Sn_F5012_28E, MONY_BUF_SIZE );
	memmove ( F5032_28C, ( *m_pSnFhyo5_012Data )->Sn_F5012_28F, MONY_BUF_SIZE );
	F5032_28c = ( *m_pSnFhyo5_012Data )->Sn_F5012_28sw;

	// 第二種事業及び第五種事業
	memmove ( F5032_29A, ( *m_pSnFhyo5_012Data )->Sn_F5012_29D, MONY_BUF_SIZE );
	memmove ( F5032_29B, ( *m_pSnFhyo5_012Data )->Sn_F5012_29E, MONY_BUF_SIZE );
	memmove ( F5032_29C, ( *m_pSnFhyo5_012Data )->Sn_F5012_29F, MONY_BUF_SIZE );
	F5032_29c = ( *m_pSnFhyo5_012Data )->Sn_F5012_29sw;

	// 第二種事業及び第六種事業
	memmove ( F5032_30A, ( *m_pSnFhyo5_012Data )->Sn_F5012_30D, MONY_BUF_SIZE );
	memmove ( F5032_30B, ( *m_pSnFhyo5_012Data )->Sn_F5012_30E, MONY_BUF_SIZE );
	memmove ( F5032_30C, ( *m_pSnFhyo5_012Data )->Sn_F5012_30F, MONY_BUF_SIZE );
	F5032_30c = ( *m_pSnFhyo5_012Data )->Sn_F5012_30sw;

	// 第三種事業及び第四種事業
	memmove ( F5032_31A, ( *m_pSnFhyo5_012Data )->Sn_F5012_31D, MONY_BUF_SIZE );
	memmove ( F5032_31B, ( *m_pSnFhyo5_012Data )->Sn_F5012_31E, MONY_BUF_SIZE );
	memmove ( F5032_31C, ( *m_pSnFhyo5_012Data )->Sn_F5012_31F, MONY_BUF_SIZE );
	F5032_31c = ( *m_pSnFhyo5_012Data )->Sn_F5012_31sw;

	// 第三種事業及び第五種事業
	memmove ( F5032_32A, ( *m_pSnFhyo5_012Data )->Sn_F5012_32D, MONY_BUF_SIZE );
	memmove ( F5032_32B, ( *m_pSnFhyo5_012Data )->Sn_F5012_32E, MONY_BUF_SIZE );
	memmove ( F5032_32C, ( *m_pSnFhyo5_012Data )->Sn_F5012_32F, MONY_BUF_SIZE );
	F5032_32c = ( *m_pSnFhyo5_012Data )->Sn_F5012_32sw;

	// 第三種事業及び第六種事業
	memmove ( F5032_33A, ( *m_pSnFhyo5_012Data )->Sn_F5012_33D, MONY_BUF_SIZE );
	memmove ( F5032_33B, ( *m_pSnFhyo5_012Data )->Sn_F5012_33E, MONY_BUF_SIZE );
	memmove ( F5032_33C, ( *m_pSnFhyo5_012Data )->Sn_F5012_33F, MONY_BUF_SIZE );
	F5032_33c = ( *m_pSnFhyo5_012Data )->Sn_F5012_33sw;

	// 第四種事業及び第五種事業
	memmove ( F5032_34A, ( *m_pSnFhyo5_012Data )->Sn_F5012_34D, MONY_BUF_SIZE );
	memmove ( F5032_34B, ( *m_pSnFhyo5_012Data )->Sn_F5012_34E, MONY_BUF_SIZE );
	memmove ( F5032_34C, ( *m_pSnFhyo5_012Data )->Sn_F5012_34F, MONY_BUF_SIZE );
	F5032_34c = ( *m_pSnFhyo5_012Data )->Sn_F5012_34sw;

	// 第四種事業及び第六種事業
	memmove ( F5032_35A, ( *m_pSnFhyo5_012Data )->Sn_F5012_35D, MONY_BUF_SIZE );
	memmove ( F5032_35B, ( *m_pSnFhyo5_012Data )->Sn_F5012_35E, MONY_BUF_SIZE );
	memmove ( F5032_35C, ( *m_pSnFhyo5_012Data )->Sn_F5012_35F, MONY_BUF_SIZE );
	F5032_35c = ( *m_pSnFhyo5_012Data )->Sn_F5012_35sw;

	// 第五種事業及び第六種事業
	memmove ( F5032_36A, ( *m_pSnFhyo5_012Data )->Sn_F5012_36D, MONY_BUF_SIZE );
	memmove ( F5032_36B, ( *m_pSnFhyo5_012Data )->Sn_F5012_36E, MONY_BUF_SIZE );
	memmove ( F5032_36C, ( *m_pSnFhyo5_012Data )->Sn_F5012_36F, MONY_BUF_SIZE );
	F5032_36c = ( *m_pSnFhyo5_012Data )->Sn_F5012_36sw;

	// 選択可能な計算式区分の内から選択した金額
	memmove ( F5032_37A, ( *m_pSnFhyo5_012Data )->Sn_F5012_37D, MONY_BUF_SIZE );
	memmove ( F5032_37B, ( *m_pSnFhyo5_012Data )->Sn_F5012_37E, MONY_BUF_SIZE );
	memmove ( F5032_37C, ( *m_pSnFhyo5_012Data )->Sn_F5012_37F, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo5_032::ReadData2()
{
	// 旧税率は全てクリア
	char mony[MONY_BUF_SIZE] = { 0 };
	memset(mony, '\0', MONY_BUF_SIZE);

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359001" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "359901" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A001" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A701" ), mony );

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		if ( F5032_20c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359002" ), F5032_20A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359002" ));
		}
		if ( F5032_20c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359003" ), F5032_20B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359003" ) );
		}
		if ( F5032_20c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359000" ), F5032_20C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359000" ) );
		}
		if ( F5032_21c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359102" ), F5032_21A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359102" ) );
		}
		if ( F5032_21c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359103" ), F5032_21B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359103" ) );
		}
		if ( F5032_21c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359100" ), F5032_21C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359100" ) );
		}
		if ( F5032_22c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359202" ), F5032_22A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359202" ) );
		}
		if ( F5032_22c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359203" ), F5032_22B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359203" ) );
		}
		if ( F5032_22c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359200" ), F5032_22C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359200" ) );
		}
		if ( F5032_23c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359302" ), F5032_23A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359302" ) );
		}
		if ( F5032_23c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359303" ), F5032_23B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359303" ) );
		}
		if ( F5032_23c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359300" ), F5032_23C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359300" ) );
		}
		if ( F5032_24c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359402" ), F5032_24A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359402" ) );
		}
		if ( F5032_24c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359403" ), F5032_24B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359403" ) );
		}
		if ( F5032_24c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359400" ), F5032_24C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359400" ) );
		}
		if ( F5032_25c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359502" ), F5032_25A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359502" ) );
		}
		if ( F5032_25c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359503" ), F5032_25B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359503" ) );
		}
		if ( F5032_25c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359500" ), F5032_25C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359500" ) );
		}
		if ( F5032_26c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359602" ), F5032_26A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359602" ) );
		}
		if ( F5032_26c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359603" ), F5032_26B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359603" ) );
		}
		if ( F5032_26c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359600" ), F5032_26C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359600" ) );
		}
		if ( F5032_27c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359702" ), F5032_27A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359702" ) );
		}
		if ( F5032_27c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359703" ), F5032_27B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359703" ) );
		}
		if ( F5032_27c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359700" ), F5032_27C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359700" ) );
		}
		if ( F5032_28c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359802" ), F5032_28A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359802" ) );
		}
		if ( F5032_28c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359803" ), F5032_28B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359803" ) );
		}
		if ( F5032_28c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359800" ), F5032_28C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359800" ) );
		}
		if ( F5032_29c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359902" ), F5032_29A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359902" ) );
		}
		if ( F5032_29c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359903" ), F5032_29B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359903" ) );
		}
		if ( F5032_29c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359900" ), F5032_29C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359900" ) );
		}
		if ( F5032_30c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A002" ), F5032_30A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A002" ) );
		}
		if ( F5032_30c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A003" ), F5032_30B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A003" ) );
		}
		if ( F5032_30c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A000" ), F5032_30C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A000" ) );
		}
		if ( F5032_31c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A102" ), F5032_31A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A102" ) );
		}
		if ( F5032_31c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A103" ), F5032_31B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A103" ) );
		}
		if ( F5032_31c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A100" ), F5032_31C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A100" ) );
		}
		if ( F5032_32c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A202" ), F5032_32A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A202" ) );
		}
		if ( F5032_32c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A203" ), F5032_32B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A203" ) );
		}
		if ( F5032_32c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A200" ), F5032_32C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A200" ) );
		}
		if ( F5032_33c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A302" ), F5032_33A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A302" ) );
		}
		if ( F5032_33c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A303" ), F5032_33B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A303" ) );
		}
		if ( F5032_33c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A300" ), F5032_33C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A300" ) );
		}
		if ( F5032_34c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A402" ), F5032_34A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A402" ) );
		}
		if ( F5032_34c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A403" ), F5032_34B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A403" ) );
		}
		if ( F5032_34c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A400" ), F5032_34C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A400" ) );
		}
		if ( F5032_35c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A502" ), F5032_35A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A502" ) );
		}
		if ( F5032_35c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A503" ), F5032_35B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A503" ) );
		}
		if ( F5032_35c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A500" ), F5032_35C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A500" ) );
		}
		if ( F5032_36c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A602" ), F5032_36A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A602" ) );
		}
		if ( F5032_36c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A603" ), F5032_36B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A603" ) );
		}
		if ( F5032_36c & 0x8000 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A600" ), F5032_36C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A600" ) );
		}

		if ( F5032_21c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E000" ), F5032_21C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E000" ) );
		}

		if ( F5032_22c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E001" ), F5032_22C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E001" ) );
		}

		if ( F5032_23c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E002" ), F5032_23C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E002" ) );
		}

		if ( F5032_24c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E003" ), F5032_24C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E003" ) );
		}

		if ( F5032_25c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E004" ), F5032_25C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E004" ) );
		}

		if ( F5032_26c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E005" ), F5032_26C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E005" ) );
		}

		if ( F5032_27c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E006" ), F5032_27C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E006" ) );
		}

		if ( F5032_28c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E007" ), F5032_28C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E007" ) );
		}

		if ( F5032_29c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E008" ), F5032_29C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E008" ) );
		}

		if ( F5032_30c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E009" ), F5032_30C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E009" ) );
		}

		if ( F5032_31c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00A" ), F5032_31C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00A" ) );
		}

		if ( F5032_32c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00B" ), F5032_32C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00B" ) );
		}

		if ( F5032_33c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00C" ), F5032_33C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00C" ) );
		}

		if ( F5032_34c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00D" ), F5032_34C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00D" ) );
		}

		if ( F5032_35c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00E" ), F5032_35C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00E" ) );
		}

		if ( F5032_36c & 0x8880 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00F" ), F5032_36C );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00F" ) );
		}
	}

	//if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) {
	//	SetKjCode();
	//}
}

//=========================================================
// データ書込み
//---------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
void CShinFhyo5_032::WriteData()
{
	char zerobuf[MONY_BUF_SIZE] = { 0 };

	IsEnableItems ( 1, m_id, m_code, m_typecnt );
	m_4val = Is4Val ( 0 );

	// 入力可能欄ならデータベースへ保存
	// 4×みなし仕入率
	if ( ( m_typecnt > 1 ) && ( m_4val ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20D, F5032_20A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20E, F5032_20B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20F, F5032_20C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_20sw = F5032_20c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_20F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_20sw = 0;
	}

	// 4×みなし仕入率
	//if ( m_id == ID5032_21C ) {
		if ( IsDispArea ( ID5032_21A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21D, F5032_21A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21E, F5032_21B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21F, F5032_21C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_21sw = F5032_21c;
		( *m_pSnFhyo5_012Data )->Sn_F5012_21kbn = F5032_21kbn;

	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_21F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_21sw = 0;
		( *m_pSnFhyo5_012Data )->Sn_F5012_21kbn = 0;
	}

	// 第一種事業及び第二種事業
	//if ( m_id == ID5032_22C ) {
	if ( IsDispArea ( ID5032_22A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22D, F5032_22A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22E, F5032_22B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22F, F5032_22C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_22sw = F5032_22c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_22F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_22sw = 0;
	}

	// 第一種事業及び第三種事業
	//if ( m_id == ID5032_23C ) {
	if ( IsDispArea ( ID5032_23A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23D, F5032_23A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23E, F5032_23B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23F, F5032_23C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_23sw = F5032_23c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_23F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_23sw = 0;
	}

	// 第一種事業及び第四種事業
	//if ( m_id == ID5032_24C ) {
	if ( IsDispArea ( ID5032_24A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24D, F5032_24A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24E, F5032_24B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24F, F5032_24C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_24sw = F5032_24c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_24F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_24sw = 0;
	}

	// 第一種事業及び第五種事業
	//if ( m_id == ID5032_25C ) {
	if ( IsDispArea ( ID5032_25A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25D, F5032_25A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25E, F5032_25B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25F, F5032_25C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_25sw = F5032_25c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_25F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_25sw = 0;
	}

	// 第一種事業及び第六種事業
	//if ( m_id == ID5032_26C ) {
	if ( IsDispArea ( ID5032_26A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26D, F5032_26A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26E, F5032_26B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26F, F5032_26C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_26sw = F5032_26c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_26F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_26sw = 0;
	}

	// 第二種事業及び第三種事業
	//if ( m_id == ID5032_27C ) {
	if ( IsDispArea ( ID5032_27A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27D, F5032_27A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27E, F5032_27B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27F, F5032_27C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_27sw = F5032_27c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_27F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_27sw = 0;
	}

	// 第二種事業及び第四種事業
	//if ( m_id == ID5032_28C ) {
	if ( IsDispArea ( ID5032_28A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28D, F5032_28A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28E, F5032_28B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28F, F5032_28C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_28sw = F5032_28c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_28F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_28sw = 0;
	}

	// 第二種事業及び第五種事業
	//if ( m_id == ID5032_29C ) {
	if ( IsDispArea ( ID5032_29A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29D, F5032_29A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29E, F5032_29B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29F, F5032_29C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_29sw = F5032_29c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_29F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_29sw = 0;
	}

	// 第二種事業及び第六種事業
	//if ( m_id == ID5032_30C ) {
	if ( IsDispArea ( ID5032_30A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30D, F5032_30A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30E, F5032_30B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30F, F5032_30C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_30sw = F5032_30c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_30F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_30sw = 0;
	}

	// 第三種事業及び第四種事業
	//if ( m_id == ID5032_31C ) {
	if ( IsDispArea ( ID5032_31A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31D, F5032_31A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31E, F5032_31B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31F, F5032_31C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_31sw = F5032_31c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_31F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_31sw = 0;
	}

	// 第三種事業及び第五種事業
	//if ( m_id == ID5032_32C ) {
	if ( IsDispArea ( ID5032_32A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32D, F5032_32A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32E, F5032_32B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32F, F5032_32C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_32sw = F5032_32c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_32F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_32sw = 0;
	}

	// 第三種事業及び第六種事業
	//if ( m_id == ID5032_33C ) {
	if ( IsDispArea ( ID5032_33A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33D, F5032_33A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33E, F5032_33B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33F, F5032_33C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_33sw = F5032_33c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_33F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_33sw = 0;
	}

	// 第四種事業及び第五種事業
	//if ( m_id == ID5032_34C ) {
	if ( IsDispArea ( ID5032_34A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34D, F5032_34A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34E, F5032_34B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34F, F5032_34C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_34sw = F5032_34c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_34F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_34sw = 0;
	}

	// 第四種事業及び第六種事業
	//if ( m_id == ID5032_35C ) {
	if ( IsDispArea ( ID5032_35A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35D, F5032_35A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35E, F5032_35B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35F, F5032_35C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_35sw = F5032_35c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_35F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_35sw = 0;
	}

	// 第五種事業及び第六種事業
	//if ( m_id == ID5032_36C ) {
	if ( IsDispArea ( ID5032_36A ) ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36D, F5032_36A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36E, F5032_36B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36F, F5032_36C, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_36sw = F5032_36c;
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_36F, zerobuf, MONY_BUF_SIZE );
		( *m_pSnFhyo5_012Data )->Sn_F5012_36sw = 0;
	}

	// 選択可能な計算式区分の内から選択した金額
	if ( m_id != 0 ) {
		memset ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37X, '\0', MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37D, F5032_37A, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37E, F5032_37B, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37F, F5032_37C, MONY_BUF_SIZE );
	}
	else {
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37X, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37D, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37E, zerobuf, MONY_BUF_SIZE );
		memmove ( ( *m_pSnFhyo5_012Data )->Sn_F5012_37F, zerobuf, MONY_BUF_SIZE );
	}
}


//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo5_032::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray	money;
	m_money35 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );
	money = m_money35;

	int cnt = ( int )money.GetCount();
	int incnt = 0;

	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			// 原則計算を適用する場合　4×みなし仕入率
			if ( money[i][j].code == _T( "359002" ) ) {
				memmove ( F5032_20A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359003" ) ) {
				memmove ( F5032_20B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359000") ) {
				memmove ( F5032_20C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 1種類の事業で75%以上　4×みなし仕入率
			else if ( money[i][j].code == _T( "359102" ) ) {
				memmove ( F5032_21A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359103" ) ) {
				memmove ( F5032_21B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359100" ) ) {
				memmove ( F5032_21C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第一種事業及び第二種事業
			else if ( money[i][j].code == _T( "359202" ) ) {
				memmove ( F5032_22A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359203" ) ) {
				memmove ( F5032_22B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359200" ) ) {
				memmove ( F5032_22C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第一種事業及び第三種事業
			else if ( money[i][j].code == _T( "359302" ) ) {
				memmove ( F5032_23A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359303" ) ) {
				memmove ( F5032_23B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359300" ) ) {
				memmove ( F5032_23C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第一種事業及び第四種事業
			else if ( money[i][j].code == _T( "359402" ) ) {
				memmove ( F5032_24A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359403" ) ) {
				memmove ( F5032_24B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359400" ) ) {
				memmove ( F5032_24C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第一種事業及び第五種事業
			else if ( money[i][j].code == _T( "359502" ) ) {
				memmove ( F5032_25A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359503" ) ) {
				memmove ( F5032_25B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359500" ) ) {
				memmove ( F5032_25C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第一種事業及び第六種事業
			else if ( money[i][j].code == _T( "359602" ) ) {
				memmove ( F5032_26A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359603" ) ) {
				memmove ( F5032_26B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359600" ) ) {
				memmove ( F5032_26C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第二種事業及び第三種事業
			else if ( money[i][j].code == _T( "359702" ) ) {
				memmove ( F5032_27A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359703" ) ) {
				memmove ( F5032_27B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359700" ) ) {
				memmove ( F5032_27C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第二種事業及び第四種事業
			else if ( money[i][j].code == _T( "359802" ) ) {
				memmove ( F5032_28A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359803" ) ) {
				memmove ( F5032_28B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359800" ) ) {
				memmove ( F5032_28C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第二種事業及び第五種事業
			else if ( money[i][j].code == _T( "359902" ) ) {
				memmove ( F5032_29A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359903" ) ) {
				memmove ( F5032_29B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "359900" ) ) {
				memmove ( F5032_29C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第二種事業及び第六種事業
			else if ( money[i][j].code == _T( "35A002" ) ) {
				memmove ( F5032_30A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A003" ) ) {
				memmove ( F5032_30B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A000" ) ) {
				memmove ( F5032_30C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第三種事業及び第四種事業
			else if ( money[i][j].code == _T( "35A102" ) ) {
				memmove ( F5032_31A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A103" ) ) {
				memmove ( F5032_31B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A100" ) ) {
				memmove ( F5032_31C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第三種事業及び第五種事業
			else if ( money[i][j].code == _T( "35A202" ) ) {
				memmove ( F5032_32A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A203" ) ) {
				memmove ( F5032_32B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A200" ) ) {
				memmove ( F5032_32C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第三種事業及び第六種事業
			else if ( money[i][j].code == _T( "35A302" ) ) {
				memmove ( F5032_33A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A303" ) ) {
				memmove ( F5032_33B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A300" ) ) {
				memmove ( F5032_33C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第四種事業及び第五種事業
			else if ( money[i][j].code == _T( "35A402" ) ) {
				memmove ( F5032_34A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A403" ) ) {
				memmove ( F5032_34B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A400" ) ) {
				memmove ( F5032_34C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第四種事業及び第六種事業
			else if ( money[i][j].code == _T( "35A502" ) ) {
				memmove ( F5032_35A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A503" ) ) {
				memmove ( F5032_35B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A500" ) ) {
				memmove ( F5032_35C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 第五種事業及び第六種事業
			else if ( money[i][j].code == _T( "35A602" ) ) {
				memmove ( F5032_36A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A603" ) ) {
				memmove ( F5032_36B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A600" ) ) {
				memmove ( F5032_36C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 選択可能な計算式区分のうちから選択した金額
			else if ( money[i][j].code == _T( "35A702" ) ) {
				memmove ( F5032_37A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "35A703" ) ) {
				memmove ( F5032_37B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if (money[i][j].code == _T( "35A700" ) ) {
				memmove ( F5032_37C, money[i][j].arith, MONY_BUF_SIZE );
			}
		}
	}

	int	gensoku = 0;
	char buff[256] = { 0 };
	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362007" ) );
	m_pArith->l_print ( buff, money[0][0].arith, _T( "SSSS" ) );
	gensoku = atoi( buff );
	if ( gensoku ) {
		( *m_pSnHeadData )->Sn_TOKUR = 0;
	}
	else {
		( *m_pSnHeadData )->Sn_TOKUR = 1;
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
int CShinFhyo5_032::SyukeiDataSet ( int cID )
{
	char val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL1, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	// 原則計算を適用する場合　4×みなし仕入率
	case ID5032_20C:
		if ( m_pArith->l_cmp ( F5032_20C, val ) ) {
			F5032_20c |= 0x8000;
			memmove ( F5032_20C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359000" ), F5032_20C );
		}
		break;
	case ID5032_20A:
		if ( m_pArith->l_cmp ( F5032_20A, val ) ) {
			F5032_20c |= 0x80;
			memmove ( F5032_20A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359002" ), F5032_20A );
		}
		break;
	case ID5032_20B:
		if ( m_pArith->l_cmp ( F5032_20B, val ) ) {
			F5032_20c |= 0x800;
			memmove ( F5032_20B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359003" ), F5032_20B );
		}
		break;

		// 1種類の事業で75％以上　4×みなし仕入率
	case ID5032_21C:
		if ( m_pArith->l_cmp ( F5032_21C, val ) ) {
			F5032_21c |= 0x8000;
			memmove ( F5032_21C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359100" ), F5032_21C );
		}
		break;
	case ID5032_21A:
		if ( m_pArith->l_cmp ( F5032_21A, val ) ) {
			F5032_21c |= 0x80;
			memmove ( F5032_21A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359102" ), F5032_21A );
		}
		break;
	case ID5032_21B:
		if ( m_pArith->l_cmp ( F5032_21B, val ) ) {
			F5032_21c |= 0x800;
			memmove ( F5032_21B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359103" ), F5032_21B );
		}
		break;

		// 第一種事業及び第二種事業
	case ID5032_22C:
		if ( m_pArith->l_cmp ( F5032_22C, val ) ) {
			F5032_22c |= 0x8000;
			memmove ( F5032_22C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359200" ), F5032_22C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E001" ), F5032_22C );
		}
		break;
	case ID5032_22A:
		if ( m_pArith->l_cmp ( F5032_22A, val ) ) {
			F5032_22c |= 0x80;
			memmove ( F5032_22A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359202" ), F5032_22A );
		}
		break;
	case ID5032_22B:
		if ( m_pArith->l_cmp ( F5032_22B, val ) ) {
			F5032_22c |= 0x800;
			memmove ( F5032_22B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359203" ), F5032_22B );
		}
		break;

		// 第一種事業及び第三種事業
	case ID5032_23C:
		if ( m_pArith->l_cmp ( F5032_23C, val ) ) {
			F5032_23c |= 0x8000;
			memmove ( F5032_23C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359300" ), F5032_23C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E002" ), F5032_23C );
		}
		break;
	case ID5032_23A:
		if ( m_pArith->l_cmp ( F5032_23A, val ) ) {
			F5032_23c |= 0x80;
			memmove ( F5032_23A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359302" ), F5032_23A );
		}
		break;
	case ID5032_23B:
		if ( m_pArith->l_cmp ( F5032_23B, val ) ) {
			F5032_23c |= 0x800;
			memmove ( F5032_23B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359303" ), F5032_23B );
		}
		break;

		// 第一種事業及び第四種事業
	case ID5032_24C:
		if ( m_pArith->l_cmp ( F5032_24C, val ) ) {
			F5032_24c |= 0x8000;
			memmove ( F5032_24C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359400" ), F5032_24C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E003" ), F5032_24C );
		}
		break;
	case ID5032_24A:
		if ( m_pArith->l_cmp ( F5032_24A, val ) ) {
			F5032_24c |= 0x80;
			memmove ( F5032_24A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359402" ), F5032_24A );
		}
		break;
	case ID5032_24B:
		if ( m_pArith->l_cmp ( F5032_24B, val ) ) {
			F5032_24c |= 0x800;
			memmove ( F5032_24B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359403" ), F5032_24B );
		}
		break;

		// 第一種事業及び第五種事業
	case ID5032_25C:
		if ( m_pArith->l_cmp ( F5032_25C, val ) ) {
			F5032_25c |= 0x8000;
			memmove ( F5032_25C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359500" ), F5032_25C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E004" ), F5032_25C );
		}
		break;
	case ID5032_25A:
		if ( m_pArith->l_cmp ( F5032_25A, val ) ) {
			F5032_25c |= 0x80;
			memmove ( F5032_25A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359502" ), F5032_25A );
		}
		break;
	case ID5032_25B:
		if ( m_pArith->l_cmp ( F5032_25B, val ) ) {
			F5032_25c |= 0x800;
			memmove ( F5032_25B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359503" ), F5032_25B );
		}
		break;

		// 第一種事業及び第六種事業
	case ID5032_26C:
		if ( m_pArith->l_cmp ( F5032_26C, val ) ) {
			F5032_26c |= 0x8000;
			memmove ( F5032_26C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359600" ), F5032_26C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E005" ), F5032_26C );
		}
		break;
	case ID5032_26A:
		if ( m_pArith->l_cmp ( F5032_26A, val ) ) {
			F5032_26c |= 0x80;
			memmove ( F5032_26A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359602" ), F5032_26A );
		}
		break;
	case ID5032_26B:
		if ( m_pArith->l_cmp ( F5032_26B, val ) ) {
			F5032_26c |= 0x800;
			memmove ( F5032_26B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359603" ), F5032_26B );
		}
		break;

		// 第二種事業及び第三種事業
	case ID5032_27C:
		if ( m_pArith->l_cmp ( F5032_27C, val ) ) {
			F5032_27c |= 0x8000;
			memmove ( F5032_27C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359700" ), F5032_27C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E006" ), F5032_27C );
		}
		break;
	case ID5032_27A:
		if ( m_pArith->l_cmp ( F5032_27A, val ) ) {
			F5032_27c |= 0x80;
			memmove ( F5032_27A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359702" ), F5032_27A );
		}
		break;
	case ID5032_27B:
		if ( m_pArith->l_cmp ( F5032_27B, val ) ) {
			F5032_27c |= 0x800;
			memmove ( F5032_27B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359703" ), F5032_27B );
		}
		break;

		// 第二種事業及び第四種事業
	case ID5032_28C:
		if ( m_pArith->l_cmp ( F5032_28C, val ) ) {
			F5032_28c |= 0x8000;
			memmove ( F5032_28C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359800" ), F5032_28C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E007" ), F5032_28C );
		}
		break;
	case ID5032_28A:
		if ( m_pArith->l_cmp ( F5032_28A, val ) ) {
			F5032_28c |= 0x80;
			memmove ( F5032_28A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359802" ), F5032_28A );
		}
		break;
	case ID5032_28B:
		if ( m_pArith->l_cmp ( F5032_28B, val ) ) {
			F5032_28c |= 0x800;
			memmove ( F5032_28B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359803" ), F5032_28B );
		}
		break;

		// 第二種事業及び第五種事業
	case ID5032_29C:
		if ( m_pArith->l_cmp ( F5032_29C, val ) ) {
			F5032_29c |= 0x8000;
			memmove ( F5032_29C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359900" ), F5032_29C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E008" ), F5032_29C );
		}
		break;
	case ID5032_29A:
		if ( m_pArith->l_cmp ( F5032_29A, val ) ) {
			F5032_29c |= 0x80;
			memmove ( F5032_29A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359902" ), F5032_29A );
		}
		break;
	case ID5032_29B:
		if ( m_pArith->l_cmp ( F5032_29B, val ) ) {
			F5032_29c |= 0x800;
			memmove ( F5032_29B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "359903" ), F5032_29B );
		}
		break;


		// 第二種事業及び第六種事業
	case ID5032_30C:
		if ( m_pArith->l_cmp ( F5032_30C, val ) ) {
			F5032_30c |= 0x8000;
			memmove ( F5032_30C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A000" ), F5032_30C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E009" ), F5032_30C );
		}
		break;
	case ID5032_30A:
		if ( m_pArith->l_cmp ( F5032_30A, val ) ) {
			F5032_30c |= 0x80;
			memmove ( F5032_30A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A002" ), F5032_30A );
		}
		break;
	case ID5032_30B:
		if ( m_pArith->l_cmp ( F5032_30B, val ) ) {
			F5032_30c |= 0x800;
			memmove ( F5032_30B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A003" ), F5032_30B );
		}
		break;


		// 第三種事業及び第四種事業
	case ID5032_31C:
		if ( m_pArith->l_cmp ( F5032_31C, val ) ) {
			F5032_31c |= 0x8000;
			memmove ( F5032_31C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A100" ), F5032_31C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00A" ), F5032_31C );
		}
		break;
	case ID5032_31A:
		if ( m_pArith->l_cmp ( F5032_31A, val ) ) {
			F5032_31c |= 0x80;
			memmove ( F5032_31A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A102" ), F5032_31A );
		}
		break;
	case ID5032_31B:
		if ( m_pArith->l_cmp ( F5032_31B, val ) ) {
			F5032_31c |= 0x800;
			memmove ( F5032_31B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A103" ), F5032_31B );
		}
		break;

		// 第三種事業及び第五種事業
	case ID5032_32C:
		if ( m_pArith->l_cmp ( F5032_32C, val ) ) {
			F5032_32c |= 0x8000;
			memmove ( F5032_32C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A200" ), F5032_32C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00B" ), F5032_32C );
		}
		break;
	case ID5032_32A:
		if ( m_pArith->l_cmp ( F5032_32A, val ) ) {
			F5032_32c |= 0x80;
			memmove ( F5032_32A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A202" ), F5032_32A );
		}
		break;
	case ID5032_32B:
		if ( m_pArith->l_cmp ( F5032_32B, val ) ) {
			F5032_32c |= 0x800;
			memmove ( F5032_32B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A203" ), F5032_32B );
		}
		break;

		// 第三種事業及び第六種事業
	case ID5032_33C:
		if ( m_pArith->l_cmp ( F5032_33C, val ) ) {
			F5032_33c |= 0x8000;
			memmove ( F5032_33C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A300" ), F5032_33C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00C" ), F5032_33C );
		}
		break;
	case ID5032_33A:
		if ( m_pArith->l_cmp ( F5032_33A, val ) ) {
			F5032_33c |= 0x80;
			memmove ( F5032_33A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A302" ), F5032_33A );
		}
		break;
	case ID5032_33B:
		if ( m_pArith->l_cmp ( F5032_33B, val ) ) {
			F5032_33c |= 0x800;
			memmove ( F5032_33B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A303" ), F5032_33B );
		}
		break;

		// 第四種事業及び第五種事業
	case ID5032_34C:
		if ( m_pArith->l_cmp ( F5032_34C, val ) ) {
			F5032_34c |= 0x8000;
			memmove ( F5032_34C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A400" ), F5032_34C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00D" ), F5032_34C );
		}
		break;
	case ID5032_34A:
		if ( m_pArith->l_cmp ( F5032_34A, val ) ) {
			F5032_34c |= 0x80;
			memmove ( F5032_34A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A402" ), F5032_34A );
		}
		break;
	case ID5032_34B:
		if ( m_pArith->l_cmp ( F5032_34B, val ) ) {
			F5032_34c |= 0x800;
			memmove ( F5032_34B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A403" ), F5032_34B );
		}
		break;

		// 第四種事業及び第六種事業
	case ID5032_35C:
		if ( m_pArith->l_cmp ( F5032_35C, val ) ) {
			F5032_35c |= 0x8000;
			memmove ( F5032_35C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A500" ), F5032_35C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00E" ), F5032_35C );
		}
		break;
	case ID5032_35A:
		if ( m_pArith->l_cmp ( F5032_35A, val ) ) {
			F5032_35c |= 0x80;
			memmove ( F5032_35A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A502" ), F5032_35A );
		}
		break;
	case ID5032_35B:
		if ( m_pArith->l_cmp ( F5032_35B, val ) ) {
			F5032_35c |= 0x800;
			memmove ( F5032_35B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A503" ), F5032_35B );
		}
		break;

		// 第五種事業及び第六種事業
	case ID5032_36C:
		if ( m_pArith->l_cmp ( F5032_36C, val ) ) {
			F5032_36c |= 0x8000;
			memmove ( F5032_36C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A600" ), F5032_36C );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00F" ), F5032_36C );
		}
		break;
	case ID5032_36A:
		if ( m_pArith->l_cmp ( F5032_36A, val ) ) {
			F5032_36c |= 0x80;
			memmove ( F5032_36A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A602" ), F5032_36A );
		}
		break;
	case ID5032_36B:
		if ( m_pArith->l_cmp ( F5032_36B, val ) ) {
			F5032_36c |= 0x800;
			memmove ( F5032_36B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35A603" ), F5032_36B );
		}
		break;
	}

	CalqGokei ( cID );

	return MemoryReget();
}


//=========================================================
// ダイアグラム初期属性セット
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo5_032::InitDiagAttr()
{
	int cnt = 0;

	IsEnableItems ( 0, m_id, m_code, m_typecnt );
	m_flg = Gokeiclctype();
	m_4val = Is4Val ( 0 );
	if ( !m_4val ) {
		m_id = 0;
		m_code.Empty();
	}

	GetTokurei();

	while ( 1 ) {
		if ( Fhyo5_032[cnt].Index == 0 ) {
			break;
		}

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo5_032[cnt].AllUnConnect;
		}
		else if ( ( m_typecnt < 2 ) || ( m_4val == 0 ) ) {
			// 単一業種の時には入力不可
			Flg = Fhyo5_032[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				if ( ( Fhyo5_032[cnt].Index == ID5032_20A ) || ( Fhyo5_032[cnt].Index == ID5032_20B ) ) {
					Flg = Fhyo5_032[cnt].UnConnect;
				}
				else if ( ( Fhyo5_032[cnt].Index == ID5032_20C ) && ( m_flg & 0x01 ) ) {
					Flg = Fhyo5_032[cnt].UnConnect;
				}
				// 付表5-3(1)の売上割合をチェック
				/*else if ( ( m_typecnt > 1 ) && ( ( Fhyo5_032[cnt].Index == ID5032_21A ) || ( Fhyo5_032[cnt].Index == ID5032_21B ) ) && ( m_id == ID5032_21C ) ) {
					Flg = Fhyo5_032[cnt].UnConnect;
				}
				else if ( ( m_typecnt > 1 ) && ( Fhyo5_032[cnt].Index == ID5032_21C ) && ( m_id == ID5032_21C ) && ( m_flg & 0x01 ) ) {
					Flg = Fhyo5_032[cnt].UnConnect;
				}
				else if ( ( m_typecnt > 2 ) && ( ( Fhyo5_032[cnt].Index == ( m_id - 2 ) ) || ( Fhyo5_032[cnt].Index == ( m_id - 1 ) ) ) ) {
					Flg = Fhyo5_032[cnt].UnConnect;
				}
				else if ( ( m_typecnt > 2 ) && ( Fhyo5_032[cnt].Index == m_id ) && ( m_flg & 0x01 ) ) {
					Flg = Fhyo5_032[cnt].UnConnect;
				}*/
				else if ( IsDispArea ( Fhyo5_032[cnt].Index ) == 1 ) {
					Flg = Fhyo5_032[cnt].UnConnect;
					CalqGokei(  Fhyo5_032[cnt].Index );
				}
				else {
					Flg = 0;
					ClearInput ( Fhyo5_032[cnt].Index );
				}
			}
			else {
				Flg = Fhyo5_032[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Fhyo5_032[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo5_032[cnt].Connect );
		cnt++;

	}

	m_Fhyo5_032Diag01.EnableDelete ( 0 );

	//if ( IsKjCodeReset() == true ) {
	//	ResetKjCode();
	//}

	// 付表5-3(1)の売上割合チェック時に手入力のクリアを行っているため、再集計が必要
	MemoryReget();

	// 集計後出力項目の変更
	if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x40 ) {
		SetPrintArea();
	}
	else {
		AllPrint();
	}

	return 0;
}


//=========================================================
// 有利な計算式を判別
//---------------------------------------------------------
// 引数
//		reflg		:	(ON)再集計フラグ
//		Idx			:	合計行のインデックス
//		code		:	合計行のコード
//		typeCnt		:	業種数
//---------------------------------------------------------
// 返送値
//			0		:	正常終了
//=========================================================
int CShinFhyo5_032::IsEnableItems ( int reflg, short& Idx, CString& code, int& typeCnt )
{
	MoneyBasejagArray	money;
	char max_valttl[MONY_BUF_SIZE] = { 0 };
	char buff[256] = { 0 };
	short maxid = 0;
	CString	maxcode;

	typeCnt = 0;

	// 業種数
	if ( reflg ) {
		m_money35 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );
	}
	money = m_money35;

	int cnt = ( int )money.GetCount();
	int incnt = 0;

	BOOL sgn = FALSE;
	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			if ( money[i][j].code == _T( "35F000" ) ) {
				m_pArith->l_print ( buff, money[i][j].arith, _T( "SSS" ) );
				typeCnt = atoi ( buff );

				sgn = TRUE;
				break;
			}

			if ( sgn == TRUE ) {
				break;
			}
		}
	}

	int tkubun = 0;
	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362006" ) );
	m_pArith->l_print ( buff, money[0][0].arith, _T( "SSSS" ) );
	tkubun = atoi ( buff );

	switch ( tkubun ) {
	case 1:
		Idx = ID5032_21C;
		code = _T( "35E000" );
		break;
	case 2:
		Idx = ID5032_22C;
		code = _T( "35E001" );
		break;
	case 3:
		Idx = ID5032_23C;
		code = _T( "35E002" );
		break;
	case 4:
		Idx = ID5032_24C;
		code = _T( "35E003" );
		break;
	case 5:
		Idx = ID5032_25C;
		code = _T( "35E004" );
		break;
	case 6:
		Idx = ID5032_26C;
		code = _T( "35E005" );
		break;
	case 7:
		Idx = ID5032_27C;
		code = _T( "35E006" );
		break;
	case 8:
		Idx = ID5032_28C;
		code = _T( "35E007" );
		break;
	case 9:
		Idx = ID5032_29C;
		code = _T( "35E008" );
		break;
	case 10:
		Idx = ID5032_30C;
		code = _T( "35E009" );
		break;
	case 11:
		Idx = ID5032_31C;
		code = _T( "35E00A" );
		break;
	case 12:
		Idx = ID5032_32C;
		code = _T( "35E00B" );
		break;
	case 13:
		Idx = ID5032_33C;
		code = _T( "35E00C" );
		break;
	case 14:
		Idx = ID5032_34C;
		code = _T( "35E00D" );
		break;
	case 15:
		Idx = ID5032_35C;
		code = _T( "35E00E" );
		break;
	case 16:
		Idx = ID5032_36C;
		code = _T( "35E00F" );
		break;
	default:
		if ( typeCnt >= 2 ) {
			Idx = ID5032_20C;
			code.Empty();
		}
		else {
			Idx = 0;
			code.Empty();
		}
	}

	F5032_21kbn = 0;
	//if ( Idx == ID5032_21C ) {
	if ( IsDispArea ( ID5032_21A ) ) {
		int minashi = 0;
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362002" ) );
		m_pArith->l_print ( buff, money[0][0].arith, _T( "SSSS" ) );
		minashi = atoi ( buff );
		switch ( minashi ) {
		case 9000:
			F5032_21kbn = 1;
			break;
		case 8000:
			F5032_21kbn = 2;
			break;
		case 7000:
			F5032_21kbn = 3;
			break;
		case 6000:
			F5032_21kbn = 4;
			break;
		case 5000:
			F5032_21kbn = 5;
			break;
		case 4000:
			F5032_21kbn = 6;
			break;
		default:
			F5032_21kbn = 0;
			break;
		}
	}

	m_PrintId = Idx;

	return 0;
}

//=========================================================
// 合計欄の計算式を判別
//---------------------------------------------------------
// 返送値
//		0		:	横計
//		1		:	別計算 ( 実額入力可 )
//=========================================================
int CShinFhyo5_032::Gokeiclctype()
{
	MoneyBasejagArray money;
	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362003" ) );

	if ( m_pArith->l_test ( money[0][0].arith ) ) {
		return 1;
	}

	return 0;
}

//=========================================================
// 控除対象仕入税額計算の為の消費税額判定
//---------------------------------------------------------
// 引数
//		reflg	:	(ON)再集計フラグ
//---------------------------------------------------------
// 返送値
//		0		:	全税率に金額なし
//		1		:	金額あり
//=========================================================
int CShinFhyo5_032::Is4Val ( int reflg )
{
	MoneyBasejagArray money;
	char val[MONY_BUF_SIZE] = { 0 },
		 max_valttl[MONY_BUF_SIZE] = { 0 };

	if ( reflg ) {
		m_money35 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );
	}
	money = m_money35;

	int cnt = ( int )money.GetCount();
	int incnt = 0;

	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			if ( money[i][j].code == _T( "350402" ) ) {
				memmove ( val, money[i][j].arith, MONY_BUF_SIZE );
				if ( m_pArith->l_test ( val ) != 0 ) {
					return 1;
				}
			}
			else if ( money[i][j].code == _T( "350403" ) ) {
				memmove ( val, money[i][j].arith, MONY_BUF_SIZE );
				if ( m_pArith->l_test ( val ) != 0 ) {
					return 1;
				}
			}
			else if ( money[i][j].code == _T( "357402" ) ) {
				memmove ( val, money[i][j].arith, MONY_BUF_SIZE );
				if ( m_pArith->l_test ( val ) != 0 ) {
					return 1;
				}
			}
			else if ( money[i][j].code == _T( "357403" ) ) {
				memmove ( val, money[i][j].arith, MONY_BUF_SIZE );
				if ( m_pArith->l_test( val ) != 0 ) {
					return 1;
				}
			}
		}
	}

	return 0;
}

//=========================================================
// ポジションセット
//=========================================================
void CShinFhyo5_032::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元5
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( ( index = m_Fhyo5_032Diag01.GetPosition() ) != -1 ) {
			m_Fhyo5_032Diag01.SetFocus();	// ダミーでセット
			m_Fhyo5_032Diag01.SetPosition ( index );
			m_Fhyo5_032Diag01.SetFocus();
			return;
		}
	}

	GetDlgItem ( IDC_STATIC_INPUTLOCK )->SetFocus();
	m_Fhyo5_032Diag01.SetFocus();

	//　4×みなし仕入率へ
	m_Fhyo5_032Diag01.SetPosition ( ID5032_20A );
}

//=========================================================
// 画面出力
//=========================================================
void CShinFhyo5_032::DispFh5_032()
{
	// 画面初期化
	for ( int i = ID5032_20A; i <= ID5032_37C; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ( short )i );
	}

	// ダイアグラム初期属性セット
	InitDiagAttr();

/*	long flg = 0;
	BOOL sw = FALSE;
	if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) {
		flg = DIAG_MDFY_OPAQUE;
		sw = TRUE;
	}
	else {
		flg = DIAG_MDFY_TRANSPARENT;
		sw = TRUE;
	}
	
	if ( ChangeBackColor ( sw ) == 0 ) {
		m_Fhyo5_032Diag01.ModifyItem ( SELECTITEM, flg );
	}
	else {
		m_Fhyo5_032Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_TRANSPARENT );
	}
*/

	// 入力データの表示
	f5032data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo5_032::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo5_032::dsp_cls ( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
	diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
}


//=========================================================
// 項目表示
//---------------------------------------------------------
// 引数
//		atno		：		項目番号
//=========================================================
void CShinFhyo5_032::f5032data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f5032data_20();
	case 2:
		f5032data_21();
	case 3:
		f5032data_22();
	case 4:
		f5032data_23();
	case 5:
		f5032data_24();
	case 6:
		f5032data_25();
	case 7:
		f5032data_26();
	case 8:
		f5032data_27();
	case 9:
		f5032data_28();
	case 10:
		f5032data_29();
	case 11:
		f5032data_30();
	case 12:
		f5032data_31();
	case 13:
		f5032data_32();
	case 14:
		f5032data_33();
	case 15:
		f5032data_34();
	case 16:
		f5032data_35();
	case 17:
		f5032data_36();
	case 18:
		f5032data_37();

	default:
		break;
	}
}

//=========================================================
// 4×みなし仕入率
//=========================================================
void CShinFhyo5_032::f5032data_20()
{
	if ( ( m_typecnt > 1 ) && ( m_4val ) ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_20A, ( F5032_20c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_20B, ( F5032_20c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_20C, ( F5032_20c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_20A, F5032_20A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_20B, F5032_20B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_20C, F5032_20C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_20A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_20B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_20C );
	}
}

//=========================================================
// 4×みなし仕入率
//=========================================================
void CShinFhyo5_032::f5032data_21()
{
	/*if ( m_id == ID5032_21C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_21A, ( F5032_21c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_21B, ( F5032_21c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_21C, ( F5032_21c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_21A, F5032_21A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_21B, F5032_21B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_21C, F5032_21C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_21A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_21B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_21C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_21A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_21A, ( F5032_21c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_21B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_21B, ( F5032_21c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_21C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_21C, ( F5032_21c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_21A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_21A, F5032_21A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_21B, F5032_21B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_21C, F5032_21C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_21A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_21B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_21C );
	}
}

//=========================================================
// 第一種事業及び第二種事業
//=========================================================
void CShinFhyo5_032::f5032data_22()
{
	/*if ( m_id == ID5032_22C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_22A, ( F5032_22c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_22B, ( F5032_22c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_22C, ( F5032_22c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_22A, F5032_22A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_22B, F5032_22B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_22C, F5032_22C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_22A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_22B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_22C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_22A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_22A, ( F5032_22c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_22B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_22B, ( F5032_22c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_22C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_22C, ( F5032_22c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_22A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_22A, F5032_22A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_22B, F5032_22B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_22C, F5032_22C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_22A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_22B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_22C );
	}
}

//=========================================================
// 第一種事業及び第三種事業
//=========================================================
void CShinFhyo5_032::f5032data_23()
{
	/*if ( m_id == ID5032_23C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_23A, ( F5032_23c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_23B, ( F5032_23c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_23C, ( F5032_23c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_23A, F5032_23A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_23B, F5032_23B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_23C, F5032_23C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_23A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_23B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_23C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_23A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_23A, ( F5032_23c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_23B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_23B, ( F5032_23c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_23C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_23C, ( F5032_23c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_23A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_23A, F5032_23A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_23B, F5032_23B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_23C, F5032_23C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_23A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_23B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_23C );
	}
}

//=========================================================
// 第一種事業及び第四種事業
//=========================================================
void CShinFhyo5_032::f5032data_24()
{
	/*if ( m_id == ID5032_24C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_24A, ( F5032_24c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_24B, ( F5032_24c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_24C, ( F5032_24c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_24A, F5032_24A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_24B, F5032_24B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_24C, F5032_24C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_24A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_24B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_24C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_24A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_24A, ( F5032_24c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_24B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_24B, ( F5032_24c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_24C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_24C, ( F5032_24c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_24A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_24A, F5032_24A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_24B, F5032_24B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_24C, F5032_24C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_24A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_24B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_24C );
	}
}

//=========================================================
// 第一種事業及び第五種事業
//=========================================================
void CShinFhyo5_032::f5032data_25()
{
	/*if ( m_id == ID5032_25C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_25A, ( F5032_25c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_25B, ( F5032_25c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_25C, ( F5032_25c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_25A, F5032_25A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_25B, F5032_25B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_25C, F5032_25C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_25A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_25B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_25C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_25A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_25A, ( F5032_25c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_25B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_25B, ( F5032_25c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_25C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_25C, ( F5032_25c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_25A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_25A, F5032_25A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_25B, F5032_25B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_25C, F5032_25C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_25A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_25B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_25C );
	}
}

//=========================================================
// 第一種事業及び第六種事業
//=========================================================
void CShinFhyo5_032::f5032data_26()
{
	/*if ( m_id == ID5032_26C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_26A, ( F5032_26c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_26B, ( F5032_26c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_26C, ( F5032_26c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_26A, F5032_26A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_26B, F5032_26B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_26C, F5032_26C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_26A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_26B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_26C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_26A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_26A, ( F5032_26c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_26B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_26B, ( F5032_26c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_26C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_26C, ( F5032_26c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_26A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_26A, F5032_26A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_26B, F5032_26B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_26C, F5032_26C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_26A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_26B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_26C );
	}
}

//=========================================================
// 第二種事業及び第三種事業
//=========================================================
void CShinFhyo5_032::f5032data_27()
{
	/*if ( m_id == ID5032_27C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_27A, ( F5032_27c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_27B, ( F5032_27c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_27C, ( F5032_27c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_27A, F5032_27A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_27B, F5032_27B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_27C, F5032_27C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_27A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_27B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_27C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_27A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_27A, ( F5032_27c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_27B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_27B, ( F5032_27c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_27C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_27C, ( F5032_27c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_27A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_27A, F5032_27A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_27B, F5032_27B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_27C, F5032_27C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_27A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_27B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_27C );
	}
}

//=========================================================
// 第二種事業及び第四種事業
//=========================================================
void CShinFhyo5_032::f5032data_28()
{
	/*if ( m_id == ID5032_28C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_28A, ( F5032_28c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_28B, ( F5032_28c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_28C, ( F5032_28c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_28A, F5032_28A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_28B, F5032_28B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_28C, F5032_28C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_28A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_28B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_28C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_28A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_28A, ( F5032_28c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_28B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_28B, ( F5032_28c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_28C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_28C, ( F5032_28c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_28A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_28A, F5032_28A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_28B, F5032_28B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_28C, F5032_28C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_28A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_28B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_28C );
	}
}

//=========================================================
// 第二種事業及び第五種事業
//=========================================================
void CShinFhyo5_032::f5032data_29()
{
	/*if ( m_id == ID5032_29C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_29A, ( F5032_29c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_29B, ( F5032_29c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_29C, ( F5032_29c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_29A, F5032_29A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_29B, F5032_29B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_29C, F5032_29C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_29A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_29B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_29C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_29A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_29A, ( F5032_29c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_29B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_29B, ( F5032_29c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_29C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_29C, ( F5032_29c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_29A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_29A, F5032_29A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_29B, F5032_29B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_29C, F5032_29C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_29A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_29B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_29C );
	}
}

//=========================================================
// 第二種事業及び第六種事業
//=========================================================
void CShinFhyo5_032::f5032data_30()
{
	/*if ( m_id == ID5032_30C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_30A, ( F5032_30c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_30B, ( F5032_30c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_30C, ( F5032_30c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_30A, F5032_30A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_30B, F5032_30B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_30C, F5032_30C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_30A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_30B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_30C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_30A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_30A, ( F5032_30c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_30B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_30B, ( F5032_30c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_30C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_30C, ( F5032_30c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_30A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_30A, F5032_30A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_30B, F5032_30B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_30C, F5032_30C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_30A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_30B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_30C );
	}
}

//=========================================================
// 第三種事業及び第四種事業
//=========================================================
void CShinFhyo5_032::f5032data_31()
{
	/*if ( m_id == ID5032_31C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_31A, ( F5032_31c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_31B, ( F5032_31c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_31C, ( F5032_31c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_31A, F5032_31A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_31B, F5032_31B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_31C, F5032_31C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_31A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_31B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_31C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_31A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_31A, ( F5032_31c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_31B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_31B, ( F5032_31c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_31C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_31C, ( F5032_31c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_31A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_31A, F5032_31A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_31B, F5032_31B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_31C, F5032_31C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_31A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_31B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_31C );
	}
}

//=========================================================
// 第三種事業及び第五種事業
//=========================================================
void CShinFhyo5_032::f5032data_32()
{
	/*if ( m_id == ID5032_32C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_32A, ( F5032_32c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_32B, ( F5032_32c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_32C, ( F5032_32c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_32A, F5032_32A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_32B, F5032_32B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_32C, F5032_32C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_32A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_32B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_32C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_32A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_32A, ( F5032_32c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_32B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_32B, ( F5032_32c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_32C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_32C, ( F5032_32c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_32A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_32A, F5032_32A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_32B, F5032_32B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_32C, F5032_32C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_32A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_32B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_32C );
	}
}

//=========================================================
// 第三種事業及び第六種事業
//=========================================================
void CShinFhyo5_032::f5032data_33()
{
	/*if ( m_id == ID5032_33C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_33A, ( F5032_33c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_33B, ( F5032_33c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_33C, ( F5032_33c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_33A, F5032_33A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_33B, F5032_33B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_33C, F5032_33C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_33A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_33B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_33C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_33A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_33A, ( F5032_33c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_33B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_33B, ( F5032_33c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_33C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_33C, ( F5032_33c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_33A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_33A, F5032_33A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_33B, F5032_33B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_33C, F5032_33C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_33A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_33B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_33C );
	}
}

//=========================================================
// 第四種事業及び第五種事業
//=========================================================
void CShinFhyo5_032::f5032data_34()
{
	/*if ( m_id == ID5032_34C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_34A, ( F5032_34c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_34B, ( F5032_34c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_34C, ( F5032_34c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_34A, F5032_34A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_34B, F5032_34B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_34C, F5032_34C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_34A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_34B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_34C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_34A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_34A, ( F5032_34c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_34B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_34B, ( F5032_34c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_34C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_34C, ( F5032_34c & 0x8000 ) ? 3 : 0 );	
		}
	}
	if ( IsDispArea ( ID5032_34A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_34A, F5032_34A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_34B, F5032_34B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_34C, F5032_34C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_34A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_34B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_34C );
	}
}

//=========================================================
// 第四種事業及び第六種事業
//=========================================================
void CShinFhyo5_032::f5032data_35()
{
	/*if ( m_id == ID5032_35C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_35A, ( F5032_35c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_35B, ( F5032_35c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_35C, ( F5032_35c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_35A, F5032_35A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_35B, F5032_35B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_35C, F5032_35C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_35A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_35B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_35C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_35A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_35A, ( F5032_35c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_35B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_35B, ( F5032_35c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_35C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_35C, ( F5032_35c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_35A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_35A, F5032_35A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_35B, F5032_35B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_35C, F5032_35C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_35A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_35B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_35C );
	}
}

//=========================================================
// 第五種事業及び第六種事業
//=========================================================
void CShinFhyo5_032::f5032data_36()
{
	/*if ( m_id == ID5032_36C ) {
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_36A, ( F5032_36c & 0x80 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_36B, ( F5032_36c & 0x800 ) ? 3 : 0 );
			if ( m_flg & 0x01 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_36C, ( F5032_36c & 0x8000 ) ? 3 : 0 );
			}
		}
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_36A, F5032_36A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_36B, F5032_36B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_36C, F5032_36C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_36A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_36B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_36C );
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5032_36A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_36A, ( F5032_36c & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_36B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_36B, ( F5032_36c & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5032_36C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5032_36C, ( F5032_36c & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5032_36A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_36A, F5032_36A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_36B, F5032_36B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_36C, F5032_36C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_36A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_36B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_36C );
	}
}

//=========================================================
// 選択可能な計算式区分の内から選択した金額
//=========================================================
void CShinFhyo5_032::f5032data_37()
{
	if ( m_id != 0 ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_37A, F5032_37A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_37B, F5032_37B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5032_37C, F5032_37C );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_37A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_37B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5032_37C );
	}
}

//=========================================================
// 表示金額取得
//=========================================================
void CShinFhyo5_032::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 4×みなし仕入率
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_20A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_20A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_20B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_20B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_20C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_20C, DiagData.data_val, MONY_BUF_SIZE );

	// 4×みなし仕入率
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_21A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_21A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_21B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_21B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_21C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_21C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業及び第二種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_22A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_22A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_22B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_22B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_22C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_22C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業及び第三種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_23A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_23A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_23B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_23B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_23C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_23C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業及び第四種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_24A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_24A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_24B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_24B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_24C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_24C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業及び第五種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_25A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_25A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_25B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_25B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_25C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_25C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業及び第六種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_26A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_26A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_26B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_26B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_26C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_26C, DiagData.data_val, MONY_BUF_SIZE );

	// 第二種事業及び第三種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_27A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_27A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_27B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_27B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_27C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_27C, DiagData.data_val, MONY_BUF_SIZE );

	// 第二種事業及び第四種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_28A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_28A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_28B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_28B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_28C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_28C, DiagData.data_val, MONY_BUF_SIZE );

	// 第二種事業及び第五種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_29A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_29A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_29B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_29B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_29C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_29C, DiagData.data_val, MONY_BUF_SIZE );

	// 第二種事業及び第六種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_30A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_30A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_30B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_30B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_30C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_30C, DiagData.data_val, MONY_BUF_SIZE );

	// 第三種事業及び第四種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_31A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_31A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_31B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_31B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_31C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_31C, DiagData.data_val, MONY_BUF_SIZE );

	// 第三種事業及び第五種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_32A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_32A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_32B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_32B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_32C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_32C, DiagData.data_val, MONY_BUF_SIZE );

	// 第三種事業及び第六種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_33A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_33A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_33B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_33B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_33C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_33C, DiagData.data_val, MONY_BUF_SIZE );

	// 第四種事業及び第五種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_34A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_34A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_34B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_34B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_34C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_34C, DiagData.data_val, MONY_BUF_SIZE );

	// 第四種事業及び第六種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_35A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_35A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_35B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_35B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_35C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_35C, DiagData.data_val, MONY_BUF_SIZE );

	// 第五種事業及び第六種事業
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_36A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_36A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_36B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_36B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_36C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_36C, DiagData.data_val, MONY_BUF_SIZE );

	// 選択可能な計算式区分の内から選択した金額
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_37A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_37A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_37B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_37B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL1, ID5032_37C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F5032_37C, DiagData.data_val, MONY_BUF_SIZE );
}


//=========================================================
// 入力項目色変え
//---------------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=========================================================
void CShinFhyo5_032::ChangeColor(unsigned short id, short index, int sign)
{
	DIAGRAM_ATTRIBUTE DA;

	diag_getattr ( id, index, &DA, CIcsdiagctrl);
	switch ( sign ) {
	case 1:		// 入力不可 (白色)
		DA.attr_bcolor = BC_WHITE;
		break;
	case 2:		// ロック (灰色)
		DA.attr_bcolor = BC_GRAY;
		break;
	case 3:		// 特殊色① (青色)
		DA.attr_bcolor = BC_BLUE;
		break;
	case 4:		// 特殊色② (緑色)
		DA.attr_bcolor = BC_GREEN;
		break;
	case 5:		// 特殊色 (薄緑色)
		DA.attr_bcolor = BC_GREEN_L;
		break;
	default:
		DA.attr_bcolor = BC_CREAM;
	}

	diag_setattr ( id, index, &DA, FALSE, CIcsdiagctrl);
}

//=========================================================
// フォーカス検索
//---------------------------------------------------------
// 引数
//		Index	:	ダイアグラム内インデックス
//		Move	:	0 = 下, 1 = 上
//---------------------------------------------------------
// 返送値
//		次のフォーカスインデックス
//=========================================================
int CShinFhyo5_032::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY5_032CNT; i++ ) {
		if ( Index == FCS_Fhyo5_032[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo5_032[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY5_032CNT; j++ ) {
			m_Fhyo5_032Diag01.GetAttribute ( FCS_Fhyo5_032[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_032[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo5_032Diag01.GetAttribute ( FCS_Fhyo5_032[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_032[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo5_032::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F5032_20c & 0x8880 ) || ( F5032_21c & 0x8880 ) || ( F5032_22c & 0x8880 ) || ( F5032_23c & 0x8880 ) ||
			 ( F5032_24c & 0x8880 ) || ( F5032_25c & 0x8880 ) || ( F5032_26c & 0x8880 ) || ( F5032_27c & 0x8880 ) ||
			 ( F5032_28c & 0x8880 ) || ( F5032_29c & 0x8880 ) || ( F5032_30c & 0x8880 ) || ( F5032_31c & 0x8880 ) ||
			 ( F5032_32c & 0x8880 ) || ( F5032_33c & 0x8880 ) || ( F5032_34c & 0x8880 ) || ( F5032_35c & 0x8880 ) ||
			 ( F5032_36c & 0x8880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

//=========================================================
// 表示項目？
//---------------------------------------------------------
// 引数
//			index			：	ダイアグラム内インデックス
//---------------------------------------------------------
// 返送値
//			0				：	非表示項目
//			1				：	表示項目
//			-1				：	エラー
//=========================================================
int CShinFhyo5_032::IsDispArea ( short index )
{
	int ret = 0;

	//int id1 = 0, id2 = 0;
	int sw = 0;		// 1 = 第一種
	int *pDispSign = NULL;
	bool getFlg = false;

	//unsigned short uWari = 0, tmpWari1 = 0, tmpWari2 = 0;

	//char tmpMony[MONY_BUF_SIZE] = { 0 }, tmpMony2[MONY_BUF_SIZE] = { 0 };
	//DIAGRAM_DATA DiagData;

	if( m_id == 0 ) {
		return 0;
	}

	switch ( index ) {
	case ID5032_21A:
	case ID5032_21B:
	case ID5032_21C:
		if ( index == ID5032_21C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}
		sw = 1;
		pDispSign = &F5032_21Disp;
		getFlg = true;
		break;

	case ID5032_22A:
	case ID5032_22B:
	case ID5032_22C:
		if ( index == ID5032_22C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAINISHU;
		pDispSign = &F5032_22Disp;
		getFlg = true;
		break;

	case ID5032_23A:
	case ID5032_23B:
	case ID5032_23C:
		if ( index == ID5032_23C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAISANSHU;
		pDispSign = &F5032_23Disp;
		getFlg = true;
		break;

	case ID5032_24A:
	case ID5032_24B:
	case ID5032_24C:
		if ( index == ID5032_24C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5032_24Disp;
		getFlg = true;
		break;

	case ID5032_25A:
	case ID5032_25B:
	case ID5032_25C:
		if ( index == ID5032_25C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5032_25Disp;
		getFlg = true;
		break;

	case ID5032_26A:
	case ID5032_26B:
	case ID5032_26C:
		if ( index == ID5032_26C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5032_26Disp;
		getFlg = true;
		break;

	case ID5032_27A:
	case ID5032_27B:
	case ID5032_27C:
		if ( index == ID5032_27C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAISANSHU;
		pDispSign = &F5032_27Disp;
		getFlg = true;
		break;

	case ID5032_28A:
	case ID5032_28B:
	case ID5032_28C:
		if ( index == ID5032_28C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5032_28Disp;
		getFlg = true;
		break;

	case ID5032_29A:
	case ID5032_29B:
	case ID5032_29C:
		if ( index == ID5032_29C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5032_29Disp;
		getFlg = true;
		break;

	case ID5032_30A:
	case ID5032_30B:
	case ID5032_30C:
		if ( index == ID5032_30C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5032_30Disp;
		getFlg = true;
		break;

	case ID5032_31A:
	case ID5032_31B:
	case ID5032_31C:
		if ( index == ID5032_31C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5032_31Disp;
		getFlg = true;
		break;

	case ID5032_32A:
	case ID5032_32B:
	case ID5032_32C:
		if ( index == ID5032_32C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5032_32Disp;
		getFlg = true;
		break;

	case ID5032_33A:
	case ID5032_33B:
	case ID5032_33C:
		if ( index == ID5032_33C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5032_33Disp;
		getFlg = true;
		break;

	case ID5032_34A:
	case ID5032_34B:
	case ID5032_34C:
		if ( index == ID5032_34C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIYONSHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5032_34Disp;
		getFlg = true;
		break;

	case ID5032_35A:
	case ID5032_35B:
	case ID5032_35C:
		if ( index == ID5032_35C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIYONSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5032_35Disp;
		getFlg = true;
		break;

	case ID5032_36A:
	case ID5032_36B:
	case ID5032_36C:
		if ( index == ID5032_36C ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIGOSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5032_36Disp;
		getFlg = true;
		break;

	default:
		break;
	}

	ret = 0;
	/*if ( sw ) {
		// 単一事業
		for ( int i = DAIICHISHU; i <= DAIROKUSHU; i++ ) {
			uWari = 0;
			memset( tmpMony, '\0', MONY_BUF_SIZE );
			GetUriageWariai2 ( i, &uWari, tmpMony );
			uWari /= 10;

			if ( uWari >= URIAGEWARIAI ) {
				ret = 1;
				break;
			}
		}
	}
	else if ( ( id1 != 0 ) && ( id2 != 0 ) ) {
		if( m_typecnt > 2 ) {
			// 複数事業
			tmpWari1 = 0;
			memset( tmpMony, '\0', MONY_BUF_SIZE );
			GetUriageWariai2 ( id1, &tmpWari1, tmpMony );

			tmpWari2 = 0;
			memset( tmpMony2, '\0', MONY_BUF_SIZE );
			GetUriageWariai2 ( id2, &tmpWari2, tmpMony2 );

			//if ( ( tmpWari1 != 0 ) && ( tmpWari2 != 0 ) ) {
			if( ( m_pArith->l_test( tmpMony ) > 0 ) && ( m_pArith->l_test( tmpMony2 ) > 0 ) ) {
				uWari = 0;
				uWari = tmpWari1 + tmpWari2;
				uWari /= 10;
				if ( uWari >= URIAGEWARIAI ) {
					ret = 1;
				}
			}
		}
	}*/
	if( getFlg ) {
		ASSERT( pDispSign );
		if( pDispSign ) {
			if( sw ) {
				// 単一業種
				if( m_typecnt > 1 ) {
					if( *pDispSign & 0x01 ) {
						ret = 1;
					}
				}
			}
			else {
				// 複数業種
				if( m_typecnt > 2 ) {
					if( *pDispSign & 0x01 ) {
						ret = 1;
					}
				}
			}
		}
	}


	return ret;
}

//=========================================================
// 合計欄の計算
//---------------------------------------------------------
// 引数
//			index			：	ダイアグラム内インデックス
//---------------------------------------------------------
// 返送値
//			0			:	正常終了
//=========================================================
int CShinFhyo5_032::CalqGokei ( int index )
{
	if( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	char tmpMony[MONY_BUF_SIZE] = { 0 };
	CString code, setCode;
	MoneyBasejagArray money;

	memset ( tmpMony, '\0', sizeof ( tmpMony ) );
	switch ( index ) {
	case ID5032_21A:
	case ID5032_21B:
	case ID5032_21C:
		if ( F5032_21c & 0x8880 ) {
			code = _T( "359100" );
			setCode = _T( "35E000" );//masa
		}
		break;

	case ID5032_22A:
	case ID5032_22B:
	case ID5032_22C:
		if ( F5032_22c & 0x8880 ) {
			code = _T( "359200" );
			setCode = _T( "35E001" );
		}
		break;

	case ID5032_23A:
	case ID5032_23B:
	case ID5032_23C:
		if ( F5032_23c & 0x8880 ) {
			code = _T( "359300" );
			setCode = _T( "35E002" );
		}
		break;

	case ID5032_24A:
	case ID5032_24B:
	case ID5032_24C:
		if ( F5032_24c & 0x8880 ) {
			code = _T( "359400" );
			setCode = _T( "35E003" );
		}
		break;

	case ID5032_25A:
	case ID5032_25B:
	case ID5032_25C:
		if ( F5032_25c & 0x8880 ) {
			code = _T( "359500" );
			setCode = _T( "35E004" );
		}
		break;

	case ID5032_26A:
	case ID5032_26B:
	case ID5032_26C:
		if ( F5032_26c & 0x8880 ) {
			code = _T( "359600" );
			setCode = _T( "35E005" );
		}
		break;

	case ID5032_27A:
	case ID5032_27B:
	case ID5032_27C:
		if ( F5032_27c & 0x8880 ) {
			code = _T( "359700" );
			setCode = _T( "35E006" );
		}
		break;

	case ID5032_28A:
	case ID5032_28B:
	case ID5032_28C:
		if ( F5032_28c & 0x8880 ) {
			code = _T( "359800" );
			setCode = _T( "35E007" );
		}
		break;

	case ID5032_29A:
	case ID5032_29B:
	case ID5032_29C:
		if ( F5032_29c & 0x8880 ) {
			code = _T( "359900" );
			setCode = _T( "35E008" );
		}
		break;

	case ID5032_30A:
	case ID5032_30B:
	case ID5032_30C:
		if ( F5032_30c & 0x8880 ) {
			code = _T( "35A000" );
			setCode = _T( "35E009" );
		}
		break;

	case ID5032_31A:
	case ID5032_31B:
	case ID5032_31C:
		if ( F5032_31c & 0x8880 ) {
			code = _T( "35A100" );
			setCode = _T( "35E00A" );
		}
		break;

	case ID5032_32A:
	case ID5032_32B:
	case ID5032_32C:
		if ( F5032_32c & 0x8880 ) {
			code = _T( "35A200" );
			setCode = _T( "35E00B" );
		}
		break;

	case ID5032_33A:
	case ID5032_33B:
	case ID5032_33C:
		if ( F5032_33c & 0x8880 ) {
			code = _T( "35A300" );
			setCode = _T( "35E00C" );
		}
		break;

	case ID5032_34A:
	case ID5032_34B:
	case ID5032_34C:
		if ( F5032_34c & 0x8880 ) {
			code = _T( "35A400" );
			setCode = _T( "35E00D" );
		}
		break;

	case ID5032_35A:
	case ID5032_35B:
	case ID5032_35C:
		if ( F5032_35c & 0x8880 ) {
			code = _T( "35A500" );
			setCode = _T( "35E00E" );
		}
		break;

	case ID5032_36A:
	case ID5032_36B:
	case ID5032_36C:
		if ( F5032_36c & 0x8880 ) {
			code = _T( "35A600" );
			setCode = _T( "35E00F" );
		}
		break;

	default:
		break;
	}

	if ( ( !code.IsEmpty() ) && ( !setCode.IsEmpty() ) ) {
		money = ( *m_pSyzSyukei )->GetShinkokuData ( code );
		memmove ( tmpMony, money[0][0].arith, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( setCode, tmpMony );
	}

	return 0;
}

//=========================================================
// 手入力クリア
//---------------------------------------------------------
// 引数
//			index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo5_032::ClearInput ( short index )
{
	switch ( index ) {
	case ID5032_21A:
	case ID5032_21B:
	case ID5032_21C:
		if ( index == ID5032_21A ) {
			F5032_21c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359102" ) );
		}
		else if ( index == ID5032_21B ) {
			F5032_21c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359103" ) );
		}
		else if ( index == ID5032_21C ) {
			F5032_21c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359100" ) );
		}

		if ( !( F5032_21c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E000" ) );
		}
		break;

	case ID5032_22A:
	case ID5032_22B:
	case ID5032_22C:
		if ( index == ID5032_22A ) {
			F5032_22c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359202" ) );
		}
		else if ( index == ID5032_22B ) {
			F5032_22c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359203" ) );
		}
		else if ( index == ID5032_22C ) {
			F5032_22c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359200" ) );
		}

		if ( !( F5032_22c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E001" ) );
		}
		break;

	case ID5032_23A:
	case ID5032_23B:
	case ID5032_23C:
		if ( index == ID5032_23A ) {
			F5032_23c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359302" ) );
		}
		else if ( index == ID5032_23B ) {
			F5032_23c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359303" ) );
		}
		else if ( index == ID5032_23C ) {
			F5032_23c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359300" ) );
		}

		if ( !( F5032_23c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E002" ) );
		}
		break;

	case ID5032_24A:
	case ID5032_24B:
	case ID5032_24C:
		if ( index == ID5032_24A ) {
			F5032_24c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359402" ) );
		}
		else if ( index == ID5032_24B ) {
			F5032_24c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359403" ) );
		}
		else if ( index == ID5032_24C ) {
			F5032_24c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359400" ) );
		}

		if ( !( F5032_24c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E003" ) );
		}
		break;

	case ID5032_25A:
	case ID5032_25B:
	case ID5032_25C:
		if ( index == ID5032_25A ) {
			F5032_25c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359502" ) );
		}
		else if ( index == ID5032_25B ) {
			F5032_25c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359503" ) );
		}
		else if ( index == ID5032_25C ) {
			F5032_25c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359500" ) );
		}

		if ( !( F5032_25c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E004" ) );
		}
		break;

	case ID5032_26A:
	case ID5032_26B:
	case ID5032_26C:
		if ( index == ID5032_26A ) {
			F5032_26c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359602" ) );
		}
		else if ( index == ID5032_26B ) {
			F5032_26c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359603" ) );
		}
		else if ( index == ID5032_26C ) {
			F5032_26c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359600" ) );
		}

		if ( !( F5032_26c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E005" ) );
		}
		break;

	case ID5032_27A:
	case ID5032_27B:
	case ID5032_27C:
		if ( index == ID5032_27A ) {
			F5032_27c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359702" ) );
		}
		else if ( index == ID5032_27B ) {
			F5032_27c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359703" ) );
		}
		else if ( index == ID5032_27C ) {
			F5032_27c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359700" ) );
		}

		if ( !( F5032_27c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E006" ) );
		}
		break;

	case ID5032_28A:
	case ID5032_28B:
	case ID5032_28C:
		if ( index == ID5032_28A ) {
			F5032_28c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359802" ) );
		}
		else if ( index == ID5032_28B ) {
			F5032_28c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359803" ) );
		}
		else if ( index == ID5032_28C ) {
			F5032_28c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359800" ) );
		}

		if ( !( F5032_28c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E007" ) );
		}
		break;

	case ID5032_29A:
	case ID5032_29B:
	case ID5032_29C:
		if ( index == ID5032_29A ) {
			F5032_29c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359902" ) );
		}
		else if ( index == ID5032_29B ) {
			F5032_29c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359903" ) );
		}
		else if ( index == ID5032_29C ) {
			F5032_29c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359900" ) );
		}

		if ( !( F5032_29c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E008" ) );
		}
		break;

	case ID5032_30A:
	case ID5032_30B:
	case ID5032_30C:
		if ( index == ID5032_30A ) {
			F5032_30c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A002" ) );
		}
		else if ( index == ID5032_30B ) {
			F5032_30c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A003" ) );
		}
		else if ( index == ID5032_30C ) {
			F5032_30c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A000" ) );
		}

		if ( !( F5032_30c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E009" ) );
		}
		break;

	case ID5032_31A:
	case ID5032_31B:
	case ID5032_31C:
		if ( index == ID5032_31A ) {
			F5032_31c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A102" ) );
		}
		else if ( index == ID5032_31B ) {
			F5032_31c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A103" ) );
		}
		else if ( index == ID5032_31C ) {
			F5032_31c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A100" ) );
		}

		if ( !( F5032_31c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00A" ) );
		}
		break;

	case ID5032_32A:
	case ID5032_32B:
	case ID5032_32C:
		if ( index == ID5032_32A ) {
			F5032_32c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A202" ) );
		}
		else if ( index == ID5032_32B ) {
			F5032_32c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A203" ) );
		}
		else if ( index == ID5032_32C ) {
			F5032_32c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A200" ) );
		}

		if ( !( F5032_32c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00B" ) );
		}
		break;

	case ID5032_33A:
	case ID5032_33B:
	case ID5032_33C:
		if ( index == ID5032_33A ) {
			F5032_33c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A302" ) );
		}
		else if ( index == ID5032_33B ) {
			F5032_33c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A303" ) );
		}
		else if ( index == ID5032_33C ) {
			F5032_33c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A300" ) );
		}

		if ( !( F5032_33c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00C" ) );
		}
		break;

	case ID5032_34A:
	case ID5032_34B:
	case ID5032_34C:
		if ( index == ID5032_34A ) {
			F5032_34c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A402" ) );
		}
		else if ( index == ID5032_34B ) {
			F5032_34c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A403" ) );
		}
		else if ( index == ID5032_34C ) {
			F5032_34c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A400" ) );
		}

		if ( !( F5032_34c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00D" ) );
		}
		break;

	case ID5032_35A:
	case ID5032_35B:
	case ID5032_35C:
		if ( index == ID5032_35A ) {
			F5032_35c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A502" ) );
		}
		else if ( index == ID5032_35B ) {
			F5032_35c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A503" ) );
		}
		else if ( index == ID5032_35C ) {
			F5032_35c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A500" ) );
		}

		if ( !( F5032_35c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00E" ) );
		}
		break;

	case ID5032_36A:
	case ID5032_36B:
	case ID5032_36C:
		if ( index == ID5032_36A ) {
			F5032_36c &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A602" ) );
		}
		else if ( index == ID5032_36B ) {
			F5032_36c &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A603" ) );
		}
		else if ( index == ID5032_36C ) {
			F5032_36c &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A600" ) );
		}

		if ( !( F5032_36c & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00F" ) );
		}
		break;

	default:
		break;
	}
}

//=========================================================
// 売上割合の取得
//---------------------------------------------------------
// 引数
//			id			：	取得する売上割合の事業種
//			pWari		：	売上割合
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_032::GetUriageWariai ( int id, unsigned short *pWari )
{
	ASSERT ( pWari );
	if ( pWari == NULL ) {
		return -1;
	}

	int ret = 0;
	CString tag, tmpTag;

	tag.Empty();
	switch ( id ) {
	case DAIICHISHU:
		tag = _T( "CTD00130" );
		break;
	case DAINISHU:
		tag = _T( "CTD00200" );
		break;
	case DAISANSHU:
		tag = _T( "CTD00270" );
		break;
	case DAIYONSHU:
		tag = _T( "CTD00340" );
		break;
	case DAIGOSHU:
		tag = _T( "CTD00410" );
		break;
	case DAIROKUSHU:
		tag = _T( "CTD00480" );
		break;
	default:
		break;
	}
	
	*pWari = 0;
	if ( !tag.IsEmpty() ) {
		tmpTag.Empty();
		( ( CH31HyoView* )m_pParent )->GetPercentage ( tag, tmpTag, pWari );
		ret = 0;
	}
	else {
		ret = -1;
	}

	return ret;
}

//=========================================================
// 売上割合の取得
//---------------------------------------------------------
// 引数
//			id			：	取得する売上割合の事業種
//			pWari		：	売上割合
//			pArith		：	合計金額
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_032::GetUriageWariai2 ( int id, unsigned short *pWari, char *pArith )
{
	ASSERT ( pWari );
	ASSERT ( pArith );
	if ( ( pWari == NULL ) || ( pArith == NULL ) ) {
		return -1;
	}

	int ret = 0;

	*pWari = 0;
	memset( pArith, '\0', MONY_BUF_SIZE );
	ret = ( ( CH31HyoView* )m_pParent )->GetPercentage2( id, pWari, pArith );

	return ret;
}

//=========================================================
// 出力する控除対象仕入税額をセット
//---------------------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//=========================================================
int CShinFhyo5_032::SetPrintArea()
{
	int ret = 0;
	int index = 0;
	bool errFlg = false;

	if ( !( ( *m_pSnHeadData )->Sn_GenericSgn & 0x40 ) ) {
		return 0;
	}

	// 一度リセットしておく
	AllPrint();

	// 入力ロック中等、m_idがクリアされていることがあったため、別変数を用意
	//switch ( m_id ) {
	switch ( m_PrintId ) {
	case ID5032_21C:
		F5032_21c |= 0x01;
		break;
	case ID5032_22C:
		F5032_22c |= 0x01;
		break;
	case ID5032_23C:
		F5032_23c |= 0x01;
		break;
	case ID5032_24C:
		F5032_24c |= 0x01;
		break;
	case ID5032_25C:
		F5032_25c |= 0x01;
		break;
	case ID5032_26C:
		F5032_26c |= 0x01;
		break;
	case ID5032_27C:
		F5032_27c |= 0x01;
		break;
	case ID5032_28C:
		F5032_28c |= 0x01;
		break;
	case ID5032_29C:
		F5032_29c |= 0x01;
		break;
	case ID5032_30C:
		F5032_30c |= 0x01;
		break;
	case ID5032_31C:
		F5032_31c |= 0x01;
		break;
	case ID5032_32C:
		F5032_32c |= 0x01;
		break;
	case ID5032_33C:
		F5032_33c |= 0x01;
		break;
	case ID5032_34C:
		F5032_34c |= 0x01;
		break;
	case ID5032_35C:
		F5032_35c |= 0x01;
		break;
	case ID5032_36C:
		F5032_36c |= 0x01;
		break;
	default:
		errFlg = true;
		break;
	}

	if ( errFlg ) {
		return -1;
	}

	return 0;
}

//=========================================================
// 全項目出力
//=========================================================
void CShinFhyo5_032::AllPrint()
{
	F5032_20c &= ~0x01;
	F5032_21c &= ~0x01;
	F5032_22c &= ~0x01;
	F5032_23c &= ~0x01;
	F5032_24c &= ~0x01;
	F5032_25c &= ~0x01;
	F5032_26c &= ~0x01;
	F5032_27c &= ~0x01;
	F5032_28c &= ~0x01;
	F5032_29c &= ~0x01;
	F5032_30c &= ~0x01;
	F5032_31c &= ~0x01;
	F5032_32c &= ~0x01;
	F5032_33c &= ~0x01;
	F5032_34c &= ~0x01;
	F5032_35c &= ~0x01;
	F5032_36c &= ~0x01;
}

//=========================================================
// 入力項目？
//---------------------------------------------------------
// 返送値
//			0			:		非入力項目
//			1			:		入力項目
//			-1			:		エラー
//=========================================================
int CShinFhyo5_032::IsInputArea ( short index )
{
	int ret = 0;

	// 単一業種の場合は不可
	if ( ( m_typecnt < 2 ) || ( m_4val == 0 ) ) {
		ret = 0;
	}
	else {
		if ( IsDispArea ( index ) == 1 ) {
			ret = 1;
		}
		else {
			ret = 0;
		}
	}

	return ret;
}

//=========================================================
// 特例計算取得
//---------------------------------------------------------
// 返送値
//		0			:			正常終了
//=========================================================
int CShinFhyo5_032::GetTokurei()
{
	MoneyBasejagArray money;
	int count = 0, inCount = 0;
	char tmpMony[MONY_BUF_SIZE] = { 0 };
	int *pDispSign = NULL;
	bool bGet = false;

	if( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		money = ( *m_pSyzSyukei )->GetShinkokuData( _T( "35" ) );

		count = ( int )money.GetCount();
		for( int i = 0; i < count; i++ ) {
			inCount = ( int )money[i].GetCount();
			for( int j = 0; j < inCount; j++ ) {
				memset( tmpMony, '\0', MONY_BUF_SIZE );
				pDispSign = NULL;
				bGet = false;

				if( money[i][j].code == _T( "35E200" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_21Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E201" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_22Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E202" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_23Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E203" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_24Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E204" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_25Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E205" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_26Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E206" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_27Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E207" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_28Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E208" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_29Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E209" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_30Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20A" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_31Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20B" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_32Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20C" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_33Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20D" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_34Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20E" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_35Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20F" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5032_36Disp;
					bGet = true;
				}

				if( bGet ) {
					ASSERT( pDispSign );
					if( pDispSign ) {
						if( m_pArith->l_test( tmpMony ) > 0 ) {
							*pDispSign |= 0x01;
						}
						else {
							*pDispSign &= ~0x01;
						}
					}
				}
			}
		}
	}
	else {
		// 入力ロック中は全表示
		F5032_21Disp |= 0x01;
		F5032_22Disp |= 0x01;
		F5032_23Disp |= 0x01;
		F5032_24Disp |= 0x01;
		F5032_25Disp |= 0x01;
		F5032_26Disp |= 0x01;
		F5032_27Disp |= 0x01;
		F5032_28Disp |= 0x01;
		F5032_29Disp |= 0x01;
		F5032_30Disp |= 0x01;
		F5032_31Disp |= 0x01;
		F5032_32Disp |= 0x01;
		F5032_33Disp |= 0x01;
		F5032_34Disp |= 0x01;
		F5032_35Disp |= 0x01;
		F5032_36Disp |= 0x01;
	}

	return 0;
}

/* // 付表5 控除対象仕入税額の選択をICSP連携専用の機能とするため一時的にカット
//=========================================================
// 選択された項目の控除対象仕入税額をセット
//---------------------------------------------------------
// 引数
//			selId		：	選択された項目のID
//=========================================================
void CShinFhyo5_032::SetSelectCode ( int  selId )
{
	bool errFlg = false;

	switch ( selId ) {
	case ID_F520:
		F5032_20c |= 0x02;
		break;
	case ID_F521:
		F5032_21c |= 0x02;
		break;
	case ID_F522:
		F5032_22c |= 0x02;
		break;
	case ID_F523:
		F5032_23c |= 0x02;
		break;
	case ID_F524:
		F5032_24c |= 0x02;
		break;
	case ID_F525:
		F5032_25c |= 0x02;
		break;
	case ID_F526:
		F5032_26c |= 0x02;
		break;
	case ID_F527:
		F5032_27c |= 0x02;
		break;
	case ID_F528:
		F5032_28c |= 0x02;
		break;
	case ID_F529:
		F5032_29c |= 0x02;
		break;
	case ID_F530:
		F5032_30c |= 0x02;
		break;
	case ID_F531:
		F5032_31c |= 0x02;
		break;
	case ID_F532:
		F5032_32c |= 0x02;
		break;
	case ID_F533:
		F5032_33c |= 0x02;
		break;
	case ID_F534:
		F5032_34c |= 0x02;
		break;
	case ID_F535:
		F5032_35c |= 0x02;
		break;
	case ID_F536:
		F5032_36c |= 0x02;
		break;
	default:
		errFlg = true;
		break;
	}

	if ( errFlg ) {
		return;
	}
	
	if ( ChangeBackColor ( TRUE ) == 0 ) {
		m_Fhyo5_032Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_OPAQUE );
		m_Fhyo5_032Diag01.Refresh();
	}

	SetKjCode();
}

//=========================================================
// 控除対象仕入税額の項目指定
//=========================================================
void CShinFhyo5_032::SetKjCode()
{
	char setFlg[MONY_BUF_SIZE] = { 0 };
	char buf[32] = { 0 };
	int itemID = 0;
	bool zeroFlg = true;

	if ( F5032_20c & 0x02 ) {
		itemID = ID_F520;
		zeroFlg = false;
	}
	else if ( F5032_21c & 0x02 ) {
		itemID = ID_F521;
	}
	else if ( F5032_22c & 0x02 ) {
		itemID = ID_F522;
	}
	else if ( F5032_23c & 0x02 ) {
		itemID = ID_F523;
	}
	else if ( F5032_24c & 0x02 ) {
		itemID = ID_F524;
	}
	else if ( F5032_25c & 0x02 ) {
		itemID = ID_F525;
	}
	else if ( F5032_26c & 0x02 ) {
		itemID = ID_F526;
	}
	else if ( F5032_27c & 0x02 ) {
		itemID = ID_F527;
	}
	else if ( F5032_28c & 0x02 ) {
		itemID = ID_F528;
	}
	else if ( F5032_29c & 0x02 ) {
		itemID = ID_F529;
	}
	else if ( F5032_30c & 0x02 ) {
		itemID = ID_F530;
	}
	else if ( F5032_31c & 0x02 ) {
		itemID = ID_F531;
	}
	else if ( F5032_32c & 0x02 ) {
		itemID = ID_F532;
	}
	else if ( F5032_33c & 0x02 ) {
		itemID = ID_F533;
	}
	else if ( F5032_34c & 0x02 ) {
		itemID = ID_F534;
	}
	else if ( F5032_35c & 0x02 ) {
		itemID = ID_F535;
	}
	else if ( F5032_36c & 0x02 ) {
		itemID = ID_F536;
	}
	else {
		return;
	}

	memset ( setFlg, '\0', sizeof ( setFlg ) );
	memset ( buf, '\0', sizeof ( buf ) );
	sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), itemID );
	m_pArith->l_input ( setFlg, buf );

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "362006" ), setFlg );

	if ( zeroFlg ) {
		memset ( setFlg, '\0', sizeof ( setFlg ) );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "362007" ), setFlg );
	}
	else {
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "362007" ) );
	}
}

//=========================================================
// 控除対象仕入税額の自動集計
//=========================================================
void CShinFhyo5_032::AutoKjCode()
{
	ChangeBackColor ( FALSE );
	m_Fhyo5_032Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_TRANSPARENT );
	m_Fhyo5_032Diag01.Refresh();

	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "362006" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "362007" ) );

	F5032_20c &= ~0x02;
	F5032_21c &= ~0x02;
	F5032_22c &= ~0x02;
	F5032_23c &= ~0x02;
	F5032_24c &= ~0x02;
	F5032_25c &= ~0x02;
	F5032_26c &= ~0x02;
	F5032_27c &= ~0x02;
	F5032_28c &= ~0x02;
	F5032_29c &= ~0x02;
	F5032_30c &= ~0x02;
	F5032_31c &= ~0x02;
	F5032_32c &= ~0x02;
	F5032_33c &= ~0x02;
	F5032_34c &= ~0x02;
	F5032_35c &= ~0x02;
	F5032_36c &= ~0x02;
}

//=========================================================
// 選択された項目の背景色変更
//---------------------------------------------------------
// 引数
//			bSw		:		TRUE = ON, FALSE = OFF
//---------------------------------------------------------
// 返送値
//			0		:		正常終了
//			-1		:		エラー
//=========================================================
int CShinFhyo5_032::ChangeBackColor ( BOOL bSw )
{
	int s_idx = 0, e_idx;
	int colNo = 0;

	if ( F5032_20c & 0x02 ) {
		s_idx = ID5032_NO20_S;
		e_idx = ID5032_NO20_E;
	}
	else if ( F5032_21c & 0x02 ) {
		s_idx = ID5032_NO21_S;
		e_idx = ID5032_NO21_E;
	}
	else if ( F5032_22c & 0x02 ) {
		s_idx = ID5032_NO22_S;
		e_idx = ID5032_NO22_E;
	}
	else if ( F5032_23c & 0x02 ) {
		s_idx = ID5032_NO23_S;
		e_idx = ID5032_NO23_E;
	}
	else if ( F5032_24c & 0x02 ) {
		s_idx = ID5032_NO24_S;
		e_idx = ID5032_NO24_E;
	}
	else if ( F5032_25c & 0x02 ) {
		s_idx = ID5032_NO25_S;
		e_idx = ID5032_NO25_E;
	}
	else if ( F5032_26c & 0x02 ) {
		s_idx = ID5032_NO26_S;
		e_idx = ID5032_NO26_E;
	}
	else if ( F5032_27c & 0x02 ) {
		s_idx = ID5032_NO27_S;
		e_idx = ID5032_NO27_E;
	}
	else if ( F5032_28c & 0x02 ) {
		s_idx = ID5032_NO28_S;
		e_idx = ID5032_NO28_E;
	}
	else if ( F5032_29c & 0x02 ) {
		s_idx = ID5032_NO29_S;
		e_idx = ID5032_NO29_E;
	}
	else if ( F5032_30c & 0x02 ) {
		s_idx = ID5032_NO30_S;
		e_idx = ID5032_NO30_E;
	}
	else if ( F5032_31c & 0x02 ) {
		s_idx = ID5032_NO31_S;
		e_idx = ID5032_NO31_E;
	}
	else if ( F5032_32c & 0x02 ) {
		s_idx = ID5032_NO32_S;
		e_idx = ID5032_NO32_E;
	}
	else if ( F5032_33c & 0x02 ) {
		s_idx = ID5032_NO33_S;
		e_idx = ID5032_NO33_E;
	}
	else if ( F5032_34c & 0x02 ) {
		s_idx = ID5032_NO34_S;
		e_idx = ID5032_NO34_E;
	}
	else if ( F5032_35c & 0x02 ) {
		s_idx = ID5032_NO35_S;
		e_idx = ID5032_NO35_E;
	}
	else if ( F5032_36c & 0x02 ) {
		s_idx = ID5032_NO36_S;
		e_idx = ID5032_NO36_E;
	}
	else {
		;
	}

	if ( ( s_idx != 0 ) && ( e_idx != 0 ) ) {
		if ( bSw ) {
			colNo = 4;
		}
		else {
			colNo = 1;
		}

		for ( int i = s_idx; i <= e_idx; i++ ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, i, colNo );
		}
	}
	else {
		return -1;
	}

	return 0;
}

//=========================================================
// 選択された控除対象仕入税額のクリア
//=========================================================
void CShinFhyo5_032::ResetKjCode()
{
	// 控除対象仕入税額を選択中だが付表5-3(1)で売上割合が変更された場合の対応

	if ( !( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) ) {
		return;
	}
	
	AutoKjCode();

	char tmpID[MONY_BUF_SIZE] = { 0 };
	char buf[32] = { 0 };
	int nID = 0, selID = 0;

	MoneyBasejagArray money;

	memset ( tmpID, '\0', sizeof ( tmpID ) );
	memset ( buf, '\0', sizeof ( buf ) );

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362007" ) );
	memmove ( tmpID, money[0][0].arith, sizeof ( tmpID ) );
	m_pArith->l_print ( buf, tmpID, _T( "SSSS") );
	nID = atoi ( buf );

	if ( !nID ) {
		memset ( tmpID, '\0', sizeof ( tmpID ) );
		memset ( buf, '\0', sizeof ( buf ) );
		nID = 0;

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362006" ) );
		memmove ( tmpID, money[0][0].arith, sizeof ( tmpID ) );
		m_pArith->l_print ( buf, tmpID, _T( "SSSSSSSSSSSSS9") );
		nID = atoi ( buf );
	}

	selID = nID;

	SetSelectCode ( selID );
}


//=========================================================
// 控除対象仕入税額の再セット必要
//---------------------------------------------------------
// 引数
//		bRet		:		true  = 再セット必要
//							false = 不必要
//=========================================================
bool CShinFhyo5_032::IsKjCodeReset()
{
	bool bRet = false;
	int id = 0, st = 0;
	if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) {
		st = GetSelectItem ( id );
		if ( ( st != -1 ) && ( id != 0 ) ) {
			if ( IsDispArea ( id ) != 1 ) {
				bRet = true;
			}
		}
	}

	return bRet;
}

//=========================================================
// 選択中の項目取得
//---------------------------------------------------------
// 引数
//			getID		:		選択中の項目のA欄インデックス
//---------------------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//=========================================================
int CShinFhyo5_032::GetSelectItem ( int &getID )
{
	int id = 0;

	if ( F5032_20c & 0x02 ) {
		id = ID5032_20A;
	}
	else if ( F5032_21c & 0x02 ) {
		id = ID5032_21A;
	}
	else if ( F5032_22c & 0x02 ) {
		id = ID5032_22A;
	}
	else if ( F5032_23c & 0x02 ) {
		id = ID5032_23A;
	}
	else if ( F5032_24c & 0x02 ) {
		id = ID5032_24A;
	}
	else if ( F5032_25c & 0x02 ) {
		id = ID5032_25A;
	}
	else if ( F5032_26c & 0x02 ) {
		id = ID5032_26A;
	}
	else if ( F5032_27c & 0x02 ) {
		id = ID5032_27A;
	}
	else if ( F5032_28c & 0x02 ) {
		id = ID5032_28A;
	}
	else if ( F5032_29c & 0x02 ) {
		id = ID5032_29A;
	}
	else if ( F5032_30c & 0x02 ) {
		id = ID5032_30A;
	}
	else if ( F5032_31c & 0x02 ) {
		id = ID5032_31A;
	}
	else if ( F5032_32c & 0x02 ) {
		id = ID5032_32A;
	}
	else if ( F5032_33c & 0x02 ) {
		id = ID5032_33A;
	}
	else if ( F5032_34c & 0x02 ) {
		id = ID5032_34A;
	}
	else if ( F5032_35c & 0x02 ) {
		id = ID5032_35A;
	}
	else if ( F5032_36c & 0x02 ) {
		id = ID5032_36A;
	}
	else {
		id = -1;
	}

	getID = 0;
	if ( id != -1 ) {
		getID = id;
	}
	else {
		AutoKjCode();
	}

	return 0;
}

//=========================================================
// 再集計
//=========================================================
void CShinFhyo5_032::ReSyukei()
{
	MemoryReget();
	f5032data_all ( 0 );
}*/