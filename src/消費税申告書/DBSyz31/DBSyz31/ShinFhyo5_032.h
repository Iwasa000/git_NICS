#pragma once
#include "icsdiagctrl.h"


// ShinFhyo5_032 ダイアログ

class CShinFhyo5_032 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo5_032)

public:
	CShinFhyo5_032(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo5_032();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY5_032 };
//#endif

public:
	// 初期情報のセット
	int InitInfo ( CH31SnFhyo5_012Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith );

	// 初期化
	int Init();

	// データ読込み
	int ReadData();

	// 読込みデータを集計モジュールへ
	void ReadData2();

	// データ書込み
	void WriteData();

	// 集計処理
	int MemoryReget();

	// 集計モジュール更新
	int SyukeiDataSet ( int cID );

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// ポジションセット
	void SetItemPosition();

	// 画面出力
	void DispFh5_032();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 全項目表示
	void f5032data_all ( int atno );
	// 4×みなし仕入率
	void f5032data_20();
	// 4×みなし仕入率
	void f5032data_21();
	// 第一種事業及び第二種事業
	void f5032data_22();
	// 第一種事業及び第三種事業
	void f5032data_23();
	// 第一種事業及び第四種事業
	void f5032data_24();
	// 第一種事業及び第五種事業
	void f5032data_25();
	// 第一種事業及び第六種事業
	void f5032data_26();
	// 第二種事業及び第三種事業
	void f5032data_27();
	// 第二種事業及び第四種事業
	void f5032data_28();
	// 第二種事業及び第五種事業
	void f5032data_29();
	// 第二種事業及び第六種事業
	void f5032data_30();
	// 第三種事業及び第四種事業
	void f5032data_31();
	// 第三種事業及び第五種事業
	void f5032data_32();
	// 第三種事業及び第六種事業
	void f5032data_33();
	// 第四種事業及び第五種事業
	void f5032data_34();
	// 第四種事業及び第六種事業
	void f5032data_35();
	// 第五種事業及び第六種事業
	void f5032data_36();
	// 選択可能な計算式区分の内から選択した金額
	void f5032data_37();

	// 有利な計算式を判別
	int IsEnableItems (int reflg, short& Idx, CString& code, int& typeCnt );
	// 合計欄の計算式を判別
	int Gokeiclctype();
	// 控除対象仕入税額計算の為の消費税額判定
	int Is4Val ( int reflg );

	// 表示金額取得
	void GetAllDiagData();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int index, int Move );

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 表示項目？
	int IsDispArea ( short index );

	// 合計欄計算
	int CalqGokei ( int index );

	// 売上割合取得
	int GetUriageWariai ( int id, unsigned short *pWari );
	int GetUriageWariai2 ( int id, unsigned short *pWari, char *pArith );

	// 手入力クリア
	void ClearInput ( short index );

	// 出力する控除対象仕入税額をセット
	int SetPrintArea();

	// 全項目出力
	void AllPrint();

	// 入力項目？
	int IsInputArea ( short index );

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
	CWnd					*m_pParent;							// 親ウィンドウ

	char					F5032_20A[MONY_BUF_SIZE],			// 20.A 4×みなし仕入率 税率6.24%適用分 A
							F5032_20B[MONY_BUF_SIZE],			// 20.B 4×みなし仕入率 税率7.8%適用分  B
							F5032_20C[MONY_BUF_SIZE];			// 20.C 4×みなし仕入率 合計            C
	int						F5032_20c;							// 20.  手入力サイン

	char					F5032_21A[MONY_BUF_SIZE],			// 21.A 4×みなし仕入率 税率6.24%適用分 A
							F5032_21B[MONY_BUF_SIZE],			// 21.B 4×みなし仕入率 税率7.8%適用分  B
							F5032_21C[MONY_BUF_SIZE];			// 21.C 4×みなし仕入率 合計            C
	int						F5032_21c;							// 21.  手入力サイン
	int						F5032_21kbn;						// みなし仕入率　1:90％...
	int						F5032_21Disp;						// 21.入力許可サイン

	char					F5032_22A[MONY_BUF_SIZE],			// 22.A 第一種事業及び第二種事業 税率6.24%適用分 A
							F5032_22B[MONY_BUF_SIZE],			// 22.B 第一種事業及び第二種事業 税率7.8%適用分  B
							F5032_22C[MONY_BUF_SIZE];			// 22.C 第一種事業及び第二種事業 合計            C
	int						F5032_22c;							// 22.  手入力サイン
	int						F5032_22Disp;						// 22.入力許可サイン

	char					F5032_23A[MONY_BUF_SIZE],			// 23.A 第一種事業及び第三種事業 税率6.24%適用分 A
							F5032_23B[MONY_BUF_SIZE],			// 23.B 第一種事業及び第三種事業 税率7.8%適用分  B
							F5032_23C[MONY_BUF_SIZE];			// 23.C 第一種事業及び第三種事業 合計            C
	int						F5032_23c;							// 23.  手入力サイン
	int						F5032_23Disp;						// 23.入力許可サイン

	char					F5032_24A[MONY_BUF_SIZE],			// 24.A 第一種事業及び第四種事業 税率6.24%適用分 A
							F5032_24B[MONY_BUF_SIZE],			// 24.B 第一種事業及び第四種事業 税率7.8%適用分  B
							F5032_24C[MONY_BUF_SIZE];			// 24.C 第一種事業及び第四種事業 合計            C
	int						F5032_24c;							// 24.  手入力サイン
	int						F5032_24Disp;						// 24.入力許可サイン

	char					F5032_25A[MONY_BUF_SIZE],			// 25.A 第一種事業及び第五種事業 税率6.24%適用分 A
							F5032_25B[MONY_BUF_SIZE],			// 25.B 第一種事業及び第五種事業 税率7.8%適用分  B
							F5032_25C[MONY_BUF_SIZE];			// 25.C 第一種事業及び第五種事業 合計            C
	int						F5032_25c;							// 25.  手入力サイン
	int						F5032_25Disp;						// 25.入力許可サイン

	char					F5032_26A[MONY_BUF_SIZE],			// 26.A 第一種事業及び第六種事業 税率6.24%適用分 A
							F5032_26B[MONY_BUF_SIZE],			// 26.B 第一種事業及び第六種事業 税率7.8%適用分  B
							F5032_26C[MONY_BUF_SIZE];			// 26.C 第一種事業及び第六種事業 合計            C
	int						F5032_26c;							// 26.  手入力サイン
	int						F5032_26Disp;						// 26.入力許可サイン

	char					F5032_27A[MONY_BUF_SIZE],			// 27.A 第二種事業及び第三種事業 税率6.24%適用分 A
							F5032_27B[MONY_BUF_SIZE],			// 27.B 第二種事業及び第三種事業 税率7.8%適用分  B
							F5032_27C[MONY_BUF_SIZE];			// 27.C 第二種事業及び第三種事業 合計            C
	int						F5032_27c;							// 27.  手入力サイン
	int						F5032_27Disp;						// 27.入力許可サイン

	char					F5032_28A[MONY_BUF_SIZE],			// 28.A 第二種事業及び第四種事業 税率6.24%適用分 A
							F5032_28B[MONY_BUF_SIZE],			// 28.B 第二種事業及び第四種事業 税率7.8%適用分  B
							F5032_28C[MONY_BUF_SIZE];			// 28.C 第二種事業及び第四種事業 合計            C
	int						F5032_28c;							// 28.  手入力サイン
	int						F5032_28Disp;						// 28.入力許可サイン

	char					F5032_29A[MONY_BUF_SIZE],			// 29.A 第二種事業及び第五種事業 税率6.24%適用分 A
							F5032_29B[MONY_BUF_SIZE],			// 29.B 第二種事業及び第五種事業 税率7.8%適用分  B
							F5032_29C[MONY_BUF_SIZE];			// 29.C 第二種事業及び第五種事業 合計            C
	int						F5032_29c;							// 29.  手入力サイン
	int						F5032_29Disp;						// 29.入力許可サイン

	char					F5032_30A[MONY_BUF_SIZE],			// 30.A 第二種事業及び第六種事業 税率6.24%適用分 A
							F5032_30B[MONY_BUF_SIZE],			// 30.B 第二種事業及び第六種事業 税率7.8%適用分  B
							F5032_30C[MONY_BUF_SIZE];			// 30.C 第二種事業及び第六種事業 合計            C
	int						F5032_30c;							// 30.  手入力サイン
	int						F5032_30Disp;						// 30.入力許可サイン

	char					F5032_31A[MONY_BUF_SIZE],			// 31.A 第三種事業及び第四種事業 税率6.24%適用分 A
							F5032_31B[MONY_BUF_SIZE],			// 31.B 第三種事業及び第四種事業 税率7.8%適用分  B
							F5032_31C[MONY_BUF_SIZE];			// 31.C 第三種事業及び第四種事業 合計            C
	int						F5032_31c;							// 31.  手入力サイン
	int						F5032_31Disp;						// 31.入力許可サイン

	char					F5032_32A[MONY_BUF_SIZE],			// 32.A 第三種事業及び第五種事業 税率6.24%適用分 A
							F5032_32B[MONY_BUF_SIZE],			// 32.B 第三種事業及び第五種事業 税率7.8%適用分  B
							F5032_32C[MONY_BUF_SIZE];			// 32.C 第三種事業及び第五種事業 合計            C
	int						F5032_32c;							// 32.  手入力サイン
	int						F5032_32Disp;						// 32.入力許可サイン

	char					F5032_33A[MONY_BUF_SIZE],			// 33.A 第三種事業及び第六種事業 税率6.24%適用分 A
							F5032_33B[MONY_BUF_SIZE],			// 33.B 第三種事業及び第六種事業 税率7.8%適用分  B
							F5032_33C[MONY_BUF_SIZE];			// 33.C 第三種事業及び第六種事業 合計            C
	int						F5032_33c;							// 33.  手入力サイン
	int						F5032_33Disp;						// 33.入力許可サイン

	char					F5032_34A[MONY_BUF_SIZE],			// 34.A 第四種事業及び第五種事業 税率6.24%適用分 A
							F5032_34B[MONY_BUF_SIZE],			// 34.B 第四種事業及び第五種事業 税率7.8%適用分  B
							F5032_34C[MONY_BUF_SIZE];			// 34.C 第四種事業及び第五種事業 合計            C
	int						F5032_34c;							// 34.  手入力サイン
	int						F5032_34Disp;						// 34.入力許可サイン

	char					F5032_35A[MONY_BUF_SIZE],			// 35.A 第四種事業及び第六種事業 税率6.24%適用分 A
							F5032_35B[MONY_BUF_SIZE],			// 35.B 第四種事業及び第六種事業 税率7.8%適用分  B
							F5032_35C[MONY_BUF_SIZE];			// 35.C 第四種事業及び第六種事業 合計            C
	int						F5032_35c;							// 35.  手入力サイン
	int						F5032_35Disp;						// 35.入力許可サイン

	char					F5032_36A[MONY_BUF_SIZE],			// 36.A 第五種事業及び第六種事業 税率6.24%適用分 A
							F5032_36B[MONY_BUF_SIZE],			// 36.B 第五種事業及び第六種事業 税率7.8%適用分  B
							F5032_36C[MONY_BUF_SIZE];			// 36.C 第五種事業及び第六種事業 合計            C
	int						F5032_36c;							// 36.  手入力サイン
	int						F5032_36Disp;						// 36.入力許可サイン

	char					F5032_37A[MONY_BUF_SIZE],			// 37.A 選択可能な計算式区分の内から選択した金額 税率6.24%適用分 A
							F5032_37B[MONY_BUF_SIZE],			// 37.B 選択可能な計算式区分の内から選択した金額 税率7.8%適用分  B
							F5032_37C[MONY_BUF_SIZE];			// 37.C 選択可能な計算式区分の内から選択した金額 合計            C

	int						m_curwnd;
	
	CSnHeadData				**m_pSnHeadData;					// 消費税ヘッダ情報
	CSyzSyukei				**m_pSyzSyukei;						// 集計モジュール
	CH31SnFhyo5_012Data		**m_pSnFhyo5_012Data;				// 付表5-12データクラス

	CArithEx				*m_pArith;

	CString					m_code;								// 有利な計算式の集計コード
	short					m_id;								// 有利な計算式のコントロールＩＤ
	short					m_PrintId;							// 有隣計算式の出力用ID
	int						m_typecnt;							// 使用業種数
	int						m_flg;								// D0:F列は別計算
	int						m_4val;								// 0:控除対象仕入税額計算の為の消費税額なし
	MoneyBasejagArray		m_money35;							// 350000番台の集計データ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Fhyo5_032Diag01;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
};
