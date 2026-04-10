// midori 20/10/08u add -->
// ダイアログ”dlgPrintConfirmation3”を削除
// 画面からIDC_PRINT_RADIO2を削除
// 高さを調整
// midori 20/10/08u add <--

// midori 190505 add -->
// CdlgPrintConfirmation.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgPrintConfirmation.h"

// midori 20/10/08u del -->
//extern int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// midori 20/10/08u del <--

// CdlgPrintConfirmation ダイアログ

IMPLEMENT_DYNAMIC(CdlgPrintConfirmation, ICSDialog)

CdlgPrintConfirmation::CdlgPrintConfirmation(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgPrintConfirmation::IDD, pParent)
{

}

CdlgPrintConfirmation::~CdlgPrintConfirmation()
{
}

void CdlgPrintConfirmation::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRINT_RADIO1, m_PrtrdSHoho1);
	DDX_Control(pDX, IDC_PRINT_RADIO2, m_PrtrdSHoho2);
// midori 20/10/08u del -->
	//DDX_Control(pDX, IDC_PRINT_RADIO3, m_PrtrdSHoho3);
// midori 20/10/08u del <--
	DDX_Control(pDX, ID_PRINT_OK_BUTTON, m_PrtbtnOk);
	DDX_Control(pDX, ID_PRINT_CANCEL_BUTTON, m_PrtbtnCan);
	DDX_Control(pDX, IDC_CAUTION_PICTURE, m_Prticon);
}

BEGIN_MESSAGE_MAP(CdlgPrintConfirmation, ICSDialog)
	ON_BN_CLICKED(ID_PRINT_OK_BUTTON, &CdlgPrintConfirmation::OnBnClickedPrintOkButton)
	ON_BN_CLICKED(ID_PRINT_CANCEL_BUTTON, &CdlgPrintConfirmation::OnBnClickedPrintCancelButton)
END_MESSAGE_MAP()

// CdlgPrintConfirmation メッセージ ハンドラー

