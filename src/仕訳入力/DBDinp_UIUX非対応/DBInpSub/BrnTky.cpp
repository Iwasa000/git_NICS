/*****************************************************

		枝番摘要モジュール

	BrnTky.cpp	: インプリメンテーション ファイル

		File	:	BrnTky.h BrnTky.cpp
		Date	:	07.20 /05

******************************************************/
#include "stdafx.h"

#include "stdio.h"

#include "DBInpSubFunc.h"

#include "BrnTky.h"
#include "SyogBrn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// 枝番摘要表示文字色
#define BRTEK_ForeCOL		RGB(   0,   0, 255)		//RGB(237, 54, 22)

#define	BRNAM_MAX		20	//枝番名称MAX
extern
int get_strcnt(char* str, int byte);


// 何回も Requery をするのは 無駄なので、フラグで区別する
// 摘要を探す場合は、以下の関数を使用する
static BOOL bEzQuery = FALSE;

BOOL& ezquery_flag()
{
	return bEzQuery;
}

void ezdb_init( CDBZmSub* pZm )
{
	//CString	pbuf;
	//pbuf.Format( "ezdb_init bEzQuery = %d filter=%s", bEzQuery, pZm->ezrec->GetFilter() );
	//MyTrace(pbuf);
	//AfxMessageBox( pbuf );

	if( ezquery_flag() || strlen( (LPCTSTR)(pZm->ezrec->GetFilter()) ) ) {
		pZm->ezrec->Requery( "", 1 );
		ezquery_flag() = FALSE;
	}
}

//========================================
// 昇順並びソート比較(50音＋枝番)
//========================================
static int compare( const void *arg1, const void *arg2 )
{
	BRN_REC *p1, *p2;

	p1 = (BRN_REC*)arg1;
	p2 = (BRN_REC*)arg2;
//	return l_jsgn( &p1->kn50, &p2->kn50, (unsigned long)&p2->tkno - (unsigned long)&p2->kn50 ); 

	CString kana1, kana2;
	kana1 = p1->kn50;
	kana1.TrimRight();
	kana2 = p2->kn50;
	kana2.TrimRight();
	int rval = kana1.Compare(kana2);

	if( rval == 0 ) {
		if( p1->brn < p2->brn )	rval = -1;
		else {
			rval = 1;
		}
	}

	return rval;
}


//========================================
// 昇順並びソート比較(枝番順)
//========================================
static int compareBrn(const void *arg1, const void *arg2)
{
	BRN_REC *p1, *p2;

	p1 = (BRN_REC*)arg1;
	p2 = (BRN_REC*)arg2;

	int rval;
	if( p1->brn < p2->brn )	rval = -1;
	else {
		rval = 1;
	}

	return rval;
}



/////////////////////////////////////////////////////////////////////////////
// CBrnTkySub
// 内部の処理を行うクラス
//
class CBrnTkySub
{
public:
	CBrnTkySub();
	virtual ~CBrnTkySub();

public:
	void sub_init( CDBZmSub* Zm );
	void sub_close();
	int	sub_free();

	void tbl_free( BRN_TB *tbl );
	int tbl_realoc( BRN_TB *tbl, long max, long n );

	void tbl_free2( BRN_TB *tbl );

	BRN_NAME* _get_brtekmsg( int bmn, CString& code, int brn, BOOL full, int mode );
	BOOL	_get_brtek( BRN_REC *ptkrec, int bmn, CString& code, int brn );
	int		_get_brtek( int bmn, CString& code, int brn );

	BOOL	_get_brtektb( BRN_TB **tbl, int bmn, CString& code, CString& kana, int page_sz, int page, BOOL full, int mode );

	BOOL&	IsKmkcodeDisp();
	void	SetSyogKmkcodeDisp( BOOL bKcod );
	void	set_tkquery( BOOL bQuery );
	void	set_brnKanaJun( BOOL bKanaJun );

protected:
	void	inz();
	BRN_TB	*mk_brtektb( int bmn, CString& code, CString& kana, BOOL full = TRUE );

	BOOL	BrnKanaCheck( CString& inputkana, CString& reckana );
	int		DB_TekiyoNoRead( BRN_REC* prec, int no );
	int		DB_EdabanMasterSearch( int bmn, CString& code, int brn );

	void	make_nameformat( char* fmt, int fmtsize, LPCTSTR brnName );
// アトリビュート
protected:
	// テーブル関係
	int		m_REC_MAX;
	BRN_TB	m_BRN_TBL, m_BRN_PAGE;
	BOOL	m_tkquery;
	// 科目コード表示
	BOOL	m_bKmcode;

	CDBZmSub*	m_pZm;

	// 資金繰諸口枝番
	CSyogBrn SyogBrn;

	BOOL	m_bKanaJun;

	int		m_TKYO_LEN;
};


CBrnTkySub::CBrnTkySub()
{
	inz();
	m_tkquery = FALSE;

	IsKmkcodeDisp() = FALSE;

	m_TKYO_LEN = TKYO_LEN;

	m_bKanaJun = FALSE;
}


CBrnTkySub::~CBrnTkySub()
{
	tbl_free( &m_BRN_TBL );
}

// 科目コードを表示するか？
BOOL& CBrnTkySub::IsKmkcodeDisp()
{
	return m_bKmcode;
}

// 諸口枝番モジュールに科目コード通知
void	CBrnTkySub::SetSyogKmkcodeDisp( BOOL bKcod )
{
	SyogBrn.IsKmkcodeDisp() = bKcod;
	//諸口テーブル再作成
	SyogBrn.make_syogbrn();
}


