// dbUc181Sonota2.h : ヘッダー ファイル
//

#ifndef	_DB_UC_181_SONOTA2_H_
#define	_DB_UC_181_SONOTA2_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc181Sonota2 レコードセット

class CdbUc181Sonota2 : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int UpdateYokukiZeroClear();
// ---------------------
	virtual int UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual CString GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei=NULL, int nFgFunc=0, CMyArray* arryTotalAll=NULL);
	virtual int GetSortIkkatuOrder();
// 157153 del -->
//// midori 156968 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--

	CdbUc181Sonota2(CDatabase* pDatabase, int inFormSeq);
	//CdbUc181Sonota2(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc181Sonota2)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc181Sonota2, CRecordset)
	int		m_FormSeq;
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	CString	m_Item;
	CString	m_Val;
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
	//{{AFX_VIRTUAL(CdbUc181Sonota2)
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

#endif	//_DB_UC_181_SONOTA2_H_
