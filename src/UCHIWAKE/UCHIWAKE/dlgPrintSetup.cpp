//--------------------------------------------------
//	dlgPrintSetup.cpp
//
//	2006.02.19～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgprintsetup.h"
// midori 170105 add -->
#include "dlgIkkatuHenkou.h"
// midori 170105 add <--

// midori 190301 add -->
extern	BOOL	bG_Kanso;
// midori 190301 add <--
// 改良No.21-0086,21-0529 cor -->
//// midori 156189,156190,156191 add -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156189,156190,156191 add <--
// ------------------------------
extern	BOOL	bG_InvNo;

extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

IMPLEMENT_DYNAMIC(CdlgPrintSetup, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgPrintSetup::CdlgPrintSetup(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgPrintSetup::IDD, pParent)
{
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgPrintSetup::CdlgPrintSetup( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgPrintSetup::~CdlgPrintSetup()
{
}

void CdlgPrintSetup::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YOUSHIKI_COMBO, m_cboYoushiki);
	DDX_Control(pDX, IDC_ITEM_LABEL1, m_lblItem1);
	DDX_Control(pDX, IDC_ITEM_LABEL2, m_lblItem2);
	DDX_Control(pDX, IDC_ITEM_LABEL3, m_lblItem3);
	DDX_Control(pDX, IDC_ITEM_LABEL4, m_lblItem4);
	DDX_Control(pDX, IDC_OMISSION_ON1, m_rdoOn1);
	DDX_Control(pDX, IDC_OMISSION_ON2, m_rdoOn2);
	DDX_Control(pDX, IDC_OMISSION_ON3, m_rdoOn3);
	DDX_Control(pDX, IDC_OMISSION_ON4, m_rdoOn4);
	DDX_Control(pDX, IDC_OMISSION_OFF1, m_rdoOff1);
	DDX_Control(pDX, IDC_OMISSION_OFF2, m_rdoOff2);
	DDX_Control(pDX, IDC_OMISSION_OFF3, m_rdoOff3);
	DDX_Control(pDX, IDC_OMISSION_OFF4, m_rdoOff4);
	DDX_Control(pDX, IDC_OMISSIONSTRING_TEXT1, m_txtOmission1);
	DDX_Control(pDX, IDC_OMISSIONSTRING_TEXT2, m_txtOmission2);
	DDX_Control(pDX, IDC_OMISSIONSTRING_TEXT3, m_txtOmission3);
	DDX_Control(pDX, IDC_OMISSIONSTRING_TEXT4, m_txtOmission4);
	DDX_Control(pDX, IDC_CONAME_CHECK, m_chkCoName);
	DDX_Control(pDX, IDC_SETTLEMENT_CHECK, m_chkSettlement);
	DDX_Control(pDX, IDC_MONEY_CHECK, m_chkMoney);
// midori 200101 add -->
	DDX_Control(pDX, IDC_GENGO_CHECK, m_chkGengo);
// midori 200101 add <--
	DDX_Control(pDX, IDC_PAGE_CHECK, m_chkPage);
	DDX_Control(pDX, IDC_ZERO_OUTPUT_CHECK, m_chkZero);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
// midori 170105 add -->
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
// midori 170105 add <--
	DDX_Control(pDX, IDC_ITEM_LABEL5, m_lblItem5);
	DDX_Control(pDX, IDC_ITEM_LABEL6, m_lblItem6);
	DDX_Control(pDX, IDC_OMISSION_ON5, m_rdoOn5);
	DDX_Control(pDX, IDC_OMISSION_ON6, m_rdoOn6);
	DDX_Control(pDX, IDC_OMISSION_OFF5, m_rdoOff5);
	DDX_Control(pDX, IDC_OMISSION_OFF6, m_rdoOff6);
	DDX_Control(pDX, IDC_OMISSIONSTRING_TEXT5, m_txtOmission5);
	DDX_Control(pDX, IDC_OMISSIONSTRING_TEXT6, m_txtOmission6);
	DDX_Control(pDX, IDC_KAMOKU_CHECK, m_chkKamoku);
	DDX_Control(pDX, IDC_ZERO_HIDE_CHECK, m_chkZeroHide);
	DDX_Control(pDX, IDC_KINYU_LINE_CHECK, m_chkKinyuLine);
// midori 190505 del -->
//	DDX_Control(pDX, IDC_KAMOKU_ROW, m_chkKamokuRow);
// midori 190505 del <--
// midori 156188 add -->
	DDX_Control(pDX, IDC_KAMOKU_ROW, m_chkKamokuRow);
// midori 156188 add <--
	DDX_Control(pDX, IDC_NULL_OUTPUT_CHECK, m_chkNull);
}


BEGIN_MESSAGE_MAP(CdlgPrintSetup, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_YOUSHIKI_COMBO, &CdlgPrintSetup::OnCbnSelchangeYoushikiCombo)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgPrintSetup::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OMISSION_ON1, &CdlgPrintSetup::OnBnClickedOmissionOn1)
	ON_BN_CLICKED(IDC_OMISSION_OFF1, &CdlgPrintSetup::OnBnClickedOmissionOff1)
	ON_BN_CLICKED(IDC_OMISSION_ON2, &CdlgPrintSetup::OnBnClickedOmissionOn2)
	ON_BN_CLICKED(IDC_OMISSION_OFF2, &CdlgPrintSetup::OnBnClickedOmissionOff2)
	ON_BN_CLICKED(IDC_OMISSION_ON3, &CdlgPrintSetup::OnBnClickedOmissionOn3)
	ON_BN_CLICKED(IDC_OMISSION_OFF3, &CdlgPrintSetup::OnBnClickedOmissionOff3)
	ON_BN_CLICKED(IDC_OMISSION_ON4, &CdlgPrintSetup::OnBnClickedOmissionOn4)
	ON_BN_CLICKED(IDC_OMISSION_OFF4, &CdlgPrintSetup::OnBnClickedOmissionOff4)
	ON_BN_CLICKED(IDC_OMISSION_ON5, &CdlgPrintSetup::OnBnClickedOmissionOn5)
	ON_BN_CLICKED(IDC_OMISSION_OFF5, &CdlgPrintSetup::OnBnClickedOmissionOff5)
	ON_BN_CLICKED(IDC_OMISSION_ON6, &CdlgPrintSetup::OnBnClickedOmissionOn6)
	ON_BN_CLICKED(IDC_OMISSION_OFF6, &CdlgPrintSetup::OnBnClickedOmissionOff6)
	ON_BN_CLICKED(IDC_ZERO_OUTPUT_CHECK, &CdlgPrintSetup::OnBnClickedZeroOutputCheck)
	ON_BN_CLICKED(IDC_MONEY_CHECK, &CdlgPrintSetup::OnBnClickedMoneyCheck)
// midori 170105 add -->
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgPrintSetup::OnBnClickedButton1)
// midori 170105 add <--
END_MESSAGE_MAP()


void CdlgPrintSetup::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgPrintSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgPrintSetup::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	//	様式コンボボックス初期化
	InitYoushikiCombo( m_pDB, m_nFormSeq );
	//	様式コンボボックス変更
	ChangeYoushikiCombo( m_pDB, m_nFormSeq );
	//	出力指定取得
	GetOutputSetup( m_pDB );
// midori 160608 add -->
	// 「帳票イメージのみ出力する」にチェックが有る場合、画面のコントロールを無効にする
	if(m_nOutZero == 1)	{
		// 金額欄を空欄で出力する
		m_chkMoney.EnableWindow(FALSE);
		// 金額０円データを出力しない
		m_chkZero.EnableWindow(FALSE);
		// 金額空欄データを出力しない
		m_chkNull.EnableWindow(FALSE);
// midori UC_0061 add -->
		// 金額欄一括変更ボタン
		m_btn1.EnableWindow(FALSE);
// midori UC_0061 add <--
	}
// midori 160608 add <--
// midori 200101 add -->
	// 日付に元号を出力する
	if(bG_Kanso == TRUE)	m_chkGengo.EnableWindow(TRUE);
	else					m_chkGengo.EnableWindow(FALSE);
