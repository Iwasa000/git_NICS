#pragma	once

//#define		JSKWIDECNT	12			//カラムの横の数
#define		JSKWIDECNT	16				//カラムの横の数(決算額～返還等課税取引の一行からはじまり、うち税率の分を含め、計１６項目。０からカウント)

#define		JSKLINCNT	10				//帳票の行数。この行数でデータを保存していってる。第２種農業を追加した分を追加(データ計１０項目。０からカウント)
//#define		JSKLINCNT	9				//帳票の行数。この行数でデータを保存していってる。カラム数(うち税率分も含めデータ計９項目。０からカウント)

#define		JSKVMIN		199				//入力ボックスの最小のインデックス
//#define		JSKVMIN		194			//入力ボックスの最小のインデックス
#define		JSKVMAX		346				//入力ボックスの最大のインデックス
//#define		JSKVMAX		337			//入力ボックスの最大のインデックス




#define		JSKZKBNID	1
#define		JSKDISP1	29　			//使われてない？？


//全てのIDX定義
#define		JSKVAL01A	199				//   第１種事業（卸売業）　決算額
#define		JSKVAL01B	200				//   第１種事業（卸売業）　課税取引外
#define		JSKVAL01C	201				//   第１種事業（卸売業）　課税取引金額
#define		JSKVAL01D	202				//   第１種事業（卸売業）　返還等課税取引
#define		JSKVAL02A	203				//   第１種事業（卸売業）　うち税率７．８％適用分　決算額
#define		JSKVAL02B	204				//   第１種事業（卸売業）　うち税率７．８％適用分　課税取引外
#define		JSKVAL02C	205				//   第１種事業（卸売業）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL02D	206				//   第１種事業（卸売業）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL03A	207				//   第１種事業（卸売業）　うち税率６．２４％適用分　決算額
#define		JSKVAL03B	208				//   第１種事業（卸売業）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL03C	209				//   第１種事業（卸売業）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL03D	210				//   第１種事業（卸売業）　うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL04A	211				//   第１種事業（卸売業）　うち税率６．３％適用分　決算額
#define		JSKVAL04B	212				//   第１種事業（卸売業）　うち税率６．３％適用分　課税取引外
#define		JSKVAL04C	213				//   第１種事業（卸売業）　うち税率６．３％適用分　課税取引金額
#define		JSKVAL04D	214				//   第１種事業（卸売業）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL05A	215				//   第２種事業（小売業等）　決算額
#define		JSKVAL05B	216				//   第２種事業（小売業等）　課税取引外
#define		JSKVAL05C	217				//   第２種事業（小売業等）　課税取引金額
#define		JSKVAL05D	218				//   第２種事業（小売業等）　返還等課税取引
#define		JSKVAL06A	219				//   第２種事業（小売業等）　うち税率７．８％適用分　決算額
#define		JSKVAL06B	220				//   第２種事業（小売業等）　うち税率７．８％適用分　課税取引外
#define		JSKVAL06C	221				//   第２種事業（小売業等）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL06D	222				//   第２種事業（小売業等）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL07A	223				//   第２種事業（小売業等）　うち税率６．２４％適用分　決算額
#define		JSKVAL07B	224				//   第２種事業（小売業等）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL07C	225				//   第２種事業（小売業等）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL07D	226				//   第２種事業（小売業等）　うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL08A	227				//   第２種事業（小売業等）　うち税率６．３％適用分　決算額
#define		JSKVAL08B	228				//   第２種事業（小売業等）　うち税率６．３％適用分　課税取引外
#define		JSKVAL08C	229				//   第２種事業（小売業等）　うち税率６．３％適用分　課税取引金額
#define		JSKVAL08D	230				//   第２種事業（小売業等）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL09A	231				//   第２種事業（小売業等）【農業事業者】決算額
#define		JSKVAL09B	232				//   第２種事業（小売業等）【農業事業者】課税取引外
#define		JSKVAL09C	233				//   第２種事業（小売業等）【農業事業者】課税取引金額
#define		JSKVAL09D	234				//   第２種事業（小売業等）【農業事業者】返還等課税取引
#define		JSKVAL10A	235				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　決算額
#define		JSKVAL10B	236				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　課税取引外
#define		JSKVAL10C	237				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　課税取引金額
#define		JSKVAL10D	238				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　返還等課税取引

