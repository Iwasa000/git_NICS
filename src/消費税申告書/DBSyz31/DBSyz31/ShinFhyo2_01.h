#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CShinFhyo2_01 ダイアログ

class CShinFhyo2_01 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo2_01)

public:
	CShinFhyo2_01(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo2_01();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY2_01 };


	// 初期化
	int Init();

	// ボタン表示変更
	void ChangeBtnDisp();

	// 初期情報のセット
	int InitInfo ( CH31SnFhyo2_01Data **pSnFhyo2_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH31SnTanaData **pSnTanaData, CSyzSyukei **pSyzSyukei);

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
	void DispFh2_01();

	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );

	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );

	// 各入力データ抽出
	void GetAllDiagData();

	// 特定課税仕入れに関連するインデックス
	int ChkTkkzIdx ( short idx );

	// 特例計算に関連するインデックス
	int ChkTokureiIdx ( short id );

	// 集計モジュールを更新
	int SyukeiDataSet( int cID );
	int MemoryReget();


	// --------- 表示 --------------

	// 課税売上額
	void f201_1data();
	// 課税資産の譲渡等の対価の額
	void f201_45data();
	// 非課税売上額
	void f201_6data();
	// 資産の譲渡等の対価の額
	void f201_7data();
	//課税売上割合
	void f201_8data();
	// 課税仕入に係る支払対価の額
	void f201_9data();
	// 課税仕入に係る消費税額
	void f201_10data();
	//特定課税仕入れに係る支払対価の額
	void f201_11data();
	//特定課税仕入れに係る消費税額
	void f201_12data();
	// 課税貨物に係る消費税額
	void f201_13data();
	// 課税事業⇔免税事業 調整税額
	void f201_14data();
	// 課税仕入等の税額の合計額
	void f201_15data();
	// 課税売上割合が９５％以上の場合
	void f201_16data();
	// 課税売上にのみ要する仕入税額
	void f201_17data();
	// 共通の売上に要する仕入税額
	void f201_18data();
	// 個別対応方式控除仕入税額
	void f201_19data();
	// 比例配分方式控除仕入税額
	void f201_20data();
	// 課税売上割合が９５％未満の場合
	void f201_ko();
	// 控除税額の調整(21)
	void f201_21data();
	// 控除税額の調整(22)
	void f201_22data();
	// 差引控除仕入税額の計算
	void f201_2324data();
	// 回収に係る消費税額
	void f201_25data();
	// 他表からの計算
	void calq_by_OtherChange();
	// 特定収入を加味した控除対象仕入税額の計算
	void f201_spc();


	// 各入力のデータ表示
	void f201data_all( int atno );

	// 課税売上高5億円越えチェック
	void CheckOver500Million();
	// ５億円チェック（金額チェック部分）
	int CheckOver500MillionSub( int &MilSw, char *pVAL, char *pValCnv );

	// 棚卸データ（ダイアログ）の加減算を行う
	void TanaDataAdd();

	// 課税売上割合の取得
	unsigned short GetCalqedKzwari();

	// -----------------------------

	// 入力許可・不許可 切替
	void ModifyDiagInput( short index, int sign );

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short index, int sign );

	// フォーカス移動処理(上下) Move = 0 : 下方向	Move = 1 : 上方向
	int UpDownFocusMoveCheck ( int Index, int Move );

	// 特定収入を加味した控除対象仕入税額の計算
	void f201calq_spc ( SYZTOSPC_DATA SpcData );

	// 手入力項目あり？
	BOOL IsManualInputSign();

