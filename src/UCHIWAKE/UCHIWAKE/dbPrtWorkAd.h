// dbPrtWork3.h : ヘッダー ファイル
//

#ifndef	_DB_PRT_WORKAD_H_
#define	_DB_PRT_WORKAD_H_

#include "dbBase.h"

class CdbPrtWorkAd : public CdbBase
{
public:
// midori UC_0014 del -->
//	CdbPrtWorkAd(CDatabase* pDatabase = NULL, int pFormSeq = 0);
// midori UC_0014 del <--
// midori UC_0014 add -->
	CdbPrtWorkAd(CDatabase* pDatabase = NULL, int pFormSeq = 0, int tblId = 1);
// midori UC_0014 add <--
	DECLARE_DYNAMIC(CdbPrtWorkAd)

// フィールド/パラメータ データ
	long	m_AdOrder2;
	long	m_Seq;
// midori UC_0014 add -->
	long	m_NumPage;
// midori UC_0014 add <--
	BYTE	m_FgFunc;
// midori UC_0014 add -->
	long	m_NumGroup;
// midori UC_0014 add <--
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdKana;

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
// midori UC_0014 add -->
	int			RequerySort2( CString strFilter = _T("") );
// midori UC_0014 add <--

public:
	int			m_nFormSeq;
	CString		m_sTblName;
// midori UC_0014 add -->
	int			m_tblId;
// midori UC_0014 add <--
};

#endif	//_DB_PRT_WORKAD_H_
