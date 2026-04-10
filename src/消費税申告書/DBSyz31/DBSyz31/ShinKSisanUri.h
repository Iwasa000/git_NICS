#pragma once

#include "SimulateKuriwari.h"
#include "H31HyoView.h"

// CShinKSisanUri ダイアログ

class CShinKSisanUri : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinKSisanUri)

public:
	CShinKSisanUri(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinKSisanUri();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KSISAN_URI };

	int Init();

	int InitInfo(CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei, int snseq);
	int InitDiagAttr();
    void SetItemPosition();
    void InitTblInfo();

    void SetSyukeiOnRendo();
	int SetSyukeiDataGensoku(SIMKURIWARIZREC* pRec);
	int SetSyukeiDataKanni(SIMKURIWARIZREC* pRec);

    int TokureiTblVersionCheck(int sn_seq);

	int ReadData(int sn_seq, int version, CString filter);
	int ReadSub_data(CSYZ_TOKUREI* pTokurei);

	int WriteData(int sn_seq, int version);
	void WriteSub_data(CSYZ_TOKUREI* pTokurei, int sn_seq, int version);

	int Update_data(CSYZ_TOKUREI* pTokurei);

    int GetMxPrtPage();

	// 画面出力
	void DispKsisan_Uri();

	void dsp_prs(unsigned short Id, short Index, char *Mony); 	// 金額表示
	void dsp_cls(unsigned short Id, short Index); 	            // 金額表示クリア

	void GetAllDiagData();
	void ClearData(bool bFromRenew=false);

	// フォーカス移動処理（上下）
	int UpDownFocusMoveCheck( int Index, int Move );

	// indexからメモリ上の列番号を取得
	int GetCol_FromIndex(int index);

	// 該当の列にデータが入っているかどうか
	BOOL IsNULL_Column( int Clmindex );

	// データの初期化
	void Init_KubunColumn( int ClmIndex );

	// データの移動
	void MoveData( int ClmIndex, int sign );

	// 再集計
	void ReCalq();

	// 列番号の表示
	void DispClmNum();

	BOOL IsRightEnd(int index, int* pNextPos = NULL);
	BOOL IsLeftEnd(int index, int* pNextPos = NULL);

	// 事業区分のスクロールバーを画面下部に表示する
	void MoveWindow_JigyoKubunBar(bool bFromView=false);

	//==== 各データ処理（色変更、データセットなど） ====//
    void SetKazeikikan(); 
	void SetTekiyoTaisyokikan();
    void SetName();
	void Set_KSURI_data();
    void SetSub_KSURI_InputLockColor(short index);
	void SetSub_KSURI_data_kindval(int tblidx, int clmidx);
	void Update_KSURI_5(short index, int clmidx);
	void Update_KSURI_6(short index, int clmidx, bool bDel = false);
	void Update_KSURI_6_KEI();
	void Update_KSURI_7(short index, int clmidx, bool bDel = false);
	void Update_KSURI_7_KEI();
	void Update_KSURI_10();
	void Update_KSURI_11();
	void Check_KSURI_5Check(short index, int clmidx);
	void CheckSub_KSURI_5CheckOn(short index, int clmidx, bool bDel = false);
	void CheckSub_KSURI_5CheckOn_KanniRendo(int clmidx);
	void CheckSub_KSURI_5CheckOff(short index, int clmidx);
	void Calc_KSURI_6(short index, int clmidx, char* pVal);
	void Calc_KSURI_7(short index, int clmidx, char* pVal);
	void CheckSetData_KSURI_6(short index, int clmidx);
	void CheckSetData_KSURI_7(short index, int clmidx);
	int CheckSub_ChangeBackColor(short index, char* pVal1, char* pVal2, bool bReset = false);
	void ChangeDiagAttr();

