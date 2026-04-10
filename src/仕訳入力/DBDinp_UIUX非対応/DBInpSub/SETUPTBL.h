// ì¸óÕÇÃê›íËÇì«Ç›çûÇ›Ç‹Ç∑ÅB
#ifndef INCLUDE_SETUPTBLDEF_H
#define INCLUDE_SETUPTBLDEF_H


#ifdef _DBINPSUB
class AFX_EXT_CLASS CSETUPTBL
#else
class CSETUPTBL
#endif
{
public:
	CSETUPTBL( class CDatabase* pDatabase );

	int		Requery( CString filter );
	int		Open( CString filter );
	void	Close();
	int		MoveFirst();
	int		MoveNext();
	int		MovePrev();
	int		MoveLast();
	int		Move( int nrows );
	int		SetAbsolutePosition( int nrows );
	void	Append();
	void	Correct();
	int		GetRecordCount();

public:
	int		st;
	CString setupname;
	int		param;
	CString	parastr;
	char	rsv[128];

private:
	class CSetuptbl*	m_setup;
	char	rsv2[128];
	int		nodata;
	void	data();
};


#ifdef _DBINPSUB
class AFX_EXT_CLASS CMastSetUp
#else
class CMastSetUp
#endif
{
public:
	CMastSetUp();
	~CMastSetUp();

public:
	int MSU_SetDB(char *server, unsigned char apno, int cod, int ymd, int ntype=0, char *id=0, char *pwd=0 );
	int MSU_SetDB(int valid_ver, char *server, unsigned char apno, int cod, int ymd, int ntype=0, char *id=0, char *pwd=0 );

	int	SetUpOpen();
	int	SetUpClose();

	CSETUPTBL*	setup;
	char		rsv[128];

private:
	void	setuptbl_make();
	int		object_check( int sign, char *objectname );

	class CDatabase* m_database;
	char	rsv2[128];

};



#endif