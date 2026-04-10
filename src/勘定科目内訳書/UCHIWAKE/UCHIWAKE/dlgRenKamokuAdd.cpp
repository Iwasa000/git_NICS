// dlgRenKamokuAdd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgRenKamokuAdd.h"


// CdlgRenKamokuAdd ダイアログ

IMPLEMENT_DYNAMIC(CdlgRenKamokuAdd, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgRenKamokuAdd::CdlgRenKamokuAdd(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgRenKamokuAdd::IDD, pParent)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgRenKamokuAdd::~CdlgRenKamokuAdd()
{
}

void CdlgRenKamokuAdd::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_TREE_KAMOKU, m_treeKamoku);
	DDX_Control(pDX, IDC_TREEMESSAGE_LABEL, m_lblMessage);
}


BEGIN_MESSAGE_MAP(CdlgRenKamokuAdd, ICSDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgRenKamokuAdd::OnBnClickedOkButton)
END_MESSAGE_MAP()

void CdlgRenKamokuAdd::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamokuAdd::OnInitDialog()
{
	BOOL bDebugCursor = FALSE;

	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	// 財務DBクラスを取得
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// プログレスバー開始（暫定：カーソル変更）
	if (bDebugCursor == FALSE) {
		AfxGetApp()->DoWaitCursor(1);
	}

	// ツリーの設定
	SetTreeItem();

	// プログレスバー終了（暫定：カーソル変更）
	if (bDebugCursor == FALSE) {
		AfxGetApp()->DoWaitCursor(-1);
	}

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（選択ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamokuAdd::OnBnClickedOkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if ( GetTreeData() == 0 ){
		m_nRet = ID_DLG_OK;
		ICSDialog::OnOK();
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamokuAdd::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamokuAdd::PreTranslateMessage(MSG* pMsg)
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
			if ( GetFocusControlID( this ) == IDC_TREE_KAMOKU ){
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			}
			break;
		//	Escキー
		case VK_ESCAPE:
			//	OnCancel起動
			nID = IDCANCEL;
			break;
		// スタティックテキストがあると矢印キーの移動ができないため対応
		case VK_RIGHT:
		case VK_DOWN:
			if (  ( GetFocusControlID( this ) == ID_OK_BUTTON ) || ( GetFocusControlID( this ) == IDCANCEL ) ){
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}
			break;
		case VK_LEFT:
		case VK_UP:
			if (  ( GetFocusControlID( this ) == ID_OK_BUTTON ) || ( GetFocusControlID( this ) == IDCANCEL ) ){
				keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}
			break;
		//	F2キー
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;
		//	その他キー
		default:
			break;
		}
	}

	//	ID番号あり？
	if ( nID != 0 ){
		//	メッセージ投げる
		PostMessage( WM_COMMAND, nID );
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
// ツリーの設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenKamokuAdd::SetTreeItem()
{
	HTREEITEM		htreeBig;		// 大分類ツリーアイテムのハンドル
	HTREEITEM		htreeMiddle;	// 中分類ツリーアイテムのハンドル
	HTREEITEM		htreeSmall;		// 小分類ツリーアイテムのハンドル
	TV_INSERTSTRUCT tvItem;			// ツリービューアイテムの構造体
	int				intKubun;		// 区分
	ULONG			lngkcod;		// 科目コード
	char			cbuff[64];		// 科目名バッファ

	// 初期化
    memset((char *)&tvItem, '\0', sizeof(tvItem));
	intKubun = -1;
	m_treeKamoku.DeleteItem(TVI_ROOT);

	// ツリーアイテムの初期設定
	// 追加アイテムは最後に追加
    tvItem.hInsertAfter = TVI_LAST;
	// 有効にする値はテキスト(科目名), イメージ(アイコン),Param値(科目コード)
    tvItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	// 区分ごとに表示するビットマップを読み込む
	if( !m_imgList.m_hImageList ) {
		m_imgList.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 6, 6 );

		m_bitImage[0].LoadBitmap(IDB_TREEICON1);
		m_bitImage[1].LoadBitmap(IDB_TREEICON2);
		m_bitImage[2].LoadBitmap(IDB_TREEICON3);
		m_bitImage[3].LoadBitmap(IDB_TREEICON4);

		// CImageListにビットマップ読込み
		for( int iCnt=0; iCnt < 4; iCnt++ ) {
			m_imgList.Add( &m_bitImage[iCnt],  &m_bitImage[4] );
		}
	}

	// ツリーアイテムに区分ごとのイメージを持たせる(TVSIL_NORMAL)
	m_treeKamoku.SetImageList( &m_imgList, TVSIL_NORMAL );

	// クエリ文を作成
	CString strRequery;		// クエリ文

	// 初期化
	strRequery.Empty();

	// 科目範囲指定
	strRequery.Format("gkcod < 'ff000000'");

	// クエリ文に仕様不可科目除外クエリを加える
	strRequery += _T(" AND (gkvoid & 0x01) = 0");

	// 合計科目テーブル(gkrec)読込み
// 修正No.159338 del -->
//	if ( m_pZmSub->GokeiMeisyoOpen( strRequery , 0 ) != 0 ){
// 修正No.159338 del <--
// 修正No.159338 add -->
	if(m_pZmSub->GokeiMeisyoOpen(strRequery, 1) != 0)	{
// 修正No.159338 add <--
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( m_pZmSub );
		return;
	}
	if ( m_pZmSub->gkrec->GetRecordCount() <= 0 ){
		return;
	}

	// 合計科目を全て読み込むまでループ
	do {
		// 区分を取得
		intKubun = m_clsFunc.GetKcdKubun( m_pZmSub->gkrec->gkcod );

		// 取得区分が小分類～大分類の間
		if ( ( intKubun >= 1 ) && ( intKubun < 4 ) ){

			// 科目コードを変換
			Apt_AscToBcd( (char*)&lngkcod, (LPTSTR)LPCTSTR(m_pZmSub->gkrec->gkcod), 8 );
			hl_rev( (BYTE*)&lngkcod, 4 );

			// 取得した科目名をバッファに格納
			wsprintf( cbuff , "%-24.24s", m_pZmSub->gkrec->gknam );

			// 区分ごとの設定&ツリーアイテム挿入
			switch( intKubun ){
			
			// 大分類
			case 3:
				tvItem.hParent = TVI_ROOT;	// 最上段のツリーが親
				tvItem.item.iImage = 0;
				tvItem.item.iSelectedImage = 0;
				tvItem.item.pszText = cbuff;
				tvItem.item.lParam = (LONG)(lngkcod);
				htreeBig = m_treeKamoku.InsertItem(&tvItem);
				break;
			// 中分類
			case 2:
				tvItem.hParent = htreeBig;	// 大分類ツリーが親
				tvItem.item.iImage = 1;
				tvItem.item.iSelectedImage = 1;
				tvItem.item.pszText = cbuff;
				tvItem.item.lParam = (LONG)(lngkcod);
				htreeMiddle = m_treeKamoku.InsertItem(&tvItem);
				break;
			// 小分類
			case 1:
				tvItem.hParent = htreeMiddle;	// 中分類ツリーが親
				tvItem.item.iImage = 2;
				tvItem.item.iSelectedImage = 2;
				tvItem.item.pszText = cbuff;
				tvItem.item.lParam = (LONG)(lngkcod);
				htreeSmall = m_treeKamoku.InsertItem(&tvItem);
				// 小分類の場合は属する明細科目を設定
				SetMeisaiKamoku( &m_treeKamoku , htreeSmall , m_pZmSub->gkrec->gkcod );
				break;
			default:
				break;
			}
		}
	}while( m_pZmSub->gkrec->MoveNext() != -1 );

	// 合計科目テーブル閉じる
	m_pZmSub->GokeiMeisyoClose();


}

