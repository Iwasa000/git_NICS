//--------------------------------------------------
//	dlgOutputForm.cpp
//
//	2006.02.18～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgOutputForm.h"

IMPLEMENT_DYNAMIC(CdlgOutputForm, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgOutputForm::CdlgOutputForm(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgOutputForm::IDD, pParent)
{
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgOutputForm::CdlgOutputForm( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgOutputForm::~CdlgOutputForm()
{
}

void CdlgOutputForm::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YOUSHIKI_LABEL1, m_YoushikiLabel1);
	DDX_Control(pDX, IDC_YOUSHIKI_LABEL2, m_YoushikiLabel2);
	DDX_Control(pDX, IDC_YOUSHIKI_LABEL3, m_YoushikiLabel3);
	DDX_Control(pDX, IDC_RADIO_NORMAL, m_rdoNormal);
	DDX_Control(pDX, IDC_RADIO_ONE, m_rdoOne);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CdlgOutputForm, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_NORMAL, &CdlgOutputForm::OnBnClickedRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_ONE, &CdlgOutputForm::OnBnClickedRadioOne)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgOutputForm::OnBnClickedOk)
END_MESSAGE_MAP()

void CdlgOutputForm::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgOutputForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgOutputForm::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	//	ラベル設定
	SetLabel( m_pDB, m_nFormSeq );
	//	出力形式取得
	GetFormType( m_pDB, m_nFormSeq );

	// リサイズ
	OnInitDialogEX();

	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	通常ラジオボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgOutputForm::OnBnClickedRadioNormal()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_rdoNormal.SetCheck( TRUE );	//	通常をON
	m_rdoOne.SetCheck( FALSE );		//	個別をOFF
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	個別ラジオボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgOutputForm::OnBnClickedRadioOne()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_rdoNormal.SetCheck( FALSE );	//	通常をOFF
	m_rdoOne.SetCheck( TRUE );		//	個別をON
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OKボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgOutputForm::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	出力形式保存成功？
	if ( SetFormType( m_pDB, m_nFormSeq ) == 0 ){
		m_nRet = ID_DLG_OK;	//	戻値にOKをセット
	}

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgOutputForm::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgOutputForm::PreTranslateMessage(MSG* pMsg)
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
//	ラベル設定
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgOutputForm::SetLabel( CDatabase* pDB, short nFormSeq )
{
	int					nCnt;			//	カウント用
	CString				szTitle[3];		//	タイトルバッファ（取得用）
	CString				szShow[3];		//	タイトルバッファ（表示用）
	CdlgAddressSearch	clsAdd( this );	//	住所検索クラス（※文字列操作メソッドのみ使用）
	
	//	タイトルバッファ数分ループ
	for( nCnt = 0; nCnt < 3; nCnt++ ){
		//	タイトルバッファクリア
		szTitle[nCnt].Empty();
		szShow[nCnt].Empty();
	}

	//	様式で分岐
	switch( nFormSeq ){

	//------------------------------
	//	④仮払金（前渡金）・貸付金及び受取利息
	//------------------------------
	case ID_FORMNO_041:
	case ID_FORMNO_042:
		GetYoushikiTitle( pDB, ID_FORMNO_041, &szTitle[0] );
		GetYoushikiTitle( pDB, ID_FORMNO_042, &szTitle[1] );
		break;
	//------------------------------
	//	⑩仮受金（前受金・預り金）・源泉取得税預り金
	//------------------------------
	case ID_FORMNO_101:
	case ID_FORMNO_102:
		GetYoushikiTitle( pDB, ID_FORMNO_101, &szTitle[0] );
		GetYoushikiTitle( pDB, ID_FORMNO_102, &szTitle[1] );
		break;
	//------------------------------
	//	⑭役員報酬手当等及び人件費
	//------------------------------
	case ID_FORMNO_141:
	case ID_FORMNO_142:
		GetYoushikiTitle( pDB, ID_FORMNO_141, &szTitle[0] );
		GetYoushikiTitle( pDB, ID_FORMNO_142, &szTitle[1] );
		break;
	//------------------------------
	//	⑮地代家賃等・工業所有権等の使用料
	//------------------------------
	case ID_FORMNO_151:
	case ID_FORMNO_152:
	case ID_FORMNO_153:
		GetYoushikiTitle( pDB, ID_FORMNO_151, &szTitle[0] );
		GetYoushikiTitle( pDB, ID_FORMNO_152, &szTitle[1] );
		GetYoushikiTitle( pDB, ID_FORMNO_153, &szTitle[2] );
		break;
	//------------------------------
	//	⑯雑役・雑損失
	//------------------------------
	case ID_FORMNO_161:
	case ID_FORMNO_162:
		GetYoushikiTitle( pDB, ID_FORMNO_161, &szTitle[0] );
		GetYoushikiTitle( pDB, ID_FORMNO_162, &szTitle[1] );
		break;
	//------------------------------
	//	その他
	//------------------------------
	default:
		//	ダイアログを終了
		ICSDialog::OnCancel();
		break;
	}
	
	//	表示範囲までの文字列を切り取り
	clsAdd.GetSpritString( szTitle[0], &szShow[0], 42 ); 
	clsAdd.GetSpritString( szTitle[1], &szShow[1], 42 ); 
	clsAdd.GetSpritString( szTitle[2], &szShow[2], 42 ); 

	//	ラベルにセット
	m_YoushikiLabel1.UpdateCaption( szShow[0] , 0 );
	m_YoushikiLabel2.UpdateCaption( szShow[1] , 0 );
	m_YoushikiLabel3.UpdateCaption( szShow[2] , 0 );
}

