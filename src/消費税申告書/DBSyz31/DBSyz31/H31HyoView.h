#pragma once

#ifdef _SYZ_H31_HYO_EXPORT_CHANGE_
	#define SYZ_H31_HYO_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZ_H31_HYO_CLASS_EXPORT 
#endif

#include "BaseHyoView.h"


#define		WM_USER_UNLOCK			(WM_APP+100)
#define		WM_USER_RECALQ			(WM_APP+101)
#define		WM_USER_READ_ZEIRISHI	(WM_APP+102)
#define		WM_USER_UPDATE_INDICATE	(WM_APP+103)
#define		WM_USER_SETFOCUS		(WM_APP+104)
#define		WM_USER_DISP_SPECPASS	(WM_APP+107)
#define		WM_USER_SKJPOST_RECALQ	(WM_APP+111)
#define		WM_USER_RESET_TKKZ		(WM_APP+114)
#define		WM_USER_CHG_LOCK_DISP	(WM_APP+116)
#define		WM_USER_RESET_REFUND	(WM_APP+117)	// 還付申告のデータ再セット
#define		WM_ZMSEL_MESSAGE		( WM_APP + 118 )


#define		WM_USER_SET_ENDSTART	(WM_APP+112)

#define		WM_MYMESSAGE			(WM_APP+110)

#define JIGYO_MAX_TOKUREI_1  0x0F //事業の最大数 (第5-(1)号)
#define JIGYO_MAX_TOKUREI_23 0x0A //事業の最大数 (第5-(2)号,第5-(3)号)

//typedef struct _ARITH_VAL{
//	unsigned char	val[MONY_BUF_SIZE];
//}ARITH_VAL;

typedef struct _PTBLTDATAINFO {
	CString itemId;
	int kind;
	char* data;
	int size;
	int* diagidx;
} PTBLTDATAINFO;

extern "C" SYZ_H31_HYO_CLASS_EXPORT HINSTANCE WINAPI GetH31HyoInstance();

// CH31HyoView フォーム ビュー

class CShinFhyo1_01;
class CShinFhyo1_02;
class CShinFhyo1_03;
class CShinFhyo2_01;
class CShinFhyo2_11;
class CShinFhyo2_11Ex;
class CShinFhyo2_02;
class CShinFhyo2_12;
class CShinFhyo2_12Ex;
class CShinFhyo2_03;
class CShinFhyo2_03Ex;
class CShinFhyo4_01;
class CShinFhyo4_02;
class CShinFhyo4_03;
class CShinFhyo5_01;
class CShinFhyo5_012;
class CShinFhyo5_02;
class CShinFhyo5_022;
class CShinFhyo5_03;
class CShinFhyo5_032;
class CShinFhyo6;
class CShinFhyo6_2wari;	//('23.05.01 Add)
class CShinIppan01;
class CShinIppan01EX;
class CShinIppan01EX2;	//('23.05.01 Add)
class CShinIppan02;
class CShinKani01;
class CShinKani01EX;
class CShinKani01EX2;	//('23.05.01 Add)

class CShinKSisanUri;
class CShinKsisan_Uri2;
class CShinKshiire;		

class CShinChskEX;
class CShinHjnKskEX;
class CShinKjnKskEX;
class CShinHjnKskEX2;
class CShinKjnKskEX2;

class CShinRefundKjn01;
class CShinRefundKjn02;
class CShinRefundKjn02EX;	// 20230704
class CShinRefundHjn01;
class CShinRefundHjn02;
class CShinRefundHjn02EX;	// 20230704
class CH31SnRefundData;
class CH31SnChskData;

class CH31SnHonpyoData;
class CH31SnHonpyo2Data;
class CH31SnFhyo1_01Data;
class CH31SnFhyo1_02Data;
class CH31SnFhyo2_01Data;
class CH31SnFhyo2_02Data;
class CH31SnTanaData;
class CH31SnFhyo4_01Data;
class CH31SnFhyo4_02Data;
class CH31SnFhyo5_01Data;
class CH31SnFhyo5_012Data;
class CH31SnFhyo5_02Data;
class CH31SnFhyo5_022Data;
class CH31SnFhyo6Data;
class CH31SnFhyo6_2wariData; //('23.05.01 Add)

class CH31SnKskData;

class SYZ_H31_HYO_CLASS_EXPORT CH31HyoView : public CBaseHyoView
{
	DECLARE_DYNCREATE(CH31HyoView)

protected:
	CH31HyoView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CH31HyoView();

public:
	//enum { IDD = IDD_H31HYO_VIEW };
	enum { IDD = 101 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

// 操作
public:

	// エラーメッセージの取得
	CString& GetErrMsg() { return m_ErrMsg; };
	// 初期化

