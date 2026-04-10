// TISW修正[22-0676] 2026/03/19 START
#include "stdafx.h"
#include "SkjUserSettings.h"
#include <unordered_set>

// owntbに登録する依頼[22-0676]対応済みフラグのキー
static const CString FIXED_22_0676 = _T("FIXED_22-0676");

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pZmSub">公益クラス</param>
/// <param name="currentUser">ログインユーザー情報</param>
/// <param name="pTblhdl">テーブルコンテナ</param>
/// <param name="pSkjCnvtbl">還付申告明細用連動情報のテーブル情報</param>
SkjUserSettingsAccessor::SkjUserSettingsAccessor(CDBNpSub* pZmSub, mctl_usertbl& currentUser, CTblhdl* pTblhdl, _TB_PAR* pSkjCnvtbl)
	: m_pZmSub{ pZmSub }
	, m_currentUser{ currentUser }
	, m_pTblhdl{ pTblhdl }
	, m_pSkjCnvtbl{ pSkjCnvtbl }
{
	if (!pZmSub->owntb)
	{
		pZmSub->OwnTblOpen();
		m_isOwnTbOpend = true;
	}
}

SkjUserSettingsAccessor::~SkjUserSettingsAccessor()
{
	if (m_isOwnTbOpend)
	{
		m_pZmSub->OwnTblClose();
	}
}

/// <summary>
/// owntbから還付申告明細用連動情報の設定を取得する
/// </summary>
/// <param name="parent">親ウィンドウ</param>
/// <param name="isBulkPrintMode">一括印刷モードか</param>
/// <param name="isUnconfirmed">(out)未確定の設定か</param>
/// <returns></returns>
std::vector<COWNTB> SkjUserSettingsAccessor::GetSkjSettings(CWnd* parent, bool isBulkPrintMode, bool& isUnconfirmed)
{
	isUnconfirmed = false;

	// バージョンアップ処理
	VersionUp_22_0676();

	// 全ユーザーのデータを取得
	auto idToSettings = GetAllSkjSettings();
	bool isUserFiltered = false;
	// 全ユーザーの設定が一致するか判定
	if (idToSettings.size() > 1 && IsAllUserSettingsSame(idToSettings))
	{
		// 一致する場合は代表して先頭ユーザーの設定を利用
		auto it = ++idToSettings.begin();
		while (it != idToSettings.end())
		{
			it = idToSettings.erase(it);
		}
		isUserFiltered = true;
	}
	// 含まれるユーザーの情報を取得
	SkjUserSelectDlg::InitialParam dlgInitParam{};
	dlgInitParam.m_users = GetSkjRegisteredUserInfos(idToSettings);
	if (dlgInitParam.m_users.size() <= 1)
	{
		// 登録のある利用者が1人以下の場合は利用者選択は不要
		std::vector<COWNTB> settings{};
		if (dlgInitParam.m_users.size() == 1)
		{
			int id = dlgInitParam.m_users.begin()->user_id;
			if (id != NULL_UID || isUserFiltered)
			{
				// 仕様変更前に利用していたユーザーが一人だった場合は固定のIDで更新しておく
				Confirm(id);
			}
			settings = idToSettings[id];
		}

		return settings;
	}

	// 一括印刷モードで2人以上の登録がある場合は利用者選択ダイアログをスキップして旧仕様で動作させる
	if (isBulkPrintMode)
	{
		return GetOldVerSettings();
	}

	// デフォルト設定から変更していないユーザーIDのリストを取得する
	dlgInitParam.m_defSettingUsers = GetDefaultSettingUsers(idToSettings);

	// idToSettingsの全要素をvectorに移動
	std::vector<COWNTB> settings{};
	for (auto& elem : idToSettings)
	{
		settings.insert(settings.end(), std::make_move_iterator(elem.second.begin()), std::make_move_iterator(elem.second.end()));
	}

	// 利用者選択ダイアログを表示して結果を処理する
	ShowUserSelectDialog(parent, std::move(dlgInitParam), settings, isUnconfirmed);

	return settings;
}

