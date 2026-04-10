#ifdef _CPYIMG	//#* 05.10 /01

// SelImgDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSelImgDlg ダイアログ

//#include "ICSDialog.h"

//{{AFX_INCLUDES()
#include "icslistinp.h"
#include "icsdisp.h"
//}}AFX_INCLUDES
#include "ICSComboBox.h"


#define SIMG_DATA_MAX	40
#define SIMG_DATA_NL	6
// リスト入力 
#define SIMG_SEQ		0
#define SIMG_TKY		1
// ダイアログ コントロール一
#define	SIMG_TKSEL		0
#define SIMG_LINP		1
#define SIMG_OK			2
#define SIMG_CANCEL		3
#define SIMG_NUM		4

// 摘要表示モード
#define SELTK_NORMAL_MODE	0
#define SELTK_IMG_MODE		1
#define SELTK_STR_MODE		2


#define _SELIMG_NUMINP_


typedef struct _SELIMG_LINE
{
	unsigned short		ZREC_NO;	//ZREC[ZREC_NO-1]
	LINP_DATA		_SIMGDTA[ SIMG_TKY + 1 ];
#define SIMG_SEQ_DATA	_SIMGDTA[ SIMG_SEQ ]
#define SIMG_TKY_DATA	_SIMGDTA[ SIMG_TKY ]
	
	int		ImgSeq;

	BOOL	bImage;		// イメージ摘要 かどうか

} SELIMG_LINE;



class CSelImgDlg : public ICSDialog
{
// コンストラクション
public:
	CSelImgDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSelImgDlg)
	enum { IDD = IDD_SELIMG_DLG };
	CComboBox	m_combo;
	CICSListInp	m_Linp;
	CICSDisp	m_DispImg;
	CButton	m_OK;
	CButton	m_Cancel;
	//}}AFX_DATA

	void Initial( CDipZmSub* pZm, CDBINPDataRec* pData, RECT pRect);
	// 選択されているイメージデータ
	int			m_CImgSeq;
	char		m_Tekiyo[100];
	BOOL		m_bSelTekImage;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSelImgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	SELIMG_LINE	m_SIMG_LINE[ SIMG_DATA_NL ];
	LINP_COLUMN	m_SIMG_LDEF[ SIMG_TKY + 1];	//項目定義
	CDBINPDataRec		m_IMGDATA[ SIMG_DATA_MAX ];

	CDipZmSub* m_pZm;
	CDBINPDataRec m_BaseData;
	RECT	m_RECT;

	WORD m_Strno, m_Edrno;
	int m_SIMG_LN;
	int m_PREV_IDX;
	int m_SIMG_POS;

	DWORD m_dwTekSelectMode;

	void set_nowpos(int pos);
	int  get_nowpos();

	void SetLinpDef();
	int img_read(int vect_n, int rno, CDBINPDataRec* pImgdata, int imgmax);
	void SetLinpData(SELIMG_LINE* pLN, CDBINPDataRec* pDt );
	void line_colordsp( int idx, BOOL sgn, BOOL initflg = FALSE );
	void focus_chk();
	int ButtonOkKeydown(MSG* pMsg);
	int ButtonCancelKeydown(MSG* pMsg);
	//	08.31 /04
	int ComboSelectKeydown(MSG* pMsg);
	int img_focus( int pn );

	int init_dsp(int seq_no=-1);
	int simg_dscroll_down( int scrol_nl );
	int simg_dscroll_up( int scrol_nl );
	void simg_scroll_down( SELIMG_LINE *ldta, int max, int ln );
	void simg_scroll_up( SELIMG_LINE *ldta, int max, int ln );
	void mk_data( SELIMG_LINE* ldta, CDBINPDataRec* data, int data_n, int nline );
	void set_imgtky( SELIMG_LINE* pLN );
	void mk_zerodata( SELIMG_LINE* pLN );
	void SetData( int idx, LPVOID data, BOOL sw );

	void disp_image( int idx );
	int _dline_chg( int idx, int focs_sw = 0);

	void disp_seq( int idx );

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelImgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyInIcslistinpctrl1(short nChar, short previndex, short index);
	afx_msg void OnLButtonDblClickIcslistinpctrl1(short index);
	afx_msg void OnLButtonClickIcslistinpctrl1(short index);
	afx_msg void OnFocusedIcslistinpctrl1();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelendokCombo1();
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

#ifdef _SELIMG_NUMINP_
	BOOL	m_bShow;
	int  search_tky( int seq );

	LRESULT OnRetroFocus( WPARAM w, LPARAM l );
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetFocusIcsdbedt8ctrl1();
	void TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst);
	void KillFocusIcsdbedt8ctrl1(long inplen);
//	afx_msg void OnFocusIcsnumedit();
//	afx_msg void OnTerminationIcsnumedit(short nChar);
#endif
};



#endif // end of _CPYIMG
