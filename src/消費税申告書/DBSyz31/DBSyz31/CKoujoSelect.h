#pragma once
#include "icsdiagctrl.h"

// CKoujoSelect ダイアログ

class CKoujoSelect : public ICSDialog
{
	DECLARE_DYNAMIC(CKoujoSelect)

public:
	CKoujoSelect(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CKoujoSelect();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KOUJOSELECT };

public:
	// 初期情報のセット
	int InitInfo ( CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, CArithEx *pArith );
	// 選択された項目の取得
	int GetSelectData ( int &getId );

private:
	// ビューの初期化
	int InitView();
	// ダイアグラムの初期化
	int InitDiagAttr();
	// 売上割合チェック
	int CheckUriageWariai ( short index );
	// 売上割合取得
	int GetUriageWariai ( int id, unsigned short *pWari );
	// ラジオボタンのセット
	void SetRadioButton ( short index );
	// メッセージの取得
	int GetSelectMessage ( short index, CString &msg );
	// 選択中の項目ID取得
	int GetSelectID ( int &getID );
private:
	CSnHeadData			*m_pSnHeadData;		// ヘッダー情報
	CSyzSyukei			*m_pSyzSyukei;		// 集計クラス
	CArithEx			*m_pArith;			// 倍長演算

	int					m_selIndex;			// 選択中のインデックス
	CWnd				*m_pParent;			// 親ウィンドウ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	CIcsdiagctrl m_KselDiag;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiag8ctrl1(short index);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
