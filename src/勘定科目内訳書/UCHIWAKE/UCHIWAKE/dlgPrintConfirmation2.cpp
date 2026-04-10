// midori 190505 add -->
// dlgPrintConfirmation2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgPrintConfirmation2.h"
#include "frmUc000Common.h"

// midori 20/10/08u del -->
//extern int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// midori 20/10/08u del <--

// CdlgPrintConfirmation2 ダイアログ

IMPLEMENT_DYNAMIC(CdlgPrintConfirmation2, ICSDialog)

CdlgPrintConfirmation2::CdlgPrintConfirmation2(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgPrintConfirmation2::IDD, pParent)
{
	m_pOwner = pParent;
	m_FormSeqA[0] = 0;
	m_FormSeqA[1] = 0;
}

CdlgPrintConfirmation2::~CdlgPrintConfirmation2()
{
}

void CdlgPrintConfirmation2::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_PRINT_MSG_STATIC4, m_txtStatic4);
}

BEGIN_MESSAGE_MAP(CdlgPrintConfirmation2, ICSDialog)
	ON_BN_CLICKED(ID_PRINT_OK_BUTTON, &CdlgPrintConfirmation2::OnBnClickedPrintOkButton)
	ON_BN_CLICKED(ID_PRINT_CANCEL_BUTTON, &CdlgPrintConfirmation2::OnBnClickedPrintCancelButton)
END_MESSAGE_MAP()

// CdlgPrintConfirmation2 メッセージ ハンドラー

