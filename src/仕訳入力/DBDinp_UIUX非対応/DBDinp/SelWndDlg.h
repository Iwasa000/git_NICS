#pragma once


// CSelWndDlg ダイアログ

#include "icssel.h"
#include "icsdisp.h"


class CSelWndDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSelWndDlg)

public:
	CSelWndDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSelWndDlg();

	CICSSel	m_selfix8;
	CICSSel	m_selval32;

	CSelGuideCtrl m_selGuid;

	BOOL	m_bIgnoreActvate;

// ダイアログ データ
	enum { IDD = IDD_DLG_SELWND };

//	BOOL	Create();

	BOOL fix8cnd();
	void fix8cnd( BOOL sg );
	BOOL fixvalcnd();
	void fixvalcnd( BOOL sg );
	void fix8disp( BOOL sg );
	
	/* 固定選択画面の状態 */
	void enable_fix8( BOOL sg );
	
	void enable_fixval( BOOL sg );
	void set_valtitle( char *msg );
	void valtitle( BOOL sg );

protected:
	int		m_nID;
	CWnd*	m_pParent;

	BOOL	fix8sg;
	BOOL	fixvalsg;
	RECT	m_selfix8_rect;
	RECT	m_valtitle_rect;

	BOOL		flgPen;
	COLORREF	m_vwColor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	DECLARE_EVENTSINK_MAP()
	void SelectedIcsselctrl1(short position, long Number);
	void SelectedIcsselctrl2(short position, long Number);

	LRESULT OnSelGuideMsg( WPARAM wp, LPARAM lp );
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
