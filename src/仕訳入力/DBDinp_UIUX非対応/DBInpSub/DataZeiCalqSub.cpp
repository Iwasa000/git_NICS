#include "stdafx.h"
#include "DBinpSubFunc.h"
#include "DataZeiCalq.h"

// モード
#define _MODE_URI				0	// 売上(仮受消費税)
#define _MODE_SIRE				1	// 仕入(仮払消費税)

//-----------------------------------------------------------------------------
// 内部使用 科目名称
//-----------------------------------------------------------------------------
struct InKNREC {
	CString		kncod;
	CByteArray	knsgn;
	char		knatt;	// 貸借属性
	char		kninp;	// 入力用内／外区分 １＝内 ２＝外 ３＝別(=01.21 /08) ０＝設定なし
};

typedef CMap< DWORD, DWORD, struct InKNREC*, struct InKNREC*> CKnMap;

// インボイス対応
// XCDBDATAクラスとCDATASクラスをカット
// ZIM15_Syz.dllにも同名のクラスがあり、そちらのクラスが拡張された
// クラス名が同じであることが原因なのか、ZIM15_Syz.dllだけを入れ替えると不正終了するようになった
// おそらくクラスのサイズが変わったことが原因
// XCDBDATAクラスとCDATASクラスはこのプロジェクト内で使用されていないためカットする
// ※dbsysub.hのインクルードもカットしたいが、このヘッダー内の構造体や定義は使用されているため、
// ZIM15_Syz.dllと同様の拡張を行う
#ifdef _CUT
//=============================================================================
// 消費税用 拡張仕訳データ クラス
//=============================================================================
class XCDBDATA {
public:
	XCDBDATA();
	virtual ~XCDBDATA();
	VOID	clrXCDBDATA();

	VOID*		pdata;

	int			st;
	int			seq;
	BYTE		ksign;
	int			ddate;
	BYTE		mofs;
	int			cno;
	int			dbmn;
	CString		dkno;
	CString		dbt;
	int			dbr;
	int			cbmn;
	CString		ckno;
	CString		cre;
	int			cbr;
	CString		val;
	CString		zei;
	CByteArray	dsign;
	int			owner;
	BYTE		dtype;
	CString		tekiyo;
	int			imgsq;
	CString		snumber;
	int			tdate;
	int			edate;
	int			udate;
	CString		taika;			// 対価金額
};


//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------

// 作業種別
typedef enum {
	ID_NOSEL_CHG_DTYPE = 0,
	ID_CDATA_CHG_DTYPE = 1,	// CDATA   変換
	ID_CDBDATA_CHG_DTYPE,		// CDBDATA 変換
	ID_CSYDATA_CHG_DTYPE,		// CSYDATA 変換
	ID_CLEDGER_CHG_DTYPE,		// CLEDGER 変換
} EnumIdDBsyzChgDataType;

//=============================================================================
// 消費税対象仕訳データ変換
//=============================================================================
class CDATAS
{
public:
	CDATAS();
	~CDATAS();

public:
	XCDBDATA* to_CDBDATA(CDATA* data);
	XCDBDATA* to_CDBDATA(CDBDATA* ddata);
	XCDBDATA* to_CDBDATA(CLEDGER* leddata);
	CString* zei_adr();
	VOID to_CDBDATA(VOID);
	int CDBDATA_to();

public:
	//
	// セーブ ＆ 作業 バッファ
/*- '13.10.11 -*/
//	int sel;			// 1 ～
/*-------------*/
	int		m_ChgDtype;		// 1 ～
/*-------------*/
	CDATA	*pCDATA;		// セーブデータ	sel = 1
	CDBDATA *pCDBDATA;	// セーブデータ	sel = 2
//	CSYDATA *pCSYDATA;	// セーブデータ	sel = 3
	CLEDGER *pCLEDGER;	// セーブデータ	sel = 4

	XCDBDATA XDDATA;
	XCDBDATA* pDDATA;	// 作業データ
};
#endif


#include "dbsysub.h"


class CDataZeiCalqSub
{
public:
	CDataZeiCalqSub();
	virtual ~CDataZeiCalqSub();

public:
	int		DZCIniz(CDBZmSub* zmsub);
	int		DZCClose();

	LPCTSTR GetDefZeiData(CDBipDataRec* pdata);
	LPCTSTR GetDefZeiData(CDBINPDataRec* pdata);

private:
	LPCTSTR zeiclq(LPCTSTR sou, int zritu, int zkbn, int uri_sire, int men_type);
	LPCTSTR zeiclq_mz(LPCTSTR sou, int zritu, int zkbn, int uri_sire, int mzsrkbn);
	int		remtype(int uri_sire);

	__int64 atoi64(LPCTSTR num_str);
	LPCTSTR i64toa(__int64 i64);

	DWORD	_get_ddata_code(CDBINPDataRec* ddata, DWORD sel_code);
	int		_get_syattr(struct _DBSY_ATTR* syattr, LPCTSTR kamok, DWORD* after_fnc_code = NULL);
	int		_to_DBSY_ATTR(struct _DBSY_ATTR* syattr, CKNREC* pknrec);
	int		_to_DBSY_ATTR(struct _DBSY_ATTR* syattr, struct InKNREC* pknrec);

	int		KnOnMemry(void);
	void	KnOffMemry(void);
	struct InKNREC* findknrec(LPCTSTR kamok);


	static BOOL isSYAFUKU_SYUKYO(BYTE apno) {
		return ((apno == 0x52) || (apno == 0x59));	// 社会福祉, 宗教
	}
	int		IsKensetsuSpMaster();
	int		IsKensetsuMaster();
	int		IsMiseiKojiType();

	int		istokteim();
	int		isgensokm();
	int		iskobetm();
	int		iskanim();
	int		kihon_ukbn_attr();

	DWORD	_kamok_typ(LPCTSTR kamok, struct _DBSY_ATTR* syattr, int debt_cred);
	DWORD	_to_siwake_kbn_code(struct _DBSY_ATTR* syattr);
	int		IsTkkzSiire(struct _DBSY_ATTR *pSyattr);

	CString get_KARIBARAI();
	CString get_KARIUKE();

	DWORD chk_DEBTTANA(struct _DBSY_ATTR* db_syattr, LPCTSTR debt);
	DWORD chk_CREDTANA(struct _DBSY_ATTR* cr_syattr, LPCTSTR cred);
	DWORD chk_KOTEISISAN(struct _DBSY_ATTR* db_syattr);
	DWORD chk_MISEIKOUJI(CString& strKmk, struct _DBSY_ATTR* syattr);

	int		drec_to_sub(struct _DBSYSUB* sub, CDBINPDataRec* pdata);
	int		_sub_zei_clq(struct _DBSYSUB* sub, CDBINPDataRec* pdata);

protected:
	BOOL	_yuka(struct _DBSYSUB* sub);
	int		is_sisan_syutok(struct _DBSY_ATTR* syattr);
	int		is_sisan_jyoto(struct _DBSY_ATTR* syattr);

	// 作業バッファ関数
	int sub_inz(struct _DBSYSUB* sub);
	int _sub_date_inz(struct _DBSYSUB* sub);
	int _sub_debt_inz(struct _DBSYSUB* sub);
	int _sub_cred_inz(struct _DBSYSUB* sub);
	int _sub_kmk_inz(struct _DBSYSUB* sub, int debt_cred);
	int _sub_dbcr_inz(struct _DBSYSUB* sub);
	int _sub_value_inz(struct _DBSYSUB* sub);
	int _sub_taikaval_inz(struct _DBSYSUB* sub);

private:
	BOOL	m_Ready;	// 準備ＯＫサイン
	// サインセット
	BOOL&	isReady() { return m_Ready; }

	CDBZmSub*	m_pZm;
	CZVOLUME*	m_vol;
	CSZVOL		*pSzVol;

	CString		m_strZei;

	int			m_KnrecMax;
	CPtrArray	InKNRECary;
	CKnMap		m_KnMap;

	int			m_stKensetsuSpMaster;
	bool		m_bInvMaster, m_bInvUse;

	char		rsv[512];
};



// 仕訳データから、デフォルト税額を計算する
CDataZeiCalqSub::CDataZeiCalqSub()
{
	m_pZm = NULL;
	m_vol = NULL;
	pSzVol = NULL;
	m_KnrecMax = 0;
	m_stKensetsuSpMaster = -1;
	m_bInvMaster = m_bInvUse = false;

	// 準備ＯＫサイン初期化
	isReady() = FALSE;
}


CDataZeiCalqSub::~CDataZeiCalqSub()
{

}

