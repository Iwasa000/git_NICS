#pragma once
#include "SyzBaseDlg.h"
#include "stdafx.h"


// CShinFudousan31Ex2 ダイアログ

class CShinFudousan31Ex2 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFudousan31Ex2)

public:
	CShinFudousan31Ex2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFudousan31Ex2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FUDOUSAN31_ex2};


private:
	CWnd				*m_pParent;	// 親ウィンドウ
	CSyzUtil			m_Util;
	short				m_Idx;
	short				m_Lin;
	short				m_Clm;

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;	

	//== 印刷情報 ==
	CVPrint				*m_pVprn;							// 出力クラス
	NPRT_PAR			*m_pPset;							// 印刷情報
	PBUF				PF[30];
	SH_PRINT_CMINFO		*m_PrintCmInfo;

	const static int clmSize = 7;	//帳票の列数(科目列抜き)
	enum clmId {
		clmA = 0, clmB, clmC, clmD, clmE, clmF, clmG,
	};
	const static int rowSize = 16;	//帳票の行数
	const int idxEditMin = 130;	// 入力ボックス(金額)の最小のインデックス
	const int idxEditMax = 199;	// 入力ボックス(金額)の最大のインデックス
	const int idxYear = 1;	
	const int idxZkbn = 217; // 経理方式

	const int autoCalcChk2 = 263; // 自動計算チェック	うち軽減税率6.24%適用分  
	const int autoCalcChk3 = 264; // 　　　 〃 　　　	うち標準税率7.8%適用分   


	//表示項目
	enum idx_FudousanEx2 {
		FudVal01A = 130, FudVal01B, FudVal01C, FudVal01F,					
		FudVal02A = 134, FudVal02B, FudVal02C, FudVal02F,					
		FudVal03A = 138, FudVal03B, FudVal03C, FudVal03D, FudVal03F,					
		FudVal04A = 143, FudVal04B, FudVal04C, FudVal04D, FudVal04F,					
		FudVal04TA = 148, FudVal04TB, FudVal04TC, FudVal04TD, FudVal04TF,					
		FudVal05A = 153, FudVal05B, FudVal05C, FudVal05F, FudVal05G,		
		FudVal06A = 158, FudVal06B,																	
		FudVal07A = 160, FudVal07C, FudVal07F, FudVal07G,		
		FudVal08A = 164, FudVal08B,																	
		FudVal09A = 166, FudVal09B,																	
		FudVal10A = 168, FudVal10B, FudVal10C, FudVal10F, FudVal10G,		
		FudVal11A = 173, FudVal11B, FudVal11C, FudVal11F, FudVal11G,		
		FudVal12A = 178, FudVal12B, FudVal12C, FudVal12D, FudVal12E, FudVal12F, FudVal12G,		
		FudVal13A = 185, FudVal13B, FudVal13C, FudVal13D, FudVal13E, FudVal13F, FudVal13G,		
		FudVal14A = 192, FudVal14B, FudVal14C, FudVal14D, FudVal14E, FudVal14F, FudVal14G,		
		FudVal15A = 199,																				

		FudName03 = 200, FudName12,

	};
	//非表示項目
	enum disabledCell_FudousanEx2 {
		FudVal01D = -200,
		FudVal01E, FudVal01G,
		FudVal02D, FudVal02E, FudVal02G,
		FudVal03E, FudVal03G,
		FudVal04E, FudVal04G,
		FudVal04TE, FudVal04TG,
		FudVal05D, FudVal05E,
		FudVal06C, FudVal06D, FudVal06E, FudVal06F, FudVal06G,
		FudVal07B, FudVal07D, FudVal07E,
		FudVal08C, FudVal08D, FudVal08E, FudVal08F, FudVal08G,
		FudVal09C, FudVal09D, FudVal09E, FudVal09F, FudVal09G,
		FudVal10D, FudVal10E,
		FudVal11D, FudVal11E, FudVal15B, FudVal15C, FudVal15D, FudVal15E, FudVal15F, FudVal15G,
	};

	constexpr static std::array<std::array<int, clmSize>, rowSize> idxMap = { {
		{FudVal01A,	FudVal01B,	FudVal01C,	FudVal01D,	FudVal01E,	FudVal01F,	FudVal01G},
		{FudVal02A,	FudVal02B,	FudVal02C,	FudVal02D,	FudVal02E,	FudVal02F,	FudVal02G},
		{FudVal03A,	FudVal03B,	FudVal03C,	FudVal03D,	FudVal03E,	FudVal03F,	FudVal03G},
		{FudVal04A,	FudVal04B,	FudVal04C,	FudVal04D,	FudVal04E,	FudVal04F,	FudVal04G},
		{FudVal04TA,FudVal04TB,	FudVal04TC,	FudVal04TD,	FudVal04TE,	FudVal04TF,	FudVal04TG},
		{FudVal05A,	FudVal05B,	FudVal05C,	FudVal05D,	FudVal05E,	FudVal05F,	FudVal05G},
		{FudVal06A,	FudVal06B,	FudVal06C,	FudVal06D,	FudVal06E,	FudVal06F,	FudVal06G},
		{FudVal07A,	FudVal07B,	FudVal07C,	FudVal07D,	FudVal07E,	FudVal07F,	FudVal07G},
		{FudVal08A,	FudVal08B,	FudVal08C,	FudVal08D,	FudVal08E,	FudVal08F,	FudVal08G},
		{FudVal09A,	FudVal09B,	FudVal09C,	FudVal09D,	FudVal09E,	FudVal09F,	FudVal09G},
		{FudVal10A,	FudVal10B,	FudVal10C,	FudVal10D,	FudVal10E,	FudVal10F,	FudVal10G},
		{FudVal11A,	FudVal11B,	FudVal11C,	FudVal11D,	FudVal11E,	FudVal11F,	FudVal11G},
		{FudVal12A,	FudVal12B,	FudVal12C,	FudVal12D,	FudVal12E,	FudVal12F,	FudVal12G},
		{FudVal13A,	FudVal13B,	FudVal13C,	FudVal13D,	FudVal13E,	FudVal13F,	FudVal13G},
		{FudVal14A,	FudVal14B,	FudVal14C,	FudVal14D,	FudVal14E,	FudVal14F,	FudVal14G},
		{FudVal15A,	FudVal15B,	FudVal15C,	FudVal15D,	FudVal15E,	FudVal15F,	FudVal15G},

	} };


