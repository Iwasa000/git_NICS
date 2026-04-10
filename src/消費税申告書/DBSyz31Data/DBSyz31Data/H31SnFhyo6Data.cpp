#include "StdAfx.h"
#include "H31SnFhyo6Data.h"

CH31SnFhyo6Data::CH31SnFhyo6Data(void)
{

	Sn_F6_DEATH = 0;
	Sn_F6_DAIHYO.Empty();
	Sn_F6_LIMIT = 0;
	Sn_6BS_USE = 0;
	memset(Sn_6BS_ADR, '\0', sizeof(Sn_6BS_ADR));
	memset(Sn_6BS_TELNO1, '\0', sizeof(Sn_6BS_TELNO1));
	memset(Sn_6BS_TELNO2, '\0', sizeof(Sn_6BS_TELNO2));
	memset(Sn_6BS_TELNO3, '\0', sizeof(Sn_6BS_TELNO3));
	Sn_6BS_NMFRI.Empty();
	Sn_6BS_NAME.Empty();
	Sn_6BS_SONOTA.Empty();

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
		Sn_NO[idx] = -1;
		Sn_F6_JUSYO[idx].Empty();
		Sn_F6_FURI[idx].Empty();
		Sn_F6_NAME[idx].Empty();
		memset( Sn_F6_HASH[idx],'\0', sizeof(Sn_F6_HASH[idx]));
		Sn_F6_SYOKU[idx].Empty();
		Sn_F6_TSUDUKI[idx].Empty();
		Sn_F6_BIRTH[idx] = 0;
		Sn_F6_TEL1[idx].Empty();
		Sn_F6_TEL2[idx].Empty();
		Sn_F6_TEL3[idx].Empty();
		Sn_F6_HOTEI[idx] = 0;
		Sn_F6_BUNSHI[idx].Empty();
		Sn_F6_BUNBO[idx].Empty();
		memset( Sn_F6_KAGAKU[idx],'\0', sizeof(Sn_F6_KAGAKU[idx]));
		memset( Sn_F6_NOFUZEI[idx],'\0', sizeof(Sn_F6_NOFUZEI[idx]));
		memset( Sn_F6_NOFUCHIHO[idx],'\0', sizeof(Sn_F6_NOFUCHIHO[idx]));
		memset( Sn_F6_KANPUZEI[idx],'\0', sizeof(Sn_F6_KANPUZEI[idx]));
		memset( Sn_F6_KANPUCHIHO[idx],'\0', sizeof(Sn_F6_KANPUCHIHO[idx]));
		Sn_F6_NOFUKANPUsw[idx] = 0;
		Sn_F6_GNAME[idx].Empty();
		Sn_F6_GSEL[idx] = 0;
		Sn_F6_SNAME[idx].Empty();
		Sn_F6_SSEL[idx] = 0;
		Sn_F6_YOKIN[idx].Empty();
		Sn_F6_KOUZA[idx].Empty();
		Sn_F6_KIGO1[idx].Empty();
		Sn_F6_KIGO2[idx].Empty();
		Sn_F6_YUBIN[idx].Empty();
	}

	m_Arith.l_defn( 0x16 );
}

CH31SnFhyo6Data::~CH31SnFhyo6Data(void)
{
}

