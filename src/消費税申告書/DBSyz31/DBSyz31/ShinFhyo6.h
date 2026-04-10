#pragma once
#include "icsdiagctrl.h"
#include "icsmnedit8.h"
#include "afxwin.h"


#define MYNUM1_1 ID5041_1
#define MYNUM1_2 ID5041_2
#define MYNUM1_3 ID5041_3
#define MYNUM2_1 ID5042_1
#define MYNUM2_2 ID5042_2
#define MYNUM2_3 ID5042_3
#define MYNUM3_1 ID5043_1
#define MYNUM3_2 ID5043_2
#define MYNUM3_3 ID5043_3
#define MYNUM4_1 ID5044_1
#define MYNUM4_2 ID5044_2
#define MYNUM4_3 ID5044_3

#define SOZOKU_MAX 40
#define SOZOKU_PER_PAGE 4

//付表１・４データ
typedef struct _Fuhyo_1_4_data {
	unsigned char	Sn_1F[MONY_BUF_SIZE],
		Sn_2F[MONY_BUF_SIZE],
		Sn_3F[MONY_BUF_SIZE],
		Sn_4F[MONY_BUF_SIZE],
		Sn_5F[MONY_BUF_SIZE],
		Sn_6F[MONY_BUF_SIZE],
		Sn_7F[MONY_BUF_SIZE],

		//Sn_8A[MONY_BUF_SIZE],
		//Sn_8B[MONY_BUF_SIZE],
		//Sn_8C[MONY_BUF_SIZE],
		//Sn_8D[MONY_BUF_SIZE],
		//Sn_8E[MONY_BUF_SIZE],
		Sn_8F[MONY_BUF_SIZE],

		//Sn_9A[MONY_BUF_SIZE],
		//Sn_9B[MONY_BUF_SIZE],
		//Sn_9C[MONY_BUF_SIZE],
		//Sn_9D[MONY_BUF_SIZE],
		//Sn_9E[MONY_BUF_SIZE],
		Sn_9F[MONY_BUF_SIZE];

}Fuhyo_1_4_data;

// CShinFhyo6 ダイアログ（令和5年10月1日以降は付表６→付表７に変更）

class CShinFhyo6 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo6)

public:
	CShinFhyo6(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo6();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY6 };

	// 初期情報のセット
	int InitInfo( CH31SnFhyo6Data **pSnFhyo6Data, CH31SnFhyo1_01Data **pSnFhyo1_01Data, CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CH31SnHonpyoData **pSnHonpyoData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, int SnSeq );
//	int InitInfo( CH31SnFhyo6Data **pSnFhyo6Data, CH31SnFhyo1_01Data **pSnFhyo1_01Data, CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CH31SnHonpyoData **pSnHonpyoData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo );
//	int InitInfo( CH31SnFhyo6Data **pSnFhyo6Data, CH31SnFhyo1_01Data **pSnFhyo1_01Data, CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// データの読込み
	int ReadData();
	// データの書込み
	int WriteData();
	// 終了処理
	void EndJob();

	// マイナンバー更新
	int MyNumber_Update();
	// 履歴データ書き込み
	int MyNumber_DeleteHistory();
	// マイナンバー削除・廃棄
	int MyNumber_Haiki();
	// マイナンバー追加
	int MyNumber_Write();
	// マイナンバー削除廃棄用配列クリア
	void MyNumber_DelHaiki_Clear();
	// マイナンバー変換(ハッシュ→マイナンバー)
	void MyNumber_Change();

	// 画面出力
	void DispFh6(int mode = 0);

	// 入力許可・不許可 切替
	void ModifyDiagInput();

	// 付表６計算
	void f6calq_1to6();
	void f6calq_9to14( int index );

	//付表１・４データ取得
	int Get_Fuhyo_1_4_data(Fuhyo_1_4_data *data);

	// フォーカス移動処理（上下）
	int FocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	// 入力項目色変え
	void ChangeColor( unsigned short id, short Index, int sign );

	// 初期ポジションセット
	void SetItemPosition();

	//
	void dsp_disp( short Index, CString disp );
	void dsp_edit( short Index, CString edit );
	void dsp_combo( short Index, short combo );
	void dsp_check( short Index, short check );
	void dsp_val( short Index, char *val );
	void dsp_day( short Index, char *day );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 各入力データ抽出
	void GetAllDiagData();
	// マイナンバーにフォーカスがあった場合はフォーカスをずらす
	void MoveFocus_FromMyNum();
	// 相続人欄のスクロールバーを画面最下部に移動
	void MoveWindow_SozokuBar();
	// 相続人の列が空白列か判定
	BOOL IsNULL_Column( int line_idx, char sgn = 0x00 );

	// マイナンバー初期設定
	void InitMyNumberData();
	// マイナンバー重複チェック
	int MyNumberCheck( int sgn = 0 );
	// 相続分のチェック
	int SozokubunCheck( int sgn = 0 );
	// 最大公約数取得
	int get_gcd( int num1, int num2 );
	// 納付還付税額の合計チェック
	int NofuKanpuGokeiCheck();
	// index(diag)から行番号取得 sgn 0=メモリ上の行番号取得　1=画面上の行番号取得
	int GetLineNumber_FromIndex( int index, int sgn = 0 );
	// 右端か判定
	BOOL IsRightEnd(int index);
	// 左端か判定
	BOOL IsLeftEnd(int index);
	// コンボボックスか判定
	BOOL IsComboBox(int index);
	// マイナンバー入力か判定
	BOOL IsMyNumberEdit(int index);
	// 行番号からindex(メモリ上の配列)取得
	int GetIndex_FromLine( int line_num );

	// 相続人の配列初期化
	void Init_sozoku( int index );

	// マイナンバー削除ボタン 表示非表示
	void MyNumber_ButtomDisp();
	// マイナンバー　ハッシュ空白判定
	BOOL MyNumber_IsHashEmpty( char *hash, int size );
