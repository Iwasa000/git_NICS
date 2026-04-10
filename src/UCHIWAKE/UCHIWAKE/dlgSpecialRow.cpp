//--------------------------------------------------
//	dlgSpecialRow.cpp
//
//	2006.02.18～
//--------------------------------------------------
#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgspecialrow.h"

extern	BOOL	bG_InvNo;										// インボイス登録番号追加対応_23/11/06 add

// 改良No.21-0086,21-0529 cor -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);			// midori 156189,156190,156191 add
// ------------------------------
extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

IMPLEMENT_DYNAMIC(CdlgSpecialRow, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgSpecialRow::CdlgSpecialRow(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgSpecialRow::IDD, pParent)
{
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgSpecialRow::CdlgSpecialRow( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgSpecialRow::~CdlgSpecialRow()
{
}

void CdlgSpecialRow::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_SMALL, m_rdoSmall);
	DDX_Control(pDX, IDC_RADIO_MIDDLE, m_rdoMiddle);
	DDX_Control(pDX, IDC_SMALL_NAME, m_txtSmall);
	DDX_Control(pDX, IDC_MIDDLE_NAME, m_txtMiddle);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_SMALL_COMBO, m_cboSmall);
	DDX_Control(pDX, IDC_MIDDLE_COMBO, m_cboMiddle);
	DDX_Control(pDX, IDC_RADIO_OTHER, m_rdoOther);
	DDX_Control(pDX, IDC_NUMBER_TEXT, m_txtOther);
	DDX_Control(pDX, IDC_NAME_LABEL, m_lblName);
	DDX_Control(pDX, IDC_BRACKET_LABEL, m_lblBracket);
	DDX_Control(pDX, IDC_OTHER_LABEL1, m_lblOther1);
	DDX_Control(pDX, IDC_OTHER_LABEL2, m_lblOther2);
// midori 190505 add -->
	DDX_Control(pDX, IDC_RADIO_TITLE, m_rdoTitle);
// midori 190505 add <--
}


BEGIN_MESSAGE_MAP(CdlgSpecialRow, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_SMALL, &CdlgSpecialRow::OnBnClickedRadioSmall)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, &CdlgSpecialRow::OnBnClickedRadioMiddle)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgSpecialRow::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_OTHER, &CdlgSpecialRow::OnBnClickedRadioOther)
// midori 190505 add -->
	ON_BN_CLICKED(IDC_RADIO_TITLE, &CdlgSpecialRow::OnBnClickedRadioTitle)
// midori 190505 add <--
END_MESSAGE_MAP()


void CdlgSpecialRow::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgSpecialRow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgSpecialRow::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	//	入力確定後のフォーマット指定
	m_txtOther.SetFormat( "sss9" );

	//	コンボボックスに括弧種別設定
	SetBracketKind( &m_cboSmall );		//	小計
	SetBracketKind( &m_cboMiddle );		//	中計

	//	特殊行の名称を取得
	GetSpecialName( m_nFormSeq, m_pDB );

	// リサイズ
	OnInitDialogEX();
	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（小計）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSpecialRow::OnBnClickedRadioSmall()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetRadioButton( IDC_RADIO_SMALL );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（中計）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSpecialRow::OnBnClickedRadioMiddle()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetRadioButton( IDC_RADIO_MIDDLE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（一括金額）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSpecialRow::OnBnClickedRadioOther()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetRadioButton( IDC_RADIO_OTHER );
}

// midori 190505 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（科目行）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSpecialRow::OnBnClickedRadioTitle()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetRadioButton( IDC_RADIO_TITLE );
}
// midori 190505 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OKボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSpecialRow::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	特殊行の名称を登録
	if ( SetSpecialName( m_nFormSeq, m_pDB ) == 0 ){
		m_nRet = ID_DLG_OK;		//	戻値設定
	}

	CDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSpecialRow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgSpecialRow::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

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

