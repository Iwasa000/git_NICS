#pragma once
#include "icsdiagctrl.h"


// CShinKani ダイアログ

class CShinKani : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinKani)

public:
	CShinKani(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinKani();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KANI };

	// 初期情報のセット
	int InitInfo( CH26SnHonpyoData **pSnHonpyoData, CH26SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// ダイアグラム初期設定
	int ChgDiagAttr();
	// 終了処理
	void EndJob();
	// 入力位置のセット
	void SetItemPosition( int no );
	// 税務署入力にフォーカスセット('15.02.03)
	void SetTaxOfficeInput();
	// 入力位置を保存 ('15.03.02)
	void SaveInputPosition();
	// 入力位置を復元 ('15.03.02)
	void RestoreInputPosition();
	// 終了時のフォーカス移動 ('15.03.02)
	void MoveFocusInEnd();
	// 還付金融機関入力にフォーカスセット('16.12.26)
	void SetKanpuKinyuInput();

	// 初期データ表示
	void DispInitKani();
//2016.10.20 INSERT START
	// 初期データ上部表示
	void DispInitKaniUpper();
//2016.10.20 INSERT END
	// 本表：金額欄表示
	void DispKani();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 最終データ読込み
	void GetDiagData();

	// データの読込み
	int ReadData();
	// データの書込み
	int WriteData();

	// プレプリント
	void PerPrint();
//2016.02.23 INSERT START
	void PerPrint2(); //欄外メモ
//2016.02.23 INSERT END

	//== 計算 ==
	// １．課税標準額
	void calqx_1();
	// ２．消費税額
	void calqx_2();
	// ７．控除税額小計
	void calqx_7( int sw );
	// ８．控除不足還付税額・９．差引税額（２＋３－７）
	void calqx_89( int sw );
	// １０．中間納付税額
	void calqx_10();
	// １１．納付税額（９－１０）・１２．中間納付還付
	void calqx_1112( int sw );
	// １４．差引き納付税額
	int calqx_14( int sw );
	// １７．控除不足還付税額（地方消費税）
	void calqx_17( int sw );
	// １８．差引税額（地方消費税）
	void calqx_18( int sw );
	// １９．譲渡割額還付額 ･ ２０．譲渡割額納税額（地方消費税）
	void calqx_1920( int sw );
	// ２１．中間納付譲渡割額
	void calqx_21();
	// ２２．納付譲渡割額（２０－２１）・２３．中間還付譲渡割額
	void calqx_2223( int sw );
	// ２５．差引納付譲渡割額
	int calqx_25( int sw );
	// ２６．消費税及び地方消費税の合計（納付又は還付）税額
	void calqx_26( int sw );
	// 各入力の計算表示
	void calqx_all( int atno, int sw );
	// 別表の計算
	void calqx_otherlists();
	// 他表からの計算
	void calq_by_OtherChange( int ksw );

	//[Y] 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );

	// 控除対象仕入税額の計算方法の判定	('15.01.27)
	int judge_skj_calq();

//2016.06.22 INSERT START
	BOOL IsKanpu();
//2016.06.22 INSERT END

private:
	CWnd				*m_pParent;				// 親ウィンドウ

	//== 帳票データ ==
	char				XM1[MONY_BUF_SIZE],		//  1. 課税標準額
						XM2[MONY_BUF_SIZE],		//  2. 消費税額計
						XM3[MONY_BUF_SIZE],		//  3. 控除過大調整税額 or 貸倒回収に係る消費税額
						XM4[MONY_BUF_SIZE],		//  4. 控除対象仕入税額
						XM5[MONY_BUF_SIZE],		//  5. 返還等対価の税額
						XM6[MONY_BUF_SIZE],		//  6. 貸倒れに係る税額
						XM7[MONY_BUF_SIZE],		//  7. 控除税額小計
						XM8[MONY_BUF_SIZE],		//  8. 控除不足還付税額
						XM9[MONY_BUF_SIZE],		//  9. (2+3-7) 差引税額
						XM10[MONY_BUF_SIZE],	// 10. 中間納付税額
						XM11[MONY_BUF_SIZE],	// 11. (9-10) 納付税額
						XM12[MONY_BUF_SIZE],	// 12. 中間納付還元税額
						XM13[MONY_BUF_SIZE],	// 13. 既確定税額
						XM14[MONY_BUF_SIZE],	// 14. 差引納付税額
						XM15[MONY_BUF_SIZE],	// 15. 課税売上高
						SM15[MONY_BUF_SIZE],	// 15. 課税売上高(save)	
						XM16[MONY_BUF_SIZE],	// 16. 総売上高 or 基準期間の課税売上高
						XM17[MONY_BUF_SIZE],	// 17. 控除不足還付税額
						XM18[MONY_BUF_SIZE],	// 18. 差引税額
						XM19[MONY_BUF_SIZE],	// 19. (17*25%) 還付額
						XM20[MONY_BUF_SIZE],	// 20. (18*25%) 納税額
						XM21[MONY_BUF_SIZE],	// 21. 中間納付譲渡割額
						XM22[MONY_BUF_SIZE],	// 22. 納付譲渡割額
						XM23[MONY_BUF_SIZE],	// 23. 中間還付譲渡割額
						XM24[MONY_BUF_SIZE],	// 24. 既確定譲渡割額
						XM25[MONY_BUF_SIZE],	// 25. 差引納付譲渡割額
						XM26[MONY_BUF_SIZE];	// 26. 消費税･地方消費税合計

	int					SW_WARIFU;				// 割賦基準の適用
	int					SW_NOBEBA;				// 延払基準の適用
	int					SW_KOUJIS;				// 工事進行基準の適用
	int					SW_GENKIN;				// 現金主義会計の適用
	int					SW_KISK22;				// 規則22条1項の適用

	//== 帳票データ本体 ==
	CH26SnHonpyoData	**m_pSnHonpyoData;
	CH26SnFhyo10Data	**m_pSnFhyo40Data;		// 付表４データ
	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報

	CArithEx			*m_pArith;
	CSyzUtil			m_Util;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報

//	static	int		kani_sw;	// 簡易課税ＳＷ
	int					m_kanisw;				// 簡易課税ＳＷ	(0:通常、免税, 2以上:簡易課税)

//	static	int		DpSw = 0;
	int					m_DpSw;					// ???

//static	char	M1c, dm;
	char				m_M1c;					// ???

	int					m_curwnd;				//[Y]
	int					m_curInPos;				// 現入力位置保存('15.03.02)


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_KaniDiag02;
	CIcsdiagctrl m_KaniDiag03;
	CIcsdiagctrl m_KaniDiag04;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl3(short index);
	void EditOFFIcsdiagctrl3(short index);
	void TerminationIcsdiagctrl3(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl4(short index);
	void EditOFFIcsdiagctrl4(short index);
	void TerminationIcsdiagctrl4(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnBnClickedButton1();
};
