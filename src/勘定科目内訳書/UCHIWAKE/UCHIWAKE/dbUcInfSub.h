// dbUcInfSub.h : ヘッダー ファイル
//
#ifndef		_DB_UC_INF_SUB_H_
#define		_DB_UC_INF_SUB_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSub レコードセット

class CdbUcInfSub : public CdbBase
{
public:
	int UpdateCurInfo();
	int UpdateTitleTK(int inFormSeq, int inTK);
	int RequeryFormOrder(int FormOrder);
	int RequeryFormSeq(int FormSeq);
	virtual int Init(int FormIndex);
	int			InitOrder(BOOL SortType=1);
	CdbUcInfSub(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfSub)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfSub, CRecordset)
	int		m_CurCtlIndex;
	BYTE	m_CurField;
	int		m_CurPage;
	BYTE	m_CurRow;
	BYTE	m_FormNo;
	BYTE	m_FormNo2;
	BYTE	m_FormNo3;
	int		m_FormOrder;
	int		m_FormSeq;
	BYTE	m_OpCykeiKakko;
	CString	m_OpCykeiName;
	BYTE	m_OpIkkatu;
	BYTE	m_OpIkkatuFg;
	CString	m_OpIkkatuVal;
	CString	m_OpIkkatuValOrg;
	BYTE	m_OpKaiPage;
	BYTE	m_OpOutNullLine;
	BYTE	m_OpOutSyokei;
	BYTE	m_OpSykeiAuto;
	BYTE	m_OpSykeiKakko;
	CString	m_OpSykeiName;
	BYTE	m_OpTitleKn;
	BYTE	m_OpTitleKnFg;
	BYTE	m_OutKei;
	BYTE	m_Sort1;
	BYTE	m_Sort2;
	BYTE	m_Sort3;
	CString	m_Title;
	CString	m_TitleNo;
	CString	m_TitleNo2;
	CString	m_TitleOrg;
	CString	m_TitleOrgTujo;
	CString	m_TitleSave;
	CString	m_TitleSaveTujo;
	CByteArray m_GeneralConstVal;
	CByteArray m_GeneralVar;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfSub)
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

#endif		//_DB_UC_INF_SUB_H_
