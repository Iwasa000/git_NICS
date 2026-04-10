#pragma once

#define JIGYOKUBUN_MAX 0x07

typedef enum {

	ID_JIGYOKUBUN_NASHI = 0,   //‹æ•ª‚È‚µ
	ID_JIGYOKUBUN_OROSHI = 1,  //‘æ1–‹Æí ‰µ”„‹Æ
	ID_JIGYOKUBUN_KOURI = 2,   //‘æ2–‹Æí ¬”„‹Æ
	ID_JIGYOKUBUN_SEIZO = 3,   //‘æ3–‹Æí »‘¢‹Æ
	ID_JIGYOKUBUN_SONOTA = 4,  //‘æ4–‹Æí ‚»‚Ì‘¼
	ID_JIGYOKUBUN_SERVICE = 5, //‘æ5–‹Æí ƒT[ƒrƒX
	ID_JIGYOKUBUN_HUDOSAN = 6, //‘æ6–‹Æí •s“®Y

} EnumIdJigyokubun;

typedef enum {

	ID_SYUKEIKUBUN_NASHI = 0,             //‹æ•ª‚È‚µ.
	ID_SYUKEIKUBUN_URIAGE,                //”„ã.
	ID_SYUKEIKUBUN_SHIIRE,                //d“ü.
	ID_SYUKEIKUBUN_TOKUTEI_SHIIRE,        //d“ü.(“Á’è‰ÛÅd“ü‚ÉŒW‚é)
    ID_SYUKEIKUBUN_TOKUTEI_SHIIRE_HENKANN,//d“ü.(“Á’è‰ÛÅd“ü‚ÉŒW‚é).•ÔŠÒ.
	ID_SYUKEIKUBUN_KAMOTSU_SHIIRE,        //d“ü.(•ÛÅ’nˆæ‚©‚çˆø‚«æ‚Á‚½‰ÛÅ‰İ•¨‚ÉŒW‚é)
    ID_SYUKEIKUBUN_TYOUSEI_KASAN_SHIIRE,  //d“ü.(”[Å‹`–±‚Ì–Æœ‚ğó‚¯‚È‚¢(ó‚¯‚é)‚±‚Æ‚Æ‚È‚Á‚½ê‡‚É‚¨‚¯‚éÁ”ïÅŠz‚Ì’²®(‰ÁZ–”‚ÍŒ¸Z)Šz)
    ID_SYUKEIKUBUN_TYOUSEI_GENSAN_SHIIRE, //d“ü.(”[Å‹`–±‚Ì–Æœ‚ğó‚¯‚È‚¢(ó‚¯‚é)‚±‚Æ‚Æ‚È‚Á‚½ê‡‚É‚¨‚¯‚éÁ”ïÅŠz‚Ì’²®(‰ÁZ–”‚ÍŒ¸Z)Šz)
	ID_SYUKEIKUBUN_SHIIRE_HENKANN,        //d“ü.(d“ü•ÔŠÒ)
	ID_SYUKEIKUBUN_HANBAISYOREIKINHIYO,   //”Ì”„§—ã‹à”ï—p.
	ID_SYUKEIKUBUN_URIAGEWARIBIKI,        //”„ãŠ„ˆø.
	ID_SYUKEIKUBUN_KASHIDAOREKAISYU,      //‘İ“|‰ñû.
	ID_SYUKEIKUBUN_KASHIDAORESON,         //‘İ“|‘¹¸.
	ID_SYUKEIKUBUN_KARIUKESYOHIZEI,       //‰¼óÁ”ïÅ.
	ID_SYUKEIKUBUN_KARIBARAISYOHIZEI,     //‰¼•¥Á”ïÅ.

} EnumIdSyukeiKubun;

typedef enum {
    ID_KARIKATA = 0, //Ø•û‰È–Ú
    ID_KASHIKATA     //‘İ•û‰È–Ú
} EnumIdKariKashiKbn;

typedef struct _SiwakeRec {

	long ddate;
	CString dbt;
	char dbtgroup;
	char dbtgroupmeisai;
	CString cre;
	char cregroup;
	char cregroupmeisai;
	CString val;
	CString zei;
	CString taika;
	char zeikubun;
	char zeiritsu;
	char syutokujoto;
	char jigyokubun;
	char siwakekubun;

} SiwakeRec;

