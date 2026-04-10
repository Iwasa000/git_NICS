#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"

// ShinFhyo2_03 ダイアログ

class CShinFhyo2_03 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo2_03)

public:
	CShinFhyo2_03(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo2_03();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY2_03 };
//#endif


public:
	// 初期情報のセット
	int	 InitInfo ( CH31SnFhyo2_01Data **pSnFhyo2_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH31SnTanaData **pSnTanaData, CSyzSyukei **pSyzSyukei );

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

	// 特例計算に関連するインデックス
	int ChkTokureiIdx ( short id );

	// 画面出力
	void DispFh2_03();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 課税売上高5億円越えチェック
	void CheckOver500Million();
	// ５億円チェック（金額チェック部分）
	int CheckOver500MillionSub( int &MilSw, char *pVAL, char *pValCnv );

	// 全項目表示
	void f203data_all ( int atno );
	// 課税売上(税抜き)表示
	void f203data_1();
	// 免税売上額表示
	void f203data_2();
	// 非課税資産の輸出等の金額、海外支店等へ移送した資産の価額表示
	void f203data_3();
	// 課税資産の譲渡等の対価の額表示
	void f203data_4_5();
	// 非課税売上額表示
	void f203data_6();
	// 資産の譲渡等の対価の額表示
	void f203data_7();
	// 課税売上割合表示
	void f203data_8();
	// 課税仕入れに係る支払対価の額(税込み)表示
	void f203data_9();
	// 課税仕入れに係る消費税額表示
	void f203data_10();
	// 特定課税仕入れに係る支払対価の額表示
	void f203data_11();
	// 特定課税仕入れに係る消費税額表示
	void f203data_12();
	// 課税貨物に係る消費税額表示
	void f203data_13();
	// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額表示
	void f203data_14();
	// 課税仕入れ等の税額の合計額表示
	void f203data_15();
	// 課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合表示
	void f203data_16();
	// 15のうち、課税売上げにのみ要するもの表示
	void f203data_17();
	// 15のうち、課税売上げと非課税売上げに共通して要するもの表示
	void f203data_18();
	// 個別対応方式により控除する課税仕入れ等の税額表示
	void f203data_19();
	// 一括比例配分方式により控除する課税仕入れ等の税額表示
	void f203data_20();
	// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額表示
	void f203data_21();
	// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額表示
	void f203data_22();
	// 控除対象仕入税額がプラスの時表示
	void f203data_23();
	// 控除対象仕入税額がマイナスの時表示
	void f203data_24();
	// 貸倒回収に係る消費税額表示
	void f203data_25();
	// 居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額表示
	void f203data_26();

	// 課税売上割合を変更
	void Changef203data_8();

	// 棚卸データの加減算を行う
	void TanaDataAdd();

	// 表示金額取得
	void GetAllDiagData();

	// 課税売上割合の取得
	unsigned short GetCalqedKzwari();

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
	void f203calq_spc ( SYZTOSPC_DATA SpcData );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd*				m_pParent;		// 親ウィンドウ
	int					m_curwnd;

	char				F203_1A[MONY_BUF_SIZE],			// 1.A	課税売上(税抜き) 税率6.24％適用分 A
						F203_1B[MONY_BUF_SIZE],			// 1.B	課税売上(税抜き) 税率7.8％適用分　B
						F203_1C[MONY_BUF_SIZE],			// 1.C	課税売上(税抜き) 合計 C

						F203_2C[MONY_BUF_SIZE],			// 2.C	免税売上額 合計 C

						F203_3C[MONY_BUF_SIZE],			// 3.C	非課税資産の輸出等の金額、海外支店等へ移送した資産の価額 合計 C

						F203_4C[MONY_BUF_SIZE],			// 4.C	課税資産の譲渡等の対価の額(1+2+3) 合計 C

						F203_5C[MONY_BUF_SIZE],			// 5.C	課税資産の譲渡等の対価の額(4の金額) 合計 C

						F203_6C[MONY_BUF_SIZE],			// 6.C	非課税売上額 合計 C

						F203_7C[MONY_BUF_SIZE],			// 7.C	資産の譲渡等の対価の額(5+6) 合計 C

						F203_9A[MONY_BUF_SIZE],			// 9.A	課税仕入れに係る支払対価の額(税込み) 税率6.24％適用分　A
						F203_9B[MONY_BUF_SIZE],			// 9.B	課税仕入れに係る支払対価の額(税込み) 税率7.8％適用分　B
						F203_9C[MONY_BUF_SIZE],			// 9.C	課税仕入れに係る支払対価の額(税込み) 合計 C

						F203_10A[MONY_BUF_SIZE],		// 10.A	課税仕入れに係る消費税額 税率6.24％適用分　A
						F203_10B[MONY_BUF_SIZE],		// 10.B	課税仕入れに係る消費税額 税率7.8％適用分　B
						F203_10C[MONY_BUF_SIZE],		// 10.C	課税仕入れに係る消費税額 合計 C

						F203_11B[MONY_BUF_SIZE],		// 11.B	特定課税仕入れに係る支払対価の額 税率7.8％適用分　B
						F203_11C[MONY_BUF_SIZE],		// 11.C	特定課税仕入れに係る支払対価の額 合計 C

						F203_12B[MONY_BUF_SIZE],		// 12.B	特定課税仕入れに係る消費税額 税率7.8％適用分　B
						F203_12C[MONY_BUF_SIZE],		// 12.C	特定課税仕入れに係る消費税額 合計 C

						F203_13A[MONY_BUF_SIZE],		// 13.A	課税貨物に係る消費税額 税率6.24％適用分　A
						F203_13B[MONY_BUF_SIZE],		// 13.B	課税貨物に係る消費税額 税率7.8％適用分　B
						F203_13C[MONY_BUF_SIZE],		// 13.C	課税貨物に係る消費税額 合計 C

						F203_14A[MONY_BUF_SIZE],		// 14.A	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率6.24％適用分　A
						F203_14B[MONY_BUF_SIZE],		// 14.B	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率7.8％適用分 B
						F203_14C[MONY_BUF_SIZE],		// 14.C	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 合計 C

						F203_15A[MONY_BUF_SIZE],		// 15.A	課税仕入れ等の税額の合計額(10+12+13±14) 税率6.24％適用分　A
						F203_15B[MONY_BUF_SIZE],		// 15.B	課税仕入れ等の税額の合計額(10+12+13±14) 税率7.8％適用分　B
						F203_15C[MONY_BUF_SIZE],		// 15.C	課税仕入れ等の税額の合計額(10+12+13±14) 合計 C

						F203_16A[MONY_BUF_SIZE],		// 16.A	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 税率6.24％適用分　A
						F203_16B[MONY_BUF_SIZE],		// 16.B	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 税率7.8％適用分　B
						F203_16C[MONY_BUF_SIZE],		// 16.C	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 合計 C

						F203_17A[MONY_BUF_SIZE],		// 17.A	15のうち、課税売上げにのみ要するもの 税率6.24％適用分　A
						F203_17B[MONY_BUF_SIZE],		// 17.B	15のうち、課税売上げにのみ要するもの 税率7.8％適用分　B
						F203_17C[MONY_BUF_SIZE],		// 17.C	15のうち、課税売上げにのみ要するもの 合計 C

						F203_18A[MONY_BUF_SIZE],		// 18.A	15のうち、課税売上げと非課税売上げに共通して要するもの 税率6.24％適用分　A
						F203_18B[MONY_BUF_SIZE],		// 18.B	15のうち、課税売上げと非課税売上げに共通して要するもの 税率7.8％適用分　B
						F203_18C[MONY_BUF_SIZE],		// 18.C	15のうち、課税売上げと非課税売上げに共通して要するもの 合計 C

						F203_19A[MONY_BUF_SIZE],		// 19.A	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率6.24％適用分　A
						F203_19B[MONY_BUF_SIZE],		// 19.B	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率7.8％適用分　B
						F203_19C[MONY_BUF_SIZE],		// 19.C	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 合計 C

						F203_20A[MONY_BUF_SIZE],		// 20.A	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率6.24％適用分　A
						F203_20B[MONY_BUF_SIZE],		// 20.B	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率7.8％適用分　B
						F203_20C[MONY_BUF_SIZE],		// 20.C	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 合計 C

						F203_21A[MONY_BUF_SIZE],		// 21.A	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率6.24％適用分　A
						F203_21B[MONY_BUF_SIZE],		// 21.B	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率7.8％適用分　B
						F203_21C[MONY_BUF_SIZE],		// 21.C	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 合計 C

						F203_22A[MONY_BUF_SIZE],		// 22.A	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率6.24％適用分　A
						F203_22B[MONY_BUF_SIZE],		// 22.B	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率7.8％適用分　B
						F203_22C[MONY_BUF_SIZE],		// 22.C	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 合計 C

						F203_26A[MONY_BUF_SIZE],		// 23.A	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率6.24％適用分 A
						F203_26B[MONY_BUF_SIZE],		// 23.B	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率7.8％適用分　B
						F203_26C[MONY_BUF_SIZE],		// 23.C	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 合計 C

						F203_23A[MONY_BUF_SIZE],		// 24.A	控除対象仕入税額がプラスの時 税率6.24％適用分　A
						F203_23B[MONY_BUF_SIZE],		// 24.B	控除対象仕入税額がプラスの時 税率7.8％適用分　B
						F203_23C[MONY_BUF_SIZE],		// 24.C	控除対象仕入税額がプラスの時 合計 C\

						F203_24A[MONY_BUF_SIZE],		// 25.A	控除対象仕入税額がマイナスの時 税率6.24％適用分　A
						F203_24B[MONY_BUF_SIZE],		// 25.B	控除対象仕入税額がマイナスの時 税率7.8％適用分　B
						F203_24C[MONY_BUF_SIZE],		// 25.C	控除対象仕入税額がマイナスの時 合計 C

						F203_25A[MONY_BUF_SIZE],		// 26.A	貸倒回収に係る消費税額 税率6.24％適用分　A
						F203_25B[MONY_BUF_SIZE],		// 26.B	貸倒回収に係る消費税額 税率7.8％適用分　B
						F203_25C[MONY_BUF_SIZE];		// 26.C	貸倒回収に係る消費税額 合計 C

	unsigned short		F203_8EX;						// 8.F課税売上割合
	int					F203_8C;						// 手入力された課税売上割合

	int					F203_10c,
						F203_12c,
						F203_19c,
						F203_20c,
						F203_23c;

	long				eymd;

	CH31SnFhyo2_01Data	**m_pSnFhyo2_01Data;
	CH31SnTanaData		**m_pSnTanaData;				// 棚卸調整データ
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報

	SYC_SHININFO		*m_pShinInfo;					// 共通情報
	CArithEx			*m_pArith;						// 倍長演算
	CSyzSyukei			**m_pSyzSyukei;					// 消費税集計クラス
	CSyzUtil			m_Util;

	BOOL				m_SpcSgn;						// 特定収入転記サイン

	BOOL				m_UwSgn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo2_03Diag01;
	CIcsdiagctrl m_Fhyo2_03Diag02;
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
