#include "stdafx.h"

#include "DInputSub.h"
#include "DBInpSubFunc.h"


class CSWKDSubW : public CDInputSub
{
public:
	CSWKDSubW();
	virtual ~CSWKDSubW();


public:
	struct _AUTOSEL m_AutoSel;
	CWnd*	m_pParent;

};


CSWKDSubW::CSWKDSubW()
{
	//CSWKDataSub用サインON
	UseSWKDsub();
	m_pParent = NULL;
}


CSWKDSubW::~CSWKDSubW()
{

}



///////////////////////////////////////////////////////////////////////////
//仕訳入力データ関連を扱うクラス

CSWKDataSub::CSWKDataSub() : psub(NULL)
{
	psub = new CSWKDSubW();
}

CSWKDataSub::~CSWKDataSub()
{
	if( psub != NULL ) {
		delete psub;
	}
}


int	CSWKDataSub::SWKDIniz(CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pParent)
{
	if( psub == NULL )	return -1;

	CDinpINI	ini;
	ini.Init(pZm);

	//ポインタ内容を保存する
	struct _AUTOSEL* psel = ini.GetDinpSW();
	memcpy(&psub->m_AutoSel, psel, sizeof(struct _AUTOSEL));
	psub->m_pParent = pParent;

	if( psub->InitSub(pZm, pSy, &psub->m_AutoSel, pParent) != 0 )
		return -1;

	return 0;
}


int	CSWKDataSub::SWKDClose()
{
	if( psub == NULL )	return -1;

	psub->CloseSub();

	return 0;
}



int	CSWKDataSub::km_syattr(DBKNREC* pKn) 
{
	return psub->km_syattr(pKn);
}

int	CSWKDataSub::km_syattr(CString& kcode)
{
	return psub->km_syattr(kcode);
}

int	CSWKDataSub::km_yusyutsu(DBKNREC* pKn)
{
	return psub->km_yusyutsu(pKn);
}

int	CSWKDataSub::km_yusyutsu(CString& kcode)
{
	return psub->km_yusyutsu(kcode);
}


int	CSWKDataSub::km_zeikbn(DBKNREC* pKn)
{
	return psub->km_zeikbn(pKn);
}

int	CSWKDataSub::km_zeikbn(CString& kcode)
{
	return psub->km_zeikbn(kcode);
}

void	CSWKDataSub::GetCData(CDBipDataRec* rec)
{
	return psub->GetCData(rec);
}
void	CSWKDataSub::GetCData(CDBINPDataRec* rec)
{
	return psub->GetCData(rec);
}

void	CSWKDataSub::SetCDBData(CDBipDataRec* rec)
{
	return psub->SetCDBData(rec);
}
void	CSWKDataSub::SetCDBData(CDBINPDataRec* rec)
{
	return psub->SetCDBData(rec);
}


void	CSWKDataSub::GetCDBData(CDBipDataRec* rec, CDBDATA* ptr/* = NULL*/)
{
	return psub->GetCDBData(rec, ptr);
}
void	CSWKDataSub::GetCDBData(CDBINPDataRec* rec, CDBDATA* ptr/* = NULL*/)
{
	return psub->GetCDBData(rec, ptr);
}

int		CSWKDataSub::DB_DataCorrect(CDBipDataRec* data)
{
	return psub->DB_DataCorrect(data);
}
int		CSWKDataSub::DB_DataCorrect(CDBINPDataRec* data)
{
	return psub->DB_DataCorrect(data);
}

int		CSWKDataSub::IsHjiyuItemGyosyu() 
{
	return psub->IsHjiyuItemGyosyu();
}


int		CSWKDataSub::DB_SyIniz(CDBipDataRec* data) 
{
	return psub->DB_SyIniz(data);
}
int		CSWKDataSub::DB_SyIniz(CDBINPDataRec* data)
{
	return psub->DB_SyIniz(data);
}


