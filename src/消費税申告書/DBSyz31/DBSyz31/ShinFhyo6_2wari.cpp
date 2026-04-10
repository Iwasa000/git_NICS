// ShinFhyo6_2wari.cpp : 実装ファイル
//

//#include "pch.h"
#include "stdafx.h"
#include "ShinFhyo6_2wari.h"
#include "ShinFhyo6_2wariidx.h"
//#include "afxdialogex.h"


// ShinFhyo6_2wari ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo6_2wari, CSyzBaseDlg)

CShinFhyo6_2wari::CShinFhyo6_2wari(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinFhyo6_2wari::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo6_01Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pShinInfo ( NULL )
	, m_pSyzSyukei ( NULL )
{
	memset ( F601_1A, '\0', sizeof ( F601_1A ) );
	memset ( F601_1B, '\0', sizeof ( F601_1B ) );
	memset ( F601_1C, '\0', sizeof ( F601_1C ) );

	memset ( F601_2A, '\0', sizeof ( F601_2A ) );
	memset ( F601_2B, '\0', sizeof ( F601_2B ) );
	memset ( F601_2C, '\0', sizeof ( F601_2C ) );

	memset ( F601_3A, '\0', sizeof ( F601_3A ) );
	memset ( F601_3B, '\0', sizeof ( F601_3B ) );
	memset ( F601_3C, '\0', sizeof ( F601_3C ) );

	memset ( F601_4A, '\0', sizeof ( F601_4A ) );
	memset ( F601_4B, '\0', sizeof ( F601_4B ) );
	memset ( F601_4C, '\0', sizeof ( F601_4C ) );

	memset ( F601_5A, '\0', sizeof ( F601_5A ) );
	memset ( F601_5B, '\0', sizeof ( F601_5B ) );
	memset ( F601_5C, '\0', sizeof ( F601_5C ) );

	memset ( F601_6A, '\0', sizeof ( F601_6A ) );
	memset ( F601_6B, '\0', sizeof ( F601_6B ) );
	memset ( F601_6C, '\0', sizeof ( F601_6C ) );

	memset ( F601_7A, '\0', sizeof ( F601_7A ) );
	memset ( F601_7B, '\0', sizeof ( F601_7B ) );
	memset ( F601_7C, '\0', sizeof ( F601_7C ) );

	memset ( F601_8A, '\0', sizeof ( F601_8A ) );
	memset ( F601_8B, '\0', sizeof ( F601_8B ) );
	memset ( F601_8C, '\0', sizeof ( F601_8C ) );

	F601_3c = 0;

	m_curwnd = -1;
}

CShinFhyo6_2wari::~CShinFhyo6_2wari()
{
}

void CShinFhyo6_2wari::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo6_01Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo6_01Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo6_2wari, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinFhyo6_2wari, CSyzBaseDlg)
	ON_EVENT(CShinFhyo6_2wari, IDC_ICSDIAG8CTRL2, 1, CShinFhyo6_2wari::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo6_2wari, IDC_ICSDIAG8CTRL2, 2, CShinFhyo6_2wari::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo6_2wari, IDC_ICSDIAG8CTRL2, 3, CShinFhyo6_2wari::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// ShinFhyo4_03 メッセージ ハンドラー


BOOL CShinFhyo6_2wari::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	m_pBottomDiag = ( CIcsdiagctrl* )GetDlgItem ( IDC_ICSDIAG8CTRL2 );
//----->('23.05.01 cor)
	//m_BottomIdx = 97;
//------
	m_BottomIdx = 87;//コントロール一番最下のDiagのID指定。これで縦スクロール出る
//<-----

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo6_2wari::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo6_2wari::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo6_2wari::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BOOL CShinFhyo6_2wari::PreTranslateMessage(MSG* pMsg)
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

void CShinFhyo6_2wari::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo6_2wari::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char CkMy[MONY_BUF_SIZE] = {0};

	switch ( index ) {
	case ID601_1A:	//課税標準額
	case ID601_1B:
		SyukeiDataSet ( index );
		f601data_all ( 1 );
		break;
	case ID601_3A:	//課税標準額に対する消費税額
	case ID601_3B:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL2, index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( index == ID601_3A ) {
			if ( m_pArith->l_cmp ( F601_3A, CkMy ) ) {
				F601_3c |= 0x80;
				F601_3c &= ~0x10;
			}
		}
		else if ( index == ID601_3B ) {
//			if ( m_pArith->l_cmp ( F601_1B, CkMy ) ) {	//('23.05.01 Del)
			if ( m_pArith->l_cmp ( F601_3B, CkMy ) ) {
				F601_3c |= 0x800;
				F601_3c &= ~0x100;
			}
		}
		SyukeiDataSet ( index );
		f601data_all ( 3 );
		break;
	case ID601_4A:	//貸倒回収に係る消費税額
	case ID601_4B:
		SyukeiDataSet ( index );
		f601data_all ( 4 );
		break;
	case ID601_5A:	//売上対価の返還等に係る消費税額
	case ID601_5B:
		SyukeiDataSet ( index );
		f601data_all ( 5 );
		break;
	case ID601_8A:	//貸倒れに係る税額
	case ID601_8B:
		SyukeiDataSet ( index );
		f601data_all ( 8 );
		break;
	default:
		break;
	}
}

