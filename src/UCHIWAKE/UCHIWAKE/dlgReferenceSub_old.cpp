//--------------------------------------------------
//	dlgreferencesub_old.cpp
//
//	2023.11.14～
//	dlgreferencesub.cppの変更前。
//	dlgreferencesub.cppはインボイス登録番号追加版に変更
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgreferencesub_old.h"
#include "CharController.h"
// 修正No.157628 add -->
#include "frmUc000Common.h"
// 修正No.157628 add <--

IMPLEMENT_DYNAMIC(CdlgReferenceSub_Old, ICSDialog)

// No.200601 add -->
extern	BOOL	bG_Kanso;
// No.200601 add <--

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgReferenceSub_Old::CdlgReferenceSub_Old(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgReferenceSub_Old::IDD, pParent)
{
// 修正No.157628 add -->
	m_pOwner = pParent;
// 修正No.157628 add <--
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgReferenceSub_Old::CdlgReferenceSub_Old( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
// 修正No.157628 add -->
	m_pOwner = pParent;
// 修正No.157628 add <--
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgReferenceSub_Old::~CdlgReferenceSub_Old()
{
}

void CdlgReferenceSub_Old::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_DIAG, m_diagInput);
	DDX_Control(pDX, ID_SEARCH, m_btnSearch);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CdlgReferenceSub_Old, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReferenceSub_Old::OnBnClickedOk)
	ON_BN_CLICKED(ID_SEARCH, &CdlgReferenceSub_Old::OnBnClickedSearch)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgReferenceSub_Old, ICSDialog)
	ON_EVENT(CdlgReferenceSub_Old, IDC_INPUT_DIAG, 3, CdlgReferenceSub_Old::TerminationInputDiag, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CdlgReferenceSub_Old, IDC_INPUT_DIAG, 11, CdlgReferenceSub_Old::ReadStringIcsdiag8Kanasearch, VTS_I2 VTS_BSTR)
END_EVENTSINK_MAP()

void CdlgReferenceSub_Old::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgReferenceSub_Old::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReferenceSub_Old::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 背景色取得
	ICSColorInfo icsColInfo;
	m_ViewColor = GetSysColor( COLOR_BTNFACE );
	if ( ((ICSWinApp *)AfxGetApp())->GetStanderdColor( icsColInfo ) == 0 ){
		if ( icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON ){
			m_ViewColor = icsColInfo.ViewColor();
		}
	}

	//	ディスプレイ初期化
	InitDisplay( m_nType, m_szTitle, m_szOrder, m_szName1, m_szName2, m_szAdd1, m_szAdd2, m_szKana1, m_szKana2 );
	// リサイズ
	OnInitDialogEX();
	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（検索ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub_Old::OnBnClickedSearch()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	銀行／住所検索ダイアログ表示
	ShowSearchDialog( m_nType );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub_Old::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	int		nRet;		//	戻値
	
	//	入力データチェック
// 改良No.22-0404,22-0812 del -->
	//nRet = CheckInputData( m_nType, m_szName1, m_szName2, m_szAdd1, m_szAdd2, m_szKana1, m_szKana2, m_nSeq );
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
	nRet = CheckInputData(m_nType, m_szName1, m_szName2, m_szAdd1, m_szAdd2, m_szKana1, m_szKana2, m_nGrSeq, m_nSeq);
