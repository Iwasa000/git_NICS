// ShinFhyo1_03.cpp : 実装ファイル
//

//#include "pch.h"
#include "stdafx.h"
#include "ShinFhyo1_03.h"
#include "ShinFhyo1_03idx.h"
//#include "afxdialogex.h"
#include "H31HyoView.h"


// ShinFhyo1_03 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo1_03, CSyzBaseDlg)

CShinFhyo1_03::CShinFhyo1_03(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo1_03::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo1_01Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pShinInfo ( NULL )
	, m_pSyzSyukei ( NULL )
{
	memset ( F103_1A, '\0', sizeof ( F103_1A ) );
	memset ( F103_1B, '\0', sizeof ( F103_1B ) );
	memset ( F103_1C, '\0', sizeof ( F103_1C ) );

	memset ( F103_1_1A, '\0', sizeof ( F103_1_1A ) );
	memset ( F103_1_1B, '\0', sizeof ( F103_1_1B ) );
	memset ( F103_1_1C, '\0', sizeof ( F103_1_1C ) );

	memset ( F103_1_2C, '\0', sizeof ( F103_1_2C ) );

	memset ( F103_2A, '\0', sizeof ( F103_2A ) );
	memset ( F103_2B, '\0', sizeof ( F103_2B ) );
	memset ( F103_2C, '\0', sizeof ( F103_2C ) );

	memset ( F103_3A, '\0', sizeof ( F103_3A ) );
	memset ( F103_3B, '\0', sizeof ( F103_3B ) );
	memset ( F103_3C, '\0', sizeof ( F103_3C ) );

	memset ( F103_4A, '\0', sizeof ( F103_4A ) );
	memset ( F103_4B, '\0', sizeof ( F103_4B ) );
	memset ( F103_4C, '\0', sizeof ( F103_4C ) );

	memset ( F103_5A, '\0', sizeof ( F103_5A ) );
	memset ( F103_5B, '\0', sizeof ( F103_5B ) );
	memset ( F103_5C, '\0', sizeof ( F103_5C ) );

	memset ( F103_5_1A, '\0', sizeof ( F103_5_1A ) );
	memset ( F103_5_1B, '\0', sizeof ( F103_5_1B ) );
	memset ( F103_5_1C, '\0', sizeof ( F103_5_1C ) );

	memset ( F103_5_2C, '\0', sizeof ( F103_5_2C ) );

	memset ( F103_6A, '\0', sizeof ( F103_6A ) );
	memset ( F103_6B, '\0', sizeof ( F103_6B ) );
	memset ( F103_6C, '\0', sizeof ( F103_6C ) );

	memset ( F103_7A, '\0', sizeof ( F103_7A ) );
	memset ( F103_7B, '\0', sizeof ( F103_7B ) );
	memset ( F103_7C, '\0', sizeof ( F103_7C ) );

	memset ( F103_8C, '\0', sizeof ( F103_8C ) );

	memset ( F103_9C, '\0', sizeof ( F103_9C ) );

	memset ( F103_10C, '\0', sizeof ( F103_10C ) );

	memset ( F103_11C, '\0', sizeof ( F103_11C ) );

	memset ( F103_12C, '\0', sizeof ( F103_12C ) );

	memset ( F103_13C, '\0', sizeof ( F103_13C ) );

	F103_1c = F103_2c = 0;

	m_curwnd = -1;
}

CShinFhyo1_03::~CShinFhyo1_03()
{
}

void CShinFhyo1_03::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo1_03Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo1_03Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo1_03, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo1_03, CSyzBaseDlg)
	ON_EVENT(CShinFhyo1_03, IDC_ICSDIAG8CTRL2, 1, CShinFhyo1_03::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo1_03, IDC_ICSDIAG8CTRL2, 2, CShinFhyo1_03::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo1_03, IDC_ICSDIAG8CTRL2, 3, CShinFhyo1_03::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// ShinFhyo1_03 メッセージ ハンドラー


BOOL CShinFhyo1_03::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	// 初期化
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL2 );
	m_BottomIdx = 120;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo1_03::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo1_03::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo1_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BOOL CShinFhyo1_03::PreTranslateMessage(MSG* pMsg)
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
			DIAGRAM_DATA   dd;
			if ( m_curwnd != -1)  {
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

void CShinFhyo1_03::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}


