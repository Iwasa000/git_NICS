// dlgRenTekiyo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgRenTekiyo.h"


// CdlgRenTekiyo ダイアログ

IMPLEMENT_DYNAMIC(CdlgRenTekiyo, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgRenTekiyo::CdlgRenTekiyo(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgRenTekiyo::IDD, pParent)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgRenTekiyo::~CdlgRenTekiyo()
{
}

void CdlgRenTekiyo::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_TEKIYO_LIST, m_lstTekiyo);
}

void CdlgRenTekiyo::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

BEGIN_MESSAGE_MAP(CdlgRenTekiyo, ICSDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgRenTekiyo::OnBnClickedOkButton)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgRenTekiyo, ICSDialog)
	ON_EVENT(CdlgRenTekiyo, IDC_TEKIYO_LIST, 10, CdlgRenTekiyo::SelChangeTekiyoList, VTS_I2)
	ON_EVENT(CdlgRenTekiyo, IDC_TEKIYO_LIST, 6, CdlgRenTekiyo::LButtonClickTekiyoList, VTS_I2)
	ON_EVENT(CdlgRenTekiyo, IDC_TEKIYO_LIST, 7, CdlgRenTekiyo::LButtonDblClickTekiyoList, VTS_I2)
	ON_EVENT(CdlgRenTekiyo, IDC_TEKIYO_LIST, 9, CdlgRenTekiyo::FocusedTekiyoList, VTS_NONE)
END_EVENTSINK_MAP()

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenTekiyo::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 財務DBクラスを取得
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// リスト初期化
	InitList();
	//	リストデータ取得
	GetListData( m_pZmSub );
	//	位置移動
	MovePosition( 0, m_nStartIndex );
	// リサイズ
	OnInitDialogEX();

	//	戻値を返す
	return( TRUE ); 
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（選択ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenTekiyo::OnBnClickedOkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//	OKボタン有効？
	if ( m_btnOK.IsWindowEnabled() == TRUE ){
		// 取得摘要コードをセット
		m_Tkcod = GetTekiyoCode( GetListIndex() );
		//	戻値に選択ボタンが押されたをセット
		m_nRet = ID_DLG_OK;
		ICSDialog::OnOK();
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenTekiyo::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenTekiyo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	long	nID = 0;	//	ID番号

	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN ){

		//	パラメータで分岐
		switch( pMsg->wParam ){
		
		//	Enterキー
		case VK_RETURN:
			//	OnOk起動
//			nID = IDOK;
			//	リストに居る？
			if ( GetFocusControlID( this ) == IDC_TEKIYO_LIST ){
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			}
			break;
		//	Escキー
		case VK_ESCAPE:
			//	OnCancel起動
			nID = IDCANCEL;
			break;
		//	F2キー
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;
		//	PageUp
		case VK_PRIOR:
		//	PageDown
		case VK_NEXT:
			//	現在行確認
			CheckNowRowPosition();
			break;
		//	その他キー
		default:
			break;
		}
	}

	//	CICSListInpのスクロールイベントが取得出来なかったため
	//	解決方法が見つかるまで暫定処理として
	//	マウス移動イベントで現在行の確認処理を実装
	else if (( pMsg->message == WM_NCMOUSEMOVE ) || ( pMsg->message == WM_MOUSEMOVE )){
		//	現在行確認
		CheckNowRowPosition();
	}

	//	ID番号あり？
	if ( nID != 0 ){
		//	メッセージ投げる
		PostMessage( WM_COMMAND, nID );
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	セルが移動した
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenTekiyo::SelChangeTekiyoList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//	現在行確認
	CheckNowRowPosition();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（シングル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenTekiyo::LButtonClickTekiyoList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//	現在行確認
	CheckNowRowPosition();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（ダブル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenTekiyo::LButtonDblClickTekiyoList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	OnBnClickedOkButton();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	フォーカス（来た）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenTekiyo::FocusedTekiyoList()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//	前回選択行あり？
	if ( m_nOldRow >= 0 ){
		//	行の背景色を更新
		ReDrawRowColor();
	}
}