/// <summary>
/// owntbに還付申告明細用連動情報の設定を保存する
/// </summary>
void SkjUserSettingsAccessor::SaveSkjSettings()
{
	// 共通設定(ユーザーIDがNULL)を削除
	CString templateSql = _T("DELETE FROM owntb WHERE apl_name = '%s' AND itm_name = '%s' AND u_id IS NULL");
	CString sql{};
	if (IsSkj52())
	{
		sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ52_ITMNAME_2);
	}
	else
	{
		sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ_ITMNAME_2);
	}
	m_pZmSub->m_database->ExecuteSQL(sql);

	// 共通設定(ユーザーIDがNULL)として保存
	int cnt = 0;
	SH_SKJ_CNV	SkjCnv;
	CSyzUtil util{};
	while (1)
	{
		if (m_pTblhdl->th_rget(&SkjCnv, m_pSkjCnvtbl, cnt)) break;
		m_pZmSub->owntb->AddNew();
		m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
		if (IsSkj52())
			m_pZmSub->owntb->itm_name = SKJ52_ITMNAME_2;
		else	m_pZmSub->owntb->itm_name = SKJ_ITMNAME_2;
		m_pZmSub->owntb->itm_seq = SkjCnv.SKJSeq;
		m_pZmSub->owntb->u_id = NULL_UID;
		util.char_to_cstring(&m_pZmSub->owntb->code[0], SkjCnv.SKJCod, 8);
		m_pZmSub->owntb->vl[0] = SkjCnv.HJNCod;
		m_pZmSub->owntb->vl[1] = SkjCnv.KJNCod;
		m_pZmSub->owntb->Update();
		cnt++;
	}
}

/// <summary>
/// 依頼[22-0676]対応後の初回実行時にバージョンアップ処理を行う
/// </summary>
void SkjUserSettingsAccessor::VersionUp_22_0676()
{
	// owntbでバージョンアップ状態を管理する
	CString filter;
	filter.Format(OWNTBL_SQL, DBSYZSHIN_APLNAME, FIXED_22_0676);
	if (m_pZmSub->owntb->Requery(filter) != ERR)
	{
		// バージョンアップ処理済み
		return;
	}

	m_pZmSub->m_database->BeginTrans();
	try
	{
		// 既存キーに"_2"を付与したowntbレコードを複製する
		CString templateSql =
			_T("INSERT INTO owntb ( ")
			_T("  apl_name, ")
			_T("  itm_name, ")
			_T("  itm_seq, ")
			_T("  u_id, ")
			_T("  vl1, ")
			_T("  vl2, ")
			_T("  code1 ")
			_T(") ")
			_T("SELECT ")
			_T("  apl_name, ")
			_T("  CONCAT(RTRIM(itm_name), '_2'), ")
			_T("  itm_seq, ")
			_T("  u_id, ")
			_T("  vl1, ")
			_T("  vl2, ")
			_T("  code1 ")
			_T("FROM ")
			_T("  owntb ")
			_T("WHERE ")
			_T("  apl_name = '%s' ")
			_T("  AND itm_name IN ('%s', '%s') ");
		CString sql{};
		sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ_ITMNAME, SKJ52_ITMNAME);
		m_pZmSub->m_database->ExecuteSQL(sql);

		// バージョンアップ済みフラグの登録
		templateSql =
			_T("INSERT INTO owntb ( ")
			_T("  apl_name, ")
			_T("  itm_name, ")
			_T("  itm_seq ")
			_T(") VALUES ( ")
			_T("  '%s', ")
			_T("  '%s', ")
			_T("  1 ")
			_T(") ");
		sql.Format(templateSql, DBSYZSHIN_APLNAME, FIXED_22_0676);
		m_pZmSub->m_database->ExecuteSQL(sql);

		m_pZmSub->m_database->CommitTrans();
	}
	catch (...)
	{
		m_pZmSub->m_database->Rollback();
		throw;
	}
}

