#pragma once

class CSnData
{
public:
//<-- SVHEAD START
	unsigned char	SVvers;		// ファイル・バージョン					szvol->SVvers
	unsigned short	SVzper[3];	// 税率テーブル							szvol->SVzper
//	SVdmy???
	unsigned short	SVsper[5];	// 簡易課税（みなし仕入率テーブル）		szvol->SVsper1

	unsigned char	SVkan0[6];	// 簡易課税適用の課税売上高範囲（下限）	szvol->SVkan0
	unsigned char	SVkan1[6];	// 簡易課税適用の課税売上高範囲（上限）	szvol->SVkan1
	unsigned char	SVmctl[13];	// 月別集計管理テーブル					独自！
	unsigned char	SVzkbn;		//										独自！

	unsigned short	SVzrto;		// 改正消費税率 (4.0%)					40 固定
	unsigned short	SVlrto;		// 地方消費税率 (1.0%)					10 固定

	unsigned char 	SVkcod[8][8];	// ★消費税関連科目テーブル			szvol->SVkcod1

	unsigned char	SVsasw;		// 申告書エリア　登録ＳＷ				szvol->SVsasw
	unsigned char	SVmzsw;		// 複数税率処理ｓｗ						szvol->SVmzsw
	unsigned char	SVhksw;		// 販売奨励金収入科目 登録ｓｗ			szvol->SVhksw
	unsigned char	SV_5sw;		// 第５種事業に係る経過措置に関するｓｗ	szvol->SVd5sw
	unsigned char	SVkojn;		// 個人事業種選択ｓｗ D0:個人以外 D4:個人	szvol->SVkojn
	unsigned char	SVzchg;		// 免税⇔課税切り替えサイン				szvol->SVzchg
	unsigned char	SVtval1[6];	// 調整課税仕入等に係る棚卸資産額		使用？？？
	unsigned char	SVtval2[6];	// 調整課税仕入等に係る棚卸消費税額		使用？？？
	unsigned char	SVzuri[6];	// 基準期間の課税売上高					szvol->SVzuri

	char			SVkazei;	// 消費税　課税区分／仕入税額の按分法	szvol->SVkazei

	unsigned char	SVsacq;		// 申告集計サイン						szvol->SVsacq
	unsigned char	SVzisq;		// 担当税理士選択番号					szvol->SVzisq
	unsigned char	SVsign;		// 特殊サイン							szvol->SVsign
								// 棚卸調整　　込み・抜きサイン D4: ON=込み OFF=抜き
								// 基礎計算表　込み・抜きサイン D5: ON=込み OFF=抜き

	char			SVtmth[12];	// 中間申告前年又は仮決算サイン			独自！
//--> SVHEAD END

//--> SN_HEAD START
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
					Sn_ZSEIRI[4],	// 税務署整理番号　　　［ＢＣＤ８桁］		szsnk->Sn_ZSEIRI

					Sn_KDAYS[3],	// 課税期間　自　　　  ［ＢＣＤ６桁］		szsnk->Sn_KDAYS
					Sn_KDAYE[3],	//			 至　　　  ［　　〃　　］		szsnk->Sn_KDAYE
					Sn_MDAYS[3],	// 中間申告の計算期間  ［　　〃　　］		szsnk->Sn_MDAYS
					Sn_MDAYE[3],	//					   ［　　〃　　］		szsnk->Sn_MDAYE
					Sn_PRDAY[3],	// 提出年月日　　　　　［ＢＣＤ６桁］		szsnk->Sn_PRDAY
					Sn_SPECIAL,		// D0:ON:簡易計算する						szsnk->Sn_SPECIAL
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
//<-- SN_HEAD END

//--> SN_DATA START
	unsigned char	Sn_KSTDM[6],	// 課税標準額（自動車）　［ＢＩＮ１２桁］		szsnk->Sn_KSTDM
					Sn_KSTD[6],		// 　　〃　　（その他）　［ＢＩＮ１２桁］		szsnk->Sn_KSTD
					Sn_TIKAM[6],	// 対価の返還等をした金額（自動車）［〃］		szsnk->Sn_TIKAM
					Sn_TIKA[6],		// 　　　　　〃　　　　　（その他）［〃］		szsnk->Sn_TIKA
					Sn_SYTXM[6],	// 消費税額（自動車）　　［ＢＩＮ１２桁］		szsnk->Sn_SYTXM
					Sn_SYTX[6],		// 　　〃　（その他）　　［　　 〃 　　］		szsnk->Sn_SYTX

