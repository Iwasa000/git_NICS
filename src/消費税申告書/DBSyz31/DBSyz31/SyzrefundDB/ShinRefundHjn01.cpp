// ShinRefundHjn01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinRefundHjn01.h"
#include "RefundDataDef.h"

#include "..\H26HyoView.h"

#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"
#include "DiagCtlYsMeisai.h"
#include "DiagCtlRiyuu.h"
#include "DiagCtlYusyutsu.h"

// CShinRefundHjn01 ダイアログ

IMPLEMENT_DYNAMIC(CShinRefundHjn01, CRefundBaseDialog)

CShinRefundHjn01::CShinRefundHjn01(CWnd* pParent /*=NULL*/)
	: CRefundBaseDialog(CShinRefundHjn01::IDD, pParent)
{
	//----> 20230802 修正依頼162932
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_RIYUU);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_YUSYUTSU);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_MEISAI_H1);
	diagKeyVec.emplace_back(IDC_ICSDIAGCTRL_MEISAI_H2);
	//<---- 20230802 修正依頼162932
}

CShinRefundHjn01::~CShinRefundHjn01()
{
}

void CShinRefundHjn01::DoDataExchange(CDataExchange* pDX)
{
	CRefundBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_RIYUU, m_RiyuuDiag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_H1, m_Meisai1Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_MEISAI_H2, m_Meisai2Diag);
	DDX_Control(pDX, IDC_ICSDIAGCTRL_YUSYUTSU, m_YusyutsuDiag);
}


BEGIN_MESSAGE_MAP(CShinRefundHjn01, CRefundBaseDialog)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DEL1, &CShinRefundHjn01::OnBnClickedButtonDel1)
	ON_BN_CLICKED(IDC_BUTTON_INS1, &CShinRefundHjn01::OnBnClickedButtonIns1)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, &CShinRefundHjn01::OnBnClickedButtonDel2)
	ON_BN_CLICKED(IDC_BUTTON_INS2, &CShinRefundHjn01::OnBnClickedButtonIns2)
	ON_BN_CLICKED(IDC_BUTTON_UP1, &CShinRefundHjn01::OnBnClickedButtonUp1)
	ON_BN_CLICKED(IDC_BUTTON_DOWN1, &CShinRefundHjn01::OnBnClickedButtonDown1)
	ON_BN_CLICKED(IDC_BUTTON_UP2, &CShinRefundHjn01::OnBnClickedButtonUp2)
	ON_BN_CLICKED(IDC_BUTTON_DOWN2, &CShinRefundHjn01::OnBnClickedButtonDown2)
END_MESSAGE_MAP()


// CShinRefundHjn01 メッセージ ハンドラ

