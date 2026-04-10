/*****************************************************************
	DBUchiImpDEF.h
	
	データインポートモジュール定義ヘッダ
		定数、列挙型、構造体

	2009.06.18	New
******************************************************************/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// エラーコード関連
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////////////////////////////////////////////
// エラーコード(内部)
typedef	enum {
	IMP_ERRCODE_SUCCESS	= 0,			// 正常終了
// midori 160603 del -->
//	IMP_ERRCODE_FINDFOLDER,				// エラー（フォルダがない）
// midori 160603 del <--
	IMP_ERRCODE_CREATEDFOLDER,			// エラー（ファイル作成失敗）
	IMP_ERRCODE_CSVOPEN,				// エラー（CSVファイルのオープン失敗）
	IMP_ERRCODE_FIXDATA,				// エラー（固定データが見つからない）
	IMP_ERRCODE_CONAME,					// エラー（会社名が異なる）
	IMP_ERRCODE_IMPORT,					// エラー（データインポート失敗）
	IMP_ERRCODE_EXPORT,					// エラー（データエクスポート失敗）
	IMP_ERRCODE_DB,						// エラー（DBエラー）
	IMP_ERRCODE_DRIVE_EMPTY,			// エラー（ドライブ容量不足）
	IMP_ERRCODE_OTHER,					// エラー（その他）
// midori 150894 cor -->
//	IMP_ERRCODE_IMPOPT_READ				// エラー（インポートオプション読み込み失敗）	20111007
// ---------------------
	IMP_ERRCODE_IMPOPT_READ,			// エラー（インポートオプション読み込み失敗）	20111007
// midori 170504 del -->
	//IMP_ERRCODE_IMPORT_TITLE			// エラー（データインポート失敗「様式名」）
// midori 170504 del <--
// midori 170504 add -->
	IMP_ERRCODE_IMPORT_TITLE,			// エラー（データインポート失敗「様式名」）
// 157766 del -->
	//IMP_ERRCODE_IMPORT_ITEM				// エラー（データインポート失敗「項目タイトル」）
// 157766 del <--
// 157766 add -->
	IMP_ERRCODE_IMPORT_ITEM,			// エラー（データインポート失敗「項目タイトル」）
	IMP_ERRCODE_NUMOFITEM				// エラー（項目数が一致していません）
// 157766 add <--
// midori 170504 add <--
// midori 150894 add <--
} EnumImpErrorCode;

//////////////////////////////////////////////////////////////////
// エラーコード(外部)
#define IMP_DIALOG_ERRCODE_OK			0		// OK（処理が行われた）
#define IMP_DIALOG_ERRCODE_CANCEL		-1		// キャンセル（処理が行われていない）

//////////////////////////////////////////////////////////////////
// レコード（行）の種類
typedef	enum {
	IMP_REC_OK	= 0,					// 0：インポート／エクスポート対象のレコード
	IMP_REC_NG,							// 1：インポート／エクスポート対象外のレコード
	IMP_REC_EOF,						// 2：EOF（レコードがない）
	IMP_REC_IKKATU,						// 3：一括集計金額のレコード
	IMP_REC_HIDE						// 4：非表示
} EnumImpRec;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// テーブル関連
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
#define IMP_MAX_NUM_ROW					250		// 最大行番号


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CSVファイル関連
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////////////////////////////////////////////
// CSVの情報
typedef	struct	tag_ImpCsvInfo {
	BOOL		fImport;						// 対象（TRUE=インポート, FALSE=エクスポート）
	char		szFolderPath[MAX_PATH];			// フォルダパス
	char		szFileName[256];				// ファイル名
	char		szSheetName[256];				// 帳票名			[2009/07/08 Add for Error Message]
	int			nLine;							// 行番号
	char		szId[256];						// 識別子
	char		szColumn[256];					// カラム名
	char		szErrType[256];					// エラー種別（文字列長エラー、文字種別エラー等々）
// midori 150894 add -->
	// 様式
	char		szErr[SHEET_NUM];				// エラーが発生した様式
	int			nAllErr;						// 1:すべての様式がエラー
	// データ
	int			nErr;							// 1:識別子のエラー 2:項目数のエラー
// midori 180404 add -->
												// 3:⑭-2 人件費の内訳書の区分エラー
// midori 180404 add <--
	int			nKind;							// 種類(0:文字 1:金額 2:面積等 3:日付)
	int			nLength;						// 最大文字(桁)数
	int			nLength2;						// 小数点以下の最大桁数(nKind=2のみ)
	int			nType;							// エラーの状態１(0:正数 1:負数)
	int			nType2;							// エラーの状態２(0:整数部 1:小数部 2:両方 nKind=2のみ)
// midori 150894 add <--
} IMP_CSV_INFO;

