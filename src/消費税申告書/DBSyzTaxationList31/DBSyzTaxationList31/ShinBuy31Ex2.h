#pragma once


// CShinBuy31Ex2 ダイアログ

class CShinBuy31Ex2 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinBuy31Ex2)

public:
	CShinBuy31Ex2(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CShinBuy31Ex2();

// ダイアログ データ

	enum { IDD = IDD_DIALOG_BUY31_ex2 };

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	void InitDiagAttr();

// 22-0167,22-0361 del -->
	//// 追加文言の表示/非表示
	//void InitAddText();
// 22-0167,22-0361 del <--

	// 画面表示
	virtual void DispList(int mode);

	// ダイアグデータ表示
	void DispBuyDiagData();

	// 各入力データ抽出
	int GetDiagData(long index, short leng, DIAGRAM_DATA *data, int mode);

	// 手入力サインのクリア
	int ClearManualInputSign();

	//フォーカス移動処理（上下）Move = 0 : 下方向	Move = 1: 上方向
	int FocusMoveCheck(int Index, int Move);

	// 初期ポジションセット
	virtual void SetItemPosition();
	void SetItemPositionEx();

	// 入力項目色変え
	void ChangeColor(unsigned short id, short Index, int sign);

	// 表示判定
	int DispJudge(int clm, int line);

	// ポジションの末端設定
	virtual void SetFocusRange();

	// 印刷情報のセット
	void SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo );
	// 印刷１
	int PrintHyo1( int pno, int fno, CDC *pDC, char *pBp );
	// 印刷２
	int PrintHyo2( int pno, int fno, CDC *pDC, char *pBp );
	// 印刷３
	int PrintHyo3( int pno, int fno, CDC *pDC, char *pBp );


private:
	CWnd				*m_pParent;	// 親ウィンドウ
	CSyzUtil			m_Util;		// ユーティリティクラス
	int					m_Idx;		// ???
	int					m_Lin;		// ???

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;

	const static int	clmSize = 5;	//帳票の列数(科目列抜き)

	//== 印刷情報 ==
	CVPrint				*m_pVprn;							// 出力クラス
	NPRT_PAR			*m_pPset;							// 印刷情報
	PBUF				PF[30];
	SH_PRINT_CMINFO		*m_PrintCmInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	CIcsdiagctrl m_BuyDiag;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
// 22-0167,22-0361 add -->
	void CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data);
	int GetColumNum( int index );
// 22-0167,22-0361 add <--

	void DiagReSetPosition(void);	// 修正No.168690 add 
};
