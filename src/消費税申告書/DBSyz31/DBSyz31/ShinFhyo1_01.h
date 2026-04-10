#pragma once
#include "icsdiagctrl.h"


// CShinFhyo1_01 ダイアログ

class CShinFhyo1_01 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo1_01)

public:
	CShinFhyo1_01(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo1_01();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY1_01 };



	// 初期情報のセット
	int InitInfo( CH31SnFhyo1_01Data **pSnFhyo1_01Data , CSnHeadData **pSnHeadData , SYC_SHININFO *pShinInfo, CArithEx *pArith, CSyzSyukei **pSyzSyukei);

	// 初期化
	int Init();

	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 初期ポジションセット
	void SetItemPosition();

	// データの読込み
	int ReadData( int pas );
	void ReadData2();

	// データの書込み
	int WriteData();


	// 画面出力
	void DispFh1_01();

	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );

	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );

	// 各入力データ抽出
	void GetAllDiagData();


	// 特定課税仕入に関連するインデックス？
	int ChkTkkzIdx( short idx );

	// 特定課税仕入に関する項目の金額クリア
	void ClearTkkzMony();

	//====各データ処理（色変更、データセットなど）=====//
	//　課税標準額
	void f101_1data();
	//課税資産の譲渡等の対価の額
	void f101_1_1data();
	//特定課税仕入れに係る支払対価の額
	void f101_1_2data();
	// 消費税額
	void f101_2data();
	// 控除過大調整税額
	void f101_3data();
	// 控除対象仕入税額
	void f101_4data();
	// 返還等対価に係る税額
	void f101_5data();
	// 売上げの返還等対価に係る税額
	void f101_5_1data();
	// 特定課税仕入れの返還等対価に係る税額
	void f101_5_2data();
	// 貸倒れに係る税額
	void f101_6data();
	// 控除税額小計
	void f101_7data();
	// 控除不足還付税額
	void f101_8data();
	// 差引税額
	void f101_9data();
	// 合計差引税額
	void f101_10data();
	// 準となる消費税額　控除不足還付税額
	void f101_11data();
	// 準となる消費税額　差引税額
	void f101_12data();
	// 合計差引地方消費税の課税標準となる消費税額
	void f101_13data();
	// 譲渡割額　還付額
	void f101_14data();
	// 譲渡割額　納税額
	void f101_15data();
	// 合計差引譲渡割額
	void f101_16data();

	// 各入力のデータ表示
	void f101data_all( int atno );

	// 集計モジュールを更新
	int SyukeiDataSet(int cID);
	int MemoryReget();


	//フォーカス移動処理（上下）Move = 0 : 下方向	Move = 1: 上方向
	int UpDownFocusMoveCheck(int Index , int Move);

	// 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 特例帳票を使用しているか
	bool IsUseTokurei();


