#pragma once

//=============================================================================
// 消費税非連動：マスター管理モジュール
//-----------------------------------------------------------------------------
//	start	'13.08.05
//	latest	'19.02.22
//=============================================================================

#include "SyzMakeSubRec8.h"

class CDBZmSub;
class CSyzMakeSubW;
#ifdef _ICS_SYZMAKESUB_DLLMAKE_
class __declspec(dllexport) CSyzMakeSub
#else
class __declspec(dllimport) CSyzMakeSub
#endif
{
private:
	CSyzMakeSubW	*m_pSyzMakeSubW;
	CString			m_ErrMsg;			// エラーメッセージ
public:
	char			*pdummy1;			// 内部使用

	// コンストラクタ
	CSyzMakeSub();
	// デストラクタ
	virtual ~CSyzMakeSub();

	// エラーメッセージ取得
	CString& GetErrorMsg();

	// データベースマスターの作成
	int MakeDBMaster( SYZMAKE_MKCOINFO &mkcoinfo, CWnd *pParent );
	// データベースマスターの削除
	int DeleteDBMaster( SYZMAKE_MKCOINFO &mkcoinfo );

	// 消費税関連の状態チェック
	int CheckSyzState( CDBZmSub *pZmsub, CString &Msg, SYZ_CHKINFO &ChkInfo );
	// 消費税不正仕訳のチェック
	int CheckWrongData( CDBZmSub *pZmsub );
	// 誘導メッセージ取得
	CString& GetGuideMsg();

	// 平成26年改正 消費税コンバージョン
	int ConversionToZei8( CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst );

	// ５％を明記すべきマスター？('13.12.14)
	BOOL IsNeed5PerNote( CDBZmSub *pZmsub );
	// 消費税集計テーブル内の不正仕訳チェック('13.12.27)
	int ChkSzitmInSzsyk( CDatabase *pDatabase, BOOL isKani );
	// 消費税不正仕訳のチェック('14.03.01)
	int CheckWrongData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd );
	// 簡易税不正仕訳のチェック('15.02.23)
	int CheckWrongKaniData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd );
	// 特定収入不正仕訳のチェック('15.03.11)
	int CheckWrongSpcData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd );
	// 前期以前の不正月別情報の修正('16.12.19)
	int RepairZmonthInfo( CDBZmSub *pZmsub );
	// zvolumeの再作成('17.05.01)
	int ReMakeZvolume( SYZMAKE_MKCOINFO &mkcoinfo );
	// マスターを作成する時の必須項目をチェック('18.08.21)
	int CheckEssentialItem( SYZMAKE_MKCOINFO &mkcoinfo );

	// 平成31年改正 消費税コンバージョン
	int ConversionToZei10(CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst);
	// 平成31年改正 自動コンバージョンを行える？
	bool IsAbleToAutoVup();
	// 3%か5%の輸入仕入仕訳のチェック
	int Check3or5ImportData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd);
	// 別記仕訳のチェック
	int CheckSepParaData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd);

	// 消費税申告書に関連するデータの削除 ('20.02.20)
	int DeleteSyzData(SYZMAKE_MKCOINFO &mkcoinfo, int optionflg);

	// 当期の不正月別情報の修正('20.08.28)
	int RepairMonthInfo(CDBZmSub *pZmsub);

	// 消費税申告書のデータクリア処理（会社登録課税区分切り替え時に呼び出し） '22.11.29
	int ClearSyzShinData(CDBZmSub *pZmsub , int optionflg);

	// 不正な免税事業者からの課税仕入 有無チェック('23.03.29)
	int CheckWrongMzSrData(CDBZmSub *pZmsub);
	// 免税事業者からの課税仕入 有無チェック('23.03.29)
	int CheckMzSrData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd);

	// 消費税の集計済サインを落とす('23.08.28)
	int TrunOffSyzCksw(CDBZmSub *pZmsub);

	//-----------------------------------------------------------------------------
	// 指定期間内の仕訳に打たれているインボイス登録番号の正当性をチェックする.
	// Web-APIの判定時間に時間がかかるためチェック中待ちダイアログを表示する.
	//-----------------------------------------------------------------------------
	// 引数	pZmsub		     ：	マスターに接続済の財務クラスのポインタ
	//		StYymmdd	     ：	開始年月日
	//		EdYymmdd	     ：	終了年月日
	//      WrongInvoiceDataList： 不正データ詳細
    //      pParent           :  親ウィンドウ
	//-----------------------------------------------------------------------------
	// 返送値	0		：	正常（インボイス登録番号データ不正なし）
	//			1		：	正常（インボイス登録番号データ不正あり）
	//		   -1		：	エラー
	//-----------------------------------------------------------------------------
	int CheckWrongInvoiceData(CDBZmSub *pZmSub, long StYymmdd, long EdYymmdd, SYZ_WrongInvoiceDataList& WrongInvoiceDataList, CWnd* pParent);

protected:
	char	rsv1[512];

};	// CSyzMakeSub
