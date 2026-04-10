#include "stdafx.h"
#include "SimKuriwariW.h"
#include "SimKuriwariDlg.h"

CSimKuriwariW::CSimKuriwariW(CDBZmSub* pZmSub)
{
    m_pZmSub = pZmSub;
    BOOL bOpenFlg = FALSE;

    if( m_pZmSub->zvol == NULL) {
		m_pZmSub->VolumeOpen();
        bOpenFlg = TRUE;
	}
	m_kazeihousiki = m_SyzUtil.GetKazeihoushiki(m_pZmSub->zvol); 
    m_pSimKuriwariDlg = NULL;
    m_pSyukeiTenkiDlg = NULL;

    if( bOpenFlg ) {
		m_pZmSub->VolumeClose();
	}
}

CSimKuriwariW::~CSimKuriwariW()
{
	if (m_pSimKuriwariDlg) {
		m_pSimKuriwariDlg->DestroyWindow();
		delete m_pSimKuriwariDlg;
		m_pSimKuriwariDlg = NULL;
	}
}

int CSimKuriwariW::CreateDlg(CWnd* pParent)
{
    int rt = 0;

	if (m_pSimKuriwariDlg) {
		delete m_pSimKuriwariDlg;
		m_pSimKuriwariDlg = NULL;
	}

	if ((m_pSimKuriwariDlg = new CSimKuriwariDlg(m_pZmSub, pParent)) == NULL) {
		rt = 0;
	}

	rt = (int)m_pSimKuriwariDlg->DoModal();
	if (rt == -1) {
		m_pSimKuriwariDlg->DestroyWindow();
		delete m_pSimKuriwariDlg;
		m_pSimKuriwariDlg = NULL;
		return rt;
	}


    return rt;
}

int CSimKuriwariW::CreateSyukeiDlg(CWnd* pParent,
	long sddate, long eddate)
{
	int rt = 0;

	if (m_pSyukeiTenkiDlg) {
		delete m_pSyukeiTenkiDlg;
		m_pSyukeiTenkiDlg = NULL;
	}

	m_Syukei.m_pZmSub = m_pZmSub;
	m_Syukei.Init();
	m_MngTables.m_pZmSub = m_pZmSub;
	m_MngTables.Init();

	m_MngTables.CreateEigyobiTable();
	m_MngTables.CreateSyukujitsuTable();
	m_MngTables.CreateKyujitsuTable();
	m_MngTables.CreateSettingHolidayTable();

	m_MngTables.ReadSyukujitsu();
	m_MngTables.ReadKyujitsu();

	m_MngTables.CreateTmpTable();
    m_MngTables.CreateSyukeiTenkiTable();
	m_Syukei.GetDbData(sddate, eddate);
	m_Syukei.SyukeiDay();
	m_Syukei.SyukeiRenzoku10Eigyobi();

	if ((m_pSyukeiTenkiDlg = new CSyukeiTenkiDlg(pParent, &m_MngTables, &m_Syukei, m_kazeihousiki)) == NULL) {
		rt = 0;
	}

	rt = (int)m_pSyukeiTenkiDlg->DoModal();
	if (rt == -1) {
		m_pSyukeiTenkiDlg->DestroyWindow();
		delete m_pSyukeiTenkiDlg;
		m_pSyukeiTenkiDlg = NULL;
		return rt;
	}


	return rt;
}

void CSimKuriwariW::EndSimKuriwariDlg(int nResult)
{
	if (m_pSimKuriwariDlg) {
        m_pSimKuriwariDlg->EndSettingHolidaysDlg(nResult);
		m_pSimKuriwariDlg->EndDialog(nResult);
	}
}

void CSimKuriwariW::EndSyukeiDlg(int nResult)
{
	if (m_pSyukeiTenkiDlg) {
        m_pSyukeiTenkiDlg->EndDialog(nResult);
	}
}

int CSimKuriwariW::ReadDlgSyukeiData(SIMKURIWARIZREC* pRec)
{
    int rt = 0;

	m_MngTables.m_pZmSub = m_pZmSub;
	m_MngTables.Init();
    m_MngTables.ReadSyukeiTenkiTable(pRec);

    return rt;
}

void CSimKuriwariW::SyukeiKuriwari10Eigyobi(SIMKURIWARIZREC* pRec, long sddate, long eddate)
{
    int rt = 0;

	m_Syukei.m_pZmSub = m_pZmSub;
	m_Syukei.Init();
	m_MngTables.m_pZmSub = m_pZmSub;
	m_MngTables.Init();

	m_MngTables.CreateEigyobiTable();
	m_MngTables.CreateSyukujitsuTable();
	m_MngTables.CreateKyujitsuTable();
	m_MngTables.CreateSettingHolidayTable();

	m_MngTables.ReadSyukujitsu();
	m_MngTables.ReadKyujitsu();

	//10営業日関連のメンバ(集計除外日設定あり)だけ先に集計.
	m_MngTables.CreateTmpTable(true);
	m_Syukei.GetDbData(sddate, eddate, true);
	m_Syukei.SyukeiDay();
	m_Syukei.SyukeiRenzoku10Eigyobi();
	m_Syukei.GetSyukeiUriwari10EigyobiJogaiAri(pRec);

	//10営業日関連のメンバ以外を集計.
	m_MngTables.CreateTmpTable(false);
	m_Syukei.GetDbData(sddate, eddate, false);
	m_Syukei.SyukeiDay();
	m_Syukei.SyukeiRenzoku10Eigyobi();
	m_Syukei.GetSyukeiUriwari10Eigyobi(pRec);

    return;
}

void CSimKuriwariW::SyukeiKsirewari(SIMKSIREWARIZREC* pRec, bool IsNeedTkkz, long sddate, long eddate)
{
	int rt = 0;

	m_Syukei.m_pZmSub = m_pZmSub;
	m_Syukei.m_IsNeedTkkz = IsNeedTkkz;
	m_Syukei.Init();
	m_MngTables.m_pZmSub = m_pZmSub;
	m_MngTables.Init();

	m_MngTables.CreateEigyobiTable();
	m_MngTables.CreateSyukujitsuTable();
	m_MngTables.CreateKyujitsuTable();
	m_MngTables.CreateSettingHolidayTable();

	m_MngTables.ReadSyukujitsu();
	m_MngTables.ReadKyujitsu();

	m_MngTables.CreateTmpTable(false, true);

	m_Syukei.GetDbDataSub(sddate, eddate, 0, false);
	m_Syukei.SyukeiDay();

    m_Syukei.GetSyukeiSirewari(pRec);

	return;
}

