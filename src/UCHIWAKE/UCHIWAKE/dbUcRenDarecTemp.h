// dbUcRenDarecTemp.h : ヘッダー ファイル
//
#ifndef		_DB_UC_REN_DAREC_TEMP_H_
#define		_DB_UC_REN_DAREC_TEMP_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenDarecTemp レコードセット

class CdbUcRenDarecTemp : public CdbBase
{
public:
	CdbUcRenDarecTemp(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcRenDarecTemp)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcRenDarecTemp, CRecordset)
	long	m_seq;
	long	m_ddate;
	CString	m_dbt;
	CString	m_cre;
	CString	m_val;
	CString	m_tekiyo;
	CString	m_invono;	// インボイス登録番号追加対応_23/11/27 add
	BYTE	m_FgConv;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcRenDarecTemp)
	public:
	int		RequerySeq(long inSeq);
	int		RequeryFgConvLikeTekiyo(BYTE inFgConv, CString inTekiyo);
	int		RequeryFgConv(BYTE inFgConv);
	int		CreateNewRecord(long inSeq);
	int		DeleteAllRecord();
	virtual int Init(long inSeq);
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	int m_SeqUserStartNum;		// Seqのユーザ利用番号の開始値を指示(定数)
								// m_SeqUserStartNum コンストラクタにて設定
};

#endif		//_DB_UC_REN_DAREC_TEMP_H_
