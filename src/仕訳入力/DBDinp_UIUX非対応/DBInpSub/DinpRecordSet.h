#ifndef	INCLUDE_DINPRECORDSET_H
#define	INCLUDE_DINPRECORDSET_H


/////////////////////////////////////////////////////////////////////////////
// CSetuptbl レコードセット

class CSetuptbl : public CRecordset
{
public:
	CSetuptbl(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSetuptbl)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CSetuptbl, CRecordset)
	CString	m_setupname;
	long	m_param;
	CString	m_parastr;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSetuptbl)
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


/////////////////////////////////////////////////////////////////////////////
// CProc レコードセット

class CProc : public CRecordset
{
public:
	CProc(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CProc)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CProc, CRecordset)
	long		m_st;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CProc)
	public:
	virtual CString GetDefaultSQL(); 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


#endif