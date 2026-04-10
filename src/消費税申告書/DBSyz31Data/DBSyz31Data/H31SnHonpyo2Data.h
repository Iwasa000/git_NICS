#pragma once

//=========================================
// 本表2データ（平成31年改正)
//=========================================

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnHonpyo2Data
{
public:
	char*					rsv;

	unsigned char			Sn_XM1[MONY_BUF_SIZE],		// 課税標準額

							Sn_XM2[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額の合計額 3%適用分
							Sn_XM3[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額の合計額 4%適用分
							Sn_XM4[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額の合計額 6.3%適用分
							Sn_XM5[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額の合計額 6.24%適用分
							Sn_XM6[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額の合計額 7.8%適用分
							Sn_XM7[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額の合計額

							Sn_XM8[MONY_BUF_SIZE],		// 特定課税仕入れに係る支払い対価の額の合計額 6.3%適用分
							Sn_XM9[MONY_BUF_SIZE],		// 特定課税仕入れに係る支払い対価の額の合計額 7.8%適用分
							Sn_XM10[MONY_BUF_SIZE],		// 特定課税仕入れに係る支払い対価の額の合計額

							Sn_XM11[MONY_BUF_SIZE],		// 消費税額

							Sn_XM12[MONY_BUF_SIZE],		// 11の内訳 3%適用分
							Sn_XM13[MONY_BUF_SIZE],		// 11の内訳 4%適用分
							Sn_XM14[MONY_BUF_SIZE],		// 11の内訳 6.3%適用分
							Sn_XM15[MONY_BUF_SIZE],		// 11の内訳 6.24%適用分
							Sn_XM16[MONY_BUF_SIZE],		// 11の内訳 7.8%適用分

							Sn_XM17[MONY_BUF_SIZE],		// 返還等対価に係る税額
							Sn_XM18[MONY_BUF_SIZE],		// 売上げの返還等対価に係る税額
							Sn_XM19[MONY_BUF_SIZE],		// 特定課税仕入れの返還等対価に係る税額

							Sn_XM20[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額
							Sn_XM21[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額 4%適用分
							Sn_XM22[MONY_BUF_SIZE],		// 地方消費税の課税標準となる消費税額 6.3%適用分
							Sn_XM23[MONY_BUF_SIZE];		// 地方消費税の課税標準となる消費税額 6.24%及び7.8%適用分

protected:
	char					rsv1[512];

private:
	CDBSyzAccess			m_DBSyzAccess;
	CSyzUtil				m_Util;

public:
	CH31SnHonpyo2Data();
	~CH31SnHonpyo2Data();

	// DBからデータ読込み
	int	GetData ( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書き込み
	int	SetData ( CDBNpSub *pDBNpSub, int sn_seq );
	// 更新時の金額クリア
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
};