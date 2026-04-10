// ChangeTax.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ChangeTax.h"
#include "ChangeTaxIdx.h"

// CChangeTax ダイアログ

IMPLEMENT_DYNAMIC(CChangeTax, ICSDialog)

CChangeTax::CChangeTax(CWnd* pParent /*=NULL*/)
	: ICSDialog(CChangeTax::IDD, pParent)
	, m_pSnHeadData ( NULL )
	, m_pSnTanaData ( NULL )
	, m_CbSw ( 0 )
	, m_ChSw ( 0 )
	, m_CbSt ( 0 )
	, m_pArith ( NULL )
{
	memset ( MNK40, '\0', sizeof ( MNK40 ) );
	memset ( MNZ40, '\0', sizeof ( MNZ40 ) );
	memset ( MENZK, '\0', sizeof ( MENZK ) );
	memset ( MENZZ, '\0', sizeof ( MENZZ ) );
	memset ( MNK63, '\0', sizeof ( MNK63 ) );
	memset ( MNZ63, '\0', sizeof ( MNZ63 ) );

	tansw = NULL;

	tmpChSw = 0;
}

CChangeTax::~CChangeTax()
{
}

void CChangeTax::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_ICSDIAG1);
	DDX_Control(pDX, IDC_STA1, m_txt1);
}


BEGIN_MESSAGE_MAP(CChangeTax, ICSDialog)
END_MESSAGE_MAP()


// CChangeTax メッセージ ハンドラ

BOOL CChangeTax::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	//	TODO:  ここに初期化を追加してください
	Init();

	ICSDialog::OnInitDialogEX();

	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

