#pragma once

//=============================================================================
// 消費税非連動：マスター管理モジュール（内部クラス）
//-----------------------------------------------------------------------------
//	start	'13.08.05
//	latest	'13.08.05
//=============================================================================

#include "SyzMakeSub8.h"
#include "DBWaitDialog.h"

class CSyzMakeSubW
{
public:
	CSyzMakeSubW();
	virtual ~CSyzMakeSubW();

	// エラーメッセージ取得
	CString& GetErrorMsg();

	// データベースマスターの作成
	int MakeDBMaster( SYZMAKE_MKCOINFO &mkcoinfo, CWnd *pParent );
	// データベースマスターの削除
	int DeleteDBMaster( SYZMAKE_MKCOINFO &mkcoinfo );
	// zvolumeの再作成('17.05.01)
	int ReMakeZvolume( SYZMAKE_MKCOINFO &mkcoinfo );

	// 消費税の状態チェック
	int CheckSyzState( CDBZmSub *pZmsub, CString *pMsg, SYZ_CHKINFO &ChkInfo );
	// 消費税不正仕訳のチェック
	int CheckWrongData( CDBZmSub *pZmsub );
	// 誘導メッセージ取得
	CString& GetGuideMsg();

	// 平成26年改正 消費税コンバージョン
	int ConversionToZei8( CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst );
	// 平成31年改正 消費税コンバージョン
	int ConversionToZei10(CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst);

	// 消費税属性区分の取得
	int GetSyzAtrKbn();
	// 消費税率の取得
	int GetSyzZritu();

	// ５％を明記すべきマスター？
	BOOL IsNeed5PerNote( CDBZmSub *pZmsub );
	// 消費税集計テーブル内の不正仕訳チェック
	int ChkSzitmInSzsyk( CDatabase *pDatabase, BOOL isKani );
	// 消費税不正仕訳のチェック
	int CheckWrongData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd );

	// 輸入仕訳のコンバージョン
	int CnvImportShiwake( CDBZmSub *pZmsub );
	// 完成時振替仕訳のコンバージョン
	int CnvKensetuFurikaeShiawke( CDBZmSub *pZmsub, bool &bIsChanged );

	// 建設：未成工事支出金タイプで消費税判定に付箋使用マスター？
	int IsKensetsuSpMaster(CDBZmSub *pZmsub);

	// 簡易税不正仕訳のチェック('15.02.23)
	int CheckWrongKaniData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd );
	// 特定収入不正仕訳のチェック('15.03.11)
	int CheckWrongSpcData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd );
	// 前期以前の不正月別情報の修正('16.12.19)
	int RepairZmonthInfo( CDBZmSub *pZmsub );
	// 3%か5%の輸入仕入の仕訳あり？
	int ChkInput3or5ImportData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd);
	// 別記仕訳のチェック
	int CheckSepParaData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd);
	// 当期の不正月別情報の修正('20.08.28)
	int RepairMonthInfo( CDBZmSub *pZmsub );

	// 免税事業者からの課税仕入不正仕訳のチェック('23.03.29)
	int CheckWrongMzSrData(CDBZmSub *pZmsub);
	// 免税事業者からの課税仕入 有無チェック ('23.03.29)
	int CheckMzSrData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd);
	// 免税事業者からの課税仕入 の仕訳あり？('23.03.29)
	int ChkInputZeiMzSrData(class CDatabase *pChkDB, int stymd, int edymd);

	// 会社コード登録チェック ('17.04.10)
	int ChkRegistCoCode( SYZMAKE_MKCOINFO &mkcoinfo );

	// マスターを作成する時の必須項目をチェック('18.08.21)
	int CheckEssentialItem( SYZMAKE_MKCOINFO &mkcoinfo );

	// 消費税集計テーブル（szsyk）のPK変換
	int CnvSzsykPK(CDBZmSub *pZmsub);
	// 平成31年改正 自動コンバージョンを行える？
	bool IsAbleToAutoVup();

	// 消費税申告書に関連するデータの削除 ('20.02.20)
	int DeleteSyzData(SYZMAKE_MKCOINFO &mkcoinfo, int optionflg);

	//消費税申告書のデータクリア処理（会社登録課税区分切り替え時に呼び出し） '22.11.29
	int ClearSyzShinData(CDBZmSub *pZmsub, int optionflg);
	// インボイス施行日以降に残高発生有り？	'23.05.15
	int CheckZanAfterInvoice(CDBZmSub *pZmsub);

	// 消費税の集計済サインを落とす('23.08.28)
	int TrunOffSyzCksw(CDBZmSub *pZmsub);

	// 指定期間内の仕訳に打たれているインボイス登録番号の正当性をチェックする.
	int CheckWrongInvoiceData(CDBZmSub *pZmSub, long StYymmdd, long EdYymmdd, SYZ_WrongInvoiceDataList& WrongInvoiceDataList, CWnd* pParent);


