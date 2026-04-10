// TkswkDlg.h : ヘッダー ファイル
//
//{{AFX_INCLUDES()
#include "icslistinp.h"
//}}AFX_INCLUDES
#ifndef INCLUDE_TKSWKDLG_H
#define INCLUDE_TKSWKDLG_H

/////////////////////////////////////////////////////////////////////////////
// CTkswkDlg ダイアログ
#include "icslistinp.h"

#include "icsdbedt.h"
#include "BaseDlg.h"



class CTkswkDlg : public CBaseDlg
{
// コンストラクション
public:
	CTkswkDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTkswkDlg)
	enum { IDD = IDD_DLG_SWKTK };
	CICSListInp	m_listswk;
	CICSListInp	m_listtky;
	CButton	m_Copy;
	//}}AFX_DATA
	CICSDBEDT	m_DBEdt;

	virtual void WndShow(BOOL bShow);
	virtual void WndShow_Add(BOOL bShow);
	virtual void FocusMove( int direction );
	virtual void SetFirstPosition();
	virtual void InitWork( double x, double y );
	virtual BOOL IsEnableToolBar( int nID );
	virtual void AddTekiyoJob( LPARAM lParam );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTkswkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void MakeSiwakeFrm();
	void MakeTkrenFrm();

	void SWKselDisp( BOOL bFirst = FALSE );
	void TorokDisp();
	int SiwakeInpSet( BOOL bMastRead );
	void SwkInpAllSetSub();
	void SetNewSwkLine();
	int	TkyoInpSet( int idx );
	void SetNowLineBkColor( int setidx, BOOL bDraw = FALSE );
	void TkrenListPosMark( int mode );

	BOOL BothDispSwtkyo( CString& dbt, CString& cre );
	BOOL IsValidSwtkyoIdx( int idx );

	void SwtkNewlineCheck( short index, short clm  );
	void BothKamokColorSet( short index );

	BOOL CheckSwtkData( short idx, short colm, LPUNKNOWN data, BOOL bDelete = FALSE );
	void DeleteSwtkData( int idx );
	BOOL ChangeSwtkData( int idx, SWKCOD_DATA& sd );
	
	int  DeleteTkyo( int index );
	void SetTkyListData( int index, int code, CString Kname );

	BOOL DoubleRegChk( int code, SWKCOD_DATA* pRec = NULL );
	BOOL DoubleRegChkEX( SWKCOD_DATA *pChkRec, SWKCOD_DATA *pRec = NULL );
	BOOL DoubleReg_SwkChk( SWKCOD_DATA sd, int curidx = -1 );
	void TekiCopySub( LPARAM lParam );
	int	 TkyCopyDB( int srcIdx, int dtIdx, int add_sgn = 0 );

	void GetSwkRegMsg( CString& msg, SWKCOD_DATA sc );

	void TkyoAddSub( SWKCOD_DATA* psc, int code );
	void SetNewTkyoInput( int index );
	BOOL IsEnableAddNewSwkLine();
	void HomeButtonCondition();

	BOOL swk_kanasearch( int index, int clm, LPCTSTR kana, CString& newcode );

	CSwcdArray	m_Swcd;
	SWKCOD_DATA	m_CurData;

	int		m_Didx, m_Dclm;		// 現在処理データインデックス & カラム
	int		m_CurDidx;
	int		m_Tkyidx;
	int		m_InsIdx;
	int		m_PosMarkIndex;
	int		m_swkMax, m_swkCnt;
	BOOL	m_bFocusSet;
	BOOL	m_bInitOK;
	BOOL	m_bSetCred;		// 貸方に入力カーソルをセットするか
	BOOL	m_bFirstShow;	// 初回表示サイン
	BOOL	m_bTkTopDisp;
	BOOL	m_bNoCurSwkCheck;	// EditON でのカレントレコードの更新をしない

	int		m_TKYO_LEN;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTkswkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCopy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnUser1_Msg( WPARAM wParam, LPARAM lParam );
	LRESULT OnRetroFocus( WPARAM wParam, LPARAM lParam );
	LRESULT OnListInsDel( WPARAM wParam, LPARAM lParam );

public:
	void EditONIcslinp8Swk(short index, short column);
	void TerminationIcslinp8Swk(short nChar, short len, short index, short clm, LPUNKNOWN data);
	void EditONIcslinp8Tky(short index, short column);
	void TerminationIcslinp8Tky(short nChar, short len, short index, short clm, LPUNKNOWN data);
	void FocusedIcslinp8Tky();
	void EditOFFIcslinp8Tky(short nChar);

	void ImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring);
	void KanaTerminationIcsdbedtctrl1(LPCTSTR Kana);
	void SetFocusIcsdbedtctrl1();
	void KillFocusIcsdbedtctrl1(long inplen);
	void TerminationIcsdbedtctrl1(long nChar, long inplen, long kst);

public:
	void KeyInIcslinp8Swk(short nChar, short previndex, short index);
	void KeyInIcslinp8Tky(short nChar, short previndex, short index);
	void FocusedIcslinp8Swk();
	void SelChangeIcslinp8Swk(short index);
};


#endif // INCLUDE_TKSWKDLG_H
