#include "StdAfx.h"

// CDBDinpView
#include "DBDinp.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"

#include "input1.h"
#include "DataLine.h"
#include "DinpSub.h"
#include "MstRw.h"
#include "external.h"

//#include "DinpPara.h"

extern CDipZmSub* pDBzm;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 新規ライン
extern	struct _DATA_LINE	INP_LINE;
// 五十音摘要
extern char save_50kana[16];

#ifdef DB_OLD_CLOSE
extern int SW_TK50;
extern int T50pag;
extern int T50pmax;
extern int T50dcnt;

	// 仕訳摘要
extern int SW_AutoTK;
extern int TATpage;
extern int TATpmax;
extern int TATdcnt;

	// 連想適用
extern int TKDflg;
//extern short RTKct[2];
//extern short RTblk[2];
extern int RTKdsw;
extern int RTpage;
extern char TKDsw;

	// 部門選択 02.27 /02
extern short BMNdsw;
extern int GCNT;

	// 科目名称パラメータ
extern int KPGcnt;
extern int CDPG;
extern int AC_sel;
extern int NOW_SEL;
extern int NOW_CNT;

#endif

	// 科目五十音
extern char saveITEM50kana[16];


	// 摘要枝番
//extern int BRNTKY_sel;	// 枝番摘要選択
extern struct _TKYBRN_SEL TKYBRN_SEL;	// 枝番摘要呼び出し

	// 枝番残高
extern CDBipEZREC BREC1, BREC2;

	// 仕訳関係
extern CDBINPDataRec CREC,SREC;


struct _DinpPara
{
	_DinpPara() {		//コンストラクタでデータレコード初期化 [07.24 /13]
		Para_Reset();
	};

#ifdef DB_OLD_CLOSE
	// 五十音摘要(22b)
	char save_50kana[10];
	int SW_TK50;
	int T50pag;
	int T50pmax;
	int T50dcnt;

	// 仕訳摘要(16b) ... 12.09 /02
	int SW_AutoTK;
	int TATpage;
	int TATpmax;
	int TATdcnt;

	// 連想適用(21b)
	int TKDflg;
	short RTKct[2];
	short RTblk[2];
	int RTKdsw;
	int RTpage;
	char TKDsw;

	// 部門選択(4b) ... 02.27 /02
	short	BMNdsw;
	WORD	BMNpage;

	// 科目五十音(14b)
	char saveITEM50kana[10];
	int GCNT;

	// 科目名称パラメータ(22+4b)
	int KPGcnt;
	int CDPG;
	int AC_sel;
	int NOW_SEL;
	int NOW_CNT;
#endif

	char saveITEM50kana[16];

	// 枝番未入力チェック状況サイン
	int m_NOBRNCHK_CND;
	int m_BMKJICHK_CND;

	// 摘要枝番
	struct _TKYBRN_SEL TKYBRN_SEL;	// 摘要枝番呼び出し
	int BRNTKY_sel;	// 枝番摘要選択

	// 枝番残高
	CDBipEZREC BREC1, BREC2;

	//仕訳関係
	CDBINPDataRec CREC,SREC;

	void Para_Reset() {
		int ofs = offsetof( struct _DinpPara, TKYBRN_SEL );
		memset( this, 0, ofs );
		TKYBRN_SEL.TS_Reset();
		BRNTKY_sel = 0;
		BREC1.Reset();	BREC2.Reset();
		CREC.Reset();	SREC.Reset();
	}
};

static struct _DinpPara SavePara[4];	/*	[0] ... _APPEND
											[1] ... _RETREV
											[2] ... _KEYSCAN
											[3] ... _MODEFY
										*/

// パラメータアドレスリード
static struct _DinpPara* getDinpPara( int inp_mode )
{
	switch( inp_mode )
	{
	case _APPEND:	//入力
		return &SavePara[0];
	case _RETREV:	//スキャンキー登録
		return &SavePara[1];
	case _KEYSCAN:	//キー検索による修正
		return &SavePara[2];
	case _MODEFY:	//行戻りによる修正
		return &SavePara[3];
	default:
		return NULL;
	}
}

