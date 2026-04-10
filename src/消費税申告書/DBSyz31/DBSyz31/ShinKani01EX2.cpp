// ShinKani01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKani01EX2.h"
#include "ShinKani01idxEX2.h"

#include "H31HyoView.h"
#include "PrePrint.h"
#include "ChkMsg.h"


// CShinKani01EX2 ダイアログ

IMPLEMENT_DYNAMIC(CShinKani01EX2, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinKani01EX2::CShinKani01EX2(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinKani01EX2::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnHonpyoData ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pArith ( NULL )
	, m_pShinInfo ( NULL )
	, m_curInPos ( -1 )
{
	memset ( XM1, '\0', sizeof ( XM1 ) );
	memset ( XM2, '\0', sizeof ( XM2 ) );
	memset ( XM3, '\0', sizeof ( XM3 ) );
	memset ( XM4, '\0', sizeof ( XM4 ) );
	memset ( XM5, '\0', sizeof ( XM5 ) );
	memset ( XM6, '\0', sizeof ( XM6 ) );
	memset ( XM7, '\0', sizeof ( XM7 ) );
	memset ( XM8, '\0', sizeof ( XM8 ) );
	memset ( XM9, '\0', sizeof ( XM9 ) );
	memset ( XM10, '\0', sizeof ( XM10 ) );
	memset ( XM11, '\0', sizeof ( XM11 ) );
	memset ( XM12, '\0', sizeof ( XM12 ) );
	memset ( XM13, '\0', sizeof ( XM13 ) );
	memset ( XM14, '\0', sizeof ( XM14 ) );
	memset ( XM15, '\0', sizeof ( XM15 ) );
	memset ( XM16, '\0', sizeof ( XM16 ) );
	memset ( XM17, '\0', sizeof ( XM17 ) );
	memset ( XM18, '\0', sizeof ( XM18 ) );
	memset ( XM19, '\0', sizeof ( XM19 ) );
	memset ( XM20, '\0', sizeof ( XM20 ) );
	memset ( XM21, '\0', sizeof ( XM21 ) );
	memset ( XM22, '\0', sizeof ( XM22 ) );
	memset ( XM23, '\0', sizeof ( XM23 ) );
	memset ( XM24, '\0', sizeof ( XM24 ) );
	memset ( XM25, '\0', sizeof ( XM25 ) );
	memset ( XM26, '\0', sizeof ( XM26 ) );

	memset ( KURI1, '\0', sizeof ( KURI1 ) );
	memset ( KURI2, '\0', sizeof ( KURI2 ) );
	memset ( KURI3, '\0', sizeof ( KURI3 ) );
	memset ( KURI4, '\0', sizeof ( KURI4 ) );
	memset ( KURI5, '\0', sizeof ( KURI5 ) );
	memset ( KURI6, '\0', sizeof ( KURI6 ) );

	SW_WARIFU = SW_NOBEBA = SW_KOUJIS = SW_GENKIN = SW_KISK22 = 0;

	m_curwnd = -1;

	m_IsKanpuEnable = TRUE;
}

CShinKani01EX2::~CShinKani01EX2()
{
}

void CShinKani01EX2::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Kani01Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Kani01Diag02);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Kani01Diag03);
	DDX_Control(pDX, IDC_EDIT_MYNUM, m_edit_mynum1);
	DDX_Control(pDX, IDC_EDIT_MYNUM2, m_edit_mynum2);
	DDX_Control(pDX, IDC_EDIT_MYNUM3, m_edit_mynum3);
	DDX_Control(pDX, IDC_EDIT_MYNUM4, m_edit_mynum4);
}


BEGIN_MESSAGE_MAP(CShinKani01EX2, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinKani01EX2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CShinKani01EX2 メッセージ ハンドラ

BOOL CShinKani01EX2::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	// 初期化
	Init();

	// ダイアグラム初期設定
	ChgDiagAttr();

	// 「(個人の方)公金受取口座の利用」にチェックがある場合は還付金融機関を入力不可
	if( (*m_pSnHeadData)->Sn_GenericSgn & 0x400 ){
		EnableKanpuKinyu( SW_HIDE );
	}

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 84;//コントロール一番最下のDiagのID指定。これで縦スクロール出る


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinKani01EX2::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	MoveDiagMNEdit();
}

void CShinKani01EX2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MoveDiagMNEdit();

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinKani01EX2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

//=============================================================
// 初期情報のセット
//-------------------------------------------------------------
// 引数
//		pSnHonpyoData	：	本表データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSyzSyukei		:	集計モジュール
//-------------------------------------------------------------
// 返送値
//		0				:	正常終了
//		-1				:	エラー
//=============================================================
int CShinKani01EX2::InitInfo ( CSnHeadData **pSnHeadData, CH31SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CSyzSyukei **pSyzSyukei )
{
	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT ( pSnHonpyoData );
	if ( pSnHonpyoData == NULL ) {
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT ( pArith );
	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT ( pShinInfo );
	if ( pShinInfo == NULL ) {
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	return 0;
}

//================================
// 初期化
//--------------------------------
// 返送値
//			0	:	正常終了
//			-1	:	エラー
//================================
int CShinKani01EX2::Init()
{
	// ダイアグラム初期属性セット
	InitDiagAttr();

	// データ読込み
	ReadData();

	// 集計
	ReadData2();
	MemoryReget();
	GetFhyoData();

	// 初期データ表示
	DispKani();

	// 「(個人の方)公金受取口座の利用」にチェックがある場合は還付金融機関を入力不可
	if( (*m_pSnHeadData)->Sn_GenericSgn & 0x400 ){
		EnableKanpuKinyu( SW_HIDE );
	}

	return 0;
}

//=====================================
// ダイアグラム初期設定
//-------------------------------------
// 返送値
//		0	:	正常終了
//=====================================
int CShinKani01EX2::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE DA;

	// 納税地 住所(上)
	m_Kani01Diag01.GetAttribute ( ID2030, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag01.SetAttribute ( ID2030, ( LPUNKNOWN )&DA, TRUE );

	// 納税地 住所(下)
	m_Kani01Diag01.GetAttribute ( ID2031, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag01.SetAttribute ( ID2031, ( LPUNKNOWN )&DA, TRUE );

	// 氏名・屋号
	m_Kani01Diag01.GetAttribute ( ID2051, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag01.SetAttribute ( ID2051, ( LPUNKNOWN )&DA, TRUE );

	// 被合併名
	m_Kani01Diag01.GetAttribute ( ID2052, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag01.SetAttribute ( ID2052, ( LPUNKNOWN )&DA, TRUE );

	// 代表者氏名
	m_Kani01Diag01.GetAttribute ( ID2061, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag01.SetAttribute ( ID2061, ( LPUNKNOWN )&DA, TRUE );

	// 法人・個人番号
	int kubun = 0;
	HWND edit[10] = { NULL };
	int diag_index[2][10] = { { -1 }, -1 };
	int nKojinDispModify = 0;
	int nHojinDispModify = 0;

	if ( ( *m_pSnHeadData )->IsSoleProprietor() == TRUE ) {	// 個人
		kubun = MN_KOJIN;

		edit[0] = m_edit_mynum1.GetSafeHwnd();
		edit[1] = m_edit_mynum2.GetSafeHwnd();
		edit[2] = m_edit_mynum3.GetSafeHwnd();
		m_edit_mynum4.ShowWindow ( SW_HIDE );

		diag_index[0][0] = KANI01EX2_NUM_KOJIN1;
		diag_index[0][1] = KANI01EX2_NUM_KOJIN2;
		diag_index[0][2] = KANI01EX2_NUM_KOJIN3;
		diag_index[0][3] = -1;

		nKojinDispModify = DIAG_MDFY_OPAQUE | DIAG_MDFY_READONLY;
		nHojinDispModify = DIAG_MDFY_TRANSPARENT;
	}
	else {	// 法人
		kubun = MN_HOJIN;

		edit[0] = m_edit_mynum1.GetSafeHwnd();
		edit[1] = m_edit_mynum2.GetSafeHwnd();
		edit[2] = m_edit_mynum3.GetSafeHwnd();
		edit[3] = m_edit_mynum4.GetSafeHwnd();

		diag_index[0][0] = KANI01EX2_NUM_HOJIN1;		
		diag_index[0][1] = KANI01EX2_NUM_HOJIN2;
		diag_index[0][2] = KANI01EX2_NUM_HOJIN3;
		diag_index[0][3] = KANI01EX2_NUM_HOJIN4;
		diag_index[0][4] = -1;

		nKojinDispModify = DIAG_MDFY_TRANSPARENT;
		nHojinDispModify = DIAG_MDFY_OPAQUE | DIAG_MDFY_READONLY;
	}

	// 法人番号
	int idx_cnt = 0;
	for ( idx_cnt = KANI01EX2_NUM_HOJIN1; idx_cnt <= KANI01EX2_NUM_HOJINHYPHEN3; idx_cnt++ ) {
		m_Kani01Diag01.ModifyItem ( idx_cnt, nHojinDispModify );
	}

	// 個人番号
	for ( idx_cnt = KANI01EX2_NUM_KOJIN1; idx_cnt <= KANI01EX2_NUM_KOJINHYPHEN2; idx_cnt++ ) {
		m_Kani01Diag01.ModifyItem ( idx_cnt, nKojinDispModify );
	}

	m_ctrl_mynum.Init ( this, kubun, ( *m_pSnHeadData )->m_apno, TRUE, edit, m_Kani01Diag01.GetSafeHwnd(), diag_index, MN_NO_PICKUP_DIAGDATA );

	MoveDiagMNEdit();

	m_ctrl_mynum.SetHeightMargin ( MN_HEIGHT_CENTER );
	m_ctrl_mynum.EnableWindow ( FALSE );
	m_ctrl_mynum.ShowWindow ( SW_SHOW );

	// 銀行名
	m_Kani01Diag03.GetAttribute ( ID4210, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag03.SetAttribute ( ID4210, ( LPUNKNOWN )&DA, FALSE);

	// 銀行支店名
	m_Kani01Diag03.GetAttribute ( ID4211, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag03.SetAttribute ( ID4211, ( LPUNKNOWN )&DA, FALSE);

	// 郵便局名
	m_Kani01Diag03.GetAttribute ( ID4312, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Kani01Diag03.SetAttribute ( ID4312, ( LPUNKNOWN )&DA, FALSE);

	// DELETEキー1回で削除モード設定
	m_Kani01Diag01.EnableDelete ( 0 );
	m_Kani01Diag02.EnableDelete ( 0 );
	m_Kani01Diag03.EnableDelete ( 0 );

	return 0;
}

//=====================================
// ダイアグラム初期設定
//=====================================
void CShinKani01EX2::ChgDiagAttr()
{
	int cnt = 0;
	while ( 1 ) {
		if ( Kani1[cnt].Index == 0 ) {
			break;
		}
		int Flg = 0;

		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Kani1[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Kani1[cnt].UnConnect;
			}
			else {
				Flg = Kani1[cnt].Connect;
			}
			//----> 2023.5
			if (Kani1[cnt].Index == ID2062) {
				if (!(*m_pSnHeadData)->IsSoleProprietor()) {
					Flg = 0;
				}
			}
			//<---
		}
		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Kani1[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Kani1[cnt].Connect );

		if ( ( Kani1[cnt].Index == ID2050 ) || ( Kani1[cnt].Index == ID2060 ) ) {
			m_Kani01Diag01.ModifyItem ( Kani1[cnt].Index, ( Flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY ) | DIAG_MODIFY_IMEKANA );
		}

		cnt++;
	}

	cnt = 0;
	while ( 1 ) {
		if ( Kani2[cnt].Index == 0 ) {
			break;
		}

		int Flg = 0;
		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Kani2[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Kani2[cnt].UnConnect;
			}
			else {
				Flg = Kani2[cnt].Connect;
			}

			if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN  &0xff ) % 2 ) ) {
				if ( ( Kani2[cnt].Index == ID3XM10 ) || ( Kani2[cnt].Index == ID3XM21 ) ) {
					Flg = 0;
				}
			}

			if ( ( Kani2[cnt].Index == ID3XM13 ) || ( Kani2[cnt].Index == ID3XM24 ) ) {
				if ( Flg == 1 ) {
					// 修正確定・修正中間
					if ( ( ( *m_pSnHeadData )->Sn_SKKBN == 0x03 ) || ( ( *m_pSnHeadData )->Sn_SKKBN == 0x04 ) ) {
						;
					}
					else {
						Flg = 0;
					}
				}
				else {
					Flg = 0;
				}
			}

			// 2割特例の場合はこの課税期間の課税売上高は記載不要のため、入力も閉じる
			if( Kani2[cnt].Index == ID3XM15 ){
				if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 ){
					Flg = 0;
				}
			}
		}

		int flg_ex = 0;
		if ( Flg == 2 ) {
			if ( ( *m_pSnHeadData )->SVmzsw == 0 ) {
				// 経過措置なし
				flg_ex = TRUE;
			}
			else {
				// 経過措置あり
				flg_ex = FALSE;
			}
		}
		else {
			if ( Flg ) {
				flg_ex = TRUE;
			}
			else {
				flg_ex = FALSE;
			}
		}
		
		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Kani2[cnt].Index, flg_ex, ( *m_pSnHeadData )->Sn_Sign4, Kani2[cnt].Connect );
		cnt++;
	}

	cnt = 0;
	while ( 1 ) {
		if ( Kani3[cnt].Index == 0 ) {
			break;
		}

		int Flg = 0;
		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Kani3[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Kani3[cnt].UnConnect;
			}
			else {
				Flg = Kani3[cnt].Connect;
			}

			//----> 2023.5
			if (Kani3[cnt].Index == ID4420) {
				if (IsJunkakutei() || !(*m_pSnHeadData)->IsSoleProprietor()) {
					Flg = 0;
				}
			}
			//<----

	
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL3, Kani3[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Kani3[cnt].Connect );


		cnt++;
	}

	if ( ( *m_pSnHeadData )->Sn_Sign4&0x01 ) {
		GetDlgItem ( IDC_BUTTON1 )->EnableWindow ( FALSE );
	}
	else {
		GetDlgItem ( IDC_BUTTON1 )->EnableWindow ( TRUE );
	}
}

//=====================================
// 初期データ表示
//=====================================
void CShinKani01EX2::DispKani()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[512] = { 0 };
	char cbuf[128] = { 0 };

	// 課税期間の取得
	long symd = 0;
	if (!((*m_pSnHeadData)->Sn_SKKBN % 2)) {
		symd = (*m_pSnHeadData)->Sn_MDAYS;
	}
	else {
		symd = (*m_pSnHeadData)->Sn_KDAYS;
	}

	// 作成年月日
	char yy = 0, mm = 0, dd = 0;
	//----->[23-0203]前回：予定(26号)から切替えた直後のみ、提出日をクリアする('24.07.23 cor)
	//( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_PRESENT_DATE, &yy, &mm, &dd );
	//------
	if( !( ( CH31HyoView* )m_pParent )->GetPastYoteiSgn() ) {
		(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yy, &mm, &dd);
	}
	else {
		char tmpData = 0;
		(*m_pSnHeadData)->SetYmdDataGen(ID_ICSSH_PRESENT_DATE, 0, tmpData);
		(*m_pSnHeadData)->SetYmdDataGen(ID_ICSSH_PRESENT_DATE, 1, tmpData);
		(*m_pSnHeadData)->SetYmdDataGen(ID_ICSSH_PRESENT_DATE, 2, tmpData);
		DiagData.data_day[0] = 0x00;
		m_Kani01Diag01.SetData(ID2000, (struct IUnknown*)&DiagData);
		m_Kani01Diag01.SetData(ID2001, (struct IUnknown*)&DiagData);
		m_Kani01Diag01.SetData(ID2002, (struct IUnknown*)&DiagData);
		( ( CH31HyoView* )m_pParent )->ChgPastYoteiSgn( FALSE );
	}
	//<-----
	if ( ( yy & 0xff ) ) {
		DiagData.data_day[0] = yy;
		m_Kani01Diag01.SetData ( ID2000, ( struct IUnknown* )&DiagData );
	}
	if ( ( mm & 0xff ) ) {
		DiagData.data_day[0] = mm;
		m_Kani01Diag01.SetData ( ID2001, ( struct IUnknown* )&DiagData );
	}
	if ( ( dd & 0xff ) ) {
		DiagData.data_day[0] = dd;
		m_Kani01Diag01.SetData ( ID2002, ( struct IUnknown* )&DiagData );
	}

	// 税務署名
	memset ( buf, '\0', sizeof ( buf ) );
	int len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_ZMSYO, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 0x00 ) && ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 'F' ) ) {
		sprintf_s ( buf, sizeof ( buf ), _T( "署番号 %-5.5s" ), ( *m_pSnHeadData )->Sn_TAXNO );
	}
	else {
		sprintf_s ( buf, sizeof ( buf ), _T( "署番号未登録") );
	}
	DiagData.data_disp = buf;
	m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );

	// 納税地 郵便番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2020, ( struct IUnknown* )&DiagData );

	// 納税地 住所(上)
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_NOZEI, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2030, ( struct IUnknown* )&DiagData );

	// 納税地 住所(下)
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )&( *m_pSnHeadData )->Sn_NOZEI[40], 40 );
	memmove ( buf, &( *m_pSnHeadData )->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2031, ( struct IUnknown* )&DiagData );

	// 納税地 電話番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO1, 6 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2040, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO2, 4 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2041, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO3, 4 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2042, ( struct IUnknown* )&DiagData );

	// 氏名・屋号 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_COFRI, 80 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2050, ( struct IUnknown* )&DiagData );

	// 氏名・屋号
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_CONAM, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2051, ( struct IUnknown* )&DiagData );

	// 被合併名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_GPNAM, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2052, ( struct IUnknown* )&DiagData );

	// 代表者氏名 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIFRI, 68 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2060, ( struct IUnknown* )&DiagData );

	// 代表者氏名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIHYO, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2061, ( struct IUnknown* )&DiagData );

	// マイナンバー
	int ret = 0;
	if ( ( ( CH31HyoView* )m_pParent )->GetShRevType() >= ID_ICSSH_MYNUMBER_REVTYPE ) {
		CString str_myno;
		( *m_pSnHeadData )->GetMyNumber ( str_myno );
		ret = m_ctrl_mynum.SetData ( str_myno );
	}

	// （個人の方）振替継続希望