//ダイアグラム上での行数
	enum diagLine {
		FudDiagLine1, FudDiagLine2, FudDiagLine3, FudDiagLine4, FudDiagLine4TA,
		FudDiagLine5, FudDiagLine6, FudDiagLine7, FudDiagLine8, FudDiagLine9,
		FudDiagLine10, FudDiagLine11, FudDiagLine12, FudDiagLine13, FudDiagLine14,
		FudDiagLine15,
	};

	//データ保存位置及び出力時の行数、ShinJigyo31ExIdx.hのKJIGLINEと同等
	enum KFVAL_dataLine {
		FudDataLine1 = 0,    //売上
		FudDataLine4TA = 15, //返還等対価
		FudDataLine2 = 1, FudDataLine3, FudDataLine4, FudDataLine5, FudDataLine6,
		FudDataLine7, FudDataLine8, FudDataLine9, FudDataLine10, FudDataLine11,
		FudDataLine12, FudDataLine13, FudDataLine14, FudDataLine15,
	};

	//diagLineからデータ書き込み位置を指定するための配列
	constexpr static long dataPos[] = {
		FudDataLine1,	FudDataLine2,	FudDataLine3,	FudDataLine4,	FudDataLine4TA,
		FudDataLine5,	FudDataLine6,	FudDataLine7,	FudDataLine8,	FudDataLine9,
		FudDataLine10,	FudDataLine11,	FudDataLine12,	FudDataLine13,	FudDataLine14,
		FudDataLine15,
	};

	const static int FCSitemMax = 53;
	static FCSUPDOWN_INFO FCS_UD[FCSitemMax + 1];
	static FCSUPDOWN_INFO FCS_LR[FCSitemMax + 1];
	void setFCSArray(); //フォーカス移動用配列の初期化
	bool useSmallFontSize();//印刷時に使用するフォントサイズの判定


