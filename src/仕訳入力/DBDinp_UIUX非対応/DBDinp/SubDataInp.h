#ifndef INCLUDE_SUBDATAINP_H
#define INCLUDE_SUBDATAINP_H



// メッセージ
#define WM_SUBDATA_INEND		(WM_APP+0x28)	// サブ入力ダイアログ キー END
/*****************************************************************
	WPARAM	下位 1 = キーによる移動, 0 = focus off
			上位 キーコード
	LPARAM	変更データのCDBINPDataRec ポインタ
 *****************************************************************/


// サブ入力ダイアログの表示位置サイン
#define SDT_TOPLEFT			0	// 指定矩形の上左側に表示
#define SDT_TOPRIGHT		1	// 　　〃　　上右側に表示
#define SDT_BOTTOMLEFT		2	// 　　〃　　下左側に表示
#define SDT_BOTTOMRIGHT		3	// 　　〃　　下右側に表示

// サブ入力ダイアログ内のコントロール表示ポジション
#define SD_TGNO_PN		0
#define SD_TGDATE_PN	1
#define SD_HUSEN_PN		2



class CSubDataInp
{
public:
	CSubDataInp();
	virtual ~CSubDataInp();

	int		SetPara( CDBZmSub* pZm, CWnd* parent );
	int		SetCurData( BOOL bMdfyOK, CDBINPDataRec* cdata );
	int		DialogON( CRect rect, int pos, int pn );
	int		DialogOFF();

	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );

	void	set_focus();
	void	F2Job();

private:
	class CSubDataSub* pSub;

};






#endif //INCLUDE_SUBDATAINP_H