//**************************************************
// 明細科目の設定
//	【引数】	pTree	…　ツリービュークラスのポインタ
//				htree	…　ツリーアイテムのハンドル
//				kcod	…　対象財務科目コード
//	【戻値】	なし
//**************************************************
void CdlgRenKamokuAdd::SetMeisaiKamoku( CTreeCtrl* pTree, HTREEITEM htree, CString kcod )
{
	CString strFilter = "";			// クエリ文
	TV_INSERTSTRUCT tvItem;			// ツリービューアイテムの構造体
	ULONG			lngkcod;		// 科目コード
	char			cbuff[64];		// 科目名バッファ

	// 初期化
    memset((char *)&tvItem, '\0', sizeof(tvItem));

	// クエリ文作成(対象科目コードの小分類以下の明細科目を全検索)
	strFilter.Format( "kncod LIKE '%s%%'", kcod.Left(6) );

	// クエリ文に仕様不可科目除外クエリを加える
	strFilter += _T(" AND (knvoid & 0x01) = 0");

	// 科目名称テーブルオープン
// 修正No.159338 del -->
//	if ( m_pZmSub->KamokuMeisyoOpen( (strFilter) , 0 ) != 0 ){
// 修正No.159338 del <--
// 修正No.159338 add -->
	if(m_pZmSub->KamokuMeisyoOpen(strFilter, 1) != 0)	{
// 修正No.159338 add <--
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( m_pZmSub );
		return;
	}
	if ( m_pZmSub->knrec->GetRecordCount() <= 0 ){

		// 科目名称テーブル閉じる(2012/7/20)
		m_pZmSub->KamokuMeisyoClose();

		return;
	}

	// ツリーアイテムの情報を設定
    tvItem.hInsertAfter = TVI_LAST;	// 追加アイテムは最後に追加
	tvItem.hParent = htree;			// ツリーの親を設定
    tvItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;	// 有効にする値はテキスト(科目名),
																						// イメージ(アイコン),Param値(科目コード)
	tvItem.item.iImage = 3;
	tvItem.item.iSelectedImage = 3;
	// 対象の全明細科目を読み込む
	do {
		// 科目コードを変換
		Apt_AscToBcd( (char*)&lngkcod, (LPTSTR)LPCTSTR( m_pZmSub->knrec->kncod), 8 );
		hl_rev( (BYTE*)&lngkcod, 4 );

		// 取得した科目名をバッファ
		wsprintf( cbuff , "%-24.24s", m_pZmSub->knrec->knnam );

		// ツリーアイテムの情報を設定
		tvItem.item.pszText = cbuff;
		tvItem.item.lParam = (LONG)(lngkcod);

		pTree->InsertItem( &tvItem );

	}while( m_pZmSub->knrec->MoveNext() != -1 );

	// 科目名称テーブル閉じる
	m_pZmSub->KamokuMeisyoClose();
}

