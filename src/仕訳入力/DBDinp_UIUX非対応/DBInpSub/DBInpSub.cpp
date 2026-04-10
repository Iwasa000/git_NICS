// DBInpSub.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>

#include "DBInpSubFunc.h"
#include "icsdbedt.h"

#include "resource.h"
#include "EdaRegDlg.h"

#include "BaseDlg.h"

#include "TkregDlg.h"
#include "Forksel.h"
#include "TkyVer2Dlg.h"

#include "DBDinpRec.h"
#include "ScanKey.h"

#include "IndkmkDlg.h"
#include "KpubDlg.h"

#include "perform_chk.h"

#include "TkySel.h"
#include "CStRecordSet.h"
#include "DinpINI.h"

#include "DInputSub.h"
#include "DnpBaseClass.h"
#include "DnpInpDialog.h"



#ifdef _MANAGED
#error /clr と共にコンパイルするには DBInpSub.cpp 内の操作手順を参照してください。
// /clr をプロジェクトに追加したい場合は、次の操作を行ってください:
//	1. 上記の afxdllx.h のインクルードを削除する
//	2. .cpp ファイルを、プリコンパイル済みヘッダーが無効にされていて次のテキストを含み、
//	   /clr がスローされていないプロジェクトに追加します:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE DBInpSubDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE gInstance;
int WINAPI DenpInputDialogDestroy();


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DBInpSub.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(DBInpSubDLL, hInstance))
			return 0;

		gInstance = hInstance;

		// この DLL をリソース チェーンへ挿入します。
		// メモ: 拡張 DLL が MFC アプリケーションではなく
		//  MFC 標準 DLL (ActiveX コントロールのような)
		//  に暗黙的にリンクされる場合、この行を DllMain
		//  から削除して、この拡張 DLL からエクスポート
		//  された別の関数内へ追加してください。
		//  この拡張 DLL を使用する標準 DLL はこの拡張 DLL
		//  を初期化するために明示的にその関数を呼び出します。
		//  それ以外の場合は、CDynLinkLibrary オブジェクトは
		//  標準 DLL のリソース チェーンへアタッチされず、
		//  その結果重大な問題となります。

		new CDynLinkLibrary(DBInpSubDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DBInpSub.DLL Terminating!\n");
		//伝票形式ダイアログ削除
		DenpInputDialogDestroy();

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(DBInpSubDLL);
	}
	return 1;   // OK
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


int func_genymd_to_seireki( int src_ymd );


// 文字列科目コード より科目名称を取得
struct _DBKNREC* DB_PjisToKnrec( CString pjiscode, CDBZmSub* pZm, BOOL bALL/*=FALSE*/ )
{
#ifdef DB_VER1_CLOSE
	const int kmcd_max = 6;

	CString tmp;
	tmp = pjiscode;
	int l, n, i;
	l = tmp.GetLength();
	if( (n = (kmcd_max - l)) > 0 ) {
		for( i = 0; i < n; i++ ) {
			tmp += " ";
		}
	}
#endif
	DWORD tmp;
	tmp = _atoh( (char*)(LPCTSTR)pjiscode );
	hl_rev( &tmp, 4 );

	DBKNREC* pKn;
	pKn = pZm->PjisToKnrec( tmp );

	if( pKn ) {
		if( bALL ) {
			return pKn;
		}
		else {
			if( (pKn->knvoid & 0x01) )
				return NULL;
			else
				return pKn;
		}
	}
	
	return NULL;
}


// DWORD 科目コード より科目名称を取得
struct _DBKNREC* DB_PjisToKnrec( DWORD dwPjis, CDBZmSub* pZm, BOOL bALL/*=FALSE*/  )
{
	DBKNREC* pKn;
	pKn = pZm->PjisToKnrec( dwPjis );

	if( pKn ) {
		if( bALL ) {
			return pKn;
		}
		else {
			if( (pKn->knvoid & 0x01) )
				return NULL;
			else
				return pKn;
		}
	}
	
	return NULL;
}


// 拡張合計名称の 元合計名称取得
DBGKREC* get_gksrc( DWORD gkcode, CDBZmSub* pZm )
{
	DWORD dwCode, tmp;
	DBGKREC* pGk;

	do {
		pGk = pZm->PjisToGkrec( gkcode );

		if( pGk ) {
			tmp = pGk->gksrc;
			hl_rev( &tmp, 4 );
			
			dwCode = (tmp & 0x0000f000);
			if( (dwCode & 0x00008000) <= 0 ) 
				break;
			else {
				// まだ、追加小分類
				gkcode = pGk->gksrc;
				pGk = NULL;
			}
		}
		else {
			pGk = NULL;
			break;
		}
	} while( 1 );

	// 追加基本小分類
	return pGk;
}


// 文字列科目コード より科目名称を取得
struct _DBKNREC* DB_SyusiPjisToKnrec( CString pjiscode, CDBZmSub* pZm )
{
	DWORD tmp;
	tmp = _atoh( (char*)(LPCTSTR)pjiscode );
	hl_rev( &tmp, 4 );


	return DB_SyusiPjisToKnrec( tmp, pZm );
}


// DWORD 科目コード より科目名称を取得
struct _DBKNREC* DB_SyusiPjisToKnrec( DWORD dwPjis, CDBZmSub* pZm )
{
	DBKNREC* pKn;
	pKn = pZm->SyusiPjisToKsrec( dwPjis );

	if( pKn ) {
		if( (pKn->knvoid & 0x01) )
			return NULL;
		else
			return pKn;
	}
	
	return NULL;
}



void set_codestr( char* str, int strsize, int code, int keta )
{
	char fmt[10];

	sprintf_s( fmt, sizeof fmt, "%%0%dd", keta );

	if( code != -1 )	sprintf_s( str, strsize, fmt, code );
	else				str[0] = '\0';
}


void set_codestring( char* str, int strsize, char* codestring, int keta )
{
	char fmt[10];

	sprintf_s( fmt, sizeof fmt, "%%0%ds", keta );
	CString stmp( codestring );

	if( ! stmp.IsEmpty() ) {
		stmp.TrimRight();	//左右のスペース除去
		stmp.TrimLeft();
		sprintf_s( str, strsize, fmt, stmp );
	}
	else {
		::ZeroMemory( str, strsize );
	}
}


// 科目コードを 変換
void TV_kamokucodeConv( WORD& kmkcode, CString& codestr, int sgn )
{
	if( sgn == CONV2_1 ) {
		int tmp = _atoh( (char*)(LPCTSTR)codestr );
		kmkcode = hl_lev( tmp );
	}
	else {
		codestr.Format( "%04x", hl_lev( kmkcode ) );
	}
}



// 税率タイプ	返送値 5%
//				返送値 8%
//
int SyzZeiritsuType( CDBZmSub* pZm )
{
	//マスターバージョンによる。
	//vtbl
	CZmGen8		ZmGen;
	int vver = ZmGen.CheckValiableVersion( pZm->m_database, ID_H26SYZ_VUP_CHK );

	int z_type = 5;

	if( vver >= 1 ) {

		int ver2 = ZmGen.CheckValiableVersion( pZm->m_database, ID_H31SYZ_VUP_CHK );

		EnumIdSyzKikan	syzKikan = ZmGen.GetSyzKikan(pZm->zvol->ee_ymd);
		
		if( ver2 >= 1 ) {
			switch( syzKikan ) {
			case ID_ZMG_SYZ_10PER_KIKAN:
				z_type = 10;
				break;
			case ID_ZMG_SYZ_8PER_KIKAN:
				z_type = 8;
				break;
			}
		}
		else {
			if( syzKikan == ID_ZMG_SYZ_8PER_KIKAN ) {
				z_type = 8;
			}
		}
	}

	return z_type;
}


DBSUB_FUNC
BOOL WINAPI isdigitstr( char* p ) 
{
# ifdef CLOSE // 07.10 /01
	char* chk;
	chk = p;
# else
	unsigned char* chk;
	chk = (unsigned char*)p;
# endif

	while( *chk ) {
		if( isdigit( *chk++ ) )
			continue;
		else
			return FALSE;
	}
	return TRUE;
}


DBSUB_FUNC
int WINAPI kana_chk( CString *bf, int ln )
{
	int		i;
	TCHAR c;
	TRACE("kana_chk TOP bf = %s, ln(%d)\n", *bf, ln);

	for( i=0; i < ln; i++ ) {
		c = bf->GetAt(i);

		if( _cclass( c ) ) return( 1 );
		else if( c == 0 ) return ( 0 );

		if( c == 'ｰ' ) bf->SetAt( i, '-' );
	}

	return( 0 );
}

//------------------------------------------
//	文字列の ゼロ を取り除く
//
//------------------------------------------
DBSUB_FUNC
void WINAPI trim_zero( char *bf, int sizebf, int ln )
{
	int		i, n, sw;
	int		sz;
	LPSTR	p;
	CString	tmp;

	sz = (ln < 256) ? 256 : (ln + 64);

	p = tmp.GetBuffer( sz );

	sw = n = 0;
	for( i = 0; i < ln; i++ ) {
		if( bf[i] == '0' ) {
			if( ! sw )	continue;
			else {
				p[n++] = bf[i];
			}
		}
		else {
			if( ! sw )	sw = 1;
			p[n++] = bf[i];
		}
	}
	p[n] = '\0';
	tmp.ReleaseBuffer();
	
	strcpy_s( bf, sizebf, tmp );
}



/*-----------------------------------
	漢字スペースカット
------------------------------------*/
DBSUB_FUNC
int WINAPI ksp_cut( char *des, char *sou, int n )
{
	int cnt, i, st;
	char* pchk = sou;

	cnt = i = 0;
	for( ; n > 0 ; )
	{
		if( !*sou )
			break;
		else {
			st = IsKindOfLetter( pchk, i );

			if( st == 1 ) {			//ascii
				if( *sou == ' ' )	sou++;
				else {
					*des++ = *sou++;
					cnt++;
				}
				i++;
				n--;
			}
			else if( st == 2 ) {	// 漢字
				if( !strncmp( sou, "　", 2 ) ){
					sou += 2;
				}
				else {
					*des++ = *sou++;
					*des++ = *sou++;
					cnt += 2;
				}
				i += 2;
				n -= 2;
			}
			else	return 0;
		}
	}
	*des = '\0';
	return cnt;
}


/*==========================================================
	入力されたカナバッファよりチェックしてコピーする
	基本的には、絞込み検索のため 1 バイトだか、
	ｼｬ, ﾊﾟ		など は、2バイト分コピーする
	ｼﾞｬ, ﾋﾞｬ	など は、3バイト分コピーする

  ==========================================================*/
DBSUB_FUNC 
void WINAPI kana_copy( char *copybuf, LPCTSTR kana )
{
	BYTE chk[2];
	chk[0] = (BYTE)kana[1];
	chk[1] = (BYTE)kana[2];

	copybuf[0] = (char)kana[0];
	copybuf[1] = '\0';

//	if( (chk >= 'ｧ' && chk <= 'ｯ') || chk == 'ﾟ' || chk == 'ﾞ' ) {
	if( (chk[0] >= (BYTE)0xA7 && chk[0] <= (BYTE)0xAF) || chk[0] == (BYTE)0xDE || chk[0] == (BYTE)0xDF ) {
		copybuf[1] = chk[0];
		copybuf[2] = '\0';

		if( (chk[1] >= (BYTE)0xA7 && chk[1] <= (BYTE)0xAF) ) {
			copybuf[2] = chk[1];
			copybuf[3] = '\0';
		}
	}
}


/*==========================================================
	ローマ字カナ入力で 途中の状態
	ｼｻ と入力する前の　ｼS のようなとき、S は除く

  ==========================================================*/
DBSUB_FUNC
void WINAPI kana_check(char *copybuf, LPCTSTR kana)
{
	int idx = 0;
	BYTE	moji;

	for( int n = 0; kana[n] != '\0'; n++ ) {
		moji = (BYTE)kana[n];
		//半角長音 変換
		if( moji == (BYTE)'ｰ' ) moji = (BYTE)'-';

		if( (moji >= (BYTE)0xA6 && moji <= (BYTE)0xDF) || moji == (BYTE)'-' ) {
			copybuf[idx++] = moji;
		}
	}
}



DBSUB_FUNC
void WINAPI DWORDcodeToSTRcode( DWORD kncod, char* string, int strsize )
{
	DWORD tmp;
	tmp = kncod;

	hl_rev( &tmp, 4 );
	sprintf_s( string, strsize, "%08x", tmp );
}


// 月日をBCDで返送
//	date[0] = 0x00->決修, 0x6X 中間 
//
//  dst_date[0],[1] には 通常の日付をセット
//	dst_date[0] ... D7-D5 bit が 0 = 通常, 1 = 決修, 2 = 中間
//
DBSUB_FUNC
int WINAPI Myvd_sign( BYTE* date, BYTE* dst_date, CDBZmSub* pZm )
{
	int ret = -1;

	BYTE work[6] = {0};
	BYTE smd[4] = {0};
	BYTE emd[4] = {0};
	BYTE sgn = 0;

	ASSERT( (date[1] > 0x00 && date[1] <= 0x31) || date[1] == (BYTE)0xff );

	if( *date == 0x00) {	//決修
		pZm->minfo->MoveFirst();
		do {
			if( pZm->minfo->kes_sgn == 4 ) {
				DateConv( smd, (int*)&pZm->minfo->smd, CONV2_1 );
				DateConv( emd, (int*)&pZm->minfo->emd, CONV2_1 );
		
				if( date[1] != (BYTE)0xff ) {
#ifdef CLOSE
					if( date[1] >= smd[1] ) {
						work[0] = smd[0];
					}
					else if( date[1] <= emd[1] ) {
						work[0] = emd[0];
					}
#endif
					if( date[1] <= emd[1] ) {
						work[0] = emd[0];
					}
					else if( date[1] >= smd[1] ) {
						work[0] = smd[0];
					}
					work[1] = date[1];
				}
				else {
					memcpy( work, emd, 2 );
				}
				break;
			}
		} while( pZm->minfo->MoveNext() == 0 );
		sgn = 4;
	}
	else if( (*date & 0xf0) == 0x60 ) {	//中間
		pZm->minfo->MoveFirst();
		int chu_sg;
		chu_sg = (*date & 0x0f);
		do {

			if( pZm->minfo->kes_sgn == chu_sg /*&& pZm->minfo->ofset < 13*/ ) {
				DateConv( smd, (int*)&pZm->minfo->smd, CONV2_1 );
				DateConv( emd, (int*)&pZm->minfo->emd, CONV2_1 );

				if( date[1] != (BYTE)0xff ) {
#ifdef CLOSE
					if( date[1] >= smd[1] ) {
						work[0] = smd[0];
					}
					else if( date[1] <= emd[1] ) {
						work[0] = emd[0];
					}
#endif 
					if( date[1] <= emd[1] ) {
						work[0] = emd[0];
					}
					else if( date[1] >= smd[1] ) {
						work[0] = smd[0];
					}

					work[1] = date[1];
				}
				else {
					memcpy( work, emd, 2 );
				}
				break;
			}
		} while( pZm->minfo->MoveNext() == 0 );
		sgn = chu_sg;
	}
	else {	//　通常
		pZm->minfo->MoveFirst();
		int yy1 = pZm->minfo->emd;
		do {
			DateConv( smd, (int*)&pZm->minfo->smd, CONV2_1 );
			DateConv( emd, (int*)&pZm->minfo->emd, CONV2_1 );

			if( date[1] == (BYTE)0xff ) {	//日の入力なし
				if( date[0] == emd[0] ) {
					work[0] = emd[0];
					break;
				}
			}
			else {
				//比較用に年度越えサインをセット。
				int yy1, yy2, yy3, ss_mmdd;
				yy1 = pZm->zvol->ss_ymd/10000;
				yy2 = pZm->minfo->smd/10000;
				yy3 = pZm->minfo->emd/10000;

				//開始日以前は年度越え
				ss_mmdd	= (pZm->zvol->ss_ymd%10000);
				char buf[36];
				sprintf_s( buf, "%02x%02x", date[0], date[1] );
				int num_date = atoi( buf );
				if( ss_mmdd > num_date ) date[0] |=0x80;

				if( yy1 < yy2 )	smd[0] |= 0x80;
				if( yy1 < yy3 )	emd[0] |= 0x80;

				if( memcmp( date, smd, 2 ) >= 0 && memcmp( date, emd, 2 ) <= 0 ) {
					if( (date[0]&0x1f) == (smd[0]&0x1f) ) {
						work[0] = (smd[0]&0x1f);
						work[1] = date[1];
						break;
					}
					else if( (date[0]&0x1f) == (emd[0]&0x1f) ) {
						work[0] = (emd[0]&0x1f);
						work[1] = date[1];
						break;
					}
				}
			}
		} while( pZm->minfo->MoveNext() == 0 );

	}
	if( work[0] ) {
		// うるう年のチェック
		if( work[0] == (BYTE)0x02 && work[1] == (BYTE)0x29 ) {
			int yy, mm;

			pZm->minfo->MoveFirst();
			do {
				yy = pZm->minfo->smd/10000;
				mm = (pZm->minfo->smd%10000) / 100;
				if( mm == 2 ) {
					// うるう年のチェック
					if( ! uru_year( yy ) )
						return -1;

					break;
				}
			} while( pZm->minfo->MoveNext() == 0 );
		}
		memcpy( dst_date, work, 2 );
		
		KESY_SG_SET( dst_date[0], sgn );
		ret = 0;
	}

	return ret;
}


// 月日を 変換
DBSUB_FUNC
int	WINAPI DateConv( BYTE* bcddate, int* date, int sgn )
{
	char buf[20];

	if( sgn == CONV1_2 ) {
		// BCD -> int	
		sprintf_s( buf, sizeof buf, "%02x%02x", (*bcddate)&0x1f, *(bcddate+1) );
		*date = atoi( buf );
	}
	else {
		// int → BCD
		sprintf_s( buf, sizeof buf, "%d", *date );
		if( strlen( buf ) == 8 ) {
			l_pack( bcddate, &buf[4], 4 );
		}
		else {
			l_pack( bcddate, &buf[0], 4 );
		}
	}

	return 0;
}


// 年月日を 変換
DBSUB_FUNC
int	WINAPI YmdConv( BYTE* bcdymd, int* date, int sgn )
{
	char buf[30];

	if( CONV1_2 ) {
		// BCD -> int	
		l_unpac( buf, bcdymd, 8 );
		*date = atoi( buf );
	}
	else {
		// int → BCD
		sprintf_s( buf, sizeof buf,  "%d", *date );
		l_pack( bcdymd, &buf[0], 8 );
	}

	return 0;
}

#ifdef CLOSE
// BCD 日付 から int 日付へ 変換
DBSUB_FUNC
int WINAPI BcdDateToYmd( BYTE* bcddate, int* ymd, CDBZmSub* pZm )
{
	BYTE bcd[10] = {0};
	int ymdbuff, date;

	pZm->minfo->MoveFirst();
	do {
		DateConv( bcd, (int*)&pZm->minfo->smd, CONV2_1 );

		if( ((*bcddate)&0x1f) == bcd[0] ) {
			ymdbuff = pZm->minfo->smd / 10000;
			ymdbuff *= 10000;
			DateConv( bcddate, &date, CONV1_2 );
			ymdbuff += date;
			*ymd = ymdbuff;
			break;
		}
	} while( pZm->minfo->MoveNext() == 0 );
	return 0;
}
#endif

// BCD を文字列に
//  BCD[0],[1] には 通常の日付をセット
//	BCD[0] ... D7-D5 bit が 0 = 通常, 1 = 決修, 2-5 = 中間
DBSUB_FUNC
void WINAPI BcdDateToString( char* str, int strsize, BYTE* bcddate, BOOL bMnthFix /*=FALSE*/)
{
	if( ! bMnthFix ) {
		if( bcddate[0] != 0 && bcddate[1] != 0 ) {
			if( KESY_SG_GET( bcddate[0] ) == 0 )
				sprintf_s( str, strsize, "%02x.%02x", (bcddate[0]&0x1f), bcddate[1] );
			else
				sprintf_s( str, strsize, "%02x/%02x", (bcddate[0]&0x1f), bcddate[1] );
		}
		else	strcpy_s( str, strsize, "      " );
	}
	else {
		if( bcddate[0] != 0 && bcddate[1] != 0 ) {
			sprintf_s( str, strsize, "%02x", bcddate[1] );
		}
		else	strcpy_s( str, strsize, "      " );	
	}
}

DBSUB_FUNC
void WINAPI BcdDateToString( CString& string, BYTE* bcddate, BOOL bMnthFix /*=FALSE*/)
{
	LPTSTR p;
	p = string.GetBuffer(20);
	BcdDateToString( p, 20, bcddate, bMnthFix );
	string.ReleaseBuffer(-1);
}

// int 西暦日付を 平成年月日 文字列に
DBSUB_FUNC
void WINAPI IntsrekitoGenstr( char* str, int strsize, int ddate )
{
	int mm, dd, yy, gengo;

	if( ddate <= 0 ) str[0] = '\0';
	else {
		CString genstr;

		if( SeirekiToWareki( ddate, gengo, yy, mm, dd, genstr ) == 0 ) {
			sprintf_s( str, strsize, "%02d.%02d.%02d", yy, mm, dd );
		}
		else {
			 str[0] = '\0';
		}
	}
}

DBSUB_FUNC
int WINAPI _op_max_calq( int keta )
{
	char nine = '9';
	char buf[30] = {0};

	for( int i = 0; i < keta; i++ ) {
		buf[i] = nine;
	}

	return atoi( buf );
}

/////////////////////////////////////////////////////////////////////////////
// DBxxx ocx にセットする関数