#define		JSKVAL11A	239				//   第３種事業（製造業等）　決算額
#define		JSKVAL11B	240				//   第３種事業（製造業等）　課税取引外
#define		JSKVAL11C	241				//   第３種事業（製造業等）　課税取引金額
#define		JSKVAL11D	242				//   第３種事業（製造業等）　返還等課税取引
#define		JSKVAL12A	243				//   第３種事業（製造業等）　うち税率７．８％適用分　決算額
#define		JSKVAL12B	244				//   第３種事業（製造業等）　うち税率７．８％適用分　課税取引外
#define		JSKVAL12C	245				//   第３種事業（製造業等）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL12D	246				//   第３種事業（製造業等）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL13A	247				//   第３種事業（製造業等）　うち税率６．２４％適用分　決算額
#define		JSKVAL13B	248				//   第３種事業（製造業等）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL13C	249				//   第３種事業（製造業等）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL13D	250				//   第３種事業（製造業等）　うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL14A	251				//   第３種事業（製造業等）　うち税率６．３％適用分　決算額
#define		JSKVAL14B	252				//   第３種事業（製造業等）　うち税率６．３％適用分　課税取引外
#define		JSKVAL14C	253				//   第３種事業（製造業等）　うち税率６．３％適用分　課税取引金額
#define		JSKVAL14D	254				//   第３種事業（製造業等）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL15A	255				//   第３種事業（製造業等）【農業事業者】決算額
#define		JSKVAL15B	256				//   第３種事業（製造業等）【農業事業者】課税取引外
#define		JSKVAL15C	257				//   第３種事業（製造業等）【農業事業者】課税取引金額
#define		JSKVAL15D	258				//   第３種事業（製造業等）【農業事業者】返還等課税取引
#define		JSKVAL16A	259				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　決算額
#define		JSKVAL16B	260				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　課税取引外
#define		JSKVAL16C	261				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　課税取引金額
#define		JSKVAL16D	262				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　返還等課税取引
#define		JSKVAL17A	263				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　決算額
#define		JSKVAL17B	264				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　課税取引外
#define		JSKVAL17C	265				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　課税取引金額
#define		JSKVAL17D	266				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　返還等課税取引

#define		JSKVAL18A	267				//   第４種事業（その他）　決算額
#define		JSKVAL18B	268				//   第４種事業（その他）　課税取引外
#define		JSKVAL18C	269				//   第４種事業（その他）　課税取引金額
#define		JSKVAL18D	270				//   第４種事業（その他）　返還等課税取引
#define		JSKVAL19A	271				//   第４種事業（その他）　うち税率７．８％適用分　決算額
#define		JSKVAL19B	272				//   第４種事業（その他）　うち税率７．８％適用分　課税取引外
#define		JSKVAL19C	273				//   第４種事業（その他）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL19D	274				//   第４種事業（その他）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL20A	275				//   第４種事業（その他）　うち税率６．２４％適用分　決算額
#define		JSKVAL20B	276				//   第４種事業（その他）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL20C	277				//   第４種事業（その他）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL20D	278				//   第４種事業（その他）　うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL21A	279				//   第４種事業（その他）　うち税率６．３％適用分　決算額
#define		JSKVAL21B	280				//   第４種事業（その他）　うち税率６．３％適用分　課税取引外
#define		JSKVAL21C	281				//   第４種事業（その他）　うち税率６．３％適用分　課税取引金額
#define		JSKVAL21D	282				//   第４種事業（その他）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL22A	283				//   第４種事業（その他）【固定資産等の譲渡】決算額
#define		JSKVAL22B	284				//   第４種事業（その他）【固定資産等の譲渡】課税取引外
#define		JSKVAL22C	285				//   第４種事業（その他）【固定資産等の譲渡】課税取引金額
#define		JSKVAL22D	286				//   第４種事業（その他）【固定資産等の譲渡】返還等課税取引
#define		JSKVAL23A	287				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　決算額
#define		JSKVAL23B	288				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　課税取引外
#define		JSKVAL23C	289				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　課税取引金額
#define		JSKVAL23D	290				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　返還等課税取引
#define		JSKVAL24A	291				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　決算額
#define		JSKVAL24B	292				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　課税取引外
#define		JSKVAL24C	293				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　課税取引金額
#define		JSKVAL24D	294				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL25A	295				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　決算額
#define		JSKVAL25B	296				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　課税取引外
#define		JSKVAL25C	297				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　課税取引金額
#define		JSKVAL25D	298				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　返還等課税取引

