
#ifndef _SUBFNC_H_
#define _SUBFNC_H_

# ifdef RPTKEYDEL	// 09.04 /99
int RepKeyDel( tagMSG* pMsg );
# endif
BOOL isEntrySys();

void Buzzer();
void Buzzer2();

# ifdef BIGIMG	// 03.16 /00
unsigned char *img_trim( IMREC *des, IMREC *sou, long x, long y );
# endif

int	date_devide( int date, int* mm, int* dd );

#endif	// end of _SUBFNC_H_