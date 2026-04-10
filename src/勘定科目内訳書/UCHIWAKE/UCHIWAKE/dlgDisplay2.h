#pragma once

#include "icsbutton.h"

#include "dbUc011Yotyokin.h"
#include "dbUc021Uketoritegata.h"
#include "dbUc031Urikakekin.h"
#include "dbUc041Karibarai.h"
#include "dbUc042Kasituke.h"
#include "dbUc051Tanaoroshi.h"
#include "dbUc061Yuukasyouken.h"
#include "dbUc071Koteishisan.h"
#include "dbUc081Siharaitegata.h"
#include "dbUc091Kaikakekin.h"
#include "dbUc101Kariukekin.h"
#include "dbUc102Gensen.h"
#include "dbUc111Kariirekin.h"
#include "dbUc121Totiuriage.h"
#include "dbUc131Uriagedaka.h"
#include "dbUc141Yakuin.h"
#include "dbUc142Jinkenhi.h"
#include "dbUc151Tidaiyatin.h"
#include "dbUc152Kenrikin.h"
#include "dbUc153Kougyou.h"
#include "dbUc161Zatueki.h"
#include "dbUc162Zatuson.h"
#include "dbUc171Sonota1.h"
#include "dbUc181Sonota2.h"

#define		VAL_FORMAT01		"sssssssssssss9"
#define		VAL_FORMAT02		"ss,sss,sss,sss,ss9"
#define		VAL_FORMAT03		"sssssssssssssss9"
#define		VAL_FORMAT04		"s,sss,sss,sss,sss,ss9"
#define		VAL_FORMAT05		"sssssssss9.999"
#define		VAL_FORMAT06		"s,sss,sss,ss9.999"
#define		VAL_FORMAT07		"ssssssssss9.99"
#define		VAL_FORMAT08		"ss,sss,sss,ss9.99"
#define		VAL_FORMAT09		"sssssssssss9"
#define		VAL_FORMAT10		"sss,sss,sss,ss9"
#define		VAL_FORMAT11		"sssssssssssss9.999"
#define		VAL_FORMAT12		"ss,sss,sss,sss,ss9.999"
#define		VAL_FORMAT13		"sssssssssssssssss9"
#define		VAL_FORMAT14		"sss,sss,sss,sss,sss,ss9"
#define		VAL_FORMAT15		"ssssssssssss9.99"
#define		VAL_FORMAT16		"s,sss,sss,sss,ss9.99"
#define		VAL_FORMAT17		"ss,sss,sss,ss9"
#define		VAL_FORMAT18		"sss,sss,ss9"
#define		VAL_FORMAT19		"sss,sss,ss9.999"
#define		VAL_FORMAT20		"ss,sss,ss9.99"
#define		VAL_FORMAT21		"s,sss,ss9.99"
#define		VAL_FORMAT22		"sss,ss9"
#define		VAL_FORMAT23		"sss,ss9.99"
#define		VAL_FORMAT24		"ss9.999"
#define		VAL_FORMAT25		"s9.999"
#define		VAL_FORMAT26		"s,sss,sss,ss9"
// No.200903 add -->
#define		VAL_FORMAT27		"s,sss,sss,sss,ss9"
// No.200903 add <--

typedef struct	_HIDE_DATA	{
	int		hd_0;	// 選択値
	int		hd_1;	// シーケンス番号
}HIDE_DATA;

// CdlgDisplay2 ダイアログ

class CdlgDisplay2 : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgDisplay2)

public:
	CdlgDisplay2(CWnd* pParent = NULL);					//	標準コンストラクタ
	CdlgDisplay2(UINT id, CWnd* pParent = NULL);		//	コンストラクタ
	virtual ~CdlgDisplay2();							//	デストラクタ

// ダイアログ データ
	enum { IDD = IDD_DIALOG_DISPLAY2 };

public:
	ICSReportControl	m_RepDisplay;			//	保管参照/一括金額参照リスト
	ICSStatic			m_lblHosoku;			//	補足
	ICSButton			m_btnDelete;			//	削除ボタン
	ICSButton			m_btnOK;				//	戻すボタン
	ICSButton			m_btnCancel;			//	閉じるボタン
	ICSButton			m_btnAllOn;				//	全選択ボタン
	ICSButton			m_btnAllOff;			//	全解除ボタン
