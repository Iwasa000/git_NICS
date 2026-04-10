// ShinFhyo1_02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo1_02.h"
#include "ShinFhyo1_02Idx.h"
#include "H31HyoView.h"

// CShinFhyo1_02 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo1_02, CSyzBaseDlg)

CShinFhyo1_02::CShinFhyo1_02(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo1_02::IDD, pParent)
	, m_pSnFhyo1_02Data(NULL)
	, m_pSnHeadData ( NULL )
	, m_pShinInfo(NULL)
	, m_pSyzSyukei(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
{
	memset ( F102_1A, '\0', sizeof ( F102_1A ) );
	memset ( F102_1B, '\0', sizeof ( F102_1B ) );
	memset ( F102_1C, '\0', sizeof ( F102_1C ) );
	memset ( F102_1X, '\0', sizeof ( F102_1X ) );

	memset ( F102_1_1A, '\0', sizeof ( F102_1_1A ) );
	memset ( F102_1_1B, '\0', sizeof ( F102_1_1B ) );
	memset ( F102_1_1C, '\0', sizeof ( F102_1_1C ) );
	memset ( F102_1_1X, '\0', sizeof ( F102_1_1X ) );

	memset ( F102_1_2C, '\0', sizeof ( F102_1_2C ) );
	memset ( F102_1_2X, '\0', sizeof ( F102_1_2X ) );

	memset ( F102_2A, '\0', sizeof ( F102_2A ) );
	memset ( F102_2B, '\0', sizeof ( F102_2B ) );
	memset ( F102_2C, '\0', sizeof ( F102_2C ) );
	memset ( F102_2X, '\0', sizeof ( F102_2X ) );

	memset ( F102_3A, '\0', sizeof ( F102_3A ) );
	memset ( F102_3B, '\0', sizeof ( F102_3B ) );
	memset ( F102_3C, '\0', sizeof ( F102_3C ) );
	memset ( F102_3X, '\0', sizeof ( F102_3X ) );

	memset ( F102_4A, '\0', sizeof ( F102_4A ) );
	memset ( F102_4B, '\0', sizeof ( F102_4B ) );
	memset ( F102_4C, '\0', sizeof ( F102_4C ) );
	memset ( F102_4X, '\0', sizeof ( F102_4X ) );

	memset ( F102_5A, '\0', sizeof ( F102_5A ) );
	memset ( F102_5B, '\0', sizeof ( F102_5B ) );
	memset ( F102_5C, '\0', sizeof ( F102_5C ) );
	memset ( F102_5X, '\0', sizeof ( F102_5X ) );

	memset ( F102_5_1A, '\0', sizeof ( F102_5_1A ) );
	memset ( F102_5_1B, '\0', sizeof ( F102_5_1B ) );
	memset ( F102_5_1C, '\0', sizeof ( F102_5_1C ) );
	memset ( F102_5_1X, '\0', sizeof ( F102_5_1X ) );

	memset ( F102_5_2C, '\0', sizeof ( F102_5_2C ) );
	memset ( F102_5_2X, '\0', sizeof ( F102_5_2X ) );

	memset ( F102_6A, '\0', sizeof ( F102_6A ) );
	memset ( F102_6B, '\0', sizeof ( F102_6B ) );
	memset ( F102_6C, '\0', sizeof ( F102_6C ) );
	memset ( F102_6X, '\0', sizeof ( F102_6X ) );

	memset ( F102_7A, '\0', sizeof ( F102_7A ) );
	memset ( F102_7B, '\0', sizeof ( F102_7B ) );
	memset ( F102_7C, '\0', sizeof ( F102_7C ) );
	memset ( F102_7X, '\0', sizeof ( F102_7X ) );

	memset ( F102_8A, '\0', sizeof ( F102_8A ) );
	memset ( F102_8B, '\0', sizeof ( F102_8B ) );
	memset ( F102_8C, '\0', sizeof ( F102_8C ) );
	memset ( F102_8X, '\0', sizeof ( F102_8X ) );

	memset ( F102_9A, '\0', sizeof ( F102_9A ) );
	memset ( F102_9B, '\0', sizeof ( F102_9B ) );
	memset ( F102_9C, '\0', sizeof ( F102_9C ) );
	memset ( F102_9X, '\0', sizeof ( F102_9X ) );


	memset ( F102_11B, '\0', sizeof ( F102_11B ) );
	memset ( F102_11C, '\0', sizeof ( F102_11C ) );
	memset ( F102_11X, '\0', sizeof ( F102_11X ) );

	memset ( F102_12B, '\0', sizeof ( F102_12B ) );
	memset ( F102_12C, '\0', sizeof ( F102_12C ) );
	memset ( F102_12X, '\0', sizeof ( F102_12X ) );

	memset ( F102_13B, '\0', sizeof ( F102_13B ) );
	memset ( F102_13C, '\0', sizeof ( F102_13C ) );
	memset ( F102_13X, '\0', sizeof ( F102_13X ) );

	memset ( F102_14B, '\0', sizeof ( F102_14B ) );
	memset ( F102_14C, '\0', sizeof ( F102_14C ) );
	memset ( F102_14X, '\0', sizeof ( F102_14X ) );

	memset ( F102_15B, '\0', sizeof ( F102_15B ) );
	memset ( F102_15C, '\0', sizeof ( F102_15C ) );
	memset ( F102_15X, '\0', sizeof ( F102_15X ) );


	m_curwnd = -1;

	F102_1c = 0;
	F102_2c = 0;
	F102_5c = 0;
}

CShinFhyo1_02::~CShinFhyo1_02()
{
}

void CShinFhyo1_02::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo1_02Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo1_02Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo1_02, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo1_02 メッセージ ハンドラ

BOOL CShinFhyo1_02::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	//コントロール一番最下のDiagのID指定。これで縦スクロール出る
	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 157;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo1_02::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo1_02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo1_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}
