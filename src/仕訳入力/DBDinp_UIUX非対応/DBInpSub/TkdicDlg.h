// TkdicDlg.h : ヘッダー ファイル
//
//{{AFX_INCLUDES()
#include "icslistinp.h"
#include "icsdbedt.h"
//}}AFX_INCLUDES
#ifndef INCLUDE_TKDICDLG_H
#define INCLUDE_TKDICDLG_H

/////////////////////////////////////////////////////////////////////////////
// CTkdicDlg ダイアログ

#include "BaseDlg.h"
#include "TkEdit.h"
#include "SELDLG.h"
#include "SwkTkyDef.h"
#include <SyzInvoice.h>

// CTkdicDlg ダイアログ
#define		TK_CODE		0
#define		TK_NAME		1
#define		TK_KANA		2
#define		TK_KUBUN	3
#define		TK_SYOHI	4
#define		TK_INVNO	5
#define		TK_MENZEI	6

#define		BIT_MENZEI	0x10

class CTkdicDlg : public CBaseDlg
{
// コンストラクション
public:
	CTkdicDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CTkdicDlg();//　デストラクタ追加

// ダイアログ データ
	//{{AFX_DATA(CTkdicDlg)
	enum { IDD = IDD_DLG_TKDIC };
	CTkEdit		m_Edit;
	CICSListInp	m_listtkdic;
	CButton	m_EBLeft;
	CButton	m_EBRight;
	CButton	m_EBAdd;
	CButton	m_EBSub;
	CButton m_EBHome;
	CButton m_RBtorok;
	CButton m_RBdebkmk;
	CButton m_RBcrekmk;
	CButton m_RBdebcre;
	CButton m_RBswk;
	
	CICSDBEDT	m_DBEkana;
	CICSDBEDT	m_DBEnum;
	CICSDBEDT	m_DBEinv;
	//}}AFX_DATA

	virtual void WndShow(BOOL bShow);
	virtual void FocusMove( int direction );
	virtual void SetFirstPosition();

	virtual void InitWork( double x, double y );
	virtual BOOL IsEnableToolBar( int nID );
	virtual void LastDataGet();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTkdicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	char	m_Kana[10];
	CString m_svTky;
	char	m_TkyInvno[16];
	BYTE	m_Menchk;

	int		insNewLine;
	int		addNewLine;

	int edit_keydown_sw;

	int post_row;
	int post_col;

	int m_iniz;

// インプリメンテーション
protected:
	BOOL	m_bInitOK, m_bListTermOK;
	int		m_NowIdx, m_NowClm, m_InputSign;
	int		m_Tkno, m_TkMax, m_TkNumber;
	int		m_InsIndex;

	CBrush	m_White;
	DIS_TKREC	m_Ctkrec;
	SELDLG*	m_SelDlgWnd;
	TK_ADD	m_TkAddPar;
	CZIM_TkjourUtil*	pTkjour;

	int		m_TKYO_LEN;
	int		m_SyzType;
	BOOL	m_bSyzInp;

	void MakeTKdicFrm();
	void TorokDisp();
	void SetItemListTkyo(int tkcod = 0, int clm = TK_CODE );
	void SetItemStateTkyo(int tkcod = 0, int clm = TK_CODE);
	void MakeTkyoLine( CTKREC * tkrec );
	void MakeTkyoLine(int entry_cnt);
	void IcsLinpTermination(short nChar, short len, short index, short clm, LPUNKNOWN data);

	void ListInsertClear();

	BOOL IsUsedTKcode( int tkcode );

	void dline_chg( int index );
	BOOL CheckTKrecData( short index, short colm, LPUNKNOWN data );
	void Update_Tkrec( int index );
	int Update_TkrecEx( CTKREC *pTkrec );
	void TkyoNewlineData();
	void DeleteTkrec( int tkcode );
	void Current_Recset( int index );
	int SrchTKname( char* teki, int* tkno, BOOL bOther );

	int AddTkrec_OtherReg();

	void TknoInp( int id );
	void set_focus( int id );

	void get_kanadata();
	int	 KamokuOwnTblCheck();

	//-------------->摘要全削除対応2014.11.20
	char	Tk_UseItem;	//摘要コードをどれで使用しているかをビットにて判定
						//D0ビットON・・・科目対応摘要で使用中
						//D1ビットON・・・仕訳対応摘要で使用中
						//D2ビットON・・・科目枝番で使用中
						//D3ビットON・・・収支科目枝番で使用中
						//D4ビットON・・・部門枝番で使用中
						//D5ビットON・・・収支部門枝番で使用中
	
