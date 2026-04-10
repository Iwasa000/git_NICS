// ShinRefundHjn02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinRefundHjn02EX.h"
#include "RefundDataDef.h"

#include "..\H26HyoView.h"

#include "DiagCtlShiire.h"
#include "DiagCtlShiireHo.h"
#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"
#include "DiagCtlKotei.h"
#include "DiagCtlTokusyu.h"

// CShinRefundHjn02EX ダイアログ

IMPLEMENT_DYNAMIC(CShinRefundHjn02EX, CRefundBaseDialog)

CShinRefundHjn02EX::CShinRefundHjn02EX(CWnd* pParent /*=NULL*/)
	: CRefundBaseDialog(CShinRefundHjn02EX::IDD, pParent)
{
	//----> 20230802 修正依頼162932
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_SHIIRE_HO);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_TOKUSYU);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_MEISAI_H3);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_MEISAI_H4);
	//<---- 20230802 修正依頼162932
}

CShinRefundHjn02EX::~CShinRefundHjn02EX()
{
}

void CShinRefundHjn02EX::DoDataExchange(CDataExchange* pDX)
{
	CRefundBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_H3, m_Meisai3Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_H4, m_Meisai4Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_TOKUSYU, m_TokusyuDiag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_SHIIRE_HO, m_ShiireDiag);
}


BEGIN_MESSAGE_MAP(CShinRefundHjn02EX, CRefundBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEL1, &CShinRefundHjn02EX::OnBnClickedButtonDel1)
	ON_BN_CLICKED(IDC_BUTTON_INS1, &CShinRefundHjn02EX::OnBnClickedButtonIns1)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, &CShinRefundHjn02EX::OnBnClickedButtonDel2)
	ON_BN_CLICKED(IDC_BUTTON_INS2, &CShinRefundHjn02EX::OnBnClickedButtonIns2)
	ON_BN_CLICKED(IDC_BUTTON_UP1, &CShinRefundHjn02EX::OnBnClickedButtonUp1)
	ON_BN_CLICKED(IDC_BUTTON_DOWN1, &CShinRefundHjn02EX::OnBnClickedButtonDown1)
	ON_BN_CLICKED(IDC_BUTTON_UP2, &CShinRefundHjn02EX::OnBnClickedButtonUp2)
	ON_BN_CLICKED(IDC_BUTTON_DOWN2, &CShinRefundHjn02EX::OnBnClickedButtonDown2)
END_MESSAGE_MAP()


// CShinRefundHjn02EX メッセージ ハンドラ

BOOL CShinRefundHjn02EX::OnInitDialog()
{
	CRefundBaseDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	refundvol.KPType = ID_ICSSH_REF_HOJIN;

	SetDiagMap();

	SetKeyMove();

	bottom_diag  = &m_TokusyuDiag;
	bottom_index = HT_INP2;

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
void CShinRefundHjn02EX::EndJob()
{
	CWnd	*pParent = GetParent();
	if( pParent ){
		pParent->SetFocus();
	}
}

BOOL CShinRefundHjn02EX::PreTranslateMessage(MSG* pMsg)
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
void CShinRefundHjn02EX::ReUpdateData()
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
void CShinRefundHjn02EX::OnBnClickedButtonDel1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai3Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < HT1_TYPE )||( HT5_ADDR < sel_idx )){
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

void CShinRefundHjn02EX::OnBnClickedButtonIns1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai3Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < HT1_TYPE )||( HT5_ADDR < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H3);
	control->Line_Insert(sel_idx);

	m_Meisai3Diag.SetPosition(sel_idx);
}

void CShinRefundHjn02EX::OnBnClickedButtonDel2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai4Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < HK1_TYPE )||( HK10_ADDR < sel_idx )){
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

void CShinRefundHjn02EX::OnBnClickedButtonIns2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai4Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	if(( sel_idx < HK1_TYPE )||( HK10_ADDR < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H4);
	control->Line_Insert(sel_idx);

	m_Meisai4Diag.SetPosition(sel_idx);
}


//2016.02.23 INSERT END

//----------------------------------------------------
// １行上へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn02EX::OnBnClickedButtonUp1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai3Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	if ((sel_idx < HT2_TYPE) || (HT5_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H3);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx - 1);

	m_Meisai3Diag.SetPosition(sel_idx - HT_CLMCNT);
}