// 摘要クエリーサイン をセット
// 他で、摘要のクエリーをした場合。
void CBrnTkySub::set_tkquery( BOOL bQuery )
{
	m_tkquery = bQuery;
}


/*============================	
	  inz()
	変数イニシャライズ
  ============================*/
void CBrnTkySub::inz()
{
	m_REC_MAX = 0;
//	memset( &m_BRN_TBL, 0, sizeof(m_BRN_TBL) );
//	memset( &m_BRN_PAGE, 0, sizeof(m_BRN_PAGE) );

	m_BRN_TBL.bmn = -1;
	m_BRN_TBL.code.Empty();
	m_BRN_TBL.brn_kn50.Empty();
	m_BRN_TBL.ForeCol = 0;
	m_BRN_TBL.full = 0;	
	m_BRN_TBL.page_max = 0;
	m_BRN_TBL.page_now = 0;
	m_BRN_TBL.page_recn = 0;
	m_BRN_TBL.rec_n = 0;
	m_BRN_TBL.pBRN_REC = NULL;

	m_BRN_PAGE.bmn = -1;
	m_BRN_PAGE.code.Empty();
	m_BRN_PAGE.brn_kn50.Empty();
	m_BRN_PAGE.ForeCol = 0;
	m_BRN_PAGE.full = 0;	
	m_BRN_PAGE.page_max = 0;
	m_BRN_PAGE.page_now = 0;
	m_BRN_PAGE.page_recn = 0;
	m_BRN_PAGE.rec_n = 0;
	m_BRN_PAGE.pBRN_REC = NULL;

	// 表示文字色
	m_BRN_TBL.ForeCol = BRTEK_ForeCOL;

}


// 初期化処理
void CBrnTkySub::sub_init( CDBZmSub* pZm )
{
	m_pZm = pZm;
	// 諸口枝番イニシャライズ
	SyogBrn.syogbrn_iniz(pZm);

	// 会社切り替えで前の枝番が残っている。
	tbl_free2( &m_BRN_TBL );

	int len = GetTekiyoLength( pZm );
	if( len > 0 )	m_TKYO_LEN = len;
}

// 
// 諸口枝番 レコード開放しないと メモリーリークする
void CBrnTkySub::sub_close()
{
	SyogBrn.syogbrn_free();
}


//
// 同一条件で再作成するために現在情報をクリア
//
int	CBrnTkySub::sub_free()
{

	tbl_free2( &m_BRN_TBL );
#ifdef OLD_CLOSE
	tbl_free( &m_BRN_TBL );
	m_BRN_TBL.bmn = -1;
	m_BRN_TBL.code.Empty();
	m_BRN_TBL.brn_kn50.Empty();

	size_t ofs1, ofs2;
	ofs1 = offsetof( BRN_TB, ForeCol );
	ofs2 = offsetof( BRN_TB, pBRN_REC );
	memset( &m_BRN_TBL.ForeCol, '\0', ofs2-ofs1 );
#endif

	// 諸口枝番 フリー
	SyogBrn.syogbrn_free();
	SyogBrn.syogbrn_iniz( m_pZm );

	return 0;
}

/*============================
	テーブルフリー
  ============================*/
void CBrnTkySub::tbl_free( BRN_TB *tbl )
{
	if( tbl->pBRN_REC )
		delete []tbl->pBRN_REC;
	
	tbl->pBRN_REC = NULL;
}

// 05.11 /12
void CBrnTkySub::tbl_free2( BRN_TB *tbl )
{
	tbl_free( tbl );

	tbl->bmn = -1;
	tbl->code.Empty();
	tbl->brn_kn50.Empty();

	size_t ofs1, ofs2;
	ofs1 = offsetof( BRN_TB, ForeCol );
	ofs2 = offsetof( BRN_TB, pBRN_REC );

	memset( &tbl->ForeCol, '\0', ofs2-ofs1 );
}


/*============================
	テーブル アロケート
	-> テーブルマックス
  ============================*/
int CBrnTkySub::tbl_realoc( BRN_TB *tbl, long max, long n )
{
	int ret;

	if( max != n || tbl->pBRN_REC == NULL )
	{
		tbl_free( tbl );

		if( tbl->pBRN_REC = new BRN_REC[n] )
		{
			ret = n;
		}
		else
			ret = 0;
	}
	else
		ret = max;

	tbl->code.Empty();

	size_t ofs1, ofs2;
	ofs1 = offsetof( BRN_TB, ForeCol );
	ofs2 = offsetof( BRN_TB, pBRN_REC );

	memset( &tbl->ForeCol, '\0', ofs2-ofs1 );

	return ret;
}