void CShinFhyo6_2wari::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Fhyo6_01Diag02.SetPosition ( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index, 0 );
		m_Fhyo6_01Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) ) {
		m_Fhyo6_01Diag02.SetNextPosition();
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo6_01Diag02.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
		case ID601_3A:
			F601_3c &= ~0x80;

			( *m_pSyzSyukei )->ResetShinkokuData ( _T("312402") );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T("342002") );
			f601data_all ( 0 );
			m_Fhyo6_01Diag02.SetPosition ( ID601_3A );
			break;

		case ID601_3B:
			F601_3c &= ~0x800;

			( *m_pSyzSyukei )->ResetShinkokuData ( _T("312403") );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T("342003") );
			f601data_all ( 0 );
			m_Fhyo6_01Diag02.SetPosition ( ID601_3B );
			break;

		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		}
	}

	int nowpos = m_Fhyo6_01Diag02.GetPosition();
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
//		pSnFhyo6_01Data	：	付表6データクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSyzSyukei		：	集計クラス
//		pArith			：	倍長演算クラス
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
//int CShinFhyo6_2wari::InitInfo ( CH31SnFhyo4_01Data **pSnFhyo4_01Data, CH31SnFhyo5_01Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith )
int CShinFhyo6_2wari::InitInfo(CH31SnFhyo6_2wariData **pSnFhyo6_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith){

	ASSERT(pSnFhyo6_01Data);
	if (pSnFhyo6_01Data == NULL) {
		return -1;
	}
	m_pSnFhyo6_01Data = pSnFhyo6_01Data;

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
int CShinFhyo6_2wari::Init()
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
		m_Fhyo6_01Diag01.ModifyItem ( 3, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh6_01();

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
int CShinFhyo6_2wari::ReadData()
{
	// 課税資産の譲渡等の対価の額
	memmove ( F601_1A, ( *m_pSnFhyo6_01Data )->Sn_F601_1A, MONY_BUF_SIZE );
	memmove ( F601_1B, ( *m_pSnFhyo6_01Data )->Sn_F601_1B, MONY_BUF_SIZE );
	memmove ( F601_1C, ( *m_pSnFhyo6_01Data )->Sn_F601_1C, MONY_BUF_SIZE );

	// 課税標準額
	memmove ( F601_2A, ( *m_pSnFhyo6_01Data )->Sn_F601_2A, MONY_BUF_SIZE );
	memmove ( F601_2B, ( *m_pSnFhyo6_01Data )->Sn_F601_2B, MONY_BUF_SIZE );
	memmove ( F601_2C, ( *m_pSnFhyo6_01Data )->Sn_F601_2C, MONY_BUF_SIZE );

	// 課税標準額に対する消費税額
	memmove ( F601_3A, ( *m_pSnFhyo6_01Data )->Sn_F601_3A, MONY_BUF_SIZE );
	memmove ( F601_3B, ( *m_pSnFhyo6_01Data )->Sn_F601_3B, MONY_BUF_SIZE );
	memmove ( F601_3C, ( *m_pSnFhyo6_01Data )->Sn_F601_3C, MONY_BUF_SIZE );
	F601_3c = ( *m_pSnFhyo6_01Data )->Sn_F601_3sw;	// 手入力サイン 

	// 貸倒回収に係る消費税額
	memmove ( F601_4A, ( *m_pSnFhyo6_01Data )->Sn_F601_4A, MONY_BUF_SIZE );
	memmove ( F601_4B, ( *m_pSnFhyo6_01Data )->Sn_F601_4B, MONY_BUF_SIZE );
	memmove ( F601_4C, ( *m_pSnFhyo6_01Data )->Sn_F601_4C, MONY_BUF_SIZE );

	// 売上対価の返還等に係る消費税額
	memmove ( F601_5A, ( *m_pSnFhyo6_01Data )->Sn_F601_5A, MONY_BUF_SIZE );
	memmove ( F601_5B, ( *m_pSnFhyo6_01Data )->Sn_F601_5B, MONY_BUF_SIZE );
	memmove ( F601_5C, ( *m_pSnFhyo6_01Data )->Sn_F601_5C, MONY_BUF_SIZE );
	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove ( F601_6A, ( *m_pSnFhyo6_01Data )->Sn_F601_6A, MONY_BUF_SIZE );
	memmove ( F601_6B, ( *m_pSnFhyo6_01Data )->Sn_F601_6B, MONY_BUF_SIZE );
	memmove ( F601_6C, ( *m_pSnFhyo6_01Data )->Sn_F601_6C, MONY_BUF_SIZE );

	// 特別控除税額
	memmove ( F601_7A, ( *m_pSnFhyo6_01Data )->Sn_F601_7A, MONY_BUF_SIZE );
	memmove ( F601_7B, ( *m_pSnFhyo6_01Data )->Sn_F601_7B, MONY_BUF_SIZE );
	memmove ( F601_7C, ( *m_pSnFhyo6_01Data )->Sn_F601_7C, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( F601_8A, ( *m_pSnFhyo6_01Data )->Sn_F601_8A, MONY_BUF_SIZE );
	memmove ( F601_8B, ( *m_pSnFhyo6_01Data )->Sn_F601_8B, MONY_BUF_SIZE );
	memmove ( F601_8C, ( *m_pSnFhyo6_01Data )->Sn_F601_8C, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 読込みデータを集計モジュールへ
//=========================================================
void CShinFhyo6_2wari::ReadData2()
{

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		//課税資産の譲渡等の対価の額
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F601_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341902" ), F601_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F601_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341903" ), F601_1B );

		//課税標準額
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312102"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312103"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341802"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341803"));

		//課税標準額に対する消費税額
		if ( F601_3c & 0x80 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312402" ), F601_3A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342002" ), F601_3A );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342002" ) );
		}
		if ( F601_3c & 0x800 ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312403" ), F601_3B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342003" ), F601_3B );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342003" ) );
		}

		//貸倒回収に係る消費税額
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F601_4A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342102" ), F601_4A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F601_4B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342103" ), F601_4B );

		//返還等対価に係る税額
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312702" ), F601_5A );	
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342302" ), F601_5A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312703" ), F601_5B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342303" ), F601_5B );

