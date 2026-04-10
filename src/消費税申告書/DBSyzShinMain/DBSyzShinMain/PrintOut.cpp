#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"
#include "SkjUserSettings.h" // TISW修正[22-0676] 2026/03/19

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 24/02/19wd_特定収入対応 add -->
extern PRNEXSUB_INF									m_PrnEXSub;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub;
// 24/02/19wd_特定収入対応 add <--
//20240307
extern PRNEXSUB_INF									m_PrnEXSub_KAZ;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_KAZ;

extern PRNEXSUB_INF									m_PrnEXSub_ZEI;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_ZEI;

int		Sv_busu[3];			// 24/07/10_個別タブから出力 add

// CDBSyzShinMainView

//-----------------------------------------------------------------------------
// 印刷情報の初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::InitPrintInfo()
{
	// プリンタチェック
	char	PrinterName[80]={0};
	m_PrintCmInfo.isColorPrn = FALSE;
	if( PCOM_GetString(_T("SelectPrinter"), PrinterName) != -1 ){
		m_PrintCmInfo.isColorPrn = IsColorPrinter( PrinterName );
	}

// 24/07/10_個別タブから出力 add -->
	for(int ii=0; ii<3; ii++)	{
		Sv_busu[ii] = 0;
	}
// 24/07/10_個別タブから出力 add <--

	// 出力情報取得
	GetPrInitInfo();
	GetPrInitInfo2();	// 部数
//// 24/02/26wd_特定収入対応 add -->
//	GetPrnExSubInfo();	// 特定収入計算表 個別出力設定
//// 24/02/27wd_特定収入対応 add <--

	// 印刷微調整情報の取得
	GetPrAdjustInfo();

	return 0;
}

//-----------------------------------------------------------------------------
// 出力情報取得
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	正常終了
//			FALSE	：	エラー
//-----------------------------------------------------------------------------
//BOOL CDBSyzShinMainView::GetInitInfo()
BOOL CDBSyzShinMainView::GetPrInitInfo()
{
	CString	filter;
	CString	filter_sub;//yoshida201225
	BOOL result = TRUE;
//--> '15.04.04 CUT START
//	char			codbf[10];
//	int				cnt, flg, seq;
//	unsigned int	rsiz, rcnt;
//	unsigned long	tsiz;
//	int				thd;
//<-- '15.04.04 CUT END

	// オーナーテーブルオープン
	if( m_pZmSub->owntb == NULL ){
		if( m_pZmSub->OwnTblOpen() == -1 ){
			return FALSE;
		}
	}

	unsigned short	numno;
	unsigned long	numlong;
	char			numbf[16]={0};

	EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo = {0};
	if( m_pMainFrame ){
		m_pMainFrame->GetBatchPrintInfo( &BpState, &BpPrintInfo );
	}

	//----> 20240116
	if( m_pMainFrame ){
		m_pMainFrame->GetScoreLinkInfo( m_isScoreLinkCall, m_ScoreLinkPdfNam );
	}

	CString itm_name = "";
	if( m_isScoreLinkCall != -1 ){
		itm_name.Format( DBSYZSHIN_ITMNAME );
	}
	else{
		itm_name.Format( BpState ? BpPrintInfo.ForkKey : DBSYZSHIN_ITMNAME );
	}
	//<---- 20240116

	// 印刷情報の取得
	//filter.Format( OWNTBL_SQL, DBSYZSHIN_APLNAME, (BpState ? BpPrintInfo.ForkKey : DBSYZSHIN_ITMNAME) );
	filter.Format( OWNTBL_SQL, DBSYZSHIN_APLNAME, itm_name );
	if( m_pZmSub->owntb->Requery(filter) == -1 ){

		//---->yoshida201225
		//一括印刷にて、レコードない場合には財務レコードをチェックし、あれば財務レコードデータ分をセットする
		//データをセットするだけでレコードは追加しない。終了時のレコード書き込みにまかす形
		int record_chk = 0; 
		
		//if (BpState) {
		if (BpState && m_isScoreLinkCall == -1) { // 20240116
			filter_sub.Empty();
			filter_sub.Format(OWNTBL_SQL, DBSYZSHIN_APLNAME, DBSYZSHIN_ITMNAME);
			if (m_pZmSub->owntb->Requery(filter_sub) == -1) {
				record_chk = 0;
			}
			else {
				record_chk = 1;
			}
		}
		if (record_chk == 1) {
			m_PrintCmInfo.OP1 = (char)m_pZmSub->owntb->vl[0];
			m_PrintCmInfo.OP2 = (char)m_pZmSub->owntb->vl[1];
			m_PrintCmInfo.OP3 = (char)m_pZmSub->owntb->vl[2];
			m_PrintCmInfo.OP4 = (char)m_pZmSub->owntb->vl[3];
			m_PrintCmInfo.OP5 = (char)m_pZmSub->owntb->vl[4];
			m_PrintCmInfo.OP6 = (char)m_pZmSub->owntb->vl[5];
			m_PrintCmInfo.OP7 = (char)m_pZmSub->owntb->vl[6];
			m_PrintCmInfo.OP8 = (char)m_pZmSub->owntb->vl[7];
			m_PrintCmInfo.OP9 = (char)m_pZmSub->owntb->vl[8];
			m_PrintCmInfo.OPA = (char)m_pZmSub->owntb->vl[9];
			m_PrintCmInfo.OPB = (char)m_pZmSub->owntb->vl[10];
			m_PrintCmInfo.OPC = (char)m_pZmSub->owntb->vl[11];
			m_PrintCmInfo.OPD = (char)m_pZmSub->owntb->vl[12];
			m_PrintCmInfo.OPE = (char)m_pZmSub->owntb->vl[13];
			m_PrintCmInfo.OPF = (char)m_pZmSub->owntb->vl[14];
			if (m_PrintCmInfo.OPF == -1) {
				m_PrintCmInfo.OPF = 0;
			}
			m_PrintCmInfo.OPG = (char)m_pZmSub->owntb->vl[15];
			if (m_PrintCmInfo.OPG == -1) {
				m_PrintCmInfo.OPG = 0;
			}
			m_PrintCmInfo.OPH = (char)m_pZmSub->owntb->vl[16];
			if (m_PrintCmInfo.OPH == -1) {
				m_PrintCmInfo.OPH = 0;
			}
			m_PrintCmInfo.OPI = (char)m_pZmSub->owntb->vl[17];
			if (m_PrintCmInfo.OPI == -1) {
				m_PrintCmInfo.OPI = 0;
			}
			m_PrintCmInfo.PsSigDate = m_pZmSub->owntb->vl[18];
			if (m_PrintCmInfo.PsSigDate <= 0) {
				m_PrintCmInfo.PsSigDate = 0x0310;
			}
			m_PrintCmInfo.PsSigConame = m_pZmSub->owntb->vl[19];
			if (m_PrintCmInfo.PsSigConame <= 0) {
				m_PrintCmInfo.PsSigConame = 0x0a20;
			}

			if (BpState) {
				::ZeroMemory(&Save_Head, sizeof(Save_Head));
				Save_Head[0] = m_pSnHeadData->Sn_TYPE;
				Save_Head[1] = m_pSnHeadData->Sn_SPECIAL;
				Save_Head[2] = m_pSnHeadData->Sn_Sign1;
				Save_Head[3] = m_pSnHeadData->Sn_Sign2;
				Save_Head[4] = m_pSnHeadData->Sn_Sign3;
				Save_Head[5] = m_pSnHeadData->Sn_Sign4;
				Save_Headlong = m_pSnHeadData->Sn_PrintOutSgn;
				Save_Headlong2 = m_pSnHeadData->Sn_PrintOutSgn2;
			}

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[10]);
			m_PrintCmInfo.OPJ = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[11]);
			m_PrintCmInfo.OPK = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[12]);
			m_PrintCmInfo.OPL = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[13]);
			m_PrintCmInfo.OPM = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[14]);
			m_PrintCmInfo.OPN = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[15]);
			m_PrintCmInfo.OPO = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[16]);
			m_PrintCmInfo.OPP = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[17]);
			m_PrintCmInfo.OPQ = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[18]);
			m_PrintCmInfo.OPR = atoi(numbf);

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[19]);
			m_PrintCmInfo.OPS = atoi(numbf);

			//2016.02.23 INSERT START
			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[0]);
			m_PrintCmInfo.OPT = atoi(numbf);
			//2016.02.23 INSERT END

			//--> '17.03.28 INS START
			if (m_pZmSub->owntb->code[1].GetLength()) {
				memset(numbf, '\0', sizeof(numbf));
				strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[1]);
				m_PrintCmInfo.OPU = atoi(numbf);
			}
			else {
				m_PrintCmInfo.OPU = 1;
			}
			//<-- '17.03.28 INS END
			//2018.03.13 INSERT START
			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[2]);
			m_PrintCmInfo.OPV = atoi(numbf);
			//2018.03.13 INSERT END

			if (m_pZmSub->owntb->code[3].GetLength()) {
				memset(numbf, '\0', sizeof(numbf));
				strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[3]);
				m_PrintCmInfo.OP_GENGO = atoi(numbf);
			}
			else {
				m_PrintCmInfo.OP_GENGO = 1;
			}

			//---->yoshida210114
			memset(m_PrintCmInfo.PsOfceName, '\0', sizeof(m_PrintCmInfo.PsOfceName));
			GetIcsofnDirectory(m_PrintCmInfo.PsOfceName);

			if (m_pSnHeadData->Sn_Sign2 & 0x10) {
				m_PrintCmInfo.OPsign |= 0x01;
			}
			else {
				m_PrintCmInfo.OPsign &= ~0x01;
			}
			//<----------------

// 改良依頼 20-0352 修正 add -->
			if( GetActViewVer() == ID_VER_SYZ31 ) {
				// 課税標準額等の内訳書 代表者氏名 又は 氏名			0x01=ON チェックなし	0x01=OFF チェックあり
				// 課税標準額等の内訳書 代表者氏名又は氏名 フリガナ		0x02=ON チェックなし	0x02=OFF チェックあり
				// 更正の請求			代表者氏名 又は 氏名			0x03=ON チェックなし	0x03=OFF チェックあり
				// 更正の請求			代表者氏名又は氏名 フリガナ		0x04=ON チェックなし	0x04=OFF チェックあり
				if (m_pZmSub->owntb->code[4].GetLength()) {
					memset(numbf, '\0', sizeof(numbf));
					strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[4]);
					m_PrintCmInfo.OPW = atoi(numbf);
				}
				else {
					// 未入力の場合、既存マスターかつ、対応後初回起動時と判断して
					// 申告書本表の[代表者氏名 又は 氏名][代表者氏名又は氏名 フリガナ]により初期値の設定を行う
					// 申告書本表 代表者氏名 又は 氏名 チェック無し
					if(m_PrintCmInfo.OP5 == 1) {
						//課税標準額の内訳書、更正の請求のチェックをOFFにする
						m_PrintCmInfo.OPW |= 0x01;
						m_PrintCmInfo.OPW |= 0x04;
					}
					// 申告書本表 代表者氏名又は氏名 フリガナ チェック無し
					if(m_PrintCmInfo.OPG == 1) {
						//課税標準額の内訳書、更正の請求のチェックをOFFにする
						m_PrintCmInfo.OPW |= 0x02;
						m_PrintCmInfo.OPW |= 0x08;
					}
				}
			}
// 改良依頼 20-0352 修正 add <--


			if (m_pZmSub->owntb->Requery(filter) == -1) {
				return FALSE;
			}
		}
		else {
			// 新規登録
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
			//m_pZmSub->owntb->itm_name = (BpState ? BpPrintInfo.ForkKey : DBSYZSHIN_ITMNAME);
			m_pZmSub->owntb->itm_name = itm_name;
			m_pZmSub->owntb->vl[0] = (long)m_PrintCmInfo.OP1;
			m_pZmSub->owntb->vl[1] = (long)m_PrintCmInfo.OP2;
			m_pZmSub->owntb->vl[2] = (long)m_PrintCmInfo.OP3;
			m_pZmSub->owntb->vl[3] = (long)m_PrintCmInfo.OP4;
			m_pZmSub->owntb->vl[4] = (long)m_PrintCmInfo.OP5;
			m_pZmSub->owntb->vl[5] = (long)m_PrintCmInfo.OP6;
			m_pZmSub->owntb->vl[6] = (long)m_PrintCmInfo.OP7;
			m_pZmSub->owntb->vl[7] = (long)m_PrintCmInfo.OP8;
			m_pZmSub->owntb->vl[8] = (long)m_PrintCmInfo.OP9;
			m_pZmSub->owntb->vl[9] = (long)m_PrintCmInfo.OPA;
			m_pZmSub->owntb->vl[10] = (long)m_PrintCmInfo.OPB;
			m_pZmSub->owntb->vl[11] = (long)m_PrintCmInfo.OPC;
			m_pZmSub->owntb->vl[12] = (long)m_PrintCmInfo.OPD;
			m_pZmSub->owntb->vl[13] = (long)m_PrintCmInfo.OPE;
			m_pZmSub->owntb->vl[14] = (long)m_PrintCmInfo.OPF;
			m_pZmSub->owntb->vl[15] = (long)m_PrintCmInfo.OPG;
			m_pZmSub->owntb->vl[16] = (long)m_PrintCmInfo.OPH;
			m_pZmSub->owntb->vl[17] = (long)m_PrintCmInfo.OPI;
			m_pZmSub->owntb->vl[18] = m_PrintCmInfo.PsSigDate = 0x0310;
			m_pZmSub->owntb->vl[19] = m_PrintCmInfo.PsSigConame = 0x0a20;

			if (BpState) {
				numno = (unsigned short)m_pSnHeadData->Sn_TYPE;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numno);
				m_pZmSub->owntb->str[0] = numbf;

				numno = (unsigned short)m_pSnHeadData->Sn_SPECIAL;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numno);
				m_pZmSub->owntb->str[1] = numbf;

				numno = (unsigned short)m_pSnHeadData->Sn_Sign1;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numno);
				m_pZmSub->owntb->str[3] = numbf;

				numno = (unsigned short)m_pSnHeadData->Sn_Sign2;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numno);
				m_pZmSub->owntb->str[4] = numbf;

				numno = (unsigned short)m_pSnHeadData->Sn_Sign3;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numno);
				m_pZmSub->owntb->str[5] = numbf;

				numno = (unsigned short)m_pSnHeadData->Sn_Sign4;	// 付表６
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numno);
				m_pZmSub->owntb->str[6] = numbf;

				numlong = (unsigned long)m_pSnHeadData->Sn_PrintOutSgn;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numlong);
				m_pZmSub->owntb->str[7] = numbf;

				numlong = (unsigned long)m_pSnHeadData->Sn_PrintOutSgn2;
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), numlong);
				m_pZmSub->owntb->str[8] = numbf;

				::ZeroMemory(&Save_Head, sizeof(Save_Head));
				Save_Head[0] = m_pSnHeadData->Sn_TYPE;
				Save_Head[1] = m_pSnHeadData->Sn_SPECIAL;
				Save_Head[2] = m_pSnHeadData->Sn_Sign1;
				Save_Head[3] = m_pSnHeadData->Sn_Sign2;
				Save_Head[4] = m_pSnHeadData->Sn_Sign3;
				Save_Head[5] = m_pSnHeadData->Sn_Sign4;
				Save_Headlong = m_pSnHeadData->Sn_PrintOutSgn;
				Save_Headlong2 = m_pSnHeadData->Sn_PrintOutSgn2;
			}


			// 追加印刷
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPJ);
			m_pZmSub->owntb->str[10] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPK);
			m_pZmSub->owntb->str[11] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPL);
			m_pZmSub->owntb->str[12] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPM);
			m_pZmSub->owntb->str[13] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPN);
			m_pZmSub->owntb->str[14] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPO);
			m_pZmSub->owntb->str[15] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPP);
			m_pZmSub->owntb->str[16] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPQ);
			m_pZmSub->owntb->str[17] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPR);
			m_pZmSub->owntb->str[18] = numbf;

			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPS);
			m_pZmSub->owntb->str[19] = numbf;

			//2016.02.23 INSERT START
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPT);
			m_pZmSub->owntb->code[0] = numbf;
			//2016.02.23 INSERT END

			//--> '17.03.28 INS START
			m_PrintCmInfo.OPU = 1;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPU);
			m_pZmSub->owntb->code[1] = numbf;
			//<-- '17.03.28 INS END
			//2018.03.13 INSERT START
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPV);
			m_pZmSub->owntb->code[2] = numbf;
			//2018.03.13 INSERT END

			m_PrintCmInfo.OP_GENGO = 1;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OP_GENGO);
			m_pZmSub->owntb->code[3] = numbf;

