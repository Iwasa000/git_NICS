// CChangeTaxEx2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ChangeTaxEx2.h"
#include "afxdialogex.h"
#include "ChangeTaxEx2Idx.h"


// CChangeTaxEx2 ダイアログ

IMPLEMENT_DYNAMIC(CChangeTaxEx2, ICSDialog)

CChangeTaxEx2::CChangeTaxEx2(CWnd* pParent /*=nullptr*/)
	//: ICSDialog(IDD_DIALOG4, pParent)
	: ICSDialog(IDD_DIALOG_CHGTAXEX2, pParent)
	, m_pSnFhyo2_01Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pSnTanaData(NULL)
	, m_CbSw(0)
	, m_ChSw(0)
	, m_CbSt(0)
{
	memset(MNK624K, '\0', MONY_BUF_SIZE);
	memset(MNZ624K, '\0', MONY_BUF_SIZE);
	memset(MNK624H, '\0', MONY_BUF_SIZE);
	memset(MNZ624H, '\0', MONY_BUF_SIZE);
	memset(MNK624C, '\0', MONY_BUF_SIZE);
	memset(MNZ624C, '\0', MONY_BUF_SIZE);

	memset(MNK78K, '\0', MONY_BUF_SIZE);
	memset(MNZ78K, '\0', MONY_BUF_SIZE);
	memset(MNK78H, '\0', MONY_BUF_SIZE);
	memset(MNZ78H, '\0', MONY_BUF_SIZE);
	memset(MNK78C, '\0', MONY_BUF_SIZE);
	memset(MNZ78C, '\0', MONY_BUF_SIZE);
	tmpChSw = 0;
	tansw = 0;
}

CChangeTaxEx2::~CChangeTaxEx2()
{
}

void CChangeTaxEx2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ICSDIAGCTRL1, DISPID(), m_ICSDIAG1);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_ICSDIAG1);	// こっちに書き換えたけど大丈夫？
	//DDX_Control(pDX, IDC_ICSDISPCTRL1, DISPID(), m_txt1);
	//DDX_Control(pDX, IDC_ICSDISPCTRL1, m_txt1);
}


