#pragma once
#include "icsdiagctrl.h"


// CShinFhyo50EX ダイアログ

class CShinFhyo50EX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo50EX)

public:
	CShinFhyo50EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo50EX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY50 };

	// 初期情報のセット
	int InitInfo( CH27SnFhyo50Data **pSnFhyo50Data, CH27SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CH27SnHonpyoData **pSnHonpyoData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// 画面出力
	void DispFh5();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 売上割合表示
	void dsp_uws( unsigned short Id, short Index, unsigned short Pers, short Sign );
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );
	// 各入力データ抽出
	void GetAllDiagData();

	// データの読込み
	int ReadData( int upck );
	// データの書込み
	int WriteData();

	//== 計算 ==
	// 控除対象仕入税額計算の基礎となる消費税額
	void f5calq_4();
	// １種類の事業の専業者の場合
	void f5calq_5();
	// 事業区分別の合計消費税額 （期間計）
	void f5calq_gts( char ssg );
	// 事業別課税売上割合の表示
	void f5calq_uws();
	// 第一種事業
	void f5calq_ku1();
	// 第二種事業
	void f5calq_ku2();
	// 第三種事業
	void f5calq_ku3();
	// 第四種事業
	void f5calq_ku4();
	// 第五種事業
	void f5calq_ku5();
	// 第六種事業
	void f5calq_ku6();
	// 事業別課税売上割合の計算
	void f5calq_ku();
	// 原則計算を適用する場合
	void f5calq_gn();
	// 控除対象仕入税額の計算(原則計算)
	void f50calq_skj_gn( char *dest );
	// 控除対象仕入税額の計算式区分の表示
	void f5dsp_tk();
	// 控除対象仕入税額の計算(特例計算)
	void f50calq_skj_tk( char *dest, const char *src, const char *num1, const char *rate1, const char *num2, const char *rate2, const char *deno );
	// 入力直後の特例計算サイン
	void Tokurei50();
	// 控除対象仕入税額の計算
	void f5calq_tk();
	// 各入力の計算表示
	void f5calq_all( int atno );
	// 簡易課税　2種類以上の業種での特例計算チェック
	int f5_toku75( char *Base, char *Add1, char *Add2 );
	// 他表からの計算
	void calq_by_OtherChange();
	// 構成比の計算
	int upersnt( char *mm, char *tt );
	// 売上構成比の計算
	int uwariai( char ssg );
	// 事業区分別の合計額の計算
	int f5calq_6();

	//--------->フォーカス移動処理（上下）
	int UpDownFocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	//<----------------

	// 初期ポジションセット
	void SetItemPosition();

	void AllTkSwClear();
	int TkSwCheck(int idx);

	// 課税取引金額計算書から転記時のフォーカス移動
	void MoveFocusInTaxTenki();

