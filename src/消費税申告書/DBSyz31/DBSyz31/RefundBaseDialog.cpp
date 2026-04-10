#include "stdafx.h"
#include ".\SyzrefundDB\RefundDataDef.h"
#include "RefundBaseDialog.h"
#include "H31HyoView.h"

REFUNDVOL CRefundBaseDialog::refundvol = {0};
REFUNDHED CRefundBaseDialog::refundhead = {0};

REFUNDREC* CRefundBaseDialog::KAZEIU_REC = 0;
REFUNDREC* CRefundBaseDialog::YUNYUT_REC = 0;
REFUNDREC* CRefundBaseDialog::SHIIRE_REC = 0;
REFUNDREC* CRefundBaseDialog::TANAOR_REC = 0;
REFUNDREC* CRefundBaseDialog::KOTEIS_REC = 0;

CSnHeadData *CRefundBaseDialog::pSnHeadData;
CSnHeadData *CDiagControlBase::pSnHeadData;
CH31SnRefundData *CRefundBaseDialog::pSnRefundData;
//--> '15.02.03 INS START
CSyzUtil	*CRefundBaseDialog::pUtil;
CDBNpSub	*CRefundBaseDialog::pZmSub;
CSyzUtil	*CDiagControlBase::pUtil;
CDBNpSub	*CDiagControlBase::pZmSub;
//--> '15.02.03 INS END

//----> 20230704
//CZmselDB	*CRefundBaseDialog::pZmSel;
CZmselDB	*CDiagControlBase::pZmSel;
//<---- 20230704


inline CString MKString(char* str,int len)
{
	char temp[256] = {0};
	memmove(temp,str,len);

	return temp;
}

// CRefundBaseDialog ダイアログ

IMPLEMENT_DYNAMIC(CRefundBaseDialog, ICSDialog)

CRefundBaseDialog::CRefundBaseDialog(UINT id, CWnd* pParent)
//	: ICSDialog(id, pParent) , arith(0x16) 
	: ICSDialog(id, pParent) 
	, m_pParent(pParent)
{

	bottom_diag  = 0;
	bottom_index = 0;

	m_pagesiz = m_sclmax = 0;

	CurrentDiagID = 0;

	m_wPos = 0;
	m_cmPos = 0;
	m_ScW = 0;

	//----> 20230704
	pZmSel = NULL;
	//<---- 20230704

//2016.02.23 INSERT START
	//LOGBRUSH	lb;
	//lb.lbStyle = BS_SOLID;
	//lb.lbColor = PALETTERGB( 255, 0, 0 );
	//m_hBrushLock.CreateBrushIndirect( &lb );
//2016.02.23 INSERT END
}

CRefundBaseDialog::~CRefundBaseDialog()
{
	//----> 20230704
	if( pZmSel ){
		if( pZmSel->IsDisplay() != FALSE ){
			pZmSel->DialogOFF();
		}
		delete pZmSel;
		pZmSel = NULL;
	}
	//if( CDiagControlBase::pZmSel ){
	//	delete CDiagControlBase::pZmSel;
	//	CDiagControlBase::pZmSel = NULL;
	//}
	//<---- 20230704

	POSITION	pos = DiagControlMap.GetStartPosition();
	while( pos != NULL ){
		int key;
		CDiagControlBase* control = 0;
		DiagControlMap.GetNextAssoc( pos, key, control );
	
		delete control;
	}
	DiagControlMap.RemoveAll();
	
	DestroyWindow();
}

void CRefundBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INPUTLOCK, m_InputLock);
}

BEGIN_MESSAGE_MAP(CRefundBaseDialog, ICSDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_MESSAGE( WM_ZMSEL_MESSAGE, OnShowZmselDialog )
END_MESSAGE_MAP()

BOOL CRefundBaseDialog::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	brush.CreateSolidBrush(HPCOLOR);
	brush2.CreateSolidBrush(BKCOLORG);

	if( !CDiagControlBase::pSnHeadData ){
		CDiagControlBase::pSnHeadData = pSnHeadData; 
	}

//--> '15.02.03 INS START
	if( !CDiagControlBase::pUtil ){
		CDiagControlBase::pUtil = pUtil; 
	}
	if( !CDiagControlBase::pZmSub ){
		CDiagControlBase::pZmSub = pZmSub; 
	}
//<-- '15.02.03 INS END

//2016.02.23 INSERT START
	Make_StaticFont();

	m_InputLock.DefaultColor( FALSE );
	COLORREF color;
	color = PALETTERGB( 255, 0, 0 );
	m_InputLock.SetBackColor(color);
	color = PALETTERGB( 255, 255, 255 );
	m_InputLock.SetTextColor(color);

	//過年度(確定)マスターは非表示
	int id[] =  {IDC_BUTTON_DEL1,IDC_BUTTON_DEL2,IDC_BUTTON_INS1,IDC_BUTTON_INS2};
	for(int cnt = 0;cnt<ARRAYCOUNT(id);cnt++){
		CWnd *item;
		item = GetDlgItem(id[cnt]);

		if( item != NULL ){
			if( pSnHeadData->Sn_Sign4&0x01 ){
			//if( !(pZmSub->zvol->tl_cor & 0x10) ) {
				item->EnableWindow(FALSE);
			}else{
				item->EnableWindow(TRUE);
			}
		}
	}

//2016.02.23 INSERT END

	//----> 20230704
	//pZmSel = new CZmselDB();
	//if( pZmSel != NULL ){
	//	pZmSel->SetDialogResize( FALSE );//科目選択を小さく表示
	//	pZmSel->SetPara( pZmSub, this, NULL,  callback );
	//	CDiagControlBase::pZmSel = pZmSel;	// Hjn01、Hjn02Exと2回通ってしまうため常にセットする
	//}
	//<---- 20230704

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//----> 20230704
void CALLBACK callback( UINT n, void* p, CWnd* pwnd )
{
	CRefundBaseDialog*	pBaseDlg = (CRefundBaseDialog*)pwnd;

	if( n == IDOK ) {
		ZmselDBData* zd = (ZmselDBData*)p;
		
		TRACE( "*** callback IDOK mode = %d, errflg %d\n", zd->selmode, zd->errflg );
		if( zd->selmode == ZSEL_INVNO ){
			if( strlen( zd->sel_name ) > 0 ){
				CIcsdiagctrl* diag = pBaseDlg->GetCurrentDiag();
				CDiagControlBase* control = pBaseDlg->GetDiagControl( pBaseDlg->CurrentDiagID );
				control->CheckInvno( 0, zd );
			}
		}
	}
}
//<---- 20230704

BOOL CRefundBaseDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd *pWnd = GetFocus();

	if( pMsg->message == WM_KEYDOWN ){

		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 ) return TRUE;

		if( ComboBoxTerm((char)pMsg->wParam) ) return TRUE;

		if( (pMsg->wParam==VK_NEXT) || (pMsg->wParam==VK_PRIOR) ){
			//----> 20230704
			if( pZmSel && pZmSel->IsDisplay() != FALSE ) {
				if( pMsg->wParam == VK_NEXT ){
					pZmSel->PageDown();
				}
				if( pMsg->wParam == VK_PRIOR ){
					pZmSel->PageUp();
				}
			}
			//----> 20230704
			else{
				SCROLLINFO	ScrollInfo;
				int	c = GetScrollPos( SB_VERT );
				GetScrollInfo( SB_VERT, &ScrollInfo );
				if( pMsg->wParam == VK_NEXT )
					FIpItemScrolldw( c+ScrollInfo.nPage );
				if( pMsg->wParam == VK_PRIOR )
					FIpItemScrolldw( c-ScrollInfo.nPage );
			}
			return TRUE;
		}
		if( pMsg->wParam == VK_DELETE){
			CIcsdiagctrl* diag = GetCurrentDiag();
			CDiagControlBase* control = GetDiagControl(CurrentDiagID);
			if(diag && control){
				// インボイス登録番号のエラーメッセージが複数回表示されてしまうため、EditOffをコールしない
				// Deleteキーを押下すると削除メッセージに入力した内容が表示されていないことがあったため、
				// 一度SetPosition()を呼ぶ
				//diag->SetPosition(diag->GetPosition());
				control->Termination( diag->GetPosition(), (short)pMsg->wParam);
				diag->SetPosition(diag->GetPosition());
			}
			return 1;
		}
//2016.02.23 INSERT START
		if( pMsg->wParam == VK_INSERT){
			CIcsdiagctrl* diag = GetCurrentDiag();
			CDiagControlBase* control = GetDiagControl(CurrentDiagID);
			if(diag && control){
				// 一度SetPosition()を呼ぶ
				diag->SetPosition(diag->GetPosition());
				control->Termination( diag->GetPosition(), (short)pMsg->wParam);
				diag->SetPosition(diag->GetPosition());
			}
			return 1;
		}
//2016.02.23 INSERT END
		if( pMsg->wParam == VK_END ){
//			((CMainFrame *)AfxGetMainWnd())->PostMessageA( WM_CLOSE, 0 );
//			return 1;

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
/*//shimizu各ダイアログへ移動
		if( pMsg->wParam == VK_INSERT ) {
			VolBankInit();
			SetDispPosition();
			return( 1 );
		}
*///////////
//-->削除 harino ('14.03.03)
//		if( (VK_F1 <= pMsg->wParam) && (pMsg->wParam <= VK_F12) ){
//			if( m_pParent ){
//				m_pParent->PostMessageA( WM_KEYDOWN, pMsg->wParam );
//			}
//			return TRUE;
//		}
//<--削除 harino ('14.03.03)
//-->追加 harino ('14.03.03)
		if( pMsg->wParam == VK_F2 ){
			CIcsdiagctrl* diag = GetCurrentDiag();
			CDiagControlBase* control = GetDiagControl(CurrentDiagID);
			if(diag && control){
				control->Termination( diag->GetPosition(), (short)pMsg->wParam);
			}
			return TRUE;
		}
//<--追加 harino ('14.03.03)
//----> 20230704
		if( pMsg->wParam == VK_ESCAPE ){
			if( pZmSel &&  pZmSel->IsDisplay() != FALSE ){
				pZmSel->DialogOFF();
				return TRUE;
			}
		}
		if( pMsg->wParam == VK_HOME ){
			if( pZmSel && pZmSel->IsDisplay() != FALSE ) {
				pZmSel->PageChange();
				return TRUE;
			}
		}
//<---- 20230704
	}
	else if( pMsg->message == WM_MOUSEWHEEL ) {
		if( pZmSel && pZmSel->IsDisplay() != FALSE ) {
			return TRUE;
		}
	}
	//else if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN) {
	//	if (CheckInvno(1) == false) {
	//		return TRUE;
	//	}
	//}
	return ICSDialog::PreTranslateMessage(pMsg);
}

