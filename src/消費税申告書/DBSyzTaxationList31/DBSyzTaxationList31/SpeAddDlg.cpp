// SpeAddDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SpeAddDlg.h"
#include "TaxationList31View.h"

#define		WM_USER_MESSAGE_1			( WM_APP + 100 )

// CSpeAddDlg ダイアログ

IMPLEMENT_DYNAMIC(CSpeAddDlg, ICSDialog)

CSpeAddDlg::CSpeAddDlg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_SPEADD, pParent)
	, m_pSnHeadData(NULL)
	, m_pSpcListData(NULL)
	, m_pZmSub(NULL)
	, m_pArith(NULL)
{
	m_ShinSpecific52_1_31	= NULL;
// 24/02/15_特定収入 add -->
	m_ShinSpecific52_2_31	= NULL;
	m_ShinSpecific52_31_31	= NULL;
	m_ShinSpecific52_32_31	= NULL;
	m_ShinSpecific52_33_31	= NULL;
	m_ShinSpecific52_41_31	= NULL;
	m_ShinSpecific52_42_31	= NULL;
	m_ShinSpecific52_43_31	= NULL;
// 24/02/15_特定収入 add <--

	chktype = 0;
// 24/02/14_特定収入 add -->
	memset(&m_TsTbl52,0,sizeof(TB_PAR));
	m_TsTbl52.tp_tno = -1;
	m_Type = 0;
	m_CurPage = 1;
// 24/02/14_特定収入 add <--
}

CSpeAddDlg::~CSpeAddDlg()
{
// 24/02/14_特定収入 add -->
	if(m_TsTbl52.tp_tno != -1)	{
		m_TblHdl.th_close(m_TsTbl52.tp_tno);
		m_TsTbl52.tp_tno = -1;
	}
	if(m_ShinSpecific52_1_31 != NULL)	{
		delete	m_ShinSpecific52_1_31;
		m_ShinSpecific52_1_31	= NULL;
	}
	if(m_ShinSpecific52_2_31 != NULL)	{
		delete	m_ShinSpecific52_2_31;
		m_ShinSpecific52_2_31 = NULL;
	}
	if(m_ShinSpecific52_31_31 != NULL)	{
		delete	m_ShinSpecific52_31_31;
		m_ShinSpecific52_31_31 = NULL;
	}
	if(m_ShinSpecific52_32_31 != NULL)	{
		delete	m_ShinSpecific52_32_31;
		m_ShinSpecific52_32_31 = NULL;
	}
	if(m_ShinSpecific52_33_31 != NULL)	{
		delete	m_ShinSpecific52_33_31;
		m_ShinSpecific52_33_31 = NULL;
	}
	if(m_ShinSpecific52_41_31 != NULL)	{
		delete	m_ShinSpecific52_41_31;
		m_ShinSpecific52_41_31 = NULL;
	}
	if(m_ShinSpecific52_42_31 != NULL)	{
		delete	m_ShinSpecific52_42_31;
		m_ShinSpecific52_42_31 = NULL;
	}
	if(m_ShinSpecific52_43_31 != NULL)	{
		delete	m_ShinSpecific52_43_31;
		m_ShinSpecific52_43_31 = NULL;
	}
// 24/02/14_特定収入 add <--
}

void CSpeAddDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABSPE, m_SpeTab);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK4, m_Check4);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	// 24/02/14_特定収入 add -->
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_PageDiag);
	// 24/02/14_特定収入 add <--
	DDX_Control(pDX, IDC_STATIC_INPUTLOCK, m_InputLock);
}

void CSpeAddDlg::InitInfo(CSnHeadData *pSnHeadData, CH31SpcListData *pSpcListData, CArithEx *pArith, CDBNpSub *pZmSub)
{
	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return;
	}
	m_pSnHeadData = pSnHeadData;
// 240308_h_160358 add -->
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	{
		m_Eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else	{
		m_Eymd = m_pSnHeadData->Sn_KDAYE;
	}
// 240308_h_160358 add <--

	ASSERT(pSpcListData);
	if (pSpcListData == NULL) {
		return ;
	}
	m_pSpcListData = pSpcListData;

	ASSERT(pArith);
	if (pArith == NULL) {
		return ;
	}
	m_pArith = pArith;

	ASSERT(pZmSub);
	if (pZmSub == NULL) {
		return;
	}
	m_pZmSub = pZmSub;

// 24/02/14_特定収入 add -->
	// ダイアログ内で操作する計算表５－２データを確保する
	//int	rsiz = (unsigned int)sizeof(TSREC52);					// レコード長
	//int	rcnt = 210;												// 最大登録数
	int	tsiz = (unsigned long)m_pSpcListData->TsTbl52.tp_rlen * (unsigned long)m_pSpcListData->TsTbl52.tp_rmax;
	int	ii=0;
	memset(&m_TsTbl52,0,sizeof(TB_PAR));
	m_TsTbl52.tp_tno = m_TblHdl.th_open(tsiz,m_pSpcListData->TsTbl52.tp_rlen);
	m_TsTbl52.tp_rmax = m_pSpcListData->TsTbl52.tp_rmax;
	m_TsTbl52.tp_rnum = 0;
	m_TsTbl52.tp_rlen = m_pSpcListData->TsTbl52.tp_rlen;
	m_TsTbl52.tp_koff = 0;
	m_TsTbl52.tp_klen = 0;			// キーなし
	for(ii=0; ii<m_pSpcListData->TsTbl52.tp_rnum; ii++)	{
		if(m_pSpcListData->m_TblHdl.th_rget(&m_TSrec52,&m_pSpcListData->TsTbl52,ii))	{
			break;
		}
		if(m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,ii))	{
			break;
		}
	}
// 24/02/14_特定収入 add <--
}

