#pragma once


// CShinJigyo31Ex ダイアログ

class CShinJigyo31Ex : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinJigyo31Ex)

public:
	CShinJigyo31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinJigyo31Ex();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_JIGYO31_ex };
	
//#endif

private:
	CWnd		*m_pParent;		// 親ウィンドウ
	CSyzUtil	m_Util;

	int			m_FcsUpEnd;		// 入力可能なセル ( 上移動での末端 )
	int			m_FcsLeftEnd;	// 入力可能なセル ( 左移動での末端 )
	int			m_FcsDownEnd;	// 入力可能なセル ( 下移動での末端 )
	int			m_FcsRightEnd;	// 入力可能なセル ( 右移動での末端 )

	int			m_Idx;
	short		m_Lin;
	short		m_Clm;

	int			m_curwnd;

	// 印刷情報
	CVPrint				*m_pVprn;			// 出力クラス
	NPRT_PAR			*m_pPset;			// 印刷情報
	PBUF				PF[30];
	SH_PRINT_CMINFO		*m_PrintCmInfo;

public:
	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	void InitDiagAttr();

	// 斜線の表示/非表示
	void InitDiagonalLines();

	// 経過措置のチェック
	int CheckSVmzsw ( int index );

	// 画面表示
	void DispList ( int mode );

	// ダイアグラムデータ表示
	void DispJigyoDiagData ( long dlin );

	// 表示判定
	int DispJudge ( int clm, int line );

	// 各入力データ取得
	int GetDiagData ( long index, short leng, DIAGRAM_DATA *pData, int mode );

	// 手入力サインのクリア
	int ClearManualInputSign();

	// 次フォーカスの場所検索
	int FocusMoveCheck ( int Index, int Move );

	// 初期ポジションセット
	virtual void SetItemPosition();
	void SetItemPositionEx();

	// ポジションの末端設定
	virtual void SetFocusRange();

	// 印刷情報のセット
	void SetPrintInfo ( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo );

	// 印刷１
	int PrintHyo1 ( int pno, int fno, CDC *pDC, char *pBp );

	// 印刷２
	int PrintHyo2 ( int pno, int fno, CDC *pDC, char *pBp );

	// 印刷３
	int PrintHyo3 ( int pno, int fno, CDC *pDC, char *pBp );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CIcsdiagctrl m_JigyoDiag;
	virtual BOOL OnInitDialog();

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 22-0167,22-0361 add -->
	void CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data);
	void ColumnControl( void );
	int GetColumnKind( int pInd );
// 22-0167,22-0361 add <--
	void DiagReSetPosition(void);	// 修正No.168690 add
};
