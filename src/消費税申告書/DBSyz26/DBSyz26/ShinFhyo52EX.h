#pragma once
#include "icsdiagctrl.h"


// CShinFhyo52EX ダイアログ

class CShinFhyo52EX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo52EX)

public:
	CShinFhyo52EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo52EX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY52 };

	// 初期情報のセット
	int InitInfo( CH27SnFhyo50Data **pSnFhyo52Data, CH27SnFhyo10Data **pSnFhyo40Data, CH27SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CDBNpSub *pZmSub, CSyzSyukei *pSubSyzSyukei );
//	int InitInfo( CH26SnFhyo50Data **pSnFhyo52Data, CH26SnFhyo10Data **pSnFhyo40Data, CH26SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CDBNpSub *pZmSub, CSyzSyukei *pSubSyzSyukei );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 金額フィールドの連結	('14.06.10)
	void ConnectMonyField();

	// データの読込み
	int ReadData( int upck );
	// データの書込み
	int WriteData();
	// 控除対象仕入税額の再書込み
	int ReWriteOnlySkjData();

	// 画面出力
	void DispFh52();
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 売上割合表示
	void dsp_uws( unsigned short Id, short Index, unsigned short Pers, short Sign );
	// ダイアグラムのインデックスから関連項目を取得する
	int GetRelInfoByDiagIdx( short Idx, char **pRtMony );
	// ダイアグラムのインデックスから基準金額を取得する
	int GetBaseMonyByDiagIdx( short Idx, char **pRtMony );

	//== 計算 ==
	// 課税標準額に対する消費税額
	void f52calq_1();
	// 貸倒回収に対する消費税額
	void f52calq_2();
	// 売上対価の返還等に係る消費税額
	void f52calq_3();
	// 控除対象仕入税額計算の基礎となる消費税額
	void f52calq_4();
	// 事業区分別の合計消費税額 （期間計）
	void f52calq_gts( int sg );
	// 事業区分別の合計消費税額
	void f52calq_gks();
	// 事業別課税売上割合の表示
	void f52calq_uws();
	// 第一種事業
	void f52calq_ku1();
	// 第二種事業
	void f52calq_ku2();
	// 第三種事業
	void f52calq_ku3();
	// 第四種事業
	void f52calq_ku4();
	// 第五種事業
	void f52calq_ku5();

	//[Y]
	// 第六種事業
	void f52calq_ku6();

	// 事業別課税売上割合の計算
/*- '14.06.09 -*/
//	void f52calq_ku();
/*-------------*/
	void f52calq_ku( int tk_clqsw=1 );
/*-------------*/
	// 各入力の計算表示
/*- '14.06.09 -*/
//	void f52calq_all( int atno );
/*-------------*/
	void f52calq_all( int atno, int tk_clqsw=1 );
/*-------------*/
	// １種類の事業の専業者の場合
	void f52calq_5();
	// 入力直後の特例計算サイン
	void Tokurei52();
	// 他表からの計算
	void calq_by_OtherChange( int sw );
	// 各事業の課税売上高の横計
	void f52calq_gsubtotal();
	// 売上構成比の計算
	int calq_CfrOfSales( char ssg );
	// 構成比の計算
	int upersnt( char *mm, char *tt );
	// 控除対象仕入税額の計算（原則計算を適用する場合）
	void f52calq_gn();
	// 控除対象仕入税額の計算（特例計算を適用する場合）
	void f52calq_tk();
	// 簡易課税　2種類以上の業種での特例計算チェック
	int f52_toku75( char *Base, char *Add1, char *Add2 );
	// 簡易課税　売上区分チェック
	int f52_urikbn();
	// 控除対象仕入税額の計算(原則計算)
	void f52calq_skj_gn( char *dest, int ztype );
	// 控除対象仕入税額の計算(特例計算)
	void f52calq_skj_tk( char *dest, const char *src, const char *num1, const char *rate1, const char *num2, const char *rate2, const char *deno );

	// １フィールドの控除対象仕入税額の計算	('14.06.10)
	void f52calq_one_skj( int rIdx, int cIdx, char *pVal );
	// １フィールドの控除対象仕入税額の計算(特例計算：2種類で75%以上)	('14.06.10)
	void f52calq_one_skj_tk75over( int rIdx, int cIdx, char *pVal );
	// １フィールドの控除対象仕入税額の計算(特例計算：1種類で75%以上)	('14.06.10)
	void f52calq_one_skj_one_tk75over( int rIdx, int cIdx );
	// 該当セルを計算するのに必要な金額フィールドを取得	('14.06.10)
	void f52calq_get_clqfield( int rIdx, int cIdx, char *pVal1, char *pValttl, char *pRate1, char *pRate2 );
	// １フィールドの控除対象仕入税額の計算(原則計算)	('14.06.10)
	void f52calq_one_skj_gen( int rIdx, int cIdx );
	// 控除対象仕入税額の計算方法の判定	('14.12.18)
	int judge_skj_calq();

	// 全計算