private:
	CWnd				*m_pParent;				// 親ウィンドウ


	//== 帳票データ ==
	char				F101_1X[MONY_BUF_SIZE],			//  1.X    課税標準額　旧税率分小計　X
						F101_1D[MONY_BUF_SIZE],			//  1.D    課税標準額　税率6.24％適用分　D
						F101_1E[MONY_BUF_SIZE],			//  1.E    課税標準額　税率7.8％適用分　E
						F101_1F[MONY_BUF_SIZE];			//  1.F    課税標準額　合計 F

	char				F101_1_1X[MONY_BUF_SIZE],		//	1-1.X  課税資産の譲渡等の対価の額　旧税率分小計　X
						F101_1_1D[MONY_BUF_SIZE],		//  1-1.D  課税資産の譲渡等の対価の額　税率6.24％適用分　D
						F101_1_1E[MONY_BUF_SIZE],		//  1-1.E  課税資産の譲渡等の対価の額　税率7.8％適用分　E
						F101_1_1F[MONY_BUF_SIZE];		//	1-1.F  課税資産の譲渡等の対価の額　合計 F

	char				F101_1_2X[MONY_BUF_SIZE],		//  1-2.X  特定課税仕入れに係る支払い対価の額　旧税率分小計　X
						F101_1_2E[MONY_BUF_SIZE],		//  1-2.E  特定課税仕入れに係る支払い対価の額　税率7.8％適用分　E
						F101_1_2F[MONY_BUF_SIZE];		//	1-2.F  特定課税仕入れに係る支払い対価の額　合計 F

	char				F101_2X[MONY_BUF_SIZE],			//  2.X    消費税額　旧税率分小計　X
						F101_2D[MONY_BUF_SIZE],			//  2.D    消費税額　税率6.24％適用分　D
						F101_2E[MONY_BUF_SIZE],			//  2.E    消費税額　税率7.8％適用分　E
						F101_2F[MONY_BUF_SIZE];			//	2.F    消費税額　合計 F

	char				F101_3X[MONY_BUF_SIZE],			//  3.X    控除過大調整税額　旧税率分小計　X
						F101_3D[MONY_BUF_SIZE],			//  3.D    控除過大調整税額　税率6.24％適用分　D
						F101_3E[MONY_BUF_SIZE],			//  3.E    控除過大調整税額　税率7.8％適用分　E
						F101_3F[MONY_BUF_SIZE];			//	3.F    控除過大調整税額　合計 F

	char				F101_4X[MONY_BUF_SIZE],			//  4.X    控除対象仕入れ税額　旧税率分小計　X
						F101_4D[MONY_BUF_SIZE],			//  4.D    控除対象仕入れ税額　税率6.24％適用分　D
						F101_4E[MONY_BUF_SIZE],			//  4.E    控除対象仕入れ税額　税率7.8％適用分　E
						F101_4F[MONY_BUF_SIZE];			//	4.F    控除対象仕入れ税額　合計 F

	char				F101_5X[MONY_BUF_SIZE],			//  5.X    返還等対価に係る税額　旧税率分小計　X
						F101_5D[MONY_BUF_SIZE],			//  5.D    返還等対価に係る税額　税率6.24％適用分　D
						F101_5E[MONY_BUF_SIZE],			//  5.E    返還等対価に係る税額　税率7.8％適用分　E
						F101_5F[MONY_BUF_SIZE];			//	5.F    返還等対価に係る税額　合計 F

	char				F101_5_1X[MONY_BUF_SIZE],		//  5-1.X  売上げの返還等対価に係る税額　旧税率分小計　X
						F101_5_1D[MONY_BUF_SIZE],		//  5-1.D  売上げの返還等対価に係る税額　税率6.24％適用分　D
						F101_5_1E[MONY_BUF_SIZE],		//  5-1.E  売上げの返還等対価に係る税額　税率7.8％適用分　E
						F101_5_1F[MONY_BUF_SIZE];		//	5-1.F  売上げの返還等対価に係る税額　合計 F

	char				F101_5_2X[MONY_BUF_SIZE],		//  5-2.X  特定課税仕入れの返還等対価に係る税額　旧税率分小計　X
						F101_5_2E[MONY_BUF_SIZE],		//  5-2.E  特定課税仕入れの返還等対価に係る税額　税率7.8％適用分　E
						F101_5_2F[MONY_BUF_SIZE];		//	5-2.F  特定課税仕入れの返還等対価に係る税額　合計 F

	char				F101_6X[MONY_BUF_SIZE],			//  6.X    貸倒れに係る税額　旧税率分小計　X
						F101_6D[MONY_BUF_SIZE],			//  6.D    貸倒れに係る税額　税率6.24％適用分　D
						F101_6E[MONY_BUF_SIZE],			//  6.E    貸倒れに係る税額　税率7.8％適用分　E
						F101_6F[MONY_BUF_SIZE];			//	6.F    貸倒れに係る税額　合計 F

	char				F101_7X[MONY_BUF_SIZE],			//  7.X    控除税額小計　旧税率分小計　X
						F101_7D[MONY_BUF_SIZE],			//  7.D    控除税額小計　税率6.24％適用分　D
						F101_7E[MONY_BUF_SIZE],			//  7.E    控除税額小計　税率7.8％適用分　E
						F101_7F[MONY_BUF_SIZE];			//	7.F    控除税額小計　合計 F

	char				F101_8X[MONY_BUF_SIZE],			//  8.X    控除不足還付税額　旧税率分小計　X
						F101_8D[MONY_BUF_SIZE],			//  8.D    控除不足還付税額　税率6.24％適用分　D
						F101_8E[MONY_BUF_SIZE],			//  8.E    控除不足還付税額　税率7.8％適用分　E
						F101_8F[MONY_BUF_SIZE];			//	8.F    控除不足還付税額　合計 F

	char				F101_9X[MONY_BUF_SIZE],			//  9.X    差引税額　旧税率分小計　X
						F101_9D[MONY_BUF_SIZE],			//  9.D    差引税額　税率6.24％適用分　D
						F101_9E[MONY_BUF_SIZE],			//  9.E    差引税額　税率7.8％適用分　E
						F101_9F[MONY_BUF_SIZE];			//	9.F    差引税額　合計 F

	char				F101_10F[MONY_BUF_SIZE];		//	10.F   合計差引税額　合計 F

	char				F101_11X[MONY_BUF_SIZE],		//  11.X   控除不足還付税額　旧税率分小計　X
						F101_11E[MONY_BUF_SIZE],		//  11.E   控除不足還付税額　税率7.8％適用分　E
						F101_11F[MONY_BUF_SIZE];		//	11.F   控除不足還付税額　合計 F

	char				F101_12X[MONY_BUF_SIZE],		//  12.X   差引税額　旧税率分小計　X
						F101_12E[MONY_BUF_SIZE],		//  12.E   差引税額　税率7.8％適用分　E
						F101_12F[MONY_BUF_SIZE];		//	12.F   差引税額　合計 F

	char				F101_13X[MONY_BUF_SIZE],		//  13.X   合計差引地方消費税の課税標準となる消費税額　旧税率分小計　X
						F101_13E[MONY_BUF_SIZE],		//  13.E   合計差引地方消費税の課税標準となる消費税額　税率7.8％適用分　E
						F101_13F[MONY_BUF_SIZE];		//	13.F   合計差引地方消費税の課税標準となる消費税額　合計 F

	char				F101_14X[MONY_BUF_SIZE],		//  14.X   還付額　旧税率分小計　X
						F101_14E[MONY_BUF_SIZE],		//  14.E   還付額　税率7.8％適用分　E
						F101_14F[MONY_BUF_SIZE];		//	14.F   還付額　合計 F

	char				F101_15X[MONY_BUF_SIZE],		//  15.X   納税額　旧税率分小計　X
						F101_15E[MONY_BUF_SIZE],		//  15.E   納税額　税率7.8％適用分　E
						F101_15F[MONY_BUF_SIZE];		//	15.F   納税額　合計 F

	char				F101_16F[MONY_BUF_SIZE];		//	16.F   合計差引譲渡割額　合計 F



	//== 帳票データ本体 ==//
	CH31SnFhyo1_01Data	**m_pSnFhyo1_01Data;
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報

	SYC_SHININFO		*m_pShinInfo;					// 共通情報(現時点では使用してないが、今後用に処理を入れておく)
	CArithEx			*m_pArith;
	CSyzSyukei			**m_pSyzSyukei;					// 消費税集計クラス


	int					m_FrSw;							// Readサイン？元処理引用

	int					F101_1c;						// 課税標準額　				手入力＆集計サイン　(D3 , D7 , D11・・・手入力サイン　)
	int					F101_2c;						// 消費税額　　				手入力＆集計サイン　(D3 , D7 , D11・・・手入力サイン　)
	int					F101_5c;						// 返還等対価に係る税額		手入力＆集計サイン　(D3 , D7 , D11・・・手入力サイン　)

	int					m_curwnd;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Fhyo1_01Diag01;
	CIcsdiagctrl m_Fhyo1_01Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl2(short index);
	void EditOFFIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
