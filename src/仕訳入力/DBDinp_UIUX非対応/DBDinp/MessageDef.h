//
// Dinp.exe Use Message
//
#define WM_STARTFORK	(WM_APP+0x00)	// フォーク開始
#define WM_RETROFOCUS	(WM_APP+0x01)
#define WM_DSPSCANTBL	(WM_APP+0x02)	// スキャンテーブル作成中
#define WM_TAIDSP		(WM_APP+0x03)	// 貸借合計の表示
#define WM_SETPOSITION	(WM_APP+0x04)
#define WM_SENDSEQ		(WM_APP+0x05)

#define WM_CHGTOOLBAR	(WM_APP+0x05)

#define	MESSAGE_DATAMAX	(WM_APP+0x06)
#define	MESSAGE_IMGST_KEY	(WM_APP+0x07)
#define	MESSAGE_IMGST_FOCUS	(WM_APP+0x08)

#ifdef REGDATA	// 07.28 /00
#define WM_REGDATA		(WM_APP+0x0c)
#define defRDT_DIALOG_ON	0
#define defRDT_GET_RGDTA	1

#define defRSL_InizListInp	10
// 05.13 /13
#define defRSL_ListClick	11
#endif

//#ifdef _NEWFOCUS_	// 01.27 /01
#define WM_RETROFOCUS2	(WM_APP+0x0d)
//#endif

#ifdef _ICSPEN_	// 01.20 /01
#define WM_BUNKI	(WM_APP+0x0e)
#endif
#define IDALT	99

#define WM_BKINP_END	(WM_APP+0x0f)

#define WM_CORRECTMSG	(WM_APP+0x10)

// 仕訳入力用 message
#define WM_DBDINPMSG	(WM_APP+0x11)
 #define DWP_LINECOUNT		0	// OnSize での 仕訳行数処理
 #define DWP_COPYMENU		1	// 右クリックコピー処理のメニュー表示
 #define DWP_DISPLINE		2	// 仕訳行の再表示

 #define DWP_SELECTED1		3	// 固定選択欄でのクリック選択
 #define DWP_SELECTED2		4	// 32科目選択欄でのクリック選択

 #define DWP_SETFOCUS		5	// フォーカスセット
 #define DWP_DENPDIALOG		6	// 伝票修正ダイアログ

 #define DWP_TKYSWKDLG		7

#define WM_SELGUIDEMSG	(WM_APP+0x12)


// 伝票入力用 message
#define WM_DENPYOMSG		(WM_APP+0x13)

//コンボボックス選択用
#define	WM_USER_SELCHG_MSG	 (WM_APP+0x14)

