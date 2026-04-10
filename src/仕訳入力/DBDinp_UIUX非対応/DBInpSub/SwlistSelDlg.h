// SwlistSelDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSwlistSelDlg ダイアログ
//{{AFX_INCLUDES()
#include "icslistinp.h"
//}}AFX_INCLUDES

//#include "ICSDialog.h"


class CSwlistSelDlg : public ICSDialog
{
// コンストラクション
public:
	CSwlistSelDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSwlistSelDlg)
	enum { IDD = IDD_DLG_SWLISTSEL };
	CICSListInp	m_listswk;
	CButton	m_Ok;
	CButton	m_Cancel;	
	//}}AFX_DATA

	void SetZmClass(CDBZmSub* pZm, CSwcdArray* pSwary );
	void MakeSiwakeFrm();
	int  SiwakeInpSet();

	int	m_SelIndex;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSwlistSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CDBZmSub* m_pZm;
	CSwcdArray* m_pSwk;
	BOOL	  m_bInit;

	LRESULT afx_msg OnUser1_Msg(WPARAM wParam, LPARAM lParam);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSwlistSelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	afx_msg void OnKeyInIcslistinpctrl1(short nChar, short previndex, short index);
	afx_msg void OnLButtonDblClickIcslistinpctrl1(short index);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
