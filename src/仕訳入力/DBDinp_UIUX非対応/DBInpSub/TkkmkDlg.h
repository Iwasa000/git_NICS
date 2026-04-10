// TkkmkDlg.h : ヘッダー ファイル
//
//{{AFX_INCLUDES()
#include "icsdbedt.h"
//}}AFX_INCLUDES
#ifndef INCLUDE_TKKMKDLG_H
#define INCLUDE_TKKMKDLG_H

/////////////////////////////////////////////////////////////////////////////
// CTkkmkDlg ダイアログ
#include "icslistinp.h"

#include "BaseDlg.h"
#include "ICSListBox.h"
#include "afxwin.h"


class CTkkmkDlg : public CBaseDlg
{
// コンストラクション
public:
	CTkkmkDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTkkmkDlg)
	enum { IDD = IDD_DLG_RENTK };
	ICSListBox	m_RenSubList;
	CSpinButtonCtrl	m_PageCtrl;
	CICSListInp	m_listtkren;
	CICSDBEDT	m_DBedtRenpg;

	CButton m_BTchg;

	//}}AFX_DATA

	virtual void WndShow(BOOL bShow);
	virtual void WndShow_Add(BOOL bShow);
	virtual void FocusMove( int direction );
	virtual void SetFirstPosition(void);
	virtual void InitWork( double x, double y );
	virtual BOOL IsEnableToolBar( int nID );
	virtual void AddTekiyoJob( LPARAM lParam );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTkkmkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL m_bFocusSet;
	BOOL m_bNOkmk;
	BOOL m_bCHGmode;

	CString	m_CHGcode;
	BOOL	m_bListTermOK;

	int	m_RenDispPage;
	int m_PosMarkIndex;
	int m_InsIndex;
	int m_rnMax;
	int m_rnCnt;
	int	m_KanaIndex;	// 07.22 /08

	int	m_TKYO_LEN;

	void MakeTkrenFrm();
	void MoveTitleCtrl();
	void SetRensoList( int pageno, BOOL bRequery = TRUE );
	void Tklist_Clear();
	int	 KamokuRenPage( int page );

	BOOL DoubleRegChk( int tkcod, int rpgno );
	void SetTkrenRecord( int index );
	void SetTkrenListData( int index, int code, CString Kname );
	void DeleteRtrec( int index );
	void TorokDisp();
	void AddEmptyData();

	void ChangeRensoList( int dc_sw );

	int AddTekiyoFunc( char* kmkcod, int tkcod, int dc_sw );
	long TermEditRenPage(long nChar, long inplen, long kst);
	void TkrenListPosMark( int mode, int set_idx = -1 );

	BOOL RTRECcount();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTkkmkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioDebt();
	afx_msg void OnRadioCred();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditONIcslistinpctrl1(short index, short column);
	afx_msg void OnKeyInIcslistinpctrl1(short nChar, short previndex, short index);
	afx_msg void OnTerminationIcslistinpctrl1(short nChar, short len, short index, short clm, LPUNKNOWN data);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnUser1_Msg( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnListInsDel( WPARAM wParam, LPARAM lParam );

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedButtonChg();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void KillFocusIcsdbedtpage(long inplen);
	void TerminationIcsdbedtpage(long nChar, long inplen, long kst);
public:
	CEdit m_EditPg;
public:
	afx_msg void OnEnKillfocusEdit();
};



#endif // INCLUDE_TKKMKDLG_H
