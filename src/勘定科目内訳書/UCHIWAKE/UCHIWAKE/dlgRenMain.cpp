// dlgRenMain.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgRenMain.h"

// CdlgRenMain ダイアログ

IMPLEMENT_DYNAMIC(CdlgRenMain, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgRenMain::CdlgRenMain(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgRenMain::IDD, pParent)
{
	m_blnTkcodf = FALSE;
	m_blnTkcodt = FALSE;

}
//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgRenMain::~CdlgRenMain()
{
}

void CdlgRenMain::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_KAMOKU_BUTTON, m_btnKamoku);
	DDX_Control(pDX, IDC_RADIO_KAMOKU, m_btnKz);
	DDX_Control(pDX, IDC_RADIO_EDABAN, m_btnEz);
	DDX_Control(pDX, IDC_RADIO_SIWAKE, m_btnDa);
	DDX_Control(pDX, IDC_RADIO_KAMOKUSYUUKEI, m_btnKs);
	DDX_Control(pDX, IDC_RADIO_ALLDATACLEAR, m_btnAllClear);
	DDX_Control(pDX, IDC_RADIO_VALRENDO, m_btnValRendo);
	DDX_Control(pDX, IDC_STARTTEKI_EDIT, m_txtTkcodf);
	DDX_Control(pDX, IDC_ENDTEKI_EDIT, m_txtTkcodt);
	DDX_Control(pDX, IDC_ZERODATA_CHECK, m_chkZero);
	DDX_Control(pDX, IDC_GASSAN_CHECK, m_chkGassan);
// midori 152153 add -->
	DDX_Control(pDX, IDC_KMKNAME_CHECK, m_chkKmkname);
	DDX_Control(pDX, IDC_F9UPD_CHECK, m_chkF9Upd);
// midori 152153 add <--
	DDX_Control(pDX, IDC_YOUSHIKI_LABEL, m_lblYoushiki );
	DDX_Control(pDX, IDC_STARTTEKI_LABEL, m_lblStartTeki);
	DDX_Control(pDX, IDC_ENDTEKI_LABEL, m_lblEndTeki);
	DDX_Control(pDX, IDC_STARTTEKINAME_LABEL, m_lblStartTekiName);
	DDX_Control(pDX, IDC_ENDTEKINAME_LABEL, m_lblEndTekiName);
	DDX_Control(pDX, IDC_TEKIMESSAGE_LABEL, m_lblTekiMessage);
	DDX_Control(pDX, IDC_DATESELECT_RENDO, m_SelMonth);
	DDX_Control(pDX, IDC_COMBO_MAKEUP_METHOD, m_cmbMakeupMethod);
// midori 161110 add -->
	DDX_Control(pDX, IDC_EDAREN_CHECK, m_chkEdaren);
// midori 161110 add <--
}

BEGIN_MESSAGE_MAP(CdlgRenMain, ICSDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_SIWAKE, &CdlgRenMain::OnBnClickedRadioSiwake)
	ON_BN_CLICKED(IDC_RADIO_KAMOKU, &CdlgRenMain::OnBnClickedRadioKamoku)
	ON_BN_CLICKED(IDC_RADIO_EDABAN, &CdlgRenMain::OnBnClickedRadioEdaban)
	ON_BN_CLICKED(IDC_RADIO_KAMOKUSYUUKEI, &CdlgRenMain::OnBnClickedRadioKamokuSyuukei)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgRenMain::OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_KAMOKU_BUTTON, &CdlgRenMain::OnBnClickedKamokuButton)
	ON_BN_CLICKED(IDC_RADIO_ALLDATACLEAR, &CdlgRenMain::OnBnClickedRadioAlldataclear)
	ON_MESSAGE(ICS_SELDATE_MESSAGE_CHANGE, OnDateSelectMessageChangeDATESELECT_RENDO)
	ON_CBN_SELCHANGE(IDC_COMBO_MAKEUP_METHOD, &CdlgRenMain::OnCbnSelchangeComboMakeupMethod)
// midori 152153 add -->
	ON_BN_CLICKED(IDC_EDAREN_CHECK, &CdlgRenMain::OnBnClickedEdarenCheck)
	ON_BN_CLICKED(IDC_KMKNAME_CHECK, &CdlgRenMain::OnBnClickedKmknameCheck)
// midori 152153 add <--
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgRenMain, ICSDialog)
	ON_EVENT(CdlgRenMain, IDC_STARTTEKI_EDIT, 1, CdlgRenMain::TerminationStarttekiEdit, VTS_I2 VTS_I2 VTS_BSTR)
	ON_EVENT(CdlgRenMain, IDC_ENDTEKI_EDIT, 1, CdlgRenMain::TerminationEndtekiEdit, VTS_I2 VTS_I2 VTS_BSTR)
	ON_EVENT(CdlgRenMain, IDC_STARTTEKI_EDIT, 2, CdlgRenMain::FocusStarttekiEdit, VTS_NONE)
	ON_EVENT(CdlgRenMain, IDC_ENDTEKI_EDIT, 2, CdlgRenMain::FocusEndtekiEdit, VTS_NONE)
END_EVENTSINK_MAP()