void CShinFhyo1_03::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char CkMy[MONY_BUF_SIZE] =  { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch ( index ) {
	case ID103_1_1A:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( CkMy, F103_1_1A ) ) {
			if ( !( F103_1c & 0x80 ) ) {
				F103_1c |= 0x10;
			}
		}

		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 1 );
		break;
	case ID103_1_1B:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( CkMy, F103_1_1B ) ) {
			if ( !( F103_1c & 0x800 ) ) {
				F103_1c |= 0x100;
			}
		}

		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 1 );
		break;
	case ID103_1_2B:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( CkMy, F103_1_2B ) ) {
			if ( !( F103_1c & 0x800 ) ) {
				F103_1c |= 0x100;
			}
		}

		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 1 );
		break;
	case ID103_2A:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( CkMy, F103_2A ) ) {
			if ( !( F103_2c & 0x80 ) ) {
				F103_2c |= 0x10;
			}
		}
		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 2 );
		break;
	case ID103_2B:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( CkMy, F103_2B ) ) {
			if ( !( F103_2c & 0x800 ) ) {
				F103_2c |= 0x100;
			}
		}

		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 2 );
		break;
	case ID103_5_1A:
	case ID103_5_1B:
		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 5 );
		break;
	case ID103_5_2B:
		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 5 );
		break;
	case ID103_6A:
	case ID103_6B:		
		// 再計算
		SyukeiDataSet ( index );
		f103data_all ( 6 );
		break;
	default:
		break;
	}
}