/// <summary>
/// owntbから還付申告明細用連動情報の設定を取得する(全利用者)
/// </summary>
/// <returns>ユーザーID -> 設定リスト</returns>
std::unordered_map<int, std::vector<COWNTB>> SkjUserSettingsAccessor::GetAllSkjSettings()
{
	CString		filter;
	if (IsSkj52())
	{
		filter.Format(OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ52_ITMNAME_2);
	}
	else
	{
		filter.Format(OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ_ITMNAME_2);
	}

	std::unordered_map<int, std::vector<COWNTB>> idToSettings{};
	m_pZmSub->owntb->Requery(filter);
	int moveResult = m_pZmSub->owntb->MoveFirst();
	while (moveResult != ERR)
	{
		if (idToSettings.find(m_pZmSub->owntb->u_id) == idToSettings.end())
		{
			idToSettings.insert({ m_pZmSub->owntb->u_id, std::vector<COWNTB>{} });
		}
		idToSettings[m_pZmSub->owntb->u_id].push_back(*m_pZmSub->owntb);
		moveResult = m_pZmSub->owntb->MoveNext();
	}

	return idToSettings;
}

/// <summary>
/// デフォルト設定から変更していないユーザーIDのリストを取得する
/// </summary>
/// <param name="settings">ユーザーID -> 還付申告明細用連動情報の設定リスト</param>
/// <returns></returns>
std::unordered_set<int> SkjUserSettingsAccessor::GetDefaultSettingUsers(const std::unordered_map<int, std::vector<COWNTB>>& idToSettings)
{
	// 科目コード -> SH_SKJ_CNVのハッシュを作成
	CMap<CString, LPCTSTR, SH_SKJ_CNV, SH_SKJ_CNV> skjCnvHash{};
	SH_SKJ_CNV	SkjCnv;
	CSyzUtil util{};
	for (int i = 0; i < m_pSkjCnvtbl->tp_rnum; ++i)
	{
		m_pTblhdl->th_rget(&SkjCnv, m_pSkjCnvtbl, i);
		CString key;
		util.char_to_cstring(&key, SkjCnv.SKJCod, 8);
		skjCnvHash.SetAt(key, SkjCnv);
	}

	// デフォルト設定と完全一致するユーザーIDを取得
	std::unordered_set<int> defSettingUsers{};
	for (auto& elem : idToSettings)
	{
		bool changed{ false };
		for (auto& setting : elem.second)
		{
			auto* pSkjCnv = skjCnvHash.PLookup(setting.code[0]);
			if (!pSkjCnv)
			{
				continue;
			}

			if (pSkjCnv->value.HJNCod != setting.vl[0])
			{
				changed = true;
				break;
			}
			if (pSkjCnv->value.KJNCod != setting.vl[1])
			{
				changed = true;
				break;
			}
		}

		if (!changed)
		{
			defSettingUsers.insert(elem.first);
		}
	}

	return defSettingUsers;
}

/// <summary>
/// 全ユーザーの設定が一致するか
/// </summary>
/// <param name="settings">ユーザーID -> 還付申告明細用連動情報の設定リスト</param>
/// <returns></returns>
bool SkjUserSettingsAccessor::IsAllUserSettingsSame(const std::unordered_map<int, std::vector<COWNTB>>& idToSettings)
{
	if (idToSettings.size() <= 1)
	{
		return true;
	}

	// codeでソートしてから全件比較する
	auto compare = [](COWNTB& l, COWNTB& r) { return l.code[0] < r.code[0]; };
	std::vector<COWNTB> topUserSettings = idToSettings.begin()->second;
	std::sort(topUserSettings.begin(), topUserSettings.end(), compare);
	for (auto it = ++idToSettings.begin(); it != idToSettings.end(); ++it)
	{
		std::vector<COWNTB> currentSettings = it->second;
		if (topUserSettings.size() != currentSettings.size())
		{
			return false;
		}

		std::sort(currentSettings.begin(), currentSettings.end(), compare);
		for (int i = 0; i < topUserSettings.size(); ++i)
		{
			auto& topElem = topUserSettings[i];
			auto& curElem = currentSettings[i];
			if (topElem.code[0] != curElem.code[0]) return false;
			if (topElem.vl[0] != curElem.vl[0]) return false;
			if (topElem.vl[1] != curElem.vl[1]) return false;
		}
	}

	return true;
}

