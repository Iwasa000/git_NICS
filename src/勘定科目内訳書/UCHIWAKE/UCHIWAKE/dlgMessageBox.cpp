//--------------------------------------------------
//	dlgMessageBox.h
//
//	2006.03.09～
//--------------------------------------------------

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "resource.h"
#include "dlgMessageBox.h"

// CdlgMessageBox ダイアログ

IMPLEMENT_DYNAMIC(CdlgMessageBox, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgMessageBox::CdlgMessageBox(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgMessageBox::IDD, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgMessageBox::~CdlgMessageBox()
{
}

void CdlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LABEL, m_lblMessage);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_btn1);
	DDX_Control(pDX, IDC_BUTTON_SECOND, m_btn2);
	DDX_Control(pDX, IDC_BUTTON_THIRD, m_btn3);
}


BEGIN_MESSAGE_MAP(CdlgMessageBox, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CdlgMessageBox::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_SECOND, &CdlgMessageBox::OnBnClickedButtonSecond)
	ON_BN_CLICKED(IDC_BUTTON_THIRD, &CdlgMessageBox::OnBnClickedButtonThird)
END_MESSAGE_MAP()


// CdlgMessageBox メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgMessageBox::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	this->SetWindowTextA( m_szTitle );			//	ダイアログタイトル
	m_lblMessage.UpdateCaption( m_szMessage , 0 );		//	メッセージ
	m_btn1.SetWindowTextA( m_szButton1 );		//	第１ボタン
	m_btn2.SetWindowTextA( m_szButton2 );		//	第２ボタン
	m_btn3.SetWindowTextA( m_szButton3 );		//	第３ボタン
	m_btn2.ShowWindow( m_fButton2 );			//	第２ボタンの表示の有無
	m_btn3.ShowWindow( m_fButton3 );			//	第３ボタンの表示の有無

	// TODO:  ここに初期化を追加してください

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ウィンドウメッセージをフィルタリング
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgMessageBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	int		nID;	//	ID

	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN ){

		//	パラメータで分岐
		switch( pMsg->wParam ){
/*
		//	Esc
		case VK_RETURN:

			//	フォーカスのあるコントロールのID取得
			nID = GetFocusControlID( this );
			
			//	IDで分岐
			switch( nID ){
			//	第１ボタン
			case IDC_BUTTON_FIRST:
				OnBnClickedButtonFirst();
				break;
			//	第２ボタン
			case IDC_BUTTON_SECOND:
				OnBnClickedButtonSecond();
				break;
			//	第３ボタン
			case IDC_BUTTON_THIRD:
				OnBnClickedButtonThird();
				break;
			}
			break;
*/

		// スタティックテキストがあると矢印キーの移動ができないため対応
		case VK_RIGHT:
		case VK_DOWN:
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			return ( 1 );
			break;
		case VK_LEFT:
		case VK_UP:
		//	F2キーが押された
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（第１ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgMessageBox::OnBnClickedButtonFirst()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_nRet = ID_DLG_BUTTON1;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（第２ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgMessageBox::OnBnClickedButtonSecond()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_nRet = ID_DLG_BUTTON2;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（第３ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgMessageBox::OnBnClickedButtonThird()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_nRet = ID_DLG_BUTTON3;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgMessageBox::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}


//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgMessageBox::GetFocusControlID( CWnd* pParent /*=NULL*/)
{
	int		nRet = 0;			//	戻値
	CWnd*	pControl = NULL;	//	コントロール
	
	//	親ハンドルあり？
	if ( pParent != NULL ){

		//	フォーカスのあるコントロールを取得
		pControl = pParent->GetFocus();
		
		//	コントロール取得成功？
		if ( pControl != NULL ){
			//	コントロールID取得
			nRet = pControl->GetDlgCtrlID();
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	ダイアログ表示（ボタン１個）
//	【引数】	szTitle			…	ダイアログタイトル
//				szMessage		…	メッセージ
//										※最大行数は５行
//										※１行の最大文字数は２５文字（全角）
//										※１行内で２５文字を超えると自動折り返しされる
//										※任意の位置で改行したい場合は"\r\n"を文字列に追加
//				szButton1		…	第１ボタンの文言（最大文字数は７文字（全角））
//	【戻値】	ID_DLG_BUTTON1	…	第１ボタンが押された
//
//	|￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣|
//	| ! メッセージ                               |
//	|                                            |
//	|                             [第１ボタン]   |
//	|＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿|
//
//**************************************************
int	CdlgMessageBox::ShowDialog( CString szTitle, CString szMessage,
							    CString szButton1 )
{
	m_nRet = ID_DLG_BUTTON1;	//	戻値初期化
	m_fButton2 = FALSE;			//	第２ボタン非表示
	m_fButton3 = FALSE;			//	第３ボタン非表示
	m_szTitle = szTitle;		//	ダイアログタイトル
	m_szMessage = szMessage;	//	メッセージ取得
	m_szButton1 = szButton1;	//	第１ボタンの文言取得
	m_szButton2.Empty();		//	第２ボタンの文言初期化
	m_szButton3.Empty();		//	第３ボタンの文言初期化

	//	モーダルで表示
	DoModal();

	//	戻値を返す
	return( m_nRet );
}

//**************************************************
//	ダイアログ表示（ボタン２個）
//	【引数】	szTitle			…	ダイアログタイトル
//				szMessage		…	メッセージ
//										※最大行数は５行
//										※１行の最大文字数は２５文字（全角）
//										※１行内で２５文字を超えると自動折り返しされる
//										※任意の位置で改行したい場合は"\r\n"を文字列に追加
//				szButton1		…	第１ボタンの文言（最大文字数は７文字（全角））
//				szButton2		…	第２ボタンの文言（最大文字数は７文字（全角））
//	【戻値】	ID_DLG_BUTTON1	…	第１ボタンが押された
//				ID_DLG_BUTTON2	…	第２ボタンが押された
//
//	|￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣|
//	| ! メッセージ                               |
//	|                                            |
//	|                [第２ボタン] [第１ボタン]   |
//	|＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿|
//
//**************************************************
int	CdlgMessageBox::ShowDialog( CString szTitle, CString szMessage,
							    CString szButton1,CString szButton2 )
{
	m_nRet = ID_DLG_BUTTON1;	//	戻値初期化
	m_fButton2 = TRUE;			//	第２ボタン表示
	m_fButton3 = FALSE;			//	第３ボタン非表示
	m_szTitle = szTitle;		//	ダイアログタイトル
	m_szMessage = szMessage;	//	メッセージ取得
	m_szButton1 = szButton1;	//	第１ボタンの文言取得
	m_szButton2 = szButton2;	//	第２ボタンの文言取得
	m_szButton3.Empty();		//	第３ボタンの文言初期化
	
	//	モーダルで表示
	DoModal();

	//	戻値を返す
	return( m_nRet );
}

//**************************************************
//	ダイアログ表示（ボタン３個）
//	【引数】	szTitle			…	ダイアログタイトル
//				szMessage		…	メッセージ
//										※最大行数は５行
//										※１行の最大文字数は２５文字（全角）
//										※１行内で２５文字を超えると自動折り返しされる
//										※任意の位置で改行したい場合は"\r\n"を文字列に追加
//				szButton1		…	第１ボタンの文言（最大文字数は７文字（全角））
//				szButton2		…	第２ボタンの文言（最大文字数は７文字（全角））
//				szButton3		…	第３ボタンの文言（最大文字数は７文字（全角））
//	【戻値】	ID_DLG_BUTTON1	…	第１ボタンが押された
//				ID_DLG_BUTTON2	…	第２ボタンが押された
//				ID_DLG_BUTTON3	…	第３ボタンが押された
//
//	|￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣|
//	| ! メッセージ                               |
//	|                                            |
//	|   [第３ボタン] [第２ボタン] [第１ボタン]   |
//	|＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿|
//
//**************************************************
int	CdlgMessageBox::ShowDialog( CString szTitle, CString szMessage,
							    CString szButton1, CString szButton2, 
								CString szButton3 )
{
	m_nRet = ID_DLG_BUTTON1;	//	戻値初期化
	m_fButton2 = TRUE;			//	第２ボタン表示
	m_fButton3 = TRUE;			//	第３ボタン表示
	m_szTitle = szTitle;		//	ダイアログタイトル
	m_szMessage = szMessage;	//	メッセージ取得
	m_szButton1 = szButton1;	//	第１ボタンの文言取得
	m_szButton2 = szButton2;	//	第２ボタンの文言取得
	m_szButton3 = szButton3;	//	第３ボタンの文言取得
	
	//	モーダルで表示
	DoModal();

	//	戻値を返す
	return( m_nRet );
}