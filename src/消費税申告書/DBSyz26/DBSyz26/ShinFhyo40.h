#pragma once
#include "icsdiagctrl.h"


// CShinFhyo40 ダイアログ

class CShinFhyo40 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo40)

public:
	CShinFhyo40(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo40();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY40 };

	// 初期情報のセット
	int InitInfo( CH26SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo , CH26SnHonpyoData **pSnHonpyoData );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// データの読込み
	int ReadData( int pas );
	// データの書込み
	int WriteData();

	// 画面出力
	void DispFh4();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 各入力データ抽出
	void GetAllDiagData();

	//== 計算 ==
	// 課税標準　計算
	void f4calq_3( int sw );
	// 消費税額(個々の税率)
	void f4calq_4( int sw );
/*- '14.01.22_CUT -*/
//	// 消費税額(３％税率分)
//	void f4calq_5( int sw );
/*-----------------*/
	// 消費税額計
	void f4calq_6( int sw );
	// 控除過大調整税額	･ 控除対象仕入税額
	void f4calq_78( int sw );
	// 返還等対価に係る税額
	void f4calq_9( int sw );
	// 貸倒に係る税額
	void f4calq_10( int sw );
	// 控除税額小計
	void f4calq_11( int sw );
	// 控除不足税額（11－6－7）･ 差引税額（6+7-11)
	void f4calq_1213( int sw );
	// 合計差引税額（13－12)
	void f4calq_14( int sw );
	// 譲渡割額(14,15)
	void f4calq_15( int sw );
	// 各入力の計算表示
	void f4calq_all( int atno, int sw );
	// 他表からの計算
	void calq_by_OtherChange();
	// 他表の再集計による再表示
	void disp_by_recalq();
	void calqx_otherlists();

	//--------->フォーカス移動処理（上下）
	int UpDownFocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	//<----------------

	// 初期ポジションセット
	void SetItemPosition();

	// 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );

private:
	CWnd				*m_pParent;	// 親ウィンドウ

	//== 帳票データ ==
	char				F41A[MONY_BUF_SIZE],	//  1.A 課税標準額(税率３％分)
						F41B[MONY_BUF_SIZE],	//  1.B     〃    (税率４％分)
						F41C[MONY_BUF_SIZE],	//  1.C     〃    (税率６.３％分)
						F41T[MONY_BUF_SIZE];	//  1.D     〃    (合計)

	char				F42A[MONY_BUF_SIZE],	//  2.A 消費税額(税率３％分)
						F42B[MONY_BUF_SIZE],	//  2.B     〃  (税率４％分)
						F42C[MONY_BUF_SIZE],	//  2.C     〃  (税率６.３％分)
						F42T[MONY_BUF_SIZE];	//  2.D     〃  (合計)

	char				F43A[MONY_BUF_SIZE],	//  3.A 貸倒回収に係る消費税額(税率３％分)
						F43B[MONY_BUF_SIZE],	//  3.B     〃                (税率４％分)
						F43C[MONY_BUF_SIZE],	//  3.C     〃                (税率６.３％分)
						F43T[MONY_BUF_SIZE],	//  3.D     〃                (合計)
						F43S[MONY_BUF_SIZE];	//  3.S

	char				F44A[MONY_BUF_SIZE],	//  4.A 控除対象仕入税額(税率３％分)
						F44B[MONY_BUF_SIZE],	//  4.B     〃          (税率４％分)
						F44C[MONY_BUF_SIZE],	//  4.C     〃          (税率６.３％分)
						F44T[MONY_BUF_SIZE];	//  4.D     〃          (合計)

	char				F45A[MONY_BUF_SIZE],	//  5.A 返還等対価に係る税額(税率３％分)
						F45B[MONY_BUF_SIZE],	//  5.B     〃              (税率４％分)
						F45C[MONY_BUF_SIZE],	//  5.C     〃              (税率６.３％分)
						F45T[MONY_BUF_SIZE];	//  5.D     〃              (合計)

	char				F46A[MONY_BUF_SIZE],	//  6.A 貸倒に係る税額(税率３％分)
						F46B[MONY_BUF_SIZE],	//  6.B     〃        (税率４％分)
						F46C[MONY_BUF_SIZE],	//  6.C     〃        (税率６.３％分)
						F46T[MONY_BUF_SIZE];	//  6.D     〃        (合計)

	char				F47A[MONY_BUF_SIZE],	//  7.A 控除税額小計(税率３％分)
						F47B[MONY_BUF_SIZE],	//  7.B     〃      (税率４％分)
						F47C[MONY_BUF_SIZE],	//  7.C     〃      (税率６.３％分)
						F47T[MONY_BUF_SIZE];	//  7.D     〃      (合計)

	char				F48A[MONY_BUF_SIZE],	//  8.A 控除不足還付税額(税率３％分)
						F48B[MONY_BUF_SIZE],	//  8.B     〃          (税率４％分)
						F48C[MONY_BUF_SIZE],	//  8.C     〃          (税率６.３％分)
						F48T[MONY_BUF_SIZE];	//  8.D     〃          (合計)

	char				F49A[MONY_BUF_SIZE],	//  9.A 差引税額(税率３％分)
						F49B[MONY_BUF_SIZE],	//  9.B     〃  (税率４％分)
						F49C[MONY_BUF_SIZE],	//  9.C     〃  (税率６.３％分)
						F49T[MONY_BUF_SIZE];	//  9.D     〃  (合計)

	char				F410T[MONY_BUF_SIZE];	// 10.D 合計差引税額

	char				F411B[MONY_BUF_SIZE],	// 11.B 控除不足還付税額(税率４％分)
						F411C[MONY_BUF_SIZE],	// 11.C     〃          (税率６.３％分)
						F411T[MONY_BUF_SIZE];	// 11.D     〃          (合計)

	char				F412B[MONY_BUF_SIZE],	// 12.B     〃  (税率４％分)
						F412C[MONY_BUF_SIZE],	// 12.C     〃  (税率６.３％分)
						F412T[MONY_BUF_SIZE];	// 12.D     〃  (合計)

	char				F413T[MONY_BUF_SIZE];	// 13.D 合計差引税額

	char				F414B[MONY_BUF_SIZE],	// 14.B 還付税額(税率４％分)
						F414C[MONY_BUF_SIZE],	// 14.C     〃  (税率６.３％分)
						F414T[MONY_BUF_SIZE];	// 14.D     〃  (合計)

	char				F415B[MONY_BUF_SIZE],	// 15.B 納税額(税率４％分)
						F415C[MONY_BUF_SIZE],	// 15.C     〃(税率６.３％分)
						F415T[MONY_BUF_SIZE];	// 15.D     〃(合計)

	char				F416T[MONY_BUF_SIZE];	// 16.D 合計差引譲渡割額

	int					F4c;					// □□□□ □□□□ □□□□ □□□□ □□□□ □□□■ □□□■ □□□■
												// 変更サイン
												//  D0-D3:3％
												//  D4-D7:4％
												//  D8-D11:6.3％

	//== 帳票データ本体 ==
	CH26SnFhyo10Data	**m_pSnFhyo40Data;
	CH26SnHonpyoData	**m_pSnHonpyoData;		//[Y] 本表
	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報

	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報
	CSyzUtil			m_Util;

	int					m_FrSw;					// 

	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_Fhyo40Diag01;
	CIcsdiagctrl m_Fhyo40Diag02;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
