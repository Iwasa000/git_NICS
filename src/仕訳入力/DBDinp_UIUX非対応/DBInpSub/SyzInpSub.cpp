#include "stdafx.h"
#include "resource.h"

#include "SkbnIn.h"
#include "SyzInp.h"
#include "DBInpSubFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HINSTANCE gInstance;


//////////////////////////////////////////////////////////////////////////////////
class CSyzInpSub
{
public:
	CSyzInpSub();
	~CSyzInpSub();

public:
	void SetDenpMode();
	void SetInvNoDispCount(int count);

	int	SetPara( CDBZmSub* pZm, CDBSyohi* pSy, CWnd* parent );
	int	DialogON( BOOL bOK, CRect rect, int posflg, int pn );
	int	DialogOFF();
	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );
	void	GetCrntWindowRect(CRect& rect);

	int		SetSkbnSelNo( int no );
	void	set_nextpos();
	void	set_nowfocus();
	int		CheckSyzPn( int vect );
	DWORD	GetSyzswkKbn();
	int		GetInvoiceInfo();
	BOOL	CheckInvno();
	int		RenewInvNo();
	void	CancelInvNoIME();

public:
	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
	CWnd*		m_pParent;

	CSkbnIn*	m_pDialog;	
	BOOL		m_bResize;

private:
	// インボイス対応
	bool m_bInvMaster;
	bool m_bInvUse;
	bool m_bInvDisp;
	int	 m_Koujo80_50;		//-1:期間外, 0=控除割合(80%), 1=80%, 50%混在, 2=50%
	int  m_InpMode;
	int  m_InvnoDispCount;	//インボイス登録番号の摘要名称の表示数(Default = 32)

	CArray<INVOICE_INFO> m_InvAry;
};



CSyzInpSub::CSyzInpSub()
{
	m_pZm = NULL;
	m_pSy = NULL;
	m_pDialog = NULL;
	m_bResize = TRUE;

	// インボイス対応
	m_bInvMaster = false;
	m_bInvUse = false;
	m_bInvDisp = false;
	m_InpMode = INPMODE_NORMMAL;
	m_InvnoDispCount = 32;

	m_Koujo80_50 = SYZ_MEN_NOT;
}

CSyzInpSub::~CSyzInpSub()
{
}

void CSyzInpSub::SetDenpMode()
{
	// 伝票入力では消費税ダイアログにインボイス項目を表示しない
	// 伝票入力が可能な業務が仕訳入力・定型仕訳登録・ZIM15_InpSub.dllと少数のため、
	// 既存の関数を更新するのではなく新規関数を作成する

	// 伝票入力セット
	m_InpMode = INPMODE_DENP;
}


void CSyzInpSub::SetInvNoDispCount(int count)
{
	m_InvnoDispCount = count;
}


int CSyzInpSub::SetPara( CDBZmSub* pZm, CDBSyohi* pSy, CWnd* parent )
{
	m_pZm = pZm;
	m_pSy = pSy;
	m_pParent = parent;

	HINSTANCE h =  AfxGetResourceHandle();
	AfxSetResourceHandle( gInstance );

	BOOL bToku = FALSE;
	if( m_pZm->zvol->apno >= 0x50 )
		bToku = TRUE;

	// インボイス対応
	CZmGen8 zmGen;
	// バージョンアップマスターか判定
	m_bInvMaster = zmGen.IsInvoiceVupMaster( pZm );
	if( m_bInvMaster ) {
#ifdef CLOSE
		if(( pZm->zvol->s_sgn2 & 0x0f ) >= 2 ) {
			// 簡易課税の場合は免税事業者からの課税仕入れと登録番号を非表示にするため、インボイス期間外としておく
			m_bInvUse = false;
		}
		else {
#endif
			// インボイス期間マスターか
			m_bInvUse = zmGen.IsAbleToUseInvoice( pZm );
//		}
		m_Koujo80_50 = SYZ_MEN_NOT;

		if( m_bInvUse ) {
			m_Koujo80_50 = GetMenzeiWariai(m_pZm->zvol);
		}
		// 登録番号を表示するか
		m_bInvDisp = zmGen.IsDispInvno( pZm );
	}
	else {
		m_Koujo80_50 = SYZ_MEN_NOT;
		m_bInvUse = false;
		m_bInvDisp = false;
	}

	m_pDialog = new CSkbnIn( parent, bToku, m_bResize, m_bInvMaster, m_bInvUse, m_bInvDisp, m_InpMode, m_Koujo80_50);
	if( m_pDialog == NULL )	return -1;

	AfxSetResourceHandle( h );

	m_pDialog->GetWindowRect( &m_pDialog->m_DlgRect );

#ifdef TEST_CLOSE
CRect rc1;
rc1 = m_pDialog->m_orgRect;
MyTrace( "@CSyzInpSub::SetPara[m_orgRect] %d, %d, %d, %d, [w = %d, h = %d]\n", rc1.left, rc1.top, rc1.right, rc1.bottom,
		rc1.Width(), rc1.Height() );
rc1 = m_pDialog->m_DlgRect;
MyTrace( "@CSyzInpSub::SetPara[m_DlgRect] %d, %d, %d, %d [w = %d, h = %d]\n", rc1.left, rc1.top, rc1.right, rc1.bottom,
		rc1.Width(), rc1.Height() );
#endif

	m_pDialog->m_pZm = pZm;
	m_pDialog->m_pSy = pSy;

	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		GetInvoiceInfo();
		m_pDialog->SetInvArray( &m_InvAry );
		m_pDialog->SetInvPageCount(m_InvnoDispCount);
	}
	else {
		m_pDialog->SetInvArray(NULL);
	}


	return 0;
}


int	CSyzInpSub::RenewInvNo()
{
	// インボイス対応
	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		GetInvoiceInfo();
		m_pDialog->RenewInvArray(&m_InvAry);
	}
	return 0;
}


void CSyzInpSub::CancelInvNoIME()
{
	// インボイス対応
	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		m_pDialog->CancelInvNoIME();
	}
}


