#pragma	once

//#define		JSKWIDECNT	12			//カラムの横の数
//#define		JSKWIDECNT	16				//カラムの横の数(決算額～返還等課税取引の一行からはじまり、うち税率の分を含め、計１６項目。０からカウント)
#define		JSKWIDECNTex	12				//カラムの横の数(決算額～返還等課税取引の一行からはじまり、うち税率の分を含め、計１６項目。０からカウント)

#define		JSKLINCNT	10				//帳票の行数。この行数でデータを保存していってる。第２種農業を追加した分を追加(データ計１０項目。０からカウント)
//#define		JSKLINCNT	9				//帳票の行数。この行数でデータを保存していってる。カラム数(うち税率分も含めデータ計９項目。０からカウント)

//#define		JSKVMIN		199				//入力ボックスの最小のインデックス
#define		JSKVMINex		154				//入力ボックスの最小のインデックス
//#define		JSKVMIN		194			//入力ボックスの最小のインデックス
//#define		JSKVMAX		346				//入力ボックスの最大のインデックス
#define		JSKVMAXex		265				//入力ボックスの最大のインデックス
//#define		JSKVMAX		337			//入力ボックスの最大のインデックス




#define		JSKZKBNID	1
//#define		JSKDISP1	29　			//使われてない？？


//全てのIDX定義
#define		JSKVAL01Aex	154				//   第１種事業（卸売業）　決算額
#define		JSKVAL01Bex	155				//   第１種事業（卸売業）　課税取引外
#define		JSKVAL01Cex	156				//   第１種事業（卸売業）　課税取引金額
#define		JSKVAL01Dex	157				//   第１種事業（卸売業）　返還等課税取引
#define		JSKVAL02Aex	158				//   第１種事業（卸売業）　うち税率７．８％適用分　決算額
#define		JSKVAL02Bex	159				//   第１種事業（卸売業）　うち税率７．８％適用分　課税取引外
#define		JSKVAL02Cex	160				//   第１種事業（卸売業）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL02Dex	161				//   第１種事業（卸売業）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL03Aex	162				//   第１種事業（卸売業）　うち税率６．２４％適用分　決算額
#define		JSKVAL03Bex	163				//   第１種事業（卸売業）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL03Cex	164				//   第１種事業（卸売業）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL03Dex	165				//   第１種事業（卸売業）　うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL04A	211				//   第１種事業（卸売業）　うち税率６．３％適用分　決算額
//#define		JSKVAL04B	212				//   第１種事業（卸売業）　うち税率６．３％適用分　課税取引外
//#define		JSKVAL04C	213				//   第１種事業（卸売業）　うち税率６．３％適用分　課税取引金額
//#define		JSKVAL04D	214				//   第１種事業（卸売業）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL05Aex	166				//   第２種事業（小売業等）　決算額
#define		JSKVAL05Bex	167				//   第２種事業（小売業等）　課税取引外
#define		JSKVAL05Cex	168				//   第２種事業（小売業等）　課税取引金額
#define		JSKVAL05Dex	169				//   第２種事業（小売業等）　返還等課税取引
#define		JSKVAL06Aex	170				//   第２種事業（小売業等）　うち税率７．８％適用分　決算額
#define		JSKVAL06Bex	171				//   第２種事業（小売業等）　うち税率７．８％適用分　課税取引外
#define		JSKVAL06Cex	172				//   第２種事業（小売業等）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL06Dex	173				//   第２種事業（小売業等）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL07Aex	174				//   第２種事業（小売業等）　うち税率６．２４％適用分　決算額
#define		JSKVAL07Bex	175				//   第２種事業（小売業等）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL07Cex	176				//   第２種事業（小売業等）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL07Dex	177				//   第２種事業（小売業等）　うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL08A	227				//   第２種事業（小売業等）　うち税率６．３％適用分　決算額
//#define		JSKVAL08B	228				//   第２種事業（小売業等）　うち税率６．３％適用分　課税取引外
//#define		JSKVAL08C	229				//   第２種事業（小売業等）　うち税率６．３％適用分　課税取引金額
//#define		JSKVAL08D	230				//   第２種事業（小売業等）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL09Aex	178				//   第２種事業（小売業等）【農業事業者】決算額
#define		JSKVAL09Bex	179				//   第２種事業（小売業等）【農業事業者】課税取引外
#define		JSKVAL09Cex	180				//   第２種事業（小売業等）【農業事業者】課税取引金額
#define		JSKVAL09Dex	181				//   第２種事業（小売業等）【農業事業者】返還等課税取引
#define		JSKVAL10Aex	182				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　決算額
#define		JSKVAL10Bex	183				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　課税取引外
#define		JSKVAL10Cex	184				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　課税取引金額
#define		JSKVAL10Dex	185				//   第２種事業（小売業等）【農業事業者】うち税率６．２４％適用分　返還等課税取引

