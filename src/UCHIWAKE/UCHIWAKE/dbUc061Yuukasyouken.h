// dbUc061Yuukasyouken.h : ヘッダー ファイル
//

#ifndef	_DB_UC_061_YUUKASYOUKEN_H_
#define	_DB_UC_061_YUUKASYOUKEN_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc061Yuukasyouken レコードセット

class CdbUc061Yuukasyouken : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int UpdateYokukiZeroClear();
// ---------------------
	virtual int UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual	CString GetZeroMoneyMoveField();
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

	CdbUc061Yuukasyouken(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc061Yuukasyouken)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc061Yuukasyouken, CRecordset)
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	CString	m_Syurui;
	CString	m_Suuryou;
	CString	m_Tanni;
	CString	m_Val;
	CString	m_Val2;
	long	m_SpDate;
	CString	m_Riyuu;
	CString	m_Suuryou2;
	CString	m_Tanni2;
	CString	m_Val3;
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_Teki;
// midori 152137 add -->
	CString m_KnKana;
// midori 152137 add <--
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
	//{{AFX_VIRTUAL(CdbUc061Yuukasyouken)
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

#endif	//_DB_UC_061_YUUKASYOUKEN_H_
