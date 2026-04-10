// midori 156950 add -->
// dlgPrintConfirmation4.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgPrintConfirmation4.h"
#include "frmUc000Common.h"

// CdlgPrintConfirmation4 ダイアログ

IMPLEMENT_DYNAMIC(CdlgPrintConfirmation4, ICSDialog)

CdlgPrintConfirmation4::CdlgPrintConfirmation4(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgPrintConfirmation4::IDD, pParent)
{
	m_pOwner = pParent;
	m_nFormSeq[1] = 0;
}

CdlgPrintConfirmation4::~CdlgPrintConfirmation4()
{
}

void CdlgPrintConfirmation4::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRINT4_RADIO1, m_Prt4Radio1);
	DDX_Control(pDX, IDC_PRINT4_RADIO2, m_Prt4Radio2);
	DDX_Control(pDX, IDC_PRINT4_RADIO3, m_Prt4Radio3);
	DDX_Control(pDX, IDC_PRINT4_RADIO4, m_Prt4Radio4);
	DDX_Control(pDX, ID_PRINT4_OK_BUTTON, m_Prt4btnOk);
	DDX_Control(pDX, ID_PRINT4_CANCEL_BUTTON, m_Prt4btnCan);
	DDX_Control(pDX, IDC_PRINT4_CAUTION_PICTURE, m_Prt4icon);
	DDX_Control(pDX, IDC_PRINT4_MSG_STATIC3, m_Prt4Static3);
	DDX_Control(pDX, IDC_PRINT4_MSG_STATIC4, m_Prt4Static4);
}

BEGIN_MESSAGE_MAP(CdlgPrintConfirmation4, ICSDialog)
	ON_BN_CLICKED(ID_PRINT4_OK_BUTTON, &CdlgPrintConfirmation4::OnBnClickedPrintOkButton)
	ON_BN_CLICKED(ID_PRINT4_CANCEL_BUTTON, &CdlgPrintConfirmation4::OnBnClickedPrintCancelButton)
END_MESSAGE_MAP()

// CdlgPrintConfirmation4 メッセージ ハンドラー

