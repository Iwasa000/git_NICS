#pragma once


// CChkMsg ダイアログ

class CChkMsg : public ICSDialog
{
	DECLARE_DYNAMIC(CChkMsg)

public:
	CChkMsg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CChkMsg();

	CSnHeadData**	m_pSnHeadData;	// 消費税ヘッダ情報
	short			m_ChkState;		// 振替納税を引き続き希望する  0:しない / 1:する
	int				m_MsgSign;		// メッセージの種別　0:振替継続希望 / 1:公金受取口座の利用

	// データセット
	void	InitInfo( CSnHeadData** pSnHeadData, short ChkState, int MsgSign );

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHKMSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIcsdiagctrl m_diag;
};