//----->(メモ)
//		付表５－３由来の「控除対象仕入税額の計算の基礎となる消費税額」「特別控除税額」は
//		他項目を使用して計算を行う為か、Set/Reset共に行っていない。
//<-----

		//貸倒れに係る税額
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313002" ), F601_8A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342402" ), F601_8A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313003" ), F601_8B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342403" ), F601_8B );

	}
}

//=========================================================
// データ書込み
//--------------------------------------------------------
// 返送値
//		0	:	正常終了
//=========================================================
int CShinFhyo6_2wari::WriteData()
{
	//(メモ)書き込み時は付表６データの方へ行う
	// 課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_1A, F601_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_1B, F601_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_1C, F601_1C, MONY_BUF_SIZE );

	// 課税標準額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_2A, F601_2A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_2B, F601_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_2C, F601_2C, MONY_BUF_SIZE );

	// 課税標準額に対する消費税額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_3A, F601_3A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_3B, F601_3B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_3C, F601_3C, MONY_BUF_SIZE );
	(*m_pSnFhyo6_01Data)->Sn_F601_3sw = F601_3c;

	// 貸倒回収に係る消費税額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_4A, F601_4A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_4B, F601_4B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_4C, F601_4C, MONY_BUF_SIZE );

	// 売上対価の返還等に係る消費税額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_5A, F601_5A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_5B, F601_5B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_5C, F601_5C, MONY_BUF_SIZE );

	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_6A, F601_6A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_6B, F601_6B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_6C, F601_6C, MONY_BUF_SIZE );

	// 特別控除税額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_7A, F601_7A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_7B, F601_7B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_7C, F601_7C, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_8A, F601_8A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_8B, F601_8B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo6_01Data )->Sn_F601_8C, F601_8C, MONY_BUF_SIZE );

	return 0;
}