int	CSyzInpSub::DialogON( BOOL bOK, CRect rect, int sypos, int pn )
{
	if( m_pDialog == NULL )	return	-1;
	
FILETrace("SYZ DialogON TOP = sypos(%d), rect( %d,%d, %d,%d)\n", sypos, rect.left, rect.top, rect.right, rect.bottom);

	BOOL	st;
	int setpn;

	st = m_pDialog->skbn_dataset( &setpn );
	if( ! st ) return -1;

	RECT	newrect;
	double width, height;

	m_pDialog->m_DlgRect.NormalizeRect();

	width = (double)m_pDialog->m_DlgRect.Width();
	height = (double)m_pDialog->m_DlgRect.Height();

FILETrace("SYZ DialogON sypos(%d), m_pDialog->m_DlgRect( %d,%d, %d,%d)\n", sypos, 
			m_pDialog->m_DlgRect.left, m_pDialog->m_DlgRect.top, m_pDialog->m_DlgRect.right, m_pDialog->m_DlgRect.bottom );
FILETrace("SYZ DialogON width (%f), height (%f)\n", width, height);

	// 09.09 /10
	double grx, gry;	
	Dlg_GetRate( m_pDialog, grx, gry );

	if( m_bResize ) {
		if( grx != 0 && gry != 0 ) {
			width = grx * width;
			height = gry * height;
		}
	}
FILETrace("SYZ DialogON grx, gry( %f,%f)\n", grx, gry);
FILETrace("SYZ DialogON width(2) (%f), height(2) (%f)\n", width, height);

	BOOL bVert, bHorz;
	CRect zkbnRect;
	int ctrlWidth;
	m_pDialog->DlgRectCheck(bVert, bHorz, &zkbnRect, &ctrlWidth);
	FILETrace("SYZ DialogON DlgRectCheck　bVert(%d), bHorz(%d)  zkbnRect(%d,%d,%d,%d,), ctrlWidth=%d\n", bVert, bHorz, zkbnRect.left, zkbnRect.top, zkbnRect.right, zkbnRect.bottom, ctrlWidth);
	int typ = GetMultiWindowType();
	if( typ == MULTIWINDOW_FULL ) {
		if( width < ctrlWidth ) {
			width = ctrlWidth + 4;
		}
	}

	if( sypos == SYP_TOPLEFT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - (LONG)height; /*( height * y );*/
		newrect.right = rect.left;
		newrect.left = newrect.right - (LONG)width; /*( width * x ); */
	}
	else if( sypos == SYP_TOPRIGHT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - (LONG)height; /*( height * y );*/
		newrect.left = rect.left;	   
		newrect.right = newrect.left + (LONG)width; /*( width * x ); */
	}
	else if( sypos == SYP_BOTTOMLEFT ) {
		newrect.top = rect.bottom;
		newrect.bottom = newrect.top + (LONG)height; /*( height * y );*/
		newrect.right = rect.left;	   
		newrect.left = newrect.right - (LONG)width; /*( width * x ); */
	}
	else {
		newrect.top = rect.bottom;
		newrect.bottom = newrect.top + (LONG)height; /*( height * y );*/
		newrect.left = rect.left;
		newrect.right = rect.left + (LONG)width; /*( width * x ); */
	}
	CRect	paRect;
	m_pParent->GetWindowRect( paRect );
	m_pParent->ScreenToClient( paRect );

	if( newrect.right > paRect.right ) {
		int n = newrect.right - paRect.right;
		newrect.left	-= n;
		newrect.right	-= n;
	}

	m_pDialog->EnableWindow( TRUE );
	WINDOWPLACEMENT	wp;
	m_pDialog->GetWindowPlacement( &wp );
	wp.showCmd	=	SW_SHOWNA;
	m_pDialog->SetWindowPlacement( &wp );

	m_pDialog->SetWindowPos(&CWnd::wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW);

FILETrace("SYZ DialogON pos1( %d,%d,%d,%d,)\n", newrect.left, newrect.top, newrect.right, newrect.bottom);


	// ダイアログ位置の調整
	if( grx != 0 && gry != 0 ) {
		width = m_pDialog->m_DlgRect.Width();
		height = m_pDialog->m_DlgRect.Height();

		m_pDialog->DlgRectCheck(bVert, bHorz, &zkbnRect, &ctrlWidth);
		FILETrace("SYZ DialogON DlgRectCheck(2)　bVert(%d), bHorz(%d)  zkbnRect(%d,%d,%d,%d,), ctrlWidth=%d\n", bVert, bHorz, zkbnRect.left, zkbnRect.top, zkbnRect.right, zkbnRect.bottom, ctrlWidth);

		if( bVert || bHorz ) {
			if (!bVert) {
				height = gry * height;
			}
			else {
				height = m_pDialog->GetDlgHeight();
			}
			if( ! bHorz )	width	= grx * width;
			else {
				width = m_pDialog->GetDlgWidth();
			}
			if( typ == MULTIWINDOW_FULL ) {
				if( width < ctrlWidth ) {
					width = ctrlWidth + 4;
				}
			}

			if( sypos == SYP_TOPLEFT ) {
				newrect.bottom = rect.top;
				newrect.top = newrect.bottom - height; /*( height * y );*/
				newrect.right = rect.left;
				newrect.left = newrect.right - width; /*( width * x ); */
			}
			else if( sypos == SYP_TOPRIGHT ) {
				newrect.bottom = rect.top;
				newrect.top = newrect.bottom - height; /*( height * y );*/
				newrect.left = rect.left;	   
				newrect.right = newrect.left + width; /*( width * x ); */
			}
			else if( sypos == SYP_BOTTOMLEFT ) {
				newrect.top = rect.bottom;
				newrect.bottom = newrect.top + height; /*( height * y );*/
				newrect.right = rect.left;	   
				newrect.left = newrect.right - width; /*( width * x ); */
			}
			else {
				newrect.top = rect.bottom;
				newrect.bottom = newrect.top + height; /*( height * y );*/
				newrect.left = rect.left;
				newrect.right = rect.left + width; /*( width * x ); */
			}
			int typ = GetMultiWindowType();
			if( typ == MULTIWINDOW_FULL ) {
				if( newrect.right < zkbnRect.right ) {
					newrect.right = zkbnRect.right + 2;
				}
			}
			m_pDialog->SetWindowPos( &CWnd::wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );

			FILETrace("SYZ DialogON SetWindowPos(2) %d, %d, %d, %d\n", newrect.left, newrect.top, newrect.right, newrect.bottom );

		}
	}

FILETrace( "SYZ DialogON setpn = %08x, pn = %d\n", setpn, pn );

	if( pn == -1 ) {
		//簡易課税・個別対応は、売上／仕入区分に
		if( m_pSy->IsKaniMaster() && (setpn & (0x01 << SY_URISIRE_PN)) ) {
			pn = SY_URISIRE_PN;
		}
		if( m_pSy->IsKobetsuMaster() && (setpn & (0x01 << SY_URISIRE_PN)) ) {
			pn = SY_URISIRE_PN;
		}

		if( pn == -1 ) {
			//税区分をデフォルトにする
			if( (setpn & (0x01 << SY_SZKBN_PN)) ) {
				pn = SY_SZKBN_PN;
			}
			else {
				for( int i = 0; i <= SY_TOKUS_PN; i++ ) {
					if( (setpn & (0x01 << i)) ) {
						pn = i;
						break;
					}
				}
			}
		}
	}
	else {
		//項目セット可能位置にカーソルセット
		if( !(setpn & (0x01 << pn)) ) {
			//税区分をデフォルトにする
			if( (setpn & (0x01 << SY_SZKBN_PN)) ) {
				pn = SY_SZKBN_PN;
			}
			else {
				for( int i = 0; i <= SY_TOKUS_PN; i++ ) {
					if( (setpn & (0x01 << i)) ) {
						pn = i;
						break;
					}
				}
			}
		}
	}

FILETrace( "SYZ set_fastfocus pn = %d\n", pn );

	// インボイス対応
	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		GetInvoiceInfo();
		m_pDialog->SetInvArray(&m_InvAry);
	}

	m_pDialog->set_fastfocus( bOK, pn );
	
