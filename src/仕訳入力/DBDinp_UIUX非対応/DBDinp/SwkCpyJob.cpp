#include "stdafx.h"

#include "resource.h"
#include "SwkCpyJob.h"
#include "DBRecal8.h"
#include "CStRecordSet.h"

#include "input1.h"
#include "external.h"
#include "mstrw.h"
#include "BmnChk.h"

// CDBDinpView
#include "DBDinp.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"
extern class CDBDinpView *p_DBDinpView;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _MDFIMG_	// 05.14 /10

extern CBmnHaniCheck	BmChk;


CSWKCpySrc::CSWKCpySrc()
{
	// メモリマップドファイルの生成
	m_hMapping = ::CreateFileMapping(
		(HANDLE)INVALID_HANDLE_VALUE,	// 共有メモリの場合
		NULL,							// セキュリティ属性。NULLでよい
		PAGE_READWRITE,					// プロテクト属性を読み書き可能に指定
		0,								// ファイルサイズの上位32ビット
		SIZE_SHAREMEM,					// ファイルサイズの下位32ビット
		NAME_SHAREMEM );				// メモリマップドファイルの名前

	m_bExist = FALSE;
	m_pMappingView	= NULL;
	m_pMutex		= NULL;

	// エラー
	DWORD dwErr;
	if( m_hMapping == NULL ) {
		dwErr = ::GetLastError();
	}
	else {
		dwErr = GetLastError();
		if( dwErr == ERROR_ALREADY_EXISTS ) {
			// 既にコピー元情報がある。
			m_bExist = TRUE;
		}
	}

	// プロセス内のアドレス空間にファイルのビューをマップ
	m_pMappingView = ::MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, 0, 0, SIZE_SHAREMEM);

//MyTrace( "m_pMappingView = %08x bExist[%d]", m_pMappingView, m_bExist );

	// ミューテックスオブジェクトの生成
	m_pMutex = new CMutex(FALSE, "SHM_SWKCPY_MUTEX");

	m_pDESzm	= NULL;
	m_pSRCzm	= NULL;
	pKMATSET8	= NULL;
	m_swImg		= 0;
	m_swAbort	= 0;

	m_pMsgDlg	= NULL;
}



CSWKCpySrc::~CSWKCpySrc()
{
	// ★ファイルのビューをアンマップし、マップのハンドルをクローズ
	BOOL b = ::UnmapViewOfFile(m_pMappingView);
	::CloseHandle(m_hMapping);

	// ★ミューテックスオブジェクトの破棄
	if( m_pMutex != NULL )
		delete m_pMutex;

	if( pKMATSET8 != NULL ) {
		delete pKMATSET8;
		pKMATSET8 = NULL;
	}
}


// メモリマップドファイル の 内容を バイナリでトレース
void CSWKCpySrc::TraceMappingView()
{
	BYTE* pByte;
	pByte = (BYTE*)m_pMappingView;

	int idx, a, n;
	char dbf[256];
	CString str;

	for( a = 0; a < 100; a++ ) {
		idx = a * 16;
		str.Format( "%03d:", a );

		for( n = 0; n < 16; n++ ) {
			sprintf_s( dbf, sizeof dbf, "%02x ", pByte[idx+n] );
			str += dbf;
		}
		MyTrace( "%s", str );
	}
}


//
//	共有メモリの内容を取得
//
void CSWKCpySrc::SrcDataRead() 
{
	// ★共有メモリの内容を取得。このときミューテックスオブジェクトを
	// 使用して同期処理をおこなう
	m_pMutex->Lock(INFINITE);
	m_pScpySrc = (SWKCPY_SRC*)m_pMappingView;

	m_pMutex->Unlock();

/*
	MyTrace( "@SrcDataRead sc_sqlsvr = %s\n",	m_pScpySrc->sc_sqlsvr );
	MyTrace( "@SrcDataRead sc_apno = %s\n",		m_pScpySrc->sc_apno );
	MyTrace( "@SrcDataRead sc_cocode = %s\n",	m_pScpySrc->sc_cocode );
	MyTrace( "@SrcDataRead sc_kikan = %s\n",	m_pScpySrc->sc_kikan );
	MyTrace( "@SrcDataRead sc_cnt = %d\n",		m_pScpySrc->sc_cnt );
*/
}

//
//	共有メモリへ書き込み
//
void CSWKCpySrc::SrcDataWrite(SWKCPY_SRC* scd ) 
{
/*
MyTrace( "@SrcDataWrite sc_sqlsvr = %s\n", scd->sc_sqlsvr );
MyTrace( "@SrcDataWrite sc_apno = %s\n", scd->sc_apno );
MyTrace( "@SrcDataWrite sc_cocode = %s\n", scd->sc_cocode );
MyTrace( "@SrcDataWrite sc_kikan = %s\n", scd->sc_kikan );
MyTrace( "@SrcDataWrite sc_cnt = %d\n", scd->sc_cnt );
*/
	// ★共有メモリへ書き込む。このときミューテックスオブジェクトを
	// 使用して同期処理をおこなう
	m_pMutex->Lock(INFINITE);

	int size = offsetof(SWKCPY_SRC, sc_seq);

	memcpy(m_pMappingView, scd, size );
	for( int i = 0; i < scd->sc_cnt; i++ ) {
		((SWKCPY_SRC*)m_pMappingView)->sc_seq[i] = scd->sc_seq[i];
	}

	m_pMutex->Unlock();
}


// 
BOOL CSWKCpySrc::SrcMappingCondition()
{
	if( m_hMapping == NULL )
		return FALSE;

	return TRUE;
}

//
//	コピー先・コピー元の財務クラスをセット
//
int CSWKCpySrc::SetCpyZmsub( CDipZmSub* pDes, CDBZmSub* pSrc, CWnd* pParent )
{
	m_pDESzm	= pDes;
	m_pSRCzm	= pSrc;
	m_pParent	= pParent;

	m_swImg = 0;

	if( pKMATSET8 != NULL ) {
		delete pKMATSET8;
		pKMATSET8 = NULL;
	}

	CString strmsg;
	if( (pKMATSET8 = new CKmatset8) == NULL ) {
		strmsg = _T(" 科目拡張用変換テーブルのメモリ確保が出来ません。");
		ICSMessageBox(strmsg);
		return -1;
	}

	CString msgbuf;
	//業種が異なる場合はコピーできないので、変換テーブル作成しない。
	if( CopyGyosyuCheck( msgbuf ) == 0 ) {

		// 拡張科目変換テーブル作成
		int ret = pKMATSET8->inztbl( m_pSRCzm, m_pDESzm );
		if( ret == -1 )
		{	
			strmsg = _T(" 科目拡張用変換テーブルの作成が出来ません。");
			ICSMessageBox(strmsg);
			return -1;
		}
	}

	if ( (m_pSRCzm->zvol->ocr_idev&0x0f) == 3)  {		// [抽出元]イメージあり

		if ((m_pDESzm->zvol->ocr_idev&0x0f) == 3)
			m_swImg = 1;		// [抽出先]イメージあり
		else  
			m_swImg = 2;		// [抽出先]イメージなし
	}

	return 0;
}