// 改良依頼 20-0352 修正 add -->
			if( GetActViewVer() == ID_VER_SYZ31 ) {
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), m_PrintCmInfo.OPW);
				m_pZmSub->owntb->code[4] = numbf;
			}
// 改良依頼 20-0352 修正 add <--

			m_pZmSub->owntb->Update();
			if (m_pZmSub->owntb->Requery(filter) == -1) {
				return FALSE;
			}
		}
		//<------
	}
	else{


		m_PrintCmInfo.OP1 = (char)m_pZmSub->owntb->vl[0];
		m_PrintCmInfo.OP2 = (char)m_pZmSub->owntb->vl[1];
		m_PrintCmInfo.OP3 = (char)m_pZmSub->owntb->vl[2];
		m_PrintCmInfo.OP4 = (char)m_pZmSub->owntb->vl[3];
		m_PrintCmInfo.OP5 = (char)m_pZmSub->owntb->vl[4];
		m_PrintCmInfo.OP6 = (char)m_pZmSub->owntb->vl[5];
		m_PrintCmInfo.OP7 = (char)m_pZmSub->owntb->vl[6];
		m_PrintCmInfo.OP8 = (char)m_pZmSub->owntb->vl[7];
		m_PrintCmInfo.OP9 = (char)m_pZmSub->owntb->vl[8];
		m_PrintCmInfo.OPA = (char)m_pZmSub->owntb->vl[9];
		m_PrintCmInfo.OPB = (char)m_pZmSub->owntb->vl[10];
		m_PrintCmInfo.OPC = (char)m_pZmSub->owntb->vl[11];
		m_PrintCmInfo.OPD = (char)m_pZmSub->owntb->vl[12];
		m_PrintCmInfo.OPE = (char)m_pZmSub->owntb->vl[13];
		m_PrintCmInfo.OPF = (char)m_pZmSub->owntb->vl[14];
		if( m_PrintCmInfo.OPF == -1 ){
			m_PrintCmInfo.OPF = 0;
		}
		m_PrintCmInfo.OPG = (char)m_pZmSub->owntb->vl[15];
		if( m_PrintCmInfo.OPG == -1 ){
			m_PrintCmInfo.OPG = 0;
		}
		m_PrintCmInfo.OPH = (char)m_pZmSub->owntb->vl[16];
		if( m_PrintCmInfo.OPH == -1 ){
			m_PrintCmInfo.OPH = 0;
		}
		m_PrintCmInfo.OPI = (char)m_pZmSub->owntb->vl[17];
		if( m_PrintCmInfo.OPI == -1 ){
			m_PrintCmInfo.OPI = 0;
		}
		m_PrintCmInfo.PsSigDate = m_pZmSub->owntb->vl[18];
		if( m_PrintCmInfo.PsSigDate <= 0 ){
			m_PrintCmInfo.PsSigDate = 0x0310;
		}
		m_PrintCmInfo.PsSigConame = m_pZmSub->owntb->vl[19];
		if( m_PrintCmInfo.PsSigConame <= 0 ){
			m_PrintCmInfo.PsSigConame = 0x0a20;	
		}

		if( BpState && m_isScoreLinkCall == -1 ){
			::ZeroMemory( &Save_Head , sizeof( Save_Head ) );
			Save_Head[0] = m_pSnHeadData->Sn_TYPE;
			Save_Head[1] = m_pSnHeadData->Sn_SPECIAL;
			Save_Head[2] = m_pSnHeadData->Sn_Sign1;
			Save_Head[3] = m_pSnHeadData->Sn_Sign2;
			Save_Head[4] = m_pSnHeadData->Sn_Sign3;
			Save_Head[5] = m_pSnHeadData->Sn_Sign4;
			Save_Headlong = m_pSnHeadData->Sn_PrintOutSgn;
			Save_Headlong2 = m_pSnHeadData->Sn_PrintOutSgn2;

			memset( numbf, '\0', sizeof( numbf ) );
			strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[0] );
			numno = atoi( numbf );
			m_pSnHeadData->Sn_TYPE = (char)numno;

			memset( numbf, '\0', sizeof( numbf ) );
			strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[1] );
			numno = atoi( numbf );
			m_pSnHeadData->Sn_SPECIAL = (char)numno;

			memset( numbf, '\0', sizeof( numbf ) );
			strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[3] );
			numno = atoi( numbf );
			m_pSnHeadData->Sn_Sign1 = (char)numno;



			memset( numbf, '\0', sizeof( numbf ) );

	

			strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[4] );
			numno = atoi( numbf );
			m_pSnHeadData->Sn_Sign2 = (char)numno;

			
			memset( numbf, '\0', sizeof( numbf ) );
			strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[5] );
			numno = atoi( numbf );
			m_pSnHeadData->Sn_Sign3 = (char)numno;

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[6]);
			numno = atoi(numbf);
			m_pSnHeadData->Sn_Sign4 = (char)numno;
// 24/07/02_361 add -->
			if(Save_Head[5] & 0x01)	{
				m_pSnHeadData->Sn_Sign4 |= 0x01;
			}
// 24/07/02_361 add <--

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[7]);
			numlong = atoi(numbf);
			m_pSnHeadData->Sn_PrintOutSgn = numlong;

			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[8]);
			numlong = atoi(numbf);
			m_pSnHeadData->Sn_PrintOutSgn2 = numlong;
		}

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[10] );
		m_PrintCmInfo.OPJ = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[11] );
		m_PrintCmInfo.OPK = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[12] );
		m_PrintCmInfo.OPL = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[13] );
		m_PrintCmInfo.OPM = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[14] );
		m_PrintCmInfo.OPN = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[15] );
		m_PrintCmInfo.OPO = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[16] );
		m_PrintCmInfo.OPP = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[17] );
		m_PrintCmInfo.OPQ = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[18] );
		m_PrintCmInfo.OPR = atoi( numbf );

		memset( numbf, '\0', sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->str[19] );
		m_PrintCmInfo.OPS = atoi( numbf );

//2016.02.23 INSERT START
		memset( numbf, '\0',sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->code[0] );
		m_PrintCmInfo.OPT = atoi( numbf );
//2016.02.23 INSERT END

//--> '17.03.28 INS START
		if( m_pZmSub->owntb->code[1].GetLength() ){
			memset( numbf, '\0',sizeof( numbf ) );
			strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->code[1] );
			m_PrintCmInfo.OPU = atoi( numbf );
		}
		else{
			m_PrintCmInfo.OPU = 1;
		}
//<-- '17.03.28 INS END
//2018.03.13 INSERT START
		memset( numbf, '\0',sizeof( numbf ) );
		strcpy_s( numbf, (LPCTSTR)m_pZmSub->owntb->code[2] );
		m_PrintCmInfo.OPV = atoi( numbf );
//2018.03.13 INSERT END
		
		if( m_pZmSub->owntb->code[3].GetLength() ){
			memset(numbf, '\0', sizeof(numbf));
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[3]);
			m_PrintCmInfo.OP_GENGO = atoi(numbf);
		}
		else{
			m_PrintCmInfo.OP_GENGO = 1;
		}

// 改良依頼 20-0352 修正 add -->
		if( GetActViewVer() == ID_VER_SYZ31 ) {
			if (m_pZmSub->owntb->code[4].GetLength()) {
				memset(numbf, '\0', sizeof(numbf));
				strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->code[4]);
				m_PrintCmInfo.OPW = atoi(numbf);
			}
			else {
				// 未入力の場合、既存マスターかつ、対応後初回起動時と判断して
				// 申告書本表の[代表者氏名 又は 氏名][代表者氏名又は氏名 フリガナ]により初期値の設定を行う
				// 申告書本表 代表者氏名 又は 氏名 チェック無し
				if(m_PrintCmInfo.OP5 == 1) {
					//課税標準額の内訳書、更正の請求のチェックをOFFにする
					m_PrintCmInfo.OPW |= 0x01;
					m_PrintCmInfo.OPW |= 0x04;
				}
				// 申告書本表 代表者氏名又は氏名 フリガナ チェック無し
				if(m_PrintCmInfo.OPG == 1) {
					//課税標準額の内訳書、更正の請求のチェックをOFFにする
					m_PrintCmInfo.OPW |= 0x02;
					m_PrintCmInfo.OPW |= 0x08;
				}
			}
		}
// 改良依頼 20-0352 修正 add <--

		if( m_pZmSub->owntb->Requery(filter) == -1 ){
			return FALSE;
		}
	}

	memset( m_PrintCmInfo.PsOfceName, '\0', sizeof(m_PrintCmInfo.PsOfceName) );
	GetIcsofnDirectory( m_PrintCmInfo.PsOfceName );

//--> '14.04.30 INS START
	
	if( m_pSnHeadData->Sn_Sign2&0x10 ){
		m_PrintCmInfo.OPsign |= 0x01;
	}
	else{
		m_PrintCmInfo.OPsign &= ~0x01;
	}
//<-- '14.04.30 INS END

	return TRUE;
}