void CShinFhyo1_03::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
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
		m_Fhyo1_03Diag02.SetPosition ( pos );
	}

	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck (index, 0 );
		m_Fhyo1_03Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) ) {
		m_Fhyo1_03Diag02.SetNextPosition();
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo1_03Diag02.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch( index ){
		case ID103_1_1A:		// 課税資産の譲渡等の対価の額
			F103_1c &= ~0x80;
			F103_1c |= 0x10;
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID103_1_1B:		// 課税資産の譲渡等の対価の額
			F103_1c &= ~0x800;
			F103_1c |= 0x100;
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID103_2A:			// 消費税額
			F103_2c &= ~0x80;
			F103_2c |= 0x10;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			m_Fhyo1_03Diag02.SetPosition ( index );
			f103data_all ( 2 );
			break;
		case ID103_2B:			// 消費税額
			F103_2c &= ~0x800;
			F103_2c |= 0x100;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
			m_Fhyo1_03Diag02.SetPosition ( index );
			f103data_all ( 2 );
			break;
		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		}
	}

	// 移動先または自身のポジション
	int nowpos = m_Fhyo1_03Diag02.GetPosition();

	RECT	rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int	cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo1_01Data	：	付表1-1データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSyzSyukei		：	集計クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo1_03::InitInfo ( CH31SnFhyo1_01Data **pSnFhyo1_01Data, CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CSyzSyukei **pSyzSyukei )
{
	ASSERT ( pSnFhyo1_01Data );
	if ( pSnFhyo1_01Data == NULL ) {
		return -1;
	}
	m_pSnFhyo1_01Data = pSnFhyo1_01Data;

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
int CShinFhyo1_03::Init()
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
		m_Fhyo1_03Diag01.ModifyItem ( 3, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh1_03();

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
int CShinFhyo1_03::ReadData()
{
	// 課税標準額
	memmove ( F103_1A, ( *m_pSnFhyo1_01Data )->Sn_F101_1D, MONY_BUF_SIZE );
	memmove ( F103_1B, ( *m_pSnFhyo1_01Data )->Sn_F101_1E, MONY_BUF_SIZE );
	memmove ( F103_1C, ( *m_pSnFhyo1_01Data )->Sn_F101_1F, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( F103_1_1A, ( *m_pSnFhyo1_01Data )->Sn_F101_1_1D, MONY_BUF_SIZE );
	memmove ( F103_1_1B, ( *m_pSnFhyo1_01Data )->Sn_F101_1_1E, MONY_BUF_SIZE );
	memmove ( F103_1_1C, ( *m_pSnFhyo1_01Data )->Sn_F101_1_1F, MONY_BUF_SIZE );

	// 特定課税仕入に係る支払い対価の額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F103_1_2B, ( *m_pSnFhyo1_01Data )->Sn_F101_1_2E, MONY_BUF_SIZE );
		memmove ( F103_1_2C, ( *m_pSnFhyo1_01Data )->Sn_F101_1_2F, MONY_BUF_SIZE );
	}
	else {
		memset ( F103_1_2B, '\0', MONY_BUF_SIZE );
		memset ( F103_1_2C, '\0', MONY_BUF_SIZE );
	}

	// 消費税額
	memmove ( F103_2A, ( *m_pSnFhyo1_01Data )->Sn_F101_2D, MONY_BUF_SIZE );
	memmove ( F103_2B, ( *m_pSnFhyo1_01Data )->Sn_F101_2E, MONY_BUF_SIZE );
	memmove ( F103_2C, ( *m_pSnFhyo1_01Data )->Sn_F101_2F, MONY_BUF_SIZE );
	F103_2c = ( *m_pSnFhyo1_01Data )->Sn_F101_2c;

	// 控除過大調整税額
	memmove ( F103_3A, ( *m_pSnFhyo1_01Data )->Sn_F101_3D, MONY_BUF_SIZE );
	memmove ( F103_3B, ( *m_pSnFhyo1_01Data )->Sn_F101_3E, MONY_BUF_SIZE );
	memmove ( F103_3C, ( *m_pSnFhyo1_01Data )->Sn_F101_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( F103_4A, ( *m_pSnFhyo1_01Data )->Sn_F101_4D, MONY_BUF_SIZE );
	memmove ( F103_4B, ( *m_pSnFhyo1_01Data )->Sn_F101_4E, MONY_BUF_SIZE );
	memmove ( F103_4C, ( *m_pSnFhyo1_01Data )->Sn_F101_4F, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( F103_5A, ( *m_pSnFhyo1_01Data )->Sn_F101_5D, MONY_BUF_SIZE );
	memmove ( F103_5B, ( *m_pSnFhyo1_01Data )->Sn_F101_5E, MONY_BUF_SIZE );
	memmove ( F103_5C, ( *m_pSnFhyo1_01Data )->Sn_F101_5F, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	memmove ( F103_5_1A, ( *m_pSnFhyo1_01Data )->Sn_F101_5_1D, MONY_BUF_SIZE );
	memmove ( F103_5_1B, ( *m_pSnFhyo1_01Data )->Sn_F101_5_1E, MONY_BUF_SIZE );
	memmove ( F103_5_1C, ( *m_pSnFhyo1_01Data )->Sn_F101_5_1F, MONY_BUF_SIZE );

	// 特定課税仕入れの返還等対価に係る税額
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		memmove ( F103_5_2B, ( *m_pSnFhyo1_01Data )->Sn_F101_5_2E, MONY_BUF_SIZE );
		memmove ( F103_5_2C, ( *m_pSnFhyo1_01Data )->Sn_F101_5_2F, MONY_BUF_SIZE );
	}
	else {
		memset ( F103_5_2B, '\0', MONY_BUF_SIZE );
		memset ( F103_5_2C, '\0', MONY_BUF_SIZE );
	}

	// 貸倒れに係る税額
	memmove ( F103_6A, ( *m_pSnFhyo1_01Data )->Sn_F101_6D, MONY_BUF_SIZE );
	memmove ( F103_6B, ( *m_pSnFhyo1_01Data )->Sn_F101_6E, MONY_BUF_SIZE );
	memmove ( F103_6C, ( *m_pSnFhyo1_01Data )->Sn_F101_6F, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( F103_7A, ( *m_pSnFhyo1_01Data )->Sn_F101_7D, MONY_BUF_SIZE );
	memmove ( F103_7B, ( *m_pSnFhyo1_01Data )->Sn_F101_7E, MONY_BUF_SIZE );
	memmove ( F103_7C, ( *m_pSnFhyo1_01Data )->Sn_F101_7F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( F103_8C, ( *m_pSnFhyo1_01Data )->Sn_F101_8F, MONY_BUF_SIZE );

	// 差引税額
	memmove ( F103_9C, ( *m_pSnFhyo1_01Data )->Sn_F101_9F, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 控除不足還付税額
	memmove ( F103_10C, ( *m_pSnFhyo1_01Data )->Sn_F101_11F, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額
	memmove ( F103_11C, ( *m_pSnFhyo1_01Data )->Sn_F101_12F, MONY_BUF_SIZE );

	// 還付額
	memmove ( F103_12C, ( *m_pSnFhyo1_01Data )->Sn_F101_14F, MONY_BUF_SIZE );

	// 納税額
	memmove ( F103_13C, ( *m_pSnFhyo1_01Data )->Sn_F101_15F, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo1_03::ReadData2()
{
	// 旧税率は全てクリア
	char mony[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312901" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313001" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "313901" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "314001" ), mony );

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312102" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312103" ) );

		if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x30 ) == 0x00 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F103_1_1A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F103_1_1B );
			//( *m_pSyzSyukei )->SetShinkokuData ( _T( "312303" ), F103_1_2B );
		}
		//else {
		//	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() == FALSE ) {
		//		(*m_pSyzSyukei)->ResetShinkokuData (_T( "312303" ) );
		//	}
		//	else {
		//		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312303" ), F103_1_2B );
		//	}
		//}
		//----> 20231025
		else{
			if( ((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() ){
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F103_1_1A );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F103_1_1B );				
			}
		}
		//<---- 20231025

		if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() == FALSE ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312303" ) );
		}
		else {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312303" ), F103_1_2B );
		}

		if ( F103_2c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312402" ), F103_2A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
		}
		if ( F103_2c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312403" ), F103_2B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
		}

		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312702" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312703" ) );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312802" ), F103_5_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312803" ), F103_5_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312903" ), F103_5_2B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313002" ), F103_6A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313003" ), F103_6B );
	}
	else {
		//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) ) {
		if( IsUseTokurei() ){
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312102" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312103" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
		}
	}
}