void CdlgRenMain::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenMain::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 財務DBクラスを取得
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// 財務連動ダイアログコントロール設定
	GetRenMainData( m_pDB , m_pRenInfo , m_nFormSeq );

	// リサイズ
	OnInitDialogEX();

	//	戻値を返す
	return( TRUE ); 

}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「科目参照」ボタンイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedKamokuButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CdlgRenKamoku dlgKamoku( this );
	// ダイアログ呼び出し
	dlgKamoku.ShowDialog( m_pDB , m_pRenInfo , m_nFormSeq );
	// 新規内訳書科目追加確認フラグ格納
	if ( dlgKamoku.m_UcKmkEntry == 1 ){
		m_UcKmkEntry = dlgKamoku.m_UcKmkEntry;
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	Okボタンイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedOkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	実行前のデータチェック(科目を１つ以上登録している?摘要開始・終了番号は正しい?)
	if ( CheckSetData() == FALSE ){
		return;
	}

	// 「連動方法」の値別に実行確認メッセージを分ける
	CString strMsg = "";
	// 「金額のみ再連動する」の場合
	if ( m_btnValRendo.GetCheck() == TRUE ){
		strMsg = _T("財務データと連動している内訳書データの\n金額欄のみ更新します。");
	}
	// 「全データクリア後に財務連動を行う」の場合
	else{
		strMsg = _T("これまで入力したデータを全てクリアした後、\n新たに内訳書のデータを自動で作成します。");
	}

	//	警告メッセージ
	if ( ICSMessageBox( strMsg, MB_OKCANCEL, 0, 0, this ) == IDCANCEL ){
		//	抜ける
		return;
	}

	//	データ設定成功？
	if ( SetMainData( m_pDB, m_nFormSeq ) == TRUE ){
		//	戻値を設定
		m_nRet = ID_DLG_OK;
	}

	// 月指定情報ライト
	m_SelMonth.WriteInformation();

	CDialog::OnOK();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenMain::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	int intID;		// フォーカスが移っているコントロールID
	CWnd	*pWnd;	// フォーカスが移っているコントロールのハンドル

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
		//	F11キーが押された
		case VK_F11:
			// フォーカスを持つウィンドウを取得
			pWnd = GetFocus();
			// フォーカスを持つコントロールのIDを取得
			intID = pWnd->GetDlgCtrlID();
			// 摘要ダイアログ表示～反映
			UpdateTekiyoData( intID );
			return( 1 );
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「摘要開始番号」エディットTerminationEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::TerminationStarttekiEdit(short nChar, short length, LPCTSTR data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TerminationCommon( nChar , data , &m_lblStartTekiName , &m_blnTkcodf , &m_txtTkcodf );
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「摘要開始番号」エディットFocusEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::FocusStarttekiEdit()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// F11押下メッセージ表示
// 231222_168478
	//if ( m_blnTkcodf == TRUE ){
		// ※コントロールの表示/非表示の切り替え処理をここで行うと
		// リターンキーでのフォーカス移動ができなくなるため文字の切り替え処理に変更
		//GetDlgItem( IDC_TEKIMESSAGE_LABEL )->ShowWindow( TRUE );
		m_lblTekiMessage.UpdateCaption("※[F11]キーで摘要リストから選択可能",0);
	//}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「摘要終了番号」エディットTerminationEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::TerminationEndtekiEdit(short nChar, short length, LPCTSTR data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TerminationCommon( nChar , data , &m_lblEndTekiName , &m_blnTkcodt , &m_txtTkcodt );
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「摘要終了番号」エディットFocusEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::FocusEndtekiEdit()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// F11押下メッセージ非表示
// 231222_168478
	//if ( m_blnTkcodt == TRUE ){
		// ※コントロールの表示/非表示の切り替え処理をここで行うと
		// リターンキーでのフォーカス移動ができなくなるため文字の切り替え処理に変更
		//GetDlgItem( IDC_TEKIMESSAGE_LABEL )->ShowWindow( TRUE );
		m_lblTekiMessage.UpdateCaption("※[F11]キーで摘要リストから選択可能",0);
	//}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「科目残高」ラジオボタン選択時
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedRadioKamoku()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	// ラジオボタンセット
	m_btnKz.SetCheck(TRUE);
	m_btnEz.SetCheck(FALSE);
	m_btnDa.SetCheck(FALSE);
	m_btnKs.SetCheck(FALSE);

	// 「連動方法」チェック
	SetRendoTypeControl();

	SetSiwakeControl( FALSE );

// midori 161110 add -->
	// 枝番未登録･未入力のデータを連動する 有効/無効
	m_chkEdaren.EnableWindow( FALSE );
// midori 161110 add <--
// midori 152153 add -->
	// 科目名称を取引先、金融機関名称に転記する
	m_chkKmkname.EnableWindow(TRUE);
	// 取引先、金融機関の[F9 参照]に登録する
	if( m_nFormSeq < ID_FORMNO_171 && m_RegAutoRef == FALSE && 
		m_chkKmkname.GetCheck() == TRUE)	{
		m_chkF9Upd.EnableWindow(TRUE);
	}
	else	{
		m_chkF9Upd.EnableWindow(FALSE);
	}
// midori 152153 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「枝番残高」ラジオボタン選択時
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedRadioEdaban()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// ラジオボタンセット
	m_btnKz.SetCheck(FALSE);
	m_btnEz.SetCheck(TRUE);
	m_btnDa.SetCheck(FALSE);
	m_btnKs.SetCheck(FALSE);

	// 「連動方法」チェック
	SetRendoTypeControl();

	SetSiwakeControl( FALSE );
// midori 161110 add -->
	// 枝番未登録･未入力のデータを連動する 有効/無効
	m_chkEdaren.EnableWindow( TRUE );
// midori 161110 add <--
// midori 152153 add -->
	// 科目名称を取引先、金融機関名称に転記する
	if(m_chkEdaren.GetCheck() == TRUE)	{
		m_chkKmkname.EnableWindow(TRUE);
	}
	else	{
		m_chkKmkname.EnableWindow(FALSE);
	}
	// 取引先、金融機関の[F9 参照]に登録する
	if( m_nFormSeq < ID_FORMNO_171 && m_RegAutoRef == FALSE && m_chkKmkname.IsWindowEnabled() == TRUE &&
		m_chkKmkname.GetCheck() == TRUE)	{
		m_chkF9Upd.EnableWindow(TRUE);
	}
	else	{
		m_chkF9Upd.EnableWindow(FALSE);
	}
// midori 152153 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「仕訳データ」ラジオボタン選択時
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedRadioSiwake()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// ラジオボタンセット
	m_btnKz.SetCheck(FALSE);
	m_btnEz.SetCheck(FALSE);
	m_btnDa.SetCheck(TRUE);
	m_btnKs.SetCheck(FALSE);

	// 「連動方法」チェック
	SetRendoTypeControl();

	SetSiwakeControl( TRUE );
// midori 161110 add -->
	// 枝番未登録･未入力のデータを連動する 有効/無効
	m_chkEdaren.EnableWindow( FALSE );
// midori 161110 add <--
// midori 152153 add -->
	// 科目名称を取引先、金融機関名称に転記する
	m_chkKmkname.EnableWindow(FALSE);
	// 取引先、金融機関の[F9 参照]に登録する
	m_chkF9Upd.EnableWindow(FALSE);
// midori 152153 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「科目残高集計」ラジオボタン選択時
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedRadioKamokuSyuukei()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// ラジオボタンセット
	m_btnKz.SetCheck(FALSE);
	m_btnEz.SetCheck(FALSE);
	m_btnDa.SetCheck(FALSE);
	m_btnKs.SetCheck(TRUE);

	// 「連動方法」チェック
	SetRendoTypeControl();

	SetSiwakeControl( FALSE );
// midori 161110 add -->
	// 枝番未登録･未入力のデータを連動する 有効/無効
	m_chkEdaren.EnableWindow( FALSE );
// midori 161110 add <--
// midori 152153 add -->
	// 科目名称を取引先、金融機関名称に転記する
	m_chkKmkname.EnableWindow(FALSE);
	// 取引先、金融機関の[F9 参照]に登録する
	m_chkF9Upd.EnableWindow(FALSE);
// midori 152153 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「全データクリア後に財務連動を行う」ラジオボタン選択時
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedRadioAlldataclear()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_btnAllClear.SetCheck(TRUE);
	m_btnValRendo.SetCheck(FALSE);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	「金額のみ再連動する」ラジオボタン選択時
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenMain::OnBnClickedRadioValrendo()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_btnAllClear.SetCheck(FALSE);
	m_btnValRendo.SetCheck(TRUE);
}

//**************************************************
//	財務連動テーブル読込み&コントロール設定
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgRenMain::GetRenMainData( CDatabase* pDB , RENZVOL_INFO* pRenInfo , int nFormSeq )
{
	CdbUcRenMain mfcRec(pDB);	// 財務連動テーブル
	CString		 strTitle;		// 様式タイトル

	try{
	
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();

				// 様式タイトル名表示
				GetYoushikiTitle( m_pDB , nFormSeq , &strTitle );
				m_lblYoushiki.UpdateCaption( strTitle , 0 );
				// 元データ欄を設定
				SetOriginalData( &mfcRec , pRenInfo );
				// 連動方法欄を設定
				SetRendoType( &mfcRec );
				// 詳細設定欄を設定
				SetDetailData( &mfcRec );

			}

			//	レコード終了
			mfcRec.Fin();
		}
	}
	catch(...){
		
		//	レコードセット開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード終了
			mfcRec.Fin();
		}
	}

	// 出力期間コントロールの初期化
	InitOutputTermCtrl();

	// 集計方法ダイアログ

}