void CSpeAddDlg::InitTab()
{
	m_ShinSpecific52_1_31 = new CShinSpecific52_1_31(this,&m_TSrec52);
	m_ShinSpecific52_1_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_1_31->Create(IDD_DIALOG_K52_1_31, &m_SpeTab);

// 24/02/15_特定収入 add -->
	m_ShinSpecific52_2_31 = new CShinSpecific52_2_31(this, &m_TSrec52);
	m_ShinSpecific52_2_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_2_31->Create(IDD_DIALOG_K52_2_31, &m_SpeTab);

	m_ShinSpecific52_31_31 = new CShinSpecific52_31_31(this, &m_TSrec52);
	m_ShinSpecific52_31_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_31_31->Create(IDD_DIALOG_K52_31_31, &m_SpeTab);

	m_ShinSpecific52_32_31 = new CShinSpecific52_32_31(this, &m_TSrec52);
	m_ShinSpecific52_32_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_32_31->Create(IDD_DIALOG_K52_32_31, &m_SpeTab);

// 240308_h_160358 cor -->
	//m_ShinSpecific52_33_31 = new CShinSpecific52_33_31(this, &m_TSrec52);
	//m_ShinSpecific52_33_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	//m_ShinSpecific52_33_31->Create(IDD_DIALOG_K52_33_31, &m_SpeTab);
// -----------------------
	if(m_Eymd > KIKAN_END_INV80)	{
		m_ShinSpecific52_33_31 = new CShinSpecific52_33_31(this, &m_TSrec52);
		m_ShinSpecific52_33_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		m_ShinSpecific52_33_31->Create(IDD_DIALOG_K52_33_31, &m_SpeTab);
	}
// 240308_h_160358 cor <--

	m_ShinSpecific52_41_31 = new CShinSpecific52_41_31(this, &m_TSrec52);
	m_ShinSpecific52_41_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_41_31->Create(IDD_DIALOG_K52_41_31, &m_SpeTab);

	m_ShinSpecific52_42_31 = new CShinSpecific52_42_31(this, &m_TSrec52);
	m_ShinSpecific52_42_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_42_31->Create(IDD_DIALOG_K52_42_31, &m_SpeTab);

// 240308_h_160358 cor -->
	//m_ShinSpecific52_43_31 = new CShinSpecific52_43_31(this, &m_TSrec52);
	//m_ShinSpecific52_43_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	//m_ShinSpecific52_43_31->Create(IDD_DIALOG_K52_43_31, &m_SpeTab);
// -----------------------
	if(m_Eymd > KIKAN_END_INV80)	{
		m_ShinSpecific52_43_31 = new CShinSpecific52_43_31(this, &m_TSrec52);
		m_ShinSpecific52_43_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		m_ShinSpecific52_43_31->Create(IDD_DIALOG_K52_43_31, &m_SpeTab);
	}
// 240308_h_160358 cor <--

// 24/02/15_特定収入 add <--

	m_SpeTab.DeleteAllItems();
	m_SpeTab.InsertItem(0, "計算表５－２(１)", m_ShinSpecific52_1_31->m_hWnd);

// 24/02/14_特定収入 add -->
	// ------------------------------------------------------------------------
	// 画面上部の初期表示
	// ------------------------------------------------------------------------
	// 頁の分子			[1]
	CmnDiagSetValue(&m_PageDiag,1,1,0);
	// ボタンコントロール等
	PageButtonCtrl();
// 24/02/14_特定収入 add <--

	PostMessage(WM_MYMESSAGESPEADDTAB, 0, 0);

	return;
}
void CSpeAddDlg::AddTab(int type)
{
// 24/02/15_特定収入 add -->
	m_Type = type;
// 24/02/15_特定収入 add <--
	/*m_SpeTab.DeleteAllItems();

	m_ShinSpecific52_1_31 = new CShinSpecific52_1_31(this);
	m_ShinSpecific52_1_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	m_ShinSpecific52_1_31->Create(IDD_DIALOG_K52_1_31, &m_SpeTab);
	m_SpeTab.InsertItem(0, "計算表５－２(１)", m_ShinSpecific52_1_31->m_hWnd);*/
	int count = m_SpeTab.GetItemCount();
	if (count == 2) {
		m_SpeTab.DeleteItem(1);
	}
// 240308_h_160358 add -->
	else if (count == 3) {
		m_SpeTab.DeleteItem(2);
		m_SpeTab.DeleteItem(1);
	}
// 240308_h_160358 add <--
	else if (count == 4) {
		m_SpeTab.DeleteItem(3);
		m_SpeTab.DeleteItem(2);
		m_SpeTab.DeleteItem(1);
	}

	if (type == 0) {
// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_2_31 = new CShinSpecific52_2_31(this);
		//m_ShinSpecific52_2_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_2_31->Create(IDD_DIALOG_K52_2_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
		m_SpeTab.InsertItem(1, "計算表５－２(２)", m_ShinSpecific52_2_31->m_hWnd);
	}
	else if (type == 1) {
// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_31_31 = new CShinSpecific52_31_31(this);
		//m_ShinSpecific52_31_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_31_31->Create(IDD_DIALOG_K52_31_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
		m_SpeTab.InsertItem(1, "計算表５－２(３)－１", m_ShinSpecific52_31_31->m_hWnd);

// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_32_31 = new CShinSpecific52_32_31(this);
		//m_ShinSpecific52_32_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_32_31->Create(IDD_DIALOG_K52_32_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
		m_SpeTab.InsertItem(2, "計算表５－２(３)－２", m_ShinSpecific52_32_31->m_hWnd);

// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_33_31 = new CShinSpecific52_33_31(this);
		//m_ShinSpecific52_33_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_33_31->Create(IDD_DIALOG_K52_33_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
// 240308_h_160358 cor -->
		//m_SpeTab.InsertItem(3, "計算表５－２(３)－３", m_ShinSpecific52_33_31->m_hWnd);
// -----------------------
		if(m_Eymd > KIKAN_END_INV80)	{
			m_SpeTab.InsertItem(3, "計算表５－２(３)－３", m_ShinSpecific52_33_31->m_hWnd);
		}
// 240308_h_160358 cor <--
	}
	else if (type == 2) {
// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_41_31 = new CShinSpecific52_41_31(this);
		//m_ShinSpecific52_41_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_41_31->Create(IDD_DIALOG_K52_41_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
		m_SpeTab.InsertItem(1, "計算表５－２(４)－１", m_ShinSpecific52_41_31->m_hWnd);

// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_42_31 = new CShinSpecific52_42_31(this);
		//m_ShinSpecific52_42_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_42_31->Create(IDD_DIALOG_K52_42_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
		m_SpeTab.InsertItem(2, "計算表５－２(４)－２", m_ShinSpecific52_42_31->m_hWnd);

// 24/02/15_特定収入 del -->
		//m_ShinSpecific52_43_31 = new CShinSpecific52_43_31(this);
		//m_ShinSpecific52_43_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
		//m_ShinSpecific52_43_31->Create(IDD_DIALOG_K52_43_31, &m_SpeTab);
// 24/02/15_特定収入 del <--
// 240308_h_160358 cor -->
		//m_SpeTab.InsertItem(3, "計算表５－２(４)－３", m_ShinSpecific52_43_31->m_hWnd);
// -----------------------
		if(m_Eymd > KIKAN_END_INV80)	{
			m_SpeTab.InsertItem(3, "計算表５－２(４)－３", m_ShinSpecific52_43_31->m_hWnd);
		}
// 240308_h_160358 cor <--
	}
	int selno = m_SpeTab.GetCurSel();
	ReInitSpcDisp(selno, chktype);
}