//----->('23.05.01 Add)
	if (!(*m_pSnHeadData)->Sn_KOJIN) {
		(*m_pSnHeadData)->Sn_GenericSgn &= ~0x200;
	}
//<-----
	DiagData.data_check = ( short )(( *m_pSnHeadData )->Sn_GenericSgn & 0x200 );
	m_Kani01Diag01.SetData ( ID2062, ( struct IUnknown* )&DiagData );

	// 所管
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_SYOKN, 3 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2081, ( struct IUnknown* )&DiagData );

	// 整理番号
	if ( ( ( *m_pSnHeadData )->Sn_SEIRI[0] & 0xff ) != 0xff ) {
		memset ( buf, '\0', sizeof ( buf ) );
		m_pArith->l_unpac ( buf, ( *m_pSnHeadData )->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_Kani01Diag01.SetData ( ID2082, ( struct IUnknown* )&DiagData );
	}

	// 課税期間
	yy = mm = dd = 0;
	int gengo = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd , &gengo);
	if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2100, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2101, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2102, ( struct IUnknown* )&DiagData );
	}

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2110, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2111, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2112, ( struct IUnknown* )&DiagData );
	}

	// 申告区分
	memset ( buf, '\0', sizeof ( buf ) );
	switch ( ( *m_pSnHeadData )->Sn_SKKBN ) {
		case 1:
			if ( ( *m_pSnHeadData )->Sn_Syuusei & 0x10 ) {
				memmove ( buf, _T( " 準確定 " ), 8 );
			}
			else {
				memmove	( buf, _T( "　確定　" ), 8 );
			}
			break;
		case 2:
			memmove ( buf, _T( "　中間　" ), 8 );
			break;
		case 3 :
			memmove ( buf, _T( "修正確定" ), 8 );
			break;
		default:
			memmove ( buf, _T( "修正中間" ), 8 );
			break;
	}
	DiagData.data_disp = buf;
	m_Kani01Diag01.SetData ( ID2120, ( struct IUnknown* )&DiagData );

	// 中間期間
	if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) ) {
		yy = mm = dd = 0;
		gengo = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd , &gengo);
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2130, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2131, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2132, ( struct IUnknown* )&DiagData );
		}
		yy = mm = dd = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd );
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2140, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2141, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2142, ( struct IUnknown* )&DiagData );
		}
	}

	// 金額欄表示
	Disp_Kanidata ( 0 );
	Disp_FhyoData();

	// 付記・参考
	for ( int i = ID4000; i <= ID4ZI2; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL3, ( short )i );
	}

	// 割賦基準の適用
	SW_WARIFU = ( int )( ( *m_pSnHeadData )->Sn_KAPPU & 0xff );
	if ( SW_WARIFU ) {
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4000, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4001, ( struct IUnknown* )&DiagData );
	}
	else {
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4000, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4001, ( struct IUnknown* )&DiagData );
	}

	// 延払基準の適用
	SW_NOBEBA = ( int )( ( *m_pSnHeadData )->Sn_NOBE & 0xff );
	if ( SW_NOBEBA ) {
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4010, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4011, ( struct IUnknown* )&DiagData );
	}
	else {
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4010, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4011, ( struct IUnknown* )&DiagData );
	}

	// 工事進行基準の適用
	SW_KOUJIS = ( int )( ( *m_pSnHeadData )->Sn_KOJI & 0xff );
	if ( SW_KOUJIS ) {
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4020, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4021, ( struct IUnknown* )&DiagData );
	}
	else {
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4020, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4021, ( struct IUnknown* )&DiagData );
	}

	// 現金主義会計の適用
	SW_GENKIN = ( int )( ( *m_pSnHeadData )->Sn_GENKN & 0xff );
	if ( SW_GENKIN ) {
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4030, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4031, ( struct IUnknown* )&DiagData );
	}
	else {
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData ( ID4030, ( struct IUnknown* )&DiagData );
		DiagData.data_check = 1;
		m_Kani01Diag03.SetData ( ID4031, ( struct IUnknown* )&DiagData );
	}

	// 規則22条1項の適用
	if (symd >= ICS_SH_INVOICE_DAY) {	//課税期間が令和5年10月1日以後開始の場合はチェックを付けない様にする
		DiagData.data_check = 0;
		m_Kani01Diag03.SetData(ID4040, (struct IUnknown*)&DiagData);
		m_Kani01Diag03.SetData(ID4041, (struct IUnknown*)&DiagData);
	}
	else {
		SW_KISK22 = (int)((*m_pSnHeadData)->Sn_TUMIAG & 0xff);
		if (SW_KISK22) {
			DiagData.data_check = 1;
			m_Kani01Diag03.SetData ( ID4040, ( struct IUnknown* )&DiagData );
			DiagData.data_check = 0;
			m_Kani01Diag03.SetData ( ID4041, ( struct IUnknown* )&DiagData );
		}
		else {
			DiagData.data_check = 0;
			m_Kani01Diag03.SetData ( ID4040, ( struct IUnknown* )&DiagData );
			DiagData.data_check = 1;
			m_Kani01Diag03.SetData ( ID4041, ( struct IUnknown* )&DiagData );
		}
	}

//----->('23.05.01 Add)		(メモ)ID番号の調整はリソース完成まで保留。
	// 税額控除に係る経過措置の適用（２割特例）
	DiagData.data_check = (short)((*m_pSnHeadData)->m_DispTabSgn & 0x2000);
	m_Kani01Diag03.SetData(ID2WARI, (struct IUnknown*)&DiagData);
//<-----

	// 金融機関 名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_BANK, 20 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_BANK, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4210, ( struct IUnknown* )&DiagData );

	// 銀行・金庫等
	DiagData.data_combo = ( short )( ( *m_pSnHeadData )->Sn_FINANCE & 0x0f );
	m_Kani01Diag03.SetData ( ID4212, ( struct IUnknown* )&DiagData );

	// 金融機関 支店名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_SITEN, 20 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_SITEN, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4211, ( struct IUnknown* )&DiagData );

	// 本社・支店等
	DiagData.data_combo = ( short )( ( *m_pSnHeadData )->Sn_CHAIN & 0x0f );
	m_Kani01Diag03.SetData ( ID4213, ( struct IUnknown* )&DiagData );

	// 金融機関 預金種別
	memset ( buf, '\0', sizeof ( buf ) );
	len = kjlen ( ( *m_pSnHeadData )->Sn_YOKIN, 8 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YOKIN, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4214, ( struct IUnknown* )&DiagData );

	// 金融機関 口座番号
	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( ( *m_pSnHeadData )->Sn_KOZA[0] & 0xff ) != 0xff ) {
		memmove ( cbuf, ( *m_pSnHeadData )->Sn_KOZA, 5 );
		m_pArith->l_unpac ( buf, cbuf, 10 );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4215, ( struct IUnknown* )&DiagData );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4215 );
	}

	// 郵便局 名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_YUBIN, 16 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YUBIN, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4312, ( struct IUnknown* )&DiagData );

	// 銀行or郵便局
	if ( ( *m_pSnHeadData )->Sn_BYsw ) {
		DiagData.data_combo = 1;
	}
	else {
		DiagData.data_combo = 0;
	}
	m_Kani01Diag03.SetData ( ID4410, ( struct IUnknown* )&DiagData );

	// 郵便局 預金番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YUNUM1, 5 );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4310, ( struct IUnknown* )&DiagData );

	// 郵便局 預金番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YUNUM2, 8 );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4311, ( struct IUnknown* )&DiagData );


	// （個人の方）公金受取口座の利用
//----->('23.06.28 cor)
//	if( IsJunkakutei() ){	// 2022/12/27 準確定の場合はチェックを外して入力不可
//------
	if (IsJunkakutei() || (!(*m_pSnHeadData)->IsSoleProprietor())) {	// 2022/12/27 準確定の場合はチェックを外して入力不可
//<-----
		( *m_pSnHeadData )->Sn_GenericSgn &= ~0x400;
	}
	DiagData.data_check = ( short )(( *m_pSnHeadData )->Sn_GenericSgn & 0x400 );
	m_Kani01Diag03.SetData ( ID4420, ( struct IUnknown* )&DiagData );

	// 税務署整理欄
	memset ( cbuf, '\0', sizeof ( cbuf ) );
	if ( ( ( *m_pSnHeadData )->Sn_ZSEIRI[0] & 0xff ) != 0xff ) {
		if ( ( ( *m_pSnHeadData )->Sn_ZSEIRI[3] & 0x0f ) != 0x0f ) {	// 8桁入力済
			m_pArith->l_unpac ( cbuf, ( *m_pSnHeadData )->Sn_ZSEIRI, 8 );
			if ( cbuf[0] != 0x30 ) {
				cbuf[7] = '\0';
			}
			else {
				int itct = 0;
				for ( itct = 0; itct < 7; ++itct ) {
					cbuf[itct] = cbuf[itct + 1];
				}
				cbuf[itct] = '\0';
			}
			memset ( ( *m_pSnHeadData )->Sn_ZSEIRI, '\0', 4 );
			m_pArith->l_pack ( ( *m_pSnHeadData )->Sn_ZSEIRI, cbuf, 7 );
		}
		memset ( cbuf, '\0', sizeof ( cbuf ) );
		m_pArith->l_unpac ( cbuf, ( *m_pSnHeadData )->Sn_ZSEIRI, 8 );
		DiagData.data_edit = ( char* )cbuf;
		m_Kani01Diag03.SetData ( ID4RNO, ( struct IUnknown* )&DiagData );
	}

	// 税理士情報
	if ( m_pShinInfo->isExistOffice ) {
		int i = 0, j = 0, k = 0;
		// 事務所名又は法人名
		memset ( buf, '\0', sizeof ( buf ) );
		memcpy ( buf, m_pShinInfo->pO_DATA->o_ofcnam, sizeof ( m_pShinInfo->pO_DATA->o_ofcnam ) );
		DiagData.data_disp = buf;
		m_Kani01Diag03.SetData ( ID4ZI0, ( struct IUnknown* )&DiagData );

		// 担当税理士氏名
		memset ( buf, '\0', sizeof ( buf ) );
		if ( ( i = ( int )strlen ( m_pShinInfo->pZ_DATA->z_zeinam ) ) > 40 ) {
			i = 40;
		}
		memcpy ( buf, m_pShinInfo->pZ_DATA->z_zeinam, i );
		DiagData.data_disp = buf;
		m_Kani01Diag03.SetData ( ID4ZI1, ( struct IUnknown* )&DiagData );

		// 電話番号
		memset ( buf, '\0', sizeof ( buf ) );
		for ( j = 0; j < 18; ++j ) {
			buf[j] = 0x20;
		}
		for ( j = k = 0; ( j < 30 ) || ( m_pShinInfo->pO_DATA->o_telno[j] ); ++j ) {
			if ( ( m_pShinInfo->pO_DATA->o_telno[j] == '-' ) || ( m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ' ) ) {
				switch ( k ) {
					case 0:
						k = ( ( j < 6 ) ? j : 6 );
						memmove ( &buf[6 - k], &m_pShinInfo->pO_DATA->o_telno[0], k );
						k = 1;
						i = j + 1;
						continue;
					case 1:
						if ( ( j - i ) < 5 ) {
							k = ( j - i );
						}
						else {
							k = 5;
						}
						memmove ( &buf[8], &m_pShinInfo->pO_DATA->o_telno[i], k );
						memmove ( &buf[13], &m_pShinInfo->pO_DATA->o_telno[++j], 5 );
						k = 2;
						break;
					default:
						break;
				}
				if ( k > 1 ) {
					break;
				}
			}
		}
		if ( !k ) {
			memmove ( buf, &m_pShinInfo->pO_DATA->o_telno[0], ( j > 18 ? 18 : j ) );
		}
		else {
			if ( k == 1 ) {
				memmove ( &buf[8], &m_pShinInfo->pO_DATA->o_telno[i], 4 );
			}
		}
		DiagData.data_disp = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4ZI2, ( struct IUnknown* )&DiagData );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4ZI0 );
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4ZI1 );
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4ZI2 );
	}

	// 税理士法第30条
	DiagData.data_check = ( short )( ( *m_pSnHeadData )->Sn_ZeiHou & 0x01 );
	m_Kani01Diag03.SetData ( ID4ZI3, ( struct IUnknown* )&DiagData );

	// 税理士法第条33条の2
	DiagData.data_check = ( short )( ( *m_pSnHeadData )->Sn_ZeiHou & 0x02 );
	m_Kani01Diag03.SetData ( ID4ZI4, ( struct IUnknown* )&DiagData );
}

//===========================================
// 初期データ表示
//===========================================
void CShinKani01EX2::DispInitKaniUpper()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[512] = { 0 };
	char cbuf[128] = { 0 };

	// 作成年月日
	char yy = 0, mm = 0, dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_PRESENT_DATE, &yy, &mm, &dd );
	if ( ( yy & 0xff ) ) {
		DiagData.data_day[0] = yy;
		m_Kani01Diag01.SetData ( ID2000, ( struct IUnknown* )&DiagData );
	}
	if ( ( mm & 0xff ) ) {
		DiagData.data_day[0] = mm;
		m_Kani01Diag01.SetData ( ID2001, ( struct IUnknown* )&DiagData );
	}
	if ( ( dd & 0xff ) ) {
		DiagData.data_day[0] = dd;
		m_Kani01Diag01.SetData ( ID2002, ( struct IUnknown* )&DiagData );
	}

	// 税務署名
	memset ( buf, '\0', sizeof ( buf ) );
	int len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_ZMSYO, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 0x00 ) && ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 'F' ) ) {
		sprintf_s ( buf, sizeof ( buf ), _T( "署番号 %-5.5s" ), ( *m_pSnHeadData )->Sn_TAXNO );
	}
	else {
		sprintf_s ( buf, sizeof ( buf ), _T( "署番号未登録") );
	}
	DiagData.data_disp = buf;
	m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );

	// 納税地 郵便番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2020, ( struct IUnknown* )&DiagData );

	// 納税地 住所(上)
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_NOZEI, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2030, ( struct IUnknown* )&DiagData );

	// 納税地 住所(下)
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )&( *m_pSnHeadData )->Sn_NOZEI[40], 40 );
	memmove ( buf, &( *m_pSnHeadData )->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2031, ( struct IUnknown* )&DiagData );

	// 納税地 電話番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO1, 6 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2040, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO2, 4 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2041, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO3, 4 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2042, ( struct IUnknown* )&DiagData );

	// 氏名・屋号 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_COFRI, 80 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2050, ( struct IUnknown* )&DiagData );

	// 氏名・屋号
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_CONAM, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2051, ( struct IUnknown* )&DiagData );

	// 被合併名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_GPNAM, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2052, ( struct IUnknown* )&DiagData );

	// 代表者氏名 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIFRI, 68 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2060, ( struct IUnknown* )&DiagData );

	// 代表者氏名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIHYO, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2061, ( struct IUnknown* )&DiagData );

	// マイナンバー
	int ret = 0;
	CString str_myno;
	( *m_pSnHeadData )->GetMyNumber ( str_myno );
	ret = m_ctrl_mynum.SetData ( str_myno );

	// （個人の方）振替継続希望
//----->('23.05.01 Add)
	if (!(*m_pSnHeadData)->Sn_KOJIN) {
		(*m_pSnHeadData)->Sn_GenericSgn &= ~0x200;
	}
