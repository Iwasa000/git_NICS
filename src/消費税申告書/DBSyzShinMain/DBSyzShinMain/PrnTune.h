#pragma once
#include "icsdbedt8ctrl1.h"
#include "afxcmn.h"
#include "afxwin.h"


// CPrnTune ダイアログ

class CPrnTune : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnTune)

public:
	CPrnTune(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnTune();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_TUNE };

private:
	int				CbCt;
	int				AnCt;
	int				ItCt;
	int				XVP;
	int				XHP;
	int				XDVP;
	int				XDHP;
	int				XELA;
	int				Answer;

	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub		*m_pZmSub;			// 公益クラス
	SYC_SHININFO	*m_pShinInfo;		// 共通情報
	CSyzUtil		m_Util;				// ユーティリティクラス

	SH_PRINT_CMINFO	*m_pPrintCmInfo;	// 印刷共通情報
	int				m_swBeForked;

public:
	// 初期情報セット
	int InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked );
	// KSK用紙設定が必要？ ('15.11.24)
	bool IsNeedKSK( int sel );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdbedt8ctrl1 m_DBEdit1;
	CIcsdbedt8ctrl1 m_DBEdit2;
	CIcsdbedt8ctrl1 m_DBEdit3;
	CIcsdbedt8ctrl1 m_DBEdit4;
	CIcsdbedt8ctrl1 m_DBEdit5;
	CSpinButtonCtrl m_Spin1;
	CSpinButtonCtrl m_Spin2;
	CSpinButtonCtrl m_Spin3;
	CSpinButtonCtrl m_Spin4;
	CSpinButtonCtrl m_Spin5;
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8ctrl2(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8ctrl3(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8ctrl4(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8ctrl5(long nChar, long inplen, long kst);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedCancel();
public:

//	static	CDBSyzShinView *pSyzShin;

	void MovePrintGetData( void );
	void SetInitPrnTune( void );

	CComboBox m_Combo1;
	afx_msg void OnCbnSelchangeCombo1();

	CStatic m_STATIC_OCR;
	CStatic m_STATIC_IMG;
	afx_msg void OnBnClickedButton2();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