//	WINDOWPLACEMENT	wp;
//	m_pDialog->GetWindowPlacement( &wp );
//	wp.showCmd	=	SW_SHOWNA;
//	m_pDialog->SetWindowPlacement( &wp );

//	m_pDialog->GetWindowRect( pDialog->CrntRect );
//	m_pParent->ScreenToClient( pDialog->CrntRect );
	m_pDialog->RedrawWindow();

	m_pDialog->m_Dispflg	=	TRUE;

	return 0;
}


int CSyzInpSub::DialogOFF()
{
	if( m_pDialog == NULL )	return	-1;
//	m_pDialog->GetWindowRect( m_pDialog->DlgRect );
//	pParent->ScreenToClient( m_pDialog->DlgRect );
	m_pDialog->ShowWindow( SW_HIDE );
//	m_pDialog->MoveWindow(0,0,0,0);
	m_pDialog->EnableWindow( FALSE );

	m_pDialog->m_Dispflg	=	FALSE;

	return 0;
}


BOOL CSyzInpSub::IsDisplay()
{
	return m_pDialog->m_Dispflg;
}


void CSyzInpSub::GetCrntRect( CSize& size )
{
	if( m_pDialog == NULL ) {
		size.cx = 0; size.cy = 0;
		return;
	}

	RECT	newrect;
	double width, height;

	width = m_pDialog->m_DlgRect.Width();
	height = m_pDialog->m_DlgRect.Height();

	// 09.09 /10
	double grx, gry;	
	Dlg_GetRate( m_pDialog, grx, gry );

	if( grx != 0 && gry != 0 ) {
		width	= grx * width;
		height	= gry * height;
	}

	newrect.top		= m_pDialog->m_DlgRect.top;
	newrect.left	= m_pDialog->m_DlgRect.left;
	newrect.bottom	= newrect.top + height;
	newrect.right	= newrect.left + width;

/*
	newrect.top		= m_pDialog->m_DlgRect.top;
	newrect.bottom	= m_pDialog->m_DlgRect.bottom;
	newrect.right	= m_pDialog->m_DlgRect.right;
	newrect.left	= m_pDialog->m_DlgRect.left;
*/
//	newrect.top = newrect.bottom - ( height * y );
//	newrect.right = m_pDialog->m_DlgRect.left + (x * 10);
//	newrect.left = newrect.right - ( width * x ) + (y * 10);

	size.cx = newrect.right - newrect.left;
	size.cy = newrect.bottom - newrect.top;
}


void CSyzInpSub::GetCrntWindowRect(CRect& rect)
{
	if( m_pDialog == NULL ) {
		rect.SetRectEmpty();
		return;
	}
	m_pDialog->GetWindowRect(rect);
}


int	 CSyzInpSub::SetSkbnSelNo( int no )
{
	return m_pDialog->SetSkbnSelNo( no );
}

void CSyzInpSub::set_nextpos()
{
	m_pDialog->set_nextpos();
}

void CSyzInpSub::set_nowfocus()
{
	m_pDialog->set_nowfocus();
}

int CSyzInpSub::CheckSyzPn(int vect)
{
	return m_pDialog->CheckSyzPn(vect);
}


DWORD	CSyzInpSub::GetSyzswkKbn()
{
	return m_pDialog->m_defSwkKbn;
}


//=============================================================================
// 摘要情報から登録番号の取得
//=============================================================================
int CSyzInpSub::GetInvoiceInfo()
{
	::GetInvoiceInfo(m_pZm, m_InvAry);

	return 0;
}

BOOL CSyzInpSub::CheckInvno()
{
	return m_pDialog->CheckInvnoInput();
}


////////////////////////////////////////////////////////////////////////////////////////////
// エクスポートクラス
CSyzInp::CSyzInp()
{
	pSub = NULL;
	if( (pSub = new CSyzInpSub) == NULL )
		return;
}


CSyzInp::~CSyzInp()
{
	if( pSub ) {
		delete pSub;
		pSub = NULL;
	}
}

void CSyzInp::SetDenpMode()
{
	pSub->SetDenpMode();
}


void CSyzInp::SetInvNoDispCount(int count)
{
	pSub->SetInvNoDispCount(count);
}

void CSyzInp::SetDialogResize(BOOL bResize)
{
	pSub->m_bResize = bResize;
}


int CSyzInp::SetPara( CDBZmSub* pZm, CDBSyohi* pSy, CWnd* parent )
{
	DBGFUNCTION;
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->SetPara( pZm, pSy, parent );
}

int	CSyzInp::RenewInvNo()
{
	return pSub->RenewInvNo();
}

void CSyzInp::CancelInvNoIME()
{
	return pSub->CancelInvNoIME();
}

// DialogON を 呼び出す前に 該当仕訳で、CDBSyohiのSyIniz()を呼び出す必要あり
int	CSyzInp::DialogON( BOOL bOK, CRect rect, int posflg, int pn )
{
	return pSub->DialogON( bOK, rect, posflg, pn );
}

int CSyzInp::DialogOFF()
{
	return pSub->DialogOFF();
}

BOOL CSyzInp::IsDisplay()
{
	return pSub->IsDisplay();
}


void CSyzInp::GetCrntRect( CSize& size )
{
	pSub->GetCrntRect( size );
}


void CSyzInp::GetCrntWindowRect(CRect& rect)
{
	pSub->GetCrntWindowRect(rect);
}


int	 CSyzInp::SetSkbnSelNo( int no )
{
	return pSub->SetSkbnSelNo( no );
}

int CSyzInp::CheckSyzPn(int vect)
{
	return pSub->CheckSyzPn( vect );
}

void CSyzInp::set_nextpos()
{
	pSub->set_nextpos();
}

void CSyzInp::set_nowfocus()
{
	pSub->set_nowfocus();
}


DWORD	CSyzInp::GetSyzswkKbn()
{
	return pSub->GetSyzswkKbn();
}

BOOL	CSyzInp::CheckInvno()
{
	return pSub->CheckInvno();
}


