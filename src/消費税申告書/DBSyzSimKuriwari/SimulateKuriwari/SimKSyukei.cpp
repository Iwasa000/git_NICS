#include "stdafx.h"
#include "SimKSyukei.h"

//è¡îÔê≈èàóùÉOÉãÅ[ÉvãÊï™
#define KAMOKUGROUP_SISAN       0x01 //éëéY
#define KAMOKUGROUP_URIAGE      0x02 //îÑè„
#define KAMOKUGROUP_SIRE        0x04 //édì¸
#define KAMOKUGROUP_KEIHI       0x08 //åoîÔ
#define KAMOKUGROUP_TAIKASISAN  0x11 //ëŒâø&éëéY

#define KAMOKUMEISAI_SISAN_SYUTOKU 0x00
#define KAMOKUMEISAI_SISAN_JOTO    0x01

#define KAMOKUMEISAI_URIAGE_URIAGE                 0x00
#define KAMOKUMEISAI_URIAGE_URIAGEHENPIN           0x01
#define KAMOKUMEISAI_URIAGE_EIGYOGAISYUEKI         0x02
#define KAMOKUMEISAI_URIAGE_TOKUBETSURIEKI         0x03
#define KAMOKUMEISAI_URIAGE_UKETORIRISOKU          0x04
#define KAMOKUMEISAI_URIAGE_KASHIDAOREKAISYU       0x05
#define KAMOKUMEISAI_URIAGE_HANBAISYOREIKINHIYO    0x06
#define KAMOKUMEISAI_URIAGE_URIAGEWARIBIKI         0x07
#define KAMOKUMEISAI_URIAGE_HIKAZEIYUSYUTSUURIAGE  0x08
#define KAMOKUMEISAI_URIAGE_YUSYUTUMENZEIURIAGE    0x09
#define KAMOKUMEISAI_URIAGE_KARIUKESYOUHIZEISEISAN 0x0A

#define KAMOKUMEISAI_SIRE_SIRE                     0x00
#define KAMOKUMEISAI_SIRE_SIREHENPIN               0x01
#define KAMOKUMEISAI_SIRE_HANBAISYOIREIKINSYUNYU   0x02
#define KAMOKUMEISAI_SIRE_SIREWARIBIKI             0x03
#define KAMOKUMEISAI_SIRE_YUNYUSIRE                0x04
#define KAMOKUMEISAI_SIRE_TANAOROSHIKASAN          0x05
#define KAMOKUMEISAI_SIRE_TANAOROSHIGENSAN         0x06
#define KAMOKUMEISAI_SIRE_KARIBARAISYOUHIZEISEISAN 0x07

#define KAMOKUMEISAI_TAIKASISAN_SYUTOKUJOTO    0x02
#define KAMOKUMEISAI_TAIKASISAN_YUKASYOKENJOTO 0x03
#define KAMOKUMEISAI_TAIKASISAN_TOTIJOTO       0x04

#define KAMOKUMEISAI_KEIHI_KEIHI             0x00
#define KAMOKUMEISAI_KEIHI_EIGYOGAIHIYO      0x01
#define KAMOKUMEISAI_KEIHI_TOKUBETSUSONSITSU 0x02
#define KAMOKUMEISAI_KEIHI_KASIDAORESONSITSU 0x03

#define SISAN_SYUTOKU      0x00
#define SISAN_JOTO         0x20

#define ZEIKUBUN_SOTOZEI   0x00
#define ZEIKUBUN_UTIZEI    0x10
#define ZEIKUBUN_BEKKI     0x20

#define ZEIRITSU_8         0x03
#define ZEIRITSU_8KEIGEN   0x04
#define ZEIRITSU_10        0x05

/* è¡îÔê≈édñÛãÊï™ */
typedef enum {

	ID_SKA_URIAGE = 1,                    //  1: îÑÅ@Å@Å@Å@Å@è„
	ID_SKA_KASHIDAORESON = 2,             //  2: ë›Å@ì|Å@ëπ  é∏
	ID_SKA_KASHIDAOREKAISYU = 3,          //  3: ë›Å@ì|Å@âÒÅ@é˚
	ID_SKA_YUNYUSIRE = 4,                 //  4: óAÅ@ì¸Å@édÅ@ì¸
	ID_SKA_HIKAZEIYUSYUTSUURIAGE = 5,     //  5: îÒâ€ê≈óAèoîÑè„  
	ID_SKA_HANBAISYOUREIKINNYU = 6,       //  6: îÃîÑèßó„ã‡é˚ì¸
	ID_SKA_HANBAISYOUREIKINHIYO = 7,      //  7: îÃîÑèßó„ã‡îÔóp  
	ID_SKA_SHIIREWARIBIKI = 8,            //  8: édÅ@ì¸Å@äÑÅ@à¯
	ID_SKA_URIAGEWARIBIKI = 9,            //  9: îÑÅ@è„Å@äÑÅ@à¯  
	ID_SKA_TANAKASAN = 10,                // 10: íIâµí≤êÆâ¡éZäz
	ID_SKA_TANAGENSAN = 11,               // 11: íIâµí≤êÆå∏éZäz
	ID_SKA_YUSYUTSUMENZEIURIAGE = 13,     // 13: óAèoÅ@ñ∆ê≈îÑè„ 
    ID_SKA_URIAGENEBIKIMODORI = 17,       // 17: îÑè„ílà¯ÅEñﬂÇË
	ID_SKA_UKETORIRISOKU = 20,            // 20: éÛÅ@éÊÅ@óòÅ@ëß
	ID_SKA_SHIIRE = 21,                   // 21: édÅ@Å@Å@Å@Å@ì¸
	ID_SKA_KEIHI = 23,                    // 23: åoÅ@Å@Å@Å@Å@îÔ
	ID_SKA_SYOUHIZEISEISAN = 28,          // 28: è¡ îÔ ê≈ ê¥ éZ
	ID_SKA_TKKZ_SHIIRE_SUB = 29,          // 29: ì¡íËâ€ê≈édì¸Åiñ{ëÃÅj
	ID_SKA_TKKZ_SHIIREHEN_SUB = 30,       // 30: ì¡íËâ€ê≈édì¸ÇÃï‘ä“Åiñ{ëÃÅj
	ID_SKA_YUNYUSHIIRE_KOKUZEI = 32,      // 32: óAì¸édì¸Åiçëê≈)
	ID_SKA_YUNYUSHIIRE_TIHOUZEI = 33,     // 33: óAì¸édì¸Åiínï˚ê≈Åj
	ID_SKA_YUNYUSHIIRE_KOKUTIGASSAN = 34, // 34: óAì¸édì¸ÅiçëínçáéZÅj

} EnumIdSiwakeKubun;

CSimKSyukei::CSimKSyukei()
{
}

CSimKSyukei::~CSimKSyukei()
{
}

void CSimKSyukei::Init()
{
	m_ArithEx.l_defn(0x16);

    BOOL bOpenFlg = FALSE;
    if( m_pZmSub != NULL ) {
        if( m_pZmSub->zvol == NULL) {
            m_pZmSub->VolumeOpen();
            bOpenFlg = TRUE;
		}
	    m_kazeihousiki = m_SyzUtil.GetKazeihoushiki(m_pZmSub->zvol);
	}

	CString	filter;
	filter.Format(_T("Sn_SEQ = 0"));

	if (m_pZmSub->szsnk) {
		m_pZmSub->szsnk->Requery(filter);
	}
	else {
		if (m_pZmSub->SzsnkOpen(filter)) {
			return;
		}
	}
	m_kwariai95oversw = !(m_pZmSub->szsnk->Sn_UPERS);

    if( bOpenFlg ) {
        m_pZmSub->VolumeClose();
	}

}

bool CSimKSyukei::CheckJigyokubunSettei(int idx_jigyokubun)
{
    BOOL rt = false;

	BOOL bOpenFlg = FALSE;
	if (m_pZmSub != NULL) {
		if (m_pZmSub->zvol == NULL) {
			m_pZmSub->VolumeOpen();
			bOpenFlg = TRUE;
		}
	}

    if ( (m_pZmSub->zvol->s_sgn2&0x80) && (m_pZmSub->zvol->s_sgn3&0xf8) ) {
		//ï°êîã∆éÌ
        switch( idx_jigyokubun ) {
            case ID_JIGYOKUBUN_OROSHI:
                if( m_pZmSub->zvol->s_sgn3&0x10 ) {
					rt = true;
				}
				break;
			case ID_JIGYOKUBUN_KOURI:
				if (m_pZmSub->zvol->s_sgn3 & 0x20) {
					rt = true;
				}
				break;
            case ID_JIGYOKUBUN_SEIZO:
				if (m_pZmSub->zvol->s_sgn3 & 0x40) {
					rt = true;
				}
				break;
			case ID_JIGYOKUBUN_SONOTA:
				rt = true;
				break;
			case ID_JIGYOKUBUN_SERVICE:
				if (m_pZmSub->zvol->s_sgn3 & 0x80) {
					rt = true;
				}
				break;
			case ID_JIGYOKUBUN_HUDOSAN:
				if (m_pZmSub->zvol->s_sgn3 & 0x08) {
					rt = true;
				}
				break;
		}
	}
    else {
		//íPàÍã∆éÌ  
        rt = true;
	}

	if (bOpenFlg) {
		m_pZmSub->VolumeClose();
	}

    return rt;
}

void CSimKSyukei::GetDbData(long sddate, long eddate, bool bSettingEigyobi)
{

    for(int jigyokubun = 0; jigyokubun < JIGYOKUBUN_MAX; jigyokubun++){
        if( CheckJigyokubunSettei(jigyokubun) == false) {
            continue;
		}
        GetDbDataSub(sddate, eddate, jigyokubun, bSettingEigyobi);
	}

	return;
}

