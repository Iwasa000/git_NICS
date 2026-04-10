//--------------------------------------------------
//	dlgDisplay.h
//
//	2006.02.20～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgdisplay.h"

IMPLEMENT_DYNAMIC(CdlgDisplay, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgDisplay::CdlgDisplay(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgDisplay::IDD, pParent)
{
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgDisplay::CdlgDisplay( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgDisplay::~CdlgDisplay()
{
	m_font.DeleteObject();
}

void CdlgDisplay::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISPLAY_LIST, m_lstDisplay);
	DDX_Control(pDX, ID_DELETE_BUTTON, m_btnDelete);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_HOSOKU_LABEL, m_lblHosoku);
	DDX_Control(pDX, ID_ALL_BUTTON_ON, m_btnAllOn);
	DDX_Control(pDX, ID_ALL_BUTTON_OFF, m_btnAllOff);
}

BEGIN_MESSAGE_MAP(CdlgDisplay, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgDisplay::OnBnClickedOk)
	ON_BN_CLICKED(ID_DELETE_BUTTON, &CdlgDisplay::OnBnClickedDeleteButton)
	ON_BN_CLICKED(ID_ALL_BUTTON_ON, &CdlgDisplay::OnBnClickedAllButton)
	ON_BN_CLICKED(ID_ALL_BUTTON_OFF, &CdlgDisplay::OnBnClickedAllButtonOff)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgDisplay, ICSDialog)
	ON_EVENT(CdlgDisplay, IDC_DISPLAY_LIST, 7, CdlgDisplay::LButtonDblClickDisplayList, VTS_I2)
	ON_EVENT(CdlgDisplay, IDC_DISPLAY_LIST, 6, CdlgDisplay::LButtonClickDisplayList, VTS_I2)
END_EVENTSINK_MAP()

void CdlgDisplay::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgDisplay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgDisplay::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	//	種別で分岐
	switch( m_nType ){
	//------------------------------
	//	一括金額
	//------------------------------
	case ID_FGSHOW_IKKATU:
		SetWindowText( "一括金額参照" );		//	ダイアログタイトル変更
		m_btnDelete.ShowWindow( FALSE );		//	削除ボタンを非表示
		break;
	//------------------------------
	//	一時保管
	//------------------------------
	case ID_FGSHOW_HOKAN:
		SetWindowText( "保管参照" );		//	ダイアログタイトル変更
		m_btnDelete.ShowWindow( TRUE );			//	削除ボタンを表示
		break;
	}

// midori 160612 cor -->
//	// 操作の補足メッセージを表示
//	VERIFY(m_font.CreatePointFont(105, "MS ｺﾞｼｯｸ"));
//	m_lblHosoku.SetFont(&m_font);		//	フォントサイズの設定
//	m_lblHosoku.UpdateCaption( _T("クリック又はスペースキーで選択/解除（複数選択可能）\n※選択データはグレーで表示"), 0 );
// ---------------------
	if(m_DataKakutei)	{
		// 入力確定済み
		m_lblHosoku.ShowWindow(SW_HIDE);
	}
	else	{
		// 操作の補足メッセージを表示
		VERIFY(m_font.CreatePointFont(105, "MS ｺﾞｼｯｸ"));
		m_lblHosoku.SetFont(&m_font);		//	フォントサイズの設定
		m_lblHosoku.UpdateCaption( _T("クリック又はスペースキーで選択/解除（複数選択可能）\n※選択データはグレーで表示"), 0 );
	}
