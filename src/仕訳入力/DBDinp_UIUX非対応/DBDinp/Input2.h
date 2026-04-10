/*=============================

	data input header
		file	input2.h

==============================*/


int		M_KOJI = 0;
BOOL	M_MISEIKOJI_TYPE = FALSE;

char	FMT15[] = _T("sss,sss,sss,ss9");
char	FMT18[] = _T("ss,sss,sss,sss,ss9");

char	d_bmn[] = "部　門";
char	d_kjno[] = "工事番号";
char	Image[] = "《イメージ摘要》";
char	MFULL[]   = "特殊業務のマスタ設定変更で最大仕訳登録件数を確認してください。";
char	MFULL2[]   = "特殊業務のマスタ設定変更で最大仕訳登録件数を増やせます。";
char	MFULL3[]   = "最大仕訳件数になりました。";


CDBINPDataRec	ZREC[RECMAX], CREC, SREC;
//CDBINPDataRec	ZEIREC[4], *AREC_p;
CDBINPDataRec	RWREC[4];

class CZVOLUME* Voln1 = 0;
class CMINFO*	Minfo = 0;

CDBipEZREC	BREC1, BREC2;

unsigned char	SYOG_d[6], SYOG_c[6], SG_def[6];
unsigned char	GENK_d[6], GENK_c[6], GK_def[6];


char	BRmst;
char	save_50kana[16];

int		SV_INP_mode;

int		REAL_SEQ_NO;

int		SEQ_NO,D_SEQN, DENP_NO, START_SEQ, USERS_SEQ;
int		SV_BOTOM_SEQ_NO;
short	DOPEN_MODE;		// データオープンモード


char saveITEM50kana[16];
struct _TKYBRN_SEL TKYBRN_SEL;	// 枝番摘要呼び出し

int		BMON_LNG = 0;
int		KMKCD_LN = 0;
int		BMON_MST = FALSE;

int		SCROLL_NL_ = 0;

/*	save variable storage for scan function	*/
CDBINPDataRec	SCAN_rec;	/* correct data save */

#ifdef IMPORT	// 10.14 /98
CDBINPDataRec	NXT_rec, NXT_rec2;
int NXTDTA_sg = FALSE;
#endif

/* 消費税関係ＳＷ． */
char	TKattr, TKattr2;

//
// 財務クラス
CDipZmSub*	pDBzm = NULL;
// 消費税関係
CDBSyohi*	pDBsy = NULL;
// ドキュメントクラス
CDocSub*	pDCsb = NULL;

// 税額計算
CDataZeiCalq*	pZeiCl = NULL;

// 摘要対応
CZIM_TkjourUtil*	pTkjour = NULL;

// 請求書管理オプション
CAddInvData*	pAddInv = NULL;


// インボイス対応
bool	bInvMaster = false;	// インボイスバージョンアップマスター
bool	bInvUse = false;	// インボイス期間マスター
bool	bInvDisp = false;	// 登録番号表示フラグ


// 内部取引用
#ifdef NAIBU_VER2
CDBNpCal*	pDBnc = NULL;
#endif

void ExitApp()
{
}
// イメージマスターオープンサイン
int IMG_master = ERR;

// スキャンオンリーサイン
BOOL bSCAN_ONLY_MODE = FALSE;
// マスター全体が確定か？
// D8ON：上手君参照モード
BOOL bCONFIRM_MASTER = FALSE;


// 仕訳コピー用選択
CDWordArray	gSelArray;			// 選択データのSEQ配列
int			gSelDirection = 0;	// 0 = 選択なし状態, -1 = 上, 1 = 下
int			gSelType = 0;		// 0 = 選択なし,	1 = shiftキー
								//					2 = ctrlキー
int			gSelSeq = 0;		// 現在の選択SEQ
BOOL		gSelUpdate = FALSE;	// 選択データ画面再表示

// 証憑番号を使用するマスターか？
BOOL bDOCEVI_Master;

// 摘要消費税 保存レコード
CDBipTKREC	gTKattrec;

// スキャナ保存のマスターか？
BOOL bSCANSAVE_Master = FALSE;

const char* WizTitle = "請求書管理オプション";