LPCTSTR CSWKDataSub::sy_chk_txt(CDBipDataRec *data, struct _SY_MSG_PACK2* smp)
{
	static CString	systr;
	systr = psub->sy_chk_txt(data, smp);

	psub->GetCDBData(data);

	systr_adj(systr);
	return systr;
}

LPCTSTR CSWKDataSub::sy_chk_txt(CDBINPDataRec *data, struct _SY_MSG_PACK2* smp)
{
	static CString	systr;
	systr = psub->sy_chk_txt(data, smp);

	psub->GetCDBData(data);

	systr_adj(systr);
	return systr;
}

// 消費税 文字列スペース調整
void CSWKDataSub::systr_adj(CString& systr)
{
	int idx = systr.Find('/');

	if( idx != -1 ) {
		CString a, b, str;
		a = systr.Left(idx);

		int n = kjlen((void*)(LPCTSTR)a, idx);
		str = a.Left(n);
		if( n < idx )	str += " ";
		//MyTrace( "@systr_adj a = %s, n = %d, str = %s", a, n, str );

		int len = systr.GetLength();
		b = systr.Right(len - idx);

		systr = str + b;
	}
}

// 業種・売り仕入マスターチェック
int CSWKDataSub::SyzeiMasterType()
{
	return psub->SyzeiMasterType();
}


int		CSWKDataSub::brchk1(CDBipDataRec *dta, int dbcr_sg)
{
	return psub->brchk1(dta, dbcr_sg);
}
int		CSWKDataSub::brchk1(CDBINPDataRec *dta, int dbcr_sg)
{
	return psub->brchk1(dta, dbcr_sg);
}

void	CSWKDataSub::del_dtaeda(CDBipDataRec *dta, int dbcr_sg) 
{
	return psub->del_dtaeda(dta, dbcr_sg);
}
void	CSWKDataSub::del_dtaeda(CDBINPDataRec *dta, int dbcr_sg)
{
	return psub->del_dtaeda(dta, dbcr_sg);
}

int		CSWKDataSub::bchk_dbr(CDBipDataRec *dta, int msg_sg)
{
	return psub->bchk_dbr(dta, msg_sg);
}
int		CSWKDataSub::bchk_dbr(CDBINPDataRec *dta, int msg_sg)
{
	return psub->bchk_dbr(dta, msg_sg);
}

int		CSWKDataSub::bchk_cbr(CDBipDataRec *dta, int msg_sg)
{
	return psub->bchk_cbr(dta, msg_sg);
}
int		CSWKDataSub::bchk_cbr(CDBINPDataRec *dta, int msg_sg)
{
	return psub->bchk_cbr(dta, msg_sg);
}

void	CSWKDataSub::bchk_msg(int rval, int dbcr_sg, int bmn, CString& code, int brn)
{
	return psub->bchk_msg(rval, dbcr_sg, bmn, code, brn);
}

BOOL	CSWKDataSub::codeinp_chk(LPCTSTR code)
{
	return psub->codeinp_chk(code);
}

int	CSWKDataSub::brmsrch(CDBipEZREC *brec, CDBipDataRec *data, int dbcr_sg)
{
	return psub->brmsrch(brec, data, dbcr_sg);
}
int	CSWKDataSub::brmsrch(CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg)
{
	return psub->brmsrch(brec, data, dbcr_sg);
}

int	CSWKDataSub::br_entry(CDBipEZREC *brec, CDBipDataRec *data, int dbcr_sg, int msg_sg)
{
	return psub->br_entry(brec, data, dbcr_sg, msg_sg);
}
int	CSWKDataSub::br_entry(CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg, int msg_sg)
{
	return psub->br_entry(brec, data, dbcr_sg, msg_sg);
}

int	CSWKDataSub::zroeda_torok(CDBipDataRec *data, int dbcr_sg)
{
	return psub->zroeda_torok(data, dbcr_sg);
}
int	CSWKDataSub::zroeda_torok(CDBINPDataRec *data, int dbcr_sg)
{
	return psub->zroeda_torok(data, dbcr_sg);
}

