// dlgDeleteDataEachYoushiki.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgDeleteDataEachYoushiki.h"


// CdlgDeleteDataEachYoushiki ダイアログ

IMPLEMENT_DYNAMIC(CdlgDeleteDataEachYoushiki, ICSDialog)

CdlgDeleteDataEachYoushiki::CdlgDeleteDataEachYoushiki(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgDeleteDataEachYoushiki::IDD, pParent)
{

}

CdlgDeleteDataEachYoushiki::~CdlgDeleteDataEachYoushiki()
{
}

void CdlgDeleteDataEachYoushiki::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_REPORT1, m_report);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_ALL_TUJO, m_btn_All_Tsujo);
	DDX_Control(pDX, IDC_BUTTON_ALL_HOKAN, m_btn_All_Hokan);
}


BEGIN_MESSAGE_MAP(CdlgDeleteDataEachYoushiki, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALL_TUJO, &CdlgDeleteDataEachYoushiki::OnBnClickedButtonAllTujo)
	ON_BN_CLICKED(IDC_BUTTON_ALL_HOKAN, &CdlgDeleteDataEachYoushiki::OnBnClickedButtonAllHokan)
END_MESSAGE_MAP()


// CdlgDeleteDataEachYoushiki メッセージ ハンドラ

BOOL CdlgDeleteDataEachYoushiki::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// コントロール初期化
	InitControls();

	// データセット
	if( GetData() == FALSE ){
		ICSMessageBox( "様式データを取得できません。処理を終了します。", MB_ICONSTOP );
		EndDialog(ID_DLG_CANCEL);
	}

	// フォーカスをレポートコントロールにあてる
	m_report.MoveDown(FALSE, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//	【戻値】	ID_DLG_OK		…	OKボタン押された
//				ID_DLG_CANCEL	…	キャンセルボタン押された
//**************************************************
int CdlgDeleteDataEachYoushiki::ShowDialog(CDatabase* pDB /*=NULL*/, CMap<int, int, DELETE_YOUSHIKI, DELETE_YOUSHIKI> &map)
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
	m_map.InitHashTable( UC_ID_FORMNO_MAX + 10/*最大値の20%を指定すると高速化するようだ*/ );
	
	//	データベースハンドルあり？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;			//	データベースハンドル
				DoModal();				//	モーダルで表示
			}
		}
		catch(...){
		}
	}
	// キャンセルならここでリターン
	if( m_nRet != ID_DLG_OK )	return m_nRet;

	// 削除ボタン押下ならマップコピー
	POSITION pos = m_map.GetStartPosition();
	while( pos != NULL ){
		int nKey = 0;
		DELETE_YOUSHIKI stValue;

		// キーと値取得
		m_map.GetNextAssoc( pos, nKey, stValue );

		// コピー
		map[ nKey ] = stValue;
	}

	//	戻値を返す
	return( m_nRet );
}

//**************************************************
//	コントロール初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgDeleteDataEachYoushiki::InitControls()
{
	// レポートコントロールカラム設定
	ICSReportColumns	*m_pClms	= NULL;
	ICSReportColumn		*m_pClm		= NULL;

	m_report.Resize( TRUE, ICSResizeDefault );
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
	m_report.AddColumn(new ICSReportColumn(0, NULL, 1));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 1));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 2));
	m_report.AddColumn(new ICSReportColumn(3, NULL, 10));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	m_pClms	= m_report.GetColumns();

	for ( int i = 0; i < 4; i++ ) {
		m_pClm = m_pClms->GetAt( i );	

		switch( i ){
			case 0:
				m_pClm->SetCaption ( _T("通常") );
				break;
			case 1:
				m_pClm->SetCaption ( _T("保管") );
				break;
			case 2:
				m_pClm->SetCaption ( _T("様式番号") );
				break;
			case 3:
				m_pClm->SetCaption ( _T("様式名称") );
				break;
		}
	}
}