// 改良No.22-0404,22-0812 add <--

	//	戻値で分岐
	switch( nRet ){
	//	編集あり（重複なし）
	case 1:
		//	戻値をOKに設定
		m_nRet = ID_DLG_OK;
		break;

	//	編集あり（重複あり）
	case 2:
		ICSMessageBox( "既に登録されています。", MB_OK, 0, 0, this );	//	警告メッセージ
		return;															//	抜ける
		break;

	//	入力なし
	case 3:
		ICSMessageBox( "入力されていません。", MB_OK, 0, 0, this );		//	警告メッセージ
		return;															//	抜ける
		break;

// No.200601 add -->
	// 戻す（電子申告文字数チェックメッセージで「いいえ(N)」を選択）
	case 4:
		return;															//	抜ける
// No.200601 add <--

	//	その他（編集なし）
	default:
		break;
	}

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub_Old::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージを処理する前に発生するイベント
//	【引数】	pMsg	…	MSG構造体のポインタ
//	【戻値】	0		…	メッセージを通常の方法で処理
//				0以外	…	メッセージ処理を終了
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReferenceSub_Old::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	char	cShift = 0;	//	SHIFTキー状態
	int		nID = 0;	//	ID
	BOOL	bFlag = FALSE;

	nID = GetFocusControlID( this );			//	現在フォーカスのあるコントロールID取得

	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	「Tab」キーが押された
		case VK_TAB:
			
			cShift = ( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得
			
			//	SHIFTキーが押されてる？
			if ( cShift & 0x80 ){

				//	検索ボタンにフォーカスあり？
				if ( nID == ID_OK_BUTTON ){
					
					//	参照型で分岐
					switch( m_nType ){
					//	金融機関
					case ID_DLGTYPE_BANK:
						m_diagInput.SetPosition( 20 );	//	金融機関用ｶﾅ名テキストに移動
						return( 1 );
						break;
					//	取引先
					case ID_DLGTYPE_ADDRESS:
						m_diagInput.SetPosition( 16 );	//	取引先用ｶﾅテキストに移動
						return( 1 );
						break;
					//	科目
					case ID_DLGTYPE_KAMOKU:
						m_diagInput.SetPosition( 18 );	//	科目用ｶﾅテキストに移動
						return( 1 );
						break;
					}
				}
			}
			//	SHIFTキーが押されてない
			else{

				//	検索ボタンにフォーカスあり？
				if ( nID == ID_SEARCH ){
					
					//	参照型で分岐
					switch( m_nType ){
					//	金融機関
					case ID_DLGTYPE_BANK:
						m_diagInput.SetPosition( 3 );	//	金融機関テキストに移動
						return( 1 );
						break;
					//	取引先
					case ID_DLGTYPE_ADDRESS:
						m_diagInput.SetPosition( 8 );	//	名称テキストに移動
						return( 1 );
						break;
					}
				}
				//	キャンセルボタンにフォーカスあり
				else if ( nID == IDCANCEL ){
					
					//	科目？
					if ( m_nType == ID_DLGTYPE_KAMOKU ){
						m_diagInput.SetPosition( 3 );	//	名称テキストに移動
						return( 1 );
					}
				}
			}
			break;
		
		//	「F2」キー押した
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;

		//	「F7」ボタン押した
		case VK_F7:

			//	金融機関？
			if ( m_nType == ID_DLGTYPE_BANK ){
				//	検索ボタン起動
				PostMessage( WM_COMMAND, ID_SEARCH );
			}
			break;

		//	「HOME」ボタン押した
		case VK_HOME:

			//	取引先？
			if ( m_nType == ID_DLGTYPE_ADDRESS ){
				//	検索ボタン起動
				PostMessage( WM_COMMAND, ID_SEARCH );
			}
			break;
		
		//	↑キーが押された
		case VK_UP:
		//	←キーが押された
		case VK_LEFT:
	
			//	OKボタンにフォーカスあり？
			if ( nID == ID_OK_BUTTON ){		
				keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}
			break; 

		//	↓キーが押された
		case VK_DOWN:
		//	→キーが押された
		case VK_RIGHT:
			
			//	参照型で分岐
			switch( m_nType ){
			//	金融機関
			case ID_DLGTYPE_BANK:
			//	取引先
			case ID_DLGTYPE_ADDRESS:

				//	検索ボタンにフォーカスあり？
				if ( nID == ID_SEARCH ){
					bFlag = TRUE;
				}
				break;
			
			//	科目
			case ID_DLGTYPE_KAMOKU:
				
				//	キャンセルボタンにフォーカスあり？
				if ( nID == IDCANCEL ){
					bFlag = TRUE;
				}
				break;
			}

			//	フラグオン？
			if ( bFlag == TRUE ){
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}
			break;

		//	その他
		default:
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（ICSDiagコントロール）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub_Old::TerminationInputDiag(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	char	cShift = 0;		//	SHIFTキー状態

	//	仮想キーコードで分岐
	switch( nChar ){
	//	「Tab」キーが押された
	case VK_TAB:
	//	「Enter」キーが押された
	case VK_RETURN:
		
		cShift = ( char )GetKeyState( VK_SHIFT );				//	SHIFTキー状態取得
		
		//	SHIFTキー押されてて且つ「Tab」キーが押されてる？
		if (( cShift & 0x80 ) && ( nChar == VK_TAB )){
			
			//	参照型で分岐
			switch( m_nType ){
			//	金融機関
			case ID_DLGTYPE_BANK:				
				//	オフセットが3？
				if ( index == 3 ){
					GetDlgItem( ID_SEARCH )->SetFocus();	//	銀行検索にフォーカス移動
				}
				//	オフセットが3以外
				else{
					m_diagInput.SetPrevPosition();			//	前のテキストにフォーカス移動
				}
				break;

			//	取引先
			case ID_DLGTYPE_ADDRESS:

				//	オフセットが8？
				if ( index == 8 ){
					GetDlgItem( ID_SEARCH )->SetFocus();	//	住所検索にフォーカス移動
				}
				//	オフセットが7以外
				else{
					m_diagInput.SetPrevPosition();			//	前のテキストにフォーカス移動
				}
				break;

			//	科目
			case ID_DLGTYPE_KAMOKU:
				if( index == 18 ){
					m_diagInput.SetPrevPosition();			//	前のテキストにフォーカス移動
				}
				else{
					GetDlgItem( IDCANCEL )->SetFocus();			//	キャンセルボタンにフォーカス移動
				}
				break;
			}
			
		}
		//	SHIFTキー押されてない又は「Enter」キー押された
		else{

			//	参照型で分岐
			switch( m_nType ){
			//	金融機関
			case ID_DLGTYPE_BANK:
				
				//	オフセットが5？
				if ( index == 20 ){
					GetDlgItem( ID_OK_BUTTON )->SetFocus();			//	OKボタンにフォーカス移動
				}
				else{
					m_diagInput.SetNextPosition();			//	次のテキストにフォーカス移動
				}
				break;
			//	取引先
			case ID_DLGTYPE_ADDRESS:

				//	オフセットが12？
				if ( index == 16 ){
					GetDlgItem( ID_OK_BUTTON )->SetFocus();			//	OKボタンにフォーカス移動
				}
				else{
					m_diagInput.SetNextPosition();			//	次のテキストにフォーカス移動
				}
				break;
			//	科目
			case ID_DLGTYPE_KAMOKU:
				if( index == 18 ){
					GetDlgItem( ID_OK_BUTTON )->SetFocus();				//	OKボタンにフォーカス移動
				}
				else{
					m_diagInput.SetNextPosition();			//	次のテキストにフォーカス移動
				}				
				break;
			}
		}
		break;

	//	「Delete」キーが押された
	case VK_DELETE:
		//	データクリア
		//switch( m_nType ){
		//	case ID_DLGTYPE_BANK:	// 金融機関
		//		if( index == 3)			m_diagInput.DataClear( 14, TRUE ); 
		//		else if( index == 5 )	m_diagInput.DataClear( 20, TRUE ); 
		//		break;
		//	case ID_DLGTYPE_ADDRESS:	// 取引先
		//		m_diagInput.DataClear( 16, TRUE ); 
		//		break;
		//	case ID_DLGTYPE_KAMOKU:		// 科目
		//		m_diagInput.DataClear( 18, TRUE ); 
		//		break;
		//}
		m_diagInput.DataClear( index, TRUE ); 
		break;

	//	その他
	default:
		break;
	}
}

//**************************************************
//	背景色の変更
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::DiagSetBackColor( short nIndex )
{
	DIAGRAM_ATTRIBUTE	udAtt;		//	属性構造体
	
	//	初期化
	InitAttr( &udAtt );
	//	属性を取得
	m_diagInput.GetAttribute( nIndex, ( LPUNKNOWN )&udAtt );
	//	背景色を変更
	udAtt.attr_bcolor = m_ViewColor;
	//	属性を設定
	m_diagInput.SetAttribute( nIndex, ( LPUNKNOWN )&udAtt, TRUE );
}

//**************************************************
//	表示データセット
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::DiagSetDataDisp( short nIndex, CString szTitle )
{
	DIAGRAM_DATA		udData;		//	データ構造体
	
	//	初期化
	m_clsFunc.DiagInit( &udData );
	//	データ取得
	m_diagInput.GetData( nIndex, ( LPUNKNOWN )&udData );
	//	表示データセット
	udData.data_disp = szTitle;
	//	データ設定
	m_diagInput.SetData( nIndex, ( LPUNKNOWN )&udData );

}

//**************************************************
//	セルを非表示
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::DiagSetEnabled( short nIndex )
{	
	//	入力不可に設定（※非表示にしても入力受付してしまうため）
	m_diagInput.ModifyItem( nIndex, DIAG_MDFY_READONLY );
	//	非表示に設定
	m_diagInput.ModifyItem( nIndex, DIAG_MDFY_TRANSPARENT );
}

//**************************************************
//	入力データセット
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::DiagSetDataEdit( short nIndex, CString szData )
{
	DIAGRAM_DATA		udData;		//	データ構造体
	
	//	初期化
	m_clsFunc.DiagInit( &udData );
	//	データ取得
	m_diagInput.GetData( nIndex, ( LPUNKNOWN )&udData );
	//	入力データセット
	udData.data_edit = szData;
	//	データ設定
	m_diagInput.SetData( nIndex, ( LPUNKNOWN )&udData );
}

/*
CUcFunctionCommon::DiagGetString()へ移行
//**************************************************
//	入力データ取得
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	入力データ文字列
//**************************************************
CString CdlgReferenceSub_Old::DiagGetDataEdit( short nIndex )
{
	DIAGRAM_DATA		udData;		//	データ構造体
	
	//	初期化
	m_clsFunc.DiagInit( &udData );
	//	データ取得
	m_diagInput.GetData( nIndex, ( LPUNKNOWN )&udData );
	//	戻値に入力データを返す
	return( udData.data_edit );
}
*/

//**************************************************
//	最大入力文字数設定（ANK文字の入力有効も同時に行う）
//	【引数】	nIndex		…	セルインデックス
//				nMax		…	最大入力文字数
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::DiagSetInputMax( short nIndex, int nMax )
{
	DIAGRAM_ATTRIBUTE	udAtt;		//	属性構造体
	
	//	初期化
	InitAttr( &udAtt );
	//	属性を取得
	m_diagInput.GetAttribute( nIndex, ( LPUNKNOWN )&udAtt );
	//	文字列数
	udAtt.attr_inpmax = nMax;
	// 属性にANK文字の入力有効指定
	udAtt.attr_editattr = DIAG_EATTR_ZENHAN;
	//	属性を設定
	m_diagInput.SetAttribute( nIndex, ( LPUNKNOWN )&udAtt, TRUE );
}

//**************************************************
//	ディスプレイ初期化
//	【引数】	nType			…	参照型
//				szTitle			…	ダイアログタイトル
//				szOrder			…	順序番号
//				szName1			…	名称1
//				szName2			…	名称2
//				szAdd1			…	住所1
//				szAdd2			…	住所2
//				szKana1			…　ｶﾅ文字1
//				szKana2			…　ｶﾅ文字2
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::InitDisplay( EnumIdDlgType nType, CString szTitle,
								    CString szOrder, CString szName1,
								    CString szName2, CString szAdd1,
									CString szAdd2, CString szKana1,
									CString szKana2)
{
	int		nCnt;	//	カウント用
	
	//	ダイアログのタイトルを設定
	this->SetWindowText( szTitle );

	// 背景色を指定
	m_diagInput.SetBackColor( m_ViewColor );

	DiagSetBackColor( 0 );								//	背景色を変更
	DiagSetDataDisp( 0, "番号　　　　：" );				//	番号ラベル表示
	DiagSetBackColor( 1 );								//	順序番号の背景色変更
	DiagSetDataDisp( 1, szOrder );						//	順序番号データセット

	//	参照型で分岐
	switch( m_nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:		
		
		//	セル6～12を非表示に設定
		for( nCnt = 6; nCnt <= 12; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		//	セル15～18を非表示に設定
		for( nCnt = 15; nCnt <= 18; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		
		DiagSetBackColor( 2 );							//	背景色を変更
		DiagSetDataDisp( 2, "金融機関名　：" );			//	金融機関名ラベル表示
		DiagSetDataEdit( 3, szName1 );					//	金融機関名データセット
		DiagSetInputMax( 3, 10 );						//	金融機関名の入力文字数設定
		DiagSetBackColor( 4 );							//	背景色を変更
		DiagSetDataDisp( 4, "支店名　　　：" );			//	支店名ラベル表示
		DiagSetDataEdit( 5, szName2 );					//	支店名データセット
		DiagSetInputMax( 5, 10 );						//	支店名の入力文字数設定
		DiagSetBackColor( 13 );							//	背景色を変更
		DiagSetDataDisp( 13, "金融機関ｶﾅ名：" );		//	ｶﾅ名ラベル表示
		DiagSetDataEdit( 14, szKana1 );					//	金融機関ｶﾅ名データセット
		DiagSetInputMax( 14, 6 );						//	ｶﾅ名の入力文字数設定
		DiagSetBackColor( 19 );							//	背景色を変更
		DiagSetDataDisp( 19, "支店名ｶﾅ名　：" );		//	ｶﾅ名ラベル表示
		DiagSetDataEdit( 20, szKana2 );					//	支店名ｶﾅ名データセット
		DiagSetInputMax( 20, 6 );						//	ｶﾅ名の入力文字数設定
		m_diagInput.SetPosition( 3 );					//	初期フォーカス位置に移動
		m_btnSearch.ShowWindow( TRUE );					//	検索ボタン有効
		m_btnSearch.SetWindowText( "銀行検索[F7]" );	//	検索ボタンに名称設定
		break;

	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	セル2～5を非表示に設定
		for( nCnt = 2; nCnt <= 5; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		//　セル13,14を非表示に設定
		for( nCnt = 13; nCnt <= 14; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		//　セル17～20を非表示に設定
		for( nCnt = 17; nCnt <= 20; nCnt++ ){
			DiagSetEnabled( nCnt );
		}

		DiagSetBackColor( 6 );							//	背景色を変更
		DiagSetDataDisp( 6, "名称　　　　：" );			//	名称ラベル表示
		DiagSetDataEdit( 8, m_clsFunc.StrDocking( szName1, szName2 ) );	//	名称データセット
		DiagSetInputMax( 8, 20 );							//	名称の入力文字数設定
		DiagSetBackColor( 9 );							//	背景色を変更
		DiagSetDataDisp( 9, "所在地　　　：" );			//	所在地ラベル表示
		DiagSetDataEdit( 11, szAdd1 );					//	所在地１データセット
		DiagSetInputMax( 11, 20 );						//	所在地１の入力文字数設定
		DiagSetDataEdit( 12, szAdd2 );					//	所在地２データセット
		DiagSetInputMax( 12, 20 );						//	所在地２の入力文字数設定
		DiagSetBackColor( 15 );							//	背景色を変更
		DiagSetDataDisp( 15, "ｶﾅ名　　　　：" );		//	ｶﾅ名ラベル表示
		DiagSetDataEdit( 16, szKana1 );					//	ｶﾅ名データセット
		DiagSetInputMax( 16, 6 );						//	ｶﾅ名の入力文字数設定
		m_diagInput.SetPosition( 8 );					//	初期フォーカス位置に移動
		m_btnSearch.ShowWindow( TRUE );					//	検索ボタン有効
		m_btnSearch.SetWindowText( "住所検索[HOME]" );	//	検索ボタンに名称設定
		break;

	//	科目
	case ID_DLGTYPE_KAMOKU:
		
		//	セル4～9を非表示に設定
		for( nCnt = 4; nCnt <= 12; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		//　セル13～16を非表示に設定
		for( nCnt = 13; nCnt <= 16; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		//　セル19,20を非表示に設定
		for( nCnt = 19; nCnt <= 20; nCnt++ ){
			DiagSetEnabled( nCnt );
		}
		
		DiagSetBackColor( 2 );							//	背景色を変更
		DiagSetDataDisp( 2, "科目　　　　：" );			//	名称ラベル表示
		DiagSetDataEdit( 3, szName1 );					//	名称データセット
		DiagSetInputMax( 3, m_nInputLen );				//	名称の入力文字数設定
		DiagSetBackColor( 17 );							//	背景色を変更
		DiagSetDataDisp( 17, "ｶﾅ名　　　　：" );		//	ｶﾅ名ラベル表示
		DiagSetDataEdit( 18, szKana1 );					//	金融機関ｶﾅ名データセット
		DiagSetInputMax( 18, 6 );						//	ｶﾅ名の入力文字数設定
		m_diagInput.SetPosition( 3 );					//	初期フォーカス位置に移動
		m_btnSearch.ShowWindow( FALSE );				//	検索ボタン無効
		break;
	}
}

//**************************************************
//	入力データチェック
//	【引数】	nType		…	参照型
//				szName1		…	名称1
//				szName2		…	名称2
//				szAdd1		…	住所1
//				szAdd2		…	住所2
//				szKana1		…	カナ1
//				szKana2		…	カナ2
//				nGrSeq		…	グループ番号
//				nSeq		…	シーケンス番号
//	【戻値】	0			…	編集なし
//				1			…	編集あり（重複なし）
//				2			…	編集あり（重複あり）
//				3			…	入力なし
//**************************************************
// 改良No.22-0404,22-0812 del -->
//int CdlgReferenceSub_Old::CheckInputData( EnumIdDlgType nType, CString szName1, 
//									  CString szName2, CString szAdd1,
//									  CString szAdd2, CString szKana1,
//									  CString szKana2, int nSeq )
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
int CdlgReferenceSub_Old::CheckInputData(EnumIdDlgType nType,
									 CString szName1, CString szName2,
									 CString szAdd1, CString szAdd2,
									 CString szKana1, CString szKana2, int nGrSeq, int nSeq)
// 改良No.22-0404,22-0812 add <--
{
	int			nRet = 0;	//	戻値
	int			nCnt;		//	カウント用
	CString		szInput[5];	//	入力文字列
// No.200601 add -->
	size_t		len=0;
	CString		msg=_T("");
// No.200601 add <--
// 修正No.157628 add -->
	CString		cst=_T("");
// 修正No.157628 add <--

	// 初期化
	for(nCnt=0; nCnt<5; nCnt++)	{
		szInput[nCnt].Empty();
	}

	CStringArray	strArray;	// 文字列分割した場合のバッファ

	// 参照型で分岐
	switch( m_nType )	{
		//	金融機関
		case ID_DLGTYPE_BANK:
			szInput[0] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 3 ) );	//	データ取得
			szInput[1] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 5 ) );	//	データ取得
			szInput[2] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 14 ) );	//	データ取得
			szInput[3] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 20 ) );	//	データ取得
		
// No.200601 del -->
			////	入力なし？
			//if (( szInput[0] == "" ) && ( szInput[1] == "" )){
			//	nRet = 3;
			//}
			////	入力内容と登録内容が変わった？
			//else if (( szInput[0].Compare( szName1 ) != 0 ) || 
			//         ( szInput[1].Compare( szName2 ) != 0 ) ||
			//		 ( szInput[2].Compare( szKana1 ) != 0 ) ||
			//		 ( szInput[3].Compare( szKana2 ) != 0 )){
			//	
			//	//	重複してる？
			//	if ( CheckRepeatBank( szInput[0], szInput[1], szInput[2], szInput[3] ) == TRUE ){
			//		nRet = 2;
			//	}
			//	else{
			//		m_RetName1 = szInput[0];	//	返却名称1設定
			//		m_RetName2 = szInput[1];	//	返却名称2設定
			//		m_RetKana1 = szInput[2];	//　返却ｶﾅ1設定
			//		m_RetKana2 = szInput[3];	//　返却ｶﾅ2設定
			//		nRet = 1;					//	戻値を編集ありに設定
			//	}
			//}
// No.200601 del <--
// No.200601 add -->
			// 入力なし？
			if((szInput[0] == "") && (szInput[1] == ""))	{
				nRet = 3;
				break;
			}
			// 入力内容と登録内容が変わった？
			if( (szInput[0].Compare( szName1 ) != 0) || 
				(szInput[1].Compare( szName2 ) != 0) ||
				(szInput[2].Compare( szKana1 ) != 0) ||
				(szInput[3].Compare( szKana2 ) != 0) )	{
				// 重複してる？
				if(CheckRepeatBank(szInput[0], szInput[1], szInput[2], szInput[3] ) == TRUE)	{
					nRet = 2;
					break;
				}
			}

			if(bG_Kanso == TRUE)	{
				// 電子帳票の文字数チェックを行う
				// 銀行名
// 修正No.157628 del -->
				//len = _tcsclen(szInput[0].GetBuffer());
// 修正No.157628 del <--
// 修正No.157628 add -->
				cst.Empty();
				cst = szInput[0].GetBuffer();
				((CfrmUc000Common *)m_pOwner)->ReplaceHankakuKana(cst);
				len = _tcsclen(cst.GetBuffer());
// 修正No.157628 add <--
				if(len > UC_BANK_LEN)	{
					msg.Empty();
					msg.Format(_T("金融機関名が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"), UC_BANK_LEN);
					if(ICSMessageBox(msg, MB_YESNO|MB_ICONQUESTION) == IDNO)	{
						m_diagInput.SetPosition( 3 );	//	金融機関テキストに移動
						nRet = 4;
						break;
					}
				}
				else	{
					// 支店名
// 修正No.157628 del -->
					//len = _tcsclen(szInput[1].GetBuffer());
// 修正No.157628 del <--
// 修正No.157628 add -->
					cst.Empty();
					cst = szInput[1].GetBuffer();
					((CfrmUc000Common *)m_pOwner)->ReplaceHankakuKana(cst);
					len = _tcsclen(cst.GetBuffer());
// 修正No.157628 add <--
					if(len > UC_BANK_LEN)	{
						msg.Empty();
						msg.Format(_T("支店名が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"), UC_BANK_LEN);
						if(ICSMessageBox(msg, MB_YESNO|MB_ICONQUESTION) == IDNO)	{
							m_diagInput.SetPosition( 5 );	//	支店名テキストに移動
							nRet = 4;
							break;
						}
					}
				}
			}
			// 登録可能
			m_RetName1 = szInput[0];	//	返却名称1設定
			m_RetName2 = szInput[1];	//	返却名称2設定
			m_RetKana1 = szInput[2];	//　返却ｶﾅ1設定
			m_RetKana2 = szInput[3];	//　返却ｶﾅ2設定
			nRet = 1;					//	戻値を編集ありに設定
// No.200601 add <--
			break;

		//	取引先
		case ID_DLGTYPE_ADDRESS:
			m_clsFunc.StrDivision(m_clsFunc.DiagGetString(&m_diagInput, 8), &strArray, 2, TRUE, TRUE);
			szInput[0] = strArray.GetAt( 0 );
			szInput[1] = strArray.GetAt( 1 );
			szInput[2] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 11 ) );	//	データ取得
			szInput[3] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 12 ) );	//	データ取得
			szInput[4] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 16 ) );	//	データ取得
		
			//	入力なし？
			if(( szInput[0] == "" ) && ( szInput[1] == "" ) && 
				( szInput[2] == "" ) && ( szInput[3] == "" )){
				nRet = 3;
			}
			//	入力内容と登録内容が変わった？
			else if( ( szInput[0].Compare( szName1 ) != 0 ) || 
					 ( szInput[1].Compare( szName2 ) != 0 ) ||
					 ( szInput[2].Compare( szAdd1 ) != 0 ) ||
					 ( szInput[3].Compare( szAdd2 ) != 0 ) ||
					 ( szInput[4].Compare( szKana1 ) != 0 ) )	{
			
// 改良No.22-0404,22-0812 del -->
				////	重複してる？
				//if( CheckRepeatAddress( szInput[0], szInput[1], szInput[2], szInput[3], szInput[4], nSeq ) == TRUE )	{
				//	nRet = 2;
				//}
				//else{
				//	m_RetName1 = szInput[0];	//	返却名称1設定
				//	m_RetName2 = szInput[1];	//	返却名称2設定
				//	m_RetAdd1 = szInput[2];		//	返却住所1設定
				//	m_RetAdd2 = szInput[3];		//	返却住所2設定
				//	m_RetKana1 = szInput[4];	//	返却ｶﾅ1設定
				//	nRet = 1;					//	戻値を編集ありに設定
				//}
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
				// 重複してる？
				if(CheckRepeatAddress(szInput[0], szInput[1], szInput[2], szInput[3], szInput[4], nGrSeq) == TRUE)	{
					nRet = 2;
					break;
				}

				m_RetName1 = szInput[0];	//	返却名称1設定
				m_RetName2 = szInput[1];	//	返却名称2設定
				m_RetAdd1 = szInput[2];		//	返却住所1設定
				m_RetAdd2 = szInput[3];		//	返却住所2設定
				m_RetKana1 = szInput[4];	//	返却ｶﾅ1設定
				nRet = 1;					//	戻値を編集ありに設定
// 改良No.22-0404,22-0812 add <--
			}

			break;

		//	科目
		case ID_DLGTYPE_KAMOKU:		
			szInput[0] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 3 ) );	//	データ取得
			szInput[1] = m_clsFunc.DeleteRightSpace( m_clsFunc.DiagGetString( &m_diagInput, 18 ) );	//	データ取得
		
