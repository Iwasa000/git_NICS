// ShinIppan02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinIppan02.h"
#include "ShinIppan02idx.h"
#include "H31HyoView.h"

// CShinIppan02 ダイアログ

IMPLEMENT_DYNAMIC(CShinIppan02, CSyzBaseDlg)

CShinIppan02::CShinIppan02(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinIppan02::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnHeadData ( NULL )
	, m_pSnHonpyo2Data ( NULL )
	, m_pArith ( NULL )
	, m_pShinInfo ( NULL )
	, m_pZmsub( NULL )
	, m_pSyzSyukei( NULL )
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

	m_curwnd = -1;
}

CShinIppan02::~CShinIppan02()
{
}

void CShinIppan02::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Ippan02Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Ippan02Diag02);
}


BEGIN_MESSAGE_MAP(CShinIppan02, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinIppan02 メッセージ ハンドラ

BOOL CShinIppan02::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	// 初期化
	Init();

	// ダイアグラム属性変更
	ChgDiagAttr();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 73;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinIppan02::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinIppan02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinIppan02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
//		pSnHonpyo2Data	：	本表データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-------------------------------------------------------------
// 返送値
//		0				:	正常終了
//		-1				:	エラー
//=============================================================
int CShinIppan02::InitInfo ( CSnHeadData **pSnHeadData, CH31SnHonpyo2Data **pSnHonpyo2Data, CArithEx *pArith, SYC_SHININFO *pShinInfo , CDBNpSub *pZmsub , CSyzSyukei **pSyukei )
{
	ASSERT( pSnHonpyo2Data );
	if( pSnHonpyo2Data == NULL ){
		return -1;
	}
	m_pSnHonpyo2Data = pSnHonpyo2Data;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT ( pArith );
	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT(pZmsub);
	if (pZmsub == NULL) {
		return -1;
	}
	m_pZmsub = pZmsub;

	ASSERT(pSyukei);
	if (pSyukei == NULL) {
		return -1;
	}
	m_pSyzSyukei = pSyukei;

	return 0;
}

//=====================================
// 初期化
//-------------------------------------
// 返送値
//		0	:	正常終了
//		-1	:	エラー
//=====================================
int CShinIppan02::Init()
{
	// ダイアグラム初期属性セット
	InitDiagAttr();

	// データ読込み
	ReadData();

	// 初期データ表示
	DispIppan2();

	// 初期ポジションセット
	SetItemPosition();

	return 0;
}

//=====================================
// ダイアグラム初期設定
//-------------------------------------
// 返送値
//		0	:	正常終了
//=====================================
int CShinIppan02::ChgDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Ippan1[cnt].Index == 0 )  {
			break;
		}

		int Flg = 0;
		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Ippan1[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Ippan1[cnt].UnConnect;
			}
			else {
				Flg = Ippan1[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Ippan1[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Ippan1[cnt].Connect );
		if ( ( Ippan1[cnt].Index == ID2030 ) || ( Ippan1[cnt].Index == ID2040 ) ) {
			m_Ippan02Diag01.ModifyItem ( Ippan1[cnt].Index, ( Flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY ) | DIAG_MODIFY_IMEKANA );
		}

		cnt++;
	}

	cnt = 0;
	while ( 1 ) {
		if ( Ippan2[cnt].Index == 0 ) {
			break;
		}

		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Ippan2[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Ippan2[cnt].UnConnect;
			}
			else {
				Flg = Ippan2[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Ippan2[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Ippan2[cnt].Connect );
		cnt++;
	}

// 220412 add -->
	// 令和四年四月一日以後終了課税期間分から特例計算（5-3）のチェック欄は非表示
	long	eymd=0;
	if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	{
		eymd = (*m_pSnHeadData)->Sn_MDAYE;
	}
	else	{
		eymd = (*m_pSnHeadData)->Sn_KDAYE;
	}
	m_Tok53Mod=FALSE;
	if(eymd >= TOK53MODIFY)	{
		m_Ippan02Diag01.ModifyItem(14,DIAG_MDFY_TRANSPARENT);
		m_Ippan02Diag01.ModifyItem(ID2062,DIAG_MDFY_TRANSPARENT);
		m_Tok53Mod = TRUE;
	}
// 220412 add <--

	return 0;
}

//=====================================
// ダイアログ初期属性セット
//-------------------------------------
// 返送値
//		0	:	正常終了
//		-1	:	エラー
//=====================================
int CShinIppan02::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE DA;

	// 納税地 住所(上)
	m_Ippan02Diag01.GetAttribute ( ID2010, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Ippan02Diag01.SetAttribute ( ID2010, ( LPUNKNOWN )&DA, TRUE );

	// 納税地 住所(下)
	m_Ippan02Diag01.GetAttribute ( ID2011, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Ippan02Diag01.SetAttribute ( ID2011, ( LPUNKNOWN )&DA, TRUE );

	// 氏名・屋号
	m_Ippan02Diag01.GetAttribute ( ID2030, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Ippan02Diag01.SetAttribute ( ID2030, ( LPUNKNOWN )&DA, TRUE );

	// 被合併名
	m_Ippan02Diag01.GetAttribute ( ID2032, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Ippan02Diag01.SetAttribute ( ID2032, ( LPUNKNOWN )&DA, TRUE );

	// 代表者氏名
	m_Ippan02Diag01.GetAttribute ( ID2041, ( LPUNKNOWN )&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Ippan02Diag01.SetAttribute ( ID2041, ( LPUNKNOWN )&DA, TRUE );

	return 0;
}

//=====================================
// 初期データ表示
//=====================================
void CShinIppan02::DispIppan2()
{
	DIAGRAM_ATTRIBUTE DA;
	int Index = 0;

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char buf[512] = { 0 };

	// 納税地 郵便番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2000, ( struct IUnknown* )&DiagData );

	// 納税地 住所(上)
	memset ( buf, '\0', sizeof ( buf ) );
	int len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_NOZEI, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );

	// 納税地 住所(下)
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )&( *m_pSnHeadData )->Sn_NOZEI[40], 40 );
	memmove ( buf, &( *m_pSnHeadData )->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );

	// 納税地 電話番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO1, sizeof ( ( *m_pSnHeadData )->Sn_TELNO1 ) );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2020, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO2, sizeof ( ( *m_pSnHeadData )->Sn_TELNO2 ) );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2021, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO3, sizeof ( ( *m_pSnHeadData )->Sn_TELNO3 ) );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2022, ( struct IUnknown* )&DiagData );

	// 氏名・屋号 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_COFRI, 80 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2030, ( struct IUnknown* )&DiagData );

	// 氏名・屋号
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_CONAM, sizeof ( ( *m_pSnHeadData )->Sn_CONAM ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2031, ( struct IUnknown* )&DiagData );

	// 被合併名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_GPNAM, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2032, ( struct IUnknown* )&DiagData );

	// 代表者氏名 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIFRI, 68 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2040, ( struct IUnknown* )&DiagData );

	// 代表者氏名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIHYO, sizeof ( ( *m_pSnHeadData )->Sn_DIHYO ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2041, ( struct IUnknown* )&DiagData );

	// 整理番号
	if ( ( ( *m_pSnHeadData )->Sn_SEIRI[0] & 0xff ) != 0xff ) {
		memset ( buf, '\0', sizeof ( buf ) );
		m_pArith->l_unpac ( buf, ( *m_pSnHeadData )->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_Ippan02Diag01.SetData ( ID2050, ( struct IUnknown* )&DiagData );
	}

	// 課税期間
	char yy = 0, mm = 0, dd = 0;
	int gengo = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd , &gengo);
	if ( ( yy & 0xff ) &&  ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2100, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2101, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2102, ( struct IUnknown* )&DiagData );

		DiagData.data_disp.Empty();
		if( gengo == ID_ICSSH_REIWA_GENGO ){
			DiagData.data_disp = SYZ31_REIWA_G_STR;
		}
		else if( gengo == ID_ICSSH_HEISEI_GENGO ){
			DiagData.data_disp = SYZ31_HEISEI_G_STR;
		}
		m_Ippan02Diag01.SetData ( ID2103, ( struct IUnknown* )&DiagData );
	}

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	if ( ( yy & 0xff ) &&  ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2110, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2111, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2112, ( struct IUnknown* )&DiagData );
	}

	// 申告区分
	memset ( buf, '\0', sizeof ( buf ) );
	switch ( ( *m_pSnHeadData )->Sn_SKKBN ) {
	case 1:
		if ( ( *m_pSnHeadData )->Sn_Syuusei & 0x10 ) {
			memmove ( buf, _T( "準確定" ), 8 );
		}
		else {
			memmove ( buf, _T( "　確定　" ), 8 );
		}
		break;
	case 2:
		memmove ( buf, _T( "　中間　" ), 8 );
		break;
	case 3:
		memmove ( buf, _T( "修正確定" ), 8 );
		break;
	default:
		memmove ( buf, _T( "修正中間" ), 8 );
		break;
	}
	DiagData.data_disp = buf;
	m_Ippan02Diag01.SetData ( ID2120, ( struct IUnknown* )&DiagData );

	// 中間申告
	if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) ) {
		yy = mm = dd = 0;
		gengo = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd , &gengo);
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2130, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2131, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2132, ( struct IUnknown* )&DiagData );

			DiagData.data_disp.Empty();
			if( gengo == ID_ICSSH_REIWA_GENGO ){
				DiagData.data_disp = SYZ31_REIWA_G_STR;
			}
			else if( gengo == ID_ICSSH_HEISEI_GENGO ){
				DiagData.data_disp = SYZ31_HEISEI_G_STR;
			}
			m_Ippan02Diag01.SetData ( ID2133, ( struct IUnknown* )&DiagData );
		}

		yy = mm = dd = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd );
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2140, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2141, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2142, ( struct IUnknown* )&DiagData );
		}
	}
	else{
		DiagData.data_disp.Empty();
		if( gengo == ID_ICSSH_REIWA_GENGO ){
			DiagData.data_disp = SYZ31_REIWA_G_STR;
		}
		else if( gengo == ID_ICSSH_HEISEI_GENGO ){
			DiagData.data_disp = SYZ31_HEISEI_G_STR;
		}
		m_Ippan02Diag01.SetData ( ID2133, ( struct IUnknown* )&DiagData );
	}

	EnumIdIcsShKazeihoushiki	KzHoushiki = m_Util.GetKazeihoushiki(m_pZmsub->zvol);

	//yoshida190704
	// 軽減売上割合(10営業日)
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) {
		if ((*m_pSnHeadData)->m_DispTabSgn & 0x10) {
			DiagData.data_check = 1;
		}
		else {
			DiagData.data_check = 0;
		}
		m_Ippan02Diag01.SetData(ID2060, (struct IUnknown*)&DiagData);
	

		// 小売等軽減仕入割合
		if (KzHoushiki == ID_ICSSH_GENSOKU) {	// 原則課税
			if ((*m_pSnHeadData)->m_DispTabSgn & 0x20) {
				DiagData.data_check = 1;
			}
			else {
				DiagData.data_check = 0;
			}
		}
		else { //簡易課税 
			DiagData.data_check = 0;
		}
		m_Ippan02Diag01.SetData(ID2061, (struct IUnknown*)&DiagData);
	

		// 小売等軽減売上割合
		if (KzHoushiki == ID_ICSSH_GENSOKU) {	// 原則課税
			if ((*m_pSnHeadData)->m_DispTabSgn & 0x40){
				DiagData.data_check = 1;
			}
			else {
				DiagData.data_check = 0;
			}
		}
		else { //簡易課税 
			DiagData.data_check = 0;
		}
		m_Ippan02Diag01.SetData(ID2062, (struct IUnknown*)&DiagData);
	}
	else {
		DiagData.data_check = 0;
		m_Ippan02Diag01.SetData ( ID2060, ( struct IUnknown* )&DiagData );
		m_Ippan02Diag01.SetData ( ID2061, ( struct IUnknown* )&DiagData );
		m_Ippan02Diag01.SetData ( ID2062, ( struct IUnknown* )&DiagData );
	}

	// 金額表示
	GetFhyoData();

	SetDiagData();
}

