//--------------------------------------------------
//	dlgYoushikiSelect.cpp
//
//	2006.02.21～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgyoushikiselect.h"

// midori 155015 add -->
extern	BOOL	bG_Kanso;
// midori 155015 add <--

IMPLEMENT_DYNAMIC(CdlgYoushikiSelect, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgYoushikiSelect::CdlgYoushikiSelect(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgYoushikiSelect::IDD, pParent)
{
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgYoushikiSelect::CdlgYoushikiSelect( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgYoushikiSelect::~CdlgYoushikiSelect()
{
}

void CdlgYoushikiSelect::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_YOUSHIKI_LIST, m_lstYoushiki);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_SKIPDOWN_BUTTON, m_btnSkipDown);
	DDX_Control(pDX, ID_SKIPUP_BUTTON, m_btnSkipUp);
	DDX_Control(pDX, IDC_CUSTOM_REPORT_YOUSHIKI_LIST, m_report);
}


BEGIN_MESSAGE_MAP(CdlgYoushikiSelect, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgYoushikiSelect::OnBnClickedOk)
	ON_BN_CLICKED(ID_SKIPUP_BUTTON, &CdlgYoushikiSelect::OnBnClickedSkipupButton)
	ON_BN_CLICKED(ID_SKIPDOWN_BUTTON, &CdlgYoushikiSelect::OnBnClickedSkipdownButton)
	ON_NOTIFY(ICS_NM_REPORT_SELCHANGED, IDC_CUSTOM_REPORT_YOUSHIKI_LIST, OnNMReportSelchangedCUSTOM_REPORT_YOUSHIKI_LIST)
	ON_NOTIFY(NM_DBLCLK, IDC_CUSTOM_REPORT_YOUSHIKI_LIST, OnNMDblclkReportYoushikiList)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgYoushikiSelect, ICSDialog)
	//ON_EVENT(CdlgYoushikiSelect, IDC_YOUSHIKI_LIST, 7, CdlgYoushikiSelect::LButtonDblClickYoushikiList, VTS_I2)
	//ON_EVENT(CdlgYoushikiSelect, IDC_YOUSHIKI_LIST, 10, CdlgYoushikiSelect::SelChangeYoushikiList, VTS_I2)
	//ON_EVENT(CdlgYoushikiSelect, IDC_YOUSHIKI_LIST, 9, CdlgYoushikiSelect::FocusedYoushikiList, VTS_NONE)
	//ON_EVENT(CdlgYoushikiSelect, IDC_YOUSHIKI_LIST, 6, CdlgYoushikiSelect::LButtonClickYoushikiList, VTS_I2)
END_EVENTSINK_MAP()