// No.200601 del -->
			////	入力なし？
			//if ( szInput[0] == "" ){
			//	nRet = 3;
			//}
			////	入力内容と登録内容が変わった？
			//else if (( szInput[0].Compare( szName1 ) != 0 ) ||
			//		 ( szInput[1].Compare( szKana1 ) != 0 )){
			//	
			//	//	重複してる？
			//	if ( CheckRepeatKamoku( szInput[0], szInput[1], nSeq ) == TRUE ){
			//		nRet = 2;
			//	}
			//	else{
			//		m_RetName1 = szInput[0];	//	返却名称1設定
			//		m_RetKana1 = szInput[1];	//　返却ｶﾅ名称1設定
			//		nRet = 1;					//	戻値を編集ありに設定
			//	}
			//}
// No.200601 del <--
// No.200601 add -->
			//	入力なし？
			if ( szInput[0] == "" ){
				nRet = 3;
				break;
			}
			//	入力内容と登録内容が変わった？
			if (( szInput[0].Compare( szName1 ) != 0 ) ||
				( szInput[1].Compare( szKana1 ) != 0 )){
				//	重複してる？
// 改良No.22-0404,22-0812 del -->
				//if ( CheckRepeatKamoku( szInput[0], szInput[1], nSeq ) == TRUE ){
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
				if(CheckRepeatKamoku(szInput[0], szInput[1], nGrSeq) == TRUE)	{
// 改良No.22-0404,22-0812 add <--
					nRet = 2;
					break;
				}
			}
			if(bG_Kanso == TRUE) {
// 改良No.22-0404,22-0812 del -->
//				//if(nSeq != ID_FORMNO_021 && nSeq != ID_FORMNO_111) {
//// 修正No.157897 del -->
//				//if(nSeq != ID_FORMNO_021 && nSeq != ID_FORMNO_111 && nSeq != ID_FORMNO_171 && nSeq != ID_FORMNO_181)	{
//// 修正No.157897 del <--
//// 修正No.157897 add -->
//				if(!(nSeq == ID_FORMNO_021 || nSeq == ID_FORMNO_111 || (nSeq >= ID_FORMNO_171 && nSeq <= ID_FORMNO_1720) || (nSeq >= ID_FORMNO_181 && nSeq <= ID_FORMNO_1820)))	{
//// 修正No.157897 add <--
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
				if(!(nGrSeq == ID_FORMNO_021 || nGrSeq == ID_FORMNO_111 || (nGrSeq >= ID_FORMNO_171 && nGrSeq <= ID_FORMNO_1720) || (nGrSeq >= ID_FORMNO_181 && nGrSeq <= ID_FORMNO_1820)))	{
// 改良No.22-0404,22-0812 add <--
// 修正No.157628 del -->
					//len = _tcsclen(szInput[0].GetBuffer());
// 修正No.157628 del <--
// 修正No.157628 add -->
					cst.Empty();
					cst = szInput[0].GetBuffer();
					((CfrmUc000Common *)m_pOwner)->ReplaceHankakuKana(cst);
					len = _tcsclen(cst.GetBuffer());
// 修正No.157628 add <--
					if(len > UC_KAMOKU_LEN) {
						msg.Empty();
// 修正No.157876 del -->
						//msg.Format(_T("科目名称が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"),UC_KAMOKU_LEN);
// 修正No.157876 del -->
// 修正No.157876 del -->
						msg.Format(_T("科目が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"),UC_KAMOKU_LEN);
// 修正No.157876 del -->
						if(ICSMessageBox(msg, MB_YESNO|MB_ICONQUESTION) == IDNO)	{
							m_diagInput.SetPosition( 3 );	//	科目名テキストに移動
							nRet = 4;
							break;
						}
					}
				}
			}
			// 登録可能
			m_RetName1 = szInput[0];	//	返却名称1設定
			m_RetKana1 = szInput[1];	//　返却ｶﾅ名称1設定
			nRet = 1;					//	戻値を編集ありに設定
