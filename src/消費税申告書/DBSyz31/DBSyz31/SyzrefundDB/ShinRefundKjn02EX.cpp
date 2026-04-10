// ShinRefundKjn02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinRefundKjn02EX.h"
#include "RefundDataDef.h"

#include "..\H26HyoView.h"

#include "DiagCtlShiire.h"
#include "DiagCtlShiireKo.h"
#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"
#include "DiagCtlKotei.h"
#include "DiagCtlTokusyu.h"

// CShinRefundKjn02EX ダイアログ

IMPLEMENT_DYNAMIC(CShinRefundKjn02EX, CRefundBaseDialog)

CShinRefundKjn02EX::CShinRefundKjn02EX(CWnd* pParent /*=NULL*/)
	: CRefundBaseDialog(CShinRefundKjn02EX::IDD, pParent)
{
	//----> 20230802 修正依頼162932
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_SHIIRE_KO);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_TOKUSYU);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_MEISAI_K3);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_MEISAI_K4);
	//<---- 20230802 修正依頼162932
}

CShinRefundKjn02EX::~CShinRefundKjn02EX()
{
}

void CShinRefundKjn02EX::DoDataExchange(CDataExchange* pDX)
{
	CRefundBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_SHIIRE_KO, m_ShiireDiag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_K3, m_Meisai3Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_K4, m_Meisai4Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_TOKUSYU, m_TokusyuDiag);
}


BEGIN_MESSAGE_MAP(CShinRefundKjn02EX, CRefundBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEL1, &CShinRefundKjn02EX::OnBnClickedButtonDel1)
	ON_BN_CLICKED(IDC_BUTTON_INS1, &CShinRefundKjn02EX::OnBnClickedButtonIns1)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, &CShinRefundKjn02EX::OnBnClickedButtonDel2)
	ON_BN_CLICKED(IDC_BUTTON_INS2, &CShinRefundKjn02EX::OnBnClickedButtonIns2)
	ON_BN_CLICKED(IDC_BUTTON_UP1, &CShinRefundKjn02EX::OnBnClickedButtonUp1)
	ON_BN_CLICKED(IDC_BUTTON_DOWN1, &CShinRefundKjn02EX::OnBnClickedButtonDown1)
	ON_BN_CLICKED(IDC_BUTTON_UP2, &CShinRefundKjn02EX::OnBnClickedButtonUp2)
	ON_BN_CLICKED(IDC_BUTTON_DOWN2, &CShinRefundKjn02EX::OnBnClickedButtonDown2)
END_MESSAGE_MAP()


// CShinRefundKjn02EX メッセージ ハンドラ

BOOL CShinRefundKjn02EX::OnInitDialog()
{
	CRefundBaseDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	refundvol.KPType = ID_ICSSH_REF_KOJIN;

	SetDiagMap();

	SetKeyMove();

	bottom_diag  = &m_TokusyuDiag;
	bottom_index = KT_INP2;

	//if( pSnHeadData->Sn_Sign4&0x80 ){
	m_ShiireDiag.SetFocus();
	//}
	//else{
	//	m_Meisai3Diag.SetFocus();
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CShinRefundKjn02EX::EndJob()
{
	CWnd	*pParent = GetParent();
	if( pParent ){
		pParent->SetFocus();
	}
}

BOOL CShinRefundKjn02EX::PreTranslateMessage(MSG* pMsg)
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
			return( 1 );
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
	else if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN) {
		if (CheckInvno(1) == false) {
			return TRUE;
		}
	}

	return CRefundBaseDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 保存データの更新
//-----------------------------------------------------------------------------
void CShinRefundKjn02EX::ReUpdateData()
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

	SetDiagMap();

	SetKeyMove();
}
//2016.02.23 INSERT START
void CShinRefundKjn02EX::OnBnClickedButtonDel1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai3Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < KT1_TYPE )||( KT5_ADDR < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K3);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai3Diag.SetPosition(sel_idx);
}

void CShinRefundKjn02EX::OnBnClickedButtonIns1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai3Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < KT1_TYPE )||( KT5_ADDR < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K3);
	control->Line_Insert(sel_idx);

	m_Meisai3Diag.SetPosition(sel_idx);
}

void CShinRefundKjn02EX::OnBnClickedButtonDel2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai4Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < KK1_TYPE )||( KK5_ADDR < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K4);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai4Diag.SetPosition(sel_idx);
}

