

#ifndef	_INCLUDE_SWKCPYSRC_H_
#define	_INCLUDE_SWKCPYSRC_H_


#define NAME_SHAREMEM	"SHMEM_SWKCPY"
#define	SIZE_SHAREMEM	64*1024

#define COPY_MAXSWKCNT	1000


#include <Kmatset8.h>
#include "SCopyMsgDlg.h"

// 仕訳コピー元
typedef struct _SWKCPY_SRC
{
	char	sc_sqlsvr[256];
	char	sc_apno[20];
	char	sc_cocode[20];
	char	sc_kikan[20];
	char	sc_dmy[4];
	int		sc_cnt;

	int		sc_seq[1];

} SWKCPY_SRC;



class CSWKCpySrc
{
public:
	CSWKCpySrc();
	virtual ~CSWKCpySrc();

	void	SrcDataRead();
	void	SrcDataWrite(SWKCPY_SRC* scd );

	BOOL	SrcMappingCondition();
	void	TraceMappingView();

	void	SetDbtoMd2( CDBDATA* rec );
	int		CheckData2( CDBDATA * pDataRec, int* pEflg);
	int		PickData( int bseq );
	int		BmonNamHanichk( CDBDATA* rec, BOOL bBulk );
	int		MastChk();

	int		SetCpyZmsub( CDipZmSub* pDes, CDBZmSub* pSrc, CWnd* m_pParent );
	void	SetMdtoDESdbdata( CDBDATA* rec );

	int		CopyGyosyuCheck( CString& msgstr );
	int		SyzVersionChk();

public:
	SWKCPY_SRC*	m_pScpySrc;

// インプリメンテーション
protected:
	HANDLE	m_hMapping;     // メモリマップドファイルのハンドル
	void*	m_pMappingView;  // ファイルのビューへのポインタ
	CMutex*	m_pMutex;      // ミューテックスオブジェクト
	BOOL	m_bExist;

	CKmatset8*	pKMATSET8;

	CDipZmSub*	m_pDESzm;
	CDBZmSub*	m_pSRCzm;

	CWnd*		m_pParent;

	int			m_swImg;
	int			m_swAbort;

	int			m_Erflg;
	CString		m_Ermsg;

	CSCopyMsgDlg	*m_pMsgDlg;

};




#endif //_INCLUDE_SWKCPYSRC_H_