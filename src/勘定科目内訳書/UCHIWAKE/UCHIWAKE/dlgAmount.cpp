// dlgAmount.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgAmount.h"


IMPLEMENT_DYNAMIC(CdlgAmount, ICSDialog)

/////////////////////////////////////////////////////////////////////////////
// CdlgAmount ダイアログ

//--------------------------------------------------
//	コンストラクタ
//	【引数】	pParent		…	親ハンドル
//--------------------------------------------------
CdlgAmount::CdlgAmount(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgAmount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CdlgAmount)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

CdlgAmount::CdlgAmount( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

CdlgAmount::~CdlgAmount()
{
}

void CdlgAmount::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdlgAmount)
	DDX_Control(pDX, IDC_RADIO_NOTHING, m_rdoNothing);
	DDX_Control(pDX, IDC_RADIO_PAGE, m_rdoPage);
	DDX_Control(pDX, IDC_RADIO_AMOUNT, m_rdoAmount);
	DDX_Control(pDX, IDC_RADIO_BOTH, m_rdoBoth);
	DDX_Control(pDX, IDC_RADIO_NAME_KEI, m_rdoKei);
	DDX_Control(pDX, IDC_RADIO_NAME_GOUKEI, m_rdoGoukei);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CdlgAmount, ICSDialog)
	//{{AFX_MSG_MAP(CdlgAmount)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgAmount::OnBnClickedOkButton)
END_MESSAGE_MAP()


void CdlgAmount::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

/////////////////////////////////////////////////////////////////////////////
// CdlgAmount メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgAmount::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	//	「オフ」あり？
	if ( m_bType & AM_TYPE_OFF ){
		m_rdoNothing.EnableWindow( TRUE );
	}
	else{
		m_rdoNothing.EnableWindow( FALSE );
	}

	//	「頁計」あり？
	if ( m_bType & AM_TYPE_PAGEKEI ){
		m_rdoPage.EnableWindow( TRUE );
	}
	else{
		m_rdoPage.EnableWindow( FALSE ); 
	}
	
	//	「累計」あり？
	if ( m_bType & AM_TYPE_RUIKEI ){
		m_rdoAmount.EnableWindow( TRUE );
	}
	else{
		m_rdoAmount.EnableWindow( FALSE );
	}

	//	「頁計と累計」あり？
	if ( m_bType & AM_TYPE_BOTH ){
		m_rdoBoth.EnableWindow( TRUE );
	}
	else{
		m_rdoBoth.EnableWindow( FALSE );
	}

	//	データ設定
	GetAmountData( m_pDB, m_nFormSeq );

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

HBRUSH CdlgAmount::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgAmount::OnBnClickedOkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	データ設定成功？
	if ( SetAmountData( m_pDB, m_nFormSeq ) == 0 ){
		//	戻値を設定
		m_nRet = ID_DLG_OK;
	}

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgAmount::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgAmount::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	F2キーが押された
		case VK_F2:

			//	F2キーをShift + Tabイベントに書き換え
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );									//	抜ける
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	データ取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgAmount::GetAmountData( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub	mfcRec( pDB );		//	メイン情報テーブルクラス

	try{
	
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	レコードを先頭に移動
				mfcRec.MoveFirst();
				
				//	頁計・累計の値で分岐
				switch( mfcRec.m_OutKei ){
				//	オフ
				case ID_OUTKEI_OFF:
					m_rdoNothing.SetCheck( TRUE );
					break;
				//	頁計
				case ID_OUTKEI_PAGEKEI:

					//	頁計有効？
					if ( m_rdoPage.IsWindowEnabled() == TRUE ){
						m_rdoPage.SetCheck( TRUE );
					}
					else{
						m_rdoNothing.SetCheck( TRUE );
					}
					break;
				//	累計
				case ID_OUTKEI_RUIKEI:

					//	累計有効？
					if ( m_rdoAmount.IsWindowEnabled() == TRUE ){
						m_rdoAmount.SetCheck( TRUE );
					}
					else{
						m_rdoNothing.SetCheck( TRUE );
					}
					break;
				//	頁計と累計
				case ID_OUTKEI_BOTH:

					//	頁計と累計有効？
					if ( m_rdoBoth.IsWindowEnabled() == TRUE ){
						m_rdoBoth.SetCheck( TRUE );
					}
					else{
						m_rdoNothing.SetCheck( TRUE );
					}
					break;
				//	デフォルト
				default:
					m_rdoNothing.SetCheck( TRUE );
					break;
				}
			}
			//	レコード終了
			mfcRec.Fin();
		}

		// 計名称設定
		if( !m_nNameKei ){	// 0→"計"
			m_rdoKei.SetCheck( TRUE );
		}
		else{ // 0以外→"合計"
			m_rdoGoukei.SetCheck( TRUE );
		}
		//switch( m_nNameKei ){
		//	case ID_NAME_KEI:
		//		m_rdoKei.SetCheck( TRUE );
		//		break;
		//	case ID_NAME_GOUKEI:
		//		m_rdoGoukei.SetCheck( TRUE );
		//		break;
		//	default:
		//		m_rdoKei.SetCheck( FALSE );
		//		m_rdoGoukei.SetCheck( FALSE );
		//		break;
		//}
	}
	catch(...){
		
		//	レコードセット開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード終了
			mfcRec.Fin();
		}
	}
}