void CSimKSyukei::GetDbDataSub(long sddate, long eddate, int jigyokubun, bool bSettingEigyobi)
{

	m_SiwakeRecList[jigyokubun].clear();

	CRecordset mstDbrec(m_pZmSub->m_database);
	CString getVal;

	SiwakeRec Rec = {0};

	CString sq, subsq;
	sq.Empty();
	subsq.Empty();

	if ((m_kazeihousiki == ID_ICSSH_GENSOKU) || (m_kazeihousiki == ID_ICSSH_MENZEI)) {
		sq += "SELECT T1.ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, val, zei, taika, zeikubun, zeiritsu, syutokujoto, jigyokubun, siwakekubun ";
		sq += "FROM  simku_eigyobi AS T1 ";
		sq += "LEFT OUTER JOIN #datarec_SiwakeKubunAriNasi AS T2 ";
		sq += "ON (T1.ddate = T2.ddate) ";
		if (bSettingEigyobi) {
			subsq.Format(_T("WHERE T1.hieigyobi = 0 AND (jigyokubun = %d OR jigyokubun IS NULL) "), jigyokubun);
		}
		else {
			subsq.Format(_T("WHERE (jigyokubun = %d OR jigyokubun IS NULL) "), jigyokubun);
		}
		sq += subsq;
		subsq.Empty();
		sq += "ORDER BY T1.ddate ";

	}
	else if (m_kazeihousiki == ID_ICSSH_KANNI) {

		sq += "IF OBJECT_ID('tempdb..#datarec_SiwakeKubunAriNasiTmp', 'u') IS NOT NULL DROP TABLE #datarec_SiwakeKubunAriNasiTmp ";
		sq += "SELECT * INTO #datarec_SiwakeKubunAriNasiTmp FROM tempdb..#datarec_SiwakeKubunAriNasi ";
		subsq.Format(_T("DELETE FROM #datarec_SiwakeKubunAriNasiTmp WHERE jigyokubun <> %d "), jigyokubun);
		sq += subsq;
		m_pZmSub->m_database->ExecuteSQL(sq);

		sq.Empty();
		subsq.Empty();

		sq += "SELECT T1.ddate, dbt, dbtgroup, dbtgroupmeisai, cre, cregroup, cregroupmeisai, val, zei, taika, zeikubun, zeiritsu, syutokujoto, jigyokubun, siwakekubun ";
		sq += "FROM  simku_eigyobi AS T1 ";
		sq += "LEFT OUTER JOIN #datarec_SiwakeKubunAriNasiTmp AS T2 ";
		sq += "ON (T1.ddate = T2.ddate) ";
		if (bSettingEigyobi) {
			subsq.Format(_T("WHERE T1.hieigyobi = 0"));
		}
		sq += subsq;
		subsq.Empty();
		sq += "ORDER BY T1.ddate ";

	}

	try {
		mstDbrec.Open(CRecordset::forwardOnly, sq);
		for (int i = 0;; i++) {
			if (i != 0) {
				mstDbrec.MoveNext();
			}
			if (mstDbrec.IsEOF()) {
				break;
			}
			for (int i = 0; i < mstDbrec.m_nResultCols; i++) {
				mstDbrec.GetFieldValue(i, getVal);
				switch (i) {
				case 0:
					Rec.ddate = atol(getVal.Trim());
					break;
				case 1:
					Rec.dbt = getVal.Trim();
					break;
				case 2:
					Rec.dbtgroup = atoi(getVal.Trim());
					break;
				case 3:
					Rec.dbtgroupmeisai = atoi(getVal.Trim());
					break;
				case 4:
					Rec.cre = getVal.Trim();
					break;
				case 5:
					Rec.cregroup = atoi(getVal.Trim());
					break;
				case 6:
					Rec.cregroupmeisai = atoi(getVal.Trim());
					break;
				case 7:
					Rec.val = getVal.Trim();
					break;
				case 8:
					Rec.zei = getVal.Trim();
					break;
				case 9:
					Rec.taika = getVal.Trim();
					break;
				case 10:
					Rec.zeikubun = atoi(getVal.Trim());
					break;
				case 11:
					Rec.zeiritsu = atoi(getVal.Trim());
					break;
				case 12:
					Rec.syutokujoto = atoi(getVal.Trim()); 
					break;
				case 13:
					Rec.jigyokubun = jigyokubun;
					break;
				case 14:
					Rec.siwakekubun = atoi(getVal.Trim());
					break;
				default:
					break;
				}
			}

			if ((Rec.ddate < TOKUREI_KIKAN_S) || (Rec.ddate > TOKUREI_KIKAN_E)) {
				continue;
			}

            if( sddate && eddate ) {
				if ((Rec.ddate < sddate) || (Rec.ddate > eddate)) {
					continue;
				}
			}

   //         if( Rec.jigyokubun == 0 ) {
			//	DbgViewTrace(_T("ÅyDEBUGÅz%d %s [%02d] Rec.ddate:%d Rec.jigyokubun:%d dbt:%s dbtgroup:0x%02x dbtgroupmeisai:0x%02x cre:%s cregroup:0x%02x cregroupmeisai:0x%02x val:%s zei:%s\n"),
   //                 __LINE__, __FUNCTION__, 
			//		jigyokubun,
			//		Rec.ddate,
			//		Rec.jigyokubun,
			//		Rec.dbt,
			//		Rec.dbtgroup,
			//		Rec.dbtgroupmeisai,
			//		Rec.cre,
			//		Rec.cregroup,
			//		Rec.cregroupmeisai,
			//		Rec.val,
			//		Rec.zei
			//	);
			//}

			m_SiwakeRecList[jigyokubun].push_back(Rec);
		}

	}
	catch (CDBException* dbe) {
		dbe->Delete();
		return;
	}

	mstDbrec.Close();

}

void CSimKSyukei::SyukeiDay()
{

#ifdef _DEBUG
	//DbgViewTrace(_T("ÅyDEBUGÅz"));
	//DbgViewTrace(_T(__FUNCTION__));
	//DbgViewTrace(_T("\n"));
#endif

	for (int i = 0; i < JIGYOKUBUN_MAX; i++) {
		m_DayRecList[i].clear();
	}

	DayRec Rec[JIGYOKUBUN_MAX];
	memset(&Rec, 0, sizeof(DayRec) * JIGYOKUBUN_MAX);

	int seq[JIGYOKUBUN_MAX] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };

	long prev_ddate = 0;
	char prev_jigyokubun = 0;

	EnumIdSyukeiKubun syukeikubun;

    for(int jigyokubun = 0; jigyokubun < JIGYOKUBUN_MAX; jigyokubun++){

		for (std::vector<SiwakeRec>::iterator itr = m_SiwakeRecList[jigyokubun].begin(); itr != m_SiwakeRecList[jigyokubun].end(); itr++) {

			if (prev_ddate == itr->ddate) {
				continue;
			}

			//édñÛÉfÅ[É^Çâcã∆ì˙, ãÊï™ñàÇ…èWåv.
			Rec[itr->jigyokubun].seq = seq[itr->jigyokubun]++;
			Rec[itr->jigyokubun].ddate = itr->ddate;

			for (std::vector<SiwakeRec>::iterator subitr = m_SiwakeRecList[jigyokubun].begin(); subitr != m_SiwakeRecList[jigyokubun].end(); subitr++) {

				if ((subitr->ddate == itr->ddate) && (subitr->jigyokubun == itr->jigyokubun)) {

					if (subitr->val.IsEmpty() && subitr->zei.IsEmpty()) {
						Rec[itr->jigyokubun].IsZero = 1;
					}
					else {
						Rec[itr->jigyokubun].IsZero = 0;
					}

					//éÿï˚â»ñ⁄
					SubSyukeiDayGetSyukeiKubun(&syukeikubun, itr, subitr, ID_KARIKATA);
					SubSyukeiDaySetData(&syukeikubun, itr, subitr, &Rec[0], ID_KARIKATA);

					//ë›ï˚â»ñ⁄
					SubSyukeiDayGetSyukeiKubun(&syukeikubun, itr, subitr, ID_KASHIKATA);
					SubSyukeiDaySetData(&syukeikubun, itr, subitr, &Rec[0], ID_KASHIKATA);

				}

			}

			m_DayRecList[itr->jigyokubun].push_back(Rec[itr->jigyokubun]);

#ifdef _DEBUG
			//char str[64], str2[64], str3[64], str4[64], str5[64], str6[64], str7[64], str8[64], str9[64];

   //         if( itr->jigyokubun == 0 ) {

			//	DbgViewTrace(_T("ÅyDEBUGÅz[%02d] seq = %d ddate = %ld jigyokubun = %d isZero = %d "),
			//		itr->jigyokubun, Rec[itr->jigyokubun].seq, Rec[itr->jigyokubun].ddate, itr->jigyokubun, Rec[itr->jigyokubun].IsZero);

			//	DbgViewTrace(_T("keigenuriage = %s uriage = %s keigensire = %s sire = %s tksire10 = %s tksire8 = %s kamotsusire = %s kariuke = %s keigenkariuke = %s\n"),
			//		m_ArithEx.l_print(str, &Rec[itr->jigyokubun].keigenuriage[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str2, &Rec[itr->jigyokubun].uriage[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str3, &Rec[itr->jigyokubun].keigensire[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str4, &Rec[itr->jigyokubun].sire[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str5, &Rec[itr->jigyokubun].tksire10[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str6, &Rec[itr->jigyokubun].tksire8[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str7, &Rec[itr->jigyokubun].kamotsusire[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str8, &Rec[itr->jigyokubun].kariuke[0], _T("s,sss,sss,sss,ss9")),
			//		m_ArithEx.l_print(str9, &Rec[itr->jigyokubun].keigenkariuke[0], _T("s,sss,sss,sss,ss9"))
			//	);
			//}

#endif

			memset(&Rec, 0, sizeof(DayRec) * JIGYOKUBUN_MAX);
			prev_ddate = itr->ddate;
			prev_jigyokubun = itr->jigyokubun;
		}

	}

	return;
}