void Copydbdata(CDBDATA* dbdata, CDBDATA* src)
{
	dbdata->seq = src->seq;
	dbdata->ksign = src->ksign;
	dbdata->ddate = src->ddate;
	dbdata->mofs = src->mofs;
	dbdata->cno = src->cno;
	dbdata->dbmn = src->dbmn;
	dbdata->dkno = src->dkno;
	dbdata->dbt = src->dbt;
	dbdata->dbr = src->dbr;
	dbdata->cbmn = src->cbmn;
	dbdata->ckno = src->ckno;
	dbdata->cre = src->cre;
	dbdata->cbr = src->cbr;
	dbdata->val = src->val;
	dbdata->zei = src->zei;
	dbdata->dsign.RemoveAll();
	for( int i = 0; i < src->dsign.GetSize(); i++ ) {
		dbdata->dsign.Add(src->dsign.GetAt(i));
	}
	dbdata->owner = src->owner;
	dbdata->dtype = src->dtype;
	dbdata->tekiyo = src->tekiyo;
	dbdata->imgsq = src->imgsq;
	dbdata->snumber = src->snumber;
	dbdata->tdate = src->tdate;
	dbdata->edate = src->edate;
	dbdata->udate = src->udate;
	dbdata->icno = src->icno;			// 内部伝票番号
	dbdata->skn = src->skn;			// 資金繰り科目
	dbdata->tag = src->tag;			// 付箋コメント
	dbdata->tsign.RemoveAll();
	for( int i = 0; i < src->tsign.GetSize(); i++ ) {
		dbdata->tsign.Add(src->tsign.GetAt(i));
	}
	dbdata->hjiyu = src->hjiyu;			// 変動事由
	dbdata->taika = src->taika;			// 対価金額
	dbdata->cuser = src->cuser;			// 変更ユーザ
	dbdata->invno = src->invno;			// 登録番号
	dbdata->rnumber = src->rnumber;
}




//////////////////////////////////////////////////////////////////////////////////


CSyzInpLiteSub::CSyzInpLiteSub()
{
	m_pZm = NULL;
	m_pSy = NULL;
	m_pDialog = NULL;
	m_bResize = FALSE;
	m_bNoDlg = FALSE;
	m_bSzvolOpen = FALSE;
		
	// インボイス対応
	m_bInvMaster = false;
	m_bInvUse = false;
	m_bInvDisp = false;
	m_Koujo80_50 = SYZ_MEN_NOT;

	m_dispmode = 0;
}

CSyzInpLiteSub::~CSyzInpLiteSub()
{
	TRACE("~CSyzInpLiteSub m_pSy = %08x", m_pSy);

	if( m_pSy != NULL ) {
		if( m_bSzvolOpen ) {
			m_pZm->SyohizeiClose();
			m_bSzvolOpen = FALSE;
		}
		m_pSy->SyClose();
		delete m_pSy;
	}
}


// 輸入仕入などの科目か？
// 輸入仕入は、外税・別記だけなど、制御がかかっているので、通常の課税科目と同じ扱いにするため
//
int	_sykmk_check(DBKNREC* pkmk)
{
	int ret = 0;
	if( pkmk == NULL )	return 0;

	if( pkmk->knsgn[0] == 0x04 ) {
		switch( pkmk->knsgn[1] ) {
		case 0x04:	//輸入仕入
		case 0x11:	//輸入仕入(国税)
		case 0x12:	//輸入仕入(地方税)
		case 0x13:	//輸入仕入(合算)
			ret = 1;
			break;
		default:
			break;
		}
	}
	//輸出
	if( pkmk->knsgn[0] == 0x02 && pkmk->knsgn[1] == 0x09 ) {
		ret = 1;
	}
	return ret;
}


//------------------------------------------------------------
//	科目が消費税科目か？
//
//	return	0以外	消費税科目(1=不課税, 2=課税, 3=非課税)
//					D8  ON: 棚卸し科目
//					D9  ON: 仮受・仮払消費税
//					D10 ON: 未成工事支出金(未成工事タイプのみ完成工事振替)
//			0		消費税科目でない
//------------------------------------------------------------
int km_syattr(DBKNREC* pKn, CDBZmSub* pzm)
{
	BYTE	attr[10] = { 0 };
	int retn;

	if( pKn == NULL ) {
		return 0;
	}
	memmove(&attr, &pKn->knsgn, sizeof(pKn->knsgn));

	if( attr[0] && attr[2] & 0x0f )
		retn = attr[2] & 0x0f;
	else	retn = 0;

	if( retn == 0 ) {
		if( pKn->stock ) {
			retn |= 0x100;
		}

		DWORD dwCode;
		dwCode = pKn->kncod;
		hl_rev(&dwCode, 4);
		// 仮受・仮払

		// 06.27 /12
		DWORD svkcd1, svkcd2;
		svkcd1 = _atoh((LPSTR)(LPCTSTR)pzm->szvol->SVkcod1);
		svkcd2 = _atoh((LPSTR)(LPCTSTR)pzm->szvol->SVkcod2);

		if( (dwCode == svkcd1 || dwCode == svkcd2) ||
			((dwCode & 0x80) && (dwCode & 0xffffff00) == (svkcd1 & 0xffffff00)) ||
			((dwCode & 0x80) && (dwCode & 0xffffff00) == (svkcd2 & 0xffffff00)) ) {
			//if( (dwCode == svkcd1 || dwCode == svkcd2) ) {
			retn |= 0x0200;
		}
		else {
			//個人兼業
			if( pzm->zvol->apno == 0x10 && (pzm->zvol->ind_table & 0x01) ) {
				if( dwCode == 0x01071110 || dwCode == 0x01071120 || dwCode == 0x04051610 || dwCode == 0x04051620 )
					retn |= 0x0200;
			}
#ifdef CLOSE
			if( m_pZm->zvol->apno == 0x20 && M_MISEIKOJI_TYPE ) {
				if( (dwCode & 0xffffff00) == 0x01060200 ) {
					retn |= 0x0400;
				}
			}
#endif
		}
	}

	return(retn);
}