// No.200601 add <--
			break;
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	銀行／住所検索ダイアログ表示
//	【引数】	nType	…	参照型
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::ShowSearchDialog( EnumIdDlgType nType )
{
	ICSBank				clsBank;		//	銀行クラス
	UCHAR				cBkCode[2];		//	銀行コード
	UCHAR				cBcCode[2];		//	支店コード
	CString				szBuf1;			//	バッファ１
	CString				szBuf2;			//	バッファ２
	CdlgAddressSearch	clsAdd( this );	//	住所検索クラス

	//	初期化
	szBuf1.Empty();
	szBuf2.Empty();

	// 参照型で分岐
	switch(nType)	{
	case ID_DLGTYPE_BANK:			// 金融機関
		// 初期化
		memset(cBkCode, 0xFF, sizeof(cBkCode));
		memset(cBcCode, 0xFF, sizeof(cBcCode));
		
		// 銀行検索ダイアログを呼び出しOKボタンで終了？
		if(clsBank.BankSelect(cBkCode, cBcCode, &szBuf1, &szBuf2, 0, this) == TRUE)	{
// 改良No.22-0404,22-0812 add -->
			// 銀行検索から名称を持ってくると、頭に半角スペースが入っている時があるので、頭のスペースをカット
			// （リリース時のみ現象が確認できた。デバックではなぜか現象出ない。）
			szBuf1.TrimLeft();
			szBuf2.TrimLeft();
// 改良No.22-0404,22-0812 add <--

			//	金融機関名セルにデータセット
			DiagSetDataEdit(3, szBuf1);
			//	支店名セルにデータセット
			DiagSetDataEdit(5, szBuf2);

// midori 161109 add -->
			// 銀行カナ、支店カナも取得
			BankKanaSelect(cBkCode, cBcCode, &szBuf1, &szBuf2);
			//	金融機関カナ名セルにデータセット
			DiagSetDataEdit(14, szBuf1);
			//	支店カナ名セルにデータセット
			DiagSetDataEdit(20, szBuf2);
// midori 161109 add <--
		}
		//	OKボタンにフォーカス移動
		m_btnOK.SetFocus();
		break;

	case ID_DLGTYPE_ADDRESS:		//	取引先
		// 住所検索ダイアログでOKが押された？
		if(clsAdd.ShowDialog(40, 40) == ID_DLG_OK)	{
			// 所在地セルにデータセット
			DiagSetDataEdit(11, clsAdd.m_AdAdd1);
			DiagSetDataEdit(12, clsAdd.m_AdAdd2);
			// 所在地セルにフォーカス移動
			if(clsAdd.m_AdAdd2 == "")	{
				// セットするセルは住所欄(上段)
				m_diagInput.SetPosition(11);
			}
			else	{
				// セットするセルは住所欄(下段)
				m_diagInput.SetPosition(12);
			}
		}
		break;
	}
}

