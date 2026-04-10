#pragma once
#include "..\icsdiagctrl.h"


// CShinRefundHjn01 ダイアログ

class CShinRefundHjn01 : public CRefundBaseDialog
{
	DECLARE_DYNAMIC(CShinRefundHjn01)

public:
	CShinRefundHjn01(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinRefundHjn01();

// ダイアログ データ
	enum { IDD = IDD_SYZREFUND_HJ1 };

	// 終了処理
	void EndJob();

//2016.02.23 UPDATE START
//	void	KinyuuDlgHjn( int sw );
	int		KinyuuDlgHjn( int sw );
//2016.02.23 UPDATE END
	// 保存データの更新
	void	ReUpdateData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CIcsdiagctrl m_RiyuuDiag;
	CIcsdiagctrl m_Meisai1Diag;
	CIcsdiagctrl m_Meisai2Diag;
	CIcsdiagctrl m_YusyutsuDiag;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//2016.02.23 INSERT START
	//void Line_Delete( int line_idx );
	//void Line_Insert( int line_idx );
	afx_msg void OnBnClickedButtonDel1();
	afx_msg void OnBnClickedButtonIns1();
	afx_msg void OnBnClickedButtonDel2();
	afx_msg void OnBnClickedButtonIns2();
//2016.02.23 INSERT END

	DECLARE_EVENTSINK_MAP()
//	void EditONIcsdiagctrlMeisaiH1(short index);
//	void EditOFFIcsdiagctrlMeisaiH2(short index);
//	void EditONIcsdiagctrlMeisaiH2(short index);
};
