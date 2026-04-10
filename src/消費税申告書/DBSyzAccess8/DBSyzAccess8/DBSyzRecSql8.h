#ifndef  __DBSYZACC_ICS_DBSYZACCSQL__
#define  __DBSYZACC_ICS_DBSYZACCSQL__

//-----------------------------------------------------------------------------
// SQL 定義
//-----------------------------------------------------------------------------

// 納付書（領収済通知書）
// テーブル名
static	char*	SHINPAYMENT_TBL			=	_T("shinpayment");
// テーブル作成
static	char	*SHINPAYMENT_SQL		=	_T("CREATE TABLE [dbo].[%s] (\n"
											"[NFItem] [int] NOT NULL, \n"
											"[NFLine] [int] NOT NULL, \n"
											"[NFResv] [int] NOT NULL, \n"
											"[NFSequ] [int] NOT NULL, \n"
											"[NFKskb] [tinyint] NULL, \n"	
											"[NFKsdm] [tinyint] NULL, \n"	
											"[NFStnm] [varchar](50) NULL, \n"
											"[NFJflg] [int] NULL, \n"	
											"[NFHval] [decimal](14, 0)	NULL, \n"
											"[NFJval] [decimal](14, 0)	NULL, \n"
											"[NFKval] [decimal](14, 0)	NULL, \n"
											"[NFRval] [decimal](14, 0)	NULL, \n"
											"[NFEval] [decimal](14, 0)	NULL, \n"
											"[NFGval] [decimal](14, 0)	NULL, \n"
											"[NFHjvl] [decimal](14, 0)	NULL, \n"
											"[NFVal1] [decimal](14, 0)	NULL, \n"
											"[NFVal2] [decimal](14, 0)	NULL, \n"
											"[NFSign] [binary](128) NULL \n"
											") ON [PRIMARY]\n");

// 消費税の還付申告に関する明細書（基本情報）
// テーブル名
static	char	*REFUNDVOL_TBL		=	_T("refundvol");
// テーブル作成
static	char	*REFUNDVOL_SQL		=	_T("CREATE TABLE [dbo].[%s] (\n"
											"[KPItem] [int] NOT NULL, \n"
											"[KPLine] [int] NOT NULL, \n"
											"[KPResv] [int] NOT NULL, \n"
											"[KPSequ] [int] NOT NULL, \n"
											"[KPType] [int] NOT NULL, \n"
											"[KPRsgn] [tinyint] NULL, \n"	
											"[KPRsdm] [tinyint] NULL, \n"	
											"[KPRrup] [varchar](50) NULL, \n"
											"[KPRrlw] [varchar](50) NULL, \n"
											"[KPRrdm] [varchar](50) NULL, \n"
											"[KPKuck] [tinyint] NULL, \n"	
											"[KPKusg] [tinyint] NULL, \n"	
											"[KPEbnk] [varchar](20) NULL, \n"
											"[KPEbsg] [tinyint] NULL, \n"	
											"[KPEbdm] [tinyint] NULL, \n"	
											"[KPEstn] [varchar](20) NULL, \n"
											"[KPEssg] [tinyint] NULL, \n"	
											"[KPEsdm] [tinyint] NULL, \n"	
											"[KPEykn] [varchar](10) NULL, \n"
											"[KPEkoz] [varchar](12) NULL, \n"
											"[KPEyn1] [varchar](10) NULL, \n"
											"[KPEyn2] [varchar](10) NULL, \n"
											"[KPEsup] [varchar](50) NULL, \n"
											"[KPEslw] [varchar](50) NULL, \n"
											"[KPEaup] [varchar](50) NULL, \n"
											"[KPEalw] [varchar](50) NULL, \n"
											"[KPSsgn] [tinyint] NULL, \n"	
											"[KPSsta] [tinyint] NULL, \n"	
											"[KPSnam] [varchar](20) NULL, \n"
											"[KPTsck] [tinyint] NULL, \n"	
											"[KPTsgn] [tinyint] NULL, \n"	
											"[KPKsck] [tinyint] NULL, \n"	
											"[KPKsgn] [tinyint] NULL, \n"	
											"[KPSnen] [tinyint] NULL, \n"	
											"[KPSndm] [tinyint] NULL, \n"	
											"[KPSpn1] [varchar](128) NULL, \n"
											"[KPSpn2] [varchar](128) NULL, \n"
											"[KPSpn3] [varchar](128) NULL, \n"
											"[KPSpn4] [varchar](128) NULL, \n"
											"[KPSpn5] [varchar](128) NULL, \n"
											"[KPSpn6] [varchar](128) NULL \n"
										  ") ON [PRIMARY]\n");