//**************************************************
//	様式タイトル取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				pszTitle	…	タイトル格納バッファ
//	【戻値】	なし
//**************************************************
void CdlgRenMain::GetYoushikiTitle( CDatabase* pDB, short nFormSeq, CString* pszTitle )
{
	CdbUcInfSub		mfcRec( pDB );		//	uc_inf_subテーブルクラス
	CString			szBuf;				//	バッファ

	try{
		
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();								//	レコードを先頭に移動
				szBuf.Empty();									//	バッファをクリア
				szBuf = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;	//	様式番号結合
				szBuf += "．";									//	区切り文字列を付加
				szBuf += mfcRec.m_Title;						//	様式タイトル結合
				*pszTitle = szBuf;								//	タイトル格納バッファに渡す
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
//	元データ欄の設定
//	【引数】	mfcRec			…	財務連動DBクラスのハンドル
//	【戻値】	なし
//**************************************************
void CdlgRenMain::SetOriginalData( CdbUcRenMain* mfcRec , RENZVOL_INFO* pRenInfo ){

		BOOL blnEzFlag = FALSE;		// 枝番残高有効/無効フラグ

		//-------- 元データラジオボタンの選択可/不可をセット ----------//
		// 科目残高セット
		m_btnKz.EnableWindow( mfcRec->m_OpRenTypeKz );
		// 枝番残高セット
		// 会社情報内の枝番残高フラグを見る(枝番が有効な帳表のみ)
		if ( mfcRec->m_OpRenTypeEz == TRUE ){
			if ( pRenInfo->EzFlag & ID_ICS_EZFLAG_TRUE ){
				blnEzFlag = TRUE;
			}
		}
		m_btnEz.EnableWindow( blnEzFlag );
		// 仕訳データ&摘要開始・終了番号セット
		m_btnDa.EnableWindow( mfcRec->m_OpRenTypeDa );
		// 科目集計残高セット
		m_btnKs.EnableWindow( mfcRec->m_OpRenTypeKs );

		//------------ 保存されている元データを選択 ----------//
		BOOL blnFlag = FALSE;	// 摘要開始・終了番号の有効/無効
		switch ( mfcRec->m_OpRenType ){

		case ID_RENDATA_KZ:
			m_btnKz.SetCheck( TRUE );
			break;
		case ID_RENDATA_EZ:
			if ( blnEzFlag == TRUE ){
				m_btnEz.SetCheck( TRUE );
			}
			// 枝番残高フラグチェック&枝番残高無効の場合
			// 上から選択可能な元データを選び選択
			else{
				if ( mfcRec->m_OpRenTypeKz == TRUE ){
					m_btnKz.SetCheck( TRUE );
				}
				else if ( mfcRec->m_OpRenTypeDa == TRUE ){
					m_btnDa.SetCheck( TRUE );
				}
				else if ( mfcRec->m_OpRenTypeKs == TRUE ){
					m_btnKs.SetCheck( TRUE );
				}
			}
			break;
		case ID_RENDATA_DA:
			m_btnDa.SetCheck( TRUE );
			blnFlag = TRUE;
			break;
		case ID_RENDATA_KS:
			m_btnKs.SetCheck( TRUE );
			break;
		}

		//------------ 摘要データをセット ----------//
		CString strFilter = "";		// クエリ文
		CString strtknamf = "";		// 開始摘要名
		CString strtknamt = "";		// 終了摘要名
		char	ctkcodf[8];			// 開始摘要コード
		char	ctkcodt[8];			// 終了摘要コード

		m_txtTkcodf.SetFormat( "sssss9" );
		m_txtTkcodt.SetFormat( "sssss9" );

		// 摘要コード表示
		if ( ( mfcRec->m_OpTkcodFrom <= 0 ) && ( mfcRec->m_OpTkcodTo <= 0 ) ){
			ZeroMemory( ctkcodf , sizeof(char[8]) );
			ZeroMemory( ctkcodt , sizeof(char[8]) );
		}
		else{
			_itoa_s( mfcRec->m_OpTkcodFrom , &ctkcodf[0] , sizeof( ctkcodf ) , 10 );
			_itoa_s( mfcRec->m_OpTkcodTo , &ctkcodt[0] , sizeof( ctkcodt ) , 10 );
		}
		m_txtTkcodf.SetData( ctkcodf );
		m_txtTkcodt.SetData( ctkcodt );

		// 摘要名取得
		m_clsFunc.GetTekiyoData( m_pZmSub , mfcRec->m_OpTkcodFrom , &strtknamf );
		m_clsFunc.GetTekiyoData( m_pZmSub , mfcRec->m_OpTkcodTo , &strtknamt );

		// 取得科目名表示
		m_lblStartTekiName.UpdateCaption( strtknamf , 0 );
		m_lblEndTekiName.UpdateCaption( strtknamt , 0 );

		// 選択された元データが仕訳データ以外なら摘要開始・終了番号欄無効
		// 取引先合算チェックボックスも無効
		SetSiwakeControl( m_btnDa.GetCheck() );
}

//**************************************************
//	連動方法欄の設定
//	【引数】	mfcRec			…	財務連動DBクラスのハンドル
//	【戻値】	なし
//**************************************************
void CdlgRenMain::SetRendoType( CdbUcRenMain* mfcRec ){

	// 前回実行した財務データを取得
	m_nOpOldRenType = mfcRec->m_OpOldRenType;

	// コントロール設定
	if ( SetRendoTypeControl() == TRUE){
		if( IsRendoValDflt() == FALSE ){
			// 連動方法の値をセット
			if ( mfcRec->m_OpUpdType == ID_OPUPDTYPE_ALLCLEAR ){
				m_btnAllClear.SetCheck(TRUE);
				m_btnValRendo.SetCheck(FALSE);
			}
			else if( mfcRec->m_OpUpdType == ID_OPUPDTYPE_VALRENDO ){
				m_btnAllClear.SetCheck(FALSE);
				m_btnValRendo.SetCheck(TRUE);
			}
		}
	}
}

//**************************************************
//	詳細設定欄の設定
//	【引数】	mfcRec			…	財務連動DBクラスのハンドル
//	【戻値】	なし
//**************************************************
void CdlgRenMain::SetDetailData( CdbUcRenMain* mfcRec )
{
// midori 161110 add -->
	int		nOpOldRenType=0;
// midori 161110 add <--

	// 残高０円データの設定
	m_chkZero.SetCheck( mfcRec->m_OpOutZeroEn );
	// 2007.03.07_仕様追加(⑭-2人件費は常に無効)
	if ( m_nFormSeq == ID_FORMNO_142 ){
		m_chkZero.EnableWindow(FALSE);
	}
	// 取引先合算の設定
	m_chkGassan.SetCheck( mfcRec->m_OpGassan );
// midori 161110 add -->
	// 枝番未登録･未入力のデータを連動する
	if(m_btnEz.GetCheck() == TRUE)	{
		m_chkEdaren.EnableWindow(TRUE);
	}
	else	{
		m_chkEdaren.EnableWindow(FALSE);
	}
	m_chkEdaren.SetCheck( mfcRec->m_OpEdaren );
// midori 161110 add <--
// midori 152153 add -->
	// 科目名称を取引先、金融機関名称に転記する
	if(m_btnKz.GetCheck() == TRUE || (m_btnEz.GetCheck() == TRUE && m_chkEdaren.GetCheck() == TRUE))	{
		m_chkKmkname.EnableWindow(TRUE);
	}
	else	{
		m_chkKmkname.EnableWindow(FALSE);
	}
	m_chkKmkname.SetCheck( mfcRec->m_OpKmkname );
	// 取引先、金融機関の[F9 参照]に登録する
	if( m_nFormSeq < ID_FORMNO_171 && m_RegAutoRef == FALSE && m_chkKmkname.IsWindowEnabled() == TRUE &&
		m_chkKmkname.GetCheck() == TRUE)	{
		m_chkF9Upd.EnableWindow(TRUE);
	}
	else	{
		m_chkF9Upd.EnableWindow(FALSE);
	}
	m_chkF9Upd.SetCheck( mfcRec->m_OpF9Upd );
// midori 152153 add <--
}

//**************************************************
//	摘要開始・終了番号エディット欄の更新に伴う摘要名更新処理
//	【引数】	nChar			…	押下されたキーNo
//				data			…　対象エディット内の文字列
//				lblData			…	対象スタティックのポインタ
//				blnFlag			…	初期起動時のTerminationイベント実行を防ぐフラグ
//				picsHedt		…　対象エディットのメンバ変数ポインタ
//	【戻値】	なし
//**************************************************
void CdlgRenMain::TerminationCommon( short nChar, LPCTSTR data, ICSStatic* lblData , BOOL* blnFlag , CICSHedit* picsHedt ){

	long	lngCode = 0;		// 摘要コード
	CString	strTekiNam = "";	// 摘要名

// 231222_168478 del -->
	//if( !*blnFlag ){
	//	*blnFlag = true;
	//	return;
	//}
// 231222_168478 del <--
	
	// フォーカスを抜けた?
	if( nChar == 0 ){
		// 摘要コード取得
		lngCode = atol( data ); 
		// 摘要名取得
		m_clsFunc.GetTekiyoData( m_pZmSub , lngCode , &strTekiNam );
		// 摘要コード・摘要名表示
		lblData->UpdateCaption( strTekiNam , 0 );
		// F11押下メッセージ非表示
		//GetDlgItem( IDC_TEKIMESSAGE_LABEL )->ShowWindow( FALSE );
		m_lblTekiMessage.UpdateCaption( "" , 0 );
	}
}
//**************************************************
//	連動摘要一覧ダイアログ表示～取得値の反映
//	【引数】	intID			…	コントロールID
//	【戻値】	なし
//**************************************************
void CdlgRenMain::UpdateTekiyoData( int intID ){

	CdlgRenTekiyo dlgTeki( this );	// 連動摘要一覧ダイアログ
	CICSHedit*	  txtEdit;			// ICSHeditポインタ
	char		  cTkcod[8];		// 摘要コード格納バッファ
	char		  cTkcodRet[8];		// 選択摘要コード文字列
	short		  sLen;				// 閾値格納バッファ
	long		  lngCode = 0;		// 摘要コード
	long		  lngBuf;			// 選択摘要コード
	CString		  strTekiNam;		// 選択摘要名
	ICSStatic*	  lblData;			// ICSStaticクラスのポインタ

	// 摘要開始・終了番号のみ実行
	if ( ( intID == IDC_STARTTEKI_EDIT ) || ( intID == IDC_ENDTEKI_EDIT ) ){
		// 摘要開始・終了番号のメンバ変数格納
		if ( intID == IDC_STARTTEKI_EDIT ){
			txtEdit = &m_txtTkcodf;
			lblData = &m_lblStartTekiName;
		}
		else if ( intID == IDC_ENDTEKI_EDIT ){
			txtEdit = &m_txtTkcodt;
			lblData = &m_lblEndTekiName;
		}

		// 摘要コード取得
		txtEdit->GetData( (LPUNKNOWN)&cTkcod , &sLen );
		lngCode = atol( cTkcod ); 

		// 連動摘要一覧ダイアログ表示
		if ( dlgTeki.ShowDialog( lngCode ) == ID_DLG_OK ){
			// 選択摘要コード取得
			lngBuf = dlgTeki.m_Tkcod;
			_itoa_s( lngBuf , &cTkcodRet[0] , sizeof( cTkcodRet ) , 10 );
			// 選択摘要名取得
			m_clsFunc.GetTekiyoData( m_pZmSub , lngBuf , &strTekiNam );
			// 摘要コード・摘要名表示
			txtEdit->SetData( cTkcodRet );
			lblData->UpdateCaption( strTekiNam , 0 );
		}
	}
}

//**************************************************
//	設定データ保存
//	【引数】	pDB				…	データベースハンドル
//				pRenInfo		…	ICS会社情報構造体
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	実行ボタン押した
//				ID_DLG_CANCEL	…	キャンセルボタン押した
//**************************************************
BOOL CdlgRenMain::SetMainData( CDatabase* pDB , int nFormSeq ){

	BOOL			blnRet = FALSE;		// 戻り値
	CdbUcRenMain	mfcRec( pDB );		// メイン情報テーブルクラス
// midori 150892 add -->
	CdbUcInfMain	mfcRecMain( pDB );	// メイン情報テーブル
// midori 150892 add <--
	CdbUcInfSub		mfcRecSub( pDB );	// サブ情報テーブル
	char			cTkcodf[8];			// 摘要開始番号格納バッファ
	char			cTkcodt[8];			// 摘要終了番号格納バッファ
	short			sLen;				// 閾値用格納バッファ
	
	try{
	
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
					
				//	レコードを先頭に移動
				mfcRec.MoveFirst();
				//	編集開始
				mfcRec.Edit();
				
				// 元データ保存
				//	「科目残高」にチェックあり？
				if ( m_btnKz.GetCheck() == TRUE ){
					mfcRec.m_OpRenType = ID_RENDATA_KZ;
				}
				//	「枝番残高」にチェックあり？
				if ( m_btnEz.GetCheck() == TRUE ){
					mfcRec.m_OpRenType = ID_RENDATA_EZ;
				}
				//	「仕訳データ」にチェックあり？
				if ( m_btnDa.GetCheck() == TRUE ){
					mfcRec.m_OpRenType = ID_RENDATA_DA;
				}
				//	「科目残高集計」にチェックあり？
				if ( m_btnKs.GetCheck() == TRUE ){
					mfcRec.m_OpRenType = ID_RENDATA_KS;
				}

				// 摘要開始・終了番号保存
				m_txtTkcodf.GetData( (LPUNKNOWN)&cTkcodf , &sLen );
				m_txtTkcodt.GetData( (LPUNKNOWN)&cTkcodt , &sLen );
				mfcRec.m_OpTkcodFrom = atol( cTkcodf );
				mfcRec.m_OpTkcodTo = atol( cTkcodt );

//				// 実行する財務データ種を保存
//				mfcRec.m_OpOldRenType = mfcRec.m_OpRenType;

				// 連動方法を保存
				// 「全データクリア後に財務連動を行う」チェック
				if ( m_btnAllClear.GetCheck() == TRUE ){				
					mfcRec.m_OpUpdType = ID_OPUPDTYPE_ALLCLEAR;
				}
				if ( m_btnValRendo.GetCheck() == TRUE ){

					mfcRec.m_OpUpdType = ID_OPUPDTYPE_VALRENDO;

					if( m_nOpOldRenType == -1 ){
						// 仕訳データ＆科目残高集計の場合は「全データクリア～」にする
						if( ( mfcRec.m_OpRenType == ID_RENDATA_DA ) || ( mfcRec.m_OpRenType == ID_RENDATA_KS ) ){
							mfcRec.m_OpUpdType = ID_OPUPDTYPE_ALLCLEAR;
						}
						else{
							mfcRec.m_OpUpdType = ID_OPUPDTYPE_VALRENDO;
						}
					}
				}

				// 残高０円フラグ
				if ( m_chkZero.GetCheck() == TRUE ){
					mfcRec.m_OpOutZeroEn = TRUE;
				}
				else{
					mfcRec.m_OpOutZeroEn = FALSE;
				}
				// 同一取引先合算フラグ
				if ( m_chkGassan.GetCheck() == TRUE ){
					mfcRec.m_OpGassan = TRUE;
				}
				else{
					mfcRec.m_OpGassan = FALSE;
				}
			
// midori 161110 add -->
				// 枝番未登録･未入力のデータを連動する
				if(m_chkEdaren.GetCheck() == TRUE)	{
					mfcRec.m_OpEdaren = TRUE;
				}
				else{
					mfcRec.m_OpEdaren = FALSE;
				}
// midori 161110 add <--
// midori 152153 add -->
				// 科目名称を取引先、金融機関名称に転記する
				if(m_chkKmkname.GetCheck() == TRUE)	{
					mfcRec.m_OpKmkname = TRUE;
				}
				else{
					mfcRec.m_OpKmkname = FALSE;
				}
				// 取引先、金融機関の[F9 参照]に登録する
				if(m_chkF9Upd.GetCheck() == TRUE)	{
					mfcRec.m_OpF9Upd = TRUE;
				}
				else{
					mfcRec.m_OpF9Upd = FALSE;
				}
// midori 152153 add <--

				//	レコード更新
				mfcRec.Update();

				// メンバ変数に格納
				m_FormSeq = mfcRec.m_FormSeq;			// 様式シーケンス番号
				m_OpRenType = mfcRec.m_OpRenType;		// 選択元データ
				m_OpRenTypeKz = mfcRec.m_OpRenTypeKz;	// 科目残高On/Offフラグ
				m_OpRenTypeEz = mfcRec.m_OpRenTypeEz;	// 枝番残高On/Offフラグ
				m_OpRenTypeDa = mfcRec.m_OpRenTypeDa;	// 仕訳データOn/Offフラグ
				m_OpRenTypeKs = mfcRec.m_OpRenTypeKs;	// 科目集計残高On/Offフラグ
				m_OpTkcodFrom = mfcRec.m_OpTkcodFrom;	// 仕訳時の摘要開始番号
				m_OpTkcodTo = mfcRec.m_OpTkcodTo;		// 仕訳時の摘要終了番号
				m_OpOutZeroEn = mfcRec.m_OpOutZeroEn;	// 残高0円時のデータ作成On/Offフラグ
				m_OpGassan = mfcRec.m_OpGassan;			// 同一取引先合算On/Offフラグ
				m_OpUpdType = mfcRec.m_OpUpdType;		// 連動方法選択フラグ
				m_OpOldRenType = mfcRec.m_OpOldRenType;	// 前回選択した財務データ
// midori 161110 add -->
				m_OpEdaren = mfcRec.m_OpEdaren;			// 枝番未登録･未入力のデータを連動するOn/Offフラグ
// midori 161110 add <--
// midori 152153 add -->
				m_OpKmkName = mfcRec.m_OpKmkname;		// 科目名称を取引先、金融機関名称に転記する
				m_OpF9Upd = mfcRec.m_OpF9Upd;			// 取引先、金融機関の[F9 参照]に登録する
// midori 152153 add <--

				//	成功
				blnRet = TRUE;
			}

			//	レコード終了
			mfcRec.Fin();
		}

		// uc_inf_sub書き込み
		mfcRecSub.RequeryFormSeq( nFormSeq );

		// 編集開始
		mfcRecSub.Edit();

		CCtrlSubGeneralVar cs( mfcRecSub );

		// 「金額のみを再連動する」のデフォルト状態
		if ( m_OpOldRenType != -1 ){
			if( !(cs.bDfltRendoKikan) ){
				
				cs.bDfltRendoKikan = TRUE;
			}
		}
// midori 150892 del -->
		//// 集計方法
		//if( m_cmbMakeupMethod.GetCurSel() == ID_COMBO_MAKEUP_KIKAN ){
		//	cs.bMakeUpMethod = TRUE;
		//}
		//else{
		//	cs.bMakeUpMethod = FALSE;
		//}
// midori 150892 del <--
		cs.Set( mfcRecSub );

		// 更新
		mfcRecSub.Update();

		//	レコード終了
		mfcRecSub.Fin();

// midori 150892 add -->
		// 集計方法のみメインに保存する
		if(mfcRecMain.Init() == DB_ERR_OK){
			mfcRecMain.Edit();
			CCtrlMainGeneralVar cm(mfcRecMain);
			if(m_cmbMakeupMethod.GetCurSel() == ID_COMBO_MAKEUP_KIKAN)	{
				cm.bMakeUpMethod = TRUE;
			}
			else{
				cm.bMakeUpMethod = FALSE;
			}
			cm.Set(mfcRecMain);
			mfcRecMain.Update();
		}
		mfcRecMain.Fin();
// midori 150892 add <--

	}
	catch(...){
		
		//	レコードセット開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード終了
			mfcRec.Fin();
		}
		if ( mfcRecSub.IsOpen() ){
			//	レコード終了
			mfcRecSub.Fin();
		}
	}

	//	戻値を返す
	return( blnRet );

}

