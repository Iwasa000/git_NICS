// midori 190505 add -->
#pragma once

#include "icsreportcontrol.h"


// --------------------------------------------------------------------------------------------------------------------
typedef struct IKKATU_CONF_DATA_{
	int			FormSeq;		// 様式シーケンス
	int			cmb1;			// 出力方法	0:改頁･空行を詰める、1:改頁･空行を保持する、2:第１ソート項目で改頁する
	int			start;			// 開始頁
	int			end;			// 終了頁
	int			max_end;		// 最大頁数

	IKKATU_CONF_DATA_(){
		FormSeq = 0;
		cmb1 = 0;
		start = 0;
		end = 0;
		max_end = 0;
	}
}IKKATU_CONF_DATA;

enum enumIdColumnIkkatuConfirmation{	// レポートコントロールのカラムID
	ih_youshiki_no,		// 様式番号
	ih_youshiki_name,	// 様式名称
	ih_Combo,			// 出力方法
	ih_start,			// 開始頁
	ih_end,				// 終了頁
};
// --------------------------------------------------------------------------------------------------------------------

// CdlgIkkatuConfirmation ダイアログ

class CdlgIkkatuConfirmation : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgIkkatuConfirmation)

public:
	CdlgIkkatuConfirmation(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CdlgIkkatuConfirmation();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IKKATU_CONFIRMATION };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	ICSReportControl			m_report;
	CICSDisp					m_Disp1;				// ガイドメッセージ１
// midori 156907 add -->
	CICSDisp					m_Disp2;				// ガイドメッセージ２
// midori 20/10/08u del -->
	//CICSDisp					m_Disp3;				// ガイドメッセージ３
// midori 20/10/08u del <--
	CICSDisp					m_Disp4;				// ガイドメッセージ４
// midori 156907 add <--
	ICSButton					m_btnOk;				// 「OK」ボタン
	ICSButton					m_btnCan;				// 「ｷｬﾝｾﾙ」ボタン

private:
	int							m_nRet;					// 戻り値
	CDatabase*					m_pDB;					// データベースハンドル
	CUcFunctionCommon			m_clsFunc;				// 共通関数クラス

public:
	CMap<int,int,IKKATU_CONF_DATA,IKKATU_CONF_DATA>		m_mapIkkatuConfData;
	vector<PRINT_ALL_DATA>*								m_vecPrintAll;			// レポートコントロールのレコード情報を持つベクタ

	int		ShowDialog( CDatabase* pDB, vector<PRINT_ALL_DATA>* pvecPrintAll);

	// レポートコントロールの初期化
	void	InitReportHeader(void);
	//	レポートコントロール　グリッド線の設定
	void	SetReportGrid(void);
	//	レポートコントロールにデータを追加
	void	AddRecords(void);
	//	レポートのチェックボックスにデータをセット
	void	SetData(void);

	// 出力方法コンボボックスが変更された場合、開始頁と終了頁を再セット
	int		CheckComboDt(int, int);
	//	入力されたページ数が範囲内かどうか全列チェック
	int		CheckPageNum_All(void);
	//	入力されたページ数が範囲内かどうかチェック
	int		CheckPageNum(int, int);
	//	レポートの状態をマップに更新
	void	UpdateIkkatuConfMap(void);

	//	キー操作
	void	UpdateStateFromKey(int key);


	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void OnNMReportEditkeydownCUSTOM_REPORT_IKKATUCONF(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkillfocusCUSTOM_REPORT_IKKATUCONF(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportValuechangedCUSTOM_REPORT_IKKATUCONF(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedCancelButton();
};
// midori 190505 add <--
