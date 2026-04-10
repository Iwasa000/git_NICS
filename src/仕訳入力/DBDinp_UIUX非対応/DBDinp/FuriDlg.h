#pragma once

#include "DnpBaseDlg.h"
#include "icsdiag8ctrl.h"
#include "icsdisp.h"
#include "ImageStatic.h"
#include "HusenStatic.h"
#include "TransparentStatic.h"

// CFuriDlg ダイアログ

class CFuriDlg : public CDnpBaseDlg
{
	DECLARE_DYNAMIC(CFuriDlg)

public:
	CFuriDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CFuriDlg();

public:
	CIcsdiag8ctrl	m_diag[2];
	CICSDisp		m_Djob;
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

//	CTransparentStatic	m_stFocus;


// ダイアログ データ
	enum { IDD = IDD_DLG_FURI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	void	BmkjTitleChange();

	DECLARE_MESSAGE_MAP()

protected:
	int		m_FuriMode;

public:
	virtual BOOL OnInitDialog();	
	virtual void diag_inz();
	virtual void ChangeJob( int job );
	virtual int	 del_idx( int ln );
	virtual void GetDeleteRect( int ln, CRect& rect );
	virtual void GetLineRect( int ln, CRect& rect );
	virtual void AutoSiwake( int mode );

	virtual void DispLineInfo( int nowln );

	void	FuriModeChange( int mode );
	void	SetTanbmnCheck( int nCheck );
	void	SiwakeTypeDisp( LPCTSTR type );

	void	syog_iniz();

	CWnd*	m_pParent;

public:
	DECLARE_EVENTSINK_MAP()

public:
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void KanaTermIcsdiag8ctrl2(short index, LPCTSTR kana);

protected:
	virtual void PostNcDestroy();
public:
	void ImeEndCompositionIcsdbedt8ctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	void TerminationIcsdbedt8ctrl(UINT ID, long nChar, long inplen, long kst);
	void SetFocusIcsdbedt8ctrl(UINT ID);
	void KillFocusIcsdbedt8ctrl(UINT ID, long inplen);
	void KanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana);

public:
	afx_msg void OnBnClickedBmninpChk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CScrollBar m_scBar;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	virtual int ScrollbarMsgFunc(  UINT Msg, WPARAM wParam, LPARAM lParam );
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