					Sn_KJADJZ[6],	// 控除過大調整税額・一般［　　 〃 　　］		szsnk->Sn_KJADJZ
					             	// 貸倒回収に係税額・簡易［　　 〃 　　］

					Sn_SIREZ[6],	// 控除対象仕入税額　　　［　　 〃 　　］		szsnk->Sn_SIREZ
					Sn_HKANZ[6],	// 返還等対価に係る税額　［　　 〃 　　］		szsnk->Sn_HKANZ
					Sn_KSITZ[6],	// 貸倒れに係る税額　　　［　　 〃 　　］		szsnk->Sn_KSITZ

					Sn_TYNOFZ[6],	// 中間納付税額（年一回）［　　 〃 　　］		szsnk->Sn_TYNOFZ
					Sn_EDNOFZ[6],	// 最終納付税額　　　　　［　　 〃 　　］		szsnk->Sn_EDNOFZ
					Sn_KAKTIZ[6],	// 既確定税額　　　　　　［　　 〃 　　］		szsnk->Sn_KAKTIZ

					Sn_TTYWAR[6],	// 地方消費税　中間納付譲渡割額（年一回）		szsnk->Sn_TTYWAR
					Sn_TKAKTZ[6],	//     〃　　　既確定譲渡割額					szsnk->Sn_TKAKTZ
					Sn_TEDNOF[6],	//     〃　　　最終納付税額						szsnk->Sn_TEDNOF

					Sn_KZURI[6],	// 課税売上高　　　　　　［　　 〃 　　］		szsnk->Sn_KZURI
					Sn_SOURI[6],	// 総売上高　　　　　　　［　　 〃 　　］		szsnk->Sn_SOURI

					Sn_ZZNOFZ[6],	// 前々回納付税額　　　　［　　 〃 　　］		szsnk->Sn_ZZNOFZ
					Sn_ZNOFZ[6],	// 前回納付税額　　　　　［　　 〃 　　］		szsnk->Sn_ZNOFZ
					Sn_WURI[6],		// 課税売上高　　ＷＯＲＫ［　　 〃 　　］		szsnk->Sn_WURI
					Sn_ZZURI[6],	// 基準期間の課税売上高  ［　　 〃 　　］		szsnk->Sn_ZZURI
					Sn_ZURI[6],		// 前年の課税売上高　　  ［　　 〃 　　］		szsnk->Sn_ZURI

					Sn_KURI1[6],	// 第一種事業　課税売上　［　　 〃 　　］		szsnk->Sn_KURI[0]
					Sn_KURI2[6],	// 第二種事業　課税売上　［　　 〃　　 ］		szsnk->Sn_KURI[1]
					Sn_KURI3[6],	// 第三種事業　課税売上　［　　 〃 　　］		szsnk->Sn_KURI[2]
					Sn_KURI4[6],	// 第四種事業　課税売上  ［　　 〃 　　］		szsnk->Sn_KURI[3]
					Sn_KURI5[6],	// 第五種事業　課税売上  ［　　 〃 　　］		szsnk->Sn_KURI[4]

					Sn_GENKAI[6],	// 限界控除税額　　　　　［　　 〃 　　］		無し？？？
					Sn_WGENKA[6];	// 限界控除税額(手入力額)［　　 〃 　　］		無し？？？

	unsigned short	Sn_UP1,			// 第一種事業　売上割合							szsnk->Sn_UP[0]
					Sn_UP2,			// 第二種事業　売上割合							szsnk->Sn_UP[1]
					Sn_UP3,			// 第三種事業　売上割合							szsnk->Sn_UP[2]
					Sn_UP4,			// 第四種事業　売上割合							szsnk->Sn_UP[3]
					Sn_UP5;			// 第五種事業　売上割合							szsnk->Sn_UP[4]

