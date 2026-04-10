
#pragma once

//-----------------------------------------------------------------------------
// 消費税申告書：共通情報
//-----------------------------------------------------------------------------


// キー移動時
#define		REPLY_ERR		0x00000001
#define		REPLY_OK		0x00000002
#define		REPLY_NEW		0x00000004
#define		REPLY_NEXT		0x00010000
#define		REPLY_BACK		0x00020000
#define		REPLY_STAY		0x00040000
#define		REPLY_MOVE		0x00080000
#define		REPLY_DELT		0x80000000


//== クラス ==
class	CShinCom
{
public:
	CShinCom();
	~CShinCom();

	CBrush& GetBkClrBrsh(){ return m_BkClrBrsh; };	

private:
	CBrush	m_BkClrBrsh;	// バックカラー

//== 文字列 ==
#define	FMT15	_T("SSS,SSS,SSS,SSS")
#define	FMT16	_T("SSS,SSS,SSS,SS9")
#define	FMT14	_T("SS,SSS,SSS,SSS")
#define	FMT13	_T("SSS,SSS,SSS,S")
#define	FMT12	_T("SSSSSSSSSSSS")
#define	FMT120	_T("SSSSSSSSSSS9")
#define	FMT110	_T("SSS,SSS,SSS,")
#define	FMT11	_T("SSS,SSS,SSS")
#define	FMT10	_T("SS,SSS,SSS")
#define	FMT010	_T("SSSSSSSSSS")
#define	FMT09	_T("SSSSSSSSS")
#define	FMT12S	_T("sss,sss,sss,sss")
#define	FMT011EX	_T("SSSSSSSSSSS")
#define	FMT12EX		_T("SSSSSSSSSSSSS")
#define	FMT120EX	_T("SSSSSSSSSSSS9")
#define	FMT13EX		_T("SSSSSSSSSSSSSS")
#define	FMT130EX	_T("SSSSSSSSSSSSS9")
#define	FMT15_EX	_T("S,SSS,SSS,SSS,SSS")
#define	FMT16_EX	_T("SS,SSS,SSS,SSS,SSS")
#define	FMT110_EX	_T("SS,SSS,SSS,SSS,")
#define	FMT120_EX	_T("SS,SSS,SSS,SSS,S")
#define	FM10H		_T("s,sssssss")

#define	FMT14Z		_T("SS,SSS,SSS,SS9")		// '15.01.27
#define	FMT15_EXZ	_T("S,SSS,SSS,SSS,SS9")		// '15.01.27
#define	FMT11Z		_T("SSS,SSS,SS9")			// '15.02.03

#define	FMT16_EXZ	_T("SS,SSS,SSS,SSS,SS9")

};

// 元号
static	char*	SYZ31_EMPTY_G_STR	=	_T("　　");
static	char*	SYZ31_HEISEI_G_STR	=	_T("平成");
static	char*	SYZ31_REIWA_G_STR	=	_T("令和");

// 令和2年3月1日以降の課税期間であれば、納税者有利になるよう計算
//#define TAXADVANTAGE 20200301

// 出荷の都合により、令和2年3月1日から令和2年4月1日に変更
// 令和2年4月1日以降の課税期間であれば、納税者有利になるよう計算
#define TAXADVANTAGE 20200401

// 令和2年8月1日以降の課税期間であれば、簡易課税の計算を修正
#define TAXMODIFY 20200801

// 220412 add -->
// 令和4年4月1日以降の課税期間であれば、特例計算（5-3）のチェック欄を非表示
#define TOK53MODIFY 20220401
// 220412 add <--

// 更正の請求/新様式対応
// 令和4年12月31日以後の課税期間であれば、更正の請求の「この請求前の金額」列をカット
// 「この請求前の期確定税額」項目を追加
#define KSKMODIFY 20221231

#define	SIROIROCOLOR	PALETTERGB( 255, 251, 240 )

// 付表5　75%以上判定用
#define URIAGEWARIAI	75

// 令和4年4月1日以降の課税期間の場合、振替継続希望欄と公金受取口座の利用項目が本表に追加
#define R04REFORM 20220401

// 2割特例適用期間　令和５年10月１日から令和８年９月30日までの日の属する各課税期間
#define INVOICE_2WARI_EDAY 20260930

// 二割特例ビット CSnHeadData::m_DispTabSgn
#define BIT_NIWARITOKUREI	0x2000

enum {
	DAIICHISHU = 1,
	DAINISHU,
	DAISANSHU,
	DAIYONSHU,
	DAIGOSHU,
	DAIROKUSHU
};

enum {
	ID_F520 = 0,	// 付表5 20.原則計算を適用する場合
	ID_F521,		// 21.特例計算を適用する場合 1種類の事業で75%以上
	ID_F522,		// 22.2種類の事業で75%以上
	ID_F523,		// 23
	ID_F524,		// 24
	ID_F525,		// 25
	ID_F526,		// 26
	ID_F527,		// 27
	ID_F528,		// 28
	ID_F529,		// 29
	ID_F530,		// 30
	ID_F531,		// 31
	ID_F532,		// 32
	ID_F533,		// 33
	ID_F534,		// 34
	ID_F535,		// 35
	ID_F536,		// 36
};

enum {
	FHYO5_1 = 0,
	FHYO5_2
};