// --------------------------------------------------------------------------------------------------------------------
//	ダイアログ表示
// --------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				pFormSeq		…	FormSeq番号
//	【戻値】	ID_DLG_OK		…	はいボタンが押された
//				ID_DLG_CANCEL	…	いいえボタンが押された
// --------------------------------------------------------------------------------------------------------------------
int CdlgPrintConfirmation4::ShowDialog(CDatabase* pDB, int pFormSeq)
{
	m_nRet = ID_DLG_CANCEL;		// 戻値初期化

	if(pDB != NULL)	{
		try	{
			if(pDB->IsOpen())	{
				m_pDB			= pDB;		// データベースハンドル取得
				m_nFormSeq[0]	= pFormSeq;	// FormSeq番号

				DoModal();				// モーダルで表示
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
BOOL CdlgPrintConfirmation4::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// リサイズ
	OnInitDialogEX();

	// アイコンを「？」に変更する
	HICON	hIcon = LoadIcon(NULL,IDI_EXCLAMATION);
	m_Prt4icon.SetIcon(hIcon);

	// 画面設定
	InitDisp();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// --------------------------------------------------------------------------------------------------------------------
//	画面の設定
// --------------------------------------------------------------------------------------------------------------------
int CdlgPrintConfirmation4::InitDisp(void)
{
	int					editSHoho[2]={0};
	int					ii=0;
	int					max=0;
	int					len=0;
	int					flg=0;
	int					flg1=0,flg2=0,flg3=0;
	CString				mm[2]={_T("")};
	CString				title=_T(""),title2=_T("");
	CdbUcInfSub			mfcRec(m_pDB);
	CdbUcLstItemSort	mfcRec2(m_pDB);		//	uc_lst_item_sortテーブルクラス
	CUcFunctionCommon	ufc;

	m_nFormSeq[0] = ID_FORMNO_161;
	m_nFormSeq[1] = ID_FORMNO_162;
	// タイトルを取得
	for(ii=0;ii<2;ii++) {
		mm[ii] = _T("");
		if(mfcRec.RequeryFormSeq(m_nFormSeq[ii]) == DB_ERR_OK)	{
			if(!(mfcRec.IsEOF()))	{
				title = mfcRec.m_Title;
				ufc.GetSpritString(title,&title2,70);
				if(title != title2)	{
					title = title2 + _T("... ");
				}
				mm[ii] += mfcRec.m_TitleNo + mfcRec.m_TitleNo2 + title;
			}
			mfcRec.Fin();
		}
	}

	m_Prt4Static3.SetWindowText(mm[0]);
	m_Prt4Static4.SetWindowText(mm[1]);

	editSHoho[0] = 0;
	editSHoho[1] = 0;
	// 出力方法をデータベースから取得
	for(ii=0;ii<2;ii++) {
		// 編集－ソート設定の改頁を行うが有効の時のみ
		// 出力方法「編集－ソート設定の第１ソート項目でソート改頁して出力」を選択可能にする
		// ii = 0	１様式目
		// ii = 1	２様式目
		if(mfcRec.RequeryFormSeq(m_nFormSeq[ii]) == DB_ERR_OK)	{
			if( !(mfcRec.IsEOF()) )	{
				CCtrlSubGeneralVar	cs(mfcRec);
				// 「出力方法」
				if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE )	{
					// 新規起動時
					m_SHoho[ii] = 1;		// 「編集－ソート設定の第１ソート項目でソート改頁して出力」をデフォルト
				}
				else	{
					if(cs.bPrnHoho1)	{
						m_SHoho[ii] = 0;		// 改頁･空行などの編集を詰めて出力
					}
					else if(cs.bPrnHoho2)	{
						m_SHoho[ii] = 1;		// 編集－ソート設定の第１ソート項目でソート改頁して出力
					}
				}
			}
			mfcRec.Fin();
		}
	}
	// 「出力方法」の選択をセット
	// １様式目
	if(m_SHoho[0] == 0)	{
		m_Prt4Radio1.SetCheck(1);
		m_Prt4Radio2.SetCheck(0);
	}
	else {
		m_Prt4Radio1.SetCheck(0);
		m_Prt4Radio2.SetCheck(1);
	}
	// ２様式目
	if(m_SHoho[1] == 0)	{
		m_Prt4Radio3.SetCheck(1);
		m_Prt4Radio4.SetCheck(0);
	}
	else {
		m_Prt4Radio3.SetCheck(0);
		m_Prt4Radio4.SetCheck(1);
	}

	// フォーカスをセット
	if(m_SHoho == 0)	c_set(IDC_PRINT4_RADIO1);
	else				c_set(IDC_PRINT4_RADIO2);

	return(0);
}

// --------------------------------------------------------------------------------------------------------------------
//	メッセージ処理
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgPrintConfirmation4::PreTranslateMessage(MSG* pMsg)
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
					if(pControl->GetDlgCtrlID() == ID_PRINT4_OK_BUTTON)	{
						OnBnClickedPrintOkButton();
					}
					else if(pControl->GetDlgCtrlID() == ID_PRINT4_CANCEL_BUTTON)	{
						OnBnClickedPrintCancelButton();
					}
					else {
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
void CdlgPrintConfirmation4::OnBnClickedPrintOkButton()
{
	CdbUcInfSub		mfcRec(m_pDB);		//	uc_inf_subテーブルクラス
	int				ii=0;

	m_nRet = ID_DLG_OK;

	// 「出力方法」
	// １様式目
	if(m_Prt4Radio1.GetCheck() == 1)			m_SHoho[0] = 0;
	else if(m_Prt4Radio2.GetCheck() == 1)		m_SHoho[0] = 1;
	// ２様式目
	if(m_Prt4Radio3.GetCheck() == 1)			m_SHoho[1] = 0;
	else if(m_Prt4Radio4.GetCheck() == 1)		m_SHoho[1] = 1;

	//---------------------------------------------------
	for(ii=0;ii<2;ii++) {
		// チェックの内容を保存
		if(mfcRec.RequeryFormSeq(m_nFormSeq[ii]) == DB_ERR_OK)	{
			if( !(mfcRec.IsEOF()) )	{
				CCtrlSubGeneralVar	cs(mfcRec);

				mfcRec.Edit();

				// 「出力方法」
				if(m_SHoho[ii] == 0)	{
					// 改頁･空行などの編集を詰めて出力
					cs.bPrnHoho1 = TRUE;
					cs.bPrnHoho2 = FALSE;
				}
				else if(m_SHoho[ii] == 1)	{
					// 編集－ソート設定の第１ソート項目でソート改頁して出力
					cs.bPrnHoho1 = FALSE;
					cs.bPrnHoho2 = TRUE;
				}

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
void CdlgPrintConfirmation4::OnBnClickedPrintCancelButton()
{
	m_nRet = ID_DLG_CANCEL;

	ICSDialog::OnCancel();
}
// midori 156950 add <--
