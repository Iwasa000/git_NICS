// dlgYokukiMessage.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgYokukiMessage.h"


// CdlgYokukiMessage ダイアログ

IMPLEMENT_DYNAMIC(CdlgYokukiMessage, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgYokukiMessage::CdlgYokukiMessage(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgYokukiMessage::IDD, pParent)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgYokukiMessage::~CdlgYokukiMessage()
{
}

void CdlgYokukiMessage::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LABEL, m_lblMessage);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CdlgYokukiMessage, ICSDialog)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgYokukiMessage::OnBnClickedOkButton)
END_MESSAGE_MAP()


// CdlgYokukiMessage メッセージ ハンドラ

BOOL CdlgYokukiMessage::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 翌期更新確認メッセージ作成
	CString strMess;
	strMess = CreateYokukiMessage();
	m_lblMessage.UpdateCaption( strMess , 0 );

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMessage::OnBnClickedOkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMessage::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgYokukiMessage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

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

			//	F2キーをShift + Tabイベントに書き換え
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );									//	抜ける
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	翌期更新確認メッセージ作成
//	【引数】	なし
//	【戻値】	翌期更新確認メッセージ
//**************************************************
CString CdlgYokukiMessage::CreateYokukiMessage()
{
	CString	strZvol;		// 会社マスター決算期間メッセージ
	CString strUchi;		// 内訳書決算期間メッセージ
	CString strMess;		// 確認メッセージ
	CdateConvert dateCnv;	// 日付変換クラス
	char	cZvolSSymd[3];	// 会社マスター決算期間(開始)
	char	cZvolEEymd[3];	// 会社マスター決算期間(終了)
	char	cUchiSSymd[3];	// 内訳書決算期間(開始)
	char	cUchiEEymd[3];	// 内訳書決算期間(終了)

	// 初期化
	strZvol.Empty();
	strUchi.Empty();
	strMess.Empty();
	ZeroMemory( cZvolSSymd , sizeof(char[3]));
	ZeroMemory( cZvolEEymd , sizeof(char[3]));
	ZeroMemory( cUchiSSymd , sizeof(char[3]));
	ZeroMemory( cUchiEEymd , sizeof(char[3]));

	// 会社マスター決算期間(開始)変換
	dateCnv.Convert( m_lngZvolSSymd );
	cZvolSSymd[0] = dateCnv.m_cWareki;
	cZvolSSymd[1] = dateCnv.m_cMonth;
	cZvolSSymd[2] = dateCnv.m_cDay;

	// 会社マスター決算期間(終了)変換
	dateCnv.Convert( m_lngZvolEEymd );
	cZvolEEymd[0] = dateCnv.m_cWareki;
	cZvolEEymd[1] = dateCnv.m_cMonth;
	cZvolEEymd[2] = dateCnv.m_cDay;

	// 内訳書決算期間(開始)変換
	dateCnv.Convert( m_lngUchiSSymd );
	cUchiSSymd[0] = dateCnv.m_cWareki;
	cUchiSSymd[1] = dateCnv.m_cMonth;
	cUchiSSymd[2] = dateCnv.m_cDay;

	// 内訳書決算期間(終了)変換
	dateCnv.Convert( m_lngUchiEEymd );
	cUchiEEymd[0] = dateCnv.m_cWareki;
	cUchiEEymd[1] = dateCnv.m_cMonth;
	cUchiEEymd[2] = dateCnv.m_cDay;

	// 文字列作成
	strZvol.Format( "会社　期間：　%02x年%02x月%02x日　～　%02x年%02x月%02x日\n" , 
						cZvolSSymd[0] , cZvolSSymd[1] , cZvolSSymd[2] , cZvolEEymd[0] , cZvolEEymd[1] , cZvolEEymd[2] );

	strUchi.Format( "内訳書期間：　%02x年%02x月%02x日　～　%02x年%02x月%02x日\n\n" , 
						cUchiSSymd[0] , cUchiSSymd[1] , cUchiSSymd[2] , cUchiEEymd[0] , cUchiEEymd[1] , cUchiEEymd[2] );

	strMess = strZvol + strUchi + "決算期間が異なります。金額欄等の更新を行いますか？";


	return strMess;

}

//**************************************************
//	ダイアログ表示
//	【引数】	lngZvolSSymd	…	会社マスター決算期間(開始)
//				lngZvolEEymd	…	会社マスター決算期間(終了)
//				lngUchiSSymd	…	内訳書決算期間(開始)
//				lngUchiEEymd	…	内訳書決算期間(終了)
//	【戻値】	ID_DLG_OK		…	OKボタンで終了
//				ID_DLG_CANCEL	…	キャンセルボタンで終了
//**************************************************
int	CdlgYokukiMessage::ShowDialog( long lngZvolSSymd , long lngZvolEEymd , long lngUchiSSymd , long lngUchiEEymd )
{
	m_nRet = ID_DLG_CANCEL;			//	戻値を初期化

	m_lngZvolSSymd = lngZvolSSymd;	//	財務マスター開始期間
	m_lngZvolEEymd = lngZvolEEymd;	//	財務マスター終了期間
	m_lngUchiSSymd = lngUchiSSymd;	//	内訳書保存開始期間
	m_lngUchiEEymd = lngUchiEEymd;	//	内訳書保存終了期間

	//	モーダルで表示
	DoModal();

	//	戻値を返す
	return( m_nRet );
}
