#pragma once


// CShinSell ダイアログ

class CShinSell : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinSell)

public:
	CShinSell(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSell();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SELL };

	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	void InitDiagAttr();
	// 画面表示
	virtual void DispList( int mode );
	// ダイアグデータ表示
	void DispSellDiagData();
	// カーソル移動
	void Diag1_Edit( short index, UINT flg, short nChar );

	// 各入力データ抽出
	int GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode );

	// 手入力サインのクリア
	int ClearManualInputSign();

	// 次フォーカスの場所検索（上下動作）'14.08.04
//	int UpDownFocusMoveCheck(int Index , int Move);
	int FocusMoveCheck(int Index , int Move);

	// 初期ポジションセット ('14.08.08)
	virtual void SetItemPosition();

	// 印刷情報のセット
	void SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo );
	// 印刷１
	int PrintHyo1( int pno, int fno, CDC *pDC, char *pBp );
	// 印刷２
	int PrintHyo2( int pno, int fno, CDC *pDC, char *pBp );
	
	// 表示判定
	int DispJudge( int clm, int line );
	// ポジションの末端設定
//2016.06.22 UPDATE START
//	void SetFocusRange();
	virtual void SetFocusRange();
//2016.06.22 UPDATE END

private:
	CWnd				*m_pParent;	// 親ウィンドウ'14.08.06
	CSyzUtil			m_Util;
	short				m_Idx;
	short				m_Lin;

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;	//'14.08.04


	//== 印刷情報 ==
	CVPrint				*m_pVprn;							// 出力クラス
	NPRT_PAR			*m_pPset;							// 印刷情報
	PBUF				PF[30];
	SH_PRINT_CMINFO		*m_PrintCmInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Sell;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data);
};