private:
	// ボリュームコントロール登録
	int AddMasterToVlctl( SYZMAKE_MKCOINFO &mkcoinfo );
	// DB の作成
	int CreateDataBase( SYZMAKE_MKCOINFO &mkcoinfo );
	// データベース内 初期テーブルの作成
	int CreateInitZtable( SYZMAKE_MKCOINFO &mkcoinfo );
	// データベース内 テーブルデータの挿入
	int InsertInitTbData( SYZMAKE_MKCOINFO &mkcoinfo );
	// マスターの削除（会社情報）
	int DeleteMasterFromVctl( SYZMAKE_MKCOINFO &mkcoinfo, bool &haveImg );
	// データベース全削除
	int DropDatabaseAll( SYZMAKE_MKCOINFO &mkcoinfo, bool haveImg );
	// データベース全削除（ATOMⅡクラウド環境）
	int DropDatabaseAllInCloud( SYZMAKE_MKCOINFO &mkcoinfo, bool haveImg );

	// 上手君ユーザ情報作成
	int MakeJkAcsAry();

	// ボリュームラベル初期化
	int InitZVolume( SYZMAKE_MKCOINFO &mkcoinfo, bool bGetJzuser );
	// ボリュームラベルデータ用SQLの作成
	int MakeVolumeDataSql( char *pbuf, SYZMAKE_MKCOINFO &mkcoinfo );
	// 部門ボリュームラベルデータ用SQLの作成
	int MakeBmVolumeDataSql( char *pbuf, SYZMAKE_MKCOINFO &mkcoinfo );
	// 月別情報データ用SQLの作成
	int MakeMonthInfoDataSql( int mofs, char *pbuf, SYZMAKE_MKCOINFO &mkcoinfo );
	// 月別情報データ用SQLの作成
	int MakeMonthInfoDataSql( int mofs, char *pbuf, char *pDbSvrName );
	// 前期月別情報テーブルの作成
	int MakeZMonthInfoDataSql( int kofs, int mofs, char* pbuf, SYZMAKE_MKCOINFO &mkcoinfo );
	// SQL文字列データへの変換
	int ConvertToSqldata( EnumIdIcsSyzCnvSqlType nType, void* pData, int nSize, CString& rStr, BOOL doSeparate=TRUE );
	// データベースカラム数取得
	int GetColumnCount( LPCTSTR pTname, LPCTSTR pSvrName );
	// 月別情報データの初期化
	int InitMonthInfo( SYZMAKE_MKCOINFO &mkcoinfo );
	// 決算修正サイン更新
	int UpdateKIsgn();
	// 財務サブスイッチ更新
	int UpdateZmsubSubsw( SYZMAKE_MKCOINFO &mkcoinfo );
	// 和暦年月日の比較
	int CmpWarekiYMD(int gengo1, BYTE* pYmd1, int gengo2, BYTE* pYmd2, int cmpNum=3);

	// ウエイトダイアログ表示
	int ShowWaitDialog( CWnd *pParent );
	// ウエイトダイアログ非表示
	int HideWaitDialog( CWnd *pParent );
	// タイトルセット
	int SetWaitTitle( LPCTSTR pTitle );

	// データ変換
	int Conversion( int nowVersion, CDBZmSub *pZmsub, EnumIdDataCnvType cnvtype );

	// 消費税バージョンの取得
	int GetZvolumeSver( class CDatabase *pChkDB, int &nSver );
	
	// 該当税率の仕訳あり？
	int ChkInputZeiDataSub(class CDatabase *pChkDB, EmumIdZeirituType ztype);
	// 8% の仕訳あり？
	int ChkInputZei8Data( class CDatabase *pChkDB );
	// 10% の仕訳あり？
	int ChkInputZei10Data(class CDatabase *pChkDB);
	// 8%（軽減） の仕訳あり？
	int ChkInputZei8RdData(class CDatabase *pChkDB);

	// 該当税率の仕訳あり？
	int ChkInputZeiDataSub(class CDatabase *pChkDB, int stymd, int edymd, EmumIdZeirituType ztype);
	// 8% の仕訳あり？
	int ChkInputZei8Data( class CDatabase *pChkDB, int stymd, int edymd );
	// 10% の仕訳あり？
	int ChkInputZei10Data(class CDatabase *pChkDB, int stymd, int edymd);
	// 8%（軽減） の仕訳あり？
	int ChkInputZei8RdData(class CDatabase *pChkDB, int stymd, int edymd);
	// 免税事業者からの仕入仕訳あり？
	int ChkInputStockFromMenzeiData(class CDatabase *pChkDB, int stymd, int edymd);

	// 該当月オフセットの取得
	int GetChkMonOfset(CDBZmSub *pZmsub, int chkStYymmdd, int chkEdYymmdd, int& StMofs, int& EdMofs);
	// 仮受・仮払消費税のコード取得
	int GetKariUkeBaraiKcd(CDBZmSub *pZmsub, CString& KribaraiKcd, CString& KriukeKcd);

	// 不正仕訳のコンバージョン
	int CnvWrongShiwake( CDBZmSub *pZmsub, BOOL &IsChanged, EnumIdDataCnvType cnvtype );
	// 該当日以降の仕訳あり？
	int ChkDataAfterDate( class CDatabase *pChkDB, int ymd, bool isChkDel );
	// 変換 ( 文字列→BCD )
	void AscToBcd( char* buff, char* data, int len );
	// 消費税バージョンと期末年月日の取得
	int GetZvolumeSverEymd( class CDatabase *pChkDB, int &nSver, int &nEymd );
	// 消費税バージョンとマスタータイプの取得
	int GetZvolumeSverMtype( class CDatabase *pChkDB, int &nSver, int &nMtype );
	// 平成27年改正 簡易課税第６種コンバージョン ('14.12.12)
	int ConversionToKani6( CDBZmSub *pZmsub );
	// szvolの情報を取得する ('14.12.12)
	int GetSzvolumeInfo( class CDatabase *pChkDB, int &SVvers );
	// 簡易第六種 の仕訳あり？('15.02.11)
	int ChkInputZeiKani6Data( class CDatabase *pChkDB );
	// 簡易第六種 の仕訳あり？('15.02.23)
	int ChkInputZeiKani6Data( class CDatabase *pChkDB, int stymd, int edymd );
	// 簡易課税マスター？('15.02.11)
	BOOL IsKaniMaster( class CDatabase *pChkDB );
	// 特定収入仕訳あり？('15.03.11)
	int ChkInputZeiSpcData( class CDatabase *pChkDB, int stymd, int edymd );

	// 国税と地方税を分割した輸入仕入の仕訳あり？
	int ChkInputExtImportData( class CDatabase *pChkDB, int stymd, int edymd );
	// 完成時振替の仕訳あり？
	int ChkInputKojiFurikaeData(class CDatabase *pChkDB, int stymd, int edymd);
	// 3%か5%の輸入仕入の仕訳あり？
	int ChkInput3or5ImportData(class CDatabase *pChkDB, int stymd, int edymd);
	// 別記の仕訳あり？
	int ChkInputSepParaData(CDBZmSub *pZmsub, int stymd, int edymd);
	// 仮受・仮払消費税の仕訳あり？
	int ChkInputKariUBData(CDBZmSub *pZmsub, int stymd, int edymd);

	// テーブル作成用のスクリプトを取得
	void GetMakeTableScript( SYZMAKE_MKCOINFO &mkcoinfo, char*** table, char*** systbl, int &tmax );
	// スクリプトファイル(ztable.sql)のパスを取得
	void GetZtableScriptPath( CString &sqlPath, SYZMAKE_MKCOINFO &mkcoinfo );

	// データ変換
	void CnvToJcymd( int type, int smofs, JCYMD& dstJcymd );

	// テーブルの存在チェック
	bool IsExistTbl( class CDatabase *pChkDB, CString& tblname );


	//'20.04.16
	int UpdateMasterToVlctl(SYZMAKE_MKCOINFO &mkcoinfo);

	//20.09.08
	//内訳書Vtblバージョン取得
	int GetVtblUtiwakeVer(CDatabase* db, int &uVver , SYZMAKE_MKCOINFO &mkcoinfo);

	//20.09.08
	//内訳書Vtblバージョン更新
	int UpdateVtblUtiwakeVer(CDatabase* db, int &uVver, int mode , SYZMAKE_MKCOINFO &mkcoinfo);


