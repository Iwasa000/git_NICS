// ShinRefundHjn02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinRefundHjn02.h"
#include "RefundDataDef.h"

#include "..\H26HyoView.h"

#include "DiagCtlShiire.h"
#include "DiagCtlShiireHo.h"
#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"
#include "DiagCtlKotei.h"
#include "DiagCtlTokusyu.h"

// CShinRefundHjn02 ダイアログ

IMPLEMENT_DYNAMIC(CShinRefundHjn02, CRefundBaseDialog)

CShinRefundHjn02::CShinRefundHjn02(CWnd* pParent /*=NULL*/)
	: CRefundBaseDialog(CShinRefundHjn02::IDD, pParent)
{

}

CShinRefundHjn02::~CShinRefundHjn02()
{
}

void CShinRefundHjn02::DoDataExchange(CDataExchange* pDX)
{
	CRefundBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_H3, m_Meisai3Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_H4, m_Meisai4Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_TOKUSYU, m_TokusyuDiag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_SHIIRE_HO, m_ShiireDiag);
}


BEGIN_MESSAGE_MAP(CShinRefundHjn02, CRefundBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEL1, &CShinRefundHjn02::OnBnClickedButtonDel1)
	ON_BN_CLICKED(IDC_BUTTON_INS1, &CShinRefundHjn02::OnBnClickedButtonIns1)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, &CShinRefundHjn02::OnBnClickedButtonDel2)
	ON_BN_CLICKED(IDC_BUTTON_INS2, &CShinRefundHjn02::OnBnClickedButtonIns2)
END_MESSAGE_MAP()


// CShinRefundHjn02 メッセージ ハンドラ

BOOL CShinRefundHjn02::OnInitDialog()
{
	CRefundBaseDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	refundvol.KPType = ID_ICSSH_REF_HOJIN;

	SetDiagControlMap( m_ShiireDiag.GetDlgCtrlID(),  new CDiagCtlShiireHo(m_ShiireDiag),   new CSyzDataSet(refundvol, SHIIRE_REC, KOJIN_SHIRE_CNT ) );
	SetDiagControlMap( m_TokusyuDiag.GetDlgCtrlID(), new CDiagCtlTokusyu(m_TokusyuDiag,1), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_Meisai3Diag.GetDlgCtrlID(), new CDiagCtlTorihiki(m_Meisai3Diag,HOJIN3_LINE_CNT,33), new CSyzDataSet(refundvol, TANAOR_REC, HOJIN3_LINE_CNT, &refundvol.KPTsck,&refundvol.KPTsgn) );
	SetDiagControlMap( m_Meisai4Diag.GetDlgCtrlID(), new CDiagCtlKotei(m_Meisai4Diag,HOJIN4_LINE_CNT,58),    new CSyzDataSet(refundvol, KOTEIS_REC, HOJIN4_LINE_CNT, &refundvol.KPKsck,&refundvol.KPKsgn) );

	{	pair<int,CRect> next[] = { pair<int,CRect>( 84,CRect(NO_MOVE,NO_MOVE,NO_MOVE,3)), pair<int,CRect>( 85,CRect(NO_MOVE,NO_MOVE,NO_MOVE,37)),
								   pair<int,CRect>( 90,CRect(NO_MOVE,NO_MOVE,3,38)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(  3,CRect(90,84,NO_MOVE,NO_MOVE)),     pair<int,CRect>( 37,CRect(NO_MOVE,85,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 38,CRect(NO_MOVE,90,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_ShiireDiag,m_Meisai3Diag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>( 57,CRect(NO_MOVE,NO_MOVE,NO_MOVE,58)), pair<int,CRect>( 58,CRect(NO_MOVE,NO_MOVE,NO_MOVE,59)),
								   pair<int,CRect>( 59,CRect(NO_MOVE,NO_MOVE,NO_MOVE,60)), pair<int,CRect>( 60,CRect(NO_MOVE,NO_MOVE,NO_MOVE,3)),
								   pair<int,CRect>( 61,CRect(NO_MOVE,NO_MOVE,NO_MOVE,62)), pair<int,CRect>( 62,CRect(NO_MOVE,NO_MOVE,3,63)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 58,CRect(NO_MOVE,57,NO_MOVE,NO_MOVE)), pair<int,CRect>( 59,CRect(NO_MOVE,58,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 60,CRect(NO_MOVE,59,NO_MOVE,NO_MOVE)), pair<int,CRect>(  3,CRect(62,60,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 62,CRect(NO_MOVE,61,NO_MOVE,NO_MOVE)), pair<int,CRect>( 63,CRect(NO_MOVE,62,NO_MOVE,NO_MOVE)),};
		SetCrossOver(m_Meisai3Diag,m_Meisai4Diag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>( 112,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)), pair<int,CRect>( 113,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)),
								   pair<int,CRect>( 114,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)), pair<int,CRect>( 115,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)),
								   pair<int,CRect>( 116,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)), pair<int,CRect>( 117,CRect(NO_MOVE,NO_MOVE,1,1)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(   1,CRect(117,112,NO_MOVE,NO_MOVE)),   pair<int,CRect>(   6,CRect(117,112,NO_MOVE,NO_MOVE)),};
		SetCrossOver(m_Meisai4Diag,m_TokusyuDiag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>(  6,CRect(NO_MOVE,NO_MOVE,59,59)) };
		pair<int,CRect> prev[] = { pair<int,CRect>( 59,CRect(6,6,NO_MOVE,NO_MOVE)),pair<int,CRect>( 60,CRect(NO_MOVE,6,NO_MOVE,NO_MOVE)) };
		SetCrossOver(m_TokusyuDiag,m_ShiireDiag,next,prev);
	}

	bottom_diag  = &m_TokusyuDiag;
	bottom_index = 6;

	if( pSnHeadData->Sn_Sign4&0x80 ){
		m_ShiireDiag.SetFocus();
	}
	else{
		m_Meisai3Diag.SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CShinRefundHjn02::EndJob()
{
	CWnd	*pParent = GetParent();
	if( pParent ){
		pParent->SetFocus();
	}
}

BOOL CShinRefundHjn02::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN )	{
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;
		if( pMsg->wParam == VK_F5 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F5 );
				return( 1 );
			}
//			((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F5 );
		}
		if( pMsg->wParam == VK_F4 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F4 );
				return( 1 );
			}
//			((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F4 );
		}
		if( pMsg->wParam == VK_F7 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F7 );
				return( 1 );
			}
