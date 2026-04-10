#pragma once


// CSpecPassOffEx ダイアログ

enum ZEIRITSU{
	zeiritsu_gokei = 0,
	zeiritsu_8per,
	zeiritsu_5per,
};

class CSpecPassOffEx : public ICSDialog
{
	DECLARE_DYNAMIC(CSpecPassOffEx)

public:
	CSpecPassOffEx(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSpecPassOffEx();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SPECOFF_EX };

private:
	CWnd				*m_pParent;			// 親ウィンドウ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CH26SpcListData		**m_pSpcListData;	// 特定収入計算表データ
	CArithEx			*m_pArith;			// 倍長演算クラス

public:
	// 初期表示
//	int InitDisp();
//	int Disp_goukei();
	int Disp_8per();			//金額表示(８％)
	int Disp_5per();			//金額表示(５％)
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
	int Init( CSnHeadData **pSnHeadData, CH26SpcListData **pSpcListData, CArithEx *pArith );
	int InitControl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ComboSelIcsdiagctrl1(short index, LPUNKNOWN data);
};