private:

	CWnd				*m_pParent;					// 親ウィンドウ

	char				F51T[MONY_BUF_SIZE],		// 1. 課税標準額に対する消費税額（期間計）
						F51A[MONY_BUF_SIZE],		// 1. 　　　　　　〃　　　　　　（3%分）
						F51B[MONY_BUF_SIZE],		// 1. 　　　　　　〃　　　　　　（4%分）
						F51C[MONY_BUF_SIZE];		// 1. 　　　　　　〃　　　　　　（6.3%分）

	char				F52T[MONY_BUF_SIZE],		// 2. 貸倒回収額に対する消費税額（期間計）
						F52A[MONY_BUF_SIZE],		// 2. 　　　　　　〃　　　　　　（3%分）
						F52B[MONY_BUF_SIZE],		// 2. 　　　　　　〃　　　　　　（4%分）
						F52C[MONY_BUF_SIZE];		// 2. 　　　　　　〃　　　　　　（6.3%分）

	char				F53T[MONY_BUF_SIZE],		// 3. 売上対価の返還等に係る消費税額（期間計）
						F53A[MONY_BUF_SIZE],		// 3. 　　　　　　〃　　　　　　　　（3%分）
						F53B[MONY_BUF_SIZE],		// 3. 　　　　　　〃　　　　　　　　（4%分）
						F53C[MONY_BUF_SIZE];		// 3. 　　　　　　〃　　　　　　　　（6.3%分）

	char				F54T[MONY_BUF_SIZE],		// 4. 控除対象仕入税額計算の為の消費税額（期間計）
						F54A[MONY_BUF_SIZE],		// 4. 　　　　　　　　〃　　　　　　　　（3%分）
						F54B[MONY_BUF_SIZE],		// 4. 　　　　　　　　〃　　　　　　　　（4%分）
						F54C[MONY_BUF_SIZE],		// 4. 　　　　　　　　〃　　　　　　　　（6.3%分）
						F54S[MONY_BUF_SIZE];		//

	char				F55T[MONY_BUF_SIZE],		// 5. 単一事業の場合の控除対象仕入税額（期間計）
						F55A[MONY_BUF_SIZE],		// 5. 　　　　　　　　〃　　　　　　　（3%分）	
						F55B[MONY_BUF_SIZE],		// 5. 　　　　　　　　〃　　　　　　　（4%分）	
						F55C[MONY_BUF_SIZE];		// 5. 　　　　　　　　〃　　　　　　　（6.3%分）	

	char				F56T[MONY_BUF_SIZE],		// 6. 事業別課税売上高合計（期間計）
						F56A[MONY_BUF_SIZE],		// 6. 　　　　　〃　　　　（3%分）
						F56B[MONY_BUF_SIZE],		// 6. 　　　　　〃　　　　（4%分）
						F56C[MONY_BUF_SIZE],		// 6. 　　　　　〃　　　　（6.3%分）
						F56S[MONY_BUF_SIZE],
						F56SS[MONY_BUF_SIZE];
	int					F56sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F57T[MONY_BUF_SIZE],		// 7. 事業別課税売上高･第一種（期間計）
						F57A[MONY_BUF_SIZE],		// 7. 　　　　　〃　　　　　 （3%分）
						F57B[MONY_BUF_SIZE],		// 7. 　　　　　〃　　　　　 （4%分）
						F57C[MONY_BUF_SIZE];		// 7. 　　　　　〃　　　　　 （6.3%分）

	char				F58T[MONY_BUF_SIZE],		// 8. 事業別課税売上高･第二種（期間計）
						F58A[MONY_BUF_SIZE],		// 8. 　　　　　〃　　　　　 （3%分）
						F58B[MONY_BUF_SIZE],		// 8. 　　　　　〃　　　　　 （4%分）
						F58C[MONY_BUF_SIZE];		// 8. 　　　　　〃　　　　　 （6.3%分）

	char				F59T[MONY_BUF_SIZE],		// 9. 事業別課税売上高･第三種（期間計）
						F59A[MONY_BUF_SIZE],		// 9. 　　　　　〃　　　　　 （3%分）
						F59B[MONY_BUF_SIZE],		// 9. 　　　　　〃　　　　　 （4%分）
						F59C[MONY_BUF_SIZE];		// 9. 　　　　　〃　　　　　 （6.3%分）

	char				F510T[MONY_BUF_SIZE],		// 10. 事業別課税売上高･第四種（期間計）
						F510A[MONY_BUF_SIZE],		// 10. 　　　　　〃　　　　　 （3%分）
						F510B[MONY_BUF_SIZE],		// 10. 　　　　　〃　　　　　 （4%分）
						F510C[MONY_BUF_SIZE];		// 10. 　　　　　〃　　　　　 （6.3%分）

	char				F511T[MONY_BUF_SIZE],		// 11. 事業別課税売上高･第五種（期間計）
						F511A[MONY_BUF_SIZE],		// 11. 　　　　　〃　　　　　 （3%分）
						F511B[MONY_BUF_SIZE],		// 11. 　　　　　〃　　　　　 （4%分）
						F511C[MONY_BUF_SIZE];		// 11. 　　　　　〃　　　　　 （6.3%分）


	char				F512T[MONY_BUF_SIZE],		// 12. 事業別課税売上高･第六種（期間計）
						F512A[MONY_BUF_SIZE],		// 12. 　　　　　〃　　　　　 （3%分）
						F512B[MONY_BUF_SIZE],		// 12. 　　　　　〃　　　　　 （4%分）
						F512C[MONY_BUF_SIZE];		// 12. 　　　　　〃　　　　　 （6.3%分）


	char				F513T[MONY_BUF_SIZE],		// 13. 事業別課税売上に係る消費税額合計（期間計）
						F513A[MONY_BUF_SIZE],		// 13. 　　　　　　　　〃　　　　　　　（3%分）
						F513B[MONY_BUF_SIZE],		// 13. 　　　　　　　　〃　　　　　　　（4%分）
						F513C[MONY_BUF_SIZE];		// 13. 　　　　　　　　〃　　　　　　　（6.3%分）
	int					F513sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F514T[MONY_BUF_SIZE],		// 14. 事業別課税売上に係る消費税額･第一種（期間計）
						F514A[MONY_BUF_SIZE],		// 14. 　　　　　　　　〃　　　　　	　　　（3%分）
						F514B[MONY_BUF_SIZE],		// 14. 　　　　　　　　〃　　　　　	　　　（4%分）
						F514C[MONY_BUF_SIZE];		// 14. 　　　　　　　　〃　　　　　	　　　（6.3%分）
	int					F514sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F515T[MONY_BUF_SIZE],		// 15. 事業別課税売上に係る消費税額･第二種（期間計）
						F515A[MONY_BUF_SIZE],		// 15. 　　　　　　　　〃　　　　　	　　　（3%分）
						F515B[MONY_BUF_SIZE],		// 15. 　　　　　　　　〃　　　　　	　　　（4%分）
						F515C[MONY_BUF_SIZE];		// 15. 　　　　　　　　〃　　　　　	　　　（6.3%分）
	int					F515sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F516T[MONY_BUF_SIZE],		// 16. 事業別課税売上に係る消費税額･第三種（期間計）
						F516A[MONY_BUF_SIZE],		// 16. 　　　　　　　　〃　　　　　	　　　（3%分）
						F516B[MONY_BUF_SIZE],		// 16. 　　　　　　　　〃　　　　　	　　　（4%分）
						F516C[MONY_BUF_SIZE];		// 16. 　　　　　　　　〃　　　　　	　　　（6.3%分）
	int					F516sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F517T[MONY_BUF_SIZE],		// 17. 事業別課税売上に係る消費税額･第四種（期間計）
						F517A[MONY_BUF_SIZE],		// 17. 　　　　　　　　〃　　　　　	　　　（3%分）
						F517B[MONY_BUF_SIZE],		// 17. 　　　　　　　　〃　　　　　	　　　（4%分）
						F517C[MONY_BUF_SIZE];		// 17. 　　　　　　　　〃　　　　　	　　　（6.3%分）
	int					F517sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F518T[MONY_BUF_SIZE],		// 18. 事業別課税売上に係る消費税額･第五種（期間計）
						F518A[MONY_BUF_SIZE],		// 18. 　　　　　　　　〃　　　　　	　　　（3%分）
						F518B[MONY_BUF_SIZE],		// 18. 　　　　　　　　〃　　　　　	　　　（4%分）
						F518C[MONY_BUF_SIZE];		// 18. 　　　　　　　　〃　　　　　	　　　（6.3%分）
	int					F518sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F519T[MONY_BUF_SIZE],		// 19. 事業別課税売上に係る消費税額･第六種（期間計）
						F519A[MONY_BUF_SIZE],		// 19. 　　　　　　　　〃　　　　　	　　　（3%分）
						F519B[MONY_BUF_SIZE],		// 19. 　　　　　　　　〃　　　　　	　　　（4%分）
						F519C[MONY_BUF_SIZE];		// 19. 　　　　　　　　〃　　　　　	　　　（6.3%分）
	int					F519sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)

	char				F520T[MONY_BUF_SIZE],		// 20. 原則計算適用･合計 仕入税額（期間計）
						F520A[MONY_BUF_SIZE],		// 20. 　　　　　　　 〃　　　　 （3%分）
						F520B[MONY_BUF_SIZE],		// 20. 　　　　　　　 〃　　　　 （4%分）
						F520C[MONY_BUF_SIZE];		// 20. 　　　　　　　 〃　　　　 （6.3%分）

	char				F521T[MONY_BUF_SIZE],		// 21. 特例適用･合計 仕入税額(期間計)
						F521A[MONY_BUF_SIZE],		// 21. 　　　　　　　　〃　　(3%分)　　　
						F521B[MONY_BUF_SIZE],		// 21. 　　　　　　　　〃　　(4%分)　
						F521C[MONY_BUF_SIZE];		// 21. 　　　　　　　　〃　　(6.3%分)　


	char				SZsw,						// 特例計算適用SW
						SBsw,						// 特例計算適用SW (税率4%分)
						CQsw;						// 集計・計算済サイン	□□□□ □□□□ □□□□ □□□■
													//  D0:１種類の事業の専業者の場合
