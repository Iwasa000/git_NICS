// 改良依頼No.18-0686 ダイアログ新規作成

// JobEnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "DlgJobEnd2.h"

#define ICS_STANDARDCOLOR_ON			1								// 「共通設定」アプリの「設定した色を業務に使用」チェックの値

// CJobEnd ダイアログ

IMPLEMENT_DYNAMIC(CDlgJobEnd2, ICSDialog)

CDlgJobEnd2::CDlgJobEnd2(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_JOBEND2, pParent)
{
	m_nRet = -1;
}

CDlgJobEnd2::~CDlgJobEnd2()
{
}

void CDlgJobEnd2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICON_PLACE, m_IconPlace);
	DDX_Control(pDX, IDC_STATIC2, m_StaticMsg2);
}


BEGIN_MESSAGE_MAP(CDlgJobEnd2, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgJobEnd2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgJobEnd2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgJobEnd2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgJobEnd2::OnBnClickedButton4)
END_MESSAGE_MAP()


// CJobEnd メッセージ ハンドラー

// 入力ロックボタン（入力ロック後、業務を終了する）
void CDlgJobEnd2::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 戻り値をセット
	m_nRet = IDYES;

	ICSDialog::EndDialog(IDYES);
}

// いいえボタン（業務を終了する）
void CDlgJobEnd2::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 戻り値をセット
	m_nRet = IDNO;

	ICSDialog::EndDialog(IDNO);
}

// 入力に戻るボタン（終了処理を中断）
void CDlgJobEnd2::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 戻り値をセット
	m_nRet = IDINVALID_ICS;

	ICSDialog::EndDialog(IDINVALID_ICS);
}

// はいボタン（会社基本情報画面に戻る）
void CDlgJobEnd2::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 戻り値をセット

	// ※ICSExtBoxとボタンの数が異なるため、
	//   101の値を変更するようにしています
	m_nRet = 101;

	ICSDialog::EndDialog(101);
}

//************************************************
// ダイアログ表示
//【引数】	title		メッセージボックスのタイトルテキスト。
//			text		メッセージボックスに表示するテキスト。
//			btn1text	使用不可（第一ボタンの表示テキスト。）
//			btn2text	使用不可（第二ボタンの表示テキスト。）
//			btn3text	使用不可（第三ボタンの表示テキスト。）
//			btn4text	使用不可（第四ボタンの表示テキスト。）
//			type		使用不可（メッセージボックスのアイコンやフォーカスを持つボタンなどを指定する。）
//			help		使用不可
//			point		使用不可
//			※ ICSExtBox() と引数を合わせるため使用不可のものを設定しています
//			※ ICSExtBox() よりも一つボタンの数が多いためbtn4textを追加し、返送値も101を返すようにしています。
//【戻値】 IDYES:申告完了/IDNO:処理終了/IDINVALID_ICS:処理に戻る
//************************************************
int	CDlgJobEnd2::ShowDialog( LPCTSTR title, LPCTSTR text, LPCTSTR btn1text, LPCTSTR btn2text, LPCTSTR btn3text, LPCTSTR btn4text, UINT type, UINT help, short point, CWnd* pParent)
{
	// 引数をメンバ変数に保持
	m_strTitle = title;
	m_strText = text;

	// ダイアログ表示
	DoModal();

	return m_nRet;
}

