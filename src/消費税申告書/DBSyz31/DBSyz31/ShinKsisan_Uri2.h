#pragma once
#include "icsdiagctrl.h"
#include "SimulateKuriwari.h"
#include "H31HyoView.h"

// CShinKsisan_Uri2 ダイアログ

class CShinKsisan_Uri2 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinKsisan_Uri2)

public:
	CShinKsisan_Uri2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinKsisan_Uri2();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KSISAN_URI2 };
#endif

	// 初期情報のセット
	int InitInfo(CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei, int snseq);

	int Init();
	int InitDiagAttr();
	void SetItemPosition();
	void InitTblInfo();

    void SetSyukeiOnRendo();
    int SetSyukeiData(SIMKSIREWARIZREC* pRec, SIMKURIWARIZREC* pRec2=NULL);

	int TokureiTblVersionCheck(int sn_seq);

	int ReadData(int sn_seq, int version, CString filter);
	int ReadSub_data(CSYZ_TOKUREI* pTokurei);

	int WriteData(int sn_seq, int version);
	void WriteSub_data(CSYZ_TOKUREI* pTokurei, int sn_seq, int version);

	int Update_data(CSYZ_TOKUREI* pTokurei);

    int GetMxPrtPage();

	// 画面出力
	void DispKsisan_Uri2();

	void dsp_prs(unsigned short Id, short Index, char *Mony); // 金額表示
	void dsp_cls(unsigned short Id, short Index);             // 金額表示クリア

	void GetAllDiagData();  // 各入力データ抽出
	void ClearData(bool bFromRenew = false);

	// フォーカス移動処理（上下）Move = 0:下方向 Move = 1:上方向
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

	BOOL IsRightEnd(int index, int* pNextPos=NULL);
	BOOL IsLeftEnd(int index, int* pNextPos=NULL);

	// 事業区分のスクロールバーを画面最下部に表示
	void MoveWindow_JigyoKubunBar(bool bFromView=false);

	//==== 各データ処理（色変更、データセットなど） ====//
	void SetKazeikikan();
	void SetTekiyoTaisyokikan();
	void SetName();
	void Set_KSURI2_data();
	void SetSub_KSURI2_InputLockColor(short index);
	void SetSub_KSURI2_data_kindval(int tblidx, int clmidx);
    void Update_KSURI2_2_4_TokuteikazeisireOff();
	void Update_KSURI2_4(short index, int clmidx);
	void Update_KSURI2_6(short index, int clmidx);
	void Update_KSURI2_8(short index, int clmidx, bool bDel=false);
	void Update_KSURI2_8_KEI(short index, int clmidx);
	void Update_KSURI2_9(short index, int clmidx, bool bDel=false);
	void Update_KSURI2_9_KEI(short index, int clmidx);
	void Update_KSURI2_12(short index);
	void Update_KSURI2_13(short index);
	void Check_KSURI2_6Check(short index, int clmidx);
	void CheckSub_KSURI2_6CheckOn(short index, int clmidx, bool bDel=false);
	void CheckSub_KSURI2_6CheckOff(short index, int clmidx);
	void Calc_KSURI2_8(short index, int clmidx, char* pVal);
	void Calc_KSURI2_9(short index, int clmidx, char* pVal);
	void CheckSetData_KSURI2_8(short index, int clmidx);
	void CheckSetData_KSURI2_9(short index, int clmidx);
	int CheckSub_ChangeBackColor(short index, char* pVal1, char* pVal2, bool bReset=false);

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

	CSyzUtil      m_SyzUtil;

	CIcsdiagctrl m_Ksisan_Uri2Diag01;
	CIcsdiagctrl m_Ksisan_Uri2Diag02;
	CScrollBar m_bar;
	CButton m_ButtonUri2Syukei;
	CButton m_ButtonUri2ColIns;
	CButton m_ButtonUri2ColDel;

	EnumTokureiMode m_TokureiMode;
	long m_tekiyotaisyokikan_s;
	long m_tekiyotaisyokikan_e;

	//== 帳票データ ==//
	TBLDATAINFO kbndatainfo[13];
	TBLDATAINFO datainfo[6];
	char KSURI2_TITLE[JIGYO_MAX_TOKUREI_23][64];		 //事業の区分ごとの計算
	char KSURI2_1[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //１.課税仕入れに係る支払い対価の額（税込み）
	char KSURI2_2[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //２.特定課税仕入れに係る支払対価の額
	char KSURI2_3[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //３.保税地域から引き取った課税貨物に係る税込引取価額
	char KSURI2_4[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //４.課税仕入れに係る支払対価の額等の合計額
	char KSURI2_5[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //５.４のうち、軽減対象資産の譲渡等（税率6.24%適用分）にのみ要するものの金額（税込み）
	int  KSURI2_6_CHECK[JIGYO_MAX_TOKUREI_23];	        //６.小売等軽減仕入割合チェック状態.
	char KSURI2_6[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //６.小売等軽減仕入割合
	char KSURI2_7[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //７.課税資産の譲渡等の税込価額の合計額
	char KSURI2_8[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
    int  KSURI2_8_SIGN[JIGYO_MAX_TOKUREI_23];           //８.実額編集サイン.
	char KSURI2_9[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	int  KSURI2_9_SIGN[JIGYO_MAX_TOKUREI_23];           //９.実額編集サイン.

	char KSURI2_8_KEI[MONY_BUF_SIZE];		 //８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char KSURI2_9_KEI[MONY_BUF_SIZE];		 //９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	char KSURI2_10[MONY_BUF_SIZE];		     //１０.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char KSURI2_11[MONY_BUF_SIZE];           //１１.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	char KSURI2_12[MONY_BUF_SIZE];		     //１２.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char KSURI2_13[MONY_BUF_SIZE];		     //１３.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

	//□50%適用時一時データ保存用.
	char KSURI2_1_TMP[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //１.課税仕入れに係る支払い対価の額（税込み）
	char KSURI2_2_TMP[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //２.特定課税仕入れに係る支払対価の額
	char KSURI2_3_TMP[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //３.保税地域から引き取った課税貨物に係る税込引取価額
	char KSURI2_4_TMP[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //４.課税仕入れに係る支払対価の額等の合計額
	char KSURI2_5_TMP[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //５.４のうち、軽減対象資産の譲渡等（税率6.24%適用分）にのみ要するものの金額（税込み）


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
	//<------

public:
	CSimKuriwari* m_pSimKuriwari;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonUri2Syukei();
	afx_msg void OnBnClickedKubunIns();
	afx_msg void OnBnClickedKubunDel();

	DECLARE_EVENTSINK_MAP()

	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data);
};
