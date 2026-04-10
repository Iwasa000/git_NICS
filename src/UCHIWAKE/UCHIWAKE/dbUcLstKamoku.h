// dbUcLstKamoku.h : ヘッダー ファイル
//

#ifndef		_DB_UC_LST_KAMOKU_H_
#define		_DB_UC_LST_KAMOKU_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstKamoku レコードセット

class CdbUcLstKamoku : public CdbBase
{
public:
	int	 CheckRepeat( int nFormSeq, CString szKnName, CString szKnKana = NULL );
	long CheckEntry(CString inKnName , int nFormSeq, CString inKnKana = NULL );
// midori 161109 add -->
	long CheckEntry_M(CString inKnName , int nFormSeq, int nSort, CString inKnKana = NULL );
// midori 161109 add <--
	int RequeryKnOrder(int inFormSeq, int inKnOrder);
	int UpdateDataTable(int inFormSeq, int mode = 0);
	int UpdateDataTableSub(CString inTableName, int inFormSeq, int mode);
	int RequeryKnSeq( int nFormSeq, int nKnSeq );
	int ReNumberOrder(int inFormSeq, int inKnOrder, int inIncDec);
	int CreateNewRecord(int inFormSeq);
	int RequeryForm(int FormSeq);
	virtual int Init(int FormSeq);
	virtual int Init();
// midori 161109 add -->
	int RequeryForm_M(int FormSeq, int nSort);
	virtual int Init_M(int FormSeq, int nSort);
	virtual int Init_M(int nSort);
// midori 161109 add <--

	CdbUcLstKamoku(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcLstKamoku)

	int SetIkkatuVal(long seq, const CString& val);
	void GetIkkatuNeedVal(long& seq, int& order);
	void GetIkkatuNeedVal(long& seq, int& order, CString& val, CString& name);
	CString GetIkkatuOrg(int nFormSeq);													// uc_inf_subから一括金額のデフォルト値を取得

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcLstKamoku, CRecordset)
	BYTE	m_FgDel;
	BYTE	m_FgDft;
	int		m_FormSeq;
	CString	m_KnCode;
	CString	m_KnName;
	long	m_KnOrder;
	long	m_KnSeq;
	CString m_KnKana;
	CString m_IkkatuVal;
// midori 160610 add -->
	BYTE	m_PrSign;
// midori 160610 add <--
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcLstKamoku)
	public:
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


private:
	int m_SeqUserStartNum;		//KnSeqのユーザ利用番号の開始値を指示(定数)
								//m_SeqUserStartNum コンストラクタにて設定

};

#endif		//_DB_UC_LST_KAMOKU_H_
