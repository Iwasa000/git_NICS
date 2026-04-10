#pragma once
#include "icsdiagctrl.h"


// CShinFhyo5_022 ダイアログ

class CShinFhyo5_022 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo5_022)

public:
	CShinFhyo5_022(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo5_022();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY5_022 };

	// 初期情報のセット
	int InitInfo (CH31SnFhyo5_022Data **m_pSnFhyo5_022Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CSyzSyukei **pSyzSyukei);

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 初期ポジションセット
	void SetItemPosition();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int index, int Move );

	// 複数事業で強制入力不可行
	int IsProvideEditCell ( short Idx );

	// データ読み込み
	void ReadData();
	void ReadData2();

	// データ書き込み
	void WriteData();

	// 画面表示
	void Disp_f5022();

	// 金額表示
	void dsp_prs(unsigned short Id, short Index, char* Mony);

	// 金額表示クリア
	void dsp_cls(unsigned short Id, short Index);

	//== 各データ処理 ==//
	// 4×みなし仕入率
	void f5022_20data();
	// 4×みなし仕入率
	void f5022_21data();
	// 第一種事業及び第二種事業
	void f5022_22data();
	// 第一種事業及び第三種事業
	void f5022_23data();
	// 第一種事業及び第四種事業
	void f5022_24data();
	// 第一種事業及び第五種事業
	void f5022_25data();
	// 第一種事業及び第六種事業
	void f5022_26data();
	// 第二種事業及び第三種事業
	void f5022_27data();
	// 第二種事業及び第四種事業
	void f5022_28data();
	// 第二種事業及び第五種事業
	void f5022_29data();
	// 第二種事業及び第六種事業
	void f5022_30data();
	// 第三種事業及び第四種事業
	void f5022_31data();
	// 第三種事業及び第五種事業
	void f5022_32data();
	// 第三種事業及び第六種事業
	void f5022_33data();
	// 第四種事業及び第五種事業
	void f5022_34data();
	// 第四種事業及び第六種事業
	void f5022_35data();
	// 第五種事業及び第六種事業
	void f5022_36data();
	// 選択可能な計算式区分の内から選択した金額
	void f5022_37data();

	// 入力データの表示
	void Disp_f5022data(int index);

	// 各入力データ抽出
	void GetAllDiagData();

	// 集計モジュール更新
	int SyukeiDataSet ( int cID );

	// 集計モジュールからデータ取得
	int MemoryReget();

	int IsEnableItems(int reflg, short& Idx, CString& code, int& typeCnt);
	int Gokeiclctype();
	int Is4Val(int reflg);

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 表示項目？
	int IsDispArea ( short index );

	// 手入力クリア
	void ClearInput ( short index );

	// 合計欄計算
	int CalqGokei ( int index );

	// 売上割合の取得
	int GetUriageWariai ( int id, unsigned short *pWari );
	int GetUriageWariai2 ( int id, unsigned short *pWari, char *pArith );

	// 出力する控除対象仕入税額をセット
	int SetPrintArea();

	// 全項目出力
	void AllPrint();

	// 入力項目？
	int IsInputArea ( short index );

	// 付表5 手入力サイン取得
	int GetInputSign ( int id, int &inputSign );

	// 付表5-2(2)手入力サイン取得
	int GetInputSign_F522( int id, int &inputSign );

	// 特例計算取得
	int GetTokurei();

	/*// 選択された項目の控除対象仕入税額をセット
	void SetSelectCode ( int selId );

	// 控除対象仕入税額の自動集計
	void AutoKjCode();

	// 選択された項目の背景色変更
	int ChangeBackColor ( BOOL bSw );

	// 控除対象仕入税額の再セット必要
	bool IsKjCodeReset();

	// 選択中の項目取得
	int GetSelectItem ( int &getID );

	// 再集計
	void ReSyukei();*/
