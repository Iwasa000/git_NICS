#pragma once
#include "afxwin.h"
#include "icsreportcontrol.h"
#include "icsbutton.h"
#include "icsdiag.h"

// 158063 del -->
//typedef struct _AMOUNT_REC1	{
//	int		code;	// 取引先コード
//	CString	name;	// 取引先名称
//	CString	valu;	// 総額
//}AMOUNTREC1;
// 158063 del <--

// CdlgAmount1 ダイアログ

class CdlgTorihikiAmount1 : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgTorihikiAmount1)

public:
	CdlgTorihikiAmount1(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CdlgTorihikiAmount1();

// 158125 del -->
//// 158057 del -->
//	//int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv );
//// 158057 del <--
//// 158057 add -->
//	int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv, IkkatuValHandler& ivh );
//// 158057 add <--
// 158125 del <--
// 158125 add -->
	int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv, IkkatuValHandler& ivh, int pZero );
// 158125 add <--
// 158057 del -->
	//int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv );
// 158057 del <--
// 158057 add -->
	int ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv, IkkatuValHandler& ivh );
// 158057 add <--
// 158063 del -->
	//void MakeMap( CDatabase* pDB, int pFormSeq );
	//CString GetAmount( int pCode, CString pName );
// 158063 del <--

private:
	void InitReportHeader();
// 158063 del -->
	//CString MakeSql( CDatabase* pDB, int pFormSeq );
// 158063 del <--
	void AddRecords();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TORIHIKI_AMOUNT1 };

private:
	ICSButton			m_btnOK;
	ICSReportControl	m_report;
	CICSDiag			m_DiagBC;

	int			m_nRet;
	CDatabase*	m_pDB;
	int			m_FormSeq;
	CString		m_IkkatuVal;
	int			m_UseAdv;
// 158057 add -->
	IkkatuValHandler m_IkkatuValHandler;
// 158057 add <--
// 158125 add -->
	int			m_Zero;
// 158125 add <--
// 158063 del -->
	//CMap<int,int,AMOUNTREC1,AMOUNTREC1>	m_AmountMap;
// 158063 del <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