//**************************************************
//	ラジオボタン設定
//	【引数】	nID		…	チェックされたラジオボタンID
//	【戻値】	なし
//**************************************************
void CdlgSpecialRow::SetRadioButton( int nCheckID )
{	
	//	IDで分岐
	switch( nCheckID ){
	
	//	小計
	case IDC_RADIO_SMALL:
		m_rdoSmall.SetCheck( TRUE );
		m_rdoMiddle.SetCheck( FALSE );
		m_rdoOther.SetCheck( FALSE );
// midori 190505 add -->
		m_rdoTitle.SetCheck( FALSE );
// midori 190505 add <--
		break;

	//	中計
	case IDC_RADIO_MIDDLE:
		m_rdoSmall.SetCheck( FALSE );
		m_rdoMiddle.SetCheck( TRUE );
		m_rdoOther.SetCheck( FALSE );
// midori 190505 add -->
		m_rdoTitle.SetCheck( FALSE );
// midori 190505 add <--
		break;

	//	一括金額
	case IDC_RADIO_OTHER:
		m_rdoSmall.SetCheck( FALSE );
		m_rdoMiddle.SetCheck( FALSE );
		m_rdoOther.SetCheck( TRUE );
// midori 190505 add -->
		m_rdoTitle.SetCheck( FALSE );
// midori 190505 add <--
		break;

// midori 190505 add -->
	// 科目行
	case IDC_RADIO_TITLE:
		m_rdoSmall.SetCheck( FALSE );
		m_rdoMiddle.SetCheck( FALSE );
		m_rdoOther.SetCheck( FALSE );
		m_rdoTitle.SetCheck( TRUE );
	}
// midori 190505 add <--
	
	//	小計がチェックされてる？
	if ( m_rdoSmall.GetCheck() == TRUE ){
		m_txtSmall.EnableWindow( TRUE );
		m_cboSmall.EnableWindow( TRUE );
	}
	else{
		m_txtSmall.EnableWindow( FALSE );
		m_cboSmall.EnableWindow( FALSE );
	}
	
	//	中計がチェックされてる？
	if ( m_rdoMiddle.GetCheck() == TRUE ){
		m_txtMiddle.EnableWindow( TRUE );
		m_cboMiddle.EnableWindow( TRUE );
	}
	else{
		m_txtMiddle.EnableWindow( FALSE );
		m_cboMiddle.EnableWindow( FALSE );
	}

	//	一括金額がチェックされてる？
	if ( m_rdoOther.GetCheck() == TRUE ){
		m_txtOther.EnableWindow( TRUE );
		m_clsFunc.ICSStaticEnableWindow( &m_lblOther1 , TRUE );
		m_clsFunc.ICSStaticEnableWindow( &m_lblOther2 , TRUE );
	}
	else{
		m_txtOther.EnableWindow( FALSE );
		m_clsFunc.ICSStaticEnableWindow( &m_lblOther1 , FALSE );
		m_clsFunc.ICSStaticEnableWindow( &m_lblOther2 , FALSE );
	}
}

