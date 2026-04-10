// dbUcInfSubOmit.h : ヘッダー ファイル
//

#ifndef		_DB_UC_INF_SUB_OMIT_H_
#define		_DB_UC_INF_SUB_OMIT_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSubOmit レコードセット

class CdbUcInfSubOmit : public CdbBase
{
public:
	virtual int Init(int inFormSeq);
	CdbUcInfSubOmit(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfSubOmit)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfSubOmit, CRecordset)
	int		m_FormSeq;
	CString	m_ItemName;
	BYTE	m_ItemSeq;
	BYTE	m_Omit;
	CString	m_OmitStr;
	int		m_OmitMaxLen;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfSubOmit)
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

#endif		//_DB_UC_INF_SUB_OMIT_H_