BEGIN_EVENTSINK_MAP(CShinFhyo1_02, CSyzBaseDlg)
	ON_EVENT(CShinFhyo1_02, IDC_ICSDIAG8CTRL2, 1, CShinFhyo1_02::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo1_02, IDC_ICSDIAG8CTRL2, 2, CShinFhyo1_02::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo1_02, IDC_ICSDIAG8CTRL2, 3, CShinFhyo1_02::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo1_02::EditONIcsdiag8ctrl2(short index)
{
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo1_02::EditOFFIcsdiag8ctrl2(short index)
{
	char				CkMy[MONY_BUF_SIZE];
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//---->
	char ksute1000[MONY_BUF_SIZE] = { 0 };
	m_pArith->l_input(ksute1000, _T("1000"));
	//<----

	switch (index) {

		case ID102_1A:		// 課税標準額　税率3%適用分　A
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			//------------>
			//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
			m_pArith->l_div(CkMy, CkMy, ksute1000);
			m_pArith->l_mul(CkMy, CkMy, ksute1000);

			dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1A, CkMy);
			//<------------

			if (m_pArith->l_cmp(CkMy, F102_1A)) {
				if (!(F102_1c & 0x08)) {
					F102_1c |= 0x01;
				}



				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_1B:		// 課税標準額　税率4%適用分　B
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			//------------>
			//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
			m_pArith->l_div(CkMy, CkMy, ksute1000);
			m_pArith->l_mul(CkMy, CkMy, ksute1000);

			dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1B, CkMy);
			//<------------


			if (m_pArith->l_cmp(CkMy, F102_1B)) {
				if (!(F102_1c & 0x80)) {
					F102_1c |= 0x10;
				}

				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_1C:		// 課税標準額　税率6.3%適用分　C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			//------------>
			//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
			m_pArith->l_div(CkMy, CkMy, ksute1000);
			m_pArith->l_mul(CkMy, CkMy, ksute1000);

			dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1C, CkMy);
			//<------------

			if (m_pArith->l_cmp(CkMy, F102_1C)) {
				if (!(F102_1c & 0x800)) {
					F102_1c |= 0x100;
				}

				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_1_1A:	// 課税資産の譲渡等の対価の額 税率3%適用分   A
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_1_1A)) {
				if (!(F102_1c & 0x08)) {
					F102_1c |= 0x01;
				}
				if (!(F102_2c & 0x08)) {
					F102_2c |= 0x01;
				}

				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_1_1B:	// 課税資産の譲渡等の対価の額 税率4%適用分   B
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_1_1B)) {
				if (!(F102_1c & 0x80)) {
					F102_1c |= 0x10;
				}
				if (!(F102_2c & 0x80)) {
					F102_2c |= 0x10;
				}

				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_1_1C:	// 課税資産の譲渡等の対価の額 税率6.3%適用分   C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_1_1C)) {
				if (!(F102_1c & 0x800)) {
					F102_1c |= 0x100;
				}
				if (!(F102_2c & 0x800)) {
					F102_2c |= 0x100;
				}

				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_1_2C :	// 特定課税仕入に係る支払い対価の額 税率6.3%適用分   C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_2C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_1_2C)) {
				if (!(F102_1c & 0x800)) {
					F102_1c |= 0x100;
				}
				if (!(F102_2c & 0x800)) {
					F102_2c |= 0x100;
				}

				SyukeiDataSet(index);
				f102data_all(1);
			}
			break;

		case ID102_2A :		// 消費税額 税率3%適用分   A
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_2A)) {
				F102_2c |= 0x08;
				F102_2c &= ~0x01;
			}

			SyukeiDataSet(index);
			f102data_all(2);
			break;

		case ID102_2B :		// 消費税額 税率4%適用分   B
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_2B)) {
				F102_2c |= 0x80;
				F102_2c &= ~0x10;
			}

			SyukeiDataSet(index);
			f102data_all(2);
			break;

		case ID102_2C :		// 消費税額 税率6.3%適用分   C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_2C)) {
				F102_2c |= 0x800;
				F102_2c &= ~0x100;	
			}

			SyukeiDataSet(index);
			f102data_all(2);
			break;

		case ID102_5A :		// 返還等対価に係る税額 税率3%適用分   A
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5A)) {
				F102_5c |= 0x08;
				F102_5c &= ~0x01;
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_5B :		// 返還等対価に係る税額 税率4%適用分   B
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5B)) {
				F102_5c |= 0x80;
				F102_5c &= ~0x10;
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_5C :		// 返還等対価に係る税額 税率6.3%適用分   C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5C)) {
				F102_5c |= 0x800;
				F102_5c &= ~0x100;
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_5_1A :	// 売上げの返還等対価に係る税額 税率3%適用分   A
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5_1A)) {
				if (!(F102_5c & 0x08)) {
					F102_5c |= 0x01;
				}
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_5_1B :	// 売上げの返還等対価に係る税額 税率4%適用分   B
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5_1B)) {
				if (!(F102_5c & 0x80)) {
					F102_5c |= 0x10;
				}
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_5_1C :	// 売上げの返還等対価に係る税額 税率6.3%適用分   C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5_1C)) {
				if (!(F102_5c & 0x800)) {
					F102_5c |= 0x100;
				}
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_5_2C :	// 特定課税仕入れの返還等対価に係る税額 税率6.3%適用分 C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_2C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);

			if (m_pArith->l_cmp(CkMy, F102_5_2C)) {
				if (!(F102_5c & 0x800)){
					F102_5c |= 0x100;
				}
			}

			SyukeiDataSet(index);
			f102data_all(5);
			break;

		case ID102_6A :		// 貸倒れに係る税額 税率3%適用分   A
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			
			SyukeiDataSet(index);
			f102data_all(6);
			break;

		case ID102_6B :		// 貸倒れに係る税額 税率4%適用分   B
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );

			SyukeiDataSet(index);
			f102data_all(6);
			break;
		case ID102_6C :		// 貸倒れに係る税額 税率6.3%適用分  C
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );

			SyukeiDataSet(index);
			f102data_all(6);
			break;

		default :
			break;
	}
}

void CShinFhyo1_02::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TabをTabに、TabをEnterに変換
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
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
		pos = UpDownFocusMoveCheck( index, 1 );
		m_Fhyo1_02Diag02.SetPosition(pos);
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck( index, 0 );
		m_Fhyo1_02Diag02.SetPosition(pos);
	}
	if ( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ) {
		m_Fhyo1_02Diag02.SetNextPosition();
	}
	else if ( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ) {
		m_Fhyo1_02Diag02.SetPrevPosition();
	}

	if (wTerm == VK_DELETE) {
		switch ( index ) {
			case ID102_1A:		// 課税標準額
				F102_1c &= ~0x08;
				F102_1c |= 0x01;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_1B:
				F102_1c &= ~0x80;
				F102_1c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_1C:
				F102_1c &= ~0x800;
				F102_1c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;



			case ID102_1_1A:	// 課税資産の譲渡等の対価の額
				F102_1c &= ~0x08;
				F102_1c |= 0x01;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_1_1B:
				F102_1c &= ~0x80;
				F102_1c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_1_1C:
				F102_1c &= ~0x800;
				F102_1c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;



			case ID102_1_2C:	// 特定課税仕入れに係る支払い対価の額
				F102_1c &= ~0x800;
				F102_1c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;



			case ID102_2A:		// 消費税額
				F102_2c &= ~0x08;
				(*m_pSyzSyukei)->ResetShinkokuData( _T("310401") );
				f102data_all(2);
				m_Fhyo1_02Diag02.SetPosition(index);
				/*F102_2c &= ~0x08;
				F102_2c |= 0x01;
				memset(F102_2A, '\0', MONY_BUF_SIZE);
				dsp_cls(IDC_ICSDIAG8CTRL2, ID102_2A);
				f102_2data();*/
				break;

			case ID102_2B:
				F102_2c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("310402"));
				f102data_all(2);
				m_Fhyo1_02Diag02.SetPosition(index);
				/*F102_2c &= ~0x80;
				F102_2c |= 0x10;
				memset(F102_2B, '\0', MONY_BUF_SIZE);
				dsp_cls(IDC_ICSDIAG8CTRL2, ID102_2B);
				f102_2data();*/
				break;

			case ID102_2C:
				F102_2c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("310403"));
				f102data_all(2);
				m_Fhyo1_02Diag02.SetPosition(index);
				/*F102_2c &= ~0x800;
				F102_2c |= 0x100;
				memset(F102_2C, '\0', MONY_BUF_SIZE);
				dsp_cls(IDC_ICSDIAG8CTRL2, ID102_2B);
				f102_2data();*/
				break;



			case ID102_5A:		// 返還等対価に係る税額
				F102_5c &= ~0x08;
				F102_5c |= 0x01;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_5B:
				F102_5c &= ~0x80;
				F102_5c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_5C:
				F102_5c &= ~0x800;
				F102_5c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;



			case ID102_5_1A:	// 売上の返還等対価に係る税額
				F102_5c &= ~0x08;
				F102_5c |= 0x01;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_5_1B:
				F102_5c &= ~0x80;
				F102_5c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_5_1C:
				F102_5c &= ~0x800;
				F102_5c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_5_2C:	// 特定課税仕入れの返還等対価に係る税額
				F102_5c &= ~0x800;
				F102_5c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_6A:		// 貸倒れに係る税額
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_6B:
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID102_6C:
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;


			default :
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
		}
	}

	int nowpos = m_Fhyo1_02Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect(&rectA);
	diag_getrect(IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl);
	if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ) {
		int cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}

