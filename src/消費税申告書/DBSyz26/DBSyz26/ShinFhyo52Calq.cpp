
#include "stdafx.h"
#include "ShinFhyo52.h"
#include "ShinFhyo52Idx.h"

#include "H26HyoView.h"
#include <math.h>

//-----------------------------------------------------------------------------
// 課税標準額に対する消費税額
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_1()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID521T, F521T );	
	dsp_prs( IDC_ICSDIAGCTRL2, ID521A, F521A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID521B, F521B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID521C, F521C );
}

//-----------------------------------------------------------------------------
// 貸倒回収に対する消費税額
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_2()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID522T, F522T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID522A, F522A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID522B, F522B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID522C, F522C );
}

//-----------------------------------------------------------------------------
// 売上対価の返還等に係る消費税額
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_3()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID523T, F523T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID523A, F523A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID523B, F523B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID523C, F523C );
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額計算の基礎となる消費税額
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_4()
{
//	m_pArith->l_add( F524T, F521T, F522T );
//	m_pArith->l_sub( F524T, F524T, F523T );
//	[07'06.07] カット漏れ　そのまま出荷影響無し

	// 4%
	m_pArith->l_add( F524B, F521B, F522B );
	m_pArith->l_sub( F524B, F524B, F523B );
//	[07'06.07] カット漏れ　そのまま出荷影響無し
	if( m_pArith->l_test(F524B)  < 0  ){
		memset( F524B, '\0', MONY_BUF_SIZE );
	}
	// 3%
	m_pArith->l_add( F524A, F521A, F522A );
	m_pArith->l_sub( F524A, F524A, F523A );
	if( m_pArith->l_test(F524A) < 0  ){
		memset( F524A, '\0', MONY_BUF_SIZE );
	}
	// 6.3%
	m_pArith->l_add( F524C, F521C, F522C );
	m_pArith->l_sub( F524C, F524C, F523C );
	if( m_pArith->l_test(F524C) < 0 ){
		memset( F524C, '\0', MONY_BUF_SIZE );
	}
//-- '15.01.26 --
//	m_pArith->l_add( F524T, F524A, F524B );
//	m_pArith->l_add( F524T, F524T, F524C );
//---------------
	m_pArith->l_add( F524T, F521T, F522T );
	m_pArith->l_sub( F524T, F524T, F523T );
//---------------

	if( m_pArith->l_test(F524T) < 0  ){
		memset( F524T, '\0', MONY_BUF_SIZE );
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID524T, F524T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID524A, F524A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID524B, F524B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID524C, F524C );
	if( m_pArith->l_cmp(F524S, F524T) ){
//		m_CQsw &= 0x0f;
	}
}

#ifdef _20140302_CUT_
//-----------------------------------------------------------------------------
// 事業区分別の合計消費税額 （期間計）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_gts( int sg )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( (sg==2) || (F520c&0xfff) || !m_pArith->l_test(F5212T) ){
		memset( F5212T, '\0', MONY_BUF_SIZE );
		m_pArith->l_sub( WORK0, F526T, F526B );
		m_Util.percent( F5212T, WORK0, 1, 30, 0 );
		F520c &= 0xf0;
		m_pArith->l_add( F5212T, F5212T, F5212B );
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID5212T, F5212T );
}
#endif

#ifdef _20140118_EXTEND_
//-----------------------------------------------------------------------------
// 事業区分別の合計消費税額 （4%分）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_gks()
{
	char	sw;
	
//	if( (F520c&0x10) || (m_pArith->l_test(F5212B)==0) ){
	if( 0 ){
		memset( F5212B, '\0', MONY_BUF_SIZE );
		m_Util.percent( F5212B, F526B, 1, 40, 0 );
//		F520c &= 0x0f;
		sw = 2;
	}
	else{
		sw = 1;
	}
//	f52calq_gts( sw );
	
	dsp_prs( IDC_ICSDIAGCTRL2, ID5212B, F5212B );
}
#else
//-----------------------------------------------------------------------------
// 事業区分別の合計消費税額
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_gks()
{
//-- '14.10.17 --
//	// 3%分
//	m_pArith->l_add( F5212A, F5213A, F5214A );
//	m_pArith->l_add( F5212A, F5212A, F5215A );
//	m_pArith->l_add( F5212A, F5212A, F5216A );
//	m_pArith->l_add( F5212A, F5212A, F5217A );
//	// 4%分
//	m_pArith->l_add( F5212B, F5213B, F5214B );
//	m_pArith->l_add( F5212B, F5212B, F5215B );
//	m_pArith->l_add( F5212B, F5212B, F5216B );
//	m_pArith->l_add( F5212B, F5212B, F5217B );
//	// 6.3%分
//	m_pArith->l_add( F5212C, F5213C, F5214C );
//	m_pArith->l_add( F5212C, F5212C, F5215C );
//	m_pArith->l_add( F5212C, F5212C, F5216C );
//	m_pArith->l_add( F5212C, F5212C, F5217C );
//---------------
	// 3%分
	m_pArith->l_clear( F5212A );
	if( m_pArith->l_test(F5213A) > 0 ){
		m_pArith->l_add( F5212A, F5212A, F5213A );
	}
	if( m_pArith->l_test(F5214A) > 0 ){
		m_pArith->l_add( F5212A, F5212A, F5214A );
	}
	if( m_pArith->l_test(F5215A) > 0 ){
		m_pArith->l_add( F5212A, F5212A, F5215A );
	}
	if( m_pArith->l_test(F5216A) > 0 ){
		m_pArith->l_add( F5212A, F5212A, F5216A );
	}
	if( m_pArith->l_test(F5217A) > 0 ){
		m_pArith->l_add( F5212A, F5212A, F5217A );
	}
	// 4%分
	m_pArith->l_clear( F5212B );
	if( m_pArith->l_test(F5213B) > 0 ){
		m_pArith->l_add( F5212B, F5212B, F5213B );
	}
	if( m_pArith->l_test(F5214B) > 0 ){
		m_pArith->l_add( F5212B, F5212B, F5214B );
	}
	if( m_pArith->l_test(F5215B) > 0 ){
		m_pArith->l_add( F5212B, F5212B, F5215B );
	}
	if( m_pArith->l_test(F5216B) > 0 ){
		m_pArith->l_add( F5212B, F5212B, F5216B );
	}
	if( m_pArith->l_test(F5217B) > 0 ){
		m_pArith->l_add( F5212B, F5212B, F5217B );
	}
	// 6.3%分
	m_pArith->l_clear( F5212C );
	if( m_pArith->l_test(F5213C) > 0 ){
		m_pArith->l_add( F5212C, F5212C, F5213C );
	}
	if( m_pArith->l_test(F5214C) > 0 ){
		m_pArith->l_add( F5212C, F5212C, F5214C );
	}
	if( m_pArith->l_test(F5215C) > 0 ){
		m_pArith->l_add( F5212C, F5212C, F5215C );
	}
	if( m_pArith->l_test(F5216C) > 0 ){
		m_pArith->l_add( F5212C, F5212C, F5216C );
	}
	if( m_pArith->l_test(F5217C) > 0 ){
		m_pArith->l_add( F5212C, F5212C, F5217C );
	}
//---------------

//-- '15.01.26 --
//	m_pArith->l_add( F5212T, F5212A, F5212B );
//	m_pArith->l_add( F5212T, F5212T, F5212C );
//---------------
	m_pArith->l_clear( F5212T );
	if( m_pArith->l_test(F5213T) > 0 ){
		m_pArith->l_add( F5212T, F5212T, F5213T );
	}
	if( m_pArith->l_test(F5214T) > 0 ){
		m_pArith->l_add( F5212T, F5212T, F5214T );
	}
	if( m_pArith->l_test(F5215T) > 0 ){
		m_pArith->l_add( F5212T, F5212T, F5215T );
	}
	if( m_pArith->l_test(F5216T) > 0 ){
		m_pArith->l_add( F5212T, F5212T, F5216T );
	}
	if( m_pArith->l_test(F5217T) > 0 ){
		m_pArith->l_add( F5212T, F5212T, F5217T );
	}
//---------------
	
	dsp_prs( IDC_ICSDIAGCTRL2, ID5212A, F5212A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5212B, F5212B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5212C, F5212C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5212T, F5212T );
}
#endif

//-----------------------------------------------------------------------------
// 事業別課税売上割合の表示
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_uws()
{
	// 課税期間計
	dsp_uws( IDC_ICSDIAGCTRL2, ID527TE, (*m_pSnHonpyoData)->Sn_UP1, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID528TE, (*m_pSnHonpyoData)->Sn_UP2, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID529TE, (*m_pSnHonpyoData)->Sn_UP3, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID5210TE, (*m_pSnHonpyoData)->Sn_UP4, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID5211TE, (*m_pSnHonpyoData)->Sn_UP5, 1 );

/*	// うち4%分
	dsp_uws( IDC_ICSDIAGCTRL2, ID527BE, F521UPb, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID528BE, F522UPb, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID529BE, F523UPb, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID5210BE, F524UPb, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID5211BE, F525UPb, 1 );*/
}

#ifdef _20140118_EXTEND_
//-----------------------------------------------------------------------------
// 第一種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku1()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID527T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5213T );
	if( !m_pArith->l_test(F527T) ){
		memset( F5213T, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F521c&0xff) || (m_pArith->l_test(F527T)&&(m_pArith->l_test(F5213T)==0)) ){
	if( 0 ){
		memset( F5213T, '\0', MONY_BUF_SIZE );
		m_pArith->l_sub( WORK0, F527T, F527B );
//		pSyzShin->percent( F5213T, WORK0, 1, 30, 0 );
//		F521c &= 0xf0;
	}
	else{
		m_pArith->l_sub( F5213T, F5213T, F5213B );
	}
	
	dsp_cls( IDC_ICSDIAGCTRL2, ID527B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5213B );
	if( m_pArith->l_test(F527B) == 0 ){
		memset( F5213B, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F521c&0x10) || (m_pArith->l_test(F527B)&&(m_pArith->l_test(F5213B)==0)) ){
	if( 0 ){
		memset( F523B, '\0', MONY_BUF_SIZE );
//		pSyzShin->percent( F5213B, F527B, 1, 40, 0 );
//		F521c &= 0x0f;
	}
	m_pArith->l_add( F5213T, F5213T, F5213B );

	dsp_prs( IDC_ICSDIAGCTRL2, ID527B,  F527B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5213B, F5213B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID527T,  F527T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5213T, F5213T );
}
#else
//-----------------------------------------------------------------------------
// 第一種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku1()
{
	// 横計
	m_pArith->l_add( F527T, F527A, F527B );
	m_pArith->l_add( F527T, F527T, F527C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F527T) < 0 ){
		m_pArith->l_clear(F527T);
	}
//<-- '14.10.07 INS END

	dsp_cls( IDC_ICSDIAGCTRL2, ID527A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID527B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID527C );
	dsp_cls( IDC_ICSDIAGCTRL2, ID527T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID527A,  F527A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID527B,  F527B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID527C,  F527C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID527T,  F527T );

	// 税額計算
	dsp_cls( IDC_ICSDIAGCTRL2, ID5213T );
	
	if( m_pArith->l_test(F527T) == 0 ){
		memset( F5213T, '\0', MONY_BUF_SIZE );
	}
	
	dsp_cls( IDC_ICSDIAGCTRL2, ID5213A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5213B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5213C );
	if( m_pArith->l_test(F527A) == 0 ){
		memset( F5213A, '\0', MONY_BUF_SIZE );
		F5213c &= ~0x08;
	}
	if( m_pArith->l_test(F527B) == 0 ){
		memset( F5213B, '\0', MONY_BUF_SIZE );
		F5213c &= ~0x080;
	}
	if( m_pArith->l_test(F527C) == 0 ){
		memset( F5213C, '\0', MONY_BUF_SIZE );
		F5213c &= ~0x0800;
	}

	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
		if( !(F5213c&0x08) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5213A, 0 );

			if( (F521c&0x01) || (m_pArith->l_test(F527A)&&(m_pArith->l_test(F5213A)==0)) ){
				memset( F5213A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5213A, F527A, 1, 30, 0 );
				F521c &= ~0x0f;
			}
		}

		if( !(F5213c&0x80) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5213B, 0 );
			if( (F521c&0x10) || (m_pArith->l_test(F527B)&&(m_pArith->l_test(F5213B)==0)) ){
				memset( F5213B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5213B, F527B, 1, 40, 0 );
				F521c &= ~0xf0;
			}
		}

		if( !(F5213c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5213C, 0 );
			if( (F521c&0x100) || (m_pArith->l_test(F527C)&&(m_pArith->l_test(F5213C)==0)) ){
				memset( F5213C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5213C, F527C, 1, 63, 0 );
				F521c &= ~0xf00;
			}
		}
	}
	else{

	}

	m_pArith->l_add( F5213T, F5213A, F5213B );
	m_pArith->l_add( F5213T, F5213T, F5213C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5213T) < 0 ){
		m_pArith->l_clear(F5213T);
	}
//<-- '14.10.07 INS END

	dsp_prs( IDC_ICSDIAGCTRL2, ID5213A, F5213A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5213B, F5213B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5213C, F5213C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5213T, F5213T );

	//[Y]入力色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( F5213c&0x08 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5213A, 3 );
		}
		if( F5213c&0x80 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5213B, 3 );
		}
		if( F5213c&0x800 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5213C, 3 );
		}
	}
}
#endif

#ifdef _20140118_EXTEND_
//-----------------------------------------------------------------------------
// 第二種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku2()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID528T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5214T );
	if( m_pArith->l_test(F528T) == 0 ){
		memset( F5214T, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F522c&0xff) || (m_pArith->l_test(F528T)&&!m_pArith->l_test(F5214T)) ){
	if( 0 ){
		memset( F5214T, '\0', MONY_BUF_SIZE );
		m_pArith->l_sub( WORK0, F528T, F528B );
		m_Util.percent( F5214T, WORK0, 1, 30, 0 );
//		F522c &= 0xf0;
	}
	else{
		m_pArith->l_sub( F5214T, F5214T, F5214B );
	}
	
	dsp_cls( IDC_ICSDIAGCTRL2, ID528B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5214B );
	if( m_pArith->l_test(F528B) == 0 ){
		memset( F5214B, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F522c&0x10) || (m_pArith->l_test(F528B)&&(m_pArith->l_test(F5214B)==0)) ){
	if( 0 ){
		memset( F5214B, '\0', MONY_BUF_SIZE );
		m_Util.percent( F5214B, F528B, 1, 40, 0 );
//		F522c &= 0x0f;
	}
	m_pArith->l_add( F5214T, F5214T, F5214B );

	dsp_prs( IDC_ICSDIAGCTRL2, ID528B, F528B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5214B, F5214B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID528T, F528T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5214T, F5214T );
}
#else
//-----------------------------------------------------------------------------
// 第二種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku2()
{
	// 横計
	m_pArith->l_add( F528T, F528A, F528B );
	m_pArith->l_add( F528T, F528T, F528C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F528T) < 0 ){
		m_pArith->l_clear(F528T);
	}
//<-- '14.10.07 INS END

	dsp_cls( IDC_ICSDIAGCTRL2, ID528A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID528B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID528C );
	dsp_cls( IDC_ICSDIAGCTRL2, ID528T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID528A,  F528A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID528B,  F528B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID528C,  F528C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID528T,  F528T );

	// 税額計算
	dsp_cls( IDC_ICSDIAGCTRL2, ID5214T );
	
	if( m_pArith->l_test(F528T) == 0 ){
		memset( F5214T, '\0', MONY_BUF_SIZE );
	}
		
	dsp_cls( IDC_ICSDIAGCTRL2, ID5214A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5214B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5214C );
	if( m_pArith->l_test(F528A) == 0 ){
		memset( F5214A, '\0', MONY_BUF_SIZE );
		F5214c &= ~0x08;
	}
	if( m_pArith->l_test(F528B) == 0 ){
		memset( F5214B, '\0', MONY_BUF_SIZE );
		F5214c &= ~0x80;
	}
	if( m_pArith->l_test(F528C) == 0 ){
		memset( F5214C, '\0', MONY_BUF_SIZE );
		F5214c &= ~0x800;
	}
	
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
		if( !(F5214c&0x08) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5214A, 0 );
			if( (F522c&0x01) || (m_pArith->l_test(F528A)&&(m_pArith->l_test(F5214A)==0)) ){
				memset( F5214A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5214A, F528A, 1, 30, 0 );
				F522c &= ~0x0f;
			}
		}
		if( !(F5214c&0x80) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5214B, 0 );
			if( (F522c&0x10) || (m_pArith->l_test(F528B)&&(m_pArith->l_test(F5214B)==0)) ){
				memset( F5214B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5214B, F528B, 1, 40, 0 );
				F522c &= ~0xf0;
			}
		}
		if( !(F5214c&0x800) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5214C, 0 );
			if( (F522c&0x100) || (m_pArith->l_test(F528C)&&(m_pArith->l_test(F5214C)==0)) ){
				memset( F5214C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5214C, F528C, 1, 63, 0 );
				F522c &= ~0xf00;
			}
		}
	}
	else{
		/*if( (F522c&0x01) || (m_pArith->l_test(F528A)&&(m_pArith->l_test(F5214A)==0)) ){
			memset( F5214A, '\0', MONY_BUF_SIZE );
			m_Util.percent( F5214A, F528A, 1, 30, 0 );
			F522c &= ~0x0f;
		}
		if( (F522c&0x10) || (m_pArith->l_test(F528B)&&(m_pArith->l_test(F5214B)==0)) ){
			memset( F5214B, '\0', MONY_BUF_SIZE );
			m_Util.percent( F5214B, F528B, 1, 40, 0 );
			F522c &= ~0xf0;
		}
		if( (F522c&0x100) || (m_pArith->l_test(F528C)&&(m_pArith->l_test(F5214C)==0)) ){
			memset( F5214C, '\0', MONY_BUF_SIZE );
			m_Util.percent( F5214C, F528C, 1, 63, 0 );
			F522c &= ~0xf00;
		}*/
	}

	m_pArith->l_add( F5214T, F5214A, F5214B );
	m_pArith->l_add( F5214T, F5214T, F5214C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5214T) < 0 ){
		m_pArith->l_clear(F5214T);
	}
//<-- '14.10.07 INS END

	dsp_prs( IDC_ICSDIAGCTRL2, ID5214A, F5214A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5214B, F5214B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5214C, F5214C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5214T, F5214T );

	//[Y]入力色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( F5214c&0x08 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5214A, 3 );
		}
		if( F5214c&0x80 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5214B, 3 );
		}
		if( F5214c&0x800 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5214C, 3 );
		}
	}

}
#endif

#ifdef _20140118_EXTEND_
//-----------------------------------------------------------------------------
// 第三種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku3()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID529T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5215T );
	if( m_pArith->l_test(F529T) == 0 ){
		memset( F5215T, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F523c&0xff) || (m_pArith->l_test(F529T)&&(m_pArith->l_test(F5215T)==0)) ){
	if( 0 ){
		memset( F5215T, '\0', MONY_BUF_SIZE );
		m_pArith->l_sub( WORK0, F529T, F529B );
		m_Util.percent( F5215T, WORK0, 1, 30, 0 );
//		F523c &= 0xf0;
	}
	else {
		m_pArith->l_sub( F5215T, F5215T, F5215B );
	}
	
	dsp_cls( IDC_ICSDIAGCTRL2, ID529B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5215B );
	if( m_pArith->l_test(F529B) == 0 ){
		memset( F5215B, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F523c&0x10) || (m_pArith->l_test(F529B)&&(m_pArith->l_test(F5215B)==0)) ){
	if( 0 ){
		memset( F5215B, '\0', MONY_BUF_SIZE );
		m_Util.percent( F5215B, F529B, 1, 40, 0 );
//		F523c &= 0x0f;
	}
	m_pArith->l_add( F5215T, F5215T, F5215B );

	dsp_prs( IDC_ICSDIAGCTRL2, ID529B, F529B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5215B, F5215B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID529T, F529T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5215T, F5215T );
}
#else
//-----------------------------------------------------------------------------
// 第三種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku3()
{
	// 横計
	m_pArith->l_add( F529T, F529A, F529B );
	m_pArith->l_add( F529T, F529T, F529C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F529T) < 0 ){
		m_pArith->l_clear(F529T);
	}
//<-- '14.10.07 INS END

	dsp_cls( IDC_ICSDIAGCTRL2, ID529A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID529B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID529C );
	dsp_cls( IDC_ICSDIAGCTRL2, ID529T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID529A,  F529A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID529B,  F529B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID529C,  F529C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID529T,  F529T );

	// 税額計算
	dsp_cls( IDC_ICSDIAGCTRL2, ID5215T );
	
	if( m_pArith->l_test(F529T) == 0 ){
		memset( F5215T, '\0', MONY_BUF_SIZE );
	}
		
	dsp_cls( IDC_ICSDIAGCTRL2, ID5215A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5215B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5215C );
	if( m_pArith->l_test(F529A) == 0 ){
		memset( F5215A, '\0', MONY_BUF_SIZE );
		F5215c &= ~0x08;
	}
	if( m_pArith->l_test(F529B) == 0 ){
		memset( F5215B, '\0', MONY_BUF_SIZE );
		F5215c &= ~0x80;
	}
	if( m_pArith->l_test(F529C) == 0 ){
		memset( F5215C, '\0', MONY_BUF_SIZE );
		F5215c &= ~0x800;
	}
	
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
		if( !(F5215c&0x08) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5215A, 0 );
			if( (F523c&0x01) || (m_pArith->l_test(F529A)&&(m_pArith->l_test(F5215A)==0)) ){
				memset( F5215A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5215A, F529A, 1, 30, 0 );
				F523c &= ~0x0f;
			}
		}
		
		if( !(F5215c&0x80) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5215B, 0 );
			if( (F523c&0x10) || (m_pArith->l_test(F529B)&&(m_pArith->l_test(F5215B)==0)) ){
				memset( F5215B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5215B, F529B, 1, 40, 0 );
				F523c &= ~0xf0;
			}
		}

		if( !(F5215c&0x800) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5215C, 0 );
			if( (F523c&0x100) || (m_pArith->l_test(F529C)&&(m_pArith->l_test(F5215C)==0)) ){
				memset( F5215C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5215C, F529C, 1, 63, 0 );
				F523c &= ~0xf00;
			}
		}
	}
	else{
	}

	m_pArith->l_add( F5215T, F5215A, F5215B );
	m_pArith->l_add( F5215T, F5215T, F5215C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5215T) < 0 ){
		m_pArith->l_clear(F5215T);
	}
