// dlgRenKamoku.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgRenKamoku.h"

// 158145 add -->
extern	BOOL	bG_Kanso;
// 158145 add <--

// 改良No.21-0086,21-0529 add -->
extern	BOOL	bG_KmkAdd1;				// 様式⑧に科目追加に関するデータ追加をしたサイン
extern	BOOL	bG_KmkAdd2;				// 新規マスターとして様式②、⑯-2に「電子記録○○」等の科目を追加したサイン
// 改良No.21-0086,21-0529 add <--

// CdlgRenKamoku ダイアログ

IMPLEMENT_DYNAMIC(CdlgRenKamoku, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgRenKamoku::CdlgRenKamoku(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgRenKamoku::IDD, pParent)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgRenKamoku::~CdlgRenKamoku()
{
}

void CdlgRenKamoku::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
// 20-0451 add -->
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnReset);
// 20-0451 add <--
	DDX_Control(pDX, IDC_KAMOKU_LIST, m_lstKamoku);
}


BEGIN_MESSAGE_MAP(CdlgRenKamoku, ICSDialog)
	ON_WM_SIZE()
// 20-0451 add -->
	ON_BN_CLICKED(IDC_BUTTON_RESET,&CdlgRenKamoku::OnBnClickedButtonReset)
// 20-0451 add <--
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgRenKamoku, ICSDialog)
	ON_EVENT(CdlgRenKamoku, IDC_KAMOKU_LIST, 10, CdlgRenKamoku::SelChangeKamokuList, VTS_I2)
	ON_EVENT(CdlgRenKamoku, IDC_KAMOKU_LIST, 6, CdlgRenKamoku::LButtonClickKamokuList, VTS_I2)
	ON_EVENT(CdlgRenKamoku, IDC_KAMOKU_LIST, 9, CdlgRenKamoku::FocusedKamokuList, VTS_NONE)
END_EVENTSINK_MAP()

void CdlgRenKamoku::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamoku::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 財務DBクラスを取得
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	//	ツールバーの設定成功？
	if ( SetToolBar() == 0 ){
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_FAKE, FALSE );
	}
	// リスト初期化
	InitList();
	//	リストデータ取得
	GetListData();
	//	位置移動
	MovePosition( 0, 0 );
	// リサイズ
	OnInitDialogEX();

// 158145 add -->
	if(bG_Kanso == FALSE)	{
		m_btnReset.EnableWindow(FALSE);
		m_btnReset.ShowWindow(SW_HIDE);
	}
// 158145 add <--

	//	戻値を返す
	return( TRUE ); 
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamoku::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamoku::PreTranslateMessage(MSG* pMsg)
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
			if ( GetFocusControlID( this ) == IDC_KAMOKU_LIST ){
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
		//	F7キー
		case VK_F7:
			nID = ID_TOOL_BUTTON_F7;
			break;
		//	F9キー
		case VK_F9:
			nID = ID_TOOL_BUTTON_F9;
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
//	メッセージ マップを処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamoku::OnCommand(WPARAM wParam, LPARAM lParam)
{
// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	//	パラメータで分岐
	switch( wParam ){
	//	[F2]
	case ID_TOOL_BUTTON_F2:
		PostMessage( WM_KEYDOWN, VK_F2 );
		return( 1 );
		break;
	//	[F7]追加
	case ID_TOOL_BUTTON_F7:
		OnToolButtonF7();
		break;
	//	[F9]追加
	case ID_TOOL_BUTTON_F9:
		OnToolButtonF9();
		break;
	}

	return ICSDialog::OnCommand(wParam, lParam);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	セルが移動した
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamoku::SelChangeKamokuList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//	現在行確認
	CheckNowRowPosition();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（シングル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamoku::LButtonClickKamokuList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//	現在行確認
	CheckNowRowPosition();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	フォーカス（来た）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamoku::FocusedKamokuList()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//	前回選択行あり？
	if ( m_nOldRow >= 0 ){
		//	行の背景色を更新
		ReDrawRowColor();
	}
}

//**************************************************
//	F7（削除）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::OnToolButtonF7() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	int			nRet = 0;		//	戻値
	CString		szMessage;		//	メッセージ

	// 初期化
	szMessage.Empty(); 

	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F7) != 0)	{
		// メッセージ設定
		szMessage = "削除してもよろしいですか？";
		
		// 削除確認OK？
		if(ICSMessageBox(szMessage, MB_OKCANCEL | MB_DEFBUTTON2, 0, 0, this) == IDOK)	{
			// レコードの削除
			nRet = SetRecordDelete();

			// デフォルト科目？
			if(nRet == -2)	{
				// 削除不可をアナウンス
				//ICSMessageBox( _T( "規定科目なので削除できません。" ), MB_OK, 0, 0, this );	// 改良No.23-0513 del
				ICSMessageBox(_T("既定の科目は削除できません。"), MB_OK, 0, 0, this);			// 改良No.23-0513 add
			}
		}
	}
}

//**************************************************
//	F9（追加）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::OnToolButtonF9() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CdbUcRenKamoku	mfcRenRec( m_pDB );
	CdbUcLstKamoku	mfcLstRec( m_pDB );

	CdlgRenKamokuAdd dlgAdd(this);
	CdlgRenKamokuSelect dlgSelect(this);

	CString			strkcod = "";	// 科目コード
	CString			strknam = "";	// 科目名
	CString			strknamold = "";// 位置検索用
	long			lngknSeq = 0;	// 内訳書科目シーケンス番号
	long			lngSeq = 0;		// シーケンス番号
	long			lngCnt = 0;		// カウント用

	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F9 ) != 0 ){
		// 財務科目選択ダイアログ表示
		if ( dlgAdd.ShowDialog( m_pDB , m_pRenInfo , m_nFormSeq ) == ID_DLG_OK ){
			// 内訳書科目選択ダイアログ表示
// 158016 del -->
			//if ( dlgSelect.ShowDialog( m_pDB , m_nFormSeq , dlgAdd.m_Knnam, dlgAdd.m_Knkana ) == ID_DLG_OK ){
// 158016 del <--
// 158016 add -->
			if ( dlgSelect.ShowDialog( m_pDB , m_nFormSeq , dlgAdd.m_Knnam, dlgAdd.m_Knkana, dlgAdd.m_Kncod ) == ID_DLG_OK ){
// 158016 add <--
				// 内訳書科目取得?
				if ( dlgSelect.m_KnSeq > 0 ){
					try{
						// 新規レコード追加
						mfcRenRec.CreateNewRecord( m_nFormSeq , m_pRenInfo->CoType );					// レコード初期化済み

						// 特殊行データへ更新
						mfcRenRec.Edit();
						mfcRenRec.m_KcdStart = dlgAdd.m_Kncod;		// 財務科目コード取得
						mfcRenRec.m_KnSeq = dlgSelect.m_KnSeq;		// 内訳書科目シーケンス保存
						mfcRenRec.Update();
					}
					catch(...){
						//	レコード開いてる？
						if ( mfcRenRec.IsOpen() ){
							//	レコード閉じる
							mfcRenRec.Fin();
						}
					}
				}
				// 更新
				GetListData();
				// 追加した行にフォーカスを移す
				for ( lngCnt = 0; lngCnt < m_lstKamoku.GetCount(); lngCnt++ ){
					if ( mfcRenRec.m_Seq == GetSeq( lngCnt ) ){
						// 位置確定
						MovePosition( m_lstKamoku.GetTopIndex() , lngCnt );
					}
				}
			}
			// 新規内訳書科目追加確認フラグ格納
			m_UcKmkEntry = dlgSelect.m_UcKmkEntry;
		}
	}
	//	レコード閉じる
	mfcRenRec.Fin();
}

