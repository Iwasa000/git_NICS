#pragma once
#include "SyzBaseDlg.h"
#include "stdafx.h"

class CShinJigyo31Ex2 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinJigyo31Ex2)

public:
	CShinJigyo31Ex2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinJigyo31Ex2();

	// ダイアログ データ
	//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_JIGYO31_ex2 };

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
	PBUF				PF[30];
	SH_PRINT_CMINFO		*m_PrintCmInfo;

	const static int clmSize = 7;	//帳票の列数(科目列抜き)
	enum clmId {
		clmA = 0, clmB, clmC, clmD, clmE, clmF, clmG,
	};
	const static int rowSize = 35;	//帳票の行数(1-34+対価)
	const int idxEditMin = 288;	// 入力ボックス(金額)の最小のインデックス
	const int idxEditMax = 452;	// 入力ボックス(金額)の最大のインデックス
	const int idxYear = 1;
	const int idxZkbn = 459; // 経理方式


	const int autoCalcChk2 = 543; // 自動計算チェック	うち軽減税率6.24%適用分  22-0167,22-0361
	const int autoCalcChk3 = 544; // 　　　 〃 　　　	うち標準税率7.8%適用分   22-0167,22-0361

	enum idx_JigyoEx2 {
		jigVal01A  = 288, jigVal01B,  jigVal01C,  jigVal01D,             jigVal01F,             // 売上（収入）金額
		jigVal012A = 293, jigVal012B, jigVal012C, jigVal012D,            jigVal012F, 	        // (雑収入を含む)
		jigVal02A  = 298,		                                                                // 期首商品棚卸高
		jigVal03A  = 299, jigVal03B,  jigVal03C,  jigVal03D,  jigVal03E, jigVal03F,  jigVal03G, // 仕入金額
		jigVal04A  = 306,		                                                                // 小計
		jigVal05A  = 307,		                                                                // 期末商品棚卸高
		jigVal06A  = 308,		                                                                // 差引原価
		jigVal07A  = 309,		                                                                // 差引金額
		jigVal08A  = 310, jigVal08B,  jigVal08C,                         jigVal08F,  jigVal08G, // 租税公課
		jigVal09A  = 315, jigVal09B,  jigVal09C,                         jigVal09F,  jigVal09G, // 荷造運賃
		jigVal10A  = 320,             jigVal10C,                         jigVal10F,  jigVal10G, // 水道光熱費
		jigVal11A  = 324, jigVal11B,  jigVal11C,                 		 jigVal11F,	 jigVal11G, // 旅費交通費
		jigVal12A  = 329, jigVal12B,  jigVal12C,  jigVal12D,  jigVal12E, jigVal12F,  jigVal12G,	// 通信費
		jigVal13A  = 336, jigVal13B,  jigVal13C,  jigVal13D,  jigVal13E, jigVal13F,  jigVal13G,	// 広告宣伝費
		jigVal14A  = 343, jigVal14B,  jigVal14C,  jigVal14D,  jigVal14E, jigVal14F,  jigVal14G, // 接待交際費
		jigVal15A  = 350, jigVal15B,                                                            // 損害保険料
		jigVal16A  = 352,             jigVal16C,        		         jigVal16F,  jigVal16G, // 修繕費
		jigVal17A  = 356,		      jigVal17C,  jigVal17D,  jigVal17E, jigVal17F,  jigVal17G,	// 消耗品費
		jigVal18A  = 362, jigVal18B,                                                            // 減価償却費
		jigVal19A  = 364, jigVal19B,  jigVal19C,  jigVal19D,  jigVal19E, jigVal19F,  jigVal19G, // 福利厚生費
		jigVal20A  = 371, jigVal20B,  jigVal20C,           		         jigVal20F,  jigVal20G, // 給料賃金
		jigVal21A  = 376, jigVal21B,  jigVal21C,                         jigVal21F,  jigVal21G, // 外注工賃
		jigVal22A  = 381, jigVal22B,                                                            // 利子割引料
		jigVal23A  = 383, jigVal23B,  jigVal23C,                         jigVal23F,  jigVal23G, // 地代家賃
		jigVal24A  = 388, jigVal24B,                                                            // 貸倒金
		jigVal25A  = 390, jigVal25B,  jigVal25C,  jigVal25D,  jigVal25E, jigVal25F,  jigVal25G, // 経費 自由入力欄1
		jigVal26A  = 397, jigVal26B,  jigVal26C,  jigVal26D,  jigVal26E, jigVal26F,  jigVal26G, // 経費 自由入力欄2
		jigVal27A  = 404, jigVal27B,  jigVal27C,  jigVal27D,  jigVal27E, jigVal27F,  jigVal27G, // 経費 自由入力欄3
		jigVal28A  = 411, jigVal28B,  jigVal28C,  jigVal28D,  jigVal28E, jigVal28F,  jigVal28G, // 経費 自由入力欄4
		jigVal29A  = 418, jigVal29B,  jigVal29C,  jigVal29D,  jigVal29E, jigVal29F,  jigVal29G, // 経費 自由入力欄5
		jigVal30A  = 425, jigVal30B,  jigVal30C,  jigVal30D,  jigVal30E, jigVal30F,  jigVal30G, // 経費 自由入力欄6
		jigVal31A  = 432, jigVal31B,  jigVal31C,  jigVal31D,  jigVal31E, jigVal31F,  jigVal31G, // 雑費
		jigVal32A  = 439, jigVal32B,  jigVal32C,  jigVal32D,  jigVal32E, jigVal32F,  jigVal32G, // 計
		jigVal33A  = 446,	                                                                    // 差引金額
		jigVal34A  = 447,             jigVal34C,  jigVal34D,  jigVal34E, jigVal34F,  jigVal34G, // 項目3 + 項目32

		jigName25  = 453, jigName26, jigName27,	jigName28, jigName29, jigName30,                //経費 自由入力欄1-6 項目名

	};

	//非表示項目。一応区別できるようにする
	enum disabledCell_JigyoEx2 {
		jigVal01E = -200,    //最後尾の値が正にならないような負の値 
		jigVal01G, jigVal012E, jigVal012G,
		jigVal02B, jigVal02C, jigVal02D, jigVal02E, jigVal02F, jigVal02G,
		jigVal04B, jigVal04C, jigVal04D, jigVal04E, jigVal04F, jigVal04G,
		jigVal05B, jigVal05C, jigVal05D, jigVal05E, jigVal05F, jigVal05G,
		jigVal06B, jigVal06C, jigVal06D, jigVal06E, jigVal06F, jigVal06G,
		jigVal07B, jigVal07C, jigVal07D, jigVal07E, jigVal07F, jigVal07G,
		jigVal08D, jigVal08E,
		jigVal09D, jigVal09E,
		jigVal10B, jigVal10D, jigVal10E,
		jigVal11D, jigVal11E,
		jigVal15C, jigVal15D, jigVal15E, jigVal15F, jigVal15G,
		jigVal16B, jigVal16D, jigVal16E,
		jigVal17B,
		jigVal18C, jigVal18D, jigVal18E, jigVal18F, jigVal18G,
		jigVal20D, jigVal20E,
		jigVal21D, jigVal21E,
		jigVal22C, jigVal22D, jigVal22E, jigVal22F, jigVal22G,
		jigVal23D, jigVal23E,
		jigVal24C, jigVal24D, jigVal24E, jigVal24F, jigVal24G,
		jigVal33B, jigVal33C, jigVal33D, jigVal33E, jigVal33F, jigVal33G,
		jigVal34B,
	};

	constexpr static std::array<std::array<int, clmSize>, rowSize> idxMap = {{
		{jigVal01A,  jigVal01B,   jigVal01C,   jigVal01D,   jigVal01E,  jigVal01F,   jigVal01G},
		{jigVal012A, jigVal012B,  jigVal012C,  jigVal012D,  jigVal012E, jigVal012F,  jigVal012G},
		{jigVal02A,  jigVal02B,   jigVal02C,   jigVal02D,   jigVal02E,  jigVal02F,   jigVal02G},
		{jigVal03A,  jigVal03B,   jigVal03C,   jigVal03D,   jigVal03E,  jigVal03F,   jigVal03G},
		{jigVal04A,  jigVal04B,   jigVal04C,   jigVal04D,   jigVal04E,  jigVal04F,   jigVal04G},
		{jigVal05A,  jigVal05B,   jigVal05C,   jigVal05D,   jigVal05E,  jigVal05F,   jigVal05G},
		{jigVal06A,  jigVal06B,   jigVal06C,   jigVal06D,   jigVal06E,  jigVal06F,   jigVal06G},
		{jigVal07A,  jigVal07B,   jigVal07C,   jigVal07D,   jigVal07E,  jigVal07F,   jigVal07G},
		{jigVal08A,  jigVal08B,   jigVal08C,   jigVal08D,   jigVal08E,  jigVal08F,   jigVal08G},
		{jigVal09A,  jigVal09B,   jigVal09C,   jigVal09D,   jigVal09E,  jigVal09F,   jigVal09G},
		{jigVal10A,  jigVal10B,   jigVal10C,   jigVal10D,   jigVal10E,  jigVal10F,   jigVal10G},
		{jigVal11A,  jigVal11B,   jigVal11C,   jigVal11D,   jigVal11E,  jigVal11F,   jigVal11G},
		{jigVal12A,  jigVal12B,   jigVal12C,   jigVal12D,   jigVal12E,  jigVal12F,   jigVal12G},
		{jigVal13A,  jigVal13B,   jigVal13C,   jigVal13D,   jigVal13E,  jigVal13F,   jigVal13G},
		{jigVal14A,  jigVal14B,   jigVal14C,   jigVal14D,   jigVal14E,  jigVal14F,   jigVal14G},
		{jigVal15A,  jigVal15B,   jigVal15C,   jigVal15D,   jigVal15E,  jigVal15F,   jigVal15G},
		{jigVal16A,  jigVal16B,   jigVal16C,   jigVal16D,   jigVal16E,  jigVal16F,   jigVal16G},
		{jigVal17A,  jigVal17B,   jigVal17C,   jigVal17D,   jigVal17E,  jigVal17F,   jigVal17G},
		{jigVal18A,  jigVal18B,   jigVal18C,   jigVal18D,   jigVal18E,  jigVal18F,   jigVal18G},
		{jigVal19A,  jigVal19B,   jigVal19C,   jigVal19D,   jigVal19E,  jigVal19F,   jigVal19G},
		{jigVal20A,  jigVal20B,   jigVal20C,   jigVal20D,   jigVal20E,  jigVal20F,   jigVal20G},
		{jigVal21A,  jigVal21B,   jigVal21C,   jigVal21D,   jigVal21E,  jigVal21F,   jigVal21G},
		{jigVal22A,  jigVal22B,   jigVal22C,   jigVal22D,   jigVal22E,  jigVal22F,   jigVal22G},
		{jigVal23A,  jigVal23B,   jigVal23C,   jigVal23D,   jigVal23E,  jigVal23F,   jigVal23G},
		{jigVal24A,  jigVal24B,   jigVal24C,   jigVal24D,   jigVal24E,  jigVal24F,   jigVal24G},
		{jigVal25A,  jigVal25B,   jigVal25C,   jigVal25D,   jigVal25E,  jigVal25F,   jigVal25G},
		{jigVal26A,  jigVal26B,   jigVal26C,   jigVal26D,   jigVal26E,  jigVal26F,   jigVal26G},
		{jigVal27A,  jigVal27B,   jigVal27C,   jigVal27D,   jigVal27E,  jigVal27F,   jigVal27G},
		{jigVal28A,  jigVal28B,   jigVal28C,   jigVal28D,   jigVal28E,  jigVal28F,   jigVal28G},
		{jigVal29A,  jigVal29B,   jigVal29C,   jigVal29D,   jigVal29E,  jigVal29F,   jigVal29G},
		{jigVal30A,  jigVal30B,   jigVal30C,   jigVal30D,   jigVal30E,  jigVal30F,   jigVal30G},
		{jigVal31A,  jigVal31B,   jigVal31C,   jigVal31D,   jigVal31E,  jigVal31F,   jigVal31G},
		{jigVal32A,  jigVal32B,   jigVal32C,   jigVal32D,   jigVal32E,  jigVal32F,   jigVal32G},
		{jigVal33A,  jigVal33B,   jigVal33C,   jigVal33D,   jigVal33E,  jigVal33F,   jigVal33G},
		{jigVal34A,  jigVal34B,   jigVal34C,   jigVal34D,   jigVal34E,  jigVal34F,   jigVal34G},
	}};

	//ダイアグラム上での行数
	enum diagLine {
		jigDiagLine1,  jigDiagLine1TA, jigDiagLine2,  jigDiagLine3,  jigDiagLine4,
		jigDiagLine5,  jigDiagLine6,   jigDiagLine7,  jigDiagLine8,  jigDiagLine9,
		jigDiagLine10, jigDiagLine11,  jigDiagLine12, jigDiagLine13, jigDiagLine14,
		jigDiagLine15, jigDiagLine16,  jigDiagLine17, jigDiagLine18, jigDiagLine19,
		jigDiagLine20, jigDiagLine21,  jigDiagLine22, jigDiagLine23, jigDiagLine24,
		jigDiagLine25, jigDiagLine26,  jigDiagLine27, jigDiagLine28, jigDiagLine29,
		jigDiagLine30, jigDiagLine31,  jigDiagLine32, jigDiagLine33, jigDiagLine34,
	};

	//データ保存位置及び出力時の行数、ShinJigyo31ExIdx.hのKJIGLINEと同等
	enum KJVAL_dataLine{
		jigDataLine1 = 0,    //売上
		jigDataLine1TA = 34, //返還等対価
		jigDataLine2 = 1,  jigDataLine3,  jigDataLine4,  jigDataLine5,  jigDataLine6,
		jigDataLine7,      jigDataLine8,  jigDataLine9,  jigDataLine10, jigDataLine11,
		jigDataLine12,     jigDataLine13, jigDataLine14, jigDataLine15, jigDataLine16,
		jigDataLine17,     jigDataLine18, jigDataLine19, jigDataLine20, jigDataLine21,
		jigDataLine22,     jigDataLine23, jigDataLine24, jigDataLine25, jigDataLine26,
		jigDataLine27,     jigDataLine28, jigDataLine29, jigDataLine30, jigDataLine31,
		jigDataLine32,     jigDataLine33, jigDataLine34,
	};
	
	//diagLineからデータ書き込み位置を指定するための配列
	constexpr static long dataPos[] = { 
		jigDataLine1,  jigDataLine1TA, jigDataLine2,  jigDataLine3,  jigDataLine4,  
		jigDataLine5,  jigDataLine6,   jigDataLine7,  jigDataLine8,  jigDataLine9,
		jigDataLine10, jigDataLine11,  jigDataLine12, jigDataLine13, jigDataLine14,
		jigDataLine15, jigDataLine16,  jigDataLine17, jigDataLine18, jigDataLine19,
		jigDataLine20, jigDataLine21,  jigDataLine22, jigDataLine23, jigDataLine24,
		jigDataLine25, jigDataLine26,  jigDataLine27, jigDataLine28, jigDataLine29,
		jigDataLine30, jigDataLine31,  jigDataLine32, jigDataLine33, jigDataLine34,
	};


	const static int FCSitemMax = 134;
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
	void DispJigyoDiagData(long dlin);//仮対応(E,G以外)
	
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

	void DiagReSetPosition(void);	// 修正No.168690 add

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CIcsdiagctrl m_JigyoDiag;
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
		{ jigVal01A, 0, 1, 0 },
		{ jigVal01B, 0, 1, 0 },
		{ jigVal01C, 0, 0, 0 },
		{ jigVal01D, 0, 1, 0 },
		{ jigVal01F, 0, 1, 0 },
		{ jigVal012A, 0, 1, 0 },	
		{ jigVal012B, 0, 1, 0 },
		{ jigVal012C, 0, 0, 0 },
		{ jigVal012D, 0, 1, 0 },
		{ jigVal012F, 0, 1, 0 },
		{ jigVal02A, 0, 1, 0 },
		{ jigVal03A, 0, 1, 0 },
		{ jigVal03B, 0, 1, 0 },
		{ jigVal03C, 0, 0, 0 },
		{ jigVal03D, 0, 1, 0 },
		{ jigVal03E, 0, 1, 0 },
		{ jigVal03F, 0, 1, 0 },
		{ jigVal03G, 0, 1, 0 },
		{ jigVal04A, 0, 0, 0 },
		{ jigVal05A, 0, 1, 0 },
		{ jigVal06A, 0, 0, 0 },
		{ jigVal07A, 0, 0, 0 },
		{ jigVal08A, 0, 1, 0 },
		{ jigVal08B, 0, 1, 0 },
		{ jigVal08C, 0, 0, 0 },
		{ jigVal08F, 0, 1, 0 },
		{ jigVal08G, 0, 1, 0 },
		{ jigVal09A, 0, 1, 0 },
		{ jigVal09B, 0, 1, 0 },
		{ jigVal09C, 0, 0, 0 },
		{ jigVal09F, 0, 1, 0 },
		{ jigVal09G, 0, 1, 0 },
		{ jigVal10A, 0, 1, 0 },
		{ jigVal10C, 0, 0, 0 },
		{ jigVal10F, 0, 1, 0 },
		{ jigVal10G, 0, 1, 0 },
		{ jigVal11A, 0, 1, 0 },
		{ jigVal11B, 0, 1, 0 },
		{ jigVal11C, 0, 0, 0 },
		{ jigVal11F, 0, 1, 0 },
		{ jigVal11G, 0, 1, 0 },
		{ jigVal12A, 0, 1, 0 },
		{ jigVal12B, 0, 1, 0 },
		{ jigVal12C, 0, 0, 0 },
		{ jigVal12D, 0, 1, 0 },
		{ jigVal12E, 0, 1, 0 },
		{ jigVal12F, 0, 1, 0 },
		{ jigVal12G, 0, 1, 0 },
		{ jigVal13A, 0, 1, 0 },
		{ jigVal13B, 0, 1, 0 },
		{ jigVal13C, 0, 0, 0 },
		{ jigVal13D, 0, 1, 0 },
		{ jigVal13E, 0, 1, 0 },
		{ jigVal13F, 0, 1, 0 },
		{ jigVal13G, 0, 1, 0 },
		{ jigVal14A, 0, 1, 0 },
		{ jigVal14B, 0, 1, 0 },
		{ jigVal14C, 0, 0, 0 },
		{ jigVal14D, 0, 1, 0 },
		{ jigVal14E, 0, 1, 0 },
		{ jigVal14F, 0, 1, 0 },
		{ jigVal14G, 0, 1, 0 },
		{ jigVal15A, 0, 1, 0 },
		{ jigVal16A, 0, 1, 0 },
		{ jigVal16C, 0, 0, 0 },
		{ jigVal16F, 0, 1, 0 },
		{ jigVal16G, 0, 1, 0 },
		{ jigVal17A, 0, 1, 0 },
		{ jigVal17C, 0, 0, 0 },
		{ jigVal17D, 0, 1, 0 },
		{ jigVal17E, 0, 1, 0 },
		{ jigVal17F, 0, 1, 0 },
		{ jigVal17G, 0, 1, 0 },
		{ jigVal18A, 0, 1, 0 },
		{ jigVal19A, 0, 1, 0 },
		{ jigVal19B, 0, 1, 0 },
		{ jigVal19C, 0, 0, 0 },
		{ jigVal19D, 0, 1, 0 },
		{ jigVal19E, 0, 1, 0 },
		{ jigVal19F, 0, 1, 0 },
		{ jigVal19G, 0, 1, 0 },
		{ jigVal20A, 0, 1, 0 },
		{ jigVal20B, 0, 1, 0 },
		{ jigVal20C, 0, 0, 0 },
		{ jigVal20F, 0, 1, 0 },
		{ jigVal20G, 0, 1, 0 },
		{ jigVal21A, 0, 1, 0 },
		{ jigVal21B, 0, 1, 0 },
		{ jigVal21C, 0, 0, 0 },
		{ jigVal21F, 0, 1, 0 },
		{ jigVal21G, 0, 1, 0 },
		{ jigVal22A, 0, 1, 0 },
		{ jigVal23A, 0, 1, 0 },
		{ jigVal23B, 0, 1, 0 },
		{ jigVal23C, 0, 0, 0 },
		{ jigVal23F, 0, 1, 0 },
		{ jigVal23G, 0, 1, 0 },
		{ jigVal24A, 0, 1, 0 },
		{ jigVal25A, 0, 1, 0 },
		{ jigVal25B, 0, 1, 0 },
		{ jigVal25C, 0, 0, 0 },
		{ jigVal25D, 0, 1, 0 },
		{ jigVal25E, 0, 1, 0 },
		{ jigVal25F, 0, 1, 0 },
		{ jigVal25G, 0, 1, 0 },
		{ jigVal26A, 0, 1, 0 },
		{ jigVal26B, 0, 1, 0 },
		{ jigVal26C, 0, 0, 0 },
		{ jigVal26D, 0, 1, 0 },
		{ jigVal26E, 0, 1, 0 },
		{ jigVal26F, 0, 1, 0 },
		{ jigVal26G, 0, 1, 0 },
		{ jigVal27A, 0, 1, 0 },
		{ jigVal27B, 0, 1, 0 },
		{ jigVal27C, 0, 0, 0 },
		{ jigVal27D, 0, 1, 0 },
		{ jigVal27E, 0, 1, 0 },
		{ jigVal27F, 0, 1, 0 },
		{ jigVal27G, 0, 1, 0 },
		{ jigVal28A, 0, 1, 0 },
		{ jigVal28B, 0, 1, 0 },
		{ jigVal28C, 0, 0, 0 },
		{ jigVal28D, 0, 1, 0 },
		{ jigVal28E, 0, 1, 0 },
		{ jigVal28F, 0, 1, 0 },
		{ jigVal28G, 0, 1, 0 },
		{ jigVal29A, 0, 1, 0 },
		{ jigVal29B, 0, 1, 0 },
		{ jigVal29C, 0, 0, 0 },
		{ jigVal29D, 0, 1, 0 },
		{ jigVal29E, 0, 1, 0 },
		{ jigVal29F, 0, 1, 0 },
		{ jigVal29G, 0, 1, 0 },
		{ jigVal30A, 0, 1, 0 },
		{ jigVal30B, 0, 1, 0 },
		{ jigVal30C, 0, 0, 0 },
		{ jigVal30D, 0, 1, 0 },
		{ jigVal30E, 0, 1, 0 },
		{ jigVal30F, 0, 1, 0 },
		{ jigVal30G, 0, 1, 0 },
		{ jigVal31A, 0, 1, 0 },
		{ jigVal31B, 0, 1, 0 },
		{ jigVal31C, 0, 0, 0 },
		{ jigVal31D, 0, 1, 0 },
		{ jigVal31E, 0, 1, 0 },
		{ jigVal31F, 0, 1, 0 },
		{ jigVal31G, 0, 1, 0 },
		{ jigName25, 1, 1, 0 },
		{ jigName26, 1, 1, 0 },
		{ jigName27, 1, 1, 0 },
		{ jigName28, 1, 1, 0 },
		{ jigName29, 1, 1, 0 },
		{ jigName30, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	};
};