//èWåvÉfÅ[É^ãÊï™êUÇËï™ÇØ
void CSimKSyukei::SubSyukeiDayGetSyukeiKubun(EnumIdSyukeiKubun* pSyukeiKubun,
	std::vector<SiwakeRec>::iterator itr,
	std::vector<SiwakeRec>::iterator subitr,
	EnumIdKariKashiKbn type)
{

	*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;

	int group, groupmeisai;
	CString kncod;
	if (type == ID_KASHIKATA) {
		group = subitr->dbtgroup;
		groupmeisai = subitr->dbtgroupmeisai;
		kncod = subitr->dbt;
	}
	else {
		group = subitr->cregroup;
		groupmeisai = subitr->cregroupmeisai;
		kncod = subitr->cre;
	}

	if (kncod.Compare(KaribaraiSyohizei) == 0) {
		*pSyukeiKubun = ID_SYUKEIKUBUN_KARIBARAISYOHIZEI;
	}
	else if (kncod.Compare(KariukeSyohizei) == 0) {
		*pSyukeiKubun = ID_SYUKEIKUBUN_KARIUKESYOHIZEI;
	}

    if( subitr->siwakekubun ) {
	    //édñÛãÊï™Ç†ÇË.
		switch (subitr->siwakekubun) {

		    case ID_SKA_URIAGE:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
			        *pSyukeiKubun = ID_SYUKEIKUBUN_URIAGE;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;

		    case ID_SKA_YUNYUSIRE:
		    case ID_SKA_YUNYUSHIIRE_KOKUZEI:
		    case ID_SKA_YUNYUSHIIRE_TIHOUZEI:
		    case ID_SKA_YUNYUSHIIRE_KOKUTIGASSAN:
				if (group 
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI 
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_KAMOTSU_SHIIRE;
				}
                else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
			    break;

		    case ID_SKA_TKKZ_SHIIRE_SUB:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
			        *pSyukeiKubun = ID_SYUKEIKUBUN_TOKUTEI_SHIIRE;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;

			case ID_SKA_TKKZ_SHIIREHEN_SUB:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_TOKUTEI_SHIIRE_HENKANN;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;

			case ID_SKA_TANAKASAN:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_TYOUSEI_KASAN_SHIIRE;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;
			case ID_SKA_TANAGENSAN:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_TYOUSEI_GENSAN_SHIIRE;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;

		    case ID_SKA_SHIIREWARIBIKI:
			case ID_SKA_HANBAISYOUREIKINNYU:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE_HENKANN;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;

		    case ID_SKA_SHIIRE:
		    case ID_SKA_KEIHI:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
			        *pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;
			case ID_SKA_HANBAISYOUREIKINHIYO:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_HANBAISYOREIKINHIYO;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;
			case ID_SKA_URIAGEWARIBIKI:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_URIAGEWARIBIKI;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;
		    case ID_SKA_KASHIDAOREKAISYU:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_KASHIDAOREKAISYU;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;
			case ID_SKA_KASHIDAORESON:
				if (group
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					|| *pSyukeiKubun == ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_KASHIDAORESON;
				}
				else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
				break;
			case ID_SKA_YUSYUTSUMENZEIURIAGE:
		    case ID_SKA_HIKAZEIYUSYUTSUURIAGE:
		    case ID_SKA_UKETORIRISOKU:
		    case ID_SKA_SYOUHIZEISEISAN:
			    *pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
			    break;
		}
	}
    else {
	    //édñÛãÊï™Ç»Çµ.
		switch (group) {
            case KAMOKUGROUP_SISAN:
                switch(groupmeisai){
				    case KAMOKUMEISAI_SISAN_SYUTOKU:
				        *pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE;
                        break;
				    case KAMOKUMEISAI_SISAN_JOTO:
						*pSyukeiKubun = ID_SYUKEIKUBUN_URIAGE;
					    break;
				    default:
						*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
                       break;
				}
                break;
		    case KAMOKUGROUP_URIAGE:
				switch (groupmeisai) {
                   case KAMOKUMEISAI_URIAGE_URIAGE:
                   case KAMOKUMEISAI_URIAGE_EIGYOGAISYUEKI:
				   case KAMOKUMEISAI_URIAGE_TOKUBETSURIEKI:
				   case KAMOKUMEISAI_URIAGE_UKETORIRISOKU:
					   *pSyukeiKubun = ID_SYUKEIKUBUN_URIAGE;
                       break;
				   case KAMOKUMEISAI_URIAGE_HANBAISYOREIKINHIYO:
					   *pSyukeiKubun = ID_SYUKEIKUBUN_HANBAISYOREIKINHIYO;
					   break;
				   case KAMOKUMEISAI_URIAGE_URIAGEWARIBIKI:
				   case KAMOKUMEISAI_URIAGE_URIAGEHENPIN:
					   *pSyukeiKubun = ID_SYUKEIKUBUN_URIAGEWARIBIKI;
					   break;
				   case KAMOKUMEISAI_URIAGE_KASHIDAOREKAISYU:
					   *pSyukeiKubun = ID_SYUKEIKUBUN_KASHIDAOREKAISYU;
					   break;
				   case KAMOKUMEISAI_URIAGE_HIKAZEIYUSYUTSUURIAGE:
					   *pSyukeiKubun = ID_SYUKEIKUBUN_KASHIDAORESON;
					   break;
				   case KAMOKUMEISAI_URIAGE_YUSYUTUMENZEIURIAGE:
				   case KAMOKUMEISAI_URIAGE_KARIUKESYOUHIZEISEISAN:
                   default:
					   *pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
                       break;
				}
			    break;
		    case KAMOKUGROUP_SIRE:
				switch (groupmeisai) {
				    case KAMOKUMEISAI_SIRE_SIRE:
					case KAMOKUMEISAI_SIRE_TANAOROSHIKASAN:
					case KAMOKUMEISAI_SIRE_TANAOROSHIGENSAN:
						*pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE;
				        break;
                    case KAMOKUMEISAI_SIRE_YUNYUSIRE:
						*pSyukeiKubun = ID_SYUKEIKUBUN_KAMOTSU_SHIIRE;
                        break;
					case KAMOKUMEISAI_SIRE_SIREHENPIN:
					case KAMOKUMEISAI_SIRE_SIREWARIBIKI:
					case KAMOKUMEISAI_SIRE_HANBAISYOIREIKINSYUNYU:
						*pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE_HENKANN;
                        break;
                    case KAMOKUMEISAI_SIRE_KARIBARAISYOUHIZEISEISAN:
						*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
                        break;
				}
                break;
			case KAMOKUGROUP_KEIHI:
                if( groupmeisai == KAMOKUMEISAI_KEIHI_KASIDAORESONSITSU ) {
					*pSyukeiKubun = ID_SYUKEIKUBUN_KASHIDAORESON;
				}
                else {
					*pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE;
				}
			    break;
		    case KAMOKUGROUP_TAIKASISAN:
				switch (groupmeisai) {
				    case KAMOKUMEISAI_TAIKASISAN_SYUTOKUJOTO:
						if (subitr->syutokujoto == SISAN_SYUTOKU) {
						    *pSyukeiKubun = ID_SYUKEIKUBUN_SHIIRE;
						}
						else if(subitr->syutokujoto == SISAN_JOTO) {
						    *pSyukeiKubun = ID_SYUKEIKUBUN_URIAGE;
						}
                        break;
                    case KAMOKUMEISAI_TAIKASISAN_YUKASYOKENJOTO:
					case KAMOKUMEISAI_TAIKASISAN_TOTIJOTO:
					default:
					    *pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
					    break;
				}
			    break;
		    default:
				if ( *pSyukeiKubun != ID_SYUKEIKUBUN_KARIBARAISYOHIZEI
					&& *pSyukeiKubun != ID_SYUKEIKUBUN_KARIUKESYOHIZEI) {

					*pSyukeiKubun = ID_SYUKEIKUBUN_NASHI;
				}
			    break;
		}

	}

    return;
}

//èWåvÉfÅ[É^ÉZÉbÉg
void CSimKSyukei::SubSyukeiDaySetData(EnumIdSyukeiKubun* pSyukeiKubun,
                                      std::vector<SiwakeRec>::iterator itr,
                                      std::vector<SiwakeRec>::iterator subitr,
	                                  DayRec* pDayRec,
	                                  EnumIdKariKashiKbn type)
{


	switch( *pSyukeiKubun ) {

		case ID_SYUKEIKUBUN_URIAGE:
			SubSyukeiDayUriageKubun(itr, subitr, pDayRec, type);
			break;
		case ID_SYUKEIKUBUN_KARIUKESYOHIZEI:
			if ( (m_kazeihousiki == ID_ICSSH_GENSOKU) || (m_kazeihousiki == ID_ICSSH_MENZEI)) {
				SubSyukeiDayUriageKubun(itr, subitr, pDayRec, type);
			}
			SubSyukeiDayKariukeKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_SHIIRE:
		case ID_SYUKEIKUBUN_KARIBARAISYOHIZEI:
			SubSyukeiDaySireKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_TOKUTEI_SHIIRE:
			if (m_IsNeedTkkz) {
				SubSyukeiDayTokuteiSireKubun(itr, subitr, pDayRec, type);
			}
			break;

		case ID_SYUKEIKUBUN_TOKUTEI_SHIIRE_HENKANN:
			if (m_IsNeedTkkz) {
				SubSyukeiDayTokuteiSireHenkannKubun(itr, subitr, pDayRec, type);
			}
			break;

		case ID_SYUKEIKUBUN_KAMOTSU_SHIIRE:
			SubSyukeiDayKamotsuSireKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_TYOUSEI_KASAN_SHIIRE:
			SubSyukeiDayTyouseiKasanSireKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_TYOUSEI_GENSAN_SHIIRE:
			SubSyukeiDayTyouseiGensanSireKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_SHIIRE_HENKANN:
			SubSyukeiDaySireHenkannKubun(itr, subitr, pDayRec, type);
            break;

		case ID_SYUKEIKUBUN_HANBAISYOREIKINHIYO:
			SubSyukeiDayHanbaisyoreikinhiyoKubun(itr, subitr, pDayRec, type);
            break;

		case ID_SYUKEIKUBUN_URIAGEWARIBIKI:
			SubSyukeiDayUriageWaribikiKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_KASHIDAOREKAISYU:
			SubSyukeiDayKashidaorekaisyuKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_KASHIDAORESON:
			SubSyukeiDayKashidaoresonKubun(itr, subitr, pDayRec, type);
			break;

		case ID_SYUKEIKUBUN_NASHI:
        default:
            break;
	}

}