public:

	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	void InitDiagAttr();
	// 画面表示
	virtual void DispList(int mode);
	// ダイアグデータ表示
	void DispFudousan31DiagData(long dlin);
	// 表示判定
	int DispJudge(int clm, int line);
	// 各入力データ抽出
	int GetDiagData(long index, short leng, DIAGRAM_DATA *data, int mode);
	// 手入力サインのクリア
	int ClearManualInputSign();
	// 次フォーカスの場所検索（上下動作）'14.08.18
	int FocusMoveCheck(int Index, int Move);
	// 初期ポジションセット'14.09.01
	virtual void SetItemPosition();
	void SetItemPositionEx();
	// ポジションの末端設定
	virtual void SetFocusRange();

	// 印刷情報のセット
	void SetPrintInfo(CVPrint *pVprn, NPRT_PAR *pPset, SH_PRINT_CMINFO *pPrintCmInfo);
	// 印刷１
	int PrintHyo1(int pno, int fno, CDC *pDC, char *pBp);
	// 印刷２
	int PrintHyo2(int pno, int fno, CDC *pDC, char *pBp);
	// 印刷３
	int PrintHyo3(int pno, int fno, CDC *pDC, char *pBp);

	void DiagReSetPosition(void);		// 修正No.168690 add

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CIcsdiagctrl m_Fudousan31;
	virtual BOOL OnInitDialog();

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	//エディットボックスへの入力を許可するモード指定
		constexpr static SH_ITEMINDEX idxAttr[] = {
				{  FudVal01A, 0, 1, 0			},
				{  FudVal01B, 0, 1, 0			},
				{  FudVal01C, 0, 0, 0			},
				{  FudVal01F, 0, 1, 0			},

				{  FudVal02A, 0, 1, 0			},
				{  FudVal02B, 0, 1, 0			},
				{  FudVal02C, 0, 0, 0			},
				{  FudVal02F, 0, 1, 0			},

				{  FudVal03A, 0, 1, 0			},
				{  FudVal03B, 0, 1, 0			},
				{  FudVal03C, 0, 0, 0			},
				{  FudVal03D, 0, 1, 0			},
				{  FudVal03F, 0, 1, 0			},

// 230727Aw del -->
				//{  FudVal04A, 0, 1, 0			},
				//{  FudVal04B, 0, 1, 0			},
				//{  FudVal04C, 0, 0, 0			},
				//{  FudVal04D, 0, 1, 0			},
				//{  FudVal04F, 0, 1, 0			},
// 230727Aw del <--
// 230801Aw del -->
//// 230727Aw add -->
//				{  FudVal04A, 0, 0, 0			},
//				{  FudVal04B, 0, 0, 0			},
//				{  FudVal04C, 0, 0, 0			},
//				{  FudVal04D, 0, 0, 0			},
//				{  FudVal04F, 0, 0, 0			},
//// 230727Aw add <--
// 230801Aw del <--

				{  FudVal04TA, 0, 1, 0			},
				{  FudVal04TB, 0, 1, 0			},
				{  FudVal04TC, 0, 0, 0			},
				{  FudVal04TD, 0, 1, 0			},
				{  FudVal04TF, 0, 1, 0			},

				{  FudVal05A, 0, 1, 0			},
				{  FudVal05B, 0, 1, 0			},
				{  FudVal05C, 0, 0, 0			},
				{  FudVal05F, 0, 1, 0			},
				{  FudVal05G, 0, 1, 0			},

				{  FudVal06A, 0, 1, 0			},
				//{  FudVal06B, 0, 1, 0			},	// 230801Aw del

				{  FudVal07A, 0, 1, 0			},
				{  FudVal07C, 0, 0, 0			},
				{  FudVal07F, 0, 1, 0			},
				{  FudVal07G, 0, 1, 0			},

				{  FudVal08A, 0, 1, 0			},
				//{  FudVal08B, 0, 1, 0			},	// 230801Aw del

				{  FudVal09A, 0, 1, 0			},
				//{  FudVal09B, 0, 1, 0			},	// 230801Aw del

				{  FudVal10A, 0, 1, 0			},
				{  FudVal10B, 0, 1, 0			},
				{  FudVal10C, 0, 0, 0			},
				{  FudVal10F, 0, 1, 0			},
				{  FudVal10G, 0, 1, 0			},

				{  FudVal11A, 0, 1, 0			},
				{  FudVal11B, 0, 1, 0			},
				{  FudVal11C, 0, 0, 0			},
				{  FudVal11F, 0, 1, 0			},
				{  FudVal11G, 0, 1, 0			},

				{  FudVal12A, 0, 1, 0			},
				{  FudVal12B, 0, 1, 0			},
				{  FudVal12C, 0, 0, 0			},
				{  FudVal12D, 0, 1, 0			},
				{  FudVal12E, 0, 1, 0			},
				{  FudVal12F, 0, 1, 0			},
				{  FudVal12G, 0, 1, 0			},

				{  FudVal13A, 0, 1, 0			},
				{  FudVal13B, 0, 1, 0			},
				{  FudVal13C, 0, 0, 0			},
				{  FudVal13D, 0, 1, 0			},
				{  FudVal13E, 0, 1, 0			},
				{  FudVal13F, 0, 1, 0			},
				{  FudVal13G, 0, 1, 0			},

				{  FudName03, 1, 1, 0			},
				{  FudName12, 1, 1, 0			},
				{  0, 0, 0, 0			}
		};

};