// 科目が空の場合、課税科目をセットする
//	返送値：課税科目をセットした場合 1 を返送
//
int	_sydata_initsub(CDBZmSub* pzm, CDBSyohi* psyz, CDBDATA* data)
{
	int date;
	if( data->ddate == 0 ) {
		date = pzm->zvol->ip_ymd;
	}
	//	data->seq = rec->m_seq;
	//	data->ddate = date;
	data->ksign = 0;

	//課税科目
	DBKNREC* pkn, *kazei;
	kazei = NULL;
	int setkzei = 0;

	for( int n = 0; n < pzm->knm_info->reccount; n++ ) {
		pkn = pzm->pKnrec + n;

		//仕入＋課税＋税率既定 で免税でない
		if( pkn->knsgn[0] == 0x04 && pkn->knsgn[2] == 0x02 && (pkn->knsgn[3] & 0xf0) == 0x00 && (pkn->knsgn[4] & 0x10) == 0x00 ) {
			kazei = pkn;
			//入力区分→ 標準
			if( pkn->kninp == 0 ) {
				kazei = pkn;
				break;
			}
		}
	}
	if( kazei == NULL ) { //学校は仕入属性科目がなかった
		for( int n = 0; n < pzm->knm_info->reccount; n++ ) {
			pkn = pzm->pKnrec + n;
			//経費＋課税＋税率既定
			if( pkn->knsgn[0] == 0x08 && pkn->knsgn[2] == 0x02 && (pkn->knsgn[3] & 0xf0) == 0x00 && (pkn->knsgn[4] & 0x10) == 0x00 ) {
				kazei = pkn;
				//入力区分→ 標準
				if( pkn->kninp == 0 ) {
					kazei = pkn;
					break;
				}
			}
		}
	}
//	MyTrace("_sydata_initsub kazei = %.14s", kazei->knnam);

	DWORD kzeicd = 0;
	if( kazei != NULL ) {
		kzeicd = kazei->kncod;
		hl_rev(&kzeicd, 4);
	}
	else {
		kzeicd = _atoh(SYOGT);
	}

	//輸入の科目の場合は、課税科目扱いにする。(外税・別記のみの表示になるため)

	if( data->dbt.IsEmpty() ) {	// 空欄は課税扱い
		if( data->cre.IsEmpty() ) { //貸方も空欄
			data->dbt.Format("%08x", kzeicd);
			data->cre = SYOGT;
			setkzei = 1;
		}
		else {
			//貸方輸入チェック
			DBKNREC* pCre = DB_PjisToKnrec(data->cre, pzm, FALSE);
			if( _sykmk_check(pCre) ) {
				data->cre.Format("%08x", kzeicd);
				setkzei = 1;
			}
			else {
				//消費税科目でない
				if( km_syattr(pCre, pzm) == 0 ) {
					data->dbt.Format("%08x", kzeicd);
					setkzei = 1;
				}
			}
		}
	}
	else {
		//借方輸入チェック
		DBKNREC* pDbt = DB_PjisToKnrec(data->dbt, pzm, FALSE);
		if( _sykmk_check(pDbt) ) {
			data->dbt.Format("%08x", kzeicd);
			setkzei = 1;
		}

		if( data->cre.IsEmpty() ) {
			data->cre = SYOGT;
			//借方が消費税科目でない
			if( km_syattr(pDbt, pzm) == 0 ) {
				data->cre.Format("%08x", kzeicd);
				setkzei = 1;
			}
		}
		else {
			//貸方輸入チェック
			DBKNREC* pCre = DB_PjisToKnrec(data->cre, pzm, FALSE);
			if( _sykmk_check(pCre) ) {
				data->cre.Format("%08x", kzeicd);
				setkzei = 1;
			}
		}
	}

//	data->val;
//	data->zei;
//	data->dbr 
//	data->cbr 
//	data->dbmn
//	data->cbmn
//
//	data->tekiyo
//	data->owner 
//	data->dtype 

	//data->imgsq
	//data->snumber
	//data->icno

	//data->skn
	//data->tag
	//data->tsign;

	//data->hjiyu;
	//data->taika;
	//::ZeroMemory(&data->dsign[0], SZ_SYZDSGN);

	return setkzei;
}



//消費税選択用データセット
int CSyzInpLiteSub::SetSyzData(CDBDATA* pdata)
{
	DBGFUNCTION;

	Copydbdata(&m_dbdata, pdata);

	DSIGN_TRACE(m_dbdata.dsign);

	int set = _sydata_initsub(m_pZm, m_pSy, &m_dbdata);

	BYTE chk[6] = { 0 };
	BOOL bZero = FALSE;
	if( memcmp(&m_dbdata.dsign[0], chk, sizeof chk) == 0 ) {
		bZero = TRUE;
	}
//	MyTrace("SetSyzData(0) %d, dbt=%s.%d, cre=%s.%d\n", m_dbdata.ddate, m_dbdata.dbt, m_dbdata.dbr, m_dbdata.cre, m_dbdata.cbr );

	BYTE oldMen = m_dbdata.dsign[6] & 0x20;

	int eda_chk = 1;
	//枝番消費税登録サイン ＯＦＦ
	if( !(m_pZm->zvol->s_sgn2 & 0x10) )
		eda_chk = 0;

	int sv_dbr, sv_cbr;
	sv_dbr = sv_cbr = -1;

	if( eda_chk ) {
		//if( m_dbdata.dbr != -1 ) {
		//	sv_dbr = m_dbdata.dbr;
		//	m_dbdata.dbr = -1;
		//}
		//if( m_dbdata.cbr != -1 ) {
		//	sv_cbr = m_dbdata.cbr;
		//	m_dbdata.cbr = -1;
		//}
	}

	m_pSy->SyIniz(&m_dbdata);

//MyTrace("SetSyzData(0-a) SyIniz Called\n");
	DSIGN_TRACE(m_dbdata.dsign);

	if( eda_chk ) {
		//if( sv_dbr != -1 ) {
		//	m_dbdata.dbr = sv_dbr;
		//}
		//if( sv_cbr != -1 ) {
		//	m_dbdata.cbr = sv_cbr;
		//}
	}

	//科目設定で、別記に設定した科目など、別記になった場合は "内税" にする
	if( bZero ) {
		DWORD syCode = 0;
		if( m_dbdata.dsign[0] & 0x40 && m_dbdata.dsign[2] & 0x01 ) {
			if( (m_dbdata.dsign[1] & 0xf0) == 0x20 ) {
				syCode = CD_UTIZ;
			}
		}
		//空欄の初期セット科目で外税になった場合は内税にする
		if( set ) {
			if( m_dbdata.dsign[0] & 0x40 && m_dbdata.dsign[2] & 0x01 ) {
				if( (m_dbdata.dsign[1] & 0xf0) != 0x10 ) {
					syCode = CD_UTIZ;
				}
			}
		}

		if( syCode != 0 ) {
			if( m_pSy->SyFnc(syCode) == 0 ) {
				struct _SY_MSG_PACK2 smpk;
				m_pSy->SyMsg(&smpk);
			}
		}
	}
//MyTrace("SetSyzData(2) %d, dbt=%s.%d, cre=%s.%d\n", m_dbdata.ddate, m_dbdata.dbt, m_dbdata.dbr, m_dbdata.cre, m_dbdata.cbr);
	DSIGN_TRACE(m_dbdata.dsign);

	if( oldMen == 0x20 && !(m_dbdata.dsign[6]&0x20) ) {
		// 免税事業者からの仕入れ
		m_pSy->SyFnc(CD_SP_EXEMPT);
	}

//MyTrace("SetSyzData(2-a) ===============================\n"); 
	DSIGN_TRACE(m_dbdata.dsign);
//MyTrace("%s:m_dbdata.invno = %s", __FUNCTION__, m_dbdata.invno);
	
	if( m_pDialog != NULL ) {
		m_pDialog->SyGetMessage();
	}
	else {
		SyGetMessage();
	}

	return 0;
}


//

int	CSyzInpLiteSub::UpdateSyzData(CDBDATA* pdata)
{
	DBGFUNCTION;

	int ds1, ds2;
	ds1 = GetSyzDateSign(m_dbdata.ddate);
	ds2 = GetSyzDateSign(pdata->ddate);

	Copydbdata(&m_dbdata, pdata);

	//税率のデフォルト日付サインが変わった場合は、SyIniz
	if( ds1 != ds2 ) {
		m_pSy->SyIniz(&m_dbdata);
	}

	SyGetMessage();

	return 0;
}


int	CSyzInpLiteSub::TekiyoSyzData(int tksyz)
{
	DBGFUNCTION;
//MyTrace("TekiyoSyzData(int tksyz)  tksyz = %d\n", tksyz);

	m_pSy->Sy_tkyo_inp(tksyz);
	SyGetMessage();

	return 0;
}