private:
	CWnd			*m_pParent;							// 親ウィンドウ

	char			F5022_20A[MONY_BUF_SIZE],			// 20.X 4×みなし仕入率 税率3%適用分    X
					F5022_20B[MONY_BUF_SIZE],			// 20.D 4×みなし仕入率 税率6.24%適用分 D
					F5022_20C[MONY_BUF_SIZE],			// 20.E 4×みなし仕入率 税率7.8%適用分  E
					F5022_20X[MONY_BUF_SIZE];			// 20.F 4×みなし仕入率 合計            F
	int				F5022_20f;							// 20.  原則計算 再計算スイッチ

	char			F5022_21A[MONY_BUF_SIZE],			// 21.X 4×みなし仕入率 税率3%適用分    X
					F5022_21B[MONY_BUF_SIZE],			// 21.D 4×みなし仕入率 税率6.24%適用分 D
					F5022_21C[MONY_BUF_SIZE],			// 21.E 4×みなし仕入率 税率7.8%適用分  E
					F5022_21X[MONY_BUF_SIZE];			// 21.F 4×みなし仕入率 合計            F
	int				F5022_21f;							// 21.  原則計算 再計算スイッチ
	int				F5022_21kbn;						// みなし仕入率　1:90％...
	int				F5022_21Disp;						// 21.入力許可サイン

	char			F5022_22A[MONY_BUF_SIZE],			// 22.X 第一種事業及び第二種事業 税率3%適用分    X
					F5022_22B[MONY_BUF_SIZE],			// 22.D 第一種事業及び第二種事業 税率6.24%適用分 D
					F5022_22C[MONY_BUF_SIZE],			// 22.E 第一種事業及び第二種事業 税率7.8%適用分  E
					F5022_22X[MONY_BUF_SIZE];			// 22.F 第一種事業及び第二種事業 合計            F
	int				F5022_22f;							// 22.  原則計算 再計算スイッチ
	int				F5022_22Disp;						// 22.入力許可サイン

	char			F5022_23A[MONY_BUF_SIZE],			// 23.X 第一種事業及び第三種事業 税率3%適用分    X
					F5022_23B[MONY_BUF_SIZE],			// 23.D 第一種事業及び第三種事業 税率6.24%適用分 D
					F5022_23C[MONY_BUF_SIZE],			// 23.E 第一種事業及び第三種事業 税率7.8%適用分  E
					F5022_23X[MONY_BUF_SIZE];			// 23.F 第一種事業及び第三種事業 合計            F
	int				F5022_23f;							// 23.  原則計算 再計算スイッチ
	int				F5022_23Disp;						// 23.入力許可サイン

	char			F5022_24A[MONY_BUF_SIZE],			// 24.X 第一種事業及び第四種事業 税率3%適用分    X
					F5022_24B[MONY_BUF_SIZE],			// 24.D 第一種事業及び第四種事業 税率6.24%適用分 D
					F5022_24C[MONY_BUF_SIZE],			// 24.E 第一種事業及び第四種事業 税率7.8%適用分  E
					F5022_24X[MONY_BUF_SIZE];			// 24.F 第一種事業及び第四種事業 合計            F
	int				F5022_24f;							// 24.  原則計算 再計算スイッチ
	int				F5022_24Disp;						// 24.入力許可サイン

	char			F5022_25A[MONY_BUF_SIZE],			// 25.X 第一種事業及び第五種事業 税率3%適用分    X
					F5022_25B[MONY_BUF_SIZE],			// 25.D 第一種事業及び第五種事業 税率6.24%適用分 D
					F5022_25C[MONY_BUF_SIZE],			// 25.E 第一種事業及び第五種事業 税率7.8%適用分  E
					F5022_25X[MONY_BUF_SIZE];			// 25.F 第一種事業及び第五種事業 合計            F
	int				F5022_25f;							// 25.  原則計算 再計算スイッチ
	int				F5022_25Disp;						// 25.入力許可サイン

	char			F5022_26A[MONY_BUF_SIZE],			// 26.X 第一種事業及び第六種事業 税率3%適用分    X
					F5022_26B[MONY_BUF_SIZE],			// 26.D 第一種事業及び第六種事業 税率6.24%適用分 D
					F5022_26C[MONY_BUF_SIZE],			// 26.E 第一種事業及び第六種事業 税率7.8%適用分  E
					F5022_26X[MONY_BUF_SIZE];			// 26.F 第一種事業及び第六種事業 合計            F
	int				F5022_26f;							// 26.  原則計算 再計算スイッチ
	int				F5022_26Disp;						// 26.入力許可サイン

	char			F5022_27A[MONY_BUF_SIZE],			// 27.X 第二種事業及び第三種事業 税率3%適用分    X
					F5022_27B[MONY_BUF_SIZE],			// 27.D 第二種事業及び第三種事業 税率6.24%適用分 D
					F5022_27C[MONY_BUF_SIZE],			// 27.E 第二種事業及び第三種事業 税率7.8%適用分  E
					F5022_27X[MONY_BUF_SIZE];			// 27.F 第二種事業及び第三種事業 合計            F
	int				F5022_27f;							// 27.  原則計算 再計算スイッチ
	int				F5022_27Disp;						// 27.入力許可サイン

	char			F5022_28A[MONY_BUF_SIZE],			// 28.X 第二種事業及び第四種事業 税率3%適用分    X
					F5022_28B[MONY_BUF_SIZE],			// 28.D 第二種事業及び第四種事業 税率6.24%適用分 D
					F5022_28C[MONY_BUF_SIZE],			// 28.E 第二種事業及び第四種事業 税率7.8%適用分  E
					F5022_28X[MONY_BUF_SIZE];			// 28.F 第二種事業及び第四種事業 合計            F
	int				F5022_28f;							// 28.  原則計算 再計算スイッチ
	int				F5022_28Disp;						// 28.入力許可サイン

	char			F5022_29A[MONY_BUF_SIZE],			// 29.X 第二種事業及び第五種事業 税率3%適用分    X
					F5022_29B[MONY_BUF_SIZE],			// 29.D 第二種事業及び第五種事業 税率6.24%適用分 D
					F5022_29C[MONY_BUF_SIZE],			// 29.E 第二種事業及び第五種事業 税率7.8%適用分  E
					F5022_29X[MONY_BUF_SIZE];			// 29.F 第二種事業及び第五種事業 合計            F
	int				F5022_29f;							// 29.  原則計算 再計算スイッチ
	int				F5022_29Disp;						// 29.入力許可サイン

	char			F5022_30A[MONY_BUF_SIZE],			// 30.X 第二種事業及び第六種事業 税率3%適用分    X
					F5022_30B[MONY_BUF_SIZE],			// 30.D 第二種事業及び第六種事業 税率6.24%適用分 D
					F5022_30C[MONY_BUF_SIZE],			// 30.E 第二種事業及び第六種事業 税率7.8%適用分  E
					F5022_30X[MONY_BUF_SIZE];			// 30.F 第二種事業及び第六種事業 合計            F
	int				F5022_30f;							// 30.  原則計算 再計算スイッチ
	int				F5022_30Disp;						// 30.入力許可サイン

	char			F5022_31A[MONY_BUF_SIZE],			// 31.X 第三種事業及び第四種事業 税率3%適用分    X
					F5022_31B[MONY_BUF_SIZE],			// 31.D 第三種事業及び第四種事業 税率6.24%適用分 D
					F5022_31C[MONY_BUF_SIZE],			// 31.E 第三種事業及び第四種事業 税率7.8%適用分  E
					F5022_31X[MONY_BUF_SIZE];			// 31.F 第三種事業及び第四種事業 合計            F
	int				F5022_31f;							// 31.  原則計算 再計算スイッチ
	int				F5022_31Disp;						// 31.入力許可サイン

	char			F5022_32A[MONY_BUF_SIZE],			// 32.X 第三種事業及び第五種事業 税率3%適用分    X
					F5022_32B[MONY_BUF_SIZE],			// 32.D 第三種事業及び第五種事業 税率6.24%適用分 D
					F5022_32C[MONY_BUF_SIZE],			// 32.E 第三種事業及び第五種事業 税率7.8%適用分  E
					F5022_32X[MONY_BUF_SIZE];			// 32.F 第三種事業及び第五種事業 合計            F
	int				F5022_32f;							// 32.  原則計算 再計算スイッチ
	int				F5022_32Disp;						// 32.入力許可サイン

	char			F5022_33A[MONY_BUF_SIZE],			// 33.X 第三種事業及び第六種事業 税率3%適用分    X
					F5022_33B[MONY_BUF_SIZE],			// 33.D 第三種事業及び第六種事業 税率6.24%適用分 D
					F5022_33C[MONY_BUF_SIZE],			// 33.E 第三種事業及び第六種事業 税率7.8%適用分  E
					F5022_33X[MONY_BUF_SIZE];			// 33.F 第三種事業及び第六種事業 合計            F
	int				F5022_33f;							// 33.  原則計算 再計算スイッチ
	int				F5022_33Disp;						// 33.入力許可サイン

	char			F5022_34A[MONY_BUF_SIZE],			// 34.X 第四種事業及び第五種事業 税率3%適用分    X
					F5022_34B[MONY_BUF_SIZE],			// 34.D 第四種事業及び第五種事業 税率6.24%適用分 D
					F5022_34C[MONY_BUF_SIZE],			// 34.E 第四種事業及び第五種事業 税率7.8%適用分  E
					F5022_34X[MONY_BUF_SIZE];			// 34.F 第四種事業及び第五種事業 合計            F
	int				F5022_34f;							// 34.  原則計算 再計算スイッチ
	int				F5022_34Disp;						// 34.入力許可サイン

	char			F5022_35A[MONY_BUF_SIZE],			// 35.X 第四種事業及び第六種事業 税率3%適用分    X
					F5022_35B[MONY_BUF_SIZE],			// 35.D 第四種事業及び第六種事業 税率6.24%適用分 D
					F5022_35C[MONY_BUF_SIZE],			// 35.E 第四種事業及び第六種事業 税率7.8%適用分  E
					F5022_35X[MONY_BUF_SIZE];			// 35.F 第四種事業及び第六種事業 合計            F
	int				F5022_35f;							// 35.  原則計算 再計算スイッチ
	int				F5022_35Disp;						// 35.入力許可サイン

	char			F5022_36A[MONY_BUF_SIZE],			// 36.X 第五種事業及び第六種事業 税率3%適用分    X
					F5022_36B[MONY_BUF_SIZE],			// 36.D 第五種事業及び第六種事業 税率6.24%適用分 D
					F5022_36C[MONY_BUF_SIZE],			// 36.E 第五種事業及び第六種事業 税率7.8%適用分  E
					F5022_36X[MONY_BUF_SIZE];			// 36.F 第五種事業及び第六種事業 合計            F
	int				F5022_36f;							// 36.  原則計算 再計算スイッチ
	int				F5022_36Disp;						// 36.入力許可サイン

	char			F5022_37A[MONY_BUF_SIZE],			// 37.X 選択可能な計算式区分の内から選択した金額 税率3%適用分    X
					F5022_37B[MONY_BUF_SIZE],			// 37.D 選択可能な計算式区分の内から選択した金額 税率6.24%適用分 D
					F5022_37C[MONY_BUF_SIZE],			// 37.E 選択可能な計算式区分の内から選択した金額 税率7.8%適用分  E
					F5022_37X[MONY_BUF_SIZE];			// 37.F 選択可能な計算式区分の内から選択した金額 合計            F
	int				F5022_37f;							// 37.  原則計算 再計算スイッチ


	int				m_curwnd;

	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報
	CH31SnFhyo5_022Data	**m_pSnFhyo5_022Data;
	CSyzSyukei			**m_pSyzSyukei;

	CArithEx			*m_pArith;


	CString			m_code;								// 有利な計算式の集計コード
	short			m_id;								// 有利な計算式のコントロールＩＤ
	short			m_PrintId;							// 有隣計算式の出力用ID
	int				m_typecnt;							// 使用業種数
	int				m_flg;								// D0:F列は別計算
	int				m_4val;								// 0:控除対象仕入税額計算の為の消費税額なし

	MoneyBasejagArray		m_money35;					// 350000番台の集計データ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo5_022Diag01;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
