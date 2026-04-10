#include "StdAfx.h"
#include "H26SnRefundData.h"
#include "RefundDataDef.h"

const	int	REFMAX		=	50;		// 還付申告次葉最大登録数
const	int	REFUND_KEY	=	384;	// キーのオフセット

const	int	SKJMAX		=	10;
const	int	HJNOTH		=	4;		// 法人用その他
const	int	KJNOTH		=	2;		// 個人用その他
const	int	HJKTNN		=	6;		// 法人用その他
const	int	KJKTMN		=	3;		// 個人用その他
const	int	HJKOTH		=	8;		// 還付付表

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH26SnRefundData::CH26SnRefundData()
{
	memset( &JotTbl, '\0', sizeof(JotTbl) );
	memset( &ExpTbl, '\0', sizeof(ExpTbl) );
	memset( &TnaTbl, '\0', sizeof(TnaTbl) );
	memset( &KotTbl, '\0', sizeof(KotTbl) );
	JotTbl.tp_tno = -1;
	ExpTbl.tp_tno = -1;
	TnaTbl.tp_tno = -1;
	KotTbl.tp_tno = -1;

	m_Arith.l_defn( 0x16 );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH26SnRefundData::~CH26SnRefundData()
{
	if( JotTbl.tp_tno != -1 ){
		m_TblHdl.th_close( JotTbl.tp_tno );
	}
	if( ExpTbl.tp_tno != -1 ){
		m_TblHdl.th_close( ExpTbl.tp_tno );
	}
	if( TnaTbl.tp_tno != -1 ){
		m_TblHdl.th_close( TnaTbl.tp_tno );
	}
	if( KotTbl.tp_tno != -1 ){
		m_TblHdl.th_close( KotTbl.tp_tno );
	}
}

//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		pSnHeadData	：	
//		sn_seq		：
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH26SnRefundData::GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	int				thd;
	unsigned int	rsiz, rcnt;
	unsigned long	tsiz;
	int				Max, HnMx, Cnt, Putno, Seqno;

	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;
	
	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
//		errmes = _T("還付申告の明細書マスターが取得できませんできた。");
		return ERR;
	}
	// 管理部オープン
	filter.Format( "KPSequ = %d", sn_seq );
	if( pDBSyzAcc->ReFundVolOpen( pDBNpSub->m_database, filter ) == ERR ){
//		errmes =  pDBSyzAcc->GetReFundVolErrMsg();
		delete( pDBSyzAcc );
		return( ERR );
	}
	// 明細（次葉）部オープン
	if( pDBSyzAcc->ReFundRecOpen(pDBNpSub->m_database, filter) == ERR ){
//		errmes =  pDBSyzAcc->GetReFundVolErrMsg();
		pDBSyzAcc->ReFundVolClose();
		delete pDBSyzAcc;
		return ERR;
	}

	// 初期設定
	memset( RefundJot, '\0', sizeof( REFUNDREC )*10 );
	memset( RefundExp, '\0', sizeof( REFUNDREC )*10 );
	memset( RefundStk, '\0', sizeof( REFUNDREC )*20 );
	memset( RefundTna, '\0', sizeof( REFUNDREC )*10 );
	memset( RefundKot, '\0', sizeof( REFUNDREC )*10 );

	// 主な課税資産の譲渡等
	rsiz = (unsigned int)sizeof( REFUNDREC );																// レコード長
	rcnt = (unsigned int)REFMAX;																			// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	// テーブル イニシャライズ
	if( JotTbl.tp_tno != ERR )	m_TblHdl.th_close( JotTbl.tp_tno );
	if ((thd = m_TblHdl.th_open( tsiz, rsiz )) < 0)			{
//		errmes =  _T("主な課税資産の譲渡等の領域を確保できませんでした。");
		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete( pDBSyzAcc );
		return( ERR );
	}
	memset( &JotTbl, '\0', sizeof(struct TB_PAR) );	
	JotTbl.tp_tno  = thd;
	JotTbl.tp_rmax = rcnt;
	JotTbl.tp_rnum = 0;
	JotTbl.tp_rlen = rsiz;
	JotTbl.tp_koff = REFUND_KEY;
	JotTbl.tp_klen = 8;		
	JotTbl.tp_dupm = 0;
	JotTbl.tp_sufm = 0;
	JotTbl.tp_cp   = 0;
	JotTbl.tp_err  = 0;
	// 主な輸出取引等の明細
	rsiz = (unsigned int)sizeof( REFUNDREC );										// レコード長
	rcnt = (unsigned int)REFMAX;													// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	// テーブル イニシャライズ
	if( ExpTbl.tp_tno != ERR )	m_TblHdl.th_close( ExpTbl.tp_tno );
	if ((thd = m_TblHdl.th_open( tsiz, rsiz )) < 0)			{
//		errmes =  _T("主な輸出取引等の明細の領域を確保できませんでした。");
		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete( pDBSyzAcc );
		return( ERR );
	}
	memset( &ExpTbl, '\0', sizeof(struct TB_PAR) );	
	ExpTbl.tp_tno  = thd;
	ExpTbl.tp_rmax = rcnt;
	ExpTbl.tp_rnum = 0;
	ExpTbl.tp_rlen = rsiz;
	ExpTbl.tp_koff = REFUND_KEY;
	ExpTbl.tp_klen = 8;
	ExpTbl.tp_dupm = 0;
	ExpTbl.tp_sufm = 0;
	ExpTbl.tp_cp   = 0;
	ExpTbl.tp_err  = 0;
	// 主な棚卸資産・原材料等の取得
	rsiz = (unsigned int)sizeof( REFUNDREC );										// レコード長
	rcnt = (unsigned int)REFMAX;													// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	// テーブル イニシャライズ
	if( TnaTbl.tp_tno != ERR )	m_TblHdl.th_close( TnaTbl.tp_tno );
	if ((thd = m_TblHdl.th_open( tsiz, rsiz )) < 0)			{
//		errmes =  _T("主な棚卸資産・原材料等の取得の領域を確保できませんでした。");
		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete( pDBSyzAcc );
		return( ERR );
	}
	memset( &TnaTbl, '\0', sizeof(struct TB_PAR) );	
	TnaTbl.tp_tno  = thd;
	TnaTbl.tp_rmax = rcnt;
	TnaTbl.tp_rnum = 0;
	TnaTbl.tp_rlen = rsiz;
	TnaTbl.tp_koff = REFUND_KEY;
	TnaTbl.tp_klen = 8;	
	TnaTbl.tp_dupm = 0;
	TnaTbl.tp_sufm = 0;
	TnaTbl.tp_cp   = 0;
	TnaTbl.tp_err  = 0;
	// 主な固定資産等の取得
	rsiz = (unsigned int)sizeof( REFUNDREC );										// レコード長
	rcnt = (unsigned int)REFMAX;													// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	// テーブル イニシャライズ
	if( KotTbl.tp_tno != ERR )	m_TblHdl.th_close( KotTbl.tp_tno );
	if ((thd = m_TblHdl.th_open( tsiz, rsiz )) < 0)			{
//		errmes =  _T("主な固定資産等の取得の領域を確保できませんでした。");
		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete( pDBSyzAcc );
		return( ERR );
	}
	memset( &KotTbl, '\0', sizeof(struct TB_PAR) );	
	KotTbl.tp_tno  = thd;
	KotTbl.tp_rmax = rcnt;
	KotTbl.tp_rnum = 0;
	KotTbl.tp_rlen = rsiz;
	KotTbl.tp_koff = REFUND_KEY;
	KotTbl.tp_klen = 8;	
	KotTbl.tp_dupm = 0;
	KotTbl.tp_sufm = 0;
	KotTbl.tp_cp   = 0;
	KotTbl.tp_err  = 0;

	// 管理部の読み込み
	if( pDBSyzAcc->m_pRefundvol->GetRecordCount() == 0 )	{
		REFUNDVOL &RefundVol = CRefundBaseDialog::refundvol;
		RefundVol.KPEkoz[0] = (unsigned char )0xff;
		RefundVol.KPEbsg = 5;
		RefundVol.KPEssg = 5;
		RefundVol.KPKuck = 0x01;													// 取引金額など(2-1)	D0:OFF印刷なし　ON:印刷あり
		RefundVol.KPTsck = 0x01;													// 取引金額など(3-2)	D0:OFF印刷なし　ON:印刷あり
		RefundVol.KPKsck = 0x01;													// 取引金額など(3-3)	D0:OFF印刷なし　ON:印刷あり
		if( !(pDBNpSub->zvol->s_sgn1&0xc0) )	{
			RefundVol.KPKusg = 0x01;												// 税抜・税込(2-1)		D0:OFF税抜　ON税込
			RefundVol.KPSsgn = 0x11;												// 税抜・税込(3-1)		D0:OFF税抜  ON税込(損益) D4:OFF税抜  ON税込(資産)
			RefundVol.KPTsgn = 0x01;												// 税抜・税込(3-2)
			RefundVol.KPKsgn = 0x01;												// 税抜・税込(3-3)
		}
		char	yy, mm, dd;
		yy = mm = dd = 0;
		pSnHeadData->GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
		RefundVol.KPSnen = yy;

		if( !(pSnHeadData->Sn_KOJIN & 0x01) )	RefundVol.KPType = ID_ICSSH_REF_HOJIN;
		else									RefundVol.KPType = ID_ICSSH_REF_KOJIN;	

		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete( pDBSyzAcc );

		return( 0 );
	}
	if( pDBSyzAcc->m_pRefundvol->MoveFirst() == ERR )	{
//		errmes =  pDBSyzAcc->GetReFundVolErrMsg();
		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete( pDBSyzAcc );
		return( ERR );
	}
	// 管理部変換
	
	REFUNDVOL &RefundVol = CRefundBaseDialog::refundvol;
	if( !(pSnHeadData->Sn_KOJIN & 0x01) )	RefundVol.KPType = ID_ICSSH_REF_HOJIN;
	else									RefundVol.KPType = ID_ICSSH_REF_KOJIN;								// １還付申告となった主な理由
	RefundVol.KPRsgn = pDBSyzAcc->m_pRefundvol->KPRsgn;															// 該当する事項 D0:輸出D1:設備D2:その他
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPRrup, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPRrup, 50 );	// その他の理由（上段）
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPRrlw, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPRrlw, 50 );	// 〃　　　　　（下段）
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPRrdm, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPRrdm, 50 );	// 三段目用ダミー
																												// ２課税売上等に係る事項
																												// (1)主な課税資産の譲渡等
	RefundVol.KPKuck = pDBSyzAcc->m_pRefundvol->KPKuck;															// 取引金額など D0:OFF　ON 
	RefundVol.KPKusg = pDBSyzAcc->m_pRefundvol->KPKusg;															// 税込／税抜 D0:OFF税抜　ON:税込
																												// (2)主な輸出取引等の明細
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEbnk, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEbnk, 20 );	// 銀行名  2-2輸出取引の明細
	RefundVol.KPEbsg = pDBSyzAcc->m_pRefundvol->KPEbsg;															// 1.銀行2.金庫3.組合4.農協5.漁協
	RefundVol.KPEbdm = pDBSyzAcc->m_pRefundvol->KPEbdm;															// 0.銀行　1.ゆうちょ銀行
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEstn, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEstn, 20 );	// 支店名
	RefundVol.KPEssg = pDBSyzAcc->m_pRefundvol->KPEssg;															// 1.本店2.支店3.本社4.支社
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEykn, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEykn, 8 );	// 預金種別
	m_Util.asci_to_bcd( (unsigned char*)RefundVol.KPEkoz, &pDBSyzAcc->m_pRefundvol->KPEkoz, 10 );				// 口座番号
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEyn1, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEyn1, 5 );	// 郵便貯金番号①
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEyn2, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEyn2, 8 );	// 郵便貯金番号②
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEsup, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEsup, 40 );	// 主な通関業者　氏名（上段）
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEslw, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEslw, 40 );	// 　　　　　　　　　（下段）
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEaup, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEaup, 40 );	// 主な通関業者　住所（上段）
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPEalw, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPEalw, 40 );	// 　　　　　　　　　（下段）
																												// ３課税仕入れに係る事項
																												// (1)仕入金額等の明細
	RefundVol.KPSsgn = pDBSyzAcc->m_pRefundvol->KPSsgn;															// 税込／税抜 D0:OFF税抜　ON:税込(決算額) D4:OFF税抜 ON税込（資産）
	RefundVol.KPSstx = pDBSyzAcc->m_pRefundvol->KPSsta;															// 仕入控除税額手入力サイン D0 : ON.手入力
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSnam, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSnam, 16 );	// 所得名称
																												// (2)主な棚卸資産・原材料等の取得
	RefundVol.KPTsck = pDBSyzAcc->m_pRefundvol->KPTsck;															// 取引金額など D0:OFF　ON 
	RefundVol.KPTsgn = pDBSyzAcc->m_pRefundvol->KPTsgn;															// 税込／税抜 D0:OFF税抜　ON:税込
																												// (3)主な固定資産等の取得
	RefundVol.KPKsck = pDBSyzAcc->m_pRefundvol->KPKsck;															// 取引金額など D0:OFF　ON 
	RefundVol.KPKsgn = pDBSyzAcc->m_pRefundvol->KPKsgn;															// 税込／税抜 D0:OFF税抜　ON:税込
																												// ４特殊事情
	RefundVol.KPSnen = pDBSyzAcc->m_pRefundvol->KPSnen;															// 特殊事情用 年分 (個人のみ)
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSpn1, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSpn1, 120 ); 	// 特殊事情　1行目
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSpn2, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSpn2, 120 ); 	// 特殊事情　2行目
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSpn3, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSpn3, 120 ); 	// 特殊事情　3行目
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSpn4, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSpn4, 120 ); 	// 特殊事情　4行目
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSpn5, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSpn5, 120 ); 	// 特殊事情　5行目
	m_Util.cstring_to_char( (unsigned char*)RefundVol.KPSpn6, (LPCTSTR)pDBSyzAcc->m_pRefundvol->KPSpn6, 120 ); 	// 特殊事情　6行目
	// 仕入金額等の明細
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 13;
	}
	else{
		HnMx = 10;
	}
	if( (Max = pDBSyzAcc->m_pRefundrec->GetRecordCount()) != 0 )	{
		Cnt = 0;
		while( 1 ) {
			if( !Cnt )	{
				if( pDBSyzAcc->m_pRefundrec->MoveFirst() == ERR )	{
//					errmes =  pDBSyzAcc->GetReFundVolErrMsg();
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
			}
			else	{
				if( pDBSyzAcc->m_pRefundrec->MoveNext() == ERR )	break;
			}
			Cnt++;
			if( pDBSyzAcc->m_pRefundrec->KPItem != ID_ICSSH_REF_STOCK_TYPE )	continue;
			// 管理部へ変換
			Putno = pDBSyzAcc->m_pRefundrec->KPLine - 1;			
			m_Util.val_to_bin( (unsigned char*)RefundStk[Putno].KPSvl1, pDBSyzAcc->m_pRefundrec->KPSvl1 );
			m_Util.val_to_bin( (unsigned char*)RefundStk[Putno].KPSvl2, pDBSyzAcc->m_pRefundrec->KPSvl2 );
			m_Util.val_to_bin( (unsigned char*)RefundStk[Putno].KPSvl3, pDBSyzAcc->m_pRefundrec->KPSvl3 );
			m_Util.val_to_bin( (unsigned char*)RefundStk[Putno].KPSvl4, pDBSyzAcc->m_pRefundrec->KPSvl4 );
		}
	}

	// 主な課税資産の譲渡等
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	if( (Max = pDBSyzAcc->m_pRefundrec->GetRecordCount()) != 0 )	{
		Cnt = 0;
		Seqno = 1;
		while( 1 ) {
			if( !Cnt )	{
				if( pDBSyzAcc->m_pRefundrec->MoveFirst() == ERR )	{
//					errmes =  pDBSyzAcc->GetReFundVolErrMsg();
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
			}
			else	{
				if( pDBSyzAcc->m_pRefundrec->MoveNext() == ERR )	break;
			}
			Cnt++;
			if( pDBSyzAcc->m_pRefundrec->KPItem != ID_ICSSH_REF_FORTUNE_TYPE )	continue;
			Putno = pDBSyzAcc->m_pRefundrec->KPLine;			
			// 変換
			memset( &RefundRec, '\0', sizeof( REFUNDREC ) );
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSsup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSsup, 16 ); 	// 資産の種類等（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSslw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSslw, 16 ); 	// 〃　　　　　（下段）

			//m_Util.date_to_bcd( 0x04, (unsigned char*)RefundRec.KPSday, pDBSyzAcc->m_pRefundrec->KPSday );				// 譲渡年月日
			CVolDateDB	voldate;
			int yymmdd=0, dummy;
			if( pDBSyzAcc->m_pRefundrec->KPSday != 0 ){
				voldate.db_datecnvGen( 0, pDBSyzAcc->m_pRefundrec->KPSday, &dummy, &yymmdd, 0, 0 );
				m_Arith.int_bcd( &RefundRec.KPSday, yymmdd, 3 );
			}

			RefundRec.KPSdys = pDBSyzAcc->m_pRefundrec->KPSdys;													// 〃　　　　　継続サイン
			m_Util.val_to_bin( (unsigned char*)RefundRec.KPSval, pDBSyzAcc->m_pRefundrec->KPSval );					// 取引金額
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnup, 20 ); 	// 取引先名（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnlw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnlw, 20 ); 	// 〃　　　（下段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSaup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSaup, 30 ); 	// 取引先住所（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSalw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSalw, 30 ); 	// 〃　　　　（下段）
			if( Putno <= HnMx )	{
				// 管理部へ変換
				memmove( &RefundJot[Putno-1], &RefundRec, sizeof( REFUNDREC ) );
			}
			else	{
				// 明細（次葉）へ変換
				RefundRec.KPSlin = Seqno;
				if( m_TblHdl.th_add( &RefundRec, &JotTbl ) )	{
//					errmes =  _T("主な課税資産の譲渡等を読み込み中に障害が発生しました。");
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
				Seqno++;
			}
		}
	}
	// 主な輸出取引等の明細
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	if( (Max = pDBSyzAcc->m_pRefundrec->GetRecordCount()) != 0 )	{
		Cnt = 0;
		Seqno = 1;
		while( 1 ) {
			if( !Cnt )	{
				if( pDBSyzAcc->m_pRefundrec->MoveFirst() == ERR )	{
//					errmes =  pDBSyzAcc->GetReFundVolErrMsg();
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
			}
			else	{
				if( pDBSyzAcc->m_pRefundrec->MoveNext() == ERR )	break;
			}
			Cnt++;
			if( pDBSyzAcc->m_pRefundrec->KPItem != ID_ICSSH_REF_EXPORT_TYPE )	continue;
			Putno = pDBSyzAcc->m_pRefundrec->KPLine;			
			// 変換
			memset( &RefundRec, '\0', sizeof( REFUNDREC ) );
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnup, 20 ); 	// 取引先名（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnlw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnlw, 20 ); 	// 〃　　　（下段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSaup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSaup, 30 ); 	// 取引先住所（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSalw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSalw, 30 ); 	// 〃　　　　（下段）
			m_Util.val_to_bin( (unsigned char*)RefundRec.KPSval, pDBSyzAcc->m_pRefundrec->KPSval );					// 取引金額
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPStup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPStup, 20 ); 	// 主な取引商品（上段）輸出取引　専用
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPStlw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPStlw, 20 ); 	// 〃　　　　　（下段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSzup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSzup, 16 ); 	// 所轄税関名　（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSzlw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSzlw, 16 ); 	// 〃　　　　　（下段）
			if( Putno <= HnMx )	{
				// 管理部へ変換
				memmove( &RefundExp[Putno-1], &RefundRec, sizeof( REFUNDREC ) );
			}
			else	{
				// 明細（次葉）へ変換
				RefundRec.KPSlin = Seqno;
				if( m_TblHdl.th_add( &RefundRec, &ExpTbl ) )	{
//					errmes =  _T("主な輸出取引等の明細を読み込み中に障害が発生しました。");
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
				Seqno++;
			}
		}
	}
	// 主な棚卸資産・原材料等の取得
	HnMx = 5;
	if( (Max = pDBSyzAcc->m_pRefundrec->GetRecordCount()) != 0 )	{
		Cnt = 0;
		Seqno = 1;
		while( 1 ) {
			if( !Cnt )	{
				if( pDBSyzAcc->m_pRefundrec->MoveFirst() == ERR )	{
//					errmes =  pDBSyzAcc->GetReFundVolErrMsg();
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
			}
			else	{
				if( pDBSyzAcc->m_pRefundrec->MoveNext() == ERR )	break;
			}
			Cnt++;
			if( pDBSyzAcc->m_pRefundrec->KPItem != ID_ICSSH_REF_INVENT_TYPE )	continue;
			Putno = pDBSyzAcc->m_pRefundrec->KPLine;			
			// 変換
			memset( &RefundRec, '\0', sizeof( REFUNDREC ) );
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSsup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSsup, 16 ); 	// 資産の種類等（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSslw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSslw, 16 ); 	// 〃　　　　　（下段）

			//m_Util.date_to_bcd( 0x04, (unsigned char*)RefundRec.KPSday, pDBSyzAcc->m_pRefundrec->KPSday );				// 譲渡年月日
			CVolDateDB	voldate;
			int yymmdd=0, dummy;
			if( pDBSyzAcc->m_pRefundrec->KPSday != 0 ){
				voldate.db_datecnvGen( 0, pDBSyzAcc->m_pRefundrec->KPSday, &dummy, &yymmdd, 0, 0 );
				m_Arith.int_bcd( &RefundRec.KPSday, yymmdd, 3 );
			}

			RefundRec.KPSdys = pDBSyzAcc->m_pRefundrec->KPSdys;													// 〃　　　　　継続サイン
			m_Util.val_to_bin( (unsigned char*)RefundRec.KPSval, pDBSyzAcc->m_pRefundrec->KPSval );					// 取引金額
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnup, 20 ); 	// 取引先名（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnlw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnlw, 20 ); 	// 〃　　　（下段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSaup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSaup, 30 ); 	// 取引先住所（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSalw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSalw, 30 ); 	// 〃　　　　（下段）
			if( Putno <= HnMx )	{
				// 管理部へ変換
				memmove( &RefundTna[Putno-1], &RefundRec, sizeof( REFUNDREC ) );
			}
			else	{
				// 明細（次葉）へ変換
				RefundRec.KPSlin = Seqno;
				if( m_TblHdl.th_add( &RefundRec, &TnaTbl ) )	{
//					errmes =  _T("主な棚卸資産・原材料等の取得を読み込み中に障害が発生しました。");
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
				Seqno++;
			}
		}
	}

	// 主な固定資産等の取得
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	if( (Max = pDBSyzAcc->m_pRefundrec->GetRecordCount()) != 0 )	{
		Cnt = 0;
		Seqno = 1;
		while( 1 ) {
			if( !Cnt )	{
				if( pDBSyzAcc->m_pRefundrec->MoveFirst() == ERR )	{
//					errmes =  pDBSyzAcc->GetReFundVolErrMsg();
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
			}
			else	{
				if( pDBSyzAcc->m_pRefundrec->MoveNext() == ERR )	break;
			}
			Cnt++;
			if( pDBSyzAcc->m_pRefundrec->KPItem != ID_ICSSH_REF_FIXED_TYPE )	continue;
			Putno = pDBSyzAcc->m_pRefundrec->KPLine;			
			// 変換
			memset( &RefundRec, '\0', sizeof( REFUNDREC ) );
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSsup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSsup, 16 ); 	// 資産の種類等（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSslw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSslw, 16 ); 	// 〃　　　　　（下段）

			//m_Util.date_to_bcd( 0x04, (unsigned char*)RefundRec.KPSday, pDBSyzAcc->m_pRefundrec->KPSday );				// 譲渡年月日
			CVolDateDB	voldate;
			int yymmdd=0, dummy;
			if( pDBSyzAcc->m_pRefundrec->KPSday != 0 ){
				voldate.db_datecnvGen( 0, pDBSyzAcc->m_pRefundrec->KPSday, &dummy, &yymmdd, 0, 0 );
				m_Arith.int_bcd( &RefundRec.KPSday, yymmdd, 3 );
			}

			RefundRec.KPSdys = pDBSyzAcc->m_pRefundrec->KPSdys;													// 〃　　　　　継続サイン
			m_Util.val_to_bin( (unsigned char*)RefundRec.KPSval, pDBSyzAcc->m_pRefundrec->KPSval );					// 取引金額
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnup, 20 ); 	// 取引先名（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSnlw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSnlw, 20 ); 	// 〃　　　（下段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSaup, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSaup, 30 ); 	// 取引先住所（上段）
			m_Util.cstring_to_char( (unsigned char*)RefundRec.KPSalw, (LPCTSTR)pDBSyzAcc->m_pRefundrec->KPSalw, 30 ); 	// 〃　　　　（下段）
			if( Putno <= HnMx )	{
				// 管理部へ変換
				memmove( &RefundKot[Putno-1], &RefundRec, sizeof( REFUNDREC ) );
			}
			else	{
				// 明細（次葉）へ変換
				RefundRec.KPSlin = Seqno;
				if( m_TblHdl.th_add( &RefundRec, &KotTbl ) )	{
//					errmes =  _T("主な固定資産等の取得を読み込み中に障害が発生しました。");
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
				Seqno++;
			}
		}
	}

	// 後始末
	pDBSyzAcc->ReFundVolClose();
	pDBSyzAcc->ReFundRecClose();
	delete( pDBSyzAcc );

	return( 0 );
}