int	CSWKDataSub::brzan_torok(CWnd* pwnd, int bmn, CString code, int brn, BOOL dbsgn)
{
	return psub->brzan_torok(pwnd, bmn, code, brn, dbsgn);
}


int	CSWKDataSub::DB_EdabanMasterSearch(CDBipDataRec* data, int dbcr_sg)
{
	return psub->DB_EdabanMasterSearch(data, dbcr_sg);
}
int	CSWKDataSub::DB_EdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg)
{
	return psub->DB_EdabanMasterSearch(data, dbcr_sg);
}

int	CSWKDataSub::EdabanZanTorok(CDBipEZREC* ez)
{
	return psub->EdabanZanTorok(ez);
}

int	CSWKDataSub::BmnEdabanZanTorok(CDBipEZREC* ez, int bmn)
{
	return psub->BmnEdabanZanTorok(ez, bmn);
}


int	CSWKDataSub::DB_EdabanZanSearch(CString& kmkcode, int brn)
{
	return psub->DB_EdabanZanSearch(kmkcode, brn);
}

int	CSWKDataSub::DB_EdabanZanSearch2(CString& kmkcode, int tkcod)
{
	return psub->DB_EdabanZanSearch2(kmkcode, tkcod);
}

int	CSWKDataSub::DB_BmnEdabanMasterSearch(CDBipDataRec* data, int dbcr_sg)
{
	return psub->DB_BmnEdabanMasterSearch(data, dbcr_sg);
}
int	CSWKDataSub::DB_BmnEdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg)
{
	return psub->DB_BmnEdabanMasterSearch(data, dbcr_sg);
}

int	CSWKDataSub::DB_BmnEdabanMasterSearch(CDBipDataRec* data, int dbcr_sg, int tkcod)
{
	return psub->DB_BmnEdabanMasterSearch(data, dbcr_sg, tkcod);
}
int	CSWKDataSub::DB_BmnEdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg, int tkcod)
{
	return psub->DB_BmnEdabanMasterSearch(data, dbcr_sg, tkcod);
}



int	CSWKDataSub::DB_TekiyoNoRead(CDBipTKREC* tek_rec, int no)
{
	return psub->DB_TekiyoNoRead(tek_rec, no);
}

int	CSWKDataSub::tbrasrch(CDBipDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype)
{
	return psub->tbrasrch(data, tkcd, dbr_inptype, cbr_inptype);
}
int	CSWKDataSub::tbrasrch(CDBINPDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype)
{
	return psub->tbrasrch(data, tkcd, dbr_inptype, cbr_inptype);
}

int	CSWKDataSub::DB_TekiyoEdabanSet(CDBipDataRec *data, int tkcode, BOOL bBmn)
{
	return psub->DB_TekiyoEdabanSet(data, tkcode, bBmn);;
}
int	CSWKDataSub::DB_TekiyoEdabanSet(CDBINPDataRec *data, int tkcode, BOOL bBmn)
{
	return psub->DB_TekiyoEdabanSet(data, tkcode, bBmn);;
}


BOOL	CSWKDataSub::IsModify()
{
	return psub->IsModify();
}

BOOL	CSWKDataSub::IsEdaAdd()
{
	return psub->IsEdaAdd();
}


BOOL	CSWKDataSub::IsGokeiBumon(int bmn)
{
	return psub->IsGokeiBumon(bmn);
}

BOOL	CSWKDataSub::_set_bmnetc(struct _SET_KMKETC *sk)
{
	return psub->_set_bmnetc(sk);
}


BOOL	CSWKDataSub::IsSyonum()
{
	return psub->IsSyonum();
}

BOOL	CSWKDataSub::IsEdaban()
{
	return psub->IsEdaban();
}

BOOL	CSWKDataSub::IsBumon()
{
	return psub->IsBumon();
}

BOOL	CSWKDataSub::IsKoji()
{
	return psub->IsKoji();
}


