#include "stdafx.h"
#include "SyzUtil.h"

#include "dbalter8.h"

//-----------------------------------------------------------------------------
// CString 型の文字列作成
//-----------------------------------------------------------------------------
// 引数	str	：	元文字列
//		len	：	文字列長
//-----------------------------------------------------------------------------
// 返送値	：	CString 型の文字列
//-----------------------------------------------------------------------------
inline CString MKString( char* str,int len )
{
	char temp[512] = {0};
	if( len < 512 ){
		memmove( temp, str, len );
	}
	return temp;
}

//-----------------------------------------------------------------------------
// 改行を目印にして１行データをを複数行分割
//-----------------------------------------------------------------------------
// 引数	CStrArray	：	
//		cmnt		：	
//-----------------------------------------------------------------------------
void SplitCmntToArray( CStringArray& CStrArray, CString cmnt )
{
	while( cmnt.GetLength() ){
		CString AddStr;
		int kaigyo = cmnt.Find("\r\n");
		if( kaigyo != -1 ){
			AddStr = cmnt.Left(kaigyo);
			cmnt = cmnt.Right(cmnt.GetLength() - kaigyo - 2 );
		}else{
			AddStr = cmnt;
			cmnt.Empty();
		}

		CStrArray.Add(AddStr);
		TRACE("%s\n",AddStr);
	}
}

CSyzUtil::CSyzUtil()
{
	m_Arith.l_defn( 0x16 );
}

CSyzUtil::~CSyzUtil()
{
}

//-----------------------------------------------------------------------------
// 文字列を６倍長に変換
//-----------------------------------------------------------------------------
// 引数	d_val	：	金額（６倍長　バイナリ）
//		s_val	：	金額（文字列）
//-----------------------------------------------------------------------------
void CSyzUtil::val_to_bin( unsigned char *d_val, CString s_val )
{
	char	asci[20] = {0};

	memset( d_val, '\0', 6 );
	strcpy_s( asci, (LPCTSTR)s_val );
	m_Arith.l_input( d_val, asci );
}

//-----------------------------------------------------------------------------
// ６倍長を文字列に変換
//-----------------------------------------------------------------------------
// 引数	d_val	：	金額（文字列）
//		s_val	：	金額（６倍長　バイナリ）
//-----------------------------------------------------------------------------
void CSyzUtil::val_to_asci( CString *d_val, unsigned char *s_val )
{
	char	asci[20];

	d_val->Empty();
	memset( asci, '\0', sizeof( asci ) );
	m_Arith.l_print( asci, s_val, "SSSSSSSSSSSSS9" );
	*d_val += asci;
	d_val->TrimLeft();
}

//-----------------------------------------------------------------------------
// ６倍長を数値に変換
//-----------------------------------------------------------------------------
// 引数	s_val	：	変換元６倍長
//-----------------------------------------------------------------------------
// 返送値		：	変換後の数値
//-----------------------------------------------------------------------------
int	CSyzUtil::val_to_int( unsigned char *s_val )
{
	int rt = 0;

	char	asci[20] = {0};

	m_Arith.l_print( asci, s_val, "SSSSSSSSSSSSS9" );
	rt = atoi( asci );

	return rt;
}

//-----------------------------------------------------------------------------
// ６倍長　切り捨て
//-----------------------------------------------------------------------------
// 引数	des	：	
//		sou	：	
//		sgn	：	サイン
//					0x0:千円未満 0x?0:切り捨て
//					0x1:百円未満 0x?1:切り上げ
//					0x2:十円未満 0x?2:四捨五入
//-----------------------------------------------------------------------------
void CSyzUtil::l_calq( char *des, char *sou, char sgn )
{
	char 	buf1[10]={0}, buf2[10]={0}, val100[10]={0}, val10[10]={0}, val4[10]={0};
	int		msign;
		
	memset( buf1,   '\0', sizeof( buf1 ) );
	memset( val100, '\0', sizeof( val100 ) );
	memset( val10,  '\0', sizeof( val10 ) );
	memmove( buf1, sou, 6 );
	
	// 計算
	switch( sgn&0xf0 ){
		case 0x00 :
			m_Arith.l_input( val100, _T("1000") );
			m_Arith.l_input( val10, _T("100") );
			break;
		case 0x10 :
			m_Arith.l_input( val100, "100" );
			m_Arith.l_input( val10, "10" );
			break;
		default   :
			m_Arith.l_input( val100, "10" );
			m_Arith.l_input( val10, "0" );
			break;
	}
	
	// 計算
	if( (sgn &= 0x03) ){
		m_Arith.l_input( val4,  "4" );
		
		if( (msign = m_Arith.l_test(buf1)) < 0 ){
			m_Arith.l_neg( buf1 );
		}
		m_Arith.l_mod( buf2, buf1, val100 );
		m_Arith.l_div( buf2, buf2, val10 );
		
		if( sgn == 1 ){			// 切り上げ
			if( m_Arith.l_test(buf2) ){
				m_Arith.l_input( buf2, "1" );
			}
		}
		else if( sgn == 2 ){	// 四捨五入
			m_Arith.l_sub( buf2, buf2, val4 );
			if( m_Arith.l_test( buf2 ) > 0 ){
				m_Arith.l_input( buf2, "1" );
			}
			else{
				m_Arith.l_clear( buf2 );
			}
		}
		m_Arith.l_div( buf1, buf1, val100 );
		m_Arith.l_add( buf1, buf1, buf2 );
		m_Arith.l_mul( buf1, buf1, val100 );
		if( msign < 0 ){
			m_Arith.l_neg( buf1 );
		}
	}
	else {						// 切り捨て
		m_Arith.l_div( buf1, buf1, val100 );
		m_Arith.l_mul( buf1, buf1, val100 );
	}
	
	memmove( des, buf1, 6 );
}

//-----------------------------------------------------------------------------
// CString　から　Char へ
//-----------------------------------------------------------------------------
// 引数	d_asci	：	変換先
//		s_asci	：	変換元
//		len		：	変換文字列長
//-----------------------------------------------------------------------------
void CSyzUtil::cstring_to_char( unsigned char *d_asci, CString s_asci, int len )
{
	char	asci[128] = {0};
	int		cnt;

	memset( d_asci, '\0', len );
	memset( asci, '\0', sizeof(asci) );
	strcpy_s( asci, (LPCTSTR)s_asci );
	if( (cnt = (int)strlen(asci)) ){
		if( len < cnt ){
			cnt = len;
		}
		memmove( d_asci, asci, cnt );
	}
}

//-----------------------------------------------------------------------------
// 文字列をBCDにパック
//-----------------------------------------------------------------------------
// 引数	bcdbf	：	BCD
//		ascbf	：	文字列
//		len		：	文字数
//-----------------------------------------------------------------------------
void CSyzUtil::asci_to_bcd( unsigned char *bcdbf, CString *ascbf, int len )
{
	char	asci[20]={0}, bcde[10]={0};
	int		length = ascbf->GetLength();
	memmove( asci, *ascbf, length );
	m_Arith.l_pack( bcde, asci, len );
	memmove( bcdbf, bcde, len );
}

//-----------------------------------------------------------------------------
// 漢字文字列検索（内容的には全角スペースカット）
//-----------------------------------------------------------------------------
// 引数	DKnji	：	スペースカット後
//		SKnji	：	スペースカット前
//		Moji	：	最大文字数
//-----------------------------------------------------------------------------
// 返送値　		：	文字数
//-----------------------------------------------------------------------------
int	CSyzUtil::MojiSearch(  char *DKnji, char *SKnji, int Moji )
{
	int		i, j;
	char	buf[256] = {0};

	// 漢字スペースをカット
	for( i=0, j=0; i!=Moji; i++ ){
		if( *(SKnji+(i*2)) == 0x00 ){
			break;
		}
		if( strncmp( (SKnji+(i*2)), "　", 2 ) ){
			memmove( &buf[j*2], (SKnji+(i*2)), 2 );
			j++;
		}
	}
	if( j ){
		memmove( DKnji, buf, j*2 );
	}
	return (j*2);
}