void CdlgYoushikiSelect::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgYoushikiSelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgYoushikiSelect::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	InitReportHeader();

	AddRecords();

	SetDataExistVec();

	//行の背景色を更新
	ReDrawRowColor();

	//	様式シーケンス番号を取得
	m_FormSeq = GetFormSeq(m_pDB);
	int formorder = GetFormOrder(m_pDB) - 1;
	m_report.SetFocus();
	m_report.SetFocusedRow(m_report.GetRows()->GetAt(formorder));

	UpdateYoushikiMoveButton();

	// リサイズ
	OnInitDialogEX();

	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ウィンドウメッセージをフィルタリング
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgYoushikiSelect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN ){

		//	パラメータで分岐
		switch( pMsg->wParam ){
		
		//----------
		//	Enter
		//----------
		case VK_RETURN:
			//	OnOK起動
//			PostMessage( WM_COMMAND, IDOK );

			//	リストに居る？
			if ( GetFocusControlID( this ) == IDC_CUSTOM_REPORT_YOUSHIKI_LIST ){
				//PostMessage(WM_COMMAND, ID_OK_BUTTON);
				m_btnOK.SetFocus();
				return 1;
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
		//	F7
		//----------
		case VK_F7:
			//　前様式起動
			PostMessage( WM_COMMAND, ID_SKIPUP_BUTTON );
			break;

		//----------
		//	F8
		//----------
		case VK_F8:
			//　前様式起動
			PostMessage( WM_COMMAND, ID_SKIPDOWN_BUTTON );
			break;

		//----------
		//	Esc
		//----------
		case VK_ESCAPE:
			//	OnCancel起動
			PostMessage( WM_COMMAND, IDCANCEL );
			break;

		//----------
		//	↑ ←
		// F7,F8で大きくジャンプした後元の位置からの↑になってしまう為、オーバーライド
		//----------
		case VK_UP:
		case VK_LEFT:
			MoveSelectedRow(TRUE, FALSE);
			return 1;

		//----------
		//	↓ →
		// F7,F8で大きくジャンプした後元の位置からの↑になってしまう為、オーバーライド
		//----------
		case VK_DOWN:
		case VK_RIGHT:
			MoveSelectedRow(FALSE, FALSE);
			return 1;

		//----------
		//	PageUp, PageDown
		// 結構大きくジャンプ
		//----------
		case VK_PRIOR:
			MoveSelectedRow(TRUE, FALSE, 19);
			return 1;
		case VK_NEXT:
			MoveSelectedRow(FALSE, FALSE, 19);
			return 1;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（ダブル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::LButtonDblClickYoushikiList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	//	選択ボタンクリックイベント起動
	OnBnClickedOk();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	選択ボタン
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::OnBnClickedOk()
{
	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	if(s_rows == NULL)	return;
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	int index = r_row->GetIndex();
	if(index >= 0){
		// 現在の様式が選択されたら何もしない
		if(m_FormSeq == m_vecDataExist[index].FormSeq){
			ICSDialog::OnCancel();
			return;
		}
		// frm000Commonで使用する為保存
		m_FormSeq = m_vecDataExist[index].FormSeq;
		// CdbUcInfMainに書き込み
		SetFormSeq(m_pDB, m_FormSeq);
		// 返り値
		m_nRet = ID_DLG_OK;
	}

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	前の様式へボタン
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::OnBnClickedSkipupButton()
{
	if(m_btnSkipUp.IsWindowEnabled() == FALSE)	return;

	MoveSelectedRow(TRUE, m_fYoushikiSkip);

	// 背景色の青が灰色になってしまうのを防ぐ
	m_report.SetFocus();
	//m_report.Populate();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	次の様式へボタン
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::OnBnClickedSkipdownButton()
{
	if(m_btnSkipDown.IsWindowEnabled() == FALSE)	return;
	
	MoveSelectedRow(FALSE, m_fYoushikiSkip);

	// 背景色の青が灰色になってしまうのを防ぐ
	m_report.SetFocus();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	行が変更された
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::SelChangeYoushikiList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//　ボタンのenable/disable
	UpdateYoushikiMoveButton();
	
	//	行の背景色を更新
	ReDrawRowColor();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	フォーカス（来た）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::FocusedYoushikiList()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	//	行の背景色を更新
	ReDrawRowColor();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（シングル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYoushikiSelect::LButtonClickYoushikiList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//　ボタンのenable/disable
	UpdateYoushikiMoveButton();

	//	行の背景色を更新
	ReDrawRowColor();
}

//**************************************************
//	様式シーケンス番号取得
//	【引数】	pDB		…	データベースハンドル
//	【戻値】	0以外	…	成功（取得した様式シーケンス番号を返す）
//				0		…	失敗
//**************************************************
int CdlgYoushikiSelect::GetFormSeq( CDatabase* pDB )
{
	int				nRet = ID_FORMNO_011;	//	戻値
	CdbUcInfMain	mfcRec( pDB );			//	uc_inf_mainテーブルクラス

	try{
		
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();			//	レコードを先頭に移動
				nRet = mfcRec.m_FormSeq;	//	戻値に様式シーケンス番号をセット
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
//	様式シーケンス番号取得
//	【引数】	pDB		…	データベースハンドル
//	【戻値】	0以外	…	成功（取得した様式シーケンス番号を返す）
//				0		…	失敗
//**************************************************
int CdlgYoushikiSelect::GetFormOrder( CDatabase* pDB )
{
	int				nRet = ID_FORMNO_011;	//	戻値
	CdbUcInfSub		mfcRec( pDB );			//	uc_inf_mainテーブルクラス

	try{
		if ( mfcRec.RequeryFormSeq(m_FormSeq) == DB_ERR_OK ){
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();			//	レコードを先頭に移動
				nRet = mfcRec.m_FormOrder;	//	戻値に様式シーケンス番号をセット
			}
			mfcRec.Fin();
		}
	}
	catch(...){
		if ( mfcRec.IsOpen() ){
			mfcRec.Fin();
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	様式シーケンス番号保存
//	【引数】	pDB			…	データベースハンドル
//				nFormOrder	…	様式シーケンス
//	【戻値】	0以外		…	成功（保存した様式シーケンス番号を返す）
//				0			…	失敗
//**************************************************
int CdlgYoushikiSelect::SetFormSeq( CDatabase* pDB, short nFormSeq )
{
	int				nRet = 0;					//	戻値を初期化
	CdbUcInfMain	mfcMain( pDB );				//	uc_inf_mainテーブルクラス

	try{
		if ( mfcMain.Init() == DB_ERR_OK ){
			if ( !( mfcMain.IsEOF() ) ){
				mfcMain.MoveFirst();		//	レコードを先頭に移動

				//	リストで選択された様式シーケンス番号と違う？
				if ( mfcMain.m_FormSeq != nFormSeq ){
					mfcMain.Edit();					//	編集開始
					mfcMain.m_FormSeq = nFormSeq;	//	様式シーケンス番号変更
					mfcMain.Update();				//	更新
					nRet = mfcMain.m_FormSeq;		//	戻値に変更した様式シーケンス番号設定
				}
			}
			//	レコードを閉じる
			mfcMain.Fin();
		}
	}
	catch(...){
		if ( mfcMain.IsOpen() )		mfcMain.Fin();
	}
	
	return(nRet);
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				nColor	…	色値
//	【戻値】	なし
//**************************************************
void CdlgYoushikiSelect::SetColor( int nIndex, COLORREF nColor )
{
	m_report.SetRowBackColor(nIndex, nColor, FALSE);

	return;
}

//**************************************************
//	行の背景色を更新
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgYoushikiSelect::ReDrawRowColor()
{
	int cnt = 0;
	for(vector<YOUSHIKI_SEL>::iterator it = m_vecDataExist.begin(); it != m_vecDataExist.end(); it++){
		if((*it).hasData != FALSE)	SetColor( cnt, RGB( 255, 255, 153 ) );
		else						SetColor( cnt, GetSysColor( COLOR_WINDOW ) );
		cnt++;
	}
	m_report.RedrawControl();

	return;
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgYoushikiSelect::GetFocusControlID( CWnd* pParent /*=NULL*/)
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
//	【戻値】	ID_DLG_OK		…	①選択ボタンが押された場合
//									②リストがダブルクリックされた場合
//				ID_DLG_CANCEL	…	①キャンセルボタンが押された場合
//									②引数「nYoushiki」と同じ様式が選択された場合
//**************************************************
int CdlgYoushikiSelect::ShowDialog( int fYoushikiSkip, CDatabase* pDB /*=NULL*/)
{
	m_nRet		= ID_DLG_CANCEL;	//	戻値を初期化
	m_FormSeq	= 0;				//	シーケンス番号
	m_nOldRow	= -1;				//	前回選択行初期化
	m_fYoushikiSkip = fYoushikiSkip;//　様式移動モードの設定

	//	データベースハンドルあり？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){

		}
	}
	
	//	戻値を返す
	return( m_nRet );
}

//********************************************************************************
//	「前の様式へ」、「次の様式へ」のボタンの監視
//		【引数】	mode	TRUE:無条件に両隣の様式のチェック/ FALSE:データがある様式
//		【戻値】	なし
//********************************************************************************
void CdlgYoushikiSelect::UpdateYoushikiMoveButton()
{
	BOOL			bBack			= TRUE;
	BOOL			bNext			= TRUE;
	ICSReportSelectedRows* rrs = m_report.GetSelectedRows();
	ICSReportRow* rw = rrs->GetAt(0);
	if(rw == NULL){
		m_report.SetFocus();
		return;
	}
	int selected_index = rw->GetIndex();

	if( m_fYoushikiSkip == FALSE ){
		if( selected_index == 0 ){	// 様式が始端(様式番号：①のときは「前の様式へ」ボタンをグレーアウト
			bBack = FALSE;
		}
		if( selected_index == 61 ){	// 様式が終端(様式番号：⑱－２０)のときは「次の様式へ」ボタンをグレーアウト
			bNext = FALSE;
		}
	}
	else{
		int data_seq = GetRecordDataOrder(selected_index, TRUE);
		if(data_seq == -1)	bBack = FALSE;
		data_seq = GetRecordDataOrder(selected_index, FALSE);
		if(data_seq == -1)	bNext = FALSE;
	}

	GetDlgItem( ID_SKIPUP_BUTTON )->EnableWindow( bBack );		// 前の様式ボタン
	GetDlgItem( ID_SKIPDOWN_BUTTON )->EnableWindow( bNext );	// 次の様式ボタン

	// 前/後のボタンがDisableの時、フォーカスが飛んでキーが効かなくなるので、ここでレポートコントロールにフォーカスを入れる
	if(bBack == FALSE || bNext == FALSE)	m_report.SetFocus();
}

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgYoushikiSelect::InitReportHeader()
{
	//m_report.Resize( TRUE, ICSResizeDefault );
	
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
// midori 161108 cor -->
	//m_report.AddColumn(new ICSReportColumn(0, NULL, 2));
	//m_report.AddColumn(new ICSReportColumn(1, NULL, 10));
// ---------------------
	m_report.AddColumn(new ICSReportColumn(0, NULL, 20));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 100));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 15));
// midori 161108 cor <--

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	ICSReportColumns *m_pClms = m_report.GetColumns();
	int cnt = sizeof(strYoushikiSelectHeader) / sizeof(strYoushikiSelectHeader[0]);
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(strYoushikiSelectHeader[i]);
	}

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);

	// 非アクティブ時のインバース色
	//m_report.ColorInactive(RGB(51,153,255));
}

//**************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************
// midori 161108_2 cor -->
//void CdlgYoushikiSelect::AddRecords()
//{
//	CdbUcInfSub	mfcRec(m_pDB);
//	mfcRec.InitOrder();
//	mfcRec.MoveFirst();
//	while(!mfcRec.IsEOF()){
//		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
//		pRecord->AddItem( new ICSReportRecordItemText(mfcRec.m_TitleNo + mfcRec.m_TitleNo2));	// 様式番号
//		pRecord->AddItem( new ICSReportRecordItemText(mfcRec.m_Title));							// 様式名称
//
//		mfcRec.MoveNext();
//	}
//
//	// 行の高さ
//	m_report.SetItemHeight(20);
//}
// -----------------------
void CdlgYoushikiSelect::AddRecords()
{
	CdbUcInfSub	mfcRec(m_pDB);
	CString		cs=_T("");

	mfcRec.InitOrder();
	mfcRec.MoveFirst();
	while(!mfcRec.IsEOF()){
		ICSReportRecord*	pRecord  =  m_report.AddRecord(new ICSReportRecord());
		// 様式番号
		pRecord->AddItem( new ICSReportRecordItemText(mfcRec.m_TitleNo + mfcRec.m_TitleNo2));
		// 様式名称
		pRecord->AddItem( new ICSReportRecordItemText(mfcRec.m_Title));
		// 保管データ
		cs=_T("");
		CdbUc000Common*		rsData = m_clsFunc.CreateDbObject(mfcRec.m_FormSeq,m_pDB);
		if(rsData != NULL)	{
			if(rsData->GetCountFgShow(ID_FGSHOW_HOKAN) > 0)	{
				cs=_T("あり");
			}
		}
		delete rsData;
		pRecord->AddItem(new ICSReportRecordItemText(cs,ICS_EDS_NUMERIC,FALSE,FALSE,3,ICS_EDS_CENTER));
		// 
		mfcRec.MoveNext();
	}

	// 行の高さ
	m_report.SetItemHeight(20);
}
// midori 161108_2 cor <--

//**************************************************
//	各様式にデータがある（背景色を黄色にする）かどうかのベクタ作成
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgYoushikiSelect::SetDataExistVec()
{
	CdbUcInfSub	mfcRec(m_pDB);
	mfcRec.InitOrder();
	mfcRec.MoveFirst();
	while(!mfcRec.IsEOF()){

		CdbUc000Common* rsData = m_clsFunc.CreateDbObject(mfcRec.m_FormSeq, m_pDB);
		if(rsData != NULL){
			BOOL hasData = FALSE;
// midori 160607 cor -->
//			if( rsData->GetCountDataRecord() != 0 ){
// ---------------------
			if( rsData->GetCountDataRecord(1) != 0 ){
// midori 160607 cor <--
				hasData = TRUE;
			}
// midori 155015 add -->
			// 棚卸資産の内訳書、買掛金の内訳書は欄外登録の内容があるかどうかでも判断を行う
			if((mfcRec.m_FormSeq == ID_FORMNO_051 && bG_Kanso == FALSE) || mfcRec.m_FormSeq == ID_FORMNO_091)	{
				rsData->Fin();
				delete rsData;
				if(mfcRec.m_FormSeq == ID_FORMNO_051)	{
					rsData = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pDB));
				}
				else	{
					rsData = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pDB));
				}
				//if( rsData->GetCountDataRecord(1) != 0 ){
				if(rsData->GetPrintPageNum() != 0)	{
					hasData = TRUE;
				}
			}
// midori 155015 add <--
			YOUSHIKI_SEL ys;
			ys.FormSeq = mfcRec.m_FormSeq;
			ys.hasData = hasData;
			m_vecDataExist.push_back(ys);
		}

		delete rsData;		
		mfcRec.MoveNext();
	}
}
void CdlgYoushikiSelect::OnNMReportSelchangedCUSTOM_REPORT_YOUSHIKI_LIST(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更後の行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();

	UpdateYoushikiMoveButton();
}

