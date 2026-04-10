// dbUc051Tanaoroshi.h : ヘッダー ファイル
//

#ifndef	_DB_UC_051_TANAOROSHI_H_
#define	_DB_UC_051_TANAOROSHI_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc051Tanaoroshi レコードセット

class CdbUc051Tanaoroshi : public CdbUc000Common
{
public:
	int GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable);
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual int GetSortIkkatuOrder();
// midori 155525_2 del -->
	//virtual int CreateRangaiTempTable();
// midori 155525_2 del <--
// midori 155525_2 add -->
	virtual int CreateRangaiTempTable( int pOutZero );
// midori 155525_2 add <--
// 157153 del -->
//// midori 156968 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--

	CdbUc051Tanaoroshi(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc051Tanaoroshi)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc051Tanaoroshi, CRecordset)
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	CString	m_HdName;
	CString	m_Suuryou;
	CString	m_Tanni;
	CString	m_Price;
	CString	m_Val;
	CString	m_Teki;
	BYTE	m_FgInput;
// midori 152137 add -->
	CString m_KnKana;
// midori 152137 add <--

	CString m_rangai_tmp_table;
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
	//{{AFX_VIRTUAL(CdbUc051Tanaoroshi)
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

#endif	//_DB_UC_051_TANAOROSHI_H_
