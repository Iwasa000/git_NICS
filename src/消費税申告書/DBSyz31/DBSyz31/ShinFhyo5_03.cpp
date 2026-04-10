// ShinFhyo5_03.cpp : 実装ファイル
//

//#include "pch.h"
#include "stdafx.h"
#include "ShinFhyo5_03.h"
#include "ShinFhyo5_03idx.h"
//#include "afxdialogex.h"


// ShinFhyo5_03 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo5_03, CSyzBaseDlg)

CShinFhyo5_03::CShinFhyo5_03(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo5_03::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnHeadData ( NULL )
	, m_pSnFhyo5_01Data( NULL )
	, m_pSyzSyukei ( NULL )
{
	memset ( F503_1A, '\0', sizeof ( F503_1A ) );
	memset ( F503_1B, '\0', sizeof ( F503_1B ) );
	memset ( F503_1C, '\0', sizeof ( F503_1C ) );

	memset ( F503_2A, '\0', sizeof ( F503_2A ) );
	memset ( F503_2B, '\0', sizeof ( F503_2B ) );
	memset ( F503_2C, '\0', sizeof ( F503_2C ) );

	memset ( F503_3A, '\0', sizeof ( F503_3A ) );
	memset ( F503_3B, '\0', sizeof ( F503_3B ) );
	memset ( F503_3C, '\0', sizeof ( F503_3C ) );

	memset ( F503_4A, '\0', sizeof ( F503_4A ) );
	memset ( F503_4B, '\0', sizeof ( F503_4B ) );
	memset ( F503_4C, '\0', sizeof ( F503_4C ) );

	memset ( F503_5A, '\0', sizeof ( F503_5A ) );
	memset ( F503_5B, '\0', sizeof ( F503_5B ) );
	memset ( F503_5C, '\0', sizeof ( F503_5C ) );

	memset ( F503_6A, '\0', sizeof ( F503_6A ) );
	memset ( F503_6B, '\0', sizeof ( F503_6B ) );
	memset ( F503_6C, '\0', sizeof ( F503_6C ) );

	memset ( F503_7A, '\0', sizeof ( F503_7A ) );
	memset ( F503_7B, '\0', sizeof ( F503_7B ) );
	memset ( F503_7C, '\0', sizeof ( F503_7C ) );

	memset ( F503_8A, '\0', sizeof ( F503_8A ) );
	memset ( F503_8B, '\0', sizeof ( F503_8B ) );
	memset ( F503_8C, '\0', sizeof ( F503_8C ) );

	memset ( F503_9A, '\0', sizeof ( F503_9A ) );
	memset ( F503_9B, '\0', sizeof ( F503_9B ) );
	memset ( F503_9C, '\0', sizeof ( F503_9C ) );

	memset ( F503_10A, '\0', sizeof ( F503_10A ) );
	memset ( F503_10B, '\0', sizeof ( F503_10B ) );
	memset ( F503_10C, '\0', sizeof ( F503_10C ) );

	memset ( F503_11A, '\0', sizeof ( F503_11A ) );
	memset ( F503_11B, '\0', sizeof ( F503_11B ) );
	memset ( F503_11C, '\0', sizeof ( F503_11C ) );

	memset ( F503_12A, '\0', sizeof ( F503_12A ) );
	memset ( F503_12B, '\0', sizeof ( F503_12B ) );
	memset ( F503_12C, '\0', sizeof ( F503_12C ) );

	memset ( F503_13A, '\0', sizeof ( F503_13A ) );
	memset ( F503_13B, '\0', sizeof ( F503_13B ) );
	memset ( F503_13C, '\0', sizeof ( F503_13C ) );

	memset ( F503_14A, '\0', sizeof ( F503_14A ) );
	memset ( F503_14B, '\0', sizeof ( F503_14B ) );
	memset ( F503_14C, '\0', sizeof ( F503_14C ) );

	memset ( F503_15A, '\0', sizeof ( F503_15A ) );
	memset ( F503_15B, '\0', sizeof ( F503_15B ) );
	memset ( F503_15C, '\0', sizeof ( F503_15C ) );

	memset ( F503_16A, '\0', sizeof ( F503_16A ) );
	memset ( F503_16B, '\0', sizeof ( F503_16B ) );
	memset ( F503_16C, '\0', sizeof ( F503_16C ) );

	memset ( F503_17A, '\0', sizeof ( F503_17A ) );
	memset ( F503_17B, '\0', sizeof ( F503_17B ) );
	memset ( F503_17C, '\0', sizeof ( F503_17C ) );

	memset ( F503_18A, '\0', sizeof ( F503_18A ) );
	memset ( F503_18B, '\0', sizeof ( F503_18B ) );
	memset ( F503_18C, '\0', sizeof ( F503_18C ) );

	memset ( F503_19A, '\0', sizeof ( F503_19A ) );
	memset ( F503_19B, '\0', sizeof ( F503_19B ) );
	memset ( F503_19C, '\0', sizeof ( F503_19C ) );


	F503_7UP = F503_8UP = F503_9UP = F503_10UP = F503_11UP = F503_12UP = 0;
	F503_5kbn = 0;
	typeCnt = 0;

	// 手入力サイン
	F503_6c = F503_14c = F503_15c = F503_16c = F503_17c = F503_18c = F503_19c = 0;
	F503_7c = F503_8c = F503_9c = F503_10c = F503_11c = F503_12c = 0;

	m_curwnd = -1;

}

CShinFhyo5_03::~CShinFhyo5_03()
{
}

void CShinFhyo5_03::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo5_03Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo5_03Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo5_03, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo5_03, CSyzBaseDlg)
	ON_EVENT(CShinFhyo5_03, IDC_ICSDIAG8CTRL2, 1, CShinFhyo5_03::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo5_03, IDC_ICSDIAG8CTRL2, 2, CShinFhyo5_03::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo5_03, IDC_ICSDIAG8CTRL2, 3, CShinFhyo5_03::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// ShinFhyo5_03 メッセージ ハンドラー


BOOL CShinFhyo5_03::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL2 );
	m_BottomIdx = 154;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo5_03::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo5_03::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo5_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BOOL CShinFhyo5_03::PreTranslateMessage(MSG* pMsg)
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
				if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}

				return TRUE;
			}
		}
	}
	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