void CdlgYoushikiSelect::OnNMDblclkReportYoushikiList(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();

	PostMessage(WM_COMMAND, ID_OK_BUTTON);
}

int CdlgYoushikiSelect::GetRecordDataOrder(int index, BOOL mode)
{
	int ret_order = -1;
	if(mode != FALSE){
		int cnt = 0;
		for(vector<YOUSHIKI_SEL>::iterator it = m_vecDataExist.begin(); it != m_vecDataExist.begin() + index; it++){
			if(it->hasData != FALSE){
				//ret_order = it - m_vecDataExist.begin();
				ret_order = cnt;
			}
			cnt++;
		}
	}
	else{
		vector<YOUSHIKI_SEL>::iterator it = find((m_vecDataExist.begin() + index + 1), m_vecDataExist.end(), TRUE);
		if(it != m_vecDataExist.end()){
			ret_order = (int)(it - m_vecDataExist.begin());
		}
	}

	return ret_order;
}

//**************************************************
//	各様式にデータがある（背景色を黄色にする）かどうかのマップ作成
//	【引数】	mode:		TRUE/現在の様式より前 FALSE/後ろ
//				move:		TRUE/強制的に直前or直後 FALSE/m_fYoushikiSkip
//	【戻値】	なし
//**************************************************
void CdlgYoushikiSelect::MoveSelectedRow(BOOL mode, BOOL move, int move_dist)
{
	if(GetFocusControlID(this) == IDC_CUSTOM_REPORT_YOUSHIKI_LIST)	m_report.SetFocus();
	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL){
		m_report.SetFocus();
		return;
	}
	int selected_index = r_row->GetIndex();
	int next_index = selected_index;

	if( move == FALSE ){
		if(mode != FALSE){
			if( selected_index != 0 )	next_index = selected_index - 1 - move_dist;
			if(next_index < 0)	next_index = 0;
		}
		else{
			if( selected_index != UC_ID_FORMNO_MAX - 1 )	next_index = selected_index + 1 + move_dist;
			if(next_index > UC_ID_FORMNO_MAX - 1)	next_index = UC_ID_FORMNO_MAX - 1;
		}
		if(next_index != selected_index) m_report.SetUnSelectedRows();
		//m_report.SetSelectedRows(next_index, next_index);
		m_report.SetFocusedRow(next_index);
	}
	else{
		next_index = GetRecordDataOrder(selected_index, mode);
		if(next_index != selected_index) m_report.SetUnSelectedRows();
		if(next_index != -1)	m_report.SetSelectedRows(next_index, next_index);
	}

	//　ボタンのenable/disable
	UpdateYoushikiMoveButton();

	s_rows = m_report.GetSelectedRows();
	r_row = s_rows->GetAt(0);
	if(r_row != NULL)	m_report.EnsureVisible(r_row);		// 一番下でF8押下するとr_row==NULLになり、フォーカスが飛ぶ
}

void CdlgYoushikiSelect::SetGrid()
{
// midori 161108 cor -->
	//CDWordArray arryGridColumn;
	//arryGridColumn.Add(0);
	//CDWordArray arrayGridColor;
	//arrayGridColor.Add(RGB(255, 0, 0));
	//m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
// ---------------------
	CDWordArray	arryGridColumn;
	CDWordArray	arrayGridColor;

	arryGridColumn.Add(0);
	arrayGridColor.Add(RGB(255, 0, 0));
	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
	arryGridColumn.Add(1);
	arrayGridColor.Add(RGB(255, 0, 0));
	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
// midori 161108 cor <--
}