#define		JSKVAL26A	299				//   第５種事業（サービス業等）　決算額
#define		JSKVAL26B	300				//   第５種事業（サービス業等）　課税取引外
#define		JSKVAL26C	301				//   第５種事業（サービス業等）　課税取引金額
#define		JSKVAL26D	302				//   第５種事業（サービス業等）　返還等課税取引
#define		JSKVAL27A	303				//   第５種事業（サービス業等）　うち税率７．８％適用分　決算額
#define		JSKVAL27B	304				//   第５種事業（サービス業等）　うち税率７．８％適用分　課税取引外
#define		JSKVAL27C	305				//   第５種事業（サービス業等）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL27D	306				//   第５種事業（サービス業等）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL28A	307				//   第５種事業（サービス業等）　うち税率６．２４％適用分　決算額
#define		JSKVAL28B	308				//   第５種事業（サービス業等）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL28C	309				//   第５種事業（サービス業等）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL28D	310				//   第５種事業（サービス業等）　うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL29A	311				//   第５種事業（サービス業等）　うち税率６．３％適用分　決算額
#define		JSKVAL29B	312				//   第５種事業（サービス業等）　うち税率６．３％適用分　課税取引外
#define		JSKVAL29C	313				//   第５種事業（サービス業等）　うち税率６．３％適用分　課税取引金額
#define		JSKVAL29D	314				//   第５種事業（サービス業等）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL30A	315				//   第６種事業（不動産業）　決算額
#define		JSKVAL30B	316				//   第６種事業（不動産業）　課税取引外
#define		JSKVAL30C	317				//   第６種事業（不動産業）　課税取引金額
#define		JSKVAL30D	318				//   第６種事業（不動産業）　返還等課税取引
#define		JSKVAL31A	319				//   第６種事業（不動産業）　うち税率７．８％適用分　決算額
#define		JSKVAL31B	320				//   第６種事業（不動産業）　うち税率７．８％適用分　課税取引外
#define		JSKVAL31C	321				//   第６種事業（不動産業）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL31D	322				//   第６種事業（不動産業）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL32A	323				//   第６種事業（不動産業）　うち税率６．２４％適用分　決算額
#define		JSKVAL32B	324				//   第６種事業（不動産業）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL32C	325				//   第６種事業（不動産業）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL32D	326				//   第６種事業（不動産業）　うち税率６．２４％適用分　返還等課税取引
#define		JSKVAL33A	327				//   第６種事業（不動産業）　うち税率６．３％適用分　決算額
#define		JSKVAL33B	328				//   第６種事業（不動産業）　うち税率６．３％適用分　課税取引外
#define		JSKVAL33C	329				//   第６種事業（不動産業）　うち税率６．３％適用分　課税取引金額
#define		JSKVAL33D	330				//   第６種事業（不動産業）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL34A	331				//   合計　決算額　
#define		JSKVAL34B	332				//   合計　課税取引外　
#define		JSKVAL34C	333				//   合計　課税取引金額　
#define		JSKVAL34D	334				//   合計　返還等課税取引　
#define		JSKVAL35A	335				//   合計　うち税率７．８％適用分　決算額　
#define		JSKVAL35B	336				//   合計　うち税率７．８％適用分　課税取引外　
#define		JSKVAL35C	337				//   合計　うち税率７．８％適用分　課税取引金額　
#define		JSKVAL35D	338				//   合計　うち税率７．８％適用分　返還等課税取引　
#define		JSKVAL36A	339				//   合計　うち税率６．２４％適用分　決算額　
#define		JSKVAL36B	340				//   合計　うち税率６．２４％適用分　課税取引外　
#define		JSKVAL36C	341				//   合計　うち税率６．２４％適用分　課税取引金額　
#define		JSKVAL36D	342				//   合計　うち税率６．２４％適用分　返還等課税取引　
#define		JSKVAL37A	343				//   合計　うち税率６．３％適用分　決算額　
#define		JSKVAL37B	344				//   合計　うち税率６．３％適用分　課税取引外　
#define		JSKVAL37C	345				//   合計　うち税率６．３％適用分　課税取引金額　
#define		JSKVAL37D	346				//   合計　うち税率６．３％適用分　返還等課税取引　


