#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
// マイナンバーコントロール用：コールバック関数
// ※グローバル関数
//-----------------------------------------------------------------------------
static int MNCallbackFunction( void* wnd, int index, int mes_id, MSG* pMsg )
{
	
	// 実際はCCoBasicDialogのMNEditTerminationへ
	if( mes_id == ID_MN_MESSAGE ){
		TRACE("MNCallbackFunction ID_MN_MESSAGE\n");
//-- '16.01.07 --
//		return ((CDBSyzShinMainView*)wnd)->MNEditTermination( index, pMsg );
//---------------
		if( wnd && ::IsWindow(((CDBSyzShinMainView*)wnd)->m_hWnd) && pMsg ){
			return ((CDBSyzShinMainView*)wnd)->MNEditTermination( index, pMsg );
		}
//---------------
	}
	else if( mes_id == ID_MN_EDIT_ON ){
		TRACE("MNCallbackFunction ID_MN_EDIT_ON\n");
//		return ((CBaseInfoDlg*)wnd)->MNEditEditOn(index);
	}
	else if( mes_id == ID_MN_EDIT_OFF ){
		TRACE("MNCallbackFunction ID_MN_EDIT_OFF\n");
//-- '16.01.07 --
//		return ((CDBSyzShinMainView*)wnd)->MNEditEditOff( index );
//---------------
//		if( wnd && ::IsWindow(((CDBSyzShinMainView*)wnd)->m_hWnd) && pMsg ){
		if( wnd && ::IsWindow(((CDBSyzShinMainView*)wnd)->m_hWnd) ){
			return ((CDBSyzShinMainView*)wnd)->MNEditEditOff( index );
		}
//---------------
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
// マイナンバーコントロールの初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::InitMynumberControl()
{
	if( m_pMnec == NULL ){
		m_pMnec = new CICSMNEditController8();
		NTBMOD8_BeginProc( 0, NULL );
		NTBMOD8_InitNoHistoryTbl( 0, &m_nhtbl );
		NTBMOD8_InitNoDataTbl( 0, &m_ndtbl );
		SetNoHistory();
		SetCoNoHistory();
	}

	int kubun = MN_HOJIN;
	if( m_pSnHeadData->IsSoleProprietor() ){
		kubun = MN_KOJIN;
	}

	HWND edit[10] = {NULL};
	if( kubun == MN_HOJIN ){
		edit[0] = m_MnEdit1.m_hWnd;
		edit[1] = m_MnEdit2.m_hWnd;
		edit[2] = m_MnEdit3.m_hWnd;
		edit[3] = m_MnEdit4.m_hWnd;
	}
	else if( kubun == MN_KOJIN ){
		edit[0] = m_MnEdit2.m_hWnd;
		edit[1] = m_MnEdit3.m_hWnd;
		edit[2] = m_MnEdit4.m_hWnd;
	}

	int	sn_seq = 0;
	if( m_pMainFrame ){
		sn_seq = m_pMainFrame->GetSnSeq();
	}

	if( (m_svMynoInf.kubun!=kubun) || (m_svMynoInf.apno!=m_pZmSub->zvol->apno) ){
		// 初期化
		m_pMnec->Init( this, kubun, m_pZmSub->zvol->apno, TRUE, edit );
		// 上下マージン　中央
		m_pMnec->SetHeightMargin( MN_HEIGHT_CENTER );
		// コールバック関数セット
		callback_func func_pointer = MNCallbackFunction;
		m_pMnec->SetCallbackFunction( func_pointer );
	}
	
	CString	data;
	if( (m_svMynoInf.kubun!=kubun) || (m_svMynoInf.sn_seq!=sn_seq) ){
		MoveEditControl( kubun, sn_seq );
		GetData( kubun, sn_seq, data );
		m_pMnec->SetData( data );
	}
	m_pMnec->GetData( data );

	// 個人番号削除ボタンの表示／非表示を切り替える
	ChangeDispKjdelBtn();

//-- '15.08.24 --
//	if( (sn_seq<=0) || ((kubun==MN_KOJIN)&&(data.GetLength())) || InLockInput() ){
//---------------
	if( (sn_seq<=0) || ((kubun==MN_KOJIN)&&(IsEmptyKjhash()==false)) || InLockInput() ){
//---------------
		m_pMnec->EnableWindow( FALSE );
	}
	else{
		m_pMnec->EnableWindow( TRUE );
	}

	// 情報の保存
	memset( &m_svMynoInf, '\0', sizeof(SYZ_MYNO_SVINF) );
	m_svMynoInf.apno = m_pZmSub->zvol->apno;
	m_svMynoInf.kubun = kubun;
	m_svMynoInf.sn_seq = sn_seq;

	return 0;
}

//-----------------------------------------------------------------------------
// 空の個人ハッシュ？
//-----------------------------------------------------------------------------
// 返送値	true	：	空
//			false	：	何らかのデータ有
//-----------------------------------------------------------------------------
bool CDBSyzShinMainView::IsEmptyKjhash()
{
	if( IsEmptyKjhash(m_pSnHeadData->Sn_KOJIN_HASH, sizeof(m_pSnHeadData->Sn_KOJIN_HASH)) ){
		return true;
	}
	else{
		return false;
	}
}

//-----------------------------------------------------------------------------
// データの取得
//-----------------------------------------------------------------------------
// 引数	kubun	：	法人・個人区分
//		sn_seq	：	履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetData( int kubun, int sn_seq, CString& data )
{
	if( kubun == MN_HOJIN ){
		data.Format( _T("%s"), m_pSnHeadData->Sn_HOJIN_NO );
		data.Replace( _T("　"), _T(" ") );	// 全角スペースを半角スペースに置き換え
		data.TrimRight();
	}
	else if( kubun == MN_KOJIN ){
		NODATATBL ndtbl;				// これを使用するのは個人番号でnewされたときのみ
		NTBMOD8_InitNoDataTbl( 0, &ndtbl );

		unsigned char	ss[64]={0};
		memmove( ss, m_pSnHeadData->Sn_KOJIN_HASH, sizeof(m_pSnHeadData->Sn_KOJIN_HASH) );

//--> '15.07.31 INS START
		if( IsEmptyKjhash(m_pSnHeadData->Sn_KOJIN_HASH, sizeof(m_pSnHeadData->Sn_KOJIN_HASH)) == TRUE ){
			data.Empty();
			return 0;
		}
//<-- '15.07.31 INS END

		CString	hash_all;
		for( int i=0; i<16; i++ ){
			CString	num;
			num.Format( _T("%02x"), ss[i] );
			hash_all += num.Left(1) + num.Right(1);
		}
		// ハッシュ値を個人番号保存テーブルにセット
		memmove( ndtbl.arrow, hash_all, sizeof(ndtbl.arrow) );

		// 個人番号取得
		int st = NTBMOD8_GetNoDataTbl( 0, &ndtbl );
		if( st < 0 ){
			CString err;
			NTBMOD8_GetErrorMessage(err);
			if(!err.IsEmpty()){
				ICSMessageBox(err);
				data.Empty();
				return -1;
			}
		}
		data.Format( _T("%s"), ndtbl.no );

//--> '15.09.02 INS START
		if( MynoAccessPerm() == FALSE ){
			return 0;
		}
//<-- '15.09.02 INS END

		// 履歴情報作成
		if( st == 0 ){
			NOHISTTBL tmp_nhtbl;
			NTBMOD8_InitNoHistoryTbl( 0, &tmp_nhtbl );
			tmp_nhtbl = m_nhtbl;
			tmp_nhtbl.user_play = NTBMOD_USER_PLAY_BROWSE | NTBMOD_USER_PLAY_APPLI;

			CNoHistoryTblArray nhtblary;
			nhtblary.Add( tmp_nhtbl );

			// 履歴テーブルにセット
			NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );

//2018.08.21 INSERT START
			m_MyNumBrowse = TRUE;
//2018.08.21 INSERT END
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 区分に合わせたコントロールの再配置
//-----------------------------------------------------------------------------
// 引数	kubun	：	法人・個人区分
//		sn_seq	：	履歴シーケンス
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::MoveEditControl( int kubun, int sn_seq )
{
	CRect	rctCono;
	GetDlgItem(IDC_STATIC_B51)->GetWindowRect( rctCono );
	ScreenToClient( rctCono );

	if( kubun == MN_HOJIN ){
		// 法人番号
		m_StaticB10.ShowWindow( SW_HIDE );
		m_StaticB10.SetWindowTextA( _T("法人番号") );
		m_StaticB10.ShowWindow( SW_SHOW );

		CRect	rctEdit1, rctEdit2, rctEdit3, rctEdit4;
		m_MnEdit1.GetWindowRect( rctEdit1 );
		ScreenToClient( rctEdit1 );
		m_MnEdit2.GetWindowRect( rctEdit2 );
		ScreenToClient( rctEdit2 );
		m_MnEdit3.GetWindowRect( rctEdit3 );
		ScreenToClient( rctEdit3 );
		m_MnEdit4.GetWindowRect( rctEdit4 );
		ScreenToClient( rctEdit4 );

		CRect	rctSt1, rctSt2, rctSt3;
		GetDlgItem(IDC_STATIC_B11)->GetWindowRect( rctSt1 );
		ScreenToClient( rctSt1 );
		GetDlgItem(IDC_STATIC_B12)->GetWindowRect( rctSt2 );
		ScreenToClient( rctSt2 );
		GetDlgItem(IDC_STATIC_B13)->GetWindowRect( rctSt3 );
		ScreenToClient( rctSt3 );

		long	ed1Width=0, ed2Width=0, stWidth=0;
		ed1Width = rctEdit1.Width();
		ed2Width = rctEdit2.Width();
		stWidth = rctSt1.Width();

		rctEdit1.left = rctCono.left;
		rctEdit1.right = rctEdit1.left + ed1Width;
		m_MnEdit1.MoveWindow( rctEdit1 );
		m_MnEdit1.EnableWindow( TRUE );
		m_MnEdit1.ShowWindow( SW_SHOW );

		rctSt1.left = rctEdit1.left + rctEdit1.Width();
		rctSt1.right = rctSt1.left + stWidth;
		GetDlgItem(IDC_STATIC_B11)->MoveWindow( rctSt1 );
		GetDlgItem(IDC_STATIC_B11)->EnableWindow( TRUE );
		GetDlgItem(IDC_STATIC_B11)->ShowWindow( SW_SHOW );

		rctEdit2.left = rctSt1.left + rctSt1.Width();
		rctEdit2.right = rctEdit2.left + ed2Width;
		m_MnEdit2.MoveWindow( rctEdit2 );

		rctSt2.left = rctEdit2.left + rctEdit2.Width();
		rctSt2.right = rctSt2.left + stWidth;
		GetDlgItem(IDC_STATIC_B12)->MoveWindow( rctSt2 );

		rctEdit3.left = rctSt2.left + rctSt2.Width();
		rctEdit3.right = rctEdit3.left + ed2Width;
		m_MnEdit3.MoveWindow( rctEdit3 );

		rctSt3.left = rctEdit3.left + rctEdit3.Width();
		rctSt3.right = rctSt3.left + stWidth;
		GetDlgItem(IDC_STATIC_B13)->MoveWindow( rctSt3 );

		rctEdit4.left = rctSt3.left + rctSt3.Width();
		rctEdit4.right = rctEdit4.left + ed2Width;
		m_MnEdit4.MoveWindow( rctEdit4 );
	}
	else if( kubun == MN_KOJIN ){
		// 法人番号
		m_StaticB10.ShowWindow( SW_HIDE );
		m_StaticB10.SetWindowTextA( _T("個人番号") );
		m_StaticB10.ShowWindow( SW_SHOW );

		CRect	rctEdit1, rctEdit2, rctEdit3, rctEdit4;
		m_MnEdit1.GetWindowRect( rctEdit1 );
		ScreenToClient( rctEdit1 );
		m_MnEdit2.GetWindowRect( rctEdit2 );
		ScreenToClient( rctEdit2 );
		m_MnEdit3.GetWindowRect( rctEdit3 );
		ScreenToClient( rctEdit3 );
		m_MnEdit4.GetWindowRect( rctEdit4 );
		ScreenToClient( rctEdit4 );

		CRect	rctSt1, rctSt2, rctSt3;
		GetDlgItem(IDC_STATIC_B11)->GetWindowRect( rctSt1 );
		ScreenToClient( rctSt1 );
		GetDlgItem(IDC_STATIC_B12)->GetWindowRect( rctSt2 );
		ScreenToClient( rctSt2 );
		GetDlgItem(IDC_STATIC_B13)->GetWindowRect( rctSt3 );
		ScreenToClient( rctSt3 );

		CRect	rctBtn;
		GetDlgItem(IDC_BUTTON_KJDEL)->GetWindowRect( rctBtn );
		ScreenToClient( rctBtn );

		long	edWidth=0, stWidth=0, btnWidth=0, btnHeight=0;
		edWidth = rctEdit2.Width();
		stWidth = rctSt1.Width();
		btnWidth = rctBtn.Width();
		btnHeight = rctBtn.Height();

//		rctEdit1.left = rctCono.left;
//		m_MnEdit1.MoveWindow( rctEdit1 );
		m_MnEdit1.EnableWindow( FALSE );
		m_MnEdit1.ShowWindow( SW_HIDE );

//		rctSt1.left = rctEdit1.left + rctEdit1.Width();
//		GetDlgItem(IDC_STATIC_B11)->MoveWindow( rctSt1 );
		GetDlgItem(IDC_STATIC_B11)->EnableWindow( FALSE );
		GetDlgItem(IDC_STATIC_B11)->ShowWindow( SW_HIDE );

		rctEdit2.left = rctCono.left;
		rctEdit2.right = rctEdit2.left + edWidth;
		m_MnEdit2.MoveWindow( rctEdit2 );

		rctSt2.left = rctEdit2.right;
		rctSt2.right = rctSt2.left + stWidth;
		GetDlgItem(IDC_STATIC_B12)->MoveWindow( rctSt2 );

		rctEdit3.left = rctSt2.right;
		rctEdit3.right = rctEdit3.left + edWidth;
		m_MnEdit3.MoveWindow( rctEdit3 );

		rctSt3.left = rctEdit3.right;
		rctSt3.right = rctSt3.left + stWidth;
		GetDlgItem(IDC_STATIC_B13)->MoveWindow( rctSt3 );

		rctEdit4.left = rctSt3.right;
		rctEdit4.right = rctEdit4.left + edWidth;
		m_MnEdit4.MoveWindow( rctEdit4 );

		// 個人番号削除ボタン
		rctBtn.left = rctEdit4.right;
		rctBtn.right = rctBtn.left + btnWidth;
		rctBtn.top = rctEdit4.top;
		rctBtn.bottom = rctEdit4.bottom;
		GetDlgItem(IDC_BUTTON_KJDEL)->MoveWindow( rctBtn );
	}

	if( m_pMnec ){
		if( (sn_seq<=0) || ((kubun==MN_KOJIN)&&(IsEmptyKjhash()==false)) || InLockInput() ){
			m_pMnec->EnableWindow( FALSE );
		}
		else{
			m_pMnec->EnableWindow( TRUE );
		}
	}
}

//-----------------------------------------------------------------------------
// マイナンバー入力コントロールから他のコントロールへ移動する処理
//-----------------------------------------------------------------------------
// 引数	index	：	入力コントロール内でのインデックス
//		pMsg	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::MNEditTermination( int index, MSG* pMsg )
{
	if( m_pMnec == NULL ){
		return 1;
	}

	if( m_pMainFrame && m_pMainFrame->IsEndStart() ){
		return 1;
	}

	int	rt = 0;

	CString	number;

	if( pMsg->message == WM_KEYDOWN ){
		switch (pMsg->wParam){
			case VK_F2:
			case VK_UP:
			case VK_LEFT:
				if( index == 0 ){
//-- '15.12.16 --
//					if( CheckMNumber(TRUE, TRUE) ){
//---------------
					if( CheckMNumber(FALSE, -1) ){
//---------------
						c_set( IDC_CHECK_RIREKISEL );
					}
					else{
						rt = 1;
					}
				}
				break;
			case VK_DOWN:
			case VK_RIGHT:
			case VK_RETURN:
				if( index == GetMNLastIndex() ){
//-- '15.12.16 --
//					if( CheckMNumber(TRUE, TRUE) ){
//---------------
					if( CheckMNumber(FALSE, -1) ){
//---------------
						c_set( IDC_INPUT_BTN );
					}
					else{
						rt = 1;
					}
				}
				break;
//--> '15.09.07 INS START
			case VK_HOME:
				GetHjInfoFromHjno();
				rt = 1;
				break;
//--> '15.09.07 INS END
			case VK_TAB:
			{
				char	cShift = 0;
				cShift = (char)GetKeyState( VK_SHIFT );
				if( cShift&0x80 ){
					if( index == 0 ){
//-- '15.12.16 --
//						if( CheckMNumber(TRUE, TRUE) ){
//---------------
						if( CheckMNumber(FALSE, -1) ){
//---------------
							c_set( IDC_CHECK_RIREKISEL );
						}
						else{
							rt = 1;
						}
					}
				}
				else{
					if( index == GetMNLastIndex() ){
//-- '15.12.16 --
//						if( CheckMNumber(TRUE, TRUE) ){
//---------------
						if( CheckMNumber(FALSE, -1) ){
//---------------
							c_set( IDC_INPUT_BTN );
						}
						else{
							rt = 1;
						}
					}
				}
			}
				break;
		}
	}
	return rt;
}

//-----------------------------------------------------------------------------
// マイナンバー：コントロールEditOff
//-----------------------------------------------------------------------------
// 引数	index	：	マイナンバー上のインデックス
//-----------------------------------------------------------------------------
// 返送値	1	：	以降で処理を行う
//			0	：	以降で処理を行わない
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::MNEditEditOff( int index )
{
	if( m_pMainFrame && m_pMainFrame->IsEndStart() ){
		return 0;
	}

	// データの取得
	SetHeadDataFromControl();

	return 1;
}

//-----------------------------------------------------------------------------
// コントロールから実データへセット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetHeadDataFromControl()
{
	if( m_pMnec == NULL ){
		return -1;
	}

	if( m_svMynoInf.kubun == MN_HOJIN ){
		CString number;
		int st = m_pMnec->GetData( number, FALSE );
		if( st < 0 ){
			if( number.IsEmpty() == FALSE ){
				return -1;
			}
		}

		// 最終チェック
		st = m_pMnec->CheckNumber( number );
		if( st == 0 ){
			return -1;
		}

		memset( m_pSnHeadData->Sn_HOJIN_NO, '\0', sizeof(m_pSnHeadData->Sn_HOJIN_NO) );
		wsprintf( m_pSnHeadData->Sn_HOJIN_NO, _T("%s"), number.GetBuffer() );
		memset( m_pSnHeadData->Sn_HOJIN_NO_EXT, '\0', sizeof(m_pSnHeadData->Sn_HOJIN_NO_EXT) );

		TgPackData	*pTgdata = NULL;
		if( m_pMainFrame ){
			pTgdata = m_pMainFrame->GetTgData();
		}

		m_pSnHeadData->RegistMynumber( m_pZmSub, pTgdata, m_pMainFrame->GetSnSeq(), 0x01 );
	}
	else if( m_svMynoInf.kubun == MN_KOJIN ){
		// 当年度で顧問先登録
//		if( m_pMnec->IsRegist() ){
//			return 0;
//		}
		if( RegistKojinMN() ){
			return -1;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------
// 個人番号の登録
//----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//----------------------------------------------------------------------------
int CDBSyzShinMainView::RegistKojinMN()
{
	// 前回の登録情報との一致をみる為に、前回のデータを保存
	NODATATBL save_ndtbl = m_ndtbl;

	// 履歴パケットセット
	NTBMOD8_InitNoDataTbl( 0, &m_ndtbl );
	NTBMOD8_InitNoHistoryTbl( 0, &m_nhtbl );
	SetNoHistory();		// マスター情報
	SetCoNoHistory();	// 顧問先情報

	CString number;
//-- '15.12.16 --
//	int st = m_pMnec->GetData( number, TRUE );
//---------------
	int st = m_pMnec->GetData( number, 0 );
//---------------
	if( st < 0 ){
		return -1;
	}

	// 最終チェック
	st = m_pMnec->CheckNumber( number );
	if( st == FALSE ){
		return -1;
	}

	// 変更なしなら帰る
	if( number.Compare(save_ndtbl.no) == 0 ){
		return 1;
	}

	// 財務ｄｂの場合、会社名を情報部の氏名とする
	strcpy_s( m_ndtbl.nam2, sizeof(m_ndtbl.nam2), m_nhtbl.mst_nam );

	// 個人番号
	strcpy_s( m_ndtbl.no, sizeof(m_ndtbl.no), number );

	// 使用業種
	m_ndtbl.gyousyu_flg = NTBMOD_GYFLG_SYOHI;

	// 個人番号データベースへ書き込み
	st = NTBMOD8_SetNoDataTbl( 0, &m_ndtbl, &m_nhtbl );
	if( st == -1 ){
		CString err;
		NTBMOD8_GetErrorMessage( err );
		ICSMessageBox( err );
//		v.kojin_hash.RemoveAll();
		return -1;
	}

	int size = sizeof(m_ndtbl.arrow) / sizeof(m_ndtbl.arrow[0]);
	if( IsEmptyKjhash(m_ndtbl.arrow, size) ){
		return 0;
	}

	// szsnk
	memset( m_pSnHeadData->Sn_KOJIN_HASH, '\0', sizeof(m_pSnHeadData->Sn_KOJIN_HASH) );
	for( int i=0; i<size/2; i++ ){
		CString str_hash;
		str_hash = m_ndtbl.arrow[i*2];
		str_hash += m_ndtbl.arrow[(i*2)+1];

		BYTE hash = (BYTE)strtol(str_hash, NULL, 16);
		m_pSnHeadData->Sn_KOJIN_HASH[i] = hash;
	}

	TgPackData	*pTgdata = NULL;
	if( m_pMainFrame ){
		pTgdata = m_pMainFrame->GetTgData();
	}
	m_pSnHeadData->RegistMynumber( m_pZmSub, pTgdata, m_pMainFrame->GetSnSeq(), 0x02 );

	return 0;
}

//-----------------------------------------------------------------------------
// 個人番号履歴情報セット
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetNoHistory()
{
	m_nhtbl.mst_code = m_pZmSub->zvol->v_cod;
	m_nhtbl.mst_apno = m_pZmSub->zvol->apno;
//-- '15.08.11 --
//	memmove( m_nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(m_nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
//---------------
//2018.08.21 UPDATE START
	//int len =  m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );
	//if( len ){
	//	memmove( m_nhtbl.mst_nam, m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );
	//}
	//else{
	//	memmove( m_nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(m_nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
	//}
	memset(m_nhtbl.mst_nam, '\0', sizeof(m_nhtbl.mst_nam));
	memset(m_nhtbl.tgt_char, '\0', sizeof(m_nhtbl.tgt_char));
	memmove( m_nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(m_nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
	memmove( m_nhtbl.tgt_char, m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );

	// マスター情報に決算期間を追加
	CString	mst_nam_str, kessan_str;
	mst_nam_str = m_nhtbl.mst_nam;
	mst_nam_str = mst_nam_str.TrimRight();
	m_pSnHeadData->GetString_KessanKikan( &kessan_str );
	strcpy_s( m_nhtbl.mst_nam, sizeof(m_nhtbl.mst_nam), mst_nam_str+kessan_str );
//2018.08.21 UPDATE END
//---------------
	strcpy_s( m_nhtbl.gy_nam, sizeof(m_nhtbl.gy_nam), _T("消費税申告書") );
}

//-----------------------------------------------------------------------------
// 個人番号履歴情報セット（顧問先情報）
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetCoNoHistory()
{
	if( m_pMainFrame == NULL ){
		return;
	}

	TgPackData	*pTgData = m_pMainFrame->GetTgData();
//-- '16.01.30 --
//	if( pTgData->TgCode == 0 ){
//---------------
	if( pTgData->TgCode == -1 ){
//---------------
		return;
	}

	m_nhtbl.kmn_code = pTgData->TgCode;
	m_nhtbl.kmn_hkkbn = pTgData->HKkbn;
	memmove( m_nhtbl.kmn_nam, pTgData->TgName, sizeof(pTgData->TgName) );
}

//-----------------------------------------------------------------------------
// マイナンバーの最終インデックスを取得
//-----------------------------------------------------------------------------
// 返送値	-1以外	：	最終インデックス
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetMNLastIndex()
{
	if( m_svMynoInf.kubun == MN_HOJIN ){
		return 3;
	}
	else if( m_svMynoInf.kubun == MN_KOJIN ){
		return 2;
	}
	else{
		return -1;
	}
}

//-----------------------------------------------------------------------------
// マイナンバーのチェック
//-----------------------------------------------------------------------------
// 引数	isZeroOK	：	空を許可するかどうか
//		isShowMes	：	メッセージを表示するかどうか
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	有効
//			FALSE	：	無効
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::CheckMNumber( BOOL isZeroOK, int isShowMes )
{
	if( m_pMnec == NULL ){
		return FALSE;
	}

	CString number;
	int st = m_pMnec->GetData( number, isShowMes );	// GetDataでもマイナンバーの正当性チェックがかかる
	if( isZeroOK == TRUE ){
		if( number.IsEmpty() ){
			return TRUE;
		}
	}
	if( st < 0 ){
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// 入力ロック中？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	入力ロック中
//			FALSE	：	入力許可
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::InLockInput()
{
	if( m_pSnHeadData == NULL ){
		return TRUE;
	}
	else if( m_pSnHeadData->Sn_Sign4&0x01 ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 個人番号削除ボタンの表示／非表示を切り替える
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::ChangeDispKjdelBtn()
{
	// 上手くん環境なら非表示
	if( GET_EntrySign() != 0 ){
		m_KjdelBtn.ShowWindow( SW_HIDE );
		return;
	}

	if( m_svMynoInf.kubun == MN_HOJIN ){
		m_KjdelBtn.EnableWindow( FALSE );
		m_KjdelBtn.ShowWindow( SW_HIDE );
	}
	else if( m_svMynoInf.kubun == MN_KOJIN ){
		// 個人番号アクセス権限がなかったら非表示
		if( MynoAccessPerm() == FALSE ){
			m_KjdelBtn.EnableWindow( FALSE );
			m_KjdelBtn.ShowWindow( SW_HIDE );
			return;
		}
		// 履歴以外は絶えず削除不可
		int sn_seq=0;
		if( m_pMainFrame ){
			sn_seq = m_pMainFrame->GetSnSeq();
		}
		if( sn_seq <= 0 ){
			m_KjdelBtn.EnableWindow( FALSE );
			m_KjdelBtn.ShowWindow( SW_HIDE );
		}
		else{
			if( InLockInput() ){
				m_KjdelBtn.EnableWindow( FALSE );
			}
			else{
				m_KjdelBtn.EnableWindow( TRUE );
			}

//-- '15.08.24 --
//			CString	data;
//			m_pMnec->GetData( data );
//
//			if( data.GetLength() ){
//				m_KjdelBtn.ShowWindow( SW_SHOW );
//			}
//			else{
//				m_KjdelBtn.ShowWindow( SW_HIDE );
//			}
//---------------
			if( IsEmptyKjhash() ){
				m_KjdelBtn.ShowWindow( SW_HIDE );
			}
			else{
				m_KjdelBtn.ShowWindow( SW_SHOW );
			}
//---------------
		}
	}
}

//-----------------------------------------------------------------------------
// マイナンバー入力コントロールにデータをセットする
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetMNData()
{
	CString	data;
	GetData( m_svMynoInf.kubun, m_svMynoInf.sn_seq, data );

	if( m_pMnec ){
		m_pMnec->SetData( data );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 空の個人ハッシュ？
//-----------------------------------------------------------------------------
// 引数		phash	：	チェック対象
//			size	：	ハッシュのサイズ
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	空
//			FALSE	：	空でない
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsEmptyKjhash( char *phash, int size )
{
	BOOL	bRt = FALSE;

	// ハッシュ値が空なら帰る
	BOOL is_hush_empty = TRUE;
	for( int i=0; i<size; i++ ){
		int zero = 0;
		if( phash[i] != zero ){
			is_hush_empty = FALSE;
			break;
		}
	}
	if( is_hush_empty == TRUE ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 個人番号を全体から削除
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::DeleteKjhashFromNoDB()
{
	// データパケットセット
	NTBMOD8_InitNoDataTbl( 0, &m_ndtbl );
	SetKjhashToNdtbl();

	// 履歴パケットセット
	NTBMOD8_InitNoHistoryTbl( 0, &m_nhtbl );
	SetNoHistory();		// マスター情報
	SetCoNoHistory();	// 顧問先情報

	int st = NTBMOD8_DeleteNoDataTbl( 0, &m_ndtbl, &m_nhtbl );
	if( st < 0 ){
		CString err;
		NTBMOD8_GetErrorMessage( err );
		ICSMessageBox( err );
		return -1;
	}

	ClearKjhash( false );

	return 0;
}

//-----------------------------------------------------------------------------
// マイナンバーデータテーブルに個人番号ハッシュ値をセット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetKjhashToNdtbl()
{
	memset( m_ndtbl.arrow, '\0', sizeof(m_ndtbl.arrow) );
	
	CString	hash_all;
	for( int i=0; i<16; i++ ){
		BYTE hush = m_pSnHeadData->Sn_KOJIN_HASH[i];

		CString num;
		num.Format( _T("%02x"), hush );
		hash_all += num.Left(1) + num.Right(1);
	}
	memmove( m_ndtbl.arrow, hash_all, sizeof(m_ndtbl.arrow) );

	return 0;
}

//-----------------------------------------------------------------------------
// 個人番号ハッシュ値をクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ClearKjhash( bool isWriteHistory )
{
	memset( m_pSnHeadData->Sn_KOJIN_HASH, '\0', sizeof(m_pSnHeadData->Sn_KOJIN_HASH) );
	if( m_pMainFrame ){
		int	sn_seq = 0;
		sn_seq = m_pMainFrame->GetSnSeq();

		TgPackData	*pTgdata = NULL;
		if( m_pMainFrame ){
			pTgdata = m_pMainFrame->GetTgData();
		}
		unsigned char	kbFlg = 0x02;
		if( isWriteHistory ){
			kbFlg |= 0x80;
		}
		m_pSnHeadData->RegistMynumber( m_pZmSub, pTgdata, sn_seq, kbFlg );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 終了時に影響を受けないコントロールにフォーカスを移動
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::EndJobFocus()
{
	c_set( IDC_INPUT_BTN );
}

//-----------------------------------------------------------------------------
// 個人番号削除
//-----------------------------------------------------------------------------
// 返送値	RET_NO_CANCEL	：	削除キャンセル
//			RET_NODEL_H		：	ハッシュ値のみ削除
//			RET_NODEL_DB	：	データ全体から削除
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::DeleteKjno()
{
	int st = RET_NO_CANCEL;
	
	CICSMNSub8 mnsub;
	st = mnsub.NoMessageBox_Dsp( this, MEG_KIND_ONE_DEL );
	if( st == RET_NO_CANCEL ){		// キャンセル
		return st;
	}
	else if( st == RET_NODEL_H ){	// データ削除
		ClearKjhash( true );
//		SetMNData();
//		if( m_pMnec ){
//			m_pMnec->EnableWindow( TRUE );
//		}
	}
	else if( st == RET_NODEL_DB ){	// 全体から削除
		DeleteKjhashFromNoDB();
//		SetMNData();
//		if( m_pMnec ){
//			m_pMnec->EnableWindow( TRUE );
//		}
	}

	return st;
}

//-----------------------------------------------------------------------------
// 法人番号から情報を取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetHjInfoFromHjno()
{
	if( m_svMynoInf.kubun == MN_KOJIN ){
		return 0;
	}

	HJN_REC	hjnrec={0};
	CString	strErr, data;
	int		st = 0;
	if( (st=GetHjnInfo(0, hjnrec, strErr)) == 0 ){
		BOOL	isUpdate = FALSE;
		// 法人番号
		if( strlen(hjnrec.wHjnNo1) ){
			memset( m_pSnHeadData->Sn_HOJIN_NO, '\0', sizeof(m_pSnHeadData->Sn_HOJIN_NO) );
			wsprintf( m_pSnHeadData->Sn_HOJIN_NO, _T("%s"), hjnrec.wHjnNo1 );
			memset( m_pSnHeadData->Sn_HOJIN_NO_EXT, '\0', sizeof(m_pSnHeadData->Sn_HOJIN_NO_EXT) );

			CString	data;
			data.Format( _T("%s"), hjnrec.wHjnNo1 );
			if( m_pMnec ){
				m_pMnec->SetData( data );
			}
			isUpdate = TRUE;
		}

		// 会社名
		if( strlen((char*)hjnrec.wHjnName) ){
			memset( m_pSnHeadData->Sn_CONAM, '\0', sizeof(m_pSnHeadData->Sn_CONAM) );
			CString	data( hjnrec.wHjnName );
			if( data.GetLength() > 40 ){
				int kst = IsKindOfLetter( data.GetBuffer(), (40-1) );
				if( kst == 2 ){
					data = data.Left( 39 );
				}
				else{
					data = data.Left( 40 );
				}
			}
			wsprintf( (char*)m_pSnHeadData->Sn_CONAM, _T("%.40s"), data );
			isUpdate = TRUE;
		}
		// 住所(上段)
		if( strlen((char*)hjnrec.wAdrs1) || strlen((char*)hjnrec.wAdrs2) ){
			memset( m_pSnHeadData->Sn_NOZEI, '\0', 40 );
			data.Format( _T("%s%s"), hjnrec.wAdrs1, hjnrec.wAdrs2 );
			if( data.GetLength() > 40 ){
				int kst = IsKindOfLetter( data.GetBuffer(), (40-1) );
				if( kst == 2 ){
					data = data.Left( 39 );
				}
				else{
					data = data.Left( 40 );
				}
			}
						
			wsprintf( (char*)m_pSnHeadData->Sn_NOZEI, _T("%.40s"), data );
			isUpdate = TRUE;
		}
		// 住所(下段)
		if( strlen((char*)hjnrec.wAdrs3) ){
			memset( &m_pSnHeadData->Sn_NOZEI[40], '\0', 40 );
			data.Format( _T("%s"), hjnrec.wAdrs3 );
			if( data.GetLength() > 40 ){
				int kst = IsKindOfLetter( data.GetBuffer(), (40-1) );
				if( kst == 2 ){
					data = data.Left( 39 );
				}
				else{
					data = data.Left( 40 );
				}
			}
			wsprintf( (char*)&m_pSnHeadData->Sn_NOZEI[40], _T("%.40s"), data );
			isUpdate = TRUE;
		}
		// 郵便番号
		if( strlen((char*)hjnrec.wPostNo) ){
			char	buf1[64]={0}, buf2[64]={0};
			memmove( buf1, hjnrec.wPostNo, 3 );
			memmove( buf2, &hjnrec.wPostNo[3], 4 );
			data.Format( _T("%s-%s"), buf1, buf2 );
			wsprintf( (char*)&m_pSnHeadData->Sn_ZIP, _T("%s"), data );
			isUpdate = TRUE;
		}
		if( isUpdate ){
			int sn_seq=0;
			if( m_pMainFrame ){
				sn_seq = m_pMainFrame->GetSnSeq();
				m_pSnHeadData->SetData( m_pZmSub, sn_seq );
			}
		}
	}
	else if( st == -1 ){
		ICSMessageBox( strErr );
	}
	return 0;
}


