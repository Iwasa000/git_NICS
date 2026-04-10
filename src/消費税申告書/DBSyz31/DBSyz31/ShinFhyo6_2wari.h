#pragma once
#include "icsdiagctrl.h"


// ShinFhyo4_03 ダイアログ

class CShinFhyo6_2wari : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo6_2wari)

public:
	CShinFhyo6_2wari(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo6_2wari();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY6_2WARI };
//#endif

public:
	// 初期情報のセット
//	int InitInfo ( CH31SnFhyo4_01Data **pSnFhyo4_01Data, CH31SnFhyo5_01Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith );
	int InitInfo( CH31SnFhyo6_2wariData **pSnFhyo6_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith );

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
	void DispFh6_01();

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
	void f601data_all ( int atno );
	// 課税資産の譲渡等の対価の額合計表示
	void f601data_1();
	// 課税標準額表示
	void f601data_2();
	// 課税標準額に対する消費税額表示
	void f601data_3();
	// 貸倒回収に係る消費税額表示
	void f601data_4();
	// 売上対価の返還等に係る消費税額表示
	void f601data_5();
	// 控除対象仕入税額の計算の基礎となる消費税額表示
	void f601data_6();
	// 特別控除税額表示
	void f601data_7();
	// 貸倒れに係る税額表示
	void f601data_8();

	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd*				m_pParent;		// 親ウィンドウ
	int					m_curwnd;

	char				F601_1A[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額 税率6.24%適用分  A
						F601_1B[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額 税率7.8%適用分   B
						F601_1C[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額 合計 C

						F601_2A[MONY_BUF_SIZE],			// 課税標準額 税率6.24%適用分  A
						F601_2B[MONY_BUF_SIZE],			// 課税標準額 税率7.8%適用分   B
						F601_2C[MONY_BUF_SIZE],			// 課税標準額 合計 C

						F601_3A[MONY_BUF_SIZE],			// 課税標準額に対する消費税額 税率6.24%適用分  A
						F601_3B[MONY_BUF_SIZE],			// 課税標準額に対する消費税額 税率7.8%適用分   B
						F601_3C[MONY_BUF_SIZE],			// 課税標準額に対する消費税額 合計 C

						F601_4A[MONY_BUF_SIZE],			// 貸倒回収に係る消費税額 税率6.24%適用分  A
						F601_4B[MONY_BUF_SIZE],			// 貸倒回収に係る消費税額 税率7.8%適用分   B
						F601_4C[MONY_BUF_SIZE],			// 貸倒回収に係る消費税額 合計 C

						F601_5A[MONY_BUF_SIZE],			// 売上対価の返還等に係る消費税額 税率6.24%適用分  A
						F601_5B[MONY_BUF_SIZE],			// 売上対価の返還等に係る消費税額 税率7.8%適用分   B
						F601_5C[MONY_BUF_SIZE],			// 売上対価の返還等に係る消費税額 合計 C

						F601_6A[MONY_BUF_SIZE],			// 控除対象仕入税額の計算の基礎となる消費税額 税率6.24%適用分  A
						F601_6B[MONY_BUF_SIZE],			// 控除対象仕入税額の計算の基礎となる消費税額 税率7.8%適用分   B
						F601_6C[MONY_BUF_SIZE],			// 控除対象仕入税額の計算の基礎となる消費税額 合計 C

						F601_7A[MONY_BUF_SIZE],			// 特別控除税額 税率6.24%適用分  A
						F601_7B[MONY_BUF_SIZE],			// 特別控除税額 税率7.8%適用分   B
						F601_7C[MONY_BUF_SIZE],			// 特別控除税額 合計 C

						F601_8A[MONY_BUF_SIZE],			// 貸倒れに係る税額 税率6.24%適用分  A
						F601_8B[MONY_BUF_SIZE],			// 貸倒れに係る税額 税率7.8%適用分   B
						F601_8C[MONY_BUF_SIZE];			// 貸倒れに係る税額 合計 C

	int					F601_3c;

//----->(メモ)上2つ：データ取得元。　下：新規作成クラス　
//		電子申告用のデータを別に保存するなら下を使用すべき
	CH31SnFhyo4_01Data	**m_pSnFhyo4_01Data;
	CH31SnFhyo5_01Data	**m_pSnFhyo5_01Data;

	CH31SnFhyo6_2wariData  **m_pSnFhyo6_01Data;
//<-----

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
	CIcsdiagctrl m_Fhyo6_01Diag01;
	CIcsdiagctrl m_Fhyo6_01Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