//**************************************************
// 明細科目の設定
//	【引数】	なし
//	【戻値】	0 … 成功
//			   -1 … 失敗
//**************************************************
int CdlgRenKamokuAdd::GetTreeData()
{	
	int			intRet = -1;	// 戻り値
	int			intKubun = -1;	// 区分
	long		lngkcod = 0;	// 科目コード(数値)
	CString		strkcod = "";	// 科目コード(文字列)
	CString		strknam = "";	// 科目名称
	HTREEITEM	hGetItem;		// 取得ハンドル

	// 選択中のツリーアイテムハンドル取得
	hGetItem = m_treeKamoku.GetSelectedItem();

	// ハンドル取得?
	if ( hGetItem != NULL ){

		// 科目コード取得
		lngkcod = (LONG)m_treeKamoku.GetItemData( hGetItem );
		strkcod = GetHexToString( lngkcod );
		if ( strkcod == "" ){
			return intRet;
		}
		// 科目区分チェック(小分類 or 明細科目のみ登録可)
		intKubun = m_clsFunc.GetKcdKubun( strkcod );
		if ( ( intKubun != 0 ) && ( intKubun != 1 ) ){
			ICSMessageBox( _T( "小分類の合計科目または明細科目を選択してください。" ), MB_OK, 0, 0, this );
			return intRet;
		}
		// 科目重複登録チェック
		if ( CheckKcdStart( strkcod ) != 0 ){
			return intRet;
		}

		// 科目コード格納
		m_Kncod = strkcod;
		// 科目名格納
		m_Knnam = m_treeKamoku.GetItemText( hGetItem );
		intRet = 0;

		// 科目カナ取得
		if( m_pZmSub != NULL ){
			CString strFilter;
			strFilter = _T("kncod = '") + m_Kncod + _T("'");
// 修正No.159338 del -->
			//if( m_pZmSub->KamokuMeisyoOpen( strFilter, 0 ) != 0 ){
// 修正No.159338 del <--
// 修正No.159338 add -->
			if(m_pZmSub->KamokuMeisyoOpen(strFilter, 1) != 0)	{
// 修正No.159338 add <--
				m_clsFunc.ZmSubErrMsg( m_pZmSub );
				m_pZmSub->KamokuMeisyoClose();
				return intRet;
			}
			if( m_pZmSub->knrec->GetRecordCount() > 0 ){
				m_Knkana = m_pZmSub->knrec->knkana;
			}
			m_pZmSub->KamokuMeisyoClose();
		}
	}
	else{
		ICSMessageBox( _T( "財務科目が選択されていません。" ), MB_OK, 0, 0, this );
	}

	return intRet;
}