BEGIN_MESSAGE_MAP(CChangeTaxEx2, ICSDialog)
	ON_BN_CLICKED(IDOK, &CChangeTaxEx2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChangeTaxEx2::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChangeTaxEx2 メッセージ ハンドラー


BOOL CChangeTaxEx2::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください

	// 初期化
	Init();

	ICSDialog::OnInitDialogEX();
	return FALSE;
	
	//return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//----------------------------
// 初期化
//----------------------------
// 返送値	0	：　正常終了
//			-1	：	エラー
//----------------------------
int CChangeTaxEx2::Init()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};

	tansw = m_pSnTanaData->Sn_MENZsw;

	// タイトル
	switch ( m_pSnTanaData->Sn_MENZsw & 0xff ) {
		case 0x10:	// 免税業者から課税業者へ
			m_CbSw = 0;

			memset(&buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("当期より、免税業者から課税業者となる場合の課税仕入等に係る調整税額"));
			DiagData.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("控除対象仕入額"));
			DiagData.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ78, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ624, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			break;

		case 0x11:	// 課税業者から免税業者へ
			m_CbSw = 1;

			memset(&buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("翌期より、課税業者から免税業者となる場合の課税仕入等に係る調整税額"));
			DiagData.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("控除対象外仕入額"));
			DiagData.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ78, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ624, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			break;

		default :	// 免税業者から課税業者へ
			m_pSnTanaData->Sn_MENZsw = 0x00;
			m_pSnTanaData->Sn_tansw &= 0xfe;
			m_CbSw = 2;

			diag_clear(IDC_ICSDIAGCTRL1, IDTITLE, TRUE, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("控除対象仕入額"));
			DiagData.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ78, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ624, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			break;
	}

	// コンボボックス
	DiagData.data_combo = m_CbSw;
	diag_setdata(IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	if ( m_pSnHeadData->SVsign & 0x10 ) {	// 税込み
		m_ChSw = 1;
		tmpChSw = 1;
		DiagData.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		DiagData.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}
	else {									// 税抜き
		m_ChSw = 0;
		tmpChSw = 0;
		DiagData.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		DiagData.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}

	// 金額クリア
	memset(MNK624K, '\0', MONY_BUF_SIZE);
	memset(MNZ624K, '\0', MONY_BUF_SIZE);
	memset(MNK624H, '\0', MONY_BUF_SIZE);
	memset(MNZ624H, '\0', MONY_BUF_SIZE);
	memset(MNK624C, '\0', MONY_BUF_SIZE);
	memset(MNZ624C, '\0', MONY_BUF_SIZE);

	memset(MNK78K, '\0', MONY_BUF_SIZE);
	memset(MNZ78K, '\0', MONY_BUF_SIZE);
	memset(MNK78H, '\0', MONY_BUF_SIZE);
	memset(MNZ78H, '\0', MONY_BUF_SIZE);
	memset(MNK78C, '\0', MONY_BUF_SIZE);
	memset(MNZ78C, '\0', MONY_BUF_SIZE);

	memset(MNK40K, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　課税に係る
	memset(MNZ40K, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
	memset(MNK40H, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　非課税に係る
	memset(MNZ40H, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
	memset(MNK40C, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　共通に係る
	memset(MNZ40C, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

	memset(MENZKK, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　課税に係る
	memset(MENZZK, '\0', MONY_BUF_SIZE);	// 				 〃	   消費税額   (〃      )　〃
	memset(MENZKH, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　非課税に係る
	memset(MENZZH, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
	memset(MENZKC, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　共通に係る
	memset(MENZZC, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

	memset(MNK63K, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　課税に係る
	memset(MNZ63K, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
	memset(MNK63H, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　非課税に係る
	memset(MNZ63H, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
	memset(MNK63C, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　共通に係る
	memset(MNZ63C, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

	// 金額の表示 
	if ( m_pSnTanaData->Sn_MENZsw & 0xff ) {

		memmove(MNK78K, m_pSnTanaData->Sn_mnk78[0], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　課税に係る
		memmove(MNZ78K, m_pSnTanaData->Sn_mnz78[0], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(MNK78H, m_pSnTanaData->Sn_mnk78[1], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　非課税に係る
		memmove(MNZ78H, m_pSnTanaData->Sn_mnz78[1], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(MNK78C, m_pSnTanaData->Sn_mnk78[2], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　共通に係る
		memmove(MNZ78C, m_pSnTanaData->Sn_mnz78[2], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

		memmove(MNK624K, m_pSnTanaData->Sn_mnk624[0], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　課税に係る
		memmove(MNZ624K, m_pSnTanaData->Sn_mnz624[0], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(MNK624H, m_pSnTanaData->Sn_mnk624[1], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　非課税に係る
		memmove(MNZ624H, m_pSnTanaData->Sn_mnz624[1], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(MNK624C, m_pSnTanaData->Sn_mnk624[2], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　共通に係る
		memmove(MNZ624C, m_pSnTanaData->Sn_mnz624[2], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

		if (m_pSnHeadData->SVmzsw == 1) {
			memmove(MNK63K, m_pSnTanaData->Sn_mnk63[0], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　課税に係る
			memmove(MNZ63K, m_pSnTanaData->Sn_mnz63[0], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(MNK63H, m_pSnTanaData->Sn_mnk63[1], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　非課税に係る
			memmove(MNZ63H, m_pSnTanaData->Sn_mnz63[1], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(MNK63C, m_pSnTanaData->Sn_mnk63[2], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　共通に係る
			memmove(MNZ63C, m_pSnTanaData->Sn_mnz63[2], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

			memmove(MNK40K, m_pSnTanaData->Sn_mnk40[0], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　課税に係る
			memmove(MNZ40K, m_pSnTanaData->Sn_mnz40[0], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(MNK40H, m_pSnTanaData->Sn_mnk40[1], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　非課税に係る
			memmove(MNZ40H, m_pSnTanaData->Sn_mnz40[1], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(MNK40C, m_pSnTanaData->Sn_mnk40[2], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　共通に係る
			memmove(MNZ40C, m_pSnTanaData->Sn_mnz40[2], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

			memmove(MENZKK, m_pSnTanaData->Sn_menzk[0], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　課税に係る
			memmove(MENZZK, m_pSnTanaData->Sn_menzz[0], MONY_BUF_SIZE);	// 				 〃	   消費税額   (〃      )　〃
			memmove(MENZKH, m_pSnTanaData->Sn_menzk[1], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　非課税に係る
			memmove(MENZZH, m_pSnTanaData->Sn_menzz[1], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(MENZKC, m_pSnTanaData->Sn_menzk[2], MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　共通に係る
			memmove(MENZZC, m_pSnTanaData->Sn_menzz[2], MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		}
	}

	DispMony(IDMNK78K, MNK78K);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　課税に係る
	DispMony(IDMNZ78K, MNZ78K);	//				 〃	   消費税額   (〃      )　〃
	DispMony(IDMNK78H, MNK78H);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　非課税に係る
	DispMony(IDMNZ78H, MNZ78H);	//				 〃	   消費税額   (〃      )　〃
	DispMony(IDMNK78C, MNK78C);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　共通に係る
	DispMony(IDMNZ78C, MNZ78C);	//				 〃	   消費税額   (〃      )　〃

	DispMony(IDMNK624K, MNK624K);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　課税に係る
	DispMony(IDMNZ624K, MNZ624K);	//				 〃	   消費税額   (〃      )　〃
	DispMony(IDMNK624H, MNK624H);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　非課税に係る
	DispMony(IDMNZ624H, MNZ624H);	//				 〃	   消費税額   (〃      )　〃
	DispMony(IDMNK624C, MNK624C);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　共通に係る
	DispMony(IDMNZ624C, MNZ624C);	//				 〃	   消費税額   (〃      )　〃

	ComboCheck_AttrSet();

	return 0;
}

//-----------------------
// 初期情報のセット
//-----------------------
int CChangeTaxEx2::InitInfo(CH31SnFhyo2_01Data *pSnFhyo2_01Data, CSnHeadData *pSnHeadData, CH31SnTanaData *pSnTanaData, CArithEx *pArith)
{
	ASSERT(pSnFhyo2_01Data);
	if (pSnFhyo2_01Data == NULL) {
		return -1;
	}
	m_pSnFhyo2_01Data = pSnFhyo2_01Data;

	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pSnTanaData);
	if (pSnTanaData == NULL) {
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

	ASSERT(pArith);
	if (pArith == NULL) {
		return -1;
	}
	m_pArith = pArith;

	return 0;
}

//-----------------------------------------------
// 金額表示
//-----------------------------------------------
// 引数　index	：　ダイアグラム内インデックス
//		 mony	：　表示する金額
//-----------------------------------------------
void CChangeTaxEx2::DispMony( short index, char *mony )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	memmove( DiagData.data_val, mony, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

//----------------------------------------------
// 入力項目　色変え
//----------------------------------------------
// 引数　id		：　ダイアグラムID
//		 index　：　ダイアグラム内インデックス
//		 sign	：　変更サイン
//----------------------------------------------
void CChangeTaxEx2::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr(id, index, &DA, CIcsdiagctrl);
	switch (sign) {
	case 1:
		DA.attr_bcolor = BC_WHITE;
		break;		// 入力不可	（白色）
	case 2:
		DA.attr_bcolor = BC_GRAY;
		break;		// ロック	（灰色）
	case 3:
		DA.attr_bcolor = BC_BLUE;
		break;		// 特殊色①	（青色）
	case 4:
		DA.attr_bcolor = BC_GREEN;
		break;		// 特殊色②	（緑色）	
	case 5:
		DA.attr_bcolor = BC_GREEN_L;
		break;		// 特殊色③	（薄緑色）	 
	default:
		DA.attr_bcolor = BC_CREAM;
		break;		// 実額入力	（クリーム色） 
	}
	diag_setattr(id, index, &DA, FALSE, CIcsdiagctrl);
}

//-----------------------
// 税変更時の計算処理
//-----------------------
void CChangeTaxEx2::ZeiChangeCalq()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if (tmpChSw != m_ChSw) {
		if (m_ChSw == 1) {	// 税込み

			//---->yoshida
			m_Util.percent(MNZ78K, MNK78K, 12, 78, 0);
			m_Util.percent(MNZ78H, MNK78H, 12, 78, 0);
			m_Util.percent(MNZ78C, MNK78C, 12, 78, 0);
			m_Util.percent(MNZ624K, MNK624K, 11, 62, 0);
			m_Util.percent(MNZ624H, MNK624H, 11, 62, 0);
			m_Util.percent(MNZ624C, MNK624C, 11, 62, 0);
			//--------------
			/*
			m_Util.percent(MNZ78K, MNK78K, 4, 63, 0);
			m_Util.percent(MNZ78H, MNK78H, 4, 63, 0);
			m_Util.percent(MNZ78C, MNK78C, 4, 63, 0);
			m_Util.percent(MNZ624K, MNK624K, 9, 40, 0);
			m_Util.percent(MNZ624H, MNK624H, 9, 40, 0);
			m_Util.percent(MNZ624C, MNK624C, 9, 40, 0);
			*/
			//<--------------
			if (m_pSnHeadData->SVmzsw == 1) {
				m_Util.percent(MNZ40K, MNK40K, 4, 40, 0);
				m_Util.percent(MNZ40H, MNK40H, 4, 40, 0);
				m_Util.percent(MNZ40C, MNK40C, 4, 40, 0);
				m_Util.percent(MENZZK, MENZKK, 0, 30, 0);
				m_Util.percent(MENZZH, MENZKH, 0, 30, 0);
				m_Util.percent(MENZZC, MENZKC, 0, 30, 0);
				m_Util.percent(MNZ63K, MNK63K, 9, 63, 0);
				m_Util.percent(MNZ63H, MNK63H, 9, 63, 0);
				m_Util.percent(MNZ63C, MNK63C, 9, 63, 0);
			}

			m_ChSw = 1;
		}
		else {	// 税抜き
			//---->yoshida
			m_Util.percent(MNZ78K, MNK78K, 1, 78, 0);
			m_Util.percent(MNZ78H, MNK78H, 1, 78, 0);
			m_Util.percent(MNZ78C, MNK78C, 1, 78, 0);
			m_Util.percent(MNZ624K, MNK624K, 1, 62, 0);
			m_Util.percent(MNZ624H, MNK624H, 1, 62, 0);
			m_Util.percent(MNZ624C, MNK624C, 1, 62, 0);
			//--------------
			/*
			m_Util.percent(MNZ78K, MNK78K, 1, 63, 0);
			m_Util.percent(MNZ78H, MNK78H, 1, 63, 0);
			m_Util.percent(MNZ78C, MNK78C, 1, 63, 0);
			m_Util.percent(MNZ624K, MNK624K, 1, 40, 0);
			m_Util.percent(MNZ624H, MNK624H, 1, 40, 0);
			m_Util.percent(MNZ624C, MNK624C, 1, 40, 0);
			*/
			//<--------------
			if (m_pSnHeadData->SVmzsw == 1) {
				m_Util.percent(MNZ40K, MNK40K, 1, 40, 0);
				m_Util.percent(MNZ40H, MNK40H, 1, 40, 0);
				m_Util.percent(MNZ40C, MNK40C, 1, 40, 0);
				m_Util.percent(MENZZK, MENZKK, 1, 30, 0);
				m_Util.percent(MENZZH, MENZKH, 1, 30, 0);
				m_Util.percent(MENZZC, MENZKC, 1, 30, 0);
				m_Util.percent(MNZ63K, MNK63K, 1, 63, 0);
				m_Util.percent(MNZ63H, MNK63H, 1, 63, 0);
				m_Util.percent(MNZ63C, MNK63C, 1, 63, 0);
			}

			m_ChSw = 0;
		}
		memmove(DiagData.data_val, MNZ78K, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ78K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ78H, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ78H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ78C, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ78C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

		memmove(DiagData.data_val, MNZ624K, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ624K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ624H, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ624H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ624C, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ624C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}
	if (m_ChSw == 1) {
		tmpChSw = 1;
	}
	else {
		tmpChSw = 0;
	}
}

//-------------------------------------------------------------------
// 現在のコンボボックスの情報をチェックし、入力属性・背景色をセット
//-------------------------------------------------------------------
void CChangeTaxEx2::ComboCheck_AttrSet()
{
	int		flg = 0;
	short	index = 0;
	char	buf[512] = { 0 };

	switch (m_pSnHeadData->Sn_Sign4 & 0x81) {
	case 0x01:	// ロック
	case 0x81: // 非連動＆ロック
		if (m_pSnTanaData->Sn_tansw & 0x01) {
			flg = 1;
		}
		else {
			flg = 2;
		}
		diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, flg);
		diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, flg);
		diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, flg);

		for (index = IDMNK78K; index <= IDMNZ78C; index++) {
			diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, index, flg);
		}
		for (index = IDMNK624K; index <= IDMNZ624C; index++) {
			diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, index, flg);
		}
		/*
		c_cls(IDC_ICSDISPCTRL1, CIcsdisp8ctrl1);
		::ZeroMemory(buf, sizeof(buf));
		m_txt1.SetWindowTextA(buf);
		c_set(IDOK);
		*/
		break;
	case 0x80: // 非連動
		diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 0);

		if (tansw != 0x10 && tansw != 0x11) {
			diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 1);
			diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 1);
			for (index = IDMNK78K; index <= IDMNZ78C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}
			for (index = IDMNK624K; index <= IDMNZ624C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}
		}
		else {
			diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 0);
			diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 0);
			for (index = IDMNK78K; index <= IDMNZ78C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
			}
			for (index = IDMNK624K; index <= IDMNZ624C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
			}
		}

		/*
		c_cls(IDC_ICSDISPCTRL1, CIcsdisp8ctrl1);
		::ZeroMemory(buf, sizeof(buf));
		m_txt1.SetWindowTextA(buf);
		diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
		*/
		break;
	default: // 連動
		if (m_pSnTanaData->Sn_tansw & 0x01) {
			// 仕訳による棚卸調整有り時は表示のみ
			diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 1);
			diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 1);
			diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 1);

			for (index = IDMNK78K; index <= IDMNZ624C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}

			/*
			strcpy_s(buf, sizeof(buf), _T(" 棚卸調整仕訳が入力されています。\n 税額を修正する場合は、該当仕訳を変更してください。"));
			puts(IDC_ICSDISPCTRL1, buf, CIcsdisp8ctrl1);
			m_txt1.SetWindowTextA(buf);
			c_set(IDOK);
			*/
		}
		else {
			if (tansw != 0x10 && tansw != 0x11) {
				diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 0);
				diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 1);
				diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 1);
				for (index = IDMNK78K; index <= IDMNZ78C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
				for (index = IDMNK624K; index <= IDMNZ624C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
			}
			else {
				// 仕訳による棚卸調整無し時は入力可
				diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 0);
				diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 0);
				diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 0);
				for (index = IDMNK78K; index <= IDMNZ78C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
				}
				for (index = IDMNK624K; index <= IDMNZ624C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
				}
			}
			/*
			c_cls(IDC_ICSDISPCTRL1, CIcsdisp8ctrl1);
			::ZeroMemory(buf, sizeof(buf));
			m_txt1.SetWindowTextA(buf);
			diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
			*/

		}
		break;
	}

	diag_redraw(IDC_ICSDIAGCTRL1, CIcsdiagctrl);//再描画

	return;
}

void CChangeTaxEx2::OnBnClickedOk()
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (!(m_pSnTanaData->Sn_tansw & 0x01) || (m_pSnHeadData->Sn_Sign4 & 0x80)) {

		char	buf[512] = { 0 };

		if (m_CbSw == 2) {
			// 警告メッセージ
			sprintf_s(buf, sizeof(buf), _T("課税方式の切替えを解除しますか？"));
			if (ICSMessageBox(buf, MB_YESNO, 0, 0, this) == IDYES) {
				m_pSnTanaData->Sn_MENZsw = 0x00;
				tansw = 0x00;
				memset(MNK624K, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　課税に係る
				memset(MNZ624K, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
				memset(MNK624H, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　非課税に係る
				memset(MNZ624H, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
				memset(MNK624C, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　共通に係る
				memset(MNZ624C, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

				memset(MNK78K, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　課税に係る
				memset(MNZ78K, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
				memset(MNK78H, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　非課税に係る
				memset(MNZ78H, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
				memset(MNK78C, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　共通に係る
				memset(MNZ78C, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

				memset(m_pSnTanaData->Sn_MNK624, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)
				memset(m_pSnTanaData->Sn_MNZ624, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )
				memset(m_pSnTanaData->Sn_MNK78, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)
				memset(m_pSnTanaData->Sn_MNZ78, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )

				if (m_pSnHeadData->SVmzsw == 1) {
					memset(MNK40K, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　課税に係る
					memset(MNZ40K, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
					memset(MNK40H, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　非課税に係る
					memset(MNZ40H, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
					memset(MNK40C, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　共通に係る
					memset(MNZ40C, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

					memset(MENZKK, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　課税に係る
					memset(MENZZK, '\0', MONY_BUF_SIZE);	// 				 〃	   消費税額   (〃      )　〃
					memset(MENZKH, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　非課税に係る
					memset(MENZZH, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
					memset(MENZKC, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　共通に係る
					memset(MENZZC, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

					memset(MNK63K, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　課税に係る
					memset(MNZ63K, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
					memset(MNK63H, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　非課税に係る
					memset(MNZ63H, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
					memset(MNK63C, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　共通に係る
					memset(MNZ63C, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

					memset(m_pSnTanaData->Sn_MNK40, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)
					memset(m_pSnTanaData->Sn_MNZ40, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )
					memset(m_pSnTanaData->Sn_MENZK, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)
					memset(m_pSnTanaData->Sn_MENZZ, '\0', MONY_BUF_SIZE);	// 				 〃	   消費税額   (〃      )
					memset(m_pSnTanaData->Sn_MNK63, '\0', MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)
					memset(m_pSnTanaData->Sn_MNZ63, '\0', MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )
				}
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
				return;
			}
		}

		memmove(m_pSnTanaData->Sn_mnk624[0], MNK624K, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　課税に係る
		memmove(m_pSnTanaData->Sn_mnz624[0], MNZ624K, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(m_pSnTanaData->Sn_mnk624[1], MNK624H, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　非課税に係る
		memmove(m_pSnTanaData->Sn_mnz624[1], MNZ624H, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(m_pSnTanaData->Sn_mnk624[2], MNK624C, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)　共通に係る
		memmove(m_pSnTanaData->Sn_mnz624[2], MNZ624C, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

		memmove(m_pSnTanaData->Sn_mnk78[0], MNK78K, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　課税に係る
		memmove(m_pSnTanaData->Sn_mnz78[0], MNZ78K, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(m_pSnTanaData->Sn_mnk78[1], MNK78H, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　非課税に係る
		memmove(m_pSnTanaData->Sn_mnz78[1], MNZ78H, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		memmove(m_pSnTanaData->Sn_mnk78[2], MNK78C, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)　共通に係る
		memmove(m_pSnTanaData->Sn_mnz78[2], MNZ78C, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

		if (m_pSnHeadData->SVmzsw == 1) {
			memmove(m_pSnTanaData->Sn_mnk40[0], MNK40K, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　課税に係る
			memmove(m_pSnTanaData->Sn_mnz40[0], MNZ40K, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_mnk40[1], MNK40H, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　非課税に係る
			memmove(m_pSnTanaData->Sn_mnz40[1], MNZ40H, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_mnk40[2], MNK40C, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　共通に係る
			memmove(m_pSnTanaData->Sn_mnz40[2], MNZ40C, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_menzk[0], MENZKK, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　課税に係る
			memmove(m_pSnTanaData->Sn_menzz[0], MENZZK, MONY_BUF_SIZE);	// 				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_menzk[1], MENZKH, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　非課税に係る
			memmove(m_pSnTanaData->Sn_menzz[1], MENZZH, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_menzk[2], MENZKC, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　共通に係る
			memmove(m_pSnTanaData->Sn_menzz[2], MENZZC, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃

			memmove(m_pSnTanaData->Sn_mnk63[0], MNK63K, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　課税に係る
			memmove(m_pSnTanaData->Sn_mnz63[0], MNZ63K, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_mnk63[1], MNK63H, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　非課税に係る
			memmove(m_pSnTanaData->Sn_mnz63[1], MNZ63H, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
			memmove(m_pSnTanaData->Sn_mnk63[2], MNK63C, MONY_BUF_SIZE);	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)　共通に係る
			memmove(m_pSnTanaData->Sn_mnz63[2], MNZ63C, MONY_BUF_SIZE);	//				 〃	   消費税額   (〃      )　〃
		}

		m_pSnTanaData->Sn_MENZsw = tansw;

		//OKおされた時に税込み、税抜きサインを保存するようにする。キャンセルおされた場合も変わっていた為
		if (m_ChSw == 1) {//税込み
			m_pSnHeadData->SVsign |= 0x10;
		}
		else {//税抜き
			m_pSnHeadData->SVsign &= 0xef;
		}
	}

	ICSDialog::OnOK();

}


void CChangeTaxEx2::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ICSDialog::OnCancel();
}
BEGIN_EVENTSINK_MAP(CChangeTaxEx2, ICSDialog)
	ON_EVENT(CChangeTaxEx2, IDC_ICSDIAGCTRL1, 8, CChangeTaxEx2::ComboSelIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CChangeTaxEx2, IDC_ICSDIAGCTRL1, 1, CChangeTaxEx2::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTaxEx2, IDC_ICSDIAGCTRL1, 2, CChangeTaxEx2::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTaxEx2, IDC_ICSDIAGCTRL1, 3, CChangeTaxEx2::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CChangeTaxEx2::ComboSelIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	char			buf[512] = { 0 };
	DIAGRAM_DATA	diag_data;

	if (index == IDCOMBO) {
		diag_getdata(IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&diag_data, CIcsdiagctrl);
		m_CbSt = diag_data.data_combo;
		switch (m_CbSt) {
		case 0:	// 免税より課税
			m_CbSw = 0;
			tansw = 0x10;
			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("当期より、免税業者から課税業者となる場合の課税仕入等に係る調整税額"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("控除対象仕入額"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ78, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ624, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			break;

		case 1:	// 課税より免税
			m_CbSw = 1;
			tansw = 0x11;
			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("翌期より、課税業者から免税業者となる場合の課税仕入等に係る調整税額"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("控除対象外仕入額"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ78, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ624, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			break;

		default:
			m_CbSw = 2;
			tansw = 0x00;
			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T(" "));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			break;
		}

		ComboCheck_AttrSet();

	}

}


void CChangeTaxEx2::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch (index) {

	case IDNUKIZ:	// 控除対象仕入額　税抜き
		if (m_ChSw == 1) {
			m_ChSw = 0;
			diag_deledit(IDC_ICSDIAGCTRL1, CIcsdiagctrl);
			DiagData.data_check = 1;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			DiagData.data_check = 0;
			diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			ZeiChangeCalq();//計算
			diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);
		}
		break;

	case IDKOMIZ:	// 控除対象仕入額　税込み
		if (m_ChSw == 0) {
			m_ChSw = 1;
			diag_deledit(IDC_ICSDIAGCTRL1, CIcsdiagctrl);
			DiagData.data_check = 0;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			DiagData.data_check = 1;
			diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			ZeiChangeCalq();//計算
			diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);
		}
		break;

	default:

		ZeiChangeCalq();//計算
		break;
	}
}


void CChangeTaxEx2::EditOFFIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	char				CkMy[MONY_BUF_SIZE];
	char				buf[512] = { 0 };
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch (index) {
	case	IDCOMBO:	// 課税方式選択

		break;

	case	IDMNK624K:	// 調整課税仕入等に係る棚卸資産額 (6.24分)　課税に係る
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK624K, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK624K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK624K, CkMy)) {
			memmove(MNK624K, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				//------>yoshida
				m_Util.percent(MNZ624K, MNK624K, 11, 62, 0);//引数charの為、６２で渡す。モジュール側内部で6.24に切り替え
				//--------------
				//m_Util.percent(MNZ624K, MNK624K, 4, 40, 0);
				//<--------------
			}
			else {
				//------>yoshida
				m_Util.percent(MNZ624K, MNK624K, 1, 62, 0);//引数charの為、６２で渡す。モジュール側内部で6.24に切り替え
				//--------------
				//m_Util.percent(MNZ624K, MNK624K, 1, 40, 0);
				//<--------------
			}
			// 調整課税仕入等に係る消費税額 　(6.24分)　課税に係る
			memmove(DiagData.data_val, MNZ624K, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ624K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ624K:	//				 〃	   消費税額   (〃      )　〃
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ624K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ624K, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK624H:	// 調整課税仕入等に係る棚卸資産額 (6.24分)　非課税に係る
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK624H, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK624H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK624H, CkMy)) {
			memmove(MNK624H, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				//------>yoshida
				m_Util.percent(MNZ624H, MNK624H, 11, 62, 0);//引数charの為、６２で渡す。モジュール側内部で6.24に切り替え
				//--------------
				//m_Util.percent(MNZ624H, MNK624H, 4, 40, 0);
				//<-------------
			}
			else {
				//------>yoshida
				m_Util.percent(MNZ624H, MNK624H, 1, 62, 0);//引数charの為、６２で渡す。モジュール側内部で6.24に切り替え
				//--------------
				//m_Util.percent(MNZ624H, MNK624H, 1, 40, 0);
				//<-------------
			}
			// 調整課税仕入等に係る消費税額 　(6.24分)  非課税に係る
			memmove(DiagData.data_val, MNZ624H, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ624H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ624H:	//				 〃	   消費税額   (〃      )　〃
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ624H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ624H, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK624C:	// 調整課税仕入等に係る棚卸資産額 (6.24分)　共通に係る
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK624C, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK624C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK624C, CkMy)) {
			memmove(MNK624C, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				//------>yoshida
				m_Util.percent(MNZ624C, MNK624C, 11, 62, 0);//引数charの為、６２で渡す。モジュール側内部で6.24に切り替え
				//--------------
				//m_Util.percent(MNZ624C, MNK624C, 4, 40, 0);
				//<--------------
			}
			else {
				//------>yoshida
				m_Util.percent(MNZ624C, MNK624C, 1, 62, 0);//引数charの為、６２で渡す。モジュール側内部で6.24に切り替え
				//--------------
				//m_Util.percent(MNZ624C, MNK624C, 1, 40, 0);
				//<-------------
			}
			// 調整課税仕入等に係る消費税額 　(6.24分)　共通に係る
			memmove(DiagData.data_val, MNZ624C, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ624C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ624C:	//				 〃	   消費税額   (〃      )　〃
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ624C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ624C, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK78K:	// 調整課税仕入等に係る棚卸資産額 (7.8分)　課税に係る
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK78K, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK78K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK78K, CkMy)) {
			memmove(MNK78K, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				//------>yoshida
				m_Util.percent(MNZ78K, MNK78K, 12, 78, 0);
				//---------------
				//m_Util.percent(MNZ78K, MNK78K, 9, 63, 0);
				//<--------------
			}
			else {
				//------>yoshida
				m_Util.percent(MNZ78K, MNK78K, 1, 78, 0);
				//---------------
				//m_Util.percent(MNZ78K, MNK78K, 1, 63, 0);
				//<--------------
			}
			// 調整課税仕入等に係る消費税額 　(7.8分)　課税に係る
			memmove(DiagData.data_val, MNZ78K, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ78K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ78K:	//				 〃	   消費税額   (〃      )　〃
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ78K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ78K, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK78H:	// 調整課税仕入等に係る棚卸資産額 (7.8分)　非課税に係る
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK78H, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK78H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK78H, CkMy)) {
			memmove(MNK78H, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				//------>yoshida
				m_Util.percent(MNZ78H, MNK78H, 12, 78, 0);
				//--------------
				//m_Util.percent(MNZ78H, MNK78H, 9, 63, 0);
				//<--------------
			}
			else {
				//------>yoshida
				m_Util.percent(MNZ78H, MNK78H, 1, 78, 0);
				//---------------
				//m_Util.percent(MNZ78H, MNK78H, 1, 63, 0);
				//<-------------
			}
			// 調整課税仕入等に係る消費税額 　(7.8分)  非課税に係る
			memmove(DiagData.data_val, MNZ78H, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ78H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ78H:	//				 〃	   消費税額   (〃      )　〃
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ78H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ78H, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK78C:	// 調整課税仕入等に係る棚卸資産額 (7.8分)　共通に係る
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK78C, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK78C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK78C, CkMy)) {
			memmove(MNK78C, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				//------>yoshida
				m_Util.percent(MNZ78C, MNK78C, 12, 78, 0);
				//-------------
				//m_Util.percent(MNZ78C, MNK78C, 9, 63, 0);
				//<-------------
			}
			else {
				//------>yoshida
				m_Util.percent(MNZ78C, MNK78C, 1, 78, 0);
				//---------------
				//m_Util.percent(MNZ78C, MNK78C, 1, 63, 0);
				//<-------------
			}
			// 調整課税仕入等に係る消費税額 　(7.8分)　共通に係る
			memmove(DiagData.data_val, MNZ78C, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ78C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ78C:	//				 〃	   消費税額   (〃      )　〃
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ78C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ78C, DiagData.data_val, MONY_BUF_SIZE);
		break;
	default:
		break;
	}
}


void CChangeTaxEx2::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// ★左矢印と上矢印を押してもターミネーションに入ってこない。

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char				VK_FLG, VK_DMM;
	short				wTerm;

	if (nChar == 0) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	VK_FLG = VK_DMM = 0x00;
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

	switch (index) {

	case	IDCOMBO:	// 課税方式選択

		if (wTerm == VK_UP) {
			return;
		}
		if (wTerm == VK_ESCAPE) {
			OnCancel();
			return;
		}
		if (wTerm == VK_DOWN) {
			m_ICSDIAG1.ComboDropDown(IDCOMBO, TRUE);
			return;
		}
		diag_getdata(IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		if (DiagData.data_combo == 2) {
			c_set(IDOK);
			return;
		}
		// 次項目
		if ((wTerm == VK_RETURN) || (wTerm == VK_F3)) {
			if (m_ChSw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// 税込み
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// 税抜き
			}
		}
		break;

	case IDKOMIZ:	// 控除対象外仕入額　税込み
	case IDNUKIZ:	// 控除対象外仕入額　税抜き

		if ((wTerm == VK_LEFT) || (wTerm == VK_RIGHT)) {
			if (m_ChSw == 1) {
				m_ChSw = 0;
				//		m_pSnHeadData->SVsign &= 0xef;	// 税抜き
				DiagData.data_check = 1;
				diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				DiagData.data_check = 0;
				diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				ZeiChangeCalq();//計算
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);
			}
			else {
				m_ChSw = 1;//
//					m_pSnHeadData->SVsign |= 0x10;	// 税込み
				DiagData.data_check = 0;
				diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				DiagData.data_check = 1;
				diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				ZeiChangeCalq();//計算
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);
			}
		}
		// 次項目
		if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_DOWN)) {//
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78K, CIcsdiagctrl);
		}
		// 前項目へ
		if ((wTerm == VK_TAB) || (wTerm == VK_F2) || (wTerm == VK_UP)) {//
			diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
		}
		break;

	case	IDMNK624K:	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624K, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78C, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78C, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624H, CIcsdiagctrl);
		}
		break;

	case	IDMNZ624K:	//				 〃	   消費税額   (〃      )　〃
		// 次項目

		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624H, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624K, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78C, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624H, CIcsdiagctrl);
		}
		break;

	case	IDMNK624H:	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
			// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624H, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624K, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624K, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624C, CIcsdiagctrl);
		}
		break;

	case	IDMNZ624H:	//				 〃	   消費税額   (〃      )　〃
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624C, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624H, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624K, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624C, CIcsdiagctrl);
		}
		break;

	case	IDMNK624C:	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624C, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624H, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624H, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			c_set(IDOK);
		}
		break;

	case	IDMNZ624C:	//				 〃	   消費税額   (〃      )　〃
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			c_set(IDOK);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624C, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624H, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			c_set(IDOK);
		}
		break;


	case	IDMNK78K:	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78K, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			if (m_ChSw == 1) {//
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// 税込み
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// 税抜き
			}
		}
		// 上段
		if (wTerm == VK_UP) {
			if (m_ChSw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// 税込み
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// 税抜き
			}
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78H, CIcsdiagctrl);
		}
		break;

	case	IDMNZ78K:	//				 〃	   消費税額   (〃      )　〃
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78H, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78K, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			if (m_ChSw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// 税込み
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// 税抜き
			}
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78H, CIcsdiagctrl);
		}
		break;

	case	IDMNK78H:	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
			// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78H, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78K, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78K, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78C, CIcsdiagctrl);
		}
		break;

	case	IDMNZ78H:	//				 〃	   消費税額   (〃      )　〃
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78C, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78H, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78K, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78C, CIcsdiagctrl);
		}
		break;

	case	IDMNK78C:	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78C, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78H, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78H, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624K, CIcsdiagctrl);
		}
		break;

	case	IDMNZ78C:	//				 〃	   消費税額   (〃      )　〃
		// 次項目
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK624K, CIcsdiagctrl);
		}
		// 前項目へ
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK78C, CIcsdiagctrl);
		}
		// 上段
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ78H, CIcsdiagctrl);
		}
		// 下段
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624K, CIcsdiagctrl);
		}
		break;

	default:
		break;
	}
}


BOOL CChangeTaxEx2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。


	CWnd *pWnd = GetFocus();

	if (pMsg->message == WM_KEYDOWN) {

		if (GetCombineKeyDown(VK_SHIFT) && (VK_F1 <= pMsg->wParam) && (pMsg->wParam <= VK_F12)) {
			return TRUE;
		}

		if (pMsg->wParam == VK_TAB || pMsg->wParam == VK_RIGHT) {
			if (pWnd == GetDlgItem(IDOK)) {
				c_set(IDCANCEL);
				return TRUE;
			}
			else if (pWnd == GetDlgItem(IDCANCEL)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
				return TRUE;
			}
		}

		else if (pMsg->wParam == VK_UP) {
			if (pWnd == GetDlgItem(IDOK) || pWnd == GetDlgItem(IDCANCEL)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624C, CIcsdiagctrl);
				return TRUE;
			}
			//return TRUE;
		}

		else if (pMsg->wParam == VK_DOWN) {
			if (pWnd == GetDlgItem(IDOK) || pWnd == GetDlgItem(IDCANCEL)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
				return TRUE;
			}
		}

		else if (pMsg->wParam == VK_LEFT) {
			if (pWnd == GetDlgItem(IDOK)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ624C, CIcsdiagctrl);
				return TRUE;
			}
			//return TRUE;
		}

	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
