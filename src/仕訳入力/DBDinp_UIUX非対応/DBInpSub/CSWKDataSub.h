#pragma once


class CSWKDSubW;

//--追加名称取得用構造体****************
//	科目＋枝番時 の枝番摘要名称
//	部門 の部門名称
//	工事 の工事名称
//**************************************
struct _SET_KMKETC
{
	int			bmn;		//部門
	CString		kno;		//工事番号
	CString		code;		//科目コード
	int			brn;		//枝番コード
	CString		etc;		//追加名称格納
	COLORREF	etc_col;	//追加名称表示文字色
};

// 仕訳データサイン バイト数
#define SZ_DSGN			16
#define SZ_SYZDSGN		7
#define TKYO_LEN		40

#define SYGET_TYPE		0x07

#define SONEKICODE		0x08000000

// マスターフラグ
#define	MST_INSERT		0x00000001
#define MST_TKY20		0x00000002
#define MST_SYZ5_8		0x00000004
#define MST_SYZ8		0x00000008
#define MST_SYZ8_10		0x00000010
#define MST_SYZ10		0x00000020
#define MST_SYZ_INV		0x00000040

// SyzeiMasterType 返送値
#define NORMAL_MST		0	// 非消費税マスター
#define NORMAL_SYMST	1	// 通常
#define KOBET_SYMST		2	// 個別対応
#define KANI_SYMST		3	// 簡易