static SH_ITEMINDEX	ShinKniinp31[]	=	{
	{  JSKVAL01A, 0, 1, 0			},
	{  JSKVAL01B, 0, 1, 0			},
	{  JSKVAL01C, 0, 0, 0			},
	{  JSKVAL01D, 0, 1, 0			},
	{  JSKVAL02C, 0, 1, 0			},
	{  JSKVAL02D, 0, 1, 0			},
	{  JSKVAL03C, 0, 1, 0			},
	{  JSKVAL03D, 0, 1, 0			},
	{  JSKVAL04C, 0, 3, 0			},
	{  JSKVAL04D, 0, 3, 0			},

	{  JSKVAL05A, 0, 1, 0			},
	{  JSKVAL05B, 0, 1, 0			},
	{  JSKVAL05C, 0, 0, 0			},
	{  JSKVAL05D, 0, 1, 0			},
	{  JSKVAL06C, 0, 1, 0			},
	{  JSKVAL06D, 0, 1, 0			},	
	{  JSKVAL07C, 0, 1, 0			},
	{  JSKVAL07D, 0, 1, 0			},
	{  JSKVAL08C, 0, 3, 0			},
	{  JSKVAL08D, 0, 3, 0			},

	{  JSKVAL09A, 0, 1, 0			},
	{  JSKVAL09B, 0, 1, 0			},
	{  JSKVAL09C, 0, 0, 0			},
	{  JSKVAL09D, 0, 1, 0			},
	{  JSKVAL10C, 0, 1, 0			},
	{  JSKVAL10D, 0, 1, 0			},

	{  JSKVAL11A, 0, 1, 0			},
	{  JSKVAL11B, 0, 1, 0			},
	{  JSKVAL11C, 0, 0, 0			},
	{  JSKVAL11D, 0, 1, 0			},
	{  JSKVAL12C, 0, 1, 0			},
	{  JSKVAL12D, 0, 1, 0			},
	{  JSKVAL13C, 0, 1, 0			},
	{  JSKVAL13D, 0, 1, 0			},
	{  JSKVAL14C, 0, 3, 0			},
	{  JSKVAL14D, 0, 3, 0			},

	{  JSKVAL15A, 0, 1, 0			},
	{  JSKVAL15B, 0, 1, 0			},
	{  JSKVAL15C, 0, 0, 0			},
	{  JSKVAL15D, 0, 1, 0			},
	{  JSKVAL16C, 0, 1, 0			},
	{  JSKVAL16D, 0, 1, 0			},
	{  JSKVAL17C, 0, 3, 0			},
	{  JSKVAL17D, 0, 3, 0			},

	{  JSKVAL18A, 0, 1, 0			},
	{  JSKVAL18B, 0, 1, 0			},
	{  JSKVAL18C, 0, 0, 0			},
	{  JSKVAL18D, 0, 1, 0			},
	{  JSKVAL19C, 0, 1, 0			},
	{  JSKVAL19D, 0, 1, 0			},

	{  JSKVAL20A, 0, 0, 0			},
	{  JSKVAL20B, 0, 0, 0			},
	{  JSKVAL20C, 0, 1, 0			},
	{  JSKVAL20D, 0, 1, 0			},
	{  JSKVAL21A, 0, 0, 0			},
	{  JSKVAL21B, 0, 0, 0			},
	{  JSKVAL21C, 0, 3, 0			},
	{  JSKVAL21D, 0, 3, 0			},

	{  JSKVAL22A, 0, 1, 0			},
	{  JSKVAL22B, 0, 1, 0			},
	{  JSKVAL22C, 0, 0, 0			},
	{  JSKVAL23C, 0, 1, 0			},
	{  JSKVAL24A, 0, 0, 0			},
	{  JSKVAL24B, 0, 0, 0			},
	{  JSKVAL24C, 0, 1, 0			},
	{  JSKVAL24D, 0, 0, 0			},
	{  JSKVAL25A, 0, 0, 0			},
	{  JSKVAL25B, 0, 0, 0			},
	{  JSKVAL25C, 0, 3, 0			},
	{  JSKVAL25D, 0, 0, 0			},

	{  JSKVAL26A, 0, 1, 0			},
	{  JSKVAL26B, 0, 1, 0			},
	{  JSKVAL26C, 0, 0, 0			},
	{  JSKVAL26D, 0, 1, 0			},
	{  JSKVAL27C, 0, 1, 0			},
	{  JSKVAL27D, 0, 1, 0			},
	{  JSKVAL28A, 0, 0, 0			},
	{  JSKVAL28B, 0, 0, 0			},
	{  JSKVAL28C, 0, 1, 0			},
	{  JSKVAL28D, 0, 1, 0			},
	{  JSKVAL29A, 0, 0, 0			},
	{  JSKVAL29B, 0, 0, 0			},
	{  JSKVAL29C, 0, 3, 0			},
	{  JSKVAL29D, 0, 3, 0			},

	{  JSKVAL30A, 0, 1, 0			},
	{  JSKVAL30B, 0, 1, 0			},
	{  JSKVAL30C, 0, 0, 0			},
	{  JSKVAL30D, 0, 1, 0			},
	{  JSKVAL31C, 0, 1, 0			},
	{  JSKVAL31D, 0, 1, 0			},
	{  JSKVAL32C, 0, 1, 0			},
	{  JSKVAL32D, 0, 1, 0			},
	{  JSKVAL33C, 0, 3, 0			},
	{  JSKVAL33D, 0, 3, 0			},

	{	0, 0, 0, 0					}
};

