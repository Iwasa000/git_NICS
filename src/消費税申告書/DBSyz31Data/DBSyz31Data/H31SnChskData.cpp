#include "stdafx.h"
#include "H31SnChskData.h"
#include "voldatedb8.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH31SnChskData::CH31SnChskData()
{
	Sn_ZKDYS = 0;
	Sn_ZKDYE = 0;
	Sn_ZZDAY = 0;
	Sn_BSMON = 0;
	Sn_BBMON = 0;
	memset(Sn_KTNFZ, '\0', sizeof(Sn_KTNFZ));
	memset(Sn_ZSKZN, '\0', sizeof(Sn_ZSKZN));
	memset(Sn_ZSKAD, '\0', sizeof(Sn_ZSKAD));
	memset(Sn_ZSTZN, '\0', sizeof(Sn_ZSTZN));
	memset(Sn_ZSTAD, '\0', sizeof(Sn_ZSTAD));
	memset(Sn_ZSKTZ, '\0', sizeof(Sn_ZSKTZ));

	m_Arith.l_defn(0x16);

	memset(rsv1, '\0', sizeof(rsv1));
	memset(rsv2, '\0', sizeof(rsv2));
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH31SnChskData::~CH31SnChskData()
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
int CH31SnChskData::GetData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if (pDBNpSub->zvol == NULL) {
		return -1;
	}

	CString	filter;
	filter.Format(_T("Sn_SEQ=%d"), sn_seq);
	CSZSNK *pSZSNK = pDBNpSub->szsnk;
	if (pSZSNK == NULL) {
		pDBNpSub->SzsnkOpen(filter);
	}
	else {
		pSZSNK->Requery(filter);
	}
	if (pSZSNK->st == -1) {
		// 新規作成
//		pSZSNK->AddNew();
//		// 新規作成時にデフォルト値は下のものだけでOK??
		// 予定申告時の新規作成は考えない！
		return 0;
	}

	Sn_ZKDYS = pSZSNK->Sn_ZKDYS;									// 前年課税期間　自
	Sn_ZKDYE = pSZSNK->Sn_ZKDYE;									// 前年課税期間　至
	Sn_ZZDAY = pSZSNK->Sn_ZZDAY;									// 修正・更正・決定の年月日
	m_Util.val_to_bin(Sn_KTNFZ, pSZSNK->Sn_KTNFZ);				// 消費税及び地方消費税の合計納付税額
	Sn_BSMON = (char)pSZSNK->Sn_BSMON;								// 月換算　分子
	Sn_BBMON = (char)pSZSNK->Sn_BBMON;								// 月換算　分母
	for (int i = 0; i != 12; i++) {
		m_Util.val_to_bin(Sn_ZSKZN[i], pSZSNK->Sn_ZSKZN[i]);		// 国税　この申告により増加する税額
		m_Util.val_to_bin(Sn_ZSKAD[i], pSZSNK->Sn_ZSKAD[i]);		// 国税　この申告により増加する税額
		m_Util.val_to_bin(Sn_ZSTZN[i], pSZSNK->Sn_ZSTZN[i]);		// 地方税　この申告により増加する税額
		m_Util.val_to_bin(Sn_ZSTAD[i], pSZSNK->Sn_ZSTAD[i]);		// 地方税　この申告により増加する税額
		m_Util.val_to_bin(Sn_ZSKTZ[i], pSZSNK->Sn_ZSKTZ[i]);		// 消費税及び地方消費税の合計納付税額
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB へデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31SnChskData::SetData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if (pDBNpSub->zvol == NULL) {
		return -1;
	}

	CString	filter;
	filter.Format(_T("Sn_SEQ=%d"), sn_seq);
	CSZSNK *pSZSNK = pDBNpSub->szsnk;
	if (pSZSNK == NULL) {
		pDBNpSub->SzsnkOpen(filter);
	}
	else {
		pSZSNK->Requery(filter);
	}

	if (pSZSNK->st == -1) {
		// 新規作成
//		pSZSNK->AddNew();
//		// 新規作成時にデフォルト値は下のものだけでOK??
		// 予定申告時の新規作成は考えない！
		return -1;
	}
	else {
		// 更新
		pSZSNK->Edit();
	}

	pSZSNK->Sn_ZKDYS = Sn_ZKDYS;	// 前年課税期間　自
	pSZSNK->Sn_ZKDYE = Sn_ZKDYE;	// 前年課税期間　至
	pSZSNK->Sn_ZZDAY = Sn_ZZDAY;	// 修正・更正・決定の年月日
	m_Util.val_to_asci(&pSZSNK->Sn_KTNFZ, Sn_KTNFZ);	// 消費税及び地方消費税の合計納付税額
	pSZSNK->Sn_BSMON = Sn_BSMON;			// 月換算　分子
	pSZSNK->Sn_BBMON = Sn_BBMON;			// 月換算　分母
	for (int i = 0; i != 12; i++) {
		m_Util.val_to_asci(&pSZSNK->Sn_ZSKZN[i], Sn_ZSKZN[i]);		// 国税　この申告により増加する税額
		m_Util.val_to_asci(&pSZSNK->Sn_ZSKAD[i], Sn_ZSKAD[i]);		// 国税　この申告により増加する税額
		m_Util.val_to_asci(&pSZSNK->Sn_ZSTZN[i], Sn_ZSTZN[i]);		// 地方税　この申告により増加する税額
		m_Util.val_to_asci(&pSZSNK->Sn_ZSTAD[i], Sn_ZSTAD[i]);		// 地方税　この申告により増加する税額
		m_Util.val_to_asci(&pSZSNK->Sn_ZSKTZ[i], Sn_ZSKTZ[i]);		// 消費税及び地方消費税の合計納付税額
	}

	pSZSNK->Update();

	return 0;
}

//-----------------------------------------------------------------------------
// 年月日データの取得
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	取得元データの種類
//		pYy		：	年
//		pMm		：	月
//		pDd		：	日
//-----------------------------------------------------------------------------
void CH31SnChskData::GetYmdData(EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd)
{
	long	*ptmpYmd = NULL;

	if (dsDtype == ID_ZENKI_KAZEIKIKAN_FROM) {		// 前期課税期間 自
		ptmpYmd = &Sn_ZKDYS;
	}
	else if (dsDtype == ID_ZENKI_KAZEIKIKAN_TO) {	// 前期課税期間 至
		ptmpYmd = &Sn_ZKDYE;
	}
	else if (dsDtype == ID_SYUZEI_DECIDE_DAY) {		// 修正・更正・決定の年月日
		ptmpYmd = &Sn_ZZDAY;
	}
	else {
		// 該当 元データ無し
	}

	if ((*ptmpYmd) == 0) {
		return;
	}

	// 西暦を平成に変換
	int	dYear = 0;
	CVolDateDB	voldate;
	voldate.db_datecnv((*ptmpYmd), &dYear, 0, 2);
	m_Arith.int_bcd((void*)pYy, dYear, 1);

	// 月
	int dMon = 0;
	dMon = ((*ptmpYmd) % 10000) / 100;
	m_Arith.int_bcd((void*)pMm, dMon, 1);

	// 日
	int dDay = 0;
	dDay = (*ptmpYmd) % 100;
	m_Arith.int_bcd((void*)pDd, dDay, 1);
}

//-----------------------------------------------------------------------------
// 年月日データの取込
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	データの取込先
//		type	：	年？月？日？
//		data	：	BCDデータ
//-----------------------------------------------------------------------------
void CH31SnChskData::SetYmdData(EnumIdIcsShDateType dsDtype, int type, char data)
{
	int	rtYmd = 0;

	// 取得データの変換
	int nData = m_Arith.bcd_int(&data, 1);

	if (type == 0) {		// 年
		// 平成を西暦に変換
		int	dYear = 0;
		CVolDateDB	voldate;
		voldate.db_datecnv(nData, &dYear, 1, 2);
		nData = dYear;
		if (data == 0x00) {	// 取消を意味する！
			nData = 0;
		}
	}
	else if (type == 1) {	//[Y] 月
		if ((nData < 1) || (12 < nData)) {
			// 不正データ
		}
	}
	else if (type == 2) {	//[Y] 日
		if ((nData < 1) || (31 < nData)) {
			// 不正データ
		}
	}

	long	*ptmpYmd = NULL;

	if (dsDtype == ID_ZENKI_KAZEIKIKAN_FROM) {		// 前期課税期間 自
		ptmpYmd = &Sn_ZKDYS;
	}
	else if (dsDtype == ID_ZENKI_KAZEIKIKAN_TO) {	// 前期課税期間 至
		ptmpYmd = &Sn_ZKDYE;
	}
	else if (dsDtype == ID_SYUZEI_DECIDE_DAY) {		// 修正・更正・決定の年月日
		ptmpYmd = &Sn_ZZDAY;
	}
	else {
		// 該当 元データ無し
	}

	if (type == 0) {		// 年
		int	mmdd = (*ptmpYmd) % 10000;
		(*ptmpYmd) = mmdd + (nData * 10000);
	}
	else if (type == 1) {	// 月
		int yy = (*ptmpYmd) / 10000;
		int dd = (*ptmpYmd) % 100;
		(*ptmpYmd) = (yy * 10000) + (nData * 100) + dd;
	}
	else if (type == 2) {	// 日
		int yymm = (*ptmpYmd) / 100;
		(*ptmpYmd) = (yymm * 100) + nData;
	}
}

//-----------------------------------------------------------------------------
// 更新時の金額クリア
//-----------------------------------------------------------------------------
// 引数	
//
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31SnChskData::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{
	Sn_ZKDYS = 0;
	Sn_ZKDYE = 0;
	m_Arith.l_clear(Sn_KTNFZ);
	Sn_BSMON = 0;
	Sn_BBMON = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 年月日データの取得
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	取得元データの種類
//		pYy		：	年
//		pMm		：	月
//		pDd		：	日
//		pG		：　元号
//-----------------------------------------------------------------------------
void CH31SnChskData::GetYmdDataGen(EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd, int *pG)
{
	long	*ptmpYmd = NULL;
	int		gengo = 0;

	if (dsDtype == ID_ZENKI_KAZEIKIKAN_FROM) {		// 前期課税期間 自
		ptmpYmd = &Sn_ZKDYS;
	}
	else if (dsDtype == ID_ZENKI_KAZEIKIKAN_TO) {	// 前期課税期間 至
		ptmpYmd = &Sn_ZKDYE;
	}
	else if (dsDtype == ID_SYUZEI_DECIDE_DAY) {		// 修正・更正・決定の年月日
		ptmpYmd = &Sn_ZZDAY;
	}
	else {
		// 該当 元データ無し
	}

	if ((*ptmpYmd) == 0) {
		return;
	}

	// 西暦を平成に変換
	int	dYear = 0;
	CVolDateDB	voldate;
	voldate.db_datecnvGen(0, (*ptmpYmd), &gengo, &dYear, 0, 0);
	dYear = ((dYear / 10000) % 100);
	m_Arith.int_bcd((void*)pYy, dYear, 1);

	// 月
	int dMon = 0;
	dMon = ((*ptmpYmd) % 10000) / 100;
	m_Arith.int_bcd((void*)pMm, dMon, 1);

	// 日
	int dDay = 0;
	dDay = (*ptmpYmd) % 100;
	m_Arith.int_bcd((void*)pDd, dDay, 1);

	if (pG != NULL) {
		*pG = gengo;
	}
}

//-----------------------------------------------------------------------------
// 年月日データの取込（改元対応版）
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	データの取込先
//		type	：	年？月？日？
//		data	：	BCDデータ
//		gengo   ：　元号
//-----------------------------------------------------------------------------
 //暫定で平成固定
void CH31SnChskData::SetYmdDataGen(EnumIdIcsShDateType dsDtype, int type, char data, int gengo/*=4*/)
{
	int	rtYmd = 0;

	// 取得データの変換
	int nData = m_Arith.bcd_int(&data, 1);

	if (type == 0) {		// 年
		if (data == 0x00) {	// 取消を意味する！
			nData = 0;
		}
		else {
			// 和暦を西暦に変換
			int	dYear = 0;
			CVolDateDB	voldate;
			int dummy;
			voldate.db_datecnvGen(gengo, nData, &dummy, &dYear, 1, 2);
			nData = dYear;
		}
	}
	else if (type == 1) {	//[Y] 月
		if ((nData < 1) || (12 < nData)) {
			// 不正データ
		}
	}
	else if (type == 2) {	//[Y] 日
		if ((nData < 1) || (31 < nData)) {
			// 不正データ
		}
	}

	long	*ptmpYmd = NULL;

	if (dsDtype == ID_ZENKI_KAZEIKIKAN_FROM) {		// 前期課税期間 自
		ptmpYmd = &Sn_ZKDYS;
	}
	else if (dsDtype == ID_ZENKI_KAZEIKIKAN_TO) {	// 前期課税期間 至
		ptmpYmd = &Sn_ZKDYE;
	}
	else if (dsDtype == ID_SYUZEI_DECIDE_DAY) {		// 修正・更正・決定の年月日
		ptmpYmd = &Sn_ZZDAY;
	}
	else {
		// 該当 元データ無し
	}

	if (type == 0) {		// 年
		int	mmdd = (*ptmpYmd) % 10000;
		(*ptmpYmd) = mmdd + (nData * 10000);
	}
	else if (type == 1) {	// 月
		int yy = (*ptmpYmd) / 10000;
		int dd = (*ptmpYmd) % 100;
		(*ptmpYmd) = (yy * 10000) + (nData * 100) + dd;
	}
	else if (type == 2) {	// 日
		int yymm = (*ptmpYmd) / 100;
		(*ptmpYmd) = (yymm * 100) + nData;
	}
}