DBSUB_FUNC
long WINAPI DBdata_set( CWnd* pwnd, int id, VARIANT* data, int type, int bcdlen )
{
//TRACE( "DBdata_set %02x%02x%02x%02x%02x%02x ln = %d\n",	data->pbVal[0], data->pbVal[1], data->pbVal[1], data->pbVal[3],
//														data->pbVal[4], data->pbVal[5], get_nowln() );
#ifdef TEST_DEBUG
	char buf[10] = {0};
	if( memcmp( data->pbVal, buf, 6 ) == 0 ) {
		DebugBreak();
	}
#endif

	return ((CICSDBEDT*)pwnd->GetDlgItem( id ))->SetData( data, type, bcdlen );
}

DBSUB_FUNC
long WINAPI DBdata_get( CWnd* pwnd, int id, VARIANT* data, int type, int bcdlen )
{
	int ret;
	ret = ((CICSDBEDT*)pwnd->GetDlgItem( id ))->GetData( data, type, bcdlen );	
//TRACE( "DBdata_get %02x%02x%02x%02x%02x%02x ln = %d\n",	data->pbVal[0], data->pbVal[1], data->pbVal[1], data->pbVal[3],
//														data->pbVal[4], data->pbVal[5], get_nowln() );
	return ret;
}

DBSUB_FUNC
void WINAPI DBSetDispString( CWnd* pwnd, int id, LPCTSTR string )
{
	((CICSDBEDT*)pwnd->GetDlgItem( id ))->SetDisplayString( string );
}


// 11.07 /08
static int gKmcdSgn = 0;
DBSUB_FUNC
void WINAPI KamokucodeSign( int sgn )
{
	gKmcdSgn = sgn;
}


int getKamokucodeSign()
{
	return gKmcdSgn;
}


// 科目コントロールデータ取得
DBSUB_FUNC
int WINAPI DBkamokudata_get( CWnd* pwnd, int id, struct _KamokuData* data, CDBZmSub* pZm )
{
	VARIANT var;
	((CICSDBEDT*)pwnd->GetDlgItem( id ))->GetData( &var, ICSDBEDT_TYPE_STRING, 0 );

	::ZeroMemory( data, sizeof (struct _KamokuData));
//TRACE( "DBkamokudata_get id %d\n", id );

	int ret;
	if( (ret = KamokuStr_ChkLength( (char*)var.pbVal, data, pZm->zvol->edcol )) == 0 ) {
		if( ! data->kd_name[0] ) {
			CString code;
			code = data->kd_code;
			struct	_DBKNREC* pKn;
			if( pKn = DB_PjisToKnrec( code, pZm, TRUE ) ) {
				if( gKmcdSgn ) {
					kmkcode_string( 2, pKn, data->kd_name, sizeof data->kd_name );
				}
				else {
					sprintf_s(data->kd_name, sizeof data->kd_name, "%.14s", pKn->knnam ); 
				}
			}
		}
	}
	return ret;
}

// 科目コントロールデータをセット
DBSUB_FUNC
int WINAPI DBkamokudata_set( CWnd* pwnd, int id, struct _KamokuData* data, CDBZmSub* pZm )
{
	char tmp[30] = {0};
	char buf[64] = {0};

	if( data->kd_eda != -1 )
		sprintf_s( tmp, sizeof tmp, "%s.%d", data->kd_code, data->kd_eda );
	else if( data->kd_code[0] )
		sprintf_s( tmp, sizeof tmp, "%s.", data->kd_code );

	VARIANT var;
	var.pbVal = (BYTE*)tmp;
	((CICSDBEDT*)pwnd->GetDlgItem( id ))->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
	
	if( data->kd_name[0] ){
		char edabuf[20];
		set_codestr( edabuf, sizeof edabuf, data->kd_eda, pZm->zvol->edcol );
		sprintf_s( buf, sizeof buf, "%.14s%s", data->kd_name, edabuf );
	}
	else
		strcpy_s( buf, sizeof buf, "              " );
	((CICSDBEDT*)pwnd->GetDlgItem( id ))->SetDisplayString( buf );

//TRACE( "DBkamokudata_set\n" );

	return 0;
}

// 手形期日 入力文字列(ocxより)のチェック
// 西暦での入力は、やめておく
//	char* check		チェック対象文字列
//	char* result	変換後文字列格納バッファ( YY.MM.DD 平成年 )
//	int rs_sz		result のバッファサイズ
//	int symd, int eymd	ボリュームラベルの開始年月日
//	int s_tdate			前行の手形期日(未使用)
//
//	return -1	不正な入力, 正しい場合は、3 以上の入力文字数
//
DBSUB_FUNC
short WINAPI TgDateInputCheck( char* check, char* result, int rs_sz, int symd, int eymd, int s_tdate )
{
	int	len	= strlen( check );

	int	daytbl[13]	=	{ 31, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	char	tmp[16];
	int	mnth, day, yy;

	memset( tmp, 0, sizeof tmp );

	if( len <= 0 ) {
		return	-1;
	}

	for( int i = 0; i < len; i++ ) {
		if( IsCharAlphaNumeric( check[i] ) && ! IsCharAlpha( check[i] ) );
		else 
			return -1;
	}

	switch( len ) {
#ifdef CLOSE	//月日は最低必要
	case 1:
		mnth = -1;
		tmp[0] = '0';
		tmp[1] = check[0];
		day = atoi( tmp );
		if( day == 0 )	
			return -1;

		break;
	case 2:
		mnth = -1;
		tmp[0] = check[0];
		tmp[1] = check[1];
		day = atoi( tmp );
		if( day == 0 )	
			return -1;
	
		break;
#endif 
	case 3:
		tmp[0] = '0';
		tmp[1] = check[0];
		mnth = atoi( tmp );
		if( ! (mnth >= 1 && mnth <= 12) )
			return -1;
		tmp[0] = check[1];
		tmp[1] = check[2];
		day = atoi( tmp );
		if( day > daytbl[mnth] || day == 0 )
			return -1;
		break;
	case 4:
		tmp[0] = check[0];
		tmp[1] = check[1];
		mnth = atoi( tmp );
		if( ! (mnth >= 1 && mnth <= 12) )
			return -1;
		tmp[0] = check[2];
		tmp[1] = check[3];
		day = atoi( tmp );
		if( day > daytbl[mnth] || day == 0 )
			return -1;
		break;
	case 5 :	// 年は元号とみなす
		tmp[0] = check[0];
		yy = atoi( tmp );
		tmp[0] = check[1];
		tmp[1] = check[2];
		mnth = atoi( tmp );
		if( ! (mnth >= 1 && mnth <= 12) )
			return -1;
		tmp[0] = check[3];
		tmp[1] = check[4];
		day = atoi( tmp );
		if( day > daytbl[mnth] || day == 0 )
			return -1;
		break;

	case 6 :	// 年は元号とみなす
		tmp[0] = check[0];
		tmp[1] = check[1];
		yy = atoi( tmp );
		tmp[0] = check[2];
		tmp[1] = check[3];
		mnth = atoi( tmp );
		if( ! (mnth >= 1 && mnth <= 12) )
			return -1;
		tmp[0] = check[4];
		tmp[1] = check[5];
		day = atoi( tmp );
		if( day > daytbl[mnth] || day == 0 )
			return -1;
		break;
#ifdef CLOSE	// 元号のみの入力とする
	case 7 :	// 年は西暦とみなす
		tmp[0] = check[0];
		tmp[1] = check[1];
		tmp[2] = check[2];
		yy = atoi( tmp );
		if( yy >= 88 && yy <= 100 )	yy += 1900;
		else						yy += 2000;
		tmp[0] = check[3];
		tmp[1] = check[4];
		tmp[2] = '\0';
		mnth = atoi( tmp );
		if( ! (mnth >= 1 && mnth <= 12) )
			return -1;
		tmp[0] = check[5];
		tmp[1] = check[6];
		tmp[2] = '\0';
		day = atoi( tmp );
		if( day > daytbl[mnth] )
			return -1;
		break;
	case 8 :	// 年は西暦とみなす
		tmp[0] = check[0];
		tmp[1] = check[1];
		tmp[2] = check[2];
		tmp[3] = check[3];
		yy = atoi( tmp );
		tmp[0] = check[4];
		tmp[1] = check[5];
		tmp[2] = '\0';
		mnth = atoi( tmp );
		if( ! (mnth >= 1 && mnth <= 12) )
			return -1;
		tmp[0] = check[6];
		tmp[1] = check[7];
		tmp[2] = '\0';
		day = atoi( tmp );
		if( day > daytbl[mnth] )
			return -1;
		break;
#endif
	default:
//		ASSERT( FALSE );
		return -1;
		break;
	}

	// 月をセットする
	int sw = 0;
	int sy, sm, sd, eyy, emm, edd;
	
	CVolDateDB vd;
	int gen_ymd, sreki_ymd;

	if( len <= 4 ) {
		if( symd <= 0 && eymd <= 0 )
			return -1;

		sy = (symd / 10000);
		sm = (symd % 10000) / 100;
		sd = (symd % 100);

		eyy = (eymd / 10000);
		emm = (eymd % 10000) / 100;
		edd = (eymd % 100);

		if( mnth >= sm )	yy = sy;
		else				yy = eyy;

		sreki_ymd = yy * 10000;
		sreki_ymd += mnth * 100;
		sreki_ymd += day;
	}
	else if( len <= 6 ) {
		gen_ymd = yy * 10000;
		gen_ymd += mnth * 100;
		gen_ymd += day;

		sreki_ymd = func_genymd_to_seireki( gen_ymd );
		yy = (sreki_ymd/10000);
	}

	// うるう年のチェック
	if( mnth == 2 && day == 29 ) {
		if( ! uru_year( yy ) )
			return -1;
	}

	// 西暦から 元号へ変換
	int gengo;

	if( vd.db_datecnvGen( 0, sreki_ymd, &gengo, &gen_ymd, 0, 0 ) != 0 )
		return -1;

	sprintf_s( result, rs_sz, "%06d", gen_ymd );

	return	len;
}


DBSUB_FUNC
BOOL WINAPI uru_year( int year )
{
	if( !(year%400) || ( !(year%4) && (year%100) ) )
		return TRUE;

	return FALSE;
}


/////////////////////////////////////////////////////////////
// 枝番登録ダイアログ
DBSUB_FUNC 
int WINAPI EdabanTorokDialog( EDA_RECORD* pEd, CDBZmSub* pZm, CWnd* pwnd, BOOL* teki_query/* = NULL*/ )
{
//BEGIN_PERFORMANCE

HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

	CEdaRegDlg dlg(pwnd);
	dlg.InitEdaRegDlg( pEd, pZm );

	int st = dlg.DoModal();

AfxSetResourceHandle( h );

	char bf[64] = {0};
	CString filter;
	CEZREC*	ezrec;
	CBMEZREC* bmez;
	DBKNREC* pKn;

	// tkrec の カナ検索をしているか？
	if( teki_query != NULL ) {
		*teki_query = dlg.tkrec_cnd();
	}
	BYTE knsgn2, knsgn3;
	BYTE tksgn3, tksgn4;
	tksgn3 = tksgn4 = 0;

	BOOL bTkSyz = FALSE;

	pKn = DB_PjisToKnrec(CString(pEd->ed_kcod), pZm, TRUE);

	if (pEd->ed_tkcod && (pKn->knsgn[2] & 0x0f) != 0) {
		if (pZm->zvol && (pZm->zvol->s_sgn2 & 0x10)) {

			if (!tkyngetr(pEd->ed_tkcod, pZm)) {
				bTkSyz = TRUE;

				switch (pZm->tkrec->tksgn[1]) {
				case 1:		//課税
					knsgn2 = 0x02;
					knsgn3 = 0;
					break;
				case 2:		//非課税　　
					knsgn2 = 0x03;
					knsgn3 = 0;
					break;
				case 3:		//不課税
					knsgn2 = 0x01;
					knsgn3 = 0;
					break;
				case 4:		//経過－３％
					knsgn2 = 0x02;
					knsgn3 = 0x10;
					break;
				case 5:		//経過－５％
					knsgn2 = 0x02;
					knsgn3 = 0x30;
					break;
				case 6:		//経過－８％
					knsgn2 = 0x02;
					knsgn3 = 0x40;
					break;
				case 7:		//軽減－８％
					knsgn2 = 0x02;
					knsgn3 = 0x50;
					break;
				case 8:		//標準－10％
					knsgn2 = 0x02;
					knsgn3 = 0x00;
					break;

				default:
					knsgn2 = pKn->knsgn[2];
					knsgn3 = pKn->knsgn[3];
					break;
				}
				if( pZm->tkrec->dsign.GetSize() != SIZE_DSIGN ) {
					pZm->tkrec->dsign.SetSize(SIZE_DSIGN);
				}
				tksgn3 = pZm->tkrec->dsign[3];
				tksgn4 = pZm->tkrec->dsign[4];
			}
		}
	}

	if( st == IDOK ) {

#ifdef OLD_CLOSE
		if( pEd->ed_bmn != -1 ) {
			bmez = pZm->bmezrec;
			filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", pEd->ed_bmn, pEd->ed_kcod, pEd->ed_ecod );
			
			try {

				if( bmez->Requery( filter ) != 0 ) {

					bmez->AddNew();
					// 先に追加だけしておく
					bmez->ezbmn	= pEd->ed_bmn;
					bmez->ezkcd	= pEd->ed_kcod;
					bmez->ezecd	= pEd->ed_ecod;
					bmez->eztcd	= pEd->ed_tkcod;

					pKn = DB_PjisToKnrec( CString(pEd->ed_kcod), pZm );
					bmez->ezatt	= pKn->knatt;
				
					bmez->Update();
				//	bmez->Requery("", 1, 0);
					
					// 続いて編集
					if( bmez->Requery( filter ) != 0 ) {
						ASSERT( FALSE );
						goto ERR_TAG;
					}
					bmez->Edit();

			//		bmez->ezsign.Copy( ez->m_ezsign );
			//		bmez->ezritsu1 = ez->m_ezritsu1;
			//		bmez->ezritsu2 = ez->m_ezritsu2;

					if( ! bmez->eztcd ) {
						bmez->ezname	= pEd->ed_name;
						bmez->ezkana	= pEd->ed_kana;
					}
					l_print( bf, pEd->ed_zan, FMT12Z );
					bmez->ezzan	= bf;
					bmez->Update();

			/*		int max = sizeof ezrec->lhs / sizeof ezrec->lhs[0];
					for( int i = 0; i < max; i++ ) {
						ezrec->lhs[i] = ez->m_lhs[i];
						ezrec->rhs[i] = ez->m_rhs[i];
					} */
				}

				// 科目枝番が未登録のときは、一緒に登録する
				if( dlg.m_bKEzanFlg ) {
					ezrec = pZm->ezrec;
					filter.Format( "ezkcd = '%s' AND ezecd = %d", pEd->ed_kcod, pEd->ed_ecod );

					if( ezrec->Requery( filter, 1 ) != 0 ) {

						ezrec->AddNew();
						// 先に追加だけしておく
						ezrec->ezkcd	= pEd->ed_kcod;
						ezrec->ezecd	= pEd->ed_ecod;
						ezrec->eztcd	= pEd->ed_tkcod;
						ezrec->ezatt	= pKn->knatt;
					
						ezrec->Update();
					//	ezrec->Requery("", 1);
						
						// 続いて編集
						if( ezrec->Requery( filter, 1 ) != 0 ) {
							ASSERT( FALSE );
							goto ERR_TAG;
						}
						ezrec->Edit();

				//		ezrec->ezsign.Copy( ez->m_ezsign );
				//		ezrec->ezritsu1 = ez->m_ezritsu1;
				//		ezrec->ezritsu2 = ez->m_ezritsu2;

						if( ! ezrec->eztcd ) {
							ezrec->ezname	= pEd->ed_name;
							ezrec->ezkana	= pEd->ed_kana;
						}

						l_print( bf, dlg.m_KEzan, FMT12Z );
						ezrec->ezzan	= bf;
						ezrec->Update();
					}
				}
			}
			catch( CDBException* db ) {
				goto ERR_TAG;
			}
			catch( CMemoryException* me ) {
				goto ERR_TAG;
			}
		}
		else {
#endif
			ezrec = pZm->ezrec;
			filter.Format( "ezkcd = '%s' AND ezecd = %d", pEd->ed_kcod, pEd->ed_ecod );

			try {

				if( ezrec->Requery( filter, 1 ) != 0 ) {

					ezrec->AddNew();
					// 先に追加だけしておく
					ezrec->ezkcd	= pEd->ed_kcod;
					ezrec->ezecd	= pEd->ed_ecod;
					ezrec->eztcd	= pEd->ed_tkcod;

					pKn = DB_PjisToKnrec( CString(pEd->ed_kcod), pZm, TRUE );
					ezrec->ezatt	= pKn->knatt;
					memcpy(&ezrec->ezsysgn[2], &pKn->knsgn[2], 4);
					ezrec->ezsysgn[5] = 0;

					ezrec->Update();
				//	ezrec->Requery("", 1);

				}
					
				// 続いて編集
				if( ezrec->Requery( filter, 1 ) != 0 ) {
					ASSERT( FALSE );
					goto ERR_TAG;
				}
				ezrec->Edit();

		//		ezrec->ezsign.Copy( ez->m_ezsign );
		//		ezrec->ezritsu1 = ez->m_ezritsu1;
		//		ezrec->ezritsu2 = ez->m_ezritsu2;

				if( pEd->ed_tkcod ) {
					ezrec->eztcd	= pEd->ed_tkcod;
					if (bTkSyz) {
						ezrec->ezsysgn[2] = knsgn2;
						ezrec->ezsysgn[3] &= ~(0xf0);
						ezrec->ezsysgn[3] |= knsgn3;

						//摘要で売上区分
						ezrec->ezsysgn[3] &= ~(0x0f);
						ezrec->ezsysgn[3] |= tksgn3;
						//摘要で仕入区分・免税
						ezrec->ezsysgn[4] = tksgn4;
					}
				}
				else {
					ezrec->ezname	= pEd->ed_name;
					ezrec->ezkana	= pEd->ed_kana;
				}

				l_print( bf, pEd->ed_zan, FMT12Z );
				ezrec->ezzan	= bf;
				ezrec->Update();

		/*		int max = sizeof ezrec->lhs / sizeof ezrec->lhs[0];
				for( int i = 0; i < max; i++ ) {
					ezrec->lhs[i] = ez->m_lhs[i];
					ezrec->rhs[i] = ez->m_rhs[i];
				} */
				
				if( pEd->ed_bmn != -1 && (pZm->zvol->bm_isw & 0x02) ) {
					bmez = pZm->bmezrec;
					filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", pEd->ed_bmn, pEd->ed_kcod, pEd->ed_ecod );
				
					if( bmez->Requery( filter ) != 0 ) {

						bmez->AddNew();
						// 先に追加だけしておく
						bmez->ezbmn	= pEd->ed_bmn;
						bmez->ezkcd	= pEd->ed_kcod;
						bmez->ezecd	= pEd->ed_ecod;
						bmez->eztcd	= pEd->ed_tkcod;

						pKn = DB_PjisToKnrec( CString(pEd->ed_kcod), pZm, TRUE );
						bmez->ezatt	= pKn->knatt;
					
						bmez->Update();
					//	bmez->Requery("", 1, 0);
						
						// 続いて編集
						if( bmez->Requery( filter ) != 0 ) {
							ASSERT( FALSE );
							goto ERR_TAG;
						}
						bmez->Edit();

				//		bmez->ezsign.Copy( ez->m_ezsign );
				//		bmez->ezritsu1 = ez->m_ezritsu1;
				//		bmez->ezritsu2 = ez->m_ezritsu2;

						if( ! bmez->eztcd ) {
							bmez->ezname	= pEd->ed_name;
							bmez->ezkana	= pEd->ed_kana;
						}
						// 残高は ゼロ登録
				//		l_print( bf, pEd->ed_zan, FMT12Z );
				//		bmez->ezzan	= bf;
						bmez->Update();

				/*		int max = sizeof ezrec->lhs / sizeof ezrec->lhs[0];
						for( int i = 0; i < max; i++ ) {
							ezrec->lhs[i] = ez->m_lhs[i];
							ezrec->rhs[i] = ez->m_rhs[i];
						} */
					}
				}
			}
			catch( CDBException* db ) {
				goto ERR_TAG;
			}
			catch( CMemoryException* me ) {
				goto ERR_TAG;
			}
#ifdef OLD_CLOSE
		}
#endif
	}
	else {
		return IDCANCEL;
	}

	return 0;

ERR_TAG:
	return -1;
}



/////////////////////////////////////////////////////////////
// 摘要登録ダイアログ

static	TKY_REGPAR	gsTRP = {0};
static	TKY_VER2	gsTV2 = {0};

TKY_REGPAR* get_tkyregpar()
{
	return &gsTRP;
}

TKY_VER2* get_tkyver2()
{
	return &gsTV2;
}

// 何回も Requery をするのは 無駄なので、フラグで区別する
// 摘要を探す場合は、以下の関数を使用する
static BOOL bTkQuery = TRUE;

BOOL& tkquery_flag()
{
	return bTkQuery;
}


void tkdb_init( CDBZmSub* pZm )
{
	if( tkquery_flag() ) {
		pZm->tkrec->Requery("", 0);
		tkquery_flag() = FALSE;
	}
}

//-------------------------------------------------------------
//	CString& Kbnbf -------	区分格納
//	unsigned char TKszk		区分サイン
//	CDBZmSub* pZm			財務クラス
//	CDBSyohi* pSy			消費税クラス
//-------------------------------------------------------------
void skbn_msg( CString& Kbnbf, unsigned char TKszk, CDBZmSub* pZm, CDBSyohi* pSy )
{
//	CDBSyohi sy;
//	sy.SyOpen( pZm );

	switch( TKszk ) {
		case	1:		Kbnbf.Format( "%-10.10s", "課税　　　" );
						break;
		case	2:		Kbnbf.Format( "%-10.10s", "非課税　　" );
						break;
		case	3:		Kbnbf.Format( "%-10.10s", "不課税　　" );
						break;
		case	4:		if( pSy->Is5perMaster() >= 0 ) 
							Kbnbf.Format( "%-10.10s", "経過－３％" );
						else
							Kbnbf.Format( "%-10.10s", "自動車税率" );
						break;

		case	5:		Kbnbf.Format( "%-10.10s", "経過－５％"  );
						break;

		case	6:		Kbnbf.Format( "%-10.10s", "経過－８％" );
			break;

		case	7:		Kbnbf.Format( "%-10.10s", "軽減－８％" );
			break;

		case	8:		Kbnbf.Format("%-10.10s", "標準－10％");
			break;

		default	:
					Kbnbf.Empty();
					break;

	}
//	CDBZmSub の　szvol がNULLになるので
//	sy.SyClose();
}


int tkyngetr( int tkcode, CDBZmSub* pZm )
{
	if( tkquery_flag() ) {
		pZm->tkrec->Requery("", 0);
		tkquery_flag() = FALSE;
	}

	int ret = -1;

	if( pZm->tkrec->Find( tkcode ) == 1 ) ret = 0;
/*	pZm->tkrec->MoveFirst();
	if( pZm->tkrec->st == 0 ) {

		do {
			if( pZm->tkrec->tkcod == tkcode ) {
				ret = 0;
				break;
			}
		} while( pZm->tkrec->MoveNext() == 0 );
	}*/
//	MyTrace("tkyngetr tkcode = %d, ret = %d\n", tkcode, ret );

	return ret;
}


int tkykana_search( char* kana, CDBZmSub* pZm )
{
	int ret = -1;

	bTkQuery = TRUE;

	CString filter;
	filter.Format( "tkana LIKE '%s%%' COLLATE Japanese_BIN", kana );

	if( pZm->tkrec->Requery( filter, 0 ) == 0 )
		ret = 0;

	return ret;
}

// 現在摘要数を取得
int get_tknum( CDBZmSub* pZm ) 
{
	int tkcnt = 0;

	int cnt = 0;

	CString filter;
	filter = _T( "seq = 7" );	//tkrec
	pZm->dcntl->Requery(filter);
	if( pZm->dcntl->st != -1 ) {
		cnt = pZm->dcntl->cnt;
	}

	return cnt;
}

#ifndef _SLIP_ACCOUNTS_
// 選択ダイアログ
static
int sel_distribute( struct _JOBSEL_PAC *sel, int nChar )
{
	CForksel seldlg;
	int selno;

	//イニシャライズ&表示
	seldlg.OnInitial( sel, nChar );
	if( seldlg.DoModal() == IDOK )
	{
		selno = seldlg.m_radio;
	}
	else
		selno = -1;

	if( selno != -1 )
		sel->set_selno( selno );	//次回デフォルト

	return selno;

}
#else
// 選択ダイアログ
static
int sel_distribute( struct _JOBSEL_PAC *sel, int nChar, CWnd *pWnd )
{
	CForksel seldlg( pWnd );
	int selno;

	//イニシャライズ&表示
	seldlg.OnInitial( sel, nChar );
	if( seldlg.DoModal() == IDOK )
	{
		selno = seldlg.m_radio;
	}
	else
		selno = -1;

	if( selno != -1 )
		sel->set_selno( selno );	//次回デフォルト

	return selno;

}
#endif

//
// 摘要辞書・科目対応摘要登録　振り分け
static
int tky_sel( TKY_REGPAR* pTkpar, CDBZmSub* pZm, CWnd* pwnd )
{
int i,max;
char ktxt[30];
char temp[128];
int tkpage1, tkpage2;
int selno;
struct _JOBSEL_PAC TKYPageSEL;

	TKYPageSEL.set_title( "摘要関係登録" );
	TKYPageSEL.set_itmmsg( "摘要辞書・科目／仕訳対応摘要の登録を行います。");

	TKYPageSEL.set_selno( 0 );	//デフォルト

	// 09.03 /03
	TKYPageSEL.set_gdmsg("＊新規に摘要を登録する場合、『摘要辞書登録』を実行して下さい。");

	TKYPageSEL.set_selmax( max = 4 );
	tkpage1 = tkpage2 = 0;

	for( i = 0 ; i < max ; ++i )
	{
		if( i == 0 || i == 3 )
		{
			;	// 摘要辞書 & 仕訳対応摘要
		}
		else
		{
			CString pjiscode;
			pjiscode = i == 1 ? pTkpar->tr_debt : pTkpar->tr_cred;

			struct _DBKNREC* pKn;
			pKn = DB_PjisToKnrec( pjiscode, pZm, TRUE );
			if( pKn ) {
				sprintf_s( ktxt, sizeof ktxt, "%.14s", pKn->knnam ); 	
				if( i == 1 )	tkpage1 = pKn->knrtb;
				else			tkpage2 = pKn->knrtb;
			}
			else {
				sprintf_s( ktxt, sizeof ktxt, "%.14s", _T( "--------------" ) );
			}
		}

		switch( i )
		{
		case 0:
			TKYPageSEL.set_selmsg( i, "摘要辞書登録" );
			TKYPageSEL.set_selguide( i, "摘要欄から摘要辞書に登録します。" );
			if( pTkpar->tr_pos != -1 )
			{
				TKYPageSEL.set_selattr( i, TRUE );
			}
			else
			{
				TKYPageSEL.set_selattr( i, FALSE );
			}
			break;

		case 1:
			TKYPageSEL.set_selmsg( i, "「借方」科目対応摘要登録" );
			sprintf_s( temp, sizeof temp, "「%s」に対応した摘要を登録します。", ktxt );
			TKYPageSEL.set_selguide( i, temp );
			if( tkpage1 )
			{
				TKYPageSEL.set_selattr( i, TRUE );
			}
			else
			{
				TKYPageSEL.set_selattr( i, FALSE );
			}
			break;

		case 2:
			TKYPageSEL.set_selmsg( i, "「貸方」科目対応摘要登録" );
			sprintf_s( temp, sizeof temp, "「%s」に対応した摘要を登録します。", ktxt );
			TKYPageSEL.set_selguide( i, temp );
			if( tkpage2 )
			{
				TKYPageSEL.set_selattr( i, TRUE );
			}
			else
			{
				TKYPageSEL.set_selattr( i, FALSE );
			}
			break;

		case 3:
			TKYPageSEL.set_selmsg( i, "仕訳対応摘要登録" );
			TKYPageSEL.set_selguide( i, "仕訳対応摘要を登録します。" );
			if( 1 /*GET_MASTERTYPE() != DRIVE_FD*/ )
			{
				TKYPageSEL.set_selattr( i, TRUE );
			}
			else
			{
				TKYPageSEL.set_selattr( i, FALSE );
			}

			break;
		}
	}

	//FORK先選択
#ifndef _SLIP_ACCOUNTS_
	selno = sel_distribute( &TKYPageSEL, VK_F7);
#else
	selno = sel_distribute( &TKYPageSEL, VK_F7, pwnd );
#endif

	return selno;
}


//---------------------------------------------------------------------------
//	入力中の摘要登録ダイアログを表示
//		返送値		0: データベース更新なし	
//					1: データベースを更新した。
//				   -1: Error
//---------------------------------------------------------------------------
DBSUB_FUNC 
int WINAPI TekiyoTorokDialog( TKY_REGPAR* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query/* = NULL*/ )
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

//MyTrace( "@TekiyoTorokDialog TOP\n" );

	tkquery_flag() = TRUE;

	memcpy( &gsTRP, pTkpar, sizeof TKY_REGPAR );

	// 処理選択
	if( gsTRP.tr_jobtyp == TP_SELECT ) {
		int sel;
		if( (sel = tky_sel( pTkpar, pZm, pwnd )) == -1 ) {
			AfxSetResourceHandle( h );
			return IDCANCEL;
		}

		switch( sel ) {
		case 0 :	gsTRP.tr_jobtyp = TP_DIC;		break;
		case 3 :	gsTRP.tr_jobtyp = TP_SIWAKE;	break;
		default:
			gsTRP.tr_jobtyp = TP_KAMOK;
			gsTRP.tr_dcsw = (sel == 1) ? 0 : 1;
		}
	}

	TKYV2_SAVE	tksv = {0};
	clear_TKYV2_SAVE(&tksv);

	int svflg = 0;
	int st;

	while(1) {
		CTkregDlg dlg(pwnd);
		dlg.InitDlg( pZm, pSy );

		if( svflg ) {
			memcpy( dlg.m_kanabf, tksv.kana, sizeof dlg.m_kanabf );
			dlg.m_svTky = tksv.tky;
			memcpy( dlg.m_svInvno, tksv.sv_invno, sizeof dlg.m_svInvno );
			dlg.m_svMenchk = tksv.sv_menchk;
			svflg = 0;
		}
		else {
			dlg.m_svTky = tksv.sv_seltky;
			memcpy(dlg.m_svInvno, gsTRP.tr_invno, sizeof dlg.m_svInvno);
			dlg.m_svMenchk = gsTRP.tr_menchk;
		}

		st = dlg.DoModal();

		if( st == IDOK && dlg.IsKaniJob() ) {

			CTkyVer2Dlg	v2dlg( pwnd );
			v2dlg.InitDlg( pZm, pSy );

			if( svflg ) {
				v2dlg.SetSaveTkyData( &tksv );
				svflg = 0;
			}
			else {
				memcpy(tksv.tky, dlg.m_Save.sv_tky, sizeof tksv.sv_tky);
				memcpy(tksv.kana, dlg.m_Save.sv_kana, sizeof tksv.sv_kana);
				memcpy(tksv.sv_tky, dlg.m_Save.sv_tky, sizeof tksv.sv_tky);
				memcpy(tksv.sv_kana, dlg.m_Save.sv_kana, sizeof tksv.sv_kana);
				memcpy(tksv.sv_seltky, dlg.m_Save.sv_seltky, sizeof tksv.sv_seltky);
				memcpy(tksv.sv_invno, dlg.m_Save.sv_invno, sizeof tksv.sv_invno);
				tksv.sv_menchk = dlg.m_Save.sv_menchk;

				v2dlg.SetSaveTkyData(&tksv);
			}

			st = v2dlg.DoModal();
			if( st == IDOK ) {
				if( v2dlg.IsRegJob() ) {
					v2dlg.GetTkyData( &tksv );
					svflg = 1;
					continue;
				}

				v2dlg.GetRegEdaban(gsTV2.t2_brndeb, gsTV2.t2_brncre);

				gsTV2.t2_brnsgn = 0;
				if( gsTV2.t2_brndeb >= 0 ) {
					gsTV2.t2_brnsgn |= 0x01;
				}
				if( gsTV2.t2_brncre >= 0 ) {
					gsTV2.t2_brnsgn |= 0x02;
				}
			}
			else {
				gsTV2.t2_brnsgn = 0;
				gsTV2.t2_brndeb = -1;
				gsTV2.t2_brncre = -1;
			}
		}
		else {
			if( st != -1 ) {
				if( dlg.IsDataBaseUpdate() )
						st = 1;
				else	st = 0;
			}
		}

		if( teki_query != NULL )
			*teki_query	= TRUE;
		tkquery_flag() = TRUE;

		break;
	}

AfxSetResourceHandle( h );

	return st;
}