//2018.08.21 INSERT START
	// マイナンバー　空白判定(ハッシュ値のみも空白とする)
	BOOL MyNumber_IsAllEmpty();
//2018.08.21 INSERT END
	// マイナンバーコントロールのリセット
	void ResetMyNumberControl();
	// 表示中(入力中)のマイナンバーをチェック
	int MyNumber_CheckFromDisp( int sgn = 0 );
	// DIAGRAM_DATAクリア
	void DIAGRAM_DATA_clear( DIAGRAM_DATA *DiagData );

	// 全選択が必要なコントロール？（相続分の分子・分母）
	bool IsNeedAllSel( int nID );

	void DelEdit();
	void Edit_Set();
	void Insert_kaigyo( int length, int line_max, CString *str );

	// 手入力項目あり？
	BOOL IsManualInputSign();

//----> 納付書・相続人対応
	// 相続人情報の取得
	int GetSozokuInfo( NOHFUSHO_INFO *nohfuinfo, NOHFUSHO_DATA *nohfudata, CArray<int> *SozokuNoAry );

	// 納付書情報の更新
	int UpdateShinPayment( int update_type, int index, int flg = 0 );
//<---- 納付書・相続人対応

	CSnHeadData			**m_pSnHeadData;		// 消費税ヘッダ情報
	CH31SnHonpyoData	**m_pSnHonpyoData;		// 本表データ
	CH31SnFhyo1_01Data	**m_pSnFhyo1_01Data;	// 付表１データ
	CH31SnFhyo4_01Data	**m_pSnFhyo4_01Data;	// 付表４データ
	CH31SnFhyo6Data		**m_pSnFhyo6Data;		// 付表６データ

	CDBNpSub			*m_pZmSub;				// 財務クラス
	CArithEx			*m_pArith;
	CSyzUtil			m_Util;
	SYC_SHININFO		*m_pShinInfo;			// 共通情報
	CDBSyzAccess		*m_pDBSyzAcc;			// テーブルアクセスクラス
	int					m_SnSeq;				// 履歴シーケンス

	//記憶領域
