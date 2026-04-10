//--------------------------------------------------
//	dateConvert.h
//
//	2006.02.17～
//--------------------------------------------------

#ifndef __DATECONVERT_H__
#define __DATECONVERT_H__

#pragma once
#include "ICSArith8.h"
// midori 181101 add -->
#include <voldatedb8.h>
// midori 181101 add <--

//	元号定数
#define	DC_GENGOU_REIGAI	"例外"	//	例外
#define	DC_GENGOU_MEIJI		"明治"	//	明治
#define	DC_GENGOU_TAISYOU	"大正"	//	大正
#define	DC_GENGOU_SYOUWA	"昭和"	//	昭和
#define	DC_GENGOU_HEISEI	"平成"	//	平成

#define DC_DATE_GENGOU		0x01	//	元号
#define DC_DATE_WAREKI		0x02	//	和暦
#define DC_DATE_MONTH		0x04	//	月
#define DC_DATE_DAY			0x08	//	日

#define	DC_DATE_WM			DC_DATE_WAREKI | DC_DATE_MONTH		//	和暦、月
#define	DC_DATE_WMD			DC_DATE_WM | DC_DATE_DAY			//	和暦、月、日
#define	DC_DATE_GW			DC_DATE_GENGOU | DC_DATE_WAREKI		//	元号、和暦
#define	DC_DATE_GWM			DC_DATE_GW | DC_DATE_MONTH			//	元号、和暦、月
#define	DC_DATE_GWMD		DC_DATE_GWM | DC_DATE_DAY			//	元号、和暦、月、日

#define DC_DATE_WMD_SSYOUWA	19890101		// 「和暦、月、日」で扱う昭和の開始日
#define DC_DATE_WMD_ESYOUWA	19890107		// 「和暦、月、日」で扱う昭和の終了日

#define DC_DATE_SWAREKI		0x01			// 「和暦」元年

#define DC_REKINEN_SEIREKI	1
#define DC_REKINEN_WAREKI	0

//	元号種列挙型
enum DC_GENGOU_TYPE{
	GT_REIGAI = 0,	//	例外
	GT_MEIJI,		//	明治
	GT_TAISYOU,		//	大正
	GT_SYOUWA,		//	昭和
// midori 181101 del -->
	//GT_HEISEI		//	平成
// midori 181101 del <--
// midori 181101 add -->
	GT_HEISEI,		//	平成
	GT_KAIGEN		//	改元
// midori 181101 add <--
};

//	元号テーブル
struct _DC_GENGOU_TABLE{
	int		nStart;			//	開始年月日（西暦）
	int		nEnd;			//	終了年月日（西暦）
	int		nBasicYear;		//	基準年

	char	cStartWareki;	//	開始和暦（BCD）
	char	cStartMonth;	//	開始月（BCD）
	char	cStartDay;		//	開始日（BCD）

	char	cEndWareki;		//	終了和暦（BCD）
	char	cEndMonth;		//	終了月（BCD）
	char	cEndDay;		//	終了日（BCD）
};

//	変換テーブル
static struct _DC_GENGOU_TABLE _DC_CHANGE_TABLE[] = {
	{		 0,	18680908,    0,		0x00,0x00,0x00,		0x00,0x09,0x07	},	//	明治以前	…	GT_REIGAI
	{ 18680908,	19120729, 1868,		0x01,0x09,0x08,		0x45,0x07,0x29	},	//	明治		…	GT_MEIJI
	{ 19120730,	19261224, 1912,		0x01,0x07,0x30,		0x15,0x12,0x24	},	//	大正		…	GT_TAISYOU
	{ 19261225,	19890107, 1926,		0x01,0x12,0x25,		0x64,0x01,0x07	},	//	昭和		…	GT_SYOUWA
// midori 181101 del -->
	//{ 19890108,	29991231, 1989,		0x01,0x01,0x08,		0x00,0x00,0x00	}	//	平成		…	GT_HEISEI
// midori 181101 del <--
// midori 181101 add -->
	{ 19890108,	29991231, 1989,		0x01,0x01,0x08,		0x00,0x00,0x00	},	//	平成		…	GT_HEISEI
	{ 0,		0,			0,		0x00,0x00,0x00,		0x00,0x00,0x00	}
// midori 181101 add <--
};

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	日付変換クラス
//++++++++++++++++++++++++++++++++++++++++++++++++++
class CdateConvert
{
public:
	CdateConvert(void);			//	コンストラクタ
	~CdateConvert(void);		//	デストラクタ

public:
	CString		m_szDate;		//	日付（文字列）："20060730"
	CString		m_szGengou;		//	元号（文字列）："例外","明治","大正","昭和","平成"
	CString		m_szGengouHead;	//	元号（文字列）："例","明","大","昭","平"
	CString		m_szWareki;		//	和暦（文字列）："18"
	CString		m_szSeireki;	//	西暦（文字列）："2006"
	CString		m_szMonth;		//	月（文字列）："07"
	CString		m_szDay;		//	日（文字列）："30"
	