// midori 160612 cor <--
	
	// リスト初期化
	InitList( m_nFormSeq, m_pDB, m_nType );

	// リサイズ
	OnInitDialogEX();

	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ウィンドウメッセージをフィルタリング
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgDisplay::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	int	nID;
	int	nIndex;

	//	フォーカスのあるコントロールのID取得
	nID = GetFocusControlID( this );
	
	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN ){

		//	パラメータで分岐
		switch( pMsg->wParam ){

		//----------
		//	Enter
		//----------
		case VK_RETURN:

			//	リストにフォーカスあり？
			if ( nID == IDC_DISPLAY_LIST ){	
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}
			break;

		//----------
		//	F2
		//----------
		case VK_F2:
			
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;

		//----------
		//	Esc
		//----------
		case VK_ESCAPE:
			
			//	リストにフォーカスあり？
			if ( nID == IDC_DISPLAY_LIST ){
				//	OnCancel起動
				PostMessage( WM_COMMAND, IDCANCEL );
				return( 1 );
			}
			break;

		//----------
		//	スペース
		//----------
		case VK_SPACE:
			
			//	リストにフォーカスあり？
			if ( nID == IDC_DISPLAY_LIST ){
				//	リストのインデックス取得
				nIndex = m_lstDisplay.GetIndex() + m_lstDisplay.GetTopIndex();
				//	リストシングルクリックイベント呼び出し
				LButtonClickDisplayList( nIndex );
			}
			break;
		}
	}
	
	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストシングルクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::LButtonClickDisplayList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	//	リストアイテムがクリックされた？
	if ( index != -1 ){
// midori 160612 cor -->
//		//	選択行を設定
//		SetSelectRow( index );
// ---------------------
		//	選択行を設定
		//	入力データを確定するにチェック無し
		if(m_DataKakutei == FALSE)	SetSelectRow( index );
// midori 160612 cor <--
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストダブルクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::LButtonDblClickDisplayList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（削除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::OnBnClickedDeleteButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	
	//	選択アイテムなし？
	if ( CheckSelectItem() == 0 ){
		ICSMessageBox( _T("リストが１件も選択されていません。"), MB_OK, 0, 0, this );
		return;
	}

	//	削除確認でキャンセル？
	if ( ICSMessageBox( _T("選択データを削除します。"), MB_OKCANCEL | MB_DEFBUTTON2, 0, 0, this ) == IDCANCEL ){
		//	抜ける
		return;
	}
	
	//	リストアイテムをチェック
	if ( CheckListItam( TRUE ) != 0 ){
		// リスト初期化
		InitList( m_nFormSeq, m_pDB, m_nType, FALSE );
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（戻すボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	選択アイテムなし？
	if ( CheckSelectItem() == 0 ){
		ICSMessageBox( _T("リストが１件も選択されていません。"), MB_OK, 0, 0, this );
		return;
	}

	//	転記確認でキャンセル？
	if ( ICSMessageBox( _T("選択データを転記します。"), MB_OKCANCEL, 0, 0, this ) == IDCANCEL ){
		//	抜ける
		return;
	}
	
	//	リストアイテムをチェック
	if ( CheckListItam() != 0 ){
		m_nRet = ID_DLG_BACK;	//	戻値を戻すに設定
	}

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（全選択ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::OnBnClickedAllButton()
{
	CString		szSelect;	//	選択値
	COLORREF	nColor;		//	色値

	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	for( int i=0; i<m_lstDisplay.GetCount(); i++){
		//szSelect = GetSelectData( i );

		//	選択オンに変更
		szSelect = DD_SELECT_ON;
		//	色を設定
		nColor = RGB( 127, 127, 127 );
		//	選択データを設定
		SetSelectData( i, szSelect );
		//	色を設定
		SetColor( i, nColor );
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（全解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::OnBnClickedAllButtonOff()
{
	CString		szSelect;	//	選択値
	COLORREF	nColor;		//	色値

	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	for( int i=0; i<m_lstDisplay.GetCount(); i++){
		//szSelect = GetSelectData( i );

		//	選択オンに変更
		szSelect = DD_SELECT_OFF;
		//	色を設定
		nColor = GetSysColor( COLOR_WINDOW );
		//	選択データを設定
		SetSelectData( i, szSelect );
		//	色を設定
		SetColor( i, nColor );
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgDisplay::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//**************************************************
// リスト初期化
//	【引数】	nFormSeq	…	様式シーケンス番号
//				pDB			…	データベースハンドル
//				nType		…	種別
//	【戻値】	なし
//**************************************************
void CdlgDisplay::InitList( short nFormSeq, CDatabase* pDB, EnumIdFgShow nType, BOOL bHeader /*=TRUE*/ )
{
	int nRet = 0;	//	戻値

	//	様式で分岐
	switch( nFormSeq ){
	//	①.預貯金等
	case ID_FORMNO_011:
		m_nColumnMax = 7;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki011Column( m_nColumnMax );					//	様式011カラム設定
		}

		nRet = SetYoushiki011Data( pDB, nType, m_nColumnMax );	//	様式011データ設定
		break;
	//	②.受取手形
	case ID_FORMNO_021:
		m_nColumnMax = 10;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki021Column( m_nColumnMax );					//	様式021カラム設定
		}

		nRet = SetYoushiki021Data( pDB, nType, m_nColumnMax );	//	様式021データ設定
		break;
	//	③.売掛金
	case ID_FORMNO_031:
		m_nColumnMax = 7;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki031Column( m_nColumnMax );					//	様式031カラム設定
		}

		nRet = SetYoushiki031Data( pDB, nType, m_nColumnMax );	//	様式031データ設定
		break;
	//	④－１.仮払金
	case ID_FORMNO_041:
		m_nColumnMax = 8;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki041Column( m_nColumnMax );					//	様式041カラム設定
		}

		nRet = SetYoushiki041Data( pDB, nType, m_nColumnMax );	//	様式041データ設定
		break;
	//	④－２.貸付金
	case ID_FORMNO_042:
		m_nColumnMax = 10;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki042Column( m_nColumnMax );					//	様式042カラム設定
		}

		nRet = SetYoushiki042Data( pDB, nType, m_nColumnMax );	//	様式042データ設定
		break;
	//	⑤.棚卸資産
	case ID_FORMNO_051:
		m_nColumnMax = 9;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki051Column( m_nColumnMax );					//	様式051カラム設定
		}

		nRet = SetYoushiki051Data( pDB, nType, m_nColumnMax );	//	様式051データ設定
		break;
	//	⑥.有価証券
	case ID_FORMNO_061:
		m_nColumnMax = 16;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki061Column( m_nColumnMax );					//	様式051カラム設定
		}

		nRet = SetYoushiki061Data( pDB, nType, m_nColumnMax );	//	様式051データ設定
		break;
	//	⑦.固定資産
	case ID_FORMNO_071:
		m_nColumnMax = 14;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki071Column( m_nColumnMax );					//	様式071カラム設定
		}

		nRet = SetYoushiki071Data( pDB, nType, m_nColumnMax );	//	様式071データ設定
		break;
	//	⑧.支払手形
	case ID_FORMNO_081:
		m_nColumnMax = 8;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki081Column( m_nColumnMax );					//	様式081カラム設定
		}

		nRet = SetYoushiki081Data( pDB, nType, m_nColumnMax );	//	様式081データ設定
		break;
	//	⑨.買掛金
	case ID_FORMNO_091:
		m_nColumnMax = 7;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki091Column( m_nColumnMax );					//	様式091カラム設定
		}

		nRet = SetYoushiki091Data( pDB, nType, m_nColumnMax );	//	様式091データ設定
		break;
	//	⑩ー１.仮受金
	case ID_FORMNO_101:
		m_nColumnMax = 8;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki101Column( m_nColumnMax );					//	様式101カラム設定
		}

		nRet = SetYoushiki101Data( pDB, nType, m_nColumnMax );	//	様式101データ設定
		break;
	//	⑩ー２.源泉取得税
	case ID_FORMNO_102:
		m_nColumnMax = 5;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki102Column( m_nColumnMax );					//	様式102カラム設定
		}

		nRet = SetYoushiki102Data( pDB, nType, m_nColumnMax );	//	様式102データ設定
		break;
	//	⑪.借入金
	case ID_FORMNO_111:
		m_nColumnMax = 11;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki111Column( m_nColumnMax );					//	様式111カラム設定
		}

		nRet = SetYoushiki111Data( pDB, nType, m_nColumnMax );	//	様式111データ設定
		break;
	//	⑫.土地の売上高等
	case ID_FORMNO_121:
		m_nColumnMax = 13;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki121Column( m_nColumnMax );					//	様式121カラム設定
		}

		nRet = SetYoushiki121Data( pDB, nType, m_nColumnMax );	//	様式121データ設定
		break;
	//	⑬.売上高等の事業所別
	case ID_FORMNO_131:
		m_nColumnMax = 13;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki131Column( m_nColumnMax );					//	様式131カラム設定
		}

		nRet = SetYoushiki131Data( pDB, nType, m_nColumnMax );	//	様式131データ設定
		break;
	//	⑭ー１.役員報酬手当金等
	case ID_FORMNO_141:
		m_nColumnMax = 15;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki141Column( m_nColumnMax );					//	様式141カラム設定
		}

		nRet = SetYoushiki141Data( pDB, nType, m_nColumnMax );	//	様式141データ設定
		break;
	//	⑭ー２.人件費
	case ID_FORMNO_142:
		//	基本的には呼ばれないんで何もしない。
		break;
	//	⑮ー１.地代家賃等
	case ID_FORMNO_151:
		m_nColumnMax = 10;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki151Column( m_nColumnMax );					//	様式151カラム設定
		}

		nRet = SetYoushiki151Data( pDB, nType, m_nColumnMax );	//	様式151データ設定
		break;
	//	⑮ー２.権利金等の期中支払
	case ID_FORMNO_152:
		m_nColumnMax = 8;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki152Column( m_nColumnMax );					//	様式152カラム設定
		}

		nRet = SetYoushiki152Data( pDB, nType, m_nColumnMax );	//	様式152データ設定
		break;
	//	⑮ー３.工業所有権等の使用料
	case ID_FORMNO_153:
		m_nColumnMax = 9;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki153Column( m_nColumnMax );					//	様式153カラム設定
		}

		nRet = SetYoushiki153Data( pDB, nType, m_nColumnMax );	//	様式153データ設定
		break;
	//	⑯ー１.雑役
	case ID_FORMNO_161:
		m_nColumnMax = 7;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki16Column( m_nColumnMax );					//	様式161カラム設定
		}

		nRet = SetYoushiki161Data( pDB, nType, m_nColumnMax );	//	様式161データ設定
		break;
	//	⑯ー２.雑損失
	case ID_FORMNO_162:
		m_nColumnMax = 7;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki16Column( m_nColumnMax );					//	様式162カラム設定
		}

		nRet = SetYoushiki162Data( pDB, nType, m_nColumnMax );	//	様式162データ設定
		break;
	//	⑰.その他科目の内訳書①
	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:	
	case ID_FORMNO_177:	
	case ID_FORMNO_178:	
	case ID_FORMNO_179:	
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:

		m_nColumnMax = 7;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki171Column( m_nColumnMax );					//	様式171カラム設定
		}

		nRet = SetYoushiki171Data( pDB, nType, m_nColumnMax );	//	様式171データ設定
		break;
	//	⑱.その他科目の内訳書②
	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:

		m_nColumnMax = 6;										//	カラム最大値

		//	ヘッダ設定する？
		if ( bHeader == TRUE ){
			SetYoushiki181Column( m_nColumnMax );					//	様式181カラム設定
		}

		nRet = SetYoushiki181Data( pDB, nType, m_nColumnMax );	//	様式181データ設定
		break;
	}	
	
	//	戻値が0行？
	if ( nRet == 0 ){
		m_btnDelete.EnableWindow( FALSE );		//	全て戻すボタン無効
		m_btnOK.EnableWindow( FALSE );			//	戻すボタン無効
		m_btnAllOn.EnableWindow( FALSE );		//　全選択ボタン無効
		m_btnAllOff.EnableWindow( FALSE );		//　全解除ボタン無効
		m_lstDisplay.EnableWindow( FALSE );		//	リストを無効
		m_btnCancel.SetFocus();					//	閉じるボタンにフォーカス移動 
	}
	else{
// midori 160612 cor -->
//		m_btnDelete.EnableWindow( TRUE );		//	全て戻すボタン有効
//		m_btnOK.EnableWindow( TRUE );			//	戻すボタン有効
//		m_btnAllOff.EnableWindow( TRUE );		//　全解除ボタン有効
//		m_btnAllOn.EnableWindow( TRUE );		//　全選択ボタン有効
// ---------------------
		if(m_DataKakutei)	{
			m_btnDelete.EnableWindow( FALSE );		//	全て戻すボタン無効
			m_btnOK.EnableWindow( FALSE );			//	戻すボタン無効
			m_btnAllOff.EnableWindow( FALSE );		//　全解除ボタン無効
			m_btnAllOn.EnableWindow( FALSE );		//　全選択ボタン無効
		}
		else				{
			m_btnDelete.EnableWindow( TRUE );		//	全て戻すボタン有効
			m_btnOK.EnableWindow( TRUE );			//	戻すボタン有効
			m_btnAllOff.EnableWindow( TRUE );		//　全解除ボタン有効
			m_btnAllOn.EnableWindow( TRUE );		//　全選択ボタン無効
		}
// midori 160612 cor <--
		m_lstDisplay.EnableWindow( TRUE );		//	リストを有効
	}
}