//-----------------------------------------------------------------------------
// 還付申告 全テーブル削除
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：
//		sn_seq		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26SnRefundData::DeleteAllData( CDBNpSub *pDBNpSub, int sn_seq )
{
	if( sn_seq == -1 ){
		CString	str, sql;

		// 基本情報
		str = _T("IF OBJECTPROPERTY ( object_id('%s'),'ISTABLE') IS NOT NULL \n"
				 "BEGIN \n"
				 "DROP TABLE [dbo].[%s]\n"
				 "END\n" );
		sql.Format( str, _T("refundvol"), _T("refundvol") );
		try{
			pDBNpSub->m_database->ExecuteSQL( sql );
		}
		catch( CDBException *dbe ){
			dbe->Delete();
//			errmes = _T("消費税の還付申告に関する明細書(基本情報)マスターの削除中に障害が発生しました。");
			return ERR;
		}

		// 明細情報
		str = _T("IF OBJECTPROPERTY ( object_id('%s'),'ISTABLE') IS NOT NULL \n"
				 "BEGIN \n"
				 "DROP TABLE [dbo].[%s]\n"
				 "END\n" );
	
		sql.Format( str, _T("refundrec"), _T("refundrec") );
		try{
			pDBNpSub->m_database->ExecuteSQL( sql );
		}
		catch( CDBException *dbe ){
			dbe->Delete();
//			errmes = _T("消費税の還付申告に関する明細書(明細情報)マスターの削除中に障害が発生しました。");
			return ERR;
		}
	}
	else{
		CString			filter;
		CDBSyzAccess*	pDBSyzAcc;

		// 消費税拡張処理クラス
		pDBSyzAcc = NULL;
		pDBSyzAcc = new CDBSyzAccess;
		if( pDBSyzAcc == NULL )	{
//			errmes = _T("還付申告の明細書マスターが取得できませんできた。");
			return ERR;
		}
		// 管理部オープン
		filter.Format( _T("KPSequ = %d"), sn_seq );
		if( pDBSyzAcc->ReFundVolOpen( pDBNpSub->m_database, filter ) == ERR ) {
//			errmes =  pDBSyzAcc->GetReFundVolErrMsg();
			delete pDBSyzAcc;
			return ERR;
		}
		// 明細（次葉）部オープン
		if( pDBSyzAcc->ReFundRecOpen( pDBNpSub->m_database, filter ) == ERR ) {
//			errmes =  pDBSyzAcc->GetReFundVolErrMsg();
			pDBSyzAcc->ReFundVolClose();
			delete pDBSyzAcc;
			return ERR;
		}
		// 管理部
		if(	pDBSyzAcc->m_pRefundvol->GetRecordCount() )	{
			pDBSyzAcc->m_pRefundvol->MoveFirst();
			pDBSyzAcc->m_pRefundvol->Delete();
		}
		int Mct = pDBSyzAcc->m_pRefundrec->GetRecordCount();
		if( Mct ){
			// 消費税の還付申告に関する明細書の次葉マスターの取り消し
			pDBSyzAcc->m_pRefundrec->MoveFirst();
			for( int i=0; Mct!=i; i++ ){
				pDBSyzAcc->m_pRefundrec->Delete();
			}
		}
		pDBSyzAcc->ReFundVolClose();
		pDBSyzAcc->ReFundRecClose();
		delete pDBSyzAcc;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// DB へデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		pSnHeadData	：	
//		sn_seq		：
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH26SnRefundData::SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	 int	HnMx, Cnt;
	 char	Oflg = 0x00;
	 int	Eflg = 0;
	
//	m_Kojin = (Snh.Sn_KOJIN&0x01);
//	if( (Vol1.APNO&0xf0) == 0x10 )	m_Kojin = 1;

	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
//		errmes = _T("還付申告の明細書マスターが取得できませんできた。");
		return ERR;
	}
	// 管理部オープン
	filter.Format( _T("KPSequ = %d"), sn_seq );
	if( pDBSyzAcc->ReFundVolOpen( pDBNpSub->m_database, filter ) == ERR ){
//		errmes =  pDBSyzAcc->GetReFundVolErrMsg();
		delete pDBSyzAcc;
		return ERR;
	}
	// 明細（次葉）部オープン
	if( pDBSyzAcc->ReFundRecOpen( pDBNpSub->m_database, filter ) == ERR ){
//		errmes =  pDBSyzAcc->GetReFundVolErrMsg();
		pDBSyzAcc->ReFundVolClose();
		delete pDBSyzAcc;
		return ERR;
	}

	REFUNDVOL &RefundVol = CRefundBaseDialog::refundvol;
	// 管理部
	if(	pDBSyzAcc->m_pRefundvol->GetRecordCount() ){
		pDBSyzAcc->m_pRefundvol->MoveFirst();
		pDBSyzAcc->m_pRefundvol->Delete();
	}

	pDBSyzAcc->m_pRefundvol->AddNew();
	pDBSyzAcc->m_pRefundvol->KPType = RefundVol.KPType;															// 種別サイン 1=法人　2=個人 	
																												// １還付申告となった主な理由
	pDBSyzAcc->m_pRefundvol->KPRsgn = RefundVol.KPRsgn;															// 該当する事項 D0:輸出D1:設備D2:その他
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPRrup, (unsigned char*)RefundVol.KPRrup,  50 );					// その他の理由（上段）
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPRrlw, (unsigned char*)RefundVol.KPRrlw,  50 );					// 〃　　　　　（下段）
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPRrdm, (unsigned char*)RefundVol.KPRrdm,  50 );					// 三段目用ダミー
																												// ２課税売上等に係る事項
																												// (1)主な課税資産の譲渡等
	pDBSyzAcc->m_pRefundvol->KPKuck = RefundVol.KPKuck;															// 取引金額など D0:OFF　ON 
	pDBSyzAcc->m_pRefundvol->KPKusg = RefundVol.KPKusg;															// 税込／税抜 D0:OFF税抜　ON:税込
																												// (2)主な輸出取引等の明細
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEbnk, (unsigned char*)RefundVol.KPEbnk, 20 );					// 銀行名  2-2輸出取引の明細
	pDBSyzAcc->m_pRefundvol->KPEbsg = RefundVol.KPEbsg;															// 1.銀行2.金庫3.組合4.農協5.漁協
	pDBSyzAcc->m_pRefundvol->KPEbdm = RefundVol.KPEbdm;															// 0:銀行　1:ゆうちょ銀行
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEstn, (unsigned char*)RefundVol.KPEstn, 20 );					// 支店名
	RefundVol.KPEssg = pDBSyzAcc->m_pRefundvol->KPEssg = RefundVol.KPEssg;										// 1.本店2.支店3.本社4.支社
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEykn, (unsigned char*)RefundVol.KPEykn, 8 );					// 預金種別
	m_Util.bcd_to_asci( &pDBSyzAcc->m_pRefundvol->KPEkoz,(unsigned char*) RefundVol.KPEkoz, 10 );						// 口座番号
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEyn1, (unsigned char*)RefundVol.KPEyn1, 5 );					// 郵便貯金番号①
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEyn2, (unsigned char*)RefundVol.KPEyn2, 8 );					// 郵便貯金番号②
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEsup, (unsigned char*)RefundVol.KPEsup, 40 );					// 主な通関業者　氏名（上段）
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEslw, (unsigned char*)RefundVol.KPEslw, 40 );					// 　　　　　　　　　（下段）
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEaup, (unsigned char*)RefundVol.KPEaup, 40 );					// 主な通関業者　住所（上段）
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPEalw, (unsigned char*)RefundVol.KPEalw, 40 );					// 　　　　　　　　　（下段）
																												// ３課税仕入れに係る事項
																												// (1)仕入金額等の明細
	pDBSyzAcc->m_pRefundvol->KPSsgn = RefundVol.KPSsgn;															// 税込／税抜 D0:OFF税抜　ON:税込(決算額) D4:OFF税抜 ON税込（資産）
	pDBSyzAcc->m_pRefundvol->KPSsta = RefundVol.KPSstx;															// 仕入控除税額手入力サイン D0 : ON.手入力
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSnam, (unsigned char*)RefundVol.KPSnam, 16 );					// 所得名称
																												// (2)主な棚卸資産・原材料等の取得
	pDBSyzAcc->m_pRefundvol->KPTsck = RefundVol.KPTsck;															// 取引金額など D0:OFF　ON 
	pDBSyzAcc->m_pRefundvol->KPTsgn = RefundVol.KPTsgn;															// 税込／税抜 D0:OFF税抜　ON:税込
																												// (3)主な固定資産等の取得
	pDBSyzAcc->m_pRefundvol->KPKsck = RefundVol.KPKsck;															// 取引金額など D0:OFF　ON 
	pDBSyzAcc->m_pRefundvol->KPKsgn = RefundVol.KPKsgn;															// 税込／税抜 D0:OFF税抜　ON:税込
																												// ４特殊事情
	pDBSyzAcc->m_pRefundvol->KPSnen = RefundVol.KPSnen;															// 特殊事情用 年分 (個人のみ)
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSpn1, (unsigned char*)RefundVol.KPSpn1, 120 );   				// 特殊事情　1行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSpn2, (unsigned char*)RefundVol.KPSpn2, 120 );   				// 特殊事情　2行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSpn3, (unsigned char*)RefundVol.KPSpn3, 120 );   				// 特殊事情　3行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSpn4, (unsigned char*)RefundVol.KPSpn4, 120 );   				// 特殊事情　4行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSpn5, (unsigned char*)RefundVol.KPSpn5, 120 );   				// 特殊事情　5行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundvol->KPSpn6, (unsigned char*)RefundVol.KPSpn6, 120 );   				// 特殊事情　6行目

	pDBSyzAcc->m_pRefundvol->KPSequ = sn_seq;																	// 履歴番号

	pDBSyzAcc->m_pRefundvol->Update();

	int Mct = pDBSyzAcc->m_pRefundrec->GetRecordCount();
	if( Mct ){
		// 消費税の還付申告に関する明細書の次葉マスターの取り消し
		pDBSyzAcc->m_pRefundrec->MoveFirst();
		for( int i=0; Mct!=i; i++ ){
			pDBSyzAcc->m_pRefundrec->Delete();
		}
	}
	// 2-(1) 主な課税資産の譲渡等
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	for( Cnt = 0; Cnt != HnMx; Cnt++ ) {
		pDBSyzAcc->m_pRefundrec->AddNew();
		pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_FORTUNE_TYPE;			
		pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1;			
		// 変換
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSsup, (unsigned char*)RefundJot[Cnt].KPSsup, 16 ); 		// 資産の種類等（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSslw, (unsigned char*)RefundJot[Cnt].KPSslw, 16 ); 		// 〃　　　　　（下段）
		//m_Util.date_to_bin( 0, (long*)&pDBSyzAcc->m_pRefundrec->KPSday, (unsigned char*)RefundJot[Cnt].KPSday );		// 譲渡年月日
		CVolDateDB	voldate;
		int dummy;
		unsigned char yymmdd[3] = {0};
		if(( RefundJot[Cnt].KPSday[0] != 0 )&&( RefundJot[Cnt].KPSday[1] != 0 )&&( RefundJot[Cnt].KPSday[2] != 0 )){
			voldate.db_datecnvGen( 4, (unsigned char*)RefundJot[Cnt].KPSday, &dummy, yymmdd, 1, 0 );
			pDBSyzAcc->m_pRefundrec->KPSday = m_Arith.bcd_int( yymmdd, 3 );
		}

		pDBSyzAcc->m_pRefundrec->KPSdys = RefundJot[Cnt].KPSdys;												// 〃　　　　　継続サイン
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundJot[Cnt].KPSval );					// 取引金額
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundJot[Cnt].KPSnup, 20 ); 		// 取引先名（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundJot[Cnt].KPSnlw, 20 ); 		// 〃　　　（下段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundJot[Cnt].KPSaup, 30 ); 		// 取引先住所（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundJot[Cnt].KPSalw, 30 ); 		// 〃　　　　（下段）

		pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

		pDBSyzAcc->m_pRefundrec->Update();
	}

	// 2-(2) 主な輸出取引等の明細
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	for( Cnt = 0; Cnt != HnMx; Cnt++ ) {
		pDBSyzAcc->m_pRefundrec->AddNew();
		pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_EXPORT_TYPE;			
		pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1;			
		// 変換
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundExp[Cnt].KPSnup, 20 ); 		// 取引先名（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundExp[Cnt].KPSnlw, 20 ); 		// 〃　　　（下段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundExp[Cnt].KPSaup, 30 ); 		// 取引先住所（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundExp[Cnt].KPSalw, 30 ); 		// 〃　　　　（下段）
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundExp[Cnt].KPSval );					// 取引金額
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPStup, (unsigned char*)RefundExp[Cnt].KPStup, 20 ); 		// 主な取引商品（上段）輸出取引　専用
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPStlw, (unsigned char*)RefundExp[Cnt].KPStlw, 20 ); 		// 〃　　　　　（下段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSzup, (unsigned char*)RefundExp[Cnt].KPSzup, 16 ); 		// 所轄税関名　（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSzlw, (unsigned char*)RefundExp[Cnt].KPSzlw, 16 ); 		// 〃　　　　　（下段）

		pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

		pDBSyzAcc->m_pRefundrec->Update();
	}

	// 3-(1) 仕入金額等の明細
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 13;
	}
	else{
		HnMx = 10;
	}
	for( Cnt = 0; Cnt != HnMx; Cnt++ ) {
		pDBSyzAcc->m_pRefundrec->AddNew();
		pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_STOCK_TYPE;			
		pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1;			
		// 変換
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSvl1, (unsigned char*)RefundStk[Cnt].KPSvl1 );					// 仕入決算額
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSvl2, (unsigned char*)RefundStk[Cnt].KPSvl2 );					// 課税仕入にならないもの
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSvl3, (unsigned char*)RefundStk[Cnt].KPSvl3 );					// 課税仕入
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSvl4, (unsigned char*)RefundStk[Cnt].KPSvl4 );					// 共通

		pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

		pDBSyzAcc->m_pRefundrec->Update();
	}

	// 3-(2) 主な棚卸資産・原材料等の取得
	HnMx = 5;
	for( Cnt = 0; Cnt != HnMx; Cnt++ ) {
		pDBSyzAcc->m_pRefundrec->AddNew();
		pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_INVENT_TYPE;			
		pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1;			
		// 変換
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSsup, (unsigned char*)RefundTna[Cnt].KPSsup, 16 ); 		// 資産の種類等（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSslw, (unsigned char*)RefundTna[Cnt].KPSslw, 16 ); 		// 〃　　　　　（下段）
		//m_Util.date_to_bin( 0, (long*)&pDBSyzAcc->m_pRefundrec->KPSday, (unsigned char*)RefundTna[Cnt].KPSday );		// 譲渡年月日
		CVolDateDB	voldate;
		int dummy;
		unsigned char yymmdd[3] = {0};
		if(( RefundTna[Cnt].KPSday[0] != 0 )&&( RefundTna[Cnt].KPSday[1] != 0 )&&( RefundTna[Cnt].KPSday[2] != 0 )){
			voldate.db_datecnvGen( 4, (unsigned char*)RefundTna[Cnt].KPSday, &dummy, yymmdd, 1, 0 );
			pDBSyzAcc->m_pRefundrec->KPSday = m_Arith.bcd_int( yymmdd, 3 );
		}

		pDBSyzAcc->m_pRefundrec->KPSdys = RefundTna[Cnt].KPSdys;												// 〃　　　　　継続サイン
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundTna[Cnt].KPSval );					// 取引金額
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundTna[Cnt].KPSnup, 20 ); 		// 取引先名（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundTna[Cnt].KPSnlw, 20 ); 		// 〃　　　（下段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundTna[Cnt].KPSaup, 30 ); 		// 取引先住所（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundTna[Cnt].KPSalw, 30 ); 		// 〃　　　　（下段）

		pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

		pDBSyzAcc->m_pRefundrec->Update();
	}
	// 3-(3) 主な固定資産等の取得
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	for( Cnt = 0; Cnt != HnMx; Cnt++ ) {
		pDBSyzAcc->m_pRefundrec->AddNew();
		pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_FIXED_TYPE;			
		pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1;			
		// 変換
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSsup, (unsigned char*)RefundKot[Cnt].KPSsup, 16 ); 		// 資産の種類等（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSslw, (unsigned char*)RefundKot[Cnt].KPSslw, 16 ); 		// 〃　　　　　（下段）
		//m_Util.date_to_bin( 0, (long*)&pDBSyzAcc->m_pRefundrec->KPSday, (unsigned char*)RefundKot[Cnt].KPSday );		// 譲渡年月日
		CVolDateDB	voldate;
		int dummy;
		unsigned char yymmdd[3] = {0};
		if(( RefundKot[Cnt].KPSday[0] != 0 )&&( RefundKot[Cnt].KPSday[1] != 0 )&&( RefundKot[Cnt].KPSday[2] != 0 )){
			voldate.db_datecnvGen( 4, (unsigned char*)RefundKot[Cnt].KPSday, &dummy, yymmdd, 1, 0 );
			pDBSyzAcc->m_pRefundrec->KPSday = m_Arith.bcd_int( yymmdd, 3 );
		}

		pDBSyzAcc->m_pRefundrec->KPSdys = RefundKot[Cnt].KPSdys;												// 〃　　　　　継続サイン
		m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundKot[Cnt].KPSval );					// 取引金額
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundKot[Cnt].KPSnup, 20 ); 		// 取引先名（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundKot[Cnt].KPSnlw, 20 ); 		// 〃　　　（下段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundKot[Cnt].KPSaup, 30 ); 		// 取引先住所（上段）
		m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundKot[Cnt].KPSalw, 30 ); 		// 〃　　　　（下段）

		pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

		pDBSyzAcc->m_pRefundrec->Update();
	}

	// 主な課税資産の譲渡等
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	if( JotTbl.tp_rnum )	{
		Cnt = 0;
		while( 1 ) {
			if( m_TblHdl.th_rget( &RefundRec, &JotTbl, Cnt ) )	break;
			pDBSyzAcc->m_pRefundrec->AddNew();
			pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_FORTUNE_TYPE;			
			pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1+HnMx;			
			// 変換
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSsup, (unsigned char*)RefundRec.KPSsup, 16 );			
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSslw, (unsigned char*)RefundRec.KPSslw, 16 );			
			//m_Util.date_to_bin( 0, (long*)&pDBSyzAcc->m_pRefundrec->KPSday, (unsigned char*)RefundRec.KPSday );
			CVolDateDB	voldate;
			int dummy;
			unsigned char yymmdd[3] = {0};
			if(( RefundRec.KPSday[0] != 0 )&&( RefundRec.KPSday[1] != 0 )&&( RefundRec.KPSday[2] != 0 )){
				voldate.db_datecnvGen( 4, (unsigned char*)RefundRec.KPSday, &dummy, yymmdd, 1, 0 );
				pDBSyzAcc->m_pRefundrec->KPSday = m_Arith.bcd_int( yymmdd, 3 );
			}

			pDBSyzAcc->m_pRefundrec->KPSdys = RefundRec.KPSdys;													
			m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundRec.KPSval );					
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundRec.KPSnup, 20 );			
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundRec.KPSnlw, 20 );			
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundRec.KPSaup, 30 );			
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundRec.KPSalw, 30 );			

			pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

			pDBSyzAcc->m_pRefundrec->Update();
			Cnt++;
		}
	}
	// 主な輸出取引等の明細
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	if( ExpTbl.tp_rnum )	{
		Cnt = 0;
		while( 1 ) {
			if( m_TblHdl.th_rget( &RefundRec, &ExpTbl, Cnt ) )	break;
			pDBSyzAcc->m_pRefundrec->AddNew();
			pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_EXPORT_TYPE;			
			pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1+HnMx;			
			// 変換
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundRec.KPSnup, 20 );			// 取引先名（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundRec.KPSnlw, 20 );			// 〃　　　（下段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundRec.KPSaup, 30 );			// 取引先住所（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundRec.KPSalw, 30 );			// 〃　　　　（下段）
			m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundRec.KPSval );					// 取引金額
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPStup, (unsigned char*)RefundRec.KPStup, 20 );			// 主な取引商品（上段）輸出取引　専用
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPStlw, (unsigned char*)RefundRec.KPStlw, 20 );			// 〃　　　　　（下段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSzup, (unsigned char*)RefundRec.KPSzup, 16 );			// 所轄税関名　（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSzlw, (unsigned char*)RefundRec.KPSzlw, 16 );			// 〃　　　　　（下段）

			pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

			pDBSyzAcc->m_pRefundrec->Update();
			Cnt++;
		}
	}
	// 主な棚卸資産・原材料等の取得
	HnMx = 5;
	if( TnaTbl.tp_rnum ){
		Cnt = 0;
		while( 1 ) {
			if( m_TblHdl.th_rget( &RefundRec, &TnaTbl, Cnt ) )	break;
			pDBSyzAcc->m_pRefundrec->AddNew();
			pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_INVENT_TYPE;			
			pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1+HnMx;			
			// 変換
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSsup, (unsigned char*)RefundRec.KPSsup, 16 );			// 資産の種類等（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSslw, (unsigned char*)RefundRec.KPSslw, 16 );			// 〃　　　　　（下段）
			//m_Util.date_to_bin( 0, (long*)&pDBSyzAcc->m_pRefundrec->KPSday, (unsigned char*)RefundRec.KPSday );		// 取得年月日
			CVolDateDB	voldate;
			int dummy;
			unsigned char yymmdd[3] = {0};
			if(( RefundRec.KPSday[0] != 0 )&&( RefundRec.KPSday[1] != 0 )&&( RefundRec.KPSday[2] != 0 )){
				voldate.db_datecnvGen( 4, (unsigned char*)RefundRec.KPSday, &dummy, yymmdd, 1, 0 );
				pDBSyzAcc->m_pRefundrec->KPSday = m_Arith.bcd_int( yymmdd, 3 );
			}

			pDBSyzAcc->m_pRefundrec->KPSdys = RefundRec.KPSdys;													// 〃　　　　　継続サイン
			m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundRec.KPSval );					// 取引金額
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundRec.KPSnup, 20 );			// 取引先名（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundRec.KPSnlw, 20 );			// 〃　　　（下段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundRec.KPSaup, 30 );			// 取引先住所（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundRec.KPSalw, 30 );			// 〃　　　　（下段）

			pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

			pDBSyzAcc->m_pRefundrec->Update();
			Cnt++;
		}
	}
	// 主な固定資産等の取得
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 5;
	}
	else{
		HnMx = 10;
	}
	if( KotTbl.tp_rnum ){
		Cnt = 0;
		while( 1 ) {
			if( m_TblHdl.th_rget( &RefundRec, &KotTbl, Cnt ) )	break;
			pDBSyzAcc->m_pRefundrec->AddNew();
			pDBSyzAcc->m_pRefundrec->KPItem = ID_ICSSH_REF_FIXED_TYPE;			
			pDBSyzAcc->m_pRefundrec->KPLine = Cnt+1+HnMx;			
			// 変換
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSsup, (unsigned char*)RefundRec.KPSsup, 16 );			// 資産の種類等（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSslw, (unsigned char*)RefundRec.KPSslw, 16 );			// 〃　　　　　（下段）
			//m_Util.date_to_bin( 0, (long*)&pDBSyzAcc->m_pRefundrec->KPSday, (unsigned char*)RefundRec.KPSday );		// 取得年月日
			CVolDateDB	voldate;
			int dummy;
			unsigned char yymmdd[3] = {0};
			if(( RefundRec.KPSday[0] != 0 )&&( RefundRec.KPSday[1] != 0 )&&( RefundRec.KPSday[2] != 0 )){
				voldate.db_datecnvGen( 4, (unsigned char*)RefundRec.KPSday, &dummy, yymmdd, 1, 0 );
				pDBSyzAcc->m_pRefundrec->KPSday = m_Arith.bcd_int( yymmdd, 3 );
			}

			pDBSyzAcc->m_pRefundrec->KPSdys = RefundRec.KPSdys;													// 〃　　　　　継続サイン
			m_Util.val_to_asci( &pDBSyzAcc->m_pRefundrec->KPSval, (unsigned char*)RefundRec.KPSval );					// 取引金額
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnup, (unsigned char*)RefundRec.KPSnup, 20 );			// 取引先名（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSnlw, (unsigned char*)RefundRec.KPSnlw, 20 );			// 〃　　　（下段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSaup, (unsigned char*)RefundRec.KPSaup, 30 );			// 取引先住所（上段）
			m_Util.char_to_cstring( &pDBSyzAcc->m_pRefundrec->KPSalw, (unsigned char*)RefundRec.KPSalw, 30 );			// 〃　　　　（下段）

			pDBSyzAcc->m_pRefundrec->KPSequ = sn_seq;																	// 履歴番号

			pDBSyzAcc->m_pRefundrec->Update();
			Cnt++;
		}
	}

	pDBSyzAcc->ReFundVolClose();
	pDBSyzAcc->ReFundRecClose();
	
	delete pDBSyzAcc;
	return 0;
}

