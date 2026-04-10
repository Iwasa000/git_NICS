#pragma once

//-----------------------------------------------------------------------------
// 付表５データ（平成27年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH27SnFhyo50Data : public CH26SnFhyo50Data
{
public:
	unsigned char	Sn_5FURI6A[MONY_BUF_SIZE];	// 事業区分別課税売上高 第六種(税率３％分)
	unsigned char	Sn_5FURI6B[MONY_BUF_SIZE];	//         〃　　　　   第六種(税率４％分)
	unsigned char	Sn_5FURI6C[MONY_BUF_SIZE];	//         〃　　　　   第六種(税率６.３％分)

	unsigned char	Sn_5FUZK6A[MONY_BUF_SIZE];	// 事業区分別 消費税額  第六種(税率３％分)
	unsigned char	Sn_5FUZK6B[MONY_BUF_SIZE];	//         〃　　　　   第六種(税率４％分)
	unsigned char	Sn_5FUZK6C[MONY_BUF_SIZE];	//         〃　　　　   第六種(税率６.３％分)

	int				Sn_5FUZK6sw;				// 事業区分別 消費税 手入力サイン 第六種
	int				Sn_5FTKsw;					// 特例計算21 手入力サイン

	unsigned char	Sn_5FUZT6[MONY_BUF_SIZE];	// 事業区分別 消費税額  第六種(課税期間計)


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

	unsigned char	Sn_5FSZT30T[MONY_BUF_SIZE],	// 30      〃　　　　特例計算(期間計)
					Sn_5FSZT30A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT30B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT30C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT30sw;				// 手入力サイン

	unsigned char	Sn_5FSZT31T[MONY_BUF_SIZE],	// 31      〃　　　　特例計算(期間計)
					Sn_5FSZT31A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT31B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT31C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT31sw;				// 手入力サイン

	unsigned char	Sn_5FSZT32T[MONY_BUF_SIZE],	// 32      〃　　　　特例計算(期間計)
					Sn_5FSZT32A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT32B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT32C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT32sw;				// 手入力サイン

	unsigned char	Sn_5FSZT33T[MONY_BUF_SIZE],	// 33      〃　　　　特例計算(期間計)
					Sn_5FSZT33A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT33B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT33C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT33sw;				// 手入力サイン

	unsigned char	Sn_5FSZT34T[MONY_BUF_SIZE],	// 34      〃　　　　特例計算(期間計)
					Sn_5FSZT34A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT34B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT34C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT34sw;				// 手入力サイン

	unsigned char	Sn_5FSZT35T[MONY_BUF_SIZE],	// 35      〃　　　　特例計算(期間計)
					Sn_5FSZT35A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT35B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT35C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT35sw;				// 手入力サイン

	unsigned char	Sn_5FSZT36T[MONY_BUF_SIZE],	// 36      〃　　　　特例計算(期間計)
					Sn_5FSZT36A[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率３％分)	
					Sn_5FSZT36B[MONY_BUF_SIZE],	//         〃　　　　特例計算(税率４％分)	
					Sn_5FSZT36C[MONY_BUF_SIZE];	//         〃　　　　特例計算(税率６.３％分)
	int				Sn_5FSZT36sw;				// 手入力サイン

	unsigned short	Sn_5FUP6;					// 第六種事業　売り上げ割合(税率４％分)


//	int				Sn_5FSZsw;					// 特例計算適用SW 0:原則計算 1:一種類特例
												// 2:第１+第２, 3:第1+第３, 4:第１+第４ 
												// 5:第１+第５, 6:第２+第３, 7:第２+第４
												// 8:第２+第５, 9:第３+第４,10:第３+第５
												// 11:第４+第５	
//	int				Sn_5FSBsw;					// 特例計算適用SW (税率4%分)
//	int				Sn_5FCQsw;					// 集計・計算済サイン


protected:
	char			rsv3[512];


private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	BOOL			m_isTransitionalMeasure;	// 経過措置あり？
	CArithEx		m_Arith;

protected:
	char			rsv4[256];

public:
	CH27SnFhyo50Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH27SnFhyo50Data();

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
	// 横計ではない例外計算が必要？ ('15.05.07)
	BOOL IsNeedExpCalq();
};
