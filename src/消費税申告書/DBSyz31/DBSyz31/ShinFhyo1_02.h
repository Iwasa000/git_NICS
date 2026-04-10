#pragma once
#include "icsdiagctrl.h"


// CShinFhyo1_02 ダイアログ

class CShinFhyo1_02 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo1_02)

public:
	CShinFhyo1_02(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo1_02();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY1_02 };


	// 初期情報のセット
	int InitInfo ( CH31SnFhyo1_02Data **pSnFhyo1_02Data,  CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CSyzSyukei **pSyzSyukei);

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 初期ポジションセット
	void SetItemPosition();

	// データの読込み
	int ReadData( int pas );
	void ReadData2();

	// 画面出力
	void DispFh1_02();

	// 金額表示
	void dsp_prs(unsigned short Id, short Index, char *Mony);

	// 金額表示クリア
	void dsp_cls(unsigned short Id, short Index);

	// データの書込み
	int WriteData();

	// 各入力データ抽出
	void GetAllDiagData();

	// 特定課税仕入れに関連するインデックス
	int ChkTkkzIdx ( short idx );

	// 特定課税仕入れに関する項目の金額のクリア
	void ClearTkkzMony();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 集計モジュールを更新
	int SyukeiDataSet(int cID);
	int MemoryReget();

	//== 各データ処理（色変更、データセットなど） ==//
	// 課税標準額
	void f102_1data();
	// 課税資産の譲渡等の対価の額
	void f102_1_1data();
	// 特定課税仕入れに係る支払い対価の額
	void f102_1_2data();
	// 消費税額
	void f102_2data();
	// 控除過大調整税額
	void f102_3data();
	// 控除対象仕入税額
	void f102_4data();
	// 返還等対価に係る税額
	void f102_5data();
	// 売上の返還等対価に係る税額
	void f102_5_1data();
	// 特定課税仕入れの返還等対価に係る税額
	void f102_5_2data();
	// 貸倒に係る税額
	void f102_6data();
	// 控除税額小計
	void f102_7data();
	// 控除不足還付税額
	void f102_8data();
	// 差引税額
	void f102_9data();
	// 控除不足還付税額
	void f102_11data();
	// 差引税額
	void f102_12data();
	// 合計差引地方消費税の課税標準となる消費税額
	void f102_13data();
	// 還付額
	void f102_14data();
	// 納税額
	void f102_15data();

	// 各入力データ表示
	void f102data_all( int atno );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd			*m_pParent;						// 親ウィンドウ


	// == 帳票データ ==
	char			F102_1A[MONY_BUF_SIZE],			// 1.A 課税標準額 税率3%適用分   A
					F102_1B[MONY_BUF_SIZE],			// 1.B 課税標準額 税率4%適用分   B
					F102_1C[MONY_BUF_SIZE],			// 1.C 課税標準額 税率6.3%適用分 C
					F102_1X[MONY_BUF_SIZE],			// 1.X 課税標準額 旧税率分小計   X (A+B+C)
	
					F102_1_1A[MONY_BUF_SIZE],		// 1-1.A 課税資産の譲渡等の対価の額 税率3%適用分   A
					F102_1_1B[MONY_BUF_SIZE],		// 1-1.B 課税資産の譲渡等の対価の額 税率4%適用分   B
					F102_1_1C[MONY_BUF_SIZE],		// 1-1.C 課税資産の譲渡等の対価の額 税率6.3%適用分 C
					F102_1_1X[MONY_BUF_SIZE],		// 1-1.X 課税資産の譲渡等の対価の額 旧税率分小計   X (A+B+C)
							
					F102_1_2C[MONY_BUF_SIZE],		// 1-2.C 特定課税仕入に係る支払い対価の額 税率6.3%適用分 C
					F102_1_2X[MONY_BUF_SIZE],		// 1-2.X 特定課税仕入に係る支払い対価の額 旧税率分小計   X (A+B+C)

					F102_2A[MONY_BUF_SIZE],			// 2.A 消費税額 税率3%適用分   A
					F102_2B[MONY_BUF_SIZE],			// 2.B 消費税額 税率4%適用分   B
					F102_2C[MONY_BUF_SIZE],			// 2.C 消費税額 税率6.3%適用分 C
					F102_2X[MONY_BUF_SIZE],			// 2.X 消費税額 旧税率分小計   X (A+B+C)

					F102_3A[MONY_BUF_SIZE],			// 3.A 控除過大調整税額 税率3%適用分   A
					F102_3B[MONY_BUF_SIZE],			// 3.B 控除過大調整税額 税率4%適用分   B
					F102_3C[MONY_BUF_SIZE],			// 3.C 控除過大調整税額 税率6.3%適用分 C
					F102_3X[MONY_BUF_SIZE],			// 3.X 控除過大調整税額 旧税率分小計   X (A+B+C)

					F102_4A[MONY_BUF_SIZE],			// 4.A 控除対象仕入税額 税率3%適用分   A
					F102_4B[MONY_BUF_SIZE],			// 4.B 控除対象仕入税額 税率4%適用分   B
					F102_4C[MONY_BUF_SIZE],			// 4.C 控除対象仕入税額 税率6.3%適用分 C
					F102_4X[MONY_BUF_SIZE],			// 4.X 控除対象仕入税額 旧税率分小計   X (A+B+C)

					F102_5A[MONY_BUF_SIZE],			// 5.A 返還等対価に係る税額 税率3%適用分   A
					F102_5B[MONY_BUF_SIZE],			// 5.B 返還等対価に係る税額 税率4%適用分   B
					F102_5C[MONY_BUF_SIZE],			// 5.C 返還等対価に係る税額 税率6.3%適用分 C
					F102_5X[MONY_BUF_SIZE],			// 5.X 返還等対価に係る税額 旧税率分小計   X (A+B+C)

					F102_5_1A[MONY_BUF_SIZE],		// 5.1A 売上げの返還等対価に係る税額 税率3%適用分   A
					F102_5_1B[MONY_BUF_SIZE],		// 5.1B 売上げの返還等対価に係る税額 税率4%適用分   B
					F102_5_1C[MONY_BUF_SIZE],		// 5.1C 売上げの返還等対価に係る税額 税率6.3%適用分 C
					F102_5_1X[MONY_BUF_SIZE],		// 5.1X 売上げの返還等対価に係る税額 旧税率分小計   X (A+B+C)

					F102_5_2C[MONY_BUF_SIZE],		// 5.2C 特定課税仕入れの返還等対価に係る税額 税率6.3%適用分 C
					F102_5_2X[MONY_BUF_SIZE],		// 5.2X 特定課税仕入れの返還等対価に係る税額 旧税率分小計   X (A+B+C)

					F102_6A[MONY_BUF_SIZE],			// 6.A 貸倒れに係る税額 税率3%適用分   A
					F102_6B[MONY_BUF_SIZE],			// 6.B 貸倒れに係る税額 税率4%適用分   B
					F102_6C[MONY_BUF_SIZE],			// 6.C 貸倒れに係る税額 税率6.3%適用分 C
					F102_6X[MONY_BUF_SIZE],			// 6.X 貸倒れに係る税額 旧税率分小計   X (A+B+C)

					F102_7A[MONY_BUF_SIZE],			// 7.A 控除税額小計 税率3%適用分   A
					F102_7B[MONY_BUF_SIZE],			// 7.B 控除税額小計 税率4%適用分   B
					F102_7C[MONY_BUF_SIZE],			// 7.C 控除税額小計 税率6.3%適用分 C
					F102_7X[MONY_BUF_SIZE],			// 7.X 控除税額小計 旧税率分小計   X (A+B+C)

					F102_8A[MONY_BUF_SIZE],			// 8.A 控除不足還付税額 税率3%適用分   A
					F102_8B[MONY_BUF_SIZE],			// 8.B 控除不足還付税額 税率4%適用分   B
					F102_8C[MONY_BUF_SIZE],			// 8.C 控除不足還付税額 税率6.3%適用分 C
					F102_8X[MONY_BUF_SIZE],			// 8.X 控除不足還付税額 旧税率分小計   X (A+B+C)

					F102_9A[MONY_BUF_SIZE],			// 9.A 差引税額 税率3%適用分   A
					F102_9B[MONY_BUF_SIZE],			// 9.B 差引税額 税率4%適用分   B
					F102_9C[MONY_BUF_SIZE],			// 9.C 差引税額 税率6.3%適用分 C
					F102_9X[MONY_BUF_SIZE],			// 9.X 差引税額 旧税率分小計   X (A+B+C)

					F102_11B[MONY_BUF_SIZE],		// 11.B 控除不足還付税額 税率4%適用分   B
					F102_11C[MONY_BUF_SIZE],		// 11.C 控除不足還付税額 税率6.3%適用分 C
					F102_11X[MONY_BUF_SIZE],		// 11.X 控除不足還付税額 旧税率分小計   X (A+B+C)

					F102_12B[MONY_BUF_SIZE],		// 12.B 差引税額 税率4%適用分   B
					F102_12C[MONY_BUF_SIZE],		// 12.C 差引税額 税率6.3%適用分 C
					F102_12X[MONY_BUF_SIZE],		// 12.X 差引税額 旧税率分小計   X (A+B+C)

					F102_13B[MONY_BUF_SIZE],		// 13.B 合計差引地方消費税の課税標準となる消費税額 税率4%適用分   B
					F102_13C[MONY_BUF_SIZE],		// 13.C 合計差引地方消費税の課税標準となる消費税額 税率6.3%適用分 C
					F102_13X[MONY_BUF_SIZE],		// 13.X 合計差引地方消費税の課税標準となる消費税額 旧税率分小計   X (A+B+C)

					F102_14B[MONY_BUF_SIZE],		// 14.B 還付額 税率4%適用分   B
					F102_14C[MONY_BUF_SIZE],		// 14.C 還付額 税率6.3%適用分 C
					F102_14X[MONY_BUF_SIZE],		// 14.X 還付額 旧税率分小計   X (A+B+C)

					F102_15B[MONY_BUF_SIZE],		// 15.B 納税額 税率4%適用分   B
					F102_15C[MONY_BUF_SIZE],		// 15.C 納税額 税率6.3%適用分 C
					F102_15X[MONY_BUF_SIZE];		// 15.X 納税額 旧税率分小計   X (A+B+C)



	//== 帳票データ本体 ==//
	CH31SnFhyo1_02Data	**m_pSnFhyo1_02Data;
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報

	SYC_SHININFO		*m_pShinInfo;					// 共通情報(現時点では使用してないが、今後用に処理を入れておく)

	int					m_FrSw;							// Readサイン？元処理引用

	int					F102_1c;						// 課税標準額				手入力＆集計サイン　(D3 , D7 , D11・・・手入力サイン)
	int					F102_2c;						// 消費税額					手入力＆集計サイン　(D3 , D7 , D11・・・手入力サイン)
	int					F102_5c;						// 返還等対価に係る税額　	手入力＆集計サイン　(D3 , D7 , D11・・・手入力サイン)

	int					m_curwnd;

	//== その他 ==//
	CArithEx			*m_pArith;

	CSyzSyukei			**m_pSyzSyukei;					// 消費税集計クラス

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	CIcsdiagctrl m_Fhyo1_02Diag01;
	CIcsdiagctrl m_Fhyo1_02Diag02;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