//////////////////////////////////////////////////////////////////
#define IKKATU_CSV_DATA_ITEM_MAX		32
// 一括集計金額CSVデータ
typedef	struct tag_IkkatuCsvData {
	int nGroup;			// グループ番号
	CString strItem[IKKATU_CSV_DATA_ITEM_MAX];
} IKKATU_CSV_DATA;

//////////////////////////////////////////////////////////////////
// インポートオプション
typedef	struct	ImpOpt {
	BOOL		chkState;						// 入力選択チェックボックス
	int			cmbTrans;						// 転送方式
	CString		SheetNo;						// 様式番号
	CString		SheetName;						// 様式名称
	CString		FileName;						// 様式に対応するファイル名
	int			bitTarget;						// D?ビットを見るか

public:
	ImpOpt(){
		chkState = -1;
		cmbTrans = -1;
		SheetNo.Empty();
		SheetName.Empty();
		FileName.Empty();
	}
} ImpOpt;

#define CLM_CHKBOX						0
#define	CLM_CMBBOX						1

#define TRANS_OVERWRITE					0
#define TRANS_ADD						1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INIファイル関連
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////////////////////////////////////////////
// 
//#define IMP_INI_FOLDERPATH				_T("C:\\ICSWin\\Cmds\\GR1b\\JOBS50")
//#define IMP_INI_FOLDERPATH				_T("\\ICSWin\\Cmds\\GR1b\\JOBS50")		// [2009/07/27 Change for ICSWin Drive]
#define IMP_INI_FOLDERPATH				_T("\\ICSWin\\Cmds\\GR%s\\JOBS50")			// [2012/07/20 Change for Any GrNO]
#define IMP_INI_FILENAME				_T("DBUchiImp.ini")
#define IMP_INI_SECTION_SYSTEM			_T("SYSTEM")
#define IMP_INI_KEY_IMPORT_FOLDER		_T("ImportFolder")
#define IMP_INI_KEY_EXPORT_FOLDER		_T("ExportFolder")


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// メッセージ関連
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////////////////////////////////////////////
// メッセージ文言（ダイアログ）
#define IMP_DLGMSG_IMPORT_TITLE			_T("勘定科目内訳書 データ インポート")
#define IMP_DLGMSG_IMPORT_EXECUTE		_T("データをインポートします。")
#define IMP_DLGMSG_IMPORT_ATTENTION		_T("※転送方式が上書きの場合、既存のデータは削除されます。")
#define IMP_DLGMSG_IMPORT_SELECTFOLDER	_T("インポートするデータを含むフォルダを選択してください。")
#define IMP_DLGMSG_EXPORT_TITLE			_T("勘定科目内訳書 データ エクスポート")
#define IMP_DLGMSG_EXPORT_EXECUTE		_T("データをエクスポートします。")
#define IMP_DLGMSG_EXPORT_ATTENTION		_T("※エクスポートすると、既存のCSVファイルは上書きされます。")		// [2009/07/09 Add Attention Message]
#define IMP_DLGMSG_EXPORT_SELECTFOLDER	_T("データをエクスポートするフォルダを選択してください。")
#define IMP_DLGMSG_ERROR_SELECTFOLDER	_T("フォルダの参照に失敗しました。")
#define IMP_DLGMSG_IMPORT_OPTION_TITLE	_T("勘定科目内訳書　インポートオプション")
#define IMP_DLGMSG_EXPORT_OPTION_TITLE	_T("勘定科目内訳書　エクスポートオプション")
#define IMP_DLGMSG_IMPORT_BUTTON		_T("インポート開始");
#define IMP_DLGMSG_EXPORT_BUTTON		_T("エクスポート開始");
#define IMP_DLGMSG_IMPORT_SET			_T("インポートフォルダを指定してください。")
#define IMP_DLGMSG_EXPORT_SET			_T("エクスポートフォルダを指定してください。")

