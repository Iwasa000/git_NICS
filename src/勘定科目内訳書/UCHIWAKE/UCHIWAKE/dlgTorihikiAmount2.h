#pragma once
#include "afxwin.h"
#include "icsreportcontrol.h"
#include "icsbutton.h"
#include "icsdiag.h"

// 158063 del -->
//typedef struct _AMOUNT_REC2	{
//	int		tcode;	// 取引先コード
//	int		kcode;	// 科目コード
//	CString	tname;	// 取引先名称
//	CString	valu;	// 総額
//}AMOUNTREC2;
// 158063 del <--

// CdlgTorihikiAmount2 ダイアログ

class CdlgTorihikiAmount2 : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgTorihikiAmount2)

public:
	CdlgTorihikiAmount2(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CdlgTorihikiAmount2();

// 158125 del -->
//// 158065,158066 del -->
////// 158057 del -->
////	//int ShowDialog( CDatabase* pDB, int pFormSeq );
////// 158057 del <--
////// 158057 add -->
////	int ShowDialog( CDatabase* pDB, int pFormSeq, IkkatuValHandler& ivh );
////// 158057 add <--
//// 158065,158066 del <--
//// 158065,158066 add -->
//	int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, IkkatuValHandler& ivh );
//// 158065,158066 add <--
// 158125 del <--
// 158125 add -->
	int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, IkkatuValHandler& ivh, int pZero );
// 158125 add <--
// 158063 del -->
	//void MakeMap( CDatabase* pDB, int pFormSeq );
	//CString GetAmount( int pTcode, CString pTname, int pKcode );
// 158063 del <--

private:
	void InitReportHeader();
// 158063 del -->
	//CString MakeSql( CDatabase* pDB, int pFormSeq );
// 158063 del <--
	void AddRecords();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TORIHIKI_AMOUNT2 };

private:
	ICSButton			m_btnOK;
	ICSReportControl	m_report;
	CICSDiag			m_DiagBC;

	int			m_nRet;
	CDatabase*	m_pDB;
	int			m_FormSeq;
// 158065,158066 add -->
	CString		m_IkkatuVal;
// 158065,158066 add <--
// 158057 add -->
	IkkatuValHandler m_IkkatuValHandler;
// 158057 add <--
// 158125 add -->
	int			m_Zero;
// 158125 add <--
// 158063 del -->
	//CMap<int,int,AMOUNTREC2,AMOUNTREC2>	m_AmountMap;
// 158063 del <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