void CShinFhyo5_03::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo5_03::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char* ptmpMony = NULL;
	char  CkMy[MONY_BUF_SIZE] = { 0 };
	int	  tmpidx = 0;

	switch ( index ) {
		// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
		// （１）事業区分別の課税売上高(税抜)の明細 
		// 事業区分別の合計額
	case ID503_6A:
		// 7A欄～12A欄に金額がある場合集計する
		if ( ( m_pArith->l_test ( F503_7A ) ) || ( m_pArith->l_test ( F503_8A ) ) || ( m_pArith->l_test ( F503_9A ) ) ||
			( m_pArith->l_test ( F503_10A ) ) || ( m_pArith->l_test ( F503_11A ) ) || ( m_pArith->l_test ( F503_12A ) ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;
	case ID503_6B:
		// 7B欄～12B欄に金額がある場合集計する
		if ( ( m_pArith->l_test ( F503_7B ) ) || ( m_pArith->l_test ( F503_8B ) ) || ( m_pArith->l_test ( F503_9B ) ) ||
			( m_pArith->l_test ( F503_10B ) ) || ( m_pArith->l_test ( F503_11B ) ) || ( m_pArith->l_test ( F503_12B ) ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;
		//<-------------------------------------

		// 第一種事業
	case ID503_7A:
	case ID503_7B:
		// 第二種事業
	case ID503_8A:
	case ID503_8B:
		// 第三種事業
	case ID503_9A:
	case ID503_9B:
		// 第四種事業
	case ID503_10A:
	case ID503_10B:
		// 第五種事業
	case ID503_11A:
	case ID503_11B:
		// 第六種事業
	case ID503_12A:
	case ID503_12B:
		SyukeiDataSet ( index );
		f503data_all ( 5 );
		break;

	case ID503_14A:
	case ID503_14B:
		if ( index == ID503_14A ) {
			tmpidx = ID503_7A;
		}
		else if ( index == ID503_14B ) {
			tmpidx = ID503_7B;
		}
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_03Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;

	case ID503_15A:
	case ID503_15B:
		if ( index == ID503_15A ) {
			tmpidx = ID503_8A;
		}
		else if ( index == ID503_15B ) {
			tmpidx = ID503_8B;
		}
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_03Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;

	case ID503_16A:
	case ID503_16B:
		if ( index == ID503_16A ) {
			tmpidx = ID503_9A;
		}
		else if ( index == ID503_16B ) {
			tmpidx = ID503_9B;
		}
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_03Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;

	case ID503_17A:
	case ID503_17B:
		if ( index == ID503_17A ) {
			tmpidx = ID503_10A;
		}
		else if ( index == ID503_17B ) {
			tmpidx = ID503_10B;
		}
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_03Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;

	case ID503_18A:
	case ID503_18B:
		if ( index == ID503_18A ) {
			tmpidx = ID503_11A;
		}
		else if ( index == ID503_18B ) {
			tmpidx = ID503_11B;
		}
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_03Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;

	case ID503_19A:
	case ID503_19B:
		if ( index == ID503_19A ) {
			tmpidx = ID503_12A;
		}
		else if ( index == ID503_19B ) {
			tmpidx = ID503_12B;
		}
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_03Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		f503data_all ( 5 );
		break;
	default:
		break;
	}
}

void CShinFhyo5_03::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに変換
	short wTerm = 0;
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
	if ( wTerm == VK_UP ) {
		pos = UpDownFocusMoveCheck ( index, 1 );
		m_Fhyo5_03Diag02.SetPosition ( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index, 0 );
		m_Fhyo5_03Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) ) {
		m_Fhyo5_03Diag02.SetNextPosition();
	}
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo5_03Diag02.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
		case ID503_6A:
		case ID503_6B:
			if ( index == ID503_6A ) {
				F503_6c &=~ 0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357602" ) );
			}
			else if ( index == ID503_6B ) {
				F503_6c &=~ 0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357603" ) );
			}
			f503data_all ( 6 );
			m_Fhyo5_03Diag02.SetPosition( index );
			break;

		case ID503_7A:
		case ID503_7B:
			//if ( index == ID503_7B ) {
			if ( index == ID503_7A ) {
				F503_14c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358402" ) );
			}
			else if ( index == ID503_7B ) {
				F503_14c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358403" ) );
			}
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID503_8A:
		case ID503_8B:
			if ( index == ID503_8A ) {
				F503_15c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358502" ) );
			}
			else if ( index == ID503_8B ) {
				F503_15c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358503" ) );
			}
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID503_9A:
		case ID503_9B:
			if( index == ID503_9A ){
				F503_16c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358602" ) );
			}
			else if ( index == ID503_9B ) {
				F503_16c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358603" ) );
			}
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID503_10A:
		case ID503_10B:
			if ( index == ID503_10A ) {
				F503_17c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358702" ) );
			}
			else if ( index == ID503_10B ) {
				F503_17c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358703" ) );
			}
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID503_11A:
		case ID503_11B:
			if ( index == ID503_11A ) {
				F503_18c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358802" ) );
			}
			else if ( index == ID503_11B ) {
				F503_18c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358803" ) );
			}
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID503_12A:
		case ID503_12B:
			if ( index == ID503_12A ) {
				F503_19c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358902" ) );
			}
			else if ( index == ID503_12B ) {
				F503_19c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358903" ) );
			}
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID503_14A:
		case ID503_14B:
			if ( index == ID503_14A ) {
				F503_14c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358402" ) );
			}
			else if ( index == ID503_14B ) {
				F503_14c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358403" ) );
			}
			f503data_all ( 13 );
			m_Fhyo5_03Diag02.SetPosition ( index );
			break;
		case ID503_15A:
		case ID503_15B:
			if ( index == ID503_15A ) {
				F503_15c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358502" ) );
			}
			else if ( index == ID503_15B ) {
				F503_15c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358503" ) );
			}
			f503data_all ( 13 );
			m_Fhyo5_03Diag02.SetPosition ( index );
			break;
		case ID503_16A:
		case ID503_16B:
			if ( index == ID503_16A ) {
				F503_16c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358602" ) );
			}
			else if ( index == ID503_16B ) {
				F503_16c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358603" ) );
			}
			f503data_all ( 13 );
			m_Fhyo5_03Diag02.SetPosition ( index );
			break;
		case ID503_17A:
		case ID503_17B:
			if ( index == ID503_17A ) {
				F503_17c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358702" ) );
			}
			else if ( index == ID503_17B )  {
				F503_17c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358703" ) );
			}
			f503data_all ( 13 );
			m_Fhyo5_03Diag02.SetPosition ( index );
			break;
		case ID503_18A:
		case ID503_18B:
			if ( index == ID503_18A ) {
				F503_18c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358802" ) );
			}
			else if ( index == ID503_18B ) {
				F503_18c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358803"));
			}
			f503data_all ( 13 );
			m_Fhyo5_03Diag02.SetPosition ( index );
			break;
		case ID503_19A:
		case ID503_19B:
			if ( index == ID503_19A ) {
				F503_19c &= ~0x80;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358902" ) );
			}
			else if ( index == ID503_19B ) {
				F503_19c &= ~0x800;
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358903" ) );
			}
			f503data_all ( 13 );
			m_Fhyo5_03Diag02.SetPosition ( index );
			break;
		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		}
	}

	int nowpos = m_Fhyo5_03Diag02.GetPosition();

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo5_01Data	：	付表5-1データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pSyzSyukei		：	集計クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_03::InitInfo ( CH31SnFhyo5_01Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CSyzSyukei **pSyzSyukei )
{
	ASSERT ( pSnFhyo5_01Data );
	if ( pSnFhyo5_01Data == NULL ) {
		return -1;
	}
	m_pSnFhyo5_01Data = pSnFhyo5_01Data;

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT ( pArith );
	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

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
int CShinFhyo5_03::Init()
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
		m_Fhyo5_03Diag01.ModifyItem ( 10, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh5_03();

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
int CShinFhyo5_03::ReadData()
{
	// 課税標準額に対する消費税額
	memmove ( F503_1A, ( *m_pSnFhyo5_01Data )->Sn_F501_1D, MONY_BUF_SIZE );
	memmove ( F503_1B, ( *m_pSnFhyo5_01Data )->Sn_F501_1E, MONY_BUF_SIZE );
	memmove ( F503_1C, ( *m_pSnFhyo5_01Data )->Sn_F501_1F, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	memmove ( F503_2A, ( *m_pSnFhyo5_01Data )->Sn_F501_2D, MONY_BUF_SIZE );
	memmove ( F503_2B, ( *m_pSnFhyo5_01Data )->Sn_F501_2E, MONY_BUF_SIZE );
	memmove ( F503_2C, ( *m_pSnFhyo5_01Data )->Sn_F501_2F, MONY_BUF_SIZE );

	// 売上対価の返還等に係る消費税額
	memmove ( F503_3A, ( *m_pSnFhyo5_01Data )->Sn_F501_3D, MONY_BUF_SIZE );
	memmove ( F503_3B, ( *m_pSnFhyo5_01Data )->Sn_F501_3E, MONY_BUF_SIZE );
	memmove ( F503_3C, ( *m_pSnFhyo5_01Data )->Sn_F501_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove ( F503_4A, ( *m_pSnFhyo5_01Data )->Sn_F501_4D, MONY_BUF_SIZE );
	memmove ( F503_4B, ( *m_pSnFhyo5_01Data )->Sn_F501_4E, MONY_BUF_SIZE );
	memmove ( F503_4C, ( *m_pSnFhyo5_01Data )->Sn_F501_4F, MONY_BUF_SIZE );

	// 1種類の事業の専業者の場合の控除対象仕入税額
	memmove ( F503_5A, ( *m_pSnFhyo5_01Data )->Sn_F501_5D, MONY_BUF_SIZE );
	memmove ( F503_5B, ( *m_pSnFhyo5_01Data )->Sn_F501_5E, MONY_BUF_SIZE );
	memmove ( F503_5C, ( *m_pSnFhyo5_01Data )->Sn_F501_5F, MONY_BUF_SIZE );

	// 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	memmove ( F503_6A, ( *m_pSnFhyo5_01Data )->Sn_F501_6D, MONY_BUF_SIZE );
	memmove ( F503_6B, ( *m_pSnFhyo5_01Data )->Sn_F501_6E, MONY_BUF_SIZE );
	memmove ( F503_6C, ( *m_pSnFhyo5_01Data )->Sn_F501_6F, MONY_BUF_SIZE );
	F503_6c = ( *m_pSnFhyo5_01Data )->Sn_F501_6sw;	// 手入力サイン

	// 第一種事業(卸売業)
	memmove ( F503_7A, ( *m_pSnFhyo5_01Data )->Sn_F501_7D, MONY_BUF_SIZE );
	memmove ( F503_7B, ( *m_pSnFhyo5_01Data )->Sn_F501_7E, MONY_BUF_SIZE );
	memmove ( F503_7C, ( *m_pSnFhyo5_01Data )->Sn_F501_7F, MONY_BUF_SIZE );

	// 第二種事業(小売業)
	memmove ( F503_8A, ( *m_pSnFhyo5_01Data )->Sn_F501_8D, MONY_BUF_SIZE );
	memmove ( F503_8B, ( *m_pSnFhyo5_01Data )->Sn_F501_8E, MONY_BUF_SIZE );
	memmove ( F503_8C, ( *m_pSnFhyo5_01Data )->Sn_F501_8F, MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	memmove ( F503_9A, ( *m_pSnFhyo5_01Data )->Sn_F501_9D, MONY_BUF_SIZE );
	memmove ( F503_9B, ( *m_pSnFhyo5_01Data )->Sn_F501_9E, MONY_BUF_SIZE );
	memmove ( F503_9C, ( *m_pSnFhyo5_01Data )->Sn_F501_9F, MONY_BUF_SIZE );

	// 第四種事業(その他)
	memmove ( F503_10A, ( *m_pSnFhyo5_01Data )->Sn_F501_10D, MONY_BUF_SIZE );
	memmove ( F503_10B, ( *m_pSnFhyo5_01Data )->Sn_F501_10E, MONY_BUF_SIZE );
	memmove ( F503_10C, ( *m_pSnFhyo5_01Data )->Sn_F501_10F, MONY_BUF_SIZE );

	// 第五種事業(サービス業)
	memmove ( F503_11A, ( *m_pSnFhyo5_01Data )->Sn_F501_11D, MONY_BUF_SIZE );
	memmove ( F503_11B, ( *m_pSnFhyo5_01Data )->Sn_F501_11E, MONY_BUF_SIZE );
	memmove ( F503_11C, ( *m_pSnFhyo5_01Data )->Sn_F501_11F, MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	memmove ( F503_12A, ( *m_pSnFhyo5_01Data )->Sn_F501_12D, MONY_BUF_SIZE );
	memmove ( F503_12B, ( *m_pSnFhyo5_01Data )->Sn_F501_12E, MONY_BUF_SIZE );
	memmove ( F503_12C, ( *m_pSnFhyo5_01Data )->Sn_F501_12F, MONY_BUF_SIZE );

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	memmove ( F503_13A, ( *m_pSnFhyo5_01Data )->Sn_F501_13D, MONY_BUF_SIZE );
	memmove ( F503_13B, ( *m_pSnFhyo5_01Data )->Sn_F501_13E, MONY_BUF_SIZE );
	memmove ( F503_13C, ( *m_pSnFhyo5_01Data )->Sn_F501_13F, MONY_BUF_SIZE );

	// 第一種事業(卸売業)
	memmove ( F503_14A, ( *m_pSnFhyo5_01Data )->Sn_F501_14D, MONY_BUF_SIZE );
	memmove ( F503_14B, ( *m_pSnFhyo5_01Data )->Sn_F501_14E, MONY_BUF_SIZE );
	memmove ( F503_14C, ( *m_pSnFhyo5_01Data )->Sn_F501_14F, MONY_BUF_SIZE );
	F503_14c = ( *m_pSnFhyo5_01Data )->Sn_F501_14sw;	// 手入力サイン

	// 第二種事業(小売業)
	memmove ( F503_15A, ( *m_pSnFhyo5_01Data )->Sn_F501_15D, MONY_BUF_SIZE );
	memmove ( F503_15B, ( *m_pSnFhyo5_01Data )->Sn_F501_15E, MONY_BUF_SIZE );
	memmove ( F503_15C, ( *m_pSnFhyo5_01Data )->Sn_F501_15F, MONY_BUF_SIZE );
	F503_15c = ( *m_pSnFhyo5_01Data )->Sn_F501_15sw;	// 手入力サイン

	// 第三種事業(製造業等)
	memmove ( F503_16A, ( *m_pSnFhyo5_01Data )->Sn_F501_16D, MONY_BUF_SIZE );
	memmove ( F503_16B, ( *m_pSnFhyo5_01Data )->Sn_F501_16E, MONY_BUF_SIZE );
	memmove ( F503_16C, ( *m_pSnFhyo5_01Data )->Sn_F501_16F, MONY_BUF_SIZE );
	F503_16c = ( *m_pSnFhyo5_01Data )->Sn_F501_16sw;	// 手入力サイン

	// 第四種事業(その他)
	memmove ( F503_17A, ( *m_pSnFhyo5_01Data )->Sn_F501_17D, MONY_BUF_SIZE );
	memmove ( F503_17B, ( *m_pSnFhyo5_01Data )->Sn_F501_17E, MONY_BUF_SIZE );
	memmove ( F503_17C, ( *m_pSnFhyo5_01Data )->Sn_F501_17F, MONY_BUF_SIZE );
	F503_17c = ( *m_pSnFhyo5_01Data )->Sn_F501_17sw;	// 手入力サイン

	// 第五種事業(サービス業)
	memmove ( F503_18A, ( *m_pSnFhyo5_01Data )->Sn_F501_18D, MONY_BUF_SIZE );
	memmove ( F503_18B, ( *m_pSnFhyo5_01Data )->Sn_F501_18E, MONY_BUF_SIZE );
	memmove ( F503_18C, ( *m_pSnFhyo5_01Data )->Sn_F501_18F, MONY_BUF_SIZE );
	F503_18c = ( *m_pSnFhyo5_01Data )->Sn_F501_18sw;	// 手入力サイン

	// 第六種事業(不動産業)
	memmove ( F503_19A, ( *m_pSnFhyo5_01Data )->Sn_F501_19D, MONY_BUF_SIZE );
	memmove ( F503_19B, ( *m_pSnFhyo5_01Data )->Sn_F501_19E, MONY_BUF_SIZE );
	memmove ( F503_19C, ( *m_pSnFhyo5_01Data )->Sn_F501_19F, MONY_BUF_SIZE );
	F503_19c = ( *m_pSnFhyo5_01Data )->Sn_F501_19sw;	// 手入力サイン

	// 売上割合
	F503_7UP  = ( *m_pSnFhyo5_01Data )->Sn_F501_UP1;
	F503_8UP  = ( *m_pSnFhyo5_01Data )->Sn_F501_UP2;
	F503_9UP  = ( *m_pSnFhyo5_01Data )->Sn_F501_UP3;
	F503_10UP = ( *m_pSnFhyo5_01Data )->Sn_F501_UP4;
	F503_11UP = ( *m_pSnFhyo5_01Data )->Sn_F501_UP5;
	F503_12UP = ( *m_pSnFhyo5_01Data )->Sn_F501_UP6;

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo5_03::ReadData2()
{
	// 旧税率は全てクリア
	char mony[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "357901" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358001" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "358901" ), mony );

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		if ( F503_6c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357602" ), F503_6A);
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357602" ) );
		}

		if ( F503_6c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357603" ), F503_6B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357603" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357702" ), F503_7A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357703" ), F503_7B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F503_8A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F503_8B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357902" ), F503_9A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357903" ), F503_9B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358002" ), F503_10A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358003" ), F503_10B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358102" ), F503_11A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358103" ), F503_11B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358202" ), F503_12A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358203" ), F503_12B );

		if ( F503_14c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358402" ), F503_14A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358402" ) );
		}
		if ( F503_14c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358403" ), F503_14B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358403" ) );
		}
		if ( F503_15c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358502" ), F503_15A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358502" ) );
		}
		if ( F503_15c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358503" ), F503_15B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358503" ) );
		}
		if ( F503_16c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358602" ), F503_16A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358602" ) );
		}
		if ( F503_16c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358603" ), F503_16B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358603" ) );
		}
		if ( F503_17c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358702" ), F503_17A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358702" ) );
		}
		if ( F503_17c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358703" ), F503_17B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358703" ) );
		}
		if ( F503_18c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358802" ), F503_18A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358802" ) );
		}
		if ( F503_18c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358803" ), F503_18B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358803" ) );
		}
		if ( F503_19c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358902" ), F503_19A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358902" ) );
		}
		if ( F503_19c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358903" ), F503_19B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358903" ) );
		}
	}
}

