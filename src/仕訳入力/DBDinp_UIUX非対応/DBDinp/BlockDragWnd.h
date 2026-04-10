#pragma once

// CBlockDragWnd


#include "BmpDspWnd.h"
#include "DragStatic.h"

// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
#define BLOCKDRAGWND_CLASSNAME    _T("BlockDragWndClass")  // Window class name


#define BDW_WNDMAX		10

#define	BDW_MESSAGE		(WM_APP+1)

#define	BDWM_MOUSEMOVE		1
#define	BDWM_LBUTTONDOWN	2
#define	BDWM_LBUTTONUP		3
#define	BDWM_DEACTIVEDRAG	4



typedef struct _BlockInfo
{
	int			mode;		/* :0 bitmap, :1文字列*/
	CBitmap*	pBmp;
	CRect*		pRect;
	int			blkCnt;
	int*		pBlkIdx;
	BOOL		selDisp;	//TRUE:選択画面表示
	BOOL		bBmpMake;	//TRUE:完全な画面ビットマップ作成済み

	CString*	pBlkStr;
} BlockInfo;



class CBlockDragWnd : public CWnd
{
	DECLARE_DYNAMIC(CBlockDragWnd)

public:
	CBlockDragWnd();
	virtual ~CBlockDragWnd();

	BOOL RegisterWindowClass();
	BOOL Initialize();

	void GetItemRect(int Index, CRect* pRect );
	void DrawTheLines(int Index);

	CBmpDspWnd* GetBmpWndPtr( LPPOINT pt );

	int	BmpWndIDFromPoint( LPPOINT pt );
	int	WndIdxFromPoint( LPPOINT pt );
	int	WndBlockPosFromPoint( LPPOINT pt );

	int		BmpWndSortDisp();

	void	SetBlockInfo( BlockInfo* pBlk );
	int		GetBlockIdx( int* pBlkIdx );

#ifdef OLD_CLOSE
	void	DispBmpMake( CWnd* pwnd );
#endif

	void	SetSelDispShow( BOOL bShow );
	BOOL	GetSelDispShow();

	void	InitDispBlk();

protected:
	DECLARE_MESSAGE_MAP()

	CBmpDspWnd*		m_pBwnd[BDW_WNDMAX];
	CBmpDspWnd*		m_pSelBmpWnd;
	CDragStatic*	m_pDragWnd;

	CDWordArray		m_WndIdx;
	int				m_WndCnt;

	int		m_MoveToIndex, m_MovingIndex;

	CWnd*		m_pScrnWnd;
//	CBitmap		m_ScrnBmp;
	CBitmap*	m_pScrnBmp;
	CRect		m_BmpRect[BDW_WNDMAX];

	CStringArray	m_BlkStr;

	int			m_mode;	//0:ビットマップモード, 1:テキストモード

	BOOL		m_bBmpMake;
	BOOL		m_bSelShow;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	LRESULT	OnBDWMessage( WPARAM w, LPARAM l );

//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