void CSimKSyukei::SubSyukeiDayUriageKubun(std::vector<SiwakeRec>::iterator itr,
                                          std::vector<SiwakeRec>::iterator subitr, 
                                          DayRec* pDayRec, 
	                                      EnumIdKariKashiKbn type)
{
	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
    if( subitr->syutokujoto == SISAN_JOTO ) {
		m_ArithEx.l_input(&tmpval[0], subitr->taika.GetBuffer());
	}
    else {
		m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	}
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN || subitr->zeiritsu == ZEIRITSU_10) {

			//â€ê≈îÑè„. (8.0% (åyå∏ê≈ó¶) + 10.0%)
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].uriage[0], &pDayRec[itr->jigyokubun].uriage[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_BEKKI) {
			    m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].uriage_bekki[0], &pDayRec[itr->jigyokubun].uriage_bekki[0], &tmpval[0]);
			}
			else if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].uriage[0], &pDayRec[itr->jigyokubun].uriage[0], &tmpval2[0]);
			}

			//åyå∏ê≈ó¶ëŒè€â€ê≈îÑè„. (8.0% (åyå∏ê≈ó¶))
			if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigenuriage[0], &pDayRec[itr->jigyokubun].keigenuriage[0], &tmpval[0]);
				if (subitr->zeikubun == ZEIKUBUN_BEKKI) {
				    m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigenuriage_bekki[0], &pDayRec[itr->jigyokubun].keigenuriage_bekki[0], &tmpval[0]);
				}
				else if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
					m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigenuriage[0], &pDayRec[itr->jigyokubun].keigenuriage[0], &tmpval2[0]);
				}
			}

		}

	}
	else {

		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN || subitr->zeiritsu == ZEIRITSU_10) {

			//â€ê≈îÑè„. (8.0% (åyå∏ê≈ó¶) + 10.0%)
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].uriage[0], &pDayRec[itr->jigyokubun].uriage[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_BEKKI) {
			    m_ArithEx.l_add(&pDayRec[itr->jigyokubun].uriage_bekki[0], &pDayRec[itr->jigyokubun].uriage_bekki[0], &tmpval[0]);
			}
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].uriage[0], &pDayRec[itr->jigyokubun].uriage[0], &tmpval2[0]);
			}

			//åyå∏ê≈ó¶ëŒè€â€ê≈îÑè„. (8.0% (åyå∏ê≈ó¶))
			if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigenuriage[0], &pDayRec[itr->jigyokubun].keigenuriage[0], &tmpval[0]);
				if (subitr->zeikubun == ZEIKUBUN_BEKKI) {
					m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigenuriage_bekki[0], &pDayRec[itr->jigyokubun].keigenuriage_bekki[0], &tmpval[0]);
				}
				if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
					m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigenuriage[0], &pDayRec[itr->jigyokubun].keigenuriage[0], &tmpval2[0]);
				}
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayKariukeKubun(std::vector<SiwakeRec>::iterator itr,
	                                    std::vector<SiwakeRec>::iterator subitr,
                                        DayRec* pDayRec,
                                        EnumIdKariKashiKbn type)
{
	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN || subitr->zeiritsu == ZEIRITSU_10) {

			// (8.0% (åyå∏ê≈ó¶) + 10.0%)
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kariuke[0], &pDayRec[itr->jigyokubun].kariuke[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kariuke[0], &pDayRec[itr->jigyokubun].kariuke[0], &tmpval2[0]);
			}

			// (8.0% (åyå∏ê≈ó¶))
			if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigenkariuke[0], &pDayRec[itr->jigyokubun].keigenkariuke[0], &tmpval[0]);
				if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
					m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigenkariuke[0], &pDayRec[itr->jigyokubun].keigenkariuke[0], &tmpval2[0]);
				}
			}

		}

	}
	else {

		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN || subitr->zeiritsu == ZEIRITSU_10) {

			// (8.0% (åyå∏ê≈ó¶) + 10.0%)
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kariuke[0], &pDayRec[itr->jigyokubun].kariuke[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kariuke[0], &pDayRec[itr->jigyokubun].kariuke[0], &tmpval2[0]);
			}

			// (8.0% (åyå∏ê≈ó¶))
			if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigenkariuke[0], &pDayRec[itr->jigyokubun].keigenkariuke[0], &tmpval[0]);
				if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
					m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigenkariuke[0], &pDayRec[itr->jigyokubun].keigenkariuke[0], &tmpval2[0]);
				}
			}
		}

	}
}

void CSimKSyukei::SubSyukeiDaySireKubun(std::vector<SiwakeRec>::iterator itr,
                                        std::vector<SiwakeRec>::iterator subitr,
                                        DayRec* pDayRec,
                                        EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//â€ê≈édì¸çáåv. (ãåê≈ó¶ä‹Çﬁ).
		m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sire[0], &pDayRec[itr->jigyokubun].sire[0], &tmpval[0]);
		if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sire[0], &pDayRec[itr->jigyokubun].sire[0], &tmpval2[0]);
		}

		//åyå∏ê≈ó¶ëŒè€â€ê≈édì¸. (8.0% (åyå∏ê≈ó¶))
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval2[0]);
			}
		}

		//ãåê≈ó¶â€ê≈édì¸. 
		if (subitr->zeiritsu != ZEIRITSU_8KEIGEN && subitr->zeiritsu != ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sire358[0], &pDayRec[itr->jigyokubun].sire358[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sire358[0], &pDayRec[itr->jigyokubun].sire358[0], &tmpval2[0]);
			}
		}

	}
    else {

		//â€ê≈édì¸çáåv. (ãåê≈ó¶ä‹Çﬁ).
		m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sire[0], &pDayRec[itr->jigyokubun].sire[0], &tmpval[0]);
		if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sire[0], &pDayRec[itr->jigyokubun].sire[0], &tmpval2[0]);
		}

		//åyå∏ê≈ó¶ëŒè€â€ê≈édì¸. (8.0% (åyå∏ê≈ó¶))
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval2[0]);
			}
		}

		//ãåê≈ó¶â€ê≈édì¸. 
		if (subitr->zeiritsu != ZEIRITSU_8KEIGEN && subitr->zeiritsu != ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sire358[0], &pDayRec[itr->jigyokubun].sire358[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sire358[0], &pDayRec[itr->jigyokubun].sire358[0], &tmpval2[0]);
			}
		}

	}
}

void CSimKSyukei::SubSyukeiDayTokuteiSireKubun(std::vector<SiwakeRec>::iterator itr,
	                                           std::vector<SiwakeRec>::iterator subitr,
	                                           DayRec* pDayRec,
	                                           EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//ì¡íËâ€ê≈édì¸çáåv. 
		if (subitr->zeiritsu == ZEIRITSU_8) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tksire8[0], &pDayRec[itr->jigyokubun].tksire8[0], &tmpval[0]);
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tksire10[0], &pDayRec[itr->jigyokubun].tksire10[0], &tmpval[0]);
		}

		//åyå∏ê≈ó¶ëŒè€â€ê≈édì¸. (8.0% (åyå∏ê≈ó¶))
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval2[0]);
			}
		}
	}
    else {

		//ì¡íËâ€ê≈édì¸çáåv. 
		if (subitr->zeiritsu == ZEIRITSU_8) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tksire8[0], &pDayRec[itr->jigyokubun].tksire8[0], &tmpval[0]);
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tksire10[0], &pDayRec[itr->jigyokubun].tksire10[0], &tmpval[0]);
		}

		//åyå∏ê≈ó¶ëŒè€â€ê≈édì¸. (8.0% (åyå∏ê≈ó¶))
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayTokuteiSireHenkannKubun(std::vector<SiwakeRec>::iterator itr,
                                                      std::vector<SiwakeRec>::iterator subitr,
                                                      DayRec* pDayRec,
                                                      EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//ì¡íËâ€ê≈édì¸çáåv. (ï‘ä“).
		if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tksirehenkann10[0], &pDayRec[itr->jigyokubun].tksirehenkann10[0], &tmpval[0]);
		}

	}
	else {

		//ì¡íËâ€ê≈édì¸çáåv. (ï‘ä“).
		if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tksirehenkann10[0], &pDayRec[itr->jigyokubun].tksirehenkann10[0], &tmpval[0]);
		}

	}

}