private:
	CWnd*			m_pParent;		// 親ウィンドウ

	//== 帳票データ ==
	char			F201_1X[MONY_BUF_SIZE],			// 1.X	課税売上(税抜き) 旧税率分小計 X
					F201_1D[MONY_BUF_SIZE],			// 1.D	課税売上(税抜き) 税率6.24％適用分　D
					F201_1E[MONY_BUF_SIZE],			// 1.E	課税売上(税抜き) 税率7.8％適用分　E
					F201_1F[MONY_BUF_SIZE],			// 1.F	課税売上(税抜き) 合計 F

					F201_2F[MONY_BUF_SIZE],			// 2.F	免税売上額 合計 F

					F201_3F[MONY_BUF_SIZE],			// 3.F	非課税資産の輸出等の金額、海外支店等へ移送した資産の価額 合計 F

					F201_4F[MONY_BUF_SIZE],			// 4.F	課税資産の譲渡等の対価の額(1+2+3) 合計 F

					F201_5F[MONY_BUF_SIZE],			// 5.F	課税資産の譲渡等の対価の額(4の金額) 合計 F

					F201_6F[MONY_BUF_SIZE],			// 6.F	非課税売上額 合計 F

					F201_7F[MONY_BUF_SIZE],			// 7.F	資産の譲渡等の対価の額(5+6) 合計 F

					//F201_8F[MONY_BUF_SIZE],			// 8.F	課税売上割合(4/7) 合計 F

					F201_9X[MONY_BUF_SIZE],			// 9.X	課税仕入れに係る支払対価の額(税込み) 旧税率分小計 X
					F201_9D[MONY_BUF_SIZE],			// 9.D	課税仕入れに係る支払対価の額(税込み) 税率6.24％適用分　D
					F201_9E[MONY_BUF_SIZE],			// 9.E	課税仕入れに係る支払対価の額(税込み) 税率7.8％適用分　E
					F201_9F[MONY_BUF_SIZE],			// 9.F	課税仕入れに係る支払対価の額(税込み) 合計 F

					F201_10X[MONY_BUF_SIZE],		// 10.X	課税仕入れに係る消費税額 旧税率分小計 X
					F201_10D[MONY_BUF_SIZE],		// 10.D	課税仕入れに係る消費税額 税率6.24％適用分　D
					F201_10E[MONY_BUF_SIZE],		// 10.E	課税仕入れに係る消費税額 税率7.8％適用分　E
					F201_10F[MONY_BUF_SIZE],		// 10.F	課税仕入れに係る消費税額 合計 F

					F201_11X[MONY_BUF_SIZE],		// 11.X	特定課税仕入れに係る支払対価の額 旧税率分小計 X
					F201_11E[MONY_BUF_SIZE],		// 11.E	特定課税仕入れに係る支払対価の額 税率7.8％適用分　E
					F201_11F[MONY_BUF_SIZE],		// 11.F	特定課税仕入れに係る支払対価の額 合計 F

					F201_12X[MONY_BUF_SIZE],		// 12.X	特定課税仕入れに係る消費税額 旧税率分小計 X
					F201_12E[MONY_BUF_SIZE],		// 12.E	特定課税仕入れに係る消費税額 税率7.8％適用分　E
					F201_12F[MONY_BUF_SIZE],		// 12.F	特定課税仕入れに係る消費税額 合計 F

					F201_13X[MONY_BUF_SIZE],		// 13.X	課税貨物に係る消費税額 旧税率分小計 X
					F201_13D[MONY_BUF_SIZE],		// 13.D	課税貨物に係る消費税額 税率6.24％適用分　D
					F201_13E[MONY_BUF_SIZE],		// 13.E	課税貨物に係る消費税額 税率7.8％適用分　E
					F201_13F[MONY_BUF_SIZE],		// 13.F	課税貨物に係る消費税額 合計 F

					F201_14X[MONY_BUF_SIZE],		// 14.X	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 旧税率分小計 X
					F201_14D[MONY_BUF_SIZE],		// 14.D	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率6.24％適用分　D
					F201_14E[MONY_BUF_SIZE],		// 14.E	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 税率7.8％適用分 E
					F201_14F[MONY_BUF_SIZE],		// 14.F	納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額 合計 F

					F201_15X[MONY_BUF_SIZE],		// 15.X	課税仕入れ等の税額の合計額(10+12+13±14) 旧税率分小計 X
					F201_15D[MONY_BUF_SIZE],		// 15.D	課税仕入れ等の税額の合計額(10+12+13±14) 税率6.24％適用分　D
					F201_15E[MONY_BUF_SIZE],		// 15.E	課税仕入れ等の税額の合計額(10+12+13±14) 税率7.8％適用分　E
					F201_15F[MONY_BUF_SIZE],		// 15.F	課税仕入れ等の税額の合計額(10+12+13±14) 合計 F

					F201_16X[MONY_BUF_SIZE],		// 16.X	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 旧税率分小計 X
					F201_16D[MONY_BUF_SIZE],		// 16.D	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 税率6.24％適用分　D
					F201_16E[MONY_BUF_SIZE],		// 16.E	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 税率7.8％適用分　E
					F201_16F[MONY_BUF_SIZE],		// 16.F	課税売上高が５億円以下、かつ、課税売上割合が95%以上の場合(15の金額) 合計 F

					F201_17X[MONY_BUF_SIZE],		// 17.X	15のうち、課税売上げにのみ要するもの 旧税率分小計 X
					F201_17D[MONY_BUF_SIZE],		// 17.D	15のうち、課税売上げにのみ要するもの 税率6.24％適用分　D
					F201_17E[MONY_BUF_SIZE],		// 17.E	15のうち、課税売上げにのみ要するもの 税率7.8％適用分　E
					F201_17F[MONY_BUF_SIZE],		// 17.F	15のうち、課税売上げにのみ要するもの 合計 F

					F201_18X[MONY_BUF_SIZE],		// 18.X	15のうち、課税売上げと非課税売上げに共通して要するもの 旧税率分小計 X
					F201_18D[MONY_BUF_SIZE],		// 18.D	15のうち、課税売上げと非課税売上げに共通して要するもの 税率6.24％適用分　D
					F201_18E[MONY_BUF_SIZE],		// 18.E	15のうち、課税売上げと非課税売上げに共通して要するもの 税率7.8％適用分　E
					F201_18F[MONY_BUF_SIZE],		// 18.F	15のうち、課税売上げと非課税売上げに共通して要するもの 合計 F

					F201_19X[MONY_BUF_SIZE],		// 19.X	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 旧税率分小計 X
					F201_19D[MONY_BUF_SIZE],		// 19.D	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率6.24％適用分　D
					F201_19E[MONY_BUF_SIZE],		// 19.E	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率7.8％適用分　E
					F201_19F[MONY_BUF_SIZE],		// 19.F	個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 合計 F

					F201_20X[MONY_BUF_SIZE],		// 20.X	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 旧税率分小計 X
					F201_20D[MONY_BUF_SIZE],		// 20.D	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率6.24％適用分　D
					F201_20E[MONY_BUF_SIZE],		// 20.E	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率7.8％適用分　E
					F201_20F[MONY_BUF_SIZE],		// 20.F	一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 合計 F

					F201_21X[MONY_BUF_SIZE],		// 21.X	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 旧税率分小計 X
					F201_21D[MONY_BUF_SIZE],		// 21.D	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率6.24％適用分　D
					F201_21E[MONY_BUF_SIZE],		// 21.E	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 税率7.8％適用分　E
					F201_21F[MONY_BUF_SIZE],		// 21.F	課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額 合計 F

					F201_22X[MONY_BUF_SIZE],		// 22.X	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 旧税率分小計 X
					F201_22D[MONY_BUF_SIZE],		// 22.D	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率6.24％適用分　D
					F201_22E[MONY_BUF_SIZE],		// 22.E	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 税率7.8％適用分　E
					F201_22F[MONY_BUF_SIZE],		// 22.F	調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額 合計 F

					F201_23X[MONY_BUF_SIZE],		// 23.X	控除対象仕入税額がプラスの時 旧税率分小計 X
					F201_23D[MONY_BUF_SIZE],		// 23.D	控除対象仕入税額がプラスの時 税率6.24％適用分　D
					F201_23E[MONY_BUF_SIZE],		// 23.E	控除対象仕入税額がプラスの時 税率7.8％適用分　E
					F201_23F[MONY_BUF_SIZE],		// 23.F	控除対象仕入税額がプラスの時 合計 F

					F201_24X[MONY_BUF_SIZE],		// 24.X	控除対象仕入税額がマイナスの時 旧税率分小計 X
					F201_24D[MONY_BUF_SIZE],		// 24.D	控除対象仕入税額がマイナスの時 税率6.24％適用分　D
					F201_24E[MONY_BUF_SIZE],		// 24.E	控除対象仕入税額がマイナスの時 税率7.8％適用分　E
					F201_24F[MONY_BUF_SIZE],		// 24.F	控除対象仕入税額がマイナスの時 合計 F

					F201_25X[MONY_BUF_SIZE],		// 25.X	貸倒回収に係る消費税額 旧税率分小計 X
					F201_25D[MONY_BUF_SIZE],		// 25.D	貸倒回収に係る消費税額 税率6.24％適用分　D
					F201_25E[MONY_BUF_SIZE],		// 25.E	貸倒回収に係る消費税額 税率7.8％適用分　E
					F201_25F[MONY_BUF_SIZE];		// 25.F	貸倒回収に係る消費税額 合計 F

	unsigned short	F201_8EX;						// 8.F課税売上割合
	int				F201_8F;						// 手入力された課税売上割合



	int				m_curwnd;

	CH31SnFhyo2_01Data	**m_pSnFhyo2_01Data;
	CSnHeadData		**m_pSnHeadData;					// 消費税ヘッダ情報
	CH31SnTanaData	**m_pSnTanaData;					// 棚卸調整データ
	SYC_SHININFO	*m_pShinInfo;						// 共通情報
	CArithEx		*m_pArith;
	CSyzUtil		m_Util;
	CSyzSyukei		**m_pSyzSyukei;						// 消費税集計クラス




	int				F201_10c, F201_12c, F201_19c, F201_20c, F201_23c;	// 手入力サイン? 元処理引用
	int				m_FrSw;						// ??? 元処理引用
	int				m_DpSw;						// ??? 元処理引用
	int				m_DlSw;						// ??? 元処理引用

	BOOL			m_SpcSgn;					// 特定収入転記サイン

	BOOL			m_UwSgn;

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
	CIcsdiagctrl m_Fhyo2_01Diag01;
	CIcsdiagctrl m_Fhyo2_01Diag02;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_tanaoroshi;
	afx_msg void OnBnClickedButton1();
	void CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data);
	afx_msg void OnBnClickedButton2();
	CButton m_Button2;
};
