
#include "stdafx.h"
#include "ShinFhyo522.h"
#include "ShinFhyo522Idx.h"

//-----------------------------------------------------------------------------
// 控除対象仕入税額計算の基礎となる消費税額
//-----------------------------------------------------------------------------
//void CShinFhyo522::f5calq_4()
//{
//}

//-----------------------------------------------------------------------------
// 横計
//-----------------------------------------------------------------------------
// 引数	index	：	インデックス
//-----------------------------------------------------------------------------
void CShinFhyo522::f52calq_sum1( short index )
{
	m_pArith->l_add( pF5218T, pF5218A, pF5218B );
	m_pArith->l_add( pF5218T, pF5218T, pF5218C );
	if( (index==ID5218A) || (index==ID5218B) || (index==ID5218C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5218T, pF5218T );
	}
}

//-----------------------------------------------------------------------------
// 横計
//-----------------------------------------------------------------------------
// 引数	index	：	インデックス
//-----------------------------------------------------------------------------
void CShinFhyo522::f52calq_sum2( short index )
{
	m_pArith->l_add( pF5219T, pF5219A, pF5219B );
	m_pArith->l_add( pF5219T, pF5219T, pF5219C );
	if( (index==ID5219A) || (index==ID5219B) || (index==ID5219C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5219T, pF5219T );
	}
}

//-----------------------------------------------------------------------------
// 横計
//-----------------------------------------------------------------------------
// 引数	index	：	インデックス
//-----------------------------------------------------------------------------
void CShinFhyo522::f52calq_sum3( short index )
{
	m_pArith->l_add( pF5220T, pF5220A, pF5220B );
	m_pArith->l_add( pF5220T, pF5220T, pF5220C );
	if( (index==ID5220A) || (index==ID5220B) || (index==ID5220C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5220T, pF5220T );
	}

	m_pArith->l_add( pF5221T, pF5221A, pF5221B );
	m_pArith->l_add( pF5221T, pF5221T, pF5221C );
	if( (index==ID5221A) || (index==ID5221B) || (index==ID5221C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5221T, pF5221T );
	}

	m_pArith->l_add( pF5222T, pF5222A, pF5222B );
	m_pArith->l_add( pF5222T, pF5222T, pF5222C );
	if( (index==ID5222A) || (index==ID5222B) || (index==ID5222C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5222T, pF5222T );
	}

	m_pArith->l_add( pF5223T, pF5223A, pF5223B );
	m_pArith->l_add( pF5223T, pF5223T, pF5223C );
	if( (index==ID5223A) || (index==ID5223B) || (index==ID5223C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5223T, pF5223T );
	}

	m_pArith->l_add( pF5224T, pF5224A, pF5224B );
	m_pArith->l_add( pF5224T, pF5224T, pF5224C );
	if( (index==ID5224A) || (index==ID5224B) || (index==ID5224C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5224T, pF5224T );
	}

	m_pArith->l_add( pF5225T, pF5225A, pF5225B );
	m_pArith->l_add( pF5225T, pF5225T, pF5225C );
	if( (index==ID5225A) || (index==ID5225B) || (index==ID5225C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5225T, pF5225T );
	}

	m_pArith->l_add( pF5226T, pF5226A, pF5226B );
	m_pArith->l_add( pF5226T, pF5226T, pF5226C );
	if( (index==ID5226A) || (index==ID5226B) || (index==ID5226C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5226T, pF5226T );
	}

	m_pArith->l_add( pF5227T, pF5227A, pF5227B );
	m_pArith->l_add( pF5227T, pF5227T, pF5227C );
	if( (index==ID5227A) || (index==ID5227B) || (index==ID5227C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5227T, pF5227T );
	}

	m_pArith->l_add( pF5228T, pF5228A, pF5228B );
	m_pArith->l_add( pF5228T, pF5228T, pF5228C );
	if( (index==ID5228A) || (index==ID5228B) || (index==ID5228C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5228T, pF5228T );
	}

	m_pArith->l_add( pF5229T, pF5229A, pF5229B );
	m_pArith->l_add( pF5229T, pF5229T, pF5229C );
	if( (index==ID5229A) || (index==ID5229B) || (index==ID5229C) ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5229T, pF5229T );
	}
}