#define		JSKVAL11Aex	186				//   第３種事業（製造業等）　決算額
#define		JSKVAL11Bex	187				//   第３種事業（製造業等）　課税取引外
#define		JSKVAL11Cex	188				//   第３種事業（製造業等）　課税取引金額
#define		JSKVAL11Dex	189				//   第３種事業（製造業等）　返還等課税取引
#define		JSKVAL12Aex	190				//   第３種事業（製造業等）　うち税率７．８％適用分　決算額
#define		JSKVAL12Bex	191				//   第３種事業（製造業等）　うち税率７．８％適用分　課税取引外
#define		JSKVAL12Cex	192				//   第３種事業（製造業等）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL12Dex	193				//   第３種事業（製造業等）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL13Aex	194				//   第３種事業（製造業等）　うち税率６．２４％適用分　決算額
#define		JSKVAL13Bex	195				//   第３種事業（製造業等）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL13Cex	196				//   第３種事業（製造業等）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL13Dex	197				//   第３種事業（製造業等）　うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL14A	251				//   第３種事業（製造業等）　うち税率６．３％適用分　決算額
//#define		JSKVAL14B	252				//   第３種事業（製造業等）　うち税率６．３％適用分　課税取引外
//#define		JSKVAL14C	253				//   第３種事業（製造業等）　うち税率６．３％適用分　課税取引金額
//#define		JSKVAL14D	254				//   第３種事業（製造業等）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL15Aex	198				//   第３種事業（製造業等）【農業事業者】決算額
#define		JSKVAL15Bex	199				//   第３種事業（製造業等）【農業事業者】課税取引外
#define		JSKVAL15Cex	200				//   第３種事業（製造業等）【農業事業者】課税取引金額
#define		JSKVAL15Dex	201				//   第３種事業（製造業等）【農業事業者】返還等課税取引
#define		JSKVAL16Aex	202				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　決算額
#define		JSKVAL16Bex	203				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　課税取引外
#define		JSKVAL16Cex	204				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　課税取引金額
#define		JSKVAL16Dex	205				//   第３種事業（製造業等）【農業事業者】うち税率７．８％適用分　返還等課税取引
//#define		JSKVAL17A	263				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　決算額
//#define		JSKVAL17B	264				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　課税取引外
//#define		JSKVAL17C	265				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　課税取引金額
//#define		JSKVAL17D	266				//   第３種事業（製造業等）【農業事業者】うち税率６．３％適用分　返還等課税取引