BRN_NAME* CBrnTkySub::_get_brtekmsg( int bmn, CString& code, int brn, BOOL full, int mode )
{
	BRN_REC brnrec;
	int i;
	char ret_txt[128], buf[128];
	static BRN_NAME ret_name;

	if( isSYOGT_CODE( code ) && (mode & BT_SYOGTBRN_TEK) )
		return SyogBrn.get_syogbrnmsg( code, brn, full );	// 資金繰諸口枝番

	if( mode & BT_NOMALBRN_TEK )
	{
		// * 枝番to摘要レコード
		if( _get_brtek( &brnrec, bmn, code, brn ) )
		{
			if( full )
			{
				// 科目名称
				struct	_DBKNREC* pKn;
				if( pKn = DB_PjisToKnrec( code, m_pZm, TRUE ) ) {
					if( m_bKmcode ) {
						i = kmkcode_string( 2, pKn, ret_txt, sizeof ret_txt );
					}
					else {
						sprintf_s( buf, sizeof buf, "%.14s", pKn->knnam );
						i = ksp_cut( ret_txt, buf, 14 );	// スペースカット
					}
				}
				else
					i = 0;
				ret_txt[i++] = '.';
			}
			else
				i = 0;


			char fmt[16] = {0};
			make_nameformat( fmt, sizeof fmt,  brnrec.brmsg );

			sprintf_s( &ret_txt[i], sizeof ret_txt - i, fmt, brnrec.brmsg );

			ret_name.Name = ret_txt;
			ret_name.ForeColor = BRTEK_ForeCOL;	// 表示文字色

			return &ret_name;
		}
	}


	return NULL;	// Not Found

}


BOOL CBrnTkySub::_get_brtek( BRN_REC *prec, int bmn, CString& code, int brn )
{

	if( !isSYOGT_CODE( code ) && m_pZm->zvol && (m_pZm->zvol->br_sw & 0x01) )	// * 枝番マスター？
	{
		if( DB_EdabanMasterSearch( bmn, code, brn ) != 0 )
			return FALSE;

		if( bmn == -1 ) {
			if( m_pZm->ezrec->eztcd ) {
				// 摘要辞書リード
				if( DB_TekiyoNoRead( prec, m_pZm->ezrec->eztcd ) )
				{
					// 摘要Ａｌｌスペースチェック
					if( kjlen( (char*)(LPCTSTR)prec->brmsg, prec->brmsg.GetLength() ) )
						return TRUE;
				}
			}
			else {
				prec->kn50.Empty();
				prec->brn	= brn;
				prec->tkno	= 0;
				prec->brmsg	= m_pZm->ezrec->ezname;

				if( kjlen( (char*)(LPCTSTR)prec->brmsg, prec->brmsg.GetLength() ) )
					 return TRUE;
			}
		}
		else {
			if( ! (m_pZm->zvol->sub_sw & 0x02) )
				return FALSE;

			if( m_pZm->bmezrec->eztcd ) {
				// 摘要辞書リード
				if( DB_TekiyoNoRead( prec, m_pZm->bmezrec->eztcd ) )
				{
					// 摘要Ａｌｌスペースチェック
					if( kjlen( (char*)(LPCTSTR)prec->brmsg, prec->brmsg.GetLength() ) )
						return TRUE;
				}
			}
			else {
				prec->kn50.Empty();
				prec->brn	= brn;
				prec->tkno	= 0;
				prec->brmsg	= m_pZm->bmezrec->ezname;

				if( kjlen( (char*)(LPCTSTR)prec->brmsg, prec->brmsg.GetLength() ) )
					 return TRUE;
			}		
		}
	}
	return FALSE;
}

int CBrnTkySub::_get_brtek( int bmn, CString& code, int brn )
{
	int ret = 0;

	if( !isSYOGT_CODE( code ) && m_pZm->zvol && (m_pZm->zvol->br_sw & 0x01) )	// * 枝番マスター？
	{
		if( DB_EdabanMasterSearch( bmn, code, brn ) != 0 )
			return -1;

		if( bmn == -1 ) {
			if( m_pZm->ezrec->eztcd ) {
				ret = m_pZm->ezrec->eztcd;
			}
			else if( ! m_pZm->ezrec->ezname.IsEmpty() )
				ret = -1;
		}
		else {
			if( ! (m_pZm->zvol->sub_sw & 0x02) )
				return -1;

			if( m_pZm->bmezrec->eztcd ) {
				ret = m_pZm->bmezrec->eztcd;
			}
			else if( ! m_pZm->bmezrec->ezname.IsEmpty() )
				ret = -1;
		}
	}
	return ret;
}


