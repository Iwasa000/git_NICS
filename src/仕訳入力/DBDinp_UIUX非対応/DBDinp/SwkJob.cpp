
#include "stdafx.h"

#include "Swkjob.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////////////

#define SBR_MASK( cod )		(cod&0xffffff00)		// 小分類
#define SHKN_CD				0x00000001				// 資金のコード



CSwkJob::CSwkJob()
{
	m_pNPb = NULL;
	m_SHKN_OPN_MODE = -1;

	m_bKrmod = FALSE;
}

CSwkJob::~CSwkJob()
{
}


// 初期処理
int CSwkJob::Initial()
{
	//非営利法人クラス作成
	int ret = 0;

	try {
		m_pNPb = new CDBNpSub;
	}
	catch( CMemoryException* me )
	{
		me->Delete();
		return -1;
	}
	m_pNPb->ExitMode(1);

	char	sqlsvr[256],apno[20],cocode[20],kikan[20];
	if( PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan ) == 4 )	{
		if( m_pNPb->SetMasterDB( SMDB_VERS, sqlsvr, atoi( apno) , atoi( cocode) , atoi( kikan )) == -1 ) {
			ret = -1;
			goto ENDJOB;
		}
	}
	else {
		ret = -1;
		goto ENDJOB;
	}

	// 1.非営利法人ボリュームラベルテーブル
	if( m_pNPb->NpVolumeOpen() == ERR ) {
		ret = -1;
		goto ENDJOB;
	}
	// 非営利 科目名称テーブル
	if( m_pNPb->NpKamokuMeisyoOpen() == ERR ) {
		ret = -1;
		goto ENDJOB;
	}
	//自動仕訳をしない
	if( !(m_pNPb->m_pNpvol->m_npsgn[2] & 0x01) ) {
		m_SHKN_OPN_MODE = -2;
		goto ENDJOB;
	}
	else {
		// 非営利 自動仕訳テーブル
		if( m_pNPb->NpAutOpen() == ERR ) {
			ret = -1;
			goto ENDJOB;
		}
		m_SHKN_OPN_MODE = MemdataMake();
	}


ENDJOB:
	if( m_pNPb != NULL ) {
		m_pNPb->NpVolumeClose();
		m_pNPb->NpKamokuMeisyoClose();
		m_pNPb->NpAutClose();
		delete m_pNPb;
	}

	return ret;
}


int CSwkJob::MemdataMake()
{

	m_pNPb->m_pNpknrec->MoveFirstEx();

	m_Sisan[0].chkcod = 0x00000001;
	m_Sisan[0].chkArray.RemoveAll();
	m_Sisan[1].chkcod = 0x00000002;
	m_Sisan[1].chkArray.RemoveAll();
	m_Husai[0].chkcod = 0x00000003;
	m_Husai[0].chkArray.RemoveAll();
	m_Husai[1].chkcod = 0x00000004;
	m_Husai[1].chkArray.RemoveAll();

	DWORD dwCode;
	BYTE	sgn;

	// 資金科目
	if( m_pNPb->m_pNpknrec->st != -1 ) {
		do {
			dwCode = _atoh( (char*)(LPCTSTR)m_pNPb->m_pNpknrec->m_npkncd );

			if( m_pNPb->m_pNpknrec->m_npksg[0] & 0x10 ) {
				if( m_pNPb->m_pNpknrec->m_npksg[0] & (BYTE)0x80 ) {		//変更可能
					sgn = (m_pNPb->m_pNpknrec->m_npksg[1] & 0x0f);	//変更後
				}
				else {
					sgn = (m_pNPb->m_pNpknrec->m_npksg[0] & 0x0f);
				}

				switch( sgn ) {
				case 0:
					m_Sisan[0].chkArray.Add( dwCode );
					break;
				case 1:
					m_Sisan[1].chkArray.Add( dwCode );
					break;
				case 2:
					m_Husai[0].chkArray.Add( dwCode );
					break;
				case 3:
					m_Husai[1].chkArray.Add( dwCode );
					break;
				default:
					break;
				}
			}

			m_pNPb->m_pNpknrec->MoveNextEx();

		} while( m_pNPb->m_pNpknrec->st == 0 );
	}

	// 自動仕訳
	m_NpAuto.RemoveAll();
	NPAUTO_SWK nps;
	m_pNPb->m_pNpaut->MoveFirstEx();

	if( m_pNPb->m_pNpaut->st != -1 ) {
		do {
			if( !(m_pNPb->m_pNpaut->m_npavoid & 0x01) ) {

				dwCode = _atoh( (char*)(LPCTSTR)m_pNPb->m_pNpaut->m_npadb[0] );
				nps.inpDeb = dwCode;
				dwCode = _atoh( (char*)(LPCTSTR)m_pNPb->m_pNpaut->m_npacr[0] );
				nps.inpCre = dwCode;
		
				dwCode = _atoh( (char*)(LPCTSTR)m_pNPb->m_pNpaut->m_npadb[1] );
				nps.autDeb = dwCode;
				dwCode = _atoh( (char*)(LPCTSTR)m_pNPb->m_pNpaut->m_npacr[1] );
				nps.autCre = dwCode;

				m_NpAuto.Add( nps );

			}

			m_pNPb->m_pNpaut->MoveNextEx();
		} while( m_pNPb->m_pNpaut->st == 0 );
	}

	return 0;
}


