#pragma once
#include "icsdiagctrl.h"


// CShinFhyo4_02 ダイアログ

class CShinFhyo4_02 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo4_02)

public:
	CShinFhyo4_02(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo4_02();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY4_02 };

	// 初期情報のセット
	int InitInfo ( CH31SnFhyo4_02Data **pSnFhyo4_02Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith);

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// データの書込み
	int WriteData();

	// データの読込み
	int ReadData();

	// 画面出力
	void DispFh402();

	// データ表示
	void Disp_f402data ( int idx );

	// 課税標準額
	void f402_1data();

	// 課税資産の譲渡等の対価の額
	void f402_1_1data();

	// 消費税額
	void f402_2data();

	// 貸倒回収に係る消費税額
	void f402_3data();

	// 控除対象仕入税額
	void f402_4data();

	// 返還等対価に係る税額
	void f402_5data();

	// 貸倒れに係る税額
	void f402_6data();

	// 控除税額小計
	void f402_7data();

	// 控除不足還付税額
	void f402_8data();

	// 差引税額
	void f402_9data();

	// 控除不足還付税額
	void f402_11data();
	
	// 差引税額
	void f402_12data();

	// 合計差引地方消費税の課税標準となる消費税額
	void f402_13data();

	// 還付額
	void f402_14data();

	// 納税額
	void f402_15data();

	// 金額表示クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 各入力データ抽出
	void GetAllDiagData();

	// 初期ポジションセット
	void SetItemPosition();


	// 集計モジュール
	int SyukeiDataSet( int cID );
	int MemoryReget();
	void ReadData2();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int index, int Move );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd					*m_pParent;							// 親ウィンドウ

	char					F402_1A[MONY_BUF_SIZE],				// 1.A 課税標準額 税率3%適用分   A
							F402_1B[MONY_BUF_SIZE],				// 1.B 課税標準額 税率4%適用分   B
							F402_1C[MONY_BUF_SIZE],				// 1.C 課税標準額 税率6.3%適用分 C
							F402_1X[MONY_BUF_SIZE],				// 1.X 課税標準額 旧税率分小計   X (A+B+C)

							F402_1_1A[MONY_BUF_SIZE],			// 1-1.A 課税資産の譲渡等の対価の額 税率3%適用分   A
							F402_1_1B[MONY_BUF_SIZE],			// 1-1.B 課税資産の譲渡等の対価の額 税率4%適用分   B
							F402_1_1C[MONY_BUF_SIZE],			// 1-1.C 課税資産の譲渡等の対価の額 税率6.3%適用分 C
							F402_1_1X[MONY_BUF_SIZE],			// 1-1.X 課税資産の譲渡等の対価の額 旧税率分小計   X (A+B+C)

							F402_2A[MONY_BUF_SIZE],				// 2.A 消費税額 税率3%適用分   A
							F402_2B[MONY_BUF_SIZE],				// 2.B 消費税額 税率4%適用分   B
							F402_2C[MONY_BUF_SIZE],				// 2.C 消費税額 税率6.3%適用分 C
							F402_2X[MONY_BUF_SIZE],				// 2.X 消費税額 旧税率分小計   X (A+B+C)

							F402_3A[MONY_BUF_SIZE],				// 3.A 貸倒回収に係る消費税額 税率3%適用分   A
							F402_3B[MONY_BUF_SIZE],				// 3.B 貸倒回収に係る消費税額 税率4%適用分   B
							F402_3C[MONY_BUF_SIZE],				// 3.C 貸倒回収に係る消費税額 税率6.3%適用分 C
							F402_3X[MONY_BUF_SIZE],				// 3.X 貸倒回収に係る消費税額 旧税率分小計   X (A+B+C)

							F402_4A[MONY_BUF_SIZE],				// 4.A 控除対象仕入税額 税率3%適用分   A
							F402_4B[MONY_BUF_SIZE],				// 4.B 控除対象仕入税額 税率4%適用分   B
							F402_4C[MONY_BUF_SIZE],				// 4.C 控除対象仕入税額 税率6.3%適用分 C
							F402_4X[MONY_BUF_SIZE],				// 4.X 控除対象仕入税額 旧税率分小計   X (A+B+C)

							F402_5A[MONY_BUF_SIZE],				// 5.A 返還等対価に係る税額 税率3%適用分   A
							F402_5B[MONY_BUF_SIZE],				// 5.B 返還等対価に係る税額 税率4%適用分   B
							F402_5C[MONY_BUF_SIZE],				// 5.C 返還等対価に係る税額 税率6.3%適用分 C
							F402_5X[MONY_BUF_SIZE],				// 5.X 返還等対価に係る税額 旧税率分小計   X (A+B+C)

							F402_6A[MONY_BUF_SIZE],				// 6.A 貸倒れに係る税額 税率3%適用分   A
							F402_6B[MONY_BUF_SIZE],				// 6.B 貸倒れに係る税額 税率4%適用分   B
							F402_6C[MONY_BUF_SIZE],				// 6.C 貸倒れに係る税額 税率6.3%適用分 C
							F402_6X[MONY_BUF_SIZE],				// 6.X 貸倒れに係る税額 旧税率分小計   X (A+B+C)

							F402_7A[MONY_BUF_SIZE],				// 7.A 控除税額小計 税率3%適用分   A
							F402_7B[MONY_BUF_SIZE],				// 7.B 控除税額小計 税率4%適用分   B
							F402_7C[MONY_BUF_SIZE],				// 7.C 控除税額小計 税率6.3%適用分 C
							F402_7X[MONY_BUF_SIZE],				// 7.X 控除税額小計 旧税率分小計   X (A+B+C)

							F402_8A[MONY_BUF_SIZE],				// 8.A 控除不足還付税額 税率3%適用分   A
							F402_8B[MONY_BUF_SIZE],				// 8.B 控除不足還付税額 税率4%適用分   B
							F402_8C[MONY_BUF_SIZE],				// 8.C 控除不足還付税額 税率6.3%適用分 C
							F402_8X[MONY_BUF_SIZE],				// 8.X 控除不足還付税額 旧税率分小計   X (A+B+C)

							F402_9A[MONY_BUF_SIZE],				// 9.A 差引税額 税率3%適用分   A
							F402_9B[MONY_BUF_SIZE],				// 9.B 差引税額 税率4%適用分   B
							F402_9C[MONY_BUF_SIZE],				// 9.C 差引税額 税率6.3%適用分 C
							F402_9X[MONY_BUF_SIZE],				// 9.X 差引税額 旧税率分小計   X (A+B+C)

							F402_11B[MONY_BUF_SIZE],			// 11.B 控除不足還付税額 税率4%適用分   B
							F402_11C[MONY_BUF_SIZE],			// 11.C 控除不足還付税額 税率6.3%適用分 C
							F402_11X[MONY_BUF_SIZE],			// 11.X 控除不足還付税額 旧税率分小計   X (A+B+C)

							F402_12B[MONY_BUF_SIZE],			// 12.B 差引税額 税率4%適用分   B
							F402_12C[MONY_BUF_SIZE],			// 12.C 差引税額 税率6.3%適用分 C
							F402_12X[MONY_BUF_SIZE],			// 12.X 差引税額 旧税率分小計   X (A+B+C)

							F402_13B[MONY_BUF_SIZE],			// 13.B 合計差引地方消費税の課税標準となる消費税額 税率4%適用分   B
							F402_13C[MONY_BUF_SIZE],			// 13.C 合計差引地方消費税の課税標準となる消費税額 税率6.3%適用分 C
							F402_13X[MONY_BUF_SIZE],			// 13.X 合計差引地方消費税の課税標準となる消費税額 旧税率分小計   X (A+B+C)

							F402_14B[MONY_BUF_SIZE],			// 14.B 還付額 税率4%適用分   B
							F402_14C[MONY_BUF_SIZE],			// 14.C 還付額 税率6.3%適用分 C
							F402_14X[MONY_BUF_SIZE],			// 14.X 還付額 旧税率分小計   X (A+B+C)

							F402_15B[MONY_BUF_SIZE],			// 15.B 納税額 税率4%適用分   B
							F402_15C[MONY_BUF_SIZE],			// 15.C 納税額 税率6.3%適用分 C
							F402_15X[MONY_BUF_SIZE];			// 15.X 納税額 旧税率分小計   X (A+B+C)

	int						F402;								// 手入力サイン

	int						m_curwnd;

	//== 帳票データ本体 ==
	CH31SnFhyo4_02Data		**m_pSnFhyo4_02Data;
	CSnHeadData				**m_pSnHeadData;					// 消費税ヘッダ情報
	CSyzSyukei				**m_pSyzSyukei;						// 集計モジュール
	CArithEx				*m_pArith;
	CSyzUtil				m_Util;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CIcsdiagctrl m_Fhyo4_02Diag01;
	CIcsdiagctrl m_Fhyo4_02Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
