#pragma once
using namespace std;

/*
	日付コントロール処理関連
	CTimeクラスを利用し期間計算などを行う

	ICSDB標準のINT日付値や、旧AtlasプロなBYTE配列との相互変換や文字列書式化など用意
*/

typedef pair<CTime,CTime> TimePair;
typedef vector<TimePair> TimePairVector;

class CTimeControl
{
public:

	// 引数 ICSDB準拠
	static CTime GetCTime(int ymd)
	{
		int md = ymd % 10000;

		int y = ymd / 10000;	  // 年
		int m = md / 100;		  // 月
		int d = md % 100;		  // 日

		if( !y || !m || !d  ) return CTime();

		return CTime( y, m, d, 0, 0, 0 );
	}

	// 引数 Atlasタイプ
	static CTime GetCTime(const UCHAR* ymd)
	{
		return GetCTime( GetIntTime(ymd) );
	}

	// 引数 文字列 YYYYMMDD
	static CTime GetCTime(CString ymd)
	{
		return GetCTime( atoi(ymd) );
	}

	// 戻り値 ICSDB準拠
	static int GetIntTime(CTime time)
	{
		int y = time.GetYear() * 10000;	  // 年
		int m = time.GetMonth()  * 100;	  // 月
		int d = time.GetDay();			  // 日

		return y + m + d;
	}

	// 引数 Atlasタイプ
	// 戻り値 ICSDB準拠
	static int GetIntTime(const UCHAR* ymd)
	{
		CString strymd;
		strymd.Format( "%02x%02x%02x", ymd[0]&0xff,ymd[1]&0xff,ymd[2]&0xff);
		return GetSeirekiYMD(atoi(strymd));
	}

	// 引数 ymd Atlasタイプ
	static void SetYMD(UCHAR* ymd,CTime time)
	{
		int date = GetWarekiYMD(time);
		//int_bcd( ymd, date, 3 );
		int_bcd( ymd, date, 4 );
	}

	static int GetDaySpan(int date_start, int date_end)
	{
		CTimeSpan span = GetCTime(date_end) - GetCTime(date_start);
		return (int)span.GetDays() + 1;
	}

	static CTime GetPastTime(CTime time, int pastmonth ) // 年跨ぎ対応
	{
		int y = time.GetYear();
		int m = time.GetMonth();
		int d = time.GetDay();

		int year  = pastmonth / 12;
		int month = pastmonth % 12;

		y += year;
		m += month;

		if( m > 12 ){
			y++; 
			m = m % 12;
		}

		return CTime( y, m, d, 0, 0, 0 );
	}

	static CTime GetFirstDay(CTime time)
	{
		int y = time.GetYear();
		int m = time.GetMonth();
	
		return CTime( y, m, 1, 0, 0, 0 );
	}

	static CTime GetMatsubi( CTime time,int past )
	{
		CTimeSpan span(1, 0, 0, 0);
		return GetPastTime(time,past) - span;
	}

	static TimePair GetPastSmdEmd(CTime time,int past)
	{
		return TimePair(time,GetMatsubi(time,past) );
	}
	
	static int GetWarekiYMD(CTime time)
	{
		return GetWarekiYMD(GetIntTime(time));
	}

	static int GetWarekiYMD(int iymd)
	{
		int ymd = 0;
		int g = 0;
		//db_datecnv( iymd, &ymd, 0, 0);
		db_datecnvGen( 0, iymd, &g, &ymd, 0, 0);
		//return ymd;
		return (g*1000000)+ymd;
	}

	static int GetSeirekiYMD(CTime time)
	{
		return GetSeirekiYMD(GetIntTime(time));
	}

	static int GetSeirekiYMD(int iymd)
	{
		int ymd = 0;
		int g = 0;
		db_datecnvGen( 0, iymd, &g, &ymd, 1, 0);
		return ymd;
	}

	static CString GetDateStr(CTime time, CString format )
	{
		int ymd = GetWarekiYMD(time);

		ymd %= 1000000;

		int md = ymd % 10000;

		int y = ymd / 10000;	  // 年
		int m = md / 100;		  // 月
		int d = md % 100;		  // 日

		CString str;
		str.Format(format,y,m,d);
		return str;
	}

	static CString GetDateStrPair( TimePair time_pair , CString format = "%2d年%2d月%2d日" )
	{
		CString str;
		str.Format( "%s ～ %s" , GetDateStr(time_pair.first,format ), GetDateStr( time_pair.second,format ) );
		return str;
	}

	static void SetDateYMD( UCHAR* smd,UCHAR* emd, TimePair time_pair )
	{
		if( TimePair() == time_pair ) return;

		SetYMD(smd,time_pair.first);
		SetYMD(emd,time_pair.second);
	}

	static TimePair GetTimePair(UCHAR* smd,UCHAR* emd)
	{
		return TimePair(GetCTime(smd),GetCTime(emd));
	}
};