//=============================
// 初期化
//-----------------------------
// 返送値
//		0	:	正常
//		-1	:	エラー
//=============================
int CShinFhyo1_02::Init()
{
	// データ読込み
	ReadData( m_FrSw );
	m_FrSw = 1;

	// 読み込みデータを集計モジュールにセット
	ReadData2();

	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();


	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はメッセージ非表示
		m_Fhyo1_02Diag01.ModifyItem ( 4, DIAG_MDFY_TRANSPARENT );	
	}

	// 画面出力
	DispFh1_02();

	// 初期ポジションセット
	SetItemPosition();

	return 0;
}

//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnFhyo1_02Data	：	付表1－2データ読み込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込クラス
//		pShinInfo		：　共通情報
//---------------------------------------------------------
// 返送値
//			0			:	正常
//			-1			:	エラー
//=========================================================
int CShinFhyo1_02::InitInfo(CH31SnFhyo1_02Data **pSnFhyo1_02Data ,CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CSyzSyukei **pSyzSyukei)
{
	ASSERT(pSnFhyo1_02Data);
	if (pSnFhyo1_02Data == NULL) {
		return -1;
	}
	m_pSnFhyo1_02Data = pSnFhyo1_02Data;

	ASSERT( pSnHeadData );
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pShinInfo);
	if (pShinInfo == NULL) {
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT(pArith);
	if (pArith == NULL) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT(pSyzSyukei);
	if (pSyzSyukei == NULL) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	return 0;
}

//=============================
// ダイアグラム初期属性セット
//-----------------------------
// 返送値
//		0	:	正常
//		-1	:	エラー
//=============================
int CShinFhyo1_02::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo1_02[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo1_02[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo1_02[cnt].UnConnect;
				int st = ChkTkkzIdx ( Fhyo1_02[cnt].Index );
				if ( st == 1 ) {
					if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
				}
				else if ( st == 2 ) {
					if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
						Flg = 0;
					}
					else {
						Flg = 1;
					}
				}
			}
			else {
				Flg = Fhyo1_02[cnt].Connect;
			}
		}
		
		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo1_02[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo1_02[cnt].Connect );
		cnt++;

	}

	m_Fhyo1_02Diag02.EnableDelete ( 0 );

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
//		2		:	特定課税仕入で入力不可になるインデックス
//============================================================
int CShinFhyo1_02::ChkTkkzIdx(short idx)
{
	int rSt = 0;
	//---->yoshida190613
	//特定課税仕入れでなくても、 課税資産の譲渡等の対価の額が入力可とならないといけない感じなので
	//課税標準額を入力不可とし、課税資産の譲渡等の対価の額は入力可とする仕様に変更
	if (((ID102_1_2C <= idx) && (idx <= ID102_1_2X)) || ((ID102_5_2C <= idx) && (idx <= ID102_5_2X))) {
		rSt = 1;
	}

	//yoshida190622 入力不可に
//	else if (((ID102_5A <= idx) && (idx <= ID102_5X))) {
//		rSt = 2;
//	}
	//-------------------
	/*
	if ( ( ( ID102_1_1A <= idx ) && ( idx <= ID102_1_2X ) ) || ( ( ID102_5_1A <= idx ) && ( idx <= ID102_5_2X ) ) ) {
		rSt = 1;
	}
	else if ( ( ( ID102_1A <= idx ) && ( idx <= ID102_1X ) ) || ( ( ID102_5A <= idx ) && ( idx <= ID102_5X ) ) ) {
		rSt = 2;
	}
	*/
	//<-------------------

	return rSt;
}

//-----------------------------------------------------------------------------
// 特定課税仕入に関する項目の金額クリア
//-----------------------------------------------------------------------------
void CShinFhyo1_02::ClearTkkzMony()
{
	m_pArith->l_clear( F102_1_2C );
	m_pArith->l_clear( F102_1_2X );
//	memmove( F102_1A, F102_1_1A, MONY_BUF_SIZE );
//	memmove( F102_1B, F102_1_1B, MONY_BUF_SIZE );
//	memmove( F102_1C, F102_1_1C, MONY_BUF_SIZE );
//	memmove( F102_1X, F102_1_1X, MONY_BUF_SIZE );

	m_pArith->l_clear( F102_5_2C );
	m_pArith->l_clear( F102_5_2X );
//	memmove( F102_5A, F102_5_1A, MONY_BUF_SIZE );
//	memmove( F102_5B, F102_5_1B, MONY_BUF_SIZE );
//	memmove( F102_5C, F102_5_1C, MONY_BUF_SIZE );
//	memmove( F102_5X, F102_5_1X, MONY_BUF_SIZE );

	DispFh1_02();

	WriteData();
}

//======================================
// 初期ポジションセット
//======================================
void CShinFhyo1_02::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo1_02Diag02.GetPosition();
		if ( ( index = m_Fhyo1_02Diag02.GetPosition() ) != -1 ) {
			m_Fhyo1_02Diag02.SetFocus();	// ダミーでセット
			m_Fhyo1_02Diag02.SetPosition ( index );
			m_Fhyo1_02Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo1_02Diag01.SetFocus();	// ダミーでセット
	m_Fhyo1_02Diag02.SetFocus();

	//　課税標準率A欄へ
	/*
	if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
		diag_setposition ( IDC_ICSDIAG8CTRL2, ID102_1A, CIcsdiagctrl );
	}
	else {
		diag_setposition ( IDC_ICSDIAG8CTRL2, ID102_1A, CIcsdiagctrl );
	}
	*/

	if ( (*m_pSnHeadData)->Sn_Sign4&0x80 ) {
		diag_setposition(IDC_ICSDIAG8CTRL2, ID102_1_1A, CIcsdiagctrl);//yoshida190613
		/*
		if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
			diag_setposition(IDC_ICSDIAG8CTRL2, ID102_1_1A, CIcsdiagctrl);
		}
		else {
			diag_setposition(IDC_ICSDIAG8CTRL2, ID102_1A, CIcsdiagctrl);
		}*/
	}
	else {
		diag_setposition ( IDC_ICSDIAG8CTRL2, ID102_1A, CIcsdiagctrl );
	}
	
}

//=================================================
// 入力項目色変え
//-------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=================================================
void CShinFhyo1_02::ChangeColor(unsigned short id, short index, int sign)
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

