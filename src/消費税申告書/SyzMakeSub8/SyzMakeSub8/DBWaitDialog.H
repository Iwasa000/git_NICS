// DBWaitDialog.h : ヘッダー ファイル
//


#ifndef __DBWAITDIALOG_H__
#define __DBWAITDIALOG_H__


/////////////////////////////////////////////////////////////////////////////
// CDBWaitDialog ダイアログ

class CDBWaitDialog : public ICSDialog
{
// コンストラクション
public:
	CDBWaitDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDBWaitDialog)
	enum { IDD = IDD_DIALOG_DBWAIT };
	CAnimateCtrl	m_ani;
	CStatic	m_static1;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDBWaitDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CWnd*	m_pParent;


// オペレーション
public:
	void SetWaitText(LPCTSTR pStr);


	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDBWaitDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


#endif			// end of __DBWAITDIALOG_H__