	unsigned char	Sn_CK45[6],		// 自動車　課税標準額　４．５％					szsnk->Sn_CK45
					Sn_CZ45[6],		// 自動車　消費税額　　　〃						szsnk->Sn_CZ45
					Sn_CH45[6],		// 自動車　対価の返還額　〃						szsnk->Sn_CH45
					Sn_CK60[6],		// 自動車　課税標準額　６．０％					szsnk->Sn_CK60
					Sn_CZ60[6],		// 自動車　消費税額　　　〃						szsnk->Sn_CZ60
					Sn_CH60[6],		// 自動車　対価の返還額　〃						Sn_CH60
					Sn_KURIA[6],	// 事業別課税売上高合計額						szsnk->Sn_KURIA

					Sn_TYNF1[6],	// 中間納付税額	１回目	 ［ＢＩＮ１２桁］		szsnk->Sn_TYNF[0]
					Sn_TYNF2[6],	//     〃		２回目	 ［     〃	   ］		szsnk->Sn_TYNF[1]
					Sn_TYNF3[6],	//     〃		３回目	 ［     〃	   ］		szsnk->Sn_TYNF[2]
					Sn_TYNF4[6],	//     〃		４回目	 ［     〃	   ］		szsnk->Sn_TYNF[3]

					Sn_TTYNF1[6],	// 中間納付税額	１回目	 ［地方消費税用］		szsnk->Sn_TTYNF[0]
					Sn_TTYNF2[6],	//     〃		２回目	 ［     〃	   ］		szsnk->Sn_TTYNF[1]
					Sn_TTYNF3[6],	//     〃		３回目	 ［     〃	   ］		szsnk->Sn_TTYNF[2]
					Sn_TTYNF4[6],	//     〃		４回目	 ［     〃	   ］		szsnk->Sn_TTYNF[3]

					Sn_TYNF[12][6],	// 中間申告年１１回								szsnk->Sn_TYNF[]
					Sn_TTNF[12][6];	// 中間申告年１１回	地方消費税用				szsnk->Sn_TTYNF[]

	//	棚卸資産
	unsigned char	Sn_MNK40[6],	// 調整課税仕入等に係る棚卸資産額(新税率分)		szsnk->Sn_MNK40
					Sn_MNZ40[6],	//				 〃			  消費税額(〃)		szsnk->Sn_MNZ40
					Sn_MENZK[6],	// 調整課税仕入等に係る棚卸資産額				szsnk->Sn_MENZK
					Sn_MENZZ[6],	// 				 〃			   消費税額			szsnk->Sn_MENZZ
					Sn_mnk40[3][6],	// 調整課税仕入等に係る棚卸資産額(新税率分)		szsnk->Sn_mnk40[]
					Sn_mnz40[3][6],	//				 〃			  消費税額(〃)		szsnk->Sn_mnz40[]
					Sn_menzk[3][6],	// 調整課税仕入等に係る棚卸資産額				szsnk->Sn_menzk[]
					Sn_menzz[3][6],	// 				 〃			   消費税額			szsnk->Sn_mebzz[]
					Sn_MENZsw,		// 免税⇔課税切換ｻｲﾝ | | | |･| | | |･|			szsnk->Sn_MENZsw
									// 						  　↓	  　↓
									// 					  　切換ｱﾘ	 　 ↓
									// 			　　	ON:免税に OFF:課税に
					Sn_tansw;		// 棚卸仕訳発生サイン							szsnk->Sn_tansw

