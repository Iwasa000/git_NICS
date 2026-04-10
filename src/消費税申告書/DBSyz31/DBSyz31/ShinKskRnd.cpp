// ShinKskRnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKskRnd.h"


// CShinKskRnd ダイアログ

IMPLEMENT_DYNAMIC(CShinKskRnd, ICSDialog)

CShinKskRnd::CShinKskRnd(CWnd* pParent /*=nullptr*/)
	: ICSDialog(CShinKskRnd::IDD, pParent)
	, m_pSnKskData(NULL)
{

}

CShinKskRnd::~CShinKskRnd()
{
}

void CShinKskRnd::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_RADIO1);
	DDX_Control(pDX, IDC_RADIO2, m_RADIO2);
	DDX_Control(pDX, IDC_CHECK1, m_CHECK1);
}


BEGIN_MESSAGE_MAP(CShinKskRnd, ICSDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CShinKskRnd::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CShinKskRnd::OnBnClickedRadio2)
	ON_BN_CLICKED(IDCANCEL, &CShinKskRnd::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CShinKskRnd::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CShinKskRnd::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CShinKskRnd メッセージ ハンドラー


void CShinKskRnd::OnBnClickedRadio1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_Rsw = 0;
}


void CShinKskRnd::OnBnClickedRadio2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_Rsw = 1;
}


void CShinKskRnd::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ICSDialog::OnCancel();
}


void CShinKskRnd::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ICSDialog::OnOK();
}


void CShinKskRnd::OnBnClickedCheck1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_Ksw = m_CHECK1.GetCheck();
}


BOOL CShinKskRnd::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CString	str;
	if (!(m_pSnKskData->KsHead.KHflg & 0x01)) {
		str = _T("この請求前の金額");
	}
	else {
		switch (m_pSnKskData->KsData.KSktsw) {
		case 0:	str = _T("確定額（申告額）");	break;
		case 1:	str = _T("確定額（更正額）");	break;
		case 2:	str = _T("確定額（決定額）");	break;
		}
	}
	GetDlgItem(IDC_RADIO1)->SetWindowText(str);

	if (!(m_pSnKskData->KsHead.KHflg & 0x01)) {
		str.Format("更正の請求金額");
	}
	else {
		str.Format("正当とする額");
	}
	GetDlgItem(IDC_RADIO2)->SetWindowText(str);

	m_Rsw = 1;
	m_RADIO2.SetCheck(m_Rsw);

	m_Ksw = 0;
	m_CHECK1.SetCheck(0);

	c_set(IDC_RADIO2);

	ICSDialog::OnInitDialogEX();


	return FALSE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnKskData	：	更正の請求データ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CShinKskRnd::InitInfo(CH31SnKskData *pSnKskData)
{
	ASSERT(pSnKskData);
	if (pSnKskData == NULL) {
		return -1;
	}
	m_pSnKskData = pSnKskData;

	return 0;
}