//////////////////////////////////////////////////////////////////
// メッセージ文言（処理中）
#define IMP_PRGMSG_IMPORT				_T("データをインポートしています。")
#define IMP_PRGMSG_EXPORT				_T("データをエクスポートしています。")
#define IMP_PRGMSG_WAIT					_T("しばらくお待ちください．．．")

//////////////////////////////////////////////////////////////////
// メッセージ文言（エラー）
// midori 160603 cor -->
//#define IMP_ERRMSG_FINDFOLDER			_T("'%s' は見つかりませんでした。")
// ---------------------
#define	IMP_ERRMSG_FINDFOLDER			_T("指定されたフォルダが存在しません。")
// midori 160603 cor <--
#define IMP_ERRMSG_CREATEDFOLDER		_T("'%s' を作成できませんでした。")
//#define IMP_ERRMSG_CSVOPEN				_T("'%s' のオープンに失敗しました。")
//#define IMP_ERRMSG_FIXDATA				_T("'%s' の %d 行目に '%s' が見つかりませんでした。")
//#define IMP_ERRMSG_CONAME				_T("'%s' の会社名が異なります。")
//#define IMP_ERRMSG_PART1				_T("'%s' の %d 行目の")
//#define IMP_ERRMSG_PART2				_T("'%s' の")
#define IMP_ERRMSG_CSVOPEN				_T("'%s' (%s) を開けませんでした。")						// [2009/07/08 Change Message] 
#define IMP_ERRMSG_FIXDATA				_T("'%s' (%s) の %d 行目に '%s' が見つかりませんでした。")	// [2009/07/08 Add for Form No.]
// 157766 add -->
// 157785 del -->
//// 157780 del -->
////#define IMP_ERRMSG_NUMOFITEM			_T("'%s' (%s) %d 行目の項目数が一致していません。")
//// 157780 del <--
//// 157780 add -->
//#define IMP_ERRMSG_NUMOFITEM			_T("\"%s\"【%s】\nデータインポートに失敗しました。\n\n%d 行目の項目数が一致していません。")
//// 157780 add <--
// 157785 del <--
// 157785 add -->
// 157785 add <--
#define IMP_ERRMSG_NUMOFITEM			_T("\"%s\"【%s】\nデータインポートに失敗しました。\n\n　項目数が一致していません。")
// 157766 add <--
#define IMP_ERRMSG_CONAME				_T("'%s' (%s) の会社名が異なります。")						// [2009/07/08 Add for Form No.]
#define IMP_ERRMSG_PART1				_T("'%s' (%s) の\r\n%d 行目の%s")								// [2009/07/08 Add for Form No.]	[2016/01/18 Add column info]
#define IMP_ERRMSG_PART2				_T("'%s' (%s) の")											// [2009/07/08 Add for Form No.]
#define IMP_ERRMSG_IMPORT				_T("データ インポートに失敗しました。")
#define IMP_ERRMSG_EXPORT				_T("データ エクスポートに失敗しました。")
#define IMP_ERRMSG_DB_ACCESS			_T("データベースの操作に失敗しました。")
#define IMP_ERRMSG_DO_SHEET				_T("現在の帳票の処理を継続しますか？")
#define IMP_ERRMSG_SKIP_SHEET			_T("現在の帳票の処理をスキップし、次の帳票の処理を行いますか？")
#define IMP_ERRMSG_SKIP_LINE			_T("現在の行の処理をスキップし、次の行の処理を行いますか？")
#define IMP_ERRMSG_DRIVE_EMPTY			_T("容量が少ないため、%s 以降のデータをエクスポートできません。")		// [2009/07/28 Add for Drive Empty]
#define IMP_ERRMSG_IMPOPT_READ			_T("インポートオプションを読み込めませんでした。")			// 20111007
#define	IMP_ERRMSG_FINDFILE				_T("指定されたフォルダにCSVファイルが存在しません。")		// 20111007
// midori 150894 add -->
#define IMP_ERRMSG_TITLE1				_T("帳票タイトルのデータインポートに失敗しました。")
#define IMP_ERRMSG_TITLE2				_T("上記の様式をスキップし、データインポート処理を継続しますか？")
#define IMP_ERRMSG_TITLE3				_T("すべての様式が取り込めません。処理を中断します。")
// midori 150894 add <--

