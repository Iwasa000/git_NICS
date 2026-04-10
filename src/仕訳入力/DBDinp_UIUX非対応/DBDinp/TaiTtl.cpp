// TaiTtl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "TaiTtl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <afxmt.h>
#ifdef MUTEX	// 05.23 /00
static CMutex taidata_criticalSection;
#else
static CCriticalSection taidata_criticalSection;
#endif


void tai_zeroclear(STaiData* psd)
{
	int ofs = offsetof( STaiData, pkm_ttl );
	::ZeroMemory( psd, ofs );

	psd->pkm_ttl = NULL;
	psd->km_cnt = 0;
	psd->clq_sw = 0;

	psd->tbary.RemoveAll();
}


void tai_copydata(STaiData* dst, STaiData* src)
{
	int ofs = offsetof( STaiData, pkm_ttl );

	memcpy( dst, src, ofs );

	dst->pkm_ttl	= src->pkm_ttl;
	dst->km_cnt		= src->km_cnt;
	dst->clq_sw		= src->clq_sw;

	dst->tbary.Copy( src->tbary );
}

/////////////////////////////////////////////////////////////////////////////
// CTaiTtl

CTaiTtl::CTaiTtl()
{
	tai_zeroclear(&m_TaiData);
//	memset( &m_TaiData, 0, sizeof(m_TaiData));
	m_TaiData.clq_sw = TAICLQ_NON;
}

CTaiTtl::~CTaiTtl()
{
	free_kmktbl();
}

void CTaiTtl::free_kmktbl()
{
	if( m_TaiData.pkm_ttl != NULL ) {
		delete[] m_TaiData.pkm_ttl;
		m_TaiData.pkm_ttl = NULL;
	}
	m_TaiData.km_cnt = 0;
}

//
// 貸借データエリアイニシャライズ
void CTaiTtl::taidata_inz( int kmcnt )
{
//taidata_criticalSection.Lock();
	//検索一覧(ScanViewで表示せずに)DBDinpViewで仕訳を修正してから表示した場合、
	//金額がおかしくなるので計算しなおす。
	if( kmcnt == -1 ) {
		kmcnt = m_TaiData.km_cnt;
	}

	if( kmcnt > 0 ) {
		// 科目数が変わった場合。
		if( m_TaiData.km_cnt != kmcnt ) {
			free_kmktbl();
			tai_zeroclear(&m_TaiData);
		//	memset( &m_TaiData, 0, sizeof(m_TaiData));
		}
		else {
			int ofs;
			ofs = offsetof( STaiData, pkm_ttl );
			memset( &m_TaiData, 0, ofs );
		}

		if( m_TaiData.pkm_ttl == NULL ) {
			if( (m_TaiData.pkm_ttl = new STaiKmkData[kmcnt] ) != NULL ) {
				m_TaiData.km_cnt = kmcnt;
				memset( m_TaiData.pkm_ttl, 0, sizeof STaiKmkData * kmcnt );
			}
			else {
				m_TaiData.km_cnt = -1;	// エラー
			}
		}
		else {
			m_TaiData.km_cnt = kmcnt;
			memset( m_TaiData.pkm_ttl, 0, sizeof STaiKmkData * kmcnt );
		}
	}
	else	m_TaiData.km_cnt = -1;

	//枝番計算用
	m_TaiData.tbary.RemoveAll();

	m_TaiData.clq_sw = TAICLQ_NON;

//taidata_criticalSection.Unlock();
}

//
// 貸借データのゲット
STaiData* CTaiTtl::get_taidata()
{
//	taidata_criticalSection.Lock();
		tai_copydata( &m_TaiData2, &m_TaiData );
//		memmove( &m_TaiData2, &m_TaiData, sizeof( m_TaiData) );

//	taidata_criticalSection.Unlock();

	return &m_TaiData2;
}