void CSpeAddDlg::DelTab(int type)
{
	m_SpeTab.DeleteAllItems();

// 24/02/15_特定収入 del -->
	//m_ShinSpecific52_1_31 = new CShinSpecific52_1_31(this);
	//m_ShinSpecific52_1_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
	//m_ShinSpecific52_1_31->Create(IDD_DIALOG_K52_1_31, &m_SpeTab);
// 24/02/15_特定収入 del <--

	m_SpeTab.InsertItem(0, "計算表５－２(１)", m_ShinSpecific52_1_31->m_hWnd);

	ReInitSpcDisp(0,0);
}

BEGIN_MESSAGE_MAP(CSpeAddDlg, ICSDialog)
// 24/02/15_特定収入 del -->
	//ON_BN_CLICKED(IDC_CHECK2, &CSpeAddDlg::OnBnClickedCheck2)
	//ON_BN_CLICKED(IDC_CHECK3, &CSpeAddDlg::OnBnClickedCheck3)
	//ON_BN_CLICKED(IDC_CHECK4, &CSpeAddDlg::OnBnClickedCheck4)
// 24/02/15_特定収入 del <--
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABSPE, OnTcnSelchangeMainTab)
	ON_MESSAGE(WM_MYMESSAGESPEADDTAB, OnMyMessage)
	//ON_MESSAGE(WM_MYMESSAGESELCHANGE, OnMyMessageSelChange)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK,&CSpeAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5,&CSpeAddDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3,&CSpeAddDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4,&CSpeAddDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6,&CSpeAddDlg::OnBnClickedButton6)
// 240305_160343 add -->
	ON_BN_CLICKED(IDC_BUTTON7,&CSpeAddDlg::OnBnClickedButton7)
// 240305_160343 add <--
// 240305_160345 add -->
	ON_BN_CLICKED(IDC_BUTTON8,&CSpeAddDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9,&CSpeAddDlg::OnBnClickedButton9)
// 240305_160345 add <--
	ON_BN_CLICKED(IDC_BUTTON10,&CSpeAddDlg::OnBnClickedButton10)		// 修正No.168722 add
	ON_BN_CLICKED(IDCANCEL, &CSpeAddDlg::OnBnClickedCancel)
	ON_WM_ACTIVATE()	// 修正No.168706 add
END_MESSAGE_MAP()


// CSpeAddDlg メッセージ ハンドラー


