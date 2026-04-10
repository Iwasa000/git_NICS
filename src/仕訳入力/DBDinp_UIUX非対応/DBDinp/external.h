#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__
/*==================================
	data input external
====================================*/

extern int	M_KOJI;
extern BOOL M_MISEIKOJI_TYPE;

extern char	FMT15[];
extern char FMT18[];


extern CDBINPDataRec	ZREC[RECMAX], CREC, SREC;

#ifdef DB_OLD_CLOSE

extern uint KDPAG[8][32];
extern uint	FIXTBL[8];
extern CDBipTKREC	RTKCD[RECMAX*2],TKRECD;
extern int TKSEL_PG[36];

extern char	TKDsw;		/* tekiyo off=0 renso=1,2 siwake=2,1 50on=3 */
extern int		SW_TK50;
extern int		TKDflg, RTpage;
extern int		CDPG,M_ofs,AC_sel;
extern int		KPGcnt;
extern int		T50pag,T50pmax,T50dcnt;

extern int		GCNT;
//extern short	KCD50C[AC_MAX*2];
extern short	KCD50C[AC_MAX*PG_MAX];	//#* 03.08 /02
extern short	KCOD[2];

//extern int		BRNTKY_sel;				// 枝番摘要選択
//extern short	RTblk[2];
//extern short	RTKct[2];

extern short	BMNdsw;	//#* 02.27 /02

#endif

extern class CZVOLUME* Voln1;
extern class CMINFO*   Minfo;

extern unsigned char	SYOG_d[6], SYOG_c[6], SG_def[6];
extern unsigned char	GENK_d[6], GENK_c[6], GK_def[6];

extern char	save_50kana[16];
extern char saveITEM50kana[16];


extern int		REAL_SEQ_NO;

extern int		SEQ_NO,D_SEQN,DENP_NO, START_SEQ, USERS_SEQ;
extern int		SV_BOTOM_SEQ_NO;
extern short	DOPEN_MODE;

extern struct _TKYBRN_SEL TKYBRN_SEL;	// 枝番摘要呼び出し
extern int		BMON_LNG;
extern int		KMKCD_LN;
extern int		BMON_MST;

extern int		SCROLL_NL_;

/*	save variable storage for scan function	*/
extern CDBINPDataRec SCAN_rec; /* correct data save */

#ifdef IMPORT	// 10.14 /98
extern CDBINPDataRec	NXT_rec, NXT_rec2;
extern int NXTDTA_sg;
#endif

	/* 消費税関係ＳＷ． */
extern	char	TKattr, TKattr2;

/*	枝番関係	*/
extern	CDBipEZREC	BREC1, BREC2;
extern char		BRmst;

extern CDipZmSub*	pDBzm;
extern CDBSyohi*	pDBsy;
extern CDocSub*		pDCsb;
extern CDataZeiCalq*	pZeiCl;
extern CZIM_TkjourUtil*	pTkjour;
extern CAddInvData*	pAddInv;


// インボイス対応
extern bool	bInvMaster;		// インボイスバージョンアップマスター
extern bool bInvUse;		// インボイス期間マスター
extern bool	bInvDisp;		// 登録番号表示フラグ


#ifdef NAIBU_VER2
extern CDBNpCal*	pDBnc;
#endif

// イメージマスターオープンサイン
extern int IMG_master;

// スキャンオンリーサイン
extern BOOL bSCAN_ONLY_MODE;
// 確定マスター
extern BOOL bCONFIRM_MASTER;

extern char	Image[];


// 仕訳コピー用選択
extern	CDWordArray	gSelArray;
extern	int			gSelDirection;
extern	int			gSelType;
extern	int			gSelSeq;
extern	BOOL		gSelUpdate;

// 証憑番号使用マスター
extern BOOL bDOCEVI_Master;

// 摘要消費税 保存レコード
extern CDBipTKREC	gTKattrec;

// スキャナ保存のマスターか？
extern BOOL bSCANSAVE_Master;

extern const char* WizTitle;

#endif	// end of __EXTERNAL_H__
