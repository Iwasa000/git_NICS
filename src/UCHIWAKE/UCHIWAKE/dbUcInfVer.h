// dbUcInfVer.h : ヘッダー ファイル
//
#ifndef		_DB_UC_INF_VER_H_
#define		_DB_UC_INF_VER_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfVer レコードセット

class CdbUcInfVer : public CdbBase
{
public:
	int VersionCheck(int inVer);
	CdbUcInfVer(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfVer)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfVer, CRecordset)
	int		m_Seq;
	int		m_Version;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfVer)
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

#endif		//_DB_UC_INF_VER_H_
