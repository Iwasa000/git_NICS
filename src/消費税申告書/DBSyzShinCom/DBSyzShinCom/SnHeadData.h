#pragma	once

#include <NoTblMod8.h>	// '15.07.30

class CSyzUtil;
class CArithEx;

//== 定数 ==
// セットする年月日対象
typedef enum {
	ID_ICSSH_KAZEIKIKAN_FROM	=	1,	// 課税期間 自
	ID_ICSSH_KAZEIKIKAN_TO,				// 課税期間 至
	ID_ICSSH_CHUUKAN_FROM,				// 中間申告の計算期間 自
	ID_ICSSH_CHUUKAN_TO,				// 中間申告の計算期間 至
	ID_ICSSH_PRESENT_DATE,				// 提出年月日
	ID_ICSSH_KESSAN_FROM,				// 決算期間 自
	ID_ICSSH_KESSAN_TO,					// 決算期間 至
	ID_ZENKI_KAZEIKIKAN_FROM,			// 前期課税期間 自
	ID_ZENKI_KAZEIKIKAN_TO,				// 前期課税期間 至
	ID_SYUZEI_DECIDE_DAY				// 修正・更正・決定の年月日
} EnumIdIcsShDateType;


// 改正バージョン '19.02.27
typedef enum {
	ID_VER_SYZ26 = 0,
	ID_VER_SYZ31						// 消費税10％改正
} EnumIdKaiseiVer;

class CDBNpSub;
class CSyzSyukei;
class SYZSHINCOM_CLASS_EXPORT CSnHeadData
{
public:
	unsigned char	Sn_VER,				// ファイルバ－ジョン (Begin = 0x00)	szsnk->Sn_VER
					Sn_SKKBN,			// 申告区分　…　１＝確定				szsnk->Sn_SKKBN
										//				２＝中間
										//				３＝修正確定
										//				４＝修正中間
										//				０＝空白
					Sn_KOJIN,			// 個人以外の事業者         ON :12ヶ月計算	szsnk->Sn_KOJIN
					Sn_KOANB,			// 個人事業者　ON:月数按分, OFF:12ヶ月計算	szsnk->Sn_KOANB
					Sn_ITIREN[4],		// 一連番号　　　　　　［ＢＣＤ８桁］		szsnk->Sn_ITIREN
					Sn_SEIRI[4],		// 整理番号　　　　　　［ＢＣＤ８桁］		szsnk->Sn_SEIRI
					Sn_ZSEIRI[4];		// 税務署整理番号　　　［ＢＣＤ８桁］		szsnk->Sn_ZSEIRI

//					Sn_KDAYS[3],		// 課税期間　自　　　  ［ＢＣＤ６桁］		szsnk->Sn_KDAYS
//					Sn_KDAYE[3],		//			 至　　　  ［　　〃　　］		szsnk->Sn_KDAYE
//					Sn_MDAYS[3],		// 中間申告の計算期間  ［　　〃　　］		szsnk->Sn_MDAYS
//					Sn_MDAYE[3],		//					   ［　　〃　　］		szsnk->Sn_MDAYE
//					Sn_PRDAY[3],		// 提出年月日　　　　　［ＢＣＤ６桁］		szsnk->Sn_PRDAY
	long			Sn_KDAYS,			// 課税期間　自　　　  ［ＢＣＤ６桁］		szsnk->Sn_KDAYS
					Sn_KDAYE,			//			 至　　　  ［　　〃　　］		szsnk->Sn_KDAYE
					Sn_MDAYS,			// 中間申告の計算期間  ［　　〃　　］		szsnk->Sn_MDAYS
					Sn_MDAYE,			//					   ［　　〃　　］		szsnk->Sn_MDAYE
					Sn_PRDAY;			// 提出年月日　　　　　［ＢＣＤ６桁］		szsnk->Sn_PRDAY
	unsigned char	Sn_SPECIAL,			// D0:ON:簡易計算する						szsnk->Sn_SPECIAL
										// D1:ON:印刷 申告書本表「提出用」					貨物の税額より80％で計算
										// D2:ON:印刷 申告書本表「控用」					印刷	更正の請求書（申出書）	
										// D3:ON:印刷 付表２　　又は　付表５				印刷	還付申告に関する明細書(P.1)
										// D4:ON:印刷 付表１　　又は　付表４				印刷	本表（一般・簡易）
										// D5:ON:印刷 付表２―２又は　付表５-２				印刷	付表１・４
										// D6:ON:印刷 第２６号様式							印刷	付表２・５
										// D7:ON:印刷 仕入控除税額に関する明細書			印刷	付表２-２・５-２