void CSimKSyukei::SubSyukeiDayKamotsuSireKubun(std::vector<SiwakeRec>::iterator itr,
	                                           std::vector<SiwakeRec>::iterator subitr,
	                                           DayRec* pDayRec,
	                                           EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//â€ê≈édì¸çáåv. (ãåê≈ó¶ä‹Çﬁ).
		m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kamotsusire[0], &pDayRec[itr->jigyokubun].kamotsusire[0], &tmpval[0]);
		if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kamotsusire[0], &pDayRec[itr->jigyokubun].kamotsusire[0], &tmpval2[0]);
		}

		//åyå∏ê≈ó¶ëŒè€â€ê≈édì¸. (8.0% (åyå∏ê≈ó¶))
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval2[0]);
			}
		}

		//ãåê≈ó¶â€ê≈édì¸. 
		if (subitr->zeiritsu != ZEIRITSU_8KEIGEN && subitr->zeiritsu != ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kamotsusire358[0], &pDayRec[itr->jigyokubun].kamotsusire358[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kamotsusire358[0], &pDayRec[itr->jigyokubun].kamotsusire358[0], &tmpval2[0]);
			}
		}

	}
	else {

		//â€ê≈édì¸çáåv. (ãåê≈ó¶ä‹Çﬁ).
		m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kamotsusire[0], &pDayRec[itr->jigyokubun].kamotsusire[0], &tmpval[0]);
		if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kamotsusire[0], &pDayRec[itr->jigyokubun].kamotsusire[0], &tmpval2[0]);
		}

		//åyå∏ê≈ó¶ëŒè€â€ê≈édì¸. (8.0% (åyå∏ê≈ó¶))
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].keigensire[0], &pDayRec[itr->jigyokubun].keigensire[0], &tmpval2[0]);
			}
		}

		//ãåê≈ó¶â€ê≈édì¸. 
		if (subitr->zeiritsu != ZEIRITSU_8KEIGEN && subitr->zeiritsu != ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kamotsusire358[0], &pDayRec[itr->jigyokubun].kamotsusire358[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kamotsusire358[0], &pDayRec[itr->jigyokubun].kamotsusire358[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayTyouseiKasanSireKubun(std::vector<SiwakeRec>::iterator itr,
	                                                std::vector<SiwakeRec>::iterator subitr,
	                                                DayRec* pDayRec,
	                                                EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//íIâµí≤êÆäz.
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &tmpval2[0]);
			}
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &tmpval2[0]);
			}
		}

	}
	else {

		//íIâµí≤êÆäz.
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &pDayRec[itr->jigyokubun].tanaoroshikasankei8[0], &tmpval2[0]);
			}
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
			    m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &pDayRec[itr->jigyokubun].tanaoroshikasan10[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayTyouseiGensanSireKubun(std::vector<SiwakeRec>::iterator itr,
	                                                 std::vector<SiwakeRec>::iterator subitr,
                                                     DayRec* pDayRec,
                                                     EnumIdKariKashiKbn type)
{
	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//íIâµí≤êÆäz.
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &tmpval2[0]);
			}
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &tmpval2[0]);
			}
		}

	}
	else {

		//íIâµí≤êÆäz.
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &pDayRec[itr->jigyokubun].tanaoroshigensankei8[0], &tmpval2[0]);
			}
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &pDayRec[itr->jigyokubun].tanaoroshigensan10[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDaySireHenkannKubun(std::vector<SiwakeRec>::iterator itr,
                                               std::vector<SiwakeRec>::iterator subitr,
                                               DayRec* pDayRec,
                                               EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		//â€ê≈édì¸çáåv. (ãåê≈ó¶ä‹Çﬁ).ï‘ä“.
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sirehenkannkei8[0], &pDayRec[itr->jigyokubun].sirehenkannkei8[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sirehenkannkei8[0], &pDayRec[itr->jigyokubun].sirehenkannkei8[0], &tmpval2[0]);
			}
		}
		else if (subitr->zeiritsu == ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sirehenkann10[0], &pDayRec[itr->jigyokubun].sirehenkann10[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sirehenkann10[0], &pDayRec[itr->jigyokubun].sirehenkann10[0], &tmpval2[0]);
			}
		}

		//ãåê≈ó¶â€ê≈édì¸. 
		if (subitr->zeiritsu != ZEIRITSU_8KEIGEN && subitr->zeiritsu != ZEIRITSU_10) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sirehenkann358[0], &pDayRec[itr->jigyokubun].sirehenkann358[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].sirehenkann358[0], &pDayRec[itr->jigyokubun].sirehenkann358[0], &tmpval2[0]);
			}
		}

	}
	else {

		//â€ê≈édì¸çáåv. (ãåê≈ó¶ä‹Çﬁ).ï‘ä“.
		if (subitr->zeiritsu == ZEIRITSU_8KEIGEN) {

			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sirehenkannkei8[0], &pDayRec[itr->jigyokubun].sirehenkannkei8[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sirehenkannkei8[0], &pDayRec[itr->jigyokubun].sirehenkannkei8[0], &tmpval2[0]);
			}

		} else if (subitr->zeiritsu == ZEIRITSU_10) {

			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sirehenkann10[0], &pDayRec[itr->jigyokubun].sirehenkann10[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sirehenkann10[0], &pDayRec[itr->jigyokubun].sirehenkann10[0], &tmpval2[0]);
			}

		}

		//ãåê≈ó¶â€ê≈édì¸. 
		if (subitr->zeiritsu != ZEIRITSU_8KEIGEN && subitr->zeiritsu != ZEIRITSU_10) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sirehenkann358[0], &pDayRec[itr->jigyokubun].sirehenkann358[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].sirehenkann358[0], &pDayRec[itr->jigyokubun].sirehenkann358[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayHanbaisyoreikinhiyoKubun(std::vector<SiwakeRec>::iterator itr,
                                               std::vector<SiwakeRec>::iterator subitr,
                                               DayRec* pDayRec,
                                               EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &tmpval2[0]);
			}

		}

	}
	else {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &pDayRec[itr->jigyokubun].hanbaisyoreikinhiyo[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayUriageWaribikiKubun(std::vector<SiwakeRec>::iterator itr,
                                                  std::vector<SiwakeRec>::iterator subitr,
                                                  DayRec* pDayRec,
                                                  EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].uriagewaribiki[0], &pDayRec[itr->jigyokubun].uriagewaribiki[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].uriagewaribiki[0], &pDayRec[itr->jigyokubun].uriagewaribiki[0], &tmpval2[0]);
			}
		}

	}
	else {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].uriagewaribiki[0], &pDayRec[itr->jigyokubun].uriagewaribiki[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].uriagewaribiki[0], &pDayRec[itr->jigyokubun].uriagewaribiki[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayKashidaorekaisyuKubun(std::vector<SiwakeRec>::iterator itr,
                                                 	std::vector<SiwakeRec>::iterator subitr,
                                                 	DayRec* pDayRec,
                                                	EnumIdKariKashiKbn type)
{

	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &tmpval2[0]);
			}
		}

	}
	else {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &pDayRec[itr->jigyokubun].kashidaorekaisyu[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SubSyukeiDayKashidaoresonKubun(std::vector<SiwakeRec>::iterator itr,
	                                             std::vector<SiwakeRec>::iterator subitr,
                                                 DayRec* pDayRec,
                                                 EnumIdKariKashiKbn type)
{
	char tmpval[6], tmpval2[6];
	memset(&tmpval[0], 0, sizeof(tmpval));
	memset(&tmpval2[0], 0, sizeof(tmpval2));
	m_ArithEx.l_input(&tmpval[0], subitr->val.GetBuffer());
	m_ArithEx.l_input(&tmpval2[0], subitr->zei.GetBuffer());

	if (type == ID_KASHIKATA) {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kashidaoreson[0], &pDayRec[itr->jigyokubun].kashidaoreson[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_add(&pDayRec[itr->jigyokubun].kashidaoreson[0], &pDayRec[itr->jigyokubun].kashidaoreson[0], &tmpval2[0]);
			}
		}

	}
	else {

		if ((subitr->zeiritsu == ZEIRITSU_8KEIGEN) || (subitr->zeiritsu == ZEIRITSU_10)) {
			m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kashidaoreson[0], &pDayRec[itr->jigyokubun].kashidaoreson[0], &tmpval[0]);
			if (subitr->zeikubun == ZEIKUBUN_SOTOZEI) {
				m_ArithEx.l_sub(&pDayRec[itr->jigyokubun].kashidaoreson[0], &pDayRec[itr->jigyokubun].kashidaoreson[0], &tmpval2[0]);
			}
		}

	}

}

void CSimKSyukei::SyukeiRenzoku10Eigyobi()
{

#ifdef _DEBUG
	//DbgViewTrace(_T("\n"));
	//DbgViewTrace(_T("ÅyDEBUGÅz"));
	//DbgViewTrace(_T(__FUNCTION__));
	//DbgViewTrace(_T("\n"));
#endif

	for (int i = 0; i < JIGYOKUBUN_MAX; i++) {
		m_TenDayRecList[i].clear();
	}

	TenDayRec Rec[JIGYOKUBUN_MAX];
	memset(&Rec, 0, sizeof(TenDayRec) * JIGYOKUBUN_MAX);

	char tmpvalstr[64];
	CString tmpstr;

	//m_DayRecListÇ©ÇÁ10âcã∆ì˙ñàÇ…â€ê≈îÑè„,â€ê≈îÑè„(åyå∏ê≈ó¶Çâ¡éZ)ÇèWåv.
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {

		for (std::vector<DayRec>::iterator itr = m_DayRecList[idx_jigyokbn].begin(); itr != m_DayRecList[idx_jigyokbn].end(); itr++) {

			//10âcã∆ì˙íPà ÇÃÉäÉXÉgÇçÏê¨.
			Rec[idx_jigyokbn].sseq = itr->seq;
			Rec[idx_jigyokbn].sddate = itr->ddate;

			for (std::vector<DayRec>::iterator subitr = m_DayRecList[idx_jigyokbn].begin(); subitr != m_DayRecList[idx_jigyokbn].end(); subitr++) {

				//äeâcã∆ì˙Ç©ÇÁ10âcã∆ì˙êÊÇ‹Ç≈.
				if ((itr->seq <= subitr->seq) && (subitr->seq <= itr->seq + 10 - 1)) {

					Rec[idx_jigyokbn].eseq = subitr->seq;
					Rec[idx_jigyokbn].eddate = subitr->ddate;

					if (!subitr->IsZero) {
						m_ArithEx.l_add(&Rec[idx_jigyokbn].keigenuriage[0], &Rec[idx_jigyokbn].keigenuriage[0], subitr->keigenuriage);
						m_ArithEx.l_add(&Rec[idx_jigyokbn].uriage[0], &Rec[idx_jigyokbn].uriage[0], subitr->uriage);
						m_ArithEx.l_add(&Rec[idx_jigyokbn].keigenuriage_bekki[0], &Rec[idx_jigyokbn].keigenuriage_bekki[0], subitr->keigenuriage_bekki);
						m_ArithEx.l_add(&Rec[idx_jigyokbn].uriage_bekki[0], &Rec[idx_jigyokbn].uriage_bekki[0], subitr->uriage_bekki);
					}

				}

			}

			//åyå∏îÑè„äÑçáÇåvéZ.
			if (!Rec[idx_jigyokbn].IsZero) {
				m_ArithEx.l_par100(&Rec[idx_jigyokbn].keigenwariai[0], &Rec[idx_jigyokbn].keigenuriage[0], &Rec[idx_jigyokbn].uriage[0], 2, 0);
			}
			else {
				memset(&Rec[idx_jigyokbn].keigenwariai[0], '\0', sizeof(Rec[idx_jigyokbn].keigenwariai));
			}

			m_ArithEx.l_print(&tmpvalstr[0], &Rec[idx_jigyokbn].keigenwariai[0], _T("ssss9"));
			tmpstr.Format(_T("%s"), tmpvalstr);
			Rec[idx_jigyokbn].keigenwariai_f = _ttof(tmpstr);

			//òAë±ÇµÇƒ10âcã∆ì˙ñ≥Ç¢èÍçáÇÕëŒè€äO.
			if ((Rec[idx_jigyokbn].eseq - Rec[idx_jigyokbn].sseq + 1) == 10) {
#ifdef _DEBUG
    //            if( idx_jigyokbn == 0 ) {
				//	char str[64], str2[64], str3[64];
				//	DbgViewTrace(_T("ÅyDEBUGÅz[%02d].sseq = %d sddate = %ld eseq = %d eddate = %ld jigyokbn = %d keigen = %s uriage = %s wariai = %s wariai_f = %lf\n"),
				//		idx_jigyokbn,
				//		Rec[idx_jigyokbn].sseq, Rec[idx_jigyokbn].sddate,
				//		Rec[idx_jigyokbn].eseq, Rec[idx_jigyokbn].eddate,
				//		idx_jigyokbn,
				//		m_ArithEx.l_print(str, &Rec[idx_jigyokbn].keigenuriage[0], _T("s,sss,sss,sss,ss9")),
				//		m_ArithEx.l_print(str2, &Rec[idx_jigyokbn].uriage[0], _T("s,sss,sss,sss,ss9")),
				//		m_ArithEx.l_print(str3, &Rec[idx_jigyokbn].keigenwariai[0], _T("ssss9")),
				//		Rec[idx_jigyokbn].keigenwariai_f
				//	);
				//}
#endif
				m_TenDayRecList[idx_jigyokbn].push_back(Rec[idx_jigyokbn]);
			}
			memset(&Rec, 0, sizeof(TenDayRec) * JIGYOKUBUN_MAX);
			memset(&tmpvalstr[0], '\0', sizeof(tmpvalstr));
			tmpstr.IsEmpty();
		}

		std::sort(m_TenDayRecList[idx_jigyokbn].begin(), m_TenDayRecList[idx_jigyokbn].end());

#ifdef _DEBUG
  //      if( idx_jigyokbn == 0 ) {
		//	DbgViewTrace(_T("\nÅyDEBUGÅzÉ\Å[Égå„\n"));
		//	for (std::vector<TenDayRec>::iterator itr = m_TenDayRecList[idx_jigyokbn].begin(); itr != m_TenDayRecList[idx_jigyokbn].end(); itr++) {
		//		char str[64], str2[64], str3[64];
		//		DbgViewTrace(_T("ÅyDEBUGÅzsseq = %d sddate = %ld eseq =%d eddate = %ld jigyokbn = %d keigen = %s uriage = %s wariai = %s\n"),
		//			itr->sseq, itr->sddate,
		//			itr->eseq, itr->eddate,
		//			idx_jigyokbn,
		//			m_ArithEx.l_print(str, &itr->keigenuriage[0], _T("s,sss,sss,sss,ss9")),
		//			m_ArithEx.l_print(str2, &itr->uriage[0], _T("s,sss,sss,sss,ss9")),
		//			m_ArithEx.l_print(str3, &itr->keigenwariai[0], _T("sss9"))
		//		);
		//	}
		//}
#endif
	}

	return;
}

