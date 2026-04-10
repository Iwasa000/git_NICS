// dbUcInfFormType.h : ヘッダー ファイル
//

#ifndef		_DB_UC_INF_FORM_TYPE_H_
#define		_DB_UC_INF_FORM_TYPE_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfFormType レコードセット

class CdbUcInfFormType : public CdbBase
{
public:
	int RequeryForm(int inFormNo);
	virtual int Init(int inFormNo);
			int InitOrder(BOOL SortType=1);
	CdbUcInfFormType(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfFormType)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfFormType, CRecordset)
	BYTE	m_FormNo;
	BYTE	m_FormType;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfFormType)
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

#endif		//_DB_UC_INF_FORM_TYPE_H_

