/*========================================================================
		枝番摘要表示関係 
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_BRNTKY_DEF_H
#define INCLUDE_BRNTKY_DEF_H


#define	SYOGT	"00000101"		// ｢諸口｣内部コード
//#ifdef SCHOOL	// 05.25 /00
//#define GNKIN	((Voln1->apno == 0x40||Voln1->apno == 0x52) ? 0x1131 : 0x1111 )	// ｢現金｣内部コード 12.07 /01
//#else
//#define GNKIN	"01010101"		// ｢現金｣内部コード
//#endif

// 枝番摘要モード
#define BT_NOMALBRN_TEK 0x01		// 枝番摘要
#define BT_SYOGTBRN_TEK 0x02		// 資金繰諸口枝番


typedef struct tag_BRN_NAME {
	CString Name;
	OLE_COLOR ForeColor;
} BRN_NAME;


typedef struct tag_BRN_REC {
	int			 	brn;	// 枝番
	int				tkno;	// 摘要レコード番号( 0の時は 枝番残高の名称を使用している )
	CString			kn50;	// 50音
	CString			brmsg;	// 名称
} BRN_REC;


typedef struct tag_BRN_TB {
	int				bmn;		// 部門コード
	CString			code;		// 科目コード
	CString			brn_kn50;	// 50音検索サイン
	OLE_COLOR		ForeCol;	// 表示文字色
	BOOL			full;		// 名称の表示タイプ
								//		TRUE ... "科目.摘要名称"
								//		FALSE .. "摘要名称"
	unsigned short	page_max;	// 最大ページ
	unsigned short	page_now;	// 現在ページ
	unsigned short	page_recn;	// 1ページ分のレコード数
	unsigned short	rec_n;		// 現在ページのレコード数
	tag_BRN_REC *pBRN_REC;

} BRN_TB;

__declspec(dllexport) BOOL& ezquery_flag();

__declspec(dllexport) void ezdb_init( CDBZmSub* pZm );


#endif // INCLUDE_BRNTKY_DEF_H