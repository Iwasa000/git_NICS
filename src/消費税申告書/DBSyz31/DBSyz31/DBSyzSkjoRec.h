// 仕入控除税額に関する明細書マスター 
//	SDEDUCT.H00		:	法人用
//	SDEDUCT.K00		:	個人用

#pragma pack( push, 2 )

// 集計科目範囲
typedef	struct	_SK_KMRAN{
	unsigned	short	skcd,		// 集計科目範囲　開始
						ekcd;		// 集計科目範囲　最終
	unsigned	char	Vdmy3[4];
}	SK_KMRAN;

// 控除対象取引金額
typedef	struct _SK_KJCALC{

	unsigned	char		KesMny[6],	// イ　決算額					[BIN 12桁]
							NoKjMny[6],	// ロ　左のうち控除対象外金額	[BIN 12桁]
							KjTsMny[6],	// ハ　控除対象取引金額			[BIN 12桁]
							Vdmy1[14];
	
	unsigned	short		StKcd,		// 指定科目範囲　開始
							EdKcd;		// 指定科目範囲　最終
	
	unsigned	char		Vdmy2[14];
		
	SK_KMRAN				KmRan[20];	// 集計科目範囲

	unsigned	char		Vdmy3[48];

}	SK_KJCALC;

// 明細情報
typedef	struct	_SK_DTLINF {

	unsigned	char	AsKndUpBuf[16],	// 資産の種類（上段）	[漢字半角8文字]
						Vdmy1[8],
						AsKndLwBuf[16],	// 資産の種類（下段）	[漢字半角8文字]
						Vdmy2[8];
	unsigned	char	Gengo,			// 取得年月日　元号
						Vdmy3[4],
						Day[3],			// 取得年月日　年月日
						Vdmy4[1];
	unsigned	char	TrMny[6],		// 取引金額等			[BIN 12桁]
						Vdmy5[4];
	unsigned	char	NameUpBuf[20],	// 取引先氏名（上段）	[漢字半角10文字]
						Vdmy6[12],
						NameLwBuf[20],	// 取引先氏名（下段）	[漢字半角10文字]
						Vdmy7[12];
	unsigned	char	AdrUpBuf[30],	// 取引先住所（上段）	[漢字半角15文字]
						Vdmy8[18],
						AdrLwBuf[30],	// 取引先住所（下段）	[漢字半角15文字]
						Vdmy9[18];
	unsigned	char	Vdmy10[32];

}	SK_DTLINF;

///////////////////////////////////////////////////////////////////////////////
// 管理・基礎情報 (256Byte)
typedef	struct _SK_HEAD {
	unsigned	char	Sk_FVer,		// ファイルバージョンNo
						Sk_Vdmy1[15];
	unsigned	long	Sk_FSiz,		// ファイルサイズ
						SK_MOfst,		// マスターエリアのオフセット
						SK_MSiz;		// マスターエリアサイズ
	unsigned	char	SK_Vdmy2[228];
}	SK_HEAD;

///////////////////////////////////////////////////////////////////////////////
// 【法人】仕入控除税額情報 ( 9216Byte )
typedef struct _HJ_SKDATA {

	// 1 控除対象取引金額------------------------------------------------------
	unsigned	char		Hj_PlTxSgn,		// 控除対象取引金額　損益科目　税抜・税込の別
											// D0 : ON.	税込	OFF.税抜
							Hj_Vdmy1[15];
	
	SK_KJCALC				Hj_PlKmk[6];	// 損益科目・・・	[0] 商品仕入高等
											//					[1] 販売費・一般管理費
											//					[2] 営業外費用
											//					[3] その他
											//					[4] 未使用
											//					[5] 小計
	
	unsigned	char		Hj_Vdmy2[240];

	unsigned	char		Hj_AsTxSgn,		// 控除対象取引金額　資産科目　税抜・税込の別
											// D0 : ON.	税込	OFF.税抜
							Hj_Vdmy3[15];

	SK_KJCALC				Hj_AsKmk[6];	// 資産科目・・・	[0] 固定資産
											//					[1] 繰延資産
											//					[2] 未使用
											//					[3] 未使用
											//					[4] 未使用
											//					[5] 小計

	unsigned	char		Hj_Vdmy4[240];

	unsigned	char		Hj_SrKjTxMny[6];// 仕入控除税額	[BIN 12桁]

	unsigned	char		Hj_SrKjTxSw;	// 仕入控除税額手入力サイン
											// D0 : ON.手入力
	unsigned	char		Hj_Vdmy5[121];

	// 2 還付申告となった主な理由----------------------------------------------
	unsigned	char		Hj_SeTsSgn,		// 設備投資
											// □□□□□□□■
											// D0 : ON. 選択有り
							Hj_YuMzSgn,		// 輸出等の免税取引の割合が高い
											// □□□□□□□■
											// D0 : ON. 選択有り
							Hj_EtcSgn,		// その他
											// □□□□□□□■
											// D0 : ON. 選択有り
							Hj_Vdmy6[13];

	unsigned	char		Hj_EtcRsUpBuf[48],	// その他の理由（上段）[漢字半角20文字]
							Hj_Vdmy7[8],
							Hj_EtcRsLwBuf[48],	// その他の理由（下段）[漢字半角20文字]
							Hj_Vdmy8[8];

	// 3 主な課税仕入れ等の明細------------------------------------------------
	unsigned	char		Hj_KzSrTxSgn,	// 主な課税仕入れ等の明細　税込・税抜の別
											// D0 : ON.	税込	OFF.税抜
							Hj_Vdmy9[15];
	
	SK_DTLINF				Hj_KzSrDtl[20];	// 主な課税仕入れ等の明細

	unsigned	char		Hj_Vdmy10[240];
	
}	HJ_SKDATA;

