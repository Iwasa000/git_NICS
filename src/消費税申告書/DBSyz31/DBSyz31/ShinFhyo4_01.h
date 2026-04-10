#pragma once
#include "icsdiagctrl.h"


// CShinFhyo4_01 ダイアログ

class CShinFhyo4_01 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo4_01)

public:
	CShinFhyo4_01(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo4_01();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY4_01 };

	// 初期情報のセット
	int InitInfo ( CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith);

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 初期ポジションセット
	void SetItemPosition();

	// データの読込み
	int ReadData();

	// データの書込み
	int WriteData();

	// 画面出力
	void DispFh4_01();

	// 金額表示
	void dsp_prs(unsigned short Id, short Index, char* Mony);

	// 金額表示クリア
	void dsp_cls(unsigned short Id, short Index);

	// 各入力データ抽出
	void GetAllDiagData();

	// 集計モジュールを更新
	int SyukeiDataSet( int cID );
	int MemoryReget();
	void ReadData2();


	//==== 表示 ====//

	// 課税標準額表示
	void f401_1data();
	// 課税資産の譲渡等の対価の額
	void f401_1_1data();
	// 消費税額
	void f401_2data();
	// 貸倒回収に係る消費税額
	void f401_3data();
	// 控除対象仕入税額
	void f401_4data();
	// 返還等対価に係る税額
	void f401_5data();
	// 貸倒れに係る税額
	void f401_6data();
	// 控除税額小計
	void f401_7data();
	// 控除不足還付税額
	void f401_8data();
	// 差引税額
	void f401_9data();
	// 合計差引税額
	void f401_10data();
	// 控除不足還付税額
	void f401_11data();
	// 差引税額
	void f401_12data();
	// 合計差引地方消費税の課税標準となる消費税額
	void f401_13data();
	// 還付額
	void f401_14data();
	// 納税額
	void f401_15data();
	// 合計差引譲渡割額
	void f401_16data();

	// データ表示
	void Disp_f401data(int idx);

	//================


	// 入力項目色変え
	void ChangeColor ( unsigned short id, short Index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 課税取引金額計算書から転記時のフォーカス移動
	void MoveFocusInTaxTenki();

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 特例帳票を使用しているか
	bool IsUseTokurei();

private:
	CWnd					*m_pParent;							// 親ウィンドウ

	char					F401_1X[MONY_BUF_SIZE],				// 1.X 課税標準額 旧税率分小計    X
							F401_1D[MONY_BUF_SIZE],				// 1.D 課税標準額 税率6.24%適用分 D
							F401_1E[MONY_BUF_SIZE],				// 1.E 課税標準額 税率7.8%適用分  E
							F401_1F[MONY_BUF_SIZE],				// 1.F 課税標準額 合計            F

							F401_1_1X[MONY_BUF_SIZE],			// 1-1.X 課税資産の譲渡等の対価の額 旧税率分小計    X
							F401_1_1D[MONY_BUF_SIZE],			// 1-1.D 課税資産の譲渡等の対価の額 税率6.24%適用分 D
							F401_1_1E[MONY_BUF_SIZE],			// 1-1.E 課税資産の譲渡等の対価の額 税率7.8%適用分  E
							F401_1_1F[MONY_BUF_SIZE],			// 1-1.F 課税資産の譲渡等の対価の額 合計            F

							F401_2X[MONY_BUF_SIZE],				// 2.X 消費税額 旧税率分小計    X
							F401_2D[MONY_BUF_SIZE],				// 2.D 消費税額 税率6.24%適用分 D
							F401_2E[MONY_BUF_SIZE],				// 2.E 消費税額 税率7.8%適用分  E
							F401_2F[MONY_BUF_SIZE],				// 2.F 消費税額 合計            F

							F401_3X[MONY_BUF_SIZE],				// 3.X 貸倒回収に係る消費税額 旧税率分小計    X
							F401_3D[MONY_BUF_SIZE],				// 3.D 貸倒回収に係る消費税額 税率6.24%適用分 D
							F401_3E[MONY_BUF_SIZE],				// 3.E 貸倒回収に係る消費税額 税率7.8%適用分  E
							F401_3F[MONY_BUF_SIZE],				// 3.F 貸倒回収に係る消費税額 合計            F

							F401_4X[MONY_BUF_SIZE],				// 4.X 控除対象仕入れ税額 旧税率分小計    X
							F401_4D[MONY_BUF_SIZE],				// 4.D 控除対象仕入れ税額 税率6.24%適用分 D
							F401_4E[MONY_BUF_SIZE],				// 4.E 控除対象仕入れ税額 税率7.8%適用分  E
							F401_4F[MONY_BUF_SIZE],				// 4.F 控除対象仕入れ税額 合計            F

							F401_5X[MONY_BUF_SIZE],				// 5.X 返還等対価に係る税額 旧税率分小計    X
							F401_5D[MONY_BUF_SIZE],				// 5.D 返還等対価に係る税額 税率6.24%適用分 D
							F401_5E[MONY_BUF_SIZE],				// 5.E 返還等対価に係る税額 税率7.8%適用分  E
							F401_5F[MONY_BUF_SIZE],				// 5.F 返還等対価に係る税額 合計            F

							F401_6X[MONY_BUF_SIZE],				// 6.X 貸倒れに係る税額 旧税率分小計    X
							F401_6D[MONY_BUF_SIZE],				// 6.D 貸倒れに係る税額 税率6.24%適用分 D
							F401_6E[MONY_BUF_SIZE],				// 6.E 貸倒れに係る税額 税率7.8%適用分  E
							F401_6F[MONY_BUF_SIZE],				// 6.F 貸倒れに係る税額 合計            F

							F401_7X[MONY_BUF_SIZE],				// 7.X 控除税額小計 旧税率分小計    X
							F401_7D[MONY_BUF_SIZE],				// 7.D 控除税額小計 税率6.24%適用分 D
							F401_7E[MONY_BUF_SIZE],				// 7.E 控除税額小計 税率7.8%適用分  E
							F401_7F[MONY_BUF_SIZE],				// 7.F 控除税額小計 合計            F

							F401_8X[MONY_BUF_SIZE],				// 8.X 控除不足還付税額 旧税率分小計    X
							F401_8D[MONY_BUF_SIZE],				// 8.D 控除不足還付税額 税率6.24%適用分 D
							F401_8E[MONY_BUF_SIZE],				// 8.E 控除不足還付税額 税率7.8%適用分  E
							F401_8F[MONY_BUF_SIZE],				// 8.F 控除不足還付税額 合計            F

							F401_9X[MONY_BUF_SIZE],				// 9.X 差引税額 旧税率分小計    X
							F401_9D[MONY_BUF_SIZE],				// 9.D 差引税額 税率6.24%適用分 D
							F401_9E[MONY_BUF_SIZE],				// 9.E 差引税額 税率7.8%適用分  E
							F401_9F[MONY_BUF_SIZE],				// 9.F 差引税額 合計            F

							F401_10F[MONY_BUF_SIZE],			// 10.F 合計差引税額 合計 F

							F401_11X[MONY_BUF_SIZE],			// 11.X 控除不足還付税額 旧税率分小計    X
							F401_11E[MONY_BUF_SIZE],			// 11.E 控除不足還付税額 税率7.8%適用分  E
							F401_11F[MONY_BUF_SIZE],			// 11.F 控除不足還付税額 合計            F

							F401_12X[MONY_BUF_SIZE],			// 12.X 差引税額 旧税率分小計    X
							F401_12E[MONY_BUF_SIZE],			// 12.E 差引税額 税率7.8%適用分  E
							F401_12F[MONY_BUF_SIZE],			// 12.F 差引税額 合計            F

							F401_13X[MONY_BUF_SIZE],			// 13.X 合計差引地方消費税の課税標準となる消費税額 旧税率分小計    X
							F401_13E[MONY_BUF_SIZE],			// 13.E 合計差引地方消費税の課税標準となる消費税額 税率7.8%適用分  E
							F401_13F[MONY_BUF_SIZE],			// 13.F 合計差引地方消費税の課税標準となる消費税額 合計            F

							F401_14X[MONY_BUF_SIZE],			// 14.X 還付額 旧税率分小計    X
							F401_14E[MONY_BUF_SIZE],			// 14.E 還付額 税率7.8%適用分  E
							F401_14F[MONY_BUF_SIZE],			// 14.F 還付額 合計            F

							F401_15X[MONY_BUF_SIZE],			// 15.X 納付額 旧税率分小計    X
							F401_15E[MONY_BUF_SIZE],			// 15.E 納付額 税率7.8%適用分  E
							F401_15F[MONY_BUF_SIZE],			// 15.F 納付額 合計            F

							F401_16F[MONY_BUF_SIZE];			// 16.F 合計差引譲渡割額 合計 F


	int						F401;								// □□□□ □□□□ □□□□ □□□□ □□□□ □□□■ □□□■ □□□■
																// 変更サイン
																// D0-D3	:旧税率分
																// D4-D7	:6.24％
																// D8-D11	:7.8％

	int						F401_1c;							// 1-1手入力サイン

	//== 帳票データ本体 ==//
	CH31SnFhyo4_01Data		**m_pSnFhyo4_01Data;
	CSnHeadData				**m_pSnHeadData;					// 消費税ヘッダ情報
	CSyzSyukei				**m_pSyzSyukei;						// 消費税集計クラス
	CArithEx				*m_pArith;

	int						m_curwnd;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo4_01Diag01;
	CIcsdiagctrl m_Fhyo4_01Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