/*- '14.06.09 -*/
//	int f52calq_tk_all();
/*-------------*/
	int f52calq_tk_all( int tk_clqsw=1 );
/*-------------*/
	// 原則・特例：最有利になる表示位置を取得
	int f52calq_tk_for_disp();

	//== 付表５－２（２）との連動用 ==
	// 連動情報の取得
	void GetLinkedData( SH_52EX_LINKED_INFO *pLinkedInfo );

	//--------->フォーカス移動処理（上下）
	int UpDownFocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	//<----------------

	// 初期ポジションセット
	void SetItemPosition();

	// 最も有利な特例計算位置を取得
	int GetMostProfitCalq(){ return m_dspSkjSw; };


private:
	CWnd				*m_pParent;					// 親ウィンドウ

	char				F521T[MONY_BUF_SIZE],		//  1. 課税標準額に対する消費税額（期間計）
						F521A[MONY_BUF_SIZE],		//  1. 　　　　　　〃　　　　　　（3%分）
						F521B[MONY_BUF_SIZE],		//  1. 　　　　　　〃　　　　　　（4%分）
						F521C[MONY_BUF_SIZE];		//  1. 　　　　　　〃　　　　　　（6.3%分）

	char				F522T[MONY_BUF_SIZE],		//  2. 貸倒回収額に対する消費税額（期間計）
						F522A[MONY_BUF_SIZE],		//  2. 　　　　　　〃　　　　　　（3%分）
						F522B[MONY_BUF_SIZE],		//  2. 　　　　　　〃　　　　　　（4%分）
						F522C[MONY_BUF_SIZE];		//  2. 　　　　　　〃　　　　　　（6.3%分）

	char				F523T[MONY_BUF_SIZE],		//  3. 売上対価の返還等に係る消費税額（期間計）
						F523A[MONY_BUF_SIZE],		//  3. 　　　　　　〃　　　　　　　　（3%分）
						F523B[MONY_BUF_SIZE],		//  3. 　　　　　　〃　　　　　　　　（4%分）
						F523C[MONY_BUF_SIZE];		//  3. 　　　　　　〃　　　　　　　　（6.3%分）

	char				F524T[MONY_BUF_SIZE],		//  4. 控除対象仕入税額計算の為の消費税額（期間計）
						F524A[MONY_BUF_SIZE],		//	4. 　　　　　　　　〃　　　　　　　　（3%分）
						F524B[MONY_BUF_SIZE],		//	4. 　　　　　　　　〃　　　　　　　　（4%分）
						F524C[MONY_BUF_SIZE];		//	4. 　　　　　　　　〃　　　　　　　　（6.3%分）

	char				F525T[MONY_BUF_SIZE],		//	5. 単一事業の場合の控除対象仕入税額（期間計）
						F525A[MONY_BUF_SIZE],		//  5. 　　　　　　　　〃　　　　　　　（3%分）	
						F525B[MONY_BUF_SIZE],		//  5. 　　　　　　　　〃　　　　　　　（4%分）	
						F525C[MONY_BUF_SIZE];		//  5. 　　　　　　　　〃　　　　　　　（6.3%分）	

	char				F526T[MONY_BUF_SIZE],		//	6. 事業別課税売上高合計（期間計）
						F526A[MONY_BUF_SIZE],		//	6. 　　　　　〃　　　　（3%分）	
						F526B[MONY_BUF_SIZE],		//	6. 　　　　　〃　　　　（4%分）	
						F526C[MONY_BUF_SIZE];		//	6. 　　　　　〃　　　　（6.3%分）	

	char				F527T[MONY_BUF_SIZE],		//	7. 事業別課税売上高･第一種（期間計）
						F527A[MONY_BUF_SIZE],		//	7. 　　　　　〃　　　　　 （3%分）	
						F527B[MONY_BUF_SIZE],		//	7. 　　　　　〃　　　　　 （4%分）	
						F527C[MONY_BUF_SIZE];		//	7. 　　　　　〃　　　　　 （6.3%分）	

	char				F528T[MONY_BUF_SIZE],		//	8. 事業別課税売上高･第二種（期間計）
						F528A[MONY_BUF_SIZE],		//	8. 　　　　　〃　　　　　 （3%分）
						F528B[MONY_BUF_SIZE],		//	8. 　　　　　〃　　　　　 （4%分）
						F528C[MONY_BUF_SIZE];		//	8. 　　　　　〃　　　　　 （6.3%分）

	char				F529T[MONY_BUF_SIZE],		//	9. 事業別課税売上高･第三種（期間計）
						F529A[MONY_BUF_SIZE],		//	9. 　　　　　〃　　　　　 （3%分）
						F529B[MONY_BUF_SIZE],		//	9. 　　　　　〃　　　　　 （4%分）
						F529C[MONY_BUF_SIZE];		//	9. 　　　　　〃　　　　　 （6.3%分）

	char				F5210T[MONY_BUF_SIZE],		// 10. 事業別課税売上高･第四種（期間計）
						F5210A[MONY_BUF_SIZE],		// 10. 　　　　　〃　　　　　 （3%分）
						F5210B[MONY_BUF_SIZE],		// 10. 　　　　　〃　　　　　 （4%分）
						F5210C[MONY_BUF_SIZE];		// 10. 　　　　　〃　　　　　 （6.3%分）

	char				F5211T[MONY_BUF_SIZE],		// 11. 事業別課税売上高･第五種（期間計）
						F5211A[MONY_BUF_SIZE],		// 11. 　　　　　〃　　　　　 （3%分）
						F5211B[MONY_BUF_SIZE],		// 11. 　　　　　〃　　　　　 （4%分）
						F5211C[MONY_BUF_SIZE];		// 11. 　　　　　〃　　　　　 （6.3%分）

	char				F5212T[MONY_BUF_SIZE],		// 12. 事業別課税売上高･第六種（期間計）
						F5212A[MONY_BUF_SIZE],		// 12. 　　　　　〃　　　　　 （3%分）
						F5212B[MONY_BUF_SIZE],		// 12. 　　　　　〃　　　　　 （4%分）
						F5212C[MONY_BUF_SIZE];		// 12. 　　　　　〃　　　　　 （6.3%分）

	char				F5213T[MONY_BUF_SIZE],		// 13. 事業別課税売上に係る消費税額合計（期間計）
						F5213A[MONY_BUF_SIZE],		// 13. 　　　　　　　　〃　　　　　　　（3%分）
						F5213B[MONY_BUF_SIZE],		// 13. 　　　　　　　　〃　　　　　　　（4%分）
						F5213C[MONY_BUF_SIZE];		// 13. 　　　　　　　　〃　　　　　　　（6.3%分）

	char				F5214T[MONY_BUF_SIZE],		// 14. 事業別課税売上に係る消費税額･第一種（期間計）
						F5214A[MONY_BUF_SIZE],		// 14. 　　　　　　　　〃　　　　　	　　　（3%分）
						F5214B[MONY_BUF_SIZE],		// 14. 　　　　　　　　〃　　　　　	　　　（4%分）
						F5214C[MONY_BUF_SIZE];		// 14. 　　　　　　　　〃　　　　　	　　　（6.3%分）

	char				F5215T[MONY_BUF_SIZE],		// 15. 事業別課税売上に係る消費税額･第二種（期間計）
						F5215A[MONY_BUF_SIZE],		// 15. 　　　　　　　　〃　　　　　	　　　（3%分）
						F5215B[MONY_BUF_SIZE],		// 15. 　　　　　　　　〃　　　　　	　　　（4%分）
						F5215C[MONY_BUF_SIZE];		// 15. 　　　　　　　　〃　　　　　	　　　（6.3%分）

	char				F5216T[MONY_BUF_SIZE],		// 16. 事業別課税売上に係る消費税額･第三種（期間計）
						F5216A[MONY_BUF_SIZE],		// 16. 　　　　　　　　〃　　　　　	　　　（3%分）
						F5216B[MONY_BUF_SIZE],		// 16. 　　　　　　　　〃　　　　　	　　　（4%分）
						F5216C[MONY_BUF_SIZE];		// 16. 　　　　　　　　〃　　　　　	　　　（6.3%分）

	char				F5217T[MONY_BUF_SIZE],		// 17. 事業別課税売上に係る消費税額･第四種（期間計）
						F5217A[MONY_BUF_SIZE],		// 17. 　　　　　　　　〃　　　　　	　　　（3%分）
						F5217B[MONY_BUF_SIZE],		// 17. 　　　　　　　　〃　　　　　	　　　（4%分）
						F5217C[MONY_BUF_SIZE];		// 17. 　　　　　　　　〃　　　　　	　　　（6.3%分）

	char				F5218T[MONY_BUF_SIZE],		// 18. 事業別課税売上に係る消費税額･第五種（期間計）
						F5218A[MONY_BUF_SIZE],		// 18. 　　　　　　　　〃　　　　　	　　　（3%分）
						F5218B[MONY_BUF_SIZE],		// 18. 　　　　　　　　〃　　　　　	　　　（4%分）
						F5218C[MONY_BUF_SIZE];		// 18. 　　　　　　　　〃　　　　　	　　　（6.3%分）

	char				F5219T[MONY_BUF_SIZE],		// 19. 事業別課税売上に係る消費税額･第六種（期間計）
						F5219A[MONY_BUF_SIZE],		// 19. 　　　　　　　　〃　　　　　	　　　（3%分）
						F5219B[MONY_BUF_SIZE],		// 19. 　　　　　　　　〃　　　　　	　　　（4%分）
						F5219C[MONY_BUF_SIZE];		// 19. 　　　　　　　　〃　　　　　	　　　（6.3%分）
		
	char				F5220T[MONY_BUF_SIZE],		// 20. 原則計算適用･合計 仕入税額（期間計）
						F5220A[MONY_BUF_SIZE],		// 20. 　　　　　　　 〃　　　　  （3%分）
						F5220B[MONY_BUF_SIZE],		// 20. 　　　　　　　 〃　　　　  （4%分）
						F5220C[MONY_BUF_SIZE];		// 20. 　　　　　　　 〃　　　　  （6.3%分）

	char				F5221T[MONY_BUF_SIZE],		// 21. 特例適用･合計 仕入税額(期間計)	
						F5221A[MONY_BUF_SIZE],		// 21. 　　　　　　　　〃　　(3%分)	　
						F5221B[MONY_BUF_SIZE],		// 21. 　　　　　　　　〃　　(4%分)	　
						F5221C[MONY_BUF_SIZE];		// 21. 　　　　　　　　〃　　(6.3%分)	　

	char				F5222T[MONY_BUF_SIZE],		// 22. 第一種及び第二種事業(期間計)	
						F5222A[MONY_BUF_SIZE],		// 22. 　　　　　　　　〃　　(3%分)	　
						F5222B[MONY_BUF_SIZE],		// 22. 　　　　　　　　〃　　(4%分)	　
						F5222C[MONY_BUF_SIZE];		// 22. 　　　　　　　　〃　　(6.3%分)	　
	int					F5222c;						// 22. 手入力サイン

	char				F5223T[MONY_BUF_SIZE],		// 23. 第一種及び第三種事業(期間計)	
						F5223A[MONY_BUF_SIZE],		// 23. 　　　　　　　　〃　　(3%分)	　
						F5223B[MONY_BUF_SIZE],		// 23. 　　　　　　　　〃　　(4%分)	　
						F5223C[MONY_BUF_SIZE];		// 23. 　　　　　　　　〃　　(6.3%分)	　
	int					F5223c;						// 23. 手入力サイン

	char				F5224T[MONY_BUF_SIZE],		// 24. 第一種及び第四種事業(期間計)	
						F5224A[MONY_BUF_SIZE],		// 24. 　　　　　　　　〃　　(3%分)	　
						F5224B[MONY_BUF_SIZE],		// 24. 　　　　　　　　〃　　(4%分)	　
						F5224C[MONY_BUF_SIZE];		// 24. 　　　　　　　　〃　　(6.3%分)	　
	int					F5224c;						// 24. 手入力サイン

	char				F5225T[MONY_BUF_SIZE],		// 25. 第一種及び第五種事業(期間計)	
						F5225A[MONY_BUF_SIZE],		// 25. 　　　　　　　　〃　　(3%分)	　
						F5225B[MONY_BUF_SIZE],		// 25. 　　　　　　　　〃　　(4%分)	　
						F5225C[MONY_BUF_SIZE];		// 25. 　　　　　　　　〃　　(6.3%分)	　
	int					F5225c;						// 25. 手入力サイン

	char				F5226T[MONY_BUF_SIZE],		// 26. 第一種及び第六種事業(期間計)	
						F5226A[MONY_BUF_SIZE],		// 26. 　　　　　　　　〃　　(3%分)	　
						F5226B[MONY_BUF_SIZE],		// 26. 　　　　　　　　〃　　(4%分)	　
						F5226C[MONY_BUF_SIZE];		// 26. 　　　　　　　　〃　　(6.3%分)	　
	int					F5226c;						// 26. 手入力サイン

	char				F5227T[MONY_BUF_SIZE],		// 27. 第二種及び第三種事業(期間計)	
						F5227A[MONY_BUF_SIZE],		// 27. 　　　　　　　　〃　　(3%分)	　
						F5227B[MONY_BUF_SIZE],		// 27. 　　　　　　　　〃　　(4%分)	　
						F5227C[MONY_BUF_SIZE];		// 27. 　　　　　　　　〃　　(6.3%分)	　
	int					F5227c;						// 27. 手入力サイン

	char				F5228T[MONY_BUF_SIZE],		// 28. 第二種及び第四種事業(期間計)	
						F5228A[MONY_BUF_SIZE],		// 28. 　　　　　　　　〃　　(3%分)	　
						F5228B[MONY_BUF_SIZE],		// 28. 　　　　　　　　〃　　(4%分)	　
						F5228C[MONY_BUF_SIZE];		// 28. 　　　　　　　　〃　　(6.3%分)	　
	int					F5228c;						// 28. 手入力サイン

	char				F5229T[MONY_BUF_SIZE],		// 29. 第二種及び第五種事業(期間計)	
						F5229A[MONY_BUF_SIZE],		// 29. 　　　　　　　　〃　　(3%分)	　
						F5229B[MONY_BUF_SIZE],		// 29. 　　　　　　　　〃　　(4%分)	　
						F5229C[MONY_BUF_SIZE];		// 29. 　　　　　　　　〃　　(6.3%分)	　
	int					F5229c;						// 29. 手入力サイン

	char				F5230T[MONY_BUF_SIZE],		// 30. 第二種及び第六種事業(期間計)	
						F5230A[MONY_BUF_SIZE],		// 30. 　　　　　　　　〃　　(3%分)	　
						F5230B[MONY_BUF_SIZE],		// 30. 　　　　　　　　〃　　(4%分)	　
						F5230C[MONY_BUF_SIZE];		// 30. 　　　　　　　　〃　　(6.3%分)	　
	int					F5230c;						// 30. 手入力サイン

	char				F5231T[MONY_BUF_SIZE],		// 31. 第三種及び第四種事業(期間計)	
						F5231A[MONY_BUF_SIZE],		// 31. 　　　　　　　　〃　　(3%分)	　
						F5231B[MONY_BUF_SIZE],		// 31. 　　　　　　　　〃　　(4%分)	　
						F5231C[MONY_BUF_SIZE];		// 31. 　　　　　　　　〃　　(6.3%分)	　
	int					F5231c;						// 31. 手入力サイン

	char				F5232T[MONY_BUF_SIZE],		// 32. 第三種及び第五種事業(期間計)	
						F5232A[MONY_BUF_SIZE],		// 32. 　　　　　　　　〃　　(3%分)	　
						F5232B[MONY_BUF_SIZE],		// 32. 　　　　　　　　〃　　(4%分)	　
						F5232C[MONY_BUF_SIZE];		// 32. 　　　　　　　　〃　　(6.3%分)	　
	int					F5232c;						// 32. 手入力サイン

	char				F5233T[MONY_BUF_SIZE],		// 33. 第三種及び第六種事業(期間計)	
						F5233A[MONY_BUF_SIZE],		// 33. 　　　　　　　　〃　　(3%分)	　
						F5233B[MONY_BUF_SIZE],		// 33. 　　　　　　　　〃　　(4%分)	　
						F5233C[MONY_BUF_SIZE];		// 33. 　　　　　　　　〃　　(6.3%分)	　
	int					F5233c;						// 33. 手入力サイン

	char				F5234T[MONY_BUF_SIZE],		// 34. 第四種及び第五種事業(期間計)	
						F5234A[MONY_BUF_SIZE],		// 34. 　　　　　　　　〃　　(3%分)	　
						F5234B[MONY_BUF_SIZE],		// 34. 　　　　　　　　〃　　(4%分)	　
						F5234C[MONY_BUF_SIZE];		// 34. 　　　　　　　　〃　　(6.3%分)	　
	int					F5234c;						// 34. 手入力サイン

	char				F5235T[MONY_BUF_SIZE],		// 35. 第四種及び第六種事業(期間計)	
						F5235A[MONY_BUF_SIZE],		// 35. 　　　　　　　　〃　　(3%分)	　
						F5235B[MONY_BUF_SIZE],		// 35. 　　　　　　　　〃　　(4%分)	　
						F5235C[MONY_BUF_SIZE];		// 35. 　　　　　　　　〃　　(6.3%分)	　
	int					F5235c;						// 35. 手入力サイン

	char				F5236T[MONY_BUF_SIZE],		// 36. 第五種及び第六種事業(期間計)	
						F5236A[MONY_BUF_SIZE],		// 36. 　　　　　　　　〃　　(3%分)	　
						F5236B[MONY_BUF_SIZE],		// 36. 　　　　　　　　〃　　(4%分)	　
						F5236C[MONY_BUF_SIZE];		// 36. 　　　　　　　　〃　　(6.3%分)	　
	int					F5236c;						// 36. 手入力サイン

	char				F5237T[MONY_BUF_SIZE],		// 37. 選択した控除対象仕入税額(期間計)	
						F5237A[MONY_BUF_SIZE],		// 37. 　　　　　　　　〃　　(3%分)	　
						F5237B[MONY_BUF_SIZE],		// 37. 　　　　　　　　〃　　(4%分)	　
						F5237C[MONY_BUF_SIZE];		// 37. 　　　　　　　　〃　　(6.3%分)	　

	char				F526S[MONY_BUF_SIZE],		//	6. 事業別課税売上高合計（期間計）
						F526SS[MONY_BUF_SIZE];		//	6. 事業別課税売上高合計（期間計）

	char				F524S[MONY_BUF_SIZE];		//  4. 控除対象仕入税額計算の為の消費税額（期間計）

	SH_FH5EX_MONY		m_Fh5Mony;					// 付表5の金額フィールド管理用('14.06.10)

	int					F520c,						// 
						F521c,						//  7.第一種 再計算スイッチ
						F522c,						//  8.第二種 再計算スイッチ
						F523c,						//  9.第三種 再計算スイッチ
						F524c,						// 10.第四種 再計算スイッチ
						F525c,						// 11.第五種 再計算スイッチ
						F526c;						// 12.第六種 再計算スイッチ


													//[Y]（２）消費税額の明細用
	int					F5213c,						// 
						F5214c,						//  14.第一種 再計算スイッチ
						F5215c,						//  15.第二種 再計算スイッチ
						F5216c,						//  16.第三種 再計算スイッチ
						F5217c,						//  17.第四種 再計算スイッチ
						F5218c,						//  18.第五種 再計算スイッチ
						F5219c;						//  19.第六種 再計算スイッチ
													// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)												



	int					F5220c,						// 原則計算 再計算スイッチ
						F5221c;						// 特例計算 再計算スイッチ

	unsigned short		F521UPb,					// 課税売上割合・第一種	(4%分)
						F522UPb,					// 課税売上割合・第二種	(4%分)
						F523UPb,					// 課税売上割合・第三種	(4%分)
						F524UPb,					// 課税売上割合・第四種	(4%分)
						F525UPb,					// 課税売上割合・第五種	(4%分)
						F526UPb;					// 課税売上割合・第六種	(4%分)

	//== 帳票データ本体 ==
	CH27SnFhyo50Data	**m_pSnFhyo52Data;			// 付表５－２
	CH27SnFhyo10Data	**m_pSnFhyo40Data;			// 付表４
	CH27SnHonpyoData	**m_pSnHonpyoData;			// 本表
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報
	CDBNpSub			*m_pZmSub;					// 公益クラス
	CSyzSyukei			*m_pSubSyzSyukei;			// 集計クラス

	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_typeCnt;					// 業種数
	char				m_CQsw;						// 仕入税額集計サイン
													// □□□■ □□□■
													//  D0：ON:単一事業, OFF:複数事業
													//  D4：手入力による変更あり（原則計算結果・特例計算結果で手入力）

	char				m_Szsw;						// 最も有利な特例計算適用時の仕入控除税額判定サイン
													//  1:19, 2:20, 3:21, 4:22, 5:23, 6:24, 7:25, 8:26, 9:27, 10:28, 11:29
													//  0:18 or 該当無し

	char				m_SvSzsw;					// 一つ前の最も有利な特例計算適用時の仕入控除税額判定サイン保存用（'14.06.11）
	int					m_dspSkjSw;					// 仕入控除税額計算の、どの行が表示されているか（'14.06.11）
													// □□□□ □□□□  □□□□ □□□■  ■■■■ ■■■■  ■■■■ ■■■■
													//  D0：20(原則計算)
													//  D1：21(特例計算：1種類の事業で75％以上)
													//  D2：22(特例計算：2種類の事業で75％以上)
													//  D3：23(〃)
													//  D4：24(〃)
													//  D5：25(〃)
													//  D6：26(〃)
													//  D7：27(〃)
													//  D8：28(〃)
													//  D9：29(〃)
													//  D10：30(〃)
													//  D11：31(〃)
													//  D12：32(〃)
													//  D13：33(〃)
													//  D14：34(〃)
													//  D15：35(〃)
													//  D16：36(〃)


	int					m_tkSw;						// 特例計算摘要スイッチ
													// □■■■ ■■■■  ■■■■ ■■■■  □□□□ □□□□  □□■■ ■■■■
													//  D0 - D15：単一業種の時の選択事業
													//   D0：第一業種
													//   D1：第二業種
													//   D2：第三業種
													//   D3：第四業種
													//   D4：第五業種
													//   D5：第六業種
													//  D16 - D31：二種類の時の選択事業
													//   D16：１＋２
													//   D17：１＋３
													//   D18：１＋４
													//   D19：１＋５
													//   D20：１＋６
													//   D21：２＋３
													//   D22：２＋４
													//   D23：２＋５
													//   D24：２＋６
													//   D25：３＋４
													//   D26：３＋５
													//   D27：３＋６
													//   D28：４＋５
													//   D29：４＋６
													//   D30：５＋６
													
	char				m_SBsw;						// 

	int					m_FrSw;						// ???
	int					m_Tmp;						// ???
	int					m_DpSw;						// ???

	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Fhyo52Diag01;
	CIcsdiagctrl m_Fhyo52Diag02;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
