// recordset.h : ヘッダー ファイル

/////////////////////////////////////////////////////////////////////////////
// CBmeszsyk レコードセット
class CBMESZSYK ;
class CBmeszsyk : public CRecordset
{
public:
	CBmeszsyk(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBmeszsyk)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CBmeszsyk, CRecordset)
	class CBMESZSYK	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESZSYK)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CBmespsyk レコードセット
class CBMESPSYK ;
class CBmespsyk : public CRecordset
{
public:
	CBmespsyk(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBmespsyk)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CBmespsyk, CRecordset)
	class CBMESPSYK	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CShinPayment レコードセット
class CSHINPAYMENT ;
class CShinPayment : public CRecordset
{
public:
	CShinPayment(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CShinPayment)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CShinPayment, CRecordset)
	class CSHINPAYMENT	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSHINPAYMENT)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CRefundvol レコードセット
class CREFUNDVOL ;
class CRefundvol : public CRecordset
{
public:
	CRefundvol(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRefundvol)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CRefundvol, CRecordset)
	class CREFUNDVOL	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CRefundrec レコードセット
class CREFUNDREC ;
class CRefundrec : public CRecordset
{
public:
	CRefundrec(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRefundrec)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CRefundrec, CRecordset)
	class CREFUNDREC	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CSkjrec レコードセット
class CSKJREC ;
class CSkjrec : public CRecordset
{
public:
	CSkjrec(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSkjrec)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CSkjrec, CRecordset)
	class CSKJREC	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSkjrec)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CReask レコードセット
class CREASK ;
class CReask : public CRecordset
{
public:
	CReask(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CReask)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CReask, CRecordset)
	class CREASK	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CReask)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CKjsnk レコードセット
class CKJSNK ;
class CKjsnk : public CRecordset
{
public:
	CKjsnk(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CKjsnk)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CKjsnk, CRecordset)
	class CKJSNK	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CKjsnk)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CSh_lst_val レコードセット（消費税申告書（金額））	('14.01.06)
class CSH_LST_VAL ;
class CSh_lst_val : public CRecordset
{
public:
	CSh_lst_val(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSh_lst_val)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CSh_lst_val, CRecordset)
	class CSH_LST_VAL	sh;
	//}}AFX_FIELD

public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSh_lst_val)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CSh_lst_rate レコードセット（消費税申告書（割合））	('14.01.06)
class CSH_LST_RATE ;
class CSh_lst_rate : public CRecordset
{
public:
	CSh_lst_rate(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSh_lst_rate)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CSh_lst_rate, CRecordset)
	class CSH_LST_RATE	sh;
	//}}AFX_FIELD

public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSh_lst_rate)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//2018.03.13 INSERT START
// 付表６　レコードセット
class CFHYO60REC ;
class CFhyo60Rec : public CRecordset
{
public:
	CFhyo60Rec(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CFhyo60Rec)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CRefundrec, CRecordset)
	class CFHYO60REC	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

// 付表６　相続人　レコードセット
class CSYZ_SOZOKU ;
class CSyz_Sozoku : public CRecordset
{
public:
	CSyz_Sozoku(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSyz_Sozoku)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CRefundrec, CRecordset)
	class CSYZ_SOZOKU	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
	public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
//2018.03.13 INSERT END

// 特例帳票　レコードセット
class CSYZ_TOKUREI;
class CSyz_Tokurei : public CRecordset
{
public:
	CSyz_Tokurei(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSyz_Tokurei)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CRefundrec, CRecordset)
	class CSYZ_TOKUREI	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

// 消費税集計サブテーブル　レコードセット
class CSZSYKSUB;
class CSzsyksub : public CRecordset
{
public:
	CSzsyksub(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSzsyksub)

	// フィールド/パラメータ データ
	//{{AFX_FIELD(CRefundrec, CRecordset)
	class CSZSYKSUB	pr;
	//}}AFX_FIELD

public:
	// 管理用クラス
//	CDBPrtblW*	m_pCDBPrtblW;
//	void SetDBPrtbl( CDBPrtblW*	pCDBPrtblW ){ m_pCDBPrtblW = pCDBPrtblW; };

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBMESPSYK)
public:
	virtual CString GetDefaultSQL();				 	// デフォルトの Recordset 用の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