//==============================================
// フォーカス検索
//----------------------------------------------
// 引数
//		Index	:	ダイアグラム内インデックス
//		Move	:	0 = 下, 1 = 上
//----------------------------------------------
// 返送値
//		次のフォーカスインデックス
//==============================================
int CShinFhyo1_02::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY1_02CNT; i++ ) {
		if ( Index == FCS_Fhyo1_02[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo1_02[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY1_02CNT; j++ ) {
			m_Fhyo1_02Diag02.GetAttribute ( FCS_Fhyo1_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo1_02[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo1_02Diag02.GetAttribute ( FCS_Fhyo1_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo1_02[j].IDNo;
				break;
			}
		}
	}

	return idx;
}


//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数		pas	：　
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo1_02::ReadData( int pas )
{
	// 課税標準額
	memmove(F102_1A, (*m_pSnFhyo1_02Data)->Sn_F102_1A, MONY_BUF_SIZE);
	memmove(F102_1B, (*m_pSnFhyo1_02Data)->Sn_F102_1B, MONY_BUF_SIZE);
	memmove(F102_1C, (*m_pSnFhyo1_02Data)->Sn_F102_1C, MONY_BUF_SIZE);
	memmove(F102_1X, (*m_pSnFhyo1_02Data)->Sn_F102_1X, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額
//	if ( (*m_pSnHeadData)->IsNeedTkkzDisp() ) {
		memmove(F102_1_1A, (*m_pSnFhyo1_02Data)->Sn_F102_1_1A, MONY_BUF_SIZE);
		memmove(F102_1_1B, (*m_pSnFhyo1_02Data)->Sn_F102_1_1B, MONY_BUF_SIZE);
		memmove(F102_1_1C, (*m_pSnFhyo1_02Data)->Sn_F102_1_1C, MONY_BUF_SIZE);
		memmove(F102_1_1X, (*m_pSnFhyo1_02Data)->Sn_F102_1_1X, MONY_BUF_SIZE);
//	}
//	else {
//		memset(F102_1_1A, '\0', MONY_BUF_SIZE);
//		memset(F102_1_1B, '\0', MONY_BUF_SIZE);
//		memset(F102_1_1C, '\0', MONY_BUF_SIZE);
//		memset(F102_1_1X, '\0', MONY_BUF_SIZE);
//	}

	// 特定課税仕入に係る支払い対価の額
	if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
		memmove(F102_1_2C, (*m_pSnFhyo1_02Data)->Sn_F102_1_2C, MONY_BUF_SIZE);
		memmove(F102_1_2X, (*m_pSnFhyo1_02Data)->Sn_F102_1_2X, MONY_BUF_SIZE);
	}
	else {
		memset(F102_1_2C, '\0', MONY_BUF_SIZE);
		memset(F102_1_2X, '\0', MONY_BUF_SIZE);
	}

	// 消費税額
	memmove(F102_2A, (*m_pSnFhyo1_02Data)->Sn_F102_2A, MONY_BUF_SIZE);
	memmove(F102_2B, (*m_pSnFhyo1_02Data)->Sn_F102_2B, MONY_BUF_SIZE);
	memmove(F102_2C, (*m_pSnFhyo1_02Data)->Sn_F102_2C, MONY_BUF_SIZE);
	memmove(F102_2X, (*m_pSnFhyo1_02Data)->Sn_F102_2X, MONY_BUF_SIZE);
	F102_2c = (*m_pSnFhyo1_02Data)->Sn_F102_2c;


	// 控除過大調整税額
	memmove(F102_3A, (*m_pSnFhyo1_02Data)->Sn_F102_3A, MONY_BUF_SIZE);
	memmove(F102_3B, (*m_pSnFhyo1_02Data)->Sn_F102_3B, MONY_BUF_SIZE);
	memmove(F102_3C, (*m_pSnFhyo1_02Data)->Sn_F102_3C, MONY_BUF_SIZE);
	memmove(F102_3X, (*m_pSnFhyo1_02Data)->Sn_F102_3X, MONY_BUF_SIZE);

	// 控除対象仕入税額
	memmove(F102_4A, (*m_pSnFhyo1_02Data)->Sn_F102_4A, MONY_BUF_SIZE);
	memmove(F102_4B, (*m_pSnFhyo1_02Data)->Sn_F102_4B, MONY_BUF_SIZE);
	memmove(F102_4C, (*m_pSnFhyo1_02Data)->Sn_F102_4C, MONY_BUF_SIZE);
	memmove(F102_4X, (*m_pSnFhyo1_02Data)->Sn_F102_4X, MONY_BUF_SIZE);

	// 返還等対価に係る税額
	memmove(F102_5A, (*m_pSnFhyo1_02Data)->Sn_F102_5A, MONY_BUF_SIZE);
	memmove(F102_5B, (*m_pSnFhyo1_02Data)->Sn_F102_5B, MONY_BUF_SIZE);
	memmove(F102_5C, (*m_pSnFhyo1_02Data)->Sn_F102_5C, MONY_BUF_SIZE);	
	memmove(F102_5X, (*m_pSnFhyo1_02Data)->Sn_F102_5X, MONY_BUF_SIZE);

	// 売上げの返還等対価に係る税額
//	if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
		memmove(F102_5_1A, (*m_pSnFhyo1_02Data)->Sn_F102_5_1A, MONY_BUF_SIZE);
		memmove(F102_5_1B, (*m_pSnFhyo1_02Data)->Sn_F102_5_1B, MONY_BUF_SIZE);
		memmove(F102_5_1C, (*m_pSnFhyo1_02Data)->Sn_F102_5_1C, MONY_BUF_SIZE);
		memmove(F102_5_1X, (*m_pSnFhyo1_02Data)->Sn_F102_5_1X, MONY_BUF_SIZE);
//	}
//	else {
//		memset(F102_5_1A, '\0', MONY_BUF_SIZE);
//		memset(F102_5_1B, '\0', MONY_BUF_SIZE);
//		memset(F102_5_1C, '\0', MONY_BUF_SIZE);
//		memset(F102_5_1X, '\0', MONY_BUF_SIZE);
//	}

	// 特定課税仕入れの返還等対価に係る税額
	if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
		memmove(F102_5_2C, (*m_pSnFhyo1_02Data)->Sn_F102_5_2C, MONY_BUF_SIZE);
		memmove(F102_5_2X, (*m_pSnFhyo1_02Data)->Sn_F102_5_2X, MONY_BUF_SIZE);
	}
	else {
		memset(F102_5_2C, '\0', MONY_BUF_SIZE);
		memset(F102_5_2X, '\0', MONY_BUF_SIZE);
	}

	// 貸倒れに係る税額
	memmove(F102_6A, (*m_pSnFhyo1_02Data)->Sn_F102_6A, MONY_BUF_SIZE);
	memmove(F102_6B, (*m_pSnFhyo1_02Data)->Sn_F102_6B, MONY_BUF_SIZE);
	memmove(F102_6C, (*m_pSnFhyo1_02Data)->Sn_F102_6C, MONY_BUF_SIZE);
	memmove(F102_6X, (*m_pSnFhyo1_02Data)->Sn_F102_6X, MONY_BUF_SIZE);


	
	// 控除税額小計
	memmove(F102_7A, (*m_pSnFhyo1_02Data)->Sn_F102_7A, MONY_BUF_SIZE);
	memmove(F102_7B, (*m_pSnFhyo1_02Data)->Sn_F102_7B, MONY_BUF_SIZE);
	memmove(F102_7C, (*m_pSnFhyo1_02Data)->Sn_F102_7C, MONY_BUF_SIZE);
	memmove(F102_7X, (*m_pSnFhyo1_02Data)->Sn_F102_7X, MONY_BUF_SIZE);

	// 控除不足還付税額
	memmove(F102_8A, (*m_pSnFhyo1_02Data)->Sn_F102_8A, MONY_BUF_SIZE);
	memmove(F102_8B, (*m_pSnFhyo1_02Data)->Sn_F102_8B, MONY_BUF_SIZE);
	memmove(F102_8C, (*m_pSnFhyo1_02Data)->Sn_F102_8C, MONY_BUF_SIZE);
	memmove(F102_8X, (*m_pSnFhyo1_02Data)->Sn_F102_8X, MONY_BUF_SIZE);

	// 差引税額
	memmove(F102_9A, (*m_pSnFhyo1_02Data)->Sn_F102_9A, MONY_BUF_SIZE);
	memmove(F102_9B, (*m_pSnFhyo1_02Data)->Sn_F102_9B, MONY_BUF_SIZE);
	memmove(F102_9C, (*m_pSnFhyo1_02Data)->Sn_F102_9C, MONY_BUF_SIZE);
	memmove(F102_9X, (*m_pSnFhyo1_02Data)->Sn_F102_9X, MONY_BUF_SIZE);

	// 控除不足還付税額
	memmove(F102_11B, (*m_pSnFhyo1_02Data)->Sn_F102_11B, MONY_BUF_SIZE);
	memmove(F102_11C, (*m_pSnFhyo1_02Data)->Sn_F102_11C, MONY_BUF_SIZE);
	memmove(F102_11X, (*m_pSnFhyo1_02Data)->Sn_F102_11X, MONY_BUF_SIZE);

	// 差引税額
	memmove(F102_12B, (*m_pSnFhyo1_02Data)->Sn_F102_12B, MONY_BUF_SIZE);
	memmove(F102_12C, (*m_pSnFhyo1_02Data)->Sn_F102_12C, MONY_BUF_SIZE);
	memmove(F102_12X, (*m_pSnFhyo1_02Data)->Sn_F102_12X, MONY_BUF_SIZE);

	// 合計差引地方消費税の課税標準となる消費税額
	memmove(F102_13B, (*m_pSnFhyo1_02Data)->Sn_F102_13B, MONY_BUF_SIZE);
	memmove(F102_13C, (*m_pSnFhyo1_02Data)->Sn_F102_13C, MONY_BUF_SIZE);
	memmove(F102_13X, (*m_pSnFhyo1_02Data)->Sn_F102_13X, MONY_BUF_SIZE);

	// 還付額
	memmove(F102_14B, (*m_pSnFhyo1_02Data)->Sn_F102_14B, MONY_BUF_SIZE);
	memmove(F102_14C, (*m_pSnFhyo1_02Data)->Sn_F102_14C, MONY_BUF_SIZE);
	memmove(F102_14X, (*m_pSnFhyo1_02Data)->Sn_F102_14X, MONY_BUF_SIZE);

	// 納税額
	memmove(F102_15B, (*m_pSnFhyo1_02Data)->Sn_F102_15B, MONY_BUF_SIZE);
	memmove(F102_15C, (*m_pSnFhyo1_02Data)->Sn_F102_15C, MONY_BUF_SIZE);
	memmove(F102_15X, (*m_pSnFhyo1_02Data)->Sn_F102_15X, MONY_BUF_SIZE);


	return 0;
}

//-----------------------------------------------------------------------------
// 付表1-02 読み込みデータを集計モジュールにセット
//-----------------------------------------------------------------------------
void CShinFhyo1_02::ReadData2()
{
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) { // 非連動の場合

		//------------>	
		/*
		char ksute1000[MONY_BUF_SIZE] = { 0 };
		m_pArith->l_input(ksute1000, _T("1000"));
		
		//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
		m_pArith->l_div(F102_1A, F102_1A, ksute1000);
		m_pArith->l_mul(F102_1A, F102_1A, ksute1000);

		m_pArith->l_div(F102_1B, F102_1B, ksute1000);
		m_pArith->l_mul(F102_1B, F102_1B, ksute1000);

		m_pArith->l_div(F102_1C, F102_1C, ksute1000);
		m_pArith->l_mul(F102_1C, F102_1C, ksute1000);
		*/
		//<------------

		//---->yoshida190613
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310101"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310102"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310103"));
		//------------------
		/*
		(*m_pSyzSyukei)->SetShinkokuData(_T("310101"), F102_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310102"), F102_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310103"), F102_1C);
		*/
		//<------------------
		(*m_pSyzSyukei)->SetShinkokuData(_T("310201"), F102_1_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310202"), F102_1_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310203"), F102_1_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310303"), F102_1_2C);

		if (F102_2c & 0x08) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("310401"), F102_2A);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("310401"));

		}
		if (F102_2c & 0x80) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("310402"), F102_2B);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("310402"));
		}
		if (F102_2c & 0x800) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("310403"), F102_2C);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("310403"));
		}

		//---->yoshida190622
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310701"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310702"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310703"));
		//------------------