//<-----
	DiagData.data_check = ( short )(( *m_pSnHeadData )->Sn_GenericSgn & 0x200 );
	m_Kani01Diag01.SetData ( ID2062, ( struct IUnknown* )&DiagData );

	// 所管
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_SYOKN, 3 );
	DiagData.data_edit = buf;
	m_Kani01Diag01.SetData ( ID2081, ( struct IUnknown* )&DiagData );

	// 整理番号
	if ( ( ( *m_pSnHeadData )->Sn_SEIRI[0] & 0xff ) != 0xff ) {
		memset ( buf, '\0', sizeof ( buf ) );
		m_pArith->l_unpac ( buf, ( *m_pSnHeadData )->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_Kani01Diag01.SetData ( ID2082, ( struct IUnknown* )&DiagData );
	}

	// 課税期間
	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2100, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2101, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2102, ( struct IUnknown* )&DiagData );
	}

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2110, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2111, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Kani01Diag01.SetData ( ID2112, ( struct IUnknown* )&DiagData );
	}

	// 申告区分
	memset ( buf, '\0', sizeof ( buf ) );
	switch ( ( *m_pSnHeadData )->Sn_SKKBN ) {
	case 1:
		if ( ( *m_pSnHeadData )->Sn_Syuusei & 0x10 ) {
			memmove ( buf, _T( " 準確定 " ), 8 );
		}
		else {
			memmove	( buf, _T( "　確定　" ), 8 );
		}
		break;
	case 2:
		memmove ( buf, _T( "　中間　" ), 8 );
		break;
	case 3 :
		memmove ( buf, _T( "修正確定" ), 8 );
		break;
	default:
		memmove ( buf, _T( "修正中間" ), 8 );
		break;
	}
	DiagData.data_disp = buf;
	m_Kani01Diag01.SetData ( ID2120, ( struct IUnknown* )&DiagData );

	// 中間期間
	if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) ) {
		yy = mm = dd = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd );
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2130, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2131, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2132, ( struct IUnknown* )&DiagData );
		}
		yy = mm = dd = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd );
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2140, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2141, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2142, ( struct IUnknown* )&DiagData );
		}
	}

	// 金融機関 名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_BANK, 20 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_BANK, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4210, ( struct IUnknown* )&DiagData );

	// 銀行・金庫等
	DiagData.data_combo = ( short )( ( *m_pSnHeadData )->Sn_FINANCE & 0x0f );
	m_Kani01Diag03.SetData ( ID4212, ( struct IUnknown* )&DiagData );

	// 金融機関 支店名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_SITEN, 20 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_SITEN, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4211, ( struct IUnknown* )&DiagData );

	// 本社・支店等
	DiagData.data_combo = ( short )( ( *m_pSnHeadData )->Sn_CHAIN & 0x0f );
	m_Kani01Diag03.SetData ( ID4213, ( struct IUnknown* )&DiagData );

	// 金融機関 口座番号
	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( ( *m_pSnHeadData )->Sn_KOZA[0] & 0xff ) != 0xff ) {
		memmove ( cbuf, ( *m_pSnHeadData )->Sn_KOZA, 5 );
		l_unpac ( buf, cbuf, 10 );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4215, ( struct IUnknown* )&DiagData );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4215 );
	}

	// 郵便局 名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_YUBIN, 16 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YUBIN, len );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4312, ( struct IUnknown* )&DiagData );

	// 銀行or郵便局
	if ( ( *m_pSnHeadData )->Sn_BYsw ) {
		DiagData.data_combo = 1;
	}
	else {
		DiagData.data_combo = 0;
	}
	m_Kani01Diag03.SetData ( ID4410, ( struct IUnknown* )&DiagData );

	// 郵便局 預金番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YUNUM1, 5 );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4310, ( struct IUnknown* )&DiagData );

	// 郵便局 預金番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_YUNUM2, 8 );
	DiagData.data_edit = ( char* )buf;
	m_Kani01Diag03.SetData ( ID4311, ( struct IUnknown* )&DiagData );

	// （個人の方）公金受取口座の利用
//----->('23.06.28 cor)
//	if( IsJunkakutei() ){	// 2022/12/27 準確定の場合はチェックを外して入力不可
//------
	if (IsJunkakutei() || (!(*m_pSnHeadData)->IsSoleProprietor())) {	// 2022/12/27 準確定の場合はチェックを外して入力不可
//<-----
		( *m_pSnHeadData )->Sn_GenericSgn &= ~0x400;
	}
	DiagData.data_check = ( short )( ( *m_pSnHeadData )->Sn_GenericSgn & 0x400 );
	m_Kani01Diag03.SetData ( ID4420, ( struct IUnknown* )&DiagData );

	// 税務署整理欄
	memset ( cbuf, '\0', sizeof ( cbuf ) );
	if ( ( ( *m_pSnHeadData )->Sn_ZSEIRI[0] & 0xff ) != 0xff ) {
		if ( ( ( *m_pSnHeadData )->Sn_ZSEIRI[3] & 0x0f ) != 0x0f ) {	// 8桁入力済
			m_pArith->l_unpac ( cbuf, ( *m_pSnHeadData )->Sn_ZSEIRI, 8 );
			if ( cbuf[0] != 0x30 ) {
				cbuf[7] = '\0';
			}
			else {
				int itct = 0;
				for ( itct = 0; itct < 7; ++itct ) {
					cbuf[itct] = cbuf[itct + 1];
				}
				cbuf[itct] = '\0';
			}
			memset ( ( *m_pSnHeadData )->Sn_ZSEIRI, '\0', 4 );
			m_pArith->l_pack ( ( *m_pSnHeadData )->Sn_ZSEIRI, cbuf, 7 );
		}
		memset ( cbuf, '\0', sizeof ( cbuf ) );
		m_pArith->l_unpac ( cbuf, ( *m_pSnHeadData )->Sn_ZSEIRI, 8 );
		DiagData.data_edit = ( char* )cbuf;
		m_Kani01Diag03.SetData ( ID4RNO, ( struct IUnknown* )&DiagData );
	}

	// 税理士情報
	if ( m_pShinInfo->isExistOffice ) {
		int i = 0, j = 0, k = 0;
		// 事務所名又は法人名
		memset ( buf, '\0', sizeof ( buf ) );
		memcpy ( buf, m_pShinInfo->pO_DATA->o_ofcnam, sizeof ( m_pShinInfo->pO_DATA->o_ofcnam ) );
		DiagData.data_disp = buf;
		m_Kani01Diag03.SetData ( ID4ZI0, ( struct IUnknown* )&DiagData );

		// 担当税理士氏名
		memset ( buf, '\0', sizeof ( buf ) );
		if ( ( i = ( int )strlen ( m_pShinInfo->pZ_DATA->z_zeinam ) ) > 40 ) {
			i = 40;
		}
		memcpy ( buf, m_pShinInfo->pZ_DATA->z_zeinam, i );
		DiagData.data_disp = buf;
		m_Kani01Diag03.SetData ( ID4ZI1, ( struct IUnknown* )&DiagData );

		// 電話番号
		memset ( buf, '\0', sizeof ( buf ) );
		for ( j = 0; j < 18; ++j ) {
			buf[j] = 0x20;
		}
		for ( j = k = 0; j < 30 || m_pShinInfo->pO_DATA->o_telno[j]; ++j ) {
			if ( ( m_pShinInfo->pO_DATA->o_telno[j] == '-' ) || ( m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ' ) ) {
				switch ( k ) {
				case 0:
					k = ( ( j < 6 ) ? j : 6 );
					memmove ( &buf[6 - k], &m_pShinInfo->pO_DATA->o_telno[0], k );
					k = 1;
					i = j + 1;
					continue;
				case 1:
					if ( ( j - i ) < 5 ) {
						k = ( j - i );
					}
					else {
						k = 5;
					}
					memmove ( &buf[8], &m_pShinInfo->pO_DATA->o_telno[i], k );
					memmove ( &buf[13], &m_pShinInfo->pO_DATA->o_telno[++j], 5 );
					k = 2;
					break;
				default:
					break;
				}
				if ( k > 1 ) {
					break;
				}
			}
		}
		if ( !k ) {
			memmove ( buf, &m_pShinInfo->pO_DATA->o_telno[0], ( j > 18 ? 18 : j ) );
		}
		else {
			if ( k == 1 ) {
				memmove ( &buf[8], &m_pShinInfo->pO_DATA->o_telno[i], 4 );
			}
		}
		DiagData.data_disp = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4ZI2, ( struct IUnknown* )&DiagData );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4ZI0 );
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4ZI1 );
		dsp_cls ( IDC_ICSDIAG8CTRL3, ID4ZI2 );
	}
}

//=======================================================
// 金額表示
//-------------------------------------------------------
// 引数
//		Id			:		ダイアグラムID
//		Index		:		ダイアグラム内インデックス
//		Mony		:		表示金額
//=======================================================
void CShinKani01EX2::dsp_prs ( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( Id == IDC_ICSDIAG8CTRL2 ) {
		switch ( Index ) {
			case ID3XM2:	// 消費税額
				memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
				m_Kani01Diag02.SetData ( Index, ( struct IUnknown* )&DiagData );
				break;
			default:
				if( Index == ID3XM15 ) {
					if((( CH31HyoView* )m_pParent )->IsNiwariTokurei() ) {
						// 2割特例の場合はこの課税期間の課税売上高は記載不要のためセットしない
						break;
					}
				}
				memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
				m_Kani01Diag02.SetData ( Index, ( struct IUnknown* )&DiagData );
				break;
		}
	}
	else if ( Id == IDC_ICSDIAG8CTRL3 ) {
		switch ( Index ) {
			case ID4100:
			case ID4110:
			case ID4120:
			case ID4130:
			case ID4140:
			case ID4150:
				memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
				m_Kani01Diag03.SetData ( Index, ( struct IUnknown* )&DiagData );break;
			default:
				break;
		}
	}
}

//=======================================================
// 金額表示クリア
//-------------------------------------------------------
// 引数
//		Id			:		ダイアグラムID
//		Index		:		ダイアグラム内インデックス
//=======================================================
void CShinKani01EX2::dsp_cls ( unsigned short Id, short Index )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( Id == IDC_ICSDIAG8CTRL1 ) {	// 基本情報
		char tmpData = 0;

		switch ( Index ) {
			case ID2000:	// 提出年
				( *m_pSnHeadData )->SetYmdDataGen ( ID_ICSSH_PRESENT_DATE, 0, tmpData );
				DiagData.data_day[0] = 0x00;
				m_Kani01Diag01.SetData ( ID2000, ( struct IUnknown* )&DiagData );
				break;
			case ID2001:	// 提出月
				( *m_pSnHeadData )->SetYmdDataGen ( ID_ICSSH_PRESENT_DATE, 1, tmpData );
				DiagData.data_day[0] = 0x00;
				m_Kani01Diag01.SetData ( ID2001, ( struct IUnknown* )&DiagData );
				break;
			case ID2002:	// 提出日
				( *m_pSnHeadData )->SetYmdDataGen ( ID_ICSSH_PRESENT_DATE, 2, tmpData );
				DiagData.data_day[0] = 0x00;
				m_Kani01Diag01.SetData ( ID2002, ( struct IUnknown* )&DiagData );
				break;
			case ID2010:	// 税務署名
			case ID2020:	// 郵便番号
			case ID2030:	// 納税地 上段
			case ID2031:	// 納税地 下段
			case ID2040:	// 電話番号 市外
			case ID2041:	// 電話番号 市内
			case ID2042:	// 電話番号 局番
			case ID2050:	// 名称又は屋号 フリガナ
			case ID2051:	// 名称又は屋号
			case ID2052:	// 被合併名
			case ID2060:	// 代表者又は氏名 フリガナ
			case ID2061:	// 代表者又は氏名
				m_Kani01Diag01.DataClear ( Index, TRUE );
				break;
			case ID2062:	// （個人の方）振替継続希望
				// チェックボックスのため、動作なし
				break;
			case ID2081:	// 所管
				memset ( ( *m_pSnHeadData )->Sn_SYOKN, '\0', 3 );
				m_Kani01Diag01.DataClear ( Index, TRUE );
				break;
			case ID2082:	// 整理番号
				memset ( ( *m_pSnHeadData )->Sn_SEIRI, '\0', sizeof ( ( *m_pSnHeadData )->Sn_SEIRI ) );
				( *m_pSnHeadData )->Sn_SEIRI[0] = 0xff;
				m_Kani01Diag01.DataClear ( Index, TRUE );
				break;
			default:
				m_Kani01Diag01.DataClear ( Index, TRUE );
				break;
		}
	}
	else if ( Id == IDC_ICSDIAG8CTRL2 ) {	// 税額計算
		switch ( Index ) {
			case ID3XM1:	// 1.  課税標準額
			case ID3XM2:	// 2.  消費税額
			case ID3XM3:	// 3.  貸倒回収に係る消費税額
			case ID3XM4:	// 4.  控除対象仕入税額
			case ID3XM5:	// 5.  返還等対価に係る税額
			case ID3XM6:	// 6.  貸倒れに係る税額
			case ID3XM7:	// 7.  控除税額小計
			case ID3XM8:	// 8.  控除不足還付税額
			case ID3XM9:	// 9.  差引税額
			case ID3XM10:	// 10. 中間納付税額
			case ID3XM11:	// 11. 納付税額
			case ID3XM12:	// 12. 中間納付還付税額
			case ID3XM13:	// 13. 既確定税額
			case ID3XM14:	// 14. 差引納付税額
			case ID3XM15:	// 15. この課税期間の課税売上高
			case ID3XM16:	// 16. 基準期間の課税売上高
			case ID3XM17:	// 17. 控除不足還付税額
			case ID3XM18:	// 18. 差引税額
			case ID3XM19:	// 19. 還付額
			case ID3XM20:	// 20. 納税額
			case ID3XM21:	// 21. 中間納付譲渡割額
			case ID3XM22:	// 22. 納付譲渡割額
			case ID3XM23:	// 23. 中間納付還付譲渡割額
			case ID3XM24:	// 24. 既確定譲渡割額
			case ID3XM25:	// 25. 差引納付譲渡割額
			case ID3XM26:	// 26. 消費税及び地方消費税の合計(納付又は還付)税額
				memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
				m_Kani01Diag02.SetData ( Index, ( struct IUnknown* )&DiagData );
				break;
			default:
				m_Kani01Diag02.DataClear ( Index, TRUE );
				break;
		}
	}
	else {		// 付記・参考
		switch ( Index ) {
			case ID4000:	// 割賦基準の適用
			case ID4001:	// 割賦基準の適用
			case ID4010:	// 延払基準の適用
			case ID4011:	// 延払基準の適用
			case ID4020:	// 工事進行基準の適用
			case ID4021:	// 工事進行基準の適用
			case ID4030:	// 現金主義会計の適用
			case ID4031:	// 現金主義会計の適用
			case ID4040:	// 課税標準額に対する消費税額の計算の特例適用
			case ID4041:	// 課税標準額に対する消費税額の計算の特例適用
			case ID4160:	// 特例計算適用
			case ID4161:	// 特例計算適用
			case ID4212:	// 金融機関 銀行・金庫等
			case ID4213:	// 金融機関 本社・支社等
			case ID4410:	// 還付金融機関選択
			case ID4420:	// （個人の方）公金受取口座の利用
			case ID4ZI3:	// 税理士法第30条
			case ID4ZI4:	// 税理士法第33条の2
				break;
			default:
				m_Kani01Diag03.DataClear ( Index, TRUE );
		}
	}
}

//=================================
// 一般本表金額表示
//---------------------------------
// 引数
//		atno	:	0 = 全データ
//=================================
void CShinKani01EX2::Disp_Kanidata ( int atno )
{
	switch ( atno ) {
	case 0:		// 全
	case 1:		// 課税標準額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM1, XM1 );
	case 2:		// 消費税額
		dsp_prs	( IDC_ICSDIAG8CTRL2, ID3XM2, XM2 );
//		Disp_Kanidata2();
	case 3:		// 貸倒回収に係る消費税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM3, XM3 );
	case 4:		// 控除対象仕入税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM4, XM4 );
	case 5:		// 返還等対価に係る税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM5, XM5 );
	case 6:		// 貸倒れに係る税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM6, XM6 );
	case 7:		// 控除税額小計
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM7, XM7 );
	case 8:		// 控除不足還付税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM8, XM8 );
	case 9:		// 差引税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM9, XM9 );
	case 10:	// 中間納付還付税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM10, XM10 );
	case 11:	// 納付税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM11, XM11 );
	case 12:	// 中間納付還付税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM12, XM12 );
	case 13:	// 既確定税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM13, XM13 );
	case 14:	// 差引納付税額
		Disp_Kanidata14();
	case 15:	// この課税期間の課税売上高
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM15, XM15 );
	case 16:	// 基準期間の課税売上高
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM16, XM16 );
	case 17:	// 控除不足還付税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM17, XM17 );
	case 18:	// 差引税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM18, XM18 );
	case 19:	// 還付額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM19, XM19 );
	case 20:	// 納税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM20, XM20 );
	case 21:	// 中間納付譲渡割額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM21, XM21 );
	case 22:	// 納付譲渡割額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM22, XM22 );
	case 23:	// 中間納付還付譲渡割額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM23, XM23 );
	case 24:	// 既確定譲渡割額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM24, XM24 );
	case 25:	// 差引納付譲渡割額
		Disp_Kanidata25();
	case 26:	// 消費税及び地方消費税の合計(納付又は還付)税額
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM26, XM26 );
		break;
	default:
		break;
	}
}

//====================================================
// 差引納付税額
//====================================================
void CShinKani01EX2::Disp_Kanidata14()
{
	switch ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) {
	case 1:		// 確定・中間
	case 2:
		memset ( XM14, '\0', MONY_BUF_SIZE );
		break;
	default:
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM14, XM14 );
		break;
	}
}

//====================================================
// 差引納付譲渡割額
//====================================================
void CShinKani01EX2::Disp_Kanidata25()
{
	switch ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) {
	case 1:		// 確定・中間
	case 2:
		memset ( XM25, '\0', MONY_BUF_SIZE );
		break;
	default:
		dsp_prs ( IDC_ICSDIAG8CTRL2, ID3XM25, XM25 );
		break;
	}
}