// 消費税の還付申告に関する明細書（次葉）
// テーブル名
static	char	*REFUNDREC_TBL		=	_T("refundrec");
// テーブル作成
static	char	*REFUNDREC_SQL		=	_T("CREATE TABLE [dbo].[%s] (\n"
											"[KPItem] [int] NOT NULL, \n"
											"[KPLine] [int] NOT NULL, \n"
											"[KPResv] [int] NOT NULL, \n"
											"[KPSequ] [int] NOT NULL, \n"
											"[KPSvl1] [decimal](14, 0)	NULL, \n"
											"[KPSvl2] [decimal](14, 0)	NULL, \n"
											"[KPSvl3] [decimal](14, 0)	NULL, \n"
											"[KPSvl4] [decimal](14, 0)	NULL, \n"
											"[KPSsup] [varchar](20) NULL, \n"
											"[KPSslw] [varchar](20) NULL, \n"
											"[KPSday] [int] NULL, \n"
											"[KPSdys] [tinyint] NULL, \n"	
											"[KPSddm] [tinyint] NULL, \n"	
											"[KPSval] [decimal](14, 0)	NULL, \n"
											"[KPSnup] [varchar](30) NULL, \n"
											"[KPSnlw] [varchar](30) NULL, \n"
											"[KPSaup] [varchar](40) NULL, \n"
											"[KPSalw] [varchar](40) NULL, \n"
											"[KPStup] [varchar](30) NULL, \n"
											"[KPStlw] [varchar](30) NULL, \n"
											"[KPSzup] [varchar](30) NULL, \n"
											"[KPSzlw] [varchar](30) NULL, \n"
											"[KPInvno] [varchar](14) NULL \n"	// 20230704 インボイス対応
										   ") ON [PRIMARY]\n");

// 20230704 インボイス対応
static char		*VUP1_REFUNDREC_SQL	=	_T("IF OBJECTPROPERTY ( object_id('refundrec'),'ISTABLE') IS NOT NULL \n"
											"ALTER TABLE [dbo].[refundrec] \n"
											"ADD [KPInvno] [varchar](14) NULL \n");

// 拡張カラム名 20230704
static char		*ADDREFUNDRECCOLUMN	=	_T("KPInvno");


// 仕入控除税額に関する明細書（次葉）
// テーブル名
static	char*	SKJREC_TBL			=	_T("skjrec");
// テーブル作成
static	char*	SKJREC_SQL			=	_T("CREATE TABLE [dbo].[%s] (\n"
											"[SkjType] [int] NOT NULL, \n"
											"[SkjLine] [int] NULL, \n"	
											"[SkjColm] [int] NULL, \n"	
											"[SkjSaveDay] [int] NULL, \n"
											"[AsKndUpBuf] [varchar](20) NULL, \n"
											"[AsKndLwBuf] [varchar](20) NULL, \n"
											"[SyutokuDay] [int] NULL, \n"
											"[TrMny] [decimal](14, 0) NULL, \n"
											"[NameUpBuf] [varchar](30) NULL, \n"
											"[NameLwBuf] [varchar](30) NULL, \n"
											"[AdrUpBuf] [varchar](40) NULL, \n"
											"[AdrLwBuf] [varchar](40) NULL \n"
											") ON [PRIMARY]\n");

