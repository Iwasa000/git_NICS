#ifndef INCLUDE_BRNTKY_H
#define INCLUDE_BRNTKY_H
// BrnTky.h : ヘッダー ファイル
//	枝番摘要を取得するクラス
//

#include "BrnTkyDef.h"


class CBrnTkySub;

//
// クラス定義
#ifdef _DBINPSUB
class AFX_EXT_CLASS CBrnTky
#else
class CBrnTky
#endif
{
// コンストラクション
public:
	CBrnTky();
	virtual ~CBrnTky();

// オペレーション
public:
	//-----------------------------------------------------------
	//	枝番摘要レコード取得
	//	BRN_REC ......... 枝番摘要レコード
	//	int bmn	......... 部門コード
	//	CString& code ... 科目コード
	//	CString& kana ... 枝番摘要カナ
	//	int page_sz ..... １ページあたりの 表示枝番摘要数
	//	int page ........ ページ数
	//	BOOL full ....... 表示内容( FALSE = 省略表示 )
	//	int mode ........ 枝番・諸口表示モード(BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK)
	//
	//　return TRUE = 該当枝番摘要あり
	//-----------------------------------------------------------
	BOOL get_brtektb( BRN_TB **tbl, int bmn, CString& code, CString& kana, int page_sz = -1, int page = 0, BOOL full = TRUE, int mode = BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK );

	//-----------------------------------------------------------
	//	枝番摘要名称レコード取得
	//	int bmn	......... 部門コード
	//	CString& code ... 科目コード
	//	int brn ......... 枝番コード
	//	BOOL full ....... 表示内容( FALSE = 省略表示 )
	//	int mode ........ 枝番・諸口表示モード(BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK)	
	//
	//　return 該当枝番名称レコードのポインタ(NULLの場合はデータなし)
	//-----------------------------------------------------------
	BRN_NAME *get_brtekmsg( int bmn, CString& code, int brn, BOOL full = TRUE, int mode  = BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK );
	
	//-----------------------------------------------------------
	//	枝番摘要レコード取得
	//	BRN_REC ......... 枝番摘要レコード
	//	int bmn	......... 部門コード
	//	CString& code ... 科目コード
	//	int brn ......... 枝番コード
	//　return TRUE = 該当枝番摘要あり
	//-----------------------------------------------------------
	BOOL get_brtek( BRN_REC *ptkrec, int bmn, CString& code, int brn );
	
	//-----------------------------------------------------------
	//	枝番摘要コード取得(返送値が摘要コード)
	//	int bmn	......... 部門コード
	//	CString& code ... 科目コード
	//	int brn ......... 枝番コード
	//-----------------------------------------------------------
	int	 get_brtek( int bmn, CString& code, int brn );

	int		brntky_iniz( CDBZmSub* pZm );
	void	brntky_close();

	//枝番テーブル開放(同一条件だと枝番テーブルを作成しないので)
	int		brtek_free();
	//科目コードを表示する
	void	brntky_kmcode( BOOL bKmcode );
	//摘要クエリーサインをセット
	void	set_tkquery( BOOL bQuery );
	//枝番カナ順
	void	set_brnKanaJun( BOOL bKanaJun );

private:
	CBrnTkySub* m_pSub;

};

/////////////////////////////////////////////////////////////////////////////

#endif	// End of INCLUDE_BRNTKY_H