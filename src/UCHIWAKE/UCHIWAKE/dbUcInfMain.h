// dbUcInfMain.h : ヘッダー ファイル
//

#ifndef		_DB_UC_INF_MAIN_H_
#define		_DB_UC_INF_MAIN_H_

#include "dbBase.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfMain レコードセット

class CdbUcInfMain : public CdbBase
{
public:
	int		UpdateFgUchiConv(BYTE bFgUchiConv);
	int		UpdateSSymdEEymd(long lngSSymd, long lngEEymd);
	virtual int Init();
	CdbUcInfMain(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUcInfMain)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUcInfMain, CRecordset)
	int			m_FormSeq;
	BYTE		m_OutKikan;
	BYTE		m_OutKingaku;		// D0:金額欄を空欄で出力する	D1:金額０円及び空欄データを出力しない　D2:金額０円を印字する　（2014/08 D1とD2は個別に持つため、以降使用しない）
	BYTE		m_OutPage;
	BYTE		m_OutZero;
	int			m_Seq;
	BYTE		m_FgUchiConv;
	long		m_ssymd;
	long		m_eeymd;
	BYTE		m_OutCoName;
	CByteArray m_GeneralConstVal;
	CByteArray m_GeneralVar;
	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUcInfMain)
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

#endif		//_DB_UC_INF_MAIN_H_