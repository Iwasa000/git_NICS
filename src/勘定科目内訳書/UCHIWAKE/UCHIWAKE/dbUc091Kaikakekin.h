// dbUc091Kaikakekin.h : ヘッダー ファイル
//

#ifndef	_DB_UC_091_KAIKAKEKIN_H_
#define	_DB_UC_091_KAIKAKEKIN_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc091Kaikakekin レコードセット

class CdbUc091Kaikakekin : public CdbUc000Common
{
public:
	int GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable);
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int UpdateDataTableWork();
	virtual int GetSortIkkatuOrder();
// midori 155525_2 del -->
	//virtual int CreateRangaiTempTable();
// midori 155525_2 del <--
// midori 155525_2 add -->
	virtual int CreateRangaiTempTable( int pOutZero );
	int			SetNullLineData091( int pOutZero );
// midori 155525_2 add <--
// 157153 del -->
//// midori 156968 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--

	CdbUc091Kaikakekin(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc091Kaikakekin)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc091Kaikakekin, CRecordset)
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_Val;
	CString	m_Teki;
// midori 152137 add -->
	CString m_KnKana;
	CString m_AdKana;
// midori 152137 add <--
	CString	m_InvNo;	// インボイス登録番号追加対応_23/10/20 add

	CString m_rangai_tmp_table;
	//}}AFX_FIELD

/*
	long	m_Seq;
	int		m_NumPage;
	BYTE	m_NumRow;
	BYTE	m_FgFunc;
	BYTE	m_FgShow;
	CString	m_KeiStr;
	long	m_NumGroup;
*/

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUc091Kaikakekin)
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

#endif	//_DB_UC_091_KAIKAKEKIN_H_