//==========================================
// 初期情報のセット
//------------------------------------------
// 引数
//	pSnHeadData		:	本表ヘッダデータ
//	pSnTanaData		:	棚卸調整データ
//	pArith			:	倍長演算
//------------------------------------------
// 返送値
//		0			:	正常終了
//		-1			:	エラー
//==========================================
int CChangeTax::InitInfo ( CSnHeadData *pSnHeadData, CH31SnTanaData *pSnTanaData, CArithEx *pArith )
{
	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT ( pSnTanaData );
	if ( pSnTanaData == NULL ) {
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

	ASSERT ( pArith );
	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

	return 0;
}

//=========================================
// 初期化
//-----------------------------------------
// 返送値
//		0		;		正常終了
//		-1		:		エラー
//=========================================
int CChangeTax::Init()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	
	char buf[512] = { 0 };

	tansw = m_pSnTanaData->Sn_MENZsw;

	switch ( m_pSnTanaData->Sn_MENZsw & 0xff ) {
	case 0x10:	// 免税から課税
		m_CbSw = 0;

		memset ( buf, '\0', sizeof ( buf ) );
		strcpy_s ( buf, sizeof ( buf ), _T( "当期より、免税業者から課税業者となる場合の\n" ) );
		strcat_s ( buf, sizeof ( buf ), _T( "課税仕入等に係る調整税額" ) );
		DiagData.data_disp = buf;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDTITLE, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		strcpy_s ( buf, sizeof ( buf ), _T( "控除対象\n" ) );
		strcat_s ( buf, sizeof ( buf ), _T( "仕入額" ) );
		DiagData.data_disp = buf;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDSUBTL, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		break;
	case 0x11:	// 課税から免税
		m_CbSw = 1;

		memset ( buf, '\0', sizeof ( buf ) );
		strcpy_s ( buf, sizeof ( buf ), _T( "翌期より、課税業者から免税業者となる場合の\n" ) );
		strcat_s ( buf, sizeof ( buf ), _T( "課税仕入等に係る調整税額" ) );
		DiagData.data_disp = buf;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDTITLE, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		strcpy_s ( buf, sizeof ( buf ), _T( "控除対象外\n" ) );
		strcat_s ( buf, sizeof ( buf ), _T( "仕入額" ) );
		DiagData.data_disp = buf;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDSUBTL, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		break;
	default:	// 課税方式切り替え解除
		m_pSnTanaData->Sn_MENZsw = 0x00;
		m_CbSw = 2;

		diag_clear ( IDC_ICSDIAGCTRL1, IDTITLE, TRUE, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		strcpy_s ( buf, sizeof ( buf ), _T( "控除対象\n" ) );
		strcat_s ( buf, sizeof ( buf ), _T( "仕入額 ") );
		DiagData.data_disp = buf;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDSUBTL, ( struct IUnknown*)&DiagData, CIcsdiagctrl );

		memset ( m_pSnTanaData->Sn_MNK63, '\0', MONY_BUF_SIZE );
		memset ( m_pSnTanaData->Sn_MNZ63, '\0', MONY_BUF_SIZE );
		memset ( m_pSnTanaData->Sn_MNK40, '\0', MONY_BUF_SIZE );
		memset ( m_pSnTanaData->Sn_MNZ40, '\0', MONY_BUF_SIZE );
		memset ( m_pSnTanaData->Sn_MENZK, '\0', MONY_BUF_SIZE );
		memset ( m_pSnTanaData->Sn_MENZZ, '\0', MONY_BUF_SIZE );
		break;
	}

	DiagData.data_combo = m_CbSw;
	diag_setdata ( IDC_ICSDIAGCTRL1, IDCOMBO, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	memset ( MNK63, '\0', MONY_BUF_SIZE );
	memset ( MNZ63, '\0', MONY_BUF_SIZE );
	memset ( MNK40, '\0', MONY_BUF_SIZE );
	memset ( MNZ40, '\0', MONY_BUF_SIZE );
	memset ( MENZK, '\0', MONY_BUF_SIZE );
	memset ( MENZZ, '\0', MONY_BUF_SIZE );

	memset ( MNK78, '\0', MONY_BUF_SIZE );
	memset ( MNZ78, '\0', MONY_BUF_SIZE );
	memset ( MNK624, '\0', MONY_BUF_SIZE );
	memset ( MNZ624, '\0', MONY_BUF_SIZE );

	memmove ( MNK63, m_pSnTanaData->Sn_MNK63, MONY_BUF_SIZE );
	memmove ( MNZ63, m_pSnTanaData->Sn_MNZ63, MONY_BUF_SIZE );

	if ( m_pSnHeadData->SVmzsw == 1 ) {
		memmove ( MNK40, m_pSnTanaData->Sn_MNK40, MONY_BUF_SIZE );
		memmove ( MNZ40, m_pSnTanaData->Sn_MNZ40, MONY_BUF_SIZE );
		memmove ( MENZK, m_pSnTanaData->Sn_MENZK, MONY_BUF_SIZE );
		memmove ( MENZZ, m_pSnTanaData->Sn_MENZZ, MONY_BUF_SIZE );
	}

	memmove ( MNK78, m_pSnTanaData->Sn_MNK78, MONY_BUF_SIZE );
	memmove ( MNZ78, m_pSnTanaData->Sn_MNZ78, MONY_BUF_SIZE );
	memmove ( MNK624, m_pSnTanaData->Sn_MNK624, MONY_BUF_SIZE );
	memmove ( MNZ624, m_pSnTanaData->Sn_MNZ624, MONY_BUF_SIZE );

	memmove ( DiagData.data_val, MNK63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAGCTRL1, IDMNK63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	memmove ( DiagData.data_val, MNZ63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAGCTRL1, IDMNZ63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );


	if ( m_pSnHeadData->SVmzsw == 1 ) {
		memmove ( DiagData.data_val, MNK40, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMNK40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memmove ( DiagData.data_val, MNZ40, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMNZ40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memmove ( DiagData.data_val, MENZK, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMENZK, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memmove ( DiagData.data_val, MENZZ, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMENZZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}

	if ( m_pSnHeadData->SVsign&0x10 ) {
		m_ChSw = 1;
		tmpChSw = 1;
		DiagData.data_check = 0;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDNUKIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		DiagData.data_check = 1;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDKOMIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		m_ChSw = 0;
		tmpChSw = 0;
		DiagData.data_check = 1;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDNUKIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		DiagData.data_check = 0;
		diag_setdata ( IDC_ICSDIAGCTRL1, IDKOMIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}

	ComboCheck_AttrSet();

	return 0;
}

//==================================
// 入力項目色変更
//==================================
void CChangeTax::ChangeColor ( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE DA;
	
	diag_getattr ( id, index, &DA, CIcsdiagctrl );
	switch ( sign ) {
	case 1:		// 入力不可 ( 白色 )
		DA.attr_bcolor = BC_WHITE;
		break;
	case 2:		// 入力不可 ( 灰色 )
		DA.attr_bcolor = BC_GRAY;
		break;
	case 3:		// 入力不可 ( 青色 )
		DA.attr_bcolor = BC_BLUE;
		break;
	case 4:		// 入力不可 ( 緑色 )
		DA.attr_bcolor = BC_GREEN;
		break;
	case 5:		// 入力不可 ( 薄緑色 )
		DA.attr_bcolor = BC_GREEN_L;
		break;
	default:	// 入力不可 ( クリーム色 )
		DA.attr_bcolor = BC_CREAM;
		break;
	}
	diag_setattr ( id, index, &DA, FALSE, CIcsdiagctrl );
}

//==================================
// 税変更時の計算処理
//==================================
void CChangeTax::ZeiChangeCalq()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( tmpChSw != m_ChSw ) {
		if ( m_ChSw == 1 ) {	// 税込み
			m_Util.percent ( MNZ40, MNK40, 4, 40, 0 );
			m_Util.percent ( MENZZ, MENZK, 0, 30, 0 );
			m_Util.percent ( MNZ63, MNK63, 9, 63, 0 );

			m_Util.percent ( MNZ624, MNK624, 11, 62, 0 );
			m_Util.percent ( MNZ78, MNK78, 12, 78, 0 );
		}
		else {	// 税抜き
			m_Util.percent ( MNZ40, MNK40, 1, 40, 0 );
			m_Util.percent ( MENZZ, MENZK, 1, 30, 0 );
			m_Util.percent ( MNZ63, MNK63, 1, 63, 0 );

			m_Util.percent ( MNZ624, MNK624, 1, 62, 0 );
			m_Util.percent ( MNZ78, MNK78, 1, 78, 0 );
		}

		memmove ( DiagData.data_val, MNZ63, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMNZ63, ( struct IUnknown* ) &DiagData, CIcsdiagctrl );

		memmove ( DiagData.data_val, MNZ40, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMNZ40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memmove ( DiagData.data_val, MENZZ, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAGCTRL1, IDMENZZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	if ( m_ChSw == 1 ) {
		tmpChSw = 1;
	}
	else {
		tmpChSw = 0;
	}
}

//==================================
// コンボボックスの情報をチェック
//==================================
void CChangeTax::ComboCheck_AttrSet()
{
	int		flg = 0;
	short	index = 0;
	char	buf[512] = { 0 };

	switch ( m_pSnHeadData->Sn_Sign4 & 0x81 ) {
	case 0x01:	// ロック
	case 0x81:	// 非連動＆ロック
		// 仕訳による棚卸調整有り時は表示のみ
		if ( m_pSnTanaData->Sn_tansw & 0x01 ) {
			flg = 1;
		}
		else {
			flg = 2;
		}

		diag_modify ( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, IDCOMBO, flg );

		diag_modify ( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, IDNUKIZ, flg );

		diag_modify ( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, IDKOMIZ, flg );

		if ( m_pSnHeadData->SVmzsw == 1 ) {
			diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, flg );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, flg );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, flg );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, flg );
		}
		else {
			diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 1 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 1 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
		}

		c_set( IDOK );
		break;
	case 0x80 :	// 非連動
		diag_modify ( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );

		if ( ( tansw != 0x10 ) && ( tansw != 0x11 ) ) {

			diag_modify ( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK63, 1 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ63, 1 );

			if ( m_pSnHeadData->SVmzsw == 1 ){
				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
			}
			else {
				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
			}
		}
		else {
			diag_modify ( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDNUKIZ, 0 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDKOMIZ, 0 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK63, 0 );

			diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ63, 0 );

			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 0 );
			}
			else {
				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 1 );

				diag_modify( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
			}
		}

		::ZeroMemory ( buf, sizeof ( buf ) );
		m_txt1.SetWindowTextA ( buf );
		diag_setposition ( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
		break;

	default: // 連動
		if ( m_pSnTanaData->Sn_tansw & 0x01 ) {
			// 仕訳による棚卸調整有り時は表示のみ
			diag_modify ( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDCOMBO, 1 );
			diag_modify ( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );
			diag_modify ( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
			ChangeColor ( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );

			for ( index = IDMNK63; index <= IDMENZZ; index++ ) {
				diag_modify ( IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, index, 1 );
			}

			strcpy_s ( buf, sizeof ( buf ), _T( " 棚卸調整仕訳が入力されています。\n 税額を修正する場合は、該当仕訳を変更してください。" ) );
			m_txt1.SetWindowTextA ( buf );
			c_set ( IDOK );
		}
		else {
			if ( ( tansw != 0x10 ) && ( tansw != 0x11 ) ) {
				diag_modify ( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDNUKIZ, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDKOMIZ, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK63, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ63, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 1 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );

			}
			else {
				diag_modify ( IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDCOMBO, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDNUKIZ, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDKOMIZ, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNK63, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK63, 0 );

				diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ63, DIAG_MDFY_EDIT, CIcsdiagctrl );
				ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ63, 0 );

				if ( m_pSnHeadData->SVmzsw == 1 ) {
					diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 0 );

					diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 0 );

					diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 0 );

					diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 0 );
				}
				else {
					diag_modify ( IDC_ICSDIAGCTRL1, IDMENZK, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZK, 1 );

					diag_modify ( IDC_ICSDIAGCTRL1, IDMENZZ, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMENZZ, 1 );

					diag_modify ( IDC_ICSDIAGCTRL1, IDMNK40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMNK40, 1 );

					diag_modify ( IDC_ICSDIAGCTRL1, IDMNZ40, DIAG_MDFY_READONLY, CIcsdiagctrl );
					ChangeColor ( IDC_ICSDIAGCTRL1, IDMNZ40, 1 );
				}
			}

			::ZeroMemory ( buf, sizeof ( buf ) );
			m_txt1.SetWindowTextA ( buf );
			diag_setposition ( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
		}
		break;
	}
	
	diag_redraw ( IDC_ICSDIAGCTRL1 , CIcsdiagctrl);	//再描画

	return;
}

