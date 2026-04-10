// dbUcLstAddress.h : ヘッダー ファイル
//
#ifndef		_DB_UC_LST_ADDRESS_H_
#define		_DB_UC_LST_ADDRESS_H_

#include "dbBase.h"
#include "FuriganaConverter.h"
//#include "UcFunctionCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstAddress レコードセット

class CdbUcLstAddress : public CdbBase
{
public:
	//int RequerySeq( int nSeq );
	//int CreateNewRecord();
	//int ReNumberOrder(int inOrderNum, int inIncDec);
	//virtual int Init();

	//??
	int AutoNumOrder(int inSeq, int inGrSeq);
	int CheckEntry	( CString inAdName1
					, CString inAdName2
					, CString inAdAdd1
					, CString inAdAdd2
					, int     inGrSeq
					, BOOL	  isRegFurigana
					);
	int CheckEntryAddNone	( CString inAdName1
							, CString inAdName2
							, CString inAdAdd1
							, CString inAdAdd2
							, int     inGrSeq
							, BOOL    fAddress
							, BOOL	  isRegFurigana
							);
// midori 152137 add -->
	int CheckEntryRen	( CString inAdName1
						, CString inAdName2
						, CString inAdAdd1
						, CString inAdAdd2
						, CString inAdKana
						, int     inGrSeq
						, BOOL	  isRegFurigana
						);
	int CheckEntryAddNoneRen	( CString inAdName1
								, CString inAdName2
								, CString inAdAdd1
								, CString inAdAdd2
								, CString inAdKana
								, int     inGrSeq
								, BOOL    fAddress
								, BOOL	  isRegFurigana
								);
// midori 152137 add <--
	int CheckRepeat ( CString	inAdName1
				    , CString	inAdName2
				    , CString	inAdAdd1
				    , CString	inAdAdd2
				    , int		inGrSeq
					, BOOL		fAddress = TRUE
					, CString	inAdKana = NULL
					);
	//??

	int UpdateStrFieldAllForNull();
// midori 152137 del -->
	//int UpdateDataTableSub	( CString inTableName		// データテーブル名
	//						, CString inNameAdSeq		// フィールド名
	//						, CString inNameAdName1		// フィールド名
	//						, CString inNameAdName2		// フィールド名
	//						, CString inNameAdAdd1		// フィールド名
	//						, CString inNameAdAdd2		// フィールド名
	//						, CString inNameAdOrder		// フィールド名
	//						, BOOL isRegAuto
	//						, BOOL isDeleteAdSeqZero = TRUE
	//						);
// midori 152137 del <--
// midori 152137 add -->
	int UpdateDataTableSub	( CString inTableName		// データテーブル名
							, CString inNameAdSeq		// フィールド名
							, CString inNameAdName1		// フィールド名
							, CString inNameAdName2		// フィールド名
							, CString inNameAdAdd1		// フィールド名
							, CString inNameAdAdd2		// フィールド名
							, CString inNameAdOrder		// フィールド名
							, CString inNameAdKana		// フィールド名
							, BOOL isRegAuto
							, BOOL isDeleteAdSeqZero = TRUE
// midori 190504 add -->
							, int nSgn = 0
// midori 190504 add <--
							);
// midori 152137 add <--
// midori 190504 del -->
	//int UpdateDataTableAll(BOOL isRegAuto);
	//int UpdateDataTable(int inFormSeq, BOOL isRegAuto);
// midori 190504 del <--
// midori 190504 add -->
	int UpdateDataTableAll(BOOL isRegAuto,int nSgn = 0);
	int UpdateDataTable(int inFormSeq, BOOL isRegAuto, int nSgn = 0);
// midori 190504 add <--
	//int UpdateDataTable(CString inTableName);
	//int UpdateDataTable(CString inTableName, int inFormSeq);
	int ReNumberOrder(int inGrSeq, int inOrderNum, int inIncDec);
	int CreateNewRecord(int inGrSeq);
	virtual int Init(int inGrSeq);
	virtual int Init();
// midori 161109 add -->
	virtual int Init_M(int inGrSeq, int nSort);
	virtual int Init_M(int nSort);
// midori 161109 add <--
	int RequeryAdOrder(int inGrSeq, int inOrder);
	int RequeryAdSeq( int inGrSeq, int inAdSeq );
	int RequeryGrSeq(int inGrSeq);
// midori 161109 add -->
	int RequeryGrSeq_M(int inGrSeq, int nSort);
// midori 161109 add <--
	int RequeryAdName(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2);
	int RequeryAdName(int inGrSeq, CString adname1, CString adname2);
	int GetSeqFromAdName(int inGrSeq, CString adname1, CString adname2);
	int GetSeqFromAdName(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2);
	CdbUcLstAddress(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcLstAddress)