private:
	CWnd			*m_pParent;		// 親ウィンドウ

	//== 帳票データ本体 ==//
	CSnHeadData		**m_pSnHeadData;	// 消費税ヘッダ情報
	SYC_SHININFO	*m_pShinInfo;		// 共通情報
	CArithEx		*m_pArith;
	CDBNpSub		*m_pZmSub;			// 財務クラス
    CDBSyzAccess    *m_pDBSyzAccess;    
	CSyzSyukei		**m_pSyzSyukei;		// 集計クラス

	int				m_curwnd;
	int				m_dispCol_left;		// 表示している左端の列番号
	int				m_Focus;			// フォーカスのあるindex

	int				m_SnSeq;			// 履歴シーケンス

	CIcsdiagctrl m_KsisanUriDiag01;
	CIcsdiagctrl m_KsisanUriDiag02;
	CScrollBar m_bar;
	CButton m_ButtonBsday;
	CButton m_ButtonUriSyukei;
	CButton m_ButtonUriColIns;
	CButton m_ButtonUriColDel;

    CSyzUtil m_SyzUtil;
	EnumIdIcsShKazeihoushiki m_kazeihousiki;

	EnumTokureiMode m_TokureiMode;
	long m_tekiyotaisyokikan_s;
	long m_tekiyotaisyokikan_e;

	//== 帳票データ ==//
	TBLDATAINFO kbndatainfo[12];
	TBLDATAINFO datainfo[6];
	char KSURI_TITLE[JIGYO_MAX_TOKUREI_1][64];		  // 事業の区分ごとの計算
	char KSURI_1[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // １．課税資産の譲渡等の税込価額の合計額
	char KSURI_2_S[JIGYO_MAX_TOKUREI_1][4];			  // ２．通常の事業を行う連続する１０営業日　（自）
	char KSURI_2_E[JIGYO_MAX_TOKUREI_1][4];			  // ２．通常の事業を行う連続する１０営業日　（至）
	char KSURI_3[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ３．2の期間中に行った課税資産の譲渡等の税込価額の合計額
	char KSURI_4[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ４．のうち、軽減対象資産の譲渡等（税込み6.24%適用分）に係る部分の金額（税込み）
	char KSURI_5[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ５．軽減売上割合
	int  KSURI_5_CHECK[JIGYO_MAX_TOKUREI_1];	      // ５．チェック状態.
	char KSURI_6[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ６．軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）
	int  KSURI_6_SIGN[JIGYO_MAX_TOKUREI_1];	          // ６．実額編集サイン.
	char KSURI_7[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ７．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	int  KSURI_7_SIGN[JIGYO_MAX_TOKUREI_1];	          // ７．実額編集サイン.
	
	char KSURI_6_KEI[MONY_BUF_SIZE];		          // ６．軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）　合計額
	char KSURI_7_KEI[MONY_BUF_SIZE];		          // ７．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　合計額
	char KSURI_8[MONY_BUF_SIZE];			          // ８．軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char KSURI_9[MONY_BUF_SIZE];			          // ９．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	char KSURI_10[MONY_BUF_SIZE];			          // １０．軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char KSURI_11[MONY_BUF_SIZE];			          // １１．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

    //□50%適用時一時データ保存用.
	char KSURI_2_S_TMP[JIGYO_MAX_TOKUREI_1][4];			  // ２．通常の事業を行う連続する１０営業日　（自）
	char KSURI_2_E_TMP[JIGYO_MAX_TOKUREI_1][4];			  // ２．通常の事業を行う連続する１０営業日　（至）
	char KSURI_3_TMP[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ３．2の期間中に行った課税資産の譲渡等の税込価額の合計額
	char KSURI_4_TMP[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE]; // ４．のうち、軽減対象資産の譲渡等（税込み6.24%適用分）に係る部分の金額（税込み）

	//第？種事業区分（連動時のみ使用）
	char KSURI_KBN[JIGYO_MAX_TOKUREI_1];

	//-->yoshida190919
	//販売奨励金費用・売上値引を減算した額（付表２転記用額）
	char HENKAN_GOKEI[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];						  // 返還金額合計値
	char KSURI_NEBIKI_KEI8[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];			          // 売上変換、軽減割合で計算した結果（８％軽減）
	char KSURI_NEBIKI_10[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];			          // 売上変換、軽減割合で計算した結果（１０％）

	char KSURI_NEBIKI_KEI8_FHYO1[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];			  // 売上変換、軽減割合で計算した結果（８％軽減） 付表１転記用
	char KSURI_NEBIKI_10_FHYO1[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];			      // 売上変換、軽減割合で計算した結果（１０％） 付表１転記用


	char KASISON_GOKEI[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];						  // 貸倒損失合計値
	char KSURI_KASISON_KEI8[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];			      // 貸倒損失、軽減割合で計算した結果（８％軽減）
	char KSURI_KASISON_10[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];				      // 貸倒損失、軽減割合で計算した結果（１０％）
	char KASIKAISYU_GOKEI[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];					  // 貸倒回収合計値
	char KSURI_KASIKAISYU_KEI8[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];			      // 貸倒回収、軽減割合で計算した結果（８％軽減）
	char KSURI_KASIKAISYU_10[JIGYO_MAX_TOKUREI_1][MONY_BUF_SIZE];				  // 貸倒回収、軽減割合で計算した結果（１０％）
	//<--------------


public:
	CSimKuriwari* m_pSimKuriwari;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonBsday();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_EVENTSINK_MAP()

	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data);
	afx_msg void OnBnClickedKubunDel();
	afx_msg void OnBnClickedKubunIns();
	afx_msg void OnBnClickedButtonUriSyukei();
};