// セーブパラメータ
void CDBDinpView::SaveDinpPara( int inp_mode )
{
	struct _DinpPara *para;

	VERIFY(para = getDinpPara( inp_mode ));


	// 科目五十音
	memmove( (char*)para->saveITEM50kana, (char*)saveITEM50kana, sizeof(para->saveITEM50kana));

	// 枝番摘要
	para->TKYBRN_SEL = TKYBRN_SEL;
	para->BRNTKY_sel = BRNTKY_sel;

	// 枝番残高
	para->BREC1 = BREC1;
	para->BREC2 = BREC2;

	//仕訳関係
	para->CREC = CREC;
	if( inp_mode == _APPEND && m_InsLine != -1 ) {
		//保存しない
	}
	else {
		para->SREC = SREC;
	}

	// 枝番未入力チェック状況サイン
	para->m_NOBRNCHK_CND = m_NOBRNCHK_CND;
	para->m_BMKJICHK_CND = m_BMKJICHK_CND;
}

// セーブパラメータ
void CDBDinpView::SaveDinpParaSREC( int inp_mode )
{
	struct _DinpPara *para;

	VERIFY(para = getDinpPara( inp_mode ));

	//仕訳関係
	para->SREC = SREC;
}



// リセットパラメータ
void CDBDinpView::ReSetDinpPara( int inp_mode )
{
	struct _DinpPara *para;

	VERIFY(para = getDinpPara( inp_mode ));

	// 科目五十音
	memmove( (char*)saveITEM50kana, (char*)para->saveITEM50kana, sizeof(para->saveITEM50kana));

	// 枝番摘要
	TKYBRN_SEL = para->TKYBRN_SEL;
	BRNTKY_sel = para->BRNTKY_sel;

	// 枝番残高
	BREC1 = para->BREC1;
	BREC2 = para->BREC2;

	//仕訳関係
	CREC = para->CREC;
	SREC = para->SREC;

	// 枝番未入力チェック状況サイン
	m_NOBRNCHK_CND = para->m_NOBRNCHK_CND;
	m_BMKJICHK_CND	 = para->m_BMKJICHK_CND;
}

// イニシャライズパラメータ
void CDBDinpView::InizDinpPara( int inp_mode )
{
	struct _DinpPara *para;

	VERIFY(para = getDinpPara( inp_mode ));

	// All Zero Clear
	para->Para_Reset();

	// 仕訳関係
	para->CREC.Reset();
	para->SREC.Reset();
}



// 入力データ等セーブエリア
struct _DinpData {
	_DinpData() {		//コンストラクタでデータレコード初期化 [06.27 /13]
//		CREC.Reset();
//		SREC.Reset();
//		SCAN_rec.Reset();
		inpline_inz( &INP_LINE );	// 入力行が 0000 表示となる場合があったため、初期化
	}

	int DDATA_LN;
	int NOW_PN;
	int SAVE_PN;
	struct _DATA_LINE INP_LINE;	//入力行管理

	//仕訳データは、_DinpPara で統合する。
//	CDBINPDataRec CREC;
//	CDBINPDataRec SREC;
//	CDBINPDataRec SCAN_rec;
};

// 入力データ等セーブエリア
static struct _DinpData SaveData[2];;	/*	[0] ... _APPEND
											[1] ... _MODEFY
											*/

// パラメータアドレスリード
void *CDBDinpView::getDinpData( int inp_mode )
{
	switch( inp_mode )
	{
	case _APPEND:	//入力
		return &SaveData[0];
//	case _RETREV:	//スキャンキー登録		07.01 /98 Cut!
//		return &SaveData[1];
	case _MODEFY:	//修正
		return &SaveData[1];
	default:
		return NULL;
	}
}

// データセーブ
void CDBDinpView::SaveDinpData( int inp_mode )
{

	struct _DinpData* para;

	para = (struct _DinpData *)getDinpData( inp_mode );
	if( !para )
		return;

	para->DDATA_LN = get_nowln();
	para->NOW_PN = get_nowpn();
	para->SAVE_PN = SAVE_PN;

	para->INP_LINE = INP_LINE;

#ifdef CLOSE
	para->CREC = CREC;
	if( inp_mode == _APPEND && m_InsLine != -1 ) {
		//保存しない
	}
	else {
		para->SREC = SREC;
	}
//	para->SCAN_rec = SCAN_rec;
#endif

	//入力行管理は、_APPEND・_MODEFY どちらも同じものをセットする。(0000 表示となる場合があった)
	if( inp_mode == _APPEND ) {
		para = (struct _DinpData *)getDinpData( _MODEFY );
		if( !para )	return;
		para->INP_LINE = INP_LINE;
	}
	else if( inp_mode == _MODEFY ) {
		para = (struct _DinpData *)getDinpData( _APPEND );
		if( !para )	return;
		para->INP_LINE = INP_LINE;
	}
}


