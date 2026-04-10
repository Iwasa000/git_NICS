// dbUc111Kariirekin.h : ヘッダー ファイル
//

#ifndef	_DB_UC_111_KARIIREKIN_H_
#define	_DB_UC_111_KARIIREKIN_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc111Kariirekin レコードセット

class CdbUc111Kariirekin : public CdbUc000Common
{
public:
	int GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable);
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
	virtual int UpdateDataTableWork();
	virtual CString GetSQLSumColumn();
	virtual int GetColumnNum();
	virtual CString GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei=NULL, int nFgFunc=0, CMyArray* arryTotalAll=NULL);
	virtual CString GetSQLZeroJudgment();
	virtual CString GetSQLInsertColumns(CString BaseTbl);
// midori 152138 del -->
//// midori 161111 cor -->
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
	virtual CString GetSQLValIsNull();
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int GetSortIkkatuOrder();

	CdbUc111Kariirekin(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc111Kariirekin)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc111Kariirekin, CRecordset)
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_Kankei;
	CString	m_Val;
	CString	m_Risoku;
	CString	m_Rate;
	CString	m_Riyuu;
	CString	m_Naiyou;
// midori 152137 add -->
	CString m_KnKana;
	CString m_AdKana1;
	CString m_AdKana2;
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
	//{{AFX_VIRTUAL(CdbUc111Kariirekin)
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

#endif	//_DB_UC_111_KARIIREKIN_H_