//			((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F7 );

		}
		if( pMsg->wParam == VK_F9 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F9 );
				return( 1 );
			}
//			((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F9 );

		}
		if( pMsg->wParam == VK_F11 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F11 );
				return( 1 );
			}
//			((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F11 );

		}
		if( pMsg->wParam == VK_F12 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F12 );
				return( 1 );
			}
//			((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F12 );
		}
#ifdef	_CLOSE
		if( pMsg->wParam == VK_F8 ) {
			OnBnClickedButton1();
			return( 1 );
		}
		if( pMsg->wParam == VK_F9 ) {
			OnBnClickedButton2();
			return( 1 );
		}
#endif
	}

	return CRefundBaseDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 保存データの更新
//-----------------------------------------------------------------------------
void CShinRefundHjn02::ReUpdateData()
{
//--> '14.10.30 INS START
	POSITION	pos = DiagControlMap.GetStartPosition();
	while( pos != NULL ){
		int key;
		CDiagControlBase* control = 0;
		DiagControlMap.GetNextAssoc( pos, key, control );
	
		if( control ){
			delete control;
		}
	}
//<-- '14.10.30 INS START
	DiagControlMap.RemoveAll();

	SetDiagControlMap( m_ShiireDiag.GetDlgCtrlID(),  new CDiagCtlShiireHo(m_ShiireDiag),   new CSyzDataSet(refundvol, SHIIRE_REC, KOJIN_SHIRE_CNT ) );
	SetDiagControlMap( m_TokusyuDiag.GetDlgCtrlID(), new CDiagCtlTokusyu(m_TokusyuDiag,1), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_Meisai3Diag.GetDlgCtrlID(), new CDiagCtlTorihiki(m_Meisai3Diag,HOJIN3_LINE_CNT,33), new CSyzDataSet(refundvol, TANAOR_REC, HOJIN3_LINE_CNT, &refundvol.KPTsck,&refundvol.KPTsgn) );
	SetDiagControlMap( m_Meisai4Diag.GetDlgCtrlID(), new CDiagCtlKotei(m_Meisai4Diag,HOJIN4_LINE_CNT,58),    new CSyzDataSet(refundvol, KOTEIS_REC, HOJIN4_LINE_CNT, &refundvol.KPKsck,&refundvol.KPKsgn) );

	{	pair<int,CRect> next[] = { pair<int,CRect>( 84,CRect(NO_MOVE,NO_MOVE,NO_MOVE,3)), pair<int,CRect>( 85,CRect(NO_MOVE,NO_MOVE,NO_MOVE,37)),
								   pair<int,CRect>( 90,CRect(NO_MOVE,NO_MOVE,3,38)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(  3,CRect(90,84,NO_MOVE,NO_MOVE)),     pair<int,CRect>( 37,CRect(NO_MOVE,85,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 38,CRect(NO_MOVE,90,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_ShiireDiag,m_Meisai3Diag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>( 57,CRect(NO_MOVE,NO_MOVE,NO_MOVE,58)), pair<int,CRect>( 58,CRect(NO_MOVE,NO_MOVE,NO_MOVE,59)),
								   pair<int,CRect>( 59,CRect(NO_MOVE,NO_MOVE,NO_MOVE,60)), pair<int,CRect>( 60,CRect(NO_MOVE,NO_MOVE,NO_MOVE,3)),
								   pair<int,CRect>( 61,CRect(NO_MOVE,NO_MOVE,NO_MOVE,62)), pair<int,CRect>( 62,CRect(NO_MOVE,NO_MOVE,3,63)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 58,CRect(NO_MOVE,57,NO_MOVE,NO_MOVE)), pair<int,CRect>( 59,CRect(NO_MOVE,58,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 60,CRect(NO_MOVE,59,NO_MOVE,NO_MOVE)), pair<int,CRect>(  3,CRect(62,60,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 62,CRect(NO_MOVE,61,NO_MOVE,NO_MOVE)), pair<int,CRect>( 63,CRect(NO_MOVE,62,NO_MOVE,NO_MOVE)),};
		SetCrossOver(m_Meisai3Diag,m_Meisai4Diag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>( 112,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)), pair<int,CRect>( 113,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)),
								   pair<int,CRect>( 114,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)), pair<int,CRect>( 115,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)),
								   pair<int,CRect>( 116,CRect(NO_MOVE,NO_MOVE,NO_MOVE,1)), pair<int,CRect>( 117,CRect(NO_MOVE,NO_MOVE,1,1)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(   1,CRect(117,112,NO_MOVE,NO_MOVE)),   pair<int,CRect>(   6,CRect(117,112,NO_MOVE,NO_MOVE)),};
		SetCrossOver(m_Meisai4Diag,m_TokusyuDiag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>(  6,CRect(NO_MOVE,NO_MOVE,59,59)) };
		pair<int,CRect> prev[] = { pair<int,CRect>( 59,CRect(6,6,NO_MOVE,NO_MOVE)),pair<int,CRect>( 60,CRect(NO_MOVE,6,NO_MOVE,NO_MOVE)) };
		SetCrossOver(m_TokusyuDiag,m_ShiireDiag,next,prev);
	}
}

//2016.02.23 INSERT START
void CShinRefundHjn02::OnBnClickedButtonDel1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai3Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 33 )||( 62 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H3);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai3Diag.SetPosition(sel_idx);
}

void CShinRefundHjn02::OnBnClickedButtonIns1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai3Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 33 )||( 62 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H3);
	control->Line_Insert(sel_idx);

	m_Meisai3Diag.SetPosition(sel_idx);
}

void CShinRefundHjn02::OnBnClickedButtonDel2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai4Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 58 )||( 117 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H4);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai4Diag.SetPosition(sel_idx);
}

void CShinRefundHjn02::OnBnClickedButtonIns2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai4Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	if(( sel_idx < 58 )||( 117 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H4);
	control->Line_Insert(sel_idx);

	m_Meisai4Diag.SetPosition(sel_idx);
}


//2016.02.23 INSERT END