BOOL CSpeAddDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Disp_WriteLock();

	InitTab();

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 24/02/15_特定収入 del -->
//void CSpeAddDlg::OnBnClickedCheck2()
//{
//	// TODO: ここにコントロール通知ハンドラー コードを追加します。
//	if (m_Check3.GetCheck()) m_Check3.SetCheck(0);
//	if (m_Check4.GetCheck()) m_Check4.SetCheck(0);
//
//	m_SpeTab.DeleteAllItems();
//	
//
//	m_ShinSpecific52_1_31 = new CShinSpecific52_1_31(this);
//	m_ShinSpecific52_1_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//	m_ShinSpecific52_1_31->Create(IDD_DIALOG_K52_1_31, &m_SpeTab);
//	m_SpeTab.InsertItem(0, "計算表５－２(１)", m_ShinSpecific52_1_31->m_hWnd);
//
//	if (m_Check2.GetCheck()) {
//		m_ShinSpecific52_2_31 = new CShinSpecific52_2_31(this);
//		m_ShinSpecific52_2_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_2_31->Create(IDD_DIALOG_K52_2_31, &m_SpeTab);
//		m_SpeTab.InsertItem(1, "計算表５－２(２)", m_ShinSpecific52_2_31->m_hWnd);
//	}
//
//	int selno = m_SpeTab.GetCurSel();
//	//ReInitSpcDisp(selno);
//}
//
//void CSpeAddDlg::OnBnClickedCheck3()
//{
//	// TODO: ここにコントロール通知ハンドラー コードを追加します。
//	if (m_Check2.GetCheck()) m_Check2.SetCheck(0);
//	if (m_Check4.GetCheck()) m_Check4.SetCheck(0);
//	m_SpeTab.DeleteAllItems();
//	
//	m_ShinSpecific52_1_31 = new CShinSpecific52_1_31(this);
//	m_ShinSpecific52_1_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//	m_ShinSpecific52_1_31->Create(IDD_DIALOG_K52_1_31, &m_SpeTab);
//	m_SpeTab.InsertItem(0, "計算表５－２(１)", m_ShinSpecific52_1_31->m_hWnd);
//
//	if (m_Check3.GetCheck()) {
//		m_ShinSpecific52_31_31 = new CShinSpecific52_31_31(this);
//		m_ShinSpecific52_31_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_31_31->Create(IDD_DIALOG_K52_31_31, &m_SpeTab);
//		m_SpeTab.InsertItem(1, "計算表５－２(３)－１", m_ShinSpecific52_31_31->m_hWnd);
//
//		m_ShinSpecific52_32_31 = new CShinSpecific52_32_31(this);
//		m_ShinSpecific52_32_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_32_31->Create(IDD_DIALOG_K52_32_31, &m_SpeTab);
//		m_SpeTab.InsertItem(2, "計算表５－２(３)－２", m_ShinSpecific52_32_31->m_hWnd);
//
//		m_ShinSpecific52_33_31 = new CShinSpecific52_33_31(this);
//		m_ShinSpecific52_33_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_33_31->Create(IDD_DIALOG_K52_33_31, &m_SpeTab);
//		m_SpeTab.InsertItem(3, "計算表５－２(３)－３", m_ShinSpecific52_33_31->m_hWnd);
//	}
//	
//	int selno = m_SpeTab.GetCurSel();
//	//ReInitSpcDisp(selno);
//	
//}
//
//void CSpeAddDlg::OnBnClickedCheck4()
//{
//	// TODO: ここにコントロール通知ハンドラー コードを追加します。
//
//	if (m_Check2.GetCheck()) m_Check2.SetCheck(0);
//	if (m_Check3.GetCheck()) m_Check3.SetCheck(0);
//	m_SpeTab.DeleteAllItems();
//
//
//	m_ShinSpecific52_1_31 = new CShinSpecific52_1_31(this);
//	m_ShinSpecific52_1_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//	m_ShinSpecific52_1_31->Create(IDD_DIALOG_K52_1_31, &m_SpeTab);
//	m_SpeTab.InsertItem(0, "計算表５－２(１)", m_ShinSpecific52_1_31->m_hWnd);
//
//	if (m_Check4.GetCheck()) {
//		m_ShinSpecific52_41_31 = new CShinSpecific52_41_31(this);
//		m_ShinSpecific52_41_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_41_31->Create(IDD_DIALOG_K52_41_31, &m_SpeTab);
//		m_SpeTab.InsertItem(1, "計算表５－２(４)－１", m_ShinSpecific52_41_31->m_hWnd);
//
//		m_ShinSpecific52_42_31 = new CShinSpecific52_42_31(this);
//		m_ShinSpecific52_42_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_42_31->Create(IDD_DIALOG_K52_42_31, &m_SpeTab);
//		m_SpeTab.InsertItem(2, "計算表５－２(４)－２", m_ShinSpecific52_42_31->m_hWnd);
//
//		m_ShinSpecific52_43_31 = new CShinSpecific52_43_31(this);
//		m_ShinSpecific52_43_31->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmSub, m_pArith);
//		m_ShinSpecific52_43_31->Create(IDD_DIALOG_K52_43_31, &m_SpeTab);
//		m_SpeTab.InsertItem(3, "計算表５－２(４)－３", m_ShinSpecific52_43_31->m_hWnd);
//	}
//	int selno = m_SpeTab.GetCurSel();
//	//ReInitSpcDisp(selno);
//}
// 24/02/15_特定収入 del <--

void CSpeAddDlg::OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{
	int selno = m_SpeTab.GetCurSel();

	ReInitSpcDisp(selno, chktype);

// 240308_h_168689 add -->
	//if(
	if(!(m_pSnHeadData->Sn_Sign4 & 0x01))	{
		//if(selno == 0 && m_TsTbl52.tp_rnum > 1)	{
		if(selno == 0)	{
			GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
		}
		else	{
			GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
		}
	}
// 240308_h_168689 add <--

	/*if (selno == 1) {
		if(chktype == 1 || chktype == 2)
	}*/
	//SetItemPositionEx(selno);
}

LRESULT CSpeAddDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	//HINSTANCE hInst = AfxGetResourceHandle();
	//AfxSetResourceHandle(g_hInstance);


	ReInitSpcDisp(m_SpeTab.GetCurSel(), chktype);
	

	//AfxSetResourceHandle(hInst);

	return 0;
}

void CSpeAddDlg::ReInitSpcDisp(int selno, int chktype)
{
	if (selno == 0) {
		m_ShinSpecific52_1_31->SpecificInit52_1_31();
	}
	else if (selno == 1) {
		if (chktype == 0) {
			m_ShinSpecific52_2_31->SpecificInit52_2_31();
		}
		else if (chktype == 1) {
			m_ShinSpecific52_31_31->SpecificInit52_31_31();
		}
		else {
			m_ShinSpecific52_41_31->SpecificInit52_41_31();
		}
	}
	else if (selno == 2) {
		if (chktype == 1) {
			m_ShinSpecific52_32_31->SpecificInit52_32_31();
		}
		else {
			m_ShinSpecific52_42_31->SpecificInit52_42_31();
		}
	}
	else {
		if (chktype == 1) {
			m_ShinSpecific52_33_31->SpecificInit52_33_31();
		}
		else {
			m_ShinSpecific52_43_31->SpecificInit52_43_31();
		}
	}
	
}

