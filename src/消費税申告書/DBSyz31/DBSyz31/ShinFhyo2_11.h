#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"

// ShinFhyo2_03 ダイアログ

class CShinFhyo2_11 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo2_11)

public:
	CShinFhyo2_11(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinFhyo2_11();

	// ダイアログ データ
	//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FHY2_11 };
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
	void DispFh2_11();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示金額クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 課税売上高5億円越えチェック
	void CheckOver500Million();
	// ５億円チェック（金額チェック部分）
	int CheckOver500MillionSub( int &MilSw, char *pVAL, char *pValCnv );

	// 全項目表示
	void f211data_all ( int atno );
	// 課税売上(税抜き)表示
	void f211data_1();
	// 免税売上額表示
	void f211data_2();
	// 非課税資産の輸出等の金額、海外支店等へ移送した資産の価額表示
	void f211data_3();
	// 課税資産の譲渡等の対価の額表示
	void f211data_4_5();
	// 非課税売上額表示
	void f211data_6();
	// 資産の譲渡等の対価の額表示
	void f211data_7();
	// 課税売上割合表示
	void f211data_8();
	// 課税仕入れに係る支払対価の額(税込み)表示
	void f211data_9();
	// 課税仕入れに係る消費税額表示
	void f211data_10();
	// 特定課税仕入れに係る支払対価の額表示
	void f211data_11();
	// 特定課税仕入れに係る消費税額表示
	void f211data_12();
	// 課税貨物に係る消費税額表示
	void f211data_13();
	// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額表示
	void f211data_14();
	// 課税仕入れ等の税額の合計額表示
	void f211data_15();
	// 課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合表示
	void f211data_16();
	// 15のうち、課税売上げにのみ要するもの表示
	void f211data_17();
	// 15のうち、課税売上げと非課税売上げに共通して要するもの表示
	void f211data_18();
	// 個別対応方式により控除する課税仕入れ等の税額表示
	void f211data_19();
	// 一括比例配分方式により控除する課税仕入れ等の税額表示
	void f211data_20();
	// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額表示
	void f211data_21();
	// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額表示
	void f211data_22();
	// 控除対象仕入税額がプラスの時表示
	void f211data_23();
	// 控除対象仕入税額がマイナスの時表示
	void f211data_24();
	// 貸倒回収に係る消費税額表示
	void f211data_25();
	// 居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額表示
	void f211data_26();

	// 課税売上割合を変更
	void Changef211data_8();

	// 棚卸データの加減算を行う
	void TanaDataAdd();

	// 表示金額取得
	void GetAllDiagData();

	// 課税売上割合の取得
	unsigned short GetCalqedKzwari();

	// ボタン表示変更
	void ChangeBtnDisp();

	// 初期ポジションセット
	void SetItemPosition();

	// フォーカス移動先取得
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 入力許可・不許可 切替
	void ModifyDiagInput ( short index, int sign );

	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

	// 特定収入を加味した控除対象仕入税額の計算
	void f211calq_spc ( SYZTOSPC_DATA SpcData );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd*				m_pParent;		// 親ウィンドウ
	int					m_curwnd;

	char				F211_1X[MONY_BUF_SIZE],			// 1.X	課税売上(税抜き) 旧税率分小計 X
						F211_1D[MONY_BUF_SIZE],			// 1.D	課税売上(税抜き) 税率6.24％適用分　D
						F211_1E[MONY_BUF_SIZE],			// 1.E	課税売上(税抜き) 税率7.8％適用分　E
						F211_1F[MONY_BUF_SIZE],			// 1.F	課税売上(税抜き) 合計 F

						F211_2F[MONY_BUF_SIZE],			// 2.F	免税売上額 合計 F

						F211_3F[MONY_BUF_SIZE],			// 3.F	非課税資産の輸出等の金額、海外支店等へ移送した資産の価額 合計 F

						F211_4F[MONY_BUF_SIZE],			// 4.F	課税資産の譲渡等の対価の額(1+2+3) 合計 F

						F211_5F[MONY_BUF_SIZE],			// 5.F	課税資産の譲渡等の対価の額(4の金額) 合計 F

						F211_6F[MONY_BUF_SIZE],			// 6.F	非課税売上額 合計 F

						F211_7F[MONY_BUF_SIZE],			// 7.F	資産の譲渡等の対価の額(5+6) 合計 F

						F211_9X[MONY_BUF_SIZE],			// 9.X	課税仕入れに係る支払対価の額(税込み) 旧税率分小計 X
						F211_9D[MONY_BUF_SIZE],			// 9.D	課税仕入れに係る支払対価の額(税込み) 税率6.24％適用分　D
						F211_9E[MONY_BUF_SIZE],			// 9.E	課税仕入れに係る支払対価の額(税込み) 税率7.8％適用分　E
						F211_9F[MONY_BUF_SIZE],			// 9.F	課税仕入れに係る支払対価の額(税込み) 合計 F

						F211_10X[MONY_BUF_SIZE],		// 10.X	課税仕入れに係る消費税額 旧税率分小計 X
						F211_10D[MONY_BUF_SIZE],		// 10.D	課税仕入れに係る消費税額 税率6.24％適用分　D
						F211_10E[MONY_BUF_SIZE],		// 10.E	課税仕入れに係る消費税額 税率7.8％適用分　E
						F211_10F[MONY_BUF_SIZE],		// 10.F	課税仕入れに係る消費税額 合計 F

						F211_11X[MONY_BUF_SIZE],		// 11.X	特定課税仕入れに係る支払対価の額 旧税率分小計 X
						F211_11E[MONY_BUF_SIZE],		// 11.E	特定課税仕入れに係る支払対価の額 税率7.8％適用分　E
						F211_11F[MONY_BUF_SIZE],		// 11.F	特定課税仕入れに係る支払対価の額 合計 F

						F211_12X[MONY_BUF_SIZE],		// 12.X	特定課税仕入れに係る消費税額 旧税率分小計 X
						F211_12E[MONY_BUF_SIZE],		// 12.E	特定課税仕入れに係る消費税額 税率7.8％適用分　E
						F211_12F[MONY_BUF_SIZE],		// 12.F	特定課税仕入れに係る消費税額 合計 F

						F211_13X[MONY_BUF_SIZE],		// 13.X	課税貨物に係る消費税額 旧税率分小計 X
						F211_13D[MONY_BUF_SIZE],		// 13.D	課税貨物に係る消費税額 税率6.24％適用分　D
						F211_13E[MONY_BUF_SIZE],		// 13.E	課税貨物に係る消費税額 税率7.8％適用分　E
						F211_13F[MONY_BUF_SIZE],		// 13.F	課税貨物に係る消費税額 合計 F

						F211_14X[MONY_BUF_SIZE],		// 14.X	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 旧税率分小計 X
						F211_14D[MONY_BUF_SIZE],		// 14.D	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率6.24％適用分　D
						F211_14E[MONY_BUF_SIZE],		// 14.E	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率7.8％適用分 E
						F211_14F[MONY_BUF_SIZE],		// 14.F	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 合計 F

						F211_15X[MONY_BUF_SIZE],		// 15.X	課税仕入れ等の税額の合計額(10+12+13±14) 旧税率分小計 X
						F211_15D[MONY_BUF_SIZE],		// 15.D	課税仕入れ等の税額の合計額(10+12+13±14) 税率6.24％適用分　D
						F211_15E[MONY_BUF_SIZE],		// 15.E	課税仕入れ等の税額の合計額(10+12+13±14) 税率7.8％適用分　E
						F211_15F[MONY_BUF_SIZE],		// 15.F	課税仕入れ等の税額の合計額(10+12+13±14) 合計 F

						F211_16X[MONY_BUF_SIZE],		// 16.X	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 旧税率分小計 X
						F211_16D[MONY_BUF_SIZE],		// 16.D	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 税率6.24％適用分　D
						F211_16E[MONY_BUF_SIZE],		// 16.E	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 税率7.8％適用分　E
						F211_16F[MONY_BUF_SIZE],		// 16.F	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 合計 F

						F211_17X[MONY_BUF_SIZE],		// 17.X	15のうち、課税売上げにのみ要するもの 旧税率分小計 X
						F211_17D[MONY_BUF_SIZE],		// 17.D	15のうち、課税売上げにのみ要するもの 税率6.24％適用分　D
						F211_17E[MONY_BUF_SIZE],		// 17.E	15のうち、課税売上げにのみ要するもの 税率7.8％適用分　E
						F211_17F[MONY_BUF_SIZE],		// 17.F	15のうち、課税売上げにのみ要するもの 合計 F

						F211_18X[MONY_BUF_SIZE],		// 18.X	15のうち、課税売上げと非課税売上げに共通して要するもの 旧税率分小計 X
						F211_18D[MONY_BUF_SIZE],		// 18.D	15のうち、課税売上げと非課税売上げに共通して要するもの 税率6.24％適用分　D
						F211_18E[MONY_BUF_SIZE],		// 18.E	15のうち、課税売上げと非課税売上げに共通して要するもの 税率7.8％適用分　E
						F211_18F[MONY_BUF_SIZE],		// 18.F	15のうち、課税売上げと非課税売上げに共通して要するもの 合計 F

						F211_19X[MONY_BUF_SIZE],		// 19.X	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 旧税率分小計 X
						F211_19D[MONY_BUF_SIZE],		// 19.D	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率6.24％適用分　D
						F211_19E[MONY_BUF_SIZE],		// 19.E	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率7.8％適用分　E
						F211_19F[MONY_BUF_SIZE],		// 19.F	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 合計 F

						F211_20X[MONY_BUF_SIZE],		// 20.X	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 旧税率分小計 X
						F211_20D[MONY_BUF_SIZE],		// 20.D	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率6.24％適用分　D
						F211_20E[MONY_BUF_SIZE],		// 20.E	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率7.8％適用分　E
						F211_20F[MONY_BUF_SIZE],		// 20.F	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 合計 F

						F211_21X[MONY_BUF_SIZE],		// 21.X	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 旧税率分小計 X
						F211_21D[MONY_BUF_SIZE],		// 21.D	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率6.24％適用分　D
						F211_21E[MONY_BUF_SIZE],		// 21.E	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率7.8％適用分　E
						F211_21F[MONY_BUF_SIZE],		// 21.F	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 合計 F

						F211_22X[MONY_BUF_SIZE],		// 22.X	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 旧税率分小計 X
						F211_22D[MONY_BUF_SIZE],		// 22.D	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率6.24％適用分　D
						F211_22E[MONY_BUF_SIZE],		// 22.E	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率7.8％適用分　E
						F211_22F[MONY_BUF_SIZE],		// 22.F	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 合計 F

						F211_26X[MONY_BUF_SIZE],		// 23.X	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 旧税率分小計 X
						F211_26D[MONY_BUF_SIZE],		// 23.D	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率6.24％適用分 D
						F211_26E[MONY_BUF_SIZE],		// 23.E	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 税率7.8％適用分　E
						F211_26F[MONY_BUF_SIZE],		// 23.F	居住用賃貸建物を課税賃貸用に供した(譲渡した)場合の加算額 合計 F

						F211_23X[MONY_BUF_SIZE],		// 24.X	控除対象仕入税額がプラスの時 旧税率分小計 X
						F211_23D[MONY_BUF_SIZE],		// 24.D	控除対象仕入税額がプラスの時 税率6.24％適用分　D
						F211_23E[MONY_BUF_SIZE],		// 24.E	控除対象仕入税額がプラスの時 税率7.8％適用分　E
						F211_23F[MONY_BUF_SIZE],		// 24.F	控除対象仕入税額がプラスの時 合計 F

						F211_24X[MONY_BUF_SIZE],		// 25.X	控除対象仕入税額がマイナスの時 旧税率分小計 X
						F211_24D[MONY_BUF_SIZE],		// 25.D	控除対象仕入税額がマイナスの時 税率6.24％適用分　D
						F211_24E[MONY_BUF_SIZE],		// 25.E	控除対象仕入税額がマイナスの時 税率7.8％適用分　E
						F211_24F[MONY_BUF_SIZE],		// 25.F	控除対象仕入税額がマイナスの時 合計 F

						F211_25X[MONY_BUF_SIZE],		// 26.X	貸倒回収に係る消費税額 旧税率分小計 X
						F211_25D[MONY_BUF_SIZE],		// 26.D	貸倒回収に係る消費税額 税率6.24％適用分　D
						F211_25E[MONY_BUF_SIZE],		// 26.E	貸倒回収に係る消費税額 税率7.8％適用分　E
						F211_25F[MONY_BUF_SIZE];		// 26.F	貸倒回収に係る消費税額 合計 F

	unsigned short		F211_8EX;						// 8.F課税売上割合
	int					F211_8F;						// 手入力された課税売上割合

	int					F211_10c,
						F211_12c,
						F211_19c,
						F211_20c,
						F211_23c;

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
	CIcsdiagctrl m_Fhyo2_11Diag01;
	CIcsdiagctrl m_Fhyo2_11Diag02;
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
