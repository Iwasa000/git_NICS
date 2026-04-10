/*------------------------------------------------------------------
		消費税管理ボリューム定義レコード データベース
	--------------------------------------------------------------
		Header   :	SYZDEF.H
	--------------------------------------------------------------
		Update   :	25-Oct./'96		平成９年度-改正消費税 対応
					16-Dec./'96		･申告書エリア登録ｓｗ 追加
									･複数税率処理ｓｗ 追加
  					12-Jun./'97		･個別対応4%集計ｓｗ 追加
					26-Feb./'98		･個人事業種選択ｓｗ 追加
					22-Oct./'98		.輸入仕入集計エリア 追加
					05-Oct./'99		.財務非連動情報     追加
					17-Nov./'00		.特定収入科目集計レコード　追加
					22-Mar./'01		.申告書集計サイン追加
								　　.特定収入処理サイン
					28-Mar./'03		.差異チェックサイン追加
					17-Mar./'04		.中間申告年11回税額エリア　追加
					17-Mar./'04		.電子申告用改正バージョン　追加
					05-Apr./'04		.非連動更新改良に為の追加
					11-Aug./'04		.棚卸入力データ領域追加
									.中間申告１１ヶ月確保
					11-Nov./'04		.前年実績による中間申告対応
  ------------------------------------------------------------------*/

#pragma pack( push, 2 )

/*----- 消費税管理情報レコード (512byte) -----*/
typedef struct	_SVHED	{
	unsigned char	SVvers,					/* ファイル・バージョン */
					SVdmy1;
	unsigned short	SVfsiz,					/* ファイル・サイズ (sec) */
					SVupos,					/* 課税仕入･売上集計エリアへのポジション */
					SVspos;					/* 申告書エリアへのポジション */
	unsigned short	SVrmax,					/* 税率最大登録数 */
					SVrnum,					/* 税率実登録数 */
					SVzper[3];				/* 税率テーブル */
	char			SVdmy[6];
	unsigned short	SVsper[5];				/* 簡易課税（みなし仕入率テーブル） */
	char			SVdmy4[6];
	unsigned char	SVgen0[6],				/* 限界控除適用の課税売上高範囲（下限） */
					SVgen1[6],				/* 限界控除適用の課税売上高範囲（上限） */
					SVkan0[6],				/* 簡易課税適用の課税売上高範囲（下限） */
					SVkan1[6],				/* 簡易課税適用の課税売上高範囲（上限） */
					SVdmy5[8],
					SVmctl[13],				/* 月別集計管理テーブル */
					SVzkbn,					/* 一括税抜き処理区分ｓｗ */
					SVstymd[3],				/* 自動車税率廃止->消費税改正施行年月日 */
					SVtkymd[3];				/* 特別消費税廃止年月日 */
	unsigned short	SVzrto,					/* 改正消費税率 (4.0%) */
					SVlrto;					/* 地方消費税率 (1.0%) */
	unsigned char 	SVkcod[8][8];			/* ★消費税関連科目テーブル  */
	char			SVdmy6[16];				/* 使用不可 */
	/* 限界控除制度廃止にともなう経過措置の為のエリア [09-Apr-96] */
	unsigned char	SVsasw,					/* 申告書エリア　登録ＳＷ */
					SVmzsw,					/* 複数税率処理ｓｗ */
					SVhksw,					/* 販売奨励金収入科目 登録ｓｗ */
					SV_5sw,					/* 第５種事業に係る経過措置に関するｓｗ */
					SVkojn,					/* 個人事業種選択ｓｗ D0:個人以外 D4:個人*/
					SVzchg,					/* 免税⇔課税切り替えサイン */
					SVtval1[6],				/* 調整課税仕入等に係る棚卸資産額 */
					SVtval2[6],				/* 調整課税仕入等に係る棚卸消費税額 */
					SVzuri[6];				/* 基準期間の課税売上高 */
	struct	_SVTBL	{
		unsigned char	SVsmd[2],			/*  開始月日            */
						SVemd[2];			/*  最終月日            */
	} SVTBL[13];
	char			SVkazei;				/* 消費税　課税区分／仕入税額の按分法 */
	unsigned char	SVcymd[3];				/* 計算上の期首                       */
	unsigned char	SVsacq;					/* 申告集計サイン */
	unsigned char	SVzisq;					/* 担当税理士選択番号 [02'13.03] */
	unsigned char	SVsign;					/* 特殊サイン [03'03.28] */
											/* 棚卸調整　　込み・抜きサイン D4: ON=込み OFF=抜き */
											/* 基礎計算表　込み・抜きサイン D5: ON=込み OFF=抜き */
	unsigned char	SVsig0;					/* 特殊サイン② [04'08.27] */

	/* [04'04.05] 非連動更新改良に為の追加 */
	unsigned char	SVsymd[3],				/* 非連動用　期首年月日 */
					SVeymd[3];				/* 〃　　　　期末年月日 */

	unsigned char	SVnver[2];				/* 電子申告バージョン [04'03.17] */

	char			SVdmy8[132];			/* SVdmy8[141] */

	/*----	平成16年消費税改正対応(160B) [04'03.17] ----*/
	char			ExSVTYNF[12][6];		/* 中間申告年11回分　国税 */
	char			ExSVTTNF[12][6];		/* 〃　　　　　　　　地方税 */
	char			SVtmth[12];				/* 中間申告前年又は仮決算サイン [04'11.22] */
	char			SVdmy9[4];				/* SVdmy9[16];		*/
} SVHED;

