#pragma once
#include "icsbutton.h"
#include "afxwin.h"

#define ID_ERR_COPY_PRINT	-1
#define ID_ERR_COPY_KAMOKU	-2
#define ID_ERR_COPY_TITLE	-3

// CdlgCopySettingInfo ダイアログ

class CdlgCopySettingInfo : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgCopySettingInfo)

public:
	CdlgCopySettingInfo(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgCopySettingInfo();
	BOOL			OnInitDialog();
// midori 160612 cor -->
//	virtual int		ShowDialog(CDatabase* pDB = NULL);
// ---------------------
	virtual int		ShowDialog(CDatabase* pDB = NULL,BOOL pDataKakutei = 0x04);
// midori 160612 cor <--
	virtual	BOOL	PreTranslateMessage(MSG* pMsg);


private:
	int		GetSettingInfoFromOtherMaster();
// midori UC_0012 del -->
//	int		SynchroReferenceData();();
// midori UC_0012 del <--
// midori UC_0012 add -->
	int		SynchroReferenceData();
// midori UC_0012 add <--
	int		CheckTableVersion(CDBZmSub* pZmSub, CString strBaseTable);

private:
	int					m_nRet;				// 戻値
// midori 160612 add -->
	BOOL				m_DataKakutei;		// 入力データを確定する
// midori 160612 add <--
	CDatabase*			m_pDB;				//	データベースハンドル
	CUcFunctionCommon	m_clsFunc;			//	共通関数クラス
	int					m_nErrPoint;		// エラー発生箇所　-1:印刷設定　-2:科目　-3:タイトル

// ダイアログ データ
	enum { IDD = IDD_DIALOG_COPY_SETTING_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

private:
	CString GetSQLGeneralVarCopy(CString strBaseTable, CString strTargetTable, int offset, int bit);
// midori UC_0056 add -->
	CString GetSQLGeneralVarCopy_Infmain(CString strBaseTable, CString strTargetTable, int offset, int bit);
// midori UC_0056 add <--

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	CButton m_chkPrint;
	CButton m_chkKamoku;
	CButton m_chkTitle;
};