//**************************************************
//	仕訳データ関連のコントロール有効/無効制御
//	【引数】	blnFlag			…	TRUE：コントロール有効
//								…	FALSE:コントロール無効
//	【戻値】	なし
//**************************************************
void CdlgRenMain::SetSiwakeControl( BOOL blnFlag ){

	m_txtTkcodf.EnableWindow( blnFlag );
	m_txtTkcodt.EnableWindow( blnFlag );
	m_clsFunc.ICSStaticEnableWindow( &m_lblStartTeki , blnFlag );
	m_clsFunc.ICSStaticEnableWindow( &m_lblEndTeki , blnFlag );
	m_clsFunc.ICSStaticEnableWindow( &m_lblStartTekiName , blnFlag );
	m_clsFunc.ICSStaticEnableWindow( &m_lblEndTekiName , blnFlag );
	m_chkGassan.EnableWindow( blnFlag );
}

//**************************************************
//	連動方法欄のコントロール有効/無効制御
//	【引数】	なし
//	【戻値】	なし
//**************************************************
BOOL CdlgRenMain::SetRendoTypeControl()
{
	BOOL blnRet = FALSE;	// 金額再連動有効:TRUE/無効:FALSE
	int	 intCheck = -2;		// チェックされた項目

	// 科目残高ON?
	if ( m_btnKz.GetCheck() == TRUE ){
		intCheck = ID_RENDATA_KZ;
	}
	// 枝番残高ON?
	if ( m_btnEz.GetCheck() == TRUE ){
		intCheck = ID_RENDATA_EZ;
	}
	// 仕訳データON? or 科目残高集計ON? ※「金額のみ再連動する」無効
	if ( ( m_btnDa.GetCheck() == TRUE ) || ( m_btnKs.GetCheck() == TRUE ) ){
		// 無効切替
		m_btnValRendo.EnableWindow(FALSE);
		// 「全データクリア後に財務連動を行う」チェック
		m_btnAllClear.SetCheck(TRUE);
		m_btnValRendo.SetCheck(FALSE);
		return FALSE;
	}

	// 選択中の財務データが元データと一致した?
	if ( intCheck == m_nOpOldRenType ){
		blnRet = TRUE;

		// デフォルト値を使用する？
		if( IsRendoValDflt() != FALSE ){
			m_btnAllClear.SetCheck(FALSE);
			m_btnValRendo.SetCheck(TRUE);
		}
	}
	else{
		blnRet = FALSE;
		// 「全データクリア後に財務連動を行う」チェック
		m_btnAllClear.SetCheck(TRUE);
		m_btnValRendo.SetCheck(FALSE);
	}

	// 有効/無効切替
	m_btnValRendo.EnableWindow(blnRet);

	return blnRet;
}