//-----------------------------------------------------------------------------
// 文字列の後ろの全角・半角スペースカット後のバッファ数を取得
//-----------------------------------------------------------------------------
// 引数		buf		：	検索対象
//			len		：	バッファ長
//-----------------------------------------------------------------------------
// 返送値			：	実バッファ長
//-----------------------------------------------------------------------------
int CSyzUtil::MixSpaceCutLength( char *buf, int len )
{
	int i = len - 1;
	for( ;i>=0 ; i-- ){
		if( i && ((buf[i-1]&0xff)==0x81) && ((buf[i]&0xff)==0x40) ){
			// 全角スペース
			i--;
		}
		else{
			if( ((buf[i]&0xff)==0x00) || ((buf[i]&0xff)==0x20/*半角スペース*/) ){
				continue;
			}
			else{
				break;
			}
		}
	}
	if( i >= 0 ){
		return( i + 1 );
	}
	else{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// 課税方式の取得
//-----------------------------------------------------------------------------
// 引数	pZvol	：	DBボリュームラベル
//-----------------------------------------------------------------------------
// 返送値		：	課税方式
//-----------------------------------------------------------------------------
EnumIdIcsShKazeihoushiki CSyzUtil::GetKazeihoushiki( CZVOLUME *pZvol )
{
	ASSERT( pZvol );

	if( (pZvol->s_sgn2&0x0f) == 0x00 ){
		return ID_ICSSH_GENSOKU;
	}
	else if( (pZvol->s_sgn2&0x0f) == 0x01 ){
		return ID_ICSSH_MENZEI;
	}
	else{
		return ID_ICSSH_KANNI;
	}
}

//-----------------------------------------------------------------------------
// 消費税申告書：非連動マスター？
//-----------------------------------------------------------------------------
// 引数		pZvol	：	DBボリュームラベル
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	非連動マスター
//			FALSE	：	非連動マスターでない
//-----------------------------------------------------------------------------
BOOL CSyzUtil::IsUnConnectMst( CZVOLUME *pZvol )
{
	ASSERT( pZvol );
	if( pZvol->sub_sw&0x20000 ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 消費税申告書：非連動マスターで簡易課税マスター？
//-----------------------------------------------------------------------------
// 引数		pZvol	：	DBボリュームラベル
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	該当マスター
//			FALSE	：	該当マスターでない
//-----------------------------------------------------------------------------
BOOL CSyzUtil::IsUnConnectKaniMst( CZVOLUME *pZvol )
{
	ASSERT( pZvol );
	if( IsUnConnectMst(pZvol) && (GetKazeihoushiki(pZvol)==ID_ICSSH_KANNI) ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 個人決算書対応マスター？
//-----------------------------------------------------------------------------
// 引数		pZvol	：	DBボリュームラベル
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	該当マスター
//			FALSE	：	該当マスターでない
//-----------------------------------------------------------------------------
BOOL CSyzUtil::IsBasicMst( CZVOLUME *pZvol )
{
	ASSERT( pZvol );
	if( ((pZvol->apno&0xf0)==0x10) || pZvol->ind_type ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// ６倍長を文字列に
//-----------------------------------------------------------------------------
// 引数	num	：	変換元
//-----------------------------------------------------------------------------
// 返送値	：	変換後文字列
//-----------------------------------------------------------------------------
CString CSyzUtil::numstring( char* num )
{
	char buf[128] = {0};
	m_Arith.l_print( buf, num, _T("SSSSSSSSSSSS9") );

	return buf;
}

//-----------------------------------------------------------------------------
// 掛け算
//-----------------------------------------------------------------------------
// 引数	dest	：	演算結果
//		a		：	被演算数１（6倍長）
//		b		：	被演算数２（6倍長）
//-----------------------------------------------------------------------------
void CSyzUtil::l_multi_d( char* dest, char* a, char* b )
{
	CString ans;
	ans.Format( _T("%.0f"), strtod(numstring(a) , 0) * strtod(numstring(b) , 0) );

	m_Arith.l_input( dest,(LPSTR)(LPCSTR)ans );
}

//-----------------------------------------------------------------------------
// 税率計算
//-----------------------------------------------------------------------------
// 引数	des			：	結果
//		sou			：	数値
//		sgn			：	0 = 税込み    
//						1 = 税抜き
//						2 = 5 / 100   
//						3 = 3 / 106 （特別消費税）
//						4 = 4 / 100 + 5
//						5 = (税率/2) / 100 */
//						6 = 4 / 100;
//						7 = 25 / 100
//						8 = 17 / 63
//						9 = 63 / 1080
//						10= 8 / 100
//						11= 624 / 10800		yoshida
//						12= 78 / 1100		yoshida
//						13= 22 / 78		地方消費税分（10％）
//						14= 10 / 100	shimizu
//		ztno		：	税率テ－ブル　＃		
//					：	軽減税率の場合（6.24％）なので、62を内部で624に変換する、（charで持たれている為）
//		hagaku		：	0 = 切り捨て　1 = 切り上げ　2 = 四捨五入
//		isUseDotnet	：	.net の関数を使うかどうか
//-----------------------------------------------------------------------------
void CSyzUtil::percent( char *des, char *sou, int sgn, char ztno, char hagaku, BOOL isUseDotnet/*=TRUE*/ )
{
	char	t_l, t_l2, xbuf[10];
	char	buf1[10]={0}, buf2[10]={0}, buf3[10]={0}, buf4[10]={0}, val10[10]={0}, val4[10]={0};
	int		msign;
	short	zeirit;
	
	t_l = m_Arith.l_retn();
	if( (t_l&0x0f) < 6 ){
		t_l2 = (t_l&0xf0) | 6;
		m_Arith.l_defn( t_l2 );
		m_Arith.l_let( buf1, sou, t_l );
	}
	else{
		memmove( buf1, sou, (0x0f&t_l) );
	}
	
	if( ztno < 0 ){
		zeirit = 60;
	}
	else if( ztno == 30 ){
		zeirit = 30;
	}
	else if( ztno == 40 ){
		zeirit = 40;
	}
	else if( ztno == 63 ){
		zeirit = 63;
	}
//--> '14.02.12 INS START
	else if( ztno == 25 ){
		zeirit = 25;
	}
	else if( ztno == 17 ){
		zeirit = 17;
	}
//<-- '14.02.12 INS END

//----->yoshida
	else if (ztno == 62) {
		zeirit = 624;
	}
	else if (ztno == 78) {
		zeirit = 78;
	}
	else if (ztno == 100) {
		zeirit = 100;
	}
//<-------------




	else{
/*- '13.01.18 -*/
//		zeirit = TBhead.SVzper[ztno];
/*-------------*/
		if( ztno == 0 ){
			zeirit = 50;
		}
		else if( ztno == 1 ){
			zeirit = 30;
		}
		else if( ztno == 2 ){
			zeirit = 0;
		}
/*-------------*/
	}
	
	if( (sgn==0) || (sgn==1) || (sgn==3) || (sgn==4) ){
		sprintf_s( xbuf, sizeof( xbuf ), _T("%d"), zeirit );
	}
	else if( sgn == 5 ){
		sprintf_s( xbuf, sizeof( xbuf ), _T("%d"), zeirit/2 );
	}
	else if( sgn == 2 ){
		strcpy_s(  xbuf, _T("50") );
	}
	else if( sgn == 6 ){
		strcpy_s(  xbuf, _T("40") );
	}
//--> '14.02.12 INS START
	else if( sgn == 7 ){
		strcpy_s(  xbuf, _T("25") );
	}
	else if( sgn == 8 ){
		strcpy_s(  xbuf, _T("17") );
	}
	else if( sgn == 9 ){
		sprintf_s( xbuf, sizeof( xbuf ), _T("%d"), zeirit );
	}
//<-- '14.02.12 INS END
//--> '14.10.21 INS START
	else if( sgn == 10 ){
		strcpy_s(  xbuf, _T("80") );
	}
//<-- '14.10.21 INS END

//---->yoshida
	else if (sgn == 11) {//軽減税率８％
		sprintf_s(xbuf, sizeof(xbuf), _T("%d"), zeirit);
	}

	else if (sgn == 12) {//１０％
		sprintf_s(xbuf, sizeof(xbuf), _T("%d"), zeirit);
	}
//<-----------
	else if( sgn == 13 ){
		strcpy_s(  xbuf, _T("22") );
	}
	else if( sgn == 14 ){
		//strcpy_s(  xbuf, _T("100") );
		strcpy_s (  xbuf, _T( "10" ) );
	}

	m_Arith.l_input( buf2, xbuf );

	hagaku &= 0x03;
	
	if( sgn == 0 ){
		sprintf_s( xbuf, sizeof(xbuf), _T("%d"), zeirit + 1000 );
	}
	else if( sgn == 3 ){
		sprintf_s( xbuf, sizeof(xbuf), _T("%d"), zeirit * 2 + 1000 );
	}
	else if( sgn == 4 ){
		sprintf_s( xbuf, sizeof(xbuf), _T("%d"), 50 + 1000 );
	}
//-- '14.10.21 --
//	else if( sgn == 1 || sgn == 2 || sgn == 5 || sgn == 6 ){
//---------------
//---yoshida---
//税抜き棚卸で6.24％の時の分母が10000となる為、税抜き部分だけ分離させた
//	else if( sgn == 1 || sgn == 2 || sgn == 5 || sgn == 6 || sgn == 10 ){
//---------------
	else if (sgn == 1) {//税抜き
		if (ztno == 62) {//6.24％の時
			strcpy_s(xbuf, _T("10000"));//分母10000に
		}
		else{
			strcpy_s(xbuf, _T("1000"));
		}
	}
	else if (sgn == 2 || sgn == 5 || sgn == 6 || sgn == 10){
		strcpy_s( xbuf, _T("1000") );
	}
//----------------

//--> '14.02.12 INS START
	else if( sgn == 7 ){
		strcpy_s( xbuf, _T("100") );
	}
	else if( sgn == 8 ){
		strcpy_s( xbuf, _T("63") );
	}
	else if( sgn == 9 ){
		sprintf_s( xbuf, sizeof(xbuf), _T("%d"), 80 + 1000 );
	}
//<-- '14.02.12 INS END

//------>yoshida
	else if (sgn == 11) {
		sprintf_s(xbuf, sizeof(xbuf), _T("%d"), 800 + 10000);
	}

	else if (sgn == 12) {
		sprintf_s(xbuf, sizeof(xbuf), _T("%d"), 100 + 1000);
	}
//<-------------
	else if( sgn == 13 ){
		strcpy_s( xbuf, _T("78") );
	}
	else if( sgn == 14 ){
		//strcpy_s( xbuf, _T("80") );
		strcpy_s ( xbuf, _T( "100" ) );
	}

	m_Arith.l_input( buf3, xbuf );

	// 計算
	if( hagaku ){
		m_Arith.l_input( val10, _T("10") );
		m_Arith.l_input( val4,  _T("4") );
		
		if( (msign=m_Arith.l_test(buf1)) < 0 ){
			m_Arith.l_neg( buf1 );
		}
		m_Arith.l_mul( buf1, buf1, buf2 );
		
		m_Arith.l_mod( buf4, buf1, buf3 );
		l_multi_d( buf4, buf4, val10 );
		m_Arith.l_div( buf4, buf4, buf3 );
		if( hagaku == 1 ){		// 切り上げ
			if( m_Arith.l_test(buf4) ){
				m_Arith.l_input( buf4, _T("1") );
			}
		}
		else if( hagaku == 2 ){	// 四捨五入
			m_Arith.l_sub( buf4, buf4, val4 );
			if( m_Arith.l_test(buf4) > 0 ){
				m_Arith.l_input( buf4, _T("1") );
			}
			else{
				m_Arith.l_clear( buf4 );
			}
		}
		m_Arith.l_div( buf1, buf1, buf3 );
		m_Arith.l_add( buf1, buf1, buf4 );
		if( msign < 0 ){
			m_Arith.l_neg( buf1 );
		}
	}
	else{
/*- '14.03.13 -*/
//		l_multi_d( buf1, buf1, buf2 );
//		m_Arith.l_div( buf1, buf1, buf3 );
/*-------------*/
		if( isUseDotnet ){
			char	tmpbuf[10]={0};
			l_pardotnet( tmpbuf, buf1, buf2, buf3 );
			memmove( buf1, tmpbuf, sizeof(buf1) );
		}
		else{
			l_multi_d( buf1, buf1, buf2 );
			m_Arith.l_div( buf1, buf1, buf3 );
		}
/*-------------*/
	}
	
	if( (t_l&0x0f) < 6 ){
		m_Arith.l_defn( t_l );
		m_Arith.l_let( des, buf1, t_l2 );
	}
	else{
		memmove( des, buf1, 0x0f & t_l );
	}
}

//-----------------------------------------------------------------------------
// ６倍長　集計　モジュ－ル
//-----------------------------------------------------------------------------
// 引数	dis		：	計算結果1
//		sou		：	
//		buns	：	
//		bunb	：	
//-----------------------------------------------------------------------------
void CSyzUtil::l_6calq( char *dis, char *sou, char *buns, char *bunb )
{
	char 	t_l, t_l2, WK[6]={0}, WS[6]={0}, WB[6]={0};
	
	t_l = m_Arith.l_retn();
	if( (t_l&0x0f) < 6 ){
		t_l2 = (t_l&0xf0) | 6;
		m_Arith.l_defn( t_l2 );
		m_Arith.l_let( WS, buns, t_l );
		m_Arith.l_let( WB, bunb, t_l );
		m_Arith.l_let( WK, sou,  t_l );
	}
	else{
		memmove( WS, buns, (0x0f&t_l) );
		memmove( WB, bunb, (0x0f&t_l) );
		memmove( WK, sou,  (0x0f&t_l) );
	}
	
	m_Arith.l_mul( WK, WK, WS );	// sou = sou * 分子
	m_Arith.l_div( WK, WK, WB );	// sou = sou / 分母
	if( (t_l&0x0f) < 6 ) {			// dis = sou
		m_Arith.l_defn( t_l );
		m_Arith.l_let( dis, WK, t_l2 );
	}
	else{
		memmove( dis, WK, (0x0f&t_l) );
	}
}

//-----------------------------------------------------------------------------
// 整数（小数点含む）入力
//-----------------------------------------------------------------------------
// 引数	hiln	：	整数部 桁数
//		loln	：	小数部 桁数
//		xbuf	：	元文字列
//		per1	：	結果
//-----------------------------------------------------------------------------
void CSyzUtil::perinp( short hiln, short loln, char *xbuf, short *per1 )
{
	char	hlbf[20]={0},	// 整数の部
			lobf[20]={0};	// 小数点以下の部
	short	hlno, lono, dgno;
	int		i, j, k, lng;

	// 初期設定
	memset( lobf, 0x30, loln );			// 小数点以下の[0x30]クリア
	lng = ( hiln + loln + 1 );			// 最大桁数小数点を含む
	for( i=0, dgno=1; i!=loln; i++ ){	// 小数点以下を整数にする為の倍数を計算
		dgno *= 10;
	}
	for( i=j=k=0; i<lng; ++i ){
		if( *(xbuf+i) == '\0' ){
			break;
		}
		if( *(xbuf+i) == '.' ){
			k++;
			continue;
		}
		if( !k ){
			hlbf[i] = *(xbuf+i);
		}
		else{
			lobf[j] = *(xbuf+i);
			j++;
		}
	}
	hlno = atoi( hlbf );
	hlno *= dgno;
	lono = atoi( lobf );
	*per1 = hlno + lono;
}

// ----------------------------------------------------------------------------
// 日付をＢＩＮからＢＣＤに変換（BIN側の年は西暦）
// ----------------------------------------------------------------------------
// date_to_bcd( char Type, unsigned char *d_ymd, long s_ymd	)
//	type	0x00 :  YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)(年は西暦)	
//			0x01 :	YYYY.MM.DD(BIN) →　MM.DD(BCD)
//			0x02 :	YYYY.MM.DD(BIN) →　MM(BCD)
//			0x03 :	YYYY.MM.DD(BIN) →　DD(BCD)
//			0x04 :  YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)(年は平成)
//  d_ymd	変換後の日付（ＢＣＤ）
//  s_yms	変換前の日付（ＢＩＮ）
// ----------------------------------------------------------------------------
void CSyzUtil::date_to_bcd( char type, unsigned char *d_ymd, long s_ymd )
{
	char	asci[10]={0}, ansr[10]={0}, year[4]={0};
//	int		yy;

	CVolDateDB	voldate;
	int yymmdd = 0, g = 0;;

	memset( asci, '\0', sizeof(asci) );
	memset( ansr, '\0', sizeof(ansr) );
	if( (s_ymd==0) || (s_ymd==NULL) ){
		return;				// [05'06.23]
	}
	
	switch( type & 0xff ){
		case 0x00 :	// YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)
			sprintf_s( asci, sizeof(asci), _T("%08d"), s_ymd );
			m_Arith.l_pack( ansr, &asci[2], 6 );
			memmove( d_ymd, ansr, 3 );
			break;
		case 0x01 :	// YYYY.MM.DD(BIN) →　MM.DD(BCD)
			sprintf_s( asci, sizeof(asci), _T("%08d"), s_ymd );
			m_Arith.l_pack( ansr, &asci[4], 4 );
			memmove( d_ymd, ansr, 2 );
			break;
		case 0x02 :	// YYYY.MM.DD(BIN) →　MM(BCD)
			sprintf_s( asci, sizeof(asci), _T("%08d"), s_ymd );
			m_Arith.l_pack( ansr, &asci[4], 2 );
			memmove( d_ymd, ansr, 1 );
			break;
		case 0x03 :	// YYYY.MM.DD(BIN) →　DD(BCD)
			sprintf_s( asci, sizeof(asci), _T("%08d"), s_ymd );
			m_Arith.l_pack( ansr, &asci[6], 2 );
			memmove( d_ymd, ansr, 1 );
			break;
		case 0x04 :	// YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)
			//sprintf_s( asci, sizeof(asci), _T("%08d"), s_ymd );
			//// 西暦年を平成年に変換
			//memset( year, '\0', sizeof(year) );
			//memmove( year, &asci[2], 2 );
			//yy = atoi( year );
			//if( yy < 89 ){
			//	yy += 12;	//  西暦２０００年以上
			//}
			//else{
			//	yy -= 88;	//		    〃　　未満
			//}
			//sprintf_s( year, sizeof( year ), _T("%02d"), yy );
			//memmove( &asci[2], year, 2 );
			//m_Arith.l_pack( ansr, &asci[2], 6 );
			//memmove( d_ymd, ansr, 3 );

			voldate.db_datecnvGen( 0, s_ymd, &g, &yymmdd, 0, 0 );
			m_Arith.int_bcd( d_ymd, yymmdd, 3 );

			break;
		default   :
			break;
	}
}

//-----------------------------------------------------------------------------
// 日付をＢＣＤからＢＩＮに変換（BIN側の年は西暦）
// YY.MM.DD(BCD) →　YYYY.MM.DD(BIN)(年は西暦)
// ----------------------------------------------------------------------------
// 引数	sg		：	
//		f_ymd	：	変換後日付（ＢＩＮ）
//		s_ymd	：	変換前日付（ＢＣＤ）
//-----------------------------------------------------------------------------
void CSyzUtil::date_to_bin( int sg, long *d_ymd, unsigned char *s_ymd )
{
	char	year[4]={0}, month[4]={0}, date[4]={0};
	long	yy, mm, dd;

	CVolDateDB	voldate;
	int yymmdd = 0, g = 0, yyyymmdd = 0;

	sprintf_s( year, sizeof(year), _T("%02x"), (*s_ymd&0xff) );
	sprintf_s( month, sizeof(month), _T("%02x"), (*(s_ymd+1)&0x1f) );
	sprintf_s( date, sizeof(date), _T("%02x"), (*(s_ymd+2)&0xff) );
	yy = atoi( year );
	mm = atoi( month );
	dd = atoi( date );
	if( sg == 0 ){
		if( (yy==0) || (mm==0) || (dd==0) ){
			*d_ymd = 0L;
			return;
		}
	}
	else{
		if( (yy==0) && (mm==0) && (dd==0) ){
			*d_ymd = 0L;
			return;
		}
	}

	// 和暦→西暦変換（元号は自動判定）
	yymmdd = m_Arith.bcd_int( s_ymd, 3 );
//	voldate.db_datecnvGen( 4, yymmdd, &g, &yyyymmdd, 1, 0 );
	voldate.db_datecnvGen(0, yymmdd, &g, &yyyymmdd, 1, 0);
	*d_ymd = yyyymmdd;

	//if( yy >= 12 ){
	//	yy = ( yy - 12 ) + 2000;	//  西暦２０００年以上
	//}
	//else{
	//	yy = ( yy + 88 ) + 1900;	//  西暦２０００年未満
	//}
	//yy *= 10000;
	//mm *= 100;
	//*d_ymd = ( yy + mm + dd );
}

//-----------------------------------------------------------------------------
// BCDを文字列に変換
//-----------------------------------------------------------------------------
// 引数	ascbf	：		文字列
//		bcdbf	：		BCD
//		lng		：		文字数
//-----------------------------------------------------------------------------
void CSyzUtil::bcd_to_asci( CString *ascbf, unsigned char *bcdbf, int lng )
{
	char	asci[20]={0};

	ascbf->Empty();
	m_Arith.l_unpac( asci, bcdbf, lng );
	*ascbf = asci;
}

//-----------------------------------------------------------------------------
// 文字列転送（Char　から　CString へ）
//-----------------------------------------------------------------------------
// 引数	d_asci	：	先 文字列
//		s_asci	：	元 文字列
//		lng		：	元 文字列長
//-----------------------------------------------------------------------------
void CSyzUtil::char_to_cstring( CString *d_asci, unsigned char *s_asci, int lng )
{
	char	asci[128]={0};

	ASSERT( d_asci );
	d_asci->Empty();
	if( lng < 128 ){
		memmove( asci, s_asci, lng );
		*d_asci = asci;
	}
	else{
		char	*ptmp = NULL;
		ptmp = new char [lng+1];
		if( ptmp ){
			ASSERT( !_T("char_to_cstring：領域確保エラー") );
			return;
		}
		memset( ptmp, '\0', sizeof(char)*(lng+1) );
		memmove( ptmp, s_asci, lng );
		d_asci->Format( _T("%s"), ptmp );
		delete [] ptmp;
	}
}

//-----------------------------------------------------------------------------
// ダイアグラムデータ 初期化
//-----------------------------------------------------------------------------
// 引数	pDgda	：	対象データ
//-----------------------------------------------------------------------------
void CSyzUtil::DiagOcxIniz( DIAGRAM_DATA *pDgda )
{
	ASSERT( pDgda );

	pDgda->data_disp.Empty();
	pDgda->data_edit.Empty();
	pDgda->data_combo = 0;
	pDgda->data_check = 0;
	memset( pDgda->data_val, '\0', sizeof(pDgda->data_val) );

	pDgda->text_combo.Empty();
	pDgda->text_check.Empty();
	memset( pDgda->data_day, '\0', sizeof(pDgda->data_day) );
	pDgda->data_imgdata = NULL;
}

//-----------------------------------------------------------------------------
// ダイアグラムデータ コピー
//-----------------------------------------------------------------------------
// 引数	xdata	：	元データ
//		idata	：	先データ
//-----------------------------------------------------------------------------
void CSyzUtil::MoveDiagData( DIAGRAM_DATA *xdata, DIAGRAM_DATA *idata )
{
	ASSERT( xdata );
	ASSERT( idata );

	// 初期化
	DiagOcxIniz( xdata );

	xdata->data_disp	=	idata->data_disp;
	xdata->data_edit	=	idata->data_edit;	
	xdata->data_combo	=	idata->data_combo;
	xdata->data_check	=	idata->data_check;
	memmove( xdata->data_val, idata->data_val, 6 );
	memmove( xdata->data_day, idata->data_day, 4 );
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CSyzUtil::LineDataSet( char *data, char *buf, int data_len ) 
{
	CStringArray kpspn;

	int lin = data_len / 2;
	
	for(int i=0; i<lin ;i++ ){
		kpspn.Add( MKString( &data[i * 2], 2 ));
	}
	
	CString data_edit;
	
	int max = (int)kpspn.GetCount();
	for( int i=0;i<max; i++ ){
		if( kpspn[i] == CString("　")){
			data_edit += "\r\n";
			continue;
		}else{
			data_edit += kpspn[i];
		}

		int mojicnt = 0;
		for( int j=i+1; (j<max)&&(!mojicnt); j++ ){
			mojicnt += kpspn[j].GetLength();
		}
	
		if( mojicnt ){
			data_edit += "\r\n";
		}
	}
	memmove( buf,data_edit,data_edit.GetLength() );
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CSyzUtil::LineDataSet( char *data, char *buf, int len, int lin ) 
{
	char	bf1[512], bf2[512], bf3[512];
	int		i, j, id, lng;

	::ZeroMemory( bf1, sizeof( bf1 ));
	sprintf_s( bf1, sizeof( bf1 ), "%s", buf );

	for( i = id = 0; i < lin; i++ ){
		j = 0;
		::ZeroMemory( bf2, sizeof( bf2 ));
		while(1){
			if( !bf1[j] )	break;
			if((bf1[j] == 0x0d)&&(bf1[j+1] == 0x0a))	// 改行マーク
				break;
			bf2[j] = bf1[j];
			j++;
		}
		lng = (int)strlen( bf2 );

		::ZeroMemory( bf3, sizeof( bf3 ));
		memmove( bf3, bf1, sizeof( bf3 ));
		::ZeroMemory( bf1, sizeof( bf1 ));

		if( bf3[lng] )	sprintf_s( bf1, sizeof( bf1 ), "%s", &bf3[lng+2] );

		memmove( &data[id], bf2, lng);
		id += len;
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 引数	data		：	元データ
//		buf			：	先データ
//		data_len	：	元データ 文字列長
//-----------------------------------------------------------------------------
void CSyzUtil::LineDataGet( char *data, char *buf, int data_len ) 
{
	CString data_str = MKString( data, data_len );

	CStringArray StrArray;
	SplitCmntToArray( StrArray, data_str );

	CString edit;
	for( int i=0; i<StrArray.GetSize(); i++ ){
		edit += StrArray[i];
		if( !StrArray[i].GetLength() ){
			edit += "　";
		}
	}

	wsprintf( buf, _T("%s"), edit.GetBuffer() );
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 引数	str		：	元データ
//		data	：	先データ
//		len		：	元データ 文字列長
//		lin		：
//-----------------------------------------------------------------------------
void CSyzUtil::LineDataGet( CString &str, char *data, int len, int lin )
{
	str.Empty();

	int		i, j, id, id2, lng;
	char	buf[256];

	for( i = id = 0; i < lin; i++ ){
		::ZeroMemory( buf, sizeof( buf ));
		memcpy( buf, &data[id], len );
		lng = (int)strlen(buf);
		if( lng )	str += buf;
		if( i+1 < lin ){
			for( j = i+1, id2 = id+len; j < lin; j++ ){
				::ZeroMemory( buf, sizeof( buf ));
				memcpy( buf, &data[id2], len );
				lng = (int)strlen(buf);
				if( lng ){
					str += "\r\n";	break;
				}
				id2 += len;
			}
		}
		id += len;
	}
}

//-----------------------------------------------------------------------------
// 半角文字カット
//-----------------------------------------------------------------------------
// 引数	check	：	チェック対象
//		size	：	文字列サイズ
//-----------------------------------------------------------------------------
// 返送値		：	カット後文字列
//-----------------------------------------------------------------------------
CString CSyzUtil::CutHankaku( LPCSTR check,int size )
//CString CSyzUtil::HankakuCut(LPCSTR check,int size)
{
	char buf[256]={};
	memmove( buf, check, size );

	CString ret = buf;
	for(int i = 0;i < ret.GetLength();i++){
		if( _ismbslead( (const unsigned char *)(LPCSTR)ret, (const unsigned char *)((LPCSTR)ret)+i) )
			i++;
		else{
			ret.Delete(i);	
			i--;
		}
	}

	return ret;
}

//-----------------------------------------------------------------------------
// 何バイト分の文字列？
//-----------------------------------------------------------------------------
// 引数	asci	：
//		max		：
//-----------------------------------------------------------------------------
// 返送値		：	バイト数
//-----------------------------------------------------------------------------
int CSyzUtil::GetByteStrLen( unsigned char *asci, int max )
//int CSyzUtil::numlen( unsigned char *asci, int max )
{
	char	*pBuf = NULL;
	pBuf = new char [max+1];
	if( pBuf == NULL ){
		ASSERT( !_T("GetByteStrLen_ERROR") );
		return 0;
	}
	memset( pBuf, '\0', sizeof(char)*(max+1) );
	memmove( pBuf, asci, max );

	int i=0;
	for( i=0; i<max; i++ ){
		if( !(pBuf[i]&0xff) ){
			break;
		}
	}

	if( pBuf ){
		delete [] pBuf;
		pBuf = NULL;
	}

	return i;
}

//-----------------------------------------------------------------------------
// 科目名称の取得
//-----------------------------------------------------------------------------
// 引数	pZmSub	：	公益クラス
//		Code	：	科目コード
//		Name	：	科目名称
//-----------------------------------------------------------------------------
// 返送値	1	：	データあり
//			0	：	データ無し
//-----------------------------------------------------------------------------
int CSyzUtil::GetKnrecName( CDBNpSub *pZmSub, char *Code, char *Name )
{
	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return 0;
	}
	ASSERT( pZmSub->knrec );
	if( pZmSub->knrec == NULL ){
		return 0;
	}

	int		rmx, rno, ans;
	char	codbf[10];

	rmx = pZmSub->knrec->GetRecordCount();
	ans = 0;
	for( rno = 0; rno < rmx ; rno++ ) {
		pZmSub->knrec->SetAbsolutePosition( rno+1 );
		// 大文字変換
		memset( codbf, '\0', sizeof( codbf ) );
		memmove( codbf, pZmSub->knrec->kncod, 8 );
		_strupr_s( codbf, sizeof( codbf ) );
		if( strncmp( codbf, Code, 8 ) )	continue;
		memmove( Name, pZmSub->knrec->knnam, 20 );
		ans = 1;
		break;
	}
	return ans;
}

//-----------------------------------------------------------------------------
// 文字列をBCDに変換
//-----------------------------------------------------------------------------
// 引数	buff	：	変換先
//		data	：	変換元
//		len		：	変換文字列長
//-----------------------------------------------------------------------------
void CSyzUtil::AscToBcd( char* buff, char* data, int len )
{
	CString	str;
	str.Format( _T("%s"), data );
	str.MakeUpper();

	// 文字数チェック
	int slen = str.GetLength();
	if( slen < len ){
		return;
	}

	int	bcdlen = len / 2;
	int	hlsw = 0;
	if( len % 2 ){
		hlsw = 1;
		bcdlen++;
	}

	BYTE	hc, lc;

	for( int i=0, j=0; i<bcdlen; i++ ){
		if( hlsw ){
			hc = 0;
			hlsw = 1;
		}
		else{
			hc = str.GetAt( j++ );
		}

		if( (hc>='A') && (hc<='Z') ){
			hc = hc - 'A' + 0x0a;
		}
		else{
			hc = hc - '0';
		}
		hc = ((hc&0x0f) << 4) & 0xf0;

		lc = str.GetAt( j++ );

		if( (lc>='A') && (lc<='Z') ){
			lc = lc - 'A' + 0x0a;
		}
		else{
			lc = lc - '0';
		}

		*(buff + i) = hc + lc;
	}
}

//-----------------------------------------------------------------------------
// 電話番号変換
//-----------------------------------------------------------------------------
// 引数	buf		：	
//		telno	：	
//		len		：	
//		lng		：	
//-----------------------------------------------------------------------------
void CSyzUtil::CnvTelno( char *buf, char *telno, int len, int lng )
{
	char	*fp, tel[128]={0}, ttel[4][32]={0};
	char	bf1[32]={0}, bf2[32]={0}, bf3[32]={0};

	memset( ttel, '\0', 32*4 );
	memmove( tel, telno, lng );

	int cnt = 2;
	if( fp = (char *) strrchr( tel, '-' ) ){
		strcpy_s( ttel[cnt--], 32, (fp+1) );		// 個人番号
		*fp = '\0';
		if( fp = (char *) strrchr( tel, '-' ) ){
			strcpy_s( ttel[cnt--], 32, (fp+1) );	// 市内局番
			*fp = '\0';
			strcpy_s( ttel[cnt], 32, tel );			// 市外局番
		}
		else{
			strcpy_s( ttel[cnt], 32, tel );			// 市内局番
		}

		while( 1 ){
			switch( cnt ){
				case 0:	// 市外局番
					memcpy( bf1, ttel[cnt], 32 );
					++cnt;
					continue;
				case 1:	// 市内局番
					memcpy( bf2, ttel[cnt], 32 );
					++cnt;	
					continue;
				case 2:	// 個人番号
					memcpy( bf3, ttel[cnt], 32 );
					++cnt;
					continue;
				default:
					break;
			}
			break;
		}

		if( (kjlen(bf1, 32)>6)|| (kjlen(bf2, 32)>4)||(kjlen(bf3, 32)>4) ){
			wsprintf( buf, "%s － %s － %s", bf1, bf2, bf3 );
			if( kjlen( buf, 32 ) > 26 ){
				buf[26] = 0;
			}
		}
		else{
			wsprintf( buf, "%6s  －  %4s  － %4s", bf1, bf2, bf3 );
		}
	}
	else{
		if( kjlen( telno, 30 ) == 0 ){
			wsprintf( buf, "        －        －" );
		}
		else{
			memcpy( buf, telno, 26 );
		}
	}
}

//-----------------------------------------------------------------------------
// 文字列の後ろの半角SPカット
//-----------------------------------------------------------------------------
// 引数	buf		：	対象バッファ
//		length	：	文字列長
//-----------------------------------------------------------------------------
void CSyzUtil::AnkSpaceCut( char *buf, int length )
{
	for( int i=(length-1); i>=0; i-- ){
		if( *(buf+i) == 0x00 ){
			continue;
		}
		if( *(buf+i) != 0x20 ){
			break;
		}
		*(buf+i) = 0x00;
	}
}

//-----------------------------------------------------------------------------
// 漢字2文字を（ひらがな・カタカナ）をカタカナに変換
//-----------------------------------------------------------------------------
// 引数	Knji	：	
//		Kana	：	
//		Moji	：	
//-----------------------------------------------------------------------------
void CSyzUtil::LetterAlter( char *Knji, char *Kana, int Moji )
{
	int		get, put, cnt, max, mov;
	char	buf0[128], buf1[4];

	memset( buf0, '\0', sizeof(buf0) );
	for( get=0, put=0, max=0; 1 ; get++ ){
		if( *(Knji+(get*2)) == 0x00 ){
			break;
		}
		memset( buf1, '\0', sizeof( buf1 ) );
		memmove( buf1, (Knji+(get*2)), 2 );
		max++;
		if( max > Moji ){
			break;
		}
		for( cnt = 0; 1; cnt++ ){
			if( ChTbl[cnt]._kana == 0x00 ){
				break;
			}
			mov = 0;
			if( strncmp(buf1, ChTbl[cnt]._knjn, 2) == 0 ){
				switch( ChTbl[cnt]._sign ){
					case 1 : 
						buf0[put++] = ChTbl[cnt]._kana;
						buf0[put++] = 'ﾞ';
						mov = 1;
						break;
					case 2 :
						buf0[put++] = ChTbl[cnt]._kana;
						buf0[put++] = 'ﾟ';
						mov = 1;
						break;
					default:
						buf0[put++] = ChTbl[cnt]._kana;
						mov = 1;
						break;
				}
			}
			if( mov ){
				break;
			}
		}
	}
	memmove( Kana, buf0, Moji );
}

//-----------------------------------------------------------------------------
// 空文字セット
//-----------------------------------------------------------------------------
// 引数	data	：
//		buf		：
//		len		：
//-----------------------------------------------------------------------------
void CSyzUtil::LineSpaceSet( char *data, char *buf, int len ) 
{
	::ZeroMemory( buf, len );
	memmove( buf, data, len );
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CSyzUtil::LineStrSet( char *data, char *st1, char *st2, int len, int ln2 ) 
{
	char	dt1[128], dt2[128], dt3[128];
	int		kl;

	int	dBufSize = (int)strlen(data) + 1;	// 受取側のバッファサイズは、初期文字列と同サイズはあると考える

	if( ln2+1 > (int)strlen( data )){	// 1行だけの場合
		sprintf_s( st1, ln2+1, "%s", data );	return;
	}

	::ZeroMemory( dt1, sizeof( dt1 ));
	::ZeroMemory( dt2, sizeof( dt2 ));
	::ZeroMemory( dt3, sizeof( dt3 ));
	sprintf_s( dt1, sizeof( dt1 ), "%s", data );

	kl = IsKindOfLetter( dt1, len-1 );
	if( kl == 2 ){		//	全角文字1バイト目
		memcpy( dt2, dt1, len+1 );
		::ZeroMemory( dt3, sizeof( dt3 ));
		sprintf_s( dt3, sizeof( dt3 ), "%s", &dt1[len+1] );
	}
	else{
		memcpy( dt2, dt1, len );
		::ZeroMemory( dt3, sizeof( dt3 ));
		sprintf_s( dt3, sizeof( dt3 ), "%s", &dt1[len] );
	}
	memcpy( dt1, dt3, sizeof( dt3 ));
	sprintf_s( st1, dBufSize, "%s", dt2 );

	int	length = (int)strlen( dt1 );
	if( length <= len )	{
		memcpy( st2, dt1, length );
		return;
	}

	kl = IsKindOfLetter( dt1, len-1 );
	if( kl == 2 ){		//	全角文字1バイト目
		memcpy( dt2, dt1, len+1 );
		::ZeroMemory( dt3, sizeof( dt3 ));
		sprintf_s( dt3, sizeof( dt3 ), "%s", &dt1[len+1] );
	}
	else{
		memcpy( dt2, dt1, len );
		::ZeroMemory( dt3, sizeof( dt3 ));
		sprintf_s( dt3, sizeof( dt3 ), "%s", &dt1[len] );
	}
	memcpy( dt1, dt3, sizeof( dt3 ));
	sprintf_s( st2, dBufSize, "%s", dt2 );
}

//-----------------------------------------------------------------------------
// カナを漢字に変換 [maxはカナ文字数]
//-----------------------------------------------------------------------------
void CSyzUtil::AtoJis( char *buf, char *ank, int max )
{
	int		i, j, k;
	int FindFlg = 0;//文字がなければ

	for( i = k = 0; i <= max; ++i ) {
		for( j = 0; atojtb[j].asc1 != 0; ++j ) {
			if( ! strncmp( atojtb[j].asc1, &ank[i], 1 ) ) {
				FindFlg = 1;
				if( atojtb[j].asc2 != 0 && i ) {
					for( ;; ++j ) {
						if( strncmp( atojtb[j].asc1, &ank[i], 1 ) )	break;
						if( ! strncmp( atojtb[j].asc2, &ank[i-1], 1 ) ) {
							if( k )	k -= 2;
							memmove( (buf+k), atojtb[j].asc3, 2 );
							k += 2;	break;
						}
					}
				}
				else {
					memmove( (buf+k), atojtb[j].asc3, 2 );
					k += 2;
				}
				break;
			}
		}
		if( !FindFlg ){
			break;
		}
		FindFlg = 0;
	}
	for( ; k < (max*2); ++k )	*(buf+k) = '\0';
}


//-----------------------------------------------------------------------------
// 文字列比較
//-----------------------------------------------------------------------------
int CSyzUtil::StringCompare( BYTE *text1, BYTE *text2, int length )
{
	char	buf1[128], buf2[128];
	int		status = 0;
	memset( buf1, '\0', sizeof( buf1 ) );
	memmove( buf1, text1, length );
	memset( buf2, '\0', sizeof( buf2 ) );
	memmove( buf2, text2, length );
	return( CompareStringSJIS( buf1, buf2 ) );
}

//-----------------------------------------------------------------------------
// 全てのAsciiのスペースをカット
//-----------------------------------------------------------------------------
void CSyzUtil::AllAnkSpaceCut( char *buf1, char *buf2, int length )
{
	int i, j;
	for( i = 0, j = 0; i != length; i++ ) {
		if( *(buf2+i) == 0x20 )	continue;
		*(buf1+j) = *(buf2+i);
		j++;
	}
}

//----------------------------------------------------------------------------
// 実数変換後の割り算
//----------------------------------------------------------------------------
// 引数	dest	：	出力用バッファ
//		src		：	被乗算数
//		nume	：	乗算数( 分子  
//		deno	：			分母  ) 
//----------------------------------------------------------------------------
void CSyzUtil::l_pardotnet_syz( char *dest, const char *src, const char *nume, const char *deno )
{
	l_pardotnet( dest, src, nume, deno );
}

//---->'20.07.27
//----------------------------------------------------------------------------
// 実数変換後の割り算（モード追加版作成。特定収入割合に使用）
//----------------------------------------------------------------------------
// 引数	dest	：	出力用バッファ
//		src		：	被乗算数
//		nume	：	乗算数( 分子  
//		deno	：			分母  ) 
//		mode	：　０ 切り捨て　１　切り上げ　２　四捨五入
//----------------------------------------------------------------------------
void CSyzUtil::l_pardotnet_syz2(char *dest, const char *src, const char *nume, const char *deno , char mode)
{
	l_pardotnet2(dest , src , nume , deno , mode);
}
//<-----


#include <math.h>
//----------------------------------------------------------------------------
// 実数変換後の割り算
//----------------------------------------------------------------------------
// 引数	dest	：	出力用バッファ
//		src		：	被乗算数
//		nume	：	乗算数( 分子  
//		deno	：			分母  ) 
//----------------------------------------------------------------------------
void CSyzUtil::l_parwari_syz( char *dest, char *src, char *nume, char *deno )
{
	char	bfSrc[64]={0}, bfNume[64]={0}, bfDeno[64]={0};
	m_Arith.l_print( bfSrc, src, _T("sssssssssssss9") );
	m_Arith.l_print( bfNume, nume, _T("sssssssssssss9") );
	m_Arith.l_print( bfDeno, deno, _T("sssssssssssss9") );

	double	dbSrc, dbNume, dbDeno;
	dbSrc = strtod( bfSrc, 0 );
	dbNume = strtod( bfNume, 0 );
	dbDeno = strtod( bfDeno, 0 );

	double	ans;
	if( dbDeno ){
//-- '14.09.18 --
//		ans = dbSrc * dbNume / dbDeno;
//---------------
		ans = dbSrc * (dbNume / dbDeno);
//---------------
	}
	else{
		ans = 0.0;
	}
	if( ans > 0 ){
		ans = floor( ans );
	}
	else{
		ans = ceil( ans );
	}

	char	bfAns[64]={0};
	sprintf_s( bfAns, sizeof(bfAns), _T("%.0f"), ans );
	m_Arith.l_input( dest, bfAns );
}

//-----------------------------------------------------------------------------
// 特定収入計算表マスター？	('15.02.06)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	非営利クラス
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	特定収入計算表マスター
//			FALSE	：	特定収入計算表マスターでない
//-----------------------------------------------------------------------------
BOOL CSyzUtil::IsSpcMaster( CDBNpSub *pDBNpSub )
{
	BOOL	bRt = FALSE;

	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return bRt;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return bRt;
	}

	if( (pDBNpSub->zvol->apno&0xf0) == 0x50 ){
		EnumIdIcsShKazeihoushiki kztype = GetKazeihoushiki(pDBNpSub->zvol);
		if( kztype == ID_ICSSH_GENSOKU ){
			bRt = TRUE;
		}
	}
	
	return bRt;
}

//-----------------------------------------------------------------------------
// どの改正様式？('15.08.27)
//-----------------------------------------------------------------------------
// 引数	pZvol	：	ボリュームラベル
//		symd	：	開始年月日
//		eymd	：	終了年月日
//		shkubun	：	申告区分
//-----------------------------------------------------------------------------
// 返送値		：	改正様式
//-----------------------------------------------------------------------------
EnumIdIcsShRevType CSyzUtil::GetShRevType( CZVOLUME *pZvol, long symd, long eymd, EnumIdIcsShShinkokuKubun shkubun/*=ID_ICSSH_KAKUTEI*/ )
{
	EnumIdIcsShRevType	rtRevtype = ID_ICSSH_NO_REVTYPE;

	ASSERT( pZvol );
	if( pZvol == NULL ){
		return rtRevtype;
	}

	// 8%改正
	if( eymd < ICS_SH_8PER_REFORM_DAY ){
		return rtRevtype;
	}
	else{
		rtRevtype = ID_ICSSH_8PER_REVTYPE;
	}

	// 簡易課税第六種
	EnumIdIcsShKazeihoushiki kzhousiki = GetKazeihoushiki( pZvol );
	if( kzhousiki == ID_ICSSH_KANNI ){
		if( symd >= ICS_SH_SIXKIND_REFORM_DAY ){
			rtRevtype = ID_ICSSH_SIXKIND_REVTYPE;
		}
	}

	// 特定課税仕入
	if( kzhousiki == ID_ICSSH_GENSOKU ){
		if( eymd >= ICS_SH_TOKUTEI_REFORM_DAY ){
			rtRevtype = ID_ICSSH_TKKZSIIRE_REVTYPE;
		}
	}

	// マイナンバー
	if( symd >= ICS_SH_MYNUMBER_REFORM_DAY ){
		rtRevtype = ID_ICSSH_MYNUMBER_REVTYPE;
	}

	// 10%改正
	if( eymd >= ICS_SH_10PER_REFORM_DAY ){
		rtRevtype = ID_ICSSH_10PER_REVTYPE;
	}

	// 消費税率10% 旧税率が適用された取引がない場合
	if ( kzhousiki == ID_ICSSH_GENSOKU ) {
		// 居住用賃貸建物を課税賃貸用に供した
		if ( eymd >= ICS_SH_KYOJU_REFORM_DAY ) {
			rtRevtype = ID_ICSSH_KYOJU_REVTYPR;
		}
	}

	return rtRevtype;
}

//-----------------------------------------------------------------------------
// どの改正様式？('17.12.26)
//-----------------------------------------------------------------------------
// 引数	pZvol	：	ボリュームラベル
//		ksymd	：	課税期間 開始年月日
//		keymd	：	課税期間 終了年月日
//		msymd	：	中間申告 開始年月日
//		meymd	：	中間申告 終了年月日
//		shkubun	：	申告区分
//-----------------------------------------------------------------------------
// 返送値		：	改正様式
//-----------------------------------------------------------------------------
EnumIdIcsShRevType CSyzUtil::GetShRevType( CZVOLUME *pZvol, long ksymd, long keymd, long msymd, long meymd, EnumIdIcsShShinkokuKubun shkubun/*=ID_ICSSH_KAKUTEI*/ )
{
	EnumIdIcsShRevType	rtRevtype = ID_ICSSH_NO_REVTYPE;

	ASSERT( pZvol );
	if( pZvol == NULL ){
		return rtRevtype;
	}

	// 判定用月日の指定
	long	chkSymd=0, chkEymd=0;

	// 準確定の判定がなかったため追加
	//if( (shkubun==ID_ICSSH_KAKUTEI) || (shkubun==ID_ICSSH_KAKUTEI_SYUUSEI) || (shkubun==ID_ICSSH_KOUSEI) ){
	if( (shkubun==ID_ICSSH_KAKUTEI) || (shkubun==ID_ICSSH_KAKUTEI_SYUUSEI) || (shkubun==ID_ICSSH_KOUSEI) || (shkubun==ID_ICSSH_JUNKAKUTEI) ){
		chkSymd = ksymd;
		chkEymd = keymd;
	}
	else if( (shkubun==ID_ICSSH_TYUUKAN) || (shkubun==ID_ICSSH_TYUUKAN_SYUUSEI) || (shkubun==ID_ICSSH_YOTEI) ){
		chkSymd = msymd;
		chkEymd = meymd;
	}

	EnumIdIcsShKazeihoushiki kzhousiki = GetKazeihoushiki( pZvol );

	// 10%改正
	if( chkEymd >= ICS_SH_10PER_REFORM_DAY ){
		rtRevtype = ID_ICSSH_10PER_REVTYPE;

		// 居住用賃貸建物を課税賃貸用に供した
		if ( kzhousiki == ID_ICSSH_GENSOKU ) {
			if ( chkEymd >= ICS_SH_KYOJU_REFORM_DAY ) {
				rtRevtype = ID_ICSSH_KYOJU_REVTYPR;
			}
		}
		return rtRevtype;
	}

	// 8%改正
	if( chkEymd < ICS_SH_8PER_REFORM_DAY ){
		return rtRevtype;
	}
	else{
		rtRevtype = ID_ICSSH_8PER_REVTYPE;
	}

	// 簡易課税第六種
	//EnumIdIcsShKazeihoushiki kzhousiki = GetKazeihoushiki( pZvol );
	if( kzhousiki == ID_ICSSH_KANNI ){
		if( chkSymd >= ICS_SH_SIXKIND_REFORM_DAY ){
			rtRevtype = ID_ICSSH_SIXKIND_REVTYPE;
		}
	}

	// 特定課税仕入
	if( kzhousiki == ID_ICSSH_GENSOKU ){
		if( chkEymd >= ICS_SH_TOKUTEI_REFORM_DAY ){
			rtRevtype = ID_ICSSH_TKKZSIIRE_REVTYPE;
		}
	}

	// 判定用月日の指定（マイナンバーの期間判定は、全体の課税期間によって判定する）
	chkSymd = ksymd;
	chkEymd = keymd;

	// マイナンバー
	if( chkSymd >= ICS_SH_MYNUMBER_REFORM_DAY ){
		rtRevtype = ID_ICSSH_MYNUMBER_REVTYPE;
	}

	return rtRevtype;
}

//2018.03.13 INSERT START

//明治　18680125～19120729
//大正　19120730～19261224
//昭和　19261225～19890107
//平成	19890108～

//const	int Meiji_start  = 18680125;
const	int Meiji_start  = 18680101; //旧暦は無視
const	int Taisyo_start = 19120730;
const	int Syowa_start  = 19261225;
const	int Heisei_start = 19890108;
const	int Reiwa_start  = 20190501;
/////////////////////////////////////////////////////////////////
//処理概要：西暦→和暦変換(明治以降)
//
//引数　　：Nengo…変換後年号 0x00=明治以前or未入力　0x01=明治　0x02=大正　0x03=昭和　0x04=平成 0x05=令和
//			Year …変換後年
//　　　　　Month…変換後月
//　　　　　Day  …変換後日
//　　　　　AD   …変換前西暦(例 2018年6月11日→20180611)
//
//返送値　： 0=正常終了
//　　　　　-1=エラー or 選択なし
/////////////////////////////////////////////////////////////////
int CSyzUtil::ADtoJC( char *Nengo, char *Year, char *Month, char *Day, int AD )
{
	*Nengo = 0x00;
	*Year  = 0x00;
	*Month = 0x00;
	*Day   = 0x00;

	if( AD == 0 ){
		return -1;
	}

	//年号　1…明治 2…大正 3…昭和 4…平成 5…令和
	int start = 0;
	if( AD >= Reiwa_start ){
		*Nengo = 0x05;
		start = Reiwa_start;
	}else{
	if( AD >= Heisei_start ){
		*Nengo = 0x04;
		start = Heisei_start;
	}else
	if( AD >= Syowa_start ){
		*Nengo = 0x03;
		start = Syowa_start;
	}else
	if( AD >= Taisyo_start ){
		*Nengo = 0x02;
		start = Taisyo_start;
	}else
	if( AD >= Meiji_start ){
		*Nengo = 0x01;
		start = Meiji_start;
	}else
		return -1;
	}

	int year = (AD/10000)-(start/10000)+1;
	data_day_conv_DecToHex( Year,  year );

	int month = (AD/100)%100;
	data_day_conv_DecToHex( Month, month );

	int day = AD%100;
	data_day_conv_DecToHex( Day, day );
	
	return 0;
}

/////////////////////////////////////////////////////////////////
//処理概要：和暦→西暦変換(明治以降)
//
//引数　　：AD   …変換後西暦(例 2018年6月11日→20180611)
//　　　　　Nengo…変換前年号 0x00=明治以前or未入力　0x01=明治　0x02=大正　0x03=昭和　0x04=平成 0x05=令和
//			Year …変換前年
//　　　　　Month…変換前月
//　　　　　Day  …変換前日
//　　　　　
//返送値　： 0=正常終了
//　　　　　-1=エラー or 選択なし
/////////////////////////////////////////////////////////////////
int CSyzUtil::JCtoAD(  int *AD, char Nengo, char Year, char Month, char Day )
{
	*AD = 0;

	if(( Year == 0x00 )||( Month == 0x00 )||( Day == 0x00 )){
		return -1;
	}

	int tmp = 0;
	int year = 0;
	data_day_conv_HexToDec( &tmp, Year );
	if( Nengo == 0x00 ){
		return -1;
	}else
	if( Nengo == 0x01 ){
		year = Meiji_start/10000 + tmp -1;
	}else
	if( Nengo == 0x02 ){
		year = Taisyo_start/10000 + tmp -1;
	}else
	if( Nengo == 0x03 ){
		year = Syowa_start/10000 + tmp -1;
	}else
	if( Nengo == 0x04 ){
		year = Heisei_start/10000 + tmp -1;
	}else
	if( Nengo == 0x05 ){
		year = Reiwa_start/10000 + tmp -1;
	}

	int month = 0;
	data_day_conv_HexToDec( &month, Month );

	int day = 0;
	data_day_conv_HexToDec( &day, Day );

	*AD = year*10000 + month*100 + day;

	return 0;
}

/////////////////////////////////////////////////////////////////
//処理概要：日付入力変換
//　　　　　data_dayの入力が、11月30日→0x11,0x30とデータ作成されるためその変換を行う
//
//
/////////////////////////////////////////////////////////////////
int CSyzUtil::data_day_conv_DecToHex( char *hex, int dec )
{

	*hex = 0x00;

	switch( dec%10 ){
		case 0:
			break;
		case 1:
			*hex = 0x01;
			break;
		case 2:
			*hex = 0x02;
			break;
		case 3:
			*hex = 0x03;
			break;
		case 4:
			*hex = 0x04;
			break;
		case 5:
			*hex = 0x05;
			break;
		case 6:
			*hex = 0x06;
			break;
		case 7:
			*hex = 0x07;
			break;
		case 8:
			*hex = 0x08;
			break;
		case 9:
			*hex = 0x09;
			break;
	}

	switch( dec/10 ){
		case 0:
			break;
		case 1:
			*hex |= 0x10;
			break;
		case 2:
			*hex |= 0x20;
			break;
		case 3:
			*hex |= 0x30;
			break;
		case 4:
			*hex |= 0x40;
			break;
		case 5:
			*hex |= 0x50;
			break;
		case 6:
			*hex |= 0x60;
			break;
		case 7:
			*hex |= 0x70;
			break;
		case 8:
			*hex |= 0x80;
			break;
		case 9:
			*hex |= 0x90;
			break;
		default:
			break;
	}

	return 0;
}
// 日付入力変換
int CSyzUtil::data_day_conv_HexToDec( int *dec, char hex )
{
	*dec = 0;

	if( (hex&0x0f) == 0x01 ){
		*dec = 1;
	}else
	if( (hex&0x0f) == 0x02 ){
		*dec = 2;
	}else
	if( (hex&0x0f) == 0x03 ){
		*dec = 3;
	}else
	if( (hex&0x0f) == 0x04 ){
		*dec = 4;
	}else
	if( (hex&0x0f) == 0x05 ){
		*dec = 5;
	}else
	if( (hex&0x0f) == 0x06 ){
		*dec = 6;
	}else
	if( (hex&0x0f) == 0x07 ){
		*dec = 7;
	}else
	if( (hex&0x0f) == 0x08 ){
		*dec = 8;
	}else
	if( (hex&0x0f) == 0x09 ){
		*dec = 9;
	}

	if( (hex&0xf0) == 0x10 ){
		*dec += 10;
	}else
	if( (hex&0xf0) == 0x20 ){
		*dec += 20;
	}else
	if( (hex&0xf0) == 0x30 ){
		*dec += 30;
	}else
	if( (hex&0xf0) == 0x40 ){
		*dec += 40;
	}else
	if( (hex&0xf0) == 0x50 ){
		*dec += 50;
	}else
	if( (hex&0xf0) == 0x60 ){
		*dec += 60;
	}else
	if( (hex&0xf0) == 0x70 ){
		*dec += 70;
	}else
	if( (hex&0xf0) == 0x80 ){
		*dec += 80;
	}else
	if( (hex&0xf0) == 0x90 ){
		*dec += 90;
	}

	return 0;
}
//2018.03.13 INSERT END

// ----------------------------------------------------------------------------
// 日付をＢＩＮからＢＣＤに変換（BIN側の年は西暦）
// ----------------------------------------------------------------------------
// date_to_bcd( char Type, unsigned char *d_ymd, long s_ymd	)
//	type	0x00 :  YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)(年は西暦)	
//			0x01 :	YYYY.MM.DD(BIN) →　MM.DD(BCD)
//			0x02 :	YYYY.MM.DD(BIN) →　MM(BCD)
//			0x03 :	YYYY.MM.DD(BIN) →　DD(BCD)
//			0x04 :  YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)(年は平成)
//  d_gengo	変換後の元号
//  d_ymd	変換後の日付（ＢＣＤ）
//  s_yms	変換前の日付（ＢＩＮ）
// ----------------------------------------------------------------------------
void CSyzUtil::date_to_bcdGen(char type, int *d_gengo, unsigned char *d_ymd, long s_ymd)
{
	char	asci[10]={ 0 }, ansr[10]={ 0 }, year[4]={ 0 };
	//	int		yy;

	CVolDateDB	voldate;
	int yymmdd = 0, g = 0;;

	memset(asci, '\0', sizeof(asci));
	memset(ansr, '\0', sizeof(ansr));
	if( (s_ymd==0) || (s_ymd==NULL) ){
		*d_gengo = 0;
		return;				// [05'06.23]
	}

	switch( type & 0xff ){
	case 0x00:	// YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)
		sprintf_s(asci, sizeof(asci), _T("%08d"), s_ymd);
		m_Arith.l_pack(ansr, &asci[2], 6);
		memmove(d_ymd, ansr, 3);
		break;
	case 0x01:	// YYYY.MM.DD(BIN) →　MM.DD(BCD)
		sprintf_s(asci, sizeof(asci), _T("%08d"), s_ymd);
		m_Arith.l_pack(ansr, &asci[4], 4);
		memmove(d_ymd, ansr, 2);
		break;
	case 0x02:	// YYYY.MM.DD(BIN) →　MM(BCD)
		sprintf_s(asci, sizeof(asci), _T("%08d"), s_ymd);
		m_Arith.l_pack(ansr, &asci[4], 2);
		memmove(d_ymd, ansr, 1);
		break;
	case 0x03:	// YYYY.MM.DD(BIN) →　DD(BCD)
		sprintf_s(asci, sizeof(asci), _T("%08d"), s_ymd);
		m_Arith.l_pack(ansr, &asci[6], 2);
		memmove(d_ymd, ansr, 1);
		break;
	case 0x04:	// YYYY.MM.DD(BIN) →　YY.MM.DD(BCD) 西暦→和暦
		voldate.db_datecnvGen(0, s_ymd, &g, &yymmdd, 0, 0);
		m_Arith.int_bcd(d_ymd, yymmdd, 3);
		*d_gengo = g;

		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 平成継続出力？
//  令和の元号になっている年月日であっても、平成の元号として出力する事
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		IsJudgeKjn	：	個人事業かどうかを判断基準とするかどうか
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	通常出力
//-----------------------------------------------------------------------------
bool CSyzUtil::IsJcContinuePrint( CSnHeadData *pSnHeadData, bool IsJudgeKjn/*=true*/ )
{
	bool bRt = false;

	// 平成継続出力よりも、令和タイプの出力が優先
	if( GetPrePrintGengo(pSnHeadData, IsJudgeKjn) == 2 ){
		return bRt;
	}

	char	dev[32]={ 0 };
	if( GCOM_GetString(_T("SystemDrive"), dev) == -1 ){
	}
	CString	SysPath;
	SysPath.Format(_T("%s:\\ICSWIN\\SYS"), dev);

	CString	FilePath;
	FilePath.Format(_T("%s\\JCCONTINUE.txt"), SysPath);

	CFileFind	find;
	if( find.FindFile(FilePath) ){
		bRt = true;
	}
	find.Close();

	return bRt;
}

//-----------------------------------------------------------------------------
// 元号無し和暦データを変換ルールに則って、元号付き和暦データに変換
//-----------------------------------------------------------------------------
void CSyzUtil::CnvNoGengoToGengoWareki(UCHAR *pSday, int *pDGengo, UCHAR *pDday)
{
	if( (pSday[0]==0x00) && (pSday[1]==0x00) && (pSday[2]==0x00) ){
		*pDGengo = 0;
		memset(pDday, '\0', sizeof(UCHAR)*4);
		return;
	}
	else{
		UCHAR	chkYmd[4]={ 0 };
		chkYmd[0] = 0x31;
		chkYmd[1] = 0x05;
		if( IsValidKaigen() ){
			if( memcmp(pSday, chkYmd, sizeof(UCHAR)*2) >= 0 ){
				// 平成31年5月以降が入力された時→令和1年5月が入力されたと自動変換
				pSday[0] -= 0x30;
			}
		}
	}
	UCHAR	svYmd[4]={ 0 };
	memmove(svYmd, pSday, sizeof(svYmd));

	// 和暦→西暦変換(元号自動変換)
	// 西暦→和暦変換(元号付きデータ）
	CVolDateDB	voldate;
	int		tmpGengo=0;
	UCHAR	SeYmd[4]={ 0 }, WaYmd[4]={ 0 };
	int		st = 0;
	if( pSday[1] ){
		if( pSday[2] ){
			st = voldate.db_datecnvGen(0, pSday, &tmpGengo, SeYmd, 1, 0);
			voldate.db_datecnvGen(0, SeYmd, &tmpGengo, WaYmd, 0, 0);
		}
		else{
			st = voldate.db_datecnvGen(0, pSday, &tmpGengo, SeYmd, 1, 1);
			voldate.db_datecnvGen(0, SeYmd, &tmpGengo, WaYmd, 0, 1);
		}
	}
	else{
		st = voldate.db_datecnvGenEx(0, pSday, &tmpGengo, SeYmd, 1, 2);
		voldate.db_datecnvGenEx(0, SeYmd, &tmpGengo, WaYmd, 0, 2);
	}
	WaYmd[1] = svYmd[1];
	WaYmd[2] = svYmd[2];

	if( st != -1 ){
		*pDGengo = tmpGengo;
		memmove(pDday, WaYmd, sizeof(WaYmd));
	}
	else{
		*pDGengo = 0;
		memset(pDday, '\0', sizeof(UCHAR)*4);
	}
}

//-----------------------------------------------------------------------------
// どの元号のプレプリントを使用するのか？
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		IsJudgKjn	：	個人事業かどうかを判断基準とするかどうか
//-----------------------------------------------------------------------------
// 返送値	1		：	平成タイプ
//			2		：	令和タイプ
//-----------------------------------------------------------------------------
int CSyzUtil::GetPrePrintGengo(CSnHeadData *pSnHeadData, bool IsJudgeKjn/*=true*/)
{
	int	rt = 1;

	ASSERT(pSnHeadData);
	if( pSnHeadData == NULL ){
		// エラーを特別扱いせずに、既存の平成タイプして扱う！
		return rt;
	}

	// 個人事業の時には、無条件で除外
	// 将来的に個人事業の帳票が出た時には、ここを外す！
	if( IsJudgeKjn ){
		if( pSnHeadData->IsSoleProprietor() ){
			return rt;
		}
	}

	// 課税期間の末日で判断！
	char	yy=0, mm=0, dd=0;
	int		gengo=0;
	if( !((pSnHeadData->Sn_SKKBN&0xff) % 2) ){
		pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd, &gengo);
	}
	else{
		pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd, &gengo);
	}

	if( gengo == ID_ICSSH_REIWA_GENGO ){
		if( (yy==1) && (mm>=5) ){
			rt = 2;
		}
		else if( yy >= 2 ){
			rt = 2;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// どの元号のプレプリントを使用するのか？
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		SyzVer		：	消費税改正バージョン
//		IsJudgKjn	：	個人事業かどうかを判断基準とするかどうか
//-----------------------------------------------------------------------------
// 返送値	1		：	平成タイプ
//			2		：	令和タイプ
//-----------------------------------------------------------------------------
int CSyzUtil::GetPrePrintGengo(CSnHeadData *pSnHeadData, int SyzVer, bool IsJudgeKjn/*=true*/)
{
	int	rt = 1;

	ASSERT(pSnHeadData);
	if( pSnHeadData == NULL ){
		// エラーを特別扱いせずに、既存の令和タイプして扱う！
		return 2;
	}

	// 個人事業種の場合は消費税改正バージョンにより判定
	// 8%の様式には令和が存在していないため
	if ( IsJudgeKjn ) {
		if ( ( pSnHeadData->IsSoleProprietor() ) && ( SyzVer == ID_VER_SYZ26 ) ) {
			return rt;
		}
	}

	// 課税期間の末日で判断！
	char	yy=0, mm=0, dd=0;
	int		gengo=0;
	if( !((pSnHeadData->Sn_SKKBN&0xff) % 2) ){
		pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd, &gengo);
	}
	else{
		pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd, &gengo);
	}

	if( gengo == ID_ICSSH_REIWA_GENGO ){
		if( (yy==1) && (mm>=5) ){
			rt = 2;
		}
		else if( yy >= 2 ){
			rt = 2;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 有効な令和年月日？
//-----------------------------------------------------------------------------
// 引数	gengo		：	元号
//		yy			：	年
//		mm			：	月
//		dd			：	日
//-----------------------------------------------------------------------------
// 返送値	true	：	有効な令和年月日
//			false	：	無効な令和年月日
//-----------------------------------------------------------------------------
bool CSyzUtil::IsValidReiwaYmd(int gengo, char yy, char mm, char dd)
{
	bool	bRt = true;

	if( gengo == ID_ICSSH_REIWA_GENGO ){
		if( ((yy&0xff)==0x01) &&
			(((mm&0xff)>=0x01) && ((mm&0xff)<=0x04))  ){
			// 日に関してはチェック対象外
			bRt = false;
		}
	}

	return bRt;
}