//====================================================
// 入力項目色変え
//----------------------------------------------------
// 引数
//		id		:		ダイアグラムId
//		Index	;		ダイアグラム内インデックス
//		sign	:		サイン
//=====================================================
void CShinKani01EX2::ChangeColor ( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr ( id, Index, &DA, CIcsdiagctrl );
	switch ( sign ){
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


//========================
// 還付申告かどうかyoshida190613
//========================
BOOL CShinKani01EX2::IsKanpu()
{
	if (m_pArith->l_test(XM26) < 0) {
		return TRUE;
	}

	return FALSE;
}

//==========================================
// 欄外印字
//==========================================
void CShinKani01EX2::PrePrint()
{
	// 欄外印字
	CPrePrint PPDialog;
	PPDialog.InitInfo ( ( *m_pSnHeadData ) );

	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle ( g_hInstance );
	PPDialog.DoModal();
	AfxSetResourceHandle ( svhInstResource );
}

//==========================================
// 終了時のフォーカス移動
//==========================================
void CShinKani01EX2::MoveFocusInEnd()
{
	// データを確定させるためにフォーカス移動
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( m_Kani01Diag01.GetPosition() == ID2010 ) {
			m_Kani01Diag01.SetPosition ( ID2000 );
		}
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL3 ) {
		if ( m_Kani01Diag03.GetPosition() == ID4410 ) {
			m_Kani01Diag03.SetPosition ( ID4RNO );
		}
	}
}

//=========================================
// マイナンバーコントロール
//=========================================
void CShinKani01EX2::MoveDiagMNEdit()
{
	if ( m_pParent && m_pParent->GetSafeHwnd() ) {
		if ( ( *m_pSnHeadData )->IsSoleProprietor() == TRUE ) {	// 個人
			m_ctrl_mynum.MoveDiagMNEdit ( KANI01EX2_NUM_KOJIN1, FALSE );
		}
		else {
			m_ctrl_mynum.MoveDiagMNEdit ( KANI01EX2_NUM_HOJIN1, FALSE );
		}

		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			m_ctrl_mynum.SetDisableColor ( BC_GRAY );
		}
		else {
			m_ctrl_mynum.SetDisableColor ( BC_WHITE );
		}
	}
}

//=========================================
// マイナンバーコントロール非表示
//=========================================
void CShinKani01EX2::HideMNEdit()
{
	m_edit_mynum1.EnableWindow ( FALSE );
	m_edit_mynum2.EnableWindow ( FALSE );
	m_edit_mynum3.EnableWindow ( FALSE );
	m_edit_mynum4.EnableWindow ( FALSE );
	m_edit_mynum1.ShowWindow ( SW_HIDE );
	m_edit_mynum2.ShowWindow ( SW_HIDE );
	m_edit_mynum3.ShowWindow ( SW_HIDE );
	m_edit_mynum4.ShowWindow ( SW_HIDE );
}

//=========================================
// 最終データ読込み
//=========================================
void CShinKani01EX2::GetDiagData()
{
	if ( m_Kani01Diag02.GetPosition() == ID3XM1 ) {
		m_Kani01Diag01.SetFocus();
	}

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 課税標準額
	m_Kani01Diag02.GetData ( ID3XM1, ( struct IUnknown* )&DiagData );
	memmove ( XM1, DiagData.data_val, MONY_BUF_SIZE );

	// 2. 消費税額
	m_Kani01Diag02.GetData ( ID3XM2, ( struct IUnknown* )&DiagData );
	memmove ( XM2, DiagData.data_val, MONY_BUF_SIZE );

	// 3. 控除過大調整税額
	m_Kani01Diag02.GetData ( ID3XM3, ( struct IUnknown* )&DiagData );
	memmove ( XM3, DiagData.data_val, MONY_BUF_SIZE );

	// 4. 控除対象仕入税額
	m_Kani01Diag02.GetData ( ID3XM4, ( struct IUnknown* )&DiagData );
	memmove ( XM4, DiagData.data_val, MONY_BUF_SIZE );

	// 5. 返還等対価に係る税額
	m_Kani01Diag02.GetData ( ID3XM5, ( struct IUnknown* )&DiagData );
	memmove ( XM5, DiagData.data_val, MONY_BUF_SIZE );

	// 6. 貸倒れに係る税額
	m_Kani01Diag02.GetData ( ID3XM6, ( struct IUnknown* )&DiagData );
	memmove ( XM6, DiagData.data_val, MONY_BUF_SIZE );

	// 7. 控除税額小計
	m_Kani01Diag02.GetData ( ID3XM7, ( struct IUnknown* )&DiagData );
	memmove ( XM7, DiagData.data_val, MONY_BUF_SIZE );

	// 8. 控除不足還付税額
	m_Kani01Diag02.GetData ( ID3XM8, ( struct IUnknown* )&DiagData );
	memmove ( XM8, DiagData.data_val, MONY_BUF_SIZE );

	// 9. 差引税額
	m_Kani01Diag02.GetData ( ID3XM9, ( struct IUnknown* )&DiagData );
	memmove ( XM9, DiagData.data_val, MONY_BUF_SIZE );

	// 10. 中間納付税額
	m_Kani01Diag02.GetData ( ID3XM10, ( struct IUnknown* )&DiagData );
	memmove ( XM10, DiagData.data_val, MONY_BUF_SIZE );

	// 11. 納付税額
	m_Kani01Diag02.GetData ( ID3XM11, ( struct IUnknown* )&DiagData );
	memmove ( XM11, DiagData.data_val, MONY_BUF_SIZE );

	// 12. 中間納付還付税額
	m_Kani01Diag02.GetData ( ID3XM12, ( struct IUnknown* )&DiagData );
	memmove ( XM12, DiagData.data_val, MONY_BUF_SIZE );

	// 13. 既確定税額
	m_Kani01Diag02.GetData ( ID3XM13, ( struct IUnknown* )&DiagData );
	memmove ( XM13, DiagData.data_val, MONY_BUF_SIZE );

	// 14. 差引納付税額
	m_Kani01Diag02.GetData ( ID3XM14, ( struct IUnknown* )&DiagData );
	memmove ( XM14, DiagData.data_val, MONY_BUF_SIZE );

	// 15. 課税売上高
	m_Kani01Diag02.GetData ( ID3XM15, ( struct IUnknown* )&DiagData );
	memmove ( XM15, DiagData.data_val, MONY_BUF_SIZE );

	// 16. 基準期間の課税売上高
	m_Kani01Diag02.GetData ( ID3XM16, ( struct IUnknown* )&DiagData );
	memmove ( XM16, DiagData.data_val, MONY_BUF_SIZE );

	// 17. 控除不足還付税額
	m_Kani01Diag02.GetData ( ID3XM17, ( struct IUnknown* )&DiagData );
	memmove ( XM17, DiagData.data_val, MONY_BUF_SIZE );

	// 18. 差引税額
	m_Kani01Diag02.GetData ( ID3XM18, ( struct IUnknown* )&DiagData );
	memmove ( XM18, DiagData.data_val, MONY_BUF_SIZE );

	// 19. 還付額
	m_Kani01Diag02.GetData ( ID3XM19, ( struct IUnknown* )&DiagData );
	memmove ( XM19, DiagData.data_val, MONY_BUF_SIZE );

	// 20. 納税額
	m_Kani01Diag02.GetData ( ID3XM20, ( struct IUnknown* )&DiagData );
	memmove ( XM20, DiagData.data_val, MONY_BUF_SIZE );

	// 21. 中間納付譲渡割額
	m_Kani01Diag02.GetData ( ID3XM21, ( struct IUnknown* )&DiagData );
	memmove ( XM21, DiagData.data_val, MONY_BUF_SIZE );

	// 22. 納付譲渡割額
	m_Kani01Diag02.GetData ( ID3XM22, ( struct IUnknown* )&DiagData );
	memmove ( XM22, DiagData.data_val, MONY_BUF_SIZE );

	// 23. 中間納付還付譲渡割額
	m_Kani01Diag02.GetData ( ID3XM23, ( struct IUnknown* )&DiagData );
	memmove ( XM23, DiagData.data_val, MONY_BUF_SIZE );

	// 24. 既確定譲渡割額
	m_Kani01Diag02.GetData ( ID3XM24, ( struct IUnknown* )&DiagData );
	memmove ( XM24, DiagData.data_val, MONY_BUF_SIZE );

	// 25. 差引納付譲渡割額
	m_Kani01Diag02.GetData ( ID3XM25, ( struct IUnknown* )&DiagData );
	memmove ( XM25, DiagData.data_val, MONY_BUF_SIZE );

	// 26. 消費税及び地方消費税の合計(納付又は還付)税額
	m_Kani01Diag02.GetData ( ID3XM26, ( struct IUnknown* )&DiagData );
	memmove ( XM26, DiagData.data_val, MONY_BUF_SIZE );
}

//==============================
// データの書込み
//------------------------------
// 返送値
//		0	:	正常終了
//		-1	:	エラー
//==============================
int CShinKani01EX2::WriteData()
{
	// 課税標準額
	memmove ( ( *m_pSnHonpyoData )->Sn_KSTD, XM1, MONY_BUF_SIZE );

	// 消費税額
	memmove ( ( *m_pSnHonpyoData )->Sn_SYTX, XM2, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	memmove ( ( *m_pSnHonpyoData )->Sn_KJADJZ, XM3, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( ( *m_pSnHonpyoData )->Sn_SIREZ, XM4, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( ( *m_pSnHonpyoData )->Sn_HKANZ, XM5, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( ( *m_pSnHonpyoData )->Sn_KSITZ, XM6, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( ( *m_pSnHonpyoData )->Sn_KJZSK, XM7, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( ( *m_pSnHonpyoData )->Sn_KJHKZ, XM8, MONY_BUF_SIZE );

	// 差引税額
	memmove ( ( *m_pSnHonpyoData )->Sn_SHZ, XM9, MONY_BUF_SIZE );

	// 中間納付税額
	memmove ( ( *m_pSnHonpyoData )->Sn_TYNOFZ, XM10, MONY_BUF_SIZE );

	// 納付税額
	memmove ( ( *m_pSnHonpyoData )->Sn_EDNOFZ, XM11, MONY_BUF_SIZE );

	// 中間納付還付税額
	memmove ( ( *m_pSnHonpyoData )->Sn_TYNKNP, XM12, MONY_BUF_SIZE );

	// 既確定税額
	memmove ( ( *m_pSnHonpyoData )->Sn_KAKTIZ, XM13, MONY_BUF_SIZE );

	// 差引納付税額
	memmove ( ( *m_pSnHonpyoData )->Sn_SHN, XM14, MONY_BUF_SIZE );

	// この課税期間の課税売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KZURI, XM15, MONY_BUF_SIZE );

	// 基準期間の課税売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_ZZURI, XM16, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( ( *m_pSnHonpyoData )->Sn_TKJHKZ, XM17, MONY_BUF_SIZE );

	// 差引税額
	memmove ( ( *m_pSnHonpyoData )->Sn_TSHZ, XM18, MONY_BUF_SIZE );

	// 還付額
	memmove ( ( *m_pSnHonpyoData )->Sn_KNP, XM19, MONY_BUF_SIZE );

	// 納税額
	memmove ( ( *m_pSnHonpyoData )->Sn_NOZ, XM20, MONY_BUF_SIZE );

	// 中間納付譲渡割額
	memmove ( ( *m_pSnHonpyoData )->Sn_TTYWAR, XM21, MONY_BUF_SIZE );

	// 納付譲渡割額
	memmove ( ( *m_pSnHonpyoData )->Sn_TEDNOF, XM22, MONY_BUF_SIZE );

	// 中間納付還付譲渡割額
	memmove ( ( *m_pSnHonpyoData )->Sn_TYNKNPJ, XM23, MONY_BUF_SIZE );

	// 既確定譲渡割額
	memmove ( ( *m_pSnHonpyoData )->Sn_TKAKTZ, XM24, MONY_BUF_SIZE );

	// 差引納付譲渡割額
	memmove ( ( *m_pSnHonpyoData )->Sn_SHNJ, XM25, MONY_BUF_SIZE );

	// 消費税及び地方消費税の合計(納付又は還付)税額
	memmove ( ( *m_pSnHonpyoData )->Sn_SYOTSG, XM26, MONY_BUF_SIZE );

	// 第1種事業売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KURI1, KURI1, MONY_BUF_SIZE );

	// 第1種事業売上割合
	( *m_pSnHonpyoData )->Sn_UP1 = KUP1;

	// 第2種事業売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KURI2, KURI2, MONY_BUF_SIZE );
	
	// 第2種事業売上割合
	( *m_pSnHonpyoData )->Sn_UP2 = KUP2;

	// 第3種事業売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KURI3, KURI3, MONY_BUF_SIZE );

	// 第3種事業売上割合
	( *m_pSnHonpyoData )->Sn_UP3 = KUP3;

	// 第4種事業売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KURI4, KURI4, MONY_BUF_SIZE );

	// 第4種事業売上割合
	( *m_pSnHonpyoData )->Sn_UP4 = KUP4;

	// 第5種事業売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KURI5, KURI5, MONY_BUF_SIZE );

	// 第5種事業売上割合
	( *m_pSnHonpyoData )->Sn_UP5 = KUP5;

	// 第6種事業売上高
	memmove ( ( *m_pSnHonpyoData )->Sn_KURI6, KURI6, MONY_BUF_SIZE );

	// 第6種事業売上割合
	( *m_pSnHonpyoData )->Sn_UP6 = KUP6;

	// 付記事項
	( *m_pSnHeadData )->Sn_KAPPU = ( char )SW_WARIFU;
	( *m_pSnHeadData )->Sn_NOBE  = ( char )SW_NOBEBA;
	( *m_pSnHeadData )->Sn_KOJI  = ( char )SW_KOUJIS;
	( *m_pSnHeadData )->Sn_GENKN = ( char )SW_GENKIN;
	( *m_pSnHeadData )->Sn_TUMIAG &= 0xfe;
	if ( SW_KISK22 ) {
		( *m_pSnHeadData )->Sn_TUMIAG |= 0x01;
	}

	return 0;
}

//==============================
// データの読込み
//------------------------------
// 返送値
//		0	:	正常終了
//		-1	:	エラー
//==============================
int CShinKani01EX2::ReadData()
{
	// 入力ロック時は集計を行わないため、全データを取得しておく

	// 課税標準額
	memmove ( XM1, ( *m_pSnHonpyoData )->Sn_KSTD, MONY_BUF_SIZE );

	// 消費税額
	memmove ( XM2, ( *m_pSnHonpyoData )->Sn_SYTX, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	memmove ( XM3, ( *m_pSnHonpyoData )->Sn_KJADJZ, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( XM4, ( *m_pSnHonpyoData )->Sn_SIREZ, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( XM5, ( *m_pSnHonpyoData )->Sn_HKANZ, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( XM6, ( *m_pSnHonpyoData )->Sn_KSITZ, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( XM7, ( *m_pSnHonpyoData )->Sn_KJZSK, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( XM8, ( *m_pSnHonpyoData )->Sn_KJHKZ, MONY_BUF_SIZE );

	// 差引税額
	memmove ( XM9, ( *m_pSnHonpyoData )->Sn_SHZ, MONY_BUF_SIZE );

	// 中間納付税額
	memmove ( XM10, ( *m_pSnHonpyoData )->Sn_TYNOFZ, MONY_BUF_SIZE );

	// 納付税額
	memmove ( XM11, ( *m_pSnHonpyoData )->Sn_EDNOFZ, MONY_BUF_SIZE );

	// 中間納付還付税額
	memmove ( XM12, ( *m_pSnHonpyoData )->Sn_TYNKNP, MONY_BUF_SIZE );

	// 既確定税額
	memmove ( XM13, ( *m_pSnHonpyoData )->Sn_KAKTIZ, MONY_BUF_SIZE );

	// 差引納付税額
	memmove ( XM14, ( *m_pSnHonpyoData )->Sn_SHN, MONY_BUF_SIZE );

	// この課税期間の課税売上高
	memmove ( XM15, ( *m_pSnHonpyoData )->Sn_KZURI, MONY_BUF_SIZE );

	// 基準期間の課税売上高
	memmove ( XM16, ( *m_pSnHonpyoData )->Sn_ZZURI, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( XM17, ( *m_pSnHonpyoData )->Sn_TKJHKZ, MONY_BUF_SIZE );

	// 差引税額
	memmove ( XM18, ( *m_pSnHonpyoData )->Sn_TSHZ, MONY_BUF_SIZE );

	// 還付額
	memmove ( XM19, ( *m_pSnHonpyoData )->Sn_KNP, MONY_BUF_SIZE );

	// 納税額
	memmove ( XM20, ( *m_pSnHonpyoData )->Sn_NOZ, MONY_BUF_SIZE );

	// 中間納付譲渡割額
	memmove ( XM21, ( *m_pSnHonpyoData )->Sn_TTYWAR, MONY_BUF_SIZE );

	// 納付譲渡割額
	memmove ( XM22, ( *m_pSnHonpyoData )->Sn_TEDNOF, MONY_BUF_SIZE );

	// 中間納付還付譲渡割額
	memmove ( XM23, ( *m_pSnHonpyoData )->Sn_TYNKNPJ, MONY_BUF_SIZE );

	// 既確定譲渡割額
	memmove ( XM24, ( *m_pSnHonpyoData )->Sn_TKAKTZ, MONY_BUF_SIZE );

	// 差引納付譲渡割額
	memmove ( XM25, ( *m_pSnHonpyoData )->Sn_SHNJ, MONY_BUF_SIZE );

	// 消費税及び地方消費税の合計(納付又は還付)税額
	memmove ( XM26, ( *m_pSnHonpyoData )->Sn_SYOTSG, MONY_BUF_SIZE );

	return 0;
}

//==============================
// 集計モジュールにセット
//==============================
void CShinKani01EX2::ReadData2()
{
	char mony[MONY_BUF_SIZE] =  { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動の場合

		if ((*m_pSnHeadData)->Sn_SKKBN == 3 || (*m_pSnHeadData)->Sn_SKKBN == 4) {//修正申告の場合
			(*m_pSyzSyukei)->SetShinkokuData(_T("331300"), XM13);
			(*m_pSyzSyukei)->SetShinkokuData(_T("332400"), XM24);
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331000" ), XM10 );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331500" ), XM15 );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331600" ), XM16 );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332100" ), XM21 );
	}
	else {

		if ((*m_pSnHeadData)->Sn_SKKBN == 3 || (*m_pSnHeadData)->Sn_SKKBN == 4) {//修正申告の場合
			(*m_pSyzSyukei)->SetShinkokuData(_T("331300"), XM13);
			(*m_pSyzSyukei)->SetShinkokuData(_T("332400"), XM24);
		}

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331000" ), XM10 );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331600" ), XM16 );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332100" ), XM21 );

		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "331500" ));	// 税額情報の当課税期間の課税売上高に手入力の金額が行ってしまうためリセット
	}

	// 中間申告の場合、還付は集計しない
	if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) ) {
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "330800" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331000" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331200" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331700" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331900" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332100" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332300" ), mony );
	}
	else {
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "330800" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "331200" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "331700" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "331900" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "332300" ) );
	}

	// 確定・中間申告の場合には、差引税額を集計しない
	if ( ( ( *m_pSnHeadData )->Sn_SKKBN == 3 ) || ( ( *m_pSnHeadData )->Sn_SKKBN == 4 ) ) {
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "331400" ) );
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "332500" ) );
	}
	else {
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331400" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332500" ), mony );

		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331300" ), mony );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332400" ), mony );
	}
}