BOOL CBrnTkySub::_get_brtektb( BRN_TB **tbl, int bmn, CString& code, CString& kana, int page_sz, int page, BOOL full, int mode )
{
	int ofs;
	BRN_TB *ptbl;
	BOOL ret = FALSE;

//	int cmp = strcmp((LPCTSTR)kana, (LPCTSTR)m_BRN_TBL.brn_kn50);
//FILETrace( "_get_brtektb code = %s,%s, kana = %s,%s, cmp(%d)\n", code, m_BRN_TBL.code, kana, m_BRN_TBL.brn_kn50, cmp );
//TRACE( "^^^^ kana.IsEmpty() %d, length %d\n", kana.IsEmpty(), kana.GetLength() );

	if( isSYOGT_CODE( code ) && (mode & BT_SYOGTBRN_TEK) )
		ptbl = SyogBrn.get_syogbrtb( full );	// 資金繰諸口枝番テーブル作成
	else if( !isSYOGT_CODE( code ) && (mode & BT_NOMALBRN_TEK) ) {
		if( strcmp( (LPCTSTR)code, (LPCTSTR)m_BRN_TBL.code ) == 0 && 
			strcmp( (LPCTSTR)kana, (LPCTSTR)m_BRN_TBL.brn_kn50 ) == 0 && bmn == m_BRN_PAGE.bmn )
			ptbl = &m_BRN_TBL;
		else 
			ptbl = mk_brtektb( bmn, code, kana, full );	// 部門枝番摘要名称テーブル(m_BRN_TBL)作成
	}
	else
		return FALSE;

	if( ptbl )
	{
		// 該当ページテーブル(m_BRN_PAGE)作成
		if( page_sz <= 0 )	// All Table
		{
			page_sz = ptbl->rec_n;
		}
		m_BRN_PAGE.page_max = (ptbl->rec_n + page_sz - 1) / page_sz;	// 最大ページ

		if( page < 0 )	// アンダーフロー
			page = 0;
		else if( page >= m_BRN_PAGE.page_max )	// オーバーフロー
			page = m_BRN_PAGE.page_max - 1;
		else			// OK
			ret = TRUE;

		ofs = page * page_sz;

	//	memmove( &m_BRN_PAGE, ptbl, (unsigned long)&ptbl->pBRN_REC - (unsigned long)ptbl );
		m_BRN_PAGE.bmn = ptbl->bmn;
		m_BRN_PAGE.code = ptbl->code;

		size_t ofs1, ofs2;
		ofs1 = offsetof( BRN_TB, ForeCol );
		ofs2 = offsetof( BRN_TB, pBRN_REC );

		memmove( &m_BRN_PAGE.ForeCol, &ptbl->ForeCol, ofs2-ofs1 );

		m_BRN_PAGE.rec_n = (ofs + page_sz < ptbl->rec_n ? page_sz : ptbl->rec_n - ofs); 
		m_BRN_PAGE.pBRN_REC = ptbl->pBRN_REC + ofs;

		m_BRN_PAGE.page_recn = page_sz;		// 1ページ分のレコード数
		m_BRN_PAGE.page_now = page + 1;		// 現在ページ
		m_BRN_PAGE.ForeCol = ptbl->ForeCol;	// 表示文字色
		// カナ
		m_BRN_PAGE.brn_kn50	= ptbl->brn_kn50;
	}
	*tbl = &m_BRN_PAGE;
	return ret;

}


int CBrnTkySub::DB_EdabanMasterSearch( int bmn, CString& code, int brn )
{
	CString filter;

	if( brn == -1 )
		return -1;

	if( bmn == -1 ) {
/*		filter.Format( "ezkcd = '%s' AND ezecd = %d", code, brn );

		if( m_pZm->ezrec->Requery( filter, 1 ) == 0 ) {
			return 0;
		}*/
		ezdb_init( m_pZm );
		if( m_pZm->ezrec->Find( code, brn ) == 1 ) return 0;
	}
	else {
		filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", bmn, code, brn );

		if( ! (m_pZm->zvol->sub_sw & 0x02) )
			return -1;

		if( m_pZm->bmezrec->Requery( filter, 1 ) == 0 ) {
			return 0;
		}	
	}

	return -1;
}


