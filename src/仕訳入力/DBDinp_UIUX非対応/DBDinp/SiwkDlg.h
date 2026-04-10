// SiwkDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSiwkDlg ダイアログ
//#include "ICSDialog.h"

#include "DataListCtrl.h"


#define WM_STARTSIWAKE		(WM_APP+111)

#define MDFY_BEFORE		0
#define MDFY_AFTER		1


class CSiwkDlg : public ICSDialog
{
// コンストラクション
public:
	CSiwkDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSiwkDlg)
	enum { IDD = IDD_DLGSIWAKE };
	CButton	m_Ikat;
	CButton	m_Modify;
	CButton	m_NoMdfy;
	CButton	m_End;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA
	CDataListCtrl	m_DtMoto;
	CDataListCtrl	m_DtNew;

	BOOL&	IsCorrect() { return m_bCorrect; };

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSiwkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	BOOL	m_bSwkDel;

protected:
	CWnd*	m_pParent;
	int		m_ID;

	BOOL m_bShow;
	BOOL m_bData;	// ﾃﾞｰﾀの有り無し
	BOOL m_bRepeat;	// 繰り返しﾌﾗｸﾞ
	BOOL m_bCorrect;

	CDBINPDataRec	m_Data, m_MdfyData;

	void MakeDspData(int sgn, CDBINPDataRec* pdt );
	void SetBtnCondition(BOOL flg, int all);

	void NextDataSet( int nxt_sg, CMsgDlg* msgdlg );
	BOOL IsNewDataWrite( CDBINPDataRec *newdata, CDBINPDataRec *moto ); 

	void SetListColumn( int line );

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSiwkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnIkatu();
	afx_msg void OnBtnNomdfy();
	afx_msg void OnBtnEnd();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnStartSiwake(WPARAM wParam, LPARAM lParam);


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
