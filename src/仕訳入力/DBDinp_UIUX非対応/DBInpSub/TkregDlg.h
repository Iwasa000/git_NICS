// TkregDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTkregDlg ダイアログ

#include "ICSTbarDlg.h"
#include "ExtTab.h"
#include "TkdicDlg.h"
#include "TkkmkDlg.h"
#include "TkswkDlg.h"
#include "icsppst8.h"


typedef CTypedPtrArray<CObArray, CBaseDlg*> CBaseDlgArray;


class CTkregDlg : public CICSTbarDlg
{
// コンストラクション
public:
	CTkregDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTkregDlg)
	enum { IDD = IDD_TEKIYO_DLG };
	//}}AFX_DATA

	void InitDlg( CDBZmSub* pZm, CDBSyohi* m_pSy );
	BOOL IsDataBaseUpdate() { return m_bUpdate; };

	BOOL& IsKaniJob() { return m_bKani; };
	void  SaveTkyData(TKYV2_SAVE* psv, BOOL bClear = FALSE);

	char m_kanabf[10];
	CString	m_svTky;	//簡易で設定した摘要
	char m_svInvno[16];
	BYTE m_svMenchk;
	TKYV2_SAVE	m_Save;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTkregDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CTkdicDlg*		m_pDicdlg;
	CTkkmkDlg*		m_pKmkdlg;
	CTkswkDlg*		m_pSwkdlg;

	CBaseDlgArray	m_TabArray;
	int		m_NOWIDX;
	int		m_swInit;
	BOOL	m_bFirst;
	BOOL	m_bUpdate;
	BOOL	m_bKani;

	CDBZmSub* m_pZm;
	CDBSyohi* m_pSy;	// SyOpen() 呼び出し済み

	void	TabChg( int index, BOOL bAdd = FALSE );
	void	TabDlgMove();
	void	GetDBupdateSgn();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTkregDlg)
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnKickIdle( WPARAM wParam, LPARAM lParam );
	LRESULT OnTekiAddJob(WPARAM wParam, LPARAM lParam);

	void OnCommandTlbar( UINT id );
	LRESULT OnMyTabChange(WPARAM wParam, LPARAM lParam);

	void strec_searchjob();

#ifdef CLOSE
	LRESULT OnIdleUpdateCmdUI( WPARAM wParam, LPARAM lParam );
	void OnUpdateCommandTlbar( CCmdUI* pCmdUI );
#endif
public:
	ICSTabCtrl	m_Tab;
//	CExtTab		m_Tab;

public:
	afx_msg void OnBnClickedBtnF8();
	afx_msg void OnBnClickedBtnF9();
	afx_msg void OnBnClickedBtnEnd();
	afx_msg void OnBnClickedBtnF3();
	int ButttonClickByMouseEvent(UINT id);

protected:
	virtual void OnCancel();
	
};