/*
CUcFunctionCommon::DiagInit()へ移行
//**************************************************************
// DIAGRAM_DATA構造体初期化関数
//【引数】	*diadata	…	初期化構造体のハンドル
//【戻値】	なし
//***************************************************************
void CdlgReferenceSub_Old::InitDiag( DIAGRAM_DATA *diadata ){
	
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
*/
//**************************************************************
// DIAGRAM_ATTRIBUTE構造体初期化関数
//【引数】	*diadata	…	初期化構造体のハンドル
//【戻値】	なし
//***************************************************************
void CdlgReferenceSub_Old::InitAttr( DIAGRAM_ATTRIBUTE *diaatt ){

	// 初期化
	memset(&diaatt->attr_bcolor, '\0', sizeof(diaatt->attr_bcolor));
	memset(&diaatt->attr_fcolor, '\0', sizeof(diaatt->attr_fcolor));
	memset(&diaatt->attr_frcolor, '\0', sizeof(diaatt->attr_frcolor));
	diaatt->attr_ofstx		= 0;
	diaatt->attr_ofsty		= 0;
	diaatt->attr_align		= 0;
	diaatt->attr_linestyle	= 0;
	diaatt->attr_linewidth	= 0;
	memset(&diaatt->attr_lfcolor, '\0', sizeof(diaatt->attr_lfcolor));
	memset(&diaatt->attr_lbcolor, '\0', sizeof(diaatt->attr_lbcolor));
	diaatt->attr_rframe		= 0;
	diaatt->attr_rwidth		= 0;
	diaatt->attr_rheight	= 0;
	diaatt->attr_inpkind	= 0;
	diaatt->attr_inpmax		= 0;
	diaatt->attr_linemax	= 0;
	diaatt->attr_frctmax	= 0;
	diaatt->attr_editkind	= 0;
	diaatt->attr_editattr	= 0;
	diaatt->attr_editformat.Empty();
	diaatt->attr_dayattr	= 0;

}

