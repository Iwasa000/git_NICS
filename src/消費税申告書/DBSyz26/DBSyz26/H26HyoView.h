#pragma once

#ifdef _SYZ_H26_HYO_EXPORT_CHANGE_
	#define SYZ_H26_HYO_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZ_H26_HYO_CLASS_EXPORT 
#endif

#include "BaseHyoView.h"

#define		WM_USER_UNLOCK			(WM_APP+100)
#define		WM_USER_RECALQ			(WM_APP+101)
#define		WM_USER_READ_ZEIRISHI	(WM_APP+102)
#define		WM_USER_UPDATE_INDICATE	(WM_APP+103)
#define		WM_USER_SETFOCUS		(WM_APP+104)
#define		WM_USER_SYZINIT			(WM_APP+105)
#define		WM_USER_TENKI			(WM_APP+106)
#define		WM_USER_DISP_SPECPASS	(WM_APP+107)
#define		WM_USER_SHINTAB_INIT	(WM_APP+108)
#define		WM_USER_SPCPOST_RECALQ	(WM_APP+109)
#define		WM_USER_SKJPOST_RECALQ	(WM_APP+111)
#define		WM_USER_SET_ENDSTART	(WM_APP+112)	// '15.07.31
#define		WM_USER_RECALQ_TKKZ		(WM_APP+113)	// '15.08.21
#define		WM_USER_RESET_TKKZ		(WM_APP+114)	// '15.09.09
#define		WM_USER_REFOCUS_PRINT	(WM_APP+115)	// '16.01.07
#define		WM_USER_CHG_LOCK_DISP	(WM_APP+116)	// '16.07.22

#define		WM_MYMESSAGE			(WM_APP+110)

//2018.03.13 INSERT START
//typedef struct _ARITH_VAL{
//	unsigned char	val[MONY_BUF_SIZE];
//}ARITH_VAL;
//2018.03.13 INSERT END

extern "C" SYZ_H26_HYO_CLASS_EXPORT HINSTANCE WINAPI GetH26HyoInstance();

// CH26HyoView フォーム ビュー

class CShinIppan;
class CShinKani;
class CShinKaniEX;
class CShinFhyo40;
class CShinFhyo40EX;
class CShinFhyo52;
class CShinFhyo52EX;
class CShinFhyo522;
class CShinFhyo522EX;
class CShinFhyo50;
class CShinFhyo50EX;
//2018.03.13 INSERT START
class CShinFhyo60;
//2018.03.13 INSERT END
class CShinFhyo10;
class CShinFhyo22;
class CShinFhyo20;
//2015.07.02 INSERT START(DEBUG)
class CShinFhyo10EX;
class CShinFhyo22EX;
class CShinFhyo20EX;
//2015.07.02 INSERT END(DEBUG)
class CShinRefundKjn01;
class CShinRefundKjn02;
class CShinRefundHjn01;
class CShinRefundHjn02;
class CShinHjnKsk;
class CShinKjnKsk;
class CShinChsk;
class CH26SnHonpyoData;
class CH26SnFhyo20Data;
class CH26SnTanaData;
class CH26SnFhyo50Data;
class CH26SnFhyo10Data;
class CH26SnRefundData;
class CH26SnChskData;
class CH26SnKskData;
//2015.08.04 INSERT START
class CH28SnFhyo10Data;
class CH28SnFhyo20Data;
//2015.08.04 INSERT END
//----->特定課税仕入対応yoshida150713
class CShinKaniEX2;
class CShinIppanEX;
class CShinChskEX;
class CShinHjnKskEX;
class CShinKjnKskEX;
class CShinHjnKskEX;
class CShinKjnKskEX;
class CTKUtiwake;
//<------------------------------
//--> '15.08.19 INSERT START
class CH28SnHopyoData;
//<-- '15.08.19 INSERT END
//2018.03.13 INSERT START
class CH28SnFhyo60Data;
//2018.03.13 INSERT END


