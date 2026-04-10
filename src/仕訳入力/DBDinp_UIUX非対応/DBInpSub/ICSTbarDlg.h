// ICSTbarDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CICSTbarDlg ダイアログ

//#include "ICSDialog.h"
#include "DlgToolBar.h"

class CICSTbarDlg : public ICSDialog
{
// コンストラクション
public:
	CICSTbarDlg(UINT dlgID, UINT barID, int border, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CICSTbarDlg)
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

	CToolBarCtrl& TBarCtrl() { return m_toolBar.GetToolBarCtrl(); };

	void GetResizeRate(double& x, double& y ) { x = m_Xrate; y = m_Yrate; };
	void SetResizeRate(double x, double y) { m_Xrate = x; m_Yrate = y; };
	
// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CICSTbarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	UINT	m_DlgID;
	UINT	m_toolBarID;
	int		m_Border;	// 0 -> 下境界なし, 1 -> 下境界あり
	CDlgToolBar	m_toolBar;
	CWnd*		m_pParent;
	
	double m_Xrate, m_Yrate;

	int	ToolBarCreate();
	
	virtual void SizeChange();
	
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CICSTbarDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * result );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