//=============================
// 入力位置のセット
//=============================
void CShinKani01EX2::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( ( index = m_Kani01Diag01.GetPosition() ) != -1 ) {
			m_Kani01Diag01.SetPosition ( index );
			m_Kani01Diag01.SetFocus();
			return;
		}
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Kani01Diag02.GetPosition() ) != -1 ) {
			m_Kani01Diag02.SetPosition ( index );
			m_Kani01Diag02.SetFocus();
			return;
		}
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL3 ) {
		if ( ( index = m_Kani01Diag03.GetPosition() ) != -1 ) {
			m_Kani01Diag03.SetPosition ( index );
			m_Kani01Diag03.SetFocus();
			return;
		}
	}

	m_Kani01Diag02.SetFocus();
	m_Kani01Diag01.SetFocus();

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return;
	}

	m_Kani01Diag01.SetPosition ( ID2000 );

	CRect rc;
	GetClientRect ( &rc );
	InvalidateRect ( &rc );
	SetRedrawScroll ( 0 );
}

//===================================
// 税務署入力にフォーカスをセット
//===================================
void CShinKani01EX2::SetTaxOfficeInput()
{
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return;
	}

	m_Kani01Diag01.SetPosition ( ID2010 );

	CRect rc;
	GetClientRect ( &rc );
	InvalidateRect ( &rc );
	SetRedrawScroll ( 0 );
}

//===================================
// 入力位置を保存
//===================================
void CShinKani01EX2::SaveInputPosition()
{
	m_curInPos = -1;

	int index = 0;
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( ( index = m_Kani01Diag01.GetPosition() ) != -1 ) {
			m_curInPos = index;
		}
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Kani01Diag02.GetPosition() ) != -1 ) {
			m_curInPos = index;
		}
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL3 ) {
		if ( ( index = m_Kani01Diag03.GetPosition() ) != -1 ) {
			m_curInPos = index;
		}
	}

	if ( m_curInPos != -1 ) {
		m_Kani01Diag02.SetPosition ( ID2000 );
	}
}

//===================================
// 入力位置を復元
//===================================
void CShinKani01EX2::RestoreInputPosition()
{
	if ( m_curInPos == -1 ) {
		return;
	}

	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		m_Kani01Diag01.SetPosition ( m_curInPos );
		m_Kani01Diag01.SetFocus();
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		m_Kani01Diag02.SetPosition ( m_curInPos );
		m_Kani01Diag02.SetFocus();
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL3 ) {
		m_Kani01Diag03.SetPosition ( m_curInPos );
		m_Kani01Diag03.SetFocus();
	}
	
}

BEGIN_EVENTSINK_MAP(CShinKani01EX2, CSyzBaseDlg)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL1, 1, CShinKani01EX2::EditONIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL2, 1, CShinKani01EX2::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL3, 1, CShinKani01EX2::EditONIcsdiag8ctrl3, VTS_I2)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL1, 2, CShinKani01EX2::EditOFFIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL2, 2, CShinKani01EX2::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL3, 2, CShinKani01EX2::EditOFFIcsdiag8ctrl3, VTS_I2)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL1, 3, CShinKani01EX2::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL2, 3, CShinKani01EX2::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL3, 3, CShinKani01EX2::TerminationIcsdiag8ctrl3, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
//----->('23.05.01 Add)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL1, 9, CShinKani01EX2::CheckButtonIcsdiag8ctrl1, VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinKani01EX2, IDC_ICSDIAG8CTRL3, 9, CShinKani01EX2::CheckButtonIcsdiag8ctrl3, VTS_I2 VTS_UNKNOWN)
//<-----
END_EVENTSINK_MAP()


void CShinKani01EX2::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	switch( index ) {
	case ID2062:	// （個人の方）振替継続希望
		// 2022/12/27 CheckButtonIcsdiag8ctrl1に移動
		/*( *m_pSnHeadData )->Sn_GenericSgn &= ~0x200;
		m_Kani01Diag01.GetData( ID2062, ( struct IUnknown* )&DiagData );
		if( DiagData.data_check ) {
			( *m_pSnHeadData )->Sn_GenericSgn |= 0x200;
		}*/
		break;
	default:
		break;
	}
}


void CShinKani01EX2::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}


void CShinKani01EX2::EditONIcsdiag8ctrl3(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL3;

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch ( index ) {
		case ID4000:	// 割賦基準の適用
			if ( !SW_WARIFU ) {
				SW_WARIFU = 1;
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4000, ( struct IUnknown* )&DiagData );
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4001, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4000 );
			}
			break;
		case ID4001:	// 割賦基準の適用
			if ( SW_WARIFU ) {
				SW_WARIFU = 0;
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4000, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4001, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4001 );
			}
			break;
		case ID4010:	// 延払基準の適用
			if ( !SW_NOBEBA ) {
				SW_NOBEBA = 1;
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4010, ( struct IUnknown* )&DiagData );
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4011, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4010 );
			}
			break;
		case ID4011:	// 延払基準の適用
			if ( SW_NOBEBA ) {
				SW_NOBEBA = 0;
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4010, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4011, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4011 );
			}
			break;
		case ID4020:	// 工事進行基準の適用
			if ( !SW_KOUJIS ) {
				SW_KOUJIS = 1;
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4020, ( struct IUnknown* )&DiagData );
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4021, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4020 );
			}
			break;
		case ID4021:	// 工事進行基準の適用
			if ( SW_KOUJIS ) {
				SW_KOUJIS = 0;
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4020, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4021, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4021 );
			}
			break;
		case ID4030:	// 現金主義会計の適用
			if ( !SW_GENKIN ) {
				SW_GENKIN = 1;
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4030, ( struct IUnknown* )&DiagData );
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4031, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4030 );
			}
			break;
		case ID4031:	// 現金主義会計の適用
			if ( SW_GENKIN ) {
				SW_GENKIN = 0;
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4030, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4031, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4031 );
			}
			break;
		case ID4040:	// 規則22条1項の適用
			if ( !SW_KISK22 ) {
				SW_KISK22 = 1;
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4040, ( struct IUnknown* )&DiagData );
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4041, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4040 );
			}
			break;
		case ID4041:	// 規則22条1項の適用
			if ( SW_KISK22 ) {
				SW_KISK22 = 0;
				DiagData.data_check = 0;
				m_Kani01Diag03.SetData ( ID4040, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.DeleteInput();
				DiagData.data_check = 1;
				m_Kani01Diag03.SetData ( ID4041, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition ( ID4041 );
			}
			break;
		case ID4420:	// （個人の方）公金受取口座の利用
			// 2022/12/27 CheckButtonIcsdiag8ctrl3に移動
			/*( *m_pSnHeadData )->Sn_GenericSgn &= ~0x400;
			m_Kani01Diag03.GetData( ID4420, ( struct IUnknown* )&DiagData );
			if( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_GenericSgn |= 0x400;
			}*/
			break;
		case ID4ZI3:	// 税理士法第30条
			( *m_pSnHeadData )->Sn_ZeiHou &= 0xfe;
			m_Kani01Diag03.GetData ( ID4ZI3, ( struct IUnknown* )&DiagData );
			if ( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_ZeiHou |= 0x01;
			}
			break;
		case ID4ZI4:	// 税理士法第33条の2
			( *m_pSnHeadData )->Sn_ZeiHou &= 0xfd;
			m_Kani01Diag03.GetData ( ID4ZI4, ( struct IUnknown* )&DiagData );
			if ( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_ZeiHou |= 0x02;
			}
			break;
		default:
			break;
	}
}


void CShinKani01EX2::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	char yymmdd[3] = { 0 };
	char buf[512] = { 0 };
	int len = 0;
	DIAGRAM_DATA DiagData, DiagDataYY, DiagDataMM, DiagDataDD;
	DiagData.data_imgdata = NULL;
	char tmpYmd[4] = { 0 };

	m_Util.DiagOcxIniz ( &DiagData );
	m_Kani01Diag01.GetData ( index, ( struct IUnknown* )&DiagData );
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2] );

	switch ( index ) {
		case ID2000:	// 提出年
			if ( DiagData.data_day[0] != 0 ) {
				m_Util.DiagOcxIniz ( &DiagDataMM );
				m_Kani01Diag01.GetData ( ID2001, ( struct IUnknown* )&DiagDataMM );
				m_Util.DiagOcxIniz ( &DiagDataDD );
				m_Kani01Diag01.GetData ( ID2002, ( struct IUnknown* )&DiagDataDD );

				// 令和の期間かチェック
				if ( m_Util.IsValidReiwaYmd ( ID_ICSSH_REIWA_GENGO, DiagData.data_day[0], DiagDataMM.data_day[0], DiagDataDD.data_day[0] ) ) {
					( *m_pSnHeadData )->SetYmdDataGen ( ID_ICSSH_PRESENT_DATE, 0, DiagData.data_day[0], DiagDataMM.data_day[0], DiagDataDD.data_day[0], 0 );
				}
				else {
					DiagData.data_day[0] = yymmdd[0];
					m_Kani01Diag01.SetData ( index, ( struct IUnknown* )&DiagData );
					m_Kani01Diag01.SetPosition ( index );
				}
			}
			else {
				DiagData.data_day[0] = yymmdd[0];
				m_Kani01Diag01.SetData ( index, ( struct IUnknown* )&DiagData );
			}
			break;
		case ID2001:	// 提出月
			if ( DiagData.data_day[0] != 0 ) {
				m_Util.DiagOcxIniz ( &DiagDataYY );
				m_Kani01Diag01.GetData ( ID2000, ( struct IUnknown* )&DiagDataYY );
				m_Util.DiagOcxIniz ( &DiagDataDD );
				m_Kani01Diag01.GetData ( ID2002, ( struct IUnknown* )&DiagDataDD );

				// 令和の期間かチェック
				if ( m_Util.IsValidReiwaYmd ( ID_ICSSH_REIWA_GENGO, DiagDataYY.data_day[0], DiagData.data_day[0], DiagDataDD.data_day[0] ) ) {
					( *m_pSnHeadData )->SetYmdDataGen ( ID_ICSSH_PRESENT_DATE, 1, DiagDataYY.data_day[0], DiagData.data_day[0], DiagDataDD.data_day[0], 0 );
				}
				else {
					DiagData.data_day[0] = yymmdd[1];
					m_Kani01Diag01.SetData ( index, ( struct IUnknown* )&DiagData );
					m_Kani01Diag01.SetPosition ( index );
				}

			}
			else {
				DiagData.data_day[0] = yymmdd[1];
				m_Kani01Diag01.SetData ( index, ( struct IUnknown* )&DiagData );
			}
			break;
		case ID2002:	// 提出日
			if ( DiagData.data_day[0] != 0 ) {
				m_Util.DiagOcxIniz ( &DiagDataYY );
				m_Kani01Diag01.GetData ( ID2000, ( struct IUnknown* )&DiagDataYY );
				m_Util.DiagOcxIniz ( &DiagDataMM );
				m_Kani01Diag01.GetData ( ID2001, ( struct IUnknown* )&DiagDataMM );

				// 令和の期間かチェック
				if ( m_Util.IsValidReiwaYmd ( ID_ICSSH_REIWA_GENGO, DiagDataYY.data_day[0], DiagDataMM.data_day[0], DiagData.data_day[0] ) ) {
					( *m_pSnHeadData )->SetYmdDataGen ( ID_ICSSH_PRESENT_DATE, 2, DiagDataYY.data_day[0], DiagDataMM.data_day[0], DiagData.data_day[0], 0 );
				}
				else {
					DiagData.data_day[0] = yymmdd[2];
					m_Kani01Diag01.SetData ( index, ( struct IUnknown* )&DiagData );
					m_Kani01Diag01.SetPosition ( index );
				}
			}
			else {
				DiagData.data_day[0] = yymmdd[2];
				m_Kani01Diag01.SetData ( index, ( struct IUnknown* )&DiagData );
			}
			break;
		case ID2010:	// 税務署名
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, ( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
			len = ( int )DiagData.data_edit.GetLength();
			memset ( ( *m_pSnHeadData )->Sn_ZMSYO, '\0', 12 );
			memmove ( ( *m_pSnHeadData )->Sn_ZMSYO, DiagData.data_edit, len );
			if ( len ) {
				char name1[20] = { 0 },
					 name2[20] = { 0 };
				memmove ( name1, ( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
				memmove ( name2, buf, 12 );
				if ( Esel_CompareTaxOffice ( name1, name2 ) ) {
					memset ( ( *m_pSnHeadData )->Sn_TAXNO, '\0', sizeof ( ( *m_pSnHeadData )->Sn_TAXNO ) );
					// 税務署コードの取得
					if ( *( m_pShinInfo->pIsOpenEselect ) == TRUE ) {
						if ( ( *m_pSnHeadData )->GetTaxationOffice ( this, m_pShinInfo->pEselect ) ) {
							// 税務署コード
							memset ( buf, '\0', sizeof ( buf ) );
							if ( ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 0x00 ) && ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 'F' ) ) {
								sprintf_s ( buf, sizeof ( buf ), _T( "署番号 %-5.5s" ), ( *m_pSnHeadData )->Sn_TAXNO );
							}
							else {
								sprintf_s ( buf, sizeof ( buf ), _T( "署番号未登録" ) );
							}
							DiagData.data_disp = buf;
							m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );
							break;
						}
						memset ( buf, '\0', sizeof ( buf ) );
						len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
						memmove ( buf, ( *m_pSnHeadData )->Sn_ZMSYO, len );
						DiagData.data_edit = buf;
						m_Kani01Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );
					}
				}
				// 税務署コード
				memset ( buf, '\0', sizeof ( buf ) );
				if ( ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 0x00 ) && ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 'F' ) ) {
					sprintf_s ( buf, sizeof ( buf ), _T( "署番号 %-5.5s" ), ( *m_pSnHeadData )->Sn_TAXNO );
				}
				else {
					sprintf_s ( buf, sizeof ( buf ), _T( "署番号未登録" ) );
				}
				DiagData.data_disp = buf;
				m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );
			}
			else {
				memset ( ( *m_pSnHeadData )->Sn_TAXNO, '\0', sizeof ( ( *m_pSnHeadData )->Sn_TAXNO ) );
				DiagData.data_disp = _T( "署番号未登録" );
				m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );
			}
			break;
		case ID2020:	// 郵便番号
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_ZIP, buf, 8 );
			break;
		case ID2030:	// 納税地 上段
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_NOZEI, buf, 40 );
			break;
		case ID2031:	// 納税地 下段
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( &( *m_pSnHeadData )->Sn_NOZEI[40], buf, 40 );
			break;
		case ID2040:	// 電話番号 市外
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_TELNO1, buf, 6 );
			break;
		case ID2041:	// 電話番号 市内
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_TELNO2, buf, 4 );
			break;
		case ID2042:	// 電話番号 局番
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_TELNO3, buf, 4 );
			break;
		case ID2050:	// 名称又は屋号 フリガナ
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_COFRI, buf, 40 * 2 );
			break;
		case ID2051:	// 名称又は屋号
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_CONAM, buf, 40 );
			break;
		case ID2052:	// 被合併名
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_GPNAM, buf, 40 );
			break;
		case ID2060:	// 代表者又は氏名 フリガナ
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_DIFRI, buf, 34 * 2 );
			break;
		case ID2061:	// 代表者又は氏名
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_DIHYO, buf, 40 );
			break;
		case ID2062:	// （個人の方）継続振替希望
			// 2022/12/27 CheckButtonIcsdiag8ctrl1に移動
			/*( *m_pSnHeadData )->Sn_GenericSgn &= ~0x200;
			m_Kani01Diag01.GetData ( ID2062, ( struct IUnknown* )&DiagData );
			if( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_GenericSgn |= 0x200;
			}*/

			// 確認メッセージ内のチェックを付けたときに画面上もチェックを付けたいため、
			// クリックした時点では一旦チェックの状態を元に戻す
			if ((*m_pSnHeadData)->Sn_GenericSgn & 0x200) {
				DiagData.data_check = 1;
			}
			else {
				DiagData.data_check = 0;
			}
			m_Kani01Diag01.SetData( ID2062, (LPUNKNOWN)&DiagData );
			break;
		case ID2081:	// 所管
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( ( *m_pSnHeadData )->Sn_SYOKN, buf, 3 );
			break;
		case ID2082:	// 整理番号
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			m_pArith->l_pack ( ( *m_pSnHeadData )->Sn_SEIRI, buf, 8 );
			break;
		default:
			break;
	}
}


