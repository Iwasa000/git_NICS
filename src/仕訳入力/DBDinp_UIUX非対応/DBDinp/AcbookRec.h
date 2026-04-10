#ifndef INCLUDE_ACBOOKREC_H
#define INCLUDE_ACBOOKREC_H

// 出納帳  1 ライン レコード

#define	AC_SEQ_PN		0
#define	AC_DNPTYPE_PN	1
#define AC_DATE_PN		2
#define AC_DENP_PN		3
#define AC_DOCEVI_PN	4
#define AC_BMON_PN		5
#define AC_KOJI_PN		6
#define AC_KMK_PN		7
#define AC_TKY_PN		8
#define AC_HJIYU_PN		9
#define AC_NKIN_PN		10
#define AC_SKIN_PN		11
#define AC_ZEI_PN		12
#define AC_SYZ_PN		13
#define AC_SZAN_PN		14

#define AC_HUSEN_PN		100


#define AC_ICNT			(AC_SZAN_PN+1)

// 基本となる部門・工事・科目指定部分
#define AC_SELBMON_PN		500		//基本部門
#define AC_SELKOJI_PN		501		//基本工事
#define AC_SELKAMOK_PN		502		//基本科目
#define AC_SELBRN_PN		503		//基本枝番
#define AC_SELDISP_PN		504		//仕訳表示ボタン


// ラインコンディション
typedef struct _ABLN_CND
{
	char	INP_sg;			// 入力済みサイン
	char	INP_type;		// 入力タイプ（「借貸枝番」で使用）
									//	０...なし
									//	１...直接入力
									//	２...「借貸枝番」＝摘要枝番による枝番。
	char	TKattr;			// 摘要消費税コード実行サイン(コード)
	char	KEY_inp;		// キーボード入力あり

	char	NEXT_MOVE_KEY;
	char	IMG_sg;			// イメージサイン

	COLORREF	ETC_txtcol;		// 枝番摘要等 の表示文字色
	CString		Etcmsg;			// 枝番摘要等 別に表示する文字列

	char	dmy[10];

} ABLN_CND;


// 出納帳 １ラインデータ
typedef struct _ACBOOK_LINE 
{
	int				abs_pos;	// データベースポジション( 1 ～
	int				dtype;		// データタイプ( 0=ノーマル仕訳, 1=元帳クラスで作成された仕訳 )
	DWORD			ac_sign;	// D1 ON:　有価証券非課税譲渡仕訳

	CDBINPDataRec	drec;
	BYTE			bcddate[4];	// BCD で作成した日付

	ABLN_CND		lcnd[AC_ICNT];	// 入力状況サイン
	char			syzstr[36];		// 消費税 文字列

	char			szan[6];		// 差引残高

	char			denp[4];	// 伝票タイプ

	int				lattr;		// 行属性フラグ(0=仕訳行, 1=前期より繰越行)

} ACBOOK_LINE;


// 差引残高構造体
typedef struct _AC_SZAN
{
	char	ac_szan[6];
} AC_SZAN;


// 差引残高クラスに変更
class CACSzan
{
public:
	CACSzan() {
		clear();
	};
	~CACSzan() {};
	CACSzan(const CACSzan& obj){};

	CACSzan& operator=(const CACSzan& obj)
	{
		if( this != &obj ) {
			memcpy( ac_szan, obj.ac_szan, sizeof ac_szan );
			ac_szrec = obj.ac_szrec;
		}
		return *this;
	}
	void	clear()
	{
		::ZeroMemory( ac_szan, sizeof ac_szan );
		ac_szrec.Reset();
	}

public:
	//メンバを増やしたら、上のoperator=(), clear()にも反映する！
	char	ac_szan[6];
	CDBINPDataRec	ac_szrec;
};


#endif // INCLUDE_ACBOOKREC_H