// dlgYakuZokuSelect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgYakuZokuSelect.h"

// midori UC_0045 add -->
extern	BOOL	bG_Kanso;
// midori UC_0045 add <--

// CdlgYakuZokuSelect ダイアログ

IMPLEMENT_DYNAMIC(CdlgYakuZokuSelect, ICSDialog)

CdlgYakuZokuSelect::CdlgYakuZokuSelect(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgYakuZokuSelect::IDD, pParent)
{

}

CdlgYakuZokuSelect::~CdlgYakuZokuSelect()
{
}

void CdlgYakuZokuSelect::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_YAKUZOKU, m_listbox);
}


BEGIN_MESSAGE_MAP(CdlgYakuZokuSelect, ICSDialog)
	ON_LBN_DBLCLK(IDC_LIST_YAKUZOKU, &CdlgYakuZokuSelect::OnLbnDblclkListYakuzoku)
END_MESSAGE_MAP()

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgYakuZokuSelect::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	// タイトル初期化
	InitTitel();
	// リストボックスの初期化
	InitListBox();
	
	// 戻値を返す
	return( TRUE );
}

// midori 154816 del -->
////**************************************************
////	ダイアログ表示
////	【引数】	nType			…	区分タイプ(0 : 役職名 1 : 代表者との関係（続柄）
////	【戻値】	ID_DLG_OK		…	Enterキーが押された
////				ID_DLG_CANCEL	…	Escキーが押された
////**************************************************
//int CdlgYakuZokuSelect::ShowDialog(int nType/*ID_KUBUN_SELECT_YAKUSYOKU*/)
//{
//	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
//	m_nType = 0;
//	m_RetStr.Empty();
//
//	m_nType = nType;		//	区分タイプ
//	DoModal();				//	モーダルで表示
//
//	//	戻値を返す
//	return( m_nRet );
//}
// midori 154816 del <--
// midori 154816 add -->
//**************************************************
//	ダイアログ表示
//	【引数】	nType			…	区分タイプ(0 : 役職名 1 : 代表者との関係（続柄）
//				strKankei		…	現在入力されている「役職名」もしくは「代表者との関係」
//	【戻値】	ID_DLG_OK		…	Enterキーが押された
//				ID_DLG_CANCEL	…	Escキーが押された
//**************************************************
int CdlgYakuZokuSelect::ShowDialog(int nType/*ID_KUBUN_SELECT_YAKUSYOKU*/, CString strKankei/*NULL*/)
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
	m_nType = 0;
	m_strKankei = "";
	m_RetStr.Empty();

	m_nType = nType;			//	区分タイプ
	m_strKankei = strKankei;	//	現在入力されている「役職名」もしくは「代表者との関係」

	DoModal();					//	モーダルで表示

	//	戻値を返す
	return( m_nRet );
}
// midori 154816 add <--