//------------------------------------------------------------
//概要：クラスの初期化
//引数：m_pZm		財務クラスポインタ(オープン済み)
//------------------------------------------------------------
int CDataZeiCalqSub::DZCIniz(CDBZmSub* zmsub)
{
	m_pZm = zmsub;

	m_vol = m_pZm->zvol;
	pSzVol = m_pZm->szvol;

// 科目名称をメモリーにあげる
	if (KnOnMemry() == -1) {
		return -1;
	}

	isReady() = TRUE;

	// インボイス対応
	CZmGen8 zmGen;
	// バージョンアップマスターか判定
	m_bInvMaster = zmGen.IsInvoiceVupMaster(m_pZm);
	if( m_bInvMaster ) {
#ifdef CLOSE
		if( (m_vol->s_sgn2 & 0x0f) >= 2 ) {
			// 簡易課税の場合は免税事業者からの課税仕入れと登録番号を非表示にするため、インボイス期間外としておく
			m_bInvUse = false;
		}
		else {
#endif
			// インボイス期間マスターか
			m_bInvUse = zmGen.IsAbleToUseInvoice(m_pZm);
//		}
	}
	return 0;
}

//------------------------------------------------------------
//概要：クラスの終了処理
//------------------------------------------------------------
int CDataZeiCalqSub::DZCClose()
{
	m_pZm = NULL;
	m_vol = NULL;
	pSzVol = NULL;

	//メモリー開放
	KnOffMemry();
	m_stKensetsuSpMaster = -1;

	isReady() = FALSE;

	return 0;
}


//------------------------------------------------------------
//概要：仕訳データの消費税サインからデフォルトの税額を返送する
//引数：pdata	税額を計算する仕訳データ
//------------------------------------------------------------
LPCTSTR CDataZeiCalqSub::GetDefZeiData(CDBipDataRec* pdata)
{
	if( !isReady() )	return NULL;

	//CDBipDataRec → CDBINPDataRec に変換
	CDBINPDataRec exdata;

	convNewDataRec(pdata, &exdata);

	m_strZei.Empty();

	struct _DBSYSUB sywork;

	sub_inz(&sywork);

	drec_to_sub(&sywork, &exdata);

	_sub_zei_clq(&sywork, &exdata);

	return m_strZei;
}



//------------------------------------------------------------
//概要：仕訳データの消費税サインからデフォルトの税額を返送する
//引数：pdata	税額を計算する仕訳データ
//------------------------------------------------------------
LPCTSTR CDataZeiCalqSub::GetDefZeiData(CDBINPDataRec* pdata)
{
	if (!isReady())	return NULL;

	m_strZei.Empty();

	struct _DBSYSUB sywork;

	sub_inz(&sywork);

	drec_to_sub(&sywork, pdata);

	_sub_zei_clq(&sywork, pdata);

	return m_strZei;
}


int CDataZeiCalqSub::drec_to_sub(struct _DBSYSUB* sub, CDBINPDataRec* pdata)
{
	sub->_SY_DATE = pdata->m_ddate;
	sub->is_DATE_RGST() = CND_REGIST;

	//借方科目セット
	struct _DBSY_KAMOK	*sub_kamok;
	sub_kamok = &sub->_SY_DEBT;
	_get_syattr(&sub_kamok->KM_SY_ATTR._DEF_ATTR, pdata->m_dbt);
	// 科目をセット
	strcpy_s(sub_kamok->SY_KMK._KMKCD, pdata->m_dbt);
	// 枝番をセット
	sub_kamok->SY_KMK._KZECD = pdata->m_dbr;
	// 部門をセット
	sub_kamok->SY_KMK._KZBCD = pdata->m_dbmn;
	// 科目の 仮受／仮払消費税 その他 または 通常科目 をチェックしてコードをセット
	sub_kamok->SY_KMK._KMKTYP = _kamok_typ(pdata->m_dbt, &sub_kamok->KM_SY_ATTR._DEF_ATTR, _CTL_DEBT);

	sub->is_DEBT_RGST() = CND_REGIST;

	//貸方科目セット
	sub_kamok = &sub->_SY_CRED;
	_get_syattr(&sub_kamok->KM_SY_ATTR._DEF_ATTR, pdata->m_cre);
	strcpy_s(sub_kamok->SY_KMK._KMKCD, pdata->m_cre);
	sub_kamok->SY_KMK._KZECD = pdata->m_cbr;
	sub_kamok->SY_KMK._KZBCD = pdata->m_cbmn;
	sub_kamok->SY_KMK._KMKTYP = _kamok_typ(pdata->m_cre, &sub_kamok->KM_SY_ATTR._DEF_ATTR, _CTL_CRED);

	sub->is_CRED_RGST() = CND_REGIST;

	//金額
	CArith	ar(0x16);
	char valstr[32] = { 0 };

	ar.l_print(valstr, pdata->m_val, "sssssssssss9");
	strcpy_s(sub->_SY_VALUE, (LPCTSTR)valstr);

	DWORD taika = _get_ddata_code(pdata, MSK_CD_TAIKAKBN);
	if (taika == CD_TAIKA) {
		ar.l_print(valstr, pdata->m_taika, "sssssssssss9");
		strcpy_s(sub->_SY_TAIKAVAL, (LPCTSTR)valstr);
	}

	sub->is_VALUE_RGST() = CND_REGIST;

	return 0;
}


//-----------------------------------------------------------------------------
// 税額計算
//-----------------------------------------------------------------------------
// 引数	sub		：	作業用バッファ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_zei_clq(struct _DBSYSUB* sub, CDBINPDataRec* pdata)
{
//	struct _DBSY_KAMOK* sub_kamok = &sub->_SY_DBCR;
	char*	val = sub->_SY_VALUE;
	char*	tkval = sub->_SY_TAIKAVAL;
	char*	sival = sub->_SY_VALUE;

	DWORD kzkbn, zritu, zkbn, taika, swkkbn, mentype;
	kzkbn = _get_ddata_code(pdata, MSK_CD_KAZEI);	// 課税区分コード
	zritu = _get_ddata_code(pdata, MSK_CD_RITU);	// 税率コード
	zkbn = _get_ddata_code(pdata, MSK_CD_ZEI);		// 税区分

	taika = _get_ddata_code(pdata, MSK_CD_TAIKAKBN);
	swkkbn = _get_ddata_code(pdata, MSK_CD_SWKBN);
	mentype = _get_ddata_code(pdata, MSK_CD_SPSIGN);

	BOOL bYuka = _yuka(sub);
	BOOL bHikaYuka = FALSE;
	if (bYuka && kzkbn == CD_HIKAZ)
		bHikaYuka = TRUE;

	// 対価の税額計算
	if (taika == CD_TAIKA || (isSYAFUKU_SYUKYO(m_vol->apno) && bHikaYuka) ) {

		if ((kzkbn == CD_KAZEI) || bHikaYuka ) {

			// 税額計算
			if (bHikaYuka) {
				// 対価の 非課税分 算出
				if (isSYAFUKU_SYUKYO(m_vol->apno)) {
					strcpy_s(sub->_SY_ZEIGAK, zeiclq(sival, zritu, CD_SOTOZ, _MODE_URI, mentype));
				}
				else {
					strcpy_s(sub->_SY_TAIKAZEI, zeiclq(tkval, zritu, CD_SOTOZ, _MODE_URI, mentype));
				}
				m_strZei = isSYAFUKU_SYUKYO(m_vol->apno) ? sub->_SY_ZEIGAK : sub->_SY_TAIKAZEI;
			}
			else {
				// 対価の 税額 算出
				strcpy_s(sub->_SY_TAIKAZEI, zeiclq(tkval, zritu, zkbn, _MODE_URI, mentype));
				m_strZei = sub->_SY_TAIKAZEI;
			}
		}
		else {
			memset(sub->_SY_TAIKAZEI, NULL, sizeof(sub->_SY_TAIKAZEI));
		}

		// 仕訳税額のクリア
		if (isSYAFUKU_SYUKYO(m_vol->apno)) {
			memset(sub->_SY_TAIKAZEI, NULL, sizeof(sub->_SY_TAIKAZEI));	// 対価税額のｸﾘｱ
		}
		else {
			memset(sub->_SY_ZEIGAK, NULL, sizeof(sub->_SY_ZEIGAK));		// 仕訳税額のｸﾘｱ
		}
	}
	// 仕訳金額の税額計算
	else if (kzkbn == CD_KAZEI) {											// 課税
		// 税額計算
		struct _DBSY_ATTR *db_syattr, *cr_syattr;
		int		uri_sire;

		// 端数処理判定
		// 借方 科目消費税属性
		db_syattr = &sub->_SY_DEBT.KM_SY_ATTR._DEF_ATTR;

		// 貸方 科目消費税属性
		cr_syattr = &sub->_SY_CRED.KM_SY_ATTR._DEF_ATTR;

		// 売上用？仕入用？
		if (taika == CD_SYUTOK) {
			uri_sire = _MODE_SIRE;
		}
		else if (is_sisan_syutok(db_syattr)) {
			uri_sire = _MODE_SIRE;
		}
		else if (is_sisan_jyoto(db_syattr)) {
			uri_sire = _MODE_URI;
		}
		else if (is_sisan_syutok(cr_syattr)) {
			uri_sire = _MODE_SIRE;
		}
		else if (is_sisan_jyoto(cr_syattr)) {
			uri_sire = _MODE_URI;
		}
		else if (db_syattr->_A & 0x0f) {
			uri_sire = (db_syattr->_A & 0x03 ? _MODE_URI : _MODE_SIRE);
		}
		else {
			uri_sire = (cr_syattr->_A & 0x03 ? _MODE_URI : _MODE_SIRE);
		}

		// 計算
		strcpy_s(sub->_SY_ZEIGAK, zeiclq(val, zritu, zkbn, uri_sire, mentype));

		// 対価税額のクリア
		memset(sub->_SY_TAIKAZEI, NULL, sizeof(sub->_SY_TAIKAZEI));
#ifdef CLOSE //完成工事も税額有
		// 完成工事振替（税額のクリア）
		if (IsKensetsuSpMaster()) {
			if ( swkkbn == CD_KANSEI_FURI ) {
				// 税額のクリア
				memset(sub->_SY_ZEIGAK, NULL, sizeof(sub->_SY_ZEIGAK));
			}
		}
#endif
		// 特定課税仕入（本体）・ 特定課税仕入返還（本体）
		if (swkkbn == CD_TKKZ_SIRE_SUB || swkkbn == CD_TKKZ_SIREHEN_SUB) {
			// 税額のクリア
			memset(sub->_SY_ZEIGAK, NULL, sizeof(sub->_SY_ZEIGAK));
		}
		m_strZei = sub->_SY_ZEIGAK;
	}
	else {
		// 税額のクリア
		memset(sub->_SY_ZEIGAK, NULL, sizeof(sub->_SY_ZEIGAK));
		// 対価税額のクリア
		memset(sub->_SY_TAIKAZEI, NULL, sizeof(sub->_SY_TAIKAZEI));
	}

	return 0;
}

#include "l_dotnet.h"
//-----------------------------------------------------------------------------
// 消費税 算出（免税事業者からの課税仕入）
//-----------------------------------------------------------------------------
// 引数	sou			：	元金額
//		zritu		：	税率
//						 CD_RITU3(3%), CD_RITU5(5%), CD_RITU0(0%), CD_RITU100(100%), CD_RITU8(8%)
//		zkbn		：	税区分
//						 CD_SOTOZ(n/100), CD_UTIZ(n/(100+n)), CD_BEKKI(0/100)
//		uri_sire	：	売上用？仕入用？
//						 0: 売上(仮受消費税)用, 1: 仕入(仮払消費税)用
//		mzsrkbn		：	免税事業者からの課税仕入区分（内税のみ自動計算）
//						 CD_SP_EXEMPT(80%), CD_SP_EXEMPT_50(50%), CD_SP_EXEMPT_00(0%)
//-----------------------------------------------------------------------------
// 返送値			：	税額
//-----------------------------------------------------------------------------
LPCTSTR CDataZeiCalqSub::zeiclq_mz(LPCTSTR sou, int zritu, int zkbn, int uri_sire, int mzsrkbn)
{
	// 税率の分子
	int	iRitu = 0;
	CString	strBs, strBb;
	if( zritu == CD_RITU5 ) {
		iRitu = 5;
	}
	else if( zritu == CD_RITU3 ) {
		iRitu = 3;
	}
	else if( (zritu == CD_RD_RITU8) || (zritu == CD_RITU8) ) {
		iRitu = 8;
	}
	else if( zritu == CD_RITU10 ) {
		iRitu = 10;
	}
	else {
		iRitu = 0;
	}
	strBs.Format(_T("%d"), iRitu);
	strBb.Format(_T("%d"), iRitu + 100);


	CArith	lArith(0x16);
	char	lsrc[6] = { 0 };
	char	lbs1[6] = { 0 }, lbb1[6] = { 0 };
	lArith.l_input(lsrc, (void*)sou);
	lArith.l_input(lbs1, strBs.GetBuffer());
	lArith.l_input(lbb1, strBb.GetBuffer());

	// 控除率
	strBs = _T("0");
	strBb = _T("100");
	if( mzsrkbn == CD_SP_EXEMPT ) {
		iRitu = 80;
	}
	else if( mzsrkbn == CD_SP_EXEMPT_50 ) {
		iRitu = 50;
	}
	else if( mzsrkbn == CD_SP_EXEMPT_00 ) {
		iRitu = 0;
	}
	else {
		iRitu = 100;
	}
	strBs.Format(_T("%d"), iRitu);
	strBb.Format(_T("%d"), 100);

	char	lbs2[6] = { 0 }, lbb2[6] = { 0 };
	lArith.l_input(lbs2, strBs.GetBuffer());
	lArith.l_input(lbb2, strBb.GetBuffer());

	int hasu = remtype(uri_sire);
	//-- 会計の段階では、指定通りに計算する！
	//	if( hasu == 9 ){
	//		// 仮に切上げが指定されている時には、四捨五入に強制変換（少しでも益税になる方向に）
	//		hasu = 5;
	//	}
	//--

	char	ans[6] = { 0 };
	l_pardotnet3(ans, lsrc, lbs1, lbb1, lbs2, lbb2, hasu);

	static char num_str[50];
	memset(num_str, '\0', sizeof(num_str));
	lArith.l_print(num_str, ans, _T("ssssssssssss9"));

	return (LPCTSTR)num_str;
}


//-----------------------------------------------------------------------------
// 消費税 算出
//-----------------------------------------------------------------------------
// 引数	sou			：	元金額
//		zritu		：	税率
//						 CD_RITU3(3%), CD_RITU5(5%), CD_RITU0(0%), CD_RITU100(100%), CD_RITU8(8%)
//		zkbn		：	税区分
//						 CD_SOTOZ(n/100), CD_UTIZ(n/(100+n)), CD_BEKKI(0/100)
//		uri_sire	：	売上用？仕入用？
//						 0: 売上(仮受消費税)用, 1: 仕入(仮払消費税)用
//		men_type	：	免税事業者の課税仕入れ 
//-----------------------------------------------------------------------------
// 返送値			：	税額
//-----------------------------------------------------------------------------
LPCTSTR CDataZeiCalqSub::zeiclq(LPCTSTR sou, int zritu, int zkbn, int uri_sire, int men_type)
{
	__int64 hasu;	// 0: 切り捨て, 5: 四捨五入, 9: 切り上げ
	__int64 a, b, c;
	__int64 c1000, c10;
	static char zei_str[50];

	//== 元金額チェック＆セット ==
	if( !strlen(sou) ) {
		strcpy_s(zei_str, "");	// NULL 税額
		return zei_str;
	}
	else {
		a = atoi64(sou);			// a = sou
	}

	// 免税事業者
	if( /*m_IsMzCalq &&*/ (zkbn == CD_UTIZ) &&
		((men_type == CD_SP_EXEMPT) || (men_type == CD_SP_EXEMPT_50) || (men_type == CD_SP_EXEMPT_00)) ) {
		return zeiclq_mz(sou, zritu, zkbn, uri_sire, men_type);
	}
	else if( /*m_IsMzCalq &&*/ (zkbn == CD_SOTOZ) &&
		((men_type == CD_SP_EXEMPT) || (men_type == CD_SP_EXEMPT_50) || (men_type == CD_SP_EXEMPT_00)) ) {
	//	return motozei;
		strcpy_s(zei_str, "");	// NULL 税額
		return zei_str;
	}

	//== 定数セット ==
	hasu = (__int64)remtype(uri_sire);
	if( a < 0 ) {
		hasu = -hasu;
	}
//	MyTrace("zeiclq: men_type(%08x), hasu = %s\n", men_type, i64toa(hasu));

	/*- '13.06.14 -*/
	//	c1000 = atoi64( "1000" );
	/*-------------*/
	c1000 = 1000;
	/*-------------*/

		//== 計算 ==
		// 分子
	switch( zritu ) {
	case CD_RITU5:	// ５％
/*- '13.06.14 -*/
//			b = pSzVol->SVzper;		// 基本税率
/*-------------*/
		b = 50;
		/*-------------*/
		break;
	case CD_RITU3:	// ３％
/*- '13.06.14 -*/
//			b = pSzVol->SVzper1;	// 経過税率
/*-------------*/
		b = 30;
		/*-------------*/
		break;
		//--> '13.06.14 INS START
	case CD_RITU8:	// ８％
		b = 80;
		break;
		//<-- '13.06.14 INS END
		//--> '19.01.23 INS START
	case CD_RITU10:		// １０％
		b = 100;
		break;
	case CD_RD_RITU8:	// ８％（軽減税率）
		b = 80;
		break;
		//<-- '19.01.23 INS END
	case CD_RITU100:
		b = 1000;
		break;

	default:		// CD_RITU0
		b = 0;
		break;
	}

	// 分母
	switch( zkbn ) {
	case CD_UTIZ:
		c = c1000 + b;	// 100 + n
		break;

	case CD_SOTOZ:
		c = c1000;		// 100
		break;

	default:			// CD_BEKKI
		c = 0;			// NULL
		break;
	}

	//== NULL分母チェック ==
	if( !c ) {
		strcpy_s(zei_str, "");	// NULL 税額
		return zei_str;
	}

	/*- '13.06.14 -*/
	//	c10 = atoi64( "10" );
	/*-------------*/
	c10 = 10;
	/*-------------*/

		// 税額  a = sou * b / c ... b (=rem)
	if( men_type == 0 || men_type == CD_SP_NO_EXEMPT ) {
		a = a * b * c10;
		a = a / c;
		b = a % c10;
		a = a / c10;

//		MyTrace("zeiclq:amari(1) b = %s, hasu = %s\n", i64toa(b), i64toa(hasu));

		// 端数
		b = b + hasu;
		b = b / c10;
		a = a + b;
	}
	else {
		double clq1;

		clq1 = (double)a * (double)b * (double)c10;
		clq1 = clq1 / (double)c;

		b = 0;
		if( men_type == CD_SP_EXEMPT ) {
			b = 800;
		}
		else if( men_type == CD_SP_EXEMPT_50 ) {
			b = 500;
		}
//		MyTrace("zeiclq:men_type = %08x, a = %s, b = %s, hasu = %s\n", men_type, i64toa(a), i64toa(b), i64toa(hasu));

		if( b != 0 ) {
			clq1 = clq1 * (double)b;
			clq1 = clq1 / (double)1000;
			b = (__int64)clq1 % c10;
			a = (__int64)clq1 / c10;
			// 端数
			b = b + hasu;
			b = b / c10;
			a = a + b;
//			MyTrace("zeiclq:amari(2) b = %s, hasu = %s\n", i64toa(b), i64toa(hasu));
		}
	}
//	MyTrace("zeiclq END: a = %d\n", a);

	return i64toa(a);		// 税額
}


//-----------------------------------------------------------------------------
// 端数計算方式ゲット
//-----------------------------------------------------------------------------
// 引数	uri_sire	：	売上用？仕入用？
//						 0: 売上(仮受消費税)用, 1: 仕入(仮払消費税)用
//-----------------------------------------------------------------------------
// 返送値			：	端数計算式
//						 0: 切り捨て, 9: 切り上げ, 5: 四捨五入
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::remtype(int uri_sire)
{
	int rem = 0;

	/*- '13.06.14 -*/
	//	int typ = ( uri_sire == _MODE_URI ?	(m_pZm->zvol->s_sgn4&0x0c)>>2 : m_pZm->zvol->s_sgn4 ) & 0x03;
	/*-------------*/
	int typ;
	if (uri_sire == _MODE_URI) {	// 売上(仮受消費税)用
		typ = ((m_pZm->zvol->s_sgn4 & 0x0c) >> 2) & 0x03;
	}
	else {							// 仕入(仮払消費税)用
		typ = (m_pZm->zvol->s_sgn4) & 0x03;
	}
	/*-------------*/
	switch (typ) {
	case 1:		// 切り上げ
		rem = 9;
		break;
	case 2:		// 四捨五入
		rem = 5;
		break;
	default:	// 切り捨て
		rem = 0;
		break;
	}

	return(rem);
}


//-----------------------------------------------------------------------------
// ASCII to __int64
//-----------------------------------------------------------------------------
// 引数	num_str	：	文字列
//-----------------------------------------------------------------------------
// 返送値		：	変換後の数値
//-----------------------------------------------------------------------------
__int64 CDataZeiCalqSub::atoi64(LPCTSTR num_str)
{
	__int64 i64;
	CString val = num_str;

	val.TrimLeft(); val.TrimRight();
	sscanf_s(num_str, "%I64d", &i64);

	return i64;
}

//-----------------------------------------------------------------------------
// __int64 to ASCII
//-----------------------------------------------------------------------------
// 引数	i64	：	数値
//-----------------------------------------------------------------------------
// 返送値	：	変換後の文字列
//-----------------------------------------------------------------------------
LPCTSTR CDataZeiCalqSub::i64toa(__int64 i64)
{
	static char num_str[50];

	sprintf_s(num_str, "%I64d", i64);

	return (LPCTSTR)num_str;
}


//-----------------------------------------------------------------------------
// データの消費税サインをコードで取出し
//-----------------------------------------------------------------------------
// 引数	ddata		：	チェック対象データ
//		sel_code	：	チェックコード
//-----------------------------------------------------------------------------
// 返送値			：	消費税サインコード
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::_get_ddata_code(CDBINPDataRec* ddata, DWORD sel_code)
{
	ASSERT(ddata);

	unsigned char dsgn = 0;	// 仕訳サイン
	unsigned char syz1 = 0;	// 税率、税区分
	unsigned char syz2 = 0;	// 課税区分
	unsigned char syz3 = 0;	// 簡易課税の売上業種区分、原則課税で個別対応時の仕入区分
	unsigned char syz4 = 0;	// 消費税仕訳区分
	unsigned char syz5 = 0;	// 特定収入区分
	unsigned char syz6 = 0;	// 免税事業者課税仕入れ 控除割合

	for (int i = 0; i < ddata->m_dsign.GetSize(); ++i) {
		switch (i) {
		case 0:	// 仕訳サイン
			dsgn = ddata->m_dsign.GetAt(0);
			continue;
		case 1:	// 税率、税区分
			syz1 = ddata->m_dsign.GetAt(1);
			continue;
		case 2:	// 課税区分
			syz2 = ddata->m_dsign.GetAt(2);
			continue;
		case 3:	// 簡易課税の売上業種区分、原則課税で個別対応時の仕入区分
			syz3 = ddata->m_dsign.GetAt(3);
			continue;
		case 4:	// 消費税仕訳区分
			syz4 = ddata->m_dsign.GetAt(4);
			continue;
		case 5:	// 特定収入
			syz5 = ddata->m_dsign.GetAt(5);
			continue;
		case 6:	// 免税事業者課税仕入れ
			syz6 = ddata->m_dsign.GetAt(6);
			continue;
		default:// End
			i = (int)ddata->m_dsign.GetSize();
			continue;
		}
	}
	// 仮受／仮払消費税科目と課税区分の判定
	struct _DBSY_ATTR	syattr;

	memset(&syattr, '\0', sizeof(syattr));
	_get_syattr(&syattr, ddata->m_dbt);
	int db_typ = _kamok_typ(ddata->m_dbt, &syattr, _CTL_DEBT);

	memset(&syattr, '\0', sizeof(syattr));
	_get_syattr(&syattr, ddata->m_cre);
	int cr_typ = _kamok_typ(ddata->m_cre, &syattr, _CTL_CRED);

	// 仮受／仮払？
	int karikmk = 0;
	if ((db_typ == _KMK_TYP_KARIUKE) || (db_typ == _KMK_TYP_KARIBARAI) || (cr_typ == _KMK_TYP_KARIUKE) || (cr_typ == _KMK_TYP_KARIBARAI)) {
		karikmk = 1;
	}

	int sydata = (dsgn & 0x40);
	int kzei = (syz2 & 0x01);	// 課税ビット

	// 税率コード
	if (sel_code & MSK_CD_RITU) {
		/*- '13.10.11 -*/
		//		if( !(sydata || karikmk) ){
		//			return 0;
		//		}
		/*-------------*/
		if ((sydata == 0) && (karikmk == 0)) {
			return 0;
		}
		/*-------------*/

		switch (syz1 & 0x0f) {
		case 0x00:
			return CD_RITU5;
		case 0x01:
			return CD_RITU3;
		case 0x02:
			return CD_RITU0;
			//--> '13.06.14 INS START
		case 0x03:
			return CD_RITU8;
			//<-- '13.06.14 INS END
			//--> '19.01.23 INS START
		case 0x04:
			return CD_RD_RITU8;
		case 0x05:
			return CD_RITU10;
			//<-- '19.01.23 INS END

		default:
			return 0;
		}
	}
	// 税区分コード
	else if (sel_code & MSK_CD_ZEI) {
		if (!(sydata && kzei)) {
			return 0;
		}

		switch (syz1 & 0xf0) {
		case 0x00:
			return CD_SOTOZ;
		case 0x10:
			return CD_UTIZ;
		case 0x20:
			return CD_BEKKI;
		default:
			return 0;
		}
	}
	// 課税区分コード
	else if (sel_code & MSK_CD_KAZEI) {
		if (!sydata) {
			return 0;
		}

		switch (syz2 & 0x0f) {
		case 0x00:
			return CD_HIKAZ;
		case 0x01:
			return CD_KAZEI;
		case 0x02:
			return CD_FUKAZ;
		default:
			return 0;
		}
	}
	// 固定資産の 取得・譲渡 区分
	else if (sel_code & MSK_CD_TAIKAKBN) {
		if (!(sydata && ((syz2 & 0x0f) != 0x02))) {
			return 0;
		}

		// 消費税データで不課税以外
		switch (syz2 & 0x30) {
		case 0x10:
			return CD_SYUTOK;
		case 0x30:
			return CD_TAIKA;
		default:
			return 0;
		}
	}
	// 特定収入
	else if (isTOKTEI_CODE(sel_code) && (sel_code & MSK_CD_TOKTEI)) {
		// 特定収入可能マスタで消費税データで不課税
		if (!(istokteim() && sydata && ((syz2 & 0x0f) == 0x02))) {
			return 0;
		}

		//-- '14.11.11 --
		//		switch( syz5 & 0x03 ) {
		//---------------
		switch (syz5 & 0x0f) {
			//---------------
		case 0x00:	// (なし)
			return MSK_CD_TOKTEI;
		case 0x01:	// 特定収入
			return CD_TK_SYNYU;
		case 0x02:	// 不特定収入
			return CD_TK_FUTOK;
		case 0x03:	// 外特定収入
			return CD_TK_GAITK;
			//--> '14.11.11 INS START
		case 0x04:	// 特定収入（5%仕入）
			return CD_TK_SYNYU_5PER;
		case 0x05:	// 不特定収入（5%仕入）
			return CD_FK_SYNYU_5PER;
		case 0x06:	// 特定収入（8%仕入）
			return CD_TK_SYNYU_8PER;
		case 0x07:	// 不特定収入（8%仕入）
			return CD_FK_SYNYU_8PER;
		case 0x08:	// 特定収入（10%仕入）
			return CD_TK_SYNYU_10PER;
		case 0x09:	// 不特定収入（10%仕入）
			return CD_FK_SYNYU_10PER;
		case 0x0A:	// 特定収入（8%軽減仕入）
			return CD_TK_SYNYU_RD_8PER;
		case 0x0B:	// 不特定収入（8%軽減仕入）
			return CD_FK_SYNYU_RD_8PER;
			//<-- '14.11.11 INS END
		default:
			return 0;
		}
	}
	// 簡易課税の売上業種区分コード
	else if (!isTOKTEI_CODE(sel_code) && (sel_code & MSK_CD_URI) && !isSPSIGN_CODE(sel_code) ) {
	SRCTrace;
		if (!(iskanim() && sydata && kzei)) {
			return 0;
		}

		switch (syz3 & 0x0f) {
		case 0x01:
			return CD_1OROSI;
		case 0x02:
			return CD_2KOURI;
		case 0x03:
			return CD_3SEIZO;
		case 0x04:
			return CD_4SONOTA;
		case 0x05:
			return CD_5SRVIC;
			//--> '14.11.11 INS START
		case 0x06:
			return CD_6FUDOSAN;
			//<-- '14.11.11 INS END
		default:
			return 0;
		}
	}
	// 原則課税で個別対応時の仕入区分コード
	else if (sel_code & MSK_CD_SIRE) {
		BOOL karibarai = FALSE;
		if ((db_typ == _KMK_TYP_KARIBARAI) || (cr_typ == _KMK_TYP_KARIBARAI)) {
			karibarai = TRUE;
		}

#ifdef CLOSE // バージョンアップはされているので
		//--> '13.06.14 INS START
		if (IsAfterVup(ID_ICSSYZ_H26_REV) == FALSE) {
			karibarai = FALSE;
		}
		//<-- '13.06.14 INS END
#endif

				// 個別対応マスタで消費税データ
		if (!(iskobetm() && (sydata || karibarai))) {
			return 0;
		}
		// 通常の仕入区分
		else if (kzei || karibarai) {
			switch (syz3 & 0xf0) {
			case 0x10:
				return CD_KAURI;
			case 0x20:
				return CD_HIURI;
			case 0x30:
				return CD_KYOURI;
			default:
				return 0;
			}
		}
		// 特定収入マスタで枠外で特定収入区分の場合の仕入区分
//-- '14.11.11 --
//		else if( istokteim() && ((syz2&0x0f)==0x02) && ((syz5&0x0f)==0x01) ){
//---------------
		else if (istokteim() && ((syz2 & 0x0f) == 0x02) && (((syz5 & 0x0f) == 0x01) || ((syz5 & 0x0f) == 0x04) || ((syz5 & 0x0f) == 0x06) || ((syz5 & 0x0f) == 0x08) || ((syz5 & 0x0f) == 0x0a))) {
			//---------------
			switch (syz5 & 0xf0) {
			case 0x10:
				return CD_TK_KAURI;
			case 0x20:
				return CD_TK_HIURI;
			case 0x30:
				return CD_TK_KYOURI;
			default:
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	// 仕訳区分コード
	else if (sel_code & MSK_CD_SWKBN) {
		switch (syz4 & 0xff) {

		case 1:
			return CD_URIAGE;
		case 2:
			return CD_KASISON;
		case 3:
			return CD_KASIKAI;
		case 4:
			return CD_YUNYU;
		case 5:
			return CD_HKYUSYUTU;
		case 6:
			return CD_HANSYONYU;
		case 7:
			return CD_HANSYOHI;
		case 8:
			return CD_SIREWARI;
		case 9:
			return CD_URIWARI;
		case 10:
			return CD_TANAKASAN;
		case 11:
			return CD_TANAGENSAN;
		case 12:
			return CD_TOKUTEI;
		case 13:	// 現在設定していないが輸出売上（消費税区分で対応）
			return CD_YUSYUTU;
		case 14:	// 資産取得（未対応）
			return CD_SISNSYTK;

		case 15:	// 資産譲渡（未対応）
			return CD_SISNJYOT;
		case 16:	// 固定資産の取得／譲渡（未対応）
			return CD_KOTEI;
		case 17:	// 売上返品（未対応）
			return CD_URIHEN;
		case 18:	// 営業外収益（未対応）
			return CD_EIGAIEKI;
		case 19:	// 特別利益（未対応）
			return CD_TOKRI;
		case 20:	// 受取利息
			return CD_UKERI;
		case 21:	// 仕入
			return CD_SIRE;
		case 22:	// 仕入返品（未対応）
			return CD_SIREHEN;
		case 23:	// 経費
			return CD_KEIHI;
		case 24:	// 営業外費用（未対応）
			return CD_EIGAIHI;
		case 25:	// 特別損失（未対応）
			return CD_TOKSON;
		case 26:	// 有価証券譲渡（未対応）
			return CD_YUKAJYOTO;
		case 27:	// 土地譲渡（未対応）
			return CD_TOTIJYOTO;
		case 28:	// 消費税精算
			return CD_ZEISEISAN;

			//--> '15.06.24 INS START
			//-- '15.08.02 --
			//			case 29:	// 特定課税仕入（本体）
			//				return CD_TKKZ_SIRE_SUB;
			//			case 30:	// 特定課税仕入（消費税）
			//				return CD_TKKZ_SIRE_ZEI;	
			//			case 31:	// 特定課税仕入返還（本体）
			//				return CD_TKKZ_SIREHEN_SUB;
			//			case 32:	// 特定課税仕入返還（消費税）
			//				return CD_TKKZ_SIREHEN_ZEI;	
			//---------------
		case 29:	// 特定課税仕入（本体）
			return CD_TKKZ_SIRE_SUB;
		case 30:	// 特定課税仕入返還（本体）
			return CD_TKKZ_SIREHEN_SUB;
			//---------------
			//<-- '15.06.24 INS END

		case 31:	// 完成時振替
			return CD_KANSEI_FURI;
		case 32:	// 輸入消費税（国税）
			return CD_YUNYU_KUNI;
		case 33:	// 輸入消費税（地方税）
			return CD_YUNYU_TIHOU;
		case 34:	// 輸入消費税（国地合算）
			return CD_YUNYU_GASSAN;

		default:
			if (((syz1 & 0x0f) == 0x02) && ((syz2 & 0x0f) == 0x01)) {
				return CD_YUSYUTU;
			}
			else if (sydata) {
				/*- '13.10.22 -*/
				//					DWORD chk = 0;
				//					// 貸倒損失？
				//					// 借方
				//					_get_syattr( &syattr, ddata->dbt );
				//					if( (syattr._A==0x08) && (syattr._B==0x03) ){
				//						chk = 1;
				//					}
				//					// 貸方
				//					_get_syattr( &syattr, ddata->cre );
				//					if( (syattr._A==0x08) && (syattr._B==0x03) ){
				//						chk = 1;
				//					}
				//					if( chk ){
				//						return(kzei ? CD_KASISON : 0);
				//					}
				//					
				//					// 貸倒回収？
				//					// 借方
				//					_get_syattr( &syattr, ddata->dbt );
				//					if( (syattr._A==0x02) && (syattr._B==0x05) ){
				//						chk = 1;
				//					}
				//					// 貸方
				//					_get_syattr( &syattr, ddata->cre );
				//					if( (syattr._A==0x02) && (syattr._B==0x05) ){
				//						chk = 1;
				//					}
				//					if( chk ){
				//						return(kzei ? CD_KASIKAI : 0);
				//					}
				/*-------------*/
									// 借方
				_get_syattr(&syattr, ddata->m_dbt);
				DWORD db_swkbn = _to_siwake_kbn_code(&syattr);

				// 貸方
				_get_syattr(&syattr, ddata->m_cre);
				DWORD cr_swkbn = _to_siwake_kbn_code(&syattr);

				// 貸倒損失？
				if ((db_swkbn == CD_KASISON) || (cr_swkbn == CD_KASISON)) {
					return(kzei ? CD_KASISON : 0);
				}
				// 貸倒回収？
				if ((db_swkbn == CD_KASIKAI) || (cr_swkbn == CD_KASIKAI)) {
					return(kzei ? CD_KASIKAI : 0);
				}
				/*-------------*/
			}
			return 0;
		}
	}
	else if( sel_code & MSK_CD_SPSIGN ) {
		if( !(sydata && kzei) ) {
			return 0;
		}
		if( !(syz6 & 0x20)) {
			return CD_SP_NO_EXEMPT;
		}
		else {
			if( ddata->m_ddate < SYZMEN1_YMD ) {

			}
			else {
				int tmp = (syz6 & 0xc0);
				tmp >>= 6;

				if( tmp == 0 ) {
					//80%
					return CD_SP_EXEMPT;
				}

				if( tmp == 1 ) {
					//50%
					return CD_SP_EXEMPT_50;
				}
				else if( tmp == 2 ) {
					//0%
				}
			}
		}
		return 0;
	}

	return 0;
}



//-----------------------------------------------------------------------------
// 科目名称→メモリー
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::KnOnMemry(void)
{
	int n;
	int sg;

	if (m_vol && m_pZm->knrec) {
		// save
		if (!InKNRECary.GetSize()) {

			m_KnrecMax = m_pZm->knrec->GetRecordCount();
			InKNRECary.SetSize(m_KnrecMax);

			for (n = 0, sg = m_pZm->knrec->MoveFirst(); (sg != -1) && (n < m_KnrecMax); ++n, sg = m_pZm->knrec->MoveNext()) {

				if (m_pZm->knrec->knvoid & 0x01) {
					--n;	// 04.30 /07 追加
					continue;
				}
				struct InKNREC* p = new struct InKNREC;

				p->kncod = m_pZm->knrec->kncod;
				for (int i = 0; i < m_pZm->knrec->knsgn.GetSize(); ++i) {
					p->knsgn.Add(m_pZm->knrec->knsgn.GetAt(i));
				}
				p->knatt = m_pZm->knrec->knatt;	// 貸借区分
				p->kninp = m_pZm->knrec->kninp;	// 入力用内／外区分 １＝内 ２＝外 ３＝別(=01.21 /08) ０＝設定なし

				InKNRECary.SetAt(n, (void*)p);
			}
			m_KnrecMax = n;	// 登録数の再セット 

			// テーブルマッピング
			m_KnMap.InitHashTable((m_KnrecMax * 120) / 100);
			for (n = 0; n < m_KnrecMax; ++n) {
				struct InKNREC* p = (struct InKNREC*)InKNRECary[n];

				m_KnMap.SetAt(_atoh((LPSTR)(LPCTSTR)(p->kncod)), p);
			}
		}
		return 0;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// メモリー科目名称→破棄
//-----------------------------------------------------------------------------
void CDataZeiCalqSub::KnOffMemry(void)
{
	struct InKNREC *p;

	for (int n = 0; n < InKNRECary.GetCount(); ++n) {
		if (p = (struct InKNREC*)InKNRECary[n]) {
			delete p;
		}
	}
	InKNRECary.RemoveAll();

	m_KnMap.RemoveAll();
	m_KnrecMax = 0;
}

//-----------------------------------------------------------------------------
// 科目名称レコードＧｅｔ
//-----------------------------------------------------------------------------
// 引数	kamok
//-----------------------------------------------------------------------------
// 返送値	NULL以外	：	該当科目名称レコード
//			NULL		：	該当無し
//-----------------------------------------------------------------------------
struct InKNREC* CDataZeiCalqSub::findknrec(LPCTSTR kamok)
{
	CString filter = kamok;

	if (m_vol && m_pZm->knrec) {
		// found loop
		struct InKNREC* pknrec;

		if (m_KnMap.Lookup(_atoh((LPSTR)(LPCTSTR)filter.Left(8)), pknrec)) {
			return pknrec;
		}
	}

	return 0;
}



//-----------------------------------------------------------------------------
// 科目から消費税属性Ｇｅｔ
//-----------------------------------------------------------------------------
// 引数	syattr			：	消費税属性の取得先
//		kamok			：	取得対象科目
//		after_fnc_code	：	アフターファンクション実行コードのセット先
//-----------------------------------------------------------------------------
// 返送値	0			：	OK
//			-1			：	NO
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_get_syattr(struct _DBSY_ATTR* syattr, LPCTSTR kamok, DWORD* after_fnc_code/*=NULL*/)
{
	struct InKNREC *pknrec;
#ifdef TEST
	CString str = kamok;

	syattr->_A = 0;
	syattr->_B = 0;
	syattr->_C = 0;
	syattr->_D = 0;
	syattr->_E = 0;
	syattr->_F = 0;
# ifdef TEISEI_20070111	// 01.11 /07 貸借属性と棚卸属性の追加
	syattr->knatt = 0;	// 貸借区分
	syattr->stock = 1;	// 棚卸サイン
# endif
#ifdef NEXT7	// 01.10 /07
	syattr->kninp = 0;	// 入力用 内／外区分
#endif

# ifdef _CODE8_	// 10.19 /05
//	if( str == "04051601" )	// 仮受 消費税
	if (str == "01010000")
# else
	if (str == "0101")
# endif
	{
		//		syattr->_A = 0x02;	// 仕入
		//		syattr->_B = 0x02;	// (仕入割引)
		//		syattr->_C = 0x02;	// 課税
		//		syattr->_C = 0x01;	// 枠外
		//		syattr->_C = 0x03;	// 非課税
		//		syattr->_D = 0x00;	// 5% | NULL
		//		syattr->_E = 0x00;
		//		syattr->_F = 0x00;

		//		syattr->_A = 0x02;	// 売上
		//		syattr->_B = 0x00;	// (売上)
		////		syattr->_C = 0x02;	// 課税
		////		syattr->_C = 0x01;	// 枠外
		//		syattr->_C = 0x03;	// 非課税
		////		syattr->_D = 0x00|0x03;	// 5% | 製造業
		//		syattr->_E = 0x00;
		//		syattr->_F = 0x00;


		//		syattr->_A = 0x11;	// 
		//		syattr->_B = 0x02;	// 固定資産
		//		syattr->_B = 0x04;	// 土地
		//		syattr->_B = 0x03;	// 有価証券
		//		syattr->_C = 0x02;	// 課税
		//		syattr->_C = 0x03;	// 非課税
		//		syattr->_D = 0x20;	// ０％
		//		syattr->_D = 0x00;	// ５％
		//		syattr->_E = 0x00;

		//		syattr->_A = 0x04;
		//		syattr->_B = 0x00;

		//		syattr->_A = 0x04;
		//		syattr->_B = 0x06;	// 
		//		syattr->_C = 0x00;	//
		//		syattr->_D = 0x00;	//
		//		syattr->_E = 0x00;
	}
	else if (str == "01020000")
	{

		syattr->_A = 0;
		syattr->_B = 0;
		syattr->_C = 0;
		syattr->_D = 0;
		syattr->_E = 0;
		syattr->_F = 0;

# ifdef TEISEI_20070111	// 01.11 /07 貸借属性と棚卸属性の追加
		syattr->knatt = 0;	// 貸借区分
		syattr->stock = 0;	// 棚卸サイン
# endif
#ifdef NEXT7	// 01.10 /07
		syattr->kninp = 0;	// 入力用 内／外区分
#endif

//		syattr->_A = 0x04;	// 仕入
//		syattr->_B = 0x00; 
////		syattr->_C = 0x02;	// 課税
//		syattr->_C = 0x01;	// 枠外
////		syattr->_C = 0x03;	// 非課税
//		syattr->_D = 0x00;	// ５％
////		syattr->_E = 1;		// 課税売上にかかる
//		syattr->_F = 0x00;


//		syattr->_A = 0x11;	 
//		syattr->_B = 0x02;	// 固定資産
//		syattr->_B = 0x04;	// 土地
//		syattr->_B = 0x03;	// 有価証券
//		syattr->_C = 0x02;	// 課税
//		syattr->_C = 0x03;	// 非課税
//		syattr->_D = 0x20;	// ０％
//		syattr->_D = 0x00;	// ５％
//		syattr->_E = 0x00;


	}

	return 0;	// OK
#else
	if (pknrec = findknrec(kamok)) {
		_to_DBSY_ATTR(syattr, pknrec);

		//--> '15.10.15 INS START
		int knSt = 0;
		//<-- '15.10.15 INS END

		if ((syattr->_A == 0x04) && (syattr->_B == 0x04)) {	// 輸入仕入
			if (isSYAFUKU_SYUKYO(m_vol->apno)) {		// 社会福祉・宗教
				;
			}
			else {
				// 属性振替
				CString strcd;
				strcd.Format(_T("%.2s"), kamok);
				if (strcd == _T("02")) {		// 固定資産
					syattr->_A = 0x11;
					syattr->_B = 0x02;			// 固定資産
				}
				//				else {
				//					syattr->_A = 0x04; syattr->_B = 0x00;			// 仕入
				//				}
			}
			// アフターファンクション実行コードセット
			if (after_fnc_code && !(*after_fnc_code)) {
				*after_fnc_code = CD_YUNYU;		// 輸入仕入
			}
		}
		//--> '15.10.15 INS START
		else if (knSt = IsTkkzSiire(syattr)) {
			// アフターファンクション実行コードセット
			if (after_fnc_code && !(*after_fnc_code)) {
				if (knSt == 1) {
					*after_fnc_code = CD_TKKZ_SIRE_SUB;
				}
				else if (knSt == 2) {
					*after_fnc_code = CD_TKKZ_SIREHEN_SUB;
				}
			}
		}
		//<-- '15.10.15 INS END

				// アフターファンクションは必要になるかも？！

		return 0;	// OK
	}
	return -1;	// Not
#endif
}

//-----------------------------------------------------------------------------
// 科目名称消費税属性取出し
//-----------------------------------------------------------------------------
// 引数	syattr	：	消費税属性の取り出し先
//		pknrec	：	取り出し元の科目名称
//-----------------------------------------------------------------------------
// 返送値	0	：	OK
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_to_DBSY_ATTR(struct _DBSY_ATTR* syattr, CKNREC* pknrec)
{
	BYTE	*p;

	/* Found */
	int n = (int)pknrec->knsgn.GetSize();	// 配列サイズ
	p = &syattr->_A;						// destination buffer
	for (int i = 0; i < sizeof(syattr->_ATTR); ++i) {
		if (i < n) {
			*(p + i) = pknrec->knsgn.GetAt(i);
		}
		else {
			*(p + i) = 0;	// 0 clear.
		}
	}

	syattr->knatt = pknrec->knatt;	// 貸借区分

	// 対象外の場合、属性をクリア
	if (!(syattr->_C & 0x0f)) {
		memset(&syattr->_A, '\0', sizeof(syattr->_ATTR));
	}

	syattr->kninp = pknrec->kninp;	// 入力用 内／外／別(=01.21 /08) 区分

	return 0;	// OK
}

//-----------------------------------------------------------------------------
// 科目名称レコードから消費税属性に展開
//-----------------------------------------------------------------------------
// 引数	syattr	：	展開先 消費税属性バッファ
//		pknrec	：	展開元 科目名称レコード
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_to_DBSY_ATTR(struct _DBSY_ATTR* syattr, struct InKNREC* pknrec)
{
	BYTE	*p;
	int		n;

	memset(syattr, '\0', sizeof(struct _DBSY_ATTR));

	n = (int)pknrec->knsgn.GetSize();	// 配列サイズ
	p = &syattr->_A;					// destination buffer
	for (int i = 0; i < sizeof(syattr->_ATTR); ++i) {
		if (i < n) {
			*(p + i) = pknrec->knsgn.GetAt(i);
		}
		else {
			*(p + i) = 0;	// 0 clear.
		}
	}

	// 01.17 /07 貸借属性と棚卸属性の追加
	syattr->knatt = pknrec->knatt;	// 貸借属性
	if ((syattr->_A == 0x04) && (syattr->_B == 0x05)) {
		syattr->tana = _KMK_TYP_KISYU_TANA;		// 棚卸調整加算額 候補
	}
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x06)) {
		syattr->tana = _KMK_TYP_KIMATU_TANA;	// 棚卸調整減算額 候補
	}
	else {
		syattr->tana = 0;
	}

	// 03.17 /07
	// 対象外の場合、属性をクリア
	if (!(syattr->_C & 0x0f)) {
		memset(&syattr->_A, '\0', sizeof(syattr->_ATTR));
	}

	// 01.17 /07
	syattr->kninp = pknrec->kninp;	// 入力用内／外区分 １＝内 ２＝外 ３＝別(=01.21 /08) ０＝設定なし

	return 0;	// OK

}


//-----------------------------------------------------------------------------
// 建設（特例法）マスター？
//-----------------------------------------------------------------------------
// 返送値	1	：	建設（特例法）マスター
//			0	：	建設（特例法）マスターでない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::IsKensetsuSpMaster()
{
	if (isReady()) {
		if (m_stKensetsuSpMaster == -1) {
			m_stKensetsuSpMaster = 0;
			if (IsKensetsuMaster()) {
				if (IsMiseiKojiType()) {
					m_stKensetsuSpMaster = 1;
				}
			}
		}

		if (m_stKensetsuSpMaster == 1) {
			return 1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 未成工事支出金タイプ？
//-----------------------------------------------------------------------------
// 返送値	1	：	未成工事支出金タイプ
//			0	：	未成工事支出金タイプでない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::IsMiseiKojiType()
{
	int	mtype = 0;

	if (isReady()) {

		CRecordset	rcset(m_pZm->m_database);

		// テーブルの有無を確認
		CString	sql;

		try {
			sql = _T("SELECT name FROM sys.objects WHERE object_id = object_id('dbo.KJVOL')");
			rcset.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
			if (!rcset.IsEOF()) {
				rcset.Close();

				sql = _T("SELECT VOL_MTYP FROM KJVOL");
				rcset.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
				if (!rcset.IsEOF()) {
					CString	tmpStr;
					rcset.GetFieldValue((SHORT)0, tmpStr);
					mtype = atoi(tmpStr);
				}
				rcset.Close();
			}
		}
		catch (CDBException *pde) {
			pde->Delete();
			return 0;
		}
	}

	return mtype;
}


//-----------------------------------------------------------------------------
// 建設マスター？
//-----------------------------------------------------------------------------
// 返送値	1	：	建設マスター
//			0	：	建設マスターでない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::IsKensetsuMaster()
{
	if (isReady()) {
		if ((m_vol->apno & 0xff) == 0x20) {
			return 1;
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 特定収入該当マスター？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	特定収入該当マスター
//			FALSE	：	特定収入該当マスターでない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::istokteim()
{
	int retn;

	if (((m_vol->apno & 0xf0) == 0x50) && isgensokm()) {
		retn = TRUE;	// 非営利 + 原則課税
	}
	else {
		retn = FALSE;
	}

	return retn;
}


//-----------------------------------------------------------------------------
// 原則課税マスター？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	原則課税マスター
//			FALSE	：	原則課税マスターでない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::isgensokm()
{
	int		retn;
	if (!(m_vol->s_sgn2 & 0x0f)) {
		retn = TRUE;
	}
	else {
		retn = FALSE;
	}
	return(retn);
}

//-----------------------------------------------------------------------------
// 個別対応マスタ－？
//-----------------------------------------------------------------------------
// 返送値	1	：	個別対応マスタ－
//			0	：	個別対応マスタ－でない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::iskobetm()
{
	return m_vol->s_sgn4 & 0x80;
}

//-----------------------------------------------------------------------------
// 簡易マスター？
//-----------------------------------------------------------------------------
// 返送値	1	：	簡易マスター
//			0	：	簡易マスターでない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::iskanim()
{
	return kihon_ukbn_attr() >= 1;					// 簡易課税(卸売業)　以上をチェック
}


//-----------------------------------------------------------------------------
// 基本業種区分の取り出し
//-----------------------------------------------------------------------------
// 返送値	：	基本業種区分
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::kihon_ukbn_attr()
{
	int ret = (m_vol->s_sgn2 & 0x0f) - 1;				// 基本業種区分（１～）の取り出し
//---------------

	return ret;
}



//-----------------------------------------------------------------------------
// 消費税科目タイプチェック＆コードに変換
//-----------------------------------------------------------------------------
// 引数	kamok		：
//		syattr		：
//		debt_cred	：	借方？貸方？
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::_kamok_typ(LPCTSTR kamok, struct _DBSY_ATTR* syattr, int debt_cred)
{
	DWORD type_code = 0;

	// 複数仮受、仮払消費税サポート
	CString str_kamok = kamok;

	if (str_kamok.Left(6) == get_KARIUKE().Left(6)) {			// 小分類比較
		type_code = _KMK_TYP_KARIUKE;							// 仮受消費税等
	}
	else if (str_kamok.Left(6) == get_KARIBARAI().Left(6)) {	// 小分類比較
		type_code = _KMK_TYP_KARIBARAI;							// 仮払消費税等
	}
	else if ((debt_cred == _CTL_DEBT) && isgensokm() && (type_code = chk_DEBTTANA(syattr, kamok))) {
		;														// 期首棚卸
	}
	else if ((debt_cred == _CTL_CRED) && isgensokm() && (type_code = chk_CREDTANA(syattr, kamok))) {
		;														// 期末棚卸
	}
	else if (type_code = chk_KOTEISISAN(syattr)) {				// 固定資産 取得・譲渡
		;
	}
	else if (type_code = chk_MISEIKOUJI(str_kamok, syattr)) {	// 未成工事支出金
		;
	}
	else {
		type_code = 0;											// 通常科目
	}

	return type_code;
}



//-----------------------------------------------------------------------------
// 仮払い消費税科目の取り出し
//-----------------------------------------------------------------------------
// 返送値	：	仮払い消費税科目
//-----------------------------------------------------------------------------
CString CDataZeiCalqSub::get_KARIBARAI()
{
	CString str = pSzVol->SVkcod1;
	return str;
}

//-----------------------------------------------------------------------------
// 仮受け消費税科目の取り出し
//-----------------------------------------------------------------------------
// 返送値	：	仮受け消費税科目
//-----------------------------------------------------------------------------
CString CDataZeiCalqSub::get_KARIUKE()
{
	CString str = pSzVol->SVkcod2;
	return str;
}

//-----------------------------------------------------------------------------
// 借方 棚卸科目チェック
//   →	_KMK_TYP_KISYU_TANA		期首棚卸
//-----------------------------------------------------------------------------
// 引数		db_syattr			：	
//			debt				：	
//-----------------------------------------------------------------------------
// 返送値	0					：	期首棚卸でない
//			_KMK_TYP_KISYU_TANA	：	期首棚卸
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::chk_DEBTTANA(struct _DBSY_ATTR* db_syattr, LPCTSTR debt)
{
	CString strdebt = debt;
	int ap = m_vol->apno;

	int sg = 0;
	ASSERT(db_syattr);
	if (db_syattr->tana == _KMK_TYP_KISYU_TANA) {
		sg = _KMK_TYP_KISYU_TANA;
	}
	return sg;
}

//-----------------------------------------------------------------------------
// 貸方 棚卸科目チェック
//   →	_KMK_TYP_KIMATU_TANA	期末棚卸
//-----------------------------------------------------------------------------
// 引数		cr_syattr				：	
//			cred					：	
//-----------------------------------------------------------------------------
// 返送値	0						：	期末棚卸でない
//			_KMK_TYP_KIMATU_TANA	：	期末棚卸
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::chk_CREDTANA(struct _DBSY_ATTR* cr_syattr, LPCTSTR cred)
{
	CString strcred = cred;
	int ap = m_vol->apno;

	int sg = 0;
	ASSERT(cr_syattr);
	if (cr_syattr->tana == _KMK_TYP_KIMATU_TANA) {
		sg = _KMK_TYP_KIMATU_TANA;
	}
	return sg;
}

//-----------------------------------------------------------------------------
// 固定資産科目チェック
//-----------------------------------------------------------------------------
// 引数		db_syattr			：	
//-----------------------------------------------------------------------------
// 返送値	_KMK_TYP_KOTEISISAN	：	固定資産
//			_KMK_TYP_YUKA		：	有価証券
//			_KMK_TYP_TOCHI		：	土地
//			0					：	上記以外
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::chk_KOTEISISAN(struct _DBSY_ATTR* db_syattr)
{
	int sg = 0;

	ASSERT(db_syattr);
	if (db_syattr->_A == 0x11) {
		switch (db_syattr->_B) {
		case 0x02:	// 固定資産
			sg = _KMK_TYP_KOTEI;
			break;
		case 0x03:	// 有価証券
			sg = _KMK_TYP_YUKA;
			break;
		case 0x04:	// 土地
			sg = _KMK_TYP_TOCHI;
			break;
		default:
			sg = 0;
			break;
		}
	}
	else if (db_syattr->_A == 0x02) {
		switch (db_syattr->_B) {
		case 0x0b:	// 有価証券譲渡
			sg = _KMK_TYP_YUKA;
			break;
		default:
			sg = 0;
			break;
		}
	}

	return sg;
}

//-----------------------------------------------------------------------------
// 未成工事支出金科目チェック
//-----------------------------------------------------------------------------
// 引数	strKmk					：	チェック対象科目
//		syattr					：	チェック対象科目 消費税レコード
//-----------------------------------------------------------------------------
// 返送値	_KMK_TYP_MISEIKOUJI	：	未成工事支出金
//			0					：	上記以外
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::chk_MISEIKOUJI(CString& strKmk, struct _DBSY_ATTR* syattr)
{
	DWORD sg = 0;

	if (IsKensetsuMaster()) {
		if (strKmk.Left(6) == _T("010602")) {
			// 小分類比較
			sg = _KMK_TYP_MISEIKOUJI;
		}
	}

	return sg;
}


//-----------------------------------------------------------------------------
// 科目属性から仕訳区分サインコード
//-----------------------------------------------------------------------------
// 引数	syattr	：	取得対象
//-----------------------------------------------------------------------------
// 返送値		：	仕訳区分サインコード
//-----------------------------------------------------------------------------
DWORD CDataZeiCalqSub::_to_siwake_kbn_code(struct _DBSY_ATTR* syattr)
{
	ASSERT(syattr);

	DWORD code = 0;

	// 貸倒損失
	if ((syattr->_A == 0x08) && (syattr->_B == 0x03)) {
		code = CD_KASISON;
	}
	// 貸倒回収
	else if ((syattr->_A == 0x02) && (syattr->_B == 0x05)) {
		code = CD_KASIKAI;
	}
	// 輸入仕入
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x04)) {
		code = CD_YUNYU;
	}
	// 輸出売上
	else if ((syattr->_A == 0x02) && (syattr->_B == 0x09)) {
		code = CD_YUSYUTU;
	}
	// 非課税輸出売上
	else if ((syattr->_A == 0x02) && (syattr->_B == 0x08)) {
		code = CD_HKYUSYUTU;
	}
	// 販売奨励金収入
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x02)) {
		code = CD_HANSYONYU;
	}
	// 販売奨励金費用
	else if ((syattr->_A == 0x02) && (syattr->_B == 0x06)) {
		code = CD_HANSYOHI;
	}
	// 売上割引
	else if ((syattr->_A == 0x02) && (syattr->_B == 0x07)) {
		code = CD_URIWARI;
	}
	// 仕入割引
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x03)) {
		code = CD_SIREWARI;
	}
	//--> '15.06.24 INS START
		// 特定課税仕入（本体）
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x0B)) {
		code = CD_TKKZ_SIRE_SUB;
	}
	// 特定課税仕入返還（本体）
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x0D)) {
		code = CD_TKKZ_SIREHEN_SUB;
	}
	//---> '15.08.02 CUT START
	//	// 特定課税仕入（消費税）
	//	else if( (syattr->_A==0x04) && (syattr->_B==0x0C) ){
	//		code = CD_TKKZ_SIRE_ZEI;
	//	}
	//	// 特定課税仕入返還（消費税）
	//	else if( (syattr->_A==0x04) && (syattr->_B==0x0E) ){
	//		code = CD_TKKZ_SIREHEN_ZEI;
	//	}
	//<--- '15.08.02 CUT END
	//<-- '15.06.24 INS END
	//--> '19.02.01 INS START
		// 完成時振替
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x0A)) {
		code = CD_KANSEI_FURI;
	}
	//<-- '19.02.01 INS END
	// 輸入消費税（国税）
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x11)) {
		code = CD_YUNYU_KUNI;
	}
	// 輸入消費税（地方税）
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x12)) {
		code = CD_YUNYU_TIHOU;
	}
	// 輸入消費税（国地合算）
	else if ((syattr->_A == 0x04) && (syattr->_B == 0x13)) {
		code = CD_YUNYU_GASSAN;
	}
	else {
		code = 0;
	}

	return code;
}


