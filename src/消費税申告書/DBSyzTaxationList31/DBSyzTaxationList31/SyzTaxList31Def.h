#pragma	once

//== 定数 ==

//const	int	ICSSH_TAXLIST_TAB_MAX	=	7;	// 課税取引高計算書 最大タブ数

const	unsigned int	REPLY_ERR	=	0x00000001;
const	unsigned int	REPLY_OK	=	0x00000002;
const	unsigned int	REPLY_NEW	=	0x00000004;
const	unsigned int	REPLY_NEXT	=	0x00010000;
const	unsigned int	REPLY_BACK	=	0x00020000;
const	unsigned int	REPLY_STAY	=	0x00040000;
const	unsigned int	REPLY_MOVE	=	0x00080000;
const	unsigned int	REPLY_DELT	=	0x80000000;

typedef enum {
	ID_ICSSH_31_GENERAL_LIST_TAB = 1,		// 一般
	ID_ICSSH_31_REALESTATE_LIST_TAB,		// 不動産
	ID_ICSSH_31_AGRICULTUTRE_LIST_TAB,		// 農業
	ID_ICSSH_31_SALES_LIST_TAB,				// 課税売上高
	ID_ICSSH_31_STOCK_LIST_TAB,				// 課税仕入高
	ID_ICSSH_31_BUSINESS_SALES_LIST_TAB		// 事業別売上高

}EnumIdIcs31KzTabtype;

typedef enum {
	ID_ICSSH_SPC31_1_TAB = 1,			// 特定収入計算表１
	ID_ICSSH_SPC31_2_TAB,				// 特定収入計算表２（１）
	ID_ICSSH_SPC31_22_TAB,				// 特定収入計算表２（２）表面
	ID_ICSSH_SPC31_22_2_TAB,			// 特定収入計算表２（２）裏面
	ID_ICSSH_SPC31_34_TAB,				// 特定収入計算表３
	ID_ICSSH_SPC31_51_TAB,				// 特定収入計算表５（１）
	ID_ICSSH_SPC31_52_TAB,				// 特定収入計算表５（２）
	ID_ICSSH_SPC31_53_TAB,				// 特定収入計算表５（３）
	ID_ICSSH_SPC31_52_1_31TAB,			// 特定収入計算表５－２（１）
	ID_ICSSH_SPC31_52_2_31TAB,			// 特定収入計算表５－２（２）
	ID_ICSSH_SPC31_52_31_31TAB,			// 特定収入計算表５－２（３）－１
	ID_ICSSH_SPC31_52_32_31TAB,			// 特定収入計算表５－２（３）－２
	ID_ICSSH_SPC31_52_33_31TAB,			// 特定収入計算表５－２（３）－３
	ID_ICSSH_SPC31_52_41_31TAB,			// 特定収入計算表５－２（４）－１
	ID_ICSSH_SPC31_52_42_31TAB,			// 特定収入計算表５－２（４）－２
	ID_ICSSH_SPC31_52_43_31TAB,			// 特定収入計算表５－２（４）－３
}EnumIdIcs31SpcTabtype;

//--> '15.02.06 INS START
// 特定収入タブ
typedef enum {
	ID_ICSSH_SPC_1_TAB = 1,			// 特定収入計算表１
	ID_ICSSH_SPC_2_1_TAB,			// 特定収入計算表２－１
	ID_ICSSH_SPC_2_2_TAB,			// 特定収入計算表２－２
	ID_ICSSH_SPC_3or4_TAB,			// 特定収入計算表３、４
	ID_ICSSH_SPC_5_1_TAB,			// 特定収入計算表５－１
	ID_ICSSH_SPC_5_2_TAB,			// 特定収入計算表５－２
	ID_ICSSH_SPC_5_3_TAB,			// 特定収入計算表５－３
	ID_ICSSH_SPC_6_TAB				// 特定収入計算表６
} EnumIdIcsSpcTabtype;

