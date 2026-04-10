#pragma once
#include "icsdiagctrl.h"


// ShinFhyo5_03 ダイアログ

class CShinFhyo5_03 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo5_03)

public:
	CShinFhyo5_03(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo5_03();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY5_03 };
//#endif

public:
	// 初期情報のセット
	int InitInfo ( CH31SnFhyo5_01Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CSyzSyukei **pSyzSyukei );

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

	// ポジションセット
	void SetItemPosition();

	// 画面出力
	void DispFh5_03();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 売上割合表示
	void dsp_uws ( unsigned short Id, short Index, unsigned short pers );

	// 表示金額取得
	void GetAllDiagData();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int index, int Move );

	// 全項目表示
	void f503data_all ( int atno );
	// 課税標準額に対する消費税額
	void f503data_1();
	// 貸倒回収に係る消費税額
	void f503data_2();
	// 売上対価の返還等対価に係る税額
	void f503data_3();
	// 控除対象仕入税額の計算の基礎となる消費税額
	void f503data_4();
	// 1種類の事業の専業者の場合の控除対象仕入税額
	void f503data_5();
	// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
	// 事業区分別の合計額
	void f503data_6();
	// 第一種事業(卸売業)
	void f503data_7();
	// 第二種事業(小売業)
	void f503data_8();
	// 第三種事業(製造業等)
	void f503data_9();
	// 第四種事業(その他)
	void f503data_10();
	// 第五種事業(サービス業)
	void f503data_11();
	// 第六種事業(不動産業)
	void f503data_12();
	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	void f503data_13();
	// 第一種事業(卸売業)
	void f503data_14();
	// 第二種事業(小売業)
	void f503data_15();
	// 第三種事業(製造業等)
	void f503data_16();
	// 第四種事業(その他)
	void f503data_17();
	// 第五種事業(サービス業)
	void f503data_18();
	// 第六種事業(不動産業)
	void f503data_19();

	// 手入力項目あり？
	BOOL IsManualInputSign();

	int GetPercentage( int id, unsigned short *pWari, char *pArith );

