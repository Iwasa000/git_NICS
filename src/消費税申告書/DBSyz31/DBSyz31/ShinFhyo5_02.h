#pragma once
#include "icsdiagctrl.h"


// CShinFhyo5_02 ダイアログ

class CShinFhyo5_02 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo5_02)

public:
	CShinFhyo5_02(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo5_02();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY5_02 };

	// 初期情報のセット
	int InitInfo ( CH31SnFhyo5_02Data **m_pSnFhyo5_02Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei);

	// 初期化
	int Init();

	// データ読み込み
	void ReadData();
	void ReadData2();

	// データ書き込み
	void WriteData();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 初期ポジションセット
	void SetItemPosition();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int index, int Move );

	// 金額表示
	void dsp_prs(unsigned short Id, short Index, char* Mony);

	// 金額表示クリア
	void dsp_cls(unsigned short Id, short Index);

	// 入力データ抽出
	void GetAllDiagData();


	//== 各入力データ処理 ==//
	// 課税標準額に対する消費税額
	void f502_1data();
	// 貸倒回収に係る消費税額
	void f502_2data();
	// 売上対価の返還等に係る消費税額
	void f502_3data();
	// 控除対象仕入税額の計算の基礎となる消費税額
	void f502_4data();
	// ④×みなし仕入率
	void f502_5data();
	// 事業区分別の合計額
	void f502_6data();
	// 第一種事業(卸売業)
	void f502_7data();
	// 第二種事業(小売業)
	void f502_8data();
	// 第三種事業(製造業等)
	void f502_9data();
	// 第四種事業(その他)
	void f502_10data();
	// 第五種事業(サービス業等)
	void f502_11data();
	// 第六種事業(不動産業)
	void f502_12data();
	// 事業区分別の合計額
	void f502_13data();
	// 第一種事業(卸売業)
	void f502_14data();
	// 第二種事業(小売業)
	void f502_15data();
	// 第三種事業(製造業等)
	void f502_16data();
	// 第四種事業(その他)
	void f502_17data();
	// 第五種事業(サービス業等)
	void f502_18data();
	// 第六種事業(不動産業)
	void f502_19data();

	// 各入力データの表示
	void Disp_f502data( int index );

	// 画面表示
	void DispFh5_02();
	
	// 集計モジュール更新
	int SyukeiDataSet ( int cID );

	// 集計モジュールからデータ取得
	int MemoryReget();

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 第二種の項目かどうか？
	BOOL CheckInputIndex2wari( int index );

	// TISW修正 2026/01/21 START
	/// <summary>
	/// 税抜セルに対応する消費税額セルの状態をリセットする
	/// </summary>
	/// <param name="index">税抜セルのインデックス</param>
	void ResetTaxCellForAmountCell(int index);
	// TISW修正 2026/01/21 END

