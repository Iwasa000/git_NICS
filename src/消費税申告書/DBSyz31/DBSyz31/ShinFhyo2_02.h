#pragma once
#include "icsdiagctrl.h"


// CShinFhyo2_02 ダイアログ

class CShinFhyo2_02 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo2_02)

public:
	CShinFhyo2_02(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo2_02();

	// 初期情報のセット
	int InitInfo (CH31SnFhyo2_02Data **pSnFhyo2_02Data, CSnHeadData **pSnHeadData, CH31SnTanaData **pSnTanaData , CArithEx * pArith, SYC_SHININFO *pShinInfo, CSyzSyukei **pSyzSyukei);

	// 初期化
	int Init();

	// ボタン表示変更
	void ChangeBtnDisp();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 初期ポジションセット
	void SetItemPosition ();

	// データの読込み
	int ReadData( int pas );
	void ReadData2();

	// データの書込み
	int WriteData( int pas );

	// 画面出力
	void DispFh2_02();

	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );

	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );

	// 各入力データ抽出
	void GetAllDiagData();

	// 特定課税仕入れに関連するインデックス
	int ChkTkkzIdx ( short idx );

	// 集計モジュールを更新
	int SyukeiDataSet(int cID);
	int MemoryReget();

	// --------- 表示 --------------

	// 課税売上額
	void f202_1data();
	// 課税資産の譲渡等の対価の額
	void f202_45data();
	// 非課税売上額
	void f202_6data();
	// 資産の譲渡等の対価の額
	void f202_7data();
	//課税売上割合
	void f202_8data();
	// 課税仕入に係る支払対価の額
	void f202_9data();
	// 課税仕入に係る消費税額
	void f202_10data();
	//特定課税仕入れに係る支払対価の額
	void f202_11data();
	//特定課税仕入れに係る消費税額
	void f202_12data();
	// 課税貨物に係る消費税額
	void f202_13data();
	// 課税事業⇔免税事業 調整税額
	void f202_14data();
	// 課税仕入等の税額の合計額
	void f202_15data();
	// 課税売上割合が９５％以上の場合
	void f202_16data();
	// 課税売上にのみ要する仕入税額
	void f202_17data();
	// 共通の売上に要する仕入税額
	void f202_18data();
	// 個別対応方式控除仕入税額
	void f202_19data();
	// 比例配分方式控除仕入税額
	void f202_20data();
	// 課税売上割合が９５％未満の場合
	void f202_ko();
	// 控除税額の調整(21)
	void f202_21data();
	// 控除税額の調整(22)
	void f202_22data();
	//	差引控除仕入税額の計算
	void f202_2324data();
	// 回収に係る消費税額
	void f202_25data();
	// 他表からの計算
	void calq_by_OtherChange();
	// 特定収入を加味した控除対象仕入税額の計算
	void f202_spc ( SYZTOSPC_DATA SpcData );


	// 各入力のデータ表示
	void f202data_all(int atno);

	// ５億円チェック（金額チェック部分）
	int CheckOver500MillionSub(int &MilSw, char *pVAL, char *pValCnv);
	// 課税売上高5億円越えチェック
	void CheckOver500Million();

	// 棚卸データ（ダイアログ）の加減算を行う
	void TanaDataAdd();

	// -----------------------------

	// 入力許可・不許可 切替
	void ModifyDiagInput(short index, int sign);

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd*			m_pParent;						// 親ウィンドウ

	char			F202_1A[MONY_BUF_SIZE],			// 1.A 課税売上額(税抜き) 税率3%適用分   A
					F202_1B[MONY_BUF_SIZE],			// 1.B 課税売上額(税抜き) 税率4%適用分   B
					F202_1C[MONY_BUF_SIZE],			// 1.C 課税売上額(税抜き) 税率6.3%適用分 C
					F202_1X[MONY_BUF_SIZE],			// 1.X 課税売上額(税抜き) 旧税率分小計   X (A+B+C)

					F202_4X[MONY_BUF_SIZE],			// 4.X 海外資産の譲渡等の対価の額 旧税率分小計   X (A+B+C)

					F202_7X[MONY_BUF_SIZE],			// 7.X 資産の譲渡等の対価の額 旧税率分小計   X (A+B+C)

					//F202_8X[MONY_BUF_SIZE],			// 8.X 課税売上割合 旧税率分小計   X (A+B+C)

					F202_9A[MONY_BUF_SIZE],			// 9.A 課税仕入れに係る支払い対価の額(税込み) 税率3%適用分   A
					F202_9B[MONY_BUF_SIZE],			// 9.B 課税仕入れに係る支払い対価の額(税込み) 税率4%適用分   B
					F202_9C[MONY_BUF_SIZE],			// 9.C 課税仕入れに係る支払い対価の額(税込み) 税率6.3%適用分 C
					F202_9X[MONY_BUF_SIZE],			// 9.X 課税仕入れに係る支払い対価の額(税込み) 旧税率分小計   X (A+B+C)

					F202_10A[MONY_BUF_SIZE],		// 10.A 課税仕入れに係る消費税額 税率3%適用分   A
					F202_10B[MONY_BUF_SIZE],		// 10.B 課税仕入れに係る消費税額 税率4%適用分   B
					F202_10C[MONY_BUF_SIZE],		// 10.C 課税仕入れに係る消費税額 税率6.3%適用分 C
					F202_10X[MONY_BUF_SIZE],		// 10.X 課税仕入れに係る消費税額 旧税率分小計   X (A+B+C)

					F202_11C[MONY_BUF_SIZE],		// 11.C 特定課税仕入れに係る支払い対価の額 税率6.3%適用分 C
					F202_11X[MONY_BUF_SIZE],		// 11.X 特定課税仕入れに係る支払い対価の額 旧税率分小計   X (A+B+C)

					F202_12C[MONY_BUF_SIZE],		// 12.C 特定課税仕入れに係る消費税額 税率6.3%適用分 C
					F202_12X[MONY_BUF_SIZE],		// 12.X 特定課税仕入れに係る消費税額 旧税率分小計   X (A+B+C)

					// 3% 4%は仕訳が発生することがないためカット
					//F202_13A[MONY_BUF_SIZE],		// 13.A 課税貨物に係る消費税額 税率3%適用分   A
					//F202_13B[MONY_BUF_SIZE],		// 13.B 課税貨物に係る消費税額 税率4%適用分   B
					F202_13C[MONY_BUF_SIZE],		// 13.C 課税貨物に係る消費税額 税率6.3%適用分 C
					F202_13X[MONY_BUF_SIZE],		// 13.X 課税貨物に係る消費税額 旧税率分小計   X (A+B+C)

					F202_14A[MONY_BUF_SIZE],		// 14.A 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率3%適用分   A
					F202_14B[MONY_BUF_SIZE],		// 14.B 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率4%適用分   B
					F202_14C[MONY_BUF_SIZE],		// 14.C 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率6.3%適用分 C
					F202_14X[MONY_BUF_SIZE],		// 14.X 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 旧税率分小計   X (A+B+C)

					F202_15A[MONY_BUF_SIZE],		// 15.A 課税仕入れ等の税額の合計額 税率3%適用分   A
					F202_15B[MONY_BUF_SIZE],		// 15.B 課税仕入れ等の税額の合計額 税率4%適用分   B
					F202_15C[MONY_BUF_SIZE],		// 15.C 課税仕入れ等の税額の合計額 税率6.3%適用分 C
					F202_15X[MONY_BUF_SIZE],		// 15.X 課税仕入れ等の税額の合計額 旧税率分小計   X (A+B+C)

					F202_16A[MONY_BUF_SIZE],		// 16.A 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 税率3%適用分   A
					F202_16B[MONY_BUF_SIZE],		// 16.B 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 税率4%適用分   B
					F202_16C[MONY_BUF_SIZE],		// 16.C 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 税率6.3%適用分 C
					F202_16X[MONY_BUF_SIZE],		// 16.X 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 旧税率分小計   X (A+B+C)

					F202_17A[MONY_BUF_SIZE],		// 17.A 15のうち、課税売上にのみ要するもの 税率3%適用分   A
					F202_17B[MONY_BUF_SIZE],		// 17.B 15のうち、課税売上にのみ要するもの 税率4%適用分   B
					F202_17C[MONY_BUF_SIZE],		// 17.C 15のうち、課税売上にのみ要するもの 税率6.3%適用分 C
					F202_17X[MONY_BUF_SIZE],		// 17.X 15のうち、課税売上にのみ要するもの 旧税率分小計   X (A+B+C)

					F202_18A[MONY_BUF_SIZE],		// 18.A 15のうち、課税売上と非課税売り上げに共通して要するもの 税率3%適用分   A
					F202_18B[MONY_BUF_SIZE],		// 18.B 15のうち、課税売上と非課税売り上げに共通して要するもの 税率4%適用分   B
					F202_18C[MONY_BUF_SIZE],		// 18.C 15のうち、課税売上と非課税売り上げに共通して要するもの 税率6.3%適用分 C
					F202_18X[MONY_BUF_SIZE],		// 18.X 15のうち、課税売上と非課税売り上げに共通して要するもの 旧税率分小計   X (A+B+C)

					F202_19A[MONY_BUF_SIZE],		// 19.A 個別対応方式により控除する課税仕入れ等の税額 税率3%適用分   A
					F202_19B[MONY_BUF_SIZE],		// 19.B 個別対応方式により控除する課税仕入れ等の税額 税率4%適用分   B
					F202_19C[MONY_BUF_SIZE],		// 19.C 個別対応方式により控除する課税仕入れ等の税額 税率6.3%適用分 C
					F202_19X[MONY_BUF_SIZE],		// 19.X 個別対応方式により控除する課税仕入れ等の税額 旧税率分小計   X (A+B+C)

					F202_20A[MONY_BUF_SIZE],		// 20.A 一括比例配分方式により控除する課税仕入れ等の税額 税率3%適用分   A
					F202_20B[MONY_BUF_SIZE],		// 20.B 一括比例配分方式により控除する課税仕入れ等の税額 税率4%適用分   B
					F202_20C[MONY_BUF_SIZE],		// 20.C 一括比例配分方式により控除する課税仕入れ等の税額 税率6.3%適用分 C
					F202_20X[MONY_BUF_SIZE],		// 20.X 一括比例配分方式により控除する課税仕入れ等の税額 旧税率分小計   X (A+B+C)

					F202_21A[MONY_BUF_SIZE],		// 21.A 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率3%適用分   A
					F202_21B[MONY_BUF_SIZE],		// 21.B 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率4%適用分   B
					F202_21C[MONY_BUF_SIZE],		// 21.C 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率6.3%適用分 C
					F202_21X[MONY_BUF_SIZE],		// 21.X 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 旧税率分小計   X (A+B+C)

					F202_22A[MONY_BUF_SIZE],		// 22.A 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率3%適用分   A
					F202_22B[MONY_BUF_SIZE],		// 22.B 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率4%適用分   B
					F202_22C[MONY_BUF_SIZE],		// 22.C 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率6.3%適用分 C
					F202_22X[MONY_BUF_SIZE],		// 22.X 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 旧税率分小計   X (A+B+C)

					F202_23A[MONY_BUF_SIZE],		// 23.A 控除対象仕入税額 税率3%適用分   A
					F202_23B[MONY_BUF_SIZE],		// 23.B 控除対象仕入税額 税率4%適用分   B
					F202_23C[MONY_BUF_SIZE],		// 23.C 控除対象仕入税額 税率6.3%適用分 C
					F202_23X[MONY_BUF_SIZE],		// 23.X 控除対象仕入税額 旧税率分小計   X (A+B+C)

					F202_24A[MONY_BUF_SIZE],		// 24.A 控除過大調整税額 税率3%適用分   A
					F202_24B[MONY_BUF_SIZE],		// 24.B 控除過大調整税額 税率4%適用分   B
					F202_24C[MONY_BUF_SIZE],		// 24.C 控除過大調整税額 税率6.3%適用分 C
					F202_24X[MONY_BUF_SIZE],		// 24.X 控除過大調整税額 旧税率分小計   X (A+B+C)

					F202_25A[MONY_BUF_SIZE],		// 25.A 貸倒回収に係る消費税額 税率3%適用分   A
					F202_25B[MONY_BUF_SIZE],		// 25.B 貸倒回収に係る消費税額 税率4%適用分   B
					F202_25C[MONY_BUF_SIZE],		// 25.C 貸倒回収に係る消費税額 税率6.3%適用分 C
					F202_25X[MONY_BUF_SIZE];		// 25.X 貸倒回収に係る消費税額 旧税率分小計   X (A+B+C)

	char			F201_1D[MONY_BUF_SIZE],			// 付表２-１　1.D 課税売上(税抜き) 税率6.24％適用分　D
					F201_1E[MONY_BUF_SIZE],			// 付表２-１　1.E 課税売上(税抜き) 税率7.8％適用分　E
					F201_2F[MONY_BUF_SIZE];			// 付表２-１　2.F 免税売上額 合計 F

	unsigned short	F202_8EX;						// 8.X課税売上割合
	int				F202_8X;						// 手入力された課税売上割合

	int				m_curwnd;

	CSnHeadData		**m_pSnHeadData;				// 消費税ヘッダ情報
	CH31SnTanaData	**m_pSnTanaData;				// 棚卸調整データ
	SYC_SHININFO	*m_pShinInfo;					// 共通情報
	CH31SnFhyo2_02Data	**m_pSnFhyo2_02Data;
	CArithEx		*m_pArith;
	CSyzUtil		m_Util;
	CSyzSyukei		**m_pSyzSyukei;						// 消費税集計クラス

	int				F202_10c, F202_12c, F202_19c, F202_20c, F202_23c;	// 手入力サイン
	int				m_FrSw;						// ??? 元処理引用
	int				m_DpSw;						// ??? 元処理引用
	int				m_DlSw;						// ??? 元処理引用

	BOOL			m_SpcSgn;					// 特定収入転記サイン

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY2_02 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo2_02Diag02;
	CIcsdiagctrl m_Fhyo2_02Diag01;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_tanaoroshi;
	afx_msg void OnBnClickedButton1();
	void CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data);
	afx_msg void OnBnClickedButton2();
	CButton m_Button2;
};