//<-- '14.10.07 INS END

	dsp_prs( IDC_ICSDIAGCTRL2, ID5215A, F5215A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5215B, F5215B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5215C, F5215C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5215T, F5215T );

	//[Y]入力色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( F5215c&0x08 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5215A, 3 );
		}
		if( F5215c&0x80 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5215B, 3 );
		}
		if( F5215c&0x800 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5215C, 3 );
		}
	}

}
#endif

#ifdef _20140118_EXTEND_
//-----------------------------------------------------------------------------
// 第四種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku4()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID5210T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5216T );
	if( m_pArith->l_test(F5210T) == 0 ){
		memset( F5216T, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F524c&0xff) || (m_pArith->l_test(F5210T)&&(m_pArith->l_test(F5216T)==0)) ){
	if( 0 ){
		memset( F5216T, '\0', MONY_BUF_SIZE );
		m_pArith->l_sub( WORK0, F5210T, F5210B );
		m_Util.percent( F5216T, WORK0, 1, 30, 0 );
//		F524c &= 0xf0;
	}
	else{
		m_pArith->l_sub( F5216T, F5216T, F5216B );
	}
	
	dsp_cls( IDC_ICSDIAGCTRL2, ID5210B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5216B );
	if( m_pArith->l_test(F5210B) == 0 ){
		memset( F5216B, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F524c&0x10) || (m_pArith->l_test(F5210B)&&(m_pArith->l_test(F5216B)==0)) ){
	if( 0 ){
		memset( F5216B, '\0', MONY_BUF_SIZE );
		m_Util.percent( F5216B, F5210B, 1, 40, 0 );
//		F524c &= 0x0f;
	}
	m_pArith->l_add( F5216T, F5216T, F5216B );

	dsp_prs( IDC_ICSDIAGCTRL2, ID5210B, F5210B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5216B, F5216B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5210T, F5210T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5216T, F5216T );
}
#else
//-----------------------------------------------------------------------------
// 第四種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku4()
{
	// 横計
	m_pArith->l_add( F5210T, F5210A, F5210B );
	m_pArith->l_add( F5210T, F5210T, F5210C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5210T) < 0 ){
		m_pArith->l_clear(F5210T);
	}
//<-- '14.10.07 INS END

	dsp_cls( IDC_ICSDIAGCTRL2, ID5210A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5210B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5210C );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5210T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5210A,  F5210A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5210B,  F5210B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5210C,  F5210C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5210T,  F5210T );

	// 税額計算
	dsp_cls( IDC_ICSDIAGCTRL2, ID5216T );
	
	if( m_pArith->l_test(F5210T) == 0 ){
		memset( F5216T, '\0', MONY_BUF_SIZE );
	}
		
	dsp_cls( IDC_ICSDIAGCTRL2, ID5216A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5216B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5216C );
	if( m_pArith->l_test(F5210A) == 0 ){
		memset( F5216A, '\0', MONY_BUF_SIZE );
		F5216c &= ~0x08;
	}
	if( m_pArith->l_test(F5210B) == 0 ){
		memset( F5216B, '\0', MONY_BUF_SIZE );
		F5216c &= ~0x80;
	}
	if( m_pArith->l_test(F5210C) == 0 ){
		memset( F5216C, '\0', MONY_BUF_SIZE );
		F5216c &= ~0x800;
	}
	
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
		if( !(F5216c&0x08) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5216A, 0 );
			if( (F524c&0x01) || (m_pArith->l_test(F5210A)&&(m_pArith->l_test(F5216A)==0)) ){
				memset( F5216A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5216A, F5210A, 1, 30, 0 );
				F524c &= ~0x0f;
			}
		}
		if( !(F5216c&0x80) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5216B, 0 );
			if( (F524c&0x10) || (m_pArith->l_test(F5210B)&&(m_pArith->l_test(F5216B)==0)) ){
				memset( F5216B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5216B, F5210B, 1, 40, 0 );
				F524c &= ~0xf0;
			}
		}
		if( !(F5216c&0x800) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5216C, 0 );
			if( (F524c&0x100) || (m_pArith->l_test(F5210C)&&(m_pArith->l_test(F5216C)==0)) ){
				memset( F5216C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5216C, F5210C, 1, 63, 0 );
				F524c &= ~0xf00;
			}
		}
	}
	else{

	}

	m_pArith->l_add( F5216T, F5216A, F5216B );
	m_pArith->l_add( F5216T, F5216T, F5216C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5216T) < 0 ){
		m_pArith->l_clear(F5216T);
	}
//<-- '14.10.07 INS END

	dsp_prs( IDC_ICSDIAGCTRL2, ID5216A, F5216A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5216B, F5216B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5216C, F5216C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5216T, F5216T );

	//[Y]入力色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( F5216c&0x08 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5216A, 3 );
		}
		if( F5216c&0x80 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5216B, 3 );
		}
		if( F5216c&0x800 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5216C, 3 );
		}
	}

}
#endif

#ifdef _20140118_EXTEND_
//-----------------------------------------------------------------------------
// 第五種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku5()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID5211T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5217T );
	if( m_pArith->l_test(F5211T) == 0 ){
		memset( F5217T, '\0', MONY_BUF_SIZE );
	}
	
//	if( (F525c&0xff) || (m_pArith->l_test(F5211T)&&(m_pArith->l_test(F5217T)==0)) ){
	if( 0 ){
		memset( F5217T, '\0', MONY_BUF_SIZE );
		m_pArith->l_sub( WORK0, F5211T, F5211B );
		m_Util.percent( F5217T, WORK0, 1, 30, 0 );
//		F525c &= 0xf0;
	}
	else{
		m_pArith->l_sub( F5217T, F5217T, F5217B );
	}
	
	dsp_cls( IDC_ICSDIAGCTRL2, ID5211B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5217B );
	
//	if( (F525c&0xff) || (m_pArith->l_test(F5211B)&&(m_pArith->l_test(F5217B)==0)) ){
	if( 0 ){
		memset( F5217B, '\0', MONY_BUF_SIZE );
		m_Util.percent( F5217B, F5211B, 1, 40, 0 );
//		F525c &= 0x0f;
	}
	m_pArith->l_add( F5217T, F5217T, F5217B );

	dsp_prs( IDC_ICSDIAGCTRL2, ID5211B, F5211B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5217B, F5217B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5211T, F5211T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5217T, F5217T );
}
#else
//-----------------------------------------------------------------------------
// 第五種事業
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku5()
{
	// 横計
	m_pArith->l_add( F5211T, F5211A, F5211B );
	m_pArith->l_add( F5211T, F5211T, F5211C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5211T) < 0 ){
		m_pArith->l_clear(F5211T);
	}
//<-- '14.10.07 INS END

	dsp_cls( IDC_ICSDIAGCTRL2, ID5211A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5211B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5211C );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5211T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5211A,  F5211A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5211B,  F5211B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5211C,  F5211C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5211T,  F5211T );

	// 税額計算
	dsp_cls( IDC_ICSDIAGCTRL2, ID5217T );
	
	if( m_pArith->l_test(F5211T) == 0 ){
		memset( F5217T, '\0', MONY_BUF_SIZE );
	}
		
	dsp_cls( IDC_ICSDIAGCTRL2, ID5217A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5217B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID5217C );
	if( m_pArith->l_test(F5211A) == 0 ){
		memset( F5217A, '\0', MONY_BUF_SIZE );
		F5217c &= ~0x08;
	}
	if( m_pArith->l_test(F5211B) == 0 ){
		memset( F5217B, '\0', MONY_BUF_SIZE );
		F5217c &= ~0x80;
	}
	if( m_pArith->l_test(F5211C) == 0 ){
		memset( F5217C, '\0', MONY_BUF_SIZE );
		F5217c &= ~0x800;
	}
	
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
		if( !(F5217c&0x08) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5217A, 0 );
			if( (F525c&0x01) || (m_pArith->l_test(F5211A)&&(m_pArith->l_test(F5217A)==0)) ){
				memset( F5217A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5217A, F5211A, 1, 30, 0 );
				F525c &= ~0x0f;
			}
		}
		if( !(F5217c&0x80) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5217B, 0 );
			if( (F525c&0x10) || (m_pArith->l_test(F5211B)&&(m_pArith->l_test(F5217B)==0)) ){
				memset( F5217B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5217B, F5211B, 1, 40, 0 );
				F525c &= ~0xf0;
			}
		}
		if( !(F5217c&0x800) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID5217C, 0 );
			if( (F525c&0x100) || (m_pArith->l_test(F5211C)&&(m_pArith->l_test(F5217C)==0)) ){
				memset( F5217C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F5217C, F5211C, 1, 63, 0 );
				F525c &= ~0xf00;
			}
		}
	}
	else{

	}

	m_pArith->l_add( F5217T, F5217A, F5217B );
	m_pArith->l_add( F5217T, F5217T, F5217C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F5217T) < 0 ){
		m_pArith->l_clear(F5217T);
	}
//<-- '14.10.07 INS END

	dsp_prs( IDC_ICSDIAGCTRL2, ID5217A, F5217A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5217B, F5217B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5217C, F5217C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID5217T, F5217T );

	//[Y]入力色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( F5217c&0x08 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5217A, 3 );
		}
		if( F5217c&0x80 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5217B, 3 );
		}
		if( F5217c&0x800 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID5217C, 3 );
		}
	}

}
#endif

//-----------------------------------------------------------------------------
// 事業別課税売上割合の計算
//-----------------------------------------------------------------------------
// 引数	tk_clqsw	：	特例計算の優先順位を再設定するかどうか（1:する, 0:しない）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_ku( int tk_clqsw/*=1*/ )
{
	char kani = 0x00;
//	m_typeCnt = 0;

//--> '14.01.18 移動
	f52calq_ku1();
	f52calq_ku2();
	f52calq_ku3();
	f52calq_ku4();
	f52calq_ku5();
//<--

	if( m_pArith->l_test(F527T) ){
		kani |= 0x01;
//		m_typeCnt++;
	}
	if( m_pArith->l_test(F528T) ){
		kani |= 0x02;
//		m_typeCnt++;
	}
	if( m_pArith->l_test(F529T) ){
		kani |= 0x04;
//		m_typeCnt++;
	}
	if( m_pArith->l_test(F5210T) ){
		kani |= 0x08;
//		m_typeCnt++;
	}
	if( m_pArith->l_test(F5211T) ){
		kani |= 0x10;
//		m_typeCnt++;
	}

	if( m_typeCnt == 1 ){
		m_CQsw |= 0x01;
		m_Szsw = m_SBsw = 0x00;
	}
	else {
		m_CQsw &= 0xf0;
		if( m_typeCnt == 0 ){
			memset( F526T,  '\0', MONY_BUF_SIZE );
			memset( F526A,  '\0', MONY_BUF_SIZE );
			memset( F526B,  '\0', MONY_BUF_SIZE );
			memset( F526C,  '\0', MONY_BUF_SIZE );
			memset( F527T,  '\0', MONY_BUF_SIZE );
			memset( F527A,  '\0', MONY_BUF_SIZE );
			memset( F527B,  '\0', MONY_BUF_SIZE );
			memset( F527C,  '\0', MONY_BUF_SIZE );
			memset( F528T,  '\0', MONY_BUF_SIZE );
			memset( F528A,  '\0', MONY_BUF_SIZE );
			memset( F528B,  '\0', MONY_BUF_SIZE );
			memset( F528C,  '\0', MONY_BUF_SIZE );
			memset( F529T,  '\0', MONY_BUF_SIZE );
			memset( F529A,  '\0', MONY_BUF_SIZE );
			memset( F529B,  '\0', MONY_BUF_SIZE );
			memset( F529C,  '\0', MONY_BUF_SIZE );
			memset( F5210T, '\0', MONY_BUF_SIZE );
			memset( F5210A, '\0', MONY_BUF_SIZE );
			memset( F5210B, '\0', MONY_BUF_SIZE );
			memset( F5210C, '\0', MONY_BUF_SIZE );
			memset( F5211T, '\0', MONY_BUF_SIZE );	
			memset( F5211A, '\0', MONY_BUF_SIZE );
			memset( F5211B, '\0', MONY_BUF_SIZE );
			memset( F5211C, '\0', MONY_BUF_SIZE );
			memset( F5212T, '\0', MONY_BUF_SIZE );	
			memset( F5212A, '\0', MONY_BUF_SIZE );
			memset( F5212B, '\0', MONY_BUF_SIZE );
			memset( F5212C, '\0', MONY_BUF_SIZE );
			memset( F5213T, '\0', MONY_BUF_SIZE );	
			memset( F5213A, '\0', MONY_BUF_SIZE );
			memset( F5213B, '\0', MONY_BUF_SIZE );
			memset( F5213C, '\0', MONY_BUF_SIZE );
			memset( F5214T, '\0', MONY_BUF_SIZE );	
			memset( F5214A, '\0', MONY_BUF_SIZE );
			memset( F5214B, '\0', MONY_BUF_SIZE );
			memset( F5214C, '\0', MONY_BUF_SIZE );
			memset( F5215T, '\0', MONY_BUF_SIZE );	
			memset( F5215A, '\0', MONY_BUF_SIZE );
			memset( F5215B, '\0', MONY_BUF_SIZE );
			memset( F5215C, '\0', MONY_BUF_SIZE );
			memset( F5216T, '\0', MONY_BUF_SIZE );
			memset( F5216A, '\0', MONY_BUF_SIZE );
			memset( F5216B, '\0', MONY_BUF_SIZE );
			memset( F5216C, '\0', MONY_BUF_SIZE );
			memset( F5217T, '\0', MONY_BUF_SIZE );	
			memset( F5217A, '\0', MONY_BUF_SIZE );
			memset( F5217B, '\0', MONY_BUF_SIZE );
			memset( F5217C, '\0', MONY_BUF_SIZE );
			memset( F5218T, '\0', MONY_BUF_SIZE );
			memset( F5218A, '\0', MONY_BUF_SIZE );
			memset( F5218B, '\0', MONY_BUF_SIZE );
			memset( F5218C, '\0', MONY_BUF_SIZE );
			memset( F5219T, '\0', MONY_BUF_SIZE );
			memset( F5219A, '\0', MONY_BUF_SIZE );
			memset( F5219B, '\0', MONY_BUF_SIZE );
			memset( F5219C, '\0', MONY_BUF_SIZE );
		}
	}

//--> '14.01.18 移動
//	f52calq_ku1();
//	f52calq_ku2();
//	f52calq_ku3();
//	f52calq_ku4();
//	f52calq_ku5();
//<--

/*- '14.02.19 -*/
//	if( calq_CfrOfSales(0) || calq_CfrOfSales(1) ){
/*-------------*/
	if( calq_CfrOfSales(0) ){
/*-------------*/
		f52calq_uws();
	}

	// 合計
	dsp_prs( IDC_ICSDIAGCTRL2, ID526A, F526A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID526B, F526B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID526C, F526C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID526T, F526T );

	// 合計消費税額
	f52calq_gks();

	if( m_pArith->l_cmp(F526S, F526T) || m_pArith->l_cmp(F526SS, F526B) ){
		m_CQsw &= 0x0f;
	}

	// 複数事業種選択時のみ表示
	if( m_typeCnt > 1 ){
		// 売上区分チェック
		f52_urikbn();
		// 原則計算を適用する場合
		f52calq_gn();
		// 特例計算を適用する場合
/*- '14.02.20 -*/
//		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//			f52calq_tk_all();
//		}
//		else{
//			f52calq_tk();
//		}
/*-------------*/
		f52calq_tk_all( tk_clqsw );
/*-------------*/
	}
}

//-----------------------------------------------------------------------------
// 各入力の計算表示
//-----------------------------------------------------------------------------
// 引数	atno		：	計算開始位置
//		tk_clqsw	：	特例計算の優先順位を再設定するかどうか（1:する, 0:しない）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_all( int atno, int tk_clqsw/*=1*/ )
{

	while( 1 ){
		switch( atno ){
			case	1 :
				f52calq_1();		// 課税標準額消費税額
			case	2 :
				f52calq_2();		// 貸倒回収消費税額
			case	3 :
				f52calq_3();		// 売上対価の返還等消費税額
				f52calq_4();		// （１）＋（２）－（３）
			case	4 :
				f52calq_gsubtotal();
				f52calq_5();		// 単一事業の場合
			case	5 :
			case	6 :
			case	7 :
			case	8 :
			case	9 :
				if( m_Tmp == 0 ){
					f52calq_ku( tk_clqsw );	// 事業別課税売上高の計算
				}
			case	10:
			case	11:
			case	12:
			case	13:
			case	14:
			case	15:	
				if( m_typeCnt > 1 ){// 複数事業種選択時のみ
					f52calq_gks();	// 課税売上に係る税額計
				}
			default	:
				break;
		}
		break;
	}
}

//----------------------------------------------------------------------------
// 各事業の課税売上高の横計
//----------------------------------------------------------------------------
void CShinFhyo52::f52calq_gsubtotal()
{
//--> '14.10.07 INS START
	m_typeCnt = 0;
//<-- '14.10.07 INS END

	// 第１種
	m_pArith->l_add( F527T, F527A, F527B );
	m_pArith->l_add( F527T, F527T, F527C );
//--> '14.10.07 INS START
//--'15.01.27 --
//	if( m_pArith->l_test(F527T) ){
//--------------
	if( m_pArith->l_test(F527A) || m_pArith->l_test(F527B) || m_pArith->l_test(F527C) ){
//--------------
		m_typeCnt++;
	}
	if( m_pArith->l_test(F527T) < 0 ){
		m_pArith->l_clear(F527T);
	}
//<-- '14.10.07 INS END
	// 第２種
	m_pArith->l_add( F528T, F528A, F528B );
	m_pArith->l_add( F528T, F528T, F528C );
//--> '14.10.07 INS START
//--'15.01.27 --
//	if( m_pArith->l_test(F528T) ){
//--------------
	if( m_pArith->l_test(F528A) || m_pArith->l_test(F528B) || m_pArith->l_test(F528C) ){
//--------------
		m_typeCnt++;
	}
	if( m_pArith->l_test(F528T) < 0 ){
		m_pArith->l_clear(F528T);
	}
//<-- '14.10.07 INS END
	// 第３種
	m_pArith->l_add( F529T, F529A, F529B );
	m_pArith->l_add( F529T, F529T, F529C );
//--> '14.10.07 INS START
//--'15.01.27 --
//	if( m_pArith->l_test(F529T) ){
//--------------
	if( m_pArith->l_test(F529A) || m_pArith->l_test(F529B) || m_pArith->l_test(F529C) ){
//--------------
		m_typeCnt++;
	}
	if( m_pArith->l_test(F529T) < 0 ){
		m_pArith->l_clear(F529T);
	}
//<-- '14.10.07 INS END
	// 第４種
	m_pArith->l_add( F5210T, F5210A, F5210B );
	m_pArith->l_add( F5210T, F5210T, F5210C );
//--> '14.10.07 INS START
//--'15.01.27 --
//	if( m_pArith->l_test(F5210T) ){
//--------------
	if( m_pArith->l_test(F5210A) || m_pArith->l_test(F5210B) || m_pArith->l_test(F5210C) ){
//--------------
		m_typeCnt++;
	}
	if( m_pArith->l_test(F5210T) < 0 ){
		m_pArith->l_clear(F5210T);
	}
//<-- '14.10.07 INS END
	// 第５種
	m_pArith->l_add( F5211T, F5211A, F5211B );
	m_pArith->l_add( F5211T, F5211T, F5211C );
//--> '14.10.07 INS START
//--'15.01.27 --
//	if( m_pArith->l_test(F5211T) ){
//--------------
	if( m_pArith->l_test(F5211A) || m_pArith->l_test(F5211B) || m_pArith->l_test(F5211C) ){
//--------------
		m_typeCnt++;
	}
	if( m_pArith->l_test(F5211T) < 0 ){
		m_pArith->l_clear(F5211T);
	}
//<-- '14.10.07 INS END

	// 縦計
	if( !(F520c&0x08) ){
		m_pArith->l_add( F526A, F527A, F528A );
		m_pArith->l_add( F526A, F526A, F529A );
		m_pArith->l_add( F526A, F526A, F5210A );
		m_pArith->l_add( F526A, F526A, F5211A );
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID526A, 0 );
		}
	}

	if( !(F520c&0x80) ){
		m_pArith->l_add( F526B, F527B, F528B );
		m_pArith->l_add( F526B, F526B, F529B );
		m_pArith->l_add( F526B, F526B, F5210B );
		m_pArith->l_add( F526B, F526B, F5211B );
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID526B, 0 );
		}
	}

	if( !(F520c&0x800) ){
		m_pArith->l_add( F526C, F527C, F528C );
		m_pArith->l_add( F526C, F526C, F529C );
		m_pArith->l_add( F526C, F526C, F5210C );
		m_pArith->l_add( F526C, F526C, F5211C );
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID526C, 0 );
		}
	}
	
	//[Y]入力色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( F520c&0x08 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID526A, 3 );
		}
		if( F520c&0x80 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID526B, 3 );
		}
		if( F520c&0x800 ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID526C, 3 );
		}
	}

	// 合計
	m_pArith->l_add( F526T, F526A, F526B );
	m_pArith->l_add( F526T, F526T, F526C );
//--> '14.10.07 INS START
	if( m_pArith->l_test(F526T) < 0 ){
		m_pArith->l_clear(F526T);
	}
//<-- '14.10.07 INS END
}