class SYZ_H26_HYO_CLASS_EXPORT CH26HyoView : public CBaseHyoView
{
	DECLARE_DYNCREATE(CH26HyoView)

protected:
	CH26HyoView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CH26HyoView();

public:
//#ifdef _SYZ_H26_HYO_EXPORT_CHANGE_
//	enum { IDD = IDD_H26HYO_VIEW };
//#else
	enum { IDD = 101 };
//#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

// 操作
public:
	// エラーメッセージの取得
	CString& GetErrMsg(){ return m_ErrMsg; };
	// 初期化
	int InitView( CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei, CSyzSyukei *pSubSyzSyukei, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl , BOOL SixKindFlg = FALSE );//[Y]150107
//	int InitView( CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei, CSyzSyukei *pSubSyzSyukei, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl );
	// 内部タブの初期化
	int InitTab( BOOL bDelete );
	// 内部タブの初期化(実体)
	int InitTabSub( BOOL bDelete );
	// 履歴シーケンスセット
	void SetSnSeq( int snseq );
	// 表示帳票の最終データ取得
	void GetDispHyoData();
	// 終了処理
	int EndJob( int wflg = 1 );
	// データ書込み
	int WriteData( BOOL isEnd, BOOL isGetData );
	// 部分データ書込み
	int WriteSepData( int flg );
	// 部分データ読込み
	int ReadSepData( int flg );
	// 全画面：画面表示 再初期化
	//------>特定課税仕入対応yoshida150713
	int ReInitDisp( BOOL isCompChange, BOOL SixKindFlg=FALSE, EnumIdIcsShRevType shrevtype=ID_ICSSH_NO_REVTYPE );	// 引数追加	shrevtype：改正様式
//2016.10.20 INSERT START
	//本表・26号様式　上部のみ再表示
	void ReDispHonpyoUpper();
//2016.10.20 INSERT END
	//--------------------------------
//	int ReInitDisp( BOOL isCompChange , BOOL SixKindFlg = FALSE );//引数追加	SixKindFlg = TRUE・・第六事業種あり	[Y]150106
	//<-------------------------------
//	int ReInitDisp( BOOL isCompChange );
	// 現在の入力位置にフォーカスセット
	void SetNowPosition();
	// 過年度プロ呼出し後にデータを吸い上げる
	int GetPastData();
	// 画面表示フラグを切り換える
	void SetDispedFlg( BOOL bShow ){ m_bShowed = bShow; };
	// 履歴シーケンス切替
	int ChangeSnSeq( int snseq );
	// 現申告データを消去して、データの再読込み
	int ReReadData();
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
	// 内部タブデータの再取得
	int ReGetData();
	// 次回申告データ作成に必要になるデータの読込み
	int ReadDataForNextShinkoku();
	// ページ切替
	void PageChange(int move);
	// 初期化が必要な年度？('14.05.13)
	BOOL IsNeedInitNendo();
	// データ読み込み（金額集計用）	('15.03.17)
	int ReadDataForCalq();

	// 印刷情報のセット
	int SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar, SH_PRINT_CMINFO *pPrintCmInfo );
	// 印刷
	int PrintSyzHyo();
	// 印刷指定ダイアログの表示
	int DispPrnDlg();
	// 本表印刷
	int PrintHonPyo( CDC *pDC, BOOL isHikae, int PPno , int fno );
	// 付表１・４出力
	int PrintFuhyo10( CDC *pDC, int kojin, int pcnt );
//2015.08.04 INSERT START
	// 付表１・４出力
	int PrintFuhyo10Ex( CDC *pDC, int kojin, int pcnt );
//2015.08.04 INSERT END
	// 付表２出力
	int PrintFuhyo20( CDC *pDC, int kojin, int pcnt );
//2015.08.04 INSERT START
	// 付表２出力
	int PrintFuhyo20Ex( CDC *pDC, int kojin, int pcnt );
//2015.08.04 INSERT END
	// 付表５出力
	int PrintFuhyo50( CDC *pDC, int kojin, int pcnt );
	// 付表２－２出力
	int PrintFuhyo22( CDC *pDC, int kojin, int pcnt );
//2015.08.04 INSERT START
	// 付表２－２出力
	int PrintFuhyo22Ex( CDC *pDC, int kojin, int pcnt );
