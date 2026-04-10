// DispOptionDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDispOptionDlg ダイアログ


#include "blockdragwnd.h"

class CDispOptionDlg : public ICSDialog
{
// コンストラクション
public:
	CDispOptionDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDispOptionDlg)
	enum { IDD = IDD_DSPOPT_DLG };
	int		m_Sw1;
	int		m_BmnYes;
	int		m_DenpYes;
	int		m_KojiYes;
	int		m_SyoYes;
	int		m_TgdtYes;
	int		m_LnHt1;
	CButton	m_OK;
	CButton	m_Cancel;
	//}}AFX_DATA

	BOOL	m_bKmast;

// ブロック単位 画面変更
	void	SetBlockInfo( BlockInfo* pBlk );
	int		GetBlockIdx( int* pBlkIdx );

	BOOL	GetSelDispShow();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDispOptionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void SetDoptControlState( int line );


	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDispOptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioSw1();
	afx_msg void OnRadioSw2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_Hjiyu;
	int m_Zei;
	int m_Syz;
	CBlockDragWnd	m_Blwnd;
	
	BOOL			m_bShow;
	BOOL			m_bEnaMove;

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedSeldspChk();
	afx_msg void OnBnClickedBtnInit();
};