//**************************************************
//	コントロールにデータをセットする
//	【引数】	なし
//	【戻値】	BOOL
//**************************************************
int CdlgDeleteDataEachYoushiki::GetData()
{
	CdbUcInfSub	mfcRec( m_pDB );

	ICSReportRecordItem		*pItem		= NULL;
	for( int i=0; i<UC_ID_FORMNO_MAX; i++ ){
		if( mfcRec.RequeryFormOrder( i + 1 ) != DB_ERR_OK ){
			mfcRec.Fin();
			return FALSE;
		}

		ICSReportRecord*  pRecord  =  m_report.AddRecord( new ICSReportRecord() );
		pItem  =  pRecord->AddItem( new ICSReportRecordItemCheckBox());	// 通常
		pItem  =  pRecord->AddItem( new ICSReportRecordItemCheckBox());	// 保管

		pItem  =  pRecord->AddItem( new ICSReportRecordItemText( mfcRec.m_TitleNo + mfcRec.m_TitleNo2 ));	// 様式番号
		pItem  =  pRecord->AddItem( new ICSReportRecordItemText( mfcRec.m_Title ));	// 様式名称
	}
	mfcRec.Fin();

	m_report.Populate();

	return TRUE;
}

//**************************************************
//	通常データ全選択/全解除ボタン押下イベント
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgDeleteDataEachYoushiki::OnBnClickedButtonAllTujo()
{
	//ChangeColumnAll( COLUMN_TSUJO );
	m_clsFunc.ChangeReportColumnAll(&m_report, COLUMN_TSUJO);
}

//**************************************************
//	保管データ全選択/全解除ボタン押下イベント
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgDeleteDataEachYoushiki::OnBnClickedButtonAllHokan()
{
	//ChangeColumnAll( COLUMN_HOKAN );
	m_clsFunc.ChangeReportColumnAll(&m_report, COLUMN_HOKAN);
}

//**************************************************
//	全選択/全解除制御
//　１つでもチェックが入っていれば全てオフにし、
//　１つもチェックが入っていないときのみ、全てオンにする
//	【引数】	int		0:通常データ　1:保管データ
//	【戻値】	なし
//**************************************************
void CdlgDeleteDataEachYoushiki::ChangeColumnAll(int column)
{
	int cnt = 0;

	// チェックが入っている数取得
	for(int i=0; i<UC_ID_FORMNO_MAX; i++){
		if( m_report.IsChecked( i, column ) == TRUE )	cnt++;
	}

	// チェック数が０以外は全てオフ
	if( cnt > 0 ){
		for(int i=0; i<UC_ID_FORMNO_MAX; i++){
			m_report.SetChecked(i, column, FALSE);
		}
	}
	else{
		for(int i=0; i<UC_ID_FORMNO_MAX; i++){
			m_report.SetChecked(i, column, TRUE);
		}
	}

	m_report.Populate();
}