HBRUSH CRefundBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
//2016.02.23 INSERT START
	//if( GetDlgItem(IDC_STATIC_INPUTLOCK) != NULL ){
	//	if(pWnd->m_hWnd==GetDlgItem(IDC_STATIC_INPUTLOCK)->m_hWnd){
	//		pDC->SetTextColor( RGB(255, 255, 255) );
	//		return (HBRUSH)m_hBrushLock;
	//	}
	//}
//2016.02.23 INSERT END

	if( nCtlColor == CTLCOLOR_STATIC ){
		if( pSnHeadData->Sn_Sign4&0x01 ){//入力ロック時に色変更
			pDC->SetBkColor( BKCOLORG );
			return brush2;
		}
		else{
			pDC->SetBkColor( HPCOLOR );
			return brush;
		}
	}
	else if( nCtlColor == CTLCOLOR_DLG ){
		hbr = m_ShinCom.GetBkClrBrsh();
	}

	return hbr;
}

BOOL CRefundBaseDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, RGB(255, 251, 240));

	return ICSDialog::OnEraseBkgnd(pDC);
}

PBUF PF1[ 30 ];
CStringArray PrintStrArray;

void PrtBufClr( void )
{
	PrintStrArray.RemoveAll();
	for(int cnt = 0; cnt != 30; cnt++ ) {
		pbclr( 0, 40, &PF1[cnt] );
	}
}

char* GetMaruMoji(int flg,int sw = 0)
{
	if( sw ) return flg ? "　" : "○";
	else     return flg ? "○" : "　";
}

CString CRefundBaseDialog::GetMoneyStr(char* val,int zerodisp)
{
	char buf[128] = {0};
	char money[6] = {0}, sen[6] = {0};

	int sw = refundvol.KPType == ID_ICSSH_REF_HOJIN;

	CArith arith(0x16);
	arith.l_input( sen, sw ? "1000" : "1" );
	arith.l_div( money, val, sen );
	
	CString format = sw ? "sss,sss,sss" : "sss,sss,sss,sss";
	if(zerodisp) format.SetAt(format.GetLength()-1, '9');

	arith.l_print(buf,money, (LPSTR)(LPCSTR)format );

	return buf;
}
CString CRefundBaseDialog::GetMoneyStr_EX(char* val,int zerodisp )
{
	char buf[128] = {0};
	char money[6] = {0}, sen[6] = {0};

	CArith arith(0x16);
	arith.l_input( sen, "1000" );
	arith.l_div( money, val, sen );
	
	CString format = "s,sss,sss,sss";
	if(zerodisp) format.SetAt(format.GetLength()-1, '9');

	arith.l_print(buf,money, (LPSTR)(LPCSTR)format );

	return buf;
}
void CRefundBaseDialog::SetTorihikiRecBuf(REFUNDREC* refunrec_data, int line,int check,int sgn, int rectyp)
{
	PrtBufClr();
	PrintStrArray.SetSize(line*4);

	if( check ){
		PF1[0].PITM[0] = GetMaruMoji(sgn);
		PF1[0].PITM[1] = GetMaruMoji(sgn,1);
	}

	for(int i = 0;i<line;i++){

		REFUNDREC& refunrec = refunrec_data[i];

		CString &day1	   = PrintStrArray[i * 4 + 0];
		CString &day2	   = PrintStrArray[i * 4 + 1];
		CString &day3	   = PrintStrArray[i * 4 + 2];
		CString &money_str = PrintStrArray[i * 4 + 3];

		if( refunrec.KPSday[0] ) day1.Format("%2x",(UCHAR)refunrec.KPSday[0]);
		if( refunrec.KPSday[1] ) day2.Format("%2x",(UCHAR)refunrec.KPSday[1]);
		if( refunrec.KPSday[2] ) day3.Format("%2x",(UCHAR)refunrec.KPSday[2]);

		money_str = GetMoneyStr(refunrec.KPSval);

		PF1[1].PITM[i] = refunrec.KPSsup;
		PF1[2].PITM[i] = refunrec.KPSslw;

		if( rectyp == 1 ){
			// (3)固定資産等の取得
			PF1[3].PITM[i] = (LPSTR)(LPCSTR)day1;
			PF1[4].PITM[i] = (LPSTR)(LPCSTR)day2;
			PF1[5].PITM[i] = (LPSTR)(LPCSTR)day3;

			PF1[6].PITM[i] = (LPSTR)(LPCSTR)money_str;
			PF1[7].PITM[i]  = refunrec.KPSnup;
			PF1[8].PITM[i] = refunrec.KPSnlw;
			PF1[9].PITM[i] = refunrec.KPSaup;
			PF1[10].PITM[i] = refunrec.KPSalw;
		}
		else if( rectyp == 2 ){
			// 棚卸資産　取引先の登録番号列あり
			if(!refunrec.KPSdys){
				PF1[3].PITM[i] = (LPSTR)(LPCSTR)day1;
				PF1[4].PITM[i] = (LPSTR)(LPCSTR)day2;
				PF1[5].PITM[i] = (LPSTR)(LPCSTR)day3;
			}else{
				PF1[6].PITM[i] = "継続";
			}

			PF1[7].PITM[i] = (LPSTR)(LPCSTR)money_str;
			PF1[8].PITM[i]  = refunrec.KPInvno;
			PF1[9].PITM[i]  = refunrec.KPSnup;
			PF1[10].PITM[i] = refunrec.KPSnlw;
			PF1[11].PITM[i] = refunrec.KPSaup;
			PF1[12].PITM[i] = refunrec.KPSalw;		
		}
		else if( rectyp == 3 ){
			// 固定資産　取引先の登録番号列あり
			PF1[3].PITM[i] = (LPSTR)(LPCSTR)day1;
			PF1[4].PITM[i] = (LPSTR)(LPCSTR)day2;
			PF1[5].PITM[i] = (LPSTR)(LPCSTR)day3;

			PF1[6].PITM[i] = (LPSTR)(LPCSTR)money_str;
			PF1[7].PITM[i]  = refunrec.KPInvno;
			PF1[8].PITM[i]  = refunrec.KPSnup;
			PF1[9].PITM[i] = refunrec.KPSnlw;
			PF1[10].PITM[i] = refunrec.KPSaup;
			PF1[11].PITM[i] = refunrec.KPSalw;
		}
		else{
			if(!refunrec.KPSdys){
				PF1[3].PITM[i] = (LPSTR)(LPCSTR)day1;
				PF1[4].PITM[i] = (LPSTR)(LPCSTR)day2;
				PF1[5].PITM[i] = (LPSTR)(LPCSTR)day3;
			}else{
				PF1[6].PITM[i] = "継続";
			}

			PF1[7].PITM[i] = (LPSTR)(LPCSTR)money_str;
			PF1[8].PITM[i]  = refunrec.KPSnup;
			PF1[9].PITM[i] = refunrec.KPSnlw;
			PF1[10].PITM[i] = refunrec.KPSaup;
			PF1[11].PITM[i] = refunrec.KPSalw;
		}
	}
}

