/*****************************************************************
	OwntbData.h
		試算表アプリで使用するowntbデータクラス

	【注意】
	　・zvol,owntbのOpenを実行してから使用して下さい
	　・会社切替が発生した場合、delete→newを実施して下さい

	2010.01.XX 社会福祉　
	　・資金収支計算書で貸借発生を出力する
*****************************************************************/
#pragma once
#include "BatchPrt.h"

//----------------------------------------------------------------
//	owntbテーブル参照情報
//----------------------------------------------------------------
#define	OWNTB_APLNAME	"DBUchiwake"			// apl_name

#define TG_COMMON_USER_ID	0					// ユーザー情報共通化 ユーザーID	2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化


typedef enum {
	OWNTB_SEQ_BPRTCMN	= 1000,				// itm_seq = 1000：一括印刷用 情報	2010/07/XX 一括印刷対応
	OWNTB_SEQ_BPRTCMN1	= 1001,				// itm_seq = 1001：一括印刷用 情報	2010/07/XX 一括印刷対応
	OWNTB_SEQ_BPRTCMN2	= 1002,				// itm_seq = 1002：一括印刷用 情報	2010/07/XX 一括印刷対応
	OWNTB_SEQ_BPRTCMN3	= 1003,				// itm_seq = 1003：一括印刷用 情報	2010/07/XX 一括印刷対応
	OWNTB_SEQ_BPRTCMN4	= 1004,				// itm_seq = 1004：一括印刷用 情報	2010/07/XX 一括印刷対応
	OWNTB_SEQ_BPRTCMN5  = 1005,				// itm_seq = 1005：一括印刷用 情報　2014/06/XX その他の内訳書１－⑥～１－⑳対応From
	OWNTB_SEQ_BPRTCMN6  = 1006,				// itm_seq = 1006：一括印刷用 情報　2014/06/XX その他の内訳書１－⑥～１－⑳対応To
	OWNTB_SEQ_BPRTCMN7  = 1007,				// itm_seq = 1007：一括印刷用 情報　2014/06/XX その他の内訳書２－⑥～２－⑳対応From
// midori 190505 del -->
	//OWNTB_SEQ_BPRTCMN8  = 1008				// itm_seq = 1008：一括印刷用 情報　2014/06/XX その他の内訳書２－⑥～２－⑳対応To
// midori 190505 del <--
// midori 190505 add -->
	OWNTB_SEQ_BPRTCMN8  = 1008,				// itm_seq = 1008：一括印刷用 情報　2014/06/XX その他の内訳書２－⑥～２－⑳対応To
	OWNTB_SEQ_BPRTCMN9  = 1009,				// itm_seq = 1009：一括印刷用 情報
	OWNTB_SEQ_BPRTCMN10 = 1010,				// itm_seq = 1009：一括印刷用 情報
	OWNTB_SEQ_BPRTCMN11 = 1011,				// itm_seq = 1009：一括印刷用 情報
	OWNTB_SEQ_BPRTCMN12 = 1012				// itm_seq = 1009：一括印刷用 情報
// midori 190505 add <--

} EnumIdOwntbSeq;

//--------------------------------------------------
// レコードの有無をチェック結果
// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
//--------------------------------------------------
typedef enum tag_ENUM_OWNTB_CHKREC_RET {
	OWNTB_CHKREC_OK_EXIST		= 0,			// レコード有り
	OWNTB_CHKREC_NG_DOSET01,					// 共通ユーザID→無し, ログインユーザID→有り, ログインユーザ以外ID→有り
	OWNTB_CHKREC_NG_DOSET02,					// 共通ユーザID→無し, ログインユーザID→無し, ログインユーザ以外ID→無し
	OWNTB_CHKREC_NG_DOSET03,					// 共通ユーザID→無し, ログインユーザID→無し, ログインユーザ以外ID→有り
	OWNTB_CHKREC_DUMMY							// （メンバ追加時の“,”付け忘れ予防）
} ENUM_OWNTB_CHKREC_RET;

// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
#define	SQLFILTER_BPRT_USRIDCHK		_T("(apl_name='%s') AND (itm_name='%s') AND (itm_seq=%d) AND (u_id<>%d) AND (u_id<>%d)")

//----------------------------------------------------------------
//	データ構造
//----------------------------------------------------------------
// 2010/07/XX 一括印刷対応
// 1000.一括印刷情報
typedef struct tag_UCWK_INFO_PRTCMN {
	int			nCoverSw;					// 表紙を出力する
	int			nKsnAddNow;					// 決算期間に「現在」を出力する
// midori 160608 add -->
	int			nZeroSw;					// 帳票イメージのみ出力する
// midori 160608 add <--
} UCWK_INFO_PRTCMN;

// 1001～1004.一括印刷情報
typedef struct tag_UCWK_INFO_BPRTCMN {
	int			nOutPut[UC_ID_FORMNO_MAX];				// 各帳表の出力ON/OFF
	int			nOPRange_From[UC_ID_FORMNO_MAX];		// 各帳表の出力ページ範囲（From）
	int			nOPRange_To[UC_ID_FORMNO_MAX];			// 各帳表の出力ページ範囲（To）
// midori 190505 add -->
	int			nOPPrnHoho[UC_ID_FORMNO_MAX];			// 各帳表の出力方法
// midori 190505 add <--
} UCWK_INFO_BPRTCMN;

// 画面選択情報（全て）
typedef struct tag_BPMK_INFO_ALL {
	UCWK_INFO_PRTCMN	uPrtCmn;			// 1000：一括印刷情報
	UCWK_INFO_BPRTCMN	uBPrtCmn;			// 1001～1004：一括印刷情報
} BPMK_INFO_ALL;


//----------------------------------------------------------------
//	データの値
//----------------------------------------------------------------
// 共通：SW定義
#define	SW_OFF			0x00				// OFF
#define	SW_ON			0x01				// ON

// メイン：用紙サイズ
typedef enum {
	ID_PRTSIZE_A4T = 0						// 0:A4縦
} EnumIdInfoPrtSize;

//----------------------------------------------------------------
//	owntbデータクラス
//----------------------------------------------------------------
class COwntbData
{
public:
	COwntbData(CDBZmSub* pZmSub, CBatchPrt* pBatchPrt);				// 2010/07/XX 一括印刷対応
	~COwntbData(void);
protected:
	BOOL	GetUserId(void);
	BOOL	StartCheck(int nItmSeq);
	//CString	GetFilterString(int nItmSeq, CString strItmName ="");	// 2010/07/XX 一括印刷対応
	CString	GetFilterString(int nItmSeq, int nUserId, CString strItmName ="");	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化

	BOOL	CreateNewRecord(int nItmSeq, BOOL fNormal = TRUE);		// 2010/08/02 一括印刷対応（新規レコード作成救済処置対応）
	void	COwntbDataInit(CDBZmSub* pZmSub);						// 2010/07/XX 一括印刷対応
	void	COwntbDataInitBprt( void );								// 2010/08/02 一括印刷対応（新規レコード作成救済処置対応）
public:
	BOOL	LoadAllData(void);
	BOOL	LoadData(int nItmSeq);
	BOOL	SaveAllData(void);
	BOOL	SaveData(int nItmSeq);

	int		CheckRecordBprtUserID( void );							// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	BOOL	ClrOwnRecByUID( CDBZmSub* pZmSub, CString strItmName );	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化

private:
	void	SwapSonotaLoad();
	void	SwapSonotaSave();

protected:
	CDBZmSub*		m_pZmSub;				// DB財務クラスのポインタ
	int				m_nUserId;				// ユーザID（owntb参照用）
	BOOL			m_fInit;				// 初期処理状態（True=成功，FALSE=エラー発生）
	CBatchPrt*		m_pBatchPrt;			// 一括印刷管理クラス	2010/07/XX 一括印刷対応
public:
	BPMK_INFO_ALL	m_uInfo;				// owntbデータ
};
