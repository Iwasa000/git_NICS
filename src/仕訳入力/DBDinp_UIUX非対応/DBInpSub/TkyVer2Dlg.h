#pragma once

#include "icsdbedt.h"

// CTkyVer2Dlg ダイアログ

class CTkyVer2Dlg : public ICSDialog
{
	DECLARE_DYNAMIC(CTkyVer2Dlg)

public:
	CTkyVer2Dlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTkyVer2Dlg();

	CICSDBEDT	m_DBEkana;
	CICSDBEDT	m_DZbrn;
	CICSDBEDT	m_CZbrn;
	CICSDBEDT	m_DBEinv;

	void	InitDlg( CDBZmSub* pZm, CDBSyohi* pSy );
	BOOL	IsDataBaseUpdate() { return m_bDBupdate; };

	BOOL	IsRegJob() { return m_bReg; };
	void	GetRegEdaban( int& dbr, int& cbr );

	void	GetTkyData( TKYV2_SAVE* psv );
	void	SaveTkyData( TKYV2_SAVE* psv );
	void	SetSaveTkyData( TKYV2_SAVE* psv );

// ダイアログ データ
	enum { IDD = IDD_TKYV2_DLG };

	char	m_kanabf[24];
	BOOL	m_bEdaReg;

// インプリメンテーション
protected:
	BOOL	m_bDBupdate;
	BOOL	m_bReg;

	CDBZmSub* m_pZm;
	CDBSyohi* m_pSy;	// SyOpen() 呼び出し済み

	DBKNREC* m_pDeb, *m_pCre;

	BOOL	m_bEdaMast;
	BOOL	m_bEZquery;
	int		m_debBrn, m_creBrn;

	char	m_debZan[6], m_creZan[6];
	CString	m_svTky, m_svKana, m_svSelTky;

	BOOL		m_SvMode;
	TKYV2_SAVE	m_Save;

	int		m_addTKcod;
	int		m_TKYO_LEN;

	char	m_TkyInvno[16];
	BYTE	m_Menchk;

private:
	// インボイス対応
	bool m_bInvMaster;	// インボイスバージョンアップマスター
	bool m_bInvUse;		// インボイス期間か	
	bool m_bInvDisp;	// インボイス登録番号の表示

	CSyzInvoice syzInvoice;
	BOOL m_bIgnoreFocus;
	CString	m_ErrMsg;
	CComboBox	m_ComboMen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	void	CheckJob( WPARAM vkey );
	int		CheckTerminationID( int id, long nChar );
	int		TerminationJob( int nID, int vect );

	int		AddKmkTekiyoFunc( int tkcod, char* teki, int btnchk );
	BOOL	DoubleRegChk( int tkcod, int rpgno );

	int		AddSiwakeTekiyoFunc( int tkcod, char* teki, int btnchk );
	int		SwkTkyoAddSub( SWKCOD_DATA* psc, int code );
	BOOL	SwkTkyo_DoubleRegChk( int code, SWKCOD_DATA* pRec);

	int		AddKmkEdabanFunc( int tkcod, char* teki, int btnchk );
	int		EdabanDoubleRegChk( int type, int tkcod, int& brn, DBKNREC* pKn, char* teki );
	int		eda_seteicheck( int type, int& tkcod );
	int		EdaNameInpSearch( LPCTSTR kcod, char* teki );

	void	EdaCtrlWork(int sign = 0);
	void	SetCheckFromSave();
	void	InvCtrlWork();
	int		SameTekiyoCheck(char* teki, char* kanabf, char* invno, BYTE menchk, int& tklast, int type);
	long	ComboBoxJob(UINT id, WPARAM wp);
	int		ButttonClickByMouseEvent(UINT id);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedBtnReg();
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdbedt8Tky(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8Kana(long nChar, long inplen, long kst);

	void set_focus( int id );
	LRESULT OnRetroFocus(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedChkTkyonly();
	void TerminationIcsdbedt8Czbrn(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8Dzbrn(long nChar, long inplen, long kst);
	afx_msg void OnBnClickedBrncreBtn();
	afx_msg void OnBnClickedBrndebBtn();
	afx_msg void OnBnClickedBtnTkysel();

	afx_msg void TerminationIcsdbedt8Invno(long nChar, long inplen, long kst);
	afx_msg void OnKillFocusIcsdbedt8Invno(long inplen);

	int GetInvnoString(char* pbuf, int szpbuf);
	BOOL CheckInvoiceNo();
	LRESULT OnMyMessage(WPARAM wp, LPARAM lp);
};