//	int				Sn_SEQ;
	char			Sn_F6_DEATH[4];
	CString			Sn_F6_DAIHYO;
	int				Sn_F6_LIMIT;

	int				Sn_F6_USE;			// 事業承継の有無
	unsigned char	Sn_F6_ADR[40*2];	// 事業承継の住所
	CString			Sn_F6_TELNO1;		// 事業承継の電話番号
	CString			Sn_F6_TELNO2;		// 事業承継の電話番号
	CString			Sn_F6_TELNO3;		// 事業承継の電話番号
	CString			Sn_F6_NMFRI;		// 事業承継のフリガナ
	CString			Sn_F6_NAME;			// 事業承継の氏名
	unsigned char 	Sn_F6_SONOTA[40*2];	// 事業承継のその他

	typedef struct _Fuhyo6_sozoku{
//		int			Sn_SEQ;
		int			Sn_NO;
		CString		Sn_F6_JUSYO;
		CString		Sn_F6_FURI;
		CString		Sn_F6_NAME;
		char		Sn_F6_HASH[33];
		CString		Sn_F6_MYNUM1;	//メモリ上で記憶するのみで、DBには書き込まない
		CString		Sn_F6_MYNUM2;
		CString		Sn_F6_MYNUM3;
		CString		Sn_F6_SYOKU;
		CString		Sn_F6_TSUDUKI;
		char		Sn_F6_BIRTH[4];
		CString		Sn_F6_TEL1;
		CString		Sn_F6_TEL2;
		CString		Sn_F6_TEL3;
		int			Sn_F6_HOTEI;
		CString		Sn_F6_BUNSHI;
		CString		Sn_F6_BUNBO;
		char		Sn_F6_KAGAKU[MONY_BUF_SIZE];
		char		Sn_F6_NOFUZEI[MONY_BUF_SIZE];
		char		Sn_F6_NOFUCHIHO[MONY_BUF_SIZE];
		char		Sn_F6_KANPUZEI[MONY_BUF_SIZE];
		char		Sn_F6_KANPUCHIHO[MONY_BUF_SIZE];
		int			Sn_F6_NOFUKANPUsw;
		CString		Sn_F6_GNAME;
		int			Sn_F6_GSEL;
		CString		Sn_F6_SNAME;
		int			Sn_F6_SSEL;
		CString		Sn_F6_YOKIN;
		CString		Sn_F6_KOUZA;
		CString		Sn_F6_KIGO1;
		CString		Sn_F6_KIGO2;
		CString		Sn_F6_YUBIN;
	}Fuhyo6_sozoku;

	Fuhyo6_sozoku sozoku[SOZOKU_MAX]; //相続人テーブル


	//== 帳票データ ==
	char			XM8[MONY_BUF_SIZE],		//  8. 控除不足還付税額
					XM11[MONY_BUF_SIZE],	// 11. (9-10) 納付税額
					XM12[MONY_BUF_SIZE],	// 12. 中間納付還元税額
					XM13[MONY_BUF_SIZE],	// 13. 既確定税額
					XM19[MONY_BUF_SIZE],	// 19. 還付額
					XM22[MONY_BUF_SIZE],	// 22. 納付譲渡割額
					XM23[MONY_BUF_SIZE],	// 23. 中間還付譲渡割額
					XM24[MONY_BUF_SIZE],	// 24. 既確定譲渡割額
					XM26[MONY_BUF_SIZE];	// 26. 消費税･地方消費税合計

	char			F61[MONY_BUF_SIZE],
					F62[MONY_BUF_SIZE],
					F63[MONY_BUF_SIZE],
					F64[MONY_BUF_SIZE],
					F65[MONY_BUF_SIZE],
					F66[MONY_BUF_SIZE],

					F69[SOZOKU_MAX][MONY_BUF_SIZE],
					F610[SOZOKU_MAX][MONY_BUF_SIZE],
					F611[SOZOKU_MAX][MONY_BUF_SIZE];

	char			m_MyNum_Haiki[SOZOKU_MAX][32];	//廃棄 個人番号
	CStringArray	m_MyNum_Haiki_Shimei;			//廃棄 氏名
	char			m_MyNum_Delete[SOZOKU_MAX][32];	//削除 個人番号
	CStringArray	m_MyNum_Delete_Shimei;			//削除 氏名
	
	int				m_MyNum_Haiki_idx;
	int				m_MyNum_Delete_idx;

		//CStringArray	My_Number1[3];
		//CStringArray	My_Number2[3];
		//CStringArray	My_Number3[3];
		//CStringArray	My_Number4[3];

	//Fuhyo6_sozoku m_sozoku[4];

		int m_EditOff_sw; //EditOffの動作のon/off

		int m_Lin;			//相続人欄の表示行(左端)
		int m_Focus;		//フォーカスのあるindex
		int	mynum_cursel;	//個人番号欄にカーソルがあるか

		CBrush mnedit_brush;	//マイナンバー入力箇所のブラシ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Fhyo6Diag;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int OnTerminationCallBack(short index, short nChar, short length, LPUNKNOWN data);
	int OnEditONCallBack(short index);
	int OnEditOFFCallBack(short index);
	int OnVScrollCallBack(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CICSMNEditController8	m_ctrl_mynum;
	CICSMNEdit8 m_edit_mynum1;
	CICSMNEdit8 m_edit_mynum2;
	CICSMNEdit8 m_edit_mynum3;
	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiagctrl1(short index);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	int F6DataSet( int index );

private:

	CWnd	*m_pParent;	// 親ウィンドウ

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	int TerminationMyNumber(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl1(short index);
	void ComboSelIcsdiagctrl1(short index, LPUNKNOWN data);

	CScrollBar m_bar;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	CFont		m_font;
	CFont		m_font_mndel[SOZOKU_PER_PAGE];
	int MynumDel(int line);
	afx_msg void OnBnClickedMynumDel1();
	afx_msg void OnBnClickedMynumDel2();
	afx_msg void OnBnClickedMynumDel3();
	afx_msg void OnBnClickedMynumDel4();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedSozokuDel();
	afx_msg void OnBnClickedSozokuIns();
};
