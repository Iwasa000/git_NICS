#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CShinFhyo22EX ダイアログ

class CShinFhyo22EX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo22EX)

public:
	CShinFhyo22EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo22EX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY22_EX };

	// 初期情報のセット
	int InitInfo( CH28SnFhyo20Data **pSnFhyo22Data, CSnHeadData **pSnHeadData, CH26SnTanaData **pSnTanaData, CH28SnFhyo10Data **pSnFhyo10Data, CH28SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// ボタン表示変更
	void ChangeBtnDisp();

	// データの読込み
	int ReadData( int pas );
	// データの書込み
	int WriteData( int pas );

	// 画面出力
	void DispFh22();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );
	// 入力許可・不許可 切替
	void ModifyDiagInput( short index, int sign );
	// 各入力データ抽出
	void GetAllDiagData();
	// 特定収入関連の手入力サイン連動('15.05.20)
	int SetCalqSw( int sw );
	// 特定課税仕入に関連するインデックス？
	int ChkTkkzIdx( short idx );
	// 特定課税仕入割合の取得('15.09.10)
//	unsigned short GetKzwari(){ return F22UWS; };
	// 課税売上割合の取得('15.09.10)
	unsigned short GetCalqedKzwari();
	// 特定課税仕入に関する項目の金額クリア('15.11.13)
	void ClearTkkzMony();

	//== 計算 ==
	// 課税売上額
	void f22calq_1();
	// 課税資産の譲渡等の対価の額
	void f22calq_45();
	// 非課税売上額
	void f22calq_6();
	// 資産の譲渡等の対価の額 ･ 課税売上割合
	void f22calq_7();
	// 課税仕入に係る支払対価の額
	void f22calq_8();
	// 課税仕入に係る消費税額
	void f22calq_9();
	//特定課税仕入れに係る支払対価の額
	void f22calq_10();
	//特定課税仕入れに係る消費税額
	void f22calq_11();
	// 課税貨物に係る消費税額
//	void f22calq_11();
	void f22calq_13();
	// 課税事業⇔免税事業 調整税額
//	void f22calq_12();
	void f22calq_14();
	// 課税仕入等の税額の合計額
//	void f22calq_13();
	void f22calq_15();
	// 課税売上割合が９５％以上の場合
//	void f22calq_14();
	void f22calq_16();
	// 課税売上にのみ要する仕入税額
//	void f22calq_15();
	void f22calq_17();
	// 共通の売上に要する仕入税額
//	void f22calq_16();
	void f22calq_18();
	// 個別対応方式控除仕入税額
//	void f22calq_17();
	void f22calq_19();
	// 比例配分方式控除仕入税額
//	void f22calq_18();
	void f22calq_20();
	// 課税売上割合が９５％未満の場合
	void f22calq_ko();
	// 控除税額の調整（１８）
//	void f22calq_19();
	void f22calq_21();
	// 控除税額の調整（１９）
//	void f22calq_20();
	void f22calq_22();
	//	差引控除仕入税額の計算
//	void f22calq_2122();
	void f22calq_2324();
	// 回収に係る消費税額
//	void f22calq_23();
	void f22calq_25();
	// 各入力の計算表示
	void f22calq_all( int atno );
	// 他表からの計算 ('14.05.20)
	void calq_by_OtherChange();
	// 特定収入を加味した控除対象仕入税額の計算
	void f22calq_spc();

	//--------->フォーカス移動処理（上下）
	int UpDownFocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	//<----------------

	// 初期ポジションセット
	void SetItemPosition();