//=========================================================
// 集計処理
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//=========================================================
int CShinFhyo6_2wari::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray	money;

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "34" ) );
	int cnt = ( int )money.GetCount();
	int incnt = 0;

	// 付表４－３の計算項目を全て再取得	
	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			// 課税資産の譲渡等の対価の額
			if ( money[i][j].code == _T( "341902" ) ) {
				memmove ( F601_1A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341903" ) ) {
				memmove ( F601_1B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341900" ) ) {
				memmove ( F601_1C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 課税標準額
			else if ( money[i][j].code == _T( "341802" ) ) {
				memmove ( F601_2A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341803" ) ) {
				memmove ( F601_2B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "341800" ) ) {
				memmove ( F601_2C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 課税標準額に対する消費税額
			else if ( money[i][j].code == _T( "342002" ) ) {
				memmove ( F601_3A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342003" ) ) {
				memmove ( F601_3B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342000" ) ) {
				memmove ( F601_3C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 貸倒回収に係る消費税額
			else if ( money[i][j].code == _T( "342102" ) ) {
				memmove ( F601_4A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342103" ) ) {
				memmove ( F601_4B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342100" ) ) {
				memmove ( F601_4C, money[i][j].arith, MONY_BUF_SIZE );
			}

			// 売上対価の返還等に係る消費税額
			else if ( money[i][j].code == _T( "342302" ) ) {
				memmove ( F601_5A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342303" ) ) {
				memmove ( F601_5B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342300" ) ) {
				memmove ( F601_5C, money[i][j].arith, MONY_BUF_SIZE );
			}
			// 貸倒れに係る税額
			else if ( money[i][j].code == _T( "342402" ) ) {
				memmove ( F601_8A, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342403" ) ) {
				memmove ( F601_8B, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if ( money[i][j].code == _T( "342400" ) ) {
				memmove ( F601_8C, money[i][j].arith, MONY_BUF_SIZE );
			}
		}
	}

	money = (*m_pSyzSyukei)->GetShinkokuData(_T("35"));
	cnt = (int)money.GetCount();
	incnt = 0;

	// 付表５－３の計算項目を全て再取得	
	for (int i = 0; i < cnt; i++) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++) {
			// 控除対象仕入税額の計算の基礎となる消費税額
			if (money[i][j].code == _T("357402")) {
				memmove(F601_6A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("357403")) {
				memmove(F601_6B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("357400")) {
				memmove(F601_6C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 特別控除税額
			else if (money[i][j].code == _T("357502")) {
				memmove(F601_7A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("357503")) {
				memmove(F601_7B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("357500")) {
				memmove(F601_7C, money[i][j].arith, MONY_BUF_SIZE);
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
int CShinFhyo6_2wari::SyukeiDataSet ( int cID )
{
	char			val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
		// 課税資産の譲渡等の対価の額
	case ID601_1A:
		memmove ( F601_1A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), F601_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341902" ), F601_1A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F601_1A );
		break;
	case ID601_1B:
		memmove ( F601_1B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), F601_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "341903" ), F601_1B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F601_1B );
		break;

		// 課税標準額に対する消費税額
	case ID601_3A:
		if ( m_pArith->l_cmp ( F601_3A, val ) ) {
			F601_3c |= 0x80;
			memmove ( F601_3A, val, MONY_BUF_SIZE ); 
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312402" ), F601_3A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342002" ), F601_3A );
		}
		break;
	case ID601_3B:
		if ( m_pArith->l_cmp ( F601_3B, val ) ) {
			F601_3c |= 0x800;
			memmove ( F601_3B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312403" ), F601_3B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "342003" ), F601_3B );
		}
		break;

		// 貸倒回収に係る消費税額
	case ID601_4A:
		memmove ( F601_4A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325002" ), F601_4A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342102" ), F601_4A );
		break;
	case ID601_4B:
		memmove ( F601_4B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "325003" ), F601_4B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342103" ), F601_4B );
		break;
		// 売上対価の返還等に係る消費税額
	case ID601_5A:
		memmove ( F601_5A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312702" ), F601_5A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342302" ), F601_5A );
		break;
	case ID601_5B:
		memmove ( F601_5B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "312703" ), F601_5B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342303" ), F601_5B );
		break;

		// 貸倒れに係る税額
	case ID601_8A:
		memmove ( F601_8A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313002" ), F601_8A );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342402" ), F601_8A );
		break;
	case ID601_8B:
		memmove ( F601_8B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "313003" ), F601_8B );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "342403" ), F601_8B );
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
int CShinFhyo6_2wari::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo6_01[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo6_01[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo6_01[cnt].UnConnect;
			}
			else {
				Flg = Fhyo6_01[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo6_01[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo6_01[cnt].Connect );
		cnt++;

	}

	m_Fhyo6_01Diag02.EnableDelete ( 0 );

	return 0;
}

//=========================================================
// 画面出力
//=========================================================
void CShinFhyo6_2wari::DispFh6_01()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 3; i <= 5; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	for( int i = ID601_1A; i <= ID601_8C; i++ ){
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	//連動の場合手入力サイン初期化
	if( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		F601_3c = 0;
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
	f601data_all ( 0 );
}

//=========================================================
// 金額表示
//---------------------------------------------------------
// 引数
//			Id			：	ダイアグラムID
//			Index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo6_2wari::dsp_prs ( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo6_2wari::dsp_cls ( unsigned short Id, short Index )
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
void CShinFhyo6_2wari::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_1C, DiagData.data_val, MONY_BUF_SIZE );

	// 2. 課税標準額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_2A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_2C, DiagData.data_val, MONY_BUF_SIZE );

	// 3. 課税標準額に対する消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_3A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_3B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_3B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_3C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_3C, DiagData.data_val, MONY_BUF_SIZE );

	// 4. 貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_4A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_4A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_4B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_4C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_4C, DiagData.data_val, MONY_BUF_SIZE );

	// 5. 売上対価の返還等に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_5A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_5A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_5B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_5C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_5C, DiagData.data_val, MONY_BUF_SIZE );

	// 6. 控除対象仕入税額の計算の基礎となる消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_6A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_6A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_6B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_6B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_6C, DiagData.data_val, MONY_BUF_SIZE );

	// 7. 特別控除税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_7A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_7A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_7B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_7B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_7C, DiagData.data_val, MONY_BUF_SIZE );

	// 8. 貸倒れに係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_8A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_8A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_8B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_8B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID601_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F601_8C, DiagData.data_val, MONY_BUF_SIZE );

}


//=========================================================
// ポジションセット
//=========================================================
void CShinFhyo6_2wari::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Fhyo6_01Diag02.GetPosition() ) != -1 ) {
			m_Fhyo6_01Diag01.SetFocus();	// ダミーでセット
			m_Fhyo6_01Diag02.SetPosition ( index );
			m_Fhyo6_01Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo6_01Diag01.SetFocus();	// ダミーでセット
	m_Fhyo6_01Diag02.SetFocus();


	// 課税資産の譲渡等の対価の額A欄
	m_Fhyo6_01Diag02.SetPosition ( ID601_1A );
}

//=========================================================
// 課税取引金額計算書から転記時のフォーカス移動
//=========================================================
void CShinFhyo6_2wari::MoveFocusInTaxTenki()
{
	//課税資産の譲渡等の対価の額
	m_Fhyo6_01Diag02.SetPosition ( ID601_1A );
}

//=========================================================
// フォーカス移動先取得
//---------------------------------------------------------
// 引数
//			Index		：	ダイアグラム内インデックス
//			Move		：	0 = 下方向, 1 = 上方向
//=========================================================
int CShinFhyo6_2wari::UpDownFocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY6_01CNT; i++ ) {
		if ( Index == FCS_Fhyo6_01[i].IDNo ) {
			break;
		}
	}

	// 現在ポジション
	idx = FCS_Fhyo6_01[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) { //下方向
		for ( j = i + 1; j < FCS_FHY6_01CNT; j++ ) {
			m_Fhyo6_01Diag02.GetAttribute ( FCS_Fhyo6_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo6_01[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) { //上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo6_01Diag02.GetAttribute ( FCS_Fhyo6_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo6_01[j].IDNo;
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
void CShinFhyo6_2wari::f601data_all ( int atno )
{
	switch( atno ){
	case 0:
	case 1:
		f601data_1();
		
	case 2:
		f601data_2();

	case 3:
		f601data_3();

	case 4:
		f601data_4();

	case 5:
		f601data_5();

	case 6:	
		f601data_6();

	case 7:
		f601data_7();

	case 8:
		f601data_8();
		break;
	default	:
		break;
	}
}

//=========================================================
// 課税資産の譲渡等の対価の額表示
//=========================================================
void CShinFhyo6_2wari::f601data_1()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_1A, F601_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_1B, F601_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_1C, F601_1C );
}

//=========================================================
// 課税標準額表示
//=========================================================
void CShinFhyo6_2wari::f601data_2()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_2A, F601_2A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_2B, F601_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_2C, F601_2C );
}

//=========================================================
// 課税標準額に対する消費税額表示
//=========================================================
void CShinFhyo6_2wari::f601data_3()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_3A, F601_3A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_3B, F601_3B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_3C, F601_3C );
	
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID601_3A, ( F601_3c & 0x80 ) ? 3 : 0 );	
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID601_3B, ( F601_3c & 0x800 ) ? 3 : 0 );	
	}
}

//=========================================================
// 貸倒回収に係る消費税額表示
//=========================================================
void CShinFhyo6_2wari::f601data_4()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_4A, F601_4A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_4B, F601_4B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_4C, F601_4C );
}

//=========================================================
// 売上対価の返還等に係る消費税額表示
//=========================================================
void CShinFhyo6_2wari::f601data_5()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_5A, F601_5A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_5B, F601_5B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_5C, F601_5C );
}

//=========================================================
// 控除対象仕入税額の計算の基礎となる消費税額表示
//=========================================================
void CShinFhyo6_2wari::f601data_6()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_6A, F601_6A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_6B, F601_6B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_6C, F601_6C );
}

//=========================================================
// 特別控除税額表示
//=========================================================
void CShinFhyo6_2wari::f601data_7()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_7A, F601_7A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_7B, F601_7B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_7C, F601_7C );
}

//=========================================================
// 貸倒れに係る税額表示
//=========================================================
void CShinFhyo6_2wari::f601data_8()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_8A, F601_8A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_8B, F601_8B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID601_8C, F601_8C );
}

//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinFhyo6_2wari::ChangeColor ( unsigned short id, short Index, int sign )
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
BOOL CShinFhyo6_2wari::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F601_3c & 0x880 ) {
			ret = TRUE;
		}
	}

	return ret;
}