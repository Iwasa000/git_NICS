#pragma once


// CShinKniinp31 ダイアログ

class CShinKniinp31 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinKniinp31)

public:
	CShinKniinp31(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinKniinp31();

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	void InitDiagAttr();

	// 画面表示
	virtual void DispList( int mode );

	// ダイアグデータ表示
	void DispKniinpKeikaDiagData( long dlin );

	// 各入力データ抽出
	int GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode );

	// 手入力サインのクリア
	int ClearManualInputSign();

	// 次フォーカスの場所検索（上下動作）
	int FocusMoveCheck(int Index , int Move);

	// 初期ポジションセット'14.09.01
	virtual void SetItemPosition();

	// 表示判定
	int DispJudge( int clm, int line );

	// ポジションの末端設定
	virtual void SetFocusRange();

	// 背景色変更
	void ChangeColor( unsigned short id, short index, int sign );

	// IDXよりカラムポジションを返送 
	int GetIndexToClmPosition(int index);

	void DiagReSetPosition(void);	// 修正No.168690 add

private:
	CWnd				*m_pParent;	// 親ウィンドウ

	CSyzUtil			m_Util;		// ユーティリティクラス
	int					m_Idx;		// ???
	int					m_Lin;		// ???
	short				m_Clm;

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_JIGYO_SELL31 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CIcsdiagctrl m_Kniinp31;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
};