// 自動仕訳処理をするか？
int CSwkJob::IsAutoSwk()
{
	// ファイルなしか 業種が違う
	if( m_SHKN_OPN_MODE < 0 ) return 0;

	return 1;
}


// 自動仕訳
//	返送値：該当の自動仕訳数(複数ある場合は選択させる)
//
int CSwkJob::AutoSwk( CDBINPDataRec* atrec, CDBINPDataRec* crec )
{
	int st, i;
	st = 0;

	// 切返しできる業種・サインか？
	if( !IsAutoSwk() ) return 0;

	INT_PTR	cnt;
	cnt = m_NpAuto.GetCount();

	DWORD dwDeb, dwCre;

	dwDeb = _atoh( (LPSTR)(LPCTSTR)crec->m_dbt );
	dwCre = _atoh( (LPSTR)(LPCTSTR)crec->m_cre );

	m_Npsame.RemoveAll();

	for( i = 0; i < cnt; i++ ) {
		// 該当仕訳検索
		// 借方
		if( m_NpAuto[i].inpDeb == SHKN_CD ) {
			if( IsShikin( crec, 0 ) == 0 )	continue;
		}
		else if( m_NpAuto[i].inpDeb != dwDeb ){
			continue;
		}
		// 貸方
		if( m_NpAuto[i].inpCre == SHKN_CD ) {
			if( IsShikin( crec, 1 ) == 0 )	continue;
		}
		else if( m_NpAuto[i].inpCre != dwCre ){
			continue;
		}

		m_Npsame.Add(m_NpAuto[i]);

		atrec->Reset();
		atrec->m_seq = crec->m_seq + 1;

		atrec->m_ddate	= crec->m_ddate;
		atrec->m_mofs	= crec->m_mofs;
		atrec->m_ksign	= crec->m_ksign;

		atrec->m_cno	= crec->m_cno;

		atrec->m_dbmn	= crec->m_dbmn;
		atrec->m_dkno.Empty();
		atrec->m_dbt.Format( "%08x", m_NpAuto[i].autDeb );
		atrec->m_dbr	= -1;

		atrec->m_cbmn	= crec->m_cbmn;
		atrec->m_ckno.Empty();
		atrec->m_cre.Format( "%08x", m_NpAuto[i].autCre );
		atrec->m_cbr	= -1;

		/* 摘要のみセット */
		atrec->m_tekiyo = crec->m_tekiyo;

		/* 証憑番号 */
		atrec->m_skn	= crec->m_skn;

		st++;
		IsKrmod() = TRUE;	// 自動仕訳中サインＯＮ
	}

	return st;
}


