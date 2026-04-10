#pragma once


// CDupliDlg ダイアログ

class CDupliDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CDupliDlg)

public:
	CDupliDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDupliDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DUPLI };
#endif

	int	m_dwChkBit; // D0:伝票番号, D1:部門, D2:工事, D3:摘要

	int koji_mast, bmon_mast;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
