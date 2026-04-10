// ProgDlg.h : ヘッダ ファイル
// CG: このファイルは「プログレス ダイアログ」コンポーネントにより追加されています。

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg ダイアログ

#ifndef __MSGDLG_H__
#define __MSGDLG_H__





class CMsgDlg : public CDialog
{
// 構築 / 破棄
public:
    CMsgDlg(UINT nCaptionID = 0);   // 標準コンストラクタ
    ~CMsgDlg();

    BOOL Create(CWnd *pParent=NULL);

    // キャンセル ボタンのチェック
    BOOL CheckCancelButton();

	void SetResizeRate(double x, double y) { m_Xrate = x; m_Yrate = y; };
	void GetResizeRate(double& x, double& y) { x = m_Xrate; y = m_Yrate; };

	int CaptionChange( int captionID );
	void HideCancelButton();

// ダイアログ データ
    //{{AFX_DATA(CMsgDlg)
    enum { IDD = IDD_MSG_DLG };
	CButton	m_Cancel;
    //}}AFX_DATA

// オーバーライド
    // ClassWizard は、この位置に仮想関数を生成してオーバーライドします。
    //{{AFX_VIRTUAL(CMsgDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
	UINT m_nCaptionID;
    
    BOOL m_bCancel;

    BOOL m_bParentDisabled;

	double m_Xrate, m_Yrate;
	BOOL m_bHideCancel;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 

    void PumpMessages();

    // 生成されたメッセージマップ関数
    //{{AFX_MSG(CMsgDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __MSGDLG_H__
