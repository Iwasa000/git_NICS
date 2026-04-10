// SWKselDlg.h : ヘッダー ファイル
//
/////////////////////////////////////////////////////////////////////////////
// CSWKselDlg ダイアログ

//#include "ICSDialog.h"
#include "SwkTkyDef.h"


class CSWKselDlg : public ICSDialog
{
// コンストラクション
public:
	CSWKselDlg(CWnd* pParent = NULL, BOOL bDCBoth = FALSE );   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSWKselDlg)
	enum { IDD = IDD_DLG_SWKSEL };
	int		m_Radio;
	CButton	m_OK;
	CButton	m_Cancel;
	//}}AFX_DATA

	void SetZmClass(CDBZmSub* pZm);

	SWK_REGSEL	m_Selected;
	static BOOL bSwkDlgShow;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSWKselDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CDBZmSub*	m_pZm;
	SWK_REGSEL	m_Rgsel[8];

	BOOL m_bDCBoth;

	void		SetRadioString(int cnt);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSWKselDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
