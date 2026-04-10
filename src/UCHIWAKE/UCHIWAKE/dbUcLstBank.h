// dbUcLstBank.h : ヘッダー ファイル
//

#ifndef		_DB_UC_LST_BANK_H_
#define		_DB_UC_LST_BANK_H_

#include "dbBase.h"
#include "FuriganaConverter.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstBank レコードセット

class CdbUcLstBank : public CdbBase
{
public:
// midori 152137 del -->
	//int UpdateDataTableSub	( CString inTableName		// データテーブル名
	//						, CString inNameBkSeq		// フィールド名
	//						, CString inNameBkName1		// フィールド名
	//						, CString inNameBkName2		// フィールド名
	//						, CString inNameBkOrder		// フィールド名
	//						, BOOL isRegAuto
	//						, BOOL isDeleteAdSeqZero = TRUE
	//						);
// midori 152137 del <--
// midori 152137 add -->
	int UpdateDataTableSub	( CString inTableName		// データテーブル名
							, CString inNameBkSeq		// フィールド名
							, CString inNameBkName1		// フィールド名
							, CString inNameBkName2		// フィールド名
							, CString inNameBkOrder		// フィールド名
							, CString inNameBkKana1		// フィールド名（カナ（金融機関））
							, CString inNameBkKana2		// フィールド名（カナ（支店））
							, BOOL isRegAuto
							, BOOL isDeleteAdSeqZero = TRUE
// midori 190504 add -->
							, int nSgn = 0
// midori 190504 add <--
							);
// midori 152137 add <--
	int AutoNumOrder(int inSeq);
// midori 190504 del -->
//	int UpdateDataTableAll(BOOL isRegAuto);
////	int UpdateDataTable(CString inTableName);
//	int UpdateDataTable(int inFormSeq, BOOL isRegAuto);
// midori 190504 del <--
// midori 190504 add -->
	int UpdateDataTableAll(BOOL isRegAuto,int nSgn = 0);
	int UpdateDataTable(int inFormSeq, BOOL isRegAuto,int nSgn = 0);
// midori 190504 add <--
	int RequerySeq( int nSeq );
	int ReNumberOrder(int inOrderNum, int inIncDec);
	int CheckEntry(CString inBkName1, CString inBkName2);
// midori 152137 add -->
	int CheckEntryRen(CString inBkName1, CString inBkName2,CString inBkKana);
// midori 152137 add <--
	int CheckRepeat(CString inBkName1, CString inBkName2, CString inBkKana1=NULL, CString inBkKana2=NULL);
	int UpdateStrFieldAllForNull();
	int UpdateStrFieldForNull(CString inFieldName);
	int CreateNewRecord();
	int RequeryBkName(CString bkname1, CString bkname2);
	int GetSeqFromBkName(CString bkname1, CString bkname2);
// midori 190301 add -->
	void Data011Convert();
// midori 190301 add <--
	void Data021Convert(void);	// インボイス登録番号追加対応_23/11/06 add
	void Data081Convert(void);	// インボイス登録番号追加対応_23/11/06 add
	virtual int Init();
// midori 161109 add -->
	virtual int Init_M(int nSort);
// midori 161109 add <--
// midori 190504 add -->
	int RequeryBkName();
// midori 190504 add <--

	CdbUcLstBank(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcLstBank)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcLstBank, CRecordset)
	CString	m_BkName1;
	CString	m_BkName2;
	BYTE	m_FgDel;
	long	m_OrderNum;
	long	m_Seq;
	CString m_BkKana1;
	CString m_BkKana2;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcLstBank)
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
	int m_SeqUserStartNum;		// Seqのユーザ利用番号の開始値を指示(定数)
								// m_SeqUserStartNum コンストラクタにて設定

};


#endif		//_DB_UC_LST_BANK_H_
