// DBSyzAccess8.h : ヘッダー ファイルDBSyzRecSlq8.h
//
#pragma once
#include "DBSyzRecDef8.h"
#include "DBSyzRecSql8.h"
#include "DBSyzRec8.h"

class CDBSyzAccess
{
public:
	__declspec(dllexport) CDBSyzAccess();
	__declspec(dllexport) virtual ~CDBSyzAccess();

// オペレーション
public:

	// 部門枝番消費税拡張
	class	CBMESZSYK	*m_pBmeszsyk;
	__declspec(dllexport) int BmeSzsykOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void BmeSzsykClose(void);
	__declspec(dllexport) CString& GetBmeSzsykErrMsg();

	// 部門枝番別特定収入
	class	CBMESPSYK	*m_pBmespsyk;
	__declspec(dllexport) int BmeSpsykOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void BmeSpsykClose(void);
	__declspec(dllexport) CString& GetBmeSpsykErrMsg();

	// 納付書（領収済通知書）
	class	CSHINPAYMENT	*m_pShinPayment;
	__declspec(dllexport) int ShinPayMentOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void ShinPayMentClose(void);
	__declspec(dllexport) CString& GetShinPayMentErrMsg();

	// 消費税の還付申告に関する明細書（基本情報）
	class CREFUNDVOL	*m_pRefundvol;
	__declspec(dllexport) int ReFundVolOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void ReFundVolClose(void);
	__declspec(dllexport) CString& GetReFundVolErrMsg();

	// 消費税の還付申告に関する明細書（次葉）
	class CREFUNDREC	*m_pRefundrec;
	__declspec(dllexport) int ReFundRecOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void ReFundRecClose(void);
	__declspec(dllexport) CString& GetReFundRecErrMsg();

	// 仕入控除税額に関する明細書の次葉
	class CSKJREC	*m_pSkjrec;
	__declspec(dllexport) int SkjrecOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void SkjrecClose(void);

	// 更正の請求書
	class CREASK	*m_pReask;
	__declspec(dllexport) int ReaskOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void ReaskClose(void);

	// 課税取引金額計算表
	class CKJSNK	*m_pKjsnk;
	__declspec(dllexport) int KjsnkOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void KjsnkClose(void);

	// 消費税申告書（金額）
	class CSH_LST_VAL	*m_pSH_LST_VAL;
	__declspec(dllexport) int OpenShLstVal( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void CloseShLstVal(void);

	// 消費税申告書（割合）
	class CSH_LST_RATE	*m_pSH_LST_RATE;
	__declspec(dllexport) int OpenShLstRate( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void CloseShLstRate(void);

//2018.03.13 INSERT START
	// 付表６ 
	class CFHYO60REC	*m_pFhyo60rec;
	__declspec(dllexport) int Fhyo60RecOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void Fhyo60RecClose(void);

	// 付表６　相続人
	class CSYZ_SOZOKU	*m_pSyz_Sozoku;
	__declspec(dllexport) int Syz_SozokuOpen( CDatabase*, LPCTSTR filter=_T("") );
	__declspec(dllexport) void Syz_SozokuClose(void);
//2018.03.13 INSERT END

    //　特例帳票
	class CSYZ_TOKUREI	*m_pSyz_Tokurei;
	__declspec(dllexport) int Syz_TokureiOpen(CDatabase*, LPCTSTR filter = _T(""));
	__declspec(dllexport) void Syz_TokureiClose(void);

    // 消費税集計サブテーブル
    class CSZSYKSUB *m_pSzsyksub;
	__declspec(dllexport) int SzsyksubOpen(CDatabase*, LPCTSTR filter = _T(""));
	__declspec(dllexport) void SzsyksubClose(void);
	__declspec(dllexport) CString& GetSzsyksubErrMsg();

    // 月別毎に税額計算種別取得関数 
	__declspec(dllexport) int GetSyzZeiCalqForEachMonth(CDBZmSub* pZmsub, CArray<ZeiCalqEachMonth, ZeiCalqEachMonth&>* pArray);

	// 月別毎に税額計算種別取得関数 
	__declspec(dllexport) int GetSyzZeiCalqForEachMonth2(CDBZmSub* pZmsub, CArray<ZeiCalqEachMonth, ZeiCalqEachMonth&>* pArray, const unsigned char Sn_TUMIAG);

    // 開始日から終了日に税額計算「積上げ計算」「割戻し計算」が混在しているかチェック.
	__declspec(dllexport) int CheckSyzZeiCalqSireKonzai(CDBZmSub* pZmsub, const int smd, const int emd, bool* bMixUriage, bool* bMixSire);

	//5億円判定
	//指定期間内の仕訳データを集計し,課税売上高が5億円を超えているかどうか判定する
	//またその集計金額を返す.
	//smd:指定期間開始日 emd:指定期間終了日(課税開始日が設定されている場合は課税開始日の前日までの集計)
    //syukei_money:集計金額(char syukei_money[6]のポインタ)
	__declspec(dllexport) bool IsOver5okuYen(CDBZmSub* pZmsub, long smd, long emd, char* syukei_money);

	//免税期間の課税売上高の集計額をSYZ_SNK_LST_VALに書き込み(合併マスター用)
	//dest:書き込み先の合併マスターの財務クラス、src：書き込み元マスターの財務クラス(どちらもボリュームラベルオープン済み)
	//srcCnt：書き込み元マスターのカウント(0...n)、0のときはレコードの金額を集計額で上書きする
	__declspec(dllexport) int SaveMenzeiUriage(CDBZmSub* dest, CDBZmSub* src, int srcCnt);
	__declspec(dllexport) CString& GetErrMsg();

	//char	Rsv0[256 - (11 * 4)];		// '19.06.13	256-(10*4) -> 256-(11*4)
	char	Rsv0[256 - (12 * 4)];		// '23.07.05	256-(11*4) -> 256-(12*4)

public:
	CString	m_ErrMsg;


	char	Rsv1[256];

private:

	int IsExist( const char *, CDatabase * );

	int MakePrfetaxTbl( const char *, const char *, CDatabase *, long );

	int VupFhyo60Rec( CDatabase* );
	int GetTblVersion(CDatabase*, LPCTSTR);

	int VupReaskRec( CDatabase* );
	int IsExistColumn( const char *, const char *, CDatabase * );
	int SetTableVersion( CDatabase *, const char *, long );
	int VupRefundRec( CDatabase* );

	int CreateTmpTable2(CDBZmSub* pZmsub, long sddate, long eddate, char*, char*);
	void GetDbDataSub(CDBZmSub* pZmsub, std::vector<SiwakeRec>& pSiwakeRecList, long sddate = 0, long eddate = 0);
	bool Syukei5okuHantei(CDBZmSub* pZmsub, std::vector<SiwakeRec>& pSiwakeRecList, char* syukei_money, char*, char*);
	bool Is5okuHanteiData(std::vector<SiwakeRec>::iterator itr, EnumIdKariKashiKbn type, char*, char*);
    int GetZeiCalqByDate(CDBZmSub* pZmsub, long ddate=0, int CalqKbn=0);
	int GetMenzeiUriage(CDBZmSub* pZmsub, char* dest);

	char	Rsv3[256];

};