/// <summary>
/// 還付申告明細用連動情報のデータソースに含まれるユーザー情報のリストを取得する
/// </summary>
/// <param name="settings">ユーザーID -> 還付申告明細用連動情報の設定リスト</param>
/// <returns></returns>
/// <remarks>1人のみの場合はuser_idのみセットして返す(DBアクセスしない)</remarks>
std::vector<mctl_usertbl> SkjUserSettingsAccessor::GetSkjRegisteredUserInfos(const std::unordered_map<int, std::vector<COWNTB>>& idToSettings)
{
	std::unordered_set<int> userIds{};
	for (auto& elem : idToSettings)
	{
		userIds.insert(elem.first);
	}

	std::vector<mctl_usertbl> userInfos{};
	if (userIds.size() == 0)
	{
		return userInfos;
	}
	if (userIds.size() == 1)
	{
		// 1人のみの場合はuser_idのみセットして返す(DBアクセスしない)
		mctl_usertbl info{};
		info.user_id = *userIds.begin();
		userInfos.push_back(info);
		return userInfos;
	}

	// usertblの情報を取得
	char	sqlsvr[128] = { 0 }, apno[32] = { 0 }, cocode[32] = { 0 }, kikan[32] = { 0 };
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan);
	n_apno = atoi(apno);
	n_cocode = atoi(cocode);
	n_kikan = atoi(kikan);

	ICSMCTL		icsmctl;
	if (icsmctl.SetDatabase(sqlsvr) != 0)
	{
		throw std::exception{ "Failed ICSMCTL::SetDatabase" };
	}
	USERTBL_Array userTblArray;
	if (icsmctl.get_userarry(&userTblArray) != 0)
	{
		throw std::exception{ "Failed ICSMCTL::get_userarry" };
	}

	for (int i = 0; i < userTblArray.GetCount(); ++i)
	{
		auto& record = userTblArray.GetAt(i);
		
		if (userIds.find(record.user_id) != userIds.end())
		{
			userInfos.push_back(record);
		}
	}

	// ログインユーザーを先頭にしておく
	auto currentUserIt = std::find_if(userInfos.begin(), userInfos.end(), [this](mctl_usertbl& u) { return u.user_id == m_currentUser.user_id; });
	if (currentUserIt != userInfos.end())
	{
		std::rotate(userInfos.begin(), currentUserIt, currentUserIt + 1);
	}

	return userInfos;
}

/// <summary>
/// バージョンアップ前の仕様で設定を取得する(一括印刷モードで利用)
/// </summary>
/// <returns></returns>
std::vector<COWNTB> SkjUserSettingsAccessor::GetOldVerSettings()
{
	CString		filter;
	if( IsSkj52() ){
		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ52_ITMNAME, m_currentUser.user_id );
	}
	else{
		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ_ITMNAME, m_currentUser.user_id );
	}

	std::vector<COWNTB> settings{};
	m_pZmSub->owntb->Requery(filter);
	int moveResult = m_pZmSub->owntb->MoveFirst();
	while (moveResult != ERR)
	{
		settings.push_back(*m_pZmSub->owntb);
		moveResult = m_pZmSub->owntb->MoveNext();
	}

	return settings;
}