	//--->yoshida190627
	int InitView(CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei, CSnHeadData *pSnHeadData, SYC_SHININFO *pShinInfo , CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl);
	//-------
//	int InitView(  CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei , CSnHeadData *pSnHeadData , SYC_SHININFO *pShinInfo);
	//<------
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
	int ReInitDisp( BOOL isCompChange, BOOL SixKindFlg=FALSE, EnumIdIcsShRevType shrevtype=ID_ICSSH_NO_REVTYPE );
	//本表・26号様式　上部のみ再表示
	void ReDispHonpyoUpper();
	// 新様式 ( 旧税率を適用した取引なし )表示？
	BOOL IsNoneMTForm();
	// 旧様式で表示
	int InitOldFormTab ( BOOL bDelete );

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
	// 付表5-1・付表5-2　手入力サインのクリア '20.01.29
	int ClearManualInputSign_Fhyo5();
	// 付表5-1　　　　 　手入力サインのクリア ('24.03.06 Add)
	int ClearManualInputSign_Fhyo51();
	// 次回申告データ作成に必要になるデータの読込み
	int ReadDataForNextShinkoku();
	// ページ切替
	void PageChange(int move);
	// 不要になった帳票のデータをクリア
	void ClearHyoData();
	void ClearHyoData12();
	void ClearHyoData22();
	void ClearHyoData42();
	void ClearHyoData52();
	void ClearHyoData522();

	// 特定課税仕入と課税売上割合のチェック
	int ChkTkkzAndKzwari(int srctab, int calltype);
	// 課税売上割合が95%以上？
	BOOL IsRatio95Over();

	// 控除対象仕入税額の再計算
	int RecalqSkj ( SYZTOSPC_DATA *pSyzToSpc );