//-----------------------------------------------------------------------------
// 出力情報取得（部数）
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	正常終了
//			FALSE	：	エラー
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::GetPrInitInfo2()
{
	// 印刷部数の読み込み（一括印刷のみ）
	CString					itm_name=_T("");
	int						ii=0,jj=0,kk=0;
	CString					filter;
	EnumIdShBatchPrintState	BpState=ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo={0};

	if(m_pMainFrame)	{
		m_pMainFrame->GetBatchPrintInfo(&BpState,&BpPrintInfo);
	}

	//----> 20240116
	if( m_pMainFrame ){
		m_pMainFrame->GetScoreLinkInfo( m_isScoreLinkCall, m_ScoreLinkPdfNam );
	}
	//<---- 20240116

	if(BpState)	{
		itm_name = GetBusuItmName(BpState,m_Util.GetKazeihoushiki(m_pZmSub->zvol),BpPrintInfo.ForkKey);
		for(ii=0; ii<3; ii++)	{
			filter.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq=%d"),DBSYZSHIN_APLNAME,itm_name,ii+1);
			if(m_pZmSub->owntb->Requery(filter) == -1)	{
				// 新規登録
				m_pZmSub->owntb->AddNew();
				m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
				m_pZmSub->owntb->itm_name = itm_name;
				m_pZmSub->owntb->itm_seq = ii+1;
				for(jj=0; jj<20; jj++)	{
					m_pZmSub->owntb->vl[jj] = 1;	// 部数の初期値は "1"
					m_pZmSub->owntb->str[jj] = _T("");
					m_pZmSub->owntb->code[jj] = _T("");
					if(kk < 50)	{
						m_PrintCmInfo.busu[kk] = (char)m_pZmSub->owntb->vl[jj];
						kk++;
					}
				}
				m_pZmSub->owntb->Update();
			}
			else	{
				for(jj=0; jj<20; jj++)	{
					if(kk < 50)	{
						m_PrintCmInfo.busu[kk] = (char)m_pZmSub->owntb->vl[jj];

// 修正No.1029,1030,168896 cor -->
//// 24/07/10_個別タブから出力 add -->
//						// ※一括印刷　決算期の至年月日が令和1年10月1日以降の場合、
//						// 　特定収入計算表、課税取引金額計算表、税務代理書面の部数に1以上の数字が入力されていれば、個別指定の部数にコピーする。その後部数を１に戻す。
//						//apl_name=DBSYZSHIN and itm_name=DBSyzShin1f0001_gb and itm_seq=2	vl4=課税取引金額計算表,vl5=特定収入計算表,vl6=税務代理書面
//						if(GetActViewVer() == ID_VER_SYZ31)	{	// 消費税10％改正以降
//							if(ii == 1)	{
//								if((jj >= 3 || jj <= 5) && m_pZmSub->owntb->vl[jj] > 1)	{
//									if(jj == 3)	{			// 課税取引金額計算表
//										Sv_busu[0] = m_pZmSub->owntb->vl[jj];
//									}
//									else if(jj == 4)	{	// 特定収入計算表
//										Sv_busu[1] = m_pZmSub->owntb->vl[jj];
//										m_PrintCmInfo.busu[kk] = 1;
//									}
//									else if(jj == 5)	{	// 税務代理書面
//										Sv_busu[2] = m_pZmSub->owntb->vl[jj];
//									}
//									//m_PrintCmInfo.busu[kk] = 1;
//								}
//							}
//						}
//// 24/07/10_個別タブから出力 add <--
// ---------------------------------
						// ※一括印刷　決算期の至年月日が令和1年10月1日以降の場合、
						// 　特定収入計算表、課税取引金額計算表、税務代理書面の部数に1以上の数字が入力されていれば、個別指定の部数にコピーする。その後部数を１に戻す。
						// ---------------------
						// 原則課税		itm_name=DBSyzShin280001_gb and itm_seq=2	vl4=課税取引金額計算表,vl5=特定収入計算表,vl6=税務代理書面
						// 原則課税/2割	itm_name=DBSyzShin280001_gb and itm_seq=2	vl5=課税取引金額計算表,vl6=特定収入計算表,vl7=税務代理書面
						// 簡易課税		itm_name=DBSyzShin280001_kb and itm_seq=2	vl5=課税取引金額計算表,vl6=特定収入計算表,vl7=税務代理書面
						// 簡易課税/2割	itm_name=DBSyzShin280001_kb and itm_seq=2	vl6=課税取引金額計算表,vl7=特定収入計算表,vl8=税務代理書面

						if(ii == 1)	{
							long	eymd = 0;
							if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
							else								eymd = m_pSnHeadData->Sn_KDAYE;

							if(GetActViewVer() == ID_VER_SYZ31)	{	// 消費税10％改正以降
								if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{	//２割特例計算を使用する
									if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{	// 原則課税
										if(jj == 4 && m_pZmSub->owntb->vl[jj] > 1)	{		// 課税取引金額計算表
											Sv_busu[0] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 5 && m_pZmSub->owntb->vl[jj] > 1)	{	// 特定収入計算表
											//Sv_busu[1] = m_pZmSub->owntb->vl[jj];
											Sv_busu[1] = 1;
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 6 && m_pZmSub->owntb->vl[jj] > 1)	{	// 税務代理書面
											Sv_busu[2] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
									}
									else	{															// 簡易課税
										if(jj == 5 && m_pZmSub->owntb->vl[jj] > 1)	{		// 課税取引金額計算表
											Sv_busu[0] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 6 && m_pZmSub->owntb->vl[jj] > 1)	{	// 特定収入計算表
											//Sv_busu[1] = m_pZmSub->owntb->vl[jj];
											Sv_busu[1] = 1;
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 7 && m_pZmSub->owntb->vl[jj] > 1)	{	// 税務代理書面
											Sv_busu[2] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
									}
								}
								else	{																		//２割特例計算を使用しない
									if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{	// 原則課税
										if(jj == 3 && m_pZmSub->owntb->vl[jj] > 1)	{		// 課税取引金額計算表
											Sv_busu[0] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 4 && m_pZmSub->owntb->vl[jj] > 1)	{	// 特定収入計算表
											Sv_busu[1] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 5 && m_pZmSub->owntb->vl[jj] > 1)	{	// 税務代理書面
											Sv_busu[2] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
									}
									else	{															// 簡易課税
										if(jj == 4 && m_pZmSub->owntb->vl[jj] > 1)	{		// 課税取引金額計算表
											Sv_busu[0] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 5 && m_pZmSub->owntb->vl[jj] > 1)	{	// 特定収入計算表
											//Sv_busu[1] = m_pZmSub->owntb->vl[jj];
											Sv_busu[1] = 1;
											m_PrintCmInfo.busu[kk] = 1;
										}
										else if(jj == 6 && m_pZmSub->owntb->vl[jj] > 1)	{	// 税務代理書面
											Sv_busu[2] = m_pZmSub->owntb->vl[jj];
											m_PrintCmInfo.busu[kk] = 1;
										}
									}
								}
							}
						}
// 修正No.1029,1030,168896 cor <--


						kk++;
					}
				}
			}
		}
	}
	else	{
		for(kk=0; kk<50; kk++)	{
			m_PrintCmInfo.busu[kk] = 1;
		}
	}
	// 起動時の部数を保存しておく
	// （理由：印刷が終わったあと初期値に戻すため）
	for(kk=0; kk<50; kk++)	{
		m_BusuSave[kk] = m_PrintCmInfo.busu[kk];
	}

	return(TRUE);
}

// 24/02/19wd_特定収入対応 add --> 20240307 各個別対応に対応
//-----------------------------------------------------------------------------
// 出力情報を取得（特定収入計算表 個別出力設定）
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	正常終了
//			FALSE	：	エラー
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::GetPrnExSubInfo(void)
{
	int					ii=0, jj=0;
	int					busu;				// 部数
	int					stpg, edpg;			// 開始頁と終了頁
	int					opsw=0;
	long				eymd = 0;
	CString				strSQL,strSQL2;
	CString				sData;
	CRecordset			rs(m_pZmSub->m_database);

	// 消費税10％改正以前
	if(GetActViewVer() != ID_VER_SYZ31)			return(TRUE);

	//（課税取引金額計算表＋税務代理書）//ownertblに保存

	EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo = { 0 };

// 24/07/10_個別タブから出力 add -->
	if(m_pMainFrame)	{
		m_pMainFrame->GetBatchPrintInfo(&BpState, &BpPrintInfo);
	}
// 24/07/10_個別タブから出力 add <--

	if (BpState) {	// 一括印刷
		// オーナーテーブルオープン
		if (m_pZmSub->owntb == NULL) {
			if (m_pZmSub->OwnTblOpen() == -1) {
				return(FALSE);
			}
			opsw = 1;
		}
	}

	CString		itm_name = _T("");
	CString		filter;
// 24/07/10_個別タブから出力 add -->
	CString		filter2;
	long		l_vl[20];
	CString		l_str[20];
	CString		l_code[20];
// 24/07/10_個別タブから出力 add <--
	char		numbf[16] = { 0 };

	if (BpState) {
		//一括印刷
		itm_name = GetBusuItmName(BpState, m_Util.GetKazeihoushiki(m_pZmSub->zvol), BpPrintInfo.ForkKey);
		//item_seq 999を使用
		filter.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, itm_name, 1000 - 1);
	}
	else {
		filter.Format(_T("apl_name='%s' and itm_name='%s' "), DBSYZSHIN_APLNAME, DBSYZSHIN_ITMNAME_SUB);
	}

	if (m_pZmSub->owntb->Requery(filter) == -1) {
// 24/07/10_個別タブから出力 cor -->
//		// 新規登録
//		m_pZmSub->owntb->AddNew();
//		m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
//		if (BpState) {
//			m_pZmSub->owntb->itm_name = itm_name;
//			m_pZmSub->owntb->itm_seq = 999;
//		}
//		else {
//			m_pZmSub->owntb->itm_name = DBSYZSHIN_ITMNAME_SUB;
//			m_pZmSub->owntb->itm_seq = 0;
//		}
//		//
//		m_pZmSub->owntb->vl[0] = 0;
//		m_pZmSub->owntb->vl[1] = 0;
//		// 課税取引金額計算表　事業所得用
//		if (m_pSnHeadData->Sn_PrintOutSgn & 0x200000) 	m_pZmSub->owntb->vl[0] |= 0x01;
//		// 課税取引金額計算表　農業所得用 
//		if (m_pSnHeadData->Sn_PrintOutSgn & 0x800000) 	m_pZmSub->owntb->vl[0] |= 0x02;
//		// 課税取引金額計算表　不動産所得用
//		if (m_pSnHeadData->Sn_PrintOutSgn & 0x400000) 	m_pZmSub->owntb->vl[0] |= 0x04;
//		// 課税売上高計算表
//		if (m_pSnHeadData->Sn_PrintOutSgn & 0x80000) 	m_pZmSub->owntb->vl[0] |= 0x08;
//		// 課税仕入高計算表 
//		if (m_pSnHeadData->Sn_PrintOutSgn & 0x100000) 	m_pZmSub->owntb->vl[0] |= 0x10;
//		// 事業別売上高
//		if (m_pSnHeadData->Sn_PrintOutSgn & 0x2000000) 	m_pZmSub->owntb->vl[0] |= 0x20;
//		// 課税取引金額計算表 チェック
//		if (m_pZmSub->owntb->vl[0]) {
//			//初期値は部数0としておく　過去の最大値などにする場合はBusuCheckAndPutを調べる
//			m_pSnHeadData->Sn_PrintOutSgn |= 0x40000000;
//			//課税取引金額計算表 部数
//			memset(numbf, '\0', sizeof(numbf));
//			sprintf_s(numbf, sizeof(numbf), _T("%d"), 1);
//			m_pZmSub->owntb->str[0] = numbf;
//			//税理士添付書面 部数
//			memset(numbf, '\0', sizeof(numbf));
//			sprintf_s(numbf, sizeof(numbf), _T("%d"), 0);
//			m_pZmSub->owntb->str[1] = numbf;
//
//		}
//		// 税理士添付書面は今までなかったため０
//		for (ii = 0; ii < 20; ii++) {
//// 24/04/03_checkon2 cor -->
//			//if (ii > 1) {
//// -------------------------
//			if(ii >= 1)	{
//// 24/04/03_checkon2 cor <--
//				m_pZmSub->owntb->vl[ii] = -1;	// 初期値は "-1"
//				m_pZmSub->owntb->str[ii] = _T("");
//			}
//			m_pZmSub->owntb->code[ii] = _T("");
//		}
//		m_pZmSub->owntb->Update();
//		m_pZmSub->owntb->Requery(filter);
// ---------------------------------
		if(BpState)		{	// 一括印刷から呼ばれている
			// ※一括印刷用の設定がちゃんと保存されていなかったので、一旦通常印刷のデータを取得してセットする。
			// 無かったら、初期値をセット

			// 新規登録
			filter2.Format(_T("apl_name='%s' and itm_name='%s' "), DBSYZSHIN_APLNAME, DBSYZSHIN_ITMNAME_SUB);
			if(m_pZmSub->owntb->Requery(filter2) == -1)	{
				m_pZmSub->owntb->AddNew();

				m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
				m_pZmSub->owntb->itm_name = itm_name;
				m_pZmSub->owntb->itm_seq = 999;

				m_pZmSub->owntb->vl[0] = 0;
				m_pZmSub->owntb->vl[1] = 0;
				// 課税取引金額計算表　事業所得用
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x200000) 	m_pZmSub->owntb->vl[0] |= 0x01;
				// 課税取引金額計算表　農業所得用 
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x800000) 	m_pZmSub->owntb->vl[0] |= 0x02;
				// 課税取引金額計算表　不動産所得用
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x400000) 	m_pZmSub->owntb->vl[0] |= 0x04;
				// 課税売上高計算表
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x80000) 	m_pZmSub->owntb->vl[0] |= 0x08;
				// 課税仕入高計算表 
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x100000) 	m_pZmSub->owntb->vl[0] |= 0x10;
				// 事業別売上高
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x2000000) 	m_pZmSub->owntb->vl[0] |= 0x20;
				// 課税取引金額計算表 チェック
				if(m_pZmSub->owntb->vl[0]) {
					//初期値は部数0としておく　過去の最大値などにする場合はBusuCheckAndPutを調べる
					m_pSnHeadData->Sn_PrintOutSgn |= 0x40000000;
					//課税取引金額計算表 部数
					memset(numbf, '\0', sizeof(numbf));
					sprintf_s(numbf, sizeof(numbf), _T("%d"), 1);
					m_pZmSub->owntb->str[0] = numbf;
					//税理士添付書面 部数
					memset(numbf, '\0', sizeof(numbf));
					sprintf_s(numbf, sizeof(numbf), _T("%d"), 0);
					m_pZmSub->owntb->str[1] = numbf;
				}
				// 税理士添付書面は今までなかったため０
				for(ii = 0; ii < 20; ii++) {
					if(ii >= 1) {
						m_pZmSub->owntb->vl[ii] = -1;	// 初期値は "-1"
						m_pZmSub->owntb->str[ii] = _T("");
					}
					m_pZmSub->owntb->code[ii] = _T("");
				}
			}
			else {
				for(ii = 0; ii < 20; ii++) {
					l_vl[ii] = m_pZmSub->owntb->vl[ii];
					l_str[ii] = m_pZmSub->owntb->str[ii];
					l_code[ii] = m_pZmSub->owntb->code[ii];
				}

				m_pZmSub->owntb->AddNew();

				m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
				m_pZmSub->owntb->itm_name = itm_name;
				m_pZmSub->owntb->itm_seq = 999;

				for(ii = 0; ii < 20; ii++) {
					m_pZmSub->owntb->vl[ii] = l_vl[ii];
					m_pZmSub->owntb->str[ii] = l_str[ii];
					m_pZmSub->owntb->code[ii] = l_code[ii];
				}
			}

			m_pZmSub->owntb->Update();
			m_pZmSub->owntb->Requery(filter);
		}
		else	{	// 通常出力
			// 新規登録
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
			m_pZmSub->owntb->itm_name = DBSYZSHIN_ITMNAME_SUB;
			m_pZmSub->owntb->itm_seq = 0;
			//
			m_pZmSub->owntb->vl[0] = 0;
			m_pZmSub->owntb->vl[1] = 0;
			// 課税取引金額計算表　事業所得用
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x200000) 	m_pZmSub->owntb->vl[0] |= 0x01;
			// 課税取引金額計算表　農業所得用 
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x800000) 	m_pZmSub->owntb->vl[0] |= 0x02;
			// 課税取引金額計算表　不動産所得用
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x400000) 	m_pZmSub->owntb->vl[0] |= 0x04;
			// 課税売上高計算表
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x80000) 	m_pZmSub->owntb->vl[0] |= 0x08;
			// 課税仕入高計算表 
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x100000) 	m_pZmSub->owntb->vl[0] |= 0x10;
			// 事業別売上高
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x2000000) 	m_pZmSub->owntb->vl[0] |= 0x20;
			// 課税取引金額計算表 チェック
			if(m_pZmSub->owntb->vl[0]) {
				//初期値は部数0としておく　過去の最大値などにする場合はBusuCheckAndPutを調べる
				m_pSnHeadData->Sn_PrintOutSgn |= 0x40000000;
				//課税取引金額計算表 部数
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), 1);
				m_pZmSub->owntb->str[0] = numbf;
				//税理士添付書面 部数
				memset(numbf, '\0', sizeof(numbf));
				sprintf_s(numbf, sizeof(numbf), _T("%d"), 0);
				m_pZmSub->owntb->str[1] = numbf;

			}
			// 税理士添付書面は今までなかったため０
			for(ii = 0; ii < 20; ii++) {
				if(ii >= 1) {
					m_pZmSub->owntb->vl[ii] = -1;	// 初期値は "-1"
					m_pZmSub->owntb->str[ii] = _T("");
				}
				m_pZmSub->owntb->code[ii] = _T("");
			}

			m_pZmSub->owntb->Update();
			m_pZmSub->owntb->Requery(filter);
		}
// 24/07/10_個別タブから出力 cor <--
	}

	int numnoKaz = 0, numnoZei = 0;

	//課税取引金額計算表 部数
// 24/07/10_個別タブから出力 cor -->
	//strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[0]);
	//numnoKaz = atoi(numbf);
	//for (int i = 0; i < 7; i++) {
	//	::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
	//	m_PrnEXSub_KAZ.check = IsBitFlg(m_pZmSub->owntb->vl[0], i);
	//	m_PrnEXSub_KAZ.busu = numnoKaz;
	//	m_PrnEXSub_KAZ.stpage = -1;
	//	m_PrnEXSub_KAZ.edpage = -1;
	//	m_pMapPrnEXSub_KAZ.SetAt(i, m_PrnEXSub_KAZ);
	//}
// ---------------------------------
	char	bf1[256],bf2[256];
// 修正No.168896,1030 add -->
	int		op_sw1 = 0;
	int		kflg = 0;
	int		Ktype = 0;	// 0:原則課税　1:簡易課税　2:簡易課税（非連動）

	if(m_pZmSub->zvol == NULL)	{
		m_pZmSub->VolumeOpen();
		op_sw1 = 1;
	}

	//タイプ判別
	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税
		if(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
			Ktype = 2;
		}
		else	{
			Ktype = 1;
		}
	}
// 修正No.168896,1030 add <--

	memset(bf2, 0, sizeof(bf2));
	for(ii=0, jj=0; ii<7; ii++, jj+=2)	{
		::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
		m_PrnEXSub_KAZ.check = IsBitFlg(m_pZmSub->owntb->vl[0], ii);

		if(GetActViewVer() == ID_VER_SYZ31) {	// 消費税10％改正以降
			// Sv_busu[0]	課税取引金額計算表
// 修正No.168896,1030 cor -->
			//if(Sv_busu[0] > 1 && _tstoi(m_pZmSub->owntb->str[2]) == 0)	{
			//	// 一括印刷で申告書出力設定側の部数に「１」以上の数値が入力されていた場合は、こっちを優先
			//	m_PrnEXSub_KAZ.busu = Sv_busu[0];
			//}
// --------------------------
			if(Sv_busu[0] > 1)	{
				// 一括印刷で申告書出力設定側の部数に「１」以上の数値が入力されていた場合は、こっちを優先
				kflg = 0;
				if(ii == 0)	{			// 課税取引金額計算表　事業所得用
					if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
						//if(Ktype == 0)	{									// 修正No.168921 del
						if(!(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)))	{	// 修正No.168921 add
							kflg = (m_pZmSub->zvol->ind_type & 0x01);
						}
					}
				}
				else if(ii == 1)	{	// 課税取引金額計算表　農業所得用 
					if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
						//if(Ktype == 0)	{									// 修正No.168921 del
						if(!(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)))	{	// 修正No.168921 add
							kflg = (m_pZmSub->zvol->ind_type & 0x04);
						}
					}
				}
				else if(ii == 2)	{	// 課税取引金額計算表　不動産所得用
					if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
						//if(Ktype == 0)	{									// 修正No.168921 del
						if(!(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)))	{	// 修正No.168921 add
							kflg = (m_pZmSub->zvol->ind_type & 0x02);
						}
					}
				}
				else if(ii == 3)	{	// 課税売上高計算表
					if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
						kflg = 1;
					}
				}
				else if(ii == 4)	{	// 課税仕入高計算表／事業別売上高
					if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
						// 原則課税
						if(Ktype == 0) {
							if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
							else								eymd = m_pSnHeadData->Sn_KDAYE;
							if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{	// ２割特例計算を使用する
							}
							else	{
								kflg = 1;
							}
						}
						// 簡易課税＆非連動
						else if (Ktype == 2) {
							kflg = 1;
						}
					}
				}

				if(kflg)	m_PrnEXSub_KAZ.busu = Sv_busu[0];
				else		m_PrnEXSub_KAZ.busu = 1;
			}
