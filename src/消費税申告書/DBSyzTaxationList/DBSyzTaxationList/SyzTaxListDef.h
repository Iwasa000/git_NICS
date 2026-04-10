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
	ID_ICSSH_GENERAL_LIST_TAB = 1,			// 一般
	ID_ICSSH_REALESTATE_LIST_TAB,			// 不動産
	ID_ICSSH_AGRICULTUTRE_LIST_TAB,			// 農業
	ID_ICSSH_SALES_LIST_TAB,				// 課税売上高
	ID_ICSSH_STOCK_LIST_TAB,				// 課税仕入高
	ID_ICSSH_BUSINESS_SALES_LIST_TAB,		// 事業別売上高
	ID_ICSSH_KEI_GENERAL_LIST_TAB,			// 一般（経過措置用）
	ID_ICSSH_KEI_REALESTATE_LIST_TAB,		// 不動産（経過措置用）
	ID_ICSSH_KEI_AGRICULTUTRE_LIST_TAB,		// 農業（経過措置用）
	ID_ICSSH_KEI_SALES_LIST_TAB,			// 課税売上高（経過措置用）
	ID_ICSSH_KEI_STOCK_LIST_TAB,			// 課税仕入高（経過措置用）
	ID_ICSSH_KEI_BUSINESS_SALES_LIST_TAB,	// 事業別売上高（経過措置用）
//2016.10.20 INSERT START
	ID_ICSSH_6SYU_BUSINESS_SALES_LIST_TAB,		// 事業別売上高　６種
	ID_ICSSH_KEI_6SYU_BUSINESS_SALES_LIST_TAB	// 事業別売上高　６種（経過措置用）
//2016.10.20 INSERT END
}EnumIdIcsKzTabtype;

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
	ID_ICSSH_SPC_8PER				// ８％
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