//		(*m_pSyzSyukei)->SetShinkokuData(_T("310701"), F102_5A);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("310702"), F102_5B);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("310703"), F102_5C);
		//<-------------------
		(*m_pSyzSyukei)->SetShinkokuData(_T("310801"), F102_5_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310802"), F102_5_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310803"), F102_5_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310903"), F102_5_2C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311001"), F102_6A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311002"), F102_6B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311003"), F102_6C);
	}
	else {

	}

}


//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo1_02::WriteData()
{
	// 課税標準額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1A, F102_1A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1B, F102_1B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1C, F102_1C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1X, F102_1X, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1_1A, F102_1_1A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1_1B, F102_1_1B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1_1C, F102_1_1C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1_1X, F102_1_1X, MONY_BUF_SIZE);

	// 特定課税仕入に係る支払い対価の額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1_2C, F102_1_2C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_1_2X, F102_1_2X, MONY_BUF_SIZE);
	

	// 消費税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_2A, F102_2A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_2B, F102_2B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_2C, F102_2C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_2X, F102_2X, MONY_BUF_SIZE);
	(*m_pSnFhyo1_02Data)->Sn_F102_2c = F102_2c;

	// 控除過大調整税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_3A, F102_3A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_3B, F102_3B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_3C, F102_3C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_3X, F102_3X, MONY_BUF_SIZE);

	// 控除対象仕入税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_4A, F102_4A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_4B, F102_4B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_4C, F102_4C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_4X, F102_4X, MONY_BUF_SIZE);

	// 返還等対価に係る税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5A, F102_5A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5B, F102_5B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5C, F102_5C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5X, F102_5X, MONY_BUF_SIZE);

	// 売上げの返還等対価に係る税額	
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5_1A, F102_5_1A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5_1B, F102_5_1B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5_1C, F102_5_1C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5_1X, F102_5_1X, MONY_BUF_SIZE);


	// 特定課税仕入れの返還等対価に係る税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5_2C, F102_5_2C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_5_2X, F102_5_2X, MONY_BUF_SIZE);
	
	// 貸倒れに係る税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_6A, F102_6A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_6B, F102_6B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_6C, F102_6C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_6X, F102_6X, MONY_BUF_SIZE);


	//----->yoshida190610
	//これより下Write処理間違っていたので修正

	// 控除税額小計
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_7A, F102_7A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_7B, F102_7B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_7C, F102_7C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_7X, F102_7X, MONY_BUF_SIZE);


	// 控除不足還付税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_8A, F102_8A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_8B, F102_8B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_8C, F102_8C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_8X, F102_8X, MONY_BUF_SIZE);

	// 差引税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_9A, F102_9A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_9B, F102_9B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_9C, F102_9C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_9X, F102_9X, MONY_BUF_SIZE);


	// 控除不足還付税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_11B, F102_11B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_11C, F102_11C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_11X, F102_11X, MONY_BUF_SIZE);


	// 差引税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_12B, F102_12B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_12C, F102_12C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_12X, F102_12X, MONY_BUF_SIZE);


	// 合計差引地方消費税の課税標準となる消費税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_13B, F102_13B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_13C, F102_13C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_13X, F102_13X, MONY_BUF_SIZE);

	// 還付額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_14B, F102_14B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_14C, F102_14C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_14X, F102_14X, MONY_BUF_SIZE);

	// 納税額
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_15B, F102_15B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_15C, F102_15C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_02Data)->Sn_F102_15X, F102_15X, MONY_BUF_SIZE);

	//----------------------------
	/*


	// 控除税額小計
	memmove(F102_7A, (*m_pSnFhyo1_02Data)->Sn_F102_7A, MONY_BUF_SIZE);
	memmove(F102_7B, (*m_pSnFhyo1_02Data)->Sn_F102_7B, MONY_BUF_SIZE);
	memmove(F102_7C, (*m_pSnFhyo1_02Data)->Sn_F102_7C, MONY_BUF_SIZE);
	memmove(F102_7X, (*m_pSnFhyo1_02Data)->Sn_F102_7X, MONY_BUF_SIZE);

	// 控除不足還付税額
	memmove(F102_8A, (*m_pSnFhyo1_02Data)->Sn_F102_8A, MONY_BUF_SIZE);
	memmove(F102_8B, (*m_pSnFhyo1_02Data)->Sn_F102_8B, MONY_BUF_SIZE);
	memmove(F102_8C, (*m_pSnFhyo1_02Data)->Sn_F102_8C, MONY_BUF_SIZE);
	memmove(F102_8X, (*m_pSnFhyo1_02Data)->Sn_F102_8X, MONY_BUF_SIZE);

	
	// 差引税額
	memmove(F102_9A, (*m_pSnFhyo1_02Data)->Sn_F102_9A, MONY_BUF_SIZE);
	memmove(F102_9B, (*m_pSnFhyo1_02Data)->Sn_F102_9B, MONY_BUF_SIZE);
	memmove(F102_9C, (*m_pSnFhyo1_02Data)->Sn_F102_9C, MONY_BUF_SIZE);
	memmove(F102_9X, (*m_pSnFhyo1_02Data)->Sn_F102_9X, MONY_BUF_SIZE);

	// 控除不足還付税額
	memmove(F102_11B, (*m_pSnFhyo1_02Data)->Sn_F102_11B, MONY_BUF_SIZE);
	memmove(F102_11C, (*m_pSnFhyo1_02Data)->Sn_F102_11C, MONY_BUF_SIZE);
	memmove(F102_11X, (*m_pSnFhyo1_02Data)->Sn_F102_11X, MONY_BUF_SIZE);

	// 差引税額
	memmove(F102_12B, (*m_pSnFhyo1_02Data)->Sn_F102_12B, MONY_BUF_SIZE);
	memmove(F102_12C, (*m_pSnFhyo1_02Data)->Sn_F102_12C, MONY_BUF_SIZE);
	memmove(F102_12X, (*m_pSnFhyo1_02Data)->Sn_F102_12X, MONY_BUF_SIZE);

	// 合計差引地方消費税の課税標準となる消費税額
	memmove(F102_13B, (*m_pSnFhyo1_02Data)->Sn_F102_13B, MONY_BUF_SIZE);
	memmove(F102_13C, (*m_pSnFhyo1_02Data)->Sn_F102_13C, MONY_BUF_SIZE);
	memmove(F102_13X, (*m_pSnFhyo1_02Data)->Sn_F102_13X, MONY_BUF_SIZE);

	// 還付額
	memmove(F102_14B, (*m_pSnFhyo1_02Data)->Sn_F102_14B, MONY_BUF_SIZE);
	memmove(F102_14C, (*m_pSnFhyo1_02Data)->Sn_F102_14C, MONY_BUF_SIZE);
	memmove(F102_14X, (*m_pSnFhyo1_02Data)->Sn_F102_14X, MONY_BUF_SIZE);

	// 納税額
	memmove(F102_15B, (*m_pSnFhyo1_02Data)->Sn_F102_15B, MONY_BUF_SIZE);
	memmove(F102_15C, (*m_pSnFhyo1_02Data)->Sn_F102_15C, MONY_BUF_SIZE);
	memmove(F102_15X, (*m_pSnFhyo1_02Data)->Sn_F102_15X, MONY_BUF_SIZE);
	*/
	//<---------------------------


	return 0;
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：　ダイアグラムID
//		Index	：　ダイアグラム内インデックス
//		Mony	：　表示金額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
	diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}