//**************************************************
//	メッセージ処理
//	Enterキーで次の項目へ　最終項目でOKボタンに移動
//	スペースキーでチェックON/OFF
//**************************************************
BOOL CdlgDeleteDataEachYoushiki::PreTranslateMessage(MSG* pMsg)
{
	ICSReportRecordItem		*pItem1		= NULL;
	ICSReportRecordItem		*pItem2		= NULL;
	ICSReportRow			*pRow		= NULL;
	ICSReportSelectedRows	*pSelRows	= NULL;
	int		nRow;
	int		nClm;
	char	cShift = 0;	//	SHIFTキー状態

	// ダイアログにフォーカスがない場合は処理しない
	// （他のウィンドウに移った時など）
	if( GetFocus() != NULL){
		if( pMsg->message == WM_KEYDOWN ) {
			// レポートコントロールにフォーカスが当たっているときのみチェック
			if( GetFocus()->GetDlgCtrlID() == m_report.GetDlgCtrlID() ){
				// キーが押された
				if( pMsg->message == WM_KEYDOWN ) {
					switch (pMsg->wParam) {
						case VK_RETURN:
							//	SHIFTキー状態取得
							cShift = ( char )GetKeyState( VK_SHIFT );
							// 選択行インデックスの取得
							pSelRows	= m_report.GetSelectedRows();
							pRow		= pSelRows->GetAt(0);
							nRow		= pRow->GetIndex();

							pItem1 = m_report.GetActiveItem();

							if( cShift & 0x80 ){	// SHIFTキーが押されてる？
								if( pItem1 != NULL ){
									nClm = pItem1->GetIndex();
									if( nClm == COLUMN_HOKAN ){	// フォーカスが通常　→保管へ
										m_report.EditItem( nRow, COLUMN_TSUJO, TRUE );
									}
									else if( nClm == COLUMN_TSUJO ){	// フォーカスが保管　→次の行へ
										if( nRow != 0 ){	// 一番最初の行以外
											m_report.MoveUp( FALSE, FALSE );
											pSelRows = m_report.GetSelectedRows();
											pRow = pSelRows->GetAt( 0 );	// 選択行は1行のみなので先頭要素を抜き出す
											nRow = pRow->GetIndex();
											m_report.EditItem( nRow, COLUMN_HOKAN, TRUE );
										}
										else{
											m_report.EditItem( nRow, COLUMN_TSUJO, TRUE );
										}
									}
								}
								else{
									m_report.EditItem( nRow, COLUMN_TSUJO, TRUE );
								}
							}
							else{
								if( pItem1 != NULL ){
									nClm = pItem1->GetIndex();
									if( nClm == COLUMN_TSUJO ){	// フォーカスが通常　→保管へ
										m_report.EditItem( nRow, COLUMN_HOKAN, TRUE );
									}
									else if( nClm == COLUMN_HOKAN ){	// フォーカスが保管　→次の行へ
										if( nRow != UC_ID_FORMNO_MAX - 1 ){	// 最終行以外
											m_report.MoveDown( FALSE, FALSE );
											pSelRows = m_report.GetSelectedRows();
											pRow = pSelRows->GetAt( 0 );	// 選択行は1行のみなので先頭要素を抜き出す
											nRow = pRow->GetIndex();
											m_report.EditItem( nRow, COLUMN_TSUJO, TRUE );
										}
										else{	// 最終行　→ＯＫボタンへ
											m_btnOK.SetFocus();
										}
									}
								}
								else{
									m_report.EditItem( nRow, COLUMN_TSUJO, TRUE );
								}
							}
							return TRUE;

						case VK_SPACE:		// Spaceキー
							// 選択行インデックスの取得
							pSelRows	= m_report.GetSelectedRows();
							pRow		= pSelRows->GetAt(0);
							nRow		= pRow->GetIndex();

							// 選択カラムインデックスの取得
							pItem1 = m_report.GetActiveItem();
							if( pItem1 != NULL ){
								nClm = pItem1->GetIndex();
								if( nClm == COLUMN_TSUJO || nClm == COLUMN_HOKAN){
									if( m_report.IsChecked(nRow, nClm ) != FALSE ){
										m_report.SetChecked(nRow, nClm, FALSE );
									}
									else{
										m_report.SetChecked(nRow, nClm, TRUE );
									}
									m_report.Populate();
									m_report.EditItem( nRow, nClm, TRUE );
								}
							}
							return TRUE;
					}
				}
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

void CdlgDeleteDataEachYoushiki::OnOK()
{
	CdbUcInfSub	mfcRec( m_pDB );

	// チェック状態のチェック
	int cnt = 0;
	for(int i=0; i< UC_ID_FORMNO_MAX; i++){

		if( mfcRec.RequeryFormOrder( i + 1 ) != DB_ERR_OK )		return;

		if( m_report.IsChecked( i, COLUMN_TSUJO ) == TRUE ){
			m_map[ mfcRec.m_FormSeq ].tsujo = TRUE;
			cnt++;
		}
		if( m_report.IsChecked( i, COLUMN_HOKAN ) == TRUE ){
			m_map[ mfcRec.m_FormSeq ].hokan = TRUE;
			cnt++;
		}
	}
	mfcRec.Fin();

	// 1件も選択されてない？
	if( cnt < 1 ){
		ICSMessageBox( _T("様式が１件も選択されていません。"), MB_OK, 0, 0, this );
		m_report.SetFocus();
		return;
	}

	// 確認ダイアログ
	if( ICSMessageBox( _T("選択した様式の全データを削除します。\nF9参照内の科目、相手先等は削除されません。"
		"\n削除したデータは元に戻せませんのでご注意ください。\n\n削除してもよろしいですか？"), MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 ) != IDOK ){
		return;
	}

	// 戻り値セット
	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

void CdlgDeleteDataEachYoushiki::SetGrid()
{
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	arryGridColumn.Add(2);

	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}