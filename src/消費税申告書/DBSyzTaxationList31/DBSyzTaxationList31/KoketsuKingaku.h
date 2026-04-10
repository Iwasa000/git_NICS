#pragma once

// KoketsuKingaku ダイアログ

class CKoketsuKingaku : public ICSDialog
{
	DECLARE_DYNAMIC(CKoketsuKingaku)

public:
	CKoketsuKingaku(CDBZmSub *pZmSub, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CKoketsuKingaku();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KOKETSUKINGAKU };

private:
	CDBZmSub *m_pZmSub;
	int m_chkIdx;

public:
	// 選択された業種を取得
	void GetKoketsuKingauItem( int &idx );

private:
	// 初期化
	void Init();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_IppanCheck;
	CButton m_FudousanCheck;
	CButton m_NougyouCheck;
	afx_msg void OnBnClickedOk();
};