typedef struct _DayRec {

	//˜A‘±‚·‚é‰c‹Æ“ú‚Ì˜A”Ô.
	int seq;
	long ddate;
	char IsZero;
	char jigyokubun;
	
	char keigenuriage[MONY_BUF_SIZE];        //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‰ÛÅ”„ã.
	char keigenuriage_bekki[MONY_BUF_SIZE];  //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‰ÛÅ”„ã.(•Ê‹L)
	char uriage[MONY_BUF_SIZE];              //“ú–ˆ‚Ì‰ÛÅ”„ã.
	char uriage_bekki[MONY_BUF_SIZE];        //“ú–ˆ‚Ì‰ÛÅ”„ã.(•Ê‹L)
	char kariuke[MONY_BUF_SIZE];             //“ú–ˆ‚Ì‰¼ó(Œy8.0%+10.0%)
	char keigenkariuke[MONY_BUF_SIZE];       //“ú–ˆ‚Ì‰¼ó(Œy8.0%)
    char keigensire[MONY_BUF_SIZE];          //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‰ÛÅd“ü.
    char sire[MONY_BUF_SIZE];                //“ú–ˆ‚Ì‰ÛÅd“ü.
	char sire358[MONY_BUF_SIZE];             //“ú–ˆ‚Ì‰ÛÅd“ü.(‹ŒÅ—¦(3%,5%,8%))
	char tksire10[MONY_BUF_SIZE];            //“ú–ˆ‚Ì“Á’è‰ÛÅd“ü‚ê‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz.
	char tksire8[MONY_BUF_SIZE];             //“ú–ˆ‚Ì“Á’è‰ÛÅd“ü‚ê‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz.(‹ŒÅ—¦“K—p•ª)
	char kamotsusire[MONY_BUF_SIZE];         //“ú–ˆ‚Ì•ÛÅ’nˆæ‚©‚çˆø‚«æ‚Á‚½‰ÛÅ‰İ•¨‚ÉŒW‚éÅˆøæ‰¿Šz.
	char kamotsusire358[MONY_BUF_SIZE];      //“ú–ˆ‚Ì•ÛÅ’nˆæ‚©‚çˆø‚«æ‚Á‚½‰ÛÅ‰İ•¨‚ÉŒW‚éÅˆøæ‰¿Šz.(‹ŒÅ—¦(3%,5%,8%))

	char tanaoroshikasankei8[MONY_BUF_SIZE]; //“ú–ˆ‚Ì’I‰µ’²®Šz(Œy8.0%)
	char tanaoroshigensankei8[MONY_BUF_SIZE];//“ú–ˆ‚Ì’I‰µ’²®Šz(Œy8.0%)
	char tanaoroshikasan10[MONY_BUF_SIZE];   //“ú–ˆ‚Ì’I‰µ’²®Šz(10.0%)
	char tanaoroshigensan10[MONY_BUF_SIZE];  //“ú–ˆ‚Ì’I‰µ’²®Šz(10.0%)
	char sirehenkann358[MONY_BUF_SIZE];      //“ú–ˆ‚Ì‹ŒÅ—¦‰ÛÅd“ü.(•ÔŠÒ).
	char sirehenkannkei8[MONY_BUF_SIZE];     //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‰ÛÅd“ü.(•ÔŠÒ).
	char sirehenkann10[MONY_BUF_SIZE];       //“ú–ˆ‚Ì‰ÛÅd“ü.(•ÔŠÒ).
    char tksirehenkann10[MONY_BUF_SIZE];     //“ú–ˆ‚Ì“Á’è‰ÛÅd“ü‚ê‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz.(•ÔŠÒ).
	char hanbaisyoreikinhiyo[MONY_BUF_SIZE]; //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ”Ì”„§—ã‹à”ï—p.
	char uriagewaribiki[MONY_BUF_SIZE];      //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ”„ãŠ„ˆø.
	char kashidaorekaisyu[MONY_BUF_SIZE];    //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‘İ“|‰ñû.
	char kashidaoreson[MONY_BUF_SIZE];       //“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‘İ“|‘¹¸.

} DayRec;

typedef struct _TenDayRec {

	int sseq;
	long sddate;
	int eseq;
	long eddate;
	char IsZero;
	//10‰c‹Æ“ú–ˆ‚ÌŒyŒ¸Å—¦‘ÎÛ‰ÛÅ”„ã.
	unsigned char keigenuriage[6];
	unsigned char keigenuriage_bekki[6];
	//10‰c‹Æ“ú–ˆ‚Ì‰ÛÅ”„ã.
	unsigned char uriage[6];
	unsigned char uriage_bekki[6];
	//ŒyŒ¸”„ãŠ„‡.
	unsigned char keigenwariai[6];
	double keigenwariai_f;

	bool operator<(const _TenDayRec& right) {
		//ŒyŒ¸”„ãŠ„‡ ~‡.
		return keigenwariai_f > right.keigenwariai_f;
	}

} TenDayRec;

class CSimKSyukei
{

public:
	CDBZmSub* m_pZmSub;
	CArithEx m_ArithEx;
	std::vector<SiwakeRec> m_SiwakeRecList[JIGYOKUBUN_MAX]; 
	std::vector<DayRec> m_DayRecList[JIGYOKUBUN_MAX];
	std::vector<TenDayRec> m_TenDayRecList[JIGYOKUBUN_MAX];
	CSyzUtil m_SyzUtil;
	EnumIdIcsShKazeihoushiki m_kazeihousiki;
    int m_kwariai95oversw;
	bool m_IsNeedTkkz;

public:
	CSimKSyukei();
	~CSimKSyukei();

