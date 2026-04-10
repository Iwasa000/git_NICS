// DBSyzAccDef.h : ヘッダー ファイル

#ifndef  __DBSYZACC_ICS_DBSYZACCDEF__
#define  __DBSYZACC_ICS_DBSYZACCDEF__

#define	MONTH_MAX	19

#pragma pack( push, 2 )

// 部門枝番別集計拡張レコード
typedef	struct	_BME_SZSYK	{
			 short	Bme_type;				// 集計タイプ						[ 2B]
			 short	Bme_mnth;				// 月オフセット（★未使用）			[ 2B]
			 int	Bme_bumn;				// 部門コード　マイナスを有効にする	[ 4B]
	unsigned char	Bme_kcod[8];			// 科目（内部）コード (８桁)		[ 8B]
			 int	Bme_edab;				// 枝番コード　マイナスを有効にする	[ 4B]	
	unsigned char	Bme_grp1;				// 消費税グループ区分コード			[ 1B]	
	unsigned char	Bme_grp2;				// 消費税グループ区分明細コード		[ 1B]		
	unsigned char	Bme_zkbn;				// 税率区分サイン					[ 1B]
	unsigned char	Bme_item;				// 売上・仕入区分サイン				[ 1B]
	unsigned char	Bme_sjsg;				// 固定資産の譲渡及び取得サイン		[ 1B]
	unsigned char	Bme_rvsg;				// リザーブ							[ 1B]
// ここまで主キーとなる		[26B]
	unsigned char	Bme_katt;				// 貸借サイン						[ 1B]	
	unsigned char	Bme_zchg;				// リザーブ							[ 1B]
	unsigned char	Bme_sgn1;				// リザーブ							[ 1B]
	unsigned char	Bme_sgn2;				// リザーブ							[ 1B]
			 char	Bme_taig[MONTH_MAX][6];	// 対象外金額						[ 6B]
			 char	Bme_hikz[MONTH_MAX][6];	// 非課税金額						[ 6B]
			 char	Bme_yuka[MONTH_MAX][6];	// 非課税有価証券５％				[ 6B]
			 char	Bme_komi[MONTH_MAX][6];	// 税込金額							[ 6B]
			 char	Bme_komz[MONTH_MAX][6];	// 税込税額							[ 6B]
			 char	Bme_nuki[MONTH_MAX][6];	// 税抜金額							[ 6B]
			 char	Bme_nukz[MONTH_MAX][6];	// 税抜税額							[ 6B]
			 char	Bme_bekk[MONTH_MAX][6];	// 別記金額							[ 6B]
			 char	Bme_ikkm[MONTH_MAX][6];	// 総額一括税抜　税込金額			[ 6B]
			 char	Bme_ikkz[MONTH_MAX][6];	// 総額一括税抜　税込税額			[ 6B]
			 char	Bme_adjs[MONTH_MAX][6];	// 総額一括税抜　調整税額			[ 6B]
			 char	Bme_iptk[MONTH_MAX][6];	// 輸入仕入　国税					[ 6B]
			 char	Bme_iptt[MONTH_MAX][6];	// 輸入仕入　地方税					[ 6B]
			 char	Bme_val1[MONTH_MAX][6];	// リザーブ							[ 6B]
			 char	Bme_val2[MONTH_MAX][6];	// リザーブ							[ 6B]
} BMESZSYK;

#pragma pack( pop )

#endif