//-----------------------------------------------------------------------------
// 財務連動
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		pSyzSyukei	：	集計クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
//-- '15.09.30 --
//int CH26SnRefundData::ConnectZmdata( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl, EnumIdIcsShRevType ShRevtype )
//---------------
int CH26SnRefundData::ConnectZmdata( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl, EnumIdIcsShRevType ShRevtype, MoneyBasejagArray &MoneyBaseArray )
//---------------
{
	ASSERT( pDBNpSub );
	ASSERT( pSyzSyukei );
	ASSERT( pTblhdl );
	ASSERT( pSkjCnvtbl );
	ASSERT( pSyzSyukei );

	// 仕入控除額の取得
	char	tmpSkjsyz[6]={0};
	MoneyBasejagArray money;
	if( ShRevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){ 
		money = pSyzSyukei->GetShinkokuData( _T("22") );
		int cnt = (int)money.GetCount();
		int	incnt = 0;
		for( int l=0; l<cnt; l++ ){
			incnt = (int)money[l].GetCount();
			for( int m=0; m<incnt; m++ ){
				if( money[l][m].code.Left(4) == _T("2209") ){
					if( money[l][m].code == _T("220901") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("220902") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("220903") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
				}
				else if( money[l][m].code.Left(4) == _T("2211") ){
					if( money[l][m].code == _T("221101") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("221102") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("221103") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
				}
				else if( money[l][m].code.Left(4) == _T("2212") ){
					if( money[l][m].code == _T("221201") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("221202") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("221203") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
				}
			}
		}
	}
	else{
		money = pSyzSyukei->GetShinkokuData( _T("12") );
		int cnt = (int)money.GetCount();
		int	incnt = 0;
		for( int l=0; l<cnt; l++ ){
			incnt = (int)money[l].GetCount();
			for( int m=0; m<incnt; m++ ){
				if( money[l][m].code.Left(4) == _T("1209") ){
					if( money[l][m].code == _T("120901") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("120902") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("120903") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
				}
				else if( money[l][m].code.Left(4) == _T("1210") ){
					if( money[l][m].code == _T("121001") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("121002") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
					if( money[l][m].code == _T("121003") ){
						m_Arith.l_add( tmpSkjsyz, tmpSkjsyz, money[l][m].arith );	
					}
				}
			}
		}
	}

	HJ_SKDATA	Hj_SkData;
	KJ_SKDATA	Kj_SkData;
	memset( &Hj_SkData, '\0', sizeof(HJ_SKDATA) );
	memset( &Kj_SkData, '\0', sizeof(KJ_SKDATA) );


//	int		i, j, k, rno, pno, flg, oth, shinsw, kojin;
	int		i, j, k, pno, flg, oth, shinsw;
//	char	codbf[10], mony[6];
	char	mony[6];
	char	Tag[6], Hka[6], Kmi[6], Kmz[6], Nki[6], Nkz[6], Bkz[6], Kaz[6], Kz3[6], Kz5[6], Kz8[6];
//	struct	_SZREC	szrec;
//--> '14.04.26 INS START
	BOOL	IsImportZan = FALSE;
//<-- '14.04.26 INS END
//--> '15.08.27 INS START
	BOOL	IsTkkzZan = FALSE;
//<-- '15.08.27 INS END

	SH_SKJ_CNV	skjcnv;

	SH_SKJ_REC	SkjRec[10] = {0};	// 集計テーブル
	for( i=0; i!=SKJMAX; i++ ){
		memset( &SkjRec[i], '\0', sizeof(SH_SKJ_REC) );
	}

	if( pDBNpSub->zvol->s_sgn1&0xc0 ){
		shinsw = 0;	// 抜き「抜き又は一括税抜き」
	}
	else{
		shinsw = 1;	// 込み「込み」
	}

//-- '15.09.30 --
//	money = pSyzSyukei->GetShisanData( PR_SHIIRE );
//---------------
	money = MoneyBaseArray;
//---------------
	int max = (int)money.GetCount();
	int inmax = 0;

	for( int i=0; i<max; i++ ){
		flg = 0;
		oth = 0;
//--> '14.04.26 INS START
		IsImportZan = FALSE;
//<-- '14.04.26 INS END
//--> '15.08.27 INS START
		IsTkkzZan = FALSE;
//<-- '15.08.27 INS END
		inmax = (int)money[i].GetCount();
		if( inmax == 0 ){
			continue;
		}
		else if( money[i][0].attr.shisan_syukei_type != -1 ){
			// 明細以外は全て飛ばす
			continue;
		}

		// 流動資産
		if( (money[i][0].attr.sc_kbn1==0x01) && (money[i][0].attr.sc_kbn2==0x00) ){
			flg++;
		}
		// 固定資産
		else if( (money[i][0].attr.sc_kbn1==0x11) && (money[i][0].attr.sc_kbn2==0x02) && (1) ){
			flg++;
		}
		// 有価証券
		else if( (money[i][0].attr.sc_kbn1==0x11) && (money[i][0].attr.sc_kbn2==0x03) && (1) ){
			flg++;
		}
		// 土地
		else if( (money[i][0].attr.sc_kbn1==0x11) && (money[i][0].attr.sc_kbn2==0x04) && (1) ){
			flg++;
		}
		// 仕入
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x00) ){
			if( money[i][0].attr.sc_attr&0x01 ){
				flg = 2;
			}
			else{
				flg++;
			}
		}
		// 仕入返品
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x01) ){
//			flg++;
//--> '14.05.12 CUT START
//			m_Arith.l_neg( money[i][0].arith );
//			m_Arith.l_neg( money[i][1].arith );
//<-- '14.05.12 CUT END
			flg = 2;
		}
		// 販売奨励金収入
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x02) ){
/*- '14.05.12 -*/
//			flg++;
/*-------------*/
			flg = 2;
/*-------------*/
		}
		// 仕入割引
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x03) ){
//			flg++;
//--> '14.05.12 CUT START
//			m_Arith.l_neg( money[i][0].arith );
//			m_Arith.l_neg( money[i][1].arith );
//<-- '14.05.12 CUT END
			flg = 2;
		}
		// 輸入仕入
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x04) ){
			if( money[i][0].attr.sc_attr&0x01 ){
				flg = 2;
			}
			else{
				flg++;
			}
//--> '14.04.26 INS START
			IsImportZan = TRUE;
//<-- '14.04.26 INS END
		}
		// 【個】棚卸調整加算額
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x05) && (1) ){
			flg++;
		}
		// 【個】棚卸調整減算額
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x06) && (1) ){
/*- '14.08.22 -*/
//			flg++;
/*-------------*/
			flg = 2;
/*-------------*/
		}
		// 経費
		else if( (money[i][0].attr.sc_kbn1==0x08) && (money[i][0].attr.sc_kbn2==0x00) ){
//-- '14.10.24 --
//			flg++;
//---------------
			if( money[i][0].attr.sc_attr&0x01 ){
				flg = 2;
			}
			else{
				flg++;
			}
//---------------
		}
		// 営業外費用
		else if( (money[i][0].attr.sc_kbn1==0x08) && (money[i][0].attr.sc_kbn2==0x01) ){
			flg++;
		}
		// 特別損失
		else if( (money[i][0].attr.sc_kbn1==0x08) && (money[i][0].attr.sc_kbn2==0x02) ){
			flg++;
		}
		else if( (pDBNpSub->zvol->apno&0xff) == 0x52 ){	// 社会福祉
			// 固定資産
			if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x08) ){
				flg++;
			}
			// 有価証券
			else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x09) ){
				flg++;
			}
		}
