// RDSel.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRDSel ダイアログ

//#include "ICSDialog.h"
#include "icscombobox.h"

#include "SELRNG.h"
//{{AFX_INCLUDES()
#include "icsdbedt.h"
//}}AFX_INCLUDES
#include "rdtatbl.h"
#include "OwnerButton.h"
#include "DataListCtrl.h"
#include "ColoredComboDD.h"


#define RDMODE_HANI		0
#define RDMODE_KOBETSU	1


class CRDSel : public ICSDialog
{
// コンストラクション
public:
	CRDSel(CRect rect, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRDSel)
	enum { IDD = IDD_RDSEL };
	CColoredComboDD	m_RDLBL;
	CButton	m_Cancel;
	CButton			m_Btn1;
	COwnerButton	m_Btn2;
	//}}AFX_DATA
	CDataListCtrl	m_List;
	CButton		m_Zritu;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CRDSel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	struct _RDTATBL*	m_pTBL;
	int	m_CmnId;
	int m_LinNum;
	int	m_Type;		//0:通常形式, 1:出納帳形式
	int	m_RDdate, m_RDmofs;

private:
	int m_NowIDC;
	int	m_TopIndex;
	int m_cbIdx;
	int m_CurIndex;		// カレント表示行
	int m_CurDataIdx;	// カレントデータインデックス
	CRect	m_DispRect;

	BOOL m_bDIALOG_OK;
	int		m_Kbsgn[200];	// 個別指定サイン

	BOOL	m_bShowFlag;
	CWnd*	m_pParent;

	CArray<CDBINPDataRec, CDBINPDataRec&> m_ACdata;
	CDWordArray							m_ACpos;

// インプリメンテーション
protected:
	void SetListColumn( int line );
	void ListDataSet( int seq );
	void dbdata_set( BOOL syinit );
	int mkrd_data_requery( int num );

	void LineCursorOFF();
	BOOL SetLineCursor( short index );
	COLORREF GetRdLineBackColor( int index );

	BOOL is_SHIFT();
	BOOL is_CTRL();
	int LinpViewLine();
	BOOL FindPageDisp( int num );
	void FocusFncs( UINT IDC, int FocusMode );
	int ActionFncs( UINT IDC, int actionmode, long nChar=NULL, int val=NULL, VOID *pval=NULL );
	int  AllSelJob();

	BOOL Kbt_SetLineCursor( short index, int shift_sel =0);
	int Kbt_ActionFncs( UINT IDC, int actionmode, long nChar=NULL, int val=NULL, VOID *pval=NULL );
	void Kbn_Rngdsp();
	BOOL Kbn_DataCheck();
	int	 Kbn_GetIndex( int cnt );
	void TitleBarTextChange();

	void SendData( void );

	void OnCancel();
	void OnOK();

	LPCTSTR rd_syline_txt();

	void OnSetDispInfo( NMHDR* pNMHDR, LRESULT* pResult);
	void OnLineChanged( NMHDR* pNMHDR, LRESULT* pResult);
	void OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult);

	void ACbookListColumn();
	void ACBK_ListDataSet( int seq );
	int  RecordDataCount();

	void StaticWndMove();

	void sel_shiftjob(int curIndex, long nChar, BOOL bPress);
	void click_seljob(int line, long nChar);
	void sel_lineupdate();
	void selection_iniz();

	int	 m_SelIndex;
	BOOL m_SelUpdate;
	int  m_SelDirection, m_SelType, m_SelSign, m_SelSign2;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRDSel)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnBnClickedChkZeiritsu(); 
	afx_msg void OnButtonAll();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnMyViewJob( WPARAM wParam, LPARAM lParam );
	LRESULT OnSelChgMsg(WPARAM wp, LPARAM lp);
public:
};