/////////////////////////////////////////////////////////////////////////////
// 摘要 キー入力が少ない登録　08.21 /08
//

DBSUB_FUNC 
int WINAPI TekiyoRegV2Dialog( TKY_VER2* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query/* = NULL*/ )
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

//MyTrace( "@TekiyoRegV2Dialog  TOP\n" );

	tkquery_flag() = TRUE;
	memcpy( &gsTV2, pTkpar, sizeof TKY_VER2 );

	TKY_REGPAR	trp = {0};
	memcpy( trp.tr_debt, pTkpar->t2_debt, sizeof pTkpar->t2_debt );
	memcpy( trp.tr_cred, pTkpar->t2_cred, sizeof pTkpar->t2_cred );
	memcpy( trp.tr_tky,  pTkpar->t2_tky,  sizeof pTkpar->t2_tky );
	memcpy( trp.tr_invno, pTkpar->t2_invno, sizeof pTkpar->t2_invno);
	trp.tr_menchk = pTkpar->t2_menchk;
	trp.tr_pos	=	pTkpar->t2_pos;
	memcpy( &gsTRP, &trp, sizeof TKY_REGPAR );

	int st = 0;

	TKYV2_SAVE	tksv = {0};
	clear_TKYV2_SAVE(&tksv);
	int svflg = 0;

	while(1) {
		CTkyVer2Dlg	dlg( pwnd );
		dlg.InitDlg( pZm, pSy );

		if( svflg ) {
			dlg.SetSaveTkyData( &tksv );
			svflg = 0;
		}

		st = dlg.DoModal();

		if( st == IDOK && dlg.IsRegJob() ) {

			dlg.GetTkyData( &tksv );

			// 詳細設定へ
			tkquery_flag() = TRUE;

			CTkregDlg regdlg(pwnd);
			regdlg.InitDlg( pZm, pSy );

			memcpy( regdlg.m_kanabf, tksv.kana, sizeof regdlg.m_kanabf );
			regdlg.m_svTky = tksv.tky;
			memcpy(regdlg.m_svInvno, tksv.sv_invno, sizeof regdlg.m_svInvno);
			regdlg.m_svMenchk = tksv.sv_menchk;

			st = regdlg.DoModal();

			if( st != -1 ) {
				if( regdlg.IsDataBaseUpdate() )
						st = 1;
				else	st = 0;
			}

			//簡易入力へ
			if( regdlg.IsKaniJob() ) {
				svflg = 1;
				memcpy(tksv.tky, regdlg.m_Save.sv_tky, sizeof tksv.sv_tky);
				memcpy(tksv.kana, regdlg.m_Save.sv_kana, sizeof tksv.sv_kana);

			//	memcpy(tksv.sv_tky, regdlg.m_Save.sv_tky, sizeof tksv.sv_tky);
				memcpy(tksv.sv_kana, regdlg.m_Save.sv_kana, sizeof tksv.sv_kana);
			//	memcpy(tksv.sv_seltky, regdlg.m_Save.sv_seltky, sizeof tksv.sv_seltky);
				memcpy(tksv.sv_invno, regdlg.m_Save.sv_invno, sizeof tksv.sv_invno);
				tksv.sv_menchk = regdlg.m_Save.sv_menchk;
				continue;
			}

			if( teki_query != NULL )
				*teki_query	= TRUE;
			tkquery_flag() = TRUE;
		}
		else {
			if( st != -1 ) {
				if( dlg.IsDataBaseUpdate() )
						st = 1;
				else	st = 0;
			}

			if( teki_query != NULL )
				*teki_query	= TRUE;
			tkquery_flag() = TRUE;

		}
		break;
	}

AfxSetResourceHandle( h );


	return st;
}

// 06.30 /11
// 摘要登録タイプを取得する。
int GetTkyRegType()
{
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;
	int tkyreg = -1;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl( utbl );
		CoptionRec*	pRec;
		pRec = mctrl.get_option( utbl.user_name );

//MyTrace( "@GetTkyRegType utbl.user_name %s\n", utbl.user_name );

		if( pRec != NULL ) {
			char* key = "DBDINP:TkyReg;";
			int len = strlen( key );

			int n = pRec->optionchar.Find( key );

			if( n >= 0 ) {
				char tmp[8] = {0};

				int k = 0;
				for( int i = n + len; ; i++ ) {
					if( pRec->optionchar[i] == '*' || pRec->optionchar[i] == '\0' ) {
						break;
					}
					else {
						tmp[k] = pRec->optionchar[i];
						if( ++k >= sizeof tmp ) {
							break;
						}
					}
				}

				tkyreg = atoi( tmp );
			}
		}
	}

	if( tkyreg == -1 ) {
		// レジストリより登録タイプ取得
		HKEY hKey;
		CString section;
		section.Format( "Software\\日本ICS株式会社\\%s\\DBAPP\\TkyReg", GET_LOGINNAME() );
	//	section = "Control Panel\\Mouse";
		int st = RegOpenKeyEx( HKEY_CURRENT_USER, section, 0, KEY_QUERY_VALUE, &hKey );
		DWORD regval, regtype, dwSize;
		regval = 0;
		if( st == ERROR_SUCCESS ) {
			regtype = REG_DWORD;
			dwSize = sizeof regval;
			st = RegQueryValueEx( hKey, "type", NULL, &regtype, (LPBYTE)&regval , &dwSize );
		}
	}

	return tkyreg;
}

//
//	ユーザの設定に対応した摘要登録ダイアログ表示
//
DBSUB_FUNC 
int WINAPI TekiyoTorokDialogEX( TKY_REGPAREX* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query/* = NULL*/ )
{
#ifdef OLD_CLOSE // 06.30 /11
	// レジストリより登録タイプ取得
	HKEY hKey;
	CString section;
	section.Format( "Software\\日本ICS株式会社\\%s\\DBAPP\\TkyReg", GET_LOGINNAME() );
//	section = "Control Panel\\Mouse";
	int st = RegOpenKeyEx( HKEY_CURRENT_USER, section, 0, KEY_QUERY_VALUE, &hKey );
	DWORD regval, regtype, dwSize;
	regval = 0;
	if( st == ERROR_SUCCESS ) {
		regtype = REG_DWORD;
		dwSize = sizeof regval;
		st = RegQueryValueEx( hKey, "type", NULL, &regtype, (LPBYTE)&regval , &dwSize );
	}
#endif
	int st;
	DWORD regval = GetTkyRegType();

	TKY_REGPAR	tp1 = {0};
	TKY_VER2	tv2 = {0};

	memcpy( tp1.tr_debt, pTkpar->tex_debt, sizeof pTkpar->tex_debt );
	memcpy( tp1.tr_cred, pTkpar->tex_cred, sizeof pTkpar->tex_cred );
	memcpy( tp1.tr_tky,  pTkpar->tex_tky,  sizeof pTkpar->tex_tky );
	memcpy( tp1.tr_invno, pTkpar->tex_invno, sizeof pTkpar->tex_invno);
	tp1.tr_menchk	= pTkpar->tex_menchk;
	tp1.tr_pos		=	pTkpar->tex_pos;
	tp1.tr_jobtyp	=	TP_SELECT;	// 必ず選択を表示する

	memcpy( tv2.t2_debt, pTkpar->tex_debt, sizeof pTkpar->tex_debt );
	memcpy( tv2.t2_cred, pTkpar->tex_cred, sizeof pTkpar->tex_cred );
	memcpy( tv2.t2_tky,  pTkpar->tex_tky,  sizeof pTkpar->tex_tky );
	memcpy( tv2.t2_invno, pTkpar->tex_invno, sizeof pTkpar->tex_invno);
	tv2.t2_menchk = pTkpar->tex_menchk;
	tv2.t2_pos	=	pTkpar->tex_pos;
	tv2.t2_sel	=	pTkpar->tex_sel;

	// 選択用に保存 [11.01 /11]
	CTkySel::m_orgTky = pTkpar->tex_tky;
	CTkySel::m_orgPos = pTkpar->tex_pos;

	if( regval != 1) {
		st = TekiyoRegV2Dialog( &tv2, pZm, pSy, pwnd, teki_query );
	}
	else {
		memcpy( &gsTV2, &tv2, sizeof TKY_VER2 );
		st = TekiyoTorokDialog( &tp1, pZm, pSy, pwnd, teki_query );
	}

	return st;
}



// 枝番対応の簡易摘要登録
int WINAPI TkyEdaRegDialog( TKY_VER2* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query/* = NULL*/ )
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );
	
	tkquery_flag() = TRUE;
	memcpy( &gsTV2, pTkpar, sizeof TKY_VER2 );

	TKY_REGPAR	trp = {0};
	memcpy( trp.tr_debt, pTkpar->t2_debt, sizeof pTkpar->t2_debt );
	memcpy( trp.tr_cred, pTkpar->t2_cred, sizeof pTkpar->t2_cred );
	memcpy( trp.tr_tky,  pTkpar->t2_tky,  sizeof pTkpar->t2_tky );
	memcpy( trp.tr_invno, pTkpar->t2_invno, sizeof pTkpar->t2_invno);
	trp.tr_pos	=	pTkpar->t2_pos;
	memcpy(trp.tr_invno, pTkpar->t2_invno, sizeof pTkpar->t2_invno);
	memcpy( &gsTRP, &trp, sizeof TKY_REGPAR );

	int st;

	TKYV2_SAVE	tksv;
	clear_TKYV2_SAVE(&tksv);
	int svflg = 0;

	while(1) {
		CTkyVer2Dlg	dlg( pwnd );
		dlg.InitDlg( pZm, pSy );
		dlg.m_bEdaReg = TRUE;

		if( svflg ) {
			dlg.SetSaveTkyData( &tksv );
			svflg = 0;
		}

		st = dlg.DoModal();

		if( st == IDOK && dlg.IsRegJob() ) {

			dlg.GetTkyData( &tksv );

			// 詳細設定へ
			tkquery_flag() = TRUE;

			CTkregDlg regdlg(pwnd);
			regdlg.InitDlg( pZm, pSy );

			memcpy( regdlg.m_kanabf, dlg.m_kanabf, sizeof regdlg.m_kanabf );
			regdlg.m_svTky = tksv.tky;
			memcpy(regdlg.m_svInvno, tksv.sv_invno, sizeof regdlg.m_svInvno);
			regdlg.m_svMenchk = tksv.sv_menchk;

			st = regdlg.DoModal();

			if( st != -1 ) {
				if( regdlg.IsDataBaseUpdate() )
						st = 1;
				else	st = 0;
			}

			//簡易入力へ
			if( regdlg.IsKaniJob() ) {
				memcpy(tksv.tky, regdlg.m_Save.sv_tky, sizeof tksv.sv_tky);
				memcpy(tksv.kana, regdlg.m_Save.sv_kana, sizeof tksv.sv_kana);

			//	memcpy(tksv.sv_tky, regdlg.m_Save.sv_tky, sizeof tksv.sv_tky);
				memcpy(tksv.sv_kana, regdlg.m_Save.sv_kana, sizeof tksv.sv_kana);
			//	memcpy(tksv.sv_seltky, regdlg.m_Save.sv_seltky, sizeof tksv.sv_seltky);
				memcpy(tksv.sv_invno, regdlg.m_Save.sv_invno, sizeof tksv.sv_invno);
				tksv.sv_menchk = regdlg.m_Save.sv_menchk;
				svflg = 1;
				continue;
			}

			if( teki_query != NULL )
				*teki_query	= TRUE;
			tkquery_flag() = TRUE;

		}
		else {
			int dlg_st = st;

			if( st != -1 ) {
				if( dlg.IsDataBaseUpdate() )
						st = 1;
				else	st = 0;
			}

			pTkpar->t2_brnsgn = 0;
			pTkpar->t2_brndeb = -1;
			pTkpar->t2_brncre = -1;

			if( dlg_st == IDOK ) {
				dlg.GetRegEdaban(pTkpar->t2_brndeb, pTkpar->t2_brncre);

				pTkpar->t2_brnsgn = 0;
				if( pTkpar->t2_brndeb >= 0 ) {
					pTkpar->t2_brnsgn |= 0x01;
				}
				if( pTkpar->t2_brncre >= 0 ) {
					pTkpar->t2_brnsgn |= 0x02;
				}
			}

			if( teki_query != NULL )
				*teki_query	= TRUE;
			tkquery_flag() = TRUE;
			
		}
		break;
	}

