// TISW修正[22-0676] 2026/03/19 START
#pragma once

#include "stdafx.h"
#include <unordered_map>
#include <unordered_set>

/// <summary>
///       [22-0676]の対応後に還付申告明細用連動情報の設定を残す利用者を選択するダイアログ
/// <para>利用者の選択のみを担当し、利用者選択後の処理は呼び出し元で行う</para>
/// </summary>
class SkjUserSelectDlg : public ICSDialog
{
	DECLARE_DYNAMIC(SkjUserSelectDlg)

public:
	/// <summary>
	/// SkjUserSelectDlgの初期化パラメータ
	/// </summary>
	struct InitialParam
	{
		/// <summary>ユーザーリスト</summary>
		std::vector<mctl_usertbl> m_users;
		/// <summary>デフォルト設定から変更のないユーザーIDリスト</summary>
		std::unordered_set<int> m_defSettingUsers;
	};

private:
	/// <summary>初期化パラメータ</summary>
	InitialParam m_initParam;

	/// <summary>選択インデックス</summary>
	int m_selectedIndex{ -1 };

	/// <summary>利用者コンボボックス</summary>
	CComboBox m_userCombo;

public:
	SkjUserSelectDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	SkjUserSelectDlg(UINT id, CWnd* pParent = NULL);
	virtual ~SkjUserSelectDlg();

	enum { IDD = IDD_DIALOG_SKJ_USR_SEL };

	// DoModalの戻り値
	static const INT RES_OK = IDOK; // 確定
	static const INT RES_CANCEL = IDCANCEL; // キャンセル
	static const INT RES_TEMP_APPLY = 100; // 一時適用

	/// <summary>
	/// 表示前の初期化処理
	/// </summary>
	/// <param name="initParam">初期化パラメータ</param>
	void Initialize(InitialParam&& initParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

	/// <summary>選択ユーザーIDを取得する</summary>
	int GetSelectedUserId();

protected:
	/// <summary>確定ボタンのクリック</summary>
	void OnOK() override;

private:
	/// <summary>一時適用ボタンのクリック</summary>
	void OnTempApplyClicked();
};

/// <summary>
/// 還付申告明細用連動情報の設定取得用クラス
/// </summary>
class SkjUserSettingsAccessor
{
public:
	/// <summary>owntbの登録でNULLとして処理されるu_idの値</summary>
	static const int NULL_UID = -1;
private:
	CDBNpSub* m_pZmSub;				// 公益クラス
	mctl_usertbl& m_currentUser;	// ログインユーザー情報
	CTblhdl* m_pTblhdl;				// テーブルコンテナ
	_TB_PAR* m_pSkjCnvtbl;			// 還付申告明細用連動情報のテーブル情報
	bool m_isOwnTbOpend = false;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pZmSub">公益クラス</param>
	/// <param name="currentUser">ログインユーザー情報</param>
	/// <param name="pTblhdl">テーブルコンテナ</param>
	/// <param name="pSkjCnvtbl">還付申告明細用連動情報のテーブル情報</param>
	SkjUserSettingsAccessor(CDBNpSub* pZmSub, mctl_usertbl& currentUser, CTblhdl* pTblhdl, _TB_PAR* pSkjCnvtbl);
	~SkjUserSettingsAccessor();

	/// <summary>
	/// owntbから還付申告明細用連動情報の設定を取得する
	/// </summary>
	/// <param name="parent">親ウィンドウ</param>
	/// <param name="isBulkPrintMode">一括印刷モードか</param>
	/// <param name="isUnconfirmed">(out)未確定の設定か</param>
	/// <returns></returns>
	std::vector<COWNTB> GetSkjSettings(CWnd* parent, bool isBulkPrintMode, bool& isUnconfirmed);

	/// <summary>
	/// owntbに還付申告明細用連動情報の設定を保存する
	/// </summary>
	void SaveSkjSettings();

private:
	/// <summary>
	/// 依頼[22-0676]対応後の初回実行時にバージョンアップ処理を行う
	/// </summary>
	void VersionUp_22_0676();

	/// <summary>
	/// owntbから還付申告明細用連動情報の設定を取得する(全利用者)
	/// </summary>
	/// <returns>ユーザーID -> 還付申告明細用連動情報の設定リスト</returns>
	std::unordered_map<int, std::vector<COWNTB>> GetAllSkjSettings();

	/// <summary>
	/// デフォルト設定から変更していないユーザーIDのリストを取得する
	/// </summary>
	/// <param name="settings">ユーザーID -> 還付申告明細用連動情報の設定リスト</param>
	/// <returns></returns>
	std::unordered_set<int> GetDefaultSettingUsers(const std::unordered_map<int, std::vector<COWNTB>>& idToSettings);

	/// <summary>
	/// 全ユーザーの設定が一致するか
	/// </summary>
	/// <param name="idToSettings">ユーザーID -> 還付申告明細用連動情報の設定リスト</param>
	/// <returns></returns>
	bool IsAllUserSettingsSame(const std::unordered_map<int, std::vector<COWNTB>>& idToSettings);

	/// <summary>
	/// 還付申告明細用連動情報のデータソースに含まれるユーザー情報のリストを取得する
	/// </summary>
	/// <param name="idToSettings">ユーザーID -> 還付申告明細用連動情報の設定リスト</param>
	/// <returns></returns>
	/// <remarks>1人のみの場合はuser_idのみセットして返す(DBアクセスしない)</remarks>
	std::vector<mctl_usertbl> GetSkjRegisteredUserInfos(const std::unordered_map<int, std::vector<COWNTB>>& idToSettings);

	/// <summary>
	/// バージョンアップ前の仕様で設定を取得する(一括印刷モードで利用)
	/// </summary>
	/// <returns></returns>
	std::vector<COWNTB> GetOldVerSettings();

	/// <summary>
	/// 利用者選択ダイアログを表示して結果を処理する
	/// </summary>
	/// <param name="parent">親ウィンドウ</param>
	/// <param name="dlgInitParam">初期化パラメータ</param>
	/// <param name="settings">(ref)設定リスト</param>
	/// <param name="isUnconfirmed">(out)未確定の設定か</param>
	void ShowUserSelectDialog(CWnd* parent, SkjUserSelectDlg::InitialParam&& dlgInitParam, std::vector<COWNTB>& settings, bool& isUnconfirmed);

	/// <summary>
	/// 選択ユーザーの設定で確定する
	/// </summary>
	/// <param name="selectedUserId">選択ユーザーID</param>
	void Confirm(int selectedUserId);

	/// <summary>
	/// 新社会福祉に該当するか
	/// </summary>
	/// <returns></returns>
	bool IsSkj52();
};
// TISW修正[22-0676] 2026/03/19 END