void CDBDinpView::SaveDinpDataNOW_PN( int inp_mode )
{
	struct _DinpData* para;

	para = (struct _DinpData *)getDinpData( inp_mode );
	if( !para )
		return;

	para->NOW_PN = get_nowpn();
}


#ifdef CLOSE

// データセーブ
void CDBDinpView::SaveDinpDataSREC( int inp_mode )
{
	struct _DinpData* para;

	para = (struct _DinpData *)getDinpData( inp_mode );
	if( !para )
		return;

	para->SREC = SREC;
}
#endif



// データセット
void CDBDinpView::SetDinpData( int inp_mode )
{
	struct _DinpData* para;

	para = (struct _DinpData *)getDinpData( inp_mode );
	if( !para )
		return;

	set_nowln( para->DDATA_LN );
	set_nowpn( para->NOW_PN );

	SAVE_PN = para->SAVE_PN;

	INP_LINE = para->INP_LINE;

//	CREC = para->CREC;
//	SREC = para->SREC;

}


// 他のビューで追加されたデータにより、保存時のモードと
// 実際に表示するモードが異なる場合の関数。
//
void CDBDinpView::SetDinpDataLite( int inp_mode )
{
	struct _DinpData* para;

	para = (struct _DinpData *)getDinpData( inp_mode );
	if( !para )
		return;

	INP_LINE = para->INP_LINE;

	struct _DinpPara *dpar;

	dpar = getDinpPara( inp_mode );
	if( dpar ) {
		CREC = dpar->CREC;
		SREC = dpar->SREC;
	}
}


// データセット(SEQ のみ) フォークで仕訳が追加されたときのため
void CDBDinpView::SaveDinpData_SEQ( int inp_mode, int seq )
{
	struct _DinpData* para;

	para = (struct _DinpData *)getDinpData( inp_mode );
	if( para ) {
		para->INP_LINE._XXDTA.dd_seq = seq;
//		para->CREC.m_seq = seq;
	}

	// 08.19 /09
	struct _DinpPara *dpar;

	dpar = getDinpPara( inp_mode );
	if( dpar ) {
		dpar->CREC.m_seq = seq;;
	}
}


//
// 入力タイプ等セーブエリア
struct _INPtype {
	WORD Line;
	char DEBT_BRN_InpType;
	char CRED_BRN_InpType;
};

static struct _INPtype SaveType[3] = {0};	/*	[0] ... _APPEND
												[1] ... _MODEFY
												[2] ... _KEYSCAN
											*/
// パラメータアドレスリード
void *CDBDinpView::getINPtype( int inp_mode )
{
	switch( inp_mode )
	{
	case _APPEND:	//入力
		return &SaveType[0];
	case _MODEFY:	//修正
		return &SaveType[1];
	case _KEYSCAN:	//キースキャン
		return &SaveType[2];
	default:
		return NULL;
	}
}

// タイプセーブ
void CDBDinpView::SaveInpType( int inp_mode, int ln )
{
	struct _INPtype* para;

	para = (struct _INPtype *)getINPtype( inp_mode );
	if( !para )
		return;

	para->Line = ln;
	para->DEBT_BRN_InpType = _getLINE_DATA( ln )->LINE_CND[ DEBT_PN ].INP_type;
	para->CRED_BRN_InpType = _getLINE_DATA( ln )->LINE_CND[ CRED_PN ].INP_type;
}

// タイプリセット
void CDBDinpView::SetInpType( int inp_mode )
{
	struct _INPtype* para;
	int ln;

	para = (struct _INPtype *)getINPtype( inp_mode );
	if( !para )
		return;

	ln = para->Line;
	_getLINE_DATA( ln )->LINE_CND[ DEBT_PN ].INP_type = para->DEBT_BRN_InpType;
	_getLINE_DATA( ln )->LINE_CND[ CRED_PN ].INP_type = para->CRED_BRN_InpType;
}



void CDBDinpView::ReCosel_DinpData()
{
	SaveDinpPara( _APPEND );
	SaveDinpPara( _MODEFY );
}