static FCSUPDOWN_INFO	FCS_ShinKniinp31[]	=	{
	{	JSKVAL01A 		},
	{	JSKVAL02A 		},
	{	JSKVAL03A 		},
	{	JSKVAL04A 		},
	{	JSKVAL05A 		},
	{	JSKVAL06A 		},
	{	JSKVAL07A 		},
	{	JSKVAL08A 		},
	{	JSKVAL09A 		},
	{	JSKVAL10A 		},

	{	JSKVAL11A 		},
	{	JSKVAL12A 		},
	{	JSKVAL13A 		},
	{	JSKVAL14A 		},
	{	JSKVAL15A 		},
	{	JSKVAL16A 		},
	{	JSKVAL17A 		},
	{	JSKVAL18A 		},
	{	JSKVAL19A 		},
	{	JSKVAL20A 		},

	{	JSKVAL21A 		},
	{	JSKVAL22A 		},
	{	JSKVAL23A 		},
	{	JSKVAL24A 		},
	{	JSKVAL25A 		},
	{	JSKVAL26A 		},
	{	JSKVAL27A 		},
	{	JSKVAL28A 		},
	{	JSKVAL29A 		},
	{	JSKVAL30A 		},

	{	JSKVAL31A 		},
	{	JSKVAL32A 		},
	{	JSKVAL33A 		},
	{	JSKVAL01B 		},
	{	JSKVAL02B 		},
	{	JSKVAL03B		},
	{	JSKVAL04B 		},
	{	JSKVAL05B 		},
	{	JSKVAL06B 		},
	{	JSKVAL07B 		},

	{	JSKVAL08B 		},
	{	JSKVAL09B 		},
	{	JSKVAL10B 		},
	{	JSKVAL11B 		},
	{	JSKVAL12B 		},
	{	JSKVAL13B 		},
	{	JSKVAL14B 		},
	{	JSKVAL15B 		},
	{	JSKVAL16B 		},
	{	JSKVAL17B 		},

	{	JSKVAL18B 		},
	{	JSKVAL19B 		},
	{	JSKVAL20B 		},
	{	JSKVAL21B 		},
	{	JSKVAL22B 		},
	{	JSKVAL23B 		},
	{	JSKVAL24B 		},
	{	JSKVAL25B 		},
	{	JSKVAL26B 		},
	{	JSKVAL27B 		},

	{	JSKVAL28B 		},
	{	JSKVAL29B 		},
	{	JSKVAL30B 		},
	{	JSKVAL31B 		},
	{	JSKVAL32B 		},
	{	JSKVAL33B 		},
	{	JSKVAL01C 		},
	{	JSKVAL02C 		},
	{	JSKVAL03C		},
	{	JSKVAL04C 		},

	{	JSKVAL05C 		},
	{	JSKVAL06C		},
	{	JSKVAL07C 		},
	{	JSKVAL08C 		},
	{	JSKVAL09C 		},
	{	JSKVAL10C 		},
	{	JSKVAL11C 		},
	{	JSKVAL12C 		},
	{	JSKVAL13C 		},
	{	JSKVAL14C 		},

	{	JSKVAL15C 		},
	{	JSKVAL16C 		},
	{	JSKVAL17C 		},
	{	JSKVAL18C 		},
	{	JSKVAL19C 		},
	{	JSKVAL20C 		},
	{	JSKVAL21C 		},
	{	JSKVAL22C 		},
	{	JSKVAL23C 		},
	{	JSKVAL24C 		},

	{	JSKVAL25C 		},
	{	JSKVAL26C 		},
	{	JSKVAL27C 		},
	{	JSKVAL28C 		},
	{	JSKVAL29C 		},
	{	JSKVAL30C 		},
	{	JSKVAL31C 		},
	{	JSKVAL32C 		},
	{	JSKVAL33C 		},
	{	JSKVAL01D 		},

	{	JSKVAL02D 		},
	{	JSKVAL03D		},
	{	JSKVAL04D 		},
	{	JSKVAL05D 		},
	{	JSKVAL06D 		},
	{	JSKVAL07D 		},
	{	JSKVAL08D 		},
	{	JSKVAL09D 		},
	{	JSKVAL10D 		},
	{	JSKVAL11D 		},

	{	JSKVAL12D 		},
	{	JSKVAL13D 		},
	{	JSKVAL14D 		},
	{	JSKVAL15D 		},
	{	JSKVAL16D 		},
	{	JSKVAL17D 		},
	{	JSKVAL18D 		},
	{	JSKVAL19D 		},
	{	JSKVAL20D 		},
	{	JSKVAL21D 		},

	{	JSKVAL26D 		},
	{	JSKVAL27D 		},
	{	JSKVAL28D 		},
	{	JSKVAL29D 		},
	{	JSKVAL30D 		},
	{	JSKVAL31D 		},
	{	JSKVAL32D 		},
	{	JSKVAL33D 		},
};//計１２８