// 修正No.168896,1030 cor <--
			else {
				//  1  2  3  4  5  6  7
				// 99 99 99 99 99 99 99
				// 12345678901234 <- 14桁
				// 99999999999999
				memset(bf1, 0, sizeof(bf1));
				sprintf_s(bf1, _T("%s"), m_pZmSub->owntb->str[2]);
				memcpy(bf2, &bf1[jj], 2);
				if(atoi(bf2) <= 0)	m_PrnEXSub_KAZ.busu = 1;
				else				m_PrnEXSub_KAZ.busu = atoi(bf2);
			}
		}
		else	{
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[0]);
			numnoKaz = atoi(numbf);
			m_PrnEXSub_KAZ.busu = numnoKaz;
		}

		m_PrnEXSub_KAZ.stpage = -1;
		m_PrnEXSub_KAZ.edpage = -1;
		m_pMapPrnEXSub_KAZ.SetAt(ii, m_PrnEXSub_KAZ);
	}

// 修正No.168896,1030 add -->
	if(op_sw1 == 1) {
		m_pZmSub->VolumeClose();
	}
// 修正No.168896,1030 add <--
// 24/07/10_個別タブから出力 cor <--

	//税理士添付書面 部数
// 修正No.168896,1030 add -->
	int									yflg = 0;
	int									op_sw2 = 0;
	int									yno=0, bunb=0;
	CArray<TAXAGENT_STFF,TAXAGENT_STFF>	car;

	if(m_pZmSub->zvol == NULL) {
		m_pZmSub->VolumeOpen();
		op_sw2 = 1;
	}
	// 呼び出し元業務が非連動かどうか
	if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg=1;
	if(op_sw2 == 1) {
		m_pZmSub->VolumeClose();
	}
// 修正No.168896,1030 add <--

// 24/07/10_個別タブから出力 cor -->
	//strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[1]);
	//numnoZei = atoi(numbf);

	//for (int i = 0; i < 11; i++) {
	//	::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
	//	m_PrnEXSub_ZEI.check = IsBitFlg(m_pZmSub->owntb->vl[1], i);
	//	m_PrnEXSub_ZEI.busu = numnoZei;
	//	m_PrnEXSub_ZEI.stpage = -1;
	//	m_PrnEXSub_ZEI.edpage = -1;
	//	m_pMapPrnEXSub_ZEI.SetAt(i, m_PrnEXSub_ZEI);
	//}
// ---------------------------------
	memset(bf2, 0, sizeof(bf2));
	for(ii=0, jj=0; ii<11; ii++, jj+=2)	{
		::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
		m_PrnEXSub_ZEI.check = IsBitFlg(m_pZmSub->owntb->vl[1], ii);

		if(GetActViewVer() == ID_VER_SYZ31) {	// 消費税10％改正以降
			// Sv_busu[2]	税務代理書面
// 修正No.168896,1030 cor -->
			//if(Sv_busu[2] > 1 && _tstoi(m_pZmSub->owntb->str[3]) == 0)	{
			//	m_PrnEXSub_ZEI.busu = Sv_busu[2];
			//}
// --------------------------
			if(Sv_busu[2] > 1)	{
				if(ii == 0)			yno = 0x01;
				else if(ii == 1)	yno = 0x02;
				else if(ii == 2)	yno = 0x04;
				else if(ii == 3)	yno = 0x08;
				else if(ii == 4)	yno = 0x10;
				else if(ii == 5)	yno = 0x20;
				else if(ii == 6)	yno = 0x40;
				else if(ii == 7)	yno = 0x80;
				else if(ii == 8)	yno = 0x100;
				else if(ii == 9)	yno = 0x200;
				else				yno = 0x400;
				bunb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bunb == 0)	m_PrnEXSub_ZEI.busu = 1;
				else			m_PrnEXSub_ZEI.busu = Sv_busu[2];
			}
// 修正No.168896,1030 cor <--
			else	{
				//  1  2  3  4  5  6  7  8  9 10 11
				// 99 99 99 99 99 99 99 99 99 99 99
				// 1234567890123456789012 <-22桁
				// 9999999999999999999999
				memset(bf1, 0, sizeof(bf1));
				sprintf_s(bf1, _T("%s"), m_pZmSub->owntb->str[3]);
				memcpy(bf2, &bf1[jj], 2);
				if(atoi(bf2) <= 0)	m_PrnEXSub_ZEI.busu = 1;
				else				m_PrnEXSub_ZEI.busu = atoi(bf2);
			}
		}
		else	{
			strcpy_s(numbf, (LPCTSTR)m_pZmSub->owntb->str[1]);
			numnoZei = atoi(numbf);
			m_PrnEXSub_ZEI.busu = numnoZei;
		}

		m_PrnEXSub_ZEI.stpage = -1;
		m_PrnEXSub_ZEI.edpage = -1;
		m_pMapPrnEXSub_ZEI.SetAt(ii, m_PrnEXSub_ZEI);
	}
// 24/07/10_個別タブから出力 cor <--

	//課税取引金額計算表　ボタン
	Kazei_DButton_sw = m_pZmSub->owntb->vl[2];
	//特定収入計算表　ボタン
	Toku_DButton_sw = m_pZmSub->owntb->vl[3];
	//税理士添付書面　ボタン
	Zeiri_DButton_sw = m_pZmSub->owntb->vl[4];

// 24/04/01_246 add -->
	// 初回はデフォルト値をセットする
	// ※課税取引金額計算表、特定収入計算表の「会社コードを出力する」は従前からのもの
	// （SH_PRINT_CMINFO::OPL）を使用している
	if(Zeiri_DButton_sw < 0)	{
		Zeiri_DButton_sw = 0;
		// 印刷項目　依頼者：氏名又は名称				ON
		Zeiri_DButton_sw = Zeiri_DButton_sw | 0x01;
		// 印刷項目　依頼者：住所又は事業所の所在地		ON
		Zeiri_DButton_sw = Zeiri_DButton_sw | 0x02;
		// 電子申告済みを印字する						OFF
	}
// 24/04/01_246 add <--

	//特定収入計算表　 非営利法人以外は必要ないのでReturn
	if((m_pZmSub->zvol->apno&0xf0) <= 0x30)	return(TRUE);

// 修正No.168714 add -->
	// 令和3年12月31日以降の課税期間
// 24/04/19特定収入追加 cor -->
	//long					eymd = 0;
	//BOOL					syzSign = FALSE;
	//if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	//else								eymd = m_pSnHeadData->Sn_KDAYE;
	//if((m_pSnHeadData->SVmzsw == 1) || (eymd < R03YEAR))	{
	//	// 旧税率ありもしくは令和3年12月1日より前
	//	syzSign = TRUE;
	//}
	//if(syzSign == TRUE || eymd < ICS_SH_INVOICE_DAY)	{
	//	return(TRUE);
	//}
// ----------------------------
	//long					eymd = 0;
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
// 修正No.168737 del -->
	//if(eymd < ICS_SH_INVOICE_DAY)	{
	//	return(TRUE);
	//}
// 修正No.168737 del <--
// 24/04/19特定収入追加 cor <--
	// 簡易課税
	if((m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01)	{
		return(TRUE);
	}
// 修正No.168714 add <--

	//EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	//SH_BPPRINT_INFO			BpPrintInfo = {0};

// 24/07/10_個別タブから出力 del -->
	//if(m_pMainFrame)	{
	//	m_pMainFrame->GetBatchPrintInfo(&BpState, &BpPrintInfo);
	//}
// 24/07/10_個別タブから出力 del <--

	long				flg=0;
	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	pTaxlist31View->IsSpecificEnable(&flg);

	if(BpState)	{	// 一括印刷
		// オーナーテーブルオープン
		if(m_pZmSub->owntb == NULL) {
			if(m_pZmSub->OwnTblOpen() == -1) {
				return(FALSE);
			}
			opsw = 1;
		}

		// オーナーテーブルから一括印刷情報の取得
		strSQL.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, BpPrintInfo.ForkKey, 1000);
		if(m_pZmSub->owntb->Requery(strSQL) == -1)	{		// 新規作成
// 24/07/10_個別タブから出力 cor -->
			// 部数（※最初は常に「1」を設定）
			busu = 1;
// ---------------------------------
			//// 部数
			//// Sv_busu[1]	特定収入計算表
			//if(Sv_busu[1] > 1)	busu = Sv_busu[1];
			//else					busu = 1;
// 24/07/10_個別タブから出力 cor <--
			// 開始頁と終了頁を取得
			stpg = 1;
			//edpg = 200;	// 修正No.168717 del
			edpg = 1;		// 修正No.168717 add

			// splistテーブルから、チェックの情報を取得
			// 部数、開始頁、終了頁はデフォルトをセット
			for(ii=0; ii<16; ii++)	{
				strSQL2  = _T("select spitln from splist ");
				strSQL2.Format(strSQL2 + _T("where spitem = 1000 and spitcl = %d"), ii);
				try	{
					rs.Open(CRecordset::forwardOnly, strSQL2);
					// 取得
					sData.Empty();
					//while(rs.IsEOF() == FALSE)	{
					if(rs.IsEOF() == FALSE)	{
						rs.GetFieldValue((short)0, sData);
						rs.MoveNext();
					}
					else	{
						// 初回起動時、特定収入計算表にチェックが付いていれば個別もＯＮにする
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000000)	{
// 修正No.168896,1030 cor -->
							//if(ii == 1 && (flg & 0x0001))	sData=_T("1");
							//if(ii == 2 && (flg & 0x0002))	sData=_T("1");
							//if(ii == 3 && (flg & 0x0004))	sData=_T("1");
							//if(ii == 4 && (flg & 0x0008))	sData=_T("1");
							//if(ii == 5 && (flg & 0x0010))	sData=_T("1");
							//if(ii == 6 && (flg & 0x0020))	sData=_T("1");
							//if(ii == 7 && (flg & 0x0040))	sData=_T("1");
							//if(ii == 8 && (flg & 0x0080))	sData=_T("1");
							//if(ii == 9 && (flg & 0x0100))	sData=_T("1");
							//if(ii ==10 && (flg & 0x0200))	sData=_T("1");
							//if(ii ==11 && (flg & 0x0400))	sData=_T("1");
							//if(ii ==12 && (flg & 0x0800))	sData=_T("1");
							//if(ii ==13 && (flg & 0x1000))	sData=_T("1");
							//if(ii ==14 && (flg & 0x2000))	sData=_T("1");
							//if(ii ==15 && (flg & 0x4000))	sData=_T("1");
// --------------------------
							if(ii == 1 && (flg & 0x0001))	{
								sData = _T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 2 && (flg & 0x0002))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 3 && (flg & 0x0004))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 4 && (flg & 0x0008))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 5 && (flg & 0x0010))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 6 && (flg & 0x0020))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 7 && (flg & 0x0040))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 8 && (flg & 0x0080))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii == 9 && (flg & 0x0100))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii ==10 && (flg & 0x0200))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii ==11 && (flg & 0x0400))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii ==12 && (flg & 0x0800))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii ==13 && (flg & 0x1000))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii ==14 && (flg & 0x2000))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
							if(ii ==15 && (flg & 0x4000))	{
								sData=_T("1");
								if(Sv_busu[1] > 1)	busu = Sv_busu[1];
							}
// 修正No.168896,1030 cor <--
						}
					}
					rs.Close();
				}
				catch(CDBException* dbe)	{
					ICSMessageBox(dbe->m_strError);
					dbe->Delete();
					rs.Close();
					return(FALSE);
				}

				::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
				switch(ii)	{
					case 0:		// 個別に出力設定を行う
						m_PrnEXSub.check = _tstoi(sData);
						break;
					case 1:		// 計算表１
					case 2:		// 計算表２(１)
					case 3:		// 計算表２(２)
					case 4:		// 計算表３、４
					case 5:		// 計算表５(１)
					case 6:		// 計算表５(２)
					case 7:		// 計算表５(３)
						m_PrnEXSub.check = _tstoi(sData);
						m_PrnEXSub.busu = busu;
						break;
					case 8:		// 計算表５－２(１)
					case 9:		// 計算表５－２(２)
					case 10:	// 計算表５－２(３)－１
					case 11:	// 計算表５－２(３)－２
					case 12:	// 計算表５－２(３)－３
					case 13:	// 計算表５－２(４)－１
					case 14:	// 計算表５－２(４)－２
					case 15:	// 計算表５－２(４)－３
						m_PrnEXSub.check = _tstoi(sData);
						m_PrnEXSub.busu = busu;
						m_PrnEXSub.stpage = stpg;
						m_PrnEXSub.edpage = edpg;
						break;
					default:
						break;
				}

				m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
			}
		}
		else	{			// 一括印刷データ作成済
			m_pMapPrnEXSub.RemoveAll();

			for(ii=1; ii<3; ii++)	{
				for(jj=0; jj<16; jj++)	{
					::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
					m_pMapPrnEXSub.Lookup(jj, m_PrnEXSub);
					if(ii == 1) {
						m_PrnEXSub.check = m_pZmSub->owntb->vl[jj];
// 修正No.168896,1030 cor -->
//// 24/07/10_個別タブから出力 cor -->
//						//m_PrnEXSub.busu = _tstoi(m_pZmSub->owntb->str[jj]);
//// ---------------------------------
//						// Sv_busu[1]	特定収入計算表
//						if(Sv_busu[1] > 1)	m_PrnEXSub.busu = Sv_busu[1];
//						else				m_PrnEXSub.busu = _tstoi(m_pZmSub->owntb->str[jj]);
//// 24/07/10_個別タブから出力 cor <--
// --------------------------
						// Sv_busu[1]	特定収入計算表
						if(Sv_busu[1] > 1)	{
							m_PrnEXSub.busu = 1;
							if(jj == 1 && (flg & 0x0001))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 2 && (flg & 0x0002))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 3 && (flg & 0x0004))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 4 && (flg & 0x0008))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 5 && (flg & 0x0010))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 6 && (flg & 0x0020))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 7 && (flg & 0x0040))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 8 && (flg & 0x0080))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj == 9 && (flg & 0x0100))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj ==10 && (flg & 0x0200))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj ==11 && (flg & 0x0400))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj ==12 && (flg & 0x0800))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj ==13 && (flg & 0x1000))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj ==14 && (flg & 0x2000))	m_PrnEXSub.busu = Sv_busu[1];
							if(jj ==15 && (flg & 0x4000))	m_PrnEXSub.busu = Sv_busu[1];
						}
						else	{
							m_PrnEXSub.busu = _tstoi(m_pZmSub->owntb->str[jj]);
						}
