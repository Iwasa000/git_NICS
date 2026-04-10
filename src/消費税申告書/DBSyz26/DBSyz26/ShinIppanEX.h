#pragma once
#include "icsdiagctrl.h"

// CShinIppanEX ダイアログ

enum {
	IPPANEX_NUM_HOJIN1			= 108,	// 法人番号１
	IPPANEX_NUM_HOJIN2,					// 法人番号２
	IPPANEX_NUM_HOJIN3,					// 法人番号３
	IPPANEX_NUM_HOJIN4,					// 法人番号４
	IPPANEX_NUM_HOJINHYPHEN1,			// 法人番号ハイフン
	IPPANEX_NUM_HOJINHYPHEN2,			// 法人番号ハイフン
	IPPANEX_NUM_HOJINHYPHEN3,			// 法人番号ハイフン
	IPPANEX_NUM_KOJIN1,					// 個人番号１
	IPPANEX_NUM_KOJIN2,					// 個人番号２
	IPPANEX_NUM_KOJIN3,					// 個人番号３
	IPPANEX_NUM_KOJINHYPHEN1,			// 個人番号ハイフン
	IPPANEX_NUM_KOJINHYPHEN2			// 個人番号ハイフン
};

class CShinIppanEX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinIppanEX)

public:
	CShinIppanEX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinIppanEX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IPPAN_EX };


	// 初期情報のセット
	int InitInfo( CH28SnHonpyoData **pSnHonpyoData, CH28SnFhyo10Data **pSnFhyo10Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// 初期ポジションセット
	void SetItemPosition();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// ダイアグラム属性変更
	int ChgDiagAttr();
	// データの読込み
	int ReadData();
	// データの書込み
	int WriteData();

	// 本表金額欄表示
	void DispIppan();
//2016.10.20 INSERT START
	// 本表上部表示
	void DispIppanUpper();
//2016.10.20 INSERT END
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 本表元号表示
	void DispGengo();

	// 金額の最終データ読込み
	void GetDiagData();
	// プレプリント
	void PerPrint();
//2016.02.23 INSERT START
	void PerPrint2(); //欄外メモ
//2016.02.23 INSERT END
	// フォーカス位置を顧問先情報と関係の無い位置にずらす('15.01.14)
	void ShiftHonpyoFocus();
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

//2015.12.08 INSERT START
	// マイナンバーエディットコントロール位置調整
	void MoveDiagMNEdit();
	// マイナンバーコントロールの表示を消す
	void HideMNEdit();
//2015.12.08 INSERT END

	//== 計算 ==
	// 他表からの計算
	void calq_by_OtherChange( int ksw, BOOL IsFromTkkz=FALSE );
	// 各入力の計算表示
	void calqx_all( int atno, int sw );
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
	//[Y] 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );

//2016.06.22 INSERT START
	BOOL IsKanpu();
//2016.06.22 INSERT END

private:
	CWnd				*m_pParent;				// 親ウィンドウ

	//== 帳票データ ==
	char				XM1[MONY_BUF_SIZE],		//  1. 課税標準額
						XM2[MONY_BUF_SIZE],		//  2. 消費税額計
						XM3[MONY_BUF_SIZE],		//  3. 控除過大調整税額
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
						XM19[MONY_BUF_SIZE],	// 19. 還付額
						XM20[MONY_BUF_SIZE],	// 20. 納税額
						XM21[MONY_BUF_SIZE],	// 21. 中間納付譲渡割額
						XM22[MONY_BUF_SIZE],	// 22. 納付譲渡割額
						XM23[MONY_BUF_SIZE],	// 23. 中間還付譲渡割額
						XM24[MONY_BUF_SIZE],	// 24. 既確定譲渡割額
						XM25[MONY_BUF_SIZE],	// 25. 差引納付譲渡割額
						XM26[MONY_BUF_SIZE];	// 26. 消費税･地方消費税合計


	int					SW_WARIFU;				// 割賦基準の適用( 1:適用, 0:未)
	int					SW_NOBEBA;				// 延払基準の適用( 1:適用, 0:未)
	int					SW_KOUJIS;				// 工事進行基準の適用( 1:適用, 0:未)
	int					SW_GENKIN;				// 現金主義会計の適用( 1:適用, 0:未)
	int					SW_KISK22;				// 規則22条1項の適用
	int					SW_WITH_TKKZ;			// 特定課税仕入に係る別表の添付（1:有, 0:無）	'15.08.28

	int					TS_KUBUN1;				// 1・2の内訳:地方消費税　区分
	char				KN3[MONY_BUF_SIZE];		// 1・2の内訳:地方消費税　課税標準額
	char				SN3[MONY_BUF_SIZE];		// 1・2の内訳:地方消費税　消費税額
	int					TS_KUBUN2;				// 17・18の内訳:地方消費税　区分
	char				CM3[MONY_BUF_SIZE];		// 17・18の内訳:地方消費税　課税標準額

	//== 帳票データ本体 ==
	CH28SnHonpyoData	**m_pSnHonpyoData;		// 本表データ
	CH28SnFhyo10Data	**m_pSnFhyo10Data;		// 付表１データ
	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報

	CArithEx			*m_pArith;
	CSyzUtil			m_Util;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報

	int					m_kanisw;				// 簡易課税ＳＷ	(0:通常、免税, 2以上:簡易課税)
	int					m_DpSw;					//
	char				m_M1c;					// 
	int					m_curwnd;
	int					m_curInPos;				// 現入力位置保存('15.03.02)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_IppanDiag01;
	CIcsdiagctrl m_IppanDiag02;
	CIcsdiagctrl m_IppanDiag03;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl3(short index);
	afx_msg void OnBnClickedButton1();
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void TerminationIcsdiagctrl3(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl1(short index);
	void EditONIcsdiagctrl2(short index);
	void EditONIcsdiagctrl3(short index);


	CICSMNEditController8	m_ctrl_mynum;
	CICSMNEdit8				m_edit_mynum1;
	CICSMNEdit8 			m_edit_mynum2;
	CICSMNEdit8 			m_edit_mynum3;
	CICSMNEdit8 			m_edit_mynum4;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