//-----------------------------------------------------------------------------
// １種類の事業の専業者の場合
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_5()
{
	dsp_cls( IDC_ICSDIAGCTRL2, ID525A );
	dsp_cls( IDC_ICSDIAGCTRL2, ID525B );
	dsp_cls( IDC_ICSDIAGCTRL2, ID525C );
	dsp_cls( IDC_ICSDIAGCTRL2, ID525T );

/////////[09'11.02]
//	dsp_cls( IDC_ICSDIAGCTRL2, ID5218T );
//	dsp_cls( IDC_ICSDIAGCTRL2, ID5218B );
//	dsp_cls( IDC_ICSDIAGCTRL2, ID5230T );
//	dsp_cls( IDC_ICSDIAGCTRL2, ID5230B );
///////////////////

	char			kani = 0x00;	// □□□■ ■■■■
									// D0:第１ ,D1:第２, D2:第３, D3:第４, D4:第５
	unsigned short	per = 0;
//	m_typeCnt = 0;

//-- '15.01.28 --
//	if( m_pArith->l_test(F527T) ){
//		kani |= 0x01;
////		m_typeCnt++;
//	}
//	if( m_pArith->l_test(F528T) ){
//		kani |= 0x02;
////		m_typeCnt++; 
//	}
//	if( m_pArith->l_test(F529T) ){
//		kani |= 0x04;
////		m_typeCnt++;
//	}
//	if( m_pArith->l_test(F5210T) ){
//		kani |= 0x08;
////		m_typeCnt++; 
//	}
//	if( m_pArith->l_test(F5211T) ){
//		kani |= 0x10;
////		m_typeCnt++;
//	}
//---------------
	if( m_pArith->l_test(F527A) || m_pArith->l_test(F527B) || m_pArith->l_test(F527C) ){
		kani |= 0x01;
	}
	if( m_pArith->l_test(F528A) || m_pArith->l_test(F528B) || m_pArith->l_test(F528C) ){
		kani |= 0x02;
	}
	if( m_pArith->l_test(F529A) || m_pArith->l_test(F529B) || m_pArith->l_test(F529C) ){
		kani |= 0x04;
	}
	if( m_pArith->l_test(F5210A) || m_pArith->l_test(F5210B) || m_pArith->l_test(F5210C) ){
		kani |= 0x08;
	}
	if( m_pArith->l_test(F5211A) || m_pArith->l_test(F5211B) || m_pArith->l_test(F5211C) ){
		kani |= 0x10;
	}
//---------------

	char	buf[512]={0};
	char	WORK0[MONY_BUF_SIZE]={0};
	char	WORK1[MONY_BUF_SIZE]={0};
	if( m_typeCnt == 1 ){
		switch( kani&0xff ){
			case 0x01 :
				per = (*m_pSnHeadData)->SVsper[0];
				break;
			case 0x02 :
				per = (*m_pSnHeadData)->SVsper[1];
				break;
			case 0x04 :
				per = (*m_pSnHeadData)->SVsper[2];
				break;
			case 0x08 :
				per = (*m_pSnHeadData)->SVsper[3];
				break;
			case 0x10 :
				per = (*m_pSnHeadData)->SVsper[4];
				break;
			default   :
				per = 0;
				break;
		}
		sprintf_s( buf, sizeof(buf), _T("%d"), (1000-per) );
		memset( WORK1, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( WORK1, buf );
		memset( WORK0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( WORK0, _T("1000") );
		
//		if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
		memset( F525A, '\0', MONY_BUF_SIZE );
		m_Util.l_6calq( F525A, F524A, WORK1, WORK0 );
		memset( F525B, '\0', MONY_BUF_SIZE );
		m_Util.l_6calq( F525B, F524B, WORK1, WORK0 );
		memset( F525C, '\0', MONY_BUF_SIZE );
		m_Util.l_6calq( F525C, F524C, WORK1, WORK0 );
		memset( F525T, '\0', MONY_BUF_SIZE );
//		}

/*- '14.03.03 -*/
//		m_Util.l_6calq( F525T, F524T, WORK1, WORK0 );
/*-------------*/
//--- '15.01.28 ---
//		m_pArith->l_add( F525T, F525A, F525B );
//		m_pArith->l_add( F525T, F525T, F525C );
//-----------------
		if( judge_skj_calq() ){
			m_Util.l_6calq( F525T, F524T, WORK1, WORK0 );
		}
		else{
			m_pArith->l_add( F525T, F525A, F525B );
			m_pArith->l_add( F525T, F525T, F525C );
		}
//-----------------
/*-------------*/
		m_CQsw |= 0x01;
		m_Szsw = m_SBsw = 0x00;

		memset( F5218T, '\0', MONY_BUF_SIZE );
		memset( F5218A, '\0', MONY_BUF_SIZE );
		memset( F5218B, '\0', MONY_BUF_SIZE );
		memset( F5218C, '\0', MONY_BUF_SIZE );
		memset( F5219T, '\0', MONY_BUF_SIZE );
		memset( F5219A, '\0', MONY_BUF_SIZE );
		memset( F5219B, '\0', MONY_BUF_SIZE );
		memset( F5219C, '\0', MONY_BUF_SIZE );
		F5218c = 0;
		F5219c = 0;

//--> '14.06.19 INS START
		m_dspSkjSw = 0;
//<-- '14.06.19 INS END
//--> '14.06.27 INS START
		(*m_pSnHeadData)->Sn_TOKUR = 0;
		F5220c = 0;
		F5221c = 0;
		F5222c = 0;
		F5223c = 0;
		F5224c = 0;
		F5225c = 0;
		F5226c = 0;
		F5227c = 0;
		F5228c = 0;
		F5229c = 0;
//<-- '14.06.27 INS END
	}
	else{
		memset( F525A, '\0', MONY_BUF_SIZE );
		memset( F525B, '\0', MONY_BUF_SIZE );
		memset( F525C, '\0', MONY_BUF_SIZE );
		memset( F525T, '\0', MONY_BUF_SIZE );
		m_CQsw &= 0xf0;
		if( m_typeCnt == 0 ){
			memset( F526T,  '\0', MONY_BUF_SIZE );
			memset( F526A,  '\0', MONY_BUF_SIZE );
			memset( F526B,  '\0', MONY_BUF_SIZE );
			memset( F526C,  '\0', MONY_BUF_SIZE );
			memset( F527T,  '\0', MONY_BUF_SIZE );
			memset( F527A,  '\0', MONY_BUF_SIZE );
			memset( F527B,  '\0', MONY_BUF_SIZE );
			memset( F527C,  '\0', MONY_BUF_SIZE );
			memset( F528T,  '\0', MONY_BUF_SIZE );
			memset( F528A,  '\0', MONY_BUF_SIZE );
			memset( F528B,  '\0', MONY_BUF_SIZE );
			memset( F528C,  '\0', MONY_BUF_SIZE );	
			memset( F529T,  '\0', MONY_BUF_SIZE );	
			memset( F529A,  '\0', MONY_BUF_SIZE );
			memset( F529B,  '\0', MONY_BUF_SIZE );
			memset( F529C,  '\0', MONY_BUF_SIZE );	
			memset( F5210T, '\0', MONY_BUF_SIZE );	
			memset( F5210A, '\0', MONY_BUF_SIZE );
			memset( F5210B, '\0', MONY_BUF_SIZE );
			memset( F5210C, '\0', MONY_BUF_SIZE );		
			memset( F5211T, '\0', MONY_BUF_SIZE );	
			memset( F5211A, '\0', MONY_BUF_SIZE );
			memset( F5211B, '\0', MONY_BUF_SIZE );
			memset( F5211C, '\0', MONY_BUF_SIZE );			
			memset( F5212T, '\0', MONY_BUF_SIZE );	
			memset( F5212A, '\0', MONY_BUF_SIZE );
			memset( F5212B, '\0', MONY_BUF_SIZE );
			memset( F5212C, '\0', MONY_BUF_SIZE );		
			memset( F5213T, '\0', MONY_BUF_SIZE );	
			memset( F5213A, '\0', MONY_BUF_SIZE );
			memset( F5213B, '\0', MONY_BUF_SIZE );
			memset( F5213C, '\0', MONY_BUF_SIZE );
			memset( F5214T, '\0', MONY_BUF_SIZE );	
			memset( F5214A, '\0', MONY_BUF_SIZE );
			memset( F5214B, '\0', MONY_BUF_SIZE );
			memset( F5214C, '\0', MONY_BUF_SIZE );
			memset( F5215T, '\0', MONY_BUF_SIZE );	
			memset( F5215A, '\0', MONY_BUF_SIZE );
			memset( F5215B, '\0', MONY_BUF_SIZE );
			memset( F5215C, '\0', MONY_BUF_SIZE );
			memset( F5216T, '\0', MONY_BUF_SIZE );	
			memset( F5216A, '\0', MONY_BUF_SIZE );
			memset( F5216B, '\0', MONY_BUF_SIZE );
			memset( F5216C, '\0', MONY_BUF_SIZE );
			memset( F5217T, '\0', MONY_BUF_SIZE );	
			memset( F5217A, '\0', MONY_BUF_SIZE );
			memset( F5217B, '\0', MONY_BUF_SIZE );
			memset( F5217C, '\0', MONY_BUF_SIZE );		
			memset( F5218T, '\0', MONY_BUF_SIZE );	
			memset( F5218A, '\0', MONY_BUF_SIZE );
			memset( F5218B, '\0', MONY_BUF_SIZE );
			memset( F5218C, '\0', MONY_BUF_SIZE );
			memset( F5219T, '\0', MONY_BUF_SIZE );	
			memset( F5219A, '\0', MONY_BUF_SIZE );
			memset( F5219B, '\0', MONY_BUF_SIZE );
			memset( F5219C, '\0', MONY_BUF_SIZE );
//--> '14.06.28 INS START
			memset( F5220T, '\0', MONY_BUF_SIZE );	
			memset( F5220A, '\0', MONY_BUF_SIZE );
			memset( F5220B, '\0', MONY_BUF_SIZE );
			memset( F5220C, '\0', MONY_BUF_SIZE );
			F5220c = 0;
			memset( F5221T, '\0', MONY_BUF_SIZE );	
			memset( F5221A, '\0', MONY_BUF_SIZE );
			memset( F5221B, '\0', MONY_BUF_SIZE );
			memset( F5221C, '\0', MONY_BUF_SIZE );
			F5221c = 0;
			memset( F5222T, '\0', MONY_BUF_SIZE );	
			memset( F5222A, '\0', MONY_BUF_SIZE );
			memset( F5222B, '\0', MONY_BUF_SIZE );
			memset( F5222C, '\0', MONY_BUF_SIZE );
			F5222c = 0;
			memset( F5223T, '\0', MONY_BUF_SIZE );	
			memset( F5223A, '\0', MONY_BUF_SIZE );
			memset( F5223B, '\0', MONY_BUF_SIZE );
			memset( F5223C, '\0', MONY_BUF_SIZE );
			F5223c = 0;
			memset( F5224T, '\0', MONY_BUF_SIZE );	
			memset( F5224A, '\0', MONY_BUF_SIZE );
			memset( F5224B, '\0', MONY_BUF_SIZE );
			memset( F5224C, '\0', MONY_BUF_SIZE );
			F5224c = 0;
			memset( F5225T, '\0', MONY_BUF_SIZE );	
			memset( F5225A, '\0', MONY_BUF_SIZE );
			memset( F5225B, '\0', MONY_BUF_SIZE );
			memset( F5225C, '\0', MONY_BUF_SIZE );
			F5225c = 0;
			memset( F5226T, '\0', MONY_BUF_SIZE );	
			memset( F5226A, '\0', MONY_BUF_SIZE );
			memset( F5226B, '\0', MONY_BUF_SIZE );
			memset( F5226C, '\0', MONY_BUF_SIZE );
			F5226c = 0;
			memset( F5227T, '\0', MONY_BUF_SIZE );	
			memset( F5227A, '\0', MONY_BUF_SIZE );
			memset( F5227B, '\0', MONY_BUF_SIZE );
			memset( F5227C, '\0', MONY_BUF_SIZE );
			F5227c = 0;
			memset( F5228T, '\0', MONY_BUF_SIZE );	
			memset( F5228A, '\0', MONY_BUF_SIZE );
			memset( F5228B, '\0', MONY_BUF_SIZE );
			memset( F5228C, '\0', MONY_BUF_SIZE );
			F5228c = 0;
			memset( F5229T, '\0', MONY_BUF_SIZE );	
			memset( F5229A, '\0', MONY_BUF_SIZE );
			memset( F5229B, '\0', MONY_BUF_SIZE );
			memset( F5229C, '\0', MONY_BUF_SIZE );
			F5229c = 0;
			memset( F5230T, '\0', MONY_BUF_SIZE );	
			memset( F5230A, '\0', MONY_BUF_SIZE );
			memset( F5230B, '\0', MONY_BUF_SIZE );
			memset( F5230C, '\0', MONY_BUF_SIZE );
//<-- '14.06.28 INS END

			if( F520c ){
				// 表示クリア
				ChangeColor( IDC_ICSDIAGCTRL2,  ID526A, 0 );
				ChangeColor( IDC_ICSDIAGCTRL2,  ID526B, 0 );
				ChangeColor( IDC_ICSDIAGCTRL2,  ID526C, 0 );
				F520c = 0;
			}
//--> '14.06.19 INS START
			m_dspSkjSw = 0;
//<-- '14.06.19 INS END
		}
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID525T, F525T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID525A, F525A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID525B, F525B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID525C, F525C );
}

//-----------------------------------------------------------------------------
// 入力直後の特例計算サイン
//-----------------------------------------------------------------------------
void CShinFhyo52::Tokurei52()
{
	/*shimizu　delete
	if( m_pArith->l_cmp(F5218T, F5219T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
	}
	else{
		(*m_pSnHeadData)->Sn_TOKUR = 0;
	}
	*/

//--> '14.06.19 INS START
	if( m_typeCnt < 2 ){
		(*m_pSnHeadData)->Sn_TOKUR = 0;
		return;
	}
//<-- '14.06.19 INS END

	if( m_pArith->l_cmp(F5218T, F5219T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5220T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5221T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5222T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5223T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5224T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5225T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5226T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5227T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5228T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}
	if( m_pArith->l_cmp(F5218T, F5229T) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		return;
	}

	(*m_pSnHeadData)->Sn_TOKUR = 0;
}

//-----------------------------------------------------------------------------
// 他表からの計算
//-----------------------------------------------------------------------------
void CShinFhyo52::calq_by_OtherChange(int sw)
{
	m_DpSw = -1;
	if( sw ){
		ReadData( 1 );
	}
	f52calq_all( 1 );
	if( m_typeCnt > 1 ){	// 複数事業種
		// 売上区分チェック
		f52_urikbn();
		// 原則計算を適用する場合
		f52calq_gn();
		// 特例計算を適用する場合
/*- '14.03.11 -*/
//		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//			f52calq_tk_all();
//		}
//		else{
//			f52calq_tk();
//		}
/*-------------*/
		f52calq_tk_all();
/*-------------*/
	}
	WriteData();
	m_DpSw = 0;
}

//-----------------------------------------------------------------------------
// 売上構成比の計算
//-----------------------------------------------------------------------------
// 引数	ssg	：
//-----------------------------------------------------------------------------
// 返送値	1	：	変更あり
//			0	：	変更なし
//-----------------------------------------------------------------------------
//int CShinFhy52::uwariai( char ssg )
int CShinFhyo52::calq_CfrOfSales( char ssg )
{
	char	m1[MONY_BUF_SIZE]={0}, m2[MONY_BUF_SIZE]={0}, m3[MONY_BUF_SIZE]={0}, m4[MONY_BUF_SIZE]={0}, m5[MONY_BUF_SIZE]={0}, tt[MONY_BUF_SIZE]={0};
	short	ps1=0, ps2=0, ps3=0, ps4=0, ps5=0, st=0;

	// 倍長指定
//	l_defn( 0x16 );

	// 第１種事業課税売上
	memmove( m1, F527T, MONY_BUF_SIZE );

	// 第２種事業課税売上
	memmove( m2, F528T, MONY_BUF_SIZE );

	// 第３種事業課税売上
	memmove( m3, F529T, MONY_BUF_SIZE );

	// 第４種事業課税売上
	memmove( m4, F5210T, MONY_BUF_SIZE );
	
	// 第５種事業課税売上
	memmove( m5, F5211T, MONY_BUF_SIZE );
	
//	if( ssg )	memmove( tt, F526B, MONY_BUF_SIZE );
//	else		memmove( tt, F526T, MONY_BUF_SIZE );
	memmove( tt, F526T, MONY_BUF_SIZE );
	
	if ( m_pArith->l_test(tt) == 0 ){
		if( (*m_pSnHonpyoData)->Sn_UP1 || (*m_pSnHonpyoData)->Sn_UP2 || (*m_pSnHonpyoData)->Sn_UP3 || (*m_pSnHonpyoData)->Sn_UP4 || (*m_pSnHonpyoData)->Sn_UP5 ){
			st = 1;
		}
		else{
			st = 0;
		}
		(*m_pSnHonpyoData)->Sn_UP1 = (*m_pSnHonpyoData)->Sn_UP2 = (*m_pSnHonpyoData)->Sn_UP3 = 0;
		(*m_pSnHonpyoData)->Sn_UP4 = (*m_pSnHonpyoData)->Sn_UP5 = 0;

		return st;
	}
	
	ps1 = (short) upersnt( m1, tt );
	ps2 = (short) upersnt( m2, tt );
	ps3 = (short) upersnt( m3, tt );
	ps4 = (short) upersnt( m4, tt );
	ps5 = (short) upersnt( m5, tt );
	
	st = 0;
	if( ps1 != (*m_pSnHonpyoData)->Sn_UP1 ){
		(*m_pSnHonpyoData)->Sn_UP1 = ps1;
		st = 1;
	}
	if( ps2 != (*m_pSnHonpyoData)->Sn_UP2 ){
		(*m_pSnHonpyoData)->Sn_UP2 = ps2;
		st = 1;
	}
	if( ps3 != (*m_pSnHonpyoData)->Sn_UP3 ){
		(*m_pSnHonpyoData)->Sn_UP3 = ps3;
		st = 1;
	}
	if( ps4 != (*m_pSnHonpyoData)->Sn_UP4 ){
		(*m_pSnHonpyoData)->Sn_UP4 = ps4;
		st = 1;
	}
	if( ps5 != (*m_pSnHonpyoData)->Sn_UP5 ){
		(*m_pSnHonpyoData)->Sn_UP5 = ps5;
		st = 1;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 構成比の計算
//-----------------------------------------------------------------------------
// 引数	mm	：
//		tt	：
//-----------------------------------------------------------------------------
// 返送値	：	構成比
//-----------------------------------------------------------------------------
int CShinFhyo52::upersnt( char *mm, char *tt )
{
	char	w1[MONY_BUF_SIZE]={0};
	char	buf[64]={0};

	m_pArith->l_par100( (unsigned char *)w1, (unsigned char *)mm, (unsigned char *)tt, 1, 0 );
//--> '14.10.07 INS START
	char	bif100[MONY_BUF_SIZE]={0};
	m_pArith->l_input( bif100, _T("1000") );
	if( m_pArith->l_cmp(w1, bif100) > 0 ){
		memmove( w1, bif100, sizeof(bif100) );
	}
//<-- '14.10.07 INS END
	m_pArith->l_print( buf, w1, "9999" );
	return atoi(buf);
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算（原則計算を適用する場合）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_gn()
{
//-- '15.02.23 --
//	char	WORK0[MONY_BUF_SIZE]={0};
//---------------
	char	WORK0[64]={0};
//---------------
	char	WORK1[MONY_BUF_SIZE]={0};
	char	PW0[MONY_BUF_SIZE], PW1[MONY_BUF_SIZE], PW2[MONY_BUF_SIZE], val[MONY_BUF_SIZE];

	if( !(m_CQsw&0xff) ){

		char	*pVal1;
		memset( PW1, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW1, _T("1000") );
		int		chkFlg=0x08;

//--> '14.10.17 INS START
		int	clqmax = 3;
		if( judge_skj_calq() ){
			clqmax = 4;
		}
//<-- '14.10.17 INS END

//-- '14.10.17 --
//		for( int i=0; i<3; i++, chkFlg<<=4 ){
//---------------
		for( int i=0; i<clqmax; i++, chkFlg<<=4 ){
//---------------
			memset( val, '\0', MONY_BUF_SIZE );
			memset( PW2, '\0', MONY_BUF_SIZE );
			memset( WORK1, '\0', MONY_BUF_SIZE );

			// 第一種事業の計算
			if( i==0 ){
				pVal1 = F5213A;
			}
			else if( i== 1 ){
				pVal1 = F5213B;
			}
			else if( i== 2 ){
				pVal1 = F5213C;
			}
//--> '14.10.17 INS START
			else if( i== 3 ){
				pVal1 = F5213T;
			}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//			if( m_pArith->l_test(pVal1) ){
//				memmove( WORK1, pVal1, MONY_BUF_SIZE );
//			}
//---------------
			if( m_pArith->l_test(pVal1) > 0 ){
				memmove( WORK1, pVal1, MONY_BUF_SIZE );
			}
			else{
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
//---------------
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), ( 1000 - (*m_pSnHeadData)->SVsper[0] ) );
			memset( PW0, '\0', MONY_BUF_SIZE );
			m_pArith->l_input( PW0, WORK0 );
			if( m_pArith->l_test(WORK1) ){
				m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );
				m_pArith->l_add( val, val, WORK1 );
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
			// 第二種事業の計算
			if( i==0 ){
				pVal1 = F5214A;
			}
			else if( i== 1 ){
				pVal1 = F5214B;
			}
			else if( i== 2 ){
				pVal1 = F5214C;
			}
//--> '14.10.17 INS START
			else if( i== 3 ){
				pVal1 = F5214T;
			}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//			if( m_pArith->l_test(pVal1) ){
//				memmove( WORK1, pVal1, MONY_BUF_SIZE );
//			}
//---------------
			if( m_pArith->l_test(pVal1) > 0 ){
				memmove( WORK1, pVal1, MONY_BUF_SIZE );
			}
			else{
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
//---------------
			sprintf_s( WORK0, sizeof(WORK0), _T("%d"), ( 1000 - (*m_pSnHeadData)->SVsper[1] ) );
			memset( PW0, '\0', MONY_BUF_SIZE );
			m_pArith->l_input( PW0, WORK0 );
			if( m_pArith->l_test(WORK1) ){
				m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );
				m_pArith->l_add( val, val, WORK1 );
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
			// 第三種事業の計算
			if( i==0 ){
				pVal1 = F5215A;
			}
			else if( i== 1 ){
				pVal1 = F5215B;
			}
			else if( i== 2 ){
				pVal1 = F5215C;
			}
//--> '14.10.17 INS START
			else if( i== 3 ){
				pVal1 = F5215T;
			}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//			if( m_pArith->l_test(pVal1) ){
//				memmove( WORK1, pVal1, MONY_BUF_SIZE );
//			}
//---------------
			if( m_pArith->l_test(pVal1) > 0 ){
				memmove( WORK1, pVal1, MONY_BUF_SIZE );
			}
			else{
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
//---------------
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), ( 1000 - (*m_pSnHeadData)->SVsper[2] ) );
			memset( PW0, '\0', MONY_BUF_SIZE );
			m_pArith->l_input( PW0, WORK0 );
			if( m_pArith->l_test(WORK1) ){
				m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );
				m_pArith->l_add( val, val, WORK1 );
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
			// 第四種事業の計算
			if( i==0 ){
				pVal1 = F5216A;
			}
			else if( i== 1 ){
				pVal1 = F5216B;
			}
			else if( i== 2 ){
				pVal1 = F5216C;
			}
//--> '14.10.17 INS START
			else if( i== 3 ){
				pVal1 = F5216T;
			}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//			if( m_pArith->l_test(pVal1) ){
//				memmove( WORK1, pVal1, MONY_BUF_SIZE );
//			}
//---------------
			if( m_pArith->l_test(pVal1) > 0 ){
				memmove( WORK1, pVal1, MONY_BUF_SIZE );
			}
			else{
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
//---------------
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), ( 1000 - (*m_pSnHeadData)->SVsper[3] ) );
			memset( PW0, '\0', MONY_BUF_SIZE );
			m_pArith->l_input( PW0, WORK0 );
			if( m_pArith->l_test(WORK1) ){
				m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );
				m_pArith->l_add( val, val, WORK1 );
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
			// 第五種事業の計算
			if( i==0 ){
				pVal1 = F5217A;
			}
			else if( i== 1 ){
				pVal1 = F5217B;
			}
			else if( i== 2 ){
				pVal1 = F5217C;
			}
//--> '14.10.17 INS START
			else if( i== 3 ){
				pVal1 = F5217T;
			}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//			if( m_pArith->l_test(pVal1) ){
//				memmove( WORK1, pVal1, MONY_BUF_SIZE );
//			}
//---------------
			if( m_pArith->l_test(pVal1) > 0 ){
				memmove( WORK1, pVal1, MONY_BUF_SIZE );
			}
			else{
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}
//---------------
			sprintf_s( WORK0, sizeof(WORK0), _T("%d"), ( 1000 - (*m_pSnHeadData)->SVsper[4] ) );
			memset( PW0, '\0', MONY_BUF_SIZE );
			m_pArith->l_input( PW0, WORK0 );
			if( m_pArith->l_test(WORK1) ){
				m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );
				m_pArith->l_add( val, val, WORK1 );
				memset( WORK1, '\0', MONY_BUF_SIZE );
			}

			char	*pValBunshi, *pValBunbo, *pValDst;
			if( i==0 ){
				pValBunshi = F524A;
				pValBunbo = F5212A;
				pValDst = F5218A;
			}
			else if( i==1 ){
				pValBunshi = F524B;
				pValBunbo = F5212B;
				pValDst = F5218B;
			}
			else if( i==2 ){
				pValBunshi = F524C;
				pValBunbo = F5212C;
				pValDst = F5218C;
			}
//--> '14.10.17 INS START
			else if( i== 3 ){
				pValBunshi = F524T;
				pValBunbo = F5212T;
				pValDst = F5218T;
			}
//<-- '14.10.17 INS END

//-- '15.02.03 --
//			if( m_pArith->l_test(val) && m_pArith->l_test(F5212T) ){
//---------------
			if( m_pArith->l_test(val) &&
				(((judge_skj_calq()==0)&&m_pArith->l_test(F5212T)) || ((judge_skj_calq()==1)&&m_pArith->l_test(pValBunbo))) ){
//---------------

/*- '14.03.11 -*/
//				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)val,  (unsigned char *)pValBunbo, 7, 0 );
//				m_pArith->l_mul100( (unsigned char *)WORK1, (unsigned char *)pValBunshi, (unsigned char *)WORK0, 7, 0 );
/*-------------*/
				f52calq_skj_gn( WORK1, i );
/*-------------*/

				// 原則手入力
				if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
					if( !(F5218c&chkFlg) ){
						memmove( pValDst, WORK1, MONY_BUF_SIZE );
					}
					else{
/*- '14.03.02 -*/
//						if( F5218c & chkFlg ){
//							if( !m_pArith->l_cmp(pValDst, WORK1) ){
//	//							ChangeColor( IDC_ICSDIAGCTRL2, ID5218T, 0 );
//								F5218c &= ~chkFlg;
//							}
//						}
/*--------------*/
					}
				}
			}
			else if( m_pArith->l_test(val) == 0 ){
				if( !(F5218c&chkFlg) ){
					memset( pValDst, '\0', MONY_BUF_SIZE );
				}
			}
		}

		// 横計
//-- '14.10.17 --
//		m_pArith->l_add( F5218T, F5218A, F5218B );
//		m_pArith->l_add( F5218T, F5218T, F5218C );
//---------------
		if( judge_skj_calq() == 0 ){
			m_pArith->l_add( F5218T, F5218A, F5218B );
			m_pArith->l_add( F5218T, F5218T, F5218C );
		}
//---------------

//		dsp_prs( IDC_ICSDIAGCTRL2, ID5218T, F5218T );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID5218B, F5218B );
	}
	if( !(m_CQsw&0x0f) ) {
//		dsp_prs( IDC_ICSDIAGCTRL2, ID5218T, F5218T );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID5218B, F5218B );
	}
	else {
//		memset( F5218T, '\0', MONY_BUF_SIZE );
//		memset( F5218B, '\0', MONY_BUF_SIZE );
/*- '14.03.02 -*/
//		F5218c = m_Szsw = m_SBsw = 0x00;
/*-------------*/
		m_Szsw = m_SBsw = 0x00;
/*-------------*/
//		dsp_prs( IDC_ICSDIAGCTRL2, ID5218T, F5218T );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID5218B, F5218B );
	}
}

#ifdef _20140219_EXTEND_
//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算（特例計算を適用する場合）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_tk()
{
	int				i;
	unsigned short	szsg;	// □□□□ □□□□ □□□□ ■■■■
							//  D0：第１種
							//  D1：第２種
							//  D2：第３種
							//  D3：第４種
	unsigned short	per1, per2;


	char			WORK0[MONY_BUF_SIZE]={0};
	char			WORK1[MONY_BUF_SIZE]={0};
	char			PW0[MONY_BUF_SIZE]={0}, val1[MONY_BUF_SIZE]={0}, val2[MONY_BUF_SIZE]={0}, *VAL=NULL;

	if( !(m_CQsw&0xff) ){
		m_Szsw = 1;
		szsg = per1 = per2 = 0;
		memset( F525T,  '\0', MONY_BUF_SIZE );
		memset( F525B,  '\0', MONY_BUF_SIZE );
		memset( F5219T, '\0', MONY_BUF_SIZE );
		memset( F5219B, '\0', MONY_BUF_SIZE );


		if( (*m_pSnHonpyoData)->Sn_UP1 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[0];
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0008;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP2 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[1];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0040;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP3 >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[2];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0100;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP4 >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[3];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0200;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP5 >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[4];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0008;
			}
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0040;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0100;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0200;
			}
		}
		else {
			m_Szsw = 0;
			if( m_typeCnt <= 2 )	goto CALQB;
#ifdef	CUT			
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP2) >= 750 )	szsg |= 0x0001;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP3) >= 750 )	szsg |= 0x0002;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0004;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0008;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP3) >= 750 )	szsg |= 0x0010;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0020;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0040;
			if( (pSyzShin->Snd.Sn_UP3 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0080;
			if( (pSyzShin->Snd.Sn_UP3 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0100;
			if( (pSyzShin->Snd.Sn_UP4 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0200;
#endif
			// 簡易課税　2種類以上の業種での特例計算チェック
			if( f52_toku75(F526T, F527T,  F528T) ){
				szsg |= 0x0001;
			}
			if( f52_toku75(F526T, F527T,  F529T) ){
				szsg |= 0x0002;
			}
			if( f52_toku75(F526T, F527T,  F5210T) ){
				szsg |= 0x0004;
			}
			if( f52_toku75(F526T, F527T,  F5211T) ){
				szsg |= 0x0008;
			}
			if( f52_toku75(F526T, F528T,  F529T) ){
				szsg |= 0x0010;
			}
			if( f52_toku75(F526T, F528T,  F5210T) ){
				szsg |= 0x0020;
			}
			if( f52_toku75(F526T, F528T,  F5211T) ){
				szsg |= 0x0040;
			}
			if( f52_toku75(F526T, F529T,  F5210T) ){
				szsg |= 0x0080;
			}
			if( f52_toku75(F526T, F529T,  F5211T) ){
				szsg |= 0x0100;
			}
			if( f52_toku75(F526T, F5210T, F5211T) ){
				szsg |= 0x0200;
			}
		}
		
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, _T("1000") );
		VAL = (char *) &F5219T[0];
		if( m_Szsw && m_pArith->l_test(F524T) ){
//		if( 0 ){
			memset( WORK1, '\0', MONY_BUF_SIZE );
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, WORK0 );
			m_Util.l_6calq( VAL, F524T, WORK1, PW0 );
		}
		for( i=0; i<10; ++i, (szsg >>= 1) ){
			if( szsg & 0x01 ){
				memset( val1, '\0', MONY_BUF_SIZE );
				memset( val2, '\0', MONY_BUF_SIZE );
				memset( WORK1, '\0', MONY_BUF_SIZE );
				switch( i ){
					case 9 :
						memmove( val1, F5216T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[3];
						break;
					case 8 :
					case 7 :
						memmove( val1, F5215T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 6 :
					case 5 :
					case 4 :
						memmove( val1, F5214T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[1];
						break;
					default:	
						memmove( val1, F5213T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[0];
						break;
				}
				switch( i ){
					case 0 :
						per2 = (*m_pSnHeadData)->SVsper[1];
						break;
					case 1 :
					case 4 :
						per2 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 2 :
					case 5 : 
					case 7 :
						per2 = (*m_pSnHeadData)->SVsper[3];
						break;
					default:
						per2 = (*m_pSnHeadData)->SVsper[4];
						break;
				}
				m_pArith->l_sub( val2, F5212T, val1 );
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - per1) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val1) ){
					m_Util.l_6calq( val1, val1, WORK1, PW0 );
				}
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per2) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val2) ){
					m_Util.l_6calq( val2, val2, WORK1, PW0 );
				}
				m_pArith->l_add( val1, val1, val2 );
				
				if( m_pArith->l_test(val1) ){
					m_pArith->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)F5212T, 7, 0 );
					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524T, (unsigned char *)WORK1, 7, 0 );
				}
				else{
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				
				if( m_pArith->l_cmp(VAL, WORK0) < 0 ){
					if( (m_Szsw==1) && (m_typeCnt<=2) ){
//					if( 0 ){
						m_Szsw = 0;
						goto CALQB;
					}
					memmove( VAL, WORK0, MONY_BUF_SIZE );
					m_Szsw = (char) (i + 2);
				}
			}
		}

