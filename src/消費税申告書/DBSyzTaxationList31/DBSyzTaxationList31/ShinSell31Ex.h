#pragma once
#include "icsdiagctrl.h"



// CShinSell31Ex ダイアログ

class CShinSell31Ex : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinSell31Ex)

public:
	CShinSell31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSell31Ex();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELL31_ex };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int		Init();
	int		InitDiagAttr();
// 22-0167,22-0361 del -->
	//void	InitAddText();
// 22-0167,22-0361 del <--
	void	SetItemPosition();
	void	SetItemPositionEx();
	int		ChangeColor(unsigned short id, short index, int sign);
	int		FocusMoveCheck(int index, int move);
	void	DispList( int mode );
	void	DispSell31DiagData();
	int		DispJudge( int clm, int line );
	int		GetLineNum( int index );
	int		GetColumNum( int index );
	int		GetColumNum2( int index );
	int		GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode );
	void	SetFocusRange();
	int		ClearManualInputSign();


	// 印刷情報のセット
	void SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo );
	// 印刷１
	int PrintHyo1( int pno, int fno, CDC *pDC, char *pBp );
	// 印刷２
	int PrintHyo2( int pno, int fno, CDC *pDC, char *pBp );
	// 印刷３
	int PrintHyo3( int pno, int fno, CDC *pDC, char *pBp );


	CWnd			*m_pParent;
	CSyzUtil		m_Util;
	short			m_Idx;
	short			m_Lin;

	int				m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int				m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int				m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int				m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int				m_curwnd;
	   
	//== 印刷情報 ==
	CVPrint				*m_pVprn;		// 出力クラス
	NPRT_PAR			*m_pPset;		// 印刷情報
	PBUF				PF[30];
	SH_PRINT_CMINFO		*m_PrintCmInfo;


	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_Sell31;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 22-0167,22-0361 add -->
	void CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data);
// 22-0167,22-0361 add <--
	void DiagReSetPosition(void);	// 修正No.168690 add
};
