// DataListLine.h : ヘッダー ファイル
//


/////////////////////////////////////////////////////////////////////////////
// CDataListLine ウィンドウ

#define DLNS_LINE1		0
#define DLNS_LINE2		1



class CDataListLine : public CDataListCtrl
{
// コンストラクション
public:
	CDataListLine();
	CDataListLine( int point );

// アトリビュート
public:


// オペレーション
protected:
    // 描画
    virtual void  OnDraw(CDC* pDC);
	virtual int	MakeDataRect( CDC* pDC, int displine );
	virtual CSize GetWidth( CDC* pDC, int id, int line );

	CSize GetWidth( CDC* pDC, int count );


public:
	void EnforceLayoutEX( BOOL bMsgSend = FALSE );

	void HjiyuTitleStr( char* s_str, char* l_str );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDataListLine)
	//}}AFX_VIRTUAL

protected:
	CArray<DT_CLMITEM, DT_CLMITEM&>	m_clmItem;

	int ClmItemArrayMake( int dipsline );

// インプリメンテーション
public:
	virtual ~CDataListLine();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDataListLine)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