BOOL CShinRefundHjn01::OnInitDialog()
{
	CRefundBaseDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	SetTitle();
	refundvol.KPType = ID_ICSSH_REF_HOJIN;

	SetDiagControlMap( m_RiyuuDiag.GetDlgCtrlID(), new CDiagCtlRiyuu(m_RiyuuDiag), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_YusyutsuDiag.GetDlgCtrlID(),new CDiagCtlYusyutsu(m_YusyutsuDiag), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_Meisai1Diag.GetDlgCtrlID(), new CDiagCtlTorihiki(m_Meisai1Diag,HOJIN1_LINE_CNT,58,6), new CSyzDataSet(refundvol, KAZEIU_REC, HOJIN1_LINE_CNT, &refundvol.KPKuck,&refundvol.KPKusg ) );
	SetDiagControlMap( m_Meisai2Diag.GetDlgCtrlID(), new CDiagCtlYsMeisai(m_Meisai2Diag,HOJIN2_LINE_CNT,55), new CSyzDataSet(refundvol, YUNYUT_REC, HOJIN2_LINE_CNT ) );
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 3,CRect(NO_MOVE,NO_MOVE,NO_MOVE,58)), pair<int,CRect>( 5,CRect(NO_MOVE,NO_MOVE,NO_MOVE,62)),
								   pair<int,CRect>( 8,CRect(NO_MOVE,NO_MOVE,3,63)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 58,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>( 59,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 60,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>(  3,CRect(8,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 62,CRect(NO_MOVE,5,NO_MOVE,NO_MOVE)), pair<int,CRect>( 63,CRect(NO_MOVE,8,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_RiyuuDiag,m_Meisai1Diag,next,prev);
	}
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 112,CRect(NO_MOVE,NO_MOVE,NO_MOVE,55)), pair<int,CRect>( 113,CRect(NO_MOVE,NO_MOVE,NO_MOVE,55)),
								   pair<int,CRect>( 114,CRect(NO_MOVE,NO_MOVE,NO_MOVE,56)), pair<int,CRect>( 115,CRect(NO_MOVE,NO_MOVE,NO_MOVE,56)),
								   pair<int,CRect>( 116,CRect(NO_MOVE,NO_MOVE,NO_MOVE,57)), pair<int,CRect>( 117,CRect(NO_MOVE,NO_MOVE,55,58)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(  55,CRect(117,112,NO_MOVE,NO_MOVE)),	pair<int,CRect>( 56,CRect(NO_MOVE,115,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>(  57,CRect(NO_MOVE,116,NO_MOVE,NO_MOVE)),pair<int,CRect>( 58,CRect(NO_MOVE,117,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>(  59,CRect(NO_MOVE,117,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai1Diag,m_Meisai2Diag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>( 101,CRect(NO_MOVE,NO_MOVE,NO_MOVE,12)), pair<int,CRect>( 102,CRect(NO_MOVE,NO_MOVE,NO_MOVE,13)),
								   pair<int,CRect>( 103,CRect(NO_MOVE,NO_MOVE,NO_MOVE,14)), pair<int,CRect>( 104,CRect(NO_MOVE,NO_MOVE,12,15)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(  12,CRect(104,101,NO_MOVE,NO_MOVE)),    pair<int,CRect>(  14,CRect(NO_MOVE,103,NO_MOVE,NO_MOVE)), };
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

HBRUSH CShinRefundHjn01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CRefundBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinRefundHjn01::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CRefundBaseDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CShinRefundHjn01::EndJob()
{
	GetDlgItem(IDC_STATIC_HELP)->SetFocus();
}

BOOL CShinRefundHjn01::PreTranslateMessage(MSG* pMsg)
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
	//		((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F7 );
			
		}
		if( pMsg->wParam == VK_F9 ) {
			if(m_pParent){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F9 );
				return( 1 );
			}
	//		((CH26HyoView *)AfxGetMainWnd())->PostMessageA( WM_KEYDOWN, VK_F9 );
		
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
		if( pMsg->wParam == VK_INSERT ) {//terminationに移動
//2016.02.23 UPDATE START
			//KinyuuDlgHjn( 0 );
			//return( 1 );
			if( KinyuuDlgHjn( 0 ) == 0 ){
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
			KinyuuDlgHjn( 1 );
			return( 1 );
		}
	}

	return CRefundBaseDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// 還付先金融機関情報の転記（法人） harino ('14.03.08)
//-----------------------------------------------------------------------------
// 引数	sw			：	0 insertキー
//						1 static
//-----------------------------------------------------------------------------
//2016.02.23 UPDATE START
//void CShinRefundHjn01::KinyuuDlgHjn( int sw )
int CShinRefundHjn01::KinyuuDlgHjn( int sw )
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
void CShinRefundHjn01::ReUpdateData()
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

	SetDiagControlMap( m_RiyuuDiag.GetDlgCtrlID(), new CDiagCtlRiyuu(m_RiyuuDiag), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_YusyutsuDiag.GetDlgCtrlID(),new CDiagCtlYusyutsu(m_YusyutsuDiag), new CSyzDataSet(refundvol) );
	SetDiagControlMap( m_Meisai1Diag.GetDlgCtrlID(), new CDiagCtlTorihiki(m_Meisai1Diag,HOJIN1_LINE_CNT,58,6), new CSyzDataSet(refundvol, KAZEIU_REC, HOJIN1_LINE_CNT, &refundvol.KPKuck,&refundvol.KPKusg ) );
	SetDiagControlMap( m_Meisai2Diag.GetDlgCtrlID(), new CDiagCtlYsMeisai(m_Meisai2Diag,HOJIN2_LINE_CNT,55), new CSyzDataSet(refundvol, YUNYUT_REC, HOJIN2_LINE_CNT ) );

	{	pair<int,CRect> next[] = { pair<int,CRect>( 3,CRect(NO_MOVE,NO_MOVE,NO_MOVE,58)), pair<int,CRect>( 5,CRect(NO_MOVE,NO_MOVE,NO_MOVE,62)),
								   pair<int,CRect>( 8,CRect(NO_MOVE,NO_MOVE,3,63)), };
		pair<int,CRect> prev[] = { pair<int,CRect>( 58,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>( 59,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 60,CRect(NO_MOVE,3,NO_MOVE,NO_MOVE)), pair<int,CRect>(  3,CRect(8,3,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>( 62,CRect(NO_MOVE,5,NO_MOVE,NO_MOVE)), pair<int,CRect>( 63,CRect(NO_MOVE,8,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_RiyuuDiag,m_Meisai1Diag,next,prev);
	}
	
	{	pair<int,CRect> next[] = { pair<int,CRect>( 112,CRect(NO_MOVE,NO_MOVE,NO_MOVE,55)), pair<int,CRect>( 113,CRect(NO_MOVE,NO_MOVE,NO_MOVE,55)),
								   pair<int,CRect>( 114,CRect(NO_MOVE,NO_MOVE,NO_MOVE,56)), pair<int,CRect>( 115,CRect(NO_MOVE,NO_MOVE,NO_MOVE,56)),
								   pair<int,CRect>( 116,CRect(NO_MOVE,NO_MOVE,NO_MOVE,57)), pair<int,CRect>( 117,CRect(NO_MOVE,NO_MOVE,55,58)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(  55,CRect(117,112,NO_MOVE,NO_MOVE)),	pair<int,CRect>( 56,CRect(NO_MOVE,115,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>(  57,CRect(NO_MOVE,116,NO_MOVE,NO_MOVE)),pair<int,CRect>( 58,CRect(NO_MOVE,117,NO_MOVE,NO_MOVE)),
								   pair<int,CRect>(  59,CRect(NO_MOVE,117,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai1Diag,m_Meisai2Diag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>( 101,CRect(NO_MOVE,NO_MOVE,NO_MOVE,12)), pair<int,CRect>( 102,CRect(NO_MOVE,NO_MOVE,NO_MOVE,13)),
								   pair<int,CRect>( 103,CRect(NO_MOVE,NO_MOVE,NO_MOVE,14)), pair<int,CRect>( 104,CRect(NO_MOVE,NO_MOVE,12,15)), };
		pair<int,CRect> prev[] = { pair<int,CRect>(  12,CRect(104,101,NO_MOVE,NO_MOVE)),    pair<int,CRect>(  14,CRect(NO_MOVE,103,NO_MOVE,NO_MOVE)), };
		SetCrossOver(m_Meisai2Diag,m_YusyutsuDiag,next,prev);
	}

	{	pair<int,CRect> next[] = { pair<int,CRect>(  25,CRect(NO_MOVE,NO_MOVE,1,1)) };
		pair<int,CRect> prev[] = { pair<int,CRect>(   1,CRect(25,25,NO_MOVE,NO_MOVE)) };
		SetCrossOver(m_YusyutsuDiag,m_RiyuuDiag,next,prev);
	}
}


//2016.02.23 INSERT START
void CShinRefundHjn01::OnBnClickedButtonDel1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//int clm_num = 6;		//項目数
	//int start_idx = 58;		//最初の項目index
	//int line_increase = 6;	//１行毎のindex増分
	//int line_num = 10;		//行数
	if( GetCurrentDiag() != &m_Meisai1Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai1Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 58 )||( 117 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	//int idx = start_idx;
	//BOOL find_flg = FALSE;
	//for( line_idx = 0; line_idx < line_num; line_idx++ ){
	//	if(( idx <= sel_idx )&&( sel_idx <= idx + clm_num - 1 )){
	//		find_flg = TRUE;
	//		break;
	//	}
	//	idx = start_idx + ((line_idx+1) * line_increase );
	//}
	//if( find_flg == FALSE ){
	//	return;
	//}

	//CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);
	//int line_idx = control->GetItemLine(sel_idx);

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);
	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	//REFUNDREC& refunrec = control->syzdata->GetRec(line_idx);

	//CString	msg;
	//msg.Format( _T("資産の種類等「%s%s」を削除します。よろしいですか？"), refunrec.KPSsup, refunrec.KPSslw );
	//if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
	//	return;
	//}

	//Line_Delete( line_idx );
	//Line_Delete( line_idx, 10, IDC_ICSDIAGCTRL_MEISAI_H1 );

	m_Meisai1Diag.SetPosition(sel_idx);
}

//void CShinRefundHjn01::Line_Delete( int line_idx )
//{
//	//int start_idx = 58;		//最初の項目index
//	//int line_increase = 6;	//１行毎のindex増分
//	int line_num = 10;		//行数
//
//	//int idx = start_idx + (line_increase * line_idx);
//	//m_Meisai1Diag.DataClear(idx++, TRUE);	//資産の種類等
//	//m_Meisai1Diag.DataClear(idx++, TRUE);	//譲渡年月日等
//	//m_Meisai1Diag.DataClear(idx++, TRUE);	//譲渡年月日等　継続
//	//m_Meisai1Diag.DataClear(idx++, TRUE);	//取引金額等
//	//m_Meisai1Diag.DataClear(idx++, TRUE);	//取引先の氏名(名称)
//	//m_Meisai1Diag.DataClear(idx++, TRUE);	//取引先の住所(所在地)
//
//	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);
//
//	REFUNDREC& refunrec0 = control->syzdata->GetRec(line_idx);
//
//
/////////////////////////////////////////////////////////////////////////////
//
//	for( int l_idx = line_idx+1; l_idx < line_num; l_idx++ ){
//
//		REFUNDREC& refunrec = control->syzdata->GetRec(l_idx-1);
//		REFUNDREC& refunrec2 = control->syzdata->GetRec(l_idx);
//
//		memmove( &refunrec, &refunrec2, sizeof(REFUNDREC));
//	}
//
//	REFUNDREC& refunrec3 = control->syzdata->GetRec(line_num-1);
//	::ZeroMemory( &refunrec3, sizeof(REFUNDREC));
//
//	control->DispInit();
//
//	
////	for( int l_idx = line_idx+1; l_idx < line_num; l_idx++ ){
////		idx = start_idx + (line_increase * l_idx);
////
////		CDiagData data;
////		DIAGRAM_ATTRIBUTE attr;
////
////		m_Meisai1Diag.GetData( idx, (LPUNKNOWN)&data );
////		m_Meisai1Diag.GetAttribute( idx, (LPUNKNOWN)&attr );
////		m_Meisai1Diag.SetData( (idx-line_increase), (LPUNKNOWN)&data );
////		m_Meisai1Diag.SetAttribute( (idx-line_increase), (LPUNKNOWN)&attr, TRUE );//資産の種類等
////		control->EditOff(idx-line_increase);
////		idx++;
////		m_Meisai1Diag.GetData( idx, (LPUNKNOWN)&data );
////		m_Meisai1Diag.GetAttribute( idx, (LPUNKNOWN)&attr );
////		m_Meisai1Diag.SetData( (idx-line_increase), (LPUNKNOWN)&data );//譲渡年月日等
////		m_Meisai1Diag.SetAttribute( (idx-line_increase), (LPUNKNOWN)&attr, TRUE );
////		control->EditOff(idx-line_increase);
////		idx++;
////		m_Meisai1Diag.GetData( idx, (LPUNKNOWN)&data );
////		m_Meisai1Diag.GetAttribute( idx, (LPUNKNOWN)&attr );
////		m_Meisai1Diag.SetData( (idx-line_increase), (LPUNKNOWN)&data );//譲渡年月日等　継続
////		m_Meisai1Diag.SetAttribute( (idx-line_increase), (LPUNKNOWN)&attr, TRUE );
////		control->EditOff(idx-line_increase);
////		idx++;
////		m_Meisai1Diag.GetData( idx, (LPUNKNOWN)&data );
////		m_Meisai1Diag.GetAttribute( idx, (LPUNKNOWN)&attr );
////		m_Meisai1Diag.SetData( (idx-line_increase), (LPUNKNOWN)&data );//取引金額等
////		m_Meisai1Diag.SetAttribute( (idx-line_increase), (LPUNKNOWN)&attr, TRUE );
////		control->EditOff(idx-line_increase);
////		idx++;
////		m_Meisai1Diag.GetData( idx, (LPUNKNOWN)&data );
////		m_Meisai1Diag.GetAttribute( idx, (LPUNKNOWN)&attr );
////		m_Meisai1Diag.SetData( (idx-line_increase), (LPUNKNOWN)&data );//取引先の氏名(名称)
////		m_Meisai1Diag.SetAttribute( (idx-line_increase), (LPUNKNOWN)&attr, TRUE );
////		control->EditOff(idx-line_increase);
////		idx++;
////		m_Meisai1Diag.GetData( idx, (LPUNKNOWN)&data );
////		m_Meisai1Diag.GetAttribute( idx, (LPUNKNOWN)&attr );
////		m_Meisai1Diag.SetData( (idx-line_increase), (LPUNKNOWN)&data );//取引先の住所(所在地)
////		m_Meisai1Diag.SetAttribute( (idx-line_increase), (LPUNKNOWN)&attr, TRUE );
////		control->EditOff(idx-line_increase);
////		idx++;
////	}
////	
////	idx = start_idx + ( line_increase * (line_num-1));
////	m_Meisai1Diag.DataClear(idx, TRUE);	//資産の種類等
////	control->EditOff(idx++);
////	m_Meisai1Diag.DataClear(idx, TRUE);	//譲渡年月日等
////	control->EditOff(idx++);
//////	m_Meisai1Diag.DataClear(idx, TRUE);	//譲渡年月日等　継続
////	DIAGRAM_DATA data2;
////	data2.data_check = 0;
////	control->check(idx,&data2);
////	control->EditOff(idx++);
////	m_Meisai1Diag.DataClear(idx, TRUE);	//取引金額等
////	control->EditOff(idx++);
////	m_Meisai1Diag.DataClear(idx, TRUE);	//取引先の氏名(名称)
////	control->EditOff(idx++);
////	m_Meisai1Diag.DataClear(idx, TRUE);	//取引先の住所(所在地)
////	control->EditOff(idx++);
//
//}

void CShinRefundHjn01::OnBnClickedButtonIns1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai1Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai1Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 58 )||( 117 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);
	//int line_idx = control->GetItemLine(sel_idx);

	//Line_Insert( line_idx, 10, IDC_ICSDIAGCTRL_MEISAI_H1  );
	control->Line_Insert(sel_idx);

	m_Meisai1Diag.SetPosition(sel_idx);
}

//void CShinRefundHjn01::Line_Insert( int line_idx )
//{
//	int line_num = 10;		//行数
//
//	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);
//
//	//最終行が空白行か判定
//	REFUNDREC& refunrec = control->syzdata->GetRec(line_num-1);
//	if( strlen( refunrec.KPSsup ) != 0 ){
//		return;
//	}else
//	if( strlen( refunrec.KPSslw ) != 0 ){
//		return;
//	}else
//	if( strlen( refunrec.KPSday ) != 0 ){
//		return;
//	}else
//	if( refunrec.KPSdys ){
//		return;	
//	}else
//	if( strlen( refunrec.KPSval ) != 0 ){
//		return;
//	}else
//	if( strlen( refunrec.KPSnup ) != 0 ){
//		return;
//	}else
//	if( strlen( refunrec.KPSnlw ) != 0 ){
//		return;
//	}else
//	if( strlen( refunrec.KPSaup ) != 0 ){
//		return;
//	}else
//	if( strlen( refunrec.KPSalw ) != 0 ){
//		return;
//	}
//
//
//	for( int l_idx = line_num-1; l_idx > line_idx; l_idx-- ){
//
//		REFUNDREC& refunrec = control->syzdata->GetRec(l_idx);
//		REFUNDREC& refunrec2 = control->syzdata->GetRec(l_idx-1);
//
//		memmove( &refunrec, &refunrec2, sizeof(REFUNDREC));
//	}
//
//	REFUNDREC& refunrec3 = control->syzdata->GetRec(line_idx);
//	::ZeroMemory( &refunrec3, sizeof(REFUNDREC));
//
//	control->DispInit();
//}
//2016.02.23 INSERT END

void CShinRefundHjn01::OnBnClickedButtonDel2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai2Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai2Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 55 )||( 104 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H2);
	//int line_idx = control->GetItemLine(sel_idx);

	//REFUNDREC& refunrec = control->syzdata->GetRec(line_idx);

	//CString	msg;
	//msg.Format( _T("取引先の氏名(名称)「%s%s」を削除します。よろしいですか？"), refunrec.KPSnup, refunrec.KPSnlw );
	//if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
	//	return;
	//}

	//Line_Delete( line_idx );
	//Line_Delete( line_idx, 10, IDC_ICSDIAGCTRL_MEISAI_H2 );

	control->Delete(sel_idx);
	//if( control->Del_Message(sel_idx) == TRUE ){
	//	control->Line_Delete(sel_idx);
	//}

	m_Meisai2Diag.SetPosition(sel_idx);
}

void CShinRefundHjn01::OnBnClickedButtonIns2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( GetCurrentDiag() != &m_Meisai2Diag ){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai2Diag.GetPosition();
	//if( sel_idx == -1 ){
	if(( sel_idx < 55 )||( 104 < sel_idx )){
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H2);
	//int line_idx = control->GetItemLine(sel_idx);

	//Line_Insert( line_idx, 10, IDC_ICSDIAGCTRL_MEISAI_H2  );
	control->Line_Insert(sel_idx);

	m_Meisai2Diag.SetPosition(sel_idx);
}
BEGIN_EVENTSINK_MAP(CShinRefundHjn01, CRefundBaseDialog)
//	ON_EVENT(CShinRefundHjn01, IDC_ICSDIAGCTRL_MEISAI_H1, 1, CShinRefundHjn01::EditONIcsdiagctrlMeisaiH1, VTS_I2)
//	ON_EVENT(CShinRefundHjn01, IDC_ICSDIAGCTRL_MEISAI_H2, 2, CShinRefundHjn01::EditOFFIcsdiagctrlMeisaiH2, VTS_I2)
//ON_EVENT(CShinRefundHjn01, IDC_ICSDIAGCTRL_MEISAI_H2, 1, CShinRefundHjn01::EditONIcsdiagctrlMeisaiH2, VTS_I2)
END_EVENTSINK_MAP()

//void CShinRefundHjn01::EditONIcsdiagctrlMeisaiH1(short index)
//{
//	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);
//	if( control->IsLineHead(index) == TRUE ){
//		GetDlgItem(IDC_BUTTON_DEL1)->SetWindowText("行削除\n(Delete)");
//	}else{
//		GetDlgItem(IDC_BUTTON_DEL1)->SetWindowText("項目削除\n(Delete)");
//	}
//}

//void CShinRefundHjn01::EditOFFIcsdiagctrlMeisaiH2(short index)
//{
//	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H2);
//	if( control->IsLineHead(index) == TRUE ){
//		GetDlgItem(IDC_BUTTON_DEL2)->SetWindowText("行削除\n(Delete)");
//	}else{
//		GetDlgItem(IDC_BUTTON_DEL2)->SetWindowText("項目削除\n(Delete)");
//	}
//}

//void CShinRefundHjn01::EditONIcsdiagctrlMeisaiH2(short index)
//{
//	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H2);
//	if( control->IsLineHead(index) == TRUE ){
//		GetDlgItem(IDC_BUTTON_DEL2)->SetWindowText("行削除\n(Delete)");
//	}else{
//		GetDlgItem(IDC_BUTTON_DEL2)->SetWindowText("項目削除\n(Delete)");
//	}
//}

//----------------------------------------------------
// １行上へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn01::OnBnClickedButtonUp1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai1Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai1Diag.GetPosition();
	if ((sel_idx < 64) || (117 < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}	

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx - 1);

	m_Meisai1Diag.SetPosition(sel_idx - 6);
}

//----------------------------------------------------
// １行下へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn01::OnBnClickedButtonDown1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai1Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai1Diag.GetPosition();
	if ((sel_idx < 58) || (111 < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H1);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx + 1);

	m_Meisai1Diag.SetPosition(sel_idx + 6);
}

//----------------------------------------------------
// １行上へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn01::OnBnClickedButtonUp2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai2Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai2Diag.GetPosition();
	if ((sel_idx < 60) || (104 < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H2);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx - 1);

	m_Meisai2Diag.SetPosition(sel_idx - 5);
}

//----------------------------------------------------
// １行下へ移動 (2020.02.07)
//----------------------------------------------------
void CShinRefundHjn01::OnBnClickedButtonDown2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (GetCurrentDiag() != &m_Meisai2Diag) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	int sel_idx = m_Meisai2Diag.GetPosition();
	if ((sel_idx < 55) || (99 < sel_idx)) {
		GetCurrentDiag()->SetPosition(GetCurrentDiag()->GetPosition());
		return;
	}

	CDiagControlBase* control = GetDiagControl(IDC_ICSDIAGCTRL_MEISAI_H2);

	int row_idx = control->GetItemLine(sel_idx);
	control->SwapTwoRows(row_idx, row_idx + 1);

	m_Meisai2Diag.SetPosition(sel_idx + 5);
}