private:
	CWnd*					m_pParent;		// 親ウィンドウ
	int						m_curwnd;

	char					F503_1A[MONY_BUF_SIZE],				// 1.A 課税標準額に対する消費税額 税率6.24%適用分 A
							F503_1B[MONY_BUF_SIZE],				// 1.B 課税標準額に対する消費税額 税率7.8%適用分  B
							F503_1C[MONY_BUF_SIZE],				// 1.C 課税標準額に対する消費税額 合計            C

							F503_2A[MONY_BUF_SIZE],				// 2.A 貸倒回収に係る消費税額 税率6.24%適用分 A
							F503_2B[MONY_BUF_SIZE],				// 2.B 貸倒回収に係る消費税額 税率7.8%適用分  B
							F503_2C[MONY_BUF_SIZE],				// 2.C 貸倒回収に係る消費税額 合計            C

							F503_3A[MONY_BUF_SIZE],				// 3.A 売上対価の返還等に係る消費税額 税率6.24%適用分 A
							F503_3B[MONY_BUF_SIZE],				// 3.B 売上対価の返還等に係る消費税額 税率7.8%適用分  B
							F503_3C[MONY_BUF_SIZE],				// 3.C 売上対価の返還等に係る消費税額 合計            C

							F503_4A[MONY_BUF_SIZE],				// 4.A 控除対象仕入税額の計算の基礎となる消費税額 税率6.24%適用分 A
							F503_4B[MONY_BUF_SIZE],				// 4.B 控除対象仕入税額の計算の基礎となる消費税額 税率7.8%適用分  B
							F503_4C[MONY_BUF_SIZE],				// 4.C控除対象仕入税額の計算の基礎となる消費税額 合計            C

							F503_5A[MONY_BUF_SIZE],				// 5.A ④×みなし仕入率 税率6.24%適用分 A
							F503_5B[MONY_BUF_SIZE],				// 5.B ④×みなし仕入率 税率7.8%適用分  B
							F503_5C[MONY_BUF_SIZE],				// 5.C ④×みなし仕入率 合計            C

							F503_6A[MONY_BUF_SIZE],				// 6.A 事業区分別の合計額 税率6.24%適用分 A
							F503_6B[MONY_BUF_SIZE],				// 6.B 事業区分別の合計額 税率7.8%適用分  B
							F503_6C[MONY_BUF_SIZE],				// 6.C 事業区分別の合計額 合計            C

							F503_7A[MONY_BUF_SIZE],				// 7.A 第一種事業(卸売業) 税率6.24%適用分 A
							F503_7B[MONY_BUF_SIZE],				// 7.B 第一種事業(卸売業) 税率7.8%適用分  B
							F503_7C[MONY_BUF_SIZE],				// 7.C 第一種事業(卸売業) 合計            C

							F503_8A[MONY_BUF_SIZE],				// 8.A 第二種事業(小売業) 税率6.24%適用分 A
							F503_8B[MONY_BUF_SIZE],				// 8.B 第二種事業(小売業) 税率7.8%適用分  B
							F503_8C[MONY_BUF_SIZE],				// 8.C 第二種事業(小売業) 合計            C

							F503_9A[MONY_BUF_SIZE],				// 9.A 第三種事業(製造業等) 税率6.24%適用分 A
							F503_9B[MONY_BUF_SIZE],				// 9.B 第三種事業(製造業等) 税率7.8%適用分  B
							F503_9C[MONY_BUF_SIZE],				// 9.C 第三種事業(製造業等) 合計            C

							F503_10A[MONY_BUF_SIZE],			// 10.A 第四種事業(その他) 税率6.24%適用分 A
							F503_10B[MONY_BUF_SIZE],			// 10.B 第四種事業(その他) 税率7.8%適用分  B
							F503_10C[MONY_BUF_SIZE],			// 10.C 第四種事業(その他) 合計            C

							F503_11A[MONY_BUF_SIZE],			// 11.A 第五種事業(サービス業等) 税率6.24%適用分 A
							F503_11B[MONY_BUF_SIZE],			// 11.B 第五種事業(サービス業等) 税率7.8%適用分  B
							F503_11C[MONY_BUF_SIZE],			// 11.C 第五種事業(サービス業等) 合計            C

							F503_12A[MONY_BUF_SIZE],			// 12.A 第六種事業(不動産業) 税率6.24%適用分 A
							F503_12B[MONY_BUF_SIZE],			// 12.B 第六種事業(不動産業) 税率7.8%適用分  B
							F503_12C[MONY_BUF_SIZE],			// 12.C 第六種事業(不動産業) 合計            C

							F503_13A[MONY_BUF_SIZE],			// 13.A 事業区分別の合計額 税率6.24%適用分 A
							F503_13B[MONY_BUF_SIZE],			// 13.B 事業区分別の合計額 税率7.8%適用分  B
							F503_13C[MONY_BUF_SIZE],			// 13.C 事業区分別の合計額 合計            C

							F503_14A[MONY_BUF_SIZE],			// 14.A 第一種事業(卸売業) 税率6.24%適用分 A
							F503_14B[MONY_BUF_SIZE],			// 14.B 第一種事業(卸売業) 税率7.8%適用分  B
							F503_14C[MONY_BUF_SIZE],			// 14.C 第一種事業(卸売業) 合計            C

							F503_15A[MONY_BUF_SIZE],			// 15.A 第二種事業(小売業) 税率6.24%適用分 A
							F503_15B[MONY_BUF_SIZE],			// 15.B 第二種事業(小売業) 税率7.8%適用分  B
							F503_15C[MONY_BUF_SIZE],			// 15.C 第二種事業(小売業) 合計            C

							F503_16A[MONY_BUF_SIZE],			// 16.A 第三種事業(製造業等) 税率6.24%適用分 A
							F503_16B[MONY_BUF_SIZE],			// 16.B 第三種事業(製造業等) 税率7.8%適用分  B
							F503_16C[MONY_BUF_SIZE],			// 16.C 第三種事業(製造業等) 合計            C

							F503_17A[MONY_BUF_SIZE],			// 17.A 第四種事業(その他) 税率6.24%適用分 A
							F503_17B[MONY_BUF_SIZE],			// 17.B 第四種事業(その他) 税率7.8%適用分  B
							F503_17C[MONY_BUF_SIZE],			// 17.C 第四種事業(その他) 合計            C

							F503_18A[MONY_BUF_SIZE],			// 18.A 第五種事業(サービス業等) 税率6.24%適用分 A
							F503_18B[MONY_BUF_SIZE],			// 18.B 第五種事業(サービス業等) 税率7.8%適用分  B
							F503_18C[MONY_BUF_SIZE],			// 18.C 第五種事業(サービス業等) 合計            C

							F503_19A[MONY_BUF_SIZE],			// 19.A 第六種事業(不動産業) 税率6.24%適用分 A
							F503_19B[MONY_BUF_SIZE],			// 19.B 第六種事業(不動産業) 税率7.8%適用分  B
							F503_19C[MONY_BUF_SIZE];			// 19.C 第六種事業(不動産業) 合計            C

	unsigned short			F503_7UP,							// 7.  第一種事業(卸売業)     売上割合
							F503_8UP,							// 8.  第二種事業(小売業)     売上割合
							F503_9UP,							// 9.  第三種事業(製造業等)   売上割合
							F503_10UP,							// 10. 第四種事業(その他)     売上割合
							F503_11UP,							// 11. 第五種事業(サービス業) 売上割合
							F503_12UP;							// 12. 第六種事業(不動産業)   売上割合

	int						F503_6c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　６
							F503_14c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　14
							F503_15c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　15
							F503_16c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　16
							F503_17c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　17
							F503_18c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　18
							F503_19c,							// 手入力サイン 事業区分別の課税売上高に係る消費税額の明細　19

							F503_7c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　7
							F503_8c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　8
							F503_9c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　9
							F503_10c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　10
							F503_11c,							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　11
							F503_12c;							// 手入力サイン 事業区分別の課税売上高(税抜き)の明細　12

	int						F503_5kbn;							// みなし仕入率　1:90％...
	int						typeCnt;							// 業種数

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Fhyo5_03Diag01;
	CIcsdiagctrl m_Fhyo5_03Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