void CRefundBaseDialog::SetYsMeisaiRecBuf(REFUNDREC* refunrec_data,int line)
{
	PrtBufClr();
	PrintStrArray.SetSize(line);

	for(int i = 0;i<line;i++){
		REFUNDREC& refunrec = refunrec_data[i];

		CString &money_str = PrintStrArray[i];
		money_str = GetMoneyStr(refunrec.KPSval);
		PF1[0].PITM[i] = refunrec.KPSnup;
		PF1[1].PITM[i] = refunrec.KPSnlw;

		PF1[2].PITM[i] = refunrec.KPSaup;
		PF1[3].PITM[i] = refunrec.KPSalw;

		PF1[4].PITM[i] = (LPSTR)(LPCSTR)money_str;

		PF1[5].PITM[i] = refunrec.KPStup;
		PF1[6].PITM[i] = refunrec.KPStlw;

		PF1[7].PITM[i] = refunrec.KPSzup;
		PF1[8].PITM[i] = refunrec.KPSzlw;
	}
}

void CRefundBaseDialog::SetShiireRecBuf(REFUNDREC* refunrec_data, int line,int kei[] , int kojin)
{
	PrtBufClr();
	PrintStrArray.SetSize(line*3);

	for(int i = 0;i<line;i++){
		REFUNDREC& refunrec = refunrec_data[i];

		CString &money_str1 = PrintStrArray[i * 3 + 0];
		CString &money_str2 = PrintStrArray[i * 3 + 1];
		CString &money_str3 = PrintStrArray[i * 3 + 2];

		if( kojin ){
			money_str1 = GetMoneyStr(refunrec.KPSvl1);
			money_str2 = GetMoneyStr(refunrec.KPSvl2);
			money_str3 = GetMoneyStr( kei[i] == 2 && refundvol.KPSstx ? refunrec.KPSvl4 : refunrec.KPSvl3, kei[i] );
		}
		else{
			money_str1 = GetMoneyStr_EX(refunrec.KPSvl1);
			money_str2 = GetMoneyStr_EX(refunrec.KPSvl2);
			money_str3 = GetMoneyStr_EX( kei[i] == 2 && refundvol.KPSstx ? refunrec.KPSvl4 : refunrec.KPSvl3, kei[i] );
/*			if( kei[i] == 2 ){
				if( pSnHeadData->Sn_Sign4&0x80 ){//非連動
					money_str3 = GetMoneyStr_EX( refundvol.KPSstx ? refunrec.KPSvl4 : refunrec.KPSvl3, kei[i] );
				}
				else{
					if( 
					money_str3 = GetMoneyStr_EX( refunrec.KPSvl4 , kei[i] );
				}
			}
			else{
				money_str3 = GetMoneyStr_EX( refunrec.KPSvl3, kei[i] );
			}
*/		}
		if(!kei[i]){
			PF1[i].PITM[0] = (LPSTR)(LPCSTR)money_str1;
			PF1[i].PITM[1] = (LPSTR)(LPCSTR)money_str2;
		}
		PF1[i].PITM[2] = (LPSTR)(LPCSTR)money_str3;
	}
}

void CRefundBaseDialog::SetTokusyuBuf()
{
	PrtBufClr();

	PF1[1].PITM[0] = refundvol.KPSpn1;
	PF1[1].PITM[1] = refundvol.KPSpn2;
	PF1[1].PITM[2] = refundvol.KPSpn3;
	PF1[1].PITM[3] = refundvol.KPSpn4;
	PF1[1].PITM[4] = refundvol.KPSpn5;
	PF1[1].PITM[5] = refundvol.KPSpn6;

	if(refundvol.KPType == ID_ICSSH_REF_KOJIN){
		PrintStrArray.SetSize(1);
		int nen = (UCHAR)refundvol.KPSnen;
		if( nen ) PrintStrArray[0].Format("%2x",nen);
		PF1[0].PITM[0] = (LPSTR)(LPCSTR)PrintStrArray[0];
	}
}

void CRefundBaseDialog::SetRiyuuBuf()
{
	PrtBufClr();
	
	PF1[0].PITM[0] = GetMaruMoji(refundvol.KPRsgn&0x01);
	PF1[1].PITM[0] = GetMaruMoji(refundvol.KPRsgn&0x02);
	PF1[2].PITM[0] = GetMaruMoji(refundvol.KPRsgn&0x04);

	if(refundvol.KPRsgn&0x04){
		PF1[3].PITM[0] = refundvol.KPRrup;
		PF1[3].PITM[1] = refundvol.KPRrlw;
	}
}

void CRefundBaseDialog::SetTitleBuf()
{
	PrtBufClr();
	PrintStrArray.SetSize(5);

	CString s,e;
	char	yy, mm, dd;
	yy = mm = dd = 0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	PrintStrArray[0].Format( "%02x  %02x  %02x", yy, mm, dd );
	yy = mm = dd = 0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	PrintStrArray[1].Format( "%02x  %02x  %02x", yy, mm, dd );

	PrintStrArray[2] = MKString((char*)&pSnHeadData->Sn_NOZEI[0],40);
	PrintStrArray[3] = MKString((char*)&pSnHeadData->Sn_NOZEI[40],40);

//★//[12'02.01]///
	if(refundvol.KPType == ID_ICSSH_REF_KOJIN)
			PrintStrArray[4] = MKString((char*)&pSnHeadData->Sn_DIHYO[0],40);
	else	PrintStrArray[4] = MKString((char*)&pSnHeadData->Sn_CONAM[0],40);
///////////////////

	PF1[0].PITM[0] = (LPSTR)(LPCSTR)PrintStrArray[0];
	PF1[0].PITM[1] = (LPSTR)(LPCSTR)PrintStrArray[1];
	PF1[1].PITM[0] = (LPSTR)(LPCSTR)PrintStrArray[2];
	PF1[1].PITM[1] = (LPSTR)(LPCSTR)PrintStrArray[3];
	PF1[2].PITM[0] = (LPSTR)(LPCSTR)PrintStrArray[4];
}

void CRefundBaseDialog::SetYusyutsuBuf()
{
	PrtBufClr();
	PrintStrArray.SetSize(1);

	PF1[0].PITM[0] = refundvol.KPEbnk;
	PF1[1].PITM[0] = refundvol.KPEstn;
	PF1[2].PITM[0] = refundvol.KPEykn;

	if(refundvol.KPEbdm){
		CString yu1 = MKString(refundvol.KPEyn1,5);
		CString yu2 = MKString(refundvol.KPEyn2,8);

		if( yu1.GetLength() || yu2.GetLength() ){
			PrintStrArray[0].Format("%s - %s",yu1,yu2);
			PF1[3].PITM[0] = (LPSTR)(LPCSTR)PrintStrArray[0];
		}
	}else{
		char kozacode[128] = {0};
		CArith arith(0x16);
		if(refundvol.KPEkoz[0] != (char)0xff){
			arith.l_unpac(kozacode,refundvol.KPEkoz,10);
			PrintStrArray[0] = kozacode;
			PF1[3].PITM[0] = (LPSTR)(LPCSTR)PrintStrArray[0];
		}
	}

	if( !CString(refundvol.KPEslw).GetLength() ) {
		PF1[4].PITM[0] = refundvol.KPEsup;
	}else{
		PF1[5].PITM[0] = refundvol.KPEsup;
		PF1[5].PITM[1] = refundvol.KPEslw;
	}

	PF1[6].PITM[0] = refundvol.KPEaup;
	PF1[6].PITM[1] = refundvol.KPEalw;

	char* sp8  = "　　　　　　　　"; char* sp4  = "　　　　";
	char* nosp = "";

	PF1[7].PITM[0]  = (refundvol.KPEbsg == 0) ? sp8 : nosp;
	PF1[8].PITM[0]  = (refundvol.KPEbsg == 1) ? sp4 : nosp;
	PF1[9].PITM[0]  = (refundvol.KPEbsg == 2) ? sp4 : nosp;
	PF1[10].PITM[0] = (refundvol.KPEbsg == 3) ? sp4 : nosp;
	PF1[11].PITM[0] = (refundvol.KPEbsg == 4) ? sp4 : nosp;

	PF1[12].PITM[0] = (refundvol.KPEssg == 0) ? sp4 : nosp;
	PF1[13].PITM[0] = (refundvol.KPEssg == 1) ? sp4 : nosp;
	PF1[14].PITM[0] = (refundvol.KPEssg == 4) ? sp8 : nosp;
	PF1[15].PITM[0] = (refundvol.KPEssg == 2) ? sp4 : nosp;
	PF1[16].PITM[0] = (refundvol.KPEssg == 3) ? sp4 : nosp;
}

