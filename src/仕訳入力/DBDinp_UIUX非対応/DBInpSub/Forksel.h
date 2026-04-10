// Forksel.h : ヘッダー ファイル
//
#ifndef __FORKSEL_H__
#define __FORKSEL_H__

#define _JOBSEL_MAX 6
//#define _JOBSEL_MAX 8



struct _JOBSEL_PAC {

friend class CForksel;

public:
	_JOBSEL_PAC() { sel_no = -1;}	//デフォルト

protected:
	CString title;			//ダイアログタイトル
	CString itemmsg;		//目的科目等
	int sel_max;			//選択メッセージ数（Ｍａｘ＝５）
	int sel_no;				//デフォルト選択番号（１～５）
	CString sel_msg[_JOBSEL_MAX];		//選択メッセージ（Ｍａｘ＝５）
	CString sel_guide[_JOBSEL_MAX];		//選択の説明（Ｍａｘ＝５）
	int sel_attr[_JOBSEL_MAX];		//選択属性	0 ... 無効表示	1 ... 有効表示
	char sel_txt[_JOBSEL_MAX][50];	//摘要等
	short sw[_JOBSEL_MAX];			//ｓｗ

	// 09.03 /03
	CString guid_msg;

public:
	void set_title( char *txt ) { title = txt; }
	void set_itmmsg( char *txt ) { itemmsg = txt; }
	void set_selmax( int n ) { sel_max = n; }
	void set_selno( int no ) { sel_no = no; } 
	int get_selno() { return sel_no; }
	void set_selmsg( int no, char *sel )
	{
		sel_msg[no] = sel;
		sel_attr[no] = 1;
	}
	CString get_selmsg( int no ) { return sel_msg[ sel_no ]; }
	void set_selguide( int no, char *guide ) { sel_guide[no] = guide; }
	void set_selattr( int no, BOOL attr ) { sel_attr[no] = attr ? 1 : 0; }
	void set_seltxt( int no, char *txt ) { strcpy_s( sel_txt[no], 50, txt ); }
	void get_seltxt( int no, char *txt ) { strcpy_s( txt, 50, sel_txt[no] ); }

	// 09.03 /03
	void set_gdmsg( char* txt ) { guid_msg = txt; }

}; 

/////////////////////////////////////////////////////////////////////////////
// CForksel ダイアログ


class CForksel : public ICSDialog
{
// コンストラクション
public:
	CForksel(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CForksel)
	enum { IDD = IDD_FORKSEL };
	int		m_radio;
	CButton	m_OK;
	CButton	m_Cancel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CForksel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インターフェイス
public:
	void OnInitial( struct _JOBSEL_PAC *sel, int nChar );
//	int get_selno();

// インプリメンテーション
protected:
//	void kakudai(CWnd* wnd);

	struct _JOBSEL_PAC *p_sel;
	int alt_key;
//	int sel_no;

	HBRUSH	m_GuidColor;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CForksel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif	// end of __FORKSEL_H__
