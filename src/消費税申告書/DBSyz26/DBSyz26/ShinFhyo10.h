#pragma once
#include "icsdiagctrl.h"


// CShinFhyo10 ダイアログ

class CShinFhyo10 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo10)

public:
	CShinFhyo10(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo10();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY10 };

	// 初期情報のセット
	int InitInfo( CH26SnFhyo10Data **pSnFhyo10Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo , CH26SnHonpyoData **pSnHonpyoData );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 初期ポジションセット
	void SetItemPosition();

	// データの読込み
	int ReadData( int pas );
	// データの書込み
	int WriteData();

	// 画面出力
	void DispFh1();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 各入力データ抽出
	void GetAllDiagData();

	//== 計算 ==
	// 課税標準　計算
	void f1calq_3( int sw );
	// 消費税額(4%税率分)
	void f1calq_4( int sw );
	// 消費税額(3%税率分)
	void f1calq_5( int sw );
	// 消費税額(6.3%税率分)
	void f1calq_5_2( int sw );
	// 消費税額計
	void f1calq_6( int sw );
	// 控除過大調整税額	･ 控除対象仕入税額
	void f1calq_78( int sw );
	// 返還等対価に係る税額
	void f1calq_9( int sw );
	// 貸倒れに係る税額
	void f1calq_10( int sw );
	// 控除税額小計
	void f1calq_11( int sw );
	// 控除不足税額（11－6－7）･ 差引税額（6+7-11)
	void f1calq_1213( int sw );
	// 合計差引税額（13－12)
	void f1calq_14( int sw );
	// 譲渡割額(14,15)
	void f1calq_15( int sw );
	// 各入力の計算表示
	void f1calq_all( int atno, int sw );
	// 他表からの計算
	void calq_by_OtherChange( int sw );

	//--------->フォーカス移動処理（上下）
	int UpDownFocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	//<----------------

	// 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );

private:
	CWnd				*m_pParent;	// 親ウィンドウ

	//== 帳票データ ==
	char				F11A[MONY_BUF_SIZE],	//  1.A 課税標準額(税率３％分)
						F11B[MONY_BUF_SIZE],	//  1.B     〃    (税率４％分)
						F11C[MONY_BUF_SIZE],	//  1.C     〃    (税率６.３％分)
						F11T[MONY_BUF_SIZE];	//  1.D     〃    (合計)

	char				F12A[MONY_BUF_SIZE],	//  2.A 消費税額(税率３％分)
						F12B[MONY_BUF_SIZE],	//  2.B     〃  (税率４％分)
						F12C[MONY_BUF_SIZE],	//  2.C     〃  (税率６.３％分)
						F12T[MONY_BUF_SIZE];	//  2.D     〃  (合計)

	char				F13A[MONY_BUF_SIZE],	//  3.A 貸倒回収に係る消費税額(税率３％分)
						F13B[MONY_BUF_SIZE],	//  3.B     〃                (税率４％分)
						F13C[MONY_BUF_SIZE],	//  3.C     〃                (税率６.３％分)
						F13T[MONY_BUF_SIZE],	//  3.D     〃                (合計)
						F13S[MONY_BUF_SIZE];	//  3.S

	char				F14A[MONY_BUF_SIZE],	//  4.A 控除対象仕入税額(税率３％分)
						F14B[MONY_BUF_SIZE],	//  4.B     〃          (税率４％分)
						F14C[MONY_BUF_SIZE],	//  4.C     〃          (税率６.３％分)
						F14T[MONY_BUF_SIZE];	//  4.D     〃          (合計)

	char				F15A[MONY_BUF_SIZE],	//  5.A 返還等対価に係る税額(税率３％分)
						F15B[MONY_BUF_SIZE],	//  5.B     〃              (税率４％分)
						F15C[MONY_BUF_SIZE],	//  5.C     〃              (税率６.３％分)
						F15T[MONY_BUF_SIZE];	//  5.D     〃              (合計)

	char				F16A[MONY_BUF_SIZE],	//  6.A 貸倒に係る税額(税率３％分)
						F16B[MONY_BUF_SIZE],	//  6.B     〃        (税率４％分)
						F16C[MONY_BUF_SIZE],	//  6.C     〃        (税率６.３％分)
						F16T[MONY_BUF_SIZE];	//  6.D     〃        (合計)

	char				F17A[MONY_BUF_SIZE],	//  7.A 控除税額小計(税率３％分)
						F17B[MONY_BUF_SIZE],	//  7.B     〃      (税率４％分)
						F17C[MONY_BUF_SIZE],	//  7.C     〃      (税率６.３％分)
						F17T[MONY_BUF_SIZE];	//  7.D     〃      (合計)

	char				F18A[MONY_BUF_SIZE],	//  8.A 控除不足還付税額(税率３％分)
						F18B[MONY_BUF_SIZE],	//  8.B     〃          (税率４％分)
						F18C[MONY_BUF_SIZE],	//  8.C     〃          (税率６.３％分)
						F18T[MONY_BUF_SIZE];	//  8.D     〃          (合計)

	char				F19A[MONY_BUF_SIZE],	//  9.A 差引税額(税率３％分)
						F19B[MONY_BUF_SIZE],	//  9.B     〃  (税率４％分)
						F19C[MONY_BUF_SIZE],	//  9.C     〃  (税率６.３％分)
						F19T[MONY_BUF_SIZE];	//  9.D     〃  (合計)

	char				F110T[MONY_BUF_SIZE];	// 10.D 合計差引税額

	char				F111B[MONY_BUF_SIZE],	// 11.B 控除不足還付税額(税率４％分)
						F111C[MONY_BUF_SIZE],	// 11.C     〃          (税率６.３％分)
						F111T[MONY_BUF_SIZE];	// 11.D     〃          (合計)

	char				F112B[MONY_BUF_SIZE],	// 12.B     〃  (税率４％分)
						F112C[MONY_BUF_SIZE],	// 12.C     〃  (税率６.３％分)
						F112T[MONY_BUF_SIZE];	// 12.D     〃  (合計)

	char				F113T[MONY_BUF_SIZE];	// 13.D 合計差引税額

	char				F114B[MONY_BUF_SIZE],	// 14.B 還付税額(税率４％分)
						F114C[MONY_BUF_SIZE],	// 14.C     〃  (税率６.３％分)
						F114T[MONY_BUF_SIZE];	// 14.D     〃  (合計)

	char				F115B[MONY_BUF_SIZE],	// 15.B 納税額(税率４％分)
						F115C[MONY_BUF_SIZE],	// 15.C     〃(税率６.３％分)
						F115T[MONY_BUF_SIZE];	// 15.D     〃(合計)

	char				F116T[MONY_BUF_SIZE];	// 16.D 合計差引譲渡割額

	//== 帳票データ本体 ==
	CH26SnFhyo10Data	**m_pSnFhyo10Data;
	CH26SnHonpyoData	**m_pSnHonpyoData;		//[Y] 本表
	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報

	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報
	CSyzUtil			m_Util;

	int					m_FrSw;					// ???
	int					F1c;					// 集計スイッチ

	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Fhyo10Diag01;
	CIcsdiagctrl m_Fhyo10Diag02;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
