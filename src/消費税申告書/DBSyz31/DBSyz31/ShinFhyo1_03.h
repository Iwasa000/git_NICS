#pragma once
#include "icsdiagctrl.h"


// ShinFhyo1_03 ダイアログ

class CShinFhyo1_03 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo1_03)

public:
	CShinFhyo1_03(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo1_03();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY1_03 };
//#endif

public:
	// 初期情報のセット
	int InitInfo ( CH31SnFhyo1_01Data **pSnFhyo1_01Data, CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CSyzSyukei **pSyzSyukei );

	// 初期化
	int Init();

	// データ読込み
	int ReadData();

	// 読込みデータを集計モジュールへ
	void ReadData2();

	// データ書込み
	int WriteData();

	// 集計処理
	int MemoryReget();

	// 集計モジュール更新
	int SyukeiDataSet ( int cID );

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 特定課税仕入項目?
	int ChkTkkzIdx ( short idx );

	// 特定課税仕入クリア
	void ClearTkkzMony();

	// 画面出力
	void DispFh1_03();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 表示金額取得
	void GetAllDiagData();

	// ポジションセット
	void SetItemPosition();

	// フォーカス移動先取得
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 全項目表示
	void f103data_all ( int atno );
	// 課税標準額表示
	void f103data_1();
	// 課税資産の譲渡等の対価の額合計表示
	void f103data_1_1();
	// 特定課税仕入に係る支払い対価の額表示
	void f103data_1_2();
	// 消費税額表示
	void f103data_2();
	// 控除過大調整税額表示
	void f103data_3();
	// 控除対象仕入税額表示
	void f103data_4();
	// 返還等対価に係る税額表示
	void f103data_5();
	// 売上げの返還等対価に係る税額表示
	void f103data_5_1();
	// 特定課税仕入れの返還等対価に係る税額表示
	void f103data_5_2();
	// 貸倒れに係る税額表示
	void f103data_6();
	// 控除税額小計表示
	void f103data_7();
	// 控除不足還付税額表示
	void f103data_8();
	// 差引税額表示
	void f103data_9();
	// 地方消費税の課税標準となる消費税額 控除不足還付税額表示
	void f103data_10();
	// 地方消費税の課税標準となる消費税額 差引税額表示
	void f103data_11();
	// 還付額表示
	void f103data_12();
	// 納税額表示
	void f103data_13();

	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 特例帳票を使用しているか
	bool IsUseTokurei();

private:
	CWnd*				m_pParent;		// 親ウィンドウ
	int					m_curwnd;

	// == 帳票データ ==
	char				F103_1A[MONY_BUF_SIZE],			// 課税標準額 税率6.24%適用分  A
						F103_1B[MONY_BUF_SIZE],			// 課税標準額 税率7.8%適用分   B
						F103_1C[MONY_BUF_SIZE],			// 課税標準額 合計 C
	
						F103_1_1A[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額 税率6.24%適用分  A
						F103_1_1B[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額 税率7.8%適用分   B
						F103_1_1C[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額合計 C
							
						F103_1_2B[MONY_BUF_SIZE],		// 特定課税仕入に係る支払い対価の額 税率7.8%適用分   B
						F103_1_2C[MONY_BUF_SIZE],		// 特定課税仕入に係る支払い対価の額 合計 C

						F103_2A[MONY_BUF_SIZE],			// 消費税額 税率6.24%適用分  A
						F103_2B[MONY_BUF_SIZE],			// 消費税額 税率7.8%適用分   B
						F103_2C[MONY_BUF_SIZE],			// 消費税額 合計 C

						F103_3A[MONY_BUF_SIZE],			// 控除過大調整税額 税率6.24%適用分  A
						F103_3B[MONY_BUF_SIZE],			// 控除過大調整税額 税率7.8%適用分   B
						F103_3C[MONY_BUF_SIZE],			// 控除過大調整税額 合計 C

						F103_4A[MONY_BUF_SIZE],			// 控除対象仕入税額 税率6.24%適用分  A
						F103_4B[MONY_BUF_SIZE],			// 控除対象仕入税額 税率7.8%適用分   B
						F103_4C[MONY_BUF_SIZE],			// 控除対象仕入税額 合計 C

						F103_5A[MONY_BUF_SIZE],			// 返還等対価に係る税額 税率6.24%適用分  A
						F103_5B[MONY_BUF_SIZE],			// 返還等対価に係る税額 税率7.8%適用分   B
						F103_5C[MONY_BUF_SIZE],			// 返還等対価に係る税額 合計 C

						F103_5_1A[MONY_BUF_SIZE],		// 売上げの返還等対価に係る税額 税率6.24%適用分  A
						F103_5_1B[MONY_BUF_SIZE],		// 売上げの返還等対価に係る税額 税率7.8%適用分   B
						F103_5_1C[MONY_BUF_SIZE],		// 売上げの返還等対価に係る税額 合計 C

						F103_5_2B[MONY_BUF_SIZE],		// 特定課税仕入れの返還等対価に係る税額 税率7.8%適用分   B
						F103_5_2C[MONY_BUF_SIZE],		// 特定課税仕入れの返還等対価に係る税額 合計 C

						F103_6A[MONY_BUF_SIZE],			// 貸倒れに係る税額 税率6.24%適用分  A
						F103_6B[MONY_BUF_SIZE],			// 貸倒れに係る税額 税率7.8%適用分   B
						F103_6C[MONY_BUF_SIZE],			// 貸倒れに係る税額 合計 C

						F103_7A[MONY_BUF_SIZE],			// 控除税額小計 税率6.24%適用分  A
						F103_7B[MONY_BUF_SIZE],			// 控除税額小計 税率7.8%適用分   B
						F103_7C[MONY_BUF_SIZE],			// 控除税額小計 合計 C

						F103_8C[MONY_BUF_SIZE],			// 控除不足還付税額 合計 C

						F103_9C[MONY_BUF_SIZE],			// 差引税額 合計 C

						F103_10C[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額 控除不足還付税額 合計 C

						F103_11C[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額 差引税額 合計 C

						F103_12C[MONY_BUF_SIZE],		// 還付額 合計 C

						F103_13C[MONY_BUF_SIZE];		// 納税額 合計 C

	int					F103_1c,
						F103_2c;

	CH31SnFhyo1_01Data	**m_pSnFhyo1_01Data;
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報

	SYC_SHININFO		*m_pShinInfo;					// 共通情報
	CArithEx			*m_pArith;						// 倍長演算
	CSyzSyukei			**m_pSyzSyukei;					// 消費税集計クラス



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Fhyo1_03Diag01;
	CIcsdiagctrl m_Fhyo1_03Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