AfxSetResourceHandle( h );


	return st;
}


//
//	ユーザの設定に対応した摘要登録ダイアログ表示
//
DBSUB_FUNC 
int WINAPI TkyEdaTorokDialog( TKY_REGPAREX* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query/* = NULL*/ )
{
#ifdef OLD_CLOSE // 06.30 /11
	// レジストリより登録タイプ取得
	HKEY hKey;
	CString section;
	section.Format( "Software\\日本ICS株式会社\\%s\\DBAPP\\TkyReg", GET_LOGINNAME() );
//	section = "Control Panel\\Mouse";
	int st = RegOpenKeyEx( HKEY_CURRENT_USER, section, 0, KEY_QUERY_VALUE, &hKey );
	DWORD regval, regtype, dwSize;
	regval = 0;
	if( st == ERROR_SUCCESS ) {
		regtype = REG_DWORD;
		dwSize = sizeof regval;
		st = RegQueryValueEx( hKey, "type", NULL, &regtype, (LPBYTE)&regval , &dwSize );
	}
#endif
	int st;
	// 06.30 /11
	DWORD regval = GetTkyRegType();

	TKY_REGPAR	tp1 = {0};
	TKY_VER2	tv2 = {0};

	memcpy( tp1.tr_debt, pTkpar->tex_debt, sizeof pTkpar->tex_debt );
	memcpy( tp1.tr_cred, pTkpar->tex_cred, sizeof pTkpar->tex_cred );
	memcpy( tp1.tr_tky,  pTkpar->tex_tky,  sizeof pTkpar->tex_tky );
	memcpy( tp1.tr_invno, pTkpar->tex_invno, sizeof pTkpar->tex_invno);
	tp1.tr_menchk = pTkpar->tex_menchk;
	tp1.tr_pos		=	pTkpar->tex_pos;
	tp1.tr_jobtyp	=	TP_SELECT;	// 必ず選択を表示する

	memcpy( tv2.t2_debt, pTkpar->tex_debt, sizeof pTkpar->tex_debt );
	memcpy( tv2.t2_cred, pTkpar->tex_cred, sizeof pTkpar->tex_cred );
	memcpy( tv2.t2_tky,  pTkpar->tex_tky,  sizeof pTkpar->tex_tky );
	memcpy( tv2.t2_invno, pTkpar->tex_invno, sizeof pTkpar->tex_invno);
	tv2.t2_menchk = pTkpar->tex_menchk;
	tv2.t2_pos	=	pTkpar->tex_pos;
	tv2.t2_sel	=	pTkpar->tex_sel;

	// 選択用に保存 [11.01 /11]
	CTkySel::m_orgTky = pTkpar->tex_tky;
	CTkySel::m_orgPos = pTkpar->tex_pos;

	if( ! regval ) {
		st = TekiyoRegV2Dialog( &tv2, pZm, pSy, pwnd, teki_query );

		pTkpar->tex_brnsgn = 0;
		pTkpar->tex_brndeb = -1;
		pTkpar->tex_brncre = -1;
	}
	else if( regval == 2 ) {
		st = TkyEdaRegDialog( &tv2, pZm, pSy, pwnd, teki_query );

		pTkpar->tex_brnsgn = tv2.t2_brnsgn;
		pTkpar->tex_brndeb = tv2.t2_brndeb;
		pTkpar->tex_brncre = tv2.t2_brncre;
	}
	else {
		memcpy( &gsTV2, &tv2, sizeof TKY_VER2 );
		st = TekiyoTorokDialog( &tp1, pZm, pSy, pwnd, teki_query );

		pTkpar->tex_brnsgn = gsTV2.t2_brnsgn;
		pTkpar->tex_brndeb = gsTV2.t2_brndeb;
		pTkpar->tex_brncre = gsTV2.t2_brncre;
	}

	return st;
}



