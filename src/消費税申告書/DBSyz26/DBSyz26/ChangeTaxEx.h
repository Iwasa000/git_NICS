#pragma once
#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"
#include "afxwin.h"


// CChangeTaxEx ダイアログ

class CChangeTaxEx : public ICSDialog
{
	DECLARE_DYNAMIC(CChangeTaxEx)

public:
	CChangeTaxEx(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CChangeTaxEx();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CHGTAXEX };

	// 初期情報のセット
	int InitInfo( CH26SnFhyo20Data *pSnFhyo20Data, CSnHeadData *pSnHeadData, CH26SnTanaData *pSnTanaData, CArithEx *pArith );
	// 初期化
	int Init();

	// 金額表示
	void DispMony( short Index, char *Mony );
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );

	void ZeiChangeCalq();			//[Y]税変更時の計算処理。関数化
	void ComboCheck_AttrSet();		//現在のコンボボックスの情報をチェックし入力属性、背景色をセット

private:
	//== 帳票データ ==
	char				MNK40K[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　課税に係る
						MNZ40K[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃
						MNK40H[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　非課税に係る
						MNZ40H[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃
						MNK40C[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率4%分)　共通に係る
						MNZ40C[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃

						MENZKK[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　課税に係る
						MENZZK[MONY_BUF_SIZE],	// 				 〃	   消費税額   (〃      )　〃
						MENZKH[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　非課税に係る
						MENZZH[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃
						MENZKC[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率3%分)　共通に係る
						MENZZC[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃
					
						MNK63K[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率6.3分)　課税に係る
						MNZ63K[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃
						MNK63H[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率6.3分)　非課税に係る
						MNZ63H[MONY_BUF_SIZE],	//				 〃	   消費税額   (〃      )　〃
						MNK63C[MONY_BUF_SIZE],	// 調整課税仕入等に係る棚卸資産額 (税率6.3分)　共通に係る
						MNZ63C[MONY_BUF_SIZE];	//				 〃	   消費税額   (〃      )　〃
	//== 帳票データ本体 ==
	CH26SnFhyo20Data	*m_pSnFhyo20Data;
	CSnHeadData			*m_pSnHeadData;			// 消費税ヘッダ情報
	CH26SnTanaData		*m_pSnTanaData;			// 棚卸調整データ
	CSyzUtil			m_Util;					// ユーティリティクラス

	CArithEx			*m_pArith;				// 倍長演算

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
//	static	CDBSyzShinView *pSyzShin;
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