void CShinKani01EX2::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char CkMy[MONY_BUF_SIZE] = { 0 };

	char ksute100[MONY_BUF_SIZE] = { 0 };
	m_pArith->l_input(ksute100, _T("100"));

	m_Kani01Diag02.GetData ( index, ( struct IUnknown* )&DiagData );
	switch ( index ) {

	case ID3XM10:	// 10. 中間納付税額

		

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );

		//------------>yoshida190621
		//手入力項目での切り捨ての為、アプリケーションにて切り捨て（100円未満切り捨て）
		m_pArith->l_div(CkMy, CkMy, ksute100);
		m_pArith->l_mul(CkMy, CkMy, ksute100);

		dsp_prs(IDC_ICSDIAG8CTRL2, ID3XM10, CkMy);
		//<------------

		SyukeiDataSet(index);
		Disp_Kanidata(10);
		break;

	//yoshida190625
	case ID3XM13:	// 13. 既確定税額
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

		SyukeiDataSet(index);
		Disp_Kanidata(13);
		break;


	case ID3XM15:	// 15. この課税期間の課税売上高
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

		SyukeiDataSet(index);
		Disp_Kanidata(15);
		break;

	case ID3XM16:	// 16. 基準期間の課税売上高
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

		SyukeiDataSet(index);
		Disp_Kanidata(15);
		break;

	case ID3XM21:	// 21. 中間納付譲渡割額

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );

		//------------>yoshida190621
		//手入力項目での切り捨ての為、アプリケーションにて切り捨て（100円未満切り捨て）
		m_pArith->l_div(CkMy, CkMy, ksute100);
		m_pArith->l_mul(CkMy, CkMy, ksute100);

		dsp_prs(IDC_ICSDIAG8CTRL2, ID3XM21, CkMy);
		//<------------

		SyukeiDataSet(index);
		Disp_Kanidata(21);
		break;

	//yoshida190625
	case ID3XM24:	// 24. 既確定譲渡割額
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
	
		SyukeiDataSet(index);
		Disp_Kanidata(24);
		break;


	default:
		break;
	}
}


void CShinKani01EX2::EditOFFIcsdiag8ctrl3(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[512] = { 0 };
	int len = 0;

	m_Kani01Diag03.GetData ( index, ( struct IUnknown* )&DiagData );

	switch ( index ) {
		case ID4210:	// 金融機関 名
			memset ( buf, '\0', sizeof ( buf ) );
			len = ( int )DiagData.data_edit.GetLength();
			if ( len ) {
				memmove ( buf, DiagData.data_edit, len );
			}
			memmove ( ( *m_pSnHeadData )->Sn_BANK, buf, 20 );
			break;
		case ID4211:	// 金融機関 支店名
			memset ( buf, '\0', sizeof ( buf ) );
			len = ( int )DiagData.data_edit.GetLength();
			if ( len ) {
				memmove ( buf, DiagData.data_edit, len );
			}
			memmove ( ( *m_pSnHeadData )->Sn_SITEN, buf, 20 );
			break;
		case ID4212:	// 銀行・金庫等
			( *m_pSnHeadData )->Sn_FINANCE = ( char )DiagData.data_combo;
			break;
		case ID4213:	// 本社・支社等
			( *m_pSnHeadData )->Sn_CHAIN = ( char )DiagData.data_combo;
			break;
		case ID4214:	// 金融機関 預金種類等
			memset ( buf, '\0', sizeof ( buf ) );
			kspclr ( buf, 4 );
			len = ( int )strlen ( DiagData.data_edit );
			if ( len ) {
				memmove ( buf, DiagData.data_edit, len );
			}
			memmove ( ( *m_pSnHeadData )->Sn_YOKIN, buf, 8 );
			break;
		case ID4215:	// 金融機関　口座番号
			memset ( buf, '\0', sizeof ( buf ) );
			memset ( buf, 0x20, 10 );
			len = ( int )strlen ( DiagData.data_edit );
			if ( len ) {
				memmove ( &buf[10 - len], DiagData.data_edit, len );
			}
			m_pArith->l_pack ( ( *m_pSnHeadData )->Sn_KOZA, buf, 10 );
			break;
		case ID4310:	// 郵便局 貯金記号
			memset ( buf, '\0', sizeof ( buf ) );
			memset ( buf, 0x20, 5 );
			len = ( int )strlen ( DiagData.data_edit );
			if ( len ) {
				memmove ( &buf[5 - len], DiagData.data_edit, len );
			}
			memmove ( ( *m_pSnHeadData )->Sn_YUNUM1, buf, 5 );
			break;
		case ID4311:	// 郵便局 貯金番号
			memset ( buf, '\0', sizeof ( buf ) );
			memset ( buf, 0x20, 8 );
			len = ( int )strlen ( DiagData.data_edit );
			if ( len ) {
				memmove ( &buf[8 - len], DiagData.data_edit, len );
			}
			memmove ( ( *m_pSnHeadData )->Sn_YUNUM2, buf, 8 );
			break;
		case ID4312:	// 郵便局 郵便局名
			memset ( buf, '\0', sizeof ( buf ) );
			len = ( int )DiagData.data_edit.GetLength();
			if ( len ) {
				memmove ( buf, DiagData.data_edit, len );
			}
			memmove ( ( *m_pSnHeadData )->Sn_YUBIN, buf, 16 );
			break;
		case ID4410:	// 還付金融機関
			if ( DiagData.data_combo ) {
				( *m_pSnHeadData )->Sn_BYsw = 1;
			}
			else {
				( *m_pSnHeadData )->Sn_BYsw = 0;
			}
			break;
		case ID4420:
			// 2022/12/27 CheckButtonIcsdiag8ctrl3に移動
			/*( *m_pSnHeadData )->Sn_GenericSgn &= ~0x400;
			m_Kani01Diag03.GetData ( ID4420, ( struct IUnknown* )&DiagData );
			if( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_GenericSgn |= 0x400;
			}*/

			// 確認メッセージ内のチェックを付けたときに画面上もチェックを付けたいため、
			// クリックした時点では一旦チェックの状態を元に戻す
			if( ( *m_pSnHeadData )->Sn_GenericSgn & 0x400 ){
				DiagData.data_check = 1;
			}
			else{
				DiagData.data_check = 0;
			}
			m_Kani01Diag03.SetData( ID4420, (LPUNKNOWN)&DiagData );
			break;
		case ID4RNO:	// 税務署整理欄
			memset ( buf, '\0', sizeof ( buf ) );
			len = DiagData.data_edit.GetLength();
			memmove ( buf, DiagData.data_edit, len );
			m_pArith->l_pack ( ( *m_pSnHeadData )->Sn_ZSEIRI, buf, 8 );
			break;
		case ID4ZI3:	// 税理士法第30条
			( *m_pSnHeadData )->Sn_ZeiHou &= 0xfe;
			if ( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_ZeiHou |= 0x01;
			}
			break;
		case ID4ZI4:	// 税理士法第33条の2
			( *m_pSnHeadData )->Sn_ZeiHou &= 0xfd;
			if ( DiagData.data_check ) {
				( *m_pSnHeadData )->Sn_ZeiHou |= 0x02;
			}
			break;
		default:
			break;
	}
}


void CShinKani01EX2::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	char buff[64] = { 0 };
	int len = 0;
	CString ZipBf, AddBf;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( nChar == 0 ) {
		return;
	}

	char buf[256] = { 0 };
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

	switch ( index ) {
		case ID2010:	// 税務署名
			if ( wTerm == VK_HOME ) {
				if ( *( m_pShinInfo->pIsOpenEselect ) ) {
					memset ( m_pShinInfo->pEselect, '\0', sizeof ( ESELECT ) );
					m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
					if ( ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 0x00 ) && ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 'F' ) ) {
						memmove ( m_pShinInfo->pEselect->cd_office, ( *m_pSnHeadData )->Sn_TAXNO, sizeof ( ( *m_pSnHeadData )->Sn_TAXNO ) );
					}
					m_Util.DiagOcxIniz ( &DiagData );
					m_Kani01Diag01.GetData ( index, ( struct IUnknown* )&DiagData );
					len = ( int )DiagData.data_edit.GetLength();
					memset ( ( *m_pSnHeadData )->Sn_ZMSYO, '\0', sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
					memmove ( ( *m_pSnHeadData )->Sn_ZMSYO, DiagData.data_edit, len );
					if ( Esel_SelectTaxOffice ( m_pShinInfo->pEselect ) == TRUE ) {
						memmove ( ( *m_pSnHeadData )->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, sizeof ( ( *m_pSnHeadData )->Sn_TAXNO ) );
						memset ( buf, '\0', sizeof ( buf ) );
						kspclr ( buf, 6 );
						len = ( int )strlen ( m_pShinInfo->pEselect->nm_office );
						memmove ( buf, m_pShinInfo->pEselect->nm_office, len );
						memmove ( ( *m_pSnHeadData )->Sn_ZMSYO, buf, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );

						// 税務署名
						memset ( buf, '\0', sizeof ( buf ) );
						len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
						memmove ( buf, ( *m_pSnHeadData )->Sn_ZMSYO, len );
						DiagData.data_edit = buf;
						m_Kani01Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );
						memset ( buf, '\0', sizeof ( buf ) );
						sprintf_s ( buf, sizeof ( buf ), _T( "署番号 %-5.5s" ), ( *m_pSnHeadData )->Sn_TAXNO );
						DiagData.data_disp = buf;
						m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );
						m_Kani01Diag01.SetPosition ( ID2020 );
					}
					else {
						m_Kani01Diag01.SetPosition ( ID2010 );
					}
				}
			}
			break;
		case ID2020:	// 郵便番号
			if ( wTerm == VK_HOME ) {
				m_Kani01Diag01.GetData ( ID2020, ( struct IUnknown* )&DiagData );
				len = DiagData.data_edit.GetLength();
				memset ( buf, '\0', sizeof ( buf ) );
				memmove ( buf, DiagData.data_edit, len );
				memmove ( ( *m_pSnHeadData )->Sn_ZIP, buf, 8 );
				ZipBf = ( *m_pSnHeadData )->Sn_ZIP;
				if ( ZipToAddress ( ZipBf, AddBf ) == TRUE ) {
					// 納税地 住所(上)
					memset ( buf, '\0', sizeof ( buf ) );
					len = AddBf.GetLength();
					memmove ( buf, AddBf, len );
					memmove ( ( *m_pSnHeadData )->Sn_NOZEI, buf, 40 );
					memset ( buff, '\0', sizeof ( buff ) );
					memmove ( buff, buf, 40 );
					DiagData.data_edit = buff;
					m_Kani01Diag01.SetData ( ID2030, ( struct IUnknown* )&DiagData );

					// 納税地 郵便番号
					len = ZipBf.GetLength();
					memset ( buf, '\0', sizeof ( buf ) );
					memmove ( buf, ZipBf, len );
					memmove ( ( *m_pSnHeadData )->Sn_ZIP, buf, 8 );
					DiagData.data_edit = buf;
					m_Kani01Diag01.SetData ( ID2020, ( struct IUnknown* )&DiagData );
					m_Kani01Diag01.SetPosition ( ID2030 );
				}
				else {
					m_Kani01Diag01.SetPosition ( ID2020 );
				}
			}
			break;
		case ID2030:	// 納税地 上段
			if ( wTerm == VK_HOME ) {
				ZipBf = ( *m_pSnHeadData )->Sn_ZIP;
				if ( ZipToAddress ( ZipBf, AddBf ) == TRUE ) {
					memset ( buf, '\0', sizeof ( buf ) );
					len = AddBf.GetLength();
					memmove ( buf, AddBf, len );
					memmove ( ( *m_pSnHeadData )->Sn_NOZEI, buf, 40 );
					memset ( buff, '\0', sizeof ( buff ) );
					memmove(buff, buf, 40);
					DiagData.data_edit = buff;
					m_Kani01Diag01.SetData ( ID2030, ( struct IUnknown* )&DiagData );
				}
				m_Kani01Diag01.SetPosition ( ID2030 );
			}
			break;
		case ID2031:	// 納税地 下段
			if ( wTerm == VK_HOME ) {
				ZipBf = ( *m_pSnHeadData )->Sn_ZIP;
				if ( ZipToAddress ( ZipBf, AddBf ) == TRUE ) {
					memset ( buf, '\0', sizeof ( buf ) );
					len = AddBf.GetLength();
					memmove ( buf, AddBf, len );
					memmove ( &( *m_pSnHeadData )->Sn_NOZEI[40], buf, 40 );
					memset ( buff, '\0', sizeof ( buff ) );
					memmove ( buff, buf, 40 );
					DiagData.data_edit = buff;
					m_Kani01Diag01.SetData ( ID2031, ( struct IUnknown* )&DiagData );
				}
				m_Kani01Diag01.SetPosition ( ID2031 );
			}
			break;
		default:
			break;
	}

	if ( wTerm == VK_DELETE ) {
		dsp_cls ( IDC_ICSDIAG8CTRL1, index );
	}

	// 次項目
	int nowdiag = 1;
	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) ) {
		if ( index == ID2082 ) {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) {
					// 確定
					m_Kani01Diag02.SetPosition(ID3XM10);	// 中間納付税額
					nowdiag = 2;
				}
				else {
					// 中間
					if ( !( *m_pSnHeadData )->SVmzsw ) {
						// 経過措置なし
						m_Kani01Diag02.SetPosition ( ID3XM15 );		// この課税期間の課税売上高
						nowdiag = 2;
					}
					else {
						// 経過措置あり
						if ( ( *m_pSnHeadData )->IsRevisedTaxReturn() ) {
							m_Kani01Diag02.SetPosition ( ID3XM13 );	// 既確定税額
							nowdiag = 2;
						}
						else {
							m_Kani01Diag02.SetPosition( ID3XM15 );
							nowdiag = 2;
						}
					}
				}
			}
			else {
				if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) {
					// 確定
					// 経過措置あり
					m_Kani01Diag02.SetPosition ( ID3XM10 );	// 中間納付税額
					nowdiag = 2;
				}
				else {
					// 中間
					// 経過措置あり
					if ( ( *m_pSnHeadData )->IsRevisedTaxReturn() ) {
						m_Kani01Diag02.SetPosition ( ID3XM13 );	// 既確定税額
						nowdiag = 2;
					}
					else {
						// 中間の場合
						if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0x02 ) == 0x02 ) {
							if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {	// 連動の場合
								m_Kani01Diag02.SetPosition ( ID3XM16 );	// 基準期間の課税売上高
								nowdiag = 2;
							}
						}
						else {
							m_Kani01Diag02.SetPosition( ID3XM15 );
							nowdiag = 2;
						}
					}
				}
			}
		}
		else {
			m_Kani01Diag01.SetNextPosition();
		}
	}

	// 前項目へ
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
		if ( index == ID2000 ) {
			m_Kani01Diag03.SetPosition ( ID4ZI4 );
			nowdiag = 3;
		}
		else {
			m_Kani01Diag01.SetPrevPosition();
		}
	}

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	int nowpos = 0;
	if ( nowdiag == 1 ) {
		nowpos = m_Kani01Diag01.GetPosition();
		m_Kani01Diag01.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}
	else if ( nowdiag == 2 ) {
		nowpos = m_Kani01Diag02.GetPosition();
		m_Kani01Diag02.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}
	else if ( nowdiag == 3 ) {
		nowpos = m_Kani01Diag03.GetPosition();
		m_Kani01Diag03.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}

	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}