BOOL	CSWKDataSub::IsMasterType(DWORD typbit) 
{
	return psub->IsMasterType( typbit);
}

BOOL	CSWKDataSub::IsInsertMaster()
{
	return psub->IsInsertMaster();
}

BOOL	CSWKDataSub::IsConFirmMaster()
{
	return psub->IsConFirmMaster();
}


BOOL	CSWKDataSub::tkrec_kanamode()
{
	return psub->tkrec_kanamode();
}

void	CSWKDataSub::tkrec_queryAll()
{
	return psub->tkrec_queryAll();
}


BOOL	CSWKDataSub::_set_kmketc(struct _SET_KMKETC* psk, BOOL full/* = TRUE*/)
{
	return psub->_set_kmketc(psk, full);
}


int		CSWKDataSub::Confirm_Mode() 
{
	return psub->Confirm_Mode();
}

COLORREF CSWKDataSub::GetConfirmBkColor()
{
	return psub->GetConfirmBkColor();
}

BOOL	CSWKDataSub::CheckConfirmBkColor(COLORREF back)
{
	return psub->CheckConfirmBkColor(back);
}

BOOL	CSWKDataSub::IsModifyConfirmData()
{
	return psub->IsModifyConfirmData();
}

BOOL	CSWKDataSub::IsJozuEnvCloudMaster()
{
	return psub->IsJozuEnvCloudMaster();
}


LPCTSTR CSWKDataSub::get_hjiyutitle(int strtype)
{
	return psub->get_hjiyutitle( strtype);
}

int	CSWKDataSub::tline_chk(CDBipDataRec *drec)
{
	return psub->tline_chk(drec);
}
int	CSWKDataSub::tline_chk(CDBINPDataRec *drec)
{
	return psub->tline_chk(drec);
}

BOOL	CSWKDataSub::bmncode_ok(int bmn)
{
	return psub->bmncode_ok(bmn);
}


int		CSWKDataSub::GetLastDataSeq()
{
	return psub->GetLastDataSeq();
}

int		CSWKDataSub::GetKariAutoBrnCnt()
{
	return psub->GetKariAutoBrnCnt();
}

int		CSWKDataSub::BumonZanTorok(int bmn, CString  kmcod)
{
	return psub->BumonZanTorok(bmn, kmcod);
}

int		CSWKDataSub::DB_DataAppend(CDBipDataRec* data, int bseq)
{
	return psub->DB_DataAppend(data, bseq);
}
int		CSWKDataSub::DB_DataAppend(CDBINPDataRec* data, int bseq)
{
	return psub->DB_DataAppend(data, bseq);
}


void	CSWKDataSub::GetInpDate(BYTE* bcddate)
{
	return psub->GetInpDate(bcddate);
}

void	CSWKDataSub::SetInpDate(int ddate, int ksign)
{
	return psub->SetInpDate(ddate, ksign);
}


int	CSWKDataSub::GetInputStrLen(unsigned char *buf)
{
	return psub->GetInputStrLen(buf);
}

BOOL CSWKDataSub::isdiff_data(CDBipDataRec *newdata, CDBipDataRec *moto)
{
	return psub->isdiff_data(newdata, moto);
}
BOOL CSWKDataSub::isdiff_data(CDBINPDataRec *newdata, CDBINPDataRec *moto)
{
	return psub->isdiff_data(newdata, moto);
}

int	CSWKDataSub::CalqSyogTotal(LPCTSTR syogCode, char *tsgd, char *tsgc, char *zan)
{
	return psub->CalqSyogTotal(syogCode, tsgd, tsgc, zan);
}

int	CSWKDataSub::memzrec_get(LPCTSTR code, struct _DBKZREC* zrec)
{
	return psub->memzrec_get(code, zrec);
}

int	CSWKDataSub::dbzrec_get(LPCTSTR code, struct _DBKZREC* zrec)
{
	return psub->dbzrec_get(code, zrec);
}