#define		JSKVAL18Aex	206				//   第４種事業（その他）　決算額
#define		JSKVAL18Bex	207				//   第４種事業（その他）　課税取引外
#define		JSKVAL18Cex	208				//   第４種事業（その他）　課税取引金額
#define		JSKVAL18Dex	209				//   第４種事業（その他）　返還等課税取引
#define		JSKVAL19Aex	210				//   第４種事業（その他）　うち税率７．８％適用分　決算額
#define		JSKVAL19Bex	211				//   第４種事業（その他）　うち税率７．８％適用分　課税取引外
#define		JSKVAL19Cex	212				//   第４種事業（その他）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL19Dex	213				//   第４種事業（その他）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL20Aex	214				//   第４種事業（その他）　うち税率６．２４％適用分　決算額
#define		JSKVAL20Bex	215				//   第４種事業（その他）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL20Cex	216				//   第４種事業（その他）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL20Dex	217				//   第４種事業（その他）　うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL21A	279				//   第４種事業（その他）　うち税率６．３％適用分　決算額
//#define		JSKVAL21B	280				//   第４種事業（その他）　うち税率６．３％適用分　課税取引外
//#define		JSKVAL21C	281				//   第４種事業（その他）　うち税率６．３％適用分　課税取引金額
//#define		JSKVAL21D	282				//   第４種事業（その他）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL22Aex	218				//   第４種事業（その他）【固定資産等の譲渡】決算額
#define		JSKVAL22Bex	219				//   第４種事業（その他）【固定資産等の譲渡】課税取引外
#define		JSKVAL22Cex	220				//   第４種事業（その他）【固定資産等の譲渡】課税取引金額
#define		JSKVAL22Dex	221				//   第４種事業（その他）【固定資産等の譲渡】返還等課税取引
#define		JSKVAL23Aex	222				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　決算額
#define		JSKVAL23Bex	223				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　課税取引外
#define		JSKVAL23Cex	224				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　課税取引金額
#define		JSKVAL23Dex	225				//   第４種事業（その他）【固定資産等の譲渡】うち税率７．８％適用分　返還等課税取引
#define		JSKVAL24Aex	226				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　決算額
#define		JSKVAL24Bex	227				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　課税取引外
#define		JSKVAL24Cex	228				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　課税取引金額
#define		JSKVAL24Dex	229				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL25A	295				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　決算額
//#define		JSKVAL25B	296				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　課税取引外
//#define		JSKVAL25C	297				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　課税取引金額
//#define		JSKVAL25D	298				//   第４種事業（その他）【固定資産等の譲渡】うち税率６．３％適用分　返還等課税取引

#define		JSKVAL26Aex	230				//   第５種事業（サービス業等）　決算額
#define		JSKVAL26Bex	231				//   第５種事業（サービス業等）　課税取引外
#define		JSKVAL26Cex	232				//   第５種事業（サービス業等）　課税取引金額
#define		JSKVAL26Dex	233				//   第５種事業（サービス業等）　返還等課税取引
#define		JSKVAL27Aex	234				//   第５種事業（サービス業等）　うち税率７．８％適用分　決算額
#define		JSKVAL27Bex	235				//   第５種事業（サービス業等）　うち税率７．８％適用分　課税取引外
#define		JSKVAL27Cex	236				//   第５種事業（サービス業等）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL27Dex	237				//   第５種事業（サービス業等）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL28Aex	238				//   第５種事業（サービス業等）　うち税率６．２４％適用分　決算額
#define		JSKVAL28Bex	239				//   第５種事業（サービス業等）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL28Cex	240				//   第５種事業（サービス業等）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL28Dex	241				//   第５種事業（サービス業等）　うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL29A	311				//   第５種事業（サービス業等）　うち税率６．３％適用分　決算額
//#define		JSKVAL29B	312				//   第５種事業（サービス業等）　うち税率６．３％適用分　課税取引外
//#define		JSKVAL29C	313				//   第５種事業（サービス業等）　うち税率６．３％適用分　課税取引金額
//#define		JSKVAL29D	314				//   第５種事業（サービス業等）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL30Aex	242				//   第６種事業（不動産業）　決算額
#define		JSKVAL30Bex	243				//   第６種事業（不動産業）　課税取引外
#define		JSKVAL30Cex	244				//   第６種事業（不動産業）　課税取引金額
#define		JSKVAL30Dex	245				//   第６種事業（不動産業）　返還等課税取引
#define		JSKVAL31Aex	246				//   第６種事業（不動産業）　うち税率７．８％適用分　決算額
#define		JSKVAL31Bex	247				//   第６種事業（不動産業）　うち税率７．８％適用分　課税取引外
#define		JSKVAL31Cex	248				//   第６種事業（不動産業）　うち税率７．８％適用分　課税取引金額
#define		JSKVAL31Dex	249				//   第６種事業（不動産業）　うち税率７．８％適用分　返還等課税取引
#define		JSKVAL32Aex	250				//   第６種事業（不動産業）　うち税率６．２４％適用分　決算額
#define		JSKVAL32Bex	251				//   第６種事業（不動産業）　うち税率６．２４％適用分　課税取引外
#define		JSKVAL32Cex	252				//   第６種事業（不動産業）　うち税率６．２４％適用分　課税取引金額
#define		JSKVAL32Dex	253				//   第６種事業（不動産業）　うち税率６．２４％適用分　返還等課税取引
//#define		JSKVAL33A	327				//   第６種事業（不動産業）　うち税率６．３％適用分　決算額
//#define		JSKVAL33B	328				//   第６種事業（不動産業）　うち税率６．３％適用分　課税取引外
//#define		JSKVAL33C	329				//   第６種事業（不動産業）　うち税率６．３％適用分　課税取引金額
//#define		JSKVAL33D	330				//   第６種事業（不動産業）　うち税率６．３％適用分　返還等課税取引