// 更正の請求
// テーブル名
static	char*	REASK_TBL			=	_T("reask");
// 電子申告テーブル作成
static	char*	REASK_SQL			=	_T("CREATE TABLE [dbo].[%s] (\n"
											"[KStype] [int] NOT NULL, \n"
											"[KSdata] [int] NULL, \n"	
											"[KSktdt] [int] NULL, \n"	
											"[KSktsw] [int] NULL, \n"
											"[KSstdt] [int] NULL, \n"
											"[KSjrdt] [int] NULL, \n"	
											"[KSreas1] [varchar](90) NULL, \n"
											"[KSreas2] [varchar](90) NULL, \n"
											"[KSreas3] [varchar](90) NULL, \n"
											"[KSreas4] [varchar](90) NULL, \n"
											"[KSdocu1] [varchar](40) NULL, \n"
											"[KSdocu2] [varchar](40) NULL, \n"
											"[KSdocu3] [varchar](40) NULL, \n"
											"[KVvflg1] [int] NULL, \n"
											"[KVsval1] [decimal](14, 0)	NULL, \n"
											"[KVkval1] [decimal](14, 0)	NULL, \n"
											"[KVvflg2] [int] NULL, \n"
											"[KVsval2] [decimal](14, 0)	NULL, \n"
											"[KVkval2] [decimal](14, 0)	NULL, \n"
											"[KVvflg3] [int] NULL, \n"
											"[KVsval3] [decimal](14, 0)	NULL, \n"
											"[KVkval3] [decimal](14, 0)	NULL, \n"
											"[KVvflg4] [int] NULL, \n"
											"[KVsval4] [decimal](14, 0)	NULL, \n"
											"[KVkval4] [decimal](14, 0)	NULL, \n"
											"[KVvflg5] [int] NULL, \n"
											"[KVsval5] [decimal](14, 0)	NULL, \n"
											"[KVkval5] [decimal](14, 0)	NULL, \n"
											"[KVvflg6] [int] NULL, \n"
											"[KVsval6] [decimal](14, 0)	NULL, \n"
											"[KVkval6] [decimal](14, 0)	NULL, \n"
											"[KVvflg7] [int] NULL, \n"
											"[KVsval7] [decimal](14, 0)	NULL, \n"
											"[KVkval7] [decimal](14, 0)	NULL, \n"
											"[KVvflg8] [int] NULL, \n"
											"[KVsval8] [decimal](14, 0)	NULL, \n"
											"[KVkval8] [decimal](14, 0)	NULL, \n"
											"[KVvflg9] [int] NULL, \n"
											"[KVsval9] [decimal](14, 0)	NULL, \n"
											"[KVkval9] [decimal](14, 0)	NULL, \n"
											"[KVvflg10] [int] NULL, \n"
											"[KVsval10] [decimal](14, 0)	NULL, \n"
											"[KVkval10] [decimal](14, 0)	NULL, \n"
											"[KVvflg11] [int] NULL, \n"
											"[KVsval11] [decimal](14, 0)	NULL, \n"
											"[KVkval11] [decimal](14, 0)	NULL, \n"
											"[KVvflg12] [int] NULL, \n"
											"[KVsval12] [decimal](14, 0)	NULL, \n"
											"[KVkval12] [decimal](14, 0)	NULL, \n"
											"[KVvflg13] [int] NULL, \n"
											"[KVsval13] [decimal](14, 0)	NULL, \n"
											"[KVkval13] [decimal](14, 0)	NULL, \n"
											"[KVvflg14] [int] NULL, \n"
											"[KVsval14] [decimal](14, 0)	NULL, \n"
											"[KVkval14] [decimal](14, 0)	NULL, \n"
											"[KVvflg15] [int] NULL, \n"
											"[KVsval15] [decimal](14, 0)	NULL, \n"
											"[KVkval15] [decimal](14, 0)	NULL, \n"
											"[KVvflg16] [int] NULL, \n"
											"[KVsval16] [decimal](14, 0)	NULL, \n"
											"[KVkval16] [decimal](14, 0)	NULL, \n"
											"[KVvflg17] [int] NULL, \n"
											"[KVsval17] [decimal](14, 0)	NULL, \n"
											"[KVkval17] [decimal](14, 0)	NULL, \n"
											"[KVvflg18] [int] NULL, \n"
											"[KVsval18] [decimal](14, 0)	NULL, \n"
											"[KVkval18] [decimal](14, 0)	NULL, \n"
											"[KVvflg19] [int] NULL, \n"
											"[KVsval19] [decimal](14, 0)	NULL, \n"
											"[KVkval19] [decimal](14, 0)	NULL, \n"
											"[KVvflg20] [int] NULL, \n"
											"[KVsval20] [decimal](14, 0)	NULL, \n"
											"[KVkval20] [decimal](14, 0)	NULL, \n"	
											"[Bname] [varchar](40)	NULL, \n"
											"[Bkbn] [int] NULL, \n"	
											"[Sname] [varchar](40)	NULL, \n"
											"[Skbn] [int] NULL, \n"	
											"[Dname] [varchar](30)	NULL, \n"
											"[Accnt] [varchar](20)	NULL, \n"
											"[Pname] [varchar](50)	NULL, \n"
											"[Acnt1] [varchar](20)	NULL, \n"
											"[Acnt2] [varchar](20)	NULL, \n"
											"[KSbpsw] [int] NULL, \n"
											"[KSthsw] [int] NULL, \n"
											"[KSkksw] [int] NULL, \n"
											"[Kikakutei1] [decimal](14, 0)	NULL, \n"	// 更正の請求/新様式対応
											"[Kikakutei2] [decimal](14, 0)	NULL \n"	
											") ON [PRIMARY]\n");

