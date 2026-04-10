// dbUcLstItemSort.h : ヘッダー ファイル
//

#ifndef		_DB_UC_LST_ITEM_SORT_H_
#define		_DB_UC_LST_ITEM_SORT_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstItemSort レコードセット

class CdbUcLstItemSort : public CdbBase
{
public:
	int RequeryFormItem(int FormSeq, int ItemSeq);
	int RequeryForm(int FormSeq);
	virtual int Init(int FormSeq);
	CdbUcLstItemSort(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcLstItemSort)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcLstItemSort, CRecordset)
	int		m_FormSeq;
	BYTE	m_ItemSeq;
	CString	m_ItemName;
	BYTE	m_FgPage;
	BYTE	m_FgSykei;
	BYTE	m_FgItem;
	CString	m_OrderStr;
	CString	m_FieldSykei;
	CString	m_FieldSykei2;
// midori 152137 add -->
	CString	m_OrderStr2;
// midori 152137 add <--
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcLstItemSort)
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

#endif		//_DB_UC_LST_ITEM_SORT_H_