//**************************************************
// リスト設定
//	【引数】	nColumnMax	…	最大カラム数
//				szTitle		…	カラムタイトル
//				nWidth		…	カラム幅
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetList( int nColumnMax, CString szTitle[], int nWidth[] )
{
	int					nCnt;			//	カウント用
	LINP_COLUMN			udColumn[32];	//	カラム構造体
	LINP_VCOLORINFO		udGrid[32];		//	縦グリッド色構造体

	//	最大カラム数分ループ
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		udColumn[nCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
		udColumn[nCnt].clm_max = 40;														//	入力最大桁数
		udColumn[nCnt].clm_attr = LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;	//	属性
		udColumn[nCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );								//	背景色
		udColumn[nCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );								//	文字色
		udColumn[nCnt].clm_width = nWidth[nCnt];											//	幅（DLU）
		udColumn[nCnt].clm_title = szTitle[nCnt];											//	タイトル

		udGrid[nCnt].pos = nCnt;						//	縦グリッド線を引くカラムのオフセット
		udGrid[nCnt].color = RGB( 0xFF, 0x00, 0x00 );	//	色
		udGrid[nCnt].type = LINP_GRID_SOLID;			//	線種
	}

	//	カラムの設定（ID、カラム数、カラム構造体、クラス名）
	m_lstDisplay.SetColumn( nColumnMax, ( LPUNKNOWN )udColumn );
	//	横グリッド線設定（ID、横グリッド線間隔、色、線種、クラス名）
	m_lstDisplay.SetHorzGrid( 5, RGB( 0x00, 0xFF, 0xFF), LINP_GRID_SOLID );
	//	縦グリッド線設定（ID、縦グリッド線数、グリッド構造体、クラス名）
	m_lstDisplay.SetVartGrid( nColumnMax, ( LPUNKNOWN )udGrid );

}