//**************************************************
// リスト初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::InitList(){
	int					nCnt;			//	カウント用
	LINP_COLUMN			udColumn[ID_TEKIYO_LIST_MAX + 3];	//	カラム構造体
	LINP_VCOLORINFO		udGrid[ID_TEKIYO_LIST_MAX + 3];		//	縦グリッド色構造体
	int					nColumnCnt = 0;	//	カラムカウント数
	
	//	最大カラム数
	m_nColumnMax = ID_TEKIYO_LIST_MAX;

	//------------------------------
	//	0カラム目（非表示）
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 6;															//	入力最大桁数
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;						//	属性（※フォーカス移動のため読み取り属性を外す）
	udColumn[nColumnCnt].clm_width = 0;															//	幅（DLU）
	udColumn[nColumnCnt].clm_title = _T( "摘要コード" );										//	タイトル
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//------------------------------
	//	1カラム目
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 6;															//	入力最大桁数
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;	//	属性
	udColumn[nColumnCnt].clm_width = 32;														//	幅（DLU）
	udColumn[nColumnCnt].clm_title = _T( "番号" );												//	タイトル
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//------------------------------
	//	2カラム目
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
	udColumn[nColumnCnt].clm_max = 10;
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	udColumn[nColumnCnt].clm_width = ( 25 * 8 ) + 8;
	udColumn[nColumnCnt].clm_title = _T( "名称" );
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//	カラムの設定
	m_lstTekiyo.SetColumn( m_nColumnMax, ( LPUNKNOWN )udColumn );
	//	横グリッド線設定
	m_lstTekiyo.SetHorzGrid( 5, RGB( 0x00, 0xFF, 0xFF), LINP_GRID_SOLID );

	//	縦グリッド線数文ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
		udGrid[nCnt].pos = nCnt;						//	縦グリッド線を引くカラムのオフセット
		udGrid[nCnt].color = RGB( 0xFF, 0x00, 0x00 );	//	色
		udGrid[nCnt].type = LINP_GRID_SOLID;			//	線種
	}
	
	//	縦グリッド線設定
	m_lstTekiyo.SetVartGrid( m_nColumnMax, ( LPUNKNOWN )udGrid );
}

//**************************************************
//	リストデータ取得
//	【引数】	CDZmSub*	…	DB財務クラスのポインタ
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::GetListData( CDBZmSub* ZmSub )
{
	long			nRet = 0;			//	戻値
	LINP_DATA		udData[ID_TEKIYO_LIST_MAX];	//	リストデータ構造体
	int				nCnt;				//	カウント用
	int				nTCnt;				//	摘要取得カウント用
	long			nLine = 0;			//	行管理用
	long			lngCnt = 1;			//	カウント用
	int				intMatchIndex = -1;	//	引数として取得した摘要コードと一致したインデックスを格納
	int				intNearIndex = -1;	//	引数として取得した摘要コード付近のコードのインデックスを格納
	BOOL			blnFlag = FALSE;	//  指定摘要コード付近インデックス判断フラグ
	CString			strFilter;			//	クエリ文

	//	リストデータ初期化
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}
	
	//	リストクリア
	m_lstTekiyo.RemoveAll();

	// 摘要名をコード順で取得
	strFilter = _T("");
	if ( ZmSub->KanaTekiyoOpen( strFilter , 0 ) == 0 ){

		// 先頭に移動
		ZmSub->tkrec->MoveFirst();

		//	レコード終端までループ
		for( nTCnt = 0; nTCnt < ZmSub->tkrec->GetRecordCount(); nTCnt++ ){
		
			//	リストに1行追加
			m_lstTekiyo.AddNewLine( TRUE );
			
			for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
				//	背景色変更
				udData[nCnt].dat_fc = PALETTERGB( 0x00, 0x00, 0x00 );	//	文字色
				udData[nCnt].dat_bc = PALETTERGB( 0xA6, 0XCA, 0xF0 );	//	背景色
				udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	属性
			}

			//	摘要コードを設定（非表示項目）
			udData[0].dat_string.Format("%d", ZmSub->tkrec->tkcod);
			m_lstTekiyo.SetColumnData( ( short )nLine, ID_TEKIYO_LIST_TKCOD, ( LPUNKNOWN )udData, TRUE );
			// 指定摘要コードと一致?
			if ( m_nTkcod == ZmSub->tkrec->tkcod ){
				intMatchIndex = nTCnt;
			}
			// 指定摘要コードを超えた?
			else if ( m_nTkcod < ZmSub->tkrec->tkcod ) {
				if ( nTCnt > 0 ){
					if ( blnFlag == FALSE ){
						intNearIndex = nTCnt;
						blnFlag = TRUE;
					}
				}
			}

			//	順序番号を設定
			udData[1].dat_string.Format("%d", ZmSub->tkrec->tkcod);
			m_lstTekiyo.SetColumnData( ( short )nLine, ID_TEKIYO_LIST_NO, ( LPUNKNOWN )udData, TRUE );
			lngCnt++;

			//	摘要名を設定
			udData[2].dat_string = ZmSub->tkrec->tkname;
			m_lstTekiyo.SetColumnData( ( short )nLine, ID_TEKIYO_LIST_TKNAME, ( LPUNKNOWN )udData, TRUE );
			
			ZmSub->tkrec->MoveNext();	//	次のレコードに移動
			nLine++;			//	行をインクリメント
		}
		// 閉じる
		ZmSub->KanaTekiyoClose();

		// 最初にフォーカス位置を移すインデックスを決定
		if ( intMatchIndex >= 0 ){
			// 一致した摘要コードがあればその位置にフォーカス
			m_nStartIndex = intMatchIndex;
		}
		else{
			if ( intNearIndex >= 0 ){
				// 指定摘要コード付近にフォーカス
				m_nStartIndex = ( intNearIndex - 1);
			}
			else{
				// 指定摘要コードに一致もせず以上のコードもない
				// 場合は最終位置にフォーカス
				m_nStartIndex = ( nTCnt - 1 );
			}
		}
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( m_pZmSub );
		return;
	}
}

