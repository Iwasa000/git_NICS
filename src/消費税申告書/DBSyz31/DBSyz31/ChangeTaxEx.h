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
	enum { IDD = IDD_DIALOG_CHGTAXEX};

	
	// 初期情報のセット
	int InitInfo(CH31SnFhyo2_02Data *pSnFhyo2_02Data, CSnHeadData *pSnHeadData, CH31SnTanaData *pSnTanaData, CArithEx *pArith);
	
	// 初期化
	int Init();

	// 金額表示
	void DispMony( short Index, char *Mony );
	
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );

	//税変更時の計算処理
	void ZeiChangeCalq();

	//現在のコンボボックスの情報をチェックし入力属性、背景色をセット
	void ComboCheck_AttrSet();		
	


private:
	//== 帳票データ ==
	char				MNK40K[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率4%分)　課税に係る
						MNZ40K[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃
						MNK40H[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率4%分)　非課税に係る
						MNZ40H[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃
						MNK40C[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率4%分)　共通に係る
						MNZ40C[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃

						MENZKK[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率3%分)　課税に係る
						MENZZK[MONY_BUF_SIZE],		// 				 〃	   消費税額   (〃      )　〃
						MENZKH[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率3%分)　非課税に係る
						MENZZH[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃
						MENZKC[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率3%分)　共通に係る
						MENZZC[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃
					
						MNK63K[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率6.3分)　課税に係る
						MNZ63K[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃
						MNK63H[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率6.3分)　非課税に係る
						MNZ63H[MONY_BUF_SIZE],		//				 〃	   消費税額   (〃      )　〃
						MNK63C[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額 (税率6.3分)　共通に係る
						MNZ63C[MONY_BUF_SIZE];		//				 〃	   消費税額   (〃      )　〃
	
	char				MNK624K[MONY_BUF_SIZE],		// 調整課税仕入れ等に係る棚卸資産額（税率6.24%）　課税に係る
						MNZ624K[MONY_BUF_SIZE],		//			〃			 消費税額　（〃　　　 ）　〃
						MNK624H[MONY_BUF_SIZE],		// 調整課税仕入れ等に係る棚卸資産額（税率6.24%）　非課税に係る
						MNZ624H[MONY_BUF_SIZE],		//			〃			 消費税額　（〃　　　 ）　〃
						MNK624C[MONY_BUF_SIZE],		// 調整課税仕入れ等に係る棚卸資産額（税率6.24%）　共通に係る
						MNZ624C[MONY_BUF_SIZE],		//			〃			 消費税額　（〃　　　 ）　〃

						MNK78K[MONY_BUF_SIZE],		// 調整課税仕入れ等に係る棚卸資産額（税率7.8%）　課税に係る
						MNZ78K[MONY_BUF_SIZE],		//			〃			 消費税額　（〃　　　 ）　〃
						MNK78H[MONY_BUF_SIZE],		// 調整課税仕入れ等に係る棚卸資産額（税率7.8%）　非課税に係る
						MNZ78H[MONY_BUF_SIZE],		//			〃			 消費税額　（〃　　　 ）　〃
						MNK78C[MONY_BUF_SIZE],		// 調整課税仕入れ等に係る棚卸資産額（税率7.8%）　共通に係る
						MNZ78C[MONY_BUF_SIZE];		//			〃			 消費税額　（〃　　　 ）　〃

	//== 帳票データ本体 ==
	CH31SnFhyo2_02Data	*m_pSnFhyo2_02Data;
	CSnHeadData			*m_pSnHeadData;			// 消費税ヘッダ情報
	CH31SnTanaData		*m_pSnTanaData;			// 棚卸調整データ
	CSyzUtil			m_Util;					// ユーティリティクラス

	CArithEx			*m_pArith;				// 倍長演算

	int					m_CbSw;					// 
	int					m_ChSw;					// 税込み税抜きサイン(一時保存用)
	int					tmpChSw;				//
	int					m_CbSt;					// 

	unsigned char		tansw;					// 棚卸仕訳発生サイン(一時保存用)
	

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
