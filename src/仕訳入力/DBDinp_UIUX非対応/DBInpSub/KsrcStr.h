// KsrcStr.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CKsrcStr ダイアログ
#include "afxpriv.h"

class CKsrcStr : public ICSDialog
{
// コンストラクション
public:
	CKsrcStr(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CKsrcStr)
	enum { IDD = IDD_KSRCSTR };
	CListBox	m_ListTkstr;
	//}}AFX_DATA

#ifdef OLD_CLOSE
	CDialogTemplate	CDtmp;
	int		DlgFontSize;
#endif

	// 類似摘要名称検索用
	CStringArray	strArray;
	int				Maxflg;
	BOOL			bOther;

	long	posx, posy;

	int		m_RecNo;

	// EndDialog() の返送値
	enum KEnd_Val 
	{
		EV_CANCEL = 10,
		EV_ADD,
		EV_REP,
		EV_OTHER
	};

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CKsrcStr)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKsrcStr)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRep();
	afx_msg void OnTorok();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