//**************************************************
//	賞味のリストインデックス取得
//	【引数】	なし
//	【戻値】	賞味のインデックス
//**************************************************
long CdlgRenTekiyo::GetListIndex()
{
	long	nRet = 0;		//	戻値
	long	nIndex = 0;		//	頁内でのインデックス
	long	nTop = 0;		//	頁のトップインデックス
	
	//	頁のトップインデックス取得
	nTop = m_lstTekiyo.GetTopIndex();
	//	頁内でのインデックス取得
	nIndex = m_lstTekiyo.GetIndex();
	//	賞味のインデックス取得
	nRet = nTop + nIndex;

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	位置移動
//	【引数】	nTop	…	トップインデックス（※4/28未使用）
//				nIndex	…	賞味のインデックス
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::MovePosition( long nTop, long nIndex )
{
	//	インデックスが範囲外？
	if (( nIndex < 0 ) || ( nIndex > ( m_lstTekiyo.GetCount() - 1 ) )){
		//	先頭にする
		nIndex = 0;
	}

	//	カーソル移動
	m_lstTekiyo.SetInputPositionEX( ( short )nIndex, 0 );
	//	現在行確認
	CheckNowRowPosition();
}

//**************************************************
//	リストからシーケンス番号取得
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	0以外	…	摘要コード
//				0		…	失敗
//**************************************************
long CdlgRenTekiyo::GetTekiyoCode( int nIndex )
{
	
	long		nRet = 0;			//	戻値
	LINP_DATA	udData;				//	リストデータ構造体
	
	//	初期化
	InitLinpData( &udData );
	//	指定カラムのデータを取得
	m_lstTekiyo.GetColumnData( nIndex, ID_TEKIYO_LIST_TKCOD, ( LPUNKNOWN )&udData );
	//	シーケンス番号を数値に変換
	nRet = atol( udData.dat_string );
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	LINP_DATA構造体初期化
//	【引数】	pData	…	LINP_DATA構造体ポインタ
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::InitLinpData( LINP_DATA* pData )
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
//	現在行確認
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::CheckNowRowPosition()
{
	long	nIndex;		//	インデックス

	//	賞味のインデックス取得
	nIndex = GetListIndex();
	//	行の背景色を更新
	ReDrawRowColor();
	//	前回選択行取得
	m_nOldRow = nIndex;
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				nFont	…	文字色
//				nBack	…	背景色
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::SetColor( int nIndex, COLORREF nFont, COLORREF nBack )
{
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のデータを取得
	m_lstTekiyo.GetData( nIndex, ( LPUNKNOWN )udData );
	
	//	カラム最大数分ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){ 
		udData[nCnt].dat_bc = nBack;	//	背景色を変更
		udData[nCnt].dat_fc = nFont;	//	文字色を変更
	}
	
	//	現在選択されている行のデータを設定
	m_lstTekiyo.SetData( nIndex, ( LPUNKNOWN )udData, TRUE );
}

//**************************************************
//	行の背景色を更新
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenTekiyo::ReDrawRowColor()
{
	long	nIndex;		//	インデックス
	long	nMax;		//	リスト最大登録数
	int		nFont;
	int		nBack;
	
	//	賞味のインデックス取得
	nIndex = GetListIndex();
	//	リスト最大登録数
	nMax = m_lstTekiyo.GetCount();
	
	//	前回選択行と現在選択行が違う？
	if ( nIndex != m_nOldRow ){
		
		//	前回選択行あり？
		if ( m_nOldRow >= 0 ){
			
			nFont = PALETTERGB( 0x00, 0x00, 0x00 );
			nBack = PALETTERGB( 0xA6, 0XCA, 0xF0 );
					
			//	前回選択行が範囲内？
			if (( m_nOldRow >= 0 ) && ( m_nOldRow < nMax )){
				//	前回選択行をクリア
				SetColor( m_nOldRow, nFont, nBack );
			}
		}
		
		//	現在選択行が範囲内？
		if (( nIndex >= 0 ) && ( nIndex < nMax )){
			//	現在選択行を選択色に変更
			SetColor( nIndex, GetSysColor( COLOR_WINDOWTEXT ), RGB( 127, 127, 127 ) );
		}
	}
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgRenTekiyo::GetFocusControlID( CWnd* pParent /*=NULL*/)
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
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pRenInfo		…	ICS会社情報構造体
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	実行ボタン押した
//				ID_DLG_CANCEL	…	キャンセルボタン押した
//**************************************************
int CdlgRenTekiyo::ShowDialog( long Tkcod )
{
	//m_pZmSub = NULL;		//  財務DBクラスハンドル
	m_nRet = ID_DLG_CANCEL;	//	戻値
	m_nColumnMax = 0;		//	最大カラム数
	m_nStartIndex = 0;		//	指定コードと一致するインデックス
	m_nOldRow = -1;			//	前回選択行
	m_Tkcod = 0;			// 指定摘要コード
	m_nTkcod = Tkcod;

	DoModal();					//	モーダルで表示
	
	return( m_nRet );
}
