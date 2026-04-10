// ChangeTaxEx.cpp : é¿ëïÉtÉ@ÉCÉã
//

#include "stdafx.h"
#include "ChangeTaxEx.h"
#include "ChangeTaxExIdx.h"


// CChangeTaxEx É_ÉCÉAÉçÉO

IMPLEMENT_DYNAMIC(CChangeTaxEx, ICSDialog)

CChangeTaxEx::CChangeTaxEx(CWnd* pParent /*=NULL*/)
	: ICSDialog(CChangeTaxEx::IDD, pParent)
	, m_pSnFhyo2_02Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pSnTanaData(NULL)
	, m_CbSw(0)
	, m_ChSw(0)
	, m_CbSt(0)
{
	memset(MNK40K, '\0', sizeof(MNK40K));
	memset(MNZ40K, '\0', sizeof(MNZ40K));
	memset(MNK40H, '\0', sizeof(MNK40H));
	memset(MNZ40H, '\0', sizeof(MNZ40H));
	memset(MNK40C, '\0', sizeof(MNK40C));
	memset(MNZ40C, '\0', sizeof(MNZ40C));
	memset(MENZKK, '\0', sizeof(MENZKK));
	memset(MENZZK, '\0', sizeof(MENZZK));
	memset(MENZKH, '\0', sizeof(MENZKH));
	memset(MENZZH, '\0', sizeof(MENZZH));
	memset(MENZKC, '\0', sizeof(MENZKC));
	memset(MENZZC, '\0', sizeof(MENZZC));

	memset(MNK63K, '\0', sizeof(MNK63K));
	memset(MNZ63K, '\0', sizeof(MNZ63K));
	memset(MNK63H, '\0', sizeof(MNK63H));
	memset(MNZ63H, '\0', sizeof(MNZ63H));
	memset(MNK63C, '\0', sizeof(MNK63C));
	memset(MNZ63C, '\0', sizeof(MNZ63C));
	tansw = NULL;
	tmpChSw = 0;

}

CChangeTaxEx::~CChangeTaxEx()
{
}

void CChangeTaxEx::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_ICSDIAG1);
	DDX_Control(pDX, IDC_STA1, m_txt1);
}


BEGIN_MESSAGE_MAP(CChangeTaxEx, ICSDialog)
END_MESSAGE_MAP()


// CChangeTaxEx ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ

BOOL CChangeTaxEx::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  Ç±Ç±Ç…èâä˙âªÇí«â¡ÇµÇƒÇ≠ÇæÇ≥Ç¢

	// èâä˙âª
	Init();

	ICSDialog::OnInitDialogEX();

	return FALSE;  // ÉRÉìÉgÉçÅ[ÉãÇ…ÉtÉHÅ[ÉJÉXÇê›íËÇµÇ»Ç¢Ç∆Ç´ÅAñﬂÇËílÇÕ TRUE Ç∆Ç»ÇËÇ‹Ç∑
				  // ó·äO: OCX ÉvÉçÉpÉeÉB ÉyÅ[ÉWÇÃñﬂÇËílÇÕ FALSE Ç∆Ç»ÇËÇ‹Ç∑
}

//-----------------------------------------------------------------------------
// èâä˙èÓïÒÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSnFhyo20Data	ÅF	ïtï\ÇQÉfÅ[É^ì«çûÇ›ÉNÉâÉX
//		pSnHeadData		ÅF	ñ{ï\ÉwÉbÉ_ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
//		pSnTanaData		ÅF	íIâµí≤êÆÉfÅ[É^ì«çûÇ›ÉNÉâÉX
//		pArith			ÅF	î{í∑ââéZÉNÉâÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0			ÅF	ê≥èÌèIóπ
//			-1			ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CChangeTaxEx::InitInfo(CH31SnFhyo2_02Data *pSnFhyo2_02Data, CSnHeadData *pSnHeadData, CH31SnTanaData *pSnTanaData, CArithEx *pArith)
{
	ASSERT(pSnFhyo2_02Data);
	if (pSnFhyo2_02Data == NULL) {
		return -1;
	}
	m_pSnFhyo2_02Data = pSnFhyo2_02Data;

	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pSnTanaData);
	if (pSnTanaData == NULL) {
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

	ASSERT(pArith);
	if (pArith == NULL) {
		return -1;
	}
	m_pArith = pArith;

	return 0;
}