#ifdef _20140610_EXTEND_
//-----------------------------------------------------------------------------
// 最も有利な方式を選択
//-----------------------------------------------------------------------------
void CShinFhyo522::f52calq_sel( short index )
{
	if( (*m_ptypeCnt) < 2 ){
		*m_pSzsw = 0;
		return;
	}
	// 最大の判定
	char	WORK1[MONY_BUF_SIZE];
	int		maxSign = 19;
	char	*pValA=NULL, *pValB=NULL, *pValC=NULL, *pValT=NULL;
	if( m_pArith->l_cmp(pF5220T, pF5219T) > 0 ){
		memmove( WORK1, pF5220T, MONY_BUF_SIZE );
		pValA = pF5220A;
		pValB = pF5220B;
		pValC = pF5220C;
		pValT = pF5220T;
		maxSign = 20;
	}
	else{
		memmove( WORK1, pF5219T, MONY_BUF_SIZE );
		pValA = pF5219A;
		pValB = pF5219B;
		pValC = pF5219C;
		pValT = pF5219T;
		maxSign = 19;
	}
	if( m_pArith->l_cmp(pF5221T, WORK1) > 0 ){
		memmove( WORK1, pF5221T, MONY_BUF_SIZE );
		pValA = pF5221A;
		pValB = pF5221B;
		pValC = pF5221C;
		pValT = pF5221T;
		maxSign = 21;
	}
	if( m_pArith->l_cmp(pF5222T, WORK1) > 0 ){
		memmove( WORK1, pF5222T, MONY_BUF_SIZE );
		pValA = pF5222A;
		pValB = pF5222B;
		pValC = pF5222C;
		pValT = pF5222T;
		maxSign = 22;
	}
	if( m_pArith->l_cmp(pF5223T, WORK1) > 0 ){
		memmove( WORK1, pF5223T, MONY_BUF_SIZE );
		pValA = pF5223A;
		pValB = pF5223B;
		pValC = pF5223C;
		pValT = pF5223T;
		maxSign = 23;
	}
	if( m_pArith->l_cmp(pF5224T, WORK1) > 0 ){
		memmove( WORK1, pF5224T, MONY_BUF_SIZE );
		pValA = pF5224A;
		pValB = pF5224B;
		pValC = pF5224C;
		pValT = pF5224T;
		maxSign = 24;
	}
	if( m_pArith->l_cmp(pF5225T, WORK1) > 0 ){
		memmove( WORK1, pF5225T, MONY_BUF_SIZE );
		pValA = pF5225A;
		pValB = pF5225B;
		pValC = pF5225C;
		pValT = pF5225T;
		maxSign = 25;
	}
	if( m_pArith->l_cmp(pF5226T, WORK1) > 0 ){
		memmove( WORK1, pF5226T, MONY_BUF_SIZE );
		pValA = pF5226A;
		pValB = pF5226B;
		pValC = pF5226C;
		pValT = pF5226T;
		maxSign = 26;
	}
	if( m_pArith->l_cmp(pF5227T, WORK1) > 0 ){
		memmove( WORK1, pF5227T, MONY_BUF_SIZE );
		pValA = pF5227A;
		pValB = pF5227B;
		pValC = pF5227C;
		pValT = pF5227T;
		maxSign = 27;
	}
	if( m_pArith->l_cmp(pF5228T, WORK1) > 0 ){
		memmove( WORK1, pF5228T, MONY_BUF_SIZE );
		pValA = pF5228A;
		pValB = pF5228B;
		pValC = pF5228C;
		pValT = pF5228T;
		maxSign = 28;
	}
	if( m_pArith->l_cmp(pF5229T, WORK1) > 0 ){
		memmove( WORK1, pF5229T, MONY_BUF_SIZE );
		pValA = pF5229A;
		pValB = pF5229B;
		pValC = pF5229C;
		pValT = pF5229T;
		maxSign = 29;
	}

	// 判定
/*- '14.04.26 -*/
//	if( m_pArith->l_cmp(pF5218T, WORK1) > 0 ){
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pF5218A );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5218B );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pF5218C );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5218T );
////		*m_pSzsw = 0;
//		(*m_pSnHeadData)->Sn_TOKUR = 0;
//	}
//	else{
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pValA );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pValB );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pValC );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pValT );
////		*m_pSzsw = maxSign - (19-1);
//		(*m_pSnHeadData)->Sn_TOKUR = 1;
//	}
/*-------------*/
	if( m_pArith->l_cmp(pF5218T, WORK1) < 0 ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pValA );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pValB );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pValC );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pValT );
		(*m_pSnHeadData)->Sn_TOKUR = 1;
	}
	else{
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pF5218A );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5218B );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pF5218C );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5218T );
		(*m_pSnHeadData)->Sn_TOKUR = 0;
	}