// No.200602 add -->
	CButton				m_chkRyaku;				//	全項目を１画面に表示するチェックボックス
// No.200602 add <--

private:
	EnumIdFgShow		m_nType;				//	種別
	CDBZmSub*			m_ZmSub;				//	財務クラス
	CDatabase*			m_pDB;					//	データベースハンドル
	short				m_nFormSeq;				//	様式シーケンス番号
	int					m_nRet;					//	戻値
	int					m_nColumnMax;			//	カラム最大数

	int					m_nNumGroup;			//	グループ番号
	CFont				m_font;					//	フォント情報
	COLORREF			m_ViewColor;			//	レポートの背景色
	COLORREF			m_SelectColor;			// レポートの背景色（選択行）
	BOOL				m_DataKakutei;			//	入力データを確定する
	CString				m_nColumnTitle[20];		//	カラムタイトル
	int					m_nColumnWidth[20];		//	カラム幅
// No.200602 add -->
	int					m_Flg1;					//	0:従来どおりの表示 1:「全項目を１画面に表示する」
	int					m_widthAll;				//	レポート全体の幅
// No.200602 add <--
	CUcFunctionCommon	m_clsFunc;				// 共通関数クラス

	CMap<int,int,HIDE_DATA,HIDE_DATA>			m_HideMap;
	HIDE_DATA									m_HideDat;

	CString				m_Fmt;					// フォーマット
// midori 190301 add -->
	int					m_Ofs;
// midori 190301 add <--

// No.158072 add -->
	int					m_SortSw[2];
	int					m_SaveSw[2];
// No.158072 add <--
// 21/06/24_01 add -->
	int					m_DtUse[20];
// 21/06/24_01 add <--

public:
	int ShowDialog( CDBZmSub* pZmSub = NULL, short nFormSeq = ID_FORMNO_011, 
					EnumIdFgShow nType = ID_FGSHOW_HOKAN, int nNumGroup = 0, BOOL pDataKakutei = 0x04 );	//	ダイアログ表示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
// No.200602 del -->
//	void	InitReportHeader(short nFormSeq);										// レポートコントロールの初期化、ヘッダ部作成
// No.200602 del <--
// No.200602 add -->
// No.158013 del -->
//	void	InitReportHeader(short pFormSeq);										// レポートコントロールの初期化、ヘッダ部作成
// No.158013 del <--
// No.158013 add -->
	void	InitReportHeader(short pFormSeq, int pSw);								// レポートコントロールの初期化、ヘッダ部作成
// No.158013 add <--
	void	GetTitleNameAndSize(int pMaxCol, int pFormNo, CString pColmnTitle[], int pColmnWidth[]);	// 様式ごとの列サイズと項目タイトルをセット
	void	ListWidthWarihuri(int pWidthAll, int pCMax, int* pCWidth);				// 各列幅に余った分の余白を足す
	void	RestoreReportnWidth(void);												// 列幅を読み込むする
	void	SaveReportnWidth(void);													// 列幅を保存する
	int		OwntbKeyMake(int pFlg);													// 保持している列幅を取得するためのキーを作成