// --------------------------------------------------------------------------------------------------------------------
//	ダイアログ表示
// --------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				pFormSeq		…	FormSeq番号
//	【戻値】	ID_DLG_OK		…	はいボタンが押された
//				ID_DLG_CANCEL	…	いいえボタンが押された
// --------------------------------------------------------------------------------------------------------------------
int CdlgPrintConfirmation2::ShowDialog(CDatabase* pDB, int pFormSeq)
{
	m_nRet = ID_DLG_CANCEL;		// 戻値初期化

	if(pDB != NULL)	{
		try	{
			if(pDB->IsOpen())	{
				m_pDB		= pDB;				// データベースハンドル取得
				m_nFormSeq	= pFormSeq;			// FormSeq番号

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
//	ダイアログ起動時のイベント
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgPrintConfirmation2::OnInitDialog()
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
int CdlgPrintConfirmation2::InitDisp(void)
{
// midori 20/10/08u del -->
	//int					editSHoho=0;
// midori 20/10/08u del <--
	int					yno=0,max=0,ii=0,len=0;
	int					flg1=0,flg2=0,flg3=0;
	CString				mm=_T(""),msg=_T("");
	CString				title=_T(""),title2=_T(""),title3=_T("");
	CdbUcInfSub			mfcRec(m_pDB);
	CdbUcLstItemSort	mfcRec2(m_pDB);		//	uc_lst_item_sortテーブルクラス
	CUcFunctionCommon	ufc;

	m_FormSeqA[0] = 0;
	m_FormSeqA[1] = 0;

// midori 20/10/08u del -->
	//// 左上にガイドを表示する
	//yno = 0;
	//switch(m_nFormSeq)	{
	//	case ID_FORMNO_041:
	//	case ID_FORMNO_042:
	//	case ID_FORMNO_101:
	//	case ID_FORMNO_102:
	//	case ID_FORMNO_142:
	//	case ID_FORMNO_161:
	//	case ID_FORMNO_162:
	//		max = 1;
	//		break;
	//	case ID_FORMNO_151:
	//		flg2 = ((CfrmUc000Common *)m_pOwner)->SortPrintMsgSub(ID_FORMNO_152);
	//		flg3 = ((CfrmUc000Common *)m_pOwner)->SortPrintMsgSub(ID_FORMNO_153);
	//		if(flg2 != 0)	max++;
	//		if(flg3 != 0)	max++;
	//		break;
	//	case ID_FORMNO_152:
	//		flg1 = ((CfrmUc000Common *)m_pOwner)->SortPrintMsgSub(ID_FORMNO_151);
	//		flg3 = ((CfrmUc000Common *)m_pOwner)->SortPrintMsgSub(ID_FORMNO_153);
	//		if(flg1 != 0)	max++;
	//		if(flg3 != 0)	max++;
	//		break;
	//	case ID_FORMNO_153:
	//		flg1 = ((CfrmUc000Common *)m_pOwner)->SortPrintMsgSub(ID_FORMNO_151);
	//		flg2 = ((CfrmUc000Common *)m_pOwner)->SortPrintMsgSub(ID_FORMNO_152);
	//		if(flg1 != 0)	max++;
	//		if(flg2 != 0)	max++;
	//		break;
	//	default:
	//		break;
	//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
	max = 1;
// midori 20/10/08u add <--
	mm = _T("");
	msg = _T("");
	for(ii=0; ii<max; ii++)	{
		if(m_nFormSeq == ID_FORMNO_041)			yno = ID_FORMNO_042;
		else if(m_nFormSeq == ID_FORMNO_042)	yno = ID_FORMNO_041;
		else if(m_nFormSeq == ID_FORMNO_101)	yno = ID_FORMNO_102;
		else if(m_nFormSeq == ID_FORMNO_102)	yno = ID_FORMNO_101;
		else if(m_nFormSeq == ID_FORMNO_142)	yno = ID_FORMNO_141;
		else if(m_nFormSeq == ID_FORMNO_161)	yno = ID_FORMNO_162;
		else if(m_nFormSeq == ID_FORMNO_162)	yno = ID_FORMNO_161;
// midori 20/10/08u del -->
		//else if(m_nFormSeq == ID_FORMNO_151)	{
		//	if(ii == 0 && flg2 != 0)	yno = ID_FORMNO_152;
		//	else						yno = ID_FORMNO_153;
		//}
		//else if(m_nFormSeq == ID_FORMNO_152)	{
		//	if(ii == 0 && flg1 != 0)	yno = ID_FORMNO_151;
		//	else						yno = ID_FORMNO_153;
		//}
		//else if(m_nFormSeq == ID_FORMNO_153)	{
		//	if(ii == 0 && flg1 != 0)	yno = ID_FORMNO_151;
		//	else						yno = ID_FORMNO_152;
		//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		else if(m_nFormSeq == ID_FORMNO_152)	{
			yno = ID_FORMNO_151;
		}
		else if(m_nFormSeq == ID_FORMNO_153)	{
			yno = ID_FORMNO_151;
		}
// midori 20/10/08u add <--
		if(mfcRec.RequeryFormSeq(yno) == DB_ERR_OK)	{
			if(!(mfcRec.IsEOF()))	{
				title = mfcRec.m_Title;
// midori 20/10/08u del -->
				//if(max == 1)	{
				//	len = 70;
				//}
				//else	{
				//	if(yno == ID_FORMNO_151)			len = 18;
				//	else if(yno == ID_FORMNO_152)		len = 26;
				//	else if(yno == ID_FORMNO_153)		len = 28;
				//	else								len = 70;
				//}
				//ufc.GetSpritString(title,&title2,len);
// midori 20/10/08u del <--
// midori 20/10/08u add -->
				ufc.GetSpritString(title,&title2,70);
// midori 20/10/08u add <--
				if(title != title2)	{
					title = title2 + _T("... ");
				}
				if(mm.IsEmpty() == FALSE)	mm += _T("、");
				mm += mfcRec.m_TitleNo + mfcRec.m_TitleNo2 + title;
			}
			mfcRec.Fin();
		}
		m_FormSeqA[ii] = yno;
	}
	msg  = mm + _T("は");

	m_txtStatic4.SetWindowTextA(msg);

// midori 20/10/08u del -->
//	// 編集－ソート設定の改頁を行うが有効の時のみ
//	// 出力方法「編集－ソート設定の第１ソート項目でソート改頁して出力」を選択可能にする
//	// 改頁対象外の様式が１件でもある場合、無効にする
//	editSHoho = 1;
//	for(ii=0;ii<max;ii++) {
//		if(mfcRec.RequeryFormSeq(m_FormSeqA[ii]) != DB_ERR_OK)	{
//			editSHoho = 0;
//			break;
//		}
//		if( mfcRec.IsEOF() )	{
//			editSHoho = 0;
//			break;
//		}
//		if(mfcRec.m_Sort1 == 0)	{
//			// 一度も編集～ソートを開いていない場合は、第１ソート項目の項目番号が書かれていない
//			if(m_FormSeqA[ii] == 2)		mfcRec.m_Sort1=8;
//			else						mfcRec.m_Sort1=1;
//		}
//		if(mfcRec2.RequeryFormItem(m_FormSeqA[ii],mfcRec.m_Sort1) != DB_ERR_OK)	{
//			editSHoho = 0;
//			break;
//		}
//		// ソート設定の登録の無い（ソート設定の行わない様式の場合は改頁対象外）
//		if(mfcRec2.IsEOF())	{
//			editSHoho = 0;
//			break;
//		}
//		// 0:改頁対象外
//		if(mfcRec2.m_FgPage == 0) {
//			editSHoho = 0;
//			break;
//		}
//// midori 156909 add -->
//		// 「改頁を行う」にチェックが付いていない
//		if(mfcRec.m_OpKaiPage == 0)	{
//			editSHoho = 0;
//			break;
//		}
//// midori 156909 add <--
//
//		mfcRec2.Fin();
//		mfcRec.Fin();
//	}
// midori 20/10/08u del <--

	// 「出力方法」の画面制御
	m_PrtrdSHoho1.EnableWindow(TRUE);
	m_PrtrdSHoho2.EnableWindow(TRUE);
// midori 20/10/08u del -->
	//if(editSHoho == 0)	{
	//	m_PrtrdSHoho3.EnableWindow(FALSE);
	//}
	//else	{
	//	m_PrtrdSHoho3.EnableWindow(TRUE);
	//}
// midori 20/10/08u del <--

	// 「出力方法」の選択を取得
	if(mfcRec.RequeryFormSeq(m_FormSeqA[0]) == DB_ERR_OK)	{
		if( !(mfcRec.IsEOF()) )	{
			CCtrlSubGeneralVar	cs(mfcRec);
// midori 20/10/08u del -->
			//// 「出力方法」
			//if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE && cs.bPrnHoho3 == FALSE)	{
			//	// 新規起動時
			//	// ソートダイアログの「改頁を行う」チェックがONの時
			//	if(mfcRec.m_OpKaiPage == 1)	{
			//		m_SHoho = 2;	// 「編集－ソート設定の第１ソート項目でソート改頁して出力」をデフォルト
			//	}
			//	else	{
			//		m_SHoho = 0;	// 「改頁･空行などの編集を詰めて出力」をデフォルト
			//	}
			//}
			//else	{
			//	if(cs.bPrnHoho1)	{
			//		m_SHoho = 0;	// 改頁･空行などの編集を詰めて出力
			//	}
			//	else if(cs.bPrnHoho2)	{
			//		m_SHoho = 1;	// 改頁･空行などの編集を保持して出力
			//	}
			//	else if(cs.bPrnHoho3)	{
			//		m_SHoho = 2;	// 編集－ソート設定の第１ソート項目でソート改頁して出力
			//	}
			//}
			//if(editSHoho == 0 && m_SHoho == 2)	m_SHoho = 0;
// midori 20/10/08u del <--
// midori 20/10/08u add -->
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

	// フォーカスセット
	if(m_SHoho == 0)		c_set(IDC_PRINT_RADIO1);
	else if(m_SHoho == 1)	c_set(IDC_PRINT_RADIO2);
// midori 20/10/08u del -->
	//else					c_set(IDC_PRINT_RADIO3);
// midori 20/10/08u del <--

	return(0);
}

// --------------------------------------------------------------------------------------------------------------------
//	メッセージ処理
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgPrintConfirmation2::PreTranslateMessage(MSG* pMsg)
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
void CdlgPrintConfirmation2::OnBnClickedPrintOkButton()
{
	int				ii=0;
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
// midori 20/10/08u add -->
	if(m_PrtrdSHoho1.GetCheck() == 1)	{
		m_SHoho = 0;
	}
	else {
		m_SHoho = 1;
	}
// midori 20/10/08u add <--

	//---------------------------------------------------
	for(ii=0;ii<2;ii++)	{
		if(m_FormSeqA[ii] == 0)	continue;
		// チェックの内容を保存
		if(mfcRec.RequeryFormSeq(m_FormSeqA[ii]) == DB_ERR_OK)	{
			if( !(mfcRec.IsEOF()) )	{
				CCtrlSubGeneralVar	cs(mfcRec);

				mfcRec.Edit();

// midori 20/10/08u del -->
				//// 「出力方法」
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
				// 「出力方法」
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
	}
	//---------------------------------------------------

	ICSDialog::OnOK();
}

// --------------------------------------------------------------------------------------------------------------------
//	ボタンクリック（キャンセルボタン）
// --------------------------------------------------------------------------------------------------------------------
void CdlgPrintConfirmation2::OnBnClickedPrintCancelButton()
{
	m_nRet = ID_DLG_CANCEL;

	ICSDialog::OnCancel();
}
// midori 190505 add <--
