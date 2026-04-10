#ifndef INCLUDE_SWKTKYDEF_H
#define	INCLUDE_SWKTKYDEF_H


// 摘要辞書登録画面より他の登録処理を行う ﾊﾟﾗﾒｰﾀ
typedef struct _TK_ADD
{
	int		ta_cod;			// 追加摘要コード( 反転はしていない )
	int		ta_typ;			// 追加する登録タイプ( 上記#define TP_XXX ) 

	union	{
		WORD	ta_kmktyp;		// 科目対応摘要登録タイプ
								// 0 = 借方, 1 = 貸方, 2 = 貸借
			
		struct {
			char	debcd[10],		// 仕訳対応摘要 借方コード
					crecd[10];		// 　　〃　　　 貸方コード
		} ta_swk[2];
	};

} TK_ADD;


// 仕訳選択 処理ｻｲﾝ
enum SwkSign 
{
	RSW_NONE,		// なし
	RSW_SIWAKE,		// 仕訳
	RSW_DEBONLY,	// 借方 のみ
	RSW_CREONLY,	// 貸方 のみ
	RSW_BOTH		// 貸借
};


// 仕訳選択 構造体
typedef struct _SWK_REGSEL
{
	CString	debcode,
			crecode;
	
	int		type;
	
} SWK_REGSEL;



#endif // INCLUDE_SWKTKYDEF_H