#include "StdAfx.h"
#include "H28SnFhyo60Data.h"

CH28SnFhyo60Data::CH28SnFhyo60Data(void)
{

//	Sn_SEQ0 = 0;
	Sn_6FSDEATH = 0;
	Sn_6FSDAIHYO.Empty();
	Sn_6FSLIMIT = 0;

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
//		Sn_SEQ[idx] = 0;
		Sn_NO[idx] = -1;
		Sn_6FSJUSYO[idx].Empty();
		Sn_6FSFURI[idx].Empty();
		Sn_6FSNAME[idx].Empty();
		memset( Sn_6FSHASH[idx],'\0', sizeof(Sn_6FSHASH[idx]));
		Sn_6FSSYOKU[idx].Empty();
		Sn_6FSTSUDUKI[idx].Empty();
		Sn_6FSBIRTH[idx] = 0;
		Sn_6FSTEL1[idx].Empty();
		Sn_6FSTEL2[idx].Empty();
		Sn_6FSTEL3[idx].Empty();
		Sn_6FSHOTEI[idx] = 0;
		Sn_6FSBUNSHI[idx].Empty();
		Sn_6FSBUNBO[idx].Empty();
		memset( Sn_6FSKAGAKU[idx],'\0', sizeof(Sn_6FSKAGAKU[idx]));
		memset( Sn_6FSNOFUZEI[idx],'\0', sizeof(Sn_6FSNOFUZEI[idx]));
		memset( Sn_6FSNOFUCHIHO[idx],'\0', sizeof(Sn_6FSNOFUCHIHO[idx]));
		memset( Sn_6FSKANPUZEI[idx],'\0', sizeof(Sn_6FSKANPUZEI[idx]));
		memset( Sn_6FSKANPUCHIHO[idx],'\0', sizeof(Sn_6FSKANPUCHIHO[idx]));
		Sn_6FSNOFUKANPUsw[idx] = 0;
		Sn_6FSGNAME[idx].Empty();
		Sn_6FSGSEL[idx] = 0;
		Sn_6FSSNAME[idx].Empty();
		Sn_6FSSSEL[idx] = 0;
		Sn_6FSYOKIN[idx].Empty();
		Sn_6FSKOUZA[idx].Empty();
		Sn_6FSKIGO1[idx].Empty();
		Sn_6FSKIGO2[idx].Empty();
		Sn_6FSYUBIN[idx].Empty();
	}

	m_Arith.l_defn( 0x16 );
}

CH28SnFhyo60Data::~CH28SnFhyo60Data(void)
{
}

