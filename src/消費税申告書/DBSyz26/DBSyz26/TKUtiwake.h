#pragma once
#include "icsdiagctrl.h"


// CTKUtiwake ダイアログ

class CTKUtiwake : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CTKUtiwake)

public:
	CTKUtiwake(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTKUtiwake();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TKUTIWAKE };

	// 初期情報のセット
	int InitInfo( CH28SnHonpyoData **pSnHonpyoData, CH28SnFhyo10Data **pSnFhyo10Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// 終了処理
	void EndJob();
	// 特定課税仕入に関する項目の金額クリア('15.12.02)
	void ClearTkkzMony();

	// データの読込み
	int ReadData();
	// データの書込み
	int WriteData();

	// 表示
	void DispTKUtiwake();
	// 表示（金額欄）
	void DispTKUtiwakeMony();

	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// ダイアグラム設定変更
	int ChgDiagAttr();
	// 金額の最終データ読み込み
	void GetDiagData();
	// ポジションセット
	void SetItemPosition();

	//== 計算 ==
	// 各入力の計算表示
	void calqx_all( int atno, int sw );
	// 2,3.内訳計算
	void calq_utiwake();
	// 1-3.課税標準額
	void calqx_1to3();
	// 4-6.返還等対価に係る税額
	void calqx_4to6();
	// 金額表示
	void dispmony();
	// 金額表示(1-6)
	void dispmony_1to6();
	// 金額表示(内訳)
	void dispmony_utiwake();


private:
	CWnd				*m_pParent;				// 親ウィンドウ

	//== 帳票データ ==
	char				XM1[MONY_BUF_SIZE],		// 1.課税標準額
						XM2[MONY_BUF_SIZE],		// 2.課税資産の譲渡等の対価の額
						XM3[MONY_BUF_SIZE],		// 3.特定課税仕入に係る支払対価の額
						XM4[MONY_BUF_SIZE],		// 4.返還等対価に係る税額
						XM5[MONY_BUF_SIZE],		// 5.売上げの返還等対価に係る税額
						XM6[MONY_BUF_SIZE];		// 6.特定課税仕入の返還等対価に係る税額

												// 2及び3の内訳
	char				UTI11[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(3%)
						UTI12[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(4%)
						UTI13[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(6.3%)

						UTI23[MONY_BUF_SIZE],	// 特定課税仕入に係る支払対価の額(6.3%)

						UTI31[MONY_BUF_SIZE],	// 合計(3%)
						UTI32[MONY_BUF_SIZE],	// 合計(4%)
						UTI33[MONY_BUF_SIZE];	// 合計(6.3%)

	//== 帳票データ本体 ==
	CH28SnHonpyoData	**m_pSnHonpyoData;		// 本表データ
	CH28SnFhyo10Data	**m_pSnFhyo10Data;		// 付表１データ
	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報

	CArithEx			*m_pArith;
	CSyzUtil			m_Util;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報

	int					m_DpSw;					// 画面の描画を伴うかどうか
	int					m_curwnd;				// 現ウィンドウ


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_TkUtiwakeDiag01;
	CIcsdiagctrl m_TkUtiwakeDiag02;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl2(short index);
};