//**************************************************
// ツールバーの設定
//	【引数】	なし
//	【戻値】	0		…	成功
//				0以外	…	失敗
//**************************************************
int CdlgRenKamoku::SetToolBar()
{
	CRect	ClientStart;	//	クライアント開始矩形
	CRect	ClientNow;		//	クライアント現在矩形
	CPoint	Offset;			//	オフセット位置
	CRect	Child;			//	子ウィンドウ矩形
	CWnd*	pChild = NULL;	//	子ウィンドウハンドル
	CRect	Window;			//	ウィンドウ矩形
	
	//	ツールバー作成
	if ( m_tbrToolBar.Create( this ) == 0 ){
		//	エラー
		return( -1 );
	}
	
	//	ビットマップ読み込み
	if ( m_tbrToolBar.LoadToolBar( IDR_RENKAMOKU_BAR ) == 0 ){
		//	エラー
		return( -2 );
	}
	
	//------------------------------
	//	スタイル設定
	//		CBRS_TOOLTIPS	…	ツールチップ表示
	//		CBRS_FLYBY		…	メッセージテキストの更新
	//		CBRS_BORDER_ANY	…	フレームを任意に設定
	//------------------------------
	m_tbrToolBar.SetBarStyle( m_tbrToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_BORDER_ANY );
	//	拡張スタイルをフラットツールバーに設定
	m_tbrToolBar.ModifyStyle( 0, 0x0800 );
/*
	//	クライアント領域の矩形情報取得
	GetClientRect( ClientStart );
	//	クライアント領域での再配置/サイズ変更
	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, ClientNow );	
	//	オフセット位置を取得
	Offset.x = ClientNow.left - ClientStart.left;
	Offset.y = ClientNow.top - ClientStart.top;
	
	//	最初の子ウィンドウ取得
	pChild = GetWindow( GW_CHILD );
	
	//	ハンドルがヌルになるまでループ
	while ( pChild != NULL ){

		//	矩形情報取得
		pChild->GetWindowRect( Child );
		//	スクリーン座標からクライアント座標に置き換え
		ScreenToClient( Child );
		//	指定オフセット分移動
		Child.OffsetRect( Offset );
		//	位置と寸法の変更
		pChild->MoveWindow( Child, FALSE );
		//	次のウィンドウ取得
		pChild = pChild->GetNextWindow();
	}
	
	//	ウィンドウの矩形情報取得
	GetWindowRect( Window );
	//	ウィンドウの右下隅の位置を修正
	Window.right += ClientStart.Width() - ClientNow.Width();
	Window.bottom += ClientStart.Height() - ClientNow.Height();
	//	位置と寸法の変更
	MoveWindow( Window, FALSE );
*/
	//	クライアント領域での再配置/サイズ変更
	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0 );
	
	//	成功
	return( 0 );
}

//**************************************************
// リスト初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::InitList(){
	int					nCnt;			//	カウント用
	LINP_COLUMN			udColumn[ID_KAMOKU_LIST_MAX + 2];	//	カラム構造体
	LINP_VCOLORINFO		udGrid[ID_KAMOKU_LIST_MAX + 2];		//	縦グリッド色構造体
	int					nColumnCnt = 0;	//	カラムカウント数

	
	//	最大カラム数
	m_nColumnMax = ID_KAMOKU_LIST_MAX;

	//------------------------------
	//	0カラム目（非表示）
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 4;															//	入力最大桁数
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;						//	属性（※フォーカス移動のため読み取り属性を外す）
	udColumn[nColumnCnt].clm_width = 0;															//	幅（DLU）
	udColumn[nColumnCnt].clm_title = _T( "シーケンス番号" );									//	タイトル
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//------------------------------
	//	1カラム目
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 4;															//	入力最大桁数
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
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 4;															//	入力最大桁数
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;	//	属性
	udColumn[nColumnCnt].clm_width = 20;														//	幅（DLU）
	udColumn[nColumnCnt].clm_title = _T( "区分" );												//	タイトル
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
	//	カラムカウント数インクリメント
	nColumnCnt++;


	//------------------------------
	//	3カラム目
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
	udColumn[nColumnCnt].clm_max = 12;
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	udColumn[nColumnCnt].clm_width = ( 21 * 6 ) - 2;
	udColumn[nColumnCnt].clm_title = _T( "財務科目" );
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//------------------------------
	//	4カラム目
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
	udColumn[nColumnCnt].clm_max = 12;
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	udColumn[nColumnCnt].clm_width = ( 12 * 8 ) + 2;
	udColumn[nColumnCnt].clm_title = _T( "内訳書科目" );
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
	//	カラムカウント数インクリメント
	nColumnCnt++;
	
	//	カラムの設定
	m_lstKamoku.SetColumn( m_nColumnMax, ( LPUNKNOWN )udColumn );
	//	横グリッド線設定
	m_lstKamoku.SetHorzGrid( 5, RGB( 0x00, 0xFF, 0xFF), LINP_GRID_SOLID );

	//	縦グリッド線数文ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
		udGrid[nCnt].pos = nCnt;						//	縦グリッド線を引くカラムのオフセット
		udGrid[nCnt].color = RGB( 0xFF, 0x00, 0x00 );	//	色
		udGrid[nCnt].type = LINP_GRID_SOLID;			//	線種
	}
	
	//	縦グリッド線設定
	m_lstKamoku.SetVartGrid( m_nColumnMax, ( LPUNKNOWN )udGrid );
}

