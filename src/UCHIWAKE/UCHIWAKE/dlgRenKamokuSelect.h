#pragma once
#include "icsbutton.h"
#include "dbUcLstKamoku.h"
#include "UcFunctionCommon.h"
#include "afxwin.h"
#include "icsknjedit.h"

// CdlgRenKamokuSelect ダイアログ

class CdlgRenKamokuSelect : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgRenKamokuSelect)

public:
	CdlgRenKamokuSelect(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgRenKamokuSelect();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_RENKAMOKUSELECT };
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	CButton m_rdoSelect;
	CButton m_rdoAdd;
	CComboBox m_cboKamoku;
	CICSKNJEdit m_txtKamoku;


public:
	int m_KnSeq;	// uc_lst_Kamokuシーケンス番号
	BYTE m_UcKmkEntry;	// 新規内訳書科目追加確認フラグ

private:
	CDatabase*	m_pDB;				// データベースハンドル
	CUcFunctionCommon	m_clsFunc;	//	共通関数クラス
	int			m_nFormSeq;			// 様式シーケンス番号
	int			m_nRet;				// 戻り値
	CString		m_Knnam;			// 財務科目名称
	CString		m_Knkana;			// 財務科目カナ
// 158016 add -->
	CString		m_Kncod;			// 財務科目コード
	CDBZmSub*	m_pZmSub;			// 財務DBクラスハンドル
	int KamokuCheck142( void );
// 158016 add <--

public:
	void SetKamokuData();
// 158016 del -->
	//int ShowDialog( CDatabase* pDB , int nFormSeq , CString Knnam, CString Knkana );
// 158016 del <--
// 158016 add -->
	int ShowDialog( CDatabase* pDB , int nFormSeq , CString Knnam, CString Knkana, CString Kncod );
// 158016 add <--
	int GetKamokuData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedRadioAdd();
	afx_msg void OnBnClickedRadioSelect();

protected:
	virtual void OnOK();
};