//----------------------------------------------------
// １行下へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn02EX::OnBnClickedButtonDown1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai3Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai3Diag.GetPosition();
	if ((sel_idx < HT1_TYPE) || (HT4_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H3);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx + 1);

	m_Meisai3Diag.SetPosition(sel_idx + HT_CLMCNT);
}

//----------------------------------------------------
// １行上へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn02EX::OnBnClickedButtonUp2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai4Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	if ((sel_idx < HK2_TYPE) || (HK10_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H4);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx - 1);

	m_Meisai4Diag.SetPosition(sel_idx - HK_CLMCNT);
}

//----------------------------------------------------
// １行下へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn02EX::OnBnClickedButtonDown2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai4Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai4Diag.GetPosition();
	if ((sel_idx < HK1_TYPE) || (HK9_ADDR < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H4);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx + 1);

	m_Meisai4Diag.SetPosition(sel_idx + HK_CLMCNT);
}

//----------------------------------------------------
// diagデータをSetDiagControlMapにセット　20230704
//----------------------------------------------------
void CShinRefundHjn02EX::SetDiagMap()
{
	// 各diagのデータをDiagControlMapにセット
	// コントロールidや、各diagの処理がまとまっているクラス、データクラスなどをnewしてセット
	// DispInitもSetDiagControlMap内で走る
	SetDiagControlMap(	m_ShiireDiag.GetDlgCtrlID(),
						new CDiagCtlShiireHo( m_ShiireDiag ),
						new CSyzDataSet( refundvol, SHIIRE_REC, KOJIN_SHIRE_CNT ) );

	SetDiagControlMap(	m_TokusyuDiag.GetDlgCtrlID(),
						new CDiagCtlTokusyu( m_TokusyuDiag, 1 ),
						new CSyzDataSet( refundvol ) );

	SetDiagControlMap(	m_Meisai3Diag.GetDlgCtrlID(),
						new CDiagCtlTorihiki( m_Meisai3Diag, HOJIN3_LINE_CNT, HT1_TYPE, HT_CLMCNT ),
						new CSyzDataSet( refundvol, TANAOR_REC, HOJIN3_LINE_CNT, &refundvol.KPTsck, &refundvol.KPTsgn ) );

	SetDiagControlMap(	m_Meisai4Diag.GetDlgCtrlID(),
						new CDiagCtlKotei( m_Meisai4Diag, HOJIN4_LINE_CNT, HK1_TYPE, HK_CLMCNT ),
						new CSyzDataSet( refundvol, KOTEIS_REC, HOJIN4_LINE_CNT, &refundvol.KPKsck, &refundvol.KPKsgn ) );
}

