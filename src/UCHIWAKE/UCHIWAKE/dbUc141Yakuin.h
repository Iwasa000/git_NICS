// dbUc141Yakuin.h : ヘッダー ファイル
//

#ifndef	_DB_UC_141_YAKUIN_H_
#define	_DB_UC_141_YAKUIN_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc141Yakuin レコードセット

class CdbUc141Yakuin : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual CString GetSQLSumColumn();
	virtual int GetColumnNum();
	virtual CString GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei=NULL, int nFgFunc=0, CMyArray* arryTotalAll=NULL);
	virtual CString GetSQLZeroJudgment();
	virtual CString GetSQLValIsNull();
// 157153 del -->
//// midori 156968 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--

	CdbUc141Yakuin(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc141Yakuin)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc141Yakuin, CRecordset)
	CString	m_NameYaku;
	CString	m_NameTant;
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_Kankei;
	BYTE	m_FpTime;
	CString	m_EmVal;			// 2006.10.23：新会社法対応
	CString	m_PrVal;			// 2006.10.23：新会社法対応
	CString	m_BfVal;			// 2006.10.23：新会社法対応
	CString	m_PfVal;			// 2006.10.23：新会社法対応
	CString	m_EtVal;			// 2006.10.23：新会社法対応
	CString	m_RtVal;			// 2006.10.23：新会社法対応
	CString	m_Val;				// 2006.10.23：新会社法対応
	BYTE	m_FgInput;
	//}}AFX_FIELD

/*
	long	m_Seq;
	int		m_NumPage;
	BYTE	m_NumRow;
	BYTE	m_FgFunc;
	BYTE	m_FgShow;
	CString	m_KeiStr;
	long	m_NumGroup;
*/

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUc141Yakuin)
	public:
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif	//_DB_UC_141_YAKUIN_H_