//
// 貸借データの加減算
static void clq_taidata( STaiData *ttl, CDBINPDataRec *drec, int sg )
{
	CArith arith;
	BOOL add;
	int	l_r, i;

//	taidata_criticalSection.Lock();
	{
		if( drec->m_dsign[0] & 0x01 )	// 取消データ
		{
			;
		}
		else
		{
			add = (sg < 0 ? FALSE : TRUE);

			l_r = arith.l_retn();
			arith.l_defn( _BIN );

			// 検索で借／貸の科目が指定された。
			if( ttl->clq_kmk[0] ) {
				if( add )	// 加算
				{
					if( drec->m_dbt == ttl->clq_kmk )
						arith.l_add( ttl->comm_deb, ttl->comm_deb, drec->m_val );
					if( drec->m_cre == ttl->clq_kmk )
						arith.l_add( ttl->comm_cre, ttl->comm_cre, drec->m_val );

					if( drec->m_dbr != -1 ) ttl->ezdt_cnt++;
					if( drec->m_cbr != -1 ) ttl->ezdt_cnt++;
				}
				else		// 減算
				{
					if( drec->m_dbt == ttl->clq_kmk )
						arith.l_sub( ttl->comm_deb, ttl->comm_deb, drec->m_val );
					if( drec->m_cre == ttl->clq_kmk )
						arith.l_sub( ttl->comm_cre, ttl->comm_cre, drec->m_val );

					if( drec->m_dbr != -1 ) ttl->ezdt_cnt--;
					if( drec->m_cbr != -1 ) ttl->ezdt_cnt--;
				}
			}

			// 諸口仕訳
			if( isSYOGT_CODE( drec->m_dbt ) || isSYOGT_CODE( drec->m_cre ) )
			{
				BOOL bSotoz = FALSE;

				if( (drec->m_dsign[0] & 0x40) && (drec->m_dsign[1]&0xf0)==0x00 &&
					(drec->m_dsign[2] & 0x0f) == 0x01 ) {
						bSotoz = TRUE;
				}

				if( isSYOGT_CODE( drec->m_dbt ) )
				{
					if( add ) {	// 加算
						arith.l_add( ttl->syog_deb, ttl->syog_deb, drec->m_val );
						if( bSotoz )
							arith.l_add( ttl->syog_deb, ttl->syog_deb, drec->m_zei );

						if( drec->m_dbr != -1 ) ttl->ezdt_cnt++;	// 08.27 /04
						if( drec->m_cbr != -1 ) ttl->ezdt_cnt++;
					}
					else {		// 減算
						arith.l_sub( ttl->syog_deb, ttl->syog_deb, drec->m_val );
						if( bSotoz )
							arith.l_sub( ttl->syog_deb, ttl->syog_deb, drec->m_zei );
						if( drec->m_dbr != -1  ) ttl->ezdt_cnt--;	// 08.27 /04
						if( drec->m_cbr != -1  ) ttl->ezdt_cnt--;
					}
				}		
				if( isSYOGT_CODE( drec->m_cre ) )
				{
					if( add ) {	// 加算
						arith.l_add( ttl->syog_cre, ttl->syog_cre, drec->m_val );
						if( bSotoz )
							arith.l_add( ttl->syog_cre, ttl->syog_cre, drec->m_zei );

						if( drec->m_dbr != -1  ) ttl->ezdt_cnt++;	// 08.27 /04
						if( drec->m_cbr != -1  ) ttl->ezdt_cnt++;
					}
					else {		// 減算
						arith.l_sub( ttl->syog_cre, ttl->syog_cre, drec->m_val );
						if( bSotoz )
							arith.l_sub( ttl->syog_cre, ttl->syog_cre, drec->m_zei );

						if( drec->m_dbr != -1  ) ttl->ezdt_cnt--;	// 08.27 /04
						if( drec->m_cbr != -1  ) ttl->ezdt_cnt--;
					}
				}
			}
			// 通常仕訳
			else
			{
				if( ! ttl->clq_kmk[0] ) {
					if( add )	// 加算
					{
						arith.l_add( ttl->comm_deb, ttl->comm_deb, drec->m_val );
						arith.l_add( ttl->comm_cre, ttl->comm_cre, drec->m_val );

						if( drec->m_dbr != -1  ) ttl->ezdt_cnt++;
						if( drec->m_cbr != -1  ) ttl->ezdt_cnt++;
					}
					else		// 減算
					{
						arith.l_sub( ttl->comm_deb, ttl->comm_deb, drec->m_val );
						arith.l_sub( ttl->comm_cre, ttl->comm_cre, drec->m_val );

						if( drec->m_dbr != -1  ) ttl->ezdt_cnt--;
						if( drec->m_cbr != -1  ) ttl->ezdt_cnt--;
					}
				}
			}

			if( add )	// 加算
				ttl->data_cnt++;
			else		// 減算
				ttl->data_cnt--;

			// 借方 
			CString tmp;
			int ok_sw = 0;
			STaiKmkData* pKmd;
			pKmd = ttl->pkm_ttl;

			if( ! isSYOGT_CODE( drec->m_dbt ) ) {
				for( i = 0; i < ttl->km_cnt; i++ ) {
					tmp = (pKmd+i)->kmcod;
					tmp.TrimRight();
					if( (! ok_sw && (pKmd+i)->kmcod[0] == '\0') || tmp == drec->m_dbt ) {
						if( (pKmd+i)->kmcod[0] == '\0' )	
							strcpy_s( (pKmd+i)->kmcod, sizeof (pKmd+i)->kmcod, drec->m_dbt );
						if( add ) {	// 加算
							arith.l_add( (pKmd+i)->kmk_kri, (pKmd+i)->kmk_kri, drec->m_val );
						}
						else {		// 減算
							arith.l_sub( (pKmd+i)->kmk_kri, (pKmd+i)->kmk_kri, drec->m_val );
						}
						ok_sw = 1;
						break;
					}
				}
				//枝番計算
				if( drec->m_dbr != -1 ) {
					int brcnt = ttl->tbary.GetCount();
					ok_sw = 0;

					for( i = 0; i < brcnt; i++ ) {
						tmp = ttl->tbary[i].kmk_cd;
						tmp.TrimRight();
						if( tmp.CompareNoCase( drec->m_dbt ) == 0 ) {
							if( ttl->tbary[i].brn_no == drec->m_dbr ) {

								if( add ) {	// 加算
									arith.l_add( ttl->tbary[i].brn_kari, ttl->tbary[i].brn_kari, drec->m_val );
								}
								else {		// 減算
									arith.l_sub( ttl->tbary[i].brn_kari, ttl->tbary[i].brn_kari, drec->m_val );
								}
								ok_sw = 1;
								break;
							}
						}
					}
					if( ! ok_sw ) {
						STaiBrnData adata = {0};

						strcpy_s( adata.kmk_cd, sizeof adata.kmk_cd, drec->m_dbt );
						adata.brn_no = drec->m_dbr;

						if( add ) {	// 加算
							arith.l_add( adata.brn_kari, adata.brn_kari, drec->m_val );
						}
						else {		// 減算
							arith.l_sub( adata.brn_kari, adata.brn_kari, drec->m_val );
						}
						ttl->tbary.Add( adata );	//新規追加
					}
				}
			}
			// 貸方
			ok_sw = 0;
			pKmd = ttl->pkm_ttl;

			if( ! isSYOGT_CODE( drec->m_cre ) ) {
				for( i = 0; i < ttl->km_cnt; i++ ) {
					tmp = (pKmd+i)->kmcod;
					tmp.TrimRight();
					if( (! ok_sw && (pKmd+i)->kmcod[0] == '\0') || tmp == drec->m_cre ) {
						if( (pKmd+i)->kmcod[0] == '\0' )	
							strcpy_s( (pKmd+i)->kmcod, sizeof (pKmd+i)->kmcod, drec->m_cre );
						if( add ) {	// 加算
							arith.l_add( (pKmd+i)->kmk_ksi, (pKmd+i)->kmk_ksi, drec->m_val );
						}
						else {		// 減算
							arith.l_sub( (pKmd+i)->kmk_ksi, (pKmd+i)->kmk_ksi, drec->m_val );
						}
						ok_sw = 1;
						break;
					}
				}
				//枝番計算
				if( drec->m_cbr != -1 ) {
					int brcnt = ttl->tbary.GetCount();
					ok_sw = 0;

					for( i = 0; i < brcnt; i++ ) {
						tmp = ttl->tbary[i].kmk_cd;
						tmp.TrimRight();
						if( tmp.CompareNoCase( drec->m_cre ) == 0 ) {
							if( ttl->tbary[i].brn_no == drec->m_cbr ) {

								if( add ) {	// 加算
									arith.l_add( ttl->tbary[i].brn_kasi, ttl->tbary[i].brn_kasi, drec->m_val );
								}
								else {		// 減算
									arith.l_sub( ttl->tbary[i].brn_kasi, ttl->tbary[i].brn_kasi, drec->m_val );
								}
								ok_sw = 1;
								break;
							}
						}
					}
					if( ! ok_sw ) {
						STaiBrnData adata = {0};

						strcpy_s( adata.kmk_cd, sizeof adata.kmk_cd, drec->m_cre );
						adata.brn_no = drec->m_cbr;

						if( add ) {	// 加算
							arith.l_add( adata.brn_kasi, adata.brn_kasi, drec->m_val );
						}
						else {		// 減算
							arith.l_sub( adata.brn_kasi, adata.brn_kasi, drec->m_val );
						}
						ttl->tbary.Add( adata );	//新規追加
					}
				}
			}
			if( ttl->clq_sw == TAICLQ_NON )
				ttl->clq_sw = TAICLQ_CNT;

			arith.l_defn( l_r );
		}
	}
//	taidata_criticalSection.Unlock();
}