//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数 Id		：　ダイアグラムID	
//		Index	：　ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinFhyo1_02::dsp_cls(unsigned short Id, short Index)
{
	
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
		case 4:		// 課税期間　自
		case 5:		// 課税期間　至
		case 3:		// 氏名又は、名称
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
		default :	// 金額
			memset(DiagData.data_val, '\0', MONY_BUF_SIZE);
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}
}


//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo1_02::GetAllDiagData()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 課税標準額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1X, DiagData.data_val, MONY_BUF_SIZE);

	// 1_1. 課税資産の譲渡等の対価の額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1_1A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1_1B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1_1C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1_1X, DiagData.data_val, MONY_BUF_SIZE);

	// 1_2. 特定課税仕入れに係る支払い対価の額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1_2C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_1_2X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_1_2X, DiagData.data_val, MONY_BUF_SIZE);

	// 2. 消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_2A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_2B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_2C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_2X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_2X, DiagData.data_val, MONY_BUF_SIZE);

	// 3. 控除過大調整税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_3A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_3A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_3B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_3B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_3C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_3C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_3X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_3X, DiagData.data_val, MONY_BUF_SIZE);

	// 4. 控除対象仕入れ税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_4A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_4A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_4B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_4B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_4C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_4C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_4X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_4X, DiagData.data_val, MONY_BUF_SIZE);

	// 5. 返還等対価に係る税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5X, DiagData.data_val, MONY_BUF_SIZE);

	// 5_1. 売上げの返還等対価に係る税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5_1A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5_1B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5_1C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5_1X, DiagData.data_val, MONY_BUF_SIZE);

	// 5_2. 特定課税仕入れの返還等対価に係る税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5_2C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_5_2X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_5_2X, DiagData.data_val, MONY_BUF_SIZE);

	// 6. 貸倒れに係る税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_6A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_6B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_6C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_6X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_6X, DiagData.data_val, MONY_BUF_SIZE);

	// 7. 控除税額小計
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_7A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_7A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_7B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_7B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_7C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_7C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_7X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_7X, DiagData.data_val, MONY_BUF_SIZE);

	// 8. 控除不足還付税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_8A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_8A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_8B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_8B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_8C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_8C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_8X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_8X, DiagData.data_val, MONY_BUF_SIZE);

	// 9. 差引税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_9A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_9A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_9B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_9B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_9C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_9C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_9X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_9X, DiagData.data_val, MONY_BUF_SIZE);

	// 10. 控除不足還付税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_11B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_11B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_11C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_11C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_11X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_11X, DiagData.data_val, MONY_BUF_SIZE);

	// 11. 差引税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_12B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_12B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_12C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_12C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_12X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_12X, DiagData.data_val, MONY_BUF_SIZE);

	// 12. 合計差引地方消費税の課税標準となる消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_13B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_13B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_13C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_13C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_13X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_13X, DiagData.data_val, MONY_BUF_SIZE);

	// 13. 還付額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_14B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_14B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_14C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_14C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_14X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_14X, DiagData.data_val, MONY_BUF_SIZE);

	// 14. 納税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_15B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_15B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_15C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_15C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID102_15X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F102_15X, DiagData.data_val, MONY_BUF_SIZE);
}


//-----------------------------------------------------------------------------
// 付表１-０２　画面出力
//-----------------------------------------------------------------------------
void CShinFhyo1_02::DispFh1_02()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;


	// 画面の初期化
	for (int i = 3; i <= 5; i++ ) {					// 課税期間・氏名
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)i);
	}
	for (int i = ID102_1A; i <= ID102_15X; i++) {	// 金額欄
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)i);
	}
	if ( !((*m_pSnHeadData)->Sn_Sign4&0x80) ) {		// 連動の場合手入力サイン初期化
		F102_1c = 0; // 手入力サイン初期化
		F102_2c = 0; // 手入力サイン初期化
		F102_5c = 0; // 手入力サイン初期化
	}


	// 期間の表示
	char	buf[512] = {0};
	char	yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd);
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata(IDC_ICSDIAG8CTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 課税期間　自

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
	sprintf_s(buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	DiagData.data_disp = (char *)buf;
	diag_setdata(IDC_ICSDIAG8CTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 課税期間　至


	// 氏名の表示
	memset( buf, '\0', sizeof(buf) );
	if ( (*m_pSnHeadData)->IsSoleProprietor() ) {	// 個人
		memmove(buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO));
	}
	else {
		memmove(buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM));
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata(IDC_ICSDIAG8CTRL2, 3, (struct IUnknown*)&DiagData, CIcsdiagctrl);

	if ( ( ( ( CH31HyoView* )m_pParent )->GetShRevType() >= ID_ICSSH_KYOJU_REVTYPR ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の25・26Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 45, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の25・26Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の25・26Ｃ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の24Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 52, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の24Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 53, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の24Ｃ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 54, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の24・25Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 45, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の24・25Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の24・25Ｃ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の23Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 52, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の23Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 53, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の23Ｃ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 54, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}

	//---->20230120
	long smd = 0, emd = 0;
	( ( CH31HyoView* )m_pParent )->GetKazeiKikan( smd, emd );
	if( emd >= ICS_SH_INVOICE_DAY ){
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の27・28Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 45, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の27・28Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の27・28Ｃ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の26Ａ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 52, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の26Ｂ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 53, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-2の26Ｃ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 54, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	//<----20230120

	// 各入力のデータを全て表示
	f102data_all( 0 );
}

//-----------------------------------------------------------------------------
// 課税標準額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_1data() 
{
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {		// 連動の場合集計しない。自動集計値を使用するため
	
	}
	else {

		/*---->yoshida190613
		//常に入力不可
		if (!(F102_1c & 0x08)) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
				if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1A, 1);
				}
				else {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1A, 0);
				}
			}
		}
		
		if ( !(F102_1c&0x80) ) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
				if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1B, 1);
				}
				else {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1B, 0);
				}
			}
		}

		if ( !(F102_1c&0x800) ) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
				if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1C, 1);
				}
				else {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1C, 0);
				}
			}
		}
		<---------*/
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1A, F102_1A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1B, F102_1B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1C, F102_1C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1X, F102_1X);


	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		if ( F102_1c&0x08 ) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1A, 3);
		}
		if ( F102_1c&0x80 ) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1B, 3);
		}
		if ( F102_1c&0x800 ) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID102_1C, 3);
		}
	}

	F102_1c &= ~0x01;
	F102_1c &= ~0x10;
	F102_1c &= ~0x100;
}