int	CSyzInpLiteSub::TekiyoSyzData(struct _SY_TKREC_* psytk)
{
	DBGFUNCTION;
//MyTrace("TekiyoSyzData(0) %d, tk_attr = %08x\n", psytk->tk_attr);
DSIGN_TRACE(psytk->tk_dsign);

	m_pSy->Sy_tkyo_inp(psytk);
	SyGetMessage();

	return 0;
}


int	CSyzInpLiteSub::SyGetMessage()
{
	DBGFUNCTION;
//MyTrace("SyGetMessage(0) %d, dbt=%s.%d, cre=%s.%d\n", m_dbdata.ddate, m_dbdata.dbt, m_dbdata.dbr, m_dbdata.cre, m_dbdata.cbr);

	if( karisyz_check() ) {
		m_sylinetext.Empty();
		return 0;
	}
	DSIGN_TRACE(m_dbdata.dsign);

	BYTE oldMen = m_dbdata.dsign[6] & 0x20;
	
	m_pSy->SyMsg(&m_smpk);

	BYTE newMen = m_dbdata.dsign[6] & 0x20;
//MyTrace("SyGetMessage menzei old- %02x, new- %02x\n", oldMen, newMen);
	if( oldMen != newMen ) {
		if( oldMen == 0x20 ) {
			// 免税 → 課税事業者からの仕入
			m_pSy->SyFnc(CD_SP_NO_EXEMPT);
		}
		else {
			// 課税 → 免税事業者からの仕入れ
			m_pSy->SyFnc(CD_SP_EXEMPT);
		}
	}

	m_sylinetext = m_pSy->Sy_get_message_short(&m_dbdata, 0x01);

	DSIGN_TRACE(m_dbdata.dsign);

	return 0;
}


// szvol がオープンされているか？
int CSyzInpLiteSub::szvol_check()
{
	if( m_pZm == NULL )
		return -1;

	if( m_pZm->szvol == NULL ) {
		if( m_pZm->SyohizeiOpen() ) {
			m_ErrMsg = "SyohizeiOpenに失敗しました。";
			return -1;
		}
		m_bSzvolOpen = TRUE;
	}
	return 0;
}

//仮受・仮払チェック
int CSyzInpLiteSub::karisyz_check(CDBDATA* pData/*= NULLL*/)
{
	CDBDATA* pd;

	if( pData == NULL )	pd = &m_dbdata;
	else {
		pd = pData;
	}
	int ret = 0;
	int ret2 = 0;
	int rval = 0;

	DBKNREC* pKn = DB_PjisToKnrec(pd->dbt, m_pZm, FALSE);
	if( pKn != NULL ) {
		ret = km_syattr(pKn, m_pZm);
	}
	pKn = DB_PjisToKnrec(pd->cre, m_pZm, FALSE);
	if( pKn != NULL ) {
		ret2 = km_syattr(pKn, m_pZm);
	}

	if( ret & 0x0200 || ret2 & 0x0200 ) {
		rval = 1;
	}

	return rval;
}


int CSyzInpLiteSub::SetPara(CDBZmSub* pZm, CWnd* parent)
{
	m_pZm = pZm;
	m_pParent = parent;

	//消費税
	if( m_pSy != NULL ) {
		if( m_bSzvolOpen ) {
			m_pZm->SyohizeiClose();
			m_bSzvolOpen = FALSE;
		}
		m_pSy->SyClose();
		delete m_pSy;
		m_pSy = NULL;
	}
		
	if( (m_pSy = new CDBSyohi()) == NULL ) {
		m_ErrMsg = "消費税オブジェクトの作成に失敗しました！";
		return -1;
	}
	if( szvol_check() != 0 ) 
		return -1;

	if( m_pSy->SyOpen(m_pZm, m_pZm->szvol) == -1 ) {
		WORD	sy_errno;
		char	sy_ermsg[256];
		m_pSy->Get_sy_error(&sy_errno, sy_ermsg);
		m_ErrMsg.Format("errno = %d, %s", sy_errno, sy_ermsg);
		delete m_pSy;
		return -1;
	} 

	HINSTANCE h = AfxGetResourceHandle();
	AfxSetResourceHandle(gInstance);

	BOOL bToku = FALSE;
	if( m_pZm->zvol->apno >= 0x50 )
		bToku = TRUE;

	// インボイス対応
	CZmGen8 zmGen;

	// バージョンアップマスターか判定
	m_bInvMaster = zmGen.IsInvoiceVupMaster( pZm );
	if( m_bInvMaster ) {
#ifdef CLOSE
		if(( pZm->zvol->s_sgn2 & 0x0f ) >= 2 ) {
			// 簡易課税の場合は免税事業者からの課税仕入れと登録番号を非表示にするため、インボイス期間外としておく
			m_bInvUse = false;
		}
		else {
#endif
			// インボイス期間マスターか
			m_bInvUse = zmGen.IsAbleToUseInvoice( pZm );
//		}
		if( m_bInvUse ) {
			m_Koujo80_50 = GetMenzeiWariai(m_pZm->zvol);
		}
		// 登録番号を表示するか
		m_bInvDisp = zmGen.IsDispInvno( pZm );
	}
	else {
		m_bInvUse = false;
		m_bInvDisp = false;
	}
	if( !(m_dispmode & 0x01) ){//インボイスを表示しない
		m_bInvMaster = false;
		m_bInvUse = false;
		m_bInvDisp = false;
	}

	MyTrace("%s:, %d, %d\n", __FUNCTION__, m_bInvMaster, m_bInvUse);
	
	//m_pDialog = new CSkbnLiteDlg(parent, m_bResize);
	m_pDialog = new CSkbnLiteDlg( parent, bToku, m_bResize, m_bInvMaster, m_bInvUse, m_bInvDisp, m_Koujo80_50);

	if( m_pDialog == NULL )	return -1;

	AfxSetResourceHandle(h);

	m_pDialog->GetWindowRect(&m_pDialog->m_DlgRect);

#ifdef TEST_CLOSE
	CRect rc1;
	rc1 = m_pDialog->m_orgRect;
	MyTrace("@CSyzInpSub::SetPara[m_orgRect] %d, %d, %d, %d, [w = %d, h = %d]\n", rc1.left, rc1.top, rc1.right, rc1.bottom,
		rc1.Width(), rc1.Height());
	rc1 = m_pDialog->m_DlgRect;
	MyTrace("@CSyzInpSub::SetPara %d, %d, %d, %d [w = %d, h = %d]\n", rc1.left, rc1.top, rc1.right, rc1.bottom,
		rc1.Width(), rc1.Height());
#endif

	m_pDialog->m_pZm = pZm;
	m_pDialog->m_pSy = m_pSy;
	m_pDialog->m_pDBDta = &m_dbdata;
	m_pDialog->m_pLineTxt = &m_sylinetext;
	m_pDialog->m_pSmpk = &m_smpk;
	m_pDialog->m_Litesub = this;

	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		GetInvoiceInfo();
		m_pDialog->SetInvArray(&m_InvAry);
		m_pDialog->SetInvPageCount(6);
	}
	else {
		m_pDialog->SetInvArray(NULL);
	}
	return 0;
}