//**************************************************
//	財務連動実行前のデータチェック
//	【引数】	なし
//	【戻値】	TRUE		…	実行可能
//				FALSE		…	実行不可
//**************************************************
BOOL CdlgRenMain::CheckSetData()
{
	BOOL blnRet = FALSE;	// 戻り値

	// 摘要開始・終了番号値のチェック
	if ( CheckTekiyoNo() == FALSE ){
		ICSMessageBox( _T("摘要開始番号または摘要終了番号の指定が正しくありません。"), MB_OK, 0, 0, this );
		return blnRet;
	}
	// 財務科目の登録状態をチェック
	if ( CheckKamokuEntry() == FALSE ){
		ICSMessageBox( _T("財務科目がリストに１件も登録されていません。"), MB_OK, 0, 0, this );
		m_btnKamoku.SetFocus();
		return blnRet;
	}

	blnRet = TRUE;

	return blnRet;
}

//**************************************************
//	摘要開始・終了番号番号値の不正チェック
//	【引数】	なし
//	【戻値】	TRUE		…	実行可能
//				FALSE		…	実行不可
//**************************************************
BOOL CdlgRenMain::CheckTekiyoNo()
{
	BOOL	blnRet = FALSE;	// 戻り値
	char	cTkcodf[8];		// 摘要開始番号文字列
	char	cTkcodt[8];		// 摘要終了番号文字列
	short	sLen = 0;		// 閾値格納用バッファ
	long	lngTkcodf = 0;	// 摘要開始番号の値
	long	lngTkcodt = 0;	// 摘要終了番号の値
	CString sTkcodf = "";	// 摘要開始番号文字列2
	CString sTkcodt = "";	// 摘要終了番号文字列2

	// 元データが仕訳データの場合のみチェック
	if ( m_btnDa.GetCheck() == TRUE ){
		// 初期化
		ZeroMemory( cTkcodf , sizeof(char[8]));
		ZeroMemory( cTkcodt , sizeof(char[8]));

		// 摘要番号を数値化
		m_txtTkcodf.GetData( (LPUNKNOWN)&cTkcodf , &sLen );
		m_txtTkcodt.GetData( (LPUNKNOWN)&cTkcodt , &sLen );

		lngTkcodf = atol( cTkcodf );
		lngTkcodt = atol( cTkcodt );

		// 空欄チェック
		sTkcodf = cTkcodf;
		sTkcodt = cTkcodt;
		if ( sTkcodf == "" ){
			lngTkcodf = -1;
		}
		if ( sTkcodt == "" ){
			lngTkcodt = -1;
		}

		// 適用開始番号が0未満?
		if ( lngTkcodf < 0 ){
			m_txtTkcodf.SetFocus();
		}
		// 摘要終了番号が1未満? or 摘要開始番号が終了番号より大きい?
		else if ( ( lngTkcodt < 0 ) || ( lngTkcodf > lngTkcodt ) ){
			m_txtTkcodt.SetFocus();
		}
		// 摘要開始・終了番号が共に0?
		else if ( ( lngTkcodf == 0 ) && ( lngTkcodt == 0 ) ){
			m_txtTkcodt.SetFocus();
		}
		else{
			blnRet = TRUE;
		}
	}
	else{
		blnRet = TRUE;
	}

	return blnRet;
}

