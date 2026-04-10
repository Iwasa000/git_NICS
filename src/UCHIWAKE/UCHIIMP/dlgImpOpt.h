#pragma once
#include "icsbutton.h"
#include "afxwin.h"
#include "icsreportcontrol.h"

#include "Resource.h"

#define	OWNTB_APL_NAME			"DBUchiwake"			// apl_name
#define OWNTB_ITM_NAME			"TRANS_TYPE"			// itm_name
#define OWNTB_ITM_SEQ			1						// itm_seq
#define	OWNTB_USER_ID			0						// u_id
#define TYPE_TRANS				1
#define TYPE_OUT				0

// CdlgImpOpt ダイアログ

class CdlgImpOpt : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgImpOpt)

public:
	CdlgImpOpt(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgImpOpt();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	int				MakeReportControl();
	int				CsvDataAdd();
	int				CheckBoxControl(BOOL chkState);	
	void			GetCheckComboState();
	int				SetOwnTblInfo();
	int				GetOwnTblInfo();
// midori 170504 del -->
//	int				UpdateState(int state, int stateType);
// midori 170504 del <--
// midori 170504 add -->
	int				UpdateState(int state, int stateType, int psw);
// midori 170504 add <--

public:	
	int				ShowDialog(CDBZmSub* pZmSub, BOOL fImport, CArray<ImpOpt, ImpOpt&> &arrImpStct);
	int				GetSheetNum();
	


//変数
private:
	CArray<ImpOpt, ImpOpt&>		m_datImp;
	CDBZmSub*					m_pZmSub;
protected:
	BOOL		m_fImport;				// TRUE=インポート, FALSE=エクスポート
	int			m_nRet;					// 戻り値
	int			m_nClm;					// カラムの総数
	int			m_clmCheck;				// 「出力」カラムインデックス
	int			m_clmTrans;				// 「転送形式」カラムインデックス
	int			m_clmYoushikiNo;		// 「様式番号」カラムインデックス
	int			m_clmYoushikiName;		// 「様式名称」カラムインデックス
// midori 170504 del -->
	//int			m_stateTrans;			// 転送方式の値
	//int			m_stateImpOut;				// インポート出力状態の値
	//int			m_stateExpOut;				// エクスポート出力状態の値
// midori 170504 del <--
// midori 170504 add -->
	int			m_stateTrans[3];		// 転送方式の値				[0]:① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
										//							[1]:⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
										//							[2]:⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
	int			m_stateImpOut[3];		// インポート出力状態の値	[0]:① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
										//							[1]:⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
										//							[2]:⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
	int			m_stateExpOut[3];		// エクスポート出力状態の値	[0]:① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
										//							[1]:⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
										//							[2]:⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
// midori 170504 add <--

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IMP_OPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSReportControl m_report1;
	
	ICSButton m_btnAllOn;
	ICSButton m_btnAllOff;
	ICSButton m_btnImpStart;
	ICSButton m_btnCancel;
	ICSButton m_btnChgTrans;
	ICSStatic m_stcOptMessage;
// 登録番号（法人番号）_23/11/21 add -->
	ICSStatic m_stcOptMessage2;
	ICSStatic m_stcOptMessage3;
// 登録番号（法人番号）_23/11/21 add <--
	afx_msg void OnBnClickedButtonAllon();
	afx_msg void OnBnClickedButtonAlloff();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonImpStart();
	afx_msg void OnBnClickedButtonChgTrans();
	void OnNMReportCheckedCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);	
	void OnNMReportEditkeydownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result);
};
