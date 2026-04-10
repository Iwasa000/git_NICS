// 24/02/19wd_特定収入対応 add -->
#pragma once

// CSpecPassOffEx2 ダイアログ

enum ZEIRITSU2{
	zeiritsu2_gokei = 0,
	zeiritsu2_8per,
	zeiritsu2_5per,
	zeiritsu2_8perK,
	zeiritsu2_10per
};

class CSpecPassOffEx2 : public ICSDialog
{
	DECLARE_DYNAMIC(CSpecPassOffEx2)

public:
	CSpecPassOffEx2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSpecPassOffEx2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SPECOFF_EX2 };

public:
	int					m_ListNo;			// 0:計算表５(１)、1:計算表５(２)、2:計算表５(３)

private:
	CWnd				*m_pParent;			// 親ウィンドウ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CH31SpcListData		**m_pSpcListData;	// 特定収入計算表データ
	CArithEx			*m_pArith;			// 倍長演算クラス

public:
	// 初期表示
	int Disp_10per();			//金額表示(１０％)
	int Disp_8perK();			//金額表示(８％軽減税率)
	int Disp_8per();			//金額表示(８％)
	int Disp_5per();			//金額表示(５％)
	void Edit_Static_10per();	//固定文字編集(１０％)
	void Edit_Static_8perK();	//固定文字編集(８％軽減税率)
	void Edit_Static_8per();	//固定文字編集(８％)
	void Edit_Static_5per();	//固定文字編集(５％)

	void Disp_AllClear();
	int GetZeiritsu();

	// 税率用コンボボックス初期化
	void InitZrtCombo();
	// 税率に合わせた画面切替
	void ChgDispByZeiritsu();

	CIcsdiagctrl m_Diag1;

	// 初期値セット
	int Init( CSnHeadData **pSnHeadData, CH31SpcListData **pSpcListData, CArithEx *pArith );
	int InitControl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ComboSelIcsdiagctrl1(short index, LPUNKNOWN data);
};
// 24/02/19wd_特定収入対応 add <--