//**************************************************
//	リストデータ取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nType		…	参照型
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::GetListData()
{
	long			nRet = 0;			//	戻値
	CdbUcRenKamoku	mfcRec( m_pDB );				//	連動科目一覧テーブル
	LINP_DATA		udData[ID_KAMOKU_LIST_MAX];			//	リストデータ構造体
	int				nCnt;				//	カウント用
	int				intKubun = 0;		//	区分
	char			szBuf[8];			//	文字列変換用バッファ
	long			nLine = 0;			//	行管理用
	long			lngCnt = 1;				//	カウント用
		
	//	リストデータ初期化
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
		InitLinpData( &udData[nCnt] );
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}
	
	//	リストクリア
	m_lstKamoku.RemoveAll();

	try{
		
		//	初期化成功？
		if ( mfcRec.RequeryFormSeqFgCoType( m_nFormSeq , m_pRenInfo->CoType , 0 ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();		//	レコードを先頭に移動
				
				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					
					// 科目有り or 科目有効の場合表示
					if ( GetKamokuEnable( m_pZmSub , mfcRec.m_KcdStart ) == TRUE ){

						//	リストに1行追加
						m_lstKamoku.AddNewLine( TRUE );

						//	デフォルト科目？
						if ( mfcRec.m_FgDft == 1 ){
							
							for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
								//	背景色変更
								udData[nCnt].dat_fc = PALETTERGB( 0x00, 0x00, 0x00 );	//	文字色
								udData[nCnt].dat_bc = PALETTERGB( 0xA6, 0XCA, 0xF0 );	//	背景色
								udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	属性
							}
						}
						else{
							//	背景色初期化
							for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
								udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
								udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
								udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
							}
						}

						//	シーケンス番号を設定（非表示項目）
						ZeroMemory( szBuf, sizeof( szBuf ) );
						_itoa_s( mfcRec.m_Seq, szBuf, 10 );
						udData[0].dat_string = szBuf;
						m_lstKamoku.SetColumnData( ( short )nLine, ID_KAMOKU_LIST_SEQ, ( LPUNKNOWN )udData, TRUE );

						//	順序番号を設定
						ZeroMemory( szBuf, sizeof( szBuf ) );
						_itoa_s( lngCnt, szBuf, 10 );
						udData[1].dat_string = szBuf;
						m_lstKamoku.SetColumnData( ( short )nLine, ID_KAMOKU_LIST_NO, ( LPUNKNOWN )udData, TRUE );
						lngCnt++;

						// 区分を設定
						intKubun = m_clsFunc.GetKcdKubun( mfcRec.m_KcdStart );
						if ( intKubun == 1 ){
							udData[2].dat_string = "★";
						}
						else{
							udData[2].dat_string = "";
						}
						m_lstKamoku.SetColumnData( ( short )nLine, ID_KAMOKU_LIST_KUBUN , ( LPUNKNOWN )udData , TRUE );

						//	財務科目名を設定
						udData[3].dat_string = GetKamokuName( m_pZmSub , mfcRec.m_KcdStart );
						m_lstKamoku.SetColumnData( ( short )nLine, ID_KAMOKU_LIST_ZKAMOKU, ( LPUNKNOWN )udData, TRUE );
						
						//	内訳書科目名を設定
						udData[4].dat_string = 	GetKamokuRecord( mfcRec.m_KnSeq );
						m_lstKamoku.SetColumnData( ( short )nLine, ID_KAMOKU_LIST_UKAMOKU, ( LPUNKNOWN )udData, TRUE );
						
						nLine++;			//	行をインクリメント
					}
					mfcRec.MoveNext();	//	次のレコードに移動

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
	
	//	戻値で分岐
	switch( nLine ){
	//	0行
	case 0:
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, FALSE );	//	[F7]削除	×
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F9, TRUE );	//	[F9]追加	×
		break;
	default:
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );	//	[F7]削除	○
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F9, TRUE );	//	[F9]追加	○
		break;
	}
}