										// 
					Sn_ZMSYO[12],		// 税務署名　　　　　　［漢字６文字］		szsnk->Sn_ZMSYO
					Sn_ZIP[10],			// 郵便番号			　 ［ＡＳＣＩＩ８桁］	szsnk->Sn_ZIP
					Sn_SYOKN[4],		// 所管 (Sn_dmy2[4],)  ［ＡＳＣＩＩ２桁］	szsnk->Sn_SYOKN
					Sn_TELNO1[6],		// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_TELNO2[4],		// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_TELNO3[4],		// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_NOZEI[40*2],		// 納税地　　　　　　　［漢字２０＊２文字］	szsnk->Sn_NOZEI
					Sn_CONAM[40],		// 名称又は，屋号　　　［漢字２０文字］		szsnk->Sn_CONAM
					Sn_COFRI[80],		// 名称又は屋号振り仮名［漢字半角２０文字］	szsnk->Sn_COFRI
					Sn_DIHYO[40],		// 代表者氏名　　　　　［漢字２０文字］		szsnk->Sn_DIHYO
					Sn_DIFRI[80],		// 代表者氏名振り仮名　［漢字半角２０文字］	szsnk->Sn_DIFRI
					Sn_KEIRI[40],		// 経理担当者氏名　　　［漢字２０文字］		szsnk->Sn_KEIRI
					Sn_GPNAM[64],		// 被合併名									szsnk->Sn_ADRD4[3]


					Sn_KAPPU,			// 割賦基準の適用　　　１＝有　０＝無		szsnk->Sn_KAPPU
					Sn_NOBE,			// 延払基準の適用　　　１＝有　０＝無		szsnk->Sn_NOBE
					Sn_KOJI,			// 工事進行基準の適用　１＝有　０＝無		szsnk->Sn_KOJI
					Sn_GENKN,			// 現金主義会計の適用　１＝有　０＝無		szsnk->Sn_GENKN
					Sn_TUMIAG,			// 規則22条１項の適用　１＝有　０＝無		szsnk->Sn_TUMIAG
					Sn_KZTNSK,			// 課税期間短縮　　　　１＝有　０＝無		szsnk->Sn_KZTNSK
					Sn_MCOUNT,			// 中間申告　年間回数　３＝３回 １＝１回	szsnk->Sn_MCOUNT
					Sn_JCOUNT,			// 中間申告　現在申告状態　１～４回			szsnk->Sn_JCOUNT
					Sn_TOKUR,			// 特例計算摘要ＳＷ							szsnk->Sn_TOKUR
					Sn_UPERS,			// 課税売上割合９５％以上のＳＷ				szsnk->Sn_UPERS
										// D0:ON:売上割合が95％以下, OFF:売上割合が95％以上
										// D1:ON:個別対応方式で課税売上割合に準ずる割合に変更
										// D4:ON:課税売上割合を変更
					Sn_TYPE;			// D0:ON:イメージ印刷	OFF:ＯＣＲ印刷		szsnk->Sn_TYPE
										// D1:ON:提出用　カラー用紙	 OFF:提出用　モノクロ用紙	ON:控用 OFF:提出用カラー
										// D2:ON:控用　　カラー用紙　OFF:控用　  モノクロ用紙
										// D3:
										// D4:
										// D5:
										// D6:
										// D7:ON:法人・個人番号欄付き用紙での出力, OFF:法人・個人番号欄無し用紙での出力(通常用紙)
	unsigned char	Sn_BYsw,			// 金融機関選択SW　OFF:銀行,ON:郵便局			szsnk->Sn_BYsw
					Sn_BANK[20],		// 銀行名称　　　　　　［漢字６文字］			szsnk->Sn_BANK
					Sn_SITEN[20],		// 支店名称　　　　　　［漢字６文字］			szsnk->Sn_SITEN
					Sn_YOKIN[8],		// 預金種別　　　　　　［漢字４文字］			szsnk->Sn_YOKIN
					Sn_YUBIN[16],		// 郵便局名　　　　　　［漢字６文字］			szsnk->Sn_YUBIN
					Sn_KOZA[5],			// 口座番号　　　　　　［ＢＣＤ10桁］			szsnk->Sn_KOZA
					Sn_YUNUM1[5],		// 郵便貯金の記号番号１(5ｹﾀ)					szsnk->Sn_YUNUM1
					Sn_YUNUM2[8],		// 郵便貯金の記号番号２(8ｹﾀ)					szsnk->Sn_YUNUM2
					Sn_FINANCE,			// 0:無し 1:銀行 2:金庫 3:組合 4:農協 5:漁協	szsnk->Sn_FINANCE
					Sn_CHAIN;			// 0:無し 1:本店 2:支店 3:本社 4:支社			szsnk->Sn_CHAIN

