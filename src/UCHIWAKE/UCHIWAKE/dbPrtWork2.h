// dbPrtWork2.h : ヘッダー ファイル
//

#ifndef	_DB_PRT_WORK2_H_
#define	_DB_PRT_WORK2_H_

#include "dbBase.h"

class CdbPrtWork2 : public CdbBase
{
public:
	CdbPrtWork2(CDatabase* pDatabase = NULL, int pFormSeq = 0);
	DECLARE_DYNAMIC(CdbPrtWork2)

// フィールド/パラメータ データ
	long	m_row;			// 01.行番号
	long	m_Seq;			// 02.シーケンス番号
	int		m_NumPage;		// 03.ページ番号
	BYTE	m_NumRow;		// 04.行番号
	BYTE	m_FgFunc;		// 05.特殊行フラグ
	BYTE	m_FgShow;		// 06.表示フラグ

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
	int			CreateNewRecord();
	int			RequeryFgShowSortPageRow(int inFgShow);
	int			RequeryPageRow(CString pRequery);

public:
	int			m_nFormSeq;
	CString		m_sTblName;
};

#endif	//_DB_PRT_WORK_H_
