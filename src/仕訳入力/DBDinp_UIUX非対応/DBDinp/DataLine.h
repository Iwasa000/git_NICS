/////////////////////////////////////////////////////////////////
// DataLine.h
#define APPEND_LINE		0

enum DICTRL_TYPE {
DIP_NONE	= 0,
DIP_MONO	= 1,
DIP_BOTH,
DIP_LIST
};

int		get_nowln();
void	_set_nowln( int ln );

struct _DATA_LINE *getLINE_DATA( void );
struct _DATA_LINE *_getLINE_DATA( int ln );

int getLINE_BumonData( int dc_sw );
CString getLINE_KojiData( int dc_sw );

struct _InpCtl* _getINP_CTL( int ln, int pn );
struct _InpCtl* getINP_CTL( int pn );

struct _InpCtl* getINP_CTL( CWnd* pwnd, int pn );
struct _InpCtl* _getINP_CTL( CWnd* pwnd, int ln, int pn );


#ifdef EXTERN_DECLARE_DATALINE

extern	struct _DATA_LINE	INP_LINE;
// data display
extern	struct _DATA_LINE DDATA_LINE[];

#endif