/// <summary>
/// 利用者選択ダイアログを表示して結果を処理する
/// </summary>
/// <param name="parent">親ウィンドウ</param>
/// <param name="dlgInitParam">初期化パラメータ</param>
/// <param name="settings">(ref)設定リスト</param>
/// <param name="isUnconfirmed">(out)未確定の設定か</param>
void SkjUserSettingsAccessor::ShowUserSelectDialog(CWnd* parent, SkjUserSelectDlg::InitialParam&& dlgInitParam, std::vector<COWNTB>& settings, bool& isUnconfirmed)
{
	SkjUserSelectDlg dlg{ parent };
	dlg.Initialize(std::move(dlgInitParam));
	INT dlgResult = dlg.DoModal();
	int selectedUserId = dlg.GetSelectedUserId();
	switch (dlgResult)
	{
	case SkjUserSelectDlg::RES_OK:
		// 確定
		// 選択ユーザーの設定を利用するとともに他ユーザーの設定を削除する
	{
		auto rmvIt = std::remove_if(settings.begin(), settings.end(), [selectedUserId](COWNTB& data)
			{
				return data.u_id != selectedUserId;
			});
		settings.erase(rmvIt, settings.end());
		Confirm(selectedUserId);
		break;
	}
	case SkjUserSelectDlg::RES_TEMP_APPLY:
		// 一時適用
		// 選択ユーザーの設定を利用するがDBは更新しない
	{
		auto rmvIt = std::remove_if(settings.begin(), settings.end(), [selectedUserId](COWNTB& data)
			{
				return data.u_id != selectedUserId;
			});
		settings.erase(rmvIt, settings.end());
		isUnconfirmed = true;
		break;
	}
	case SkjUserSelectDlg::RES_CANCEL:
		// キャンセル
		// デフォルト設定を利用
		settings.clear();
		isUnconfirmed = true;
		break;
	default:
		throw std::exception{ "Unknown value returned." };
	}
}

/// <summary>
/// 選択ユーザーの設定で確定する
/// </summary>
/// <param name="selectedUserId">選択ユーザーID</param>
void SkjUserSettingsAccessor::Confirm(int selectedUserId)
{
	// owntbから選択ユーザー以外のデータを削除する
	m_pZmSub->m_database->BeginTrans();
	try
	{
		CString templateSql;
		if (selectedUserId == NULL_UID)
		{
			templateSql = _T("DELETE FROM owntb WHERE apl_name = '%s' AND itm_name = '%s' AND u_id IS NOT NULL");
		}
		else
		{
			templateSql = _T("DELETE FROM owntb WHERE apl_name = '%s' AND itm_name = '%s' AND (u_id IS NULL OR u_id <> %d)");
		}
		CString sql{};
		if (IsSkj52())
		{
			sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ52_ITMNAME_2, selectedUserId);
		}
		else
		{
			sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ_ITMNAME_2, selectedUserId);
		}
		m_pZmSub->m_database->ExecuteSQL(sql);

		// 選択ユーザーのデータのユーザーIDをNULLで更新する
		if (selectedUserId != NULL_UID)
		{
			templateSql = _T("UPDATE owntb SET u_id = NULL WHERE apl_name = '%s' AND itm_name = '%s' AND u_id=%d");
			if (IsSkj52())
			{
				sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ52_ITMNAME_2, selectedUserId);
			}
			else
			{
				sql.Format(templateSql, DBSYZSHIN_APLNAME, SKJ_ITMNAME_2, selectedUserId);
			}
			m_pZmSub->m_database->ExecuteSQL(sql);
		}

		m_pZmSub->m_database->CommitTrans();
	}
	catch (...)
	{
		m_pZmSub->m_database->Rollback();
		throw;
	}
}

/// <summary>
/// 新社会福祉に該当するか
/// </summary>
/// <returns></returns>
bool SkjUserSettingsAccessor::IsSkj52()
{
	return (m_pZmSub->zvol->apno == 0x52) && (m_pZmSub->zvol->m_ver == 0x11);
}

// TISW修正[22-0676] 2026/03/19 END