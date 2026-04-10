/*========================================================================
		変動事由入力画面 モジュール
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_HJIYUINP_H
#define	INCLUDE_HJIYUINP_H

// メッセージ
#define WM_HJIYU_INEND		(WM_APP+0x26)	// 付箋入力ダイアログ キー END
#define WM_HJIYU_SEL		(WM_APP+0x27)	// 番号入力時の通知
/*************************************************************************
	WM_HJIYU_INEND
	
	WPARAM	キーコード
	LPARAM	変更HJIYU_DATA のポインタ
	----------------------------------------------------

	WM_HJIYU_SEL

	WPARAM	入力された番号
	(メッセージを受け取ったら WPARAMより変更データを SetHjiyuData()で 返送)
 *************************************************************************/


// 変動事由入力ダイアログの表示位置サイン
#define HJY_TOPLEFT			0	// 指定矩形の上左側に表示
#define HJY_TOPRIGHT		1	// 　　〃　　上右側に表示
#define HJY_BOTTOMLEFT		2	// 　　〃　　下左側に表示
#define HJY_BOTTOMRIGHT		3	// 　　〃　　下右側に表示


typedef struct _HJIYU_DATA
{
	BYTE	hjy_code;
	BYTE	hjy_dmy;
	char	hjy_name[48];
	int		hjy_nbcd;		//内部取引消去[01.25 /12]

	char	hsen_dmy[78-4];
} HJIYU_DATA;


#ifdef _DBINPSUB
class AFX_EXT_CLASS CHjiyuInp
#else
class CHjiyuInp
#endif
{
public:
	CHjiyuInp();
	virtual ~CHjiyuInp();

	int		SetPara( CDBZmSub* pZm, CWnd* parent );
	int		SetHjiyuData( HJIYU_DATA* data, BOOL bUpdate );
	int		DialogON( BOOL bOK, CRect rect, int pos );
	int		DialogOFF();

	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );

	void	set_focus();

	//------------------------------------------------
	//	mode = 0:変動事由、= 1:振替科目、= 2:内部取引消去
	//------------------------------------------------
	void	set_mode( int mode );

private:

	class CHjiyuInpSub* pSub;
};




#endif // INCLUDE_HUSENINP_H