/*----- 課税仕入区分／売上区分の集計エリア レコード (6->9sec) -----*/
// (naG)：Ｑ：４半期を除外しない時、13ヵ月しかレコードを持っていなくても大丈夫？
typedef struct	_SUKR	{	/* 112byte */
	unsigned char	SUskb,					/* 消費税処理区分 */
					SUdkb,					/* 仕訳区分 */
					SUcof,					/* 税率切替時の経過月オフセット＋１ */
					SUtax[13],				/* 4%での集計済みサイン */
					SUval[13][6],			/* 集計金額 */
					SUval2[6],				/* 税率切替前の 税抜き & 税込み金額 */
					SUdm2[12],
					SUkvl[13][6],			/* 簡易業種売上５％（税込み） */
					SUnvl[13][6],			/* 簡易業種売上５％（抜きみ） */
					SUkbk[13][6],			/* 簡易業種売上５％（別  記） */
					SUZi5[13][6],			/* 簡易業種売上５％　税額 　　*/
					SUZi3[13][6];			/* 簡易業種売上３％　税額 　　*/
	unsigned char	SUkm5[13][6];			/* ５％込み */
	unsigned char	SUkm3[13][6];			/* ３％込み */
} SUKR;

/*----- 輸入仕入集計エリア レコード (10sec->416byte) -----*/
typedef struct	_SUSR	{	/* 30byte */
	unsigned char	SSval5[6],
					SSzei5[6],
					SSzei4[6],
					SSval3[6],
					SSzei3[6];
} SUSR;

typedef struct	_SVSUM	{	//-- [旧] 課税仕入区分／売上区分の集計エリア
	struct	_SUKR	SSstbl[11];				/* 集計エリア・テーブル */
	char			SSdmy1[304];
} SVSUM;

typedef struct	_H9SUM	{	//-- [平成９年度] 課税仕入区分／売上区分／輸入仕入の集計エリア
	/* 課税仕入区分／売上区分集計エリア・テーブル */
	struct	_SUKR	SSstbl[16];				// (naG)
											//  0-2：仕入区分（5%）
											//  0：課税売上に係る
											//  1：非課税売上に係る
											//  2：共通に係る
											//  3-7：売上区分（5%）
											//  3：卸売業
											//  4：小売業
											//  5：製造業
											//  6：その他
											//  7：サービス
											//  8-10：仕入区分（3%）
											//  8：課税売上に係る
											//  9：非課税売上に係る
											// 10：共通に係る
											// 11-15：売上区分（3%）
											// 11：卸売業
											// 12：小売業
											// 13：製造業
											// 14：その他
											// 15：サービス

	/* 輸入仕入エリア・テーブル */
	unsigned char	SSskb,					/* 輸入仕入　消費税区分 */
					SSdmy0[15];				/* 輸入仕入　予備 */
	struct	_SUSR	SSustb[13];				/* 輸入仕入　集計エリア・テーブル */

	// 個別対応時の棚卸調整額 (72B)	[04'08.11]
	char			SVtansw,				/* 仕訳発生サイン */
					SVtdmmy;	
	char			ExSVmnk40[3][6];
	char			ExSVmnz40[3][6];
	char			ExSVmenzk[3][6];
	char			ExSVmenzz[3][6];

	char			SSdmy1[32];	// SSdmy1[106];
} H9SUM;

/*----- 消費税科目集計レコード (336byte) -----*/
typedef struct	_SDVR	{
	unsigned char	SDval[8][6];			/* ★該当の金額８項目 */
											// 0：対象外
											// 1：非課税
											// 2：有価証券５％
											// 3：込み
											// 4：込み税額
											// 5：抜き
											// 6：抜き税額
											// 7：別記
} DVR;