/////////////////////////////////////////////////////////////////////////////
// 検索ダイアログ
DBSUB_FUNC 
int WINAPI ScanItemDialog( SCAN_ITEM* pSI, DWORD nFlag, CDBZmSub* pZm, CWnd* pwnd )
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

	CScanKey dlg( pwnd );
	dlg.InitWork( pSI, nFlag, pZm );

	int st = dlg.DoModal();

	if( st == IDOK ) {
		struct _SKEY_LINE* ps;
		ps = &dlg.SCAN_KEY;

		// 表示順
		switch( ps->SCN_ORDER_DT.SKBN_SGN1 ) {
		case 0:		pSI->sc_type = SCT_ORDERSEQ;	break;
		default:	pSI->sc_type = SCT_ORDERDATE;	break;
		}
		if( dlg.m_bTKYONLY )
			pSI->sc_type |= SCT_TKYONLY;
		else
			pSI->sc_type &= ~SCT_TKYONLY;

		// 対象データ
		switch( ps->SCN_DUSER_DT.SKBN_SGN1 ) {
		case 0:		pSI->sc_duser = SCD_ALLDATA;	break;
		default:	pSI->sc_duser = SCD_CURUESR;	break;
		}

		// 対象データ
		switch( ps->SCN_STYPE_DT.SKBN_SGN1 ) {
		case 0:		pSI->sc_stype = SCS_AND;	break;
		default:	pSI->sc_stype = SCS_OR;		break;
		}

		// SEQ番号
		if( ps->LINE_CND[ SCN_SEQ1_PN ].INP_sg )
			pSI->sc_seq1 = ps->SCN_SEQ1_DT.IP_SEQ;
		else
			pSI->sc_seq1 = -1;
		if( ps->LINE_CND[ SCN_SEQ2_PN ].INP_sg )
			pSI->sc_seq2 = ps->SCN_SEQ2_DT.IP_SEQ;
		else
			pSI->sc_seq2 = -1;

		pSI->sc_seqopt = 0;
		if( ps->LINE_CND[ SCN_SEQ1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_seqopt |= SC_NOTSEARCH;
		}
		// 日付
		if( ps->LINE_CND[ SCN_DATE1_PN ].INP_sg )
			memcpy( pSI->sc_date1, ps->SCN_DATE1_DT.IP_BCDDAY, 2 );
		else
			::ZeroMemory( pSI->sc_date1, sizeof pSI->sc_date1 );
		if( ps->LINE_CND[ SCN_DATE2_PN ].INP_sg )
			memcpy( pSI->sc_date2, ps->SCN_DATE2_DT.IP_BCDDAY, 2 );
		else
			::ZeroMemory( pSI->sc_date2, sizeof pSI->sc_date2 );
		pSI->sc_dateopt = 0;
		if( ps->LINE_CND[ SCN_DATE1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dateopt |= SC_NOTSEARCH;
		}

		// 伝票番号
		if( ps->LINE_CND[ SCN_DENP1_PN ].INP_sg )
			pSI->sc_denp1 = ps->SCN_DENP1_DT.IP_DENP;
		else
			pSI->sc_denp1 = -1;
		if( ps->LINE_CND[ SCN_DENP2_PN ].INP_sg )
			pSI->sc_denp2 = ps->SCN_DENP2_DT.IP_DENP;
		else
			pSI->sc_denp2 = -1;
		pSI->sc_denpopt = 0;
		if( ps->LINE_CND[ SCN_DENP1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_denpopt |= SC_NOTSEARCH;
		}
		//借方部門
		if( ps->LINE_CND[ SCN_BMON1_PN ].INP_sg )
			pSI->sc_dbmn1 = ps->SCN_BMON1_DT.IP_BMN;
		else
			pSI->sc_dbmn1 = -1;
		if( ps->LINE_CND[ SCN_BMON2_PN ].INP_sg )
			pSI->sc_dbmn2 = ps->SCN_BMON2_DT.IP_BMN;
		else
			pSI->sc_dbmn2 = -1;
		pSI->sc_dbmnopt = 0;
		if( ps->LINE_CND[ SCN_BMON1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dbmnopt |= SC_NOTSEARCH;
		}

		//貸方部門
		if( ps->LINE_CND[ SCN_CBMN1_PN ].INP_sg )
			pSI->sc_cbmn1 = ps->SCN_CBMN1_DT.IP_BMN;
		else
			pSI->sc_cbmn1 = -1;
		if( ps->LINE_CND[ SCN_CBMN2_PN ].INP_sg )
			pSI->sc_cbmn2 = ps->SCN_CBMN2_DT.IP_BMN;
		else
			pSI->sc_cbmn2 = -1;
		pSI->sc_cbmnopt = 0;
		if( ps->LINE_CND[ SCN_CBMN1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_cbmnopt |= SC_NOTSEARCH;
		}

		// 借方工事
		if( ps->LINE_CND[ SCN_KOJI1_PN ].INP_sg )
			memcpy( pSI->sc_dkji1, ps->SCN_KOJI1_DT.IP_KOJI, sizeof pSI->sc_dkji1  );
		else
			::ZeroMemory( pSI->sc_dkji1, sizeof pSI->sc_dkji1 );
		if( ps->LINE_CND[ SCN_KOJI2_PN ].INP_sg )
			memcpy( pSI->sc_dkji2, ps->SCN_KOJI2_DT.IP_KOJI, sizeof pSI->sc_dkji2  );
		else
			::ZeroMemory( pSI->sc_dkji2, sizeof pSI->sc_dkji2 );
		pSI->sc_dkjiopt = 0;
		if( ps->LINE_CND[ SCN_KOJI1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dkjiopt |= SC_NOTSEARCH;
		}

		// 貸方工事
		if( ps->LINE_CND[ SCN_CKJI1_PN ].INP_sg )
			memcpy( pSI->sc_ckji1, ps->SCN_CKJI1_DT.IP_KOJI, sizeof pSI->sc_ckji1 );
		else
			::ZeroMemory( pSI->sc_ckji1, sizeof pSI->sc_ckji1 );
		if( ps->LINE_CND[ SCN_CKJI2_PN ].INP_sg )
			memcpy( pSI->sc_ckji2, ps->SCN_CKJI2_DT.IP_KOJI, sizeof pSI->sc_ckji2 );
		else
			::ZeroMemory( pSI->sc_ckji2, sizeof pSI->sc_ckji2 );
		pSI->sc_ckjiopt = 0;
		if( ps->LINE_CND[ SCN_CKJI1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_ckjiopt |= SC_NOTSEARCH;
		}

		// 借方
		if( ps->LINE_CND[ SCN_DEBT_PN ].INP_sg )
			memcpy( pSI->sc_dbt, ps->SCN_DEBT_DT.IP_CdNum, sizeof pSI->sc_dbt );
		else
			::ZeroMemory( pSI->sc_dbt, sizeof pSI->sc_dbt );
		pSI->sc_dbtopt = 0;
		if( ps->LINE_CND[ SCN_DEBT_PN ].KEY_inp & SC_ITEMNONE ) {
			pSI->sc_dbtopt |= SC_ITEMNONE;
		}
		if( ps->LINE_CND[ SCN_DEBT_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dbtopt |= SC_NOTSEARCH;
		}

		// 借方枝番
		if( ps->LINE_CND[ SCN_DBR1_PN ].INP_sg )
			pSI->sc_dbr1 = ps->SCN_DBR1_DT.IP_cBRN;
		else
			pSI->sc_dbr1 = -1;

		if( ps->LINE_CND[ SCN_DBR2_PN ].INP_sg )
			pSI->sc_dbr2 = ps->SCN_DBR2_DT.IP_cBRN;
		else
			pSI->sc_dbr2 = -1;
		pSI->sc_dbropt = 0;
		if( ps->LINE_CND[ SCN_DBR1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dbropt |= SC_NOTSEARCH;
		}

		// 貸方
		if( ps->LINE_CND[ SCN_CRED_PN ].INP_sg )
			memcpy( pSI->sc_cre, ps->SCN_CRED_DT.IP_CdNum, sizeof pSI->sc_cre );
		else
			::ZeroMemory( pSI->sc_cre, sizeof pSI->sc_cre );
		pSI->sc_creopt = 0;
		if( ps->LINE_CND[ SCN_CRED_PN ].KEY_inp & SC_ITEMNONE ) {
			pSI->sc_creopt |= SC_ITEMNONE;
		}
		if( ps->LINE_CND[ SCN_DEBT_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_creopt |= SC_NOTSEARCH;
		}

		// 貸方枝番
		if( ps->LINE_CND[ SCN_CBR1_PN ].INP_sg )
			pSI->sc_cbr1 = ps->SCN_CBR1_DT.IP_cBRN;
		else
			pSI->sc_cbr1 = -1;

		if( ps->LINE_CND[ SCN_CBR2_PN ].INP_sg )
			pSI->sc_cbr2 = ps->SCN_CBR2_DT.IP_cBRN;
		else
			pSI->sc_cbr2 = -1;
		pSI->sc_cbropt = 0;
		if( ps->LINE_CND[ SCN_CBR1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_cbropt |= SC_NOTSEARCH;
		}

		// 借／貸
		if( ps->LINE_CND[ SCN_DBCR_PN ].INP_sg )
			memcpy( pSI->sc_dbcr, ps->SCN_DBCR_DT.IP_CdNum, sizeof pSI->sc_dbcr );
		else
			::ZeroMemory( pSI->sc_dbcr, sizeof pSI->sc_dbcr );
		pSI->sc_dbcropt = 0;
		if( ps->LINE_CND[ SCN_DBCR_PN ].KEY_inp & SC_ITEMNONE ) {
			pSI->sc_dbcropt |= SC_ITEMNONE;
		}
		if( ps->LINE_CND[ SCN_DBCR_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dbcropt |= SC_NOTSEARCH;
		}		
		
		// 借／貸枝番
		if( ps->LINE_CND[ SCN_DCBR1_PN ].INP_sg )
			pSI->sc_dcbrn1 = ps->SCN_DCBR1_DT.IP_cBRN;
		else
			pSI->sc_dcbrn1 = -1;

		if( ps->LINE_CND[ SCN_DCBR2_PN ].INP_sg )
			pSI->sc_dcbrn2 = ps->SCN_DCBR2_DT.IP_cBRN;
		else
			pSI->sc_dcbrn2 = -1;
		pSI->sc_dcbropt = 0;
		if( ps->LINE_CND[ SCN_DCBR1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_dbcropt |= SC_NOTSEARCH;
		}		

		// 金額
		if( ps->LINE_CND[ SCN_VAL1_PN ].INP_sg ) {
			memcpy( pSI->sc_val1, ps->SCN_VAL1_DT.IP_VAL, sizeof pSI->sc_val1 );
			pSI->sc_valsgn[0] = 1;
		}
		else {
			::ZeroMemory( pSI->sc_val1, sizeof pSI->sc_val1 );
			pSI->sc_valsgn[0] = 0;
		}

		if( ps->LINE_CND[ SCN_VAL2_PN ].INP_sg ) {
			memcpy( pSI->sc_val2, ps->SCN_VAL2_DT.IP_VAL, sizeof pSI->sc_val2 );
			pSI->sc_valsgn[1] = 1;
		}
		else {
			::ZeroMemory( pSI->sc_val2, sizeof pSI->sc_val2 );
			pSI->sc_valsgn[1] = 0;
		}
		pSI->sc_valopt = 0;
		if( ps->LINE_CND[ SCN_VAL1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_valopt |= SC_NOTSEARCH;
		}				
		// 税額
		if( ps->LINE_CND[ SCN_ZEI1_PN ].INP_sg ) {
			memcpy( pSI->sc_zei1, ps->SCN_ZEI1_DT.IP_VAL, sizeof pSI->sc_zei1 );
			pSI->sc_zeisgn[0] = 1;
		}
		else {
			::ZeroMemory( pSI->sc_zei1, sizeof pSI->sc_zei1 );
			pSI->sc_zeisgn[0] = 0;
		}

		if( ps->LINE_CND[ SCN_ZEI2_PN ].INP_sg ) {
			memcpy( pSI->sc_zei2, ps->SCN_ZEI2_DT.IP_VAL, sizeof pSI->sc_zei2 );
			pSI->sc_zeisgn[1] = 1;
		}
		else {
			::ZeroMemory( pSI->sc_zei2, sizeof pSI->sc_zei2 );
			pSI->sc_zeisgn[1] = 0;
		}
		pSI->sc_zeiopt = 0;
		if( ps->LINE_CND[ SCN_ZEI1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_zeiopt |= SC_NOTSEARCH;
		}				

		// 消費税仕訳区分サイン
		if( ps->LINE_CND[ SCN_SYZSW_PN ].INP_sg )
			pSI->sc_syzsw = ps->SCN_SYZSW_DT.SKBN_SGN1;
		else
			pSI->sc_syzsw = SCZ_NONSELECT;
		pSI->sc_syzswopt = 0;
		if( ps->LINE_CND[ SCN_SYZSW_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_syzswopt |= SC_NOTSEARCH;
		}				

		// 消費税税率サイン
		if( ps->LINE_CND[ SCN_ZRITU_PN ].INP_sg )
			pSI->sc_zritu = ps->SCN_ZRITU_DT.SKBN_SGN1;
		else
			pSI->sc_zritu = SCZ_NONSELECT;
		pSI->sc_zrituopt = 0;
		if( ps->LINE_CND[ SCN_ZRITU_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_zrituopt |= SC_NOTSEARCH;
		}				

		// 消費税区分サイン
		if( ps->LINE_CND[ SCN_SKBN_PN ].INP_sg )
			pSI->sc_skbn = ps->SCN_SKBN_DT.SKBN_SGN1;
		else
			pSI->sc_skbn = SCZ_NONSELECT;
		pSI->sc_skbnopt = 0;
		if( ps->LINE_CND[ SCN_SKBN_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_skbnopt |= SC_NOTSEARCH;
		}				

		// 売上／仕入サイン
		if( ps->LINE_CND[ SCN_URISIRE_PN ].INP_sg )
			pSI->sc_urisire = ps->SCN_URISIRE_DT.SKBN_SGN1;
		else
			pSI->sc_urisire = SCZ_NONSELECT;
		pSI->sc_urisireopt = 0;
		if( ps->LINE_CND[ SCN_URISIRE_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_urisireopt |= SC_NOTSEARCH;
		}

#ifdef CLOSE
		// 特定収入[06.03 /08]
		if( ps->LINE_CND[ SCN_TOKUS_PN ].INP_sg )
			pSI->sc_tokus = ps->SCN_TOKUS_DT.SKBN_SGN1;
		else
			pSI->sc_tokus = SCZ_NONSELECT;
		pSI->sc_tokusopt = 0;
		if( ps->LINE_CND[ SCN_TOKUS_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_tokusopt |= SC_NOTSEARCH;
		}
#endif

		// 摘要
		if( ps->LINE_CND[ SCN_TKY_PN ].INP_sg )
			memcpy( pSI->sc_tky, ps->SCN_TKY_DT.IP_TKY, sizeof pSI->sc_tky );
		else
			::ZeroMemory( pSI->sc_tky, sizeof pSI->sc_tky );

		pSI->sc_tkytyp = ps->LINE_CND[ SCN_TKY_PN ].INP_type;

		// 証憑番号
		if( ps->LINE_CND[ SCN_SNUM1_PN ].INP_sg )
			memcpy( pSI->sc_snum1, ps->SCN_SNUM1_DT.IP_SNUM, sizeof pSI->sc_snum1 );
		else
			::ZeroMemory( pSI->sc_snum1, sizeof pSI->sc_snum1 );

		if( ps->LINE_CND[ SCN_SNUM2_PN ].INP_sg )
			memcpy( pSI->sc_snum2, ps->SCN_SNUM2_DT.IP_SNUM, sizeof pSI->sc_snum2 );
		else
			::ZeroMemory( pSI->sc_snum2, sizeof pSI->sc_snum2 );
		pSI->sc_snumopt = 0;
		if( ps->LINE_CND[ SCN_SNUM1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_snumopt |= SC_NOTSEARCH;
		}

		// 手形期日
		if( ps->LINE_CND[ SCN_TDAT1_PN ].INP_sg )
			pSI->sc_tgdate1 = ps->SCN_SNUM1_DT.IP_TGDAY;
		else
			pSI->sc_tgdate1 = 0;

		if( ps->LINE_CND[ SCN_TDAT2_PN ].INP_sg )
			pSI->sc_tgdate2 = ps->SCN_SNUM2_DT.IP_TGDAY;
		else
			pSI->sc_tgdate2 = 0;
		pSI->sc_tgdtopt = 0;
		if( ps->LINE_CND[ SCN_TDAT1_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_tgdtopt |= SC_NOTSEARCH;
		}

		// 付箋
		if( ps->LINE_CND[ SCN_HUSEN_PN ].INP_sg ) {
			pSI->sc_husen = ps->SCN_HUSEN_DT.SKBN_SGN1;
		}
		else {	
			pSI->sc_husen = SCZ_NONSELECT;
		}
		pSI->sc_husenopt = 0;
		if( ps->LINE_CND[ SCN_HUSEN_PN ].KEY_inp & SC_NOTSEARCH ) {
			pSI->sc_husenopt |= SC_NOTSEARCH;
		}
		// 入力漏れ
		if( ps->LINE_CND[ SCN_NOINP_PN ].INP_sg ) {
				pSI->sc_noinp = ps->SCN_NOINP_DT.SKBN_SGN1;
		}
		else	pSI->sc_noinp = 0;
	}
AfxSetResourceHandle( h);


	return st;
}


DBSUB_FUNC
void WINAPI scanitem_init( SCAN_ITEM* si )
{
//MyTrace( "scanitem_init SCAN_ITEM = (%d)\n", sizeof SCAN_ITEM );

	si->sc_type		= SCT_ORDERSEQ;
	si->sc_duser	= SCD_CURUESR;
	si->sc_stype	= SCS_AND;

	si->sc_seq1 = si->sc_seq2 = -1;
	si->sc_seqopt = 0;

	::ZeroMemory( si->sc_date1, sizeof si->sc_date1 );
	::ZeroMemory( si->sc_date2, sizeof si->sc_date2 );
	si->sc_dateopt = 0;

	si->sc_denp1 = si->sc_denp2 = -1;
	si->sc_denpopt = 0;

	si->sc_dbmn1 = si->sc_dbmn2 = -1;
	si->sc_dbmnopt = 0;

	si->sc_cbmn1 = si->sc_cbmn2 = -1;
	si->sc_cbmnopt = 0;

	::ZeroMemory( si->sc_dkji1, sizeof si->sc_dkji1 );
	::ZeroMemory( si->sc_dkji2, sizeof si->sc_dkji2 );
	si->sc_dkjiopt = 0;

	::ZeroMemory( si->sc_ckji1, sizeof si->sc_ckji1 );
	::ZeroMemory( si->sc_ckji2, sizeof si->sc_ckji2 );
	si->sc_ckjiopt = 0;

	::ZeroMemory( si->sc_dbt, sizeof si->sc_dbt );
	si->sc_dbtopt = 0;
	si->sc_dbr1 = si->sc_dbr2 = -1;
	si->sc_dbropt = 0;

	::ZeroMemory( si->sc_cre, sizeof si->sc_cre );
	si->sc_creopt = 0;
	si->sc_cbr1 = si->sc_cbr2 = -1;
	si->sc_cbropt = 0;

	::ZeroMemory( si->sc_dbcr, sizeof si->sc_dbcr );
	si->sc_dbcropt = 0;
	si->sc_dcbrn1 = si->sc_dcbrn2 = -1;
	si->sc_dcbropt = 0;
	
	::ZeroMemory( si->sc_val1, sizeof si->sc_val1 );
	::ZeroMemory( si->sc_val2, sizeof si->sc_val2 );
	::ZeroMemory( si->sc_valsgn, sizeof si->sc_valsgn );
	si->sc_valopt = 0;

	::ZeroMemory( si->sc_zei1, sizeof si->sc_zei1 );
	::ZeroMemory( si->sc_zei2, sizeof si->sc_zei2 );
	::ZeroMemory( si->sc_zeisgn, sizeof si->sc_zeisgn );
	si->sc_zeiopt = 0;

	si->sc_syzsw	= SCZ_NONSELECT;
	si->sc_zritu	= SCZ_NONSELECT;
	si->sc_skbn		= SCZ_NONSELECT;
	si->sc_urisire	= SCZ_NONSELECT;

	si->sc_syzswopt = si->sc_zrituopt = si->sc_skbnopt = si->sc_urisireopt = 0;

	::ZeroMemory( si->sc_tky, sizeof si->sc_tky );
	si->sc_tkyopt = 0;

	::ZeroMemory( si->sc_snum1, sizeof si->sc_snum1 );
	::ZeroMemory( si->sc_snum2, sizeof si->sc_snum2 );
	si->sc_snumopt = 0;

	si->sc_tgdate1 = si->sc_tgdate2 = 0;
	si->sc_tgdtopt = 0;

	si->sc_husen = 0;
	::ZeroMemory( si->sc_hucmnt, sizeof si->sc_hucmnt );
	si->sc_husenopt = 0;

	si->sc_noinp = 0;
	si->sc_hjiyu = 0;
	// 特定収入[06.03 /08]
	si->sc_tokus	= SCZ_NONSELECT;
	si->sc_tokusopt = 0;

	si->sc_gyokbn = 0;
	::ZeroMemory( si->sc_dbt2, sizeof si->sc_dbt2 );
	::ZeroMemory( si->sc_cre2, sizeof si->sc_cre2 );
	::ZeroMemory( si->sc_dbcr2, sizeof si->sc_dbcr2 );

	// 貸借部門		05.14 /13
	si->sc_dcbmn1 = -1;
	si->sc_dcbmn2 = -1;
	si->sc_dcbmnopt = 0;
	// 貸借工事		05.14 /13
	::ZeroMemory( si->sc_dckji1, sizeof si->sc_dckji1 );
	::ZeroMemory( si->sc_dckji2, sizeof si->sc_dckji2 );
	si->sc_dckjiopt = 0;

}

//
// 付箋コメント拡張版
//
DBSUB_FUNC
void WINAPI scanitem_initEX( SCAN_ITEMEX* si )
{
//MyTrace( "scanitem_initEX SCAN_ITEMEX = (%d)\n", sizeof SCAN_ITEMEX );

	si->sc_type		= SCT_ORDERSEQ;
	si->sc_duser	= SCD_CURUESR;
	si->sc_stype	= SCS_AND;

	si->sc_seq1 = si->sc_seq2 = -1;
	si->sc_seqopt = 0;

	::ZeroMemory( si->sc_date1, sizeof si->sc_date1 );
	::ZeroMemory( si->sc_date2, sizeof si->sc_date2 );
	si->sc_dateopt = 0;

	si->sc_denp1 = si->sc_denp2 = -1;
	si->sc_denpopt = 0;

	si->sc_dbmn1 = si->sc_dbmn2 = -1;
	si->sc_dbmnopt = 0;

	si->sc_cbmn1 = si->sc_cbmn2 = -1;
	si->sc_cbmnopt = 0;

	::ZeroMemory( si->sc_dkji1, sizeof si->sc_dkji1 );
	::ZeroMemory( si->sc_dkji2, sizeof si->sc_dkji2 );
	si->sc_dkjiopt = 0;

	::ZeroMemory( si->sc_ckji1, sizeof si->sc_ckji1 );
	::ZeroMemory( si->sc_ckji2, sizeof si->sc_ckji2 );
	si->sc_ckjiopt = 0;

	::ZeroMemory( si->sc_dbt, sizeof si->sc_dbt );
	si->sc_dbtopt = 0;
	si->sc_dbr1 = si->sc_dbr2 = -1;
	si->sc_dbropt = 0;

	::ZeroMemory( si->sc_cre, sizeof si->sc_cre );
	si->sc_creopt = 0;
	si->sc_cbr1 = si->sc_cbr2 = -1;
	si->sc_cbropt = 0;

	::ZeroMemory( si->sc_dbcr, sizeof si->sc_dbcr );
	si->sc_dbcropt = 0;
	si->sc_dcbrn1 = si->sc_dcbrn2 = -1;
	si->sc_dcbropt = 0;
	
	::ZeroMemory( si->sc_val1, sizeof si->sc_val1 );
	::ZeroMemory( si->sc_val2, sizeof si->sc_val2 );
	::ZeroMemory( si->sc_valsgn, sizeof si->sc_valsgn );
	si->sc_valopt = 0;

	::ZeroMemory( si->sc_zei1, sizeof si->sc_zei1 );
	::ZeroMemory( si->sc_zei2, sizeof si->sc_zei2 );
	::ZeroMemory( si->sc_zeisgn, sizeof si->sc_zeisgn );
	si->sc_zeiopt = 0;

	si->sc_syzsw	= SCZ_NONSELECT;
	si->sc_zritu	= SCZ_NONSELECT;
	si->sc_skbn		= SCZ_NONSELECT;
	si->sc_urisire	= SCZ_NONSELECT;

	si->sc_syzswopt = si->sc_zrituopt = si->sc_skbnopt = si->sc_urisireopt = 0;

	::ZeroMemory( si->sc_tky, sizeof si->sc_tky );
	si->sc_tkyopt = 0;

	::ZeroMemory( si->sc_snum1__, sizeof si->sc_snum1__);
	::ZeroMemory( si->sc_snum2__, sizeof si->sc_snum2__);
	::ZeroMemory(si->sc_rnum1, sizeof si->sc_rnum1);
	::ZeroMemory(si->sc_rnum2, sizeof si->sc_rnum2);
	si->sc_snumopt = 0;

	si->sc_tgdate1 = si->sc_tgdate2 = 0;
	si->sc_tgdtopt = 0;

	si->sc_husen = 0;
	::ZeroMemory( si->sc_hucmnt, sizeof si->sc_hucmnt );
	si->sc_husenopt = 0;

	si->sc_noinp = 0;
	si->sc_hjiyu = 0;
	// 特定収入[06.03 /08]
	si->sc_tokus	= SCZ_NONSELECT;
	si->sc_tokusopt = 0;

	si->sc_gyokbn = 0;
	::ZeroMemory( si->sc_dbt2, sizeof si->sc_dbt2 );
	::ZeroMemory( si->sc_cre2, sizeof si->sc_cre2 );
	::ZeroMemory( si->sc_dbcr2, sizeof si->sc_dbcr2 );

	// 貸借部門		05.14 /13
	si->sc_dcbmn1 = -1;
	si->sc_dcbmn2 = -1;
	si->sc_dcbmnopt = 0;
	// 貸借工事		05.14 /13
	::ZeroMemory( si->sc_dckji1, sizeof si->sc_dckji1 );
	::ZeroMemory( si->sc_dckji2, sizeof si->sc_dckji2 );
	si->sc_dckjiopt = 0;

	// 付箋コメント
	::ZeroMemory( si->sc_hucmntEX, sizeof si->sc_hucmntEX );
}



/*=============================================
	DWORD flg ...... 未使用フラグ
	CString deb .... 借方
	CString cre .... 貸方
	CDBZmSub* pZm .. 財務クラス

  =============================================*/
DBSUB_FUNC
int WINAPI swktky_search( DWORD flg, CString deb, CString cre, CDBZmSub* pZm )
{
	
//MyTrace( "swktky_search [%s][%s]\n", deb, cre );

	int tkput = 0;
	CString filter;
	char buf[64], buf2[64], tandeb[128] = {0}, tancre[128] = {0};
	struct _DBKNREC* pKn;

	if( ! flg ) {	//単独摘要
		if( deb.IsEmpty() )	strcpy_s( buf, sizeof buf, "stdbt IS NULL" );
		else {
			sprintf_s( buf, sizeof buf, "stdbt = '%s'", (LPCTSTR)deb );

			if( pKn = DB_PjisToKnrec( deb, pZm ) ) {
				if( !(pKn->knatt & 0x01) )
					sprintf_s( tandeb, sizeof tandeb, " OR (stdbt = '%s' AND stcre IS NULL)", (LPCTSTR)deb );
				else
					sprintf_s( tandeb, sizeof tandeb, " OR (stdbt = '%s' AND stcre IS NULL) OR (stdbt IS NULL AND stcre = '%s')", (LPCTSTR)deb, (LPCTSTR)deb );
			}
		}

		if( cre.IsEmpty() )	strcpy_s( buf2, sizeof buf2, "stcre IS NULL" );
		else {
			sprintf_s( buf2, sizeof buf2, "stcre = '%s'", (LPCTSTR)cre );
			if( pKn = DB_PjisToKnrec( cre, pZm ) ) {
				if( !(pKn->knatt & 0x01) )
					sprintf_s( tancre, sizeof tancre, " OR (stdbt = '%s' AND stcre IS NULL) OR (stdbt IS NULL AND stcre = '%s')", (LPCTSTR)cre, (LPCTSTR)cre );
				else
					sprintf_s( tancre, sizeof tancre, " OR (stdbt IS NULL AND stcre = '%s')", (LPCTSTR)cre );

			}
		}
	}
	else {
		//登録通り 11.13 /12
		if( deb.IsEmpty() )	strcpy_s( buf, sizeof buf, "stdbt IS NULL" );
		else {
			sprintf_s( buf, sizeof buf, "stdbt = '%s'", (LPCTSTR)deb );

			sprintf_s( tandeb, sizeof tandeb, " OR (stdbt = '%s' AND stcre IS NULL)", (LPCTSTR)deb );
		}

		if( cre.IsEmpty() )	strcpy_s( buf2, sizeof buf2, "stcre IS NULL" );
		else {
			sprintf_s( buf2, sizeof buf2, "stcre = '%s'", (LPCTSTR)cre );

			sprintf_s( tancre, sizeof tancre, " OR (stdbt IS NULL AND stcre = '%s')", (LPCTSTR)cre );
		}
	}

	filter.Format( "(%s AND %s)", buf, buf2 );
	if( tandeb[0] )	filter += tandeb;
	if( tancre[0] ) filter += tancre;

	if( pZm->strec->Requery( filter ) == 0 ) {
		tkput = pZm->strec->GetRecordCount();
	}

	return tkput;
}

// 手形コード
// 法人・運送
static char* BillCode[] = {
"01040101", "01070701", "04010101", "04050601", "04050701", "0B020102", 
"01040500", "01071300", "04010400", "04051A00", "04051B00", "0B020A00",
NULL
};

// 建設
static char* BillCode2[] = {
"01040101", "01070801", "04010101", "04050601", "04050701", "0B020102", 
"01040500", "01071300", "04010400", "04051A00", "04051B00", "0B020A00",
NULL
};

// 個人兼業ができたので 個人専用
static char* BillCode3[] = {
"01040101", "01070801", "04010101", "04050601", "04050701", "0B020102",
"01040110", "04050610", "04050710",
"01040500", "04010400", "04051A00", "04051B00",
NULL
};

/*=============================================
	手形科目かどうか(不使用）

	CString kcd ... 科目コード
	BYTE apno ..... アプリケーション番号

  =============================================*/
DBSUB_FUNC
BOOL WINAPI IsBillCode( LPCTSTR kcd, BYTE apno )
{
	int i, n;
	BOOL ok = FALSE;
	char* p, **chk;
	chk = NULL;
	CString code;
	code = kcd;
	code.MakeUpper();

	ok = 0;
	switch( apno ) {
	case 0x00:
	case 0x01:
		chk = BillCode;
		break;

	case 0x10:
		chk = BillCode3;
		break;
	case 0x20:
		chk = BillCode2;
		break;
	default:
		break;
	}

	if( ! chk )	return FALSE;

	DWORD dwCode = _atoh( (LPSTR)kcd );

	// 支払利息は除く
	if( dwCode == 0x0B020101 ) {
		return FALSE;
	}

	//
	const int keta = 6;

	for( i = 0; chk[i] != NULL; i++ ) {
		p = chk[i];

		for( n = 0; n < keta; n++ ) {
			if( code[n] == '\0' )
				break;

			if( p[n] != code[n] )
				break;
		}
		if( n == keta ) {
			ok = TRUE;
			break;
		}
	}

	return ok;
}

/*=============================================
	手形科目かどうか(小分類拡張も含める)

	CString kcd ..... 科目コード
	CDBZmSub* pzm ... 財務クラス
 =============================================*/
DBSUB_FUNC
BOOL WINAPI IsBillCodeEx(LPCTSTR kcd, CDBZmSub* pzm)
{
	int i, n;
	BOOL ok = FALSE;
	char* p, **chk;
	chk = NULL;

	CString code;

	ok = 0;
	switch (pzm->zvol->apno) {
	case 0x00:
	case 0x01:
		chk = BillCode;
		break;
	case 0x10:
		chk = BillCode3;
		break;
	case 0x20:
		chk = BillCode2;
		break;
	default:
		break;
	}

	if (!chk)	return FALSE;

	DWORD dwCode = _atoh((LPSTR)kcd);

	// 支払利息は除く
	if (dwCode == 0x0B020101) {
		return FALSE;
	}

	if ((dwCode & 0x0000F000) < 0x00008000) {
		code = kcd;
		code.MakeUpper();
	}
	else {
		// 追加元が、まだ、追加小分類のとき
		// 追加でない合計名称サーチ
		dwCode &= ~(0x000000ff);

		hl_rev(&dwCode, 4);
		DBGKREC* pBaseGk = get_gksrc(dwCode, pzm);

		if (pBaseGk != NULL) {
			dwCode = pBaseGk->gksrc;
			hl_rev(&dwCode, 4);
			code.Format("%08x", dwCode);
		}
		else {
			return FALSE;
		}
	}
	//
	const int keta = 6;

	for (i = 0; chk[i] != NULL; i++) {
		p = chk[i];

		for (n = 0; n < keta; n++) {
			if (code[n] == '\0')
				break;

			if (p[n] != code[n])
				break;
		}
		if (n == keta) {
			ok = TRUE;
			break;
		}
	}

	return ok;
}



//医療法人 変動事由対象仕訳？
//		
int khkbn_iryohojin(CString dbt, CString cre, CDBZmSub* pZm)
{
	CZmGen8	ZmGen;
	int ret = ZmGen.CheckByoinType( pZm );

	if( ret != ID_ZMG_H29REV_IRYO_TYPE ) {
		return -1;
	}
	
	DWORD kcod;
	ret = -1;

	int kab, igai;
	kab = igai = 0;

	for( int i = 0; i < 2; i++ ) {
		if( i == 0 ) {
			kcod = _atoh( (char*)(LPCTSTR)dbt );
		}
		else {
			if( ret != -1 )
				break;
			kcod = _atoh( (char*)(LPCTSTR)cre );
		}

		switch( (kcod&0xffff0000) ) {
		case 0x007010000:
		case 0x007020000:
		case 0x007030000:
		case 0x007060000:
			kab++;
			break;

		case 0x007050000:
			igai++;
			break;
		}
	}

	if( ! kab && ! igai )	ret = -1;
	else {
		if( kab && igai )	ret = 0;
		else if( kab )		ret = 0;
		else				ret = 1;
	}

	return ret;

}


//--------------------------------------------------------------------------
// 変動事由タイプ取得
// 株主資本			--- return 0
// 株主資本以外		--- return 1
// 変動事由の対象外仕訳	--- return -1;
//
//	この関数を変更した場合は、一括修正プロも変更する必要あり
//--------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI get_khkbntype( CString dbt, CString cre, CDBZmSub* pZm )
{
	int kab, igai, ret;
	kab = igai = 0;

	switch( pZm->zvol->apno ) {
		case 0x00:
		case 0x01:
		case 0x20:
			break;
		//病院は医療かどうか？
		case 0x30:
			return khkbn_iryohojin(dbt, cre, pZm);
			break;
		default:
			return -1;
			break;
	}

	DWORD kcod;

	for( int i = 0; i < 2; i++ ) {
		if( i == 0 ) {
			kcod = _atoh( (char*)(LPCTSTR)dbt );
		}
		else {
			kcod = _atoh( (char*)(LPCTSTR)cre );
		}

		switch( (kcod&0xffff0000) ) {
		case 0x07010000:
		case 0x07020000:
		case 0x07040000:	// 株主資本
			kab++;
			break;

		case 0x07030000:
			switch( (kcod&0xffffff00) ) {
			case 0x07030800:
			case 0x07030900:
			case 0x07030a00:	// 前期繰越・未処理損失は 除く
				break;
			default:
				kab++;
				break;
			}			
			break;
		case 0x07050000:	// 株主資本以外
		case 0x07060000:
		case 0x07070000:
			igai++;
			break;
		}

/*
		switch( (kcod&0x00000f0f) ) {
		case 0x00000107:
		case 0x00000207:
		case 0x00000307:
		case 0x00000407:	// 株主資本	
			kab++;
			break;
		case 0x00000507:	// 株主資本以外
		case 0x00000607:
			igai++;
			break;
		}
*/
	}

	if( ! kab && ! igai )	ret = -1;
	else {
		if( kab && igai )	ret = 0;
		else if( kab )		ret = 0;
		else				ret = 1;
	}

	return ret;
}

//--------------------------------------------------------------------
//	変動事由コードが正しいか？
//	int h_code ... 変動事由コード
//	int h_type ... 変動事由タイプ[get_khkbntype() ]
//	return TRUE is Right, FALSE is not Right
//
//--------------------------------------------------------------------
DBSUB_FUNC
BOOL WINAPI IsHenJiyuCodeRight( int h_code, int h_type )
{
	BOOL bRet = FALSE;

	switch( h_type ) {
	case 0:	// 株主資本等
		if( (h_code >=  1 && h_code <= 20) ||
			(h_code >= 31 && h_code <= 50) ) {
			bRet = TRUE;
		}
		break;
	case 1:	// 株主資本以外
		if( (h_code >= 61 && h_code <= 80) ) {
			bRet = TRUE;
		}
		break;
	default:
		break;
	}
	// 遡及事由
	if( ! bRet ) {
		if( h_type == 0 || h_type == 1 ) {
			if( (h_code >= 86 && h_code <= 90) ) {
				bRet = TRUE;
			}
		}
	}

	return bRet;
}



//--------------------------------------------------------------------
//	医療の変動事由コードが正しいか？
//	int h_code ... 変動事由コード
//	int h_type ... 変動事由タイプ[get_khkbntype() ]
//	return TRUE is Right, FALSE is not Right
//
//--------------------------------------------------------------------
DBSUB_FUNC
BOOL WINAPI IsIryoHjiyuCodeRight( int h_code, int h_type )
{
	BOOL bRet = FALSE;

	int cd2 = (h_code -100);

	return IsHenJiyuCodeRight( cd2, h_type );

}


/*-----------------------------------------------
	02.09 /02
	指定バイトのうち、文字列として正しい
	バイト数を返送します
  -----------------------------------------------*/
int get_strcnt(char* str, int byte)
{
	int k, st, btcnt;

	btcnt = 0;

	for(k = 0; k < byte && *(str+k) != 0; ) {

		st = IsKindOfLetter( (LPCTSTR)str, k );
		switch( st ) {
		case 1:		// ASCII
			k++;	
			if( k <= byte ) btcnt += 1;
			break;
		case 2:		// MB文字1バイト目
			k += 2;
			if( k <= byte ) btcnt += 2;
			break;
		default:
			return 0;
		}

	}

	return btcnt;
}


/*---VER2----*/
/*=================================================================================================
	科目名称 ＋ 科目入力コード の文字列作成
	 int		type	[0] 科目名称の間のスペースをカットせずにそのままコードを追加
						[1]	科目名称の間のスペースをカットして、そのままコードを追加
						[2] 科目名称の間のスペースをカットして、科目名称レングス内にコードを追加
	 DBKNREC	*pKn	
	 char		*buf	
	 int		buflength
  =================================================================================================*/
DBSUB_FUNC 
int WINAPI kmkcode_string( int type, DBKNREC* pKn, char *buf, int buflength )
{
	char	tmp[128] = {0};
	char	knam_txt[64] = {0};
	char	work[64] = {0};

	int		i, n;

	sprintf_s( tmp, sizeof tmp, "%.14s", pKn->knnam );
	if( type > 0 ) {
		i = ksp_cut( knam_txt, tmp, 14 );	// 漢字スペースカット
	}
	else {
		memcpy( knam_txt, tmp, 14 );
	}
#ifdef OLD_CLOSE
	// コード文字列作成
	if( pKn->knicod == -1 ) {
		strcpy_s( tmp, sizeof tmp, "(    )" );
	}
	else {
		sprintf_s( tmp, sizeof tmp, "(%04d)", pKn->knicod );
	}

	// 名称＋ コード 作成
	if( type == 2 ) {
		n = get_strcnt( knam_txt, (14-6) );
		memcpy( work, knam_txt, n );
		i = sprintf_s( buf, buflength, "%s%s", work, tmp );
	}
	else {
		i = sprintf_s( buf, buflength, "%s%s", knam_txt, tmp );
	}
#else
	// コード文字列作成
	if( pKn->knicod == -1 ) {
		strcpy_s( tmp, sizeof tmp, "    " );
	}
	else {
		sprintf_s( tmp, sizeof tmp, "%04d", pKn->knicod );
	}

	//  コード + 名称 作成
	if( type == 2 ) {
		n = get_strcnt( knam_txt, (14-4) );
		memcpy( work, knam_txt, n );
		i = sprintf_s( buf, buflength, "%s%s", tmp, work );
	}
	else {
		i = sprintf_s( buf, buflength, "%s%s", tmp, knam_txt );
	}
#endif

	return i;
}


/*---VER2----*/
/*====================================================================================
	文字列の中に 指定文字列が含まれているか？
		LPCTSTR string	全体の文字列
		LPCTSTR chk		チェックする文字列

  ====================================================================================*/
DBSUB_FUNC 
BOOL IsIncludeString( LPCTSTR string, LPCTSTR chk )
{
	int length, n, i, k, st;
	char* pcmp;

	length = strlen( string );
	n = strlen( chk );

	if( ! length || ! n )	return FALSE;

	pcmp = (char*)string;

	for( i = length, k = 0; i >= n ; )
	{
		if( !strncmp( pcmp, (char *)chk, n ) )
			break;	//Found

		st = IsKindOfLetter( string, k );
		switch( st ) {
		case 1:
			i--;	pcmp++;	k++;	break;
		case 2:
			i -= 2;	pcmp += 2;	k += 2;	break;
		default:
			return FALSE;
		}
	}

	if( i < n )
		return( FALSE );

	return TRUE;
}



/*====================================================================================
	個人の表示制限

  ====================================================================================*/
DBSUB_FUNC 
int WINAPI indkmk_dialog( CWnd* pwnd, DWORD& dwSign, CDBZmSub* pZm )
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

	CIndkmkDlg	dlg(pwnd);

	dlg.m_pZm = pZm;

	dlg.m_chkIppan	= (dwSign&0x01) ? TRUE : FALSE;
	dlg.m_chkHudo	= (dwSign&0x02) ? TRUE : FALSE;
	dlg.m_chkNogyo	= (dwSign&0x04) ? TRUE : FALSE;

	dlg.m_chkAll	= (dwSign&0x1000) ? TRUE : FALSE;

	int st;

	st = dlg.DoModal();

AfxSetResourceHandle( h );

	if( st == IDOK ) {
		dwSign	=	0;

		if( dlg.m_chkIppan )	dwSign |= 0x01;
		if( dlg.m_chkHudo )		dwSign |= 0x02;
		if( dlg.m_chkNogyo )	dwSign |= 0x04;

		if( dlg.m_chkAll )		dwSign = 0x1000;

	}

	return (st==-1) ? -1 : st;
}


/*====================================================================================
	個人の業種サインのマスターか？

	CDBZmSub* pZm:	チェックにボリュームラベルが オープンされている必要がある

  ====================================================================================*/
DBSUB_FUNC 
int WINAPI IsKojinGyosyuMaster( CDBZmSub* pZm )
{
	int type = 0;

	if( pZm->zvol->m_ver < 0x12 )
		return 0;

	if( pZm->zvol->apno == 0x10 ) {
		type = pZm->zvol->ind_type;
		if( type == 0 )	type = 0x01;
	}
	else {
		type = pZm->zvol->ind_type;
	}

	return type;
}

/*====================================================================================
	個人の兼業のマスターか？

	gyosgn は、IsKojinGyosyuMaster() の 返送値を使用すること
  ====================================================================================*/
DBSUB_FUNC 
BOOL WINAPI IsKojinKengyoMaster( int gyosgn )
{
	BOOL bRet = FALSE;

	if( (gyosgn&0x0f) == 0 )
		return FALSE;

	if( (gyosgn&0x0f) == 1 || (gyosgn&0x0f) == 2 || (gyosgn&0x0f) == 4 ) {
		// 単一業種
	}
	else {
		bRet = TRUE;
	}

	return bRet;
}


//
// 実際の科目の表示制限チェック
// 
DBSUB_FUNC 
BOOL WINAPI CheckKamokuRestrict( CDBZmSub* pZm, DBKNREC* pKn )
{
	DWORD	dwCode;

	// 個人兼業でない。
	int kjn_type;
	kjn_type = IsKojinGyosyuMaster( pZm );

	if( kjn_type == 0 )	return TRUE;

	int type = (kjn_type & 0x0f);

	if( pKn ) {
		dwCode	=	pKn->kncod;
		hl_rev( &dwCode, 4 );
	}
	else {
		return FALSE;
	}

	// 単一の業種
	if( type == 0x01 || type == 0x02 || type == 0x04 ) {
		// 貸借と損益で扱いが異なる
		if( dwCode < 0x08000000 ) {
			if( pKn->ind_ctg == 0x00 || (pKn->ind_ctg & type) )
				return TRUE;
		}
		else {
			if( pKn->ind_ctg == (BYTE)type )
				return TRUE;
		}
	}
	else {
		// 兼業
		if( dwCode < 0x08000000 ) {
			if( (pZm->zvol->ind_sgn2 & 0x10) ) {
				// 貸借個別
				if( pKn->ind_ctg == 0x00 || (pKn->ind_ctg & type) )
					return TRUE;
			}
			else {
				// 貸借共通 各メインの科目を出すため、メイン以外のサインは落とす。
				BYTE	xorbit;
				xorbit = (pZm->zvol->ind_sgn2 ^ 0x07);
				type &= ~(xorbit);

				if( pKn->ind_ctg == 0x00 || (pKn->ind_ctg & type) )
					return TRUE;
			}
		}
		else {
			if( pKn->ind_ctg & (BYTE)type )
				return TRUE;
		}
	}

	return FALSE;
}

/*====================================================================================
	公益 事業費＆管理費の表示制限 ('08.12.01)
  ------------------------------------------------------------------------------------
	引数	pwnd	：	親ウィンドウ
			pJgykcd	：	事業費コード保存先
			pKnrkcd	：	管理費コード保存先
			pZm		：	財務クラス
  ------------------------------------------------------------------------------------
	返送値	0		：	正常終了
			-1		：	エラー
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI kpubkmk_dialog( CWnd* pwnd, unsigned long* pJgykcd, unsigned long* pKnrkcd, CDBZmSub* pZm )
{
	ASSERT(pZm);

HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

	CKpubDlg	dlg(pwnd);

	dlg.m_pZm = pZm;
	dlg.SetInitKamoku( *pJgykcd, *pKnrkcd );

	int st = dlg.DoModal();

AfxSetResourceHandle( h );

	if( st == IDOK ){
		ASSERT( pJgykcd );
		ASSERT( pKnrkcd );
		dlg.GetSelJigyoKanri( pJgykcd, pKnrkcd );
	}

	return (st==-1) ? -1 : st;
}

/*====================================================================================
	公益法人のマスターか？ ('08.12.01)
  ------------------------------------------------------------------------------------
	引数	pZm		：	財務クラス（チェックにボリュームラベルが オープンされている必要がある）
  ------------------------------------------------------------------------------------
	返送値	1		：	公益法人マスター
			0		：	公益法人マスターで無い
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI IsKouekiMaster( CDBZmSub* pZm )
{
	ASSERT( pZm );
	ASSERT( pZm->zvol );

//	if( (pZm->zvol->apno&0xff) >= 0x50 ){
	if( (pZm->zvol->apno&0xff) == 0x50 || (pZm->zvol->apno&0xff) == 0x51 ){
		return 1;
	}

	return 0;
}

/*====================================================================================
	事業費・管理費が拡張されているマスターか？ ('08.12.01)
  ------------------------------------------------------------------------------------
	引数	pZm		：	財務クラス
  ------------------------------------------------------------------------------------
	返送値	1		：	事業費・管理費が拡張されているマスター
			0		：	事業費・管理費が拡張されているマスターで無い
			-1		：	エラー
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI IsExtendJgyOrKnrMaster( CDBZmSub* pZm )
{
	ASSERT( pZm );

	if( pZm->pGkrec == NULL ){
		if( pZm->GokeiMeisyoRead() == -1 ){
			return -1;
		}
	}

	BOOL isExtend = FALSE;
	for( int i=0; i<pZm->gkn_info->reccount; i++ ){
		if( (pZm->pGkrec[i].gkcod&0x000000ff) >= 0x00000080 ){
			if( (pZm->pGkrec[i].gkbcod==0x09) || (pZm->pGkrec[i].gkbcod==0x0a) ){
				isExtend = TRUE;
				break;
			}
		}
	}

	return isExtend ? 1 : 0;
}

/*====================================================================================
	事業費・管理費 表示名称を取得する ('08.12.03)
  ------------------------------------------------------------------------------------
	引数	pZm		：	財務クラス
			jgykcd	：	事業費コード
			knrkcd	：	管理費コード
			pDspstr	：	表示名称
  ------------------------------------------------------------------------------------
	返送値	0		：	正常終了
			-1		：	エラー
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI GetJgyKnrDspstr( CDBZmSub *pZm, unsigned long jgykcd, unsigned long knrkcd, CString *pDspstr )
{
	ASSERT( pZm );
	ASSERT( pDspstr );

	if( pZm->pGkrec == NULL ){
		if( pZm->GokeiMeisyoRead() == -1 ){
			return -1;
		}
	}

	DBGKREC	*pDbgkrec;
	if( jgykcd == 0xffffffff ){
		*pDspstr = "全    事    業    費    ";
	}
	else if( pDbgkrec = pZm->PjisToGkrec(jgykcd) ){
		pDspstr->Format( "%-24.24s", pDbgkrec->gknam );
	}
	else{
		// 不正値
		*pDspstr = "　　　　　　　　　　　 ";
	}
	*pDspstr += "・  ";

	if( knrkcd == 0xffffffff ){
		*pDspstr += "全    管    理    費    ";
	}
	else if( pDbgkrec = pZm->PjisToGkrec(knrkcd) ){
		CString	str;
		str.Format( "%-24.24s", pDbgkrec->gknam );
		*pDspstr += str;
	}
	else{
		// 不正値
		*pDspstr = "　　　　　　　　　　　　";
	}

	return 0;
}


// SQL の文字列サーチで ' を検索するのに失敗していたため、検索文字列を調整する

DBSUB_FUNC 
int WINAPI AdjustSQLsearch( CString& search )
{
	CString tmp;

	int len = search.GetLength();
	int i, k, st;
	int	ret = 0;

	LPSTR p = tmp.GetBuffer( (len*2)+10 );

	for( i = k = 0; i < len; ) {

		st = IsKindOfLetter( (LPCTSTR)search, i );

		if( st == 1 ) {
			if( search[i] == '\'' ) {
				p[k++] = '\'';
				p[k++] = '\'';
			}
			else {
				p[k++] = search[i];
			}
			i++;
		}
		else if( st == 2 ) {
			p[k++] = search[i];
			p[k++] = search[i+1];
			i += 2;
		}
		else {
			ret = -1;
			break;
		}
	}

	p[k] = '\0';
	tmp.ReleaseBuffer();

	if( ! ret ) {
		search = tmp;
	}

	return ret;
}

void furikana_chk( char* buf, int bfsize, CString& strText )
{
	::ZeroMemory( buf, bfsize );

	int k, byte, st, c1, n;
	byte = strText.GetLength();

	for( k = n = 0; k < byte; ) {
		st = IsKindOfLetter( (LPCTSTR)strText, k );
		c1 = (BYTE)strText[k];

		switch( st ) {
		case 1:		// ASCII
			if( c1 >= 0xA6 && c1 <= 0xDF || c1 == '-' ) {	// ｦ ～ ﾝ,ﾟ,ﾞ
				buf[n++] = c1;
				if( n >= 4 ) {
					k = byte;
				}
			}
			k++;	
			break;
		default:
			k = byte;
			break;
		}
	}
}


// ダイアログの伸縮率を取得
void Dlg_GetRate( ICSDialog* pDlg, double& grx, double& gry )
{
	int typ = GetMultiWindowType();
	if( typ == MULTIWINDOW_NOP || typ == MULTIWINDOW_NONE ) {
		grx = gry = 0;
	}
	else {
		pDlg->GetRate( grx, gry);
	}
}


DBSUB_FUNC 
LPCTSTR WINAPI GNKINCode( int apno )
{
	static const char* gnkin	= "01010101";
	static const char* SYUgen	= "03040101";	//宗教
	static const char* SCHgen	= "03010101";	//学校

	if( apno == 0x59 ) {
		return SYUgen;
	}
	else if( apno == 0x58 ) {
		return SCHgen;
	}
	else {
		return gnkin;
	}
}


//--------------------------------------------------------------
//	社会福祉改正マスターか？
//
//--------------------------------------------------------------
BOOL IsSyafuKaisei( CZVOLUME* zv )
{
//	if( IsOKGyosyu( zv->apno, SW_SOCIAL ) && zv->m_ver == 0x11 )
	if( zv->apno == 0x52 && zv->m_ver == 0x11 )
		return TRUE;

	return FALSE;
}


#include "NpHwCalq8.h"


//--------------------------------------------------------------
//	社会福祉改正マスターで、按分の登録があるマスターか？
//
//--------------------------------------------------------------
DBSUB_FUNC 
BOOL WINAPI IsSyafuKaiseiAnbun( CDBZmSub* pZm )
{
	if( pZm->zvol->apno == 0x52 && pZm->zvol->m_ver == 0x11 ) {

		CNpHwCalq chNphw( pZm );

		int st = chNphw.IsRegistNphw();
//MyTrace( "@IsSyafuKaiseiAnbun IsRegistNphw --> %d\n", st );

		if( st > 0 ) {
			return TRUE;
		}
	}

	return FALSE;
}


//--------------------------------------------------------------
//	証憑 自動加算
//
//--------------------------------------------------------------
DBSUB_FUNC 
void WINAPI DocEviAutoString( char* pDEstr, int strsize, int keta )
{
	//元の文字列から 数字部分を抜き出す。
	char bf[128] = {0};
	char wk[128] = {0};
	char str[128] = {0};
	char tmp[128] = {0};

	int k, i, st, sidx;

	strcpy_s( bf, sizeof bf, pDEstr );

	// 末の数字文字列
	sidx = -1;

	for( k = i = 0; *(bf+k) != 0; ) {
		st = IsKindOfLetter( (LPCTSTR)bf, k );
		switch( st ) {
		case 1:		// ASCII
			if( isdigit( *(bf+k) ) ) {	//数字
				if( sidx == -1 )	sidx = k;
				*(wk + i++) = *(bf+k);
				k++;
			}
			else {
				if( i > 0 ) {
					i = 0;
					::ZeroMemory( wk, sizeof wk );
					sidx = -1;
				}
				k++;
				continue;
			}
			break;
		case 2:		// MB文字1バイト目
			if( i > 0 ) {
				i = 0;
				::ZeroMemory( wk, sizeof wk );
				sidx = -1;
			}
			k += 2;
			break;
		default:
			return;
		}
	}

	if( sidx >= 0 ) {
		memcpy( str, bf, sidx );

		int s_len = sidx;	//数字以外の文字数
		int len = strlen( wk );
		int nNum = atoi( wk );

		char fmt[20];
		nNum++;

		sprintf_s( tmp, sizeof tmp, "%d", nNum );

		int len2 = strlen( tmp );

		if( len2 > len ) {	//桁あがり
			if( (len2 + s_len) > keta ) {
				sprintf_s( fmt, sizeof fmt, "%%.0%dd", len );
				sprintf_s( tmp, sizeof tmp, fmt, 0 );
			}
			else {
				sprintf_s( tmp, sizeof tmp, "%d", nNum );
			}

			sprintf_s( pDEstr, strsize, "%s%s", str, tmp );
		}
		else {
			for( int n = len-1; n >= 0; n-- ) {
				if( wk[n] == '9' ) {
					wk[n] = '0';
				}
				else {
					wk[n] += 1;
					break;
				}
			}
			sprintf_s( pDEstr, strsize, "%s%s", str, wk );
		}
	}
}




/*====================================================================================
	学校法人のマスターか？
  ------------------------------------------------------------------------------------
	引数	pZm		：	財務クラス（チェックにボリュームラベルが オープンされている必要がある）
  ------------------------------------------------------------------------------------
	返送値	1		：	学校法人マスター
			0		：	学校法人マスターで無い
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI IsGakkouMaster( CDBZmSub* pZm )
{
	ASSERT( pZm );
	ASSERT( pZm->zvol );

	if( (pZm->zvol->apno&0xff) == 0x58 ){
		return 1;
	}

	return 0;
}

/*====================================================================================
	収支科目を使用しているマスターか？

  ------------------------------------------------------------------------------------
	返送値	1		：	収支科目使用 マスター
			0		：	　　〃　　　 マスターで無い
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI IsSyusiKamokuMaster( CDBZmSub* pZm )
{
	int ret = 0;

	if( IsGakkouMaster( pZm ) || IsSyafuKaisei( pZm->zvol ) ) {
		ret++;
	}
	else if( IsKouekiMaster( pZm ) ) {
		if( pZm->zvol->apno == 0x50 ) {
			ret++;
		}
	}

	return ret;
}



// 自由摘要の文字数を返送する関数
DBSUB_FUNC 
int WINAPI GetTekiyoLength( CDBZmSub* pZm )
{
	int tkycnt = 20;

	//マスターバージョンによる。
	//vtbl

//	int vver = GetMasterVver( pZm );
	CZmGen8		ZmGen;
	int vver = ZmGen.CheckValiableVersion( pZm->m_database, ID_H26SYZ_VUP_CHK );

	if( vver >= 1 ) {
		tkycnt = 40;
	}

	return tkycnt;
}




//-----------------------------------------------------
// マスターのvverを返送する。
//
//-----------------------------------------------------
DBSUB_FUNC
int WINAPI GetMasterVver( CDBZmSub* pZm )
{
	//マスターバージョンによる。
	//vtbl
	CStRecordSet st_rec( pZm->m_database );
	st_rec.m_sql = _T( "select vers as st from vtbl where objname = 'vver'" );

	CString strError;
	int ret = 0;

	TRY {
		if( ! st_rec.Open() ) {
			ICSMessageBox( _T( "バージョンチェックが行えません！" ) );
			return -1;
		}
		
		if( ! st_rec.IsBOF() ) {
			ret = st_rec.m_st;
		}

		st_rec.Close();
	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( strError.GetBuffer(_MAX_PATH), _MAX_PATH );
		strError.ReleaseBuffer();
		ICSMessageBox( strError );
		return -1;
	}
	CATCH( CDBException, de ) {
		strError = de->m_strError;
		ICSMessageBox( strError );
		return -1;
	}
	END_CATCH

	return ret;
}




/*====================================================================================
	学校法人のＨ２７ 改正マスターか？ ('14.12.25)
  ------------------------------------------------------------------------------------
	引数	pZm		：	財務クラス（チェックにボリュームラベルが オープンされている必要がある）
  ------------------------------------------------------------------------------------
	返送値	1		：	学校法人Ｈ２７マスター
			0		：	学校法人Ｈ２７マスターで無い
  ====================================================================================*/
DBSUB_FUNC 
int WINAPI IsGakkouH27Master( CDBZmSub* pZm )
{
	ASSERT( pZm );
	ASSERT( pZm->zvol );

	if( (pZm->zvol->apno&0xff) == 0x58 && (pZm->zvol->m_ver == 0x11) ){
		return 1;
	}

	return 0;
}


int ZmsubOwntbOpen( CDBZmSub* pZm, BOOL& bOpen, BOOL bAddRecord )
{
	// ユーザー固有情報からＩＤ取得
	int user_id = -1;
	CString filter;
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;
	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl( utbl );
		user_id = utbl.user_id;
	}

	BOOL bOwnOpen = FALSE;

	filter.Format( "apl_name = '%s' AND itm_name = '%s' AND u_id = %d", INI_AppName, INI_ItmName, user_id );

	// COWNTB OpenされていなければOPEN
	if( pZm->owntb == NULL ) {
		if( pZm->OwnTblOpen( filter ) == ERR )
			return -1;
		bOwnOpen = TRUE;
	}
	else {
		pZm->owntb->Requery( filter );
	}

	if( pZm->owntb->st == -1 ) {
		if( bAddRecord ) {
			struct _AUTOSEL* psel;

			CDinpINI	dini;
			dini.Init( pZm );
			psel = dini.GetDinpSW();
			dini.SetDinpSW( psel, FALSE );

			pZm->owntb->Requery( filter );
		}
	}
	bOpen = bOwnOpen;

	return 0;
}


/*-----------------------------------------------
	摘要文字列チェック
	最後に不正文字があった場合は、'\0' を代入

  -----------------------------------------------*/
DBSUB_FUNC 
int WINAPI teki_stringcheck(char* str, int byte)
{
	int k, st, end_sw, len;

	end_sw = 0;
	len = strlen( str );

	if( len == 0 )	return 0;

	for( k = len-1; k >= 0; ) {
		st = IsKindOfLetter( (LPCTSTR)str, k );

		switch( st ) {
		case 1:		// ASCII
			k--;	
			break;
		case 2:		// MB文字
		case 3:
			k--;	
			break;
		case 0:
			*(str+k) = '\0';
			end_sw = 1;
			break;
		}

		if( end_sw )	break;
	}

	return 0;
}




int vector_finder(std::vector<int> vec, int number) 
{
	vector<int>::iterator itr;
	itr = std::find(vec.begin(), vec.end(), number);
	size_t index = std::distance( vec.begin(), itr );

	if (index != vec.size()) { // 発見できたとき
		return 1;
	}
	else { // 発見できなかったとき
		return 0;
	}
}


// 外税仕訳か？
BOOL sotozei_data( CDBipDataRec *drec )
{
	
	return _sotozei_data( &drec->m_dsign[0] );
}

// 外税仕訳か？
BOOL sotozei_data(CDBINPDataRec *drec)
{

	return _sotozei_data(&drec->m_dsign[0]);
}

// 外税仕訳か？
BOOL _sotozei_data( BYTE *dsign )
{
	BOOL bRet = FALSE;
	
	if( (dsign[0] & 0x40) && (dsign[1]&0xf0)==0x00 &&
		(dsign[2] & 0x0f) == 0x01 ) {
			bRet = TRUE;
	}

	return bRet;
}


// 譲渡仕訳か？
BOOL _joto_data( BYTE *dsign )
{
	BOOL bRet = FALSE;
	
	if( (dsign[0] & 0x40) && (dsign[2] & 0x30) == 0x30 ) {
			bRet = TRUE;
	}

	return bRet;
}



BOOL _isSyzdiff_data( BYTE *newdsign, BYTE *motodsign )
{
	BOOL ret = FALSE;

	if( (newdsign[0]&0x40) != (motodsign[0]&0x40) )
		ret = TRUE;
	else {
		if( memcmp( &newdsign[1], &motodsign[1], 5 ) != 0 ) 
			ret = TRUE;
	}

//MyTrace( "_isSyzdiff_data newdsign [%02x%02x%02x%02x%02x%02x], motodsign [%02x%02x%02x%02x%02x%02x]",
//		newdsign[0],newdsign[1],newdsign[2],newdsign[3],newdsign[4],newdsign[5],  motodsign[0],motodsign[1],motodsign[2],motodsign[3],motodsign[4],motodsign[5] );

	return ret;
}


// 業種番号のチェック
BOOL IsOKGyosyu( int apno, DWORD check_bit )
{
	int check = 0;

	BOOL bRet = FALSE;

	if( check_bit & SW_HOJIN ) {
		if( apno == 0x00 )	check++;
	}
	if( check_bit & SW_KOJIN ) {
		if( apno == 0x10 )	check++;
	}
	if( check_bit & SW_UNSO ) {
		if( apno == 0x01 )	check++;
	}
	if( check_bit & SW_KENST ) {
		if( apno == 0x20 )	check++;
	}
	if( check_bit & SW_BYOIN ) {
		if( apno == 0x30 )	check++;
	}
	if( check_bit & SW_PUB ) {
		if( apno == 0x50 )	check++;
	}
	if( check_bit & SW_PUB3 ) {
		if( apno == 0x51 )	check++;
	}
	if( check_bit & SW_SOCIAL ) {
		if( apno == 0x52 )	check++;
	}
	if( check_bit & SW_SOCIAL2 ) {
		if( apno == 0x53 )	check++;
	}
	if( check_bit & SW_SYUKYO ) {
		if( apno == 0x59 )	check++;
	}
	//学校対応
	if( check_bit & SW_SCHOOL ) {
		if( apno == 0x58 )	check++;
	}

	if( check )	bRet = TRUE;

	return bRet;
}


BYTE KaniDefaultKubun( CZVOLUME* zv )
{
	BYTE defKbn = 0;
	if( (zv->s_sgn2&0x0f) >= 2 ) {
		defKbn = (zv->s_sgn2 & 0x0f) - 1;
	}

	return defKbn;
}


//-----------------------------------------------------------
// 入力枝番タイプ
//	int nowtype			枝番入力ﾀｲﾌﾟ INP_type(LINE_CND)
//	CString nowcode		現　科目コード
//	int nowbrn			現　枝番コード
//	CString mdfycode	修正科目コード
//	int mdfybrn			修正枝番コード
//
//	return =0 科目枝番が変更になって、枝番が未入力 
//		   =1 科目枝番が変更になって、枝番が入力された
//
//-----------------------------------------------------------
int brn_inptype( int nowtype, CString nowcode, int nowbrn, CString mdfycode, int mdfybrn )
{
	// 科目が変わった場合
	if( nowcode != mdfycode )
	{
		if( mdfybrn == -1 )
			return 0;
		else
			return 1;
	}
	// 枝番が変わった場合
	else if( nowbrn != mdfybrn )
	{
		if( mdfybrn == -1 )
			return 0;
		else
			return 1;
	}
	return nowtype;
}



int RecordSetCheck(CDBZmSub* pzm)
{
	CRecordset rec(pzm->m_database);

	CString strError;
	TRY{
MyTrace("★DBInpSub★dbdata ptr = %08x data ptr = %08x(2)\n", pzm->dbdata, pzm->data);
		if (rec.Open(CRecordset::forwardOnly, "select seq from datarec")) {
			CDBVariant varValue;

			short nFields = rec.GetODBCFieldCount();
			while (!rec.IsEOF())
			{
				for (short index = 0; index < nFields; index++)
				{
					rec.GetFieldValue(index, varValue);
					// do something with varValue

MyTrace("----> GetFieldValue[%d] dwType = %08x()\n", index, varValue.m_dwType);
				}
				rec.MoveNext();
				break;
			}
			rec.Close();
		}
	}
		CATCH(CMemoryException, me) {
		me->GetErrorMessage(strError.GetBuffer(_MAX_PATH), _MAX_PATH);
		strError.ReleaseBuffer();
		MyTrace("★DBInpSub★CMemoryException %s\n", strError);
		return -1;
	}
	CATCH(CDBException, de) {
		strError = de->m_strError;
		MyTrace("★DBInpSub★CDBException code[%d], %s\n", de->m_nRetCode, strError);
		return -1;
	}
	END_CATCH

	return 0;
}


CDnpInpDialog*	gDnpdlg = NULL;

int WINAPI DenpInputDialogSub(DENP_INPUTPAR* para)
{
	int st;
	if (para->modeless == 0) {
		CDnpInpDialog	dlg;
		dlg.m_pInpPar = para;
		st = dlg.DoModal();
	}
	else {
		if (gDnpdlg == NULL) {
			gDnpdlg = new CDnpInpDialog;
			gDnpdlg->m_pInpPar = para;
			gDnpdlg->CreateDlg();
		}
		else {
			gDnpdlg->SetInputPar(para);
		}
		//20230210
		gDnpdlg->Set_show_flg(0);
			   
		st = gDnpdlg->ModelessDoModal();
	}

	if (st == IDCANCEL)
		return IDCANCEL;

	return 0;
}

int WINAPI DenpInputDialogDestroy()
{
	if (gDnpdlg != NULL) {
		gDnpdlg->DestroyWindow();
		delete gDnpdlg;
		gDnpdlg = NULL;
	}

	return 0;
}


//--------------------------------------------------------------
//	伝票形式 入力ダイアログ
//
//--------------------------------------------------------------
DBSUB_FUNC int WINAPI DenpInputDialog( DENP_INPUTPAR* para )
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

DBGFUNCTION;
	para->modeless = 1;
//	para->modeless = 0;

	int st = DenpInputDialogSub(para);

	//CDnpInpDialog	dlg;
	//dlg.m_pInpPar = para;
	//int st = dlg.DoModal();

AfxSetResourceHandle( h );
//RecordSetCheck(para->pZmsub);

	if( st == IDCANCEL ) 
		return IDCANCEL;

	return 0;
}



//--------------------------------------------------------------
//	伝票形式 入力タイプの返送
//
//--------------------------------------------------------------
DBSUB_FUNC int WINAPI DenpTypeString( BYTE denp, CString& str )
{
	str.Empty();

	switch( denp ) {
	case 0x10:	str = "入";	break;
	case 0x20:	str = "出";	break;
	case 0x30:	str = "振";	break;
	}

	return 0;
}


/*--------------------------------------------------------------
　元号変換ルール
　　０１０１０１～０１０１０７：不正データ
　　０１０１０８～０１０４３０：平成
　　０１０５０１～１９１２３１：新元号
　　２００１０１～３１０４３０：平成
　　３１０５０１～　　　　　　：新元号
--------------------------------------------------------------*/

int func_genymd_to_seireki( int src_ymd )
{
	int gengo, d_gengo, ymd;
	ymd = -1;

	if( IsValidKaigen() ) {
		if( src_ymd >= 10101 && src_ymd <= 10107 ) {
			return -1;
		}
		else if( src_ymd >= 10108 && src_ymd <= 10430 ) {
			gengo = 4;
		}
		else if( src_ymd >= 10501 && src_ymd <= 191231 ) {
			gengo = 5;
		}
		else if( src_ymd >= 200101 && src_ymd <= 310430 ) {
			gengo = 4;
		}
		else if( src_ymd >= 310501 ) {
			gengo = 5;
		}
		else {
			return -1;
		}

		db_datecnvGen( gengo, src_ymd, &d_gengo, &ymd, 1, 0 );

//MyTrace( "func_genymd_to_seireki db_datecnvGen --> gengo = %d, src_ymd = %d, d_gengo = %d, ymd = %d\n", gengo, src_ymd, d_gengo, ymd );
	}
	else {
		// 平成から西暦へ
		db_datecnv( src_ymd, &ymd, 1, 0 );

//MyTrace( "func_genymd_to_seireki db_datecnv --> src_ymd = %d,ymd = %d\n", src_ymd, ymd );
	}

	return ymd;
}


DBSUB_FUNC
int WINAPI TgDateBcdToSeireki( BYTE* bcd, CDBZmSub* pZm )
{
	char bf[64];
	int tmp;

	sprintf_s( bf, sizeof bf, "%02x%02x%02x", bcd[0], bcd[1], bcd[2] );
	int src_ymd = atoi( bf );

	int ymd = func_genymd_to_seireki( src_ymd );

	return ymd;
}


//----------------------------------------------------------------------------
// 枝番・部門 なし → ありに変更したとき、入力設定の関連サインをONにする
//	pZm	  マスターデータベース接続済みの財務クラス
//	nFlag D0 枝番 なし → ありに変更
//		  D1 部門 なし → ありに変更
//
//	return = 0  OK
//		   = -1 エラー
//----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI DBDinpSignUpdate(CDBZmSub* pZm, DWORD nFlag)
{
	// ユーザー固有情報からＩＤ取得
	int user_id = -1;
	CString filter;
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;
	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl(utbl);
		user_id = utbl.user_id;
	}

	BOOL bOwnOpen = FALSE;

	filter.Format("apl_name = '%s' AND itm_name = '%s' AND u_id = %d", INI_AppName, INI_ItmName, user_id);

	// COWNTB OpenされていなければOPEN
	if( pZm->owntb == NULL ) {
		if( pZm->OwnTblOpen(filter) == ERR )
			return -1;
		bOwnOpen = TRUE;
	}
	else {
		pZm->owntb->Requery(filter);
	}

	if( pZm->owntb->st != -1 ) {
		struct _AUTOSEL* psel;

		CDinpINI	dini;
		dini.Init(pZm);
		psel = dini.GetDinpSW();

		if( nFlag&0x01 ) {
			psel->BRN_OPT = OPT_RGSCHK;		//枝番未入力チェック
			psel->BRCPY_OPT = OPT_BRCPY;	//枝番コピー する
			psel->BRTEK_OPT = OPT_DSP;		//枝番名称の表示
		}
		if( nFlag & 0x02 ) {
			psel->BMNCHK_OPT = OPT_BMNCHK;	//部門番号チェック
		}

		dini.SetDinpSW(psel, FALSE);

		pZm->owntb->Requery(filter);
	}
	if( bOwnOpen ) {
		pZm->OwnTblClose();
	}

	return 0;
}

const	int	PER8_SYZ_START_YMD = 20140401;	// 消費税：８％施行日
const	int	PER5_SYZ_START_YMD = 19970401;	// 消費税：５％施行日
const	int	KANISIX_START_YMD = 20150401;	// 消費税：簡易課税第六業種 施行日	'14.11.11
const	int	TKKZSR_START_YMD = 20151001;	// 消費税：特定課税仕入 施行日		'15.06.24
const	int	PER10_SYZ_START_YMD = 20191001;	// 消費税：１０％施行日				'19.01.21


//日付から消費税の基本税率のサインを返送する
int	GetSyzDateSign(int date)
{
	if( date < PER5_SYZ_START_YMD ) {
		return 0;
	}
	else if( date >= PER5_SYZ_START_YMD && date < PER8_SYZ_START_YMD ) {
		return 1;
	}
	else if( date >= PER8_SYZ_START_YMD && date < PER10_SYZ_START_YMD ) {
		return 2;
	}
	else {
		return 3;
	}
}


DBSUB_FUNC
BOOL WINAPI isBrnInpstr(char* p)
{
	unsigned char* chk;
	chk = (unsigned char*)p;

	while( *chk ) {
		if( *chk == '.' || isdigit(*chk) ) {
			chk++;
			continue;
		}
		else
			return FALSE;
	}
	return TRUE;
}


//カナ複数検索の場合、最後からの数字+.部分を抽出する
DBSUB_FUNC
int WINAPI lastBrnInpStr(LPCTSTR string, char* numstr, int strsize)
{
	int byte = strlen(string);
	::ZeroMemory(numstr, strsize);

	int k, st, c1, n;
	CString nums;

	//SPACE押下
	if( string[0] == ' ' && byte == 1) {
		strcpy_s(numstr, strsize, " " );
		return byte;
	}

	for( k = byte - 1, n = 0; k >= 0; ) {
		st = IsKindOfLetter((LPCTSTR)string, k);
		c1 = (BYTE)string[k];

		switch( st ) {
		case 1:		// ASCII
			if( isdigit(c1) || c1 == '.' ) {	// 数字 OR ピリオド
				nums.Insert(0, (char)c1);
				n++;
				k--;
			}
			else {
				k = -1;	//ループ終了
			}
			break;
		default:
			k = -1;//ループ終了
			break;
		}
	}
	if( n ) {
		sprintf_s(numstr, strsize, "%s", (LPCTSTR)nums);
	}
	return n;
}


// 09.21 /22
// カナ検索タイプを取得する
DBSUB_FUNC
int WINAPI GetKanaSearchType()
{
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;
	int kanatype = -1;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl(utbl);
		CoptionRec*	pRec;
		pRec = mctrl.get_option(utbl.user_name);

		if( pRec != NULL ) {
			char* key = "DBDINP:KanaStype;";
			int len = strlen(key);

			int n = pRec->optionchar.Find(key);

			if( n >= 0 ) {
				char tmp[8] = { 0 };

				int k = 0;
				for( int i = n + len; ; i++ ) {
					if( pRec->optionchar[i] == '*' || pRec->optionchar[i] == '\0' ) {
						break;
					}
					else {
						tmp[k] = pRec->optionchar[i];
						if( ++k >= sizeof tmp ) {
							break;
						}
					}
				}
				kanatype = atoi(tmp);
			}
		}
	}
	//まだ登録なし
	if( kanatype == -1 ) {
		kanatype = 0;
	}

	return kanatype;
}




// インボイス番号カナソート
static
int __cdecl INVINFO_KanaCompare(/*void *context,*/ const void *ele1, const void *ele2)
{
	int ret;

	INVOICE_INFO* dw1, *dw2;

	dw1 = (INVOICE_INFO*)ele1;
	dw2 = (INVOICE_INFO*)ele2;

	ret = memcmp(dw1->inv_tkKana, dw2->inv_tkKana, sizeof dw1->inv_tkKana);
	//カナが同じ場合は、コード順
	if( ret == 0 ) {
		if( dw1->inv_tkNo > dw2->inv_tkNo )
			ret = 1;
		else if( dw1->inv_tkNo < dw2->inv_tkNo )
			ret = -1;
	}

	return ret;
}


int GetInvoiceInfo(CDBZmSub* pZm, CArray<INVOICE_INFO>& InvAry)
{
	// インボイス対応
	INVOICE_INFO invInfo = { 0 };
	CString tkFilter;

	tkFilter = pZm->tkrec->GetFilter();
	pZm->tkrec->Requery(_T("invno IS NOT NULL"), 0);

	struct _AUTOSEL* psel;
	CDinpINI	dini;
	dini.Init(pZm);
	psel = dini.GetDinpSW();

	InvAry.RemoveAll();
	for( int i = 0; ; i++ ) {
		if( i == 0 ) {
			if( (pZm->tkrec->MoveFirst() == -1) || (pZm->tkrec->st == -1) ) {
				break;
			}
		}
		else {
			if( (pZm->tkrec->MoveNext() == -1) || (pZm->tkrec->st == -1) ) {
				break;
			}
		}

		// 初期化
		::ZeroMemory(&invInfo, sizeof(invInfo));

		invInfo.inv_tkNo = pZm->tkrec->tkcod;	// 登録番号
		sprintf_s(invInfo.inv_tkNam, sizeof(invInfo.inv_tkNam), _T("%s"), pZm->tkrec->tkname);	// 摘要名称
		sprintf_s(invInfo.inv_tkKana, sizeof(invInfo.inv_tkKana), _T("%s"), pZm->tkrec->tkana);		// カナ
		sprintf_s(invInfo.inv_invNo, sizeof(invInfo.inv_invNo), _T("%s"), pZm->tkrec->invno);	// 登録番号

		InvAry.Add(invInfo);
	}

	if( psel->RTSWKKANA_OPT == OPT_KANAJUN ) {
		int max = InvAry.GetCount();
//		MyTrace("InvAry OPT_KANAJUN max = %d\n", max );
		if( max > 1 ) {
			qsort(&InvAry[0], InvAry.GetCount(), sizeof INVOICE_INFO, INVINFO_KanaCompare);
		}
	}

	pZm->tkrec->Requery(tkFilter, 0);

	return 0;
}




#define	_A	knsgn[0]			//  KNREC.knsgn[0]
#define	_B	knsgn[1]			//  KNREC.knsgn[1]
#define	_C	knsgn[2]			//  KNREC.knsgn[2]
#define	_D	knsgn[3]			//  KNREC.knsgn[3]
#define	_E	knsgn[4]			//  KNREC.knsgn[4]	D4：免税事業者からの仕入
#define	_F	knsgn[5]			//  KNREC.knsgn[5]


//-----------------------------------------------------------------------------
// 消費税科目？
//-----------------------------------------------------------------------------
// 引数	syattr	：	科目の消費税属性
//-----------------------------------------------------------------------------
// 返送値	1	：	消費税科目
//			0	：	消費税科目でない
//-----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI issykamok(DBKNREC* pKn)
{
	ASSERT(pKn);
	if( (pKn->_A & 0x0f) && (pKn->_C & 0x0f) ) {
		return 1;
	}
	else {
		return 0;
	}
}

//-----------------------------------------------------------------------------
// 資産取得チェック
//-----------------------------------------------------------------------------
// 引数		syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	資産取得
//			FALSE	：	資産取得でない
//-----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI is_sisan_syutok(DBKNREC* pKn)
{
	ASSERT(pKn);
	return((pKn->_A == 0x01) && (pKn->_B == 0x00));
}

//-----------------------------------------------------------------------------
// 資産譲渡チェック
//-----------------------------------------------------------------------------
// 引数		syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	資産譲渡
//			FALSE	：	資産譲渡でない
//-----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI is_sisan_jyoto(DBKNREC* pKn)
{
	ASSERT(pKn);
	return((pKn->_A == 0x01) && (pKn->_B == 0x01));
}

//-----------------------------------------------------------------------------
// 貸倒損失チェック
//-----------------------------------------------------------------------------
// 引数		syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	貸倒損失
//			FALSE	：	貸倒損失でない
//-----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI is_kasitaore(DBKNREC* pKn)
{
	ASSERT(pKn);
	return((pKn->_A == 0x08) && (pKn->_B == 0x03));
}

//-----------------------------------------------------------------------------
// 売上属性以外
//-----------------------------------------------------------------------------
// 引数	syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI uri_attr_igai(DBKNREC* pKn)
{
	ASSERT(pKn);
	return (issykamok(pKn) ? ((pKn->_A & ~0x02) & 0x0f) && !is_sisan_jyoto(pKn) : 0);		// 売上以外　属性の取り出し
}


//-----------------------------------------------------------------------------
// 科目の仕入区分属性
//-----------------------------------------------------------------------------
// 引数	syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
DBSUB_FUNC
int WINAPI sire_attr(DBKNREC* pKn)
{
	ASSERT(pKn);
	int uri_attr_igai_v = uri_attr_igai(pKn);	// 売上以外　属性
	int is_kasitaore_v = is_kasitaore(pKn);		// 貸倒損失チェック

	return is_kasitaore_v ? 0 : uri_attr_igai_v;
}


//仕入科目の仕訳か？SQL
DBSUB_FUNC
void WINAPI sql_sire(CString& sql)
{
	sql = _T("and (dbt IN(select kncod FROM knrec\
			where(cast(substring(knsgn, 1, 1) as int) != 0 and cast(substring(knsgn, 3, 1) as int) != 0) and NOT((cast(substring(knsgn, 1, 1) as int) & 0x0f) = 0x02)\
			and NOT((cast(substring(knsgn, 1, 1) as int) = 8 and cast(substring(knsgn, 2, 1) as int) = 3))\
			and NOT((cast(substring(knsgn, 1, 1) as int) = 1 and cast(substring(knsgn, 2, 1) as int) = 1))\
		)\
		or cre IN(select kncod FROM knrec\
			where(cast(substring(knsgn, 1, 1) as int) != 0 and cast(substring(knsgn, 3, 1) as int) != 0) and NOT((cast(substring(knsgn, 1, 1) as int) & 0x0f) = 0x02)\
			and NOT((cast(substring(knsgn, 1, 1) as int) = 8 and cast(substring(knsgn, 2, 1) as int) = 3))\
			and NOT((cast(substring(knsgn, 1, 1) as int) = 1 and cast(substring(knsgn, 2, 1) as int) = 1))) or dbt = (select SVkcod1 from szvol) or cre = (select SVkcod1 from szvol) )");
}


//決算期間によって 免税事業者からの課税仕入れの控除割り合いのタイプを取得する
//
DBSUB_FUNC
int WINAPI GetMenzeiWariai(CZVOLUME* zvol)
{
	int type;

	type = SYZ_MEN_NOT;

	if( zvol->ee_ymd < SYZMEN2_YMD ) {
		if( zvol->ss_ymd <= SYZMEN1_YMD && SYZMEN1_YMD <= zvol->ee_ymd ) {
			type = SYZ_MEN_80;
		}
		if( type == SYZ_MEN_NOT ) {
			if( zvol->ss_ymd > SYZMEN1_YMD ) {
				type = SYZ_MEN_80;
			}
		}
	}
	if( type == SYZ_MEN_NOT ) {
		if( zvol->ss_ymd <= SYZMEN3_YMD && SYZMEN3_YMD <= zvol->ee_ymd ) {
			type = SYZ_MEN_8050;
		}
		else if( zvol->ss_ymd <= SYZMEN3_YMD && SYZMEN3_YMD > zvol->ee_ymd ) {
			type = SYZ_MEN_80;
		}
		else {
			if( zvol->ss_ymd > SYZMEN4_YMD ) {
				type = SYZ_MEN_NOT;
			}
			else if( zvol->ss_ymd >= SYZMEN3_YMD ) {
				type = SYZ_MEN_50;
			}
		}
	}
	return type;
}


//免税事業者からの課税仕入れ 控除割合文字列取得
//
LPCTSTR GetMenzeiWariaiString(int sel)
{
	static const char* menw_str[] = {
		"80% 控除", "50% 控除",
	};

	if( sel >= 0 && sel < 2 ) {
		return menw_str[sel];
	}
	return NULL;
}


//仕入科目の仕訳か？SQL
// 仕入科目で仕訳区分が入っている仕訳 および 非課税・不課税 は除く
// 売上科目で仕訳区分が仕入・経費等、インボイス登録番号の対象仕訳を含める
//
DBSUB_FUNC
void WINAPI sql_sire_EX(CString& sql)
{
sql = _T("and ( (dbt IN(select kncod FROM knrec\
				where(cast(substring(knsgn, 1, 1) as int) != 0 and cast(substring(knsgn, 3, 1) as int) != 0) and NOT((cast(substring(knsgn, 1, 1) as int) & 0x0f) = 0x02)\
				and NOT((cast(substring(knsgn, 1, 1) as int) = 8 and cast(substring(knsgn, 2, 1) as int) = 3))\
				and NOT((cast(substring(knsgn, 1, 1) as int) = 1 and cast(substring(knsgn, 2, 1) as int) = 1))\
				and (cast(SUBSTRING(dsign, 1, 1) as int) & 0x40 = 0x40 AND cast(SUBSTRING(dsign, 3, 1) as int) & 0x0f = 0x01\
					and cast(SUBSTRING(dsign, 5, 1) as int) & 0xff = 0)\
				)\
			or cre IN(select kncod FROM knrec\
				where(cast(substring(knsgn, 1, 1) as int) != 0 and cast(substring(knsgn, 3, 1) as int) != 0) and NOT((cast(substring(knsgn, 1, 1) as int) & 0x0f) = 0x02)\
				and NOT((cast(substring(knsgn, 1, 1) as int) = 8 and cast(substring(knsgn, 2, 1) as int) = 3))\
				and NOT((cast(substring(knsgn, 1, 1) as int) = 1 and cast(substring(knsgn, 2, 1) as int) = 1))\
				and (cast(SUBSTRING(dsign, 1, 1) as int) & 0x40 = 0x40 AND cast(SUBSTRING(dsign, 3, 1) as int) & 0x0f = 0x01\
					 and cast(SUBSTRING(dsign, 5, 1) as int) & 0xff = 0)\
				)\
				or dbt = (select SVkcod1 from szvol) or cre = (select SVkcod1 from szvol))\
				or ((dbt IN(select kncod FROM knrec where cast(substring(knsgn, 1, 1) as int) & 0x0f = 0x02) or\
					cre IN(select kncod FROM knrec where(cast(substring(knsgn, 1, 1) as int) & 0x0f) = 0x02))\
					and (cast(substring(dsign, 5, 1) as int) & 0xff = 0x15 or cast(substring(dsign, 5, 1) as int) & 0xff = 0x17 or\
						cast(substring(dsign, 5, 1) as int) & 0xff = 0x06 or cast(substring(dsign, 5, 1) as int) & 0xff = 0x08))\
				)" );
}



// 08.10 /23
// 消費税毎回止まる モード取得する
DBSUB_FUNC
int WINAPI GetSyzStopMode()
{
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;
	int syzmode = -1;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl(utbl);
		CoptionRec*	pRec;
		pRec = mctrl.get_option(utbl.user_name);

		if( pRec != NULL ) {
			char* key = "DBDINP:SyzStop;";
			int len = strlen(key);

			int n = pRec->optionchar.Find(key);

			if( n >= 0 ) {
				char tmp[8] = { 0 };

				int k = 0;
				for( int i = n + len; ; i++ ) {
					if( pRec->optionchar[i] == '*' || pRec->optionchar[i] == '\0' ) {
						break;
					}
					else {
						tmp[k] = pRec->optionchar[i];
						if( ++k >= sizeof tmp ) {
							break;
						}
					}
				}
				syzmode = atoi(tmp);
			}
		}
	}
	//まだ登録なし
	if( syzmode == -1 ) {
		syzmode = 0;
	}

	return syzmode;
}

//枝番クリアを-1にするため
void clear_TKYV2_SAVE(TKYV2_SAVE* psv)
{
	::ZeroMemory(psv, sizeof TKYV2_SAVE);

#ifndef CLOSE // 24/9/13 と 24/9/30 で出荷が別れたので閉じる
		     // 24/9/30 で有効にする -> 10/31 アップ分で有効
		     // 24/9/30 は 25年03月に延期
	psv->debBrn = psv->creBrn = -1;
#endif
}

DBSUB_FUNC
void WINAPI func_invketachk(UINT idc, CWnd* pwnd, LPCTSTR imestring)
{
	VARIANT var;

	DBdata_get(pwnd, idc, &var, ICSDBEDT_TYPE_STRING, 0);
	int len = strlen((LPCTSTR)var.pbVal);

	int len2 = strlen(imestring);

	if( len2 + len == 13 ) {
		CICSDBEDT* pDedt;
		pDedt = (CICSDBEDT*)(pwnd->GetDlgItem(idc));
		int pos = pDedt->GetCaretPosition();
		var.pbVal = (BYTE*)imestring;
		pDedt->InsertData(&var, ICSDBEDT_TYPE_STRING, 0, pos);
	}
	else {
		var.pbVal = (BYTE*)imestring;
		DBdata_set(pwnd, idc, &var, ICSDBEDT_TYPE_STRING, 0);
	}
}

//
//	DBEdit のフォント と文字数から、表示するサイズを計算する
//
//
//
#include "font.h"
DBSUB_FUNC
CSize WINAPI DBEditTextSize(UINT dbedtID, CWnd* pwnd, int bytes)
{
	CString buf;
	LPSTR p = buf.GetBuffer( bytes + 10);
	for( int i = 0; i < bytes; i++ ) {
		p[i] = 'A';
	}
	p[bytes] = '\0';
	buf.ReleaseBuffer();

	CWnd* edtwnd = pwnd->GetDlgItem(dbedtID);
	if( edtwnd == NULL ) {
		CClientDC dc(pwnd);
		CSize sz;
		sz = dc.GetTextExtent(buf);
		return sz;
	}

	CFont font;
	COleFont ofont = ((CICSDBEDT*)edtwnd)->GetFont();
	CY cy = ofont.GetSize();

	LOGFONT lf = { 0 };
	lf.lfHeight = (LONG)(cy.int64 / 1000);
	lf.lfCharSet = DEFAULT_CHARSET;
	strcpy_s(lf.lfFaceName, sizeof lf.lfFaceName, ofont.GetName());

	font.CreatePointFontIndirect(&lf);

	CClientDC dc( pwnd );
	CSize sz;
	CFont* pOld;
	pOld = dc.SelectObject(&font);
	sz = dc.GetTextExtent(buf);

	dc.SelectObject(pOld);

	return sz;
}



//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CDBipDataRec

#define DIP_BASIZE	16
#define DIP_2BSIZE	2


CDBINPDataRec::CDBINPDataRec()
{
	// -1 で データなし
	m_seq = -1;
	m_dsign.SetSize(DIP_BASIZE);
	m_tsign.SetSize(DIP_2BSIZE);

	// インボイス対応
	memset(m_invno, '\0', sizeof(m_invno));
}


CDBINPDataRec::~CDBINPDataRec()
{
}


void CDBINPDataRec::Reset()
{
	m_seq = -1;
	m_ksign = 0;
	m_ddate = -1;
	m_mofs = 0;
	m_cno = -1;
	m_dbmn = -1;
	m_dkno.Empty();
	m_dbt.Empty();
	m_dbr = -1;

	m_cbmn = -1;
	m_ckno.Empty();
	m_cre.Empty();
	m_cbr = -1;
	::ZeroMemory(m_val, sizeof m_val);
	::ZeroMemory(m_zei, sizeof m_zei);

	if( m_dsign.GetSize() == DIP_BASIZE )
		memset(&m_dsign[0], 0, DIP_BASIZE);
	else {
		m_dsign.RemoveAll();
		m_dsign.SetSize(DIP_BASIZE);
	}
	m_owner = -1;
	m_dtype = 0;

	m_tekiyo.Empty();
	m_imgsq = -1;
	m_snumber.Empty();
	m_tdate = -1;
	m_edate = -1;
	m_udate = -1;
	m_icno = -1;

	m_skn.Empty();
	m_tag.Empty();

	if( m_tsign.GetSize() == DIP_2BSIZE )
		memset(&m_tsign[0], 0, DIP_2BSIZE);
	else {
		m_tsign.RemoveAll();
		m_tsign.SetSize(DIP_2BSIZE);
	}

	m_hjiyu = 0;
	::ZeroMemory(m_taika, sizeof m_taika);

	m_nbcd = -1;

	// インボイス対応
	memset(m_invno, '\0', sizeof(m_invno));
	m_rnumber.Empty();
}


// 代入演算子のオーバーロード
void CDBINPDataRec::operator=(const CDBINPDataRec& data)
{
	if( this != &data ) {
		m_seq = data.m_seq;
		m_ksign = data.m_ksign;
		m_ddate = data.m_ddate;
		m_mofs = data.m_mofs;
		m_cno = data.m_cno;
		m_dbmn = data.m_dbmn;
		m_dkno = data.m_dkno;
		m_dbt = data.m_dbt;
		m_dbr = data.m_dbr;

		m_cbmn = data.m_cbmn;
		m_ckno = data.m_ckno;
		m_cre = data.m_cre;
		m_cbr = data.m_cbr;

		memcpy(m_val, data.m_val, sizeof m_val);
		memcpy(m_zei, data.m_zei, sizeof m_zei);

		m_dsign.Copy(data.m_dsign);
		m_owner = data.m_owner;
		m_dtype = data.m_dtype;

		m_tekiyo = data.m_tekiyo;
		m_imgsq = data.m_imgsq;
		m_snumber = data.m_snumber;
		m_tdate = data.m_tdate;
		m_edate = data.m_edate;
		m_udate = data.m_udate;
		m_icno = data.m_icno;

		m_skn = data.m_skn;
		m_tag = data.m_tag;
		m_tsign.Copy(data.m_tsign);

		m_hjiyu = data.m_hjiyu;
		memcpy(m_taika, data.m_taika, sizeof m_taika);

		m_nbcd = data.m_nbcd;

		// インボイス対応
		memcpy(m_invno, data.m_invno, sizeof(m_invno));
		m_rnumber = data.m_rnumber;
	}
}


BOOL CDBINPDataRec::operator==(const CDBINPDataRec& data)
{
	//	int size, i;
	INT_PTR	size, i;
	BOOL ret = TRUE;

	if( this == &data )	return TRUE;
	else {
		// CByteArray の比較
		size = m_dsign.GetSize();
		if( data.m_dsign.GetSize() == size ) {
			for( i = 0; i < size; i++ ) {
				if( m_dsign[i] != data.m_dsign[i] ) {
					ret = FALSE;
					break;
				}
			}
		}
		else	ret = FALSE;
		if( !ret )	return FALSE;

		size = m_tsign.GetSize();
		if( data.m_tsign.GetSize() == size ) {
			for( i = 0; i < size; i++ ) {
				if( m_tsign[i] != data.m_tsign[i] ) {
					ret = FALSE;
					break;
				}
			}
		}
		else	ret = FALSE;
		if( !ret )	return FALSE;

		if( m_seq == data.m_seq &&
			m_ksign == data.m_ksign &&
			m_ddate == data.m_ddate &&
			m_mofs == data.m_mofs &&
			m_cno == data.m_cno &&
			m_dbmn == data.m_dbmn &&
			m_dkno == data.m_dkno &&
			m_dbt == data.m_dbt &&
			m_dbr == data.m_dbr &&

			m_cbmn == data.m_cbmn &&
			m_ckno == data.m_ckno &&
			m_cre == data.m_cre &&
			m_cbr == data.m_cbr &&

			!memcmp(m_val, data.m_val, sizeof m_val) &&
			!memcmp(m_zei, data.m_zei, sizeof m_zei) &&

			m_owner == data.m_owner &&
			m_dtype == data.m_dtype &&

			m_tekiyo == data.m_tekiyo &&
			m_imgsq == data.m_imgsq &&
			m_snumber == data.m_snumber &&
			m_tdate == data.m_tdate &&
			m_edate == data.m_edate &&
			m_udate == data.m_udate	&&
			m_icno == data.m_icno	&&
			m_skn == data.m_skn &&
			m_tag == data.m_tag &&
			m_hjiyu == data.m_hjiyu &&
			!memcmp(m_taika, data.m_taika, sizeof m_taika) &&
			m_nbcd == data.m_nbcd &&
			!memcmp(m_invno, data.m_invno, sizeof(m_invno)) && // インボイス対応
			m_rnumber == data.m_rnumber //債権管理番号
			) {

			;
		}
		else {
			ret = FALSE;
		}
	}
	return ret;
}

BOOL CDBINPDataRec::operator!=(const CDBINPDataRec& data)
{
	int size, i;
	BOOL ret = FALSE;

	if( this == &data )	return FALSE;
	else {
		// CByteArray の比較
		size = m_dsign.GetSize();
		if( data.m_dsign.GetSize() == size ) {
			for( i = 0; i < size; i++ ) {
				if( m_dsign[i] != data.m_dsign[i] ) {
					ret = TRUE;
					break;
				}
			}
		}
		else	ret = TRUE;
		if( ret )	return TRUE;

		size = m_tsign.GetSize();
		if( data.m_tsign.GetSize() == size ) {
			for( i = 0; i < size; i++ ) {
				if( m_tsign[i] != data.m_tsign[i] ) {
					ret = TRUE;
					break;
				}
			}
		}
		else	ret = TRUE;
		if( ret )	return TRUE;

		if( m_seq == data.m_seq &&
			m_ksign == data.m_ksign &&
			m_ddate == data.m_ddate &&
			m_mofs == data.m_mofs &&
			m_cno == data.m_cno &&
			m_dbmn == data.m_dbmn &&
			m_dkno == data.m_dkno &&
			m_dbt == data.m_dbt &&
			m_dbr == data.m_dbr &&

			m_cbmn == data.m_cbmn &&
			m_ckno == data.m_ckno &&
			m_cre == data.m_cre &&
			m_cbr == data.m_cbr &&

			!memcmp(m_val, data.m_val, sizeof m_val) &&
			!memcmp(m_zei, data.m_zei, sizeof m_zei) &&

			m_owner == data.m_owner &&
			m_dtype == data.m_dtype &&

			m_tekiyo == data.m_tekiyo &&
			m_imgsq == data.m_imgsq &&
			m_snumber == data.m_snumber &&
			m_tdate == data.m_tdate &&
			m_edate == data.m_edate &&
			m_udate == data.m_udate	&&
			m_icno == data.m_icno	&&
			m_skn == data.m_skn &&
			m_tag == data.m_tag &&
			m_hjiyu == data.m_hjiyu &&
			!memcmp(m_taika, data.m_taika, sizeof m_taika) &&
			m_nbcd == data.m_nbcd &&
			!memcmp(m_invno, data.m_invno, sizeof(m_invno)) &&// インボイス対応
			m_rnumber == data.m_rnumber //債権管理番号

			) {

			ret = FALSE;
		}
		else {
			ret = TRUE;
		}
	}
	return ret;
}


//古いデータクラスから新しいデータクラスに変換
void convNewDataRec(CDBipDataRec* old, CDBINPDataRec* newdata)
{
	newdata->m_seq = old->m_seq;
	newdata->m_ksign = old->m_ksign;
	newdata->m_ddate = old->m_ddate;
	newdata->m_mofs = old->m_mofs;
	newdata->m_cno = old->m_cno;
	newdata->m_dbmn = old->m_dbmn;
	newdata->m_dkno = old->m_dkno;
	newdata->m_dbt = old->m_dbt;
	newdata->m_dbr = old->m_dbr;

	newdata->m_cbmn = old->m_cbmn;
	newdata->m_ckno = old->m_ckno;
	newdata->m_cre = old->m_cre;
	newdata->m_cbr = old->m_cbr;

	memcpy(newdata->m_val, old->m_val, sizeof newdata->m_val);
	memcpy(newdata->m_zei, old->m_zei, sizeof newdata->m_zei);

	newdata->m_dsign.Copy(old->m_dsign);
	newdata->m_owner = old->m_owner;
	newdata->m_dtype = old->m_dtype;

	newdata->m_tekiyo = old->m_tekiyo;
	newdata->m_imgsq = old->m_imgsq;
	newdata->m_snumber = old->m_snumber;
	newdata->m_tdate = old->m_tdate;
	newdata->m_edate = old->m_edate;
	newdata->m_udate = old->m_udate;
	newdata->m_icno = old->m_icno;

	newdata->m_skn = old->m_skn;
	newdata->m_tag = old->m_tag;
	newdata->m_tsign.Copy(old->m_tsign);

	newdata->m_hjiyu = old->m_hjiyu;
	memcpy(newdata->m_taika, old->m_taika, sizeof newdata->m_taika);

	newdata->m_nbcd = old->m_nbcd;

	// インボイス対応
	memcpy(newdata->m_invno, old->m_invno, sizeof(newdata->m_invno));
}
