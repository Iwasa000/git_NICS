// ShinKskRnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKskRnd2.h"


// CShinKskRnd2 ダイアログ

IMPLEMENT_DYNAMIC(CShinKskRnd2, ICSDialog)

CShinKskRnd2::CShinKskRnd2(CWnd* pParent /*=nullptr*/)
	: ICSDialog(CShinKskRnd2::IDD, pParent)
	, m_pSnKskData(NULL)
	, m_pSnHeadData(NULL)
{

}

CShinKskRnd2::~CShinKskRnd2()
{
}

void CShinKskRnd2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CHECK1);
}


BEGIN_MESSAGE_MAP(CShinKskRnd2, ICSDialog)
	ON_BN_CLICKED(IDCANCEL, &CShinKskRnd2::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CShinKskRnd2::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CShinKskRnd2::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CShinKskRnd2 メッセージ ハンドラー


void CShinKskRnd2::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ICSDialog::OnCancel();
}


void CShinKskRnd2::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ICSDialog::OnOK();
}


void CShinKskRnd2::OnBnClickedCheck1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_Ksw = m_CHECK1.GetCheck();
}


BOOL CShinKskRnd2::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CString	str;

	// 非連動
	if( m_pSnHeadData->Sn_Sign4 & 0x80 ){

		str.Format("申告書本表から金額を転記します。\n"
					"※13・21への転記は行いません。\n\n"
					"還付先金融機関等情報も転記する場合は、以下にチェックを付けてください。\n"
					"※既存の内容は上書きされます。");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);

		m_Rsw = 1;
		m_Ksw = 0;
		m_CHECK1.SetCheck(0);

		c_set(IDC_CHECK1);
	}
	// 連動
	else{
		m_CHECK1.ShowWindow( SW_HIDE );

		str.Format("申告書本表から還付先金融機関等情報を転記します。\n"
					"よろしいですか？\n"
					"※既存の内容は上書きされます。");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);

		m_Rsw = 0;
		m_Ksw = 1;

		c_set(IDOK);
	}

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
int CShinKskRnd2::InitInfo(CH31SnKskData *pSnKskData, CSnHeadData *pSnHeadData)
{
	ASSERT(pSnKskData);
	if (pSnKskData == NULL) {
		return -1;
	}
	m_pSnKskData = pSnKskData;

	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	return 0;
}