/*-------------*/

	// 最も有利な特例計算は？
/*- '14.04.26 -*/
//	if( m_pArith->l_test(WORK1) ){
//		*m_pSzsw = maxSign - (19-1);
//	}
/*-------------*/
	if( (*m_pSnHeadData)->Sn_TOKUR == 1 ){
		if( m_pArith->l_test(WORK1) ){
			*m_pSzsw = maxSign - (19-1);
		}
	}
	else{
		*m_pSzsw = 0;
	}
/*-------------*/
}
#else
//-----------------------------------------------------------------------------
// 最も有利な方式を選択
//-----------------------------------------------------------------------------
void CShinFhyo522::f52calq_sel( short index )
{
	if( (*m_ptypeCnt) < 2 ){
		*m_pSzsw = 0;
		return;
	}

	// 最大の判定
	char	WORK1[MONY_BUF_SIZE];
//	int		maxSign = 19;
	char	*pValA=NULL, *pValB=NULL, *pValC=NULL, *pValT=NULL;

	int		maxSign = *m_pSzsw + (19-1);

	// 最大の判定
	if( ((*m_pdspSkjSw)&0x01) && (((*m_pdspSkjSw)&~0x01)==0) ){	// 原則計算のみ
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pF5218A );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5218B );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pF5218C );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5218T );
	}
	else if( ((*m_pdspSkjSw)&0x01) && ((*m_pdspSkjSw)&~0x01) ){	// 原則計算＋特例計算
		// 特例計算内での判定
		// 付表5-2-1で先に、特例計算適用時の順位付けは行われている前提
		if( (*m_pdspSkjSw)&0x02 ){
			memmove( WORK1, pF5219T, MONY_BUF_SIZE );
			pValA = pF5219A;
			pValB = pF5219B;
			pValC = pF5219C;
			pValT = pF5219T;
		}
		else if( (*m_pdspSkjSw)&0x04 ){
			memmove( WORK1, pF5220T, MONY_BUF_SIZE );
			pValA = pF5220A;
			pValB = pF5220B;
			pValC = pF5220C;
			pValT = pF5220T;
		}
		else if( (*m_pdspSkjSw)&0x08 ){
			memmove( WORK1, pF5221T, MONY_BUF_SIZE );
			pValA = pF5221A;
			pValB = pF5221B;
			pValC = pF5221C;
			pValT = pF5221T;
		}
		else if( (*m_pdspSkjSw)&0x10 ){
			memmove( WORK1, pF5222T, MONY_BUF_SIZE );
			pValA = pF5222A;
			pValB = pF5222B;
			pValC = pF5222C;
			pValT = pF5222T;
		}
		else if( (*m_pdspSkjSw)&0x20 ){
			memmove( WORK1, pF5223T, MONY_BUF_SIZE );
			pValA = pF5223A;
			pValB = pF5223B;
			pValC = pF5223C;
			pValT = pF5223T;
		}
		else if( (*m_pdspSkjSw)&0x40 ){
			memmove( WORK1, pF5224T, MONY_BUF_SIZE );
			pValA = pF5224A;
			pValB = pF5224B;
			pValC = pF5224C;
			pValT = pF5224T;
		}
		else if( (*m_pdspSkjSw)&0x80 ){
			memmove( WORK1, pF5225T, MONY_BUF_SIZE );
			pValA = pF5225A;
			pValB = pF5225B;
			pValC = pF5225C;
			pValT = pF5225T;
		}
		else if( (*m_pdspSkjSw)&0x100 ){
			memmove( WORK1, pF5226T, MONY_BUF_SIZE );
			pValA = pF5226A;
			pValB = pF5226B;
			pValC = pF5226C;
			pValT = pF5226T;
		}
		else if( (*m_pdspSkjSw)&0x200 ){
			memmove( WORK1, pF5227T, MONY_BUF_SIZE );
			pValA = pF5227A;
			pValB = pF5227B;
			pValC = pF5227C;
			pValT = pF5227T;
		}
		else if( (*m_pdspSkjSw)&0x400 ){
			memmove( WORK1, pF5228T, MONY_BUF_SIZE );
			pValA = pF5228A;
			pValB = pF5228B;
			pValC = pF5228C;
			pValT = pF5228T;
		}
		else if( (*m_pdspSkjSw)&0x800 ){
			memmove( WORK1, pF5229T, MONY_BUF_SIZE );
			pValA = pF5229A;
			pValB = pF5229B;
			pValC = pF5229C;
			pValT = pF5229T;
		}

		// 判定
//-- '15.01.27 --
//		if( m_pArith->l_cmp(pF5218T, WORK1) < 0 ){
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pValA );
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pValB );
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pValC );
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pValT );
//			(*m_pSnHeadData)->Sn_TOKUR = 1;
//		}
//		else{
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pF5218A );
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5218B );
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pF5218C );
//			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5218T );
//			(*m_pSnHeadData)->Sn_TOKUR = 0;
//		}
//---------------
		BOOL	isUseTkSum = TRUE;
		if( m_pArith->l_cmp(pF5218T, WORK1) < 0 ){
			isUseTkSum = TRUE;
		}
		else if( m_pArith->l_cmp(pF5218T, WORK1) == 0 ){
			// 原則計算と特例計算の比較
			char	genSum[MONY_BUF_SIZE]={0}, tkSum[MONY_BUF_SIZE]={0};
			m_pArith->l_add( genSum, pF5218A, pF5218B );
			m_pArith->l_add( genSum, genSum, pF5218C );
			m_pArith->l_add( tkSum, pValA, pValB );
			m_pArith->l_add( tkSum, tkSum, pValC );
			if( m_pArith->l_cmp(genSum, tkSum) < 0 ){
				isUseTkSum = TRUE;
			}
			else{
				isUseTkSum = FALSE;
			}
		}
		else{
			isUseTkSum = FALSE;
		}
		if( isUseTkSum ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pValA );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pValB );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pValC );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pValT );
			(*m_pSnHeadData)->Sn_TOKUR = 1;
		}
		else{
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pF5218A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5218B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pF5218C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5218T );
			(*m_pSnHeadData)->Sn_TOKUR = 0;
		}
