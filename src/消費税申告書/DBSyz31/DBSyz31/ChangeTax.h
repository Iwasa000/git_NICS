#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CChangeTax ダイアログ

class CChangeTax : public ICSDialog
{
	DECLARE_DYNAMIC(CChangeTax)

public:
	CChangeTax(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CChangeTax();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CHGTAX };

private:
	char				MNK40[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率4%分)
						MNZ40[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )
						MENZK[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率3%分)
						MENZZ[MONY_BUF_SIZE],	// 				 〃	   消費税額   (〃      )
						MNK63[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)
						MNZ63[MONY_BUF_SIZE];	//				 〃	   消費税額   (〃      )

	char				MNK78[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率7.8%分)
						MNZ78[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )
						MNK624[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率6.24%分)
						MNZ624[MONY_BUF_SIZE];	// 				 〃	   消費税額   (〃      )

	CArithEx			*m_pArith;				// 倍長演算
	CSyzUtil			m_Util;					// ユーティリティクラス	
	CSnHeadData			*m_pSnHeadData;			// 消費税ヘッダ情報
	CH31SnTanaData		*m_pSnTanaData;			// 棚卸調整データ

	int					m_CbSw;
	int					m_ChSw;					// 税込み税抜きサイン(一時保存用)
	int					tmpChSw;				//
	int					m_CbSt;					//

	unsigned char		tansw;					// 棚卸仕訳発生サイン(一時保存用)

public:
	// 初期情報のセット
	int InitInfo ( CSnHeadData *pSnHeadData, CH31SnTanaData *pSnTanaData, CArithEx *pArith );

	// 初期化
	int Init();

	// 入力項目色変更
	void ChangeColor ( unsigned short id, short index, int sign );

	// 税変更時の計算処理
	void ZeiChangeCalq();

	// コンボボックスの情報をチェック
	void ComboCheck_AttrSet();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CIcsdiagctrl m_ICSDIAG1;
	CStatic m_txt1;
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void ComboSelIcsdiagctrl1(short index, LPUNKNOWN data);
};