//**************************************************
//	改行文字列削除
//	【引数】	szBuf	…	文字列
//	【戻値】	改行文字列削除文字列
//**************************************************
CString CdlgReferenceSub_Old::DelCrLfString( CString szBuf )
{
	CString		szRet;		//	戻値
	CString		szCrLf;		//	改行文字列
	int			nPos = -1;	//	改行位置
	int			nLen = -1;	//	文字列長
	int			nCrLf = -1;	//	改行の文字列長
	
	//	初期化
	szRet.Empty();	
	szCrLf.Empty();	
	//	改行文字列作成
	szCrLf = '\r';
	szCrLf += '\n';

	//	改行文字列検索
	nPos = szBuf.Find( szCrLf );
	
	//	改行あり？
	if ( nPos >= 0 ){
		nLen = szBuf.GetLength();							//	文字列長取得
		nCrLf = szCrLf.GetLength();							//	改行の文字列長取得
		szRet = szBuf.Left( nPos );							//	改行文字までの文字列取得
		szRet += szBuf.Right( nLen - ( nPos + nCrLf ) );	//	改行文字列以降の文字列取得
	}
	//	改行なし
	else{
		//	引数を戻値にセット
		szRet = szBuf;
	}
	
	//	戻値を返す
	return( szRet );
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgReferenceSub_Old::GetFocusControlID( CWnd* pParent /*=NULL*/)
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
//	重複確認（金融機関）
//	【引数】	szName1		…	銀行名
//				szName2		…	支店名
//				szKana2		…	銀行名ｶﾅ
//				szKana2		…	支店名ｶﾅ
//	【戻値】	TRUE		…	重複あり
//				FALSE		…	重複なし
//**************************************************
BOOL CdlgReferenceSub_Old::CheckRepeatBank( CString szName1, CString szName2, CString szKana1, CString szKana2 )
{
	BOOL				fRet = FALSE;		//	戻値
	CdbUcLstBank		mfcRec( m_pDB );	//	金融機関名称テーブル
	
	//	重複確認
	if ( mfcRec.CheckRepeat(  szName1, szName2, szKana1, szKana2 ) == DB_ERR_OK ){
		
		//	重複あり？
		if ( !mfcRec.IsEOF() ){
			//	重複あり！
			fRet = TRUE;
		}
		//	閉じる
		mfcRec.Fin();
	}
	
	//	戻値を返す
	return( fRet );
}

//**************************************************
//	重複確認（取引先）
//	【引数】	szName1		…	名称１
//				szName2		…	名称２
//				szAdd1		…	住所１
//				szAdd2		…	住所２
//				szKana		…	ｶﾅ１
//				nGrSeq		…	グループ番号
//	【戻値】	TRUE		…	重複あり
//				FALSE		…	重複なし
//**************************************************
BOOL CdlgReferenceSub_Old::CheckRepeatAddress( CString szName1, CString szName2, CString szAdd1, CString szAdd2, CString szKana, int nGrSeq )
{
	BOOL				fRet = FALSE;		//	戻値
	CdbUcLstAddress		mfcRec( m_pDB );	//	取引先名称テーブル

	//	重複確認
	if ( mfcRec.CheckRepeat(  szName1, szName2, szAdd1, szAdd2, nGrSeq, TRUE, szKana) == DB_ERR_OK ){
		
		//	重複あり？
		if ( !mfcRec.IsEOF() ){
			//	重複あり！
			fRet = TRUE;

			//	閉じる
			mfcRec.Fin();
			
			//	戻値を返す
			return( fRet );
		}
	}

	//	閉じる
	mfcRec.Fin();

	//	戻値を返す
	return( fRet );
}

//**************************************************
//	重複確認（科目）
//	【引数】	szName1		…	科目名称
//				szName2		…　科目ｶﾅ
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	TRUE		…	重複あり
//				FALSE		…	重複なし
//**************************************************
BOOL CdlgReferenceSub_Old::CheckRepeatKamoku( CString szName1, CString szName2, int nFormSeq )
{
	BOOL				fRet = FALSE;		//	戻値
	CdbUcLstKamoku		mfcRec( m_pDB );	//	科目名称テーブル
	
	//	重複確認
	if ( mfcRec.CheckRepeat(  nFormSeq, szName1, szName2 ) == DB_ERR_OK ){
		
		//	重複あり？
		if ( !mfcRec.IsEOF() ){
			//	重複あり！
			fRet = TRUE;
		}
		//	閉じる
		mfcRec.Fin();
	}
	
	//	戻値を返す
	return( fRet );
}

/*
CUcFunctionCommon::StrDivision()を使用する
//**************************************************
//	名称文字列取得
//	【引数】	szBuf		…	文字列
//				pszAdName1	…	名称１バッファ
//				pszAdName2	…	名称２バッファ
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub_Old::GetAdName( CString szBuf, CString* pszAdName1, CString* pszAdName2 )
{
	CString		szCrLf;		//	改行文字列
	int			nPos = -1;	//	改行位置
	int			nLen = -1;	//	文字列長
	int			nCrLf = -1;	//	改行の文字列長
	
	//	初期化
	szCrLf.Empty();	
	//	改行文字列作成
	szCrLf = '\r';
	szCrLf += '\n';

	//	改行文字列検索
	nPos = szBuf.Find( szCrLf );
	
	//	改行あり？
	if ( nPos >= 0 ){
		nLen = szBuf.GetLength();									//	文字列長取得
		nCrLf = szCrLf.GetLength();									//	改行の文字列長取得
		*pszAdName1 = szBuf.Left( nPos );							//	改行文字までの文字列取得
		*pszAdName2 = szBuf.Right( nLen - ( nPos + nCrLf ) );		//	改行文字列以降の文字列取得
	}
	//	改行なし
	else{
		//	引数を戻値にセット
		*pszAdName1 = DelCrLfString( szBuf );
	}
}
*/
/*
CUcFunctionCommon::StrDocking()を使用する
//**************************************************
//	名称文字列設定
//	【引数】	szAdName1	…	名称１
//				szAdName2	…	名称２
//	【戻値】	名称文字列
//**************************************************
CString CdlgReferenceSub_Old::SetAdName( CString szAdName1, CString szAdName2 )
{
	CString		szRet;

	//	初期化
	szRet.Empty();
	
	//	データあり？
	if (( szAdName1 != "" ) || ( szAdName2 != "" )){
		//	文字列を改行コードで連結
		szRet = szAdName1 + "\r\n" + szAdName2;
	}
	
	//	戻値
	return( szRet );
}
*/
//**************************************************
//	ダイアログ表示
//	【引数】	nType			…	参照型
//				nMode			…	動作モード（追加/挿入/編集）
//				szTitle			…	ダイアログタイトル
//				nOrder			…	順序番号
//				szName1			…	名称1
//				szName2			…	名称2
//				szAdd1			…	住所1
//				szAdd2			…	住所2
//				nGrSeq			…	グループ番号(取引先)
//				nSeq			…	シーケンス場合
//				nInputLen		…	入力文字数（※科目参照のみ）
//	【戻値】	ID_DLG_OK		…	OKボタン押された
//				ID_DLG_CANCEL	…	キャンセルボタン押された
//**************************************************
int CdlgReferenceSub_Old::ShowDialog( CDatabase* pDB, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/, 
								  EnumIdDlgRefMode nMode /*=ID_DLGREF_APPEND*/, 
								  CString szTitle /*=""*/, long nOrder /*=1*/, 
								  CString szName1 /*=""*/, CString szName2 /*=""*/, 
								  CString szAdd1 /*=""*/, CString szAdd2 /*=""*/,
								  CString szKana1 /*=""*/, CString szKana2/*=""*/,
// 改良No.22-0404,22-0812 del -->
								  //int nInputLen /*=DR_INPUTLENGTH_NORMAL*/, int nSeq /*=0*/ )
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
								  int nInputLen /*=DR_INPUTLENGTH_NORMAL*/, int nGrSeq /*=0*/, int nSeq /*=0*/ )
// 改良No.22-0404,22-0812 add <--
{
	char	szBuf[8];			//	バッファ

	//	バッファ初期化
	ZeroMemory( szBuf, sizeof( szBuf ) );
	//	順序番号を文字列に変換
	_itoa_s( nOrder, szBuf, 10 );	

	m_nRet = ID_DLG_CANCEL;		//	戻値を初期化
	m_RetName1.Empty();			//	返却名称1初期化
	m_RetName2.Empty();			//	返却名称2初期化
	m_RetAdd1.Empty();			//	返却住所1初期化
	m_RetAdd2.Empty();			//	返却住所2初期化
	m_RetKana1.Empty();			//  返却ｶﾅ1初期化
	m_RetKana2.Empty();			//  返却ｶﾅ2初期化

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){
		//	データベース開いてる？
		if ( pDB->IsOpen() ){
			m_pDB = pDB;
			m_nType = nType;			//	参照型取得
			m_nMode = nMode;			//	動作モード取得
			m_szTitle = szTitle;		//	ダイアログタイトル取得
			m_szOrder = szBuf;			//	順序番号取得
			m_szName1 = szName1;		//	名称1取得
			m_szName2 = szName2;		//	名称2取得
			m_szAdd1 = szAdd1;			//	住所1取得
			m_szAdd2 = szAdd2;			//	住所2取得
			m_szKana1 = szKana1;		//  ｶﾅ文字1取得
			m_szKana2 = szKana2;		//  ｶﾅ文字1取得
			m_nInputLen = nInputLen;	//	入力文字数（※科目参照ダイアログのみ）
// 改良No.22-0404,22-0812 del -->
			//m_nSeq = nSeq;			//	シーケンス番号
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
			m_nGrSeq = nGrSeq;			//	グループ番号
			m_nSeq = nSeq;				//	シーケンス番号
// 改良No.22-0404,22-0812 add <--
			DoModal();					//	モーダルで表示
		}
	}
	
	//	戻値を返す
	return( m_nRet );
}

