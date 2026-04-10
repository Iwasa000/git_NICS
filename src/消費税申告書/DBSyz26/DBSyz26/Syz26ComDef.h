
#pragma once

//-----------------------------------------------------------------------------
// 消費税申告書：共通情報
//-----------------------------------------------------------------------------

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

// キー移動時
#define		REPLY_ERR		0x00000001
#define		REPLY_OK		0x00000002
#define		REPLY_NEW		0x00000004
#define		REPLY_NEXT		0x00010000
#define		REPLY_BACK		0x00020000
#define		REPLY_STAY		0x00040000
#define		REPLY_MOVE		0x00080000
#define		REPLY_DELT		0x80000000


//== 構造体 ==
// 付表５－２連動用
typedef struct _tag_ICS_SH_52_LINKED_INFO_ {
	int		**ptypeCnt;		// 業種数
	int		**ptkSw;		// 特例計算摘要スイッチ
	char	**pCQsw;		// 仕入税額集計サイン
	char	**pSzsw;		// 最も有利な特例計算適用時の仕入控除税額判定サイン
	char	**pSBsw;		// 
	int		**pdspSkjSw;	// 仕入控除税額計算の、どの行が表示されているか
	char	**pF5218T;		// 18. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5218A;		// 18. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5218B;		// 18. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5218C;		// 18. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5218c;		// 18. 原則計算 再計算スイッチ
	char	**pF5219T;		// 19. 特例適用･合計 仕入税額(期間計)
	char	**pF5219A;		// 19. 　　　　　　　　〃　　(3%分)	　
	char	**pF5219B;		// 19. 　　　　　　　　〃　　(4%分)	　
	char	**pF5219C;		// 19. 　　　　　　　　〃　　(6.3%分)
	int		**pF5219c;		// 19. 原則計算 再計算スイッチ

	char	**pF5220T;		// 20. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5220A;		// 20. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5220B;		// 20. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5220C;		// 20. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5220c;		// 20. 原則計算 再計算スイッチ
	char	**pF5221T;		// 21. 特例適用･合計 仕入税額(期間計)
	char	**pF5221A;		// 21. 　　　　　　　　〃　　(3%分)	　
	char	**pF5221B;		// 21. 　　　　　　　　〃　　(4%分)	　
	char	**pF5221C;		// 21. 　　　　　　　　〃　　(6.3%分)
	int		**pF5221c;		// 21. 原則計算 再計算スイッチ


	char	**pF5222T;		// 22. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5222A;		// 22. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5222B;		// 22. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5222C;		// 22. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5222c;		// 22. 原則計算 再計算スイッチ
	char	**pF5223T;		// 23. 特例適用･合計 仕入税額(期間計)
	char	**pF5223A;		// 23. 　　　　　　　　〃　　(3%分)	　
	char	**pF5223B;		// 23. 　　　　　　　　〃　　(4%分)	　
	char	**pF5223C;		// 23. 　　　　　　　　〃　　(6.3%分)
	int		**pF5223c;		// 23. 原則計算 再計算スイッチ

	char	**pF5224T;		// 24. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5224A;		// 24. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5224B;		// 24. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5224C;		// 24. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5224c;		// 24. 原則計算 再計算スイッチ
	char	**pF5225T;		// 25. 特例適用･合計 仕入税額(期間計)
	char	**pF5225A;		// 25. 　　　　　　　　〃　　(3%分)	　
	char	**pF5225B;		// 25. 　　　　　　　　〃　　(4%分)	　
	char	**pF5225C;		// 25. 　　　　　　　　〃　　(6.3%分)
	int		**pF5225c;		// 25. 原則計算 再計算スイッチ

	char	**pF5226T;		// 26. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5226A;		// 26. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5226B;		// 26. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5226C;		// 26. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5226c;		// 26. 原則計算 再計算スイッチ
	char	**pF5227T;		// 27. 特例適用･合計 仕入税額(期間計)
	char	**pF5227A;		// 27. 　　　　　　　　〃　　(3%分)	　
	char	**pF5227B;		// 27. 　　　　　　　　〃　　(4%分)	　
	char	**pF5227C;		// 27. 　　　　　　　　〃　　(6.3%分)
	int		**pF5227c;		// 27. 原則計算 再計算スイッチ

	char	**pF5228T;		// 28. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5228A;		// 28. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5228B;		// 28. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5228C;		// 28. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5228c;		// 28. 原則計算 再計算スイッチ
	char	**pF5229T;		// 29. 特例適用･合計 仕入税額(期間計)
	char	**pF5229A;		// 29. 　　　　　　　　〃　　(3%分)	　
	char	**pF5229B;		// 29. 　　　　　　　　〃　　(4%分)	　
	char	**pF5229C;		// 29. 　　　　　　　　〃　　(6.3%分)
	int		**pF5229c;		// 29. 原則計算 再計算スイッチ

	char	**pF5230T;		// 30. 選択した控除対象仕入税額(期間計)
	char	**pF5230A;		// 30. 　　　　　　　　〃　　(3%分)	　
	char	**pF5230B;		// 30. 　　　　　　　　〃　　(4%分)	　
	char	**pF5230C;		// 30. 　　　　　　　　〃　　(6.3%分)

} SH_52_LINKED_INFO;


