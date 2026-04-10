// dbUcRenKamoku.h : ヘッダー ファイル
//
#ifndef		_DB_UC_REN_KAMOKU_H_
#define		_DB_UC_REN_KAMOKU_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenKamoku レコードセット

class CdbUcRenKamoku : public CdbBase
{
public:
	CdbUcRenKamoku(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcRenKamoku)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcRenKamoku, CRecordset)
	long	m_Seq;
	int		m_FormSeq;
	BYTE	m_FgCoType;
	CString	m_KcdStart;
	long	m_KnSeq;
	BYTE	m_FgDft;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcRenKamoku)
	public:
	int		RequerySeq(long inSeq);
	int		RequeryKnSeq(long inKnSeq);
	int		RequeryFormSeqFgCoType(int inFormSeq, BYTE inFgCoType, int inAscType);
	int		CreateNewRecord(int inFormSeq, BYTE	inFgCoType);
	int		CheckRepeat(int inFormSeq, BYTE inFgCoType, CString inKcdStart , int inType = 0 );
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

#endif		//_DB_UC_REN_KAMOKU_H_