void CChangeTax::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	char buf[512] = { 0 };

	if( !( m_pSnTanaData->Sn_tansw & 0x01 ) || ( m_pSnHeadData->Sn_Sign4 & 0x80 ) ) {
		if ( m_CbSw == 2 ) { 
			// 警告メッセージ
			sprintf_s ( buf, sizeof ( buf ), _T( "課税方式の切替えを解除しますか？" ) );
			if ( ICSMessageBox ( buf, MB_YESNO, 0, 0, this ) == IDYES ) {
				m_pSnTanaData->Sn_MENZsw = 0x00;
				tansw = 0x00;
				memset ( MNK40, '\0', MONY_BUF_SIZE );
				memset ( MNZ40, '\0', MONY_BUF_SIZE );
				memset ( MENZK, '\0', MONY_BUF_SIZE );
				memset ( MENZZ, '\0', MONY_BUF_SIZE );
				memset ( MNK63, '\0', MONY_BUF_SIZE );
				memset ( MNZ63, '\0', MONY_BUF_SIZE );

				memset ( MNK624, '\0', MONY_BUF_SIZE );
				memset ( MNZ624, '\0', MONY_BUF_SIZE );
				memset ( MNK78, '\0', MONY_BUF_SIZE );
				memset ( MNZ78, '\0', MONY_BUF_SIZE );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
				return;
			}
		}

		memmove ( m_pSnTanaData->Sn_MNK63, MNK63, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MNZ63, MNZ63, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MNK40, MNK40, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MNZ40, MNZ40, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MENZK, MENZK, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MENZZ, MENZZ, MONY_BUF_SIZE );

		memmove ( m_pSnTanaData->Sn_MNK78, MNK78, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MNZ78, MNZ78, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MNK624, MNK624, MONY_BUF_SIZE );
		memmove ( m_pSnTanaData->Sn_MNZ624, MNZ624, MONY_BUF_SIZE );

		m_pSnTanaData->Sn_MENZsw = tansw;


		if ( m_ChSw == 1 ) {	//税込み
			m_pSnHeadData->SVsign |= 0x10;
		}
		else {	//税抜き
			m_pSnHeadData->SVsign &= 0xef;
		}
	}

	ICSDialog::OnOK();
}