//-------->[Y]
//== 構造体 ==
// 付表５－２連動用
typedef struct _tag_ICS_SH_52EX_LINKED_INFO_ {
	int		**ptypeCnt;		// 業種数
	int		**ptkSw;		// 特例計算摘要スイッチ
	char	**pCQsw;		// 仕入税額集計サイン
	char	**pSzsw;		// 最も有利な特例計算適用時の仕入控除税額判定サイン
	char	**pSBsw;		// 
	int		**pdspSkjSw;	// 仕入控除税額計算の、どの行が表示されているか
	char	**pF5220T;		// 20. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5220A;		// 20. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5220B;		// 20. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5220C;		// 20. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5220c;		// 20. 原則計算 再計算スイッチ
	char	**pF5221T;		// 21. 特例適用･合計 仕入税額(期間計)
	char	**pF5221A;		// 21. 　　　　　　　　〃　　(3%分)	　
	char	**pF5221B;		// 21. 　　　　　　　　〃　　(4%分)	　
	char	**pF5221C;		// 21. 　　　　　　　　〃　　(6.3%分)
	int		**pF5221c;		// 21. 原則計算 再計算スイッチ

	char	**pF5222T;		// 22. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5222A;		// 22. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5222B;		// 22. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5222C;		// 22. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5222c;		// 22. 原則計算 再計算スイッチ

	char	**pF5223T;		// 23. 特例適用･合計 仕入税額(期間計)
	char	**pF5223A;		// 23. 　　　　　　　　〃　　(3%分)	　
	char	**pF5223B;		// 23. 　　　　　　　　〃　　(4%分)	　
	char	**pF5223C;		// 23. 　　　　　　　　〃　　(6.3%分)
	int		**pF5223c;		// 23. 原則計算 再計算スイッチ

	char	**pF5224T;		// 24. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5224A;		// 24. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5224B;		// 24. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5224C;		// 24. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5224c;		// 24. 原則計算 再計算スイッチ

	char	**pF5225T;		// 25. 特例適用･合計 仕入税額(期間計)
	char	**pF5225A;		// 25. 　　　　　　　　〃　　(3%分)	　
	char	**pF5225B;		// 25. 　　　　　　　　〃　　(4%分)	　
	char	**pF5225C;		// 25. 　　　　　　　　〃　　(6.3%分)
	int		**pF5225c;		// 25. 原則計算 再計算スイッチ

	char	**pF5226T;		// 26. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5226A;		// 26. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5226B;		// 26. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5226C;		// 26. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5226c;		// 26. 原則計算 再計算スイッチ

	char	**pF5227T;		// 27. 特例適用･合計 仕入税額(期間計)
	char	**pF5227A;		// 27. 　　　　　　　　〃　　(3%分)	　
	char	**pF5227B;		// 27. 　　　　　　　　〃　　(4%分)	　
	char	**pF5227C;		// 27. 　　　　　　　　〃　　(6.3%分)
	int		**pF5227c;		// 27. 原則計算 再計算スイッチ

	char	**pF5228T;		// 28. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5228A;		// 28. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5228B;		// 28. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5228C;		// 28. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5228c;		// 28. 原則計算 再計算スイッチ

	char	**pF5229T;		// 29. 特例適用･合計 仕入税額(期間計)
	char	**pF5229A;		// 29. 　　　　　　　　〃　　(3%分)	　
	char	**pF5229B;		// 29. 　　　　　　　　〃　　(4%分)	　
	char	**pF5229C;		// 29. 　　　　　　　　〃　　(6.3%分)
	int		**pF5229c;		// 29. 原則計算 再計算スイッチ

	char	**pF5230T;		// 30. 原則計算適用･合計 仕入税額（期間計）
	char	**pF5230A;		// 30. 　　　　　　　 〃　　　　  （3%分）
	char	**pF5230B;		// 30. 　　　　　　　 〃　　　　  （4%分）
	char	**pF5230C;		// 30. 　　　　　　　 〃　　　　  （6.3%分）
	int		**pF5230c;		// 30. 原則計算 再計算スイッチ

	char	**pF5231T;		// 31. 特例適用･合計 仕入税額(期間計)
	char	**pF5231A;		// 31. 　　　　　　　　〃　　(3%分)	　
	char	**pF5231B;		// 31. 　　　　　　　　〃　　(4%分)	　
	char	**pF5231C;		// 31. 　　　　　　　　〃　　(6.3%分)
	int		**pF5231c;		// 31. 原則計算 再計算スイッチ

	char	**pF5232T;		// 32. 特例適用･合計 仕入税額(期間計)
	char	**pF5232A;		// 32. 　　　　　　　　〃　　(3%分)	　
	char	**pF5232B;		// 32. 　　　　　　　　〃　　(4%分)	　
	char	**pF5232C;		// 32. 　　　　　　　　〃　　(6.3%分)
	int		**pF5232c;		// 32. 原則計算 再計算スイッチ

	char	**pF5233T;		// 33. 特例適用･合計 仕入税額(期間計)
	char	**pF5233A;		// 33. 　　　　　　　　〃　　(3%分)	　
	char	**pF5233B;		// 33. 　　　　　　　　〃　　(4%分)	　
	char	**pF5233C;		// 33. 　　　　　　　　〃　　(6.3%分)
	int		**pF5233c;		// 33. 原則計算 再計算スイッチ

	char	**pF5234T;		// 34. 特例適用･合計 仕入税額(期間計)
	char	**pF5234A;		// 34. 　　　　　　　　〃　　(3%分)	　
	char	**pF5234B;		// 34. 　　　　　　　　〃　　(4%分)	　
	char	**pF5234C;		// 34. 　　　　　　　　〃　　(6.3%分)
	int		**pF5234c;		// 34. 原則計算 再計算スイッチ

	char	**pF5235T;		// 35. 特例適用･合計 仕入税額(期間計)
	char	**pF5235A;		// 35. 　　　　　　　　〃　　(3%分)	　
	char	**pF5235B;		// 35. 　　　　　　　　〃　　(4%分)	　
	char	**pF5235C;		// 35. 　　　　　　　　〃　　(6.3%分)
	int		**pF5235c;		// 35. 原則計算 再計算スイッチ

	char	**pF5236T;		// 36. 特例適用･合計 仕入税額(期間計)
	char	**pF5236A;		// 36. 　　　　　　　　〃　　(3%分)	　
	char	**pF5236B;		// 36. 　　　　　　　　〃　　(4%分)	　
	char	**pF5236C;		// 36. 　　　　　　　　〃　　(6.3%分)
	int		**pF5236c;		// 36. 原則計算 再計算スイッチ

	char	**pF5237T;		// 37. 選択した控除対象仕入税額(期間計)
	char	**pF5237A;		// 37. 　　　　　　　　〃　　(3%分)	　
	char	**pF5237B;		// 37. 　　　　　　　　〃　　(4%分)	　
	char	**pF5237C;		// 37. 　　　　　　　　〃　　(6.3%分)

} SH_52EX_LINKED_INFO;

