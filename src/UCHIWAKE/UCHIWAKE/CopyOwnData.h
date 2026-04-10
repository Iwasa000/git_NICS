#pragma once
using namespace std;

typedef struct OWN_INFO_{
	OWN_INFO_(){
		youshiki_skip = 0;
		ikkatu_outzero = 0;
		name_kei = 0;
	}

	int youshiki_skip;		// オプション設定→データのある様式へ移動する
	int ikkatu_outzero;		// ソート→一括金額に０円を含まない
	int name_kei;			// 合計設定→計名称
}OWN_INFO;

typedef struct INF_SUB_COPY_DATA_{
	int ikkatu_outzero;
	int name_kei;
}INF_SUB_COPY_DATA;

// テーブルバージョン7以前のFormSeqとFormOrder
static int old_map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
						101,102,103,104,105,201,202,203,204,205};

class CCopyOwnData
{
public:
			CCopyOwnData(CDBZmSub* pZmSub);
			~CCopyOwnData(void);
	BOOL	Copy();

private:
	BOOL	GetOwnTblInfo(OWN_INFO& own_info);
	BOOL	SetUchiwakeTables(const OWN_INFO& own_info);
	int		GetFormSeqNameKei(int nNameKei, int val);
	int		GetIkkatuOutZero(int nFormSeq, int isIkkatuZero);
	BOOL	CopyFromOwnTable(int youshiki_skip, map<int, INF_SUB_COPY_DATA>& inf_sub_map);

private:
	CDBZmSub* m_ZmSub;
};