//-----------------------------------------------------------------------------
// 特定課税仕入？('15.10.15)
//-----------------------------------------------------------------------------
// 引数		pSyattr	：	チェック対象消費税属性
//-----------------------------------------------------------------------------
// 返送値	1		：	特定課税仕入
//			2		：	特定課税仕入返還
//			0		：	特定課税仕入でない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::IsTkkzSiire(struct _DBSY_ATTR *pSyattr)
{
	int	nRt = 0;

	if (((pSyattr->_A == 0x04) && (pSyattr->_B == 0x0b)) || ((pSyattr->_A == 0x04) && (pSyattr->_B == 0x0c))) {
		nRt = 1;
	}
	else if (((pSyattr->_A == 0x04) && (pSyattr->_B == 0x0d)) || ((pSyattr->_A == 0x04) && (pSyattr->_B == 0x0e))) {
		nRt = 2;
	}

	return nRt;
}



//== 科目 関連 ==

//-----------------------------------------------------------------------------
// 借方または貸方が 有価証券 
//-----------------------------------------------------------------------------
// 引数		sub		：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	有価証券
//			FALSE	：	有価証券でない
//-----------------------------------------------------------------------------
BOOL CDataZeiCalqSub::_yuka(struct _DBSYSUB* sub)
{
	ASSERT(sub);
	int db = sub->_SY_DEBT.SY_KMK._KMKTYP;
	int cr = sub->_SY_CRED.SY_KMK._KMKTYP;

	return ((cr == _KMK_TYP_YUKA) || (db == _KMK_TYP_YUKA));
}