	int SetIkkatuVal(long seq, const CString& val);
	void GetIkkatuNeedVal(long& seq, int& order);
	void GetIkkatuNeedVal(long& seq, int& order, CString& val, CString& name);
	CString GetIkkatuValAddress(int inGrSeq);
// midori 190301 add -->
	void Data042Convert( void );
// midori 190301 add <--
// midori 155301 del -->
// midori 190504 add -->
//	int RequeryAdName(int GrSeq);
// midori 190504 add <--
// midori 155301 del <--
// midori 155301 add -->
	int RequeryAdName(int GrSeq, int pSw);
// midori 155301 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	int RequeryAdInfo(int pGrSeq, int pAdSeq);

// 修正No.168402,168405 add -->
	int CheckAdSeqEntry_Inv(int		inAdSeq
						  , CString	inAdName1
						  , CString	inAdName2
						  , CString	inAdAdd1
						  , CString	inAdAdd2
						  , CString	inInvNo
						  , int		inGrSeq
						  , BOOL	fAddress
						  , BOOL	isRegFurigana
						  , int		isFlag
						  );
// 修正No.168402,168405 add <--
	int CheckEntry_Inv(CString inAdName1
					 , CString inAdName2
					 , CString inAdAdd1
					 , CString inAdAdd2
					 , CString inInvNo
					 , int     inGrSeq
					 , BOOL	   isRegFurigana
					 );
	int CheckEntryAddNone_Inv(CString inAdName1
							, CString inAdName2
							, CString inAdAdd1
							, CString inAdAdd2
							, CString inInvNo
							, int     inGrSeq
							, BOOL    fAddress
							, BOOL	  isRegFurigana
							);
	int CheckEntryRen_Inv(CString inAdName1
						, CString inAdName2
						, CString inAdAdd1
						, CString inAdAdd2
						, CString inAdKana
						, CString inInvNo
						, int     inGrSeq
						, BOOL	  isRegFurigana
						);
	int CheckEntryAddNoneRen_Inv( CString inAdName1
								, CString inAdName2
								, CString inAdAdd1
								, CString inAdAdd2
								, CString inAdKana
								, CString inInvNo
								, int     inGrSeq
								, BOOL    fAddress
								, BOOL	  isRegFurigana
								);
	int CheckRepeat_Inv(  CString	inAdName1
						, CString	inAdName2
						, CString	inAdAdd1
						, CString	inAdAdd2
						, CString	inInvNo
						, int		inGrSeq
						, BOOL		fAddress = TRUE
						, CString	inAdKana = NULL
						);
// 修正No.168402,168405 add -->
	int CheckRepeat_Inv2( int		inAdSeq
						, CString	inAdName1	// 修正23/12/28 add
						, CString	inAdName2	// 修正23/12/28 add
						, CString	inAdAdd1	// 修正No.168534,168539 add
						, CString	inAdAdd2	// 修正No.168534,168539 add
						, int		inGrSeq
	);
// 修正No.168402,168405 add <--
// 修正No.168479 add -->
	int CheckRepeat_Inv3( CString	inAdName1
						, CString	inAdName2
						, CString	inAdAdd1
						, CString	inAdAdd2
						, CString	inInvNo
						, int		inGrSeq
						, BOOL		fAddress = TRUE
						, CString	inAdKana = NULL
						);
// 修正No.168479 add <--

	int UpdateDataTableSub_Inv(CString inTableName		// データテーブル名
							 , CString inNameAdSeq		// フィールド名
							 , CString inNameAdName1	// フィールド名
							 , CString inNameAdName2	// フィールド名
							 , CString inNameAdAdd1		// フィールド名
							 , CString inNameAdAdd2		// フィールド名
							 , CString inNameAdOrder	// フィールド名
							 , CString inNameAdKana		// フィールド名
							 , CString inNameInvono		// フィールド名
							 , BOOL    isRegAuto
							 , BOOL    isDeleteAdSeqZero = TRUE
							 , int     nSgn = 0
							);

	int UpdateDataTableAll_Inv(BOOL isRegAuto,int nSgn = 0);
	int UpdateDataTable_Inv(int inFormSeq, BOOL isRegAuto, int nSgn = 0);
	int GetSeqFromAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString invno);
	int GetSeqFromAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2, CString invno);
	int RequeryAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2, CString invno);
	int RequeryAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString invno);

	int GetAddressInf_Inv(int inGrSeq, CString invno);
// インボイス登録番号追加対応_23/11/06 add <--

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcLstAddress, CRecordset)
	long	m_AdSeq;
	int		m_GrSeq;
	long	m_OrderNum;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	BYTE	m_FgDel;
	CString m_AdKana;
	CString m_IkkatuVal;
	CString m_InvNo;		// インボイス登録番号追加対応_23/11/06 add
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcLstAddress)
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
	int m_SeqUserStartNum;		//AdSeqのユーザ利用番号の開始値を指示(定数)
								//m_SeqUserStartNum コンストラクタにて設定

	int UpdateStrFieldForNull(CString inFieldName);
};

#endif		//_DB_UC_LST_ADDRESS_H_
