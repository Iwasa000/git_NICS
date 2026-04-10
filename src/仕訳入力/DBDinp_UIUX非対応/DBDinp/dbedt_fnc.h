#ifndef INCLUDE_DBEDT_FNC_H
#define INCLUDE_DBEDT_FNC_H


long DBdata_set( CWnd* pwnd, int id, VARIANT* data, int type, int bcdlen );
long DBdata_get( CWnd* pwnd, int id, VARIANT* data, int type, int bcdlen );
void DBSetDispString( CWnd* pwnd, int id, LPCTSTR string );
int DBkamokudata_get( CWnd* pwnd, int id, struct _KamokuData* data );
int DBkamokudata_set( CWnd* pwnd, int id, struct _KamokuData* data );
short DateInputCheck( char* check, BYTE* bcd );
int KamokuStr_Chk( LPCTSTR string, struct _KamokuData* data, BOOL noteda = FALSE );
short TgDateInputCheck( char* check, char* result, int c_ddate, int c_tdate, int s_tdate );


#endif // INCLUDE_DBEDT_FNC_H