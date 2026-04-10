// ShinFhyo4_03.cpp : 実装ファイル
//

//#include "pch.h"
#include "stdafx.h"
#include "ShinFhyo4_03.h"
#include "ShinFhyo4_03idx.h"
//#include "afxdialogex.h"
#include "H31HyoView.h"

// ShinFhyo4_03 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo4_03, CSyzBaseDlg)

CShinFhyo4_03::CShinFhyo4_03(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo4_03::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo4_01Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pShinInfo ( NULL )
	, m_pSyzSyukei ( NULL )
{
	memset ( F403_1A, '\0', sizeof ( F403_1A ) );
	memset ( F403_1B, '\0', sizeof ( F403_1B ) );
	memset ( F403_1C, '\0', sizeof ( F403_1C ) );

	memset ( F403_1_1A, '\0', sizeof ( F403_1_1A ) );
	memset ( F403_1_1B, '\0', sizeof ( F403_1_1B ) );
	memset ( F403_1_1C, '\0', sizeof ( F403_1_1C ) );

	memset ( F403_2A, '\0', sizeof ( F403_2A ) );
	memset ( F403_2B, '\0', sizeof ( F403_2B ) );
	memset ( F403_2C, '\0', sizeof ( F403_2C ) );

	memset ( F403_3A, '\0', sizeof ( F403_3A ) );
	memset ( F403_3B, '\0', sizeof ( F403_3B ) );
	memset ( F403_3C, '\0', sizeof ( F403_3C ) );

	memset ( F403_4A, '\0', sizeof ( F403_4A ) );
	memset ( F403_4B, '\0', sizeof ( F403_4B ) );
	memset ( F403_4C, '\0', sizeof ( F403_4C ) );

	memset ( F403_5A, '\0', sizeof ( F403_5A ) );
	memset ( F403_5B, '\0', sizeof ( F403_5B ) );
	memset ( F403_5C, '\0', sizeof ( F403_5C ) );

	memset ( F403_6A, '\0', sizeof ( F403_6A ) );
	memset ( F403_6B, '\0', sizeof ( F403_6B ) );
	memset ( F403_6C, '\0', sizeof ( F403_6C ) );

	memset ( F403_7A, '\0', sizeof ( F403_7A ) );
	memset ( F403_7B, '\0', sizeof ( F403_7B ) );
	memset ( F403_7C, '\0', sizeof ( F403_7C ) );

	memset ( F403_8C, '\0', sizeof ( F403_8C ) );

	memset ( F403_9C, '\0', sizeof ( F403_9C ) );

	memset ( F403_10C, '\0', sizeof ( F403_10C ) );

	memset ( F403_11C, '\0', sizeof ( F403_11C ) );

	memset ( F403_12C, '\0', sizeof ( F403_12C ) );

	memset ( F403_13C, '\0', sizeof ( F403_13C ) );

	F403_1c = 0;
	F403_2c = 0;

	m_curwnd = -1;
}

CShinFhyo4_03::~CShinFhyo4_03()
{
}

void CShinFhyo4_03::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo4_03Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo4_03Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo4_03, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo4_03, CSyzBaseDlg)
	ON_EVENT(CShinFhyo4_03, IDC_ICSDIAG8CTRL2, 1, CShinFhyo4_03::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo4_03, IDC_ICSDIAG8CTRL2, 2, CShinFhyo4_03::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo4_03, IDC_ICSDIAG8CTRL2, 3, CShinFhyo4_03::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// ShinFhyo4_03 メッセージ ハンドラー


BOOL CShinFhyo4_03::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL2 );
	m_BottomIdx = 97;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo4_03::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo4_03::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo4_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BOOL CShinFhyo4_03::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 付表4-1更新
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

void CShinFhyo4_03::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo4_03::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char CkMy[MONY_BUF_SIZE] = {0};

	switch ( index ) {
	case ID403_1_1A:
	case ID403_1_1B:
		SyukeiDataSet ( index );
		f403data_all ( 1 );
		break;
	case ID403_2A:
	case ID403_2B:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( index == ID403_2A ) {
			if ( m_pArith->l_cmp ( F403_2A, CkMy ) ) {
				F403_2c |= 0x80;
				F403_2c &= ~0x10;
			}
		}
		else if ( index == ID403_2B ) {
			if ( m_pArith->l_cmp ( F403_2B, CkMy ) ) {
				F403_2c |= 0x800;
				F403_2c &= ~0x100;
			}
		}
		SyukeiDataSet ( index );
		f403data_all ( 2 );
		break;
	case ID403_3A:
	case ID403_3B:
		SyukeiDataSet ( index );
		f403data_all ( 3 );
		break;
	case ID403_4A:
	case ID403_4B:
		SyukeiDataSet ( index );
		f403data_all ( 4 );
		break;
	case ID403_5A:
	case ID403_5B:
		SyukeiDataSet ( index );
		f403data_all ( 5 );
		break;
	case ID403_6A:
	case ID403_6B:
		SyukeiDataSet ( index );
		f403data_all ( 6 );
		break;
	default:
		break;
	}
}

