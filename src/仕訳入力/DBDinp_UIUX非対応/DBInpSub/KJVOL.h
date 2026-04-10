// KJVOL.h : CKJVOL の宣言

#pragma once

// コードの生成元 2011年9月12日, 15:01

class CKJVOL : public CRecordset
{
public:
	CKJVOL(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CKJVOL)

// フィールド/パラメータ データ

// 次の文字列型は (記載されていれば) データベース フィールドの
// 実際のデータ型を反映しています。CStringA は ANSI データ型を、また CStringW は Unicode データ型を
// 反映しています。これは、ODBC ドライバが不必要な変換を
// 行わないようにするためです。これらのメンバを CString 型に変えて、
// ODBC ドライバが必要な変換をすべて行うようにすることも可能です。
// (注 : Unicode とこれらの変換の両方をサポートするためには、
// ODBC ドライバのバージョン 3.5 かそれ以降のものを使う必要があります)

	CStringA	m_VOL_IDCD;
	CStringA	m_VOL_CODE;
	int	m_VOL_APNO;
	BYTE	m_VOL_MTYP;
	long	m_VOL_PEDA01;
	long	m_VOL_PEDA02;
	long	m_VOL_PEDA03;
	CStringA	m_VOL_PUCD01;
	CStringA	m_VOL_PUCD02;
	CStringA	m_VOL_PUCD03;
	CStringA	m_VOL_PUCD04;
	CStringA	m_VOL_PUCD05;
	CStringA	m_VOL_PUCD06;
	CStringA	m_VOL_PUCD07;
	CStringA	m_VOL_PUCD08;
	CStringA	m_VOL_PUCD09;
	CStringA	m_VOL_PUCD10;
	BYTE	m_VOL_HFSW;
	CStringA	m_VOL_HKJC;
	int	m_VOL_HTUK01;
	int	m_VOL_HTUK02;
	int	m_VOL_HTUK03;
	int	m_VOL_HTUK04;
	int	m_VOL_HTUK05;
	int	m_VOL_HTUK06;
	int	m_VOL_HTUK07;
	int	m_VOL_HTUK08;
	int	m_VOL_HTUK09;
	int	m_VOL_HTUK10;
	int	m_VOL_HTUK11;
	int	m_VOL_HTUK12;
	int	m_VOL_HTUK13;
	int	m_VOL_HTUK14;
	int	m_VOL_HTUK15;
	int	m_VOL_HTUK16;
	BYTE	m_VOL_HCSW01;
	BYTE	m_VOL_HCSW02;
	BYTE	m_VOL_HCSW03;
	BYTE	m_VOL_HCSW04;
	BYTE	m_VOL_HCSW05;
	BYTE	m_VOL_HCSW06;
	BYTE	m_VOL_HCLQ01;
	BYTE	m_VOL_HCLQ02;
	BYTE	m_VOL_HCLQ03;
	BYTE	m_VOL_HCLQ04;
	BYTE	m_VOL_FKSW01;
	BYTE	m_VOL_FKSW02;
	BYTE	m_VOL_FKSW03;
	int	m_VOL_FKKM;
	int	m_VOL_FKKT;
	int	m_VOL_PRKM;
	int	m_VOL_PRKT;
	BYTE	m_VOL_PRSW01;
	BYTE	m_VOL_PRSW02;
	BYTE	m_VOL_PRSW03;
	BYTE	m_VOL_DMSW01;
	BYTE	m_VOL_DMSW02;
	BYTE	m_VOL_DMSW03;
	BYTE	m_VOL_DMSW04;
	BYTE	m_VOL_DMSW05;
	BYTE	m_VOL_DMSW06;
	BYTE	m_VOL_DMSW07;
	BYTE	m_VOL_DMSW08;
	BYTE	m_VOL_DMSW09;
	BYTE	m_VOL_DMSW10;
	BYTE	m_VOL_DMNO01;
	BYTE	m_VOL_DMNO02;
	BYTE	m_VOL_DMNO03;
	BYTE	m_VOL_DMNO04;
	BYTE	m_VOL_DMNO05;
	CStringA	m_VOL_DMNM01;
	CStringA	m_VOL_DMNM02;
	CStringA	m_VOL_DMNM03;
	CStringA	m_VOL_DMNM04;
	CStringA	m_VOL_DMNM05;
	long	m_REG_DATE;
	long	m_REG_TIME;
	long	m_REG_USER;
	long	m_UPD_DATE;
	long	m_UPD_TIME;
	long	m_UPD_USER;

// オーバーライド
	// ウィザードで生成された仮想関数のオーバーライド
	public:
	virtual CString GetDefaultConnect();	// 既定の接続文字列

	virtual CString GetDefaultSQL(); 	// レコードセットの既定の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX サポート

// 実装
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