//**************************************************
//	特殊行の名称を取得
//	【引数】	nFormSeq	…	様式シーケンス番号
//				pDB			…	データベースハンドル
//	【戻値】	なし
//**************************************************
void CdlgSpecialRow::GetSpecialName( short nFormSeq, CDatabase* pDB )
{	
	CdbUcInfSub	mfcRec( pDB );	//	「uc_inf_sub」テーブル用クラス
	
	try{
		
		//	レコード初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();									//	レコードを先頭行へ移動
				m_txtSmall.SetCaption( mfcRec.m_OpSykeiName );		//	レコードから小計の名称を取得してテキストに反映
				m_txtMiddle.SetCaption( mfcRec.m_OpCykeiName );		//	レコードから中計の名称を取得してテキストに反映
				m_cboSmall.SetCurSel( mfcRec.m_OpSykeiKakko );		//	括弧（小計）の選択インデックス取得
				m_cboMiddle.SetCurSel( mfcRec.m_OpCykeiKakko );		//	括弧（中計）の選択インデックス取得
				
				//	一括金額しない？
				if ( mfcRec.m_OpIkkatuFg == 0 ){
					m_rdoOther.EnableWindow( FALSE );
					m_txtOther.EnableWindow( FALSE );
					m_clsFunc.ICSStaticEnableWindow( &m_lblOther1 , FALSE );
					m_clsFunc.ICSStaticEnableWindow( &m_lblOther2 , FALSE );
				}

// midori 156188_4 del -->
//// midori 190505 add -->
//				// 呼び出し元の様式が ②受取手形、⑪借入金の内訳書の科目行 の場合のみ「科目行」の選択を有効にする
//				if(mfcRec.m_FormSeq == ID_FORMNO_021 || mfcRec.m_FormSeq == ID_FORMNO_111)	{
//					// 小計、中計、科目行の場合、科目行挿入を無効
//					if( m_RowFgFunc == ID_FGFUNC_SYOKEI || m_RowFgFunc == ID_FGFUNC_SYOKEINULL ||
//						m_RowFgFunc == ID_FGFUNC_CHUKEI || m_RowFgFunc == ID_FGFUNC_CHUKEINULL ||
//						m_RowFgFunc == ID_FGFUNC_KAMOKU) {
//						m_rdoTitle.EnableWindow( FALSE );
//					}
//					else {
//						m_rdoTitle.EnableWindow(TRUE);
//					}
//				}
//				else	{
//					m_rdoTitle.EnableWindow( FALSE );
//				}
//// midori 190505 add <--
// midori 156188_4 del <--
// midori 156188_4 add -->
				//if(KamokuRowEnableSgn(pDB,0) == 0) {						// 改良No.21-0086,21-0529 del
				if(KamokuRowEnableSgn(pDB, 0, mfcRec.m_FormSeq) == 0)	{	// 改良No.21-0086,21-0529 add
					m_rdoTitle.EnableWindow( FALSE );
					m_rdoTitle.ShowWindow(SW_HIDE);
				}
				else {
// 改良No.21-0086,21-0529 cor -->
					//// 呼び出し元の様式が ②受取手形、⑪借入金の内訳書の科目行 の場合のみ「科目行」の選択を有効にする
					//if(mfcRec.m_FormSeq == ID_FORMNO_021 || mfcRec.m_FormSeq == ID_FORMNO_111)	{
// ------------------------------
					// 呼び出し元の様式が ②受取手形、ｲﾝﾎﾞｲｽ登録番号追加版の⑧支払手形、⑪借入金の内訳書の科目行 の場合のみ「科目行」の選択を有効にする
					if(mfcRec.m_FormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && mfcRec.m_FormSeq == ID_FORMNO_081) || mfcRec.m_FormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
						// 小計、中計、科目行の場合、科目行挿入を無効
						if( m_RowFgFunc == ID_FGFUNC_SYOKEI || m_RowFgFunc == ID_FGFUNC_SYOKEINULL ||
							m_RowFgFunc == ID_FGFUNC_CHUKEI || m_RowFgFunc == ID_FGFUNC_CHUKEINULL ||
							m_RowFgFunc == ID_FGFUNC_KAMOKU) {
							m_rdoTitle.EnableWindow( FALSE );
						}
						else {
							m_rdoTitle.EnableWindow(TRUE);
						}
					}
					else	{
						m_rdoTitle.EnableWindow( FALSE );
					}
				}
// midori 156188_4 add <--

			}
			//	レコード終了
			mfcRec.Fin();
		}
	}
	catch(...){
		
		//	レコードが開かれてる？
		if ( mfcRec.IsOpen() == TRUE ){
			//	レコード終了
			mfcRec.Fin();
		}
	}
}