//---------------
	}

	// 最も有利な特例計算は？
	if( (*m_pSnHeadData)->Sn_TOKUR == 1 ){
		if( m_pArith->l_test(WORK1) ){
			*m_pSzsw = maxSign - (19-1);
		}
	}
	else{
		*m_pSzsw = 0;
	}
}
#endif

//-------------------------------------------------------------------
// 簡易課税　2種類以上の業種での特例計算チェック
//-------------------------------------------------------------------
int CShinFhyo522::f52_toku75( char *Base, char *Add1, char *Add2  )
{
/*	char	EXPW0[6], EXPW1[6], KZMY0[6], KZMY1[6], KZMY2[6];
	int		Status;

	// 計算準備
	memset( EXPW0, '\0', 6 );
	memset( EXPW1, '\0', 6 );
	m_pArith->l_input( EXPW0, _T("750") );
	m_pArith->l_input( EXPW1, _T("1000") );

	// 選択された2業種を合計する
	memset( KZMY0, '\0', 6 );
	m_pArith->l_add( KZMY0, Add1, Add2 );

	// 事業区分合計
	memset( KZMY1, '\0', 6 );
	memmove( KZMY1, Base, 6 );

	// 選択された2業種を合計を基に割合計算
	memset( KZMY2, '\0', 6 );
	m_Util.l_6calq( KZMY2, KZMY0, EXPW1, KZMY1 );	// 構成比
	
	// 特例７５％比較
	if( m_pArith->l_cmp( KZMY2, EXPW0 ) >= 0 ){
		Status = 1;	// 75%以上
	}
	else{
		Status = 0;
	}

	return( Status );*/
	return 0;
}

