// ShinRefundKjn01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinRefundKjn01.h"
#include "RefundDataDef.h"

#include "..\H26HyoView.h"

#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"
#include "DiagCtlYsMeisai.h"
#include "DiagCtlRiyuu.h"
#include "DiagCtlYusyutsu.h"


// CShinRefundKjn01 ダイアログ

IMPLEMENT_DYNAMIC(CShinRefundKjn01, CRefundBaseDialog)

CShinRefundKjn01::CShinRefundKjn01(CWnd* pParent /*=NULL*/)
	: CRefundBaseDialog(CShinRefundKjn01::IDD, pParent)
{

}

CShinRefundKjn01::~CShinRefundKjn01()
{
}

void CShinRefundKjn01::DoDataExchange(CDataExchange* pDX)
{
	CRefundBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_RIYUU, m_RiyuuDiag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_K1, m_Meisai1Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_K2, m_Meisai2Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_YUSYUTSU, m_YusyutsuDiag);
}


BEGIN_MESSAGE_MAP(CShinRefundKjn01, CRefundBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEL1, &CShinRefundKjn01::OnBnClickedButtonDel1)
	ON_BN_CLICKED(IDC_BUTTON_INS1, &CShinRefundKjn01::OnBnClickedButtonIns1)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, &CShinRefundKjn01::OnBnClickedButtonDel2)
	ON_BN_CLICKED(IDC_BUTTON_INS2, &CShinRefundKjn01::OnBnClickedButtonIns2)
END_MESSAGE_MAP()


// CShinRefundKjn01 メッセージ ハンドラ