// 修正No.168896,1030 cor <--
					}
					else {
						m_PrnEXSub.stpage = m_pZmSub->owntb->vl[jj];
						m_PrnEXSub.edpage = _tstoi(m_pZmSub->owntb->str[jj]);
					}
					m_pMapPrnEXSub.SetAt(jj, m_PrnEXSub);
				}

				strSQL.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, BpPrintInfo.ForkKey, 1000+ii);
				if(m_pZmSub->owntb->Requery(strSQL) == -1)	{
					break;
				}
			}
		}

		if(opsw == 1){
			m_pZmSub->OwnTblClose();
		}
	}
	else	{		// 通常出力
		// 特定収入計算表 個別出力設定用のマップをクリア
		m_pMapPrnEXSub.RemoveAll();

		// 部数（※最初は常に「1」を設定）
		busu = 1;

		// 開始頁と終了頁を取得
		stpg = 1;
		//edpg = 200;	// 修正No.168717 del
		edpg = 1;		// 修正No.168717 add
		
		// splistテーブルから情報を取得
		for(ii=0; ii<16; ii++)	{
			strSQL  = _T("select spitln from splist ");
			strSQL.Format(strSQL + _T("where spitem = 1000 and spitcl = %d"), ii);
			try	{
				rs.Open(CRecordset::forwardOnly, strSQL);
				// 取得
				sData.Empty();
				//while(rs.IsEOF() == FALSE)	{
				if(rs.IsEOF() == FALSE)	{
					rs.GetFieldValue((short)0, sData);
					rs.MoveNext();
				}
				else	{
					// 初回起動時、特定収入計算表にチェックが付いていれば個別もＯＮにする
					if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000000)	{
						if(ii == 1 && (flg & 0x0001))	sData=_T("1");
						if(ii == 2 && (flg & 0x0002))	sData=_T("1");
						if(ii == 3 && (flg & 0x0004))	sData=_T("1");
						if(ii == 4 && (flg & 0x0008))	sData=_T("1");
						if(ii == 5 && (flg & 0x0010))	sData=_T("1");
						if(ii == 6 && (flg & 0x0020))	sData=_T("1");
						if(ii == 7 && (flg & 0x0040))	sData=_T("1");
						if(ii == 8 && (flg & 0x0080))	sData=_T("1");
						if(ii == 9 && (flg & 0x0100))	sData=_T("1");
						if(ii ==10 && (flg & 0x0200))	sData=_T("1");
						if(ii ==11 && (flg & 0x0400))	sData=_T("1");
						if(ii ==12 && (flg & 0x0800))	sData=_T("1");
						if(ii ==13 && (flg & 0x1000))	sData=_T("1");
						if(ii ==14 && (flg & 0x2000))	sData=_T("1");
						if(ii ==15 && (flg & 0x4000))	sData=_T("1");
					}
				}
				rs.Close();
			}
			catch(CDBException* dbe)	{
				ICSMessageBox(dbe->m_strError);
				dbe->Delete();
				rs.Close();
				return(FALSE);
			}

			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));

			switch(ii)	{
				case 0:		// 個別に出力設定を行う
					m_PrnEXSub.check = _tstoi(sData);
					break;

				case 1:		// 計算表１
				case 2:		// 計算表２(１)
				case 3:		// 計算表２(２)
				case 4:		// 計算表３、４
				case 5:		// 計算表５(１)
				case 6:		// 計算表５(２)
				case 7:		// 計算表５(３)
					m_PrnEXSub.check = _tstoi(sData);
					m_PrnEXSub.busu = busu;
					break;

				case 8:		// 計算表５－２(１)
				case 9:		// 計算表５－２(２)
				case 10:	// 計算表５－２(３)－１
				case 11:	// 計算表５－２(３)－２
				case 12:	// 計算表５－２(３)－３
				case 13:	// 計算表５－２(４)－１
				case 14:	// 計算表５－２(４)－２
				case 15:	// 計算表５－２(４)－３
					m_PrnEXSub.check = _tstoi(sData);
					m_PrnEXSub.busu = busu;
					m_PrnEXSub.stpage = stpg;
					m_PrnEXSub.edpage = edpg;
					break;

				default:
					break;
			}

			m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
		}
	}
// 24/02/27wd_特定収入対応 cor <--

	return(TRUE);
}

// 24/02/27wd_特定収入対応 add -->
//-----------------------------------------------------------------------------
//	（通常出力用）
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::GetPrnExSubInfo2(void)
{
	int						ii=0;
	int						busu;				// 部数
	int						stpg, edpg;			// 開始頁と終了頁
	int						opsw=0;
	CString					strSQL;
	EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo = {0};
// 修正No.160353 add -->
	long					eymd = 0;
// 24/04/19特定収入追加 del -->
	//BOOL					syzSign = FALSE;
// 24/04/19特定収入追加 del <--

// 24/07/10_個別タブから出力 del -->
	//// 非営利法人以外
	//if((m_pZmSub->zvol->apno & 0xf0) <= 0x30)	return(TRUE);
// 24/07/10_個別タブから出力 del <--
	// 消費税10％改正以前
	if(GetActViewVer() != ID_VER_SYZ31)			return(TRUE);

// 24/07/10_個別タブから出力 add -->
	if(m_pMainFrame)	{
		m_pMainFrame->GetBatchPrintInfo(&BpState, &BpPrintInfo);
	}

	// 通常出力の時、 課税取引金額計算表、税務代理権限証書の部数をリセットする
	if( !BpState )	{	// 通常出力
		// 課税取引金額計算表
		if((int)m_pMapPrnEXSub_KAZ.GetCount() > 0)	{
			for(ii=0; ii<7; ii++)	{
				::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);
				m_PrnEXSub_KAZ.busu = 1;
				m_pMapPrnEXSub_KAZ.SetAt(ii, m_PrnEXSub_KAZ);
			}
		}
		// 税務代理権限証書
		if((int)m_pMapPrnEXSub_ZEI.GetCount() > 0)	{
			for(ii=0; ii<11; ii++)	{
				::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);
				m_PrnEXSub_ZEI.busu = 1;
				m_pMapPrnEXSub_ZEI.SetAt(ii, m_PrnEXSub_ZEI);
			}
		}
	}

	// 非営利法人以外
	if((m_pZmSub->zvol->apno & 0xf0) <= 0x30)	return(TRUE);
// 24/07/10_個別タブから出力 add <--

// 24/04/19特定収入追加 cor -->
	//// 令和3年12月31日以降の課税期間
	//if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	//else								eymd = m_pSnHeadData->Sn_KDAYE;
	//if((m_pSnHeadData->SVmzsw == 1) || (eymd < R03YEAR))	{
	//	// 旧税率ありもしくは令和3年12月1日より前
	//	syzSign = TRUE;
	//}
	//if(syzSign == TRUE || eymd < ICS_SH_INVOICE_DAY)	{
	//	return(TRUE);
	//}
// ----------------------------
	// 令和3年12月31日以降の課税期間
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
	if(eymd < ICS_SH_INVOICE_DAY)	{
		return(TRUE);
	}
// 24/04/19特定収入追加 cor <--
// 修正No.160353 add <--
// 修正No.168725 add -->
	// 簡易課税
	if((m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01)	{
		return(TRUE);
	}
// 修正No.168725 add <--

	if((int)m_pMapPrnEXSub.GetCount() <= 0) {	// マップ未作成
// 修正No.168717 cor -->
		//return(FALSE);
// ---------------------
		// 経過措置を止めた時等、テーブル未作成のまま入る可能性があるため、ここで再作成
		long				flg=0;
		CString				strSQL=_T("");
		CString				sData=_T("");
		CTaxationList31View	*pTaxlist31View = NULL;
		CRecordset			rs(m_pZmSub->m_database);

		pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
		pTaxlist31View->IsSpecificEnable(&flg);

		// 特定収入計算表 個別出力設定用のマップをクリア
		m_pMapPrnEXSub.RemoveAll();

		// 部数（※最初は常に「1」を設定）
		busu = 1;
		// 開始頁と終了頁を取得
		stpg = 1;
		edpg = 1;

		// splistテーブルから情報を取得
		for(ii=0; ii<16; ii++)	{
			strSQL  = _T("select spitln from splist ");
			strSQL.Format(strSQL + _T("where spitem = 1000 and spitcl = %d"), ii);
			try	{
				rs.Open(CRecordset::forwardOnly, strSQL);
				// 取得
				sData.Empty();
				if(rs.IsEOF() == FALSE)	{
					rs.GetFieldValue((short)0, sData);
					rs.MoveNext();
				}
				else	{
					// 初回起動時、特定収入計算表にチェックが付いていれば個別もＯＮにする
					if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000000)	{
						if(ii == 1 && (flg & 0x0001))	sData=_T("1");
						if(ii == 2 && (flg & 0x0002))	sData=_T("1");
						if(ii == 3 && (flg & 0x0004))	sData=_T("1");
						if(ii == 4 && (flg & 0x0008))	sData=_T("1");
						if(ii == 5 && (flg & 0x0010))	sData=_T("1");
						if(ii == 6 && (flg & 0x0020))	sData=_T("1");
						if(ii == 7 && (flg & 0x0040))	sData=_T("1");
						if(ii == 8 && (flg & 0x0080))	sData=_T("1");
						if(ii == 9 && (flg & 0x0100))	sData=_T("1");
						if(ii ==10 && (flg & 0x0200))	sData=_T("1");
						if(ii ==11 && (flg & 0x0400))	sData=_T("1");
						if(ii ==12 && (flg & 0x0800))	sData=_T("1");
						if(ii ==13 && (flg & 0x1000))	sData=_T("1");
						if(ii ==14 && (flg & 0x2000))	sData=_T("1");
						if(ii ==15 && (flg & 0x4000))	sData=_T("1");
					}
				}
				rs.Close();
			}
			catch(CDBException* dbe)	{
				ICSMessageBox(dbe->m_strError);
				dbe->Delete();
				rs.Close();
				return(FALSE);
			}

			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));

			switch(ii)	{
				case 0:		// 個別に出力設定を行う
					m_PrnEXSub.check = _tstoi(sData);
					break;
				case 1:		// 計算表１
				case 2:		// 計算表２(１)
				case 3:		// 計算表２(２)
				case 4:		// 計算表３、４
				case 5:		// 計算表５(１)
				case 6:		// 計算表５(２)
				case 7:		// 計算表５(３)
					m_PrnEXSub.check = _tstoi(sData);
					m_PrnEXSub.busu = busu;
					break;
				case 8:		// 計算表５－２(１)
				case 9:		// 計算表５－２(２)
				case 10:	// 計算表５－２(３)－１
				case 11:	// 計算表５－２(３)－２
				case 12:	// 計算表５－２(３)－３
				case 13:	// 計算表５－２(４)－１
				case 14:	// 計算表５－２(４)－２
				case 15:	// 計算表５－２(４)－３
					m_PrnEXSub.check = _tstoi(sData);
					m_PrnEXSub.busu = busu;
					m_PrnEXSub.stpage = stpg;
					m_PrnEXSub.edpage = edpg;
					break;
				default:
					break;
			}
			m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
		}
// 修正No.168717 cor <--
	}

	// 部数
	busu = 1;
	// 開始頁と終了頁を取得
	stpg = 1;
	edpg = 200;
	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	edpg = pTaxlist31View->GetSpecListMaxPage();

// 24/07/10_個別タブから出力 del -->
	//if(m_pMainFrame)	{
	//	m_pMainFrame->GetBatchPrintInfo(&BpState, &BpPrintInfo);
	//}
// 24/07/10_個別タブから出力 del <--

	if(BpState) {	// 一括印刷
		// オーナーテーブルオープン
		if(m_pZmSub->owntb == NULL) {
			if(m_pZmSub->OwnTblOpen() == -1) {
				return(FALSE);
			}
			opsw = 1;
		}

		// オーナーテーブルから一括印刷情報の取得
		strSQL.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, BpPrintInfo.ForkKey, 1000);
		if(m_pZmSub->owntb->Requery(strSQL) == -1) {		// 新規作成
			// 終了頁のみ更新
			for(ii=8; ii<16; ii++)	{
				::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);
				m_PrnEXSub.edpage = edpg;
				m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
			}
		}

		if(opsw == 1){
			m_pZmSub->OwnTblClose();
		}
	}
	else	{	//通常出力	※部数と頁指定を常にリセットする ※チェック　20240312　iiは０からでは？
		for(ii=1; ii<16; ii++)	{
			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);
			m_PrnEXSub.busu = m_PrnEXSub.busu = busu;
			switch(ii)	{
				case 8:		// 計算表５－２(１)
				case 9:		// 計算表５－２(２)
				case 10:	// 計算表５－２(３)－１
				case 11:	// 計算表５－２(３)－２
				case 12:	// 計算表５－２(３)－３
				case 13:	// 計算表５－２(４)－１
				case 14:	// 計算表５－２(４)－２
				case 15:	// 計算表５－２(４)－３
					m_PrnEXSub.stpage = stpg;
					m_PrnEXSub.edpage = edpg;
					break;
				default:
					break;
			}
			m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
		}
	}

	return(TRUE);
}

//-----------------------------------------------------------------------------
// 出力情報を取得（特定収入計算表 個別出力設定）
//20240307 各個別対応に対応
//（課税取引金額計算表＋特定収入計算表＋税務代理書）
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::SetPrnExSubInfo(void)
{
	int						ii=0;
	int						chek[20];
	int						busu[20];
	int						stpg[20], edpg[20];
	int						opsw=0;
	char					cbusu[20][256];				// 24/07/10_個別タブから出力 add
	CString					strSQL1,strSQL2;
	DIAGRAM_DATA			dd;
	CDatabase*				db = m_pZmSub->m_database;
	CRecordset				rs(m_pZmSub->m_database);
	EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo = {0};

	// 消費税10％改正以前
	if(GetActViewVer() != ID_VER_SYZ31)		return(TRUE);

// 24/07/10_個別タブから出力 add -->
	if(m_pMainFrame)	{
		m_pMainFrame->GetBatchPrintInfo(&BpState, &BpPrintInfo);
	}
// 24/07/10_個別タブから出力 add <--

	if (BpState){	// 一括印刷
	// オーナーテーブルオープン
		if (m_pZmSub->owntb == NULL) {
			if (m_pZmSub->OwnTblOpen() == -1) {
				return(FALSE);
			}
			opsw = 1;
		}
	}
	 
	CString		itm_name = _T("");
	CString		filter;
	char		numbf[16] = { 0 };
	int			i = 0;
	char		bf[256];				// 24/07/10_個別タブから出力 add

	 if (BpState) {
		 //一括印刷
		 itm_name = GetBusuItmName(BpState, m_Util.GetKazeihoushiki(m_pZmSub->zvol), BpPrintInfo.ForkKey);
		 //item_seq 999を使用
		 filter.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, itm_name, 1000 - 1);
	 }
	 else {
		 filter.Format(_T("apl_name='%s' and itm_name='%s' "), DBSYZSHIN_APLNAME, DBSYZSHIN_ITMNAME_SUB);
	 }

	 // 初期化
	 for ( i = 0; i < 20; i++) {
		 //chek[i] = -1;
		 //busu[i] = -1;
		 //stpg[i] = -1;
		 //edpg[i] = -1;
		 chek[i] = 0;
		 busu[i] = 1;
		 stpg[i] = 1;
		 edpg[i] = 1;
		 memset(cbusu[i], 0, sizeof(cbusu[i]));		// 24/07/10_個別タブから出力 add
	 }

	if(m_pMapPrnEXSub_KAZ.GetCount() > 0)	{
		for(i=0; i<7; i++)	{
			::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub_KAZ.Lookup(i, m_PrnEXSub_KAZ);
			if(i == 0)  busu[0] = m_PrnEXSub_KAZ.busu;
// 24/07/10_個別タブから出力 add -->
			if(GetActViewVer() == ID_VER_SYZ31)	{	// 消費税10％改正以降
				//  1  2  3  4  5  6  7
				// 99 99 99 99 99 99 99
				// 12345678901234 <-14桁
				// 99999999999999
				memset(bf, 0, sizeof(bf));
				sprintf_s(bf, _T("%02d"), m_PrnEXSub_KAZ.busu);
				strcat_s(cbusu[0], sizeof(cbusu[0]), bf);
			}
// 24/07/10_個別タブから出力 add <--

			if(m_PrnEXSub_KAZ.check)	SetBitFlg(chek[0], i);
		 }
	}

	if(m_pMapPrnEXSub_ZEI.GetCount() > 0)	{
		for(i=0; i<11; i++)	{
			::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub_ZEI.Lookup(i, m_PrnEXSub_ZEI);
			if(i == 0)  busu[1] = m_PrnEXSub_ZEI.busu;
// 24/07/10_個別タブから出力 add -->
			if(GetActViewVer() == ID_VER_SYZ31)	{	// 消費税10％改正以降
				//  1  2  3  4  5  6  7  8  9 10 11
				// 99 99 99 99 99 99 99 99 99 99 99
				// 1234567890123456789012 <-22桁
				// 9999999999999999999999
				memset(bf, 0, sizeof(bf));
				sprintf_s(bf, _T("%02d"), m_PrnEXSub_ZEI.busu);
				strcat_s(cbusu[1], sizeof(cbusu[1]), bf);
			}
// 24/07/10_個別タブから出力 add <--

			if (m_PrnEXSub_ZEI.check)	SetBitFlg(chek[1], i);
		 }
	}
	chek[2] = Kazei_DButton_sw;
	chek[3] = Toku_DButton_sw;
	chek[4] = Zeiri_DButton_sw;

	// オーナーテーブルから一括印刷情報の取得
	// チェック、部数の内容
	if (m_pZmSub->owntb->Requery(filter) == -1) {	// 新規作成
		m_pZmSub->owntb->AddNew();
		m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
		if (BpState) {
			m_pZmSub->owntb->itm_name = itm_name;
			m_pZmSub->owntb->itm_seq = 999;
		}
		else {
			m_pZmSub->owntb->itm_name = DBSYZSHIN_ITMNAME_SUB;
			m_pZmSub->owntb->itm_seq = 0;
		}
		for ( i = 0; i < 20; i++) {
// 24/07/10_個別タブから出力 cor -->
			//m_pZmSub->owntb->vl[ii] = chek[ii];
			//m_pZmSub->owntb->str[ii].Format(_T("%d"), busu[ii]);
			//m_pZmSub->owntb->code[0].Empty();
// ---------------------------------
			m_pZmSub->owntb->vl[i] = chek[i];
			if(GetActViewVer() == ID_VER_SYZ31)		{	// 消費税10％改正以降
				if(i == 2)		m_pZmSub->owntb->str[i].Format(_T("%s"), cbusu[0]);
				else if(i == 3)	m_pZmSub->owntb->str[i].Format(_T("%s"), cbusu[1]);
				else			m_pZmSub->owntb->str[i].Format(_T("%d"), busu[i]);
			}
			else	{
				m_pZmSub->owntb->str[i].Format(_T("%d"), busu[i]);
			}
			m_pZmSub->owntb->code[i].Empty();
// 24/07/10_個別タブから出力 cor <--
		}
	}
	else {											// 上書き保存
		m_pZmSub->owntb->Edit();
		for (i = 0; i < 20; i++) {
			m_pZmSub->owntb->vl[i] = chek[i];
// 24/07/10_個別タブから出力 cor -->
			//m_pZmSub->owntb->str[i].Format(_T("%d"), busu[ii]);
// ---------------------------------
			if(GetActViewVer() == ID_VER_SYZ31)	{	// 消費税10％改正以降
				if(i == 2)		m_pZmSub->owntb->str[i].Format(_T("%s"), cbusu[0]);
				else if(i == 3)	m_pZmSub->owntb->str[i].Format(_T("%s"), cbusu[1]);
				else			m_pZmSub->owntb->str[i].Format(_T("%d"), busu[i]);
			}
			else	{
				m_pZmSub->owntb->str[i].Format(_T("%d"), busu[i]);
			}
// 24/07/10_個別タブから出力 cor <--
		}
	}
	m_pZmSub->owntb->Update();

	// 非営利法人以外は必要ないのでReturn
	if ((m_pZmSub->zvol->apno & 0xf0) <= 0x30)	return(TRUE);
	if(m_pMapPrnEXSub.GetCount() == 0)			return(TRUE);

