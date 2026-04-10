// dbUcRenMain.h : ヘッダー ファイル
//
#ifndef		_DB_UC_REN_MAIN_H_
#define		_DB_UC_REN_MAIN_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenMain レコードセット

class CdbUcRenMain : public CdbBase
{
public:
	CdbUcRenMain(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcRenMain)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcRenMain, CdbBase)
	int		m_FormSeq;
	int		m_OpRenType;
	BYTE	m_OpRenTypeKz;
	BYTE	m_OpRenTypeEz;
	BYTE	m_OpRenTypeDa;
	BYTE	m_OpRenTypeKs;
	long	m_OpTkcodFrom;
	long	m_OpTkcodTo;
	BYTE	m_OpOutZeroEn;
	BYTE	m_OpGassan;
	BYTE	m_OpUpdType;
	int		m_OpOldRenType;
// midori 161110 add -->
	BYTE	m_OpEdaren;
// midori 161110 add <--
// midori 152153 add -->
	BYTE	m_OpKmkname;
	BYTE	m_OpF9Upd;
// midori 152153 add <--
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcRenMain)
	public:
	int		RequeryFormSeq(int inFormSeq);
	virtual int Init(int inFormSeq);
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif		//_DB_UC_REN_MAIN_H_