void CSWKCpySrc::SetDbtoMd2( CDBDATA* rec )
{
	CVolDateDB Vdate;
	int offset = 0;
	BYTE date[10] = {0};

	if( m_pSRCzm->dbdata->seq <= 0 )	return;

	// 04.30 '07					
	rec->owner = m_pSRCzm->dbdata->owner; 

	rec->seq = m_pSRCzm->dbdata->seq;
	rec->ksign = m_pSRCzm->dbdata->ksign;
	rec->ddate = m_pSRCzm->dbdata->ddate; 

//////////////////////////////////////////////////////////////////////////
#ifdef CLOSE	
	int_bcd( date, rec->ddate%10000 , 2 );

	// 決算期間が違う場合は、受け側の月オフセットを
	// サイセットする必要がある。
	// 03.30 '09
	/////////////////////////////////////////////////////////
	if( (m_pDESzm->zvol->cs_ymd == m_pSRCzm->zvol->cs_ymd) &&
			    (m_pDESzm->zvol->ee_ymd == m_pSRCzm->zvol->ee_ymd) )
	{
		rec->mofs = m_pSRCzm->dbdata->mofs;	
	}
	else
	{

		if( Vdate.db_vd_offset( date, &offset, m_pDESzm ) == -1 ) 
		{
			ICSMessageBox( "月のオフセットを取得できません！ ");
			return;
		}
		rec->mofs = offset; // m_pSRCzm->dbdata->mofs;		
	}
#endif
	///////////////////////////////////////////////////////////////////////////


	//
	//
	////////////////////////////////////////////////////////

	rec->mofs = m_pSRCzm->dbdata->mofs;	
	rec->cno = m_pSRCzm->dbdata->cno;

	// 04.30 '07 部門
	if( (m_pSRCzm->zvol->bm_isw) && (!m_pDESzm->zvol->bm_isw) ) 
		rec->dbmn = -1;
	else
		rec->dbmn = m_pSRCzm->dbdata->dbmn;

	rec->dkno = m_pSRCzm->dbdata->dkno;

	// コード検索
	// 06.15 '09
	ULONG SCOD = 0;
	ULONG DCOD = 0;
	ULONG RETCD = 0;
		
//	Apt_AscToBcd( (char*)&SCOD, (LPTSTR)LPCTSTR( m_pSRCzm->dbdata->dbt ), 8 );
	SCOD = _atoh( (LPSTR)(LPCTSTR)m_pSRCzm->dbdata->dbt );
	hl_rev( &SCOD, 4 );

	int RETN = 1;
	RETN = pKMATSET8->GetDstKcd( SCOD, &RETCD );
	
	DCOD = RETCD;

	if( !RETN && (SCOD != DCOD) )
	{
		hl_rev( (char*)&RETCD, 4 );
		rec->dbt.Format("%08x",RETCD ); 
	}
	else
	{
		// 07.07 '09
		if( RETN == -2 )
			rec->dbt.Empty();
		else
			rec->dbt = m_pSRCzm->dbdata->dbt;
	}

	// 枝番 04.30 '07
	if( (m_pSRCzm->zvol->br_sw ) && (!m_pDESzm->zvol->br_sw) ) 
		rec->dbr = -1;
	else
		rec->dbr = m_pSRCzm->dbdata->dbr;
	
	// 部門 04.30 '07
	if( (m_pSRCzm->zvol->bm_isw) && (!m_pDESzm->zvol->bm_isw) )
		rec->cbmn = -1;
	else
		rec->cbmn	= m_pSRCzm->dbdata->cbmn;
	
	rec->ckno	= m_pSRCzm->dbdata->ckno;

	// 06.15 '09
//	Apt_AscToBcd( (char*)&SCOD, (LPTSTR)LPCTSTR( m_pSRCzm->dbdata->cre ), 8 );
	SCOD = _atoh( (LPSTR)(LPCTSTR)m_pSRCzm->dbdata->cre );
	hl_rev( &SCOD, 4 );

	RETN = 1;
	RETN = pKMATSET8->GetDstKcd( SCOD, &RETCD );	
	DCOD = RETCD;

	if( !RETN && (SCOD != DCOD) )		
	{
		hl_rev( (char*)&RETCD, 4 );
		rec->cre.Format("%08x",RETCD ); 
	}
	else
	{
		// 07.07 '09 
		if( RETN == -2 )
			rec->cre.Empty();
		else
			rec->cre = m_pSRCzm->dbdata->cre;
	}
	// 枝番 04.30 '07	
	if( (m_pSRCzm->zvol->br_sw ) && (!m_pDESzm->zvol->br_sw) )
		rec->cbr = -1;
	else
		rec->cbr = m_pSRCzm->dbdata->cbr;

	rec->val = m_pSRCzm->dbdata->val;
	rec->zei = m_pSRCzm->dbdata->zei;	
	rec->dsign.Copy( m_pSRCzm->dbdata->dsign );

	rec->dtype = m_pSRCzm->dbdata->dtype;
	
	rec->tekiyo = m_pSRCzm->dbdata->tekiyo;

	rec->imgsq = m_pSRCzm->dbdata->imgsq;
	rec->snumber = m_pSRCzm->dbdata->snumber;
	rec->tdate = m_pSRCzm->dbdata->tdate;
	rec->edate = m_pSRCzm->dbdata->edate;
	rec->udate = m_pSRCzm->dbdata->udate;
	
	rec->tsign.Copy( m_pSRCzm->dbdata->tsign );
	
	rec->hjiyu = m_pSRCzm->dbdata->hjiyu;

	rec->taika = m_pSRCzm->dbdata->taika; // 対価金額
	
	// 10.08 '08
	//rec->tsign.Copy( m_pSRCzm->dbdata->tsign ) );

	// 05.22 '08
	// 付箋コメント
	rec->tag = m_pSRCzm->dbdata->tag;

	// 10.08 '08
	// 内部伝票番号
	rec->icno = m_pSRCzm->dbdata->icno;
	rec->skn = m_pSRCzm->dbdata->skn;

	// インボイス対応
	rec->invno = m_pSRCzm->dbdata->invno;

	// 電子記録債権対応
	rec->rnumber = m_pSRCzm->dbdata->rnumber;
}



void CSWKCpySrc::SetMdtoDESdbdata( CDBDATA* rec )
{
	m_pDESzm->dbdata->seq	=	rec->seq;

	m_pDESzm->dbdata->ksign	=	rec->ksign;
	m_pDESzm->dbdata->ddate	=	rec->ddate;
	m_pDESzm->dbdata->mofs	=	rec->mofs;
	m_pDESzm->dbdata->cno	=	rec->cno;
	m_pDESzm->dbdata->dbmn	=	rec->dbmn;
	m_pDESzm->dbdata->dkno	=	rec->dkno;
	m_pDESzm->dbdata->dbt	=	rec->dbt;
	m_pDESzm->dbdata->dbr	=	rec->dbr;
	m_pDESzm->dbdata->cbmn	=	rec->cbmn;
	m_pDESzm->dbdata->ckno	=	rec->ckno;
	m_pDESzm->dbdata->cre	=	rec->cre;
	m_pDESzm->dbdata->cbr	=	rec->cbr;
	m_pDESzm->dbdata->val	=	rec->val;
	m_pDESzm->dbdata->zei	=	rec->zei;

	m_pDESzm->dbdata->dsign.Copy( rec->dsign );
	m_pDESzm->dbdata->owner	=	m_pDESzm->DBUserNo();
	m_pDESzm->dbdata->dtype	=	0;	//入力タイプ初期化

	m_pDESzm->dbdata->tekiyo	=	rec->tekiyo;
	m_pDESzm->dbdata->imgsq		=	rec->imgsq;
	m_pDESzm->dbdata->snumber	=	rec->snumber;
	m_pDESzm->dbdata->tdate	=	rec->tdate;
//	m_pDESzm->dbdata->edate	=	rec->edate;
//	m_pDESzm->dbdata->udate	=	rec->udate;

	CString str = m_pDESzm->DBGetdate().Left( 8+2 );	// "YYYY-MM-DD"
	str.Remove( '-' );						// "YYYYMMDD"
	m_pDESzm->dbdata->edate	=	atoi( (LPCTSTR)str );
	m_pDESzm->dbdata->udate	=	-1;

//	m_pDESzm->dbdata->icno	=	rec->icno;	// 内部伝票番号
	m_pDESzm->dbdata->icno	=	-1;
	m_pDESzm->dbdata->skn	=	rec->skn;	// 資金繰り科目
	m_pDESzm->dbdata->tag	=	rec->tag;	// 付箋コメント
	m_pDESzm->dbdata->tsign.Copy( rec->tsign );
	m_pDESzm->dbdata->hjiyu		=	rec->hjiyu;	// 変動事由
	m_pDESzm->dbdata->taika		=	rec->taika;	// 対価金額
	
	m_pDESzm->dbdata->cuser		=	-1;		// 変更ユーザ

	// インボイス対応
	m_pDESzm->dbdata->invno		=	rec->invno;	// 登録番号

	// 電子記録債権対応
	m_pDESzm->dbdata->rnumber = rec->rnumber;
}


