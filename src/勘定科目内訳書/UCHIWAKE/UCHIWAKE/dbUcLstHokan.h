// dbUcLstHokan.h : ヘッダー ファイル
//

#ifndef		_DB_UC_LST_HOKAN_H_
#define		_DB_UC_LST_HOKAN_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstHokan レコードセット

class CdbUcLstHokan : public CdbBase
{
public:
	CdbUcLstHokan(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcLstHokan)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcLstHokan, CRecordset)
	int		m_FormSeq;
	long	m_Seq;
	long	m_OrderNum;
	//}}AFX_FIELD

public:
	virtual int CreateNewRecord();
	int		RequeryFormSeq(int FormSeq);
	int		RequerySeqFormSeq(int nFormSeq, int Seq);
	int		DeleteFormSeq(int nFormSeq);
//	int		GetCountFormSeq(int nFormSeq);

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcLstHokan)
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

#endif		//_DB_UC_LST_HOKAN_H_