	// 申告書本表の内容 (16b) 
	unsigned char	Sn_ZeiHou,		// D0=ON:税理士法第３０条						szsnk->Sn_ZeiHou
									// D1=ON:税理士法第３３条の２
									// D2=ON:配賦不要
									// D3=
									// D4=ON:税理士法 施行前 OFF:税理士法 施行後			★廃止！
									// D5=ON:改訂前本表		 OFF:改訂後本表
									// D6=
									// D7=ON:消費税の還付申告に関する明細書	OFF:仕入控除税額に関する明細書
					Sn_Syuusei,		// D0=
									// D1=ON:更正の請求書							szsnk->Sn_Syuusei
									// D2=ON:更正の申出書　D1がONで有効
									// D3=
									// D4=
									// D5=
									// D6=
									// D7=
					Sn_EXP100,		// 課税売上高100％								szsnk->Sn_EXP100
					Sn_Sign1,		// D0=ON:印刷 仕入控除税額に関する明細書（次葉）	ON:消費税の還付申告に関する明細書の財務連動する	★廃止！		szsnk->Sn_Sign1
									// D1=ON:印刷 還付申告に関する明細書(1/2)
									// D2=ON:印刷 還付申告に関する明細書(2/2)
									// D3=ON:印刷 還付申告に関する明細書（次葉）	
									// D4=ON:印刷 更正の請求書（申出書）				ON:課税取引金額計算表の財務連動する				★廃止！
									// D5=ON:印刷 課税売上高計算表
									// D6=ON:印刷 課税仕入高計算表
									// D7=ON:課税取引金額計算表（事業所得）				電子申告済を印刷する							★下へ移動↓
									// 
					Sn_Sign2,		// D0=ON:特定実額収入集計テ－ブルを初期化する	szsnk->Sn_Sign2
									// D1=ON:特定収入が5％未満で対象外		
									// D2=
									// D3=
									// D4=ON:税理士法人名印刷サイン
									// D5=ON:期間短縮解除後、期間の開始以降で申告書集計をする
									// D6=ON:個別対応方式の税額を期間計で計算を行う			◆新規	
									// D7=ON:電子申告済を印刷する							◆上から移動↑
					Sn_Sign3,		// D0=ON:印刷　課税取引金額計算表　不動産事業者			◆新規				szsnk->Sn_Sign3
									// D1=ON:印刷　課税取引金額計算表　農業事業者			◆新規
									// D2=ON:印刷　特定収入計算表							◆新規			
									// D3=ON:印刷　
									// D4=ON:印刷　
									// D5=ON:印刷　
									// D6=ON:印刷　
									// D7=ON:印刷　
					Sn_Sign4;		// D0=ON:ロックで入力不可								◆新規				szsnk->Sn_Sign4
									// D1=
									// D2=
									// D3=
									// D4=ON:平成２３年消費税改正後で課税売上が５億円を超えている。
									// D5=ON:更新時、還付明細書を全て初期化する。			★廃止
									// D6=
									// D7=ON:非連動（申告書集計をしないで実額で入力を行う）	◆新規

	// 付表１及び４の内容 (118b)
	unsigned char	Sn_1F1B[6],		// 課税標準額 税率４％適用分 ・-Ｂ				szsnk->Sn_1F1B
					Sn_1F2A[6],		// 　　〃　　 旧税率適用分   ・-Ａ				szsnk->Sn_1F2A
					Sn_1F4B[6],		// 消費税額   税率４％適用分 ・-Ｂ				szsnk->Sn_1F4B
					Sn_1F5A[6],		// 　　〃　　 旧税率適用分   ・-Ａ				szsnk->Sn_1F5A
					Sn_1F7A[6],		// 貸倒回収に係る消費税額    ・-Ａ				szsnk->Sn_1F7A
					Sn_1F7B[6],		// 　　　　　〃　　　　　    ・-Ｂ				szsnk->Sn_1F7B
					Sn_1F8A[6],		// 控除対象仕入税額 		 ・-Ａ				szsnk->Sn_1F8A
					Sn_1F8B[6],		// 　　　 〃　　　　 		 ・-Ｂ				szsnk->Sn_1F8B
					Sn_1F8C[6],		// 　　　 〃　　　　 		 ・-Ｃ				szsnk->Sn_1F8C
					Sn_1F9A[6],		// 返還等対価に係る税額 	 ・-Ａ				szsnk->Sn_1F9A
					Sn_1F9B[6],		// 　　　　 〃　　　　　 	 ・-Ｂ				szsnk->Sn_1F9B
					Sn_1F9C[6],		// 　　　　 〃　　　　　 	 ・-Ｃ				szsnk->Sn_1F9C
					Sn_1F10A[6],	// 貸倒に係る税額 			 ・-Ａ				szsnk->Sn_1F10A
					Sn_1F10B[6],	// 　　　〃　　　 			 ・-Ｂ				szsnk->Sn_1F10B
					Sn_1F12B[6],	// 控除不足税額　 			 ・-Ｂ				szsnk->Sn_1F12B
					Sn_1F13B[6],	// 差引税額		　 			 ・-Ｂ				szsnk->Sn_1F13B
					Sn_1F14C[6];	// 合計差引税額　 			 ・-Ｃ				szsnk->Sn_1F14C

