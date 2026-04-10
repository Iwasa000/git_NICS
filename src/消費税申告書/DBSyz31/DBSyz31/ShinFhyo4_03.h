#pragma once
#include "icsdiagctrl.h"


// ShinFhyo4_03 ダイアログ

class CShinFhyo4_03 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo4_03)

public:
	CShinFhyo4_03(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo4_03();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY4_03 };
//#endif

public:
	// 初期情報のセット
	int InitInfo ( CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith );

	// 初期化
	int Init();

	// データの読込み
	int ReadData();

	// 読込みデータを集計モジュールへ
	void ReadData2();

	// データの書込み
	int WriteData();

	// 集計処理
	int MemoryReget();

	// 集計モジュール更新
	int SyukeiDataSet ( int cID );

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 画面出力
	void DispFh4_03();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 表示金額取得
	void GetAllDiagData();

	// ポジションセット
	void SetItemPosition();

	// 課税取引金額計算書から転記時のフォーカス移動
	void MoveFocusInTaxTenki();

	// フォーカス移動先取得
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 全項目表示
	void f403data_all ( int atno );
	// 課税標準額表示
	void f403data_1();
	// 課税資産の譲渡等の対価の額合計表示
	void f403data_1_1();
	// 消費税額表示
	void f403data_2();
	// 貸倒回収に係る消費税額表示
	void f403data_3();
	// 控除対象仕入税額表示
	void f403data_4();
	// 返還等対価に係る税額表示
	void f403data_5();
	// 貸倒れに係る税額表示
	void f403data_6();
	// 控除税額小計表示
	void f403data_7();
	// 控除不足還付税額表示
	void f403data_8();
	// 差引税額表示
	void f403data_9();
	// 地方消費税の課税標準となる消費税額 控除不足還付税額表示
	void f403data_10();
	// 地方消費税の課税標準となる消費税額 差引税額表示
	void f403data_11();
	// 還付額表示
	void f403data_12();
	// 納税額表示
	void f403data_13();

	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 特例帳票を使用しているか
	bool IsUseTokurei();

private:
	CWnd*				m_pParent;		// 親ウィンドウ
	int					m_curwnd;

	char				F403_1A[MONY_BUF_SIZE],			// 課税標準額 税率6.24%適用分  A
						F403_1B[MONY_BUF_SIZE],			// 課税標準額 税率7.8%適用分   B
						F403_1C[MONY_BUF_SIZE],			// 課税標準額 合計 C
	
						F403_1_1A[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額 税率6.24%適用分  A
						F403_1_1B[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額 税率7.8%適用分   B
						F403_1_1C[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額 合計 C
							
						F403_2A[MONY_BUF_SIZE],			// 消費税額 税率6.24%適用分  A
						F403_2B[MONY_BUF_SIZE],			// 消費税額 税率7.8%適用分   B
						F403_2C[MONY_BUF_SIZE],			// 消費税額 合計 C

						F403_3A[MONY_BUF_SIZE],			// 貸倒回収に係る消費税額 税率6.24%適用分  A
						F403_3B[MONY_BUF_SIZE],			// 貸倒回収に係る消費税額 税率7.8%適用分   B
						F403_3C[MONY_BUF_SIZE],			// 貸倒回収に係る消費税額 合計 C

						F403_4A[MONY_BUF_SIZE],			// 控除対象仕入税額 税率6.24%適用分  A
						F403_4B[MONY_BUF_SIZE],			// 控除対象仕入税額 税率7.8%適用分   B
						F403_4C[MONY_BUF_SIZE],			// 控除対象仕入税額 合計 C

						F403_5A[MONY_BUF_SIZE],			// 返還等対価に係る税額 税率6.24%適用分  A
						F403_5B[MONY_BUF_SIZE],			// 返還等対価に係る税額 税率7.8%適用分   B
						F403_5C[MONY_BUF_SIZE],			// 返還等対価に係る税額 合計 C

						F403_6A[MONY_BUF_SIZE],			// 貸倒れに係る税額 税率6.24%適用分  A
						F403_6B[MONY_BUF_SIZE],			// 貸倒れに係る税額 税率7.8%適用分   B
						F403_6C[MONY_BUF_SIZE],			// 貸倒れに係る税額 合計 C

						F403_7A[MONY_BUF_SIZE],			// 控除税額小計 税率6.24%適用分  A
						F403_7B[MONY_BUF_SIZE],			// 控除税額小計 税率7.8%適用分   B
						F403_7C[MONY_BUF_SIZE],			// 控除税額小計 合計 C

						F403_8C[MONY_BUF_SIZE],			// 控除不足還付税額 合計 C

						F403_9C[MONY_BUF_SIZE],			// 差引税額 合計 C

						F403_10C[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額 控除不足還付税額 合計 C

						F403_11C[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額 差引税額 合計 C

						F403_12C[MONY_BUF_SIZE],		// 譲渡割額 還付額 合計 C

						F403_13C[MONY_BUF_SIZE];		// 譲渡割額 納税額 合計 C

	int					F403_1c;
	int					F403_2c;

	CH31SnFhyo4_01Data	**m_pSnFhyo4_01Data;
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
	CIcsdiagctrl m_Fhyo4_03Diag01;
	CIcsdiagctrl m_Fhyo4_03Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