//**************************************************
//	財務科目登録チェック
//	【引数】	なし
//	【戻値】	TRUE		…	実行可能
//				FALSE		…	実行不可
//**************************************************
BOOL CdlgRenMain::CheckKamokuEntry()
{
	CdbUcRenKamoku mfcRec( m_pDB );
	BOOL	blnRet = FALSE;	// 戻り値

	try{
		//	初期化成功？
		if ( mfcRec.RequeryFormSeqFgCoType( m_nFormSeq , m_pRenInfo->CoType , 0 ) == DB_ERR_OK ){
			//	レコードあり？
			if ( mfcRec.GetRecordCount() > 0 ){
				// 全レコード確認
				while( !mfcRec.IsEOF() ){
					if ( CheckKamokuEntrySub( m_pZmSub , mfcRec.m_KcdStart ) == TRUE ){
						blnRet = TRUE;
						break;
					}
					mfcRec.MoveNext();
				}
			}
		}
		//	レコード終了
		mfcRec.Fin();
	}
	catch(...){		
		//	レコードセット開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード終了
			mfcRec.Fin();
		}
	}

	return blnRet;
}

//**************************************************
//	対象科目が有効科目かどうか判定
//	【引数】	ZmSub				…	DB財務クラスのポインタ
//				strkncod			…	判定する科目コード
//	【戻値】	TRUE				…	科目あり or 科目有効
//				FALSE				…	科目無し or 科目無効
//**************************************************
BOOL CdlgRenMain::CheckKamokuEntrySub( CDBZmSub* ZmSub , CString strkncod )
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
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pRenInfo		…	ICS会社情報構造体
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	実行ボタン押した
//				ID_DLG_CANCEL	…	キャンセルボタン押した
//**************************************************
// midori 152153 del -->
//int CdlgRenMain::ShowDialog( CDatabase* pDB , RENZVOL_INFO* pRenInfo , int nFormSeq )
// midori 152153 del <--
// midori 152153 add -->
int CdlgRenMain::ShowDialog( CDatabase* pDB , RENZVOL_INFO* pRenInfo , int nFormSeq, BOOL nRegAutoRef )
// midori 152153 add <--
{

	// メンバ変数初期化
	m_FormSeq = 0;			// 様式シーケンス番号
	m_OpRenType = 0;		// 選択元データ
	m_OpRenTypeKz = 0;		// 科目残高On/Offフラグ
	m_OpRenTypeEz = 0;		// 枝番残高On/Offフラグ
	m_OpRenTypeDa = 0;		// 仕訳データOn/Offフラグ
	m_OpRenTypeKs = 0;		// 科目集計残高On/Offフラグ
	m_OpTkcodFrom = 0;		// 仕訳時の摘要開始番号
	m_OpTkcodTo = 0;		// 仕訳時の摘要終了番号
	m_OpOutZeroEn = 0;		// 残高0円時のデータ作成On/Offフラグ
	m_OpGassan = 0;			// 同一取引先合算On/Offフラグ
	m_UcKmkEntry = 0;		// 新規内訳書科目追加確認フラグ
	m_OpUpdType = 0;		// 連動方法選択フラグ
	m_OpOldRenType = 0;		// 前回選択した財務データ
// midori 161110 add -->
	m_OpEdaren = 0;			// 枝番未登録･未入力のデータを連動するOn/Offフラグ
// midori 161110 add <--
// midori 152153 add -->
	m_RegAutoRef = 0;		// 取引先、金融機関を自動で登録する
	m_OpKmkName = 0;		// 科目名称を取引先、金融機関名称に転記する
	m_OpF9Upd = 0;			// 取引先、金融機関の[F9 参照]に登録する
// midori 152153 add <--

	m_pDB = NULL;
	m_nFormSeq = ID_FORMNO_011;
	m_nOpOldRenType = -1;
	m_nRet = ID_DLG_CANCEL;

	//	データベースハンドルヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_pRenInfo = pRenInfo;		//  会社情報取得
				m_nFormSeq = nFormSeq;		//	様式番号取得
// midori 152153 add -->
				m_RegAutoRef = nRegAutoRef;	//	取引先、金融機関を自動で登録する
// midori 152153 add <--
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	return( m_nRet );
}