BOOL CSpeAddDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
// 240305_160345 add -->
	if(pMsg->message == WM_KEYDOWN)	{
		if(pMsg->wParam == VK_F7)	{
			if(m_SpeTab.GetItemCount() != 1)	{
				PostMessage(WM_COMMAND,MAKELONG(IDC_BUTTON8,BN_CLICKED),
									(LPARAM)GetDlgItem(IDC_BUTTON8)->m_hWnd);
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_F8)	{
			if(m_SpeTab.GetItemCount() != 1)	{
				PostMessage(WM_COMMAND,MAKELONG(IDC_BUTTON9,BN_CLICKED),
									(LPARAM)GetDlgItem(IDC_BUTTON9)->m_hWnd);
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			OnBnClickedCancel();
			return TRUE;
		}
		else if (pMsg->wParam == VK_END) {
			OnBnClickedOk();
			return TRUE;
		}
	}
// 240305_160345 add <--
	return ICSDialog::PreTranslateMessage(pMsg);
}


HBRUSH CSpeAddDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

// 24/02/14_特定収入 add -->
void CSpeAddDlg::OnBnClickedOk()
{
	// 現在頁のデータを読み込む
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,m_CurPage-1);
	// ダイアログで編集したデータを特定収入のメンバに返す
	m_pSpcListData->TsTbl52.tp_rnum = 0;
	for(int	ii=0; ii<m_TsTbl52.tp_rnum; ii++)	{
		if(m_TblHdl.th_rget(&m_TSrec52,&m_TsTbl52,ii))	{
			break;
		}
		if(m_pSpcListData->m_TblHdl.th_rput(&m_TSrec52,&m_pSpcListData->TsTbl52,ii))	{
			break;
		}
	}
	EndDialog(0);
}

//********************************************************************************
//	ICS-Controlへ値を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				セットする値
//				int				対象コントロールの種別（0:入力，1:表示）
//		RET		なし
//********************************************************************************
void CSpeAddDlg::CmnDiagSetValue( CIcsdiagctrl* pDiag, int intIndex, int intData, int intFlg )
{
	CString			strTemp;

	// 値を文字列に変換して設定
	strTemp.Format("%d", intData);
	CmnDiagSetString(pDiag, intIndex, strTemp, intFlg);
}

//********************************************************************************
//	ICS-Controlへ文字列を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				CString			セットする文字列
//				int				対象コントロールの種別（0:入力，1:表示）
//		RET		なし
//********************************************************************************
void CSpeAddDlg::CmnDiagSetString( CIcsdiagctrl* pDiag, int intIndex, CString strData, int intFlg )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	if ( strData != "" ) {
		// 初期化
		DiagInit( &diadata );

		if ( intFlg == 1 ) {
			diadata.data_disp = strData;		// 1:表示
		}
		else {
			diadata.data_edit = strData;		// 0:入力
		}

		// 文字列セット
		pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );
	}
	else {
		// データクリア
		pDiag->DataClear( intIndex, TRUE );
	}
}

//********************************************************************************
//	ICS-ControlのDIAGRAM_DATA初期化
//　【引数】	DIAGRAM_DATA*	diadata			データ
//　【戻値】	なし
//********************************************************************************
void CSpeAddDlg::DiagInit( DIAGRAM_DATA* diadata )
{
	// 初期化
	diadata->data_edit.Empty();
	diadata->data_disp.Empty();
	diadata->data_combo = 0;
	diadata->data_check = 0;
	memset( diadata->data_val, '\0', sizeof(diadata->data_val));
	memset( diadata->data_day,'\0', sizeof(diadata->data_day));
	diadata->data_imgdata = (char*)NULL;
	diadata->data_imgwidth = 0;
	diadata->data_imgheight = 0;
	diadata->text_combo.Empty();
	diadata->text_check.Empty();
	diadata->movepos = 0;
}

//********************************************************************************
//	ICS-Controlから文字列を取得
//　【引数】	CICSDiag*		pDiag			対象コントロール
//				int				intIndex		対象コントロールのインデックス番号
//　【引数】	対象コントロールの文字列
//********************************************************************************
CString CSpeAddDlg::DiagGetString( CIcsdiagctrl* pDiag, int intIndex )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	DiagInit( &diadata );
	
	// 文字列取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	
	// 取得文字列返送
	return ( diadata.data_edit );
}

//********************************************************************************
//	ICS-Controlから数値を取得
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//		RET		対象コントロールの数値
//********************************************************************************
int CSpeAddDlg::CmnDiagGetValue( CIcsdiagctrl* pDiag, int intIndex )
{
	CString			strTemp;
	int				intTemp;

	// 文字列を取得して数値に変換
	strTemp = DiagGetString(pDiag, intIndex);
	intTemp = atoi(strTemp);

	// 数値を返す
	return ( intTemp );
}