// 更正の請求/新様式対応
static char		*VUP1_REASK_SQL		=	_T("IF OBJECTPROPERTY ( object_id('reask'),'ISTABLE') IS NOT NULL \n"
											"ALTER TABLE [dbo].[reask] \n"
											"ADD [Kikakutei1] [decimal](14, 0) NULL \n"
											",[Kikakutei2] [decimal](14, 0) NULL \n");

// 拡張カラム名
static char		*ADDREASKCOLUMN		=	_T( "Kikakutei1" );

// 課税取引金額計算表
// テーブル名
static	char*	KJSNK_TBL			=	_T("kjsnk");
// テーブル作成
static	char*	KJSNK_SQL			=	_T("CREATE TABLE [dbo].[%s] (\n"
											"[kslist] [int] NOT NULL, \n"
											"[ksline] [int] NOT NULL, \n"
											"[ksitem] [int] NOT NULL, \n"
											"[kssgn1] [tinyint] NOT NULL, \n"
											"[kssgn2] [tinyint] NOT NULL, \n"
											"[kssgn3] [tinyint] NOT NULL, \n"
											"[kssgn4] [tinyint] NOT NULL, \n"
											"[kssgn5] [tinyint] NOT NULL, \n"
											"[ksname] [varchar](40) NULL, \n"
											"[ksval0] [decimal](14, 0) NULL, \n"
											"[ksval1] [decimal](14, 0) NULL, \n"
											"[ksval2] [decimal](14, 0) NULL, \n"
											"[ksval3] [decimal](14, 0) NULL, \n"
											"[ksval4] [decimal](14, 0) NULL, \n"
											"[ksval5] [decimal](14, 0) NULL \n"
											") ON [PRIMARY]\n");

// テーブル存在確認
static	char	*ISEXIST_SQL			=	_T("SELECT COUNT(*) FROM sysobjects where id = object_id('%s')");

// バージョン付加
static	char	*SET_VERSION_SQL		=	_T("EXEC sys.sp_addextendedproperty @name=N'version', @value=N'%d' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'%s'\n");

// バージョン取得
static	char	*GET_VERSION_SQL		=	_T("SELECT name, value FROM fn_listextendedproperty( NULL, 'schema', 'dbo', 'table', '%s', NULL, default )\n");

// バージョン更新
static	char	*UPD_SVER_SQL			=	_T("EXEC sp_updateextendedproperty \n"
											   "@name = N'version', @value = '%d', \n"
											   "@level0type = N'Schema', @level0name = 'dbo', \n"
											   "@level1type = N'Table', @level1name = '%s', \n"
											   "@level2type = NULL, @level2name = default; \n");


// 消費税申告書（金額）	('14.01.06)
// テーブル名
static	char	*SH_LST_VAL_TBL			=	_T("SYZ_SNK_LST_VAL");
// テーブルソート
static	char	*SH_LST_VAL_SORT		=	_T("sn_seq, hyoId, itmId");
// テーブル作成
static	char	*MK_SH_LST_VAL_SQL		=	_T("CREATE TABLE [dbo].[%s] (\n"
												"[sn_seq] [int] NOT NULL, \n"
												"[version] [int] NOT NULL, \n"
												"[hyoId] [varchar](8) NOT NULL, \n"
												"[itmId] [varchar](8) NOT NULL, \n"
												"[val] [decimal](14, 0) NULL, \n"
												//"[sign1] [int] NULL \n"				// 230718Aw del
												"[sign1] [int] NULL, \n"				// 230718Aw add
												"[ksname] [varchar](40) NULL, \n"		// 230718Aw add
												"[kssgn1] [tinyint] NULL \n"			// 230718Aw add
												") ON [PRIMARY]\n");

