#pragma once

#include "SimulateKuriwari.h"
#include "H31HyoView.h"


// CShinKshiire ダイアログ

class CShinKshiire : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinKshiire)

public:
	CShinKshiire(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinKshiire();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KSHIIRE };
#endif

	int InitInfo( CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei, int snseq);
	void Init();
	int InitDiagAttr();
	void SetItemPosition();
	void InitTblInfo();

    void SetSyukeiOnRendo( int sign = 0 );
	void SetSyukeiData(SIMKSIREWARIZREC* pSirewariRec, SIMKURIWARIZREC* pUriwariRec);

	int TokureiTblVersionCheck(int sn_seq);
    void TokureiTblV2ToV3(CSYZ_TOKUREI* pTokurei, int sn_seq);

	int ReadData(int sn_seq, int version, CString filter);
	int ReadSub_data(CSYZ_TOKUREI* pTokurei);

	int WriteData(int sn_seq, int version);
    void WriteSub_data(CSYZ_TOKUREI* pTokurei, int sn_seq, int version);

	int Update_data(CSYZ_TOKUREI* pTokurei);

    int GetMxPrtPage();

	void DispKshiire(); // 画面出力

	void dsp_prs(unsigned short Id, short Index, char *Mony); // 金額表示
	void dsp_cls(unsigned short Id, short Index);             // 金額表示クリア

	void GetAllDiagData();
    void ClearData();

	// フォーカス移動処理（上下）Move = 0:下方向 Move = 1:上方向
	int UpDownFocusMoveCheck(int Index, int Move);

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

	// 小売等軽減売上割合を再計算
	void ResetRate();

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
    void Set_KSHIIRE_data();
    void SetSub_KSHIIRE_InputLockColor(short index);
	void Update_KSHIIRE_3(short index, int clmidx, bool bDel=false);
	void Update_KSHIIRE_6(short index, int clmidx);
	void Update_KSHIIRE_7(short index, int clmidx, bool bDel=false);
	void Update_KSHIIRE_7_KEI(short index, int clmidx);
	void Update_KSHIIRE_8(short index, int clmidx, bool bDel=false);
	void Update_KSHIIRE_8_KEI(short index, int clmidx);
	void Update_KSHIIRE_121(short index, bool bDel=false);
	void Update_KSHIIRE_122(short index, bool bDel=false);
	void Update_KSHIIRE_142(short index, bool bDel=false);
	void Update_KSHIIRE_17();
	void Update_KSHIIRE_18();
	void Update_KSHIIRE_19();
	void Update_KSHIIRE_20();
	void Calc_KSHIIRE_3(short index, int clmidx, char* pVal);
	void Calc_KSHIIRE_7(short index, int clmidx, char* pVal);
	void Calc_KSHIIRE_8(short index, int clmidx, char* pVal);
	void Calc_KSHIIRE_121(short index, char* pVal);
	void Calc_KSHIIRE_122(short index, char* pVal);
	void Calc_KSHIIRE_142(short index, char* pVal);
	void CheckSetData_KSHIIRE_3(short index, int clmidx);
	void CheckSetData_KSHIIRE_7(short index, int clmidx);
	void CheckSetData_KSHIIRE_8(short index, int clmidx);
	void CheckSetData_KSHIIRE_121(short index);
	void CheckSetData_KSHIIRE_122(short index);
	void CheckSetData_KSHIIRE_142(short index);
	int CheckSub_ChangeBackColor(short index, char* pVal1, char* pVal2, bool bReset = false);
    bool Get50CheckStatusColumn0_5_1();

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

	CIcsdiagctrl m_KshiireDiag01;
	CIcsdiagctrl m_KshiireDiag02;
	CScrollBar m_bar;
	CButton m_ButtonShiireSyukei;
	CButton m_ButtonShiireColIns;
	CButton m_ButtonShiireColDel;

	EnumTokureiMode m_TokureiMode;
	long m_tekiyotaisyokikan_s;
	long m_tekiyotaisyokikan_e;

	//== 帳票データ ==//
	TBLDATAINFO kbndatainfo[14];
	TBLDATAINFO datainfo[21];

	char KSHIIRE_TITLE[JIGYO_MAX_TOKUREI_23][64];		  //事業の区分ごとの計算
	char KSHIIRE_1[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //１.課税資産の譲渡等（免税取引及び旧税率(6.3%等)が適用される取引は除く。）の税込み価格の合計額
	char KSHIIRE_2[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //２.軽減対象資産の譲渡等(6.24%適用分)の税率価額の合計額
	char KSHIIRE_3[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //３.小売等軽減売上割合（2/1）
	int  KSHIIRE_3_SIGN[JIGYO_MAX_TOKUREI_23];           //３.実額編集サイン
	char KSHIIRE_4[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //４.課税仕入れに係る支払対価の額（税込み）
	char KSHIIRE_5[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //５.保税地域から引き取った課税貨物に係る税込引取価額
	char KSHIIRE_6[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //６.課税仕入れに係る支払対価の額等の合計額
	char KSHIIRE_7[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額
	int  KSHIIRE_7_SIGN[JIGYO_MAX_TOKUREI_23];           //７.実額編集サイン
	char KSHIIRE_sirehenkannkei8[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE];
	char KSHIIRE_8[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE]; //８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額
	int  KSHIIRE_8_SIGN[JIGYO_MAX_TOKUREI_23];           //８.実額編集サイン
	char KSHIIRE_sirehenkann10[JIGYO_MAX_TOKUREI_23][MONY_BUF_SIZE];
	
	char KSHIIRE_7_KEI[MONY_BUF_SIZE];        //７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額　合計
	char KSHIIRE_8_KEI[MONY_BUF_SIZE];        //８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額　合計
	char KSHIIRE_9[MONY_BUF_SIZE];            //９.納税義務の免税を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率6.24%適用分
	char KSHIIRE_10[MONY_BUF_SIZE];           //１０.納税義務の免税を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率7.8%適用分
	char KSHIIRE_11_1[MONY_BUF_SIZE];         //１１.課税仕入れに係る支払対価の額（税込み）　税率6.24%適用分
	char KSHIIRE_11_2[MONY_BUF_SIZE];         //１１.課税仕入れに係る支払対価の額（税込み）　税率7.8%適用分
	char KSHIIRE_12_1[MONY_BUF_SIZE];         //１２.課税仕入れに係る消費税額　税率6.24%適用分
	int  KSHIIRE_12_1_SIGN;                   //１２－１.実額編集サイン
	char KSHIIRE_12_2[MONY_BUF_SIZE];         //１２.課税仕入れに係る消費税額　税率7.8%適用分
	int  KSHIIRE_12_2_SIGN;                   //１２－２.実額編集サイン
	char KSHIIRE_13_2[MONY_BUF_SIZE];         //１３.特定課税仕入れに係る支払対価の額　税率7.8%適用分
	char KSHIIRE_14_2[MONY_BUF_SIZE];         //１４.特定課税仕入れに係る消費税額　税率7.8%適用分
	char KSHIIRE_14_2_SIGN;                   //１４.実額編集サイン.
	char KSHIIRE_15_1[MONY_BUF_SIZE];         //１５.課税貨物に係る消費税額　税率6.24%適用分
	char KSHIIRE_15_2[MONY_BUF_SIZE];         //１５.課税貨物に係る消費税額　税率7.8%適用分
	char KSHIIRE_16_1[MONY_BUF_SIZE];         //１６.納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整（加算又は減算）額　税率6.24%適用分
	char KSHIIRE_16_2[MONY_BUF_SIZE];         //１６.納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整（加算又は減算）額　税率7.8%適用分
	char KSHIIRE_17[MONY_BUF_SIZE];           //１７.課税仕入れ等の税額の合計額　税率6.24%適用分
	char KSHIIRE_18[MONY_BUF_SIZE];           //１８.課税仕入れ等の税額の合計額　税率7.8%適用分
	char KSHIIRE_19[MONY_BUF_SIZE];           //１９.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額の合計額
	char KSHIIRE_20[MONY_BUF_SIZE];           //２０.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額の合計額

public:
	CSimKuriwari* m_pSimKuriwari;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonShiiresyukei();
	afx_msg void OnBnClickedKubunIns();
	afx_msg void OnBnClickedKubunDel();

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

};