static FCSUPDOWN_INFO	FCS_ShinKniinp31_LR[]	=	{
	{	JSKVAL01A 		},
	{	JSKVAL01B 		},
	{	JSKVAL01C 		},
	{	JSKVAL01D 		},
	{	JSKVAL02A 		},
	{	JSKVAL02B 		},
	{	JSKVAL02C 		},
	{	JSKVAL02D 		},
	{	JSKVAL03A 		},
	{	JSKVAL03B 		},

	{	JSKVAL03C 		},
	{	JSKVAL03D 		},
	{	JSKVAL04A 		},
	{	JSKVAL04B 		},
	{	JSKVAL04C 		},
	{	JSKVAL04D 		},
	{	JSKVAL05A 		},
	{	JSKVAL05B 		},
	{	JSKVAL05C 		},
	{	JSKVAL05D 		},

	{	JSKVAL06A 		},
	{	JSKVAL06B 		},
	{	JSKVAL06C 		},
	{	JSKVAL06D 		},
	{	JSKVAL07A 		},
	{	JSKVAL07B 		},
	{	JSKVAL07C 		},
	{	JSKVAL07D 		},
	{	JSKVAL08A 		},
	{	JSKVAL08B 		},

	{	JSKVAL08C 		},
	{	JSKVAL08D 		},
	{	JSKVAL09A 		},
	{	JSKVAL09B 		},
	{	JSKVAL09C 		},
	{	JSKVAL09D 		},
	{	JSKVAL10A 		},
	{	JSKVAL10B 		},
	{	JSKVAL10C 		},
	{	JSKVAL10D 		},

	{	JSKVAL11A 		},
	{	JSKVAL11B 		},
	{	JSKVAL11C 		},
	{	JSKVAL11D 		},
	{	JSKVAL12A 		},
	{	JSKVAL12B 		},
	{	JSKVAL12C 		},
	{	JSKVAL12D 		},
	{	JSKVAL13A 		},
	{	JSKVAL13B 		},

	{	JSKVAL13C 		},
	{	JSKVAL13D 		},
	{	JSKVAL14A 		},
	{	JSKVAL14B 		},
	{	JSKVAL14C 		},
	{	JSKVAL14D 		},
	{	JSKVAL15A 		},
	{	JSKVAL15B 		},
	{	JSKVAL15C 		},
	{	JSKVAL15D 		},

	{	JSKVAL16A 		},
	{	JSKVAL16B 		},
	{	JSKVAL16C 		},
	{	JSKVAL16D 		},
	{	JSKVAL17A 		},
	{	JSKVAL17B 		},
	{	JSKVAL17C 		},
	{	JSKVAL17D 		},
	{	JSKVAL18A 		},
	{	JSKVAL18B 		},

	{	JSKVAL18C 		},
	{	JSKVAL18D 		},
	{	JSKVAL19A 		},
	{	JSKVAL19B 		},
	{	JSKVAL19C 		},
	{	JSKVAL19D 		},
	{	JSKVAL20A 		},
	{	JSKVAL20B 		},
	{	JSKVAL20C 		},
	{	JSKVAL20D 		},

	{	JSKVAL21A 		},
	{	JSKVAL21B 		},
	{	JSKVAL21C 		},
	{	JSKVAL21D 		},
	{	JSKVAL22A 		},
	{	JSKVAL22B 		},
	{	JSKVAL22C 		},
	{	JSKVAL23A 		},
	{	JSKVAL23B 		},
	{	JSKVAL23C 		},

	{	JSKVAL24A 		},
	{	JSKVAL24B 		},
	{	JSKVAL24C 		},
	{	JSKVAL25A 		},
	{	JSKVAL25B 		},
	{	JSKVAL25C 		},
	{	JSKVAL26A 		},
	{	JSKVAL26B 		},
	{	JSKVAL26C 		},
	{	JSKVAL26D 		},

	{	JSKVAL27A 		},
	{	JSKVAL27B 		},
	{	JSKVAL27C 		},
	{	JSKVAL27D 		},	
	{	JSKVAL28A 		},
	{	JSKVAL28B 		},
	{	JSKVAL28C 		},
	{	JSKVAL28D 		},	
	{	JSKVAL29A 		},
	{	JSKVAL29B 		},

	{	JSKVAL29C 		},
	{	JSKVAL29D 		},	
	{	JSKVAL30A 		},
	{	JSKVAL30B 		},
	{	JSKVAL30C 		},
	{	JSKVAL30D 		},	
	{	JSKVAL31A 		},
	{	JSKVAL31B 		},
	{	JSKVAL31C 		},
	{	JSKVAL31D 		},	

	{	JSKVAL32A 		},
	{	JSKVAL32B 		},
	{	JSKVAL32C 		},
	{	JSKVAL32D 		},	
	{	JSKVAL33A 		},
	{	JSKVAL33B 		},
	{	JSKVAL33C 		},
	{	JSKVAL33D 		},	
};//計１２８