void CSimKSyukei::GetSyukeiUriwari10EigyobiJogaiAri(SIMKURIWARIZREC* pRec)
{

	if (pRec == NULL) {
		return;
	}

	BOOL bIsDataKbn = FALSE;
	BYTE ckflg = 0x01;

	char val100[MONY_BUF_SIZE] = { 0 };
	CString str100 = _T("100");
	m_ArithEx.l_input(&val100, str100.GetBuffer());

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {

		bIsDataKbn = FALSE;

		if (idx_jigyokbn > ID_JIGYOKUBUN_OROSHI) {
			ckflg = ckflg << 1;
		}

		//ÉGÉâÅ[É`ÉFÉbÉN.
		std::vector<TenDayRec>::iterator itr = m_TenDayRecList[idx_jigyokbn].begin();
		if (itr == m_TenDayRecList[idx_jigyokbn].end()) {
			memset(&pRec->uriage[idx_jigyokbn], '\0', sizeof(pRec->uriage[idx_jigyokbn]));
			memset(&pRec->uriage_bekki[idx_jigyokbn], '\0', sizeof(pRec->uriage_bekki[idx_jigyokbn]));
			pRec->sddate[idx_jigyokbn] = 0;
			pRec->eddate[idx_jigyokbn] = 0;
			memset(&pRec->keigenuriage[idx_jigyokbn], '\0', sizeof(pRec->keigenuriage[idx_jigyokbn]));
			memset(&pRec->keigenuriage_bekki[idx_jigyokbn], '\0', sizeof(pRec->keigenuriage_bekki[idx_jigyokbn]));
			memset(&pRec->keigenwariai[idx_jigyokbn], '\0', sizeof(pRec->keigenwariai[idx_jigyokbn]));
			continue;
		}
		memcpy(&pRec->uriage[idx_jigyokbn], itr->uriage, sizeof(pRec->uriage[idx_jigyokbn]));
		memcpy(&pRec->uriage_bekki[idx_jigyokbn], itr->uriage_bekki, sizeof(pRec->uriage_bekki[idx_jigyokbn]));

		pRec->sddate[idx_jigyokbn] = itr->sddate;
		pRec->eddate[idx_jigyokbn] = itr->eddate;

		memcpy(&pRec->keigenuriage[idx_jigyokbn], itr->keigenuriage, sizeof(pRec->keigenuriage[idx_jigyokbn]));
		memcpy(&pRec->keigenuriage_bekki[idx_jigyokbn], itr->keigenuriage_bekki, sizeof(pRec->keigenuriage_bekki[idx_jigyokbn]));

		char tmpval[MONY_BUF_SIZE];
		m_ArithEx.l_div(&tmpval, itr->keigenwariai, val100);
		memcpy(&pRec->keigenwariai[idx_jigyokbn], tmpval, sizeof(pRec->keigenwariai[idx_jigyokbn]));

#ifdef _DEBUG
		char str[16][64];
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] sddate:%d eddate:%d\n"), idx_jigyokbn, pRec->sddate[idx_jigyokbn], pRec->eddate[idx_jigyokbn]);
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriage:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[2], &pRec->uriage[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenuriage:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[3], &pRec->keigenuriage[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenwariai:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[4], &pRec->keigenwariai[idx_jigyokbn], _T("sss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriage_bekki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[7], &pRec->uriage_bekki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenuriage_bekki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[8], &pRec->keigenuriage_bekki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("\n"));
#endif

	}

	return;

}

void CSimKSyukei::GetSyukeiUriwari10Eigyobi(SIMKURIWARIZREC* pRec)
{

#ifdef _DEBUG
	DbgViewTrace(_T("\n"));
	DbgViewTrace(_T("ÅyDEBUGÅz"));
	DbgViewTrace(_T(__FUNCTION__));
	DbgViewTrace(_T("\n"));
#endif

    if( pRec == NULL ) {
        return;
	}

	BOOL bIsDataKbn = FALSE;
	BYTE ckflg = 0x01;

	char val100[MONY_BUF_SIZE] = { 0 };
	CString str100 = _T("100");
	m_ArithEx.l_input(&val100, str100.GetBuffer());

    memset(&pRec->sgaikbn_keigenuriage_sum, 0, sizeof(pRec->sgaikbn_keigenuriage_sum));
    memset(&pRec->sgaikbn_uriage_sum, 0, sizeof(pRec->sgaikbn_uriage_sum));

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {

        bIsDataKbn = FALSE;

        if (idx_jigyokbn > ID_JIGYOKUBUN_OROSHI) {
            ckflg = ckflg << 1;
		}

		memset(&pRec->uriagegoukei[idx_jigyokbn], 0, sizeof(pRec->uriagegoukei[idx_jigyokbn]));
		memset(&pRec->uriagegoukei_bekki[idx_jigyokbn], 0, sizeof(pRec->uriagegoukei_bekki[idx_jigyokbn]));
		memset(&pRec->keigenuriagegoukei[idx_jigyokbn], 0, sizeof(pRec->keigenuriagegoukei[idx_jigyokbn]));
		memset(&pRec->keigenuriagegoukei_bekki[idx_jigyokbn], 0, sizeof(pRec->keigenuriagegoukei_bekki[idx_jigyokbn]));
		memset(&pRec->hanbaisyoreikinhiyo[idx_jigyokbn], 0, sizeof(pRec->hanbaisyoreikinhiyo[idx_jigyokbn]));
		memset(&pRec->uriagewaribiki[idx_jigyokbn], 0, sizeof(pRec->uriagewaribiki[idx_jigyokbn]));
		memset(&pRec->kashidaorekaisyu[idx_jigyokbn], 0, sizeof(pRec->kashidaorekaisyu[idx_jigyokbn]));
		memset(&pRec->kashidaoreson[idx_jigyokbn], 0, sizeof(pRec->kashidaoreson[idx_jigyokbn]));

		for (std::vector<DayRec>::iterator itr = m_DayRecList[idx_jigyokbn].begin(); itr != m_DayRecList[idx_jigyokbn].end(); itr++) {
            if( !itr->IsZero ) {
				bIsDataKbn = TRUE;//'20.01.22

				m_ArithEx.l_add(&pRec->uriagegoukei[idx_jigyokbn], &pRec->uriagegoukei[idx_jigyokbn], itr->uriage);
				m_ArithEx.l_add(&pRec->keigenuriagegoukei[idx_jigyokbn], &pRec->keigenuriagegoukei[idx_jigyokbn], itr->keigenuriage);
				m_ArithEx.l_add(&pRec->uriagegoukei_bekki[idx_jigyokbn], &pRec->uriagegoukei_bekki[idx_jigyokbn], itr->uriage_bekki);
				m_ArithEx.l_add(&pRec->keigenuriagegoukei_bekki[idx_jigyokbn], &pRec->keigenuriagegoukei_bekki[idx_jigyokbn], itr->keigenuriage_bekki);
				m_ArithEx.l_add(&pRec->hanbaisyoreikinhiyo[idx_jigyokbn], &pRec->hanbaisyoreikinhiyo[idx_jigyokbn], itr->hanbaisyoreikinhiyo);
				m_ArithEx.l_add(&pRec->uriagewaribiki[idx_jigyokbn], &pRec->uriagewaribiki[idx_jigyokbn], itr->uriagewaribiki);
				m_ArithEx.l_add(&pRec->kashidaorekaisyu[idx_jigyokbn], &pRec->kashidaorekaisyu[idx_jigyokbn], itr->kashidaorekaisyu);
				m_ArithEx.l_add(&pRec->kashidaoreson[idx_jigyokbn], &pRec->kashidaoreson[idx_jigyokbn], itr->kashidaoreson);

				/*'20.01.22
				//ëŒâûï€óØÇ∆Ç»Ç¡ÇΩèàóùÅBèàóùÇÉRÉÅÉìÉgâª
                if(m_ArithEx.l_test(itr->uriage) || m_ArithEx.l_test(itr->keigenuriage)
					|| m_ArithEx.l_test(itr->uriage_bekki) || m_ArithEx.l_test(itr->keigenuriage_bekki)
					|| m_ArithEx.l_test(itr->hanbaisyoreikinhiyo) || m_ArithEx.l_test(itr->uriagewaribiki)
					|| m_ArithEx.l_test(itr->kashidaorekaisyu) || m_ArithEx.l_test(itr->kashidaoreson) ) 
				{
	                bIsDataKbn = TRUE;
				}
				*/

			}
        }

        //ÉfÅ[É^Ç™Ç†ÇÈãÊï™ÇÃÇ›ì]ãLÉTÉCÉìON.
        if( bIsDataKbn && !m_TenDayRecList[idx_jigyokbn].empty()) {
			pRec->tenkisign[idx_jigyokbn] = TRUE;
		}
        else {
			pRec->tenkisign[idx_jigyokbn] = FALSE;
		}

        //ÉGÉâÅ[É`ÉFÉbÉN.
		std::vector<TenDayRec>::iterator itr = m_TenDayRecList[idx_jigyokbn].begin();
        if( itr == m_TenDayRecList[idx_jigyokbn].end()) {
            continue;
		}

        if( m_ArithEx.l_test(&pRec->uriagegoukei[idx_jigyokbn]) ) {
            ;
		}
        else {
			pRec->sddate[idx_jigyokbn] = 0;
			pRec->eddate[idx_jigyokbn] = 0;
		}

#ifdef _DEBUG
        char str[16][64];

		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriagegoukei:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[0], &pRec->uriagegoukei[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenuriagegoukei:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[1], &pRec->keigenuriagegoukei[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] sddate:%d eddate:%d\n"), idx_jigyokbn, pRec->sddate[idx_jigyokbn], pRec->eddate[idx_jigyokbn]);
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriage:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[2], &pRec->uriage[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenuriage:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[3], &pRec->keigenuriage[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenwariai:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[4], &pRec->keigenwariai[idx_jigyokbn], _T("sss9")));

		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriagegoukei_bekki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[5], &pRec->uriagegoukei_bekki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenuriagegoukei_bekki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[6], &pRec->keigenuriagegoukei_bekki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriage_bekki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[7], &pRec->uriage_bekki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] keigenuriage_bekki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[8], &pRec->keigenuriage_bekki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));

		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] hanbaisyoreikinhiyo:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[9], &pRec->hanbaisyoreikinhiyo[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] uriagewaribki:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[10], &pRec->uriagewaribiki[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));

		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] kashidaorekaisyu:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[11], &pRec->kashidaorekaisyu[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));
		DbgViewTrace(_T("ÅyDEBUGÅz[%02d] kashidaoreson:%s\n"), idx_jigyokbn, m_ArithEx.l_print(str[12], &pRec->kashidaoreson[idx_jigyokbn], _T("s,sss,sss,sss,ss9")));

		DbgViewTrace(_T("\n"));