int CH31SnFhyo6Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
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

		Sn_F6_DEATH = pFhyo60rec->Sn_6FSDEATH;
		Sn_F6_DAIHYO = pFhyo60rec->Sn_6FSDAIHYO;
		Sn_F6_LIMIT = pFhyo60rec->Sn_6FSLIMIT;

		// 事業承継
		Sn_6BS_USE = pFhyo60rec->Sn_6BS_USE;
		m_Util.cstring_to_char(Sn_6BS_ADR, pFhyo60rec->Sn_6BS_ADR, 80);

		// 電話番号
		char	telbf[64] ={0};
		if( pFhyo60rec->Sn_6BS_TELNO.GetLength() == 0 ){
			pFhyo60rec->Sn_6BS_TELNO = _T ("                ");
		}
		m_Util.cstring_to_char( (unsigned char *)telbf, pFhyo60rec->Sn_6BS_TELNO, 16 );
		m_Util.cstring_to_char( Sn_6BS_TELNO1, telbf, 6 );		// 電話番号　市外
		m_Util.cstring_to_char( Sn_6BS_TELNO2, &telbf[7], 4 );	// 電話番号　市内
		m_Util.cstring_to_char( Sn_6BS_TELNO3, &telbf[12], 4 );	// 電話番号　個人

		Sn_6BS_NMFRI = pFhyo60rec->Sn_6BS_NMFRI;
		Sn_6BS_NAME = pFhyo60rec->Sn_6BS_NAME;
		Sn_6BS_SONOTA = pFhyo60rec->Sn_6BS_SONOTA;
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

		int idx2				= pSyz_Sozoku->Sn_NO;
		Sn_NO[idx2]				= pSyz_Sozoku->Sn_NO;
		Sn_F6_JUSYO[idx2]		= pSyz_Sozoku->Sn_6FSJUSYO;
		Sn_F6_FURI[idx2]		= pSyz_Sozoku->Sn_6FSFURI;
		Sn_F6_NAME[idx2]		= pSyz_Sozoku->Sn_6FSNAME;
		strcpy_s(Sn_F6_HASH[idx2], sizeof(Sn_F6_HASH[idx2]), pSyz_Sozoku->Sn_6FSHASH);
		Sn_F6_SYOKU[idx2]		= pSyz_Sozoku->Sn_6FSSYOKU;
		Sn_F6_TSUDUKI[idx2]		= pSyz_Sozoku->Sn_6FSTSUDUKI;
		Sn_F6_BIRTH[idx2]		= pSyz_Sozoku->Sn_6FSBIRTH;
		Sn_F6_TEL1[idx2]		= pSyz_Sozoku->Sn_6FSTEL1;
		Sn_F6_TEL2[idx2]		= pSyz_Sozoku->Sn_6FSTEL2;
		Sn_F6_TEL3[idx2]		= pSyz_Sozoku->Sn_6FSTEL3;
		Sn_F6_HOTEI[idx2]		= pSyz_Sozoku->Sn_6FSHOTEI;
		Sn_F6_BUNSHI[idx2]		= pSyz_Sozoku->Sn_6FSBUNSHI;
		Sn_F6_BUNBO[idx2]		= pSyz_Sozoku->Sn_6FSBUNBO;
		m_Util.val_to_bin( Sn_F6_KAGAKU[idx2], pSyz_Sozoku->Sn_6FSKAGAKU );
		m_Util.val_to_bin( Sn_F6_NOFUZEI[idx2], pSyz_Sozoku->Sn_6FSNOFUZEI );
		m_Util.val_to_bin( Sn_F6_NOFUCHIHO[idx2], pSyz_Sozoku->Sn_6FSNOFUCHIHO );
		m_Util.val_to_bin( Sn_F6_KANPUZEI[idx2], pSyz_Sozoku->Sn_6FSKANPUZEI );
		m_Util.val_to_bin( Sn_F6_KANPUCHIHO[idx2], pSyz_Sozoku->Sn_6FSKANPUCHIHO );
		Sn_F6_NOFUKANPUsw[idx2] = pSyz_Sozoku->Sn_6FSNOFUKANPUsw;
		Sn_F6_GNAME[idx2]		= pSyz_Sozoku->Sn_6FSGNAME;
		Sn_F6_GSEL[idx2]		= pSyz_Sozoku->Sn_6FSGSEL;
		Sn_F6_SNAME[idx2]		= pSyz_Sozoku->Sn_6FSSNAME;
		Sn_F6_SSEL[idx2]		= pSyz_Sozoku->Sn_6FSSSEL;
		Sn_F6_YOKIN[idx2]		= pSyz_Sozoku->Sn_6FSYOKIN;
		Sn_F6_KOUZA[idx2]		= pSyz_Sozoku->Sn_6FSKOUZA;
		Sn_F6_KIGO1[idx2]		= pSyz_Sozoku->Sn_6FSKIGO1;
		Sn_F6_KIGO2[idx2]		= pSyz_Sozoku->Sn_6FSKIGO2;
		Sn_F6_YUBIN[idx2]		= pSyz_Sozoku->Sn_6FSYUBIN;
	}

	return 0;
}