#ifdef _SWKDATASUB_
class AFX_EXT_CLASS CSWKDataSub
#else
class CSWKDataSub
#endif
{
public:
	CSWKDataSub();
	virtual ~CSWKDataSub();

public:
	int		SWKDIniz(CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pParent);
	int		SWKDClose();

	int		km_syattr(DBKNREC* pKn);
	int		km_syattr(CString& kcode);
	int		km_yusyutsu(DBKNREC* pKn);
	int		km_yusyutsu(CString& kcode);
	int		km_zeikbn(DBKNREC* pKn);
	int		km_zeikbn(CString& kcode);
	
	// 仕訳レコードクラスを CDBipDataRecからCDBINPDataRec に変更した（電子記録債権対応）
	// 古い環境から呼ばれることを考慮し、引数に CDBipDataRec を使用しているものは、
	// CDBINPDataRec を引数として 別関数を作成する

	void	GetCData(CDBipDataRec* rec);
	void	GetCData(CDBINPDataRec* rec);

	void	SetCDBData(CDBipDataRec* rec);
	void	SetCDBData(CDBINPDataRec* rec);

	void	GetCDBData(CDBipDataRec* rec, CDBDATA* ptr = NULL);
	void	GetCDBData(CDBINPDataRec* rec, CDBDATA* ptr = NULL);

	int		DB_DataCorrect(CDBipDataRec* data);
	int		DB_DataCorrect(CDBINPDataRec* data);
	int		IsHjiyuItemGyosyu();

	int		DB_SyIniz(CDBipDataRec* data);
	int		DB_SyIniz(CDBINPDataRec* data);

	void	systr_adj(CString& systr);
	LPCTSTR sy_chk_txt(CDBipDataRec *data, struct _SY_MSG_PACK2* smp);
	LPCTSTR sy_chk_txt(CDBINPDataRec *data, struct _SY_MSG_PACK2* smp);

	int		brchk1(CDBipDataRec *dta, int dbcr_sg);
	int		brchk1(CDBINPDataRec *dta, int dbcr_sg);
	void	del_dtaeda(CDBipDataRec *dta, int dbcr_sg);
	void	del_dtaeda(CDBINPDataRec *dta, int dbcr_sg);
	int		bchk_dbr(CDBipDataRec *dta, int msg_sg);
	int		bchk_dbr(CDBINPDataRec *dta, int msg_sg);
	int		bchk_cbr(CDBipDataRec *dta, int msg_sg);
	int		bchk_cbr(CDBINPDataRec *dta, int msg_sg);

	void	bchk_msg(int rval, int dbcr_sg, int bmn, CString& code, int brn);
	BOOL	codeinp_chk(LPCTSTR code);

	int		brmsrch(CDBipEZREC *brec, CDBipDataRec *data, int dbcr_sg);
	int		brmsrch(CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg);
	int		br_entry(CDBipEZREC *brec, CDBipDataRec *data, int dbcr_sg, int msg_sg);
	int		br_entry(CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg, int msg_sg);
	int		zroeda_torok(CDBipDataRec *data, int dbcr_sg);
	int		zroeda_torok(CDBINPDataRec *data, int dbcr_sg);

	int		brzan_torok(CWnd* pwnd, int bmn, CString code, int brn, BOOL dbsgn);

	int		DB_EdabanMasterSearch(CDBipDataRec* data, int dbcr_sg);
	int		DB_EdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg);
	int		EdabanZanTorok(CDBipEZREC* ez);
	int		BmnEdabanZanTorok(CDBipEZREC* ez, int bmn);

	int		DB_EdabanZanSearch(CString& kmkcode, int brn);
	int		DB_EdabanZanSearch2(CString& kmkcode, int tkcod);
	int		DB_BmnEdabanMasterSearch(CDBipDataRec* data, int dbcr_sg);
	int		DB_BmnEdabanMasterSearch(CDBipDataRec* data, int dbcr_sg, int tkcod);
	int		DB_BmnEdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg);
	int		DB_BmnEdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg, int tkcod);


	int		DB_TekiyoNoRead(CDBipTKREC* tek_rec, int no);
	int		tbrasrch(CDBipDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype);
	int		tbrasrch(CDBINPDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype);
	int		DB_TekiyoEdabanSet(CDBipDataRec *data, int tkcode, BOOL bBmn);
	int		DB_TekiyoEdabanSet(CDBINPDataRec *data, int tkcode, BOOL bBmn);

	BOOL	IsModify();
	BOOL	IsEdaAdd();

	BOOL	IsGokeiBumon(int bmn);
	BOOL	_set_bmnetc(struct _SET_KMKETC *sk);

	BOOL	IsSyonum();
	BOOL	IsEdaban();
	BOOL	IsBumon();
	BOOL	IsKoji();

	BOOL	IsMasterType(DWORD typbit);
	BOOL	IsInsertMaster();
	BOOL	IsConFirmMaster();

	BOOL	tkrec_kanamode();
	void	tkrec_queryAll();

	BOOL	_set_kmketc(struct _SET_KMKETC* psk, BOOL full = TRUE);

	int		Confirm_Mode();
	COLORREF GetConfirmBkColor();
	BOOL	CheckConfirmBkColor(COLORREF back);
	BOOL	IsModifyConfirmData();
	BOOL	IsJozuEnvCloudMaster();

	LPCTSTR get_hjiyutitle(int strtype);
	int		tline_chk(CDBipDataRec *drec);
	int		tline_chk(CDBINPDataRec *drec);
	BOOL	bmncode_ok(int bmn);

	int		GetLastDataSeq();
	int		GetKariAutoBrnCnt();
	int		BumonZanTorok(int bmn, CString  kmcod);
	int		DB_DataAppend(CDBipDataRec* data, int bseq);
	int		DB_DataAppend(CDBINPDataRec* data, int bseq);

	void	GetInpDate(BYTE* bcddate);
	void	SetInpDate(int ddate, int ksign);

	int		GetInputStrLen(unsigned char *buf);
	BOOL	isdiff_data(CDBipDataRec *newdata, CDBipDataRec *moto);
	BOOL	isdiff_data(CDBINPDataRec *newdata, CDBINPDataRec *moto);

	int		CalqSyogTotal(LPCTSTR syogCode, char *tsgd, char *tsgc, char *zan);
	int		memzrec_get(LPCTSTR code, struct _DBKZREC* zrec);
	int		dbzrec_get(LPCTSTR code, struct _DBKZREC* zrec);
	int		zrecget(LPCTSTR code, struct _DBKZREC* zrec);

	int		DateConv(BYTE* bcddate, CDBipDataRec* d_date, int sgn);
	int		DateConv(BYTE* bcddate, CDBINPDataRec* d_date, int sgn);
	int		DateConv(BYTE* bcddate, int* date, int sgn);
	int		BcdDateToYmd(BYTE* bcddate, int* ymd);

	BOOL	isSyzdiff_data(CDBipDataRec *newdata, CDBipDataRec *moto);
	BOOL	isSyzdiff_data(CDBINPDataRec *newdata, CDBINPDataRec *moto);

	BOOL	is_syohizeisiwake(CDBDATA* dbdata);
	BOOL	is_syohizeisiwake(CDBipDataRec* data);
	BOOL	is_syohizeisiwake(CDBINPDataRec* data);
	BOOL	IsDiffEdabanSyzSgn(char* kmkcd, int brn);

	BOOL	isnot_defzeigaku(CDBDATA* dbdata, COLORREF& back);
	BOOL	isnot_defzeigaku(CDBipDataRec* data, COLORREF& back);
	BOOL	isnot_defzeigaku(CDBINPDataRec* data, COLORREF& back);

	BOOL	is_yusyutsusiwake(CDBDATA* dbdata);
	BOOL	is_yusyutsusiwake(CDBipDataRec* data);
	BOOL	is_yusyutsusiwake(CDBINPDataRec* data);

	int		IsKaniOrKobetsu();
	int		IsKani();
	int		IsMenzei();
	int		IsKobetsu();
	bool	CheckTkrec(_SY_TKREC_ *syrec);
	BOOL	is_karibarai_menzei(BYTE* dsign);

	struct _DBKNREC* DB_PjisToKnrec(CString pjiscode, BOOL bALL=FALSE);
	struct _DBKNREC* DB_PjisToKnrec(DWORD dwPjis, BOOL bALL=FALSE);

	void	KamokuString(DBKNREC* pKn, CString& string);
	void	KamokuString(DBKNREC* pKn, char* buf, int bufsize);

	void	get_datelimit(int& sofs, int& eofs, int& sy, int& ey);
	void	set_datelimit(int sofs, int eofs, int sy, int ey);
	BOOL	check_datelimit(BYTE* bcddate);
	void	get_ofsdate(int ofs, int sgn, BYTE* bcddate);
	void	get_ofsdate(int ofs, BYTE* bcddate);

	int		inpdate_shimechk(BYTE* orgdate, char* check);
	int		Myvd_chek(BYTE* date);
	int		Myvd_sign(BYTE* date, BYTE* dst_date);

	int		IsKobetsuBmnSyz();
	int		KobetsuSiwakeCheck(CDBipDataRec* rec, int bmn);
	int		KobetsuSiwakeCheck(CDBINPDataRec* rec, int bmn);

	int		get_brtek(int bmn, CString& code, int brn);
	int		SyzeiMasterType();

	void	get_hjiyustr(BYTE h_code, CString& str);
	BOOL	GetHjiyuEtcStr(int hjcd, int nbcd, CString& str);
	BOOL	_set_kojietc(struct _SET_KMKETC *sk);

	struct _AUTOSEL *GetAUTOSEL();
	int		km_siire(DBKNREC* pKn);
	// 外税・免税仕訳か？
	BOOL	is_sotomensiwake(CDBDATA* dbdata);
	BOOL	is_sotomensiwake(CDBipDataRec* data);
	BOOL	is_sotomensiwake(CDBINPDataRec* data);

	BOOL	IsKaribarai(DWORD kncd);
	BOOL	IsKariuke(DWORD kncd);

private:
	class CSWKDSubW *psub;

	char	rsv[256];
};
