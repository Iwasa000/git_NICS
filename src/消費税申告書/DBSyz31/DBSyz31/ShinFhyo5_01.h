#pragma once
#include "icsdiagctrl.h"


// CShinFhyo5_01 ダイアログ

class CShinFhyo5_01 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo5_01)

public:
	CShinFhyo5_01(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo5_01();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY5_01 };

	// 初期情報のセット
	int InitInfo ( CH31SnFhyo5_01Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CSyzSyukei **pSyzSyukei );

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 画面出力
	void DispFh501();

	// データの読込み
	int ReadData();
	void ReadData2();
	
	// データの書込み
	int WriteData();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char* Mony );

	// 金額表示クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 売上割合表示
	void dsp_uws ( unsigned short Id, short Index, unsigned short pers );

	// データ表示
	void Disp_f501data ( int idx );

	// 課税標準額に対する消費税額
	void f501_1data();

	// 貸倒回収に係る消費税額
	void f501_2data();

	// 売上対価の返還等対価に係る税額
	void f501_3data();

	// 控除対象仕入税額の計算の基礎となる消費税額
	void f501_4data();

	// 1種類の事業の専業者の場合の控除対象仕入税額
	void f501_5data();

	// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
	// 事業区分別の合計額
	void f501_6data();

	// 第一種事業(卸売業)
	void f501_7data();

	// 第二種事業(小売業)
	void f501_8data();

	// 第三種事業(製造業等)
	void f501_9data();

	// 第四種事業(その他)
	void f501_10data();

	// 第五種事業(サービス業)
	void f501_11data();

	// 第六種事業(不動産業)
	void f501_12data();

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	void f501_13data();

	// 第一種事業(卸売業)
	void f501_14data();

	// 第二種事業(小売業)
	void f501_15data();

	// 第三種事業(製造業等)
	void f501_16data();

	// 第四種事業(その他)
	void f501_17data();

	// 第五種事業(サービス業)
	void f501_18data();

	// 第六種事業(不動産業)
	void f501_19data();

	// 売上割合表示
	void f501_uws();

	// 初期ポジションセット
	void SetItemPosition();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int index, int Move );

	// 各入力データ抽出
	void GetAllDiagData();

	// 集計モジュール更新
	int SyukeiDataSet ( int cID );
	int MemoryReget();

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 売上割合取得
	int GetPercentage( int id, unsigned short *pWari, char *pArith );

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
	CWnd			*m_pParent;							// 親ウィンドウ

	char			F501_1X[MONY_BUF_SIZE],				// 1.X 課税標準額に対する消費税額 旧税率分小計    X
					F501_1D[MONY_BUF_SIZE],				// 1.D 課税標準額に対する消費税額 税率6.24%適用分 D
					F501_1E[MONY_BUF_SIZE],				// 1.E 課税標準額に対する消費税額 税率7.8%適用分  E
					F501_1F[MONY_BUF_SIZE],				// 1.F 課税標準額に対する消費税額 合計            F

					F501_2X[MONY_BUF_SIZE],				// 2.X 貸倒回収に係る消費税額 旧税率分小計    X
					F501_2D[MONY_BUF_SIZE],				// 2.D 貸倒回収に係る消費税額 税率6.24%適用分 D
					F501_2E[MONY_BUF_SIZE],				// 2.E 貸倒回収に係る消費税額 税率7.8%適用分  E
					F501_2F[MONY_BUF_SIZE],				// 2.F 貸倒回収に係る消費税額 合計            F

					F501_3X[MONY_BUF_SIZE],				// 3.X 売上対価の返還等に係る消費税額 旧税率分小計    X
					F501_3D[MONY_BUF_SIZE],				// 3.D 売上対価の返還等に係る消費税額 税率6.24%適用分 D
					F501_3E[MONY_BUF_SIZE],				// 3.E 売上対価の返還等に係る消費税額 税率7.8%適用分  E
					F501_3F[MONY_BUF_SIZE],				// 3.F 売上対価の返還等に係る消費税額 合計            F

					F501_4X[MONY_BUF_SIZE],				// 4.X 控除対象仕入税額の計算の基礎となる消費税額 旧税率分小計    X
					F501_4D[MONY_BUF_SIZE],				// 4.D 控除対象仕入税額の計算の基礎となる消費税額 税率6.24%適用分 D
					F501_4E[MONY_BUF_SIZE],				// 4.E 控除対象仕入税額の計算の基礎となる消費税額 税率7.8%適用分  E
					F501_4F[MONY_BUF_SIZE],				// 4.F 控除対象仕入税額の計算の基礎となる消費税額 合計            F

					F501_5X[MONY_BUF_SIZE],				// 5.X ④×みなし仕入率 旧税率分小計    X
					F501_5D[MONY_BUF_SIZE],				// 5.D ④×みなし仕入率 税率6.24%適用分 D
					F501_5E[MONY_BUF_SIZE],				// 5.E ④×みなし仕入率 税率7.8%適用分  E
					F501_5F[MONY_BUF_SIZE],				// 5.F ④×みなし仕入率 合計            F

					F501_6X[MONY_BUF_SIZE],				// 6.X 事業区分別の合計額 旧税率分小計    X
					F501_6D[MONY_BUF_SIZE],				// 6.D 事業区分別の合計額 税率6.24%適用分 D
					F501_6E[MONY_BUF_SIZE],				// 6.E 事業区分別の合計額 税率7.8%適用分  E
					F501_6F[MONY_BUF_SIZE],				// 6.F 事業区分別の合計額 合計            F

					F501_7X[MONY_BUF_SIZE],				// 7.X 第一種事業(卸売業) 旧税率分小計    X
					F501_7D[MONY_BUF_SIZE],				// 7.D 第一種事業(卸売業) 税率6.24%適用分 D
					F501_7E[MONY_BUF_SIZE],				// 7.E 第一種事業(卸売業) 税率7.8%適用分  E
					F501_7F[MONY_BUF_SIZE],				// 7.F 第一種事業(卸売業) 合計            F

					F501_8X[MONY_BUF_SIZE],				// 8.X 第二種事業(小売業) 旧税率分小計    X
					F501_8D[MONY_BUF_SIZE],				// 8.D 第二種事業(小売業) 税率6.24%適用分 D
					F501_8E[MONY_BUF_SIZE],				// 8.E 第二種事業(小売業) 税率7.8%適用分  E
					F501_8F[MONY_BUF_SIZE],				// 8.F 第二種事業(小売業) 合計            F

					F501_9X[MONY_BUF_SIZE],				// 9.X 第三種事業(製造業等) 旧税率分小計    X
					F501_9D[MONY_BUF_SIZE],				// 9.D 第三種事業(製造業等) 税率6.24%適用分 D
					F501_9E[MONY_BUF_SIZE],				// 9.E 第三種事業(製造業等) 税率7.8%適用分  E
					F501_9F[MONY_BUF_SIZE],				// 9.F 第三種事業(製造業等) 合計            F

					F501_10X[MONY_BUF_SIZE],			// 10.X 第四種事業(その他) 旧税率分小計    X
					F501_10D[MONY_BUF_SIZE],			// 10.D 第四種事業(その他) 税率6.24%適用分 D
					F501_10E[MONY_BUF_SIZE],			// 10.E 第四種事業(その他) 税率7.8%適用分  E
					F501_10F[MONY_BUF_SIZE],			// 10.F 第四種事業(その他) 合計            F

					F501_11X[MONY_BUF_SIZE],			// 11.X 第五種事業(サービス業等) 旧税率分小計    X
					F501_11D[MONY_BUF_SIZE],			// 11.D 第五種事業(サービス業等) 税率6.24%適用分 D
					F501_11E[MONY_BUF_SIZE],			// 11.E 第五種事業(サービス業等) 税率7.8%適用分  E
					F501_11F[MONY_BUF_SIZE],			// 11.F 第五種事業(サービス業等) 合計            F

					F501_12X[MONY_BUF_SIZE],			// 12.X 第六種事業(不動産業) 旧税率分小計    X
					F501_12D[MONY_BUF_SIZE],			// 12.D 第六種事業(不動産業) 税率6.24%適用分 D
					F501_12E[MONY_BUF_SIZE],			// 12.E 第六種事業(不動産業) 税率7.8%適用分  E
					F501_12F[MONY_BUF_SIZE],			// 12.F 第六種事業(不動産業) 合計            F

					F501_13X[MONY_BUF_SIZE],			// 13.X 事業区分別の合計額 旧税率分小計    X
					F501_13D[MONY_BUF_SIZE],			// 13.D 事業区分別の合計額 税率6.24%適用分 D
					F501_13E[MONY_BUF_SIZE],			// 13.E 事業区分別の合計額 税率7.8%適用分  E
					F501_13F[MONY_BUF_SIZE],			// 13.F 事業区分別の合計額 合計            F

					F501_14X[MONY_BUF_SIZE],			// 14.X 第一種事業(卸売業) 旧税率分小計    X
					F501_14D[MONY_BUF_SIZE],			// 14.D 第一種事業(卸売業) 税率6.24%適用分 D
					F501_14E[MONY_BUF_SIZE],			// 14.E 第一種事業(卸売業) 税率7.8%適用分  E
					F501_14F[MONY_BUF_SIZE],			// 14.F 第一種事業(卸売業) 合計            F

					F501_15X[MONY_BUF_SIZE],			// 15.X 第二種事業(小売業) 旧税率分小計    X
					F501_15D[MONY_BUF_SIZE],			// 15.D 第二種事業(小売業) 税率6.24%適用分 D
					F501_15E[MONY_BUF_SIZE],			// 15.E 第二種事業(小売業) 税率7.8%適用分  E
					F501_15F[MONY_BUF_SIZE],			// 15.F 第二種事業(小売業) 合計            F

					F501_16X[MONY_BUF_SIZE],			// 16.X 第三種事業(製造業等) 旧税率分小計    X
					F501_16D[MONY_BUF_SIZE],			// 16.D 第三種事業(製造業等) 税率6.24%適用分 D
					F501_16E[MONY_BUF_SIZE],			// 16.E 第三種事業(製造業等) 税率7.8%適用分  E
					F501_16F[MONY_BUF_SIZE],			// 16.F 第三種事業(製造業等) 合計            F

					F501_17X[MONY_BUF_SIZE],			// 17.X 第四種事業(その他) 旧税率分小計    X
					F501_17D[MONY_BUF_SIZE],			// 17.D 第四種事業(その他) 税率6.24%適用分 D
					F501_17E[MONY_BUF_SIZE],			// 17.E 第四種事業(その他) 税率7.8%適用分  E
					F501_17F[MONY_BUF_SIZE],			// 17.F 第四種事業(その他) 合計            F

					F501_18X[MONY_BUF_SIZE],			// 18.X 第五種事業(サービス業等) 旧税率分小計    X
					F501_18D[MONY_BUF_SIZE],			// 18.D 第五種事業(サービス業等) 税率6.24%適用分 D
					F501_18E[MONY_BUF_SIZE],			// 18.E 第五種事業(サービス業等) 税率7.8%適用分  E
					F501_18F[MONY_BUF_SIZE],			// 18.F 第五種事業(サービス業等) 合計            F

					F501_19X[MONY_BUF_SIZE],			// 19.X 第六種事業(不動産業) 旧税率分小計    X
					F501_19D[MONY_BUF_SIZE],			// 19.D 第六種事業(不動産業) 税率6.24%適用分 D
					F501_19E[MONY_BUF_SIZE],			// 19.E 第六種事業(不動産業) 税率7.8%適用分  E
					F501_19F[MONY_BUF_SIZE];			// 19.F 第六種事業(不動産業) 合計            F

	unsigned short	F501_7UP,							// 7.  第一種事業(卸売業)     売上割合
					F501_8UP,							// 8.  第二種事業(小売業)     売上割合
					F501_9UP,							// 9.  第三種事業(製造業等)   売上割合
					F501_10UP,							// 10. 第四種事業(その他)     売上割合
					F501_11UP,							// 11. 第五種事業(サービス業) 売上割合
					F501_12UP;							// 12. 第六種事業(不動産業)   売上割合

	int				m_curwnd;

	int				F501_6c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　６
					F501_14c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　14
					F501_15c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　15
					F501_16c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　16
					F501_17c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　17
					F501_18c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　18
					F501_19c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　19

					F501_7c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　7
					F501_8c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　8
					F501_9c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　9
					F501_10c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　10
					F501_11c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　11
					F501_12c;							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　12

	int				F501_5kbn;							// みなし仕入率　1:90％...
	int				typeCnt;							// 業種数
	EnumTokureiMode m_TokureiMode;						// 特例５－１


	CSnHeadData				**m_pSnHeadData;					// 消費税ヘッダ情報
	CH31SnFhyo5_01Data		**m_pSnFhyo5_01Data;
	CSyzSyukei				**m_pSyzSyukei;						// 消費税集計クラス

	CArithEx				*m_pArith;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo5_01Diag01;
	CIcsdiagctrl m_Fhyo5_01Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