// --------------------------------------------------------------------------------------------------------------------
//	ダイアログ表示
// --------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				pFormSeq		…	FormSeq番号
//	【戻値】	ID_DLG_OK		…	はいボタンが押された
//				ID_DLG_CANCEL	…	いいえボタンが押された
// --------------------------------------------------------------------------------------------------------------------
int CdlgPrintConfirmation::ShowDialog(CDatabase* pDB, int pFormSeq)
{
	m_nRet = ID_DLG_CANCEL;		// 戻値初期化

	if(pDB != NULL)	{
		try	{
			if(pDB->IsOpen())	{
				m_pDB = pDB;					// データベースハンドル取得
				m_nFormSeq = pFormSeq;			// FormSeq番号
				CreateDoModal();
				DoModal();						// モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return(m_nRet);
}

// --------------------------------------------------------------------------------------------------------------------
//	改頁処理を行うサインを取得する
// --------------------------------------------------------------------------------------------------------------------
void CdlgPrintConfirmation::CreateDoModal(void)
{
	CdbUcInfSub			mfcRec(m_pDB);		//	uc_inf_subテーブルクラス
	CdbUcLstItemSort	mfcRec2(m_pDB);		//	uc_lst_item_sortテーブルクラス
	int					cnt=0;
	int					idx=0;
	int					sort=0;
	int					sortmax=0;
// midori 156909 add -->
	int					flg=0;
// midori 156909 add <--
// No.200601 del -->
	//BYTE				fgPage[9];
	//BYTE				ItemDt[9];
// No.200601 del <--

	m_SHoho = 0;
	m_EditSHoho = 0;
// midori 156909 add -->
	flg = 0;
// midori 156909 add <--

	if(mfcRec.RequeryFormSeq(m_nFormSeq) == DB_ERR_OK)	{
		if( !(mfcRec.IsEOF()) )	{
// midori 20/10/08u del -->
//			if(mfcRec.m_Sort1 == 0)	{
//				// 一度も編集～ソートを開いていない場合は、第１ソート項目の項目番号が書かれていない
//				if(m_nFormSeq == 2)	mfcRec.m_Sort1=8;
//				else				mfcRec.m_Sort1=1;
//			}
//			if(mfcRec2.RequeryFormItem(m_nFormSeq,mfcRec.m_Sort1) == DB_ERR_OK)	{
//				if(!mfcRec2.IsEOF())	{
//// midori 156909 del -->
//					//m_EditSHoho = mfcRec2.m_FgPage;			// 1:改頁対象項目
//// midori 156909 del <--
//// midori 156909 add -->
//					flg = mfcRec2.m_FgPage;			// 1:改頁対象項目
//// midori 156909 add <--
//				}
//			}
//			mfcRec2.Fin();
//
//// midori 156909 add -->
//			if(flg != 0)	{
//				m_EditSHoho = mfcRec.m_OpKaiPage;
//			}
//// midori 156909 add <--
//
//			CCtrlSubGeneralVar	cs(mfcRec);
//			// 「出力方法」
//			if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE && cs.bPrnHoho3 == FALSE)	{
//				// 新規起動時
//				// ソートダイアログの「改頁を行う」チェックがONの時
//				if(mfcRec.m_OpKaiPage == 1)	{
//					m_SHoho = 2;	// 「編集－ソート設定の第１ソート項目でソート改頁して出力」をデフォルト
//				}
//				else	{
//					m_SHoho = 0;	// 「改頁･空行などの編集を詰めて出力」をデフォルト
//				}
//			}
//			else	{
//				if(cs.bPrnHoho1)	{
//					m_SHoho = 0;	// 改頁･空行などの編集を詰めて出力
//				}
//				else if(cs.bPrnHoho2)	{
//					m_SHoho = 1;	// 改頁･空行などの編集を保持して出力
//				}
//				else if(cs.bPrnHoho3)	{
//					m_SHoho = 2;	// 編集－ソート設定の第１ソート項目でソート改頁して出力
//				}
//			}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
			CCtrlSubGeneralVar	cs(mfcRec);
			// 「出力方法」
			if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE)	{
				// 新規起動時
				m_SHoho = 1;	// 「編集－ソート設定の第１ソート項目でソート改頁して出力」をデフォルト
			}
			else	{
				if(cs.bPrnHoho1)	{
					m_SHoho = 0;	// 改頁･空行などの編集を詰めて出力
				}
				else if(cs.bPrnHoho2)	{
					m_SHoho = 1;	// 改頁･空行などの編集を保持して出力
				}
			}
// midori 20/10/08u add <--
		}
		mfcRec.Fin();
	}
}

// --------------------------------------------------------------------------------------------------------------------
//	ダイアログ起動時のイベント
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgPrintConfirmation::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// リサイズ
	OnInitDialogEX();

	// アイコンを「？」に変更する
	HICON	hIcon = LoadIcon(NULL,IDI_EXCLAMATION);
	m_Prticon.SetIcon(hIcon);

	// 画面設定
	InitDisp();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// --------------------------------------------------------------------------------------------------------------------
//	画面の設定
// --------------------------------------------------------------------------------------------------------------------
int CdlgPrintConfirmation::InitDisp(void)
{
	// 「出力方法」の選択をセット
// midori 20/10/08u del -->
	//if(m_SHoho == 0)	{
	//	m_PrtrdSHoho1.SetCheck(1);
	//	m_PrtrdSHoho2.SetCheck(0);
	//	m_PrtrdSHoho3.SetCheck(0);
	//}
	//else if(m_SHoho == 1)	{
	//	m_PrtrdSHoho1.SetCheck(0);
	//	m_PrtrdSHoho2.SetCheck(1);
	//	m_PrtrdSHoho3.SetCheck(0);
	//}
	//else	{
	//	m_PrtrdSHoho1.SetCheck(0);
	//	m_PrtrdSHoho2.SetCheck(0);
	//	m_PrtrdSHoho3.SetCheck(1);
	//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
	if(m_SHoho == 0)	{
		m_PrtrdSHoho1.SetCheck(1);
		m_PrtrdSHoho2.SetCheck(0);
	}
	else {
		m_PrtrdSHoho1.SetCheck(0);
		m_PrtrdSHoho2.SetCheck(1);
	}
// midori 20/10/08u add <--

	// 「出力方法」の画面制御
	m_PrtrdSHoho1.EnableWindow(TRUE);
	m_PrtrdSHoho2.EnableWindow(TRUE);
// midori 20/10/08u del -->
	//if(m_EditSHoho == 0)	{
	//	m_PrtrdSHoho3.EnableWindow(FALSE);
	//}
	//else	{
	//	m_PrtrdSHoho3.EnableWindow(TRUE);
	//}
// midori 20/10/08u del <--

	// フォーカスセット
	if(m_SHoho == 0)		c_set(IDC_PRINT_RADIO1);
// midori 20/10/08u del -->
	//else if(m_SHoho == 1)	c_set(IDC_PRINT_RADIO2);
	//else					c_set(IDC_PRINT_RADIO3);
// midori 20/10/08u del <--
// midori 20/10/08u add -->
	else					c_set(IDC_PRINT_RADIO2);
// midori 20/10/08u add <--

	return(0);
}

// --------------------------------------------------------------------------------------------------------------------
//	メッセージ処理
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgPrintConfirmation::PreTranslateMessage(MSG* pMsg)
{
	CWnd*	pControl = this->GetFocus();

	// キーが押された？
	if(pMsg->message == WM_KEYDOWN)	{
		//	パラメータで分岐
		switch(pMsg->wParam)	{
			case VK_F2:			//	F2キーが押された
				//	F2キーをShift + Tabイベントに書き換え
				keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
				return(TRUE);									//	抜ける
				break;

			case VK_TAB:
				if(pControl != NULL)	{
					if(GetKeyState(VK_SHIFT) & 0x8000)	{
						((CDialog *)this)->PrevDlgCtrl();
					}
					else	{
						((CDialog *)this)->NextDlgCtrl();
					}
					return(TRUE);
				}
				break;

			case VK_RETURN:
				if(pControl != NULL)	{
					if(pControl->GetDlgCtrlID() == ID_PRINT_OK_BUTTON)	{
						OnBnClickedPrintOkButton();
					}
					else if(pControl->GetDlgCtrlID() == ID_PRINT_CANCEL_BUTTON)	{
						OnBnClickedPrintCancelButton();
					}
					else	{
						((CDialog *)this)->NextDlgCtrl();
					}
					return(TRUE);
				}
				break;

			case VK_ESCAPE:
				OnBnClickedPrintCancelButton();
				return(TRUE);
				break;

			case VK_F5:
				OnBnClickedPrintOkButton();
				return(TRUE);
				break;

			default	:
				break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------------------------------------------------
//	ボタンクリック（OKボタン）
// --------------------------------------------------------------------------------------------------------------------
void CdlgPrintConfirmation::OnBnClickedPrintOkButton()
{
	CdbUcInfSub		mfcRec(m_pDB);		//	uc_inf_subテーブルクラス

	m_nRet = ID_DLG_OK;

	// 「出力方法」
// midori 20/10/08u del -->
	//if(m_PrtrdSHoho1.GetCheck() == 1)	{
	//	m_SHoho = 0;
	//}
	//else if(m_PrtrdSHoho2.GetCheck() == 1)	{
	//	m_SHoho = 1;
	//}
	//else	{
	//	m_SHoho = 2;
	//}
// midori 20/10/08u del <--
// midori 20/10/08u del -->
	if(m_PrtrdSHoho1.GetCheck() == 1)	{
		m_SHoho = 0;
	}
	else {
		m_SHoho = 1;
	}
// midori 20/10/08u del <--

	//---------------------------------------------------
	// チェックの内容を保存
	if(mfcRec.RequeryFormSeq(m_nFormSeq) == DB_ERR_OK)	{
		if( !(mfcRec.IsEOF()) )	{
			CCtrlSubGeneralVar	cs(mfcRec);

			mfcRec.Edit();

			// 「出力方法」
// midori 20/10/08u del -->
			//if(m_SHoho == 0)	{
			//	// 改頁･空行などの編集を詰めて出力
			//	cs.bPrnHoho1 = TRUE;
			//	cs.bPrnHoho2 = FALSE;
			//	cs.bPrnHoho3 = FALSE;
			//}
			//else if(m_SHoho == 1)	{
			//	// 改頁･空行などの編集を保持して出力
			//	cs.bPrnHoho1 = FALSE;
			//	cs.bPrnHoho2 = TRUE;
			//	cs.bPrnHoho3 = FALSE;
			//}
			//else if(m_SHoho == 2)	{
			//	// 編集－ソート設定の第１ソート項目でソート改頁して出力
			//	cs.bPrnHoho1 = FALSE;
			//	cs.bPrnHoho2 = FALSE;
			//	cs.bPrnHoho3 = TRUE;
			//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
			if(m_SHoho == 0)	{
				// 改頁･空行などの編集を詰めて出力
				cs.bPrnHoho1 = TRUE;
				cs.bPrnHoho2 = FALSE;
			}
			else if(m_SHoho == 1)	{
				// 編集－ソート設定の第１ソート項目でソート改頁して出力
				cs.bPrnHoho1 = FALSE;
				cs.bPrnHoho2 = TRUE;
			}
// midori 20/10/08u add <--

			cs.Set(mfcRec);
			mfcRec.Update();
		}
		mfcRec.Fin();
	}
	//---------------------------------------------------

	ICSDialog::OnOK();
}

// --------------------------------------------------------------------------------------------------------------------
//	ボタンクリック（キャンセルボタン）
// --------------------------------------------------------------------------------------------------------------------
void CdlgPrintConfirmation::OnBnClickedPrintCancelButton()
{
	m_nRet = ID_DLG_CANCEL;

	ICSDialog::OnCancel();
}
// midori 190505 add <--
