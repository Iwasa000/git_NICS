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

// CShinFhyo60 ダイアログ

class CShinFhyo60 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo60)

public:
	CShinFhyo60(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo60();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY60 };

	// 初期情報のセット
	int InitInfo( CH28SnFhyo60Data **pSnFhyo60Data, CH26SnFhyo10Data **pSnFhyo10Data, CH26SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CH26SnHonpyoData **pSnHonpyoData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo );
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

	// 付表６計算
	void f6calq_1to6();
	void f6calq_9to14( int index );


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

	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CH26SnHonpyoData	**m_pSnHonpyoData;	// 本表データ
	CH26SnFhyo10Data	**m_pSnFhyo10Data;	// 付表１データ
	CH26SnFhyo10Data	**m_pSnFhyo40Data;	// 付表４データ
	CH28SnFhyo60Data	**m_pSnFhyo60Data;	// 付表６データ

	CDBNpSub			*m_pZmSub;					// 財務クラス
	CArithEx			*m_pArith;
	CSyzUtil			m_Util;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報

	//記憶領域
//	int				Sn_SEQ;
	char			Sn_6FSDEATH[4];
	CString			Sn_6FSDAIHYO;
	int				Sn_6FSLIMIT;
	int				Sn_6FSDEATH_GENGO;

	typedef struct _Fuhyo6_sozoku{
//		int			Sn_SEQ;
		int			Sn_NO;
		CString		Sn_6FSJUSYO;
		CString		Sn_6FSFURI;
		CString		Sn_6FSNAME;
		char		Sn_6FSHASH[33];
		CString		Sn_6FSMYNUM1;	//メモリ上で記憶するのみで、DBには書き込まない
		CString		Sn_6FSMYNUM2;
		CString		Sn_6FSMYNUM3;
		CString		Sn_6FSSYOKU;
		CString		Sn_6FSTSUDUKI;
		char		Sn_6FSBIRTH[4];
		CString		Sn_6FSTEL1;
		CString		Sn_6FSTEL2;
		CString		Sn_6FSTEL3;
		int			Sn_6FSHOTEI;
		CString		Sn_6FSBUNSHI;
		CString		Sn_6FSBUNBO;
		char		Sn_6FSKAGAKU[MONY_BUF_SIZE];
		char		Sn_6FSNOFUZEI[MONY_BUF_SIZE];
		char		Sn_6FSNOFUCHIHO[MONY_BUF_SIZE];
		char		Sn_6FSKANPUZEI[MONY_BUF_SIZE];
		char		Sn_6FSKANPUCHIHO[MONY_BUF_SIZE];
		int			Sn_6FSNOFUKANPUsw;
		CString		Sn_6FSGNAME;
		int			Sn_6FSGSEL;
		CString		Sn_6FSSNAME;
		int			Sn_6FSSSEL;
		CString		Sn_6FSYOKIN;
		CString		Sn_6FSKOUZA;
		CString		Sn_6FSKIGO1;
		CString		Sn_6FSKIGO2;
		CString		Sn_6FSYUBIN;
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

					//F68[SOZOKU_MAX][MONY_BUF_SIZE],
					F69[SOZOKU_MAX][MONY_BUF_SIZE],
					F610[SOZOKU_MAX][MONY_BUF_SIZE],
					F611[SOZOKU_MAX][MONY_BUF_SIZE];
					//F612[SOZOKU_MAX][MONY_BUF_SIZE],
					//F613[SOZOKU_MAX][MONY_BUF_SIZE],
					//F614[SOZOKU_MAX][MONY_BUF_SIZE];

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
		int m_Focus_old;		//フォーカスのあるindex
		int	mynum_cursel;	//個人番号欄にカーソルがあるか

		CBrush mnedit_brush;	//マイナンバー入力箇所のブラシ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Fhyo60Diag;
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
	int F60DataSet( int index );

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
