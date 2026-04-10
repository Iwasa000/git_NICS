// dbUc131Uriagedaka.h : ヘッダー ファイル
//

#ifndef	_DB_UC_131_URIAGEDAKA_H_
#define	_DB_UC_131_URIAGEDAKA_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc131Uriagedaka レコードセット

class CdbUc131Uriagedaka : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual int UpdateDataTableWork();
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

	CdbUc131Uriagedaka(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc131Uriagedaka)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc131Uriagedaka, CRecordset)
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_HdName;
	CString	m_Kankei;
	CString	m_Naiyou;
	CString	m_Val;
	CString	m_Val2;
	CString	m_Worker;
	CString	m_Area;
	CString	m_HdName2;
	CString	m_Teki;
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
	//{{AFX_VIRTUAL(CdbUc131Uriagedaka)
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

#endif	//_DB_UC_131_URIAGEDAKA_H_
