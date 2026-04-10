// BaseDlg.h : ヘッダー ファイル
//

#ifndef __BASEDLG_INCLUDE__
#define __BASEDLG_INCLUDE__


/////////////////////////////////////////////////////////////////////////////
// CBaseDlg ダイアログ

//#include "ICSDialog.h"

//#include "Tkzmsub.h"
//#include  <ICSPrint.h>



class CBaseDlg : public ICSDialog
{
// コンストラクション
public:
	CBaseDlg(UINT dlgID, CWnd* pParent = NULL);   // 標準のコンストラクタ
	BOOL CreateDlg();

	void DlgWndMove( double x, double y );
	virtual void WndShow(BOOL bShow);

	virtual void WndShow_Add(BOOL bShow);

	virtual void FocusMove( int direction ) 
	{
		_ASSERTE( !"Don't Call BaseDlg FocusMove()" );
	};
	virtual void SetFirstPosition() 
	{
		_ASSERTE( !"Don't Call BaseDlg SetFirstPosition()" );
	};
	virtual void InitWork( double x, double y ) 
	{
		m_Xrate = x; m_Yrate = y;
	};
	// ツールバー Enable / Disable
	virtual BOOL IsEnableToolBar( int nID )
	{
		return FALSE;
	}
	virtual BOOL IsDataBaseUpdated()
	{
		return m_bDBupdate;
	}
	virtual void LastDataGet()
	{
	}

#ifdef CLOSE
	// 印刷関連
	virtual int TK_PreparePrt( PRTSET_INFO* pSet, BOOL bPreview ) 
	{
		_ASSERTE( !"Don't Call BaseDlg TK_PreparePrt()" );
		return 0;
	};
	virtual void DataPrt(void) 
	{
		_ASSERTE( !"Don't Call BaseDlg DataPrt()" );
	};
	virtual BOOL TkPrtSelect( BOOL bPreview )
	{
		_ASSERTE( !"Don't Call BaseDlg TkPrtSelect()" );
		return FALSE;
	}

	virtual void EndPrtWork(void) {};
#endif

	virtual	void AddTekiyoJob( LPARAM lParam ) {};

//	void Set_Class( CDBZmSub* pZm, CVPrint* pPrt );
	void Set_Class( CDBZmSub* pZm, CDBSyohi* pSy );

// ダイアログ データ
	//{{AFX_DATA(CBaseDlg)
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBaseDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	UINT		m_DlgID;
	CWnd*		m_pParent;
	
	// リソース のサイズ
	int			m_Xdl, m_Ydl;
	// リサイズ 比率
	double		m_Xrate, m_Yrate;
#ifdef CLOSE
	// 印刷 表関係
	LIST_PAR	m_PGLIST;
	int			m_hPGLIST;
	// アクセラレータハンドル
	HACCEL		m_hAccelTable;
#endif

	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
//	CVPrint*	m_pPrt;

	BOOL		m_bDBupdate;

	// 生成されたメッセージ マップ関数

	afx_msg LRESULT OnUser1_Msg( WPARAM wParam, LPARAM lParam );
	//{{AFX_MSG(CBaseDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



#endif //__BASEDLG_INCLUDE__