// 付表５－２（金額フィールド保存用）
typedef struct _tag_ICS_SH_FH5EX_MONY_ {
	char	*pF5213[8];	// 13: 0:計, 1:3%, 2:5%, 3:8%
	char	*pF5214[8];	// 14:
	char	*pF5215[8];	// 15:
	char	*pF5216[8];	// 16:
	char	*pF5217[8];	// 17:
	char	*pF5218[8];	// 18:
	char	*pF5219[8];	// 19:
} SH_FH5EX_MONY;


//<--------------



// 付表５－２（金額フィールド保存用）
typedef struct _tag_ICS_SH_FH5_MONY_ {
	char	*pF5212[8];	// 12: 0:計, 1:3%, 2:5%, 3:8%
	char	*pF5213[8];	// 13:
	char	*pF5214[8];	// 14:
	char	*pF5215[8];	// 15:
	char	*pF5216[8];	// 16:
	char	*pF5217[8];	// 17:
} SH_FH5_MONY;

//== 文字列 ==
#define	FMT15	_T("SSS,SSS,SSS,SSS")
#define	FMT16	_T("SSS,SSS,SSS,SS9")
#define	FMT14	_T("SS,SSS,SSS,SSS")
#define	FMT13	_T("SSS,SSS,SSS,S")
#define	FMT12	_T("SSSSSSSSSSSS")
#define	FMT120	_T("SSSSSSSSSSS9")
#define	FMT110	_T("SSS,SSS,SSS,")
#define	FMT11	_T("SSS,SSS,SSS")
#define	FMT10	_T("SS,SSS,SSS")
#define	FMT010	_T("SSSSSSSSSS")
#define	FMT09	_T("SSSSSSSSS")
#define	FMT12S	_T("sss,sss,sss,sss")
//shimizu追加
#define	FMT011EX	_T("SSSSSSSSSSS")
#define	FMT12EX		_T("SSSSSSSSSSSSS")
#define	FMT120EX	_T("SSSSSSSSSSSS9")
#define	FMT13EX		_T("SSSSSSSSSSSSSS")
#define	FMT15_EX	_T("S,SSS,SSS,SSS,SSS")
#define	FMT16_EX	_T("SS,SSS,SSS,SSS,SSS")
#define	FMT110_EX	_T("SS,SSS,SSS,SSS,")
static	char	*FM10H = _T("s,sssssss");

#define	FMT14Z		_T("SS,SSS,SSS,SS9")		// '15.01.27
#define	FMT15_EXZ	_T("S,SSS,SSS,SSS,SS9")		// '15.01.27
#define	FMT11Z		_T("SSS,SSS,SS9")			// '15.02.03

//2016.10.20 INSERT START
#define	FMT16_EXZ	_T("SS,SSS,SSS,SSS,SS9")
//2016.10.20 INSERT END

// 元号
static	char*	SYZ26_EMPTY_G_STR	=	_T("　　");
static	char*	SYZ26_HEISEI_G_STR	=	_T("平成");
static	char*	SYZ26_REIWA_G_STR	=	_T("令和");