//**************************************************
//	文字列科目コードを数値に変更する処理１
//  ※ICS様のサンプルソースから転記したため内部構造は不明
//**************************************************
void CdlgRenKamokuAdd::Apt_AscToBcd( char *buff, char *data, int leng )
{

	CString str;
	str.Format( "%s", data );
	str.MakeUpper();

	int bcdlen = leng /2;
	int	hlsw = 0;
	if( leng%2 ) {
		hlsw = 1;
		bcdlen++;
	}

	BYTE hc, lc;

	for( int i=0, j=0; i < bcdlen; i++ ) {

		if( hlsw ) {
			hc = 0;
			hlsw = 1;
		}
		else	hc = str.GetAt( j++ );

		if( hc >= 'A' && hc <= 'Z' ) {
			hc = hc-'A' + 0x0a;
		}
		else {
			hc = hc - '0';
		}
		hc = ((hc & 0x0f) << 4) & 0xf0;

		lc = str.GetAt( j++ );
		if( lc >= 'A' && lc <= 'Z' ) {
			lc = lc-'A' + 0x0a;
		}
		else {
			lc = lc - '0';
		}
		*(buff+i) = hc + lc;
	}
}

//**************************************************
//	文字列科目コードを数値に変更する処理２
//  ※ICS様のサンプルソースから転記したため内部構造は不明
//**************************************************
void CdlgRenKamokuAdd::hl_rev(BYTE* dp, int ln)
{
	BYTE	tmp;
	int		epnt;

	int	lcnt = ln / 2;
	for (int i = 0; i < lcnt; ++i)  {
		epnt = ln-i-1;
		tmp = *(dp+i);
		*(dp+i) = *(dp+epnt);
		*(dp+epnt) = tmp;
	}
}

//**************************************************
//	ダイアログ表示
//	【引数】	lngCode			…	科目コード(数値)
//	【戻値】	文字列			…	成功
//				""				…	失敗
//**************************************************
CString CdlgRenKamokuAdd::GetHexToString( long lngCode ) 
{
	char	cCode[10];	// 科目コード文字列格納バッファ
	CString	strCode;	// 科目コード文字列

	// 初期化
	ZeroMemory( &cCode , sizeof( cCode ) );
	strCode = "";

	// 数値科目コードが0以下なら実行しない
	if ( lngCode != 0 ){
		// 数値で格納されているlngCodeを文字列へ変換
		_ltoa_s( lngCode , &cCode[0] , sizeof( cCode ) , 16 );
		strCode = cCode;

		// 科目コードを文字列に変換した場合、
		// "0"が省かれている場合があるので付加する
		if ( strCode.GetLength() < 8 ){
			// 8文字になるまでループ
			for( ;strCode.GetLength() < 8; ){
				strCode = "0" + strCode;
			}
		}
	}

	return strCode;
}