// 24/07/10_個別タブから出力 del -->
	//if(m_pMainFrame)	{
	//	m_pMainFrame->GetBatchPrintInfo(&BpState, &BpPrintInfo);
	//}
// 24/07/10_個別タブから出力 del <--

	if(BpState)	{	// 一括印刷
		// オーナーテーブルオープン
		if(m_pZmSub->owntb == NULL) {
			if(m_pZmSub->OwnTblOpen() == -1) {
				return(FALSE);
			}
			opsw = 1;
		}

		// 初期化
		for(ii=0; ii<20; ii++) {
			chek[ii] = 0;
			busu[ii] = 1;
			stpg[ii] = 1;
			edpg[ii] = 1;
		} 

		for(ii=0; ii<16; ii++)	{
			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);

			chek[ii] = m_PrnEXSub.check;
			busu[ii] = m_PrnEXSub.busu;
			stpg[ii] = m_PrnEXSub.stpage;
			edpg[ii] = m_PrnEXSub.edpage;
		}

		// オーナーテーブルから一括印刷情報の取得
		// チェック、部数の内容
		strSQL1.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, BpPrintInfo.ForkKey, 1000);
		if(m_pZmSub->owntb->Requery(strSQL1) == -1)		{	// 新規作成
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
			m_pZmSub->owntb->itm_name = BpPrintInfo.ForkKey;
			m_pZmSub->owntb->itm_seq = 1000;
			for(ii=0; ii<20; ii++)	{
				m_pZmSub->owntb->vl[ii] = chek[ii];
				m_pZmSub->owntb->str[ii].Format(_T("%d"), busu[ii]);
				m_pZmSub->owntb->code[0].Empty();
			}
		}
		else	{											// 上書き保存
			m_pZmSub->owntb->Edit();
			for(ii=0; ii<20; ii++)	{
				m_pZmSub->owntb->vl[ii] = chek[ii];
				m_pZmSub->owntb->str[ii].Format(_T("%d"), busu[ii]);
			}
		}
		m_pZmSub->owntb->Update();

		// 開始頁、終了頁の内容
		strSQL1.Format(TKNEW_OWNTBL_SQL, DBSYZSHIN_APLNAME, BpPrintInfo.ForkKey, 1001);
		if(m_pZmSub->owntb->Requery(strSQL1) == -1)		{	// 新規作成
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
			m_pZmSub->owntb->itm_name = BpPrintInfo.ForkKey;
			m_pZmSub->owntb->itm_seq = 1001;
			for(ii=0; ii<20; ii++)	{
				m_pZmSub->owntb->vl[ii] = stpg[ii];
				m_pZmSub->owntb->str[ii].Format(_T("%d"), edpg[ii]);
				m_pZmSub->owntb->code[0].Empty();
			}
		}
		else	{											// 上書き保存
			m_pZmSub->owntb->Edit();
			for(ii=0; ii<20; ii++)	{
				m_pZmSub->owntb->vl[ii] = stpg[ii];
				m_pZmSub->owntb->str[ii].Format(_T("%d"), edpg[ii]);
			}
		}
		m_pZmSub->owntb->Update();

		if(opsw == 1){
			m_pZmSub->OwnTblClose();
		}
	}
	else	{		// 通常出力
		db->BeginTrans();

// 24/03/26_特定収入 add -->
		// カラム spver を追加するためOPEN関数を呼び出す
		if(m_pZmSub->NpTokuteiSyunyuKeisanOpen() == ERR)	{
			ICSMessageBox(m_pZmSub->GetZmErrMsg(),MB_ICONSTOP);
			db->Rollback();
			return(FALSE);
		}
		m_pZmSub->NpTokuteiSyunyuKeisanClose();
		int		eymd=0;
		int		ver=0;
		if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
		else								eymd = m_pSnHeadData->Sn_KDAYE;
		if(eymd >= ICS_SH_INVOICE_DAY)	ver=2;
// 24/03/26_特定収入 add <--

		// チェックのみ保存
		for(ii=0; ii<16; ii++)	{
			// 初期化
			chek[0] = 0;
			busu[0] = NULL;
			stpg[0] = edpg[0] = NULL;

			if(m_pMapPrnEXSub.GetCount() > 0)	{
				::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);

				chek[0] = m_PrnEXSub.check;
			}

			// SQL文の作成
			try	{
				strSQL1 = _T("select spitln from splist ");
				strSQL1.Format(strSQL1 + _T("where spitem = 1000 and spitcl = %d "), ii);
				rs.Open(CRecordset::forwardOnly, strSQL1);
				if(rs.IsEOF() == FALSE)	{
// 24/03/26_特定収入 cor -->
					//strSQL2.Format(_T("update splist set spitln = %d "), chek[0]);
// -------------------------
					strSQL2.Format(_T("update splist set spitln = %d, spver = %d "), chek[0], ver);
// 24/03/26_特定収入 cor <--
					strSQL2.Format(strSQL2 + _T("where spitem = 1000 and spitcl = %d "), ii);
				}
				else	{
// 24/03/26_特定収入 cor -->
					//strSQL2 = _T("insert into splist(spitem, spitln, spitcl, spitval, spitper, spitnm, spseq) \r\n");
					//strSQL2.Format(strSQL2 + _T("values(1000, %d, %d, NULL, NULL, NULL, 0) "), chek[0], ii);
// -------------------------
					strSQL2 = _T("insert into splist(spitem, spitln, spitcl, spitval, spitper, spitnm, spseq, spver) \r\n");
					strSQL2.Format(strSQL2 + _T("values(1000, %d, %d, NULL, NULL, NULL, 0, %d) "), chek[0], ii, ver);
// 24/03/26_特定収入 cor <--
				}
				rs.Close();
			
				db->ExecuteSQL(strSQL2);
			
			}
			catch(CDBException* dbe)	{
				ICSMessageBox("更新に失敗しました！\nデータは変更されません。",0, 0, 0, this);
				db->Rollback();
				rs.Close();
				dbe->Delete();
				return(FALSE);
			}
		}

		db->CommitTrans();
	}

	return(TRUE);
}
// 24/02/19wd_特定収入対応 add <--


//-----------------------------------------------------------------------------
// 印刷微調整情報の取得
//-----------------------------------------------------------------------------
//void CDBSyzShinView:: GetPrintInfo()
void CDBSyzShinMainView::GetPrAdjustInfo()
{
	int		cnt;
	char	SyzName[64]={0}, PrtName[64]={0}, TypName[64]={0}, PrtBuf1[64]={0}, PrtBuf2[64]={0}, PrtBuf3[64]={0}, PrtBuf4[64]={0};

	//for( cnt=0; cnt<20; cnt++ ){
	for (cnt = 0; cnt < 30; cnt++) {
		memset( &m_PrMv[cnt], '\0', sizeof(SH_PRMOVE) );
		memset( &m_PrMove[cnt], '\0', sizeof(SH_PRMOVE) );
		if( cnt < 10 ){
			m_PrMv[cnt]._PSign = cnt + 1;
		}
	}
	
	memset( PrtName, '\0', sizeof(PrtName) );
	if( PCOM_GetString(_T("SelectPrinter"), PrtName) == -1 ){
		return;
	}
	memset( SyzName, '\0', sizeof(SyzName) );
	strcpy_s( SyzName, sizeof(SyzName), _T("消費税申告書") );
	//for( cnt=0; cnt<20; cnt++ ){
	for (cnt = 0; cnt < 30; cnt++) {
		memset( TypName, '\0', sizeof(TypName) );
		sprintf_s( TypName, sizeof(TypName), _T("REPORT_SHEET_%02d"), cnt+1 );
		if( PP_GetAppData(0, SyzName, PrtName, TypName, PrtBuf1, PrtBuf2, PrtBuf3, PrtBuf4) == 0 ){
			m_PrMv[cnt]._PKVad = atoi( PrtBuf1 );
			m_PrMv[cnt]._PKHad = atoi( PrtBuf2 );
			m_PrMv[cnt]._PIVad = atoi( PrtBuf3 );
			m_PrMv[cnt]._PIHad = atoi( PrtBuf4 );
		}
	}
	m_PrintCmInfo.OPEla = 0;
	memset( TypName, '\0', sizeof(TypName) );
	strcpy_s( TypName, sizeof(TypName), _T("REPORT_SHEET_99") );
	if( PP_GetAppData(0, SyzName, PrtName, TypName, PrtBuf1) == 0 ){
		m_PrintCmInfo.OPEla = atoi( PrtBuf1 );
	}
}

//-----------------------------------------------------------------------------
// 印刷微調整情報の保存
//-----------------------------------------------------------------------------
//void CDBSyzShinView::SetPrintInfo()
void CDBSyzShinMainView::SetPrAdjustInfo()
{
	char	SyzName[64]={0}, PrtName[64]={0}, TypName[64]={0}, PrtBuf1[64]={0}, PrtBuf2[64]={0}, PrtBuf3[64]={0}, PrtBuf4[64]={0};

	memset( PrtName, '\0', sizeof(PrtName) );
	if( PCOM_GetString(_T("SelectPrinter"), PrtName) == -1 ){
		return;
	}
	memset( SyzName, '\0', sizeof(SyzName) );
	strcpy_s( SyzName, sizeof(SyzName), _T("消費税申告書") );
	//for( int cnt=0; cnt<20; cnt++ ){
	for (int cnt = 0; cnt < 30; cnt++) {
		memset( TypName, '\0', sizeof(TypName) );
		sprintf_s( TypName, sizeof(TypName), _T("REPORT_SHEET_%02d"), cnt+1 );
		sprintf_s( PrtBuf1, sizeof(PrtBuf1), _T("%d"), m_PrMv[cnt]._PKVad );
		sprintf_s( PrtBuf2, sizeof(PrtBuf2), _T("%d"), m_PrMv[cnt]._PKHad );
		sprintf_s( PrtBuf3, sizeof(PrtBuf3), _T("%d"), m_PrMv[cnt]._PIVad );
		sprintf_s( PrtBuf4, sizeof(PrtBuf4), _T("%d"), m_PrMv[cnt]._PIHad );
		PP_SetAppData( 0, SyzName, PrtName, TypName, PrtBuf1, PrtBuf2, PrtBuf3, PrtBuf4 );
	}

	memset( TypName, '\0', sizeof(TypName) );
	strcpy_s( TypName, sizeof(TypName), _T("REPORT_SHEET_99") );
	sprintf_s( PrtBuf1, sizeof(PrtBuf1), _T("%d"), m_PrintCmInfo.OPEla );
	PP_SetAppData( 0, SyzName, PrtName, TypName, PrtBuf1 );
}