//-----------------------------------------------------------------------------
// 資産取得チェック
//-----------------------------------------------------------------------------
// 引数		syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	資産取得
//			FALSE	：	資産取得でない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::is_sisan_syutok(struct _DBSY_ATTR* syattr)
{
	ASSERT(syattr);
	return((syattr->_A == 0x01) && (syattr->_B == 0x00));
}

//-----------------------------------------------------------------------------
// 資産譲渡チェック
//-----------------------------------------------------------------------------
// 引数		syattr	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	資産譲渡
//			FALSE	：	資産譲渡でない
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::is_sisan_jyoto(struct _DBSY_ATTR* syattr)
{
	ASSERT(syattr);
	return((syattr->_A == 0x01) && (syattr->_B == 0x01));
}

//-----------------------------------------------------------------------------
// 展開用バッファ初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::sub_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	_sub_date_inz(sub);		// 日付 初期化
	_sub_debt_inz(sub);		// 借方 初期化
	_sub_cred_inz(sub);		// 貸方 初期化
	_sub_dbcr_inz(sub);		// 借＋貸 初期化
	_sub_value_inz(sub);		// 金額 初期化
	_sub_taikaval_inz(sub);	// 対価金額 初期化
//	sub->pDDATA = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 日付 初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_date_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	sub->_SY_DATE = 0;
	sub->is_DATE_RGST() = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 借方 初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	；	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_debt_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	memset(&sub->_SY_DEBT, 0, sizeof(sub->_SY_DEBT));
	sub->is_DEBT_RGST() = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 貸方 初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	；	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_cred_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	memset(&sub->_SY_CRED, 0, sizeof(sub->_SY_CRED));
	sub->is_CRED_RGST() = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 借方・貸方 初期化
