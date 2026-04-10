#pragma once

#include "SimulateKuriwari.h"
#include "SimKuriwariDlg.h"

class CSimKuriwariW
{
public:
    CDBZmSub* m_pZmSub;
	CSimKuriwariDlg* m_pSimKuriwariDlg;
	CSyukeiTenkiDlg* m_pSyukeiTenkiDlg;
	CSimKSyukei m_Syukei;
	CMngTables m_MngTables;
	CSyzUtil m_SyzUtil;
	EnumIdIcsShKazeihoushiki m_kazeihousiki;

public:
	CSimKuriwariW(CDBZmSub* pZmSub);
	~CSimKuriwariW();

	int CreateDlg(CWnd* pParent);

	int CreateSyukeiDlg(CWnd* pParent,
		long sddate, long eddate);

	void EndSimKuriwariDlg(int nResult);
	void EndSyukeiDlg(int nResult);
    int ReadDlgSyukeiData(SIMKURIWARIZREC* pRec);
    void SyukeiKuriwari10Eigyobi(SIMKURIWARIZREC* pRec, long sddate=0, long eddate=0);
	void SyukeiKsirewari(SIMKSIREWARIZREC* pRec, bool IsNeedTkkz, long sddate=0, long eddate=0);

};

