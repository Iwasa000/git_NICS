// dbUcInfSonotaItem.h : ヘッダー ファイル
//
#ifndef		_DB_UC_IND_SONOTA_ITEM_H_
#define		_DB_UC_IND_SONOTA_ITEM_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSonotaItem レコードセット

class CdbUcInfSonotaItem : public CdbBase
{
public:
	int RequeryFormSeq(int FormSeq);
	virtual int Init(int FormSeq);
	CdbUcInfSonotaItem(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfSonotaItem)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfSonotaItem, CRecordset)
	int		m_FormSeq;
	CString	m_ItemName;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfSonotaItem)
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

#endif	//_DB_UC_IND_SONOTA_ITEM_H_