//-----------------------------------------------------------------------------
// 引数	sub			：	対象バッファ
//		debt_cred	：	借方？貸方？
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_kmk_inz(struct _DBSYSUB* sub, int debt_cred)
{
	ASSERT(sub);

	if (debt_cred == _CTL_DEBT) {
		return _sub_debt_inz(sub);
	}
	else if (debt_cred == _CTL_CRED) {
		return _sub_cred_inz(sub);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 仕訳区分 初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_dbcr_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	memset(&sub->_SY_DBCR, 0, sizeof(sub->_SY_DBCR));
	sub->is_DBCR_RGST() = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 金額 初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_value_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	memset(sub->_SY_VALUE, 0, sizeof(sub->_SY_VALUE));			// 仕訳金額
	memset(sub->_SY_ZEIGAK, 0, sizeof(sub->_SY_ZEIGAK));		// 仕訳税額

	sub->is_VALUE_RGST() = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 対価金額 初期化
//-----------------------------------------------------------------------------
// 引数		sub	：	対象バッファ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDataZeiCalqSub::_sub_taikaval_inz(struct _DBSYSUB* sub)
{
	ASSERT(sub);

	memset(sub->_SY_TAIKAVAL, 0, sizeof(sub->_SY_TAIKAVAL));	// 対価金額
	memset(sub->_SY_TAIKAZEI, 0, sizeof(sub->_SY_TAIKAZEI));	// 対価税額

	sub->is_TAIKAVAL_RGST() = 0;

	return 0;
}



///////////////////////////////////////////////////////////////////////////////////
// 仕訳データから、デフォルト税額を計算するエクスポートクラス 
//
//
CDataZeiCalq::CDataZeiCalq()
{
	pSub = new CDataZeiCalqSub();
}


CDataZeiCalq::~CDataZeiCalq()
{
	if (pSub != NULL) {
		delete pSub;
		pSub = NULL;
	}
}

//------------------------------------------------------------
//概要：クラスの初期化
//引数：m_pZm		財務クラスポインタ(オープン済み)
//------------------------------------------------------------
int CDataZeiCalq::DZCIniz(CDBZmSub* zmsub)
{
	if (pSub == NULL) return -1;

	return pSub->DZCIniz(zmsub);
}

int CDataZeiCalq::DZCClose()
{
	if (pSub == NULL) return -1;

	return pSub->DZCClose();
}

//------------------------------------------------------------
//概要：仕訳データの消費税サインからデフォルトの税額を返送する
//引数：pdata	税額を計算する仕訳データ
//------------------------------------------------------------
LPCTSTR CDataZeiCalq::GetDefZeiData(CDBipDataRec* pdata)
{
	if (pSub == NULL) return NULL;

	return pSub->GetDefZeiData(pdata);
}


LPCTSTR CDataZeiCalq::GetDefZeiData(CDBINPDataRec* pdata)
{
	if( pSub == NULL ) return NULL;

	return pSub->GetDefZeiData(pdata);
}


