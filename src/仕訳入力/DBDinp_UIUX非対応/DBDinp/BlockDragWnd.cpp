// BlockDragWnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "BlockDragWnd.h"

#include "MainFrm.h"


#define BDW_STARTID		2000

#define BDW_XOFS		5
#define BDW_YOFS		4

// CBlockDragWnd

IMPLEMENT_DYNAMIC(CBlockDragWnd, CWnd)

CBlockDragWnd::CBlockDragWnd()
{
	RegisterWindowClass();

	m_WndIdx.SetSize( BDW_WNDMAX );
	for( int i = 0; i < BDW_WNDMAX; i++ ) {
		m_WndIdx[i] = 0;
	}
	m_WndCnt = 0;

	m_MoveToIndex = -1;
	m_MovingIndex = -1;

	m_pScrnBmp = NULL;
	m_pDragWnd = NULL;

	m_bSelShow = FALSE;
}

CBlockDragWnd::~CBlockDragWnd()
{
	for( int i = 0; i < m_WndCnt; i++ ) {
		m_pBwnd[i]->DestroyWindow();
		delete m_pBwnd[i];
	}

	if( m_pDragWnd != NULL ) {
		m_pDragWnd->DestroyWindow();
		delete m_pDragWnd;
	}
}


// Register the window class if it has not already been registered.
BOOL CBlockDragWnd::RegisterWindowClass()
{
    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, BLOCKDRAGWND_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
        wndcls.hCursor          = 0;
#endif
    //	wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.hbrBackground    = ((CMainFrame*)GetDBDinpMainFrame())->GetViewColorBrush();
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = BLOCKDRAGWND_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}


BEGIN_MESSAGE_MAP(CBlockDragWnd, CWnd)
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE( BDW_MESSAGE, OnBDWMessage )
//	ON_WM_CTLCOLOR()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


#ifdef OLD_CLOSE
void CBlockDragWnd::DispBmpMake( CWnd* pwnd )
{
//	CWnd* pwnd = GetDesktopWindow();
//	CWnd* pwnd = GetParent();
	m_pScrnWnd = pwnd;

	CDC* pdc = m_pScrnWnd->GetDC();
	CRect bmRect;
	m_pScrnWnd->GetClientRect(&bmRect);
	m_ScrnBmp.CreateCompatibleBitmap(pdc, bmRect.Width(), bmRect.Height());

	CDC mDC;
	mDC.CreateCompatibleDC(pdc);
	//デスクトップイメージをbitmapへ描写する
	CGdiObject* pOld = mDC.SelectObject(&m_ScrnBmp);
	mDC.BitBlt(0, 0, bmRect.Width(), bmRect.Height(), pdc, 0, 0, SRCCOPY);
	mDC.SelectObject(pOld);
	mDC.DeleteDC();

	m_pScrnWnd->ReleaseDC( pdc );
}
#endif

//------------------------------------------------
//	m_WndIdx[0] = 1 --- [科目選択部分]
//	m_WndIdx[1] = 2 --- [既入力仕訳リスト部]
//	m_WndIdx[2] = 3 --- [入力タイプ表示部]
//	m_WndIdx[3] = 4 --- [新規入力欄 ]
//	m_WndIdx[4] = 5 --- [現金貸借部分 ]
//------------------------------------------------

void CBlockDragWnd::SetBlockInfo( BlockInfo* pBlk )
{
	int max = __min( pBlk->blkCnt, BDW_WNDMAX );

	m_pScrnBmp = pBlk->pBmp;
	m_WndCnt = max;

	m_bSelShow = pBlk->selDisp ? TRUE:FALSE;

	for( int i = 0; i < max; i++ ) {
		m_BmpRect[i].CopyRect( *(pBlk->pRect+i) );

		if( pBlk->pBlkIdx != NULL ) {
			// ゼロは初期値
			m_WndIdx[i] = *(pBlk->pBlkIdx+i);
		}
		else {
			m_WndIdx[i] = i+1;
		}

		CString* ps;
		ps = &pBlk->pBlkStr[i];
		m_BlkStr.Add( (LPCTSTR)*ps );
	}

	m_mode		= pBlk->mode;
	m_bBmpMake	= pBlk->bBmpMake;
}


