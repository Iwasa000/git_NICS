/*****************************************************************
	UcRenCommon.h

	内訳書財務連動の共通事項ヘッダファイル
		定数、列挙型、構造体

	2006.08.17	New
******************************************************************/

#ifndef	__UCRENCOMMON_H__
#define	__UCRENCOMMON_H__

//////////////////////////////////////////////////////////////////
//	ICS財務関連：会社コード管理番号 ･･･ 本来はシステム全体で定義すべき
typedef enum {
	ID_ICS_COTYPE_HOJIN =	0x00,		// 0x00	法人
	ID_ICS_COTYPE_UNSO =	0x01,		// 0x01	運送
	ID_ICS_COTYPE_KOJIN =	0x10,		// 0x10	個人
	ID_ICS_COTYPE_KENSETU =	0x20,		// 0x20	建設
	ID_ICS_COTYPE_BYOIN =	0x30		// 0x30	病院
} EnumIdICSCoType;

//////////////////////////////////////////////////////////////////
//	ICS財務関連：貸方科目の判断に使用
#define	ID_ICS_KNATT_KASI	0x01		// 貸方科目

//  ICS財務関連：ICS会社情報の枝番残高有無の判断に使用
#define ID_ICS_EZFLAG_TRUE	0x01		// 枝番残高有り

//  ICS財務関連：ICS会社情報の決算修正設定
typedef enum {
	ID_ICS_KESTYPE_KIMATSU = 0,			// 0:期末決算
	ID_ICS_KESTYPE_NONE,				// 1:決算修正なし
	ID_ICS_KESTYPE_HANKI,				// 2:半期決算
	ID_ICS_KESTYPE_SHIHANKI				// 3:四半期決算
} EnumIdICSKesType;

//  ICS財務関連：ICS決算月の設定
typedef enum {
	ID_ICS_KESSIGN_NORMAL = 0,			// 0:通常月
	ID_ICS_KESSIGN_SHIHAN1,				// 1:第一４半期
	ID_ICS_KESSIGN_HANKI,				// 2:第二４半期（中間決算）
	ID_ICS_KESSIGN_SHIHAN3,				// 3:第三４半期
	ID_ICS_KESSIGN_KIMATSU				// 4:第四４半期（期末決算）
} EnumIdICSKesSign;

//  ICS財務関連：中間決算月除外フラグの値
#define ID_ICS_KESSYKCUT_OFF	0		// 中間決算月の除外フラグ（OFF）
#define ID_ICS_KESSYKCUT_ON		1		// 中間決算月の除外フラグ（ON）

//	ICS財務関連：月オフセットの値
#define ID_ICS_OFFSET_MAX		16		// 月オフセットの最大値

//　ICS財務関連：期オフセットの値
typedef enum {
	ID_ICS_KI_OFFSET_TOUKI		= 0,
	ID_ICS_KI_OFFSET_ZENKI,
	ID_ICS_KI_OFFSET_ZENZENKI,
	ID_ICS_KI_OFFSET_3KIMAE,
	ID_ICS_KI_OFFSET_4KIMAE,
} EnumIdICSKiOffset;

//////////////////////////////////////////////////////////////////
//	財務連動：連動科目一覧、連動摘要一覧のリストデータ
#define ID_KAMOKU_LIST_MAX	5			// 連動科目一覧リストの１行カラム数
#define ID_TEKIYO_LIST_MAX	3			// 連動摘要一覧リストの１行カラム数

typedef enum {
	ID_KAMOKU_LIST_SEQ =	0,		// 0	シーケンス番号
	ID_KAMOKU_LIST_NO =		1,		// 1	番号
	ID_KAMOKU_LIST_KUBUN =  2,		// 2	区分
	ID_KAMOKU_LIST_ZKAMOKU =3,		// 3	財務科目
	ID_KAMOKU_LIST_UKAMOKU =4		// 4	内訳書科目
} EnumIdKamokuList;

