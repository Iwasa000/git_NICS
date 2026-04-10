//--------------------------------------------------
//	dlgImport.h
//
//	データインポート／エクスポートダイアログ
//
//	2009.06.17～
//--------------------------------------------------

#pragma once
#include "icsstatic.h"
#include "icsbutton.h"
#include "afxwin.h"

#include "Resource.h"

// CdlgImport ダイアログ

class CdlgImport : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgImport)

public:
	CdlgImport(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgImport();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_DLGIMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	ICSStatic	m_stcMessage;
	ICSStatic	m_stcFolder;
	CEdit		m_edtFolder;			// ICS様のテキストボックスでは実現不可能な機能があるためMFCを使用
	ICSButton	m_btnReference;
	ICSButton	m_btnOk;
	ICSButton	m_btnCancel;

	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonReference();

public:
	int			ShowDialog(BOOL fImport, CString* pstrFolderPath);

protected:
	BOOL		m_fImport;				// TRUE=インポート, FALSE=エクスポート
	CString		m_strFolderPath;		// フォルダパス
	int			m_nRet;					// 戻り値

// midori 160604 add -->
	CString		GetSpritString( CString szSrc, CString* szDst, int nPos );
// midori 160604 add <--

// 210409 del -->
//	int			GetFolderPathFromBrowser(CString strMsg, CString* pstrFolderPath, BOOL fImport);
// 210409 del <--
// 210409 add -->
	int			GetFolderPathFromBrowser(CString strMsg, CString* pstrFolderPath, BOOL fImport, CString* pstrErrMsg);
// 210409 add <--

	static int CALLBACK SHBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
};