// 07.30 '08
int CSWKCpySrc::CheckData2( CDBDATA * pDataRec, int* pEflg)
{
	int	result = -1;
	int offset = 0;

//	BYTE*	pMtbl = m_pMFtbl;

	CVolDateDB Vdate;
	VDPAC vpac;

	// DB VOL1*	pDvl1 = m_pDESzm->pVol1;
	// DB VOL2*	pDvl2 = m_pDESzm->pVol2;
	// DB DAREC* pDrec = (DAREC*)&pDataRec->darec;

	CDBDATA *pDrec = pDataRec;

	int		eFlag = 0;
	BYTE	ymd[10]={0}, date[10] = {0};
	// EZREC	nErec;
	CString filter;

	while (1)  {
		// 抽出元とのチェック
		eFlag = 1;		// 対象外仕訳

#ifdef _DPICK_CLOSE
		if (pMtbl != NULL)  {

			// 月指定時
			// if( m_pSRCzm->vd_ofst(&pDrec->_MM, &sofs ) != 0 )  break;
			int_bcd( date, pDrec->ddate%10000 , 2 );

#ifdef DBCLOSE
			if( Vdate.db_vd_offset( date, &sofs, m_pSRCzm ) != 0 ) { break; }
#endif		
			// if (!(*(pMtbl+sofs) & 1))  break;				// ≠ 指定月
			if (!(*(pMtbl+ (pDrec->mofs) ) & 1))  break;		// ≠ 指定月

		}
		else  {
			// ＳＥＱ指定時
			// MD		
			if (m_SeqTbl[0] != 0)  {

				// if (memcmp(&pDrec->m_seq, &m_SeqTbl[0], 2) < 0)  break;	// ＳＥＱ範囲外
				// DB
				// if (memcmp(&pDrec->m_seq, &m_SeqTbl[0], 6) < 0)  break;	// ＳＥＱ範囲外

				if( pDrec->seq < m_SeqTbl[0] )  break;	// ＳＥＱ範囲外
			

			}
			if (m_SeqTbl[1] != 0)  {
				
				// if (memcmp(&pDrec->m_seq, &m_SeqTbl[1], 2) > 0)  break;	// ＳＥＱ範囲外
				// DB	
				// if (memcmp(&pDrec->m_seq, &m_SeqTbl[1], 6) > 0)  break;	// ＳＥＱ範囲外
				if( pDrec->seq > m_SeqTbl[1] )  break;	// ＳＥＱ範囲外
			}
		}
#endif


		eFlag = 2;		// 取消仕訳
		if (pDrec->dsign[0] & 1)  break;
			// 抽出先とのチェック
		eFlag = 3;		// 不良仕訳

		DWORD kcd;

		DBKNREC*	pKnrec;

		//if (m_pDESzm->PjisToKnrec( pDrec->m_dbt ) == NULL)  break;
		//if (m_pDESzm->PjisToKnrec( pDrec->m_cre ) == NULL)  break;
#ifdef CLOSE
		// 10.13 '06
		m_pDESzm->KamokuMeisyoRead( 0 );
#endif

		// 受け側が不使用科目に設定されている場合は抽出しない
		// knvoid
		kcd = _atoh( (char*)(LPCTSTR)pDrec->dbt );
		hl_rev( (char *)&kcd, 4 );
	
		// if (m_pDESzm->PjisToKnrec( kcd ) == NULL)	break;
		pKnrec = m_pDESzm->PjisToKnrec( kcd );

		if( pKnrec == NULL )
			break;

		// 不使用科目
		if( pKnrec->knvoid == 1 )
			break;

		kcd = _atoh( (char*)(LPCTSTR)pDrec->cre );
		hl_rev( (char *)&kcd, 4 );
		
		// if (m_pDESzm->PjisToKnrec( kcd ) == NULL)	break;
		pKnrec = m_pDESzm->PjisToKnrec( kcd ); 
	
		if( pKnrec == NULL)	
			break;

		// 不使用科目
		if( pKnrec->knvoid == 1 )
			break;

		eFlag = 4;		// 日付範囲外仕訳
		// DBmemmove( date, &pDrec->m_ddate, 2);
		// int_bcd( date, pDrec->m_ddate%100 , 2 );
		int mmdd = pDrec->ddate%10000;
		int gen, year, ysgn;
		char ddate[40];

		int uru_chgsgn = 0;

		// うるう年のチェック
		if( mmdd == 229 ) {
			int yy, mm;

			pDBzm->minfo->MoveFirst();
			do {
				yy = pDBzm->minfo->smd / 10000;
				mm = (pDBzm->minfo->smd % 10000) / 100;
				if( mm == 2 ) {
					// うるう年のチェック
					if( !uru_year(yy) ) {
						// 2月28日にする
						mmdd = 228;
						yy = pDrec->ddate / 10000;
						pDrec->ddate = (yy*10000)+ mmdd;

						uru_chgsgn = 1;
					}
					break;
				}
			} while( pDBzm->minfo->MoveNext() == 0 );
		}
		if( pDrec->ksign ) 
		{
			// 同じ決算期間
			if( (m_pDESzm->zvol->cs_ymd == m_pSRCzm->zvol->cs_ymd) &&
			    (m_pDESzm->zvol->ee_ymd == m_pSRCzm->zvol->ee_ymd) )
			{
				if( m_pDESzm->minfo->SetAbsolutePosition( pDrec->mofs+1 ) == 0 )
				{
					switch ( m_pDESzm->minfo->kes_sgn ) 
					{				
					case  1:	mmdd = mmdd%100;	mmdd += 6100;	break;
					case  2:	mmdd = mmdd%100;	mmdd += 6200;	break;
					case  3:	mmdd = mmdd%100;	mmdd += 6300;	break;
					case  4:	mmdd = mmdd%100;	break;
					default:	break;
					}
				}
			}
			else
			{
				int i;
				int set; 

				for( set = i = 0; i < 16; i++ )
				{
					// 抽出先の月別情報の決修サインON月のと同じ月があれば
					// 該当決修月をセットする( 四半期の順番は抽出先に合わせる )
					if( m_pDESzm->minfo->SetAbsolutePosition( i+1 ) == -1 )
						continue;

					// 決修月
					if( (m_pDESzm->minfo->kes_sgn >= 1) && 
						(m_pDESzm->minfo->kes_sgn <= 4) && (m_pDESzm->minfo->insw &0x01) ) 
					{	

						if( m_pSRCzm->minfo->SetAbsolutePosition( pDrec->mofs+1 ) == 0 )
						{
							int Dmd = m_pDESzm->minfo->emd%10000;
							int Smd = m_pSRCzm->minfo->emd%10000;
							//うるう年と通常年の２月末は同じとする
							if( (Dmd == 228 && Smd == 229) || (Dmd == 229 && Smd == 228) ) {
								Smd = Dmd = 228;
							}

							if( Dmd == Smd )
							{
								switch ( m_pDESzm->minfo->kes_sgn )
								{
								case  1:	mmdd = mmdd%100;	mmdd += 6100;	
									pDrec->mofs = i;	pDrec->ksign = 1;
									set = 1;					
									break;

								case  2:	mmdd = mmdd%100;	mmdd += 6200;

									pDrec->mofs = i;	pDrec->ksign = 1;
									set = 1;					
									break;

								case  3:	mmdd = mmdd%100;	mmdd += 6300;	
									pDrec->mofs = i;	pDrec->ksign = 1;
									set = 1;
									break;

								case  4:	mmdd = mmdd%100;	
									pDrec->mofs = i;	pDrec->ksign = 1;
									set = 1;
								break;

								default:	break;
								}
								break;
							}
						}
					}
				}
				// 該当決修なし
				if( !set )
				{
					// 決修サインOFF オフセット変更
					Vdate.db_vd_dpacGen( 0, 0, pDrec->ddate, &vpac, m_pDESzm );
					pDrec->mofs = vpac.Doffset;
					pDrec->ksign = (vpac.Dsgn & 0x80) ? 1 : 0;
						
				}

			}
		}

		// DB if (m_pDESzm->vd_chek(date) != 0)  break;		// 該当月日処理状態チェック
		if( Vdate.db_vd_check( 0, mmdd, m_pDESzm ) != 0)	break;				

		// if (m_pDESzm->vd_sign(date, ymd) != 0)  break;		// 年度越えサイン付き年月日
		// DB	
		// if (db_vd_yexd( 0, mmdd, m_pDESzm ) != 0) break;

		// 月日より年度を得る 
		int gengo, d_gengo;
		if( Vdate.db_vd_yearGen( 0, mmdd, &gengo, &gen, &ysgn, m_pDESzm ) != 0) break;
		// 西暦
		if( Vdate.db_datecnvGen( gengo, gen, &d_gengo, &year, 1, 2 ) != 0 ) break;

		memset( ddate, '\0', sizeof( ddate ) );

		mmdd = pDrec->ddate%10000;

		wsprintf( ddate,"%04d%04d", year, mmdd );  

		year = atoi( ddate );

		if( pDrec->ddate != year )
		{
			pDrec->ddate = year;
		}

// 決算期間が違う場合のみに変更				
#ifdef CLOSE
		/////////////////////////////////////////////////////////////////
		// 決算期間が違う場合にオフセット
		 if( (m_pDESzm->zvol->cs_ymd == m_pSRCzm->zvol->cs_ymd) &&
			 (m_pDESzm->zvol->ee_ymd == m_pSRCzm->zvol->ee_ymd) );
		 else
		 {
			// 03.31 '09	
			int_bcd( date, pDrec->ddate%10000 , 2 );
		
			if( Vdate.db_vd_offset( date, &offset, m_pDESzm ) == -1 ) 
			{
				ICSMessageBox( "月のオフセットを取得できません！ ");
				return -1;
			}
			pDrec->mofs = offset;			
			/////////////////////////////////////////////////////////////////
		 }
#endif

		eFlag = 5;		// 一括税抜き仕訳

#ifdef _DPICK_CLOSE
		if (!(m_swFunc & 0x1))  {		// D0: 一括税抜き仕訳の抽出ｓｗ

		}
#endif
		eFlag = 6;		// 枝番未登録

		eFlag = 0;
		
		result = 0;
		break;
	}

	if (pEflg != NULL)  *pEflg = eFlag;

	return result;
}



