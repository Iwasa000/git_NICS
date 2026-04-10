#pragma once


// CSeqChgDlg ダイアログ

class CSeqChgDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSeqChgDlg)

public:
	CSeqChgDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSeqChgDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_SEQCHG };

	void SeqInfoSet();
	void set_focus( int pn );

public:

	CDWordArray	m_SeqAry;
	int			m_sCnt;

	int			m_BaseSeq;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdbedtctrl1(long nChar, long inplen, long kst);

	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