int	CSyzInpLiteSub::SetParaNoDlg(CDBZmSub* pZm)
{
	m_pZm = pZm;

	//消費税
	if( m_pSy != NULL ) {
		if( m_bSzvolOpen ) {
			m_pZm->SyohizeiClose();
			m_bSzvolOpen = FALSE;
		}
		m_pSy->SyClose();
		delete m_pSy;
		m_pSy = NULL;
	}

	if( (m_pSy = new CDBSyohi()) == NULL ) {
		m_ErrMsg = "消費税オブジェクトの作成に失敗しました！";
		return -1;
	}

	if( szvol_check() != 0 )
		return -1;

	if( m_pSy->SyOpen(m_pZm, m_pZm->szvol) == -1 ) {
		WORD	sy_errno;
		char	sy_ermsg[256];
		m_pSy->Get_sy_error(&sy_errno, sy_ermsg);
		m_ErrMsg.Format("errno = %d, %s", sy_errno, sy_ermsg);
		delete m_pSy;
		return -1;
	}
	return 0;
}



int	CSyzInpLiteSub::DialogON(BOOL bOK, CRect rect, int sypos, int pn)
{
	if( m_pDialog == NULL )	return	-1;

	BOOL	st;
	int setpn;

	st = m_pDialog->skbn_dataset(&setpn);
	if( !st ) return -1;

	RECT	newrect;
	double width, height;

	width = m_pDialog->m_DlgRect.Width();
	height = m_pDialog->m_DlgRect.Height();

	FILETrace("SYZ DialogON TOP rect(%d, %d, %d,%d), sypos = %d\n", rect.left, rect.top, rect.right, rect.bottom, sypos );
	FILETrace("SYZ DialogON width, height( %d,%d)\n", width, height);

	// 09.09 /10
	double grx, gry;
	Dlg_GetRate(m_pDialog, grx, gry);

	if( m_bResize ) {
		if( grx != 0 && gry != 0 ) {
			width = grx * width;
			height = gry * height;
		}
	}
	FILETrace("SYZ DialogON grx, gry( %f,%f)\n", grx, gry);

	if( sypos == SYP_TOPLEFT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height; /*( height * y );*/
		newrect.right = rect.left;
		newrect.left = newrect.right - width; /*( width * x ); */
	}
	else if( sypos == SYP_TOPRIGHT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height; /*( height * y );*/
		newrect.left = rect.left;
		newrect.right = newrect.left + width; /*( width * x ); */
	}
	else if( sypos == SYP_BOTTOMLEFT ) {
		newrect.top = rect.bottom;
		newrect.bottom = newrect.top + height; /*( height * y );*/
		newrect.right = rect.left;
		newrect.left = newrect.right - width; /*( width * x ); */
	}
	else {
		newrect.top = rect.bottom;
		newrect.bottom = newrect.top + height; /*( height * y );*/
		newrect.left = rect.left;
		newrect.right = rect.left + width; /*( width * x ); */
	}
	CRect	paRect;
	m_pParent->GetWindowRect(paRect);
//	m_pParent->ScreenToClient(paRect);

	if( newrect.right > paRect.right ) {
		int n = newrect.right - paRect.right;
		newrect.left -= n;
		newrect.right -= n;
	}

	m_pDialog->EnableWindow(TRUE);
	WINDOWPLACEMENT	wp;
	m_pDialog->GetWindowPlacement(&wp);
	wp.showCmd = SW_SHOWNA;
	m_pDialog->SetWindowPlacement(&wp);
	m_pDialog->SetWindowPos(&CWnd::wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW);

	FILETrace("SYZ DialogON pos1( %d,%d,%d,%d,)\n", newrect.left, newrect.top, newrect.right, newrect.bottom);

	// ダイアログ位置の調整
	if( grx != 0 && gry != 0 ) {
		width = m_pDialog->m_DlgRect.Width();
		height = m_pDialog->m_DlgRect.Height();

		BOOL bVert, bHorz;

		m_pDialog->DlgRectCheck(bVert, bHorz);

		if( bVert || bHorz ) {
			if( !bVert ) {
				height = gry * height;
			}
			else {
				height = m_pDialog->GetDlgHeight();
			}
			if( !bHorz )	width = grx * width;
			else {
				width = m_pDialog->GetDlgWidth();
			}

FILETrace("SYZ DialogON bHorz(%d), bVert(%d)\n", bHorz, bVert);
FILETrace("SYZ DialogON height(%d), width(%d)\n", height, width);

			if( sypos == SYP_TOPLEFT ) {
				newrect.bottom = rect.top;
				newrect.top = newrect.bottom - height; /*( height * y );*/
				newrect.right = rect.left;
				newrect.left = newrect.right - width; /*( width * x ); */
			}
			else if( sypos == SYP_TOPRIGHT ) {
				newrect.bottom = rect.top;
				newrect.top = newrect.bottom - height; /*( height * y );*/
				newrect.left = rect.left;
				newrect.right = newrect.left + width; /*( width * x ); */
			}
			else if( sypos == SYP_BOTTOMLEFT ) {
				newrect.top = rect.bottom;
				newrect.bottom = newrect.top + height; /*( height * y );*/
				newrect.right = rect.left;
				newrect.left = newrect.right - width; /*( width * x ); */
			}
			else {
				newrect.top = rect.bottom;
				newrect.bottom = newrect.top + height; /*( height * y );*/
				newrect.left = rect.left;
				newrect.right = rect.left + width; /*( width * x ); */
			}

			m_pDialog->SetWindowPos(&CWnd::wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW);
		}
	}

	FILETrace("SYZ DialogON setpn = %08x, pn = %d\n", setpn, pn);

	if( pn == -1 ) {
		pn = SY_SZKBN_PN;
		//簡易課税・個別対応は、売上／仕入区分に
		//if( m_pSy->IsKaniMaster() && (setpn & (0x01 << SY_URISIRE_PN)) ) {
		//	pn = SY_URISIRE_PN;
		//}
		//if( m_pSy->IsKobetsuMaster() && (setpn & (0x01 << SY_URISIRE_PN)) ) {
		//	pn = SY_URISIRE_PN;
		//}
	}
	else {
		if( pn != SY_ZRITU_PN && pn != SY_SZKBN_PN )
			pn = SY_SZKBN_PN;
	}

FILETrace("SYZ set_fastfocus pn = %d\n", pn);
	m_pDialog->set_fastfocus(bOK, pn);

	m_pDialog->m_Dispflg = TRUE;

	return 0;
}


int CSyzInpLiteSub::DialogOFF()
{
	DBGFUNCTION;

	if( m_pDialog == NULL )	return	-1;
	//	m_pDialog->GetWindowRect( m_pDialog->DlgRect );
	//	pParent->ScreenToClient( m_pDialog->DlgRect );
	m_pDialog->ShowWindow(SW_HIDE);
//		m_pDialog->MoveWindow(0,0,0,0);
	m_pDialog->EnableWindow(FALSE);

	m_pDialog->m_Dispflg = FALSE;

	return 0;
}


