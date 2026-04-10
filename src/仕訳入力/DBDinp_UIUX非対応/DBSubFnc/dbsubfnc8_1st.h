#ifdef _DBSUBFNC_
#define DBSUBFNC	__declspec( dllexport )
#else
#define	DBSUBFNC	__declspec( dllimport )
#endif


// Kamoku Data Structure
// kd_sgn --> ICSInput の chk_kmk.sgn と同じサインをセットする
//		3桁以上			0
//		2桁以下			1
//		枝番のみ		2
//		ﾋﾟﾘｵﾄﾞのみ		3
//		3桁以上+ﾋﾟﾘｵﾄﾞ	4
//		2桁以下+ﾋﾟﾘｵﾄﾞ	5
//		-1のときは、カナのみセットされている
// 

typedef struct _KamokuData
{
	char	kd_name[18];
	char	kd_code[10];
	char	kd_kana[6];
	int		kd_eda;
	short	kd_sgn;

} KamokuData;


extern "C"
{
DBSUBFNC int WINAPI DateInputCheck( char* check, BYTE* bcd );
DBSUBFNC int WINAPI KamokuStr_Chk( LPCTSTR string, struct _KamokuData* data, BOOL noteda = FALSE );
DBSUBFNC int WINAPI TekiyoStr_Chk( LPCTSTR tkstr );
// 11.13 /06
DBSUBFNC int WINAPI KamokuStr_ChkLength( LPCTSTR string, struct _KamokuData* data, int edcol );
};



