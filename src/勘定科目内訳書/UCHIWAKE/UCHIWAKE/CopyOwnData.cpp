#include "StdAfx.h"
#include "CopyOwnData.h"
#include "CtrlSubGeneralVar.h"

#define	OWNTB_APL_NAME			"DBUchiwake"				// apl_name
#define OWNTB_ITM_NAME			"DBUchiwake_SETUP"			// itm_name
#define OWNTB_ITM_SEQ			1							// itm_seq
#define	OWNTB_USER_ID			0							// u_id

CCopyOwnData::CCopyOwnData(CDBZmSub* pZmSub):m_ZmSub(NULL)
{
	if(pZmSub == NULL)	return;
	m_ZmSub = pZmSub;
}

CCopyOwnData::~CCopyOwnData(void)
{
}

BOOL CCopyOwnData::GetOwnTblInfo(OWN_INFO& own_info)
{
	if(m_ZmSub == NULL)	return FALSE;

	CString		strFilter;
	strFilter.Format(  _T("apl_name = '%s' AND itm_name = '%s'"), OWNTB_APL_NAME, OWNTB_ITM_NAME );

	if( m_ZmSub->owntb == NULL ) {
		if( m_ZmSub->OwnTblOpen() == -1 ){
			return FALSE;
		}
	}

	// 既存レコードがないならコピーサインを立てるだけ
	int st = m_ZmSub->owntb->Requery(strFilter);
	if(st == -1){
		m_ZmSub->OwnTblClose();

		CdbUcInfMain mfcMain(m_ZmSub->m_database);
		if (mfcMain.Init() != DB_ERR_OK) {
			mfcMain.Fin();
			return FALSE;
		}
		CCtrlMainGeneralVar cm(mfcMain);

		mfcMain.Edit();
		cm.bCopyOwnData = TRUE;
		cm.Set(mfcMain);
		mfcMain.Update();	
		return FALSE;
	}

	//　保存する項目が増えた場合、複数のvlを用いるのではなくビット演算を使用すること
	if( m_ZmSub->owntb->vl[0] == -1 )	m_ZmSub->owntb->vl[0] = 0;
	own_info.youshiki_skip	= m_ZmSub->owntb->vl[0];

	if( m_ZmSub->owntb->vl[1] == -1 )	m_ZmSub->owntb->vl[1] = 0;
	own_info.ikkatu_outzero = m_ZmSub->owntb->vl[1];

	if( m_ZmSub->owntb->vl[2] == -1 )	m_ZmSub->owntb->vl[2] = 0;
	own_info.name_kei = m_ZmSub->owntb->vl[2];

	// レコードクローズ
	m_ZmSub->OwnTblClose();

	return TRUE;
}

BOOL CCopyOwnData::SetUchiwakeTables(const OWN_INFO& own_info)
{
	map<int, INF_SUB_COPY_DATA> inf_sub_map;

	// owntbにビットで持っているデータをベクタに置換
	int max_cnt = sizeof(old_map) / sizeof(old_map[0]);
	for(int cnt=0; cnt<max_cnt; cnt++){
		INF_SUB_COPY_DATA sub_data;

		sub_data.name_kei= GetFormSeqNameKei(old_map[cnt], own_info.name_kei);
		sub_data.ikkatu_outzero = GetIkkatuOutZero(old_map[cnt], own_info.ikkatu_outzero);

		inf_sub_map[old_map[cnt]] = sub_data;
	}

	// uc_inf_sub、uc_inf_mainにコピー
	CopyFromOwnTable(own_info.youshiki_skip, inf_sub_map);
	return TRUE;
}