	unsigned char	Sn_ADRU1[30],		// 上段①　左									szsnk->Sn_ADRU1
					Sn_ADRU2[20],		// 上段①　右									szsnk->Sn_ADRU2
					Sn_ADRC1[20],		// 上段②　左									szsnk->Sn_ADRC1
					Sn_ADRC2[20],		// 上段②　右									szsnk->Sn_ADRC2
					Sn_ADRD1[30],		// 下段①　左									szsnk->Sn_ADRD1
					Sn_ADRD2[16],		// 下段②　右									szsnk->Sn_ADRD2
					Sn_ADDM0[6],		// リザーブ
					Sn_PPVER,			// 拡張サイン
					Sn_PPTYP,			// 特殊処理サイン D0:ON 個人を印刷				szsnk->Sn_PPTYP
					Sn_ADRD3[14],		// 下段①左の１４文字拡張分						szsnk->Sn_ADRD3
					Sn_ADKUP[10],		// 上段左番号１０桁（数字）						szsnk->Sn_ADKUP
					Sn_ADKDW[10],		// 下段左番号１０桁（数字）						szsnk->Sn_ADKDW
					Sn_ADKNO[10];		// 下段右番号　１桁（数字）						szsnk->Sn_ADKNO
	unsigned char	Sn_ADRD4[50],		// 下段①左の統一（下段①左＋下段①左の１４文字拡張分＋２桁）	szsnk->Sn_ADRD4[0]
										// Sn_PPVER D0: ON=上記拡張済み！
					Sn_ADSNO[10],		// 署別一連番号７桁（数字）										szsnk->Sn_ADRD4[1]
					Sn_ADKDC[20];		// 下段右     [10'02.01]▼	個人専用　(szsnk->Sn_ADRD6)

	int				SubInf;				// 税理士情報（New_TaxInf8.subinf）				szsnk->Sn_ADRD4[2]

	unsigned char	Sn_CK45[6],			// 自動車　課税標準額　４．５％
					Sn_CZ45[6],			// 自動車　消費税額　　　〃
					Sn_CH45[6],			// 自動車　対価の返還額　〃
					Sn_CK60[6],			// 自動車　課税標準額　６．０％
					Sn_CZ60[6],			// 自動車　消費税額　　　〃
					Sn_CH60[6];			// 自動車　対価の返還額　〃
	unsigned char	Sn_KURIA[6];		// 事業別課税売上高合計額						szsnk->Sn_KURIA	// 本当は、付表５のデータとして持つべき？？？


	unsigned char	Sn_TYNF1[6],		// 中間納付税額	１回目	 ［ＢＩＮ１２桁］
					Sn_TYNF2[6],		//     〃		２回目	 ［     〃	   ］
					Sn_TYNF3[6],		//     〃		３回目	 ［     〃	   ］
					Sn_TYNF4[6];		//     〃		４回目	 ［     〃	   ］
					 