//=========================================================
// データ書込み
//---------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo5_03::WriteData()
{
	// 課税標準額に対する消費税額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1D, F503_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1E, F503_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1F, F503_1C, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_2X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2D, F503_2A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2E, F503_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2F, F503_2C, MONY_BUF_SIZE );

	// 売上対価の返還等対価に係る消費税額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_3X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3D, F503_3A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3E, F503_3B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3F, F503_3C, MONY_BUF_SIZE );

	// 控除対象仕入税額の計算の基礎となる消費税額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_4X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4D, F503_4A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4E, F503_4B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4F, F503_4C, MONY_BUF_SIZE );

	// 1種類の事業の専業者の場合の控除対象仕入税額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_5X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5D, F503_5A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5E, F503_5B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5F, F503_5C, MONY_BUF_SIZE );

	// 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_6X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6D, F503_6A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6E, F503_6B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6F, F503_6C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_6sw = F503_6c;

	// 第一種事業(卸売業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_7X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7D, F503_7A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7E, F503_7B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7F, F503_7C, MONY_BUF_SIZE );

	// 第二種事業(小売業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_8X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8D, F503_8A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8E, F503_8B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8F, F503_8C, MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_9X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9D, F503_9A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9E, F503_9B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9F, F503_9C, MONY_BUF_SIZE );

	// 第四種事業(その他)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_10X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10D, F503_10A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10E, F503_10B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10F, F503_10C, MONY_BUF_SIZE );

	// 第五種事業(サービス業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_11X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11D, F503_11A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11E, F503_11B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11F, F503_11C, MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_12X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12D, F503_12A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12E, F503_12B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12F, F503_12C, MONY_BUF_SIZE );

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_13X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13D, F503_13A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13E, F503_13B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13F, F503_13C, MONY_BUF_SIZE );

	// 第一種事業(卸売業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_14X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14D, F503_14A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14E, F503_14B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14F, F503_14C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_14sw = F503_14c;
	if (m_pArith->l_test ( F503_14A ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14DP, F503_14A, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_14DP, '\0', MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( F503_14B ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14EP, F503_14B, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_14EP, '\0', MONY_BUF_SIZE );
	}

	// 第二種事業(小売業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_15X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15D, F503_15A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15E, F503_15B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15F, F503_15C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_15sw = F503_15c;
	if ( m_pArith->l_test ( F503_15A ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15DP, F503_15A, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_15DP, '\0', MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( F503_15B ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15EP, F503_15B, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_15EP, '\0', MONY_BUF_SIZE );
	}

	// 第三種事業(製造業等)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_16X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16D, F503_16A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16E, F503_16B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16F, F503_16C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_16sw = F503_16c;
	if ( m_pArith->l_test ( F503_16A ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16DP, F503_16A, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_16DP, '\0', MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( F503_16B ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16EP, F503_16B, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_16EP, '\0', MONY_BUF_SIZE );
	}

	// 第四種事業(その他)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_17X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17D, F503_17A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17E, F503_17B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17F, F503_17C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_17sw = F503_17c;
	if ( m_pArith->l_test ( F503_17A ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17DP, F503_17A, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_17DP, '\0', MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( F503_17B ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17EP, F503_17B, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_17EP, '\0', MONY_BUF_SIZE );
	}

	// 第五種事業(サービス業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_18X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18D, F503_18A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18E, F503_18B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18F, F503_18C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_18sw = F503_18c;
	if ( m_pArith->l_test ( F503_18A ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18DP, F503_18A, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_18DP, '\0', MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( F503_18B ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18EP, F503_18B, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_18EP, '\0', MONY_BUF_SIZE );
	}

	// 第六種事業(不動産業)
	memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_19X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19D, F503_19A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19E, F503_19B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19F, F503_19C, MONY_BUF_SIZE );
	( *m_pSnFhyo5_01Data )->Sn_F501_19sw = F503_19c;
	if ( m_pArith->l_test ( F503_19A ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19DP, F503_19A, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_19DP, '\0', MONY_BUF_SIZE );
	}
	if ( m_pArith->l_test ( F503_19B ) >= 0 ) {
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19EP, F503_19B, MONY_BUF_SIZE );
	}
	else {
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_19EP, '\0', MONY_BUF_SIZE );
	}

	// 売上割合
	( *m_pSnFhyo5_01Data )->Sn_F501_UP1 = F503_7UP;
	( *m_pSnFhyo5_01Data )->Sn_F501_UP2 = F503_8UP;
	( *m_pSnFhyo5_01Data )->Sn_F501_UP3 = F503_9UP;
	( *m_pSnFhyo5_01Data )->Sn_F501_UP4 = F503_10UP;
	( *m_pSnFhyo5_01Data )->Sn_F501_UP5 = F503_11UP;
	( *m_pSnFhyo5_01Data )->Sn_F501_UP6 = F503_12UP;

	( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 0;
	if ( typeCnt == 1 ) {

		if ( m_pArith->l_test ( F503_7A ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 1;
		}
		else if ( m_pArith->l_test ( F503_7B ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 1;
		}

		else if ( m_pArith->l_test ( F503_8A ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 2;
		}
		else if ( m_pArith->l_test ( F503_8B ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 2;
		}

		else if ( m_pArith->l_test ( F503_9A ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 3;
		}
		else if ( m_pArith->l_test ( F503_9B ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 3;
		}

		else if ( m_pArith->l_test ( F503_10A ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 4;
		}
		else if ( m_pArith->l_test ( F503_10B ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 4;
		}

		else if ( m_pArith->l_test ( F503_11A ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 5;
		}
		else if ( m_pArith->l_test ( F503_11B ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 5;
		}

		else if ( m_pArith->l_test ( F503_12A ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 6;
		}
		else if ( m_pArith->l_test ( F503_12B ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 6;
		}

		MoneyBasejagArray money;
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "350702" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 1;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "350703" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) { 
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 1;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "350802" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 2;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "350803" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 2;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "350902" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 3;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "350903" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 3;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "351002" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 4;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "351003" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 4;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "351102" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 5;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "351103" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 5;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "351202" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 6;
		}

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "351203" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			( *m_pSnFhyo5_01Data )->Sn_F501_5kbn = 6;
		}
	}


	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo5_03::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		char buff2[256] = { 0 };
		char wariai[32] = { 0 };

		MoneyBasejagArray money2;
		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35F000" ) );
		m_pArith->l_print ( buff2, money2[0][0].arith, _T( "SSS" ) );
		typeCnt = atoi ( buff2 );

		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35B100" ) );
		::ZeroMemory ( wariai, sizeof ( wariai ) );
		m_pArith->l_print ( wariai, money2[0][0].arith, _T( "sssssssssssss9" ) );
		F503_7UP = atoi ( wariai );

		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35B200" ) );
		::ZeroMemory ( wariai, sizeof ( wariai ) );
		m_pArith->l_print ( wariai, money2[0][0].arith, _T( "sssssssssssss9" ) );
		F503_8UP = atoi ( wariai );

		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35B300" ) );
		::ZeroMemory ( wariai, sizeof ( wariai ) );
		m_pArith->l_print ( wariai, money2[0][0].arith, _T( "sssssssssssss9" ) );
		F503_9UP = atoi ( wariai );

		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35B400" ) );
		::ZeroMemory ( wariai, sizeof ( wariai ) );
		m_pArith->l_print ( wariai, money2[0][0].arith, _T( "sssssssssssss9" ) );
		F503_10UP = atoi ( wariai );

		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35B500" ) );
		::ZeroMemory ( wariai, sizeof ( wariai ) );
		m_pArith->l_print ( wariai, money2[0][0].arith, _T( "sssssssssssss9" ) );
		F503_11UP = atoi ( wariai );

		money2 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35B600" ) );
		::ZeroMemory ( wariai, sizeof ( wariai ) );
		m_pArith->l_print ( wariai, money2[0][0].arith, _T( "sssssssssssss9" ) );
		F503_12UP = atoi ( wariai );

		return 0;
	}

	// 計算項目をすべて再取得
	MoneyBasejagArray money;
	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );

	int cnt = ( int )money.GetCount();
	int incnt = 0;
	char buff[256];

	for ( int l = 0; l < cnt; l++ ) {
		incnt = ( int )money[l].GetCount();
		for ( int m = 0; m < incnt; m++ ) {
			// 業種数
			if ( money[l][m].code == _T( "35F000" ) ) {
				m_pArith->l_print ( buff, money[l][m].arith, _T( "SSS" ) );
				typeCnt = atoi ( buff );
			}
			else if ( money[l][m].code == _T( "357100" ) ) {
				memmove ( F503_1C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357102" ) ) {
				memmove ( F503_1A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357103" ) ) {
				memmove ( F503_1B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357200" ) ) {
				memmove ( F503_2C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357202" ) ) {
				memmove ( F503_2A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357203" ) ) {
				memmove ( F503_2B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357300" ) ) {
				memmove ( F503_3C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357302" ) ) {
				memmove ( F503_3A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357303" ) ) {
				memmove ( F503_3B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357400" ) ) {
				memmove ( F503_4C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357402" ) ) {
				memmove ( F503_4A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357403" ) ) {
				memmove ( F503_4B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357500" ) ) {
				memmove ( F503_5C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357502" ) ) {
				memmove ( F503_5A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357503" ) ) {
				memmove ( F503_5B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357600" ) ) {
				memmove ( F503_6C, money[l][m].arith, MONY_BUF_SIZE );
				memmove ( ( *m_pSnHeadData )->Sn_KURIA, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357602" ) ) {
				memmove ( F503_6A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357603" ) ) {
				memmove ( F503_6B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357700" ) ) {
				memmove ( F503_7C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357702" ) ) {
				memmove ( F503_7A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357703" ) ) {
				memmove ( F503_7B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B100" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F503_7UP = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "357800" ) ) {
				memmove ( F503_8C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357802" ) ) {
				memmove ( F503_8A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357803" ) ) {
				memmove ( F503_8B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B200" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F503_8UP = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "357900" ) ) {
				memmove ( F503_9C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357902" ) ) {
				memmove ( F503_9A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357903" ) ) {
				memmove ( F503_9B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B300" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F503_9UP = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "358000" ) ) {
				memmove ( F503_10C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358002" ) ) {
				memmove ( F503_10A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358003" ) ) {
				memmove ( F503_10B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B400" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F503_10UP = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "358100" ) ) {
				memmove ( F503_11C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358102" ) ) {
				memmove ( F503_11A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358103" ) ) {
				memmove ( F503_11B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B500" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F503_11UP = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "358200" ) ) {
				memmove ( F503_12C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358202" ) ) {
				memmove ( F503_12A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358203" ) ) {
				memmove ( F503_12B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B600" ) ) {
				char wariai[32] = { 0 };
				m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F503_12UP = atoi ( wariai );
			}
			else if ( money[l][m].code == _T( "358300" ) ) {
				memmove ( F503_13C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358302" ) ) {
				memmove ( F503_13A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358303" ) ) {
				memmove ( F503_13B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358400" ) ) {
				memmove ( F503_14C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358402" ) ) {
				memmove ( F503_14A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358403" ) ) {
				memmove ( F503_14B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358500" ) ) {
				memmove ( F503_15C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358502" ) ) {
				memmove ( F503_15A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358503" ) ) {
				memmove ( F503_15B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358600" ) ) {
				memmove ( F503_16C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358602" ) ) {
				memmove ( F503_16A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358603" ) ) {
				memmove ( F503_16B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358700" ) ) {
				memmove ( F503_17C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358702" ) ) {
				memmove ( F503_17A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358703" ) ) {
				memmove ( F503_17B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358800" ) ) {
				memmove ( F503_18C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358802" ) ) {
				memmove ( F503_18A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358803" ) ) {
				memmove ( F503_18B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358900" ) ) {
				memmove ( F503_19C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358902" ) ) {
				memmove ( F503_19A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358903" ) ) {
				memmove ( F503_19B, money[l][m].arith, MONY_BUF_SIZE );
			}
		}
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
int CShinFhyo5_03::SyukeiDataSet ( int cID )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID503_6A:
		if ( m_pArith->l_cmp ( F503_6A, val ) ) {
			F503_6c |= 0x80;
			memmove ( F503_6A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357602" ), F503_6A );
		}
		break;
	case ID503_6B:
		if ( m_pArith->l_cmp ( F503_6B, val ) ) {
			F503_6c |= 0x800;
			memmove ( F503_6B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357603" ), F503_6B );
		}
		break;
	case ID503_7A:
		memmove ( F503_7A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357702" ), F503_7A );
		break;
	case ID503_7B:
		memmove ( F503_7B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357703" ), F503_7B );
		break;
	case ID503_8A:
		memmove ( F503_8A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F503_8A );
		break;
	case ID503_8B:
		memmove ( F503_8B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F503_8B );
		break;
	case ID503_9A:
		memmove ( F503_9A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357902" ), F503_9A );
		break;
	case ID503_9B:
		memmove ( F503_9B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357903" ), F503_9B );
		break;
	case ID503_10A:
		memmove ( F503_10A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358002" ), F503_10A );
		break;
	case ID503_10B:
		memmove ( F503_10B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358003" ), F503_10B );
		break;
	case ID503_11A:
		memmove ( F503_11A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358102" ), F503_11A );
		break;
	case ID503_11B:
		memmove ( F503_11B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358103" ), F503_11B );
		break;
	case ID503_12A:
		memmove ( F503_12A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358202" ), F503_12A );
		break;
	case ID503_12B:
		memmove ( F503_12B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358203" ), F503_12B );
		break;
	case ID503_13A:
		memmove ( F503_13A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358302" ), F503_13A );
		break;
	case ID503_13B:
		memmove ( F503_13B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358303" ), F503_13B );
		break;
	case ID503_14A:
		if ( m_pArith->l_cmp ( F503_14A, val ) ) {
			F503_14c |= 0x80;
			memmove ( F503_14A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358402" ), F503_14A );
		}
		break;
	case ID503_14B:
		if ( m_pArith->l_cmp ( F503_14B, val ) ) {
			F503_14c |= 0x800;
			memmove ( F503_14B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358403" ), F503_14B );
		}
		break;
	case ID503_15A:
		if ( m_pArith->l_cmp ( F503_15A, val ) ) {
			F503_15c |= 0x80;
			memmove ( F503_15A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358502" ), F503_15A );
		}
		break;
	case ID503_15B:
		if ( m_pArith->l_cmp ( F503_15B, val ) ) {
			F503_15c |= 0x800;
			memmove ( F503_15B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358503" ), F503_15B );
		}
		break;
	case ID503_16A:
		if ( m_pArith->l_cmp ( F503_16A, val ) ) {
			F503_16c |= 0x80;
			memmove ( F503_16A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358602" ), F503_16A );
		}
		break;
	case ID503_16B:
		if ( m_pArith->l_cmp ( F503_16B, val ) ) {
			F503_16c |= 0x800;
			memmove ( F503_16B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358603" ), F503_16B );
		}
		break;
	case ID503_17A:
		if ( m_pArith->l_cmp ( F503_17A, val ) ) {
			F503_17c |= 0x80;
			memmove ( F503_17A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358702" ), F503_17A );
		}
		break;
	case ID503_17B:
		if ( m_pArith->l_cmp ( F503_17B, val ) ) {
			F503_17c |= 0x800;
			memmove ( F503_17B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358703" ), F503_17B );
		}
		break;
	case ID503_18A:
		if ( m_pArith->l_cmp ( F503_18A, val ) ) {
			F503_18c |= 0x80;
			memmove ( F503_18A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358802" ), F503_18A );
		}
		break;
	case ID503_18B:
		if ( m_pArith->l_cmp ( F503_18B, val ) ) {
			F503_18c |= 0x800;
			memmove ( F503_18B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358803" ), F503_18B );
		}
		break;
	case ID503_19A:
		if ( m_pArith->l_cmp ( F503_19A, val ) ) {
			F503_19c |= 0x80;
			memmove ( F503_19A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358902" ), F503_19A );
		}
		break;
	case ID503_19B:
		if ( m_pArith->l_cmp ( F503_19B, val ) ) {
			F503_19c |= 0x800;
			memmove ( F503_19B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358903" ), F503_19B );
		}
		break;
	default:
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
int CShinFhyo5_03::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo5_03[cnt].Index == 0 ) {
			break;
		}

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo5_03[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo5_03[cnt].UnConnect;
			}
			else {
				Flg = Fhyo5_03[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo5_03[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo5_03[cnt].Connect );
		cnt++;
	}

	m_Fhyo5_03Diag02.EnableDelete ( 0 );

	return 0;
}

//=========================================================
// ポジションセット
//=========================================================
void CShinFhyo5_03::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Fhyo5_03Diag02.GetPosition() ) != -1 ) {
			m_Fhyo5_03Diag02.SetFocus();	// ダミーでセット
			m_Fhyo5_03Diag02.SetPosition ( index );
			m_Fhyo5_03Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo5_03Diag01.SetFocus();	// ダミーでセット
	m_Fhyo5_03Diag02.SetFocus();

	//　事業区分別の課税売上高A欄へ
	m_Fhyo5_03Diag02.SetPosition ( ID503_6A );
}


//=========================================================
// 画面出力
//=========================================================
void CShinFhyo5_03::DispFh5_03()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[512] = { 0 };

	// 画面の初期化
	for ( int i = 7; i <= 9; i++ ) {				// 課税期間、氏名欄
		dsp_cls ( IDC_ICSDIAG8CTRL1, ( short )i );
	}
	for ( int i = ID503_1A; i <= ID503_19C; i++ ) {	// 金額欄
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	// 手入力サイン初期化
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ){	// 連動の場合は手入力サイン初期化
		F503_6c = F503_14c = F503_15c = F503_16c = F503_17c = F503_18c = F503_19c = 0;
		F503_7c = F503_8c = F503_9c = F503_10c = F503_11c = F503_12c = 0;
	}

	// 期間・氏名の表示
	char yy, mm, dd;
	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, 7, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, 8, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( *m_pSnHeadData )->IsSoleProprietor() ) {
		memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, 40 );
	}
	else {
		memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, 40 );
	}
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL1, 9, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 連動時には「●単一事業を営む～」を非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		diag_clear ( IDC_ICSDIAG8CTRL1, 3, TRUE, CIcsdiagctrl );
		diag_getattr ( IDC_ICSDIAG8CTRL1, 3, &DA, CIcsdiagctrl );
		DA.attr_bcolor = SIROIROCOLOR;
		diag_setattr ( IDC_ICSDIAG8CTRL1, 3, &DA, FALSE, CIcsdiagctrl );
	}

	// データ表示
	f503data_all ( 0 );
}


//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo5_03::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo5_03::dsp_cls ( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
	case 3 :			// 氏名又は､名称
	case 4 :			// 課税期間　自
	case 5 :			// 課税期間　至
	case ID503_7W:		//  7. 売上割合
	case ID503_8W:		//  8. 売上割合
	case ID503_9W:		//  9. 売上割合
	case ID503_10W:		// 10. 売上割合
	case ID503_11W:		// 11. 売上割合	
	case ID503_12W:		// 12. 売上割合	
		diag_clear ( Id, Index, TRUE, CIcsdiagctrl );
		break;
	default :	// 金額
		memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
		diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		break;
	}
}

//=========================================================
// 売上割合表示
//---------------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		pers	:	金額
//=========================================================
void CShinFhyo5_03::dsp_uws ( unsigned short Id, short Index, unsigned short pers )
{
	if ( pers ) {
		char buf[128] = { 0 };
		DIAGRAM_DATA DiagData;
		DiagData.data_imgdata = NULL;

		sprintf_s ( buf, sizeof ( buf ), _T( "%3d.%01d" ), ( pers / 10 ), ( pers % 10 ) );	// 小数１桁
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		dsp_cls ( Id, Index );
	}
}

//=========================================================
// 表示金額取得
//=========================================================
void CShinFhyo5_03::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 課税標準額に対する消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_2A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 売上対価の返還等に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_3A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_3B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_3B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_3C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_3C, DiagData.data_val, MONY_BUF_SIZE );

	// 控除対象仕入税額の計算の基礎となる消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_4A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_4A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_4B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_4C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_4C, DiagData.data_val, MONY_BUF_SIZE );

	// ④×みなし仕入率
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_5A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_5A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_5B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_5C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_5C, DiagData.data_val, MONY_BUF_SIZE );

	// 事業区分別の合計額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_6A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_6A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_6B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_6B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_6C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業(卸売業)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_7A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_7A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_7B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_7B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_7C, DiagData.data_val, MONY_BUF_SIZE );

	// 第二種事業(小売業)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_8A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_8A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_8B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_8B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_8C, DiagData.data_val, MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_9A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_9A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_9B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_9B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_9C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_9C, DiagData.data_val, MONY_BUF_SIZE );


	// 第四種事業(その他)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_10A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_10A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_10B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_10B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_10C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_10C, DiagData.data_val, MONY_BUF_SIZE );

	// 第五種事業(サービス業等)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_11A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_11A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_11B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_11B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_11C, DiagData.data_val, MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_12A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_12A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_12B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_12B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_12C, DiagData.data_val, MONY_BUF_SIZE );

	// 事業区分別の合計額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_13A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_13A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_13B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_13B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_13C, DiagData.data_val, MONY_BUF_SIZE );

	// 第一種事業(卸売業)
	diag_getdata (  IDC_ICSDIAG8CTRL2, ID503_14A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_14A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata (  IDC_ICSDIAG8CTRL2, ID503_14B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_14B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata (  IDC_ICSDIAG8CTRL2, ID503_14C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_14C, DiagData.data_val, MONY_BUF_SIZE );

	// 第二種事業(小売業)
	diag_getdata (  IDC_ICSDIAG8CTRL2, ID503_15A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_15A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata (  IDC_ICSDIAG8CTRL2, ID503_15B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_15B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata (  IDC_ICSDIAG8CTRL2, ID503_15C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_15C, DiagData.data_val, MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_16A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_16A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_16B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_16B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_16C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_16C, DiagData.data_val, MONY_BUF_SIZE );

	// 第四種事業(その他)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_17A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_17A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_17B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_17B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_17C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_17C, DiagData.data_val, MONY_BUF_SIZE );

	// 第五種事業(サービス業等)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_18A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_18A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_18B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_18B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_18C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_18C, DiagData.data_val, MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_19A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_19A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_19B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_19B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID503_19C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F503_19C, DiagData.data_val, MONY_BUF_SIZE );
}


//=========================================================
// 入力項目色変え
//---------------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=========================================================
void CShinFhyo5_03::ChangeColor(unsigned short id, short index, int sign)
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
int CShinFhyo5_03::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY5_03CNT; i++ ) {
		if ( Index == FCS_Fhyo5_03[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo5_03[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY5_03CNT; j++ ) {
			m_Fhyo5_03Diag02.GetAttribute ( FCS_Fhyo5_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_03[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo5_03Diag02.GetAttribute ( FCS_Fhyo5_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_03[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//=========================================================
// 項目表示
//---------------------------------------------------------
// 引数
//		atno		：		項目番号
//=========================================================
void CShinFhyo5_03::f503data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f503data_1();
	case 2:
		f503data_2();
	case 3:
		f503data_3();
	case 4:
		f503data_4();
	case 5:
		f503data_5();
	case 6:
		f503data_6();
	case 7:
		f503data_7();
	case 8:
		f503data_8();
	case 9:
		f503data_9();
	case 10:
		f503data_10();
	case 11:
		f503data_11();
	case 12:
		f503data_12();
	case 13:
		f503data_13();
	case 14:
		f503data_14();
	case 15:
		f503data_15();
	case 16:
		f503data_16();
	case 17:
		f503data_17();
	case 18:
		f503data_18();
	case 19:
		f503data_19();
		break;
	default:
		break;
	}
}

//=========================================================
// 課税標準額に対する消費税額
//=========================================================
void CShinFhyo5_03::f503data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_1A, F503_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_1B, F503_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_1C, F503_1C );
}

//=========================================================
// 貸倒回収に係る消費税額
//=========================================================
void CShinFhyo5_03::f503data_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_2A, F503_2A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_2B, F503_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_2C, F503_2C );
}

//=========================================================
// 売上対価の返還等対価に係る税額
//=========================================================
void CShinFhyo5_03::f503data_3()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_3A, F503_3A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_3B, F503_3B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_3C, F503_3C );
}

//=========================================================
// 控除対象仕入税額の計算の基礎となる消費税額
//=========================================================
void CShinFhyo5_03::f503data_4()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_4A, F503_4A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_4B, F503_4B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_4C, F503_4C );
}

//=========================================================
// 1種類の事業の専業者の場合の控除対象仕入税額
//=========================================================
void CShinFhyo5_03::f503data_5()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_5A, F503_5A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_5B, F503_5B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_5C, F503_5C );
}

//=========================================================
// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
//=========================================================
void CShinFhyo5_03::f503data_6()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_6A, ( F503_6c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_6B, ( F503_6c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_6A, F503_6A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_6B, F503_6B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_6C, F503_6C );
}

//=========================================================
// 第一種事業(卸売業)
//=========================================================
void CShinFhyo5_03::f503data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_7A, F503_7A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_7B, F503_7B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_7C, F503_7C );

	dsp_uws ( IDC_ICSDIAG8CTRL2, ID503_7W, F503_7UP );
}

//=========================================================
// 第二種事業(小売業)
//=========================================================
void CShinFhyo5_03::f503data_8()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_8A, F503_8A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_8B, F503_8B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_8C, F503_8C );

	dsp_uws ( IDC_ICSDIAG8CTRL2, ID503_8W, F503_8UP );
}

//=========================================================
// 第三種事業(製造業等)
//=========================================================
void CShinFhyo5_03::f503data_9()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_9A, F503_9A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_9B, F503_9B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_9C, F503_9C );

	dsp_uws ( IDC_ICSDIAG8CTRL2, ID503_9W, F503_9UP );
}

//=========================================================
// 第四種事業(その他)
//=========================================================
void CShinFhyo5_03::f503data_10()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_10A, F503_10A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_10B, F503_10B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_10C, F503_10C );

	dsp_uws ( IDC_ICSDIAG8CTRL2, ID503_10W, F503_10UP );
}

//=========================================================
// 課税標準額に対する消費税額
//=========================================================
void CShinFhyo5_03::f503data_11()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_11A, F503_11A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_11B, F503_11B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_11C, F503_11C );

	dsp_uws ( IDC_ICSDIAG8CTRL2, ID503_11W, F503_11UP );
}

//=========================================================
// 第六種事業(不動産業)
//=========================================================
void CShinFhyo5_03::f503data_12()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_12A, F503_12A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_12B, F503_12B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_12C, F503_12C );

	dsp_uws ( IDC_ICSDIAG8CTRL2, ID503_12W, F503_12UP );
}

//=========================================================
// 事業区分別の課税売上高に係る消費税額の明細
//=========================================================
void CShinFhyo5_03::f503data_13()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_13A, F503_13A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_13B, F503_13B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_13C, F503_13C );
}

//=========================================================
// 第一種事業(卸売業)
//=========================================================
void CShinFhyo5_03::f503data_14()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		// 手入力サインなし
		if ( !( F503_14c & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_14A, 0 );
			if ( ( F503_7c & 0x10 ) || ( m_pArith->l_test ( F503_7A ) && m_pArith->l_test ( F503_14A ) == 0 ) ) {
				memset ( F503_14A, '\0', MONY_BUF_SIZE );
				// 売上割合の再計算
				F503_7c &= ~0xf0;
			}
		}
		if ( !( F503_14c & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_14B, 0 );
			if ( ( F503_7c & 0x100 ) || ( m_pArith->l_test ( F503_7B ) && m_pArith->l_test ( F503_14B ) == 0 ) ) {
				memset ( F503_14B, '\0', MONY_BUF_SIZE );
				// 売上割合の再計算
				F503_7c &= ~0xf00;
			}
		}

		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_14A, ( F503_14c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_14B, ( F503_14c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_14A, F503_14A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_14B, F503_14B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_14C, F503_14C );
}

//=========================================================
// 第二種事業(小売業)
//=========================================================
void CShinFhyo5_03::f503data_15()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_15A, ( F503_15c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_15B, ( F503_15c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_15A, F503_15A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_15B, F503_15B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_15C, F503_15C );
}

//=========================================================
// 第三種事業(製造業等)
//=========================================================
void CShinFhyo5_03::f503data_16()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_16A, ( F503_16c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_16B, ( F503_16c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_16A, F503_16A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_16B, F503_16B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_16C, F503_16C );
}

//=========================================================
// 第四種事業(その他)
//=========================================================
void CShinFhyo5_03::f503data_17()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_17A, ( F503_17c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_17B, ( F503_17c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_17A, F503_17A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_17B, F503_17B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_17C, F503_17C );
}

//=========================================================
// 第五種事業(サービス業)
//=========================================================
void CShinFhyo5_03::f503data_18()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_18A, ( F503_18c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_18B, ( F503_18c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_18A, F503_18A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_18B, F503_18B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_18C, F503_18C );
}

//=========================================================
// 第六種事業(不動産業)
//=========================================================
void CShinFhyo5_03::f503data_19()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_19A, ( F503_19c & 0x80 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID503_19B, ( F503_19c & 0x800 ) ? 3 : 0 );
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_19A, F503_19A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_19B, F503_19B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID503_19C, F503_19C );
}


//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo5_03::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F503_6c  & 0x880 ) || ( F503_14c & 0x880 ) || ( F503_15c & 0x880 ) || ( F503_16c & 0x880 ) ||
			 ( F503_17c & 0x880 ) || ( F503_18c & 0x880 ) || ( F503_19c & 0x880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

//====================================================================
// 売上割合取得
//--------------------------------------------------------------------
// 引数
//		id			:			項目ID
//		pWari		:			売上割合
//		pArith		:			合計金額
//--------------------------------------------------------------------
// 返送値
//		0			:			正常終了
//		-1
//====================================================================
int CShinFhyo5_03::GetPercentage( int id, unsigned short *pWari, char *pArith )
{
	ASSERT( pWari );
	ASSERT( pArith );
	if( ( pWari == NULL ) || ( pArith == NULL ) ) {
		return -1;
	}

	unsigned short tmpWari = 0;
	char tmpArith[MONY_BUF_SIZE] = { 0 };
	bool notFlg = false;

	*pWari = 0;
	memset( pArith, '\0', MONY_BUF_SIZE );

	switch ( id ) {
	case DAIICHISHU:
		tmpWari = F503_7UP;
		memmove( tmpArith, F503_7C, MONY_BUF_SIZE );
		break;
	case DAINISHU:
		tmpWari = F503_8UP;
		memmove( tmpArith, F503_8C, MONY_BUF_SIZE );
		break;
	case DAISANSHU:
		tmpWari = F503_9UP;
		memmove( tmpArith, F503_9C, MONY_BUF_SIZE );
		break;
	case DAIYONSHU:
		tmpWari = F503_10UP;
		memmove( tmpArith, F503_10C, MONY_BUF_SIZE );
		break;
	case DAIGOSHU:
		tmpWari = F503_11UP;
		memmove( tmpArith, F503_11C, MONY_BUF_SIZE );
		break;
	case DAIROKUSHU:
		tmpWari = F503_12UP;
		memmove( tmpArith, F503_12C, MONY_BUF_SIZE );
		break;
	default:
		notFlg = true;
		break;
	}

	if( !notFlg ) {
		*pWari = tmpWari;
		memmove( pArith, tmpArith, MONY_BUF_SIZE );
	}
	else {
		return -1;
	}

	return 0;
}