#define		FCS_SHINKNIINP31CNT	128 //横、縦、数同じなので共同でしてる。別になれば分ける必要あり

//これらはデータ保存のインデックスにも使用されてるっぽい為、数値はそのまま引き継ぎ、第２種農業だけ追加する形とする
#define		KKNIINPLINE1			0	//第一種
#define		KKNIINPLINE2			1	//第二種
#define		KKNIINPLINE2_NOUGYO		9	//第二種　農業事業者
#define		KKNIINPLINE3			2	//第三種
#define		KKNIINPLINE3_NOUGYO		3	//第三種　農業事業者
#define		KKNIINPLINE4			4	//第四種
#define		KKNIINPLINE4_KOTEI		8	//第四種　固定資産等の譲渡
#define		KKNIINPLINE5			5	//第五種
#define		KKNIINPLINE6			6	//第六種　不動産業
#define		KKNIINPLINE_KEI			7	//合計

/*このタイミングで整理した
#define		KKNIINPLINE1		0	//第一種
#define		KKNIINPLINE2		1	//第二種
#define		KKNIINPLINE3		2	//第三種
#define		KKNIINPLINE4		3	//農業事業者？(経過措置ありでは不使用) → 経過措置でも使用 2016.10.20
#define		KKNIINPLINE5		4	//第四種
#define		KKNIINPLINE6		8	//固定資産等の譲渡
#define		KKNIINPLINE7		5	//第五種
#define		KKNIINPLINE8		6	//不動産事業者
#define		KKNIINPLINE9		7	//合計
*/