#pragma once

#define JIGYOKUBUN_MAX 0x07

//軽減売上割合集計モジュール結果受け渡し用構造体.(全区分).
typedef struct _SIMKURIWARIZREC{

    //集計ダイアログ内転記サイン
    char tenkisign[JIGYOKUBUN_MAX];

	//課税資産の譲渡等の税込み価額の合計額.(適用対象期間).
	unsigned char uriagegoukei[JIGYOKUBUN_MAX][6];
	unsigned char uriagegoukei_bekki[JIGYOKUBUN_MAX][6];

	//軽減対象課税資産の譲渡等の税込み価額の合計額.(適用対象期間).
	unsigned char keigenuriagegoukei[JIGYOKUBUN_MAX][6];
	unsigned char keigenuriagegoukei_bekki[JIGYOKUBUN_MAX][6];

	//販売奨励金費用税込み額(軽8.0%+10.0%)
	unsigned char hanbaisyoreikinhiyo[JIGYOKUBUN_MAX][6];
	//売上割引税込み額(軽8.0%+10.0%)
	unsigned char uriagewaribiki[JIGYOKUBUN_MAX][6];
	//貸倒回収税込み額(軽8.0%+10.0%)
	unsigned char kashidaorekaisyu[JIGYOKUBUN_MAX][6];
	//貸倒損失税込み額(軽8.0%+10.0%)
	unsigned char kashidaoreson[JIGYOKUBUN_MAX][6];

	//通常の事業を行う連続する10営業日(開始日)
	long sddate[JIGYOKUBUN_MAX];
	//通常の事業を行う連続する10営業日(終了日)
	long eddate[JIGYOKUBUN_MAX];
	//10営業日中の課税売上.
	unsigned char uriage[JIGYOKUBUN_MAX][6];
	unsigned char uriage_bekki[JIGYOKUBUN_MAX][6];
	//10営業日中の軽減税率対象課税売上.
	unsigned char keigenuriage[JIGYOKUBUN_MAX][6];
	unsigned char keigenuriage_bekki[JIGYOKUBUN_MAX][6];

	//10営業日中の軽減売上割合.
	unsigned char keigenwariai[JIGYOKUBUN_MAX][6];

	//税率区分可能な事業のおける課税資産の譲渡等
    //軽減対象資産の譲渡等(税率6.24%適用分)の
    //対価の額の合計額(税抜き).(適用対象期間).
	unsigned char sgaikbn_keigenuriage_sum[6];

	//税率区分可能な事業のおける課税資産の譲渡等
    //軽減対象資産の譲渡等以外の課税資産の譲渡等(税率7.8%適用分)の
    //対価の額の合計額(税抜き).(適用対象期間).
	unsigned char sgaikbn_uriage_sum[6];

    //リザーブ領域.
    char rsv[400];

} SIMKURIWARIZREC;

typedef struct _SIMKSIREWARIZREC{

    //課税仕入れに係る支払対価の額(税込み)
	unsigned char sire[6];
	unsigned char sire358[6];

    //特定課税仕入れに係る支払対価の額(税込み)
    unsigned char tksire10[6];

	//特定課税仕入れに係る支払対価の額(旧税率分)(税込み)
	unsigned char tksire8[6];

    //保税地域から引き取った課税貨物に係る税込引取価額
	unsigned char kamotsusire[6];
	unsigned char kamotsusire358[6];

    //軽減対象資産の譲渡等(税率6.24%適用分)にのみ
    //要するものの金額(税込み)
    unsigned char keigensire[6];

	//課税資産の譲渡等の税込価額の合計額
	unsigned char uriagegoukei[6];

	//軽減対象資産の譲渡等の税込価額の合計額
	unsigned char keigenuriagegoukei[6];

    //棚卸調整額(軽8.0%)税率6.24%適用分
    unsigned char tanaoroshikasankei8[6];

	//棚卸調整額(軽8.0%)税率6.24%適用分
	unsigned char tanaoroshigensankei8[6];

	//棚卸調整額(10.0%)税率7.8%適用分
    unsigned char tanaoroshikasan10[6];

	//棚卸調整額(10.0%)税率7.8%適用分
	unsigned char tanaoroshigensan10[6];

	//課税仕入れに係る支払対価の額(税込み).(返還).
	unsigned char sirehenkann10[6];

	//軽減対象資産の譲渡等(税率6.24%適用分)にのみ
	//要するものの金額(税込み).(返還).
	unsigned char sirehenkannkei8[6];

	//課税仕入れに係る支払対価の額(税込み).旧税率.(返還).
	unsigned char sirehenkann358[6];

	//特定課税仕入れに係る支払対価の額(税込み).(返還).
	unsigned char tksirehenkann10[6];

    //リザーブ領域.
    char rsv[98];

} SIMKSIREWARIZREC;

#ifdef _SIMKURIWARI
#define _DLLSIMEXPORT __declspec(dllexport)
#else
#define _DLLSIMEXPORT __declspec(dllimport)
#endif

class CSimKuriwariW;
class CSimKuriwari
{

private:
	CSimKuriwariW* m_pSimKuriwariW;

public:
    char rsv[256];

public:
	_DLLSIMEXPORT CSimKuriwari(CDBZmSub* pZmsub);
	_DLLSIMEXPORT virtual ~CSimKuriwari();

	//営業日一覧ダイアログ
	_DLLSIMEXPORT int CreateDlg(CWnd* pParent);

	//集計ダイアログ
	//sddate:開始日 eddate:終了日, 指定なしの場合は決算期間内の特例適用対象期間
	_DLLSIMEXPORT int CreateSyukeiDlg(CWnd* pParent,
		long sddate, long eddate);

	//営業日一覧ダイアログ
	_DLLSIMEXPORT void EndDlg(int nResult);

	//集計ダイアログ
	_DLLSIMEXPORT void EndSyukeiDlg(int nResult);

    //祝日・休日設定・集計ダイアログ内の集計データ取得.
	_DLLSIMEXPORT int ReadDlgSyukeiData(SIMKURIWARIZREC* pRec);

    //課税資産の譲渡等の対価の額の計算
    //軽減売上割合(10営業日)を使用する課税期間用
	//sddate:開始日 eddate:終了日, 指定なしの場合は決算期間内の特例適用対象期間
	_DLLSIMEXPORT int SyukeiKuriwari10Eigyobi(SIMKURIWARIZREC* pRec, long sddate=0, long eddate=0);

    //課税資産の譲渡等の対価の額の計算表
    //小売等軽減仕入割合を使用する課税期間用
	//sddate:開始日 eddate:終了日, 指定なしの場合は決算期間内の特例適用対象期間
	_DLLSIMEXPORT int SyukeiKsirewari(SIMKSIREWARIZREC* pRec, bool IsNeedTkkz, long sddate = 0, long eddate = 0);

};