//**************************************************
//	賞味のリストインデックス取得
//	【引数】	なし
//	【戻値】	賞味のインデックス
//**************************************************
long CdlgRenKamoku::GetListIndex()
{
	long	nRet = 0;		//	戻値
	long	nIndex = 0;		//	頁内でのインデックス
	long	nTop = 0;		//	頁のトップインデックス
	
	//	頁のトップインデックス取得
	nTop = m_lstKamoku.GetTopIndex();
	//	頁内でのインデックス取得
	nIndex = m_lstKamoku.GetIndex();
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
void CdlgRenKamoku::MovePosition( long nTop, long nIndex )
{
	//	インデックスが範囲外？
	if (( nIndex < 0 ) || ( nIndex > ( m_lstKamoku.GetCount() - 1 ) )){
		//	先頭にする
		nIndex = 0;
	}

	//	カーソル移動
	m_lstKamoku.SetInputPositionEX( ( short )nIndex, 0 );
	//	現在行確認
	CheckNowRowPosition();
}

//**************************************************
//	科目のレコード取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nKnSeq		…	シーケンス番号
//	【戻値】	文字列		…	成功
//				""			…	失敗
//**************************************************
CString CdlgRenKamoku::GetKamokuRecord( long nKnSeq )
{
	CString			strRet = "";		//	戻値
	CdbUcLstKamoku	mfcRec( m_pDB );		//	uc_lst_bankテーブルクラス
	
	try{
		//	指定シーケンス番号のレコード取得成功？
		if ( mfcRec.RequeryKnSeq( m_nFormSeq, nKnSeq ) == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();							//	レコードを先頭に移動
				strRet = mfcRec.m_KnName;					//	戻値に取得科目名を設定
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

	//	戻値を返す
	return( strRet );
}

//**************************************************
//	リストからシーケンス番号取得
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	0以外	…	シーケンス番号
//				0		…	失敗
//**************************************************
long CdlgRenKamoku::GetSeq( int nIndex )
{
	
	long		nRet = 0;			//	戻値
	LINP_DATA	udData;				//	リストデータ構造体
	
	//	初期化
	InitLinpData( &udData );
	//	指定カラムのデータを取得
	m_lstKamoku.GetColumnData( nIndex, ID_KAMOKU_LIST_SEQ, ( LPUNKNOWN )&udData );
	//	シーケンス番号を数値に変換
	nRet = atoi( udData.dat_string );
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	LINP_DATA構造体初期化
//	【引数】	pData	…	LINP_DATA構造体ポインタ
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::InitLinpData( LINP_DATA* pData )
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
void CdlgRenKamoku::CheckNowRowPosition()
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
void CdlgRenKamoku::SetColor( int nIndex, COLORREF nFont, COLORREF nBack )
{
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のデータを取得
	m_lstKamoku.GetData( nIndex, ( LPUNKNOWN )udData );
	
	//	カラム最大数分ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){ 
		udData[nCnt].dat_bc = nBack;	//	背景色を変更
		udData[nCnt].dat_fc = nFont;	//	文字色を変更
	}
	
	//	現在選択されている行のデータを設定
	m_lstKamoku.SetData( nIndex, ( LPUNKNOWN )udData, TRUE );
}

//**************************************************
//	行の背景色を更新
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::ReDrawRowColor()
{
	long	nIndex;		//	インデックス
	long	nMax;		//	リスト最大登録数
	int		nFont;
	int		nBack;
	
	//	賞味のインデックス取得
	nIndex = GetListIndex();
	//	リスト最大登録数
	nMax = m_lstKamoku.GetCount();
	
	//	前回選択行と現在選択行が違う？
	if ( nIndex != m_nOldRow ){
		
		//	前回選択行あり？
		if ( m_nOldRow >= 0 ){
			
			//	科目で且つデフォルト項目？
			if ( GetFgDft( m_nOldRow ) == 1 ){
				nFont = PALETTERGB( 0x00, 0x00, 0x00 );
				nBack = PALETTERGB( 0xA6, 0XCA, 0xF0 );
			}
			//	以外
			else{
				nFont = GetSysColor( COLOR_WINDOWTEXT );
				nBack = GetSysColor( COLOR_WINDOW );
			}
			
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
//	リストからデフォルトフラグ取得
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	1		…	デフォルトフラグ
//				0以外	…	失敗
//**************************************************
long CdlgRenKamoku::GetFgDft( int nIndex )
{
	
	long			nRet = 0;			//	戻値
	long			nSeq = 0;			//	取得シーケンス番号
	LINP_DATA		udData;				//	リストデータ構造体
	CdbUcRenKamoku	mfcRec( m_pDB );	//	連動科目一覧テーブル

	//	初期化
	InitLinpData( &udData );
	//	指定カラムのデータを取得
	m_lstKamoku.GetColumnData( nIndex, ID_KAMOKU_LIST_SEQ, ( LPUNKNOWN )&udData );
	//	シーケンス番号を数値に変換
	nSeq = atoi( udData.dat_string );

	try{
		
		//	初期化成功？
		if ( mfcRec.RequerySeq( nSeq ) == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				// デフォルトフラグ取得
				nRet = mfcRec.m_FgDft;
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
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgRenKamoku::GetFocusControlID( CWnd* pParent /*=NULL*/)
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
//	レコードの削除
//	【引数】	なし
//	【戻値】	0				…	編集なし
//				0以外			…	編集あり
//**************************************************
long CdlgRenKamoku::SetRecordDelete()
{
	long			nRet = -1;			//	戻値
	long			nTopIndex = 0;		//	トップインデックス
	long			nIndex = 0;			//	現在のインデックス
	long			nKnSeq = 0;			//	シーケンス番号
	long			nMoveIndex = 0;		//	削除時のインデックス移動先
	CdbUcRenKamoku	mfcRec(m_pDB);		//	連動科目一覧テーブル

	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nKnSeq = GetSeq( nIndex );
	
	try{
		//	初期化成功？
		if ( mfcRec.RequerySeq( nKnSeq ) == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();			//	レコードを先頭に移動
// 158145 del -->
//// 20-0451 del -->
//				//if ( mfcRec.m_FgDft == 0 ){
//				//	mfcRec.Delete();		//	削除
//				//	nRet = 0;
//				//}
//				//else{
//				//	nRet = -2;
//				//}
//// 20-0451 del <--
//// 20-0451 add -->
//				mfcRec.Delete();		//	削除
//				nRet = 0;
//// 20-0451 add <--
// 158145 del <--
// 158145 add -->
				if(bG_Kanso != FALSE)	{
					mfcRec.Delete();		//	削除
					nRet = 0;
				}
				else	{
					if(mfcRec.m_FgDft == 0)	{
						mfcRec.Delete();	//	削除
						nRet = 0;
					}
					else	{
						nRet = -2;
					}
				}
// 158145 add <--
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){
		
		//	レコード開いている？
		if ( mfcRec.IsOpen() ){
			mfcRec.Fin();	//	レコード閉じる
		}
	}
	
	//	削除された？
	if ( nRet >= 0 ){
		//	前回選択行をクリア
		m_nOldRow = -1;
		//	リストのトップインデックスを保持
		nTopIndex = m_lstKamoku.GetTopIndex();
		//	リストの更新
		GetListData();
		//	位置移動
		if ( nIndex >= m_lstKamoku.GetCount() ){
			nMoveIndex = m_lstKamoku.GetCount() - 1;
		}
		else{
			nMoveIndex = nIndex;
		}
		MovePosition( nTopIndex, nMoveIndex );
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	対象科目が有効科目かどうか判定
//	【引数】	ZmSub				…	DB財務クラスのポインタ
//				strkncod			…	判定する科目コード
//	【戻値】	TRUE				…	科目あり or 科目有効
//				FALSE				…	科目無し or 科目無効
//**************************************************
BOOL CdlgRenKamoku::GetKamokuEnable( CDBZmSub* ZmSub , CString strkncod )
{

	CString	strKnFilter = "";	// SQLクエリ文(科目名称用)
	CString strGkFilter = "";	// SQLクエリ文(合計名称用)	
	BOOL	blnRet = FALSE;	// 取得フラグ

	// 科目コードがNULLなら抜ける
	if ( strkncod != "" ){
		// クエリ文作成
		strKnFilter.Format( "kncod='%s'",strkncod );
		strGkFilter.Format( "gkcod='%s'",strkncod );

		// クエリ文に仕様不可科目除外クエリを加える
		strKnFilter += _T(" AND (knvoid & 0x01) = 0");
		strGkFilter += _T(" AND (gkvoid & 0x01) = 0");

		// 科目名称テーブルから検索
		if ( ZmSub->KamokuMeisyoOpen( strKnFilter , 0 ) == 0 ){
			if ( ZmSub->knrec->GetRecordCount() > 0 ){
				blnRet = TRUE;
			}
			// 閉じる
			ZmSub->KamokuMeisyoClose();
		}
		else{
			// 2007.03 DBエラー対応
			m_clsFunc.ZmSubErrMsg( ZmSub );
			return FALSE;
		}

		// 科目名称取得に成功していれば抜ける
		if ( blnRet == TRUE ){
			return( blnRet );
		}

		// 合計名称テーブルから検索
		if ( ZmSub->GokeiMeisyoOpen( strGkFilter , 0 ) == 0 ){
			if ( ZmSub->gkrec->GetRecordCount() > 0 ){
				blnRet = TRUE;
			}
			// 閉じる
			ZmSub->GokeiMeisyoClose();
		}
		else{
			// 2007.03 DBエラー対応
			m_clsFunc.ZmSubErrMsg( ZmSub );
			return FALSE;
		}

	}

	return( blnRet );

}

//**************************************************
//	科目名称・合計名称テーブルから科目名取得
//	【引数】	strkncod			…	データベースハンドル
//	【戻値】	科目名称文字列		…	成功
//				""					…	失敗
//**************************************************
CString CdlgRenKamoku::GetKamokuName( CDBZmSub* ZmSub , CString strkncod )
{

	CString	strKnFilter = "";	// SQLクエリ文(科目名称用)
	CString strGkFilter = "";	// SQLクエリ文(合計名称用)	
	CString	strRet = "";		// 科目名称(戻り値)
	BOOL	blnFlag = FALSE;	// 取得フラグ

	// 科目コードがNULLなら抜ける
	if ( strkncod != "" ){
		// クエリ文作成
		strKnFilter.Format( "kncod='%s'",strkncod );
		strGkFilter.Format( "gkcod='%s'",strkncod );

		// 科目名称テーブルから検索
		if ( ZmSub->KamokuMeisyoOpen( strKnFilter , 0 ) == 0 ){
			if ( ZmSub->knrec->GetRecordCount() > 0 ){
				strRet = ZmSub->knrec->knnam;
				blnFlag = TRUE;
			}
			// 閉じる
			ZmSub->KamokuMeisyoClose();
		}
		else{
			// 2007.03 DBエラー対応
			m_clsFunc.ZmSubErrMsg( ZmSub );
		}


		// 科目名称取得に成功していれば抜ける
		if ( blnFlag == TRUE ){
			return( strRet );
		}

		// 合計名称テーブルから検索
		if ( ZmSub->GokeiMeisyoOpen( strGkFilter , 0 ) == 0 ){
			if ( ZmSub->gkrec->GetRecordCount() > 0 ){
				strRet = ZmSub->gkrec->gknam;
			}
			// 閉じる
			ZmSub->GokeiMeisyoClose();
		}
		else{
			// 2007.03 DBエラー対応
			m_clsFunc.ZmSubErrMsg( ZmSub );
		}

	}

	return( strRet );

}

//**************************************************
//	科目選択ダイアログから科目コードを取得
//	【引数】	ZmSub				…	DB財務クラスのハンドル
//	【戻値】	科目コード文字列	…	取得成功
//				""					…	取得失敗
//**************************************************
/*CString CdlgRenKamoku::GetKamokuCode( CDBZmSub* ZmSub )
{
	KSEL_PAR	Kpar;			// 科目選択ダイアログプロパティ
	CString		strRet = "";	// 戻り値
	RECT rc;					// 科目選択ダイアログのサイズ

	// 科目選択ダイアログ表示の前処理
	if( ZmSub->KamokuMeisyoOpen( "", 0 ) == -1 ) {
		return strRet;
	}

	// コントロールのハンドル取得
	//this->m_pWnd->GetWindowRect( &rc );
	this->m_pWnd->GetWindowRect( &rc );
	this->m_pWnd->GetClientRect( &rc );

	// 科目選択ダイアログのプロパティを設定

	ZeroMemory( &Kpar, sizeof( KSEL_PAR ));
	Kpar.ksPsgn		= KSP_CENTERCENTER;		// ポジション・サイン
	Kpar.ksFsgn		= KSF_ABSDISP;		// ファンクション・サイン
	Kpar.ksLnum		= 10;				// 表示科目数
	Kpar.ksPos.x	= rc.right;
	Kpar.ksPos.y	= rc.bottom;		// ダイアログ表示ポジション（スクリーン座標）
	
	// 科目選択ダイアログ表示
	if( Kmk50onSelect( ZmSub, &Kpar, this ) != FALSE ) {
		if( Kpar.ksStat == 0 ) {
			strRet = ZmSub->knrec->kncod;
		}
	}
	
	// 閉じる
	ZmSub->KamokuMeisyoClose();

	return strRet;

}*/

//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pRenInfo		…	ICS会社情報構造体
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	実行ボタン押した
//				ID_DLG_CANCEL	…	キャンセルボタン押した
//**************************************************
int CdlgRenKamoku::ShowDialog( CDatabase* pDB , RENZVOL_INFO* pRenInfo , int nFormSeq )
{

	m_pDB = NULL;
	m_nFormSeq = ID_FORMNO_011;
	m_nRet = ID_DLG_CANCEL;
	m_nOldRow = -1;
	m_UcKmkEntry = 0;

	//	データベースハンドルヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_pRenInfo = pRenInfo;		//  会社情報取得
				m_nFormSeq = nFormSeq;		//	様式番号取得
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	m_nRet = ID_DLG_OK;
	return( m_nRet );

}

// 20-0451 add -->
//**************************************************
//	初期化
//	【引数】	nFormSeq		…	様式シーケンス番号
//				nCoType			…	業種No
//	【戻値】	なし
//**************************************************
void CdlgRenKamoku::RecordReset( int nFormSeq, BYTE nCoType )
{
	CString				cs=_T("");
// 改良No.21-0086,21-0529 add -->
	CString				sData=_T("");
	CRecordset			rs(m_pZmSub->m_database);
	CdbUcLstKamoku		mfcRec(m_pDB);
	CdbUcRenKamoku		mfcRenRec(m_pDB);			// uc_ren_kamokuテーブルクラス
// 改良No.21-0086,21-0529 add <--

	switch(nCoType & 0xff)	{
	// 法人
	case	0x00:
		switch(nFormSeq)	{
		case	ID_FORMNO_011:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (101, 1, 0, '01020100', 101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (102, 1, 0, '01020200', 102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (103, 1, 0, '01030200', 103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (104, 1, 0, '01020300', 104, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (105, 1, 0, '01030100', 105, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (106, 1, 0, '01030300', 106, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (107, 1, 0, '01010100', 107, 1)\n");
			break;
		case	ID_FORMNO_021:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (201, 2, 0, '01040100', 201, 1)\n");
			break;
		case	ID_FORMNO_031:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (301, 3, 0, '01040200', 301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (302, 3, 0, '01070400', 302, 1)\n");
			break;
		case	ID_FORMNO_041:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (401, 4, 0, '01070500', 401, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (402, 4, 0, '01070100', 402, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (403, 4, 0, '01070600', 403, 1)\n");
			break;
		case	ID_FORMNO_042:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (501, 5, 0, '01070300', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (502, 5, 0, '02030400', 501, 1)\n");
			break;
		case	ID_FORMNO_081:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (901, 9, 0, '04010100', 901, 1)\n");
			break;
		case	ID_FORMNO_091:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1001, 10, 0, '04010200', 1001, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1002, 10, 0, '04030100', 1002, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1003, 10, 0, '04040100', 1003, 1)\n");
			break;
		case	ID_FORMNO_101:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1101, 11, 0, '04050400', 1101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1102, 11, 0, '04050100', 1102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1103, 11, 0, '04050300', 1103, 1)\n");
			break;
		case	ID_FORMNO_111:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1301, 13, 0, '04020100', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1302, 13, 0, '05010200', 1302, 1)\n");
			break;
		case	ID_FORMNO_142:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1701, 17, 0, '0A010100', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1702, 17, 0, '0A010200', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1703, 17, 0, '0A010300', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1704, 17, 0, '0A010400', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1705, 17, 0, '0A010500', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1706, 17, 0, '0A010600', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1707, 17, 0, '0A010700', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1708, 17, 0, '0F020100', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1709, 17, 0, '0F020200', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1710, 17, 0, '0F020300', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (1711, 17, 0, '0F020400', 1703, 1)\n");
			break;
		case	ID_FORMNO_161:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2101, 21, 0, '0B010B00', 2101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2102, 21, 0, '0B010100', 2102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2103, 21, 0, '0B010200', 2103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2104, 21, 0, '0C010100', 2104, 1)\n");
			break;
		case	ID_FORMNO_162:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2201, 22, 0, '0B020900', 2201, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2202, 22, 0, '0B020101', 2202, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2203, 22, 0, '0B020500', 2203, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (2204, 22, 0, '0C020100', 2204, 1)\n");
			break;
		}
		break;
	// 運送
	case	0x01:
		switch(nFormSeq)	{
		case	ID_FORMNO_011:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10101, 1, 1, '01020100', 101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10102, 1, 1, '01020200', 102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10103, 1, 1, '01030200', 103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10104, 1, 1, '01020300', 104, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10105, 1, 1, '01030100', 105, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10106, 1, 1, '01030300', 106, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10107, 1, 1, '01010100', 107, 1)\n");
			break;
		case	ID_FORMNO_021:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10201, 2, 1, '01040100', 201, 1)\n");
			break;
		case	ID_FORMNO_031:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10301, 3, 1, '01040200', 301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10302, 3, 1, '01070400', 302, 1)\n");
			break;
		case	ID_FORMNO_041:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10401, 4, 1, '01070500', 401, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10402, 4, 1, '01070100', 402, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10403, 4, 1, '01070600', 403, 1)\n");
			break;
		case	ID_FORMNO_042:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10501, 5, 1, '01070300', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10502, 5, 1, '02030400', 501, 1)\n");
			break;
		case	ID_FORMNO_081:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (10901, 9, 1, '04010100', 901, 1)\n");
			break;
		case	ID_FORMNO_091:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11001, 10, 1, '04010200', 1001, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11002, 10, 1, '04030100', 1002, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11003, 10, 1, '04040100', 1003, 1)\n");
			break;
		case	ID_FORMNO_101:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11101, 11, 1, '04050400', 1101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11102, 11, 1, '04050100', 1102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11103, 11, 1, '04050300', 1103, 1)\n");
			break;
		case	ID_FORMNO_111:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11301, 13, 1, '04020100', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11302, 13, 1, '05010200', 1302, 1)\n");
			break;
		case	ID_FORMNO_142:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11701, 17, 1, '0A010100', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11702, 17, 1, '0A010200', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11703, 17, 1, '0A010300', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11704, 17, 1, '0A010400', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11705, 17, 1, '0A010500', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11706, 17, 1, '0A010600', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11707, 17, 1, '0A010700', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11708, 17, 1, '0F020100', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11709, 17, 1, '0F020200', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11710, 17, 1, '0F020300', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (11711, 17, 1, '0F020400', 1703, 1)\n");
			break;
		case	ID_FORMNO_161:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12101, 21, 1, '0B010B00', 2101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12102, 21, 1, '0B010100', 2102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12103, 21, 1, '0B010200', 2103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12104, 21, 1, '0C010100', 2104, 1)\n");
			break;
		case	ID_FORMNO_162:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12201, 22, 1, '0B020900', 2201, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12202, 22, 1, '0B020101', 2202, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12203, 22, 1, '0B020500', 2203, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (12204, 22, 1, '0C020100', 2204, 1)\n");
			break;
		}
		break;
	// 建設
	case	0x20:
		switch(nFormSeq)	{
		case	ID_FORMNO_011:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320101, 1, 32, '01020100', 101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320102, 1, 32, '01020200', 102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320103, 1, 32, '01030200', 103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320104, 1, 32, '01020300', 104, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320105, 1, 32, '01030100', 105, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320106, 1, 32, '01030300', 106, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320107, 1, 32, '01010100', 107, 1)\n");
			break;
		case	ID_FORMNO_021:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320201, 2, 32, '01040100', 201, 1)\n");
			break;
		case	ID_FORMNO_031:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320301, 3, 32, '01040200', 301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320302, 3, 32, '01070400', 302, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320303, 3, 32, '01040300', 303, 1)\n");
			break;
		case	ID_FORMNO_041:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320401, 4, 32, '01070500', 401, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320402, 4, 32, '01070100', 402, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320403, 4, 32, '01070600', 403, 1)\n");
			break;
		case	ID_FORMNO_042:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320501, 5, 32, '01070300', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320502, 5, 32, '02030400', 501, 1)\n");
			break;
		case	ID_FORMNO_081:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (320901, 9, 32, '04010100', 901, 1)\n");
			break;
		case	ID_FORMNO_091:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321001, 10, 32, '04010200', 1001, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321002, 10, 32, '04030100', 1002, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321003, 10, 32, '04040100', 1003, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321004, 10, 32, '04010300', 1004, 1)\n");
			break;
		case	ID_FORMNO_101:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321101, 11, 32, '04050400', 1101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321102, 11, 32, '04050100', 1102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321103, 11, 32, '04050300', 1103, 1)\n");
			break;
		case	ID_FORMNO_111:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321301, 13, 32, '04020100', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321302, 13, 32, '05010200', 1302, 1)\n");
			break;
		case	ID_FORMNO_142:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321701, 17, 32, '0A010100', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321702, 17, 32, '0A010200', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321703, 17, 32, '0A010300', 1701, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321704, 17, 32, '0A010400', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321705, 17, 32, '0A010500', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321706, 17, 32, '0A010600', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321707, 17, 32, '0A010700', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321708, 17, 32, '0F020100', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321709, 17, 32, '0F020200', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321710, 17, 32, '0F020300', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321711, 17, 32, '0F020400', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321712, 17, 32, '0F020500', 1703, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (321713, 17, 32, '0F020600', 1703, 1)\n");
			break;
		case	ID_FORMNO_161:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322101, 21, 32, '0B010B00', 2101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322102, 21, 32, '0B010100', 2102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322103, 21, 32, '0B010200', 2103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322104, 21, 32, '0C010100', 2104, 1)\n");
			break;
		case	ID_FORMNO_162:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322201, 22, 32, '0B020900', 2201, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322202, 22, 32, '0B020101', 2202, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322203, 22, 32, '0B020500', 2203, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (322204, 22, 32, '0C020100', 2204, 1)\n");
			break;
		}
		break;
	// 病院
	case	0x30:
		switch(nFormSeq)	{
		case	ID_FORMNO_011:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480101, 1, 48, '01020100', 101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480102, 1, 48, '01020200', 102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480103, 1, 48, '01030200', 103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480104, 1, 48, '01020300', 104, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480105, 1, 48, '01030100', 105, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480106, 1, 48, '01030300', 106, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480107, 1, 48, '01010100', 107, 1)\n");
			break;
		case	ID_FORMNO_031:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480301, 3, 48, '01040100', 302, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480302, 3, 48, '01040200', 302, 1)\n");
			break;
		case	ID_FORMNO_041:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480401, 4, 48, '01070800', 401, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480402, 4, 48, '01070100', 402, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480403, 4, 48, '01070200', 403, 1)\n");
			break;
		case	ID_FORMNO_042:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480501, 5, 48, '01070400', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480502, 5, 48, '01070500', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480503, 5, 48, '01070600', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480504, 5, 48, '02030400', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480505, 5, 48, '02030500', 501, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480506, 5, 48, '02030600', 501, 1)\n");
			break;
		case	ID_FORMNO_081:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (480901, 9, 48, '04010100', 901, 1)\n");
			break;
		case	ID_FORMNO_091:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481001, 10, 48, '04010200', 1001, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481002, 10, 48, '04010300', 1002, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481003, 10, 48, '04040100', 1003, 1)\n");
			break;
		case	ID_FORMNO_101:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481101, 11, 48, '04050400', 1101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481102, 11, 48, '04050100', 1102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481103, 11, 48, '04050200', 1103, 1)\n");
			break;
		case	ID_FORMNO_111:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481301, 13, 48, '04020100', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481302, 13, 48, '04020200', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481303, 13, 48, '04020300', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481304, 13, 48, '04020400', 1301, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481305, 13, 48, '05010200', 1302, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481306, 13, 48, '05010400', 1302, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481307, 13, 48, '05010500', 1302, 1)\n");
			break;
		case	ID_FORMNO_142:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481701, 17, 48, '0A010400', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481702, 17, 48, '0A010500', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481703, 17, 48, '0A010600', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481704, 17, 48, '0A010700', 1702, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (481705, 17, 48, '0A010800', 1702, 1)\n");
			break;
		case	ID_FORMNO_161:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482101, 21, 48, '0B010B00', 2101, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482102, 21, 48, '0B010100', 2102, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482103, 21, 48, '0B010200', 2103, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482104, 21, 48, '0C010100', 2104, 1)\n");
			break;
		case	ID_FORMNO_162:
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482201, 22, 48, '0B020900', 2201, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482202, 22, 48, '0B020101', 2202, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482203, 22, 48, '0B020500', 2203, 1)\n");
			cs+=_T("INSERT INTO uc_ren_kamoku VALUES (482204, 22, 48, '0C020100', 2204, 1)\n");
			break;
		}
		break;
	}
	if(cs.IsEmpty() == FALSE)	{
		m_pZmSub->m_database->ExecuteSQL(cs);
	}