#define		JSKVAL34Aex	254				//   合計　決算額　
#define		JSKVAL34Bex	255				//   合計　課税取引外　
#define		JSKVAL34Cex	256				//   合計　課税取引金額　
#define		JSKVAL34Dex	257				//   合計　返還等課税取引　
#define		JSKVAL35Aex	258				//   合計　うち税率７．８％適用分　決算額　
#define		JSKVAL35Bex	259				//   合計　うち税率７．８％適用分　課税取引外　
#define		JSKVAL35Cex	260				//   合計　うち税率７．８％適用分　課税取引金額　
#define		JSKVAL35Dex	261				//   合計　うち税率７．８％適用分　返還等課税取引　
#define		JSKVAL36Aex	262				//   合計　うち税率６．２４％適用分　決算額　
#define		JSKVAL36Bex	263				//   合計　うち税率６．２４％適用分　課税取引外　
#define		JSKVAL36Cex	264				//   合計　うち税率６．２４％適用分　課税取引金額　
#define		JSKVAL36Dex	265				//   合計　うち税率６．２４％適用分　返還等課税取引　
//#define		JSKVAL37A	343				//   合計　うち税率６．３％適用分　決算額　
//#define		JSKVAL37B	344				//   合計　うち税率６．３％適用分　課税取引外　
//#define		JSKVAL37C	345				//   合計　うち税率６．３％適用分　課税取引金額　
//#define		JSKVAL37D	346				//   合計　うち税率６．３％適用分　返還等課税取引　

// 22-0167,22-0361 add -->
#define		JSSELChkTtl		305		// 自動計算チェック	タイトル
#define		JSSELChk2ex		306		// 自動計算チェック	うち標準税率7.8%適用分
#define		JSSELChk3ex		307		// 　　　 〃 　　　	うち軽減税率6.24%適用分
// 22-0167,22-0361 add <--

