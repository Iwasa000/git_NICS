#pragma once


#include "Icsdiag8ctrl.h"
#include "icsdisp.h"



// CDnpBaseDlg ダイアログ
class CDnpBaseDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CDnpBaseDlg)

public:
	CDnpBaseDlg(UINT dlgID, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDnpBaseDlg();

// ダイアログ データ
//	enum { IDD = IDD_DNPBASEDLG };

	void InitBaseDiag( CIcsdiag8ctrl* pDiag, int diagcnt );
	void InitDnpBase( DNP_POS* head, DNP_POS* data, DNP_POS* kei );
	void GetDnpNowPos( int& ln, int& pn );
	DNP_POS* GetDNP_POS( int ln, int pn );

	void kesyu_dsp( char* str );
	void inpdata_dsp();
	void dataline_disp( int dataln, int setln = -1,int offctrl =0 );

	void DiagClear( int dgno, int index );
	void DiagEditON( int diag_id, int index );
	void DiagEditOFF();
	void DiagTermination(int diag_id, short index, short nChar, short length, LPUNKNOWN data);
	void DiagKanaTermination(int diag_id, short index, LPCTSTR kana);
	
	void SetFocusDBEDT( UINT id );
	void KillFocusDBEDT( UINT id, long inplen );
	void TerminationDBEDT( UINT id, long nChar, long inplen, long kst );
	void ImeEndDBEDT( UINT id, long nChar, LPCTSTR string, LPCTSTR ystring );
	void KanaTermDBEDT(UINT ID, LPCTSTR Kana);

	void FocusLine( int ln, BOOL bFocus );
	void FocusTkyColor( int ln, BOOL bFocus );

	void SetDataLineCount( int dcnt ) { m_LineCount = dcnt; }

	void DiagInit();
	void ctl_init( int pn, BOOL enable );

	DNP_POS* GetFirstDnpPos();
	DNP_POS* GetLastDnpPos();

	void	TransLine( int dt_ln, int ln, char* syz_chg );
//	void	TransLine( CDBINPDataRec* pdt, int ln );
//	void	DeleteLine( int ln, BOOL sw );
	void	FuriSyogDataColor( int dt_ln, int ln );

	void	UpsideTermJob( int pn, long nChar );

	int set_focus( int ln, int pn );
	int set_nextfocus( int ln, int pn );
	int set_prevfocus( int ln, int pn );
	int set_datafirstfocus();

	void SetDnpyoNo( int pn, int dnpno );
	void SetMM( int pn, BYTE mm );
	void SetDD( int pn, BYTE day );

	void SetBmon( int ln, int pn, int bmn, struct _SET_KMKETC* psk, BOOL bReset = FALSE );
	void SetKoji( int ln, int pn, CString koji, struct _SET_KMKETC* psk, BOOL bReset = FALSE );

	void SetKnam( int ln, int pn,  SETMEISYO* knam, BOOL bReset = FALSE );
	void SetVal( int ln, int pn, char* val, int sgn = 0 );
	void SetTekiyo( int ln, int pn, char* tky);
	void SetTgNumber( int ln, int pn, char* tgnum);
	void SetTgDate( int ln, int pn, BYTE* tgdate);
	void SetHjiyu( int ln, int pn, CString* hjiyu );
	void SetHusen( int ln, int pn, int husen, char* hscmnt);
	void SetSyogBrn( int ln, int pn, int syogbrn );

	void PutsSyz( int ln, int pn, char* syztxt, char sy_chg, COLORREF backcol );
	void SyogDisp( char* syog );
	void total_dsp();

	void GetRect( int ln, int pn, CRect& rect );

	void SetImage( int mode, int ln, IMREC* imp );
	void OffImage( int ln, int mode );
	void SetFocusToImage( int ln, int sgn );
	void ImagePaintSw( int ln, int mode );
	void ImageBackColor( int ln, BOOL sgn );
	void ImageBackTrans( int ln, BOOL sgn, COLORREF transback );
	void KasiImageDisplay( int ln, BOOL sgn );
	
	void KasiTekiyoDisplay( BOOL bShow );
	void control_iniz();

	void DispScanTitle( CICSDisp* disp );

	void diag_tanbmninz( BOOL bTanBmn );

	virtual void diag_inz();
	virtual void ChangeJob( int job );
	virtual int	 del_idx( int ln ) { return -1; };
	//-----------------------------------------------------------
	//	伝票 1行の取消し線の矩形を取得
	//-----------------------------------------------------------
	virtual void GetDeleteRect( int ln, CRect& delrect ) {};
	virtual void AutoSiwake( int mode ) {};
	virtual void DispLineInfo( int nowln ) {};

	virtual void GetLineRect( int ln, CRect& rect ) {};
	virtual void SetDataLineColor( int ln, COLORREF back, COLORREF txt, char* syz_chg );

	BOOL	IsKasiTeki() { return	flgKasiTeki; }
	BOOL	IsKasiImage() { return	flgKasiImage; }

	BOOL	IsDenpView();
	void	ControlPos();
	void	UpdateSubWindow( int ln );
	void	cursor_set( UINT id );

	static void SetMasterPar( int bmlng, int kjlng, int edalng, int syonum );
	static void SetScanTitle();

	void	diag_disableinz();
	void	diag_sknbrn();
	void	set_scrollbar( CScrollBar* pscroll, int npos, int dmax, int pmax );

	virtual int ScrollbarMsgFunc(  UINT Msg, WPARAM wParam, LPARAM lParam ) {
		return 0;
	}

	BOOL	dataline_input( int ln );
	int		IsLine1Selpos( POINT pt );
	int		DiagRefresh();

	void	DiagDeleteInput();
	void	KanaSearchUpdate();

public:
	static	int m_BmonLNG;
	static	int m_KojiLNG;
	static	int m_EdaLNG;
	static  int m_SyoNumLNG;

protected:
	void _SetDnpyoNo(int dgno, int index, int dnpno );
	void _SetMM( int dgno, int index, BYTE mm );
	void _SetDD( int dgno, int index, BYTE day );

	void _SetBmon( int dgno, int index, int bmn, struct _SET_KMKETC* psk, BOOL bReset = FALSE );
	void _SetKoji( int dgno, int index, CString koji, struct _SET_KMKETC* psk, BOOL bReset = FALSE );

	void _SetKnam(int dgno, int index, SETMEISYO* knam, BOOL bReset = FALSE );
	void _SetSyogBrn(int dgno, int index, int syogbrn );
	void _SetVal(int dgno, short index, char* val, int sgn = 0, COLORREF back =(DWORD)-1);
	void _SetTekiyo(int dgno, short index, char* tky);

	void _SetTgNumber(int dgno, short index, char* tgnum);
	void _SetTgDate(int dgno, short index, BYTE* tgdate);
	void _SetHjiyu( int dgno, short index, CString* hjiyu );

//	void _delete_line( int ln, BOOL sw );
	BOOL IsReadOnly( UINT id );
	BOOL IsDelete( UINT id );

private:
	UINT		m_dlgID;
	DNP_POS*	m_pHDpos;
	DNP_POS*	m_pDTpos;
	DNP_POS*	m_pKeiPos;
	DNP_POS		m_PosBuf;	//ポジション返送用

	CIcsdiag8ctrl*	m_pDiag;
	int		m_dgno, m_index;
	int		m_LineCount;
	int		m_DclmCount;

	int		m_TkyID;
	int		m_TkyLine;

	HACCEL	m_hAccelTable;

	BOOL	flgKasiTeki;
	BOOL	flgKasiImage;

	static CString	m_ScanTitle;

	BOOL	m_bHjiyuEna;
	CScrollBar* m_pScBar;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRetroFocus( WPARAM w, LPARAM l );
	afx_msg LRESULT OnRetroFocus2( WPARAM w, LPARAM l );

	LRESULT OnImageStaticFocus( WPARAM wParam, LPARAM lParam );
	LRESULT OnImageStaticKey( WPARAM wParam, LPARAM lParam );

};
