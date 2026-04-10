/*********************************************

		file name	:	subfnc.cpp
		date		:	9.3 /99

				共通関数


**********************************************/
#include "stdafx.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************

		RepKeyDel()

	リピートキー削除
		->	0 ... なし
			1 ... あり
****************************************/
#ifdef RPTKEYDEL	// 09.04 /99
int RepKeyDel( tagMSG* pMsg )
{
	static int delkey = 0;
	tagMSG msg;

	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam != VK_PROCESSKEY )
		{
			if( delkey )
			{
				if( delkey < 2 )
					return 1;
				else
					--delkey;
			}
			else if( ::PeekMessage( &msg, NULL, WM_KEYUP, WM_KEYUP, PM_NOREMOVE ) )
			{
				if( msg.wParam != VK_PROCESSKEY )
				{
					delkey = 2;
				}
			}
		}
	}
	else if( pMsg->message == WM_KEYUP )
	{
		if( pMsg->wParam != VK_PROCESSKEY )
		{
			delkey = 0;
		}
	}
	return 0;
}
#endif

/**************************************

		isEntrySys()

	エントリーシステム？
		→	TRUE ... Yes
			FALSE .. No
***************************************/
BOOL isEntrySys()
{
#ifdef CLOSE	// 09.21 /99
	return (PCOM_GetInt( _T("EntrySys"), 0 ) ? TRUE : FALSE);
#else
	return FALSE;
#endif
}


/*************************************

		Buzzer()

		ブザー
				10.16 /99
*************************************/
void Buzzer()
{
	MessageBeep(0xffffffff);
}

void Buzzer2()
{
	Buzzer();

	AfxMessageBox("ブザーを鳴らしました。");
}

#ifdef BIGIMG	// 03.16 /00
/************************************

		img_trim()

		イメージデータカット
************************************/
#define TEMP_SIZE (10*1024)	// 念のためサイズを大きくとる

static unsigned char pBUF[TEMP_SIZE];

/*..... img_trim() ...........*/
unsigned char *img_trim(IMREC *des, IMREC *sou, long x, long y)
{
	int n;
	long max_xbyte;
	long xbyte, nbyte, cutbyte;
	unsigned char *sp;
	unsigned char *dp;

	xbyte = ((sou->dotxs + 15) / 16) * 2;	// 転送元、横バイト数

	nbyte = (long)floor(((double)sou->dotys * (double)x / (double)y + 15.0) / 16.0) * 2L;	// 必要バイト数
	max_xbyte = (((TEMP_SIZE / sou->dotys) + 1) / 2) * 2;	// 転送先、最大横バイト数
	nbyte = max_xbyte < nbyte ? max_xbyte : nbyte;	// 転送先あふれの防止

	sp = sou->imdata;
	dp = pBUF;
	memset(dp, '\0', TEMP_SIZE);	// イメージエリアクリア

	cutbyte = xbyte < nbyte ? xbyte : nbyte;
	for( n = 0; n < sou->dotys; ++n, sp += xbyte )
	{
		memmove(dp + nbyte * n, sp, cutbyte);
	}
	*des = *sou;

	des->dotxs = nbyte * 8;
	des->img_l = nbyte * sou->dotys;

	return dp;
}
#endif



int	date_devide(int date, int* mm, int* dd)
{
	int wk_date = (date % 10000);

	*mm = wk_date / 100;
	*dd = wk_date % 100;

	return 0;
}