/***********************************
		mk_brtektb()

		枝番摘要名称テーブル(m_BRN_TBL)作成

		->	Table Adddress
************************************/
BRN_TB *CBrnTkySub::mk_brtektb( int bmn, CString& code, CString& kana, BOOL full )
{
	int i;
	int n = 0;
	BRN_REC brnrec;
	CString filter;
	char knam_txt[128], edacd_txt[20], buf[128];
	char fmt[16];
	int	j, cnt;

#ifdef OLD_CLOSE	// 常に検索
TRACE( "----mk_brtektb top bmn %d, kana.IsEmpty() %d, length %d\n", bmn, kana.IsEmpty(), kana.GetLength() );

	if(	m_BRN_TBL.bmn		== bmn	&&		// 部門コード
		m_BRN_TBL.code		== code &&		// 科目コード
		m_BRN_TBL.brn_kn50	== kana &&		// 50音
		m_BRN_TBL.full		== full )		// 科目名称も
		return &m_BRN_TBL;
#endif

//FILETrace( "@mk_brtektb code = %s\n", code );

//	memset( &m_BRN_PAGE, '\0', sizeof( m_BRN_PAGE ) );	// ページアクセステーブルもイニシャライズ
	m_BRN_PAGE.bmn = -1;
	m_BRN_PAGE.code.Empty();
	m_BRN_PAGE.brn_kn50.Empty();
	size_t ofs1, ofs2;
	ofs1 = offsetof( BRN_TB, ForeCol );
	ofs2 = offsetof( BRN_TB, pBRN_REC );

	memset( &m_BRN_PAGE.ForeCol, '\0', ofs2-ofs1 );

	if( code.IsEmpty() || ! m_pZm->zvol || !(m_pZm->zvol->br_sw&0x01))
		return NULL;

	int ed_max = 0;

	if( bmn != -1 ) {
		// 部門枝番
		if( ! (m_pZm->zvol->sub_sw & 0x02) )
			return NULL;

		// テーブルイニシャライズ
		filter = _T( "seq = 13" );	//bmezrec
		m_pZm->dcntl->Requery(filter);
		if( m_pZm->dcntl->st != -1 ) {
			ed_max = m_pZm->dcntl->mcnt;
		}

		m_REC_MAX = tbl_realoc( &m_BRN_TBL, m_REC_MAX, ed_max );	// 枝番登録数分
		m_BRN_TBL.ForeCol = BRTEK_ForeCOL;	// 表示文字色
		m_BRN_TBL.code = code;

		if( !isSYOGT_CODE( code ) )
		{
			CWaitCursor wait;	// Ｗａｉｔカーソル

			CString filter;
			filter.Format( "ezbmn = %d and ezkcd = '%s'", bmn, code );
			m_pZm->bmezrec->Requery( filter, 1 );

			if( m_pZm->bmezrec->MoveFirst() == 0 ) {
				if( full ) {
					// 科目名称
					struct	_DBKNREC* pKn;
					if( pKn = DB_PjisToKnrec( code, m_pZm, TRUE ) ) {
						if( m_bKmcode ) {
							i = kmkcode_string( 2, pKn, knam_txt, sizeof knam_txt );
						}
						else {
							sprintf_s( buf, sizeof buf, "%.14s", pKn->knnam );
							i = ksp_cut( knam_txt, buf, 14 );	// 漢字スペースカット
						}
					}
					else
						i = 0;
					knam_txt[i++] = '.';
				}
				else	i = 0;
				
				knam_txt[i] = '\0';

				m_BRN_TBL.bmn = bmn;		// 部門コード
				m_BRN_TBL.code = code;		// 科目コード
				m_BRN_TBL.brn_kn50 = kana;	// 50音
				m_BRN_TBL.full = full;		// 科目名称も

				do {
					char ecdbf[20] = {0};
					if( m_pZm->bmezrec->st == -1 )	
						break;

					if( m_pZm->bmezrec->eztcd > 0 ) {

						if( DB_TekiyoNoRead( &brnrec, m_pZm->bmezrec->eztcd ) )
						{
							set_codestr( edacd_txt, sizeof edacd_txt, m_pZm->bmezrec->ezecd, m_pZm->zvol->edcol );
	//TRACE( "**** kana.IsEmpty() %d, length %d\n", kana.IsEmpty(), kana.GetLength() );

							if( BrnKanaCheck( kana, brnrec.kn50 ) )
							{
								if( !full )
								{
									// 摘要Ａｌｌスペースチェック
									if( kjlen( (char*)(LPCTSTR)brnrec.brmsg, brnrec.brmsg.GetLength() ) ) {
										strcpy_s( fmt, sizeof fmt, ".%s" );
										make_nameformat( fmt, sizeof fmt,  brnrec.brmsg );

										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*".%s%.32s"*/, edacd_txt, brnrec.brmsg );	// 枝番 摘要
									}
									else
										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( ".%s", edacd_txt );	// 枝番
									(m_BRN_TBL.pBRN_REC + n)->kn50 = brnrec.kn50;	// 50音
									(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;		// 枝番
									(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->bmezrec->eztcd;		// 摘要コード
								}
								else {
									// 摘要Ａｌｌスペースチェック
									if( kjlen(  (char*)(LPCTSTR)brnrec.brmsg, brnrec.brmsg.GetLength() ) )
									{
										strcpy_s( fmt, sizeof fmt, "%s" );
										make_nameformat( fmt, sizeof fmt,  brnrec.brmsg );

										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*"%s%.32s"*/, knam_txt, brnrec.brmsg );	// 摘要
										(m_BRN_TBL.pBRN_REC + n)->kn50 = brnrec.kn50;				// 50音
										(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;		// 枝番
										(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->bmezrec->eztcd;		// 摘要コード
									}
									else
									{
										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( "%s%s", knam_txt, edacd_txt );	// 枝番
										(m_BRN_TBL.pBRN_REC + n)->kn50 = brnrec.kn50;				// 50音
										(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;		// 枝番
										(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->bmezrec->eztcd;		// 摘要コード
									}
								}
								m_BRN_TBL.rec_n = ++n;
							}
						}
						else if( kana.IsEmpty() )
						{
							set_codestr( edacd_txt, sizeof edacd_txt, m_pZm->bmezrec->ezecd, m_pZm->zvol->edcol );

							if( !full )
								(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( ".%s", edacd_txt );	// 枝番
							else
								(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( "%s%s", knam_txt, edacd_txt );	// 枝番

							(m_BRN_TBL.pBRN_REC + n)->kn50.Empty();						// 50音
							(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;		// 枝番
							(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->bmezrec->eztcd;		// 摘要コード

							m_BRN_TBL.rec_n = ++n;
						}
					}
					else {
						// 残高本体の枝番名称
						set_codestr( edacd_txt, sizeof edacd_txt, m_pZm->bmezrec->ezecd, m_pZm->zvol->edcol );
//TRACE( "**** kana.IsEmpty() %d, length %d\n", kana.IsEmpty(), kana.GetLength() );

						if( BrnKanaCheck( kana, m_pZm->bmezrec->ezkana ) )
						{
							if( !full )
							{
								// 摘要Ａｌｌスペースチェック
								if( kjlen( (char*)(LPCTSTR)m_pZm->bmezrec->ezname, m_pZm->bmezrec->ezname.GetLength() ) ) {
									strcpy_s( fmt, sizeof fmt, ".%s" );
									make_nameformat( fmt, sizeof fmt, m_pZm->bmezrec->ezname );

									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*".%s%.32s"*/, edacd_txt,m_pZm->bmezrec->ezname );
								}
								else
									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( ".%s", edacd_txt );	
								(m_BRN_TBL.pBRN_REC + n)->kn50 = m_pZm->bmezrec->ezkana;	// 50音
								(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;
								(m_BRN_TBL.pBRN_REC + n)->tkno = 0;	
							}
							else {
								// 摘要Ａｌｌスペースチェック
								if( kjlen(  (char*)(LPCTSTR)m_pZm->bmezrec->ezname, m_pZm->bmezrec->ezname.GetLength() ) )
								{
									strcpy_s( fmt, sizeof fmt, "%s" );
									make_nameformat( fmt, sizeof fmt, m_pZm->bmezrec->ezname );

									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*"%s%.32s"*/, knam_txt, m_pZm->bmezrec->ezname );
									(m_BRN_TBL.pBRN_REC + n)->kn50 = m_pZm->bmezrec->ezkana;	// 50音
									(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;
									(m_BRN_TBL.pBRN_REC + n)->tkno = 0;	
								}
								else
								{
									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( "%s%s", knam_txt, edacd_txt );
									(m_BRN_TBL.pBRN_REC + n)->kn50 = m_pZm->bmezrec->ezkana;	// 50音
									(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->bmezrec->ezecd;
									(m_BRN_TBL.pBRN_REC + n)->tkno = 0;	
								}
							}
							m_BRN_TBL.rec_n = ++n;
						}					
					}
				}while( (m_pZm->bmezrec->MoveNext() == 0L) && (n < m_REC_MAX) );

				if( n )
				{
					m_BRN_TBL.page_max = 1;		// 最大ページ
					m_BRN_TBL.page_now = 1;		// 現在ページ
					m_BRN_TBL.page_recn = n;	// 1ページ分のレコード数
				}
				else
				{
					m_BRN_TBL.page_max = 0;		// 最大ページ
					m_BRN_TBL.page_now = 0;		// 現在ページ
					m_BRN_TBL.page_recn = 0;	// 1ページ分のレコード数
				}

				//
				// 50音+枝番ソート
				if( m_bKanaJun )  {
					if( m_BRN_TBL.rec_n > 1 )
						qsort( m_BRN_TBL.pBRN_REC, m_BRN_TBL.rec_n, sizeof( BRN_REC ), compare );
				}
				else {
					//枝番順もソートする(ezrecを50音順でopenしていた場合の対応)
					if( m_BRN_TBL.rec_n > 1 )
						qsort(m_BRN_TBL.pBRN_REC, m_BRN_TBL.rec_n, sizeof(BRN_REC), compareBrn);
				}
			}
		}
	}
	else {
		// 普通の枝番
		// テーブルイニシャライズ
		filter = _T( "seq = 3" );	//ezrec
		m_pZm->dcntl->Requery(filter);
		if( m_pZm->dcntl->st != -1 ) {
			ed_max = m_pZm->dcntl->mcnt;
		}

		m_REC_MAX = tbl_realoc( &m_BRN_TBL, m_REC_MAX, ed_max );	// 枝番登録数分
		m_BRN_TBL.ForeCol = BRTEK_ForeCOL;	// 表示文字色
		m_BRN_TBL.code = code;

		if( !isSYOGT_CODE( code ) )
		{
			CWaitCursor wait;	// Ｗａｉｔカーソル

/*			CString filter;
			filter.Format( "ezkcd = '%s'", code );
			m_pZm->ezrec->Requery( filter, 1 );

			if( m_pZm->ezrec->MoveFirst() == 0 )	*/
			ezdb_init( m_pZm );
			cnt = m_pZm->ezrec->Find( code );
			if( cnt > 0 ) {
				if( full )
				{
					// 科目名称
					struct	_DBKNREC* pKn;
					if( pKn = DB_PjisToKnrec( code, m_pZm, TRUE ) ) {
						if( m_bKmcode ) {
							i = kmkcode_string( 2, pKn, knam_txt, sizeof knam_txt );
						}
						else {
							sprintf_s( buf, sizeof buf, "%.14s", pKn->knnam );
							i = ksp_cut( knam_txt, buf, 14 );	// 漢字スペースカット
						}
					}
					else
						i = 0;
					knam_txt[i++] = '.';
				}
				else
					i = 0;
				knam_txt[i] = '\0';

				m_BRN_TBL.bmn = -1;			// 部門コード
				m_BRN_TBL.code = code;		// 科目コード
				m_BRN_TBL.brn_kn50 = kana;	// 50音
				m_BRN_TBL.full = full;		// 科目名称も

				j = 0;
				do {
					char ecdbf[20] = {0};
					if( m_pZm->ezrec->st == -1 )	
						break;

					if( m_pZm->ezrec->eztcd > 0 ) {

						if( DB_TekiyoNoRead( &brnrec, m_pZm->ezrec->eztcd ) )
						{
							set_codestr( edacd_txt, sizeof edacd_txt, m_pZm->ezrec->ezecd, m_pZm->zvol->edcol );
//	FILETrace( "**** kana.IsEmpty() %d, length %d\n", kana.IsEmpty(), kana.GetLength() );

							if( BrnKanaCheck( kana, brnrec.kn50 ) )
							{
								if( !full )
								{
									// 摘要Ａｌｌスペースチェック
									if( kjlen( (char*)(LPCTSTR)brnrec.brmsg, brnrec.brmsg.GetLength() ) ) {
										strcpy_s( fmt, sizeof fmt, ".%s" );
										make_nameformat( fmt, sizeof fmt, brnrec.brmsg );

										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*".%s%.32s"*/, edacd_txt, brnrec.brmsg );	// 枝番 摘要
									}
									else
										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( ".%s", edacd_txt );	// 枝番
									(m_BRN_TBL.pBRN_REC + n)->kn50 = brnrec.kn50;	// 50音
									(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;		// 枝番
									(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->ezrec->eztcd;		// 摘要コード
								}
								else {
									// 摘要Ａｌｌスペースチェック
									if( kjlen(  (char*)(LPCTSTR)brnrec.brmsg, brnrec.brmsg.GetLength() ) )
									{
										strcpy_s( fmt, sizeof fmt, "%s" );
										make_nameformat( fmt, sizeof fmt, brnrec.brmsg );

										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*"%s%.32s"*/, knam_txt, brnrec.brmsg );	// 摘要
										(m_BRN_TBL.pBRN_REC + n)->kn50 = brnrec.kn50;				// 50音
										(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;		// 枝番
										(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->ezrec->eztcd;		// 摘要コード
									}
									else
									{
										(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( "%s%s", knam_txt, edacd_txt );	// 枝番
										(m_BRN_TBL.pBRN_REC + n)->kn50 = brnrec.kn50;				// 50音
										(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;		// 枝番
										(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->ezrec->eztcd;		// 摘要コード
									}
								}
//_DEBUG_FILEOUT( "mk_brtektb ### brntky chk %s, %s, %s\n", (m_BRN_TBL.pBRN_REC + n)->brmsg, kana, brnrec.kn50 );

								m_BRN_TBL.rec_n = ++n;

							}
						}
						else if( kana.IsEmpty() )
						{
							set_codestr( edacd_txt, sizeof edacd_txt, m_pZm->ezrec->ezecd, m_pZm->zvol->edcol );

							if( !full )
								(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( ".%s", edacd_txt );	// 枝番
							else
								(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( "%s%s", knam_txt, edacd_txt );	// 枝番

							(m_BRN_TBL.pBRN_REC + n)->kn50.Empty();						// 50音
							(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;		// 枝番
							(m_BRN_TBL.pBRN_REC + n)->tkno = m_pZm->ezrec->eztcd;		// 摘要コード

							m_BRN_TBL.rec_n = ++n;
						}
					}
					else {
						// 残高本体の枝番名称
						set_codestr( edacd_txt, sizeof edacd_txt, m_pZm->ezrec->ezecd, m_pZm->zvol->edcol );
//FILETrace( "mk_brtektb **** kana.IsEmpty() %d, length %d\n", kana.IsEmpty(), kana.GetLength() );

						if( BrnKanaCheck( kana, m_pZm->ezrec->ezkana ) )
						{
							if( !full )
							{
								// 摘要Ａｌｌスペースチェック
								if( kjlen( (char*)(LPCTSTR)m_pZm->ezrec->ezname, m_pZm->ezrec->ezname.GetLength() ) ) {
									strcpy_s( fmt, sizeof fmt, ".%s" );
									make_nameformat( fmt, sizeof fmt, m_pZm->ezrec->ezname );

									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*".%s%.32s"*/, edacd_txt,m_pZm->ezrec->ezname );
								}
								else
									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( ".%s", edacd_txt );	
								(m_BRN_TBL.pBRN_REC + n)->kn50 = m_pZm->ezrec->ezkana;	// 50音
								(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;
								(m_BRN_TBL.pBRN_REC + n)->tkno = 0;	
							}
							else {
								// 摘要Ａｌｌスペースチェック
								if( kjlen(  (char*)(LPCTSTR)m_pZm->ezrec->ezname, m_pZm->ezrec->ezname.GetLength() ) )
								{
									strcpy_s( fmt, sizeof fmt, "%s" );
									make_nameformat( fmt, sizeof fmt, m_pZm->ezrec->ezname );

									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( fmt/*"%s%.32s"*/, knam_txt, m_pZm->ezrec->ezname );
									(m_BRN_TBL.pBRN_REC + n)->kn50 = m_pZm->ezrec->ezkana;	// 50音
									(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;
									(m_BRN_TBL.pBRN_REC + n)->tkno = 0;	
								}
								else
								{
									(m_BRN_TBL.pBRN_REC + n)->brmsg.Format( "%s%s", knam_txt, edacd_txt );
									(m_BRN_TBL.pBRN_REC + n)->kn50 = m_pZm->ezrec->ezkana;	// 50音
									(m_BRN_TBL.pBRN_REC + n)->brn = m_pZm->ezrec->ezecd;
									(m_BRN_TBL.pBRN_REC + n)->tkno = 0;	
								}
							}
							m_BRN_TBL.rec_n = ++n;
						}					
					}
				}while( (++j < cnt) && (m_pZm->ezrec->MoveNext() == 0L) && (n < m_REC_MAX) );

				if( n )
				{
					m_BRN_TBL.page_max = 1;		// 最大ページ
					m_BRN_TBL.page_now = 1;		// 現在ページ
					m_BRN_TBL.page_recn = n;	// 1ページ分のレコード数
				}
				else
				{
					m_BRN_TBL.page_max = 0;		// 最大ページ
					m_BRN_TBL.page_now = 0;		// 現在ページ
					m_BRN_TBL.page_recn = 0;	// 1ページ分のレコード数
				}

				//
				// 50音+枝番ソート
				if( m_bKanaJun ) {
					if( m_BRN_TBL.rec_n > 1 )
						qsort( m_BRN_TBL.pBRN_REC, m_BRN_TBL.rec_n, sizeof( BRN_REC ), compare );
				}
				else {
					//枝番順もソートする(ezrecを50音順でopenしていた場合の対応)
					if( m_BRN_TBL.rec_n > 1 )
						qsort(m_BRN_TBL.pBRN_REC, m_BRN_TBL.rec_n, sizeof(BRN_REC), compareBrn);
				}
			}
		}
	}
	return n ? &m_BRN_TBL : 0;
}



// 適合する カナチェック
BOOL CBrnTkySub::BrnKanaCheck( CString& inputkana, CString& reckana )
{
	int ipLen, rcLen;

	if( inputkana.IsEmpty() )
		return TRUE;

	ipLen = inputkana.GetLength();
	rcLen = reckana.GetLength();

	if( ipLen > rcLen )
		return FALSE;
	else {
		if( ipLen == rcLen )
			return inputkana == reckana;
		else {
			CString tmp;
			tmp = reckana.Left( ipLen );
		
			return inputkana == tmp;
		}
	}
}


// データベース 摘要読み込み
int CBrnTkySub::DB_TekiyoNoRead( BRN_REC* prec, int no )
{
	BOOL bRet = FALSE;
	if( no < 1 )	return FALSE;

	if( m_tkquery ) {
		m_pZm->tkrec->Requery( "", 0 );
		m_tkquery = FALSE;
	}

	if( m_pZm->tkrec->Find( no ) == 1 ) {
			//	prec->brn = 0;
				prec->tkno = m_pZm->tkrec->tkcod;
				prec->kn50 = m_pZm->tkrec->tkana;
				prec->brmsg = m_pZm->tkrec->tkname;

				LPSTR	p = prec->brmsg.GetBuffer(256);
				int n = kjlen( p, m_TKYO_LEN );
				*(p + n) = '\0';
				prec->brmsg.ReleaseBuffer();

				bRet = TRUE;
	}
/*
	if( m_pZm->tkrec->MoveFirst() == 0 ) {
		do {
			if( no == m_pZm->tkrec->tkcod ) {

			//	prec->brn = 0;
				prec->tkno = m_pZm->tkrec->tkcod;
				prec->kn50 = m_pZm->tkrec->tkana;
				prec->brmsg = m_pZm->tkrec->tkname;

				LPSTR	p = prec->brmsg.GetBuffer(256);
				int n = kjlen( p, TKYO_LEN );
				*(p + n) = '\0';
				prec->brmsg.ReleaseBuffer();

				bRet = TRUE;
				break;
			}
		} while( m_pZm->tkrec->MoveNext() == 0 );
	}
*/
	return bRet;
}


void CBrnTkySub::make_nameformat( char* fmt, int fmtsize, LPCTSTR brnName )
{
	int st = get_strcnt( (char*)brnName, BRNAM_MAX );

	char bf[64] = {0};

	if( st < (BRNAM_MAX-1) ) {
		strcat_s( fmt, fmtsize, "%s" );
	}
	else {
		if( st > BRNAM_MAX )	st = BRNAM_MAX;

		sprintf_s( bf, sizeof bf, "%%.%ds", st );
		strcat_s( fmt, fmtsize, bf );
	}
}


void CBrnTkySub::set_brnKanaJun( BOOL bKanaJun )
{
	m_bKanaJun = bKanaJun;
}


/////////////////////////////////////////////////////////////////////////////
// CBrnTky
//	実際に使用するクラス

CBrnTky::CBrnTky()
{
	m_pSub = NULL;
}

CBrnTky::~CBrnTky()
{
	if( m_pSub != NULL )
		delete m_pSub;

	m_pSub = NULL;
}


/***********************************
		get_brtekmsg()

		枝番摘要名称リード

		->	"枝番摘要名称"
			0 ... Not Found

  **********************************/
BRN_NAME* CBrnTky::get_brtekmsg( int bmn, CString& code, int brn, BOOL full, int mode )
{
	return m_pSub->_get_brtekmsg( bmn, code, brn, full, mode );
}

/****************************************
	枝番to摘要レコードリード

		get_brtek()

		->	1 ... Found
			0 ... Not Found
 *****************************************/
BOOL CBrnTky::get_brtek( BRN_REC *prec, int bmn, CString& code, int brn )
{
	return m_pSub->_get_brtek( prec, bmn, code, brn );
}

int CBrnTky::get_brtek( int bmn, CString& code, int brn )
{
	return m_pSub->_get_brtek( bmn, code, brn );
}

/***********************************
		get_brtektb()

		枝番摘要名称テーブルリード

		->	TRUE ... OK
			FALSE .. No
  **********************************/
BOOL CBrnTky::get_brtektb( BRN_TB **tbl, int bmn, CString& code, CString& kana, int page_sz, int page, BOOL full, int mode )
{
	return m_pSub->_get_brtektb( tbl, bmn, code, kana, page_sz, page, full, mode );
}

// クラス 初期化
int  CBrnTky::brntky_iniz( CDBZmSub* pZm )
{
	if( m_pSub == NULL ) {
		if( (m_pSub = new CBrnTkySub) == NULL )
			return -1;
	}
	m_pSub->sub_init( pZm );

	return 0;
}

// クラスの 終了
//	諸口枝番 のレコード開放
void CBrnTky::brntky_close()
{
	if( m_pSub == NULL )	return;

	m_pSub->sub_close();
}


//-------------------------------------------------------------------
//	枝番テーブル開放(同一条件だと枝番テーブルを作成しないので)
//-------------------------------------------------------------------
int CBrnTky::brtek_free()
{
	if( m_pSub == NULL )	return 0;

	return m_pSub->sub_free();
}


//科目コードを表示する
void	CBrnTky::brntky_kmcode( BOOL bKmcode )
{
	if( m_pSub == NULL )	return;

	m_pSub->IsKmkcodeDisp() =  bKmcode; 
	m_pSub->SetSyogKmkcodeDisp( bKmcode );
}

// 摘要の クエリーをセット
void CBrnTky::set_tkquery( BOOL bQuery )
{
	if( m_pSub == NULL )	return;

	m_pSub->set_tkquery( bQuery );
}


//枝番カナ順
void CBrnTky::set_brnKanaJun( BOOL bKanaJun )
{
	if( m_pSub == NULL )	return;

	m_pSub->set_brnKanaJun( bKanaJun );
}