	// 印刷情報のセット
	int SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar, SH_PRINT_CMINFO *pPrintCmInfo );
	// 本表印刷
	int PrintHonPyo( CDC *pDC, BOOL isHikae, int PPno );
	int PrintHonPyoSub1(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSub2(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSub3(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSub4(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX1(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX2(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX3(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX4(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	//----->('23.05.01 Add)
	int PrintHonPyoSubEX2_1(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX2_2(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX2_3(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyoSubEX2_4(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	//<-----


	// 課税標準額等の内訳書印刷
	int PrintHonPyo2( CDC *pDC, BOOL isHikae, int PPno );
	int PrintHonPyo2Sub1(CDC *pDC, BOOL isHikae, int PPno, char *AP);
	int PrintHonPyo2Sub2(CDC *pDC, BOOL isHikae, int PPno, char *AP);


	// 付表１－１出力
	int PrintFuhyo1_01( CDC *pDC, int kojin, int pcnt );
	// 付表１－２出力
	int PrintFuhyo1_02( CDC *pDC, int kojin, int pcnt );
	// 付表１－３出力
	int PrintFuhyo1_03( CDC *pDC, int kojin, int pcnt );
	// 付表２－１出力
	int PrintFuhyo2_01( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo2_11( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo2_11Ex( CDC *pDC, int kojin, int pcnt );
	// 付表２－２出力
	int PrintFuhyo2_02( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo2_12( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo2_12Ex( CDC *pDC, int kojin, int pcnt );
	// 付表２－３出力
	int PrintFuhyo2_03( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo2_03Ex( CDC *pDC, int kojin, int pcnt );
	// 付表４－１出力
	int PrintFuhyo4_01( CDC *pDC, int kojin, int pcnt );
	// 付表４－２出力
	int PrintFuhyo4_02( CDC *pDC, int kojin, int pcnt );
	// 付表４－３出力
	int PrintFuhyo4_03( CDC *pDC, int kojin, int pcnt );
	// 付表５－１出力
	int PrintFuhyo5_011( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo5_012( CDC *pDC, int kojin, int pcnt );
	// 付表５－２出力
	int PrintFuhyo5_021( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo5_022( CDC *pDC, int kojin, int pcnt );
	// 付表５－３出力
	int PrintFuhyo5_031( CDC *pDC, int kojin, int pcnt );
	int PrintFuhyo5_032( CDC *pDC, int kojin, int pcnt );
	// 付表６出力
	int PrintFuhyo6( CDC *pDC, int pcnt );
	int PrintFuhyo6_02( CDC *pDC, int pcnt );
	// 付表６（税率別消費税額計算表）出力		('23.05.01 Add)
	int PrintFuhyo6_2wari( CDC *pDC, int kojin, int pcnt );

	// 付表６ページ取得
	int GetMxListFhyo6();
	
	// 更正の請求(法人)　出力
	int PrintKosei_Hojin( CDC* pDC );
	int PrintKosei_HojinSub1(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_HojinSub2(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_HojinSub3(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_HojinSub4(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_HojinSub5(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_HojinSub6(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	// 更正の請求(個人)　出力
	int PrintKosei_Kojin( CDC* pDC );
	int PrintKosei_KojinSub1(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_KojinSub2(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_KojinSub3(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_KojinSub4(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_KojinSub5(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);
	int PrintKosei_KojinSub6(int dp, CDC* pDC, char *BP, BOOL isNoImage, int PPno);

	// 還付付表（法人）
	void PrintRefund_Kojin(CDC* pDC, int pno);
	// 還付付表（個人）
	void PrintRefund_Hojin(CDC* pDC, int pno);

	// 前年実績による中間申告
	int PrintChukan(CDC* pDC);
	int PrintChukan_sub1(int dp, CDC* pDC, char *BP);
	int PrintChukan_sub2(int dp, CDC* pDC, char *BP);
	int PrintChukan_sub3(int dp, CDC* pDC, char *BP);
	int PrintChukan_sub4(int dp, CDC* pDC, char *BP);

	// 前年実績による中間申告 A4ヨコ控用
	int PrintChukanEx(CDC* pDC);
	int PrintChukan_sub1Ex(int dp, CDC* pDC, char *BP);
	int PrintChukan_sub2Ex(int dp, CDC* pDC, char *BP);
	int PrintChukan_sub3Ex(int dp, CDC* pDC, char *BP);
	int PrintChukan_sub4Ex(int dp, CDC* pDC, char *BP);

    //特例帳票 テーブル情報初期化
    void InitTokureiTblInfo();
    void ClearTokureiData();
	void GetKazeiKikan(char* kikan_s, char* kikan_e);
	void GetTekiyoTaisyoKikan(char* kikan_s, char* kikan_e);
	void GetName(char* name, int kojin);
	void CloseSyzTokurei();

	// 特例１ 課税資産の譲渡等の対価の額の計算表
	int PrintTokurei_Uri1(CDC *pDC, int kojin, int pcnt);
    int ReadPrintTokurei_Uri1(int sn_seq, int version);
	int ReadSubPrintTokurei_Uri1(CSYZ_TOKUREI* pTokurei);

	// 特例２　課税資産の譲渡等の対価の額の計算表
	int PrintTokurei_Uri2(CDC *pDC, int kojin, int pcnt);
	int ReadPrintTokurei_Uri2(int sn_seq, int version);
	int ReadSubPrintTokurei_Uri2(CSYZ_TOKUREI* pTokurei);

	// 特例３　課税仕入れ等の税額の計算表
	int PrintTokurei_Shiire(CDC *pDC, int kojin, int pcnt);
	int ReadPrintTokurei_Shiire(int sn_seq, int version);
	int ReadSubPrintTokurei_Shiire(CSYZ_TOKUREI* pTokurei);
    bool Get50CheckStatusColumn0_5_1();

    int GetPageNumTokurei(int hyono);

	// 集計データの取得
	int GetSyzSyukei_Ippan();
	int GetSyzSyukei_Kani();
	int GetSyzSyukei_Ippan2();

	// 切取線
	void Lin_Print();
	// 金額をセット（中間申告用）
	int val_set(char *bf, unsigned char *val);
	// 電話番号をセット（中間申告用）
	void ZTel_Set(char *buf, char *telno, char sw, int len, int lng);

	// 法人業種で法人番号未出力？
	bool IsNotPrintHojinNo();

	// 金額取得
	int GetMony( CString &tag, CString &chtag, char *pArith );
	// 金額セット
	int SetMony( CString &tag, CString &chtag, char *pArith );
	// 金額の再取得
	int ReGetCalqedMony();
	// 仕入控除税額の財務連動
	int ConnectedSkj( MoneyBasejagArray &MoneyBaseArray, bool isUpdDisp );
	// 仕入控除税額のみの財務連動
	int ConnectedSkjZeigaku( bool isUpdDisp);
	// 仕入金額等の明細のクリア
	int ClearShiire();
	// 更正の請求用　申告書金額計算
	int CalqSyzForKsk( int mode );
	// 更正→修正確定に進んだ場合の既確定税額の金額セット
	int CalqSyzFromKsk();
	// タブタイトルの切替
	void ChengTabTitle( char type );
	// 中間及び期間短縮の更新
	int RenewSyzShin( SH_PASTINFO *pPastInfo );
	// 本表の計算
	void AlreadyDecisionCalc( int type );
	// 棚卸調整額の連動
	int CalqStatckSyz();
	// 前年の消費税額取得
	int GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith );
	// 集計済み金額の取得
	int GetCalqedMony();
	// 既確定税額のクリア
	int ClearAlreadyDecision();

	// 各表の更新処理
	void UpdateSheetFromOuter();
	// 全表の更新処理
	void UpdateAllSheet( int sgn = 0 );
	// 選択表より前までの表の更新処理
	void UpdateUntilSheet();

	void TelBufSet(char *buf, int mode);

	// 予定26号の前課税期間のセット
	void ZenkiDate_Chsk(char* ZenkSymd, char* ZenkEymd, int ZenkSg, int ZenkEg);
//	void ZenkiDate_Chsk(char* ZenkSymd , char* ZenkEymd );
	// 表示中頁の更新処理後の再表示
	void IndicateSheet();
	// タブへフォーカスセット
	void EndJobFocus();

	// 入力ロック呼び出し
	void InputRock(int mode);

	// 簡易課税で横計ではない例外計算が必要？
	BOOL IsNeedExpCalq();

	// 税務署入力にフォーカス誘導
	int GuideTaxOfficeInput();
	// 簡易課税第六種フラグをセット
	void SetSixKindFlg( BOOL SixKindFlg ){ m_SixKindFlg = SixKindFlg; };
	// 還付申告？
	BOOL IsKanpuShinkoku();
	BOOL IsKanpuShinkoku2();
	// 出力用税理士名の作成
	int MakeOutZeirishiName( char *pOutBuf ,int len, int Honpyo = 0 );
	// 入力位置を保存
	void SaveInputPosition();
	// 入力位置を復元
	void RestoreInputPosition();
	// 終了時のフォーカス移動
	void MoveFocusInEnd();

	// 改行コードの検索
	int Getindex_toNewLineCode( int start, int *end, CString str );

	// 全角・半角交じり判定
	int Strchek ( char *dmmy, int num );

	// マイナンバー履歴を書き込む
	void WriteNoHistory(int write_sgn);

	// マイナンバーあり？
	int IsExist_MyNumber_Fuhyo6();


	// 消費税改正様式の取得('15.12.07)
	EnumIdIcsShRevType	GetShRevType() { return m_ShRevType; };

	// 入力ロックコントロールの再描画
	void RedrawInputLock();


	// 特定課税仕入れが可能か？
	int IsAbleTkKz();
	// 特定課税仕入れを「あり」から「なし」へ
	int ChangeTkkzUse(int srctab, int calltype);
	// 正常な特定課税仕入れか（前期以前の返還か）を確認
	int ChkZkTkkzReturn(int srctab, int calltype);

	// 課税取引金額計算書から転記時のフォーカス移動
	int MoveFocusInTaxTenki();

	// 改正様式をセット
	void SetShRevType ( EnumIdIcsShRevType shrevtype );

	// 送付不要をカット?
	bool IsNoDeliForm();

	// 内部タブデータの再取得
	int ReGetData();

	// 更正の請求用 申告書金額計算(実部)
	int CalqSyzForKskSub ( void *pCalqedMony1, void *pCalqedMony2, char *Zei );

	// データ読込み(金額集計用)
	int ReadDataForCalq();

	// 本表の計算
	void AlreadyDecisionCalc31();

	// メモリ再確保
	void ReMemory();

	// 還付金融機関入力にフォーカス誘導
	int GuideKanpuKinyu ( int dsInput );

	// コンバージョンが必要か
	BOOL IsNeedOldDataCnv();
	// データの引き継ぎ
	int TakeOverData( int srcVer, int snSeq, CDBNpSub *pZmsub );

	// 初期化完了フラグの切り替え
	void SetInitedFlg( bool flg );
	// View の初期化完了？
	bool IsInitedView(){ return m_isInited; };
	// マイナンバー閲覧サインのセット
	void SetMyNumBrowseSgn(BOOL NoHistory_browse){ m_NoHistory_browse = NoHistory_browse; }

	// 元号の○印 未出力？ '19.08.19 大藤追加
	bool IsNotPrintGengoCircle();

	// 還付金融機関　記入漏れチェック(更正の請求)
	int Message_BlankKinyu_Kousei();

	// 付表６ 金額チェック
	int Message_NofuKanpuGokei();

	// 付表６ マイナンバーチェック
	int Message_MyNumber_error();

	// 付表６ マイナンバーチェック
	int Message_MyNumber_overlap();

	// 付表６ 相続割合チェック
	int Message_SozokuWariai();

	void EditOff_Fuhyo6();

//----> 納付書・相続人対応
	// 付表６ 相続人情報の取得
	int GetSozokuInfo( NOHFUSHO_INFO *nohfuinfo, NOHFUSHO_DATA *nohfudata, CArray<int> *SozokuNoAry );
//<---- 納付書・相続人対応

	// 手入力サイン　データベース書き込み関数
	int WriteDBManualInputSign(CString &tag, int mode = 0);//yoshida190909

	 // 特定収入計算表ダイアログ表示
	int SpecPassDlg ( int type );
//'19.10.28
	// 予定申告の月数クリア('17.02.09)
	int ClearYoteiMonth();

	// 前回：予定申告サインを取得[23-0203]('24.07.23 Add)
	BOOL GetPastYoteiSgn();

	// 前回：予定申告サインを落とす[23-0203]('24.07.23 Add)
	void ChgPastYoteiSgn( BOOL sign );

	// 手入力項目あり？
	BOOL IsManualInputSign();

	// 売上割合取得
	int GetPercentage ( CString &tag, CString &chtag, unsigned short *pWari );
	int GetPercentage2( int id, unsigned short *pWari, char *pArith );

	// 付表5-2(2) 手入力サイン取得
	int GetInputSign ( int type, int id, int &inputSign );

	// 付表5 控除対象仕入税額の出力をセット
	void SetPintArea();

	// 課税期間の取得
	void GetKazeiKikan( long &symd, long &eymd );

	// 課税期間に免税事業者の期間が含まれている？
	bool IsIncludeMenzeiKikan();

	// 二割特例使用中？
	bool IsNiwariTokurei();

	// 免税期間の集計を行う
	void MenzeiKikanSyukei( long symd, long eymd, char *pArith );

	// 課税開始日以降の課税売上高の集計を行う
	void KazeiUriageSyukei( char *pArith );

	// 還付付表フォーカス移動
	void MoveKanpuFocus();

	// 集計フラグセット
	void SetSyukeiFlg();

	// 付表5　表示中？
	//int IsFhyo5();

	// 控除対象仕入税額の選択
	//int SelectFhyo5();

	// 令和5年9月30日をまたぐ期間か  20231025
	bool IsCrossOverR050930();

	// 還付付表2　インボイス登録番号のチェック
	bool CheckInvno( int sgn );

	// 課税開始日以降の課税売上高の集計を行う(フラグ指定)
	void KazeiUriageSyukei(char *pArith, bool isNiwari);//修正依頼168613関連 2024.02.26

// 24/04/01_303 add -->
	int	Get_KouseiTymd( void );
// 24/04/01_303 add <--

protected:
	// 各表の更新処理
	void UpdateSheet(int selno, int get_sgn = 0); //get_sgn:　現在タブのデータを取得　0…する 1…しない
	// 各表の更新処理後の再表示
	void IndicateSheet(int selno);
	// 初期入力位置にフォーカスセット
	void SetItemPosition(int selno);
	// DIAGの属性変更
	void ChangeDiagAttr();
	// 還付申告用データ再設定 ('14.05.17)
	void ResetRefundData();

	// 事業の判定
	int f501_urikbn();
	// 業種数の判定
	int f501_jigyo();
	// 最有利になる表示位置を取得
	int f501_calq_tk_for_disp();
	// タブ内のダイアログを全消去
	void DeleteInsideDlg();

	//KazeiUriageSyukei(char*)、KazeiUriageSyukei(char*, bool)の実装 
	void KazeiUriageSyukeiImpl(char *pArith, bool isNiwari, int mode);//2024.02.26 

// 属性
private:
	CString				m_ErrMsg;			// エラーメッセージ
	int					m_SnSeq;			// 履歴シーケンス
	int					m_SvSnSeq;			// 履歴シーケンス（前回保存）

	CSyzSyukei			*m_pSyzSyukei;		// 消費税集計クラス

	BOOL				m_bShowed;			// 画面表示フラグ
	BOOL				m_SixKindFlg;		//m_SixKindFlg = TRUE・・第六事業種あり


	EnumIdIcsShRevType	m_ShRevType;		// 消費税改正様式

	BOOL				m_Syokai_NukiKomi;	// 税抜・税込を取得するかしないか

	long				m_DispTabSgn;		// タブ表示サイン

	BOOL				m_NoHistory_browse;	// マイナンバー履歴フラグ D0:画面表示　D1:プレビュー　D2:印刷

	BOOL				m_PastYoteiSgn;		// 前回申告：予定サイン[23-0203]('24.07.23 Add)

	// 印刷情報
	CVPrint				*m_pVprn;			// 出力クラス
	NPRT_PAR			*m_pPset;			// 印刷情報
	SH_PRINT_CMINFO		*m_pPrintCmInfo;	// 印刷共有情報

	// 内部タブ
	CArray<SH_TABMNG>	m_TabMng;			// タブ管理
	ICSDialog			*m_pFirstPage;		// 初期ページ保存

	CRect				vwRect;				// ウィンドウレクト


	CDBNpSub			*m_pZmSub;			// 公益クラス
	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報

	SYC_SHININFO		*m_pShinInfo;		// 共通情報
	CArithEx			m_Arith;			// 倍長演算
	CSyzUtil			m_Util;				// ユーティリティクラス
	CDBSyzAccess        m_DBSyzAccess;
	CSyzUtil            m_SyzUtil;

	//---->yoshida190627
	CTblhdl				*m_pTblhdl;			// テーブルハンドリング
	_TB_PAR				*m_pSkjCnvtbl;		// 仕入控除用テーブル
	//<-----

	CShinFhyo1_01		*m_pFhyo1_01;		// 付表１－１
	CShinFhyo1_02		*m_pFhyo1_02;		// 付表１－２
	CShinFhyo1_03		*m_pFhyo1_03;		// 付表１－３
	CShinFhyo2_01		*m_pFhyo2_01;		// 付表２－１
	CShinFhyo2_11		*m_pFhyo2_11;		// 付表２－１ ( 令和2年4月1日以後終了課税期間用 )
	CShinFhyo2_11Ex		*m_pFhyo2_11Ex;		// 付表２－１ ( インボイス対応 )
	CShinFhyo2_02		*m_pFhyo2_02;		// 付表２－２
	CShinFhyo2_12		*m_pFhyo2_12;		// 付表２－２ ( 令和2年4月1日以後終了課税期間用 )
	CShinFhyo2_12Ex		*m_pFhyo2_12Ex;		// 付表２－２ ( インボイス対応 )
	CShinFhyo2_03		*m_pFhyo2_03;		// 付表２－３
	CShinFhyo2_03Ex		*m_pFhyo2_03Ex;		// 付表２－３ ( インボイス対応 )
	CShinFhyo4_01		*m_pFhyo4_01;		// 付表４－１
	CShinFhyo4_02		*m_pFhyo4_02;		// 付表４－２
	CShinFhyo4_03		*m_pFhyo4_03;		// 付表４－３
	CShinFhyo5_01		*m_pFhyo5_01;		// 付表５－１
	CShinFhyo5_012		*m_pFhyo5_012;		// 付表５－１（２）
	CShinFhyo5_02		*m_pFhyo5_02;		// 付表５－２
	CShinFhyo5_022		*m_pFhyo5_022;		// 付表５－２（２）
	CShinFhyo5_03		*m_pFhyo5_03;		// 付表５－３
	CShinFhyo5_032		*m_pFhyo5_032;		// 付表５－３（２）
	CShinFhyo6			*m_pFhyo6;			// 付表６
	CShinFhyo6_2wari	*m_pFhyo6_01;		// 付表６(２割特例)	('23.05.01 Add)
	CShinIppan01		*m_pShinIppan01;	// 一般本表
	CShinIppan01EX		*m_pShinIppan01EX;	// 一般本表
	CShinIppan01EX2		*m_pShinIppan01EX2;	// 一般本表 ('23.05.01 Add)
	CShinIppan02		*m_pShinIppan02;
	CShinKani01			*m_pShinKani01;		// 簡易本表
	CShinKani01EX		*m_pShinKani01EX;	// 簡易本表
	CShinKani01EX2		*m_pShinKani01EX2;	// 簡易本表 ('23.05.01 Add)
	CShinKSisanUri		*m_pKsisanuri;		// 特例帳票１　
	CShinKsisan_Uri2	*m_pKsisanuri2;		// 特例帳票２　
	CShinKshiire		*m_pKshiire;		// 特例帳票３　

	int					m_nowTabIdx;		// 現表示タブNo.


	CShinRefundKjn01	*m_pRefundKjn01;	// 還付付表１（個人）
	CShinRefundKjn02	*m_pRefundKjn02;	// 還付付表２（個人）
	CShinRefundKjn02EX	*m_pRefundKjn02EX;	// 還付付表２（個人）	// 20230704
	CShinRefundHjn01	*m_pRefundHjn01;	// 還付付表１（法人）
	CShinRefundHjn02	*m_pRefundHjn02;	// 還付付表２（法人）
	CShinRefundHjn02EX	*m_pRefundHjn02EX;	// 還付付表２（法人）	// 20230704


	CShinChskEX			*m_pChskEX;			// 予定申告（２６号）
	CShinHjnKskEX		*m_pHjnKskEX;		// 更正の請求（法人）
	CShinKjnKskEX		*m_pKjnKskEX;		// 更正の請求（個人）
	CShinHjnKskEX2		*m_pHjnKskEX2;		// 更正の請求（法人）令和4年12月31日以後開始分
	CShinKjnKskEX2		*m_pKjnKskEX2;		// 更正の請求（個人）令和4年12月31日以後開始分 → 20230405「公金受取口座を利用する」追加版に置き換え


	//== 各帳票データ ==
	CH31SnHonpyoData	*m_pSnHonpyoData;	// 一般本表データ
	CH31SnHonpyo2Data	*m_pSnHonpyo2Data;	// 課税標準額等の内訳書データ
	CH31SnFhyo1_01Data	*m_pSnFhyo1_01Data;	// 付表１－１データ 
	CH31SnFhyo1_02Data	*m_pSnFhyo1_02Data;	// 付表１－２データ 
	CH31SnFhyo2_01Data	*m_pSnFhyo2_01Data;	// 付表２－１データ
	CH31SnFhyo2_02Data	*m_pSnFhyo2_02Data;	// 付表２－２データ
	CH31SnTanaData		*m_pSnTanaData;		// 棚卸調整データ
	CH31SnFhyo4_01Data	*m_pSnFhyo4_01Data;	// 付表４－１データ
	CH31SnFhyo4_02Data	*m_pSnFhyo4_02Data;	// 付表４－２データ
	CH31SnFhyo5_01Data	*m_pSnFhyo5_01Data;	// 付表５－１データ
	CH31SnFhyo5_012Data	*m_pSnFhyo5_012Data;// 付表５－１（２）データ
	CH31SnFhyo5_02Data	*m_pSnFhyo5_02Data;	// 付表５－２データ
	CH31SnFhyo5_022Data	*m_pSnFhyo5_022Data;// 付表５－２（２）データ
	CH31SnFhyo6Data		*m_pSnFhyo6Data;	// 付表６データ
	CH31SnFhyo6_2wariData  *m_pSnFhyo6_01Data;	// 付表６データ(２割特例)

	CH31SnRefundData	*m_pSnRefundData;	// 還付申告データ
	CH31SnChskData		*m_pSnChskData;		// 中間予定申告データ
	CH31SnKskData		*m_pSnKskData;		// 更正の請求データ

	int					m_dspSkjSw;			// 仕入控除税額計算の、どの行が表示されているか（'14.06.11）
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
	int					m_tkSw;
	int					m_typeCnt;

	//== 帳票データ ==
	char	XM1[MONY_BUF_SIZE],		//  1. 課税標準額
			XM2[MONY_BUF_SIZE],		//  2. 消費税額
			XM3[MONY_BUF_SIZE],		//  3. 控除過大調整税額
			XM4[MONY_BUF_SIZE],		//  4. 控除対象仕入税額
			XM5[MONY_BUF_SIZE],		//  5. 返還等対価に係る税額
			XM6[MONY_BUF_SIZE],		//  6. 貸倒れに係る税額
			XM7[MONY_BUF_SIZE],		//  7. 控除税額小計
			XM8[MONY_BUF_SIZE],		//  8. 控除不足還付税額
			XM9[MONY_BUF_SIZE],		//  9. 差引税額
			XM10[MONY_BUF_SIZE],	// 10. 中間納付税額
			XM11[MONY_BUF_SIZE],	// 11. 納付税額
			XM12[MONY_BUF_SIZE],	// 12. 中間納付還付税額
			XM13[MONY_BUF_SIZE],	// 13. 既確定税額
			XM14[MONY_BUF_SIZE],	// 14. 差引納付税額
			XM15[MONY_BUF_SIZE],	// 15. 課税資産の譲渡等の対価の額
			XM16[MONY_BUF_SIZE],	// 16. 資産の譲渡等の対価の額
			XM17[MONY_BUF_SIZE],	// 17. 控除不足還付税額
			XM18[MONY_BUF_SIZE],	// 18. 差引税額
			XM19[MONY_BUF_SIZE],	// 19. 還付額
			XM20[MONY_BUF_SIZE],	// 20. 納税額
			XM21[MONY_BUF_SIZE],	// 21. 中間納付譲渡割額
			XM22[MONY_BUF_SIZE],	// 22. 納付譲渡割額
			XM23[MONY_BUF_SIZE],	// 23. 中間還付譲渡割額
			XM24[MONY_BUF_SIZE],	// 24. 既確定譲渡割額
			XM25[MONY_BUF_SIZE],	// 25. 差引納付譲渡割額
			XM26[MONY_BUF_SIZE];	// 26. 消費税及び地方消費税の合計(納付又は還付)税額

	// 課税標準額等の内訳書データ
	char	XM2_1[MONY_BUF_SIZE],	// 課税標準額
			XM2_2[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額の合計額　3％
			XM2_3[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額の合計額　4％
			XM2_4[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額の合計額　6.3％
			XM2_5[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額の合計額　6.24％
			XM2_6[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額の合計額　7.8％
			XM2_7[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額の合計額　計
			XM2_8[MONY_BUF_SIZE],	// 特定課税仕入れに係る支払対価の額の合計額　6.3％
			XM2_9[MONY_BUF_SIZE],	// 特定課税仕入れに係る支払対価の額の合計額　7.8％
			XM2_10[MONY_BUF_SIZE],	// 特定課税仕入れに係る支払対価の額の合計額　計
			XM2_11[MONY_BUF_SIZE],	// 消費税額
			XM2_12[MONY_BUF_SIZE],	// ⑪の内訳　3％
			XM2_13[MONY_BUF_SIZE],	// ⑪の内訳　4％
			XM2_14[MONY_BUF_SIZE],	// ⑪の内訳　6.3％
			XM2_15[MONY_BUF_SIZE],	// ⑪の内訳　6.24％
			XM2_16[MONY_BUF_SIZE],	// ⑪の内訳　7.8％
			XM2_17[MONY_BUF_SIZE],	// 返還等対価に係る税額
			XM2_18[MONY_BUF_SIZE],	// ⑰の内訳　売上げの返還等対価に係る税額
			XM2_19[MONY_BUF_SIZE],	// ⑰の内訳　特定課税仕入れの返還等対価に係る税額
			XM2_20[MONY_BUF_SIZE],	// 地方消費税の課税標準となる消費税額  計
			XM2_21[MONY_BUF_SIZE],	// 地方消費税の課税標準となる消費税額　4％
			XM2_22[MONY_BUF_SIZE],	// 地方消費税の課税標準となる消費税額　6.3％
			XM2_23[MONY_BUF_SIZE];	// 地方消費税の課税標準となる消費税額　6.24％及び7.8％

	// 特例帳票1データ
	PTBLTDATAINFO m_Uri1kbndatainfo[10];
	PTBLTDATAINFO m_Uri1datainfo[6];
	int           m_Uri1MxPage;

	char PRT_KSURI_TITLE[JIGYO_MAX_TOKUREI_1][64]; // 事業の区分ごとの計算
	char PRT_KSURI_1[JIGYO_MAX_TOKUREI_1][16];	 // １．課税資産の譲渡等の税込価額の合計額
	char PRT_KSURI_2_S[JIGYO_MAX_TOKUREI_1][4];	 // ２．通常の事業を行う連続する１０営業日　（自）
	char PRT_KSURI_2_E[JIGYO_MAX_TOKUREI_1][4];	 // ２．通常の事業を行う連続する１０営業日　（至）
	char PRT_KSURI_3[JIGYO_MAX_TOKUREI_1][16];	 // ３．2の期間中に行った課税資産の譲渡等の税込価額の合計額
	char PRT_KSURI_4[JIGYO_MAX_TOKUREI_1][16];	 // ４．のうち、軽減対象資産の譲渡等（税込み6.24%適用分）に係る部分の金額（税込み）
	char PRT_KSURI_5[JIGYO_MAX_TOKUREI_1][16];	 // ５．軽減売上割合
	int  PRT_KSURI_5_CHECK[JIGYO_MAX_TOKUREI_1];	 // ５．軽減売上割合チェック状態.
	char PRT_KSURI_6[JIGYO_MAX_TOKUREI_1][16];	 // ６．軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI_7[JIGYO_MAX_TOKUREI_1][16];	 // ７．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

	char PRT_KSURI_6_KEI[16]; // ６．軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）　合計額
	char PRT_KSURI_7_KEI[16]; // ７．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　合計額
	char PRT_KSURI_8[16];     // ８．軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI_9[16];     // ９．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI_10[16];    // １０．軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI_11[16];    // １１．軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

	// 特例帳票2データ
	PTBLTDATAINFO m_Uri2kbndatainfo[11];
	PTBLTDATAINFO m_Uri2datainfo[6];
	int           m_Uri2MxPage;

	char PRT_KSURI2_TITLE[JIGYO_MAX_TOKUREI_23][64]; //事業の区分ごとの計算
	char PRT_KSURI2_1[JIGYO_MAX_TOKUREI_23][16];     //１.課税仕入れに係る支払い対価の額（税込み）
	char PRT_KSURI2_2[JIGYO_MAX_TOKUREI_23][16];     //２.特定課税仕入れに係る支払対価の額
	char PRT_KSURI2_3[JIGYO_MAX_TOKUREI_23][16];     //３.保税地域から引き取った課税貨物に係る税込引取価額
	char PRT_KSURI2_4[JIGYO_MAX_TOKUREI_23][16];     //４.課税仕入れに係る支払対価の額等の合計額
	char PRT_KSURI2_5[JIGYO_MAX_TOKUREI_23][16];     //５.４のうち、軽減対象資産の譲渡等（税率6.24%適用分）にのみ要するものの金額（税込み）
	char PRT_KSURI2_6[JIGYO_MAX_TOKUREI_23][16];     //６.小売等軽減仕入割合
	int  PRT_KSURI2_6_CHECK[JIGYO_MAX_TOKUREI_23];	 //６.小売等軽減仕入割合チェック状態.
	char PRT_KSURI2_7[JIGYO_MAX_TOKUREI_23][16];     //７.課税資産の譲渡等の税込価額の合計額
	char PRT_KSURI2_8[JIGYO_MAX_TOKUREI_23][16];     //８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI2_9[JIGYO_MAX_TOKUREI_23][16];     //９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

	char PRT_KSURI2_8_KEI[16]; //８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI2_9_KEI[16]; //９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI2_10[16];    //１０.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI2_11[16];    //１１.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI2_12[16];    //１２.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
	char PRT_KSURI2_13[16];    //１３.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

	// 特例帳票2データ
	PTBLTDATAINFO m_Shiirekbndatainfo[10];
	PTBLTDATAINFO m_Shiiredatainfo[18];
	int           m_ShiireMxPage;

	char PRT_KSHIIRE_TITLE[JIGYO_MAX_TOKUREI_23][64]; //事業の区分ごとの計算
	char PRT_KSHIIRE_1[JIGYO_MAX_TOKUREI_23][16];     //１.課税資産の譲渡等（免税取引及び旧税率(6.3%等)が適用される取引は除く。）の税込み価格の合計額
	char PRT_KSHIIRE_2[JIGYO_MAX_TOKUREI_23][16];     //２.軽減対象資産の譲渡等(6.24%適用分)の税率価額の合計額
	char PRT_KSHIIRE_3[JIGYO_MAX_TOKUREI_23][16];     //３.小売等軽減売上割合（2/1）
	int	 PRT_KSHIIRE_3_SIGN[JIGYO_MAX_TOKUREI_23];	  //３.小売等軽減売上割合手入力サイン
	char PRT_KSHIIRE_4[JIGYO_MAX_TOKUREI_23][16];     //４.課税仕入れに係る支払対価の額（税込み）
	char PRT_KSHIIRE_5[JIGYO_MAX_TOKUREI_23][16];     //５.保税地域から引き取った課税貨物に係る税込引取価額
	char PRT_KSHIIRE_6[JIGYO_MAX_TOKUREI_23][16];     //６.課税仕入れに係る支払対価の額等の合計額
	char PRT_KSHIIRE_7[JIGYO_MAX_TOKUREI_23][16];     //７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額
	char PRT_KSHIIRE_8[JIGYO_MAX_TOKUREI_23][16];     //８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額

	char PRT_KSHIIRE_7_KEI[16]; //７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額　合計
	char PRT_KSHIIRE_8_KEI[16]; //８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額　合計
	char PRT_KSHIIRE_9[16];     //９.納税義務の免税を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率6.24%適用分
	char PRT_KSHIIRE_10[16];    //１０.納税義務の免税を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率7.8%適用分
	char PRT_KSHIIRE_11_1[16];  //１１.課税仕入れに係る支払対価の額（税込み）　税率6.24%適用分
	char PRT_KSHIIRE_11_2[16];  //１１.課税仕入れに係る支払対価の額（税込み）　税率7.8%適用分
	char PRT_KSHIIRE_12_1[16];  //１２.課税仕入れに係る消費税額　税率6.24%適用分
	char PRT_KSHIIRE_12_2[16];  //１２.課税仕入れに係る消費税額　税率7.8%適用分
	char PRT_KSHIIRE_13_2[16];  //１３.特定課税仕入れに係る支払対価の額　税率7.8%適用分
	char PRT_KSHIIRE_14_2[16];  //１４.特定課税仕入れに係る消費税額　税率7.8%適用分
	char PRT_KSHIIRE_15_1[16];  //１５.課税貨物に係る消費税額　税率6.24%適用分
	char PRT_KSHIIRE_15_2[16];  //１５.課税貨物に係る消費税額　税率7.8%適用分
	char PRT_KSHIIRE_16_1[16];  //１６.納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整（加算又は減算）額　税率6.24%適用分
	char PRT_KSHIIRE_16_2[16];  //１６.納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整（加算又は減算）額　税率7.8%適用分
	char PRT_KSHIIRE_17[16];    //１７.課税仕入れ等の税額の合計額　税率6.24%適用分
	char PRT_KSHIIRE_18[16];    //１８.課税仕入れ等の税額の合計額　税率7.8%適用分
	char PRT_KSHIIRE_19[16];    //１９.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額の合計額
	char PRT_KSHIIRE_20[16];    //２０.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額の合計額

	bool	m_isInited;			// 初期化完了？

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSTabCtrl	m_MainTab;

public:
	void OnTcnSelchangingMainTab(NMHDR * pNotifyStruct, LRESULT * result);
	void OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result);

	afx_msg void OnButtonF2();
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	afx_msg void OnButtonF6();
	afx_msg void OnButtonF7();
	afx_msg void OnButtonF8();
	afx_msg void OnButtonF9();
	afx_msg void OnButtonF11();
	afx_msg void OnButtonF12();
	afx_msg void OnUpdateButtonF4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF11(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF12(CCmdUI *pCmdUI);

	afx_msg void OnUpdateKomonsakiInf(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZeirishiInf(CCmdUI *pCmdUI);
	afx_msg void OnZeirishiInf();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT OnMyMessage( WPARAM wParam, LPARAM lParam );
};