BOOL CSyzInpLiteSub::IsDisplay()
{
	if( m_pDialog == NULL )	return	FALSE;

	return m_pDialog->m_Dispflg;
}


void CSyzInpLiteSub::GetCrntRect(CSize& size)
{
	if( m_pDialog == NULL ) {
		size.cx = 0; size.cy = 0;
		return;
	}

	RECT	newrect;
	double width, height;

	width = m_pDialog->m_DlgRect.Width();
	height = m_pDialog->m_DlgRect.Height();

	// 09.09 /10
	double grx, gry;
	Dlg_GetRate(m_pDialog, grx, gry);

	if( grx != 0 && gry != 0 ) {
		width = grx * width;
		height = gry * height;
	}

	newrect.top = m_pDialog->m_DlgRect.top;
	newrect.left = m_pDialog->m_DlgRect.left;
	newrect.bottom = newrect.top + height;
	newrect.right = newrect.left + width;

	/*
		newrect.top		= m_pDialog->m_DlgRect.top;
		newrect.bottom	= m_pDialog->m_DlgRect.bottom;
		newrect.right	= m_pDialog->m_DlgRect.right;
		newrect.left	= m_pDialog->m_DlgRect.left;
	*/
	//	newrect.top = newrect.bottom - ( height * y );
	//	newrect.right = m_pDialog->m_DlgRect.left + (x * 10);
	//	newrect.left = newrect.right - ( width * x ) + (y * 10);

	size.cx = newrect.right - newrect.left;
	size.cy = newrect.bottom - newrect.top;
}


int	 CSyzInpLiteSub::SetSkbnSelNo(int no)
{
	if( m_pDialog == NULL )	return	-1;

	return m_pDialog->SetSkbnSelNo(no);
}

void CSyzInpLiteSub::set_nextpos()
{
	if( m_pDialog == NULL )	return;

	m_pDialog->set_nextpos();
}

void CSyzInpLiteSub::set_nowfocus()
{
	if( m_pDialog == NULL )	return;

	m_pDialog->set_nowfocus();
}

CString CSyzInpLiteSub::GetSyzLineText()
{
	return m_sylinetext;
//	return m_pDialog->m_sylinetxt;
}

void CSyzInpLiteSub::GetNowSyzData(CDBDATA* pdata)
{
	Copydbdata(pdata, &m_dbdata);

	//DSIGN_TRACE(m_dbdata.dsign);
	//MyTrace("%s:m_dbdata.invno = %s", __FUNCTION__, m_dbdata.invno);
}


void CSyzInpLiteSub::Close()
{
	if( m_pSy != NULL ) {
		if( m_bSzvolOpen ) {
			m_pZm->SyohizeiClose();
			m_bSzvolOpen = FALSE;
		}
		m_pSy->SyClose();
		delete m_pSy;
		m_pSy = NULL;
		if( m_pDialog ) {
			m_pDialog->m_pSy = m_pSy;
		}
	}
}
void CSyzInpLiteSub::SetDispMode( int mode )
{
	m_dispmode = mode;
}
//=============================================================================
// 摘要情報から登録番号の取得
//=============================================================================
int CSyzInpLiteSub::GetInvoiceInfo()
{
	// インボイス対応
	::GetInvoiceInfo(m_pZm, m_InvAry);

	return 0;
}

//=============================================================================
// 財務クラス再読み込み
//=============================================================================
void CSyzInpLiteSub::ReloadData()
{
	if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
		GetInvoiceInfo();
		m_pDialog->SetInvArray( &m_InvAry );
	}
	else {
		m_InvAry.RemoveAll();
		m_pDialog->SetInvArray(NULL);
	}
}


//=============================================================================
// 
//=============================================================================
BOOL CSyzInpLiteSub::CheckInvno()
{
	return TRUE;
}

//=============================================================================
//『外税カットしない』確認
//=============================================================================
BOOL CSyzInpLiteSub::SotoNotCut()
{
//	m_dispmode |= 0x02;

	return (m_dispmode & 0x02) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// エクスポートクラス
CSyzInpLite::CSyzInpLite()
{
	pSub = NULL;
	if( (pSub = new CSyzInpLiteSub) == NULL )
		return;
}


CSyzInpLite::~CSyzInpLite()
{
	TRACE("~CSyzInpLite psub = %08x", pSub);
	if( pSub ) {
		delete pSub;
		pSub = NULL;
	}
}


void CSyzInpLite::SetDialogResize(BOOL bResize)
{
	pSub->m_bResize = bResize;
}


int CSyzInpLite::SetPara(CDBZmSub* pZm, CWnd* parent)
{
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->SetPara(pZm, parent);
}


int CSyzInpLite::SetParaNoDlg(CDBZmSub* pZm)
{
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->SetParaNoDlg(pZm);
}


int	CSyzInpLite::SetSyzData(CDBDATA* pdata)
{
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->SetSyzData(pdata);
}


int	CSyzInpLite::UpdateSyzData(CDBDATA* pdata)
{
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->UpdateSyzData(pdata);
}


int	CSyzInpLite::TekiyoSyzData(int tksyz)
{
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->TekiyoSyzData(tksyz);
}


int	CSyzInpLite::TekiyoSyzData(struct _SY_TKREC_* psytk)
{
	if( pSub == NULL ) {
		return -1;
	}

	return pSub->TekiyoSyzData(psytk);
}


int	CSyzInpLite::DialogON(BOOL bOK, CRect rect, int posflg, int pn)
{
	return pSub->DialogON(bOK, rect, posflg, pn);
}


int CSyzInpLite::DialogOFF()
{
	DBGFUNCTION;
	return pSub->DialogOFF();
}

BOOL CSyzInpLite::IsDisplay()
{
	return pSub->IsDisplay();
}


void CSyzInpLite::GetCrntRect(CSize& size)
{
	pSub->GetCrntRect(size);
}


int	 CSyzInpLite::SetSkbnSelNo(int no)
{
	return pSub->SetSkbnSelNo(no);
}


void CSyzInpLite::set_nextpos()
{
	pSub->set_nextpos();
}

void CSyzInpLite::set_nowfocus()
{
	pSub->set_nowfocus();
}


CString CSyzInpLite::GetSyzLineText()
{
	return pSub->GetSyzLineText();
}

void CSyzInpLite::GetNowSyzData(CDBDATA* pdata)
{
	return pSub->GetNowSyzData(pdata);
}


void CSyzInpLite::Close()
{
	return pSub->Close();
}
void CSyzInpLite::SetDispMode( int mode )
{
	pSub->SetDispMode( mode );
}


void CSyzInpLite::ReloadData()
{
	pSub->ReloadData();
}

BOOL CSyzInpLite::CheckInvno()
{
	return pSub->CheckInvno();
}