BOOL CDlgJobEnd2::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	this->SetWindowText(m_strTitle);
	GetDlgItem(IDC_STATIC1)->SetWindowText(m_strText);

	HICON hIcon;
	hIcon = AfxGetApp()->LoadStandardIcon(IDI_QUESTION);
	if (hIcon != NULL) {
		// 見つかった場合、アイコンを表示
		m_IconPlace.SetIcon(hIcon);
	}

	// 現在設定されている背景色をICSDiagに設定
	COLORREF		DlgColor;
	ICSColorInfo	m_ColorInfo;
	DlgColor = GetSysColor( COLOR_BTNFACE );
	if(((ICSWinApp*)AfxGetApp())->GetStanderdColor(m_ColorInfo) == 0){
		if(m_ColorInfo.m_swOnOff == ICS_STANDARDCOLOR_ON){
			DlgColor = m_ColorInfo.ViewColor();
		}
	}

	m_StaticMsg2.DefaultColor(FALSE);
	m_StaticMsg2.SetTextColor(RGB(0xFF, 0x00, 0x00));
	m_StaticMsg2.SetBackColor(DlgColor);

	// 終了ダイアログの再配置
	CRect rcOrg, rcResize;
	GetWindowRect( &rcOrg );	// ダイアログオリジナルサイズ

	//********************************************
	// リサイズ
	//********************************************
	ICSDialog::OnInitDialogEX();

	GetWindowRect( &rcResize );		// リサイズ後のダイアログウィンドウ
	// 再配置
	ResetDlgWindow( rcOrg, rcResize );
	GetDlgItem(IDC_BUTTON2)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgJobEnd2::ResetDlgWindow( CRect rcOrg, CRect rcResize )
{

	CString str;

	int xrate = rcResize.Width()*100 / rcOrg.Width();
	int yrate = rcResize.Height()*100 / rcOrg.Height();

	CRect rc1, rc2;
	str.Format( "xr = %d, yr = %d", xrate, yrate );
//	AfxMessageBox( str );

	if( yrate <= 100 ) return;
	if( (xrate * 100 / yrate) > 160 ) return;	// 横方向への伸長が　縦方向の伸長の1.6倍以上の場合は調整しない。


//	str.Format( "rate = %d", xrate * 100 / yrate  );
//	AfxMessageBox( str );

	rc1 = rcOrg;
	rc2 = rcResize;
	if( xrate && yrate && xrate > yrate ) { // 横方向に伸長されている場合にのみ調整する

		// ダイアログウィンドウ
		if( xrate > yrate ) {
			rc2.right = rc2.right * 100 / xrate * yrate / 100+50;
		}
		else rc2.bottom = rc2.bottom * 100 / yrate * xrate / 100;

		MoveWindow( &rc2 );
		CenterWindow( GetParent() );

// ボタン　いいえ
		GetDlgItem(IDC_BUTTON2)->GetWindowRect( &rc2 );
		ScreenToClient( &rc2 );
		if( xrate > yrate ) {
			rc2.left = rc2.left * 100 / xrate * yrate / 100;
			rc2.right = rc2.right * 100 / xrate * yrate / 100;
		}
		else {
			rc2.top = rc2.top * 100 / yrate * xrate / 100;
			rc2.bottom = rc2.bottom * 100 / yrate * xrate / 100;
		}
		GetDlgItem(IDC_BUTTON2)->MoveWindow( &rc2 );

// ボタン　入力に戻る
		GetDlgItem(IDC_BUTTON3)->GetWindowRect( &rc2 );
		ScreenToClient( &rc2 );
		if( xrate > yrate ) {
			rc2.left = rc2.left * 100 / xrate * yrate / 100;
			rc2.right = rc2.right * 100 / xrate * yrate / 100;
		}
		else {
			rc2.top = rc2.top * 100 / yrate * xrate / 100;
			rc2.bottom = rc2.bottom * 100 / yrate * xrate / 100;
		}
		GetDlgItem(IDC_BUTTON3)->MoveWindow( &rc2 );

// ボタン　はい
		GetDlgItem(IDC_BUTTON4)->GetWindowRect( &rc2 );
		ScreenToClient( &rc2 );
		if( xrate > yrate ) {
			rc2.left = rc2.left * 100 / xrate * yrate / 100;
			rc2.right = rc2.right * 100 / xrate * yrate / 100;
		}
		else {
			rc2.top = rc2.top * 100 / yrate * xrate / 100;
			rc2.bottom = rc2.bottom * 100 / yrate * xrate / 100;
		}
		GetDlgItem(IDC_BUTTON4)->MoveWindow( &rc2 );

// ボタン　入力ロック
		GetDlgItem(IDC_BUTTON1)->GetWindowRect( &rc2 );
		ScreenToClient( &rc2 );
		if( xrate > yrate ) {
			rc2.left = rc2.left * 100 / xrate * yrate / 100;
			rc2.right = rc2.right * 100 / xrate * yrate / 100;
		}
		else {
			rc2.top = rc2.top * 100 / yrate * xrate / 100;
			rc2.bottom = rc2.bottom * 100 / yrate * xrate / 100;
		}
		GetDlgItem(IDC_BUTTON1)->MoveWindow( &rc2 );


		GetWindowRect( &rc1 );
// メッセージ１
		GetDlgItem( IDC_STATIC1 )->GetWindowRect( &rc2 );
		rc2.right = rc1.right-20;
		ScreenToClient( &rc2 );
		GetDlgItem( IDC_STATIC1 )->MoveWindow( &rc2 );

// メッセージ２
		GetDlgItem( IDC_STATIC2 )->GetWindowRect( &rc2 );
		rc2.right = rc1.right-20;
		ScreenToClient( &rc2 );
		GetDlgItem( IDC_STATIC2)->MoveWindow( &rc2 );
	}
}