//================================================
// データの読込み
//------------------------------------------------
// 返送値
//		0		:		正常終了
//================================================
int CShinIppan02::ReadData()
{
	// 課税標準額等の内訳書には入力項目がないが、ロック時に集計を行わないためデータを取得しておく
	// 課税標準額
	memmove ( XM1, ( *m_pSnHonpyo2Data )->Sn_XM1, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 3%適用分
	memmove ( XM2, ( *m_pSnHonpyo2Data )->Sn_XM2, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 4%適用分
	memmove ( XM3, ( *m_pSnHonpyo2Data )->Sn_XM3, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 6.3%適用分
	memmove ( XM4, ( *m_pSnHonpyo2Data )->Sn_XM4, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 6.24%適用分
	memmove ( XM5, ( *m_pSnHonpyo2Data )->Sn_XM5, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 7.8%適用分
	memmove ( XM6, ( *m_pSnHonpyo2Data )->Sn_XM6, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( XM7, ( *m_pSnHonpyo2Data )->Sn_XM7, MONY_BUF_SIZE );

	// 特定課税仕入れに係る支払い対価の額 6.3%適用分
	memmove ( XM8, ( *m_pSnHonpyo2Data )->Sn_XM8, MONY_BUF_SIZE );

	// 特定課税仕入れに係る支払い対価の額 7.8%適用分
	memmove ( XM9, ( *m_pSnHonpyo2Data )->Sn_XM9, MONY_BUF_SIZE );

	// 特定課税仕入れに係る支払い対価の額
	memmove ( XM10, ( *m_pSnHonpyo2Data )->Sn_XM10, MONY_BUF_SIZE );

	// 消費税額
	memmove ( XM11, ( *m_pSnHonpyo2Data )->Sn_XM11, MONY_BUF_SIZE );

	// 11の内訳 3%適用分
	memmove ( XM12, ( *m_pSnHonpyo2Data )->Sn_XM12, MONY_BUF_SIZE );

	// 11の内訳 4%適用分
	memmove ( XM13, ( *m_pSnHonpyo2Data )->Sn_XM13, MONY_BUF_SIZE );

	// 11の内訳 6.3%適用分
	memmove ( XM14, ( *m_pSnHonpyo2Data )->Sn_XM14, MONY_BUF_SIZE );

	// 11の内訳 6.24%適用分
	memmove ( XM15, ( *m_pSnHonpyo2Data )->Sn_XM15, MONY_BUF_SIZE );

	// 11の内訳 7.8%適用分
	memmove ( XM16, ( *m_pSnHonpyo2Data )->Sn_XM16, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( XM17, ( *m_pSnHonpyo2Data )->Sn_XM17, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	memmove ( XM18, ( *m_pSnHonpyo2Data )->Sn_XM18, MONY_BUF_SIZE );

	// 特定課税仕入れの返還等対価に係る税額
	memmove ( XM19, ( *m_pSnHonpyo2Data )->Sn_XM19, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額
	memmove ( XM20, ( *m_pSnHonpyo2Data )->Sn_XM20, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 4%適用分
	memmove ( XM21, ( *m_pSnHonpyo2Data )->Sn_XM21, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 6.3%適用分
	memmove ( XM22, ( *m_pSnHonpyo2Data )->Sn_XM22, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 6.24%及び7.8%適用分
	memmove ( XM23, ( *m_pSnHonpyo2Data )->Sn_XM23, MONY_BUF_SIZE );

	return 0;
}

//================================================
// データの書込み
//------------------------------------------------
// 返送値
//		0		:		正常終了
//================================================
int CShinIppan02::WriteData()
{
	// 課税標準額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM1, XM1, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 3%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM2, XM2, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 4%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM3, XM3, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 6.3%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM4, XM4, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 6.24%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM5, XM5, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額 7.8%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM6, XM6, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM7, XM7, MONY_BUF_SIZE );

	// 特定課税仕入れに係る支払い対価の額 6.3%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM8, XM8, MONY_BUF_SIZE );

	// 特定課税仕入れに係る支払い対価の額 7.8%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM9, XM9, MONY_BUF_SIZE );

	// 特定課税仕入れに係る支払い対価の額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM10, XM10, MONY_BUF_SIZE );

	// 消費税額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM11, XM11, MONY_BUF_SIZE );

	// 11の内訳 3%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM12, XM12, MONY_BUF_SIZE );

	// 11の内訳 4%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM13, XM13, MONY_BUF_SIZE );

	// 11の内訳 6.3%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM14, XM14, MONY_BUF_SIZE );

	// 11の内訳 6.24%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM15, XM15, MONY_BUF_SIZE );

	// 11の内訳 7.8%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM16, XM16, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM17, XM17, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM18, XM18, MONY_BUF_SIZE );

	// 特定課税仕入れの返還等対価に係る税額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM19, XM19, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM20, XM20, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 4%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM21, XM21, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 6.3%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM22, XM22, MONY_BUF_SIZE );

	// 地方消費税の課税標準となる消費税額 6.24%及び7.8%適用分
	memmove ( ( *m_pSnHonpyo2Data )->Sn_XM23, XM23, MONY_BUF_SIZE );

	return 0;
}

//================================================
// 金額表示
//------------------------------------------------
// 引数
//		Id		:		ダイアグラムID
//		Index	:		ダイアグラムインデックス
//		Mony	:		金額
//================================================
void CShinIppan02::dsp_prs ( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( Id == IDC_ICSDIAG8CTRL2 ) {
		switch ( Index ) {
		case ID3000:
		case ID3100:
		case ID3101:
		case ID3102:
		case ID3103:
		case ID3104:
		case ID3105:
		case ID3110:
		case ID3111:
		case ID3112:
		case ID3200:
		case ID3210:
		case ID3211:
		case ID3212:
		case ID3213:
		case ID3214:
		case ID3300:
		case ID3310:
		case ID3311:
		case ID3400:
		case ID3401:
		case ID3402:
		case ID3403:
			memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
			m_Ippan02Diag02.SetData ( Index, ( struct IUnknown* )&DiagData );
			break;
		default:
			break;
		}
	}
}

//================================================
// 表示金額クリア
//------------------------------------------------
// 引数
//		Id		:		ダイアグラムID
//		Index	:		ダイアグラムインデックス
//================================================
void CShinIppan02::dsp_cls ( unsigned short Id, short Index )
{
	if ( Id == IDC_ICSDIAG8CTRL1 ) {	// 基本情報
		switch ( Index ) {
			case ID2000:	// 郵便番号
			case ID2010:	// 納税地 上段
			case ID2011:	// 納税地 下段
			case ID2020:	// 電話番号 市外
			case ID2021:	// 電話番号 市内
			case ID2022:	// 電話番号 局番
			case ID2030:	// 名称又は屋号 フリガナ
			case ID2031:	// 名称又は屋号
			case ID2032:	// 被合併名
			case ID2040:	// 代表者又は氏名 フリガナ
			case ID2041:	// 代表者又は氏名
				m_Ippan02Diag01.DataClear ( Index, TRUE );
				break;
			case ID2050:	// 整理番号
				memset ( ( *m_pSnHeadData )->Sn_SEIRI, '\0', sizeof ( ( *m_pSnHeadData )->Sn_SEIRI ) );
				m_Ippan02Diag01.DataClear ( Index, TRUE );
			default:
				break;

		}
	}
}

//=============================================
// 初期ポジション
//=============================================
void CShinIppan02::SetItemPosition()
{
	int index = 0;

	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( ( index = m_Ippan02Diag01.GetPosition() ) != -1 ) {
			m_Ippan02Diag01.SetPosition ( index );
			m_Ippan02Diag01.SetFocus();
			return;
		}
	}
	else if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		if ( ( index = m_Ippan02Diag02.GetPosition() ) != -1 ) {
			m_Ippan02Diag02.SetPosition ( index );
			m_Ippan02Diag02.SetFocus();
			return;
		}
	}

	m_Ippan02Diag02.SetFocus();
	m_Ippan02Diag01.SetFocus();

	// 確定時は全入力不可
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return;
	}

	// 再描画
	CRect	rc;
	this->GetClientRect ( &rc );
	this->InvalidateRect ( &rc );

	SetRedrawScroll ( 0 );

	m_Ippan02Diag01.SetPosition(ID2060);
}

//====================================
// 本表上部表示
//====================================
void CShinIppan02::DispIppan2Upper()
{
	DIAGRAM_ATTRIBUTE DA;
	int Index = 0;

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char buf[512] = { 0 };

	// 納税地 郵便番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2000, ( struct IUnknown* )&DiagData );

	// 納税地 住所(上)
	memset ( buf, '\0', sizeof ( buf ) );
	int len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_NOZEI, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2010, ( struct IUnknown* )&DiagData );

	// 納税地 住所(下)
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )&( *m_pSnHeadData )->Sn_NOZEI[40], 40 );
	memmove ( buf, &( *m_pSnHeadData )->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2011, ( struct IUnknown* )&DiagData );

	// 納税地 電話番号
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO1, sizeof ( ( *m_pSnHeadData )->Sn_TELNO1 ) );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2020, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO2, sizeof ( ( *m_pSnHeadData )->Sn_TELNO2 ) );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2021, ( struct IUnknown* )&DiagData );
	memset ( buf, '\0', sizeof ( buf ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_TELNO3, sizeof ( ( *m_pSnHeadData )->Sn_TELNO3 ) );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2022, ( struct IUnknown* )&DiagData );

	// 氏名・屋号 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_COFRI, 80 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2030, ( struct IUnknown* )&DiagData );

	// 氏名・屋号
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_CONAM, sizeof ( ( *m_pSnHeadData )->Sn_CONAM ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2031, ( struct IUnknown* )&DiagData );

	// 被合併名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_GPNAM, 40 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2032, ( struct IUnknown* )&DiagData );

	// 代表者氏名 フリガナ
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIFRI, 68 );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2040, ( struct IUnknown* )&DiagData );

	// 代表者氏名
	memset ( buf, '\0', sizeof ( buf ) );
	len = m_Util.MixSpaceCutLength ( ( char* )( *m_pSnHeadData )->Sn_DIHYO, sizeof ( ( *m_pSnHeadData )->Sn_DIHYO ) );
	memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_Ippan02Diag01.SetData ( ID2041, ( struct IUnknown* )&DiagData );

	// 整理番号
	if ( ( ( *m_pSnHeadData )->Sn_SEIRI[0] & 0xff ) != 0xff ) {
		memset ( buf, '\0', sizeof ( buf ) );
		m_pArith->l_unpac ( buf, ( *m_pSnHeadData )->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_Ippan02Diag01.SetData ( ID2050, ( struct IUnknown* )&DiagData );
	}

	// 課税期間
	char yy = 0, mm = 0, dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	if ( ( yy & 0xff ) &&  ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2100, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2101, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2102, ( struct IUnknown* )&DiagData );
	}

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	if ( ( yy & 0xff ) &&  ( mm & 0xff ) && ( dd & 0xff ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2110, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2111, ( struct IUnknown* )&DiagData );
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
		DiagData.data_disp = buf;
		m_Ippan02Diag01.SetData ( ID2112, ( struct IUnknown* )&DiagData );
	}

	// 申告区分
	memset ( buf, '\0', sizeof ( buf ) );
	switch ( ( *m_pSnHeadData )->Sn_SKKBN ) {
	case 1:
		if ( ( *m_pSnHeadData )->Sn_Syuusei & 0x10 ) {
			memmove ( buf, _T( "準確定" ), 8 );
		}
		else {
			memmove ( buf, _T( "　確定　" ), 8 );
		}
		break;
	case 2:
		memmove ( buf, _T( "　中間　" ), 8 );
		break;
	case 3:
		memmove ( buf, _T( "修正確定" ), 8 );
		break;
	default:
		memmove ( buf, _T( "修正中間" ), 8 );
		break;
	}
	DiagData.data_disp = buf;
	m_Ippan02Diag01.SetData ( ID2120, ( struct IUnknown* )&DiagData );

	// 中間申告
	if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) ) {
		yy = mm = dd = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd );
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2130, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2131, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2132, ( struct IUnknown* )&DiagData );
		}

		yy = mm = dd = 0;
		( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd );
		if ( ( yy & 0xff ) && ( mm & 0xff ) && ( dd & 0xff ) ) {
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), yy );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2140, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), mm );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2141, ( struct IUnknown* )&DiagData );
			memset ( buf, '\0', sizeof ( buf ) );
			sprintf_s ( buf, sizeof ( buf ), _T( "%02x" ), dd );
			DiagData.data_disp = buf;
			m_Ippan02Diag01.SetData ( ID2142, ( struct IUnknown* )&DiagData );
		}
	}
}

