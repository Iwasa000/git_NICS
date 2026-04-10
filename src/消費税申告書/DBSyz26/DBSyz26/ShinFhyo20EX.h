#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CShinFhyo20EX ダイアログ

class CShinFhyo20EX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo20EX)

public:
	CShinFhyo20EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo20EX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY20_EX };

public:
	// 初期情報のセット
//-- '15.08.18 --
//	int InitInfo( CH26SnFhyo20Data **pSnFhyo20Data, CSnHeadData **pSnHeadData, CH26SnTanaData **pSnTanaData, CH26SnFhyo10Data **pSnFhyo10Data, CH26SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
//---------------
	int InitInfo( CH28SnFhyo20Data **pSnFhyo20Data, CSnHeadData **pSnHeadData, CH26SnTanaData **pSnTanaData, CH28SnFhyo10Data **pSnFhyo10Data, CH28SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
//---------------
	// 初期化
	int Init();
	// データの読込み
	int ReadData( int pas );
	// データの書込み
	int WriteData( int pas );

	// 画面出力
	void DispFh2();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 各入力データ抽出
	void GetAllDiagData();
	// 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 業種による画面表示の変更
	void ChangeDispByApno();
	// ポジションセット
	void SetItemPosition( int Index, BOOL isScroll=FALSE );
	// 特定課税仕入に関連するインデックス？
	int ChkTkkzIdx( short idx );
	// 特定課税仕入割合の取得('15.09.10)
//	unsigned short GetKzwari(){ return F2UWS; };
	// 課税売上割合の取得('15.09.10)
	unsigned short GetCalqedKzwari();
	// 特定課税仕入に関する項目の金額クリア('15.11.13)
	void ClearTkkzMony();

	// 特定収入関連の手入力サイン連動('15.05.20)
	int SetCalqSw( int sw );

	//== 計算 ==
	// 課税資産の譲渡等の対価の額
	void f2calq_45();
	// 資産の譲渡等の対価の額 ･ 課税売上割合
	void f2calq_7();
	// 課税仕入に係る消費税額
	void f2calq_9();
	// 特定課税仕入れに係る消費税額
	void f2calq_11();
	// 課税貨物に係る消費税額
//	void f2calq_11();
	void f2calq_13();
	// 課税仕入等の税額の合計額
//	void f2calq_13();
	void f2calq_15();
	// 課税売上割合が９５％以上の場合
//	void f2calq_14();
	void f2calq_16();
	// 課税売上にのみ要する仕入税額
//	void f2calq_15();
	void f2calq_17();
	// 共通の売上に要する仕入税額
//	void f2calq_16();
	void f2calq_18();
	// 個別対応方式控除仕入税額
//	void f2calq_17();
	void f2calq_19();
	// 比例配分方式控除仕入税額
//	void f2calq_18();
	void f2calq_20();
	// 課税売上割合が９５％未満の場合
	void f2calq_ko();
	// 差引控除仕入税額の計算
//	void f2calq_2122();
	void f2calq_2324();
	// 各入力の計算表示
	void f2calq_all( int atno );
	// 特定収入を加味した控除対象仕入税額の計算('15.04.04)
	void f2calq_spc();

	// 初期ポジションセット
	void SetItemPosition();

private:
	CWnd				*m_pParent;					// 親ウィンドウ

	//== 帳票データ ==
	char				F21A[MONY_BUF_SIZE],		//  1. 課税売上額
						F21B[MONY_BUF_SIZE],		//  1. 課税売上額
						F21C[MONY_BUF_SIZE];		//  1. 課税売上額

	char				F22A[MONY_BUF_SIZE],		//  2. 免税売上額
						F22B[MONY_BUF_SIZE],		//  2. 免税売上額
						F22C[MONY_BUF_SIZE];		//  2. 免税売上額

	char				F23A[MONY_BUF_SIZE],		//  3. 非課税資産の輸出金額
						F23B[MONY_BUF_SIZE],		//  3. 非課税資産の輸出金額
						F23C[MONY_BUF_SIZE],		//  3. 非課税資産の輸出金額
						F24C[MONY_BUF_SIZE];		//  4. 課税資産の譲渡等の対価の額

	char				F26A[MONY_BUF_SIZE],		//	6. 非課税売上額
						F26B[MONY_BUF_SIZE],		//	6. 非課税売上額
						F26C[MONY_BUF_SIZE],		//	6. 非課税売上額
						F27C[MONY_BUF_SIZE];		//  7. 資産の譲渡等の対価の額
		
	char				F28A[MONY_BUF_SIZE],		//	8. 課税仕入に係る支払対価の額
						F28B[MONY_BUF_SIZE],		//	8. 課税仕入に係る支払対価の額
						F28C[MONY_BUF_SIZE];		//	8. 課税仕入に係る支払対価の額

	char				F29A[MONY_BUF_SIZE],		//	9. 課税仕入に係る消費税額
						F29B[MONY_BUF_SIZE],		//	9. 課税仕入に係る消費税額
						F29C[MONY_BUF_SIZE];		//	9. 課税仕入に係る消費税額
	
	char				F210A[MONY_BUF_SIZE],		// 10.特定課税仕入れに係る支払対価の額
						F210B[MONY_BUF_SIZE],		// 10.特定課税仕入れに係る支払対価の額
						F210C[MONY_BUF_SIZE];		// 10.特定課税仕入れに係る支払対価の額

	char				F211A[MONY_BUF_SIZE],		// 11.特定課税仕入れに係る消費税額
						F211B[MONY_BUF_SIZE],		// 11.特定課税仕入れに係る消費税額
						F211C[MONY_BUF_SIZE];		// 11.特定課税仕入れに係る消費税額
	
	char				F212A[MONY_BUF_SIZE],		//   . 課税貨物に係る支払対価の額
						F212B[MONY_BUF_SIZE],		//   . 課税貨物に係る支払対価の額
						F212C[MONY_BUF_SIZE];		//   . 課税貨物に係る支払対価の額
													
	char				F213A[MONY_BUF_SIZE],		// 12. 課税貨物に係る消費税額
						F213B[MONY_BUF_SIZE],		// 12. 課税貨物に係る消費税額
						F213C[MONY_BUF_SIZE];		// 12. 課税貨物に係る消費税額
													   
	char				F214A[MONY_BUF_SIZE],		// 13. 調整税額
						F214B[MONY_BUF_SIZE],		// 13. 調整税額
						F214C[MONY_BUF_SIZE];		// 13. 調整税額
													   
	char				F215A[MONY_BUF_SIZE],		// 14. 課税仕入等の税額の合計額
						F215B[MONY_BUF_SIZE],		// 14. 課税仕入等の税額の合計額
						F215C[MONY_BUF_SIZE];		// 14. 課税仕入等の税額の合計額
													   
	char				F217A[MONY_BUF_SIZE],		// 16. 課税売上に係る仕入税額
						F217B[MONY_BUF_SIZE],		// 16. 課税売上に係る仕入税額
						F217C[MONY_BUF_SIZE];		// 16. 課税売上に係る仕入税額
													   
	char				F218A[MONY_BUF_SIZE],		// 17. 共通の売上に係る仕入税額
						F218B[MONY_BUF_SIZE],		// 17. 共通の売上に係る仕入税額
						F218C[MONY_BUF_SIZE];		// 17. 共通の売上に係る仕入税額
													   
	char				F219A[MONY_BUF_SIZE],		// 18. 個別対応方式控除仕入税額
						F219B[MONY_BUF_SIZE],		// 18. 個別対応方式控除仕入税額
						F219C[MONY_BUF_SIZE];		// 18. 個別対応方式控除仕入税額
													   
	char				F217AEX[MONY_BUF_SIZE],		// 16. 課税売上に係る仕入税額
						F217BEX[MONY_BUF_SIZE],		// 16. 課税売上に係る仕入税額
						F217CEX[MONY_BUF_SIZE];		// 16. 課税売上に係る仕入税額
													   
	char				F218AEX[MONY_BUF_SIZE],		// 17. 共通の売上に係る仕入税額
						F218BEX[MONY_BUF_SIZE],		// 17. 共通の売上に係る仕入税額
						F218CEX[MONY_BUF_SIZE];		// 17. 共通の売上に係る仕入税額

	char				F220A[MONY_BUF_SIZE],		// 19. 比例配分方式控除仕入税額
						F220B[MONY_BUF_SIZE],		// 19. 比例配分方式控除仕入税額
						F220C[MONY_BUF_SIZE];		// 19. 比例配分方式控除仕入税額
													   
	char				F221A[MONY_BUF_SIZE],		// 20. 調整対象固定資産に係る調整税額
						F221B[MONY_BUF_SIZE],		// 20. 調整対象固定資産に係る調整税額
						F221C[MONY_BUF_SIZE];		// 20. 調整対象固定資産に係る調整税額
													   
	char				F222A[MONY_BUF_SIZE],		// 21. 調整対象固定資産を課税業務用に転用した調整額
						F222B[MONY_BUF_SIZE],		// 21. 調整対象固定資産を課税業務用に転用した調整額
						F222C[MONY_BUF_SIZE];		// 21. 調整対象固定資産を課税業務用に転用した調整額
													   
	char				F223A[MONY_BUF_SIZE],		// 22. 差引控除対象仕入税額
						F223B[MONY_BUF_SIZE],		// 22. 差引控除対象仕入税額
						F223C[MONY_BUF_SIZE];		// 22. 差引控除対象仕入税額
													   
	char				F224A[MONY_BUF_SIZE],		// 23. 差引控除過大調整税額
						F224B[MONY_BUF_SIZE],		// 23. 差引控除過大調整税額
						F224C[MONY_BUF_SIZE];		// 23. 差引控除過大調整税額
													
	char				F225A[MONY_BUF_SIZE],		// 24. 貸倒回収に係る消費税額
						F225B[MONY_BUF_SIZE],		// 24. 貸倒回収に係る消費税額
						F225C[MONY_BUF_SIZE];		// 24. 貸倒回収に係る消費税額
													
	char				FS25A[MONY_BUF_SIZE],		
						FS25B[MONY_BUF_SIZE],		
						FS25C[MONY_BUF_SIZE];		

	int					F2UWS;						// 課税売上割合
	int					F2UWSEX;					// 課税売上割合（手入力値） 
	int					m_UWexp;					// 

	int					F219c,						// 計算スイッチ
						F220c,
						F223c;

	int					F29c;						// 計算スイッチ
	int					F211c;
	int					F213c;

	//== 帳票データ本体 ==
//-- '15.08.18 --
//	CH26SnFhyo20Data	**m_pSnFhyo20Data;			// 付表２データ
//---------------
	CH28SnFhyo20Data	**m_pSnFhyo20Data;			// 付表２データ
//---------------
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報
	CH26SnTanaData		**m_pSnTanaData;			// 棚卸調整データ
//-- '15.08.18 --
//	CH26SnFhyo10Data	**m_pSnFhyo10Data;			// 付表１データ
//	CH26SnHonpyoData	**m_pSnHonpyoData;			// 本表データ
//---------------
	CH28SnFhyo10Data	**m_pSnFhyo10Data;			// 付表１データ
	CH28SnHonpyoData	**m_pSnHonpyoData;			// 本表データ
//---------------

	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_FrSw;						// ???
	int					m_DlSw;						// ???

	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CIcsdiagctrl m_Fhyo20Diag01;
	CIcsdiagctrl m_Fhyo20Diag02;
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