void CChangeTax::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnCancel();
}


BOOL CChangeTax::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	CWnd *pWnd = GetFocus();

	if ( pMsg->message == WM_KEYDOWN ) {

		if ( ( GetCombineKeyDown ( VK_SHIFT ) && ( pMsg->wParam >= VK_F1 ) && ( pMsg->wParam <= VK_F12 ) ) ) {
			return TRUE;
		}

		if ( ( pMsg->wParam == VK_TAB ) || ( pMsg->wParam == VK_RIGHT ) ) {
			if ( pWnd == GetDlgItem ( IDOK ) ) {
				c_set ( IDCANCEL );
				return TRUE;	
			}
			else if ( pWnd == GetDlgItem ( IDCANCEL ) ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );	
				return TRUE;
			}
		}
		else if ( pMsg->wParam == VK_UP ) {
			if ( ( pWnd == GetDlgItem ( IDOK ) ) || ( pWnd == GetDlgItem ( IDCANCEL ) ) ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMENZZ, CIcsdiagctrl );
				return TRUE;	
			}
		}
		else if ( pMsg->wParam == VK_DOWN ) {
			if ( ( pWnd == GetDlgItem ( IDOK ) ) || ( pWnd == GetDlgItem ( IDCANCEL ) ) ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
				return TRUE;	
			}
		}
		else if ( pMsg->wParam == VK_LEFT ) {
			if ( pWnd == GetDlgItem ( IDOK ) ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMENZZ, CIcsdiagctrl );
				return TRUE;	
			}
		}

	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CChangeTax, ICSDialog)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 1, CChangeTax::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 2, CChangeTax::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 3, CChangeTax::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CChangeTax, IDC_ICSDIAGCTRL1, 8, CChangeTax::ComboSelIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CChangeTax::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){

	case IDNUKIZ:	// 控除対象仕入額　税抜き
		if(m_ChSw == 1){
			m_ChSw = 0;
			diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
			DiagData.data_check = 1;
			diag_setdata( IDC_ICSDIAGCTRL1,IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			DiagData.data_check = 0;
			diag_setdata( IDC_ICSDIAGCTRL1,IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			ZeiChangeCalq();

			diag_setposition( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );
		}
		break;

	case IDKOMIZ:	// 控除対象仕入額　税込み

		if(m_ChSw == 0){
			m_ChSw = 1;
			diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
			DiagData.data_check = 0;
			diag_setdata( IDC_ICSDIAGCTRL1,IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			DiagData.data_check = 1;
			diag_setdata( IDC_ICSDIAGCTRL1,IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			ZeiChangeCalq();
			diag_setposition( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );
		}
		break;

	default :
		ZeiChangeCalq();
		break;
	}
}

void CChangeTax::EditOFFIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char CkMy[MONY_BUF_SIZE] = { 0 };
	char buf[512] = { 0 };
	
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ) {
	case IDCOMBO:	// 課税方式選択
		break;
	case IDMNK63:	// 調整課税仕入等に係る棚卸資産額 (税率6.3％分)
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, MNK63, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAGCTRL1, IDMNK63, ( struct IUnknown*) &DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 

		if ( m_pArith->l_cmp ( MNK63, CkMy ) ) {
			memmove ( MNK63, CkMy, MONY_BUF_SIZE );
			if ( m_ChSw == 1 ) {
				m_Util.percent ( MNZ63, MNK63, 9, 63, 0 );
			}
			else {
				m_Util.percent ( MNZ63, MNK63, 1, 63, 0 );
			}

			memmove ( DiagData.data_val, MNZ63, MONY_BUF_SIZE );
			diag_setdata ( IDC_ICSDIAGCTRL1, IDMNZ63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}	
		break;
	case IDMNK40:	// 調整課税仕入等に係る棚卸資産額 (税率4％分)
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, MNK40, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAGCTRL1, IDMNK40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 

		if ( m_pArith->l_cmp ( MNK40, CkMy ) ) {
			memmove ( MNK40, CkMy, MONY_BUF_SIZE );
			if ( m_ChSw == 1 ) {
				m_Util.percent ( MNZ40, MNK40, 4, 40, 0 );
			}
			else {
				m_Util.percent ( MNZ40, MNK40, 1, 40, 0 );
			}

			memmove ( DiagData.data_val, MNZ40, MONY_BUF_SIZE );
			diag_setdata ( IDC_ICSDIAGCTRL1, IDMNZ40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}	
		break;
	case IDMENZK:	// 調整課税仕入等に係る棚卸資産額 (税率3％分)
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, MENZK, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAGCTRL1, IDMENZK, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 

		if ( m_pArith->l_cmp ( MENZK, CkMy ) ) {
			memmove ( MENZK, CkMy, MONY_BUF_SIZE );
			if ( m_ChSw == 1 ) {
				m_Util.percent ( MENZZ, MENZK, 0, 30, 0 );
			}
			else {
				m_Util.percent ( MENZZ, MENZK, 1, 30, 0 );
			}

			memmove ( DiagData.data_val, MENZZ, MONY_BUF_SIZE );
			diag_setdata ( IDC_ICSDIAGCTRL1, IDMENZZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}	
		break;
	case IDMNZ63:	// 調整課税仕入等に係る消費税額   (6.3%分)
		diag_getdata ( IDC_ICSDIAGCTRL1, IDMNZ63, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( MNZ63, DiagData.data_val, MONY_BUF_SIZE ); 
		break;
	case IDMNZ40:	// 調整課税仕入等に係る消費税額   (4%分)
		diag_getdata ( IDC_ICSDIAGCTRL1, IDMNZ40, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove( MNZ40, DiagData.data_val, MONY_BUF_SIZE ); 
		break;
	case IDMENZZ:	// 調整課税仕入等に係る消費税額   (3%分)
		diag_getdata ( IDC_ICSDIAGCTRL1, IDMENZZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( MENZZ, DiagData.data_val, MONY_BUF_SIZE ); 
		break;
	default:
		break;
	}
}

void CChangeTax::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char  VK_FLG = 0x00,
		  VK_DMM = 0x00;
	short wTerm = 0;

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( nChar == 0 ){
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	VK_FLG = VK_DMM = 0x00;
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

	switch ( index ){
	case IDCOMBO:	// 課税方式選択

		if ( wTerm == VK_UP ) {
			return;
		}

		if ( wTerm == VK_ESCAPE ) {
			OnCancel();
			return;
		}

		if ( wTerm == VK_DOWN ) {
			m_ICSDIAG1.ComboDropDown ( IDCOMBO, TRUE );
			return;
		}

		diag_getdata ( IDC_ICSDIAGCTRL1, IDCOMBO, ( struct IUnknown*) &DiagData, CIcsdiagctrl );
		if ( DiagData.data_combo == 2 ) {
			c_set( IDOK );
			return;
		}

		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) ) {
			if( m_ChSw == 1 ){
				diag_setposition ( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );
			}
		}

		break;

	case IDKOMIZ:	// 控除対象外仕入額　税込み
	case IDNUKIZ:	// 控除対象外仕入額　税抜き

		if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_RIGHT ) ) {
			if ( m_ChSw == 1 ) {
				m_ChSw = 0;
				
				DiagData.data_check = 1;
				diag_setdata ( IDC_ICSDIAGCTRL1,IDNUKIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
				DiagData.data_check = 0;
				diag_setdata ( IDC_ICSDIAGCTRL1,IDKOMIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

				ZeiChangeCalq();
				diag_setposition ( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );
			}
			else{
				m_ChSw = 1;
				DiagData.data_check = 0;
				diag_setdata ( IDC_ICSDIAGCTRL1,IDNUKIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

				DiagData.data_check = 1;
				diag_setdata ( IDC_ICSDIAGCTRL1,IDKOMIZ, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

				ZeiChangeCalq();
				diag_setposition ( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );
			}
		}
		 
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) ) {
			diag_setposition ( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
			diag_setposition ( IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl );
		}
		break;				
	case IDMNK40:	// 調整課税仕入等に係る棚卸資産額 (税率4分)
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) || ( wTerm == VK_RIGHT ) ) {
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMENZK, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ40, CIcsdiagctrl );
			}
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
			if( m_ChSw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );
			}
		}
		break;
	case IDMNK63:	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) || ( wTerm == VK_RIGHT ) ) {
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNK40, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
			}
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
			if ( m_ChSw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl );
			}
		}
		break;
	case IDMENZK:	// 調整課税仕入等に係る棚卸資産額 (税率3%分)
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) || ( wTerm == VK_RIGHT ) ) {
			diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
			diag_setposition ( IDC_ICSDIAGCTRL1, IDMNK40, CIcsdiagctrl );
		}
		break;
	case IDMNZ40:	// 調整課税仕入等に係る消費税額   (税率4%分)
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm==VK_F3 ) || ( wTerm == VK_DOWN ) || ( wTerm == VK_RIGHT ) ) {
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMENZZ, CIcsdiagctrl );
			}
			else {
				c_set ( IDOK );
			}
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2) || ( wTerm == VK_UP ) ) {
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ63, CIcsdiagctrl );
			}
		}
		break;
	case IDMNZ63:	// 調整課税仕入等に係る消費税額   (税率6.3%分)
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_DOWN ) || ( wTerm == VK_RIGHT ) ) {
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ40, CIcsdiagctrl );
			}
			else {
				c_set ( IDOK );
			}
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMENZK, CIcsdiagctrl );
			}
			else {
				diag_setposition ( IDC_ICSDIAGCTRL1, IDMNK63, CIcsdiagctrl );
			}
		}
		break;
	case IDMENZZ:	// 調整課税仕入等に係る消費税額   (旧税率分)
		// 次項目
		if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3) || ( wTerm == VK_DOWN ) || ( wTerm == VK_RIGHT ) ) {
			c_set ( IDOK );
		}
		// 前項目へ
		if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) || ( wTerm == VK_UP ) ) {
			diag_setposition ( IDC_ICSDIAGCTRL1, IDMNZ40, CIcsdiagctrl );
		}
		break;
	default:
		break;
	}
}