//--> '15.08.27 INS START
		// 特定課税仕入
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x0b) ){
			flg++;
			IsTkkzZan = TRUE;
		}
		// 特定課税仕入の返還
		else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x0d) ){
			if( money[i][0].attr.sc_attr&0x01 ){
				flg = 2;
			}
			else{
				flg++;
			}
			IsTkkzZan = TRUE;
		}
//<-- '15.08.27 INS END

		if( !flg ){
			continue;
		}

		memset( &skjcnv, '\0', sizeof(skjcnv) );
		memmove( skjcnv.SKJCod, money[i][0].code, money[i][0].code.GetLength() );
		// キーは全て大文字で格納されているので、変換！
		_strupr_s( (char*)skjcnv.SKJCod, sizeof(skjcnv.SKJCod) );

		if( !pTblhdl->th_acs(thRead, &skjcnv, pSkjCnvtbl) ){
			if( pSnHeadData->IsSoleProprietor() ){
				pno = skjcnv.KJNCod;
			}
			else{
				pno = skjcnv.HJNCod;
			}
			if( !pno ){
				continue;
			}
//--> '14.10.24 INS START
			else if( (pno>=10) || (pno<0) ){
				// 不正値は加算しない
				continue;
			}
//<-- '14.10.24 INS END
			pno--;
			memset( Tag, '\0', 6 );
			memset( Hka, '\0', 6 );
			memset( Kmi, '\0', 6 );
			memset( Kmz, '\0', 6 );
			memset( Nki, '\0', 6 );
			memset( Nkz, '\0', 6 );
			memset( Bkz, '\0', 6 );
			memset( Kaz, '\0', 6 );
			memset( Kz3, '\0', 6 );
			memset( Kz5, '\0', 6 );
			memset( Kz8, '\0', 6 );

/*- '14.02.10 -*/
//			if( shinsw ){	// 税込み
//				m_Arith.l_add( Kaz, Kaz, Kmi );
//				m_Arith.l_add( Kaz, Kaz, Nki );
//				m_Arith.l_add( Kaz, Kaz, Nkz );
//			}
//			else{			// 税抜き
//				m_Arith.l_sub( Kaz, Kmi, Kmz );
//				m_Arith.l_add( Kaz, Kaz, Nki );
//			}
/*-------------*/
			if( money[i][0].attr.shisan_hikazei_hukazei == 0x01 ){
				m_Arith.l_add( Hka, Hka, money[i][2].arith );
			}
			else if( money[i][0].attr.shisan_hikazei_hukazei == 0x02 ){
				m_Arith.l_add( Tag, Tag, money[i][2].arith );
			}
			else{
//--- '15.08.27 ---
///*- '14.04.26 -*/
////				if( shinsw ){
////					m_Arith.l_add( Kaz, Kaz, money[i][2].arith );
////				}
////				else{
////					m_Arith.l_add( Kaz, Kaz, money[i][0].arith );
////				}
///*-------------*/
//				if( IsImportZan ){
//					if( shinsw ){
//						m_Arith.l_add( Tag, Kaz, money[i][2].arith );
//					}
//					else{
///*- '14.05.13 -*/
////						m_Arith.l_add( Tag, Kaz, money[i][2].arith );
///*-------------*/
//						m_Arith.l_add( Tag, Kaz, money[i][0].arith );
///*-------------*/
//					}
//				}
//				else if( shinsw ){
//					m_Arith.l_add( Kaz, Kaz, money[i][2].arith );
//				}
//				else{
//					m_Arith.l_add( Kaz, Kaz, money[i][0].arith );
//				}
///*-------------*/
//-----------------
				if( IsImportZan || (IsTkkzZan==TRUE)&&(pSnHeadData->IsNeedTkkzDisp()==FALSE) ){
					if( shinsw ){
						m_Arith.l_add( Tag, Kaz, money[i][2].arith );
					}
					else{
						m_Arith.l_add( Tag, Kaz, money[i][0].arith );
					}
				}
				else if( shinsw ){
					m_Arith.l_add( Kaz, Kaz, money[i][2].arith );
				}
				else{
					m_Arith.l_add( Kaz, Kaz, money[i][0].arith );
				}
//-----------------
			}
/*-------------*/
			if( flg == 2 ){
				m_Arith.l_sub( SkjRec[pno].SZan, SkjRec[pno].SZan, Kaz );	// 課税
				m_Arith.l_sub( SkjRec[pno].SZan, SkjRec[pno].SZan, Tag );	// 対象外
				m_Arith.l_sub( SkjRec[pno].SZan, SkjRec[pno].SZan, Hka );	// 非課税

				m_Arith.l_sub( SkjRec[pno].SKaz, SkjRec[pno].SKaz, Kaz );

				// 控除対象取引金額《簡易課税のみ使用》
				if( money[i][0].attr.sc_zino == 0 ){		// ５％
					m_Arith.l_sub( SkjRec[pno].SKz5, SkjRec[pno].SKz5, Kaz );
								}
				else if( money[i][0].attr.sc_zino == 1 ){	// ３％
					m_Arith.l_sub( SkjRec[pno].SKz3, SkjRec[pno].SKz3, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 3 ){	// ８％
					m_Arith.l_sub( SkjRec[pno].SKz8, SkjRec[pno].SKz8, Kaz );
				}
			}
			else{
				m_Arith.l_add( SkjRec[pno].SZan, SkjRec[pno].SZan, Kaz );	// 課税
				m_Arith.l_add( SkjRec[pno].SZan, SkjRec[pno].SZan, Tag );	// 対象外
				m_Arith.l_add( SkjRec[pno].SZan, SkjRec[pno].SZan, Hka );	// 非課税
				// 控除対象取引金額
				m_Arith.l_add( SkjRec[pno].SKaz, SkjRec[pno].SKaz, Kaz );
				// 控除対象取引金額《簡易課税のみ使用》
				if( money[i][0].attr.sc_zino == 0 ){		// ５％
					m_Arith.l_add( SkjRec[pno].SKz5, SkjRec[pno].SKz5, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 1 ){	// ３％
					m_Arith.l_add( SkjRec[pno].SKz3, SkjRec[pno].SKz3, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 3 ){	// ８％
					m_Arith.l_add( SkjRec[pno].SKz8, SkjRec[pno].SKz8, Kaz );
				}
			}
			oth = 1;

			continue;
		}

		if( !oth ){
			// 該当以外は法人用は『その他』・個人用は事業所得の『必要経費』に集計されるKJKTMN
			if( pSnHeadData->IsSoleProprietor() ){
				// 還付付表
/*- '14.02.10 -*/
//				if( m_TaxRefund ) {
//					if( (szrec.SZgrp1&0x0f) == 0x01 )	pno = KJKTMN - 1;	// 個人用その他固定資産		
//					else								pno = KJNOTH - 1;	// 個人用その他
//				}
//				else	{
//					if( (szrec.SZgrp1&0xff) == 0x11 )	pno = KJKTMN - 1;	// 個人用その他固定資産		
//					else								pno = KJNOTH - 1;	// 個人用その他
//				}
/*-------------*/
				if( (money[i][0].attr.sc_kbn1&0x0f) == 0x01 ){
					pno = KJKTMN - 1;	// 個人用その他固定資産		
				}
				else{
					pno = KJNOTH - 1;	// 個人用その他
				}
/*-------------*/
			}
			else{
				// 還付付表
/*- '14.02.10 -*/
//				if( m_TaxRefund ) {
//					if( (szrec.SZgrp1&0x0f) == 0x01 )	pno = HJKOTH - 1;	// 法人用その他資産	
//					else								pno = HJNOTH - 1;	// 法人用その他
//				}
//				else	{
//					if( (szrec.SZgrp1&0xff) == 0x11 )	pno = HJKTNN - 1;	// 法人用その他固定資産	
//					else								pno = HJNOTH - 1;	// 法人用その他
//				}
/*-------------*/
				if( (money[i][0].attr.sc_kbn1&0x0f) == 0x01 ){
					pno = HJKOTH - 1;	// 法人用その他資産	
				}
				else{
					pno = HJNOTH - 1;	// 法人用その他
				}
/*-------------*/
			}
			memset( Tag, '\0', 6 );
			memset( Hka, '\0', 6 );
			memset( Kmi, '\0', 6 );
			memset( Kmz, '\0', 6 );
			memset( Nki, '\0', 6 );
			memset( Nkz, '\0', 6 );
			memset( Bkz, '\0', 6 );
			memset( Kaz, '\0', 6 );
			memset( Kz3, '\0', 6 );
			memset( Kz5, '\0', 6 );
			memset( Kz8, '\0', 6 );
//			for( i = smon; i <= emon; i++ ) {
//				l_add( Tag, Tag, &szrec.SZdtbl[i].SDval[0] );
//				l_add( Hka, Hka, &szrec.SZdtbl[i].SDval[1] );
//				l_add( Kmi, Kmi, &szrec.SZdtbl[i].SDval[3] );
//				l_add( Kmz, Kmz, &szrec.SZdtbl[i].SDval[4] );
//				l_add( Nki, Nki, &szrec.SZdtbl[i].SDval[5] );
//				l_add( Nkz, Nkz, &szrec.SZdtbl[i].SDval[6] );
//				l_add( Nki, Nki, &szrec.SZdtbl[i].SDval[7] );
//			}
//			if( (szrec.SZgrp1&0xff) == 0x04 && (szrec.SZgrp2&0xff) == 0x04 )	{
//				// 輸入仕入
//				l_add( Tag, Tag, Nki );
//				memset( Nki, '\0', 6 );
//				if( shinsw )	{		//★[13'11.05]//
//					l_add( Tag, Tag, Nkz );
//					memset( Nkz, '\0', 6 );
//				}
//			}

/*- '14.02.10 -*/
//			if( shinsw ){	// 税込み
//				l_add( Kaz, Kaz, Kmi );
//				l_add( Kaz, Kaz, Nki );
//				l_add( Kaz, Kaz, Nkz );
//			}
//			else{			// 税抜き
//				l_sub( Kaz, Kmi, Kmz );
//				l_add( Kaz, Kaz, Nki );
//			}
/*-------------*/
			if( money[i][0].attr.shisan_hikazei_hukazei == 0x01 ){
				m_Arith.l_add( Hka, Kaz, money[i][2].arith );
			}
			else if( money[i][0].attr.shisan_hikazei_hukazei == 0x02 ){
				m_Arith.l_add( Tag, Kaz, money[i][2].arith );
			}
			else{
				if( shinsw ){
					m_Arith.l_add( Kaz, Kaz, money[i][2].arith );
				}
				else{
					m_Arith.l_add( Kaz, Kaz, money[i][0].arith );
				}
			}
/*-------------*/
			if( flg == 2 ){
				// 決算額へ集計
				m_Arith.l_sub( SkjRec[pno].SZan, SkjRec[pno].SZan, Kaz );	// 課税
				m_Arith.l_sub( SkjRec[pno].SZan, SkjRec[pno].SZan, Tag );	// 対象外
				m_Arith.l_sub( SkjRec[pno].SZan, SkjRec[pno].SZan, Hka );	// 非課税

				// 控除対象取引金額
				m_Arith.l_sub( SkjRec[pno].SKaz, SkjRec[pno].SKaz, Kaz );
				// 控除対象取引金額《簡易課税のみ使用》
				if( money[i][0].attr.sc_zino == 0 ){		// ５％
					m_Arith.l_sub( SkjRec[pno].SKz5, SkjRec[pno].SKz5, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 1 ){	// ３％
					m_Arith.l_sub( SkjRec[pno].SKz3, SkjRec[pno].SKz3, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 3 ){	// ８％
					m_Arith.l_sub( SkjRec[pno].SKz8, SkjRec[pno].SKz8, Kaz );
				}
			}
			else{
				// 決算額へ集計
				m_Arith.l_add( SkjRec[pno].SZan, SkjRec[pno].SZan, Kaz );	// 課税
				m_Arith.l_add( SkjRec[pno].SZan, SkjRec[pno].SZan, Tag );	// 対象外
				m_Arith.l_add( SkjRec[pno].SZan, SkjRec[pno].SZan, Hka );	// 非課税

				// 控除対象取引金額
				m_Arith.l_add( SkjRec[pno].SKaz, SkjRec[pno].SKaz, Kaz );
				// 控除対象取引金額《簡易課税のみ使用》
				if( money[i][0].attr.sc_zino == 0 ){		// ５％
					m_Arith.l_add( SkjRec[pno].SKz5, SkjRec[pno].SKz5, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 1 ){	// ３％
					m_Arith.l_add( SkjRec[pno].SKz3, SkjRec[pno].SKz3, Kaz );
				}
				else if( money[i][0].attr.sc_zino == 3 ){	// ８％
					m_Arith.l_add( SkjRec[pno].SKz8, SkjRec[pno].SKz8, Kaz );
				}
			}
		}
	}
	//== 仕入控除対象金額の取り込み ==
	if( pSnHeadData->IsSoleProprietor() ){
		// 初期設定
		Kj_SkData.Kj_KzSrTxSgn = shinsw;

		for( i=0; i<6; i++ ){
			memset( Kj_SkData.Kj_JgSyKmk[i].KesMny, '\0', 6 );			// 決算額
			memset( Kj_SkData.Kj_JgSyKmk[i].NoKjMny, '\0', 6 );			// 控除対象外金額
			memset( Kj_SkData.Kj_JgSyKmk[i].KjTsMny, '\0', 6 );			// 控除対象取引金額
		}
		for( i=0; i<6; i++ ){
			memset( Kj_SkData.Kj_HdSyKmk[i].KesMny, '\0', 6 );			// 決算額
			memset( Kj_SkData.Kj_HdSyKmk[i].NoKjMny, '\0', 6 );			// 控除対象外金額
			memset( Kj_SkData.Kj_HdSyKmk[i].KjTsMny, '\0', 6 );			// 控除対象取引金額
		}
		for( i=0; i<6; i++ ){
			memset( Kj_SkData.Kj_SytkKmk[i].KesMny, '\0', 6 );			// 決算額
			memset( Kj_SkData.Kj_SytkKmk[i].NoKjMny, '\0', 6 );			// 控除対象外金額
			memset( Kj_SkData.Kj_SytkKmk[i].KjTsMny, '\0', 6 );			// 控除対象取引金額
		}
		memset( Kj_SkData.Kj_KzSrMny, '\0', 6 );						// 課税仕入高の合計
		Kj_SkData.Kj_SrKjTxSw = 0x00;									// 手入力解除
		memset( Kj_SkData.Kj_SrKjTxMny, '\0', 6 );						// 仕入控除税額
		memset( Kz3, '\0', 6 );
		memset( Kz5, '\0', 6 );
		memset( Kz8, '\0', 6 );

		for( i=0, j=0, k=0; i!=SKJMAX; i++ ){
			switch( i ){
				case 0 :
					k = 0;
					j = 0;
					break;
				case 1 :
					k = 0;	
					j = 1;
					break;
				case 2 :
					k = 0;
					j = 2;
					break;
				case 4 :
					k = 1;
					j = 0;
					break;
				case 5 :
					k = 1;
					j = 1;
					break;
				case 7 :	
					k = 2;	
					j = 0;
					break;
				case 8 :	
					k = 2;	
					j = 1;
					break;
				case 9 :
					k = 2;
					j = 2;	
					break;
				default:	
					continue;
					break;
			}
			switch( k ){
				case 1 :
					memset( mony, '\0', 6 );
					m_Arith.l_sub( mony, SkjRec[i].SZan, SkjRec[i].SKaz );
					m_Arith.l_add( Kj_SkData.Kj_HdSyKmk[j].KesMny,  Kj_SkData.Kj_HdSyKmk[j].KesMny,  SkjRec[i].SZan );
					m_Arith.l_add( Kj_SkData.Kj_HdSyKmk[j].NoKjMny, Kj_SkData.Kj_HdSyKmk[j].NoKjMny, mony );
					m_Arith.l_add( Kj_SkData.Kj_HdSyKmk[j].KjTsMny, Kj_SkData.Kj_HdSyKmk[j].KjTsMny, SkjRec[i].SKaz );
					// 合計
					m_Arith.l_add( Kj_SkData.Kj_HdSyKmk[5].KesMny,  Kj_SkData.Kj_HdSyKmk[5].KesMny,  SkjRec[i].SZan );
					m_Arith.l_add( Kj_SkData.Kj_HdSyKmk[5].NoKjMny, Kj_SkData.Kj_HdSyKmk[5].NoKjMny, mony );
					m_Arith.l_add( Kj_SkData.Kj_HdSyKmk[5].KjTsMny, Kj_SkData.Kj_HdSyKmk[5].KjTsMny, SkjRec[i].SKaz );
					break;
				case 2 :
					memset( mony, '\0', 6 );
					m_Arith.l_sub( mony, SkjRec[i].SZan, SkjRec[i].SKaz );
					m_Arith.l_add( Kj_SkData.Kj_SytkKmk[j].KesMny,  Kj_SkData.Kj_SytkKmk[j].KesMny,  SkjRec[i].SZan );
					m_Arith.l_add( Kj_SkData.Kj_SytkKmk[j].NoKjMny, Kj_SkData.Kj_SytkKmk[j].NoKjMny, mony );
					m_Arith.l_add( Kj_SkData.Kj_SytkKmk[j].KjTsMny, Kj_SkData.Kj_SytkKmk[j].KjTsMny, SkjRec[i].SKaz );
					// 合計
					m_Arith.l_add( Kj_SkData.Kj_SytkKmk[5].KesMny,  Kj_SkData.Kj_SytkKmk[5].KesMny,  SkjRec[i].SZan );
					m_Arith.l_add( Kj_SkData.Kj_SytkKmk[5].NoKjMny, Kj_SkData.Kj_SytkKmk[5].NoKjMny, mony );
					m_Arith.l_add( Kj_SkData.Kj_SytkKmk[5].KjTsMny, Kj_SkData.Kj_SytkKmk[5].KjTsMny, SkjRec[i].SKaz );
					break;
				default:
					memset( mony, '\0', 6 );
					m_Arith.l_sub( mony, SkjRec[i].SZan, SkjRec[i].SKaz );
					m_Arith.l_add( Kj_SkData.Kj_JgSyKmk[j].KesMny,  Kj_SkData.Kj_JgSyKmk[j].KesMny,  SkjRec[i].SZan );
					m_Arith.l_add( Kj_SkData.Kj_JgSyKmk[j].NoKjMny, Kj_SkData.Kj_JgSyKmk[j].NoKjMny, mony );
					m_Arith.l_add( Kj_SkData.Kj_JgSyKmk[j].KjTsMny, Kj_SkData.Kj_JgSyKmk[j].KjTsMny, SkjRec[i].SKaz );
					// 合計
					m_Arith.l_add( Kj_SkData.Kj_JgSyKmk[5].KesMny,  Kj_SkData.Kj_JgSyKmk[5].KesMny,  SkjRec[i].SZan );
					m_Arith.l_add( Kj_SkData.Kj_JgSyKmk[5].NoKjMny, Kj_SkData.Kj_JgSyKmk[5].NoKjMny, mony );
					m_Arith.l_add( Kj_SkData.Kj_JgSyKmk[5].KjTsMny, Kj_SkData.Kj_JgSyKmk[5].KjTsMny, SkjRec[i].SKaz );
					break;
			}
			// 課税対象仕入額
			m_Arith.l_add( Kz3, Kz3, SkjRec[i].SKz3 );
			m_Arith.l_add( Kz5, Kz5, SkjRec[i].SKz5 );
			m_Arith.l_add( Kz8, Kz5, SkjRec[i].SKz8 );
		}
		// 課税仕入高の合計⑫
		memmove( Kj_SkData.Kj_KzSrMny, Kj_SkData.Kj_JgSyKmk[5].KjTsMny, 6 );
		m_Arith.l_add( Kj_SkData.Kj_KzSrMny, Kj_SkData.Kj_KzSrMny, Kj_SkData.Kj_HdSyKmk[5].KjTsMny );
		m_Arith.l_add( Kj_SkData.Kj_KzSrMny, Kj_SkData.Kj_KzSrMny, Kj_SkData.Kj_SytkKmk[5].KjTsMny );

		// 仕入控除税額⑬
		if( m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_GENSOKU ){
//shimizu			Kj_SkData.Kj_SrKjTxSw = 0x01;	// 手入力あり
/*- '14.02.10 -*/
//			if( m_TaxRefund ) {
//				// 原則課税
//				if( TBhead.SVmzsw == 1 )	{				// ３％あり
//					l_add( Kj_SkData.Kj_SrKjTxMny, Snd.Sn_2F9A, Snd.Sn_2F9B );
//					l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, Snd.Sn_2F11A );					// 輸入仕入を加算
//					l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, Snd.Sn_2F11B );
//				}
//				else	{									// ３％なし
//					l_add( Kj_SkData.Kj_SrKjTxMny, Snd.Sn_2F9B, Snd.Sn_2F11B );								// 輸入仕入を加算
//				}
//			}
//			else	{
//				// 原則課税
//				if( TBhead.SVmzsw == 1 )	l_add( Kj_SkData.Kj_SrKjTxMny, Snd.Sn_1F8A, Snd.Sn_1F8B );				// ３％あり
//				else						memmove( Kj_SkData.Kj_SrKjTxMny, Snd.Sn_1F8B, 6 );						// ３％なし
//			}
/*-------------*/
//			// 原則課税
//			if( pSnHeadData->SVmzsw == 1 ){				// ３％あり
//				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, pSnHeadData->Sn_2F9A, pSnHeadData->Sn_2F9B );
//				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, pSnHeadData->Sn_2F11A );					// 輸入仕入を加算
//				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, pSnHeadData->Sn_2F11B );
//			}
//			else{									// ３％なし
//				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, tmpSkjsyz );
//				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, pSnHeadData->Sn_2F9B, pSnHeadData->Sn_2F11B );					// 輸入仕入を加算
//			}
			m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, tmpSkjsyz );
/*-------------*/
		}
		else{
			// 簡易課税 
			memset( mony, '\0', 6 );
			if( shinsw ){
				// 込み
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz3, 0, 30, 0 );
				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz5, 4, 40, 0 );
				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz8, 9, 63, 0 );
				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, mony );
			}
			else{
				// 抜き
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz3, 1, 30, 0 );
				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz5, 1, 40, 0 );
				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz8, 1, 63, 0 );
				m_Arith.l_add( Kj_SkData.Kj_SrKjTxMny, Kj_SkData.Kj_SrKjTxMny, mony );
			}
		}

		// 基本情報の更新
		REFUNDVOL &RefundVol = CRefundBaseDialog::refundvol;
		// 込／抜き
		RefundVol.KPSsgn &= 0xfe;
		if( shinsw )	RefundVol.KPSsgn |= 0x01;

		// 仕入取引等の明細
		memset( RefundStk, '\0', sizeof( REFUNDREC )*20 );
		// 事業所得
		// ① 仕入金額
		memmove( RefundStk[0].KPSvl1, Kj_SkData.Kj_JgSyKmk[0].KesMny,  6 );
		memmove( RefundStk[0].KPSvl2, Kj_SkData.Kj_JgSyKmk[0].NoKjMny, 6 );
		memmove( RefundStk[0].KPSvl3, Kj_SkData.Kj_JgSyKmk[0].KjTsMny, 6 );
		// ② 必要経費
		memmove( RefundStk[1].KPSvl1, Kj_SkData.Kj_JgSyKmk[1].KesMny,  6 );
		memmove( RefundStk[1].KPSvl2, Kj_SkData.Kj_JgSyKmk[1].NoKjMny, 6 );
		memmove( RefundStk[1].KPSvl3, Kj_SkData.Kj_JgSyKmk[1].KjTsMny, 6 );
		// ③ 固定資産の取得
		memmove( RefundStk[2].KPSvl1, Kj_SkData.Kj_JgSyKmk[2].KesMny,  6 );
		memmove( RefundStk[2].KPSvl2, Kj_SkData.Kj_JgSyKmk[2].NoKjMny, 6 );
		memmove( RefundStk[2].KPSvl3, Kj_SkData.Kj_JgSyKmk[2].KjTsMny, 6 );
		// ④ 小計
		memmove( RefundStk[3].KPSvl1, Kj_SkData.Kj_JgSyKmk[5].KesMny,  6 );
		memmove( RefundStk[3].KPSvl2, Kj_SkData.Kj_JgSyKmk[5].NoKjMny, 6 );
		memmove( RefundStk[3].KPSvl3, Kj_SkData.Kj_JgSyKmk[5].KjTsMny, 6 );
		// 不動産所得
		// ⑤ 必要経費
		memmove( RefundStk[4].KPSvl1, Kj_SkData.Kj_HdSyKmk[0].KesMny,  6 );
		memmove( RefundStk[4].KPSvl2, Kj_SkData.Kj_HdSyKmk[0].NoKjMny, 6 );
		memmove( RefundStk[4].KPSvl3, Kj_SkData.Kj_HdSyKmk[0].KjTsMny, 6 );
		// ⑥ 固定資産の取得
		memmove( RefundStk[5].KPSvl1, Kj_SkData.Kj_HdSyKmk[1].KesMny,  6 );
		memmove( RefundStk[5].KPSvl2, Kj_SkData.Kj_HdSyKmk[1].NoKjMny, 6 );
		memmove( RefundStk[5].KPSvl3, Kj_SkData.Kj_HdSyKmk[1].KjTsMny, 6 );
		// ⑦ 小計
		memmove( RefundStk[6].KPSvl1, Kj_SkData.Kj_HdSyKmk[5].KesMny,  6 );
		memmove( RefundStk[6].KPSvl2, Kj_SkData.Kj_HdSyKmk[5].NoKjMny, 6 );
		memmove( RefundStk[6].KPSvl3, Kj_SkData.Kj_HdSyKmk[5].KjTsMny, 6 );
		// その他の所得
		// ⑧ 仕入金額
		memmove( RefundStk[7].KPSvl1, Kj_SkData.Kj_SytkKmk[0].KesMny,  6 );
		memmove( RefundStk[7].KPSvl2, Kj_SkData.Kj_SytkKmk[0].NoKjMny, 6 );
		memmove( RefundStk[7].KPSvl3, Kj_SkData.Kj_SytkKmk[0].KjTsMny, 6 );
		// ⑨ 必要経費
		memmove( RefundStk[8].KPSvl1, Kj_SkData.Kj_SytkKmk[1].KesMny,  6 );
		memmove( RefundStk[8].KPSvl2, Kj_SkData.Kj_SytkKmk[1].NoKjMny, 6 );
		memmove( RefundStk[8].KPSvl3, Kj_SkData.Kj_SytkKmk[1].KjTsMny, 6 );
		// ⑩ 固定資産の取得
		memmove( RefundStk[9].KPSvl1, Kj_SkData.Kj_SytkKmk[2].KesMny,  6 );
		memmove( RefundStk[9].KPSvl2, Kj_SkData.Kj_SytkKmk[2].NoKjMny, 6 );
		memmove( RefundStk[9].KPSvl3, Kj_SkData.Kj_SytkKmk[2].KjTsMny, 6 );
		// ⑪ 小計
		memmove( RefundStk[10].KPSvl1, Kj_SkData.Kj_SytkKmk[5].KesMny,  6 );
		memmove( RefundStk[10].KPSvl2, Kj_SkData.Kj_SytkKmk[5].NoKjMny, 6 );
		memmove( RefundStk[10].KPSvl3, Kj_SkData.Kj_SytkKmk[5].KjTsMny, 6 );
		// ⑫ 課税仕入高の合計
		memmove( RefundStk[11].KPSvl3, Kj_SkData.Kj_KzSrMny, 6 );
		// ⑬ 課税仕入れ等の税額の合計
		memmove( RefundStk[12].KPSvl4, Kj_SkData.Kj_SrKjTxMny, 6 );

		// 実額入力
		RefundVol.KPSstx &= 0xfe;
		if( m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_GENSOKU ){
			if( m_Arith.l_cmp( RefundStk[12].KPSvl4 , RefundStk[12].KPSvl3 ) == 0 ){
				RefundVol.KPSstx |= 0x00;
			}
			else{
				RefundVol.KPSstx |= 0x01;
			}
		}
	}
	else{
		// 初期設定
		Hj_SkData.Hj_PlTxSgn = shinsw;
		for( i=0; i<6; i++ ){
			memset( Hj_SkData.Hj_PlKmk[i].KesMny, '\0', 6 );			// 決算額
			memset( Hj_SkData.Hj_PlKmk[i].NoKjMny, '\0', 6 );			// 控除対象外金額
			memset( Hj_SkData.Hj_PlKmk[i].KjTsMny, '\0', 6 );			// 控除対象取引金額
		}
		Hj_SkData.Hj_AsTxSgn = shinsw;
		for( i=0; i<6; i++ ){
			memset( Hj_SkData.Hj_AsKmk[i].KesMny, '\0', 6 );			// 決算額
			memset( Hj_SkData.Hj_AsKmk[i].NoKjMny, '\0', 6 );			// 控除対象外金額
			memset( Hj_SkData.Hj_AsKmk[i].KjTsMny, '\0', 6 );			// 控除対象取引金額
		}
		Hj_SkData.Hj_SrKjTxSw = 0x00;									// 手入力解除
		memset( Hj_SkData.Hj_SrKjTxMny, '\0', 6 );						// 仕入控除税額

		Hj_SkData.Hj_KzSrTxSgn = shinsw;

		memset( Kz3, '\0', 6 );
		memset( Kz5, '\0', 6 );
		memset( Kz8, '\0', 6 );
		// 損益科目
		for( i = 0, j = 0, k = 0; i != SKJMAX; i++ ) {
			switch( i ) {
				case 0 :	k = 0;	j = 0;		break;
				case 1 :	k = 0;	j = 1;		break;
				case 2 :	k = 0;	j = 2;		break;
				case 3 :	k = 0;	j = 3;		break;
				case 5 :	k = 1;	j = 0;		break;
				case 6 :	k = 1;	j = 1;		break;
				case 7 :	k = 1;	j = 2;		break;
				default:	continue;	
			}
			char	SZAN[6], NZAN[6], KZAN[6];
			memset( SZAN, '\0', sizeof( SZAN ) );	
			memset( NZAN, '\0', sizeof( NZAN ) );
			memset( KZAN, '\0', sizeof( KZAN ) );
			m_Util.l_calq( SZAN, SkjRec[i].SZan, 0x00 ); 
			m_Util.l_calq( KZAN, SkjRec[i].SKaz, 0x00 ); 
			m_Arith.l_sub( NZAN, SZAN, KZAN );
			if( !k )	{
				m_Arith.l_add( Hj_SkData.Hj_PlKmk[j].KesMny,  Hj_SkData.Hj_PlKmk[j].KesMny,  SZAN );
				m_Arith.l_add( Hj_SkData.Hj_PlKmk[j].NoKjMny, Hj_SkData.Hj_PlKmk[j].NoKjMny, NZAN );
				m_Arith.l_add( Hj_SkData.Hj_PlKmk[j].KjTsMny, Hj_SkData.Hj_PlKmk[j].KjTsMny, KZAN );
				// 合計
				m_Arith.l_add( Hj_SkData.Hj_PlKmk[5].KesMny,  Hj_SkData.Hj_PlKmk[5].KesMny,  SZAN );
				m_Arith.l_add( Hj_SkData.Hj_PlKmk[5].NoKjMny, Hj_SkData.Hj_PlKmk[5].NoKjMny, NZAN );
				m_Arith.l_add( Hj_SkData.Hj_PlKmk[5].KjTsMny, Hj_SkData.Hj_PlKmk[5].KjTsMny, KZAN );
			}
			else	{
				m_Arith.l_add( Hj_SkData.Hj_AsKmk[j].KesMny,  Hj_SkData.Hj_AsKmk[j].KesMny,  SZAN );
				m_Arith.l_add( Hj_SkData.Hj_AsKmk[j].NoKjMny, Hj_SkData.Hj_AsKmk[j].NoKjMny, NZAN );
				m_Arith.l_add( Hj_SkData.Hj_AsKmk[j].KjTsMny, Hj_SkData.Hj_AsKmk[j].KjTsMny, KZAN );
				// 合計																 
				m_Arith.l_add( Hj_SkData.Hj_AsKmk[5].KesMny,  Hj_SkData.Hj_AsKmk[5].KesMny,  SZAN );
				m_Arith.l_add( Hj_SkData.Hj_AsKmk[5].NoKjMny, Hj_SkData.Hj_AsKmk[5].NoKjMny, NZAN );
				m_Arith.l_add( Hj_SkData.Hj_AsKmk[5].KjTsMny, Hj_SkData.Hj_AsKmk[5].KjTsMny, KZAN );
																					 
			}
			// 課税対象仕入額
			m_Arith.l_add( Kz3, Kz3, SkjRec[i].SKz3 );
			m_Arith.l_add( Kz5, Kz5, SkjRec[i].SKz5 );
			m_Arith.l_add( Kz8, Kz8, SkjRec[i].SKz8 );
		}
		// 仕入控除税額⑬
		if( m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_GENSOKU ){
//			Hj_SkData.Hj_SrKjTxSw = 0x01;	// 手入力ありshimizu
			// 原則課税
/*- '14.03.10 -*/
//			if( pSnHeadData->SVmzsw == 1 ){				// ３％あり
//				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Snd.Sn_2F9A, Snd.Sn_2F9B );
//				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, Snd.Sn_2F11A );					// 輸入仕入を加算
//				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, Snd.Sn_2F11B );
//			}
//			else	{									// ３％なし
//				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Snd.Sn_2F9B, Snd.Sn_2F11B );								// 輸入仕入を加算
//			}
/*-------------*/
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, tmpSkjsyz );
/*-------------*/
		}
		else	{
			// 簡易課税 
			memset( mony, '\0', 6 );
			if( shinsw )	{
				// 込み
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz3, 0, 30, 0 );
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz5, 4, 40, 0 );
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz8, 9, 63, 0 );
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, mony );
			}
			else	{
				// 抜き
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz3, 1, 30, 0 );
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz5, 1, 40, 0 );
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, mony );
				memset( mony, '\0', 6 );
				m_Util.percent( mony, Kz8, 1, 63, 0 );
				m_Arith.l_add( Hj_SkData.Hj_SrKjTxMny, Hj_SkData.Hj_SrKjTxMny, mony );
			}
		}
		// 基本情報の更新
		REFUNDVOL &RefundVol = CRefundBaseDialog::refundvol;
		// 込／抜き
		RefundVol.KPSsgn &= 0xee;
		if( shinsw )	RefundVol.KPSsgn |= 0x11;
		// 実額入力
		RefundVol.KPSstx &= 0xfe;
		if( m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_GENSOKU ){
			RefundVol.KPSstx |= 0x01;
		}
		// 仕入取引等の明細
		memset( RefundStk, '\0', sizeof( REFUNDREC )*20 );
		// 損益計算書
		// ① 商品仕入金高等
		memmove( RefundStk[0].KPSvl1, Hj_SkData.Hj_PlKmk[0].KesMny,  6 );
		memmove( RefundStk[0].KPSvl2, Hj_SkData.Hj_PlKmk[0].NoKjMny, 6 );
		memmove( RefundStk[0].KPSvl3, Hj_SkData.Hj_PlKmk[0].KjTsMny, 6 );
		// ② 販売費・一般管理費
		memmove( RefundStk[1].KPSvl1, Hj_SkData.Hj_PlKmk[1].KesMny,  6 );
		memmove( RefundStk[1].KPSvl2, Hj_SkData.Hj_PlKmk[1].NoKjMny, 6 );
		memmove( RefundStk[1].KPSvl3, Hj_SkData.Hj_PlKmk[1].KjTsMny, 6 );
		// ③ 営業外費用
		memmove( RefundStk[2].KPSvl1, Hj_SkData.Hj_PlKmk[2].KesMny,  6 );
		memmove( RefundStk[2].KPSvl2, Hj_SkData.Hj_PlKmk[2].NoKjMny, 6 );
		memmove( RefundStk[2].KPSvl3, Hj_SkData.Hj_PlKmk[2].KjTsMny, 6 );
		// ④ その他
		memmove( RefundStk[3].KPSvl1, Hj_SkData.Hj_PlKmk[3].KesMny,  6 );
		memmove( RefundStk[3].KPSvl2, Hj_SkData.Hj_PlKmk[3].NoKjMny, 6 );
		memmove( RefundStk[3].KPSvl3, Hj_SkData.Hj_PlKmk[3].KjTsMny, 6 );
		// ⑤ 小計
		memmove( RefundStk[4].KPSvl1, Hj_SkData.Hj_PlKmk[5].KesMny,  6 );
		memmove( RefundStk[4].KPSvl2, Hj_SkData.Hj_PlKmk[5].NoKjMny, 6 );
		memmove( RefundStk[4].KPSvl3, Hj_SkData.Hj_PlKmk[5].KjTsMny, 6 );
		// 資産科目
		// ⑥ 必要経費
		memmove( RefundStk[5].KPSvl1, Hj_SkData.Hj_AsKmk[0].KesMny,  6 );
		memmove( RefundStk[5].KPSvl2, Hj_SkData.Hj_AsKmk[0].NoKjMny, 6 );
		memmove( RefundStk[5].KPSvl3, Hj_SkData.Hj_AsKmk[0].KjTsMny, 6 );
		// ⑦ 固定資産の取得
		memmove( RefundStk[6].KPSvl1, Hj_SkData.Hj_AsKmk[1].KesMny,  6 );
		memmove( RefundStk[6].KPSvl2, Hj_SkData.Hj_AsKmk[1].NoKjMny, 6 );
		memmove( RefundStk[6].KPSvl3, Hj_SkData.Hj_AsKmk[1].KjTsMny, 6 );
		// ⑧ その他
		memmove( RefundStk[7].KPSvl1, Hj_SkData.Hj_AsKmk[2].KesMny,  6 );
		memmove( RefundStk[7].KPSvl2, Hj_SkData.Hj_AsKmk[2].NoKjMny, 6 );
		memmove( RefundStk[7].KPSvl3, Hj_SkData.Hj_AsKmk[2].KjTsMny, 6 );
		// ⑨ 小計
		memmove( RefundStk[8].KPSvl1, Hj_SkData.Hj_AsKmk[5].KesMny,  6 );
		memmove( RefundStk[8].KPSvl2, Hj_SkData.Hj_AsKmk[5].NoKjMny, 6 );
		memmove( RefundStk[8].KPSvl3, Hj_SkData.Hj_AsKmk[5].KjTsMny, 6 );
		// ⑩ 課税仕入れ等の税額の合計
		memmove( RefundStk[9].KPSvl4, Hj_SkData.Hj_SrKjTxMny, 6 );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化によるデータクリア