int CH28SnFhyo60Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;

	///////////////////
	//付表６
	///////////////////
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );

	if( m_DBSyzAccess.m_pFhyo60rec ){
		m_DBSyzAccess.m_pFhyo60rec->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.Fhyo60RecOpen(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CFHYO60REC *pFhyo60rec = m_DBSyzAccess.m_pFhyo60rec;

	if(( pFhyo60rec->MoveFirst() != -1 )&&(pFhyo60rec->st!=-1)){

		//Sn_SEQ0 = pFhyo60rec->Sn_SEQ;
		Sn_6FSDEATH = pFhyo60rec->Sn_6FSDEATH;
		Sn_6FSDAIHYO = pFhyo60rec->Sn_6FSDAIHYO;
		Sn_6FSLIMIT = pFhyo60rec->Sn_6FSLIMIT;
	}

	///////////////////
	//相続人テーブル
	///////////////////
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );
	if( m_DBSyzAccess.m_pSyz_Sozoku ){
		m_DBSyzAccess.m_pSyz_Sozoku->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.Syz_SozokuOpen(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSYZ_SOZOKU *pSyz_Sozoku = m_DBSyzAccess.m_pSyz_Sozoku;

	for( int idx=0;; idx++ ){
		if( idx==0 ){
			if( (pSyz_Sozoku->MoveFirst()==-1) || (pSyz_Sozoku->st==-1) ){
				break;
			}
		}
		else{
			if( (pSyz_Sozoku->MoveNext()==-1) || (pSyz_Sozoku->st==-1) ){
				break;
			}
		}

		if( idx >= SOZOKU_MAX ){
			break;
		}

		if( pSyz_Sozoku->Sn_NO >= SOZOKU_MAX ){
			continue;
		}

		int idx2 = pSyz_Sozoku->Sn_NO;
		//Sn_SEQ[idx] = pSyz_Sozoku->Sn_SEQ;
		Sn_NO[idx2] = pSyz_Sozoku->Sn_NO;
		Sn_6FSJUSYO[idx2] = pSyz_Sozoku->Sn_6FSJUSYO;
		Sn_6FSFURI[idx2] = pSyz_Sozoku->Sn_6FSFURI;
		Sn_6FSNAME[idx2] = pSyz_Sozoku->Sn_6FSNAME;
		//wsprintf(Sn_6FSHASH[idx2], "%s", pSyz_Sozoku->Sn_6FSHASH );
		strcpy_s(Sn_6FSHASH[idx2], sizeof(Sn_6FSHASH[idx2]), pSyz_Sozoku->Sn_6FSHASH);
		Sn_6FSSYOKU[idx2] = pSyz_Sozoku->Sn_6FSSYOKU;
		Sn_6FSTSUDUKI[idx2] = pSyz_Sozoku->Sn_6FSTSUDUKI;
		Sn_6FSBIRTH[idx2] = pSyz_Sozoku->Sn_6FSBIRTH;
		Sn_6FSTEL1[idx2] = pSyz_Sozoku->Sn_6FSTEL1;
		Sn_6FSTEL2[idx2] = pSyz_Sozoku->Sn_6FSTEL2;
		Sn_6FSTEL3[idx2] = pSyz_Sozoku->Sn_6FSTEL3;
		Sn_6FSHOTEI[idx2] = pSyz_Sozoku->Sn_6FSHOTEI;
		Sn_6FSBUNSHI[idx2] = pSyz_Sozoku->Sn_6FSBUNSHI;
		Sn_6FSBUNBO[idx2] = pSyz_Sozoku->Sn_6FSBUNBO;
		m_Util.val_to_bin( Sn_6FSKAGAKU[idx2], pSyz_Sozoku->Sn_6FSKAGAKU );
		m_Util.val_to_bin( Sn_6FSNOFUZEI[idx2], pSyz_Sozoku->Sn_6FSNOFUZEI );
		m_Util.val_to_bin( Sn_6FSNOFUCHIHO[idx2], pSyz_Sozoku->Sn_6FSNOFUCHIHO );
		m_Util.val_to_bin( Sn_6FSKANPUZEI[idx2], pSyz_Sozoku->Sn_6FSKANPUZEI );
		m_Util.val_to_bin( Sn_6FSKANPUCHIHO[idx2], pSyz_Sozoku->Sn_6FSKANPUCHIHO );
		Sn_6FSNOFUKANPUsw[idx2] = pSyz_Sozoku->Sn_6FSNOFUKANPUsw;
		Sn_6FSGNAME[idx2] = pSyz_Sozoku->Sn_6FSGNAME;
		Sn_6FSGSEL[idx2] = pSyz_Sozoku->Sn_6FSGSEL;
		Sn_6FSSNAME[idx2] = pSyz_Sozoku->Sn_6FSSNAME;
		Sn_6FSSSEL[idx2] = pSyz_Sozoku->Sn_6FSSSEL;
		Sn_6FSYOKIN[idx2] = pSyz_Sozoku->Sn_6FSYOKIN;
		Sn_6FSKOUZA[idx2] = pSyz_Sozoku->Sn_6FSKOUZA;
		Sn_6FSKIGO1[idx2] = pSyz_Sozoku->Sn_6FSKIGO1;
		Sn_6FSKIGO2[idx2] = pSyz_Sozoku->Sn_6FSKIGO2;
		Sn_6FSYUBIN[idx2] = pSyz_Sozoku->Sn_6FSYUBIN;
	}

	return 0;
}

int CH28SnFhyo60Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{

	ASSERT( pDBNpSub );

	if( pDBNpSub == NULL ){
		return -1;
	}

	CString	filter;
	filter.Format( _T("sn_seq = %d"), sn_seq );

	///////////////////
	//付表６
	///////////////////
	if( m_DBSyzAccess.m_pFhyo60rec ){
		m_DBSyzAccess.m_pFhyo60rec->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.Fhyo60RecOpen(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CFHYO60REC *pFhyo60rec = m_DBSyzAccess.m_pFhyo60rec;

	if( (pFhyo60rec->MoveFirst()==-1) || (pFhyo60rec->st==-1) ){	// データ無し
		pFhyo60rec->AddNew();
		pFhyo60rec->Sn_SEQ = sn_seq;
		pFhyo60rec->Sn_6FSDEATH			= Sn_6FSDEATH;
		pFhyo60rec->Sn_6FSDAIHYO		= Sn_6FSDAIHYO;
		pFhyo60rec->Sn_6FSLIMIT			= Sn_6FSLIMIT;
		pFhyo60rec->Update();
	}else{ //データ有り
		pFhyo60rec->Edit();
		pFhyo60rec->Sn_6FSDEATH			= Sn_6FSDEATH;
		pFhyo60rec->Sn_6FSDAIHYO		= Sn_6FSDAIHYO;
		pFhyo60rec->Sn_6FSLIMIT			= Sn_6FSLIMIT;
		pFhyo60rec->Update();
	}

	///////////////////
	//相続人テーブル
	///////////////////
	filter.Format( _T("sn_seq = %d"), sn_seq );
	if( m_DBSyzAccess.m_pSyz_Sozoku ){
		m_DBSyzAccess.m_pSyz_Sozoku->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.Syz_SozokuOpen(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSYZ_SOZOKU *pSyz_Sozoku = m_DBSyzAccess.m_pSyz_Sozoku;

	int rec_cnt = pSyz_Sozoku->GetRecordCount();
	for( int cnt = 0; cnt < rec_cnt; cnt++ ){
		if( cnt == 0 ){
			if(( pSyz_Sozoku->MoveFirst() == -1 )||( pSyz_Sozoku->st == -1 )){
				break;
			}
		}/*else{
			if(( pSyz_Sozoku->MoveNext() == -1 )||( pSyz_Sozoku->st == -1 )){
				break;	
			}
		}*/
		pSyz_Sozoku->Delete();
	}

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
		if( Sn_NO[idx] == -1 ){
			continue;
		}

		char hash_tmp[33] = {0};
		memmove(hash_tmp, Sn_6FSHASH[idx], sizeof(hash_tmp)-1);

		pSyz_Sozoku->AddNew();
		pSyz_Sozoku->Sn_SEQ				= sn_seq;
		pSyz_Sozoku->Sn_NO				= Sn_NO[idx];
		pSyz_Sozoku->Sn_6FSJUSYO		= Sn_6FSJUSYO[idx];
		pSyz_Sozoku->Sn_6FSFURI			= Sn_6FSFURI[idx];
		pSyz_Sozoku->Sn_6FSNAME			= Sn_6FSNAME[idx];
		pSyz_Sozoku->Sn_6FSHASH.Format(hash_tmp);
		pSyz_Sozoku->Sn_6FSSYOKU		= Sn_6FSSYOKU[idx];
		pSyz_Sozoku->Sn_6FSTSUDUKI		= Sn_6FSTSUDUKI[idx];
		pSyz_Sozoku->Sn_6FSBIRTH		= Sn_6FSBIRTH[idx];
		pSyz_Sozoku->Sn_6FSTEL1			= Sn_6FSTEL1[idx];  
		pSyz_Sozoku->Sn_6FSTEL2			= Sn_6FSTEL2[idx];
		pSyz_Sozoku->Sn_6FSTEL3			= Sn_6FSTEL3[idx];
		pSyz_Sozoku->Sn_6FSHOTEI		= Sn_6FSHOTEI[idx];
		pSyz_Sozoku->Sn_6FSBUNSHI		= Sn_6FSBUNSHI[idx];
		pSyz_Sozoku->Sn_6FSBUNBO		= Sn_6FSBUNBO[idx];
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSKAGAKU, Sn_6FSKAGAKU[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSNOFUZEI, Sn_6FSNOFUZEI[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSNOFUCHIHO, Sn_6FSNOFUCHIHO[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSKANPUZEI, Sn_6FSKANPUZEI[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSKANPUCHIHO, Sn_6FSKANPUCHIHO[idx]);
		pSyz_Sozoku->Sn_6FSNOFUKANPUsw	= Sn_6FSNOFUKANPUsw[idx];
		pSyz_Sozoku->Sn_6FSGNAME		= Sn_6FSGNAME[idx];
		pSyz_Sozoku->Sn_6FSGSEL			= Sn_6FSGSEL[idx];
		pSyz_Sozoku->Sn_6FSSNAME		= Sn_6FSSNAME[idx];
		pSyz_Sozoku->Sn_6FSSSEL			= Sn_6FSSSEL[idx];
		pSyz_Sozoku->Sn_6FSYOKIN		= Sn_6FSYOKIN[idx];
		pSyz_Sozoku->Sn_6FSKOUZA		= Sn_6FSKOUZA[idx];
		pSyz_Sozoku->Sn_6FSKIGO1		= Sn_6FSKIGO1[idx];
		pSyz_Sozoku->Sn_6FSKIGO2		= Sn_6FSKIGO2[idx];
		pSyz_Sozoku->Sn_6FSYUBIN		= Sn_6FSYUBIN[idx];
		pSyz_Sozoku->Update();	
	}							
	return 0;					
}								

//-----------------------------------------------------------------------------
// 更新時の金額クリア
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		type		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH28SnFhyo60Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{

	Sn_6FSDEATH = 0;
	Sn_6FSDAIHYO.Empty();
	Sn_6FSLIMIT = 0;

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
		//int				Sn_SEQ[SOZOKU_MAX];
		Sn_NO[idx] = -1;
		Sn_6FSJUSYO[idx].Empty();
		Sn_6FSFURI[idx].Empty();
		Sn_6FSNAME[idx].Empty();
		memset( Sn_6FSHASH[idx], '\0', sizeof(Sn_6FSHASH[idx]));
		Sn_6FSSYOKU[idx].Empty();
		Sn_6FSTSUDUKI[idx].Empty();
		Sn_6FSBIRTH[idx] = 0;
		Sn_6FSTEL1[idx].Empty();
		Sn_6FSTEL2[idx].Empty();
		Sn_6FSTEL3[idx].Empty();
		Sn_6FSHOTEI[idx] = 0;
		Sn_6FSBUNSHI[idx].Empty();
		Sn_6FSBUNBO[idx].Empty();
		m_Arith.l_clear( Sn_6FSKAGAKU[idx] );
		m_Arith.l_clear( Sn_6FSNOFUZEI[idx] );
		m_Arith.l_clear( Sn_6FSNOFUCHIHO[idx] );
		m_Arith.l_clear( Sn_6FSKANPUZEI[idx] );
		m_Arith.l_clear( Sn_6FSKANPUCHIHO[idx] );
		Sn_6FSNOFUKANPUsw[idx] = 0;
		Sn_6FSGNAME[idx].Empty();
		Sn_6FSGSEL[idx] = 0;
		Sn_6FSSNAME[idx].Empty();
		Sn_6FSSSEL[idx] = 0; 
		Sn_6FSYOKIN[idx].Empty();
		Sn_6FSKOUZA[idx].Empty();
		Sn_6FSKIGO1[idx].Empty();
		Sn_6FSKIGO2[idx].Empty();
		Sn_6FSYUBIN[idx].Empty();
	}
	
	return 0;
}