//********************************************************************************
//	頁ボタンのコントロール
//　【引数】	なし
//　【戻値】	なし
//********************************************************************************
void CSpeAddDlg::PageButtonCtrl( void )
{
	CString	cs=_T("");
	BOOL	bsw=FALSE;

	// 前へボタン
	m_CurPage = CmnDiagGetValue(&m_PageDiag,1);
	if(m_CurPage > 1)	bsw=TRUE;
	else				bsw=FALSE;
	GetDlgItem(IDC_BUTTON3)->EnableWindow(bsw);

	// 次へボタン
	if(m_CurPage < m_TsTbl52.tp_rnum)	bsw=TRUE;
	else								bsw=FALSE;
	GetDlgItem(IDC_BUTTON4)->EnableWindow(bsw);

	// 挿入ボタン
	if(m_TsTbl52.tp_rnum < m_TsTbl52.tp_rmax)	bsw=TRUE;
	else										bsw=FALSE;
	//入力ロック時押下不可
	if(m_pSnHeadData->Sn_Sign4 & 0x01)			bsw = FALSE;
	GetDlgItem(IDC_BUTTON5)->EnableWindow(bsw);

// 240305_160343 add -->
	// 追加ボタン
	GetDlgItem(IDC_BUTTON7)->EnableWindow(bsw);
// 240305_160343 add <--

	// 削除ボタン
	//if(m_TsTbl52.tp_rnum > 1)					bsw=TRUE;
	//else										bsw=FALSE;
	//入力ロック時押下不可
	if (m_pSnHeadData->Sn_Sign4 & 0x01)			bsw = FALSE;
	else										bsw = TRUE;
	GetDlgItem(IDC_BUTTON6)->EnableWindow(bsw);

// 修正No.168722 add -->
	// 初期化ボタン
	//入力ロック時押下不可
	if (m_pSnHeadData->Sn_Sign4 & 0x01)			bsw = FALSE;
	else										bsw = TRUE;
	GetDlgItem(IDC_BUTTON10)->EnableWindow(bsw);
// 修正No.168722 add <--

	// 頁の分母を表示
	CmnDiagSetValue(&m_PageDiag,3,m_TsTbl52.tp_rnum,1);

	// 最大登録可能件数
	cs.Format(_T("最大登録可能件数：%3d"),m_TsTbl52.tp_rmax);
	GetDlgItem(IDC_STACNT)->SetWindowText(cs);

	// 現在頁のデータを読み込む
	m_TblHdl.th_rget(&m_TSrec52,&m_TsTbl52,m_CurPage-1);

	//// カーソル消去
	//m_ShinSpecific52_1_31->m_Specific52_1_31.DeleteInput();		// 5-2(1)
	//m_ShinSpecific52_2_31->m_Specific52_2_31.DeleteInput();		// 5-2(2)
	//m_ShinSpecific52_31_31->m_Specific52_31_31.DeleteInput();	// 5-2(3)-1
	//m_ShinSpecific52_32_31->m_Specific52_32_31.DeleteInput();	// 5-2(3)-2
	//m_ShinSpecific52_33_31->m_Specific52_33_31.DeleteInput();	// 5-2(3)-3
	//m_ShinSpecific52_41_31->m_Specific52_41_31.DeleteInput();	// 5-2(4)-1
	//m_ShinSpecific52_42_31->m_Specific52_42_31.DeleteInput();	// 5-2(4)-2
	//m_ShinSpecific52_43_31->m_Specific52_43_31.DeleteInput();	// 5-2(4)-3

	// データ表示
// 240228_No12.higuchi cor -->
	//m_ShinSpecific52_1_31->SpecificDisp52_1_31();
	//m_ShinSpecific52_2_31->SpecificDisp52_2_31();
	//m_ShinSpecific52_31_31->SpecificDisp52_31_31();
	//m_ShinSpecific52_32_31->SpecificDisp52_32_31();
	//m_ShinSpecific52_33_31->SpecificDisp52_33_31();
	//m_ShinSpecific52_41_31->SpecificDisp52_41_31();
	//m_ShinSpecific52_42_31->SpecificDisp52_42_31();
	//m_ShinSpecific52_43_31->SpecificDisp52_43_31();
// ---------------------------
	m_ShinSpecific52_1_31->IndexControl();
	m_ShinSpecific52_1_31->SpecificDisp52_1_31();
	m_ShinSpecific52_2_31->IndexControl();
	m_ShinSpecific52_2_31->SpecificDisp52_2_31();
	m_ShinSpecific52_31_31->IndexControl();
	m_ShinSpecific52_31_31->SpecificDisp52_31_31();
	m_ShinSpecific52_32_31->IndexControl();
	m_ShinSpecific52_32_31->SpecificDisp52_32_31();
// 240308_h_160358 cor -->
	//m_ShinSpecific52_33_31->IndexControl();
	//m_ShinSpecific52_33_31->SpecificDisp52_33_31();
// -----------------------
	if(m_Eymd > KIKAN_END_INV80)	{
		m_ShinSpecific52_33_31->IndexControl();
		m_ShinSpecific52_33_31->SpecificDisp52_33_31();
	}
// 240308_h_160358 cor <--
	m_ShinSpecific52_41_31->IndexControl();
	m_ShinSpecific52_41_31->SpecificDisp52_41_31();
	m_ShinSpecific52_42_31->IndexControl();
	m_ShinSpecific52_42_31->SpecificDisp52_42_31();
// 240308_h_160358 cor -->
	//m_ShinSpecific52_43_31->IndexControl();
	//m_ShinSpecific52_43_31->SpecificDisp52_43_31();
// -----------------------
	if(m_Eymd > KIKAN_END_INV80)	{
		m_ShinSpecific52_43_31->IndexControl();
		m_ShinSpecific52_43_31->SpecificDisp52_43_31();
	}
// 240308_h_160358 cor <--
// 240228_No12.higuchi cor <--
	// 計算表選択（データ表示がクラスコンストラクタ内で呼ばれているので分けた）
	//PostMessage(WM_MYMESSAGESELCHANGE,0,0);
	m_ShinSpecific52_1_31->CalcSelChange(m_TSrec52.ts521.sel);

	DialogSetFocus();
}

// 前へボタン押下
void CSpeAddDlg::OnBnClickedButton3()
{
	int		page = CmnDiagGetValue(&m_PageDiag,1);

	// 現在頁のデータを書き込む
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,page-1);

	// 頁の分子をマイナス１する
	CmnDiagSetValue(&m_PageDiag,1,page-1,0);

	// ボタンコントロール等
	PageButtonCtrl();
}

// 次へボタン押下
void CSpeAddDlg::OnBnClickedButton4()
{
	int		page = CmnDiagGetValue(&m_PageDiag,1);

	// 現在頁のデータを書き込む
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,page-1);

	// 頁の分子をマイナス１する
	CmnDiagSetValue(&m_PageDiag,1,page+1,0);

	// ボタンコントロール等
	PageButtonCtrl();
}

// 挿入ボタン押下
void CSpeAddDlg::OnBnClickedButton5()
{
	int		ii=0;
	int		page = CmnDiagGetValue(&m_PageDiag,1);

// 240305_160343 add -->
	if(ICSMessageBox(_T("次頁に新規頁を挿入します。"),MB_OKCANCEL) == IDCANCEL)	{
		return;
	}
// 240305_160343 add <--

	// 現在頁のデータを書き込む
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,page-1);

	// 現在頁より後ろを１頁ずつ後ろにずらしていく
	if(page < m_TsTbl52.tp_rnum)	{
		for(ii=m_TsTbl52.tp_rnum; ii>=page; ii--)	{
			if(m_TblHdl.th_rget(&m_TSrec52,&m_TsTbl52,ii-1))	{
				break;
			}
			if(m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,ii))	{
				break;
			}
		}
	}

	// 次頁をクリア
	memset(&m_TSrec52,0,sizeof(TSREC52));
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,page);

	// 頁の分子をプラス１する
	CmnDiagSetValue(&m_PageDiag,1,page+1,0);

	// ボタンコントロール等
	PageButtonCtrl();
}

