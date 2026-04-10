// DBSbilldata.h

#ifndef  __ICS_DBSBILLDATA__
#define  __ICS_DBSBILLDATA__

//////////////////////////////////////
// 消費税及び地方消費税の更正の請求 //
//			08'10.09	作成		//
//////////////////////////////////////

// 管理部
typedef struct _KSIHEAD{
	BYTE	KHver,			// ファイルバージョン
			KHflg;			// D0 ON:個人用 OFF:法人用
	char	KHdmy1[14];		// リザーブ

	int		KHdofs,			// データのオフセット
			KHdlng;			// データのレングス
	char	KHdmy2[8];		// リザーブ
	char	KHdmy3[224];	// リザーブ
} KSIHEAD;		// 256b

typedef struct _KSVAL{
	WORD	KVvflg;			// 実額サイン　D0:KVsval D1:KVkval
	char	KVsval[6];		// この請求前の金額(確定額)
	char	KVkval[6];		// 更正の請求金額(正当とする額)
	char	KVdmy1[2];		// リザーブ
} KSIVAL;		// 16b

// データ部
typedef struct _KSIDATA{
	BYTE	KSdata[4];		// 提出年月日		(平成固定)
	BYTE	KSktdt[4];		// 更正決定通知書日付		(平成固定)
	WORD	KSktsw;			// 0=申告、1=更正、2=決定
	BYTE	KSstdt[4],		// 修正申告提出年月日(又は更正決定通知書受理年月日)	(平成固定)
			KSjrdt[4];		// 更正決定通知書受理年月日	(平成固定)
	char	KSreas[320],	// 更正の請求をする理由等(40*3)
			KSdocu[64];		// 添付書類(15*2)
	char	KSdmy1[110];	// リザーブ

	KSIVAL	KSval[20];		// 金額
	char	KSdmy2[192];	// リザーブ

	WORD	KSbpsw;			// 0=銀行、1=郵便局
	struct	{		// 銀行等(128byte)
		char	Bname[32];		// 銀行等名称
		WORD	Bkbn;			// 銀行等区分(0:銀行/1:金庫/2:組合/3:漁協/4:農協)
		char	Sname[32];		// 本支店名称
		WORD	Skbn;			// 本支店区分(1:本店/2:支店/3:本所/4:支所)
		char	Dname[24],		// 預金
				Accnt[16];		// 口座番号
		char	Bdmy[20];
	} KSbank;
	struct	{	// 郵便局(88byte)
		char	Pname[46],		// 郵便局名称
				Acnt1[10],		// 口座番号１
				Acnt2[16];		// 口座番号２
		char	Pdmy[16];
	} KSpost;
	char	KSdmy3[38];		// リザーブ
/////////[09'12.03]	
//	char	KSdmy4[512];	// リザーブ
///////////////////
	WORD	KSthsw;			// 適用法令の区分
	WORD	KSkksw;			// 確定額区分
	char	KSdmy4[508];	// リザーブ
///////////////////

} KSIDATA;

#endif