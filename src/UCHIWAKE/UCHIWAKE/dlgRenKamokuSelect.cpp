// dlgRenKamokuSelect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgRenKamokuSelect.h"


// CdlgRenKamokuSelect ダイアログ

IMPLEMENT_DYNAMIC(CdlgRenKamokuSelect, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgRenKamokuSelect::CdlgRenKamokuSelect(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgRenKamokuSelect::IDD, pParent)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgRenKamokuSelect::~CdlgRenKamokuSelect()
{
}

void CdlgRenKamokuSelect::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RADIO_SELECT, m_rdoSelect);
	DDX_Control(pDX, IDC_RADIO_ADD, m_rdoAdd);
	DDX_Control(pDX, IDC_COMBO_KAMOKU, m_cboKamoku);
	DDX_Control(pDX, IDC_KAMOKUADD_TEXT, m_txtKamoku);
}


BEGIN_MESSAGE_MAP(CdlgRenKamokuSelect, ICSDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgRenKamokuSelect::OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_RADIO_ADD, &CdlgRenKamokuSelect::OnBnClickedRadioAdd)
	ON_BN_CLICKED(IDC_RADIO_SELECT, &CdlgRenKamokuSelect::OnBnClickedRadioSelect)
END_MESSAGE_MAP()

void CdlgRenKamokuSelect::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。

}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamokuSelect::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
// 158016 add -->
	// 財務DBクラスを取得
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
// 158016 add <--

	// コントロール設定
	SetKamokuData();

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（選択ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamokuSelect::OnBnClickedOkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if ( GetKamokuData() == 0 ){
		m_nRet = ID_DLG_OK;
		ICSDialog::OnOK();
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	新規科目追加ラジオボタン選択
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamokuSelect::OnBnClickedRadioAdd()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_cboKamoku.EnableWindow( FALSE );
	m_txtKamoku.EnableWindow( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	既存科目選択ラジオボタン選択
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamokuSelect::OnBnClickedRadioSelect()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_cboKamoku.EnableWindow( TRUE );
	m_txtKamoku.EnableWindow( FALSE );
}
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgRenKamokuSelect::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgRenKamokuSelect::PreTranslateMessage(MSG* pMsg)
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
//	コントロール設定処理
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgRenKamokuSelect::SetKamokuData()
{
	CdbUcLstKamoku mfcRec( m_pDB );
	BOOL	blnSelFlag = FALSE;	// 既存科目の有効/無効フラグ
	BOOL	blnAddFlag = FALSE;	// 新規科目の有効/無効フラグ
	
	// 取得した財務科目名称から空白を取り除く
	m_Knnam = m_clsFunc.Trim( m_Knnam );
	// 内訳書科目の科目名称数を越えさせない
	if ( m_Knnam.GetLength() > 24 ){
		m_Knnam = m_Knnam.Left(24);
	}

	// 選択＆追加ラジオチェックを外す
	m_rdoAdd.EnableWindow( FALSE );
	m_rdoAdd.SetCheck( FALSE );
	m_cboKamoku.EnableWindow( FALSE );
	m_rdoSelect.EnableWindow( FALSE );
	m_rdoSelect.SetCheck( FALSE );
	m_txtKamoku.EnableWindow( FALSE );

	// uc_lst_Kamokuから値を取得
	try{
		//	レコード初期化成功？
		if ( mfcRec.Init( m_nFormSeq ) == DB_ERR_OK ){
			// １件以上登録されている?
			if ( mfcRec.GetRecordCount() > 0 ){
				blnSelFlag = TRUE;
				m_cboKamoku.ResetContent();
				while( !mfcRec.IsEOF() ){
					// 既存科目コンボに科目名追加
					m_cboKamoku.AddString( mfcRec.m_KnName );
					mfcRec.MoveNext();
				}
			}
			// 以下の処理でKnOrderの取得を確認するため一旦0を格納
			mfcRec.m_KnOrder = 0;

			// Trimした財務科目名称と同名の内訳書科目が
			// あれば選択値をその科目に移す
			if ( mfcRec.CheckRepeat( m_nFormSeq , m_Knnam, m_Knkana ) == DB_ERR_OK ){
				if ( mfcRec.m_KnOrder > 0 ){
					m_cboKamoku.SetCurSel( mfcRec.m_KnOrder - 1 );
				}
				else{
// 158016 del -->
					//m_cboKamoku.SetCurSel(0);
// 158016 del <--
// 158016 add -->
					if(m_nFormSeq != ID_FORMNO_142)	{
						// ⑭－２人件費の内訳書以外 → 選択したのが明細科目であれば小分類科目名称で比較する
						// 小分類科目の場合も拡張されている場合があるので明細と同じ処理を行う
						int		knorder=0;
						//if(m_Kncod.Right(2) != _T("00"))	{
							CString	strFilter=_T("");
							CString	gknam=_T("");
							char	tmp[64]={0};
							strFilter.Format(_T("gkcod LIKE '%s%%'"),m_Kncod.Left(6));
							strFilter += _T(" AND (gkvoid & 0x01) = 0");
							if(m_pZmSub->GokeiMeisyoOpen(strFilter,0) != 0)	{
								m_clsFunc.ZmSubErrMsg(m_pZmSub);
								return;
							}
							if(m_pZmSub->gkrec->GetRecordCount() > 0)	{
								// 拡張された科目の場合、拡張元を取得
								if(m_pZmSub->gkrec->gksrc.IsEmpty() == FALSE)	{
									strFilter.Format(_T("gkcod='%s'"),m_pZmSub->gkrec->gksrc);
									strFilter += _T(" AND (gkvoid & 0x01) = 0");
									m_pZmSub->gkrec->Requery(strFilter,0);
								}
								wsprintf(tmp,_T("%-24.24s"),m_pZmSub->gkrec->gknam);
								gknam.Format(_T("%s"),tmp);
								gknam = m_clsFunc.Trim(gknam);
								if(gknam.GetLength() > 24)	{
									gknam = gknam.Left(24);
								}
								if(mfcRec.CheckRepeat(m_nFormSeq,gknam,_T("")) == DB_ERR_OK)	{
									knorder = mfcRec.m_KnOrder;
								}
							}
							m_pZmSub->GokeiMeisyoClose();
						//}
						if(knorder > 0)	{
							m_cboKamoku.SetCurSel(knorder - 1);
						}
						else	{
							m_cboKamoku.SetCurSel(0);
						}
					}
					else	{
						// ⑭－２人件費の内訳書 → 科目内部コードから判定する
						m_cboKamoku.SetCurSel(KamokuCheck142());
					}
// 158016 add <--
				}
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

	// 編集中の様式が科目コンボの無い帳表の場合
	// 新規内訳書科目追加はできない
	switch( m_nFormSeq ){

	//case ID_FORMNO_021:	受取手形の内訳書に科目欄追加により新規追加可
	//case ID_FORMNO_042:
	//case ID_FORMNO_081:
	case ID_FORMNO_142:
		break;
	default:
		blnAddFlag = TRUE;
		break;
	}

	// 既存科目選択欄設定
	m_rdoSelect.EnableWindow( blnSelFlag );
	m_cboKamoku.EnableWindow( blnSelFlag );

	// 新規科目追加欄設定
	m_rdoAdd.EnableWindow( blnAddFlag );
	m_txtKamoku.EnableWindow( blnAddFlag );
	if ( blnAddFlag == TRUE ){
		m_txtKamoku.SetCaption( m_Knnam );
	}

	// チェック項目を選択
	if ( blnSelFlag == TRUE ){
		m_rdoSelect.SetCheck(TRUE);
		m_cboKamoku.EnableWindow( TRUE );
		m_txtKamoku.EnableWindow( FALSE );
	}
	else if ( blnAddFlag == TRUE ){
		m_rdoAdd.SetCheck(TRUE);
		m_txtKamoku.EnableWindow( TRUE );
		m_cboKamoku.EnableWindow( FALSE );
	}

}

//**************************************************
//	データ取得処理
//	【引数】	なし
//	【戻値】	なし
//**************************************************
int CdlgRenKamokuSelect::GetKamokuData()
{
	CdbUcLstKamoku	mfcRec( m_pDB );
	int				intType = 0;			// 内訳書科目の登録種(1:選択 2:追加)
	int				intCombo = 0;			// コンボ選択値
	int				intRet = -1;			// 戻り値
	int				intKnSeq = 0;			// ローカル科目シーケンス番号
	CString			strKnnam = "";			// 追加科目名
	BOOL			blnUcKmkFlag = FALSE;	// 新規内訳書科目追加確認フラグ

	// 選択 or 追加 どちらが選択されているか?
	if(m_rdoSelect.GetCheck() == TRUE)	{
		intType = 1;
	}
	else if(m_rdoAdd.GetCheck() == TRUE)	{
		intType = 2;
	}

	if(intType > 0)	{
		try	{
			// 選択の場合
			if(intType == 1)	{
				// コンボ選択値を取得
				intCombo = m_cboKamoku.GetCurSel() + 1;
				// 選択コンボ値のシーケンス番号取得
				if(mfcRec.RequeryKnOrder(m_nFormSeq, intCombo) == DB_ERR_OK)	{
					if(mfcRec.GetRecordCount() > 0)	{
						m_KnSeq = mfcRec.m_KnSeq;
						intRet = 0;
					}
				}
			}
			// 追加の場合
			else if(intType == 2)	{
				// エディットから科目名取得
				strKnnam = m_txtKamoku.GetCaption();
				// 追加科目名はNULLを許可しない
				if(m_clsFunc.Trim( strKnnam ) == "")	{
					ICSMessageBox(_T("入力されていません。"), MB_OK, 0, 0, this);
				}
				else	{
					// 末尾スペースを削除（見た目上同じ科目名登録されるのを防ぐ）
					strKnnam = m_clsFunc.DeleteRightSpace(strKnnam);

					// 新規の内訳書科目かどうかチェック
					if(mfcRec.CheckRepeat(m_nFormSeq, strKnnam) == DB_ERR_OK)	{
						if(mfcRec.GetRecordCount() <= 0)	{
							blnUcKmkFlag = TRUE;
						}
					}
					// 末尾スペースを削除
					m_Knkana = m_clsFunc.DeleteRightSpace(m_Knkana);
					// 新規追加科目の重複チェック
					intKnSeq = mfcRec.CheckEntry(strKnnam, m_nFormSeq, m_Knkana);
					if(intKnSeq > 0)	{
						m_KnSeq = intKnSeq;
						// 未登録の内訳書科目が登録成功?
						if(blnUcKmkFlag == TRUE)	{
							m_UcKmkEntry = 1;
						}
						intRet = 0;
					}
				}
			}
			mfcRec.Fin();
		}
		catch(...)	{
			// レコードが開かれてる？
			if(mfcRec.IsOpen() == TRUE)	{
				// レコード終了
				mfcRec.Fin();
			}
		}
	}

	return(intRet);
}

//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pRenInfo		…	ICS会社情報構造体
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	実行ボタン押した
//				ID_DLG_CANCEL	…	キャンセルボタン押した
//**************************************************
// 158016 del -->
//int CdlgRenKamokuSelect::ShowDialog( CDatabase* pDB , int nFormSeq , CString Knnam, CString Knkana )
// 158016 del <--
// 158016 add -->
int CdlgRenKamokuSelect::ShowDialog( CDatabase* pDB , int nFormSeq , CString Knnam, CString Knkana, CString Kncod )
// 158016 add <--
{
// midori 152137 add -->
	CFuriganaConverter	fc;
// midori 152137 add <--
	m_pDB = NULL;
	m_nFormSeq = ID_FORMNO_011;
	m_nRet = ID_DLG_CANCEL;
	m_KnSeq = 0;
	m_Knnam = "";
	m_Knkana = "";
// 158016 add -->
	m_Kncod = "";
// 158016 add <--
	m_UcKmkEntry = 0;

	//	データベースハンドルヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_nFormSeq = nFormSeq;		//	様式番号取得
				m_Knnam = Knnam;			//	財務科目名称取得
				m_Knkana = Knkana;			//  財務科目カナ取得
// midori 152137 add -->
				if(m_Knkana.IsEmpty() == TRUE)	{
					if(m_Knnam.IsEmpty() != TRUE)	{
						m_Knkana = fc.Convert(m_Knnam);
					}
				}
// midori 152137 add <--
// 158016 add -->
				m_Kncod = Kncod;
// 158016 add <--
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	return( m_nRet );
}

// 158016 add -->
//**************************************************************************************
//	⑭－２人件費の内訳書【専用】
//　選択された財務科目が「役員報酬手当」「従業員給料手当」「従業員賃金手当」のうち
//　どの項目に該当するかを判断する
//	【引数】	なし
//	【戻値】	int	… 0:役員報酬手当 or 該当なし　1:従業員給料手当　2:従業員賃金手当
//**************************************************************************************
int CdlgRenKamokuSelect::KamokuCheck142( void )
{
	int			ret=0;
	char		tmp[64]={0};
	CString		strFilter=_T("");
	CString		gkcod=_T(""),gknam=_T("");

	strFilter.Format(_T("gkcod LIKE '%s%%'"),m_Kncod.Left(6));
	strFilter += _T(" AND (gkvoid & 0x01) = 0");
	if(m_pZmSub->GokeiMeisyoOpen(strFilter,0) != 0)	{
		m_clsFunc.ZmSubErrMsg(m_pZmSub);
		return(0);
	}
	if(m_pZmSub->gkrec->GetRecordCount() > 0)	{
		if(m_pZmSub->gkrec->gksrc.IsEmpty() == TRUE)	gkcod = m_pZmSub->gkrec->gkcod;
		else											gkcod = m_pZmSub->gkrec->gksrc;
		m_pZmSub->VolumeOpen();
		// 病院以外
		if(m_pZmSub->zvol->apno != 0x30)	{
			if(gkcod.Left(6) == _T("0a0104"))	ret=1;	// 0a010400 : 給　料　手　当
			if(gkcod.Left(6) == _T("0a0105"))	ret=1;	// 0a010500 : 賞　与　手　当
			if(gkcod.Left(6) == _T("0a0106"))	ret=1;	// 0a010600 : 退　　職　　金
			if(gkcod.Left(6) == _T("0a0107"))	ret=1;	// 0a010700 : 雑　　　　　給
			if(gkcod.Left(4) == _T("0f02"))		ret=2;	// 0f020000 : 労　　務　　費
		}
		// 病院
		else	{
			if(gkcod.Left(4) == _T("0a01"))		ret=1;	// 0a010000 : 給　　与　　費
		}
		m_pZmSub->VolumeClose();
	}
	m_pZmSub->GokeiMeisyoClose();

	return(ret);
}
// 158016 add <--