//-----------------------------------------------------------------------------
// 課税資産の譲渡等の対価の額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_1_1data() 
{
	// 色変更
	if ( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01) == 0x00) ) {
		
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1_1A, F102_1_1A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1_1B, F102_1_1B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1_1C, F102_1_1C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1_1X, F102_1_1X);
}

//-----------------------------------------------------------------------------
// 特定課税仕入れに係る支払い対価の額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_1_2data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1_2C, F102_1_2C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_1_2X, F102_1_2X);
}

//-----------------------------------------------------------------------------
// 消費税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_2data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_2A, F102_2A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_2B, F102_2B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_2C, F102_2C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_2X, F102_2X);
	if ( ((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
		ChangeColor(IDC_ICSDIAG8CTRL2, ID102_2A, (F102_2c & 0x08) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID102_2B, (F102_2c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID102_2C, (F102_2c & 0x800) ? 3 : 0);
	}

	F102_2c &= ~0x01;
	F102_2c &= ~0x10;
	F102_2c &= ~0x100;
}

//-----------------------------------------------------------------------------
// 控除過大調整税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_3data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_3A, F102_3A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_3B, F102_3B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_3C, F102_3C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_3X, F102_3X);
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_4data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_4A, F102_4A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_4B, F102_4B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_4C, F102_4C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_4X, F102_4X);
}

//-----------------------------------------------------------------------------
// 返還等対価に係る税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_5data() 
{
	if ( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){	// 連動の場合は集計しない。自動集計値を使用する為
		
	}
	else {

		/*
		if ( !(F102_5c&0x08) ) {
			if ( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01) == 0x00) ) {
				if ( (*m_pSnHeadData)->IsNeedTkkzDisp() ) {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5A, 1);
				}
				else {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5A, 0);
				}
			}
		}

		if ( !(F102_5c&0x80) ) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
				if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5B, 1);
				}
				else {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5B, 0);
				}
			}
		}

		if ( !(F102_5c&0x800) ) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
				if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5C, 1);
				}
				else {
					ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5C, 0);
				}
			}
		}
		*/

	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5A, F102_5A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5B, F102_5B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5C, F102_5C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5X, F102_5X);

	/*
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		if ((F102_5c & 0x08)) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5A, 3);
		}

		if ((F102_5c & 0x80)) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5B, 3);
		}

		if ((F102_5c & 0x800)) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID102_5C, 3);
		}
	}*/

	F102_5c &= ~0x01;
	F102_5c &= ~0x10;
	F102_5c &= ~0x100;
}

//-----------------------------------------------------------------------------
// 売上の返還等対価に係る税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_5_1data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5_1A, F102_5_1A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5_1B, F102_5_1B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5_1C, F102_5_1C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5_1X, F102_5_1X);
}

//-----------------------------------------------------------------------------
// 特定課税仕入れの返還等対価に係る税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_5_2data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5_2C, F102_5_2C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_5_2X, F102_5_2X);
}

//-----------------------------------------------------------------------------
// 貸倒れに係る税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_6data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_6A, F102_6A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_6B, F102_6B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_6C, F102_6C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_6X, F102_6X);
}

//-----------------------------------------------------------------------------
// 控除税額小計
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_7data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_7A, F102_7A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_7B, F102_7B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_7C, F102_7C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_7X, F102_7X);
}

//-----------------------------------------------------------------------------
// 控除不足還付税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_8data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_8A, F102_8A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_8B, F102_8B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_8C, F102_8C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_8X, F102_8X);
}

//-----------------------------------------------------------------------------
// 差引税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_9data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_9A, F102_9A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_9B, F102_9B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_9C, F102_9C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_9X, F102_9X);
}

//-----------------------------------------------------------------------------
// 控除不足還付税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_11data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_11B, F102_11B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_11C, F102_11C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_11X, F102_11X);
}

//-----------------------------------------------------------------------------
// 差引税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_12data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_12B, F102_12B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_12C, F102_12C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_12X, F102_12X);
}

//-----------------------------------------------------------------------------
// 合計差引地方消費税の課税標準となる消費税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_13data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_13B, F102_13B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_13C, F102_13C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_13X, F102_13X);
}
//-----------------------------------------------------------------------------
// 還付額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_14data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_14B, F102_14B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_14C, F102_14C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_14X, F102_14X);
}
//-----------------------------------------------------------------------------
// 納税額
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102_15data() 
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_15B, F102_15B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_15C, F102_15C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID102_15X, F102_15X);
}



//-----------------------------------------------------------------------------
// 各入力データを全て表示
//-----------------------------------------------------------------------------
// 引数 atno	：　0：すべて
//		sw		：
//-----------------------------------------------------------------------------
void CShinFhyo1_02::f102data_all( int atno )
{
	while ( 1 ) {
		switch ( atno ) {
			case	0 :
			case	1 :
				f102_1data();
				f102_1_1data();
				f102_1_2data();
			case	2 :
				f102_2data();
			case	3 :
				f102_3data();
			case	4 :
				f102_4data();
			case	5 :
				f102_5data();
				f102_5_1data();
				f102_5_2data();
			case	6 :
				f102_6data();
			case	7 :
				f102_7data();
			case	8 :
				f102_8data();
			case	9 :
				f102_9data();
			case	10 :
				f102_11data();
			case	11 :
				f102_12data();
			case	12 :
				f102_13data();
			case	13 :
				f102_14data();
			case	14 :
				f102_15data();
			default :
				break;
		}
		break;
	}
}