// midori 200101 add <--
	// リサイズ
	OnInitDialogEX();
	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	選択変更（様式）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnCbnSelchangeYoushikiCombo()
{
	int				nYoushikiIndex;		//	様式コンボボックスのインデックス
	int				nFormSeq = 0;
	int				nFormOrder = 0;
	
	//	様式コンボボックスのインデックス取得
	nYoushikiIndex = m_cboYoushiki.GetCurSel() + 1;
	//	シーケンス番号取得
	nFormSeq = GetFormSeq( nYoushikiIndex );
	
	if ( nFormSeq == 0 )			return;	//	シーケンス番号取得失敗？
	if ( m_nFormSeq == nFormSeq )	return;	//	様式を変えた？

	//	様式データ変更あり？
	if ( CheckYoushikiData( m_pDB, m_nFormSeq ) != 0 ){
		//	保存確認OK？
		if ( ICSMessageBox( _T("帳表別印刷設定が変更されています。\n変更内容を保存しますか？"), MB_YESNO, 0, 0, this ) == IDYES ){
			//	様式データ設定
			SetYoushikiData( m_pDB, m_nFormSeq );
		}
	}

	//	最新の様式シーケンス番号を取得
	m_nFormSeq = nFormSeq;
	//	様式変更
	ChangeYoushikiCombo( m_pDB, m_nFormSeq );
	// 金額０を印字するチェックボックスを、金額欄を出力するチェックボックスの状態で変化させる（移動前と移動後の金額０円出力チェックボックスの状態が違う可能性がある為）
	SetChkZeroHideState();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOk()
{
// midori 157099,157119 add -->
	// 確認メッセージを表示するためのチェック
	if(YoushikiDataChk(m_pDB, m_nFormSeq) != 0)	{
		m_chkKamokuRow.SetFocus();
		return;
	}
// midori 157099,157119 add <--

	//	様式データ設定
	SetYoushikiData( m_pDB, m_nFormSeq );
	//	出力指定設定
	SetOutputSetup( m_pDB );
	//　戻り値セット
	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgPrintSetup::PreTranslateMessage(MSG* pMsg)
{
	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

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
//	ラジオボタンクリック（する１）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOn1()
{
	//	省略確認
	CheckOmit( &m_txtOmission1, TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（しない１）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOff1()
{
	//	省略確認
	CheckOmit( &m_txtOmission1, FALSE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（する２）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOn2()
{
	//	省略確認
	CheckOmit( &m_txtOmission2, TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（しない２）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOff2()
{
	//	省略確認
	CheckOmit( &m_txtOmission2, FALSE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（する３）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOn3()
{
	//	省略確認
	CheckOmit( &m_txtOmission3, TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（しない３）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOff3()
{
	//	省略確認
	CheckOmit( &m_txtOmission3, FALSE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（する４）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOn4()
{
	//	省略確認
	CheckOmit( &m_txtOmission4, TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（しない４）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOff4()
{
	//	省略確認
	CheckOmit( &m_txtOmission4, FALSE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（する５）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOn5()
{
	//	省略確認
	CheckOmit( &m_txtOmission5, TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（しない５）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOff5()
{
	//	省略確認
	CheckOmit( &m_txtOmission5, FALSE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（する６）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOn6()
{
	//	省略確認
	CheckOmit( &m_txtOmission6, TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（しない６）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintSetup::OnBnClickedOmissionOff6()
{
	//	省略確認
	CheckOmit( &m_txtOmission6, FALSE );
}

//**************************************************
//	様式コンボボックス初期化
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::InitYoushikiCombo( CDatabase* pDB, short nFormSeq )
{
	int					nCnt;			//	カウント用
	CString				szBuf;			//	バッファ
	CString				szShow;			//	表示用
	CdbUcInfSub			mfcRec( pDB );	//	uc_inf_subテーブルクラス
	int					nIndex = 0;		//	インデックス番号
	CdlgAddressSearch	clsAdd( this );	//	住所検索クラス（※文字列操作メソッドのみ使用）

	//	コンボボックスをクリア
	m_cboYoushiki.ResetContent();
	
	try{
		
		//	様式数分ループ
		for( nCnt = 0; nCnt < UC_ID_FORMNO_MAX; nCnt++ ){
			
			//	初期化成功？
			if ( mfcRec.Init( nCnt + 1 ) == DB_ERR_OK ){
				
				//	レコードあり？
				if ( !( mfcRec.IsEOF() ) ){
					szBuf.Empty();									//	バッファクリア
					szShow.Empty();									//	表示用 
					szBuf = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;	//	タイトル番号文字列連結
					szBuf += "．" ;									//	ドット連結
					szBuf +=  mfcRec.m_Title;						//	タイトル文字列連結
					clsAdd.GetSpritString( szBuf, &szShow, 64 );	//	表示範囲までの文字列を切り取り
					m_cboYoushiki.AddString( szShow );				//	コンボボックスに追加
					
					//	シーケンス番号が一致した？
					if ( mfcRec.m_FormSeq == nFormSeq ){
						//	インデックス番号を取得
						nIndex = nCnt;
					}
				}
				//	レコード閉じる
				mfcRec.Fin();
			}
		}
	}
	catch(...){
		
		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	
	//	インデックスが範囲内か？
	if (( nIndex >= 0 ) && ( nIndex < UC_ID_FORMNO_MAX )){
		//	指定インデックスにカーソルセット
		m_cboYoushiki.SetCurSel( nIndex );
	}
}

//**************************************************
//	様式コンボボックス変更
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::ChangeYoushikiCombo( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSubOmit		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	//	省略項目初期化
	InitOmissionItem( FALSE );

	try{
		//	初期化成功？
		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();		//	レコードを先頭に移動

				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					
					//	カウントで分岐
					switch( mfcRec.m_ItemSeq ){
					//	1項目目
					case 1:
						//	省略データ取得
						GetOmissionData( &mfcRec, &m_lblItem1, &m_rdoOn1, &m_rdoOff1, &m_txtOmission1 );
						break;
					//	2項目目
					case 2:
						//	省略データ取得
						GetOmissionData( &mfcRec, &m_lblItem2, &m_rdoOn2, &m_rdoOff2, &m_txtOmission2 );
						break;
					//	3項目目
					case 3:
						//	省略データ取得
						GetOmissionData( &mfcRec, &m_lblItem3, &m_rdoOn3, &m_rdoOff3, &m_txtOmission3 );
						break;
					//	4項目目
					case 4:
						//	省略データ取得
						GetOmissionData( &mfcRec, &m_lblItem4, &m_rdoOn4, &m_rdoOff4, &m_txtOmission4 );
						break;
					//	5項目目
					case 5:
						//	省略データ取得
						GetOmissionData( &mfcRec, &m_lblItem5, &m_rdoOn5, &m_rdoOff5, &m_txtOmission5 );
						break;
					//	6項目目
					case 6:
						//	省略データ取得
						GetOmissionData( &mfcRec, &m_lblItem6, &m_rdoOn6, &m_rdoOff6, &m_txtOmission6 );
						break;
					default:
						break;
					}
					mfcRec.MoveNext();	//	次のレコードへ移動
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
	
	//	様式コンボボックス変更（サブ）
	ChangeYoushikiComboSub( pDB, nFormSeq );
}

//**************************************************
//	様式コンボボックス変更（サブ）
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::ChangeYoushikiComboSub( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
// midori 190301 add -->
	int				sw=0;
// midori 190301 add <--
	
	//	シーケンス番号で絞込み
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
			
			//************************************
			// 科目
			//************************************
			//	科目名タイトルが使用不可？
			if ( mfcRec.m_OpTitleKnFg == 0 ){
				//	無効に設定
				m_chkKamoku.EnableWindow( FALSE );
				//	チェックオフ
				m_chkKamoku.SetCheck( FALSE );
			}
			//	使用可能
			else {
// 改良No.21-0086,21-0529 cor -->
//// midori 160608 cor -->
////				//	有効に設定
////				m_chkKamoku.EnableWindow( TRUE );
//// ---------------------
//				if(m_nOutZero == 1)	m_chkKamoku.EnableWindow( FALSE );	//	無効に設定
//				else				m_chkKamoku.EnableWindow( TRUE );	//	有効に設定
//// midori 160608 cor <--
//				
//				//	科目名タイトル使わない？
//				if ( mfcRec.m_OpTitleKn == 0 ){
//					//	チェックオフ
//					m_chkKamoku.SetCheck( FALSE );
//				}
//				//	使う
//				else{
//					//	チェックオン
//					m_chkKamoku.SetCheck( TRUE );
//				}
// ------------------------------
				if(bG_InvNo == FALSE && mfcRec.m_FormSeq == ID_FORMNO_081) {
					m_chkKamoku.EnableWindow(FALSE);	// 無効に設定
					m_chkKamoku.SetCheck(FALSE);		// チェックオフ
				}
				else {
					if(m_nOutZero == 1)	m_chkKamoku.EnableWindow(FALSE);	// 無効に設定
					else				m_chkKamoku.EnableWindow(TRUE);		// 有効に設定

					// 科目名タイトル使わない？
					if(mfcRec.m_OpTitleKn == 0)	{
						m_chkKamoku.SetCheck(FALSE);	// チェックオフ
					}
					else	{
						m_chkKamoku.SetCheck(TRUE);		// チェックオン
					}
				}
// 改良No.21-0086,21-0529 cor <--
			}

			//************************************
			// 「金融機関を一段表示する」
			//************************************
			if( mfcRec.m_GeneralConstVal[15] & BIT_D0 ){	// 使用可能
// midori 190301 del -->
//// midori 160608 cor -->
////				m_chkKinyuLine.EnableWindow( TRUE );
//// ---------------------
//				if(m_nOutZero == 1)	m_chkKinyuLine.EnableWindow( FALSE );
//				else				m_chkKinyuLine.EnableWindow( TRUE );
//// midori 160608 cor <--
// midori 190301 del <--
// midori 190301 add -->
				if(m_nOutZero == 1)	m_chkKinyuLine.EnableWindow( FALSE );
				else				{
					sw = 0;
					// ①預貯金等の内訳書、⑧支払手形の内訳書の新様式は
					// 金融機関名と支店名が常に一段で出力されるため選択不可にする
					if(mfcRec.m_FormSeq == ID_FORMNO_011 || mfcRec.m_FormSeq == ID_FORMNO_081)	{
						if(bG_Kanso == TRUE)	{
							sw = 1;
						}
					}
					if(sw == 0)	m_chkKinyuLine.EnableWindow(TRUE);
					else		m_chkKinyuLine.EnableWindow(FALSE);
				}
// midori 190301 add <--
				CCtrlSubGeneralVar cs( mfcRec );
				if( cs.bKinyuLine ){	//チェック入ってる？
					m_chkKinyuLine.SetCheck( TRUE );
				}
				else{
					m_chkKinyuLine.SetCheck( FALSE );
				}
			}
			else{	// 使用不可
				m_chkKinyuLine.EnableWindow( FALSE );
				m_chkKinyuLine.SetCheck( FALSE );
			}

// midori 190505 del -->
//			//************************************
//			// 「科目行を出力する」
//			//************************************
//			CCtrlSubGeneralVar cs( mfcRec );
//			if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){	// 使用可能
//// midori 160608 cor -->
////				m_chkKamokuRow.EnableWindow( TRUE );
//// ---------------------
//				if(m_nOutZero == 1)	m_chkKamokuRow.EnableWindow( FALSE );
//				else				m_chkKamokuRow.EnableWindow( TRUE );
//// midori 160608 cor <--
//
//				if( cs.bOutKamokuRow ){	//チェック入ってる？
//					m_chkKamokuRow.SetCheck( TRUE );
//				}
//				else{
//					m_chkKamokuRow.SetCheck( FALSE );
//				}
//			}
//			else{	// 使用不可
//				m_chkKamokuRow.EnableWindow( FALSE );
//				m_chkKamokuRow.SetCheck( FALSE );
//			}
// midori 190505 del <--
// midori 190505 add -->
			CCtrlSubGeneralVar cs( mfcRec );
// midori 190505 add <--
// midori 156188 add -->
			//************************************
			// 「科目行を出力する」
			//************************************
// midori 157042 del -->
			//if(KamokuRowEnableSgn(m_pDB,0) == 1) {
			//	m_chkKamokuRow.EnableWindow(FALSE);
			//	m_chkKamokuRow.ShowWindow(SW_HIDE);
			//}
			//else {
// midori 157042 del <--
			if(mfcRec.m_GeneralConstVal[15] & BIT_D1)	{	// 使用可能
// midori 157099,157119 del -->
				//if(m_nOutZero == 1)	m_chkKamokuRow.EnableWindow( FALSE );
				//else				m_chkKamokuRow.EnableWindow( TRUE );
// midori 157099,157119 del <--
// midori 157099,157119 add -->
				// 「科目行を出力する」の制御
				BOOL			bEna=FALSE;
				int				kmkrowsw=0;		// 0:入力データに科目行がない or ソートダイアログ「科目行を挿入する」非表示(画面に科目行挿入を可能にするの対応前）
												// 1:入力データに科目行がある
												// 2:科目指定から呼ばれた（科目行に対する処理は不要）
				CdbUc000Common*	prc=NULL;

				while(true)	{
					// 「帳表イメージのみ出力する」サイン
					if(m_nOutZero == 1)	{
						bEna = FALSE;
						break;
					}
					// 科目指定－科目指定を行う
					if(mfcRec.m_GeneralVar[1] & BIT_D3)	{
						bEna = FALSE;
						break;
					}
					// 画面上のデータに「科目行」がないかどうか
					if(nFormSeq == ID_FORMNO_021)							prc = (CdbUc000Common*)(new CdbUc021Uketoritegata(pDB));
					else if(nFormSeq == ID_FORMNO_081 && bG_InvNo == TRUE)	prc = (CdbUc000Common*)(new CdbUc081Siharaitegata(pDB));	// 改良No.21-0086,21-0529 add
					else if(nFormSeq == ID_FORMNO_111)						prc = (CdbUc000Common*)(new CdbUc111Kariirekin(pDB));
					if(prc)	{
						kmkrowsw = prc->GetKmkRowSw();
						if(kmkrowsw == 0)	bEna = TRUE;
						else				bEna = FALSE;
						break;
					}
					else	{
						bEna = FALSE;
						break;
					}
					break;
				}
				m_chkKamokuRow.EnableWindow(bEna);
// midori 157099,157119 add <--

				if(cs.bOutKamokuRow)	{	//チェック入ってる？
					m_chkKamokuRow.SetCheck(TRUE);
				}
				else	{
					m_chkKamokuRow.SetCheck(FALSE);
				}
			}
			else	{	// 使用不可
				m_chkKamokuRow.EnableWindow(FALSE);
				m_chkKamokuRow.SetCheck(FALSE);
			}
// midori 157042 del -->
			//}
// midori 157042 del <--
// midori 156188 add <--
			//************************************
			// 「金額０円データを出力しない」
			//************************************
			if(cs.bPrintZeroData)	m_chkZero.SetCheck(TRUE);
			else					m_chkZero.SetCheck(FALSE);
			//************************************
			// 「金額空欄データを出力しない」
			//************************************
			if(cs.bPrintNullData)	m_chkNull.SetCheck(TRUE);
			else					m_chkNull.SetCheck(FALSE);
			//************************************
			// 「金額０円を印字する」（0でOFF　0以外でON）
			//************************************
			if(cs.bPrintHideZero)	m_chkZeroHide.SetCheck(FALSE);
			else					m_chkZeroHide.SetCheck(TRUE);
		}
		//	閉じる
		mfcRec.Fin();
	}
}

// midori 170105 add -->
//**************************************************
//	様式コンボボックス変更（サブ２）※金額欄のみ
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::ChangeYoushikiComboSub2( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	//	シーケンス番号で絞込み
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
			
			CCtrlSubGeneralVar cs( mfcRec );
			//************************************
			// 「金額０円データを出力しない」
			//************************************
			if(cs.bPrintZeroData)	m_chkZero.SetCheck(TRUE);
			else					m_chkZero.SetCheck(FALSE);
			//************************************
			// 「金額空欄データを出力しない」
			//************************************
			if(cs.bPrintNullData)	m_chkNull.SetCheck(TRUE);
			else					m_chkNull.SetCheck(FALSE);
			//************************************
			// 「金額０円を印字する」（0でOFF　0以外でON）
			//************************************
			if(cs.bPrintHideZero)	m_chkZeroHide.SetCheck(FALSE);
			else					m_chkZeroHide.SetCheck(TRUE);
		}
		//	閉じる
		mfcRec.Fin();
	}
}
// midori 170105 add <--

//**************************************************
//	省略項目初期化
//	【引数】	fShow	…	表示フラグ
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::InitOmissionItem( BOOL fShow )
{
	SetOmissionItem( &m_lblItem1, &m_rdoOn1, &m_rdoOff1, &m_txtOmission1, fShow );
	SetOmissionItem( &m_lblItem2, &m_rdoOn2, &m_rdoOff2, &m_txtOmission2, fShow );
	SetOmissionItem( &m_lblItem3, &m_rdoOn3, &m_rdoOff3, &m_txtOmission3, fShow );
	SetOmissionItem( &m_lblItem4, &m_rdoOn4, &m_rdoOff4, &m_txtOmission4, fShow );
	SetOmissionItem( &m_lblItem5, &m_rdoOn5, &m_rdoOff5, &m_txtOmission5, fShow );
	SetOmissionItem( &m_lblItem6, &m_rdoOn6, &m_rdoOff6, &m_txtOmission6, fShow );
}

//**************************************************
//	省略項目設定
//	【引数】	lblItem			…	省略項目ラベル
//				prdoOn			…	省略するラジオボタン
//				prdoOff			…	省略しないラジオボタン
//				ptxtOmission	…	省略文字列テキストボックス
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetOmissionItem( CICSDisp* lblItem, CButton* prdoOn, 
									  CButton* prdoOff, CICSKNJEdit* ptxtOmission,
									  BOOL fShow )
{
//	lblItem->EnableWindow( fShow );			//	省略項目ラベル
	lblItem->SetCaption( "" );				//	省略項目ラベル
	prdoOn->EnableWindow( fShow );			//	省略するラジオボタン
	prdoOn->SetCheck( FALSE );				//	省略するラジオボタン
	prdoOff->EnableWindow( fShow );			//	省略しないラジオボタン
	prdoOff->SetCheck( FALSE );				//	省略しないラジオボタン
	ptxtOmission->EnableWindow( fShow );	//	省略文字列テキストボックス
	ptxtOmission->SetCaption( "" );			//	省略文字列テキストボックス
}

//**************************************************
//	省略データ取得
//	【引数】	pmfcRec			…	uc_inf_sub_omitテーブルクラス
//				lblItem			…	省略項目ラベル
//				prdoOn			…	省略するラジオボタン
//				prdoOff			…	省略しないラジオボタン
//				ptxtOmission	…	省略文字列テキストボックス
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::GetOmissionData( CdbUcInfSubOmit* pmfcRec, CICSDisp* lblItem, 
									  CButton* prdoOn, CButton* prdoOff, 
									  CICSKNJEdit* ptxtOmission )
{
	int		nMaxLen = 0;
	BOOL	fOnOff;

	lblItem->SetCaption( pmfcRec->m_ItemName );		//	省略項目ラベル文字列取得
//	lblItem->EnableWindow( TRUE );					//	省略項目ラベル表示

// midori 160608 cor -->
//	prdoOn->EnableWindow( TRUE );					//	省略するラジオボタン表示
//	prdoOff->EnableWindow( TRUE );					//	省略しないラジオボタン表示
// ---------------------
	if(m_nOutZero == 1)	{
		prdoOn->EnableWindow( FALSE );				//	省略するラジオボタン表示
		prdoOff->EnableWindow( FALSE );				//	省略しないラジオボタン表示
	}
	else	{
		prdoOn->EnableWindow( TRUE );				//	省略するラジオボタン表示
		prdoOff->EnableWindow( TRUE );				//	省略しないラジオボタン表示
	}
// midori 160608 cor <--
	
	//	省略しない？
	if ( pmfcRec->m_Omit == 0 ){
		prdoOn->SetCheck( FALSE );					//	省略するラジオボタンオフ
		prdoOff->SetCheck( TRUE );					//	省略しないラジオボタンオン
		fOnOff = FALSE;
	}
	//	省略する
	else{
		prdoOn->SetCheck( TRUE );					//	省略するラジオボタンオン
		prdoOff->SetCheck( FALSE );					//	省略しないラジオボタンオフ
		fOnOff = TRUE;
	}
	
	//	省略確認
	CheckOmit( ptxtOmission, fOnOff );
	//	省略文字列取得
	ptxtOmission->SetCaption( pmfcRec->m_OmitStr );
	
	//	最大文字数が0以下？
	if ( pmfcRec->m_OmitMaxLen <= 0 ){
		//	とりあえず１文字は入力可能とする
		nMaxLen = 1;
	}
	//	最大文字数が8を超える？
	else if ( pmfcRec->m_OmitMaxLen > 8 ){
		//	8文字以上の入力は許可しない
		nMaxLen = 8;
	}
	//	範囲内
	else{
		nMaxLen = pmfcRec->m_OmitMaxLen;
	}
	
	//	入力最大文字数設定
	ptxtOmission->SetMaxLength( nMaxLen );
}

//**************************************************
//	様式データ設定
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetYoushikiData( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSubOmit		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	try{

		//	初期化成功？
		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();		//	レコードを先頭に移動

				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					
					//	カウントで分岐
					switch( mfcRec.m_ItemSeq ){
					//	1項目目
					case 1:
						SetOmissionData( &mfcRec, &m_lblItem1, &m_rdoOn1, &m_rdoOff1, &m_txtOmission1 );
						break;
					//	2項目目
					case 2:
						SetOmissionData( &mfcRec, &m_lblItem2, &m_rdoOn2, &m_rdoOff2, &m_txtOmission2 );
						break;
					//	3項目目
					case 3:
						SetOmissionData( &mfcRec, &m_lblItem3, &m_rdoOn3, &m_rdoOff3, &m_txtOmission3 );
						break;
					//	4項目目
					case 4:
						SetOmissionData( &mfcRec, &m_lblItem4, &m_rdoOn4, &m_rdoOff4, &m_txtOmission4 );
						break;
					//	5項目目
					case 5:
						SetOmissionData( &mfcRec, &m_lblItem5, &m_rdoOn5, &m_rdoOff5, &m_txtOmission5 );
						break;
					//	6項目目
					case 6:
						SetOmissionData( &mfcRec, &m_lblItem6, &m_rdoOn6, &m_rdoOff6, &m_txtOmission6 );
						break;
					default:
						break;
					}
					mfcRec.MoveNext();	//	次のレコードへ移動
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){
		
		//	レコード開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	
	//	様式データ設定（サブ）
	SetYoushikiDataSub( pDB, nFormSeq );
}

//**************************************************
//	様式データ設定（サブ）
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetYoushikiDataSub( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	//	シーケンス番号で絞込み
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
		
			//	科目名タイトルが使用可？
			if ( mfcRec.m_OpTitleKnFg != 0 ){
				//	編集開始
				mfcRec.Edit();
				//	フラグ取得
				mfcRec.m_OpTitleKn = m_chkKamoku.GetCheck();
				//	更新
				mfcRec.Update(); 
			}
			// 金融機関を一段表示する使用可？
			if( mfcRec.m_GeneralConstVal[15] & BIT_D0 ){
				//	編集開始
				mfcRec.Edit();
				// 汎用変数コントロールクラス
				CCtrlSubGeneralVar cs( mfcRec );
				//	フラグ取得
				if( m_chkKinyuLine.GetCheck() != FALSE )	cs.bKinyuLine = TRUE;
				else										cs.bKinyuLine = FALSE;

				cs.Set( mfcRec );
				//	更新
				mfcRec.Update();
			}
			// 汎用変数コントロールクラス
			CCtrlSubGeneralVar cs( mfcRec );
// midori 190505 del -->
//			// 科目行を出力する使用可？
//			if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){
//				int st = m_chkKamokuRow.GetCheck();
//				int old_st = 0;
//				//	フラグ取得
//				if(cs.bOutKamokuRow)	old_st = TRUE;
//				else					old_st = FALSE;
//				if(st != old_st){	// 変更された？
//					//	編集開始
//					mfcRec.Edit();
//					if(st)	cs.bOutKamokuRow = TRUE;
//					else	cs.bOutKamokuRow = FALSE;
//					cs.Set( mfcRec );
//					//	更新
//					mfcRec.Update();
//
//					m_isChangePageNum = TRUE;
//// midori 151404 add -->
//					m_isYousiki[mfcRec.m_FormOrder-1] = TRUE;
//// midori 151404 add <--
//				}
//			}
// midori 190505 del <--
// midori 156188 add -->
			// 科目行を出力する使用可？
			if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){
// midori 157042 del -->
				//if(KamokuRowEnableSgn(m_pDB,0) == 0) {
// midori 157042 del <--
				int st = m_chkKamokuRow.GetCheck();
				int old_st = 0;
				//	フラグ取得
				if(cs.bOutKamokuRow)	old_st = TRUE;
				else					old_st = FALSE;
				if(st != old_st){	// 変更された？
					//	編集開始
					mfcRec.Edit();
					if(st)	cs.bOutKamokuRow = TRUE;
					else	cs.bOutKamokuRow = FALSE;
					cs.Set( mfcRec );
					//	更新
					mfcRec.Update();

					m_isChangePageNum = TRUE;
					m_isYousiki[mfcRec.m_FormOrder-1] = TRUE;
				}
// midori 157042 del -->
				//}
// midori 157042 del <--
			}
// midori 156188 add <--

			// m_chkZero,m_chkNullの２つのチェックボックスが変更された場合、ShowKeiZeroを更新
			int changed = 0;
			// 金額０円データを出力しない
			int st = m_chkZero.GetCheck();
			int old_st = 0;
			if(cs.bPrintZeroData)	old_st = TRUE;
			else					old_st = FALSE;
			if(st != old_st){	// 変更された？
				mfcRec.Edit();
				if(st)	cs.bPrintZeroData = TRUE;
				else	cs.bPrintZeroData = FALSE;
				cs.Set(mfcRec);
				mfcRec.Update();

				changed++;
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[mfcRec.m_FormOrder-1] = TRUE;
// midori 151404 add <--
			}
			// 金額空欄データを出力しない
			st = m_chkNull.GetCheck();
			if(cs.bPrintNullData)	old_st = TRUE;
			else					old_st = FALSE;
			if(st != old_st){	// 変更された？
				mfcRec.Edit();
				if(st)	cs.bPrintNullData = TRUE;
				else	cs.bPrintNullData = FALSE;
				cs.Set(mfcRec);
				mfcRec.Update();

				changed++;
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[mfcRec.m_FormOrder-1] = TRUE;
// midori 151404 add <--
			}
			// 金額０円を印字する(ONでFALSE OFFでTRUE)
			st = m_chkZeroHide.GetCheck();
			BOOL old_state = BST_CHECKED;
			if(cs.bPrintHideZero)	old_state = BST_UNCHECKED;
			if(st != old_state){
				mfcRec.Edit();
				if(st)	cs.bPrintHideZero = FALSE;
				else	cs.bPrintHideZero = TRUE;
				cs.Set(mfcRec);
				mfcRec.Update();
			}

			if(changed)	UpdateShowKeiZero(nFormSeq);
		}
		//	閉じる
		mfcRec.Fin();
	}
}

// midori 170105 add -->
//**************************************************
//	様式データ設定（サブ２）※金額欄のみ設定
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetYoushikiDataSub2( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	//	シーケンス番号で絞込み
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
		
			// 汎用変数コントロールクラス
			CCtrlSubGeneralVar cs( mfcRec );

			// m_chkZero,m_chkNullの２つのチェックボックスが変更された場合、ShowKeiZeroを更新
			int changed = 0;
			// 金額０円データを出力しない
			int st = m_chkZero.GetCheck();
			int old_st = 0;
			if(cs.bPrintZeroData)	old_st = TRUE;
			else					old_st = FALSE;
			if(st != old_st){	// 変更された？
				mfcRec.Edit();
				if(st)	cs.bPrintZeroData = TRUE;
				else	cs.bPrintZeroData = FALSE;
				cs.Set(mfcRec);
				mfcRec.Update();

				changed++;
				m_isChangePageNum = TRUE;
				m_isYousiki[mfcRec.m_FormOrder-1] = TRUE;
			}
			// 金額空欄データを出力しない
			st = m_chkNull.GetCheck();
			if(cs.bPrintNullData)	old_st = TRUE;
			else					old_st = FALSE;
			if(st != old_st){	// 変更された？
				mfcRec.Edit();
				if(st)	cs.bPrintNullData = TRUE;
				else	cs.bPrintNullData = FALSE;
				cs.Set(mfcRec);
				mfcRec.Update();

				changed++;
				m_isChangePageNum = TRUE;
				m_isYousiki[mfcRec.m_FormOrder-1] = TRUE;
			}

			// 金額０円を印字する(ONでFALSE OFFでTRUE)
			st = m_chkZeroHide.GetCheck();
			BOOL old_state = BST_CHECKED;
			if(cs.bPrintHideZero)	old_state = BST_UNCHECKED;
			if(st != old_state){
				mfcRec.Edit();
				if(st)	cs.bPrintHideZero = FALSE;
				else	cs.bPrintHideZero = TRUE;
				cs.Set(mfcRec);
				mfcRec.Update();
			}

			if(changed)	UpdateShowKeiZero(nFormSeq);
		}
		//	閉じる
		mfcRec.Fin();
	}
}
// midori 170105 add <--

//**************************************************
//	省略データ設定
//	【引数】	pmfcRec			…	uc_inf_sub_omitテーブルクラス
//				lblItem			…	省略項目ラベル
//				prdoOn			…	省略するラジオボタン
//				prdoOff			…	省略しないラジオボタン
//				ptxtOmission	…	省略文字列テキストボックス
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetOmissionData( CdbUcInfSubOmit* pmfcRec, CICSDisp* lblItem, 
									  CButton* prdoOn, CButton* prdoOff, 
									  CICSKNJEdit* ptxtOmission )
{
	try{
		//	レコード編集開始
		pmfcRec->Edit();							
	
		//	省略するラジオボタンがチェックされてる？
		if ( prdoOn->GetCheck() == TRUE ){
			//	値をオンに設定
			pmfcRec->m_Omit = 1;
		}
		//	省略しないラジオボタンがチェックされてる
		else{
			//	値をオフに設定
			pmfcRec->m_Omit = 0;
		}
		//	省略文字列を設定
		pmfcRec->m_OmitStr = ptxtOmission->GetCaption();
		//	レコードを更新
		pmfcRec->Update();
	}
	catch(...){
	}
}

//**************************************************
//	様式データ確認
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	前回と同じ
//				0以外		…	前回から変更あり
//**************************************************
int CdlgPrintSetup::CheckYoushikiData( CDatabase* pDB, short nFormSeq )
{
	int					nRet = 0;		//	戻値
	CdbUcInfSubOmit		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	try{

		//	初期化成功？
		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();		//	レコードを先頭に移動

				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					
					//	カウントで分岐
					switch( mfcRec.m_ItemSeq ){
					//	1項目目
					case 1:
						//	省略データ変更あり？
						if ( CheckOmissionData( &mfcRec, &m_lblItem1, &m_rdoOn1, &m_rdoOff1, &m_txtOmission1 ) != 0 ){
							nRet++;
						}
						break;
					//	2項目目
					case 2:
						//	省略データ変更あり？
						if ( CheckOmissionData( &mfcRec, &m_lblItem2, &m_rdoOn2, &m_rdoOff2, &m_txtOmission2 ) != 0 ){
							nRet++;
						}
						break;
					//	3項目目
					case 3:
						//	省略データ変更あり？
						if ( CheckOmissionData( &mfcRec, &m_lblItem3, &m_rdoOn3, &m_rdoOff3, &m_txtOmission3 ) != 0 ){
							nRet++;
						}
						break;
					//	4項目目
					case 4:
						//	省略データ変更あり？
						if ( CheckOmissionData( &mfcRec, &m_lblItem4, &m_rdoOn4, &m_rdoOff4, &m_txtOmission4 ) != 0 ){
							nRet++;
						}
						break;
					//	5項目目
					case 5:
						//	省略データ変更あり？
						if ( CheckOmissionData( &mfcRec, &m_lblItem5, &m_rdoOn5, &m_rdoOff5, &m_txtOmission5 ) != 0 ){
							nRet++;
						}
						break;
					//	6項目目
					case 6:
						//	省略データ変更あり？
						if ( CheckOmissionData( &mfcRec, &m_lblItem6, &m_rdoOn6, &m_rdoOff6, &m_txtOmission6 ) != 0 ){
							nRet++;
						}
						break;
					default:
						break;
					}
					mfcRec.MoveNext();	//	次のレコードへ移動
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){
		
		//	レコード開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	
	//	様式データ確認（サブ）
	if ( CheckYoushikiDataSub( pDB, nFormSeq ) != 0 ){
		nRet++;
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	様式データ確認（サブ）
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	前回と同じ
//				0以外		…	前回から変更あり
//**************************************************
int CdlgPrintSetup::CheckYoushikiDataSub( CDatabase* pDB, short nFormSeq )
{
	int				nRet = 0;
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	
	//	「科目名入りタイトルで印刷する」チェック
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
			
			//	科目名タイトルが使用可？
			if ( mfcRec.m_OpTitleKnFg != 0 ){
				
				//	変更されてる？
				if ( mfcRec.m_OpTitleKn != m_chkKamoku.GetCheck() ){
					//	戻値をインクリメント
					nRet++;	
				}
			}
		}
		//	閉じる
		mfcRec.Fin();
	}

	// 「金融機関を一段で表示する」チェック
	if( mfcRec.m_GeneralConstVal[15] & BIT_D0 ){
		int bKinyuLine = BST_UNCHECKED;
		// ビット判定なので、ビットが立っていたらチェック状態とみなす
		CCtrlSubGeneralVar cs( mfcRec );
		if( cs.bKinyuLine ){
			bKinyuLine = BST_CHECKED;
		}
		//	変更されてる？
		if( bKinyuLine != m_chkKinyuLine.GetCheck() ){
			nRet++;
		}
	}

// midori 157042 del -->
//// midori 190505 del -->
//	//// 「科目行を出力する」チェック
//	//if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){
//	//	int bKamokuRow = BST_UNCHECKED;
//	//	// ビット判定なので、ビットが立っていたらチェック状態とみなす
//	//	CCtrlSubGeneralVar cs( mfcRec );
//	//	if( cs.bOutKamokuRow ){
//	//		bKamokuRow = BST_CHECKED;
//	//	}
//	//	//	変更されてる？
//	//	if( bKamokuRow != m_chkKamokuRow.GetCheck() ){
//	//		nRet++;
//	//	}
//	//}
//// midori 190505 del <--
//// midori 156188 add -->
//	// 「科目行を出力する」チェック
//	if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){
//		if(KamokuRowEnableSgn(m_pDB,0) == 0) {
//			int bKamokuRow = BST_UNCHECKED;
//			// ビット判定なので、ビットが立っていたらチェック状態とみなす
//			CCtrlSubGeneralVar cs( mfcRec );
//			if( cs.bOutKamokuRow ){
//				bKamokuRow = BST_CHECKED;
//			}
//			//	変更されてる？
//			if( bKamokuRow != m_chkKamokuRow.GetCheck() ){
//				nRet++;
//			}
//		}
//	}
//// midori 156188 add <--
// midori 157042 del <--
// midori 157042 add -->
	// 「科目行を出力する」チェック
	if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){
// midori 157099,157119 add -->
		//if(KamokuRowEnableSgn(m_pDB, 0) == 0)	{			// 改良No.21-0086,21-0529 del
		if(KamokuRowEnableSgn(m_pDB, 0, nFormSeq) == 0)	{	// 改良No.21-0086,21-0529 add
// midori 157099,157119 add <--
			int bKamokuRow = BST_UNCHECKED;
			// ビット判定なので、ビットが立っていたらチェック状態とみなす
			CCtrlSubGeneralVar cs( mfcRec );
			if( cs.bOutKamokuRow ){
				bKamokuRow = BST_CHECKED;
			}
			//	変更されてる？
			if( bKamokuRow != m_chkKamokuRow.GetCheck() ){
				nRet++;
			}
// midori 157099,157119 add -->
		}
// midori 157099,157119 add <--
	}
// midori 157042 add <--

	// 「金額０円データを表示しない」チェック
	CCtrlSubGeneralVar cs( mfcRec );
	BOOL isCheck = BST_UNCHECKED;
	if(cs.bPrintZeroData)	isCheck = BST_CHECKED;
	if(isCheck != m_chkZero.GetCheck()){
		nRet++;
// midori 151404 del -->
//		m_isChangePageNum = TRUE;	// 「金額０円を表示しない」と「金額空欄を表示しない」はページ数が異なる為、別フラグも持つ
// midori 151404 del <--
	}

	// 「金額空欄データを表示しない」チェック
	isCheck = BST_UNCHECKED;
	if(cs.bPrintNullData)	isCheck = BST_CHECKED;
	if(isCheck != m_chkNull.GetCheck()){
		nRet++;
// midori 151404 del -->
//		m_isChangePageNum = TRUE;	// 「金額０円を表示しない」と「金額空欄を表示しない」はページ数が異なる為、別フラグも持つ
// midori 151404 del <--
	}

	// 「金額０円を印字する」チェック
	isCheck = BST_CHECKED;
	if(cs.bPrintHideZero)	isCheck = BST_UNCHECKED;
	if(isCheck != m_chkZeroHide.GetCheck())	nRet++;

	//	戻値を返す
	return( nRet );
}

// midori 170105 add -->
//**************************************************
//	様式データ確認（サブ２）※金額欄のみのチェック
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	前回と同じ
//				0以外		…	前回から変更あり
//**************************************************
int CdlgPrintSetup::CheckYoushikiDataSub2( CDatabase* pDB, short nFormSeq )
{
	int				nRet = 0;
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス

	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
		}
		//	閉じる
		mfcRec.Fin();
	}

	// 「金額０円データを表示しない」チェック
	CCtrlSubGeneralVar cs( mfcRec );
	BOOL isCheck = BST_UNCHECKED;
	if(cs.bPrintZeroData)	isCheck = BST_CHECKED;
	if(isCheck != m_chkZero.GetCheck()){
		nRet++;
	}

	// 「金額空欄データを表示しない」チェック
	isCheck = BST_UNCHECKED;
	if(cs.bPrintNullData)	isCheck = BST_CHECKED;
	if(isCheck != m_chkNull.GetCheck()){
		nRet++;
	}

	// 「金額０円を印字する」チェック
	isCheck = BST_CHECKED;
	if(cs.bPrintHideZero)	isCheck = BST_UNCHECKED;
	if(isCheck != m_chkZeroHide.GetCheck())	nRet++;

	//	戻値を返す
	return( nRet );
}
// midori 170105 add <--

//**************************************************
//	省略データ確認
//	【引数】	pmfcRec			…	uc_inf_sub_omitテーブルクラス
//				lblItem			…	省略項目ラベル
//				prdoOn			…	省略するラジオボタン
//				prdoOff			…	省略しないラジオボタン
//				ptxtOmission	…	省略文字列テキストボックス
//	【戻値】	0				…	前回から変更なし
//				0以外			…	前回から変更あり
//**************************************************
int CdlgPrintSetup::CheckOmissionData( CdbUcInfSubOmit* pmfcRec, CICSDisp* lblItem, 
									   CButton* prdoOn, CButton* prdoOff, 
									   CICSKNJEdit* ptxtOmission )
{
	int			nRet = 0;	//	戻値
	BYTE		nOmit;		//	省略の有無
	
	//	省略するにチェックあり？
	if ( prdoOn->GetCheck() == TRUE ){
		//	省略する
		nOmit = 1;
	}
	else{
		//	省略しない
		nOmit = 0;
	}
	
	try{
		//	省略フラグが違う？
		if ( pmfcRec->m_Omit != nOmit ){
			//	戻値インクリメント	
			nRet++;
		}
		
		//	省略文字列が違う？
		if ( pmfcRec->m_OmitStr.Compare( ptxtOmission->GetCaption() ) != 0 ){
			//	戻値インクリメント
			nRet++;
		}
	}
	catch(...){
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	出力指定取得
//	【引数】	pDB		…	データベースハンドル
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::GetOutputSetup( CDatabase* pDB )
{
	CdbUcInfMain	mfcRec( pDB );		//	uc_inf_mainテーブルクラス

	try{
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();

				// 会社名を出力しない？
				if ( mfcRec.m_OutCoName == 0 ){
					m_chkCoName.SetCheck( FALSE );
				}
				// 会社名を出力する
				else{
					m_chkCoName.SetCheck( TRUE );
				}
				
				// 決算期間を出力しない？
				if ( mfcRec.m_OutKikan == 0 ){
					m_chkSettlement.SetCheck( FALSE );
				}
				// 決算期間を出力する
				else{
					m_chkSettlement.SetCheck( TRUE );
				}
				
				//	金額欄を出力しない？
				//if ( mfcRec.m_OutKingaku == 1 ){
				if ( (mfcRec.m_OutKingaku & BIT_D0) == BIT_D0 ){
					m_chkMoney.SetCheck( FALSE );
				}
				//	金額欄を出力する
				else{
					m_chkMoney.SetCheck( TRUE );
				}
				
				//	頁を出力しない？
				if ( mfcRec.m_OutPage == 0 ){
					m_chkPage.SetCheck( FALSE );
				}
				//	頁を出力する
				else{
					m_chkPage.SetCheck( TRUE );
				}

// midori 200101 add -->
				// 日付に元号を出力する
				if(bG_Kanso == TRUE)	{
// midori 155871 del -->
					////if( (mfcRec.m_GeneralVar[3] & BIT_D3) == BIT_D3 )	{
					//if(mfcRec.m_GeneralVar[3] & BIT_D3)	{
// midori 155871 del <--
// midori 155871 add -->
					if((mfcRec.m_GeneralVar[3] & BIT_D3) == 0)	{
// midori 155871 add <--
						m_chkGengo.SetCheck( TRUE );
					}
					else {
						m_chkGengo.SetCheck( FALSE );
					}
				}
// midori 200101 add <--

				// 金額０を印字するチェックボックスを、金額欄を出力するチェックボックスの状態で変化させる
				SetChkZeroHideState();

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
}

//**************************************************
//	出力指定設定
//	【引数】	pDB		…	データベースハンドル
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetOutputSetup( CDatabase* pDB )
{
	CdbUcInfMain	mfcRec( pDB );		//	uc_inf_mainテーブルクラス

	try{
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){

			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();		//	レコードを先頭に移動
				mfcRec.Edit();			//	レコード編集開始

				//	会社名を出力するにチェックあり？
				if ( m_chkCoName.GetCheck() == TRUE )		mfcRec.m_OutCoName = 1;
				else										mfcRec.m_OutCoName = 0;
				
				//	決算期間を出力するにチェックあり？
				if ( m_chkSettlement.GetCheck() == TRUE )	mfcRec.m_OutKikan = 1;
				else										mfcRec.m_OutKikan = 0;
				
				//	金額欄を出力するにチェックあり？
				if ( m_chkMoney.GetCheck() == FALSE )	mfcRec.m_OutKingaku |= BIT_D0;
				else					 				mfcRec.m_OutKingaku &= ~BIT_D0;
								
				//	頁出力にチェックあり？
				if ( m_chkPage.GetCheck() == TRUE )		mfcRec.m_OutPage = 1;
				else									mfcRec.m_OutPage = 0;

// midori 200101 add -->
				// 日付に元号を出力する
				if(bG_Kanso == TRUE)	{
// midori 155871 del -->
					//if(m_chkGengo.GetCheck() == TRUE)		mfcRec.m_GeneralVar[3] |= BIT_D3;
// midori 155871 del <--
// midori 155871 add -->
					if(m_chkGengo.GetCheck() == FALSE)		mfcRec.m_GeneralVar[3] |= BIT_D3;
// midori 155871 add <--
					else									mfcRec.m_GeneralVar[3] &= ~BIT_D3;
				}
// midori 200101 add <--

				mfcRec.Update();	//	更新
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
}

//**************************************************
//	シーケンス番号取得
//	【引数】	nFormOrder		…	順序番号
//	【戻値】	0以上			…	シーケンス番号
//				0				…	失敗
//**************************************************
int CdlgPrintSetup::GetFormSeq( int nFormOrder )
{
	int				nFormSeq = 0;		//	シーケンス番号
	CdbUcInfSub		mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
	
	//	順序番号から検索
	if ( mfcRec.RequeryFormOrder( nFormOrder ) == DB_ERR_OK ){
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
			//	シーケンス番号取得
			nFormSeq = mfcRec.m_FormSeq; 
		}
		//	閉じる
		mfcRec.Fin();
	}
	//	シーケンス番号を返す
	return( nFormSeq );
}

//**************************************************
//	順序番号取得
//	【引数】	nFormSeq		…	シーケンス番号
//	【戻値】	0以上			…	順序番号
//				0				…	失敗
//**************************************************
int CdlgPrintSetup::GetFormOrder( int nFormSeq )
{
	int				nFormOrder = 0;		//	シーケンス番号
	CdbUcInfSub		mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
	
	//	順序番号から検索
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();
			//	シーケンス番号取得
			nFormOrder = mfcRec.m_FormOrder; 
		}
		//	閉じる
		mfcRec.Fin();
	}
	//	シーケンス番号を返す
	return( nFormOrder );
}

//**************************************************
//	省略確認
//				ptxtOmit	…	省略文字入力テキスト
//				fOnOff		…	する／しないフラグ（TRUE：する／FALSE：しない）
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::CheckOmit( CICSKNJEdit* ptxtOmit, BOOL fOnOff )
{
// midori 160608 cor -->
	////	する？
	//if ( fOnOff == TRUE ){
	//	//	省略文字入力テキスト有効
	//	ptxtOmit->EnableWindow( TRUE ); 
	//}
	////	しない
	//else{
	//	//	省略文字入力テキスト有効
	//	ptxtOmit->EnableWindow( FALSE ); 
	//}
// ---------------------
	if(m_nOutZero == 1)	{
		ptxtOmit->EnableWindow( FALSE ); 
	}
	else	{
		//	する？
		if ( fOnOff == TRUE ){
			//	省略文字入力テキスト有効
			ptxtOmit->EnableWindow( TRUE ); 
		}
		//	しない
		else{
			//	省略文字入力テキスト有効
			ptxtOmit->EnableWindow( FALSE ); 
		}
	}
// midori 160608 cor <--
}

// midori 151404 cor -->
////// midori 160608 cor -->
//////**************************************************
//////	ダイアログ表示
//////	【引数】	pDB				…	データベースハンドル
//////				nFormSeq		…	様式シーケンス番号
//////	【戻値】	ID_DLG_OK		…	OKボタン押された
//////				ID_DLG_CANCEL	…	キャンセルボタン押された
//////**************************************************
////int CdlgPrintSetup::ShowDialog(BOOL& isChange, CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/ )
////// ---------------------
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースハンドル
////				nFormSeq		…	様式シーケンス番号
//
////	【戻値】	ID_DLG_OK		…	OKボタン押された
////				ID_DLG_CANCEL	…	キャンセルボタン押された
////**************************************************
//int CdlgPrintSetup::ShowDialog(BOOL& isChange, CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/, int nOutZero /*=-1*/ )
//// midori 160608 cor <--
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	isChange		…	変更サイン TRUE:再表示が必要
//				isYousiki[]		…	様式毎の変更サイン TRUE:再表示が必要
//				pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	OKボタン押された
//				ID_DLG_CANCEL	…	キャンセルボタン押された
//**************************************************
int CdlgPrintSetup::ShowDialog(BOOL& isChange, BOOL isYousiki[], CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/, int nOutZero /*=-1*/ )
// midori 151404 cor <--
{
// midori 151404 add -->
	int ii = 0;
// midori 151404 add <--

	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
	m_isChangePageNum = FALSE;
// midori 160608 add -->
	m_nOutZero = nOutZero;
// midori 160608 add <--
// midori 151404 add -->
	for(ii=0;ii<UC_ID_FORMNO_MAX;ii++)	{
		m_isYousiki[ii] = isYousiki[ii];
	}
// midori 151404 add <--

	//	データベースハンドルあり？
	if ( pDB != NULL ){
		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;			//	データベースハンドル
				m_nFormSeq = nFormSeq;	//	様式シーケンス番号取得
				DoModal();				//	モーダルで表示
				if(m_nRet != ID_DLG_CANCEL){
					isChange = m_isChangePageNum;
// midori 151404 add -->
					for(ii=0;ii<UC_ID_FORMNO_MAX;ii++)	{
						isYousiki[ii] = m_isYousiki[ii];
					}
// midori 151404 add <--
				}
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}

//**************************************************
//	「金額０円データを出力しない」クリックイベント
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::OnBnClickedZeroOutputCheck()
{
	// 「金額０円を印字する」コントロールの制御
	SetChkZeroHideState();
}

//**************************************************
//	「金額欄を空欄で出力する」クリックイベント
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::OnBnClickedMoneyCheck()
{
	// 「金額０円を印字する」コントロールの制御
	SetChkZeroHideState();
}

//**************************************************
//	「金額０円を印字する」コントロールの制御
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::SetChkZeroHideState()
{
// midori 160608 cor -->
	//// 「金額欄を空欄で出力する」がONなら、disableにする
	//if((m_chkMoney.GetCheck() != FALSE)){
	//	m_chkZeroHide.EnableWindow(FALSE);
	//}
	//else{
	//	if(m_chkZero.GetCheck() != FALSE)	m_chkZeroHide.EnableWindow(FALSE);
	//	else								m_chkZeroHide.EnableWindow(TRUE);
	//}
// ---------------------
	if(m_nOutZero == 1)	{
		m_chkZeroHide.EnableWindow(FALSE);
	}
	else	{
		// 「金額欄を空欄で出力する」がONなら、disableにする
		if((m_chkMoney.GetCheck() != FALSE)){
			m_chkZeroHide.EnableWindow(FALSE);
		}
		else{
			if(m_chkZero.GetCheck() != FALSE)	m_chkZeroHide.EnableWindow(FALSE);
			else								m_chkZeroHide.EnableWindow(TRUE);
		}
	}
// midori 160608 cor <--
}

//**************************************************
//	「金額０円データを出力しない」「金額空欄データを出力しない」の変化があった場合、その様式の計行データ更新
//	 本当はShowKeiZeroのみの更新でいいが、切り離せなかったので計行全体を更新する
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintSetup::UpdateShowKeiZero(int nFormSeq)
{
	CfrmUc000Common *pView = NULL;
	pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(nFormSeq);
	if(pView == NULL)	return;

	// 初期設定
	pView->CmnSetInitialUchiwakeInfo(nFormSeq);		// 共通データ
	pView->virSetInitialValue();					// 帳表固有データ
	// 計を求める
	pView->CmnTblCalcKei();

	delete pView;
	pView = NULL;
}

// midori 170105 add -->
//**************************************************
//	金額欄一括変更を押下した時
//**************************************************
void CdlgPrintSetup::OnBnClickedButton1()
{
	int				nCnt=0;			// カウント用
	BOOL			isChange=FALSE;
	BOOL			isYousiki[UC_ID_FORMNO_MAX] = {FALSE};
	CdbUcInfSub		mfcRec(m_pDB);	// uc_inf_subテーブルクラス

	// 金額欄に変更あり？
	if(CheckYoushikiDataSub2(m_pDB,m_nFormSeq) != 0)	{
		// 金額欄データ設定
		SetYoushikiDataSub2(m_pDB,m_nFormSeq);
	}
	CdlgIkkatuHenkou dlgClear(this);
	if(dlgClear.ShowDialog(isChange,isYousiki,m_nFormSeq,m_pDB,m_chkMoney.GetCheck()) == ID_DLG_OK)	{
		if(isChange != FALSE)	{
			// 様式コンボボックス変更（金額欄のみ更新する）
			ChangeYoushikiComboSub2(m_pDB,m_nFormSeq);
			// 金額０を印字するチェックボックスを、金額欄を出力するチェックボックスの状態で変化させる（移動前と移動後の金額０円出力チェックボックスの状態が違う可能性がある為）
			SetChkZeroHideState();
			// 様式数分ループ
			for(nCnt=0; nCnt<UC_ID_FORMNO_MAX; nCnt++)	{
				if(mfcRec.Init(nCnt + 1) == DB_ERR_OK){
					if(!(mfcRec.IsEOF()))	{
						if(isYousiki[mfcRec.m_FormOrder-1] == TRUE)	{
							UpdateShowKeiZero(mfcRec.m_FormSeq);
						}
					}
					//	レコード閉じる
					mfcRec.Fin();
				}
// midori 156915 del -->
//// midori 152712 add -->
//				m_isYousiki[nCnt] = isYousiki[nCnt];
//// midori 152712 add <--
// midori 156915 del <--
// midori 156915 add -->
				if(m_isYousiki[nCnt] == 0)	{
					m_isYousiki[nCnt] = isYousiki[nCnt];
				}
// midori 156915 add <--
			}
// midori 152712 add -->
			m_isChangePageNum = isChange;
// midori 152712 add <--
		}
	}
}
// midori 170105 add <--

// midori 157099,157119 add -->
int CdlgPrintSetup::YoushikiDataChk(CDatabase* pDB, short nFormSeq)
{
	int				ret=0;
	int				st=0,old_st=0;
	BOOL			bSign=FALSE;
	CString			cst=_T("");
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス

	ret = 0;
	bSign = FALSE;
	// シーケンス番号で絞込み
	if(mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK)	{
		// レコードあり？
		if( !mfcRec.IsEOF() )	{
			//	先頭に移動
			mfcRec.MoveFirst();

			// 汎用変数コントロールクラス
			CCtrlSubGeneralVar	cs( mfcRec );
			// 科目行を出力する使用可？
			if(mfcRec.m_GeneralConstVal[15] & BIT_D1)	{
				st = m_chkKamokuRow.GetCheck();
				old_st = 0;
				// フラグ取得
				if(cs.bOutKamokuRow)	old_st = TRUE;
				else					old_st = FALSE;
				if(st != old_st)	{	// 変更された？
					if(st)	bSign = TRUE;
				}
			}
		}
		//	閉じる
		mfcRec.Fin();
	}

	// 確認メッセージを表示
	if(bSign == TRUE)	{
// 修正No.169005 cor -->
		//cst  = _T("「科目行を出力する」のチェックは印刷のみに反映します。\r\n");
		//cst += _T("\r\n");
		//cst += _T("編集－ソートの画面で「科目行を挿入する」にチェックを付け「実行」ボタンを押す\r\n");
		//cst += _T("または、各科目の先頭行で編集－特殊行挿入を選択し科目行を選んで「挿入」ボタンを押すと、\r\n");
		//cst += _T("科目行を画面に挿入することができます。\r\n");
		//cst += _T("\r\n");
		//cst += _T("処理を続行してもよろしいですか？\r\n");
// ---------------------
		if(nFormSeq == ID_FORMNO_081 && KamokuRowEnableSgn(pDB, 0, nFormSeq) == 0)	{
			cst = _T("「科目行を出力する」のチェックは印刷のみに反映します。\r\n");
			cst += _T("\r\n");
			cst += _T("処理を続行してもよろしいですか？\r\n");
		}
		else {
			cst = _T("「科目行を出力する」のチェックは印刷のみに反映します。\r\n");
			cst += _T("\r\n");
			cst += _T("編集－ソートの画面で「科目行を挿入する」にチェックを付け「実行」ボタンを押す\r\n");
			cst += _T("または、各科目の先頭行で編集－特殊行挿入を選択し科目行を選んで「挿入」ボタンを押すと、\r\n");
			cst += _T("科目行を画面に挿入することができます。\r\n");
			cst += _T("\r\n");
			cst += _T("処理を続行してもよろしいですか？\r\n");
		}
// 修正No.169005 cor <--
// 157354 del -->
		//if(ICSMessageBox(cst, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
// 157354 del <--
// 157354 add -->
		if(ICSMessageBox(cst, MB_YESNO | MB_ICONEXCLAMATION, 0, 0, this) == IDNO)	{
// 157354 add <--
			ret = -1;
		}
	}

	return(ret);
}
// midori 157099,157119 add <--