void CChangeTax::ComboSelIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char buf[512] = { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	if ( index == IDCOMBO ) {
		diag_getdata ( IDC_ICSDIAGCTRL1, IDCOMBO, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		m_CbSt = DiagData.data_combo;
		switch ( m_CbSt ) {
		case 0:	// 免税より課税
			m_CbSw = 0;
			tansw = 0x10;

			memset ( buf, '\0', sizeof ( buf ) );
			strcpy_s ( buf, sizeof( buf ), _T( "当期より、免税業者から課税業者となる場合の\n" ) );
			strcat_s ( buf, sizeof( buf ), _T( "課税仕入等に係る調整税額" ) );
			DiagData.data_disp = buf;
			diag_setdata ( IDC_ICSDIAGCTRL1, IDTITLE, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			memset( buf, '\0', sizeof ( buf ) );
			strcpy_s( buf, sizeof ( buf ), _T( "控除対象\n" ) );
			strcat_s( buf, sizeof ( buf ), _T( "仕入額" ) );
			DiagData.data_disp = buf;
			diag_setdata ( IDC_ICSDIAGCTRL1, IDSUBTL, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			break;
		case 1:	// 課税より免税
			m_CbSw = 1;
			tansw = 0x11;

			memset ( buf, '\0', sizeof ( buf ) );
			strcpy_s ( buf, sizeof ( buf ), _T( "翌期より、課税業者から免税業者となる場合の\n" ) );
			strcat_s ( buf, sizeof ( buf ), _T( "課税仕入等に係る調整税額" ) );
			DiagData.data_disp = buf;
			diag_setdata ( IDC_ICSDIAGCTRL1, IDTITLE, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			memset ( buf, '\0', sizeof ( buf ) );
			strcpy_s ( buf, sizeof ( buf ), _T( "控除対象外\n" ) );
			strcat_s ( buf, sizeof ( buf ), _T( "仕入額" ) );
			DiagData.data_disp = buf;
			diag_setdata ( IDC_ICSDIAGCTRL1, IDSUBTL, ( struct IUnknown*) &DiagData, CIcsdiagctrl );
			break;
		default:
			m_CbSw = 2;
			tansw = 0x00;
			memset ( buf, '\0', sizeof ( buf ) );
			strcpy_s ( buf, sizeof ( buf ), _T(" ") );
			DiagData.data_disp = buf;
			diag_setdata ( IDC_ICSDIAGCTRL1, IDTITLE, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			break;
		}

		ComboCheck_AttrSet();
	}
}