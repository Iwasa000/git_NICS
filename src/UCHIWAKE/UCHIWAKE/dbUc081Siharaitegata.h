// dbUc081Siharaitegata.h : ヘッダー ファイル
//

#ifndef	_DB_UC_081_SIHARAITEGATA_H_
#define	_DB_UC_081_SIHARAITEGATA_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc081Siharaitegata レコードセット

class CdbUc081Siharaitegata : public CdbUc000Common
{
public:
// 改良No.21-0086,21-0529 add -->
	int		GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable);
	virtual CString GetSQLInsertColumns(CString BaseTbl);
	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto, int pFormSeq, int pNullAdd);
	virtual CString GetSQLResultSelect(CString strTable);
	virtual CString GetSQLZeroJudgment();
// 改良No.21-0086,21-0529 add <--

	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int UpdateDataTableWork();
	virtual int GetSortIkkatuOrder();
	virtual CString GetSortItemType();

// 157153 del -->
//// midori 156968 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--

	CdbUc081Siharaitegata(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc081Siharaitegata)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc081Siharaitegata, CRecordset)
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	long	m_SpDate;
	long	m_PmDate;
	long	m_BkSeq;
	long	m_BkOrder;
	CString	m_BkName1;
	CString	m_BkName2;
	CString	m_Val;
	CString	m_Teki;
// midori 152137 add -->
	CString m_AdKana;
// midori 152137 add <--
	CString	m_InvNo;	// インボイス登録番号追加対応_23/10/20 add
// 改良No.21-0086,21-0529 add -->
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	CString	m_KnKana;
// 改良No.21-0086,21-0529 add <--
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
	//{{AFX_VIRTUAL(CdbUc081Siharaitegata)
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

#endif	//_DB_UC_081_SIHARAITEGATA_H_
