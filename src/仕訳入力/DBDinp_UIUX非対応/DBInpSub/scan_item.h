typedef struct _SCAN_PAR
{

int	sc_opt;

int	sc_seq1, sc_seq2;
WORD	sc_seqopt;
BYTE 	sc_date1[2], sc_date2[2];
WORD	sc_dateopt;
int	sc_cno1, sc_cno2;
WORD	sc_cnoopt;
int	sc_dbmn1, sc_dbmn2;
WORD	sc_dbmnopt;
int	sc_cbmn1, sc_cbmn2;
WORD	sc_cbmnopt;

char	sc_dkji1[14], sc_dkji2[14];
WORD	sc_dkjiopt;
char	sc_ckji1[14], sc_ckji2[14];
WORD	sc_ckjiopt;

char	sc_dbt[10],
WORD	sc_dbtopt;
int	sc_dbr1;
int	sc_dbr2;
WORD	sc_dbropt;
		
char	sc_cre[10],
WORD	sc_creopt;
int	sc_cbr1;
int	sc_cbr2;
WORD	sc_cbropt;

char	sc_dbcr[10];
WORD	sc_dbcropt;
int	sc_dcbrn1;
int	sc_dcbrn2;
WORD	sc_dcbropt;

char	sc_val1[6],	sc_val2[6];
WORD	sc_valopt;
char	sc_zei1[6],	sc_zei2[6];
WORD	sc_zeiopt;

short	sc_syzskbn, sc_zritu, sc_skbn, sc_urisre;
WORD	sc_syzskbnopt, sc_zrituopt, sc_skbnopt, sc_urisreopt;

char	sc_tky[82];
WORD	sc_tkyopt;

char	sc_snum1[10], sc_snum2[10];
WORD	sc_snumopt;

BYTE	sc_tgdate1[4], sc_tgdate2[4];
WORD	sc_tgdtopt;

short	sc_husen;
char	sc_hucmnt[12];
WORD	sc_husenopt;

} SCAN_PAR;