	// 付表２－(２)及び５－(２)の内容 (172b)
	unsigned char	Sn_5FURIK[6],	// 事業区分別課税売上高合計額(期間計)			szsnk->Sn_5FURIK
					Sn_5FURIT[6],	// 事業区分別課税売上高合計額(税率4%分)			szsnk->Sn_5FURIT
					Sn_5FURI1[6],	// 　　　　〃　　　　　第一種　　〃				szsnk->Sn_5FURI[0]
					Sn_5FURI2[6],	// 　　　　〃　　　　　第二種　　〃				szsnk->Sn_5FURI[1]
					Sn_5FURI3[6],	// 　　　　〃　　　　　第三種　　〃				szsnk->Sn_5FURI[2]
					Sn_5FURI4[6],	// 　　　　〃　　　　　第四種　　〃				szsnk->Sn_5FURI[3]
					Sn_5FURI5[6],	// 　　　　〃　　　　　第五種　　〃				szsnk->Sn_5FURI[4]
					Sn_5FUZTT[6],	// 事業区分別消費税額合計額(課税期間計)			szsnk->Sn_5FUZTT
					Sn_5FUZT1[6],	// 　　　　〃　　　　第一種　　  〃				szsnk->Sn_5FUZT[0]
					Sn_5FUZT2[6],	// 　　　　〃　　　　第二種　　　〃				szsnk->Sn_5FUZT[1]	
					Sn_5FUZT3[6],	// 　　　　〃　　　　第三種　　　〃				szsnk->Sn_5FUZT[2]	
					Sn_5FUZT4[6],	// 　　　　〃　　　　第四種　　　〃				szsnk->Sn_5FUZT[3]	
					Sn_5FUZT5[6],	// 　　　　〃　　　　第五種　　　〃				szsnk->Sn_5FUZT[4]	
					Sn_5FUZKT[6],	// 　　　　〃　　　　合計額（税率4%分）			szsnk->Sn_5FUZKT
					Sn_5FUZK1[6],	// 　　　　〃　　　　第一種　　　〃				szsnk->Sn_5FUZK[0]
					Sn_5FUZK2[6],	// 　　　　〃　　　　第二種　　　〃				szsnk->Sn_5FUZK[1]
					Sn_5FUZK3[6],	// 　　　　〃　　　　第三種　　　〃				szsnk->Sn_5FUZK[2]
					Sn_5FUZK4[6],	// 　　　　〃　　　　第四種　　　〃				szsnk->Sn_5FUZK[3]
					Sn_5FUZK5[6],	// 　　　　〃　　　　第五種　　　〃				szsnk->Sn_5FUZK[4]
					Sn_5FSZTG[6],	// 控除対象仕入税額　原則計算(期間計)			szsnk->Sn_5FSZTG
					Sn_5FSZTT[6],	// 　　　 〃　　　　 特例計算	 〃				szsnk->Sn_5FSZTT
					Sn_5FSZKG[6],	// 控除対象仕入税額　原則計算(税率4%分)			szsnk->Sn_5FSZKG
					Sn_5FSZKT[6],	// 　　　 〃　　　　 特例計算	 〃				szsnk->Sn_5FSZKT
					Sn_5Fdm1[6],	// リザーブ										szsnk->Sn_5Fdm1		簡易課税専用
					Sn_5Fdm2[6],	// リザーブ										szsnk->Sn_5Fdm2
					Sn_5Fdm3[6],	// リザーブ										szsnk->Sn_5Fdm3
					Sn_5Fdm4[6],	// リザーブ										szsnk->Sn_5Fdm4
					Sn_5FSZsw,		// 特例計算適用SW 0:原則計算 1:一種類特例		szsnk->Sn_5FSZsw
									// 2:第１+第２, 3:第1+第３, 4:第１+第４ 
									// 5:第１+第５, 6:第２+第３, 7:第２+第４
									// 8:第２+第５, 9:第３+第４,10:第３+第５
									// 11:第４+第５	
					Sn_5FSBsw,		// 特例計算適用SW (税率4%分)					szsnk->Sn_5FSBsw
					Sn_5FCQsw,		// 集計・計算済サイン							szsnk->Sn_5FCQsw
					Sn_5FGKsw,		// 原則計算⑱手入力サインD0:期間  D4:うち		szsnk->Sn_5FGKsw
					Sn_2F21sw;		// 控除対象仕入税額　手入力サイン D0: D4:		szsnk->Sn_2F21sw