//-----------------------------------------------------------------------------
// 印刷前処理
//-----------------------------------------------------------------------------
// 返送値	0		：	出力可
//			0以外	：	出力不可
//-----------------------------------------------------------------------------
//int CDBSyzShinView::SyzShinPrint( void )
int CDBSyzShinMainView::CheckSyzShinPrint()
{
	// 課税売上の金額チェック
	if( CheckKzeiUri() ){
		return TRUE;
	}

	// 中間納付税額の金額チェック
	if( CheckMiddleTax() ){
		return TRUE;
	}

	// 修正申告時の年度チェックをカット
//	if( EntryCheck( ((Snh.Sn_SKKBN&0xff) < 3) ? 0 : 1 ) == TRUE ){
//		return( TRUE );
//	}

	// 申告サイン設定
//	memset( SYsw, '\0', sizeof( SYsw ) );
//	for( ItCt = 0; ItCt < 13; ++ItCt ){
//		SYsw[ItCt] = (Vol2.D_TBL[ItCt].C_KSW & 0x40) ? 0x01 : 0;
//	}

/* 不要！？！？！？
	K221sw = H301sw = 0;								// 規則２２条１項の適用（積み上げ方式）
	Skbn = (int)(Snh.Sn_SKKBN&0xff);					// 申告区分

	if( Vol1.S_SGN2 & 0x80 )	Kenei = 0x01;			// 兼業事業者
	else						Kenei = 0x00;			// 単一事業者
	Kani = (int)(Vol1.S_SGN2 & 0x0f);					// 業種区分
	toku = (int)(Snh.Sn_TOKUR & 0xff);					// 特例計算の適用
	if( Snh.Sn_TUMIAG & 0x01 )	K221sw = 1;				// 規則２２条１項の適用（積み上げ方式）
	if( Snh.Sn_TUMIAG & 0x02 )	H301sw = 1;				// 法第３０条第１項の適用（積み上げ方式）
	Srx = (int)(Snh.Sn_UPERS & 0x01);					// ON:売上割合95%以下, OFF:95%以上
	if( toku )	Kenei |= 0x10;							// 特例計算摘要ＳＷ

	if ( ( ErNo = read_tbl( (short)3 ) ) )	{
		memset( ERMS, '\0', sizeof( ERMS ) );
		switch( ErNo )	{
			case 5 :
				return( TRUE );
				break;
			case 6 :
				strcat_s( ERMS, sizeof( ERMS ), "申告区分が正しくありません。" );
				break;
			default :
				strcat_s( ERMS, sizeof( ERMS ), "申告データが読み込めません" );
				break;
		}
		ICSMessageBox( ERMS, MB_OK, 0 );	// IDOK IDCANCEL
		return( TRUE );
	}
	// 開始＆最終年月
	ofs_get();

	if( Snd.Sn_EXP100&0x01 ) Srx = 0x00;
	
	Ver = N_ver;	// [06'11.29]
*/

	// 基準期間の課税売上高チェック。
	if(BasPerideTaxSold( 1 ) ){
		return -1;
	}

	// 更正の申出書の変換
	char	yymmdd[3] = {0};
	if( m_pSnHeadData->IsSoleProprietor() ){
		// 平成23年より前の場合に許可
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		if( m_Arith.l_jsgn(yymmdd, AMENDK, 3) >= 0 ){
			m_pSnHeadData->Sn_Syuusei &= 0xfb;
		}
	}
	else{
		// 平成23年10月2日より前の場合に許可
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		if( m_Arith.l_jsgn(yymmdd, AMENDH, 3) >= 0 ){
			m_pSnHeadData->Sn_Syuusei &= 0xfb;
		}
	}

//--> '15.02.24 INS START
	if( m_pMainFrame ){
		// 還付申告？
		if( m_pMainFrame->IsKanpuShinkoku() ){
			ChangeRefundOut( 1 );
		}
		else{
			ChangeRefundOut( 0 );
		}
	}
//<-- '15.02.24 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上の金額チェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			1	：	中断
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::CheckKzeiUri()
{
	// 2割特例の場合はこの課税期間の課税売上高は記載不要のため、チェック不要
//	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) && (m_pSnHeadData->IsMiddleProvisional()==FALSE) && !(m_pSnHeadData->m_DispTabSgn&0x2000) ){
	if( m_pSnHeadData->IsMiddleProvisional()==FALSE ){	// 原則・簡易・2割特例に関係なくメッセージを表示する。
		// 金額取得
		if( m_pMainFrame == NULL ){
			return 0;
		}
		char	tmpKSTD[MONY_BUF_SIZE]={0}, tmpKZURI[MONY_BUF_SIZE]={0};
		CString	tag, chtag;
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI ){
// 240501_168733 add -->
			if(IsInputKazeiUriage() == false) {
				return(0);
			}
// 240501_168733 add <--
			//yoshida190730
			if (GetActViewVer() == ID_VER_SYZ26) {
				tag = _T("ABB00010");
				m_pMainFrame->GetMony(tag, chtag, tmpKSTD);
				tag = _T("ABB00170");
				m_pMainFrame->GetMony(tag, chtag, tmpKZURI);
			}
			else {
				tag = _T("ABI00010");
				m_pMainFrame->GetMony(tag, chtag, tmpKSTD);
				tag = _T("ABI00170");
				m_pMainFrame->GetMony(tag, chtag, tmpKZURI);
			}
		}
		else {
			if( IsInputKazeiUriage() ) {
				tag = _T("AAJ00010");
				m_pMainFrame->GetMony(tag, chtag, tmpKSTD);
				tag = _T("FFF00000");
				m_pMainFrame->GetMony(tag, chtag, tmpKZURI);
			}
			else{
				return 0;
			}
		}

		if( m_Arith.l_test(tmpKSTD) && (m_Arith.l_test(tmpKZURI)==0) ){
			CString Mesg;
			long symd = 0, eymd = 0;
			GetKazeiKikan( symd, eymd );

			if( eymd >= ICS_SH_INVOICE_DAY ){
				Mesg = _T("「基本情報」の「税額情報」にて、\n当課税期間の課税売上高を入力してください。");
			}
			else{
				Mesg = _T("「簡易本表」入力画面において、\n『この課税期間の課税売上高』を入力してください");
			}
			ICSMessageBox( Mesg, MB_OK, 0 );
			return 1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 中間納付税額の金額チェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			1	：	中断
//-----------------------------------------------------------------------------
//int CDBSyzShinView::MiddleTaxCheck( void ) 
int CDBSyzShinMainView::CheckMiddleTax() 
{	
	int		Flg = 0;
	
	if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){
		// 金額取得
		if( m_pMainFrame == NULL ){
			return 0;
		}
		char	tmpTYNOFZ[MONY_BUF_SIZE]={0}, tmpTTYWAR[MONY_BUF_SIZE]={0};
		CString	tag, chtag;
		if( GetActViewVer() == ID_VER_SYZ26 ){
			tag = _T("ABB00110");
			m_pMainFrame->GetMony( tag, chtag, tmpTYNOFZ );
			tag = _T("Sn_TTYWAR");
			m_pMainFrame->GetMony( tag, chtag, tmpTTYWAR );
		}
		else if( GetActViewVer() == ID_VER_SYZ31 ){
			EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(m_pZmSub->zvol);
			if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
				tag = _T( "AAJ00110" );		// 中間納付税額
				m_pMainFrame->GetMony ( tag, chtag, tmpTYNOFZ );
				tag = _T( "AAK00070" );		// 中間納付譲渡割額
				m_pMainFrame->GetMony ( tag, chtag, tmpTTYWAR );
			}
			else if ( KzHoushiki == ID_ICSSH_KANNI ) {
				tag = _T( "ABI00110" );		// 中間納付税額
				m_pMainFrame->GetMony ( tag, chtag, tmpTYNOFZ );
				tag = _T( "ABJ00070" );		// 中間納付譲渡割額
				m_pMainFrame->GetMony ( tag, chtag, tmpTTYWAR );
			}
		}

		if( m_Arith.l_test(tmpTYNOFZ) && (m_Arith.l_test(tmpTTYWAR)==0) ){
			Flg = 1;
		}
		if( (m_Arith.l_test(tmpTYNOFZ)==0) && m_Arith.l_test(tmpTTYWAR) ){
			Flg = 2;
		}
		if( Flg ){
			CString		Msg;
			Msg.Empty();
			Msg = _T("中間納付税額又は中間納付譲渡割額の金額が不正です！\n");
			if( Flg == 1 ){
				Msg += _T("中間納付譲渡割額(21)の税額が入力されていません。\n\n" );
			}
			else{
				Msg += _T("中間納付税額(10)の税額が入力されていません。\n\n");
			}
			Msg += _T("　処理を継続する・[はい]　／　継続しない・[いいえ]" );	
			if( ICSMessageBox( Msg, MB_YESNO | MB_DEFBUTTON2, 0 ) == IDYES ){
				return 0;
			}
			else{
				return 1;
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 印刷中？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	印刷中
//			FALSE	：	印刷中でない
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsPrinting()
{
	return m_Vprn.IsPrinting();
}

//-----------------------------------------------------------------------------
// ユーザー情報保存
//-----------------------------------------------------------------------------
// 引数	usertbl	：	ユーザー情報
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetInitInfo( mctl_usertbl &usertbl )
{
	int	cnt;

	// オープンチェック
	if( m_pZmSub->m_database == NULL ){
		return;
	}

	// オーナーテーブルオープン
	if( m_pZmSub->owntb == NULL ){
		if( m_pZmSub->OwnTblOpen() == -1 ){
			return;
		}
	}

	EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo = {0};
	if( m_pMainFrame ){
		m_pMainFrame->GetBatchPrintInfo( &BpState, &BpPrintInfo );
	}
	char					numbf[16]={0};
	unsigned short			numno = 0;
	unsigned long			numlong;

	//----> 20240116
	if( m_pMainFrame ){
		m_pMainFrame->GetScoreLinkInfo( m_isScoreLinkCall, m_ScoreLinkPdfNam );
	}

	CString itm_name = "";
	if( m_isScoreLinkCall != -1 ){
		// ScoreLinkからの呼び出しの場合は通常業務の設定を使用する。
		// 仕入税額控除とBASICの方は影響がないと考え対応は入れない。
		itm_name.Format( DBSYZSHIN_ITMNAME );
	}
	else{
		itm_name.Format( BpState ? BpPrintInfo.ForkKey : DBSYZSHIN_ITMNAME );
	}
	//<---- 20240116

	//== 印刷情報の保存 ==
	CString	filter;
	//filter.Format( OWNTBL_SQL, DBSYZSHIN_APLNAME, (BpState ? BpPrintInfo.ForkKey : DBSYZSHIN_ITMNAME) );
	filter.Format( OWNTBL_SQL, DBSYZSHIN_APLNAME, itm_name );
	if( m_pZmSub->owntb->Requery(filter) == -1 ){
		m_pZmSub->owntb->AddNew();
		m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
		//m_pZmSub->owntb->itm_name = (BpState ? BpPrintInfo.ForkKey : DBSYZSHIN_ITMNAME);
		m_pZmSub->owntb->itm_name = itm_name;
		m_pZmSub->owntb->vl[0] = (long)m_PrintCmInfo.OP1;
		m_pZmSub->owntb->vl[1] = (long)m_PrintCmInfo.OP2;
		m_pZmSub->owntb->vl[2] = (long)m_PrintCmInfo.OP3;
		m_pZmSub->owntb->vl[3] = (long)m_PrintCmInfo.OP4;
		m_pZmSub->owntb->vl[4] = (long)m_PrintCmInfo.OP5;
		m_pZmSub->owntb->vl[5] = (long)m_PrintCmInfo.OP6;
		m_pZmSub->owntb->vl[6] = (long)m_PrintCmInfo.OP7;
		m_pZmSub->owntb->vl[7] = (long)m_PrintCmInfo.OP8;
		m_pZmSub->owntb->vl[8] = (long)m_PrintCmInfo.OP9;
		m_pZmSub->owntb->vl[9] = (long)m_PrintCmInfo.OPA;
		m_pZmSub->owntb->vl[10] = (long)m_PrintCmInfo.OPB;
		m_pZmSub->owntb->vl[11] = (long)m_PrintCmInfo.OPC;
		m_pZmSub->owntb->vl[12] = (long)m_PrintCmInfo.OPD;
		m_pZmSub->owntb->vl[13] = (long)m_PrintCmInfo.OPE;
		m_pZmSub->owntb->vl[14] = (long)m_PrintCmInfo.OPF;
		m_pZmSub->owntb->vl[15] = (long)m_PrintCmInfo.OPG;
		m_pZmSub->owntb->vl[16] = (long)m_PrintCmInfo.OPH;
		m_pZmSub->owntb->vl[17] = (long)m_PrintCmInfo.OPI;		// [09'07.24]
		m_pZmSub->owntb->vl[18] = (long)m_PrintCmInfo.PsSigDate;
		m_pZmSub->owntb->vl[19] = (long)m_PrintCmInfo.PsSigConame;

		// SCORELINK対応 20240116
		//if( m_swBeFork ){
		if( m_swBeFork && m_isScoreLinkCall == -1 ){
//		if( 0 ){
			numno = (unsigned short)m_pSnHeadData->Sn_TYPE;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[0] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_SPECIAL;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[1] = numbf;

//★//[13'02.27]///
//			sprintf_s( numbf, "%0d", m_BasicSign );
//			m_pZmSub->owntb->str[2] = numbf;
///////////////////

//★//[12'10.02]///
			numno = (unsigned short)m_pSnHeadData->Sn_Sign1;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[3] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_Sign2;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[4] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_Sign3;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[5] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_Sign4;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", numno);
			m_pZmSub->owntb->str[6] = numbf;

			numlong = (unsigned long)m_pSnHeadData->Sn_PrintOutSgn;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", numlong);
			m_pZmSub->owntb->str[7] = numbf;

			numlong = (unsigned long)m_pSnHeadData->Sn_PrintOutSgn2;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", numlong);
			m_pZmSub->owntb->str[8] = numbf;

			m_pSnHeadData->Sn_TYPE = Save_Head[0];
			m_pSnHeadData->Sn_SPECIAL = Save_Head[1];
			m_pSnHeadData->Sn_Sign1 = Save_Head[2];
			m_pSnHeadData->Sn_Sign2 = Save_Head[3];
			m_pSnHeadData->Sn_Sign3 = Save_Head[4];
			m_pSnHeadData->Sn_Sign4 = Save_Head[5];
			m_pSnHeadData->Sn_PrintOutSgn = Save_Headlong;
			m_pSnHeadData->Sn_PrintOutSgn2 = Save_Headlong2;
///////////////////
		}
//★//[12'10.02]///

		// 追加印刷
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPJ );
		m_pZmSub->owntb->str[10] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPK );
		m_pZmSub->owntb->str[11] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPL );
		m_pZmSub->owntb->str[12] = numbf;
		
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPM );
		m_pZmSub->owntb->str[13] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPN );
		m_pZmSub->owntb->str[14] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPO );
		m_pZmSub->owntb->str[15] = numbf;
		
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPP );
		m_pZmSub->owntb->str[16] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPQ );
		m_pZmSub->owntb->str[17] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPR );
		m_pZmSub->owntb->str[18] = numbf;
		
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPS );
		m_pZmSub->owntb->str[19] = numbf;
///////////////////
//2016.02.23 INSERT START
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPT );
		m_pZmSub->owntb->code[0] = numbf;
//2016.02.23 INSERT END
//--> '17.03.28 INS START
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPU );
		m_pZmSub->owntb->code[1] = numbf;
//--> '17.03.28 INS END
//2018.03.13 INSERT START
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPV );
		m_pZmSub->owntb->code[2] = numbf;
//2018.03.13 INSERT END

		memset(numbf, '\0', sizeof(numbf));
		sprintf_s(numbf, sizeof(numbf), "%d", m_PrintCmInfo.OP_GENGO);
		m_pZmSub->owntb->code[3] = numbf;

// 改良依頼 20-0352 修正 add -->
		if( GetActViewVer() == ID_VER_SYZ31 ) {
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", m_PrintCmInfo.OPW);
			m_pZmSub->owntb->code[4] = numbf;
		}