BEGIN_EVENTSINK_MAP(CShinIppan02, CSyzBaseDlg)
	ON_EVENT(CShinIppan02, IDC_ICSDIAG8CTRL1, 1, CShinIppan02::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinIppan02, IDC_ICSDIAG8CTRL2, 1, CShinIppan02::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinIppan02, IDC_ICSDIAG8CTRL1, 3, CShinIppan02::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinIppan02, IDC_ICSDIAG8CTRL1, 2, CShinIppan02::EditOFFIcsdiag8ctrl1, VTS_I2)
END_EVENTSINK_MAP()


void CShinIppan02::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;
}


void CShinIppan02::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

BOOL CShinIppan02::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {

		if ( pMsg->wParam == VK_END ) {

			// 課税標準額等の内訳書更新
			WriteData();

			if ( m_pParent ) {
				m_pParent->PostMessageA ( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}



int CShinIppan02::GetFhyoData()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	BOOL isKanpu = FALSE;
	BOOL isNoneForm = FALSE;
	isNoneForm = ( ( CH31HyoView* )m_pParent )->IsNoneMTForm();

	EnumIdIcsShKazeihoushiki	KzHoushiki = m_Util.GetKazeihoushiki( m_pZmsub->zvol );

	MoneyBasejagArray	money;

	if ( KzHoushiki == ID_ICSSH_GENSOKU && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000) ) {
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "313500" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			isKanpu = TRUE;
		}

		money = (*m_pSyzSyukei)->GetShinkokuData( _T("31") );
	}
	else {
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "342900" ) );
		if ( m_pArith->l_test ( money[0][0].arith ) ) {
			isKanpu = TRUE;
		}

		money = (*m_pSyzSyukei)->GetShinkokuData( _T("34") );
	}

	int cnt = (int)money.GetCount();
	int incnt = 0;
	for (int i = 0; i < cnt; i++ ) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++ ) {

			if( KzHoushiki == ID_ICSSH_GENSOKU && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000) ){	// 原則課税
				if(money[i][j].code == _T("312100")){
					memmove(XM1, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if(money[i][j].code == _T("310201")){
					memmove(XM2, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310202")) {
					memmove(XM3, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310203")) {
					memmove(XM4, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312202")) {
					memmove(XM5, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312203")) {
					memmove(XM6, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312200")) {
					memmove(XM7, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310303")) {
					memmove(XM8, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312303")) {
					memmove(XM9, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312300")) {
					memmove(XM10, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312400")) {
					memmove(XM11, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310401")) {
					memmove(XM12, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310402")) {
					memmove(XM13, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310403")) {
					memmove(XM14, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312402")) {
					memmove(XM15, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312403")) {
					memmove(XM16, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312700")) {
					memmove(XM17, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312800")) {
					memmove(XM18, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("312900")) {
					memmove(XM19, money[i][j].arith, MONY_BUF_SIZE);
				}
				//else if (money[i][j].code == _T("313700")) {
				//	if ( isNoneForm == FALSE ) {
				//		memmove(XM20, money[i][j].arith, MONY_BUF_SIZE);
				//	}
				//}
				else if (money[i][j].code == _T("311702")) {
					memmove(XM21, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311703")) {
					memmove(XM22, money[i][j].arith, MONY_BUF_SIZE);
				}
				/*else if (money[i][j].code == _T("313703")) {
					if ( isNoneForm == FALSE ) {
						memmove(XM23, money[i][j].arith, MONY_BUF_SIZE);
					}
				}
				else if ( money[i][j].code == _T( "313600" ) ) {
					if ( isNoneForm ) {
						if ( m_pArith->l_test ( money[i][j].arith ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );
						}
					}
				}
				else if ( money[i][j].code == _T( "313500" ) ) {
					if ( isNoneForm ) {
						if ( m_pArith->l_test ( money[i][j].arith ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );

							// 還付の場合はマイナスを付ける
							m_pArith->l_neg ( XM20 );
							m_pArith->l_neg ( XM23 );
						}
					}
				}*/
				if ( isNoneForm ) {
					if ( isKanpu ) {
						if ( money[i][j].code == _T( "313500" ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );

							// 還付の場合はマイナスを付ける
							m_pArith->l_neg ( XM20 );
							m_pArith->l_neg ( XM23 );
						}
					}
					else {
						if ( money[i][j].code == _T( "313600" ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );
						}
					}

				}
				else {
					if ( money[i][j].code == _T( "313700" ) ) {
						memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
					}
					else if ( money[i][j].code == _T( "313703" ) ) {
						memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );
					}
				}
			}
			//else if( KzHoushiki == ID_ICSSH_KANNI ){	// 簡易課税
			else{
				if (money[i][j].code == _T("341800")) {
					memmove(XM1, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("340201")) {
					memmove(XM2, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("340202")) {
					memmove(XM3, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("340203")) {
					memmove(XM4, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("341902")) {
					memmove(XM5, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("341903")) {
					memmove(XM6, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("341900")) {
					memmove(XM7, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("342000")) {
					memmove(XM11, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("340301")) {
					memmove(XM12, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("340302")) {
					memmove(XM13, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("340303")) {
					memmove(XM14, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("342002")) {
					memmove(XM15, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("342003")) {
					memmove(XM16, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("342300")) {
					memmove(XM17, money[i][j].arith, MONY_BUF_SIZE);
					memmove(XM18, money[i][j].arith, MONY_BUF_SIZE);
				}
				//else if (money[i][j].code == _T("343100")) {
				//	if ( isNoneForm == FALSE ) {
				//		memmove(XM20, money[i][j].arith, MONY_BUF_SIZE);
				//	}
				//}
				else if (money[i][j].code == _T("341402")) {
					memmove(XM21, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("341403")) {
					memmove(XM22, money[i][j].arith, MONY_BUF_SIZE);
				}
				/*else if (money[i][j].code == _T("343103")) {
					if ( isNoneForm == FALSE ) {
						memmove(XM23, money[i][j].arith, MONY_BUF_SIZE);
					}
				}
				else if ( money[i][j].code == _T( "343000" ) ) {
					if ( isNoneForm ) {
						if ( m_pArith->l_test ( money[i][j].arith ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );
						}
					}
				}
				else if ( money[i][j].code == _T( "342900" ) ) {
					if ( isNoneForm ) {
						if ( m_pArith->l_test ( money[i][j].arith ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );

							// 還付の場合はマイナスを付ける
							m_pArith->l_neg ( XM20 );
							m_pArith->l_neg ( XM23 );
						}
					}
				}*/

				if ( isNoneForm ) {
					if ( isKanpu ) {
						if ( money[i][j].code == _T( "342900" ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );

							// 還付の場合はマイナスを付ける
							m_pArith->l_neg ( XM20 );
							m_pArith->l_neg ( XM23 );
						}
					}
					else {
						if ( money[i][j].code == _T( "343000" ) ) {
							memmove ( XM20, money[i][j].arith, MONY_BUF_SIZE );
							memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );
						}
					}
				}
				else {
					if ( money[i][j].code == _T( "343100" ) ) {
						memmove(XM20, money[i][j].arith, MONY_BUF_SIZE);
					}
					else if ( money[i][j].code == _T( "343103" ) ) {
						memmove ( XM23, money[i][j].arith, MONY_BUF_SIZE );
					}
				}
			}
		}
	}

	// 中間申告かつ地方消費税の課税標準となる消費税額の合計がマイナスまたは０の場合、
	// 20～23の項目を空欄にする
	if ( !( ( ( *m_pSnHeadData )->Sn_SKKBN & 0xff ) % 2 ) && ( m_pArith->l_test ( XM20 ) <= 0 ) ) {
		m_pArith->l_clear ( XM20 );
		m_pArith->l_clear ( XM21 );
		m_pArith->l_clear ( XM22 );
		m_pArith->l_clear ( XM23 );
	}

	return 0;
}

void CShinIppan02::SetDiagData()
{
	// 課税標準額
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3000, XM1 );

	// 課税資産の譲渡等の対価の額
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3100, XM2 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3101, XM3 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3102, XM4 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3103, XM5 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3104, XM6 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3105, XM7 );

	// 特定課税仕入れに係る支払い対価の額
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3110, XM8 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3111, XM9 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3112, XM10 );

	// 消費税額
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3200, XM11 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3210, XM12 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3211, XM13 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3212, XM14 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3213, XM15 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3214, XM16 );

	// 返還等対価に係る税額
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3300, XM17 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3310, XM18 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3311, XM19 );

	// 地方消費税の課税標準となる消費税額
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3400, XM20 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3401, XM21 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3402, XM22 );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID3403, XM23 );
}

void CShinIppan02::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	if (nChar == 0) {
		return;
	}

	short wTerm = 0;
	char VK_FLG = 0x00;
// 220412 add -->
	short	idx=0;
// 220412 add <--
	VK_FLG = (char)::GetKeyState(VK_SHIFT);
	if (nChar == VK_TAB) {
		if (VK_FLG & 0x80) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	switch( index ){
		case ID2060:
			// 次項目
			if( (wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3) || (wTerm == VK_DOWN) ){
				m_Ippan02Diag01.SetPosition(ID2061);
			}

			// 前項目
			if( (wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2) || (wTerm == VK_UP) ){
// 220412 del -->
				//m_Ippan02Diag01.SetPosition(ID2062);
// 220412 del <--
// 220412 add -->
				// 入力可になることはないが一応対応しておく
				if(m_Tok53Mod == FALSE)	idx=ID2062;
				else					idx=ID2061;
				m_Ippan02Diag01.SetPosition(idx);
// 220412 add <--
			}
			break;
		case ID2061:
			// 次項目
			if ((wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3) || (wTerm == VK_DOWN)) {
// 220412 del -->
				//m_Ippan02Diag01.SetPosition(ID2062);
// 220412 del <--
// 220412 add -->
				if(m_Tok53Mod == FALSE)	idx=ID2062;
				else					idx=ID2060;
				m_Ippan02Diag01.SetPosition(idx);
// 220412 add <--
			}

			// 前項目
			if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2) || (wTerm == VK_UP)) {
				m_Ippan02Diag01.SetPosition(ID2060);
			}
			break;
		case ID2062:
			// 次項目
			if ((wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3) || (wTerm == VK_DOWN)) {
				m_Ippan02Diag01.SetPosition(ID2060);
			}

			// 前項目
			if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2) || (wTerm == VK_UP)) {
				m_Ippan02Diag01.SetPosition(ID2061);
			}
			break;
		default:
			break;
	}
}


void CShinIppan02::EditOFFIcsdiag8ctrl1(short index)
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, index, (struct IUnknown*)&DiagData, CIcsdiagctrl);

	switch( index ){
		case ID2060:
			FUSOKU381 = DiagData.data_check;
			break;
		case ID2061:
			FUSOKU382 = DiagData.data_check;
			break;
		case ID2062:
			FUSOKU383 = DiagData.data_check;
			break;
		default:
			break;
	}
}