//**************************************************
//	様式タイトル取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				pszTitle	…	タイトル格納バッファ
//	【戻値】	なし
//**************************************************
void CdlgOutputForm::GetYoushikiTitle( CDatabase* pDB, short nFormSeq, CString* pszTitle )
{
	CdbUcInfSub		mfcRec( pDB );		//	uc_inf_subテーブルクラス
	CString			szBuf;				//	バッファ

	try{
		
		//	初期化成功？
		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
			
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
//	出力形式取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgOutputForm::GetFormType( CDatabase* pDB, short nFormSeq )
{
	short				nParent;		//	親番号
	CdbUcInfFormType	mfcRec( pDB );	//	uc_inf_form_typeテーブルクラス
	
	//	親番号取得
	nParent = GetParentFromOrder( pDB, nFormSeq );
	
	try{
		
		//	初期化成功？
		if ( mfcRec.Init( nParent ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();					//	レコードを先頭に移動
				
				//	出力形式が通常？
				if ( mfcRec.m_FormType == ID_FORMTYPE_NORMAL ){
					m_rdoNormal.SetCheck( TRUE );
					m_rdoOne.SetCheck( FALSE );
				}
				//	出力形式が個別
				else{
					m_rdoNormal.SetCheck( FALSE );
					m_rdoOne.SetCheck( TRUE );
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
}

//**************************************************
//	出力形式設定
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgOutputForm::SetFormType( CDatabase* pDB, short nFormSeq )
{
	short				nParent;		//	親番号
	CdbUcInfFormType	mfcRec( pDB );	//	uc_inf_form_typeテーブルクラス
	int					nRet = 1;		//	戻値

	//	親番号取得
	nParent = GetParentFromOrder( pDB, nFormSeq );
	
	try{
		
		//	初期化成功？
		if ( mfcRec.Init( nParent ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();						//	レコードを先頭に移動
				mfcRec.Edit();							//	編集開始

				//	通常にチェックあり？
				if ( m_rdoNormal.GetCheck() == TRUE ){
					mfcRec.m_FormType = ID_FORMTYPE_NORMAL;
				}
				//	個別にチェックあり
				else{
					mfcRec.m_FormType = ID_FORMTYPE_KOBETU;
				}

				mfcRec.Update();						//	更新
				m_FormType = mfcRec.m_FormType;			//	出力形式取得
				nRet = 0;								//	戻値を成功に設定
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
//	親番号取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0以外		…	指定様式番号の親番号
//**************************************************
short CdlgOutputForm::GetParentFromOrder( CDatabase* pDB, short nFormSeq )
{
	short			nRet = 0;		//	戻値初期化
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス

	try{
		//	初期化成功？
		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();			//	レコードを先頭に移動
				nRet = mfcRec.m_FormNo;		//	様式番号の「①－１」の①の部分取得
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
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	OKボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
//**************************************************
int CdlgOutputForm::ShowDialog( CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/ )
{
	m_nRet = ID_DLG_CANCEL;				//	戻値を初期化
	m_FormType = ID_FORMTYPE_NORMAL;	//	出力形式初期化

	//	データベースハンドルあり？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;			//	データベースハンドル取得
				m_nFormSeq = nFormSeq;	//	様式シーケンス番号
				DoModal();				//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}