void CdlgReferenceSub_Old::ReadStringIcsdiag8Kanasearch(short index, LPCTSTR kana)
{
	int nBaseIndex = 0;
	CString strKana;
	
	switch( m_nType ){
		case ID_DLGTYPE_BANK:	// 金融機関
			if( index == 3)			nBaseIndex = 14;
			else if( index == 5 )	nBaseIndex = 20;
			break;
		case ID_DLGTYPE_ADDRESS:	// 取引先
			nBaseIndex = 16;
			break;
		case ID_DLGTYPE_KAMOKU:		// 科目
			nBaseIndex = 18;
			break;
	}

	strKana = m_clsFunc.DiagGetString( &m_diagInput, nBaseIndex );
	if( !strKana.IsEmpty() )	return;

	CharController cc;
	cc.Divide( kana, strKana, 6/*文字*/ / 2 );
	switch( m_nType ){
		case ID_DLGTYPE_BANK:	// 金融機関
			if( index == 3)			DiagSetDataEdit( 14, strKana );		//	金融機関ｶﾅ名データセット
			else if( index == 5 )	DiagSetDataEdit( 20, strKana );		//	支店名ｶﾅ名データセット
			break;
		case ID_DLGTYPE_ADDRESS:	// 取引先
			DiagSetDataEdit( 16, strKana );		//	ｶﾅ名ラベル表示
			break;
		case ID_DLGTYPE_KAMOKU:		// 科目
			DiagSetDataEdit( 18, strKana );		//	ｶﾅ名ラベル表示
			break;
	}
}