static SH_ITEMINDEX	ShinKniinp31Ex[]	=	{
	{  JSKVAL01Aex, 0, 1, 0			},
	{  JSKVAL01Bex, 0, 1, 0			},
	{  JSKVAL01Cex, 0, 0, 0			},
	{  JSKVAL01Dex, 0, 1, 0			},
	{  JSKVAL02Cex, 0, 1, 0			},
	{  JSKVAL02Dex, 0, 1, 0			},
	{  JSKVAL03Cex, 0, 1, 0			},
	{  JSKVAL03Dex, 0, 1, 0			},
	//{  JSKVAL04C, 0, 3, 0			},
	//{  JSKVAL04D, 0, 3, 0			},

	{  JSKVAL05Aex, 0, 1, 0			},
	{  JSKVAL05Bex, 0, 1, 0			},
	{  JSKVAL05Cex, 0, 0, 0			},
	{  JSKVAL05Dex, 0, 1, 0			},
	{  JSKVAL06Cex, 0, 1, 0			},
	{  JSKVAL06Dex, 0, 1, 0			},	
	{  JSKVAL07Cex, 0, 1, 0			},
	{  JSKVAL07Dex, 0, 1, 0			},
	//{  JSKVAL08C, 0, 3, 0			},
	//{  JSKVAL08D, 0, 3, 0			},

	{  JSKVAL09Aex, 0, 1, 0			},
	{  JSKVAL09Bex, 0, 1, 0			},
	{  JSKVAL09Cex, 0, 0, 0			},
	{  JSKVAL09Dex, 0, 1, 0			},
	{  JSKVAL10Cex, 0, 1, 0			},
	{  JSKVAL10Dex, 0, 1, 0			},

	{  JSKVAL11Aex, 0, 1, 0			},
	{  JSKVAL11Bex, 0, 1, 0			},
	{  JSKVAL11Cex, 0, 0, 0			},
	{  JSKVAL11Dex, 0, 1, 0			},
	{  JSKVAL12Cex, 0, 1, 0			},
	{  JSKVAL12Dex, 0, 1, 0			},
	{  JSKVAL13Cex, 0, 1, 0			},
	{  JSKVAL13Dex, 0, 1, 0			},
	//{  JSKVAL14C, 0, 3, 0			},
	//{  JSKVAL14D, 0, 3, 0			},

	{  JSKVAL15Aex, 0, 1, 0			},
	{  JSKVAL15Bex, 0, 1, 0			},
	{  JSKVAL15Cex, 0, 0, 0			},
	{  JSKVAL15Dex, 0, 1, 0			},
	{  JSKVAL16Cex, 0, 1, 0			},
	{  JSKVAL16Dex, 0, 1, 0			},
	//{  JSKVAL17C, 0, 3, 0			},
	//{  JSKVAL17D, 0, 3, 0			},

	{  JSKVAL18Aex, 0, 1, 0			},
	{  JSKVAL18Bex, 0, 1, 0			},
	{  JSKVAL18Cex, 0, 0, 0			},
	{  JSKVAL18Dex, 0, 1, 0			},
	{  JSKVAL19Cex, 0, 1, 0			},
	{  JSKVAL19Dex, 0, 1, 0			},

	{  JSKVAL20Aex, 0, 0, 0			},
	{  JSKVAL20Bex, 0, 0, 0			},
	{  JSKVAL20Cex, 0, 1, 0			},
	{  JSKVAL20Dex, 0, 1, 0			},
	//{  JSKVAL21A, 0, 0, 0			},
	//{  JSKVAL21B, 0, 0, 0			},
	//{  JSKVAL21C, 0, 3, 0			},
	//{  JSKVAL21D, 0, 3, 0			},

	{  JSKVAL22Aex, 0, 1, 0			},
	{  JSKVAL22Bex, 0, 1, 0			},
	{  JSKVAL22Cex, 0, 0, 0			},
	{  JSKVAL23Cex, 0, 1, 0			},
	{  JSKVAL24Aex, 0, 0, 0			},
	{  JSKVAL24Bex, 0, 0, 0			},
	{  JSKVAL24Cex, 0, 1, 0			},
	{  JSKVAL24Dex, 0, 0, 0			},
	//{  JSKVAL25A, 0, 0, 0			},
	//{  JSKVAL25B, 0, 0, 0			},
	//{  JSKVAL25C, 0, 3, 0			},
	//{  JSKVAL25D, 0, 0, 0			},

	{  JSKVAL26Aex, 0, 1, 0			},
	{  JSKVAL26Bex, 0, 1, 0			},
	{  JSKVAL26Cex, 0, 0, 0			},
	{  JSKVAL26Dex, 0, 1, 0			},
	{  JSKVAL27Cex, 0, 1, 0			},
	{  JSKVAL27Dex, 0, 1, 0			},
	{  JSKVAL28Aex, 0, 0, 0			},
	{  JSKVAL28Bex, 0, 0, 0			},
	{  JSKVAL28Cex, 0, 1, 0			},
	{  JSKVAL28Dex, 0, 1, 0			},
	//{  JSKVAL29A, 0, 0, 0			},
	//{  JSKVAL29B, 0, 0, 0			},
	//{  JSKVAL29C, 0, 3, 0			},
	//{  JSKVAL29D, 0, 3, 0			},

	{  JSKVAL30Aex, 0, 1, 0			},
	{  JSKVAL30Bex, 0, 1, 0			},
	{  JSKVAL30Cex, 0, 0, 0			},
	{  JSKVAL30Dex, 0, 1, 0			},
	{  JSKVAL31Cex, 0, 1, 0			},
	{  JSKVAL31Dex, 0, 1, 0			},
	{  JSKVAL32Cex, 0, 1, 0			},
	{  JSKVAL32Dex, 0, 1, 0			},
	//{  JSKVAL33C, 0, 3, 0			},
	//{  JSKVAL33D, 0, 3, 0			},

	{	0, 0, 0, 0					}
};	// 94

