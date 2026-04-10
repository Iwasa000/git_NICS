// dbPrtWork3.h : ヘッダー ファイル
//

#ifndef	_DB_PRT_WORKBK_H_
#define	_DB_PRT_WORKBK_H_

#include "dbBase.h"

class CdbPrtWorkBk : public CdbBase
{
public:
	CdbPrtWorkBk(CDatabase* pDatabase = NULL, int pFormSeq = 0);
	DECLARE_DYNAMIC(CdbPrtWorkBk)

// フィールド/パラメータ データ
	long	m_BkOrder2;
	long	m_Seq;
	BYTE	m_FgFunc;
	long	m_BkSeq;
	long	m_BkOrder;
	long	m_KnOrder;
	CString	m_BkName1;
	CString m_BkName2;
	CString	m_BkKana1;
	CString	m_BkKana2;

// オーバーライド
	// ウィザードで生成された仮想関数のオーバーライド
public:
	virtual CString GetDefaultSQL();	// レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	int			RequerySort( int nType, CString strSort2 );
// midori 190505 add -->
	int			RequerySort2(CString strFilter);
// midori 190505 add <--

public:
	int			m_nFormSeq;
	CString		m_sTblName;
};

#endif	//_DB_PRT_WORKBK_H_
