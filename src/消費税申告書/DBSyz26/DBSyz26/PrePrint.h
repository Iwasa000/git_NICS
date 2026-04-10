#pragma once
#include "icsdbedt8ctrl1.h"
#include "afxwin.h"
#include "icsdisp8ctrl1.h"


// CPrePrint ダイアログ

class CPrePrint : public ICSDialog
{
	DECLARE_DYNAMIC(CPrePrint)

public:
	CPrePrint(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrePrint();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PREPRINT };

public:
	char			ADRU1[40],			// 上段①　左
					ADRU2[40],			// 上段①　右
					ADRC1[40],			// 上段②　左
					ADRC2[40],			// 上段②　右
					ADRD1[60],			// 下段①　左
					ADRD2[20],			// 下段②　右
					ADRD3[20],
					ADKUP[20],
					ADKDW[20],
					ADKNO[10],
					ADRD4[50],			// 下段①　拡張版（漢字＆かな対応）
					ADSNO[10],			// 署別一覧番号
					ADDR[70],
					ADDRX[70],
					NUMBF[64];

	ICSColorInfo	ColorInfo;			// 色情報

public:
	// 初期情報のセット
	int InitInfo( CSnHeadData *pSnHeadData );

private:
	//== 帳票データ本体 ==
	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報
	CSyzUtil		m_Util;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	DECLARE_EVENTSINK_MAP()
public:
	void TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst);
public:
	void TerminationIcsdbedt8ctrl2(long nChar, long inplen, long kst);
public:
	void TerminationIcsdbedt8ctrl3(long nChar, long inplen, long kst);
public:
	void TerminationIcsdbedt8ctrl4(long nChar, long inplen, long kst);
public:
	void TerminationIcsdbedt8ctrl7(long nChar, long inplen, long kst);
//public:
//	afx_msg void OnBnClickedCheck1();
//public:
//	void TerminationIcsdbedt8ctrl8(long nChar, long inplen, long kst);
//public:
//	void TerminationIcsdbedt8ctrl9(long nChar, long inplen, long kst);
//public:
//	void TerminationIcsdbedt8ctrl10(long nChar, long inplen, long kst);

public:
//	static	CDBSyzShinView *pSyzShin;

public:
	CIcsdbedt8ctrl1 m_DbEdit1;
public:
	CIcsdbedt8ctrl1 m_DbEdit2;
public:
	CIcsdbedt8ctrl1 m_DbEdit3;
public:
	CIcsdbedt8ctrl1 m_DbEdit4;
public:
	CIcsdbedt8ctrl1 m_DbEdit7;
public:
	CButton m_Check1;
//public:
//	CIcsdbedt8ctrl1 m_DbEdit8;
//public:
//	CIcsdbedt8ctrl1 m_DbEdit9;
//public:
//	CIcsdbedt8ctrl1 m_DbEdit10;
public:
	void SetFocusIcsdbedt8ctrl1();
public:
	CStatic m_PrePrintNote;
public:
	void SetFocusIcsdbedt8ctrl2();
public:
	void SetFocusIcsdbedt8ctrl3();
public:
	void SetFocusIcsdbedt8ctrl4();
public:
	void SetFocusIcsdbedt8ctrl7();
public:
	afx_msg void OnBnSetfocusCheck1();
//public:
//	void SetFocusIcsdbedt8ctrl8();
//public:
//	void SetFocusIcsdbedt8ctrl9();
//public:
//	void SetFocusIcsdbedt8ctrl10();
public:
	afx_msg void OnBnSetfocusOk();
public:
	afx_msg void OnBnSetfocusCancel();
public:
	CIcsdisp8ctrl1 m_Disp1;
	CIcsdisp8ctrl1 m_Disp2;
	CIcsdisp8ctrl1 m_Disp3;
	CIcsdisp8ctrl1 m_Disp4;
	CIcsdisp8ctrl1 m_Disp5;
//	CIcsdisp8ctrl1 m_Disp6;
//	CIcsdisp8ctrl1 m_Disp7;
//	CIcsdisp8ctrl1 m_Disp8;

	CIcsdisp8ctrl1 m_Disp20;
	CIcsdbedt8ctrl1 m_DbEdit20;
	void SetFocusIcsdbedt8ctrl20();
	void TerminationIcsdbedt8ctrl20(long nChar, long inplen, long kst);
};
