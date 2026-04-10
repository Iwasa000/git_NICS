#pragma once
#include "icsdiagctrl.h"


// CShinIppan01 ダイアログ

enum {
	IPPAN01EX_NUM_HOJIN1			= 104,	// 法人番号１
	IPPAN01EX_NUM_HOJIN2,					// 法人番号２
	IPPAN01EX_NUM_HOJIN3,					// 法人番号３
	IPPAN01EX_NUM_HOJIN4,					// 法人番号４
	IPPAN01EX_NUM_HOJINHYPHEN1,				// 法人番号ハイフン
	IPPAN01EX_NUM_HOJINHYPHEN2,				// 法人番号ハイフン
	IPPAN01EX_NUM_HOJINHYPHEN3,				// 法人番号ハイフン
	IPPAN01EX_NUM_KOJIN1,					// 個人番号１
	IPPAN01EX_NUM_KOJIN2,					// 個人番号２
	IPPAN01EX_NUM_KOJIN3,					// 個人番号３
	IPPAN01EX_NUM_KOJINHYPHEN1,				// 個人番号ハイフン
	IPPAN01EX_NUM_KOJINHYPHEN2				// 個人番号ハイフン
};

class CShinIppan01EX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinIppan01EX)

public:
	CShinIppan01EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinIppan01EX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IPPAN01EX };

	// 初期情報のセット
	int InitInfo(CSnHeadData **pSnHeadData, CH31SnHonpyoData **pHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CSyzSyukei **pSyzSyukei);

	// 初期化
	int	Init();

	// 初期ポジションセット
	void SetItemPosition();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// データの読込み
	int ReadData();
	void ReadData2();

	// データの書込み
	int WriteData();

	// 金額欄表示
	void DispIppan();

	// 本表上部表示
	void DispIppanUpper();

	// 金額表示クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 一般本表金額表示
	void Disp_Ippandata ( int atno );

	// 2. 消費税額
	void Disp_Ippandata2();

	// 14. 差引納付税額
	void Disp_Ippandata14();

	// 25. 差引納付譲渡割額
	void Disp_Ippandata25();

	// ダイアグラム属性変更
	int ChgDiagAttr();

	// 終了時のフォーカス移動
	void MoveFocusInEnd();

	// 金額の最終データ読込み
	void GetDiagData();

	// 入力項目色変え
	void ChangeColor ( unsigned short id, short Index, int sign );

	// プレプリント
	void PrePrint();

	// マイナンバーエディットコントロールの位置調整
	void MoveDiagMNEdit();

	// マイナンバーコントロール非表示
	void HideMNEdit();

	// 入力位置を保存
	void SaveInputPosition();

	// 入力位置を復元
	void RestoreInputPosition();

	// 集計セット
	int SyukeiDataSet ( int cID );

	// 集計データ取得
	int MemoryReget();

	//還付申告かどうかyoshida190613
	BOOL IsKanpu();

	// 税務署入力にフォーカスセット
	void SetTaxOfficeInput();

	// 還付金融機関入力にフォーカス
	void SetKanpuKinyuInput();

	// 付表データ表示
	void Disp_FhyoData();

	// 還付金融機関の表示・非表示
	void EnableKanpuKinyu( int sw );

	// 準確定かどうか
	BOOL IsJunkakutei();

private:
	CWnd				*m_pParent;				//　親ウィンドウ

	//== 帳票データ ==
	char				XM1[MONY_BUF_SIZE],		//  1. 課税標準額
						XM2[MONY_BUF_SIZE],		//  2. 消費税額
						XM3[MONY_BUF_SIZE],		//  3. 控除過大調整税額
						XM4[MONY_BUF_SIZE],		//  4. 控除対象仕入税額
						XM5[MONY_BUF_SIZE],		//  5. 返還等対価に係る税額
						XM6[MONY_BUF_SIZE],		//  6. 貸倒れに係る税額
						XM7[MONY_BUF_SIZE],		//  7. 控除税額小計
						XM8[MONY_BUF_SIZE],		//  8. 控除不足還付税額
						XM9[MONY_BUF_SIZE],		//  9. 差引税額
						XM10[MONY_BUF_SIZE],	// 10. 中間納付税額
						XM11[MONY_BUF_SIZE],	// 11. 納付税額
						XM12[MONY_BUF_SIZE],	// 12. 中間納付還付税額
						XM13[MONY_BUF_SIZE],	// 13. 既確定税額
						XM14[MONY_BUF_SIZE],	// 14. 差引納付税額
						XM15[MONY_BUF_SIZE],	// 15. 課税資産の譲渡等の対価の額
						XM16[MONY_BUF_SIZE],	// 16. 資産の譲渡等の対価の額
						XM17[MONY_BUF_SIZE],	// 17. 控除不足還付税額
						XM18[MONY_BUF_SIZE],	// 18. 差引税額
						XM19[MONY_BUF_SIZE],	// 19. 還付額
						XM20[MONY_BUF_SIZE],	// 20. 納税額
						XM21[MONY_BUF_SIZE],	// 21. 中間納付譲渡割額
						XM22[MONY_BUF_SIZE],	// 22. 納付譲渡割額
						XM23[MONY_BUF_SIZE],	// 23. 中間還付譲渡割額
						XM24[MONY_BUF_SIZE],	// 24. 既確定譲渡割額
						XM25[MONY_BUF_SIZE],	// 25. 差引納付譲渡割額
						XM26[MONY_BUF_SIZE];	// 26. 消費税及び地方消費税の合計(納付又は還付)税額


	int					SW_WARIFU;				// 割賦基準の適用 ( 1:適用, 0:未 )
	int					SW_NOBEBA;				// 延払基準の適用 ( 1:適用, 0:未 )
	int					SW_KOUJIS;				// 工事進行基準の適用 ( 1:適用, 0:未 )
	int					SW_GENKIN;				// 現金主義会計の適用 ( 1:適用, 0:未 )
	int					SW_KISK22;				// 規則22条1項の適用
	int					SW_WITH_TKKZ;			// 特定課税仕入に係る別表の添付（1:有, 0:無 ）

	int					m_curwnd;
	int					m_curInPos;				// 現入力位置保存

// 改良依頼 18-0439 修正 add -->
	BOOL				m_bKKMEditSgn;			// EditOnでSetPositonを行ったとき、再度EditOn内で行っている処理が
												// 動かないようにするサイン
// 改良依頼 18-0439 修正 add <--

// 2022/12/27---->
	BOOL				m_IsKanpuEnable;		// 還付金融機関が入力可かどうか　TRUE:可 / FALSE：不可
// 2022/12/27<----


private:
	//== 帳票データ本体 ==
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報
	CH31SnHonpyoData	**m_pSnHonpyoData;

	CArithEx			*m_pArith;
	CSyzUtil			m_Util;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報
	CSyzSyukei			**m_pSyzSyukei;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Ippan01Diag01;
	CIcsdiagctrl m_Ippan01Diag02;
	CIcsdiagctrl m_Ippan01Diag03;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditONIcsdiag8ctrl2(short index);
	void EditONIcsdiag8ctrl3(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl3(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void TerminationIcsdiag8ctrl3(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();

	CICSMNEditController8	m_ctrl_mynum;
	CICSMNEdit8				m_edit_mynum1;
	CICSMNEdit8 			m_edit_mynum2;
	CICSMNEdit8 			m_edit_mynum3;
	CICSMNEdit8 			m_edit_mynum4;
	void CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data);
	void CheckButtonIcsdiag8ctrl3(short index, LPUNKNOWN data);
};