BOOL CCopyOwnData::CopyFromOwnTable(int youshiki_skip, map<int, INF_SUB_COPY_DATA>& inf_sub_map)
{
	// uc_inf_subにコピー
	CdbUcInfSub mfcSub(m_ZmSub->m_database);
	for(map<int, INF_SUB_COPY_DATA>::iterator it = inf_sub_map.begin(); it != inf_sub_map.end(); it++){
		mfcSub.RequeryFormSeq(it->first);
		if(mfcSub.IsEOF())	continue;

		mfcSub.MoveFirst();
		CCtrlSubGeneralVar cs(mfcSub);
		mfcSub.Edit();
		// 一括金額に０円を含まない
		if(it->second.ikkatu_outzero)	cs.bIkkatuOutZero = TRUE;
		else							cs.bIkkatuOutZero = FALSE;
		// 計名称
		if(it->second.name_kei)			cs.bNameKei		  = TRUE;
		else							cs.bNameKei		  = FALSE;

		cs.Set(mfcSub);
		mfcSub.Update();
	}
	mfcSub.Fin();

	// uc_inf_mainにコピー
	CdbUcInfMain mfcMain(m_ZmSub->m_database);
	if (mfcMain.Init() != DB_ERR_OK) {
		mfcMain.Fin();
		return FALSE;
	}
	CCtrlMainGeneralVar cm(mfcMain);

	mfcMain.Edit();
	// データのある様式へ移動する
	if(youshiki_skip)	cm.bYoushikiSkip = TRUE;
	else				cm.bYoushikiSkip = FALSE;

	// コピー済みサイン
	cm.bCopyOwnData = TRUE;
	cm.Set(mfcMain);
	mfcMain.Update();	
	mfcMain.Fin();

	return TRUE;
}

BOOL CCopyOwnData::Copy()
{
	if(m_ZmSub == NULL) return FALSE;

	OWN_INFO own_info;
	int st = GetOwnTblInfo(own_info);
	if(st == FALSE)	return FALSE;

	st = SetUchiwakeTables(own_info);
	if(st == FALSE)	return FALSE;

	return TRUE;
}

int CCopyOwnData::GetFormSeqNameKei(int nFormSeq, int nNameKei)
{
	CdbUcInfSub		mfcRecSub(m_ZmSub->m_database);	//	uc_inf_mainテーブル
	int	nBit		= BIT_D0;
	int nFormOrder = 1;

	int max_cnt = sizeof(old_map) / sizeof(old_map[0]);
	int cnt;
	for(cnt=0; cnt<max_cnt; cnt++){
		if(old_map[cnt] == nFormSeq)	break;
	}

	nBit = nBit << (cnt);

	return nNameKei & nBit;
}

BOOL CCopyOwnData::GetIkkatuOutZero(int nFormSeq, int isIkkatuZero)
{
	int nBit = 0;
	int nRet = 0;

	switch( nFormSeq ){
		case ID_FORMNO_021:
			nBit = BIT_D0;
			break;
		case ID_FORMNO_031:
			nBit = BIT_D1;
			break;
		case ID_FORMNO_041:
			nBit = BIT_D2;
			break;
		case ID_FORMNO_042:
			nBit = BIT_D3;
			break;
		case ID_FORMNO_051:
			nBit = BIT_D4;
			break;
		case ID_FORMNO_081:
			nBit = BIT_D5;
			break;
		case ID_FORMNO_091:
			nBit = BIT_D6;
			break;
		case ID_FORMNO_101:
			nBit = BIT_D7;
			break;
		case ID_FORMNO_111:
			nBit = BIT_D8;
			break;
		case ID_FORMNO_161:
			nBit = BIT_D9;
			break;
		case ID_FORMNO_162:
			nBit = BIT_D10;
			break;
		case ID_FORMNO_171:
			nBit = BIT_D11;
			break;
		case ID_FORMNO_172:
			nBit = BIT_D12;
			break;
		case ID_FORMNO_173:
			nBit = BIT_D13;
			break;
		case ID_FORMNO_174:
			nBit = BIT_D14;
			break;
		case ID_FORMNO_175:
			nBit = BIT_D15;
			break;
		case ID_FORMNO_181:
			nBit = BIT_D16;
			break;
		case ID_FORMNO_182:
			nBit = BIT_D17;
			break;
		case ID_FORMNO_183:
			nBit = BIT_D18;
			break;
		case ID_FORMNO_184:
			nBit = BIT_D19;
			break;
		case ID_FORMNO_185:
			nBit = BIT_D20;
			break;
	}

	if( nBit != 0 ){
		nRet = isIkkatuZero & nBit;
	}

	return nRet;
}