// midori 161109 add -->
//**************************************************
// 銀行カナ、支店カナを取得
//**************************************************
void CdlgReferenceSub_Old::BankKanaSelect(UCHAR* pBkCode, UCHAR* pBcCode, CString* pBuf1, CString* pBuf2)
{
	int			ival=0;
	int			cnt=0,sw=0;
	int			bno=0,brnum=0,brofs=0;
	char		sdev[16],mdev[16];
	char		bf[128];
	UCHAR		bbf[128];
	UCHAR		cbf[128];
	CString		kana1=_T(""),kana2=_T("");
	ICSBank		clsBank;					//	銀行クラス
	_BANK_REC	brec;
	_BRAN_REC	brrec;

	memset(sdev,0,16);
	memset(mdev,0,16);
	GCOM_GetString(_T("SystemDrive"),sdev);
	PCOM_GetString(_T("MasterDevice"),mdev);
	if(clsBank.InitBankFile(sdev[0],mdev[0],TRUE) == FALSE)	{
		ICSExit(0,_T("銀行名称ファイルがオープンできません。"));
		return;
	}	
	clsBank.AccessBankFile(FALSE);

	// 銀行コード
	memset(bbf,0,128);
	memcpy(bbf,pBkCode,128);
	// 支店コード
	memset(cbf,0,128);
	memcpy(cbf,pBcCode,128);
	// 返送用変数をクリア
	pBuf1->Empty();
	pBuf2->Empty();

	bno = clsBank.GetBank(bbf);
	if(bno >= 0)	{
		// 銀行情報を取得
		brec = clsBank.m_BnkRec[bno];

		// 銀行カナ
		memset(bf,0,128);
		memcpy(bf,brec.BANK_KAN,6);
		pBuf1->Format(_T("%s"),bf);

		// 支店情報を取得
		brnum = clsBank.GetBranchNum(bbf);
		brofs = clsBank.GetBranch(bbf);
		sw = 0;
		for(cnt=brofs; cnt<brnum+brofs; cnt++)	{
			brrec = clsBank.m_BrnRec[cnt];
			if(((brrec.BRAN_CODE[0]&0xff) == (cbf[0]&0xff)) && ((brrec.BRAN_CODE[1]&0xff) == (cbf[1]&0xff)))	{
				sw = 1;
				break;
			}
		}
		if(sw == 1)	{
			memset(bf,0,128);
			memcpy(bf,brrec.BRAN_KAN,6);
			pBuf2->Format(_T("%s"),bf);
		}
	}
}
// midori 161109 add <--