static FCSUPDOWN_INFO	FCS_ShinKniinp31Ex[]	=	{
	{	JSKVAL01Aex 		},
	{	JSKVAL02Aex 		},
	{	JSKVAL03Aex 		},
	//{	JSKVAL04A 		},
	{	JSKVAL05Aex 		},
	{	JSKVAL06Aex 		},
	{	JSKVAL07Aex 		},
	//{	JSKVAL08A 		},
	{	JSKVAL09Aex 		},
	{	JSKVAL10Aex 		},

	{	JSKVAL11Aex 		},
	{	JSKVAL12Aex 		},
	{	JSKVAL13Aex 		},
	//{	JSKVAL14A 		},
	{	JSKVAL15Aex 		},
	{	JSKVAL16Aex 		},
	//{	JSKVAL17A 		},
	{	JSKVAL18Aex 		},
	{	JSKVAL19Aex 		},
	{	JSKVAL20Aex 		},

	//{	JSKVAL21A 		},
	{	JSKVAL22Aex 		},
	{	JSKVAL23Aex 		},
	{	JSKVAL24Aex 		},
	//{	JSKVAL25A 		},
	{	JSKVAL26Aex 		},
	{	JSKVAL27Aex 		},
	{	JSKVAL28Aex 		},
	//{	JSKVAL29A 		},
	{	JSKVAL30Aex 		},

	{	JSKVAL31Aex 		},
	{	JSKVAL32Aex 		},
	//{	JSKVAL33A 		},
	{	JSKVAL01Bex 		},
	{	JSKVAL02Bex 		},
	{	JSKVAL03Bex		},
	//{	JSKVAL04B 		},
	{	JSKVAL05Bex 		},
	{	JSKVAL06Bex 		},
	{	JSKVAL07Bex 		},

	//{	JSKVAL08B 		},
	{	JSKVAL09Bex 		},
	{	JSKVAL10Bex 		},
	{	JSKVAL11Bex 		},
	{	JSKVAL12Bex 		},
	{	JSKVAL13Bex 		},
	//{	JSKVAL14B 		},
	{	JSKVAL15Bex 		},
	{	JSKVAL16Bex 		},
	//{	JSKVAL17B 		},

	{	JSKVAL18Bex 		},
	{	JSKVAL19Bex 		},
	{	JSKVAL20Bex 		},
	//{	JSKVAL21B 		},
	{	JSKVAL22Bex 		},
	{	JSKVAL23Bex 		},
	{	JSKVAL24Bex 		},
	//{	JSKVAL25B 		},
	{	JSKVAL26Bex 		},
	{	JSKVAL27Bex 		},

	{	JSKVAL28Bex 		},
	//{	JSKVAL29B 		},
	{	JSKVAL30Bex 		},
	{	JSKVAL31Bex 		},
	{	JSKVAL32Bex 		},
	//{	JSKVAL33B 		},
	{	JSKVAL01Cex 		},
	{	JSKVAL02Cex 		},
	{	JSKVAL03Cex		},
	//{	JSKVAL04C 		},

	{	JSKVAL05Cex 		},
	{	JSKVAL06Cex		},
	{	JSKVAL07Cex 		},
	//{	JSKVAL08C 		},
	{	JSKVAL09Cex 		},
	{	JSKVAL10Cex 		},
	{	JSKVAL11Cex 		},
	{	JSKVAL12Cex 		},
	{	JSKVAL13Cex 		},
	//{	JSKVAL14C 		},

	{	JSKVAL15Cex 		},
	{	JSKVAL16Cex 		},
	//{	JSKVAL17C 		},
	{	JSKVAL18Cex 		},
	{	JSKVAL19Cex 		},
	{	JSKVAL20Cex 		},
	//{	JSKVAL21C 		},
	{	JSKVAL22Cex 		},
	{	JSKVAL23Cex 		},
	{	JSKVAL24Cex 		},

	//{	JSKVAL25C 		},
	{	JSKVAL26Cex 		},
	{	JSKVAL27Cex 		},
	{	JSKVAL28Cex 		},
	//{	JSKVAL29C 		},
	{	JSKVAL30Cex 		},
	{	JSKVAL31Cex 		},
	{	JSKVAL32Cex 		},
	//{	JSKVAL33C 		},
	{	JSKVAL01Dex 		},

	{	JSKVAL02Dex 		},
	{	JSKVAL03Dex		},
	//{	JSKVAL04D 		},
	{	JSKVAL05Dex 		},
	{	JSKVAL06Dex 		},
	{	JSKVAL07Dex 		},
	//{	JSKVAL08D 		},
	{	JSKVAL09Dex 		},
	{	JSKVAL10Dex 		},
	{	JSKVAL11Dex 		},

	{	JSKVAL12Dex 		},
	{	JSKVAL13Dex 		},
	//{	JSKVAL14D 		},
	{	JSKVAL15Dex 		},
	{	JSKVAL16Dex 		},
	//{	JSKVAL17D 		},
	{	JSKVAL18Dex 		},
	{	JSKVAL19Dex 		},
	{	JSKVAL20Dex 		},
	//{	JSKVAL21D 		},

	{	JSKVAL26Dex 		},
	{	JSKVAL27Dex 		},
	{	JSKVAL28Dex 		},
	//{	JSKVAL29D 		},
	{	JSKVAL30Dex 		},
	{	JSKVAL31Dex 		},
	{	JSKVAL32Dex 		},
	//{	JSKVAL33D 		},
};//計１２８