private:

	CWnd				*m_pParent;	// 親ウィンドウ

	char				F221A[MONY_BUF_SIZE],		//  1. 課税売上額
						F221B[MONY_BUF_SIZE],		//  1. 課税売上額
						F221C[MONY_BUF_SIZE];		//  1. 課税売上額

	char				F222A[MONY_BUF_SIZE],		//  2. 免税売上額
						F222B[MONY_BUF_SIZE],		//  2. 免税売上額
						F222C[MONY_BUF_SIZE];		//  2. 免税売上額

	char				F223A[MONY_BUF_SIZE],		//  3. 非課税資産の輸出金額
						F223B[MONY_BUF_SIZE],		//  3. 非課税資産の輸出金額
						F223C[MONY_BUF_SIZE],		//  3. 非課税資産の輸出金額
						F224C[MONY_BUF_SIZE];		//  4. 課税資産の譲渡等の対価の額

	char				F226A[MONY_BUF_SIZE],		//	6. 非課税売上額
						F226B[MONY_BUF_SIZE],		//	6. 非課税売上額
						F226C[MONY_BUF_SIZE],		//	6. 非課税売上額
						F227C[MONY_BUF_SIZE];		//  7. 資産の譲渡等の対価の額

	char				F228A[MONY_BUF_SIZE],		//	8. 課税仕入に係る支払対価の額
						F228B[MONY_BUF_SIZE],		//	8. 課税仕入に係る支払対価の額
						F228C[MONY_BUF_SIZE];		//	8. 課税仕入に係る支払対価の額

	char				F229A[MONY_BUF_SIZE],		//	9. 課税仕入に係る消費税額
						F229B[MONY_BUF_SIZE],		//	9. 課税仕入に係る消費税額
						F229C[MONY_BUF_SIZE];		//	9. 課税仕入に係る消費税額

	char				F2210A[MONY_BUF_SIZE],		// 10. 特定課税仕入れに係る支払対価の額
						F2210B[MONY_BUF_SIZE],		// 10. 特定課税仕入れに係る支払対価の額
						F2210C[MONY_BUF_SIZE];		// 10. 特定課税仕入れに係る支払対価の額

	char				F2211A[MONY_BUF_SIZE],		// 11. 特定課税仕入れに係る消費税額
						F2211B[MONY_BUF_SIZE],		// 11. 特定課税仕入れに係る消費税額
						F2211C[MONY_BUF_SIZE];		// 11. 特定課税仕入れに係る消費税額

	char				F2212A[MONY_BUF_SIZE],		//   . 課税貨物に係る支払対価の額
						F2212B[MONY_BUF_SIZE],		//   . 課税貨物に係る支払対価の額
						F2212C[MONY_BUF_SIZE];		//   . 課税貨物に係る支払対価の額

	char				F2213A[MONY_BUF_SIZE],		// 12. 課税貨物に係る消費税額
						F2213B[MONY_BUF_SIZE],		// 12. 課税貨物に係る消費税額
						F2213C[MONY_BUF_SIZE];		// 12. 課税貨物に係る消費税額
													   
	char				F2214A[MONY_BUF_SIZE],		// 14. 調整税額
						F2214B[MONY_BUF_SIZE],		// 14. 調整税額
						F2214C[MONY_BUF_SIZE];		// 14. 調整税額
													   
	char				F2215A[MONY_BUF_SIZE],		// 15. 課税仕入等の税額の合計額
						F2215B[MONY_BUF_SIZE],		// 15. 課税仕入等の税額の合計額
						F2215C[MONY_BUF_SIZE];		// 15. 課税仕入等の税額の合計額
													   
	char				F2217A[MONY_BUF_SIZE],		// 14. 課税売上に係る仕入税額
						F2217B[MONY_BUF_SIZE],		// 14. 課税売上に係る仕入税額
						F2217C[MONY_BUF_SIZE];		// 14. 課税売上に係る仕入税額
													   
	char				F2218A[MONY_BUF_SIZE],		// 15. 共通の売上に係る仕入税額
						F2218B[MONY_BUF_SIZE],		// 15. 共通の売上に係る仕入税額
						F2218C[MONY_BUF_SIZE];		// 15. 共通の売上に係る仕入税額
													   
	char				F2217AEX[MONY_BUF_SIZE],	// 16. 課税売上に係る仕入税額
						F2217BEX[MONY_BUF_SIZE],	// 16. 課税売上に係る仕入税額
						F2217CEX[MONY_BUF_SIZE];	// 16. 課税売上に係る仕入税額
													   
	char				F2218AEX[MONY_BUF_SIZE],	// 17. 共通の売上に係る仕入税額
						F2218BEX[MONY_BUF_SIZE],	// 17. 共通の売上に係る仕入税額
						F2218CEX[MONY_BUF_SIZE];	// 17. 共通の売上に係る仕入税額
													   
	char				F2219A[MONY_BUF_SIZE],		// 18. 個別対応方式控除仕入税額
						F2219B[MONY_BUF_SIZE],		// 18. 個別対応方式控除仕入税額
						F2219C[MONY_BUF_SIZE];		// 18. 個別対応方式控除仕入税額
													   
	char				F2220A[MONY_BUF_SIZE],		// 19. 比例配分方式控除仕入税額
						F2220B[MONY_BUF_SIZE],		// 19. 比例配分方式控除仕入税額
						F2220C[MONY_BUF_SIZE];		// 19. 比例配分方式控除仕入税額
													   
	char				F2221A[MONY_BUF_SIZE],		// 20. 調整対象固定資産に係る調整税額
						F2221B[MONY_BUF_SIZE],		// 20. 調整対象固定資産に係る調整税額
						F2221C[MONY_BUF_SIZE];		// 20. 調整対象固定資産に係る調整税額
													   
	char				F2222A[MONY_BUF_SIZE],		// 21. 調整対象固定資産を課税業務用に転用した調整額
						F2222B[MONY_BUF_SIZE],		// 21. 調整対象固定資産を課税業務用に転用した調整額
						F2222C[MONY_BUF_SIZE];		// 21. 調整対象固定資産を課税業務用に転用した調整額
													   
	char				F2223A[MONY_BUF_SIZE],		// 22. 差引控除対象仕入税額
						F2223B[MONY_BUF_SIZE],		// 22. 差引控除対象仕入税額
						F2223C[MONY_BUF_SIZE];		// 22. 差引控除対象仕入税額

	char				F2224A[MONY_BUF_SIZE],		// 23. 差引控除過大調整税額
						F2224B[MONY_BUF_SIZE],		// 23. 差引控除過大調整税額
						F2224C[MONY_BUF_SIZE];		// 23. 差引控除過大調整税額

	char				F2225A[MONY_BUF_SIZE],		// 24. 貸倒回収に係る消費税額
						F2225B[MONY_BUF_SIZE],		// 24. 貸倒回収に係る消費税額
						F2225C[MONY_BUF_SIZE];		// 24. 貸倒回収に係る消費税額
	unsigned short		F22UWS;						//     課税売上割合
	int					F22UWSEX;					//     手入力された課税売上割合

	char				FS25A[MONY_BUF_SIZE],
						FS25B[MONY_BUF_SIZE],
						FS25C[MONY_BUF_SIZE];

	//== 帳票データ本体 ==
//-- '15.08.19 --
//	CH26SnFhyo20Data	**m_pSnFhyo22Data;
//	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報
//	CH26SnTanaData		**m_pSnTanaData;			// 棚卸調整データ
//	CH26SnFhyo10Data	**m_pSnFhyo10Data;			// 付表１データ
//	CH26SnHonpyoData	**m_pSnHonpyoData;			// 本表データ
//---------------
	CH28SnFhyo20Data	**m_pSnFhyo22Data;
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報
	CH26SnTanaData		**m_pSnTanaData;			// 棚卸調整データ
	CH28SnFhyo10Data	**m_pSnFhyo10Data;			// 付表１データ
	CH28SnHonpyoData	**m_pSnHonpyoData;			// 本表データ
//---------------

	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_FrSw;						// ???
	int					m_DpSw;						// ???
	int					m_DlSw;						// ???

	int					F229c, F2211c, F2213c, F2219c, F2220c, F2223c;

	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_Fhyo22Diag01;
	CIcsdiagctrl m_Fhyo22Diag02;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_Button1;
	CButton m_Button2;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiagctrl2(short index, LPUNKNOWN data);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