	unsigned char	Sn_TTYNF1[6],		// 中間納付税額	１回目	 ［地方消費税用］
					Sn_TTYNF2[6],		//     〃		２回目	 ［     〃	   ］
					Sn_TTYNF3[6],		//     〃		３回目	 ［     〃	   ］
					Sn_TTYNF4[6];		//     〃		４回目	 ［     〃	   ］

	unsigned char	Sn_TYNF[12][6],		// 中間申告年１１回
					Sn_TTNF[12][6];		// 中間申告年１１回	地方消費税用


	unsigned char	Sn_WURI[6];			// 課税売上高　　ＷＯＲＫ［　　 〃 　　］		szsnk->Sn_WURI


	unsigned char	Sn_ZeiHou,			// D0=ON:税理士法第３０条
										// D1=ON:税理士法第３３条の２
										// D2=ON:配賦不要
										// D3=
										// D4=ON:税理士法 施行前 OFF:税理士法 施行後			★廃止！
										// D5=ON:改訂前本表		 OFF:改訂後本表
										// D6=
										// D7=ON:消費税の還付申告に関する明細書	OFF:仕入控除税額に関する明細書			
					Sn_Syuusei,			// D0=
										// D1=ON:更正の請求書
										// D2=ON:更正の申出書　D1がONで有効
										// D3=ON:予定(26号)
										// D4=ON:準確定
										// D5=
										// D6=
										// D7=
					Sn_Sign1,			// D0=ON:消費税の還付申告に関する明細書の財務連動する
										// D1=ON:印刷 還付申告に関する明細書(1/2)
										// D2=ON:印刷 還付申告に関する明細書(2/2)
										// D3=ON:印刷 還付申告に関する明細書（次葉）	
										// D4=ON:課税取引金額計算表の財務連動する
										// D5=ON:印刷 課税売上高計算表
										// D6=ON:印刷 課税仕入高計算表
										// D7=ON:課税取引金額計算表（事業所得）				電子申告済を印刷する							★下へ移動↓
										// 
					Sn_Sign2,			// D0=ON:特定実額収入集計テ－ブルを初期化する
										// D1=ON:特定収入が5％未満で対象外		
										// D2=
										// D3=
										// D4=ON:税理士法人名印刷サイン
										// D5=ON:期間短縮解除後、期間の開始以降で申告書集計をする
										// D6=ON:個別対応方式の税額を期間計で計算を行う			◆新規	
										// D7=ON:電子申告済を印刷する							◆上から移動↑
					Sn_Sign3,			// D0=ON:印刷　課税取引金額計算表　不動産事業者
										// D1=ON:印刷　課税取引金額計算表　農業事業者
										// D2=ON:印刷　特定収入計算表
										// D3=ON:印刷　第２６号様式Ａ４ヨコ控用
										// D4=ON:印刷　事業別売上高
										// D5=ON:印刷　付表５－２－２
										// D6=ON:印刷　特定課税仕入れがある場合の課税標準額の内訳書
										// D7=ON:印刷　特定課税仕入れがある場合の課税標準額の内訳書（控用）

					Sn_Sign4;			// D0=ON:ロックで入力不可								◆新規
										// D1=
										// D2=
										// D3=ON:印刷　付表６　2018.03.13
										// D4=ON:平成２３年消費税改正後で課税売上が５億円を超えている。
										// D5=ON:更新時、還付明細書を全て初期化する。			★廃止
										// D6=ON:みなし課税期間
										// D7=ON:非連動（申告書集計をしないで実額で入力を行う）	◆新規

	unsigned char	Sn_EXP100;			// 課税売上高100％
										// D0=課税売上割合が0%ですが、100%と表示して、仕入税額を全額控除する

	unsigned char	Sn_TAXNO[5];		// 税務署コード

	// -> 以降 szvol 由来
	int				SVvers;				// バージョン										szvol->SVvers
	int				SVnver;				// 電子申告バージョン								szvol->SVnver