//**************************************************
//	データ設定
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgAmount::SetAmountData( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub	mfcRec( pDB );		//	メイン情報テーブルクラス
	int				nRet = 1;			//	戻値（初期値は失敗）

	try{
	
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
					
				//	レコードを先頭に移動
				mfcRec.MoveFirst();
				//	編集開始
				mfcRec.Edit();
				
				//	「頁計」にチェックあり？
				if ( m_rdoPage.GetCheck() == TRUE ){
					mfcRec.m_OutKei = ID_OUTKEI_PAGEKEI;
				}
				//	「累計」にチェックあり？
				else if ( m_rdoAmount.GetCheck() == TRUE ){
					mfcRec.m_OutKei = ID_OUTKEI_RUIKEI;
				}
				//	「頁計と累計」にチェックあり？
				else if ( m_rdoBoth.GetCheck() == TRUE ){
					mfcRec.m_OutKei = ID_OUTKEI_BOTH;
				}
				//	上記以外はオフとする
				else{
					mfcRec.m_OutKei = ID_OUTKEI_OFF;
				}
				
				//	レコード更新
				mfcRec.Update();
				//	頁計・累計出力の値を取得
				m_OutKei = mfcRec.m_OutKei;
				//	成功
				nRet = 0;
			}

			//	レコード終了
			mfcRec.Fin();
		}

		//　計名称
		if( m_rdoKei.GetCheck() == TRUE ){
			m_nNameKei = ID_NAME_KEI;
		}
		else if( m_rdoGoukei.GetCheck() == TRUE ){
			m_nNameKei = ID_NAME_GOUKEI;
		}
		else{
			m_nNameKei = ID_NAME_KEI;
		}
	}
	catch(...){
		
		//	レコードセット開かれてる？
		if ( mfcRec.IsOpen() ){
			//	レコード終了
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
//				bType			…	計の種別
//										AM_TYPE_OFF		…	「オフ」を有効
//										AM_TYPE_PAGEKEI	…	「頁計」を有効
//										AM_TYPE_RUIKEI	…	「累計」を有効
//										AM_TYPE_BOTH	…	「頁計と累計」を有効
//										AM_TYPE_ALL		…	全て有効
//	【戻値】	ID_DLG_OK		…	OKボタンで終了
//				ID_DLG_CANCEL	…	キャンセルボタンで終了
//**************************************************
int	CdlgAmount::ShowDialog( CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/, BYTE bType /*=AM_TYPE_ALL*/, int nNameKei /*=0("計")*/)
{
	m_nRet = ID_DLG_CANCEL;		//	戻値を初期化
	m_OutKei = ID_OUTKEI_OFF;	//	頁計・累計出力の値を初期化
	
	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){		

		try{

			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB		= pDB;			//	データベースハンドル取得
				m_nFormSeq	= nFormSeq;		//	様式シーケンス番号
				m_bType		= bType;		//	計の種別
				m_nNameKei	= nNameKei;		//　計の名称（"計"か"合計"か）
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	// 戻値をセット
	//if( m_nRet != ID_DLG_CANCEL ){
	//	nNameKei = m_nNameKei;
	//}
	
	//	戻値を返す
	return( m_nRet );

}
