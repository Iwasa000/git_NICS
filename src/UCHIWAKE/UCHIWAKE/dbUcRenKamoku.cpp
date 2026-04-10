// dbUcRenKamoku.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcRenKamoku.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenKamoku

IMPLEMENT_DYNAMIC(CdbUcRenKamoku, CdbBase)

CdbUcRenKamoku::CdbUcRenKamoku(CDatabase* pdb)
	: CdbBase(pdb)
	, m_SeqUserStartNum(ID_LST_USER_START)		//Seq(ユーザ利用番号開始値)
{
	//{{AFX_FIELD_INIT(CdbUcRenKamoku)
	m_Seq = 0;
	m_FormSeq = 0;
	m_FgCoType = 0;
	m_KcdStart = _T("");
	m_KnSeq = 0;
	m_FgDft = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_REN_KAMOKU;	// テーブル名を記述
}


CString CdbUcRenKamoku::GetDefaultSQL()
{
	return _T("[dbo].[uc_ren_kamoku]");
}

void CdbUcRenKamoku::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcRenKamoku)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Byte(pFX, _T("[FgCoType]"), m_FgCoType);
	RFX_Text(pFX, _T("[KcdStart]"), m_KcdStart);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Byte(pFX, _T("[FgDft]"), m_FgDft);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenKamoku 診断

#ifdef _DEBUG
void CdbUcRenKamoku::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcRenKamoku::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			long	inSeq		シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenKamoku::Init(long inSeq)
{
	return	RequerySeq(inSeq);
}

/**********************************************************************
	RequerySeq()
		シーケンス番号の一致するレコード取得

	引数
			long	inSeq		シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenKamoku::RequerySeq(long inSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("Seq = %d", inSeq);
	m_strSort = "";

	return	RequeryWork();
}

/**********************************************************************
	RequeryKnSeq()
		内訳書科目シーケンス番号の一致するレコード取得

	引数
			long	inKnSeq		シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenKamoku::RequeryKnSeq(long inKnSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("KnSeq = %d", inKnSeq);
	m_strSort = "";

	return	RequeryWork();
}


/**********************************************************************
	RequeryFormSeqFgCoType()
		様式シーケンス番号と会社コード管理№の一致するレコード取得

	引数
			int		inFormSeq	様式シーケンス番号
			BYTE	inFgCoType	会社コード管理№
			int		inAscType	0:ダイアログ表示用，1:データ取込用
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenKamoku::RequeryFormSeqFgCoType(int inFormSeq, BYTE inFgCoType, int inAscType)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format("FormSeq = %d AND FgCoType = %d", inFormSeq, inFgCoType);
	if (inAscType == 0) {
		m_strSort = "FgDft DESC, knSeq ASC, KcdStart ASC";		// 表示時は追加分を最後に表示したい
	}
	else {
		m_strSort = "knSeq ASC, FgDft DESC, KcdStart ASC";		// データ取込時は内訳書科目順に取得したい
	}

	return	RequeryWork();
}

/**********************************************************************
	CreateNewRecord()
		新規レコードを作成、初期値を追加
		レコードセットには、新規レコードがセットされて戻ります

	引数
			int		inFormSeq	様式シーケンス番号
			BYTE	inFgCoType	会社コード管理№
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenKamoku::CreateNewRecord(int inFormSeq, BYTE	inFgCoType)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	// 新しいシーケンス番号を算出
	SqlStr =	_T("SELECT max(Seq) AS MaxNum ");
	SqlStr +=	_T("FROM ") + m_lo_TableName + _T(" ");

	if ( ! OpenEx( &rs , SqlStr , m_lo_TableName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("MaxNum"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = m_SeqUserStartNum;
		}
		else {								// 値の取得OK
//			retVal = val.m_iVal;
			switch (val.m_dwType) {
			case DBVT_UCHAR:
				retVal = val.m_chVal;		// char
				break;
			case DBVT_SHORT:
				retVal = val.m_iVal;		// short
				break;
			case DBVT_LONG:
				retVal = val.m_lVal;		// long
				break;
			default:
				retVal = val.m_lVal;		// default:long
				break;
			}
			//ユーザ使用開始値より小さい場合（ユーザ登録データが未登録の場合)
			if (retVal < m_SeqUserStartNum) {
				retVal = m_SeqUserStartNum;
			}
		}
	}
	rs.Close();

	retVal++;				//	自動付番シーケンス番号カウントアップ


	/////////////////////////////////////////////////////////////////////
	// 新規レコード追加作業
	int	ret = RequeryWork();
	if ( ret != DB_ERR_OK) {
		return ret;
	}

	AddNew();
	m_Seq = retVal;				// シーケンス番号を自動付番
	m_FormSeq = inFormSeq;		// 様式シーケンス番号
	m_FgCoType = inFgCoType;	// 会社コード管理№
	m_KcdStart = _T("");
	m_KnSeq = 0;
	m_FgDft = 0;				// ユーザー登録
	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 追加レコードを抽出
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

/**********************************************************************
	CheckRepeat()
		指定科目コードが既に登録済みか確認

	引数
			int		inFormSeq	様式シーケンス番号
			BYTE	inFgCoType	会社コード管理№
			CString	inKcdStart	検索科目コード
			int		inType		0:inKcdStartと一致するレコードを検索(デフォルト)
								1:inKcdStart以下の科目コードを含むレコードを検索
	戻値
			int					DB_ERR_OK (0)     ：未登録
								0以上			　：重複(inType = 0 … 重複レコードのシーケンス番号
												　：	 inType = 1 … 1
								DB_ERR			　：エラー
***********************************************************************/
int CdbUcRenKamoku::CheckRepeat(int inFormSeq, BYTE inFgCoType, CString inKcdStart , int inType /*=0*/ )
{
	///////////////////////////////////////////////////////////////////
	CString	strKcdCode	= "";		// 検索科目コード
	int		retVal		= DB_ERR;	// 戻り値

	// パラメータの科目コード以下の区分のレコードを取得
	if ( inType == 1 ){
		m_strFilter.Format("FormSeq = %d AND FgCoType = %d AND KcdStart LIKE '%s%%'", inFormSeq, inFgCoType , inKcdStart );
	}
	// パラメータの科目コードと一致するレコードを取得
	else{
		m_strFilter.Format("FormSeq = %d AND FgCoType = %d AND KcdStart = '%s'", inFormSeq, inFgCoType , inKcdStart );
	}

	// 実行
	RequeryWork();
	// 重複していればそのレコードのシーケンス番号を返す
	if ( !IsEOF() ){
		if ( inType == 1 ){
			return 1;
		}
		else{
			return m_Seq;
		}
	}

	// 未登録
	return DB_ERR_OK;
}