typedef enum {
	ID_TEKIYO_LIST_TKCOD =	0,		// 0	摘要コード
	ID_TEKIYO_LIST_NO	 =	1,		// 1	番号
	ID_TEKIYO_LIST_TKNAME=	2		// 2	摘要名
} EnumIdTekiyoList;

//////////////////////////////////////////////////////////////////
//	財務連動：ICS会社情報の必要データ
typedef	struct	tag_RenZvolInfo{
	BYTE		CoType;					// 業種コード	（zvol.apno）
	BYTE		EzFlag;					// 枝番残高有無	（zvol.br_sw）
} RENZVOL_INFO;

//////////////////////////////////////////////////////////////////
//	財務連動：連動元データ／前回実行時の連動元データ選択内容
typedef enum {
	ID_RENDATA_INIT = -1,				// -1	なし
	ID_RENDATA_KZ = 0,					// 0	科目残高
	ID_RENDATA_EZ,						// 1	枝番残高
	ID_RENDATA_DA,						// 2	仕訳データ
	ID_RENDATA_KS,						// 3	科目残高集計
} EnumIdRenZaimuData;

//	財務連動：連動方法の選択内容
typedef enum{
	ID_OPUPDTYPE_ALLCLEAR = 0,			// 0	全データクリア後に財務連動を行う
	ID_OPUPDTYPE_VALRENDO,				// 1	金額のみ再連動する
} EnumIdOpUpdType;

//	財務連動：財務連動テンポラリ情報
typedef enum{
	ID_RENFGTEMP_NORMAL = 0,			// 0	連動更新未実施
	ID_RENFGTEMP_UPDATE,				// 1	連動更新済み
} EnumIdRenFgTemp;

//////////////////////////////////////////////////////////////////
//	財務連動：データ取込時の内訳書１レコードの必要な情報
typedef	struct	tag_RenTempInfo{
	// レコード作成用
	int			nPage;					// 内訳書の頁番号
	int			nRow;					// 内訳書の行番号
	CString		strRenKcd;				// 財務連動元（科目）
	long		lngRenEcd;				// 財務連動元（枝番）
	long		lngKnSeq;				// 内訳書科目
	CString		strAdName;				// 財務取引先名称（金融機関/取引先）
	CString		strVal;					// 金額
	// 内部処理用
	int			intOpRenType;			// 連動元データ選択情報
	int			intOpUpdType;			// 連動方法
	CString		strKcdFrom;				// 財務科目の開始番号
	CString		strKcdTo;				// 財務科目の終了番号
	int			intKimatsu;				// 期末決算期間オフセット
	int			intStartOffset;			// 集計期間開始オフセット
	int			intEndOffset;			// 集計期間終了オフセット
	BOOL		blnEndKes[ID_ICS_OFFSET_MAX];	// 集計期間終了月が決算修正月かどうか TRUE:決算修正月
	BOOL		blnSykFlg[ID_ICS_OFFSET_MAX];	// 集計対象判断フラグ
// midori 152132 add -->
	BYTE		byteEdaren;				// 枝番未登録･未入力のデータを連動するOn/Offフラグ
	BYTE		byteKmkName;			// 科目名称を取引先、金融機関名称に転記するOn/Offフラグ
	BYTE		byteF9Upd;				// 取引先、金融機関の[F9 参照]に登録するOn/Offフラグ
	BYTE		byteEdasgn;				// 枝番残高取込時、現在取込を行っているデータが枝番取込か科目取込かのフラグ
										// FALSE:科目取込 TRUE:枝番取込 
// midori 152132 add <--
// midori 152137 add -->
	CString		strAdKana;				// 財務取引先カナ（金融機関/取引先）
// midori 152137 add <--
	CString		strInvNo;				// インボイス登録番号			// インボイス登録番号追加対応_23/11/27 add

} RENTEMP_INFO;

//////////////////////////////////////////////////////////////////
//	財務連動：集計方法
#define ID_COMBO_MAKEUP_RUIKEI	0			// 通常集計（期首からの累計）　決算書業務と同様
#define ID_COMBO_MAKEUP_KIKAN	1			// 期間集計

#endif	//	__UCRENCOMMON_H__