	unsigned char	SVsign;				// 特殊処理サイン
	unsigned char	SVmzsw;				// 複数税率処理スイッチ
	int				SVsper[16];			// 簡易課税 みなし仕入税率
										//  0：卸売業
										//  1：小売業
										//  2：製造業
										//  3：その他
										//  4：サービス業
	unsigned char	SVsasw;				// 申告書エリア登録ＳＷ
	unsigned char	SVhksw;				// 販売奨励金収入科目登録ｓｗ
	unsigned char	SVd5sw;				// 第５種事業に係る経過措置に関するｓｗ
	unsigned char	SVkojn;				// 個人事業種選択ｓｗ　d0:個人以外 d４:個人
	unsigned char	SVzchg;				// 免税＜－＞課税切替サイン
	unsigned char	SVkazei;			// 消費税　課税区分／仕入税額の按分法
	unsigned char	SVsacq;				// 申告集計サイン
	unsigned char	SVzisq;				// 担当税理士番号

	unsigned char	SVtval1[6];			// 調整課税仕入等に係る棚卸資産額
	unsigned char	SVtval2[6];			// 調整課税仕入等に係る棚卸消費税額
	unsigned char	SVzuri[6];			// 基準期間の課税売上高

	unsigned char	SVkan0[6],			// 簡易課税適用の課税売上高範囲（下限）
					SVkan1[6];			// 簡易課税適用の課税売上高範囲（上限）
	// <- ここまで szvol 由来


	unsigned char	m_apno;				// 業種(zvolume->apno)
	unsigned char	m_s_sgn2;			// 課税方式区分(zvolume->s_sgn2)
	unsigned char	m_s_sgn4;			// 処理選択sw(zvolume->s_sgn4)

	unsigned short	m_UriRatioBorder;	// 課税売上割合の閾値

	long			m_ss_ymd;			// 期首年月日
	long			m_ee_ymd;			// 期末年月日

	long			m_Zrkbn;			// 税理士区分(szsnk->Sn_ADRD4[4]を使用)			('15.02.27)
										//  R_ZEIRI.rz_zeiKbnと連動
	long			m_ZroutSgn;			// 税理士出力サイン(szsnk->Sn_ADRD4[4]を使用)	('15.02.27)
										//  □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□■■
										//  D0:税理士区分出力(ON:出力する, OFF:出力しない)
										//  D1:(直接受任)を付記(ON:出力する, OFF:出力しない)

	char			Sn_HOJIN_NO[24];	// 法人番号（13桁部分）		('15.07.11)
	char			Sn_HOJIN_NO_EXT[24];// 法人番号（付番部分）		('15.07.11)
	char			Sn_KOJIN_HASH[32];	// 個人番号ハッシュ値		('15.07.11)

	//------>特定課税仕入対応yoshida150713
	long			m_DispTabSgn;		// タブを表示するかどうかのサイン(szsnk->Sn_ADRD4[4]を使用)	。
										// 今回は特定課税仕入の関係で新表タブが追加されたので第１ビットを使用
										//  □□□□ □□□□ □□□□ □□□□ □□□■ □□□■ ■■■■ □□□■
										//  D0:特定新表タブ(ON:出力する, OFF:出力しない)
										//  D4:特例計算：第５－（１）号様式(ON:出力する, OFF:出力しない)
										//  D5:特例計算：第５－（２）号様式(ON:出力する, OFF:出力しない)
										//  D6:特例計算：第５－（３）号様式(ON:出力する, OFF:出力しない)
										//  D7:特例計算適用チェック（基本情報画面）

										//  D12:令和2年4月1日改正様式を使用する
										//  D13:2割特例計算を使用する
	//<-------------------------------

	unsigned char	Sn_DIADR[40*2];		// 代表者住所				('15.12.15)
	unsigned char	Sn_DIZIP[10];		// 代表者郵便番号			('15.12.15)
	unsigned char	Sn_DITELNO1[6],		// 代表者電話番号(市外)		('15.12.23) szsnk->Sn_ADRD4[6] を分解
					Sn_DITELNO2[4],		// 代表者電話番号(市内)		('15.12.23) szsnk->Sn_ADRD4[6] を分解
					Sn_DITELNO3[4];		// 代表者電話番号(局番)		('15.12.23) szsnk->Sn_ADRD4[6] を分解

