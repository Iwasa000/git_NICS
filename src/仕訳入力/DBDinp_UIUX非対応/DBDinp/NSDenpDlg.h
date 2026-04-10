#pragma once


// CNSDenpDlg ダイアログ
#include "DnpBaseDlg.h"
#include "icsdiag8ctrl.h"
#include "icsdisp.h"
#include "ImageStatic.h"
#include "HusenStatic.h"


class CNSDenpDlg : public CDnpBaseDlg
{
	DECLARE_DYNAMIC(CNSDenpDlg)

public:
	CNSDenpDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CNSDenpDlg();

public:
	CIcsdiag8ctrl m_diag[2];
	CICSDisp		m_Djob;
	CICSDisp		m_Bkmk;
	ImageStatic	m_image1;
	ImageStatic	m_image2;
	ImageStatic	m_image3;
	ImageStatic	m_image4;
	ImageStatic	m_image5;
	ImageStatic	m_image6;
	ImageStatic	m_image7;
	ImageStatic	m_image8;

	CHusenStatic	m_Husen1;
	CHusenStatic	m_Husen2;
	CHusenStatic	m_Husen3;
	CHusenStatic	m_Husen4;
	CHusenStatic	m_Husen5;
	CHusenStatic	m_Husen6;
	CHusenStatic	m_Husen7;

// ダイアログ データ
	enum { IDD = IDD_DLG_NSKIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	CWnd*	m_pParent;
	int		m_Type;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void diag_inz();
	virtual void ChangeJob( int job );
	virtual int	 del_idx( int ln );
	virtual void GetDeleteRect( int ln, CRect& rect );
	virtual void GetLineRect( int ln, CRect& rect );
	virtual void AutoSiwake( int mode );
	virtual void DispLineInfo( int nowln );

	virtual void SetDataLineColor( int ln, COLORREF back, COLORREF txt, char* syz_chg );

	void	ChangeDenpType( int type );
	void	DispBaseKmk( BOOL bDisp, CString& str );

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);

	void ImeEndCompositionIcsdbedt8ctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	void TerminationIcsdbedt8ctrl(UINT ID, long nChar, long inplen, long kst);
	void SetFocusIcsdbedt8ctrl(UINT ID);
	void KillFocusIcsdbedt8ctrl(UINT ID, long inplen);
	void KanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana);

	virtual int ScrollbarMsgFunc(  UINT Msg, WPARAM wParam, LPARAM lParam );

	CScrollBar m_scBar;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	virtual void PostNcDestroy();
public:
	void KanaTermIcsdiag8ctrl2(short index, LPCTSTR kana);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