// 税率種別（特定収入）
typedef enum {
	ID_ICSSH_SPC_3PER	=	1,		// ３％
	ID_ICSSH_SPC_5PER,				// ５％
	ID_ICSSH_SPC_8PER,				// ８％
	ID_ICSSH_SPC_8PERK,				// ８％ ( 軽減税率 )
	ID_ICSSH_SPC_10PER,				// １０％
} EnumIdIcsSpcZeitype;
//<-- '15.02.06 INS END

// 個人：業種区分
enum {
	ID_ICSSH_KJ_IPPAN		=	1,	// 一般
	ID_ICSSH_KJ_FUDOUSAN	=	2,	// 不動産
	ID_ICSSH_KJ_NOUGYOU		=	4	// 農業
};

//'14.08.01------->
//== 構造体 ==
// タブ管理用
typedef struct	_tag_ICS_SH_TABMNG_ {
//-- '15.02.09 --
//	EnumIdIcsKzTabtype	tabtype;	// タブ種別
//---------------
	int					tabtype;	// タブ種別
//---------------
	ICSDialog			*pWnd;		// ウィンドウ
	char				dmy[8];
} SH_TABMNG;
//<----------------


#define		FMTX13		_T("s,sss,sss,sss,sss")
#define		FMTX130		_T("s,sss,sss,sss,ss9")
#define		FMTX12		_T("SSS,SSS,SSS,SSS")
#define		FMTX4_1		_T("SSS9.9")
#define		FMTX4_2		_T("SS9.99")		// 24/02/29wd_特定収入対応 add

//== バックカラーコントロール ==
#define	SIROIROCOLOR	PALETTERGB( 255, 251, 240 )
static	CBrush	Ibrsh_ShinSpecific( SIROIROCOLOR );

//== クラス ==
class	CShinCom
{
public:
	CShinCom();
	~CShinCom();

	CBrush& GetBkClrBrsh(){ return m_BkClrBrsh; };	

private:
	CBrush	m_BkClrBrsh;	// バックカラー
};


#define R03YEAR 20211231	// 令和3年


// kasai add -->
// 課税取引（旧税率なし）開始日
// Mainでも同様の定義してるため、変更する場合はそっちも変更
#define TAX_R04_SDAY 20211231
// kasai add <--
// 2023/07/11 h.wada add --> 
// DBSyzShinMainでも同様の定義をしている
#define TAX_R05_EDAY 20231231
// h.wada <--

// 令和5年1月1日以降の課税期間の場合には、課税取引金額計算表の経理方式の切り替えを不可
#define R05_SDAY 20230101

#define TEST_KIKAN 20231231  // 20231124 切替期間まだ決めれてないため一旦仮の期間
#define KIKAN_START_INV80 20231001  // 20231124 切替期間まだ決めれてないため一旦仮の期間
#define KIKAN_END_INV80 20260930  // 20231124 切替期間まだ決めれてないため一旦仮の期間
#define KIKAN_START_INV50 20261001  // 20231124 切替期間まだ決めれてないため一旦仮の期間
#define KIKAN_END_INV50 20290930  // 20231124 切替期間まだ決めれてないため一旦仮の期間

// 230718Aw -->
static char		*ICS_TAXLIST_01_HYOID	=	_T("SHE020");	// 課税取引金額計算表(事業所得用)
static char		*ICS_TAXLIST_02_HYOID	=	_T("SHE100");	// 課税取引金額計算表(農業所得用)
static char		*ICS_TAXLIST_03_HYOID	=	_T("SHE040");	// 課税取引金額計算表(不動産所得用)
static char		*ICS_TAXLIST_04_HYOID	=	_T("SHE060");	// 課税売上高計算表
static char		*ICS_TAXLIST_05_HYOID	=	_T("SHE080");	// 課税仕入高計算表
// 230718Aw <--
// 230728Aw add -->
static char		*ICS_TAXLIST_06_HYOID	=	_T("SHE990");	// 事業別売上高計算表(非連動)
// 230728Aw add <--
