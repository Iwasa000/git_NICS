#pragma once


// CTkyEdaDlg ダイアログ
#include "icslistinp.h"


class CTkyEdaDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CTkyEdaDlg)

public:
	CTkyEdaDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTkyEdaDlg();

	CICSListInp	m_list;

// ダイアログ データ
	enum { IDD = IDD_DLG_TKYEDA };

	void	Set_Class( CDBZmSub* pZm, CDBSyohi* pSy, DBKNREC* pKn, int tkcod );
	void	Set_Info( CString& tky, CString& kana, int inieda, char* zan );
	void	MakeEdaListInp();
	void	MakeEdabanLine( CEZREC* pez );
	void	SetEdabanData();
	void	InitEdabanCode();
//	int		NewEdabanSearch( LPCTSTR kcod, int eda, int ketachk = 0 );
	int		EdaNameInpSearch( LPCTSTR kcod );

	void	GetTkyEdaData(int& mk_eda, char* zan );
	void	EdaInpCheck();

protected:
	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
	DBKNREC		m_knrec;
	int			m_tkcod;
	int			m_inieda;

	int			m_MkEda;
	CString		m_svTky, m_svKana;
	char		m_Zan[6];

	CDWordArray	m_daEda;
	int			m_TKYO_LEN;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst);
	void TerminationIcsdbedt8ctrl2(long nChar, long inplen, long kst);

	void set_focus( int id );
	LRESULT OnRetroFocus(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
public:
	void KeyInIcslinp8ctrl1(short nChar, short previndex, short index);
	void LButtonDblClickIcslinp8ctrl1(short index);
	void KillFocusIcsdbedt8ctrl1(long inplen);
};