// 消費税申告書（割合）	('14.01.06)
// テーブル名
static	char	*SH_LST_RATE_TBL		=	_T("SYZ_SNK_LST_RATE");
// テーブルソート
static	char	*SH_LST_RATE_SORT		=	_T("sn_seq, hyoId, itmId");
// テーブル作成
static	char	*MK_SH_LST_RATE_SQL		=	_T("CREATE TABLE [dbo].[%s] (\n"
												"[sn_seq] [int] NOT NULL, \n"
												"[version] [int] NOT NULL, \n"
												"[hyoId] [varchar](8) NOT NULL, \n"
												"[itmId] [varchar](8) NOT NULL, \n"
												"[rate] [int] NULL, \n"
												"[sign1] [int] NULL \n"
												") ON [PRIMARY]\n");

//2018.03.13 INSERT START
// テーブル名
static char		*FHYO60REC_TBL			=	_T("fhyo60rec");
// テーブル作成
static char		*FHYO60REC_SQL			=	_T("CREATE TABLE [dbo].[%s] (\n"
											   "[Sn_SEQ] [int] NOT NULL, \n"
											   "[Sn_6FSDEATH] [int] NULL,\n"
											   "[Sn_6FSDAIHYO] [varchar](45) NULL,\n"
											   "[Sn_6FSLIMIT] [int] NULL,\n"
											   "[Sn_6BS_USE] [int] NOT NULL, \n"				// '20.05.11：法改正により追加（バージョン２）
											   "[Sn_6BS_ADR] [varchar](80) NULL, \n"
											   "[Sn_6BS_TELNO] [varchar](18) NULL, \n"
										       "[Sn_6BS_NMFRI] [varchar](80) NULL, \n"
											   "[Sn_6BS_NAME] [varchar](40) NULL, \n"
											   "[Sn_6BS_SONOTA] [varchar](80) NULL) \n");

// テーブルバージョンアップ（１→２）
static char		*VUP1_FHYO60REC_SQL = _T("IF OBJECTPROPERTY ( object_id('fhyo60rec'),'ISTABLE') IS NOT NULL \n"
											"ALTER TABLE [dbo].[fhyo60rec] \n"
											"ADD [Sn_6BS_USE] [int] NOT NULL default 0 \n"
											",[Sn_6BS_ADR] [varchar](80) NULL \n"
											",[Sn_6BS_TELNO] [varchar](18) NULL \n"
											",[Sn_6BS_NMFRI] [varchar](80) NULL \n"
											",[Sn_6BS_NAME] [varchar](40) NULL \n"
											",[Sn_6BS_SONOTA] [varchar](80) NULL \n");

// テーブル名
static char		*SYZ_SOZOKU_TBL         =	_T("syz_sozoku");
// テーブル作成
static char		*SYZ_SOZOKU_SQL			=	_T("CREATE TABLE [dbo].[%s] (\n"
												" [Sn_SEQ] [int] NOT NULL,\n"
												" 	[Sn_NO] [int] NOT NULL,\n"
												" 	[Sn_6FSJUSYO] [varchar](90) NULL,\n"
												" 	[Sn_6FSFURI] [varchar](90) NULL,\n"
												" 	[Sn_6FSNAME] [varchar](45) NULL,\n"
												" 	[Sn_6FSHASH] [char](32) NULL,\n"
												" 	[Sn_6FSSYOKU] [varchar](40) NULL,\n"
												" 	[Sn_6FSTSUDUKI] [varchar](15) NULL,\n"
												" 	[Sn_6FSBIRTH] [int] NULL,\n"
												" 	[Sn_6FSTEL1] [varchar](18) NULL,\n"
												" 	[Sn_6FSTEL2] [varchar](18) NULL,\n"
												" 	[Sn_6FSTEL3] [varchar](18) NULL,\n"
												" 	[Sn_6FSHOTEI] [tinyint] NULL,\n"
												" 	[Sn_6FSBUNSHI] [int] NULL,\n"
												" 	[Sn_6FSBUNBO] [int] NULL,\n"
												" 	[Sn_6FSKAGAKU] [decimal](14, 0) NULL,\n"
												" 	[Sn_6FSNOFUZEI] [decimal](14, 0) NULL,\n"
												" 	[Sn_6FSNOFUCHIHO] [decimal](14, 0) NULL,\n"
												" 	[Sn_6FSKANPUZEI] [decimal](14, 0) NULL,\n"
												" 	[Sn_6FSKANPUCHIHO] [decimal](14, 0) NULL,\n"
												" 	[Sn_6FSNOFUKANPUsw] [int] NULL,\n"
												" 	[Sn_6FSGNAME] [varchar](20) NULL,\n"
												" 	[Sn_6FSGSEL] [tinyint] NOT NULL,\n"
												" 	[Sn_6FSSNAME] [varchar](20) NULL,\n"
												" 	[Sn_6FSSSEL] [tinyint] NOT NULL,\n"
												" 	[Sn_6FSYOKIN] [varchar](8) NULL,\n"
												" 	[Sn_6FSKOUZA] [varchar](10) NULL,\n"
												" 	[Sn_6FSKIGO1] [varchar](5) NULL,\n"
												" 	[Sn_6FSKIGO2] [varchar](8) NULL,\n"
												" 	[Sn_6FSYUBIN] [varchar](16) NULL,\n"
												"	PRIMARY KEY([Sn_SEQ],[Sn_NO]))\n");