//-----------------------------------------------------------------------------
int CH26SnRefundData::ClearDataByInit()
{
	memset( RefundJot, '\0', sizeof(REFUNDREC)*10 );
	memset( RefundExp, '\0', sizeof(REFUNDREC)*10 );
	memset( RefundStk, '\0', sizeof(REFUNDREC)*20 );
	memset( RefundTna, '\0', sizeof(REFUNDREC)*10 );
	memset( RefundKot, '\0', sizeof(REFUNDREC)*10 );
	JotTbl.tp_rnum = 0;		// 主な課税資産の譲渡等
	ExpTbl.tp_rnum = 0;		// 主な輸出取引等の明細
	TnaTbl.tp_rnum = 0;		// 主な棚卸資産・原材料等の取得
	KotTbl.tp_rnum = 0;		// 主な固定資産等の取得

	return 0;
}

int CH26SnRefundData::ClearData_Shiire( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{

	int	Max,HnMx,Cnt,Putno;
	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;


	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
		return ERR;
	}
	//// 管理部オープン
	//filter.Format( "KPSequ = %d", sn_seq );
	//if( pDBSyzAcc->ReFundVolOpen( pDBNpSub->m_database, filter ) == ERR ){
	//	delete( pDBSyzAcc );
	//	return( ERR );
	//}
	// 明細（次葉）部オープン
	if( pDBSyzAcc->ReFundRecOpen( pDBNpSub->m_database, filter ) == ERR ){
		pDBSyzAcc->ReFundVolClose();
		delete pDBSyzAcc;
		return ERR;
	}

	REFUNDVOL &RefundVol = CRefundBaseDialog::refundvol;
	// 管理部の読み込み
	//if( pDBSyzAcc->m_pRefundvol->GetRecordCount() != 0 )	{

	//	if( pDBSyzAcc->m_pRefundvol->MoveFirst() == ERR )	{
	//		pDBSyzAcc->ReFundVolClose();
	//		pDBSyzAcc->ReFundRecClose();
	//		delete( pDBSyzAcc );
	//		return( ERR );
	//	}

		//税抜・税込はマスターの設定に合わせる
		//pDBSyzAcc->m_pRefundvol->Edit();
		if( !(pDBNpSub->zvol->s_sgn1&0xc0) )	{
			if( pSnHeadData->IsSoleProprietor() ){
				//pDBSyzAcc->m_pRefundvol->KPSsgn = 0x01;	// 税抜・税込(3-1)		D0:OFF税抜  ON税込(損益) D4:OFF税抜  ON税込(資産)
				RefundVol.KPSsgn = 0x01;
			}else{
				//pDBSyzAcc->m_pRefundvol->KPSsgn = 0x11;
				RefundVol.KPSsgn = 0x11;
			}
		}else{
			//pDBSyzAcc->m_pRefundvol->KPSsgn = 0x00;
			RefundVol.KPSsgn = 0x00;
		}
		//pDBSyzAcc->m_pRefundvol->Update();
	//}

	// 3-(1) 仕入金額等の明細
	if( pSnHeadData->IsSoleProprietor() ){
		HnMx = 13;
	}
	else{
		HnMx = 10;
	}
	if( (Max = pDBSyzAcc->m_pRefundrec->GetRecordCount()) != 0 )	{
		Cnt = 0;
		while( 1 ) {
			if( !Cnt )	{
				if( pDBSyzAcc->m_pRefundrec->MoveFirst() == ERR )	{
					pDBSyzAcc->ReFundVolClose();
					pDBSyzAcc->ReFundRecClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
			}
			else	{
				if( pDBSyzAcc->m_pRefundrec->MoveNext() == ERR )	break;
			}
			Cnt++;
			if( pDBSyzAcc->m_pRefundrec->KPItem != ID_ICSSH_REF_STOCK_TYPE )	continue;
			// 管理部へ変換
			//pDBSyzAcc->m_pRefundrec->Edit();
			//pDBSyzAcc->m_pRefundrec->KPSvl1.Empty();
			//pDBSyzAcc->m_pRefundrec->KPSvl2.Empty();
			//pDBSyzAcc->m_pRefundrec->KPSvl3.Empty();
			//pDBSyzAcc->m_pRefundrec->KPSvl4.Empty();
			//pDBSyzAcc->m_pRefundrec->Update();

			Putno = pDBSyzAcc->m_pRefundrec->KPLine - 1;
			::ZeroMemory( RefundStk[Putno].KPSvl1, sizeof( RefundStk[Putno].KPSvl1 ));
			::ZeroMemory( RefundStk[Putno].KPSvl2, sizeof( RefundStk[Putno].KPSvl2 ));
			::ZeroMemory( RefundStk[Putno].KPSvl3, sizeof( RefundStk[Putno].KPSvl3 ));
			::ZeroMemory( RefundStk[Putno].KPSvl4, sizeof( RefundStk[Putno].KPSvl4 ));
		}
	}

	//pDBSyzAcc->ReFundVolClose();
	pDBSyzAcc->ReFundRecClose();
	
	delete pDBSyzAcc;

	return 0;
}