// 改良依頼 20-0352 修正 add <--

		m_pZmSub->owntb->Update();
		if( m_pZmSub->owntb->Requery(filter) == -1 ){
			return;
		}
	}
	else{
		// 編集
		m_pZmSub->owntb->Edit();
		m_pZmSub->owntb->vl[0] = (long)m_PrintCmInfo.OP1;
		m_pZmSub->owntb->vl[1] = (long)m_PrintCmInfo.OP2;
		m_pZmSub->owntb->vl[2] = (long)m_PrintCmInfo.OP3;
		m_pZmSub->owntb->vl[3] = (long)m_PrintCmInfo.OP4;
		m_pZmSub->owntb->vl[4] = (long)m_PrintCmInfo.OP5;
		m_pZmSub->owntb->vl[5] = (long)m_PrintCmInfo.OP6;
		m_pZmSub->owntb->vl[6] = (long)m_PrintCmInfo.OP7;
		m_pZmSub->owntb->vl[7] = (long)m_PrintCmInfo.OP8;
		m_pZmSub->owntb->vl[8] = (long)m_PrintCmInfo.OP9;
		m_pZmSub->owntb->vl[9] = (long)m_PrintCmInfo.OPA;
		m_pZmSub->owntb->vl[10] = (long)m_PrintCmInfo.OPB;
		m_pZmSub->owntb->vl[11] = (long)m_PrintCmInfo.OPC;
		m_pZmSub->owntb->vl[12] = (long)m_PrintCmInfo.OPD;
		m_pZmSub->owntb->vl[13] = (long)m_PrintCmInfo.OPE;
		m_pZmSub->owntb->vl[14] = (long)m_PrintCmInfo.OPF;
		m_pZmSub->owntb->vl[15] = (long)m_PrintCmInfo.OPG;
		m_pZmSub->owntb->vl[16] = (long)m_PrintCmInfo.OPH;
		m_pZmSub->owntb->vl[17] = (long)m_PrintCmInfo.OPI;		// [09'07.24]
		m_pZmSub->owntb->vl[18] = (long)m_PrintCmInfo.PsSigDate;
		m_pZmSub->owntb->vl[19] = (long)m_PrintCmInfo.PsSigConame;

//		if( m_swFork ){
		//if( m_swBeFork ){
		if( m_swBeFork && m_isScoreLinkCall == -1 ){
			numno = (unsigned short)m_pSnHeadData->Sn_TYPE;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[0] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_SPECIAL;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[1] = numbf;
//★//[13'02.27]///
//			sprintf_s( numbf, "%0d", m_BasicSign );
//			m_pZmSub->owntb->str[2] = numbf;
///////////////////
//★//[12'10.02]///
			numno = (unsigned short)m_pSnHeadData->Sn_Sign1;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[3] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_Sign2;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[4] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_Sign3;
			memset( numbf, '\0',sizeof( numbf ) );
			sprintf_s( numbf, sizeof( numbf ), "%d", numno );
			m_pZmSub->owntb->str[5] = numbf;

			numno = (unsigned short)m_pSnHeadData->Sn_Sign4;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", numno);
			m_pZmSub->owntb->str[6] = numbf;

			numlong = (unsigned long)m_pSnHeadData->Sn_PrintOutSgn;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", numlong);
			m_pZmSub->owntb->str[7] = numbf;

			numlong = (unsigned long)m_pSnHeadData->Sn_PrintOutSgn2;
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", numlong);
			m_pZmSub->owntb->str[8] = numbf;

			m_pSnHeadData->Sn_TYPE = Save_Head[0];
			m_pSnHeadData->Sn_SPECIAL = Save_Head[1];
			m_pSnHeadData->Sn_Sign1 = Save_Head[2];
			m_pSnHeadData->Sn_Sign2 = Save_Head[3];
			m_pSnHeadData->Sn_Sign3 = Save_Head[4];
			m_pSnHeadData->Sn_Sign4 = Save_Head[5];
			m_pSnHeadData->Sn_PrintOutSgn = Save_Headlong;
			m_pSnHeadData->Sn_PrintOutSgn2 = Save_Headlong2;
///////////////////
		}


//★//[12'10.02]///
		// 追加印刷
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPJ );
		m_pZmSub->owntb->str[10] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPK );
		m_pZmSub->owntb->str[11] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPL );
		m_pZmSub->owntb->str[12] = numbf;
		
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPM );
		m_pZmSub->owntb->str[13] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPN );
		m_pZmSub->owntb->str[14] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPO );
		m_pZmSub->owntb->str[15] = numbf;
		
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPP );
		m_pZmSub->owntb->str[16] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPQ );
		m_pZmSub->owntb->str[17] = numbf;

		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPR );
		m_pZmSub->owntb->str[18] = numbf;
		
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPS );
		m_pZmSub->owntb->str[19] = numbf;

///////////////////
//2016.02.23 INSERT START
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPT );
		m_pZmSub->owntb->code[0] = numbf;
//2016.02.23 INSERT END
//--> '17.03.28 INS START
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPU );
		m_pZmSub->owntb->code[1] = numbf;
//--> '17.03.28 INS END
//2018.03.13 INSERT START
		memset( numbf, '\0',sizeof( numbf ) );
		sprintf_s( numbf, sizeof( numbf ), "%d", m_PrintCmInfo.OPV );
		m_pZmSub->owntb->code[2] = numbf;
//2018.03.13 INSERT END

		memset(numbf, '\0', sizeof(numbf));
		sprintf_s(numbf, sizeof(numbf), "%d", m_PrintCmInfo.OP_GENGO);
		m_pZmSub->owntb->code[3] = numbf;

// 改良依頼 20-0352 修正 add -->
		if( GetActViewVer() == ID_VER_SYZ31 ) {
			memset(numbf, '\0', sizeof(numbf));
			sprintf_s(numbf, sizeof(numbf), "%d", m_PrintCmInfo.OPW);
			m_pZmSub->owntb->code[4] = numbf;
		}
// 改良依頼 20-0352 修正 add <--

		m_pZmSub->owntb->Update();
		if( m_pZmSub->owntb->Requery(filter) == -1 ){
			return;
		}
	}

//--> '14.04.30 INS START
	if( m_PrintCmInfo.OPsign&0x01 ){
		m_pSnHeadData->Sn_Sign2 |= 0x10;
	}
	else{
		m_pSnHeadData->Sn_Sign2 &= ~0x10;
	}
//<-- '14.04.30 INS END

	////////////////////////////////////////////////////
	// 仕入控除税額に関する明細書の変換テーブルの保存 //
	////////////////////////////////////////////////////

	// TISW修正[22-0676] 2026/03/19 START
//	// 取り消し
//	if( m_pZmSub->zvol->apno == 0x52 &&  m_pZmSub->zvol->m_ver == 0x11 ){
//		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ52_ITMNAME, usertbl.user_id );
//	}
//	else{
//		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ_ITMNAME, usertbl.user_id );
//	}
//	if( m_pZmSub->owntb->Requery(filter) != -1 ){
///*- '14.03.27 -*/
////		while( 1 ){
////			if( m_pZmSub->owntb->Move( 0 ) == ERR ){
////				break;
////			}
////			if( m_pZmSub->owntb->Delete() == ERR ){
////				continue;
////			}
////		}
///*-------------*/
//		int ownCnt = (int)m_pZmSub->owntb->GetRecordCount();
//		for( int i=ownCnt; i>0; i-- ){
//			m_pZmSub->owntb->SetAbsolutePosition( i );
//			m_pZmSub->owntb->Delete();
//		}
///*-------------*/
//	}
//
//	// 保存
//	cnt = 0;
//	SH_SKJ_CNV	SkjCnv;
//	while( 1 ){
//		if( m_pTblhdl->th_rget( &SkjCnv, m_pSkjCnvtbl, cnt ) ) break;
//		m_pZmSub->owntb->AddNew();
//		m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
//		if( m_pZmSub->zvol->apno == 0x52 &&  m_pZmSub->zvol->m_ver == 0x11 )
//			m_pZmSub->owntb->itm_name = SKJ52_ITMNAME;
//		else	m_pZmSub->owntb->itm_name = SKJ_ITMNAME;
//		m_pZmSub->owntb->itm_seq  = SkjCnv.SKJSeq;
//		m_pZmSub->owntb->u_id	   = usertbl.user_id;
//		m_Util.char_to_cstring( &m_pZmSub->owntb->code[0], SkjCnv.SKJCod, 8 );
//		m_pZmSub->owntb->vl[0]  = SkjCnv.HJNCod;
//		m_pZmSub->owntb->vl[1]  = SkjCnv.KJNCod;
//		m_pZmSub->owntb->Update();
//		cnt++;
//	}

	// 保存
	SkjUserSettingsAccessor skjSettingsAccess{ m_pZmSub, usertbl, m_pTblhdl, m_pSkjCnvtbl };
	skjSettingsAccess.SaveSkjSettings();
	// TISW修正[22-0676] 2026/03/19 END

	// SCORELINK対応 20240116
	// 取り消し
	//filter.Format( OWNTBL_SQL, DBSYZBASIC_APLNAME, (BpState ? BpPrintInfo.ForkKey : DBSYZBASIC_ITMNAME) );
	filter.Format( OWNTBL_SQL, DBSYZBASIC_APLNAME, (( BpState && m_isScoreLinkCall == -1 ) ? BpPrintInfo.ForkKey : DBSYZBASIC_ITMNAME) );
	if( m_pZmSub->owntb->Requery(filter) != -1 ){	
		m_pZmSub->owntb->Move( 0 );
		m_pZmSub->owntb->Delete();
	}

	//filter.Format( OWNTBL_SQL, DBSYZBASIC_APLNAME, (BpState ? BpPrintInfo.ForkKey : DBSYZBASIC_ITMNAME) );
	filter.Format( OWNTBL_SQL, DBSYZBASIC_APLNAME, (( BpState && m_isScoreLinkCall == -1 ) ? BpPrintInfo.ForkKey : DBSYZBASIC_ITMNAME) );
	if( m_pZmSub->owntb->Requery(filter) == -1 ){
		// 新規登録
		m_pZmSub->owntb->AddNew();
		m_pZmSub->owntb->apl_name = DBSYZBASIC_APLNAME;
		//m_pZmSub->owntb->itm_name = (BpState ? BpPrintInfo.ForkKey : DBSYZBASIC_ITMNAME);
		m_pZmSub->owntb->itm_name = (( BpState && m_isScoreLinkCall == -1 ) ? BpPrintInfo.ForkKey : DBSYZBASIC_ITMNAME);
		for( cnt = 0; cnt != 5; cnt++ ) {
			m_pZmSub->owntb->vl[cnt] = m_pShinInfo->PNO[cnt];
		}
		switch( m_pShinInfo->CpnSw ){
			case 0x01 :	
				m_pZmSub->owntb->vl[5] = 1;	
				break;
			case 0x03 :	
				m_pZmSub->owntb->vl[5] = 2;	
				break;
			default:	
				m_pZmSub->owntb->vl[5] = 0;	
				break;
		}
		m_pZmSub->owntb->vl[6] = m_pShinInfo->kdat1;
		m_pZmSub->owntb->vl[7] = m_pShinInfo->kdat2;
		m_pZmSub->owntb->Update();
		if( m_pZmSub->owntb->Requery(filter) == -1 ){
			return;
		}
	}
}

//-----------------------------------------------------------------------------
// ユーザー情報保存（部数）
//-----------------------------------------------------------------------------
// 引数	usertbl	：	ユーザー情報
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetInitInfo2( void )
{
	// 印刷部数の書き込み
	CString					itm_name=_T("");
	CString					filter=_T("");
	int						ii=0,jj=0,kk=0;
	EnumIdShBatchPrintState	BpState=ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo={0};

	if(m_pMainFrame)	{
		m_pMainFrame->GetBatchPrintInfo(&BpState,&BpPrintInfo);
	}

	//----> 20240116
	if( m_pMainFrame ){
		m_pMainFrame->GetScoreLinkInfo( m_isScoreLinkCall, m_ScoreLinkPdfNam );
	}
	//<---- 20240116

	itm_name = GetBusuItmName(BpState,m_Util.GetKazeihoushiki(m_pZmSub->zvol),BpPrintInfo.ForkKey);
	for(ii=0; ii<3; ii++)	{
		filter.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq=%d"),DBSYZSHIN_APLNAME,itm_name,ii+1);
		if(m_pZmSub->owntb->Requery(filter) == -1)	{
			// 新規登録
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
			m_pZmSub->owntb->itm_name = itm_name;
			m_pZmSub->owntb->itm_seq = ii+1;
			for(jj=0; jj<20; jj++)	{
				if(kk < 50)	{
					m_pZmSub->owntb->vl[jj] = m_PrintCmInfo.busu[kk];
					kk++;
				}
				else	{
					m_pZmSub->owntb->vl[jj] = 1;	// 部数の初期値は "1"
				}
				m_pZmSub->owntb->str[jj] = _T("");
				m_pZmSub->owntb->code[jj] = _T("");
			}
		}
		else	{
			m_pZmSub->owntb->Edit();
			for(jj=0; jj<20; jj++)	{
				if(kk < 50)	{
					m_pZmSub->owntb->vl[jj] = m_PrintCmInfo.busu[kk];
					kk++;
				}
			}
		}
		m_pZmSub->owntb->Update();
	}
}

//-----------------------------------------------------------------------------
// 一括印刷をエラー状態にセット
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetBpPrintErr()
{
	if( m_pMainFrame ){
		m_pMainFrame->SetBpPrintErr();
	}
}

//-----------------------------------------------------------------------------
// 出力キャンセル
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetOptionMode()
{
	m_Vprn.SetOptionMode( POF_PWCLOSE );
}

//-----------------------------------------------------------------------------
// owntbに書き込む部数のitm_nameを返す【部数指定】
//-----------------------------------------------------------------------------
// 引　数	int		：	1=一括印刷
//			int		：	ID_ICSSH_GENSOKU=原則課税　ID_ICSSH_KANNI=簡易課税
//			CString	：	一括印刷のキー名
// 返送値	CString	：	item_name
//-----------------------------------------------------------------------------
CString CDBSyzShinMainView::GetBusuItmName( int pSw1, int pSw2, CString pKeyName )
{
	CString	rcs=_T("");

	if( pSw1 && m_isScoreLinkCall == -1 ){
//	if(pSw1)	{
		if(pSw2 == ID_ICSSH_GENSOKU)	{
			rcs.Format(_T("%s_gb"),pKeyName);
		}
		else	{
			rcs.Format(_T("%s_kb"),pKeyName);
		}
	}
	else	{
		if(pSw2 == ID_ICSSH_GENSOKU)	{
			rcs.Format(_T("%s_gb"),DBSYZSHIN_ITMNAME);
		}
		else	{
			rcs.Format(_T("%s_kb"),DBSYZSHIN_ITMNAME);
		}
	}
	return(rcs);
}


//-----------------------------------------------------------------------------
// 該当ビットが立っているかどうか）
//-----------------------------------------------------------------------------
// BitFlg どのD?をみるか　15まで
// 返送値	TRUE	：	正常終了
//			FALSE	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::IsBitFlg(int target, int BitFlg) {
	int st = 0;

	switch (BitFlg) {

		case 0:
			if (target & 0x01)	st = 1;
			break;
		case 1:
			if (target & 0x02)	st = 1;
			break;
		case 2:
			if (target & 0x04)	st = 1;
			break;
		case 3:
			if (target & 0x08)	st = 1;
			break;
		case 4:
			if (target & 0x10)	st = 1;
			break;
		case 5:
			if (target & 0x20)	st = 1;
			break;
		case 6:
			if (target & 0x40)	st = 1;
			break;
		case 7:
			if (target & 0x80)	st = 1;
			break;
		case 8:
			if (target & 0x100) st = 1;
			break;
		case 9:
			if (target & 0x200)	st = 1;
			break;
		case 10:
			if (target & 0x400)	st = 1;
			break;
		case 11:
			if (target & 0x800)	st = 1;
			break;
		case 12:
			if (target & 0x1000)	st = 1;
			break;
		case 13:
			if (target & 0x2000)	st = 1;
			break;
		case 14:
			if (target & 0x4000)	st = 1;
			break;
		case 15:
			if (target & 0x8000)	st = 1;
			break;

	}
	return st;
}




//-----------------------------------------------------------------------------
// 該当ビットを立てる
//-----------------------------------------------------------------------------
// BitFlg どのD?をみるか　15まで
// 返送値	TRUE	：	正常終了
//			FALSE	：	エラー
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetBitFlg(int& target, int BitFlg) {
	int st = 0;

	switch (BitFlg) {

	case 0:
		target |= 0x01;		break;
	case 1:
		target |= 0x02;		break;
	case 2:
		target |= 0x04;		break;
	case 3:
		target |= 0x08;		break;
	case 4:
		target |= 0x10;		break;
	case 5:
		target |= 0x20;		break;
	case 6:
		target |= 0x40;		break;
	case 7:
		target |= 0x80;		break;
	case 8:
		target |= 0x100;		break;
	case 9:
		target |= 0x200;		break;
	case 10:
		target |= 0x400;		break;
	case 11:
		target |= 0x800;		break;
	case 12:
		target |= 0x1000;		break;
	case 13:
		target |= 0x2000;		break;
	case 14:
		target |= 0x4000;		break;
	case 15:
		target |= 0x8000;		break;

	}
}