int CSWKCpySrc::PickData( int bseq )
{
	char	buff[128];

	CString	OcrPath, sMsg;

	int	n;

	long sSeq = 0;		// [抽出元]仕訳ＳＥＱ-Ｎｏ
	long nDnum = 0;		// [抽出元]仕訳数
	long dseq = 0;		// [抽出先]仕訳ＳＥＱ-Ｎｏ	
	long imgsq = 0;		// DB [抽出先]イメージＳＥＱ-Ｎｏ
	int	Eflag = 0;		// エラーフラグ

	CString filter, buf;

#ifdef _MDFIMG_	// 05.14 /10
	int imagetable_size = 0;
#endif

	// DB int	nDnum = (int)m_pSRCzm->data_info->reccount;	// pSvl1->DLIN_C;
	// m_pSRCzm->dbdata->Requery( -2, 0, "", 0, 0 );

	// コピー仕訳をクエリー
	SWKCPY_SRC*	psrc;
	psrc = m_pScpySrc;
	int fst = 0;

	BOOL bBlkIns = FALSE;

#ifdef CLOSE // BulkInsert では、追加仕訳の番号管理が難しい。
	// 1000 仕訳より多ければ、BULK INSERT
	if( psrc->sc_cnt > 1000 ) {
		bBlkIns = TRUE;
	}
#endif

	for( n = 0; n < psrc->sc_cnt; n++ ) {
		if( !fst )
		{
			filter.Format( "seq = %d", psrc->sc_seq[n] );
			fst = 1;
		}
		else
		{
			filter += " OR ";
			buf.Format( "seq = %d", psrc->sc_seq[n] );
			filter += buf;
		}
	}

	m_pSRCzm->dbdata->Requery( -2, 0, filter, 0, 0 );
	nDnum = m_pSRCzm->dbdata->GetRecordCount();

	// 最終仕訳の番号
	CStRecordSet st_rec( m_pDESzm->m_database );
	st_rec.m_sql = _T( "select top(1) seq as st from datarec order by seq DESC" );

	TRY {
		if( ! st_rec.Open() ) {
			m_Ermsg = _T( "最終仕訳の番号が調べられません！" );
			return -1;
		}

		while( ! st_rec.IsEOF() ) {
			st_rec.MoveNext();
		}

		if( st_rec.GetRecordCount() > 0 ) {
			dseq	= st_rec.m_st;
		}
		else {
			dseq	= 0;
		}

		st_rec.Close();

	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( m_Ermsg.GetBuffer(_MAX_PATH), _MAX_PATH );
		m_Ermsg.ReleaseBuffer();
		return -1;	
	}
	CATCH( CDBException, de ) {
		m_Ermsg = de->m_strError;
		return -1;
	}
	END_CATCH

//	m_pDESzm->dbdata->MoveLast();
//	dseq = m_pDESzm->dbdata->seq;

	if( dseq < 0 ) dseq = 0;

	if ( m_swImg == 1 )  
	{ 
		// m_pDESzm->image->Requery("");
#ifdef _MDFIMG_	// 05.14 /10
		imgsq = m_pDESzm->image->GetRecordCount();
#else
		int iDnum = m_pDESzm->image->GetRecordCount();
		
		if( m_pDESzm->image->SetAbsolutePosition( iDnum ) == 0 ) 
		{
			imgsq = m_pDESzm->image->iseq;
		}
#endif
		if( imgsq < 0 )
		{
			imgsq = 0;
		}
	}

	int err_ret = -1;
	int	nPcnt = 0;		// 抽出仕訳数カウンタ
	int	nOcnt = 0;		// 範囲外仕訳数カウンタ
	int	nEcnt[10];
	int img_add = 0;

	if( m_pMsgDlg == NULL ) {
		m_pMsgDlg = new CSCopyMsgDlg();
		m_pMsgDlg->m_strMsg = _T( "仕訳をコピーしています・・・" );

		m_pMsgDlg->m_btnSW[0] = SW_HIDE;
		m_pMsgDlg->Create(m_pParent);
	}

	m_swAbort = FALSE;
	memset(&nEcnt[0], 0, sizeof(int)*10);

	m_pMsgDlg->m_PrgRcnt.SetRange(0, nDnum);

//	int	eCpos = (m_pMFtbl != NULL) ? 3 : 2;		// エラーカウント位置
	int	eCpos = 2;		// エラーカウント位置

	int rno;

#ifdef DBPICK_CLOSE
	m_pDESzm->KamokuMeisyoRead( 0 );
#endif

	// 07.30 '08 高速化
	CDBDATA DataBuf2;

	CBlkIns Blk;	// クラスの作成

	if( bBlkIns ) {
		char	dPath[256];
		CString	m_strWpth, str;

		if (COM_GetWorkDirectory(dPath) == FALSE )
		{
		}	
		m_strWpth = dPath;		// ワークディレクトリ・パス名
		
		Blk.Iniz( m_pDESzm, m_strWpth  );
	}
	else {
		// ロック
//		m_pDESzm->dbdata->BeginTransLock();
	}

	if( nDnum > psrc->sc_cnt )
		nDnum = psrc->sc_cnt;

	for ( rno = 0; rno < /*(WORD)*/nDnum; ++rno )  
	{
		Eflag = 0;		// エラーフラグ ('05.09.07)

		m_pMsgDlg->m_PrgRcnt.SetPos( rno );
		if( m_pMsgDlg->CheckCancelButton() )
			m_swAbort = TRUE;

		if (m_swAbort)  break;		// キャンセルボタンによる中止

		// [抽出元] データＲｅａｄ		

#ifdef DBCLOSE
		if( m_pSRCzm->dbdata->SetAbsolutePosition( rno+1 ) == ERR )	break;
#endif

		int getseq = psrc->sc_seq[rno];
		int ok_sgn = 0;

		m_pSRCzm->dbdata->MoveFirst();
		do {
			if( getseq == m_pSRCzm->dbdata->seq ) {
				ok_sgn = 1;
				break;
			}
		} while( m_pSRCzm->dbdata->MoveNext() == 0 );

		if( ! ok_sgn )	break;

		// 07.30 '08 高速化	
		SetDbtoMd2( &DataBuf2 );
			
		sSeq = DataBuf2.seq;
		// sSeq = m_pSRCzm->dbdata->seq; 
		// [抽出元] データＣｈｅｃｋ
		if( CheckData2( &DataBuf2, &Eflag ) != 0 )  
		{
			if( Eflag == 6 )  
			{
				// 枝番登録数オーバー ('05.07.22)
				// break;
				err_ret = -2;
				goto ERR_TAG;
			//	return -2;
			}
			// 指定範囲外等
			if( Eflag >= eCpos )  
			{					
				if( nOcnt == 0 )  
				{
#ifdef DBPICK_PRGRCNT
					// ダイアログ拡張
					VextendWindow( pCont3, 4 );		
					pCont3->EnableWindow( TRUE );
					pCont3->ShowWindow( SW_SHOW );
					pItem3->EnableWindow( TRUE );
					pItem3->ShowWindow( SW_SHOW );
#endif
				}
				++nOcnt;				
				wsprintf( buff, _T("%7d 件 "), nOcnt );

#ifdef DBPICK_PRGRCNT
				pCont3->SetWindowText( buff );
				_GetMessage();
#endif
			}
			if (Eflag > 0)  ++nEcnt[ Eflag-1 ];
			continue;
		}

		// 取消／出力済みサイン --> ＯＦＦ
		// DB DataBuf.darec._DSGN &= 0xf8;		
		// 07.30 '08 高速化			
		// DataBuf.m_dsign[0] &= 0xf8;	
		DataBuf2.dsign[0] &= 0xf8;	
		// 確定サインOFF
//		DataBuf2.dsign[7] &= ~0x30;
		DataBuf2.dsign[7] &= ~0x70;		// 02.28 /12

		// 原票サインＯＦＦ
		DataBuf2.dsign[6] &= ~0x01;		// 02.28 /12

		// 摘要文字数 チェック＆調整
#ifdef MD		
		m_pDESzm->TekiyoAdjust( &DataBuf, m_pSRCzm->zvol->tk_ln );
#endif
		// イメージデータ Ｗｒｉｔｅ ＆ 更新
		if (m_swImg)  {
			if (m_swImg == 1)  {
				// 07.30 '08
				// if (DataBuf.m_dsign[0] & 0x80)  {
				if (DataBuf2.dsign[0] & 0x80)  {

#ifdef MD
// DBIMG 07.24
					DAIMG*	pDimg = (DAIMG*)DataBuf._TKS;
					if ((st = m_pDESzm->TekiyoImageAppend(1, DataBuf._TKS, OcrPath)) < 0)  {
						///**/sMsg.Format(_T("rno = %d , st = %d"), rno, st);
						if (st == -2)  {
							sMsg = _T("転送先と転送元のイメージ線密度が異なるため、");
							sMsg += _T("\n転送することはできません！");
						}
						break;
					}
#endif
					// DB
					// m_pSRCzm->image->Requery( "" );
					// if( m_pSRCzm->image->SetAbsolutePosition( DataBuf.m_imgsq ) == 0 ) 
					// 07.30 '08 高速化対応
					if( m_pSRCzm->image->SetAbsolutePosition( DataBuf2.imgsq ) == 0 ) 
				
					{
#ifdef _MDFIMG_	// 05.14 /10
						if( !imagetable_size ) {
							//イメージのサイズが１０２４よりも大きいとき
							//テーブルの拡張が必要です。

							if( m_pSRCzm->image->imgdt.GetCount() > 1024 ) {
								// イメージサイズを調べる
							//	CImageSize imgsz( m_pDESzm->m_database );
								CStRecordSet	st_rec( m_pDESzm->m_database );
								st_rec.m_sql = _T( "select max_length as st from sys.columns where object_id = object_id(N'image') and name = N'imgdt'" );

								TRY {
							//		if( ! imgsz.Open() ) {
							//			return -2;
							//		}
							//		imgsz.Close();

									if( ! st_rec.Open() ) {
										err_ret = -3;
										goto ERR_TAG;
									}
									st_rec.Close();

							//		if( imgsz.m_maxlength <= 1024 ) {
									if( st_rec.m_st <= 1024 ) {
										m_pDESzm->m_database->ExecuteSQL( "ALTER TABLE image ALTER COLUMN imgdt varbinary(2048)" );
										m_pDESzm->image->Requery("iseq = 1");
									}
								}
								CATCH( CMemoryException, me ) {
									err_ret = -3;
									goto ERR_TAG;
								}
								CATCH( CDBException, de ) {
									err_ret = -3;
									goto ERR_TAG;
								}
								END_CATCH

								imagetable_size = 2048;
							}
						}
#endif
						m_pDESzm->image->AddNew();
						m_pDESzm->image->iseq = ++imgsq;
						m_pDESzm->image->imsgn.Copy( m_pSRCzm->image->imsgn );
						m_pDESzm->image->imgdt.Copy( m_pSRCzm->image->imgdt );
						m_pDESzm->image->dcdimg.Copy( m_pSRCzm->image->dcdimg );
						m_pDESzm->image->Update();
						// DataBuf.m_imgsq = imgsq;
						DataBuf2.imgsq = imgsq;

						img_add++;
					}
				}
			}
			else if (m_swImg == 2)  {
#ifdef DBCLOSE
				m_pDESzm->TekiyoSpaceClear(&DataBuf);
#endif
				// DB
				// 07.30 '08 高速化対応			
				// DataBuf.m_dsign[0] &= ~0x80;
				// DataBuf.m_imgsq = 0;
				DataBuf2.dsign[0] &= ~0x80;
				DataBuf2.imgsq = 0;
			}
		}

		// [抽出先] データ Ｗｒｉｔｅ ＆ 更新

		// 07.30 '08 高速化対応
		// DataBuf.m_seq = ++dseq; 
		DataBuf2.seq = ++dseq; 
		
		// 部門あり	
		if( (m_pSRCzm->zvol->bm_isw) && (m_pDESzm->zvol->bm_isw) )
		{	
#ifdef KEEP_CLOSE // to do 部門範囲
			if( DataBuf2.dbmn != -1 )
				BmonNamchk2( 0, &DataBuf2 );
			if( DataBuf2.cbmn != -1 )
				BmonNamchk2( 1, &DataBuf2 );
#endif
			BmonNamHanichk( &DataBuf2, bBlkIns );
		}

// 07.30 '08 高速化対応 CLOSE
#ifdef DBCLOSE
		SetMdtoCb( &DataBuf );
#endif

		if( bBlkIns ) {
			// 07.30 '08 高速化対応
			if( Blk.PutBlkData( &DataBuf2 ) == ERR )	break;
		}
		else {
			SetMdtoDESdbdata( &DataBuf2 );
			CDBINPDataRec	drec;
			m_pDESzm->GetCDBData( &drec );

			p_DBDinpView->CheckCopySwkData( &drec );

			if( m_pDESzm->DB_DataAppend( &drec, bseq ) == ERR)
				break;

			p_DBDinpView->set_insrec( &drec, bseq );

			DBDinpAppendParamUpdate( &drec, bseq );
			syog_genk( &drec, _ADD );
		}

		++nPcnt;

#ifdef DBPICK_PRGRCNT
		// wsprintf(buff, _T("%6d 件 "), nPcnt);
		wsprintf( buff, _T("%7d 件 "), nPcnt );		
		pCont2->SetWindowText( buff );
		// _GetMessage();
#endif
	}

	if( bBlkIns ) {
		// 07.30 '08
		// BLKINSERT 用ファイルを使いデータの一括追加を行います。
		Blk.WrtBlkData();
	}
	else {
//		m_pDESzm->dbdata->CommitTransUnLock();
	}

	m_pMsgDlg->m_PrgRcnt.SetPos( rno );

	if( bBlkIns ) {
		if( rno )
		{
			CString msg; 
			// m_pDESzm->BmnameOpen();
			m_pDESzm->BumonzanOpen();
			m_pDESzm->BumonEdabanOpen("ezbmn = -1");
			m_pDESzm->SyukeiDataOpen( 0 );
			/// 07.25 '08
			if( rno >= 1000 ) {
				m_pMsgDlg->SetMessageText("\n 残高を更新しています．．．．．");
			}

			int ret = db_recalc_ex( m_pDESzm, m_pParent, msg );

			m_pDESzm->SyukeiDataClose();
			// m_pDESzm->BmnameClose();
			m_pDESzm->BumonzanClose();
			m_pDESzm->BumonEdabanClose();

			m_pDESzm->SwkCopyAfterJob();
		}
	}

	if( img_add ) {
		m_pDESzm->image->Requery("");
	}
#ifdef DBPICK_CLOSE
	if (m_pParent != NULL)
		m_pParent->PostMessage(WM_USER_1, MAKEWPARAM(WM_USERCHANGED, 0), 0);
#endif

	if (nOcnt > 0)  {
		CString	msg;
		msg = _T( "コピーできない仕訳がありました。\r\n" );

		for (int i = eCpos - 1; i < 5; ++i)  {
			if (nEcnt[i] != 0)  {
				msg += "\r\n";

				if (i == 0)  sprintf_s( buff, sizeof buff, _T(" 指定範囲外仕訳：%7d 件"), nEcnt[i]);
				if (i == 1)  sprintf_s( buff, sizeof buff, _T(" 取消仕訳　　　：%7d 件"), nEcnt[i]);
				if (i == 2)  sprintf_s( buff, sizeof buff, _T(" 未設定科目仕訳：%7d 件"), nEcnt[i]);
				if (i == 3)  sprintf_s( buff, sizeof buff, _T(" 日付範囲外仕訳：%7d 件"), nEcnt[i]);
				if (i == 4)  sprintf_s( buff, sizeof buff, _T(" 一括税抜き仕訳：%7d 件"), nEcnt[i]);

				msg += buff;
			}
		}
		m_pMsgDlg->SetMessageText("");
		// ユーザ確認を待つ
		ICSMessageBox( msg, 0,0,0, m_pMsgDlg );
	}

	if( m_pMsgDlg != NULL ) {
		m_pMsgDlg->DestroyWindow();
		delete m_pMsgDlg;
		m_pMsgDlg = NULL;
	}

	return nPcnt;

ERR_TAG:

	if( m_pMsgDlg != NULL ) {
		m_pMsgDlg->DestroyWindow();
		delete m_pMsgDlg;
		m_pMsgDlg = NULL;
	}

	return err_ret;
}


