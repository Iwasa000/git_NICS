#pragma once
#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"
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

	// 初期情報のセット
	int InitInfo( CH26SnFhyo20Data *pSnFhyo20Data, CSnHeadData *pSnHeadData, CH26SnTanaData *pSnTanaData, CArithEx *pArith );
	// 初期化
	int Init();
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );

	void ZeiChangeCalq();			//[Y]税変更時の計算処理。関数化
	void ComboCheck_AttrSet();		//現在のコンボボックスの情報をチェックし入力属性、背景色をセット

private:

	//== 帳票データ ==
	char				MNK40[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率4%分)
						MNZ40[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )
						MENZK[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率3%分)
						MENZZ[MONY_BUF_SIZE],	// 				 〃	   消費税額   (〃      )
						MNK63[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率6.3%分)
						MNZ63[MONY_BUF_SIZE];	//				 〃	   消費税額   (〃      )

	CArithEx			*m_pArith;				// 倍長演算
	CSyzUtil			m_Util;					// ユーティリティクラス

	//== 帳票データ本体 ==
	CH26SnFhyo20Data	*m_pSnFhyo20Data;
	CSnHeadData			*m_pSnHeadData;			// 消費税ヘッダ情報
	CH26SnTanaData		*m_pSnTanaData;			// 棚卸調整データ

	int					m_CbSw;					// ???
	int					m_ChSw;					//[Y] 税込み税抜きサイン(一時保存用)
	int					tmpChSw;				//[Y]
	int					m_CbSt;					// ???

	unsigned char		tansw;					//[Y] 棚卸仕訳発生サイン(一時保存用)

	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	CIcsdiagctrl m_ICSDIAG1;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	DECLARE_EVENTSINK_MAP()
public:
	void EditONIcsdiagctrl1(short index);
public:
	void EditOFFIcsdiagctrl1(short index);
public:
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void ComboSelIcsdiagctrl1(short index, LPUNKNOWN data);
	CStatic m_txt1;
};
