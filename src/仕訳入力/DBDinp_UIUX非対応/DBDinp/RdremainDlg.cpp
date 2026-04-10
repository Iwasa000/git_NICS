// RdremainDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "RdremainDlg.h"

#include "ICSDisp.h"
#include "font.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRdremainDlg ダイアログ

IMPLEMENT_DYNAMIC(CRdremainDlg, ICSDialog)

CRdremainDlg::CRdremainDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CRdremainDlg::IDD, pParent)
{
	m_pParent = pParent;

	Create( CRdremainDlg::IDD, pParent );
}

CRdremainDlg::~CRdremainDlg()
{
}

void CRdremainDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRdremainDlg, ICSDialog)
END_MESSAGE_MAP()


// CRdremainDlg メッセージ ハンドラ

BOOL CRdremainDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	ICSDialog::OnInitDialogEX();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CRdremainDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;

//	ICSDialog::PostNcDestroy();
}


void CRdremainDlg::SetRemainString(CString& str, int height )
{
	CWnd* dspwnd = GetDlgItem(IDC_ICSDISPRDREMAIN);

	static COleFont	oft;
	oft = ((CICSDisp*)dspwnd)->GetFont();
	CY y = oft.GetSize();

	const int minsz = 16;
	if( height <= minsz ) {
		y.Lo = 100000;
	}
	else {
		int sa = height - minsz;
		if( sa >= 11 )	sa = 10;
		int plus = (sa * 8)*1000;
		y.Lo = 100000 + plus;
	}
//	MyTrace("SetRemainString (%s) height = [%d], %d\n", str, height, y.Lo);
	oft.SetSize(y);
	((CICSDisp*)dspwnd)->SetFont(oft);

	((CICSDisp*)dspwnd)->ColmDisp(0, str);
}

void CRdremainDlg::PreSubclassWindow()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	ICSDialog::PreSubclassWindow();
}