	/*unsigned char	Sn_Sign5,			// D0=ON:印刷　付表１－１	付表４－１
										// D1=ON:印刷　付表１－２	付表４－２
										// D2=ON:印刷　付表２－１	付表５－１
										// D3=ON:印刷　付表２－２	付表５－１(２)
										// D4=ON:印刷				付表５－２
										// D5=ON:印刷　				付表５－２(２)
										// D6=ON:印刷　
										// D7=ON:印刷

					Sn_Sign6;			// D0=ON:印刷　特例１ 課税資産の譲渡等の対価の額の計算表(軽減売上割合～)
										// D1=ON:印刷　特例２ 課税資産の譲渡等の対価の額の計算表(小売等軽減仕入割合～)
										// D2=ON:印刷　特例３ 課税仕入れ等の税額の計算表
										// D3=ON:印刷　
										// D4=ON:印刷	
										// D5=ON:印刷　
										// D6=ON:印刷　
										// D7=ON:印刷
	*/									// 削除 ('19.07.24)

	unsigned char	Kaisei_VER;			// EnumIdKaiseiVer

	long			Sn_PrintOutSgn;		// 各帳票の出力サイン ('19.07.24)	※Sn_Sign5、Sn_Sign6、rsv1(2Byte)切り崩し
											// D0 =ON:印刷 申告書本表「提出用」
											// D1 =ON:印刷 申告書本表「控用」
											// D2 =ON:印刷 課税標準額等の内訳書「提出用」
											// D3 =ON:印刷 課税標準額等の内訳書「控用」
											// D4 =ON:印刷 付表１－１ 又は 付表４－１
											// D5 =ON:印刷 付表１－２ 又は 付表４－２
											// D6 =ON:印刷 付表２－１ 又は 付表５－１（１）
											// D7 =ON:印刷 付表２―２ 又は 付表５－２（１）

											// D8 =ON:印刷 付表５－１（２）
											// D9 =ON:印刷 付表５－２（２）
											// D10=ON:印刷 付表６
											// D11=ON:印刷 第２６号様式
											// D12=ON:印刷 第２６号様式Ａ４ヨコ控用
											// D13=ON:印刷 還付申告に関する明細書(1/2)
											// D14=ON:印刷 還付申告に関する明細書(2/2)
											// D15=ON:印刷 更正の請求書（申出書）

											// D16=ON:印刷 特例１
											// D17=ON:印刷 特例２
											// D18=ON:印刷 特例３
											// D19=ON:印刷 課税売上高計算表
											// D20=ON:印刷 課税仕入高計算表
											// D21=ON:印刷 課税取引金額計算表　事業所得
											// D22=ON:印刷 課税取引金額計算表　不動産事業者
											// D23=ON:印刷 課税取引金額計算表　農業事業者

											// D24=ON:印刷 特定収入計算表
											// D25=ON:印刷 事業別売上高
											// D26=ON:印刷 付表１－３ 又は 付表４－３
											// D27=ON:印刷 付表２－３ 又は 付表５－３（１）
											// D28=ON:印刷 付表５－３（２）
											// D29=
											// D30=
											// D31=


	long			Sn_GenericSgn;		// 汎用サイン('19.09.03)	※rsv1(4Byte)切り崩し
											// D0  = ON:本表・付表 8%→10%コンバージョンサイン
											// D1  = ON:課税取引金額計算表 8%→10%コンバージョンサイン
											// D2  = ON:簡易本表：入力ロックして業務を終了すると4の項目に15の金額が上書きされていた不具合を修正サイン
											//		   修正申告の時に本表26の金額に100円未満の金額が表示されていた不具合を修正サイン
											// D3  =

											// D4  = ON:新型コロナウイルスによる申告・納付期限延長申請チェック情報
											// D5  = ON:付表6 令和2年4月1日改正前の様式で出力する
											// D6  = ON:付表5 控除対象仕入税額が最も大きくなる項目のみ出力( 簡易課税のみ )
											// D7  = ON:2021/09リリースプロ　起動サイン

											// D8  = ON:更正の請求 reaskテーブル拡張サイン（Kikakutei1/Kikakutei2）令和4年12月31日以後終了課税期間分の帳票に対応
											// D9  = ON:振替継続希望
											// D10 = ON:公金受取口座の利用
											// D11 = ON:課税取引金額計算表　7.8%適用分の自動計算サイン