static FCSUPDOWN_INFO	FCS_ShinKniinp31Ex_LR[]	=	{
	{	JSKVAL01Aex 		},
	{	JSKVAL01Bex 		},
	{	JSKVAL01Cex 		},
	{	JSKVAL01Dex 		},
	{	JSKVAL02Aex 		},
	{	JSKVAL02Bex 		},
	{	JSKVAL02Cex 		},
	{	JSKVAL02Dex 		},
	{	JSKVAL03Aex 		},
	{	JSKVAL03Bex 		},

	{	JSKVAL03Cex 		},
	{	JSKVAL03Dex 		},
	//{	JSKVAL04A 		},
	//{	JSKVAL04B 		},
	//{	JSKVAL04C 		},
	//{	JSKVAL04D 		},
	{	JSKVAL05Aex 		},
	{	JSKVAL05Bex 		},
	{	JSKVAL05Cex 		},
	{	JSKVAL05Dex 		},

	{	JSKVAL06Aex 		},
	{	JSKVAL06Bex 		},
	{	JSKVAL06Cex 		},
	{	JSKVAL06Dex 		},
	{	JSKVAL07Aex 		},
	{	JSKVAL07Bex 		},
	{	JSKVAL07Cex 		},
	{	JSKVAL07Dex 		},
	//{	JSKVAL08A 		},
	//{	JSKVAL08B 		},

	//{	JSKVAL08C 		},
	//{	JSKVAL08D 		},
	{	JSKVAL09Aex 		},
	{	JSKVAL09Bex 		},
	{	JSKVAL09Cex 		},
	{	JSKVAL09Dex 		},
	{	JSKVAL10Aex 		},
	{	JSKVAL10Bex 		},
	{	JSKVAL10Cex 		},
	{	JSKVAL10Dex 		},

	{	JSKVAL11Aex 		},
	{	JSKVAL11Bex 		},
	{	JSKVAL11Cex 		},
	{	JSKVAL11Dex 		},
	{	JSKVAL12Aex 		},
	{	JSKVAL12Bex 		},
	{	JSKVAL12Cex 		},
	{	JSKVAL12Dex 		},
	{	JSKVAL13Aex 		},
	{	JSKVAL13Bex 		},

	{	JSKVAL13Cex 		},
	{	JSKVAL13Dex 		},
	//{	JSKVAL14A 		},
	//{	JSKVAL14B 		},
	//{	JSKVAL14C 		},
	//{	JSKVAL14D 		},
	{	JSKVAL15Aex 		},
	{	JSKVAL15Bex 		},
	{	JSKVAL15Cex 		},
	{	JSKVAL15Dex 		},

	{	JSKVAL16Aex 		},
	{	JSKVAL16Bex 		},
	{	JSKVAL16Cex 		},
	{	JSKVAL16Dex 		},
	//{	JSKVAL17A 		},
	//{	JSKVAL17B 		},
	//{	JSKVAL17C 		},
	//{	JSKVAL17D 		},
	{	JSKVAL18Aex 		},
	{	JSKVAL18Bex 		},

	{	JSKVAL18Cex 		},
	{	JSKVAL18Dex 		},
	{	JSKVAL19Aex 		},
	{	JSKVAL19Bex 		},
	{	JSKVAL19Cex 		},
	{	JSKVAL19Dex 		},
	{	JSKVAL20Aex 		},
	{	JSKVAL20Bex 		},
	{	JSKVAL20Cex 		},
	{	JSKVAL20Dex 		},

	//{	JSKVAL21A 		},
	//{	JSKVAL21B 		},
	//{	JSKVAL21C 		},
	//{	JSKVAL21D 		},
	{	JSKVAL22Aex 		},
	{	JSKVAL22Bex 		},
	{	JSKVAL22Cex 		},
	{	JSKVAL23Aex 		},
	{	JSKVAL23Bex 		},
	{	JSKVAL23Cex 		},

	{	JSKVAL24Aex 		},
	{	JSKVAL24Bex 		},
	{	JSKVAL24Cex 		},
	//{	JSKVAL25A 		},
	//{	JSKVAL25B 		},
	//{	JSKVAL25C 		},
	{	JSKVAL26Aex 		},
	{	JSKVAL26Bex 		},
	{	JSKVAL26Cex 		},
	{	JSKVAL26Dex 		},

	{	JSKVAL27Aex 		},
	{	JSKVAL27Bex 		},
	{	JSKVAL27Cex 		},
	{	JSKVAL27Dex 		},	
	{	JSKVAL28Aex 		},
	{	JSKVAL28Bex 		},
	{	JSKVAL28Cex 		},
	{	JSKVAL28Dex 		},	
	//{	JSKVAL29A 		},
	//{	JSKVAL29B 		},

	//{	JSKVAL29C 		},
	//{	JSKVAL29D 		},	
	{	JSKVAL30Aex 		},
	{	JSKVAL30Bex 		},
	{	JSKVAL30Cex 		},
	{	JSKVAL30Dex 		},	
	{	JSKVAL31Aex 		},
	{	JSKVAL31Bex 		},
	{	JSKVAL31Cex 		},
	{	JSKVAL31Dex 		},	

	{	JSKVAL32Aex 		},
	{	JSKVAL32Bex 		},
	{	JSKVAL32Cex 		},
	{	JSKVAL32Dex 		},	
	//{	JSKVAL33A 		},
	//{	JSKVAL33B 		},
	//{	JSKVAL33C 		},
	//{	JSKVAL33D 		},	
};//計１２８

//#define		FCS_SHINKNIINP31CNT	128 //横、縦、数同じなので共同でしてる。別になれば分ける必要あり
#define		FCS_SHINKNIINP31CNTex	97 //横、縦、数同じなので共同でしてる。別になれば分ける必要あり

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