//*************************************************************************************
//	科目コードの重複チェック処理
//	機能：取得した科目コードの重複登録をチェックする
//	１．指定科目コードが登録済み
//		例："01010101"が登録されている状態で"01010101"を入力→エラーメッセージ1を表示
//	２．指定科目コードを含む合計科目が登録済み
//		例："01000000"が登録されている状態で"01010101"を入力→エラーメッセージ2を表示
//	３．指定科目コードに含まれる科目が登録済み
//		例："01010101"が登録されている状態で"01000000"を入力→エラーメッセージ3を表示
//	４．指定科目コードが不正(コード桁数が8文字に満たない場合 or "00000000"の場合等)
//		例："0101"を入力 or "00000000"を入力→エラーメッセージ4を表示
//
//	【引数】	strkcd		…	チェック科目コード
//	【戻値】	0			…	未登録
//				-1			…	チェック科目～大分類までのいずれかで登録済み
//				-2			…　チェック科目に含まれる科目が登録済み	
//				-3			…	不正科目コード(8文字じゃない)
//**************************************************************************************
int CdlgRenKamokuAdd::CheckKcdStart( CString strkcd ){

	CdbUcRenKamoku mfcRec( m_pDB );
	int intKubun = 0;		// 科目の区分
	int	intTrim  = 0;		// 区分
	int intCnt = 0;			// カウント用
	CString strCode = "";	// 検索科目名

	// 区分を取得
	intKubun = m_clsFunc.GetKcdKubun( strkcd );

	// 入力科目から大分類まで検索
	if ( ( intKubun >= 0 ) && ( intKubun < 4 ) ){
		for ( intCnt = intKubun; intCnt < 4; intCnt++ ){
			
			// 区分によって検索科目名を作成
			switch( intCnt ){

			// 入力科目
			case 0:
				strCode = strkcd;
				break;
			// 小分類
			case 1:
				intTrim = 6;
				strCode = strkcd.Left(intTrim) + _T("00");
				break;
			// 中分類
			case 2:
				intTrim = 4;
				strCode = strkcd.Left(intTrim) + _T("0000");
				break;
			// 大分類
			case 3:
				intTrim = 2;
				strCode = strkcd.Left(intTrim) + _T("000000");
				break;
			// それ以外
			default:
				strCode = _T("");
				break;
			}

			// DBでの区分の重複チェック
			if ( mfcRec.CheckRepeat( m_nFormSeq , m_pRenInfo->CoType , strCode ) > DB_ERR_OK ){
				if ( intKubun == intCnt ){
					// エラーメッセージ1
					ICSMessageBox( _T( "既に登録されています。" ), MB_OK, 0, 0, this );
				}
				else{
					// エラーメッセージ2
					ICSMessageBox( _T( "この科目を含む合計科目が既に登録されています。" ), MB_OK, 0, 0, this );
				}
				mfcRec.Fin();
				return -1;
			}
			// 取得合計科目内の科目が登録されているかのチェック
			if ( ( intKubun > 0 ) && ( intKubun == intCnt ) ){
				if ( mfcRec.CheckRepeat( m_nFormSeq , m_pRenInfo->CoType , strkcd.Left(intTrim) , 1 ) > DB_ERR_OK ){
					// エラーメッセージ3
					ICSMessageBox( _T( "この合計科目に含まれる科目が既に登録されています。" ), MB_OK, 0, 0, this );
					mfcRec.Fin();
					return -2;
				}
			}
		}
	}
	// 入力科目～大分類では無い場合
	else{
		// エラーメッセージ4
		ICSMessageBox( _T( "登録に失敗しました。" ), MB_OK, 0, 0, this );
		mfcRec.Fin();
		return  - 3;
	}


	return 0;

}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgRenKamokuAdd::GetFocusControlID( CWnd* pParent /*=NULL*/)
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
int CdlgRenKamokuAdd::ShowDialog( CDatabase* pDB , RENZVOL_INFO* pRenInfo , int nFormSeq )
{
	m_pDB		= NULL;
	m_nFormSeq  = ID_FORMNO_011;
	m_nRet		= ID_DLG_CANCEL;
	m_Kncod		= "";
	m_Knnam		= "";
	m_Knkana	= "";

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

	return( m_nRet );
}


