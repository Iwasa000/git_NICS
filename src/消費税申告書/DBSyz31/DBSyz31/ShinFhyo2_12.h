#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"

// ShinFhyo2_03 ダイアログ

class CShinFhyo2_12 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo2_12)

public:
	CShinFhyo2_12(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo2_12();

	// ダイアログ データ
	//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY2_12 };
	//#endif


public:
	// 初期情報のセット
	int	 InitInfo ( CH31SnFhyo2_02Data **pSnFhyo2_02Data, CSnHeadData **pSnHeadData, CH31SnTanaData **pSnTanaData , CArithEx *pArith, SYC_SHININFO *pShinInfo, CSyzSyukei **pSyzSyukei );

	// 初期化
	int	 Init();

	// データ読込み
	int ReadData();

	// 読込みデータを集計モジュールへ
	void ReadData2();

	// データ書込み
	int WriteData();

	// 集計処理
	int MemoryReget();

	// 集計モジュールを更新
	int SyukeiDataSet ( int cID );

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 特定課税仕入れに関連するインデックス
	int ChkTkkzIdx ( short idx );

	// 画面出力
	void DispFh2_12();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 課税売上高5億円越えチェック
	void CheckOver500Million();
	// ５億円チェック（金額チェック部分）
	int CheckOver500MillionSub( int &MilSw, char *pVAL, char *pValCnv );

	// 全項目表示
	void F212data_all ( int atno );
	// 課税売上(税抜き)表示
	void F212data_1();
	// 課税資産の譲渡等の対価の額表示
	void F212data_4();
	// 資産の譲渡等の対価の額表示
	void F212data_7();
	// 課税売上割合表示
	void F212data_8();
	// 課税仕入れに係る支払対価の額(税込み)表示
	void F212data_9();
	// 課税仕入れに係る消費税額表示
	void F212data_10();
	// 特定課税仕入れに係る支払対価の額表示
	void F212data_11();
	// 特定課税仕入れに係る消費税額表示
	void F212data_12();
	// 課税貨物に係る消費税額表示
	void F212data_13();
	// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額表示
	void F212data_14();
	// 課税仕入れ等の税額の合計額表示
	void F212data_15();
	// 課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合表示
	void F212data_16();
	// 15のうち、課税売上げにのみ要するもの表示
	void F212data_17();
	// 15のうち、課税売上げと非課税売上げに共通して要するもの表示
	void F212data_18();
	// 個別対応方式により控除する課税仕入れ等の税額表示
	void F212data_19();
	// 一括比例配分方式により控除する課税仕入れ等の税額表示
	void F212data_20();
	// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額表示
	void F212data_21();
	// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額表示
	void F212data_22();
	// 控除対象仕入税額がプラスの時表示
	void F212data_23();
	// 控除対象仕入税額がマイナスの時表示
	void F212data_24();
	// 貸倒回収に係る消費税額表示
	void F212data_25();
	// 居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額表示
	void F212data_26();

	// 課税売上割合を変更
	void ChangeF212data_8();

	// 棚卸データの加減算を行う
	void TanaDataAdd();

	// 表示金額取得
	void GetAllDiagData();

	// ボタン表示変更
	void ChangeBtnDisp();

	// 初期ポジションセット
	void SetItemPosition ();

	// フォーカス移動先取得
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 入力許可・不許可 切替
	void ModifyDiagInput ( short index, int sign );

	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

	// 特定収入を加味した控除対象仕入税額の計算
	void F212calq_spc ( SYZTOSPC_DATA SpcData );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd*				m_pParent;		// 親ウィンドウ
	int					m_curwnd;

		char			F212_1A[MONY_BUF_SIZE],			// 1.A 課税売上額(税抜き) 税率3%適用分   A
						F212_1B[MONY_BUF_SIZE],			// 1.B 課税売上額(税抜き) 税率4%適用分   B
						F212_1C[MONY_BUF_SIZE],			// 1.C 課税売上額(税抜き) 税率6.3%適用分 C
						F212_1X[MONY_BUF_SIZE],			// 1.X 課税売上額(税抜き) 旧税率分小計   X (A+B+C)

						F212_4X[MONY_BUF_SIZE],			// 4.X 海外資産の譲渡等の対価の額 旧税率分小計   X (A+B+C)

						F212_7X[MONY_BUF_SIZE],			// 7.X 資産の譲渡等の対価の額 旧税率分小計   X (A+B+C)

						F212_9A[MONY_BUF_SIZE],			// 9.A 課税仕入れに係る支払い対価の額(税込み) 税率3%適用分   A
						F212_9B[MONY_BUF_SIZE],			// 9.B 課税仕入れに係る支払い対価の額(税込み) 税率4%適用分   B
						F212_9C[MONY_BUF_SIZE],			// 9.C 課税仕入れに係る支払い対価の額(税込み) 税率6.3%適用分 C
						F212_9X[MONY_BUF_SIZE],			// 9.X 課税仕入れに係る支払い対価の額(税込み) 旧税率分小計   X (A+B+C)

						F212_10A[MONY_BUF_SIZE],		// 10.A 課税仕入れに係る消費税額 税率3%適用分   A
						F212_10B[MONY_BUF_SIZE],		// 10.B 課税仕入れに係る消費税額 税率4%適用分   B
						F212_10C[MONY_BUF_SIZE],		// 10.C 課税仕入れに係る消費税額 税率6.3%適用分 C
						F212_10X[MONY_BUF_SIZE],		// 10.X 課税仕入れに係る消費税額 旧税率分小計   X (A+B+C)

						F212_11C[MONY_BUF_SIZE],		// 11.C 特定課税仕入れに係る支払い対価の額 税率6.3%適用分 C
						F212_11X[MONY_BUF_SIZE],		// 11.X 特定課税仕入れに係る支払い対価の額 旧税率分小計   X (A+B+C)

						F212_12C[MONY_BUF_SIZE],		// 12.C 特定課税仕入れに係る消費税額 税率6.3%適用分 C
						F212_12X[MONY_BUF_SIZE],		// 12.X 特定課税仕入れに係る消費税額 旧税率分小計   X (A+B+C)

						F212_13C[MONY_BUF_SIZE],		// 13.C 課税貨物に係る消費税額 税率6.3%適用分 C
						F212_13X[MONY_BUF_SIZE],		// 13.X 課税貨物に係る消費税額 旧税率分小計   X (A+B+C)

						F212_14A[MONY_BUF_SIZE],		// 14.A 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率3%適用分   A
						F212_14B[MONY_BUF_SIZE],		// 14.B 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率4%適用分   B
						F212_14C[MONY_BUF_SIZE],		// 14.C 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率6.3%適用分 C
						F212_14X[MONY_BUF_SIZE],		// 14.X 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 旧税率分小計   X (A+B+C)

						F212_15A[MONY_BUF_SIZE],		// 15.A 課税仕入れ等の税額の合計額 税率3%適用分   A
						F212_15B[MONY_BUF_SIZE],		// 15.B 課税仕入れ等の税額の合計額 税率4%適用分   B
						F212_15C[MONY_BUF_SIZE],		// 15.C 課税仕入れ等の税額の合計額 税率6.3%適用分 C
						F212_15X[MONY_BUF_SIZE],		// 15.X 課税仕入れ等の税額の合計額 旧税率分小計   X (A+B+C)

						F212_16A[MONY_BUF_SIZE],		// 16.A 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 税率3%適用分   A
						F212_16B[MONY_BUF_SIZE],		// 16.B 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 税率4%適用分   B
						F212_16C[MONY_BUF_SIZE],		// 16.C 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 税率6.3%適用分 C
						F212_16X[MONY_BUF_SIZE],		// 16.X 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合 旧税率分小計   X (A+B+C)

						F212_17A[MONY_BUF_SIZE],		// 17.A 15のうち、課税売上にのみ要するもの 税率3%適用分   A
						F212_17B[MONY_BUF_SIZE],		// 17.B 15のうち、課税売上にのみ要するもの 税率4%適用分   B
						F212_17C[MONY_BUF_SIZE],		// 17.C 15のうち、課税売上にのみ要するもの 税率6.3%適用分 C
						F212_17X[MONY_BUF_SIZE],		// 17.X 15のうち、課税売上にのみ要するもの 旧税率分小計   X (A+B+C)

						F212_18A[MONY_BUF_SIZE],		// 18.A 15のうち、課税売上と非課税売り上げに共通して要するもの 税率3%適用分   A
						F212_18B[MONY_BUF_SIZE],		// 18.B 15のうち、課税売上と非課税売り上げに共通して要するもの 税率4%適用分   B
						F212_18C[MONY_BUF_SIZE],		// 18.C 15のうち、課税売上と非課税売り上げに共通して要するもの 税率6.3%適用分 C
						F212_18X[MONY_BUF_SIZE],		// 18.X 15のうち、課税売上と非課税売り上げに共通して要するもの 旧税率分小計   X (A+B+C)

						F212_19A[MONY_BUF_SIZE],		// 19.A 個別対応方式により控除する課税仕入れ等の税額 税率3%適用分   A
						F212_19B[MONY_BUF_SIZE],		// 19.B 個別対応方式により控除する課税仕入れ等の税額 税率4%適用分   B
						F212_19C[MONY_BUF_SIZE],		// 19.C 個別対応方式により控除する課税仕入れ等の税額 税率6.3%適用分 C
						F212_19X[MONY_BUF_SIZE],		// 19.X 個別対応方式により控除する課税仕入れ等の税額 旧税率分小計   X (A+B+C)

						F212_20A[MONY_BUF_SIZE],		// 20.A 一括比例配分方式により控除する課税仕入れ等の税額 税率3%適用分   A
						F212_20B[MONY_BUF_SIZE],		// 20.B 一括比例配分方式により控除する課税仕入れ等の税額 税率4%適用分   B
						F212_20C[MONY_BUF_SIZE],		// 20.C 一括比例配分方式により控除する課税仕入れ等の税額 税率6.3%適用分 C
						F212_20X[MONY_BUF_SIZE],		// 20.X 一括比例配分方式により控除する課税仕入れ等の税額 旧税率分小計   X (A+B+C)

						F212_21A[MONY_BUF_SIZE],		// 21.A 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率3%適用分   A
						F212_21B[MONY_BUF_SIZE],		// 21.B 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率4%適用分   B
						F212_21C[MONY_BUF_SIZE],		// 21.C 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率6.3%適用分 C
						F212_21X[MONY_BUF_SIZE],		// 21.X 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 旧税率分小計   X (A+B+C)

						F212_22A[MONY_BUF_SIZE],		// 22.A 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率3%適用分   A
						F212_22B[MONY_BUF_SIZE],		// 22.B 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率4%適用分   B
						F212_22C[MONY_BUF_SIZE],		// 22.C 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率6.3%適用分 C
						F212_22X[MONY_BUF_SIZE],		// 22.X 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 旧税率分小計   X (A+B+C)

						F212_26A[MONY_BUF_SIZE],		// 23.A	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率3%適用分   A
						F212_26B[MONY_BUF_SIZE],		// 23.B	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率4%適用分   B
						F212_26C[MONY_BUF_SIZE],		// 23.C	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率6.3%適用分 C
						F212_26X[MONY_BUF_SIZE],		// 23.X	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 旧税率分小計   X (A+B+C)

						F212_23A[MONY_BUF_SIZE],		// 24.A 控除対象仕入税額 税率3%適用分   A
						F212_23B[MONY_BUF_SIZE],		// 24.B 控除対象仕入税額 税率4%適用分   B
						F212_23C[MONY_BUF_SIZE],		// 24.C 控除対象仕入税額 税率6.3%適用分 C
						F212_23X[MONY_BUF_SIZE],		// 24.X 控除対象仕入税額 旧税率分小計   X (A+B+C)

						F212_24A[MONY_BUF_SIZE],		// 25.A 控除過大調整税額 税率3%適用分   A
						F212_24B[MONY_BUF_SIZE],		// 25.B 控除過大調整税額 税率4%適用分   B
						F212_24C[MONY_BUF_SIZE],		// 25.C 控除過大調整税額 税率6.3%適用分 C
						F212_24X[MONY_BUF_SIZE],		// 25.X 控除過大調整税額 旧税率分小計   X (A+B+C)

						F212_25A[MONY_BUF_SIZE],		// 26.A 貸倒回収に係る消費税額 税率3%適用分   A
						F212_25B[MONY_BUF_SIZE],		// 26.B 貸倒回収に係る消費税額 税率4%適用分   B
						F212_25C[MONY_BUF_SIZE],		// 26.C 貸倒回収に係る消費税額 税率6.3%適用分 C
						F212_25X[MONY_BUF_SIZE];		// 26.X 貸倒回収に係る消費税額 旧税率分小計   X (A+B+C)

	char				F211_1D[MONY_BUF_SIZE],			// 付表２-１　1.D 課税売上(税抜き) 税率6.24％適用分　D
						F211_1E[MONY_BUF_SIZE],			// 付表２-１　1.E 課税売上(税抜き) 税率7.8％適用分　E
						F211_2F[MONY_BUF_SIZE];			// 付表２-１　2.F 免税売上額 合計 F

	unsigned short		F212_8EX;						// 8.F課税売上割合
	int					F212_8X;						// 手入力された課税売上割合

	int					F212_10c,
						F212_12c,
						F212_19c,
						F212_20c,
						F212_23c;

	long				eymd;

	CH31SnFhyo2_02Data	**m_pSnFhyo2_02Data;
	CH31SnTanaData		**m_pSnTanaData;				// 棚卸調整データ
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報

	SYC_SHININFO		*m_pShinInfo;					// 共通情報
	CArithEx			*m_pArith;						// 倍長演算
	CSyzSyukei			**m_pSyzSyukei;					// 消費税集計クラス
	CSyzUtil			m_Util;

	BOOL				m_SpcSgn;						// 特定収入転記サイン



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo2_12Diag01;
	CIcsdiagctrl m_Fhyo2_12Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_tanaoroshi;
	CButton m_Button2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
