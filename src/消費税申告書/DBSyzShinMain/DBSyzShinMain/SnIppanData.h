#pragma once

class CSnIppanData
{
public:
	unsigned char	Sn_KSTDM[6],	// 課税標準額（自動車）　［ＢＩＮ１２桁］		szsnk->Sn_KSTDM
					Sn_KSTD[6],		// 　　〃　　（その他）　［ＢＩＮ１２桁］		szsnk->Sn_KSTD			
					Sn_TIKAM[6],	// 対価の返還等をした金額（自動車）［〃］		szsnk->Sn_TIKAM
					Sn_TIKA[6],		// 　　　　　〃　　　　　（その他）［〃］		szsnk->Sn_TIKA
					Sn_SYTXM[6],	// 消費税額（自動車）　　［ＢＩＮ１２桁］		szsnk->Sn_SYTXM
					Sn_SYTX[6],		// 　　〃　（その他）　　［　　 〃 　　］		szsnk->Sn_SYTX

					Sn_KJADJZ[6],	// 控除過大調整税額・一般［　　 〃 　　］		szsnk->Sn_KJADJZ
					             	// 貸倒回収に係税額・簡易［　　 〃 　　］

					Sn_SIREZ[6],	// 控除対象仕入税額　　　［　　 〃 　　］		szsnk->Sn_SIREZ
					Sn_HKANZ[6],	// 返還等対価に係る税額　［　　 〃 　　］		szsnk->Sn_HKANZ
					Sn_KSITZ[6],	// 貸倒れに係る税額　　　［　　 〃 　　］		szsnk->Sn_KSITZ

					Sn_TYNOFZ[6],	// 中間納付税額（年一回）［　　 〃 　　］		szsnk->Sn_TYNOFZ
					Sn_EDNOFZ[6],	// 最終納付税額　　　　　［　　 〃 　　］		szsnk->Sn_EDNOFZ
					Sn_KAKTIZ[6],	// 既確定税額　　　　　　［　　 〃 　　］		szsnk->Sn_KAKTIZ

					Sn_TTYWAR[6],	// 地方消費税　中間納付譲渡割額（年一回）		szsnk->Sn_TTYWAR
					Sn_TKAKTZ[6],	//     〃　　　既確定譲渡割額					szsnk->Sn_TKAKTZ
					Sn_TEDNOF[6],	//     〃　　　最終納付税額						szsnk->Sn_TEDNOF

					Sn_KZURI[6],	// 課税売上高　　　　　　［　　 〃 　　］		szsnk->Sn_KZURI
					Sn_SOURI[6],	// 総売上高　　　　　　　［　　 〃 　　］		szsnk->Sn_SOURI

					Sn_ZZNOFZ[6],	// 前々回納付税額　　　　［　　 〃 　　］		szsnk->Sn_ZZNOFZ
					Sn_ZNOFZ[6],	// 前回納付税額　　　　　［　　 〃 　　］		szsnk->Sn_ZNOFZ
					Sn_WURI[6],		// 課税売上高　　ＷＯＲＫ［　　 〃 　　］		szsnk->Sn_WURI
					Sn_ZZURI[6],	// 基準期間の課税売上高  ［　　 〃 　　］		szsnk->Sn_ZZURI
					Sn_ZURI[6],		// 前年の課税売上高　　  ［　　 〃 　　］		szsnk->Sn_ZURI

					Sn_KURI1[6],	// 第一種事業　課税売上　［　　 〃 　　］		szsnk->Sn_KURI[0]
					Sn_KURI2[6],	// 第二種事業　課税売上　［　　 〃　　 ］		szsnk->Sn_KURI[1]
					Sn_KURI3[6],	// 第三種事業　課税売上　［　　 〃 　　］		szsnk->Sn_KURI[2]
					Sn_KURI4[6],	// 第四種事業　課税売上  ［　　 〃 　　］		szsnk->Sn_KURI[3]
					Sn_KURI5[6],	// 第五種事業　課税売上  ［　　 〃 　　］		szsnk->Sn_KURI[4]

					Sn_GENKAI[6],	// 限界控除税額　　　　　［　　 〃 　　］		無し？？？
					Sn_WGENKA[6];	// 限界控除税額(手入力額)［　　 〃 　　］		無し？？？

	unsigned short	Sn_UP1,			// 第一種事業　売上割合							szsnk->Sn_UP[0]
					Sn_UP2,			// 第二種事業　売上割合							szsnk->Sn_UP[1]
					Sn_UP3,			// 第三種事業　売上割合							szsnk->Sn_UP[2]
					Sn_UP4,			// 第四種事業　売上割合							szsnk->Sn_UP[3]
					Sn_UP5;			// 第五種事業　売上割合							szsnk->Sn_UP[4]

public:
	// DB からデータ読込み
	void SetData( CDBNpSub *pDBNpSub, int sn_seq );

private:
	CDBSyzAccess	m_DBSyzAccess;
	CUtil			m_Util;
};