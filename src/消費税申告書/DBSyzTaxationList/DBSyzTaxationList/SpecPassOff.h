#pragma once


// CSpecPassOff ダイアログ

class CSpecPassOff : public ICSDialog
{
	DECLARE_DYNAMIC(CSpecPassOff)

public:
	CSpecPassOff(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSpecPassOff();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SPECOFF };

private:
	CWnd				*m_pParent;			// 親ウィンドウ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CH26SpcListData		**m_pSpcListData;	// 特定収入計算表データ
	CArithEx			*m_pArith;			// 倍長演算クラス

public:
	// 初期表示
	int InitDisp();
	// 初期値セット
	int Init( CSnHeadData **pSnHeadData, CH26SpcListData **pSpcListData, CArithEx *pArith );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