											// D12 = ON:公金受取口座を利用する（更正の請求/令和5年3月31日）
											// D13 = ON:課税取引金額計算表 データ保存場所変更コンバージョンサイン（230919）

// 20-0225 add -->
	long			Sn_PrintOutSgn2;	// 各帳票の出力サイン２ ('22.03.03)	※rsv1(4Byte)切り崩し  内容が変更されてるためコメント書き直し
											// D0=ON:　申告書本表　提出用カラー/モノクロ　ON:カラー OFF:モノクロ
											// D1=ON:　申告書本表　控　用カラー/モノクロ　ON:カラー OFF:モノクロ
											// D2=ON:　申告書本表　チェック情報
											// D3=ON:　申告書本表　チェック情報（提出用）
											// D4=ON:　申告書本表　チェック情報（控用）
											// D5=ON:　課税標準額等の内訳書　チェック情報
											// D6=ON:　課税標準額等の内訳書　チェック情報（提出用）
											// D7=ON:　課税標準額等の内訳書　チェック情報（控用）
											// D8=ON:　
											// D9=ON:　
											// 新様式のみ、追加に伴い Sn_TYPEのD1,D2、Sn_PrintOutSgnのD0～D3は未使用に。
// 20-0225 add <--

protected:
// 20-0225 del -->
	//char			rsv1[307];			// '15.02.27	512->504
// 20-0225 del <--
// 20-0225 add -->
	char			rsv1[303];			// '15.02.27	512->504
// 20-0225 add <--
										// '15.07.11	504->424
										// '15.07.13	424->420
										// '15.12.15	420->330
										// '15.12.23	330->316
										// '19.02.18	316->315
										// '19.02.27	315->314
										// '19.04.16	314->313
										// '19.07.24	313->311
										// '19.09.03	311->307
// 20-0225 add -->
										// '22.03.03	307->303
// 20-0225 add <--
private:
	CSyzUtil		m_Util;
	CArithEx		m_Arith;

protected:
	char			rsv2[256];

public:
	CSnHeadData();   // 標準コンストラクタ
	virtual ~CSnHeadData();

private:
	// DB からデータ読込み(szvol 関連)
	int GetDataSzvolSub( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み(szvol 関連)
	int SetDataSzvolSub( CDBNpSub *pDBNpSub, int sn_seq );

public:
	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData( CSyzSyukei *pSyzSyukei );

	// 課税売上割合に準ずる割合を使用？
	BOOL IsUseSemiUriRatio();
	// 仕入税額の按分方法が個別対応？
	BOOL IsKobetuSiireAnbun();
	// 課税売上割合95%以上？
	BOOL IsRatioOver95();
	// 修正申告？
	BOOL IsRevisedTaxReturn();
	// 個人？
	BOOL IsSoleProprietor();
	// 消費税の還付申告明細書を使用？
	BOOL IsUseTaxRefund();
	// 中間予定申告？
	BOOL IsMiddleProvisional();
	// 集計データの読込み必要？
	BOOL IsReadCalqedData();
	// 非連動処理の入力状態？
	BOOL IsInUnconnectInput( CDBNpSub *pDBNpSub );

	// 年月日データの取得
	void GetYmdData( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd );
	// 年月日データの取込
	void SetYmdData( EnumIdIcsShDateType dsDtype, int type, char data );

	// 税務署コードの取得
	int GetTaxationOffice( CWnd *pParent, ESELECT *pEselect );

	// ロック解除をするかどうか
	int DoUnlock( CDBNpSub *pDBNpSub, CWnd *pParent );
	
	// 課税売上高５億円チェック
	int CheckTaxationSales( char *kazeiuri, char *valcnv, int s_tno, int d_tno );

	// 申告区分の取得
	BYTE  GetKubun();
	// 申告区分のセット
	void SetKubun( int kubun );
	// 期間短縮
	int IsKikanTansyuku();
	// 中間申告？
	int IsTyukanShinkoku();
	// 確定申告？
	int IsKakuteiShinkoku();
//2018.03.13 INSERT START
	// 準確定申告？
	int IsJunkakuteiShinkoku();
//2018.03.13 INSERT END

	// 更新時のデータクリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
	// 既存データクリア
	void ClearReadData();

	// 連動処理？
	BOOL IsDataConnected();

	// ヘッダバージョンの書き換え
	int SetSzvolHeadVersion( CDBNpSub *pDBNpSub, int version );
	// 電子申告バージョンの付加
	int SetEselVersion( CDBNpSub *pDBNpSub, int sn_seq );
	// 申告区分の取得
	BYTE GetKubun( unsigned char skkbn, unsigned char syuusei );

private:
	// 電子申告バージョンの取得
	int GetEselVersion( CDBNpSub *pDBNpSub, unsigned char skkbn, unsigned char syuusei, long kdayE, long mdayE, long kdayS, long mdayS );
	// 文字列データをサインに分解('15.02.27)
	int SepStrToSign( int type, CString &srcStr );
	// サインを文字列データに結合('15.02.27)
	int BindSignToStr( int type, CString &dstStr );

public:
	// 特定収入計算表 金額連動？('15.03.09)
	BOOL IsSpcDataLinked( CDBNpSub *pDBNpSub );
	// マイナンバーの取込('15.07.11)
	int TakeInMynumber( CDBNpSub *pDBNpSub, TgPackData *pTgdata, CString &ErrMsg, int gyousyu_flg, int sn_seq=0 );
	// 個人番号ハッシュ値の比較('15.07.11)
	int CompareKojinHash( CByteArray &h1, CByteArray &h2 );
	// 個人番号ハッシュ値に変更有？('15.07.11)
	BOOL IsChangedKojinHash( CByteArray &h1 );
	// 個人番号管理データの取得('15.07.11)
	int GetNoData( CDBNpSub *pDBNpSub, void *ndtbl );
	// SQL 文による個人番号ハッシュのデータクリア('15.07.11)
	int ClearKjhashBySql( CDBNpSub *pDBNpSub, int seq, TgPackData *pTgdata );
	// 正しい個人番号ハッシュ値？('15.07.11)
	BOOL IsRightKjhash( CByteArray& kj_hash );
	// 正しい個人番号ハッシュ値？('15.07.11)
	BOOL IsRightKjhash();
	// マイナンバーデータの登録
	int RegistMynumber( CDBNpSub *pDBNpSub, TgPackData *pTgdata, int sn_seq, unsigned char kbFlg );
	// 履歴情報のセット('15.07.30)
	void SetNoHistory( NOHISTTBL *pNohist, CDBNpSub *pDBNpSub, TgPackData *pTgdata );
	// マイナンバーデータの初期化('15.07.30)
	void InitNodatatbl( NODATATBL *pNodata );
	// 履歴情報レコードのデータセット('15.09.14)
	void SetRirekiInfo( TgPackData* pTgdata, CDBNpSub* pDBNpSub, NOHISTTBL* pNhtbl );
	// 特定課税仕入表示必要有り？('15.08.20)
	BOOL IsNeedTkkzDisp();
//2015.10.20 INSERT START
	// 個人番号・法人番号取得
	int GetMyNumber(CString &str);
	// 個人番号・法人番号取得（ハイフン編集）
	int GetMyNumber_hyphen(CString &str);
//2015.10.20 INSERT END
	// 法人・個人番号欄付き出力？('16.07.11)
	bool IsPrintWithMynoHonpyo();
//2018.08.21 INSERT START
	int GetString_KessanKikan( CString* kessan );
//2018.08.21 INSERT END

	// 年月日データの取得（改元対応）
	void GetYmdDataGen( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd, int *pG=NULL );
	// 年月日データの取込（改元対応）
	void SetYmdDataGen( EnumIdIcsShDateType dsDtype, int type, char data, int gengo=4 );
	// 年月日データの取込（改元対応）
	void SetYmdDataGen( EnumIdIcsShDateType dsDtype, int mType, char yy, char mm, char dd, int gengo=4 );

// 20-0225 add -->
	BOOL Is10PerKikan( void );
	int PrintOutSgnConvert( int pSw1, int pSw2 );
// 20-0225 add <--
};