BOOL CShinFhyo1_02::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			// 付表1-2更新
			GetAllDiagData();
			WriteData();

			if (m_pParent) {
				m_pParent->PostMessageA(WM_KEYDOWN, VK_END);
			}

			return TRUE;
		}
		if (pMsg->wParam == VK_F2) {
			DIAGRAM_DATA   dd;
			if (m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				diag_getdata(m_curwnd, result, &dd, CIcsdiagctrl);
				if (m_curwnd == IDC_ICSDIAG8CTRL2) {
					TerminationIcsdiag8ctrl2(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

// 集計モジュール
int CShinFhyo1_02::SyukeiDataSet(int cID)
{

	DIAGRAM_DATA	DiagData;
	char			val[MONY_BUF_SIZE];

	diag_getdata(IDC_ICSDIAG8CTRL2, cID, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(val, DiagData.data_val, MONY_BUF_SIZE);

	switch (cID) {
	case ID102_1A:
		memmove(F102_1A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310101"), F102_1A);
		break;
	case ID102_1B:
		memmove(F102_1B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310102"), F102_1B);
		break;
	case ID102_1C:
		memmove(F102_1C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310103"), F102_1C);
		break;
	case ID102_1_1A:
		memmove(F102_1_1A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310201"), F102_1_1A);
		break;
	case ID102_1_1B:
		memmove(F102_1_1B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310202"), F102_1_1B);
		break;
	case ID102_1_1C:
		memmove(F102_1_1C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310203"), F102_1_1C);
		break;
	case ID102_1_2C:
		memmove(F102_1_2C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310303"), F102_1_2C);
		break;

	case ID102_2A:
		if ( m_pArith->l_cmp(F102_2A, val) ) {
			F102_2c |= 0x08;
			memmove(F102_2A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("310401"), F102_2A);
		}
		break;
	case ID102_2B:
		if ( m_pArith->l_cmp(F102_2B, val) ) {
			F102_2c |= 0x80;
			memmove(F102_2B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("310402"), F102_2B);
		}
		break;
	case ID102_2C:
		if ( m_pArith->l_cmp(F102_2C, val) ) {
			F102_2c |= 0x800;
			memmove(F102_2C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("310403"), F102_2C);
		}
		break;

	case ID102_5A:
		memmove(F102_5A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310701"), F102_5A);
		break;
	case ID102_5B:
		memmove(F102_5B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310702"), F102_5B);
		break;
	case ID102_5C:
		memmove(F102_5C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310703"), F102_5C);
		break;
	case ID102_5_1A:
		memmove(F102_5_1A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310801"), F102_5_1A);
		break;
	case ID102_5_1B:
		memmove(F102_5_1B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310802"), F102_5_1B);
		break;
	case ID102_5_1C:
		memmove(F102_5_1C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310803"), F102_5_1C);
		break;
	case ID102_5_2C:
		memmove(F102_5_2C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310903"), F102_5_2C);
		break;
	case ID102_6A:
		memmove(F102_6A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311001"), F102_6A);
		break;
	case ID102_6B:
		memmove(F102_6B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311002"), F102_6B);
		break;
	case ID102_6C:
		memmove(F102_6C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311003"), F102_6C);
		break;
	}

	return MemoryReget();
}

int CShinFhyo1_02::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目を全て再取得
	MoneyBasejagArray	money;

	money = (*m_pSyzSyukei)->GetShinkokuData(_T("31"));
	int cnt = (int)money.GetCount();
	int	incnt = 0;
	for (int l = 0; l < cnt; l++) {
		incnt = (int)money[l].GetCount();
		for (int m = 0; m < incnt; m++) {
			// 課税標準額
			if (money[l][m].code == _T("310101"))
				memmove(F102_1A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310102"))
				memmove(F102_1B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310103"))
				memmove(F102_1C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310100"))
				memmove(F102_1X, money[l][m].arith, MONY_BUF_SIZE);

			// 課税資産の譲渡等の対価の額
			if (money[l][m].code == _T("310201"))
				memmove(F102_1_1A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310202"))
				memmove(F102_1_1B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310203"))
				memmove(F102_1_1C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310200"))
				memmove(F102_1_1X, money[l][m].arith, MONY_BUF_SIZE);

			// 特定仕入に係る支払対価の額
			if (money[l][m].code == _T("310303"))
				memmove(F102_1_2C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310300"))
				memmove(F102_1_2X, money[l][m].arith, MONY_BUF_SIZE);

			// 消費税額
			if (money[l][m].code == _T("310401"))
				memmove(F102_2A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310402"))
				memmove(F102_2B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310403"))
				memmove(F102_2C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310400"))
				memmove(F102_2X, money[l][m].arith, MONY_BUF_SIZE);

			// 控除過大調整税額
			if (money[l][m].code == _T("310501"))
				memmove(F102_3A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310502"))
				memmove(F102_3B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310503"))
				memmove(F102_3C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310500"))
				memmove(F102_3X, money[l][m].arith, MONY_BUF_SIZE);

			// 控除対象仕入税額
			if (money[l][m].code == _T("310601"))
				memmove(F102_4A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310602"))
				memmove(F102_4B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310603"))
				memmove(F102_4C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310600"))
				memmove(F102_4X, money[l][m].arith, MONY_BUF_SIZE);

			// 返還等対価に係る税額
			if (money[l][m].code == _T("310701"))
				memmove(F102_5A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310702"))
				memmove(F102_5B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310703"))
				memmove(F102_5C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310700"))
				memmove(F102_5X, money[l][m].arith, MONY_BUF_SIZE);
			
			// 売上の返還等に係る税額
			if (money[l][m].code == _T("310801"))
				memmove(F102_5_1A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310802"))
				memmove(F102_5_1B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310803"))
				memmove(F102_5_1C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310800"))
				memmove(F102_5_1X, money[l][m].arith, MONY_BUF_SIZE);

			// 特定課税仕入の返還等対価に係る税額
			if (money[l][m].code == _T("310903"))
				memmove(F102_5_2C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("310900"))
				memmove(F102_5_2X, money[l][m].arith, MONY_BUF_SIZE);

			// 貸倒に係る税額
			if (money[l][m].code == _T("311001"))
				memmove(F102_6A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311002"))
				memmove(F102_6B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311003"))
				memmove(F102_6C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311000"))
				memmove(F102_6X, money[l][m].arith, MONY_BUF_SIZE);

			// 控除税額小計
			if (money[l][m].code == _T("311101"))
				memmove(F102_7A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311102"))
				memmove(F102_7B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311103"))
				memmove(F102_7C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311100"))
				memmove(F102_7X, money[l][m].arith, MONY_BUF_SIZE);

			// 控除不足還付税額
			if (money[l][m].code == _T("311201"))
				memmove(F102_8A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311202"))
				memmove(F102_8B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311203"))//yoshida
			//if (money[l][m].code == _T("311201"))
				memmove(F102_8C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311200"))
				memmove(F102_8X, money[l][m].arith, MONY_BUF_SIZE);

			// 差引税額
			if (money[l][m].code == _T("311301"))
				memmove(F102_9A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311302"))
				memmove(F102_9B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311303"))
				memmove(F102_9C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311300"))
				memmove(F102_9X, money[l][m].arith, MONY_BUF_SIZE);

			// 控除不足還付税額
			if (money[l][m].code == _T("311502"))
				memmove(F102_11B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311503"))
				memmove(F102_11C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311500"))
				memmove(F102_11X, money[l][m].arith, MONY_BUF_SIZE);

			// 差引税額
			if (money[l][m].code == _T("311602"))
				memmove(F102_12B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311603"))
				memmove(F102_12C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311600"))
				memmove(F102_12X, money[l][m].arith, MONY_BUF_SIZE);

			// 合計差引地方消費税の課税標準となる消費税額
			if (money[l][m].code == _T("311702"))
				memmove(F102_13B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311703"))
				memmove(F102_13C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311700"))
				memmove(F102_13X, money[l][m].arith, MONY_BUF_SIZE);

			// 還付額
			if (money[l][m].code == _T("311802"))
				memmove(F102_14B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311803"))
				memmove(F102_14C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311800"))
				memmove(F102_14X, money[l][m].arith, MONY_BUF_SIZE);

			// 納税額
			if (money[l][m].code == _T("311902"))
				memmove(F102_15B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311903"))
				memmove(F102_15C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("311900"))
				memmove(F102_15X, money[l][m].arith, MONY_BUF_SIZE);

		}
	}

	return 0;
}

//====================================
// 手入力項目あり？
//------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//====================================
BOOL CShinFhyo1_02::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F102_2c & 0x888 ) {
			ret = TRUE;
		}
	}

	return ret;
}