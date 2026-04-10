//--------------------------------------------------
//	dateConvert.cpp
//
//	2006.02.17～
//--------------------------------------------------

#include "StdAfx.h"
#include "dateConvert.h"

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdateConvert::CdateConvert(void)
{
// midori 181101 add -->
	// 平成を書き換えて、新元号を追加する
	if(IsValidKaigen() == TRUE)	{
		// 平成
		_DC_CHANGE_TABLE[4].nEnd		= 20190430;
		_DC_CHANGE_TABLE[4].cEndWareki	= 0x31;
		_DC_CHANGE_TABLE[4].cEndMonth	= 0x04;
		_DC_CHANGE_TABLE[4].cEndDay		= 0x30;
		// 新元号
		_DC_CHANGE_TABLE[5].nStart		= 20190501;
		_DC_CHANGE_TABLE[5].nEnd		= 29991231;
		_DC_CHANGE_TABLE[5].nBasicYear	= 2019;
		_DC_CHANGE_TABLE[5].cStartWareki= 0x01;
		_DC_CHANGE_TABLE[5].cStartMonth	= 0x05;
		_DC_CHANGE_TABLE[5].cStartDay	= 0x01;
		_DC_CHANGE_TABLE[5].cEndWareki	= 0x00;
		_DC_CHANGE_TABLE[5].cEndMonth	= 0x00;
		_DC_CHANGE_TABLE[5].cEndDay		= 0x00;
	}
// midori 181101 add <--
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdateConvert::~CdateConvert(void)
{
}

//**************************************************
//	変換（日付：西暦８桁）
//	【引数】	nDate	…	日付（西暦）
//	【戻値】	0		…	成功
//				0以外	…	失敗
//**************************************************
int CdateConvert::Convert( int nDate )
{	
	//	データ初期化
	InitData();

	//	西暦（文字列）取得
	m_szSeireki = GetYearString( nDate, DC_REKINEN_SEIREKI );		//	西暦（文字列）取得
	m_szMonth = GetMonthString( nDate );			//	月（文字列）取得
	m_szDay = GetDayString( nDate );				//	日（文字列）取得
	m_szDate = m_szSeireki + m_szMonth + m_szDay;	//	日付（文字列）取得

	//	西暦（数値）取得
	m_nDate = nDate;								//	日付（数値）取得
	m_nSeireki = GetYearValue( nDate, DC_REKINEN_SEIREKI );			//	西暦（数値）取得
	m_nMonth = GetMonthValue( nDate );				//	月（数値）取得
	m_nDay = GetDayValue( nDate );					//	日（数値）取得
	
	//	「西暦」→「和暦」変換
	ConvertSeirekiToWareki( nDate, &m_szGengou, &m_szGengouHead, &m_nGengou, &m_szWareki, &m_nWareki );
	
	//	「bcd」に変換
	int_bcd( &m_cGengou, m_nGengou, 1 );			//	元号（BCD）取得
	int_bcd( &m_cWareki, m_nWareki, 1 );			//	和暦（BCD）取得
	int_bcd( &m_cMonth, m_nMonth, 1 );				//	月（BCD）取得
	int_bcd( &m_cDay, m_nDay, 1 );					//	日（BCD）取得
	
	//	戻値を返す（現在は0オンリー）
	return( 0 );
}

//**************************************************
//	変換（日付：西暦８桁 +バッファ設定 ）
//	【引数】	nDate	…	日付（西暦）
//				pcDate	…	バッファ（４バイト固定）
//				nFlag	…	バッファに設定したい値を指定
//								DC_DATE_GENGOU	：元号
//								DC_DATE_WAREKI	：和暦
//								DC_DATE_MONTH	：月
//								DC_DATE_DAY		：日
//								DC_DATE_WM		：和暦、月
//								DC_DATE_WMD		：和暦、月、日
//								DC_DATE_GW		：元号、和暦
//								DC_DATE_GWM		：元号、和暦、月
//								DC_DATE_GWMD	：元号、和
//	【戻値】	0		…	成功
//				0以外	…	失敗
//**************************************************
int CdateConvert::Convert( int nDate, char* pcDate, int nFlag )
{
	int		nRet;	//	戻値
	int		nCnt;	//	カウント
	
	//	変換（日付：西暦８桁）
	nRet = Convert( nDate );

	//	変換（日付：西暦８桁）成功？
	if ( nRet == 0 ){
		
		//	バッファ分ループ
		for( nCnt = 0; nCnt < 4; nCnt++ ){
			pcDate[nCnt] = 0x00;	//	クリア
		}
		
		//	カウント初期化
		nCnt = 0;

		//	元号必要？
		if ( nFlag & DC_DATE_GENGOU ){
			pcDate[nCnt] = m_cGengou;	//	元号を設定
			nCnt++;						//	インクリメント
		}

		//	和暦必要？
		if ( nFlag & DC_DATE_WAREKI ){
			pcDate[nCnt] = m_cWareki;	//	和暦を設定
			nCnt++;						//	インクリメント
		}

		//	月必要？
		if ( nFlag & DC_DATE_MONTH ){
			pcDate[nCnt] = m_cMonth;	//	月を設定
			nCnt++;						//	インクリメント
		}

		//	日必要？
		if ( nFlag & DC_DATE_DAY ){
			pcDate[nCnt] = m_cDay;		//	日を設定
			nCnt++;						//	インクリメント
		}
	}

	//	戻値を返す
	return( nRet );
}

// midori 181101 del -->
////**************************************************
////	変換（日付：西暦８桁 or 和暦６桁）
////	【引数】	nDate	…	日付
////				fRekinen…　暦年サイン
////	【戻値】	0		…	成功
////				0以外	…	失敗
////**************************************************
//int CdateConvert::Convert( int nDate, int fRekinen )
//{	
//	//	データ初期化
//	InitData();
//
//	//	日付（文字列）取得
//	m_szSeireki	= GetYearString( nDate, fRekinen );		//	年（文字列）取得
//	m_szMonth	= GetMonthString( nDate );				//	月（文字列）取得
//	m_szDay		= GetDayString( nDate );				//	日（文字列）取得
//	m_szDate	= m_szSeireki + m_szMonth + m_szDay;	//	日付（文字列）取得
//
//	//	日付（数値）取得
//	m_nDate		= nDate;								//	日付（数値）取得
//	m_nSeireki	= GetYearValue( nDate, fRekinen );		//	年（数値）取得
//	m_nMonth	= GetMonthValue( nDate );				//	月（数値）取得
//	m_nDay		= GetDayValue( nDate );					//	日（数値）取得
//
//	//	暦年サインが西暦なら和暦に変換
//	if( fRekinen != 0 ){
//		ConvertSeirekiToWareki( nDate, &m_szGengou, &m_szGengouHead, &m_nGengou, &m_szWareki, &m_nWareki );
//	}
//	else{	// 和暦の場合、元号が判定できないので「平成」に決め打ち
//		m_szGengou		= DC_GENGOU_HEISEI;
//		m_szGengouHead	= m_szGengou.Left( 2 );
//		m_nWareki		= m_nSeireki;
//		m_nGengou		= m_nWareki;		
//		m_szWareki.Format("%02d", m_nWareki);
//	}
//
//	//	「bcd」に変換
//	int_bcd( &m_cGengou, m_nGengou, 1 );			//	元号（BCD）取得
//	int_bcd( &m_cWareki, m_nWareki, 1 );			//	和暦（BCD）取得
//	int_bcd( &m_cMonth, m_nMonth, 1 );				//	月（BCD）取得
//	int_bcd( &m_cDay, m_nDay, 1 );					//	日（BCD）取得
//	
//	//	戻値を返す（現在は0オンリー）
//	return( 0 );
//}
// midori 181101 del <--

//**************************************************
//	変換（BCD）
//	※元号、和暦、月、日が全て「0x00」だった場合のみ戻値に「0」を返します。
//	【引数】	cGengou		…	元号（BCD）	※「0x00」を指定した場合、自動で「0x04：平成」に設定
//				cWareki		…	和暦（BCD）	※「0x00」を指定した場合、自動で「0x01：1年」に設定
//				cMonth		…	月（BCD）	※「0x00」を指定した場合、自動で「0x01：1月」に設定
//				cDay		…	日（BCD）	※「0x00」を指定した場合、自動で「0x01：1日」に設定
//	【戻値】	日付（数値）
//**************************************************
int CdateConvert::Convert( char cGengou, char cWareki, char cMonth, char cDay )
{
	int		nDate = 0;	//	日付（数値）
	int		nCheck = 0;	//	チェック用
	
	//	元号に「0x00」が指定？
	if ( cGengou == 0x00 ){
// midori 181101 del -->
//		cGengou = 0x04;		//	「平成」に設定
// midori 181101 del <--
// midori 181101 add -->
		// 和暦の年月日から「平成」か「改元」か判断を行う
		cGengou = GetNewGengo(cWareki, cMonth, cDay);
// midori 181101 add <--
		nCheck++;
	}
	
	//	和暦に「0x00」が指定？
	if ( cWareki == 0x00 ){
		cWareki = 0x01;		//	「1年」に設定
		nCheck++;
	}
	
	//	月に「0x00」が指定？
	if ( cMonth == 0x00 ){
		nCheck++;
	}

	//	年に「0x00」が指定？
	if ( cDay == 0x00 ){
		nCheck++;
	}
	
	//	「元号」「和暦」「月」「日」何れかに値が代入されていた？
	if ( nCheck != 4 ){
		
		//	日付確認
		CheckDate( &cGengou, &cWareki, &cMonth, &cDay );
		//	BCDをintに変換
		nDate = ConvertBcdToInt( cGengou, cWareki, cMonth, cDay );
	}

	//	変換（日付：西暦８桁）失敗？
	if ( Convert( nDate ) != 0 ){
		//	日付（数値）をクリア
		nDate = 0;
	}

	//	戻値を返す
	return( nDate );
}

//**************************************************
//	変換（日付：西暦８桁 +バッファ設定 ）
//  ※「和暦.月.日」フォーマットの場合の対応処理
//	→引数の日付（西暦）が「昭和(19890101～19890107)」の範囲内時、「平成の和暦(1)」を設定する
//	→上記範囲外の場合、FALSEで返す(変換した日付は返さない)
//	【引数】	nDate	…	日付（西暦）
//				pcDate	…	バッファ（４バイト固定）
//				nFlag	…　バッファに設定したい値を指定
//	【戻値】	TRUE	…	範囲内(19890101～19890107)
//				0以外	…	範囲外
//**************************************************
BOOL CdateConvert::ConvertWMD( int nDate, char* pcDate, int nFlag )
{
	// 日付が「19890101～19890107」か?
	if ( ( nDate >= DC_DATE_WMD_SSYOUWA ) && ( nDate <= DC_DATE_WMD_ESYOUWA ) ){
		// 日付変換
		Convert( nDate, pcDate, nFlag );
		// 和暦を「元年(0x01)」指定
		pcDate[0] = DC_DATE_SWAREKI;
		// 和暦「元年」対応あり
		return TRUE;
	}

	return FALSE;
}

//**************************************************
//	日付確認（※元号の変わり目で月または日が指定されていない場合は自動で判断）
//	【引数】	cGengou		…	元号（BCD）
//				cWareki		…	和暦（BCD）
//				cMonth		…	月（BCD）
//				cDay		…	日（BCD）
//	【戻値】	なし
//**************************************************
void CdateConvert::CheckDate( char* cGengou, char* cWareki, char* cMonth, char* cDay )
{	
	//	開始年（元年）？
	if ( *cWareki == _DC_CHANGE_TABLE[*cGengou].cStartWareki ){

		//	月の入力なし？または、開始月でかつ日の入力なし？
		if (( *cMonth == 0x00 ) || (( *cMonth == _DC_CHANGE_TABLE[*cGengou].cStartMonth ) && ( *cDay == 0x00 ))){
			*cMonth = _DC_CHANGE_TABLE[*cGengou].cStartMonth;
			*cDay = _DC_CHANGE_TABLE[*cGengou].cStartDay;
		}
	}
	//	終了年？
	else if ( *cWareki == _DC_CHANGE_TABLE[*cGengou].cEndWareki ){

		//	月の入力なし？または、終了月でかつ日の入力なし？
		if (( *cMonth == 0x00 ) || (( *cMonth == _DC_CHANGE_TABLE[*cGengou].cEndMonth ) && ( *cDay == 0x00 ))){
			*cMonth = _DC_CHANGE_TABLE[*cGengou].cEndMonth;
			*cDay = _DC_CHANGE_TABLE[*cGengou].cEndDay;
		}
	}
	//	以外
	else{

		//	月の入力なし？
		if ( *cMonth == 0x00 ){
			*cMonth = 0x01;
		}

		//	年の入力なし？
		if ( *cDay == 0x00 ){
			*cDay = 0x01;
		}
	}
}

// midori 97 add -->
//**************************************************
//	日付の取得
//	【引数】	cGengou		…	元号（BCD）
//				cWareki		…	和暦（BCD）
//				cMonth		…	月（BCD）
//				cDay		…	日（BCD）
//	【戻値】	なし
//**************************************************
void CdateConvert::GetYmDay( char cGengou, char cWareki, char cMonth, BYTE* cDay )
{	
	//	開始年（元年）
	if ( cWareki == _DC_CHANGE_TABLE[cGengou].cStartWareki ){
		//	開始月
		if (cMonth == _DC_CHANGE_TABLE[cGengou].cStartMonth ){
			*cDay = _DC_CHANGE_TABLE[cGengou].cStartDay;
		}
		else {
			*cDay = 0x01;
		}
	}
	//	終了年
	else if ( cWareki == _DC_CHANGE_TABLE[cGengou].cEndWareki ){
		//	終了月
		if (cMonth == _DC_CHANGE_TABLE[cGengou].cEndMonth){
			*cDay = _DC_CHANGE_TABLE[cGengou].cEndDay;
		}
		else {
			*cDay = 0x01;
		}
	}
	//	以外
	else{
		*cDay = 0x01;
	}
}
// midori 97 add <--

//**************************************************
//	データ初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdateConvert::InitData()
{
	m_szDate.Empty();		//	日付（文字列）："20060730"
	m_szGengou.Empty();		//	元号（文字列）："例外","明治","大正","昭和","平成"
	m_szGengouHead.Empty();	//	元号（文字列）："例","明","大","昭","平"
	m_szWareki.Empty();		//	和暦（文字列）："18"
	m_szSeireki.Empty();	//	西暦（文字列）："2006"
	m_szMonth.Empty();		//	月（文字列）："07"
	m_szDay.Empty();		//	日（文字列）："30"
	
	m_nDate = 0;			//	日付（数値）
	m_nGengou = 0;			//	元号（数値）
	m_nWareki = 0;			//	和暦（数値）
	m_nSeireki = 0;			//	西暦（数値）
	m_nMonth = 0;			//	月（数値）
	m_nDay = 0;				//	日（数値）

	m_cGengou = 0x00;		//	元号（BCD）："0x04"
	m_cWareki = 0x00;		//	和暦（BCD）："0x18"
	m_cMonth = 0x00;		//	月（BCD）："0x07"
	m_cDay = 0x00;			//	日（BCD）："0x30"
}

//**************************************************
//	日付を文字列で取得
//	【引数】	nDate	…	日付（例：20060730）
//				nStart	…	文字列取得開始位置
//				nLen	…	文字列取得長
//	【戻値】	日付の文字列
//**************************************************
CString CdateConvert::GetString( int nDate, int nStart, int nLen )
{
	int			nMax;		//	文字列長
	CString		szBuf;		//	バッファ
	CString		szRet;		//	戻値
	
	//	初期化
	szBuf.Empty();
	szRet.Empty();
	
	//	数値を文字列変換
	szBuf.Format( "%d",  nDate );
	//	文字列長取得
	nMax = szBuf.GetLength();
	//	指定位置の文字列を切り取り
	szRet = szBuf.Mid( nMax - nStart, nLen );

	//	戻値を返す
	return( szRet );
}

//**************************************************
//	日付を数値で取得
//	【引数】	nDate	…	日付（例：20060730）
//				nStart	…	文字列取得開始位置
//				nLen	…	文字列取得長
//	【戻値】	日付の数値
//**************************************************
int CdateConvert::GetValue( int nDate, int nStart, int nLen )
{
	return( atoi( GetString( nDate, nStart, nLen ) ) );
}

//**************************************************
//	「年」文字列取得
//	【引数】	nDate	…	日付（例：20060730 or 230730）
//				fRekinen…　暦年サイン 1:西暦／0:和暦
//	【戻値】	「年」文字列
//**************************************************
CString CdateConvert::GetYearString( int nDate, int fRekinen )
{
	int		valStart = 0;
	int		nLen	 = 0;
	CString strDate;

	strDate.Format( "%d", nDate );

	if( fRekinen == DC_REKINEN_SEIREKI){
		valStart = strDate.GetLength();		// 文字列の先頭から
		// 西暦指定なのに、230730のような和暦がくる可能性がある為、取得する「年」の桁数を変更する
		if(valStart == 7)		nLen = 3;		// Ex.)西暦123年04月05日
		else if(valStart == 6)	nLen = 2;		// Ex.)西暦12年03月04日
		else if(valStart == 5)	nLen = 1;		// Ex.)西暦1年02月03日
		else					nLen = 4;		// 4文字分(2006)　正しくはここ
	}
	else{									// 2文字分(23)
		valStart = strDate.GetLength();		// 文字列の先頭から
		if(valStart == 5)	nLen = 1;		// 和暦が1桁台（例：平成1年）
		else				nLen = 2;		// 和暦が2桁台（例：平成10年）
	}

	return( GetString( nDate, valStart, nLen ) );
}

//**************************************************
//	「年」数値取得
//	【引数】	nDate	…	日付（例：20060730 or 230730）
//				fRekinen…　暦年サイン 1:西暦／0:和暦
//	【戻値】	「年」数値
//**************************************************
int CdateConvert::GetYearValue( int nDate, int fRekinen )
{
	int		valStart = 0;
	int		nLen	 = 0;
	CString strDate;

	strDate.Format( "%d", nDate );

	if( fRekinen == DC_REKINEN_SEIREKI ){
		valStart = strDate.GetLength();		// 文字列の先頭から
		// 西暦指定なのに、230730のような和暦がくる可能性がある為、取得する「年」の桁数を変更する
		if(valStart == 7)		nLen = 3;		// Ex.)西暦123年04月05日
		else if(valStart == 6)	nLen = 2;		// Ex.)西暦12年03月04日
		else if(valStart == 5)	nLen = 1;		// Ex.)西暦1年02月03日
		else					nLen = 4;		// 4文字分(2006)　正しくはここ
	}
	else{										// 2文字分(23)
		valStart = strDate.GetLength();			// 文字列の先頭から
		if(valStart == 5)		nLen = 1;		// 和暦が1桁台（例：平成1年）
		else					nLen = 2;		// 和暦が2桁台（例：平成10年）
	}
	return( GetValue( nDate, valStart, nLen ) );
}

//**************************************************
//	「月」文字列取得
//	【引数】	nDate	…	日付（例：20060730）
//	【戻値】	「月」文字列
//**************************************************
CString CdateConvert::GetMonthString( int nDate )
{
	return( GetString( nDate, 4, 2 ) );
}

//**************************************************
//	「月」数値取得
//	【引数】	nDate	…	日付（例：20060730）
//	【戻値】	「月」数値
//**************************************************
int CdateConvert::GetMonthValue( int nDate )
{
	return( GetValue( nDate, 4, 2 ) );
}

//**************************************************
//	「日」文字列取得
//	【引数】	nDate	…	日付（例：20060730）
//	【戻値】	「日」文字列
//**************************************************
CString CdateConvert::GetDayString( int nDate )
{
	return( GetString( nDate, 2, 2 ) );
}

//**************************************************
//	「日」数値取得
//	【引数】	nDate	…	日付（例：20060730）
//	【戻値】	「日」数値
//**************************************************
int CdateConvert::GetDayValue( int nDate )
{
	return( GetValue( nDate, 2, 2 ) );
}

//**************************************************
//	「西暦」→「和暦」変換
//	【引数】	nDate			…	日付（西暦）
//				pszGengou		…	元号（文字列）バッファ
//				pszGengouHead	…	元号（文字列）バッファ
//				pnGengou		…	元号（数値）バッファ
//				pszWareki		…	和暦（文字列）バッファ
//				pnWareki		…	和暦（数値）バッファ
//	【戻値】	なし
//**************************************************
void CdateConvert::ConvertSeirekiToWareki( int nDate, CString* pszGengou, CString* pszGengouHead, int* pnGengou, CString* pszWareki, int* pnWareki )
{
	//	初期化
	*pnGengou = GT_REIGAI;
	*pnWareki = 0;
	*pszGengou = DC_GENGOU_REIGAI;

// midori 181101 add -->
	if(IsValidKaigen() == TRUE)	{
		//	新元号？
		if ( CheckWarekiData( nDate, pnGengou, pszWareki, pnWareki, GT_KAIGEN ) == 1 ){
			CString	cs = *pszGengou;
			db_vd_NoToStrGen(5,cs);
			*pszGengou = cs;
		}
	}
// midori 181101 add <--

	//	平成？
	if ( CheckWarekiData( nDate, pnGengou, pszWareki, pnWareki, GT_HEISEI ) == 1 ){
		*pszGengou = DC_GENGOU_HEISEI;
	}
	
	//	昭和？
	if ( CheckWarekiData( nDate, pnGengou, pszWareki, pnWareki, GT_SYOUWA ) == 1 ){
		*pszGengou = DC_GENGOU_SYOUWA;
	}
	
	//	大正？
	if ( CheckWarekiData( nDate, pnGengou, pszWareki, pnWareki, GT_TAISYOU ) == 1 ){
		*pszGengou = DC_GENGOU_TAISYOU;
	}
	
	//	明治？
	if ( CheckWarekiData( nDate, pnGengou, pszWareki, pnWareki, GT_MEIJI ) == 1 ){
		*pszGengou = DC_GENGOU_MEIJI;
	}
	
	//	元号の頭文字取得
	*pszGengouHead = pszGengou->Left( 2 );
}

//**************************************************
//	指定元号の和暦範囲内か確認（+データ設定）
//	【引数】	nDate		…	日付（西暦）
//				pnGengou	…	元号（数値）バッファ
//				pszWareki	…	和暦（文字列）バッファ
//				pnWareki	…	和暦（数値）バッファ
//				nType		…	元号種
//	【戻値】	0			…	指定元号の和暦範囲外
//				0以外		…	指定元号の和暦範囲内
//**************************************************
int CdateConvert::CheckWarekiData( int nDate, int* pnGengou, CString* pszWareki, int* pnWareki, DC_GENGOU_TYPE nType )
{
	int		nRet = 0;	//	戻値
	int		nSeireki;	//	西暦
	
	//	指定元号の和暦範囲内？
	// 2016.01.12 西暦（8桁）でないなら平成とする
	// 決算書等で西暦指定であるが、12年03月04日等の指定が来る可能性がある）
// midori 181101 del -->
	//if ((( _DC_CHANGE_TABLE[ nType ].nStart <= nDate ) &&
	//	( _DC_CHANGE_TABLE[ nType ].nEnd >= nDate )) ||
	//	(((nDate != 0) && (nDate < 10000000)) && (nType == GT_HEISEI))){
// midori 181101 del <--
// midori 181101 add -->
	if (( _DC_CHANGE_TABLE[ nType ].nStart <= nDate ) &&
		( _DC_CHANGE_TABLE[ nType ].nEnd >= nDate )){
// midori 181101 add <--
		
		//	西暦（数値）を取得
		nSeireki = GetYearValue( nDate, DC_REKINEN_SEIREKI );	
		//	元号（数値）をバッファに設定
		*pnGengou = nType;
		//	和暦（数値）をバッファに設定（ 西暦 - ( 和暦基準年 + 1 ) ）
		*pnWareki = nSeireki - ( _DC_CHANGE_TABLE[ nType ].nBasicYear - 1 );
		//	和暦（文字列）を取得
		pszWareki->Format( "%02d",*pnWareki );
		//	戻値を範囲内に設定
		nRet = 1;
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	BCDをintに変換
//	【引数】	nGengou		…	元号（BCD）
//				nWareki		…	和暦（BCD）
//				nMonth		…	月（BCD）
//				nDay		…	日（BCD）
//	【戻値】
//**************************************************
int CdateConvert::ConvertBcdToInt( char cGengou, char cWareki, char cMonth, char cDay )
{
	int			nRet = 0;	//	戻値
	int			nGengou;	//	元号（数値）
	int			nCnt;		//	カウント用
	int			nBuf[3];	//	バッファ（数値）
// midori 181101 add -->
	int			nCur = 0;	//	現在の元号
// midori 181101 add <--
	CString		szBuf;		//	バッファ（文字列）
	CString		szDate;		//	日付（文字列）
	
	//	初期化
	ZeroMemory( nBuf, sizeof( nBuf ) );
	szDate.Empty(); 
	
	//	元号（数値）取得
	nGengou = bcd_int( &cGengou, 1 );
	
	//	元号が範囲外？
// midori 181101 del -->
	//if (( nGengou < GT_MEIJI ) || ( nGengou > GT_HEISEI )){
	//	//	平成に設定
	//	nGengou = GT_HEISEI;
	//}
// midori 181101 del <--
// midori 181101 add -->
	if(IsValidKaigen() == TRUE)	nCur = GT_KAIGEN;
	else						nCur = GT_HEISEI;
	if((nGengou < GT_MEIJI) || (nGengou > nCur))	{
		nGengou = nCur;
	}
// midori 181101 add <--

	//	西暦（数値）を設定
	nBuf[0] = bcd_int( &cWareki, 1 ) + ( _DC_CHANGE_TABLE[nGengou].nBasicYear - 1 );
	//	月（数値）を設定
	nBuf[1] = bcd_int( &cMonth, 1 );
	//	日（数値）を設定
	nBuf[2] = bcd_int( &cDay, 1 );
	
	//	西暦～月～日にループ
	for( nCnt = 0; nCnt < 3; nCnt++ ){
		szBuf.Empty();							//	バッファ（文字列）初期化
		szBuf.Format( "%02d", nBuf[nCnt] );		//	数値を文字列に変換
		szDate += szBuf;						//	日付（文字列）に連結
	}
	
	//	日付（文字列）を日付（数値）に変換
	nRet = atoi( szDate );

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	変換(文字列数値をBCD型に変換)
// 【引数】		szDate	…	文字列(日付)
// 【戻値】		BCD型の数値
//**************************************************
char CdateConvert::ConvertStringToBcd( CString szDate )
{
	int		nDate = 0;			// int型に変換された文字列
	char	cRet = 0x00;	// 戻値

	// 文字列が空の場合は0x00で返す
	if ( szDate != "" ){
		nDate = atoi( szDate );
		int_bcd( &cRet , nDate , 1 );
	}

	return ( cRet );
}

//**************************************************
//	変換(文字列数値をBCD型に変換)
// 【引数】		cDate	…	BCD
// 【戻値】		文字列
//**************************************************
CString CdateConvert::ConvertBcdToString( char cDate )
{
	int		nDate = 0;		// int型に変換されたBCD型の値
	CString	strRet = "";	// 戻値
	
	if ( cDate != 0x00 ){
		// BCDから数値を取得
		nDate = bcd_int( &cDate , 1 );
		// 数値を文字列に変換
		strRet.Format("%d", nDate);
	}

	return ( strRet );
}

//**************************************************
//	閏年判定
//	【引数】	cGengou		…	元号（BCD）
//				cWareki		…	和暦（BCD）
//	【戻値】	TRUE		…　閏年
//				FALSE		…	閏年では無い
//**************************************************
BOOL CdateConvert::CheckLeapYear( char cGengou , char cWareki )
{
	int intYear = 0;		// 西暦
	BOOL blnRet  = FALSE;	// 戻り値

	// 引数から西暦を取得
	intYear = Convert( cGengou , cWareki , 0x00 , 0x00 );

	// 取得した西暦の「年」のみを取得(月、日は省く)
	intYear = intYear / 10000;

	// 閏年か判定
	if ( ( intYear % 4 == 0 ) && ( intYear % 100 != 0 ) || ( intYear % 400 == 0 ) ){
		blnRet = TRUE;
	}
	else{
		blnRet = FALSE;
	}

    return blnRet;
}

//**************************************************
//	対象年月から末日取得
//	【引数】	cGengou		…	元号（BCD）
//				cWareki		…	和暦（BCD）
//				cMonth		…	月（BCD）
//	【戻値】	BCD型の数値
//**************************************************
char CdateConvert::GetLastDay( char cGengou , char cWareki , char cMonth )
{
	char cRet = 0x00;		// 戻り値
	int intMonth = 0;		// 月
	
	// 選択月から末日を求める
	intMonth = bcd_int( &cMonth, 1 );
	switch( intMonth ){

	// 末日が「31日」の月
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		cRet = 0x31;
		break;
	// 末日が「30日」の月
	case 4:
	case 6:
	case 9:
	case 11:
		cRet = 0x30;
		break;
	// 対象月が２月の場合、閏年を考慮
	case 2:
		// 閏年
		if ( CheckLeapYear( cGengou , cWareki ) == TRUE ){
			cRet = 0x29;
		}
		// 閏年でない
		else{
			cRet = 0x28;
		}
	}

	return cRet;
}

//**************************************************
//	変換(BCD型の西暦数値をint型に変換)
// 【引数】		cSeireki	…	西暦（BCD）
//				cMonth		…	月（BCD）
//				cDay		…	日（BCD）
// 【戻値】		文字列
//**************************************************
int CdateConvert::ConvertBcdSeireki6ToInt( char cSeireki , char cMonth , char cDay )
{
	int			intTemp[3];
	CString		strTemp;

	intTemp[0] = bcd_int(&cSeireki, 1);		// yy（2桁）
	intTemp[1] = bcd_int(&cMonth, 1);		// mm
	intTemp[2] = bcd_int(&cDay, 1);			// dd

	// 20yymmdd
	strTemp.Format("20%02d%02d%02d", intTemp[0], intTemp[1], intTemp[2]);
	
	return atoi(strTemp);
}

// midori 181101 add -->
//**************************************************
//	和暦の年月日から元号を返す
// 【引数】		cWareki		…	和暦（BCD）
//				cMonth		…	月（BCD）
//				cDay		…	日（BCD）
// 【戻値】		元号番号
//**************************************************
char CdateConvert::GetNewGengo(char cWareki, char cMonth, char cDay)
{
	int		yy = 0;
	int		mm = 0;
	int		dd = 0;
	int		ymd = 0;
	char	gengo = 0x00;

	if(IsValidKaigen() == FALSE) {
		gengo = 0x04;
		return(gengo);
	}

	yy = bcd_int( &cWareki, 1 );
	mm = bcd_int( &cMonth, 1 );
	dd = bcd_int( &cDay, 1 );

	// 年月日の入力あり
	if(yy != 0 && mm != 0 && dd != 0) {
		ymd = (yy * 10000) + (mm * 100) + dd;
		if(ymd <= 10107)			gengo = 4;		// 平成
		else if(ymd <= 10430)		gengo = 4;		// 平成
		else if(ymd <= 191231)		gengo = 5;		// 新元号
		else if(ymd <= 310430)		gengo = 4;		// 平成
		else						gengo = 5;		// 新元号
	}
	// 年月の入力あり
	else if(yy != 0 && mm != 0) {
		ymd = (yy * 10000) + (mm * 100);
		if(ymd <= 10100)			gengo = 4;		// 平成
		else if(ymd <= 10400)		gengo = 4;		// 平成
		else if(ymd <= 191200)		gengo = 5;		// 新元号
		else if(ymd <= 310400)		gengo = 4;		// 平成
		else						gengo = 5;		// 新元号
	}
	// 年の入力あり
	else if(yy != 0) {
		ymd = (yy * 10000);
		if(ymd <= 10000)			gengo = 4;		// 平成
		else if(ymd <= 190000)		gengo = 5;		// 新元号
		else if(ymd <= 310000)		gengo = 4;		// 平成
		else						gengo = 5;		// 新元号
	}
	// 年月日に入力なし
	else {
		gengo = 5;		// 新元号
	}
	return (gengo);
}
// midori 181101 add <--