//----------------------------------------------------
// diag間のキー移動の設定　20230704
//----------------------------------------------------
void CShinRefundHjn02EX::SetKeyMove()
{
	// int　：現在地
	// CRect：上下左右の移動方向（left,top,right,bottom）
	{	
		// 仕入→棚卸の移動
		pair<int,CRect> next[] = { pair<int,CRect>( HS8_KGK,      CRect( NO_MOVE, NO_MOVE, NO_MOVE, HT_TCHK ) ),
								   pair<int,CRect>( HS8_NOSIIRE,  CRect( NO_MOVE, NO_MOVE, NO_MOVE, HT1_NAME ) ),
								   pair<int,CRect>( HS10_KZSIIRE, CRect( NO_MOVE, NO_MOVE, HT_TCHK, HT1_ADDR ) ), };

		// 仕入←棚卸の移動
		pair<int,CRect> prev[] = { pair<int,CRect>( HT_TCHK,	CRect( HS10_KZSIIRE, HS8_KGK, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HT1_NAME,   CRect( NO_MOVE, HS8_NOSIIRE, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HT1_ADDR,   CRect( NO_MOVE, HS10_KZSIIRE, NO_MOVE, NO_MOVE ) ), };
		SetCrossOver(m_ShiireDiag,m_Meisai3Diag,next,prev);
	}

	{	
		// 棚卸→固定の移動
		pair<int,CRect> next[] = { pair<int,CRect>( HT5_TYPE,    CRect( NO_MOVE, NO_MOVE, NO_MOVE, HK1_TYPE ) ),
								   pair<int,CRect>( HT5_YEAR,    CRect( NO_MOVE, NO_MOVE, NO_MOVE, HK1_YEAR ) ),
								   pair<int,CRect>( HT5_YEARKZK, CRect( NO_MOVE, NO_MOVE, NO_MOVE, HK1_YEAR ) ),
								   pair<int,CRect>( HT5_MNY,     CRect( NO_MOVE, NO_MOVE, NO_MOVE, HK_TCHK ) ),
								   pair<int,CRect>( HT5_INVNO,   CRect( NO_MOVE, NO_MOVE, NO_MOVE, HK1_INVNO ) ),
								   pair<int,CRect>( HT5_NAME,    CRect( NO_MOVE, NO_MOVE, NO_MOVE, HK1_NAME ) ),
								   pair<int,CRect>( HT5_ADDR,    CRect( NO_MOVE, NO_MOVE, HK_TCHK, HK1_ADDR ) ), };

		// 棚卸←固定の移動
		pair<int,CRect> prev[] = { pair<int,CRect>( HK1_TYPE,   CRect( NO_MOVE, HT5_TYPE, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HK1_YEAR,   CRect( NO_MOVE, HT5_YEAR, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HK_TCHK,	CRect( HT5_ADDR, HT5_MNY, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HK1_INVNO,	CRect( NO_MOVE, HT5_INVNO, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HK1_NAME,   CRect( NO_MOVE, HT5_NAME, NO_MOVE, NO_MOVE ) ), 
								   pair<int,CRect>( HK1_ADDR,   CRect( NO_MOVE, HT5_ADDR, NO_MOVE, NO_MOVE ) ),};
		SetCrossOver(m_Meisai3Diag,m_Meisai4Diag,next,prev);
	}

	{	
		// 固定→特殊の移動
		pair<int,CRect> next[] = { pair<int,CRect>( HK10_TYPE, CRect( NO_MOVE, NO_MOVE, NO_MOVE, HT_INP1 ) ),
								   pair<int,CRect>( HK10_YEAR, CRect( NO_MOVE, NO_MOVE, NO_MOVE, HT_INP1 ) ),
								   pair<int,CRect>( HK10_MNY,  CRect( NO_MOVE, NO_MOVE, NO_MOVE, HT_INP1 ) ),
								   pair<int,CRect>( HK10_NAME, CRect( NO_MOVE, NO_MOVE, NO_MOVE, HT_INP1 ) ),
								   pair<int,CRect>( HK10_ADDR, CRect( NO_MOVE, NO_MOVE, HT_INP1, HT_INP1 ) ), };

		// 固定←特殊の移動
		pair<int,CRect> prev[] = { pair<int,CRect>( HT_INP1, CRect( HK10_ADDR, HK10_TYPE, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HT_INP2, CRect( HK10_ADDR, HK10_TYPE, NO_MOVE, NO_MOVE ) ), };
		SetCrossOver(m_Meisai4Diag,m_TokusyuDiag,next,prev);
	}

	{	
		// 特殊→仕入の移動
		pair<int,CRect> next[] = { pair<int,CRect>( HT_INP2, CRect( NO_MOVE, NO_MOVE, HS_KOMI, HS_KOMI ) ) };
		
		// 特殊←仕入の移動
		pair<int,CRect> prev[] = { pair<int,CRect>( HS_KOMI, CRect( HT_INP2, HT_INP2, NO_MOVE, NO_MOVE ) ),
								   pair<int,CRect>( HS_NUKI, CRect( NO_MOVE, HT_INP2, NO_MOVE, NO_MOVE ) ) };
		SetCrossOver(m_TokusyuDiag,m_ShiireDiag,next,prev);
	}
}

//==========================================================
// フォーカス移動
//==========================================================
void CShinRefundHjn02EX::MoveFocus()
{
	// 登録番号欄にフォーカスがある状態でプレビューを行うと
	// プレビュー画面に登録番号選択画面が表示されるため、
	// 別の項目にフォーカスを移動する
	if( GetCurrentDiag() == &m_Meisai3Diag ) {
		// 一行目の資産の種類へ
		this->SetFocus();
		m_Meisai3Diag.SetPosition( HT1_TYPE );
	}
	else if( GetCurrentDiag() == &m_Meisai4Diag ) {
		// 一行目の資産の種類へ
		this->SetFocus();
		m_Meisai4Diag.SetPosition( HK1_TYPE );
	}
}