//	char				F50c,		// F512sw で代用
//						F51c,		// F513sw で代用
//						F52c,		// F514sw で代用
//						F53c,		// F515sw で代用
//						F54c,		// F516sw で代用
//						F55c;		// F517sw で代用
	unsigned short		F51UPb,						// 課税売上割合・第一種	(4%分)
						F52UPb,						// 課税売上割合・第二種	(4%分)
						F53UPb,						// 課税売上割合・第三種	(4%分)
						F54UPb,						// 課税売上割合・第四種	(4%分)
						F55UPb,						// 課税売上割合・第五種	(4%分)
						F56UPb;						// 課税売上割合・第六種	(4%分)

	int					F520sw;						// 手入力サイン	□□□□ ■□□■ ■□□■ ■□□■
													//  D0:関連する箇所で入力あり, D3:自分自身を変更(税率3%)
													//  D4:関連する箇所で入力あり, D7:自分自身を変更(税率5%)
													//  D8:関連する箇所で入力あり, D11:自分自身を変更(税率8%)
	int					F521sw,
						F522sw,
						F523sw,
						F524sw,
						F525sw,
						F526sw,
						F527sw,
						F528sw,
						F529sw,
						F530sw,
						F531sw,
						F532sw,
						F533sw,
						F534sw,
						F535sw,
						F536sw;


	char				F522T[MONY_BUF_SIZE],		// 22. 特例適用･合計 仕入税額(期間計)
						F522A[MONY_BUF_SIZE],		// 22. 　　　　　　　　〃　　(3%分)　　　
						F522B[MONY_BUF_SIZE],		// 22. 　　　　　　　　〃　　(4%分)　
						F522C[MONY_BUF_SIZE];		// 22. 　　　　　　　　〃　　(6.3%分)　
	char				F523T[MONY_BUF_SIZE],		// 23. 特例適用･合計 仕入税額(期間計)
						F523A[MONY_BUF_SIZE],		// 23. 　　　　　　　　〃　　(3%分)　　　
						F523B[MONY_BUF_SIZE],		// 23. 　　　　　　　　〃　　(4%分)　
						F523C[MONY_BUF_SIZE];		// 23. 　　　　　　　　〃　　(6.3%分)　
	char				F524T[MONY_BUF_SIZE],		// 24. 特例適用･合計 仕入税額(期間計)
						F524A[MONY_BUF_SIZE],		// 24. 　　　　　　　　〃　　(3%分)　　　
						F524B[MONY_BUF_SIZE],		// 24. 　　　　　　　　〃　　(4%分)　
						F524C[MONY_BUF_SIZE];		// 24. 　　　　　　　　〃　　(6.3%分)　
	char				F525T[MONY_BUF_SIZE],		// 25. 特例適用･合計 仕入税額(期間計)
						F525A[MONY_BUF_SIZE],		// 25. 　　　　　　　　〃　　(3%分)　　　
						F525B[MONY_BUF_SIZE],		// 25. 　　　　　　　　〃　　(4%分)　
						F525C[MONY_BUF_SIZE];		// 25. 　　　　　　　　〃　　(6.3%分)　
	char				F526T[MONY_BUF_SIZE],		// 26. 特例適用･合計 仕入税額(期間計)
						F526A[MONY_BUF_SIZE],		// 26. 　　　　　　　　〃　　(3%分)　　　
						F526B[MONY_BUF_SIZE],		// 26. 　　　　　　　　〃　　(4%分)　
						F526C[MONY_BUF_SIZE];		// 26. 　　　　　　　　〃　　(6.3%分)　
	char				F527T[MONY_BUF_SIZE],		// 27. 特例適用･合計 仕入税額(期間計)
						F527A[MONY_BUF_SIZE],		// 27. 　　　　　　　　〃　　(3%分)　　　
						F527B[MONY_BUF_SIZE],		// 27. 　　　　　　　　〃　　(4%分)　
						F527C[MONY_BUF_SIZE];		// 27. 　　　　　　　　〃　　(6.3%分)　
	char				F528T[MONY_BUF_SIZE],		// 28. 特例適用･合計 仕入税額(期間計)
						F528A[MONY_BUF_SIZE],		// 28. 　　　　　　　　〃　　(3%分)　　　
						F528B[MONY_BUF_SIZE],		// 28. 　　　　　　　　〃　　(4%分)　
						F528C[MONY_BUF_SIZE];		// 28. 　　　　　　　　〃　　(6.3%分)　
	char				F529T[MONY_BUF_SIZE],		// 29. 特例適用･合計 仕入税額(期間計)
						F529A[MONY_BUF_SIZE],		// 29. 　　　　　　　　〃　　(3%分)　　　
						F529B[MONY_BUF_SIZE],		// 29. 　　　　　　　　〃　　(4%分)　
						F529C[MONY_BUF_SIZE];		// 29. 　　　　　　　　〃　　(6.3%分)　
	char				F530T[MONY_BUF_SIZE],		// 30. 特例適用･合計 仕入税額(期間計)
						F530A[MONY_BUF_SIZE],		// 30. 　　　　　　　　〃　　(3%分)　　　
						F530B[MONY_BUF_SIZE],		// 30. 　　　　　　　　〃　　(4%分)　
						F530C[MONY_BUF_SIZE];		// 30. 　　　　　　　　〃　　(6.3%分)　
	char				F531T[MONY_BUF_SIZE],		// 31. 特例適用･合計 仕入税額(期間計)
						F531A[MONY_BUF_SIZE],		// 31. 　　　　　　　　〃　　(3%分)　　　
						F531B[MONY_BUF_SIZE],		// 31. 　　　　　　　　〃　　(4%分)　
						F531C[MONY_BUF_SIZE];		// 31. 　　　　　　　　〃　　(6.3%分)　
	char				F532T[MONY_BUF_SIZE],		// 32. 特例適用･合計 仕入税額(期間計)
						F532A[MONY_BUF_SIZE],		// 32. 　　　　　　　　〃　　(3%分)　　　
						F532B[MONY_BUF_SIZE],		// 32. 　　　　　　　　〃　　(4%分)　
						F532C[MONY_BUF_SIZE];		// 32. 　　　　　　　　〃　　(6.3%分)　
	char				F533T[MONY_BUF_SIZE],		// 33. 特例適用･合計 仕入税額(期間計)
						F533A[MONY_BUF_SIZE],		// 33. 　　　　　　　　〃　　(3%分)　　　
						F533B[MONY_BUF_SIZE],		// 33. 　　　　　　　　〃　　(4%分)　
						F533C[MONY_BUF_SIZE];		// 33. 　　　　　　　　〃　　(6.3%分)　
	char				F534T[MONY_BUF_SIZE],		// 34. 特例適用･合計 仕入税額(期間計)
						F534A[MONY_BUF_SIZE],		// 34. 　　　　　　　　〃　　(3%分)　　　
						F534B[MONY_BUF_SIZE],		// 34. 　　　　　　　　〃　　(4%分)　
						F534C[MONY_BUF_SIZE];		// 34. 　　　　　　　　〃　　(6.3%分)　
	char				F535T[MONY_BUF_SIZE],		// 35. 特例適用･合計 仕入税額(期間計)
						F535A[MONY_BUF_SIZE],		// 35. 　　　　　　　　〃　　(3%分)　　　
						F535B[MONY_BUF_SIZE],		// 35. 　　　　　　　　〃　　(4%分)　
						F535C[MONY_BUF_SIZE];		// 35. 　　　　　　　　〃　　(6.3%分)　
	char				F536T[MONY_BUF_SIZE],		// 36. 特例適用･合計 仕入税額(期間計)
						F536A[MONY_BUF_SIZE],		// 36. 　　　　　　　　〃　　(3%分)　　　
						F536B[MONY_BUF_SIZE],		// 36. 　　　　　　　　〃　　(4%分)　
						F536C[MONY_BUF_SIZE];		// 36. 　　　　　　　　〃　　(6.3%分)

	char				F537T[MONY_BUF_SIZE],		// 37. 特例適用･合計 仕入税額(期間計)
						F537A[MONY_BUF_SIZE],		// 37. 　　　　　　　　〃　　(3%分)　　　
						F537B[MONY_BUF_SIZE],		// 37. 　　　　　　　　〃　　(4%分)　
						F537C[MONY_BUF_SIZE];		// 37. 　　　　　　　　〃　　(6.3%分)　

	//== 帳票データ本体 ==
	CH27SnFhyo50Data	**m_pSnFhyo50Data;			// 付表５
	CH27SnFhyo10Data	**m_pSnFhyo40Data;			// 付表４
	CH27SnHonpyoData	**m_pSnHonpyoData;			// 本表
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報

	CDBNpSub			*m_pZmSub;					// 財務クラス
	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_typeCnt;					// 業種数
	int					m_FrSw;						// ???
	int					m_DpSw;						// ???
	int					m_Tmp;						// ???


	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Fhyo50Diag01;
	CIcsdiagctrl m_Fhyo50Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl2(short index);
	void EditOFFIcsdiagctrl2(short index);
	void TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data);
};