//----------------------------------------------------------------
//	部門範囲チェック 
//	[BlkInsert時は部門名称を追加]
//
//----------------------------------------------------------------
int CSWKCpySrc::BmonNamHanichk( CDBDATA* rec, BOOL bBulk )
{
	if( rec->dbmn != -1 ) {
		if( ! BmChk.BmnHaniCheck( rec->dbmn, rec->dbt ) ) {
			rec->dbmn = -1;
		}
	}

	if( bBulk ) {
		if( rec->dbmn != -1 ) {
			m_pDESzm->BumonZanTorok(rec->dbmn, rec->dbt );
		}
	}
	if( rec->cbmn != -1 ) {
		if( ! BmChk.BmnHaniCheck( rec->cbmn, rec->cre ) ) {
			rec->cbmn = -1;	
		}
	}

	if( bBulk ) {
		if( rec->cbmn != -1 ) {
			m_pDESzm->BumonZanTorok(rec->cbmn, rec->cre );
		}
	}

	return 0;
}


//---------------------------------------------------
//	コピー元・コピー先の改正消費税バージョンチェック
//
//---------------------------------------------------
int CSWKCpySrc::SyzVersionChk()
{
	WORD	desSign, srcSign;
	desSign = srcSign = 0;

	CDatabase*	pdb;
	WORD*		pSign;

	CString mastnam[2];

	for( int n = 0; n < 2; n++ ) {
		pdb = (n == 0) ? m_pDESzm->m_database : m_pSRCzm->m_database;
		pSign = (n == 0) ? &desSign : &srcSign;

		CZmGen8		ZmGen;
		// インボイス対応
		//int vver = ZmGen.CheckValiableVersion( pdb, ID_H26SYZ_VUP_CHK|ID_H31SYZ_VUP_CHK, mastnam[n] );
		int vver = ZmGen.CheckValiableVersion( pdb, ( ID_H26SYZ_VUP_CHK | ID_H31SYZ_VUP_CHK | ID_INV_SYZ_VUP_CHK ), mastnam[n] );
		*pSign = vver;
	}
	int ret = 0;

	if( desSign != srcSign ) {
		m_Ermsg.Format( "[コピー元]：%s \n[コピー先]：%s \n\nマスターバージョンが異なる為、仕訳のコピーはできません。", mastnam[1], mastnam[0]);
		m_Ermsg += _T("\n\n確認してください。");
		ret = -1;
	}
	return ret;
}