private:
	CWnd					*m_pParent;							// 親ウィンドウ

	char					F502_1A[MONY_BUF_SIZE],				// 1.X 課税標準額に対する消費税額 旧税率分小計    X
							F502_1B[MONY_BUF_SIZE],				// 1.D 課税標準額に対する消費税額 税率6.24%適用分 D
							F502_1C[MONY_BUF_SIZE],				// 1.E 課税標準額に対する消費税額 税率7.8%適用分  E
							F502_1X[MONY_BUF_SIZE],				// 1.F 課税標準額に対する消費税額 合計            F

							F502_2A[MONY_BUF_SIZE],				// 2.X 貸倒回収に係る消費税額 旧税率分小計    X
							F502_2B[MONY_BUF_SIZE],				// 2.D 貸倒回収に係る消費税額 税率6.24%適用分 D
							F502_2C[MONY_BUF_SIZE],				// 2.E 貸倒回収に係る消費税額 税率7.8%適用分  E
							F502_2X[MONY_BUF_SIZE],				// 2.F 貸倒回収に係る消費税額 合計            F

							F502_3A[MONY_BUF_SIZE],				// 3.X 売上対価の返還等に係る消費税額 旧税率分小計    X
							F502_3B[MONY_BUF_SIZE],				// 3.D 売上対価の返還等に係る消費税額 税率6.24%適用分 D
							F502_3C[MONY_BUF_SIZE],				// 3.E 売上対価の返還等に係る消費税額 税率7.8%適用分  E
							F502_3X[MONY_BUF_SIZE],				// 3.F 売上対価の返還等に係る消費税額 合計            F

							F502_4A[MONY_BUF_SIZE],				// 4.X 控除対象仕入税額の計算の基礎となる消費税額 旧税率分小計    X
							F502_4B[MONY_BUF_SIZE],				// 4.D 控除対象仕入税額の計算の基礎となる消費税額 税率6.24%適用分 D
							F502_4C[MONY_BUF_SIZE],				// 4.E 控除対象仕入税額の計算の基礎となる消費税額 税率7.8%適用分  E
							F502_4X[MONY_BUF_SIZE],				// 4.F 控除対象仕入税額の計算の基礎となる消費税額 合計            F

							F502_5A[MONY_BUF_SIZE],				// 5.X ④×みなし仕入率 旧税率分小計    X
							F502_5B[MONY_BUF_SIZE],				// 5.D ④×みなし仕入率 税率6.24%適用分 D
							F502_5C[MONY_BUF_SIZE],				// 5.E ④×みなし仕入率 税率7.8%適用分  E
							F502_5X[MONY_BUF_SIZE],				// 5.F ④×みなし仕入率 合計            F

							F502_6A[MONY_BUF_SIZE],				// 6.X 事業区分別の合計額 旧税率分小計    X
							F502_6B[MONY_BUF_SIZE],				// 6.D 事業区分別の合計額 税率6.24%適用分 D
							F502_6C[MONY_BUF_SIZE],				// 6.E 事業区分別の合計額 税率7.8%適用分  E
							F502_6X[MONY_BUF_SIZE],				// 6.F 事業区分別の合計額 合計            F

							F502_7A[MONY_BUF_SIZE],				// 7.X 第一種事業(卸売業) 旧税率分小計    X
							F502_7B[MONY_BUF_SIZE],				// 7.D 第一種事業(卸売業) 税率6.24%適用分 D
							F502_7C[MONY_BUF_SIZE],				// 7.E 第一種事業(卸売業) 税率7.8%適用分  E
							F502_7X[MONY_BUF_SIZE],				// 7.F 第一種事業(卸売業) 合計            F
	
							F502_8A[MONY_BUF_SIZE],				// 8.X 第二種事業(小売業) 旧税率分小計    X
							F502_8B[MONY_BUF_SIZE],				// 8.D 第二種事業(小売業) 税率6.24%適用分 D
							F502_8C[MONY_BUF_SIZE],				// 8.E 第二種事業(小売業) 税率7.8%適用分  E
							F502_8X[MONY_BUF_SIZE],				// 8.F 第二種事業(小売業) 合計            F

							F502_9A[MONY_BUF_SIZE],				// 9.X 第三種事業(製造業等) 旧税率分小計    X
							F502_9B[MONY_BUF_SIZE],				// 9.D 第三種事業(製造業等) 税率6.24%適用分 D
							F502_9C[MONY_BUF_SIZE],				// 9.E 第三種事業(製造業等) 税率7.8%適用分  E
							F502_9X[MONY_BUF_SIZE],				// 9.F 第三種事業(製造業等) 合計            F

							F502_10A[MONY_BUF_SIZE],			// 10.X 第四種事業(その他) 旧税率分小計    X
							F502_10B[MONY_BUF_SIZE],			// 10.D 第四種事業(その他) 税率6.24%適用分 D
							F502_10C[MONY_BUF_SIZE],			// 10.E 第四種事業(その他) 税率7.8%適用分  E
							F502_10X[MONY_BUF_SIZE],			// 10.F 第四種事業(その他) 合計            F

							F502_11A[MONY_BUF_SIZE],			// 11.X 第五種事業(サービス業等) 旧税率分小計    X
							F502_11B[MONY_BUF_SIZE],			// 11.D 第五種事業(サービス業等) 税率6.24%適用分 D
							F502_11C[MONY_BUF_SIZE],			// 11.E 第五種事業(サービス業等) 税率7.8%適用分  E
							F502_11X[MONY_BUF_SIZE],			// 11.F 第五種事業(サービス業等) 合計            F

							F502_12A[MONY_BUF_SIZE],			// 12.X 第六種事業(不動産業) 旧税率分小計    X
							F502_12B[MONY_BUF_SIZE],			// 12.D 第六種事業(不動産業) 税率6.24%適用分 D
							F502_12C[MONY_BUF_SIZE],			// 12.E 第六種事業(不動産業) 税率7.8%適用分  E
							F502_12X[MONY_BUF_SIZE],			// 12.F 第六種事業(不動産業) 合計            F

							F502_13A[MONY_BUF_SIZE],			// 13.X 事業区分別の合計額 旧税率分小計    X
							F502_13B[MONY_BUF_SIZE],			// 13.D 事業区分別の合計額 税率6.24%適用分 D
							F502_13C[MONY_BUF_SIZE],			// 13.E 事業区分別の合計額 税率7.8%適用分  E
							F502_13X[MONY_BUF_SIZE],			// 13.F 事業区分別の合計額 合計            F

							F502_14A[MONY_BUF_SIZE],			// 14.X 第一種事業(卸売業) 旧税率分小計    X
							F502_14B[MONY_BUF_SIZE],			// 14.D 第一種事業(卸売業) 税率6.24%適用分 D
							F502_14C[MONY_BUF_SIZE],			// 14.E 第一種事業(卸売業) 税率7.8%適用分  E
							F502_14X[MONY_BUF_SIZE],			// 14.F 第一種事業(卸売業) 合計            F

							F502_15A[MONY_BUF_SIZE],			// 15.X 第二種事業(小売業) 旧税率分小計    X
							F502_15B[MONY_BUF_SIZE],			// 15.D 第二種事業(小売業) 税率6.24%適用分 D
							F502_15C[MONY_BUF_SIZE],			// 15.E 第二種事業(小売業) 税率7.8%適用分  E
							F502_15X[MONY_BUF_SIZE],			// 15.F 第二種事業(小売業) 合計            F

							F502_16A[MONY_BUF_SIZE],			// 16.X 第三種事業(製造業等) 旧税率分小計    X
							F502_16B[MONY_BUF_SIZE],			// 16.D 第三種事業(製造業等) 税率6.24%適用分 D
							F502_16C[MONY_BUF_SIZE],			// 16.E 第三種事業(製造業等) 税率7.8%適用分  E
							F502_16X[MONY_BUF_SIZE],			// 16.F 第三種事業(製造業等) 合計            F

							F502_17A[MONY_BUF_SIZE],			// 17.X 第四種事業(その他) 旧税率分小計    X
							F502_17B[MONY_BUF_SIZE],			// 17.D 第四種事業(その他) 税率6.24%適用分 D
							F502_17C[MONY_BUF_SIZE],			// 17.E 第四種事業(その他) 税率7.8%適用分  E
							F502_17X[MONY_BUF_SIZE],			// 17.F 第四種事業(その他) 合計            F

							F502_18A[MONY_BUF_SIZE],			// 18.X 第五種事業(サービス業等) 旧税率分小計    X
							F502_18B[MONY_BUF_SIZE],			// 18.D 第五種事業(サービス業等) 税率6.24%適用分 D
							F502_18C[MONY_BUF_SIZE],			// 18.E 第五種事業(サービス業等) 税率7.8%適用分  E
							F502_18X[MONY_BUF_SIZE],			// 18.F 第五種事業(サービス業等) 合計            F

							F502_19A[MONY_BUF_SIZE],			// 19.X 第六種事業(不動産業) 旧税率分小計    X
							F502_19B[MONY_BUF_SIZE],			// 19.D 第六種事業(不動産業) 税率6.24%適用分 D
							F502_19C[MONY_BUF_SIZE],			// 19.E 第六種事業(不動産業) 税率7.8%適用分  E
							F502_19X[MONY_BUF_SIZE];			// 19.F 第六種事業(不動産業) 合計            F

	int						F502_6s,
							F502_7s,							// 7.  第一種 再計算スイッチ
							F502_8s,							// 8.  第二種 再計算スイッチ
							F502_9s,							// 9.  第三種 再計算スイッチ
							F502_10s,							// 10. 第四種 再計算スイッチ
							F502_11s,							// 11. 第五種 再計算スイッチ
							F502_12s;							// 12. 第六種 再計算スイッチ

	int						F502_13s,
							F502_14s,							// 14.  第一種 再計算スイッチ
							F502_15s,							// 15.  第二種 再計算スイッチ
							F502_16s,							// 16.  第三種 再計算スイッチ
							F502_17s,							// 17.  第四種 再計算スイッチ
							F502_18s,							// 18.  第五種 再計算スイッチ
							F502_19s;							// 19.  第六種 再計算スイッチ

	int						F502_5kbn;							// みなし仕入率　1:90％...
	int						typeCnt;							// 業種数

	int						m_curwnd;

	CDBNpSub				*m_pZmSub;					// 公益クラス

	CSnHeadData				**m_pSnHeadData;			// 消費税ヘッダ情報
	CH31SnFhyo5_02Data		**m_pSnFhyo5_02Data;
	CSyzSyukei				**m_pSyzSyukei;

	CArithEx				*m_pArith;
	CSyzUtil				m_Util;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo5_02Diag01;
	CIcsdiagctrl m_Fhyo5_02Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