// 資金科目か？
int CSwkJob::IsShikin( CDBINPDataRec* data, int dbcr_sg )
{
	DWORD kcd;
	SHKN_KMK* pSkm;
	int i , j, st;

	if(dbcr_sg == 0)	kcd = _atoh( (LPSTR)(LPCTSTR)data->m_dbt );
	else				kcd = _atoh( (LPSTR)(LPCTSTR)data->m_cre );

	st = 0;	

	for( i = 0; i < 2; i++ ) {
		pSkm = (i == 0) ? &m_Sisan[0] : &m_Husai[0];	// 資金資産・負債

		for( j = 0; j < pSkm->chkArray.GetCount(); j++ ) {
			// 該当科目
			if( SBR_MASK( pSkm->chkArray[j] ) == SBR_MASK( kcd ) ) {
				st = 1;	break;
			}
		}

		if( st )	break;
	}

	return st;
}

// 対象元仕訳を保存
void CSwkJob::KeepCrec(CDBINPDataRec* crec)
{
	m_Moto = *crec;
}

// 対象元仕訳を取得
void CSwkJob::GetMotoRec(CDBINPDataRec* moto)
{
	*moto = m_Moto;
}

// 切返し仕訳を保存
void CSwkJob::KeepKrswk(CDBINPDataRec* pkrswk)
{
	m_Krirec = *pkrswk;
}

// 切返し仕訳を取得
void CSwkJob::GetKrswk(CDBINPDataRec* pkrswk )
{
	*pkrswk = m_Krirec;
}


void CSwkJob::GetAutoSwk(CDBINPDataRec* atrec, CDBINPDataRec* crec, int index)
{
	atrec->Reset();

	INT_PTR cnt = m_Npsame.GetCount();
	if (index >= cnt)
		return;

	atrec->m_seq = crec->m_seq + 1;

	atrec->m_ddate = crec->m_ddate;
	atrec->m_mofs = crec->m_mofs;
	atrec->m_ksign = crec->m_ksign;

	atrec->m_cno = crec->m_cno;

	atrec->m_dbmn = crec->m_dbmn;
	atrec->m_dkno.Empty();
	atrec->m_dbt.Format("%08x", m_Npsame[index].autDeb);
	atrec->m_dbr = -1;

	atrec->m_cbmn = crec->m_cbmn;
	atrec->m_ckno.Empty();
	atrec->m_cre.Format("%08x", m_Npsame[index].autCre);
	atrec->m_cbr = -1;

	/* 摘要のみセット */
	atrec->m_tekiyo = crec->m_tekiyo;

	/* 証憑番号 */
	atrec->m_skn = crec->m_skn;

}

#ifdef OLD_CLOSE

// 切返し仕訳のＳＥＱ調整
void CSwkJob::SeqAdjust(CDBINPDataRec* pData, CDBINPDataRec* crec)
{
	UINT seq;
	int t_l;

	t_l = l_retn();
	l_defn( 0x12 );

	l_let( &seq, &crec->darec._SEQ, 0x42 );
	seq++;
	l_let( &pData->darec._SEQ, &seq, 0x42 );	/* SEQ 加算 */
	l_defn( t_l );
}


// 仕訳のチェックをするか？
int CSwkJob::IsSwkCheck()
{
	// ファイルなしか 業種が違う
	if( m_SHKN_OPN_MODE < 0 ) return 0;

	// 仕訳のチェックサインＯＦＦ
	if( !(m_pZm->pVol1->SCH_CA2 & 0x01) )	return 0;

	return 1;
}