BOOL CBlockDragWnd::Initialize()
{
	RECT rect;
	int id, max;

//	ModifyStyle(0,  WS_CLIPCHILDREN);

	max = __min( m_WndCnt, BDW_WNDMAX );

	m_WndCnt = max;
	CRect rcWnd;
	GetClientRect( rcWnd );

	CDWordArray	bmHtAry;
	int bmpTotalHt = 0;
	double ritu;

#ifdef OLD_CLOSE
	for( int i = 0; i < max; i++ ) {
		CBitmap bmp;
		bmp.LoadBitmap( IDB_DINPBMP1+i );
		BITMAP bi;
		::ZeroMemory( &bi, sizeof BITMAP );
		bmp.GetBitmap( &bi );
		bmp.DeleteObject();

		bmHtAry.Add( bi.bmHeight );
		bmpTotalHt += bi.bmHeight;
	}
#else
	for( int i = 0; i < max; i++ ) {
		bmHtAry.Add( m_BmpRect[i].Height() );
		bmpTotalHt += m_BmpRect[i].Height();
	}
#endif

	ritu = (double)( (double)(rcWnd.Height() - (BDW_YOFS*2)) / (double)bmpTotalHt);

	for( int i = 0; i < max; i++ ) {
		m_pBwnd[i] = new CBmpDspWnd(this);

		id = BDW_STARTID+i;
	//	id = BDW_STARTID+ (m_WndIdx[i]-1);

		CString str;
		str.Format( "id = %d", id );

		DWORD dwStaticStyle = /*SS_NOTIFY |SS_ETCHEDFRAME*/WS_EX_CLIENTEDGE | WS_EX_TRANSPARENT;

		if( i == 0 ) {
			rect.top	= BDW_YOFS;
		}
		else {
			rect.top	= rect.bottom;
		}
		rect.left	= 0;
		rect.bottom	= rect.top + (bmHtAry[i] * ritu);
	//	rect.right	= rect.left + /*bi.bmWidth*/ rcWnd.Width()-10;
		rect.right	= rect.left + rcWnd.Width() - BDW_XOFS;

		m_pBwnd[i]->m_nID = id;

		if( ! m_mode ) {
		//	m_pBwnd[i]->SetBitmapID( IDB_DINPBMP1+i);

			if( i == 0 ) {
				if( ! m_bSelShow && !m_bBmpMake ) {
					m_pBwnd[i]->SetDispBitmap( NULL, m_BmpRect[i], m_BlkStr[i] );
				}
				else {
					m_pBwnd[i]->SetDispBitmap( m_pScrnBmp, m_BmpRect[i], m_BlkStr[i] );
				}
			}
			else {
				m_pBwnd[i]->SetDispBitmap( m_pScrnBmp, m_BmpRect[i], m_BlkStr[i] );
			}
		}
		else {
			m_pBwnd[i]->SetDispBitmap( NULL, m_BmpRect[i], m_BlkStr[i] );
		}

		m_pBwnd[i]->Create( NULL, "test", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
								rect, this, id );

//		m_pBwnd[i]->CreateEx( dwStaticStyle, NULL, "test",WS_CHILD | WS_BORDER,
//								rect, NULL, id );
//		m_pBwnd[i]->ModifyStyleEx(0, dwStaticStyle, 0);

		m_pBwnd[i]->SetWindowText(str);
	}

	BmpWndSortDisp();

	return TRUE;
}


// CBlockDragWnd メッセージ ハンドラ

BOOL CBlockDragWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	return CWnd::PreTranslateMessage(pMsg);
}

UINT CBlockDragWnd::OnGetDlgCode()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
    UINT nCode =  DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTTAB | DLGC_WANTARROWS;

 	return nCode;
//	return CWnd::OnGetDlgCode();
}

void CBlockDragWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CWnd* pwnd = GetDlgItem(BDW_STARTID);

	if( pwnd != NULL ) {
		((CDragStatic*)pwnd)->m_Back = RGB(0,0,128);
		pwnd->Invalidate();
		pwnd->UpdateWindow();
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


// BDW_MESSAGE メッセージ処理
LRESULT CBlockDragWnd::OnBDWMessage( WPARAM w, LPARAM l )
{
	CRect clRect;
	GetClientRect( clRect );

	if( w == BDWM_MOUSEMOVE ) {
		int x = HIWORD( l );
		int y = LOWORD( l );
		CPoint pt( x, y );
		ScreenToClient( &pt );

		CRect rect;

		if( m_pDragWnd != NULL ) {
			m_pDragWnd->GetWindowRect( &rect );
			ScreenToClient( &rect );

			CPoint pt2( rect.left, rect.bottom );
			int pos = WndBlockPosFromPoint( &pt2 );

			BOOL bOutSide = FALSE;
			if( ! clRect.PtInRect( pt2 ) ) {
				bOutSide = TRUE;
			}
			// 移動先の下線を描画
			if( pos != m_MoveToIndex && ! bOutSide) {
				DrawTheLines( pos );
			}
			//ポインタがドラッグウィンドウから外れた。
			m_pDragWnd->GetWindowRect( &rect );
			CPoint mouse_pt( HIWORD( l ), LOWORD( l ) );
			if( ! rect.PtInRect( mouse_pt ) ) {
				PostMessage( BDW_MESSAGE, BDWM_LBUTTONUP, 0 );
			}
		}
	}
	else if( w == BDWM_LBUTTONDOWN ) {
		int x = HIWORD( l );
		int y = LOWORD( l );
		CPoint pt( x, y );
		ScreenToClient( &pt );

		m_MovingIndex = -1;
		m_MoveToIndex = -1;

		if( m_pDragWnd == NULL ) {
			m_pDragWnd = new CDragStatic(this);

			CRect rect, bmRC;

			m_pSelBmpWnd = GetBmpWndPtr( &pt );

			int id = BDW_STARTID + m_WndCnt + 10;

			if( m_pSelBmpWnd != NULL ) {
				for( int n = 0; n < m_WndCnt; n++ ) {
					m_pBwnd[n]->CursorChangeFlg() = FALSE;
				}
				m_pSelBmpWnd->GetWindowRect( rect );
				ScreenToClient( rect );
				m_pSelBmpWnd->DspWndSelected() = TRUE;
				m_pSelBmpWnd->Invalidate();
				m_pSelBmpWnd->UpdateWindow();

				CString str;
				str.Format( "pBmpWnd = %08x", m_pSelBmpWnd );

//				DWORD dwStaticStyle = /*SS_NOTIFY |SS_ETCHEDFRAME*/WS_EX_CLIENTEDGE | WS_EX_TRANSPARENT;
				m_pSelBmpWnd->GetBitmapRect( bmRC );
				if( ! m_mode ) {
					m_pDragWnd->SetDispBitmap( m_pScrnBmp, &bmRC, m_pSelBmpWnd->m_Txt );
				}
				else {
					m_pDragWnd->SetDispBitmap( NULL, &bmRC, m_pSelBmpWnd->m_Txt );
				}

		//		m_pDragWnd->Create( NULL, str, WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CLIPSIBLINGS,
		//								rect, this, id );
				m_pDragWnd->CreateEx( WS_EX_TOOLWINDOW, AfxRegisterWndClass(0), str, WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CLIPSIBLINGS,
										rect, this, id );

				m_pDragWnd->SetDragCursor( (UINT)IDC_SIZEALL/*IDC_HAND*/ );
				m_pDragWnd->ActivateDrag();

				//移動する ウィンドウインデックスをセット
				m_MovingIndex = WndBlockPosFromPoint( &pt );
			}
		}
	}
	else if( w == BDWM_LBUTTONUP ) {
		if( m_pDragWnd != NULL ) {
			m_pDragWnd->DestroyWindow();
			delete m_pDragWnd;
			m_pDragWnd = NULL;

			for( int n = 0; n < m_WndCnt; n++ ) {
				m_pBwnd[n]->CursorChangeFlg() = TRUE;
			}

			m_pSelBmpWnd->DspWndSelected() = FALSE;
			m_pSelBmpWnd->RedrawWindow();
			m_pSelBmpWnd = NULL;

			RedrawWindow();
		}
	}
	else if( w == BDWM_DEACTIVEDRAG ) {
		if( m_pDragWnd != NULL ) {
			m_pDragWnd->DestroyWindow();
			delete m_pDragWnd;
			m_pDragWnd = NULL;
		}
		for( int n = 0; n < m_WndCnt; n++ ) {
			m_pBwnd[n]->CursorChangeFlg() = TRUE;
		}

		Invalidate();
		UpdateWindow();

		if( m_MovingIndex != -1 ) {
			int nInsVal = m_WndIdx[ m_MovingIndex ];

			if( nInsVal == BLK_DTNEW || nInsVal == BLK_DTLIST ) {
				CDWordArray	dwAry;
				dwAry.Copy( m_WndIdx );

				// 新規入力
				if( m_MoveToIndex < m_MovingIndex ) {
					dwAry.RemoveAt( m_MovingIndex );
					if( m_MoveToIndex == -1 ) {
						dwAry.InsertAt( m_WndCnt-1, nInsVal );
					}
					else {
						dwAry.InsertAt( m_MoveToIndex, nInsVal );
					}
				}
				else {
					dwAry.InsertAt( m_MoveToIndex, nInsVal );
					dwAry.RemoveAt( m_MovingIndex );
				}

				int list_blk = 0;
				for( int k = 0; k < dwAry.GetCount(); k++ ) {
					if( dwAry[k] == BLK_DTNEW ) {
						break;
					}
					else if( dwAry[k] == BLK_DTLIST ) {
						list_blk++;
					}
				}

				if( ! list_blk ) {
					ICSMessageBox( "新規入力行は、仕訳リストより上側に移動できません！", 0,0,0,this);
				}
				else {
					m_WndIdx.RemoveAll();
					m_WndIdx.Copy( dwAry );
				}
			}
			else {
				if( m_MoveToIndex < m_MovingIndex ) {

					m_WndIdx.RemoveAt( m_MovingIndex );
					if( m_MoveToIndex == -1 ) {
						m_WndIdx.InsertAt( m_WndCnt-1, nInsVal );
					}
					else {
						m_WndIdx.InsertAt( m_MoveToIndex, nInsVal );
					}
				}
				else {
					m_WndIdx.InsertAt( m_MoveToIndex, nInsVal );
					m_WndIdx.RemoveAt( m_MovingIndex );
				}
			}
		}

		m_pSelBmpWnd->DspWndSelected() = FALSE;
		m_pSelBmpWnd->Invalidate();
		m_pSelBmpWnd->UpdateWindow();
		m_pSelBmpWnd = NULL;

		BmpWndSortDisp();

		m_MovingIndex = -1;
		m_MoveToIndex = -1;

#ifdef _DBG_DISP
CDC *pDC = GetDC();
CString strDbg;
		for( int n = 0; n < m_WndCnt; n++ ) {
strDbg.Format( "m_WndIdx[%d] = %d", n, m_WndIdx[n] );
pDC->TextOutA( 50, 10+(n*20), strDbg );
		}
ReleaseDC( pDC );
#endif/**/
	}

	return 1;
}

//--------------------------------------------------------------
//	x,y	座標より　ウィンドウブロック表示ウィンドウ を 取得
//		返送値	ウィンドウポインタ
//
//--------------------------------------------------------------
CBmpDspWnd* CBlockDragWnd::GetBmpWndPtr( LPPOINT pt )
{
	CBmpDspWnd* pdw = NULL;

	int idx = WndIdxFromPoint( pt );

	if( idx > 0 ) {
		pdw = m_pBwnd[idx-1];
	}
	return pdw;
}


//--------------------------------------------------------------
//	x,y	座標より　ウィンドウブロック位置を 取得
//		返送値	0 から n まで( 0 が上側 )
//
//--------------------------------------------------------------
int	CBlockDragWnd::WndBlockPosFromPoint( LPPOINT pt )
{
	int pos = -1;
	int nTmpid;

	for( int i = 0; i < m_WndCnt; i++ ) {
		int n = m_WndIdx[i];
	
		//選択部分非表示
		if( ! m_bSelShow ) {
			if( n == BLK_SELECT )
				continue;
		}

		if( n > 0 )	n--;

		nTmpid = BDW_STARTID + n;
		CRect rcWnd;
		GetDlgItem(nTmpid)->GetWindowRect( rcWnd );
		ScreenToClient( rcWnd );

		if( rcWnd.PtInRect( *pt ) ) {
			pos = i;
			break;
		}
	}

	return pos;
}

//--------------------------------------------------------------
//	x,y	座標より　ウィンドウブロック の id を 取得
//		返送値	1 から n まで ( 位置は 入れ替わっている可能性 )
//
//--------------------------------------------------------------
int CBlockDragWnd::WndIdxFromPoint( LPPOINT pt )
{
	int idx = 0;
	int nTmpid;

	for( int i = 0; i < m_WndCnt; i++ ) {
		int n = m_WndIdx[i];

		//選択部分非表示
		if( ! m_bSelShow ) {
			if( n == BLK_SELECT )
				continue;
		}

		if( n > 0 )	n--;

		nTmpid = BDW_STARTID + n;
		CRect rcWnd;
		GetDlgItem(nTmpid)->GetWindowRect( rcWnd );
		ScreenToClient( rcWnd );

		if( rcWnd.PtInRect( *pt ) ) {
			idx = (n+1);
			break;
		}
	}

	return idx;
}

//--------------------------------------------------------------
//	x,y	座標より　ウィンドウブロック表示ウィンドウ ID を 取得
//		返送値	ウィンドウ ID
//
//--------------------------------------------------------------
int CBlockDragWnd::BmpWndIDFromPoint( LPPOINT pt )
{
	int idx = 0;
	int nTmpid = 0;

	idx = WndIdxFromPoint( pt );

	if( idx > 0 ) {
		nTmpid = BDW_STARTID + (idx - 1);
	}

	return nTmpid;
}



void CBlockDragWnd::GetItemRect(int Index, CRect* pRect )
{
	if( Index >= m_WndCnt || Index < 0 ) {
		pRect->SetRect( -1,-1,-1,-1 );
		return;
	}
	//選択部分非表示
	if( ! m_bSelShow ) {
		if( m_WndIdx[Index] == BLK_SELECT ) {
			pRect->SetRect( -1,-1,-1,-1 );
			return;
		}
	}

	int id = BDW_STARTID + (m_WndIdx[Index] - 1);

	CRect rcWnd;
	GetDlgItem(id)->GetWindowRect( rcWnd );

	ScreenToClient( rcWnd );

	pRect->CopyRect( &rcWnd );
}

// 移動先の下線描画
void CBlockDragWnd::DrawTheLines(int Index)
{
	CRect ClientRect;
	GetClientRect(&ClientRect);

	CDC *pDC = GetDC();

	CRect Rect;
	int Width = 3;
	if (m_MoveToIndex != m_MovingIndex)
	{
		CPen Pen(PS_SOLID,Width, GetSysColor( COLOR_3DFACE )/*RGB(255,255,255)*/);
		CPen *pOldPen = pDC->SelectObject(&Pen);
		if (m_MoveToIndex != -1 )
		{
			GetItemRect(m_MoveToIndex,&Rect);
			if (ClientRect.PtInRect(Rect.TopLeft()))
			{
				pDC->MoveTo(Rect.left,Rect.top+1);
				pDC->LineTo(ClientRect.right-(Width/2),Rect.top+1);
			}
		}
		else
		{
			GetItemRect( m_WndCnt-1,&Rect);
			if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
			{
				pDC->MoveTo(Rect.left,Rect.bottom);
				pDC->LineTo(ClientRect.right-(Width/2),Rect.bottom);
			}
		}
		pDC->SelectObject(pOldPen);
	}
//CString strDbg;
//strDbg.Format( "To = %2d, ing = %2d  ", m_MoveToIndex, m_MovingIndex );
//pDC->TextOutA( ClientRect.right - 150, 10, strDbg );

	//移動しようとしている インデックス
	m_MoveToIndex = Index;

//strDbg.Format( "Index = %3d", Index );
//pDC->TextOutA( ClientRect.right - 150, 30, strDbg  );

//strDbg.Format( "(3) = %3d, %3d", m_MoveToIndex, m_MovingIndex );
//pDC->TextOutA( ClientRect.right - 150, 50, strDbg  );

	if (m_MoveToIndex != m_MovingIndex)
	{
		CPen Pen(PS_SOLID,Width,RGB(255,0,0));
		CPen *pOldPen = pDC->SelectObject(&Pen);
		if (m_MoveToIndex != -1 )
		{
			GetItemRect(Index,&Rect);
			if (ClientRect.PtInRect(Rect.TopLeft()))
			{
				pDC->MoveTo(Rect.left,Rect.top+1);
				pDC->LineTo(ClientRect.right-(Width/2),Rect.top+1);
			}
		}
		else
		{
			GetItemRect( m_WndCnt-1,&Rect);

			CRect drRect;
			if( m_pDragWnd != NULL ) {
				m_pDragWnd->GetWindowRect( &drRect );
				ScreenToClient( &drRect );
			}

		//	if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
			if( drRect.bottom >= Rect.bottom )
			{
				pDC->MoveTo(Rect.left,Rect.bottom);
				pDC->LineTo(ClientRect.right-(Width/2),Rect.bottom);
			}
		}
		pDC->SelectObject(pOldPen);
	}

	ReleaseDC(pDC);
}



//--------------------------------------------------------------
//	表示BMPウィンドウを 並び替えて移動
//
//--------------------------------------------------------------
int CBlockDragWnd::BmpWndSortDisp()
{
	int wndcnt = 0;
	int nTmpid = 0;
	CRect rect, bmpRect;

	for( int n = 0; n < m_WndCnt; n++ ) {
		nTmpid = BDW_STARTID + (m_WndIdx[n] - 1);
		//選択部分非表示
		if( m_WndIdx[n] == BLK_SELECT ) {
			if( ! m_bSelShow ) {
				GetDlgItem(nTmpid)->ShowWindow( SW_HIDE );
				continue;
			}
			else {
				GetDlgItem(nTmpid)->ShowWindow( SW_SHOW );
			}
		}

		GetDlgItem(nTmpid)->GetWindowRect( &bmpRect );
		ScreenToClient( &bmpRect );

		if( wndcnt == 0 ) {
			rect.top	= BDW_YOFS;
		}
		else {
			rect.top	= rect.bottom;
		}
		wndcnt++;

		rect.left = bmpRect.left;
		rect.right = bmpRect.right;
		rect.bottom = rect.top + bmpRect.Height();

		GetDlgItem(nTmpid)->MoveWindow( &rect );
	}

	return 0;
}

//--------------------------------------------------
//		ブロック情報を返送する。
//
//
//--------------------------------------------------
int CBlockDragWnd::GetBlockIdx( int* pBlkIdx )
{
	for( int n = 0; n < m_WndCnt; n++ ) {
		*(pBlkIdx + n) = m_WndIdx[n];
	}

	return m_WndCnt;
}


void	CBlockDragWnd::SetSelDispShow( BOOL bShow )
{
	m_bSelShow = bShow;

	BmpWndSortDisp();
}


BOOL	CBlockDragWnd::GetSelDispShow()
{
	return m_bSelShow;
}


//-----------------------------------
//		初期設定に戻す。
//-----------------------------------
void CBlockDragWnd::InitDispBlk()
{
	m_WndIdx.RemoveAll();

	m_WndIdx.Add( BLK_SELECT );
	m_WndIdx.Add( BLK_DTLIST );
	m_WndIdx.Add( BLK_INPTYPE );
	m_WndIdx.Add( BLK_DTNEW );
	m_WndIdx.Add( BLK_GNKIN );

//	m_bSelShow = FALSE;
	BmpWndSortDisp();
}

//HBRUSH CBlockDragWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  ここで DC の属性を変更してください。
//
//	// TODO:  既定値を使用したくない場合は別のブラシを返します。
//
//	hbr = (HBRUSH)m_brBack.GetSafeHandle();
//
//	return hbr;
//}

//BOOL CBlockDragWnd::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//	return CWnd::OnEraseBkgnd(pDC);
//	BOOL ret=CWnd::OnEraseBkgnd(pDC);	//派生元の関数を呼ぶ
//
//	pDC->SetBkColor( m_clBack );		//これが無いとTextOut()の背景が変わらない
//
//	return ret;
//}
