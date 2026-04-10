#ifndef ZANASK_INCLUDE_H
#define ZANASK_INCLUDE_H


struct _DIP_KZREC
{
	char	kz_code[10];
	BYTE	kz_att;
	BYTE	kz_dmy;
	char	kz_zan[6];
	char	kz_kari[20][6];
	char	kz_kasi[20][6];
};


struct _TTLDSP_TBL* ttldsp( int mode );
struct _TTLDSP_TBL* ttldsp( int mode, CMINFO *volp );

int zrecget2( LPCTSTR, struct _DIP_KZREC * );
int zrecget2X( LPCTSTR code, struct _DBKZREC *mrec );


#endif // end of ZANASK_INCLUDE_H