// 改良No.22-0404,22-0812 add -->
// 修正No.162360,162361 add -->
//#ifndef	__DLGREFERENCE_H__
//#define	__DLGREFERENCE_H__

#pragma once
#include "icsbutton.h"
#include "icsreportcontrol.h"

#include "UcCommon.h"			//	定数ヘッダ
#include "afxwin.h"


// CdlgReferenceDelChk2 ダイアログ

class CdlgReferenceDelChk2 : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgReferenceDelChk2)

public:
	CdlgReferenceDelChk2(CWnd* pParent = NULL);				// 標準コンストラクター
	CdlgReferenceDelChk2(UINT id, CWnd* pParent = NULL);	// コンストラクタ
	virtual ~CdlgReferenceDelChk2();						// デストラクタ

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REFERENCE_DELCHK2 };
//#endif

public:
	CStatic				m_stcMsg1;
	CStatic				m_stcMsg2;
	CStatic				m_stcMsg3;
	CStatic				m_stcMsg4;
	CStatic				m_stcMsg5;
	CStatic				m_stcMsg6;
	CStatic				m_stcMsg7;
	ICSReportControl	m_report;
	ICSButton			m_btnOK;
	ICSButton			m_btnCancel;

private:
	CDatabase*			m_pDB;			// データベースハンドル
	int					m_nRet;			// 戻値
	CWnd*				m_pOwner;		// 呼び元のウインドウ
	EnumIdDlgType		m_nType;		// 参照型
	short				m_nFormSeq;		// 様式シーケンス番号
	CString				m_Name1;
	CString				m_Name2;
	CString				m_Name3;
	CString				m_Name4;			// 修正No.162360,162361 add
	CArray<int,int>		m_DelFormNo;
	int					m_nGrSeq;		// グループ番号
	CDBZmSub*			m_pZmSub;		// 財務dbクラス

	CUcFunctionCommon	m_clsFunc;		// 共通関数クラス

public:
	_RD_BANK			m_udBank;		//	金融機関
	_RD_ADDRESS			m_udAddress;	//	取引先


public:
	int		ShowDialog( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq,
						CString sName1, CString sName2, CArray<int,int> &cDelseq );

private:
	void	DispInit(void);				// メッセージの作成
	int		ReportInit(void);			// レポートの初期化
	int		ReportSet(void);			// レポートの表示
	void	SetReportGrid(void);		// レポート　グリッド線の設定

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedOk();

protected:
	afx_msg	LRESULT OnUser1Message(WPARAM wParam, LPARAM lParam);	// 修正No.162348 add

	virtual void OnOK();

};
// 修正No.162360,162361 add <--
// 改良No.22-0404,22-0812 add <--