void CShinKani01EX2::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if ( nChar == 0 ) {
		return;
	}

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

	// 次項目
	int nowdiag = 2;
	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) ) {
		if ( ( *m_pSnHeadData )->IsRevisedTaxReturn() ) {
			if ( index == ID3XM24 ) {
				if ( SW_WARIFU ) {
					m_Kani01Diag03.SetPosition ( ID4000 );
					nowdiag = 3;
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4001 );
					nowdiag = 3;
				}
			}
			else {
				m_Kani01Diag02.SetNextPosition();
			}
		}
		else {
			if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) {
				int Id = 0;
				Id = ID3XM21;
				if ( index == Id ) {
					if ( SW_WARIFU ) {
						m_Kani01Diag03.SetPosition ( ID4000 );
						nowdiag = 3;
					}
					else {
						m_Kani01Diag03.SetPosition ( ID4001 );
						nowdiag = 3;
					}
				}
				else {
					m_Kani01Diag02.SetNextPosition();
				}
			}
			else {
				int Id = 0;
				Id = ID3XM16;
				if ( index == Id ){
					if ( SW_WARIFU ) {
						m_Kani01Diag03.SetPosition ( ID4000 );
						nowdiag = 3;
					}
					else {
						m_Kani01Diag03.SetPosition ( ID4001 );
						nowdiag = 3;
					}
				}
				else {
					m_Kani01Diag02.SetNextPosition();
				}
			}
		}
	}

	// 前項目
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {	// 経過措置がある場合
				if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) {	// 確定・非連動・経過措置あり
					if ( index == ID3XM10 ) {
						m_Kani01Diag01.SetPosition ( ID2082 );	//　整理番号
						nowdiag = 1;
					}
					else {
						m_Kani01Diag02.SetPrevPosition();
					}
				}
				else {	// 中間・非連動・経過措置あり
					if ( index == ID3XM15 ) {
						m_Kani01Diag01.SetPosition ( ID2082 );	//　整理番号
						nowdiag = 1;
					}
					else {
						m_Kani01Diag02.SetPrevPosition();
					}
				}
			}
			else {
				if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) {	// 確定・非連動・経過措置なし
					if( index == ID3XM10 ){
						m_Kani01Diag01.SetPosition ( ID2082 );
						nowdiag = 1;
					}
					else {
						m_Kani01Diag02.SetPrevPosition();
					}
				}
				else {	// 中間・非連動・経過措置なし
					if( index == ID3XM15 ){
						m_Kani01Diag01.SetPosition ( ID2082 );
						nowdiag = 1;
					}
					else {
						m_Kani01Diag02.SetPrevPosition();
					}
				}
			}
		}
		else {
			if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) {
				if ( index == ID3XM10 ) {
					m_Kani01Diag01.SetPosition ( ID2082 );	// 整理番号
					nowdiag = 1;
				}
				else {
					m_Kani01Diag02.SetPrevPosition();
				}
			}
			else {
				if ( ( ( *m_pSnHeadData )->Sn_SKKBN & 0x02 ) == 0x02 ) {
					if ( index == ID3XM16 ) {
						m_Kani01Diag01.SetPosition ( ID2082 );	// 整理番号
						nowdiag = 1;
					}
					else {
						m_Kani01Diag02.SetPrevPosition();
					}
				}
				else {
					if ( index == ID3XM13 ) {
						m_Kani01Diag01.SetPosition ( ID2082 );
						nowdiag = 1;
					}
					else {
						m_Kani01Diag02.SetPrevPosition();
					}
				}
			}
		}
	}

	if ( wTerm == VK_DELETE ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		m_Kani01Diag02.SetPosition(index);
	}

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	int nowpos = 0;
	if ( nowdiag == 1 ) {
		nowpos = m_Kani01Diag01.GetPosition();
		m_Kani01Diag01.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}
	else if ( nowdiag == 2 ) {
		nowpos = m_Kani01Diag02.GetPosition();
		m_Kani01Diag02.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}
	else if ( nowdiag == 3 ) {
		nowpos = m_Kani01Diag03.GetPosition();
		m_Kani01Diag03.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}

	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}


void CShinKani01EX2::TerminationIcsdiag8ctrl3(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if ( nChar == 0 ) {
		return;
	}

	char			buf[512]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

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

	if ( wTerm == VK_DELETE ) {
		dsp_cls ( IDC_ICSDIAG8CTRL3, index );
		return;
	}

	int nowdiag = 3;
	switch ( index ) {
		case ID4000:	// 割賦基準の適用
		case ID4001:
			if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_RIGHT ) ) {
				if ( SW_WARIFU ) {
					SW_WARIFU = 0;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4001, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4000, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4001 );
				}
				else {
					SW_WARIFU = 1;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4000, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4001, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4000 );
				}
				break;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) ) {
				if ( SW_NOBEBA ) {
					m_Kani01Diag03.SetPosition ( ID4010 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4011 );
				}
			}

			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) ) {
					// 中間申告
					if ( ( *m_pSnHeadData )->IsRevisedTaxReturn() ) {
						m_Kani01Diag02.SetPosition ( ID3XM24 );
						nowdiag = 2;
					}
					else {
						m_Kani01Diag02.SetPosition ( ID3XM16 );
						nowdiag = 2;
					}
				}
				else {
					// 確定
					if ( ( *m_pSnHeadData )->IsRevisedTaxReturn() ) {
						m_Kani01Diag02.SetPosition ( ID3XM24 );
						nowdiag = 2;
					}
					else {
						m_Kani01Diag02.SetPosition ( ID3XM21 );
						nowdiag = 2;
					}
				}
			}
			break;
		case ID4010:	// 延払基準の適用
		case ID4011:
			if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_RIGHT ) ) {
				if ( SW_NOBEBA ) {
					SW_NOBEBA = 0;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4011, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4010, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4011 );
				}
				else {
					SW_NOBEBA = 1;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4010, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4011, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4010 );
				}
				break;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( VK_DOWN ) ) {
				if ( SW_KOUJIS ) {
					m_Kani01Diag03.SetPosition ( ID4020 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4021 );
				}
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if ( SW_WARIFU ) {
					m_Kani01Diag03.SetPosition ( ID4000 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4001 );
				}
			}
			break;
		case ID4020:	// 工事進行基準の適用
		case ID4021:
			if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_RIGHT ) ) {
				if ( SW_KOUJIS ) {
					SW_KOUJIS = 0;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4021, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4020, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4021 );
				}
				else {
					SW_KOUJIS = 1;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4020, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4021, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4020 );
				}
				break;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( VK_DOWN ) ) {
				if ( SW_GENKIN ) {
					m_Kani01Diag03.SetPosition ( ID4030 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4031 );
				}
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if ( SW_NOBEBA ) {
					m_Kani01Diag03.SetPosition ( ID4010 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4011 );
				}
			}
			break;
		case ID4030:	// 現金主義会計の適用
		case ID4031:
			if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_RIGHT ) ) {
				if ( SW_GENKIN ) {
					SW_GENKIN = 0;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4031, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4030, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4031 );
				}
				else {
					SW_GENKIN = 1;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4030, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4031, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4030 );
				}
				break;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( VK_DOWN ) ) {
				if( m_IsKanpuEnable ){
					m_Kani01Diag03.SetPosition ( ID4210 );
				}
				else{
					m_Kani01Diag03.SetPosition ( ID4420 );
				}
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if ( SW_KOUJIS) {
					m_Kani01Diag03.SetPosition ( ID4020 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4021 );
				}
			}
			break;
		case ID4040:
		case ID4041:
			if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_RIGHT ) ) {
				if ( SW_KISK22 ) {
					SW_KISK22 = 0;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4041, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4040, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4041 );
				}
				else {
					SW_KISK22 = 1;
					DiagData.data_check = 1;
					m_Kani01Diag03.SetData ( ID4040, ( struct IUnknown* )&DiagData );
					DiagData.data_check = 0;
					m_Kani01Diag03.SetData ( ID4041, ( struct IUnknown* )&DiagData );
					m_Kani01Diag03.SetPosition ( ID4040 );
				}
				break;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( VK_DOWN ) ) {
				if ( SW_KISK22 ) {
					m_Kani01Diag03.SetPosition ( ID4160 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4161 );
				}
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if ( SW_GENKIN ) {
					m_Kani01Diag03.SetPosition ( ID4030 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4031 );
				}
			}
			break;
		case ID4210:	// 金融機関 名
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4212 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if ( SW_GENKIN ) {
					m_Kani01Diag03.SetPosition ( ID4030 );
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4031 );
				}
			}
			break;
		case ID4211:	// 金融機関 支店名
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4213 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4212 );
			}
			break;
		case ID4212:	// 銀行・金庫等
			// 矢印制御
			if (wTerm == VK_DOWN) {
				m_Kani01Diag03.ComboDropDown(ID4212, TRUE);
				return;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4211 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 )) {
				m_Kani01Diag03.SetPosition ( ID4210 );
			}
			break;
		case ID4213:	// 本店・支店等
			// 矢印制御
			if ( wTerm == VK_DOWN ) {
				m_Kani01Diag03.ComboDropDown ( ID4213, TRUE );
				return;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4214 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 )) {
				m_Kani01Diag03.SetPosition ( ID4211 );
			}
			break;
		case ID4214:	// 金融機関 預金種名
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4215 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4213 );
			}
			break;
		case ID4215:	// 金融機関 口座番号
			if ( wTerm == VK_DELETE ) {
				memset ( ( *m_pSnHeadData )->Sn_KOZA, 0xff, 8 );
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( buf, 0x20, 10 );
				dsp_cls ( IDC_ICSDIAG8CTRL3, ID4215 );
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4310 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4214 );
			}
			break;
		case ID4310:	// 郵便局 貯金記号
			if ( wTerm == VK_DELETE ) {
				memset ( ( *m_pSnHeadData )->Sn_YUNUM1, 0x20, 5 );
				dsp_cls ( IDC_ICSDIAG8CTRL3, ID4310 );
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4311 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4215 );
			}
			break;
		case ID4311:	// 郵便局 貯金番号
			if ( wTerm == VK_DELETE ) {
				memset ( ( *m_pSnHeadData )->Sn_YUNUM2, 0x20, 8 );
				dsp_cls ( IDC_ICSDIAG8CTRL3, ID4311 );
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4312 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4310 );
			}
			break;
		case ID4312:	// 郵便局 郵便局名
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4410 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4311 );
			}
			break;
		case ID4410:	// 還付金融機関
			// 矢印制御
			if ( wTerm == VK_DOWN ) {
				m_Kani01Diag03.ComboDropDown ( ID4410, TRUE );
				return;
			}
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				if(( *m_pSnHeadData )->IsSoleProprietor() ) {
					if( IsJunkakutei() ){
						m_Kani01Diag03.SetPosition ( ID4RNO );
					}
					else{
						m_Kani01Diag03.SetPosition ( ID4420 );
					}
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4RNO );
				}
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4312 );
			}
			break;
		case ID4420:	// （個人の方）公金受取口座の利用
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4RNO );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if( m_IsKanpuEnable ){
					m_Kani01Diag03.SetPosition ( ID4410 );
				}
				else{
					if ( SW_GENKIN ) {
						m_Kani01Diag03.SetPosition ( ID4030 );
					}
					else {
						m_Kani01Diag03.SetPosition ( ID4031 );
					}
				}
			}
			break;
		case ID4RNO:	// 税務署整理欄
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4ZI3 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				if(( *m_pSnHeadData )->IsSoleProprietor() ) {
					if( IsJunkakutei() ){
						m_Kani01Diag03.SetPosition ( ID4410 );
					}
					else{
						m_Kani01Diag03.SetPosition ( ID4420 );
					}
				}
				else {
					m_Kani01Diag03.SetPosition ( ID4410 );
				}
			}
			break;
		case ID4ZI3:	// 税理士法第30条
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag03.SetPosition ( ID4ZI4 );
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				m_Kani01Diag03.SetPosition ( ID4RNO );
			}
			break;
		case ID4ZI4:	// 税理士法第33条の2
			// 次項目
			if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
				m_Kani01Diag01.SetPosition ( ID2000 );
				nowdiag = 1;
			}
			// 前項目
			if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
				//m_Kani01Diag03.SetPosition ( ID4212 );
				m_Kani01Diag03.SetPosition ( ID4ZI3 );

			}
			break;
		default:
			break;
	}

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	int nowpos = 0;
	if ( nowdiag == 1 ) {
		nowpos = m_Kani01Diag01.GetPosition();
		m_Kani01Diag01.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}
	else if ( nowdiag == 2 ) {
		nowpos = m_Kani01Diag02.GetPosition();
		m_Kani01Diag02.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}
	else if ( nowdiag == 3 ) {
		nowpos = m_Kani01Diag03.GetPosition();
		m_Kani01Diag03.GetRect ( nowpos, ( LPUNKNOWN )&rectB );
	}

	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}


BOOL CShinKani01EX2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// フォーカス移動
			MoveFocusInEnd();

			// 簡易本表更新
			GetDiagData();
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
					TerminationIcsdiag8ctrl1 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}
				else if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}
				else if ( m_curwnd == IDC_ICSDIAG8CTRL3 ) {
					TerminationIcsdiag8ctrl3 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}
				return TRUE;
			}
		}

		// コンボボックスにフォーカスがある場合の対応
		if ( ( m_curwnd == IDC_ICSDIAG8CTRL1 ) || ( m_curwnd == IDC_ICSDIAG8CTRL2 ) || ( m_curwnd == IDC_ICSDIAG8CTRL3 ) ) {
			if ( ( pMsg->wParam == VK_LEFT ) || ( pMsg->wParam == VK_RIGHT ) || ( pMsg->wParam == VK_RETURN ) ) {
				DIAGRAM_ATTRIBUTE dattr;
				short result = ( ( CIcsdiagctrl* )GetDlgItem ( m_curwnd ) )->GetPosition();
				diag_getattr ( m_curwnd, result, &dattr, CIcsdiagctrl );
				if ( dattr.attr_inpkind == 1 ) {
					DIAGRAM_DATA dd;
					m_Util.DiagOcxIniz ( &dd );
					diag_getdata ( m_curwnd, result, &dd, CIcsdiagctrl );
					diag_cbdrop ( m_curwnd, result, FALSE, CIcsdiagctrl );
					if ( m_curwnd == IDC_ICSDIAG8CTRL3 ) {
						TerminationIcsdiag8ctrl3 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
					}
					return TRUE;
				}
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


void CShinKani01EX2::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	char buf[512] = { 0 };
	int len = 0;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( *m_pShinInfo->pIsOpenEselect ) {
		memset ( m_pShinInfo->pEselect, '\0', sizeof ( ESELECT ) );
		m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
		if ( ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 0x00 ) && ( ( ( *m_pSnHeadData )->Sn_TAXNO[0] & 0xff ) != 'F' ) ) {
			memmove ( m_pShinInfo->pEselect->cd_office, ( *m_pSnHeadData )->Sn_TAXNO, sizeof ( ( *m_pSnHeadData )->Sn_TAXNO ) );
		}

		if ( Esel_SelectTaxOffice ( m_pShinInfo->pEselect ) == IDOK ) {
			memmove ( ( *m_pSnHeadData )->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, 5 );
			memset ( buf, '\0', sizeof ( buf ) );
			len = ( int )strlen ( m_pShinInfo->pEselect->nm_office );
			memmove ( buf, m_pShinInfo->pEselect->nm_office, len );
			memmove ( ( *m_pSnHeadData )->Sn_ZMSYO, buf, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );

			// 税務署名
			len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_ZMSYO, sizeof ( ( *m_pSnHeadData )->Sn_ZMSYO ) );
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, ( *m_pSnHeadData )->Sn_ZMSYO, len );
			DiagData.data_edit = buf;
			m_Kani01Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "署番号 %-5.5s" ), ( *m_pSnHeadData )->Sn_TAXNO );
			DiagData.data_disp = buf;
			m_Kani01Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );
		}
	}
}


//=================================
// 集計セット
//=================================
int CShinKani01EX2::SyukeiDataSet ( int cID )
{
	DIAGRAM_DATA DiagData;
	char val[MONY_BUF_SIZE] = { 0 };

	DiagData.data_imgdata = NULL;
	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID3XM10:
		memmove ( XM10, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "331000" ), XM10 );
		break;

	//yoshida190625
	case ID3XM13:
		memmove(XM13, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("331300"), XM13);
		break;

	case ID3XM15:
		memmove(XM15, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("331500"), XM15);
		break;

	case ID3XM16:
		memmove(XM16, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("331600"), XM16);
		break;


	case ID3XM21:
		memmove ( XM21, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "332100" ), XM21 );
		break;


	//yoshida190625
	case ID3XM24:
		memmove(XM24, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("332400"), XM24);
		break;

	}

	

	return MemoryReget();
}