BOOL CShinRefundKjn01::OnInitDialog()
{
	CRefundBaseDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	SetTitle();
	refundvol.KPType = ID_ICSSH_REF_KOJIN;

	SetDiagControlMap( m_RiyuuDiag.GetDlgCtrlID(),   new CDiagCtlRiyuu(m_RiyuuDiag),       new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_YusyutsuDiag.GetDlgCtrlID(),new CDiagCtlYusyutsu(m_YusyutsuDiag), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_Meisai1Diag.GetDlgCtrlID(), new CDiagCtlTorihiki(m_Meisai1Diag,KOJIN1_LINE_CNT,33), new CSyzDataSet(refundvol, KAZEIU_REC, KOJIN1_LINE_CNT, &refundvol.KPKuck,&refundvol.KPKusg) );
	SetDiagControlMap( m_Meisai2Diag.GetDlgCtrlID(), new CDiagCtlYsMeisai(m_Meisai2Diag,KOJIN2_LINE_CNT,30), new CSyzDataSet(refundvol, YUNYUT_REC, KOJIN2_LINE_CNT ) );

	{	pair<int,CRect> next[] = { pair<int,CRect>( 3,CRect(NO_MOVE,NO_MOVE,NO_MOVE,33)), pair<int,CRect>( 5,CRect(NO_MOVE,NO_MOVE,NO_MOVE,37)),
								   pair<int,CRect>( 8,CRect(NO_MOVE,NO_MOVE,3,38)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 33,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>( 34,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 35,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>(  3,CRect(8,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 37,CRect(NO_MOVE,5,NO_MOVE,NO_MOVE)), pair<int,CRect>( 38,CRect(NO_MOVE,8,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_RiyuuDiag,m_Meisai1Diag,next,prev);
	}
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 57,CRect(NO_MOVE,NO_MOVE,NO_MOVE,30)), pair<int,CRect>( 58,CRect(NO_MOVE,NO_MOVE,NO_MOVE,30)),
								   pair<int,CRect>( 59,CRect(NO_MOVE,NO_MOVE,NO_MOVE,31)), pair<int,CRect>( 60,CRect(NO_MOVE,NO_MOVE,NO_MOVE,31)),
								   pair<int,CRect>( 61,CRect(NO_MOVE,NO_MOVE,NO_MOVE,32)), pair<int,CRect>( 62,CRect(NO_MOVE,NO_MOVE,30,33)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 30,CRect(62,57,NO_MOVE,NO_MOVE)),	   pair<int,CRect>( 31,CRect(NO_MOVE,60,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 32,CRect(NO_MOVE,61,NO_MOVE,NO_MOVE)), pair<int,CRect>( 33,CRect(NO_MOVE,62,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 34,CRect(NO_MOVE,62,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai1Diag,m_Meisai2Diag,next,prev);
	}
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 51,CRect(NO_MOVE,NO_MOVE,NO_MOVE,12)), pair<int,CRect>( 52,CRect(NO_MOVE,NO_MOVE,NO_MOVE,13)),
								   pair<int,CRect>( 53,CRect(NO_MOVE,NO_MOVE,NO_MOVE,14)), pair<int,CRect>( 54,CRect(NO_MOVE,NO_MOVE,12,15)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 12,CRect(54,51,NO_MOVE,NO_MOVE)),      pair<int,CRect>( 14,CRect(NO_MOVE,53,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai2Diag,m_YusyutsuDiag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>(  25,CRect(NO_MOVE,NO_MOVE,1,1)) };
		pair<int,CRect> prev[] = { pair<int,CRect>(   1,CRect(25,25,NO_MOVE,NO_MOVE)) };
		SetCrossOver(m_YusyutsuDiag,m_RiyuuDiag,next,prev);
	}

	bottom_diag  = &m_YusyutsuDiag;
	bottom_index = 11;

	m_RiyuuDiag.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CShinRefundKjn01::EndJob()
{
	GetDlgItem(IDC_STATIC_HELP)->SetFocus();
}

BOOL CShinRefundKjn01::PreTranslateMessage(MSG* pMsg)
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

		if( pMsg->wParam == VK_INSERT ) { //terminationに変更
//2016.02.23 UPDATE START
			//KinyuuDlgKjn( 0 );
			//return( 1 );
			if( KinyuuDlgKjn( 0 ) == 0 ){
				return( 1 );
			}
//2016.02.23 UPDATE END
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
	else if( pMsg->message == WM_LBUTTONDOWN ){
		CRect rect;
		GetDlgItem(IDC_STATIC_HELP)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt) != 0) {
			KinyuuDlgKjn( 1 );
			return( 1 );
		}
	}

	return CRefundBaseDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// 還付先金融機関情報の転記（個人） harino ('14.03.08)
//-----------------------------------------------------------------------------
// 引数	sw			：	0 insertキー
//						1 static
//-----------------------------------------------------------------------------
//2016.02.23 UPDATE START
//void CShinRefundKjn01::KinyuuDlgKjn( int sw )
int CShinRefundKjn01::KinyuuDlgKjn( int sw )
//2016.02.23 UPDATE END
{
	CIcsdiagctrl*	diag;
	int				id, index;

	if( !(pSnHeadData->Sn_Sign4&0x01) ){
		if( sw == 1 ) {	// staticをクリックされた場合
			diag = GetCurrentDiag();
			id = IDC_ICSDIAGCTRL_YUSYUTSU;
			index = diag->GetPosition();
		}
		else {	// insertキーを押された場合
			diag = GetCurrentDiag();
			// 金融機関にフォーカスがある場合に限定する
			if( diag == &m_YusyutsuDiag ){
				id = diag->GetDlgCtrlID();
				index = diag->GetPosition();
				if( (index < 12) || (index > 23) ) {	
//2016.02.23 UPDATE START
//					return;
					return 0;
//2016.02.23 UPDATE END
				}
			}
//2016.02.23 UPDATE START
//			else return;
			else return 1;
//2016.02.23 UPDATE END
		}
		VolBankInit();
		(GetDiagControl(id))->DispInit();	// 該当部のみデータ再作成
		diag->SetPosition(index);			// 元のポジションに戻す
	}
//2016.02.23 INSERT START
	return 0;
//2016.02.23 INSERT END
}

//-----------------------------------------------------------------------------
// 保存データの更新
//-----------------------------------------------------------------------------
void CShinRefundKjn01::ReUpdateData()
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

	SetDiagControlMap( m_RiyuuDiag.GetDlgCtrlID(),   new CDiagCtlRiyuu(m_RiyuuDiag),       new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_YusyutsuDiag.GetDlgCtrlID(),new CDiagCtlYusyutsu(m_YusyutsuDiag), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_Meisai1Diag.GetDlgCtrlID(), new CDiagCtlTorihiki(m_Meisai1Diag,KOJIN1_LINE_CNT,33), new CSyzDataSet(refundvol, KAZEIU_REC, KOJIN1_LINE_CNT, &refundvol.KPKuck,&refundvol.KPKusg) );
	SetDiagControlMap( m_Meisai2Diag.GetDlgCtrlID(), new CDiagCtlYsMeisai(m_Meisai2Diag,KOJIN2_LINE_CNT,30), new CSyzDataSet(refundvol, YUNYUT_REC, KOJIN2_LINE_CNT ) );

	{	pair<int,CRect> next[] = { pair<int,CRect>( 3,CRect(NO_MOVE,NO_MOVE,NO_MOVE,33)), pair<int,CRect>( 5,CRect(NO_MOVE,NO_MOVE,NO_MOVE,37)),
								   pair<int,CRect>( 8,CRect(NO_MOVE,NO_MOVE,3,38)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 33,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>( 34,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 35,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>(  3,CRect(8,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 37,CRect(NO_MOVE,5,NO_MOVE,NO_MOVE)), pair<int,CRect>( 38,CRect(NO_MOVE,8,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_RiyuuDiag,m_Meisai1Diag,next,prev);
	}
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 57,CRect(NO_MOVE,NO_MOVE,NO_MOVE,30)), pair<int,CRect>( 58,CRect(NO_MOVE,NO_MOVE,NO_MOVE,30)),
								   pair<int,CRect>( 59,CRect(NO_MOVE,NO_MOVE,NO_MOVE,31)), pair<int,CRect>( 60,CRect(NO_MOVE,NO_MOVE,NO_MOVE,31)),
								   pair<int,CRect>( 61,CRect(NO_MOVE,NO_MOVE,NO_MOVE,32)), pair<int,CRect>( 62,CRect(NO_MOVE,NO_MOVE,30,33)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 30,CRect(62,57,NO_MOVE,NO_MOVE)),	   pair<int,CRect>( 31,CRect(NO_MOVE,60,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 32,CRect(NO_MOVE,61,NO_MOVE,NO_MOVE)), pair<int,CRect>( 33,CRect(NO_MOVE,62,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 34,CRect(NO_MOVE,62,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai1Diag,m_Meisai2Diag,next,prev);
	}
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 51,CRect(NO_MOVE,NO_MOVE,NO_MOVE,12)), pair<int,CRect>( 52,CRect(NO_MOVE,NO_MOVE,NO_MOVE,13)),
								   pair<int,CRect>( 53,CRect(NO_MOVE,NO_MOVE,NO_MOVE,14)), pair<int,CRect>( 54,CRect(NO_MOVE,NO_MOVE,12,15)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 12,CRect(54,51,NO_MOVE,NO_MOVE)),      pair<int,CRect>( 14,CRect(NO_MOVE,53,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai2Diag,m_YusyutsuDiag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>(  25,CRect(NO_MOVE,NO_MOVE,1,1)) };
		pair<int,CRect> prev[] = { pair<int,CRect>(   1,CRect(25,25,NO_MOVE,NO_MOVE)) };
		SetCrossOver(m_YusyutsuDiag,m_RiyuuDiag,next,prev);
	}
}

//2016.02.23 INSERT START
void CShinRefundKjn01::OnBnClickedButtonDel1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai1Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai1Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < 33 )||( 62 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K1);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai1Diag.SetPosition(sel_idx);
}

void CShinRefundKjn01::OnBnClickedButtonIns1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai1Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai1Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < 33 )||( 62 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}


	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K1);
	control->Line_Insert(sel_idx);

	m_Meisai1Diag.SetPosition(sel_idx);
}

void CShinRefundKjn01::OnBnClickedButtonDel2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai2Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai2Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < 30 )||( 54 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K2);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai2Diag.SetPosition(sel_idx);
}

void CShinRefundKjn01::OnBnClickedButtonIns2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai2Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai2Diag.GetPosition();
//	if( sel_idx == -1 ){
	if(( sel_idx < 30 )||( 54 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_K2);
	control->Line_Insert(sel_idx);

	m_Meisai2Diag.SetPosition(sel_idx);
}
//2016.02.23 INSERT END