void CRefundBaseDialog::HojinPrint(CDC* pDC,int pno)
{
//	int	sno = 10;
	int	sno = 49;

	if(pno == 1){
		SetTitleBuf();
		_xprn( pno+sno, 1 , PF1, 0, pDC, 0, 0x0001 );

		SetRiyuuBuf();
		_xprn( pno+sno, 2 , PF1, 0, pDC, 0, 0x0001 );

		SetTorihikiRecBuf( KAZEIU_REC, HOJIN1_LINE_CNT, refundvol.KPKuck, refundvol.KPKusg );
		_xprn( pno+sno, 3 , PF1, 0, pDC, 0, 0x0001 );

		SetYsMeisaiRecBuf( YUNYUT_REC, HOJIN2_LINE_CNT );
		_xprn( pno+sno, 4 , PF1, 0, pDC, 0, 0x0001 );

		SetYusyutsuBuf();
		_xprn( pno+sno, 5 , PF1, 0, pDC, 0, 0x0001 );

	}else{

		//----> 20230704
		int eymd = 0;
		if( !( pSnHeadData->Sn_SKKBN % 2 ))		eymd = pSnHeadData->Sn_MDAYE;
		else									eymd = pSnHeadData->Sn_KDAYE;

		if( eymd >= ICS_SH_INVOICE_DAY ){
			// インボイス改正版は#53で追加したため＋2する
			int kei[] = {0,0,0,0,0,0,0,0,0,2};
			SetShiireRecBuf(SHIIRE_REC, HOJIN_SHIRE_CNT, kei , 0);

			PF1[10].PITM[0] = GetMaruMoji(refundvol.KPSsgn&0x01);
			PF1[10].PITM[1] = GetMaruMoji(refundvol.KPSsgn&0x01,1);
			PF1[11].PITM[0] = GetMaruMoji(refundvol.KPSsgn&0x10);
			PF1[11].PITM[1] = GetMaruMoji(refundvol.KPSsgn&0x10,1);
			_xprn( pno+sno+2, 1 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( TANAOR_REC, HOJIN3_LINE_CNT, refundvol.KPTsck, refundvol.KPTsgn, 2 );
			_xprn( pno+sno+2, 2 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( KOTEIS_REC, HOJIN4_LINE_CNT, refundvol.KPKsck, refundvol.KPKsgn, 3 );
			_xprn( pno+sno+2, 3 , PF1, 0, pDC, 0, 0x0001 );

			SetTokusyuBuf();
			_xprn( pno+sno+2, 4 , PF1, 0, pDC, 0, 0x0001 );
		}
		//<---- 20230704
		else{
			int kei[] = {0,0,0,0,0,0,0,0,0,2};
			SetShiireRecBuf(SHIIRE_REC, HOJIN_SHIRE_CNT, kei , 0);

			PF1[10].PITM[0] = GetMaruMoji(refundvol.KPSsgn&0x01);
			PF1[10].PITM[1] = GetMaruMoji(refundvol.KPSsgn&0x01,1);
			PF1[11].PITM[0] = GetMaruMoji(refundvol.KPSsgn&0x10);
			PF1[11].PITM[1] = GetMaruMoji(refundvol.KPSsgn&0x10,1);
			_xprn( pno+sno, 1 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( TANAOR_REC, HOJIN3_LINE_CNT, refundvol.KPTsck, refundvol.KPTsgn );
			_xprn( pno+sno, 2 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( KOTEIS_REC, HOJIN4_LINE_CNT, refundvol.KPKsck, refundvol.KPKsgn, 1 );
			_xprn( pno+sno, 3 , PF1, 0, pDC, 0, 0x0001 );

			SetTokusyuBuf();
			_xprn( pno+sno, 4 , PF1, 0, pDC, 0, 0x0001 );		
		}
	}
}

void CRefundBaseDialog::KojinPrint(CDC* pDC,int pno)
{
//	int	sno = 12;
	int	sno = 49;

	if(pno == 1){
		SetTitleBuf();
		_xprn( pno+sno, 1 , PF1, 0, pDC, 0, 0x0001 );

		SetRiyuuBuf();
		_xprn( pno+sno, 2 , PF1, 0, pDC, 0, 0x0001 );

		SetTorihikiRecBuf( KAZEIU_REC, KOJIN1_LINE_CNT, refundvol.KPKuck, refundvol.KPKusg );
		_xprn( pno+sno, 3 , PF1, 0, pDC, 0, 0x0001 );

		SetYsMeisaiRecBuf( YUNYUT_REC, KOJIN2_LINE_CNT );
		_xprn( pno+sno, 4 , PF1, 0, pDC, 0, 0x0001 );

		SetYusyutsuBuf();
		_xprn( pno+sno, 5 , PF1, 0, pDC, 0, 0x0001 );

	}else{

		//----> 20230704
		int eymd = 0;
		if( !( pSnHeadData->Sn_SKKBN % 2 ))		eymd = pSnHeadData->Sn_MDAYE;
		else									eymd = pSnHeadData->Sn_KDAYE;

		if( eymd >= ICS_SH_INVOICE_DAY ){

			// インボイス改正版は#53で追加したため＋2する
			PrtBufClr();
			int kei[] = {0,0,0,0,0,0,0,0,0,0,0,1,2};
			SetShiireRecBuf(SHIIRE_REC, KOJIN_SHIRE_CNT, kei , 1);
			PF1[14].PITM[0] = GetMaruMoji(refundvol.KPSsgn&0x01);
			PF1[14].PITM[1] = GetMaruMoji(refundvol.KPSsgn&0x01,1);
			int size = (int)PrintStrArray.GetSize();

			char	Str[20] = {0};
			memset( Str, '\0', sizeof( Str ) );
			CSyzUtil	m_Util;
			int Length = MixSpaceCutLength_EX( (char *)refundvol.KPSnam, 16 );
			if( Length > 4) {
				for(int i=0; i< 8; i++ ){
					char buf[10] = {0};
					memmove( buf, &refundvol.KPSnam[i*2], 2 );
					PrintStrArray.Add(buf);
					PF1[15].PITM[i] = (LPSTR)(LPCSTR)PrintStrArray[size+i];
				}
			}
			_xprn( pno+sno+2, 1 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( TANAOR_REC, KOJIN3_LINE_CNT, refundvol.KPTsck, refundvol.KPTsgn, 2 );
			_xprn( pno+sno+2, 2 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( KOTEIS_REC, KOJIN4_LINE_CNT, refundvol.KPKsck, refundvol.KPKsgn, 3 );
			_xprn( pno+sno+2, 3 , PF1, 0, pDC, 0, 0x0001 );

			SetTokusyuBuf();
			_xprn( pno+sno+2, 4 , PF1, 0, pDC, 0, 0x0001 );

			if( Length <= 4 ) {
				PrtBufClr();
				for(int i=0; i < 2; i++ ){
					char buf[16] = {0};
					memmove( buf, &refundvol.KPSnam[i*2], 2 );
					PrintStrArray.Add(buf);
					PF1[0].PITM[i] = (LPSTR)(LPCSTR)PrintStrArray[i];
				}
				_xprn( pno+sno+2, 5 , PF1, 0, pDC, 0, 0x0001 );

			}
		}
		//<---- 20230704
		else{
			PrtBufClr();
			int kei[] = {0,0,0,0,0,0,0,0,0,0,0,1,2};
			SetShiireRecBuf(SHIIRE_REC, KOJIN_SHIRE_CNT, kei , 1);
			PF1[14].PITM[0] = GetMaruMoji(refundvol.KPSsgn&0x01);
			PF1[14].PITM[1] = GetMaruMoji(refundvol.KPSsgn&0x01,1);
			int size = (int)PrintStrArray.GetSize();

			char	Str[20] = {0};
			memset( Str, '\0', sizeof( Str ) );
			CSyzUtil	m_Util;
			int Length = MixSpaceCutLength_EX( (char *)refundvol.KPSnam, 16 );
			if( Length > 4) {
				for(int i=0; i< 8; i++ ){
					char buf[10] = {0};
					memmove( buf, &refundvol.KPSnam[i*2], 2 );
					PrintStrArray.Add(buf);
					PF1[15].PITM[i] = (LPSTR)(LPCSTR)PrintStrArray[size+i];
				}
			}
			_xprn( pno+sno, 1 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( TANAOR_REC, KOJIN3_LINE_CNT, refundvol.KPTsck, refundvol.KPTsgn );
			_xprn( pno+sno, 2 , PF1, 0, pDC, 0, 0x0001 );

			SetTorihikiRecBuf( KOTEIS_REC, KOJIN4_LINE_CNT, refundvol.KPKsck, refundvol.KPKsgn, 1 );
			_xprn( pno+sno, 3 , PF1, 0, pDC, 0, 0x0001 );

			SetTokusyuBuf();
			_xprn( pno+sno, 4 , PF1, 0, pDC, 0, 0x0001 );

			if( Length <= 4 ) {
				PrtBufClr();
				for(int i=0; i < 2; i++ ){
					char buf[16] = {0};
					memmove( buf, &refundvol.KPSnam[i*2], 2 );
					PrintStrArray.Add(buf);
					PF1[0].PITM[i] = (LPSTR)(LPCSTR)PrintStrArray[i];
				}
				_xprn( pno+sno, 5 , PF1, 0, pDC, 0, 0x0001 );

			}		
		}
	}
}
//	漢字文字数　検索
int	CRefundBaseDialog::MixSpaceCutLength_EX( char *buf, int lng )
{
	int		i;

	i = lng - 1;
	for( ; i >= 0; i-- ) {
		if( (i) && (buf[i-1]&0xff) == 0x81 && (buf[i]&0xff) == 0x40 )	i--;
		else	{
			if( (buf[i]&0xff) == 0x00 || (buf[i]&0xff) == 0x20 )	continue;
			else	break;
		}
	}
	if ( i >= 0 )	return( i + 1 );
	else			return(0);
}


CIcsdiagctrl* CRefundBaseDialog::GetCurrentDiag()
{
	return (CIcsdiagctrl*)GetDlgItem(CurrentDiagID);
}

CDiagControlBase* CRefundBaseDialog::GetDiagControl(UINT CtlID)
{
	CDiagControlBase* control = 0;
	DiagControlMap.Lookup(CtlID,control);
	if( !control ) return 0;

	return control;
}

void CRefundBaseDialog::FIpItemScrolldw( int mpos )
{
	int smax = GetScrollLimit( SB_VERT );
	if ( mpos < 0 )		mpos = 0;
	if ( mpos > smax )	mpos = smax;

	WPARAM	wParam;
	wParam = mpos << 16;
	wParam |= (int)SB_THUMBTRACK;
	PostMessage( WM_VSCROLL, wParam );
}

void CRefundBaseDialog::SetActivePosition()
{
	CIcsdiagctrl* diag = GetCurrentDiag();
	if( !diag ) return;

	diag->SetPosition( diag->GetPosition() );
}

int CRefundBaseDialog::ComboBoxTerm(char nChar)
{
	CIcsdiagctrl* diag = GetCurrentDiag();
	if( !diag ) return 0;

	CDiagControlBase* control = GetDiagControl( diag->GetDlgCtrlID() );
	if( !control ) return 0;

	if( control->IsCombobox( diag->GetPosition() ) )
		return control->Termination( diag->GetPosition(), nChar);

	return 0;
}

void CRefundBaseDialog::SetScroll(UINT CtlID,short index)
{
	CIcsdiagctrl* diag = (CIcsdiagctrl*)GetDlgItem(CtlID);
	if( !diag ) return;

	RECT	a, b;
	GetWindowRect( &a );
	diag->GetRect(index,(LPUNKNOWN)&b);

	if( (a.top > b.top) || (a.bottom < b.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		FIpItemScrolldw( (cpos+b.top-a.top)-((a.bottom-a.top)/2) );
	}
}

int CRefundBaseDialog::Termination(UINT CtlID,int index,char nChar)
{
	CDiagControlBase* control = GetDiagControl( CtlID );
	if( !control ) return 0;

	return control->Termination(index, nChar);
}

void CRefundBaseDialog::SetDiagControlMap(UINT CtlID, CDiagControlBase* diagctl, CSyzDataSet* syzdata)
{
	DiagControlMap.SetAt( CtlID, diagctl);
	if( !syzdata ) syzdata = new CSyzDataSet(refundvol);
	
	diagctl->SetSyzData(syzdata);
	diagctl->DispInit();
}

void CRefundBaseDialog::SetTitle() 
{
	CIcsdiagctrl* diag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAGCTRL_TITLE);
	if( !diag ) return;

	CDiagData DD;

	// 課税期間
	CString s,e;
	char	yy, mm, dd;
	yy = mm = dd = 0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	s.Format( "%02x.%02x.%02x", yy, mm, dd );
	yy = mm = dd = 0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	e.Format( "%02x.%02x.%02x", yy, mm, dd );
	DD.data_disp.Format("　%s　～　%s　",s,e);
	diag->SetData(3,DD);

	// 所在地
	DD.data_disp = MKString((char*)&pSnHeadData->Sn_NOZEI[0],40) + "\n" + MKString((char*)&pSnHeadData->Sn_NOZEI[40],40);
	diag->SetData(6,DD);

	// 名称
//★//[12'02.01]///
	if(refundvol.KPType == ID_ICSSH_REF_KOJIN)
			DD.data_disp = MKString((char*)&pSnHeadData->Sn_DIHYO[0],40);
	else	DD.data_disp = MKString((char*)&pSnHeadData->Sn_CONAM[0],40);
///////////////////
	diag->SetData(7,DD);

}

CString GetSchPath()
{
	char path[_MAX_PATH];
	
	::GetModuleFileName(0,path,sizeof path);
	TCHAR drive[_MAX_DRIVE];	TCHAR dir[_MAX_DIR];	TCHAR fname[_MAX_FNAME];	TCHAR ext[_MAX_EXT];
	_splitpath_s(path,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,_MAX_FNAME,ext,_MAX_EXT);

	_makepath_s(path,_MAX_PATH,drive,dir,"","");

	return path;
}

#ifdef _DEBUG

void CRefundBaseDialog::VolRecFill() 
{
	refundvol.KPEbsg = 0;
	refundvol.KPEssg = 0;

	refundvol.KPKuck = 1;
	refundvol.KPTsck = 1;
	refundvol.KPKsck = 1;
	refundvol.KPKusg = 1;
	refundvol.KPTsgn = 1;
	refundvol.KPKsgn = 1;

	refundvol.KPRsgn = 0x07;
	refundvol.KPSsgn = 0x09;
	
	strcpy_s(refundvol.KPEbnk,"銀行名あああああああ");
	strcpy_s(refundvol.KPEstn,"支店名あああああああ");
	strcpy_s(refundvol.KPEykn,"預金名あ");

	strcpy_s(refundvol.KPEsup,"主な通関業者氏名上段ああああああああああ");
	strcpy_s(refundvol.KPEslw,"主な通関業者氏名下段ああああああああああ");
	strcpy_s(refundvol.KPEaup,"主な通関業者住所上段ああああああああああ");
	strcpy_s(refundvol.KPEalw,"主な通関業者住所上段ああああああああああ");

	strcpy_s(refundvol.KPSpn1,"特殊事情１あああああああああああああああああああああああああ特殊事情１あああああああああああああああああああああああああ");
	strcpy_s(refundvol.KPSpn2,"特殊事情２あああああああああああああああああああああああああ特殊事情２あああああああああああああああああああああああああ");
	strcpy_s(refundvol.KPSpn3,"特殊事情３あああああああああああああああああああああああああ特殊事情３あああああああああああああああああああああああああ");
	strcpy_s(refundvol.KPSpn4,"特殊事情４あああああああああああああああああああああああああ特殊事情４あああああああああああああああああああああああああ");
	strcpy_s(refundvol.KPSpn5,"特殊事情５あああああああああああああああああああああああああ特殊事情５あああああああああああああああああああああああああ");
	strcpy_s(refundvol.KPSpn6,"特殊事情６あああああああああああああああああああああああああ特殊事情６あああああああああああああああああああああああああ");

	strcpy_s(refundvol.KPRrup,"その他の理由上段ああああああああああああ");
	strcpy_s(refundvol.KPRrlw,"その他の理由下段ああああああああああああ");
	
	if( refundvol.KPType == ID_ICSSH_REF_KOJIN ){
		strcpy_s(refundvol.KPSnam,"ああああああああ");
		refundvol.KPSnen =(char) 0x99;
	}

	refundvol.KPEkoz[0] = (char)0x12;
	refundvol.KPEkoz[1] = (char)0x34;
	refundvol.KPEkoz[2] = (char)0x56;
	refundvol.KPEkoz[3] = (char)0x78;
	refundvol.KPEkoz[4] = (char)0x90;

	REFUNDREC** rec[] = { &KAZEIU_REC,&YUNYUT_REC,&TANAOR_REC,&KOTEIS_REC,&SHIIRE_REC };

	int hline_min_cnt[] = { HOJIN1_LINE_CNT,HOJIN2_LINE_CNT,HOJIN3_LINE_CNT,HOJIN4_LINE_CNT,HOJIN_SHIRE_CNT };
	int kline_min_cnt[] = { KOJIN1_LINE_CNT,KOJIN2_LINE_CNT,KOJIN3_LINE_CNT,KOJIN4_LINE_CNT,KOJIN_SHIRE_CNT };

	int *line_min = refundvol.KPType == ID_ICSSH_REF_HOJIN ? hline_min_cnt : kline_min_cnt;

	for(int i = 0;i<5;i++){
	
		REFUNDREC* refunrec_data = *rec[i];
		memset(*rec[i],0,sizeof(REFUNDREC) * line_min[i]);
		
		for(int j = 0;j<line_min[i];j++){

			REFUNDREC& refunrec = refunrec_data[j];
	
			if(i == 1){
				strcpy_s(refunrec.KPSzup,"所轄税関名上段あ");
				strcpy_s(refunrec.KPSzlw,"所轄税関名下段あ");
				strcpy_s(refunrec.KPStup, "主な取引商品上段ああ");
				strcpy_s(refunrec.KPStlw, "主な取引商品下段ああ");
			}

			if(!(i == 1 || i == 4 ))
				refunrec.KPSday[0] = refunrec.KPSday[1] = refunrec.KPSday[2] = 0x01;

			arith.l_input( refunrec.KPSvl1," 99999999000");
			arith.l_input( refunrec.KPSvl2,"  9999999000");
			arith.l_input( refunrec.KPSval," 99999999000");

			if(i != 4){
				strcpy_s(refunrec.KPSsup,"種類上段ああああ");
				strcpy_s(refunrec.KPSslw,"種類下段ああああ");
				strcpy_s(refunrec.KPSnup,"氏名上段ああああああ");
				strcpy_s(refunrec.KPSnlw,"氏名下段ああああああ");
				strcpy_s(refunrec.KPSaup, "住所上段あああああああああああ");
				strcpy_s(refunrec.KPSalw, "住所下段あああああああああああ");
			}
		}
	}
}

#endif
//sw追加
//0:個人１1:法人１2:法人3:個人2
void CRefundBaseDialog::SetInitPosition( int sw )
{
	//-->追加 harino ('14.03.07)
	// 前回ポジションの復元
//-- '15.01.27 --
//	CIcsdiagctrl* diag = GetCurrentDiag();
//	int id = diag->GetDlgCtrlID();
//	GetDlgItem(IDC_ICSDISPCTRL_REF1)->SetFocus();
//	diag->SetFocus();
//	CDiagControlBase* control = GetDiagControl(CurrentDiagID);
//	if(diag && control){
//		diag->SetPosition(diag->GetPosition());
//		return;
//	}
//---------------
	CIcsdiagctrl* diag = GetCurrentDiag();
	if( diag != NULL ){
		int id = diag->GetDlgCtrlID();
		GetDlgItem(IDC_ICSDISPCTRL_REF1)->SetFocus();
		diag->SetFocus();
		CDiagControlBase* control = GetDiagControl(CurrentDiagID);
		if( control != NULL ){
			diag->SetPosition(diag->GetPosition());
			return;
		}
	}
//---------------
	//<--追加 harino ('14.03.07)

	POSITION	pos = DiagControlMap.GetStartPosition();
	while( pos != NULL ){
		int key;
		CDiagControlBase* control = 0;

		GetDlgItem(IDC_ICSDISPCTRL_REF1)->SetFocus();

		if( !(pSnHeadData->Sn_Sign4&0x80) && ( sw == 3) ){
			DiagControlMap.Lookup(IDC_ICSDIAGCTRL_MEISAI_H3,control);
			pos = 0;
		}
		else{
			if( sw == 2 ){
				DiagControlMap.Lookup(IDC_ICSDIAGCTRL_SHIIRE_KO,control);
				pos = 0;
			}
			else{
				DiagControlMap.GetNextAssoc( pos, key, control);
			}
		}
		control->SetInitPos();
	}
}

void CRefundBaseDialog::SetDispPosition(int sw)
{
	//----> 20230802 del 修正依頼162932
	/*
	POSITION	pos = DiagControlMap.GetStartPosition();
	while( pos != NULL ){
		int key;
		CDiagControlBase* control = 0;
//2016.02.23 UPDATE START
		//if( !(pSnHeadData->Sn_Sign4&0x80) && ( sw == 3) ){
		//	DiagControlMap.Lookup(IDC_ICSDIAGCTRL_MEISAI_H3,control);
		//	pos = 0;
		//}
		//else{
		//	DiagControlMap.GetNextAssoc( pos, key, control);
		//}

		DiagControlMap.GetNextAssoc( pos, key, control);

		if( !(pSnHeadData->Sn_Sign4&0x80) && ( sw == 3) ){
			if( control == GetDiagControl(IDC_ICSDIAGCTRL_SHIIRE_HO)){
				continue;
			}
		}
	//2016.02.23 UPDATE END
		control->DispInit();
	}
	*/
	//<---- 20230802 del 修正依頼162932
	//----> 20230802 add 修正依頼162932
	//GetNextAssoc()は要素へのアクセス順が不定であるためLookupに変更
	//要素へのアクセスは派生クラスのReUpdateData()/OnInitDialog()でDiagControlMapに追加する順に行う
	for (auto &itr : diagKeyVec) {
		CDiagControlBase* control = 0;
		DiagControlMap.Lookup(itr, control);
		if (control == NULL) { continue; }

		if (!(pSnHeadData->Sn_Sign4 & 0x80) && (sw == 3)) {
			if (control == GetDiagControl(IDC_ICSDIAGCTRL_SHIIRE_HO)) {
				continue;
			}
		}
		control->DispInit();
	}
	//<---- 20230802 add 修正依頼162932
}

// 初期設定
void CRefundBaseDialog::VolRecClear()
{

	// 処理確認
	if( ICSMessageBox( FMT_MS_UPDATE, (MB_YESNO | MB_DEFBUTTON2)) == IDNO )	return;
	char	sign[10] = {0};
	sign[0] = refundvol.KPKuck;					// 取引金額など(2-1)	D0:OFF印刷なし　ON:印刷あり
	sign[1] = refundvol.KPTsck;					// 取引金額など(3-2)	D0:OFF印刷なし　ON:印刷あり
	sign[2] = refundvol.KPKsck;					// 取引金額など(3-3)	D0:OFF印刷なし　ON:印刷あり
	sign[3] = refundvol.KPKusg;					// 税抜・税込(2-1)		D0:OFF税抜　ON税込
	sign[4] = refundvol.KPSsgn;					// 税抜・税込(3-1)		D0:OFF税抜  ON税込(損益) D4:OFF税抜ON税込(資産)
	sign[5] = refundvol.KPTsgn;					// 税抜・税込(3-2)
	sign[6] = refundvol.KPKsgn;					// 税抜・税込(3-3)
	sign[7] = refundvol.KPType;					// 法人個人の別
	memset( &refundvol, '\0', sizeof( REFUNDVOL ) );
	refundvol.KPEkoz[0] = (unsigned char )0xff;
	refundvol.KPEbsg = 5;
	refundvol.KPEssg = 5;
	refundvol.KPKuck = sign[0];					// 取引金額など(2-1)	D0:OFF印刷なし　ON:印刷あり
	refundvol.KPTsck = sign[1];					// 取引金額など(3-2)	D0:OFF印刷なし　ON:印刷あり
	refundvol.KPKsck = sign[2];					// 取引金額など(3-3)	D0:OFF印刷なし　ON:印刷あり
	refundvol.KPKusg = sign[3];					// 税抜・税込(2-1)		D0:OFF税抜　ON税込
	refundvol.KPSsgn = sign[4];					// 税抜・税込(3-1)		D0:OFF税抜  ON税込(損益) D4:OFF税抜ON税込(資産)
	refundvol.KPTsgn = sign[5];					// 税抜・税込(3-2)
	refundvol.KPKsgn = sign[6];					// 税抜・税込(3-3)
	refundvol.KPType = sign[7];					// 法人個人の別

	pSnRefundData->ClearDataByInit();

//	memset( pSyzShin->RefundJot, '\0', sizeof( REFUNDREC )*10 );
//	memset( pSyzShin->RefundExp, '\0', sizeof( REFUNDREC )*10 );
//	memset( pSyzShin->RefundStk, '\0', sizeof( REFUNDREC )*20 );
//	memset( pSyzShin->RefundTna, '\0', sizeof( REFUNDREC )*10 );
//	memset( pSyzShin->RefundKot, '\0', sizeof( REFUNDREC )*10 );
//	pSyzShin->JotTbl.tp_rnum = 0;		// 主な課税資産の譲渡等
//	pSyzShin->ExpTbl.tp_rnum = 0;		// 主な輸出取引等の明細
//	pSyzShin->TnaTbl.tp_rnum = 0;		// 主な棚卸資産・原材料等の取得
//	pSyzShin->KotTbl.tp_rnum = 0;		// 主な固定資産等の取得
}

// 金融機関の取得
void CRefundBaseDialog::VolBankInit()
{
	if( ICSMessageBox( FMT_MS_UPBANK, (MB_YESNO | MB_DEFBUTTON2)) == IDNO )	return;
	int		len;
	char	buf[128];
	// 銀行
	::ZeroMemory( buf, sizeof(buf) );
	::ZeroMemory( refundvol.KPEbnk, sizeof(refundvol.KPEbnk));
	memcpy( buf, pSnHeadData->Sn_BANK, sizeof(pSnHeadData->Sn_BANK));
	len = kjlen( buf, sizeof(buf) );
	memcpy( refundvol.KPEbnk, buf, len );
	switch(pSnHeadData->Sn_FINANCE){
	case 1:		refundvol.KPEbsg = 1;		break;
	case 2:		refundvol.KPEbsg = 2;		break;
	case 3:		refundvol.KPEbsg = 3;		break;
	case 4:		refundvol.KPEbsg = 4;		break;
	case 5:		refundvol.KPEbsg = 5;		break;
	default:	refundvol.KPEbsg = 0;		break;
	}
	::ZeroMemory( buf, sizeof(buf));
	::ZeroMemory( refundvol.KPEstn, sizeof(refundvol.KPEstn));
	memcpy( buf, pSnHeadData->Sn_SITEN, sizeof(pSnHeadData->Sn_SITEN));
	len = kjlen( buf, sizeof(buf) );
	if( len > 20 )	len = 20;
	memcpy( refundvol.KPEstn, buf, len );
	switch(pSnHeadData->Sn_CHAIN){
	case 1:		refundvol.KPEssg = 1;		break;
	case 2:		refundvol.KPEssg = 2;		break;
	case 3:		refundvol.KPEssg = 3;		break;
	case 4:		refundvol.KPEssg = 5;		break;
	case 5:		refundvol.KPEssg = 4;		break;
	case 6:		refundvol.KPEssg = 5;		break;	//★//[13'09.25]
	default:	refundvol.KPEssg = 0;		break;
	}
	::ZeroMemory( buf, sizeof(buf));
	::ZeroMemory( refundvol.KPEykn, sizeof(refundvol.KPEykn));
	memcpy( buf, pSnHeadData->Sn_YOKIN, sizeof(pSnHeadData->Sn_YOKIN));
	len = kjlen( buf, sizeof(buf) );
	memcpy( refundvol.KPEykn, buf, len );
	memmove( refundvol.KPEkoz, pSnHeadData->Sn_KOZA, 5 );
	memmove( refundvol.KPEyn1, pSnHeadData->Sn_YUNUM1, 5 );
	memmove( refundvol.KPEyn2, pSnHeadData->Sn_YUNUM2, 8 );

	if( pSnHeadData->Sn_BYsw )	refundvol.KPEbdm = 0x01;	//★//[13'09.25]//

}

void CRefundBaseDialog::OnSize(UINT nType, int cx, int cy)
{
	if(bottom_diag) {
		SetScrollPos( SB_VERT, 0, TRUE );
		ScrollWindow( 0, m_cmPos, NULL );
		m_cmPos = 0;
	}
	// リサイズ
	ICSDialog::OnSize(nType, cx, cy);
	if ( bottom_diag )	{
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect( &a );
		bottom_diag->GetRect( bottom_index, (LPUNKNOWN)&b );
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE )
			ScrollInfo.nPos = 0;
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CRefundBaseDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

//★//[12'02.09]///
//	int	cpos = GetScrollPos( SB_VERT );
//	int	mpos;
//	
//	if ( nSBCode == SB_LINEDOWN )			mpos = cpos+15;
//	else if ( nSBCode == SB_LINEUP )		mpos = cpos-15;
//	else if ( nSBCode == SB_PAGEDOWN )		mpos = cpos+m_pagesiz;
//	else if ( nSBCode == SB_PAGEUP )		mpos = cpos-m_pagesiz;
//	else if ( nSBCode == SB_THUMBTRACK )	mpos = nPos;
//
//	if ( nSBCode == SB_LINEDOWN || nSBCode == SB_LINEUP || nSBCode == SB_PAGEDOWN ||
//			nSBCode == SB_PAGEUP || nSBCode == SB_THUMBTRACK ) {
//		if ( mpos < 0 )		mpos = 0;
//		if ( mpos > m_sclmax )	mpos = m_sclmax;
//		SetScrollPos( SB_VERT, mpos, TRUE );
//		ScrollWindow( 0, cpos-mpos, NULL );
//	}
///////////////////
	if(pScrollBar) return;
	SCROLLINFO SI;
	GetScrollInfo( SB_VERT, &SI );
	int nowPos = m_wPos;

	switch(nSBCode) {
	case SB_LINEUP:
	case SB_PAGEUP:
		if( nSBCode == SB_LINEUP )				m_wPos -= m_ScW;
		else									m_wPos -= SI.nPage;
		if( m_wPos <= 0 )						m_wPos = 0;
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		if( nSBCode == SB_LINEDOWN )			m_wPos += m_ScW;
		else									m_wPos += SI.nPage;
		if( m_wPos >= SI.nMax-(int)SI.nPage )	m_wPos = SI.nMax-SI.nPage;
		break;
	case SB_THUMBTRACK:
		m_wPos = nPos;
		break;
	}
	if ( nSBCode == SB_LINEDOWN || nSBCode == SB_LINEUP || nSBCode == SB_PAGEDOWN ||
			nSBCode == SB_PAGEUP || nSBCode == SB_THUMBTRACK ) {
		ScrollWindow( 0, nowPos-m_wPos );
		SetScrollPos( SB_VERT, m_wPos );
//--> '16.07.22 INS START
		UpdateWindow();
//<-- '16.07.22 INS END
		m_cmPos = m_wPos;
	}
///////////////////
	ICSDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BEGIN_EVENTSINK_MAP(CRefundBaseDialog, ICSFormView)
	ON_EVENT_RANGE(CRefundBaseDialog, IDC_ICSDIAGCTRL_MEISAI_H1, IDC_ICSDIAGCTRL_YUSYUTSU, 1 /* EditON */, OnEditONIcsdiagctrl, VTS_I4 VTS_I2 )
	ON_EVENT_RANGE(CRefundBaseDialog, IDC_ICSDIAGCTRL_MEISAI_H1, IDC_ICSDIAGCTRL_YUSYUTSU, 2 /* EditOFF */, OnEditOFFIcsdiagctrl, VTS_I4 VTS_I2)
	ON_EVENT_RANGE(CRefundBaseDialog, IDC_ICSDIAGCTRL_MEISAI_H1, IDC_ICSDIAGCTRL_YUSYUTSU, 3 /* Termination */, OnTerminationIcsdiagctrl, VTS_I4 VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT_RANGE(CRefundBaseDialog, IDC_ICSDIAGCTRL_MEISAI_H1, IDC_ICSDIAGCTRL_YUSYUTSU, 9 /* CheckButton */, OnCheckButtonIcsdiagctrl, VTS_I4 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CRefundBaseDialog::OnEditONIcsdiagctrl(UINT CtlID, short index) 
{
	CurrentDiagID = CtlID;
	SetScroll(CtlID,index);

	// 行の移動ができないときに「１行上（下）へ」ボタンを無効にする (2020.02.06)
	// D0 : IDC_BUTTON_UP1
	// D1 : IDC_BUTTON_DOWN1
	// D2 : IDC_BUTTON_UP2
	// D3 : IDC_BUTTON_DOWN2
	int UpDownOffFlg = 0x0F;
	// 選択していない方を無効に
	switch (CtlID) {
	case IDC_ICSDIAGCTRL_MEISAI_H1:
	case IDC_ICSDIAGCTRL_MEISAI_H3:
	case IDC_ICSDIAGCTRL_MEISAI_K1:
	case IDC_ICSDIAGCTRL_MEISAI_K3:
		UpDownOffFlg = 0x04 | 0x08;
		break;
	case IDC_ICSDIAGCTRL_MEISAI_H2:
	case IDC_ICSDIAGCTRL_MEISAI_H4:
	case IDC_ICSDIAGCTRL_MEISAI_K2:
	case IDC_ICSDIAGCTRL_MEISAI_K4:
		UpDownOffFlg = 0x01 | 0x02;
		break;
	default:
		break;
	}
	
	if (UpDownOffFlg != 0x0F) {
		CDiagControlBase* control = GetDiagControl(CtlID);
		if (control->GetItemLine(index) < 0) {
			UpDownOffFlg = 0x0F;
		}
		// 先頭行なら「上へ」を無効に
		else if (control->GetItemLine(index) == 0) {
			UpDownOffFlg |= 0x01 | 0x04;
		}
		// 最終行なら「下へ」を無効に
		else  if (control->IsBottomRow(index)) {
			UpDownOffFlg |= 0x02 | 0x08;
		}
	}
	GetDlgItem(IDC_BUTTON_UP1)->EnableWindow(!(UpDownOffFlg & 0x01));	// FALSEで無効になるため、!をつけている
	GetDlgItem(IDC_BUTTON_DOWN1)->EnableWindow(!(UpDownOffFlg & 0x02));
	GetDlgItem(IDC_BUTTON_UP2)->EnableWindow(!(UpDownOffFlg & 0x04));
	GetDlgItem(IDC_BUTTON_DOWN2)->EnableWindow(!(UpDownOffFlg & 0x08));

//2016.02.23 INSERT START
	int ctrl = 0;
	switch( CtlID ){
		case IDC_ICSDIAGCTRL_MEISAI_H1:
		case IDC_ICSDIAGCTRL_MEISAI_H3:
		case IDC_ICSDIAGCTRL_MEISAI_K1:
		case IDC_ICSDIAGCTRL_MEISAI_K3:
			ctrl = IDC_BUTTON_DEL1;
			break;
		case IDC_ICSDIAGCTRL_MEISAI_H2:
		case IDC_ICSDIAGCTRL_MEISAI_H4:
		case IDC_ICSDIAGCTRL_MEISAI_K2:
		case IDC_ICSDIAGCTRL_MEISAI_K4:
			ctrl = IDC_BUTTON_DEL2;
			break;
		default:
			break;
	}

	if( ctrl != 0 ){
		CDiagControlBase* control = GetDiagControl(CtlID);
		if( control->GetItemLine(index) < 0 ){
			return;
		}
		if( control->IsLineHead(index) == TRUE ){
			GetDlgItem(ctrl)->SetWindowText("行削除\n(Delete)");
		}else{
			GetDlgItem(ctrl)->SetWindowText("項目削除\n(Delete)");
		}
	}

	// Zmselダイアログ表示
	PostMessage( WM_ZMSEL_MESSAGE, ( WPARAM )CtlID, ( LPARAM )index );
//2016.02.23 INSERT END
}

void CRefundBaseDialog::OnCheckButtonIcsdiagctrl(UINT CtlID, short index, LPUNKNOWN data) 
{
	CDiagControlBase* control = GetDiagControl(CtlID);
	if( !control ) return;

	control->check(index,(DIAGRAM_DATA*)data);
}

void CRefundBaseDialog::OnTerminationIcsdiagctrl(UINT CtlID, short index, short nChar, short length, LPUNKNOWN data) 
{
	Termination(CtlID,index,(char)nChar);
}

void CRefundBaseDialog::OnEditOFFIcsdiagctrl(UINT CtlID,short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CDiagControlBase* control = GetDiagControl( CtlID );
	if( !control ) return;

	control->EditOff(index);

	//----> 20230704
	if( pZmSel && control->IsTorihikiInvnoClm( index ) != FALSE ){
		if( pZmSel->IsDisplay() != FALSE ){
			pZmSel->DialogOFF();
		}
	}//<---- 20230704
}

void CRefundBaseDialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

void CRefundBaseDialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnCancel();
}


//yoshida
//処理判断できないので後日修正

//--> '15.01.30 INS START
//-----------------------------------------------------------------------------
// 他表で変更があったので再集計
//-----------------------------------------------------------------------------
// 引数	sw					：	どのダイアログ？
//									0:個人1, 1:法人1, 2:法人2, 3:個人2
//		pH26SnFhyo20Data	：	付表２のデータ
//		shRevType			：	消費税改正様式
//-----------------------------------------------------------------------------
// 返送値	0				：	正常終了
//			-1				：	エラー
//-----------------------------------------------------------------------------
int CRefundBaseDialog::calq_by_OtherChange( int sw, CH31SnFhyo2_01Data *pH31SnFhyo2_01Data, EnumIdIcsShRevType shRevType )
{
	// 原則課税時のみ
	if( pUtil->GetKazeihoushiki(pZmSub->zvol) != ID_ICSSH_GENSOKU ){
		return 0;
	}

	// 更新対象データ
	if(pH31SnFhyo2_01Data == NULL ){
		return 0;
	}
	// 金額セット
	if( SHIIRE_REC == NULL ){
		return 0;
	}
	// 対象帳票？
	if( (sw==0) || (sw==1) ){
		return 0;
	}

	CArith arith(0x16);
//-- '15.04.27 --
//	arith.l_add( SHIIRE_REC[9].KPSvl3, pH26SnFhyo20Data->Sn_2F9A, pH26SnFhyo20Data->Sn_2F10A );
//	arith.l_add( SHIIRE_REC[9].KPSvl3, SHIIRE_REC[9].KPSvl3, pH26SnFhyo20Data->Sn_2F9B );
//	arith.l_add( SHIIRE_REC[9].KPSvl3, SHIIRE_REC[9].KPSvl3, pH26SnFhyo20Data->Sn_2F10B );
//	arith.l_add( SHIIRE_REC[9].KPSvl3, SHIIRE_REC[9].KPSvl3, pH26SnFhyo20Data->Sn_2F9C );
//	arith.l_add( SHIIRE_REC[9].KPSvl3, SHIIRE_REC[9].KPSvl3, pH26SnFhyo20Data->Sn_2F10C );
//---------------

	if( sw == 2 ){
		if ((pSnHeadData->m_DispTabSgn & 0xc0) != 0xc0) {	// 特例適用時は別関数で集計済
			arith.l_add( SHIIRE_REC[9].KPSvl3, pH31SnFhyo2_01Data->Sn_F201_10F, pH31SnFhyo2_01Data->Sn_F201_12F );
			arith.l_add( SHIIRE_REC[9].KPSvl3, SHIIRE_REC[9].KPSvl3, pH31SnFhyo2_01Data->Sn_F201_13F );
			arith.l_add( SHIIRE_REC[9].KPSvl3, SHIIRE_REC[9].KPSvl3, pH31SnFhyo2_01Data->Sn_F201_28F);//yoshida230626
		}
	}
	else if ( sw == 3 ) {
		if ((pSnHeadData->m_DispTabSgn & 0xc0) != 0xc0) {	// 特例適用時は別関数で集計済
			arith.l_add( SHIIRE_REC[12].KPSvl3, pH31SnFhyo2_01Data->Sn_F201_10F, pH31SnFhyo2_01Data->Sn_F201_12F );
			arith.l_add( SHIIRE_REC[12].KPSvl3, SHIIRE_REC[12].KPSvl3, pH31SnFhyo2_01Data->Sn_F201_13F );
			arith.l_add( SHIIRE_REC[12].KPSvl3, SHIIRE_REC[12].KPSvl3, pH31SnFhyo2_01Data->Sn_F201_28F);//yoshida230626
		}
	}

//---------------

	// 手入力中には表示の更新はしない
	if( refundvol.KPSstx ){
		return 0;
	}

	CDiagControlBase* control = 0;
	if( sw==2 ){
		DiagControlMap.Lookup( IDC_ICSDIAGCTRL_SHIIRE_HO, control );
	}
	else if( sw==3 ){
		DiagControlMap.Lookup( IDC_ICSDIAGCTRL_SHIIRE_KO, control );
	}
	if( control == NULL ){
		return 0;
	}
	control->DispInit();

	return 0;
}


//<-- '15.01.30 INS END
//2016.02.23 INSERT START
//「入力ロック中」のフォント作成
int CRefundBaseDialog::Make_StaticFont()
{
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);

	if( item == NULL ){
		return -1;
	}

	LOGFONT log_font;
	item->GetFont()->GetLogFont(&log_font);
	log_font.lfWeight = FW_BOLD;

	m_font.DeleteObject();
	m_font.CreateFontIndirectA( &log_font );

	item->SetFont( &m_font, FALSE );

	return 0;
}

//「入力ロック中」の表示・非表示
void CRefundBaseDialog::Static_Update( unsigned char Sn_Sign4 )
{
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);

	if( item == NULL ){
		return;
	}

	if( Sn_Sign4&0x01){
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(TRUE);
	}else{
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(FALSE);
	}
}


//==================================================
// ZmSelの初期化
//--------------------------------------------------
// 返送値
//		0			:			正常終了
//		-1			:			エラー
//==================================================
int CRefundBaseDialog::InitZmSel()
{
	pZmSel = new CZmselDB();
	if( pZmSel != NULL ){
		pZmSel->SetDialogResize( FALSE );//科目選択を小さく表示
		pZmSel->SetPara( pZmSub, this, NULL,  callback );
		CDiagControlBase::pZmSel = pZmSel;
	}
	else {
		return -1;
	}

	return 0;
}

//==================================================
// ZmSelの表示
//==================================================
LRESULT CRefundBaseDialog::OnShowZmselDialog( WPARAM wParam, LPARAM lParam )
{
	int ctrlID = ( int )wParam;
	int index = ( int )lParam;

	//---->20230704
	CDiagControlBase* control = GetDiagControl(ctrlID);
	if( pZmSel && control->IsTorihikiInvnoClm( index ) != FALSE ){
		if( pZmSel->IsDisplay() != FALSE ){
			pZmSel->DialogOFF();
		}

		// 非連動dbは科目設定を開けないため、摘要に登録番号を入力することはないので科目選択は表示しない。
		// また、会社登録で「インボイス登録番号の表示」を「表示しない」に設定している場合も出さない。
		if( pUtil->IsUnConnectMst( pZmSub->zvol ) == FALSE && !pZmSub->zvol->s_sgn8 ){

			// 位置調整
			CRect diagrect, zmselrect;
			CIcsdiagctrl* diag = GetCurrentDiag();
			diag->GetRect( index, (LPUNKNOWN)&diagrect );
			ScreenToClient( &diagrect );

			pZmSel->GetScreenRect( zmselrect );

			//上下
			if(( diagrect.top - zmselrect.Height() - 10 ) > 0 ) {
				zmselrect.top = diagrect.top - 10;
			}
			else {
				zmselrect.top = diagrect.bottom + zmselrect.Height() + 10;
			}
			//左右
			LONG sWidth = zmselrect.Width();
			if ((sWidth / 2 ) > diagrect.right) {
				//マルチ右など
				zmselrect.left = diagrect.left - (sWidth / 2);
			}
			else {
				//基本
				zmselrect.left = 10;
			}


			pZmSel->DispCloseButton( 1 );
			pZmSel->DialogON( ZSEL_INVNO, ZSEL_TOPLEFT, &zmselrect );
		}
	}
	else {
		if( pZmSel && pZmSel->IsDisplay() != FALSE ){
			pZmSel->DialogOFF();
		}
	}
	//<----20230704

	return 0L;
}

//==========================================================
// インボイス登録番号のチェック
//----------------------------------------------------------
// 引数
//		sgn		:		0　科目選択をマウスクリックした場合
//				:　		1　diagに入力した場合
//				:		2　F4プレビューやタブ移動など
//				:		3　処理終了
//----------------------------------------------------------
// 返送値
//		true	:		問題なし
//		false	:		不正な登録番号あり
//==========================================================
bool CRefundBaseDialog::CheckInvno( int sgn )
{	
	CDiagControlBase* control = GetDiagControl(CurrentDiagID);
	if( control ){
		return control->CheckInvno( sgn );
	}
	return true;
}