//**************************************************
//	リストボックス初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
int CdlgYakuZokuSelect::InitListBox()
{
	int nRet = 0;
	int nMax = 0;
	m_arryKubunData.RemoveAll();

	switch( m_nType ){
		case ID_KUBUN_SELECT_YAKUSYOKU:
			nMax = sizeof NAME_YAKU / sizeof NAME_YAKU[0];
			for(int i=0; i<nMax; i++){
				m_arryKubunData.Add(NAME_YAKU[i]);
			}
			break;

		case ID_KUBUN_SELECT_KANKEI:
// midori UC_0045 del -->
			//nMax = sizeof NAME_KANKEI / sizeof NAME_KANKEI[0];
			//for(int i=0; i<nMax; i++){
			//	m_arryKubunData.Add(NAME_KANKEI[i]);
			//}
// midori UC_0045 del <--
// midori UC_0045 add -->
			if(bG_Kanso == TRUE)	{
				nMax = sizeof NAME_KANKEI_K / sizeof NAME_KANKEI_K[0];
				for(int i=0; i<nMax; i++){
					m_arryKubunData.Add(NAME_KANKEI_K[i]);
				}
			}
			else	{
				nMax = sizeof NAME_KANKEI / sizeof NAME_KANKEI[0];
				for(int i=0; i<nMax; i++){
					m_arryKubunData.Add(NAME_KANKEI[i]);
				}
			}
// midori UC_0045 add <--
			break;
	}

	// リストボックスにデータセット
	for(int i=0; i<nMax; i++){
		m_listbox.AddString( "･" + m_arryKubunData.GetAt(i) );
	}

	// リストボックスの配置設定
	CRect ColRect;
	m_listbox.GetClientRect(ColRect);

// midori UC_0046 del -->
//	int nColumnWidth = ColRect.Width() / 3;
// midori UC_0046 del <--
// midori UC_0046 add -->
	int nColumnWidth = ColRect.Width() / 4;
// midori UC_0046 add <--

	m_listbox.SetColumnWidth(nColumnWidth);

// midori 154816 del -->
//	m_listbox.SetCurSel( 0 );		// 始めの位置
// midori 154816 del <--
// midori 154816 add -->
	int		nFocus=0;
	CString	sTemp="";

	nFocus = 0;
	sTemp = "";
	nMax = (int)m_arryKubunData.GetCount();

	if(m_strKankei != "") {
// midori 154827 add -->
		// 当てはまる文字がなかったら「その他」にインバースを掛けるために番号をセットしておく
		nFocus = nMax-1;
// midori 154827 add <--
		// 位置より文字列を検索
		for(int ii=0; ii<nMax; ii++)	{
			sTemp = m_arryKubunData.GetAt(ii);
			if(m_strKankei == sTemp) {
				nFocus = ii;
				break;
			}
		}
	}
	m_listbox.SetCurSel(nFocus);		// 始めの位置
// midori 154816 add <--

	//CRect DlgRect;
	//int nWidth, nHeight;
	//this->GetWindowRect(&DlgRect);					// ﾀﾞｲｱﾛｸﾞのｻｲｽﾞ
	//nWidth = DlgRect.Width();
	//nHeight = DlgRect.Height();

	//DlgRect.left = m_InpRect.left;
	//DlgRect.right = DlgRect.left + nWidth;
	//DlgRect.top = m_InpRect.bottom;
	//DlgRect.bottom = DlgRect.top + nHeight;

	//MoveWindow( &DlgRect );

	return nRet = 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgYakuZokuSelect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	int nSelNo = 0;
	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	Enterキー or ダブルクリック
		case VK_RETURN:
			nSelNo = GetData();				// 選択された文字列の取得
			m_nRet = ID_DLG_OK;
//			EndDialog( IDOK );
			break;
		case VK_ESCAPE:
//			EndDialog( IDCANCEL );
			m_nRet = ID_DLG_CANCEL;
//			return TRUE;
			break;
		}
	}
	else if( pMsg->message == WM_LBUTTONDBLCLK ){
		nSelNo = GetData();				// 選択された文字列の取得
		m_nRet = ID_DLG_OK;
		EndDialog( ID_DLG_OK );
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYakuZokuSelect::OnLbnDblclkListYakuzoku()
{
	int selno = GetData();				// 選択された文字列の取得

	//EndDialog( ID_DLG_OK );
}

//**************************************************
//	選択された文字列の取得
//	【引数】	なし
//	【戻値】	なし
//**************************************************
int CdlgYakuZokuSelect::GetData()
{
	int nSelNo = m_listbox.GetCurSel();		// 現在の位置を取得
	int nMax = (int)m_arryKubunData.GetCount();

	// 位置より文字列を検索
	for(int i = 0; i<nMax; i++){
		if( nSelNo == i ){
			m_RetStr = m_arryKubunData.GetAt(i);
		}
	}

	return nSelNo;
}

//**************************************************
//	ダイアログのタイトル初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
int CdlgYakuZokuSelect::InitTitel()
{
	CString strTitle;

	switch( m_nType ){
		case ID_KUBUN_SELECT_YAKUSYOKU:
			strTitle = _T("役職選択");
			break;
		case ID_KUBUN_SELECT_KANKEI:
			strTitle = _T("続柄選択");
			break;
	}

	SetWindowText(strTitle);

	return 0;
}