void CShinRefundKjn02EX::OnBnClickedButtonIns2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai4Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < KK1_TYPE )||( KK5_ADDR < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K4);
	control->Line_Insert(sel_idx);

	m_Meisai4Diag.SetPosition(sel_idx);
}
//2016.02.23 INSERT END

//----------------------------------------------------
// １行上へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundKjn02EX::OnBnClickedButtonUp1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai3Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	if ((sel_idx < KT2_TYPE) || (KT5_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K3);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx - 1);

	m_Meisai3Diag.SetPosition(sel_idx - KT_CLMCNT);
}

//----------------------------------------------------
// １行下へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundKjn02EX::OnBnClickedButtonDown1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai3Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	if ((sel_idx < KT1_TYPE) || (KT4_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K3);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx + 1);

	m_Meisai3Diag.SetPosition(sel_idx + KT_CLMCNT);
}

//----------------------------------------------------
// １行上へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundKjn02EX::OnBnClickedButtonUp2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai4Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	if ((sel_idx < KK2_TYPE) || (KK5_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K4);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx - 1);

	m_Meisai4Diag.SetPosition(sel_idx - KK_CLMCNT);
}

//----------------------------------------------------
// １行下へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundKjn02EX::OnBnClickedButtonDown2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai4Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	if ((sel_idx < KK1_TYPE) || (KK4_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K4);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx + 1);

	m_Meisai4Diag.SetPosition(sel_idx + KK_CLMCNT);
}

//----------------------------------------------------
// diagデータをSetDiagControlMapにセット　20230704
//----------------------------------------------------
void CShinRefundKjn02EX::SetDiagMap()
{
	// 各diagのデータをDiagControlMapにセット
	// コントロールidや、各diagの処理がまとまっているクラス、データクラスなどをnewしてセット
	// DispInitもSetDiagControlMap内で走る
	SetDiagControlMap(	m_ShiireDiag.GetDlgCtrlID(),
						new CDiagCtlShiireKo( m_ShiireDiag ),
						new CSyzDataSet( refundvol, SHIIRE_REC, KOJIN_SHIRE_CNT ) );

	SetDiagControlMap(	m_TokusyuDiag.GetDlgCtrlID(),
						new CDiagCtlTokusyu( m_TokusyuDiag, 0 ),
						new CSyzDataSet( refundvol ) );

	SetDiagControlMap(	m_Meisai3Diag.GetDlgCtrlID(),
						new CDiagCtlTorihiki( m_Meisai3Diag, KOJIN3_LINE_CNT, KT1_TYPE, KT_CLMCNT ),
						new CSyzDataSet( refundvol, TANAOR_REC, KOJIN3_LINE_CNT, &refundvol.KPTsck, &refundvol.KPTsgn ) );

	SetDiagControlMap(	m_Meisai4Diag.GetDlgCtrlID(),
						new CDiagCtlKotei( m_Meisai4Diag, KOJIN4_LINE_CNT, KK1_TYPE, KK_CLMCNT ),
						new CSyzDataSet( refundvol, KOTEIS_REC, KOJIN4_LINE_CNT, &refundvol.KPKsck, &refundvol.KPKsgn ) );
}