// 240305_160343 add -->
// 追加ボタン押下
void CSpeAddDlg::OnBnClickedButton7()
{
	int		ii=0;
	int		page = CmnDiagGetValue(&m_PageDiag,1);

	if(ICSMessageBox(_T("最終頁に新規頁を追加します。"),MB_OKCANCEL) == IDCANCEL)	{
		return;
	}

	// 現在頁のデータを書き込む
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,page-1);

	// 最終頁をクリア
	memset(&m_TSrec52,0,sizeof(TSREC52));
	m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,m_TsTbl52.tp_rnum);

	// 頁の分子を最終頁に＋１する
	CmnDiagSetValue(&m_PageDiag,1,m_TsTbl52.tp_rnum,0);

	// ボタンコントロール等
	PageButtonCtrl();
}
// 240305_160343 add <--

// 削除ボタン押下
void CSpeAddDlg::OnBnClickedButton6()
{
	int		ii=0;
	int		page = CmnDiagGetValue(&m_PageDiag,1);

	if(ICSMessageBox(_T("現在の頁を削除してもよろしいですか？"),MB_YESNO|MB_DEFBUTTON2) == IDNO)	{
		return;
	}

	// 現在頁より後ろを１頁ずつ前にずらしていく
	for(ii=page-1; (ii+1)<m_TsTbl52.tp_rnum; ii++)	{
		if(m_TblHdl.th_rget(&m_TSrec52,&m_TsTbl52,ii+1))	{
			break;
		}
		if(m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,ii))	{
			break;
		}
	}

	if(m_TsTbl52.tp_rnum > 1) {
		// 最終頁を削除
		m_TsTbl52.tp_rnum--;

		// 最終頁にいたときは頁の分子をマイナス１する
		if(m_TsTbl52.tp_rnum < page) {
			CmnDiagSetValue(&m_PageDiag, 1, page - 1, 0);
		}
	}
	else {
		memset(&m_TSrec52, 0, sizeof(TSREC52));
		m_TblHdl.th_rput(&m_TSrec52, &m_TsTbl52, 0);
	}

	// ボタンコントロール等
	PageButtonCtrl();
}

BEGIN_EVENTSINK_MAP(CSpeAddDlg,ICSDialog)
	ON_EVENT(CSpeAddDlg,IDC_ICSDIAG8CTRL1,3,CSpeAddDlg::TerminationIcsdiag8ctrl1,VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CSpeAddDlg,IDC_ICSDIAG8CTRL1,2,CSpeAddDlg::EditOFFIcsdiag8ctrl1,VTS_I2)
END_EVENTSINK_MAP()

// 頁コントロールのキー操作
void CSpeAddDlg::TerminationIcsdiag8ctrl1(short index,short nChar,short length,LPUNKNOWN data)
{
	switch(nChar)	{
		// 現在表示しているタブの先頭項目にフォーカスする
		case VK_RETURN:
		case VK_TAB:
			if (m_pSnHeadData->Sn_Sign4 & 0x01) {
				int		page = CmnDiagGetValue(&m_PageDiag, 1);
				if (page != m_CurPage) {
					if (page >= 1 && page <= m_TsTbl52.tp_rnum) {
						// 現在頁のデータを書き込む
						m_TblHdl.th_rput(&m_TSrec52, &m_TsTbl52, m_CurPage - 1);
						// ボタンコントロール等
						PageButtonCtrl();
					}
					else {
						CmnDiagSetValue(&m_PageDiag, 1, m_CurPage, 0);
					}
				}
			}
			else
				DialogSetFocus();
			break;
	}
}

// 頁コントロールのロストフォーカス
void CSpeAddDlg::EditOFFIcsdiag8ctrl1(short index)
{
	int		page = CmnDiagGetValue(&m_PageDiag,1);
	if(page != m_CurPage)	{
		if(page >= 1 && page <= m_TsTbl52.tp_rnum)	{
			// 現在頁のデータを書き込む
			m_TblHdl.th_rput(&m_TSrec52,&m_TsTbl52,m_CurPage-1);
			// ボタンコントロール等
			PageButtonCtrl();
		}
		else	{
			CmnDiagSetValue(&m_PageDiag,1,m_CurPage,0);
		}
	}
}

//LRESULT CSpeAddDlg::OnMyMessageSelChange( WPARAM wParam, LPARAM lParam )
//{
//	m_ShinSpecific52_1_31->CalcSelChange(m_TSrec52.ts521.sel);
//	return(0);
//}

void CSpeAddDlg::DialogSetFocus( void )
{
	int		sel = m_SpeTab.GetCurSel();

	// 5-2(1)
	if(sel == 0)	{
		m_ShinSpecific52_1_31->m_Specific52_1_31.SetPosition(6);
	}
	else if(sel == 1)	{
		// 5-2(2)
		if(m_Type == 0)	{
			m_ShinSpecific52_2_31->m_Specific52_2_31.SetPosition(10);
		}
		// 5-2(3)-1
		else if(m_Type == 1)	{
			m_ShinSpecific52_31_31->m_Specific52_31_31.SetPosition(76);
		}
		// 5-2(4)-1
		else	{
			m_ShinSpecific52_41_31->m_Specific52_41_31.SetPosition(52);
		}
	}
	else if(sel == 2)	{
		// 5-2(3)-2
		if(m_Type == 1)	{
			m_ShinSpecific52_32_31->m_Specific52_32_31.SetPosition(82);
		}
		// 5-2(4)-2
		else	{
			m_ShinSpecific52_42_31->m_Specific52_42_31.SetPosition(58);
		}
	}
	else if(sel == 3)	{
		// 5-2(3)-3
		if(m_Type == 1)	{
			m_ShinSpecific52_33_31->m_Specific52_33_31.SetPosition(82);
		}
		// 5-2(4)-3
		else	{
			m_ShinSpecific52_43_31->m_Specific52_43_31.SetPosition(58);
		}
	}
}
// 24/02/14_特定収入 add <--