	unsigned short	Sn_5FUP1,		// 第一種事業　売り上げ割合(税率4%分)			szsnk->Sn_5FUP[0]
					Sn_5FUP2,		// 第二種事業　　　　　　〃						szsnk->Sn_5FUP[1]
					Sn_5FUP3,		// 第三種事業　　　　　　〃						szsnk->Sn_5FUP[2]
					Sn_5FUP4,		// 第四種事業　　　　　　〃						szsnk->Sn_5FUP[3]
					Sn_5FUP5;		// 第五種事業　　　　　　〃						szsnk->Sn_5FUP[4]

	// 付表３の内容 (64b)															使用？？？
	unsigned char	Sn_3F1[6],		// 限界控除前の税額								使用？？？
					Sn_3F2[6],		// 限界控除税額
					Sn_3F3[6],		// 差引税額
					Sn_3F4[6],		// 地方消費税：課税標準の基礎金額
					Sn_3F5[6],		// 　　〃　　：限界控除税額相当額
					Sn_3F6[6],		// 　　〃　　：課税標準となる消費税額
					Sn_3FA[6],		// 現行法による限界控除税額	
					Sn_3FB[6],		// 改正法による　　 〃
					Sn_3FMON1,		// 限界控除 特例適用日前月数
					Sn_3FMON2,		// 限界控除 特例適用日後月数
					Sn_3FMON3,		// 平成９年４月以後の月数
					Sn_3Fdm[13];	// 限界控除 特例適用日後月数


	// 集計のための追加エリア (36b) 
	unsigned char	Sn_2F17A[6],	// 付表2｢17｣-A 個別対応控除税額手入力額			szsnk->Sn_2F17A
					Sn_2F17B[6],	// 付表2｢17｣-B 個別対応控除税額手入力額			szsnk->Sn_2F17B
					Sn_2F18A[6],	// 付表2｢18｣-A 比例配分控除税額手入力額			szsnk->Sn_2F18A
					Sn_2F18B[6],	// 付表2｢18｣-B 比例配分控除税額手入力額			szsnk->Sn_2F18B
					Sn_2F21A[6],	// 付表2｢21｣-A 控除対象仕入税額手入力額			szsnk->Sn_2F21A
					Sn_2F21B[6];	// 付表2｢21｣-B 控除対象仕入税額手入力額			szsnk->Sn_2F21B

// 前年実績による中間申告 (378B) [04'11.25]
	unsigned char	Sn_ZKDYS[3],	// 前期課税期間　自								szsnk->Sn_ZKDYS
					Sn_ZKDYE[3],	// 〃　　　　　　至								szsnk->Sn_ZKDYE
					Sn_ZZDAY[3],	// 修正・更正・決定の年月日						szsnk->Sn_ZZDAY
					Sn_BSMON,		// 月換算	分子								szsnk->Sn_BSMON
					Sn_BBMON,		// 〃　　　 分母								szsnk->Sn_BBMON
					Sn_KTNFZ[6],	// 消費税及び地方消費税の合計納付税額			szsnk->Sn_KTNFZ
					Sn_ZSKZN[6][12],// 国　税　この申告前の税額						szsnk->Sn_ZSKZN[]
					Sn_ZSKAD[6][12],// 〃　　　この申告により増加する税額			szsnk->Sn_ZSKAD[]
					Sn_ZSTZN[6][12],// 地方税　この申告前の税額						szsnk->Sn_ZSTZN[]
					Sn_ZSTAD[6][12],// 〃　　　この申告により増加する税額			szsnk->Sn_ZSTAD[]
					Sn_ZSKTZ[6][12];// 消費税及び地方消費税の合計納付税額			szsnk->Sn_ZSKTZ[]

