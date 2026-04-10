#pragma once
#include "icsreportcontrol.h"
#include "afxwin.h"


// CSkjKjnConvert ダイアログ

class CSkjKjnConvert : public ICSDialog
{
	DECLARE_DYNAMIC(CSkjKjnConvert)

public:
	CSkjKjnConvert(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSkjKjnConvert();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SKJKJNCNV };

	bool m_isSkjUsrUnconfirmed; // VerUpで還付申告明細用連動情報設定を採用する利用者が未確定か // TISW修正[22-0676] 2026/03/19

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	BOOL	InizDataList;
	int		Inp_l, Inp_c;

	void SetListColm();						// 変換情報の表示				橋
	int GetKjnCodName( int );				// 集計項目番号の取得
	void SetSnameString( int, int );		// ポジションセット

public:
	// 初期情報のセット
	int InitInfo( CDBNpSub *pZmSub, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl );

	DECLARE_MESSAGE_MAP()

private:
	CSnHeadData		*m_pSnHeadData;	// 消費税ヘッダ情報
	CDBNpSub		*m_pZmSub;		// 公益クラス
	CTblhdl			*m_pTblhdl;		// テーブルハンドリング
	_TB_PAR			*m_pSkjCnvtbl;	// 仕入控除用テーブル
	CSyzUtil		m_Util;

public:
//	static	CDBSyzShinView *pSyzShin;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonKana();
	ICSReportControl m_Report;
	CListBox m_ListBox;
	afx_msg void OnLbnDblclkList1();
	void OnNMReportLbuttondownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportValuechangedCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkeydownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditsetfocusCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkillfocusCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
};
