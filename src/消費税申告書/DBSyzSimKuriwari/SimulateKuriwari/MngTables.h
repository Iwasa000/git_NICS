#pragma once

typedef struct _HORIDAYREC {
	int ymd;
	char sname[64];

	bool operator<(const _HORIDAYREC& right) {
		return ymd < right.ymd;
	}

} HOLIDAYREC;

class CMngTables
{

public:
	CDBZmSub* m_pZmSub;
	std::vector<HOLIDAYREC> m_syukujitsu_list;
	std::vector<HOLIDAYREC> m_kyujitsu_list;
	CVolDateDB m_Voldate;
	CArithEx m_ArithEx;
	CSyzUtil m_SyzUtil;
	EnumIdIcsShKazeihoushiki m_kazeihousiki;

public:
	CMngTables();

    void Init();

	//集計用一時テーブル.
	void CreateTmpTable(bool bSettingEigyobi = true, bool bSyukeiKsirewari = false);

    //営業日関連.
	void CreateEigyobiTable();
	void DeleteEigyobiTable();
	void UpdateEigyobiTable(char* ck_wday);
	void UpdateKyujitsuEigyobiTable(int ymd, bool hflg);

    //祝日リスト.
	void CreateSyukujitsuTable();
	void DeleteSyukujitsuTable();
	void ReadSyukujitsu();
	void UpdateSyukujitsuTable();
    void ReInitSyukujitsuList();

    //休日リスト.
	void CreateKyujitsuTable();
	void DeleteKyujitsuTable();
	void ReadKyujitsu();
	void UpdateKyujitsuTable();

	//祝日・休日設定関連
	void CreateSettingHolidayTable();
	void DeleteSettingHolidayTable();
	void ReadSettingHolidayTable ( int *wday );
	void UpdateSettingHolidayTable(char* ck_wday);
	char GetYobiCheckStateFromHolidayTable(int wday);

	// テーブル存在チェック
	int  IsExistTable ( CString table );

    //集計データ関連
	void CreateSyukeiTenkiTable();
	void DeleteSyukeiTenkiTable();
    void ReadSyukeiTenkiTable(SIMKURIWARIZREC* pRec);
	void UpdateSyukeiTenkiTable(ICSReportControl* pReport, char* ck_tenkikbn, bool* ck_st);
	void UpdateSyukeiTenkiTable(int idx_jigyokbn, 
                                char* pKeigenUriageGoukei,
                                char* pKariuke, char* pKeigenKariuke); 
    void ClearJigyokbnSyukeiTenkiTable(int idx_jigyokbn);
    char GetTenkiCheckStateFromSyukeiTenkiTable(int tenkikbn);

	int GetOffsetDay(int year, int month, int day);
	bool IsSyukujitsu(int ymd);
	bool IsSyukujitsuFromMemory(int ymd);
	void RemakeSyukujitsuList(ICSReportControl* pReport);
	bool IsKyujitsuFromMemory(int ymd);
	int DeleteKyujitsuFromMemory(int ymd);
	void RemakeKyujitsuList(ICSReportControl* pReport);

	~CMngTables();
};