	//	輸入仕入（拡張版）(101B)
	unsigned char	Sn_UNK40[6],	// 輸入仕入に係る仕入額(新税率分)				未使用？？？
					Sn_UNZ40[6],	//			 〃	 消費税額(〃)					未使用？？？
					Sn_UENZK[6],	// 輸入仕入に係る仕入額							未使用？？？
					Sn_UENZZ[6],	// 			 〃　消費税額						未使用？？？
					Sn_unk40[3][6],	// 輸入仕入に係る仕入額(新税率分)				未使用？？？
					Sn_unz40[3][6],	//			 〃　消費税額(〃)					未使用？？？
					Sn_uenzk[3][6],	// 輸入仕入に係る仕入額							未使用？？？
					Sn_uenzz[3][6],	// 			 〃　消費税額						未使用？？？
					Sn_usisw;		// 輸入仕入仕訳発生サイン						未使用？？？

	// 税務署コード	[08'12.02]
	unsigned char	Sn_TAXNO[5];	// 税務署コード									szsnk->Sn_KSNO
//<-- SN_DATA END

//--> SN_PPRT START
	// 欄外印字項目　定義	[05'05.06]
	unsigned char	Sn_ADRU1[30],	// 上段①　左									szsnk->Sn_ADRU1
					Sn_ADRU2[20],	// 上段①　右									szsnk->Sn_ADRU2
					Sn_ADRC1[20],	// 上段②　左									szsnk->Sn_ADRC1
					Sn_ADRC2[20],	// 上段②　右									szsnk->Sn_ADRC2
					Sn_ADRD1[30],	// 下段①　左									szsnk->Sn_ADRD1
					Sn_ADRD2[16],	// 下段②　右									szsnk->Sn_ADRD2
					Sn_PPVER,		// 拡張サイン									szsnk->Sn_PPVER
					Sn_PPTYP,		// 特殊処理サイン D0:ON 個人を印刷				szsnk->Sn_PPTYP
					Sn_ADRD3[14],	// 下段①左の１４文字拡張分						szsnk->Sn_ADRD3
					Sn_ADKUP[10],	// 上段左番号１０桁（数字）						szsnk->Sn_ADKUP
					Sn_ADKDW[10],	// 下段左番号１０桁（数字）						szsnk->Sn_ADKDW
					Sn_ADKNO[10];	// 下段右番号　１桁（数字）						szsnk->Sn_ADKNO
	unsigned char	Sn_ADRD4[50],	// 下段①左の統一（下段①左＋下段①左の１４文字拡張分＋２桁）		szsnk->Sn_ADRD4[0]
									// Sn_PPVER D0: ON=上記拡張済み！		
					Sn_ADSNO[10],	// 署別一連番号７桁（数字）											szsnk->Sn_ADRD4[1]
					Sn_ADKDC[20];	// 下段右     [10'02.01]▼	個人専用　(szsnk->Sn_ADRD6)				未使用？？？	
			 char	Sn_ADDM1[244];	// 264 → 244 [10'02.01]▼											未使用？？？

//<-- SN_PPRT END

};