//2015.08.04 INSERT END
	// 付表５－２出力
	int PrintFuhyo52( CDC *pDC, int kojin, int pcnt );
	// 付表５－２＿１出力
	int PrintFuhyo52_1( CDC *pDC, int kojin, int pcnt );
	// 付表５－２＿２出力
	int PrintFuhyo52_2( CDC *pDC, int kojin, int pcnt );
	// 還付付表（法人）
	void PrintKojinRefund( CDC* pDC,int pno );
	// 還付付表（個人）
	void PrintHojinRefund( CDC* pDC,int pno );
//2015.08.04 INSERT START
//	int PrintUchiwake( CDC *pDC, int pcnt );
	int PrintUchiwake( CDC *pDC, BOOL isHikae, int PPno , int pcnt );
//2015.08.04 INSERT END
	// 前年実績による中間申告
	int data_mast( CDC* pDC );
	// 前年実績による中間申告２
	int data_mast_EX( CDC* pDC );

	//---------->[Y]150120
	//簡易課税第六種対応
	int PrintFuhyo50_EX( CDC *pDC, int kojin, int pcnt );// 第六種対応 付表５出力
	int PrintFuhyo52_EX( CDC *pDC, int kojin, int pcnt );// 第六種対応 付表５－２出力
	int PrintFuhyo52_1_EX( CDC *pDC, int kojin, int pcnt );// 第六種対応 付表５－２＿１出力
	int PrintFuhyo52_2_EX( CDC *pDC, int kojin, int pcnt );// 第六種対応 付表５－２＿２出力
	int PrintHonPyo_EX( CDC *pDC, BOOL isHikae, int PPno , int fno );// 第六種対応 本表印刷
	//<-----------------------

//2018/03/13 INSERT START
	int PrintFuhyo60( CDC *pDC, int pcnt );	//付表６
	int CalqFuhyo60( ARITH_VAL *F60val );
	int GetMxListFhyo60();
