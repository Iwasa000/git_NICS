#pragma once

//-----------------------------------------------------------------------------
// 付表５データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH26SnFhyo50Data
{
public:
	unsigned char	Sn_5FURIK[MONY_BUF_SIZE],	// 事業区分別課税売上高合計額(期間計)

					Sn_5FURITA[MONY_BUF_SIZE],	// 事業区分別 課税売上高合計額(税率３％分)
					Sn_5FURITB[MONY_BUF_SIZE],	// 事業区分別 課税売上高合計額(税率４％分)
					Sn_5FURITC[MONY_BUF_SIZE];	// 事業区分別 課税売上高合計額(税率６.３％分)

	int				Sn_5FURITsw;				//[Y] 事業区分別 課税売上高合計額 手入力サイン

	unsigned char	Sn_5FURI1A[MONY_BUF_SIZE],	//         〃　　　　第一種(税率３％分)
					Sn_5FURI2A[MONY_BUF_SIZE],	//         〃　　　　第二種　  　〃	
					Sn_5FURI3A[MONY_BUF_SIZE],	//         〃　　　　第三種　  　〃	
					Sn_5FURI4A[MONY_BUF_SIZE],	//         〃　　　　第四種　  　〃	
					Sn_5FURI5A[MONY_BUF_SIZE];	//         〃　　　　第五種　  　〃	

	unsigned char	Sn_5FURI1B[MONY_BUF_SIZE],	//         〃　　　　第一種(税率４％分)
					Sn_5FURI2B[MONY_BUF_SIZE],	//         〃　　　　第二種　  　〃	
					Sn_5FURI3B[MONY_BUF_SIZE],	//         〃　　　　第三種　  　〃	
					Sn_5FURI4B[MONY_BUF_SIZE],	//         〃　　　　第四種　  　〃	
					Sn_5FURI5B[MONY_BUF_SIZE];	//         〃　　　　第五種　  　〃	

	unsigned char	Sn_5FURI1C[MONY_BUF_SIZE],	//         〃　　　　第一種(税率６.３％分)
					Sn_5FURI2C[MONY_BUF_SIZE],	//         〃　　　　第二種　  　〃	
					Sn_5FURI3C[MONY_BUF_SIZE],	//         〃　　　　第三種　  　〃	
					Sn_5FURI4C[MONY_BUF_SIZE],	//         〃　　　　第四種　　  〃	
					Sn_5FURI5C[MONY_BUF_SIZE];	//         〃　　　　第五種　 　 〃	

	unsigned char	Sn_5FUZKTA[MONY_BUF_SIZE],	// 事業区分別 消費税額合計額(税率３％分)
					Sn_5FUZK1A[MONY_BUF_SIZE],	//         〃　　　　第一種　　　〃	
					Sn_5FUZK2A[MONY_BUF_SIZE],	//         〃　　　　第二種　　　〃	
					Sn_5FUZK3A[MONY_BUF_SIZE],	//         〃　　　　第三種　　　〃	
					Sn_5FUZK4A[MONY_BUF_SIZE],	//         〃　　　　第四種　　　〃	
					Sn_5FUZK5A[MONY_BUF_SIZE];	//         〃　　　　第五種　　　〃	

	unsigned char	Sn_5FUZKTB[MONY_BUF_SIZE],	//         〃　　　　合計額(税率４％分)
					Sn_5FUZK1B[MONY_BUF_SIZE],	//         〃　　　　第一種　　　〃	
					Sn_5FUZK2B[MONY_BUF_SIZE],	//         〃　　　　第二種　　　〃	
					Sn_5FUZK3B[MONY_BUF_SIZE],	//         〃　　　　第三種　　　〃	
					Sn_5FUZK4B[MONY_BUF_SIZE],	//         〃　　　　第四種　　　〃	
					Sn_5FUZK5B[MONY_BUF_SIZE];	//         〃　　　　第五種　　　〃	

	unsigned char	Sn_5FUZKTC[MONY_BUF_SIZE],	//         〃　　　　合計額(税率６.３％分)
					Sn_5FUZK1C[MONY_BUF_SIZE],	//         〃　　　　第一種　　　〃	
					Sn_5FUZK2C[MONY_BUF_SIZE],	//         〃　　　　第二種　　　〃	
					Sn_5FUZK3C[MONY_BUF_SIZE],	//         〃　　　　第三種　　　〃	
					Sn_5FUZK4C[MONY_BUF_SIZE],	//         〃　　　　第四種　　　〃	
					Sn_5FUZK5C[MONY_BUF_SIZE];	//         〃　　　　第五種　　　〃	

	int				Sn_5FUZK1sw,				//[Y] 事業区分別 消費税 手入力サイン
					Sn_5FUZK2sw,
					Sn_5FUZK3sw,
					Sn_5FUZK4sw,
					Sn_5FUZK5sw;
	int				Sn_5FUZKTsw;				//[Y] 事業区分別 消費税額合計額 手入力サイン


	unsigned char	Sn_5FUZTT[MONY_BUF_SIZE],	// 事業区分別 消費税額合計額(課税期間計)	
					Sn_5FUZT1[MONY_BUF_SIZE],	//         〃　　　　第一種　　  〃	
					Sn_5FUZT2[MONY_BUF_SIZE],	//         〃　　　　第二種　　　〃	
					Sn_5FUZT3[MONY_BUF_SIZE],	//         〃　　　　第三種　　　〃	
					Sn_5FUZT4[MONY_BUF_SIZE],	//         〃　　　　第四種　　　〃	
					Sn_5FUZT5[MONY_BUF_SIZE];	//         〃　　　　第五種　　　〃	

	unsigned char	Sn_5FSZTG[MONY_BUF_SIZE],	// 控除対象仕入税額　原則計算(期間計)
					Sn_5FSZKGA[MONY_BUF_SIZE],	//         〃　　　　原則計算(税率３％分)	
					Sn_5FSZKGB[MONY_BUF_SIZE],	//         〃　　　　原則計算(税率４％分)	
					Sn_5FSZKGC[MONY_BUF_SIZE];	//         〃　　　　原則計算(税率６.３％分)	
	int				Sn_5FGKsw;					// 原則計算⑱手入力サインD0:期間  D4:うち

	unsigned char	Sn_5FSZTT[MONY_BUF_SIZE],	//         〃　　　　特例計算(期間計)
					Sn_5FSZKTA[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZKTB[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZKTC[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT19sw;				// 手入力サイン

//--> '14.02.25
	unsigned char	Sn_5FSZT20T[MONY_BUF_SIZE],	// 20      〃　　　　特例計算(期間計)
					Sn_5FSZT20A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT20B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT20C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT20sw;				// 手入力サイン

	unsigned char	Sn_5FSZT21T[MONY_BUF_SIZE],	// 21      〃　　　　特例計算(期間計)
					Sn_5FSZT21A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT21B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT21C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT21sw;				// 手入力サイン

	unsigned char	Sn_5FSZT22T[MONY_BUF_SIZE],	// 22      〃　　　　特例計算(期間計)
					Sn_5FSZT22A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT22B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT22C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT22sw;				// 手入力サイン

	unsigned char	Sn_5FSZT23T[MONY_BUF_SIZE],	// 23      〃　　　　特例計算(期間計)
					Sn_5FSZT23A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT23B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT23C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT23sw;				// 手入力サイン

	unsigned char	Sn_5FSZT24T[MONY_BUF_SIZE],	// 24      〃　　　　特例計算(期間計)
					Sn_5FSZT24A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT24B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT24C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT24sw;				// 手入力サイン

	unsigned char	Sn_5FSZT25T[MONY_BUF_SIZE],	// 25      〃　　　　特例計算(期間計)
					Sn_5FSZT25A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT25B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT25C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT25sw;				// 手入力サイン

	unsigned char	Sn_5FSZT26T[MONY_BUF_SIZE],	// 26      〃　　　　特例計算(期間計)
					Sn_5FSZT26A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT26B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT26C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT26sw;				// 手入力サイン

	unsigned char	Sn_5FSZT27T[MONY_BUF_SIZE],	// 27      〃　　　　特例計算(期間計)
					Sn_5FSZT27A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT27B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT27C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT27sw;				// 手入力サイン

	unsigned char	Sn_5FSZT28T[MONY_BUF_SIZE],	// 28      〃　　　　特例計算(期間計)
					Sn_5FSZT28A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT28B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT28C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT28sw;				// 手入力サイン

	unsigned char	Sn_5FSZT29T[MONY_BUF_SIZE],	// 29      〃　　　　特例計算(期間計)
					Sn_5FSZT29A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT29B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT29C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT29sw;				// 手入力サイン
//<-- '14.02.25



	unsigned short	Sn_5FUP1,					// 第一種事業　売り上げ割合(税率４％分)
					Sn_5FUP2,					// 第二種事業　　　　　　〃	
					Sn_5FUP3,					// 第三種事業　　　　　　〃	
					Sn_5FUP4,					// 第四種事業　　　　　　〃	
					Sn_5FUP5;					// 第五種事業　　　　　　〃	

	int				Sn_5FSZsw;					// 特例計算適用SW 0:原則計算 1:一種類特例
												// 2:第１+第２, 3:第1+第３, 4:第１+第４ 
												// 5:第１+第５, 6:第２+第３, 7:第２+第４
												// 8:第２+第５, 9:第３+第４,10:第３+第５
												// 11:第４+第５	
	int				Sn_5FSBsw;					// 特例計算適用SW (税率4%分)
	int				Sn_5FCQsw;					// 集計・計算済サイン


protected:
	char			rsv1[512];


private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	BOOL			m_isTransitionalMeasure;	// 経過措置あり？
	CArithEx		m_Arith;

protected:
	char			rsv2[256];

public:
	CH26SnFhyo50Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH26SnFhyo50Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );
	// 現行データ構造からのデータの引継ぎ
	int GetPastData( CDBNpSub *pDBNpSub, int sn_seq );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
	// 横計ではない例外計算が必要？ ('14.10.17)
	BOOL IsNeedExpCalq();
};