//2018.03.13 INSERT END

// テーブル名
static char		*SYZ_TOKUREI_TBL        =   _T("syz_tokurei"); 
// テーブルソート
static	char	*SYZ_TOKUREI_VAL_SORT   =   _T("sn_seq, hyoId, itmId, idx");
// テーブル作成
static char		*SYZ_TOKUREI_SQL		=	_T("CREATE TABLE [dbo].[%s] (\n"
												"[sn_seq] [int] NOT NULL,\n"
												"[version] [int] NOT NULL,\n"
                                                "[hyoId] [varchar](8) NOT NULL, \n"
                                                "[itmId] [varchar](8) NOT NULL, \n"
	                                            "[idx] [int] NOT NULL, \n"
                                                "[kind] [int] NOT NULL, \n"
	                                            "[val] [decimal](14, 0) NULL, \n"
	                                            "[rate] [int] NULL, \n"
	                                            "[str] [varchar](32) NULL, \n"
	                                            "[sign1] [int] NULL \n"
                                               	") ON [PRIMARY]\n");

// テーブル名
static char		*SYZ_SZSYKSUB_TBL = _T("szsyksub");
// テーブルソート
static	char	*SYZ_SZSYKSUB_VAL_SORT = _T("Szkcod");
// テーブル作成
static char		*SYZ_SZSYKSUB_SQL = _T("CREATE TABLE [dbo].[%s] (\n"
										"[Szkcod] [char](8) NOT NULL, \n" /* 消費税対象明細科目コード */
										"[Szgrp1] [tinyint] NOT NULL, \n"    /* 消費税処理グループ区分 */
										"[Szgrp2] [tinyint] NOT NULL, \n"    /* 消費税処理グループ区分明細 */
										"[SZzkbn] [tinyint] NOT NULL, \n"    /* 税率区分 */
										"[Szitm]  [tinyint], \n"             /* 売上・仕入区分サイン */
										"[SZsjsg] [tinyint], \n"             /**/
										"[Szkatt] [tinyint], \n"             /* 貸借サイン */
										"[SZzchg] [tinyint], \n"             /* 税率切替サイン */
										"[SZrsv]  [binary](1), \n"           /* リザーブ */
										"[Syksubofs] [tinyint], \n"             /* 月オフセット */
										"[Sztaig] decimal(14,0), \n"         /* 金額 */
										"[Szhikz] decimal(14,0), \n"         /* 金額 */
										"[Szyuka] decimal(14,0), \n"         /* 金額 */
										"[Szkomi] decimal(14,0), \n"         /* 金額 */
										"[Szkomz] decimal(14,0), \n"         /* 金額 */
										"[Sznuki] decimal(14,0), \n"         /* 金額 */
										"[Sznukz] decimal(14,0), \n"         /* 金額 */
										"[Szbeki] decimal(14,0), \n"         /* 金額 */
										") ON [PRIMARY]\n");

// カラム存在チェック
static char		*ISEXISTCOLUMN			=	_T( "SELECT COUNT( * ) FROM sys.columns WHERE name = '%s' AND object_id = object_id( '%s' )" );


#endif	// __DBSYZACC_ICS_DBSYZACCSQL__