//**************************************************
//	「金額のみを再連動する」にデフォルト値をセットするか
//	【引数】	なし
//	【戻値】	TRUE	:	デフォルト値を使用する
//										  使用しない
//**************************************************
BOOL CdlgRenMain::IsRendoValDflt()
{
	CdbUcInfSub	mfcRecSub( m_pDB );		//	uc_inf_subテーブルクラス
	BOOL nRet = FALSE;

	if ( mfcRecSub.RequeryFormSeq( m_nFormSeq ) == DB_ERR_OK ){			
		if( !( mfcRecSub.IsEOF() ) ){
			CCtrlSubGeneralVar cs( mfcRecSub );
			if( !(cs.bDfltRendoKikan )){
				nRet = TRUE;
			}
		}
	}

	//	レコード終了
	mfcRecSub.Fin();

	return nRet;
}

//**************************************************
//	出力期間の指定に関わるコントロールを初期化する
//	【引数】	なし
//	【戻値】	0	: 初期化成功
//				-1  : 初期化失敗
//**************************************************
int CdlgRenMain::InitOutputTermCtrl()
{
// midori 150892 cor -->
	//CdbUcInfSub mfcRec( m_pDB );

	//if( mfcRec.RequeryFormSeq( m_nFormSeq ) != DB_ERR_OK ){
	//	return -1;
	//}
	//// 集計方法コンボボックスの初期化
	//int cmbItem = 0;
	//CCtrlSubGeneralVar cs( mfcRec );
	//if( cs.bMakeUpMethod ){
	//	cmbItem = ID_COMBO_MAKEUP_KIKAN;
	//}
	//m_cmbMakeupMethod.SetCurSel( cmbItem );
// -------------------------------
	CdbUcInfMain	mfcMain( m_pDB );

	if (mfcMain.Init() != DB_ERR_OK) {
		return -1;
	}
	// 集計方法コンボボックスの初期化
	CCtrlMainGeneralVar cm( mfcMain );
	int cmbItem = 0;
	if( cm.bMakeUpMethod ){
		cmbItem = ID_COMBO_MAKEUP_KIKAN;
	}
	m_cmbMakeupMethod.SetCurSel( cmbItem );
// midori 150892 cor <--

	// 共通情報ではなく、プログラム固有の期間情報を使用
	m_SelMonth.SetMode(ICS_DATESELECT_MODE_ONESELF);

	// 月指定コントロール初期化
	m_SelMonth.Iniz(m_pZmSub, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_M, 0, 0 );

	// 選択開始月が期首でないなら期首にセット
	SetMonthOffsetRuikei();

	return 0;
}