//2018/03/13 INSERT END

	// 更正の請求　法人用 
	int data_hjn( CDC* pDC );
	// 印刷関数
	int hjn_dprn1( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int hjn_dprn2( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int hjn_dprn3( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int hjn_dprn4( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int hjn_dprn5( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int hjn_dprn6( int dp, CDC* pDC, char *BP, BOOL isNoImage );

	// 更正の請求　個人用 
	int data_kjn( CDC* pDC );
	// 印刷関数
	int kjn_dprn1( int dp, CDC* pDC, char *BP, BOOL	isNoImage );
	// 印刷関数
	int kjn_dprn2( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int kjn_dprn3( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int kjn_dprn4( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int kjn_dprn5( int dp, CDC* pDC, char *BP, BOOL isNoImage );
	// 印刷関数
	int kjn_dprn6( int dp, CDC* pDC, char *BP, BOOL isNoImage );

	// 前年実績による中間申告(実部１)
	int _dprn1( int dp, CDC* pDC, char *BP );
	// 前年実績による中間申告(実部２)
	int _dprn2( int dp, CDC* pDC, char *BP );
	// 前年実績による中間申告(実部３)
	int _dprn3( int dp, CDC* pDC, char *BP );
	// 前年実績による中間申告(実部４)
	int _dprn4( int dp, CDC* pDC, char *BP );
	// 切取線
	void Lin_Print();
	// 前年実績による中間申告２(実部１)
	int _dprn1_EX( int dp, CDC* pDC, char *BP );
	// 前年実績による中間申告２(実部２)
	int _dprn2_EX( int dp, CDC* pDC, char *BP );
	// 前年実績による中間申告２(実部３)
	int _dprn3_EX( int dp, CDC* pDC, char *BP );
	// 前年実績による中間申告２(実部４)
	int _dprn4_EX( int dp, CDC* pDC, char *BP );

	// 金額取得
	int GetMony( CString &tag, CString &chtag, char *pArith );
	// 金額セット
	int SetMony( CString &tag, CString &chtag, char *pArith );
	// 金額の再取得
	int ReGetCalqedMony();
	// 仕入控除税額の財務連動
//-- '15.09.30 --
//	int ConnectedSkj();
//---------------
	int ConnectedSkj( MoneyBasejagArray &MoneyBaseArray, bool isUpdDisp );
//---------------
//2016.02.23 INSERT START
	// 仕入金額等の明細のクリア
	int ClearShiire();
//2016.02.23 INSERT END
	// 更正の請求用　申告書金額計算
	int CalqSyzForKsk( int mode );
	// タブタイトルの切替
	void ChengTabTitle( char type );
	// 中間及び期間短縮の更新
	int RenewSyzShin( SH_PASTINFO *pPastInfo );
	// 本表の計算(1)～(26)迄
	void AlreadyDecisionCalc( int type );
	// 本表の計算(1)～(26)迄(H26年版)
	void AlreadyDecisionCalcH26();
	// 本表の計算(1)～(26)迄(H25年版)
	void AlreadyDecisionCalcH25();
	// 棚卸調整額の連動
	int CalqStatckSyz();
	// 前年の消費税額取得
	int GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith );
	// 集計済み金額の取得('14.06.30)
	int GetCalqedMony();
	// 既確定税額のクリア('15.01.28)
	int ClearAlreadyDecision();

	// 本表から他表への計算
	void CalqFromKanHonpyo();
	// 付表5-2(2)の表示呼出
	void DispFhyo522();
	// 本表から他表への計算 ('14.05.20)
	void CalqFromIpaHonpyo();

	// 各表の更新処理
	void UpdateSheetFromOuter();
	// 全表の更新処理
//2017.03.01 UPDATE START
//	void UpdateAllSheet();
	void UpdateAllSheet( int sgn = 0 );
//2017.03.01 UPDATE END

	//[Y] 選択表より前までの表の更新処理
	void UpdateUntilSheet();

	void TelBufSet( char *buf, int mode );
	// 予定26号の前課税期間のセット
//	void ZenkiDate_Chsk(char* ZenkSymd , char* ZenkEymd );
	void ZenkiDate_Chsk(char* ZenkSymd , char* ZenkEymd, int ZenkSg, int ZenkEg );
	// 表示中頁の更新処理後の再表示
	void IndicateSheet();
	// タブへフォーカスセット
	void EndJobFocus();

	// 入力ロック呼び出し'14.10.08
	void InputRock(int mode);

	// 簡易課税で横計ではない例外計算が必要？	('14.10.17)
	BOOL IsNeedExpCalq();

	// 本表のフォーカス位置を顧問先情報と関係の無い位置にずらす	('15.01.14)
	void ShiftHonpyoFocus();
	// 税務署入力にフォーカス誘導('15.02.03)
	int GuideTaxOfficeInput();
	// 特定収入計算表ダイアログ表示('15.02.16)
	int SpecPassDlg( int type );
	// 簡易課税第六種フラグをセット('15.02.20)
	void SetSixKindFlg( BOOL SixKindFlg ){ m_SixKindFlg = SixKindFlg; };
	// 還付申告？('15.02.24)
	BOOL IsKanpuShinkoku();
//2016/06/22 INSERT START
	// 還付申告？
	BOOL IsKanpuShinkoku2();
//2016/06/22 INSERT END
	// 出力用税理士名の作成('15.02.27)
	void MakeOutZeirishiName( char *pOutBuf ,int len );
	// 入力位置を保存 ('15.03.02)
	void SaveInputPosition();
	// 入力位置を復元 ('15.03.02)
	void RestoreInputPosition();
	// 終了時のフォーカス移動 ('15.03.02)
	void MoveFocusInEnd();
	// 特定収入計算表 金額連動？ ('15.03.02)
	BOOL IsSpcDataLinked();
	// 控除対象仕入税額の再計算('15.04.03)
	int RecalqSkj( SYZTOSPC_DATA *pSyzToSpc );
	// 還付金融機関入力にフォーカス誘導('16.12.26)
	int GuideKanpuKinyu( int dsInput );
	// 課税取引金額計算書から転記時のフォーカス移動('16.12.27)
	int MoveFocusInTaxTenki();

	//------>特定課税仕入対応yoshida150713
	// 特定収入フラグをセット
//	void SetTokuteiFlg( BOOL TokuteiFlg ){ m_TokuteiFlg = TokuteiFlg; };
	//<-------------------------------

	// 改正様式をセットする ('15.08.27)
	void SetShRevType( EnumIdIcsShRevType shrevtype );
	// 特定課税仕入可能？('15.09.09)
	int IsAbleTkKz();
	// 特定課税仕入を『有』から『無し』へ('15.09.09)
	int ChangeTkkzUse( int srctab, int calltype );
	// 特定課税仕入と課税売上割合のチェック('15.09.10)
	int ChkTkkzAndKzwari( int srctab, int calltype );
	// 課税売上割合が95%以上？('15.09.10)
	BOOL IsRatio95Over();
	// 正常な特定課税仕入(前期以前の返還)かを確認('15.10.06)
	int ChkZkTkkzReturn( int srctab, int calltype );

	// 消費税改正様式の取得('15.12.07)
	EnumIdIcsShRevType	GetShRevType(){ return m_ShRevType; };
	// 送付不要をカットする様式？('18.12.18)
	bool IsNoDeliForm();

//2016.06.22 INSERT START
	int Message_BlankKinyu_Kanpu();
	int Message_BlankKinyu_Kousei();
//2016.06.22 INSERT END
//2018.03.13 INSERT START
	//納付還付税額の合計と合計額が一致するか
	int Message_NofuKanpuGokei();

	int Message_MyNumber_error();

	int Message_MyNumber_overlap();

	int Message_SozokuWariai();

	void EditOff_Fuhyo6();

	int Getindex_toNewLineCode( int start, int *end, CString str );
//2018.03.13 INSERT END
//2018.08.21 INSERT START
	int IsExist_MyNumber_Fuhyo6();

	// マイナンバー履歴を書き込む
	void WriteNoHistory(int write_sgn);

	// マイナンバー閲覧サインのセット
	void SetMyNumBrowseSgn(BOOL NoHistory_browse){m_NoHistory_browse = NoHistory_browse;}
//2018.08.21 INSERT END

	// 予定申告の月数クリア('17.02.09)
	int ClearYoteiMonth();
	// 入力ロックコントロールの再描画	('17.02.27)
	void RedrawInputLock();

	// 終了処理フラグの切り替え('18.06.22)
	void SetEndStartFlg( BOOL flg );
	// 終了処理中？
	BOOL IsEndStart(){ return m_isEndStart; };

	// 平成令和元号選択？
	bool IsSwitch_HeiseiReiwa( bool IsJudgeKojin = true );
	// 令和プレプリント？('19.06.12)
	bool IsReiwaPrnt();
	// 元号の○印 未出力？
	bool IsNotPrintGengoCircle();
	// データの引き継ぎ
	int TakeOverData(int srcVer, EnumIdIcsShRevType shRevType, BOOL sixKindFlg, int snSeq, CDBNpSub *pZmsub);

	// 初期化完了フラグの切り替え
	void SetInitedFlg( bool flg );
	// View の初期化完了？
	bool IsInitedView(){ return m_isInited; };

	// 更正の請求書の提出年月日　元号番号取得
	int	 GetGengoNo_Ksk();

protected:
	// 各表の更新処理
//2017.03.01 UPDATE START
//	void UpdateSheet( int selno);
	void UpdateSheet( int selno, int get_sgn = 0 ); //get_sgn:　現在タブのデータを取得　0…する 1…しない
//2017.03.01 UPDATE END
	// 各表の更新処理後の再表示
	void IndicateSheet( int selno );
	// 初期入力位置にフォーカスセット
	void SetItemPosition( int selno );
	// DIAGの属性変更
	void ChangeDiagAttr();
	// タブ内のダイアログを全消去
	void DeleteInsideDlg();
	// 旧データ読込み必要性のチェック
	BOOL IsNeedOldDataCnv();
	// 申告書用データ作成
	int MakeSnkData();
	// 還付申告用データ再設定 ('14.05.17)
	void ResetRefundData();

	// 更正の請求用　申告書金額計算（実部）
	int CalqSyzForKskSub( void *pCalqedMony1, void *pCalqedMony2, char *Zei );
	// 更正の請求用　申告書金額計算（実部：平成25年版）
	int CalqSyzForKskSub_H25( void *pCalqedMony1, void *pCalqedMony2, char *Zei );

	// 金額をセット（中間申告用）
	int val_set( char *bf, unsigned char *val );
	// 電話番号をセット（中間申告用）
	void ZTel_Set( char *buf, char *telno, char sw, int len, int lng );

	// 法人業種で法人番号未出力？
	bool IsNotPrintHojinNo();

	// 平成継続出力？
	bool IsJcContinuePrint( bool IsJudgeKjn );
	// 令和プレプリント情報の初期化('19.06.12)
	void InitReiwaPrnt();

	bool CheckInvno( int sgn );

// 属性
private:
	CString				m_ErrMsg;			// エラーメッセージ
	CDBNpSub			*m_pZmSub;			// 公益クラス
	SYC_SHININFO		*m_pShinInfo;		// 共通情報
	CArithEx			m_Arith;			// 倍長演算
	int					m_SnSeq;			// 履歴シーケンス
	int					m_SvSnSeq;			// 履歴シーケンス（前回保存）
	CSyzUtil			m_Util;				// ユーティリティクラス
	CSyzSyukei			*m_pSyzSyukei;		// 消費税集計クラス
	CSyzSyukei			*m_pSubSyzSyukei;	// 消費税集計クラス（サブ集計）
	CTblhdl				*m_pTblhdl;			// テーブルハンドリング
	BOOL				m_bShowed;			// 画面表示フラグ
	BOOL				m_bNeedFocusSet;	// フォーカスセットが必要？
	BOOL				m_isEndStart;		// 終了処理開始？('18.06.22)

	BOOL				m_SixKindFlg;		//m_SixKindFlg = TRUE・・第六事業種あり	[Y]150106

	//------>特定課税仕入対応yoshida150713
//	BOOL				m_TokuteiFlg;		// m_TokuteiFlg = TRUE・・特定収入チェックボックス表示（期間で判定）　FALSE・・非表示
	//<-------------------------------

	EnumIdIcsShRevType	m_ShRevType;		// 消費税改正様式('15.08.27)

//2016.02.23 INSERT START
	BOOL				m_Syokai_NukiKomi;	// 税抜・税込を取得するかしないか
//2016.02.23 INSERT END

//2018.08.21 INSERT START
	BOOL				m_NoHistory_browse;	// マイナンバー履歴フラグ D0:画面表示　D1:プレビュー　D2:印刷
//2018.08.21 INSERT END

	//== 外部情報 ==
	ESELECT				*m_pEselect;		// 電子申告用　税務署コード選択モジュール
	BOOL				*m_pIsOpenEselect;	// 税務署コードモジュールをオープンした？
	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報
	_TB_PAR				*m_pSkjCnvtbl;		// 仕入控除用テーブル

	// 印刷情報
	CVPrint				*m_pVprn;			// 出力クラス
	NPRT_PAR			*m_pPset;			// 印刷情報
	SH_PRINT_CMINFO		*m_pPrintCmInfo;	// 印刷共有情報

	// 内部タブ
	CArray<SH_TABMNG>	m_TabMng;			// タブ管理
	ICSDialog			*m_pFirstPage;		// 初期ページ保存
	CShinIppan			*m_pIppan;			// 一般本表
	//----->特定課税仕入対応yoshida150713
	CShinIppanEX		*m_pIppanEX;		// 一般本表(特定課税仕入用)
	//<------------------------------
	CShinKani			*m_pKani;			// 簡易本表
	CShinKaniEX			*m_pKaniEX;			// 簡易本表(第六種用)
	//----->特定課税仕入対応yoshida150805
	CShinKaniEX2		*m_pKaniEX2;		// 簡易本表(特定課税仕入用)
	//<------------------------------
	CShinFhyo40			*m_pFhyo40;			// 付表４
	CShinFhyo40EX		*m_pFhyo40EX;		// 付表４(第六種用)
	CShinFhyo52			*m_pFhyo52;			// 付表５－２（１）
	CShinFhyo52EX		*m_pFhyo52EX;		// 付表５－２（１）(第六種用)
	CShinFhyo522		*m_pFhyo522;		// 付表５－２（２）
	CShinFhyo522EX		*m_pFhyo522EX;		// 付表５－２（２）(第六種用)
	CShinFhyo50			*m_pFhyo50;			// 付表５
	CShinFhyo50EX		*m_pFhyo50EX;		// 付表５(第六種用)
	CShinFhyo10			*m_pFhyo10;			// 付表１
	CShinFhyo22			*m_pFhyo22;			// 付表２－２
	CShinFhyo20			*m_pFhyo20;			// 付表２
//2015.07.02 INSERT START(DEBUG)
	CShinFhyo10EX		*m_pFhyo10EX;			// 付表１
	CShinFhyo22EX		*m_pFhyo22EX;			// 付表２－２
	CShinFhyo20EX		*m_pFhyo20EX;			// 付表２
//2015.07.02 INSERT END(DEBUG)
//2018.03.13 INSERT START
	CShinFhyo60			*m_pFhyo60;			// 付表６
//2018.03.13 INSERT END

	//----->特定課税仕入対応yoshida150713
	CTKUtiwake			*m_pTKUtiwake;	// 特定課税仕入新表データ
	//<------------------------------

	int					m_nowTabIdx;		// 現表示タブNo.

	CShinRefundKjn01	*m_pRefundKjn01;	// 還付付表１（個人）
	CShinRefundKjn02	*m_pRefundKjn02;	// 還付付表２（個人）
	CShinRefundHjn01	*m_pRefundHjn01;	// 還付付表１（法人）
	CShinRefundHjn02	*m_pRefundHjn02;	// 還付付表２（法人）

	CShinHjnKsk			*m_pHjnKsk;			// 更正の請求（法人）
	CShinKjnKsk			*m_pKjnKsk;			// 更正の請求（個人）

	CShinChsk			*m_pChsk;			// 予定申告（２６号）
	//----->特定課税仕入対応yoshida150805
	CShinChskEX			*m_pChskEX;			// 予定申告（２６号）
	CShinHjnKskEX		*m_pHjnKskEX;		// 更正の請求（法人）
	CShinKjnKskEX		*m_pKjnKskEX;		// 更正の請求（個人）
	//<----------------------------------

	CRect				vwRect;				// ウィンドウレクト
	//== 各帳票データ ==
	CH26SnHonpyoData	*m_pSnHonpyoData;	// 一般本表データ
	CH26SnFhyo10Data	*m_pSnFhyo10Data;	// 付表１データ
	CH26SnFhyo20Data	*m_pSnFhyo20Data;	// 付表２データ
	CH26SnFhyo20Data	*m_pSnFhyo22Data;	// 付表２－２データ
	CH26SnTanaData		*m_pSnTanaData;		// 棚卸調整データ
	CH26SnFhyo10Data	*m_pSnFhyo40Data;	// 付表４データ
	CH26SnFhyo50Data	*m_pSnFhyo50Data;	// 付表５データ
	CH26SnFhyo50Data	*m_pSnFhyo52Data;	// 付表５－２データ
//2018.03.13 INSERT START
	CH28SnFhyo60Data	*m_pSnFhyo60Data;	// 付表５－２データ
//2018.03.13 INSERT END
	CH26SnRefundData	*m_pSnRefundData;	// 還付申告データ
	CH26SnChskData		*m_pSnChskData;		// 中間予定申告データ
	CH26SnKskData		*m_pSnKskData;		// 更正の請求データ

	int					m_JcContinuePrint;	// 和暦継続出力？
	bool				m_isReiwaPrnt;		// 令和プレプリント

	bool				m_isInited;			// 初期化完了？
											// InitView() 関数がコールされて、変数等が初期化されたかどうかをキーとします

protected:
	virtual void DoDataExchange(CDataExchange* pDX);		// DDX/DDV サポート
	LRESULT OnUserRecalqTkkz(WPARAM wParam, LPARAM lParam);		// '15.08.21

	DECLARE_MESSAGE_MAP()
public:
	ICSTabCtrl	m_MainTab;
	virtual void OnInitialUpdate();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnTcnSelchangingMainTab(NMHDR * pNotifyStruct, LRESULT * result);
	void OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnButtonF2();
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	afx_msg void OnButtonF9();
	afx_msg void OnButtonF7();
	afx_msg void OnButtonF8();
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
	afx_msg void OnButtonF6();
	afx_msg void OnUpdateButtonF6(CCmdUI *pCmdUI);
	afx_msg void OnZeirishiInf();
	afx_msg void OnUpdateZeirishiInf(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonF11();
	afx_msg void OnUpdateButtonF11(CCmdUI *pCmdUI);
	afx_msg void OnUpdateKomonsakiInf(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
	afx_msg void OnButtonF12();
	afx_msg void OnUpdateButtonF12(CCmdUI *pCmdUI);

	LRESULT OnMyMessage( WPARAM wParam, LPARAM lParam );
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


