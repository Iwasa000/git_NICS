#pragma	once

class CSnHeadData
{
public:
	unsigned char	Sn_VER,			// ファイルバ－ジョン (Begin = 0x00)	szsnk->Sn_VER
					Sn_SKKBN,		// 申告区分　…　１＝確定				szsnk->Sn_SKKBN
									//				２＝中間
									//				３＝修正確定
									//				４＝修正中間
									//				０＝空白
					Sn_KOJIN,		// 個人以外の事業者         ON :12ヶ月計算	szsnk->Sn_KOJIN
					Sn_KOANB,		// 個人事業者　ON:月数按分, OFF:12ヶ月計算	szsnk->Sn_KOANB
					Sn_ITIREN[4],	// 一連番号　　　　　　［ＢＣＤ８桁］		szsnk->Sn_ITIREN
					Sn_SEIRI[4],	// 整理番号　　　　　　［ＢＣＤ８桁］		szsnk->Sn_SEIRI
					Sn_ZSEIRI[4];	// 税務署整理番号　　　［ＢＣＤ８桁］		szsnk->Sn_ZSEIRI

//					Sn_KDAYS[3],	// 課税期間　自　　　  ［ＢＣＤ６桁］		szsnk->Sn_KDAYS
//					Sn_KDAYE[3],	//			 至　　　  ［　　〃　　］		szsnk->Sn_KDAYE
//					Sn_MDAYS[3],	// 中間申告の計算期間  ［　　〃　　］		szsnk->Sn_MDAYS
//					Sn_MDAYE[3],	//					   ［　　〃　　］		szsnk->Sn_MDAYE
//					Sn_PRDAY[3],	// 提出年月日　　　　　［ＢＣＤ６桁］		szsnk->Sn_PRDAY
	long			Sn_KDAYS,		// 課税期間　自　　　  ［ＢＣＤ６桁］		szsnk->Sn_KDAYS
					Sn_KDAYE,		//			 至　　　  ［　　〃　　］		szsnk->Sn_KDAYE
					Sn_MDAYS,		// 中間申告の計算期間  ［　　〃　　］		szsnk->Sn_MDAYS
					Sn_MDAYE,		//					   ［　　〃　　］		szsnk->Sn_MDAYE
					Sn_PRDAY;		// 提出年月日　　　　　［ＢＣＤ６桁］		szsnk->Sn_PRDAY
	unsigned char	Sn_SPECIAL,		// D0:ON:簡易計算する						szsnk->Sn_SPECIAL
									// D1:ON:印刷 申告書本表「提出用」					貨物の税額より80％で計算
									// D2:ON:印刷 申告書本表「控用」					印刷	更正の請求書（申出書）	
									// D3:ON:印刷 付表２　　又は　付表５				印刷	還付申告に関する明細書(P.1)
									// D4:ON:印刷 付表１　　又は　付表４				印刷	本表（一般・簡易）
									// D5:ON:印刷 付表２―２又は　付表５-２				印刷	付表１・４
									// D6:ON:印刷 第２６号様式							印刷	付表２・５
									// D7:ON:印刷 仕入控除税額に関する明細書			印刷	付表２-２・５-２
									// 
					Sn_ZMSYO[12],	// 税務署名　　　　　　［漢字６文字］		szsnk->Sn_ZMSYO
					Sn_ZIP[10],		// 郵便番号			　 ［ＡＳＣＩＩ８桁］	szsnk->Sn_ZIP
					Sn_SYOKN[4],	// 所管 (Sn_dmy2[4],)  ［ＡＳＣＩＩ２桁］	szsnk->Sn_SYOKN
					Sn_TELNO1[6],	// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_TELNO2[4],	// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_TELNO3[4],	// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_NOZEI[40*2],	// 納税地　　　　　　　［漢字２０＊２文字］	szsnk->Sn_NOZEI
					Sn_CONAM[40],	// 名称又は，屋号　　　［漢字２０文字］		szsnk->Sn_CONAM
					Sn_COFRI[80],	// 名称又は屋号振り仮名［漢字半角２０文字］	szsnk->Sn_COFRI
					Sn_DIHYO[40],	// 代表者氏名　　　　　［漢字２０文字］		szsnk->Sn_DIHYO
					Sn_DIFRI[80],	// 代表者氏名振り仮名　［漢字半角２０文字］	szsnk->Sn_DIFRI
					Sn_KEIRI[40],	// 経理担当者氏名　　　［漢字２０文字］		szsnk->Sn_KEIRI


					Sn_KAPPU,		// 割賦基準の適用　　　１＝有　０＝無		szsnk->Sn_KAPPU
					Sn_NOBE,		// 延払基準の適用　　　１＝有　０＝無		szsnk->Sn_NOBE
					Sn_KOJI,		// 工事進行基準の適用　１＝有　０＝無		szsnk->Sn_KOJI
					Sn_GENKN,		// 現金主義会計の適用　１＝有　０＝無		szsnk->Sn_GENKN
					Sn_TUMIAG,		// 規則22条１項の適用　１＝有　０＝無		szsnk->Sn_TUMIAG
					Sn_KZTNSK,		// 課税期間短縮　　　　１＝有　０＝無		szsnk->Sn_KZTNSK
					Sn_MCOUNT,		// 中間申告　年間回数　３＝３回 １＝１回	szsnk->Sn_MCOUNT
					Sn_JCOUNT,		// 中間申告　現在申告状態　１～４回			szsnk->Sn_JCOUNT
					Sn_TOKUR,		// 特例計算摘要ＳＷ							szsnk->Sn_TOKUR
					Sn_UPERS,		// 課税売上割合９５％以上のＳＷ				szsnk->Sn_UPERS
					Sn_TYPE;		// D0:ON:イメージ印刷	OFF:ＯＣＲ印刷		szsnk->Sn_TYPE
									// D1:ON:提出用　カラー用紙	 OFF:提出用　モノクロ用紙	ON:控用 OFF:提出用カラー
									// D2:ON:控用　　カラー用紙　OFF:控用　  モノクロ用紙
									// D3:
									// D4:
									// D5:
									// D6:
									// D7:
	unsigned char	Sn_BYsw,		// 金融機関選択SW　OFF:銀行,ON:郵便局			szsnk->Sn_BYsw
					Sn_BANK[20],	// 銀行名称　　　　　　［漢字６文字］			szsnk->Sn_BANK
					Sn_SITEN[20],	// 支店名称　　　　　　［漢字６文字］			szsnk->Sn_SITEN
					Sn_YOKIN[8],	// 預金種別　　　　　　［漢字４文字］			szsnk->Sn_YOKIN
					Sn_YUBIN[16],	// 郵便局名　　　　　　［漢字６文字］			szsnk->Sn_YUBIN
					Sn_KOZA[5],		// 口座番号　　　　　　［ＢＣＤ10桁］			szsnk->Sn_KOZA
					Sn_YUNUM1[5],	// 郵便貯金の記号番号１(5ｹﾀ)					szsnk->Sn_YUNUM1
					Sn_YUNUM2[8],	// 郵便貯金の記号番号２(8ｹﾀ)					szsnk->Sn_YUNUM2
					Sn_FINANCE,		// 0:無し 1:銀行 2:金庫 3:組合 4:農協 5:漁協	szsnk->Sn_FINANCE
					Sn_CHAIN;		// 0:無し 1:本店 2:支店 3:本社 4:支社			szsnk->Sn_CHAIN

public:
	CSnHeadData();   // 標準コンストラクタ
	virtual ~CSnHeadData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );

private:
	CUtil			m_Util;
	CArithEx		m_Arith;
};