//
// 貸借データへ加算
void CTaiTtl::add_taidata( CDBINPDataRec *drec )
{
	clq_taidata( &m_TaiData, drec, 1 );
}

//
// 貸借データから減算
void CTaiTtl::sub_taidata( CDBINPDataRec *drec )
{
	clq_taidata( &m_TaiData, drec, -1 );
}


int _cdecl kmktbl_cmp( const void* elem1, const void* elem2 )
{
	STaiKmkData *pkm1, *pkm2;

	pkm1 = (STaiKmkData *)elem1;
	pkm2 = (STaiKmkData *)elem2;

	int ret;

	if( pkm1->kmcod[0] == 0 &&  pkm2->kmcod[0] == 0 )
		ret = 0;
	else if( pkm1->kmcod[0] == 0 )	ret = 1;
	else if( pkm2->kmcod[0] == 0 )	ret = -1;
	else							ret = strcmp( pkm1->kmcod, pkm2->kmcod );

	return ret;
}


//枝番データソート
int _cdecl brnarray_cmp( const void* elem1, const void* elem2 )
{
	STaiBrnData *p1, *p2;

	p1 = (STaiBrnData *)elem1;
	p2 = (STaiBrnData *)elem2;

	int ret;

	if( p1->kmk_cd == 0 &&  p2->kmk_cd[0] == 0 )
		ret = 0;
	else if( p1->kmk_cd[0] == 0 )	ret = 1;
	else if( p2->kmk_cd[0] == 0 )	ret = -1;
	else {
		ret = strcmp( p1->kmk_cd, p2->kmk_cd );
		if( ret == 0 ) {
			if( p1->brn_no == p2->brn_no )		ret = 0;
			else if( p1->brn_no < p2->brn_no )	ret = -1;
			else								ret = 1;
		}
	}

	return ret;
}


//
// 貸借合計計算終了
void CTaiTtl::kmktbl_sort()
{
//	taidata_criticalSection.Lock();

		qsort( m_TaiData.pkm_ttl, m_TaiData.km_cnt, sizeof STaiKmkData, kmktbl_cmp );

		INT_PTR cnt = m_TaiData.tbary.GetCount();
		if( cnt > 0 ) {
			qsort( &m_TaiData.tbary[0], m_TaiData.tbary.GetCount(), sizeof STaiBrnData, brnarray_cmp );
		}
	
//	taidata_criticalSection.Unlock();
}


//
// 貸借合計計算終了
void CTaiTtl::end_taiclq()
{
//	taidata_criticalSection.Lock();
		m_TaiData.clq_sw = TAICLQ_END;
//	taidata_criticalSection.Unlock();
}


//
// 貸借合計計算サイン初期化
void CTaiTtl::init_taiclq()
{
	m_TaiData.clq_sw = TAICLQ_NON;
}


BOOL CTaiTtl::IsEndCalq()
{
	BOOL bret;
//	taidata_criticalSection.Lock();
		bret = (m_TaiData.clq_sw == TAICLQ_END) ? TRUE : FALSE;
//	taidata_criticalSection.Unlock();

	return bret;
}