//**************************************************
//	特殊行の名称を登録
//	【引数】	nFormSeq	…	様式シーケンス番号
//				pDB			…	データベースハンドル
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgSpecialRow::SetSpecialName( short nFormSeq, CDatabase* pDB )
{
	int			nRet = 1;		//	戻値
	int			nIndex;			//	インデックス
	CdbUcInfSub	mfcRec( pDB );	//	「uc_inf_sub」テーブル用クラス

	try{
		//	レコード初期化
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){

			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();										//	レコードを先頭行へ移動
				mfcRec.Edit();											//	レコード編集開始
				mfcRec.m_OpSykeiName = m_txtSmall.GetCaption();			//	レコードの小計名称のメンバ変数にテキストの内容を代入
				mfcRec.m_OpCykeiName = m_txtMiddle.GetCaption();		//	レコードの中計名称のメンバ変数にテキストの内容を代入
				mfcRec.m_OpSykeiKakko = m_cboSmall.GetCurSel();			//	括弧（小計）の選択インデックスを代入
				mfcRec.m_OpCykeiKakko = m_cboMiddle.GetCurSel();		//	括弧（中計）の選択インデックスを代入
				mfcRec.Update();										//	レコード更新

				//	小計がチェックされてる？
				if ( m_rdoSmall.GetCheck() == TRUE ){
					m_FgFunc = ID_FGFUNC_SYOKEI;
					m_OpName = mfcRec.m_OpSykeiName; 
					nIndex = m_cboSmall.GetCurSel(); 
				}
				//	中計がチェックされてる？
				else if ( m_rdoMiddle.GetCheck() == TRUE ){
					m_FgFunc = ID_FGFUNC_CHUKEI;
					m_OpName = mfcRec.m_OpCykeiName;
					nIndex = m_cboMiddle.GetCurSel(); 
				}
				//	一括金額がチェックされてる？
// midori 190505 del -->
				//else	{
// midori 190505 del <--
// midori 190505 add -->
				else if ( m_rdoOther.GetCheck() == TRUE)	{
// midori 190505 add <--
					m_FgFunc = ID_FGFUNC_IKKATUMANUAL;
					m_OpName = NAME_IKKATU_1 + GetOtherNumber( m_txtOther.GetCaption() ) + NAME_IKKATU_2;
					nIndex = 0;
				}
// midori 190505 add -->
				// 科目行がチェックされてる？
				else	{
					m_FgFunc = ID_FGFUNC_KAMOKU;
					m_OpName = _T("");
					nIndex = 0;
				}
// midori 190505 add <--

				//	範囲内？
				if (( nIndex < 0 ) || ( nIndex >= ID_BRACKET_MAX )){
					//	強制的になしを選択
					nIndex = 0;
				}

				//	括弧文字列取得
				m_OpLeft = BRACKET_KIND[nIndex].szLeft;
				m_OpRight = BRACKET_KIND[nIndex].szRight;

				nRet = 0;	//	戻値に成功をセット
			}
			//	レコード終了
			mfcRec.Fin();
		}
	}
	catch(...){
		
		//	レコードが開かれてる？
		if ( mfcRec.IsOpen() == TRUE ){
			//	レコード終了
			mfcRec.Fin();
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	その他件数文字列取得
//	【引数】	szNumber	…	入力された件数
//	【戻値】	その他件数文字列
//**************************************************
CString CdlgSpecialRow::GetOtherNumber( CString szNumber )
{
	CString		szRet;		//	戻値
	int			nNumber;	//	件数（数値）

	//	初期化
	szRet.Empty();

	nNumber = atoi( szNumber );		//	件数を数値に変換（スペース除去）
	szRet.Format( "%d", nNumber );	//	件数を文字列に再変換
	szRet = " " + szRet;			//	件数の前に半角スペースを付加
	
	//	戻値を返す
	return( szRet );
}

//**************************************************
//	コンボボックスに括弧種別設定
//	【引数】	pCombo		…	コンボボックスのポインタ
//	【戻値】	なし
//**************************************************
void CdlgSpecialRow::SetBracketKind( CComboBox*	pCombo )
{
	int			nCnt;		//	カウント用
	CString		szBuf;		//	バッファ
	
	//	コンボボックスクリア
	pCombo->ResetContent();
	
	//	括弧の最大数分ループ
	for( nCnt = 0; nCnt < ID_BRACKET_MAX; nCnt++ ){
		//	バッファ初期化
		szBuf.Empty(); 
		
		//	括弧なし？
		if ( nCnt == ID_BRACKET_NOTHING ){
			//	「なし」
			szBuf = NAME_BRACKET_NOTHING;
		}
		else{
			//	括弧文字列取得
			szBuf = BRACKET_KIND[nCnt].szLeft + BRACKET_KIND[nCnt].szRight;
		}
		
		//	コンボボックスに文字列追加
		pCombo->AddString( szBuf ); 
	}
}

//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	挿入ボタンで終了
//				ID_DLG_CANCEL	…	キャンセルボタンで終了
//**************************************************
// midori 190505 del -->
//int CdlgSpecialRow::ShowDialog( CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/ )
// midori 190505 del <--
// midori 190505 add -->
int CdlgSpecialRow::ShowDialog( CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/, int nRowFgFunc /*=ID_FGFUNC_NULL*/)
// midori 190505 add <--
{
	m_nRet = ID_DLG_CANCEL;		//	戻値を初期化
	m_FgFunc = ID_FGFUNC_NULL;	//	特殊行フラグを初期化（空行）
	m_OpName.Empty();			//	名称を初期化
	m_OpLeft.Empty();			//	左括弧
	m_OpRight.Empty();			//	右括弧
// midori 190505 add -->
	m_RowFgFunc = nRowFgFunc;
// midori 190505 add <--

	//	データベースハンドルあり？
	if ( pDB != NULL ){

		try{

			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_nFormSeq = nFormSeq;		//	様式シーケンス番号取得
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}
	
	//	戻値を返す
	return( m_nRet );
}
