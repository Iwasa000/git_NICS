// dbUcInfYokuki.h : ヘッダー ファイル
//
#ifndef		_DB_UC_INF_YOKUKI_H_
#define		_DB_UC_INF_YOKUKI_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfYokuki レコードセット

class CdbUcInfYokuki : public CdbBase
{
public:
	CdbUcInfYokuki(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfYokuki)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfYokuki, CdbBase)
	int		m_FormSeq;
	BYTE	m_OpYokukiFg;
	BYTE	m_OpYokuki;
	BYTE	m_OpClearFg;
	BYTE	m_OpClear;
	BYTE	m_OpHokanFg;
	BYTE	m_OpHokan;
// midori 152387 add -->
	BYTE	m_OpNullClearFg;
	BYTE	m_OpNullClear;
// midori 152387 add <--
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfYokuki)
	public:
	int		RequeryFormSeq(int inFormSeq);
	virtual int Init(int inFormSeq);
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
			int InitSort(BOOL SortType=1);
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif		//_DB_UC_INF_YOKUKI_H_