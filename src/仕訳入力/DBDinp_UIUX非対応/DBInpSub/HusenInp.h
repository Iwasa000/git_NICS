/*========================================================================
		付箋入力画面 モジュール
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_HUSENINP_H
#define	INCLUDE_HUSENINP_H

// メッセージ
#define WM_HSINP_INEND		(WM_APP+0x25)	// 付箋入力ダイアログ キー END
/*****************************************************************
	WPARAM	下位 1 = キーによる移動, 0 = combobox focus off
			上位 キーコード
	LPARAM	変更データのHSEN_DATA ポインタ
 *****************************************************************/


// 付箋入力ダイアログの表示位置サイン
#define HSN_TOPLEFT			0	// 指定矩形の上左側に表示
#define HSN_TOPRIGHT		1	// 　　〃　　上右側に表示
#define HSN_BOTTOMLEFT		2	// 　　〃　　下左側に表示
#define HSN_BOTTOMRIGHT		3	// 　　〃　　下右側に表示

// 付箋入力ダイアログ内のコントロール表示ポジション
#define HSN_HSEL_PN			0
#define HSN_COMMENT_PN		1


typedef struct _HSEN_DATA
{
	int		hsen_inpsg;
	int		hsen_sel;
	char	hsen_cmt[42];

	char	hsen_dmy[42-12];
} HSEN_DATA;



#ifdef _DBINPSUB
class AFX_EXT_CLASS CHusenInp
#else
class CHusenInp
#endif
{
public:
	CHusenInp();
	virtual ~CHusenInp();

	int		SetPara( CDBZmSub* pZm, CWnd* parent );
	void	SetHusenData( HSEN_DATA* cdata, HSEN_DATA* sdata );
	int		DialogON( BOOL bOK, CRect rect, int sypos, int pn );
	int		DialogOFF();

	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );

	void	set_nextpos();
	void	set_nowfocus();

private:

	class CHusenInpSub* pSub;
};




#endif // INCLUDE_HUSENINP_H