CALQB:
		m_SBsw = 1;
		szsg = per1 = per2 = 0;
		memset( F5219B, '\0', MONY_BUF_SIZE );
		if( F521UPb >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[0];
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0008;
			}
		}
		else if( F522UPb >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[1];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0040;
			}
		}
		else if( F523UPb >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[2];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0100;
			}
		}
		else if( F524UPb >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[3];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0200;
			}
		}
		else if( F525UPb >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[4];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0008;
			}
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0040;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0100;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0200;
			}
		}
		else {
			m_SBsw = 0;
			if( m_typeCnt <= 2 ){
				goto CALQ;
			}
#ifdef	CUT
			if( (F521UPb + F522UPb) >= 750 )	szsg |= 0x0001;
			if( (F521UPb + F523UPb) >= 750 )	szsg |= 0x0002;
			if( (F521UPb + F524UPb) >= 750 )	szsg |= 0x0004;
			if( (F521UPb + F525UPb) >= 750 )	szsg |= 0x0008;
			if( (F522UPb + F523UPb) >= 750 )	szsg |= 0x0010;
			if( (F522UPb + F524UPb) >= 750 )	szsg |= 0x0020;
			if( (F522UPb + F525UPb) >= 750 )	szsg |= 0x0040;
			if( (F523UPb + F524UPb) >= 750 )	szsg |= 0x0080;
			if( (F523UPb + F525UPb) >= 750 )	szsg |= 0x0100;
			if( (F524UPb + F525UPb) >= 750 )	szsg |= 0x0200;
#endif
			// 簡易課税　2種類以上の業種での特例計算チェック
			if( f52_toku75(F526B, F527B,  F528B) ){
				szsg |= 0x0001;
			}
			if( f52_toku75(F526B, F527B,  F529B ) ){
				szsg |= 0x0002;
			}
			if( f52_toku75(F526B, F527B,  F5210B) ){
				szsg |= 0x0004;
			}
			if( f52_toku75(F526B, F527B,  F5211B) ){
				szsg |= 0x0008;
			}
			if( f52_toku75(F526B, F528B,  F529B ) ){
				szsg |= 0x0010;
			}
			if( f52_toku75(F526B, F528B,  F5210B) ){
				szsg |= 0x0020;
			}
			if( f52_toku75(F526B, F528B,  F5211B) ){
				szsg |= 0x0040;
			}
			if( f52_toku75(F526B, F529B,  F5210B) ){
				szsg |= 0x0080;
			}
			if( f52_toku75(F526B, F529B,  F5211B) ){
				szsg |= 0x0100;
			}
			if( f52_toku75(F526B, F5210B, F5211B) ){
				szsg |= 0x0200;
			}
		}
		
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, _T("1000") );
		if( m_SBsw ){
			memset( WORK1, '\0', MONY_BUF_SIZE );
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, WORK0 );
			if( m_pArith->l_test(F524B) ){
				m_Util.l_6calq( F5219B, F524B, WORK1, PW0 );
			}
		}
		for( i=0; i<10; ++i, (szsg >>= 1) ){
			if( szsg & 0x01 ) {
				memset( val1, '\0', MONY_BUF_SIZE );
				memset( val2, '\0', MONY_BUF_SIZE );
				memset( WORK1, '\0', MONY_BUF_SIZE );
				switch( i ){
					case 9 :
						memmove( val1, F5216B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[3];
						break;
					case 8 :
					case 7 :
						memmove( val1, F5215B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 6 :
					case 5 :
					case 4 :
						memmove( val1, F5214B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[1];
						break;
					default:
						memmove( val1, F5213B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[0];
						break;
				}
				switch( i ){
					case 0 :
						per2 = (*m_pSnHeadData)->SVsper[1];
						break;
					case 1 :
					case 4 :
						per2 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 2 :
					case 5 :
					case 7 :
						per2 = (*m_pSnHeadData)->SVsper[3];
						break;
					default:
						per2 = (*m_pSnHeadData)->SVsper[4];
						break;
				}
				
				m_pArith->l_sub( val2, F5212B, val1 );
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - per1) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val1) ){
					m_Util.l_6calq(val1, val1, WORK1, PW0 );
				}
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - per2) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val2) ){
					m_Util.l_6calq(val2, val2, WORK1, PW0 );
				}
				m_pArith->l_add( val1, val1, val2 );
				
				if( m_pArith->l_test(val1) ){
					m_pArith->l_par100( (unsigned char *)WORK1,(unsigned char *)val1, (unsigned char *)F5212B, 7, 0 );
					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524B, (unsigned char *)WORK1, 7, 0 );
				}
				else{
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F5219B, WORK0) < 0 ){
					if( (m_SBsw == 1) && (m_typeCnt<=2) ){
						m_SBsw = 0;
						goto CALQ;
					}
					memmove( F5219B, WORK0, MONY_BUF_SIZE );
					m_SBsw = (char) (i + 2);
				}
			}
		}
	}
CALQ:
	if( m_pParent ){
		((CH26HyoView*)m_pParent)->DispFhyo522();
	}
}
#else
//-----------------------------------------------------------------------------
// 事業の判定
//-----------------------------------------------------------------------------
int CShinFhyo52::f52_urikbn()
{
	if( m_typeCnt == 1 ){		// 単一事業
		m_tkSw = 0;
		if( m_pArith->l_test(F527T) ){
			m_tkSw |= 0x01;
		}
		else if( m_pArith->l_test(F528T) ){
			m_tkSw |= 0x02;
		}
		else if( m_pArith->l_test(F5210T) ){
			m_tkSw |= 0x04;
		}
		else if( m_pArith->l_test(F5211T) ){
			m_tkSw |= 0x08;
		}
		else if( m_pArith->l_test(F5212T) ){
			m_tkSw |= 0x10;
		}
	}
	else if( m_typeCnt >= 2 ){	// 複数事業
		// 単一事業？
//		m_tkSw &= ~0xffff;
		m_tkSw = 0;
		if( (*m_pSnHonpyoData)->Sn_UP1 >= 750 ){
			m_tkSw |= 0x01;
//--> '14.04.22 INS START
//-- '15.03.10 --
//			if( m_pArith->l_test(F528T) ){
//				m_tkSw |= 0x000100;
//			}
//			if( m_pArith->l_test(F529T) ){
//				m_tkSw |= 0x000200;
//			}
//			if( m_pArith->l_test(F5210T) ){
//				m_tkSw |= 0x000400;
//			}
//			if( m_pArith->l_test(F5211T) ){
//				m_tkSw |= 0x000800;
//			}
//---------------
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F528T) ){
					m_tkSw |= 0x000100;
				}
				if( m_pArith->l_test(F529T) ){
					m_tkSw |= 0x000200;
				}
				if( m_pArith->l_test(F5210T) ){
					m_tkSw |= 0x000400;
				}
				if( m_pArith->l_test(F5211T) ){
					m_tkSw |= 0x000800;
				}
			}
//---------------
//<-- '14.04.22 INS END
		}
		else if( (*m_pSnHonpyoData)->Sn_UP2 >= 750 ){
			m_tkSw |= 0x02;
//--> '14.04.22 INS START
//-- '15.03.10 --
//			if( m_pArith->l_test(F527T) ){
//				m_tkSw |= 0x000100;
//			}
//			if( m_pArith->l_test(F529T) ){
//				m_tkSw |= 0x001000;
//			}
//			if( m_pArith->l_test(F5210T) ){
//				m_tkSw |= 0x002000;
//			}
//			if( m_pArith->l_test(F5211T) ){
//				m_tkSw |= 0x004000;
//			}
//---------------
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F527T) ){
					m_tkSw |= 0x000100;
				}
				if( m_pArith->l_test(F529T) ){
					m_tkSw |= 0x001000;
				}
				if( m_pArith->l_test(F5210T) ){
					m_tkSw |= 0x002000;
				}
				if( m_pArith->l_test(F5211T) ){
					m_tkSw |= 0x004000;
				}
			}
//---------------
//<-- '14.04.22 INS END
		}
		else if( (*m_pSnHonpyoData)->Sn_UP3 >= 750 ){
			m_tkSw |= 0x04;
//--> '14.04.22 INS START
//-- '15.03.10 --
//			if( m_pArith->l_test(F527T) ){
//				m_tkSw |= 0x000200;
//			}
//			if( m_pArith->l_test(F528T) ){
//				m_tkSw |= 0x001000;
//			}
//			if( m_pArith->l_test(F5210T) ){
//				m_tkSw |= 0x008000;
//			}
//			if( m_pArith->l_test(F5211T) ){
//				m_tkSw |= 0x010000;
//			}
//---------------
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F527T) ){
					m_tkSw |= 0x000200;
				}
				if( m_pArith->l_test(F528T) ){
					m_tkSw |= 0x001000;
				}
				if( m_pArith->l_test(F5210T) ){
					m_tkSw |= 0x008000;
				}
				if( m_pArith->l_test(F5211T) ){
					m_tkSw |= 0x010000;
				}
			}
//---------------
//<-- '14.04.22 INS END
		}
		else if( (*m_pSnHonpyoData)->Sn_UP4 >= 750 ){
			m_tkSw |= 0x08;
//--> '14.04.22 INS START
//-- '15.03.10 --
//			if( m_pArith->l_test(F527T) ){
//				m_tkSw |= 0x000400;
//			}
//			if( m_pArith->l_test(F528T) ){
///*- '14.05.21 -*/
////				m_tkSw |= 0x001000;
///*-------------*/
//				m_tkSw |= 0x002000;
///*-------------*/
//			}
//			if( m_pArith->l_test(F529T) ){
//				m_tkSw |= 0x008000;
//			}
//			if( m_pArith->l_test(F5211T) ){
//				m_tkSw |= 0x010000;
//			}
//---------------
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F527T) ){
					m_tkSw |= 0x000400;
				}
				if( m_pArith->l_test(F528T) ){
				m_tkSw |= 0x002000;
				}
				if( m_pArith->l_test(F529T) ){
					m_tkSw |= 0x008000;
				}
				if( m_pArith->l_test(F5211T) ){
					m_tkSw |= 0x010000;
				}
			}
//---------------
//<-- '14.04.22 INS END
		}
		else if( (*m_pSnHonpyoData)->Sn_UP5 >= 750 ){
			m_tkSw |= 0x10;
//--> '14.04.22 INS START
//-- '15.03.10 --
//			if( m_pArith->l_test(F527T) ){
//				m_tkSw |= 0x000800;
//			}
//			if( m_pArith->l_test(F528T) ){
//				m_tkSw |= 0x004000;
//			}
//			if( m_pArith->l_test(F529T) ){
//				m_tkSw |= 0x010000;
//			}
//			if( m_pArith->l_test(F5210T) ){
//				m_tkSw |= 0x020000;
//			}
//---------------
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F527T) ){
					m_tkSw |= 0x000800;
				}
				if( m_pArith->l_test(F528T) ){
					m_tkSw |= 0x004000;
				}
				if( m_pArith->l_test(F529T) ){
					m_tkSw |= 0x010000;
				}
				if( m_pArith->l_test(F5210T) ){
					m_tkSw |= 0x020000;
				}
			}
//---------------
//<-- '14.04.22 INS END
		}

//-- '15.03.10 --
//		if( !(m_tkSw&0xff) ){	// 複数事業
//---------------
		if( !(m_tkSw&0xff) && (m_typeCnt>2) ){	// 複数事業（３業種以上）
//---------------
			if( f52_toku75(F526T, F527T,  F528T) ){
				m_tkSw |= 0x000100;
			}
			if( f52_toku75(F526T, F527T,  F529T) ){
				m_tkSw |= 0x000200;
			}
			if( f52_toku75(F526T, F527T,  F5210T) ){
				m_tkSw |= 0x000400;
			}
			if( f52_toku75(F526T, F527T,  F5211T) ){
				m_tkSw |= 0x000800;
			}
			if( f52_toku75(F526T, F528T,  F529T) ){
				m_tkSw |= 0x001000;
			}
			if( f52_toku75(F526T, F528T,  F5210T) ){
				m_tkSw |= 0x002000;
			}
			if( f52_toku75(F526T, F528T,  F5211T) ){
				m_tkSw |= 0x004000;
			}
			if( f52_toku75(F526T, F529T,  F5210T) ){
				m_tkSw |= 0x008000;
			}
			if( f52_toku75(F526T, F529T,  F5211T) ){
				m_tkSw |= 0x010000;
			}
			if( f52_toku75(F526T, F5210T, F5211T) ){
				m_tkSw |= 0x020000;
			}
			if( F5220c || F5221c || F5223c || F5224c || F5225c || F5226c || F5227c || F5228c || F5229c){
				if( F5220c && ( m_tkSw & 0x000100 ) ){
					m_tkSw = 0x000100;
				}
				else if( F5221c && ( m_tkSw & 0x000200 )){
					m_tkSw = 0x000200;
				}
				else if( F5222c && ( m_tkSw & 0x000400 )){
					m_tkSw = 0x000400;
				}
				else if( F5223c && ( m_tkSw & 0x000800 )){
					m_tkSw = 0x000800;
				}
				else if( F5224c && ( m_tkSw & 0x001000 )){
					m_tkSw = 0x001000;
				}
				else if( F5225c && ( m_tkSw & 0x002000 )){
					m_tkSw = 0x002000;
				}
				else if( F5226c && ( m_tkSw & 0x004000 )){
					m_tkSw = 0x004000;
				}
				else if( F5227c && ( m_tkSw & 0x008000 )){
					m_tkSw = 0x008000;
				}
				else if( F5228c && ( m_tkSw & 0x010000 )){
					m_tkSw = 0x010000;
				}
				else if( F5229c && ( m_tkSw & 0x020000 )){
					m_tkSw = 0x020000;
				}
			}
		}
