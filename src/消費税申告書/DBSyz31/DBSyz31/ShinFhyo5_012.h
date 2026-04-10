#pragma once
#include "icsdiagctrl.h"


// CShinFhyo5_012 ダイアログ

class CShinFhyo5_012 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo5_012)

public:
	CShinFhyo5_012(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo5_012();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY5_012 };

	// 初期情報のセット
	int InitInfo (CH31SnFhyo5_012Data **pSnFhyo5_012Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith);

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

	// データ書き込み
	void WriteData();

	// 画面表示
	void Disp_f5012();

	// 金額表示
	void dsp_prs(unsigned short Id, short Index, char* Mony);

	// 金額表示クリア
	void dsp_cls(unsigned short Id, short Index);


	//== 各データ処理 ==//
	// 4×みなし仕入率
	void f5012_20data();
	// 4×みなし仕入率
	void f5012_21data();
	// 第一種事業及び第二種事業
	void f5012_22data();
	// 第一種事業及び第三種事業
	void f5012_23data();
	// 第一種事業及び第四種事業
	void f5012_24data();
	// 第一種事業及び第五種事業
	void f5012_25data();
	// 第一種事業及び第六種事業
	void f5012_26data();
	// 第二種事業及び第三種事業
	void f5012_27data();
	// 第二種事業及び第四種事業
	void f5012_28data();
	// 第二種事業及び第五種事業
	void f5012_29data();
	// 第二種事業及び第六種事業
	void f5012_30data();
	// 第三種事業及び第四種事業
	void f5012_31data();
	// 第三種事業及び第五種事業
	void f5012_32data();
	// 第三種事業及び第六種事業
	void f5012_33data();
	// 第四種事業及び第五種事業
	void f5012_34data();
	// 第四種事業及び第六種事業
	void f5012_35data();
	// 第五種事業及び第六種事業
	void f5012_36data();
	// 選択可能な計算式区分の内から選択した金額
	void f5012_37data();

	// 入力データの表示
	void Disp_f5012data( int index );

	// 各入力データ抽出
	void GetAllDiagData();

	// 集計モジュール関連
	void ReadData2();
	int SyukeiDataSet( int cID );
	int MemoryReget();

	int IsEnableItems(int reflg, short& Idx, CString& code, int& typeCnt);
	int Gokeiclctype();
	int Is4Val(int reflg);

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 表示項目
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

	// 付表5-1(2)手入力サイン
	int GetInputSign_F512( int id, int &inputSign );

	// 特例計算取得
	int GetTokurei();

	// 選択された項目の控除対象仕入税額をセット
	/*void SetSelectCode ( int selId );

	// 控除対象仕入税額の自動集計
	void AutoKjCode();

	// 選択された項目を控除対象仕入税額に反映
	void SetKjCode();

	// 選択された項目の背景色変更
	int ChangeBackColor ( BOOL bSw );

	// 控除対象仕入税額の再セット
	void ResetKjCode();

	// 控除対象仕入税額の再セット必要
	bool IsKjCodeReset();

	// 選択中の項目取得
	int GetSelectItem ( int &getID );

	// 再集計
	void ReSyukei();*/
