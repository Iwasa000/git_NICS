// CChkMsg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ChkMsg.h"
#include "afxdialogex.h"


// CChkMsg ダイアログ

IMPLEMENT_DYNAMIC(CChkMsg, ICSDialog)

CChkMsg::CChkMsg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_CHKMSG, pParent)
{
	m_pSnHeadData	= NULL;
	m_ChkState		= 0;
	m_MsgSign		= 0;
}

CChkMsg::~CChkMsg()
{
}

void CChkMsg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_diag);
}


BEGIN_MESSAGE_MAP(CChkMsg, ICSDialog)
	ON_BN_CLICKED(IDOK, &CChkMsg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChkMsg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChkMsg メッセージ ハンドラー

BOOL CChkMsg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	CDiagData data;
	data.data_imgdata = NULL;

	if( m_MsgSign == 0 ){
		// 振替継続希望
		SetWindowTextA( "振替納税に関する事項" );

		CString msg;
		msg.Format( "納税地が異動した場合又は納税地を変更する場合で、\r\n"
					"引き続き振替納税を希望する場合は、チェックをONにしてください。\r\n"
					"(異動・変更があった場合でも、所轄税務署が変更にならない場合は不要です)" );
		GetDlgItem(IDC_STATIC1)->SetWindowTextA( msg );

		data.text_check = "振替納税を引き続き希望する";
	}
	else if( m_MsgSign == 1 ){
		// 公金受取口座の利用
		SetWindowTextA( "公金受取口座の利用" );

		CString msg;
		msg.Format( "既に公金受取口座の登録が済んでおり、今回の申告の還付金について、\n"
					"公金受取口座への振り込みを希望する場合、チェックをONにしてください。" );
		GetDlgItem(IDC_STATIC1)->SetWindowTextA( msg );

		data.text_check = "公金受取口座の利用";
	}

	if( m_ChkState ){
		data.data_check = 1;
	}
	else{
		data.data_check = 0;
	}

	m_diag.SetData( 0, data );

	// 環境設定色
	COLORREF color;
	ICSColorInfo colorinfo;
	((ICSWinApp *)AfxGetApp())->GetStanderdColor(colorinfo);
	if( colorinfo.m_swOnOff == 1 ){
		color = colorinfo.ViewColor();
	}
	else{
		color = RGB( 0xee, 0xee, 0xee );
	}
	DIAGRAM_ATTRIBUTE attr;
	m_diag.GetAttribute( 0, (LPUNKNOWN)&attr );
	attr.attr_bcolor  = color;
	attr.attr_frcolor = color;
	m_diag.SetAttribute( 0, (LPUNKNOWN)&attr, TRUE );

	m_diag.SetFocus();
	m_diag.SetPosition( 0 );

	ICSDialog::OnInitDialogEX();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CChkMsg::InitInfo( CSnHeadData** pSnHeadData, short ChkState, int MsgSign )
{
	m_pSnHeadData	= pSnHeadData;
	m_ChkState		= ChkState;
	m_MsgSign		= MsgSign;
}

void CChkMsg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDiagData data;
	data.data_imgdata = NULL;
	m_diag.GetData( 0, data );
	if( data.data_check ){
		m_ChkState = 1;

		// 公金受取口座にチェックを付けた後にマイナンバーが削除される可能性もあるため、
		// 以下のメッセージを出してチェックを付けられないようにする
		if( m_MsgSign == 1 ){
			CString str_myno;
			if( (( *m_pSnHeadData )->GetMyNumber ( str_myno ) == 1) && ((*m_pSnHeadData)->Sn_GenericSgn & 0x400 || ((*m_pSnHeadData)->Sn_GenericSgn & 0x1000) ) ){
				ICSMessageBox( "公金受取口座を利用する際に必要な以下の項目が未登録のため、処理を継続できません。\n\n「個人番号」" );
				return;
			}
		}		
	}
	else{
		m_ChkState = 0;
	}
	ICSDialog::OnOK();
}

void CChkMsg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ICSDialog::OnCancel();
}

BOOL CChkMsg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){

		if( pMsg->wParam == VK_TAB ){
			if(::GetKeyState(VK_SHIFT) & 0x80){
				pMsg->wParam = VK_UP;
			}
			else{
				pMsg->wParam = VK_DOWN;
			}
		}
		CWnd* fwnd = GetFocus();
		int nID = 0;
		if ( fwnd ){
			nID = fwnd->GetParent()->GetDlgCtrlID();
			if( nID != IDC_ICSDIAG8CTRL1 ){
				nID = fwnd->GetDlgCtrlID();
			}
		}
		switch( pMsg->wParam ){
			// 次に移動
			case VK_RETURN:
			case VK_DOWN:
			case VK_RIGHT:
			case VK_F3:
				if( nID == IDC_ICSDIAG8CTRL1 ){
					GetDlgItem(IDOK)->SetFocus();
					return TRUE;
				}
				else if( nID ==IDOK ){
					if( pMsg->wParam == VK_RETURN ){
						OnOK();
					}
					else{
						GetDlgItem(IDCANCEL)->SetFocus();
						return TRUE;
					}
				}
				else if( nID == IDCANCEL ){
					if( pMsg->wParam == VK_RETURN ){
						OnCancel();
						return TRUE;
					}
					else{
						m_diag.SetFocus();
						m_diag.SetPosition( 0 );
						return TRUE;
					}
				}
				break;
			// 前に移動
			case VK_UP:
			case VK_LEFT:
			case VK_F2:
				if( nID == IDC_ICSDIAG8CTRL1 ){
					GetDlgItem(IDCANCEL)->SetFocus();
					return TRUE;
				}
				else if( nID ==IDOK ){
					if( pMsg->wParam == VK_RETURN ){
						OnOK();
					}
					else{
						m_diag.SetFocus();
						m_diag.SetPosition( 0 );
						return TRUE;
					}
				}
				else if( nID == IDCANCEL ){
					if( pMsg->wParam == VK_RETURN ){
						OnCancel();
						return TRUE;
					}
					else{
						GetDlgItem(IDOK)->SetFocus();
						return TRUE;
					}
				}
				break;
			case VK_END:
			case VK_ESCAPE:
				OnCancel();
				break;
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}