//-----------------------------------------------------------------------------
// èâä˙âª
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//			-1	ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CChangeTaxEx::Init()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char				buf[512] = { 0 };

	tansw = m_pSnTanaData->Sn_MENZsw;

	// É^ÉCÉgÉãï\é¶
	switch (m_pSnTanaData->Sn_MENZsw & 0xff) {
	case 0x10:	// ñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç÷
		m_CbSw = 0;

		memset(buf, '\0', sizeof(buf));
		strcpy_s(buf, sizeof(buf), _T("ìñä˙ÇÊÇËÅAñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃâ€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz"));
		DiagData.data_disp = buf;
		diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl);

		memset(buf, '\0', sizeof(buf));
		strcpy_s(buf, sizeof(buf), _T("çTèúëŒè€édì¸äz"));
		DiagData.data_disp = buf;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ6, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ4, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		break;

	case 0x11:	// â€ê≈ã∆é“Ç©ÇÁñ∆ê≈ã∆é“Ç÷
		m_CbSw = 1;

		memset(buf, '\0', sizeof(buf));
		strcpy_s(buf, sizeof(buf), _T("óÇä˙ÇÊÇËÅAâ€ê≈ã∆é“Ç©ÇÁñ∆ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃâ€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz"));
		DiagData.data_disp = buf;
		diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&DiagData, CIcsdiagctrl);

		memset(buf, '\0', sizeof(buf));
		strcpy_s(buf, sizeof(buf), _T("çTèúëŒè€äOédì¸äz"));
		DiagData.data_disp = buf;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ6, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ4, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		break;

	default:	// ñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç÷
		m_pSnTanaData->Sn_MENZsw = 0x00;
		m_pSnTanaData->Sn_tansw &= 0xfe;
		m_CbSw = 2;

		diag_clear(IDC_ICSDIAGCTRL1, IDTITLE, TRUE, CIcsdiagctrl);

		memset(buf, '\0', sizeof(buf));
		strcpy_s(buf, sizeof(buf), _T("çTèúëŒè€édì¸äz"));
		DiagData.data_disp = buf;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ6, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ4, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);

		break;
	}

	// èàóùï\é¶
	DiagData.data_combo = m_CbSw;
	diag_setdata(IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// çTèúëŒè€äOédì¸ê≈äz
	if (m_pSnHeadData->SVsign & 0x10) {	// ê≈çûÇ›
		m_ChSw = 1;
		tmpChSw = 1;
		DiagData.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		DiagData.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}
	else {						// ê≈î≤Ç´
		m_ChSw = 0;
		tmpChSw = 0;
		DiagData.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		DiagData.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}

	// ëSã‡äzÉNÉäÉA
	memset(MNK40K, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@â€ê≈Ç…åWÇÈ
	memset(MNZ40K, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	memset(MNK40H, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
	memset(MNZ40H, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	memset(MNK40C, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@ã§í Ç…åWÇÈ
	memset(MNZ40C, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

	memset(MENZKK, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@â€ê≈Ç…åWÇÈ
	memset(MENZZK, '\0', MONY_BUF_SIZE);	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	memset(MENZKH, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
	memset(MENZZH, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	memset(MENZKC, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@ã§í Ç…åWÇÈ
	memset(MENZZC, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

	memset(MNK63K, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@â€ê≈Ç…åWÇÈ
	memset(MNZ63K, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	memset(MNK63H, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
	memset(MNZ63H, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	memset(MNK63C, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@ã§í Ç…åWÇÈ
	memset(MNZ63C, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

	memset(MNK624K, '\0', MONY_BUF_SIZE);
	memset(MNZ624K, '\0', MONY_BUF_SIZE);
	memset(MNK624H, '\0', MONY_BUF_SIZE);
	memset(MNZ624H, '\0', MONY_BUF_SIZE);
	memset(MNK624C, '\0', MONY_BUF_SIZE);
	memset(MNZ624C, '\0', MONY_BUF_SIZE);

	memset(MNK78K, '\0', MONY_BUF_SIZE);
	memset(MNZ78K, '\0', MONY_BUF_SIZE);
	memset(MNK78H, '\0', MONY_BUF_SIZE);
	memset(MNZ78H, '\0', MONY_BUF_SIZE);
	memset(MNK78C, '\0', MONY_BUF_SIZE);
	memset(MNZ78C, '\0', MONY_BUF_SIZE);

	// ã‡äzï\é¶
	if (m_pSnTanaData->Sn_MENZsw & 0xff) {
		memmove(MNK63K, m_pSnTanaData->Sn_mnk63[0], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(MNZ63K, m_pSnTanaData->Sn_mnz63[0], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(MNK63H, m_pSnTanaData->Sn_mnk63[1], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(MNZ63H, m_pSnTanaData->Sn_mnz63[1], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(MNK63C, m_pSnTanaData->Sn_mnk63[2], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@ã§í Ç…åWÇÈ
		memmove(MNZ63C, m_pSnTanaData->Sn_mnz63[2], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		if (m_pSnHeadData->SVmzsw == 1) {
			memmove(MNK40K, m_pSnTanaData->Sn_mnk40[0], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@â€ê≈Ç…åWÇÈ
			memmove(MNZ40K, m_pSnTanaData->Sn_mnz40[0], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
			memmove(MNK40H, m_pSnTanaData->Sn_mnk40[1], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
			memmove(MNZ40H, m_pSnTanaData->Sn_mnz40[1], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
			memmove(MNK40C, m_pSnTanaData->Sn_mnk40[2], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@ã§í Ç…åWÇÈ
			memmove(MNZ40C, m_pSnTanaData->Sn_mnz40[2], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

			memmove(MENZKK, m_pSnTanaData->Sn_menzk[0], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@â€ê≈Ç…åWÇÈ
			memmove(MENZZK, m_pSnTanaData->Sn_menzz[0], MONY_BUF_SIZE);	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
			memmove(MENZKH, m_pSnTanaData->Sn_menzk[1], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
			memmove(MENZZH, m_pSnTanaData->Sn_menzz[1], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
			memmove(MENZKC, m_pSnTanaData->Sn_menzk[2], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@ã§í Ç…åWÇÈ
			memmove(MENZZC, m_pSnTanaData->Sn_menzz[2], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		}

		memmove(MNK78K, m_pSnTanaData->Sn_mnk78[0], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(MNZ78K, m_pSnTanaData->Sn_mnz78[0], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(MNK78H, m_pSnTanaData->Sn_mnk78[1], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(MNZ78H, m_pSnTanaData->Sn_mnz78[1], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(MNK78C, m_pSnTanaData->Sn_mnk78[2], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@ã§í Ç…åWÇÈ
		memmove(MNZ78C, m_pSnTanaData->Sn_mnz78[2], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		memmove(MNK624K, m_pSnTanaData->Sn_mnk624[0], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(MNZ624K, m_pSnTanaData->Sn_mnz624[0], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(MNK624H, m_pSnTanaData->Sn_mnk624[1], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(MNZ624H, m_pSnTanaData->Sn_mnz624[1], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(MNK624C, m_pSnTanaData->Sn_mnk624[2], MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@ã§í Ç…åWÇÈ
		memmove(MNZ624C, m_pSnTanaData->Sn_mnz624[2], MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	}

	DispMony(IDMNK63K, MNK63K);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@â€ê≈Ç…åWÇÈ
	DispMony(IDMNZ63K, MNZ63K);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	DispMony(IDMNK63H, MNK63H);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
	DispMony(IDMNZ63H, MNZ63H);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	DispMony(IDMNK63C, MNK63C);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@ã§í Ç…åWÇÈ
	DispMony(IDMNZ63C, MNZ63C);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

	if (m_pSnHeadData->SVmzsw == 1) {

		DispMony(IDMNK40K, MNK40K);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@â€ê≈Ç…åWÇÈ
		DispMony(IDMNZ40K, MNZ40K);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		DispMony(IDMNK40H, MNK40H);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		DispMony(IDMNZ40H, MNZ40H);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		DispMony(IDMNK40C, MNK40C);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@ã§í Ç…åWÇÈ
		DispMony(IDMNZ40C, MNZ40C);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		DispMony(IDMENZKK, MENZKK);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@â€ê≈Ç…åWÇÈ
		DispMony(IDMENZZK, MENZZK);	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		DispMony(IDMENZKH, MENZKH);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		DispMony(IDMENZZH, MENZZH);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		DispMony(IDMENZKC, MENZKC);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™ï™)Å@ã§í Ç…åWÇÈ
		DispMony(IDMENZZC, MENZZC);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
	}

	ComboCheck_AttrSet();


	return 0;
}

void CChangeTaxEx::OnOK()
{
	// TODO: Ç±Ç±Ç…ì¡íËÇ»ÉRÅ[ÉhÇí«â¡Ç∑ÇÈÇ©ÅAÇ‡ÇµÇ≠ÇÕäÓñ{ÉNÉâÉXÇåƒÇ—èoÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB
	if (!(m_pSnTanaData->Sn_tansw & 0x01) || (m_pSnHeadData->Sn_Sign4 & 0x80)) {

		char	buf[512] = { 0 };

		if (m_CbSw == 2) {
			// åxçêÉÅÉbÉZÅ[ÉW
			sprintf_s(buf, sizeof(buf), _T("â€ê≈ï˚éÆÇÃêÿë÷Ç¶ÇâèúÇµÇ‹Ç∑Ç©ÅH"));
			if (ICSMessageBox(buf, MB_YESNO, 0, 0, this) == IDYES) {
				m_pSnTanaData->Sn_MENZsw = 0x00;
				tansw = 0x00;
				memset(MNK40K, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@â€ê≈Ç…åWÇÈ
				memset(MNZ40K, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK40H, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
				memset(MNZ40H, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK40C, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@ã§í Ç…åWÇÈ
				memset(MNZ40C, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

				memset(MENZKK, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@â€ê≈Ç…åWÇÈ
				memset(MENZZK, '\0', MONY_BUF_SIZE);	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MENZKH, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
				memset(MENZZH, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MENZKC, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@ã§í Ç…åWÇÈ
				memset(MENZZC, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

				memset(MNK63K, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@â€ê≈Ç…åWÇÈ
				memset(MNZ63K, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK63H, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
				memset(MNZ63H, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK63C, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@ã§í Ç…åWÇÈ
				memset(MNZ63C, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

				memset(m_pSnTanaData->Sn_MNK40, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)
				memset(m_pSnTanaData->Sn_MNZ40, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				memset(m_pSnTanaData->Sn_MENZK, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)
				memset(m_pSnTanaData->Sn_MENZZ, '\0', MONY_BUF_SIZE);	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )
				memset(m_pSnTanaData->Sn_MNK63, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)
				memset(m_pSnTanaData->Sn_MNZ63, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )

				memset(MNK624K, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@â€ê≈Ç…åWÇÈ
				memset(MNZ624K, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK624H, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
				memset(MNZ624H, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK624C, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@ã§í Ç…åWÇÈ
				memset(MNZ624C, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

				memset(MNK78K, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@â€ê≈Ç…åWÇÈ
				memset(MNZ78K, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK78H, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
				memset(MNZ78H, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
				memset(MNK78C, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@ã§í Ç…åWÇÈ
				memset(MNZ78C, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

				memset(m_pSnTanaData->Sn_MNK624, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)
				memset(m_pSnTanaData->Sn_MNZ624, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )
				memset(m_pSnTanaData->Sn_MNK78, '\0', MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)
				memset(m_pSnTanaData->Sn_MNZ78, '\0', MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )

			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
				return;
			}
		}
		memmove(m_pSnTanaData->Sn_mnk40[0], MNK40K, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz40[0], MNZ40K, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk40[1], MNK40H, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz40[1], MNZ40H, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk40[2], MNK40C, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶4%ï™)Å@ã§í Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz40[2], MNZ40C, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_menzk[0], MENZKK, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_menzz[0], MENZZK, MONY_BUF_SIZE);	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_menzk[1], MENZKH, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_menzz[1], MENZZH, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_menzk[2], MENZKC, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶3%ï™)Å@ã§í Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_menzz[2], MENZZC, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		memmove(m_pSnTanaData->Sn_mnk63[0], MNK63K, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz63[0], MNZ63K, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk63[1], MNK63H, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz63[1], MNZ63H, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk63[2], MNK63C, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.3%ï™)Å@ã§í Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz63[2], MNZ63C, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		memmove(m_pSnTanaData->Sn_mnk624[0], MNK624K, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz624[0], MNZ624K, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk624[1], MNK624H, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz624[1], MNZ624H, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk624[2], MNK624C, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶6.24%ï™)Å@ã§í Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz624[2], MNZ624C, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		memmove(m_pSnTanaData->Sn_mnk78[0], MNK78K, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@â€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz78[0], MNZ78K, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk78[1], MNK78H, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz78[1], MNZ78H, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		memmove(m_pSnTanaData->Sn_mnk78[2], MNK78C, MONY_BUF_SIZE);	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ê≈ó¶7.8%ï™)Å@ã§í Ç…åWÇÈ
		memmove(m_pSnTanaData->Sn_mnz78[2], MNZ78C, MONY_BUF_SIZE);	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV

		m_pSnTanaData->Sn_MENZsw = tansw;

		//OKÇ®Ç≥ÇÍÇΩéûÇ…ê≈çûÇ›ÅAê≈î≤Ç´ÉTÉCÉìÇï€ë∂Ç∑ÇÈÇÊÇ§Ç…Ç∑ÇÈÅBÉLÉÉÉìÉZÉãÇ®Ç≥ÇÍÇΩèÍçáÇ‡ïœÇÌÇ¡ÇƒÇ¢ÇΩà◊
		if (m_ChSw == 1) {//ê≈çûÇ›
			m_pSnHeadData->SVsign |= 0x10;
		}
		else {//ê≈î≤Ç´
			m_pSnHeadData->SVsign &= 0xef;
		}

	}

	ICSDialog::OnOK();
}

void CChangeTaxEx::OnCancel()
{
	// TODO: Ç±Ç±Ç…ì¡íËÇ»ÉRÅ[ÉhÇí«â¡Ç∑ÇÈÇ©ÅAÇ‡ÇµÇ≠ÇÕäÓñ{ÉNÉâÉXÇåƒÇ—èoÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB

	ICSDialog::OnCancel();
}

//-----------------------------------------------------------------------------
// ã‡äzï\é¶
//-----------------------------------------------------------------------------
// à¯êî	index	ÅF	É_ÉCÉAÉOÉâÉÄì‡ÉCÉìÉfÉbÉNÉX
//		Mony	ÅF	ï\é¶Ç∑ÇÈã‡äz
//-----------------------------------------------------------------------------
void CChangeTaxEx::DispMony(short Index, char *Mony)
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	memmove(DiagData.data_val, Mony, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
// ì¸óÕçÄñ⁄Å@êFïœÇ¶
//-----------------------------------------------------------------------------
// à¯êî	id		ÅG	É_ÉCÉAÉOÉâÉÄID
//		index	ÅF	É_ÉCÉAÉOÉâÉÄì‡ÉCÉìÉfÉbÉNÉX
//		sign	ÅF	ïœçXÉTÉCÉì
//-----------------------------------------------------------------------------
void CChangeTaxEx::ChangeColor(unsigned short id, short index, int sign)
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr(id, index, &DA, CIcsdiagctrl);
	switch (sign) {
	case 1:
		DA.attr_bcolor = BC_WHITE;
		break;		// ì¸óÕïsâ¬	ÅiîíêFÅj
	case 2:
		DA.attr_bcolor = BC_GRAY;
		break;		// ÉçÉbÉN	ÅiäDêFÅj
	case 3:
		DA.attr_bcolor = BC_BLUE;
		break;		// ì¡éÍêFá@	Åiê¬êFÅj
	case 4:
		DA.attr_bcolor = BC_GREEN;
		break;		// ì¡éÍêFáA	ÅióŒêFÅj	
	case 5:
		DA.attr_bcolor = BC_GREEN_L;
		break;		// ì¡éÍêFáB	ÅiîñóŒêFÅj	 
	default:
		DA.attr_bcolor = BC_CREAM;
		break;		// é¿äzì¸óÕ	ÅiÉNÉäÅ[ÉÄêFÅj 
	}
	diag_setattr(id, index, &DA, FALSE, CIcsdiagctrl);
}


BOOL CChangeTaxEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Ç±Ç±Ç…ì¡íËÇ»ÉRÅ[ÉhÇí«â¡Ç∑ÇÈÇ©ÅAÇ‡ÇµÇ≠ÇÕäÓñ{ÉNÉâÉXÇåƒÇ—èoÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB

	CWnd *pWnd = GetFocus();

	if (pMsg->message == WM_KEYDOWN) {

		if (GetCombineKeyDown(VK_SHIFT) && (VK_F1 <= pMsg->wParam) && (pMsg->wParam <= VK_F12)) {
			return TRUE;
		}

		if (pMsg->wParam == VK_TAB || pMsg->wParam == VK_RIGHT) {
			if (pWnd == GetDlgItem(IDOK)) {
				c_set(IDCANCEL);
				return TRUE;
			}
			else if (pWnd == GetDlgItem(IDCANCEL)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
				return TRUE;
			}
		}

		else if (pMsg->wParam == VK_UP) {
			if (pWnd == GetDlgItem(IDOK) || pWnd == GetDlgItem(IDCANCEL)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZC, CIcsdiagctrl);
				return TRUE;
			}
		}

		else if (pMsg->wParam == VK_DOWN) {
			if (pWnd == GetDlgItem(IDOK) || pWnd == GetDlgItem(IDCANCEL)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
				return TRUE;
			}
		}

		else if (pMsg->wParam == VK_LEFT) {
			if (pWnd == GetDlgItem(IDOK)) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZC, CIcsdiagctrl);
				return TRUE;
			}
		}

	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
BEGIN_EVENTSINK_MAP(CChangeTaxEx, ICSDialog)
	ON_EVENT(CChangeTaxEx, IDC_ICSDIAGCTRL1, 1, CChangeTaxEx::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTaxEx, IDC_ICSDIAGCTRL1, 2, CChangeTaxEx::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CChangeTaxEx, IDC_ICSDIAGCTRL1, 3, CChangeTaxEx::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CChangeTaxEx, IDC_ICSDIAGCTRL1, 8, CChangeTaxEx::ComboSelIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CChangeTaxEx::EditONIcsdiagctrl1(short index)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch (index) {

	case IDNUKIZ:	// çTèúëŒè€édì¸äzÅ@ê≈î≤Ç´
		if (m_ChSw == 1) {
			m_ChSw = 0;
			diag_deledit(IDC_ICSDIAGCTRL1, CIcsdiagctrl);
			DiagData.data_check = 1;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			DiagData.data_check = 0;
			diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			ZeiChangeCalq();//åvéZ
			diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);
		}
		break;

	case IDKOMIZ:	// çTèúëŒè€édì¸äzÅ@ê≈çûÇ›
		if (m_ChSw == 0) {
			m_ChSw = 1;
			diag_deledit(IDC_ICSDIAGCTRL1, CIcsdiagctrl);
			DiagData.data_check = 0;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			DiagData.data_check = 1;
			diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			ZeiChangeCalq();//åvéZ
			diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);
		}
		break;

	default:

		ZeiChangeCalq();//åvéZ
		break;
	}
}

//-----------------------------------------------------------------------------
// ê≈ïœçXéûÇÃåvéZèàóùä÷êîâª
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CChangeTaxEx::ZeiChangeCalq()
{

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if (tmpChSw != m_ChSw) {
		if (m_ChSw == 1) {
			m_Util.percent(MNZ40K, MNK40K, 4, 40, 0);
			m_Util.percent(MNZ40H, MNK40H, 4, 40, 0);
			m_Util.percent(MNZ40C, MNK40C, 4, 40, 0);
			m_Util.percent(MENZZK, MENZKK, 0, 30, 0);
			m_Util.percent(MENZZH, MENZKH, 0, 30, 0);
			m_Util.percent(MENZZC, MENZKC, 0, 30, 0);
			m_Util.percent(MNZ63K, MNK63K, 9, 63, 0);
			m_Util.percent(MNZ63H, MNK63H, 9, 63, 0);
			m_Util.percent(MNZ63C, MNK63C, 9, 63, 0);

			m_Util.percent(MNZ78K, MNK78K, 12, 78, 0);
			m_Util.percent(MNZ78H, MNK78H, 12, 78, 0);
			m_Util.percent(MNZ78C, MNK78C, 12, 78, 0);
			m_Util.percent(MNZ624K, MNK624K, 11, 62, 0);
			m_Util.percent(MNZ624H, MNK624H, 11, 62, 0);
			m_Util.percent(MNZ624C, MNK624C, 11, 62, 0);

			m_ChSw = 1;
		}
		else {
			m_Util.percent(MNZ40K, MNK40K, 1, 40, 0);
			m_Util.percent(MNZ40H, MNK40H, 1, 40, 0);
			m_Util.percent(MNZ40C, MNK40C, 1, 40, 0);
			m_Util.percent(MENZZK, MENZKK, 1, 30, 0);
			m_Util.percent(MENZZH, MENZKH, 1, 30, 0);
			m_Util.percent(MENZZC, MENZKC, 1, 30, 0);
			m_Util.percent(MNZ63K, MNK63K, 1, 63, 0);
			m_Util.percent(MNZ63H, MNK63H, 1, 63, 0);
			m_Util.percent(MNZ63C, MNK63C, 1, 63, 0);

			m_Util.percent(MNZ78K, MNK78K, 1, 78, 0);
			m_Util.percent(MNZ78H, MNK78H, 1, 78, 0);
			m_Util.percent(MNZ78C, MNK78C, 1, 78, 0);
			m_Util.percent(MNZ624K, MNK624K, 1, 62, 0);
			m_Util.percent(MNZ624H, MNK624H, 1, 62, 0);
			m_Util.percent(MNZ624C, MNK624C, 1, 62, 0);

			m_ChSw = 0;
		}
		memmove(DiagData.data_val, MNZ40K, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ40K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ40H, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ40H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ40C, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ40C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MENZZK, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMENZZK, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MENZZH, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMENZZH, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MENZZC, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMENZZC, (struct IUnknown *)&DiagData, CIcsdiagctrl);

		memmove(DiagData.data_val, MNZ63K, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ63K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ63H, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ63H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(DiagData.data_val, MNZ63C, MONY_BUF_SIZE);
		diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ63C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	}
	if (m_ChSw == 1) {
		tmpChSw = 1;
	}
	else {
		tmpChSw = 0;
	}
}


void CChangeTaxEx::EditOFFIcsdiagctrl1(short index)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	char				CkMy[MONY_BUF_SIZE];
	char				buf[512] = { 0 };
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch (index) {
	case	IDCOMBO:	// â€ê≈ï˚éÆëIë

		break;

	case	IDMNK40K:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK40K, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK40K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK40K, CkMy)) {
			memmove(MNK40K, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MNZ40K, MNK40K, 4, 40, 0);
			}
			else {
				m_Util.percent(MNZ40K, MNK40K, 1, 40, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
			memmove(DiagData.data_val, MNZ40K, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ40K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ40K:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ40K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ40K, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK40H:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK40H, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK40H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK40H, CkMy)) {
			memmove(MNK40H, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MNZ40H, MNK40H, 4, 40, 0);
			}
			else {
				m_Util.percent(MNZ40H, MNK40H, 1, 40, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)  îÒâ€ê≈Ç…åWÇÈ
			memmove(DiagData.data_val, MNZ40H, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ40H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ40H:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ40H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ40H, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK40C:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK40C, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK40C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK40C, CkMy)) {
			memmove(MNK40C, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MNZ40C, MNK40C, 4, 40, 0);
			}
			else {
				m_Util.percent(MNZ40C, MNK40C, 1, 40, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
			memmove(DiagData.data_val, MNZ40C, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ40C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ40C:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ40C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ40C, DiagData.data_val, MONY_BUF_SIZE);
		break;


	case	IDMENZKK:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MENZKK, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMENZKK, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MENZKK, CkMy)) {
			memmove(MENZKK, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MENZZK, MENZKK, 0, 30, 0);
			}
			else {
				m_Util.percent(MENZZK, MENZKK, 1, 30, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
			memmove(DiagData.data_val, MENZZK, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMENZZK, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMENZZK:	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMENZZK, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MENZZK, DiagData.data_val, MONY_BUF_SIZE);
		break;


	case	IDMENZKH:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MENZKH, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMENZKH, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MENZKH, CkMy)) {
			memmove(MENZKH, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MENZZH, MENZKH, 0, 30, 0);
			}
			else {
				m_Util.percent(MENZZH, MENZKH, 1, 30, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
			memmove(DiagData.data_val, MENZZH, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMENZZH, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMENZZH:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMENZZH, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MENZZH, DiagData.data_val, MONY_BUF_SIZE);
		break;


	case	IDMENZKC:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MENZKC, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMENZKC, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MENZKC, CkMy)) {
			memmove(MENZKC, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MENZZC, MENZKC, 0, 30, 0);
			}
			else {
				m_Util.percent(MENZZC, MENZKC, 1, 30, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
			memmove(DiagData.data_val, MENZZC, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMENZZC, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMENZZC:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMENZZC, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MENZZC, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK63K:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK63K, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK63K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK63K, CkMy)) {
			memmove(MNK63K, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MNZ63K, MNK63K, 9, 63, 0);
			}
			else {
				m_Util.percent(MNZ63K, MNK63K, 1, 63, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
			memmove(DiagData.data_val, MNZ63K, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ63K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ63K:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ63K, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ63K, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK63H:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK63H, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK63H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK63H, CkMy)) {
			memmove(MNK63H, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MNZ63H, MNK63H, 9, 63, 0);
			}
			else {
				m_Util.percent(MNZ63H, MNK63H, 1, 63, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)  îÒâ€ê≈Ç…åWÇÈ
			memmove(DiagData.data_val, MNZ63H, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ63H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ63H:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ63H, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ63H, DiagData.data_val, MONY_BUF_SIZE);
		break;

	case	IDMNK63C:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, MNK63C, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNK63C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(MNK63C, CkMy)) {
			memmove(MNK63C, CkMy, MONY_BUF_SIZE);
			if (m_ChSw == 1) {
				m_Util.percent(MNZ63C, MNK63C, 9, 63, 0);
			}
			else {
				m_Util.percent(MNZ63C, MNK63C, 1, 63, 0);
			}
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈè¡îÔê≈äz Å@(êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
			memmove(DiagData.data_val, MNZ63C, MONY_BUF_SIZE);
			diag_setdata(IDC_ICSDIAGCTRL1, IDMNZ63C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
		break;

	case	IDMNZ63C:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		diag_getdata(IDC_ICSDIAGCTRL1, IDMNZ63C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(MNZ63C, DiagData.data_val, MONY_BUF_SIZE);
		break;
	default:
		break;
	}
}


void CChangeTaxEx::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char				VK_FLG, VK_DMM;
	short				wTerm;

	if (nChar == 0) {
		return;
	}

	// Shift+TABÇTABÇ…ÅATABÇENTERÇ…ï‘ä“
	VK_FLG = VK_DMM = 0x00;
	VK_FLG = (char)::GetKeyState(VK_SHIFT);
	if (nChar == VK_TAB) {
		if (VK_FLG & 0x80) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	switch (index) {

	case	IDCOMBO:	// â€ê≈ï˚éÆëIë

		if (wTerm == VK_UP) {
			return;
		}
		if (wTerm == VK_ESCAPE) {
			OnCancel();
			return;
		}
		if (wTerm == VK_DOWN) {
			m_ICSDIAG1.ComboDropDown(IDCOMBO, TRUE);
			return;
		}
		diag_getdata(IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		if (DiagData.data_combo == 2) {
			c_set(IDOK);
			return;
		}
		// éüçÄñ⁄
		if ((wTerm == VK_RETURN) || (wTerm == VK_F3)) {
			if (m_ChSw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// ê≈çûÇ›
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// ê≈î≤Ç´
			}
		}
		break;

	case IDKOMIZ:	// çTèúëŒè€äOédì¸äzÅ@ê≈çûÇ›
	case IDNUKIZ:	// çTèúëŒè€äOédì¸äzÅ@ê≈î≤Ç´

		if ((wTerm == VK_LEFT) || (wTerm == VK_RIGHT)) {
			if (m_ChSw == 1) {
				m_ChSw = 0;
				//		m_pSnHeadData->SVsign &= 0xef;	// ê≈î≤Ç´
				DiagData.data_check = 1;
				diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				DiagData.data_check = 0;
				diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				ZeiChangeCalq();//åvéZ
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);
			}
			else {
				m_ChSw = 1;//
//					m_pSnHeadData->SVsign |= 0x10;	// ê≈çûÇ›
				DiagData.data_check = 0;
				diag_setdata(IDC_ICSDIAGCTRL1, IDNUKIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				DiagData.data_check = 1;
				diag_setdata(IDC_ICSDIAGCTRL1, IDKOMIZ, (struct IUnknown *)&DiagData, CIcsdiagctrl);
				ZeiChangeCalq();//åvéZ
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);
			}
		}
		// éüçÄñ⁄
		if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_DOWN)) {//
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63K, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if ((wTerm == VK_TAB) || (wTerm == VK_F2) || (wTerm == VK_UP)) {//
			diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
		}
		break;

	case	IDMNK40K:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40K, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63C, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63C, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40H, CIcsdiagctrl);
		}
		break;

	case	IDMNZ40K:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄

		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40H, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40K, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63C, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40H, CIcsdiagctrl);
		}
		break;

	case	IDMNK40H:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
			// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40H, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40K, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40K, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40C, CIcsdiagctrl);
		}
		break;

	case	IDMNZ40H:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40C, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40H, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40K, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40C, CIcsdiagctrl);
		}
		break;

	case	IDMNK40C:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40C, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40H, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40H, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			if (m_pSnHeadData->SVmzsw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKK, CIcsdiagctrl);
			}
			else {
				c_set(IDOK);
			}
		}
		break;

	case	IDMNZ40C:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			if (m_pSnHeadData->SVmzsw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKK, CIcsdiagctrl);
			}
			else {
				c_set(IDOK);
			}
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40C, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40H, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			if (m_pSnHeadData->SVmzsw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZK, CIcsdiagctrl);
			}
			else {
				c_set(IDOK);
			}
		}
		break;

	case	IDMENZKK:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZK, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40C, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40C, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKH, CIcsdiagctrl);
		}
		break;

	case	IDMENZZK:	// 				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKH, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKK, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40C, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZH, CIcsdiagctrl);
		}
		break;

	case	IDMENZKH:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZH, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZK, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKK, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKC, CIcsdiagctrl);
		}
		break;

	case	IDMENZZH:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKC, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKH, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZK, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZC, CIcsdiagctrl);
		}
		break;

	case	IDMENZKC:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (ãåê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZC, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZH, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKH, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			c_set(IDOK);
		}
		break;

	case	IDMENZZC:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			c_set(IDOK);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZKC, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMENZZH, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			c_set(IDOK);
		}
		break;
	case	IDMNK63K:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@â€ê≈Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63K, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			if (m_ChSw == 1) {//
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// ê≈çûÇ›
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// ê≈î≤Ç´
			}
		}
		// è„íi
		if (wTerm == VK_UP) {
			if (m_ChSw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// ê≈çûÇ›
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// ê≈î≤Ç´
			}
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63H, CIcsdiagctrl);
		}
		break;

	case	IDMNZ63K:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63H, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63K, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			if (m_ChSw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDKOMIZ, CIcsdiagctrl);	// ê≈çûÇ›
			}
			else {
				diag_setposition(IDC_ICSDIAGCTRL1, IDNUKIZ, CIcsdiagctrl);	// ê≈î≤Ç´
			}
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63H, CIcsdiagctrl);
		}
		break;

	case	IDMNK63H:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@îÒâ€ê≈Ç…åWÇÈ
			// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63H, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63K, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63K, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63C, CIcsdiagctrl);
		}
		break;

	case	IDMNZ63H:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63C, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63H, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63K, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63C, CIcsdiagctrl);
		}
		break;

	case	IDMNK63C:	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz (êVê≈ó¶ï™)Å@ã§í Ç…åWÇÈ
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63C, CIcsdiagctrl);
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63H, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63H, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			if (m_pSnHeadData->SVmzsw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40K, CIcsdiagctrl);
			}
			else {
				c_set(IDOK);
			}
		}
		break;

	case	IDMNZ63C:	//				 ÅV	   è¡îÔê≈äz   (ÅV      )Å@ÅV
		// éüçÄñ⁄
		if (wTerm == VK_RETURN || wTerm == VK_F3 || wTerm == VK_RIGHT) {
			if (m_pSnHeadData->SVmzsw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMNK40K, CIcsdiagctrl);
			}
			else {
				c_set(IDOK);
			}
		}
		// ëOçÄñ⁄Ç÷
		if (wTerm == VK_TAB || wTerm == VK_LEFT || wTerm == VK_F2) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNK63C, CIcsdiagctrl);
		}
		// è„íi
		if (wTerm == VK_UP) {
			diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ63H, CIcsdiagctrl);
		}
		// â∫íi
		if (wTerm == VK_DOWN) {
			if (m_pSnHeadData->SVmzsw == 1) {
				diag_setposition(IDC_ICSDIAGCTRL1, IDMNZ40K, CIcsdiagctrl);
			}
			else {
				c_set(IDOK);
			}
		}
		break;

	default:
		break;
	}
}

void CChangeTaxEx::ComboSelIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: Ç±Ç±Ç…ÉÅÉbÉZÅ[ÉW ÉnÉìÉhÉâ ÉRÅ[ÉhÇí«â¡ÇµÇ‹Ç∑ÅB

	char			buf[512] = { 0 };
	DIAGRAM_DATA	diag_data;

	if (index == IDCOMBO) {
		diag_getdata(IDC_ICSDIAGCTRL1, IDCOMBO, (struct IUnknown *)&diag_data, CIcsdiagctrl);
		m_CbSt = diag_data.data_combo;
		switch (m_CbSt) {
		case 0:	// ñ∆ê≈ÇÊÇËâ€ê≈
			m_CbSw = 0;
			tansw = 0x10;
			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("ìñä˙ÇÊÇËÅAñ∆ê≈ã∆é“Ç©ÇÁâ€ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃâ€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("çTèúëŒè€édì¸äz"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ4, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJZ, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ6, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			break;

		case 1:	// â€ê≈ÇÊÇËñ∆ê≈
			m_CbSw = 1;
			tansw = 0x11;
			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("óÇä˙ÇÊÇËÅAâ€ê≈ã∆é“Ç©ÇÁñ∆ê≈ã∆é“Ç∆Ç»ÇÈèÍçáÇÃâ€ê≈édì¸ìôÇ…åWÇÈí≤êÆê≈äz"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl);

			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T("çTèúëŒè€äOédì¸äz"));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ4, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJZ, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			diag_setdata(IDC_ICSDIAGCTRL1, IDNKOJ6, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			break;

		default:
			m_CbSw = 2;
			tansw = 0x00;
			memset(buf, '\0', sizeof(buf));
			strcpy_s(buf, sizeof(buf), _T(" "));
			diag_data.data_disp = buf;
			diag_setdata(IDC_ICSDIAGCTRL1, IDTITLE, (struct IUnknown *)&diag_data, CIcsdiagctrl);
			break;
		}

		ComboCheck_AttrSet();

	}

}

//-----------------------------------------------------------------------------
//	åªç›ÇÃÉRÉìÉ{É{ÉbÉNÉXÇÃèÓïÒÇÉ`ÉFÉbÉNÇµì¸óÕëÆê´ÅAîwåiêFÇÉZÉbÉg
//-----------------------------------------------------------------------------
void CChangeTaxEx::ComboCheck_AttrSet()
{

	int		flg = 0;
	short	index = 0;
	char	buf[512] = { 0 };

	switch (m_pSnHeadData->Sn_Sign4 & 0x81) {
	case 0x01:	// ÉçÉbÉN
	case 0x81: // îÒòAìÆÅïÉçÉbÉN
		if (m_pSnTanaData->Sn_tansw & 0x01) {
			flg = 1;
		}
		else {
			flg = 2;
		}
		diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, flg);
		diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, flg);
		diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, flg);

		for (index = IDMNK63K; index <= IDMNZ63C; index++) {
			diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, index, flg);
		}

		if (m_pSnHeadData->SVmzsw == 1) {
			for (index = IDMNK40K; index <= IDMNZ40C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, flg);
			}
			for (index = IDMENZKK; index <= IDMENZZC; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, flg);
			}
		}
		else {
			for (index = IDMNK40K; index <= IDMNZ40C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}

			for (index = IDMENZKK; index <= IDMENZZC; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}
		}
		c_cls(IDC_ICSDISPCTRL1, CIcsdisp8ctrl1);
		::ZeroMemory(buf, sizeof(buf));
		m_txt1.SetWindowTextA(buf);
		c_set(IDOK);
		break;
	case 0x80: // îÒòAìÆ
		diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
		ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 0);

		if (tansw != 0x10 && tansw != 0x11) {
			diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 1);
			diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 1);
			for (index = IDMNK63K; index <= IDMNZ63C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}
			if (m_pSnHeadData->SVmzsw == 1) {
				for (index = IDMNK40K; index <= IDMNZ40C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
				for (index = IDMENZKK; index <= IDMENZZC; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
			}
			else {
				for (index = IDMNK40K; index <= IDMNZ40C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
				for (index = IDMENZKK; index <= IDMENZZC; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
			}
		}
		else {
			diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 0);
			diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 0);
			for (index = IDMNK63K; index <= IDMNZ63C; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
			}
			if (m_pSnHeadData->SVmzsw == 1) {
				for (index = IDMNK40K; index <= IDMNZ40C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
				}
				for (index = IDMENZKK; index <= IDMENZZC; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
				}
			}
			else {
				for (index = IDMNK40K; index <= IDMNZ40C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
				for (index = IDMENZKK; index <= IDMENZZC; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
			}
		}

		c_cls(IDC_ICSDISPCTRL1, CIcsdisp8ctrl1);
		::ZeroMemory(buf, sizeof(buf));
		m_txt1.SetWindowTextA(buf);
		diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);
		break;
	default: // òAìÆ
		if (m_pSnTanaData->Sn_tansw & 0x01) {
			// édñÛÇ…ÇÊÇÈíIâµí≤êÆóLÇËéûÇÕï\é¶ÇÃÇ›
			diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 1);
			diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 1);
			diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
			ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 1);

			for (index = IDMNK63K; index <= IDMENZZC; index++) {
				diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
			}

			strcpy_s(buf, sizeof(buf), _T(" íIâµí≤êÆédñÛÇ™ì¸óÕÇ≥ÇÍÇƒÇ¢Ç‹Ç∑ÅB\n ê≈äzÇèCê≥Ç∑ÇÈèÍçáÇÕÅAäYìñédñÛÇïœçXÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB"));
			puts(IDC_ICSDISPCTRL1, buf, CIcsdisp8ctrl1);
			m_txt1.SetWindowTextA(buf);
			c_set(IDOK);
		}
		else {
			if (tansw != 0x10 && tansw != 0x11) {
				diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 0);
				diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 1);
				diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_READONLY, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 1);
				for (index = IDMNK63K; index <= IDMNZ63C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
				}
				if (m_pSnHeadData->SVmzsw == 1) {
					for (index = IDMNK40K; index <= IDMNZ40C; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
					}
					for (index = IDMENZKK; index <= IDMENZZC; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
					}
				}
				else {
					for (index = IDMNK40K; index <= IDMNZ40C; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
					}
					for (index = IDMENZKK; index <= IDMENZZC; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
					}
				}

			}
			else {
				// édñÛÇ…ÇÊÇÈíIâµí≤êÆñ≥ÇµéûÇÕì¸óÕâ¬
				diag_modify(IDC_ICSDIAGCTRL1, IDCOMBO, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDCOMBO, 0);
				diag_modify(IDC_ICSDIAGCTRL1, IDNUKIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDNUKIZ, 0);
				diag_modify(IDC_ICSDIAGCTRL1, IDKOMIZ, DIAG_MDFY_EDIT, CIcsdiagctrl);
				ChangeColor(IDC_ICSDIAGCTRL1, IDKOMIZ, 0);
				for (index = IDMNK63K; index <= IDMNZ63C; index++) {
					diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
					ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
				}
				if (m_pSnHeadData->SVmzsw == 1) {
					for (index = IDMNK40K; index <= IDMNZ40C; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
					}
					for (index = IDMENZKK; index <= IDMENZZC; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_EDIT, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 0);
					}
				}
				else {
					for (index = IDMNK40K; index <= IDMNZ40C; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
					}
					for (index = IDMENZKK; index <= IDMENZZC; index++) {
						diag_modify(IDC_ICSDIAGCTRL1, index, DIAG_MDFY_READONLY, CIcsdiagctrl);
						ChangeColor(IDC_ICSDIAGCTRL1, index, 1);
					}
				}

			}
			c_cls(IDC_ICSDISPCTRL1, CIcsdisp8ctrl1);
			::ZeroMemory(buf, sizeof(buf));
			m_txt1.SetWindowTextA(buf);
			diag_setposition(IDC_ICSDIAGCTRL1, IDCOMBO, CIcsdiagctrl);

		}
		break;
	}

	diag_redraw(IDC_ICSDIAGCTRL1, CIcsdiagctrl);//çƒï`âÊ

	return;
}

