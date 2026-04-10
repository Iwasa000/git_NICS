//{{AFX_INCLUDES()
#include "icsdbedt.h"
#include "icsdateselect.h"
//}}AFX_INCLUDES
#ifndef _AUTOINP_H_
#define _AUTOINP_H_
// AutoInp.h : ヘッダー ファイル
//


/////////////////////////////////////////////////////////////////////////////


class CAutoInp : public ICSDialog
{
// コンストラクション
public:
	CAutoInp(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CAutoInp)
	enum { IDD = IDD_AUTOINP };
	CComboBox	m_DENP;

	int		m_KMPG;
	int		m_KMNO;
	int		m_BMNSEL;
	int		m_SWKTEK;
	int		m_SWTKYD;

	CButton	m_OK;
	CButton	m_Cancel;
	BOOL	m_BMON;
	BOOL	m_CRED;
	BOOL	m_DATE;
	BOOL	m_DEBT;
	BOOL	m_BRN;
	BOOL	m_BRTEK;
	BOOL	m_BRTOTKY;
	BOOL	m_SYGBRN;
	BOOL	m_SYIMP;
	BOOL	m_BRCPY;
	BOOL	m_BMNDSP;
	BOOL	m_KOJI;
	BOOL	m_SYOBUZ;
	BOOL	m_RDDATE;
	BOOL	m_RDBRN;
	BOOL	m_BMNCHK;
	CICSDBEDT	m_DBEdtKmk;
	BOOL	m_DCKMchk;
	BOOL	m_KanaDisp;
	BOOL	m_Hjiyu;
	BOOL	m_KojiTky;
//	BOOL	m_BMNBrn;
	// 検索一覧で科目コード表示
	BOOL	m_ScnCode;
	BOOL	m_ZeiChk;
	BOOL	m_DelSwk;
	BOOL	m_ScnClr;

	BOOL	m_KjiInp;
	BOOL	m_KanKji;
	BOOL	m_KjiKmk;
	BOOL	m_BRNKak;
	CComboBox	m_DEvi;
	BOOL	m_BMNtkbr;
	BOOL	m_BrnKana;
	BOOL	m_RTSWK;
	BOOL	m_BmnTky;
#ifdef CLOSE
	ICSDateSelect m_selDate;
#endif
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CAutoInp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// オペレーション
public:
	void OnInitial( int koji_sw, struct _AUTOSEL *sel, int no = 0, int nChar = 0, BOOL APPEND_MODE = TRUE );
	void get_data( struct _AUTOSEL *sel);


public:
	// 日付データ
	int	m_Sofs, m_Eofs, m_Sdate, m_Edate;

// インプリメンテーション
protected:
	int m_grpno;
	int alt_key;
	BOOL m_APPEND_MODE;
	struct _AUTOSEL m_Sel;
	int		m_DenpSel;
	int		m_DEviSel;

	void dsp_onoff();
	BOOL ok_grp( int grpno );
	int grp_back( int grpno );
	int grp_for( int grpno );
	int grp_left( int grpno );
	int grp_right( int grpno );
	int grp_up( int grpno );
	int grp_down( int grpno );

	void goto_grp( int grpno );
	int  goto_grpcheck( int grpno, int nChar );

	int get_grpno();
	short F2F3( short nChar );
	short OnTerm( short nChar );

	// 金額チェックボックスコントロール
	void ValCheckCtrl( UINT ID );
	void TkyCheckCtrl( UINT ID );
	void ZmoveCheckCtrl( UINT ID );
//	void kakudai(CWnd* wnd);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAutoInp)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnImeEndCompositionIcsdbedtBskmk(short nChar, LPCTSTR string, LPCTSTR ystring);
	afx_msg void OnKanaTerminationIcsdbedtBskmk(LPCTSTR Kana);
	afx_msg void OnTerminationIcsdbedtBskmk(long nChar, long inplen, long kst);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChkValcopy();
	afx_msg void OnBnClickedChkValnot();
	afx_msg void OnBnClickedChkTkytype1();
	afx_msg void OnBnClickedChkTkytype2();
	afx_msg void OnBnClickedChkTkytype3();
	afx_msg void OnBnClickedChkZmvdate();
	afx_msg void OnBnClickedChkZmvmnth();
	afx_msg void OnBnClickedChkKmkana();

	LRESULT OnRetroFocus(WPARAM wp, LPARAM lp);
	CString m_imeStr;
};


// グループ番号定義
//	 この順番が、Enter キーの移動順になる

#define GR_DATE		1
#define GR_BMON		2
#define GR_KOJI		3
#define GR_DEBT		4
#define GR_CRED		5
//#define GR_BILL	
//#define GR_HUSEN	
#define GR_BMNCHK	6
#define GR_BMNDSP	7
#define GR_BMNTKY	8
#define GR_KJTKY	9
//#define GR_BMNBRN	8
#define GR_BMNSEL	10
#define GR_KJIINP	11
#define GR_KANKJI	12
#define GR_KJIKMK	13

#define GR_BRN			14
#define GR_BRTEK		15
#define GR_BRCPY		16
#define GR_BRTOTKY		17
#define GR_SYGBRN		18
//#define GR_BRNKAK		18
#define GR_BRNKANA		19
#define GR_BMNTKBR		20

#define GR_DENP			21
#define GR_SNO			22
#define GR_RDDATE		23
#define GR_RDBRN		24
#define GR_ZEIMDFY		25

#define GR_SYOBUZ		26
#define GR_HJIYU		27

#define GR_DCKMCOD		28
#define GR_DCKMCHK		29
#define GR_SCNCODE		30
#define GR_DELSWK		31
#define GR_SCNCLR		32

#define GR_KMPAGE		33
#define GR_KMCODE		34
#define GR_KANADISP		35

#define GR_SWKTEK		36
#define GR_SWTKYD		37
#define GR_RTSWK		38


#define GR_VALCOPY		39
#define GR_TKYREG		40
#define GR_ZMOVE		41

#define GR_TKTAB		42
#define GR_ENDKAKU		43
#define GR_DENPTYPE		44
#define GR_KANASEARCH	45
#define GR_SYZSTOP		46

//#define GR_SYIMPORT	18

#define GRP_MAX (GR_SYZSTOP)




#endif	// end of _AUTOINP_H_