void CShinFhyo4_03::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをEnterに変換
	char VK_FLG = 0x00;
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	short wTerm = 0;
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
		m_Fhyo4_03Diag02.SetPosition ( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index, 0 );
		m_Fhyo4_03Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) ) {
		m_Fhyo4_03Diag02.SetNextPosition();
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo4_03Diag02.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
		case ID403_2A:
			F403_2c &= ~0x80;

			( *m_pSyzSyukei )->ResetShinkokuData ( _T("342002") );
			f403data_all ( 0 );
			m_Fhyo4_03Diag02.SetPosition ( ID403_2A );
			break;

		case ID403_2B:
			F403_2c &= ~0x800;

			( *m_pSyzSyukei)->ResetShinkokuData( _T("342003") );
			f403data_all ( 0 );
			m_Fhyo4_03Diag02.SetPosition ( ID403_2B );
			break;

		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		}
	}

	int nowpos = m_Fhyo4_03Diag02.GetPosition();
	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );

	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectB.top ) / 2 ) );
	}
}

//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo4_01Data	：	付表4-1データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSyzSyukei		：	集計クラス
//		pArith			：	倍長演算クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo4_03::InitInfo ( CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith )
{
	ASSERT ( pSnFhyo4_01Data );
	if ( pSnFhyo4_01Data == NULL ) {
		return -1;
	}
	m_pSnFhyo4_01Data = pSnFhyo4_01Data;

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
int CShinFhyo4_03::Init()
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
		m_Fhyo4_03Diag01.ModifyItem ( 3, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh4_03();

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
int CShinFhyo4_03::ReadData()
{
	// 課税標準額
	memmove ( F403_1A, ( *m_pSnFhyo4_01Data )->Sn_F401_1D, MONY_BUF_SIZE );
	memmove ( F403_1B, ( *m_pSnFhyo4_01Data )->Sn_F401_1E, MONY_BUF_SIZE );
	memmove ( F403_1C, ( *m_pSnFhyo4_01Data )->Sn_F401_1F, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( F403_1_1A, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1D, MONY_BUF_SIZE );
	memmove ( F403_1_1B, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1E, MONY_BUF_SIZE );
	memmove ( F403_1_1C, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1F, MONY_BUF_SIZE );

	// 消費税額
	memmove ( F403_2A, ( *m_pSnFhyo4_01Data )->Sn_F401_2D, MONY_BUF_SIZE );
	memmove ( F403_2B, ( *m_pSnFhyo4_01Data )->Sn_F401_2E, MONY_BUF_SIZE );
	memmove ( F403_2C, ( *m_pSnFhyo4_01Data )->Sn_F401_2F, MONY_BUF_SIZE );
	F403_2c = ( *m_pSnFhyo4_01Data )->Sn_F401_2sw;	// 手入力サイン 

	// 貸倒回収に係る消費税額
	memmove ( F403_3A, ( *m_pSnFhyo4_01Data )->Sn_F401_3D, MONY_BUF_SIZE );
	memmove ( F403_3B, ( *m_pSnFhyo4_01Data )->Sn_F401_3E, MONY_BUF_SIZE );
	memmove ( F403_3C, ( *m_pSnFhyo4_01Data )->Sn_F401_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( F403_4A, ( *m_pSnFhyo4_01Data )->Sn_F401_4D, MONY_BUF_SIZE );
	memmove ( F403_4B, ( *m_pSnFhyo4_01Data )->Sn_F401_4E, MONY_BUF_SIZE );
	memmove ( F403_4C, ( *m_pSnFhyo4_01Data )->Sn_F401_4F, MONY_BUF_SIZE );

	// 返還等対価に係る消費税額
	memmove ( F403_5A, ( *m_pSnFhyo4_01Data )->Sn_F401_5D, MONY_BUF_SIZE );
	memmove ( F403_5B, ( *m_pSnFhyo4_01Data )->Sn_F401_5E, MONY_BUF_SIZE );
	memmove ( F403_5C, ( *m_pSnFhyo4_01Data )->Sn_F401_5F, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( F403_6A, ( *m_pSnFhyo4_01Data )->Sn_F401_6D, MONY_BUF_SIZE );
	memmove ( F403_6B, ( *m_pSnFhyo4_01Data )->Sn_F401_6E, MONY_BUF_SIZE );
	memmove ( F403_6C, ( *m_pSnFhyo4_01Data )->Sn_F401_6F, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( F403_7A, ( *m_pSnFhyo4_01Data )->Sn_F401_7D, MONY_BUF_SIZE );
	memmove ( F403_7B, ( *m_pSnFhyo4_01Data )->Sn_F401_7E, MONY_BUF_SIZE );
	memmove ( F403_7C, ( *m_pSnFhyo4_01Data )->Sn_F401_7F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( F403_8C, ( *m_pSnFhyo4_01Data )->Sn_F401_8F, MONY_BUF_SIZE );

	// 差引税額
	memmove ( F403_9C, ( *m_pSnFhyo4_01Data )->Sn_F401_9F, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 控除不足還付税額
	memmove ( F403_10C, ( *m_pSnFhyo4_01Data )->Sn_F401_11F, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 差引税額 
	memmove ( F403_11C, ( *m_pSnFhyo4_01Data )->Sn_F401_12F, MONY_BUF_SIZE );

	// 譲渡割額 還付額
	memmove ( F403_12C, ( *m_pSnFhyo4_01Data )->Sn_F401_14F, MONY_BUF_SIZE );

	// 譲渡割額 納税額
	memmove ( F403_13C, ( *m_pSnFhyo4_01Data )->Sn_F401_15F, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo4_03::ReadData2()
{
	// 旧税率は全てクリア
	char mony[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "341801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "341901" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342001" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342401" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342501" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342601" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "342901" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "343001" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "343101" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "343201" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "343301" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T( "343401" ), mony );

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312102" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312103" ) );

		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "341802" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "341803" ) );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F403_1_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341902" ), F403_1_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F403_1_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341903" ), F403_1_1B );

		if ( F403_2c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312402" ), F403_2A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342002" ), F403_2A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342002" ) );
		}
		if ( F403_2c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312403" ), F403_2B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342003" ), F403_2B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342003" ) );
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F403_3A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342102" ), F403_3A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F403_3B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342103" ), F403_3B );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312702" ), F403_5A );		
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342302" ), F403_5A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312703" ), F403_5B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342303" ), F403_5B );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313002" ), F403_6A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342402" ), F403_6A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313003" ), F403_6B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342403" ), F403_6B );

	}
	else {
		//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) ) {
		if( IsUseTokurei() ){
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312102" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312103" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "341802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "341803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342002" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342003" ) );
		}
	}
}

