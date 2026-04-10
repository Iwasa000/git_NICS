#pragma once

class CRefundBaseDialog : public ICSDialog
{
	DECLARE_DYNAMIC(CRefundBaseDialog)
	CMap<int,int,CDiagControlBase*,CDiagControlBase*> DiagControlMap;
	CArith arith;

	int CurrentDiagID;
public:
	
	CRefundBaseDialog(CWnd* pParent = NULL){ TRACE("0"); }   // 標準コンストラクタ
	CRefundBaseDialog(UINT id, CWnd* pParent = NULL);
	virtual ~CRefundBaseDialog();

//	static SN_HEAD*	 snhead;
	static CSnHeadData *pSnHeadData;
	static CH26SnRefundData *pSnRefundData;
	static REFUNDHED	refundhead;
	static REFUNDVOL	refundvol;
	static CSyzUtil		*pUtil;		// '15.02.03
	static CDBNpSub		*pZmSub;	// '15.02.03
	static int			m_JcContinuePrint;	// '19.05.14
	
	static REFUNDREC* KAZEIU_REC;
	static REFUNDREC* YUNYUT_REC;
	static REFUNDREC* SHIIRE_REC;
	static REFUNDREC* TANAOR_REC;
	static REFUNDREC* KOTEIS_REC;

	void SetInitPosition(int sw);
	void SetDispPosition(int sw);
	static int MixSpaceCutLength_EX( char *buf, int lng );
	static CString GetMoneyStr(char* val,int zerodisp = 0);
	static CString GetMoneyStr_EX(char* val,int zerodisp = 0 );//shimizu
	static void HojinPrint(CDC* pDC,int pno);
	static void KojinPrint(CDC* pDC,int pno);
	static void VolRecClear();
	static void CRefundBaseDialog::VolBankInit();

	static void SetYsMeisaiRecBuf(REFUNDREC* refunrec_data,int line);
	static void SetTorihikiRecBuf(REFUNDREC* refunrec_data, int line,int check,int sgn);
	static void SetShiireRecBuf(REFUNDREC* refunrec_data, int line,int kei[] , int kojin);
	static void SetTitleBuf();
	static void SetTokusyuBuf();
	static void SetRiyuuBuf();
	static void SetYusyutsuBuf();
//	void MixSpaceCutLength( char *buf, int lng );
//	static CDBSyzShinView *pSyzShin;
	void SetTitle();

	// 他表で変更があったので再集計	('15.02.03)
	int calq_by_OtherChange( int sw, CH26SnFhyo20Data *pH26SnFhyo20Data, EnumIdIcsShRevType shRevType );

	static	bool IsJcContinuePrint();

protected:

	CWnd				*m_pParent;					// 親ウィンドウ

	int			m_svscll, m_pagesiz, m_sclmax, bottom_index;
	int			m_wPos;
	int			m_cmPos;
	int			m_ScW;
	
//	CSyzUtil	m_Util;
	CShinCom	m_ShinCom;		// 消費税申告書 共通情報

	CIcsdiagctrl* bottom_diag;

	CBrush brush;
	CBrush brush2;

//2016.02.23 INSERT START
//	CBrush		m_hBrushLock;
	CFont		m_font;
	ICSStatic m_InputLock;
//2016.02.23 INSERT END

	CIcsdiagctrl* GetCurrentDiag();
	CDiagControlBase* GetDiagControl(UINT CtlID);

	void SetDiagControlMap(UINT CtlID,CDiagControlBase* diagctl, CSyzDataSet* syzdata = 0);
		
	int  Termination(UINT CtlID,int index,char nChar);

	void FIpItemScrolldw( int mpos );
	void SetActivePosition();

	void SetScroll(UINT CtlID, short index);
	int  ComboBoxTerm(char nChar);

//2016.02.23 INSERT START
	int Make_StaticFont();
	void Static_Update( unsigned char Sn_Sign4 ); //CSnHeadDataのSn_Sign4
	void Line_Delete( int line_idx, int line_max, int control_id );
	void Line_Insert( int line_idx, int line_max, int control_id );
//2016.02.23 INSERT END


#ifdef _DEBUG
	void VolRecFill();
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnEditONIcsdiagctrl(UINT CtlID, short index);
	afx_msg void OnEditOFFIcsdiagctrl(UINT CtlID,short index);
	afx_msg void OnCheckButtonIcsdiagctrl(UINT CtlID, short index, LPUNKNOWN data);
	afx_msg void OnTerminationIcsdiagctrl(UINT CtlID, short index, short nChar, short length, LPUNKNOWN data);
	DECLARE_EVENTSINK_MAP()
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
