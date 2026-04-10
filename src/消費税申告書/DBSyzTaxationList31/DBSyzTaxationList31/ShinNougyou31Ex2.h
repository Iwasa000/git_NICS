#pragma once
#include "SyzBaseDlg.h"
#include "stdafx.h"

class CShinNougyou31Ex2 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinNougyou31Ex2)

public:
	CShinNougyou31Ex2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinNougyou31Ex2();

	// ダイアログ データ
	//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NOUGYOU31_ex2 };

	//#endif

private:
	CWnd		*m_pParent;		// 親ウィンドウ
	CSyzUtil	m_Util;

	int			m_FcsUpEnd;		// 入力可能なセル ( 上移動での末端 )
	int			m_FcsLeftEnd;	// 入力可能なセル ( 左移動での末端 )
	int			m_FcsDownEnd;	// 入力可能なセル ( 下移動での末端 )
	int			m_FcsRightEnd;	// 入力可能なセル ( 右移動での末端 )

	int			m_Idx;
	short		m_Lin;
	short		m_Clm;

	int			m_curwnd;

	// 印刷情報
	CVPrint				*m_pVprn;			// 出力クラス
	NPRT_PAR			*m_pPset;			// 印刷情報
	//PBUF				PF[30];
	PBUF				PF[50];
	SH_PRINT_CMINFO		*m_PrintCmInfo;

	const static int clmSize = 7;	//帳票の列数(科目列抜き)
	enum clmId {
		clmA = 0, clmB, clmC, clmD, clmE, clmF, clmG,
	};
	const static int rowSize = 39;	//帳票の行数(1-36+対価)
	const int idxEditMin = 285;	// 入力ボックス(金額)の最小のインデックス
	const int idxEditMax = 443;	// 入力ボックス(金額)の最大のインデックス
	const int idxYear = 1;
	const int idxZkbn = 496; // 経理方式


	const int autoCalcChk2 = 519; // 自動計算チェック	うち軽減税率6.24%適用分  22-0167,22-0361
	const int autoCalcChk3 = 520; // 　　　 〃 　　　	うち標準税率7.8%適用分   22-0167,22-0361

	enum idx_NougyouEx2 {
		nouName27  = 282, nouName28,  nouName29,
		nouVal01A  = 285, nouVal01B,  nouVal01C,  nouVal01D,             nouVal01F,             // 販売金額
		nouVal02A  = 290,             nouVal02C,  nouVal02D,             nouVal02F, 	        // 家事消費金額
		nouVal022A = 294, nouVal022B, nouVal022C, nouVal022D,            nouVal022F,            // 事業消費金額
		nouVal03A  = 299, nouVal03B,  nouVal03C,  nouVal03D,             nouVal03F,             // 雑収入
		nouVal032C = 304,                         nouVal032D,            nouVal032F,            // 未成熟果樹収入
		nouVal04A  = 307, nouVal04B,  nouVal04C,  nouVal04D,             nouVal04F,             // 小計
		nouVal042A = 312, nouVal042B, nouVal042C, nouVal042D,            nouVal042F,            // (返還等対価)
		nouVal05A  = 317,		                                                                // 農産物の棚卸高　期首
		nouVal06A  = 318,                                                                       // 農産物の棚卸高　期末
		nouVal07A  = 319,                                                                       // 計

		nouVal08A  = 320, nouVal08B,  nouVal08C,                         nouVal08F,  nouVal08G, // 租税公課
		nouVal09A  = 325, nouVal09B,  nouVal09C,  nouVal09D,  nouVal09E, nouVal09F,  nouVal09G, // 種苗費
		nouVal10A  = 332, nouVal10B,  nouVal10C,                         nouVal10F,  nouVal10G,	// 素畜費
		nouVal11A  = 337, nouVal11B,  nouVal11C,  nouVal11D,  nouVal11E, nouVal11F,  nouVal11G,	// 肥料費
		nouVal12A  = 344, nouVal12B,  nouVal12C,  nouVal12D,  nouVal12E, nouVal12F,  nouVal12G, // 飼料費
		nouVal13A  = 351,             nouVal13C,                         nouVal13F,  nouVal13G, // 農具費
		nouVal14A  = 355,             nouVal14C,                         nouVal14F,  nouVal14G, // 農薬・衛生費
		nouVal15A  = 359,             nouVal15C,                         nouVal15F,  nouVal15G,	// 諸材料費
		nouVal16A  = 363,             nouVal16C,                         nouVal16F,  nouVal16G, // 修繕費
		nouVal17A  = 367, nouVal17B,  nouVal17C,                         nouVal17F,  nouVal17G, // 動力光熱費
		nouVal18A  = 372,             nouVal18C,                         nouVal18F,  nouVal18G, // 作業用衣料費
		nouVal19A  = 376, nouVal19B,                                                            // 農業共済掛金
		nouVal20A  = 378, nouVal20B,                                                            // 減価償却費
		nouVal21A  = 380, nouVal21B,  nouVal21C,                         nouVal21F,  nouVal21G, // 荷造運賃手数料
		nouVal22A  = 385, nouVal22B,  nouVal22C,                         nouVal22F,  nouVal22G, // 雇人費
		nouVal23A  = 390, nouVal23B,                                                            // 利子割引料
		nouVal24A  = 392, nouVal24B,  nouVal24C,                         nouVal24F,  nouVal24G, // 地代・賃借料
		nouVal25A  = 397, nouVal25B,  nouVal25C,                         nouVal25F,  nouVal25G, // 土地改良費
		nouVal26A  = 402, nouVal26B,                                                            // 貸倒金
		nouVal27A  = 404, nouVal27B,  nouVal27C,  nouVal27D,  nouVal27E, nouVal27F,  nouVal27G, // 経費 自由入力欄1
		nouVal28A  = 411, nouVal28B,  nouVal28C,  nouVal28D,  nouVal28E, nouVal28F,  nouVal28G, // 経費 自由入力欄2
		nouVal29A  = 418, nouVal29B,  nouVal29C,  nouVal29D,  nouVal29E, nouVal29F,  nouVal29G, // 経費 自由入力欄3
		nouVal30A  = 425, nouVal30B,  nouVal30C,  nouVal30D,  nouVal30E, nouVal30F,  nouVal30G, // 雑費
		nouVal31A  = 432, nouVal31B,  nouVal31C,  nouVal31D,  nouVal31E, nouVal31F,  nouVal31G, // 小計
		nouVal32A  = 439,                                                                       // 農産物以外の棚卸高　期首
		nouVal33A  = 440,                                                                       // 農産物以外の棚卸高　期末
		nouVal34A  = 441,                                                                       // 経費から差し引く果樹牛馬等の育成費用
		nouVal35A  = 442,                                                                       // 計
		nouVal36A  = 443,                                                                       // 差引金額
	};

	//非表示項目。一応区別できるようにする
	enum disabledCell_NougyouEx2 {
		nouVal01E = -200,   //最後尾の値が正にならないような負の値 
		nouVal01G,
		nouVal02B,  nouVal02E,  nouVal02G,
		nouVal022E, nouVal022G,
		nouVal03E,  nouVal03G,
		nouVal032A, nouVal032B, nouVal032E, nouVal032G,
		nouVal04E,  nouVal04G,
		nouVal042E, nouVal042G,
		nouVal05B,  nouVal05C,  nouVal05D,  nouVal05E,  nouVal05F,  nouVal05G,
		nouVal06B,  nouVal06C,  nouVal06D,  nouVal06E,  nouVal06F,  nouVal06G,
		nouVal07B,  nouVal07C,  nouVal07D,  nouVal07E,  nouVal07F,  nouVal07G,
		nouVal08D,  nouVal08E,
		nouVal10D,  nouVal10E,
		nouVal13B,  nouVal13D,  nouVal13E,
		nouVal14B,  nouVal14D,  nouVal14E,
		nouVal15B,  nouVal15D,  nouVal15E,
		nouVal16B,  nouVal16D,  nouVal16E,
		nouVal17D,  nouVal17E,
		nouVal18B,  nouVal18D,  nouVal18E,
		nouVal19C,  nouVal19D,  nouVal19E,  nouVal19F,  nouVal19G,
		nouVal20C,  nouVal20D,  nouVal20E,  nouVal20F,  nouVal20G,
		nouVal21D,  nouVal21E,
		nouVal22D,  nouVal22E,
		nouVal23C,  nouVal23D,  nouVal23E,  nouVal23F,  nouVal23G,
		nouVal24D,  nouVal24E,
		nouVal25D,  nouVal25E,
		nouVal26C,  nouVal26D,  nouVal26E,  nouVal26F,  nouVal26G,
		nouVal32B,  nouVal32C,  nouVal32D,  nouVal32E,  nouVal32F,  nouVal32G,
		nouVal33B,  nouVal33C,  nouVal33D,  nouVal33E,  nouVal33F,  nouVal33G,
		nouVal34B,  nouVal34C,  nouVal34D,  nouVal34E,  nouVal34F,  nouVal34G,
		nouVal35B,  nouVal35C,  nouVal35D,  nouVal35E,  nouVal35F,  nouVal35G,
		nouVal36B,  nouVal36C,  nouVal36D,  nouVal36E,  nouVal36F,  nouVal36G,
	};

	constexpr static std::array<std::array<int, clmSize>, rowSize> idxMap = {{
		{nouVal01A,  nouVal01B,   nouVal01C,   nouVal01D,   nouVal01E,  nouVal01F,   nouVal01G},
		{nouVal02A,  nouVal02B,   nouVal02C,   nouVal02D,   nouVal02E,  nouVal02F,   nouVal02G},
		{nouVal022A, nouVal022B,  nouVal022C,  nouVal022D,  nouVal022E, nouVal022F,  nouVal022G},
		{nouVal03A,  nouVal03B,   nouVal03C,   nouVal03D,   nouVal03E,  nouVal03F,   nouVal03G},
		{nouVal032A, nouVal032B,  nouVal032C,  nouVal032D,  nouVal032E, nouVal032F,  nouVal032G},
		{nouVal04A,  nouVal04B,   nouVal04C,   nouVal04D,   nouVal04E,  nouVal04F,   nouVal04G},
		{nouVal042A, nouVal042B,  nouVal042C,  nouVal042D,  nouVal042E, nouVal042F,  nouVal042G},
		{nouVal05A,  nouVal05B,   nouVal05C,   nouVal05D,   nouVal05E,  nouVal05F,   nouVal05G},
		{nouVal06A,  nouVal06B,   nouVal06C,   nouVal06D,   nouVal06E,  nouVal06F,   nouVal06G},
		{nouVal07A,  nouVal07B,   nouVal07C,   nouVal07D,   nouVal07E,  nouVal07F,   nouVal07G},
		{nouVal08A,  nouVal08B,   nouVal08C,   nouVal08D,   nouVal08E,  nouVal08F,   nouVal08G},
		{nouVal09A,  nouVal09B,   nouVal09C,   nouVal09D,   nouVal09E,  nouVal09F,   nouVal09G},
		{nouVal10A,  nouVal10B,   nouVal10C,   nouVal10D,   nouVal10E,  nouVal10F,   nouVal10G},
		{nouVal11A,  nouVal11B,   nouVal11C,   nouVal11D,   nouVal11E,  nouVal11F,   nouVal11G},
		{nouVal12A,  nouVal12B,   nouVal12C,   nouVal12D,   nouVal12E,  nouVal12F,   nouVal12G},
		{nouVal13A,  nouVal13B,   nouVal13C,   nouVal13D,   nouVal13E,  nouVal13F,   nouVal13G},
		{nouVal14A,  nouVal14B,   nouVal14C,   nouVal14D,   nouVal14E,  nouVal14F,   nouVal14G},
		{nouVal15A,  nouVal15B,   nouVal15C,   nouVal15D,   nouVal15E,  nouVal15F,   nouVal15G},
		{nouVal16A,  nouVal16B,   nouVal16C,   nouVal16D,   nouVal16E,  nouVal16F,   nouVal16G},
		{nouVal17A,  nouVal17B,   nouVal17C,   nouVal17D,   nouVal17E,  nouVal17F,   nouVal17G},
		{nouVal18A,  nouVal18B,   nouVal18C,   nouVal18D,   nouVal18E,  nouVal18F,   nouVal18G},
		{nouVal19A,  nouVal19B,   nouVal19C,   nouVal19D,   nouVal19E,  nouVal19F,   nouVal19G},
		{nouVal20A,  nouVal20B,   nouVal20C,   nouVal20D,   nouVal20E,  nouVal20F,   nouVal20G},
		{nouVal21A,  nouVal21B,   nouVal21C,   nouVal21D,   nouVal21E,  nouVal21F,   nouVal21G},
		{nouVal22A,  nouVal22B,   nouVal22C,   nouVal22D,   nouVal22E,  nouVal22F,   nouVal22G},
		{nouVal23A,  nouVal23B,   nouVal23C,   nouVal23D,   nouVal23E,  nouVal23F,   nouVal23G},
		{nouVal24A,  nouVal24B,   nouVal24C,   nouVal24D,   nouVal24E,  nouVal24F,   nouVal24G},
		{nouVal25A,  nouVal25B,   nouVal25C,   nouVal25D,   nouVal25E,  nouVal25F,   nouVal25G},
		{nouVal26A,  nouVal26B,   nouVal26C,   nouVal26D,   nouVal26E,  nouVal26F,   nouVal26G},
		{nouVal27A,  nouVal27B,   nouVal27C,   nouVal27D,   nouVal27E,  nouVal27F,   nouVal27G},
		{nouVal28A,  nouVal28B,   nouVal28C,   nouVal28D,   nouVal28E,  nouVal28F,   nouVal28G},
		{nouVal29A,  nouVal29B,   nouVal29C,   nouVal29D,   nouVal29E,  nouVal29F,   nouVal29G},
		{nouVal30A,  nouVal30B,   nouVal30C,   nouVal30D,   nouVal30E,  nouVal30F,   nouVal30G},
		{nouVal31A,  nouVal31B,   nouVal31C,   nouVal31D,   nouVal31E,  nouVal31F,   nouVal31G},
		{nouVal32A,  nouVal32B,   nouVal32C,   nouVal32D,   nouVal32E,  nouVal32F,   nouVal32G},
		{nouVal33A,  nouVal33B,   nouVal33C,   nouVal33D,   nouVal33E,  nouVal33F,   nouVal33G},
		{nouVal34A,  nouVal34B,   nouVal34C,   nouVal34D,   nouVal34E,  nouVal34F,   nouVal34G},
		{nouVal35A,  nouVal35B,   nouVal35C,   nouVal35D,   nouVal35E,  nouVal35F,   nouVal35G},
		{nouVal36A,  nouVal36B,   nouVal36C,   nouVal36D,   nouVal36E,  nouVal36F,   nouVal36G},
	}};

	//ダイアグラム上での行数
	enum diagLine {
		nouDiagLine1,  nouDiagLine2K,   nouDiagLine2J, nouDiagLine3,  nouDiagLine3M,
		nouDiagLine4,  nouDiagLine4TA, nouDiagLine5,   nouDiagLine6,  nouDiagLine7,
		nouDiagLine8,  nouDiagLine9,   nouDiagLine10,  nouDiagLine11, nouDiagLine12,
		nouDiagLine13, nouDiagLine14,  nouDiagLine15,  nouDiagLine16, nouDiagLine17,
		nouDiagLine18, nouDiagLine19,  nouDiagLine20,  nouDiagLine21, nouDiagLine22,
		nouDiagLine23, nouDiagLine24,  nouDiagLine25,  nouDiagLine26, nouDiagLine27,
		nouDiagLine28, nouDiagLine29,  nouDiagLine30,  nouDiagLine31, nouDiagLine32,
		nouDiagLine33, nouDiagLine34,  nouDiagLine35,  nouDiagLine36,
	};

	//データ保存位置及び出力時の行数、ShinNougyou31ExIdx.hのKNOULINEと同等
	enum KNVAL_dataLine{
		nouDataLine1 = 0,    //販売金額
		nouDataLine4TA = 38, //返還等対価
		nouDataLine2K = 1, nouDataLine2J, nouDataLine3,  nouDataLine3M, nouDataLine4,
		nouDataLine5,      nouDataLine6,  nouDataLine7,  nouDataLine8,  nouDataLine9,
		nouDataLine10,     nouDataLine11, nouDataLine12, nouDataLine13, nouDataLine14,
		nouDataLine15,     nouDataLine16, nouDataLine17, nouDataLine18, nouDataLine19,
		nouDataLine20,     nouDataLine21, nouDataLine22, nouDataLine23, nouDataLine24,
		nouDataLine25,     nouDataLine26, nouDataLine27, nouDataLine28, nouDataLine29,
		nouDataLine30,     nouDataLine31, nouDataLine32, nouDataLine33, nouDataLine34,
		nouDataLine35,     nouDataLine36,
	};
	
	//diagLineからデータ書き込み位置を指定するための配列
	constexpr static long dataPos[] = { 
		nouDataLine1,  nouDataLine2K,  nouDataLine2J, nouDataLine3,  nouDataLine3M,
		nouDataLine4,  nouDataLine4TA, nouDataLine5,  nouDataLine6,  nouDataLine7,
		nouDataLine8,  nouDataLine9,   nouDataLine10, nouDataLine11, nouDataLine12,
		nouDataLine13, nouDataLine14,  nouDataLine15, nouDataLine16, nouDataLine17,
		nouDataLine18, nouDataLine19,  nouDataLine20, nouDataLine21, nouDataLine22,
		nouDataLine23, nouDataLine24,  nouDataLine25, nouDataLine26, nouDataLine27,
		nouDataLine28, nouDataLine29,  nouDataLine30, nouDataLine31, nouDataLine32,
		nouDataLine33, nouDataLine34,  nouDataLine35, nouDataLine36,
	};


	//const static int FCSitemMax = 134;
	const static int FCSitemMax = 130;
	static FCSUPDOWN_INFO FCS_UD[FCSitemMax + 1];
	static FCSUPDOWN_INFO FCS_LR[FCSitemMax + 1];
	void setFCSArray(); //フォーカス移動用配列の初期化

	bool useSmallFontSize();//印刷時に使用するフォントサイズの判定