int CH31SnFhyo6Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
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

	char	telbf[64]={0}, tel1[10]={0}, tel2[10]={0}, tel3[10]={0}, buf[512]={0};
	CString	tel;

	if( (pFhyo60rec->MoveFirst()==-1) || (pFhyo60rec->st==-1) ){
		pFhyo60rec->AddNew();
		pFhyo60rec->Sn_SEQ = sn_seq;
	}
	else{
		pFhyo60rec->Edit();
	}

	pFhyo60rec->Sn_6FSDEATH	= Sn_F6_DEATH;
	pFhyo60rec->Sn_6FSDAIHYO = Sn_F6_DAIHYO;
	pFhyo60rec->Sn_6FSLIMIT	= Sn_F6_LIMIT;

	// 事業承継
	pFhyo60rec->Sn_6BS_USE = Sn_6BS_USE;
	memset(buf, '\0', sizeof(buf));
	memset(buf, 0x20, 40);
	int kjln = kjlen(Sn_6BS_ADR, 40);
	memmove(buf, Sn_6BS_ADR, kjln);
	kjln = kjlen(&Sn_6BS_ADR[40], 40);
	memmove(&buf[40], &Sn_6BS_ADR[40], kjln);
	m_Util.char_to_cstring(&pFhyo60rec->Sn_6BS_ADR, (unsigned char *)buf, 80);

	// 電話番号
	memset(telbf, 0x20, sizeof(telbf));
	memmove(tel1, Sn_6BS_TELNO1, 6);
	memmove(tel2, Sn_6BS_TELNO2, 4);
	memmove(tel3, Sn_6BS_TELNO3, 4);
	tel = tel1;
	memmove(telbf, tel, tel.GetLength());
	tel = tel2;
	memmove(&telbf[7], tel, tel.GetLength());
	tel = tel3;
	memmove(&telbf[12], tel, tel.GetLength());
	m_Util.char_to_cstring(&pFhyo60rec->Sn_6BS_TELNO, (unsigned char *)telbf, 16);

	pFhyo60rec->Sn_6BS_NMFRI = Sn_6BS_NMFRI;
	pFhyo60rec->Sn_6BS_NAME = Sn_6BS_NAME;
	pFhyo60rec->Sn_6BS_SONOTA = Sn_6BS_SONOTA;

	pFhyo60rec->Update();

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
		memmove(hash_tmp, Sn_F6_HASH[idx], sizeof(hash_tmp)-1);

		pSyz_Sozoku->AddNew();
		pSyz_Sozoku->Sn_SEQ				= sn_seq;
		pSyz_Sozoku->Sn_NO				= Sn_NO[idx];
		pSyz_Sozoku->Sn_6FSJUSYO		= Sn_F6_JUSYO[idx];
		pSyz_Sozoku->Sn_6FSFURI			= Sn_F6_FURI[idx];
		pSyz_Sozoku->Sn_6FSNAME			= Sn_F6_NAME[idx];
		pSyz_Sozoku->Sn_6FSHASH.Format(hash_tmp);
		pSyz_Sozoku->Sn_6FSSYOKU		= Sn_F6_SYOKU[idx];
		pSyz_Sozoku->Sn_6FSTSUDUKI		= Sn_F6_TSUDUKI[idx];
		pSyz_Sozoku->Sn_6FSBIRTH		= Sn_F6_BIRTH[idx];
		pSyz_Sozoku->Sn_6FSTEL1			= Sn_F6_TEL1[idx];  
		pSyz_Sozoku->Sn_6FSTEL2			= Sn_F6_TEL2[idx];
		pSyz_Sozoku->Sn_6FSTEL3			= Sn_F6_TEL3[idx];
		pSyz_Sozoku->Sn_6FSHOTEI		= Sn_F6_HOTEI[idx];
		pSyz_Sozoku->Sn_6FSBUNSHI		= Sn_F6_BUNSHI[idx];
		pSyz_Sozoku->Sn_6FSBUNBO		= Sn_F6_BUNBO[idx];
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSKAGAKU, Sn_F6_KAGAKU[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSNOFUZEI, Sn_F6_NOFUZEI[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSNOFUCHIHO, Sn_F6_NOFUCHIHO[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSKANPUZEI, Sn_F6_KANPUZEI[idx]);
		m_Util.val_to_asci(&pSyz_Sozoku->Sn_6FSKANPUCHIHO, Sn_F6_KANPUCHIHO[idx]);
		pSyz_Sozoku->Sn_6FSNOFUKANPUsw	= Sn_F6_NOFUKANPUsw[idx];
		pSyz_Sozoku->Sn_6FSGNAME		= Sn_F6_GNAME[idx];
		pSyz_Sozoku->Sn_6FSGSEL			= Sn_F6_GSEL[idx];
		pSyz_Sozoku->Sn_6FSSNAME		= Sn_F6_SNAME[idx];
		pSyz_Sozoku->Sn_6FSSSEL			= Sn_F6_SSEL[idx];
		pSyz_Sozoku->Sn_6FSYOKIN		= Sn_F6_YOKIN[idx];
		pSyz_Sozoku->Sn_6FSKOUZA		= Sn_F6_KOUZA[idx];
		pSyz_Sozoku->Sn_6FSKIGO1		= Sn_F6_KIGO1[idx];
		pSyz_Sozoku->Sn_6FSKIGO2		= Sn_F6_KIGO2[idx];
		pSyz_Sozoku->Sn_6FSYUBIN		= Sn_F6_YUBIN[idx];
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
int CH31SnFhyo6Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{

	Sn_F6_DEATH = 0;
	Sn_F6_DAIHYO.Empty();
	Sn_F6_LIMIT = 0;

	// 事業承継
	Sn_6BS_USE = 0;
	memset( Sn_6BS_ADR, '\0', sizeof(Sn_6BS_ADR) );
	memset( Sn_6BS_TELNO1, '\0', sizeof(Sn_6BS_TELNO1) );
	memset( Sn_6BS_TELNO2, '\0', sizeof(Sn_6BS_TELNO2) );
	memset( Sn_6BS_TELNO3, '\0', sizeof(Sn_6BS_TELNO3) );
	Sn_6BS_NMFRI.Empty();
	Sn_6BS_NAME.Empty();
	Sn_6BS_SONOTA.Empty();

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
		Sn_NO[idx] = -1;
		Sn_F6_JUSYO[idx].Empty();
		Sn_F6_FURI[idx].Empty();
		Sn_F6_NAME[idx].Empty();
		memset( Sn_F6_HASH[idx], '\0', sizeof(Sn_F6_HASH[idx]));
		Sn_F6_SYOKU[idx].Empty();
		Sn_F6_TSUDUKI[idx].Empty();
		Sn_F6_BIRTH[idx] = 0;
		Sn_F6_TEL1[idx].Empty();
		Sn_F6_TEL2[idx].Empty();
		Sn_F6_TEL3[idx].Empty();
		Sn_F6_HOTEI[idx] = 0;
		Sn_F6_BUNSHI[idx].Empty();
		Sn_F6_BUNBO[idx].Empty();
		m_Arith.l_clear( Sn_F6_KAGAKU[idx] );
		m_Arith.l_clear( Sn_F6_NOFUZEI[idx] );
		m_Arith.l_clear( Sn_F6_NOFUCHIHO[idx] );
		m_Arith.l_clear( Sn_F6_KANPUZEI[idx] );
		m_Arith.l_clear( Sn_F6_KANPUCHIHO[idx] );
		Sn_F6_NOFUKANPUsw[idx] = 0;
		Sn_F6_GNAME[idx].Empty();
		Sn_F6_GSEL[idx] = 0;
		Sn_F6_SNAME[idx].Empty();
		Sn_F6_SSEL[idx] = 0; 
		Sn_F6_YOKIN[idx].Empty();
		Sn_F6_KOUZA[idx].Empty();
		Sn_F6_KIGO1[idx].Empty();
		Sn_F6_KIGO2[idx].Empty();
		Sn_F6_YUBIN[idx].Empty();
	}
	
	return 0;
}