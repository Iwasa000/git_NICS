// dbUc021Uketoritegata.h : ヘッダー ファイル
//

#ifndef	_DB_UC_021_UKETORITEGATA_H_
#define	_DB_UC_021_UKETORITEGATA_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc021Uketoritegata レコードセット

class CdbUc021Uketoritegata : public CdbUc000Common
{
public:
	int			GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable);
	virtual int CreateNewRecord();
// 157153 del -->
//// midori 156188 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//	virtual int CreateNewRecord3(int pNumPage,int pNumRow);
//// midori 156188 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual CString GetSQLInsertColumns(CString BaseTbl);
// midori 161111 cor -->
// midori 152138 del -->
////// midori 160610 cor -->
//////	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn);
////// ---------------------
////	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto);
////// midori 160610 cor <--
//// ---------------------
//	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto, int pFormSeq);
//// midori 161111 cor <--
// midori 152138 del <--
// midori 152138 add -->
	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto, int pFormSeq, int pNullAdd);
// midori 152138 add <--
	virtual CString GetSQLResultSelect(CString strTable);
	virtual int GetSortIkkatuOrder();
	virtual CString GetSortItemType();
// midori 190505 add -->
	virtual CString GetSQLZeroJudgment();
// midori 190505 add <--

	CdbUc021Uketoritegata(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc021Uketoritegata)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc021Uketoritegata, CRecordset)
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
	long	m_Bk2Seq;
	long	m_Bk2Order;
	CString	m_Bk2Name1;
	CString	m_Bk2Name2;
	CString	m_Teki;
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
// midori 152137 add -->
	CString	m_AdKana;
	CString	m_KnKana;
// midori 152137 add <--
	CString	m_InvNo;	// インボイス登録番号追加対応_23/10/20 add
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
	//{{AFX_VIRTUAL(CdbUc021Uketoritegata)
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

#endif	//_DB_UC_021_UKETORITEGATA_H_