public:


	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	void InitDiagAttr();

	//->CShinJigyo31Exからのコピー時に削除した関数 
	//void InitDiagonalLines();    //斜線の表示/非表示、令和2年4月1日改正様式とのリソース切り替え処理用
	//int CheckSVmzsw(int index);  //用途不明、CShinJigyo31Ex内で呼び出されていない
	//void ColumnControl(void);    //自動計算機能使用時の入力制御、入力列が増えたことにより自動計算機能は削除
	//int GetColumnKind(int pInd); //void ColumnControl(void)内でのみ使用する関数
	//void CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data); //自動計算機能用なので未実装
	//<-
	
	
	// 画面表示
	void DispList(int mode);

	// ダイアグラムデータ表示
	void DispNougyouDiagData(long dlin);//仮対応(E,G以外)
	
	// 表示判定
	int DispJudge(int clm, int line);//仮対応(E,G以外)
	

	// 各入力データ取得
	int GetDiagData(long index, short leng, DIAGRAM_DATA *pData, int mode);//仮対応(E,G以外)

	// 手入力サインのクリア
	int ClearManualInputSign();

	// 次フォーカスの場所検索
	int FocusMoveCheck(int Index, int Move);

	// 初期ポジションセット
	virtual void SetItemPosition();
	void SetItemPositionEx();

	// ポジションの末端設定
	virtual void SetFocusRange();

	// 印刷情報のセット
	void SetPrintInfo(CVPrint *pVprn, NPRT_PAR *pPset, SH_PRINT_CMINFO *pPrintCmInfo);

	// 印刷１
	int PrintHyo1(int pno, int fno, CDC *pDC, char *pBp);

	// 印刷２
	int PrintHyo2(int pno, int fno, CDC *pDC, char *pBp);//仮対応(E,G以外)

	// 印刷３
	int PrintHyo3(int pno, int fno, CDC *pDC, char *pBp);//仮対応(E,G以外)

	// 印刷４
	int PrintHyo4(int pno, int fno, CDC *pDC, char *pBp);//仮対応(E,G以外)

	void DiagReSetPosition(void);		// 修正No.168690 add

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CIcsdiagctrl m_NougyouDiag;
	virtual BOOL OnInitDialog();

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


