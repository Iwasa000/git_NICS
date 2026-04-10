#pragma once

//-----------------------------------------------------------------------------
// 一般本表データ（平成31年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnHonpyoData
{
public:

	char*			rsv;

	unsigned char	Sn_KSTDM[MONY_BUF_SIZE],	// 課税標準額（自動車）　［ＢＩＮ１２桁］		szsnk->Sn_KSTDM
					Sn_KSTD[MONY_BUF_SIZE],		// 　　〃　　（その他）　［ＢＩＮ１２桁］		szsnk->Sn_KSTD			
					Sn_TIKAM[MONY_BUF_SIZE],	// 対価の返還等をした金額（自動車）［〃］		szsnk->Sn_TIKAM
					Sn_TIKA[MONY_BUF_SIZE],		// 　　　　　〃　　　　　（その他）［〃］		szsnk->Sn_TIKA
					Sn_SYTXM[MONY_BUF_SIZE],	// 消費税額（自動車）　　［ＢＩＮ１２桁］		szsnk->Sn_SYTXM
					Sn_SYTX[MONY_BUF_SIZE];		// 　　〃　（その他）　　［　　 〃 　　］		szsnk->Sn_SYTX
	int				Sn_SYTsw;					// 消費税額（その他）手入力サイン D0: D4:

	unsigned char	Sn_KJADJZ[MONY_BUF_SIZE],	// 控除過大調整税額・一般［　　 〃 　　］		szsnk->Sn_KJADJZ
												// 貸倒回収に係税額・簡易［　　 〃 　　］

					Sn_SIREZ[MONY_BUF_SIZE],	// 控除対象仕入税額　　　［　　 〃 　　］		szsnk->Sn_SIREZ
					Sn_HKANZ[MONY_BUF_SIZE],	// 返還等対価に係る税額　［　　 〃 　　］		szsnk->Sn_HKANZ
					Sn_KSITZ[MONY_BUF_SIZE],	// 貸倒れに係る税額　　　［　　 〃 　　］		szsnk->Sn_KSITZ

					Sn_TYNOFZ[MONY_BUF_SIZE],	// 中間納付税額（年一回）［　　 〃 　　］		szsnk->Sn_TYNOFZ
					Sn_EDNOFZ[MONY_BUF_SIZE],	// 最終納付税額　　　　　［　　 〃 　　］		szsnk->Sn_EDNOFZ
					Sn_KAKTIZ[MONY_BUF_SIZE],	// 既確定税額　　　　　　［　　 〃 　　］		szsnk->Sn_KAKTIZ
	
					Sn_TTYWAR[MONY_BUF_SIZE],	// 地方消費税　中間納付譲渡割額（年一回）		szsnk->Sn_TTYWAR
					Sn_TKAKTZ[MONY_BUF_SIZE],	//     〃　　　既確定譲渡割額					szsnk->Sn_TKAKTZ
					Sn_TEDNOF[MONY_BUF_SIZE],	//     〃　　　最終納付税額						szsnk->Sn_TEDNOF

					Sn_KZURI[MONY_BUF_SIZE],	// 課税売上高　　　　　　［　　 〃 　　］		szsnk->Sn_KZURI
					Sn_SOURI[MONY_BUF_SIZE];	// 総売上高　　　　　　　［　　 〃 　　］		szsnk->Sn_SOURI

	unsigned char	Sn_ZZNOFZ[MONY_BUF_SIZE],	// 前々回納付税額　　　　［　　 〃 　　］		szsnk->Sn_ZZNOFZ
					Sn_ZNOFZ[MONY_BUF_SIZE],	// 前回納付税額　　　　　［　　 〃 　　］		szsnk->Sn_ZNOFZ
//					Sn_WURI[MONY_BUF_SIZE],		// 課税売上高　　ＷＯＲＫ［　　 〃 　　］		szsnk->Sn_WURI			ヘッダテーブルへ！
					Sn_ZZURI[MONY_BUF_SIZE],	// 基準期間の課税売上高  ［　　 〃 　　］		szsnk->Sn_ZZURI
					Sn_ZURI[MONY_BUF_SIZE];		// 前年の課税売上高　　  ［　　 〃 　　］		szsnk->Sn_ZURI

	unsigned char	Sn_KURI1[MONY_BUF_SIZE],	// 第一種事業　課税売上　［　　 〃 　　］		szsnk->Sn_KURI[0]
					Sn_KURI2[MONY_BUF_SIZE],	// 第二種事業　課税売上　［　　 〃　　 ］		szsnk->Sn_KURI[1]
					Sn_KURI3[MONY_BUF_SIZE],	// 第三種事業　課税売上　［　　 〃 　　］		szsnk->Sn_KURI[2]
					Sn_KURI4[MONY_BUF_SIZE],	// 第四種事業　課税売上  ［　　 〃 　　］		szsnk->Sn_KURI[3]
					Sn_KURI5[MONY_BUF_SIZE],	// 第五種事業　課税売上  ［　　 〃 　　］		szsnk->Sn_KURI[4]
					Sn_KURI6[MONY_BUF_SIZE],	// 第六種事業　課税売上

					Sn_GENKAI[MONY_BUF_SIZE],	// 限界控除税額　　　　　［　　 〃 　　］		無し？？？
					Sn_WGENKA[MONY_BUF_SIZE];	// 限界控除税額(手入力額)［　　 〃 　　］		無し？？？


	unsigned short	Sn_UP1,						// 第一種事業　売上割合							szsnk->Sn_UP[0]
					Sn_UP2,						// 第二種事業　売上割合							szsnk->Sn_UP[1]
					Sn_UP3,						// 第三種事業　売上割合							szsnk->Sn_UP[2]
					Sn_UP4,						// 第四種事業　売上割合							szsnk->Sn_UP[3]
					Sn_UP5,						// 第五種事業　売上割合							szsnk->Sn_UP[4]
					Sn_UP6;						// 第六種事業　売上割合

	unsigned short	Sn_TS_KUBUN1;				// 1・2の内訳:地方消費税　区分
	unsigned char	Sn_TS_TDM1[MONY_BUF_SIZE],	// 1・2の内訳:地方消費税　課税標準額
					Sn_TS_SYT1[MONY_BUF_SIZE];	// 1・2の内訳:地方消費税　消費税額

	unsigned short	Sn_TS_KUBUN2;				// 17・18の内訳:地方消費税　区分
	unsigned char	Sn_TS_SYT2[MONY_BUF_SIZE];	// 17・18の内訳:地方消費税　課税標準額

	int				Sn_ZNOsw;					// 前回納付税額の情報が変更されているときのsw（予定申告にて変更されているかを見るため追加)


	unsigned char	Sn_KSJT3[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(3%)
					Sn_KSJT4[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(4%)
					Sn_KSJT63[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(6.3%)
					Sn_KSJTKEI[MONY_BUF_SIZE];	// 課税資産の譲渡等の対価の額(合計)

	unsigned char	Sn_TKST63[MONY_BUF_SIZE],	// 特定課税仕入に係る支払対価の額(6.3%)
					Sn_TKSTKEI[MONY_BUF_SIZE];	// 特定課税仕入に係る支払対価の額(合計)

	unsigned char	Sn_URHKNZ[MONY_BUF_SIZE],	// 売上の返還等対価に係る税額
					Sn_TKHKNZ[MONY_BUF_SIZE];	// 特定課税仕入の返還等対価に係る税額


	unsigned char	Sn_KJZSK[MONY_BUF_SIZE],	// 控除税額小計
					Sn_KJHKZ[MONY_BUF_SIZE],	// 控除不足還付税額
					Sn_SHZ[MONY_BUF_SIZE],		// 差引税額
					Sn_TYNKNP[MONY_BUF_SIZE],	// 中間納付還付税額
					Sn_SHN[MONY_BUF_SIZE],		// 差引納付税額
					Sn_TKJHKZ[MONY_BUF_SIZE],	// 控除不足還付税額
					Sn_TSHZ[MONY_BUF_SIZE],		// 差引税額
					Sn_KNP[MONY_BUF_SIZE],		// 還付額
					Sn_NOZ[MONY_BUF_SIZE],		// 納税額
					Sn_TYNKNPJ[MONY_BUF_SIZE],	// 中間納付還付譲渡割額
					Sn_SHNJ[MONY_BUF_SIZE],		// 差引納付譲渡割額
					Sn_SYOTSG[MONY_BUF_SIZE];	// 消費税及び地方消費税の合計(納付又は還付)税額

	unsigned char	Sn_KazeiUriage[MONY_BUF_SIZE];	// 課税期間における課税売上高

protected:
	//char			rsv1[512];
	char			rsv1[506];	// 2023.08.10	Sn_KazeiUriage追加

private:
	CDBSyzAccess			m_DBSyzAccess;
	CSyzUtil				m_Util;

protected:
	char			rsv2[256];


public:
	CH31SnHonpyoData();
	virtual ~CH31SnHonpyoData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype=0 );
	// 旧様式のデータ取得
	int GetPastData ( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData );
	// 別バージョンからのデータの引き継ぎ
	int TakeOverData( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData, int srcVer );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
	// 更新時の金額追加書き込み
	int SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq);

	// 集計済みデータのセット
	int SetCalqedData(CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei);

	// 最終データとして、どちらのデータが残っている？
	int ChkExistKzdata(CDBNpSub *pDBNpSub, int sn_seq, int& Kzhoushiki);
	// DB からデータ読込み
	int GetDataExtend( CDBNpSub *pDBNpSub, int sn_seq, int rdtype=0 );

protected:
	// DB からデータ読込み（本体）
	int GetDataSub(CDBNpSub *pDBNpSub, int sn_seq, int KzHoushiki);
	// 更新時の金額追加書き込み（免税）
	int SetMenzeiDataInRenew(CDBNpSub *pDBNpSub, int sn_seq);
	// 更新時の金額書き込み（原則課税）
	int SetGensokuDataInRenew(CDBNpSub *pDBNpSub, int sn_seq);
	// 更新時の金額書き込み（簡易課税）
	int SetKanniDataInRenew(CDBNpSub *pDBNpSub, int sn_seq);
};