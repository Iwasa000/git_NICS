// TtlDsp.h : ヘッダー ファイル
//
#include "icsocx.h"
#include "etcdef.h"

//{{AFX_INCLUDES()
#include "icslistinp.h"
//}}AFX_INCLUDES
/////////////////////////////////////////////////////////////////////////////
#include "icslistinp.h"

// CTtlDsp ダイアログ
#include "ICSDialog.h"


class CTtlDsp : public ICSDialog
{
// コンストラクション
public:
	CTtlDsp(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTtlDsp)
	enum { IDD = IDD_TTLDSP };
	CICSListInp	m_linp;
	CButton	m_OK;
	CButton	m_Cancel;
	CButton	m_Btn1;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTtlDsp)
	public:
	virtual BOOL PreTranslateMessage(struct tagMSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void set_alt( int alt_msg, char *alt_txt);
	int ttl_dsp( int mode );

protected:
	int m_IDALT;
	char *m_pAltTxt;

	void mk_linedef( int colm_max, LINP_COLUMN *defn, struct _TTLDSP_TBL *tbl  );
	BOOL dsp_ttldsp( int colm_max, LINP_DATA *data, struct _TTLDSP_TBL *tbl );
//	void kakudai(CWnd* wnd);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTtlDsp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnAlt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
