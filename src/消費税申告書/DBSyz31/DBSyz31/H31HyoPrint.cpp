// H31HyoPrint.cpp : 印刷部 実装ファイル
//

#include "stdafx.h"
#include "H31HyoView.h"

#include "ShinFhyo6.h"
#include "ShinFhyo5_012.h"
#include "ShinFhyo5_022.h"
#include "ShinFhyo5_032.h"

const	int	KSK_BUF_SIZE = 512 * 20;

const	int CULM_PER_PAGE_F6	=	4;

const	int CULM_PER_PAGE_TOKUREI_URI1 = 3;
const	int CULM_PER_PAGE_TOKUREI_URI2 = 2;
const	int CULM_PER_PAGE_TOKUREI_SHIIRE = 2;
const	int MAX_PAGE_TOKUREI_URI1 = 5;
const	int MAX_PAGE_TOKUREI_URI2 = 5;
const	int MAX_PAGE_TOKUREI_SHIIRE = 5;

//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn			：	出力クラス
//		pPrtPar			：	印刷情報
//		pPrintCmInfo	：	印刷共有情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar, SH_PRINT_CMINFO *pPrintCmInfo )
{
	// 情報セット
	ASSERT( pVprn );
	if( pVprn == NULL ){
		return -1;
	}
	m_pVprn = pVprn;

	ASSERT( pPrtPar );
	if( pPrtPar == NULL ){
		return -1;
	}
	m_pPset = pPrtPar;

	ASSERT( pPrintCmInfo );
	if( pPrintCmInfo == NULL ){
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

	if ( m_pSnHeadData->m_DispTabSgn & 0x80 ) {
	    InitTokureiTblInfo();
	    ClearTokureiData();
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 本表印刷
//-----------------------------------------------------------------------------
// 引数	pDC		：	デバイスコンテキスト
//		isHikae	：	控え印刷かどうか
//		PPno	：
//		fno		：	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CH31HyoView::PrintHonPyo( CDC *pDC, BOOL isHikae, int PPno )
{
	char			*AP = NULL;
	long symd = 0, eymd = 0;

	unsigned int	size = 2048;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}

	// 課税期間の取得
	GetKazeiKikan( symd, eymd );

	int st = 0;

//----->('23.05.01 Add)	
	if ( eymd >= ICS_SH_INVOICE_DAY ) {
		if (!st) st = PrintHonPyoSubEX2_1(pDC, isHikae, PPno, AP);
		if (!st) st = PrintHonPyoSubEX2_2(pDC, isHikae, PPno, AP);
		if (!st) st = PrintHonPyoSubEX2_3(pDC, isHikae, PPno, AP);
		if (!st) st = PrintHonPyoSubEX2_4(pDC, isHikae, PPno, AP);
	}
//<-----
	else if(( eymd >= R04REFORM ) && ( m_pSnHeadData->IsSoleProprietor() )) {
		// 令和4年4月日以降の課税期間かつ、個人業種
		if( !st ) {
			st = PrintHonPyoSubEX1(pDC, isHikae, PPno, AP);
		}
		if( !st ) {
			st = PrintHonPyoSubEX2(pDC, isHikae, PPno, AP);
		}
		if( !st ) {
			st = PrintHonPyoSubEX3(pDC, isHikae, PPno, AP);
		}
		if( !st ) {
			st = PrintHonPyoSubEX4(pDC, isHikae, PPno, AP);
		}

	}
	else {
		if (!st) st = PrintHonPyoSub1(pDC, isHikae, PPno, AP);
		if (!st) st = PrintHonPyoSub2(pDC, isHikae, PPno, AP);
		if (!st) st = PrintHonPyoSub3(pDC, isHikae, PPno, AP);
		if (!st) st = PrintHonPyoSub4(pDC, isHikae, PPno, AP);
	}

	if (AP) {
		delete AP;
		AP = NULL;
	}

	return 0;
}

int CH31HyoView::PrintHonPyoSub1(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド１を印字

	int				i, j;
	char			*dp, buf[512];
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	memset(PF1, '\0', sizeof(PF1));

	// 提出年月日
	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	if ((yymmdd[0] & 0xff) || (yymmdd[1] & 0xff) || (yymmdd[2] & 0xff)) {
		for (i = j = 0; j < 3; ++j, dp += 3) {
			if (yymmdd[j] & 0xff) {
				wsprintf(dp, _T("%2X"), yymmdd[j] & 0xff);
			}
			else {
				strcpy_s(dp, 2 + 1, "  ");
			}
			PF1[i].PITM[j] = dp;
		}
	}
	else {
		pbclr(0, 3, &PF1[(i = 0)]);
	}

	// 税務署長名
	i++;
	memmove(dp, m_pSnHeadData->Sn_ZMSYO, 6 * 2);
	PF1[i++].PITM[0] = dp;
	dp += 13;

	// 納税地
	if (!m_pPrintCmInfo->OP3) {
		// 郵便番号
		if (j = (int)strlen((char *)m_pSnHeadData->Sn_ZIP)) {
			strcpy_s(dp, 2 + 1, "〒");
			memmove((dp + 2), m_pSnHeadData->Sn_ZIP, j);
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else {
			PF1[i++].PITM[0] = (char *)0;
		}

		// 納税地（２行）
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[0], 20 * 2);
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[40], 20 * 2);
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 2, &PF1[i++]);
	}

	// 納税地電話番号
	if (!m_pPrintCmInfo->OPE) {
		memset(buf, '\0', sizeof(buf));
		// 電話番号
		memmove(buf, m_pSnHeadData->Sn_TELNO1, 6);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		j = (int)strlen(buf);
		memmove(dp, "      ", 6);
		memmove((dp + (6 - j)), buf, 6);
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO2, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));

		PF1[i].PITM[1] = dp;	dp += 5;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO3, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else {
		pbclr(0, 3, &PF1[i++]);
	}

	// 氏名又は屋号	
	if (!m_pPrintCmInfo->OPF) {
		// 法人名（振りがな）
		memmove(dp, m_pSnHeadData->Sn_COFRI, 80);
		PF1[i++].PITM[0] = dp;
		dp += 81;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 法人名
	if (!m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM) {
		memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者（振りがな）
	if (!m_pPrintCmInfo->OPG) {
		memmove(dp, m_pSnHeadData->Sn_DIFRI, 68);
		PF1[i++].PITM[0] = dp;
		dp += 69;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者名
	if (!m_pPrintCmInfo->OP5) {
		memmove(dp, m_pSnHeadData->Sn_DIHYO, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// マイナンバー
	CICSMNSub8 mnsub;
	if ((isHikae&&m_pSnHeadData->IsSoleProprietor()) || IsNotPrintHojinNo()) {
		pbclr(0, 1, &PF1[i++]);
	}
	else {
		CString myno_str, myno_prn;
		if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
			if (m_pPset->psSigCoprn & 0x4000) {
				myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
			}
			else {
				myno_prn = myno_str;
			}
			if (m_pSnHeadData->IsSoleProprietor() == TRUE) {
				myno_prn = ' ' + myno_prn;
			}
			memmove(dp, myno_prn, 13);
			PF1[i++].PITM[0] = dp;
			dp += 14;
		}
		else {
			pbclr(0, 1, &PF1[i++]);
		}
	}

	// 一連番号
	if (((m_pSnHeadData->Sn_ITIREN[0] & 0xff) != 0xff) && (m_pSnHeadData->IsSoleProprietor() == FALSE)) {

		memset(buf, '\0', sizeof(buf));
		m_Arith.l_unpac(buf, m_pSnHeadData->Sn_ITIREN, 8);
		for (j = (int)strlen(buf); j > 0; ) {
			if (buf[--j] != 0x20) {
				break;
			}
		}
		++j;
		if (j < 8) {
			memmove(dp, "        ", 8 - j);
		}
		memmove((dp + (8 - j)), buf, j);
		PF1[i++].PITM[0] = dp;
		dp += 9;
	}
	else {
		PF1[i++].PITM[0] = (char *)0;
	}

	int CCSw = 0;

	// 整理番号等出力サイン
	if ((m_pPrintCmInfo->PRtype & 0x80) ||
		((!m_pPrintCmInfo->OP1) || (IsNotPrintGengoCircle() == false)) ) {

		int gengo = 0;
		if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
			pbclr(0, 1, &PF1[i++]);
			pbclr(0, 6, &PF1[i]);
			if (IsNotPrintGengoCircle() == false) {
				// 申告期間自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);

			}
		}
		else {
			if ((m_pSnHeadData->Sn_SEIRI[0] & 0xff) != 0xff) {
				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
				for (j = (int)strlen(buf); j > 0; ) {
					if (buf[--j] != 0x20) {
						break;
					}
				}
				++j;
				if (j < 8) {
					memmove(dp, "        ", 8 - j);
				}
				// 整理番号
				memmove((dp + (8 - j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 9;
			}
			else {
				PF1[i++].PITM[0] = (char *)0;
			}

			if (CCSw) {	// 強制期間変更
				for (j = 0; j < 3; ++j, dp += 3) {				// 申告期間自
					PF1[i].PITM[j] = dp;
				}
				for (; j < 6; ++j, dp += 3) {					// 申告期間至
					PF1[i].PITM[j] = dp;
				}

				//i++;
				//PF1[i].PITM[0] = (char*)0;
				//PF1[i].PITM[1] = (char*)0;
			}
			else {
				gengo = 0;
				// 申告期間自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				for (j = 0; j < 3; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
					PF1[i].PITM[j] = dp;
				}
				// 申告期間至
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				for (; j < 6; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
					PF1[i].PITM[j] = dp;
				}
			}

		}
		//i++;
		if (IsNotPrintGengoCircle()) {
			pbclr(0, 2, &PF1[++i]);
			
		}
		else {
			if (CCSw) {	// 強制期間変更
				i++;
				PF1[i].PITM[0] = (char *)0;
				PF1[i].PITM[1] = (char *)0;
			}
			else {
				i++;
				if (IsNotPrintGengoCircle()) {
					gengo = -1;
				}
				if (gengo == ID_ICSSH_REIWA_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = (char*)0;
					PF1[i].PITM[1] = dp;
					dp += 5;
				}
				else if (gengo == ID_ICSSH_HEISEI_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = dp;
					PF1[i].PITM[1] = (char*)0;
					dp += 5;
				}
				else {
					PF1[i].PITM[0] = (char*)0;
					PF1[i].PITM[1] = (char*)0;
				}
			}
				
		}
			
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 6, &PF1[i]);
		pbclr(0, 2, &PF1[++i]);
	}

	switch (m_pSnHeadData->Sn_SKKBN) {
	case 1:
		if (!(m_pPrintCmInfo->PRtype & 0x80)) {
			// ＯＣＲ用紙
			if (m_pPrintCmInfo->OPC) {
				PF1[++i].PITM[0] = (char *)0;
			}
			else {
				if (m_pSnHeadData->Sn_Syuusei & 0x10) {
					PF1[++i].PITM[0] = _T("準確定");
				}
				else {
					PF1[++i].PITM[0] = _T("　確定　");
				}
			}
		}
		else {
			if (m_pSnHeadData->Sn_Syuusei & 0x10) {
				PF1[++i].PITM[0] = _T("準確定");
			}
			else {
				PF1[++i].PITM[0] = _T("　確定　");
			}
		}
		break;
	case 2:
		PF1[++i].PITM[0] = _T("　中間　");
		break;
	case 3:
		PF1[++i].PITM[0] = _T("修正確定");
		break;
	case 4:
		PF1[++i].PITM[0] = _T("修正中間");
		break;
	}

	// 整理番号等出力サイン
	if ((m_pPrintCmInfo->PRtype & 0x80) ||
		(!m_pPrintCmInfo->OP1) || (IsNotPrintGengoCircle() == false)) {

		int gengo = 0;
		//中間申告期間自･至
		switch (m_pSnHeadData->Sn_SKKBN) {
		case 1:
		case 3:
			pbclr(0, 6, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			break;
		default:

			if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
				pbclr(0, 6, &PF1[++i]);
				if (IsNotPrintGengoCircle() == false) {
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				}
			}
			else {
				if (CCSw) {	// 強制期間変更
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}
					for (; j < 6; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}

					//i++;
					//PF1[i].PITM[0] = ( char* )0;
					//PF1[i].PITM[1] = ( char* )0;
				}
				else {
					gengo = 0;
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
						PF1[i].PITM[j] = dp;
					}
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					for (; j < 6; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
						PF1[i].PITM[j] = dp;
					}

				}
			}

			if (IsNotPrintGengoCircle()) {
				pbclr(0, 2, &PF1[++i]);
	
			}
			else {
				if (CCSw) {	// 強制期間変更
					i++;
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = (char *)0;
				}
				else{
					if (IsNotPrintGengoCircle()) {
						gengo = -1;
					}

					i++;
					if (gengo == ID_ICSSH_REIWA_GENGO) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char*)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if (gengo == ID_ICSSH_HEISEI_GENGO) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char*)0;
						dp += 5;
					}
					else {
						PF1[i].PITM[0] = (char*)0;
						PF1[i].PITM[1] = (char*)0;
					}
				}
			}
			break;
		}
	}
	else {
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 2, &PF1[++i]);
	}

	return _xprn(PP, 1, PF1, 0, pDC, isNoImage);
}

int CH31HyoView::PrintHonPyoSub2(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド２を印字

	int				i, j;
	char			*dp;
	// 税理士法人
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = { 0 };

	char	PW0[6], PW1[6];
	memset(PW0, '\0', 6);
	m_Arith.l_input(PW0, _T("1000"));
	memset(PW1, '\0', 6);
	m_Arith.l_input(PW1, _T("100"));

	i = j = 0;

	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) { //原則
		GetSyzSyukei_Ippan();
	}
	else if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) { //簡易
		GetSyzSyukei_Kani();
	}

	//①　課税標準額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM1, PW0);
	//m_Arith.l_print(dp, WORK0, FMT12EX);
	m_Arith.l_print(dp, WORK0, FMT011EX);

	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//②　消費税額
	//m_Arith.l_print(dp, m_pSnHonpyoData->Sn_SYTX, FMT12EX);
	m_Arith.l_print(dp, XM2, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//③　控除過大調整税額
	m_Arith.l_print(dp, XM3, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//④　控除対象仕入税額
	m_Arith.l_print(dp, XM4, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑤　返還等対価に係る税額
	m_Arith.l_print(dp, XM5, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑥　貸倒れに係る税額
	m_Arith.l_print(dp, XM6, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑦　控除税額小計
	m_Arith.l_print(dp, XM7, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑧　控除不足還付税額
	m_Arith.l_print(dp, XM8, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑨　差引税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM9, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑩　中間納付税額
	if ( !( m_pPrintCmInfo->PRtype & 0x80 ) && m_pPrintCmInfo->OPB ) {
		PF1[i].PITM[j++] = ( char* )0;
	}
	else {
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, XM10, PW1);
		m_Arith.l_print(dp, WORK0, FMT12);

		PF1[i].PITM[j++] = dp;
		dp += 17;
	}

	//⑪　納付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM11, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);

	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑫　中間納付還付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM12, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);

	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑬　既確定税額
	m_Arith.l_print(dp, XM13, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑭　差引納付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM14, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;
	
	i++;
	j = 0;

	//⑮　課税資産の譲渡等の対価の額
	m_Arith.l_print(dp, XM15, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑯　資産の譲渡等の対価の額
	m_Arith.l_print(dp, XM16, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑰　控除不足還付税額
	m_Arith.l_print(dp, XM17, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;
	
	i++;
	j = 0;

	//⑱　差引税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM18, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑲　還付額
	m_Arith.l_print(dp, XM19, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//20　納税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM20, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//21　中間納付譲渡割額
	if ( !( m_pPrintCmInfo->PRtype & 0x80 ) && m_pPrintCmInfo->OPB  ) {
		PF1[i].PITM[j++] = ( char* )0;
	}
	else {
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, XM21, PW1);
		m_Arith.l_print(dp, WORK0, FMT12);
		PF1[i].PITM[j++] = dp;
		dp += 17;
	}

	//22　納付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM22, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//23　中間納付還付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM23, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//24　既確定譲渡割額
	m_Arith.l_print(dp, XM24, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//25　差引納付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM25, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//26　消費税及び地方消費税の合計(納付又は還付)税額
	m_Arith.l_print(dp, XM26, FMT130EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	return _xprn(PP, 2, PF1, 0, pDC, isNoImage);

}

int CH31HyoView::PrintHonPyoSub3(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド３を印字
	int				i, j, k;
	char			*dp, buf[512];
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = { 0 };

	char	PW0[6];
	memset(PW0, '\0', 6);
	m_Arith.l_input(PW0, _T("1000"));

	// 税理士法　施行前帳票	スケジュール　スキップ
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// 付記事項
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "○" : "　");	// 割賦基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "　" : "○");	
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE ? "○" : "　");	// 延払基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE ? "　" : "○");	
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI ? "○" : "　");  // 工事進行基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI ? "　" : "○");  
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "○" : "　");	// 現金主義会計の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "　" : "○");
	PF1[i].PITM[j++] = ((m_pSnHeadData->Sn_TUMIAG & 0x01) ? "○" : "　"); // 課税標準額に対する～
	PF1[i++].PITM[j] = ((m_pSnHeadData->Sn_TUMIAG & 0x01) ? "　" : "○");

	j = 0;
	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
		if (m_pSnHeadData->IsUseSemiUriRatio()) {
			PF1[i].PITM[j++] = "○";
			PF1[i].PITM[j++] = "　";
			PF1[i].PITM[j++] = (char *)0;
		}
		else {
			if (!(m_pSnHeadData->Sn_Sign4 & 0x10) &&
				((m_pSnHeadData->IsRatioOver95() == TRUE) || (m_pSnHeadData->Sn_EXP100 & 0x01))) {	// 全額控除
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = "○";
			}
			else {
				if (m_pSnHeadData->IsKobetuSiireAnbun()) {
					PF1[i].PITM[j++] = ("○");
					PF1[i].PITM[j++] = ("　");
				}
				else {
					PF1[i].PITM[j++] = ("　");
					PF1[i].PITM[j++] = ("○");
				}
				PF1[i].PITM[j++] = (char *)0;
			}
		}

		i++;
		j = 0;

		//基準期間の課税売上高
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0);
		m_Arith.l_print(dp, WORK0, FMT14);
		PF1[i].PITM[j++] = dp;
		dp += 19;
	}
	else {

		char PW[MONY_BUF_SIZE] = { 0 };
		char WORK[MONY_BUF_SIZE] = { 0 };
		memset ( PW, '\0', MONY_BUF_SIZE );
		m_Arith.l_input ( PW, _T( "1000" ) );

		// 事業別課税売上高
		//第１種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI1 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI1, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第２種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI2 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI2, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第３種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI3 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI3, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第４種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI4 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI4, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第５種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI5 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI5, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第６種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI6 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI6, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		i++;
		j = 0;

		// 事業別課税売上割合
		j = 0;
		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP1 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP1 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP2 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP2 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP3 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP3 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP4 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP4 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP5 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP5 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP6 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", (m_pSnHonpyoData)->Sn_UP6 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i++].PITM[j] = dp;
		dp += 4;


		PF1[i].PITM[0] = ((m_pSnHeadData->Sn_TOKUR) ? "○" : "　");			// 特例計算適用
		PF1[i].PITM[1] = ((m_pSnHeadData->Sn_TOKUR) ? "　" : "○");
	}

	pbclr(0, 2, &PF1[++i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);
	pbclr(0, 1, &PF1[i + 4]);

	//yoshida190606
	if ((m_Arith.l_test(XM26) < 0) || m_pPrintCmInfo->OPI) {	// 還付金融機関表示されてなかった修正
//	if (/*(m_Arith.l_test(m_pSnHonpyoData->) < 0) ||*/ m_pPrintCmInfo->OPI) {	// 還付金融機関
		if (!m_pSnHeadData->Sn_BYsw) {			// 銀行
			memmove(dp, m_pSnHeadData->Sn_BANK, 10 * 2);
			PF1[i].PITM[0] = dp;
			dp += 21;

			memmove(dp, m_pSnHeadData->Sn_SITEN, 10 * 2);
			PF1[i++].PITM[1] = dp;
			dp += 21;

			memmove(dp, m_pSnHeadData->Sn_YOKIN, 4 * 2);
			PF1[i++].PITM[0] = dp;
			dp += 9;

			memset(buf, '\0', sizeof(buf));
			m_Arith.l_unpac(buf, m_pSnHeadData->Sn_KOZA, 10);
			for (j = (int)strlen(buf); j > 0; ) {
				if (buf[--j] != 0x20) {
					break;
				}
			}
			++j;

			if (j < 10) {
				memmove(dp, "          ", 10 - j);
			}
			// 口座番号
			memmove((dp + (10 - j)), buf, j);
			PF1[i++].PITM[0] = dp;
			dp += 11;
		}
		else {
			i += 3;
			memmove(dp, m_pSnHeadData->Sn_YUBIN, 8 * 2);
			PF1[i++].PITM[0] = dp;
			dp += 17;

			memset(buf, '\0', sizeof(buf));
			memmove(buf, m_pSnHeadData->Sn_YUNUM1, 5);
			j = (int)strlen(buf);
			memset(buf, '\0', sizeof(buf));
			memmove(buf, m_pSnHeadData->Sn_YUNUM2, 8);
			k = (int)strlen(buf);
			if (j || k) {
				strcpy_s(buf, 8 + 1, "        ");											//?
				if (j) {
					memmove(&buf[0], m_pSnHeadData->Sn_YUNUM1, j);
				}
				if (k) {
					memmove(&buf[((m_pPrintCmInfo->PRtype & 0x80) ? 7 : 8)], m_pSnHeadData->Sn_YUNUM2, k);
				}
				dp = &buf[0];
			}
			PF1[i++].PITM[0] = dp;
		}
	}

	return _xprn(PP, 3, PF1, 0, pDC, isNoImage);
}

int CH31HyoView::PrintHonPyoSub4(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド４～１４を印字

	int				i, j, k, h;
	char			*dp, buf[512];
	// 税理士法人
	int				Zlng, Zsw;
	char			Znam[128] = { 0 }, Smei[128] = { 0 };
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	int st = 0;

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// 整理番号等出力サイン
	pbclr(0, 1, &PF1[i]);
	if ((m_pPrintCmInfo->PRtype & 0x80) || !m_pPrintCmInfo->OP1) {
		if ((m_pSnHeadData->Sn_ZSEIRI[0] & 0xff) != 0xff) {
			char	tmpbuf[512] = { 0 };
			memset(tmpbuf, '\0', sizeof(tmpbuf));
			if ((m_pSnHeadData->Sn_ZSEIRI[3] & 0x0f) != 0x0f) {
				j = 8;
			}
			else {
				j = 7;
			}
			l_unpac(tmpbuf, m_pSnHeadData->Sn_ZSEIRI, j);
			if (j == 8) {				// 8桁入力の場合:先頭0はカット
				if (tmpbuf[0] != 0x30) {
					tmpbuf[7] = '\0';		// 以外は後1桁カット
				}
				else {
					for (j = 0; j < 7; ++j) {
						tmpbuf[j] = tmpbuf[j + 1];
					}
					tmpbuf[j] = '\0';
				}
				j = 7;
			}
			for (j = (int)strlen(tmpbuf); j > 0; ) {
				if (tmpbuf[--j] != 0x20) {
					break;
				}
			}
			++j;

			// 税務署整理番号
			memset(buf, '\0', sizeof(buf));
			memmove(buf, "        ", 8);
			memmove(&buf[7 - j], tmpbuf, j);
			memmove(dp, &buf[0], 8);
			PF1[i].PITM[0] = dp;
			dp += 9;
		}
	}


	int	 rt = 0;
	int	 Zimusyolng = 0;
	char Zimusyonam[128] = {0};

	// 税理士名出力
	for (++i, j = 0, k = 0; j < 2; ++j) {
		pbclr(0, 1, &PF1[i + j]);
	}
	Zlng = Zsw = 0;
	memset(Znam, '\0', sizeof(Znam));
	memset(Smei, '\0', sizeof(Smei));

	int	SmlFontZrtype = 0;	// 0:通常, 1:全角20文字より大

	if (!m_pPrintCmInfo->OP2) {
		if ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01)) {
			// 税理士氏名を手書きする為に、事務署名＆税理士法人名を縮小で印刷
			// フィールド No.10で印刷
			if ((j = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40) {
				j = 40;
			}
			memmove(Znam, m_pShinInfo->pO_DATA->o_ofcnam, j);
			Zlng = m_Util.MixSpaceCutLength(Znam, j);
			memmove(dp, "　", 2);
			PF1[i].PITM[0] = dp;
			dp += (2 + 1);
		}
		else {
			if (m_pPrintCmInfo->OPA != 2) {
				// 税理士氏名又は税理士法人名
				rt = MakeOutZeirishiName(Znam, sizeof(Znam), 1);
				j = (int)strlen(Znam);
				Zlng = m_Util.MixSpaceCutLength(Znam, j);

				if (Zlng > 40) {
					memmove(dp, "　", 2);
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					SmlFontZrtype = 1;
				}
				else if(Zlng > 26){
					memmove(dp, Znam, Zlng);
					PF1[i].PITM[0] = dp;
					dp += (j + 1);
				}
				else {					// １３文字以下は全角でフィールド No.8で印刷
					memmove(dp, "　", 2);
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					Zsw = 1;
				}
			}
		}
	}

	// 税理士電話番号
	if (!m_pPrintCmInfo->OP9) {
		for (j = 0; j < 19; ++j) {
			buf[j] = 0x20;
		}
		buf[j] = '\0';
		for (++i, j = k = 0; (j < 30) || m_pShinInfo->pO_DATA->o_telno[j]; ++j) {
			if ((m_pShinInfo->pO_DATA->o_telno[j] == '-') || (m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ') ||
				(m_pShinInfo->pO_DATA->o_telno[j] == '(') || (m_pShinInfo->pO_DATA->o_telno[j] == ')')) {
				switch (k) {
				case 0:
					k = ((j < 6) ? j : 6);
					memmove(&buf[6 - k], &m_pShinInfo->pO_DATA->o_telno[0], k);
					k = 1;	h = j + 1;
					continue;
				case 1:
					if ((j - h) < 5) {
						k = (j - h);
					}
					else {
						k = 5;
					}
					memmove(&buf[8], &m_pShinInfo->pO_DATA->o_telno[h], k);
					memmove(&buf[14], &m_pShinInfo->pO_DATA->o_telno[++j], 5);
					k = 2;
					break;
				default:
					break;
				}
				if (k > 1) {
					break;
				}
			}
		}
		if (!k) {
			memmove(buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 19 ? 19 : j));
		}
		else {
			if (k == 1) {
				memmove(&buf[8], &m_pShinInfo->pO_DATA->o_telno[h], 4);
			}
		}
		memmove(dp, buf, 19);

		PF1[i].PITM[0] = dp;
		dp += 20;
	}

	if (st = _xprn(PP, 4, PF1, 0, pDC, isNoImage)) return st;

	// 還付を受けようとする金融期間	
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	pbclr(0, 1, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);
	pbclr(0, 1, &PF1[i + 4]);
	pbclr(0, 1, &PF1[i + 5]);
	pbclr(0, 1, &PF1[i + 6]);
	pbclr(0, 1, &PF1[i + 7]);
	pbclr(0, 1, &PF1[i + 8]);
	pbclr(0, 1, &PF1[i + 9]);

	//yoshida190606
	if ((m_Arith.l_test(XM26) < 0) || m_pPrintCmInfo->OPI) {	// 還付金融機関表示されてなかった修正
//	if (/*(m_Arith.l_test(m_pSnHonpyoData->) < 0) ||*/ m_pPrintCmInfo->OPI) {	// 還付金融機関
		if (!m_pSnHeadData->Sn_BYsw) {			// 銀行
			switch (m_pSnHeadData->Sn_FINANCE & 0x0f) {
			case 0:
				PF1[i].PITM[0] = " ";
				break;
			case 1:
				PF1[i + 1].PITM[0] = " ";
				break;
			case 2:
				PF1[i + 2].PITM[0] = " ";
				break;
			case 3:
				PF1[i + 3].PITM[0] = " ";
				break;
			case 4:
				PF1[i + 4].PITM[0] = " ";
				break;
			default:
				break;
			}

			switch (m_pSnHeadData->Sn_CHAIN & 0x0f) {
			case 0:
				PF1[i + 5].PITM[0] = " ";
				break;
			case 1:
				PF1[i + 6].PITM[0] = " ";
				break;
			case 2:
				PF1[i + 7].PITM[0] = " ";
				break;
			case 3:
				PF1[i + 8].PITM[0] = " ";
				break;
			case 5:
				PF1[i + 9].PITM[0] = " ";
				break;
			default:
				break;
			}
		}
	}

	if (st = _xprn(PP, 5, PF1, 0, pDC, isNoImage)) return st;

	// 欄外印字項目出力
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	pbclr(0, 1, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);

	pbclr(0, 1, &PF1[i + 4]);		// 個人	下段①
	pbclr(0, 1, &PF1[i + 5]);		// 〃	個人
	pbclr(0, 1, &PF1[i + 6]);		// 〃	上段
	pbclr(0, 1, &PF1[i + 7]);		// 〃	下段
	pbclr(0, 1, &PF1[i + 8]);		// 〃	税務署処理欄　区分３
	// 拡張		
	pbclr(0, 1, &PF1[i + 9]);		// 法人	下段
	pbclr(0, 1, &PF1[i + 10]);		// 〃　	署別一連番号
	// 所管
	pbclr(0, 1, &PF1[i + 11]);

	// 欄外下拡張
	char	ADRDX[128], ADRD2[20], ADRD4[50];
	int		ln2, ln4;
	memset(ADRDX, '\0', sizeof(ADRDX));
	memset(ADRDX, 0x20, 60);
	memset(ADRD2, '\0', sizeof(ADRD2));
	memmove(ADRD2, m_pSnHeadData->Sn_ADRD2, 16);
	memset(ADRD4, '\0', sizeof(ADRD4));
	memmove(ADRD4, m_pSnHeadData->Sn_ADRD4, 44);;
	ln2 = (int)strlen(ADRD2);
	ln4 = (int)strlen(ADRD4);
	memmove(ADRDX, ADRD4, ln4);
	memmove(&ADRDX[44], ADRD2, ln2);

	if (!m_pPrintCmInfo->OP7 && (m_pSnHeadData->IsSoleProprietor() == FALSE)) {
		memmove(dp, m_pSnHeadData->Sn_ADRU1, 30);
		PF1[i].PITM[0] = dp;
		dp += 32;

		memmove(dp, m_pSnHeadData->Sn_ADRU2, 20);
		PF1[i + 1].PITM[0] = dp;
		dp += 22;

		memmove(dp, m_pSnHeadData->Sn_ADRC1, 20);
		PF1[i + 2].PITM[0] = dp;
		dp += 22;

		memmove(dp, m_pSnHeadData->Sn_ADRC2, 20);
		PF1[i + 3].PITM[0] = dp;
		dp += 22;

		if (m_pSnHeadData->IsSoleProprietor()) {
			memmove(dp, ADRDX, 60);
			PF1[i + 4].PITM[0] = dp;
			dp += 62;

			if (m_pSnHeadData->Sn_PPTYP & 0x01) {
				memmove(dp, "個人", 4);
				PF1[i + 5].PITM[0] = dp;
				dp += 6;
			}
			memmove(dp, m_pSnHeadData->Sn_ADKUP, 10);
			PF1[i + 6].PITM[0] = dp;
			dp += 12;

			memmove(dp, m_pSnHeadData->Sn_ADKDW, 10);
			PF1[i + 7].PITM[0] = dp;
			dp += 12;

			memmove(dp, m_pSnHeadData->Sn_ADKNO, 1);
			PF1[i + 8].PITM[0] = dp;
			dp += 2;
		}
		else {
			memmove(dp, ADRDX, 60);
			PF1[i + 9].PITM[0] = dp;
			dp += 62;
			memmove(dp, m_pSnHeadData->Sn_ADSNO, 7);
			PF1[i + 10].PITM[0] = dp;
			dp += 9;
		}
	}

	if ((m_pPrintCmInfo->PRtype & 0x80) || !m_pPrintCmInfo->OP1) {		// 整理番号等出力サイン
		memmove(dp, m_pSnHeadData->Sn_SYOKN, 3);
		PF1[i + 11].PITM[0] = dp;
		dp += 5;
	}

	if (st = _xprn(PP, 6, PF1, 0, pDC, isNoImage)) return st;

	// 税理士法　施行後帳票
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	pbclr(0, 1, &PF1[0]);
	pbclr(0, 1, &PF1[1]);

	if (m_pSnHeadData->Sn_ZeiHou & 0x01) {
		PF1[0].PITM[0] = "○";
	}
	else {
		PF1[0].PITM[0] = (char *)0;
	}

	if (m_pSnHeadData->Sn_ZeiHou & 0x02) {
		PF1[1].PITM[0] = "○";
	}
	else {
		PF1[1].PITM[0] = (char *)0;
	}
	// 個人改正帳票対応
	if (m_pSnHeadData->IsSoleProprietor()) {		// 個人
		// 一連番号
		if ((m_pSnHeadData->Sn_ITIREN[0] & 0xff) != 0xff) {
			memset(buf, '\0', sizeof(buf));
			m_Arith.l_unpac(buf, m_pSnHeadData->Sn_ITIREN, 8);
			for (j = (int)strlen(buf); j > 0; ) {
				if (buf[--j] != 0x20) {
					break;
				}
			}
			++j;
			if (j < 8) {
				memmove(dp, "        ", 8 - j);
			}
			memmove((dp + (8 - j)), buf, j);
			PF1[2].PITM[0] = dp;
			dp += 9;
		}
		else {
			PF1[2].PITM[0] = (char *)0;
		}

// 220412 del -->
		//// 送付
		//if (IsNoDeliForm()) {
		//	PF1[3].PITM[0] = (char *)0;
		//}
		//else {
		//	if ((m_pSnHeadData->Sn_ZeiHou & 0x04)) {
		//		PF1[3].PITM[0] = "○";
		//	}
		//	else {
		//		PF1[3].PITM[0] = (char *)0;
		//	}
		//}
// 220412 del <--
// 220412 add -->
		// 翌年以降送付不要欄のカット
		PF1[3].PITM[0] = (char *)0;
// 220412 add <--
	}
	else {
		PF1[2].PITM[0] = (char *)0;

// 220412 del -->
		//// 送付
		//if (IsNoDeliForm()) {
		//	PF1[3].PITM[0] = (char *)0;
		//}
		//else {
		//	if ((m_pSnHeadData->Sn_ZeiHou & 0x04)) {
		//		PF1[3].PITM[0] = "○";
		//	}
		//	else {
		//		PF1[3].PITM[0] = (char *)0;
		//	}
		//}
// 220412 del <--
// 220412 add -->
		// 翌年以降送付不要欄のカット
		PF1[3].PITM[0] = (char *)0;
// 220412 add <--
	}

	if (st = _xprn(PP, 7, PF1, 0, pDC, isNoImage)) return st;

	pbclr(0, 1, &PF1[0]);
	pbclr(0, 1, &PF1[1]);

	// memo
	// OPA		→1：事務所名 / 2：事務所名と税理士名
	// OPSign	→0x01：事務所名を縮小
	// OP2		→事務所名と税理士名のどっちかにチェックがついてたら0で来る。2つともチェックなしなら1で来る。

	if (!m_pPrintCmInfo->OP2) {
		if ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01)) {	// 「事務所名」と「事務所名を縮小して出力」にチェック
			// 事務署名＆税理士法人名を縮小で印刷
			// 税理士法 施行後
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;

			// 税理士法 施行後
			pbclr(0, 1, &PF1[0]);
			memmove(dp, Znam, Zlng);
			PF1[0].PITM[0] = dp;
			dp += (Zlng + 1);
			if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;


			// 税理士法　施行後 追加！
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
		}
		else {
			if (m_pPrintCmInfo->OPA == 2) {	// 「事務所名」と「税理士名」にチェック

				if( m_pPrintCmInfo->OPsign & 0x01 ){	// 「事務所名を縮小して出力」にチェック
					
					// 事務署名＆税理士法人名を縮小で印刷
					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法 施行後
					if ((Zlng = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40) {
						Zlng = 40;
					}
					memmove(Znam, m_pShinInfo->pO_DATA->o_ofcnam, Zlng);
					pbclr(0, 1, &PF1[0]);
					memmove(dp, Znam, Zlng);
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法　施行後　税理士氏名
					rt = MakeOutZeirishiName(Smei, sizeof(Smei), 1);
					Zlng = (int)strlen(Smei);
					if (Zlng > 27) {	// 14文字以上なら縮小させる
						memset(Znam, '\0', sizeof(Znam));
						memmove(Znam, Smei, Zlng);
						pbclr(0, 1, &PF1[0]);

						if( Zlng <= 40 ){
							SmlFontZrtype = 2;
						}
						else{
							SmlFontZrtype = 3;
						}
					}
					else {
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Smei, Zlng);
						PF1[0].PITM[0] = dp;
						dp += (Zlng + 1);
					}
					if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
				}
				else {	// 「事務所名」・「税理士名」を通常フォントで表示

					// 事務所名
					Zimusyolng = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam);
					if( Zimusyolng > 26 ){	// 13文字以上なら縮小させる
						memmove(Zimusyonam, m_pShinInfo->pO_DATA->o_ofcnam, Zimusyolng);
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Zimusyonam, Zimusyolng);
						PF1[0].PITM[0] = dp;
						dp += (Zimusyolng + 1);
						if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;
					}
					else{
						memmove(Zimusyonam, m_pShinInfo->pO_DATA->o_ofcnam, Zimusyolng);
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Zimusyonam, Zimusyolng);
						PF1[0].PITM[0] = dp;
						dp += (Zimusyolng + 1);
						if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
					}


					// 税理士名
					rt = MakeOutZeirishiName(Smei, sizeof(Smei), 1);
					Zlng = (int)strlen(Smei);
					if (Zlng > 26 ) {	// 13文字以上なら縮小させる
						memset(Znam, '\0', sizeof(Znam));
						memmove(Znam, Smei, Zlng);
						pbclr(0, 1, &PF1[0]);

						if ( Zlng <= 40 ) {
							SmlFontZrtype = 2;
						}
						else {
							SmlFontZrtype = 3;
						}
					}
					else {
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Smei, Zlng);
						PF1[0].PITM[0] = dp;
						dp += (Zlng + 1);
					}
					if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
				}
			}
			else {	// 事務所名のみ
				if (Zsw) {
					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					memmove(dp, Znam, Zlng);
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;
					// 税理士法　施行後 追加！
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
				}
			}
		}
	}

	if (m_pPrintCmInfo->OPM) {	// 被合併名称
		dp = AP;
		i = j = 0;
		memset(dp, '\0', size);
		pbclr(0, 1, &PF1[0]);
		pbclr(0, 1, &PF1[1]);

		// 法人名
		if (!m_pPrintCmInfo->OP4) {
			memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
			PF1[0].PITM[0] = dp;
			dp += 41;
		}
		memmove(dp, m_pSnHeadData->Sn_GPNAM, 20 * 2);
		PF1[1].PITM[0] = dp;
		dp += 41;
		if (st = _xprn(PP, 13, PF1, 0, pDC, isNoImage)) return st;
	}


	// 税理士名で最小フォント（事務所名にチェックがないとき）
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 1) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 14, PF1, 0, pDC, isNoImage)) return st;

	// 事務所名＋税理士名で最小フォント
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 2) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 15, PF1, 0, pDC, isNoImage)) return st;


	//準確定の場合選択的に文言を出力
	if (m_pSnHeadData->Sn_Syuusei & 0x10) {
		if (!m_pPrintCmInfo->OPV) {
			dp = AP;
			memset(dp, '\0', size);
			PF1[0].PITM[0] = _T("被相続人");
			if (st = _xprn(PP, 16, PF1, 0, pDC, isNoImage)) return st;	// 使うのは個人マスターの時のみ
		}
	}

	// 税理士名が13文字以上の時
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if( SmlFontZrtype == 3 ){
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else{
		pbclr(0, 1, &PF1[0]);
	}
	if(st = _xprn(PP, 19, PF1, 0, pDC, isNoImage)) return st;


	// 税理士区分+税理士名で20文字超えるときは直接受任の文言を税理士名の下に出す
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if ( m_pSnHeadData->m_ZroutSgn & 0x02 ) {
		if( rt == 1 ){
			CString str;
			str.Format( "（直接受任）" );
			int lng = str.GetLength();
			memmove(dp, str, lng);
			PF1[i].PITM[0] = dp;
			dp += (lng + 1);
		}
		else{
			pbclr(0, 1, &PF1[1]);
		}
		if( (SmlFontZrtype != 3) && (SmlFontZrtype != 1) ){
			if(st = _xprn(PP, 20, PF1, 0, pDC, isNoImage))	return st;
		}
		else{	// フォント小
			if(st = _xprn(PP, 22, PF1, 0, pDC, isNoImage))	return st;
		}
	}


	// 個人番号欄に斜線
	if ( m_pSnHeadData->IsSoleProprietor()) {
		if (isHikae) {
			int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
			int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
			m_pVprn->DrawSline(0, LKnrW0, 3650 + xPlus, 5360 + yPlus, 9750 + xPlus, 4850 + yPlus);
		}
	}

	// 電子申告完了済 
	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn) && isHikae) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 1L) + (PV1 / 4) + yPlus;
		long XPosition = (PH * 4L) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, "ＭＳ　ゴシック");
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar)) {
			return 1;
		}

		YPosition = PV1 + (PV1 / 4) + yPlus;
		XPosition = (PH * 12L) + xPlus;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar)) {
			return 1;
		}

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);
	}

	// 控用
	if (isHikae && m_pPrintCmInfo->OPD) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 2) + (PV1 / 2) + yPlus;
		long XPosition = (PH * 56L) + (PH / 3) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb | SFp_yc);			// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (8L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("【控用】"), &SfPar)) {
			return 1;
		}

		m_pVprn->DeleteRegFont(Gothic108);
	}

	// 新型コロナウイルスによる申告・納付期限延長申請
	if ( m_Util.IsBasicMst ( m_pZmSub->zvol ) && m_pSnHeadData->Sn_GenericSgn & 0x10 ) {
		CString str;
		int lng = 0;

		dp = AP;
		memset ( dp, '\0', size );
		str.Format ( _T( "新型コロナウイルスによる申告・" ) );
		lng = str.GetLength();
		memmove ( dp, str, lng );
		PF1[0].PITM[0] = dp;
		dp += ( lng + 1 );

		str.Format ( _T( "納付期限延長申請" ) );
		lng = str.GetLength();
		memmove ( dp, str, lng );
		PF1[1].PITM[0] = dp;

		if ( st = _xprn ( PP, 23, PF1, 0, pDC, isNoImage ) ) {
			return st;
		}
	}

	return 0;
}

//============================================================================
// 本表印刷（令和4年4月1日改正様式用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX1(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド１を印字

	int				i, j;
	char			*dp, buf[512];
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	memset(PF1, '\0', sizeof(PF1));

	// 提出年月日
	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	if ((yymmdd[0] & 0xff) || (yymmdd[1] & 0xff) || (yymmdd[2] & 0xff)) {
		for (i = j = 0; j < 3; ++j, dp += 3) {
			if (yymmdd[j] & 0xff) {
				wsprintf(dp, _T("%2X"), yymmdd[j] & 0xff);
			}
			else {
				strcpy_s(dp, 2 + 1, "  ");
			}
			PF1[i].PITM[j] = dp;
		}
	}
	else {
		pbclr(0, 3, &PF1[(i = 0)]);
	}

	// 税務署長名
	i++;
	memmove(dp, m_pSnHeadData->Sn_ZMSYO, 6 * 2);
	PF1[i++].PITM[0] = dp;
	dp += 13;

	// 納税地
	if (!m_pPrintCmInfo->OP3) {
		// 郵便番号
		if (j = (int)strlen((char *)m_pSnHeadData->Sn_ZIP)) {
			strcpy_s(dp, 2 + 1, "〒");
			memmove((dp + 2), m_pSnHeadData->Sn_ZIP, j);
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else {
			PF1[i++].PITM[0] = (char *)0;
		}

		// 納税地（２行）
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[0], 20 * 2);
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[40], 20 * 2);
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 2, &PF1[i++]);
	}

	// 納税地電話番号
	if (!m_pPrintCmInfo->OPE) {
		memset(buf, '\0', sizeof(buf));
		// 電話番号
		memmove(buf, m_pSnHeadData->Sn_TELNO1, 6);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		j = (int)strlen(buf);
		memmove(dp, "      ", 6);
		memmove((dp + (6 - j)), buf, 6);
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO2, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));

		PF1[i].PITM[1] = dp;	dp += 5;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO3, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else {
		pbclr(0, 3, &PF1[i++]);
	}

	// 氏名又は屋号	
	if (!m_pPrintCmInfo->OPF) {
		// 法人名（振りがな）
		memmove(dp, m_pSnHeadData->Sn_COFRI, 80);
		PF1[i++].PITM[0] = dp;
		dp += 81;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 法人名
	if (!m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM) {
		memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者（振りがな）
	if (!m_pPrintCmInfo->OPG) {
		memmove(dp, m_pSnHeadData->Sn_DIFRI, 68);
		PF1[i++].PITM[0] = dp;
		dp += 69;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者名
	if (!m_pPrintCmInfo->OP5) {
		memmove(dp, m_pSnHeadData->Sn_DIHYO, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// マイナンバー
	CICSMNSub8 mnsub;
	if ((isHikae&&m_pSnHeadData->IsSoleProprietor()) || IsNotPrintHojinNo()) {
		pbclr(0, 1, &PF1[i++]);
	}
	else {
		CString myno_str, myno_prn;
		if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
			if (m_pPset->psSigCoprn & 0x4000) {
				myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
			}
			else {
				myno_prn = myno_str;
			}
			if (m_pSnHeadData->IsSoleProprietor() == TRUE) {
				myno_prn = ' ' + myno_prn;
			}
			memmove(dp, myno_prn, 13);
			PF1[i++].PITM[0] = dp;
			dp += 14;
		}
		else {
			pbclr(0, 1, &PF1[i++]);
		}
	}

	// （個人の方）振替継続希望
	PF1[i++].PITM[0] = (( m_pSnHeadData->Sn_GenericSgn & 0x200 ) ? _T( "○" ) : _T( "　" ));

	int CCSw = 0;

	// 整理番号等出力サイン
	if ((m_pPrintCmInfo->PRtype & 0x80) || (!m_pPrintCmInfo->OP1)) {

		int gengo = 0;
		if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
			pbclr(0, 1, &PF1[i++]);
			pbclr(0, 6, &PF1[i]);

			// 申告期間自
			memset(yymmdd, '\0', sizeof(yymmdd));
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
		}
		else {
			if ((m_pSnHeadData->Sn_SEIRI[0] & 0xff) != 0xff) {
				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
				for (j = (int)strlen(buf); j > 0; ) {
					if (buf[--j] != 0x20) {
						break;
					}
				}
				++j;
				if (j < 8) {
					memmove(dp, "        ", 8 - j);
				}
				// 整理番号
				memmove((dp + (8 - j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 9;
			}
			else {
				PF1[i++].PITM[0] = (char *)0;
			}

			if (CCSw) {	// 強制期間変更
				for (j = 0; j < 3; ++j, dp += 3) {				// 申告期間自
					PF1[i].PITM[j] = dp;
				}
				for (; j < 6; ++j, dp += 3) {					// 申告期間至
					PF1[i].PITM[j] = dp;
				}

			}
			else {
				gengo = 0;
				// 申告期間自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				for (j = 0; j < 3; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
					PF1[i].PITM[j] = dp;
				}
				// 申告期間至
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				for (; j < 6; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
					PF1[i].PITM[j] = dp;
				}
			}

		}
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 6, &PF1[i]);
		pbclr(0, 2, &PF1[++i]);
	}

	switch (m_pSnHeadData->Sn_SKKBN) {
	case 1:
		if (!(m_pPrintCmInfo->PRtype & 0x80)) {
			// ＯＣＲ用紙
			if (m_pPrintCmInfo->OPC) {
				PF1[++i].PITM[0] = (char *)0;
			}
			else {
				if (m_pSnHeadData->Sn_Syuusei & 0x10) {
					PF1[++i].PITM[0] = _T("準確定");
				}
				else {
					PF1[++i].PITM[0] = _T("　確定　");
				}
			}
		}
		else {
			if (m_pSnHeadData->Sn_Syuusei & 0x10) {
				PF1[++i].PITM[0] = _T("準確定");
			}
			else {
				PF1[++i].PITM[0] = _T("　確定　");
			}
		}
		break;
	case 2:
		PF1[++i].PITM[0] = _T("　中間　");
		break;
	case 3:
		PF1[++i].PITM[0] = _T("修正確定");
		break;
	case 4:
		PF1[++i].PITM[0] = _T("修正中間");
		break;
	}

	// 整理番号等出力サイン
	if ((m_pPrintCmInfo->PRtype & 0x80) || (!m_pPrintCmInfo->OP1)) {

		int gengo = 0;
		//中間申告期間自･至
		switch (m_pSnHeadData->Sn_SKKBN) {
		case 1:
		case 3:
			pbclr(0, 6, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			break;
		default:

			if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
				pbclr(0, 6, &PF1[++i]);
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
			}
			else {
				if (CCSw) {	// 強制期間変更
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}
					for (; j < 6; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}

				}
				else {
					gengo = 0;
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
						PF1[i].PITM[j] = dp;
					}
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					for (; j < 6; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
						PF1[i].PITM[j] = dp;
					}

				}
			}

			break;
		}
	}
	else {
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 2, &PF1[++i]);
	}

	return _xprn(PP, 1, PF1, 0, pDC, isNoImage);
}

//============================================================================
// 本表印刷（令和4年4月1日改正様式用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX2(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド２を印字

	int				i, j;
	char			*dp;
	// 税理士法人
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = { 0 };

	char	PW0[6], PW1[6];
	memset(PW0, '\0', 6);
	m_Arith.l_input(PW0, _T("1000"));
	memset(PW1, '\0', 6);
	m_Arith.l_input(PW1, _T("100"));

	i = j = 0;

	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) { //原則
		GetSyzSyukei_Ippan();
	}
	else if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) { //簡易
		GetSyzSyukei_Kani();
	}

	//①　課税標準額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM1, PW0);
	m_Arith.l_print(dp, WORK0, FMT011EX);

	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//②　消費税額
	m_Arith.l_print(dp, XM2, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//③　控除過大調整税額
	m_Arith.l_print(dp, XM3, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//④　控除対象仕入税額
	m_Arith.l_print(dp, XM4, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑤　返還等対価に係る税額
	m_Arith.l_print(dp, XM5, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑥　貸倒れに係る税額
	m_Arith.l_print(dp, XM6, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑦　控除税額小計
	m_Arith.l_print(dp, XM7, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑧　控除不足還付税額
	m_Arith.l_print(dp, XM8, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑨　差引税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM9, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑩　中間納付税額
	if ( !( m_pPrintCmInfo->PRtype & 0x80 ) && m_pPrintCmInfo->OPB ) {
		PF1[i].PITM[j++] = ( char* )0;
	}
	else {
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, XM10, PW1);
		m_Arith.l_print(dp, WORK0, FMT12);

		PF1[i].PITM[j++] = dp;
		dp += 17;
	}

	//⑪　納付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM11, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);

	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑫　中間納付還付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM12, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);

	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑬　既確定税額
	m_Arith.l_print(dp, XM13, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑭　差引納付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM14, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑮　課税資産の譲渡等の対価の額
	m_Arith.l_print(dp, XM15, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑯　資産の譲渡等の対価の額
	m_Arith.l_print(dp, XM16, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑰　控除不足還付税額
	m_Arith.l_print(dp, XM17, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑱　差引税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM18, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑲　還付額
	m_Arith.l_print(dp, XM19, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//20　納税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM20, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//21　中間納付譲渡割額
	if ( !( m_pPrintCmInfo->PRtype & 0x80 ) && m_pPrintCmInfo->OPB  ) {
		PF1[i].PITM[j++] = ( char* )0;
	}
	else {
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, XM21, PW1);
		m_Arith.l_print(dp, WORK0, FMT12);
		PF1[i].PITM[j++] = dp;
		dp += 17;
	}

	//22　納付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM22, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//23　中間納付還付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM23, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//24　既確定譲渡割額
	m_Arith.l_print(dp, XM24, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//25　差引納付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM25, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//26　消費税及び地方消費税の合計(納付又は還付)税額
	m_Arith.l_print(dp, XM26, FMT130EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	return _xprn(PP, 2, PF1, 0, pDC, isNoImage);

}

//============================================================================
// 本表印刷（令和4年4月1日改正様式用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX3(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド３を印字
	int				i, j, k;
	char			*dp, buf[512];
	BOOL			isNoImage = FALSE;

	// 課税期間の取得
	long symd = 0, eymd = 0;
	GetKazeiKikan(symd, eymd);

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = { 0 };

	char	PW0[6];
	memset(PW0, '\0', 6);
	m_Arith.l_input(PW0, _T("1000"));

	// 税理士法　施行前帳票	スケジュール　スキップ
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// 付記事項
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "○" : "　");	// 割賦基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "　" : "○");	
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE ? "○" : "　");	// 延払基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE ? "　" : "○");	
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI ? "○" : "　");  // 工事進行基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI ? "　" : "○");  
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "○" : "　");	// 現金主義会計の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "　" : "○");
	//----->課税期間が令和5年10月1日以後開始の場合、「課税標準額に対する～…特例の適用」の〇印字を無しに('23.07.11 cor)
	if (symd >= ICS_SH_INVOICE_DAY) {							// 課税標準額に対する～
		PF1[i].PITM[j++] = "　";
		PF1[i++].PITM[j] = "　";
	}
	else {
		PF1[i].PITM[j++] = ((m_pSnHeadData->Sn_TUMIAG & 0x01) ? "○" : "　");
		PF1[i++].PITM[j] = ((m_pSnHeadData->Sn_TUMIAG & 0x01) ? "　" : "○");
	}
	//<-----

	j = 0;
	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
		if (m_pSnHeadData->IsUseSemiUriRatio()) {
			PF1[i].PITM[j++] = "○";
			PF1[i].PITM[j++] = "　";
			PF1[i].PITM[j++] = (char *)0;
		}
		else {
			if (!(m_pSnHeadData->Sn_Sign4 & 0x10) &&
				((m_pSnHeadData->IsRatioOver95() == TRUE) || (m_pSnHeadData->Sn_EXP100 & 0x01))) {	// 全額控除
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = "○";
			}
			else {
				if (m_pSnHeadData->IsKobetuSiireAnbun()) {
					PF1[i].PITM[j++] = ("○");
					PF1[i].PITM[j++] = ("　");
				}
				else {
					PF1[i].PITM[j++] = ("　");
					PF1[i].PITM[j++] = ("○");
				}
				PF1[i].PITM[j++] = (char *)0;
			}
		}

		i++;
		j = 0;

		//基準期間の課税売上高
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0);
		m_Arith.l_print(dp, WORK0, FMT14);
		PF1[i].PITM[j++] = dp;
		dp += 19;
	}
	else {

		char PW[MONY_BUF_SIZE] = { 0 };
		char WORK[MONY_BUF_SIZE] = { 0 };
		memset ( PW, '\0', MONY_BUF_SIZE );
		m_Arith.l_input ( PW, _T( "1000" ) );

		// 事業別課税売上高
		//第１種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI1 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI1, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第２種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI2 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI2, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第３種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI3 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI3, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第４種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI4 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI4, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第５種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI5 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI5, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		//第６種
		memset ( WORK, '\0', MONY_BUF_SIZE );
		if ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI6 ) >= 0 ) {
			memmove ( WORK, m_pSnHonpyoData->Sn_KURI6, MONY_BUF_SIZE );
			m_Util.l_calq ( WORK, WORK, 2 );
			m_Arith.l_div ( WORK, WORK, PW );
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12E)) {
				m_Arith.l_print(dp, WORK, FMT11Z);
			}
			else {
				m_Arith.l_print(dp, WORK, FMT11);
			}
		}
		PF1[i].PITM[j++] = dp;
		dp += 12;

		i++;
		j = 0;

		// 事業別課税売上割合
		j = 0;
		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP1 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP1 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP2 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP2 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP3 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP3 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP4 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP4 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP5 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP5 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP6 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", (m_pSnHonpyoData)->Sn_UP6 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i++].PITM[j] = dp;
		dp += 4;


		PF1[i].PITM[0] = ((m_pSnHeadData->Sn_TOKUR) ? "○" : "　");			// 特例計算適用
		PF1[i].PITM[1] = ((m_pSnHeadData->Sn_TOKUR) ? "　" : "○");
	}

	pbclr(0, 1, &PF1[++i]);
	pbclr(0, 2, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);
	pbclr(0, 1, &PF1[i + 4]);
	pbclr(0, 1, &PF1[i + 5]);

	//yoshida190606
	if ((m_Arith.l_test(XM26) < 0) || m_pPrintCmInfo->OPI) {	// 還付金融機関表示されてなかった修正
	// （個人の方）公金受取口座の利用
		PF1[i++].PITM[0] = (( m_pSnHeadData->Sn_GenericSgn & 0x400 ) ? _T( "○" ) : _T( "　" ));

		//yoshida221226
		if (!(m_pSnHeadData->Sn_GenericSgn & 0x400)) {//公金受取口座に○がない場合のみ金融機関出力
			if (!m_pSnHeadData->Sn_BYsw) {			// 銀行
				memmove(dp, m_pSnHeadData->Sn_BANK, 10 * 2);
				PF1[i].PITM[0] = dp;
				dp += 21;

				memmove(dp, m_pSnHeadData->Sn_SITEN, 10 * 2);
				PF1[i++].PITM[1] = dp;
				dp += 21;

				memmove(dp, m_pSnHeadData->Sn_YOKIN, 4 * 2);
				PF1[i++].PITM[0] = dp;
				dp += 9;

				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_KOZA, 10);
				for (j = (int)strlen(buf); j > 0; ) {
					if (buf[--j] != 0x20) {
						break;
					}
				}
				++j;

				if (j < 10) {
					memmove(dp, "          ", 10 - j);
				}
				// 口座番号
				memmove((dp + (10 - j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 11;
			}
			else {
				i += 3;
				memmove(dp, m_pSnHeadData->Sn_YUBIN, 8 * 2);
				PF1[i++].PITM[0] = dp;
				dp += 17;

				memset(buf, '\0', sizeof(buf));
				memmove(buf, m_pSnHeadData->Sn_YUNUM1, 5);
				j = (int)strlen(buf);
				memset(buf, '\0', sizeof(buf));
				memmove(buf, m_pSnHeadData->Sn_YUNUM2, 8);
				k = (int)strlen(buf);
				if (j || k) {
					strcpy_s(buf, 8 + 1, "        ");											//?
					if (j) {
						memmove(&buf[0], m_pSnHeadData->Sn_YUNUM1, j);
					}
					if (k) {
						memmove(&buf[((m_pPrintCmInfo->PRtype & 0x80) ? 7 : 8)], m_pSnHeadData->Sn_YUNUM2, k);
					}
					dp = &buf[0];
				}
				PF1[i++].PITM[0] = dp;
			}
		}
	}

	return _xprn(PP, 3, PF1, 0, pDC, isNoImage);
}

//============================================================================
// 本表印刷（令和4年4月1日改正様式用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX4(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド４～１４を印字

	int				i, j, k, h;
	char			*dp, buf[512];
	// 税理士法人
	int				Zlng, Zsw;
	char			Znam[128] = { 0 }, Smei[128] = { 0 };
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	int st = 0;

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// 整理番号等出力サイン
	pbclr(0, 1, &PF1[i]);
	if ((m_pPrintCmInfo->PRtype & 0x80) || !m_pPrintCmInfo->OP1) {
		if ((m_pSnHeadData->Sn_ZSEIRI[0] & 0xff) != 0xff) {
			char	tmpbuf[512] = { 0 };
			memset(tmpbuf, '\0', sizeof(tmpbuf));
			if ((m_pSnHeadData->Sn_ZSEIRI[3] & 0x0f) != 0x0f) {
				j = 8;
			}
			else {
				j = 7;
			}
			l_unpac(tmpbuf, m_pSnHeadData->Sn_ZSEIRI, j);
			if (j == 8) {				// 8桁入力の場合:先頭0はカット
				if (tmpbuf[0] != 0x30) {
					tmpbuf[7] = '\0';		// 以外は後1桁カット
				}
				else {
					for (j = 0; j < 7; ++j) {
						tmpbuf[j] = tmpbuf[j + 1];
					}
					tmpbuf[j] = '\0';
				}
				j = 7;
			}
			for (j = (int)strlen(tmpbuf); j > 0; ) {
				if (tmpbuf[--j] != 0x20) {
					break;
				}
			}
			++j;

			// 税務署整理番号
			memset(buf, '\0', sizeof(buf));
			memmove(buf, "        ", 8);
			memmove(&buf[7 - j], tmpbuf, j);
			memmove(dp, &buf[0], 8);
			PF1[i].PITM[0] = dp;
			dp += 9;
		}
	}


	int	 rt = 0;
	int	 Zimusyolng = 0;
	char Zimusyonam[128] = {0};

	// 税理士名出力
	for (++i, j = 0, k = 0; j < 2; ++j) {
		pbclr(0, 1, &PF1[i + j]);
	}
	Zlng = Zsw = 0;
	memset(Znam, '\0', sizeof(Znam));
	memset(Smei, '\0', sizeof(Smei));

	int	SmlFontZrtype = 0;	// 0:通常, 1:全角20文字より大

	if (!m_pPrintCmInfo->OP2) {
		if ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01)) {
			// 税理士氏名を手書きする為に、事務署名＆税理士法人名を縮小で印刷
			// フィールド No.10で印刷
			if ((j = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40) {
				j = 40;
			}
			memmove(Znam, m_pShinInfo->pO_DATA->o_ofcnam, j);
			Zlng = m_Util.MixSpaceCutLength(Znam, j);
			memmove(dp, "　", 2);
			PF1[i].PITM[0] = dp;
			dp += (2 + 1);
		}
		else {
			if (m_pPrintCmInfo->OPA != 2) {
				// 税理士氏名又は税理士法人名
				rt = MakeOutZeirishiName(Znam, sizeof(Znam), 1);
				j = (int)strlen(Znam);
				Zlng = m_Util.MixSpaceCutLength(Znam, j);

				if (Zlng > 40) {
					memmove(dp, "　", 2);
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					SmlFontZrtype = 1;
				}
				else if(Zlng > 26){
					memmove(dp, Znam, Zlng);
					PF1[i].PITM[0] = dp;
					dp += (j + 1);
				}
				else {					// １３文字以下は全角でフィールド No.8で印刷
					memmove(dp, "　", 2);
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					Zsw = 1;
				}
			}
		}
	}

	// 税理士電話番号
	if (!m_pPrintCmInfo->OP9) {
		for (j = 0; j < 19; ++j) {
			buf[j] = 0x20;
		}
		buf[j] = '\0';
		for (++i, j = k = 0; (j < 30) || m_pShinInfo->pO_DATA->o_telno[j]; ++j) {
			if ((m_pShinInfo->pO_DATA->o_telno[j] == '-') || (m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ') ||
				(m_pShinInfo->pO_DATA->o_telno[j] == '(') || (m_pShinInfo->pO_DATA->o_telno[j] == ')')) {
				switch (k) {
				case 0:
					k = ((j < 6) ? j : 6);
					memmove(&buf[6 - k], &m_pShinInfo->pO_DATA->o_telno[0], k);
					k = 1;	h = j + 1;
					continue;
				case 1:
					if ((j - h) < 5) {
						k = (j - h);
					}
					else {
						k = 5;
					}
					memmove(&buf[8], &m_pShinInfo->pO_DATA->o_telno[h], k);
					memmove(&buf[14], &m_pShinInfo->pO_DATA->o_telno[++j], 5);
					k = 2;
					break;
				default:
					break;
				}
				if (k > 1) {
					break;
				}
			}
		}
		if (!k) {
			memmove(buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 19 ? 19 : j));
		}
		else {
			if (k == 1) {
				memmove(&buf[8], &m_pShinInfo->pO_DATA->o_telno[h], 4);
			}
		}
		memmove(dp, buf, 19);

		PF1[i].PITM[0] = dp;
		dp += 20;
	}

	if (st = _xprn(PP, 4, PF1, 0, pDC, isNoImage)) return st;

	// 還付を受けようとする金融期間	
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	pbclr(0, 1, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);
	pbclr(0, 1, &PF1[i + 4]);
	pbclr(0, 1, &PF1[i + 5]);
	pbclr(0, 1, &PF1[i + 6]);
	pbclr(0, 1, &PF1[i + 7]);
	pbclr(0, 1, &PF1[i + 8]);
	pbclr(0, 1, &PF1[i + 9]);

	//yoshida190606
	if ((m_Arith.l_test(XM26) < 0) || m_pPrintCmInfo->OPI) {	// 還付金融機関表示されてなかった修正

		//yoshida221226
		if (!(m_pSnHeadData->Sn_GenericSgn & 0x400)) {//公金受取口座に○がない場合のみ金融機関出力

			if (!m_pSnHeadData->Sn_BYsw) {			// 銀行
				switch (m_pSnHeadData->Sn_FINANCE & 0x0f) {
				case 0:
					PF1[i].PITM[0] = " ";
					break;
				case 1:
					PF1[i + 1].PITM[0] = " ";
					break;
				case 2:
					PF1[i + 2].PITM[0] = " ";
					break;
				case 3:
					PF1[i + 3].PITM[0] = " ";
					break;
				case 4:
					PF1[i + 4].PITM[0] = " ";
					break;
				default:
					break;
				}

				switch (m_pSnHeadData->Sn_CHAIN & 0x0f) {
				case 0:
					PF1[i + 5].PITM[0] = " ";
					break;
				case 1:
					PF1[i + 6].PITM[0] = " ";
					break;
				case 2:
					PF1[i + 7].PITM[0] = " ";
					break;
				case 3:
					PF1[i + 8].PITM[0] = " ";
					break;
				case 5:
					PF1[i + 9].PITM[0] = " ";
					break;
				default:
					break;
				}
			}
		}
	}

	if (st = _xprn(PP, 5, PF1, 0, pDC, isNoImage)) return st;

	// 欄外印字項目出力
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	pbclr(0, 1, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);

	pbclr(0, 1, &PF1[i + 4]);		// 個人	下段①
	pbclr(0, 1, &PF1[i + 5]);		// 〃	個人
	pbclr(0, 1, &PF1[i + 6]);		// 〃	上段
	pbclr(0, 1, &PF1[i + 7]);		// 〃	下段
	pbclr(0, 1, &PF1[i + 8]);		// 〃	税務署処理欄　区分３
									// 拡張		
	pbclr(0, 1, &PF1[i + 9]);		// 法人	下段
	pbclr(0, 1, &PF1[i + 10]);		// 〃　	署別一連番号
									// 所管
	pbclr(0, 1, &PF1[i + 11]);

	// 欄外下拡張
	char	ADRDX[128], ADRD2[20], ADRD4[50];
	int		ln2, ln4;
	memset(ADRDX, '\0', sizeof(ADRDX));
	memset(ADRDX, 0x20, 60);
	memset(ADRD2, '\0', sizeof(ADRD2));
	memmove(ADRD2, m_pSnHeadData->Sn_ADRD2, 16);
	memset(ADRD4, '\0', sizeof(ADRD4));
	memmove(ADRD4, m_pSnHeadData->Sn_ADRD4, 44);;
	ln2 = (int)strlen(ADRD2);
	ln4 = (int)strlen(ADRD4);
	memmove(ADRDX, ADRD4, ln4);
	memmove(&ADRDX[44], ADRD2, ln2);

	if (!m_pPrintCmInfo->OP7 && (m_pSnHeadData->IsSoleProprietor() == FALSE)) {
		memmove(dp, m_pSnHeadData->Sn_ADRU1, 30);
		PF1[i].PITM[0] = dp;
		dp += 32;

		memmove(dp, m_pSnHeadData->Sn_ADRU2, 20);
		PF1[i + 1].PITM[0] = dp;
		dp += 22;

		memmove(dp, m_pSnHeadData->Sn_ADRC1, 20);
		PF1[i + 2].PITM[0] = dp;
		dp += 22;

		memmove(dp, m_pSnHeadData->Sn_ADRC2, 20);
		PF1[i + 3].PITM[0] = dp;
		dp += 22;

		if (m_pSnHeadData->IsSoleProprietor()) {
			memmove(dp, ADRDX, 60);
			PF1[i + 4].PITM[0] = dp;
			dp += 62;

			if (m_pSnHeadData->Sn_PPTYP & 0x01) {
				memmove(dp, "個人", 4);
				PF1[i + 5].PITM[0] = dp;
				dp += 6;
			}
			memmove(dp, m_pSnHeadData->Sn_ADKUP, 10);
			PF1[i + 6].PITM[0] = dp;
			dp += 12;

			memmove(dp, m_pSnHeadData->Sn_ADKDW, 10);
			PF1[i + 7].PITM[0] = dp;
			dp += 12;

			memmove(dp, m_pSnHeadData->Sn_ADKNO, 1);
			PF1[i + 8].PITM[0] = dp;
			dp += 2;
		}
		else {
			memmove(dp, ADRDX, 60);
			PF1[i + 9].PITM[0] = dp;
			dp += 62;
			memmove(dp, m_pSnHeadData->Sn_ADSNO, 7);
			PF1[i + 10].PITM[0] = dp;
			dp += 9;
		}
	}

	if ((m_pPrintCmInfo->PRtype & 0x80) || !m_pPrintCmInfo->OP1) {		// 整理番号等出力サイン
		memmove(dp, m_pSnHeadData->Sn_SYOKN, 3);
		PF1[i + 11].PITM[0] = dp;
		dp += 5;
	}

	if (st = _xprn(PP, 6, PF1, 0, pDC, isNoImage)) return st;

	// 税理士法　施行後帳票
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	pbclr(0, 1, &PF1[0]);
	pbclr(0, 1, &PF1[1]);

	if (m_pSnHeadData->Sn_ZeiHou & 0x01) {
		PF1[0].PITM[0] = "○";
	}
	else {
		PF1[0].PITM[0] = (char *)0;
	}

	if (m_pSnHeadData->Sn_ZeiHou & 0x02) {
		PF1[1].PITM[0] = "○";
	}
	else {
		PF1[1].PITM[0] = (char *)0;
	}

	if (st = _xprn(PP, 7, PF1, 0, pDC, isNoImage)) return st;

	pbclr(0, 1, &PF1[0]);
	pbclr(0, 1, &PF1[1]);

	// memo
	// OPA		→1：事務所名 / 2：事務所名と税理士名
	// OPSign	→0x01：事務所名を縮小
	// OP2		→事務所名と税理士名のどっちかにチェックがついてたら0で来る。2つともチェックなしなら1で来る。

	if (!m_pPrintCmInfo->OP2) {
		if ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01)) {	// 「事務所名」と「事務所名を縮小して出力」にチェック
																				// 事務署名＆税理士法人名を縮小で印刷
																				// 税理士法 施行後
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;

			// 税理士法 施行後
			pbclr(0, 1, &PF1[0]);
			memmove(dp, Znam, Zlng);
			PF1[0].PITM[0] = dp;
			dp += (Zlng + 1);
			if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;


			// 税理士法　施行後 追加！
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
		}
		else {
			if (m_pPrintCmInfo->OPA == 2) {	// 「事務所名」と「税理士名」にチェック

				if( m_pPrintCmInfo->OPsign & 0x01 ){	// 「事務所名を縮小して出力」にチェック

														// 事務署名＆税理士法人名を縮小で印刷
														// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法 施行後
					if ((Zlng = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40) {
						Zlng = 40;
					}
					memmove(Znam, m_pShinInfo->pO_DATA->o_ofcnam, Zlng);
					pbclr(0, 1, &PF1[0]);
					memmove(dp, Znam, Zlng);
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法　施行後　税理士氏名
					rt = MakeOutZeirishiName(Smei, sizeof(Smei), 1);
					Zlng = (int)strlen(Smei);
					if (Zlng > 27) {	// 14文字以上なら縮小させる
						memset(Znam, '\0', sizeof(Znam));
						memmove(Znam, Smei, Zlng);
						pbclr(0, 1, &PF1[0]);

						if( Zlng <= 40 ){
							SmlFontZrtype = 2;
						}
						else{
							SmlFontZrtype = 3;
						}
					}
					else {
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Smei, Zlng);
						PF1[0].PITM[0] = dp;
						dp += (Zlng + 1);
					}
					if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
				}
				else {	// 「事務所名」・「税理士名」を通常フォントで表示

						// 事務所名
					Zimusyolng = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam);
					if( Zimusyolng > 26 ){	// 13文字以上なら縮小させる
						memmove(Zimusyonam, m_pShinInfo->pO_DATA->o_ofcnam, Zimusyolng);
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Zimusyonam, Zimusyolng);
						PF1[0].PITM[0] = dp;
						dp += (Zimusyolng + 1);
						if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;
					}
					else{
						memmove(Zimusyonam, m_pShinInfo->pO_DATA->o_ofcnam, Zimusyolng);
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Zimusyonam, Zimusyolng);
						PF1[0].PITM[0] = dp;
						dp += (Zimusyolng + 1);
						if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
					}


					// 税理士名
					rt = MakeOutZeirishiName(Smei, sizeof(Smei), 1);
					Zlng = (int)strlen(Smei);
					if (Zlng > 26 ) {	// 13文字以上なら縮小させる
						memset(Znam, '\0', sizeof(Znam));
						memmove(Znam, Smei, Zlng);
						pbclr(0, 1, &PF1[0]);

						if ( Zlng <= 40 ) {
							SmlFontZrtype = 2;
						}
						else {
							SmlFontZrtype = 3;
						}
					}
					else {
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Smei, Zlng);
						PF1[0].PITM[0] = dp;
						dp += (Zlng + 1);
					}
					if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
				}
			}
			else {	// 事務所名のみ
				if (Zsw) {
					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					memmove(dp, Znam, Zlng);
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;
					// 税理士法　施行後 追加！
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
				}
			}
		}
	}

	if (m_pPrintCmInfo->OPM) {	// 被合併名称
		dp = AP;
		i = j = 0;
		memset(dp, '\0', size);
		pbclr(0, 1, &PF1[0]);
		pbclr(0, 1, &PF1[1]);

		// 法人名
		if (!m_pPrintCmInfo->OP4) {
			memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
			PF1[0].PITM[0] = dp;
			dp += 41;
		}
		memmove(dp, m_pSnHeadData->Sn_GPNAM, 20 * 2);
		PF1[1].PITM[0] = dp;
		dp += 41;
		if (st = _xprn(PP, 13, PF1, 0, pDC, isNoImage)) return st;
	}


	// 税理士名で最小フォント（事務所名にチェックがないとき）
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 1) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 14, PF1, 0, pDC, isNoImage)) return st;

	// 事務所名＋税理士名で最小フォント
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 2) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 15, PF1, 0, pDC, isNoImage)) return st;


	//準確定の場合選択的に文言を出力
	if (m_pSnHeadData->Sn_Syuusei & 0x10) {
		if (!m_pPrintCmInfo->OPV) {
			dp = AP;
			memset(dp, '\0', size);
			PF1[0].PITM[0] = _T("被相続人");
			if (st = _xprn(PP, 16, PF1, 0, pDC, isNoImage)) return st;	// 使うのは個人マスターの時のみ
		}
	}

	// 税理士名が13文字以上の時
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if( SmlFontZrtype == 3 ){
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else{
		pbclr(0, 1, &PF1[0]);
	}
	if(st = _xprn(PP, 19, PF1, 0, pDC, isNoImage)) return st;


	// 税理士区分+税理士名で20文字超えるときは直接受任の文言を税理士名の下に出す
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if ( m_pSnHeadData->m_ZroutSgn & 0x02 ) {
		if( rt == 1 ){
			CString str;
			str.Format( "（直接受任）" );
			int lng = str.GetLength();
			memmove(dp, str, lng);
			PF1[i].PITM[0] = dp;
			dp += (lng + 1);
		}
		else{
			pbclr(0, 1, &PF1[1]);
		}
		if( (SmlFontZrtype != 3) && (SmlFontZrtype != 1) ){
			if(st = _xprn(PP, 20, PF1, 0, pDC, isNoImage))	return st;
		}
		else{	// フォント小
			if(st = _xprn(PP, 22, PF1, 0, pDC, isNoImage))	return st;
		}
	}


	// 個人番号欄に斜線
	if ( m_pSnHeadData->IsSoleProprietor()) {
		if (isHikae) {
			int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
			int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
			m_pVprn->DrawSline(0, LKnrW0, 3650 + xPlus, 5360 + yPlus, 9750 + xPlus, 4850 + yPlus);
		}
	}

	// 電子申告完了済 
	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn) && isHikae) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 1L) + (PV1 / 4) + yPlus;
		long XPosition = (PH * 4L) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, "ＭＳ　ゴシック");
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar)) {
			return 1;
		}

		YPosition = PV1 + (PV1 / 4) + yPlus;
		XPosition = (PH * 12L) + xPlus;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar)) {
			return 1;
		}

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);
	}

	// 控用
	if (isHikae && m_pPrintCmInfo->OPD) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 2) + (PV1 / 2) + yPlus;
		long XPosition = (PH * 56L) + (PH / 3) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb | SFp_yc);			// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (8L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("【控用】"), &SfPar)) {
			return 1;
		}

		m_pVprn->DeleteRegFont(Gothic108);
	}

	// 新型コロナウイルスによる申告・納付期限延長申請
	if ( m_Util.IsBasicMst ( m_pZmSub->zvol ) && m_pSnHeadData->Sn_GenericSgn & 0x10 ) {
		CString str;
		int lng = 0;

		dp = AP;
		memset ( dp, '\0', size );
		str.Format ( _T( "新型コロナウイルスによる申告・" ) );
		lng = str.GetLength();
		memmove ( dp, str, lng );
		PF1[0].PITM[0] = dp;
		dp += ( lng + 1 );

		str.Format ( _T( "納付期限延長申請" ) );
		lng = str.GetLength();
		memmove ( dp, str, lng );
		PF1[1].PITM[0] = dp;

		if ( st = _xprn ( PP, 23, PF1, 0, pDC, isNoImage ) ) {
			return st;
		}
	}

	return 0;
}

//============================================================================
// 本表印刷（インボイス２割特例用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX2_1(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド１を印字

	int				i, j;
	char			*dp, buf[512];
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	memset(PF1, '\0', sizeof(PF1));

	// 提出年月日
	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	if ((yymmdd[0] & 0xff) || (yymmdd[1] & 0xff) || (yymmdd[2] & 0xff)) {
		for (i = j = 0; j < 3; ++j, dp += 3) {
			if (yymmdd[j] & 0xff) {
				wsprintf(dp, _T("%2X"), yymmdd[j] & 0xff);
			}
			else {
				strcpy_s(dp, 2 + 1, "  ");
			}
			PF1[i].PITM[j] = dp;
		}
	}
	else {
		pbclr(0, 3, &PF1[(i = 0)]);
	}

	// 税務署長名
	i++;
	memmove(dp, m_pSnHeadData->Sn_ZMSYO, 6 * 2);
	PF1[i++].PITM[0] = dp;
	dp += 13;

	// 納税地
	if (!m_pPrintCmInfo->OP3) {
		// 郵便番号
		if (j = (int)strlen((char *)m_pSnHeadData->Sn_ZIP)) {
			strcpy_s(dp, 2 + 1, "〒");
			memmove((dp + 2), m_pSnHeadData->Sn_ZIP, j);
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else {
			PF1[i++].PITM[0] = (char *)0;
		}

		// 納税地（２行）
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[0], 20 * 2);
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[40], 20 * 2);
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 2, &PF1[i++]);
	}

	// 納税地電話番号
	if (!m_pPrintCmInfo->OPE) {
		memset(buf, '\0', sizeof(buf));
		// 電話番号
		memmove(buf, m_pSnHeadData->Sn_TELNO1, 6);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		j = (int)strlen(buf);
		memmove(dp, "      ", 6);
		memmove((dp + (6 - j)), buf, 6);
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO2, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));

		PF1[i].PITM[1] = dp;	dp += 5;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO3, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else {
		pbclr(0, 3, &PF1[i++]);
	}

	// 氏名又は屋号	
	if (!m_pPrintCmInfo->OPF) {
		// 法人名（振りがな）
		memmove(dp, m_pSnHeadData->Sn_COFRI, 80);
		PF1[i++].PITM[0] = dp;
		dp += 81;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 法人名
	if (!m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM) {
		memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者（振りがな）
	if (!m_pPrintCmInfo->OPG) {
		memmove(dp, m_pSnHeadData->Sn_DIFRI, 68);
		PF1[i++].PITM[0] = dp;
		dp += 69;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者名
	if (!m_pPrintCmInfo->OP5) {
		memmove(dp, m_pSnHeadData->Sn_DIHYO, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// マイナンバー
	CICSMNSub8 mnsub;
	if ((isHikae&&m_pSnHeadData->IsSoleProprietor()) || IsNotPrintHojinNo()) {
		pbclr(0, 1, &PF1[i++]);
	}
	else {
		CString myno_str, myno_prn;
		if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
			if (m_pPset->psSigCoprn & 0x4000) {
				myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
			}
			else {
				myno_prn = myno_str;
			}
			if (m_pSnHeadData->IsSoleProprietor() == TRUE) {
				myno_prn = ' ' + myno_prn;
			}
			memmove(dp, myno_prn, 13);
			PF1[i++].PITM[0] = dp;
			dp += 14;
		}
		else {
			pbclr(0, 1, &PF1[i++]);
		}
	}

	// （個人の方）振替継続希望
	PF1[i++].PITM[0] = ((m_pSnHeadData->Sn_GenericSgn & 0x200) ? _T("○") : _T("　"));


	// !PRtype & 0x80	OCR印刷
	// PRtype & 0x80	白紙印刷
	// OP1 == 0			課税期間、整理番号等出力

	// 整理番号
	if( m_pPrintCmInfo->PRtype & 0x80 || !m_pPrintCmInfo->OP1 ){
		if ((m_pSnHeadData->Sn_SEIRI[0] & 0xff) != 0xff) {
			memset(buf, '\0', sizeof(buf));
			m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
			for (j = (int)strlen(buf); j > 0; ) {
				if (buf[--j] != 0x20) {
					break;
				}
			}
			++j;
			if (j < 8) {
				memmove(dp, "        ", 8 - j);
			}
			// 整理番号
			memmove((dp + (8 - j)), buf, j);
			PF1[i++].PITM[0] = dp;
			dp += 9;
		}
		else{
			PF1[i++].PITM[0] = (char *)0;
		}
	}
	else{
		PF1[i++].PITM[0] = (char *)0;
	}

	// 課税期間、元号の〇（法人のみ）
	if(( m_pPrintCmInfo->PRtype & 0x80 ) || (( !m_pPrintCmInfo->OP1 ) || ( IsNotPrintGengoCircle() == false ))) {
		int gengo = 0;
		if ( !( m_pPrintCmInfo->PRtype & 0x80 ) && m_pPrintCmInfo->OP1 ) {
			pbclr( 0, 6, &PF1[i] );

			memset( yymmdd, '\0', sizeof( yymmdd ));
			m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
		}
		else {
			// 申告期間自
			memset(yymmdd, '\0', sizeof(yymmdd));
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
			for (j = 0; j < 3; ++j, dp += 3) {
				wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
				PF1[i].PITM[j] = dp;
			}
			// 申告期間至
			memset(yymmdd, '\0', sizeof(yymmdd));
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
			for (; j < 6; ++j, dp += 3) {
				wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
				PF1[i].PITM[j] = dp;
			}
		}

		// 元号の〇（法人のみ）
		if( !m_pSnHeadData->IsSoleProprietor() ){

			if (IsNotPrintGengoCircle()) {
				pbclr(0, 2, &PF1[++i]);
			}
			else {
				i++;
				if (gengo == ID_ICSSH_REIWA_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = (char*)0;
					PF1[i].PITM[1] = dp;
					dp += 5;
				}
				else if (gengo == ID_ICSSH_HEISEI_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = dp;
					PF1[i].PITM[1] = (char*)0;
					dp += 5;
				}
				else {
					PF1[i].PITM[0] = (char*)0;
					PF1[i].PITM[1] = (char*)0;
				}		
			}
		}
		else{
			// OCR印刷はスケジュールが法人個人で一本のため、元号の〇を出力する用の記述がある
			// その分を初期化しておく
			if( !(m_pPrintCmInfo->PRtype & 0x80) ){
				pbclr(0, 2, &PF1[++i]);
			}
		}
	}
	else{
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 6, &PF1[i]);
		//pbclr(0, 2, &PF1[++i]);		
	}

	/*int CCSw = 0;

	// 整理番号等出力サイン
	if ((m_pPrintCmInfo->PRtype & 0x80) || (!m_pPrintCmInfo->OP1)) {

		int gengo = 0;
		if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
			pbclr(0, 1, &PF1[i++]);
			pbclr(0, 6, &PF1[i]);

			// 申告期間自
			memset(yymmdd, '\0', sizeof(yymmdd));
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
		}
		else {
			if ((m_pSnHeadData->Sn_SEIRI[0] & 0xff) != 0xff) {
				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
				for (j = (int)strlen(buf); j > 0; ) {
					if (buf[--j] != 0x20) {
						break;
					}
				}
				++j;
				if (j < 8) {
					memmove(dp, "        ", 8 - j);
				}
				// 整理番号
				memmove((dp + (8 - j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 9;
			}
			else {
				PF1[i++].PITM[0] = (char *)0;
			}

			if (CCSw) {	// 強制期間変更
				for (j = 0; j < 3; ++j, dp += 3) {				// 申告期間自
					PF1[i].PITM[j] = dp;
				}
				for (; j < 6; ++j, dp += 3) {					// 申告期間至
					PF1[i].PITM[j] = dp;
				}

			}
			else {
				gengo = 0;
				// 申告期間自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				for (j = 0; j < 3; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
					PF1[i].PITM[j] = dp;
				}
				// 申告期間至
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				for (; j < 6; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
					PF1[i].PITM[j] = dp;
				}
			}

		}
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 6, &PF1[i]);
		pbclr(0, 2, &PF1[++i]);
	}*/

	switch (m_pSnHeadData->Sn_SKKBN) {
	case 1:
		if (!(m_pPrintCmInfo->PRtype & 0x80)) {
			// ＯＣＲ用紙
			if (m_pPrintCmInfo->OPC) {
				PF1[++i].PITM[0] = (char *)0;
			}
			else {
				if (m_pSnHeadData->Sn_Syuusei & 0x10) {
					PF1[++i].PITM[0] = _T("準確定");
				}
				else {
					PF1[++i].PITM[0] = _T("　確定　");
				}
			}
		}
		else {
			if (m_pSnHeadData->Sn_Syuusei & 0x10) {
				PF1[++i].PITM[0] = _T("準確定");
			}
			else {
				PF1[++i].PITM[0] = _T("　確定　");
			}
		}
		break;
	case 2:
		PF1[++i].PITM[0] = _T("　中間　");
		break;
	case 3:
		PF1[++i].PITM[0] = _T("修正確定");
		break;
	case 4:
		PF1[++i].PITM[0] = _T("修正中間");
		break;
	}

	// 中間申告期間
	if(( m_pPrintCmInfo->PRtype & 0x80 ) || (( !m_pPrintCmInfo->OP1 ) || ( IsNotPrintGengoCircle() == false ))) {
		int gengo = 0;
		switch (m_pSnHeadData->Sn_SKKBN) {
		case 1:
		case 3:
			pbclr(0, 6, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			break;
		default:
			if ( !( m_pPrintCmInfo->PRtype & 0x80 ) && m_pPrintCmInfo->OP1 ) {
				pbclr( 0, 6, &PF1[i] );

				memset( yymmdd, '\0', sizeof( yymmdd ));
				m_pSnHeadData->GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
			}
			else {
				// 中間申告期間 自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				for (++i, j = 0; j < 3; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
					PF1[i].PITM[j] = dp;
				}
				// 中間申告期間 至
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				for (; j < 6; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
					PF1[i].PITM[j] = dp;
				}
			}

			// 元号の〇（法人のみ）
			if( !m_pSnHeadData->IsSoleProprietor() ){
				if (IsNotPrintGengoCircle()) {
					pbclr(0, 2, &PF1[++i]);
				}
				else {
					i++;
					if (gengo == ID_ICSSH_REIWA_GENGO) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char*)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if (gengo == ID_ICSSH_HEISEI_GENGO) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char*)0;
						dp += 5;
					}
					else {
						PF1[i].PITM[0] = (char*)0;
						PF1[i].PITM[1] = (char*)0;
					}
				}
			}
			else{
				// OCR印刷はスケジュールが法人個人で一本のため、元号の〇を出力する用の記述がある
				// その分を初期化しておく
				if( !(m_pPrintCmInfo->PRtype & 0x80) ){
					pbclr(0, 2, &PF1[++i]);				
				}
			}
			break;
		}
	}
	else{
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 6, &PF1[++i]);
		//pbclr(0, 2, &PF1[++i]);
	}

	// 整理番号等出力サイン
	/*if ((m_pPrintCmInfo->PRtype & 0x80) || (!m_pPrintCmInfo->OP1)) {

		int gengo = 0;
		//中間申告期間自･至
		switch (m_pSnHeadData->Sn_SKKBN) {
		case 1:
		case 3:
			pbclr(0, 6, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
			break;
		default:

			if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
				pbclr(0, 6, &PF1[++i]);
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
			}
			else {
				if (CCSw) {	// 強制期間変更
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}
					for (; j < 6; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}

				}
				else {
					gengo = 0;
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
						PF1[i].PITM[j] = dp;
					}
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					for (; j < 6; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
						PF1[i].PITM[j] = dp;
					}

				}
			}

			break;
		}
	}
	else {
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 6, &PF1[++i]);
		pbclr(0, 2, &PF1[++i]);
	}*/

	return _xprn(PP, 1, PF1, 0, pDC, isNoImage);
}

//============================================================================
// 本表印刷（インボイス２割特例用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX2_2(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド２を印字

	int				i, j;
	char			*dp;
	// 税理士法人
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = { 0 };

	char	PW0[6], PW1[6];
	memset(PW0, '\0', 6);
	m_Arith.l_input(PW0, _T("1000"));
	memset(PW1, '\0', 6);
	m_Arith.l_input(PW1, _T("100"));

	i = j = 0;

	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) { //原則
		GetSyzSyukei_Ippan();
	}
	else if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) { //簡易
		GetSyzSyukei_Kani();
	}

	//①　課税標準額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM1, PW0);
	m_Arith.l_print(dp, WORK0, FMT011EX);

	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//②　消費税額
	m_Arith.l_print(dp, XM2, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//③　控除過大調整税額
	m_Arith.l_print(dp, XM3, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//④　控除対象仕入税額
	m_Arith.l_print(dp, XM4, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑤　返還等対価に係る税額
	m_Arith.l_print(dp, XM5, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑥　貸倒れに係る税額
	m_Arith.l_print(dp, XM6, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑦　控除税額小計
	m_Arith.l_print(dp, XM7, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑧　控除不足還付税額
	m_Arith.l_print(dp, XM8, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑨　差引税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM9, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑩　中間納付税額
	if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OPB) {
		PF1[i].PITM[j++] = (char*)0;
	}
	else {
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, XM10, PW1);
		m_Arith.l_print(dp, WORK0, FMT12);

		PF1[i].PITM[j++] = dp;
		dp += 17;
	}

	//⑪　納付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM11, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);

	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑫　中間納付還付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM12, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);

	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑬　既確定税額
	m_Arith.l_print(dp, XM13, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑭　差引納付税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM14, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑮　課税資産の譲渡等の対価の額
	m_Arith.l_print(dp, XM15, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//⑯　資産の譲渡等の対価の額
	m_Arith.l_print(dp, XM16, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑰　控除不足還付税額
	m_Arith.l_print(dp, XM17, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑱　差引税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM18, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//⑲　還付額
	m_Arith.l_print(dp, XM19, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//20　納税額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM20, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//21　中間納付譲渡割額
	if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OPB) {
		PF1[i].PITM[j++] = (char*)0;
	}
	else {
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, XM21, PW1);
		m_Arith.l_print(dp, WORK0, FMT12);
		PF1[i].PITM[j++] = dp;
		dp += 17;
	}

	//22　納付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM22, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	//23　中間納付還付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM23, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//24　既確定譲渡割額
	m_Arith.l_print(dp, XM24, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	//25　差引納付譲渡割額
	memset(WORK0, '\0', sizeof(WORK0));
	m_Arith.l_div(WORK0, XM25, PW1);
	m_Arith.l_print(dp, WORK0, FMT12);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	//26　消費税及び地方消費税の合計(納付又は還付)税額
	m_Arith.l_print(dp, XM26, FMT130EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	return _xprn(PP, 2, PF1, 0, pDC, isNoImage);

}

//============================================================================
// 本表印刷（インボイス２割特例用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX2_3(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド３を印字
	int				i, j, k;
	char			*dp, buf[512];
	BOOL			isNoImage = FALSE;

	// 課税期間の取得
	long symd = 0, eymd = 0;
	GetKazeiKikan(symd, eymd);

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

//	PBUF	PF1[30];
	PBUF	PF1[30] = { 0 };

	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = { 0 };

	char	PW0[6];
	memset(PW0, '\0', 6);
	m_Arith.l_input(PW0, _T("1000"));

	// 税理士法　施行前帳票	スケジュール　スキップ
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// 付記事項
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "○" : "　");	// 割賦基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "　" : "○");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE ? "○" : "　");	// 延払基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE ? "　" : "○");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI ? "○" : "　");  // 工事進行基準の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI ? "　" : "○");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "○" : "　");	// 現金主義会計の適用
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "　" : "○");
	//----->課税期間が令和5年10月1日以後開始の場合、「課税標準額に対する～…特例の適用」の〇印字を無しに('23.07.11 cor)
	if ( symd >= ICS_SH_INVOICE_DAY ) {							// 課税標準額に対する～
		PF1[i].PITM[j++] = "　";
		PF1[i++].PITM[j] = "　";
	}
	else {
		PF1[i].PITM[j++] = ((m_pSnHeadData->Sn_TUMIAG & 0x01) ? "○" : "　");
		PF1[i++].PITM[j] = ((m_pSnHeadData->Sn_TUMIAG & 0x01) ? "　" : "○");
	}
	//<-----

	j = 0;
	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
		if( !( m_pSnHeadData->m_DispTabSgn & 0x2000 ) ){
			if (m_pSnHeadData->IsUseSemiUriRatio()) {
				PF1[i].PITM[j++] = "○";
				PF1[i].PITM[j++] = "　";
				PF1[i].PITM[j++] = (char *)0;
			}
			else {
				if (!(m_pSnHeadData->Sn_Sign4 & 0x10) &&
					((m_pSnHeadData->IsRatioOver95() == TRUE) || (m_pSnHeadData->Sn_EXP100 & 0x01))) {	// 全額控除
					PF1[i].PITM[j++] = (char *)0;
					PF1[i].PITM[j++] = (char *)0;
					PF1[i].PITM[j++] = "○";
				}
				else {
					if (m_pSnHeadData->IsKobetuSiireAnbun()) {
						PF1[i].PITM[j++] = ("○");
						PF1[i].PITM[j++] = ("　");
					}
					else {
						PF1[i].PITM[j++] = ("　");
						PF1[i].PITM[j++] = ("○");
					}
					PF1[i].PITM[j++] = (char *)0;
				}
			}
		}

		i++;
		j = 0;

		//基準期間の課税売上高
		memset(WORK0, '\0', sizeof(WORK0));
		m_Arith.l_div(WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0);
		m_Arith.l_print(dp, WORK0, FMT14);
		PF1[i].PITM[j++] = dp;
		dp += 19;
	}
	else {

		// 2割特例の場合は表示不要
		if( !(m_pSnHeadData->m_DispTabSgn & 0x2000) ){
			char PW[MONY_BUF_SIZE] = { 0 };
			char WORK[MONY_BUF_SIZE] = { 0 };
			memset(PW, '\0', MONY_BUF_SIZE);
			m_Arith.l_input(PW, _T("1000"));

			// 事業別課税売上高
			//第１種
			memset(WORK, '\0', MONY_BUF_SIZE);
			if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) >= 0) {
				memmove(WORK, m_pSnHonpyoData->Sn_KURI1, MONY_BUF_SIZE);
				m_Util.l_calq(WORK, WORK, 2);
				m_Arith.l_div(WORK, WORK, PW);
				if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7E)) {
					m_Arith.l_print(dp, WORK, FMT11Z);
				}
				else {
					m_Arith.l_print(dp, WORK, FMT11);
				}
			}
			PF1[i].PITM[j++] = dp;
			dp += 12;

			//第２種
			memset(WORK, '\0', MONY_BUF_SIZE);
			if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) >= 0) {
				memmove(WORK, m_pSnHonpyoData->Sn_KURI2, MONY_BUF_SIZE);
				m_Util.l_calq(WORK, WORK, 2);
				m_Arith.l_div(WORK, WORK, PW);
				if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8E)) {
					m_Arith.l_print(dp, WORK, FMT11Z);
				}
				else {
					m_Arith.l_print(dp, WORK, FMT11);
				}
			}
			PF1[i].PITM[j++] = dp;
			dp += 12;

			//第３種
			memset(WORK, '\0', MONY_BUF_SIZE);
			if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) >= 0) {
				memmove(WORK, m_pSnHonpyoData->Sn_KURI3, MONY_BUF_SIZE);
				m_Util.l_calq(WORK, WORK, 2);
				m_Arith.l_div(WORK, WORK, PW);
				if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9E)) {
					m_Arith.l_print(dp, WORK, FMT11Z);
				}
				else {
					m_Arith.l_print(dp, WORK, FMT11);
				}
			}
			PF1[i].PITM[j++] = dp;
			dp += 12;

			//第４種
			memset(WORK, '\0', MONY_BUF_SIZE);
			if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) >= 0) {
				memmove(WORK, m_pSnHonpyoData->Sn_KURI4, MONY_BUF_SIZE);
				m_Util.l_calq(WORK, WORK, 2);
				m_Arith.l_div(WORK, WORK, PW);
				if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10E)) {
					m_Arith.l_print(dp, WORK, FMT11Z);
				}
				else {
					m_Arith.l_print(dp, WORK, FMT11);
				}
			}
			PF1[i].PITM[j++] = dp;
			dp += 12;

			//第５種
			memset(WORK, '\0', MONY_BUF_SIZE);
			if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) >= 0) {
				memmove(WORK, m_pSnHonpyoData->Sn_KURI5, MONY_BUF_SIZE);
				m_Util.l_calq(WORK, WORK, 2);
				m_Arith.l_div(WORK, WORK, PW);
				if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11E)) {
					m_Arith.l_print(dp, WORK, FMT11Z);
				}
				else {
					m_Arith.l_print(dp, WORK, FMT11);
				}
			}
			PF1[i].PITM[j++] = dp;
			dp += 12;

			//第６種
			memset(WORK, '\0', MONY_BUF_SIZE);
			if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) >= 0) {
				memmove(WORK, m_pSnHonpyoData->Sn_KURI6, MONY_BUF_SIZE);
				m_Util.l_calq(WORK, WORK, 2);
				m_Arith.l_div(WORK, WORK, PW);
				if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12E)) {
					m_Arith.l_print(dp, WORK, FMT11Z);
				}
				else {
					m_Arith.l_print(dp, WORK, FMT11);
				}
			}
			PF1[i].PITM[j++] = dp;
			dp += 12;
		}

		i++;
		j = 0;

		// 事業別課税売上割合
		j = 0;
		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP1 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP1 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP2 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP2 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP3 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP3 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP4 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP4 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP5 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", m_pSnHonpyoData->Sn_UP5 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%3d", m_pSnHonpyoData->Sn_UP6 / 10);
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if ((m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6) > 0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA) > 0)) {
			wsprintf(dp, "%01d", (m_pSnHonpyoData)->Sn_UP6 % 10);
		}
		strcat_s(dp, 4 + 1, "  ");
		PF1[i++].PITM[j] = dp;
		dp += 4;


		if( IsNiwariTokurei() ) {
			// ２割特例適用時には特例計算適用は不要
			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = ( char* )0;
		}
		else {
			PF1[i].PITM[0] = ((m_pSnHeadData->Sn_TOKUR) ? "○" : "　");			// 特例計算適用
			PF1[i].PITM[1] = ((m_pSnHeadData->Sn_TOKUR) ? "　" : "○");
		}
	}

	//(メモ)スケジュールファイル調整するまで一旦コメントアウト
	PF1[++i].PITM[0] = ((m_pSnHeadData->m_DispTabSgn & 0x2000) ? _T("○") : _T("　"));	// 2割特例

	pbclr(0, 1, &PF1[++i]);
	pbclr(0, 2, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);
	pbclr(0, 1, &PF1[i + 4]);
	pbclr(0, 1, &PF1[i + 5]);

	//yoshida190606
	if ((m_Arith.l_test(XM26) < 0) || m_pPrintCmInfo->OPI) {	// 還付金融機関表示されてなかった修正

		if( m_pSnHeadData->IsSoleProprietor() ){
		// （個人の方）公金受取口座の利用
			PF1[i++].PITM[0] = ((m_pSnHeadData->Sn_GenericSgn & 0x400) ? _T("○") : _T("　"));
		}
		else {
			PF1[i++].PITM[0] = ( char* )0;
		}

		//yoshida221226
		if (!(m_pSnHeadData->Sn_GenericSgn & 0x400)) {//公金受取口座に○がない場合のみ金融機関出力
			if (!m_pSnHeadData->Sn_BYsw) {			// 銀行
				memmove(dp, m_pSnHeadData->Sn_BANK, 10 * 2);
				PF1[i].PITM[0] = dp;
				dp += 21;

				memmove(dp, m_pSnHeadData->Sn_SITEN, 10 * 2);
				PF1[i++].PITM[1] = dp;
				dp += 21;

				memmove(dp, m_pSnHeadData->Sn_YOKIN, 4 * 2);
				PF1[i++].PITM[0] = dp;
				dp += 9;

				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_KOZA, 10);
				for (j = (int)strlen(buf); j > 0; ) {
					if (buf[--j] != 0x20) {
						break;
					}
				}
				++j;

				if (j < 10) {
					memmove(dp, "          ", 10 - j);
				}
				// 口座番号
				memmove((dp + (10 - j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 11;
			}
			else {
				i += 3;
				memmove(dp, m_pSnHeadData->Sn_YUBIN, 8 * 2);
				PF1[i++].PITM[0] = dp;
				dp += 17;

				memset(buf, '\0', sizeof(buf));
				memmove(buf, m_pSnHeadData->Sn_YUNUM1, 5);
				j = (int)strlen(buf);
				memset(buf, '\0', sizeof(buf));
				memmove(buf, m_pSnHeadData->Sn_YUNUM2, 8);
				k = (int)strlen(buf);
				if (j || k) {
					strcpy_s(buf, 8 + 1, "        ");											//?
					if (j) {
						memmove(&buf[0], m_pSnHeadData->Sn_YUNUM1, j);
					}
					if (k) {
						memmove(&buf[((m_pPrintCmInfo->PRtype & 0x80) ? 7 : 8)], m_pSnHeadData->Sn_YUNUM2, k);
					}
					dp = &buf[0];
				}
				PF1[i++].PITM[0] = dp;
			}
		}
	}
	return _xprn(PP, 3, PF1, 0, pDC, isNoImage);
}

//============================================================================
// 本表印刷（インボイス２割特例用）
//============================================================================
int CH31HyoView::PrintHonPyoSubEX2_4(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド４～１４を印字

	int				i, j, k, h;
	char			*dp, buf[512];
	// 税理士法人
	int				Zlng, Zsw;
	char			Znam[128] = { 0 }, Smei[128] = { 0 };
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	int		PP = PPno;

	int st = 0;

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// 整理番号等出力サイン
	pbclr(0, 1, &PF1[i]);
	if ((m_pPrintCmInfo->PRtype & 0x80) || !m_pPrintCmInfo->OP1) {
		if ((m_pSnHeadData->Sn_ZSEIRI[0] & 0xff) != 0xff) {
			char	tmpbuf[512] = { 0 };
			memset(tmpbuf, '\0', sizeof(tmpbuf));
			if ((m_pSnHeadData->Sn_ZSEIRI[3] & 0x0f) != 0x0f) {
				j = 8;
			}
			else {
				j = 7;
			}
			l_unpac(tmpbuf, m_pSnHeadData->Sn_ZSEIRI, j);
			if (j == 8) {				// 8桁入力の場合:先頭0はカット
				if (tmpbuf[0] != 0x30) {
					tmpbuf[7] = '\0';		// 以外は後1桁カット
				}
				else {
					for (j = 0; j < 7; ++j) {
						tmpbuf[j] = tmpbuf[j + 1];
					}
					tmpbuf[j] = '\0';
				}
				j = 7;
			}
			for (j = (int)strlen(tmpbuf); j > 0; ) {
				if (tmpbuf[--j] != 0x20) {
					break;
				}
			}
			++j;

			// 税務署整理番号
			memset(buf, '\0', sizeof(buf));
			memmove(buf, "        ", 8);
			memmove(&buf[7 - j], tmpbuf, j);
			memmove(dp, &buf[0], 8);
			PF1[i].PITM[0] = dp;
			dp += 9;
		}
	}


	int	 rt = 0;
	int	 Zimusyolng = 0;
	char Zimusyonam[128] = { 0 };

	// 税理士名出力
	for (++i, j = 0, k = 0; j < 2; ++j) {
		pbclr(0, 1, &PF1[i + j]);
	}
	Zlng = Zsw = 0;
	memset(Znam, '\0', sizeof(Znam));
	memset(Smei, '\0', sizeof(Smei));

	int	SmlFontZrtype = 0;	// 0:通常, 1:全角20文字より大

	if (!m_pPrintCmInfo->OP2) {
		if ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01)) {
			// 税理士氏名を手書きする為に、事務署名＆税理士法人名を縮小で印刷
			// フィールド No.10で印刷
			if ((j = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40) {
				j = 40;
			}
			memmove(Znam, m_pShinInfo->pO_DATA->o_ofcnam, j);
			Zlng = m_Util.MixSpaceCutLength(Znam, j);
			memmove(dp, "　", 2);
			PF1[i].PITM[0] = dp;
			dp += (2 + 1);
		}
		else {
			if (m_pPrintCmInfo->OPA != 2) {
				// 税理士氏名又は税理士法人名
				rt = MakeOutZeirishiName(Znam, sizeof(Znam), 1);
				j = (int)strlen(Znam);
				Zlng = m_Util.MixSpaceCutLength(Znam, j);

				if (Zlng > 40) {
					memmove(dp, "　", 2);
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					SmlFontZrtype = 1;
				}
				else if (Zlng > 26) {
					memmove(dp, Znam, Zlng);
					PF1[i].PITM[0] = dp;
					dp += (j + 1);
				}
				else {					// １３文字以下は全角でフィールド No.8で印刷
					memmove(dp, "　", 2);
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					Zsw = 1;
				}
			}
		}
	}

	// 税理士電話番号
	if (!m_pPrintCmInfo->OP9) {
		for (j = 0; j < 19; ++j) {
			buf[j] = 0x20;
		}
		buf[j] = '\0';
		for (++i, j = k = 0; (j < 30) || m_pShinInfo->pO_DATA->o_telno[j]; ++j) {
			if ((m_pShinInfo->pO_DATA->o_telno[j] == '-') || (m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ') ||
				(m_pShinInfo->pO_DATA->o_telno[j] == '(') || (m_pShinInfo->pO_DATA->o_telno[j] == ')')) {
				switch (k) {
				case 0:
					k = ((j < 6) ? j : 6);
					memmove(&buf[6 - k], &m_pShinInfo->pO_DATA->o_telno[0], k);
					k = 1;	h = j + 1;
					continue;
				case 1:
					if ((j - h) < 5) {
						k = (j - h);
					}
					else {
						k = 5;
					}
					memmove(&buf[8], &m_pShinInfo->pO_DATA->o_telno[h], k);
					memmove(&buf[14], &m_pShinInfo->pO_DATA->o_telno[++j], 5);
					k = 2;
					break;
				default:
					break;
				}
				if (k > 1) {
					break;
				}
			}
		}
		if (!k) {
			memmove(buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 19 ? 19 : j));
		}
		else {
			if (k == 1) {
				memmove(&buf[8], &m_pShinInfo->pO_DATA->o_telno[h], 4);
			}
		}
		memmove(dp, buf, 19);

		PF1[i].PITM[0] = dp;
		dp += 20;
	}

	if (st = _xprn(PP, 4, PF1, 0, pDC, isNoImage)) return st;

	// 還付を受けようとする金融期間	
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	pbclr(0, 1, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);
	pbclr(0, 1, &PF1[i + 4]);
	pbclr(0, 1, &PF1[i + 5]);
	pbclr(0, 1, &PF1[i + 6]);
	pbclr(0, 1, &PF1[i + 7]);
	pbclr(0, 1, &PF1[i + 8]);
	pbclr(0, 1, &PF1[i + 9]);

	//yoshida190606
	if ((m_Arith.l_test(XM26) < 0) || m_pPrintCmInfo->OPI) {	// 還付金融機関表示されてなかった修正

		//yoshida221226
		if (!(m_pSnHeadData->Sn_GenericSgn & 0x400)) {//公金受取口座に○がない場合のみ金融機関出力

			if (!m_pSnHeadData->Sn_BYsw) {			// 銀行
				switch (m_pSnHeadData->Sn_FINANCE & 0x0f) {
				case 0:
					PF1[i].PITM[0] = " ";
					break;
				case 1:
					PF1[i + 1].PITM[0] = " ";
					break;
				case 2:
					PF1[i + 2].PITM[0] = " ";
					break;
				case 3:
					PF1[i + 3].PITM[0] = " ";
					break;
				case 4:
					PF1[i + 4].PITM[0] = " ";
					break;
				default:
					break;
				}

				switch (m_pSnHeadData->Sn_CHAIN & 0x0f) {
				case 0:
					PF1[i + 5].PITM[0] = " ";
					break;
				case 1:
					PF1[i + 6].PITM[0] = " ";
					break;
				case 2:
					PF1[i + 7].PITM[0] = " ";
					break;
				case 3:
					PF1[i + 8].PITM[0] = " ";
					break;
				case 5:
					PF1[i + 9].PITM[0] = " ";
					break;
				default:
					break;
				}
			}
		}
	}

	if (st = _xprn(PP, 5, PF1, 0, pDC, isNoImage)) return st;

	// 欄外印字項目出力
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	pbclr(0, 1, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 1, &PF1[i + 2]);
	pbclr(0, 1, &PF1[i + 3]);

	pbclr(0, 1, &PF1[i + 4]);		// 個人	下段①
	pbclr(0, 1, &PF1[i + 5]);		// 〃	個人
	pbclr(0, 1, &PF1[i + 6]);		// 〃	上段
	pbclr(0, 1, &PF1[i + 7]);		// 〃	下段
	pbclr(0, 1, &PF1[i + 8]);		// 〃	税務署処理欄　区分３
									// 拡張		
	pbclr(0, 1, &PF1[i + 9]);		// 法人	下段
	pbclr(0, 1, &PF1[i + 10]);		// 〃　	署別一連番号
									// 所管
	pbclr(0, 1, &PF1[i + 11]);

	// 欄外下拡張
	char	ADRDX[128], ADRD2[20], ADRD4[50];
	int		ln2, ln4;
	memset(ADRDX, '\0', sizeof(ADRDX));
	memset(ADRDX, 0x20, 60);
	memset(ADRD2, '\0', sizeof(ADRD2));
	memmove(ADRD2, m_pSnHeadData->Sn_ADRD2, 16);
	memset(ADRD4, '\0', sizeof(ADRD4));
	memmove(ADRD4, m_pSnHeadData->Sn_ADRD4, 44);;
	ln2 = (int)strlen(ADRD2);
	ln4 = (int)strlen(ADRD4);
	memmove(ADRDX, ADRD4, ln4);
	memmove(&ADRDX[44], ADRD2, ln2);

	if (!m_pPrintCmInfo->OP7 && (m_pSnHeadData->IsSoleProprietor() == FALSE)) {
		memmove(dp, m_pSnHeadData->Sn_ADRU1, 30);
		PF1[i].PITM[0] = dp;
		dp += 32;

		memmove(dp, m_pSnHeadData->Sn_ADRU2, 20);
		PF1[i + 1].PITM[0] = dp;
		dp += 22;

		memmove(dp, m_pSnHeadData->Sn_ADRC1, 20);
		PF1[i + 2].PITM[0] = dp;
		dp += 22;

		memmove(dp, m_pSnHeadData->Sn_ADRC2, 20);
		PF1[i + 3].PITM[0] = dp;
		dp += 22;

		if (m_pSnHeadData->IsSoleProprietor()) {
			memmove(dp, ADRDX, 60);
			PF1[i + 4].PITM[0] = dp;
			dp += 62;

			if (m_pSnHeadData->Sn_PPTYP & 0x01) {
				memmove(dp, "個人", 4);
				PF1[i + 5].PITM[0] = dp;
				dp += 6;
			}
			memmove(dp, m_pSnHeadData->Sn_ADKUP, 10);
			PF1[i + 6].PITM[0] = dp;
			dp += 12;

			memmove(dp, m_pSnHeadData->Sn_ADKDW, 10);
			PF1[i + 7].PITM[0] = dp;
			dp += 12;

			memmove(dp, m_pSnHeadData->Sn_ADKNO, 1);
			PF1[i + 8].PITM[0] = dp;
			dp += 2;
		}
		else {
			memmove(dp, ADRDX, 60);
			PF1[i + 9].PITM[0] = dp;
			dp += 62;
			memmove(dp, m_pSnHeadData->Sn_ADSNO, 7);
			PF1[i + 10].PITM[0] = dp;
			dp += 9;
		}
	}

	if ((m_pPrintCmInfo->PRtype & 0x80) || !m_pPrintCmInfo->OP1) {		// 整理番号等出力サイン
		memmove(dp, m_pSnHeadData->Sn_SYOKN, 3);
		PF1[i + 11].PITM[0] = dp;
		dp += 5;
	}

	if (st = _xprn(PP, 6, PF1, 0, pDC, isNoImage)) return st;

	// 税理士法　施行後帳票
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	pbclr(0, 1, &PF1[0]);
	pbclr(0, 1, &PF1[1]);

	if (m_pSnHeadData->Sn_ZeiHou & 0x01) {
		PF1[0].PITM[0] = "○";
	}
	else {
		PF1[0].PITM[0] = (char *)0;
	}

	if (m_pSnHeadData->Sn_ZeiHou & 0x02) {
		PF1[1].PITM[0] = "○";
	}
	else {
		PF1[1].PITM[0] = (char *)0;
	}

	if (st = _xprn(PP, 7, PF1, 0, pDC, isNoImage)) return st;

	pbclr(0, 1, &PF1[0]);
	pbclr(0, 1, &PF1[1]);

	// memo
	// OPA		→1：事務所名 / 2：事務所名と税理士名
	// OPSign	→0x01：事務所名を縮小
	// OP2		→事務所名と税理士名のどっちかにチェックがついてたら0で来る。2つともチェックなしなら1で来る。

	if (!m_pPrintCmInfo->OP2) {
		if ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01)) {	// 「事務所名」と「事務所名を縮小して出力」にチェック
																				// 事務署名＆税理士法人名を縮小で印刷
																				// 税理士法 施行後
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;

			// 税理士法 施行後
			pbclr(0, 1, &PF1[0]);
			memmove(dp, Znam, Zlng);
			PF1[0].PITM[0] = dp;
			dp += (Zlng + 1);
			if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;


			// 税理士法　施行後 追加！
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
			pbclr(0, 1, &PF1[0]);
			if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
		}
		else {
			if (m_pPrintCmInfo->OPA == 2) {	// 「事務所名」と「税理士名」にチェック

				if (m_pPrintCmInfo->OPsign & 0x01) {	// 「事務所名を縮小して出力」にチェック

														// 事務署名＆税理士法人名を縮小で印刷
														// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法 施行後
					if ((Zlng = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40) {
						Zlng = 40;
					}
					memmove(Znam, m_pShinInfo->pO_DATA->o_ofcnam, Zlng);
					pbclr(0, 1, &PF1[0]);
					memmove(dp, Znam, Zlng);
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法　施行後　税理士氏名
					rt = MakeOutZeirishiName(Smei, sizeof(Smei), 1);
					Zlng = (int)strlen(Smei);
					if (Zlng > 27) {	// 14文字以上なら縮小させる
						memset(Znam, '\0', sizeof(Znam));
						memmove(Znam, Smei, Zlng);
						pbclr(0, 1, &PF1[0]);

						if (Zlng <= 40) {
							SmlFontZrtype = 2;
						}
						else {
							SmlFontZrtype = 3;
						}
					}
					else {
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Smei, Zlng);
						PF1[0].PITM[0] = dp;
						dp += (Zlng + 1);
					}
					if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
				}
				else {	// 「事務所名」・「税理士名」を通常フォントで表示

						// 事務所名
					Zimusyolng = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam);
					if (Zimusyolng > 26) {	// 13文字以上なら縮小させる
						memmove(Zimusyonam, m_pShinInfo->pO_DATA->o_ofcnam, Zimusyolng);
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Zimusyonam, Zimusyolng);
						PF1[0].PITM[0] = dp;
						dp += (Zimusyolng + 1);
						if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;
					}
					else {
						memmove(Zimusyonam, m_pShinInfo->pO_DATA->o_ofcnam, Zimusyolng);
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Zimusyonam, Zimusyolng);
						PF1[0].PITM[0] = dp;
						dp += (Zimusyolng + 1);
						if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
					}


					// 税理士名
					rt = MakeOutZeirishiName(Smei, sizeof(Smei), 1);
					Zlng = (int)strlen(Smei);
					if (Zlng > 26) {	// 13文字以上なら縮小させる
						memset(Znam, '\0', sizeof(Znam));
						memmove(Znam, Smei, Zlng);
						pbclr(0, 1, &PF1[0]);

						if (Zlng <= 40) {
							SmlFontZrtype = 2;
						}
						else {
							SmlFontZrtype = 3;
						}
					}
					else {
						pbclr(0, 1, &PF1[0]);
						memmove(dp, Smei, Zlng);
						PF1[0].PITM[0] = dp;
						dp += (Zlng + 1);
					}
					if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
				}
			}
			else {	// 事務所名のみ
				if (Zsw) {
					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					memmove(dp, Znam, Zlng);
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if (st = _xprn(PP, 8, PF1, 0, pDC, isNoImage)) return st;


					// 税理士法 施行後
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 9, PF1, 0, pDC, isNoImage)) return st;
					// 税理士法　施行後 追加！
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 10, PF1, 0, pDC, isNoImage)) return st;
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 11, PF1, 0, pDC, isNoImage)) return st;
					pbclr(0, 1, &PF1[0]);
					if (st = _xprn(PP, 12, PF1, 0, pDC, isNoImage)) return st;
				}
			}
		}
	}

	if (m_pPrintCmInfo->OPM) {	// 被合併名称
		dp = AP;
		i = j = 0;
		memset(dp, '\0', size);
		pbclr(0, 1, &PF1[0]);
		pbclr(0, 1, &PF1[1]);

		// 法人名
		if (!m_pPrintCmInfo->OP4) {
			memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
			PF1[0].PITM[0] = dp;
			dp += 41;
		}
		memmove(dp, m_pSnHeadData->Sn_GPNAM, 20 * 2);
		PF1[1].PITM[0] = dp;
		dp += 41;
		if (st = _xprn(PP, 13, PF1, 0, pDC, isNoImage)) return st;
	}


	// 税理士名で最小フォント（事務所名にチェックがないとき）
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 1) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 14, PF1, 0, pDC, isNoImage)) return st;

	// 事務所名＋税理士名で最小フォント
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 2) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 15, PF1, 0, pDC, isNoImage)) return st;


	//準確定の場合選択的に文言を出力
	if (m_pSnHeadData->Sn_Syuusei & 0x10) {
		if (!m_pPrintCmInfo->OPV) {
			dp = AP;
			memset(dp, '\0', size);
			PF1[0].PITM[0] = _T("被相続人");
			if (st = _xprn(PP, 16, PF1, 0, pDC, isNoImage)) return st;	// 使うのは個人マスターの時のみ
		}
	}

	// 税理士名が13文字以上の時
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (SmlFontZrtype == 3) {
		memmove(dp, Znam, Zlng);
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else {
		pbclr(0, 1, &PF1[0]);
	}
	if (st = _xprn(PP, 19, PF1, 0, pDC, isNoImage)) return st;


	// 税理士区分+税理士名で20文字超えるときは直接受任の文言を税理士名の下に出す
	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;
	if (m_pSnHeadData->m_ZroutSgn & 0x02) {
		if (rt == 1) {
			CString str;
			str.Format("（直接受任）");
			int lng = str.GetLength();
			memmove(dp, str, lng);
			PF1[i].PITM[0] = dp;
			dp += (lng + 1);
		}
		else {
			pbclr(0, 1, &PF1[1]);
		}
		if ((SmlFontZrtype != 3) && (SmlFontZrtype != 1)) {
			if (st = _xprn(PP, 20, PF1, 0, pDC, isNoImage))	return st;
		}
		else {	// フォント小
			if (st = _xprn(PP, 22, PF1, 0, pDC, isNoImage))	return st;
		}
	}


	// 個人番号欄に斜線
	if (m_pSnHeadData->IsSoleProprietor()) {
		if (isHikae) {
			int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
			int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
			m_pVprn->DrawSline(0, LKnrW0, 3650 + xPlus, 5360 + yPlus, 9750 + xPlus, 4850 + yPlus);
		}
	}

	// 電子申告完了済 
	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn) && isHikae) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 1L) + (PV1 / 4) + yPlus;
		long XPosition = (PH * 4L) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, "ＭＳ　ゴシック");
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar)) {
			return 1;
		}

		YPosition = PV1 + (PV1 / 4) + yPlus;
		XPosition = (PH * 12L) + xPlus;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar)) {
			return 1;
		}

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);
	}

	// 控用
	if (isHikae && m_pPrintCmInfo->OPD) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 2) + (PV1 / 2) + yPlus;
		long XPosition = (PH * 56L) + (PH / 3) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb | SFp_yc);			// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (8L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("【控用】"), &SfPar)) {
			return 1;
		}

		m_pVprn->DeleteRegFont(Gothic108);
	}

	// 新型コロナウイルスによる申告・納付期限延長申請
	if (m_Util.IsBasicMst(m_pZmSub->zvol) && m_pSnHeadData->Sn_GenericSgn & 0x10) {
		CString str;
		int lng = 0;

		dp = AP;
		memset(dp, '\0', size);
		str.Format(_T("新型コロナウイルスによる申告・"));
		lng = str.GetLength();
		memmove(dp, str, lng);
		PF1[0].PITM[0] = dp;
		dp += (lng + 1);

		str.Format(_T("納付期限延長申請"));
		lng = str.GetLength();
		memmove(dp, str, lng);
		PF1[1].PITM[0] = dp;

		if (st = _xprn(PP, 23, PF1, 0, pDC, isNoImage)) {
			return st;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本表印刷
//-----------------------------------------------------------------------------
// 引数	pDC		：	デバイスコンテキスト
//		isHikae	：	控え印刷かどうか
//		PPno	：
//		fno		：	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CH31HyoView::PrintHonPyo2( CDC *pDC, BOOL isHikae, int PPno )
{
	char			*AP = NULL;

	unsigned int	size = 2048;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}

	int st = 0;
	if (!st) st = PrintHonPyo2Sub1(pDC, isHikae, PPno, AP);
	if (!st) st = PrintHonPyo2Sub2(pDC, isHikae, PPno, AP);

	if (AP) {
		delete AP;
		AP = NULL;
	}

	return 0;
}

int CH31HyoView::PrintHonPyo2Sub1(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	//フィールド１を印字

	int				i, j;
	char			*dp, buf[512];
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	unsigned int	size = 2048;
	dp = AP;
	memset(dp, '\0', size);

	PBUF	PF1[30];
	memset ( PF1, '\0', sizeof ( PF1 ) );

	int		PP = PPno;


	char	yymmdd[3] = { 0 };
	i = j = 0;

	// 納税地
	if (!m_pPrintCmInfo->OP3) {
		// 郵便番号
		if (j = (int)strlen((char *)m_pSnHeadData->Sn_ZIP)) {
			strcpy_s(dp, 2 + 1, "〒");
			memmove((dp + 2), m_pSnHeadData->Sn_ZIP, j);
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else {
			PF1[i++].PITM[0] = (char *)0;
		}

		// 納税地（２行）
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[0], 20 * 2);
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove(dp, &m_pSnHeadData->Sn_NOZEI[40], 20 * 2);
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 2, &PF1[i++]);
	}

	// 納税地電話番号
	if (!m_pPrintCmInfo->OPE) {
		memset(buf, '\0', sizeof(buf));
		// 電話番号
		memmove(buf, m_pSnHeadData->Sn_TELNO1, 6);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		j = (int)strlen(buf);
		memmove(dp, "      ", 6);
		memmove((dp + (6 - j)), buf, 6);
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO2, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));

		PF1[i].PITM[1] = dp;	dp += 5;

		memset(buf, '\0', sizeof(buf));
		memmove(buf, m_pSnHeadData->Sn_TELNO3, 4);
		str.Format(_T("%s"), buf);
		str.TrimRight();
		memset(buf, '\0', sizeof(buf));
		memmove(buf, str, str.GetLength());
		memmove(dp, buf, sizeof(buf));
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else {
		pbclr(0, 3, &PF1[i++]);
	}

	// 氏名又は屋号	
	if (!m_pPrintCmInfo->OPF) {
		// 法人名（振りがな）
		memmove(dp, m_pSnHeadData->Sn_COFRI, 80);
		PF1[i++].PITM[0] = dp;
		dp += 81;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}


	// 氏名又は屋号
	if (!m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM) {	// 名称又は屋号にチェック、被合併名称チェックなし
		memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;

		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 1, &PF1[i++]);
	}
	else {
		pbclr(0, 1, &PF1[i++]);
		if (m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM) {
			pbclr(0, 1, &PF1[i++]);
		}
	}

	int a = i;
	if (m_pPrintCmInfo->OPM) {	// 被合併名称にチェック
		pbclr(0, 1, &PF1[a]);
		pbclr(0, 1, &PF1[a + 1]);

		if (!m_pPrintCmInfo->OP4) {	// 名称又は屋号にチェック
			memmove(dp, m_pSnHeadData->Sn_CONAM, 20 * 2);
			PF1[a].PITM[0] = dp;
			dp += 41;
			i++;
		}
		else {
			i++;
		}
		memmove(dp, m_pSnHeadData->Sn_GPNAM, 20 * 2);
		PF1[a + 1].PITM[0] = dp;
		dp += 41;
		i++;
	}
	else {
		if (m_pPrintCmInfo->OP4) {
			pbclr(0, 1, &PF1[i++]);
		}
	}


	// 代表者（振りがな）
// 改良依頼 20-0352 修正 del -->
	//if ( !m_pPrintCmInfo->OPG ) {
// 改良依頼 20-0352 修正 del <--
// 改良依頼 20-0352 修正 add -->
	if ( !(m_pPrintCmInfo->OPW & 0x02) ) {
// 改良依頼 20-0352 修正 add <--
		memmove( dp, m_pSnHeadData->Sn_DIFRI, 68 );
		PF1[i++].PITM[0] = dp;
		dp += 69;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 代表者名
// 改良依頼 20-0352 修正 del -->
	//if (!m_pPrintCmInfo->OP5) {
// 改良依頼 20-0352 修正 del <--
// 改良依頼 20-0352 修正 add -->
	if ( !(m_pPrintCmInfo->OPW & 0x01) ) {
	// 改良依頼 20-0352 修正 add <--
		memmove(dp, m_pSnHeadData->Sn_DIHYO, 20 * 2);
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else {
		pbclr(0, 1, &PF1[i++]);
	}

	// 改正法附則による税額の特例計算
// 220412 add -->
	// 令和四年四月一日以後終了課税期間分から特例計算（5-3）のチェック欄は非表示
	long	eymd=0;
	BOOL	sw=FALSE;
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	{
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else	{
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if(eymd >= TOK53MODIFY)	{
		sw=TRUE;
	}
// 220412 add <--
	if ( m_pSnHeadData->m_DispTabSgn & 0x80 ) {
		PF1[i++].PITM[0] = ((m_pSnHeadData->m_DispTabSgn & 0x10) ? "〇" : "　");	// 附則３８①
		PF1[i++].PITM[0] = ((m_pSnHeadData->m_DispTabSgn & 0x20) ? "〇" : "　");	// 附則３８②
// 220412 del -->
		//PF1[i++].PITM[0] = ((m_pSnHeadData->m_DispTabSgn & 0x40) ? "〇" : "　");	// 附則３８③
// 220412 del <--
// 220412 add -->
		if(sw == FALSE)	{
			PF1[i++].PITM[0] = ((m_pSnHeadData->m_DispTabSgn & 0x40) ? "〇" : "　");	// 附則３８③
		}
// 220412 add <--
	}
	else {
		PF1[i++].PITM[0] = ( "　" );	// 附則３８①
		PF1[i++].PITM[0] = ( "　" );	// 附則３８②
// 220412 del -->
		//PF1[i++].PITM[0] = ( "　" );	// 附則３８③
// 220412 del <--
// 220412 add -->
		if(sw == FALSE)	{
			PF1[i++].PITM[0] = ( "　" );	// 附則３８③
		}
// 220412 add <--
	}


	// 整理番号等出力サイン
	int CCSw = 0;
	int gengo1 = 0, gengo2 = 0;

	if ((m_pPrintCmInfo->PRtype & 0x80) ||
		((!m_pPrintCmInfo->OP1) || (IsNotPrintGengoCircle() == false))) {

		if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
			pbclr(0, 1, &PF1[i++]);
			pbclr(0, 6, &PF1[i]);
			if (IsNotPrintGengoCircle() == false) {
				// 申告期間自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);

			}
		}
		else {
			if ((m_pSnHeadData->Sn_SEIRI[0] & 0xff) != 0xff) {
				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
				for (j = (int)strlen(buf); j > 0; ) {
					if (buf[--j] != 0x20) {
						break;
					}
				}
				++j;
				if (j < 8) {
					memmove(dp, "        ", 8 - j);
				}
				// 整理番号
				memmove((dp + (8 - j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 9;
			}
			else {
				PF1[i++].PITM[0] = (char *)0;
			}

			if (CCSw) {	// 強制期間変更
				for (j = 0; j < 3; ++j, dp += 3) {				// 申告期間自
					PF1[i].PITM[j] = dp;
				}
				for (; j < 6; ++j, dp += 3) {					// 申告期間至
					PF1[i].PITM[j] = dp;
				}
			}
			else {
				// 申告期間自
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
				for (j = 0; j < 3; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
					PF1[i].PITM[j] = dp;
				}
				// 申告期間至
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
				for (; j < 6; ++j, dp += 3) {
					wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
					PF1[i].PITM[j] = dp;
				}
			}
		}

// 220412 del -->
		//if (IsNotPrintGengoCircle()) {
// 220412 del <--
// 220412 add -->
		// 新様式(令和のみ)なら元号に○印は不要
		if(IsNotPrintGengoCircle() || sw != FALSE)	{
// 220412 add <--
			pbclr(0, 2, &PF1[++i]);

		}
		else {
			if (CCSw) {	// 強制期間変更
				i++;
				PF1[i].PITM[0] = (char *)0;
				PF1[i].PITM[1] = (char *)0;
			}
			else {
				i++;
				if (IsNotPrintGengoCircle()) {
					gengo1 = -1;
				}
				if (gengo1 == ID_ICSSH_REIWA_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = (char*)0;
					PF1[i].PITM[1] = dp;
					dp += 5;
				}
				else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = dp;
					PF1[i].PITM[1] = (char*)0;
					dp += 5;
				}
				else {
					PF1[i].PITM[0] = (char*)0;
					PF1[i].PITM[1] = (char*)0;
				}
			}
		}

	}
	else {
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 6, &PF1[i]);
		pbclr(0, 2, &PF1[++i]);
	}

	switch (m_pSnHeadData->Sn_SKKBN) {
	case 1:
		if (!(m_pPrintCmInfo->PRtype & 0x80)) {
			// ＯＣＲ用紙
			if (m_pPrintCmInfo->OPC) {
				PF1[++i].PITM[0] = (char *)0;
			}
			else {
				if (m_pSnHeadData->Sn_Syuusei & 0x10) {
					PF1[++i].PITM[0] = _T("準確定");
				}
				else {
					PF1[++i].PITM[0] = _T("　確定　");
				}
			}
		}
		else {
			if (m_pSnHeadData->Sn_Syuusei & 0x10) {
				PF1[++i].PITM[0] = _T("準確定");
			}
			else {
				PF1[++i].PITM[0] = _T("　確定　");
			}
		}
		break;
	case 2:
		PF1[++i].PITM[0] = _T("　中間　");
		break;
	case 3:
		PF1[++i].PITM[0] = _T("修正確定");
		break;
	case 4:
		PF1[++i].PITM[0] = _T("修正中間");
		break;
	}

	// 整理番号等出力サイン
	if ((m_pPrintCmInfo->PRtype & 0x80) ||
		(!m_pPrintCmInfo->OP1) || (IsNotPrintGengoCircle() == false)) {

		//中間申告期間自･至
		switch (m_pSnHeadData->Sn_SKKBN) {
		case 1:
		case 3:
			pbclr(0, 6, &PF1[++i]);
			i++;
			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = ( char* )0;
			i++;
			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = ( char* )0;
			break;
		default:
			if (!(m_pPrintCmInfo->PRtype & 0x80) && m_pPrintCmInfo->OP1) {
				pbclr(0, 6, &PF1[++i]);
				if (IsNotPrintGengoCircle() == false) {
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
				}
			}
			else {
				if (CCSw) {	// 強制期間変更
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}
					for (; j < 6; ++j, dp += 3) {
						PF1[i].PITM[j] = dp;
					}
				}
				else {
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
					for (++i, j = 0; j < 3; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j] & 0xff);
						PF1[i].PITM[j] = dp;
					}
					memset(yymmdd, '\0', sizeof(yymmdd));
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
					for (; j < 6; ++j, dp += 3) {
						wsprintf(dp, _T("%02X"), yymmdd[j - 3] & 0xff);
						PF1[i].PITM[j] = dp;
					}
				}
			}
// 220412 del -->
			//if (IsNotPrintGengoCircle()) {
// 220412 del <--
// 220412 add -->
			// 新様式(令和のみ)なら元号に○印は不要
			if(IsNotPrintGengoCircle() || sw != FALSE)	{
// 220412 add <--
				pbclr(0, 2, &PF1[++i]);

			}
			else{
				if (CCSw) {	// 強制期間変更
					i++;
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = (char *)0;
				}
				else {
					if (IsNotPrintGengoCircle()) {
						gengo1 = -1;
					}

					i++;
					if (gengo1 == ID_ICSSH_REIWA_GENGO) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char*)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char*)0;
						dp += 5;
					}
					else {
						PF1[i].PITM[0] = (char*)0;
						PF1[i].PITM[1] = (char*)0;
					}
				}

			}

			break;
		}
	}
	else {
		pbclr(0, 6, &PF1[++i]);
	}


	
	return _xprn(PP, 1, PF1, 0, pDC, isNoImage);
}

int CH31HyoView::PrintHonPyo2Sub2(CDC *pDC, BOOL isHikae, int PPno, char *AP)
{
	// フィールド２を印字

	int			i, j;
	char		*dp;

	int st = 0;

	// 税理士法人
	CString		str = _T("");
	BOOL		isNoimage = FALSE;

	if( m_pPrintCmInfo->PRtype & 0x80 ){
		isNoimage = FALSE;
	}
	else {
		isNoimage = TRUE;
	}

	unsigned int		size = 2048;
	dp = AP;
	memset( dp, '\0', size );

	PBUF	PF1[30];
	int		PP = PPno;

	char	WORK0[MONY_BUF_SIZE] = {0};

	char	PW0[6], PW1[6];
	memset( PW0, '\0', MONY_BUF_SIZE );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', MONY_BUF_SIZE );
	m_Arith.l_input( PW1, _T("100") );

	i = j = 0;

	GetSyzSyukei_Ippan2();	// 原則も簡易も同じ


	// 課税標準額
	memset( WORK0, '\0', sizeof(WORK0) );
	m_Arith.l_div( WORK0, XM2_1, PW0 );
	m_Arith.l_print( dp, WORK0, FMT13EX);
	PF1[i].PITM[0] = dp;
	dp += 17;

	i++;
	j = 0;

	// 課税資産の譲渡等の対価の額の合計額　3％
	m_Arith.l_print( dp, XM2_2, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 課税資産の譲渡等の対価の額の合計額　4％
	m_Arith.l_print(dp, XM2_3, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 課税資産の譲渡等の対価の額の合計額　6.3％
	m_Arith.l_print(dp, XM2_4, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 課税資産の譲渡等の対価の額の合計額　6.24％
	m_Arith.l_print(dp, XM2_5, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 課税資産の譲渡等の対価の額の合計額　7.8％
	m_Arith.l_print(dp, XM2_6, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 課税資産の譲渡等の対価の額の合計額　計
	m_Arith.l_print(dp, XM2_7, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;
	
	// 特定課税仕入れに係る支払対価の額の合計額　6.3％
	m_Arith.l_print(dp, XM2_8, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 特定課税仕入れに係る支払対価の額の合計額　7.8％
	m_Arith.l_print(dp, XM2_9, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 特定課税仕入れに係る支払対価の額の合計額　計
	m_Arith.l_print(dp, XM2_10, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	// 消費税額
	m_Arith.l_print(dp, XM2_11, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑪の内訳　3％
	m_Arith.l_print(dp, XM2_12, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑪の内訳　4％
	m_Arith.l_print(dp, XM2_13, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑪の内訳　6.3％
	m_Arith.l_print(dp, XM2_14, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑪の内訳　6.24％
	m_Arith.l_print(dp, XM2_15, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑪の内訳　7.8％
	m_Arith.l_print(dp, XM2_16, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	// 返還等対価に係る税額
	m_Arith.l_print(dp, XM2_17, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑰の内訳　売上げの返還等対価に係る税額
	m_Arith.l_print(dp, XM2_18, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// ⑰の内訳　特定課税仕入れの返還等対価に係る税額
	m_Arith.l_print(dp, XM2_19, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	i++;
	j = 0;

	// 地方消費税の課税標準となる消費税額  計
	m_Arith.l_print(dp, XM2_20, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 地方消費税の課税標準となる消費税額　4％
	m_Arith.l_print(dp, XM2_21, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 地方消費税の課税標準となる消費税額　6.3％
	m_Arith.l_print(dp, XM2_22, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;

	// 地方消費税の課税標準となる消費税額　6.24％及び7.8％
	m_Arith.l_print(dp, XM2_23, FMT13EX);
	PF1[i].PITM[j++] = dp;
	dp += 17;


	//return _xprn( PP, 2, PF1, 0, pDC, isNoimage );
	if ( st = _xprn( PP, 2, PF1, 0, pDC, isNoimage ) ) {
		return st;
	}

	// 控用
	if ( isHikae && m_pPrintCmInfo->OPD ) {
		int yPlus = ( int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[1].HyoTn]._PIVad;
		int xPlus = ( int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[1].HyoTn]._PIHad;
		long	PH  = 2540L / 10L ;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = ( PV1 * 2 ) + ( PV1 / 2 ) + yPlus;
		long XPosition = ( PH * 56L ) + ( PH / 3 ) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont ( MAKELONG ( 108, 1 ), 0, 0, "ＭＳ　ゴシック" );
		struct	_SF_PAR		SfPar;
		memset ( ( char* )&SfPar, '\0', sizeof ( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb | SFp_yc );			// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = ( 8L * PH );							// 横出力領域幅
		SfPar.sf_yfld =	PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz =	Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if( m_pVprn->DrawString ( 0, XPosition, YPosition, _T( "【控用】" ), &SfPar ) ) {
			return -1;
		}

		m_pVprn->DeleteRegFont ( Gothic108 );
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 付表１－１出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo1_01( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp, PW0[MONY_BUF_SIZE];

	short emfNo = 0;
	long smd = 0, emd = 0;
	GetKazeiKikan( smd, emd );
	if( emd >= ICS_SH_INVOICE_DAY ){
		emfNo = MAKEWORD ( 1, 1 );
	}
	else{
		if ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) {
			if ( !( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) {
				emfNo = MAKEWORD ( 1, 1 );
			}	
			else {
				emfNo = MAKEWORD( 2, 2 );
			}
		}
		else {
			emfNo = MAKEWORD( 1, 1 );
		}
	}
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	size = 1024;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	
	char	WORK0[MONY_BUF_SIZE] = {0};

	memset( PW0, '\0', MONY_BUF_SIZE );
	m_Arith.l_input( PW0, _T("1000") );

	memset( WORK0, '\0', MONY_BUF_SIZE );

PBUF	PF1[30];
int		PP;
	//PP = 31;
	PP = 20;

	dp = AP;
	memset( dp, '\0', size );
	i = j = 0;

int CCSw = 0;
	// 強制課税期間印刷
	if( CCSw ){		// 強制期間変更
	}
	else{
		char	yymmdd[3] = {0};
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;

		memset( yymmdd, '\0', sizeof(yymmdd) );
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}
	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

//旧税率分小計Ｘ
	j = 0;
	//Ｘ-①
	m_Arith.l_div( WORK0, m_pSnFhyo1_01Data->Sn_F101_1X, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	
	i++;
	j = 0;
	//Ｘ-①-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-①-2
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_2X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-②
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_2X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-③
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_3X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-④
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_4X,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑤
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑤-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑤-2
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_2X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑥
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_6X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑦控除税額小計
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_7X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑧控除不足還付税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_8X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑨差引税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_9X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑩
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｘ-⑪
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_11X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑫
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_12X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j=0;

	//Ｘ-⑬
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_13X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑭
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_14X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑮
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_15X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑯
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ){
	//	goto POUT1;
	//}
	// 課税期間によって出力帳票を分ける
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, emfNo ) ){
		goto POUT1;
	}

//税率6.24%適用分Ｄ
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	memset( WORK0, '\0', MONY_BUF_SIZE );
	//Ｄ-①
	m_Arith.l_div( WORK0, m_pSnFhyo1_01Data->Sn_F101_1D, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	
	i++;
	j = 0;
	//Ｄ-①-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｄ-①-2
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
	//Ｄ-②
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_2D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｄ-③
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_3D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｄ-④
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_4D,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｄ-⑤
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｄ-⑤-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｄ-⑤-2
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
	//Ｄ-⑥
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_6D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ-⑦控除税額小計
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_7D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ-⑧控除不足還付税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_8D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ-⑨差引税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_9D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ-⑩
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｄ-⑪
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｄ-⑫
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	i++;
	j=0;

	//Ｄ-⑬
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｄ-⑭
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｄ-⑮
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｄ-⑯
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ){
	//	goto POUT1;
	//}
	if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, emfNo ) ) ) {
		goto POUT1;
	}


//税率7.8%適用分Ｅ
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	memset( WORK0, '\0', MONY_BUF_SIZE );
	//Ｅ-①
	m_Arith.l_div( WORK0, m_pSnFhyo1_01Data->Sn_F101_1E, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	
	i++;
	j = 0;
	//Ｅ-①-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-①-2
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_2E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-②
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_2E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-③
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_3E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-④
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_4E,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-⑤
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-⑤-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-⑤-2
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_2E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｅ-⑥
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_6E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑦控除税額小計
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_7E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ-⑧控除不足還付税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_8E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑨差引税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_9E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑩
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//Ｅ-⑪
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_11E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑫
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_12E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j=0;

	//Ｅ-⑬
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_13E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑭
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_14E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑮
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_15E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ-⑯
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;

	//if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ){
	//	goto POUT1;
	//}
	if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, emfNo ) ) ) {
		goto POUT1;
	}

//合計（X＋D＋E）①
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	memset( WORK0, '\0', MONY_BUF_SIZE );
	//Ｆ-①
	m_Arith.l_div( WORK0, m_pSnFhyo1_01Data->Sn_F101_1F, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	
	i++;
	j = 0;
	//Ｆ-①-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-①-2
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_1_2F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-②
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_2F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-③
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_3F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-④
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_4F,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-⑤
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-⑤-1
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-⑤-2
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5_2F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｆ-⑥
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_6F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑦控除税額小計
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_7F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑧控除不足還付税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_8F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑨差引税額
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_9F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑩
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_10F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑪
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_11F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑫
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_12F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j=0;

	//Ｆ-⑬
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_13F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑭
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_14F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑮
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_15F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ-⑯
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_16F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//if( j = _xprn( PP, 4, PF1, 0, pDC, isNoImage, 1 ) ){
	//	goto POUT1;
	//}

	if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, isNoImage, emfNo ) ) ) {
		goto POUT1;
	}

	if (AP) {
		delete AP;
		AP = NULL;
	}

POUT1:
	if (AP) {
		delete AP;
		AP = NULL;
	}
	
	return j;
	return 0;
}


//-----------------------------------------------------------------------------
// 付表１－２出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo1_02( CDC *pDC, int kojin, int pcnt )
{

	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp, PW0[MONY_BUF_SIZE];

	short emfNo = 0;
	long smd = 0, emd= 0;
	GetKazeiKikan( smd, emd );
	if( emd >= ICS_SH_INVOICE_DAY ){
		emfNo = MAKEWORD( 1, 1 );
	}
	else{
		if ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) {
			if ( !( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) {
				emfNo = MAKEWORD ( 1, 1 );
			}	
			else {
				emfNo = MAKEWORD( 2, 2 );
			}
		}
		else {
			emfNo = MAKEWORD( 1, 1 );
		}
	}

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}


	char	WORK0[MONY_BUF_SIZE] = { 0 };

	memset(PW0, '\0', MONY_BUF_SIZE);
	m_Arith.l_input(PW0, _T("1000"));

	memset(WORK0, '\0', MONY_BUF_SIZE);

	PBUF	PF1[30];
	int		PP;
	//PP = 32;
	PP = 21;

	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;

	int CCSw = 0;

	// 強制課税期間印刷
	if (CCSw) {		// 強制期間変更
	}
	else {
		char	yymmdd[3] = { 0 };
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
		PF1[i].PITM[0] = dp;
		dp += 9;

		memset(yymmdd, '\0', sizeof(yymmdd));
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}

	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	//税率３％適用分Ａ
	j = 0;
	//Ａ-①
	m_Arith.l_div(WORK0, m_pSnFhyo1_02Data->Sn_F102_1A, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;
	//Ａ-①-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_1_1A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ａ-①-2
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-②
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_2A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ａ-③
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_3A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ａ-④
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_4A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ａ-⑤
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ａ-⑤-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5_1A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ａ-⑤-2
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-⑥
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_6A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ-⑦控除税額小計
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_7A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ-⑧控除不足還付税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_8A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ-⑨差引税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_9A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ-⑩
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-⑪
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-⑫
	PF1[i].PITM[j++] = (char *)0;

	i++;
	j = 0;

	//Ａ-⑬
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-⑭
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-⑮
	PF1[i].PITM[j++] = (char *)0;
	//Ａ-⑯
	PF1[i].PITM[j++] = (char *)0;

	//if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT12;
	}

	//税率４％適用分Ｂ
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);
	//Ｂ-①
	m_Arith.l_div(WORK0, m_pSnFhyo1_02Data->Sn_F102_1B, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;
	//Ｂ-①-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_1_1B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-①-2
	PF1[i].PITM[j++] = (char *)0;
	//Ｂ-②
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_2B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-③
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_3B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-④
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_4B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑤
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑤-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5_1B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑤-2
	PF1[i].PITM[j++] = (char *)0;
	//Ｂ-⑥
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_6B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ-⑦控除税額小計
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_7B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ-⑧控除不足還付税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_8B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ-⑨差引税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_9B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ-⑩
	PF1[i].PITM[j++] = (char *)0;
	//Ｂ-⑪
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_11B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑫
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_12B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j = 0;

	//Ｂ-⑬
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_13B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑭
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_14B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑮
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_15B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｂ-⑯
	PF1[i].PITM[j++] = (char *)0;

	//if (j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT12;
	}

	//税率6.3%適用分Ｃ
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);
	//Ｃ-①
	m_Arith.l_div(WORK0, m_pSnFhyo1_02Data->Sn_F102_1C, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;
	//Ｃ-①-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_1_1C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-①-2
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_1_2C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-②
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_2C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-③
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_3C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-④
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_4C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-⑤
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-⑤-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5_1C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-⑤-2
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5_2C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｃ-⑥
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_6C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑦控除税額小計
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_7C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ-⑧控除不足還付税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_8C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑨差引税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_9C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑩
	PF1[i].PITM[j++] = (char *)0;

	//Ｃ-⑪
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_11C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑫
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_12C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j = 0;

	//Ｃ-⑬
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_13C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑭
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_14C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑮
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_15C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ-⑯
	PF1[i].PITM[j++] = (char *)0;

	//if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT12;
	}
	
	//合計（Ａ＋Ｂ＋Ｃ）①
	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);
	//Ｘ-①
	m_Arith.l_div(WORK0, m_pSnFhyo1_02Data->Sn_F102_1X, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;
	//Ｘ-①-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_1_1X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-①-2
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_1_2X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-②
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_2X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-③
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_3X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-④
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_4X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑤
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑤-1
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5_1X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑤-2
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_5_2X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ｘ-⑥
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_6X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑦控除税額小計
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_7X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑧控除不足還付税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_8X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑨差引税額
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_9X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑩
	PF1[i].PITM[j++] = (char *)0;

	//Ｘ-⑪
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_11X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑫
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_12X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j = 0;

	//Ｘ-⑬
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_13X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑭
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_14X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑮
	m_Arith.l_print(dp, m_pSnFhyo1_02Data->Sn_F102_15X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ-⑯
	PF1[i].PITM[j++] = (char *)0;

	//if (j = _xprn(PP, 4, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 4, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT12;
	}

	if(AP){
		delete AP;
		AP = NULL;
	}


POUT12:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return j;
	return 0;
}

//=============================================================================
// 付表１－３出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo1_03( CDC *pDC, int kojin, int pcnt )
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp =NULL, 
					PW0[MONY_BUF_SIZE] = { 0 },
					PW1[MONY_BUF_SIZE] = { 0 };

	BOOL isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	size = 1024;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	char WORK0[MONY_BUF_SIZE] = { 0 };

	memset ( PW0, '\0', MONY_BUF_SIZE );
	m_Arith.l_input ( PW0, _T( "1000" ) );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	memset ( PW1, '\0', MONY_BUF_SIZE );
	m_Arith.l_input ( PW1, _T( "100" ) );

	PBUF PF1[30];
	int	 PP = 0;
	PP = 22;

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;

	int CCSw = 0;

	char yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;
	
	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;


	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT1;
	}

	// 税率6.24%適用分A
	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	// 1-A
	m_Arith.l_div ( WORK0, m_pSnFhyo1_01Data->Sn_F101_1D, PW0 );
	m_Arith.l_print ( dp, WORK0, FMT110_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "   " ) );
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 1-1-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_1_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-2-A
	PF1[i].PITM[j++] = ( char* )0;
	dp += 19;

	// 2-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_2D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 3-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_3D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_4D,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-1-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-2-A
	PF1[i].PITM[j++] = ( char* )0;
	dp += 19;

	// 6-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_6D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 7-A
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_7D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT1;
	}


	// 税率7.8%適用分B
	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	// 1-B
	m_Arith.l_div ( WORK0, m_pSnFhyo1_01Data->Sn_F101_1E, PW0 );
	m_Arith.l_print ( dp, WORK0, FMT110_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "   " ) );
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 1-1-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_1_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-2-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_1_2E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_2E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 3-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_3E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_4E,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-B
	m_Arith.l_print( dp, m_pSnFhyo1_01Data->Sn_F101_5E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-1-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-2-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5_2E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_6E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 7-B
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_7E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT1;
	}

	// 合計 ( A + B ) C
	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	// 1-C
	m_Arith.l_div ( WORK0, m_pSnFhyo1_01Data->Sn_F101_1F, PW0 );
	m_Arith.l_print ( dp, WORK0, FMT110_EX );
	strcat_s ( dp, 19, _T( "   " ) );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	// 1-1-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_1_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-2-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_1_2F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-2-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_2F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 3-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_3F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_4F,  FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-1-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-2-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_5_2F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_6F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 7-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_7F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 8-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_8F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 9-C
	m_Arith.l_div ( WORK0, m_pSnFhyo1_01Data->Sn_F101_9F, PW1 );
	m_Arith.l_print ( dp, WORK0, FMT120_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "  " ) );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 10-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_11F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 11-C
	m_Arith.l_div ( WORK0, m_pSnFhyo1_01Data->Sn_F101_12F, PW1 );
	m_Arith.l_print ( dp, WORK0, FMT120_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "  " ) );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 12-C
	m_Arith.l_print ( dp, m_pSnFhyo1_01Data->Sn_F101_14F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 13-C
	m_Arith.l_div ( WORK0, m_pSnFhyo1_01Data->Sn_F101_15F, PW1 );
	m_Arith.l_print ( dp, WORK0, FMT120_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "  " ) );
	PF1[i].PITM[j++] = dp;

	if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT1;
	}


POUT1:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//-----------------------------------------------------------------------------
// 付表２－１出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo2_01( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp;

	unsigned short	SEno = 1;
	if( m_pPrintCmInfo->PRtype&0x80 ){
	}
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = 0;	// イメージ出力有り
	}
	else{
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}

PBUF	PF1[30];
int		PP;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

	//PP = 33;
	PP = 25;

int	CCSw = 0;
int	FontFlg = 0;//0:12桁以下の場合、大きく表示 1:13桁以上の場合、小さくする
CString str = _T("");



	// 強制課税期間印刷
	if( CCSw ){	// 強制期間変更
	}
	else{
		char	yymmdd[3] = {0};
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;	
	dp += (40 + 1);
	
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT21;
	}

	i = 0;
	j = 0;

	// ①-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ①-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ①-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ①-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ②-X
	PF1[i].PITM[j++] = (char *)0;
	// ②-D
	PF1[i].PITM[j++] = (char *)0;
	// ②-E
	PF1[i].PITM[j++] = (char *)0;
	// ②-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_2F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ③-X
	PF1[i].PITM[j++] = (char *)0;
	// ③-D
	PF1[i].PITM[j++] = (char *)0;
	// ③-E
	PF1[i].PITM[j++] = (char *)0;
	// ③-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_3F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ④-X
	PF1[i].PITM[j++] = (char *)0;
	// ④-D
	PF1[i].PITM[j++] = (char *)0;
	// ④-E
	PF1[i].PITM[j++] = (char *)0;
	// ④-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_4F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ⑤-X
	PF1[i].PITM[j++] = (char *)0;
	// ⑤-D
	PF1[i].PITM[j++] = (char *)0;
	// ⑤-E
	PF1[i].PITM[j++] = (char *)0;
	// ⑤-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ⑥-X
	PF1[i].PITM[j++] = (char *)0;
	// ⑥-D
	PF1[i].PITM[j++] = (char *)0;
	// ⑥-E
	PF1[i].PITM[j++] = (char *)0;
	// ⑥-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_6F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑦-X
	PF1[i].PITM[j++] = (char *)0;
	// ⑦-D
	PF1[i].PITM[j++] = (char *)0;
	// ⑦-E
	PF1[i].PITM[j++] = (char *)0;
	// ⑦-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_7F, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;


	// ⑧-F
	if ( m_pSnHeadData->Sn_UPERS & 0x10 ) {
		wsprintf( dp, _T("%3d.%02d"), m_pSnFhyo2_01Data->Sn_F201_8F_UW / 100, m_pSnFhyo2_01Data->Sn_F201_8F_UW % 100 );//yoshida
	}
	else {
		wsprintf( dp, _T("%3d.%02d"), m_pSnFhyo2_01Data->Sn_F201_8EX / 100, m_pSnFhyo2_01Data->Sn_F201_8EX % 100 );//yoshida
	}
	PF1[i++].PITM[0] = dp;
	dp += 7;
	
	j = 0;
	// ⑨-X
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_9X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑨-D
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_9D, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑨-E
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_9E, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑨-F
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_9F, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	for( int t = 0; t < 28; t++){ //①～⑦
		if( PF1[0].PITM[t] == NULL ) continue;
		str.Format( _T("%s") , PF1[0].PITM[t] );
		str.TrimLeft();
		if( str.GetLength() > 15 ){
			FontFlg = 1;
			break;
		}
	}
	if( !FontFlg ){ //⑨
		for( int t = 0; t < 4; t++){
			if( PF1[2].PITM[t] == NULL ) continue;
			str.Format( _T("%s") , PF1[2].PITM[t] );
			str.TrimLeft();
			if( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}

	if( FontFlg ){
		if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT21;
		}
	}
	else{
		if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	// ⑩-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑩-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑩-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑩-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10F, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	j = 0;
	// ⑪-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_11X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	
	// ⑪-D
	PF1[i].PITM[j++] = (char *)0;

	// ⑪-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_11E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑪-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_11F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑫-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_12X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	
	// ⑫-D
	PF1[i].PITM[j++] = (char *)0;

	// ⑫-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_12E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑫-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_12F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_13X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_13D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_13E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_13F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-X
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_14X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-D
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_14D, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-E
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_14E, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-F
	m_Arith.l_print(dp, m_pSnFhyo2_01Data->Sn_F201_14F, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_15X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_15D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_15E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_15F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_16X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_16D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_16E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_16F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_17X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_17D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_17E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_17F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_18X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_18D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_18E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_18F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if (FontFlg) {
		if (j = _xprn(PP, 5, PF1, 0, pDC, 0, 1)) {
			goto POUT21;
		}
	}
	else {
		if (j = _xprn(PP, 4, PF1, 0, pDC, 0, 1)) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// ⑲-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_19X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑲-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_19D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑲-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_19E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑲-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_19F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_20X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_20D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_20E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_20F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_21X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_21D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_21E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_21F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_22X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_22D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_22E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_22F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_23X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_23D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_23E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_23F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_24X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_24D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_24E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_24F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-X
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_25X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-D
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_25D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-E
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_25E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-F
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_25F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if( FontFlg ){
		if( j = _xprn( PP, 7, PF1, 0, pDC, 0, 1 ) ){
			goto POUT21;
		}
	}
	else{
		if( j = _xprn( PP, 6, PF1, 0, pDC, 0, 1 ) ){
			goto POUT21;
		}
	}

	if(AP){
		delete AP;
		AP = NULL;
	}


POUT21:
	if( AP ){
		delete AP;
		AP = NULL;
	}
	
	return j;
}

//=============================================================================
// 付表２－１出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo2_11( CDC *pDC, int kojin, int pcnt )
{

	int	i = 0, j = 0;
	unsigned int size = 0;
	char			*AP = NULL;
	char			*dp = NULL;

	unsigned short	SEno = 1;

	BOOL	isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = 0;	// イメージ出力有り
	}
	else{
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	PBUF PF1[30];
	int	PP = 0;

	dp = AP;
	memset ( dp, '\0', size );
	i = 0;

	PP = 27;

	int	CCSw = 0;
	int	FontFlg = 0;	//0:12桁以下の場合、大きく表示 1:13桁以上の場合、小さくする
	CString str;

	 char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;	
	dp += ( 40 + 1 );

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ) ) {
		goto POUT21;
	}

	i = 0;
	j = 0;

	// 1-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-X
	PF1[i].PITM[j++] = ( char* )0;
	// 2-D
	PF1[i].PITM[j++] = ( char* )0;
	// 2-E
	PF1[i].PITM[j++] = ( char* )0;
	// 2-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_2F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-X
	PF1[i].PITM[j++] = ( char* )0;
	// 3-D
	PF1[i].PITM[j++] = ( char* )0;
	// 3-E
	PF1[i].PITM[j++] = ( char* )0;
	// 3-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_3F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 4-X
	PF1[i].PITM[j++] = ( char* )0;
	// 4-D
	PF1[i].PITM[j++] = ( char* )0;
	// 4-E
	PF1[i].PITM[j++] = ( char* )0;
	// 4-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_4F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-X
	PF1[i].PITM[j++] = ( char* )0;
	// 5-D
	PF1[i].PITM[j++] = ( char* )0;
	// 5-E
	PF1[i].PITM[j++] = ( char* )0;
	// 5-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-X
	PF1[i].PITM[j++] = ( char* )0;
	// 6-D
	PF1[i].PITM[j++] = ( char* )0;
	// 6-E
	PF1[i].PITM[j++] = ( char* )0;
	// 6-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_6F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 7-X
	PF1[i].PITM[j++] = ( char* )0;
	// 7-D
	PF1[i].PITM[j++] = ( char* )0;
	// 7-E
	PF1[i].PITM[j++] = ( char* )0;
	// 7-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_7F, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;

	// 8-F
	if ( m_pSnHeadData->Sn_UPERS & 0x10 ) {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW % 100 ) );
	}
	else {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8EX / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8EX % 100 ) );
	}
	PF1[i++].PITM[0] = dp;
	dp += 7;

	j = 0;
	// 9-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	for ( int t = 0; t < 28; t++ ) { // 1～7
		if ( PF1[0].PITM[t] == NULL ) {
			continue;
		}
		str.Format ( _T( "%s" ) , PF1[0].PITM[t] );
		str.TrimLeft();
		if ( str.GetLength() > 15 ) {
			FontFlg = 1;
			break;
		}
	}
	if ( !FontFlg ) { // 9
		for ( int t = 0; t < 4; t++ ) {
			if ( PF1[2].PITM[t] == NULL ) {
				continue;
			}
			str.Format ( _T( "%s" ) , PF1[2].PITM[t] );
			str.TrimLeft();
			if ( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}

	if( FontFlg ) {
		if ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ) {
			goto POUT21;
		}
	}
	else {
		if ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 10-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10F, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	j = 0;
	// 11-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-D
	PF1[i].PITM[j++] = ( char* )0;

	// 11-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-D
	PF1[i].PITM[j++] = ( char* )0;

	// 12-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 5, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 19-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 7, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 6, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

POUT21:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//=============================================================================
// 付表２－１出力　インボイス対応
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo2_11Ex( CDC *pDC, int kojin, int pcnt )
{

	int	i = 0, j = 0;
	unsigned int size = 0;
	char			*AP = NULL;
	char			*dp = NULL;

	unsigned short	SEno = 1;

	BOOL	isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = 0;	// イメージ出力有り
	}
	else{
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	PBUF PF1[30];
	int	PP = 0;

	dp = AP;
	memset ( dp, '\0', size );
	i = 0;

	PP = 31;

	int	CCSw = 0;
	int	FontFlg = 0;	//0:12桁以下の場合、大きく表示 1:13桁以上の場合、小さくする
	CString str;

	 char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;	
	dp += ( 40 + 1 );

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ) ) {
		goto POUT21;
	}

	i = 0;
	j = 0;

	// 1-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-X
	PF1[i].PITM[j++] = ( char* )0;
	// 2-D
	PF1[i].PITM[j++] = ( char* )0;
	// 2-E
	PF1[i].PITM[j++] = ( char* )0;
	// 2-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_2F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-X
	PF1[i].PITM[j++] = ( char* )0;
	// 3-D
	PF1[i].PITM[j++] = ( char* )0;
	// 3-E
	PF1[i].PITM[j++] = ( char* )0;
	// 3-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_3F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 4-X
	PF1[i].PITM[j++] = ( char* )0;
	// 4-D
	PF1[i].PITM[j++] = ( char* )0;
	// 4-E
	PF1[i].PITM[j++] = ( char* )0;
	// 4-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_4F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-X
	PF1[i].PITM[j++] = ( char* )0;
	// 5-D
	PF1[i].PITM[j++] = ( char* )0;
	// 5-E
	PF1[i].PITM[j++] = ( char* )0;
	// 5-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-X
	PF1[i].PITM[j++] = ( char* )0;
	// 6-D
	PF1[i].PITM[j++] = ( char* )0;
	// 6-E
	PF1[i].PITM[j++] = ( char* )0;
	// 6-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_6F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 7-X
	PF1[i].PITM[j++] = ( char* )0;
	// 7-D
	PF1[i].PITM[j++] = ( char* )0;
	// 7-E
	PF1[i].PITM[j++] = ( char* )0;
	// 7-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_7F, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;

	// 8-F
	if ( m_pSnHeadData->Sn_UPERS & 0x10 ) {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW % 100 ) );
	}
	else {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8EX / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8EX % 100 ) );
	}
	PF1[i++].PITM[0] = dp;
	dp += 7;

	j = 0;
	// 9-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	for ( int t = 0; t < 28; t++ ) { // 1～7
		if ( PF1[0].PITM[t] == NULL ) {
			continue;
		}
		str.Format ( _T( "%s" ) , PF1[0].PITM[t] );
		str.TrimLeft();
		if ( str.GetLength() > 15 ) {
			FontFlg = 1;
			break;
		}
	}
	if ( !FontFlg ) { // 9
		for ( int t = 0; t < 4; t++ ) {
			if ( PF1[2].PITM[t] == NULL ) {
				continue;
			}
			str.Format ( _T( "%s" ) , PF1[2].PITM[t] );
			str.TrimLeft();
			if ( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}

	if( FontFlg ) {
		if ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ) {
			goto POUT21;
		}
	}
	else {
		if ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 10-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_10F, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	//---->20230120
	j = 0;
	// 11-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_27X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_27D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_27E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_27F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	
	// 12-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_28X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_28D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_28E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_28F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//<----20230120

	// 13-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-D
	PF1[i].PITM[j++] = ( char* )0;

	// 13-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-D
	PF1[i].PITM[j++] = ( char* )0;

	// 14-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 5, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 21-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-X
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-D
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-E
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-F
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 7, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 6, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

POUT21:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}


//-----------------------------------------------------------------------------
// 付表２－２出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo2_02( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp;

	unsigned short	SEno = 1;
	if (m_pPrintCmInfo->PRtype & 0x80) {
	}

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = 0;	// イメージ出力有り
	}
	else {
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}

	PBUF	PF1[30];
	int		PP;

	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;

	//PP = 34;
	PP = 26;

	int	CCSw = 0;
	int	FontFlg = 0;//0:12桁以下の場合、大きく表示 1:13桁以上の場合、小さくする
	CString str = _T("");



	// 強制課税期間印刷
	if (CCSw) {	// 強制期間変更
	}
	else {
		char	yymmdd[3] = { 0 };
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset(yymmdd, '\0', sizeof(yymmdd));
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, SEno)) {
		goto POUT22;
	}

	i = 0;
	j = 0;

	// ①-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_1A, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ①-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_1B, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ①-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_1C, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ①-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_1X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ②-A
	PF1[i].PITM[j++] = (char *)0;
	// ②-B
	PF1[i].PITM[j++] = (char *)0;
	// ②-C
	PF1[i].PITM[j++] = (char *)0;
	// ②-X
	PF1[i].PITM[j++] = (char *)0;

	// ③-A
	PF1[i].PITM[j++] = (char *)0;
	// ③-B
	PF1[i].PITM[j++] = (char *)0;
	// ③-C
	PF1[i].PITM[j++] = (char *)0;
	// ③-X
	PF1[i].PITM[j++] = (char *)0;

	// ④-A
	PF1[i].PITM[j++] = (char *)0;
	// ④-B
	PF1[i].PITM[j++] = (char *)0;
	// ④-C
	PF1[i].PITM[j++] = (char *)0;
	// ④-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_4X, FMT16_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// ⑤-A
	PF1[i].PITM[j++] = (char *)0;
	// ⑤-B
	PF1[i].PITM[j++] = (char *)0;
	// ⑤-C
	PF1[i].PITM[j++] = (char *)0;
	// ⑤-X
	PF1[i].PITM[j++] = (char *)0;

	// ⑥-A
	PF1[i].PITM[j++] = (char *)0;
	// ⑥-B
	PF1[i].PITM[j++] = (char *)0;
	// ⑥-C
	PF1[i].PITM[j++] = (char *)0;
	// ⑥-X
	PF1[i].PITM[j++] = (char *)0;

	// ⑦-A
	PF1[i].PITM[j++] = (char *)0;
	// ⑦-B
	PF1[i].PITM[j++] = (char *)0;
	// ⑦-C
	PF1[i].PITM[j++] = (char *)0;
	// ⑦-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_7X, FMT16_EX);
	PF1[i++].PITM[j] = dp;
	dp += 19;


	// ⑧-X
	//m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_8X, FMT16_EX);
	wsprintf( dp, _T("%3d.%02d"), m_pSnFhyo2_02Data->Sn_F202_8EX / 100, m_pSnFhyo2_02Data->Sn_F202_8EX % 100 );
	PF1[i++].PITM[0] = dp;
	dp += 7;

	j = 0;
	// ⑨-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_9A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑨-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_9B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑨-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_9C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑨-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_9X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	for (int t = 0; t < 28; t++) { //①～⑦
		if (PF1[0].PITM[t] == NULL) continue;
		str.Format(_T("%s"), PF1[0].PITM[t]);
		str.TrimLeft();
		if (str.GetLength() > 15) {
			FontFlg = 1;
			break;
		}
	}
	if (!FontFlg) { //⑨
		for (int t = 0; t < 4; t++) {
			if (PF1[2].PITM[t] == NULL) continue;
			str.Format(_T("%s"), PF1[2].PITM[t]);
			str.TrimLeft();
			if (str.GetLength() > 15) {
				FontFlg = 1;
				break;
			}
		}
	}

	if (FontFlg) {
		if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, SEno)) {
			goto POUT22;
		}
	}
	else {
		if (j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, SEno)) {
			goto POUT22;
		}
	}

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// ⑩-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_10A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑩-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_10B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑩-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_10C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑩-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_10X, FMT15);
	PF1[i++].PITM[j] = dp;
	dp += 16;

	j = 0;
	// ⑪-A
	PF1[i].PITM[j++] = (char *)0;

	// ⑪-B
	PF1[i].PITM[j++] = (char *)0;

	// ⑪-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_11C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑪-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_11X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑫-A
	PF1[i].PITM[j++] = (char *)0;

	// ⑫-B
	PF1[i].PITM[j++] = (char *)0;

	// ⑫-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_12C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑫-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_12X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_13A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_13B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_13C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑬-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_13X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_14A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_14B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_14C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑭-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_14X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_15A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_15B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_15C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑮-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_15X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_16A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_16B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_16C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑯-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_16X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_17A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_17B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_17C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑰-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_17X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_18A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_18B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_18C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑱-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_18X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if (FontFlg) {
		if (j = _xprn(PP, 5, PF1, 0, pDC, 0, 1)) {
			goto POUT22;
		}
	}
	else {
		if (j = _xprn(PP, 4, PF1, 0, pDC, 0, 1)) {
			goto POUT22;
		}
	}

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);

	// ⑲-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_19A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑲-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_19B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑲-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_19C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// ⑲-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_19X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_20A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_20B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_20C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_20X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_21A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_21B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_21C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_21X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_22A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_22B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_22C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_22X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_23A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_23B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_23C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_23X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_24A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_24B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_24C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_24X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-A
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_25A, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_25B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-C
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_25C, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-X
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_25X, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if (FontFlg) {
		if (j = _xprn(PP, 7, PF1, 0, pDC, 0, 1)) {
			goto POUT22;
		}
	}
	else {
		if (j = _xprn(PP, 6, PF1, 0, pDC, 0, 1)) {
			goto POUT22;
		}
	}

	if(AP){
		delete AP;
		AP = NULL;
	}

POUT22:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	// 課税貨物に係る消費税額　斜線
	int yPlus = (int)m_pPrintCmInfo->pPrMove[AD_FHYO2]._PIVad;
	int xPlus = (int)m_pPrintCmInfo->pPrMove[AD_FHYO2]._PIHad;

	if ( m_Arith.l_test(m_pSnFhyo2_02Data->Sn_F202_13A) == 0)
		m_pVprn->DrawSline(0, LKnrW0, 9500+xPlus, 15802+yPlus, 11990+xPlus, 14920+yPlus);
	if ( m_Arith.l_test(m_pSnFhyo2_02Data->Sn_F202_13B) == 0)
		m_pVprn->DrawSline(0, LKnrW0, 11990+xPlus, 15802+yPlus, 14480+xPlus, 14920+yPlus);

	return j;
}


//=============================================================================
// 付表２－２出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo2_12( CDC *pDC, int kojin, int pcnt )
{
	int	i = 0, j = 0;
	unsigned int size = 0;
	char *AP = NULL;
	char *dp = NULL;

	unsigned short	SEno = 1;

	BOOL isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = 0;	// イメージ出力有り
	}
	else {
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	PBUF PF1[30];
	int	PP = 0;

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;

	PP = 28;

	int	CCSw = 0;
	int	FontFlg = 0;	// 0:12桁以下の場合、大きく表示 1:13桁以上の場合、小さくする
	CString str;
	   
	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen (ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;
	dp += ( 40 + 1 );

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ) ) {
		goto POUT22;
	}

	i = 0;
	j = 0;

	// 1-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1A, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1B, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-A
	PF1[i].PITM[j++] = ( char* )0;
	// 2-B
	PF1[i].PITM[j++] = ( char* )0;
	// 2-C
	PF1[i].PITM[j++] = ( char* )0;
	// 2-X
	PF1[i].PITM[j++] = ( char* )0;

	// 3-A
	PF1[i].PITM[j++] = ( char* )0;
	// 3-B
	PF1[i].PITM[j++] = ( char* )0;
	// 3-C
	PF1[i].PITM[j++] = ( char* )0;
	// 3-X
	PF1[i].PITM[j++] = ( char* )0;

	// 4-A
	PF1[i].PITM[j++] = ( char* )0;
	// 4-B
	PF1[i].PITM[j++] = ( char* )0;
	// 4-C
	PF1[i].PITM[j++] = ( char* )0;
	// 4-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_4X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	PF1[i].PITM[j++] = ( char* )0;
	// 5-B
	PF1[i].PITM[j++] = ( char* )0;
	// 5-C
	PF1[i].PITM[j++] = ( char* )0;
	// 5-X
	PF1[i].PITM[j++] = ( char* )0;

	// 6-A
	PF1[i].PITM[j++] = ( char* )0;
	// 6-B
	PF1[i].PITM[j++] = ( char* )0;
	// 6-C
	PF1[i].PITM[j++] = ( char* )0;
	// 6-X
	PF1[i].PITM[j++] = ( char* )0;

	// 7-A
	PF1[i].PITM[j++] = ( char* )0;
	// 7-B
	PF1[i].PITM[j++] = ( char* )0;
	// 7-C
	PF1[i].PITM[j++] = ( char* )0;
	// 7-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_7X, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;


	// 8-X
	wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_02Data->Sn_F202_8EX / 100 ), ( m_pSnFhyo2_02Data->Sn_F202_8EX % 100 ) );
	PF1[i++].PITM[0] = dp;
	dp += 7;

	j = 0;
	// 9-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	for (int t = 0; t < 28; t++) { // 1～7
		if ( PF1[0].PITM[t] == NULL ) {
			continue;
		}
		str.Format ( _T( "%s" ), PF1[0].PITM[t] );
		str.TrimLeft();
		if ( str.GetLength() > 15 ) {
			FontFlg = 1;
			break;
		}
	}
	if ( !FontFlg ) { // 9
		for ( int t = 0; t < 4; t++ ) {
			if ( PF1[2].PITM[t] == NULL ) {
				continue;
			}
			str.Format ( _T( "%s" ), PF1[2].PITM[t] );
			str.TrimLeft();
			if ( str.GetLength() > 15 ) {
				FontFlg = 1;
				break;
			}
		}
	}

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT22;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT22;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 10-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10X, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	j = 0;
	// 11-A
	PF1[i].PITM[j++] = ( char* )0;

	// 11-B
	PF1[i].PITM[j++] = ( char* )0;

	// 11-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_11C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_11X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-A
	PF1[i].PITM[j++] = ( char* )0;

	// 12-B
	PF1[i].PITM[j++] = ( char* )0;

	// 12-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_12C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_12X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_13A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_13B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_13C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_13X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 5, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 19-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26X, FMT15 );
	PF1[i].PITM[j++] = dp;

	dp += 16;
	// 24-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 7, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 6, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}

POUT22:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	// 課税貨物に係る消費税額　斜線
	int yPlus = ( int )m_pPrintCmInfo->pPrMove[AD_FHYO2]._PIVad;
	int xPlus = ( int )m_pPrintCmInfo->pPrMove[AD_FHYO2]._PIHad;

	if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_13A ) == 0 ) {
		m_pVprn->DrawSline ( 0, LKnrW0, ( 9500 + xPlus ), ( 15802 + yPlus ), ( 11990 + xPlus ), ( 14920 + yPlus ) );
	}
	if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_13B ) == 0 ) { 
		m_pVprn->DrawSline ( 0, LKnrW0, ( 11990 + xPlus ), ( 15802 + yPlus ), ( 14480 + xPlus ), ( 14920 + yPlus ) );
	}

	return j;
}

//=============================================================================
// 付表２－２出力  インボイス対応
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo2_12Ex( CDC *pDC, int kojin, int pcnt )
{
	int	i = 0, j = 0;
	unsigned int size = 0;
	char *AP = NULL;
	char *dp = NULL;

	unsigned short	SEno = 1;

	BOOL isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = 0;	// イメージ出力有り
	}
	else {
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	PBUF PF1[30];
	int	PP = 0;

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;

	PP = 32;

	int	CCSw = 0;
	int	FontFlg = 0;	// 0:12桁以下の場合、大きく表示 1:13桁以上の場合、小さくする
	CString str;
	   
	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen (ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;
	dp += ( 40 + 1 );

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ) ) {
		goto POUT22;
	}

	i = 0;
	j = 0;

	// 1-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1A, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1B, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_1X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-A
	PF1[i].PITM[j++] = ( char* )0;
	// 2-B
	PF1[i].PITM[j++] = ( char* )0;
	// 2-C
	PF1[i].PITM[j++] = ( char* )0;
	// 2-X
	PF1[i].PITM[j++] = ( char* )0;

	// 3-A
	PF1[i].PITM[j++] = ( char* )0;
	// 3-B
	PF1[i].PITM[j++] = ( char* )0;
	// 3-C
	PF1[i].PITM[j++] = ( char* )0;
	// 3-X
	PF1[i].PITM[j++] = ( char* )0;

	// 4-A
	PF1[i].PITM[j++] = ( char* )0;
	// 4-B
	PF1[i].PITM[j++] = ( char* )0;
	// 4-C
	PF1[i].PITM[j++] = ( char* )0;
	// 4-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_4X, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	PF1[i].PITM[j++] = ( char* )0;
	// 5-B
	PF1[i].PITM[j++] = ( char* )0;
	// 5-C
	PF1[i].PITM[j++] = ( char* )0;
	// 5-X
	PF1[i].PITM[j++] = ( char* )0;

	// 6-A
	PF1[i].PITM[j++] = ( char* )0;
	// 6-B
	PF1[i].PITM[j++] = ( char* )0;
	// 6-C
	PF1[i].PITM[j++] = ( char* )0;
	// 6-X
	PF1[i].PITM[j++] = ( char* )0;

	// 7-A
	PF1[i].PITM[j++] = ( char* )0;
	// 7-B
	PF1[i].PITM[j++] = ( char* )0;
	// 7-C
	PF1[i].PITM[j++] = ( char* )0;
	// 7-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_7X, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;


	// 8-X
	wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_02Data->Sn_F202_8EX / 100 ), ( m_pSnFhyo2_02Data->Sn_F202_8EX % 100 ) );
	PF1[i++].PITM[0] = dp;
	dp += 7;

	j = 0;
	// 9-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_9X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	for (int t = 0; t < 28; t++) { // 1～7
		if ( PF1[0].PITM[t] == NULL ) {
			continue;
		}
		str.Format ( _T( "%s" ), PF1[0].PITM[t] );
		str.TrimLeft();
		if ( str.GetLength() > 15 ) {
			FontFlg = 1;
			break;
		}
	}
	if ( !FontFlg ) { // 9
		for ( int t = 0; t < 4; t++ ) {
			if ( PF1[2].PITM[t] == NULL ) {
				continue;
			}
			str.Format ( _T( "%s" ), PF1[2].PITM[t] );
			str.TrimLeft();
			if ( str.GetLength() > 15 ) {
				FontFlg = 1;
				break;
			}
		}
	}

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT22;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT22;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 10-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_10X, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	//---->20230120
	j = 0;
	// 11-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_27A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_27B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_27C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_27X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_28A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_28B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_28C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_28X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//<----20230120

	// 13-A
	PF1[i].PITM[j++] = ( char* )0;

	// 13-B
	PF1[i].PITM[j++] = ( char* )0;

	// 13-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_11C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_11X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-A
	PF1[i].PITM[j++] = ( char* )0;

	// 14-B
	PF1[i].PITM[j++] = ( char* )0;

	// 14-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_12C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_12X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_13A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-B
	m_Arith.l_print(dp, m_pSnFhyo2_02Data->Sn_F202_13B, FMT15);
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_13C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_13X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_14X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_15X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_16X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_17X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_18X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 5, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 21-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_19X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_20X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_21X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_22X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_26X, FMT15 );
	PF1[i].PITM[j++] = dp;

	dp += 16;
	// 26-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_23X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_24X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-A
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-B
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-C
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-X
	m_Arith.l_print ( dp, m_pSnFhyo2_02Data->Sn_F202_25X, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 7, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 6, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT22;
		}
	}

POUT22:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//=============================================================================
// 付表２－３出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo2_03( CDC *pDC, int kojin, int pcnt )
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp = NULL;

	unsigned short	SEno = 1;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
	}

	BOOL	isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = 0;	// イメージ出力有り
	}
	else {
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	PBUF PF1[30];
	int	PP = 0;

	dp = AP;
	memset ( dp, '\0', size );
	i = 0;

	PP = 30;

	int	CCSw = 0;
	int	FontFlg = 0;
	CString str;

	char yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof (m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;	
	dp += ( 40 + 1 );

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ) ) {
		goto POUT21;
	}

	i = 0;
	j = 0;

	// 1-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-A
	PF1[i].PITM[j++] = ( char* )0;

	// 2-B
	PF1[i].PITM[j++] = ( char* )0;

	// 2-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_2F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-A
	PF1[i].PITM[j++] = ( char* )0;

	// 3-B
	PF1[i].PITM[j++] = ( char* )0;

	// 3-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_3F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 4-A
	PF1[i].PITM[j++] = ( char* )0;

	// 4-B
	PF1[i].PITM[j++] = ( char* )0;

	// 4-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_4F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	PF1[i].PITM[j++] = ( char* )0;

	// 5-B
	PF1[i].PITM[j++] = ( char* )0;

	// 5-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-A
	PF1[i].PITM[j++] = ( char* )0;

	// 6-B
	PF1[i].PITM[j++] = ( char* )0;

	// 6-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_6F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 7-A
	PF1[i].PITM[j++] = ( char* )0;

	// 7-B
	PF1[i].PITM[j++] = ( char* )0;

	// 7-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_7F, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;

	// 8-C
	if ( m_pSnHeadData->Sn_UPERS & 0x10 ) {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW % 100 ) );
	}
	else {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8EX / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8EX % 100 ) );
	}
	PF1[i++].PITM[0] = dp;
	dp += 7;
	j = 0;

	// 9-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-C
	m_Arith.l_print (dp, m_pSnFhyo2_01Data->Sn_F201_9F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 1～7
	for ( int t = 0; t < 21; t++) {
		if ( PF1[0].PITM[t] == NULL ) {
			continue;
		}
		str.Format ( _T( "%s" ) , PF1[0].PITM[t] );
		str.TrimLeft();
		if ( str.GetLength() > 15 ) {
			FontFlg = 1;
			break;
		}
	}
	// 9
	if ( !FontFlg ) {
		for ( int t = 0; t < 3; t++) {
			if ( PF1[2].PITM[t] == NULL ){
				continue;
			}
			str.Format ( _T( "%s" ) , PF1[2].PITM[t] );
			str.TrimLeft();
			if ( str.GetLength() > 15 ) {
				FontFlg = 1;
				break;
			}
		}
	}

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT21;
		}
	}
	else{
		if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 10-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-B
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-C
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10F, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	j = 0;
	// 11-A
	PF1[i].PITM[j++] = ( char* )0;

	// 11-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-A
	PF1[i].PITM[j++] = ( char* )0;

	// 12-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 5, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 19-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_20D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_25D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if  ( FontFlg ){
		if ( ( j = _xprn ( PP, 7, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 6, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

POUT21:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//=============================================================================
// 付表２－３出力  インボイス対応
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo2_03Ex( CDC *pDC, int kojin, int pcnt )
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp = NULL;

	unsigned short	SEno = 1;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
	}

	BOOL	isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = 0;	// イメージ出力有り
	}
	else {
		isNoImage = 1;	// イメージ出力無し
	}

	size = 1024;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	PBUF PF1[30];
	int	PP = 0;

	dp = AP;
	memset ( dp, '\0', size );
	i = 0;

	PP = 33;

	int	CCSw = 0;
	int	FontFlg = 0;
	CString str;

	char yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof (m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;	
	dp += ( 40 + 1 );

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ) ) {
		goto POUT21;
	}

	i = 0;
	j = 0;

	// 1-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1D, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1E, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 1-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_1F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-A
	PF1[i].PITM[j++] = ( char* )0;

	// 2-B
	PF1[i].PITM[j++] = ( char* )0;

	// 2-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_2F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-A
	PF1[i].PITM[j++] = ( char* )0;

	// 3-B
	PF1[i].PITM[j++] = ( char* )0;

	// 3-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_3F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 4-A
	PF1[i].PITM[j++] = ( char* )0;

	// 4-B
	PF1[i].PITM[j++] = ( char* )0;

	// 4-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_4F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	PF1[i].PITM[j++] = ( char* )0;

	// 5-B
	PF1[i].PITM[j++] = ( char* )0;

	// 5-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_5F, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-A
	PF1[i].PITM[j++] = ( char* )0;

	// 6-B
	PF1[i].PITM[j++] = ( char* )0;

	// 6-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_6F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 7-A
	PF1[i].PITM[j++] = ( char* )0;

	// 7-B
	PF1[i].PITM[j++] = ( char* )0;

	// 7-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_7F, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;

	// 8-C
	if ( m_pSnHeadData->Sn_UPERS & 0x10 ) {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8F_UW % 100 ) );
	}
	else {
		wsprintf ( dp, _T( "%3d.%02d" ), ( m_pSnFhyo2_01Data->Sn_F201_8EX / 100 ), ( m_pSnFhyo2_01Data->Sn_F201_8EX % 100 ) );
	}
	PF1[i++].PITM[0] = dp;
	dp += 7;
	j = 0;

	// 9-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_9E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-C
	m_Arith.l_print (dp, m_pSnFhyo2_01Data->Sn_F201_9F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 1～7
	for ( int t = 0; t < 21; t++) {
		if ( PF1[0].PITM[t] == NULL ) {
			continue;
		}
		str.Format ( _T( "%s" ) , PF1[0].PITM[t] );
		str.TrimLeft();
		if ( str.GetLength() > 15 ) {
			FontFlg = 1;
			break;
		}
	}
	// 9
	if ( !FontFlg ) {
		for ( int t = 0; t < 3; t++) {
			if ( PF1[2].PITM[t] == NULL ){
				continue;
			}
			str.Format ( _T( "%s" ) , PF1[2].PITM[t] );
			str.TrimLeft();
			if ( str.GetLength() > 15 ) {
				FontFlg = 1;
				break;
			}
		}
	}

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT21;
		}
	}
	else{
		if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 10-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-B
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-C
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_10F, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;

	//---->20230120
	j = 0;
	// 11-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_27D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-B
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_27E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-C
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_27F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_28D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-B
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_28E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-C
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_28F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//<----20230120

	// 13-A
	PF1[i].PITM[j++] = ( char* )0;

	// 13-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 13-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_11F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-A
	PF1[i].PITM[j++] = ( char* )0;

	// 14-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_12F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_13F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_14F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_15F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_16F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_17F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_18F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( FontFlg ) {
		if ( ( j = _xprn ( PP, 5, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );

	// 21-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 21-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_19F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_20D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 22-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_20F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 23-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_21F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 24-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_22F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 25-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_26F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 26-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_23F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-A
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 27-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_24F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-A
	m_Arith.l_print( dp, m_pSnFhyo2_01Data->Sn_F201_25D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-B
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 28-C
	m_Arith.l_print ( dp, m_pSnFhyo2_01Data->Sn_F201_25F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if  ( FontFlg ){
		if ( ( j = _xprn ( PP, 7, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}
	else {
		if ( ( j = _xprn ( PP, 6, PF1, 0, pDC, 0, 1 ) ) ) {
			goto POUT21;
		}
	}

POUT21:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//-----------------------------------------------------------------------------
// 付表４－１出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo4_01( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp, PW0[MONY_BUF_SIZE];

	short emfNo = 0;
	if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
		emfNo = MAKEWORD ( 2, 2 );
	}
	else {
		emfNo = MAKEWORD( 1, 1 );
	}

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}

	memset(PW0, '\0', MONY_BUF_SIZE);
	m_Arith.l_input(PW0, _T("1000"));

	char	WORK0[MONY_BUF_SIZE] = { 0 };
	memset(WORK0, '\0', MONY_BUF_SIZE);

PBUF	PF1[30];
int		PP;

	//PP = 31;
	PP = 20;

	dp = AP;
	memset( dp, '\0', size );
	i = j = 0;

int CCSw = 0;

	// 強制課税期間印刷
	if( CCSw ){		// 強制期間変更
	}
	else{
		char	yymmdd[3] = {0};
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;

		memset( yymmdd, '\0', sizeof(yymmdd) );
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}
	// 氏名
	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	// 旧税率分小計Ｘ
	j = 0;

	//Ｘ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_01Data->Sn_F401_1X, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｘ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_1_1X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_2X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_3X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_4X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_5X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－６
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_6X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－７
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_7X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－８
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_8X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－９
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_9X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ｘ－１１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_11X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_12X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_13X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_14X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_15X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１６
	PF1[i].PITM[j++] = (char *)0;

	//if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT41;
	}


	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	//Ｄ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_01Data->Sn_F401_1D, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｄ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_1_1D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_2D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_3D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_4D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_5D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－６
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_6D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－７
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_7D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－８
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_8D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－９
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_9D, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｄ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ｄ－１１
	PF1[i].PITM[j++] = (char *)0;

	//Ｄ－１２
	PF1[i].PITM[j++] = (char *)0;

	//Ｄ－１３
	PF1[i].PITM[j++] = (char *)0;

	//Ｄ－１４
	PF1[i].PITM[j++] = (char *)0;

	//Ｄ－１５
	PF1[i].PITM[j++] = (char *)0;

	//Ｄ－１６
	PF1[i].PITM[j++] = (char *)0;

	// (j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT41;
	}


	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	//Ｅ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_01Data->Sn_F401_1E, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｅ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_1_1E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_2E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_3E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_4E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_5E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－６
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_6E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－７
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_7E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－８
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_8E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－９
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_9E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ｅ－１１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_11E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－１２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_12E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－１３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_13E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－１４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_14E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－１５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_15E, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｅ－１６
	PF1[i].PITM[j++] = (char *)0;

	//if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, 1)) {
	if ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT41;
	}


	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	//Ｆ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_01Data->Sn_F401_1F, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｆ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_1_1F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_2F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_3F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_4F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_5F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－６
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_6F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－７
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_7F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－８
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_8F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－９
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_9F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１０
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_10F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１１
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_11F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１２
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_12F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１３
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_13F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１４
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_14F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１５
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_15F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｆ－１６
	m_Arith.l_print(dp, m_pSnFhyo4_01Data->Sn_F401_16F, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if (j = _xprn(PP, 4, PF1, 0, pDC, isNoImage, 1)) {
		goto POUT41;
	}

	// 氏名
	dp = AP;	i = j = 0;
	memset(dp, '\0', size);
	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[0].PITM[0] = dp;
	//if (j = _xprn(PP, 5, PF1, 2, pDC, 0, 1)) {
	if ( j = _xprn ( PP, 5, PF1, 2, pDC, 0, emfNo ) ) {
		goto POUT41;
	}

	if(AP){
		delete AP;
		AP = NULL;
	}


POUT41:
	if(AP){
		delete AP;
		AP = NULL;
	}

	return j;
	return 0;
}


//-----------------------------------------------------------------------------
// 付表４－２出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo4_02( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp, PW0[MONY_BUF_SIZE];

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}

	memset(PW0, '\0', MONY_BUF_SIZE);
	m_Arith.l_input(PW0, _T("1000"));

	char	WORK0[MONY_BUF_SIZE] = { 0 };
	memset(WORK0, '\0', MONY_BUF_SIZE);

	PBUF	PF1[30];
	int		PP;

	//PP = 32;
	PP = 21;

	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;

	int CCSw = 0;

	// 強制課税期間印刷
	if (CCSw) {		// 強制期間変更
	}
	else {
		char	yymmdd[3] = { 0 };
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
		PF1[i].PITM[0] = dp;
		dp += 9;

		memset(yymmdd, '\0', sizeof(yymmdd));
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	// 税率３％適用分Ａ
	j = 0;

	//Ａ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_02Data->Sn_F402_1A, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ａ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_1_1A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_2A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_3A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_4A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_5A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－６
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_6A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－７
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_7A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－８
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_8A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－９
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_9A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ａ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ａ－１１
	PF1[i].PITM[j++] = (char *)0;

	//Ａ－１２
	PF1[i].PITM[j++] = (char *)0;

	//Ａ－１３
	PF1[i].PITM[j++] = (char *)0;

	//Ａ－１４
	PF1[i].PITM[j++] = (char *)0;

	//Ａ－１５
	PF1[i].PITM[j++] = (char *)0;

	//Ａ－１６
	PF1[i].PITM[j++] = (char *)0;

	if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1)) {
		goto POUT42;
	}


	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	//Ｂ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_02Data->Sn_F402_1B, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｂ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_1_1B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_2B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_3B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_4B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_5B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－６
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_6B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－７
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_7B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－８
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_8B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－９
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_9B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ｂ－１１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_11B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－１２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_12B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－１３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_13B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－１４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_14B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－１５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_15B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｂ－１６
	PF1[i].PITM[j++] = (char *)0;

	if (j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, 1)) {
		goto POUT42;
	}


	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	//Ｃ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_02Data->Sn_F402_1C, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｃ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_1_1C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_2C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_3C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_4C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_5C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－６
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_6C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－７
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_7C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－８
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_8C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－９
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_9C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ｃ－１１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_11C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－１２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_12C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－１３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_13C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－１４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_14C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－１５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_15C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｃ－１６
	PF1[i].PITM[j++] = (char *)0;

	if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, 1)) {
		goto POUT42;
	}


	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	//Ｘ－１
	m_Arith.l_div(WORK0, m_pSnFhyo4_02Data->Sn_F402_1X, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	//Ｘ－１－１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_1_1X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_2X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_3X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_4X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_5X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－６
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_6X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－７
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_7X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－８
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_8X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－９
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_9X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１０
	PF1[i].PITM[j++] = (char *)0;

	//Ｘ－１１
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_11X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１２
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_12X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１３
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_13X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１４
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_14X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１５
	m_Arith.l_print(dp, m_pSnFhyo4_02Data->Sn_F402_15X, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ｘ－１６
	PF1[i].PITM[j++] = (char *)0;

	if (j = _xprn(PP, 4, PF1, 0, pDC, isNoImage, 1)) {
		goto POUT42;
	}

	if(AP){
		delete AP;
		AP = NULL;
	}


POUT42:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return j;
	return 0;
}


//=============================================================================
// 付表４－３出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo4_03( CDC *pDC, int kojin, int pcnt )
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp = NULL, PW0[MONY_BUF_SIZE] = { 0 };
	char			PW1[MONY_BUF_SIZE] = { 0 };

	BOOL isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	size = 1024;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}

	memset ( PW0, '\0', MONY_BUF_SIZE );
	m_Arith.l_input ( PW0, _T( "1000" ) );

	memset ( PW1, '\0', MONY_BUF_SIZE );
	m_Arith.l_input ( PW1, _T( "100" ) );

	char WORK0[MONY_BUF_SIZE] = { 0 };
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	PBUF PF1[30];
	int	PP = 0;

	PP = 22;

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;

	char yymmdd[3] = {0};
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT41;
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	// 1-A
	m_Arith.l_div ( WORK0, m_pSnFhyo4_01Data->Sn_F401_1D, PW0 );
	m_Arith.l_print ( dp, WORK0, FMT110_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "   " ) );
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 1-1-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_1_1D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_2D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 3-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_3D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_4D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_5D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_6D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 7-A
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_7D, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT41;
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	// 1-B
	m_Arith.l_div(WORK0, m_pSnFhyo4_01Data->Sn_F401_1E, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 1-1-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_1_1E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_2E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 3-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_3E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_4E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_5E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_6E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 7-B
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_7E, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ) {
		goto POUT41;
	}

	dp = AP;
	i = j = 0;
	memset ( dp, '\0', size );
	memset ( WORK0, '\0', MONY_BUF_SIZE );

	// 1-C
	m_Arith.l_div ( WORK0, m_pSnFhyo4_01Data->Sn_F401_1F, PW0 );
	m_Arith.l_print ( dp, WORK0, FMT110_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "   " ) );
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 1-1-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_1_1F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 2-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_2F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 3-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_3F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_4F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_5F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_6F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 7-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_7F, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 8-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_8F, FMT15_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	// 9-C
	m_Arith.l_div ( WORK0, m_pSnFhyo4_01Data->Sn_F401_9F, PW1 );
	m_Arith.l_print ( dp, WORK0, FMT120_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "  " ) );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	// 10-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_11F, FMT15_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	// 11-C
	m_Arith.l_div ( WORK0, m_pSnFhyo4_01Data->Sn_F401_12F, PW1 );
	m_Arith.l_print ( dp, WORK0, FMT120_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "  " ) );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	// 12-C
	m_Arith.l_print ( dp, m_pSnFhyo4_01Data->Sn_F401_14F, FMT15_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	// 13-C
	m_Arith.l_div ( WORK0, m_pSnFhyo4_01Data->Sn_F401_15F, PW1 );
	m_Arith.l_print ( dp, WORK0, FMT120_EX );
	strcat_s ( dp, ( 18 + 1 ), _T( "  " ) );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	j = 0;

	if ( ( j = _xprn (PP, 4, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT41;
	}

POUT41:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}


//-----------------------------------------------------------------------------
// 付表５－１出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo5_011( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp;

	short emfNo = 0;
	if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
		emfNo = MAKEWORD ( 2, 2 );
	}
	else {
		emfNo = MAKEWORD( 1, 1 );
	}

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	size = 512;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}
	memset(AP, '\0', (sizeof(char)*size));

	PBUF	PF1[30] = { 0 };
	int		PP;

	//PP = 33;
	PP = 25;

	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;

	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(dp, "%02X %02X %02X", yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
	PF1[i].PITM[0] = dp;
	dp += 9;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(dp, "%02X %02X %02X", yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_1X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_1D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_1E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-E


	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_2X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_2D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_2E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_3X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_3D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_3E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-E


	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_4X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_4D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_4E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-E


	i++;
	j = 0;
	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_1F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-F

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_2F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-F

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_3F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-F

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_4F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-F

	i++;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_5X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_5D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_5E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-E


	i++;
	j = 0;
	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_5F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-F

	//if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1))	goto POUT511;
	if ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT511;
	}


	i = 0, j = 0;
	dp = AP;
	memset(dp, '\0', size);
	pbclr(0, 2, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 15, &PF1[i + 2]);
	pbclr(0, 15, &PF1[i + 3]);
	pbclr(0, 15, &PF1[i + 4]);
	pbclr(0, 1, &PF1[i + 5]);

	if (m_pSnFhyo5_01Data->Sn_F501_5kbn) goto POUT511_2;

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8X) ||
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10X) || 
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12X) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6X, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6X, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_7X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_8X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_9X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_10X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_11X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_12X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-X

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8D) ||
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10D) || 
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12D) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6D, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6D, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_7D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_8D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_9D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_10D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_11D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_12D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-D

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8E) ||
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10E) || 
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6E, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6E, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_7E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_8E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_9E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_10E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_11E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_12E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-E

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_6X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_6D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_6E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_6F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_7F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_7F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_8F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_8F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_9F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_9F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_10F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_10F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_11F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_11F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_12F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_12F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-F

	i++;
	j = 0;

	if( m_pSnFhyo5_01Data->Sn_F501_UP1 ){	// 7-割合
		//wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP1 / 100, m_pSnFhyo5_01Data->Sn_F501_UP1 % 10 );
		wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP1 / 10, m_pSnFhyo5_01Data->Sn_F501_UP1 % 10 );	// 小数１桁
	}
	if( m_pSnFhyo5_01Data->Sn_F501_UP1 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//大きいときは2番目の値で印字
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if( m_pSnFhyo5_01Data->Sn_F501_UP2 ){	// 8-割合
		//wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP2 / 100, m_pSnFhyo5_01Data->Sn_F501_UP2 % 10 );
		wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP2 / 10, m_pSnFhyo5_01Data->Sn_F501_UP2 % 10 );	// 小数１桁
	}
	if( m_pSnFhyo5_01Data->Sn_F501_UP2 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//大きいときは2番目の値で印字
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if( m_pSnFhyo5_01Data->Sn_F501_UP3 ){	// 9-割合
		//wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP3 / 100, m_pSnFhyo5_01Data->Sn_F501_UP3 % 10 );
		wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP3 / 10, m_pSnFhyo5_01Data->Sn_F501_UP3 % 10 );	// 小数１桁
	}
	if( m_pSnFhyo5_01Data->Sn_F501_UP3 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//大きいときは2番目の値で印字
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if( m_pSnFhyo5_01Data->Sn_F501_UP4 ){	// 10-割合
		//wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP4 / 100, m_pSnFhyo5_01Data->Sn_F501_UP4 % 10 );
		wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP4 / 10, m_pSnFhyo5_01Data->Sn_F501_UP4 % 10 );	// 小数１桁
	}
	if( m_pSnFhyo5_01Data->Sn_F501_UP4 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//大きいときは2番目の値で印字
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if( m_pSnFhyo5_01Data->Sn_F501_UP5 ){	// 11-割合
		//wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP5 / 100, m_pSnFhyo5_01Data->Sn_F501_UP5 % 10 );
		wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP5 / 10, m_pSnFhyo5_01Data->Sn_F501_UP5 % 10 );	// 小数１桁
	}
	if( m_pSnFhyo5_01Data->Sn_F501_UP5 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//大きいときは2番目の値で印字
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if( m_pSnFhyo5_01Data->Sn_F501_UP6 ){	// 12-割合
		//wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP6 / 100, m_pSnFhyo5_01Data->Sn_F501_UP6 % 10 );
		wsprintf( dp, "%3d.%01d", m_pSnFhyo5_01Data->Sn_F501_UP6 / 10, m_pSnFhyo5_01Data->Sn_F501_UP6 % 10 );	// 小数１桁
	}
	if( m_pSnFhyo5_01Data->Sn_F501_UP6 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//大きいときは2番目の値で印字
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	//if (j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, 1))	goto POUT511;
	if ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT511;
	}


	i = 0, j = 0;
	dp = AP;
	memset(dp, '\0', size);
	pbclr(0, 2, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 15, &PF1[i + 2]);

	BOOL	isTotalZeroOut = FALSE;	// 13Dの0円を出力するかどうか

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_13X, FMT15);
	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16X) ||
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19X) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13X, FMT16 );
		isTotalZeroOut = TRUE;
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13X, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_14XP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_15XP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_16XP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_17XP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_18XP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-X

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_19XP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-X

	i++;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_13D, FMT15);
	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16D) ||
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19D) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13D, FMT16 );
		isTotalZeroOut = TRUE;
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13D, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_14DP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14D) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_15DP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15D) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_16DP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16D) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_17DP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17D) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_18DP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18D) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-D

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_19DP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19D) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-D

	i++;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_13E, FMT15);
	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16E) ||
		m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18E) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13E, FMT16 );
		isTotalZeroOut = TRUE;
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13E, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_14EP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14E) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_15EP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15E) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_16EP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16E) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_17EP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17E) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_18EP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18E) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-E

	m_Arith.l_print(dp, m_pSnFhyo5_01Data->Sn_F501_19EP, m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19E) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-E

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_13X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_13D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_13E) || isTotalZeroOut ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_14F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_14F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_15F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_15F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_16F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_16F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_17F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_17F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_18F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_18F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-F

	if( m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19E) ){
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_19F, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_19F, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-F

	//if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, 1))	goto POUT511;
	if ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, emfNo ) ) {
		goto POUT511;
	}


	i = 0, j = 0;

	int		Flg = 0;
	unsigned char	gyoFlg = 0x00;	// D0:第１種, D1:第２種, D2:第３種, D3:第４種, D4:第５種, D5:第６種
	if ((m_Arith.l_test( m_pSnFhyo5_01Data->Sn_F501_7X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7E)) ||
		m_Arith.l_test( m_pSnHonpyoData->Sn_KURI1)) {
		Flg++;
		gyoFlg |= 0x01;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8E)) ||
		m_Arith.l_test( m_pSnHonpyoData->Sn_KURI2)) {
		Flg++;
		gyoFlg |= 0x02;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9E)) ||
		m_Arith.l_test( m_pSnHonpyoData->Sn_KURI3)) {
		Flg++;
		gyoFlg |= 0x04;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10E)) ||
		m_Arith.l_test( m_pSnHonpyoData->Sn_KURI4)) {
		Flg++;
		gyoFlg |= 0x08;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11E)) ||
		m_Arith.l_test( m_pSnHonpyoData->Sn_KURI5)) {
		Flg++;
		gyoFlg |= 0x10;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12X) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12D) || m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12E)) ||
		m_Arith.l_test( m_pSnHonpyoData->Sn_KURI6)) {
		Flg++;
		gyoFlg |= 0x20;
	}


	/*if (Flg == 1) {

		// 単一事業者　○
		dp = AP;	i = j = 0;
		memset(dp, '\0', size);
		pbclr(0, 5, &PF1[0]);
		if (IsNeedExpCalq() == FALSE) {
			if (m_pSnHonpyoData->Sn_UP1 >= 1000) PF1[0].PITM[0] = "　";
			if (m_pSnHonpyoData->Sn_UP2 >= 1000) PF1[0].PITM[1] = "　";
			if (m_pSnHonpyoData->Sn_UP3 >= 1000) PF1[0].PITM[2] = "　";
			if (m_pSnHonpyoData->Sn_UP4 >= 1000) PF1[0].PITM[3] = "　";
			if (m_pSnHonpyoData->Sn_UP5 >= 1000) PF1[0].PITM[4] = "　";
			if (m_pSnHonpyoData->Sn_UP6 >= 1000) PF1[0].PITM[5] = "　";
		}
		else {
			if (gyoFlg & 0x01) {
				PF1[0].PITM[0] = "　";
			}
			else if (gyoFlg & 0x02) {
				PF1[0].PITM[1] = "　";
			}
			else if (gyoFlg & 0x04) {
				PF1[0].PITM[2] = "　";
			}
			else if (gyoFlg & 0x08) {
				PF1[0].PITM[3] = "　";
			}
			else if (gyoFlg & 0x10) {
				PF1[0].PITM[4] = "　";
			}
			else if (gyoFlg & 0x20) {
				PF1[0].PITM[5] = "　";
			}
		}
		//---------------

		m_pVprn->SetSfncEnv(SElfnc, 1);
		if (j = _xprn(PP, 4, PF1, 2, pDC, 0, 1))	goto POUT511;

		goto POUT511;		// 単一事業
	}
	*/

	if (AP) {
		delete AP;
		AP = NULL;
	}
	
POUT511_2:
	if (m_pSnFhyo5_01Data->Sn_F501_5kbn) {
		// 単一事業者　○
		dp = AP;
		memset(dp, '\0', size);
		pbclr(0, 5, &PF1[0]);

		PF1[0].PITM[m_pSnFhyo5_01Data->Sn_F501_5kbn - 1] = "　";

		m_pVprn->SetSfncEnv(SElfnc, 1);
		//if (j = _xprn(PP, 4, PF1, 2, pDC, 0, 1))	goto POUT511;
		if ( j = _xprn ( PP, 4, PF1, 2, pDC, 0, emfNo ) ) {
			goto POUT511;
		}

		goto POUT511;		// 単一事業
	}
	

POUT511:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//-----------------------------------------------------------------------------
// 付表５－１－２出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo5_012(CDC *pDC, int kojin, int pcnt)
{
	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp;
	bool printFlg = false;

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	if ( m_pSnHeadData->Sn_GenericSgn & 0x40 ) {
		// 1項目のみ出力
		printFlg = true;
	}

	//size = 512;
	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}
	memset(AP, '\0', (sizeof(char)*size));


	PBUF	PF1[30];
	int		PP;

	//PP = 34;
	PP = 26;

	dp = AP;
	memset(dp, '\0', size);
	i = 0;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_20X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-X

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_20D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-D

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_20E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-E

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_20F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-F

	i++;
	j = 0;

//	if (IsOneOver750) {
	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-X

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-D

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-E

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-F*/

	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_21sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-X

			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-D

			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-E

			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-F
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
			PF1[i].PITM[j++] = ( char* )0;
			PF1[i].PITM[j++] = ( char* )0;
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21X, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-X

		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21D, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-D

		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21E, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-E

		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_21F, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-F
	}
//	}

	if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1))	goto POUT512;

	i = j = 0;

	dp = AP;
	memset(dp, '\0', size);

	f501_calq_tk_for_disp();
	int Sign = 0;

	/*for (int tokurei_cnt = 0, chkflg = 0x04; tokurei_cnt < 15; tokurei_cnt++, chkflg <<= 1) {
		if (m_dspSkjSw&chkflg) {
			break;
		}
		Sign++;
	}

	for (int cnt = 0; cnt < Sign; cnt++) {
		PF1[i].PITM[j++] = (char *)0;
	}

	if (Sign >= 0) {
		switch (Sign) {
		case 0:

			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			

			break;
		case 1:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			break;
		case 2:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 3:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 4:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 5:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 6:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 7:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 8:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 9:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 10:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 11:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 12:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 13:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 14:
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

				break;
		}
	}

	for (int cnt = Sign + 1; cnt < 15; cnt++) {
		PF1[i].PITM[j++] = (char *)0;
	}*/

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;			
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;			
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;			
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_22sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;			
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;			
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;			
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_23sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_24sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_25sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_26sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_27sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_28sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_29sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_30sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_31sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_32sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_33sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_34sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_35sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36X, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_36sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36X, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36X, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	
	//if (j = _xprn(PP, 3, PF1, 0, pDC, 0, 1))	goto POUT512;
	if (j = _xprn(PP, 2, PF1, 0, pDC, 0, 1))	goto POUT512;

	dp = AP;	i = j = 0;
	memset(dp, '\0', size);

	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_37X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_37D, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_37E, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_37F, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;

	//if (j = _xprn(PP, 4, PF1, 0, pDC, 0, 1))	goto POUT512;
	if (j = _xprn(PP, 3, PF1, 0, pDC, 0, 1))	goto POUT512;


//	if (IsOneOver750) {
		dp = AP;	i = j = 0;
		memset(dp, '\0', size);
		pbclr(0, 5, &PF1[i]);

		/*if (m_pSnHonpyoData->Sn_UP1 >= 750) {
			PF1[0].PITM[0] = "　";
		}
		else if (m_pSnHonpyoData->Sn_UP2 >= 750) {
			PF1[0].PITM[1] = "　";
		}
		else if (m_pSnHonpyoData->Sn_UP3 >= 750) {
			PF1[0].PITM[2] = "　";
		}
		else if (m_pSnHonpyoData->Sn_UP4 >= 750) {
			PF1[0].PITM[3] = "　";
		}
		else if (m_pSnHonpyoData->Sn_UP5 >= 750) {
			PF1[0].PITM[4] = "　";
		}
		else if (m_pSnHonpyoData->Sn_UP6 >= 750) {
			PF1[0].PITM[5] = "　";
		}*/

		//if (m_pSnFhyo5_012Data->Sn_F5012_21kbn) {
		//	PF1[0].PITM[m_pSnFhyo5_012Data->Sn_F5012_21kbn-1] = "　";
		//}
		if ( printFlg ) {
			if ( m_pSnFhyo5_012Data->Sn_F5012_21sw & 0x01 ) {
				if (m_pSnFhyo5_012Data->Sn_F5012_21kbn) {
					PF1[0].PITM[m_pSnFhyo5_012Data->Sn_F5012_21kbn-1] = "　";
				}
			}
		}
		else {
			if (m_pSnFhyo5_012Data->Sn_F5012_21kbn) {
				PF1[0].PITM[m_pSnFhyo5_012Data->Sn_F5012_21kbn-1] = "　";
			}
		}

		//フレーム種を有効とする為の前処理
		m_pVprn->SetSfncEnv(SElfnc, 1);
		if (j = _xprn(PP, 5, PF1, 0, pDC, 0, 1))	goto POUT512;
//	}

	if(AP){
		delete AP;
		AP = NULL;
	}
	
POUT512:
	if (AP) {
		delete AP;
		AP = NULL;
	}
	
	return j;
}


//-----------------------------------------------------------------------------
// 付表５－２出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo5_021( CDC *pDC, int kojin, int pcnt )
{

	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp;

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	size = 512;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}
	memset(AP, '\0', (sizeof(char)*size));

	PBUF	PF1[30] = { 0 };
	int		PP;

	//PP = 35;
	PP = 27;

	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;

	char	yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(dp, "%02X %02X %02X", yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
	PF1[i].PITM[0] = dp;
	dp += 9;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(dp, "%02X %02X %02X", yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_1A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_1B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_1C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_2A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_2B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_2C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-C


	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_3A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_3B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_3C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_4A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_4B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_4C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-C

	i++;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_1X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_2X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_3X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_4X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4-X

	i++;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_5A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_5B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-D

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_5C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-E

	i++;
	j = 0;
	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_5X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 5-F

	if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1))	goto POUT521;


	i = 0, j = 0;
	dp = AP;
	memset(dp, '\0', size);
	pbclr(0, 2, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 15, &PF1[i + 2]);
	pbclr(0, 15, &PF1[i + 3]);
	pbclr(0, 15, &PF1[i + 4]);
	pbclr(0, 15, &PF1[i + 5]);

	if (m_pSnFhyo5_02Data->Sn_F502_5kbn) goto POUT521_2;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8A) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10A) || 
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12A) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6A, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6A, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_7A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_8A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_9A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_10A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_11A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_12A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-A

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8B) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10B) || 
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12B) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6B, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6B, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_7B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_8B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_9B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_10B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_11B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_12B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-B

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8C) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10C) || 
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12C) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6C, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6C, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_7C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_8C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_9C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_10C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_11C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_12C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-C

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8X) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10X) || 
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12X) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6X, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_6X, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_7X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_8X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_9X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_10X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_11X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_12X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 12-X

	if (j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, 1))	goto POUT521;


	i = 0, j = 0;
	dp = AP;
	memset(dp, '\0', size);
	pbclr(0, 2, &PF1[i]);
	pbclr(0, 1, &PF1[i + 1]);
	pbclr(0, 15, &PF1[i + 2]);
	pbclr(0, 15, &PF1[i + 3]);

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16A) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19A) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13A, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13A, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_14AP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14A) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_15AP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15A) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_16AP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16A) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_17AP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17A) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_18AP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18A) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-A

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_19AP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19A) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-A

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16B) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19B) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13B, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13B, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_14BP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14B) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_15BP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15B) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_16BP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16B) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_17BP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17B) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_18BP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18B) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-B

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_19BP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19B) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-B

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16C) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18C) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19C) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13C, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13C, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_14CP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14C) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_15CP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15C) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_16CP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16C) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_17CP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17C) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_18CP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18C) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-C

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_19CP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19C) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-C

	i++;
	j = 0;

	if( m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16X) ||
		m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18X) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19X) ){
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13X, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo5_02Data->Sn_F502_13X, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_14XP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_15XP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_16XP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_17XP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_18XP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18-X

	m_Arith.l_print(dp, m_pSnFhyo5_02Data->Sn_F502_19XP, m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19X) >= 0 ? FMT15 : FMT16);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 19-X

	if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, 1))	goto POUT521;


	i = 0, j = 0;

	int		Flg = 0;
	unsigned char	gyoFlg = 0x00;	// D0:第１種, D1:第２種, D2:第３種, D3:第４種, D4:第５種, D5:第６種
	if ((m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_7C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1)) {
		Flg++;
		gyoFlg |= 0x01;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_8C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2)) {
		Flg++;
		gyoFlg |= 0x02;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_9C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3)) {
		Flg++;
		gyoFlg |= 0x04;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_10C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4)) {
		Flg++;
		gyoFlg |= 0x08;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_11C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5)) {
		Flg++;
		gyoFlg |= 0x10;
	}
	if ((m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12A) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12B) || m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_12C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6)) {
		Flg++;
		gyoFlg |= 0x20;
	}

	/*if (Flg == 1) {

		// 単一事業者　○
		dp = AP;	i = j = 0;
		memset(dp, '\0', size);
		pbclr(0, 5, &PF1[0]);
		if (IsNeedExpCalq() == FALSE) {
			if (m_pSnHonpyoData->Sn_UP1 >= 1000) PF1[0].PITM[0] = "　";
			if (m_pSnHonpyoData->Sn_UP2 >= 1000) PF1[0].PITM[1] = "　";
			if (m_pSnHonpyoData->Sn_UP3 >= 1000) PF1[0].PITM[2] = "　";
			if (m_pSnHonpyoData->Sn_UP4 >= 1000) PF1[0].PITM[3] = "　";
			if (m_pSnHonpyoData->Sn_UP5 >= 1000) PF1[0].PITM[4] = "　";
			if (m_pSnHonpyoData->Sn_UP6 >= 1000) PF1[0].PITM[5] = "　";
		}
		else {
			if (gyoFlg & 0x01) {
				PF1[0].PITM[0] = "　";
			}
			else if (gyoFlg & 0x02) {
				PF1[0].PITM[1] = "　";
			}
			else if (gyoFlg & 0x04) {
				PF1[0].PITM[2] = "　";
			}
			else if (gyoFlg & 0x08) {
				PF1[0].PITM[3] = "　";
			}
			else if (gyoFlg & 0x10) {
				PF1[0].PITM[4] = "　";
			}
			else if (gyoFlg & 0x20) {
				PF1[0].PITM[5] = "　";
			}
		}
		//---------------

		m_pVprn->SetSfncEnv(SElfnc, 1);
		if (j = _xprn(PP, 4, PF1, 2, pDC, 0, 1))	goto POUT521;

		goto POUT521;		// 単一事業
	}*/

	if(AP){
		delete AP;
		AP = NULL;
	}

POUT521_2:
	if (m_pSnFhyo5_02Data->Sn_F502_5kbn) {
		// 単一事業者　○
		dp = AP;
		memset(dp, '\0', size);
		pbclr(0, 5, &PF1[0]);

		PF1[0].PITM[m_pSnFhyo5_02Data->Sn_F502_5kbn - 1] = "　";

		m_pVprn->SetSfncEnv(SElfnc, 1);
		if (j = _xprn(PP, 4, PF1, 2, pDC, 0, 1))	goto POUT521;

		goto POUT521;		// 単一事業
	}


POUT521:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//-----------------------------------------------------------------------------
// 付表５－２出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo5_022(CDC *pDC, int kojin, int pcnt)
{

	int				i, j;
	unsigned int	size;
	char			*AP = NULL;
	char			*dp;
	bool printFlg = false;

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	if ( m_pSnHeadData->Sn_GenericSgn & 0x40 ) {
		// 1項目のみ出力
		printFlg = true;
	}

	//size = 512;
	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}
	memset(AP, '\0', (sizeof(char)*size));


	PBUF	PF1[30];
	int		PP;

	//PP = 36;
	PP = 28;

	dp = AP;
	memset(dp, '\0', size);
	i = 0;
	j = 0;

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_20A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-X

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_20B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-D

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_20C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-E

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_20X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-F

	i++;
	j = 0;

	//	if (IsOneOver750) {
	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-X

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-D

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-E

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;			// 21-F
	*/

	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_21sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-X

			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-D

			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-E

			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;			// 21-F
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
			PF1[i].PITM[j++] = ( char* )0;
			PF1[i].PITM[j++] = ( char* )0;
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21A, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-X

		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21B, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-D

		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21C, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-E

		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_21X, FMT15);
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-F
	}
//	}

	if (j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1))	goto POUT522;

	i = j = 0;

	dp = AP;
	memset(dp, '\0', size);

	f501_calq_tk_for_disp();
	int Sign = 0;

	/*for (int tokurei_cnt = 0, chkflg = 0x04; tokurei_cnt < 15; tokurei_cnt++, chkflg <<= 1) {
		if (m_dspSkjSw&chkflg) {
			break;
		}
		Sign++;
	}

	for (int cnt = 0; cnt < Sign; cnt++) {
		PF1[i].PITM[j++] = (char *)0;
	}

	if (Sign >= 0) {
		switch (Sign) {
		case 0:

			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 1:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			break;
		case 2:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 3:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 4:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 5:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 6:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 7:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 8:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 9:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 10:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 11:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 12:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 13:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		case 14:
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36A, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36B, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36C, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36X, FMT15);
			PF1[i].PITM[j++] = dp;	dp += 16;

			break;
		}
	}

	for (int cnt = Sign + 1; cnt < 15; cnt++) {
		PF1[i].PITM[j++] = (char *)0;
	}*/

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;			
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;			
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;			
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_22sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;			
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;			
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;			
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;			
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_22X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_23sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_23X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_24sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_24X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_25sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_25X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_26sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_26X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_27sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_27X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_28sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_28X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_29sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_29X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_30sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_30X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_31sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_31X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_32sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_32X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_33sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_33X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_34sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_34X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_35sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_35X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}
	j++;

	/*m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36A, FMT15);
	PF1[0].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36B, FMT15);
	PF1[1].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36C, FMT15);
	PF1[2].PITM[j] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36X, FMT15);
	PF1[3].PITM[j] = dp;	dp += 16;*/
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_36sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36A, FMT15);
			PF1[0].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36B, FMT15);
			PF1[1].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36C, FMT15);
			PF1[2].PITM[j] = dp;	dp += 16;
			m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36X, FMT15);
			PF1[3].PITM[j] = dp;	dp += 16;
		}
		else {
			PF1[0].PITM[j] = ( char* )0;
			PF1[1].PITM[j] = ( char* )0;
			PF1[2].PITM[j] = ( char* )0;
			PF1[3].PITM[j] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36A, FMT15);
		PF1[0].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36B, FMT15);
		PF1[1].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36C, FMT15);
		PF1[2].PITM[j] = dp;	dp += 16;
		m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_36X, FMT15);
		PF1[3].PITM[j] = dp;	dp += 16;
	}

	//if (j = _xprn(PP, 3, PF1, 0, pDC, 0, 1))	goto POUT522;
	if (j = _xprn(PP, 2, PF1, 0, pDC, 0, 1))	goto POUT522;

	dp = AP;	i = j = 0;
	memset(dp, '\0', size);

	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_37A, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_37B, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_37C, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;
	m_Arith.l_print(dp, m_pSnFhyo5_022Data->Sn_F5022_37X, FMT15);
	PF1[i].PITM[j++] = dp;	dp += 16;

	//if (j = _xprn(PP, 4, PF1, 0, pDC, 0, 1))	goto POUT522;
	if (j = _xprn(PP, 3, PF1, 0, pDC, 0, 1))	goto POUT522;


	//	if (IsOneOver750) {
	dp = AP;	i = j = 0;
	memset(dp, '\0', size);
	pbclr(0, 5, &PF1[i]);

	/*if (m_pSnHonpyoData->Sn_UP1 >= 750) {
		PF1[0].PITM[0] = "　";
	}
	else if (m_pSnHonpyoData->Sn_UP2 >= 750) {
		PF1[0].PITM[1] = "　";
	}
	else if (m_pSnHonpyoData->Sn_UP3 >= 750) {
		PF1[0].PITM[2] = "　";
	}
	else if (m_pSnHonpyoData->Sn_UP4 >= 750) {
		PF1[0].PITM[3] = "　";
	}
	else if (m_pSnHonpyoData->Sn_UP5 >= 750) {
		PF1[0].PITM[4] = "　";
	}
	else if (m_pSnHonpyoData->Sn_UP6 >= 750) {
		PF1[0].PITM[5] = "　";
	}*/

	//if (m_pSnFhyo5_022Data->Sn_F5022_21kbn) {
	//	PF1[0].PITM[m_pSnFhyo5_022Data->Sn_F5022_21kbn-1] = "　";
	//}
	if ( printFlg ) {
		if ( m_pSnFhyo5_022Data->Sn_F5022_21sw & 0x01 ) {
			if (m_pSnFhyo5_022Data->Sn_F5022_21kbn) {
				PF1[0].PITM[m_pSnFhyo5_022Data->Sn_F5022_21kbn-1] = "　";
			}
		}
	}
	else {
		if (m_pSnFhyo5_022Data->Sn_F5022_21kbn) {
			PF1[0].PITM[m_pSnFhyo5_022Data->Sn_F5022_21kbn-1] = "　";
		}
	}

	//フレーム種を有効とする為の前処理
	m_pVprn->SetSfncEnv(SElfnc, 1);
	if (j = _xprn(PP, 5, PF1, 0, pDC, 0, 1))	goto POUT522;
	//	}

	if (AP) {
		delete AP;
		AP = NULL;
	}

POUT522:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return j;
}



//=============================================================================
// 付表５－３出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo5_031( CDC *pDC, int kojin, int pcnt )
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp = NULL;

	BOOL isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	size = 512;
	if ( ( AP = new char[size] ) == NULL ) {
		return -1;
	}
	memset ( AP, '\0', ( sizeof ( char )*size ) );

	PBUF PF1[30];
	int	PP = 0;
	PP = 30;

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;

	char yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset ( yymmdd, '\0', sizeof ( yymmdd ) );
	m_pSnHeadData->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	wsprintf ( dp, _T( "%02X %02X %02X" ), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff );
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if ( kojin ) {	// 個人事業者の場合は代表者氏名
		j = sizeof ( m_pSnHeadData->Sn_DIHYO );
		memmove ( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// 法人名
		j = sizeof ( m_pSnHeadData->Sn_CONAM );
		memmove ( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[i++].PITM[0] = dp;
	dp += ( 40 + 1 );

	j = 0;

	// 1-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_1D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	
	// 1-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_1E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 2-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_2D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 2-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_2E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_3D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_3E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	
	// 4-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_4D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 4-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_4E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	// 1-C
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_1F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 2-C
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_2F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 3-C
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_3F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 4-C
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_4F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	// 5-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_5D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 5-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_5E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;
	
	// 5-C
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_5F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT531;
	}

	i = 0, j = 0;
	dp = AP;
	memset ( dp, '\0', size );
	pbclr ( 0, 2,  &PF1[i] );
	pbclr ( 0, 1,  &PF1[i + 1] );
	pbclr ( 0, 15, &PF1[i + 2] );
	pbclr ( 0, 15, &PF1[i + 3] );
	pbclr ( 0, 15, &PF1[i + 4] );
	pbclr ( 0, 1,  &PF1[i + 5] );

	if ( m_pSnFhyo5_01Data->Sn_F501_5kbn ) {
		goto POUT531_2;
	}

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_7D ) )  || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_8D ) )  ||
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_9D ) )  || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_10D ) ) || 
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_11D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_12D ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_6D, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_6D, FMT15 );
	}

	// 6-A
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 7-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_7D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 8-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_8D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_9D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_10D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_11D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_12D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_7E ) )  || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_8E ) )  ||
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_9E ) )  || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_10E ) ) || 
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_11E ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_12E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_6E, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_6E, FMT15 );
	}

	// 6-B
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 7-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_7E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 8-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_8E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 9-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_9E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 10-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_10E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 11-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_11E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 12-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_12E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_6D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_6E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_6F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_6F, FMT15 );
	}

	// 6-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_7D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_7E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_7F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_7F, FMT15 );
	}

	// 7-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_8D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_8E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_8F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_8F, FMT15 );
	}

	// 8-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_9D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_9E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_9F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_9F, FMT15 );
	}

	// 9-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_10D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_10E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_10F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_10F, FMT15 );
	}

	// 10-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_11D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_11E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_11F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_11F, FMT15 );
	}

	// 11-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_12D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_12E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_12F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_12F, FMT15 );
	}

	// 12-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	// 7-割合
	if ( m_pSnFhyo5_01Data->Sn_F501_UP1 ) {
		wsprintf ( dp, _T( "%3d.%01d" ), ( m_pSnFhyo5_01Data->Sn_F501_UP1 / 10 ), ( m_pSnFhyo5_01Data->Sn_F501_UP1 % 10 ) );
	}

	//大きいときは2番目の値で印字
	if ( m_pSnFhyo5_01Data->Sn_F501_UP1 >= 1000 ) {
		PF1[i+1].PITM[j++] = dp;
	}
	else {
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	// 8-割合
	if ( m_pSnFhyo5_01Data->Sn_F501_UP2 ) { 
		wsprintf ( dp, _T( "%3d.%01d" ), ( m_pSnFhyo5_01Data->Sn_F501_UP2 / 10 ), ( m_pSnFhyo5_01Data->Sn_F501_UP2 % 10 ) );
	}
	if ( m_pSnFhyo5_01Data->Sn_F501_UP2 >= 1000 ) {
		PF1[i+1].PITM[j++] = dp;
	}
	else {
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	// 9-割合
	if ( m_pSnFhyo5_01Data->Sn_F501_UP3 ) {
		wsprintf ( dp, _T( "%3d.%01d" ), ( m_pSnFhyo5_01Data->Sn_F501_UP3 / 10 ), ( m_pSnFhyo5_01Data->Sn_F501_UP3 % 10 ) );
	}
	if ( m_pSnFhyo5_01Data->Sn_F501_UP3 >= 1000 ) {
		PF1[i+1].PITM[j++] = dp;
	}
	else {
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	// 10-割合
	if ( m_pSnFhyo5_01Data->Sn_F501_UP4 ) {
		wsprintf ( dp, _T( "%3d.%01d" ), ( m_pSnFhyo5_01Data->Sn_F501_UP4 / 10 ), ( m_pSnFhyo5_01Data->Sn_F501_UP4 % 10 ) );
	}
	if ( m_pSnFhyo5_01Data->Sn_F501_UP4 >= 1000 ) {
		PF1[i+1].PITM[j++] = dp;
	}
	else {
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	// 11-割合
	if ( m_pSnFhyo5_01Data->Sn_F501_UP5 ) {
		wsprintf ( dp, _T( "%3d.%01d" ), ( m_pSnFhyo5_01Data->Sn_F501_UP5 / 10 ), ( m_pSnFhyo5_01Data->Sn_F501_UP5 % 10 ) );
	}
	if ( m_pSnFhyo5_01Data->Sn_F501_UP5 >= 1000 ) {
		PF1[i+1].PITM[j++] = dp;
	}
	else {
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if ( m_pSnFhyo5_01Data->Sn_F501_UP6 ) {
		wsprintf ( dp, _T( "%3d.%01d" ), ( m_pSnFhyo5_01Data->Sn_F501_UP6 / 10 ), ( m_pSnFhyo5_01Data->Sn_F501_UP6 % 10 ) );
	}
	if ( m_pSnFhyo5_01Data->Sn_F501_UP6 >= 1000 ) {
		PF1[i+1].PITM[j++] = dp;
	}
	else {
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;

	if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT531;
	}

	i = 0, j = 0;
	dp = AP;
	memset ( dp, '\0', size );
	pbclr ( 0, 2, &PF1[i] );
	pbclr ( 0, 1, &PF1[i + 1] );
	pbclr ( 0, 15, &PF1[i + 2] );

	BOOL isTotalZeroOut = FALSE;	// 13Dの0円を出力するかどうか

	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13D, FMT15 );
	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_14D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_15D ) ) ||
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_16D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_17D ) ) ||
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_18D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_19D ) ) ) { 
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13D, FMT16 );
		isTotalZeroOut = TRUE;
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13D, FMT15 );
	}

	// 13-A
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_14DP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_14D ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_15DP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_15D ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_16DP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_16D ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_17DP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_17D ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_18DP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_18D ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-A
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_19DP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_19D ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13E, FMT15);
	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_14E ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_15E ) ) ||
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_16E ) ) || (	m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_17E ) ) || 
		 ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_18E ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_19E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13E, FMT16 );
		isTotalZeroOut = TRUE;
	}
	else {
		m_Arith.l_print( dp, m_pSnFhyo5_01Data->Sn_F501_13E, FMT15 );
	}
	
	// 13-B
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 14-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_14EP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_14E ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 15-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_15EP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_15E ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 16-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_16EP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_16E ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 17-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_17EP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_17E ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 18-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_18EP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_18E ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 19-B
	m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_19EP, m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_19E ) >= 0 ? FMT15 : FMT16 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_13D ) ) || ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_13E ) ) || ( isTotalZeroOut ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_13F, FMT15 );
	}

	// 13-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_14D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_14E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_14F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_14F, FMT15 );
	}

	// 14-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_15D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_15E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_15F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_15F, FMT15 );
	}

	// 15-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_16D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_16E ) ) ){
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_16F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_16F, FMT15 );
	}

	// 16-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_17D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_17E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_17F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_17F, FMT15 );
	}

	// 17-C
	PF1[i].PITM[j++] = dp;
	dp += 16;	

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_18D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_18E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_18F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_18F, FMT15 );
	}

	// 18-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_19D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_19E ) ) ) {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_19F, FMT16 );
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_01Data->Sn_F501_19F, FMT15 );
	}

	// 19-C
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT531;
	}

	i = 0, j = 0;

	int	 Flg = 0;
	unsigned char	gyoFlg = 0x00;	// D0:第１種, D1:第２種, D2:第３種, D3:第４種, D4:第５種, D5:第６種
	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_7D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_7E ) ) ||
		 ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI1 ) ) ) {
		Flg++;
		gyoFlg |= 0x01;
	}

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_8D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_8E ) ) ||
		 ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI2 ) ) ) {
		Flg++;
		gyoFlg |= 0x02;
	}

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_9D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_9E ) ) ||
		 ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI3 ) ) ) {
		Flg++;
		gyoFlg |= 0x04;
	}

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_10D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_10E ) ) ||
		 ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI4 ) ) ) {
		Flg++;
		gyoFlg |= 0x08;
	}

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_11D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_11E ) ) ||
		 ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI5 ) ) ) {
		Flg++;
		gyoFlg |= 0x10;
	}

	if ( ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_12D ) ) || ( m_Arith.l_test ( m_pSnFhyo5_01Data->Sn_F501_12E ) ) ||
		 ( m_Arith.l_test ( m_pSnHonpyoData->Sn_KURI6 ) ) ) {
		Flg++;
		gyoFlg |= 0x20;
	}


POUT531_2:
	if ( m_pSnFhyo5_01Data->Sn_F501_5kbn ) {
		// 単一事業者　○
		dp = AP;
		memset ( dp, '\0', size );
		pbclr ( 0, 6, &PF1[0] );

		PF1[0].PITM[m_pSnFhyo5_01Data->Sn_F501_5kbn - 1] = _T( "　" );

		m_pVprn->SetSfncEnv ( SElfnc, 1 );
		if ( ( j = _xprn ( PP, 4, PF1, 2, pDC, 0, 1 ) ) ) {
			goto POUT531;
		}
		goto POUT531;		// 単一事業
	}


POUT531:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//=============================================================================
// 付表５－３－２出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo5_032(CDC *pDC, int kojin, int pcnt)
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp = NULL;
	bool			printFlg = false;

	BOOL isNoImage = FALSE;
	if ( m_pPrintCmInfo->PRtype & 0x80 ) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	if ( m_pSnHeadData->Sn_GenericSgn & 0x40 ) {
		// 1項目のみ出力
		printFlg = true;
	}


	size = 1024;
	if ( ( AP = new char[size] ) == NULL) {
		return -1;
	}
	memset ( AP, '\0', ( sizeof ( char )*size ) );


	PBUF PF1[30];
	int	PP = 0;
	PP = 31;

	dp = AP;
	memset ( dp, '\0', size );
	i = 0;

	// 20-A
	m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_20D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-B
	m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_20E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 20-C
	m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_20F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	i++;
	j = 0;

	// 21-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_21sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}

	dp += 16;	

	// 21-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_21sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 21-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_21sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_21F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	if ( ( j = _xprn ( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ) ) {
		goto POUT532;
	}

	i = j = 0;
	dp = AP;
	memset ( dp, '\0', size );

	f501_calq_tk_for_disp();
	int Sign = 0;
	
	// 22-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_22sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 23-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_23sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 24-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_24sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 25-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_25sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 26-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_26sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 27-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_27sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 28-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_28sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 29-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_29sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 30-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_30sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 31-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_31sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 32-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_32sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 33-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_33sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 34-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_34sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 35-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_35sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 36-A
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_36sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_36D, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;
	i++;
	j = 0;

	// 22-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_22sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 23-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_23sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 24-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_24sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 25-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_25sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 26-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_26sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 27-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_27sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 28-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_28sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 29-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_29sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 30-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_30sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 31-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_31sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 32-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_32sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 33-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_33sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 34-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_34sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 35-B
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_35sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35E, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 36-B
	//m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_36sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36E, FMT15);
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;
	i++;
	j = 0;

	// 22-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_22sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_22F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 23-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_23sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_23F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 24-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_24sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_24F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 25-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_25sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_25F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 26-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_26sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_26F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 27-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_27sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_27F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 28-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_28sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_28F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 29-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_29sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_29F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 30-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_30sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_30F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 31-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_31sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_31F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 32-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_32sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_32F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 33-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_33sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_33F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 34-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_34sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_34F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 35-C
	//m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15 );
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_35sw & 0x01 ) {
			m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15 );
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_35F, FMT15 );
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	// 36-C
	//m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
	//PF1[i].PITM[j++] = dp;
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_36sw & 0x01 ) {
			m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
			PF1[i].PITM[j++] = dp;
		}
		else {
			PF1[i].PITM[j++] = ( char* )0;
		}
	}
	else {
		m_Arith.l_print(dp, m_pSnFhyo5_012Data->Sn_F5012_36F, FMT15);
		PF1[i].PITM[j++] = dp;
	}
	dp += 16;

	if ( ( j = _xprn ( PP, 2, PF1, 0, pDC, 0, 1 ) ) ) {
		goto POUT532;
	}

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;

	// 37-A
	m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_37D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 37-B
	m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_37E, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	// 37-C
	m_Arith.l_print ( dp, m_pSnFhyo5_012Data->Sn_F5012_37F, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	if ( ( j = _xprn ( PP, 3, PF1, 0, pDC, 0, 1 ) ) ) {
		goto POUT532;
	}

	dp = AP;
	memset ( dp, '\0', size );
	i = j = 0;
	pbclr ( 0, 5, &PF1[i] );


	//if ( m_pSnFhyo5_012Data->Sn_F5012_21kbn ) {
	//	PF1[0].PITM[m_pSnFhyo5_012Data->Sn_F5012_21kbn-1] = _T( "　" );
	//}
	if ( printFlg ) {
		if ( m_pSnFhyo5_012Data->Sn_F5012_21sw & 0x01 ) {
			if ( m_pSnFhyo5_012Data->Sn_F5012_21kbn ) {
				PF1[0].PITM[m_pSnFhyo5_012Data->Sn_F5012_21kbn-1] = _T( "　" );
			}
		}
	}
	else {
		if ( m_pSnFhyo5_012Data->Sn_F5012_21kbn ) {
			PF1[0].PITM[m_pSnFhyo5_012Data->Sn_F5012_21kbn-1] = _T( "　" );
		}
	}

	//フレーム種を有効とする為の前処理
	m_pVprn->SetSfncEnv ( SElfnc, 1 );
	if ( ( j = _xprn ( PP, 4, PF1, 0, pDC, 0, 1 ) ) ) {
		goto POUT532;
	}

POUT532:
	if ( AP ) {
		delete AP;
		AP = NULL;
	}

	return j;
}

//-----------------------------------------------------------------------------
// 付表６出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo6( CDC *pDC, int pcnt )
{
	int				i, j;
	char			*AP = NULL;
	char			*dp, buf[512] = {0};
	char tmpbuf[512] = { 0 };
	int ret = 0;
	int len = 0;

	CString str = _T("");
	unsigned short	SEno = 1;
	BOOL			isNoImage = FALSE;

	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	unsigned int	size=2048;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	dp = AP;
	memset( dp, '\0', size );
	i=j=0;

	PBUF	PF1[30]={0};
	//int		PP = 9;
	int		PP = 35;

	char	PW0[6], PW1[6];
	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_Arith.l_input( PW1, _T("100") );


	//本表データ計算
	//ARITH_VAL F60val[6];
	unsigned char F60val[6][MONY_BUF_SIZE];
	if (!m_pFhyo6) {
		return 1;
	}
	m_pFhyo6->f6calq_1to6();
	memmove(F60val[0], m_pFhyo6->F61, MONY_BUF_SIZE);
	memmove(F60val[1], m_pFhyo6->F62, MONY_BUF_SIZE);
	memmove(F60val[2], m_pFhyo6->F63, MONY_BUF_SIZE);
	memmove(F60val[3], m_pFhyo6->F64, MONY_BUF_SIZE);
	memmove(F60val[4], m_pFhyo6->F65, MONY_BUF_SIZE);
	memmove(F60val[5], m_pFhyo6->F66, MONY_BUF_SIZE);

	BOOL IsNofu = FALSE,IsKanpu = FALSE;
	if( m_Arith.l_test( F60val[0] ) != 0 ){
		IsNofu = TRUE;
	}
	if( m_Arith.l_test( F60val[3] ) != 0 ){
		IsKanpu = TRUE;
	}

	// 申告期間自
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[3] = {0};
	memset( yymmdd, '\0', sizeof(yymmdd) );
	m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1 );
	wsprintf( dp, _T("%2X   %2X   %2X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
	PF1[i].PITM[0] = dp;
	dp += 13;

	// 申告期間至
	memset( yymmdd, '\0', sizeof(yymmdd) );
	m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2 );
	wsprintf( dp, _T("%2X   %2X   %2X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
	PF1[i++].PITM[1] = dp;
	dp += 13;

	if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
		memmove ( dp, "    ", 4 );
		PF1[i].PITM[0] = ( char* )0;
		PF1[i].PITM[1] = dp;
		dp += 5;
	}
	else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
		memmove ( dp, "    ", 4 );
		PF1[i].PITM[0] = dp;
		PF1[i].PITM[1] = ( char* )0;
		dp += 5;
	}
	else {
		PF1[i].PITM[0] = (char* )0;
		PF1[i].PITM[1] = (char* )0;
	}
	i++;

	if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
		memmove ( dp, "    ", 4 );
		PF1[i].PITM[0] = ( char* )0;
		PF1[i].PITM[1] = dp;
		dp += 5;
	}
	else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
		memmove ( dp, "    ", 4 );
		PF1[i].PITM[0] = dp;
		PF1[i].PITM[1] = ( char* )0;
		dp += 5;
	}
	else {
		PF1[i].PITM[0] = dp;
		PF1[i].PITM[1] = ( char*) 0;
	}
	i++;

	// 整理番号
	if( (m_pSnHeadData->Sn_SEIRI[0]&0xff) != 0xff ){
		memset( buf, '\0', sizeof( buf ) );
		m_Arith.l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
		int k;
		for( k=(int)strlen(buf); k>0; ){
			if( buf[--k] != 0x20 ){
				break;
			}
		}
		++k;
		if( k < 8 ){
			memmove( dp, "        ", 8-k );
		}
		memmove( (dp+(8-k)), buf, k );				
		PF1[i++].PITM[0] = dp;
		dp += 9;
	}
	else{
		PF1[i++].PITM[0] = (char *)0;
	}

	if( pcnt == 1 ){
		// 納税地（２行）
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[0],  20*2 );				
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[40], 20*2 );
		PF1[i++].PITM[1] = dp;
		dp += 41;

		// 代表者（フリガナ）
		// 付表６では25文字で表示しているため、印刷も同様にする
		//memmove( dp, m_pSnHeadData->Sn_DIFRI, 50 );
		//int len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIFRI, sizeof(m_pSnHeadData->Sn_DIFRI) );
		//len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIFRI, sizeof(m_pSnHeadData->Sn_DIFRI) );
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, m_pSnHeadData->Sn_DIFRI, 50 );
		memmove ( dp, buf, 50 );
		len = m_Util.MixSpaceCutLength ( buf, 50 );
		if( len <= 56 ){
			//PF1[i].PITM[0] = dp;
			PF1[i++].PITM[0] = dp;
			PF1[i++].PITM[0] = ( char* )0;
		}else{
			//PF1[i+1].PITM[0] = dp;
			PF1[i++].PITM[0] = ( char* )0;
			PF1[i++].PITM[0] = dp;
		}
		//i=i+2;
		dp += 69;
		
		// 代表者名
		memmove( dp, m_pSnHeadData->Sn_DIHYO, 20*2 );
		len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );
		if( len <= 28 ){
			//PF1[i].PITM[0] = dp;
			PF1[i++].PITM[0] = dp;
			PF1[i++].PITM[0] = ( char* )0;
		}else{
			PF1[i++].PITM[0] = ( char* )0;
			PF1[i++].PITM[0] = dp;
		}
		//i=i+2;
		dp += 41;

		//死亡年月日
		//char nn,yy,mm,dd;
		char nn = 0x00, yy = 0x00, mm = 0x00, dd = 0x00;
		if( m_pSnFhyo6Data->Sn_F6_DEATH ){
			m_Util.ADtoJC( &nn, &yy, &mm, &dd, m_pSnFhyo6Data->Sn_F6_DEATH );
			wsprintf( dp, _T("%2X   %2X   %2X"), yy, mm, dd );
			PF1[i++].PITM[0] = dp;
			dp += 13;
		}else{
			//i++;
			PF1[i++].PITM[0] = ( char* )0;
		}

		if ( nn == ID_ICSSH_REIWA_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = (char*) 0;
			PF1[i].PITM[1] = dp;
			dp += 5;
		}
		//else {
		else if ( nn == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = (char* )0;
			dp += 5;
		}
		i++;

		//相続人等の代表者の氏名
		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo6Data->Sn_F6_DAIHYO.GetBuffer() );
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[0] = dp;
		dp += 21;

		//限定承認
		if( m_pSnFhyo6Data->Sn_F6_LIMIT == 0 ){
			PF1[i++].PITM[0] = (char *)0;
		}else{
			PF1[i++].PITM[0] = " ";
		}

		//①　納める消費税及び地方消費税の合計額
		m_Arith.l_print( dp, F60val[0], FMT16_EX );
		PF1[i].PITM[j] = dp;
		dp+=19;
		//②　①のうち消費税
		m_Arith.l_print( dp, F60val[1], FMT16_EX );
		PF1[i].PITM[j+1] = dp;
		dp+=19;
		//③　①のうち地方消費税
		m_Arith.l_print( dp, F60val[2], FMT16_EX );
		PF1[i++].PITM[j+2] = dp;
		dp+=19;
		//④　還付される消費税及び地方消費税の合計額
		m_Arith.l_print( dp, F60val[3], FMT16_EX );
		PF1[i].PITM[j] = dp;
		dp+=19;
		//⑤　④のうち消費税
		m_Arith.l_print( dp, F60val[4], FMT16_EX );
		PF1[i].PITM[j+1] = dp;
		dp+=19;
		//⑥　④のうち地方消費税
		m_Arith.l_print( dp, F60val[5], FMT16_EX );
		PF1[i++].PITM[j+2] = dp;
		dp+=19;
	}
	if( ret = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT6;
	}

	for ( int cnt = 0; cnt < 30; cnt++ ) {
		pbclr ( 0, 40, &PF1[cnt] );
	}

	int idx = (pcnt-1)*CULM_PER_PAGE_F6;

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	for( j = 0; j < CULM_PER_PAGE_F6; idx++, j++ ){
		i=0;

		//住所
		int line_end = 0;
		int line_start = 0;
		int isEnd = 0;

		for( int line_num = 0; line_num < 4; line_num++ ){
			if( isEnd == -1 ){
				PF1[i++].PITM[j] =(char *)0;
				continue;
			}
			str = m_pSnFhyo6Data->Sn_F6_JUSYO[idx];
			isEnd = Getindex_toNewLineCode( line_start, &line_end, str );
			if( line_end > 0 ){
				str = m_pSnFhyo6Data->Sn_F6_JUSYO[idx].Mid( line_start, line_end );
				if( str.GetLength() > 0 ){
					line_start += line_end+2;
					memset( buf, '\0', sizeof( buf ) );
					memmove( buf , str , str.GetLength());
					memmove( dp, buf, (int)strlen( buf ) );
					PF1[i++].PITM[j] = dp;
					dp += 21;
				}else{
					PF1[i++].PITM[j] =(char *)0;
				}
			}else{
				line_start += 2;
				PF1[i++].PITM[j] =(char *)0;
			}
		}

		//ふりがな
		str = m_pSnFhyo6Data->Sn_F6_FURI[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 21;

		//氏名
		str = m_pSnFhyo6Data->Sn_F6_NAME[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 21;

		//個人番号
		int size = sizeof(m_pSnFhyo6Data->Sn_F6_HASH[idx]);
		BOOL mynum_empty = TRUE;
		for( int h_cnt = 0; h_cnt < size; h_cnt++ ){
			if( m_pSnFhyo6Data->Sn_F6_HASH[idx][h_cnt] == '\0' ){
				break;
			}
			if( m_pSnFhyo6Data->Sn_F6_HASH[idx][h_cnt] != 0 ){
				mynum_empty = FALSE;
				break;
			}
		}

		if( mynum_empty == FALSE ){
			NODATATBL ndtbl = {0};
			
			NTBMOD8_InitNoDataTbl( 0, &ndtbl );

			memmove( ndtbl.arrow, m_pSnFhyo6Data->Sn_F6_HASH[idx], sizeof( ndtbl.arrow ));

			if( NTBMOD8_GetNoDataTbl( 0, &ndtbl ) == 0 ){

				CICSMNSub8 mnsub;
				CString myno_prn;

				str.Format(ndtbl.no);

				if( m_pPset->psSigCoprn&0x4000){
					myno_prn = mnsub.MakePrintKojinNo( str,  m_pVprn->Get_KojinPrintFlg() );
				}else{
					myno_prn = str;
				}

				memmove( dp, myno_prn, 12 );
				PF1[i++].PITM[j] = dp;
				dp += 13;
			}else{
				PF1[i++].PITM[j] = (char *)0;//pbclr( j, j, &PF1[i++] );
			}
		}else{
			PF1[i++].PITM[j] =(char *)0;//pbclr( j, j, &PF1[i++] );
		}

		//職業
		line_end = 0;
		line_start = 0;
		isEnd = 0;

		str = m_pSnFhyo6Data->Sn_F6_SYOKU[idx];
		if( str.GetLength() <= 10 ){ //１行
			PF1[i++].PITM[j] = (char *)0;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
		}else
		if( str.GetLength() <= 22 ){ //２行
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;

			CString str1,str2;
			// 全角と半角が混ざっていると文字化けしている
			/*str1 = str.Mid(0,10);
			str2 = str.Mid(12,str.GetLength()-12);

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;*/


			str.Replace ( _T( "\r\n" ), _T( "" ) );
			memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
			memset( buf, '\0', sizeof( buf ) );
			wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
			memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

			len = strlen ( buf );
			memmove ( dp, buf, len );
			PF1[i++].PITM[j] = dp;
			dp += 12;

			str1 = str.Mid ( len, str.GetLength() );
			memset ( buf, '\0', sizeof ( buf ) );
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( buf, _T( "%s" ), str1.GetBuffer() );
			memmove ( dp, buf, ( int )strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 12;

		}else{ //３行
			CString str1,str2,str3;
			// 全角と半角が混ざっていると文字化けしている
			/*str1 = str.Mid(0,10);
			str2 = str.Mid(12,10);
			str3 = str.Mid(24, str.GetLength()-24);

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str3 , str3.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;*/

			str.Replace ( _T( "\r\n" ), _T( "" ) );
			memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
			memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

			len = strlen ( buf );
			memmove ( dp, buf, len );
			PF1[i++].PITM[j] = dp;
			dp += 12;

			str1 = str.Mid ( len, str.GetLength() );
			memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( tmpbuf, _T( "%s" ), str1.GetBuffer() );
			memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

			len = strlen ( buf );
			memmove ( dp, buf, len );
			PF1[i++].PITM[j] = dp;
			dp += 12;

			str2 = str1.Mid ( len, str1.GetLength() );
			memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( tmpbuf, _T( "%s" ), str2.GetBuffer() );
			memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

			len = strlen ( buf );
			memmove ( dp, buf, len );
			PF1[i++].PITM[j] = dp;
			dp += 12;

			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
		}


		//続柄
		str = m_pSnFhyo6Data->Sn_F6_TSUDUKI[idx];
		if( str.GetLength() <= 6 ){
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen ( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 7;
		}else{
			CString str1,str2;
			// 全角と半角が混ざっていると文字化けしている
			/*str1 = str.Mid(0,6);
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 7;

			str2 = str.Mid(6,4);
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 5;

			PF1[i++].PITM[j] = (char *)0;*/

			memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
			memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 6 ) );

			len = strlen ( buf );
			memmove ( dp, buf, len );
			PF1[i++].PITM[j] = dp;
			dp += 8;

			memset ( buf, '\0', sizeof ( buf ) );
			str1 = str.Mid ( len, 10 );
			wsprintf ( buf, _T( "%s" ), str1.GetBuffer() );
			memmove ( dp, buf, ( int )strlen ( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 8;

			PF1[i++].PITM[j] = ( char* )0;
		}


		//生年月日
		for ( int ii = i; ii < ( i + 5 ); ii++ ) {
			PF1[ii].PITM[j] = ( char* )0;
		}
		if(m_pSnFhyo6Data->Sn_F6_BIRTH[idx]){
			char nengo,yy,mm,dd;
			m_Util.ADtoJC( &nengo, &yy, &mm, &dd, m_pSnFhyo6Data->Sn_F6_BIRTH[idx] );

 			if( nengo == 0x00 ){		//選択なし
				;
			}else if( nengo == 0x01 ){	//明治
				PF1[i].PITM[j] = " ";
			}else if( nengo == 0x02 ){	//大正
				PF1[i+1].PITM[j] = " ";
			}else if( nengo == 0x03 ){	//昭和
				PF1[i+2].PITM[j] = " ";
			}else if( nengo == 0x04 ){	//平成
				PF1[i+3].PITM[j] = " ";
			}else if( nengo == 0x05 ){	//令和
				PF1[i+4].PITM[j] = " ";
			}
			i = i+5;

			wsprintf( dp, _T("%2X    %2X    %2X"), yy, mm, dd );
			PF1[i++].PITM[j] = dp;
			dp += 15;
		}else{
			//i = i+5;
			i = i+6;
		}

		//電話番号	
		m_pSnFhyo6Data->Sn_F6_TEL1[idx];
		m_pSnFhyo6Data->Sn_F6_TEL2[idx];
		m_pSnFhyo6Data->Sn_F6_TEL3[idx];
		wsprintf( dp, _T("%6s"), m_pSnFhyo6Data->Sn_F6_TEL1[idx]);
		PF1[i++].PITM[j] = dp;
		dp += 7;
		wsprintf( dp, _T("%4s"), m_pSnFhyo6Data->Sn_F6_TEL2[idx]);
		PF1[i++].PITM[j] = dp;
		dp += 5;
		wsprintf( dp, _T("%4s"), m_pSnFhyo6Data->Sn_F6_TEL3[idx] );
		PF1[i++].PITM[j] = dp;
		dp += 5;

		//相続分
		PF1[i].PITM[j] = (char *)0;
		PF1[i+1].PITM[j] = (char *)0;
		if( m_pSnFhyo6Data->Sn_F6_HOTEI[idx] == 1 ){
			PF1[i].PITM[j] = " ";
		}else if( m_pSnFhyo6Data->Sn_F6_HOTEI[idx] == 2 ){
			PF1[i+1].PITM[j] = " ";
		}
		i=i+2;

		str = m_pSnFhyo6Data->Sn_F6_BUNSHI[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 4;

		str = m_pSnFhyo6Data->Sn_F6_BUNBO[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 4;

		//相続財産の価額
		m_Arith.l_print( dp, m_pSnFhyo6Data->Sn_F6_KAGAKU[idx], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
	}

	if( ret = _xprn( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT6;
	}

	idx = (pcnt-1)*CULM_PER_PAGE_F6;

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	char bunshi[MONY_BUF_SIZE] = {0};
	char bunbo[MONY_BUF_SIZE] = {0};

	char zei_nofu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char chihozei_nofu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char total_nofu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char zei_kanpu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char chihozei_kanpu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char total_kanpu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};


	for( j = 0; j < CULM_PER_PAGE_F6; idx++, j++ ){

		i=0;

		//納付(還付)税額の計算

		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo6Data->Sn_F6_BUNBO[idx].GetBuffer() );
		m_Arith.l_input( bunbo, buf );

		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo6Data->Sn_F6_BUNSHI[idx].GetBuffer() );
		m_Arith.l_input( bunshi, buf );

		if( IsNofu ){
			//⑨
			if( m_pSnFhyo6Data->Sn_F6_NOFUKANPUsw[idx]&0x01 ){
				memmove( zei_nofu[j], m_pSnFhyo6Data->Sn_F6_NOFUZEI[idx], MONY_BUF_SIZE );
			}else{
				if( m_Arith.l_test( bunbo ) != 0 ){
					m_Arith.l_mul( zei_nofu[j], bunshi, F60val[1] );
					m_Arith.l_div( zei_nofu[j], zei_nofu[j], bunbo );
					m_Arith.l_div( zei_nofu[j], zei_nofu[j], PW1 ); //100円未満切り捨て
					m_Arith.l_mul( zei_nofu[j], zei_nofu[j], PW1 );
				}
			}
			//⑩
			if( m_pSnFhyo6Data->Sn_F6_NOFUKANPUsw[idx]&0x02 ){
				memmove( chihozei_nofu[j], m_pSnFhyo6Data->Sn_F6_NOFUCHIHO[idx], MONY_BUF_SIZE );
			}else{
				if( m_Arith.l_test( bunbo ) != 0 ){
					m_Arith.l_mul( chihozei_nofu[j], bunshi, F60val[2] );
					m_Arith.l_div( chihozei_nofu[j], chihozei_nofu[j], bunbo );
					m_Arith.l_div( chihozei_nofu[j], chihozei_nofu[j], PW1 ); //100円未満切り捨て
					m_Arith.l_mul( chihozei_nofu[j], chihozei_nofu[j], PW1 );
				}
			}
			//⑪
			m_Arith.l_add( total_nofu[j], zei_nofu[j], chihozei_nofu[j] );
		}
		if( IsKanpu ){
			//⑫
			memmove( zei_kanpu[j],m_pSnFhyo6Data->Sn_F6_KANPUZEI[idx],MONY_BUF_SIZE );
			//⑬
			memmove( chihozei_kanpu[j],m_pSnFhyo6Data->Sn_F6_KANPUCHIHO[idx],MONY_BUF_SIZE );
			//⑭
			m_Arith.l_add( total_kanpu[j], zei_kanpu[j], chihozei_kanpu[j] );
			if( m_Arith.l_test(total_kanpu[j]) < 0 ){
				m_Arith.l_neg(total_kanpu[j]);
			}
		}

		m_Arith.l_print( dp, zei_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, chihozei_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, total_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, zei_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, chihozei_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, total_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
	}
	if( ret = _xprn( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT6;
	}


	if(( IsKanpu == TRUE )|| m_pPrintCmInfo->OPI ){

		idx = (pcnt-1)*CULM_PER_PAGE_F6;

		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );

		for( j = 0; j < CULM_PER_PAGE_F6; idx++, j++ ){

			i=0;

			//銀行名等
			if( m_pSnFhyo6Data->Sn_F6_GNAME[idx].GetLength() > 10 ){
				// 全角と半角が混ざっていると文字化けしている
				/*str = m_pSnFhyo6Data->Sn_F6_GNAME[idx].Mid(0,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx].Mid(10,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				PF1[i++].PITM[j] = (char *)0;*/

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx].Mid ( len, 10 );
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( buf, _T( "%s" ), str.GetBuffer() );
				memmove ( dp, buf, ( int )strlen ( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				PF1[i++].PITM[j] = ( char* )0;
			}else{
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen ( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;
			}

			PF1[i].PITM[j] = (char *)0;
			PF1[i+1].PITM[j] = (char *)0;
			PF1[i+2].PITM[j] = (char *)0;
			PF1[i+3].PITM[j] = (char *)0;
			PF1[i+4].PITM[j] = (char *)0;
			CString str = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
			str.Replace(_T("　"),"");
			if( str.Trim().GetLength() ){
				switch(m_pSnFhyo6Data->Sn_F6_GSEL[idx]){
					case 0: //銀行
						PF1[i].PITM[j] = " ";
						break;
					case 1: //金庫
						PF1[i+1].PITM[j] = " ";
						break;
					case 2: //組合
						PF1[i+2].PITM[j] = " ";
						break;
					case 3: //農協
						PF1[i+3].PITM[j] = " ";
						break;
					case 4: //漁協
						PF1[i+4].PITM[j] = " ";
						break;
					default:
						break;
				}
			}
			i = i+5;

			//支店名等
			if( m_pSnFhyo6Data->Sn_F6_SNAME[idx].GetLength() > 10 ){
				// 全角と半角が混ざっていると文字化けしている
				/*str = m_pSnFhyo6Data->Sn_F6_SNAME[idx].Mid(0,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx].Mid(10,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				PF1[i++].PITM[j] = (char *)0;*/

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx];
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx].Mid ( len, 10 );
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( buf, _T( "%s" ), str.GetBuffer() );
				memmove ( dp, buf, ( int )strlen ( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				PF1[i++].PITM[j] = ( char* )0;

			}else{
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx];
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;
			}

			PF1[i].PITM[j] = (char *)0;
			PF1[i+1].PITM[j] = (char *)0;
			PF1[i+2].PITM[j] = (char *)0;
			PF1[i+3].PITM[j] = (char *)0;
			PF1[i+4].PITM[j] = (char *)0;
			CString str1,str2;
			str1 = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
			str2 = m_pSnFhyo6Data->Sn_F6_SNAME[idx];
			str1.Replace(_T("　"),"");
			str2.Replace(_T("　"),"");
			if(( str1.Trim().GetLength() != 0 )||( str2.Trim().GetLength() != 0 )){ //銀行名入力済でも本店支店選択表示
				switch(m_pSnFhyo6Data->Sn_F6_SSEL[idx]){
					case 0: //本店
						PF1[i].PITM[j] = " ";
						break;
					case 1: //支店
						PF1[i+1].PITM[j] = " ";
						break;
					case 2: //本所
						PF1[i+3].PITM[j] = " ";
						break;
					case 3: //支所
						PF1[i+4].PITM[j] = " ";
						break;
					case 5: //出張所
						PF1[i+2].PITM[j] = " ";
						break;
					case 4: //なし
					case 6: //上記以外
						break;
					default:
						break;
				}
			}
			i = i+5;

			//預金の種類
			str = m_pSnFhyo6Data->Sn_F6_YOKIN[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 17;

			//口座番号
			str = m_pSnFhyo6Data->Sn_F6_KOUZA[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			//記号番号
			str = m_pSnFhyo6Data->Sn_F6_KIGO1[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 6;

			str = m_pSnFhyo6Data->Sn_F6_KIGO2[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 9;

			//郵便局名
			str = m_pSnFhyo6Data->Sn_F6_YUBIN[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 17;
		}
		if( ret = _xprn( PP, 4, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT6;
		}
	}

	if (AP) {
		delete AP;
		AP = NULL;
	}


POUT6:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return ret;
}

//-----------------------------------------------------------------------------
// 付表６出力
//-----------------------------------------------------------------------------
// 引数	pDC
//		pcnt 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::PrintFuhyo6_02( CDC *pDC, int pcnt )
{

	int				i, j;
	char			*AP = NULL;
	char			*dp, buf[512] = {0};
	char tmpbuf[512] = { 0 };
	int ret = 0;
	int len = 0;

	CString str = _T("");
	unsigned short	SEno = 1;
	BOOL			isNoImage = FALSE;

	//----->インボイス施工日以降は付表７に変更('23.07.10 Add)
	short emfNo = 0;
	long smd = 0, emd = 0;
	GetKazeiKikan(smd, emd);
	if ( emd >= ICS_SH_INVOICE_DAY ) {
		emfNo = MAKEWORD(2, 2);
	}
	else {
		emfNo = MAKEWORD(1, 1);
	}
	//<-----

	if( m_pPrintCmInfo->PRtype&&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	unsigned int	size=2048;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	dp = AP;
	memset( dp, '\0', size );
	i=j=0;

	PBUF	PF1[30]={0};
	//int		PP = 9;
	int		PP = 36;

	char	PW0[6], PW1[6];
	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_Arith.l_input( PW1, _T("100") );


	//本表データ計算
	//ARITH_VAL F60val[6];
	unsigned char F60val[6][MONY_BUF_SIZE];
	if (!m_pFhyo6) {
		return 1;
	}
	m_pFhyo6->f6calq_1to6();
	memmove(F60val[0], m_pFhyo6->F61, MONY_BUF_SIZE);
	memmove(F60val[1], m_pFhyo6->F62, MONY_BUF_SIZE);
	memmove(F60val[2], m_pFhyo6->F63, MONY_BUF_SIZE);
	memmove(F60val[3], m_pFhyo6->F64, MONY_BUF_SIZE);
	memmove(F60val[4], m_pFhyo6->F65, MONY_BUF_SIZE);
	memmove(F60val[5], m_pFhyo6->F66, MONY_BUF_SIZE);

	BOOL IsNofu = FALSE,IsKanpu = FALSE;
	if( m_Arith.l_test( F60val[0] ) != 0 ){
		IsNofu = TRUE;
	}
	if( m_Arith.l_test( F60val[3] ) != 0 ){
		IsKanpu = TRUE;
	}

	if ( emd >= ICS_SH_INVOICE_DAY ) {
		i += 3;
	}

		// 申告期間自
		int gengo1 = 0, gengo2 = 0;
		char	yymmdd[3] = {0};
		memset( yymmdd, '\0', sizeof(yymmdd) );
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1 );
		wsprintf( dp, _T("%2X   %2X   %2X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 13;

		// 申告期間至
		memset( yymmdd, '\0', sizeof(yymmdd) );
		m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2 );
		wsprintf( dp, _T("%2X   %2X   %2X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 13;

		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = dp;
			dp += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = ( char* )0;
			dp += 5;
		}
		else {
			PF1[i].PITM[0] = (char* )0;
			PF1[i].PITM[1] = (char* )0;
		}
		i++;

		if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = dp;
			dp += 5;
		}
		else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = ( char* )0;
			dp += 5;
		}
		else {
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = ( char*) 0;
		}
		i++;

	// 整理番号
	if( (m_pSnHeadData->Sn_SEIRI[0]&0xff) != 0xff ){
		memset( buf, '\0', sizeof( buf ) );
		m_Arith.l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
		int k;
		for( k=(int)strlen(buf); k>0; ){
			if( buf[--k] != 0x20 ){
				break;
			}
		}
		++k;
		if( k < 8 ){
			memmove( dp, "        ", 8-k );
		}
		memmove( (dp+(8-k)), buf, k );				
		PF1[i++].PITM[0] = dp;
		dp += 9;
	}
	else{
		PF1[i++].PITM[0] = (char *)0;
	}

	//if( pcnt == 1 ){	// 19-0539 2ページ目以降も納税地などを出力する
		// 納税地（２行）
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[0],  20*2 );				
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[40], 20*2 );
		PF1[i++].PITM[1] = dp;
		dp += 41;

		// 代表者（フリガナ）
		// 付表６では25文字で表示しているため、印刷も同様にする
		//memmove( dp, m_pSnHeadData->Sn_DIFRI, 50 );
		//int len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIFRI, sizeof(m_pSnHeadData->Sn_DIFRI) );
		//len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIFRI, sizeof(m_pSnHeadData->Sn_DIFRI) );
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, m_pSnHeadData->Sn_DIFRI, 50 );
		memmove ( dp, buf, 50 );
		len = m_Util.MixSpaceCutLength ( buf, 50 );
		if( len <= 56 ){
			//PF1[i].PITM[0] = dp;
			PF1[i++].PITM[0] = dp;
			PF1[i++].PITM[0] = ( char* )0;
		}else{
			//PF1[i+1].PITM[0] = dp;
			PF1[i++].PITM[0] = ( char* )0;
			PF1[i++].PITM[0] = dp;
		}
		//i=i+2;
		dp += 69;

		// 代表者名
		memmove( dp, m_pSnHeadData->Sn_DIHYO, 20*2 );
		len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );
		if( len <= 28 ){
			//PF1[i].PITM[0] = dp;
			PF1[i++].PITM[0] = dp;
			PF1[i++].PITM[0] = ( char* )0;
		}else{
			PF1[i++].PITM[0] = ( char* )0;
			PF1[i++].PITM[0] = dp;
		}
		//i=i+2;
		dp += 41;

		//死亡年月日
		//char nn,yy,mm,dd;
		char nn = 0x00, yy = 0x00, mm = 0x00, dd = 0x00;
		if( m_pSnFhyo6Data->Sn_F6_DEATH ){
			m_Util.ADtoJC( &nn, &yy, &mm, &dd, m_pSnFhyo6Data->Sn_F6_DEATH );
			wsprintf( dp, _T("%2X   %2X   %2X"), yy, mm, dd );
			PF1[i++].PITM[0] = dp;
			dp += 13;
		}else{
			//i++;
			PF1[i++].PITM[0] = ( char* )0;
		}

		if ( nn == ID_ICSSH_REIWA_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = (char*) 0;
			PF1[i].PITM[1] = dp;
			dp += 5;
		}
		//else {
		else if ( nn == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( dp, "    ", 4 );
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = (char* )0;
			dp += 5;
		}
		i++;

		// 事業承継の有無
		memmove ( dp, "    ", 4 );
		if ( m_pSnFhyo6Data->Sn_6BS_USE ) {
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = ( char* )0;
		}
		else {
			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = dp;
		}
		dp += 5;
		i++;

		if ( m_pSnFhyo6Data->Sn_6BS_USE  ) {
			// 住所等
			memmove ( dp, m_pSnFhyo6Data->Sn_6BS_ADR, 40 );
			PF1[i].PITM[0] = dp;
			dp += 41;

			memmove ( dp, &m_pSnFhyo6Data->Sn_6BS_ADR[40], 40 );
			PF1[i].PITM[1] = dp;
			dp += 41;
			i++;

			// 電話番号
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, m_pSnFhyo6Data->Sn_6BS_TELNO1, 6 );
			str.Format ( _T( "%s" ), buf );
			str.TrimRight();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, str, str.GetLength() );
			memmove ( dp, buf, 6 );
			PF1[i].PITM[0] = dp;
			dp += 7;

			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, m_pSnFhyo6Data->Sn_6BS_TELNO2, 4 );
			str.Format ( _T( "%s" ), buf );
			str.TrimRight();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, str, str.GetLength() );
			memmove ( dp, buf, sizeof ( buf ) );
			PF1[i].PITM[1] = dp;
			dp += 5;

			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, m_pSnFhyo6Data->Sn_6BS_TELNO3, 4 );
			str.Format ( _T( "%s" ), buf );
			str.TrimRight();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, str, str.GetLength() );
			memmove ( dp, buf, sizeof ( buf ) );
			PF1[i++].PITM[2] = dp;
			j = 0;
			dp += 5;

			// フリガナ
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( buf, _T( "%s" ), m_pSnFhyo6Data->Sn_6BS_NMFRI.GetBuffer() );
			len = strlen ( buf );
			memmove ( dp, buf, len );
			if ( len <= 56 ) {
				PF1[i++].PITM[0] = dp;
				PF1[i++].PITM[0] = ( char* )0;
			}
			else{
				PF1[i++].PITM[0] = ( char* )0;
				PF1[i++].PITM[0] = dp;
			}
			dp += 69;

			// 氏名
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( buf, _T( "%s" ), m_pSnFhyo6Data->Sn_6BS_NAME.GetBuffer() );
			len = strlen ( buf );
			memmove ( dp, buf, len );
			if ( len <= 28 ) {
				PF1[i++].PITM[0] = dp;
				PF1[i++].PITM[0] = ( char* )0;
			}
			else{
				PF1[i++].PITM[0] = ( char* )0;
				PF1[i++].PITM[0] = dp;
			}
			dp += 41;

			// その他参考事項
			unsigned char sonota[80] = { 0 };
			int cnt = 0;
			m_Util.cstring_to_char ( sonota, m_pSnFhyo6Data->Sn_6BS_SONOTA, 80 );
			for ( int idx = 0; idx < 2; idx++ ) {
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				if ( idx == 0 ) {
					memmove ( buf, sonota, 40 );
				}
				else {
					memmove ( buf, &sonota[40], 40 );
				}

				memcpy ( tmpbuf, buf, Strchek ( buf, 20 ) );

				len = strlen ( tmpbuf );
				memmove ( dp, tmpbuf, len );
				PF1[i].PITM[cnt++] = dp;
				dp += 22;

				memmove ( dp, &buf[len], ( 40 - len ) );
				PF1[i].PITM[cnt++] = dp;
				dp += 22;
			}

		}
		else {
			PF1[i].PITM[0] = ( char* )0;
			PF1[i++].PITM[1] = ( char* )0;

			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = ( char* )0;
			PF1[i++].PITM[2] = ( char* )0;

			PF1[i++].PITM[0] = ( char* )0;
			PF1[i++].PITM[0] = ( char* )0;

			PF1[i++].PITM[0] = ( char* )0;
			PF1[i++].PITM[0] = ( char* )0;

			PF1[i].PITM[0] = ( char* )0;
			PF1[i].PITM[1] = ( char* )0;
			PF1[i].PITM[2] = ( char* )0;
			PF1[i].PITM[3] = ( char* )0;
		}
		i++;

		//相続人等の代表者の氏名
		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo6Data->Sn_F6_DAIHYO.GetBuffer() );
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[0] = dp;
		dp += 21;

		//限定承認
		if( m_pSnFhyo6Data->Sn_F6_LIMIT == 0 ){
			PF1[i++].PITM[0] = (char *)0;
		}else{
			PF1[i++].PITM[0] = " ";
		}

		//①　納める消費税及び地方消費税の合計額
		m_Arith.l_print( dp, F60val[0], FMT16_EX );
		PF1[i].PITM[j] = dp;
		dp+=19;
		//②　①のうち消費税
		m_Arith.l_print( dp, F60val[1], FMT16_EX );
		PF1[i].PITM[j+1] = dp;
		dp+=19;
		//③　①のうち地方消費税
		m_Arith.l_print( dp, F60val[2], FMT16_EX );
		PF1[i++].PITM[j+2] = dp;
		dp+=19;
		//④　還付される消費税及び地方消費税の合計額
		m_Arith.l_print( dp, F60val[3], FMT16_EX );
		PF1[i].PITM[j] = dp;
		dp+=19;
		//⑤　④のうち消費税
		m_Arith.l_print( dp, F60val[4], FMT16_EX );
		PF1[i].PITM[j+1] = dp;
		dp+=19;
		//⑥　④のうち地方消費税
		m_Arith.l_print( dp, F60val[5], FMT16_EX );
		PF1[i++].PITM[j+2] = dp;
		dp+=19;
	//}
	if( ret = _xprn( PP, 1, PF1, 0, pDC, isNoImage, emfNo ) ){
		goto POUT6;
	}

	for ( int cnt = 0; cnt < 30; cnt++ ) {
		pbclr ( 0, 40, &PF1[cnt] );
	}

	int idx = (pcnt-1)*CULM_PER_PAGE_F6;

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	for( j = 0; j < CULM_PER_PAGE_F6; idx++, j++ ){
		i=0;

		//住所
		int line_end = 0;
		int line_start = 0;
		int isEnd = 0;

		for( int line_num = 0; line_num < 4; line_num++ ){
			if( isEnd == -1 ){
				PF1[i++].PITM[j] =(char *)0;
				continue;
			}
			str = m_pSnFhyo6Data->Sn_F6_JUSYO[idx];
			isEnd = Getindex_toNewLineCode( line_start, &line_end, str );
			if( line_end > 0 ){
				str = m_pSnFhyo6Data->Sn_F6_JUSYO[idx].Mid( line_start, line_end );
				if( str.GetLength() > 0 ){
					line_start += line_end+2;
					memset( buf, '\0', sizeof( buf ) );
					memmove( buf , str , str.GetLength());
					memmove( dp, buf, (int)strlen( buf ) );
					PF1[i++].PITM[j] = dp;
					dp += 21;
				}else{
					PF1[i++].PITM[j] =(char *)0;
				}
			}else{
				line_start += 2;
				PF1[i++].PITM[j] =(char *)0;
			}
		}

		//ふりがな
		str = m_pSnFhyo6Data->Sn_F6_FURI[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 21;

		//氏名
		str = m_pSnFhyo6Data->Sn_F6_NAME[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 21;

		//個人番号
		int size = sizeof(m_pSnFhyo6Data->Sn_F6_HASH[idx]);
		BOOL mynum_empty = TRUE;
		for( int h_cnt = 0; h_cnt < size; h_cnt++ ){
			if( m_pSnFhyo6Data->Sn_F6_HASH[idx][h_cnt] == '\0' ){
				break;
			}
			if( m_pSnFhyo6Data->Sn_F6_HASH[idx][h_cnt] != 0 ){
				mynum_empty = FALSE;
				break;
			}
		}

		if( mynum_empty == FALSE ){
			NODATATBL ndtbl = {0};

			NTBMOD8_InitNoDataTbl( 0, &ndtbl );

			memmove( ndtbl.arrow, m_pSnFhyo6Data->Sn_F6_HASH[idx], sizeof( ndtbl.arrow ));

			if( NTBMOD8_GetNoDataTbl( 0, &ndtbl ) == 0 ){

				CICSMNSub8 mnsub;
				CString myno_prn;

				str.Format(ndtbl.no);

				if( m_pPset->psSigCoprn&0x4000){
					myno_prn = mnsub.MakePrintKojinNo( str,  m_pVprn->Get_KojinPrintFlg() );
				}else{
					myno_prn = str;
				}

				memmove( dp, myno_prn, 12 );
				PF1[i++].PITM[j] = dp;
				dp += 13;
			}else{
				PF1[i++].PITM[j] = (char *)0;//pbclr( j, j, &PF1[i++] );
			}
		}else{
			PF1[i++].PITM[j] =(char *)0;//pbclr( j, j, &PF1[i++] );
		}

		//職業
		line_end = 0;
		line_start = 0;
		isEnd = 0;

		str = m_pSnFhyo6Data->Sn_F6_SYOKU[idx];
		if( str.GetLength() <= 10 ){ //１行
			PF1[i++].PITM[j] = (char *)0;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
		}else
			if( str.GetLength() <= 22 ){ //２行
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				CString str1,str2;
				// 全角と半角が混ざっていると文字化けしている
				/*str1 = str.Mid(0,10);
				str2 = str.Mid(12,str.GetLength()-12);

				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str1 , str1.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str2 , str2.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;*/


				str.Replace ( _T( "\r\n" ), _T( "" ) );
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset( buf, '\0', sizeof( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str1 = str.Mid ( len, str.GetLength() );
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( buf, _T( "%s" ), str1.GetBuffer() );
				memmove ( dp, buf, ( int )strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 12;

			}else{ //３行
				CString str1,str2,str3;
				// 全角と半角が混ざっていると文字化けしている
				/*str1 = str.Mid(0,10);
				str2 = str.Mid(12,10);
				str3 = str.Mid(24, str.GetLength()-24);

				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str1 , str1.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str2 , str2.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str3 , str3.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;*/

				str.Replace ( _T( "\r\n" ), _T( "" ) );
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str1 = str.Mid ( len, str.GetLength() );
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str1.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str2 = str1.Mid ( len, str1.GetLength() );
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str2.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;
			}


		//続柄
		str = m_pSnFhyo6Data->Sn_F6_TSUDUKI[idx];
		if( str.GetLength() <= 6 ){
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen ( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 7;
		}else{
			CString str1,str2;
			// 全角と半角が混ざっていると文字化けしている
			/*str1 = str.Mid(0,6);
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 7;

			str2 = str.Mid(6,4);
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 5;

			PF1[i++].PITM[j] = (char *)0;*/

			memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
			memset ( buf, '\0', sizeof ( buf ) );
			wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
			memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 6 ) );

			len = strlen ( buf );
			memmove ( dp, buf, len );
			PF1[i++].PITM[j] = dp;
			dp += 8;

			memset ( buf, '\0', sizeof ( buf ) );
			str1 = str.Mid ( len, 10 );
			wsprintf ( buf, _T( "%s" ), str1.GetBuffer() );
			memmove ( dp, buf, ( int )strlen ( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 8;

			PF1[i++].PITM[j] = ( char* )0;
		}


		//生年月日
		for ( int ii = i; ii < ( i + 5 ); ii++ ) {
			PF1[ii].PITM[j] = ( char* )0;
		}
		if(m_pSnFhyo6Data->Sn_F6_BIRTH[idx]){
			char nengo,yy,mm,dd;
			m_Util.ADtoJC( &nengo, &yy, &mm, &dd, m_pSnFhyo6Data->Sn_F6_BIRTH[idx] );

			if( nengo == 0x00 ){		//選択なし
				;
			}else if( nengo == 0x01 ){	//明治
				PF1[i].PITM[j] = " ";
			}else if( nengo == 0x02 ){	//大正
				PF1[i+1].PITM[j] = " ";
			}else if( nengo == 0x03 ){	//昭和
				PF1[i+2].PITM[j] = " ";
			}else if( nengo == 0x04 ){	//平成
				PF1[i+3].PITM[j] = " ";
			}else if( nengo == 0x05 ){	//令和
				PF1[i+4].PITM[j] = " ";
			}
			i = i+5;

			wsprintf( dp, _T("%2X    %2X    %2X"), yy, mm, dd );
			PF1[i++].PITM[j] = dp;
			dp += 15;
		}else{
			//i = i+5;
			i = i+6;
		}

		//電話番号	
		m_pSnFhyo6Data->Sn_F6_TEL1[idx];
		m_pSnFhyo6Data->Sn_F6_TEL2[idx];
		m_pSnFhyo6Data->Sn_F6_TEL3[idx];
		wsprintf( dp, _T("%6s"), m_pSnFhyo6Data->Sn_F6_TEL1[idx]);
		PF1[i++].PITM[j] = dp;
		dp += 7;
		wsprintf( dp, _T("%4s"), m_pSnFhyo6Data->Sn_F6_TEL2[idx]);
		PF1[i++].PITM[j] = dp;
		dp += 5;
		wsprintf( dp, _T("%4s"), m_pSnFhyo6Data->Sn_F6_TEL3[idx] );
		PF1[i++].PITM[j] = dp;
		dp += 5;

		//相続分
		PF1[i].PITM[j] = (char *)0;
		PF1[i+1].PITM[j] = (char *)0;
		if( m_pSnFhyo6Data->Sn_F6_HOTEI[idx] == 1 ){
			PF1[i].PITM[j] = " ";
		}else if( m_pSnFhyo6Data->Sn_F6_HOTEI[idx] == 2 ){
			PF1[i+1].PITM[j] = " ";
		}
		i=i+2;

		str = m_pSnFhyo6Data->Sn_F6_BUNSHI[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 4;

		str = m_pSnFhyo6Data->Sn_F6_BUNBO[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 4;

		//相続財産の価額
		m_Arith.l_print( dp, m_pSnFhyo6Data->Sn_F6_KAGAKU[idx], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
	}

	if( ret = _xprn( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT6;
	}

	idx = (pcnt-1)*CULM_PER_PAGE_F6;

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	char bunshi[MONY_BUF_SIZE] = {0};
	char bunbo[MONY_BUF_SIZE] = {0};

	char zei_nofu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char chihozei_nofu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char total_nofu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char zei_kanpu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char chihozei_kanpu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};
	char total_kanpu[CULM_PER_PAGE_F6][MONY_BUF_SIZE] = {0};


	for( j = 0; j < CULM_PER_PAGE_F6; idx++, j++ ){

		i=0;

		//納付(還付)税額の計算

		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo6Data->Sn_F6_BUNBO[idx].GetBuffer() );
		m_Arith.l_input( bunbo, buf );

		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo6Data->Sn_F6_BUNSHI[idx].GetBuffer() );
		m_Arith.l_input( bunshi, buf );

		if( IsNofu ){
			//⑨
			if( m_pSnFhyo6Data->Sn_F6_NOFUKANPUsw[idx]&0x01 ){
				memmove( zei_nofu[j], m_pSnFhyo6Data->Sn_F6_NOFUZEI[idx], MONY_BUF_SIZE );
			}else{
				if( m_Arith.l_test( bunbo ) != 0 ){
					m_Arith.l_mul( zei_nofu[j], bunshi, F60val[1] );
					m_Arith.l_div( zei_nofu[j], zei_nofu[j], bunbo );
					m_Arith.l_div( zei_nofu[j], zei_nofu[j], PW1 ); //100円未満切り捨て
					m_Arith.l_mul( zei_nofu[j], zei_nofu[j], PW1 );
				}
			}
			//⑩
			if( m_pSnFhyo6Data->Sn_F6_NOFUKANPUsw[idx]&0x02 ){
				memmove( chihozei_nofu[j], m_pSnFhyo6Data->Sn_F6_NOFUCHIHO[idx], MONY_BUF_SIZE );
			}else{
				if( m_Arith.l_test( bunbo ) != 0 ){
					m_Arith.l_mul( chihozei_nofu[j], bunshi, F60val[2] );
					m_Arith.l_div( chihozei_nofu[j], chihozei_nofu[j], bunbo );
					m_Arith.l_div( chihozei_nofu[j], chihozei_nofu[j], PW1 ); //100円未満切り捨て
					m_Arith.l_mul( chihozei_nofu[j], chihozei_nofu[j], PW1 );
				}
			}
			//⑪
			m_Arith.l_add( total_nofu[j], zei_nofu[j], chihozei_nofu[j] );
		}
		if( IsKanpu ){
			//⑫
			memmove( zei_kanpu[j],m_pSnFhyo6Data->Sn_F6_KANPUZEI[idx],MONY_BUF_SIZE );
			//⑬
			memmove( chihozei_kanpu[j],m_pSnFhyo6Data->Sn_F6_KANPUCHIHO[idx],MONY_BUF_SIZE );
			//⑭
			m_Arith.l_add( total_kanpu[j], zei_kanpu[j], chihozei_kanpu[j] );
			if( m_Arith.l_test(total_kanpu[j]) < 0 ){
				m_Arith.l_neg(total_kanpu[j]);
			}
		}

		m_Arith.l_print( dp, zei_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, chihozei_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, total_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, zei_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, chihozei_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, total_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
	}
	if( ret = _xprn( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT6;
	}


	if(( IsKanpu == TRUE )|| m_pPrintCmInfo->OPI ){

		idx = (pcnt-1)*CULM_PER_PAGE_F6;

		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );

		for( j = 0; j < CULM_PER_PAGE_F6; idx++, j++ ){

			i=0;

			//銀行名等
			if( m_pSnFhyo6Data->Sn_F6_GNAME[idx].GetLength() > 10 ){
				// 全角と半角が混ざっていると文字化けしている
				/*str = m_pSnFhyo6Data->Sn_F6_GNAME[idx].Mid(0,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx].Mid(10,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				PF1[i++].PITM[j] = (char *)0;*/

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx].Mid ( len, 10 );
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( buf, _T( "%s" ), str.GetBuffer() );
				memmove ( dp, buf, ( int )strlen ( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				PF1[i++].PITM[j] = ( char* )0;
			}else{
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				str = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen ( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;
			}

			PF1[i].PITM[j] = (char *)0;
			PF1[i+1].PITM[j] = (char *)0;
			PF1[i+2].PITM[j] = (char *)0;
			PF1[i+3].PITM[j] = (char *)0;
			PF1[i+4].PITM[j] = (char *)0;
			CString str = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
			str.Replace(_T("　"),"");
			if( str.Trim().GetLength() ){
				switch(m_pSnFhyo6Data->Sn_F6_GSEL[idx]){
				case 0: //銀行
					PF1[i].PITM[j] = " ";
					break;
				case 1: //金庫
					PF1[i+1].PITM[j] = " ";
					break;
				case 2: //組合
					PF1[i+2].PITM[j] = " ";
					break;
				case 3: //農協
					PF1[i+3].PITM[j] = " ";
					break;
				case 4: //漁協
					PF1[i+4].PITM[j] = " ";
					break;
				default:
					break;
				}
			}
			i = i+5;

			//支店名等
			if( m_pSnFhyo6Data->Sn_F6_SNAME[idx].GetLength() > 10 ){
				// 全角と半角が混ざっていると文字化けしている
				/*str = m_pSnFhyo6Data->Sn_F6_SNAME[idx].Mid(0,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx].Mid(10,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				PF1[i++].PITM[j] = (char *)0;*/

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx];
				memset ( tmpbuf, '\0', sizeof ( tmpbuf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( tmpbuf, _T( "%s" ), str.GetBuffer() );
				memcpy ( buf, tmpbuf, Strchek ( tmpbuf, 10 ) );

				len = strlen ( buf );
				memmove ( dp, buf, len );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx].Mid ( len, 10 );
				memset ( buf, '\0', sizeof ( buf ) );
				memset ( buf, '\0', sizeof ( buf ) );
				wsprintf ( buf, _T( "%s" ), str.GetBuffer() );
				memmove ( dp, buf, ( int )strlen ( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 12;

				PF1[i++].PITM[j] = ( char* )0;

			}else{
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				str = m_pSnFhyo6Data->Sn_F6_SNAME[idx];
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;
			}

			PF1[i].PITM[j] = (char *)0;
			PF1[i+1].PITM[j] = (char *)0;
			PF1[i+2].PITM[j] = (char *)0;
			PF1[i+3].PITM[j] = (char *)0;
			PF1[i+4].PITM[j] = (char *)0;
			CString str1,str2;
			str1 = m_pSnFhyo6Data->Sn_F6_GNAME[idx];
			str2 = m_pSnFhyo6Data->Sn_F6_SNAME[idx];
			str1.Replace(_T("　"),"");
			str2.Replace(_T("　"),"");
			if(( str1.Trim().GetLength() != 0 )||( str2.Trim().GetLength() != 0 )){ //銀行名入力済でも本店支店選択表示
				switch(m_pSnFhyo6Data->Sn_F6_SSEL[idx]){
				case 0: //本店
					PF1[i].PITM[j] = " ";
					break;
				case 1: //支店
					PF1[i+1].PITM[j] = " ";
					break;
				case 2: //本所
					PF1[i+3].PITM[j] = " ";
					break;
				case 3: //支所
					PF1[i+4].PITM[j] = " ";
					break;
				case 5: //出張所
					PF1[i+2].PITM[j] = " ";
					break;
				case 4: //なし
				case 6: //上記以外
					break;
				default:
					break;
				}
			}
			i = i+5;

			//預金の種類
			str = m_pSnFhyo6Data->Sn_F6_YOKIN[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 17;

			//口座番号
			str = m_pSnFhyo6Data->Sn_F6_KOUZA[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			//記号番号
			str = m_pSnFhyo6Data->Sn_F6_KIGO1[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 6;

			str = m_pSnFhyo6Data->Sn_F6_KIGO2[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 9;

			//郵便局名
			str = m_pSnFhyo6Data->Sn_F6_YUBIN[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 17;
		}
		if( ret = _xprn( PP, 4, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT6;
		}
	}

	if (AP) {
		delete AP;
		AP = NULL;
	}


POUT6:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return ret;
}

//=============================================================================
// 付表６（税率別消費税額計算表）出力
//-----------------------------------------------------------------------------
// 引数
//		pDC		:		デバイスコンテキスト
//		kojin	:		個人業種？
//		pcnt	:		未使用
//-----------------------------------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=============================================================================
int CH31HyoView::PrintFuhyo6_2wari(CDC *pDC, int kojin, int pcnt)
{
	int				i = 0, j = 0;
	unsigned int	size = 0;
	char			*AP = NULL;
	char			*dp = NULL, PW0[MONY_BUF_SIZE] = { 0 };
	char			PW1[MONY_BUF_SIZE] = { 0 };

	BOOL isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;
	}
	else {
		isNoImage = TRUE;
	}

	size = 1024;
	if ((AP = new char[size]) == NULL) {
		return -1;
	}

	memset(PW0, '\0', MONY_BUF_SIZE);
	m_Arith.l_input(PW0, _T("1000"));

	memset(PW1, '\0', MONY_BUF_SIZE);
	m_Arith.l_input(PW1, _T("100"));

	char WORK0[MONY_BUF_SIZE] = { 0 };
	memset(WORK0, '\0', MONY_BUF_SIZE);

	PBUF PF1[30] = {0};
	int	PP = 0;

	PP = 34;

	dp = AP;
	memset(dp, '\0', size);
	i = j = 0;

	char yymmdd[3] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
	PF1[i].PITM[0] = dp;
	dp += 9;

	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(dp, _T("%02X %02X %02X"), yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);
	PF1[i++].PITM[1] = dp;
	dp += 9;

	if (kojin) {	// 個人事業者の場合は代表者氏名
		j = sizeof(m_pSnHeadData->Sn_DIHYO);
		memmove(dp, m_pSnHeadData->Sn_DIHYO, j);
	}
	else {			// 法人名
		j = sizeof(m_pSnHeadData->Sn_CONAM);
		memmove(dp, m_pSnHeadData->Sn_CONAM, j);
	}
	PF1[i++].PITM[0] = dp;
	dp += (40 + 1);

	if ((j = _xprn(PP, 1, PF1, 0, pDC, isNoImage, 1))) {
		goto POUT41;
	}

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	// 1-A
//----->('23.05.01 cor)
	//m_Arith.l_div(WORK0, m_pSnFhyo6_01Data->Sn_F601_1A, PW0);
	//m_Arith.l_print(dp, WORK0, FMT110_EX);
	//strcat_s(dp, (18 + 1), _T("   "));
//------
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_1A, FMT15_EX);
//<-----
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 2-A
//----->('23.05.01 cor)
//	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_2A, FMT15_EX);
//------
	m_Arith.l_div(WORK0, m_pSnFhyo6_01Data->Sn_F601_2A, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, (18 + 1), _T("   "));
//<-----
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 3-A
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_3A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-A
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_4A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-A
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_5A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-A
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_6A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 7-A
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_7A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 8-A
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_8A, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if ((j = _xprn(PP, 2, PF1, 0, pDC, isNoImage, 1))) {
		goto POUT41;
	}

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	// 1-B
//----->('23.05.01 cor)
	//m_Arith.l_div(WORK0, m_pSnFhyo6_01Data->Sn_F601_1B, PW0);
	//m_Arith.l_print(dp, WORK0, FMT110_EX);
	//strcat_s(dp, 18 + 1, _T("   "));
//------
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_1B, FMT15_EX);
//<-----
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 2-B
//----->('23.05.01 cor)
//	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_2B, FMT15_EX);
//------
	m_Arith.l_div(WORK0, m_pSnFhyo6_01Data->Sn_F601_2B, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, 18 + 1, _T("   "));
//<-----
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 3-B
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_3B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-B
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_4B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-B
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_5B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-B
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_6B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 7-B
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_7B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 8-B
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_8B, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if (j = _xprn(PP, 3, PF1, 0, pDC, isNoImage, 1)) {
		goto POUT41;
	}

	dp = AP;
	i = j = 0;
	memset(dp, '\0', size);
	memset(WORK0, '\0', MONY_BUF_SIZE);

	// 1-C
//----->('23.05.01 cor)
	//m_Arith.l_div(WORK0, m_pSnFhyo6_01Data->Sn_F601_1C, PW0);
	//m_Arith.l_print(dp, WORK0, FMT110_EX);
	//strcat_s(dp, (18 + 1), _T("   "));
//------
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_1C, FMT15_EX);
//<-----
	PF1[i].PITM[j] = dp;
	dp += 19;
	i++;
	j = 0;

	// 2-C
//----->('23.05.01 cor)
//	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_2C, FMT15_EX);
//------
	m_Arith.l_div(WORK0, m_pSnFhyo6_01Data->Sn_F601_2C, PW0);
	m_Arith.l_print(dp, WORK0, FMT110_EX);
	strcat_s(dp, (18 + 1), _T("   "));
//<-----
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 3-C
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_3C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 4-C
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_4C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 5-C
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_5C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// 6-C
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_6C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 7-C
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_7C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;
	i++;
	j = 0;

	// 8-C
	m_Arith.l_print(dp, m_pSnFhyo6_01Data->Sn_F601_8C, FMT15_EX);
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if ((j = _xprn(PP, 4, PF1, 0, pDC, isNoImage, 1))) {
		goto POUT41;
	}

POUT41:
	if (AP) {
		delete AP;
		AP = NULL;
	}

	return j;
}

	// 付表６ページ取得
int CH31HyoView::GetMxListFhyo6()
{
	int CULM_PER_PAGE = 4;
	int ret = 0;
	int max = 0;
	for (int cnt = 0; cnt < SOZOKU_MAX; cnt++) {
		if (m_pSnFhyo6Data->Sn_NO[cnt] + 1 > 0) { //Sn_NOは０オリジンなので+1
			max = m_pSnFhyo6Data->Sn_NO[cnt] + 1;
		}
	}
	if (max%CULM_PER_PAGE) {
		ret = (max / CULM_PER_PAGE) + 1;
	}
	else {
		ret = (max / CULM_PER_PAGE);
	}

	if (ret == 0) { //データ０でも１ページ出しとく
		ret = 1;
	}

	return ret;
}

int CH31HyoView::PrintKosei_Hojin(CDC* pDC)
{
	int		dp, st;

	char	ZHnam[128] = { 0 };		// 税理士法人名
	char	ZName[128] = { 0 };		// 税理士氏名
	memcpy(ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam));
	memcpy(ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam));

	BOOL isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力　有
	}
	else {
		isNoImage = TRUE;	// 　　〃　　　　無
	}

	char	*BP = NULL;
	if ((BP = new char[KSK_BUF_SIZE]) == NULL) {
		return -1;
	}
	st = 0;

	int Pno = 0;
	if( m_pHjnKskEX ){
		//Pno = 15;
		Pno = 55;
	}
	else if( m_pHjnKskEX2 ){	// 更正の請求/新様式対応
		Pno = 56;
	}

	for (dp = 1; dp <= 6; ++dp) {
		switch (dp) {
		case	1:	st = PrintKosei_HojinSub1(dp, pDC, BP, isNoImage, Pno);		break;
		case	2:	st = PrintKosei_HojinSub2(dp, pDC, BP, isNoImage, Pno);		break;
		case	3:	st = PrintKosei_HojinSub3(dp, pDC, BP, isNoImage, Pno);		break;
		case	4:	st = PrintKosei_HojinSub4(dp, pDC, BP, isNoImage, Pno);		break;
		case	5:	st = PrintKosei_HojinSub5(dp, pDC, BP, isNoImage, Pno);		break;
		case	6:	st = PrintKosei_HojinSub6(dp, pDC, BP, isNoImage, Pno);		break;
		}
		if (st)	break;
	}

	if (BP) {
		delete BP;
		BP = NULL;
	}

	//電子申告完了済 
	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn)) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIHad;

		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 2L) + yPlus;
		//		long XPosition = ( PH *  17L );
		long XPosition = (PH * 7L) + (PH / 2) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, "ＭＳ　ゴシック");
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar))	return(1);

		YPosition = (PV1 * 2L) + yPlus;;
		//		XPosition = ( PH *  25L );
		XPosition = (PH * 15L) + (PH / 2) + xPlus;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar))	return(1);

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);

	}

	return(st);

	return 0;
}

int CH31HyoView::PrintKosei_HojinSub1(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	int		i, j, k, sw;
	char	buf[128];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 税務署名
	::ZeroMemory(buf, sizeof(buf));
	m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_ZMSYO, buf, 12);
	memmove(BP1, buf, 12);
	PF[i++].PITM[0] = BP1;	BP1 += (12 + 1);

	if (!m_pPrintCmInfo->OP3) {
		// 郵便番号
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_ZIP, buf, 10);
		memmove(BP1, buf, 10);
		PF[i++].PITM[0] = BP1;	BP1 += (10 + 1);

		// 納税地
		for (j = 0; j < 2; j++) {
			::ZeroMemory(buf, sizeof(buf));
			m_Util.LineSpaceSet((char*)&m_pSnHeadData->Sn_NOZEI[40 * j], buf, 40);
			memmove(BP1, buf, 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
	}
	else {
		PF[i++].PITM[0] = (char *)0;
		PF[i].PITM[0] = (char *)0;
		PF[i].PITM[1] = (char *)0;
	}
	i++;

	// 納税地電話番号
	if (!m_pPrintCmInfo->OPE) {
		/*::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_TELNO1, buf, 6);
		memmove(BP1, buf, 6);
		PF[i++].PITM[0] = BP1;	BP1 += (6 + 1);
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_TELNO2, buf, 4);
		memmove(BP1, buf, 4);
		PF[i++].PITM[0] = BP1;	BP1 += (4 + 1);
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_TELNO3, buf, 4);
		memmove(BP1, buf, 4);
		PF[i++].PITM[0] = BP1;	BP1 += (4 + 1);*/

		::ZeroMemory ( buf, sizeof ( buf ) );
		TelBufSet ( buf, 1 );
		int len = ( int )strlen ( buf );
		memmove ( BP1, buf, len );
		PF[i++].PITM[0] = BP1;
		BP1 += ( len + 1 );
	}
	else {
		PF[i++].PITM[0] = (char *)0;
		//PF[i++].PITM[0] = (char *)0;
		//PF[i++].PITM[0] = (char *)0;
	}
	if (!m_pPrintCmInfo->OPF) {
		// 法人名 フリガナ
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_COFRI, buf, 80);
		memmove(BP1, buf, 80);
		PF[i++].PITM[0] = BP1;	BP1 += (80 + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}

	// 法人名
	if (!m_pPrintCmInfo->OP4) {
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_CONAM, buf, 40);
		memmove(BP1, buf, 40);
		PF[i++].PITM[0] = BP1;	BP1 += (40 + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}


	CICSMNSub8 mnsub;
	CString myno_str, myno_prn;

	if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
		if (m_pPset->psSigCoprn & 0x4000) {
			myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
		}
		else {
			myno_prn = myno_str;
		}
		memmove(BP1, myno_prn, 13);
		PF[i++].PITM[0] = BP1; BP1 += (13 + 1);
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}


	// 代表者（振りがな）
// 改良依頼 20-0352 修正 del -->
	//if (!m_pPrintCmInfo->OPG) {
// 改良依頼 20-0352 修正 del <--
// 改良依頼 20-0352 修正 add -->
	if ( !(m_pPrintCmInfo->OPW & 0x08) ) {
// 改良依頼 20-0352 修正 add <--
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_DIFRI, buf, 80);
		memmove(BP1, buf, 80);
		PF[i++].PITM[0] = BP1;	BP1 += (80 + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}

	// 代表者名
// 改良依頼 20-0352 修正 del -->
	//if (!m_pPrintCmInfo->OP5) {
// 改良依頼 20-0352 修正 del <--
// 改良依頼 20-0352 修正 add -->
	if ( !(m_pPrintCmInfo->OPW & 0x04) ) {
// 改良依頼 20-0352 修正 add <--
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_DIHYO, buf, 40);
		memmove(BP1, buf, 40);
		PF[i++].PITM[0] = BP1;	BP1 += (40 + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}

	// 課税期間
	k = i;
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);

	char	yymmdd[3] = { 0 };
	int gengo1 = 0, gengo2 = 0;
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);

	if (m_pSnHeadData->Sn_Syuusei & 0x04)	i += 2;
	for (j = 0; j < 3; j++, BP1 += 3) {
		sw = 0;
		if (j == 0) {
			if (yymmdd[j])	sw = 1;
			else {
				if (yymmdd[j + 1])	sw = 1;
			}
		}
		else {
			if (yymmdd[j])	sw = 1;
		}
		if (sw)	sprintf_s(BP1, 3, "%2x", yymmdd[j]);
		else		memset(BP1, '\0', 2);
		PF[i].PITM[j] = BP1;
	}

	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = 0; j < 3; j++, BP1 += 3) {
		sw = 0;
		if (j == 0) {
			if (yymmdd[j])	sw = 1;
			else {
				if (yymmdd[j + 1])	sw = 1;
			}
		}
		else {
			if (yymmdd[j])	sw = 1;
		}
		if (sw)
			sprintf_s(BP1, 3, "%2x", yymmdd[j]);
		else
			memset(BP1, '\0', 2);
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 元号が令和固定となったため、〇の出力は旧様式のみ
	if( m_pHjnKskEX ){
		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = (char*) 0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char* )0;
			PF[i].PITM[1] = (char* )0;
		}
		i++;

		if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = (char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*) 0;
			PF[i].PITM[1] = (char*) 0;
		}
		i++;
	}

	if (m_pSnHeadData->Sn_Syuusei & 0x04)
		return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0202));
	else	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_HojinSub2(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	int		i, j, k, x, sw;

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 提出年月日
	for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
		sw = 0;
		if (x == 0) {
			if (m_pSnKskData->KsData.KSdata[x])	sw = 1;
			else {
				if (m_pSnKskData->KsData.KSdata[x + 1])	sw = 1;
			}
		}
		else {
			if (m_pSnKskData->KsData.KSdata[x])	sw = 1;
		}
		if (sw)	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSdata[x]);
		else		memset(BP1, '\0', 2);
		PF[i].PITM[j] = BP1;
	}
	i++;

	k = i;
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	pbclr(0, 30, &PF[k++]);
	if (m_pSnHeadData->Sn_Syuusei & 0x04) {
		i += 4;
		// 更正決定通知書日付
		for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (m_pSnKskData->KsData.KSktdt[x])	sw = 1;
				else {
					if (m_pSnKskData->KsData.KSktdt[x + 1])	sw = 1;
				}
			}
			else {
				if (m_pSnKskData->KsData.KSktdt[x])	sw = 1;
			}
			if (sw)	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]);
			else		memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
		i++;

		// 申告、更正、決定
		for (j = 0; j < 3; j++, BP1 += 2) {
			if (j == m_pSnKskData->KsData.KSktsw)	sprintf_s(BP1, 2, " ");
			else						memset(BP1, '\0', 1);
			PF[i].PITM[j] = BP1;
		}

		return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0202));

	}
	else {
		// 更正決定通知書日付
		for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (m_pSnKskData->KsData.KSktdt[x])	sw = 1;
				else {
					if (m_pSnKskData->KsData.KSktdt[x + 1])	sw = 1;
				}
			}
			else {
				if (m_pSnKskData->KsData.KSktdt[x])	sw = 1;
			}
			if (sw)	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]);
			else		memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
		i++;

		// 元号が令和固定となったため、〇の出力は旧様式のみ
		if( m_pHjnKskEX ){
			if ( m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_REIWA_GENGO ) {
				memmove ( BP1, "    ", 4 );
				PF[i].PITM[0] = ( char* )0;
				PF[i].PITM[1] = BP1;
				BP1 += 5;
			}
			else if ( m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_HEISEI_GENGO ) {
				memmove ( BP1, "    ", 4 );
				PF[i].PITM[0] = BP1;
				PF[i].PITM[1] = ( char* )0;
				BP1 += 5;
			}
			else {
				PF[i].PITM[0] = ( char* )0;
				PF[i].PITM[1] = ( char* )0;
			}
			i++;
		}

		// 申告、更正、決定
		for (j = 0; j < 3; j++, BP1 += 2) {
			if (j == m_pSnKskData->KsData.KSktsw)	sprintf_s(BP1, 2, " ");
			else						memset(BP1, '\0', 1);
			PF[i].PITM[j] = BP1;
		}
		i++;

		for (j = 0; j < 2; j++, BP1 += 2) {
			if (j + 1 == m_pSnKskData->KsData.KSthsw)	sprintf_s(BP1, 2, " ");
			else									memset(BP1, '\0', 1);
			PF[i].PITM[j] = BP1;
		}
		i++;
		if (m_pSnKskData->KsData.KSthsw == 3)		sprintf_s(BP1, 2, " ");
		else									memset(BP1, '\0', 1);
		PF[i].PITM[0] = BP1;

		return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
	}
}

int CH31HyoView::PrintKosei_HojinSub3(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF PF[30];
	memset(PF, '\0', sizeof(PF));

	int	PFidx = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	if( m_pHjnKskEX ){

		// この請求前の金額
		for ( int x = 0; x < 10; x++ ) {
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x].KVsval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove( BP1, buf, len );
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		for ( int x = 0; x < 9; x++ ) { // KSval[10]-[19]
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x+10].KVsval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		// 更正の請求金額
		for ( int x = 0; x < 10; x++ ) {
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x].KVkval, FMT16_EX );	
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		for ( int x = 0; x < 9; x++ ) {	// KSval[10]-[19]
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x+10].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;
	}
	else if( m_pHjnKskEX2 ){	// 更正の請求/新様式対応

		// 更正の請求金額
		// 1～12
		for ( int x = 0; x < 12; x++ ) {
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		// 13
		{
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[20].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[0] = BP1;
			BP1 += (len + 1);
			PFidx++;
		}

		// 14～20
		for ( int x = 0; x < 7; x++ ) {	// KSval[12]-[19]
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x+12].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		// 21
		{
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[21].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[0] = BP1;
			BP1 += (len + 1);
			PFidx++;
		}
	}

	return(_xprn(PPno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_HojinSub4(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	int		i, j, x, sw;
	char	buf[256];
	int		len;

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 更正の請求をする理由等
	for (j = 0, len = 80; j < 3; j++) {
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet(&m_pSnKskData->KsData.KSreas[len*j], buf, len);
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 修正申告提出年月日
	for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
		sw = 0;
		if (x == 0) {
			if (m_pSnKskData->KsData.KSstdt[x])	sw = 1;
			else {
				if (m_pSnKskData->KsData.KSstdt[x + 1])	sw = 1;
			}
		}
		else {
			if (m_pSnKskData->KsData.KSstdt[x])	sw = 1;
		}
		if (sw)	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSstdt[x]);
		else		memset(BP1, '\0', 2);
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 元号が令和固定となったため、〇の出力は旧様式のみ
	if( m_pHjnKskEX ){
		if ( m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = ( char* )0;
		}
		i++;
	}

	// 更正決定通知書受理年月日
	for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
		sw = 0;
		if (x == 0) {
			if (m_pSnKskData->KsData.KSjrdt[x])	sw = 1;
			else {
				if (m_pSnKskData->KsData.KSjrdt[x + 1])	sw = 1;
			}
		}
		else {
			if (m_pSnKskData->KsData.KSjrdt[x])	sw = 1;
		}
		if (sw)	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSjrdt[x]);
		else		memset(BP1, '\0', 2);
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 元号が令和固定となったため、〇の出力は旧様式のみ
	if( m_pHjnKskEX ){
		if ( m_pSnKskData->KsData.KSjrdt[0] == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( m_pSnKskData->KsData.KSjrdt[0] == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = ( char* )0;
		}
		i++;
	}

	// 添付書類
	for (j = 0, len = 30; j < 2; j++) {
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet(&m_pSnKskData->KsData.KSdocu[len*j], buf, len);
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_HojinSub5(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	int		i, j, len;
	char	st1[128], st2[128], cbf[256];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;
	memset(cbf, ' ', sizeof(cbf));

	if (!m_pSnKskData->KsData.KSbpsw) {	// 銀行
	// 銀行等名称
		::ZeroMemory(st1, sizeof(st1));
		::ZeroMemory(st2, sizeof(st2));
		m_Util.LineStrSet(m_pSnKskData->KsData.KSbank.Bname, st1, st2, 10, 12);
		if ((int)strlen(st2)) {
			len = (int)strlen(st1);
			memmove(BP1, st1, len);
			PF[i].PITM[0] = BP1;	BP1 += (len + 1);
			len = (int)strlen(st2);
			memmove(BP1, st2, len);
			PF[i++].PITM[1] = BP1;	BP1 += (len + 1);
			memmove(BP1, cbf, 20);
			PF[i++].PITM[0] = BP1;	BP1 += (20 + 1);
		}
		else {
			memmove(BP1, cbf, 20);
			PF[i].PITM[0] = BP1;	BP1 += (20 + 1);
			memmove(BP1, cbf, 20);
			PF[i++].PITM[1] = BP1;	BP1 += (20 + 1);
			len = (int)strlen(st1);
			memmove(BP1, st1, len);
			PF[i++].PITM[0] = BP1;	BP1 += (len + 1);
		}

		// 銀行等区分
		if ((int)strlen(m_pSnKskData->KsData.KSbank.Bname)) {

			if (!m_pSnKskData->KsData.KSbank.Bkbn)	sprintf_s(BP1, 2, " ");
			else								memset(BP1, '\0', 1);
			PF[i].PITM[0] = BP1;
			BP1 += 2;
			i++;

			for (j = 0; j < 2; j++, BP1 += 2) {
				if (j + 1 == m_pSnKskData->KsData.KSbank.Bkbn)	sprintf_s(BP1, 2, " ");
				else									memset(BP1, '\0', 1);
				PF[i].PITM[j] = BP1;
			}
			i++;

			for (j = 0; j < 2; j++, BP1 += 2) {
				if (j + 3 == m_pSnKskData->KsData.KSbank.Bkbn)	sprintf_s(BP1, 2, " ");
				else										memset(BP1, '\0', 1);
				PF[i].PITM[j] = BP1;
			}
			i++;
		}
		else {

			memset(BP1, '\0', 1);
			PF[i].PITM[0] = BP1;
			BP1 += 2;
			i++;

			for (j = 0; j < 2; j++, BP1 += 2) {
				memset(BP1, '\0', 1);
				PF[i].PITM[j] = BP1;
			}
			i++;
			for (j = 0; j < 2; j++, BP1 += 2) {
				memset(BP1, '\0', 1);
				PF[i].PITM[j] = BP1;
			}
			i++;
		}

		// 本支店名称
		::ZeroMemory(st1, sizeof(st1));
		::ZeroMemory(st2, sizeof(st2));
		m_Util.LineStrSet(m_pSnKskData->KsData.KSbank.Sname, st1, st2, 10, 12);
		if ((int)strlen(st2)) {
			len = (int)strlen(st1);
			memmove(BP1, st1, len);
			PF[i].PITM[0] = BP1;	BP1 += (len + 1);
			len = (int)strlen(st2);
			memmove(BP1, st2, len);
			PF[i++].PITM[1] = BP1;	BP1 += (len + 1);
			memmove(BP1, cbf, 20);
			PF[i++].PITM[0] = BP1;	BP1 += (20 + 1);
		}
		else {
			memmove(BP1, cbf, 20);
			PF[i].PITM[0] = BP1;	BP1 += (20 + 1);
			memmove(BP1, cbf, 20);
			PF[i++].PITM[1] = BP1;	BP1 += (20 + 1);
			len = (int)strlen(st1);
			memmove(BP1, st1, len);
			PF[i++].PITM[0] = BP1;	BP1 += (len + 1);
		}

		for (j = 0; j < 2; j++, BP1 += 2) {
			if (j + 1 == m_pSnKskData->KsData.KSbank.Skbn) {
				sprintf_s(BP1, 2, " ");
			}
			else {
				memset(BP1, '\0', 1);
			}
			PF[i].PITM[j] = BP1;
		}
		i++;
		for (j = 0; j < 2; j++, BP1 += 2) {
			if (j + 3 == m_pSnKskData->KsData.KSbank.Skbn) {
				sprintf_s(BP1, 2, " ");
			}
			else {
				memset(BP1, '\0', 1);
			}
			PF[i].PITM[j] = BP1;
		}
		i++;
		if (5 == m_pSnKskData->KsData.KSbank.Skbn) {
			sprintf_s(BP1, 2, " ");
		}
		else {
			memset(BP1, '\0', 1);
		}
		PF[i].PITM[0] = BP1;
		BP1 += 2;
		i++;
		//---------------

			// 預金
		memmove(BP1, m_pSnKskData->KsData.KSbank.Dname, 12);
		PF[i++].PITM[0] = BP1;	BP1 += (12 + 1);

		// 口座番号
		memmove(BP1, m_pSnKskData->KsData.KSbank.Accnt, 10);
		PF[i++].PITM[0] = BP1;	BP1 += (10 + 1);
	}
	else {
		for (i = 0; i != 30; i++) {
			pbclr(0, 30, &PF[i]);
		}
	}

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_HojinSub6(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	char	ZHnam[128] = { 0 };		// 税理士法人名
	char	ZName[128] = { 0 };		// 税理士氏名
	memcpy(ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam));
	memcpy(ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam));

	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	int		i, len;
	char	buf[256], cbf[256];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;
	memset(cbf, ' ', sizeof(cbf));

	// 郵便局
		// 口座番号1
	if (!m_pSnKskData->KsData.KSbpsw)	memmove(BP1, cbf, 5);
	else					memmove(BP1, m_pSnKskData->KsData.KSpost.Acnt1, 5);
	PF[i++].PITM[0] = BP1;	BP1 += (5 + 1);
	// 口座番号2
	if (!m_pSnKskData->KsData.KSbpsw)	memmove(BP1, cbf, 8);
	else					memmove(BP1, m_pSnKskData->KsData.KSpost.Acnt2, 8);
	PF[i++].PITM[0] = BP1;	BP1 += (8 + 1);
	// 郵便局名称
	if (!m_pSnKskData->KsData.KSbpsw)	memmove(BP1, cbf, 12);
	else					memmove(BP1, m_pSnKskData->KsData.KSpost.Pname, 16);
	PF[i++].PITM[0] = BP1;	BP1 += (16 + 1);

	// 税理士名
	char	zbuf[256];
	::ZeroMemory(zbuf, sizeof(zbuf));
	::ZeroMemory(buf, sizeof(buf));
	if (m_pPrintCmInfo->OP2) {
		memmove(BP1, "\0", 40);
		PF[i].PITM[0] = BP1;	BP1 += (40 + 1);
	}
	else {
		char	tmpbuf[256] = { 0 };
		MakeOutZeirishiName(tmpbuf, sizeof(tmpbuf));
		if (m_pPrintCmInfo->OPA == 2) {	// 法人名 & 氏名
			sprintf_s(zbuf, sizeof(zbuf), _T("%s　%s"), ZHnam, tmpbuf);
		}
		else {
			memmove(zbuf, tmpbuf, sizeof(zbuf));
		}
		len = (int)strlen(zbuf);
		memmove(BP1, zbuf, len);
		PF[i].PITM[0] = BP1;	BP1 += (len + 1);
	}

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}


int CH31HyoView::PrintKosei_Kojin(CDC* pDC)
{
	int		dp, st;

	char	ZHnam[128] = { 0 };		// 税理士法人名
	char	ZName[128] = { 0 };		// 税理士氏名
	memmove(ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam));
	memmove(ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam));

	BOOL	isNoImage = FALSE;
	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力　有
	}
	else {
		isNoImage = TRUE;	// 　　〃　　　　無
	}

	char	*BP = NULL, *BP1 = NULL;

	if ((BP = new char[KSK_BUF_SIZE]) == NULL) {
		return -1;
	}

	BP1 = BP;
	l_defn(0x16);
	st = 0;

	int Pno = 0;
	if( m_pKjnKskEX ){
		//Pno = 16;
		Pno = 55;
	}
	else if( m_pKjnKskEX2 ){	// 更正の請求/新様式対応
		Pno = 56;
	}

	for (dp = 1; dp <= 6; ++dp) {
		switch (dp) {
		case	1:	st = PrintKosei_KojinSub1(dp, pDC, BP1, isNoImage, Pno);		break;
		case	2:	st = PrintKosei_KojinSub2(dp, pDC, BP1, isNoImage, Pno);		break;
		case	3:	st = PrintKosei_KojinSub3(dp, pDC, BP1, isNoImage, Pno);		break;
		case	4:	st = PrintKosei_KojinSub4(dp, pDC, BP1, isNoImage, Pno);		break;
		case	5:	st = PrintKosei_KojinSub5(dp, pDC, BP1, isNoImage, Pno);		break;
		case	6:	st = PrintKosei_KojinSub6(dp, pDC, BP1, isNoImage, Pno);		break;
		}
		if (st)	break;
	}

	if (BP) {
		delete BP;
		BP = NULL;
	}

	//電子申告完了済 
	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn)) {
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIHad;

		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 2L) + yPlus;
		//		long XPosition = ( PH *  17L );
		long XPosition = (PH * 9L) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, _T("ＭＳ　ゴシック"));
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, _T("ＭＳ　ゴシック"));
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar))	return(1);

		YPosition = (PV1 * 2L) + yPlus;
		//		XPosition = ( PH *  25L );
		XPosition = (PH * 17L) + xPlus;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar)) {
			return(1);
		}

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);

	}

	return(st);

	return 0;
}

int CH31HyoView::PrintKosei_KojinSub1(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	int		i, j, sw;
	char	buf[256];
	int		len;

	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 税務署名
	::ZeroMemory(buf, sizeof(buf));
	m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_ZMSYO, buf, 12);
	memmove(BP1, buf, 12);
	PF[i++].PITM[0] = BP1;
	BP1 += (12 + 1);

	if (!m_pPrintCmInfo->OP3) {
		// 郵便番号
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_ZIP, buf, 10);
		memmove(BP1, buf, 10);
		PF[i++].PITM[0] = BP1;	BP1 += (10 + 1);

		// 納税地
		for (j = 0; j < 2; j++) {
			::ZeroMemory(buf, sizeof(buf));
			m_Util.LineSpaceSet((char*)&m_pSnHeadData->Sn_NOZEI[40 * j], buf, 40);
			memmove(BP1, buf, 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
	}
	else {
		PF[i++].PITM[0] = (char *)0;
		PF[i].PITM[0] = (char *)0;
		PF[i].PITM[1] = (char *)0;
	}
	i++;

	// 納税地電話番号
	if (!m_pPrintCmInfo->OPE) {
		::ZeroMemory(buf, sizeof(buf));
		TelBufSet(buf, 1);
		len = (int)strlen(buf);
		memmove(BP1, buf, len);
		PF[i++].PITM[0] = BP1;	BP1 += (len + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}

	// 代表者（振りがな）
// 改良依頼 20-0352 修正 del -->
	//if (!m_pPrintCmInfo->OPG) {
// 改良依頼 20-0352 修正 del <--
// 改良依頼 20-0352 修正 add -->
	if ( !(m_pPrintCmInfo->OPW & 0x08) ) {
// 改良依頼 20-0352 修正 add <--
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_DIFRI, buf, 80);
		memmove(BP1, buf, 80);
		PF[i++].PITM[0] = BP1;	BP1 += (80 + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}

	// 代表者名
// 改良依頼 20-0352 修正 del -->
	//if (!m_pPrintCmInfo->OP5) {
// 改良依頼 20-0352 修正 del <--
// 改良依頼 20-0352 修正 add -->
	if ( !(m_pPrintCmInfo->OPW & 0x04) ) {
// 改良依頼 20-0352 修正 add <--
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet((char*)m_pSnHeadData->Sn_DIHYO, buf, 40);
		memmove(BP1, buf, 40);
		PF[i++].PITM[0] = BP1;	BP1 += (40 + 1);
	}
	else {
		PF[i++].PITM[0] = (char *)0;
	}


	CICSMNSub8 mnsub;
	CString myno_str, myno_prn;

	if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
		if (m_pPset->psSigCoprn & 0x4000) {
			myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
		}
		else {
			myno_prn = myno_str;
		}
		memmove(BP1, myno_prn, 12);
		PF[i++].PITM[0] = BP1; BP1 += (12 + 1);
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}

	// 課税期間
	char	yymmdd[3] = { 0 };
	int		gengo1 = 0,
			gengo2 = 0;
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = 0; j < 3; j++, BP1 += 3) {
		sw = 0;
		if (j == 0) {
			if (yymmdd[j]) {
				sw = 1;
			}
			else {
				if (yymmdd[j + 1]) {
					sw = 1;
				}
			}
		}
		else {
			if (yymmdd[j]) {
				sw = 1;
			}
		}
		if (sw) {
			sprintf_s(BP1, 3, "%2x", yymmdd[j]);
		}
		else {
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}

	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = 0; j < 3; j++, BP1 += 3) {
		sw = 0;
		if (j == 0) {
			if (yymmdd[j]) {
				sw = 1;
			}
			else {
				if (yymmdd[j + 1]) {
					sw = 1;
				}
			}
		}
		else {
			if (yymmdd[j]) {
				sw = 1;
			}
		}
		if (sw) {
			sprintf_s(BP1, 3, "%2x", yymmdd[j]);
		}
		else {
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 元号が令和固定となったため、〇の出力は旧様式のみ
	if( m_pKjnKskEX ){
		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = ( char* )0;
		}
		i++;

		if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = ( char* )0;
		}
		i++;
	}

	if (m_pSnHeadData->Sn_Syuusei & 0x04) {
		return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0202));
	}
	else {
		return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
	}
}

int CH31HyoView::PrintKosei_KojinSub2(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	int		i, j, x, sw;

	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;
	int Pno = PPno;

	// 提出年月日
	for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
		sw = 0;
		if (x == 0) {
			if (m_pSnKskData->KsData.KSdata[x]) {
				sw = 1;
			}
			else {
				if (m_pSnKskData->KsData.KSdata[x + 1]) {
					sw = 1;
				}
			}
		}
		else {
			if (m_pSnKskData->KsData.KSdata[x]) {
				sw = 1;
			}
		}
		if (sw) {
			sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSdata[x]);
		}
		else {
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 更正決定通知書日付
	for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
		sw = 0;
		if (x == 0) {
			if (m_pSnKskData->KsData.KSktdt[x]) {
				sw = 1;
			}
			else {
				if (m_pSnKskData->KsData.KSktdt[x + 1]) {
					sw = 1;
				}
			}
		}
		else {
			if (m_pSnKskData->KsData.KSktdt[x]) {
				sw = 1;
			}
		}
		if (sw) {
			sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]);
		}
		else {
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 元号が令和固定となったため、〇の出力は旧様式のみ
	if( m_pKjnKskEX ){
		if ( m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = ( char* )0;
		}
		i++;
	}

	// 申告、更正、決定
	for (j = 0; j < 3; j++, BP1 += 2) {
		if (j == m_pSnKskData->KsData.KSktsw) {
			sprintf_s(BP1, 2, " ");
		}
		else {
			memset(BP1, '\0', 1);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_KojinSub3(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF PF[30];
	memset(PF, '\0', sizeof(PF));

	int	PFidx = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	if( m_pKjnKskEX ){

		// この請求前の金額
		for ( int x = 0; x < 10; x++ ) {
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x].KVsval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove( BP1, buf, len );
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		for ( int x = 0; x < 9; x++ ) { // KSval[10]-[19]
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x+10].KVsval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		// 更正の請求金額
		for ( int x = 0; x < 10; x++ ) {
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x].KVkval, FMT16_EX );	
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		for ( int x = 0; x < 9; x++ ) {	// KSval[10]-[19]
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x+10].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;
	}
	else if( m_pKjnKskEX2 ){	// 更正の請求/新様式対応

		// 更正の請求金額
		// 1～12
		for ( int x = 0; x < 12; x++ ) {
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		// 13
		{
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[20].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[0] = BP1;
			BP1 += (len + 1);
			PFidx++;
		}

		// 14～20
		for ( int x = 0; x < 7; x++ ) {	// KSval[12]-[19]
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[x+12].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[x] = BP1;
			BP1 += (len + 1);
		}
		PFidx++;

		// 21
		{
			char buf[128] = {0};
			m_Arith.l_print( buf, &m_pSnKskData->KsData.KSval[21].KVkval, FMT16_EX );
			int len = (int)strlen(buf);
			memmove(BP1, buf, len);
			PF[PFidx].PITM[0] = BP1;
			BP1 += (len + 1);
			PFidx++;
		}
	}

	return(_xprn(PPno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_KojinSub4(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	int		i, j, x, sw;
	char	buf[256];
	int		len;

	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 更正の請求をする理由等
	for (j = 0, len = 80; j < 3; j++) {
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet(&m_pSnKskData->KsData.KSreas[len*j], buf, len);
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 修正申告提出年月日又は更正決定通知書受理年月日
	for (x = 1, j = 0; x < 4; j++, x++, BP1 += 3) {
		sw = 0;
		if (x == 0) {
			if (m_pSnKskData->KsData.KSstdt[x])	sw = 1;
			else {
				if (m_pSnKskData->KsData.KSstdt[x + 1])	sw = 1;
			}
		}
		else {
			if (m_pSnKskData->KsData.KSstdt[x])	sw = 1;
		}
		if (sw)	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSstdt[x]);
		else		memset(BP1, '\0', 2);
		PF[i].PITM[j] = BP1;
	}
	i++;

	// 元号が令和固定となったため、〇の出力は旧様式のみ
	if( m_pKjnKskEX ){
		if ( m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if ( m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = ( char* )0;
			PF[i].PITM[1] = ( char* )0;
		}
		i++;
	}

	// 添付書類
	for (j = 0, len = 30; j < 2; j++) {
		::ZeroMemory(buf, sizeof(buf));
		m_Util.LineSpaceSet(&m_pSnKskData->KsData.KSdocu[len*j], buf, len);
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	if( m_pKjnKskEX ){
		// 確定額（　　額）
		::ZeroMemory(buf, sizeof(buf));
		switch (m_pSnKskData->KsData.KSktsw) {
		case 0:	strcpy_s(buf, sizeof(buf), "申　告");	break;
		case 1:	strcpy_s(buf, sizeof(buf), "更　正");	break;
		case 2:	strcpy_s(buf, sizeof(buf), "決　定");	break;
		}
		len = (int)strlen(buf);
		memmove(BP1, buf, len);
		PF[i++].PITM[0] = BP1;	BP1 += (len + 1);
	}

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_KojinSub5(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	int		i, j, len;
	char	st1[128], st2[128], cbf[256];

	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;
	memset(cbf, ' ', sizeof(cbf));

	// 課税期間の取得
	long symd = 0, eymd = 0;
	GetKazeiKikan( symd, eymd );

	if( (eymd >= KSKMODIFY) && (m_pSnHeadData->Sn_GenericSgn & 0x1000) ){	// 公金受取口座にチェックがある場合は金融機関を出力しない
		memmove( BP1, "レ", 10 );
		PF[12].PITM[0] = BP1;
	}
	else{
		if (!m_pSnKskData->KsData.KSbpsw) {	// 銀行
			// 銀行等名称
			::ZeroMemory(st1, sizeof(st1));
			::ZeroMemory(st2, sizeof(st2));
			m_Util.LineStrSet(m_pSnKskData->KsData.KSbank.Bname, st1, st2, 10, 12);
			if ((int)strlen(st2)) {
				len = (int)strlen(st1);
				memmove(BP1, st1, len);
				PF[i].PITM[0] = BP1;	BP1 += (len + 1);
				len = (int)strlen(st2);
				memmove(BP1, st2, len);
				PF[i++].PITM[1] = BP1;	BP1 += (len + 1);
				memmove(BP1, cbf, 20);
				PF[i++].PITM[0] = BP1;	BP1 += (20 + 1);
			}
			else {
				memmove(BP1, cbf, 20);
				PF[i].PITM[0] = BP1;	BP1 += (20 + 1);
				memmove(BP1, cbf, 20);
				PF[i++].PITM[1] = BP1;	BP1 += (20 + 1);
				len = (int)strlen(st1);
				memmove(BP1, st1, len);
				PF[i++].PITM[0] = BP1;	BP1 += (len + 1);
			}

			// 銀行等区分
			if ((int)strlen(m_pSnKskData->KsData.KSbank.Bname)) {

				if (!m_pSnKskData->KsData.KSbank.Bkbn)	sprintf_s(BP1, 2, " ");
				else								memset(BP1, '\0', 1);
				PF[i].PITM[0] = BP1;
				BP1 += 2;
				i++;

				for (j = 0; j < 2; j++, BP1 += 2) {
					if (j + 1 == m_pSnKskData->KsData.KSbank.Bkbn)	sprintf_s(BP1, 2, " ");
					else										memset(BP1, '\0', 1);
					PF[i].PITM[j] = BP1;
				}
				i++;

				for (j = 0; j < 2; j++, BP1 += 2) {
					if (j + 3 == m_pSnKskData->KsData.KSbank.Bkbn)	sprintf_s(BP1, 2, " ");
					else										memset(BP1, '\0', 1);
					PF[i].PITM[j] = BP1;
				}
				i++;
			}
			else {

				memset(BP1, '\0', 1);
				PF[i].PITM[0] = BP1;
				BP1 += 2;
				i++;

				for (j = 0; j < 2; j++, BP1 += 2) {
					memset(BP1, '\0', 1);
					PF[i].PITM[j] = BP1;
				}
				i++;
				for (j = 0; j < 2; j++, BP1 += 2) {
					memset(BP1, '\0', 1);
					PF[i].PITM[j] = BP1;
				}
				i++;
			}

			// 本支店名称
			::ZeroMemory(st1, sizeof(st1));
			::ZeroMemory(st2, sizeof(st2));
			m_Util.LineStrSet(m_pSnKskData->KsData.KSbank.Sname, st1, st2, 10, 12);
			if ((int)strlen(st2)) {
				len = (int)strlen(st1);
				memmove(BP1, st1, len);
				PF[i].PITM[0] = BP1;	BP1 += (len + 1);
				len = (int)strlen(st2);
				memmove(BP1, st2, len);
				PF[i++].PITM[1] = BP1;	BP1 += (len + 1);
				memmove(BP1, cbf, 20);
				PF[i++].PITM[0] = BP1;	BP1 += (20 + 1);
			}
			else {
				memmove(BP1, cbf, 20);
				PF[i].PITM[0] = BP1;	BP1 += (20 + 1);
				memmove(BP1, cbf, 20);
				PF[i++].PITM[1] = BP1;	BP1 += (20 + 1);
				len = (int)strlen(st1);
				memmove(BP1, st1, len);
				PF[i++].PITM[0] = BP1;	BP1 += (len + 1);
			}

			// 本支店区分
			for (j = 0; j < 2; j++, BP1 += 2) {
				if (j + 1 == m_pSnKskData->KsData.KSbank.Skbn) {
					sprintf_s(BP1, 2, " ");
				}
				else {
					memset(BP1, '\0', 1);
				}
				PF[i].PITM[j] = BP1;
			}
			i++;
			for (j = 0; j < 2; j++, BP1 += 2) {
				if (j + 3 == m_pSnKskData->KsData.KSbank.Skbn) {
					sprintf_s(BP1, 2, " ");
				}
				else {
					memset(BP1, '\0', 1);
				}
				PF[i].PITM[j] = BP1;
			}
			i++;

			if (5 == m_pSnKskData->KsData.KSbank.Skbn) {
				sprintf_s(BP1, 2, " ");
			}
			else {
				memset(BP1, '\0', 1);
			}
			PF[i].PITM[0] = BP1;
			BP1 += 2;
			i++;
			//---------------

			// 預金
			memmove(BP1, m_pSnKskData->KsData.KSbank.Dname, 12);
			PF[i++].PITM[0] = BP1;	BP1 += (12 + 1);

			// 口座番号
			memmove(BP1, m_pSnKskData->KsData.KSbank.Accnt, 10);
			PF[i++].PITM[0] = BP1;	BP1 += (10 + 1);
		}
		else {
			for (i = 0; i != 30; i++) {
				pbclr(0, 30, &PF[i]);
			}
		}
	}

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

int CH31HyoView::PrintKosei_KojinSub6(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno)
{
	PBUF	PF[30];
	memset(PF, '\0', sizeof(PF));
	int Pno = PPno;

	int		i;
	char	buf[256], cbf[256];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;
	memset(cbf, ' ', sizeof(cbf));

	// 課税期間の取得
	long symd = 0, eymd = 0;
	GetKazeiKikan( symd, eymd );

	// 郵便局
	if( (eymd >= KSKMODIFY) && (m_pSnHeadData->Sn_GenericSgn & 0x1000) ){
		i+=4;
	}
	else{
		// 口座番号1
		if (!m_pSnKskData->KsData.KSbpsw)	memmove(BP1, cbf, 5);
		else								memmove(BP1, m_pSnKskData->KsData.KSpost.Acnt1, 5);
		PF[i++].PITM[0] = BP1;	BP1 += (5 + 1);
		::ZeroMemory(buf, sizeof(buf));
		if (!m_pSnKskData->KsData.KSbpsw)	strcpy_s(buf, sizeof(buf), "  ");
		else								strcpy_s(buf, sizeof(buf), "　");
		memmove(BP1, buf, 2);
		PF[i++].PITM[0] = BP1;	BP1 += (2 + 1);
		// 口座番号2
		if (!m_pSnKskData->KsData.KSbpsw)	memmove(BP1, cbf, 8);
		else								memmove(BP1, m_pSnKskData->KsData.KSpost.Acnt2, 8);
		PF[i++].PITM[0] = BP1;	BP1 += (8 + 1);
		// 郵便局名称
		if (!m_pSnKskData->KsData.KSbpsw)	memmove(BP1, cbf, 12);
		else								memmove(BP1, m_pSnKskData->KsData.KSpost.Pname, 16);
		PF[i++].PITM[0] = BP1;	BP1 += (16 + 1);
	}

	char	ZHnam[128] = { 0 };		// 税理士法人名
	char	ZName[128] = { 0 };		// 税理士氏名
	memmove(ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam));
	MakeOutZeirishiName(ZName, sizeof(ZName));

	// 税理士名
	if (m_pPrintCmInfo->OP2) {
		memmove(BP1, cbf, 40);
		PF[i].PITM[0] = BP1;	BP1 += (40 + 1);
		memmove(BP1, cbf, 40);
		PF[i++].PITM[1] = BP1;	BP1 += (40 + 1);
		memmove(BP1, cbf, 40);
		PF[i++].PITM[0] = BP1;	BP1 += (40 + 1);
	}
	else {
		if (m_pPrintCmInfo->OPA == 2 || (m_pPrintCmInfo->OPA == 1 && (m_pPrintCmInfo->OPsign & 0x01))) {
			m_Util.LineSpaceSet(ZHnam, buf, 40);
			memmove(BP1, buf, 40);
			PF[i].PITM[0] = BP1;	BP1 += (40 + 1);
			if (m_pPrintCmInfo->OPA == 1 && (m_pPrintCmInfo->OPsign & 0x01))
				memmove(BP1, cbf, 40);
			else {
				int slen = (int)strlen(ZName) + 1;
				memmove(BP1, ZName, slen);
			}
			PF[i++].PITM[1] = BP1;	BP1 += (40 + 1);
			memmove(BP1, cbf, 40);
			PF[i++].PITM[0] = BP1;	BP1 += (40 + 1);
		}
		else {
			memmove(BP1, cbf, 40);
			PF[i].PITM[0] = BP1;	BP1 += (40 + 1);
			memmove(BP1, cbf, 40);
			PF[i++].PITM[1] = BP1;	BP1 += (40 + 1);

			int slen = 0;
			if (m_pPrintCmInfo->OPA == 1) {
				slen = 40;
				m_Util.LineSpaceSet(ZHnam, buf, slen);
				memmove(BP1, buf, slen);
			}
			else {
				slen = (int)strlen(ZName);
				memmove(BP1, ZName, slen);
			}
			PF[i++].PITM[0] = BP1;
			BP1 += (slen + 1);
		}
	}

	return(_xprn(Pno, dp, PF, 0, pDC, isNoImage, 0x0101));
}

void  CH31HyoView::TelBufSet(char *buf, int mode)
{
	char	bf0[64], bf1[8], bf2[8], bf3[8];
	int		len;

	memset(bf1, '\0', sizeof(bf1));
	memset(bf2, '\0', sizeof(bf2));
	memset(bf3, '\0', sizeof(bf3));

	len = kjlen(m_pSnHeadData->Sn_TELNO1, 6);
	memmove(bf1, m_pSnHeadData->Sn_TELNO1, len);
	len = kjlen(m_pSnHeadData->Sn_TELNO2, 4);
	memmove(bf2, m_pSnHeadData->Sn_TELNO2, len);
	len = kjlen(m_pSnHeadData->Sn_TELNO3, 4);
	memmove(bf3, m_pSnHeadData->Sn_TELNO3, len);
	memset(bf0, '\0', sizeof(bf0));
	if (!mode)
		sprintf_s(bf0, sizeof(bf0), "(%-6s) %-4s － %-4s", bf1, bf2, bf3);
	else
		sprintf_s(bf0, sizeof(bf0), "%-6s－ %-4s － %-4s", bf1, bf2, bf3);
	len = (int)strlen(bf0);
	memmove(buf, bf0, len);
}

//-----------------------------------------------------------------------------
// 還付付表（法人）
//-----------------------------------------------------------------------------
// 引数	pDC	：	
//		pno	：	
//-----------------------------------------------------------------------------
void CH31HyoView::PrintRefund_Kojin(CDC* pDC, int pno)
{
	CRefundBaseDialog::KojinPrint(pDC, pno);
}

//-----------------------------------------------------------------------------
// 還付付表（個人）
//-----------------------------------------------------------------------------
// 引数	pDC	：	
//		pno	：	
//-----------------------------------------------------------------------------
void CH31HyoView::PrintRefund_Hojin(CDC* pDC, int pno)
{
	CRefundBaseDialog::HojinPrint(pDC, pno);
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan(CDC* pDC)
{
	int		dp, st;

	char	*BP = NULL;
	if ((BP = new char[KSK_BUF_SIZE]) == NULL) {
		return -1;
	}
	memset(BP, '\0', KSK_BUF_SIZE);
	char	*BP1 = BP;

	st = 0;

	for (dp = 1; dp <= 5; ++dp) {
		switch (dp) {
		case	1:	st = PrintChukan_sub1(dp, pDC, BP1);		break;
		case	2:	st = PrintChukan_sub2(dp, pDC, BP1);		break;
		case	3:	st = PrintChukan_sub3(dp, pDC, BP1);		break;
		case	4:	st = PrintChukan_sub4(dp, pDC, BP1);		break;
		}
		if (st)	break;
	}

	if (BP) {
		delete BP;
		BP = NULL;
	}


	if ((m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE) && m_pSnHeadData->IsSoleProprietor()) {
		m_pVprn->DrawSline(0, LKnrW0, 1500, 21230, 8070, 20530); //個人番号欄に斜線
	}


	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn)) {
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 36L) + (PV1 / 2);
		long XPosition = (PH / 2);
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, "ＭＳ　ゴシック");
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar))	return(1);

		YPosition = (PV1 * 36L) + (PV1 / 2);
		XPosition = (PH * 8L) + (PH / 2);
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar))	return(1);

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);

	}

	return(st);
}


//-----------------------------------------------------------------------------
// 前年実績による中間申告(実部１)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub1(int dp, CDC* pDC, char *BP)
{
	int		i, j;
	char	buf[128];
	char	bf1[10], bf2[10], bf3[10];
	int		len;
	PBUF	PF[30];

	memset(PF, '\0', sizeof(PF));
	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 納税地
	if (!m_pPrintCmInfo->OP3) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, &m_pSnHeadData->Sn_NOZEI[0], 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
		for (j = 0; j < 2; j++) {
			memmove(BP1, &m_pSnHeadData->Sn_NOZEI[40], 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
		pbclr(0, 2, &PF[i++]);
	}

	// Tel
	if (!m_pPrintCmInfo->OPE) {
		memset(buf, '\0', sizeof(buf));
		memset(bf1, '\0', sizeof(bf1));
		memset(bf2, '\0', sizeof(bf2));
		memset(bf3, '\0', sizeof(bf3));
		len = kjlen(m_pSnHeadData->Sn_TELNO1, 6);
		memmove(bf1, m_pSnHeadData->Sn_TELNO1, len);
		len = kjlen(m_pSnHeadData->Sn_TELNO2, 4);
		memmove(bf2, m_pSnHeadData->Sn_TELNO2, len);
		len = kjlen(m_pSnHeadData->Sn_TELNO3, 4);
		memmove(bf3, m_pSnHeadData->Sn_TELNO3, len);
		wsprintf(buf, "%6s －%4s －%4s", bf1, bf2, bf3);
		len = (int)strlen(buf);
		for (j = 0; j < 2; j++) {
			memmove(BP1, buf, len);
			PF[i].PITM[j] = BP1;	BP1 += (len + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
	}

	// 名称又は屋号 フリガナ
	if (!m_pPrintCmInfo->OPF) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, m_pSnHeadData->Sn_COFRI, 80);
			PF[i].PITM[j] = BP1;	BP1 += (80 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
	}

	// 名称又は屋号
	if (!m_pPrintCmInfo->OP4) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, m_pSnHeadData->Sn_CONAM, 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
	}


	CICSMNSub8 mnsub;
	CString myno_str, myno_prn;
	if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
		if (m_pPset->psSigCoprn & 0x4000) {
			myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
		}
		else {
			myno_prn = myno_str;
		}
		if (m_pSnHeadData->IsSoleProprietor() == TRUE) {
			myno_prn = ' ' + myno_prn;
		}
		for (j = 0; j < 2; j++) {
			memmove(BP1, myno_prn, 13);
			if ((j == 1) && m_pSnHeadData->IsSoleProprietor()) {
				memset(BP1, '\0', 13);
			}
			PF[i].PITM[j] = BP1; BP1 += (13 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
	}

	// 代表者氏名 フリガナ
	if (!m_pPrintCmInfo->OPG) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, m_pSnHeadData->Sn_DIFRI, 80);
			PF[i].PITM[j] = BP1;	BP1 += (80 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
	}

	// 代表者氏名
	if (!m_pPrintCmInfo->OP5) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, m_pSnHeadData->Sn_DIHYO, 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 2, &PF[i++]);
	}

	return(_xprn(1, dp, PF, 0, pDC, 0, 0x0102));
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告(実部２)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub2(int dp, CDC* pDC, char *BP)
{
	int		i, j;
	int		len;
	char	buf[128];
	PBUF	PF[30];

	memset(PF, '\0', sizeof(PF));

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 氏名＆法人名
	for (j = 0; j < 2; j++) {	// 上詰め　法人名
		if (m_pPrintCmInfo->OP2 != 0) {
			;
		}
		else
			if (m_pPrintCmInfo->OPA == 2 || (m_pPrintCmInfo->OPA == 1 && m_pPrintCmInfo->OPsign & 0x01)) {
				memmove(BP1, m_pShinInfo->pO_DATA->o_ofcnam, 40);
			}
		PF[i].PITM[j] = BP1;
		BP1 += (40 + 1);
	}
	i++;

		// 氏名（氏名＆法人名）
	int	slen = 0;
	if (m_pPrintCmInfo->OP2 != 0) {
		;
	}
	else

		if (m_pPrintCmInfo->OPA == 2) {
			memset(buf, '\0', sizeof(buf));
			MakeOutZeirishiName(buf, sizeof(buf));
			slen = (int)strlen(buf);
		}
	if (slen > 40) {
		pbclr(0, 2, &PF[i++]);

		memmove(BP1, buf, slen);
		for (j = 0; j < 2; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
	}
	else if (slen > 0) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;

		pbclr(0, 2, &PF[i++]);
	}
	else {
		pbclr(0, 2, &PF[i++]);
		pbclr(0, 2, &PF[i++]);
	}

	// 氏名or法人名
	slen = 0;
	memset(buf, '\0', sizeof(buf));
	if (m_pPrintCmInfo->OP2 != 0) {
		;
	}
	else if ((m_pPrintCmInfo->OPA == 2) || ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01))) {
		;
	}
	else {
		memset(buf, '\0', sizeof(buf));
		MakeOutZeirishiName(buf, sizeof(buf));
		slen = (int)strlen(buf);
	}

	if (slen > 40) {
		pbclr(0, 2, &PF[i++]);
		for (j = 0; j < 2; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
	}
	else if (slen > 0) {
		for (j = 0; j < 2; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
		pbclr(0, 2, &PF[i++]);
	}
	else {
		pbclr(0, 2, &PF[i++]);
		pbclr(0, 2, &PF[i++]);
	}

	// Tel
	memset(buf, '\0', sizeof(buf));
	ZTel_Set(buf, m_pPrintCmInfo->ZtelP, m_pPrintCmInfo->OP9, 16, 30);
	len = (int)strlen(buf);
	for (j = 0; j < 2; j++) {
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;
		BP1 += (len + 1);
	}
	i++;

	return(_xprn(1, dp, PF, 0, pDC, 0, 0x0102));
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告(実部３)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub3(int dp, CDC* pDC, char *BP)
{
	int		i, j, k, x;
	char	buf[128];
	CString str;
	int		len, sw = 0;
	PBUF	PF[30];

	memset(PF, '\0', sizeof(PF));

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 提出年月日
	char	yymmdd[4] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				// 前0を出さない
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	// 税務署名
	for (j = 0; j < 2; j++) {
		memmove(BP1, m_pSnHeadData->Sn_ZMSYO, 12);
		PF[i].PITM[j] = BP1;	BP1 += (12 + 1);
	}
	i++;

	// 整理番号
	memset(buf, '\0', sizeof(buf));
	l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
	len = (int)strlen(buf);
	str.Empty();
	for (j = 8; j > 0; j--) {
		if (len == j) {
			str += buf;		break;
		}
		str += ' ';
	}
	for (j = 0; j < 2; j++) {

		wsprintf(BP1, "%s", str);
		PF[i].PITM[j] = BP1;	BP1 += (8 + 1);
	}
	i++;

	// 課税期間
	int gengo1 = 0, gengo2 = 0;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	int ii = 0;
	for ( j = 0; j < 2; j++ ) {
		ii = i;
		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = ( char* )0;
		}

		if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = ( char* )0;
		}
	}
	i = ii;

	// 中間申告の計算期間
	gengo1 = gengo2 = 0;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	ii = 0;
	for ( j = 0; j < 2; j++ ) {
		ii = i;

		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = ( char* )0;
		}

		if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = (char *)0;
			PF[ii++].PITM[j] = (char *)0;
		}
	}
	i = ii;

	return(_xprn(1, dp, PF, 0, pDC, 0, 0x0102));
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告(実部４)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub4(int dp, CDC* pDC, char *BP)
{
	int		i, j, k, x;
	char	buf[128];
	int		len, sw;
	char	val[6];
	PBUF	PF[30];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 前課税期間
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[4] = { 0 };
	m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	int ii = 0;
	for ( j = 0; j < 2; j++ ) {
		ii = i;
		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = ( char* )0;
		}

		if ( gengo2 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = ( char* )0;
		}
	}
	i = ii;

	// 修正・更正・決定の年月日
	gengo1 = gengo2 = 0;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 2; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	ii = 0;
	for ( j = 0; j < 2; j++ ) {
		ii = i;
		if ( gengo1 == ID_ICSSH_REIWA_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = BP1;
			BP1 += 5;
		}
		else if ( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
			memmove ( BP1, "    ", 4 );
			PF[ii++].PITM[j] = BP1;
			PF[ii++].PITM[j] = ( char* )0;
			BP1 += 5;
		}
		else {
			PF[ii++].PITM[j] = ( char* )0;
			PF[ii++].PITM[j] = ( char* )0;
		}
	}
	i = ii;

	// 前課税期間の消費税額
	len = val_set(buf, m_pSnHonpyoData->Sn_ZNOFZ);
	for (j = 0; j < 2; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 月数換算
	memset(buf, '\0', sizeof(buf));
	if (m_pSnChskData->Sn_BSMON) {
		wsprintf(buf, "%2d", m_pSnChskData->Sn_BSMON);
	}
	len = (int)strlen(buf);
	for (j = 0; j < 2; j++) {
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;
		BP1 += (len + 1);
	}
	i++;
	memset(buf, '\0', sizeof(buf));
	if (m_pSnChskData->Sn_BBMON) {
		wsprintf(buf, "%2d", m_pSnChskData->Sn_BBMON);
	}
	len = (int)strlen(buf);
	for (j = 0; j < 2; j++) {
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;
		BP1 += (len + 1);
	}
	i++;

	// 修正申告
	if (m_pSnHeadData->Sn_SKKBN == 4) {
		for (x = 0; x < 5; x++, i++) {
			memset(val, '\0', 6);
			switch (x) {
			case 0:		// 消費税　この申告前の税額
				memmove(val, m_pSnChskData->Sn_ZSKZN, 6);	break;
			case 1:		// 消費税　この申告により増加する税額
				memmove(val, m_pSnChskData->Sn_ZSKAD, 6);	break;
			case 2:		// 地方消費税　この申告前の税額
				memmove(val, m_pSnChskData->Sn_ZSTZN, 6);	break;
			case 3:		// 地方消費税　この申告により増加する税額
				memmove(val, m_pSnChskData->Sn_ZSTAD, 6);	break;
			case 4:		// 合計納付税額
				memmove(val, m_pSnChskData->Sn_ZSKTZ, 6);	break;
			}
			len = val_set(buf, (unsigned char*)val);
			for (j = 0; j < 2; j++) {
				memcpy(BP1, buf, len);
				PF[i].PITM[j] = BP1;	BP1 += (len + 1);
			}
		}
	}
	else {
		memset(buf, '\0', sizeof(buf));
		for (x = 0; x < 5; x++, i++) {
			for (j = 0; j < 2; j++) {
				memcpy(BP1, buf, 11);
				PF[i].PITM[j] = BP1;	BP1 += (11 + 1);
			}
		}
	}

	// 納付すべき消費税額
	len = val_set(buf, m_pSnHonpyoData->Sn_EDNOFZ);
	for (j = 0; j < 2; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 納付すべき地方消費税額
	len = val_set(buf, m_pSnHonpyoData->Sn_TEDNOF);
	for (j = 0; j < 2; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 合計納付税額
	len = val_set(buf, m_pSnChskData->Sn_KTNFZ);
	for (j = 0; j < 2; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	int st = 0;
	//return(_xprn(1, dp, PF, 0, pDC, 0, 0x0102));
	if ( ( st = _xprn ( 1, dp, PF, 0, pDC, 0, 0x0102 ) ) ) {
		return st;
	}

	// 新型コロナウイルスによる申告・納付期限延長申請
	if ( m_pSnHeadData->Sn_GenericSgn & 0x10 ) {
		CString str;
		len = 0;
		str.Format ( _T( "新型コロナウイルスによる申告・納付期限延長申請" ) );
		len = str.GetLength();
		memmove ( BP1, str.GetBuffer(), len );
		PF[0].PITM[0] = BP1;
		PF[0].PITM[0] = BP1;
		PF[0].PITM[1] = BP1;
		if ( ( st = _xprn ( 1, ( dp + 1 ), PF, 0, pDC, 0, 0x0102 ) ) ) {
			return st;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告２
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukanEx(CDC* pDC)
{
	int		dp, st;

	char	*BP = NULL;
	if ((BP = new char[KSK_BUF_SIZE]) == NULL) {
		return -1;
	}
	char *BP1 = BP;

	st = 0;

	for (dp = 1; dp <= 5; ++dp) {
		switch (dp) {
		case	1:	st = PrintChukan_sub1Ex(dp, pDC, BP1);		break;
		case	2:	st = PrintChukan_sub2Ex(dp, pDC, BP1);		break;
		case	3:	st = PrintChukan_sub3Ex(dp, pDC, BP1);		break;
		case	4:	st = PrintChukan_sub4Ex(dp, pDC, BP1);		break;
		}
		if (st)	break;
	}

	if (BP) {
		delete BP;
		BP = NULL;
	}

	if ((m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE) && m_pSnHeadData->IsSoleProprietor()) {
		m_pVprn->DrawSline(0, LKnrW0, 5750, 11300, 12300, 10600); //個人番号欄に斜線
	}

	if (*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn)) {
		long	PH = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
		long YPosition = (PV1 * 12L);
		long XPosition = (PH * 15L) + (PH / 2);
		int	Gothic095 = m_pVprn->RegisterFont(90, 0, 0, "ＭＳ　ゴシック");
		int	Gothic108 = m_pVprn->RegisterFont(MAKELONG(108, 1), 0, 0, "ＭＳ　ゴシック");
		struct	_SF_PAR		SfPar;
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (11L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic108;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, _T("電子申告済"), &SfPar))	return(1);

		YPosition = (PV1 * 12L);
		XPosition = (PH * 23L) + (PH / 2);
		memset((char *)&SfPar, '\0', sizeof(struct _SF_PAR));
		SfPar.sf_fcsg = (SFp_xl | SFp_yb);					// ファンクションサイン
		SfPar.sf_hvsg = 0;										// 出力方向
		SfPar.sf_xfld = (35L * PH);								// 横出力領域幅
		SfPar.sf_yfld = PV1;									// 縦出力領域幅
		SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		SfPar.sf_mrgn[1] = 10;									// 　　〃　　　　　右
		SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		SfPar.sf_ksiz = Gothic095;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ		
		SfPar.sf_erto = 0;										// 文字拡大縮小率
		if (m_pVprn->DrawString(0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar))	return(1);

		m_pVprn->DeleteRegFont(Gothic095);
		m_pVprn->DeleteRegFont(Gothic108);

	}

	return(st);
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告２(実部１)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub1Ex(int dp, CDC* pDC, char *BP)
{
	int		i, j;
	char	buf[128];
	char	bf1[10], bf2[10], bf3[10];
	int		len;
	PBUF	PF[30];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 納税地
	if (!m_pPrintCmInfo->OP3) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, &m_pSnHeadData->Sn_NOZEI[0], 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
		for (j = 0; j < 1; j++) {
			memmove(BP1, &m_pSnHeadData->Sn_NOZEI[40], 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 1, &PF[i++]);
		pbclr(0, 1, &PF[i++]);
	}

	// Tel
	if (!m_pPrintCmInfo->OPE) {
		memset(buf, '\0', sizeof(buf));
		memset(bf1, '\0', sizeof(bf1));
		memset(bf2, '\0', sizeof(bf2));
		memset(bf3, '\0', sizeof(bf3));
		len = kjlen(m_pSnHeadData->Sn_TELNO1, 6);
		memmove(bf1, m_pSnHeadData->Sn_TELNO1, len);
		len = kjlen(m_pSnHeadData->Sn_TELNO2, 4);
		memmove(bf2, m_pSnHeadData->Sn_TELNO2, len);
		len = kjlen(m_pSnHeadData->Sn_TELNO3, 4);
		memmove(bf3, m_pSnHeadData->Sn_TELNO3, len);
		wsprintf(buf, "%6s －%4s －%4s", bf1, bf2, bf3);
		len = (int)strlen(buf);
		for (j = 0; j < 1; j++) {
			memmove(BP1, buf, len);
			PF[i].PITM[j] = BP1;	BP1 += (len + 1);
		}
		i++;
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}

	// 名称又は屋号 フリガナ
	if (!m_pPrintCmInfo->OPF) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, m_pSnHeadData->Sn_COFRI, 80);
			PF[i].PITM[j] = BP1;	BP1 += (80 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}

	// 名称又は屋号
	if (!m_pPrintCmInfo->OP4) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, m_pSnHeadData->Sn_CONAM, 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}


	if (m_pSnHeadData->IsSoleProprietor()) {
		pbclr(0, 1, &PF[i++]);
	}
	else {

		CICSMNSub8 mnsub;
		CString myno_str, myno_prn;
		if (m_pSnHeadData->GetMyNumber(myno_str) == 0) {
			if (m_pPset->psSigCoprn & 0x4000) {
				myno_prn = mnsub.MakePrintKojinNo(myno_str, m_pVprn->Get_KojinPrintFlg());
			}
			else {
				myno_prn = myno_str;
			}
			if (m_pSnHeadData->IsSoleProprietor() == TRUE) {
				myno_prn = ' ' + myno_prn;
			}
			for (j = 0; j < 1; j++) {
				memmove(BP1, myno_prn, 13);
				PF[i].PITM[j] = BP1; BP1 += (13 + 1);
			}
			i++;
		}
		else {
			pbclr(0, 1, &PF[i++]);
		}
	}

	// 代表者氏名 フリガナ
	if (!m_pPrintCmInfo->OPG) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, m_pSnHeadData->Sn_DIFRI, 80);
			PF[i].PITM[j] = BP1;	BP1 += (80 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}

	// 代表者氏名
	if (!m_pPrintCmInfo->OP5) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, m_pSnHeadData->Sn_DIHYO, 40);
			PF[i].PITM[j] = BP1;	BP1 += (40 + 1);
		}
		i++;
	}
	else {
		pbclr(0, 1, &PF[i++]);
	}

	return(_xprn(2, dp, PF, 0, pDC, 0, 0x0101));
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告２(実部２)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub2Ex(int dp, CDC* pDC, char *BP)
{
	int		i, j;
	int		len;
	char	buf[128];
	PBUF	PF[30];

	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 氏名＆法人名追加 08/04/22
	for (j = 0; j < 1; j++) {	// 上詰め　法人名
		if (m_pPrintCmInfo->OP2 != 0) {
			;
		}
		else
			if (m_pPrintCmInfo->OPA == 2 || (m_pPrintCmInfo->OPA == 1 && m_pPrintCmInfo->OPsign & 0x01))
				memmove(BP1, m_pShinInfo->pO_DATA->o_ofcnam, 40);
		PF[i].PITM[j] = BP1;
		BP1 += (40 + 1);
	}
	i++;

	// 氏名（氏名＆法人名）
	int slen = 0;
	if (m_pPrintCmInfo->OP2 != 0) {
		;
	}
	else
		if (m_pPrintCmInfo->OPA == 2) {
			memset(buf, '\0', sizeof(buf));
			MakeOutZeirishiName(buf, sizeof(buf));
			slen = (int)strlen(buf);
		}
	if (slen > 40) {
		pbclr(0, 1, &PF[i++]);
		for (j = 0; j < 1; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
	}
	else if (slen > 0) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
		pbclr(0, 1, &PF[i++]);
	}
	else {
		pbclr(0, 1, &PF[i++]);
		pbclr(0, 1, &PF[i++]);
	}


	slen = 0;
	memset(buf, '\0', sizeof(buf));
	if (m_pPrintCmInfo->OP2 != 0) {
		;
	}
	else if ((m_pPrintCmInfo->OPA == 2) || ((m_pPrintCmInfo->OPA == 1) && (m_pPrintCmInfo->OPsign & 0x01))) {
		;
	}
	else {
		memset(buf, '\0', sizeof(buf));
		MakeOutZeirishiName(buf, sizeof(buf));
		slen = (int)strlen(buf);
	}
	if (slen > 40) {
		pbclr(0, 1, &PF[i++]);
		for (j = 0; j < 1; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
	}
	else if (slen > 0) {
		for (j = 0; j < 1; j++) {
			memmove(BP1, buf, slen);
			PF[i].PITM[j] = BP1;
			BP1 += (slen + 1);
		}
		i++;
		pbclr(0, 1, &PF[i++]);
	}
	else {
		pbclr(0, 1, &PF[i++]);
		pbclr(0, 1, &PF[i++]);
	}

	// Tel
	memset(buf, '\0', sizeof(buf));
	ZTel_Set(buf, m_pPrintCmInfo->ZtelP, m_pPrintCmInfo->OP9, 16, 30);
	len = (int)strlen(buf);
	for (j = 0; j < 1; j++) {
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;
		BP1 += (len + 1);
	}
	i++;

	return(_xprn(2, dp, PF, 0, pDC, 0, 0x0101));
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告２(実部３)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub3Ex(int dp, CDC* pDC, char *BP)
{
	int		i, j, k, x;
	char	buf[128];
	CString str;
	int		len, sw = 0;
	PBUF	PF[30];


	memset(PF, '\0', sizeof(PF));
	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 提出年月日
	char	yymmdd[4] = { 0 };
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	// 税務署名
	for (j = 0; j < 1; j++) {
		memmove(BP1, m_pSnHeadData->Sn_ZMSYO, 12);
		PF[i].PITM[j] = BP1;	BP1 += (12 + 1);
	}
	i++;

	// 整理番号
	memset(buf, '\0', sizeof(buf));
	m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
	len = (int)strlen(buf);
	str.Empty();
	for (j = 8; j > 0; j--) {
		if (len == j) {
			str += buf;		break;
		}
		str += ' ';
	}
	for (j = 0; j < 1; j++) {

		wsprintf(BP1, "%s", str);
		PF[i].PITM[j] = BP1;	BP1 += (8 + 1);
	}
	i++;

	// 課税期間
	int gengo1 = 0, gengo2 = 0;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = (char*)0;
		}

		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = (char*)0;
		}

		i++;

	// 中間申告の計算期間
	gengo1 = gengo2 = 0;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;


		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = (char*)0;
		}

		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;

	return(_xprn(2, dp, PF, 0, pDC, 0, 0x0101));
}

//-----------------------------------------------------------------------------
// 前年実績による中間申告２(実部４)
//-----------------------------------------------------------------------------
int CH31HyoView::PrintChukan_sub4Ex(int dp, CDC* pDC, char *BP)
{
	int		i, j, k, x;
	char	buf[128];
	int		len, sw;
	char	val[6];
	PBUF	PF[30];

	memset(PF, '\0', sizeof(PF));
	i = 0;
	memset(BP, '\0', KSK_BUF_SIZE);
	char *BP1 = BP;

	// 前課税期間
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[4] = { 0 };
	m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = (char*)0;
		}

		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = (char*)0;
		}

		i++;


	// 修正・更正・決定の年月日
	gengo1 = gengo2 = 0;
	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	for (j = k = 0; k < 1; k++) {
		for (x = 0; x < 3; j++, x++, BP1 += 3) {
			sw = 0;
			if (x == 0) {
				if (yymmdd[x])	sw = 1;
				else {
					if (yymmdd[x + 1])	sw = 1;
				}
			}
			else {
				if (yymmdd[x])	sw = 1;
			}
			if (sw) {
				//wsprintf(BP1, "%02X", yymmdd[x]);
				wsprintf(BP1, "%2X", yymmdd[x]);
			}
			else
				memset(BP1, '\0', 2);
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char*)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char*)0;
			PF[i].PITM[1] = (char*)0;
		}

		i++;

	// 前課税期間の消費税額
	len = val_set(buf, m_pSnHonpyoData->Sn_ZNOFZ);
	for (j = 0; j < 1; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 月数換算
	memset(buf, '\0', sizeof(buf));
	if (m_pSnChskData->Sn_BSMON) {
		wsprintf(buf, "%2d", m_pSnChskData->Sn_BSMON);
	}
	len = (int)strlen(buf);
	for (j = 0; j < 1; j++) {
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;
		BP1 += (len + 1);
	}
	i++;
	memset(buf, '\0', sizeof(buf));
	if (m_pSnChskData->Sn_BBMON) {
		wsprintf(buf, "%2d", m_pSnChskData->Sn_BBMON);
	}
	len = (int)strlen(buf);
	for (j = 0; j < 1; j++) {
		memmove(BP1, buf, len);
		PF[i].PITM[j] = BP1;
		BP1 += (len + 1);
	}
	i++;

	// 修正申告
	if (m_pSnHeadData->Sn_SKKBN == 4) {
		for (x = 0; x < 5; x++, i++) {
			memset(val, '\0', 6);
			switch (x) {
			case 0:		// 消費税　この申告前の税額
				memmove(val, m_pSnChskData->Sn_ZSKZN, 6);	break;
			case 1:		// 消費税　この申告により増加する税額
				memmove(val, m_pSnChskData->Sn_ZSKAD, 6);	break;
			case 2:		// 地方消費税　この申告前の税額
				memmove(val, m_pSnChskData->Sn_ZSTZN, 6);	break;
			case 3:		// 地方消費税　この申告により増加する税額
				memmove(val, m_pSnChskData->Sn_ZSTAD, 6);	break;
			case 4:		// 合計納付税額
				memmove(val, m_pSnChskData->Sn_ZSKTZ, 6);	break;
			}
			len = val_set(buf, (unsigned char*)val);
			for (j = 0; j < 1; j++) {
				memcpy(BP1, buf, len);
				PF[i].PITM[j] = BP1;	BP1 += (len + 1);
			}
		}
	}
	else {
		memset(buf, '\0', sizeof(buf));
		for (x = 0; x < 5; x++, i++) {
			for (j = 0; j < 1; j++) {
				memcpy(BP1, buf, 11);
				PF[i].PITM[j] = BP1;	BP1 += (11 + 1);
			}
		}
	}

	// 納付すべき消費税額
	len = val_set(buf, m_pSnHonpyoData->Sn_EDNOFZ);
	for (j = 0; j < 1; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 納付すべき地方消費税額
	len = val_set(buf, m_pSnHonpyoData->Sn_TEDNOF);
	for (j = 0; j < 1; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	// 合計納付税額
	len = val_set(buf, m_pSnChskData->Sn_KTNFZ);
	for (j = 0; j < 1; j++) {
		memcpy(BP1, buf, len);
		PF[i].PITM[j] = BP1;	BP1 += (len + 1);
	}
	i++;

	//return(_xprn(2, dp, PF, 0, pDC, 0, 0x0101));
	int st = 0;
	if ( ( st = _xprn ( 2, dp, PF, 0, pDC, 0, 0x0101 ) ) ) {
		return st;
	}

	// 新型コロナウイルスによる申告・納付期限延長申請
	if ( m_pSnHeadData->Sn_GenericSgn & 0x10 ) {
		CString str;
		len = 0;
		str.Format ( _T( "新型コロナウイルスによる申告・納付期限延長申請" ) );
		len = str.GetLength();
		memmove ( BP1, str.GetBuffer(), len );
		PF[0].PITM[0] = BP1;
		if ( ( st = _xprn ( 2, ( dp + 1 ), PF, 0, pDC, 0, 0x0101 ) ) ) {
			return st;
		}
	}

	return 0;

}

//-----------------------------------------------------------------------------
// 切取線
//-----------------------------------------------------------------------------
void CH31HyoView::Lin_Print()
{
	long	sx, sy, ex, ey;
	long	dotx, doty;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
	doty = (m_pPrintCmInfo->dat1 * 10L);
	dotx = (m_pPrintCmInfo->dat2 * 10L);
	for (int i = 0; i < 16; i++) {
		sx = sy = ex = ey = 0L;
		switch (i) {
		case  0:	sx = 0;	sy = 400;	ex = 500;	ey = 400;	break;	// 横	提出用　上左
		case  1:	sx = 0;	sy = 250;	ex = 0;	ey = 550;	break;	// 縦
		case  2:	sx = 19400;	sy = 400;	ex = 19900;	ey = 400;	break;	// 横	提出用　上右
		case  3:	sx = 19900;	sy = 250;	ex = 19900;	ey = 550;	break;	// 縦
		case  4:	sx = 0;	sy = 9850;	ex = 500;	ey = 9850;	break;	// 横	提出用　下左
		case  5:	sx = 0;	sy = 9700;	ex = 0;	ey = 10000;	break;	// 縦
		case  6:	sx = 19400;	sy = 9850;	ex = 19900;	ey = 9850;	break;	// 横	提出用　下右
		case  7:	sx = 19900;	sy = 9700;	ex = 19900;	ey = 10000;	break;	// 縦
		case  8:	sx = 0;	sy = 14850;	ex = 500;	ey = 14850;	break;	// 横	控用　　上左
		case  9:	sx = 0;	sy = 14700;	ex = 0;	ey = 15000;	break;	// 縦
		case 10:	sx = 19400;	sy = 14850;	ex = 19900;	ey = 14850;	break;	// 横	控用　　上右
		case 11:	sx = 19900;	sy = 14700;	ex = 19900;	ey = 15000;	break;	// 縦
		case 12:	sx = 0;	sy = 24300;	ex = 500;	ey = 24300;	break;	// 横	控用　　下左
		case 13:	sx = 0;	sy = 24150;	ex = 0;	ey = 24450;	break;	// 縦
		case 14:	sx = 19400;	sy = 24300;	ex = 19900;	ey = 24300;	break;	// 横	控用　　下右
		case 15:	sx = 19900;	sy = 24150;	ex = 19900;	ey = 24450;	break;	// 縦
		}
		sx += dotx;	sy += doty;	ex += dotx;	ey += doty;
		m_pVprn->DrawSline(0, LKnrW0, sx, sy, ex, ey);
	}
}

//-----------------------------------------------------------------------------
// 金額をセット（中間申告用）
//-----------------------------------------------------------------------------
int CH31HyoView::val_set(char *bf, unsigned char *val)
{
	char	num1[6], num2[6];
	char	buf[128];
	CString str1, str2, ans;
	int		js, len;

	memset(bf, '\0', 128);
	memset(buf, '\0', sizeof(buf));
	memcpy(num1, val, 6);
	m_Arith.l_input(num2, "100");
	m_Arith.l_div(num1, num1, num2);
	m_Arith.l_print(buf, num1, FM10H);
	str1 = buf;
	ans.Empty();
	for (int i = 0; i < 2; i++) {
		if (i == 0)	js = 5;
		else			js = 3;
		ans += str1.Left(js);
		ans += " ";
		str2 = str1.Mid(js);
		str1 = str2;
	}
	ans += str1;
	wsprintf(bf, "%s", ans);
	len = (int)strlen(bf);

	return len;
}

//-----------------------------------------------------------------------------
// 電話番号をセット（中間申告用）
//-----------------------------------------------------------------------------
void CH31HyoView::ZTel_Set(char *buf, char *telno, char sw, int len, int lng)
{
	char	*fp, tel[128], ttel[4][32];
	int		cnt;
	char	bf1[32], bf2[32], bf3[32];

	memset(bf1, '\0', 32);
	memset(bf2, '\0', 32);
	memset(bf3, '\0', 32);
	memset(ttel, '\0', 32 * 4);
	memset(tel, '\0', sizeof(tel));
	memcpy(tel, telno, lng);

	if (sw) {
		wsprintf(buf, "       －     －");
		return;
	}

	cnt = 2;
	if (fp = (char *)strrchr(tel, '-')) {
		strcpy_s(ttel[cnt--], 32, (fp + 1));			/* 個人番号 */
		*fp = '\0';
		if (fp = (char *)strrchr(tel, '-')) {
			strcpy_s(ttel[cnt--], 32, (fp + 1));		/* 市内局番 */
			*fp = '\0';
			strcpy_s(ttel[cnt], 32, tel);				/* 市外局番 */
		}
		else	strcpy_s(ttel[cnt], 32, tel);			/* 市内局番 */

		while (1) {
			switch (cnt) {
			case 0:	/* 市外局番 */
				memcpy(bf1, ttel[cnt], 32);
				++cnt;	continue;
			case 1:	/* 市内局番 */
				memcpy(bf2, ttel[cnt], 32);
				++cnt;	continue;
			case 2:	/* 個人番号 */
				memcpy(bf3, ttel[cnt], 32);
				++cnt;	continue;
			default:	break;
			}
			break;
		}
		if ((kjlen(bf1, 32) > 6) || (kjlen(bf2, 32) > 4) || (kjlen(bf3, 32) > 4)) {
			wsprintf(buf, "%s－%s－%s", bf1, bf2, bf3);
			if (kjlen(buf, 32) > 22)	buf[22] = 0;
		}
		else	wsprintf(buf, "%6s －%4s －%4s", bf1, bf2, bf3);
	}
	else {
		if (kjlen(telno, 30) == 0)
			wsprintf(buf, "       －     －");
		else
			memcpy(buf, telno, 26);
	}
}

void CH31HyoView::InitTokureiTblInfo()
{
	m_Uri1kbndatainfo[0] = { _T("CUB00030"), DATAKIND_STR, PRT_KSURI_TITLE[0], 64, NULL };
	m_Uri1kbndatainfo[1] = { _T("CUB00040"), DATAKIND_VAL, PRT_KSURI_1[0], 16, NULL };
	m_Uri1kbndatainfo[2] = { _T("CUB00060"), DATAKIND_DATE, PRT_KSURI_2_S[0], 4, NULL };
	m_Uri1kbndatainfo[3] = { _T("CUB00070"), DATAKIND_DATE, PRT_KSURI_2_E[0], 4, NULL };
	m_Uri1kbndatainfo[4] = { _T("CUB00080"), DATAKIND_VAL, PRT_KSURI_3[0], 16, NULL };
	m_Uri1kbndatainfo[5] = { _T("CUB00090"), DATAKIND_VAL, PRT_KSURI_4[0], 16, NULL };
	m_Uri1kbndatainfo[6] = { _T("CUB00100"), DATAKIND_RATE, PRT_KSURI_5[0], 16, NULL };
	m_Uri1kbndatainfo[7] = { _T("CUB00105"), DATAKIND_CHECK, (char*)&PRT_KSURI_5_CHECK[0], sizeof(int), NULL };
	m_Uri1kbndatainfo[8] = { _T("CUB00110"), DATAKIND_VAL, PRT_KSURI_6[0], 16, NULL };
	m_Uri1kbndatainfo[9] = { _T("CUB00120"), DATAKIND_VAL, PRT_KSURI_7[0], 16, NULL };

	m_Uri1datainfo[0] = { _T("CUB00140"), DATAKIND_VAL, PRT_KSURI_6_KEI, 16, NULL };
	m_Uri1datainfo[1] = { _T("CUB00150"), DATAKIND_VAL, PRT_KSURI_7_KEI, 16, NULL };
	m_Uri1datainfo[2] = { _T("CUC00010"), DATAKIND_VAL, PRT_KSURI_8, 16, NULL };
	m_Uri1datainfo[3] = { _T("CUC00020"), DATAKIND_VAL, PRT_KSURI_9, 16, NULL };
	m_Uri1datainfo[4] = { _T("CUD00010"), DATAKIND_VAL, PRT_KSURI_10, 16, NULL };
	m_Uri1datainfo[5] = { _T("CUD00020"), DATAKIND_VAL, PRT_KSURI_11, 16, NULL };

	m_Uri2kbndatainfo[0] = { _T("CVB00030"), DATAKIND_STR, PRT_KSURI2_TITLE[0], 64, NULL };
	m_Uri2kbndatainfo[1] = { _T("CVB00050"), DATAKIND_VAL, PRT_KSURI2_1[0], 16, NULL };
	m_Uri2kbndatainfo[2] = { _T("CVB00060"), DATAKIND_VAL, PRT_KSURI2_2[0], 16, NULL };
	m_Uri2kbndatainfo[3] = { _T("CVB00070"), DATAKIND_VAL, PRT_KSURI2_3[0], 16, NULL };
	m_Uri2kbndatainfo[4] = { _T("CVB00080"), DATAKIND_VAL, PRT_KSURI2_4[0], 16, NULL };
	m_Uri2kbndatainfo[5] = { _T("CVB00090"), DATAKIND_VAL, PRT_KSURI2_5[0], 16, NULL };
	m_Uri2kbndatainfo[6] = { _T("CVB00100"), DATAKIND_RATE, PRT_KSURI2_6[0], 16, NULL };
	m_Uri2kbndatainfo[7] = { _T("CVB00105"), DATAKIND_CHECK, (char*)&PRT_KSURI2_6_CHECK[0], sizeof(int), NULL };
	m_Uri2kbndatainfo[8] = { _T("CVB00110"), DATAKIND_VAL, PRT_KSURI2_7[0], 16, NULL };
	m_Uri2kbndatainfo[9] = { _T("CVB00120"), DATAKIND_VAL, PRT_KSURI2_8[0], 16, NULL };
	m_Uri2kbndatainfo[10] = { _T("CVB00130"), DATAKIND_VAL, PRT_KSURI2_9[0], 16, NULL };

	m_Uri2datainfo[0] = { _T("CVB00150"), DATAKIND_VAL, PRT_KSURI2_8_KEI, 16, NULL };
	m_Uri2datainfo[1] = { _T("CVB00160"), DATAKIND_VAL, PRT_KSURI2_9_KEI, 16, NULL };
	m_Uri2datainfo[2] = { _T("CVC00010"), DATAKIND_VAL, PRT_KSURI2_10, 16, NULL };
	m_Uri2datainfo[3] = { _T("CVC00020"), DATAKIND_VAL, PRT_KSURI2_11, 16, NULL };
	m_Uri2datainfo[4] = { _T("CVD00010"), DATAKIND_VAL, PRT_KSURI2_12, 16, NULL };
	m_Uri2datainfo[5] = { _T("CVD00020"), DATAKIND_VAL, PRT_KSURI2_13, 16, NULL };

	m_Shiirekbndatainfo[0] = { _T("CWB00030"), DATAKIND_STR, PRT_KSHIIRE_TITLE[0], 64, NULL };
	m_Shiirekbndatainfo[1] = { _T("CWB00040"), DATAKIND_VAL, PRT_KSHIIRE_1[0], 16, NULL };
	m_Shiirekbndatainfo[2] = { _T("CWB00050"), DATAKIND_VAL, PRT_KSHIIRE_2[0], 16, NULL };
	m_Shiirekbndatainfo[3] = { _T("CWB00060"), DATAKIND_RATE, PRT_KSHIIRE_3[0], 16, NULL };
	m_Shiirekbndatainfo[4] = { _T("CWB00065"), DATAKIND_SIGN, (char*)&PRT_KSHIIRE_3_SIGN[0], sizeof ( int ), NULL };
	m_Shiirekbndatainfo[5] = { _T("CWB00080"), DATAKIND_VAL, PRT_KSHIIRE_4[0], 16, NULL };
	m_Shiirekbndatainfo[6] = { _T("CWB00090"), DATAKIND_VAL, PRT_KSHIIRE_5[0], 16, NULL };
	m_Shiirekbndatainfo[7] = { _T("CWB00100"), DATAKIND_VAL, PRT_KSHIIRE_6[0], 16, NULL };
	m_Shiirekbndatainfo[8] = { _T("CWB00110"), DATAKIND_VAL, PRT_KSHIIRE_7[0], 16, NULL };
	m_Shiirekbndatainfo[9] = { _T("CWB00120"), DATAKIND_VAL, PRT_KSHIIRE_8[0], 16, NULL };

	m_Shiiredatainfo[0] = { _T("CWB00140"), DATAKIND_VAL, PRT_KSHIIRE_7_KEI, 16, NULL };
	m_Shiiredatainfo[0] = { _T("CWB00140"), DATAKIND_VAL, PRT_KSHIIRE_7_KEI, 16, NULL };
	m_Shiiredatainfo[1] = { _T("CWB00150"), DATAKIND_VAL, PRT_KSHIIRE_8_KEI, 16, NULL };
	m_Shiiredatainfo[2] = { _T("CWB00170"), DATAKIND_VAL, PRT_KSHIIRE_9, 16, NULL };
	m_Shiiredatainfo[3] = { _T("CWB00180"), DATAKIND_VAL, PRT_KSHIIRE_10, 16, NULL };
	m_Shiiredatainfo[4] = { _T("CWC00030"), DATAKIND_VAL, PRT_KSHIIRE_11_1, 16, NULL };
	m_Shiiredatainfo[5] = { _T("CWC00040"), DATAKIND_VAL, PRT_KSHIIRE_11_2, 16, NULL };
	m_Shiiredatainfo[6] = { _T("CWC00060"), DATAKIND_VAL, PRT_KSHIIRE_12_1, 16, NULL };
	m_Shiiredatainfo[7] = { _T("CWC00070"), DATAKIND_VAL, PRT_KSHIIRE_12_2, 16, NULL };
	m_Shiiredatainfo[8] = { _T("CWC00080"), DATAKIND_VAL, PRT_KSHIIRE_13_2, 16, NULL };
	m_Shiiredatainfo[9] = { _T("CWC00090"), DATAKIND_VAL, PRT_KSHIIRE_14_2, 16, NULL };
	m_Shiiredatainfo[10] = { _T("CWC00110"), DATAKIND_VAL, PRT_KSHIIRE_15_1, 16, NULL };
	m_Shiiredatainfo[11] = { _T("CWC00120"), DATAKIND_VAL, PRT_KSHIIRE_15_2, 16, NULL };
	m_Shiiredatainfo[12] = { _T("CWC00140"), DATAKIND_VAL, PRT_KSHIIRE_16_1, 16, NULL };
	m_Shiiredatainfo[13] = { _T("CWC00150"), DATAKIND_VAL, PRT_KSHIIRE_16_2, 16, NULL };
	m_Shiiredatainfo[14] = { _T("CWC00170"), DATAKIND_VAL, PRT_KSHIIRE_17, 16, NULL };
	m_Shiiredatainfo[15] = { _T("CWC00180"), DATAKIND_VAL, PRT_KSHIIRE_18, 16, NULL };
	m_Shiiredatainfo[16] = { _T("CWD00010"), DATAKIND_VAL, PRT_KSHIIRE_19, 16, NULL };
	m_Shiiredatainfo[17] = { _T("CWD00020"), DATAKIND_VAL, PRT_KSHIIRE_20, 16, NULL };

	return;
}

void CH31HyoView::ClearTokureiData()
{
	for (int tblidx = 0; tblidx < sizeof(m_Uri1kbndatainfo) / sizeof(m_Uri1kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
			memset(m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * idx_jigyokbn,
				'\0', m_Uri1kbndatainfo[tblidx].size);
		}
	}

	for (int tblidx = 0; tblidx < sizeof(m_Uri1datainfo) / sizeof(m_Uri1datainfo[0]); tblidx++) {
		memset(m_Uri1datainfo[tblidx].data, '\0', m_Uri1datainfo[tblidx].size);
	}

	for (int tblidx = 0; tblidx < sizeof(m_Uri2kbndatainfo) / sizeof(m_Uri2kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			memset(m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * idx_jigyokbn,
				'\0', m_Uri2kbndatainfo[tblidx].size);
		}
	}

	for (int tblidx = 0; tblidx < sizeof(m_Uri2datainfo) / sizeof(m_Uri2datainfo[0]); tblidx++) {
		memset(m_Uri2datainfo[tblidx].data, '\0', m_Uri2datainfo[tblidx].size);
	}

	for (int tblidx = 0; tblidx < sizeof(m_Shiirekbndatainfo) / sizeof(m_Shiirekbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			memset(m_Shiirekbndatainfo[tblidx].data + m_Shiirekbndatainfo[tblidx].size * idx_jigyokbn,
				'\0', m_Shiirekbndatainfo[tblidx].size);
		}
	}

	for (int tblidx = 0; tblidx < sizeof(m_Shiiredatainfo) / sizeof(m_Shiiredatainfo[0]); tblidx++) {
		memset(m_Shiiredatainfo[tblidx].data, '\0', m_Shiiredatainfo[tblidx].size);
	}

	return;
}

void CH31HyoView::GetKazeiKikan(char* pkikan_s, char* pkikan_e)
{
	char yymmdd[3] = { 0 };

	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(pkikan_s, "%02X  %02X  %02X", yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);

	memset(yymmdd, '\0', sizeof(yymmdd));
	m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	wsprintf(pkikan_e, "%02X  %02X  %02X", yymmdd[0] & 0xff, yymmdd[1] & 0xff, yymmdd[2] & 0xff);

	return;
}

void CH31HyoView::GetTekiyoTaisyoKikan(char* pkikan_s, char* pkikan_e)
{
	int TOKUREITEKIYOKIKAN_S = 20191001;
	int TOKUREITEKIYOKIKAN_E = 20230930;

	CVolDateDB voldate;
	int s_gengo = 0, d_gengo = 0;
	int year = 0, kazeikikan_s = 0, kazeikikan_e = 0, tekiyotaisyokikan_s = 0, tekiyotaisyokikan_e = 0;

	char yy, mm, dd;
	CString yystr, mmstr, ddstr;
	if( (m_pSnHeadData->Sn_SKKBN == 0x02) || (m_pSnHeadData->Sn_SKKBN == 0x04) ){
		// 中間申告
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd);
	}
	else {
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd);
	}
	yystr.Format(_T("%02x"), yy);
	mmstr.Format(_T("%02x"), mm);
	ddstr.Format(_T("%02x"), dd);
	year = atoi(yystr) * 10000 + atoi(mmstr) * 100 + atoi(ddstr);
	voldate.db_datecnvGen(s_gengo, year, &d_gengo, &kazeikikan_s, 1, 0);

	if( (m_pSnHeadData->Sn_SKKBN == 0x02) || (m_pSnHeadData->Sn_SKKBN == 0x04) ){
		// 中間申告
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd);
	}
	else {
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
	}
	yystr.Format(_T("%02x"), yy);
	mmstr.Format(_T("%02x"), mm);
	ddstr.Format(_T("%02x"), dd);
	year = atoi(yystr) * 10000 + atoi(mmstr) * 100 + atoi(ddstr);
	voldate.db_datecnvGen(s_gengo, year, &d_gengo, &kazeikikan_e, 1, 0);

	CString tmpstr = _T("");
	if ((TOKUREITEKIYOKIKAN_S <= kazeikikan_s) && (kazeikikan_s <= TOKUREITEKIYOKIKAN_E)) {
		voldate.db_datecnvGen(s_gengo, kazeikikan_s, &d_gengo, &tekiyotaisyokikan_s, 0, 0);
	}
	else if ((kazeikikan_s <= TOKUREITEKIYOKIKAN_S) && (TOKUREITEKIYOKIKAN_S <= kazeikikan_e)) {
		voldate.db_datecnvGen(s_gengo, TOKUREITEKIYOKIKAN_S, &d_gengo, &tekiyotaisyokikan_s, 0, 0);
	}
	tmpstr.Format(_T("%08d"), tekiyotaisyokikan_s);
	wsprintf(pkikan_s, "%02s  %02s  %02s", tmpstr.Mid(2, 2), tmpstr.Mid(4, 2), tmpstr.Mid(6, 2));

	if ((TOKUREITEKIYOKIKAN_S <= kazeikikan_e) && (kazeikikan_e <= TOKUREITEKIYOKIKAN_E)) {
		voldate.db_datecnvGen(s_gengo, kazeikikan_e, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	else if ((kazeikikan_s <= TOKUREITEKIYOKIKAN_E) && (TOKUREITEKIYOKIKAN_E <= kazeikikan_e)) {
		voldate.db_datecnvGen(s_gengo, TOKUREITEKIYOKIKAN_E, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	tmpstr.Format(_T("%08d"), tekiyotaisyokikan_e);
	wsprintf(pkikan_e, "%02s  %02s  %02s", tmpstr.Mid(2, 2), tmpstr.Mid(4, 2), tmpstr.Mid(6, 2));

}

void CH31HyoView::GetName(char* name, int kojin)
{
	if (kojin) {
		memmove(name, m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO));
	}
	else {
		memmove(name, m_pSnHeadData->Sn_CONAM, sizeof(m_pSnHeadData->Sn_CONAM));
	}
}

int CH31HyoView::PrintTokurei_Uri1(CDC *pDC, int kojin, int pcnt)
{
    int pno = 60;
    PBUF pbuf[4] = {0}, pbuf2[16] = {0}, pbuf3[8] = {0};
    int bufidx = 0;
	BOOL isNoImage = FALSE;

	char* buf = nullptr;
	char* val = nullptr;
	int size = 1024;
	if ( ( buf = new char[size] ) == nullptr ) {
		return -1;
	}
	val = buf;
	memset ( val, '\0', size );

	if (m_pPrintCmInfo->PRtype & 0x80) {
	    isNoImage = FALSE;	// イメージ出力有り
	}
	else {
	    isNoImage = TRUE;	// イメージ出力無し
	}

	//課税期間
	char kazeikikan_s[16] = { 0 }, kazeikikan_e[16] = { 0 };
	GetKazeiKikan(kazeikikan_s, kazeikikan_e);
	pbuf[bufidx].PITM[0] = kazeikikan_s;
	pbuf[bufidx++].PITM[1] = kazeikikan_e;

	//適用対象期間
	char tekiyotaisyokikan_s[16] = {0}, tekiyotaisyokikan_e[16] = { 0 };
	GetTekiyoTaisyoKikan(tekiyotaisyokikan_s, tekiyotaisyokikan_e);
	pbuf[bufidx].PITM[0] = tekiyotaisyokikan_s;
	pbuf[bufidx++].PITM[1] = tekiyotaisyokikan_e; 

    //氏名
	char name[44] = { 0 };
    GetName(name, kojin);
	pbuf[bufidx++].PITM[0] = name;

	//フィールド1.
	if (_xprn(pno, 1, pbuf, 0, pDC, isNoImage)) {
		return -1;
	}

    //テーブルからデータ読み込み.
	ReadPrintTokurei_Uri1(m_SnSeq, TOKUREI_TBL_VERSION);

    bufidx = 0;
	for (int tblidx = 0; tblidx < (sizeof(m_Uri1kbndatainfo) / sizeof(m_Uri1kbndatainfo[0])); tblidx++) {

		for (int rowidx = 0; rowidx < CULM_PER_PAGE_TOKUREI_URI1; rowidx++) {

            if(m_Uri1kbndatainfo[tblidx].kind == DATAKIND_VAL){

				//③④は50%適用ONの時は表示しない.
                if( (m_Uri1kbndatainfo[tblidx].itemId.Compare(_T("CUB00080")) == 0) 
					|| (m_Uri1kbndatainfo[tblidx].itemId.Compare(_T("CUB00090")) == 0) ) {

					if (PRT_KSURI_5_CHECK[CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx] == 0) {
						m_Arith.l_print(val,
							m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx),
							FMT15);
						pbuf2[bufidx].PITM[rowidx] = val;
						val += 16;
					}
				}
                else {
					m_Arith.l_print(val,
						m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx),
						FMT15);
					pbuf2[bufidx].PITM[rowidx] = val;
					val += 16;
				}

			}
			else if (m_Uri1kbndatainfo[tblidx].kind == DATAKIND_DATE) {

				//②は50%適用ONの時は表示しない.
				if ((m_Uri1kbndatainfo[tblidx].itemId.Compare(_T("CUB00060")) == 0)
					|| (m_Uri1kbndatainfo[tblidx].itemId.Compare(_T("CUB00070")) == 0)) {

					if (PRT_KSURI_5_CHECK[CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx] == 0) {
						char* date = m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx);
						if (date[0] != '\0') {
							wsprintf(val, "%02X  %02X  %02X", date[0] & 0xff, date[1] & 0xff, date[2] & 0xff);
							pbuf2[bufidx].PITM[rowidx] = val;
							val += 16;
						}
					}
				}
                else {
					char* date = m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx);
					if (date[0] != '\0') {
						wsprintf(val, "%02X  %02X  %02X", date[0] & 0xff, date[1] & 0xff, date[2] & 0xff);
						pbuf2[bufidx].PITM[rowidx] = val;
						val += 16;
					}
				}

			}
			else if (m_Uri1kbndatainfo[tblidx].kind == DATAKIND_CHECK) {
                ;
			}
			else if (m_Uri1kbndatainfo[tblidx].kind == DATAKIND_RATE) {

                if( m_Arith.l_test(PRT_KSURI_3[0 + (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx)]) 
					|| m_Arith.l_test(PRT_KSURI_4[0 + (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx)]) ) {

					pbuf2[bufidx].PITM[rowidx] = m_Uri1kbndatainfo[tblidx].data
						+ m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx);

				}
                else if( PRT_KSURI_5_CHECK[CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx] ){

					pbuf2[bufidx].PITM[rowidx] = m_Uri1kbndatainfo[tblidx].data
						+ m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx);
				}
			}
			else {

				pbuf2[bufidx].PITM[rowidx] = m_Uri1kbndatainfo[tblidx].data
					+ m_Uri1kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI1 * (pcnt - 1) + rowidx);
			}

            if( (pcnt == MAX_PAGE_TOKUREI_URI1) && (rowidx == (CULM_PER_PAGE_TOKUREI_URI1-1)) ) {
                break;
			}

		}

		if (m_Uri1kbndatainfo[tblidx].kind != DATAKIND_CHECK) {
			bufidx++;
		}

	}

	// 合計は最終ページのみ印字する
	if ( pcnt == m_Uri1MxPage ) {
		m_Arith.l_print(val, PRT_KSURI_6_KEI, FMT15);
		pbuf2[bufidx++].PITM[0] = val;
		val += 16;
		
		m_Arith.l_print(val, PRT_KSURI_7_KEI, FMT15);
		pbuf2[bufidx++].PITM[0] = val;
		val += 16;
	}
	else {
		pbuf2[bufidx++].PITM[0] = ( char* )0;
		pbuf2[bufidx++].PITM[0] = ( char* )0;
	}

	//フィールド2.
	if (_xprn(pno, 2, pbuf2, 0, pDC, isNoImage)) {
		return -1;
	}



    bufidx = 0;
	if ( pcnt == m_Uri1MxPage ) {
		m_Arith.l_print(val, PRT_KSURI_8, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI_9, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI_10, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI_11, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;
	}
	else {
		pbuf3[bufidx++].PITM[0] = ( char* )0;
		pbuf3[bufidx++].PITM[0] = ( char* )0;
		pbuf3[bufidx++].PITM[0] = ( char* )0;
	}

	//フィールド3.
	if (_xprn(pno, 3, pbuf3, 0, pDC, isNoImage)) {
		return -1;
	}

	if ( buf ) {
		delete buf;
		buf = nullptr;
	}

    return 0;
}

int CH31HyoView::ReadPrintTokurei_Uri1(int sn_seq, int version)
{
	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_1_HYOID, version);

	if (m_DBSyzAccess.m_pSyz_Tokurei) {
		m_DBSyzAccess.m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return -1;
		}
	}

	CSYZ_TOKUREI* pSyzTokurei = m_DBSyzAccess.m_pSyz_Tokurei;
	if ((pSyzTokurei->MoveFirst() == -1) || (pSyzTokurei->st == -1)) {
		return 0;
	}
	else {

		for (int i = 0;; i++) {

			if (i == 0) {
				if ((pSyzTokurei->MoveFirst() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}
			else {
				if ((pSyzTokurei->MoveNext() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}

			if (ReadSubPrintTokurei_Uri1(pSyzTokurei)) {
				continue;
			}

		}
	}

    return 0;
}

int CH31HyoView::ReadSubPrintTokurei_Uri1(CSYZ_TOKUREI* pTokurei)
{

	for (int tblidx = 0; tblidx < (sizeof(m_Uri1kbndatainfo) / sizeof(m_Uri1kbndatainfo[0])); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
			if ((pTokurei->itemId == m_Uri1kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {

				if (pTokurei->kind == DATAKIND_STR) {
					memmove(m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * idx_jigyokbn,
						pTokurei->str, m_Uri1kbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_DATE) {
					char *date = m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * idx_jigyokbn;
					date[0] = (char)strtol(pTokurei->str.Mid(0, 2), NULL, 16);
					date[1] = (char)strtol(pTokurei->str.Mid(2, 2), NULL, 16);
					date[2] = (char)strtol(pTokurei->str.Mid(4, 2), NULL, 16);
					date[3] = '\0';
				}
				else if (pTokurei->kind == DATAKIND_VAL) {
					m_SyzUtil.val_to_bin(
						(unsigned char*)(m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * idx_jigyokbn), 
						pTokurei->val);
				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					ratestr.Format(_T("%d"), pTokurei->rate);
					memmove(m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * idx_jigyokbn,
						ratestr.GetBuffer(), m_Uri1kbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_CHECK) {
                    *(m_Uri1kbndatainfo[tblidx].data + m_Uri1kbndatainfo[tblidx].size * idx_jigyokbn) = pTokurei->sign1;
				}
				return 1;
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(m_Uri1datainfo) / sizeof(m_Uri1datainfo[0]); tblidx++) {
		if (pTokurei->itemId == m_Uri1datainfo[tblidx].itemId) {

			if (pTokurei->kind == DATAKIND_STR) {
				memmove(m_Uri1datainfo[tblidx].data, pTokurei->str, m_Uri1datainfo[tblidx].size);
			}
			else if (pTokurei->kind == DATAKIND_VAL) {
				m_SyzUtil.val_to_bin((unsigned char*)m_Uri1datainfo[tblidx].data, pTokurei->val);
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				ratestr.Format(_T("%d"), pTokurei->rate);
				memmove(m_Uri1datainfo[tblidx].data, ratestr.GetBuffer(), m_Uri1datainfo[tblidx].size);
			}

			return 1;
		}
	}

    return 0;
}

int CH31HyoView::PrintTokurei_Uri2(CDC *pDC, int kojin, int pcnt)
{
	int pno = 61;
	PBUF pbuf[4] = { 0 }, pbuf2[16] = { 0 }, pbuf3[8] = { 0 };
	int bufidx = 0;
	BOOL isNoImage = FALSE;

	char* buf = nullptr;
	char* val = nullptr;
	int size = 1024;
	if ( ( buf = new char[size] ) == nullptr ) {
		return -1;
	}
	val = buf;
	memset ( val, '\0', size );


	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	//課税期間
	char kazeikikan_s[16] = { 0 }, kazeikikan_e[16] = { 0 };
	GetKazeiKikan(kazeikikan_s, kazeikikan_e);
	pbuf[bufidx].PITM[0] = kazeikikan_s;
	pbuf[bufidx++].PITM[1] = kazeikikan_e;

	//適用対象期間
	char tekiyotaisyokikan_s[16] = { 0 }, tekiyotaisyokikan_e[16] = { 0 };
	GetTekiyoTaisyoKikan(tekiyotaisyokikan_s, tekiyotaisyokikan_e);
	pbuf[bufidx].PITM[0] = tekiyotaisyokikan_s;
	pbuf[bufidx++].PITM[1] = tekiyotaisyokikan_e;

	//氏名
	char name[44] = { 0 };
	GetName(name, kojin);
	pbuf[bufidx++].PITM[0] = name;

	//フィールド1.
	if (_xprn(pno, 1, pbuf, 0, pDC, isNoImage)) {
		return -1;
	}

	//テーブルからデータ読み込み.
	ReadPrintTokurei_Uri2(m_SnSeq, TOKUREI_TBL_VERSION);

	bufidx = 0;
	for (int tblidx = 0; tblidx < (sizeof(m_Uri2kbndatainfo) / sizeof(m_Uri2kbndatainfo[0])); tblidx++) {

		for (int rowidx = 0; rowidx < CULM_PER_PAGE_TOKUREI_URI2; rowidx++) {

            if(m_Uri2kbndatainfo[tblidx].kind == DATAKIND_VAL ) {

                if((m_Uri2kbndatainfo[tblidx].itemId.Compare(_T("CVB00050")) == 0)
					|| (m_Uri2kbndatainfo[tblidx].itemId.Compare(_T("CVB00060")) == 0)
					|| (m_Uri2kbndatainfo[tblidx].itemId.Compare(_T("CVB00070")) == 0)
					|| (m_Uri2kbndatainfo[tblidx].itemId.Compare(_T("CVB00080")) == 0)
					|| (m_Uri2kbndatainfo[tblidx].itemId.Compare(_T("CVB00090")) == 0) ) {

					if (PRT_KSURI2_6_CHECK[CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx] == 0) {
						m_Arith.l_print(val,
							m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx),
							FMT15);
						pbuf2[bufidx].PITM[rowidx] = val;
						val += 16;
					}
				}
                else {

					m_Arith.l_print(val,
						m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx),
						FMT15);
					pbuf2[bufidx].PITM[rowidx] = val;
					val += 16;
				}
			}
			else if (m_Uri2kbndatainfo[tblidx].kind == DATAKIND_CHECK) {
                ;
			}
			else if (m_Uri2kbndatainfo[tblidx].kind == DATAKIND_RATE) {

				if (m_Arith.l_test(PRT_KSURI2_4[0 + (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx)])
					|| m_Arith.l_test(PRT_KSURI2_5[0 + (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx)])) {

					pbuf2[bufidx].PITM[rowidx] = m_Uri2kbndatainfo[tblidx].data
						+ m_Uri2kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx);
				}
                else if(PRT_KSURI2_6_CHECK[CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx]){

					pbuf2[bufidx].PITM[rowidx] = m_Uri2kbndatainfo[tblidx].data
						+ m_Uri2kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx);
				}

			}
            else {
				pbuf2[bufidx].PITM[rowidx] = m_Uri2kbndatainfo[tblidx].data
					+ m_Uri2kbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_URI2 * (pcnt - 1) + rowidx);
			}

			if (pcnt > MAX_PAGE_TOKUREI_URI2) {
				break;
			}

		}

		if (m_Uri2kbndatainfo[tblidx].kind != DATAKIND_CHECK) {
		    bufidx++;
		}

	}

	// 合計は最終ページのみ印字する
	if ( pcnt == m_Uri2MxPage ) {
		m_Arith.l_print(val, PRT_KSURI2_8_KEI, FMT15);
		pbuf2[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI2_9_KEI, FMT15);
		pbuf2[bufidx++].PITM[0] = val;
		val += 16;
	}
	else {
		pbuf2[bufidx++].PITM[0] = ( char* )0;
		pbuf2[bufidx++].PITM[0] = ( char* )0;
	}

	//フィールド2.
	if (_xprn(pno, 2, pbuf2, 0, pDC, isNoImage)) {
		return -1;
	}

	bufidx = 0;
	if ( pcnt == m_Uri2MxPage ) {
		m_Arith.l_print(val, PRT_KSURI2_10, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI2_11, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI2_12, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSURI2_13, FMT15);
		pbuf3[bufidx++].PITM[0] = val;
		val += 16;
	}
	else {
		pbuf3[bufidx++].PITM[0] = ( char* )0;
		pbuf3[bufidx++].PITM[0] = ( char* )0;
		pbuf3[bufidx++].PITM[0] = ( char* )0;
	}

	//フィールド3.
	if (_xprn(pno, 3, pbuf3, 0, pDC, isNoImage)) {
		return -1;
	}

	if ( buf ) {
		delete buf;
		buf = nullptr;
	}

	return 0;
}

int CH31HyoView::ReadPrintTokurei_Uri2(int sn_seq, int version)
{
	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_2_HYOID, version);

	if (m_DBSyzAccess.m_pSyz_Tokurei) {
		m_DBSyzAccess.m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return -1;
		}
	}

	CSYZ_TOKUREI* pSyzTokurei = m_DBSyzAccess.m_pSyz_Tokurei;
	if ((pSyzTokurei->MoveFirst() == -1) || (pSyzTokurei->st == -1)) {
		return 0;
	}
	else {

		for (int i = 0;; i++) {

			if (i == 0) {
				if ((pSyzTokurei->MoveFirst() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}
			else {
				if ((pSyzTokurei->MoveNext() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}

			if (ReadSubPrintTokurei_Uri2(pSyzTokurei)) {
				continue;
			}

		}
	}

	return 0;
}

int CH31HyoView::ReadSubPrintTokurei_Uri2(CSYZ_TOKUREI* pTokurei)
{
	for (int tblidx = 0; tblidx < (sizeof(m_Uri2kbndatainfo) / sizeof(m_Uri2kbndatainfo[0])); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			if ((pTokurei->itemId == m_Uri2kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {

				if (pTokurei->kind == DATAKIND_STR) {
					memmove(m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * idx_jigyokbn,
						pTokurei->str, m_Uri2kbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_VAL) {
					m_SyzUtil.val_to_bin(
						(unsigned char*)(m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * idx_jigyokbn),
						pTokurei->val);
				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					ratestr.Format(_T("%d"), pTokurei->rate);
					memmove(m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * idx_jigyokbn,
						ratestr.GetBuffer(), m_Uri2kbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_CHECK) {
					*(m_Uri2kbndatainfo[tblidx].data + m_Uri2kbndatainfo[tblidx].size * idx_jigyokbn) = pTokurei->sign1;
				}
				return 1;
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(m_Uri2datainfo) / sizeof(m_Uri2datainfo[0]); tblidx++) {
		if (pTokurei->itemId == m_Uri2datainfo[tblidx].itemId) {

			if (pTokurei->kind == DATAKIND_STR) {
				memmove(m_Uri2datainfo[tblidx].data, pTokurei->str, m_Uri2datainfo[tblidx].size);
			}
			else if (pTokurei->kind == DATAKIND_VAL) {
				m_SyzUtil.val_to_bin((unsigned char*)m_Uri2datainfo[tblidx].data, pTokurei->val);
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				ratestr.Format(_T("%d"), pTokurei->rate);
				memmove(m_Uri2datainfo[tblidx].data, ratestr.GetBuffer(), m_Uri2datainfo[tblidx].size);
			}

			return 1;
		}
	}

	return 0;
}

int CH31HyoView::PrintTokurei_Shiire(CDC *pDC, int kojin, int pcnt)
{
	int pno = 62;
	PBUF pbuf[4] = { 0 }, pbuf2[16] = { 0 }, pbuf3[16] = { 0 }, pbuf4[4] = {0};
	int bufidx = 0;
	BOOL isNoImage = FALSE;

	char* buf = nullptr;
	char* val = nullptr;
	int size = 1024;
	if ( ( buf = new char[size] ) == nullptr ) {
		return -1;
	}
	val = buf;
	memset ( val, '\0', size );

	if (m_pPrintCmInfo->PRtype & 0x80) {
		isNoImage = FALSE;	// イメージ出力有り
	}
	else {
		isNoImage = TRUE;	// イメージ出力無し
	}

	//課税期間
	char kazeikikan_s[16] = { 0 }, kazeikikan_e[16] = { 0 };
	GetKazeiKikan(kazeikikan_s, kazeikikan_e);
	pbuf[bufidx].PITM[0] = kazeikikan_s;
	pbuf[bufidx++].PITM[1] = kazeikikan_e;

	//適用対象期間
	char tekiyotaisyokikan_s[16] = { 0 }, tekiyotaisyokikan_e[16] = { 0 };
	GetTekiyoTaisyoKikan(tekiyotaisyokikan_s, tekiyotaisyokikan_e);
	pbuf[bufidx].PITM[0] = tekiyotaisyokikan_s;
	pbuf[bufidx++].PITM[1] = tekiyotaisyokikan_e;

	//氏名
	char name[44] = { 0 };
	GetName(name, kojin);
	pbuf[bufidx++].PITM[0] = name;

	//フィールド1.
	if (_xprn(pno, 1, pbuf, 0, pDC, isNoImage)) {
		return -1;
	}

	//テーブルからデータ読み込み.
	ReadPrintTokurei_Shiire(m_SnSeq, TOKUREI_TBL_VERSION);

	bufidx = 0;
	for (int tblidx = 0; tblidx < (sizeof(m_Shiirekbndatainfo) / sizeof(m_Shiirekbndatainfo[0])); tblidx++) {

		for (int rowidx = 0; rowidx < CULM_PER_PAGE_TOKUREI_SHIIRE; rowidx++) {

            if(m_Shiirekbndatainfo[tblidx].kind == DATAKIND_VAL) {
				m_Arith.l_print(val,
					m_Shiirekbndatainfo[tblidx].data + m_Shiirekbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_SHIIRE* (pcnt - 1) + rowidx),
					FMT15);
				pbuf2[bufidx].PITM[rowidx] = val;
				val += 16;
			}
            else if(m_Shiirekbndatainfo[tblidx].kind == DATAKIND_RATE) {

				if (m_Arith.l_test(PRT_KSHIIRE_1[0 + (CULM_PER_PAGE_TOKUREI_SHIIRE* (pcnt - 1) + rowidx)])
					|| m_Arith.l_test(PRT_KSHIIRE_2[0 + (CULM_PER_PAGE_TOKUREI_SHIIRE* (pcnt - 1) + rowidx)])) {

					pbuf2[bufidx].PITM[rowidx] = m_Shiirekbndatainfo[tblidx].data
						+ m_Shiirekbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_SHIIRE* (pcnt - 1) + rowidx);
				}
				else if ( ( PRT_KSHIIRE_3_SIGN[CULM_PER_PAGE_TOKUREI_SHIIRE * ( pcnt - 1 ) + rowidx] == 1 ) && ( m_pSnHeadData->Sn_Sign4 & 0x80 ) ) {
					pbuf2[bufidx].PITM[rowidx] = m_Shiirekbndatainfo[tblidx].data
						+ m_Shiirekbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_SHIIRE * (pcnt - 1) + rowidx);
				}
				//else if( Get50CheckStatusColumn0_5_1() && (rowidx == 0) ) {
				else if( Get50CheckStatusColumn0_5_1() && (rowidx == 0) && (m_pSnHeadData->m_DispTabSgn & 0x10) && ((m_pSnHeadData->Sn_Sign4 & 0x80) == 0x00)) {
					pbuf2[bufidx].PITM[rowidx] = m_Shiirekbndatainfo[tblidx].data
						+ m_Shiirekbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_SHIIRE* (pcnt - 1) + rowidx);
				}

			}
            else {
				pbuf2[bufidx].PITM[rowidx] = m_Shiirekbndatainfo[tblidx].data
					+ m_Shiirekbndatainfo[tblidx].size * (CULM_PER_PAGE_TOKUREI_SHIIRE * (pcnt - 1) + rowidx);
			}

			if (pcnt > MAX_PAGE_TOKUREI_SHIIRE) {
				break;
			}

		}

		if ( m_Shiirekbndatainfo[tblidx].kind != DATAKIND_SIGN ) {
			bufidx++;
		}
	}

	char* prt_shiire_2[] = { PRT_KSHIIRE_7_KEI, PRT_KSHIIRE_8_KEI, PRT_KSHIIRE_9, PRT_KSHIIRE_10 };
    for(int idx = 0; idx < (sizeof(prt_shiire_2)/sizeof(prt_shiire_2[0])); idx++){
		m_Arith.l_print(val, prt_shiire_2[idx], FMT15);

		// 合計は最終ページのみ印字する
		if ( pcnt == m_ShiireMxPage ) {
			pbuf2[bufidx++].PITM[0] = val;
			val += 16;
		}
		else {
			pbuf2[bufidx++].PITM[0] = ( char* )0;
		}
	}

	//フィールド2.
	if (_xprn(pno, 2, pbuf2, 0, pDC, isNoImage)) {
		return -1;
	}

	char* prt_shiire_3[] = { PRT_KSHIIRE_11_1, PRT_KSHIIRE_11_2, PRT_KSHIIRE_12_1, PRT_KSHIIRE_12_2, PRT_KSHIIRE_13_2,
							 PRT_KSHIIRE_14_2, PRT_KSHIIRE_15_1, PRT_KSHIIRE_15_2, PRT_KSHIIRE_16_1, PRT_KSHIIRE_16_2,
	                         PRT_KSHIIRE_17, PRT_KSHIIRE_18 };
	bufidx = 0;

	for (int idx = 0; idx < (sizeof(prt_shiire_3) / sizeof(prt_shiire_3[0])); idx++) {
		m_Arith.l_print(val, prt_shiire_3[idx], FMT15);

		if ( pcnt == m_ShiireMxPage ) {
			pbuf3[bufidx++].PITM[0] = val;
			val += 16;
		}
		else {
			pbuf3[bufidx++].PITM[0] = ( char* )0;
		}
	}

	//フィールド3.
	if (_xprn(pno, 3, pbuf3, 0, pDC, isNoImage)) {
		return -1;
	}

	//フィールド4.
	bufidx = 0;
	if ( pcnt == m_ShiireMxPage ) {
		m_Arith.l_print(val, PRT_KSHIIRE_19, FMT15);
		pbuf4[bufidx++].PITM[0] = val;
		val += 16;

		m_Arith.l_print(val, PRT_KSHIIRE_20, FMT15);
		pbuf4[bufidx++].PITM[0] = val;
		val += 16;
	}
	else {
		pbuf4[bufidx++].PITM[0] = ( char* )0;
		pbuf4[bufidx++].PITM[0] = ( char* )0;
	}

	if (_xprn(pno, 4, pbuf4, 0, pDC, isNoImage)) {
		return -1;
	}

	if ( buf ) {
		delete buf;
		buf = nullptr;
	}

	return 0;
}

int CH31HyoView::ReadPrintTokurei_Shiire(int sn_seq, int version)
{
	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_3_HYOID, version);

	if (m_DBSyzAccess.m_pSyz_Tokurei) {
		m_DBSyzAccess.m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return -1;
		}
	}

	CSYZ_TOKUREI* pSyzTokurei = m_DBSyzAccess.m_pSyz_Tokurei;
	if ((pSyzTokurei->MoveFirst() == -1) || (pSyzTokurei->st == -1)) {
		return 0;
	}
	else {

		for (int i = 0;; i++) {

			if (i == 0) {
				if ((pSyzTokurei->MoveFirst() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}
			else {
				if ((pSyzTokurei->MoveNext() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}

			if (ReadSubPrintTokurei_Shiire(pSyzTokurei)) {
				continue;
			}

		}
	}

	return 0;
}

int CH31HyoView::ReadSubPrintTokurei_Shiire(CSYZ_TOKUREI* pTokurei)
{
	for (int tblidx = 0; tblidx < (sizeof(m_Shiirekbndatainfo) / sizeof(m_Shiirekbndatainfo[0])); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			if ((pTokurei->itemId == m_Shiirekbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {

				if (pTokurei->kind == DATAKIND_STR) {
					memmove(m_Shiirekbndatainfo[tblidx].data + m_Shiirekbndatainfo[tblidx].size * idx_jigyokbn,
						pTokurei->str, m_Shiirekbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_VAL) {
					m_SyzUtil.val_to_bin(
						(unsigned char*)(m_Shiirekbndatainfo[tblidx].data + m_Shiirekbndatainfo[tblidx].size * idx_jigyokbn),
						pTokurei->val);
				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					ratestr.Format(_T("%d"), pTokurei->rate);
					memmove(m_Shiirekbndatainfo[tblidx].data + m_Shiirekbndatainfo[tblidx].size * idx_jigyokbn,
						ratestr.GetBuffer(), m_Shiirekbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_SIGN) {
					*(m_Shiirekbndatainfo[tblidx].data + m_Shiirekbndatainfo[tblidx].size * idx_jigyokbn) = pTokurei->sign1;
				}
				return 1;
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(m_Shiiredatainfo) / sizeof(m_Shiiredatainfo[0]); tblidx++) {
		if (pTokurei->itemId == m_Shiiredatainfo[tblidx].itemId) {

			if (pTokurei->kind == DATAKIND_STR) {
				memmove(m_Shiiredatainfo[tblidx].data, pTokurei->str, m_Shiiredatainfo[tblidx].size);
			}
			else if (pTokurei->kind == DATAKIND_VAL) {
				m_SyzUtil.val_to_bin((unsigned char*)m_Shiiredatainfo[tblidx].data, pTokurei->val);
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				ratestr.Format(_T("%d"), pTokurei->rate);
				memmove(m_Shiiredatainfo[tblidx].data, ratestr.GetBuffer(), m_Shiiredatainfo[tblidx].size);
			}

			return 1;
		}
	}

	return 0;
}

bool CH31HyoView::Get50CheckStatusColumn0_5_1()
{
	bool rt = false;

	//5-(1)号様式の1列目の□50%適用チェックボックスの状態を取得.
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CUB00105' and idx = %d"), m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION, 0);

	CSYZ_TOKUREI* pSyzTokurei = m_DBSyzAccess.m_pSyz_Tokurei;
	if (pSyzTokurei) {
		pSyzTokurei->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return rt;
		}
	}

	if (pSyzTokurei != NULL && pSyzTokurei->sign1) {
		rt = true;
	}

	return rt;
}

//特例帳票ページ取得.
int CH31HyoView::GetPageNumTokurei(int hyono)
{
    int* pMxPage[3] = { &m_Uri1MxPage,
                        &m_Uri2MxPage,
						&m_ShiireMxPage };

	return *pMxPage[hyono];
}

//-----------------------------------------------------------------------------
// 法人業種で法人番号未出力？
//-----------------------------------------------------------------------------
// 返送値	true	：	法人番号未出力
//			false	：	法人番号出力
//-----------------------------------------------------------------------------
bool CH31HyoView::IsNotPrintHojinNo()
{
	if ((m_pSnHeadData->IsSoleProprietor() == FALSE) && (m_pPrintCmInfo->OPU == 0) && !(m_pPrintCmInfo->PRtype & 0x80)) {
		return true;
	}
	return false;
}

int CH31HyoView::GetSyzSyukei_Ippan()
{
	// 課税標準額
	memmove ( XM1, m_pSnHonpyoData->Sn_KSTD, MONY_BUF_SIZE );

	// 消費税額
	memmove ( XM2, m_pSnHonpyoData->Sn_SYTX, MONY_BUF_SIZE );

	// 控除過大調整税額
	memmove ( XM3, m_pSnHonpyoData->Sn_KJADJZ, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( XM4, m_pSnHonpyoData->Sn_SIREZ, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( XM5, m_pSnHonpyoData->Sn_HKANZ, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( XM6, m_pSnHonpyoData->Sn_KSITZ, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( XM7, m_pSnHonpyoData->Sn_KJZSK, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( XM8, m_pSnHonpyoData->Sn_KJHKZ, MONY_BUF_SIZE );

	// 差引税額
	memmove ( XM9, m_pSnHonpyoData->Sn_SHZ, MONY_BUF_SIZE );

	// 中間納付税額
	memmove ( XM10, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE );

	// 納付税額
	memmove ( XM11, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );

	// 中間納付還付税額
	memmove ( XM12, m_pSnHonpyoData->Sn_TYNKNP, MONY_BUF_SIZE );

	// 既確定税額
	memmove ( XM13, m_pSnHonpyoData->Sn_KAKTIZ, MONY_BUF_SIZE );

	// 差引納付税額
	memmove ( XM14, m_pSnHonpyoData->Sn_SHN, MONY_BUF_SIZE );

	if( !IsNiwariTokurei() ) {
		// 課税資産の譲渡等の対価の額
		memmove ( XM15, m_pSnHonpyoData->Sn_KZURI, MONY_BUF_SIZE );

		// 資産の譲渡等の対価の額
		memmove ( XM16, m_pSnHonpyoData->Sn_SOURI, MONY_BUF_SIZE );
	}
	else {
		memset( XM15, '\0', MONY_BUF_SIZE );
		memset( XM16, '\0', MONY_BUF_SIZE );
	}

	// 控除不足還付税額
	memmove ( XM17, m_pSnHonpyoData->Sn_TKJHKZ, MONY_BUF_SIZE );

	// 差引税額
	memmove ( XM18, m_pSnHonpyoData->Sn_TSHZ, MONY_BUF_SIZE );

	// 還付額
	memmove ( XM19, m_pSnHonpyoData->Sn_KNP, MONY_BUF_SIZE );

	// 納税額
	memmove ( XM20, m_pSnHonpyoData->Sn_NOZ, MONY_BUF_SIZE );

	// 中間納付譲渡割額
	memmove ( XM21, m_pSnHonpyoData->Sn_TTYWAR, MONY_BUF_SIZE );

	// 納付譲渡割額
	memmove ( XM22, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );

	// 中間納付還付譲渡割額
	memmove ( XM23, m_pSnHonpyoData->Sn_TYNKNPJ, MONY_BUF_SIZE );

	// 既確定譲渡割額
	memmove ( XM24, m_pSnHonpyoData->Sn_TKAKTZ, MONY_BUF_SIZE );

	// 差引納付譲渡割額
	memmove ( XM25, m_pSnHonpyoData->Sn_SHNJ, MONY_BUF_SIZE );

	// 消費税及び地方消費税の合計(納付又は還付)税額
	memmove ( XM26, m_pSnHonpyoData->Sn_SYOTSG, MONY_BUF_SIZE );

	return 0;
}

int CH31HyoView::GetSyzSyukei_Kani()
{
	// 課税標準額
	memmove ( XM1, m_pSnHonpyoData->Sn_KSTD, MONY_BUF_SIZE );

	// 消費税額
	memmove ( XM2, m_pSnHonpyoData->Sn_SYTX, MONY_BUF_SIZE );

	// 控除過大調整税額
	memmove ( XM3, m_pSnHonpyoData->Sn_KJADJZ, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( XM4, m_pSnHonpyoData->Sn_SIREZ, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( XM5, m_pSnHonpyoData->Sn_HKANZ, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( XM6, m_pSnHonpyoData->Sn_KSITZ, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( XM7, m_pSnHonpyoData->Sn_KJZSK, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( XM8, m_pSnHonpyoData->Sn_KJHKZ, MONY_BUF_SIZE );

	// 差引税額
	memmove ( XM9, m_pSnHonpyoData->Sn_SHZ, MONY_BUF_SIZE );

	// 中間納付税額
	memmove ( XM10, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE );

	// 納付税額
	memmove ( XM11, m_pSnHonpyoData->Sn_EDNOFZ, MONY_BUF_SIZE );

	// 中間納付還付税額
	memmove ( XM12, m_pSnHonpyoData->Sn_TYNKNP, MONY_BUF_SIZE );

	// 既確定税額
	memmove ( XM13, m_pSnHonpyoData->Sn_KAKTIZ, MONY_BUF_SIZE );

	// 差引納付税額
	memmove ( XM14, m_pSnHonpyoData->Sn_SHN, MONY_BUF_SIZE );

	if( !IsNiwariTokurei()) {
		// この課税資産の譲渡等の対価の額
		memmove ( XM15, m_pSnHonpyoData->Sn_KZURI, MONY_BUF_SIZE );
	}
	else {
		memset( XM15, '\0', MONY_BUF_SIZE );
	}

	// 基準期間の課税売上高
	memmove ( XM16, m_pSnHonpyoData->Sn_ZZURI, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( XM17, m_pSnHonpyoData->Sn_TKJHKZ, MONY_BUF_SIZE );

	// 差引税額
	memmove ( XM18, m_pSnHonpyoData->Sn_TSHZ, MONY_BUF_SIZE );

	// 還付額
	memmove ( XM19, m_pSnHonpyoData->Sn_KNP, MONY_BUF_SIZE );

	// 納税額
	memmove ( XM20, m_pSnHonpyoData->Sn_NOZ, MONY_BUF_SIZE );

	// 中間納付譲渡割額
	memmove ( XM21, m_pSnHonpyoData->Sn_TTYWAR, MONY_BUF_SIZE );

	// 納付譲渡割額
	memmove ( XM22, m_pSnHonpyoData->Sn_TEDNOF, MONY_BUF_SIZE );

	// 中間納付還付譲渡割額
	memmove ( XM23, m_pSnHonpyoData->Sn_TYNKNPJ, MONY_BUF_SIZE );

	// 既確定譲渡割額
	memmove ( XM24, m_pSnHonpyoData->Sn_TKAKTZ, MONY_BUF_SIZE );

	// 差引納付譲渡割額
	memmove ( XM25, m_pSnHonpyoData->Sn_SHNJ, MONY_BUF_SIZE );

	// 消費税及び地方消費税の合計(納付又は還付)税額
	memmove ( XM26, m_pSnHonpyoData->Sn_SYOTSG, MONY_BUF_SIZE );

	return 0;
}

int CH31HyoView::GetSyzSyukei_Ippan2()
{
	// 課税標準額
	memmove(XM2_1, m_pSnHonpyo2Data->Sn_XM1, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額 3%適用分
	memmove(XM2_2, m_pSnHonpyo2Data->Sn_XM2, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額 4%適用分
	memmove(XM2_3, m_pSnHonpyo2Data->Sn_XM3, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額 6.3%適用分
	memmove(XM2_4, m_pSnHonpyo2Data->Sn_XM4, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額 6.24%適用分
	memmove(XM2_5, m_pSnHonpyo2Data->Sn_XM5, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額 7.8%適用分
	memmove(XM2_6, m_pSnHonpyo2Data->Sn_XM6, MONY_BUF_SIZE);

	// 課税資産の譲渡等の対価の額
	memmove(XM2_7, m_pSnHonpyo2Data->Sn_XM7, MONY_BUF_SIZE);

	// 特定課税仕入れに係る支払い対価の額 6.3%適用分
	memmove(XM2_8, m_pSnHonpyo2Data->Sn_XM8, MONY_BUF_SIZE);

	// 特定課税仕入れに係る支払い対価の額 7.8%適用分
	memmove(XM2_9, m_pSnHonpyo2Data->Sn_XM9, MONY_BUF_SIZE);

	// 特定課税仕入れに係る支払い対価の額
	memmove(XM2_10, m_pSnHonpyo2Data->Sn_XM10, MONY_BUF_SIZE);

	// 消費税額
	memmove(XM2_11, m_pSnHonpyo2Data->Sn_XM11, MONY_BUF_SIZE);

	// 11の内訳 3%適用分
	memmove(XM2_12, m_pSnHonpyo2Data->Sn_XM12, MONY_BUF_SIZE);

	// 11の内訳 4%適用分
	memmove(XM2_13, m_pSnHonpyo2Data->Sn_XM13, MONY_BUF_SIZE);

	// 11の内訳 6.3%適用分
	memmove(XM2_14, m_pSnHonpyo2Data->Sn_XM14, MONY_BUF_SIZE);

	// 11の内訳 6.24%適用分
	memmove(XM2_15, m_pSnHonpyo2Data->Sn_XM15, MONY_BUF_SIZE);

	// 11の内訳 7.8%適用分
	memmove(XM2_16, m_pSnHonpyo2Data->Sn_XM16, MONY_BUF_SIZE);

	// 返還等対価に係る税額
	memmove(XM2_17, m_pSnHonpyo2Data->Sn_XM17, MONY_BUF_SIZE);

	// 売上げの返還等対価に係る税額
	memmove(XM2_18, m_pSnHonpyo2Data->Sn_XM18, MONY_BUF_SIZE);

	// 特定課税仕入れの返還等対価に係る税額
	memmove(XM2_19, m_pSnHonpyo2Data->Sn_XM19, MONY_BUF_SIZE);

	// 地方消費税の課税標準となる消費税額
	memmove(XM2_20, m_pSnHonpyo2Data->Sn_XM20, MONY_BUF_SIZE);

	// 地方消費税の課税標準となる消費税額 4%適用分
	memmove(XM2_21, m_pSnHonpyo2Data->Sn_XM21, MONY_BUF_SIZE);

	// 地方消費税の課税標準となる消費税額 6.3%適用分
	memmove(XM2_22, m_pSnHonpyo2Data->Sn_XM22, MONY_BUF_SIZE);

	// 地方消費税の課税標準となる消費税額 6.24%及び7.8%適用分
	memmove(XM2_23, m_pSnHonpyo2Data->Sn_XM23, MONY_BUF_SIZE);

	return 0;
}

//==================================
// 特例テーブルクローズ
//==================================
void CH31HyoView::CloseSyzTokurei()
{
	if ( m_DBSyzAccess.m_pSyz_Tokurei ) {
		m_DBSyzAccess.Syz_TokureiClose();
	}
}

//-----------------------------------------------------------------------------
// 元号の○印 未出力？
//-----------------------------------------------------------------------------
// 返送値	true	：	未出力
//			false	：	出力
//-----------------------------------------------------------------------------
bool CH31HyoView::IsNotPrintGengoCircle()
{
	if (!(m_pPrintCmInfo->PRtype & 0x80) && (m_pPrintCmInfo->OP_GENGO == 0)) {
		return true;
	}
	return false;
}