//-- '15.03.10 --
//		else{
//---------------
		else if( (m_tkSw&0xff) && (m_typeCnt>1) ){
//---------------
			// 単一事業
			unsigned short	per1=0;
			if( (m_tkSw&0xff) == 0x01 ){
				per1 = (*m_pSnHeadData)->SVsper[0];
			}
			else if( (m_tkSw&0xff) == 0x02 ){
				per1 = (*m_pSnHeadData)->SVsper[1];
			}
			else if( (m_tkSw&0xff) == 0x04 ){
				per1 = (*m_pSnHeadData)->SVsper[2];
			}
			else if( (m_tkSw&0xff) == 0x08 ){
				per1 = (*m_pSnHeadData)->SVsper[3];
			}
			else if( (m_tkSw&0xff) == 0x10 ){
				per1 = (*m_pSnHeadData)->SVsper[4];
			}

//--> '14.10.17 INS START
			int	clqmax = 3;
			if( judge_skj_calq() ){
				clqmax = 4;
			}
//<-- '14.10.17 INS END

			// 単一事業で75%以上
			char	strBuf[64]={0};	// '15.02.23
			char	WORK0[MONY_BUF_SIZE]={0};
			char	WORK1[MONY_BUF_SIZE]={0};
			char	PW0[MONY_BUF_SIZE]={0}, val1[MONY_BUF_SIZE]={0}, val2[MONY_BUF_SIZE]={0};
			char	*pValSrc=NULL, *pValDst=NULL;
			int		chkFlg=0x08;

			m_pArith->l_input( PW0, _T("1000") );
//-- '14.10.17 --
//			for( int i=0; i<3; i++, chkFlg<<=4 ){
//---------------
			for( int i=0; i<clqmax; i++, chkFlg<<=4 ){
//---------------
				if( i==0 ){
					pValDst = F5219A;
					pValSrc = F524A;
				}
				else if( i==1 ){
					pValDst = F5219B;
					pValSrc = F524B;
				}
				else if( i==2 ){
					pValDst = F5219C;
					pValSrc = F524C;
				}
//--> '14.10.17 INS START
				else if( i==3 ){
					pValDst = F5219T;
					pValSrc = F524T;
				}
//<-- '14.10.17 INS END

				if( m_pArith->l_test(pValSrc) ){
					memset( WORK1, '\0', MONY_BUF_SIZE );
//-- '15.02.23 --
//					sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
//					m_pArith->l_input( WORK1, WORK0 );
//---------------
					sprintf_s( strBuf, sizeof( strBuf ), _T("%d"), (1000-per1) );
					m_pArith->l_input( WORK1, strBuf );
//---------------
					if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
						if( !(F5219c&chkFlg) ){
							m_Util.l_6calq( pValDst, pValSrc, WORK1, PW0 );
						}
					}
				}
				else{
					// 元が０円の時には手入力を許さない
					if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
						if( (F5219c&chkFlg) ){
							//F5219c &= ~chkFlg;
						}
						else{
							memset( WORK1, '\0', MONY_BUF_SIZE );
//-- '15.02.23 --
//							sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
//							m_pArith->l_input( WORK1, WORK0 );
//---------------
							sprintf_s( strBuf, sizeof( strBuf ), _T("%d"), (1000-per1) );
							m_pArith->l_input( WORK1, strBuf );
//---------------
							m_Util.l_6calq( pValDst, pValSrc, WORK1, PW0 );
						}
					}
				}
			}

//-- '14.10.17 --
//			// 横計
//			m_pArith->l_add( F5219T, F5219A, F5219B );
//			m_pArith->l_add( F5219T, F5219T, F5219C );
//---------------
			if( clqmax==3 ){
				// 横計
				m_pArith->l_add( F5219T, F5219A, F5219B );
				m_pArith->l_add( F5219T, F5219T, F5219C );
			}
//---------------
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算(原則計算)
//-----------------------------------------------------------------------------
// 引数	dest	：	計算結果
//		ztype	：	税タイプ(0:3%, 1:5%, 2:8%, 3:合計)
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_skj_gn( char *dest, int ztype )
{

	char	*pVal13=NULL, *pVal14=NULL, *pVal15=NULL, *pVal16=NULL, *pVal17=NULL;
	char	*pVal4=NULL, *pVal12=NULL;
	char	zeroVal[MONY_BUF_SIZE] = {0};

	if( ztype == 0 ){
		pVal13 = F5213A;
		pVal14 = F5214A;
		pVal15 = F5215A;
		pVal16 = F5216A;
		pVal17 = F5217A;
		pVal4 = F524A;
		pVal12 = F5212A;
	}
	else if( ztype == 1 ){
		pVal13 = F5213B;
		pVal14 = F5214B;
		pVal15 = F5215B;
		pVal16 = F5216B;
		pVal17 = F5217B;
		pVal4 = F524B;
		pVal12 = F5212B;
	}
	else if( ztype == 2 ){
		pVal13 = F5213C;
		pVal14 = F5214C;
		pVal15 = F5215C;
		pVal16 = F5216C;
		pVal17 = F5217C;
		pVal4 = F524C;
		pVal12 = F5212C;
	}
//--> '14.10.17 INS START
	else if( ztype == 3 ){
		pVal13 = F5213T;
		pVal14 = F5214T;
		pVal15 = F5215T;
		pVal16 = F5216T;
		pVal17 = F5217T;
		pVal4 = F524T;
		pVal12 = F5212T;
	}
//<-- '14.10.17 INS END
	else{
		return;
	}

//--> '14.10.17 INS START
	if( m_pArith->l_test(pVal13) < 0 ){
		pVal13 = zeroVal;
	}
	if( m_pArith->l_test(pVal14) < 0 ){
		pVal14 = zeroVal;
	}
	if( m_pArith->l_test(pVal15) < 0 ){
		pVal15 = zeroVal;
	}
	if( m_pArith->l_test(pVal16) < 0 ){
		pVal16 = zeroVal;
	}
	if( m_pArith->l_test(pVal17) < 0 ){
		pVal17 = zeroVal;
	}
//<-- '14.10.17 INS END

	char	bfVal13[64]={0}, bfVal14[64]={0}, bfVal15[64]={0}, bfVal16[64]={0}, bfVal17[64]={0}, bfVal4[64]={0}, bfVal12[64]={0};
	m_pArith->l_print( bfVal13, pVal13, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal14, pVal14, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal15, pVal15, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal16, pVal16, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal17, pVal17, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal4, pVal4, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal12, pVal12, _T("sssssssssssss9") );

	double	dbVal13, dbVal14, dbVal15, dbVal16, dbVal17, dbVal4, dbVal12;
	dbVal13 = strtod( bfVal13, 0 );
	dbVal14 = strtod( bfVal14, 0 );
	dbVal15 = strtod( bfVal15, 0 );
	dbVal16 = strtod( bfVal16, 0 );
	dbVal17 = strtod( bfVal17, 0 );
	dbVal4 = strtod( bfVal4, 0 );
	dbVal12 = strtod( bfVal12, 0 );

	double	ans;
	if( dbVal12 != 0.0 ){
/*- '14.07.02 -*/
//		ans = dbVal4 * (((dbVal13*0.9) + (dbVal14*0.8) + (dbVal15*0.7) + (dbVal16*0.6) + (dbVal17*0.5)) / dbVal12);
/*-------------*/
//-- '15.02.10 --
//		ans = dbVal4 * ( (((dbVal13*900.0)+(dbVal14*800.0)+(dbVal15*700.0)+(dbVal16*600.0)+(dbVal17*500.0))/1000.0) / dbVal12 );
//---------------
		if( dbVal4 != dbVal12 ){
			ans = dbVal4 * ( (((dbVal13*900.0)+(dbVal14*800.0)+(dbVal15*700.0)+(dbVal16*600.0)+(dbVal17*500.0))/1000.0) / dbVal12 );
		}
		else{
			ans = ((dbVal13*900.0)+(dbVal14*800.0)+(dbVal15*700.0)+(dbVal16*600.0)+(dbVal17*500.0)) / 1000.0;
		}
//---------------
/*-------------*/
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
	m_pArith->l_input( dest, bfAns );
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算(特例計算)
//-----------------------------------------------------------------------------
// 引数	dest	：	計算結果
//		src		：	被乗算数
//		num1	：	分子（被乗算数１）
//		rate1	：	分子（被乗算割合１）
//		num2	：	分子（被乗算数２）
//		rate2	：	分子（被乗算割合２）
//		deno	：	分母
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_skj_tk( char *dest, const char *src, const char *num1, const char *rate1, const char *num2, const char *rate2, const char *deno )
{
	// 今回は、集計モジュールを使用せずに自前で計算
	char	bfSrc[64]={0}, bfNum1[64]={0}, bfRate1[64]={0}, bfNum2[64]={0}, bfRate2[64]={0}, bfDeno[64]={0};
	m_pArith->l_print( bfSrc, (char*)src, _T("sssssssssssss9") );
	m_pArith->l_print( bfNum1, (char*)num1, _T("sssssssssssss9") );
	m_pArith->l_print( bfRate1, (char*)rate1, _T("sssssssssssss9") );
	m_pArith->l_print( bfNum2, (char*)num2, _T("sssssssssssss9") );
	m_pArith->l_print( bfRate2, (char*)rate2, _T("sssssssssssss9") );
	m_pArith->l_print( bfDeno, (char*)deno, _T("sssssssssssss9") );

	double	dbSrc, dbNum1, dbRate1, dbNum2, dbRate2, dbDeno;
	dbSrc = strtod( bfSrc, 0 );
	dbNum1 = strtod( bfNum1, 0 );
	dbRate1 = strtod( bfRate1, 0 );
	dbNum2 = strtod( bfNum2, 0 );
	dbRate2 = strtod( bfRate2, 0 );
	dbDeno = strtod( bfDeno, 0 );

	double	ans;
/*- '14.07.02 -*/
//	ans = dbSrc * ( ((dbNum1*(dbRate1/1000.0)) + (dbNum2*(dbRate2/1000.0))) );
/*-------------*/
	ans = dbSrc * ( ((dbNum1*dbRate1)+(dbNum2*dbRate2)) / 1000.0 );
/*-------------*/
	if( dbDeno != 0.0 ){
		ans = ans / dbDeno;
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
	m_pArith->l_input( dest, bfAns );
}

#ifdef _20140313_CUT_
//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算（特例計算を適用する場合）
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_tk()
{
	int				i;
	unsigned short	szsg;	// □□□□ □□□□ □□□□ ■■■■
							//  D0：第１種
							//  D1：第２種
							//  D2：第３種
							//  D3：第４種
	unsigned short	per1, per2;


	char			WORK0[MONY_BUF_SIZE]={0};
	char			WORK1[MONY_BUF_SIZE]={0};
	char			PW0[MONY_BUF_SIZE]={0}, val1[MONY_BUF_SIZE]={0}, val2[MONY_BUF_SIZE]={0}, *VAL=NULL;

	if( 1 ){
		m_Szsw = 1;
		szsg = per1 = per2 = 0;
//		memset( F525T,  '\0', MONY_BUF_SIZE );
//		memset( F525B,  '\0', MONY_BUF_SIZE );
//		memset( F5219T, '\0', MONY_BUF_SIZE );
//		memset( F5219B, '\0', MONY_BUF_SIZE );

		if( (*m_pSnHonpyoData)->Sn_UP1 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[0];
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0008;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP2 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[1];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0040;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP3 >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[2];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0100;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP4 >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[3];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211T) ){
				szsg |= 0x0200;
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP5 >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[4];
			if( m_pArith->l_test(F527T) ){
				szsg |= 0x0008;
			}
			if( m_pArith->l_test(F528T) ){
				szsg |= 0x0040;
			}
			if( m_pArith->l_test(F529T) ){
				szsg |= 0x0100;
			}
			if( m_pArith->l_test(F5210T) ){
				szsg |= 0x0200;
			}
		}
		else {
			m_Szsw = 0;
			if( m_typeCnt <= 2 )	goto CALQB;
#ifdef	CUT			
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP2) >= 750 )	szsg |= 0x0001;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP3) >= 750 )	szsg |= 0x0002;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0004;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0008;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP3) >= 750 )	szsg |= 0x0010;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0020;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0040;
			if( (pSyzShin->Snd.Sn_UP3 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0080;
			if( (pSyzShin->Snd.Sn_UP3 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0100;
			if( (pSyzShin->Snd.Sn_UP4 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0200;
#endif
			// 簡易課税　2種類以上の業種での特例計算チェック
			if( f52_toku75(F526T, F527T,  F528T) ){
				szsg |= 0x0001;
			}
			if( f52_toku75(F526T, F527T,  F529T) ){
				szsg |= 0x0002;
			}
			if( f52_toku75(F526T, F527T,  F5210T) ){
				szsg |= 0x0004;
			}
			if( f52_toku75(F526T, F527T,  F5211T) ){
				szsg |= 0x0008;
			}
			if( f52_toku75(F526T, F528T,  F529T) ){
				szsg |= 0x0010;
			}
			if( f52_toku75(F526T, F528T,  F5210T) ){
				szsg |= 0x0020;
			}
			if( f52_toku75(F526T, F528T,  F5211T) ){
				szsg |= 0x0040;
			}
			if( f52_toku75(F526T, F529T,  F5210T) ){
				szsg |= 0x0080;
			}
			if( f52_toku75(F526T, F529T,  F5211T) ){
				szsg |= 0x0100;
			}
			if( f52_toku75(F526T, F5210T, F5211T) ){
				szsg |= 0x0200;
			}
		}
		
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, _T("1000") );
		VAL = (char *) &F5219T[0];
		if( m_Szsw && m_pArith->l_test(F524T) ){
//		if( 0 ){
			memset( WORK1, '\0', MONY_BUF_SIZE );
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, WORK0 );
			m_Util.l_6calq( VAL, F524T, WORK1, PW0 );
		}
		for( i=0; i<10; ++i, (szsg >>= 1) ){
			if( szsg & 0x01 ){
				memset( val1, '\0', MONY_BUF_SIZE );
				memset( val2, '\0', MONY_BUF_SIZE );
				memset( WORK1, '\0', MONY_BUF_SIZE );
				switch( i ){
					case 9 :
						memmove( val1, F5216T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[3];
						break;
					case 8 :
					case 7 :
						memmove( val1, F5215T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 6 :
					case 5 :
					case 4 :
						memmove( val1, F5214T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[1];
						break;
					default:	
						memmove( val1, F5213T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[0];
						break;
				}
				switch( i ){
					case 0 :
						per2 = (*m_pSnHeadData)->SVsper[1];
						break;
					case 1 :
					case 4 :
						per2 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 2 :
					case 5 : 
					case 7 :
						per2 = (*m_pSnHeadData)->SVsper[3];
						break;
					default:
						per2 = (*m_pSnHeadData)->SVsper[4];
						break;
				}
				// a:(12 - ??) * X%
				m_pArith->l_sub( val2, F5212T, val1 );
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per1) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val1) ){
					m_Util.l_6calq( val1, val1, WORK1, PW0 );
				}
				// b:?? * X%
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per2) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val2) ){
					m_Util.l_6calq( val2, val2, WORK1, PW0 );
				}
				// c:a+b
				m_pArith->l_add( val1, val1, val2 );
				
				if( m_pArith->l_test(val1) ){
/*- '14.03.11 -*/
					// d:c/12
					m_pArith->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)F5212T, 7, 0 );
					// e:4*d
					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524T, (unsigned char *)WORK1, 7, 0 );
/*-------------*/
					
/*-------------*/
				}
				else{
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				
				if( m_pArith->l_cmp(VAL, WORK0) < 0 ){
					if( (m_Szsw==1) && (m_typeCnt<=2) ){
//					if( 0 ){
						m_Szsw = 0;
						goto CALQB;
					}
					memmove( VAL, WORK0, MONY_BUF_SIZE );
					m_Szsw = (char) (i + 2);
				}
			}
		}

CALQB:
		m_SBsw = 1;
		szsg = per1 = per2 = 0;
		memset( F5219B, '\0', MONY_BUF_SIZE );
		if( F521UPb >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[0];
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0008;
			}
		}
		else if( F522UPb >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[1];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0001;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0040;
			}
		}
		else if( F523UPb >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[2];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0002;
			}
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0010;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0100;
			}
		}
		else if( F524UPb >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[3];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0004;
			}
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0020;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0080;
			}
			if( m_pArith->l_test(F5211B) ){
				szsg |= 0x0200;
			}
		}
		else if( F525UPb >= 750 ) {
			per1 = (*m_pSnHeadData)->SVsper[4];
			if( m_pArith->l_test(F527B) ){
				szsg |= 0x0008;
			}
			if( m_pArith->l_test(F528B) ){
				szsg |= 0x0040;
			}
			if( m_pArith->l_test(F529B) ){
				szsg |= 0x0100;
			}
			if( m_pArith->l_test(F5210B) ){
				szsg |= 0x0200;
			}
		}
		else {
			m_SBsw = 0;
			if( m_typeCnt <= 2 ){
				goto CALQ;
			}
#ifdef	CUT
			if( (F521UPb + F522UPb) >= 750 )	szsg |= 0x0001;
			if( (F521UPb + F523UPb) >= 750 )	szsg |= 0x0002;
			if( (F521UPb + F524UPb) >= 750 )	szsg |= 0x0004;
			if( (F521UPb + F525UPb) >= 750 )	szsg |= 0x0008;
			if( (F522UPb + F523UPb) >= 750 )	szsg |= 0x0010;
			if( (F522UPb + F524UPb) >= 750 )	szsg |= 0x0020;
			if( (F522UPb + F525UPb) >= 750 )	szsg |= 0x0040;
			if( (F523UPb + F524UPb) >= 750 )	szsg |= 0x0080;
			if( (F523UPb + F525UPb) >= 750 )	szsg |= 0x0100;
			if( (F524UPb + F525UPb) >= 750 )	szsg |= 0x0200;
#endif
			// 簡易課税　2種類以上の業種での特例計算チェック
			if( f52_toku75(F526B, F527B,  F528B) ){
				szsg |= 0x0001;
			}
			if( f52_toku75(F526B, F527B,  F529B ) ){
				szsg |= 0x0002;
			}
			if( f52_toku75(F526B, F527B,  F5210B) ){
				szsg |= 0x0004;
			}
			if( f52_toku75(F526B, F527B,  F5211B) ){
				szsg |= 0x0008;
			}
			if( f52_toku75(F526B, F528B,  F529B ) ){
				szsg |= 0x0010;
			}
			if( f52_toku75(F526B, F528B,  F5210B) ){
				szsg |= 0x0020;
			}
			if( f52_toku75(F526B, F528B,  F5211B) ){
				szsg |= 0x0040;
			}
			if( f52_toku75(F526B, F529B,  F5210B) ){
				szsg |= 0x0080;
			}
			if( f52_toku75(F526B, F529B,  F5211B) ){
				szsg |= 0x0100;
			}
			if( f52_toku75(F526B, F5210B, F5211B) ){
				szsg |= 0x0200;
			}
		}
		
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, _T("1000") );
		if( m_SBsw ){
			memset( WORK1, '\0', MONY_BUF_SIZE );
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, WORK0 );
			if( m_pArith->l_test(F524B) ){
				m_Util.l_6calq( F5219B, F524B, WORK1, PW0 );
			}
		}
		for( i=0; i<10; ++i, (szsg >>= 1) ){
			if( szsg & 0x01 ) {
				memset( val1, '\0', MONY_BUF_SIZE );
				memset( val2, '\0', MONY_BUF_SIZE );
				memset( WORK1, '\0', MONY_BUF_SIZE );
				switch( i ){
					case 9 :
						memmove( val1, F5216B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[3];
						break;
					case 8 :
					case 7 :
						memmove( val1, F5215B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 6 :
					case 5 :
					case 4 :
						memmove( val1, F5214B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[1];
						break;
					default:
						memmove( val1, F5213B, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[0];
						break;
				}
				switch( i ){
					case 0 :
						per2 = (*m_pSnHeadData)->SVsper[1];
						break;
					case 1 :
					case 4 :
						per2 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 2 :
					case 5 :
					case 7 :
						per2 = (*m_pSnHeadData)->SVsper[3];
						break;
					default:
						per2 = (*m_pSnHeadData)->SVsper[4];
						break;
				}
				
				m_pArith->l_sub( val2, F5212B, val1 );
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - per1) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val1) ){
					m_Util.l_6calq(val1, val1, WORK1, PW0 );
				}
				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - per2) );
				m_pArith->l_input( WORK1, WORK0 );
				if( m_pArith->l_test(val2) ){
					m_Util.l_6calq(val2, val2, WORK1, PW0 );
				}
				m_pArith->l_add( val1, val1, val2 );
				
				if( m_pArith->l_test(val1) ){
					m_pArith->l_par100( (unsigned char *)WORK1,(unsigned char *)val1, (unsigned char *)F5212B, 7, 0 );
					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524B, (unsigned char *)WORK1, 7, 0 );
				}
				else{
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F5219B, WORK0) < 0 ){
					if( (m_SBsw == 1) && (m_typeCnt<=2) ){
						m_SBsw = 0;
						goto CALQ;
					}
					memmove( F5219B, WORK0, MONY_BUF_SIZE );
					m_SBsw = (char) (i + 2);
				}
			}
		}
	}
CALQ:
	if( m_pParent ){
		((CH26HyoView*)m_pParent)->DispFhyo522();
	}
}
#endif
#endif