//====================================
// 入力ロック表示
//====================================
int CSpeAddDlg::Disp_WriteLock()
{
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);
	if (item == NULL) {
		return -1;
	}

	LOGFONT logfont;
	item->GetFont()->GetLogFont(&logfont);
	logfont.lfWeight = FW_BOLD;

	m_Font.DeleteObject();
	m_Font.CreateFontIndirectA(&logfont);

	item->SetFont(&m_Font, FALSE);

	//カラー設定
	m_InputLock.DefaultColor(FALSE);
	COLORREF color;
	color = PALETTERGB(255, 0, 0);
	m_InputLock.SetBackColor(color);
	color = PALETTERGB(255, 255, 255);
	m_InputLock.SetTextColor(color);


	//入力ロック表示
	if (m_pSnHeadData->Sn_Sign4 & 0x01) {
		GetDlgItem(IDC_STATIC_INPUTLOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_INPUTLOCK)->EnableWindow(FALSE);
	}

	return 0;
}

// 240305_160345 add -->
// 前表 (F7) ボタン押下
void CSpeAddDlg::OnBnClickedButton8()
{
	int selno = m_SpeTab.GetCurSel();
	if(selno == 0)	{
		selno = m_SpeTab.GetItemCount() - 1;
	}
	else	{
		selno--;
	}
	m_SpeTab.SetCurSel(selno);
}

// 次表 (F8) ボタン押下
void CSpeAddDlg::OnBnClickedButton9()
{
	int selno = m_SpeTab.GetCurSel();
	if(selno == m_SpeTab.GetItemCount() - 1)	{
		selno = 0;
	}
	else	{
		selno++;
	}
	m_SpeTab.SetCurSel(selno);
}

// 修正No.168722 add -->
// 初期化ボタン
void CSpeAddDlg::OnBnClickedButton10()
{
	int		rendo;
	CString	str;

	str  = _T("取戻し対象特定収入のデータを全てクリアします。\n");
	str += _T("処理を継続しますか？");
	if(ICSMessageBox(str, (MB_YESNO | MB_DEFBUTTON2)) == IDNO)	{
		return;
	}

	if((m_pSnHeadData->Sn_Sign4 & 0x80) || (m_pSpcListData->m_Util.IsUnConnectMst(m_pZmSub->zvol)))	{	// 非連動
		rendo = 1;
	}
	else	{
		rendo = 0;
	}

	// 初期化処理を実行
	m_TsTbl52.tp_rnum = 0;
	// 1頁分だけ追加
	memset(&m_TSrec52, 0, sizeof(TSREC52));
	m_TblHdl.th_rput(&m_TSrec52, &m_TsTbl52, 0);


	// 頁の分子を1にする
	CmnDiagSetValue(&m_PageDiag, 1, 1, 0);

	// ボタンコントロール等
	PageButtonCtrl();



	//RedrawWindow();
}
// 修正No.168722 add <--

// 前表、次表ボタンの制御
void CSpeAddDlg::ListChgButtonCtrl( void )
{
	if(m_SpeTab.GetItemCount() == 1)	{
		GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
	}
	else	{
		GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
	}
}
// 240305_160345 add <--

void CSpeAddDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if(!(m_pSnHeadData->Sn_Sign4 & 0x01))	{
		if (ICSMessageBox("入力中のデータは破棄されます。よろしいですか？", MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDNO) {
			return;
		}
	}
	EndDialog(0);
}

// 修正No.168706 add -->
void CSpeAddDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	int		sel = 0;
	int		pos = 0;

	switch(nState) {
		case	WA_ACTIVE:			// アクティブ化
		case	WA_CLICKACTIVE:		// マウスクリックでアクティブ
			sel = m_SpeTab.GetCurSel();
			// 5-2(1)
			if(sel == 0)	{
				pos = m_ShinSpecific52_1_31->m_Specific52_1_31.GetPosition();
				m_ShinSpecific52_1_31->m_Specific52_1_31.SetPosition(pos);
			}
			else if(sel == 1)	{
				// 5-2(2)
				if(m_Type == 0)	{
					pos = m_ShinSpecific52_2_31->m_Specific52_2_31.GetPosition();
					m_ShinSpecific52_2_31->m_Specific52_2_31.SetPosition(pos);
				}
				// 5-2(3)-1
				else if(m_Type == 1)	{
					pos = m_ShinSpecific52_31_31->m_Specific52_31_31.GetPosition();
					m_ShinSpecific52_31_31->m_Specific52_31_31.SetPosition(pos);
				}
				// 5-2(4)-1
				else	{
					pos = m_ShinSpecific52_41_31->m_Specific52_41_31.GetPosition();
					m_ShinSpecific52_41_31->m_Specific52_41_31.SetPosition(pos);
				}
			}
			else if(sel == 2)	{
				// 5-2(3)-2
				if(m_Type == 1)	{
					pos = m_ShinSpecific52_32_31->m_Specific52_32_31.GetPosition();
					m_ShinSpecific52_32_31->m_Specific52_32_31.SetPosition(pos);
				}
				// 5-2(4)-2
				else	{
					pos = m_ShinSpecific52_42_31->m_Specific52_42_31.GetPosition();
					m_ShinSpecific52_42_31->m_Specific52_42_31.SetPosition(pos);
				}
			}
			else if(sel == 3)	{
				// 5-2(3)-3
				if(m_Type == 1)	{
					pos = m_ShinSpecific52_33_31->m_Specific52_33_31.GetPosition();
					m_ShinSpecific52_33_31->m_Specific52_33_31.SetPosition(pos);
				}
				// 5-2(4)-3
				else	{
					pos = m_ShinSpecific52_43_31->m_Specific52_43_31.GetPosition();
					m_ShinSpecific52_43_31->m_Specific52_43_31.SetPosition(pos);
				}
			}
			break;
	}

	ICSDialog::OnActivate(nState, pWndOther, bMinimized);
}

// 修正No.168706 add <--
