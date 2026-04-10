#pragma once


#include "Icsdiag8ctrl.h"
//#include "icsdisp.h"


// CRDdnpBaseDlg ダイアログ
class CRDdnpBaseDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CRDdnpBaseDlg)

public:
	CRDdnpBaseDlg(UINT dlgID, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRDdnpBaseDlg();

// ダイアログ データ
//	enum { IDD = IDD_DNPBASEDLG };

	void InitBaseDiag( CIcsdiag8ctrl* pDiag, int diagcnt );
	void InitDnpBase( DNP_POS* head, DNP_POS* data, DNP_POS* kei, int totalitemcnt );
	void GetDnpNowPos( int& ln, int& pn );
	DNP_POS* GetDNP_POS( int ln, int pn );

	void SetDenpyoKind( int kind ) { m_kind = kind;	};

	void kesyu_dsp( char* str );
	void inpdata_dsp();

	void DiagClear( int dgno, int index );
	
	void FocusTkyColor( int ln, BOOL bFocus );

	void SetDataLineCount( int dcnt ) { m_LineCount = dcnt; }

	void DiagInit();
	void ctl_init( int pn, BOOL enable );

	DNP_POS* GetFirstDnpPos();
	DNP_POS* GetLastDnpPos();

//	void	TransLine( CDBipDataRec* pdt, int ln );
//	void	DeleteLine( int ln, BOOL sw );

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
#ifdef RDNP_CLOSE
	void SetTgNumber( int ln, int pn, char* tgnum);
	void SetTgDate( int ln, int pn, BYTE* tgdate);
	void SetHjiyu( int ln, int pn, CString* hjiyu );
	void SetHusen( int ln, int pn, int husen );
#endif
	void SetSyogBrn( int ln, int pn, int syogbrn );

	void PutsSyz( int dgno, int dgidx, char* syztxt );
	void SyogDisp( char* syog );
	void total_dsp();

	void GetRect( int ln, int pn, CRect& rect );

	void control_iniz();

//	void DispScanTitle( CICSDisp* disp );

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
	virtual void SetDataLineColor( int ln, COLORREF back, COLORREF txt );


	BOOL	IsDenpView();
	void	ControlPos();
	void	UpdateSubWindow( int ln );
	void	cursor_set( UINT id );

	static void SetMasterPar( int bmlng, int kjlng );

	void	diag_disableinz();
	void	set_scrollbar( CScrollBar* pscroll, int npos, int dmax, int pmax );

	virtual int ScrollbarMsgFunc(  UINT Msg, WPARAM wParam, LPARAM lParam ) {
		return 0;
	}

	BOOL	dataline_input( int ln );
	int		IsLine1Selpos( POINT pt );
	int		DiagRefresh();
	void	SetRDNPLabel( CString& label );

public:
	static	int m_BmonLNG;
	static	int m_KojiLNG;

protected:
	void _SetDnpyoNo(int dgno, int index, int dnpno );
	void _SetMM( int dgno, int index, BYTE mm );
	void _SetDD( int dgno, int index, BYTE day );

	void _SetBmon( int dgno, int index, int bmn, struct _SET_KMKETC* psk, BOOL bReset = FALSE );
	void _SetKoji( int dgno, int index, CString koji, struct _SET_KMKETC* psk, BOOL bReset = FALSE );

	void _SetKnam(int dgno, int index, SETMEISYO* knam, BOOL bReset = FALSE );
	void _SetSyogBrn(int dgno, int index, int syogbrn );
	void _SetVal(int dgno, short index, char* val, int sgn = 0);
	void _SetTekiyo(int dgno, short index, char* tky);

	void _SetTgNumber(int dgno, short index, char* tgnum);
	void _SetTgDate(int dgno, short index, BYTE* tgdate);
	void _SetHjiyu( int dgno, short index, CString* hjiyu );

//	void _delete_line( int ln, BOOL sw );
	BOOL IsReadOnly( UINT id );
	BOOL IsDelete( UINT id );

	int	 GetLineItemCount();
	int	 GetPosIndex( int ln, DNP_POS* pos );

	int		m_AllLineCount;
	int		m_ItemCount;

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

	int		m_kind;

	CString	m_sLabel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
//	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRetroFocus( WPARAM w, LPARAM l );
	afx_msg LRESULT OnRetroFocus2( WPARAM w, LPARAM l );

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