typedef struct	_SZREC	{
	unsigned char	SZkcod[8];				/* ★消費税対象明細科目コード */
	unsigned char	SZgrp1,					/* 消費税処理グループ区分 */
					SZgrp2,					/* 消費税処理グループ区分 明細 */
					SZzkbn,					/* 税率区分 */
					SZitem,					/* 売上／仕入区分サイン [05'03.24] */
					SZsjsg,					/* 譲渡サイン */
					SZkatt,					/* 貸借サイン */
					SZzchg,					/* 月途中での税率切替ｓｗ */
					SZdmy1;
	struct	_SDVR	SZdtbl[13];				/* 集計テーブル */
	unsigned char	SZval2[2][6],			/* 税率切替後の 税抜き & 税込み金額 */
					SZdmy2[2];				/* 科目と仕訳の属性が違う場合の情報格納に使用 */
} SZREC;

/*----- 特定収入科目集計レコード (336byte) -----*/
typedef struct	_SPVR	{
	unsigned char	SPval[6][6];
} SPVR;

typedef struct	_SPREC	{
	unsigned char	SPkcod[8];				/* ★消費税対象明細科目コード */
	unsigned char	SPgrp1,					/* 消費税処理グループ区分 */
					SPgrp2,					/* 消費税処理グループ区分 明細 */
					SPdmy1,					/* リザーブ */
					SPdmy2,					/* リザーブ */
					SPkatt,					/* 貸借サイン */
					SPdmy3[9];				/* リザーブ */
	struct	_SPVR	SPdtbl[13];				/* 集計テーブル */
	unsigned char	SPdmy4[28];		/* リザーブ */
} SPREC;

/* 特定収入　計算表２の明細 */
typedef	struct	_TSREC	{
			 long	TsLine;				// 行番号
			 char	TsName[40];			// 項目名
			 char	TsJsyu[6],			// 資産の譲渡等の対価以外の収入（実額入力）
					TsTsyu[6],			// 特定収入		比例
					TsFsyu[6],			// 不特定収入	比例
					TsGsyu[6];			// 特定収入合計	比例
			char	TsSsyu[6],			// 特定収入　　 個別　課税売上に係る
					TsHsyu[6],			// 〃			〃　　非課税に係る
					TsKsyu[6];			// 〃			〃　　共通にかかつ
} TSREC;

// スイッチ定義
typedef struct	DAY_PAC	{
		 char	Symd[4],
				Eymd[4];
} DAYPAC;

//１２ヶ月データ期間テーブル計算用
typedef struct	_date	{
	int	s_yy,s_mm,s_dd,e_yy,e_mm,e_dd;
} DATES;

//★//[12'11.01]///
// 国税納付書比較用
typedef	struct	_PAYMENT	{
unsigned char	PM_SKKBN,		// 申告区分
				PM_KZTNSK,		// 期間短縮
				PM_MCOUNT,		// 中間申告回数
				PM_Syuusei,		// 講税の請求
				PM_DMME1[12],	// ダミー
				PM_KDAYS[3],	// 課税期間　自
				PM_KDAYE[3],	// 　　　　　至
				PM_MDAYS[3],	// 中間申告の計算期間　自
				PM_MDAYE[3],	// 　　　　　　　　　　至
				PM_DMME2[100];	// ダミー
} PAYMENT;

// 期間短縮及び中間申告期間テーブル
typedef	struct	_SHIN_MONTH	{
	int		SMsnum;							// 申告回数	
	int		SMskey;							// 予備 主キー	
	char	SMskbn;							// 申告区分
	char	SMskts;							// 期間短縮回数
	char	SMscnt;							// 中間申告回数
	char	SMsign;							// 予備サイン
	char	SMsday[4];						// 対象期間　自	
	char	SMeday[4];						// 対象期間　至	
} SHINMONTH;

// 申告書履歴情報レコード
typedef struct _SHIN_INFO	{
	int		SIitem;								// 保存種別
	int		SIsequ;								// 保存番号	
	int		SIsnum;								// 申告回数	
	int		SIskey;								// 予備 主キー	
	char	SIskbn;								// 申告区分
	char	SIsign;								// D0:ON決定 OFF:未決定
	char	SIksdy[4];							// 課税期間　自		
	char	SIkedy[4];							// 課税期間　至		
	char	SImsdy[4];							// 中間＆短縮期間　自
	char	SImedy[4];							// 中間＆短縮期間　至
	char	SIcsdy[4];							// 変更後期間　自
	char	SIcedy[4];							// 変更後期間　至
	char	SIpday[4];							// 提出年月日	
	char	SIkday[4];							// 更新年月日
	char	SIuser[64];							// ユーザ名
	char	SIday1[4];							// 年月日①
	char	SIusr1[64];							// ユーザ名①
	char	SIday2[4];							// 年月日②
	char	SIusr2[64];							// ユーザ名②
	char	SImflg[128];						// 実額フラグ[128B]
} SHININFO;

#pragma pack( pop )