//**************************************************
//	月指定コントロールの値が変更された際に呼ばれる。
//	集計方法が期首からの累計の場合、左クリックでも右クリックと同様の振る舞いをする
//	【引数】	なし
//	【戻値】	0L
//**************************************************
LRESULT CdlgRenMain::OnDateSelectMessageChangeDATESELECT_RENDO(WPARAM wParam, LPARAM lParam)
{
	// 選択開始月が期首でないなら期首にセット
	SetMonthOffsetRuikei();

	return	0L;
}

//********************************************************************************
//	財務連動：月指定コントロールの開始オフセットを期首にセットする
//		IN		なし
//		RET		int				-1 : 失敗
//********************************************************************************
int CdlgRenMain::SetMonthOffsetRuikei()
{
	// 集計方法が「期間集計」なら何もしない
	if( m_cmbMakeupMethod.GetCurSel() == ID_COMBO_MAKEUP_KIKAN )	return	1;

	// 入力されている期間の取得
	int ostart = 0, oend = 0, dstart = 0, dend = 0;
	m_SelMonth.GetMonthOffset( ostart, oend, dstart, dend );	

	// 選択開始月が期首なら何もしない
	if(ostart == 0)		return 1;

	// 開始オフセットを期首にセット
	ostart = 0;

	if( m_pZmSub->zvol == NULL ){
		m_pZmSub->VolumeOpen();

		// 開始年月日を期首にセット
		dstart = m_pZmSub->zvol->ss_ymd;

		m_pZmSub->VolumeClose();
	}

	// 更新
	return m_SelMonth.SetMonthOffset( ostart, oend, dstart, dend );
}

//********************************************************************************
//	財務連動：集計期間の取得
//		IN		RENTEMP_INFO*	財務連動で使用するアプリ情報
//		RET		int				-1 : 失敗
//********************************************************************************
int CdlgRenMain::RendoGetSyukeiTerm(RENTEMP_INFO* uRenInfo)
{
	int dstart, dend;

	// 月指定コントロールから指定期間を取得（オフセットのみ使用）
	return m_SelMonth.GetMonthOffset( uRenInfo->intStartOffset, uRenInfo->intEndOffset, dstart, dend );
}

//**************************************************
//	コンボボックスが変更された際に呼ばれる。
//	期間集計→通常集計に変更されたら開始月オフセットを期首にセット
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenMain::OnCbnSelchangeComboMakeupMethod()
{
	// 選択開始月が期首でないなら期首にセット
	SetMonthOffsetRuikei();
}
void CdlgRenMain::OnCancel()
{
	// 月指定情報ライト
	m_SelMonth.WriteInformation();

	ICSDialog::OnCancel();
}

// midori 152153 add -->
void CdlgRenMain::OnBnClickedEdarenCheck()
{
	if(m_chkEdaren.GetCheck() == TRUE)	{
		m_chkKmkname.EnableWindow(TRUE);
		if( m_nFormSeq < ID_FORMNO_171 && m_RegAutoRef == FALSE && 
			m_chkKmkname.GetCheck() == TRUE)	{
			m_chkF9Upd.EnableWindow(TRUE);
		}
		else	{
			m_chkF9Upd.EnableWindow(FALSE);
		}
	}
	else	{
		m_chkKmkname.EnableWindow(FALSE);
		m_chkF9Upd.EnableWindow(FALSE);
	}
}

void CdlgRenMain::OnBnClickedKmknameCheck()
{
	if( m_nFormSeq < ID_FORMNO_171 && m_RegAutoRef == FALSE && 
		m_chkKmkname.GetCheck() == TRUE)	{
		m_chkF9Upd.EnableWindow(TRUE);
	}
	else	{
		m_chkF9Upd.EnableWindow(FALSE);
	}
}
// midori 152153 add <--