int	CSWKDataSub::zrecget(LPCTSTR code, struct _DBKZREC* zrec)
{
	return psub->zrecget(code, zrec);
}


int	CSWKDataSub::DateConv(BYTE* bcddate, CDBipDataRec* d_date, int sgn)
{
	return psub->DateConv(bcddate, d_date, sgn);
}
int	CSWKDataSub::DateConv(BYTE* bcddate, CDBINPDataRec* d_date, int sgn)
{
	return psub->DateConv(bcddate, d_date, sgn);
}

int	CSWKDataSub::DateConv(BYTE* bcddate, int* date, int sgn)
{
	return psub->DateConv(bcddate, date, sgn);
}

int	CSWKDataSub::BcdDateToYmd(BYTE* bcddate, int* ymd)
{
	return psub->BcdDateToYmd(bcddate, ymd);
}


BOOL CSWKDataSub::isSyzdiff_data(CDBipDataRec *newdata, CDBipDataRec *moto)
{
	return psub->isSyzdiff_data(newdata, moto);
}
BOOL CSWKDataSub::isSyzdiff_data(CDBINPDataRec *newdata, CDBINPDataRec *moto)
{
	return psub->isSyzdiff_data(newdata, moto);
}


BOOL CSWKDataSub::is_syohizeisiwake(CDBDATA* dbdata)
{
	return psub->is_syohizeisiwake(dbdata);
}

BOOL CSWKDataSub::is_syohizeisiwake(CDBipDataRec* data)
{
	return psub->is_syohizeisiwake(data);
}
BOOL CSWKDataSub::is_syohizeisiwake(CDBINPDataRec* data)
{
	return psub->is_syohizeisiwake(data);
}

BOOL CSWKDataSub::is_yusyutsusiwake(CDBDATA* dbdata)
{
	return psub->is_yusyutsusiwake(dbdata);
}

BOOL CSWKDataSub::is_yusyutsusiwake(CDBipDataRec* data)
{
	return psub->is_yusyutsusiwake(data);
}
BOOL CSWKDataSub::is_yusyutsusiwake(CDBINPDataRec* data)
{
	return psub->is_yusyutsusiwake(data);
}

BOOL CSWKDataSub::IsDiffEdabanSyzSgn(char* kmkcd, int brn)
{
	return psub->IsDiffEdabanSyzSgn(kmkcd, brn);
}


BOOL	CSWKDataSub::isnot_defzeigaku(CDBDATA* dbdata, COLORREF& back)
{
	return psub->isnot_defzeigaku(dbdata, back);
}

BOOL	CSWKDataSub::isnot_defzeigaku(CDBipDataRec* data, COLORREF& back)
{
	return psub->isnot_defzeigaku(data, back);

}
BOOL	CSWKDataSub::isnot_defzeigaku(CDBINPDataRec* data, COLORREF& back)
{
	return psub->isnot_defzeigaku(data, back);

}

int	CSWKDataSub::IsKaniOrKobetsu()
{
	return psub->IsKaniOrKobetsu();
}

int		CSWKDataSub::IsKani()
{
	return psub->IsKani();
}

int		CSWKDataSub::IsMenzei()
{
	return psub->IsMenzei();
}

int		CSWKDataSub::IsKobetsu()
{
	return psub->IsKobetsu();
}

bool	CSWKDataSub::CheckTkrec(_SY_TKREC_ *syrec)
{
	return psub->CheckTkrec(syrec);
}

BOOL  CSWKDataSub::is_karibarai_menzei(BYTE* dsign)
{
	return psub->is_karibarai_menzei(dsign);
}

void	CSWKDataSub::KamokuString(DBKNREC* pKn, CString& string)
{
	return psub->KamokuString(pKn, string);
}

void	CSWKDataSub::KamokuString(DBKNREC* pKn, char* buf, int bufsize)
{
	return psub->KamokuString(pKn, buf, bufsize);
}


