// KoketsuMeisyo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "KoketsuMeisyo.h"


// CKoketsuMeisyo ダイアログ

IMPLEMENT_DYNAMIC(CKoketsuMeisyo, ICSDialog)

CKoketsuMeisyo::CKoketsuMeisyo(CWnd* pParent /*=NULL*/)
	: ICSDialog(CKoketsuMeisyo::IDD, pParent)
{

}

CKoketsuMeisyo::~CKoketsuMeisyo()
{
}

void CKoketsuMeisyo::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_RadioY);
}


BEGIN_MESSAGE_MAP(CKoketsuMeisyo, ICSDialog)
	ON_BN_CLICKED(IDC_KOKETSUMEISYO_Y, &CKoketsuMeisyo::OnBnClickedKoketsumeisyoY)
	ON_BN_CLICKED(IDC_KOKETSUMEISYO_N, &CKoketsuMeisyo::OnBnClickedKoketsumeisyoN)
	ON_BN_CLICKED(IDOK, &CKoketsuMeisyo::OnBnClickedOk)
END_MESSAGE_MAP()


// CKoketsuMeisyo メッセージ ハンドラ

void CKoketsuMeisyo::OnBnClickedKoketsumeisyoY()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_KHkoke == 0 ){
		m_KHkoke = 1;
	}
}

void CKoketsuMeisyo::OnBnClickedKoketsumeisyoN()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_KHkoke == 1 ){
		m_KHkoke = 0;
	}
}

BOOL CKoketsuMeisyo::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

int CKoketsuMeisyo::Init()
{
	if( m_KHkoke ){
		CheckRadioButton( IDC_KOKETSUMEISYO_Y, IDC_KOKETSUMEISYO_N, IDC_KOKETSUMEISYO_Y );
		//m_RadioY.SetCheck( 1 );
	}
	else{
		CheckRadioButton( IDC_KOKETSUMEISYO_Y, IDC_KOKETSUMEISYO_N, IDC_KOKETSUMEISYO_N );
		//m_RadioY.SetCheck( 0 );
	}
	

	return 0;
}
void CKoketsuMeisyo::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_KHkoke == 1 ){
		//ICSMessageBox("課税取引金額計算表の科目名が入力不可になります。\n（個人決算書名称変更から取得されるようになります。）");
		ICSMessageBox("課税取引金額計算表の科目欄へ個人決算書名称を転記します。\n転記後は入力不可になります。");
	}else if( m_KHkoke == 0 ){
		//ICSMessageBox("課税取引金額計算表の科目名が入力可能になります。");
		ICSMessageBox("課税取引金額計算表へ転記した個人決算書名称はクリアされ、\n科目欄が入力可能になります。");
	}

	OnOK();
}