private:
	CWnd			*m_pParent;							// 親ウィンドウ

	char			F5012_20X[MONY_BUF_SIZE],			// 20.X 4×みなし仕入率 旧税率分小計    X
					F5012_20D[MONY_BUF_SIZE],			// 20.D 4×みなし仕入率 税率6.24%適用分 D
					F5012_20E[MONY_BUF_SIZE],			// 20.E 4×みなし仕入率 税率7.8%適用分  E
					F5012_20F[MONY_BUF_SIZE];			// 20.F 4×みなし仕入率 合計            F
	int				F5012_20f;							// 20.  手入力サイン

	char			F5012_21X[MONY_BUF_SIZE],			// 21.X 4×みなし仕入率 旧税率分小計    X
					F5012_21D[MONY_BUF_SIZE],			// 21.D 4×みなし仕入率 税率6.24%適用分 D
					F5012_21E[MONY_BUF_SIZE],			// 21.E 4×みなし仕入率 税率7.8%適用分  E
					F5012_21F[MONY_BUF_SIZE];			// 21.F 4×みなし仕入率 合計            F
	int				F5012_21f;							// 21.  手入力サイン
	int				F5012_21kbn;						// みなし仕入率　1:90％...
	int				F5012_21Disp;						// 21.入力許可サイン

	char			F5012_22X[MONY_BUF_SIZE],			// 22.X 第一種事業及び第二種事業 旧税率分小計    X
					F5012_22D[MONY_BUF_SIZE],			// 22.D 第一種事業及び第二種事業 税率6.24%適用分 D
					F5012_22E[MONY_BUF_SIZE],			// 22.E 第一種事業及び第二種事業 税率7.8%適用分  E
					F5012_22F[MONY_BUF_SIZE];			// 22.F 第一種事業及び第二種事業 合計            F
	int				F5012_22f;							// 22.  手入力サイン
	int				F5012_22Disp;						// 22.入力許可サイン

	char			F5012_23X[MONY_BUF_SIZE],			// 23.X 第一種事業及び第三種事業 旧税率分小計    X
					F5012_23D[MONY_BUF_SIZE],			// 23.D 第一種事業及び第三種事業 税率6.24%適用分 D
					F5012_23E[MONY_BUF_SIZE],			// 23.E 第一種事業及び第三種事業 税率7.8%適用分  E
					F5012_23F[MONY_BUF_SIZE];			// 23.F 第一種事業及び第三種事業 合計            F
	int				F5012_23f;							// 23.  手入力サイン
	int				F5012_23Disp;						// 23.入力許可サイン

	char			F5012_24X[MONY_BUF_SIZE],			// 24.X 第一種事業及び第四種事業 旧税率分小計    X
					F5012_24D[MONY_BUF_SIZE],			// 24.D 第一種事業及び第四種事業 税率6.24%適用分 D
					F5012_24E[MONY_BUF_SIZE],			// 24.E 第一種事業及び第四種事業 税率7.8%適用分  E
					F5012_24F[MONY_BUF_SIZE];			// 24.F 第一種事業及び第四種事業 合計            F
	int				F5012_24f;							// 24.  手入力サイン
	int				F5012_24Disp;						// 24.入力許可サイン

	char			F5012_25X[MONY_BUF_SIZE],			// 25.X 第一種事業及び第五種事業 旧税率分小計    X
					F5012_25D[MONY_BUF_SIZE],			// 25.D 第一種事業及び第五種事業 税率6.24%適用分 D
					F5012_25E[MONY_BUF_SIZE],			// 25.E 第一種事業及び第五種事業 税率7.8%適用分  E
					F5012_25F[MONY_BUF_SIZE];			// 25.F 第一種事業及び第五種事業 合計            F
	int				F5012_25f;							// 25.  手入力サイン
	int				F5012_25Disp;						// 25.入力許可サイン

	char			F5012_26X[MONY_BUF_SIZE],			// 26.X 第一種事業及び第六種事業 旧税率分小計    X
					F5012_26D[MONY_BUF_SIZE],			// 26.D 第一種事業及び第六種事業 税率6.24%適用分 D
					F5012_26E[MONY_BUF_SIZE],			// 26.E 第一種事業及び第六種事業 税率7.8%適用分  E
					F5012_26F[MONY_BUF_SIZE];			// 26.F 第一種事業及び第六種事業 合計            F
	int				F5012_26f;							// 26.  手入力サイン
	int				F5012_26Disp;						// 26.入力許可サイン

	char			F5012_27X[MONY_BUF_SIZE],			// 27.X 第二種事業及び第三種事業 旧税率分小計    X
					F5012_27D[MONY_BUF_SIZE],			// 27.D 第二種事業及び第三種事業 税率6.24%適用分 D
					F5012_27E[MONY_BUF_SIZE],			// 27.E 第二種事業及び第三種事業 税率7.8%適用分  E
					F5012_27F[MONY_BUF_SIZE];			// 27.F 第二種事業及び第三種事業 合計            F
	int				F5012_27f;							// 27.  手入力サイン
	int				F5012_27Disp;						// 27.入力許可サイン

	char			F5012_28X[MONY_BUF_SIZE],			// 28.X 第二種事業及び第四種事業 旧税率分小計    X
					F5012_28D[MONY_BUF_SIZE],			// 28.D 第二種事業及び第四種事業 税率6.24%適用分 D
					F5012_28E[MONY_BUF_SIZE],			// 28.E 第二種事業及び第四種事業 税率7.8%適用分  E
					F5012_28F[MONY_BUF_SIZE];			// 28.F 第二種事業及び第四種事業 合計            F
	int				F5012_28f;							// 28.  手入力サイン
	int				F5012_28Disp;						// 28.入力許可サイン

	char			F5012_29X[MONY_BUF_SIZE],			// 29.X 第二種事業及び第五種事業 旧税率分小計    X
					F5012_29D[MONY_BUF_SIZE],			// 29.D 第二種事業及び第五種事業 税率6.24%適用分 D
					F5012_29E[MONY_BUF_SIZE],			// 29.E 第二種事業及び第五種事業 税率7.8%適用分  E
					F5012_29F[MONY_BUF_SIZE];			// 29.F 第二種事業及び第五種事業 合計            F
	int				F5012_29f;							// 29.  手入力サイン
	int				F5012_29Disp;						// 29.入力許可サイン

	char			F5012_30X[MONY_BUF_SIZE],			// 30.X 第二種事業及び第六種事業 旧税率分小計    X
					F5012_30D[MONY_BUF_SIZE],			// 30.D 第二種事業及び第六種事業 税率6.24%適用分 D
					F5012_30E[MONY_BUF_SIZE],			// 30.E 第二種事業及び第六種事業 税率7.8%適用分  E
					F5012_30F[MONY_BUF_SIZE];			// 30.F 第二種事業及び第六種事業 合計            F
	int				F5012_30f;							// 30.  手入力サイン
	int				F5012_30Disp;						// 30.入力許可サイン

	char			F5012_31X[MONY_BUF_SIZE],			// 31.X 第三種事業及び第四種事業 旧税率分小計    X
					F5012_31D[MONY_BUF_SIZE],			// 31.D 第三種事業及び第四種事業 税率6.24%適用分 D
					F5012_31E[MONY_BUF_SIZE],			// 31.E 第三種事業及び第四種事業 税率7.8%適用分  E
					F5012_31F[MONY_BUF_SIZE];			// 31.F 第三種事業及び第四種事業 合計            F
	int				F5012_31f;							// 31.  手入力サイン
	int				F5012_31Disp;						// 31.入力許可サイン

	char			F5012_32X[MONY_BUF_SIZE],			// 32.X 第三種事業及び第五種事業 旧税率分小計    X
					F5012_32D[MONY_BUF_SIZE],			// 32.D 第三種事業及び第五種事業 税率6.24%適用分 D
					F5012_32E[MONY_BUF_SIZE],			// 32.E 第三種事業及び第五種事業 税率7.8%適用分  E
					F5012_32F[MONY_BUF_SIZE];			// 32.F 第三種事業及び第五種事業 合計            F
	int				F5012_32f;							// 32.  手入力サイン
	int				F5012_32Disp;						// 32.入力許可サイン

	char			F5012_33X[MONY_BUF_SIZE],			// 33.X 第三種事業及び第六種事業 旧税率分小計    X
					F5012_33D[MONY_BUF_SIZE],			// 33.D 第三種事業及び第六種事業 税率6.24%適用分 D
					F5012_33E[MONY_BUF_SIZE],			// 33.E 第三種事業及び第六種事業 税率7.8%適用分  E
					F5012_33F[MONY_BUF_SIZE];			// 33.F 第三種事業及び第六種事業 合計            F
	int				F5012_33f;							// 33.  手入力サイン
	int				F5012_33Disp;						// 33.入力許可サイン

	char			F5012_34X[MONY_BUF_SIZE],			// 34.X 第四種事業及び第五種事業 旧税率分小計    X
					F5012_34D[MONY_BUF_SIZE],			// 34.D 第四種事業及び第五種事業 税率6.24%適用分 D
					F5012_34E[MONY_BUF_SIZE],			// 34.E 第四種事業及び第五種事業 税率7.8%適用分  E
					F5012_34F[MONY_BUF_SIZE];			// 34.F 第四種事業及び第五種事業 合計            F
	int				F5012_34f;							// 34.  手入力サイン
	int				F5012_34Disp;						// 34.入力許可サイン

	char			F5012_35X[MONY_BUF_SIZE],			// 35.X 第四種事業及び第六種事業 旧税率分小計    X
					F5012_35D[MONY_BUF_SIZE],			// 35.D 第四種事業及び第六種事業 税率6.24%適用分 D
					F5012_35E[MONY_BUF_SIZE],			// 35.E 第四種事業及び第六種事業 税率7.8%適用分  E
					F5012_35F[MONY_BUF_SIZE];			// 35.F 第四種事業及び第六種事業 合計            F
	int				F5012_35f;							// 35.  手入力サイン
	int				F5012_35Disp;						// 35.入力許可サイン

	char			F5012_36X[MONY_BUF_SIZE],			// 36.X 第五種事業及び第六種事業 旧税率分小計    X
					F5012_36D[MONY_BUF_SIZE],			// 36.D 第五種事業及び第六種事業 税率6.24%適用分 D
					F5012_36E[MONY_BUF_SIZE],			// 36.E 第五種事業及び第六種事業 税率7.8%適用分  E
					F5012_36F[MONY_BUF_SIZE];			// 36.F 第五種事業及び第六種事業 合計            F
	int				F5012_36f;							// 36.  手入力サイン
	int				F5012_36Disp;						// 36.入力許可サイン

	char			F5012_37X[MONY_BUF_SIZE],			// 37.X 選択可能な計算式区分の内から選択した金額 旧税率分小計    X
					F5012_37D[MONY_BUF_SIZE],			// 37.D 選択可能な計算式区分の内から選択した金額 税率6.24%適用分 D
					F5012_37E[MONY_BUF_SIZE],			// 37.E 選択可能な計算式区分の内から選択した金額 税率7.8%適用分  E
					F5012_37F[MONY_BUF_SIZE];			// 37.F 選択可能な計算式区分の内から選択した金額 合計            F

	int				m_curwnd;



	CSnHeadData				**m_pSnHeadData;			// 消費税ヘッダ情報
	CSyzSyukei				**m_pSyzSyukei;				// 集計モジュール
	CH31SnFhyo5_012Data		**m_pSnFhyo5_012Data;		// 付表5-12データクラス

	CArithEx				*m_pArith;

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
	CIcsdiagctrl m_Fhyo5_012Diag01;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
