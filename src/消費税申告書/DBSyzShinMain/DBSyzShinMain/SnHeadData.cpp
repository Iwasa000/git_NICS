#include "StdAfx.h"
#include "SnHeadData.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CSnHeadData::CSnHeadData()
{
	Sn_VER		= 0;
	Sn_SKKBN	= 0;
	Sn_KOJIN	= 0;
	Sn_KOANB	= 0;
	memset( Sn_ITIREN, '\0', sizeof(Sn_ITIREN) );
	memset( Sn_SEIRI, '\0', sizeof(Sn_SEIRI) );
	memset( Sn_ZSEIRI, '\0', sizeof(Sn_ZSEIRI) );
	Sn_KDAYS	= 0;
	Sn_KDAYE	= 0;
	Sn_MDAYS	= 0;
	Sn_MDAYE	= 0;
	Sn_PRDAY	= 0;
	Sn_SPECIAL	= 0;
	memset( Sn_ZMSYO, '\0', sizeof(Sn_ZMSYO) );
	memset( Sn_ZIP, '\0', sizeof(Sn_ZIP) );
	memset( Sn_SYOKN, '\0', sizeof(Sn_SYOKN) );
	memset( Sn_TELNO1, '\0', sizeof(Sn_TELNO1) );
	memset( Sn_TELNO2, '\0', sizeof(Sn_TELNO2) );
	memset( Sn_TELNO3, '\0', sizeof(Sn_TELNO3) );
	memset( Sn_NOZEI, '\0', sizeof(Sn_NOZEI) );
	memset( Sn_CONAM, '\0', sizeof(Sn_CONAM) );
	memset( Sn_COFRI, '\0', sizeof(Sn_COFRI) );
	memset( Sn_DIHYO, '\0', sizeof(Sn_DIHYO) );
	memset( Sn_DIFRI, '\0', sizeof(Sn_DIFRI) );
	memset( Sn_KEIRI, '\0', sizeof(Sn_KEIRI) );
	Sn_KAPPU	= 0;
	Sn_NOBE		= 0;
	Sn_KOJI		= 0;
	Sn_GENKN	= 0;
	Sn_TUMIAG	= 0;
	Sn_KZTNSK	= 0;
	Sn_MCOUNT	= 0;
	Sn_JCOUNT	= 0;
	Sn_TOKUR	= 0;
	Sn_UPERS	= 0;
	Sn_TYPE		= 0;
	Sn_BYsw		= 0;
	memset( Sn_BANK, '\0', sizeof(Sn_BANK) );
	memset( Sn_SITEN, '\0', sizeof(Sn_SITEN) );
	memset( Sn_YOKIN, '\0', sizeof(Sn_YOKIN) );
	memset( Sn_YUBIN, '\0', sizeof(Sn_YUBIN) );
	memset( Sn_KOZA, '\0', sizeof(Sn_KOZA) );
	memset( Sn_YUNUM1, '\0', sizeof(Sn_YUNUM1) );
	memset( Sn_YUNUM2, '\0', sizeof(Sn_YUNUM2) );
	Sn_FINANCE	= 0;
	Sn_CHAIN	= 0;

	m_Arith.l_defn( 0x16 );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CSnHeadData::~CSnHeadData()
{
}


//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );

	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
	}
	if( pDBNpSub->szsnk->st == -1 ){
		// 新規
		return 0;
	}

	CSZSNK *pSZSNK = pDBNpSub->szsnk;

	// データ読込み
	Sn_VER		= pSZSNK->Sn_VER;												// ファイルバージョン
	Sn_SKKBN	= pSZSNK->Sn_SKKBN;												// 申告区分
	Sn_KOJIN	= pSZSNK->Sn_KOJIN;												// 個人以外の事業者　ON:12ヶ月按分
	Sn_KOANB	= pSZSNK->Sn_KOANB;												// 個人事業者　ON:月按分　OFF:12ヶ月按分	
	m_Util.asci_to_bcd( Sn_ITIREN, &pSZSNK->Sn_ITIREN, 8 );						// 一連番号
	m_Util.asci_to_bcd( Sn_SEIRI, &pSZSNK->Sn_SEIRI, 8 );						// 整理番号

	unsigned char	Work[20];
	memset( Work, '\0', sizeof( Work ) );
	&pSZSNK->Sn_ZSEIRI.TrimRight();												// スペースカット
	m_Util.cstring_to_char( Work, pSZSNK->Sn_ZSEIRI, 8 );						// 納税署整理番号
	m_Arith.l_pack( Sn_ZSEIRI, Work, 8 );
	Sn_KDAYS = pSZSNK->Sn_KDAYS;												// 課税期間　自
	Sn_KDAYE = pSZSNK->Sn_KDAYE;												// 課税期間　至
	Sn_MDAYS = pSZSNK->Sn_MDAYS;												// 中間期間　自
	Sn_MDAYE = pSZSNK->Sn_MDAYE;												// 中間期間　至
	Sn_PRDAY = pSZSNK->Sn_PRDAY;												// 提出年月日

	Sn_SPECIAL	= pSZSNK->Sn_SPECIAL;											// 特殊処理サイン
	m_Util.cstring_to_char( Sn_ZMSYO, pSZSNK->Sn_ZMSYO, 12 );					// 税務署名
	m_Util.cstring_to_char( Sn_ZIP, pSZSNK->Sn_ZIP, 8 );						// 郵便番号
	m_Util.cstring_to_char( Sn_SYOKN, pSZSNK->Sn_SYOKN, 3 );					// 所管

	char	telbf[64] = {0};
	memset( telbf, '\0', sizeof( telbf ) );
	if( pSZSNK->Sn_TELNO.GetLength() == 0 ){
		pSZSNK->Sn_TELNO = _T ("                " );
	}
	m_Util.cstring_to_char( (unsigned char *)telbf, pSZSNK->Sn_TELNO, 16 );		// 電話番号
	m_Util.cstring_to_char( Sn_TELNO1, telbf, 6 );								// 電話番号　市外
	m_Util.cstring_to_char( Sn_TELNO2, &telbf[7], 4 );							// 電話番号　市内
	m_Util.cstring_to_char( Sn_TELNO3, &telbf[12], 4 );							// 電話番号　個人

	m_Util.cstring_to_char( Sn_NOZEI, pSZSNK->Sn_NOZEI, 80 );					// 納税地

	if( kjlen( Sn_NOZEI, 80 ) == 0 ){
		memset( Sn_NOZEI, '\0', 80 );
	}

	m_Util.cstring_to_char( Sn_CONAM, pSZSNK->Sn_CONAM, 40 );					// 氏名又は屋号
	m_Util.cstring_to_char( Sn_COFRI, pSZSNK->Sn_COFRI, 80 );					// 氏名又は屋号フリガナ
	m_Util.cstring_to_char( Sn_DIHYO, pSZSNK->Sn_DIHYO, 40 );					// 代表者氏名
	m_Util.cstring_to_char( Sn_DIFRI, pSZSNK->Sn_DIFRI, 80 );					// 代表者氏名フリガナ
	m_Util.cstring_to_char( Sn_KEIRI, pSZSNK->Sn_KEIRI, 40 );					// 経理担当者氏名
	Sn_KAPPU	= pSZSNK->Sn_KAPPU;												// 割賦基準の適用
	Sn_NOBE		= pSZSNK->Sn_NOBE;												// 延払基準の適用
	Sn_KOJI		= pSZSNK->Sn_KOJI;												// 工事進行基準の適用
	Sn_GENKN	= pSZSNK->Sn_GENKN;												// 現金主義会計の適用
	Sn_TUMIAG	= pSZSNK->Sn_TUMIAG;											// 規則22条1項の適用
	Sn_KZTNSK	= pSZSNK->Sn_KZTNSK;											// 課税期間短縮
	Sn_MCOUNT	= pSZSNK->Sn_MCOUNT;											// 中間期間　年回数
	Sn_JCOUNT	= pSZSNK->Sn_JCOUNT;											// 中間申告　現在申告状態
	Sn_TOKUR	= pSZSNK->Sn_TOKUR;												// 特例計算適用サイン
	Sn_UPERS	= pSZSNK->Sn_UPERS;												// 課税売上割合90%以上のサイン
	Sn_TYPE		= pSZSNK->Sn_TYPE;												// 施行署及び施行署以外のサイン
	Sn_BYsw		= pSZSNK->Sn_BYsw;												// 金融機関選択サイン
	m_Util.cstring_to_char( Sn_BANK, pSZSNK->Sn_BANK, 20 );						// 銀行名称
	m_Util.cstring_to_char( Sn_SITEN, pSZSNK->Sn_SITEN, 20 );					// 支店名称
	m_Util.cstring_to_char( Sn_YOKIN, pSZSNK->Sn_YOKIN, 8 );					// 預金種別
	m_Util.cstring_to_char( Sn_YUBIN, pSZSNK->Sn_YUBIN, 16 );					// 郵便局名
	m_Util.asci_to_bcd( Sn_KOZA, &pSZSNK->Sn_KOZA, 10 );						// 口座番号
	m_Util.cstring_to_char( Sn_YUNUM1, pSZSNK->Sn_YUNUM1, 5 );					// 郵便貯金番号①
	m_Util.cstring_to_char( Sn_YUNUM2, pSZSNK->Sn_YUNUM2, 8 );					// 郵便貯金番号②
	Sn_FINANCE	= pSZSNK->Sn_FINANCE;											// 銀行、金庫等
	Sn_CHAIN	= pSZSNK->Sn_CHAIN;												// 本店、支店等

	return 0;
}