// 文字列科目コード より科目名称を取得
struct _DBKNREC* CSWKDataSub::DB_PjisToKnrec(CString pjiscode, BOOL bALL/*=FALSE*/)
{
	return ::DB_PjisToKnrec(pjiscode, psub->m_pZm, bALL);
}


// DWORD 科目コード より科目名称を取得
struct _DBKNREC* CSWKDataSub::DB_PjisToKnrec(DWORD dwPjis, BOOL bALL/*=FALSE*/)
{
	return ::DB_PjisToKnrec(dwPjis, psub->m_pZm, bALL);
}


struct _AUTOSEL *CSWKDataSub::GetAUTOSEL()
{
	return &psub->m_AutoSel;
}


void CSWKDataSub::get_datelimit(int& sofs, int& eofs, int& sy, int& ey)
{
	psub->get_datelimit( sofs, eofs, sy, ey);
}


void CSWKDataSub::set_datelimit(int sofs, int eofs, int sy, int ey)
{
	psub->set_datelimit(sofs, eofs, sy, ey);
}

BOOL CSWKDataSub::check_datelimit(BYTE* bcddate)
{
	return psub->check_datelimit(bcddate);
}

void CSWKDataSub::get_ofsdate(int ofs, int sgn, BYTE* bcddate)
{
	psub->get_ofsdate( ofs, sgn, bcddate);
}

void CSWKDataSub::get_ofsdate(int ofs, BYTE* bcddate)
{
	psub->get_ofsdate( ofs, bcddate);
}
	

int CSWKDataSub::inpdate_shimechk(BYTE* orgdate, char* check)
{
	return psub->inpdate_shimechk(orgdate, check);
}

int	CSWKDataSub::Myvd_chek(BYTE* date)
{
	return psub->Myvd_chek(date);
}

int	CSWKDataSub::Myvd_sign(BYTE* date, BYTE* dst_date)
{
	return psub->Myvd_sign(date, dst_date);
}


int	CSWKDataSub::IsKobetsuBmnSyz()
{
	return psub->IsKobetsuBmnSyz();
}


int CSWKDataSub::KobetsuSiwakeCheck(CDBipDataRec* rec, int bmn)
{
	return psub->KobetsuSiwakeCheck( rec, bmn);
}
int CSWKDataSub::KobetsuSiwakeCheck(CDBINPDataRec* rec, int bmn)
{
	return psub->KobetsuSiwakeCheck(rec, bmn);
}


int	CSWKDataSub::get_brtek(int bmn, CString& code, int brn)
{
	return psub->get_brtek( bmn, code, brn);
}

void CSWKDataSub::get_hjiyustr(BYTE h_code, CString& str)
{
	return psub->get_hjiyustr(h_code, str);
}

BOOL CSWKDataSub::GetHjiyuEtcStr(int hjcd, int nbcd, CString& str)
{
	return psub->GetHjiyuEtcStr(hjcd, nbcd, str);
}


BOOL CSWKDataSub::_set_kojietc(struct _SET_KMKETC *sk)
{
	return psub->_set_kojietc(sk);
}


int	CSWKDataSub::km_siire(DBKNREC* pKn)
{
	return psub->km_siire(pKn);
}

// 外税・免税仕訳か？
BOOL CSWKDataSub::is_sotomensiwake(CDBDATA* dbdata)
{
	return psub->is_sotomensiwake(dbdata);
}
BOOL CSWKDataSub::is_sotomensiwake(CDBipDataRec* data)
{
	return psub->is_sotomensiwake(data);
}
BOOL CSWKDataSub::is_sotomensiwake(CDBINPDataRec* data)
{
	return psub->is_sotomensiwake(data);
}


// 仮払仕訳か？
BOOL CSWKDataSub::IsKaribarai(DWORD kncd)
{
	return psub->IsKaribarai(kncd);
}

// 仮受仕訳か？
BOOL CSWKDataSub::IsKariuke(DWORD kncd)
{
	return psub->IsKariuke(kncd);
}