	int			m_nDate;		//	日付（数値）
	int			m_nGengou;		//	元号（数値）
	int			m_nWareki;		//	和暦（数値）
	int			m_nSeireki;		//	西暦（数値）
	int			m_nMonth;		//	月（数値）
	int			m_nDay;			//	日（数値）

	char		m_cGengou;		//	元号（BCD）："0x04"
	char		m_cWareki;		//	和暦（BCD）："0x18"
	char		m_cMonth;		//	月（BCD）："0x07"
	char		m_cDay;			//	日（BCD）："0x30"

public:
	int Convert( int nDate );															//	変換（日付：西暦８桁）
	int Convert( int nDate, char* pcDate, int nFlag );									//	変換（日付：西暦８桁 +バッファ設定 ）
// midori 181101 del -->
//	int Convert( int nDate, int fRekinen );												//　変換（日付：西暦８桁or和暦６桁）
// midori 181101 del <--
	int Convert( char cGengou, char cWareki, char cMonth, char cDay );					//	変換（BCD）
	BOOL ConvertWMD( int nDate, char* pcDate, int nFlag );								//	変換 (「和暦.月.日」フォーマットの昭和対応)
	char ConvertStringToBcd( CString szDate );											//	変換 (文字列→BCD)
	CString ConvertBcdToString( char cDate );											//	変換 (BCD→文字列)
	char GetLastDay( char cGengou , char cWareki , char cMonth );						//	末日取得
	int ConvertBcdSeireki6ToInt( char cSeireki , char cMonth , char cDay );				//	変換 (BCD→int)
// midori 181101 add -->
	char GetNewGengo(char cWareki, char cMonth, char cDay);								//	和暦から元号を取得（改元対応）
// midori 181101 add <--
// midori 97 add -->
	void GetYmDay( char cGengou, char cWareki, char cMonth, BYTE* cDay );				//	日付を取得
// midori 97 add <--

private:
	void CheckDate( char* cGengou, char* cWareki, char* cMonth, char* cDay );			//	日付確認
	void InitData();																	//	データ初期化
	CString GetString( int nDate, int nStart, int nLen );								//	日付を文字列で取得
	int GetValue( int nDate, int nStart, int nLen );									//	日付を数値で取得
	CString GetYearString( int nDate, int fRekinen );									//	「年」文字列取得
	int GetYearValue( int nDate, int fRekinen );										//	「年」数値取得
	CString GetMonthString( int nDate );												//	「月」文字列取得
	int GetMonthValue( int nDate );														//	「月」数値取得
	CString GetDayString( int nDate );													//	「日」文字列取得
	int GetDayValue( int nDate );														//	「日」数値取得
	void ConvertSeirekiToWareki( int nDate, CString* pszGengou,
								 CString* pszGengouHead, int* pnGengou,
								 CString* pszWareki, int* pnWareki );					//	「西暦」→「和暦」変換
	int CheckWarekiData( int nDate, int* pnGengou, CString* pszWareki,
						 int* pnWareki, DC_GENGOU_TYPE nType );							//	指定元号の和暦範囲内か確認（+データ設定）
	int ConvertBcdToInt( char cGengou, char cWareki, char cMonth, char cDay );			//	BCDをintに変換
	BOOL CheckLeapYear( char cGengou , char cWareki );									//	閏年チェック
};

#endif __DATECONVERT_H__