// 仕訳のチェック
//		
int CSwkJob::SwkCheck( CDBINPDataRec* data )
{
	int i;

	if( !IsSwkCheck() )	return 0;

	PCREC* pc;
	pc = m_PIrec.PCREC;

	// 既にチェック済みの仕訳の場合
	if( data->darec._SEQ == m_Chkinf.seq && data->darec._DBT == m_Chkinf.deb &&
		data->darec._CRE == m_Chkinf.cre && m_Chkinf.bChk )
		return 0;
	else {
		// 初めてチェックする
		m_Chkinf.seq = data->darec._SEQ;
		m_Chkinf.deb = data->darec._DBT;
		m_Chkinf.cre = data->darec._CRE;
		m_Chkinf.bChk = TRUE;	// チェックサインＯＮ
	}
	
	for( i = 0; i < m_PKH.PK_IPR; i++ ) {

		if( CheckSwkCode( (pc+i)->PC_DBT1, (pc+i)->PC_DBT2, data, 0 ) == 0 )
			continue;
		else {
			if( CheckSwkCode( (pc+i)->PC_CRE1, (pc+i)->PC_CRE2, data, 1 ) ) {
				return 1;
			}
		}
	}

	return 0;
}

// 仕訳のチェック
//		WORD chk1 ....... チェックコード１
//		WORD chk2 ....... チェックコード２
//		CDBINPDataRec* data ... データレコード
//		int dbcr_sg ..... 0 = 借方, 1 = 貸方
//
int CSwkJob::CheckSwkCode( WORD chk1, WORD chk2, CDBINPDataRec* data, int dbcr_sg )
{
	WORD kcd, scod, ecod;
	SHKN_KMK* pSkm;
	int i, j, st;

	kcd = (dbcr_sg == 0) ? data->darec._DBT : data->darec._CRE;

	st = 0;

	if( chk1 == 0x0500 || chk1 == 0x0600 || chk2 == 0x0500 || chk2 == 0x0600 ) { 
		// 正味財産増加の科目
		GetSmzaisanCod( 0x0500, &scod );
		GetSmzaisanCod( 0x0600, &ecod );

		if( memcmp( (BYTE*)&kcd, (BYTE*)&scod, 2 ) >= 0 && 
			memcmp( (BYTE*)&kcd, (BYTE*)&ecod, 2 ) <= 0 ) {
			st = 1;
		}
	}
	else if( chk1 == 0x0700 || chk1 == 0x0800 || chk2 == 0x0700 || chk2 == 0x0800 ) {
		// 正味財産減少の科目
		GetSmzaisanCod( 0x0700, &scod );
		GetSmzaisanCod( 0x0800, &ecod );

		if( memcmp( (BYTE*)&kcd, (BYTE*)&scod, 2 ) >= 0 && 
			memcmp( (BYTE*)&kcd, (BYTE*)&ecod, 2 ) <= 0 ) {
			st = 1;
		}
	}
	else {

		for( i = 0; i < 2; i++ ) {

			switch( (i==0) ? chk1 : chk2 ) {
			case 0x0100: pSkm = &m_Sisan[0];	// 資金資産
						 break;
			case 0x0200: pSkm = &m_Husai[0];	// 資金負債
						 break;
			case 0x0300: pSkm = &m_Sisan[1];	// 非資金資産
						 break;
			case 0x0400: pSkm = &m_Husai[1];	// 非資金負債
						 break;
			default:	 continue;
			}

			for( j = 0; j < pSkm->chknum; j++ ) {
				// 該当科目
				if( SBR_MASK( *(pSkm->pShkm+j) ) == SBR_MASK( kcd ) ) {
					st = 1;	break;
				}
			}

			if( st )	break;
		}

	}

	return st;

}


// 正味財産科目のコード取得
void CSwkJob::GetSmzaisanCod( WORD code, WORD* kmkcod )
{

	switch( code ) {
	case 0x0500:	// 正味財産増加科目の範囲　開始
		*kmkcod = m_PKH.PK_SZS;		break;
	case 0x0600:	//       〃　　　　　　　　終了
		*kmkcod = m_PKH.PK_SZE;		break;
	case 0x0700:	// 正味財産減少科目の範囲　開始
		*kmkcod = m_PKH.PK_SGS;		break;
	case 0x0800:	//       〃　　　　　　　　終了
		*kmkcod = m_PKH.PK_SGE;		break;
	default:
		*kmkcod = 0;
	}
}

#endif