//----------------------------------------------------
// diag間のキー移動の設定　20230704
//----------------------------------------------------
void CShinRefundKjn02EX::SetKeyMove()
{
	// int　：現在地
	// CRect：上下左右の移動方向（left,top,right,bottom）
	{	
		// 仕入→棚卸の移動
		pair<int,CRect> next[] = {	pair<int,CRect>( KS_SYOTOKU,	CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT1_TYPE ) ),
									pair<int,CRect>( KS10_KSN,		CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT_TCHK ) ),
									pair<int,CRect>( KS10_NOSIIRE,	CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT1_NAME ) ),
									pair<int,CRect>( KS13_KZSIIRE,	CRect( NO_MOVE, NO_MOVE, KT_TCHK, KT1_ADDR ) ), };

		// 仕入←棚卸の移動
		pair<int,CRect> prev[] = {	pair<int,CRect>( KT1_TYPE,	CRect( NO_MOVE, KS_SYOTOKU, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KT_TCHK,	CRect( KS13_KZSIIRE, KS10_KSN, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KT1_NAME,	CRect( NO_MOVE, KS10_NOSIIRE, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KT1_ADDR,	CRect( NO_MOVE, KS13_KZSIIRE, NO_MOVE, NO_MOVE ) ), };
		SetCrossOver(m_ShiireDiag,m_Meisai3Diag,next,prev);
	}
	
	{	
		// 棚卸→固定の移動
		pair<int,CRect> next[] = {	pair<int,CRect>( KT5_TYPE,	  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KK1_TYPE ) ),
									pair<int,CRect>( KT5_YEAR,	  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KK1_YEAR ) ),
									pair<int,CRect>( KT5_YEARKZK, CRect( NO_MOVE, NO_MOVE, NO_MOVE, KK1_YEAR ) ),
									pair<int,CRect>( KT5_MNY,	  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KK_TCHK ) ),
									pair<int,CRect>( KT5_INVNO,	  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KK1_INVNO ) ),
									pair<int,CRect>( KT5_NAME,	  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KK1_NAME ) ),
									pair<int,CRect>( KT5_ADDR,	  CRect( NO_MOVE, NO_MOVE, KK_TCHK, KK1_ADDR ) ), };

		// 棚卸←固定の移動
		pair<int,CRect> prev[] = {	pair<int,CRect>( KK1_TYPE,	 CRect( NO_MOVE, KT5_TYPE, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KK1_YEAR,   CRect( NO_MOVE, KT5_YEAR, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KK_TCHK,	 CRect( KT5_ADDR, KT5_MNY, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KK1_INVNO,  CRect( NO_MOVE, KT5_INVNO, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KK1_NAME,   CRect( NO_MOVE, KT5_NAME, NO_MOVE, NO_MOVE) ),
									pair<int,CRect>( KK1_ADDR,   CRect( NO_MOVE, KT5_ADDR, NO_MOVE, NO_MOVE) ), };
		SetCrossOver(m_Meisai3Diag,m_Meisai4Diag,next,prev);
	}
	
	{	
		// 固定→特殊の移動
		pair<int,CRect> next[] = {	pair<int,CRect>( KK5_TYPE,  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT_INP1 ) ),
									pair<int,CRect>( KK5_YEAR,  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT_INP1 ) ),
									pair<int,CRect>( KK5_MNY,   CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT_INP1 ) ),
									pair<int,CRect>( KK5_INVNO, CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT_INP1 ) ),
									pair<int,CRect>( KK5_NAME,  CRect( NO_MOVE, NO_MOVE, NO_MOVE, KT_INP1 ) ),
									pair<int,CRect>( KK5_ADDR,  CRect( NO_MOVE, NO_MOVE, KT_INP1, KT_INP1 ) ), };

		// 固定←特殊の移動
		pair<int,CRect> prev[] = {	pair<int,CRect>( KT_INP1, CRect( KK5_ADDR, KK5_TYPE, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KT_INP2, CRect( KK5_ADDR, KK5_TYPE, NO_MOVE, NO_MOVE ) ),};
		SetCrossOver(m_Meisai4Diag,m_TokusyuDiag,next,prev);
	}

	{	
		// 特殊→仕入の移動
		pair<int,CRect> next[] = {	pair<int,CRect>( KT_INP2, CRect( NO_MOVE, NO_MOVE, KS_KOMI, KS_NUKI ) ) };

		// 特殊←仕入の移動
		pair<int,CRect> prev[] = {	pair<int,CRect>( KS_KOMI, CRect( KS_KOMI, KS_NUKI, NO_MOVE, NO_MOVE ) ),
									pair<int,CRect>( KS_NUKI, CRect( NO_MOVE, KT_INP2, NO_MOVE, NO_MOVE ) ) };
		SetCrossOver(m_TokusyuDiag,m_ShiireDiag,next,prev);
	}
}

//==========================================================
// フォーカス移動
//==========================================================
void CShinRefundKjn02EX::MoveFocus()
{
	// 登録番号欄にフォーカスがある状態でプレビューを行うと
	// プレビュー画面に登録番号選択画面が表示されるため、
	// 別の項目にフォーカスを移動する
	if( GetCurrentDiag() == &m_Meisai3Diag ) {
		// 一行目の資産の種類へ
		this->SetFocus();
		m_Meisai3Diag.SetPosition( KT1_TYPE );
	}
	else if( GetCurrentDiag() == &m_Meisai4Diag ) {
		// 一行目の資産の種類へ
		this->SetFocus();
		m_Meisai4Diag.SetPosition( KK1_TYPE );
	}
}