// 改良No.21-0086,21-0529 add -->
	//bG_KmkAdd2	マスター作成時、「電子記録○○」の科目を追加したサイン
	if(bG_KmkAdd2 == TRUE)	{
		// 病院マスターの場合
		CZmGen8			ZmGen;
		EnumIdByoinType	nByoinType = ID_ZMG_NOT_BYOIN_TYPE;
		if(m_pZmSub != NULL)	{
			m_pZmSub->VolumeOpen();
			if(nCoType == 0x30) {
				// どのタイプの病院マスター？
				nByoinType = ZmGen.CheckByoinType(m_pZmSub);
			}
			m_pZmSub->VolumeClose();
		}

		switch(nFormSeq) {
			case	ID_FORMNO_021:
				mfcRec.RequeryForm(nFormSeq);
				while(!mfcRec.IsEOF())	{
					sData = mfcRec.m_KnName;
					sData.Replace("　", "");
					sData.Replace(" ", "");
					sData = sData.Trim();
					if(strcmp(sData, ADDKMK_0211_UCHINAME) == 0)	{		// 電子記録債権
						// 新規レコード追加
						mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
						// 特殊行データへ更新
						mfcRenRec.Edit();
						mfcRenRec.m_KcdStart = ADDKMK_0211_KCODE;	// 財務科目コード取得
						mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
						mfcRenRec.Update();
					}
					//else if(strcmp(sData, ADDKMK_0212_UCHINAME) == 0)	{	// 割引電子記録債権
						// ※割引電子記録債権はセットしない
					//}
					//else if(((nCoType & 0xff) != 0x30) && 
					//		strcmp(sData, ADDKMK_0213_UCHINAME) == 0)	{	// 営業外電子記録債権
					//	// 新規レコード追加
					//	mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
					//	// 特殊行データへ更新
					//	mfcRenRec.Edit();
					//	mfcRenRec.m_KcdStart = ADDKMK_0213_KCODE;	// 財務科目コード取得
					//	mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
					//	mfcRenRec.Update();
					//}
					//else if(((nCoType & 0xff) == 0x30) && (nByoinType < ID_ZMG_H29REV_IRYO_TYPE) && 
					//		strcmp(sData, ADDKMK_0214_UCHINAME) == 0)	{	// 医業外電子記録債権
					//	// 新規レコード追加
					//	mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
					//	// 特殊行データへ更新
					//	mfcRenRec.Edit();
					//	mfcRenRec.m_KcdStart = ADDKMK_0214_KCODE;	// 財務科目コード取得
					//	mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
					//	mfcRenRec.Update();
					//}
					//else if(((nCoType & 0xff) == 0x30) && (nByoinType >= ID_ZMG_H29REV_IRYO_TYPE) && 
					//		strcmp(sData, ADDKMK_0215_UCHINAME) == 0)	{	// 事業外電子記録債権
					//	// 新規レコード追加
					//	mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
					//	// 特殊行データへ更新
					//	mfcRenRec.Edit();
					//	mfcRenRec.m_KcdStart = ADDKMK_0215_KCODE;	// 財務科目コード取得
					//	mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
					//	mfcRenRec.Update();
					//}

					mfcRec.MoveNext();
				}
				// レコード閉じる
				mfcRec.Fin();
				mfcRenRec.Fin();
				break;

			case	ID_FORMNO_081:
				mfcRec.RequeryForm(nFormSeq);
				while(!mfcRec.IsEOF())	{
					sData = mfcRec.m_KnName;
					sData.Replace("　", "");
					sData.Replace(" ", "");
					sData = sData.Trim();
					if(strcmp(sData, ADDKMK_0811_UCHINAME) == 0)	{		// 電子記録債務
						// 新規レコード追加
						mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
						// 特殊行データへ更新
						mfcRenRec.Edit();
						mfcRenRec.m_KcdStart = ADDKMK_0811_KCODE;	// 財務科目コード取得
						mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
						mfcRenRec.Update();
					}
					//else if(((nCoType & 0xff) != 0x30) && 
					//		strcmp(sData, ADDKMK_0812_UCHINAME) == 0)	{	// 営業外電子記録債務
					//	// 新規レコード追加
					//	mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
					//	// 特殊行データへ更新
					//	mfcRenRec.Edit();
					//	mfcRenRec.m_KcdStart = ADDKMK_0812_KCODE;	// 財務科目コード取得
					//	mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
					//	mfcRenRec.Update();
					//}
					//else if(((nCoType & 0xff) == 0x30) && (nByoinType < ID_ZMG_H29REV_IRYO_TYPE) && 
					//		strcmp(sData, ADDKMK_0813_UCHINAME) == 0)	{		// 医業外電子記録債務
					//	// 新規レコード追加
					//	mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
					//	// 特殊行データへ更新
					//	mfcRenRec.Edit();
					//	mfcRenRec.m_KcdStart = ADDKMK_0813_KCODE;	// 財務科目コード取得
					//	mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
					//	mfcRenRec.Update();
					//}
					//else if(((nCoType & 0xff) == 0x30) && (nByoinType >= ID_ZMG_H29REV_IRYO_TYPE) && 
					//		strcmp(sData, ADDKMK_0814_UCHINAME) == 0)	{		// 事業外電子記録債務
					//	// 新規レコード追加
					//	mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
					//	// 特殊行データへ更新
					//	mfcRenRec.Edit();
					//	mfcRenRec.m_KcdStart = ADDKMK_0814_KCODE;	// 財務科目コード取得
					//	mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
					//	mfcRenRec.Update();
					//}

					mfcRec.MoveNext();
				}
				// レコード閉じる
				mfcRec.Fin();
				mfcRenRec.Fin();
				break;

			case	ID_FORMNO_162:
				mfcRec.RequeryForm(nFormSeq);
				while(!mfcRec.IsEOF()) {
					sData = mfcRec.m_KnName;
					sData.Replace("　", "");
					sData.Replace(" ", "");
					sData = sData.Trim();
					if(strcmp(sData, ADDKMK_1621_UCHINAME) == 0)	{		// 電子記録債権売却損
						// 新規レコード追加
						mfcRenRec.CreateNewRecord(nFormSeq, m_pRenInfo->CoType);					// レコード初期化済み
						// 特殊行データへ更新
						mfcRenRec.Edit();
						mfcRenRec.m_KcdStart = ADDKMK_1621_KCODE;	// 財務科目コード取得
						mfcRenRec.m_KnSeq = mfcRec.m_KnSeq;			// 内訳書科目シーケンス保存
						mfcRenRec.Update();
					}

					mfcRec.MoveNext();
				}
				// レコード閉じる
				mfcRec.Fin();
				mfcRenRec.Fin();
				break;
		}
	}
// 改良No.21-0086,21-0529 add <--
}

void CdlgRenKamoku::OnBnClickedButtonReset()
{
	CString		cs=_T("");

	cs = _T("連動科目を初期状態に戻します。\n");
	cs+= _T("（追加した科目はすべて削除されます）\n");
	cs+= _T("よろしいですか？");
	if(ICSMessageBox(cs,MB_YESNO|MB_DEFBUTTON2) == IDYES)	{
		try	{
			// レコードを一旦削除する
			CString		cs=_T("");
			cs.Format(_T("DELETE FROM uc_ren_kamoku WHERE FormSeq=%d AND FgCoType=%d"),m_nFormSeq,m_pRenInfo->CoType);
			m_pZmSub->m_database->ExecuteSQL(cs);
			// レコードを追加する
			RecordReset(m_nFormSeq,m_pRenInfo->CoType);
			// リストデータ取得
			GetListData();
			// 位置移動
			MovePosition(0,0);
		}
		catch(CDBException *e)	{
			ICSMessageBox(e->m_strError,MB_ICONSTOP);
			return;
		}
	}
}
// 20-0451 add <--