private:
	//エディットボックスへの入力を許可するモード指定
	//自由入力欄項目名は確定時以外入力許可
	//A列の一部とC列は常に入力不可
	//C列がない行のB列にはモードを指定しない
	constexpr static SH_ITEMINDEX idxAttr[] = {	
		{ nouVal01A, 0, 1, 0 },
		{ nouVal01B, 0, 1, 0 },
		{ nouVal01D, 0, 1, 0 },
		{ nouVal01E, 0, 1, 0 },
		{ nouVal01F, 0, 1, 0 },
		{ nouVal02A, 0, 1, 0 },
		//{ nouVal02D, 0, 0, 0 },	// 230802Aw del
		{ nouVal02D, 0, 1, 0 },		// 230802Aw add
		{ nouVal02E, 0, 1, 0 },
		{ nouVal02F, 0, 1, 0 },
		{ nouVal022A, 0, 1, 0 },
		{ nouVal022B, 0, 1, 0 },
		{ nouVal022D, 0, 1, 0 },
		{ nouVal022E, 0, 1, 0 },
		{ nouVal022F, 0, 1, 0 },
		{ nouVal03A, 0, 1, 0 },
		{ nouVal03B, 0, 1, 0 },
		//{ nouVal03D, 0, 0, 0 },	// 230802Aw del
		{ nouVal03D, 0, 1, 0 },		// 230802Aw add
		{ nouVal03E, 0, 1, 0 },
		{ nouVal03F, 0, 1, 0 },
		{ nouVal032C, 0, 1, 0 },
		{ nouVal032D, 0, 1, 0 },
		{ nouVal032E, 0, 1, 0 },
		{ nouVal032F, 0, 1, 0 },
		{ nouVal042A, 0, 1, 0 },
		{ nouVal042B, 0, 1, 0 },
		{ nouVal042D, 0, 1, 0 },
		{ nouVal042E, 0, 1, 0 },
		{ nouVal042F, 0, 1, 0 },
		{ nouVal05A, 0, 1, 0 },
		{ nouVal06A, 0, 1, 0 },
		{ nouVal08A, 0, 1, 0 },
		{ nouVal08B, 0, 1, 0 },
		{ nouVal08D, 0, 1, 0 },
		{ nouVal08E, 0, 0, 0 },
		{ nouVal08F, 0, 1, 0 },
		{ nouVal08G, 0, 1, 0 },
		{ nouVal09A, 0, 1, 0 },
		{ nouVal09B, 0, 1, 0 },
		{ nouVal09D, 0, 1, 0 },
		{ nouVal09E, 0, 1, 0 },
		{ nouVal09F, 0, 1, 0 },
		{ nouVal09G, 0, 1, 0 },
		{ nouVal10A, 0, 1, 0 },
		{ nouVal10B, 0, 1, 0 },
		{ nouVal10D, 0, 1, 0 },
		{ nouVal10E, 0, 0, 0 },
		{ nouVal10F, 0, 1, 0 },
		{ nouVal10G, 0, 1, 0 },
		{ nouVal11A, 0, 1, 0 },
		{ nouVal11B, 0, 1, 0 },
		{ nouVal11D, 0, 1, 0 },
		{ nouVal11E, 0, 1, 0 },
		{ nouVal11F, 0, 1, 0 },
		{ nouVal11G, 0, 1, 0 },
		{ nouVal12A, 0, 1, 0 },
		{ nouVal12B, 0, 1, 0 },
		{ nouVal12D, 0, 1, 0 },
		{ nouVal12E, 0, 1, 0 },
		{ nouVal12F, 0, 1, 0 },
		{ nouVal12G, 0, 1, 0 },
		{ nouVal13A, 0, 1, 0 },
		{ nouVal13D, 0, 1, 0 },
		{ nouVal13E, 0, 0, 0 },
		{ nouVal13F, 0, 1, 0 },
		{ nouVal13G, 0, 1, 0 },
		{ nouVal14A, 0, 1, 0 },
		{ nouVal14D, 0, 1, 0 },
		{ nouVal14E, 0, 0, 0 },
		{ nouVal14F, 0, 1, 0 },
		{ nouVal14G, 0, 1, 0 },
		{ nouVal15A, 0, 1, 0 },
		{ nouVal15D, 0, 1, 0 },
		{ nouVal15E, 0, 0, 0 },
		{ nouVal15F, 0, 1, 0 },
		{ nouVal15G, 0, 1, 0 },
		{ nouVal16A, 0, 1, 0 },
		{ nouVal16D, 0, 1, 0 },
		{ nouVal16E, 0, 0, 0 },
		{ nouVal16F, 0, 1, 0 },
		{ nouVal16G, 0, 1, 0 },
		{ nouVal17A, 0, 1, 0 },
		{ nouVal17B, 0, 1, 0 },
		{ nouVal17D, 0, 1, 0 },
		{ nouVal17E, 0, 0, 0 },
		{ nouVal17F, 0, 1, 0 },
		{ nouVal17G, 0, 1, 0 },
		{ nouVal18A, 0, 1, 0 },
		{ nouVal18D, 0, 1, 0 },
		{ nouVal18E, 0, 0, 0 },
		{ nouVal18F, 0, 1, 0 },
		{ nouVal18G, 0, 1, 0 },
		{ nouVal19A, 0, 1, 0 },
		{ nouVal20A, 0, 1, 0 },
		{ nouVal21A, 0, 1, 0 },
		{ nouVal21B, 0, 1, 0 },
		{ nouVal21D, 0, 1, 0 },
		{ nouVal21E, 0, 0, 0 },
		{ nouVal21F, 0, 1, 0 },
		{ nouVal21G, 0, 1, 0 },
		{ nouVal22A, 0, 1, 0 },
		{ nouVal22B, 0, 1, 0 },
		{ nouVal22D, 0, 1, 0 },
		{ nouVal22E, 0, 0, 0 },
		{ nouVal22F, 0, 1, 0 },
		{ nouVal22G, 0, 1, 0 },
		{ nouVal23A, 0, 1, 0 },
		{ nouVal24A, 0, 1, 0 },
		{ nouVal24B, 0, 1, 0 },
		{ nouVal24D, 0, 1, 0 },
		{ nouVal24E, 0, 0, 0 },
		{ nouVal24F, 0, 1, 0 },
		{ nouVal24G, 0, 1, 0 },
		{ nouVal25A, 0, 1, 0 },
		{ nouVal25B, 0, 1, 0 },
		{ nouVal25D, 0, 1, 0 },
		{ nouVal25E, 0, 0, 0 },
		{ nouVal25F, 0, 1, 0 },
		{ nouVal25G, 0, 1, 0 },
		{ nouVal26A, 0, 1, 0 },
//		{ nouVal26B, 0, 1, 0 },	// 230727Aw del
		{ nouVal26B, 0, 0, 0 },	// 230727Aw add
		{ nouVal27A, 0, 1, 0 },
		{ nouVal27B, 0, 1, 0 },
		{ nouVal27D, 0, 1, 0 },
		{ nouVal27E, 0, 1, 0 },
		{ nouVal27F, 0, 1, 0 },
		{ nouVal27G, 0, 1, 0 },
		{ nouVal28A, 0, 1, 0 },
		{ nouVal28B, 0, 1, 0 },
		{ nouVal28D, 0, 1, 0 },
		{ nouVal28E, 0, 1, 0 },
		{ nouVal28F, 0, 1, 0 },
		{ nouVal28G, 0, 1, 0 },
		{ nouVal29A, 0, 1, 0 },
		{ nouVal29B, 0, 1, 0 },
		{ nouVal29D, 0, 1, 0 },
		{ nouVal29E, 0, 1, 0 },
		{ nouVal29F, 0, 1, 0 },
		{ nouVal29G, 0, 1, 0 },
		{ nouVal30A, 0, 1, 0 },
		{ nouVal30B, 0, 1, 0 },
		{ nouVal30D, 0, 1, 0 },
		{ nouVal30E, 0, 1, 0 },
		{ nouVal30F, 0, 1, 0 },
		{ nouVal30G, 0, 1, 0 },
		{ nouVal32A, 0, 1, 0 },
		{ nouVal33A, 0, 1, 0 },
		{ nouVal34A, 0, 1, 0 },
		{ nouName27, 1, 1, 0 },
		{ nouName28, 1, 1, 0 },
		{ nouName29, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	};
};

