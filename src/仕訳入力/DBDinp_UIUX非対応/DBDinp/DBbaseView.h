#pragma once

#include <DBNpSub8.h>
#include "SelectData.h"
#include "icssel.h"

#include "SelWndDlg.h"


// 選択画面 消費税ポジション
// SelectData.h の SL_XXXXX_PN の数値とかぶらないように
//
//#define BVW_SYZ_PN		20
#define BVW_SYZ_PN		90		// 01.25 /13


#define	PRINT_NONE		0
#define	PRINT_VERIFY	1
#define	PRINT_DOCIMG	2



// CDBbaseView フォーム ビュー

class CDBbaseView : public ICSFormView
{
	DECLARE_DYNCREATE(CDBbaseView)

protected:
	CDBbaseView() {};           // 動的生成で使用される protected コンストラクタ
	CDBbaseView(UINT id);
	virtual ~CDBbaseView();

	CICSSel	m_selfix8;
	CICSSel	m_selval32;
	CSelGuideCtrl m_selGuid;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

	BOOL		BRNTKY_sel;
	BOOL		bKamokuCode;
	int			m_SelDispPN;
	int			m_SET_SYZINP_PN;	// 消費税ダイアログ表示時に最初にカーソル置いておきたい場所
	char		m_tky_50kana[20];	// 摘要５０音検索用 カナ

	BOOL		m_bNotNumber;

	CSyzInp*	m_pSyzInp;
	CHusenInp*	m_pHusenInp;
	CHjiyuInp*	m_pHjiyuInp;
	CSubDataInp*	m_pSubDtInp;
	
	RECT	m_original_rect;	//設計時のビューサイズ
	CRect	m_ChkViewRect;		//サイズ変更されたかどうかのチェック用

	static int CALLBACK SetSelectFix8( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp );
	static int CALLBACK SetSelectSel32( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp );

	BOOL		m_bmkj_kanaF12;

	int			m_InpMode;		// 入力形式（通常入力：伝票入力）
public:
	static CSelectData m_Seldata;
	static BOOL	m_bSelDataInit;

	static	CVPrint	m_Vprn;
	static int		m_ImgPrintFlg;
	static int		m_End_PrintDlg;
	static NPRT_PAR	m_Pset;			/*- プリンタ情報 -*/

	PCOL_DEF		m_Cset;			/*- カラー定義パケット -*/
	SF_PAR			sSfpr;			/*- 出力用文字種 -*/
	PCSV_PAR		m_Csvp;
	CString			m_csvCnam;

	BOOL fix8cnd();
	void fix8cnd( BOOL sg );
	BOOL fixvalcnd();
	void fixvalcnd( BOOL sg );
	void fix8disp( BOOL sg );
	
	/* 固定選択画面の状態 */
	void enable_fix8( BOOL sg );
	
	void enable_fixval( BOOL sg );
	void set_valtitle( char *msg );
	void valtitle( BOOL sg );

	void SelectDispAdjust();

	int InitWork(BOOL syuse);
	virtual void CloseWork();
	void UpdateSelectPar();
	void SetSelectWindow();
	void ResetSelect();

	BOOL tky_50kana_chk( char *data );
	int tky_select( CDBipTKREC* ptkrec, char *data );

	void SyzInpMake();
	void Destroy_SyzInp();
	virtual void Destroy_SyzInpSubFunc() {};
	void SyzInpDelete();
	void DispSyzIchiran( WPARAM wParam, LPARAM lParam );

	void HusenInpMake();
	void Destroy_HusenInp();
	void HusenInpDelete();

	void HjiyuInpMake();
	void Destroy_HjiyuInp();
	void HjiyuInpDelete();

	void SubDataInpMake();
	void SubDataInpDelete();
	void Destroy_SubDataInp();

	BOOL InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );

	int	 BRNTKY_select( int bmn, CString& code, char* kana = NULL, BOOL bDisp = FALSE );
	long SelItemDsp( long nChar );

	virtual void LastDataGet();
	virtual void ForkUcomSet();
	virtual int ReloadView();
	virtual int TabClickChangedPrevView();
	virtual int TabLbuttonDownJob() { return 0; };
	virtual void ReCoselSetFocus( BOOL bNewCo ) {};
	virtual void OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	virtual void SoundBuzzer() {};
	virtual void CalcMouseEvent();

	BOOL bmncode_todata( CDBINPDataRec* data, int number, int dc_sw, int hchk = 0, CString* bmnname = NULL );
	BOOL kojicode_todata( CDBINPDataRec* data, CString* kjnam, int number, int dc_sw );

	long SelBmonDsp( long nChar );
	long SelKojiDsp( long nChar );
	long SelTkyDsp( long nChar );

	BOOL Kojicode_Ok( CString& kjcode );
	int  BaseSelectJob( SelPar* par, long nChar = -1 );

	BOOL IsEnableHjiyu( CDBINPDataRec *pcrec );
	BOOL IsHjiyuOrNPfuri( LPCTSTR dbt, LPCTSTR cre );

	BOOL koji_noinpcheck( CString& kno, CString& code, int dbcr_sg );
	BOOL koji_kanseicheck( CString& kno, CString& code, int dbcr_sg );

	BOOL bmn_noinpcheck( int bmn, CString& code, int dbcr_sg, CString* pMsg = NULL );
	BOOL& NotSelNumber() { return m_bNotNumber; };

	BOOL EdabanDispReload( CString& kmkcd, int bmncd =-2 );
	long SelNaibuDsp( long nChar );

#ifdef _SLIP_ACCOUNTS_
	int tky_select_in_img( CDBipTKREC* ptkrec, char *data );
#endif

	int MakeSelWnd( BOOL bIgnoreActive = FALSE );
	void SelWndDelete();

	BOOL& BumonKojiKanaF12();
	void  RDzeritsuModeWrite( int old_mode );

	//印刷関連
	static int   SetNprtParam(CImgMng* pMng );
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);


	//複数カナ文字検索
	int FukuKanaSearch(MSG* pMsg, int edaonly = 0, int possgn = 0);
	int KanaCheckVK_BACK(MSG* pMsg, int possgn = 0);
	virtual HWND GetKamokuCtrlHWND();
	virtual void SelectWindowShow(BOOL bShow) { };

	virtual BOOL EnableCallPro();

protected:
	BOOL	fix8sg;
	BOOL	fixvalsg;
	RECT m_selfix8_rect;
	RECT m_valtitle_rect;

	BOOL	flgPen;

	COLORREF	m_vwColor;

	CSelWndDlg*	m_pSelWnd;

	BOOL	m_bRegSwk, m_bRegDlg;
	struct _RDTATBL	m_RDTATBL;

	void	RdataTblIniz( void );
	int		RemainRdata( void );
	BOOL	GetRdata( CDBINPDataRec* rdrec, CDBINPDataRec* pCrec, CDBINPDataRec* pSrec, BOOL* cnd );

	virtual	void RDRemainDispJOB( void ) {}
public:
	void	EndRdata( void );
	int		rd_reccheck( CDBINPDataRec *crec, CDBINPDataRec *pRDrec, DWORD* pSyCode );
	void	InvnoCheck(CDBINPDataRec* prec);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