//=================================
// 集計データ取得
//=================================
int CShinKani01EX2::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray money;
	int cnt = 0, incnt = 0;

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "33" ) );
	cnt = ( int )money.GetCount();

	for ( int l = 0; l < cnt; l++ ) {
		incnt = money[l].GetCount();
		for ( int m = 0; m < incnt; m++ ) {
			if ( money[l][m].code == _T( "330100" ) ) {
				memmove ( XM1, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "330200" ) ) {
				memmove ( XM2, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330300" ) ) {
				memmove ( XM3, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330400" ) ) {
				memmove ( XM4, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330500" ) ) {
				memmove ( XM5, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330600" ) ) {
				memmove ( XM6, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330700" ) ) {
				memmove ( XM7, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330800" ) ) {
				memmove ( XM8, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "330900" ) ) {
				memmove ( XM9, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331000" ) ) {
				memmove ( XM10, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331100" ) ) {
				memmove ( XM11, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331200" ) ) {
				memmove ( XM12, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331300" ) ) {
				memmove ( XM13, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331400" ) ) {
				memmove ( XM14, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331500" ) ) {
				memmove ( XM15, money[l][m].arith, MONY_BUF_SIZE );
				if(((( CH31HyoView* )m_pParent )->IsIncludeMenzeiKikan() ) && !(( *m_pSnHeadData )->Sn_Sign4 & 0x80 )) {
					long symd = 0, eymd = 0;
					char money[MONY_BUF_SIZE] = { 0 };

					(( CH31HyoView* )m_pParent )->GetKazeiKikan( symd, eymd );
					(( CH31HyoView* )m_pParent )->MenzeiKikanSyukei( symd, eymd, money );
					m_pArith->l_add( XM15, XM15, money );
				}
			}
			else if  ( money[l][m].code == _T( "331600" ) ) {
				memmove ( XM16, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331700" ) ) {
				memmove ( XM17, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331800" ) ) {
				memmove ( XM18, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "331900" ) ) {
				memmove ( XM19, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "332000" ) ) {
				memmove ( XM20, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "332100" ) ) {
				memmove ( XM21, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "332200" ) ) {
				memmove ( XM22, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "332300" ) ) {
				memmove ( XM23, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "332400" ) ) {
				memmove ( XM24, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if  ( money[l][m].code == _T( "332500" ) ) {
				memmove ( XM25, money[l][m].arith, MONY_BUF_SIZE );
			}
			//else if  ( money[l][m].code == _T( "332600" ) ) {
			//	memmove ( XM26, money[l][m].arith, MONY_BUF_SIZE );
			//}


			if ((*m_pSnHeadData)->Sn_SKKBN == 3 || (*m_pSnHeadData)->Sn_SKKBN == 4) {//修正申告の場合
				if  ( money[l][m].code == _T( "332700" ) ) {
					memmove ( XM26, money[l][m].arith, MONY_BUF_SIZE );
				}
			}
			else {
				if  ( money[l][m].code == _T( "332600" ) ) {
					memmove ( XM26, money[l][m].arith, MONY_BUF_SIZE );
				}
			}
		}
	}

	return 0;
}

//===========================================
// 付表データ取得
//-------------------------------------------
// 返送値
//		0		:		正常終了
//===========================================
int CShinKani01EX2::GetFhyoData()
{
	// 2割特例の場合は課税売上割合は記載不要のためセットしない
	if( !((*m_pSnHeadData)->m_DispTabSgn & 0x2000) ){

		MoneyBasejagArray money;
		int cnt = 0, incnt = 0;

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );
		cnt = ( int )money.GetCount();

		for ( int l = 0; l < cnt; l++ ) {
			incnt = ( int )money[l].GetCount();
			for ( int m = 0; m < incnt; m++ ) {
				if ( money[l][m].code == _T( "357700" ) ) {
					memmove ( KURI1, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "35B100" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
					KUP1 = atoi ( wariai );
				}
				else if ( money[l][m].code == _T( "357800" ) ) {
					memmove ( KURI2, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "35B200" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
					KUP2 = atoi ( wariai );
				}
				else if ( money[l][m].code == _T( "357900" ) ) {
					memmove ( KURI3, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "35B300" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
					KUP3 = atoi ( wariai );
				}
				else if ( money[l][m].code == _T( "358000" ) ) {
					memmove ( KURI4, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "35B400" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
					KUP4 = atoi ( wariai );
				}
				else if ( money[l][m].code == _T( "358100" ) ) {
					memmove ( KURI5, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "35B500" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
					KUP5 = atoi ( wariai );
				}
				else if ( money[l][m].code == _T( "358200" ) ) {
					memmove ( KURI6, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "35B600" ) ) {
					char wariai[32] = { 0 };
					m_pArith->l_print ( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
					KUP6 = atoi ( wariai );
				}
			}
		}
	}
	else{
		KUP1 = KUP2 = KUP3 = KUP4 = KUP5 = KUP6 = 0;
		memset( KURI1, '\0', sizeof(KURI1) );
		memset( KURI2, '\0', sizeof(KURI2) );
		memset( KURI3, '\0', sizeof(KURI3) );
		memset( KURI4, '\0', sizeof(KURI4) );
		memset( KURI5, '\0', sizeof(KURI5) );
		memset( KURI6, '\0', sizeof(KURI6) );
	}

	return 0;
}

//============================================
// 付表データ表示
//============================================
void CShinKani01EX2::Disp_FhyoData()
{
	char CkMy[MONY_BUF_SIZE] = { 0 };

	char M1000[MONY_BUF_SIZE] = { 0 };
	m_pArith->l_input (M1000, _T( "1000" ) );

	// 第一種事業 課税売上
	memset ( CkMy, '\0', MONY_BUF_SIZE );

	m_Util.l_calq( CkMy, KURI1 , 2);//四捨五入
	m_pArith->l_div(CkMy, CkMy, M1000);
	dsp_prs ( IDC_ICSDIAG8CTRL3, ID4100, CkMy );

	// 第一種事業 売上割合
	dsp_uws ( IDC_ICSDIAG8CTRL3, ID4101, KUP1 );

	// 第二種事業 課税売上
	memset ( CkMy, '\0', MONY_BUF_SIZE );
	m_Util.l_calq(CkMy, KURI2, 2);//四捨五入
	m_pArith->l_div(CkMy, CkMy, M1000);
	dsp_prs ( IDC_ICSDIAG8CTRL3, ID4110, CkMy );

	// 第二種事業 売上割合
	dsp_uws ( IDC_ICSDIAG8CTRL3, ID4111, KUP2 );

	// 第三種事業 課税売上
	memset ( CkMy, '\0', MONY_BUF_SIZE );
	m_Util.l_calq(CkMy, KURI3, 2);//四捨五入
	m_pArith->l_div(CkMy, CkMy, M1000);
	dsp_prs ( IDC_ICSDIAG8CTRL3, ID4120, CkMy );

	// 第三種事業 売上割合
	dsp_uws ( IDC_ICSDIAG8CTRL3, ID4121, KUP3 );

	// 第四種事業 課税売上
	memset ( CkMy, '\0', MONY_BUF_SIZE );
	m_Util.l_calq(CkMy, KURI4, 2);//四捨五入
	m_pArith->l_div(CkMy, CkMy, M1000);
	dsp_prs ( IDC_ICSDIAG8CTRL3, ID4130, CkMy );

	// 第四種事業 売上割合
	dsp_uws ( IDC_ICSDIAG8CTRL3, ID4131, KUP4 );

	// 第五種事業 課税売上
	memset ( CkMy, '\0', MONY_BUF_SIZE );
	m_Util.l_calq(CkMy, KURI5, 2);//四捨五入
	m_pArith->l_div(CkMy, CkMy, M1000);
	dsp_prs ( IDC_ICSDIAG8CTRL3, ID4140, CkMy );

	// 第五種事業 売上割合
	dsp_uws ( IDC_ICSDIAG8CTRL3, ID4141, KUP5 );

	// 第六種事業 課税売上
	memset ( CkMy, '\0', MONY_BUF_SIZE );
	m_Util.l_calq(CkMy, KURI6, 2);//四捨五入
	m_pArith->l_div(CkMy, CkMy, M1000);
	dsp_prs ( IDC_ICSDIAG8CTRL3, ID4150, CkMy );

	// 第六種事業 売上割合
	dsp_uws ( IDC_ICSDIAG8CTRL3, ID4151, KUP6 );

	// 特例計算適用
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if( !(( CH31HyoView* )m_pParent )->IsNiwariTokurei() ) {
		int tcheck = ( int )( ( *m_pSnHeadData )->Sn_TOKUR & 0xff );
		if ( tcheck ) {
			DiagData.data_check = 1;
			m_Kani01Diag03.SetData ( ID4160, ( struct IUnknown* )&DiagData );
			DiagData.data_check = 0;
			m_Kani01Diag03.SetData ( ID4161, ( struct IUnknown* )&DiagData );
		}
		else {
			DiagData.data_check = 0;
			m_Kani01Diag03.SetData ( ID4160, ( struct IUnknown* )&DiagData );
			DiagData.data_check = 1;
			m_Kani01Diag03.SetData ( ID4161, ( struct IUnknown* )&DiagData );
		}
	}
}

//=================================================
// 売上割合表示
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		pers	:	金額
//=================================================
void CShinKani01EX2::dsp_uws ( unsigned short Id, short Index, unsigned short pers )
{
	if ( pers ) {
		char buf[128] = { 0 };
		DIAGRAM_DATA DiagData;
		DiagData.data_imgdata = NULL;

		sprintf_s ( buf, sizeof ( buf ), _T( "%3d.%01d" ), ( pers / 10 ), ( pers % 10 ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		dsp_cls ( Id, Index );
	}
}

//======================================
// 還付金融機関入力にフォーカスセット
//======================================
void CShinKani01EX2::SetKanpuKinyuInput()
{
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return;
	}

	m_Kani01Diag03.SetPosition ( ID4210 );

	CRect rc;
	GetClientRect ( &rc );
	InvalidateRect ( &rc );

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	int nowpos = 0;
	nowpos = m_Kani01Diag03.GetPosition();
	m_Kani01Diag03.GetRect ( nowpos, ( LPUNKNOWN )&rectB );

	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.bottom ) / 2 ) );
	}
}

//----> 2022/12/27
//===================================================
// 還付金融機関の表示・非表示
//---------------------------------------------------
// 引数
//		sw		:		SW_SHOW:表示 / SW_HIDE:非表示
//===================================================
void CShinKani01EX2::EnableKanpuKinyu( int sw )
{
	int id = 0;

	// 表示・非表示
	if( sw == SW_SHOW ){

		m_IsKanpuEnable = TRUE;

		int  len = 0;
		char buf[512] = {0};
		char cbuf[128] = {0};
		DIAGRAM_DATA DiagData;
		DiagData.data_imgdata = NULL;

		// 金融機関 名
		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_BANK, 20 );
		memmove ( buf, ( *m_pSnHeadData )->Sn_BANK, len );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4210, ( struct IUnknown* )&DiagData );

		// 銀行・金庫等
		DiagData.data_combo = ( short )( ( *m_pSnHeadData )->Sn_FINANCE & 0x0f );
		m_Kani01Diag03.SetData ( ID4212, ( struct IUnknown* )&DiagData );

		// 金融機関 支店名
		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_SITEN, 20 );
		memmove ( buf, ( *m_pSnHeadData )->Sn_SITEN, len );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4211, ( struct IUnknown* )&DiagData );

		// 本社・支店等
		DiagData.data_combo = ( short )( ( *m_pSnHeadData )->Sn_CHAIN & 0x0f );
		m_Kani01Diag03.SetData ( ID4213, ( struct IUnknown* )&DiagData );

		// 金融機関 預金種別
		memset ( buf, '\0', sizeof ( buf ) );
		len = kjlen ( ( *m_pSnHeadData )->Sn_YOKIN, 8 );
		memmove ( buf, ( *m_pSnHeadData )->Sn_YOKIN, len );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4214, ( struct IUnknown* )&DiagData );

		// 金融機関 口座番号
		memset ( buf, '\0', sizeof ( buf ) );
		if ( ( ( *m_pSnHeadData )->Sn_KOZA[0] & 0xff ) != 0xff ) {
			memmove ( cbuf, ( *m_pSnHeadData )->Sn_KOZA, 5 );
			m_pArith->l_unpac ( buf, cbuf, 10 );
			DiagData.data_edit = ( char* )buf;
			m_Kani01Diag03.SetData ( ID4215, ( struct IUnknown* )&DiagData );
		}
		else {
			dsp_cls ( IDC_ICSDIAG8CTRL3, ID4215 );
		}

		// 郵便局 名
		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_YUBIN, 16 );
		memmove ( buf, ( *m_pSnHeadData )->Sn_YUBIN, len );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4312, ( struct IUnknown* )&DiagData );

		// 銀行or郵便局
		if ( ( *m_pSnHeadData )->Sn_BYsw ) {
			DiagData.data_combo = 1;
		}
		else {
			DiagData.data_combo = 0;
		}
		m_Kani01Diag03.SetData ( ID4410, ( struct IUnknown* )&DiagData );

		// 郵便局 預金番号
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, ( *m_pSnHeadData )->Sn_YUNUM1, 5 );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4310, ( struct IUnknown* )&DiagData );

		// 郵便局 預金番号
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, ( *m_pSnHeadData )->Sn_YUNUM2, 8 );
		DiagData.data_edit = ( char* )buf;
		m_Kani01Diag03.SetData ( ID4311, ( struct IUnknown* )&DiagData );

	}
	else if( sw == SW_HIDE ){

		m_IsKanpuEnable = FALSE;

		// 金融機関
		for ( id = ID4210; id <= ID4215; id++ ) {
			dsp_cls( IDC_ICSDIAG8CTRL3, id );
		}
		// 郵便局
		for ( id = ID4310; id <= ID4312; id++ ) {
			dsp_cls( IDC_ICSDIAG8CTRL3, id );
		}
		// 還付先金融機関選択
		dsp_cls( IDC_ICSDIAG8CTRL3, ID4410 );
	}


	// 入力制御
	int Modify = 0;
	int colorSign = 0;

	if( sw == SW_SHOW ){
		Modify = DIAG_MDFY_EDIT;
		colorSign = 0;
	}
	else if( sw == SW_HIDE ){
		Modify = DIAG_MDFY_READONLY;
		colorSign = 2;
	}

	// 金融機関
	for ( id = ID4210; id <= ID4215; id++ ) {
		m_Kani01Diag03.ModifyItem ( id, Modify );
		ChangeColor( IDC_ICSDIAG8CTRL3, id, colorSign );
	}
	// 郵便局
	for ( id = ID4310; id <= ID4312; id++ ) {
		m_Kani01Diag03.ModifyItem ( id, Modify );
		ChangeColor( IDC_ICSDIAG8CTRL3, id, colorSign );
	}
	// 還付先金融機関選択
	m_Kani01Diag03.ModifyItem( ID4410, Modify );
	ChangeColor( IDC_ICSDIAG8CTRL3, ID4410, colorSign );


	m_Kani01Diag03.RedrawWindow();
}

//===================================================
// diagのチェックボックスに変更があった場合の処理
//===================================================
void CShinKani01EX2::CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	switch( index ){
		case ID2062:	// （個人の方）振替継続希望
		{
			DIAGRAM_DATA DiagData;
			DiagData.data_imgdata = NULL;
			m_Kani01Diag01.GetData( index, ( struct IUnknown* )&DiagData );
			DiagData.data_check = !DiagData.data_check;
			CChkMsg chkmsg;
			chkmsg.InitInfo( m_pSnHeadData, DiagData.data_check, 0 );
			HINSTANCE svhInstResource = AfxGetResourceHandle();
			AfxSetResourceHandle ( g_hInstance );
			if( chkmsg.DoModal() == IDOK ){
				if( chkmsg.m_ChkState ){
					(*m_pSnHeadData)->Sn_GenericSgn |= 0x200;
					DiagData.data_check = 1;
				}
				else{
					(*m_pSnHeadData)->Sn_GenericSgn &= ~0x200;
					DiagData.data_check =  0;
				}
			}
			AfxSetResourceHandle ( svhInstResource );
			m_Kani01Diag01.SetData( index, ( struct IUnknown* )&DiagData );
			m_Kani01Diag01.SetPosition( index );	// 描画切り替わらないため同じ個所にセット
			break;
		}
		default:
			break;
	}
}

//===================================================
// diagのチェックボックスに変更があった場合の処理
//===================================================
void CShinKani01EX2::CheckButtonIcsdiag8ctrl3(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	switch( index ){
		case ID4420:	// （個人の方）公金受取口座の利用
		{	
			DIAGRAM_DATA DiagData;
			DiagData.data_imgdata = NULL;

			// マイナンバー未入力の場合はその旨のメッセージを出す
			CString str_myno;
			if( ( *m_pSnHeadData )->GetMyNumber ( str_myno ) == 1 ){

				// チェックを付けた後にマイナンバーが削除されていた場合は、一旦確認メッセージを出す
				if( ( *m_pSnHeadData )->Sn_GenericSgn & 0x400 ){
					m_Kani01Diag03.GetData( index, ( struct IUnknown* )&DiagData );
					DiagData.data_check = !DiagData.data_check;
					CChkMsg chkmsg;
					chkmsg.InitInfo( m_pSnHeadData, DiagData.data_check, 1 );
					HINSTANCE svhInstResource = AfxGetResourceHandle();
					AfxSetResourceHandle ( g_hInstance );
					if( chkmsg.DoModal() == IDOK ){
						(*m_pSnHeadData)->Sn_GenericSgn &= ~0x400;
						EnableKanpuKinyu( SW_SHOW );
						DiagData.data_check = 0;
					}
					AfxSetResourceHandle ( svhInstResource );
				}
				else{
					ICSMessageBox( "「個人番号」の登録がないため、「公金受取口座の利用」は選択できません。" );
					DiagData.data_check = 0;
				}
				m_Kani01Diag03.SetData( index, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition( index );	// 描画切り替わらないため同じ個所にセット
			}
			// マイナンバー登録済みの場合は確認メッセージを出す
			else{
				m_Kani01Diag03.GetData( index, ( struct IUnknown* )&DiagData );
				DiagData.data_check = !DiagData.data_check;	
				CChkMsg chkmsg;
				chkmsg.InitInfo( m_pSnHeadData, DiagData.data_check, 1 );
				HINSTANCE svhInstResource = AfxGetResourceHandle();
				AfxSetResourceHandle ( g_hInstance );
				if( chkmsg.DoModal() == IDOK ){
					if( chkmsg.m_ChkState ){
						(*m_pSnHeadData)->Sn_GenericSgn |= 0x400;
						DiagData.data_check = 1;
						EnableKanpuKinyu( SW_HIDE );
					}
					else{
						(*m_pSnHeadData)->Sn_GenericSgn &= ~0x400;
						DiagData.data_check = 0;
						EnableKanpuKinyu( SW_SHOW );
					}
				}
				AfxSetResourceHandle ( svhInstResource );
				m_Kani01Diag03.SetData( index, ( struct IUnknown* )&DiagData );
				m_Kani01Diag03.SetPosition( index );	// 描画切り替わらないため同じ個所にセット
			}
			break;
		}
		default:
			break;
	}
}

//===================================================
// 準確定かどうか
//---------------------------------------------------
// 戻り値
//		TRUE:準確定 / FALSE:準確定じゃない
//===================================================
BOOL CShinKani01EX2::IsJunkakutei()
{
	if( ( *m_pSnHeadData )->Sn_SKKBN == 1 && ((*m_pSnHeadData)->Sn_Syuusei & 0xff) == 0x10 ){
		return TRUE;
	}
	return FALSE;
}
//<---- 2022/12/27