//---------------------------------------------------
//	コピー元・コピー先のマスターの再チェック
//
//---------------------------------------------------
int CSWKCpySrc::MastChk()
{
	CVolDateDB	cDateDB;
	
	m_Erflg = 0;
	m_Ermsg.Empty();	
	int i, nEsg, symd, eymd;

	for ( nEsg = -1; ;)  {
		// 業種チェック
		if( CopyGyosyuCheck( m_Ermsg ) != 0 ) {
			nEsg = -2;
			break;
		}
		//改正消費税のチェック
		if( SyzVersionChk() != 0 ) {
			nEsg = -2;
			break;
		}

		if ( m_pSRCzm->zvol->m_type & 1)  {		// 合併マスター
			m_Ermsg = _T("\n 残高合併マスターは、処理できません！");
			break;
		}

		long dcnt;
		int j;			
		
		if( m_pSRCzm->datalin ) 
		{
			m_pSRCzm->DatalinFree();
		}
		m_pSRCzm->DatalinRead();

		for( j = dcnt = 0; j < 16; j++ ) 
		{
			m_pSRCzm->minfo->SetAbsolutePosition( j+1 );
			// dcnt += ZmSub.datalin->dcnt[ mofs ]/*ZmSub.minfo->lin_mn*/ ;
			dcnt += m_pSRCzm->datalin->dcnt[j];
		}
		if ( /*pSvol1->DLIN_C*/dcnt == 0 )  
		{
			m_Ermsg = _T("\n 選択された会社には、仕訳が入力されていません！");
			break;
		}

#ifdef DBPICK_CLOSE
		/*--- 消費税 チェック ---*/
		if ((m_pSRCzm->zvol->s_mst & 1) && (m_pDsyo->IsSyohizeiMaster() == TRUE) /* DB &&
			(m_pDESzm->IsIkkatsuMaster() == FALSE)*/ )				
		{
			int	flag = 0;

			if (((m_pSRCzm->zvol->s_sgn1 & 0x80) != (m_pDESzm->zvol->s_sgn1 & 0x80)) ||
										((m_pSRCzm->zvol->s_sgn1 & 0xc0) == 0xc0))  {
				flag = 1;
			}
			else if (m_pDsyo->IsNukiMaster() == TRUE) 
			{
				if ((m_pSRCzm->zvol->s_sgn1 & 0x0f) != (m_pDESzm->zvol->s_sgn1 & 0x0f))  flag = 2;
			}
			if (flag)  
			{
				m_Ermsg = _T(" [抽出先]の会社と消費税処理方法が異なります！");

				CString	str;
				  				
				// const VOL1*	pVol1 = (i == 0) ? pDvol1 : pSvol1;
				
				str.Format("\n      [抽出%s]: %s", "先",
							(!(m_pDESzm->zvol->s_sgn1 & 0x80)) ? "税込み" :

							(!(m_pDESzm->zvol->s_sgn1 & 0x40)) ? "税抜き" : "期末一括税抜き");

				m_Ermsg += str;

				if (flag == 2)  
				{
					str.Format("（資産:%s／仕入:%s／経費:%s）",
										(m_pDESzm->zvol->s_sgn1 & 0x01) ? "抜" : "込",
										(m_pDESzm->zvol->s_sgn1 & 0x04) ? "抜" : "込",
										(m_pDESzm->zvol->s_sgn1 & 0x08) ? "抜" : "込");
					m_Ermsg += str;
				}


				str.Format("\n      [抽出%s]: %s", "元",
							(!(m_pSRCzm->zvol->s_sgn1 & 0x80)) ? "税込み" :

							(!(m_pSRCzm->zvol->s_sgn1 & 0x40)) ? "税抜き" : "期末一括税抜き");

				m_Ermsg += str;

				if (flag == 2)  
				{
					str.Format("（資産:%s／仕入:%s／経費:%s）",
										(m_pSRCzm->zvol->s_sgn1 & 0x01) ? "抜" : "込",
										(m_pSRCzm->zvol->s_sgn1 & 0x04) ? "抜" : "込",
										(m_pSRCzm->zvol->s_sgn1 & 0x08) ? "抜" : "込");
					m_Ermsg += str;
				}				
				break;
			}
		}
#endif

		// 03.02 '07
		// 枝番あり
		if( BRmst )
		{

			if(	m_pSRCzm->zvol->edcol > m_pDESzm->zvol->edcol )
			{
				m_Ermsg = _T(" [コピー元]の会社と枝番入力桁数が異なります！");
				
				CString	str;

				str.Format("\n\n      [コピー先]: 枝番入力 %6d 桁", m_pDESzm->zvol->edcol );
				m_Ermsg += str;
				str.Format(  "\n      [コピー元]: 枝番入力 %6d 桁", m_pSRCzm->zvol->edcol );
				m_Ermsg += str;		
				str.Format(  "\n\n  %s", "枝番入力桁設定の変更が必要です。" );
				m_Ermsg += str;

				nEsg = -2;
				break;
			}		
		}
		else
		{

		}
		// 部門あり
		if( BMON_MST )
		{
			if(	m_pSRCzm->zvol->bmcol > m_pDESzm->zvol->bmcol )
			{
				m_Ermsg = _T(" [コピー元]の会社と部門入力桁数が異なります！");
				
				CString	str;

				str.Format("\n\n      [コピー先]: 部門入力 %6d 桁", m_pDESzm->zvol->bmcol );
				m_Ermsg += str;
				str.Format(  "\n      [コピー元]: 部門入力 %6d 桁", m_pSRCzm->zvol->bmcol );
				m_Ermsg += str;		
				str.Format(  "\n\n  %s", "部門入力桁設定の変更が必要です。" );
				m_Ermsg += str;

				nEsg = -2;
				break;
			}
		}

		if( M_KOJI )
		{
			if(	m_pSRCzm->zvol->kjcol > m_pDESzm->zvol->kjcol )
			{
				m_Ermsg = _T(" [コピー元]の会社と工事番号入力桁数が異なります！");
				
				CString	str;

				str.Format("\n\n      [コピー先]: 工事番号入力 %6d 桁", m_pDESzm->zvol->kjcol );
				m_Ermsg += str;
				str.Format(  "\n      [コピー元]: 工事番号入力 %6d 桁", m_pSRCzm->zvol->kjcol );
				m_Ermsg += str;		
				str.Format(  "\n\n  %s", "工事番号入力桁設定の変更が必要です。" );
				m_Ermsg += str;

				nEsg = -2;
				break;
			}
		}

		//証憑番号
		if( bDOCEVI_Master ) {
			if(	m_pSRCzm->zvol->snocol > m_pDESzm->zvol->snocol )
			{
				m_Ermsg = _T(" [コピー元]の会社と証憑番号桁数が異なります！");
				
				CString	str;

				str.Format("\n\n      [コピー先]: 証憑番号入力 %6d 桁", m_pDESzm->zvol->snocol );
				m_Ermsg += str;
				str.Format(  "\n      [コピー元]: 証憑番号入力 %6d 桁", m_pSRCzm->zvol->snocol );
				m_Ermsg += str;		
				str.Format(  "\n\n  %s", "証憑番号入力桁設定の変更が必要です。" );
				m_Ermsg += str;

				nEsg = -2;
				break;
			}
		}

//#ifdef DBMD
		/*--- イメージ チェック ---*/
		if ((m_pSRCzm->zvol->ocr_idev&0x0f) == 3)  {
			// if (m_pDESzm->pVol1->OCR_IDEV == 3)  {
			if( (m_pDESzm->zvol->ocr_idev&0x0f) == 3 ) 
			{
				m_pDESzm->ImageClose();	
				/*-- イメージ Ｏｐｅｎ --*/
				// int	result = m_pDESzm->TekiyoImageOpen();
				int	result = m_pDESzm->ImageOpen();

				if( result < 0 )
				{
					m_Ermsg = _T("\n [コピー先]の会社の「摘要イメージファイル」を");
					m_Ermsg += _T("\n オープンできないため、処理できません！");

					if( result == -2 )
					{
						m_Ermsg = _T("\n [コピー先]の会社には、未転記データがあるため、");
						m_Ermsg += _T("\n 処理できません！");
						m_Ermsg += _T("\n 転記を行ってから、やり直してください！");
					}
					nEsg = -3;
					break;
				}
# ifdef _RCVRIMG_	// 05.19 /10
				ImgRcvry( m_pDESzm->image );
# endif
			}
		}
//#endif

		/*--- 決算期間 チェック ---*/
		//if (memcmp(pSvol1->EE_YMD, pDvol1->EE_YMD, 3) != 0)  {
		if( m_pDESzm->zvol->cs_ymd != m_pSRCzm->zvol->cs_ymd ) {

			m_Ermsg = _T(" [コピー元]の会社と決算期間が異なります！");
			int gengo;

			for ( i = 0; i < 2; ++i )  {

				if( i == 0 )
				{
					cDateDB.db_datecnvGen( 0, m_pDESzm->zvol->ss_ymd, &gengo, &symd, 0, 0 );
					cDateDB.db_datecnvGen( 0, m_pDESzm->zvol->ee_ymd, &gengo, &eymd, 0, 0 );
				}
				else
				{
					cDateDB.db_datecnvGen( 0, m_pSRCzm->zvol->ss_ymd, &gengo, &symd, 0, 0 );
					cDateDB.db_datecnvGen( 0, m_pSRCzm->zvol->ee_ymd, &gengo, &eymd, 0, 0 );
				}

				CString	str;
				str.Format("\n      [コピー%s]: %2d年%2d月%2d日 ～ %2d年%2d月%2d日",
					(i == 0) ? "先" : "元",
					symd / 10000, (symd % 10000) / 100, symd % 100,
					eymd / 10000, (eymd % 10000) / 100, eymd % 100 );
						
				m_Ermsg += str;

			}

			nEsg = 1;		// ['01.06.27]
			//break;		// 期間が異なる場合、中止 ['99.4.13]
		}

		//  [コピー元]がイメージありで、[コピー先]イメージなし
		if( m_swImg == 2 ) {
			m_Ermsg = _T(" [コピー元]の会社とＯＣＲ処理が異なります！");
			
			CString	str;

			str.Format("\n\n      [コピー先]: ＯＣＲ処理 『行わない』" );
			m_Ermsg += str;
			str.Format(  "\n      [コピー元]: ＯＣＲ処理 『行う』" );
			m_Ermsg += str;
			str = "\n\n イメージ摘要はコピーされません。";
			m_Ermsg += str;

			nEsg = 1;
		}

		if (nEsg < 0)  nEsg = 0;
		break;
	}

//////////////////////////////////////////////////////////////////////
#ifdef MD
	// 枝番なし
	if (nEsg < 0 )  
	{
		if( m_pSRCzm->zvol->br_sw )
		{
			m_Ermsg = _T(" [抽出元]の会社に枝番設定がされています。");
			CString	str;
			str.Format("\n\n      [抽出先]: 枝番設定なし", m_pDESzm->zvol->edcol );
			m_Ermsg += str;
			str.Format(  "\n      [抽出元]: 枝番設定あり", m_pSRCzm->zvol->edcol );
			m_Ermsg += str;		
			str.Format(  "\n\n  仕訳に付いている枝番は抽出されません　" );
			m_Ermsg += str;
			nEsg = 1;		// ['01.06.27]
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////

#ifdef DBPICK_CLOSE
	if (nEsg == 0)  EndDialog(IDOK);
	else  
	{
		if (nEsg > -3)  {
			CWnd*	pOk = GetDlgItem(IDOK);
			CWnd*	pCancel = GetDlgItem(IDCANCEL);
			if (nEsg > 0)  {
				pOk->SetWindowText(_T("はい"));
				pCancel->SetWindowText(_T("いいえ"));
				m_Ermsg += _T("\n\n 処理を続行しますか？");
				PostMessage(WM_USER_1, MAKEWPARAM(WM_SETFOCUS, IDCANCEL));
			}
			else  {
				pCancel->EnableWindow(FALSE);
				pCancel->ShowWindow(SW_HIDE);
				MoveCenterWindow(pOk);
				if (nEsg > -2)  m_Ermsg += _T("\n\n 会社を選択し直してください！");
			}
		}
		GetDlgItem(IDC_STATIC_ERRMSG)->SetWindowText(m_Ermsg);
	}
#endif

//	nEsg = -3;
//MyTrace( "nEsg = %d, m_Ermsg = %s", nEsg, m_Ermsg );

	int st, ret = 0;

	if (nEsg != 0) {
		ret = -1;
		if (nEsg > -3)  {

			if (nEsg > 0)  {
				m_Ermsg += _T("\n\n 処理を続行しますか？");
			}
			else  {
				if (nEsg > -2)  m_Ermsg += _T("\n\n 仕訳のコピーをやり直してください！");
			}

		//	CSCopyMsgDlg	cdlg( m_Ermsg, m_pParent );

			if( nEsg > 0 ) {
				st = ICSMessageBox( m_Ermsg, MB_YESNO,0,0, m_pParent );
			//	cdlg.m_strOK	 = _T("はい");
			//	cdlg.m_strCANCEL = _T("いいえ");
			}
			else {
				st = ICSMessageBox( m_Ermsg, MB_OK,0,0, m_pParent );
			//	cdlg.m_strOK = _T("ＯＫ");
			//	cdlg.m_btnSW[1] = SW_HIDE;
				ret = -1;
			}
		//	st = cdlg.DoModal();

			if( nEsg > 0 ) {
			//	if( st == IDOK )
				if( st == IDYES )
					ret = 0;
			}
		}
		else {
			ICSMessageBox( m_Ermsg, 0,0,0, m_pParent );
		}
	}

	m_Erflg = nEsg;

	return ret;
}



void master_gyoname( BYTE apno, CString& gystr )
{

	if ( apno == 0x00 )
	{
		gystr = _T(" 法人マスター  ") ;
	}
	else if ( apno == 0x01 )
	{
		gystr = _T(" 運送業マスター  ") ;
	}
	else if ( apno == 0x30 )
	{
		gystr = _T(" 病院マスター  ") ;
	}
	else if ( apno == 0x20 )
	{
		gystr = _T(" 建設マスター  ") ;
	}
	else if ( apno == 0x10 ){
		gystr = _T(" 個人事業未設定マスター  ") ;
	}
	else if ( apno == 0x50 )
	{
		gystr = _T(" 公益法人マスター  ") ;
	}
	else if ( apno == 0x51 )
	{
		gystr = _T(" 公益法人Ⅲマスター  ") ;
	}
	else if ( apno == 0x52 )
	{
		gystr = _T(" 社会福祉法人マスター  ") ;
	}
	else if ( apno == 0x59 )
	{
		gystr = _T(" 宗教法人マスター  ") ;
	}
	else if ( apno == 0x58 )
	{
		gystr = _T(" 学校法人マスター  ") ;
	}
	else {
		gystr.Format( _T(" 番号[%02x]マスター  "), apno );
	}	
}
//
//	業種チェック
//
int CSWKCpySrc::CopyGyosyuCheck( CString& msgstr )
{
	int dktb, dcol, dsg1, dsg2, dsg3, dsg4, dsg5, dsg6, dsg7;
	int sktb, scol, ssg1, ssg2, ssg3, ssg4, ssg5, ssg6, ssg7;

	int err_ret, set_cnt;
	CString	str;	
	CString sbuf;
	CString ktstr1, ktstr2;
	CString* ktstr;

	err_ret = 0;

	// 個人業種対応 業種チェック
	if ( (m_pDESzm->zvol->ind_table != m_pSRCzm->zvol->ind_table) || 
		 (m_pDESzm->zvol->ind_type  != m_pSRCzm->zvol->ind_type)  || 
		 (m_pDESzm->zvol->ind_sgn2  != m_pSRCzm->zvol->ind_sgn2) || 
		 (m_pDESzm->zvol->apno != m_pSRCzm->zvol->apno) )
	{
		// 0x01	青色・一般
		// 0x11	収支・一般
		// 0x02	青色・不動産
		// 0x12 収支・不動産
		// 0x04	青色・農業
		// 0x14	収支・農業

		// 簡易科目体系 青色(一般／不動産) 貸借共通 一般
		for ( int i = 0; i < 2; i++ ) 
		{
			set_cnt = 0;

			ktstr = (i == 0) ? &ktstr1 : &ktstr2;

			int itable = (i == 0) ? m_pDESzm->zvol->ind_table : m_pSRCzm->zvol->ind_table;
			int itype =  (i == 0) ? m_pDESzm->zvol->ind_type  : m_pSRCzm->zvol->ind_type;
			int isgn2 =  (i == 0) ? m_pDESzm->zvol->ind_sgn2  : m_pSRCzm->zvol->ind_sgn2;

			*ktstr = (itable == 0) ? _T(" 標準科目体系 ") : _T(" 簡易科目体系 ");

			if( !i )
				dktb = itable;
			else
				sktb = itable;

			*ktstr += (itype&0x10) ? _T("業種: 収支 (") : _T("業種: 青色 (");

			if( !i )
				dcol = (itype&0x10);		
			else
				scol = (itype&0x10);

			if( !i ) { dsg1 = dsg2 = dsg3 = dsg4 = dsg5 = dsg6 = dsg7 = 0; }
			else	 { ssg1 = ssg2 = ssg3 = ssg4 = ssg5 = ssg6 = ssg7 = 0; }

			if ( itype&0x1 ) 
			{
				*ktstr += _T("一般");

				set_cnt ++;

				if( !i )  dsg1 = 1;
				else	  ssg1 = 1; 

			}
			if ( itype&0x2 ) 
			{
				if ( itype&0x1 )	*ktstr += _T("／");
				*ktstr += _T("不動産");
			
				set_cnt ++;

				if( !i )  dsg2 = 1;
				else	  ssg2 = 1; 

			}
			if ( itype&0x4 ) 
			{
				if ( itype&0x3 )	*ktstr += _T("／");
				*ktstr += _T("農業");

				set_cnt ++;

				if( !i )  dsg3 = 1;
				else	  ssg3 = 1; 

			}

			*ktstr += _T(")");

			// 08.25 '08
			// ケタ揃えスペース
			if( !i )
			{
				if( set_cnt == 1 )
				{
					if( dsg2 )
						*ktstr += _T("　　　　　　");
					else
						*ktstr += _T("　　　　　　　");
				}
				if( set_cnt == 2 )
				{					
					if( dsg2 )
						*ktstr += _T("　　　");
					else
						*ktstr += _T("　　　　");
				}
			}
			else
			{
				if( set_cnt == 1 )
				{
					if( ssg2 )
						*ktstr += _T("　　　　　　");
					else
						*ktstr += _T("　　　　　　　");	
				}
				if( set_cnt == 2 )
				{
					if( ssg2 )
						*ktstr += _T("　　　");
					else
						*ktstr += _T("　　　　");
				}
			}

			if( isgn2 )
			{
				*ktstr += _T("\r\n　　　　　　");

				if( isgn2&0x01 )
				{
					*ktstr += _T(" 貸借: 一般");
					if( !i )  dsg4 = 1;
					else	  ssg4 = 1; 

				}
				if( isgn2&0x02 )
				{
					*ktstr += _T(" 貸借: 不動産");
					if( !i )  dsg5 = 1;
					else	  ssg5 = 1; 

				}
				if( isgn2&0x04 )
				{
					*ktstr += _T(" 貸借: 農業");
					if( !i )  dsg6 = 1;
					else	  ssg6 = 1; 

				}
				if( isgn2&0x10 )
				{
					*ktstr += _T(" 貸借: 業種別");
					if( !i )  dsg7 = 1;
					else	  ssg7 = 1; 
				}
			}

			// 個人以外
			if( (!itable) && (!itype) && (!isgn2) )
			{
				if( !i )
				{
					master_gyoname( m_pDESzm->zvol->apno, *ktstr );
				}	
				else
				{
					master_gyoname( m_pSRCzm->zvol->apno, *ktstr );
				}
			}
		}

		if( dktb != sktb )	err_ret = 1;
		if( dcol != scol )	err_ret = 1;

		if( m_pDESzm->zvol->apno != m_pSRCzm->zvol->apno )
			err_ret = 1;

		if( ssg1 )
		{
			if( !dsg1 )	err_ret = 1;  
		}
		if( ssg2 )
		{
			if( !dsg2 )	err_ret = 1;  
		}
		if( ssg3 )
		{
			if( !dsg3 )	err_ret = 1;  
		}
		////////////////////////////
		// 貸借サイン

		if( ssg4 )
		{
			// 一般 / 業種別
			if( !err_ret && dsg7 )
			{
			}
			else
			{
				if( !dsg4 )	err_ret = 1;
			}
		}
		if( ssg5 )
		{
				// 不動産 / 業種別
			if( !err_ret && dsg7 )
			{
			}
			else
			{
				if( !dsg5 )	err_ret = 1;
			}
		}
		if( ssg6 )
		{
			// 農業 / 業種別
			if( !err_ret && dsg7 )
			{			
			}
			else
			{
				if( !dsg6 )	err_ret = 1;
			}
		}
		if( ssg7 )
		{
			if( !dsg7 )	err_ret = 1;  
		}

		str.Format(_T("コピー先とコピー元の業種区分が違います。\n\n　コピー先：%s\n　コピー元：%s"),
						ktstr1, ktstr2);
		str += _T("\n\n確認してください。");

#ifdef DBCLOSE
ind_sgn2

0x01=貸借共通（ 一般メイン ）		
0x02=貸借共通（ 不動産メイン ）
0x04=貸借共通（ 農業メイン ）

0x10=貸借個別
			// 06.30 '08
			// 標準テーブルの場合は、業種チェックの必要なし
			if( (!m_pDESzm->zvol->ind_table) && (!m_pSRCzm->zvol->ind_table) ) 
			{
				err_ret = 0;
			}
#endif

		if( (!m_pDESzm->zvol->ind_table) &&	(!m_pDESzm->zvol->ind_type) && (!m_pSRCzm->zvol->ind_sgn2 ) )
		{
			if( m_pSRCzm->zvol->ind_sgn2 || m_pSRCzm->zvol->ind_type || m_pSRCzm->zvol->ind_table ) 
				err_ret = 1;
		}
		if( (!m_pSRCzm->zvol->ind_sgn2) && (!m_pSRCzm->zvol->ind_type) && (!m_pSRCzm->zvol->ind_table) )
		{	
			if( (m_pDESzm->zvol->ind_table) || (m_pDESzm->zvol->ind_type) || (m_pDESzm->zvol->ind_sgn2 ) )
				err_ret = 1;
		}
	}
	else {
		//社会福祉改正
		if( m_pDESzm->zvol->apno == m_pSRCzm->zvol->apno ) {
			if(m_pDESzm->zvol->apno == 0x52 ) {
				if( m_pDESzm->zvol->m_ver != m_pSRCzm->zvol->m_ver )
				{
					str = "＊平成24年改正会計基準\r\n　改正前、改正後間での仕訳のコピーはできません。";
					str += _T("\n\n確認してください。");

					err_ret = -1;
				}
			}
		}
	}

	if( err_ret != 0 ) {
		msgstr = str;
	}

	return err_ret;
}