private:
	CString			m_ErrMsg;				// エラーメッセージ
	CString			m_GuideMsg;				// 処理誘導メッセージ
	CDBWaitDialog	*m_pWdlg;				// ウエイトダイアログ

	ICSMCTL			m_Mctl;					// マスターコントロール
	DBAccess		m_DBAccess;				// DBアクセスクラス
	CVolDateDB		m_VolDate;				// 期間情報
	CArith			m_Arith;				// 倍長演算クラス
	char			m_mkCoName[256];		// 作成対象会社名（ＤＢ名）
	CString			m_phydisk;				// マスター格納パス
	CZVOLUME		m_mkZVolume;			// ボリュームラベル

	int				m_nInsw[MONTHALL];		// 入力許可サイン
	int				m_nSime;				// 締日
	BYTE			m_KIsgn[5][MONTHALL];	// 決算修正情報（当期～４期前）
	BYTE			m_KJsgn[5][MONTHALL];	// 決算除外情報（当期～４期前）
	int				m_nKsgn;				// 決算サイン
	int				m_nZKsgn[4];			// 決算サイン（前期）
//	DAPAC_BCD		m_Dapac[MONTHALL*5];	// 開始・終了月日（当期～４期前）
	DAPACGEN_BCD	m_Dapac[MONTHALL*5];	// 開始・終了月日（当期～４期前）

	int				m_MSgengo[MONTHALL*5];	// 開始元号
	UCHAR			m_MSymd[MONTHALL*5][3];	// 開始月日
	int				m_MEgengo[MONTHALL*5];	// 終了元号
	UCHAR			m_MEymd[MONTHALL*5][3];	// 終了月日

	int				m_nMNdt1[MONTHALL*5];	// 人数データ１
	int				m_nMNdt2[MONTHALL*5];	// 人数データ２


	CArray<COACS, COACS&>	m_AcsAry;		// 会社アクセステーブル
	CArray<COACS, COACS&>	m_JkAcsAry;		// 会社アクセステーブル（上手くん）
	USERTBL2_Array			m_UserAry;		// ユーザテーブル
	USERTBL2_Array			m_jUserAry;		// 顧問先テーブル
	USERTBL2_Array			m_jCoUserAry;	// 顧問先テーブル（上手くん）
	mctl_usertbl			m_user;			// ログインユーザ
	int						m_owner_id;		// オーナー
	BOOL					m_plcusr;		// パブリックユーザsw
	BOOL					m_isKjok;		// 経理上手君かどうか

	int						Utiwake_ver;		//内訳書バージョン
	int						Vtbl_utiwakeback;	//２：元なかったので、レコード削除　１:戻す必要あり(レコード残し)　0:戻す必要なし　


};	// CSyzMakeSubW