#endif

	}


	char str1[64], str2[64];
	DbgViewTrace(_T("ÅyDEBUGÅzsgaikbn_keigenuriage_sum:%s\n"), m_ArithEx.l_print(str1, &pRec->sgaikbn_keigenuriage_sum, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzsgaikbn_uriage_sum:%s\n"), m_ArithEx.l_print(str2, &pRec->sgaikbn_uriage_sum, _T("s,sss,sss,sss,ss9")));

    return;
}

void CSimKSyukei::GetSyukeiSirewari(SIMKSIREWARIZREC* pRec)
{

#ifdef _DEBUG
	DbgViewTrace(_T("\n"));
	DbgViewTrace(_T("ÅyDEBUGÅz"));
	DbgViewTrace(_T(__FUNCTION__));
	DbgViewTrace(_T("\n"));
#endif

	if (pRec == NULL) {
		return;
	}

	memset(&pRec->sire, 0, sizeof(pRec->sire));
	memset(&pRec->sire358, 0, sizeof(pRec->sire358));
	memset(&pRec->tksire8, 0, sizeof(pRec->tksire8));
	memset(&pRec->tksire10, 0, sizeof(pRec->tksire10));
	memset(&pRec->kamotsusire, 0, sizeof(pRec->kamotsusire));
	memset(&pRec->kamotsusire358, 0, sizeof(pRec->kamotsusire358));
    memset(&pRec->keigensire, 0, sizeof(pRec->keigensire));
	memset(&pRec->uriagegoukei, 0, sizeof(pRec->uriagegoukei));
	memset(&pRec->keigenuriagegoukei, 0, sizeof(pRec->keigenuriagegoukei));
	memset(&pRec->tanaoroshikasankei8, 0, sizeof(pRec->tanaoroshikasankei8));
	memset(&pRec->tanaoroshigensankei8, 0, sizeof(pRec->tanaoroshigensankei8));
	memset(&pRec->tanaoroshikasan10, 0, sizeof(pRec->tanaoroshikasan10));
	memset(&pRec->tanaoroshigensan10, 0, sizeof(pRec->tanaoroshigensan10));
	memset(&pRec->sirehenkann10, 0, sizeof(pRec->sirehenkann10));
	memset(&pRec->sirehenkannkei8, 0, sizeof(pRec->sirehenkannkei8));
	memset(&pRec->sirehenkann358, 0, sizeof(pRec->sirehenkann358));
	memset(&pRec->tksirehenkann10, 0, sizeof(pRec->tksirehenkann10));

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {

		for (std::vector<DayRec>::iterator itr = m_DayRecList[idx_jigyokbn].begin(); itr != m_DayRecList[idx_jigyokbn].end(); itr++) {

			if (idx_jigyokbn == ID_JIGYOKUBUN_NASHI) {
				if (!itr->IsZero) {
					m_ArithEx.l_add(&pRec->sire, &pRec->sire, itr->sire);
					m_ArithEx.l_add(&pRec->sire358, &pRec->sire358, itr->sire358);
					m_ArithEx.l_add(&pRec->tksire8, &pRec->tksire8, itr->tksire8);
					m_ArithEx.l_add(&pRec->tksire10, &pRec->tksire10, itr->tksire10);
					m_ArithEx.l_add(&pRec->kamotsusire, &pRec->kamotsusire, itr->kamotsusire);
					m_ArithEx.l_add(&pRec->kamotsusire358, &pRec->kamotsusire358, itr->kamotsusire358);
					m_ArithEx.l_add(&pRec->keigensire, &pRec->keigensire, itr->keigensire);
					m_ArithEx.l_add(&pRec->tanaoroshikasankei8, &pRec->tanaoroshikasankei8, itr->tanaoroshikasankei8);
					m_ArithEx.l_add(&pRec->tanaoroshigensankei8, &pRec->tanaoroshigensankei8, itr->tanaoroshigensankei8);
					m_ArithEx.l_add(&pRec->tanaoroshikasan10, &pRec->tanaoroshikasan10, itr->tanaoroshikasan10);
					m_ArithEx.l_add(&pRec->tanaoroshigensan10, &pRec->tanaoroshigensan10, itr->tanaoroshigensan10);
					m_ArithEx.l_add(&pRec->sirehenkann10, &pRec->sirehenkann10, itr->sirehenkann10);
					m_ArithEx.l_add(&pRec->sirehenkannkei8, &pRec->sirehenkannkei8, itr->sirehenkannkei8);
					m_ArithEx.l_add(&pRec->sirehenkann358, &pRec->sirehenkann358, itr->sirehenkann358);
					m_ArithEx.l_add(&pRec->tksirehenkann10, &pRec->tksirehenkann10, itr->tksirehenkann10);
				}
			}

			if (!itr->IsZero) {
                if( m_kazeihousiki == ID_ICSSH_KANNI ) {
                    if( idx_jigyokbn == ID_JIGYOKUBUN_NASHI ) {
						m_ArithEx.l_add(&pRec->uriagegoukei, &pRec->uriagegoukei, itr->uriage);
						m_ArithEx.l_add(&pRec->keigenuriagegoukei, &pRec->keigenuriagegoukei, itr->keigenuriage);
					}
				}
                else {
					m_ArithEx.l_add(&pRec->uriagegoukei, &pRec->uriagegoukei, itr->uriage);
					m_ArithEx.l_add(&pRec->keigenuriagegoukei, &pRec->keigenuriagegoukei, itr->keigenuriage);
				}
			}

        }

    }

	char val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	char val624[MONY_BUF_SIZE], val780[MONY_BUF_SIZE];

	CString str100 = _T("100");
	CString str108 = _T("108");
	CString str110 = _T("110");
	CString str624 = _T("624");
	CString str780 = _T("780");

	m_ArithEx.l_input(&val100, str100.GetBuffer());
	m_ArithEx.l_input(&val108, str108.GetBuffer());
	m_ArithEx.l_input(&val110, str110.GetBuffer());
	m_ArithEx.l_input(&val624, str624.GetBuffer());
	m_ArithEx.l_input(&val780, str780.GetBuffer());

	m_ArithEx.l_mul(pRec->tksire8, pRec->tksire8, val108);
	m_ArithEx.l_div(pRec->tksire8, pRec->tksire8, val100);

	m_ArithEx.l_mul(pRec->tksire10, pRec->tksire10, val110);
	m_ArithEx.l_div(pRec->tksire10, pRec->tksire10, val100);

	m_ArithEx.l_mul(pRec->tksirehenkann10, pRec->tksirehenkann10, val110);
	m_ArithEx.l_div(pRec->tksirehenkann10, pRec->tksirehenkann10, val100);

	m_ArithEx.l_mul(pRec->tanaoroshikasankei8, pRec->tanaoroshikasankei8, val624);
	m_ArithEx.l_div(pRec->tanaoroshikasankei8, pRec->tanaoroshikasankei8, val100);
	m_ArithEx.l_div(pRec->tanaoroshikasankei8, pRec->tanaoroshikasankei8, val108);
	m_ArithEx.l_mul(pRec->tanaoroshigensankei8, pRec->tanaoroshigensankei8, val624);
	m_ArithEx.l_div(pRec->tanaoroshigensankei8, pRec->tanaoroshigensankei8, val100);
	m_ArithEx.l_div(pRec->tanaoroshigensankei8, pRec->tanaoroshigensankei8, val108);
	m_ArithEx.l_mul(pRec->tanaoroshikasan10, pRec->tanaoroshikasan10, val780);
	m_ArithEx.l_div(pRec->tanaoroshikasan10, pRec->tanaoroshikasan10, val100);
	m_ArithEx.l_div(pRec->tanaoroshikasan10, pRec->tanaoroshikasan10, val110);
	m_ArithEx.l_mul(pRec->tanaoroshigensan10, pRec->tanaoroshigensan10, val780);
	m_ArithEx.l_div(pRec->tanaoroshigensan10, pRec->tanaoroshigensan10, val100);
	m_ArithEx.l_div(pRec->tanaoroshigensan10, pRec->tanaoroshigensan10, val110);

#ifdef _DEBUG
    char str[16][64];
	DbgViewTrace(_T("ÅyDEBUGÅzsire:%s\n"), m_ArithEx.l_print(str[0], &pRec->sire, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzsire358:%s\n"), m_ArithEx.l_print(str[1], &pRec->sire358, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅztksire8:%s\n"), m_ArithEx.l_print(str[2], &pRec->tksire8, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅztksire10:%s\n"), m_ArithEx.l_print(str[3], &pRec->tksire10, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzkamotsusire:%s\n"), m_ArithEx.l_print(str[4], &pRec->kamotsusire, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzkamotsusire358:%s\n"), m_ArithEx.l_print(str[5], &pRec->kamotsusire358, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzkeigensire:%s\n"), m_ArithEx.l_print(str[6], &pRec->keigensire, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzuriagegoukei:%s\n"), m_ArithEx.l_print(str[7], &pRec->uriagegoukei, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzkeigenuriagegoukei:%s\n"), m_ArithEx.l_print(str[8], &pRec->keigenuriagegoukei, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅztanakasankei8:%s\n"), m_ArithEx.l_print(str[9], &pRec->keigenuriagegoukei, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅztanakasan10:%s\n"), m_ArithEx.l_print(str[10], &pRec->tanaoroshikasan10, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅztanagensan10:%s\n"), m_ArithEx.l_print(str[11], &pRec->tanaoroshigensan10, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzsirehenkan:%s\n"), m_ArithEx.l_print(str[12], &pRec->sirehenkann10, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzkeigensirehenkan:%s\n"), m_ArithEx.l_print(str[13], &pRec->sirehenkannkei8, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅzsirehenkan358:%s\n"), m_ArithEx.l_print(str[14], &pRec->sirehenkann358, _T("s,sss,sss,sss,ss9")));
	DbgViewTrace(_T("ÅyDEBUGÅztksirehenkann10:%s\n"), m_ArithEx.l_print(str[15], &pRec->tksirehenkann10, _T("s,sss,sss,sss,ss9")));
#endif

    return;
}

//5â≠â~îªíËëŒè€ÉfÅ[É^ÇèWåvÇµÅA5â≠â~Çí¥Ç¶ÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©îªíËÇ∑ÇÈ.
bool CSimKSyukei::Syukei5okuHantei(char* syukei_money)
{
    bool bOver = false;

	char money_val[6] = { 0 };
	char money_zei[6] = { 0 };
	char money_sum[6] = { 0 };

	memset(&money_val[0], 0, sizeof(money_val));
	memset(&money_zei[0], 0, sizeof(money_zei));
	memset(&money_sum[0], 0, sizeof(money_sum));

#if _DEBUG
	char strmoney_sum[64] = { 0 };
	char strmoney_val[64] = { 0 };
	char strmoney_zei[64] = { 0 };
#endif

	//m_SiwakeRecListÇÃ5â≠â~îªíËëŒè€ÉfÅ[É^Ç©ÇÁã‡äzÇèWåv.
    for (std::vector<SiwakeRec>::iterator itr = m_SiwakeRecList[0].begin(); itr != m_SiwakeRecList[0].end(); itr++) {

#if _DEBUG
   //     DbgViewTrace(_T("ÅyDEBUGÅz%d %s() dbt:%s dbtgroup:0x%02x dbtgroupmeisai:0x%02x cre:%s cregroup:0x%02x cregroupmeisai:0x%02x val:%s zei:%s\n"), 
			//__LINE__, __FUNCTION__, itr->dbt, itr->dbtgroup, itr->dbtgroupmeisai, itr->cre, itr->cregroup, itr->cregroupmeisai, itr->val, itr->zei);
#endif

		if (itr->syutokujoto == SISAN_JOTO) {
			m_ArithEx.l_input(&money_val[0], itr->taika.GetBuffer());
		}
		else {
			m_ArithEx.l_input(&money_val[0], itr->val.GetBuffer());
		}
		m_ArithEx.l_input(&money_zei[0], itr->zei.GetBuffer());

		//ë›ï˚â»ñ⁄
		if (Is5okuHanteiData(itr, ID_KASHIKATA)) {
#if _DEBUG
			//DbgViewTrace(_T("ÅyDEBUGÅz%d %s() money_sum:%s - money_val:%s = "),
			//	__LINE__, __FUNCTION__,
			//	m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
			//	m_ArithEx.l_print(strmoney_val, &money_val[0], _T("s,sss,sss,sss,ss9"))
			//);
#endif
			if ((itr->dbt.Compare(KariukeSyohizei) == 0) && (itr->ddate < INVOICE_SYZ_YMD)) {
				//âºéÛè¡îÔê≈ÇÃèÍçá, â€ê≈äJénì˙à»ëOÇ»ÇÁà¯Ç≠.
				m_ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_val[0]);
			}
			else {
				m_ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_val[0]);
			}
#if _DEBUG
			//DbgViewTrace(_T("money_sum:%s \n"), m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif

			//ì‡ê≈ â€ê≈äJénì˙à»ç~ÇÕê≈î≤Ç´.
			if ((itr->zeikubun == ZEIKUBUN_UTIZEI) && (itr->ddate >= INVOICE_SYZ_YMD)) {
#if _DEBUG
				//DbgViewTrace(_T("ÅyDEBUGÅz%d %s() [ì‡ê≈] money_sum:%s + money_zei:%s = "),
				//	__LINE__, __FUNCTION__,
				//	m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	m_ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				m_ArithEx.l_add(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}

			//äOê≈ â€ê≈äJénì˙ÇÊÇËëOÇÕê≈çûÇ›
			if ((itr->zeikubun == ZEIKUBUN_SOTOZEI) && (itr->ddate < INVOICE_SYZ_YMD)) {
#if _DEBUG
				//DbgViewTrace(_T("ÅyDEBUGÅz%d %s() [äOê≈] money_sum:%s - money_zei:%s = "),
				//	__LINE__, __FUNCTION__,
				//	m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	m_ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				m_ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}
		}

		//éÿï˚â»ñ⁄
		if(Is5okuHanteiData(itr, ID_KARIKATA)) {
#if _DEBUG
			//DbgViewTrace(_T("ÅyDEBUGÅz%d %s() money_sum:%s + money_val:%s = "),
			//	__LINE__, __FUNCTION__,
			//	m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
			//	m_ArithEx.l_print(strmoney_val, &money_val[0], _T("s,sss,sss,sss,ss9"))
			//);
#endif
			if ((itr->cre.Compare(KariukeSyohizei) == 0) && (itr->ddate < INVOICE_SYZ_YMD)) {
				//âºéÛè¡îÔê≈ÇÃèÍçá, â€ê≈äJénì˙à»ëOÇ»ÇÁë´Ç∑.
				m_ArithEx.l_add(&money_sum[0], &money_sum[0], &money_val[0]);
			}
            else {
				m_ArithEx.l_add(&money_sum[0], &money_sum[0], &money_val[0]);
			}
#if _DEBUG
			//DbgViewTrace(_T("money_sum:%s \n"), m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif

			//ì‡ê≈ â€ê≈äJénì˙à»ç~ÇÕê≈î≤Ç´.
			if ((itr->zeikubun == ZEIKUBUN_UTIZEI) && (itr->ddate >= INVOICE_SYZ_YMD)) {
#if _DEBUG
				//DbgViewTrace(_T("ÅyDEBUGÅz%d %s() [ì‡ê≈] money_sum:%s - money_zei:%s = "), 
				//	__LINE__, __FUNCTION__,
				//	m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	m_ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				m_ArithEx.l_sub(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}

			//äOê≈ â€ê≈äJénì˙ÇÊÇËëOÇÕê≈çûÇ›
            if( (itr->zeikubun == ZEIKUBUN_SOTOZEI) && (itr->ddate < INVOICE_SYZ_YMD)) {
#if _DEBUG
				//DbgViewTrace(_T("ÅyDEBUGÅz%d %s() [äOê≈] money_sum:%s + money_zei:%s = "),
				//	__LINE__, __FUNCTION__,
				//	m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")),
				//	m_ArithEx.l_print(strmoney_zei, &money_zei[0], _T("s,sss,sss,sss,ss9"))
				//);
#endif
				m_ArithEx.l_add(&money_sum[0], &money_sum[0], &money_zei[0]);
#if _DEBUG
				//DbgViewTrace(_T("money_sum:%s \n"), m_ArithEx.l_print(strmoney_sum, &money_sum[0], _T("s,sss,sss,sss,ss9")));
#endif
			}
		}
	}

    //5â≠â~Çí¥Ç¶ÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
    char money_5oku[6] = {0};
	m_ArithEx.l_input(&money_5oku[0], _T("500000000"));
    if(m_ArithEx.l_cmp(&money_sum[0], &money_5oku[0]) > 0 ) {
        bOver = true;
	}

    memcpy(&syukei_money[0], &money_sum[0], sizeof(money_sum));

    return bOver;
}

//édñÛÉfÅ[É^Ç™5â≠â~îªíËópÇÃÉfÅ[É^Ç©Ç«Ç§Ç©.
bool CSimKSyukei::Is5okuHanteiData(std::vector<SiwakeRec>::iterator itr, EnumIdKariKashiKbn type)
{

    bool bSyukei = false;
	int group, groupmeisai;
	CString kncod;

	if (type == ID_KASHIKATA) {
		group = itr->dbtgroup;
		groupmeisai = itr->dbtgroupmeisai;
		kncod = itr->dbt;
	}
	else {
		group = itr->cregroup;
		groupmeisai = itr->cregroupmeisai;
		kncod = itr->cre;
	}

    //âºéÛè¡îÔê≈
	if (kncod.Compare(KariukeSyohizei) == 0) {
        bSyukei = true;
        return bSyukei;
	}

	if (itr->siwakekubun) {
		//è¡îÔê≈édñÛãÊï™Ç†ÇË.
		switch (itr->siwakekubun) {
		    case ID_SKA_URIAGE:               //îÑè„
			case ID_SKA_URIAGENEBIKIMODORI:   //îÑè„ílà¯ÅEñﬂÇË
			case ID_SKA_URIAGEWARIBIKI:       //îÑè„äÑà¯
			case ID_SKA_YUSYUTSUMENZEIURIAGE: //óAèoÅ@ñ∆ê≈îÑè„ 
				if (group) {
					bSyukei = true;
				}
				break;
		}
	}
	else {
		//édñÛãÊï™Ç»Çµ.
		switch (group) {
		case KAMOKUGROUP_SISAN: //éëéY
			switch (groupmeisai) {
			case KAMOKUMEISAI_SISAN_JOTO: //éëéYè˜ìn
				bSyukei = true;
				break;
			}
			break;
		case KAMOKUGROUP_TAIKASISAN: //ëŒâø&éëéY
			switch (groupmeisai) {
			case KAMOKUMEISAI_TAIKASISAN_SYUTOKUJOTO: //è˜ìn
				if (itr->syutokujoto == SISAN_JOTO) {
					bSyukei = true;
				}
				break;
			}
			break;
		case KAMOKUGROUP_URIAGE: //îÑè„
			switch (groupmeisai) {
			case KAMOKUMEISAI_URIAGE_URIAGE:              //îÑè„
			case KAMOKUMEISAI_URIAGE_URIAGEHENPIN:        //îÑè„ï‘ïi
			case KAMOKUMEISAI_URIAGE_EIGYOGAISYUEKI:      //âcã∆äOé˚âv 
			case KAMOKUMEISAI_URIAGE_TOKUBETSURIEKI:      //ì¡ï óòâv
			case KAMOKUMEISAI_URIAGE_UKETORIRISOKU:       //éÛéÊóòëß
			case KAMOKUMEISAI_URIAGE_URIAGEWARIBIKI:      //îÑè„äÑà¯
			case KAMOKUMEISAI_URIAGE_YUSYUTUMENZEIURIAGE: //óAèoñ∆ê≈îÑè„
				bSyukei = true;
				break;
			}
			break;
		}
	}

	return bSyukei;
}

