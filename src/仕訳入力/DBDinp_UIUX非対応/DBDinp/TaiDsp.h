// TaiTtl.h : ヘッダー ファイル
//

#include "TaiData.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiDsp ダイアログ

//#include "ICSDialog.h"
#include "ICSCombobox.h"
#include "icslistinp.h"

class CTaiDsp : public ICSDialog
{
// コンストラクション
public:
	CTaiDsp(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTaiDsp)
	enum { IDD = IDD_TAIDSP };
	ICSComboBox	m_KmCbo;
	CButton	m_OK;
	CButton	m_Btn1;
	CICSListInp	m_List;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTaiDsp)
	public:
	virtual int DoModal();
	virtual BOOL PreTranslateMessage(struct tagMSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	BOOL m_Active;

	void tai_dsp( int mode );
	void set_alt( int alt_msg = 0, char *alt_txt = 0 );
	void dsp_data( STaiData *tdata );

protected:
	int m_Mode;
	int m_IDALT;
	char *m_pAltTxt;
	STaiData *m_pTaiData, *m_pKmkDspData;

//	void kakudai(CWnd* wnd);

	void Kari_Kasi_Dsp( STaiData *tdata, int index );
	
	void MakeEdaListInp();
	void brn_disp( STaiData *tdata, int index );

protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTaiDsp)
	virtual void OnCancel();
	afx_msg void OnAlt();
	virtual void OnOK();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeKmCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT TaiDSP( WPARAM wParam, LPARAM lParam);

};