	int Change_AllTKInfo(int OldCode, int NewCode, int mode);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTkdicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHomebutton();
	afx_msg void OnLeftbutton();
	afx_msg void OnRightbutton();
	afx_msg void OnSubbutton();
	afx_msg void OnAddbutton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonTorok();
	afx_msg void OnEditONIcslistinpctrl1(short index, short column);
	afx_msg void OnTerminationIcslistinpctrl1(short nChar, short len, short index, short clm, LPUNKNOWN data);
	afx_msg void OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
	afx_msg void OnTerminationIcsdbedtNum(long nChar, long inplen, long kst);
	afx_msg void OnKillFocusIcsdbedtctrl1(long inplen);
	afx_msg void OnKillFocusIcsdbedtNum(long inplen);
	afx_msg void OnEditOFFIcslistinpctrl1(short nChar);
	afx_msg void OnFocusedIcslistinpctrl1();
	afx_msg void OnKeyInIcslistinpctrl1(short nChar, short previndex, short index);
	afx_msg void OnButtonDebkmk();
	afx_msg void OnButtonCrekmk();
	afx_msg void OnButtonDebcre();
	afx_msg void OnButtonSwk();
	afx_msg void OnButtonFuri();

	afx_msg void OnBnIns();
	afx_msg void OnBnDel();

	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);

	//afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);

	//afx_msg void OnTerminationIcslinpTkdic(short nChar, short len, short index, short clm, LPUNKNOWN data);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnTKDICMessage( WPARAM wparam, LPARAM lparam );
	LRESULT OnRetroFocus(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser_1(WPARAM wParam, LPARAM lParam);

	LRESULT FontSize(WPARAM wParam, LPARAM lParam);

	// インボイス対応
	// 免税事業者からの課税仕入れ
	LRESULT OnMenzeiMessage( WPARAM wParam, LPARAM lParam );
	// 仕入/売上区分
	LRESULT OnKubunMessage( WPARAM wParam, LPARAM lParam );

	int  DeleteTklnkjour(int tkcod);


public:
	void ReadStringIcslistinpctrl1(long index, LPCTSTR string);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	ICSReportControl m_report_tkdic;

	typedef struct	_CTKDATA_REC {
		//	int			st;
		long		tkcod;
		CString		tkana;
		CString		tksgn;
		CString		tkname;
		CString		tkari;		// 未使用
		CString		tkasi;		// 未使用
		CString		bktkwd;		// 銀行csv摘要語句
		long		karicod;
		long		kasicod;
		//char        tel[15];//('15.12.08) 電話番号
		CString		tel;
		CString		multi;	// 摘要対応
		// インボイス対応
		CString		invno;		// 登録番号
		CString		taxfree;	// 免税事業者からの課税仕入れ
		CString		dsign;		// 仕入・売上区分
	} CTKDATA_REC;

	map<long, CTKDATA_REC> m_TbCust;

	void OnNMReportEditsetfocusREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result);
	void SetReportRecord(ICSReportRecord* pRecord);
	//void OnTerminationIcslinpTkdic(short nChar, short len, short index, short clm, LPUNKNOWN data);
	

	//CZIM_TkjourUtil*	pTkjour;
	void OnNMReportEditkeydownREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result);

	void map_struct_clear(CTKDATA_REC* tkdata_rec);

	CString		ErrMsg;

	int CheckUseTkcod(int tkcode);

	void SetMapColor();

	void SetNexControl(int s_row, int s_clm, BOOL s_edit);

	void SetPrevControl(int s_row, int s_clm, BOOL s_edit);

	int		PosCngChk;//１・・ポジション変更　０・・同ポジション
	//ICS_Report_Control m_report_tkdic;
	//void OnNMReportEditsetfocusREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditimeendREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkillfocusREPORT_TKDIC(NMHDR * pNotifyStruct, LRESULT * result);

	int GetTableOfset(int tkcod);
	int idx = 0;

	void SaveTkyData(TKYV2_SAVE* psv, BOOL bClear = FALSE);
	BOOL CheckInvoiceNo();

private:
	// インボイス対応
	bool m_bInvMaster;	// インボイスバージョンアップマスター
	bool m_bInvUse;		// インボイス期間か	
	bool m_bInvDisp;	// インボイス登録番号の表示

	CSyzInvoice syzInvoice;

	int m_syzKubn;

	BOOL		m_bIgnoreFocus;
	CString		m_ErrMsg;
	CComboBox	m_ComboMen;
	
	long ComboBoxJob(UINT id, WPARAM wp);

private:
	// map更新
	void SetCTKDATA_REC( CTKREC *tkrec, int SetNo );
	// レポートコントロールパラメータ設定
	void SetReportPara();
	// tkrec更新
	int UpdateTkrecEx( CTKREC *pTkrec );
	// スペースカット
	void kspTrim( CString &OrgStr );

	// 課税方式のチェック
	int CheckSyzKubun( CZVOLUME *pZvol );

	// 仕入/売上区分の表示
	int SkbnMsg( CString& Kbnbf, unsigned char TKszk, int type = 0, int flg = 0 );

	int		is5perm();
	int		is8perm();
	int		is10perm();

	int  TerminationJob(int nID, int vect);
	void TerminationIcsdbedt8Invno(long nChar, long inplen, long kst);
	void OnKillFocusIcsdbedt8Invno(long inplen);

	int GetInvnoString(char* pbuf, int szpbuf);
	LRESULT OnInvnoMessage(WPARAM wp, LPARAM lp);

};



#endif // INCLUDE_TKDICDLG_H