///////////////////////////////////////////////////////////////////////////////
// 【個人】仕入控除税額情報エリア ( 9356Byte )
typedef	struct	_KJ_SKDATA {

	// 1.課税仕入高の計算------------------------------------------------------
	unsigned	char		Kj_KzSrTxSgn,	// 課税仕入高の計算　税抜・税込の別
											// D0 : ON.	税込	OFF.税抜
							Kj_Vdmy1[15];

	SK_KJCALC				Kj_JgSyKmk[6];	// 事業所得・・・	[0] 仕入金額
											//					[1] 必要経費
											//					[2] 固定資産等の取得価額
											//					[3] 未使用
											//					[4] 未使用
											//					[5] 小計
	unsigned	char		Kj_Vdmy2[240];

	SK_KJCALC				Kj_HdSyKmk[6];	// 不動産所得・・・	[0] 必要経費
											//					[1] 固定資産等の取得価額
											//					[2] 未使用
											//					[3] 未使用
											//					[4] 未使用
											//					[5] 小計
	unsigned	char		Kj_Vdmy3[240];

	unsigned	char		Kj_SytkBuf[18],	// 所得名称	[漢字8文字] [06'10.31 +2 ]
							Kj_Vdmy4[14];	//						[06'10.31 -2 ]

	SK_KJCALC				Kj_SytkKmk[6];	// ○○所得・・・	[0] 仕入金額
											//					[1] 必要経費
											//					[2] 固定資産等の取得価額
											//					[3] 未使用
											//					[4] 未使用
											//					[5] 小計
	unsigned	char		Kj_Vdmy5[240];

	unsigned	char		Kj_KzSrMny[6],	// 課税仕入高の合計額	[BIN 12桁]
							Kj_SrKjTxMny[6],// 仕入控除税額			[BIN 12桁]
							Kj_SrKjTxSw,	// 仕入控除税額手入力サイン
											// D0 : ON.手入力
							Kj_Vdmy6[115];

	// 2.還付となった主な理由サイン--------------------------------------------
	unsigned	char		Kj_KsKnSgn,			// 固定資産の購入サイン
												// □□□□□□□■
												// D0 : ON. 選択有り
							Kj_SrZkSgn,			// 仕入金額・経費の増加サイン
												// □□□□□□□■
												// D0 : ON. 選択有り
							Kj_EtcSgn,			// その他サイン
												// □□□□□□□■
												// D0 : ON. 選択有り
							Kj_Vdmy7[13],
							Kj_EtRsUpBuf[53],	// その他の理由（上段）[漢字半角25文字]
							Kj_Vdmy8[3],
							Kj_EtRsLwBuf[53],	// その他の理由（下段）[漢字半角25文字]
							Kj_Vdmy9[3];

	// 3.固定資産等の取得状況--------------------------------------------------
	unsigned	char		Kj_KzSrTxSgn2,	// 主な課税仕入れ等の明細　税込・税抜の別
											// D0 : ON.	税込	OFF.税抜
							Kj_Vdmy10[15];

	SK_DTLINF				Kj_KsStBuf[20];	// 固定資産等の取得状況

	unsigned	char		Kj_Vdmy11[240];

	// 4.特殊事情--------------------------------------------------------------
	unsigned	char		Kj_SpCrcNen,	// 特殊事情　年度
							Kj_Vdmy12[15];

	struct	_SPCRCUM {
		unsigned	char	SpCrcBuf[123],	// 特殊事情	[漢字半角60文字]
							Vdmy[5];
	} Kj_SpCrc[6];							// ×６行

	unsigned	char		Kj_Vdmy13[240];

} KJ_SKDATA;


/////////[10'03.05]
// 仕入れ控除税額に関する明細書の明細入力■
typedef	struct	_SKJ_KSR {
				int		SkjLine;		// 行番号	ソートキー
				int		SkjColm;		// 列番号	〃
	unsigned	char	SkjSaveDay[4];	// 保千月日	〃

	unsigned	char	AsKndUpBuf[16],	// 資産の種類（上段）	[漢字半角8文字]
						Vdmy1[8],
						AsKndLwBuf[16],	// 資産の種類（下段）	[漢字半角8文字]
						Vdmy2[8];
	unsigned	char	Gengo,			// 取得年月日　元号
						Vdmy3[5],
						Day[3],			// 取得年月日　年月日
						Vdmy4[1];
	unsigned	char	TrMny[6],		// 取引金額等			[BIN 12桁]
						Vdmy5[4];
	unsigned	char	NameUpBuf[20],	// 取引先氏名（上段）	[漢字半角10文字]
						Vdmy6[12],
						NameLwBuf[20],	// 取引先氏名（下段）	[漢字半角10文字]
						Vdmy7[12];
	unsigned	char	AdrUpBuf[30],	// 取引先住所（上段）	[漢字半角15文字]
						Vdmy8[18],
						AdrLwBuf[30],	// 取引先住所（下段）	[漢字半角15文字]
						Vdmy9[18];
	unsigned	char	Vdmy10[16];		

}	SKJ_KSR;
///////////////////

#pragma pack( pop )