// No.200602 add <--
	CString	GetSonota2ItemName(void);												// 項目名称取得（⑱その他科目の内訳書②専用）
	void	SetGrid(void);															// レポートコントロールの垂直線を設定
	void	InitReportData(short nFormSeq, CDatabase* pDB, EnumIdFgShow nType);		// レポートコントロールの初期化、ヘッダ部作成

	int		SetYoushiki011Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ①.預貯金等［様式011］データ設定
	int		SetYoushiki021Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ②.受取手形［様式021］データ設定
	int		SetYoushiki021IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ②.受取手形［様式021］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki031Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ③.売掛金［様式031］データ設定
	int		SetYoushiki031IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ③.売掛金［様式031］データ設定(インボイス番号対応版)						インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki041Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ④－１.仮払金［様式041］データ設定
	int		SetYoushiki041IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ④－１.仮払金［様式041］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki042Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ④－２.貸付金［様式042］データ設定
	int		SetYoushiki042IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ④－２.貸付金［様式042］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki051Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑤.棚卸資産［様式051］データ設定
	int		SetYoushiki061Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑥.有価証券［様式061］データ設定
	int		SetYoushiki071Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑦.固定資産［様式071］データ設定
	int		SetYoushiki071IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑦.固定資産［様式071］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki081Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑧.支払手形［様式081］データ設定
	int		SetYoushiki081IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑧.支払手形［様式081］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki091Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑨.買掛金［様式091］データ設定
	int		SetYoushiki091IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑨.買掛金［様式091］データ設定(インボイス番号対応版)						インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki101Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑩－１.仮受金［様式101］データ設定
	int		SetYoushiki101IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑩－１.仮受金［様式101］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki102Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑩－２.源泉取得税［様式102］データ設定
	int		SetYoushiki111Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑪.借入金［様式111］データ設定
	int		SetYoushiki121Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑫.土地の売上高等［様式121］データ設定
	int		SetYoushiki121IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑫.土地の売上高等［様式121］データ設定(インボイス番号対応版)				インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki131Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑬.売上高等の事業所別［様式131］データ設定
	int		SetYoushiki141Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑭－１.役員報酬手当金等［様式141］データ設定
	int		SetYoushiki151Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑮－１.地代家賃等［様式151］データ設定
	int		SetYoushiki151IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑮－１.地代家賃等［様式151］データ設定(インボイス番号対応版)				インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki152Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑮－２.権利金等の期中支払［様式152］データ設定
	int		SetYoushiki152IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑮－２.権利金等の期中支払［様式152］データ設定(インボイス番号対応版)		インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki153Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑮－３.工業所有権等の使用料［様式153］データ設定
	int		SetYoushiki153IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑮－３.工業所有権等の使用料［様式153］データ設定(インボイス番号対応版)	インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki161Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑯－１.雑益［様式161］データ設定
	int		SetYoushiki161IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑯－１.雑益［様式161］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki162Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑯－２.雑損失［様式162］データ設定
	int		SetYoushiki162IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑯－２.雑損失［様式162］データ設定(インボイス番号対応版)					インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki171Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑰.その他科目の内訳書①［様式171］データ設定
	int		SetYoushiki171IData(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);	// ⑰.その他科目の内訳書①［様式171］データ設定(インボイス番号対応版)		インボイス登録番号追加対応_23/09/11 add
	int		SetYoushiki181Data(CDatabase* pDB, EnumIdFgShow nType, int nColumnMax);		// ⑱.その他科目の内訳書②［様式181］データ設定

	int		CheckSelectItem(void);
	int		CheckListItam(BOOL fDel = FALSE);
	int		SetFgShow(CdbUc000Common* pRec, BOOL fDel=FALSE);						// 選択行の表示フラグ設定
	void	UpdateStateFromKey(int key);

// No.158072 del -->
//// No.158013 add -->
//	CString	MakeFormatNum(int nIntDigit, int nDecDigit = 0, BOOL bFgComma = TRUE, BOOL bComma13 = TRUE);
//// No.158013 add <--
// No.158072 del <--
// No.158072 add -->
	int		GetHeadClm(CPoint point);
// No.158072 add <--

// 21/06/24_01 add -->
	BOOL	DataUseCheck(LPCTSTR psText, int pindex);								// データが存在したらフラグを立てる
// 21/06/24_01 add <--


public:
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedAllButtonOn();
	afx_msg void OnBnClickedAllButtonOff();
	afx_msg void OnBnSetfocusOkButton();
	afx_msg void OnBnSetfocusDeleteButton();
	afx_msg void OnBnSetfocusCancelButton();
	afx_msg void OnBnSetfocusAllButtonOn();
	afx_msg void OnBnSetfocusAllButtonOff();

	void OnNMReportLbuttondownDISPLAY_REPORT(NMHDR * pNotifyStruct, LRESULT * result);
	LRESULT OnUserRcclick(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserFocusset(WPARAM wParam, LPARAM lParam);
// No.200602 add -->
	afx_msg void OnBnClickedCheckRyaku();
	afx_msg void OnBnSetfocusCheckRyaku();
// No.200602 add <--

// No.158072 add -->
	LRESULT OnUserReportSort( WPARAM wParam, LPARAM lParam );
// No.158072 add <--
};
