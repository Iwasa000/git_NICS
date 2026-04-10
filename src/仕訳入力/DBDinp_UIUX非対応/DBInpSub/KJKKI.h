// KJKKI.h : CKJKKIrecord の宣言

#pragma once

// コードの生成元 2007年2月1日, 9:43

class CKJKKIrecord : public CRecordset
{
public:
	CKJKKIrecord(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CKJKKIrecord)

// フィールド/パラメータ データ

// 次の文字列型は (記載されていれば) データベース フィールドの
// 実際のデータ型を反映しています。CStringA は ANSI データ型を、また CStringW は Unicode データ型を
// 反映しています。これは、ODBC ドライバが不必要な変換を
// 行わないようにするためです。これらのメンバを CString 型に変えて、
// ODBC ドライバが必要な変換をすべて行うようにすることも可能です。
// (注 : Unicode とこれらの変換の両方をサポートするためには、
// ODBC ドライバのバージョン 3.5 かそれ以降のものを使う必要があります)

	CKJKKI	kj;
/*
	CStringA	m_KKI_KJCD;
	CStringA	m_KKI_KN50;
	BYTE	m_KKI_PGEN;
	long	m_KKI_PYMD;
	BYTE	m_KKI_CGEN;
	long	m_KKI_CYMD;
	BYTE	m_KKI_YGEN;
	long	m_KKI_YYMD;
	BYTE	m_KKI_KGEN;
	long	m_KKI_KYMD;
	BYTE	m_KKI_HAIF;
	BYTE	m_KKI_FURI;
	long	m_KKI_HSAG;
	CStringA	m_KKI_HACD;
	CStringA	m_KKI_EICD;
	CStringA	m_KKI_GBCD;
	long	m_KKI_RIT01;
	long	m_KKI_RIT02;
	long	m_KKI_RIT03;
	CStringA	m_KKI_NAME;
	CStringA	m_KKI_RYNM;
	CStringA	m_KKI_SENM;
	CStringA	m_KKI_BIKO;
	CStringA	m_KKI_GENB;
	CStringA	m_KKI_DMCD01;
	CStringA	m_KKI_DMCD02;
	CStringA	m_KKI_DMCD03;
	BYTE	m_KKI_DMSW01;
	BYTE	m_KKI_DMSW02;
	BYTE	m_KKI_DMSW03;
	BYTE	m_KKI_DMSW04;
	BYTE	m_KKI_DMSW05;
	long	m_KKI_DMSW06;
	long	m_KKI_DMSW07;
	long	m_KKI_DMSW08;
	long	m_KKI_DMSW09;
	long	m_KKI_DMSW10;
	double	m_KKI_DMVL01;
	double	m_KKI_DMVL02;
	double	m_KKI_DMVL03;
	double	m_KKI_DMVL04;
	double	m_KKI_DMVL05;
	CStringA	m_KKI_DMNM01;
	CStringA	m_KKI_DMNM02;
	CStringA	m_KKI_DMNM03;
	long	m_REG_DATE;
	long	m_REG_TIME;
	int	m_REG_USER;
	long	m_UPD_DATE;
	long	m_UPD_TIME;
	int	m_UPD_USER;
*/

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