//=========================================================
// データ書込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo1_03::WriteData()
{
	// 課税標準額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1D, F103_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1E, F103_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1F, F103_1C,  MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_1D, F103_1_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_1E, F103_1_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_1F, F103_1_1C, MONY_BUF_SIZE );

	// 特定課税仕入に係る支払い対価の額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_2X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_2E, F103_1_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_1_2F, F103_1_2C, MONY_BUF_SIZE );

	// 消費税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_2X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_2D, F103_2A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_2E, F103_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_2F, F103_2C, MONY_BUF_SIZE );
	( *m_pSnFhyo1_01Data )->Sn_F101_2c = F103_2c;

	// 控除過大調整税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_3X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_3D, F103_3A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_3E, F103_3B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_3F, F103_3C, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_4X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_4D, F103_4A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_4E, F103_4B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_4F, F103_4C, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_5X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5D, F103_5A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5E, F103_5B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5F, F103_5C, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_1D, F103_5_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_1E, F103_5_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_1F, F103_5_1C, MONY_BUF_SIZE );

	// 特定課税仕入れの返還等対価に係る税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_2X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_2E, F103_5_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_5_2F, F103_5_2C, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_6X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_6D, F103_6A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_6E, F103_6B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_6F, F103_6C, MONY_BUF_SIZE );

	// 控除税額小計
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_7X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_7D, F103_7A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_7E, F103_7B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_7F, F103_7C, MONY_BUF_SIZE );

	// 控除不足還付税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_8X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_8D, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_8E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_8F, F103_8C, MONY_BUF_SIZE );

	// 差引税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_9X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_9D, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_9E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_9F, F103_9C, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_10F, '\0', MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 控除不足還付税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_11X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_11E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_11F, F103_10C, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_12X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_12E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_12F, F103_11C, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_13X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_13E, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_13F, '\0', MONY_BUF_SIZE );

	// 還付額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_14X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_14E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_14F, F103_12C, MONY_BUF_SIZE );

	// 納税額
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_15X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_15E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo1_01Data )->Sn_F101_15F, F103_13C, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_16E, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo1_01Data )->Sn_F101_16F, '\0', MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo1_03::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目を全て再取得
	MoneyBasejagArray	money;

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "31" ) );
	int cnt = ( int )money.GetCount();
	int	incnt = 0;
	for ( int l = 0; l < cnt; l++ ) {
		incnt = ( int )money[l].GetCount();
		for  ( int m = 0; m < incnt; m++ ) {
			// 課税標準額
			if ( money[l][m].code == _T( "312102" ) ) {
				memmove ( F103_1A, money[l][m].arith, MONY_BUF_SIZE) ;
			}
			else if ( money[l][m].code == _T( "312103" ) ) {
				memmove ( F103_1B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312100" ) ) {
				memmove ( F103_1C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 課税資産の譲渡等の対価の額
			else if ( money[l][m].code == _T( "312202" ) ) {
				memmove ( F103_1_1A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312203" ) ) {
				memmove ( F103_1_1B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312200" ) ) {
				memmove ( F103_1_1C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 特定仕入に係る支払対価の額
			else if ( money[l][m].code == _T( "312303" ) ) {
				memmove ( F103_1_2B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312300" ) ) {
				memmove ( F103_1_2C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 消費税額
			else if ( money[l][m].code == _T( "312402" ) ) {
				memmove ( F103_2A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312403" ) ) {
				memmove ( F103_2B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312400" ) ) {
				memmove ( F103_2C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 控除過大調整税額
			else if ( money[l][m].code == _T( "312502" ) ) {
				memmove ( F103_3A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312503" ) ) {
				memmove ( F103_3B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312500" ) ) {
				memmove ( F103_3C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 控除対象仕入税額
			else if ( money[l][m].code == _T( "312602" ) ) {
				memmove ( F103_4A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312603" ) ) {
				memmove ( F103_4B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312600" ) ) {
				memmove ( F103_4C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 返還等対価に係る税額
			else if ( money[l][m].code == _T( "312702" ) ) {
				memmove ( F103_5A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312703" ) ) {
				memmove ( F103_5B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312700" ) ) {
				memmove ( F103_5C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 売上の返還等に係る税額
			else if ( money[l][m].code == _T( "312802" ) ) {
				memmove ( F103_5_1A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312803" ) ) {
				memmove ( F103_5_1B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "312800" ) ) {
				memmove ( F103_5_1C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 特定課税仕入の返還等対価に係る税額
			else if ( money[l][m].code == _T( "312903" ) ) {
				memmove(F103_5_2B, money[l][m].arith, MONY_BUF_SIZE);
			}
			else if ( money[l][m].code == _T( "312900" ) ) {
				memmove(F103_5_2C, money[l][m].arith, MONY_BUF_SIZE);
			}

			// 貸倒に係る税額
			else if ( money[l][m].code == _T( "313002" ) ) {
				memmove ( F103_6A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "313003" ) ) {
				memmove ( F103_6B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "313000" ) ) {
				memmove ( F103_6C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 控除税額小計
			else if ( money[l][m].code == _T( "313102" ) ) {
				memmove ( F103_7A, money[l][m].arith, MONY_BUF_SIZE );
				}
			else if ( money[l][m].code == _T( "313103" ) ) {
				memmove ( F103_7B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "313100" )) {
				memmove ( F103_7C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 控除不足還付税額
			else if ( money[l][m].code == _T( "313200" ) ) {
				memmove ( F103_8C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 差引税額
			else if ( money[l][m].code == _T( "313300" ) ) {
				memmove ( F103_9C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 地方消費税の課税標準となる消費税額 控除不足還付税額
			else if ( money[l][m].code == _T( "313500" ) ) {
				memmove(F103_10C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 地方消費税の課税標準となる消費税額 差引税額
			else if ( money[l][m].code == _T( "313600" ) ) {
				memmove ( F103_11C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 還付額
			else if ( money[l][m].code == _T( "313800" ) ) {
				memmove ( F103_12C, money[l][m].arith, MONY_BUF_SIZE );
			}

			// 納税額
			else if ( money[l][m].code == _T( "313900" ) ) {
				memmove ( F103_13C, money[l][m].arith, MONY_BUF_SIZE );
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
int CShinFhyo1_03::SyukeiDataSet ( int cID )
{
	char val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID103_1_1A:
		memmove ( F103_1_1A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F103_1_1A );
		break;
	case ID103_1_1B:
		memmove ( F103_1_1B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F103_1_1B );
		break;
	case ID103_1_2B:
		memmove ( F103_1_2B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312303" ), F103_1_2B );
		break;
	case ID103_2A:
		if ( m_pArith->l_cmp ( F103_2A, val ) ) {
			F103_2c |= 0x80;
			memmove ( F103_2A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312402" ), F103_2A );
		}
		break;
	case ID103_2B:
		if ( m_pArith->l_cmp ( F103_2B, val ) ) {
			F103_2c |= 0x800;
			memmove ( F103_2B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312403" ), F103_2B );
		}
		break;
	case ID103_5_1A:
		memmove ( F103_5_1A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312802" ), F103_5_1A );
		break;
	case ID103_5_1B:
		memmove ( F103_5_1B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312803" ), F103_5_1B );
		break;
	case ID103_5_2B:
		memmove ( F103_5_2B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312903" ), F103_5_2B );
		break;
	case ID103_6A:
		memmove ( F103_6A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313002" ), F103_6A );
		break;
	case ID103_6B:
		memmove ( F103_6B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313003" ), F103_6B );
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
int CShinFhyo1_03::InitDiagAttr()
{
	int cnt = 0;
	int Flg = 0;

	while ( 1 ){
		Flg = 0;

		if ( Fhyo1_03[cnt].Index == 0 ) {
			break;
		}

		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo1_03[cnt].AllUnConnect;
		}
		else {
			if( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo1_03[cnt].UnConnect;
				//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) 
				if( ( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() )	// 20231025 令和5年9月30日をまたぐ場合は入力可
					&& ( ( Fhyo1_03[cnt].Index == ID103_1_1A ) || ( Fhyo1_03[cnt].Index == ID103_1_1B ) ) ) {
					Flg = 0;
				}
				else {
					int st = ChkTkkzIdx ( Fhyo1_03[cnt].Index );
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
				Flg = Fhyo1_03[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo1_03[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo1_03[cnt].Connect );
		cnt++;
	}

	//DELETEキー１回で削除モード設定
	m_Fhyo1_03Diag02.EnableDelete ( 0 );

	return 0;
}

//=========================================================
// 特定課税仕入項目?
//---------------------------------------------------------
// 引数
//			idx			：	入力項目
//---------------------------------------------------------
// 返送値
//			0			：	特定課税仕入に関連しない
//			1			：	特手課税仕入で入力可能項目
//=========================================================
int CShinFhyo1_03::ChkTkkzIdx ( short idx )
{
	int ret = 0;

	if ( ( idx == ID103_1_2B ) || ( idx == ID103_5_2B ) ) {
		ret = 1;
	}

	return ret;
}

//=========================================================
// 特定課税仕入クリア
//=========================================================
void CShinFhyo1_03::ClearTkkzMony()
{
	m_pArith->l_clear ( F103_1_2B );
	m_pArith->l_clear ( F103_1_2C );
	m_pArith->l_clear ( F103_5_2B );
	m_pArith->l_clear ( F103_5_2C );

	ReadData2();
	MemoryReget();
	WriteData();
}


//=========================================================
// 画面出力
//=========================================================
void CShinFhyo1_03::DispFh1_03()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 3; i <= 5; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	for( int i = ID103_1A; i <= ID103_13C; i++ ){
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	//連動の場合手入力サイン初期化
	if( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		F103_2c = 0;
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
	diag_setdata ( IDC_ICSDIAG8CTRL2, 3, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	//---->20230120
	long smd = 0, emd = 0;
	( ( CH31HyoView* )m_pParent )->GetKazeiKikan( smd, emd );
	if( emd >= ICS_SH_INVOICE_DAY ){
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-3の27・28Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-3の27・28Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 41, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-3の26Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-3の26Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	//<----20230120

	// 各項目のデータ全て表示
	f103data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo1_03::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo1_03::dsp_cls ( unsigned short Id, short Index )
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
// 表示金額取得
//=========================================================
void CShinFhyo1_03::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 課税標準額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 特定課税仕入に係る支払い対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_1_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_1_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_2A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 控除過大調整税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_3A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_3B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_3B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_3C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_3C, DiagData.data_val, MONY_BUF_SIZE );

	// 控除対象仕入税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_4A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_4A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_4B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_4C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_4C, DiagData.data_val, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5C, DiagData.data_val, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 特定課税仕入れの返還等対価に係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_5_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_5_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_6A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_6A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_6B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_6B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_6C, DiagData.data_val, MONY_BUF_SIZE );

	// 控除税額小計
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_7A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_7A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_7B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_7B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_7C, DiagData.data_val, MONY_BUF_SIZE );

	// 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_8C, DiagData.data_val, MONY_BUF_SIZE );

	// 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_9C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_9C, DiagData.data_val, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_10C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_10C, DiagData.data_val, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_11C, DiagData.data_val, MONY_BUF_SIZE );

	// 還付額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_12C, DiagData.data_val, MONY_BUF_SIZE );

	// 納税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID103_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F103_13C, DiagData.data_val, MONY_BUF_SIZE );
}

//=========================================================
// ポジションセット
//=========================================================
void CShinFhyo1_03::SetItemPosition()
{	
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Fhyo1_03Diag02.GetPosition() ) != -1 ) {
			m_Fhyo1_03Diag01.SetFocus();
			m_Fhyo1_03Diag02.SetPosition ( index );
			m_Fhyo1_03Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo1_03Diag01.SetFocus();
	m_Fhyo1_03Diag02.SetFocus();

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) {
			if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) && ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) ) {
				// 特定課税仕入へ
				diag_setposition ( IDC_ICSDIAG8CTRL2, ID103_1_2B, CIcsdiagctrl );
			}
			else {
				// 消費税額へ
				diag_setposition ( IDC_ICSDIAG8CTRL2, ID103_2A, CIcsdiagctrl );
			}
		}
		else {
			//課税標準額へ
			diag_setposition ( IDC_ICSDIAG8CTRL2, ID103_1_1A, CIcsdiagctrl );
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
int CShinFhyo1_03::UpDownFocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY1_03CNT; i++ ) {
		if ( Index == FCS_Fhyo1_03[i].IDNo ) {
			break;
		}
	}

	// 現在ポジション
	idx = FCS_Fhyo1_03[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) { //下方向
		for ( j = i + 1; j < FCS_FHY1_03CNT; j++ ) {
			m_Fhyo1_03Diag02.GetAttribute ( FCS_Fhyo1_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo1_03[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) { //上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo1_03Diag02.GetAttribute ( FCS_Fhyo1_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo1_03[j].IDNo;
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
void CShinFhyo1_03::f103data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f103data_1();
		f103data_1_1();
		f103data_1_2();

	case 2:
		f103data_2();

	case 3:
		f103data_3();

	case 4:
		f103data_4();

	case 5:
		f103data_5();
		f103data_5_1();
		f103data_5_2();

	case 6:	
		f103data_6();

	case 7:
		f103data_7();

	case 8:
		f103data_8();

	case 9:
		f103data_9();

	case 10:
		f103data_10();

	case 11:
		f103data_11();

	case 12:
		f103data_12();

	case 13:
		f103data_13();
		break;
	default	:
		break;
	}
}

//=========================================================
// 課税標準額表示
//=========================================================
void CShinFhyo1_03::f103data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1A, F103_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1B, F103_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1C, F103_1C );
}

//=========================================================
// 課税資産の譲渡等の対価の額表示
//=========================================================
void CShinFhyo1_03::f103data_1_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1_1A, F103_1_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1_1B, F103_1_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1_1C, F103_1_1C );
}

//=========================================================
// 特定課税仕入に係る支払い対価の額表示
//=========================================================
void CShinFhyo1_03::f103data_1_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1_2B, F103_1_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_1_2C, F103_1_2C );
}

//=========================================================
// 消費税額表示
//=========================================================
void CShinFhyo1_03::f103data_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_2A, F103_2A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_2B, F103_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_2C, F103_2C );

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID103_2A, ( F103_2c & 0x80 ) ? 3 : 0 );	
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID103_2B, ( F103_2c & 0x800 ) ? 3 : 0 );	
	}
}

//=========================================================
// 控除過大調整税額表示
//=========================================================
void CShinFhyo1_03::f103data_3()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_3A, F103_3A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_3B, F103_3B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_3C, F103_3C );
}

//=========================================================
//  控除対象仕入税額表示
//=========================================================
void CShinFhyo1_03::f103data_4()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_4A, F103_4A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_4B, F103_4B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_4C, F103_4C );
}

//=========================================================
// 返還等対価に係る税額表示
//=========================================================
void CShinFhyo1_03::f103data_5()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5A, F103_5A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5B, F103_5B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5C, F103_5C );
}

//=========================================================
// 売上げの返還等対価に係る税額
//=========================================================
void CShinFhyo1_03::f103data_5_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5_1A, F103_5_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5_1B, F103_5_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5_1C, F103_5_1C );
}

//=========================================================
// 特定課税仕入れの返還等対価に係る税額表示
//=========================================================
void CShinFhyo1_03::f103data_5_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5_2B, F103_5_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_5_2C, F103_5_2C );
}

//=========================================================
// 貸倒れに係る税額表示
//=========================================================
void CShinFhyo1_03::f103data_6()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_6A, F103_6A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_6B, F103_6B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_6C, F103_6C );
}

//=========================================================
// 控除税額小計表示
//=========================================================
void CShinFhyo1_03::f103data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_7A, F103_7A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_7B, F103_7B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_7C, F103_7C );
}

//=========================================================
// 控除不足還付税額表示
//=========================================================
void CShinFhyo1_03::f103data_8()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_8C, F103_8C );
}

//=========================================================
// 差引税額表示
//=========================================================
void CShinFhyo1_03::f103data_9()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_9C, F103_9C );
}

//=========================================================
// 地方消費税の課税標準となる消費税額 控除不足還付税額 表示
//=========================================================
void CShinFhyo1_03::f103data_10()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_10C, F103_10C );
}

//=========================================================
// 地方消費税の課税標準となる消費税額 差引税額 表示
//=========================================================
void CShinFhyo1_03::f103data_11()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_11C, F103_11C );
}

//=========================================================
// 還付額表示
//=========================================================
void CShinFhyo1_03::f103data_12()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_12C, F103_12C );
}

//=========================================================
// 納税額表示
//=========================================================
void CShinFhyo1_03::f103data_13()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID103_13C, F103_13C );
}

//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinFhyo1_03::ChangeColor ( unsigned short id, short Index, int sign )
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
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo1_03::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F103_2c & 0x880 ) {
			ret = TRUE;
		}
	}

	return ret;
}

//=========================================================
// 特例帳票を使用しているか  20231025
//---------------------------------------------------------
// 返送値
//	true		:		使用している
//	false		:		使用していない
//=========================================================
bool CShinFhyo1_03::IsUseTokurei()
{
	bool rt = false;

	if( (*m_pSnHeadData)->m_DispTabSgn&0x80 && (*m_pSnHeadData)->m_DispTabSgn&0x30 ){
		// 特例5-1使用
		rt = true;
	}

	return rt;
}