#define IMP_ERRMSG_ITM_NOT_NUM			_T("数字以外の文字が含まれています。")						// 20160118
#define IMP_ERRMSG_ITM_NOT_HANKAKU		_T("半角以外の文字が含まれています。")						// 20160118
// midori 150894 cor -->
//#define IMP_ERRMSG_ITM_OVER_LENGTH		_T("文字数が多すぎます。")									// 20160118
// ---------------------
#define IMP_ERRMSG_ITM_OVER_LENGTH		_T("最大文字数を超えています。")
#define IMP_ERRMSG_ITM_OVER_LENGTH_N	_T("最大桁数を超えています。")
// midori 150894 cor <--
#define IMP_ERRMSG_ITM_DATE_TYPE		_T("日付が正しくありません。")								// 20160118
#define IMP_ERRMSG_ITM_DATE_GENGO		_T("元号が正しくありません。")								// 20160118
#define IMP_ERRMSG_ITM_NOT_TARGET_ROW	_T("インポート対象行ではありません。")						// 20160118　カラムを指定しない
// midori 150972 cor -->
//#define IMP_ERRMSG_ITM_142_KUBUN		_T("区分が正しくありません。")								// 20160118　カラムを指定しない
// ---------------------
#define IMP_ERRMSG_ITM_052_KUBUN		_T("区分が正しくありません。")
#define IMP_ERRMSG_ITM_141_KUBUN		_T("区分が正しくありません。")
#define IMP_ERRMSG_ITM_142_KUBUN		_T("'区分' が正しくありません。")
// midori 150972 cor <--

// 登録番号（法人番号）_23/11/21 add -->
#define IMP_ERRMSG_ITM_THNO_ERR1		_T("13桁未満です。")
#define IMP_ERRMSG_ITM_THNO_ERR2		_T("13桁を超えています。")
#define IMP_ERRMSG_ITM_THNO_ERR3		_T(" T または数字以外の文字が含まれています。")
#define IMP_ERRMSG_ITM_THNO_ERR4		_T("正しくありません。")
// 登録番号（法人番号）_23/11/21 add <--

/////////////////////////////////////////////////////////////////
// メッセージ文言（終了）
#define IMP_INFMSG_IMPORT				_T("データ インポートを終了しました。")
#define IMP_INFMSG_EXPORT				_T("データ エクスポートを終了しました。")


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 日付関連
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/////////////////////////////////////////////////////////////////
// 日付変換タイプ
typedef	enum {
	IMP_DATE_CONV_TYPE_A = 0,			// 0：gee.mm.dd
	IMP_DATE_CONV_TYPE_B,				// 1：gee.mm
	IMP_DATE_CONV_TYPE_C,				// 2：Gee.mm
	IMP_DATE_CONV_TYPE_D				// 3：Gee
} EnumImpDateConvType;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// その他
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////////////////////////////////////////////
// インポート/エクスポート
#define ID_IMPORT						1
#define ID_EXPORT						0

extern BOOL RemoveCommaImportNum(CString &strVal );