//**************************************************
// 様式011カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki011Column( int nColumnMax )
{
	CString		szTitle[7];		//	カラムタイトル
	int			nWidth[7];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "金融機関名";
	szTitle[3]= "種類";
	szTitle[4]= "口座番号";
	szTitle[5]= "期末現在高";
	szTitle[6]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 21 * 8 ) + 8; 
	nWidth[3] = ( 8 * 8 ) + 8;
	nWidth[4] = ( 14 * 4 ) + 4;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 40 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式011データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki011Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[7];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();					//	リストの全アイテム消去
	CdbUc011Yotyokin	mfcRec( pDB );			//	uc_011_yotyokinテーブルクラス

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					//	支店名あり？
					if ( !mfcRec.m_BkName2.IsEmpty() ){			
						udData[2].dat_string.Format( "%s／%s", mfcRec.m_BkName1, mfcRec.m_BkName2 );
					}
					else{
						udData[2].dat_string = mfcRec.m_BkName1;				//	金融機関名設定
					}
					
					udData[3].dat_string = mfcRec.m_KnName;					//	種類設定
					udData[4].dat_string = mfcRec.m_AcNum;					//	口座番号設定
					udData[5].dat_string = GetFormatValue( mfcRec.m_Val );	//	期末現在高設定
					udData[6].dat_string = mfcRec.m_Teki;					//	適用設定
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式021カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki021Column( int nColumnMax )
{
	CString		szTitle[10];		//	カラムタイトル
	int			nWidth[10];			//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "振出人";
	szTitle[4]= "振出年月日";
	szTitle[5]= "支払期日";
	szTitle[6]= "支払銀行名";
	szTitle[7]= "金額";
	szTitle[8]= "割引銀行名";
	szTitle[9]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 12 * 8 ) + 8;
	nWidth[3] = ( 20 * 8 ) + 8; 
	nWidth[4] = ( 10 * 4 ) + 4;
	nWidth[5] = ( 8 * 4 ) + 4;
	nWidth[6] = ( 21 * 8 ) + 8;
	nWidth[7] = ( 14 * 4 ) + 4;
	nWidth[8] = ( 21 * 8 ) + 8;
	nWidth[9] = ( 32 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式021データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki021Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[10];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc021Uketoritegata	mfcRec( pDB );	//	uc_021_Uketoritegataテーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	振出人

					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate );							//	日付変換
						udData[4].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );	//	振出年月日
					}
					else{
						udData[4].dat_string.Empty(); 
					}
						
					if ( mfcRec.m_PmDate > 0 ){
						clsDate.Convert( mfcRec.m_PmDate );							//	日付変換
						udData[5].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );	//	支払期日
					}
					else{
						udData[5].dat_string.Empty(); 
					}
					
					//	支店名あり？
					if ( !mfcRec.m_BkName2.IsEmpty() ){			
						udData[6].dat_string.Format( "%s／%s", mfcRec.m_BkName1, mfcRec.m_BkName2 );	//	支払銀行名
					}
					else{
						udData[6].dat_string = mfcRec.m_BkName1;										//	支払銀行名
					}

					udData[7].dat_string = GetFormatValue( mfcRec.m_Val );		//	金額設定
					
					//	支店名あり？
					if ( !mfcRec.m_Bk2Name2.IsEmpty() ){			
						udData[8].dat_string.Format( "%s／%s", mfcRec.m_Bk2Name1, mfcRec.m_Bk2Name2 );	//	支払銀行名
					}
					else{
						udData[8].dat_string = mfcRec.m_Bk2Name1;										//	支払銀行名
					}

					udData[9].dat_string = mfcRec.m_Teki;						//	適用設定

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式031カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki031Column( int nColumnMax )
{
	CString		szTitle[7];		//	カラムタイトル
	int			nWidth[7];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "相手先（名称）";
	szTitle[4]= "相手先（所在地）";
	szTitle[5]= "期末現在高";
	szTitle[6]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 26 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式031データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki031Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[9];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc031Urikakekin		mfcRec( pDB );	//	uc_031_Urikakekinテーブルクラス

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;						//	科目 
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	名称
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;	//	住所
					udData[5].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[6].dat_string = mfcRec.m_Teki;						//	適用

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式041カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki041Column( int nColumnMax )
{
	CString		szTitle[8];		//	カラムタイトル
	int			nWidth[8];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "相手先（名称）";
	szTitle[4]= "相手先（所在地）";
	szTitle[5]= "相手先（関係）";
	szTitle[6]= "期末現在高";
	szTitle[7]= "取引の内容";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 10 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4;
	nWidth[7] = ( 20 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式041データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki041Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[8];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc041Karibarai		mfcRec( pDB );	//	uc_041_Karibaraiテーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;						//	科目 
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	名称
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;	//	住所
					udData[5].dat_string = mfcRec.m_Kankei;						//	法人・代表者との関係
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[7].dat_string = mfcRec.m_Naiyou;						//	取引の内容

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式042カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki042Column( int nColumnMax )
{
	CString		szTitle[10];	//	カラムタイトル
	int			nWidth[10];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "貸付先";
	szTitle[3]= "法人・代表者との関係";
	szTitle[4]= "所在地";
	szTitle[5]= "期末現在高";
	szTitle[6]= "期中の受取利息額";
	szTitle[7]= "利率";
	szTitle[8]= "貸付理由";
	szTitle[9]= "担保の内容";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 20 * 8 ) + 8; 
	nWidth[3] = ( 10 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 16 * 4 ) + 4;
	nWidth[7] = ( 14 * 4 ) + 4;
	nWidth[8] = ( 24 * 8 ) + 8;
	nWidth[9] = ( 30 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式042データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki042Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[10];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc042Kasituke		mfcRec( pDB );	//	uc_042_kasitukeテーブルクラス
	CdateConvert			clsDate;		//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	
					udData[3].dat_string = mfcRec.m_Kankei; 
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[5].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[6].dat_string = GetFormatValue( mfcRec.m_Risoku, "sssssssssssssss9" );
					udData[7].dat_string = GetFormatValue( mfcRec.m_Rate, "sssssssss9.999" );
					udData[8].dat_string = mfcRec.m_Riyuu; 
					udData[9].dat_string = mfcRec.m_Naiyou;						//	取引の内容

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式051カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki051Column( int nColumnMax )
{
	CString		szTitle[9];		//	カラムタイトル
	int			nWidth[9];		//	カラム幅
	
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "品目";
	szTitle[4]= "数量";
	szTitle[5]= "単位";
	szTitle[6]= "単価";
	szTitle[7]= "期末現在高";
	szTitle[8]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 14 * 4 ) + 4;
	nWidth[5] = ( 2 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4;
	nWidth[7] = ( 12 * 4 ) + 4;			//	※自動計算で桁あふれするケースがあり12桁以上は「************」と表示させるため
	nWidth[8] = ( 30 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式051データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki051Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[9];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();			//	リストの全アイテム消去
	CdbUc051Tanaoroshi	mfcRec( pDB );	//	uc_051_yotyokinテーブルクラス

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );												//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;											//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );											//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );											//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;													//	非表示カラムにシーケンス番号設定
					udData[2].dat_string = mfcRec.m_KnName;											//	科目
					udData[3].dat_string = mfcRec.m_HdName;											//	品目 
					udData[4].dat_string = GetFormatValue( mfcRec.m_Suuryou, "ssssssssss9.99" );	//	数量
					udData[5].dat_string = mfcRec.m_Tanni;											//	単位 
					udData[6].dat_string = GetFormatValue( mfcRec.m_Price, "ssssssssss9.99" );		//	単価
					udData[7].dat_string = GetFormatValue( mfcRec.m_Val , "sssssssssss9");			//	期末現在高
																									//	※自動計算で桁あふれするケースがあり12桁以上は「************」と表示させるため
					udData[8].dat_string = mfcRec.m_Teki;											//	摘要
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式061カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki061Column( int nColumnMax )
{
	CString		szTitle[16];	//	カラムタイトル
	int			nWidth[16];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "区分";
	szTitle[3]= "種類・銘柄";
	szTitle[4]= "期末現在高（数量）";
	szTitle[5]= "期末現在高（単位）";
	szTitle[6]= "期末現在高（上段）";
	szTitle[7]= "期末現在高（下段）";
	szTitle[8]= "異動年月日";
	szTitle[9]= "異動事由";
	szTitle[10]= "数量";
	szTitle[11]= "単位";
	szTitle[12]= "金額";
	szTitle[13]= "売却先（名称）";
	szTitle[14]= "売却先（所在地）";
	szTitle[15]= "摘要";
	
	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 16 * 8 ) + 8;
	nWidth[4] = ( 18 * 4 ) + 4;
	nWidth[5] = ( 9 * 8 ) + 8;
	nWidth[6] = ( 18 * 4 ) + 4;
	nWidth[7] = ( 18 * 4 ) + 4;
	nWidth[8] = ( 10 * 4 ) + 4;
	nWidth[9] = ( 12 * 8 ) + 8;
	nWidth[10] = ( 14 * 4 ) + 4;
	nWidth[11] = ( 2 * 8 ) + 8;
	nWidth[12] = ( 14 * 4 ) + 4;
	nWidth[13] = ( 30 * 8 ) + 8;
	nWidth[14] = ( 30 * 8 ) + 8;
	nWidth[15] = ( 30 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式061データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki061Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[16];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc061Yuukasyouken	mfcRec( pDB );	//	uc_061_yuukasyoukenテーブルクラス
	CdateConvert			clsDate;		//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );												//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;											//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );											//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );											//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;													//	非表示カラムにシーケンス番号設定

					udData[2].dat_string = mfcRec.m_KnName; 
					udData[3].dat_string = mfcRec.m_Syurui; 
					udData[4].dat_string = GetFormatValue( mfcRec.m_Suuryou, "sssssssssssss9.999" ); 
					udData[5].dat_string = mfcRec.m_Tanni; 
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val, "sssssssssssssssss9" );
					udData[7].dat_string = GetFormatValue( mfcRec.m_Val2, "sssssssssssssssss9" );
					
					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate );
						udData[8].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );
					}
					else{
						udData[8].dat_string.Empty();
					}

					udData[9].dat_string = mfcRec.m_Riyuu; 
					udData[10].dat_string = GetFormatValue( mfcRec.m_Suuryou2, "sssssssss9.999" ); 
					udData[11].dat_string = mfcRec.m_Tanni2;
					udData[12].dat_string = GetFormatValue( mfcRec.m_Val3 );
					udData[13].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[14].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[15].dat_string = mfcRec.m_Teki; 
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式071カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki071Column( int nColumnMax )
{
	CString		szTitle[14];	//	カラムタイトル
	int			nWidth[14];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "種類・構造";
	szTitle[3]= "用途";
	szTitle[4]= "面積";
	szTitle[5]= "物件の所在地";
	szTitle[6]= "期末現在高";
	szTitle[7]= "異動年月日";
	szTitle[8]= "異動事由";
	szTitle[9]= "取得価額";
	szTitle[10]= "帳簿価額";
	szTitle[11]= "売却先（名称）";
	szTitle[12]= "売却先（所在地）";
	szTitle[13]= "取得年月";
	
	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 14 * 8 ) + 8; 
	nWidth[3] = ( 10 * 8 ) + 8;
	nWidth[4] = ( 14 * 4 ) + 4;
	nWidth[5] = ( 40 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4;
	nWidth[7] = ( 10 * 4 ) + 4;
	nWidth[8] = ( 12 * 8 ) + 8;
	nWidth[9] = ( 14 * 4 ) + 4;
	nWidth[10] = ( 14 * 4 ) + 4;
	nWidth[11] = ( 30 * 8 ) + 8;
	nWidth[12] = ( 30 * 8 ) + 8;
	nWidth[13] = ( 16 * 4 ) + 4 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式071データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki071Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[14];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc071Koteishisan		mfcRec( pDB );	//	uc_071_koteishisanテーブルクラス
	CdateConvert			clsDate;		//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );												//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;											//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );											//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );											//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;													//	非表示カラムにシーケンス番号設定

					udData[2].dat_string = mfcRec.m_Syurui; 
					udData[3].dat_string = mfcRec.m_Youto; 
					udData[4].dat_string = GetFormatValue( mfcRec.m_Area, "ssssssssss9.99" );
					udData[5].dat_string = mfcRec.m_Ad2Add1 + mfcRec.m_Ad2Add2;
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );

					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate );
						udData[7].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );
					}
					else{
						udData[7].dat_string.Empty(); 
					}

					udData[8].dat_string = mfcRec.m_Riyuu;
					udData[9].dat_string = GetFormatValue( mfcRec.m_MsVal );
					udData[10].dat_string = GetFormatValue( mfcRec.m_MsVal2 );
					udData[11].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[12].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					
					if ( mfcRec.m_SpDate2 > 0 ){
						clsDate.Convert( mfcRec.m_SpDate2 );
						udData[13].dat_string.Format( "%s%s.%s", clsDate.m_szGengou, clsDate.m_szWareki, clsDate.m_szMonth );  
					}
					else{
						udData[13].dat_string.Empty(); 
					}
		
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式081カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki081Column( int nColumnMax )
{
	CString		szTitle[8];		//	カラムタイトル
	int			nWidth[8];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "支払先";
	szTitle[3]= "振出年月日";
	szTitle[4]= "支払期日";
	szTitle[5]= "支払銀行名";
	szTitle[6]= "金額";
	szTitle[7]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 20 * 8 ) + 8; 
	nWidth[3] = ( 10 * 4 ) + 4;
	nWidth[4] = ( 8 * 4 ) + 4;
	nWidth[5] = ( 21 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4;
	nWidth[7] = ( 40 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式081データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki081Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[8];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc081Siharaitegata	mfcRec( pDB );	//	uc_081_Shiharaitegataテーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	振出人

					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate );							//	日付変換
						udData[3].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );	//	振出年月日
					}
					else{
						udData[3].dat_string.Empty(); 
					}
					
					if ( mfcRec.m_PmDate > 0 ){
						clsDate.Convert( mfcRec.m_PmDate );							//	日付変換
						udData[4].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );	//	支払期日
					}
					else{
						udData[4].dat_string.Empty(); 
					}
					
					//	支店名あり？
					if ( !mfcRec.m_BkName2.IsEmpty() ){			
						udData[5].dat_string.Format( "%s／%s", mfcRec.m_BkName1, mfcRec.m_BkName2 );	//	支払銀行名
					}
					else{
						udData[5].dat_string = mfcRec.m_BkName1;										//	支払銀行名
					}

					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );		//	金額設定
					udData[7].dat_string = mfcRec.m_Teki;						//	適用設定

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式091カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki091Column( int nColumnMax )
{
	CString		szTitle[7];		//	カラムタイトル
	int			nWidth[7];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "相手先（名称）";
	szTitle[4]= "相手先（所在地）";
	szTitle[5]= "期末現在高";
	szTitle[6]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 20 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式091データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki091Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[9];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc091Kaikakekin		mfcRec( pDB );	//	uc_091_kaikakekinテーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;						//	科目 
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	名称
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;	//	住所
					udData[5].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[6].dat_string = mfcRec.m_Teki;						//	適用

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式101カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki101Column( int nColumnMax )
{
	CString		szTitle[8];		//	カラムタイトル
	int			nWidth[8];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "相手先（名称）";
	szTitle[4]= "相手先（所在地）";
	szTitle[5]= "相手先（関係）";
	szTitle[6]= "期末現在高";
	szTitle[7]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 7 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4;
	nWidth[7] = ( 20 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式101データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki101Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[8];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc101Kariukekin		mfcRec( pDB );	//	uc_101_kariukekinテーブルクラス
	
	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;						//	科目 
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	名称
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;	//	住所
					udData[5].dat_string = mfcRec.m_Kankei;						//	法人・代表者との関係
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[7].dat_string = mfcRec.m_Naiyou;						//	取引の内容

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式102カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki102Column( int nColumnMax )
{
	CString		szTitle[5];		//	カラムタイトル
	int			nWidth[5];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "年月日";
	szTitle[3]= "所得の種類";
	szTitle[4]= "期末現在高";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 6 * 4 ) + 4; 
	nWidth[3] = ( 5 * 8 ) + 8;
	nWidth[4] = ( 14 * 4 ) + 4;
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式102データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki102Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[5];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();			//	リストの全アイテム消去
	CdbUc102Gensen		mfcRec( pDB );	//	uc_102_gensenテーブルクラス
	CdateConvert			clsDate;	//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate ); 
						udData[2].dat_string.Format( "%s.%s", clsDate.m_szWareki, clsDate.m_szMonth ); 
					}
					else{
						udData[2].dat_string.Empty(); 
					}

					udData[3].dat_string = mfcRec.m_KnName; 
					udData[4].dat_string = GetFormatValue( mfcRec.m_Val );	//	期末現在高設定
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式111カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki111Column( int nColumnMax )
{
	CString		szTitle[11];	//	カラムタイトル
	int			nWidth[11];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "借入先";
	szTitle[4]= "法人・代表者との関係";
	szTitle[5]= "所在地";
	szTitle[6]= "期末現在高";
	szTitle[7]= "期中の支払利子額";
	szTitle[8]= "利率";
	szTitle[9]= "借入理由";
	szTitle[10]= "担保の内容";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8; 
	nWidth[4] = ( 10 * 8 ) + 8;
	nWidth[5] = ( 40 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4;
	nWidth[7] = ( 16 * 4 ) + 4;
	nWidth[8] = ( 14 * 4 ) + 4;
	nWidth[9] = ( 20 * 8 ) + 8;
	nWidth[10] = ( 26 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式111データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki111Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[11];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc111Kariirekin		mfcRec( pDB );	//	uc_111_kariirekinテーブルクラス
	CdateConvert			clsDate;		//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_KnName;
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[4].dat_string = mfcRec.m_Kankei; 
					udData[5].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[7].dat_string = GetFormatValue( mfcRec.m_Risoku, "sssssssssssssss9" );
					udData[8].dat_string = GetFormatValue( mfcRec.m_Rate, "sssssssss9.999" );
					udData[9].dat_string = mfcRec.m_Riyuu; 
					udData[10].dat_string = mfcRec.m_Naiyou;					//	取引の内容

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式121カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki121Column( int nColumnMax )
{
	CString		szTitle[13];	//	カラムタイトル
	int			nWidth[13];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "区分";
	szTitle[3]= "商品の所在地";
	szTitle[4]= "地目";
	szTitle[5]= "総面積";
	szTitle[6]= "売上年月";
	szTitle[7]= "売上先（名称）";
	szTitle[8]= "売上先（所在地）";
	szTitle[9]= "売上面積";
	szTitle[10]= "売上金額（上段）";
	szTitle[11]= "売上金額（下段）";
	szTitle[12]= "売上商品の取得年";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 5 * 8 ) + 8; 
	nWidth[3] = ( 30 * 8 ) + 8;
	nWidth[4] = ( 8 * 8 ) + 8;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 8 * 4 ) + 4;
	nWidth[7] = ( 20 * 8 ) + 8;
	nWidth[8] = ( 40 * 8 ) + 8;
	nWidth[9] = ( 14 * 4 ) + 4;
	nWidth[10] = ( 16 * 4 ) + 4;
	nWidth[11] = ( 16 * 4 ) + 4;
	nWidth[12] = ( 16 * 4 ) + 4 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式121データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki121Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[13];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc121Totiuriage		mfcRec( pDB );	//	uc_121_totiuriageテーブルクラス
	CdateConvert			clsDate;		//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_KnName; 
					udData[3].dat_string = mfcRec.m_Ad2Add1 + mfcRec.m_Ad2Add2;
					udData[4].dat_string = mfcRec.m_HdName; 
					udData[5].dat_string = GetFormatValue( mfcRec.m_Area, "ssssssssss9.99" );

					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate ); 
						udData[6].dat_string.Format( "%s.%s", clsDate.m_szWareki, clsDate.m_szMonth ); 
					}
					else{
						udData[6].dat_string.Empty(); 
					}

					udData[7].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[8].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[9].dat_string = GetFormatValue( mfcRec.m_Area2, "ssssssssss9.99" );
					udData[10].dat_string = GetFormatValue( mfcRec.m_Val, "sssssssssssssss9" );
					udData[11].dat_string = GetFormatValue( mfcRec.m_Val2, "sssssssssssssss9" );

					if ( mfcRec.m_SpDate2 > 0 ){
						clsDate.Convert( mfcRec.m_SpDate2 ); 
						udData[12].dat_string.Format( "%s%s", clsDate.m_szGengou, clsDate.m_szWareki );  
					}
					else{
						udData[12].dat_string.Empty();
					}

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式131カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki131Column( int nColumnMax )
{
	CString		szTitle[13];	//	カラムタイトル
	int			nWidth[13];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "事業所の名称";
	szTitle[3]= "事業所（所在地）";
	szTitle[4]= "責任者氏名";
	szTitle[5]= "代表者との関係";
	szTitle[6]= "事業等の内容";
	szTitle[7]= "売上高";
	szTitle[8]= "期末棚卸高";
	szTitle[9]= "期末従業員数";
	szTitle[10]= "使用建物の延面積";
	szTitle[11]= "源泉所得税納付署";
	szTitle[12]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 20 * 8 ) + 8; 
	nWidth[3] = ( 40 * 8 ) + 8; 
	nWidth[4] = ( 16 * 8 ) + 8;
	nWidth[5] = ( 8 * 8 ) + 8;
	nWidth[6] = ( 15 * 8 ) + 8;
	nWidth[7] = ( 14 * 4 ) + 4;
	nWidth[8] = ( 14 * 4 ) + 4;
	nWidth[9] = ( 14 * 4 ) + 4;
	nWidth[10] = ( 16 * 4 ) + 4;
	nWidth[11] = ( 10 * 8 ) + 8;
	nWidth[12] = ( 24 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式131データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki131Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[13];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc131Uriagedaka		mfcRec( pDB );	//	uc_131_uriagedakaテーブルクラス
	CdateConvert			clsDate;		//	日付変換

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_AdName1;
					udData[3].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[4].dat_string = mfcRec.m_HdName;
					udData[5].dat_string = mfcRec.m_Kankei;
					udData[6].dat_string = mfcRec.m_Naiyou;
					udData[7].dat_string = GetFormatValue( mfcRec.m_Val ); 
					udData[8].dat_string = GetFormatValue( mfcRec.m_Val2 );
					udData[9].dat_string = GetFormatValue( mfcRec.m_Worker );
					udData[10].dat_string = GetFormatValue( mfcRec.m_Area, "ssssssssssss9.99" );
					udData[11].dat_string = mfcRec.m_HdName2;
					udData[12].dat_string = mfcRec.m_Teki;

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式141カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki141Column( int nColumnMax )
{
	CString		szTitle[15];	//	カラムタイトル
	int			nWidth[15];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "役職名";
	szTitle[3]= "担当業務";
	szTitle[4]= "氏名";
	szTitle[5]= "代表者との関係";
	szTitle[6]= "住所";
	szTitle[7]= "常勤・非常勤の別";
	szTitle[8]= "使用人職務分";				// 2006.10.23：新会社法対応
	szTitle[9]= "定期同額給与";				// 2006.10.23：新会社法対応
	szTitle[10]= "事前確定届出給与";		// 2006.10.23：新会社法対応
	szTitle[11]= "利益連動給与";			// 2006.10.23：新会社法対応
	szTitle[12]= "その他";					// 2006.10.23：新会社法対応
	szTitle[13]= "退職給与";				// 2006.10.23：新会社法対応
	szTitle[14]= "役員給与計";				// 2006.10.23：新会社法対応

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 8 * 8 ) + 8; 
	nWidth[4] = ( 24 * 8 ) + 8; 
	nWidth[5] = ( 7 * 8 ) + 8;
	nWidth[6] = ( 40 * 8 ) + 8;
	nWidth[7] = ( 8 * 8 ) + 8;
	nWidth[8] = ( 12 * 4 ) + 4;
	nWidth[9] = ( 12 * 4 ) + 4;
	nWidth[10] = ( 16 * 4 ) + 4;
	nWidth[11] = ( 12 * 4 ) + 4;
	nWidth[12] = ( 12 * 4 ) + 4;
	nWidth[13] = ( 12 * 4 ) + 4;
	nWidth[14] = ( 12 * 4 ) + 4 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式141データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki141Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[15];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc141Yakuin			mfcRec( pDB );	//	uc_141_yakkuinテーブルクラス
	CdateConvert			clsDate;		//	日付変換
	CString					szFpTime;		//	常勤・非常勤の文字列格納
	
	//	初期化
	szFpTime.Empty();

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_NameYaku; 
					udData[3].dat_string = mfcRec.m_NameTant;
					udData[4].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[5].dat_string = mfcRec.m_Kankei;
					udData[6].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					
					//	常勤非常勤のフラグで分岐
					switch( mfcRec.m_FpTime ){
					//	常勤
					case 1:
						szFpTime = "常";
						break;
					//	非常勤
					case 2:
						szFpTime = "非";
						break;
					//	その他
					default:
						szFpTime.Empty(); 
						break;
					}
					udData[7].dat_string = szFpTime;
					udData[8].dat_string = GetFormatValue( mfcRec.m_EmVal, "sssssssssss9" );		// 2006.10.23：新会社法対応
					udData[9].dat_string = GetFormatValue( mfcRec.m_PrVal, "sssssssssss9" );		// 2006.10.23：新会社法対応
					udData[10].dat_string = GetFormatValue( mfcRec.m_BfVal, "sssssssssssssss9" );	// 2006.10.23：新会社法対応
					udData[11].dat_string = GetFormatValue( mfcRec.m_PfVal, "sssssssssss9" );		// 2006.10.23：新会社法対応
					udData[12].dat_string = GetFormatValue( mfcRec.m_EtVal, "sssssssssss9" );		// 2006.10.23：新会社法対応
					udData[13].dat_string = GetFormatValue( mfcRec.m_RtVal, "sssssssssss9" );		// 2006.10.23：新会社法対応
					udData[14].dat_string = GetFormatValue( mfcRec.m_Val, "sssssssssss9" );			// 2006.10.23：新会社法対応

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式151カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki151Column( int nColumnMax )
{
	CString		szTitle[10];	//	カラムタイトル
	int			nWidth[10];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "地代・家賃の区分";
	szTitle[3]= "借地物件の用途";
	szTitle[4]= "所在地";
	szTitle[5]= "貸主の名称";
	szTitle[6]= "貸主の所在地";
	szTitle[7]= "支払対象期間";
	szTitle[8]= "支払貸借料";
	szTitle[9]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 40 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 20 * 8 ) + 8;
	nWidth[6] = ( 40 * 8 ) + 8;
	nWidth[7] = ( 18 * 4 ) + 4;
	nWidth[8] = ( 14 * 4 ) + 4;
	nWidth[9] = ( 42 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式151データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki151Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[10];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc151Tidaiyatin		mfcRec( pDB );	//	uc_151_tidaiyatinテーブルクラス
	CdateConvert			clsDate;
	CString					szDate[2];
	
	szDate[0].Empty();
	szDate[1].Empty();

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName; 
					udData[3].dat_string = mfcRec.m_Youto;
					udData[4].dat_string = mfcRec.m_Ad2Add1 + mfcRec.m_Ad2Add2;
					udData[5].dat_string = mfcRec.m_AdName1;
					udData[6].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;

					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate ); 
						szDate[0].Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );
					}
					else{
						szDate[0].Empty();
					}

					if ( mfcRec.m_SpDate2 > 0 ){
						clsDate.Convert( mfcRec.m_SpDate2 ); 
						szDate[1].Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );
					}
					else{
						szDate[1].Empty();
					}
					
					udData[7].dat_string.Format( "%8s～%8s", szDate[0], szDate[1] );
					udData[8].dat_string = GetFormatValue( mfcRec.m_Val );
					udData[9].dat_string = mfcRec.m_Teki;

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式152カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki152Column( int nColumnMax )
{
	CString		szTitle[8];		//	カラムタイトル
	int			nWidth[8];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "支払先の名称";
	szTitle[3]= "支払先の所在地";
	szTitle[4]= "支払年月日";
	szTitle[5]= "支払金額";
	szTitle[6]= "権利金等の内容";
	szTitle[7]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 20 * 8 ) + 8; 
	nWidth[3] = ( 40 * 8 ) + 8;
	nWidth[4] = ( 10 * 4 ) + 4;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 26 * 8 ) + 8;
	nWidth[7] = ( 33 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式152データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki152Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[8];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc152Kenrikin		mfcRec( pDB );	//	uc_152_kenrikinテーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_AdName1;
					udData[3].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;

					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate ); 
						udData[4].dat_string.Format( "%s.%s.%s", clsDate.m_szWareki, clsDate.m_szMonth, clsDate.m_szDay );  
					}
					else{
						udData[4].dat_string.Empty(); 
					}

					udData[5].dat_string = GetFormatValue( mfcRec.m_Val );
					udData[6].dat_string = mfcRec.m_Naiyou;
					udData[7].dat_string = mfcRec.m_Teki;
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式153カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki153Column( int nColumnMax )
{
	CString		szTitle[9];		//	カラムタイトル
	int			nWidth[9];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "名称";
	szTitle[3]= "支払先の名称";
	szTitle[4]= "支払先の所在地";
	szTitle[5]= "契約期間";
	szTitle[6]= "支払対象期間";
	szTitle[7]= "支払金額";
	szTitle[8]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 10 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 20 * 4 ) + 4;
	nWidth[6] = ( 12 * 4 ) + 4;
	nWidth[7] = ( 14 * 4 ) + 4;
	nWidth[8] = ( 30 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式153データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki153Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[9];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();				//	リストの全アイテム消去
	CdbUc153Kougyou			mfcRec( pDB );	//	uc_153_kougyouテーブルクラス
	CdateConvert			clsDate;
	CString					szDate[2];
	
	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_HdName;
					udData[3].dat_string = mfcRec.m_AdName1;
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					
					szDate[0].Empty();
					szDate[1].Empty();
					
					if ( mfcRec.m_KyDate > 0 ){
						clsDate.Convert( mfcRec.m_KyDate );
						szDate[0].Format( "%s%s.%s", clsDate.m_szGengou, clsDate.m_szWareki, clsDate.m_szMonth );
					}
					
					if ( mfcRec.m_KyDate2 > 0 ){
						clsDate.Convert( mfcRec.m_KyDate2 );
						szDate[1].Format( "%s%s.%s", clsDate.m_szGengou, clsDate.m_szWareki, clsDate.m_szMonth );
					}
					udData[5].dat_string.Format( "%9s～%9s", szDate[0], szDate[1] );
					
					szDate[0].Empty();
					szDate[1].Empty();
					
					if ( mfcRec.m_SpDate > 0 ){
						clsDate.Convert( mfcRec.m_SpDate );
						szDate[0].Format( "%s.%s", clsDate.m_szWareki, clsDate.m_szMonth );
					}
					
					if ( mfcRec.m_SpDate2 > 0 ){
						clsDate.Convert( mfcRec.m_SpDate2 );
						szDate[1].Format( "%s.%s", clsDate.m_szWareki, clsDate.m_szMonth );
					}
					udData[6].dat_string.Format( "%5s～%5s", szDate[0], szDate[1] );

					udData[7].dat_string = GetFormatValue( mfcRec.m_Val );
					udData[8].dat_string = mfcRec.m_Teki;
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式161／162カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki16Column( int nColumnMax )
{
	CString		szTitle[7];		//	カラムタイトル
	int			nWidth[7];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "取引の内容";
	szTitle[4]= "相手先";
	szTitle[5]= "所在地";
	szTitle[6]= "金額";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 24 * 8 ) + 8;
	nWidth[4] = ( 20 * 8 ) + 8;
	nWidth[5] = ( 40 * 8 ) + 8;
	nWidth[6] = ( 14 * 4 ) + 4 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式161データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki161Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[7];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();			//	リストの全アイテム消去
	CdbUc161Zatueki	mfcRec( pDB );		//	uc_061_zatuekiテーブルクラス

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_KnName;
					udData[3].dat_string = mfcRec.m_Naiyou; 
					udData[4].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[5].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式162データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki162Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[7];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();			//	リストの全アイテム消去
	CdbUc162Zatuson	mfcRec( pDB );		//	uc_062_zatusonテーブルクラス

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
					
					udData[2].dat_string = mfcRec.m_KnName;
					udData[3].dat_string = mfcRec.m_Naiyou; 
					udData[4].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;
					udData[5].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;
					udData[6].dat_string = GetFormatValue( mfcRec.m_Val );
					
					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式171カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki171Column( int nColumnMax )
{
	CString		szTitle[7];		//	カラムタイトル
	int			nWidth[7];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= "相手先（名称）";
	szTitle[4]= "相手先（所在地）";
	szTitle[5]= "期末現在高";
	szTitle[6]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 20 * 8 ) + 8;
	nWidth[4] = ( 40 * 8 ) + 8;
	nWidth[5] = ( 14 * 4 ) + 4;
	nWidth[6] = ( 26 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式171データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki171Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[7];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();							//	リストの全アイテム消去
	CdbUc171Sonota1			mfcRec( pDB, m_nFormSeq );	//	uc_171_sonota1テーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;						//	科目 
					udData[3].dat_string = mfcRec.m_AdName1 + mfcRec.m_AdName2;	//	名称
					udData[4].dat_string = mfcRec.m_AdAdd1 + mfcRec.m_AdAdd2;	//	住所
					udData[5].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[6].dat_string = mfcRec.m_Teki;						//	適用

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
// 様式181カラム設定
//	【引数】	nColumnMax	…	カラム最大数
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetYoushiki181Column( int nColumnMax )
{
	CString		szTitle[6];		//	カラムタイトル
	int			nWidth[6];		//	カラム幅
		
	//	カラムタイトル設定
	szTitle[0]= "";				//	非表示項目（選択値）
	szTitle[1]= "";				//	非表示項目（シーケンス番号）
	szTitle[2]= "科目";
	szTitle[3]= GetSonota2ItemName();
	szTitle[4]= "期末現在高";
	szTitle[5]= "摘要";

	//	カラム幅設定（（ 文字数 × １文字幅 ） + 余白用１文字幅）
	nWidth[0] = 0;				//	非表示項目
	nWidth[1] = 0;				//	非表示項目
	nWidth[2] = ( 8 * 8 ) + 8; 
	nWidth[3] = ( 40 * 8 ) + 8;
	nWidth[4] = ( 14 * 4 ) + 4;
	nWidth[5] = ( 40 * 8 ) + 8 + ( 8 );	//	垂直スクロールバー用に更に余白１文字幅追加
	
	// リスト設定
	SetList( nColumnMax, szTitle, nWidth );
}

//**************************************************
// 様式181データ設定
//	【引数】	pDB			…	データベースハンドル
//				nType		…	種別
//				nColumnMax	…	カラム最大数
//	【戻値】	行数
//**************************************************
int CdlgDisplay::SetYoushiki181Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax )
{		
	LINP_DATA	udData[6];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	int			nLine = 0;		//	行管理用
	char		szBuf[8];
	int			nRet;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );							//	LINP_DATA構造体初期化
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}

	m_lstDisplay.RemoveAll();							//	リストの全アイテム消去
	CdbUc181Sonota2			mfcRec( pDB, m_nFormSeq );	//	uc_181_sonota2テーブルクラス
	CdateConvert			clsDate;

	try{
		int ret;
		//	種別で分岐
		switch( nType ){
		//	一時保管
		case ID_FGSHOW_HOKAN:
			ret = mfcRec.Open( AFX_DB_USE_DEFAULT_TYPE, mfcRec.GetSQLHokan( m_nFormSeq ) );
			if( ret != 0 ){
				nRet = DB_ERR_OK;
			}
			break;
		//	一括金額
		case ID_FGSHOW_IKKATU:
			nRet = mfcRec.RequeryFgShowIkkatu( m_nNumGroup );
			break;
		//	デフォルト
		default:
			nRet = mfcRec.RequeryFgShowZero();
			break;
		}
		
		//	初期化成功？
		if ( nRet == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();									
				
				//	終端までループ
				while( !mfcRec.IsEOF() ){
				
					m_lstDisplay.AddNewLine( TRUE );						//	新規行追加
					udData[0].dat_string = DD_SELECT_OFF;					//	非表示カラムに選択値を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );					//	バッファクリア
					_itoa_s( ( mfcRec.m_Seq ), szBuf, 10 );					//	シーケンス番号を文字列に変換
					udData[1].dat_string = szBuf;							//	非表示カラムにシーケンス番号設定
	
					udData[2].dat_string = mfcRec.m_KnName;						//	科目 
					udData[3].dat_string = mfcRec.m_Item;						//	項目 
					udData[4].dat_string = GetFormatValue( mfcRec.m_Val );		//	期末現在高
					udData[5].dat_string = mfcRec.m_Teki;						//	適用

					//	カラム数分ループ
					for( nCnt = 0; nCnt < nColumnMax; nCnt++ ){
						//	カラム登録
						m_lstDisplay.SetColumnData( nLine, nCnt, ( LPUNKNOWN )udData, TRUE );
					}
					//	次のレコードに移動
					mfcRec.MoveNext();
					//	行をインクリメント
					nLine++;
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}	
	}
	
	//	行数を返す
	return( nLine );
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgDisplay::GetFocusControlID( CWnd* pParent /*=NULL*/)
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
//	フォーマットした値取得
//	【引数】	szSrc		…	フォーマット対象
//				szFormat	…	出力フォーマット
//	【戻値】	フォーマットした値取得
//**************************************************
CString CdlgDisplay::GetFormatValue( CString szSrc, char* szFormat /*="sssssssssssss9"*/ )
{
	CString		szRet;			//	戻値
	char		szBuf[32];		//	バッファ
	char		szVal[6];		//	フォーマット対象（BCD）
	
	//	初期化
	szRet.Empty();
	
	//	フォーマット対象がヌルじゃない？
	if ( szSrc != "" ){

		//	初期化
		szRet = "0"; 	
		ZeroMemory( szBuf, sizeof( szBuf ) );
		ZeroMemory( szVal, sizeof( szVal ) );

		//	除算対象をBCDに変換
		ZeroMemory( szBuf, sizeof( szBuf ) );
		strcpy_s( &szBuf[0], sizeof( szBuf ), szSrc );
		l_input( szVal, szBuf );

		//	指定フォーマットで文字列に変換
		ZeroMemory( szBuf, sizeof( szBuf ) );
		l_print( szBuf, szVal, szFormat );
		
		//	戻値に設定
		szRet = szBuf;
	}

	//	戻値を返す
	return( szRet );
}

//**************************************************
//	LINP_DATA構造体初期化
//	【引数】	pData	…	LINP_DATA構造体ポインタ
//	【戻値】	なし
//**************************************************
void CdlgDisplay::InitLinpData( LINP_DATA* pData )
{
	pData->dat_attr = 0;
	pData->dat_bc = 0;
	ZeroMemory( pData->dat_bmn, sizeof( pData->dat_bmn ) );
	ZeroMemory( pData->dat_day, sizeof( pData->dat_day ) );
	pData->dat_dpn = 0;
	ZeroMemory( pData->dat_eda, sizeof( pData->dat_eda ) );
	pData->dat_fc = 0;
	pData->dat_initflg = 0;
	pData->dat_kamoku.kamoku_code = 0;  
	pData->dat_kamoku.kamoku_edaban = 0;
	ZeroMemory( pData->dat_kamoku.kamoku_name, sizeof( pData->dat_kamoku.kamoku_name ) );
	pData->dat_kamoku.kamoku_num = 0;
	ZeroMemory( pData->dat_numeric, sizeof( pData->dat_numeric ) );
	pData->dat_sel.sel_data = 0;
	pData->dat_sel.sel_num = 0;
	pData->dat_sel.sel_rslt = 0;
	pData->dat_seq = 0;
	pData->dat_string.Empty();
	ZeroMemory( pData->dat_tek.tek_data, sizeof( pData->dat_tek.tek_data ) );
	pData->dat_tek.tek_height = 0;
	pData->dat_tek.tek_image = NULL;
	pData->dat_tek.tek_width = 0;
	ZeroMemory( pData->dat_val, sizeof( pData->dat_val ) );
}

//**************************************************
//	選択行を設定
//	【引数】	nIndex		…	インデックス
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetSelectRow( int nIndex )
{
	CString		szSelect;	//	選択値
	COLORREF	nColor;		//	色値
	
	//	選択データを取得
	szSelect = GetSelectData( nIndex );
	
	//	選択オフ？
	if ( szSelect == DD_SELECT_OFF ){
		//	選択オンに変更
		szSelect = DD_SELECT_ON;
		//	色を設定
		nColor = RGB( 127, 127, 127 );
	}
	//	選択オン
	else{
		//	選択オフに変更
		szSelect = DD_SELECT_OFF;
		//	色を設定
		nColor = GetSysColor( COLOR_WINDOW );
	}

	//	選択データを設定
	SetSelectData( nIndex, szSelect );
	//	色を設定
	SetColor( nIndex, nColor );
}

//**************************************************
//	選択データを取得
//	【引数】	nIndex		…	インデックス
//	【戻値】	選択値
//**************************************************
CString CdlgDisplay::GetSelectData( int nIndex )
{
	CString		szRet;			//	戻値
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	
	//	戻値初期化
	szRet.Empty(); 
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のデータを取得
	m_lstDisplay.GetData( nIndex, ( LPUNKNOWN )&udData );
	//	選択値取得
	szRet = udData[0].dat_string;
	
	//	戻値を返す
	return( szRet );
}

//**************************************************
//	選択データを設定
//	【引数】	nIndex		…	インデックス
//				szSelect	…	選択値
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetSelectData( int nIndex, CString szSelect )
{
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のデータを取得
	m_lstDisplay.GetData( nIndex, ( LPUNKNOWN )&udData );
	//	選択値を設定
	udData[0].dat_string = szSelect;
	//	現在選択されている行のデータを設定
	m_lstDisplay.SetData( nIndex, ( LPUNKNOWN )&udData, FALSE );
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				nColor	…	色値
//	【戻値】	なし
//**************************************************
void CdlgDisplay::SetColor( int nIndex, COLORREF nColor )
{
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のデータを取得
	m_lstDisplay.GetData( nIndex, ( LPUNKNOWN )udData );
	
	//	カラム最大数分ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){ 
		udData[nCnt].dat_bc = nColor;	//	背景色を変更
	}
	
	//	現在選択されている行のデータを設定
	m_lstDisplay.SetData( nIndex, ( LPUNKNOWN )udData, TRUE );
}

//**************************************************
//	リストアイテムをチェック
//	【引数】	fDel	…	削除フラグ
//	【戻値】	変更（削除）したアイテム数
//**************************************************
int CdlgDisplay::CheckListItam( BOOL fDel /*=FALSE*/ )
{
	int					nRet = 0;	//	戻値
	CdbUc000Common*		pRec = 0;	//	データベースクラスポインタ
	
	//	様式で分岐
	switch( m_nFormSeq ){
	case ID_FORMNO_011:
		pRec = new CdbUc011Yotyokin( m_pDB );
		break;
	case ID_FORMNO_021:
		pRec = new CdbUc021Uketoritegata( m_pDB );
		break;
	case ID_FORMNO_031:
		pRec = new CdbUc031Urikakekin( m_pDB );
		break;
	case ID_FORMNO_041:
		pRec = new CdbUc041Karibarai( m_pDB );
		break;
	case ID_FORMNO_042:
		pRec = new CdbUc042Kasituke( m_pDB );
		break;
	case ID_FORMNO_051:
		pRec = new CdbUc051Tanaoroshi( m_pDB );
		break;
	case ID_FORMNO_061:
		pRec = new CdbUc061Yuukasyouken( m_pDB );
		break;
	case ID_FORMNO_071:
		pRec = new CdbUc071Koteishisan( m_pDB );
		break;
	case ID_FORMNO_081:
		pRec = new CdbUc081Siharaitegata( m_pDB );
		break;
	case ID_FORMNO_091:
		pRec = new CdbUc091Kaikakekin( m_pDB );
		break;
	case ID_FORMNO_101:
		pRec = new CdbUc101Kariukekin( m_pDB );
		break;
	case ID_FORMNO_102:
		pRec = new CdbUc102Gensen( m_pDB );
		break;
	case ID_FORMNO_111:
		pRec = new CdbUc111Kariirekin( m_pDB );
		break;
	case ID_FORMNO_121:
		pRec = new CdbUc121Totiuriage( m_pDB );
		break;
	case ID_FORMNO_131:
		pRec = new CdbUc131Uriagedaka( m_pDB );
		break;
	case ID_FORMNO_141:
		pRec = new CdbUc141Yakuin( m_pDB );
		break;
	case ID_FORMNO_142:
		pRec = new CdbUc142Jinkenhi( m_pDB );
		break;
	case ID_FORMNO_151:
		pRec = new CdbUc151Tidaiyatin( m_pDB );
		break;
	case ID_FORMNO_152:
		pRec = new CdbUc152Kenrikin( m_pDB );
		break;
	case ID_FORMNO_153:
		pRec = new CdbUc153Kougyou( m_pDB );
		break;
	case ID_FORMNO_161:
		pRec = new CdbUc161Zatueki( m_pDB );
		break;
	case ID_FORMNO_162:
		pRec = new CdbUc162Zatuson( m_pDB );
		break;
	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		pRec = new CdbUc171Sonota1( m_pDB, m_nFormSeq );
		break;
	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		pRec = new CdbUc181Sonota2( m_pDB, m_nFormSeq );
		break;
	}	
	
	//	選択行の表示フラグ設定
	nRet = SetFgShow( pRec, fDel );
	//	データベースクラスを破棄
	delete( pRec );

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	選択行の表示フラグ設定
//	【引数】	pRec	…	データベースクラス
//				fDel	…	
//	【戻値】	変更（削除）したアイテム数
//**************************************************
int CdlgDisplay::SetFgShow( CdbUc000Common* pRec,  BOOL fDel /*=FALSE*/ )
{
	int				nRet = 0;	//	戻値
	int				nCnt;		//	カウント用
	int				nSeq;		//	シーケンス番号
	BYTE			nFgShow;	//	表示フラグ
	CdbUcLstHokan	mfcRec( m_pDB );

	//	リストアイテム分ループ
	for( nCnt = 0; nCnt < m_lstDisplay.GetCount(); nCnt++ ){

		//	選択値がオン？
		if ( GetSelectData( nCnt ) == DD_SELECT_ON ){

			//	シーケンス番号取得
			nSeq = GetSeq( nCnt );
			
			//	シーケンス番号で検索
			if ( pRec->RequerySeq( nSeq ) == DB_ERR_OK ){
				
				//	レコードあり？
				if ( !pRec->IsEOF() ){
					
					//	レコードの先頭に移動
					pRec->MoveFirst();
					
					//	削除する？
					if ( fDel == TRUE ){
						//	レコードの削除
						pRec->Delete(); 
					}
					else{
						
						//	編集開始
						pRec->Edit();
						
						//	種別で分岐
						switch( m_nType ){
						//	一括金額
						case ID_FGSHOW_IKKATU:
							nFgShow = ID_FGSHOW_RET_IKKATU;
							break;
						//	一時保管
						case ID_FGSHOW_HOKAN:
							nFgShow = ID_FGSHOW_RET_HOKAN;
							break;
						}
						
						//	表示フラグを変更
						pRec->m_FgShow = nFgShow;
						//	レコードの更新
						pRec->Update();
					}
					//	戻値インクリメント
					nRet++;
				}
				//	レコード閉じる
				pRec->Fin(); 
			}

			// 保管テーブル削除
			mfcRec.RequerySeqFormSeq( m_nFormSeq, nSeq );
			if ( !mfcRec.IsEOF() ){
				mfcRec.MoveFirst();
				if ( fDel == TRUE ){
					mfcRec.Delete(); 
				}
			}
		}
	}

	// 保管テーブルのOrderNum整理
	mfcRec.RequeryFormSeq( m_nFormSeq );
	if( !mfcRec.IsEOF() ){
		int nOrderNum = 1;
		mfcRec.MoveFirst();
		while( !mfcRec.IsEOF() ){
			mfcRec.Edit();
			mfcRec.m_OrderNum = nOrderNum;
			mfcRec.Update();

			// オーダーインクリメント
			nOrderNum++;

			// 次のレコードへ
			mfcRec.MoveNext();
		}
	}
	mfcRec.Fin();

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	シーケンス番号取得
//	【引数】	なし
//	【戻値】	シーケンス番号
//**************************************************
int CdlgDisplay::GetSeq( int nIndex )
{
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nRet = 0;		//	戻値
	int			nCnt;
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のカラムに設定されているデータを取得
	m_lstDisplay.GetData( nIndex, ( LPUNKNOWN )&udData );
	//	取得したデータを数値に変換
	nRet = atoi( udData[1].dat_string );

	//	取得した値を返す
	return( nRet );
}

//**************************************************
//	選択アイテムの確認
//	【引数】	なし
//	【戻値】	選択されているアイテム数
//**************************************************
int CdlgDisplay::CheckSelectItem()
{
	int		nRet = 0;	//	戻値
	int		nCnt;		//	カウント用

	//	リストアイテム分ループ
	for( nCnt = 0; nCnt < m_lstDisplay.GetCount(); nCnt++ ){
		//	選択値がオン？
		if ( GetSelectData( nCnt ) == DD_SELECT_ON ){
			//	戻値インクリメント
			nRet++;
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	改行文字列削除
//	【引数】	szBuf	…	文字列
//	【戻値】	改行文字列削除文字列
//**************************************************
CString CdlgDisplay::DelCrLfString( CString szBuf )
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
//	項目名称取得（⑱その他科目の内訳書②専用）
//	【引数】	なし
//	【戻値】	項目名称
//**************************************************
CString CdlgDisplay::GetSonota2ItemName()
{
	CString					szRet;				//	戻値
	CdbUcInfSonotaItem		mfcRec( m_pDB );	//	uc_inf_sonota_itemテーブルクラス
	
	//	戻値初期化
	szRet.Empty(); 
	
	//	レコード初期化成功？
	if ( mfcRec.Init( m_nFormSeq ) == DB_ERR_OK ){
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
			//	項目名称を取得
			szRet = mfcRec.m_ItemName; 
		}
		//	閉じる
		mfcRec.Fin();
	}

	//	戻値を返す
	return( szRet );
}

// midori 160612 cor -->
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースハンドル
////				nFormSeq		…	様式シーケンス番号
////				nType			…	種別
////				nNumGroup		…	グループ番号
////	【戻値】	ID_DLG_BACK		…	戻すが選択された
////				ID_DLG_BACKALL	…	全て戻すが選択された
////				ID_DLG_CANCEL	…	キャンセルが選択された
////**************************************************
//int CdlgDisplay::ShowDialog( CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/,
//							 EnumIdFgShow nType /*=ID_FGSHOW_ZERO*/, int nNumGroup /*=0*/ )
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nType			…	種別
//				nNumGroup		…	グループ番号
//				pDataKakutei	…	入力データを確定する
//	【戻値】	ID_DLG_BACK		…	戻すが選択された
//				ID_DLG_BACKALL	…	全て戻すが選択された
//				ID_DLG_CANCEL	…	キャンセルが選択された
//**************************************************
int CdlgDisplay::ShowDialog( CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/,
							 EnumIdFgShow nType /*=ID_FGSHOW_ZERO*/, int nNumGroup /*=0*/, BOOL pDataKakutei /*=0x04*/ )
// midori 160612 cor <--
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
// midori 160612 add -->
	m_DataKakutei = pDataKakutei;
// midori 160612 add <--

	//	データベースハンドルあり？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_nFormSeq = nFormSeq;		//	様式シーケンス番号取得
				m_nType = nType;			//	種別取得
				m_nNumGroup = nNumGroup;	//	グループ番号
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}
	
	//	戻値を返す
	return( m_nRet );
}