    void Init();
	bool CheckJigyokubunSettei(int idx_jigyokubun);
	void GetDbData(long sddate=0, long eddate=0, bool bSettingEigyobi=false);
	void GetDbDataSub(long sddate = 0, long eddate = 0, int jigyokubun = 0, bool bSettingEigyobi = false);
	void SyukeiDay();
    void SubSyukeiDayGetSyukeiKubun(EnumIdSyukeiKubun* pSyukeiKubun,
                                    std::vector<SiwakeRec>::iterator itr,
                                    std::vector<SiwakeRec>::iterator subitr,
		                            EnumIdKariKashiKbn type);

	void SubSyukeiDaySetData(EnumIdSyukeiKubun* pSyukeiKubun,
		                     std::vector<SiwakeRec>::iterator itr,
                             std::vector<SiwakeRec>::iterator subitr,
		                     DayRec* pDayRec,
		                     EnumIdKariKashiKbn type);

	void SubSyukeiDayUriageKubun(std::vector<SiwakeRec>::iterator itr, 
                                 std::vector<SiwakeRec>::iterator subitr, 
                                 DayRec* pDayRec,
		                         EnumIdKariKashiKbn type);

	void SubSyukeiDayKariukeKubun(std::vector<SiwakeRec>::iterator itr,
                                  std::vector<SiwakeRec>::iterator subitr,
                                  DayRec* pDayRec,
                                  EnumIdKariKashiKbn type);

	void SubSyukeiDaySireKubun(std::vector<SiwakeRec>::iterator itr,
		                       std::vector<SiwakeRec>::iterator subitr,
		                       DayRec* pDayRec,
                               EnumIdKariKashiKbn type);

	void SubSyukeiDayTokuteiSireKubun(std::vector<SiwakeRec>::iterator itr,
		                              std::vector<SiwakeRec>::iterator subitr,
		                              DayRec* pDayRec,
		                              EnumIdKariKashiKbn type);

	void SubSyukeiDayTokuteiSireHenkannKubun(std::vector<SiwakeRec>::iterator itr,
                                             std::vector<SiwakeRec>::iterator subitr,
                                             DayRec* pDayRec,
                                             EnumIdKariKashiKbn type);

	void SubSyukeiDayKamotsuSireKubun(std::vector<SiwakeRec>::iterator itr,
		                              std::vector<SiwakeRec>::iterator subitr,
		                              DayRec* pDayRec,
		                              EnumIdKariKashiKbn type);

	void SubSyukeiDayTyouseiKasanSireKubun(std::vector<SiwakeRec>::iterator itr,
	                                       std::vector<SiwakeRec>::iterator subitr,
                                           DayRec* pDayRec,
                                           EnumIdKariKashiKbn type);

	void SubSyukeiDayTyouseiGensanSireKubun(std::vector<SiwakeRec>::iterator itr,
		                                    std::vector<SiwakeRec>::iterator subitr,
		                                    DayRec* pDayRec,
                                            EnumIdKariKashiKbn type);

	void SubSyukeiDaySireHenkannKubun(std::vector<SiwakeRec>::iterator itr,
                                      std::vector<SiwakeRec>::iterator subitr,
                                      DayRec* pDayRec,
                                      EnumIdKariKashiKbn type);

	void SubSyukeiDayHanbaisyoreikinhiyoKubun(std::vector<SiwakeRec>::iterator itr,
		                                      std::vector<SiwakeRec>::iterator subitr,
                                              DayRec* pDayRec,
                                              EnumIdKariKashiKbn type);

	void SubSyukeiDayUriageWaribikiKubun(std::vector<SiwakeRec>::iterator itr,
                                         std::vector<SiwakeRec>::iterator subitr,
                                         DayRec* pDayRec,
                                         EnumIdKariKashiKbn type);

	void SubSyukeiDayKashidaorekaisyuKubun(std::vector<SiwakeRec>::iterator itr,
		                                 std::vector<SiwakeRec>::iterator subitr,
                                         DayRec* pDayRec,
                                         EnumIdKariKashiKbn type);

	void SubSyukeiDayKashidaoresonKubun(std::vector<SiwakeRec>::iterator itr,
                                         std::vector<SiwakeRec>::iterator subitr,
                                         DayRec* pDayRec,
                                         EnumIdKariKashiKbn type);

    void SyukeiRenzoku10Eigyobi();
    void GetSyukeiUriwari10EigyobiJogaiAri(SIMKURIWARIZREC* pRec);
    void GetSyukeiUriwari10Eigyobi(SIMKURIWARIZREC* pRec);
    void GetSyukeiSirewari(SIMKSIREWARIZREC* pRec);
	//5‰­‰~”»’è‘ÎÛƒf[ƒ^‚ğWŒv‚µA5‰­‰~‚ğ’´‚¦‚Ä‚¢‚é‚©‚Ç‚¤‚©”»’è‚·‚é.
    bool Syukei5okuHantei(char* syukei_money);
    //d–óƒf[ƒ^‚ª5‰­‰~”»’è—p‚Ìƒf[ƒ^‚©‚Ç‚¤‚©.
    bool Is5okuHanteiData(std::vector<SiwakeRec>::iterator itr,	EnumIdKariKashiKbn type);

};