//-----------------------------------------------------------------------------
// 簡易課税　2種類以上の業種での特例計算チェック
//-----------------------------------------------------------------------------
// 引数	Base	：	
//		Add1	：	
//		Add2	：	
//-----------------------------------------------------------------------------
// 返送値	1	：	75%以上
//			0	：	75%以上でない
//-----------------------------------------------------------------------------
int CShinFhyo52::f52_toku75( char *Base, char *Add1, char *Add2  )
{
	int		st = 0;

	char	EXPW0[MONY_BUF_SIZE], EXPW1[MONY_BUF_SIZE], KZMY0[MONY_BUF_SIZE], KZMY1[MONY_BUF_SIZE], KZMY2[MONY_BUF_SIZE];

	// 計算準備
	m_pArith->l_input( EXPW0, _T("750") );
	m_pArith->l_input( EXPW1, _T("1000") );

	// 選択された2業種を合計する
	memset( KZMY0, '\0', MONY_BUF_SIZE );
	m_pArith->l_add( KZMY0, Add1, Add2 );

	// 事業区分合計
	memset( KZMY1, '\0', MONY_BUF_SIZE );
	memmove( KZMY1, Base, MONY_BUF_SIZE );

	// 選択された2業種を合計を基に割合計算
	memset( KZMY2, '\0', MONY_BUF_SIZE );
	m_Util.l_6calq( KZMY2, KZMY0, EXPW1, KZMY1 );		// 構成比
	
	// 特例７５％比較
	if( m_pArith->l_cmp(KZMY2, EXPW0) >= 0 ){
		st = 1;	// 75%以上
	}
	else{
		st = 0;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 原則・特例：最有利になる表示位置を取得
//-----------------------------------------------------------------------------
int CShinFhyo52::f52calq_tk_for_disp()
{
	char			val1[MONY_BUF_SIZE], val2[MONY_BUF_SIZE], WORK1[MONY_BUF_SIZE], PW0[MONY_BUF_SIZE], WORK0[MONY_BUF_SIZE];
	char			nume1[MONY_BUF_SIZE], nume2[MONY_BUF_SIZE], rate1[MONY_BUF_SIZE], rate2[MONY_BUF_SIZE];
	char			strBuf[64];	// '15.02.23

	char			val3per[MONY_BUF_SIZE], val5per[MONY_BUF_SIZE], val8per[MONY_BUF_SIZE], valttl[MONY_BUF_SIZE];
	char			max_valttl[MONY_BUF_SIZE];
	char			*pVal;

	int				calqSw = 0;
	int				chkFlg = 0x08;

	unsigned short	per1, per2;

	m_pArith->l_input( PW0, _T("1000") );

	int	maxSign = 0;
	memset( max_valttl, '\0', MONY_BUF_SIZE );
	for( int i=0; i<10; ++i ){

		// 各基準額が75%以上？
		int	tmpchkflg = (0x100<<i);
		if( !(m_tkSw&tmpchkflg) ){
			continue;
		}

		memset( val3per, '\0', MONY_BUF_SIZE );
		memset( val5per, '\0', MONY_BUF_SIZE );
		memset( val8per, '\0', MONY_BUF_SIZE );
		memset( valttl, '\0', MONY_BUF_SIZE );

		// 税率ごと
		for( int j=0; j<3; j++, chkFlg<<=4 ){
			memset( val1, '\0', MONY_BUF_SIZE );
			memset( val2, '\0', MONY_BUF_SIZE );
			memset( WORK0, '\0', MONY_BUF_SIZE );
			memset( WORK1, '\0', MONY_BUF_SIZE );
			memset( nume1, '\0', MONY_BUF_SIZE );	// 分子１
			memset( nume2, '\0', MONY_BUF_SIZE );	// 分子２
			memset( rate1, '\0', MONY_BUF_SIZE );	// 割合
			memset( rate2, '\0', MONY_BUF_SIZE );	// 割合
//--> '15.02.23 INS START
			memset( strBuf, '\0', sizeof(strBuf) );
//<-- '15.02.23 INS END

			switch( i ){
				case 9 :
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5216A, MONY_BUF_SIZE );
//						memmove( nume1, F5216A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5216B, MONY_BUF_SIZE );
//						memmove( nume1, F5216B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5216C, MONY_BUF_SIZE );
//						memmove( nume1, F5216C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5216A) > 0 ){
							memmove( val1, F5216A, MONY_BUF_SIZE );
							memmove( nume1, F5216A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5216B) > 0 ){
							memmove( val1, F5216B, MONY_BUF_SIZE );
							memmove( nume1, F5216B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5216C) > 0 ){
							memmove( val1, F5216C, MONY_BUF_SIZE );
							memmove( nume1, F5216C, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[3];
					break;
				case 8 :
				case 7 :
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5215A, MONY_BUF_SIZE );
//						memmove( nume1, F5215A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5215B, MONY_BUF_SIZE );
//						memmove( nume1, F5215B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5215C, MONY_BUF_SIZE );
//						memmove( nume1, F5215C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5215A) > 0 ){
							memmove( val1, F5215A, MONY_BUF_SIZE );
							memmove( nume1, F5215A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5215B) > 0 ){
							memmove( val1, F5215B, MONY_BUF_SIZE );
							memmove( nume1, F5215B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5215C) > 0 ){
							memmove( val1, F5215C, MONY_BUF_SIZE );
							memmove( nume1, F5215C, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[2];
					break;
				case 6 :
				case 5 :
				case 4 :
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5214A, MONY_BUF_SIZE );
//						memmove( nume1, F5214A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5214B, MONY_BUF_SIZE );
//						memmove( nume1, F5214B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5214C, MONY_BUF_SIZE );
//						memmove( nume1, F5214C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5214A) > 0 ){
							memmove( val1, F5214A, MONY_BUF_SIZE );
							memmove( nume1, F5214A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5214B) > 0 ){
							memmove( val1, F5214B, MONY_BUF_SIZE );
							memmove( nume1, F5214B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5214C) > 0 ){
							memmove( val1, F5214C, MONY_BUF_SIZE );
							memmove( nume1, F5214C, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[1];
					break;
				default:	
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5213A, MONY_BUF_SIZE );
//						memmove( nume1, F5213A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5213B, MONY_BUF_SIZE );
//						memmove( nume1, F5213B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5213C, MONY_BUF_SIZE );
//						memmove( nume1, F5213C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5213A) > 0 ){
							memmove( val1, F5213A, MONY_BUF_SIZE );
							memmove( nume1, F5213A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5213B) > 0 ){
							memmove( val1, F5213B, MONY_BUF_SIZE );
							memmove( nume1, F5213B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5213C) > 0 ){
							memmove( val1, F5213C, MONY_BUF_SIZE );
							memmove( nume1, F5213C, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[0];
					break;
			}

			// 比率
			if( i==0 ){
				per2 = (*m_pSnHeadData)->SVsper[1];
			}
			else if( (i==1) || (i==4) ){
				per2 = (*m_pSnHeadData)->SVsper[2];
			}
			else if( (i==2) || (i==5) || (i==7) ){
				per2 = (*m_pSnHeadData)->SVsper[3];
			}
			else{
				per2 = (*m_pSnHeadData)->SVsper[4];
			}

			if( j==0 ){
				m_pArith->l_sub( val2, F5212A, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}
			else if( j==1 ){
				m_pArith->l_sub( val2, F5212B, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}
			else if( j==2 ){
				m_pArith->l_sub( val2, F5212C, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}

//-- '15.02.23 --
//			sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per1) );
//			m_pArith->l_input( WORK1, WORK0 );
//			m_pArith->l_input( rate1, WORK0 );
//---------------
			sprintf_s( strBuf, sizeof(strBuf), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, strBuf );
			m_pArith->l_input( rate1, strBuf );
//---------------
			if( m_pArith->l_test(val1) ){
				m_Util.l_6calq( val1, val1, WORK1, PW0 );
			}
//-- '15.02.23 --
//			sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per2) );
//			m_pArith->l_input( WORK1, WORK0 );
//			m_pArith->l_input( rate2, WORK0 );
//---------------
			sprintf_s( strBuf, sizeof(strBuf), _T("%d"), (1000-per2) );
			m_pArith->l_input( WORK1, strBuf );
			m_pArith->l_input( rate2, strBuf );
//---------------
			if( m_pArith->l_test(val2) ){
				m_Util.l_6calq( val2, val2, WORK1, PW0 );
			}
			m_pArith->l_add( val1, val1, val2 );
			
			if( m_pArith->l_test(val1) ){
				if( j==0 ){
					f52calq_skj_tk( WORK0, F524A, nume1, rate1, nume2, rate2, F5212A );
				}
				else if( j==1 ){
					f52calq_skj_tk( WORK0, F524B, nume1, rate1, nume2, rate2, F5212B );
				}
				else if( j==2 ){
					f52calq_skj_tk( WORK0, F524C, nume1, rate1, nume2, rate2, F5212C );
				}
			}
			else{
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			// 各税率の金額へ
			if( j==0 ){
				memmove( val3per, WORK0, MONY_BUF_SIZE );
			}
			else if( j==1 ){
				memmove( val5per, WORK0, MONY_BUF_SIZE );
			}
			else if( j==2 ){
				memmove( val8per, WORK0, MONY_BUF_SIZE );
			}
		}

		// 最有利は?
		m_pArith->l_add( valttl, val3per, val5per );
		m_pArith->l_add( valttl, valttl, val8per );

		if( m_pArith->l_cmp(max_valttl, valttl) < 0 ){
			memmove( max_valttl, valttl, MONY_BUF_SIZE );
			maxSign = 20+i;
		}
	}

	// 19：1種類で75%以上
	if( m_tkSw&0xff ){
		unsigned short	per1=0;
		if( (m_tkSw&0xff) == 0x01 ){
			per1 = (*m_pSnHeadData)->SVsper[0];
		}
		else if( (m_tkSw&0xff) == 0x02 ){
			per1 = (*m_pSnHeadData)->SVsper[1];
		}
		else if( (m_tkSw&0xff) == 0x04 ){
			per1 = (*m_pSnHeadData)->SVsper[2];
		}
		else if( (m_tkSw&0xff) == 0x08 ){
			per1 = (*m_pSnHeadData)->SVsper[3];
		}
		else if( (m_tkSw&0xff) == 0x10 ){
			per1 = (*m_pSnHeadData)->SVsper[4];
		}

		memset( val3per, '\0', MONY_BUF_SIZE );
		memset( val5per, '\0', MONY_BUF_SIZE );
		memset( val8per, '\0', MONY_BUF_SIZE );
		memset( valttl, '\0', MONY_BUF_SIZE );

		for( int j=0; j<3; j++ ){
			memset( WORK0, '\0', sizeof(WORK0) );
			memset( WORK1, '\0', sizeof(WORK1) );
			memset( val1, '\0', sizeof(val1) );

			if( j==0 ){
				memmove( val1, F524A, MONY_BUF_SIZE );
				pVal = val3per;
			}
			else if( j==1 ){
				memmove( val1, F524B, MONY_BUF_SIZE );
				pVal = val5per;
			}
			else if( j==2 ){
				memmove( val1, F524C, MONY_BUF_SIZE );
				pVal = val8per;
			}

			if( m_pArith->l_test(val1) ){
//-- '15.02.23 --
//				sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per1) );
//				m_pArith->l_input( WORK1, WORK0 );
//---------------
				sprintf_s( strBuf, sizeof(strBuf), _T("%d"), (1000-per1) );
				m_pArith->l_input( WORK1, strBuf );
//---------------
				m_Util.l_6calq( pVal, val1, WORK1, PW0 );
			}
		}

		// 最有利は?
		m_pArith->l_add( valttl, val3per, val5per );
		m_pArith->l_add( valttl, valttl, val8per );

		if( m_pArith->l_cmp(max_valttl, valttl) < 0 ){
			memmove( max_valttl, valttl, MONY_BUF_SIZE );
			maxSign = 19;
		}
	}

	m_dspSkjSw = 0;
	if( m_typeCnt > 1 ){
//-- '15.01.27 --
//		if( m_pArith->l_test(F5218T) ){
//			m_dspSkjSw |= 0x01;
//		}
//---------------
		if( m_pArith->l_test(F5218A) || m_pArith->l_test(F5218B) || m_pArith->l_test(F5218C) ){
			m_dspSkjSw |= 0x01;
		}
//---------------
		if( m_pArith->l_test(max_valttl) ){
			long	onbit = 0x02<<(maxSign-19);
			m_dspSkjSw |= onbit;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 全計算
//-----------------------------------------------------------------------------
// 引数	tk_clqsw	：	特例計算の優先順位を再設定するかどうか（1:する, 0:しない）
//-----------------------------------------------------------------------------
int CShinFhyo52::f52calq_tk_all( int tk_clqsw/*=1*/ )
{
	char			val1[MONY_BUF_SIZE], val2[MONY_BUF_SIZE], WORK1[MONY_BUF_SIZE], PW0[MONY_BUF_SIZE], WORK0[MONY_BUF_SIZE];
	char			*VAL;
	char			nume1[MONY_BUF_SIZE], nume2[MONY_BUF_SIZE], rate1[MONY_BUF_SIZE], rate2[MONY_BUF_SIZE];
	char			strBuf[64];	// '15.02.23

	int				calqSw = 0;
	int				chkFlg = 0x08;

	unsigned short	per1, per2;

//--> '14.10.17 INS START
	int	clqmax = 3;
	if( judge_skj_calq() ){
		clqmax = 4;
	}
//<-- '14.10.17 INS END

	m_pArith->l_input( PW0, _T("1000") );

//-- '14.10.17 --
//	for( int j=0; j<3; j++, chkFlg<<=4 ){
//---------------
	for( int j=0; j<clqmax; j++, chkFlg<<=4 ){
//---------------
		for( int i=0; i<10; ++i ){
			memset( val1, '\0', MONY_BUF_SIZE );
			memset( val2, '\0', MONY_BUF_SIZE );
			memset( WORK0, '\0', MONY_BUF_SIZE );
			memset( WORK1, '\0', MONY_BUF_SIZE );
			memset( nume1, '\0', MONY_BUF_SIZE );	// 分子１
			memset( nume2, '\0', MONY_BUF_SIZE );	// 分子２
			memset( rate1, '\0', MONY_BUF_SIZE );	// 割合
			memset( rate2, '\0', MONY_BUF_SIZE );	// 割合
//--> '15.02.23 INS START
			memset( strBuf, '\0', sizeof(strBuf) );
//<-- '15.02.23 INS END

			switch( i ){
				case 9 :
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5216A, MONY_BUF_SIZE );
//						memmove( nume1, F5216A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5216B, MONY_BUF_SIZE );
//						memmove( nume1, F5216B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5216C, MONY_BUF_SIZE );
//						memmove( nume1, F5216C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5216A) > 0 ){
							memmove( val1, F5216A, MONY_BUF_SIZE );
							memmove( nume1, F5216A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5216B) > 0 ){
							memmove( val1, F5216B, MONY_BUF_SIZE );
							memmove( nume1, F5216B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5216C) > 0 ){
							memmove( val1, F5216C, MONY_BUF_SIZE );
							memmove( nume1, F5216C, MONY_BUF_SIZE );
						}
					}
					else if( j==3 ){
						if( m_pArith->l_test(F5216T) > 0 ){
							memmove( val1, F5216T, MONY_BUF_SIZE );
							memmove( nume1, F5216T, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[3];
					break;
				case 8 :
				case 7 :
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5215A, MONY_BUF_SIZE );
//						memmove( nume1, F5215A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5215B, MONY_BUF_SIZE );
//						memmove( nume1, F5215B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5215C, MONY_BUF_SIZE );
//						memmove( nume1, F5215C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5215A) > 0 ){
							memmove( val1, F5215A, MONY_BUF_SIZE );
							memmove( nume1, F5215A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5215B) > 0 ){
							memmove( val1, F5215B, MONY_BUF_SIZE );
							memmove( nume1, F5215B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5215C) > 0 ){
							memmove( val1, F5215C, MONY_BUF_SIZE );
							memmove( nume1, F5215C, MONY_BUF_SIZE );
						}
					}
					else if( j==3 ){
						if( m_pArith->l_test(F5215T) > 0 ){
							memmove( val1, F5215T, MONY_BUF_SIZE );
							memmove( nume1, F5215T, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[2];
					break;
				case 6 :
				case 5 :
				case 4 :
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5214A, MONY_BUF_SIZE );
//						memmove( nume1, F5214A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5214B, MONY_BUF_SIZE );
//						memmove( nume1, F5214B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5214C, MONY_BUF_SIZE );
//						memmove( nume1, F5214C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5214A) > 0 ){
							memmove( val1, F5214A, MONY_BUF_SIZE );
							memmove( nume1, F5214A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5214B) > 0 ){
							memmove( val1, F5214B, MONY_BUF_SIZE );
							memmove( nume1, F5214B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5214C) > 0 ){
							memmove( val1, F5214C, MONY_BUF_SIZE );
							memmove( nume1, F5214C, MONY_BUF_SIZE );
						}
					}
					else if( j==3 ){
						if( m_pArith->l_test(F5214T) > 0 ){
							memmove( val1, F5214T, MONY_BUF_SIZE );
							memmove( nume1, F5214T, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[1];
					break;
				default:	
//-- '14.10.17 --
//					if( j==0 ){
//						memmove( val1, F5213A, MONY_BUF_SIZE );
//						memmove( nume1, F5213A, MONY_BUF_SIZE );
//					}
//					else if( j==1 ){
//						memmove( val1, F5213B, MONY_BUF_SIZE );
//						memmove( nume1, F5213B, MONY_BUF_SIZE );
//					}
//					else if( j==2 ){
//						memmove( val1, F5213C, MONY_BUF_SIZE );
//						memmove( nume1, F5213C, MONY_BUF_SIZE );
//					}
//---------------
					if( j==0 ){
						if( m_pArith->l_test(F5213A) > 0 ){
							memmove( val1, F5213A, MONY_BUF_SIZE );
							memmove( nume1, F5213A, MONY_BUF_SIZE );
						}
					}
					else if( j==1 ){
						if( m_pArith->l_test(F5213B) > 0 ){
							memmove( val1, F5213B, MONY_BUF_SIZE );
							memmove( nume1, F5213B, MONY_BUF_SIZE );
						}
					}
					else if( j==2 ){
						if( m_pArith->l_test(F5213C) > 0 ){
							memmove( val1, F5213C, MONY_BUF_SIZE );
							memmove( nume1, F5213C, MONY_BUF_SIZE );
						}
					}
					else if( j==3 ){
						if( m_pArith->l_test(F5213T) > 0 ){
							memmove( val1, F5213T, MONY_BUF_SIZE );
							memmove( nume1, F5213T, MONY_BUF_SIZE );
						}
					}
//---------------
					per1 = (*m_pSnHeadData)->SVsper[0];
					break;
			}

			switch( i ){
				case 0 :
					per2 = (*m_pSnHeadData)->SVsper[1];
					break;
				case 1 :
				case 4 :
					per2 = (*m_pSnHeadData)->SVsper[2];
					break;
				case 2 :
				case 5 : 
				case 7 :
					per2 = (*m_pSnHeadData)->SVsper[3];
					break;
				default:
					per2 = (*m_pSnHeadData)->SVsper[4];
					break;
			}

			if( j==0 ){
				m_pArith->l_sub( val2, F5212A, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}
			else if( j==1 ){
				m_pArith->l_sub( val2, F5212B, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}
			else if( j==2 ){
				m_pArith->l_sub( val2, F5212C, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}
//--> '14.10.17 INS START
			else if( j==3 ){
				m_pArith->l_sub( val2, F5212T, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );
			}
//<-- '14.10.17 INS END

//-- '15.02.23 --
//			sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per1) );
//			m_pArith->l_input( WORK1, WORK0 );
//			m_pArith->l_input( rate1, WORK0 );
//---------------
			sprintf_s( strBuf, sizeof(strBuf), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, strBuf );
			m_pArith->l_input( rate1, strBuf );
//---------------
			if( m_pArith->l_test(val1) ){
				m_Util.l_6calq( val1, val1, WORK1, PW0 );
			}
//-- '15.02.23 --
//			sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000-per2) );
//			m_pArith->l_input( WORK1, WORK0 );
//			m_pArith->l_input( rate2, WORK0 );
//---------------
			sprintf_s( strBuf, sizeof(strBuf), _T("%d"), (1000-per2) );
			m_pArith->l_input( WORK1, strBuf );
			m_pArith->l_input( rate2, strBuf );
//---------------
			if( m_pArith->l_test(val2) ){
				m_Util.l_6calq( val2, val2, WORK1, PW0 );
			}
			m_pArith->l_add( val1, val1, val2 );
			
			if( m_pArith->l_test(val1) ){
				if( j==0 ){
/*- '14.03.11 -*/
//					m_pArith->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)F5212A, 7, 0 );
//					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524A, (unsigned char *)WORK1, 7, 0 );
/*-------------*/
					f52calq_skj_tk( WORK0, F524A, nume1, rate1, nume2, rate2, F5212A );
/*-------------*/
				}
				else if( j==1 ){
/*- '14.03.11 -*/
//					m_pArith->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)F5212B, 7, 0 );
//					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524B, (unsigned char *)WORK1, 7, 0 );
/*-------------*/
					f52calq_skj_tk( WORK0, F524B, nume1, rate1, nume2, rate2, F5212B );
/*-------------*/
				}
				else if( j==2 ){
/*- '14.03.11 -*/
//					m_pArith->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)F5212C, 7, 0 );
//					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F524C, (unsigned char *)WORK1, 7, 0 );
/*-------------*/
					f52calq_skj_tk( WORK0, F524C, nume1, rate1, nume2, rate2, F5212C );
/*-------------*/
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					f52calq_skj_tk( WORK0, F524T, nume1, rate1, nume2, rate2, F5212T );
				}
//<-- '14.10.17 INS END
			}
			else{
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( i==0 ){
				if( j==0 ){
					VAL = F5220A;
				}
				else if( j==1 ){
					VAL = F5220B;
				}
				else if( j==2 ){
					VAL = F5220C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5220T;
				}
//<-- '14.10.17 INS END
				calqSw = F5220c;
			}
			else if( i==1 ){
				if( j==0 ){
					VAL = F5221A;
				}
				else if( j==1 ){
					VAL = F5221B;
				}
				else if( j==2 ){
					VAL = F5221C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5221T;
				}
//<-- '14.10.17 INS END
				calqSw = F5221c;
			}
			else if( i==2 ){
				if( j==0 ){
					VAL = F5222A;
				}
				else if( j==1 ){
					VAL = F5222B;
				}
				else if( j==2 ){
					VAL = F5222C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5222T;
				}
//<-- '14.10.17 INS END
				calqSw = F5222c;
			}
			else if( i==3 ){
				if( j==0 ){
					VAL = F5223A;
				}
				else if( j==1 ){
					VAL = F5223B;
				}
				else if( j==2 ){
					VAL = F5223C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5223T;
				}
//<-- '14.10.17 INS END
				calqSw = F5223c;
			}
			else if( i==4 ){
				if( j==0 ){
					VAL = F5224A;
				}
				else if( j==1 ){
					VAL = F5224B;
				}
				else if( j==2 ){
					VAL = F5224C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5224T;
				}
//<-- '14.10.17 INS END
				calqSw = F5224c;
			}
			else if( i==5 ){
				if( j==0 ){
					VAL = F5225A;
				}
				else if( j==1 ){
					VAL = F5225B;
				}
				else if( j==2 ){
					VAL = F5225C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5225T;
				}
//<-- '14.10.17 INS END
				calqSw = F5225c;
			}
			else if( i==6 ){
				if( j==0 ){
					VAL = F5226A;
				}
				else if( j==1 ){
					VAL = F5226B;
				}
				else if( j==2 ){
					VAL = F5226C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5226T;
				}
//<-- '14.10.17 INS END
				calqSw = F5226c;
			}
			else if( i==7 ){
				if( j==0 ){
					VAL = F5227A;
				}
				else if( j==1 ){
					VAL = F5227B;
				}
				else if( j==2 ){
					VAL = F5227C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5227T;
				}
//<-- '14.10.17 INS END
				calqSw = F5227c;
			}
			else if( i==8 ){
				if( j==0 ){
					VAL = F5228A;
				}
				else if( j==1 ){
					VAL = F5228B;
				}
				else if( j==2 ){
					VAL = F5228C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5228T;
				}
//<-- '14.10.17 INS END
				calqSw = F5228c;
			}
			else if( i==9 ){
				if( j==0 ){
					VAL = F5229A;
				}
				else if( j==1 ){
					VAL = F5229B;
				}
				else if( j==2 ){
					VAL = F5229C;
				}
//--> '14.10.17 INS START
				else if( j==3 ){
					VAL = F5229T;
				}
//<-- '14.10.17 INS END
				calqSw = F5229c;
			}
			if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
				if( !(calqSw&chkFlg) ){
					memmove( VAL, WORK0, MONY_BUF_SIZE );
				}
			}

#ifdef _DEBUG
			char	moji[128] = {0};
			m_pArith->l_print( moji, VAL, _T("sss,sss,sss,sss,ss9") );
			int hoge = 0;
			hoge++;
#endif
			
//			if( m_pArith->l_cmp(VAL, WORK0) < 0 ){
//				if( (m_Szsw==1) && (m_typeCnt<=2) ){
//	//					if( 0 ){
//					m_Szsw = 0;
//					goto CALQB;
//				}
//				memmove( VAL, WORK0, MONY_BUF_SIZE );
//				m_Szsw = (char) (i + 2);
//			}
		}
	}

	if( !(m_tkSw&0xff) ){
		memset( F5219T, '\0', sizeof( F5219T ));
		memset( F5219A, '\0', sizeof( F5219A ));
		memset( F5219B, '\0', sizeof( F5219B ));
		memset( F5219C, '\0', sizeof( F5219C ));
		F5219c = 0;
	}

//--> '14.10.17 INS START
	BOOL	isAddtype = TRUE;
	if( judge_skj_calq() == 1 ){
		isAddtype = FALSE;
	}
//--> '14.10.17 INS END

//-- '14.10.17 --
//	if( m_tkSw&0x100 ){
//		m_pArith->l_add( F5220T, F5220A, F5220B );
//		m_pArith->l_add( F5220T, F5220T, F5220C );
//	}
//	else{
//		memset( F5220A, '\0', sizeof(F5220A) );
//		memset( F5220B, '\0', sizeof(F5220B) );
//		memset( F5220C, '\0', sizeof(F5220C) );
//		memset( F5220T, '\0', sizeof(F5220T) );
//		F5220c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x100 ){
			m_pArith->l_add( F5220T, F5220A, F5220B );
			m_pArith->l_add( F5220T, F5220T, F5220C );
		}
		else{
			memset( F5220A, '\0', sizeof(F5220A) );
			memset( F5220B, '\0', sizeof(F5220B) );
			memset( F5220C, '\0', sizeof(F5220C) );
			memset( F5220T, '\0', sizeof(F5220T) );
			F5220c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x100) ){
			memset( F5220A, '\0', sizeof(F5220A) );
			memset( F5220B, '\0', sizeof(F5220B) );
			memset( F5220C, '\0', sizeof(F5220C) );
			memset( F5220T, '\0', sizeof(F5220T) );
			F5220c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x200 ){
//		m_pArith->l_add( F5221T, F5221A, F5221B );
//		m_pArith->l_add( F5221T, F5221T, F5221C );
//	}
//	else{
//		memset( F5221A, '\0', sizeof(F5221A) );
//		memset( F5221B, '\0', sizeof(F5221B) );
//		memset( F5221C, '\0', sizeof(F5221C) );
//		memset( F5221T, '\0', sizeof(F5221T) );
//		F5221c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x200 ){
			m_pArith->l_add( F5221T, F5221A, F5221B );
			m_pArith->l_add( F5221T, F5221T, F5221C );
		}
		else{
			memset( F5221A, '\0', sizeof(F5221A) );
			memset( F5221B, '\0', sizeof(F5221B) );
			memset( F5221C, '\0', sizeof(F5221C) );
			memset( F5221T, '\0', sizeof(F5221T) );
			F5221c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x200) ){
			memset( F5221A, '\0', sizeof(F5221A) );
			memset( F5221B, '\0', sizeof(F5221B) );
			memset( F5221C, '\0', sizeof(F5221C) );
			memset( F5221T, '\0', sizeof(F5221T) );
			F5221c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x400 ){
//		m_pArith->l_add( F5222T, F5222A, F5222B );
//		m_pArith->l_add( F5222T, F5222T, F5222C );
//	}
//	else{
//		memset( F5222A, '\0', sizeof(F5222A) );
//		memset( F5222B, '\0', sizeof(F5222B) );
//		memset( F5222C, '\0', sizeof(F5222C) );
//		memset( F5222T, '\0', sizeof(F5222T) );
//		F5222c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x400 ){
			m_pArith->l_add( F5222T, F5222A, F5222B );
			m_pArith->l_add( F5222T, F5222T, F5222C );
		}
		else{
			memset( F5222A, '\0', sizeof(F5222A) );
			memset( F5222B, '\0', sizeof(F5222B) );
			memset( F5222C, '\0', sizeof(F5222C) );
			memset( F5222T, '\0', sizeof(F5222T) );
			F5222c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x400) ){
			memset( F5222A, '\0', sizeof(F5222A) );
			memset( F5222B, '\0', sizeof(F5222B) );
			memset( F5222C, '\0', sizeof(F5222C) );
			memset( F5222T, '\0', sizeof(F5222T) );
			F5222c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x800 ){
//		m_pArith->l_add( F5223T, F5223A, F5223B );
//		m_pArith->l_add( F5223T, F5223T, F5223C );
//	}
//	else{
//		memset( F5223A, '\0', sizeof(F5223A) );
//		memset( F5223B, '\0', sizeof(F5223B) );
//		memset( F5223C, '\0', sizeof(F5223C) );
//		memset( F5223T, '\0', sizeof(F5223T) );
//		F5223c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x800 ){
			m_pArith->l_add( F5223T, F5223A, F5223B );
			m_pArith->l_add( F5223T, F5223T, F5223C );
		}
		else{
			memset( F5223A, '\0', sizeof(F5223A) );
			memset( F5223B, '\0', sizeof(F5223B) );
			memset( F5223C, '\0', sizeof(F5223C) );
			memset( F5223T, '\0', sizeof(F5223T) );
			F5223c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x800) ){
			memset( F5223A, '\0', sizeof(F5223A) );
			memset( F5223B, '\0', sizeof(F5223B) );
			memset( F5223C, '\0', sizeof(F5223C) );
			memset( F5223T, '\0', sizeof(F5223T) );
			F5223c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x1000 ){
//		m_pArith->l_add( F5224T, F5224A, F5224B );
//		m_pArith->l_add( F5224T, F5224T, F5224C );
//	}
//	else{
//		memset( F5224A, '\0', sizeof(F5224A) );
//		memset( F5224B, '\0', sizeof(F5224B) );
//		memset( F5224C, '\0', sizeof(F5224C) );
//		memset( F5224T, '\0', sizeof(F5224T) );
//		F5224c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x1000 ){
			m_pArith->l_add( F5224T, F5224A, F5224B );
			m_pArith->l_add( F5224T, F5224T, F5224C );
		}
		else{
			memset( F5224A, '\0', sizeof(F5224A) );
			memset( F5224B, '\0', sizeof(F5224B) );
			memset( F5224C, '\0', sizeof(F5224C) );
			memset( F5224T, '\0', sizeof(F5224T) );
			F5224c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x1000) ){
			memset( F5224A, '\0', sizeof(F5224A) );
			memset( F5224B, '\0', sizeof(F5224B) );
			memset( F5224C, '\0', sizeof(F5224C) );
			memset( F5224T, '\0', sizeof(F5224T) );
			F5224c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x2000 ){
//		m_pArith->l_add( F5225T, F5225A, F5225B );
//		m_pArith->l_add( F5225T, F5225T, F5225C );
//	}
//	else{
//		memset( F5225A, '\0', sizeof(F5225A) );
//		memset( F5225B, '\0', sizeof(F5225B) );
//		memset( F5225C, '\0', sizeof(F5225C) );
//		memset( F5225T, '\0', sizeof(F5225T) );
//		F5225c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x2000 ){
			m_pArith->l_add( F5225T, F5225A, F5225B );
			m_pArith->l_add( F5225T, F5225T, F5225C );
		}
		else{
			memset( F5225A, '\0', sizeof(F5225A) );
			memset( F5225B, '\0', sizeof(F5225B) );
			memset( F5225C, '\0', sizeof(F5225C) );
			memset( F5225T, '\0', sizeof(F5225T) );
			F5225c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x2000) ){
			memset( F5225A, '\0', sizeof(F5225A) );
			memset( F5225B, '\0', sizeof(F5225B) );
			memset( F5225C, '\0', sizeof(F5225C) );
			memset( F5225T, '\0', sizeof(F5225T) );
			F5225c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x4000 ){
//		m_pArith->l_add( F5226T, F5226A, F5226B );
//		m_pArith->l_add( F5226T, F5226T, F5226C );
//	}
//	else{
//		memset( F5226A, '\0', sizeof(F5226A) );
//		memset( F5226B, '\0', sizeof(F5226B) );
//		memset( F5226C, '\0', sizeof(F5226C) );
//		memset( F5226T, '\0', sizeof(F5226T) );
//		F5226c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x4000 ){
			m_pArith->l_add( F5226T, F5226A, F5226B );
			m_pArith->l_add( F5226T, F5226T, F5226C );
		}
		else{
			memset( F5226A, '\0', sizeof(F5226A) );
			memset( F5226B, '\0', sizeof(F5226B) );
			memset( F5226C, '\0', sizeof(F5226C) );
			memset( F5226T, '\0', sizeof(F5226T) );
			F5226c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x4000) ){
			memset( F5226A, '\0', sizeof(F5226A) );
			memset( F5226B, '\0', sizeof(F5226B) );
			memset( F5226C, '\0', sizeof(F5226C) );
			memset( F5226T, '\0', sizeof(F5226T) );
			F5226c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x8000 ){
//		m_pArith->l_add( F5227T, F5227A, F5227B );
//		m_pArith->l_add( F5227T, F5227T, F5227C );
//	}
//	else{
//		memset( F5227A, '\0', sizeof(F5227A) );
//		memset( F5227B, '\0', sizeof(F5227B) );
//		memset( F5227C, '\0', sizeof(F5227C) );
//		memset( F5227T, '\0', sizeof(F5227T) );
//		F5227c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x8000 ){
			m_pArith->l_add( F5227T, F5227A, F5227B );
			m_pArith->l_add( F5227T, F5227T, F5227C );
		}
		else{
			memset( F5227A, '\0', sizeof(F5227A) );
			memset( F5227B, '\0', sizeof(F5227B) );
			memset( F5227C, '\0', sizeof(F5227C) );
			memset( F5227T, '\0', sizeof(F5227T) );
			F5227c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x8000) ){
			memset( F5227A, '\0', sizeof(F5227A) );
			memset( F5227B, '\0', sizeof(F5227B) );
			memset( F5227C, '\0', sizeof(F5227C) );
			memset( F5227T, '\0', sizeof(F5227T) );
			F5227c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x10000 ){
//		m_pArith->l_add( F5228T, F5228A, F5228B );
//		m_pArith->l_add( F5228T, F5228T, F5228C );
//	}
//	else{
//		memset( F5228A, '\0', sizeof(F5228A) );
//		memset( F5228B, '\0', sizeof(F5228B) );
//		memset( F5228C, '\0', sizeof(F5228C) );
//		memset( F5228T, '\0', sizeof(F5228T) );
//		F5228c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x10000 ){
			m_pArith->l_add( F5228T, F5228A, F5228B );
			m_pArith->l_add( F5228T, F5228T, F5228C );
		}
		else{
			memset( F5228A, '\0', sizeof(F5228A) );
			memset( F5228B, '\0', sizeof(F5228B) );
			memset( F5228C, '\0', sizeof(F5228C) );
			memset( F5228T, '\0', sizeof(F5228T) );
			F5228c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x10000) ){
			memset( F5228A, '\0', sizeof(F5228A) );
			memset( F5228B, '\0', sizeof(F5228B) );
			memset( F5228C, '\0', sizeof(F5228C) );
			memset( F5228T, '\0', sizeof(F5228T) );
			F5228c = 0;
		}
	}
//---------------

//-- '14.10.17 --
//	if( m_tkSw&0x20000 ){
//		m_pArith->l_add( F5229T, F5229A, F5229B );
//		m_pArith->l_add( F5229T, F5229T, F5229C );
//	}
//	else{
//		memset( F5229A, '\0', sizeof(F5229A) );
//		memset( F5229B, '\0', sizeof(F5229B) );
//		memset( F5229C, '\0', sizeof(F5229C) );
//		memset( F5229T, '\0', sizeof(F5229T) );
//		F5229c = 0;
//	}
//---------------
	if( isAddtype ){
		if( m_tkSw&0x20000 ){
			m_pArith->l_add( F5229T, F5229A, F5229B );
			m_pArith->l_add( F5229T, F5229T, F5229C );
		}
		else{
			memset( F5229A, '\0', sizeof(F5229A) );
			memset( F5229B, '\0', sizeof(F5229B) );
			memset( F5229C, '\0', sizeof(F5229C) );
			memset( F5229T, '\0', sizeof(F5229T) );
			F5229c = 0;
		}
	}
	else{
		if( !(m_tkSw&0x20000) ){
			memset( F5229A, '\0', sizeof(F5229A) );
			memset( F5229B, '\0', sizeof(F5229B) );
			memset( F5229C, '\0', sizeof(F5229C) );
			memset( F5229T, '\0', sizeof(F5229T) );
			F5229c = 0;
		}
	}
//---------------

//--> '14.06.11 INS START
//	// 特例計算適用時の最大に変化有り？
//	if( m_SvSzsw == -1 ){
//		m_dspSkjSw = 0;
//	}
//	else if( m_SvSzsw == 0 ){
//		// 元々 原則計算
//		m_dspSkjSw = 0;
//	}
//	else if( m_SvSzsw > 0 ){
//		// 元々 特例計算
//		int tmpmaxSign = m_SvSzsw + (19-1);
//		if( tmpmaxSign != maxSign ){
//			m_dspSkjSw = 0;
//		}
//	}
//<-- '14.06.11 INS END

//--> '14.06.11 INS START
	// 表示は 原則＋特例の最有利
/*-- '14.06.15 --*/
//	m_dspSkjSw = 0;
//	if( m_pArith->l_test(F5218T) ){
//		m_dspSkjSw |= 0x01;
//	}
//	if( m_pArith->l_test(pValT) ){
//		long	onbit = 0x02<<(maxSign-19);
//		m_dspSkjSw |= onbit;
//	}
/*---------------*/
	f52calq_tk_for_disp();
/*---------------*/
//<-- '14.06.11 INS END

	// 最大の判定
	int	maxSign = 19;
	char	*pValA=NULL, *pValB=NULL, *pValC=NULL, *pValT=NULL;

	if( m_dspSkjSw&0x02 ){
		memmove( WORK1, F5219T, MONY_BUF_SIZE );
		pValA = F5219A;
		pValB = F5219B;
		pValC = F5219C;
		pValT = F5219T;
		maxSign = 19;
	}
	else if( m_dspSkjSw&0x04 ){
		memmove( WORK1, F5220T, MONY_BUF_SIZE );
		pValA = F5220A;
		pValB = F5220B;
		pValC = F5220C;
		pValT = F5220T;
		maxSign = 20;
	}
	else if( m_dspSkjSw&0x08 ){
		memmove( WORK1, F5221T, MONY_BUF_SIZE );
		pValA = F5221A;
		pValB = F5221B;
		pValC = F5221C;
		pValT = F5221T;
		maxSign = 21;
	}
	else if( m_dspSkjSw&0x10 ){
		memmove( WORK1, F5222T, MONY_BUF_SIZE );
		pValA = F5222A;
		pValB = F5222B;
		pValC = F5222C;
		pValT = F5222T;
		maxSign = 22;
	}
	else if( m_dspSkjSw&0x20 ){
		memmove( WORK1, F5223T, MONY_BUF_SIZE );
		pValA = F5223A;
		pValB = F5223B;
		pValC = F5223C;
		pValT = F5223T;
		maxSign = 23;
	}
	else if( m_dspSkjSw&0x40 ){
		memmove( WORK1, F5224T, MONY_BUF_SIZE );
		pValA = F5224A;
		pValB = F5224B;
		pValC = F5224C;
		pValT = F5224T;
		maxSign = 24;
	}
	else if( m_dspSkjSw&0x80 ){
		memmove( WORK1, F5225T, MONY_BUF_SIZE );
		pValA = F5225A;
		pValB = F5225B;
		pValC = F5225C;
		pValT = F5225T;
		maxSign = 25;
	}
	else if( m_dspSkjSw&0x100 ){
		memmove( WORK1, F5226T, MONY_BUF_SIZE );
		pValA = F5226A;
		pValB = F5226B;
		pValC = F5226C;
		pValT = F5226T;
		maxSign = 26;
	}
	else if( m_dspSkjSw&0x200 ){
		memmove( WORK1, F5227T, MONY_BUF_SIZE );
		pValA = F5227A;
		pValB = F5227B;
		pValC = F5227C;
		pValT = F5227T;
		maxSign = 27;
	}
	else if( m_dspSkjSw&0x400 ){
		memmove( WORK1, F5228T, MONY_BUF_SIZE );
		pValA = F5228A;
		pValB = F5228B;
		pValC = F5228C;
		pValT = F5228T;
		maxSign = 28;
	}
	else if( m_dspSkjSw&0x800 ){
		memmove( WORK1, F5229T, MONY_BUF_SIZE );
		pValA = F5229A;
		pValB = F5229B;
		pValC = F5229C;
		pValT = F5229T;
		maxSign = 29;
	}

/*- '14.04.26 -*/
//	if( m_pArith->l_cmp(F5218T, pValT) > 0 ){
//		// 原則計算
//		memmove( F5230A, F5218A, MONY_BUF_SIZE );
//		memmove( F5230B, F5218B, MONY_BUF_SIZE );
//		memmove( F5230C, F5218C, MONY_BUF_SIZE );
//		memmove( F5230T, F5218T, MONY_BUF_SIZE );
////		m_Szsw = 0;
//		(*m_pSnHeadData)->Sn_TOKUR = 0;
//	}
//	else{
//		// 特例計算
//		memmove( F5230A, pValA, MONY_BUF_SIZE );
//		memmove( F5230B, pValB, MONY_BUF_SIZE );
//		memmove( F5230C, pValC, MONY_BUF_SIZE );
//		memmove( F5230T, pValT, MONY_BUF_SIZE );
////		m_Szsw = maxSign - (19-1);
//		(*m_pSnHeadData)->Sn_TOKUR = 1;
//	}
/*-------------*/
	if( pValT ){
//-- '15.01.27 --
//		if( m_pArith->l_cmp(F5218T, pValT) < 0 ){
//			// 特例計算
//			memmove( F5230A, pValA, MONY_BUF_SIZE );
//			memmove( F5230B, pValB, MONY_BUF_SIZE );
//			memmove( F5230C, pValC, MONY_BUF_SIZE );
//			memmove( F5230T, pValT, MONY_BUF_SIZE );
//			(*m_pSnHeadData)->Sn_TOKUR = 1;
//		}
//		else{
//			// 原則計算
//			memmove( F5230A, F5218A, MONY_BUF_SIZE );
//			memmove( F5230B, F5218B, MONY_BUF_SIZE );
//			memmove( F5230C, F5218C, MONY_BUF_SIZE );
//			memmove( F5230T, F5218T, MONY_BUF_SIZE );
//			(*m_pSnHeadData)->Sn_TOKUR = 0;
//		}
//---------------
		BOOL	isUseTkSum = TRUE;
		if( m_pArith->l_cmp(F5218T, pValT) < 0 ){
			isUseTkSum = TRUE;
		}
		else if( m_pArith->l_cmp(F5218T, pValT) == 0 ){
			// 原則計算と特例計算の比較
			char	genSum[MONY_BUF_SIZE]={0}, tkSum[MONY_BUF_SIZE]={0};
			m_pArith->l_add( genSum, F5218A, F5218B );
			m_pArith->l_add( genSum, genSum, F5218C );
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
			memmove( F5230A, pValA, MONY_BUF_SIZE );
			memmove( F5230B, pValB, MONY_BUF_SIZE );
			memmove( F5230C, pValC, MONY_BUF_SIZE );
			memmove( F5230T, pValT, MONY_BUF_SIZE );
			(*m_pSnHeadData)->Sn_TOKUR = 1;
		}
		else{
			memmove( F5230A, F5218A, MONY_BUF_SIZE );
			memmove( F5230B, F5218B, MONY_BUF_SIZE );
			memmove( F5230C, F5218C, MONY_BUF_SIZE );
			memmove( F5230T, F5218T, MONY_BUF_SIZE );
			(*m_pSnHeadData)->Sn_TOKUR = 0;
		}
//---------------
	}
	else{
		// 原則計算
		memmove( F5230A, F5218A, MONY_BUF_SIZE );
		memmove( F5230B, F5218B, MONY_BUF_SIZE );
		memmove( F5230C, F5218C, MONY_BUF_SIZE );
		memmove( F5230T, F5218T, MONY_BUF_SIZE );
		(*m_pSnHeadData)->Sn_TOKUR = 0;
	}
/*-------------*/

	// 最も有利な特例計算は？
/*- '14.04.26 -*/
//	if( m_pArith->l_test(WORK1) ){
//		m_Szsw = maxSign - (19-1);
////--> '14.04.22 INS START
//		if( m_Szsw == 1 ){
//			m_tkSw &= 0xff;
//		}
//		else{
//			m_tkSw &= ~0xff;
//		}
////<-- '14.04.22 INS END
//	}
/*-------------*/
	if( (*m_pSnHeadData)->Sn_TOKUR == 1 ){
//-- '15.01.27 --
//		if( m_pArith->l_test(WORK1) ){
//---------------
		if( m_pArith->l_test(WORK1) || m_pArith->l_test(pValA) || m_pArith->l_test(pValB) || m_pArith->l_test(pValC) ){
//---------------
			m_Szsw = maxSign - (19-1);
//--> '14.06.16 CUT START
//			if( m_Szsw == 1 ){
//				m_tkSw &= 0xff;
//			}
//			else{
//				m_tkSw &= ~0xff;
//			}
//<-- '14.06.16 CUT END
		}
	}
	else{
		m_Szsw = 0;
//--> '14.06.16 CUT START
//		m_tkSw = 0;
//<-- '14.06.16 CUT END
	}
/*-------------*/

//--> '14.06.11 INS START
/*	if( (*m_pSnHeadData)->Sn_TOKUR == 1 ){
		m_dspSkjSw = 0;
		// 特例計算
		long	chkbit = 0x01<<m_Szsw;
		m_dspSkjSw |= chkbit;
		if( m_pArith->l_test(F5218T) ){
			m_dspSkjSw |= 0x01;
		}
	}
	else{
		if( m_dspSkjSw&~0x01 ){
			if( F5218c ){
				// 変更しない
				;
			}
			else{
				m_dspSkjSw = 0;
				// 特例計算
				if( F5219c ){
					m_dspSkjSw |= 0x02;
				}
				if( F5220c ){
					m_dspSkjSw |= 0x04;
				}
				if( F5221c ){
					m_dspSkjSw |= 0x08;
				}
				if( F5222c ){
					m_dspSkjSw |= 0x10;
				}
				if( F5223c ){
					m_dspSkjSw |= 0x20;
				}
				if( F5224c ){
					m_dspSkjSw |= 0x40;
				}
				if( F5225c ){
					m_dspSkjSw |= 0x80;
				}
				if( F5226c ){
					m_dspSkjSw |= 0x100;
				}
				if( F5227c ){
					m_dspSkjSw |= 0x200;
				}
				if( F5228c ){
					m_dspSkjSw |= 0x400;
				}
				if( F5229c ){
					m_dspSkjSw |= 0x800;
				}
			}
		}

		// 原則計算
		m_dspSkjSw |= 0x01;
	}*/
//<-- '14.06.11 INS END

#ifdef _DEBUG
	char	moji[128] = {0};
	CArithEx	Arith(0x16);

	Arith.l_print( moji, F5220A, _T("sss,sss,sss,sss,ss9") );
	Arith.l_print( moji, F5220B, _T("sss,sss,sss,sss,ss9") );
	Arith.l_print( moji, F5220C, _T("sss,sss,sss,sss,ss9") );
	Arith.l_print( moji, F5220T, _T("sss,sss,sss,sss,ss9") );
#endif
	
	//計算後、該当行以外の手入力サインを落とす
	if( F5220c ){
		m_Szsw = 2;
	}
	else if( F5221c ){
		m_Szsw = 3;
	}
	else if( F5222c ){
		m_Szsw = 4;
	}
	else if( F5223c ){
		m_Szsw = 5;
	}
	else if( F5224c ){
		m_Szsw = 6;
	}
	else if( F5225c ){
		m_Szsw = 7;
	}
	else if( F5226c ){
		m_Szsw = 8;
	}
	else if( F5227c ){
		m_Szsw = 9;
	}
	else if( F5228c ){
		m_Szsw = 10;
	}
	else if( F5229c ){
		m_Szsw = 11;
	}
	
	//もう一度
	if( m_Szsw != 2 ){
		F5220c = 0;
	}
	if( m_Szsw != 3 ){
		F5221c = 0;
	}
	if( m_Szsw != 4 ){
		F5222c = 0;
	}
	if( m_Szsw != 5 ){
		F5223c = 0;
	}
	if( m_Szsw != 6 ){
		F5224c = 0;
	}
	if( m_Szsw != 7 ){
		F5225c = 0;
	}
	if( m_Szsw != 8 ){
		F5226c = 0;
	}
	if( m_Szsw != 9 ){
		F5227c = 0;
	}
	if( m_Szsw != 10 ){
		F5228c = 0;
	}
	if( m_Szsw != 11 ){
		F5229c = 0;
	}

//--> '14.06.11 INS START
	m_SvSzsw = m_Szsw;
//<-- '14.06.11 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// １フィールドの控除対象仕入税額の計算(原則計算)	('14.06.10)
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_one_skj_gen( int rIdx, int cIdx )
{
	if( cIdx == 0 ){
		f52calq_skj_gn( F5218A, cIdx );
	}
	else if( cIdx == 1 ){
		f52calq_skj_gn( F5218B, cIdx );
	}
	else if( cIdx == 2 ){
		f52calq_skj_gn( F5218C, cIdx );
	}
//--> '14.10.17 INS START
	else if( cIdx == 3 ){
		f52calq_skj_gn( F5218T, cIdx );
	}
//<-- '14.10.17 INS END

//-- '14.10.17 --
//	// 横計
//	m_pArith->l_add( F5218T, F5218A, F5218B );
//	m_pArith->l_add( F5218T, F5218T, F5218C );
//---------------
	// 横計
	if( judge_skj_calq() == 0 ){
		m_pArith->l_add( F5218T, F5218A, F5218B );
		m_pArith->l_add( F5218T, F5218T, F5218C );
	}
//---------------
}

//-----------------------------------------------------------------------------
// 該当セルを計算するのに必要な金額フィールドを取得
//-----------------------------------------------------------------------------
// 引数	rIdx	：
//		cIdx	：
//		pVal1	：
//		pValttl	：
//		pRate1	：
//		pRate2	：
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_get_clqfield( int rIdx, int cIdx, char *pVal1, char *pValttl, char *pRate1, char *pRate2 )
{
	char	val1[MONY_BUF_SIZE]={0}, valttl[MONY_BUF_SIZE]={0};
	char	rate1[MONY_BUF_SIZE]={0}, rate2[MONY_BUF_SIZE]={0};
	int		nRate1=0, nRate2=0;

	if( ((cIdx+1)<0) || (8<=(cIdx+1)) ){
		return;
	}

	// 被演算金額、割合１
//-- '14.10.17 --
//	if( (2<=rIdx) && (rIdx<=5) ){
//		memmove( val1, m_Fh5Mony.pF5213[cIdx+1], MONY_BUF_SIZE );
//		memmove( valttl, m_Fh5Mony.pF5212[cIdx+1], MONY_BUF_SIZE );
//		nRate1 = (*m_pSnHeadData)->SVsper[0];
//	}
//	else if( (6<=rIdx) && (rIdx<=8) ){
//		memmove( val1, m_Fh5Mony.pF5214[cIdx+1], MONY_BUF_SIZE );
//		memmove( valttl, m_Fh5Mony.pF5212[cIdx+1], MONY_BUF_SIZE );
//		nRate1 = (*m_pSnHeadData)->SVsper[1];
//	}
//	else if( (9<=rIdx) && (rIdx<=10) ){
//		memmove( val1, m_Fh5Mony.pF5215[cIdx+1], MONY_BUF_SIZE );
//		memmove( valttl, m_Fh5Mony.pF5212[cIdx+1], MONY_BUF_SIZE );
//		nRate1 = (*m_pSnHeadData)->SVsper[2];
//	}
//	else if( rIdx == 11 ){
//		memmove( val1, m_Fh5Mony.pF5216[cIdx+1], MONY_BUF_SIZE );
//		memmove( valttl, m_Fh5Mony.pF5212[cIdx+1], MONY_BUF_SIZE );
//		nRate1 = (*m_pSnHeadData)->SVsper[3];
//	}
//---------------
	int tmpIdx=0;
	if( cIdx==3 ){	// 合計列
		tmpIdx = 0;
	}
	else{			// 各税率列
		tmpIdx = cIdx+1;
	}
	if( (2<=rIdx) && (rIdx<=5) ){
		memmove( val1, m_Fh5Mony.pF5213[tmpIdx], MONY_BUF_SIZE );
		memmove( valttl, m_Fh5Mony.pF5212[tmpIdx], MONY_BUF_SIZE );
		nRate1 = (*m_pSnHeadData)->SVsper[0];
	}
	else if( (6<=rIdx) && (rIdx<=8) ){
		memmove( val1, m_Fh5Mony.pF5214[tmpIdx], MONY_BUF_SIZE );
		memmove( valttl, m_Fh5Mony.pF5212[tmpIdx], MONY_BUF_SIZE );
		nRate1 = (*m_pSnHeadData)->SVsper[1];
	}
	else if( (9<=rIdx) && (rIdx<=10) ){
		memmove( val1, m_Fh5Mony.pF5215[tmpIdx], MONY_BUF_SIZE );
		memmove( valttl, m_Fh5Mony.pF5212[tmpIdx], MONY_BUF_SIZE );
		nRate1 = (*m_pSnHeadData)->SVsper[2];
	}
	else if( rIdx == 11 ){
		memmove( val1, m_Fh5Mony.pF5216[tmpIdx], MONY_BUF_SIZE );
		memmove( valttl, m_Fh5Mony.pF5212[tmpIdx], MONY_BUF_SIZE );
		nRate1 = (*m_pSnHeadData)->SVsper[3];
	}
//---------------
	nRate1 = 1000 - nRate1;

	// 割合２
	if( rIdx==2 ){
		nRate2 = (*m_pSnHeadData)->SVsper[1];
	}
	else if( (rIdx==3) || (rIdx==6) ){
		nRate2 = (*m_pSnHeadData)->SVsper[2];
	}
	else if( (rIdx==4) || (rIdx==7) || (rIdx==9) ){
		nRate2 = (*m_pSnHeadData)->SVsper[3];
	}
	else if( (rIdx==5) || (rIdx==8) || (rIdx==10) || (rIdx==11) ){
		nRate2 = (*m_pSnHeadData)->SVsper[4];
	}
	nRate2 = 1000 - nRate2;

	// 割合の変換
	char	buf[64]={0};
	sprintf_s( buf, sizeof(buf), _T("%d"), nRate1 );
	m_pArith->l_input( rate1, buf );
	sprintf_s( buf, sizeof(buf), _T("%d"), nRate2 );
	m_pArith->l_input( rate2, buf );

	// 取得
	memmove( pVal1, val1, MONY_BUF_SIZE );
	memmove( pValttl, valttl, MONY_BUF_SIZE );
	memmove( pRate1, rate1, MONY_BUF_SIZE );
	memmove( pRate2, rate2, MONY_BUF_SIZE );
}

//-----------------------------------------------------------------------------
// １フィールドの控除対象仕入税額の計算(特例計算：2種類で75%以上)
//-----------------------------------------------------------------------------
// 引数	rIdx	：	行インデックス	(0:⑱, 1:⑲, 2～11:⑳～29)
//		cIdx	：	列インデックス	(0:3%, 1:5%, 2:8%)
//		pVal	：	計算結果金額の取得先
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_one_skj_tk75over( int rIdx, int cIdx, char *pVal )
{
	char			val1[MONY_BUF_SIZE]={0}, val2[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, WORK0[MONY_BUF_SIZE]={0};
//--> '15.04.04 CUT START
//	char			*VAL;
//<-- '15.04.04 CUT END
	char			nume1[MONY_BUF_SIZE]={0}, nume2[MONY_BUF_SIZE]={0}, rate1[MONY_BUF_SIZE]={0}, rate2[MONY_BUF_SIZE]={0};

	int				calqSw = 0;
	int				chkFlg = 0x08;

//--> '15.04.04 CUT START
//	unsigned short	per1, per2;
//<-- '15.04.04 CUT END


	f52calq_get_clqfield( rIdx, cIdx, nume1, nume2, rate1, rate2 );

	// 金額発生チェック
	char	sagaku[MONY_BUF_SIZE]={0};
	memmove( val1, nume1, MONY_BUF_SIZE );
	m_pArith->l_sub( sagaku, nume2, nume1 );
	memmove( val2, sagaku, MONY_BUF_SIZE );

	m_pArith->l_input( PW0, _T("1000") );
	if( m_pArith->l_test(val1) ){
		m_Util.l_6calq( val1, val1, rate1, PW0 );
	}
	if( m_pArith->l_test(val2) ){
		m_Util.l_6calq( val2, val2, rate2, PW0 );
	}
	m_pArith->l_add( val1, val1, val2 );

	if( m_pArith->l_test(val1) ){
		if( cIdx==0 ){
			f52calq_skj_tk( WORK0, F524A, nume1, rate1, sagaku, rate2, nume2 );
		}
		else if( cIdx==1 ){
			f52calq_skj_tk( WORK0, F524B, nume1, rate1, sagaku, rate2, nume2 );
		}
		else if( cIdx==2 ){
			f52calq_skj_tk( WORK0, F524C, nume1, rate1, sagaku, rate2, nume2 );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			f52calq_skj_tk( WORK0, F524T, nume1, rate1, sagaku, rate2, nume2 );
		}
//<-- '14.10.17 INS END
	}
	else{
		memset( WORK0, '\0', MONY_BUF_SIZE );
	}

	// 金額取得
	memmove( pVal, WORK0, MONY_BUF_SIZE );

//--> '14.10.17 INS START
	BOOL	isAddtype = TRUE;
	if( judge_skj_calq() == 1 ){
		isAddtype = FALSE;
	}
//<-- '14.10.17 INS END

	// 保存金額更新
	if( rIdx == 2 ){
		if( cIdx == 0 ){
			memmove( F5220A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5220B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5220C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5220T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5220T, F5220A, F5220B );
//		m_pArith->l_add( F5220T, F5220T, F5220C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5220T, F5220A, F5220B );
			m_pArith->l_add( F5220T, F5220T, F5220C );
		}
//---------------
	}
	else if( rIdx == 3 ){
		if( cIdx == 0 ){
			memmove( F5221A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5221B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5221C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5221T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5221T, F5221A, F5221B );
//		m_pArith->l_add( F5221T, F5221T, F5221C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5221T, F5221A, F5221B );
			m_pArith->l_add( F5221T, F5221T, F5221C );
		}
//---------------
	}
	else if( rIdx == 4 ){
		if( cIdx == 0 ){
			memmove( F5222A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5222B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5222C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5222T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5222T, F5222A, F5222B );
//		m_pArith->l_add( F5222T, F5222T, F5222C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5222T, F5222A, F5222B );
			m_pArith->l_add( F5222T, F5222T, F5222C );
		}
//---------------
	}
	else if( rIdx == 5 ){
		if( cIdx == 0 ){
			memmove( F5223A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5223B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5223C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5223T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5223T, F5223A, F5223B );
//		m_pArith->l_add( F5223T, F5223T, F5223C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5223T, F5223A, F5223B );
			m_pArith->l_add( F5223T, F5223T, F5223C );
		}
//---------------
	}
	else if( rIdx == 6 ){
		if( cIdx == 0 ){
			memmove( F5224A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5224B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5224C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5224T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5224T, F5224A, F5224B );
//		m_pArith->l_add( F5224T, F5224T, F5224C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5224T, F5224A, F5224B );
			m_pArith->l_add( F5224T, F5224T, F5224C );
		}
//---------------
	}
	else if( rIdx == 7 ){
		if( cIdx == 0 ){
			memmove( F5225A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5225B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5225C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5225T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5225T, F5225A, F5225B );
//		m_pArith->l_add( F5225T, F5225T, F5225C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5225T, F5225A, F5225B );
			m_pArith->l_add( F5225T, F5225T, F5225C );
		}
//---------------
	}
	else if( rIdx == 8 ){
		if( cIdx == 0 ){
			memmove( F5226A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5226B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5226C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5226T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5226T, F5226A, F5226B );
//		m_pArith->l_add( F5226T, F5226T, F5226C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5226T, F5226A, F5226B );
			m_pArith->l_add( F5226T, F5226T, F5226C );
		}
//---------------
	}
	else if( rIdx == 9 ){
		if( cIdx == 0 ){
			memmove( F5227A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5227B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5227C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5227T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5227T, F5227A, F5227B );
//		m_pArith->l_add( F5227T, F5227T, F5227C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5227T, F5227A, F5227B );
			m_pArith->l_add( F5227T, F5227T, F5227C );
		}
//---------------
	}
	else if( rIdx == 10 ){
		if( cIdx == 0 ){
			memmove( F5228A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5228B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5228C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5228T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5228T, F5228A, F5228B );
//		m_pArith->l_add( F5228T, F5228T, F5228C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5228T, F5228A, F5228B );
			m_pArith->l_add( F5228T, F5228T, F5228C );
		}
//---------------
	}
	else if( rIdx == 11 ){
		if( cIdx == 0 ){
			memmove( F5229A, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 1 ){
			memmove( F5229B, WORK0, MONY_BUF_SIZE );
		}
		else if( cIdx == 2 ){
			memmove( F5229C, WORK0, MONY_BUF_SIZE );
		}
//--> '14.10.17 INS START
		else if( cIdx == 3 ){
			memmove( F5229T, WORK0, MONY_BUF_SIZE );
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		m_pArith->l_add( F5229T, F5229A, F5229B );
//		m_pArith->l_add( F5229T, F5229T, F5229C );
//---------------
		if( isAddtype ){
			m_pArith->l_add( F5229T, F5229A, F5229B );
			m_pArith->l_add( F5229T, F5229T, F5229C );
		}
//---------------
	}
}

//-----------------------------------------------------------------------------
// １フィールドの控除対象仕入税額の計算
//-----------------------------------------------------------------------------
// 引数	rIdx	：	行インデックス	(0:⑱, 1:⑲, 2～11:⑳～29)
//		cIdx	：	列インデックス	(0:3%, 1:5%, 2:8%, 3:合計)
//		pVal	：	計算結果金額の取得先
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_one_skj( int rIdx, int cIdx, char *pVal )
{
	if( rIdx == 0 ){					// 原則計算
		f52calq_one_skj_gen( rIdx, cIdx );
	}
	else if( rIdx == 1 ){				// 特例計算(1種類で75%以上)
		f52calq_one_skj_one_tk75over( rIdx, cIdx );
	}
	else if( (2<=rIdx) && (rIdx<=11) ){	// 特例計算(2種類で75%以上)
		f52calq_one_skj_tk75over( rIdx, cIdx, pVal );
	}
}

//-----------------------------------------------------------------------------
// １フィールドの控除対象仕入税額の計算(特例計算：2種類で75%以上)
//-----------------------------------------------------------------------------
// 引数	rIdx	：	行インデックス	(0:⑱, 1:⑲, 2～11:⑳～29)
//		cIdx	：	列インデックス	(0:3%, 1:5%, 2:8%)
//-----------------------------------------------------------------------------
void CShinFhyo52::f52calq_one_skj_one_tk75over( int rIdx, int cIdx )
{
	unsigned short	per1=0;
	if( (m_tkSw&0xff) == 0x01 ){
		per1 = (*m_pSnHeadData)->SVsper[0];
	}
	else if( (m_tkSw&0xff) == 0x02 ){
		per1 = (*m_pSnHeadData)->SVsper[1];
	}
	else if( (m_tkSw&0xff) == 0x04 ){
		per1 = (*m_pSnHeadData)->SVsper[2];
	}
	else if( (m_tkSw&0xff) == 0x08 ){
		per1 = (*m_pSnHeadData)->SVsper[3];
	}
	else if( (m_tkSw&0xff) == 0x10 ){
		per1 = (*m_pSnHeadData)->SVsper[4];
	}
	else{
		return;
	}

	// 単一事業で75%以上
	char	WORK0[MONY_BUF_SIZE]={0};
	char	WORK1[MONY_BUF_SIZE]={0};
	char	PW0[MONY_BUF_SIZE]={0}, val1[MONY_BUF_SIZE]={0}, val2[MONY_BUF_SIZE]={0};
	char	*pValSrc=NULL, *pValDst=NULL;
	int		chkFlg=0x08;
	char	strBuf[64]={0};	// '15.02.23

	m_pArith->l_input( PW0, _T("1000") );
	if( cIdx==0 ){
		pValDst = F5219A;
		pValSrc = F524A;
	}
	else if( cIdx==1 ){
		pValDst = F5219B;
		pValSrc = F524B;
		chkFlg<<=4;
	}
	else if( cIdx==2 ){
		pValDst = F5219C;
		pValSrc = F524C;
		chkFlg<<=8;
	}
//--> '14.10.17 INS START
	else if( cIdx==3 ){
		pValDst = F5219T;
		pValSrc = F524T;
		chkFlg<<=12;
	}
//<-- '14.10.17 INS END
	else{
		return;
	}

	if( m_pArith->l_test(pValSrc) ){
		memset( WORK1, '\0', MONY_BUF_SIZE );
//-- '15.02.23 --
//		sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
//		m_pArith->l_input( WORK1, WORK0 );
//---------------
		sprintf_s( strBuf, sizeof( strBuf ), _T("%d"), (1000-per1) );
		m_pArith->l_input( WORK1, strBuf );
//---------------
		if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
/*- '14.06.16 -*/
//			if( !(F5219c&chkFlg) ){
//				m_Util.l_6calq( pValDst, pValSrc, WORK1, PW0 );
//			}
/*-------------*/
			m_Util.l_6calq( pValDst, pValSrc, WORK1, PW0 );
/*-------------*/
		}
	}
	else{
		// 元が０円の時には手入力を許さない
		if( (*m_pSnHeadData)->IsDataConnected() == FALSE ){
/*- '14.06.16 -*/
//			if( (F5219c&chkFlg) ){
//				//F5219c &= ~chkFlg;
//				;
//			}
//			else{
//				memset( WORK1, '\0', MONY_BUF_SIZE );
//				sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
//				m_pArith->l_input( WORK1, WORK0 );
//				m_Util.l_6calq( pValDst, pValSrc, WORK1, PW0 );
//			}
/*-------------*/
			memset( WORK1, '\0', MONY_BUF_SIZE );
//-- '15.02.23 --
//			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), (1000-per1) );
//			m_pArith->l_input( WORK1, WORK0 );
//---------------
			sprintf_s( strBuf, sizeof( strBuf ), _T("%d"), (1000-per1) );
			m_pArith->l_input( WORK1, strBuf );
//---------------
			m_Util.l_6calq( pValDst, pValSrc, WORK1, PW0 );
/*-------------*/
		}
	}

//-- '14.10.17 --
//	// 横計
//	m_pArith->l_add( F5219T, F5219A, F5219B );
//	m_pArith->l_add( F5219T, F5219T, F5219C );
//---------------
	// 横計
	if( judge_skj_calq() == 0 ){
		m_pArith->l_add( F5219T, F5219A, F5219B );
		m_pArith->l_add( F5219T, F5219T, F5219C );
	}
//---------------
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算方法の判定	('14.12.18)
//-----------------------------------------------------------------------------
// 返送値	1	：	D列は別計算
//			0	：	D列はA+B+C
//-----------------------------------------------------------------------------
int CShinFhyo52::judge_skj_calq()
{
	// 貸倒回収あり？
	if( m_pArith->l_test(F522A) ){
		return 1;
	}
	if( m_pArith->l_test(F522B) ){
		return 1;
	}
	if( m_pArith->l_test(F522C) ){
		return 1;
	}

	// 税額にマイナス？
	if( m_pArith->l_test(F5213A) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5213B) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5213C) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5214A) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5214B) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5214C) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5215A) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5215B) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5215C) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5216A) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5216B) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5216C) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5217A) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5217B) < 0 ){
		return 1;
	}
	if( m_pArith->l_test(F5217C) < 0 ){
		return 1;
	}

	return 0;
}