//=========================================================
// データ書込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo4_03::WriteData()
{
	// 課税標準額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1D, F403_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1E, F403_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1F, F403_1C, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1D, F403_1_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1E, F403_1_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1F, F403_1_1C, MONY_BUF_SIZE );

	// 消費税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_2X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2D, F403_2A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2E, F403_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2F, F403_2C, MONY_BUF_SIZE );
	(*m_pSnFhyo4_01Data)->Sn_F401_2sw = F403_2c;

	// 貸倒回収に係る消費税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_3X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3D, F403_3A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3E, F403_3B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3F, F403_3C, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_4X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4D, F403_4A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4E, F403_4B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4F, F403_4C, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_5X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5D, F403_5A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5E, F403_5B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5F, F403_5C, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_6X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6D, F403_6A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6E, F403_6B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6F, F403_6C, MONY_BUF_SIZE );

	// 控除税額小計
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_7X, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7D, F403_7A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7E, F403_7B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7F, F403_7C, MONY_BUF_SIZE );

	// 差引税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_8X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_8D, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_8E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_8F, F403_8C, MONY_BUF_SIZE );

	// 差引税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_9X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_9D, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_9E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_9F, F403_9C, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_10F, '\0', MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 控除不足還付税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_11X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_11E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_11F, F403_10C, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 差引税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_12X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_12E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_12F, F403_11C, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_13X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_13E, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_13F, '\0', MONY_BUF_SIZE );

	// 譲渡割額 還付額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_14X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_14E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_14F, F403_12C, MONY_BUF_SIZE );

	// 譲渡割額 納税額
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_15X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_15E, '\0', MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_15F, F403_13C, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo4_01Data )->Sn_F401_16F, '\0', MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo4_03::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray	money;

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "34" ) );
	int cnt = ( int )money.GetCount();
	int incnt = 0;

	// 計算項目を全て再取得
	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			// 課税標準額
			if ( money[i][j].code == _T( "341802" ) ) {
				memmove ( F403_1A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341803" ) ) {
				memmove ( F403_1B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341800" ) ) {
				memmove ( F403_1C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 課税資産の譲渡等の対価の額
			else if ( money[i][j].code == _T( "341902" ) ) {
				memmove ( F403_1_1A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341903" ) ) {
				memmove ( F403_1_1B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341900" ) ) {
				memmove ( F403_1_1C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 消費税額
			else if ( money[i][j].code == _T( "342002" ) ) {
				memmove ( F403_2A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342003" ) ) {
				memmove ( F403_2B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342000" ) ) {
				memmove ( F403_2C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 貸倒回収に係る消費税額
			else if ( money[i][j].code == _T( "342102" ) ) {
				memmove ( F403_3A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342103" ) ) {
				memmove ( F403_3B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342100" ) ) {
				memmove ( F403_3C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 控除対象仕入税額
			else if ( money[i][j].code == _T( "342202" ) ) {
				memmove ( F403_4A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342203" ) ) {
				memmove ( F403_4B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342200" ) ) {
				memmove ( F403_4C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 返還等対価に係る税額
			else if ( money[i][j].code == _T( "342302" ) ) {
				memmove ( F403_5A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342303" ) ) {
				memmove ( F403_5B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342300" ) ) {
				memmove ( F403_5C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 貸倒れに係る税額
			else if ( money[i][j].code == _T( "342402" ) ) {
				memmove ( F403_6A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342403" ) ) {
				memmove ( F403_6B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342400" ) ) {
				memmove ( F403_6C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 控除税額小計
			else if ( money[i][j].code == _T( "342502" ) ) {
				memmove ( F403_7A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342503" ) ) {
				memmove ( F403_7B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342500" ) ) {
				memmove ( F403_7C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 控除不足還付税額
			else if ( money[i][j].code == _T( "342600" ) ) {
				memmove ( F403_8C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 差引税額
			else if ( money[i][j].code == _T( "342700" ) ) {
				memmove ( F403_9C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 控除不足還付税額
			else if ( money[i][j].code == _T( "342900" ) ) {
				memmove ( F403_10C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 差引税額
			else if ( money[i][j].code == _T( "343000" ) ) {
				memmove ( F403_11C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 還付額
			else if ( money[i][j].code == _T( "343200" ) ) {
				memmove ( F403_12C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 納付額
			else if ( money[i][j].code == _T( "343300" ) ) {
				memmove ( F403_13C, money[i][j].arith, MONY_BUF_SIZE );
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
int CShinFhyo4_03::SyukeiDataSet ( int cID )
{
	char			val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
		// 課税資産の譲渡等の対価の額
	case ID403_1_1A:
		memmove ( F403_1_1A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F403_1_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341902" ), F403_1_1A );
		break;
	case ID403_1_1B:
		memmove ( F403_1_1B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F403_1_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341903" ), F403_1_1B );
		break;

		// 消費税額
	case ID403_2A:
		if ( m_pArith->l_cmp ( F403_2A, val ) ) {
			F403_2c |= 0x80;
			memmove ( F403_2A, val, MONY_BUF_SIZE ); 
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342002" ), F403_2A );
		}
		break;
	case ID403_2B:
		if ( m_pArith->l_cmp ( F403_2B, val ) ) {
			F403_2c |= 0x800;
			memmove ( F403_2B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342003" ), F403_2B );
		}
		break;

		// 貸倒回収に係る消費税額
	case ID403_3A:
		memmove ( F403_3A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F403_3A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342102" ), F403_3A );
		break;
	case ID403_3B:
		memmove ( F403_3B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F403_3B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342103" ), F403_3B );
		break;

		// 控除対象仕入税額
	case ID403_4A:
		memmove ( F403_4A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342202" ), F403_4A );
		break;
	case ID403_4B:
		memmove ( F403_4B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342203" ), F403_4B );
		break;

		// 返還等対価に係る税額
	case ID403_5A:
		memmove ( F403_5A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312702" ), F403_5A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342302" ), F403_5A );
		break;
	case ID403_5B:
		memmove ( F403_5B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312703" ), F403_5B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342303" ), F403_5B );
		break;

		// 貸倒れに係る税額
	case ID403_6A:
		memmove ( F403_6A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313002" ), F403_6A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342402" ), F403_6A );
		break;
	case ID403_6B:
		memmove ( F403_6B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313003" ), F403_6B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342403" ), F403_6B );
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
int CShinFhyo4_03::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo4_03[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo4_03[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) )
				if ( (!((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() ) // 20231025 令和5年9月30日をまたぐ場合は入力可
					&& ( ( Fhyo4_03[cnt].Index == ID403_1_1A ) || ( Fhyo4_03[cnt].Index == ID403_1_1B ) ) ) {
					Flg = 0;
				}
				else {
					Flg = Fhyo4_03[cnt].UnConnect;
				}
			}
			else {
				Flg = Fhyo4_03[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo4_03[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo4_03[cnt].Connect );
		cnt++;

	}

	m_Fhyo4_03Diag02.EnableDelete ( 0 );

	return 0;
}

//=========================================================
// 画面出力
//=========================================================
void CShinFhyo4_03::DispFh4_03()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 3; i <= 5; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	for( int i = ID403_1A; i <= ID403_13C; i++ ){
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	//連動の場合手入力サイン初期化
	if( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		F403_2c = 0;
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

	// 各項目のデータ全て表示
	f403data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo4_03::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo4_03::dsp_cls ( unsigned short Id, short Index )
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

//=================================================
// 表示金額取得
//=================================================
void CShinFhyo4_03::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 課税標準額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 1-1. 課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_1_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_1_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_1_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_1_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_1_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_1_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 2. 消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_2A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 3. 貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_3A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_3B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_3B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_3C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_3C, DiagData.data_val, MONY_BUF_SIZE );

	// 4. 控除対象仕入税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_4A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_4A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_4B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_4C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_4C, DiagData.data_val, MONY_BUF_SIZE );

	// 5. 返還等対価に係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_5A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_5A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_5B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_5C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_5C, DiagData.data_val, MONY_BUF_SIZE );

	// 6. 貸倒れに係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_6A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_6A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_6B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_6B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_6C, DiagData.data_val, MONY_BUF_SIZE );

	// 7. 控除税額小計
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_7A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_7A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_7B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_7B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_7C, DiagData.data_val, MONY_BUF_SIZE );

	// 8. 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_8C, DiagData.data_val, MONY_BUF_SIZE );

	// 9. 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_9C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_9C, DiagData.data_val, MONY_BUF_SIZE );

	// 10. 地方消費税の課税標準となる消費税 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_10C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_10C, DiagData.data_val, MONY_BUF_SIZE );

	// 11. 地方消費税の課税標準となる消費税 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_11C, DiagData.data_val, MONY_BUF_SIZE );

	// 12. 譲渡割額 還付額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_12C, DiagData.data_val, MONY_BUF_SIZE );

	// 13. 譲渡割額 納税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID403_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F403_13C, DiagData.data_val, MONY_BUF_SIZE );
}


//=========================================================
// ポジションセット
//=========================================================
void CShinFhyo4_03::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Fhyo4_03Diag02.GetPosition() ) != -1 ) {
			m_Fhyo4_03Diag01.SetFocus();	// ダミーでセット
			m_Fhyo4_03Diag02.SetPosition ( index );
			m_Fhyo4_03Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo4_03Diag01.SetFocus();	// ダミーでセット
	m_Fhyo4_03Diag02.SetFocus();


	//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) ) {
	if( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() ){	// 20231025 令和5年9月30日をまたぐ場合は入力可
		// 消費税額A欄
		m_Fhyo4_03Diag02.SetPosition ( ID403_2A );
	}
	else {
		// 課税標準額A欄
		m_Fhyo4_03Diag02.SetPosition ( ID403_1_1A );
	}

}

//=========================================================
// 課税取引金額計算書から転記時のフォーカス移動
//=========================================================
void CShinFhyo4_03::MoveFocusInTaxTenki()
{
	m_Fhyo4_03Diag02.SetPosition ( ID403_1_1A );
}

//=========================================================
// フォーカス移動先取得
//---------------------------------------------------------
// 引数
//			Index		：	ダイアグラム内インデックス
//			Move		：	0 = 下方向, 1 = 上方向
//=========================================================
int CShinFhyo4_03::UpDownFocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY4_03CNT; i++ ) {
		if ( Index == FCS_Fhyo4_03[i].IDNo ) {
			break;
		}
	}

	// 現在ポジション
	idx = FCS_Fhyo4_03[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) { //下方向
		for ( j = i + 1; j < FCS_FHY4_03CNT; j++ ) {
			m_Fhyo4_03Diag02.GetAttribute ( FCS_Fhyo4_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo4_03[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) { //上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo4_03Diag02.GetAttribute ( FCS_Fhyo4_03[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo4_03[j].IDNo;
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
void CShinFhyo4_03::f403data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f403data_1();
		f403data_1_1();

	case 2:
		f403data_2();

	case 3:
		f403data_3();

	case 4:
		f403data_4();

	case 5:
		f403data_5();

	case 6:	
		f403data_6();

	case 7:
		f403data_7();

	case 8:
		f403data_8();

	case 9:
		f403data_9();

	case 10:
		f403data_10();

	case 11:
		f403data_11();

	case 12:
		f403data_12();

	case 13:
		f403data_13();
		break;
	default	:
		break;
	}
}

//=========================================================
// 課税標準額表示
//=========================================================
void CShinFhyo4_03::f403data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_1A, F403_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_1B, F403_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_1C, F403_1C );
}

//=========================================================
// 課税資産の譲渡等の対価の額表示
//=========================================================
void CShinFhyo4_03::f403data_1_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_1_1A, F403_1_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_1_1B, F403_1_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_1_1C, F403_1_1C );
}

//=========================================================
// 消費税額表示
//=========================================================
void CShinFhyo4_03::f403data_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_2A, F403_2A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_2B, F403_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_2C, F403_2C );

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID403_2A, ( F403_2c & 0x80 ) ? 3 : 0 );	
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID403_2B, ( F403_2c & 0x800 ) ? 3 : 0 );	
	}
}

//=========================================================
// 貸倒回収に係る消費税額表示
//=========================================================
void CShinFhyo4_03::f403data_3()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_3A, F403_3A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_3B, F403_3B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_3C, F403_3C );
}

//=========================================================
// 控除対象仕入税額表示
//=========================================================
void CShinFhyo4_03::f403data_4()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_4A, F403_4A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_4B, F403_4B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_4C, F403_4C );
}

//=========================================================
// 返還等対価に係る税額表示
//=========================================================
void CShinFhyo4_03::f403data_5()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_5A, F403_5A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_5B, F403_5B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_5C, F403_5C );
}

//=========================================================
// 貸倒れに係る税額表示
//=========================================================
void CShinFhyo4_03::f403data_6()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_6A, F403_6A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_6B, F403_6B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_6C, F403_6C );
}

//=========================================================
// 控除税額小計表示
//=========================================================
void CShinFhyo4_03::f403data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_7A, F403_7A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_7B, F403_7B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_7C, F403_7C );
}

//=========================================================
// 控除不足還付税額表示
//=========================================================
void CShinFhyo4_03::f403data_8()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_8C, F403_8C );
}

//=========================================================
// 差引税額表示
//=========================================================
void CShinFhyo4_03::f403data_9()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_9C, F403_9C );
}

//=========================================================
// 地方消費税の課税標準となる消費税額 控除不足還付税額 表示
//=========================================================
void CShinFhyo4_03::f403data_10()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_10C, F403_10C );
}

//=========================================================
// 地方消費税の課税標準となる消費税額 差引税額 表示
//=========================================================
void CShinFhyo4_03::f403data_11()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_11C, F403_11C );
}

//=========================================================
// 譲渡割額 還付額 表示
//=========================================================
void CShinFhyo4_03::f403data_12()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_12C, F403_12C );
}

//=========================================================
// 譲渡割額 納税額 表示
//=========================================================
void CShinFhyo4_03::f403data_13()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID403_13C, F403_13C );
}

//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinFhyo4_03::ChangeColor ( unsigned short id, short Index, int sign )
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
BOOL CShinFhyo4_03::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F403_2c & 0x880 ) {
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
bool CShinFhyo4_03::IsUseTokurei()
{
	bool rt = false;

	if( (*m_pSnHeadData)->m_DispTabSgn&0x80 && (*m_pSnHeadData)->m_DispTabSgn&0x10 ){
		// 特例5-1使用
		rt = true;
	}

	return rt;
}