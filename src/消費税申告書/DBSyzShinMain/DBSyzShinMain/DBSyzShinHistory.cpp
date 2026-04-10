#include "stdafx.h"
#include "resource.h" 
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#define ARRAYCOUNT( _array ) (sizeof(_array) / sizeof(_array[0]))
#define swap(a,b) {a += b;b = a - b;a -= b;}

using namespace std;

/*
	履歴保存 基本仕様
	・現在申告分(最終申告分) を 0 とし、履歴が古い程大きいシーケンスとする
	・「次の申告へ」で履歴を保存毎にシーケンス番号を更新する

	IGNORE_DUP_KEY off のためシーケンスの重複が一時的でも許されない
	1 最終申告分(0番)をテンポラリテーブルにコピー
	2 元テーブル シーケンスを更新 "UPDATE テーブル set シーケンス = シーケンス + 1"
	3 テンポラリテーブルから 元テーブルにもどす

	"テーブル名"に"シーケンス"のカラムがあるかどうかチェック。簡易課税マスターで特定収入テーブルのとき無いことがある
	SELECT * FROM Sys.Columns WHERE name = "シーケンス" AND object_id = object_id('"テーブル名"')
*/
CString GetQuery(CString seq,CString tbname)
{
	CString sq;
	sq += "IF EXISTS( SELECT * FROM Sys.Columns WHERE name = '"+seq+"' AND object_id = object_id('"+ tbname + "') ) \n";
	sq += "BEGIN \n";
	sq += "EXEC(' \n";
	sq += "		if object_id(''tempdb..#temp_"+ tbname +"'', ''u'') is not null drop table #temp_"+ tbname +" \n";
	sq += "		SELECT * INTO #temp_"+ tbname +" FROM "+ tbname +" WHERE "+ seq +" = 0 \n";
	sq += "		UPDATE "+ tbname +" set "+ seq +" = "+ seq +" + 1 \n";
	sq += "		INSERT INTO "+ tbname +" SELECT * from #temp_"+ tbname +" \n";
	sq += "') \n";
	sq += "END\n";

	return sq;
}

CString GetQueryDrop(CString seq,CString tbname)
{
	CString sq;
	sq += "IF EXISTS( SELECT * FROM Sys.Columns WHERE name = '"+seq+"' AND object_id = object_id('"+ tbname + "') ) \n";
	sq += "BEGIN \n";
	sq += "EXEC(' \n";
	sq += "		DELETE FROM "+ tbname +" WHERE "+ seq +" = 0 \n";
	sq += "		UPDATE "+ tbname +" set "+ seq +" = "+ seq +" - 1\n";
	sq += "') \n";
	sq += "END\n";
	return sq;
}

CString GetQueryDropOnly(CString seq,CString tbname)
{
	CString sq;
	sq += "IF EXISTS( SELECT * FROM Sys.Columns WHERE name = '"+seq+"' AND object_id = object_id('"+ tbname + "') ) \n";
	sq += "BEGIN \n";
	sq += "EXEC(' \n";
	sq += "		DELETE FROM "+ tbname +" WHERE "+ seq +" = 0 \n";
	sq += "') \n";
	sq += "END\n";
	return sq;
}

CString GetQueryDropOnlyH26(CString seq, CString tbname, CString ver)
{
	CString sq;
	sq += "IF EXISTS( SELECT * FROM Sys.Columns WHERE name = '"+seq+"' AND object_id = object_id('"+ tbname + "') ) \n";
	sq += "BEGIN \n";
	sq += "EXEC(' \n";
	sq += "		DELETE FROM "+ tbname +" WHERE "+ seq +" = 0 AND " + ver + " = 1 \n";
	sq += "') \n";
	sq += "END\n";
	return sq;
}


//CString GetQuerySelectDrop(CString seq,CString tbname)
//{
//	CString sq;
//	sq += "IF EXISTS( SELECT * FROM Sys.Columns WHERE name = '"+seq+"' AND object_id = object_id('"+ tbname + "') ) \n";
//	sq += "BEGIN \n";
//	sq += "EXEC(' \n";
//	sq += "		DELETE FROM "+ tbname +" WHERE "+ seq +" = 0 AND kslist < 10 \n";
//	sq += "') \n";
//	sq += "END\n";
//
//	return sq;
//}

CString GetQueryAllDrop(CString seq,CString tbname)
{
	CString sq;
	sq += "IF OBJECTPROPERTY ( object_id('"+ tbname +"'),'ISTABLE') IS NOT NULL \n";
	sq += "BEGIN \n";
	sq += "EXEC(' \n";
	sq += "		DELETE FROM "+ tbname +" WHERE "+ seq +" != 0 \n";
	sq += "') \n";
	sq += "END\n";
	return sq;
}

int IsTyukanShinkoku(const int skkbn)
{
	return skkbn == tyuukan || skkbn == tyuukan_syuuse || skkbn == yotei;
}

int IsKakuteiShinkoku(const int skkbn)
{
	return skkbn == kakutei || skkbn == kakutei_syuuse || skkbn == kousei;
}

map<int,CString> GetMapSkbnName()
{
	pair<int,CString> list[] = {
		pair<int,CString>( kakutei, "確定"),
		pair<int,CString>( tyuukan, "中間"),
		pair<int,CString>( yotei ,"予定(26号)"),
//2016.10.20 UPDATE START
		//pair<int,CString>( kakutei_syuuse,"確定修正"),
		//pair<int,CString>( tyuukan_syuuse,"中間修正"),
		pair<int,CString>( kakutei_syuuse,"修正確定"),
		pair<int,CString>( tyuukan_syuuse,"修正中間"),
//2016.10.20 UPDATE END
		pair<int,CString>( kousei,"更正の請求"),
//2018.03.13 INSERT START
		pair<int,CString>( junkakutei, "準確定"),
//2018.03.13 INSERT END
	};

	map<int,CString> comboitmmap;
	for(int i = 0;i<ARRAYCOUNT(list);i++)
		comboitmmap.insert(list[i]);

	return comboitmmap;
}

CString GetKubunStr(int kubun,int minashi)
{
	CString str = GetMapSkbnName()[kubun];
	if( minashi )
		str += "(みなし課税期間)";
	return str;
}

map<int,int> makemap(vector<int>& vec,int swap_sw)
{
	map<int,int> _map;
	for(int i = 0;i<(int)vec.size();i++){
		pair<int,int> p(i,vec[i]);
		if(swap_sw) swap(p.first,p.second);
		_map.insert(p);
	}
	return _map;
}

vector<int> GetAllShinkokuKubun()
{
	int all_list[] = { kakutei, tyuukan, yotei, kakutei_syuuse, tyuukan_syuuse, kousei, };
	vector<int> kbvec;
	for(int i = 0;i<ARRAYCOUNT(all_list);i++)
		kbvec.push_back(all_list[i]);

	return kbvec;
}

map<int,int> CDBSyzShinMainView::GetMapOfsKubun( int swap_sw )
{
	set<int> s;
	s.insert(kakutei);
	s.insert(kakutei_syuuse);
	s.insert(kousei);

	if( IsTyukanAri() ){
		s.insert(tyuukan);
		s.insert(tyuukan_syuuse);
		s.insert(yotei);
	}

//2018.03.13 INSERT START
	if( IsKojin()&&IsMynumber() ){
		s.insert(junkakutei);
	}
//2018.03.13 INSERT END

	vector<int> kbvec;
	for(set<int>::iterator it = s.begin();it != s.end();it++)
		kbvec.push_back(*it);

	return makemap( kbvec, swap_sw );
}

map<int,int> CDBSyzShinMainView::GetMapMcount( int swap_sw )
{
	int tyukan[]   = { 1, 3, 11, };
	int kakutei[]  = { 11, 3, };

	int* list = 0;
	int max = 0;

	if( curset.IsTyukanShinkoku() ){
		list = tyukan;
		max = ARRAYCOUNT(tyukan);
	}else{
		list = kakutei;
		max = ARRAYCOUNT(kakutei);
	}

	vector<int> vec;
	for(int i = 0;i<max;i++)
		vec.push_back( list[i] );

	return makemap(vec,swap_sw);
}

map<int,CString> GetMcountName(int Skbn )
{
	map<int,CString> McountName;

	pair<int,CString> list[] = {
		pair<int,CString>(11,"1か月"),
		pair<int,CString>(3,"3か月"),
		pair<int,CString>(1,"6か月"),
	};

	for(int i = 0;i<ARRAYCOUNT(list);i++)
		McountName.insert(list[i]);

	return McountName;
}

int CDBSyzShinMainView::ReportGetCount()
{
	return m_PastReport.GetRecords()->GetCount();
}

int CDBSyzShinMainView::IsExistsHistory()
{
	return ReportGetCount();
}

void CDBSyzShinMainView::CreateShininfo(CDatabase* db)
{
	CString sq;
	sq += "IF OBJECTPROPERTY ( object_id('ShinInfo'),'ISTABLE') IS NULL \n";
	sq += "	CREATE TABLE [dbo].[shininfo]( ";
	sq += "		[seq] [int] NOT NULL, ";
	sq += "		[updateday] [int] NOT NULL, ";
	sq += "		[ind_type] [int] NOT NULL, ";
	sq += "		[S_SGN2] [int] NOT NULL, ";
	sq += "		[S_SGN4] [int] NOT NULL,";
	sq += "		[SVsasw] [tinyint] NULL,";
	sq += "		[SVmzsw] [tinyint] NULL,";
	sq += "		[SVhksw] [tinyint] NULL,";
	sq += "		[SVd5sw] [tinyint] NULL,";
	sq += "		[SVkojn] [tinyint] NULL,";
	sq += "		[SVzchg] [tinyint] NULL,";
	sq += "		[SVkazei] [tinyint] NULL,";
	sq += "		[SVsacq] [tinyint] NULL,";
	sq += "		[SVzisq] [tinyint] NULL,";
	sq += "		[SVsign] [tinyint] NULL,";
	sq += "		[SVzuri] [decimal](14, 0) NULL";
	sq += "		) \n";

	db->ExecuteSQL( sq ); 
}

int CDBSyzShinMainView::SavePastShinkokuData(int delflg)
{
	CString	ErMs;
	// 変更前の区分に情報を書き込まないように場所移動
	//if( m_pMainFrame->CloseSzsnk( 0, ErMs, FALSE ) == ERR ){
	//	return 1;
	//}

	CDatabase* db = m_pZmSub->m_database;

	db->BeginTrans();
	try{

		db->ExecuteSQL( GetQuery("Sn_Seq","szsnk") ); 				// 消費税申告書
		db->ExecuteSQL( GetQuery("seq","szskj") ); 					// 仕入控除税額に関する明細書
		db->ExecuteSQL( GetQuery("SkjSaveDay","skjrec") ); 			// 仕入控除税額に関する明細書（次葉）
		db->ExecuteSQL( GetQuery("KPSequ","refundvol") ); 			// 消費税額の還付申告に関する明細書
		db->ExecuteSQL( GetQuery("KPSequ","refundrec") ); 			// 消費税額の還付申告に関する明細書（次葉）
		db->ExecuteSQL( GetQuery("KSkksw","reask") );				// 消費税及び地方消費税の更正の請求書
		db->ExecuteSQL( GetQuery("kssgn5","Kjsnk") ); 				// 課税取引金額計算表
		db->ExecuteSQL( GetQuery("NFSequ","ShinPayment") );			// 納付
		db->ExecuteSQL( GetQuery("spseq","splist") );				// 特定収入計算表（非営利のみ）
		db->ExecuteSQL( GetQuery("sn_seq","SYZ_SNK_LST_VAL") );		// 消費税申告書(金額)
		db->ExecuteSQL( GetQuery("sn_seq","SYZ_SNK_LST_RATE") );	// 消費税申告書(割合)
//2018.03.13 INSERT START
		db->ExecuteSQL( GetQuery("Sn_SEQ","fhyo60rec") );			// 付表６
		db->ExecuteSQL( GetQuery("Sn_SEQ","syz_sozoku") );			// 付表６　相続人
//2018.03.13 INSERT END

		db->ExecuteSQL( GetQuery("Sn_SEQ","syz_tokurei") );			// 特例帳票

		int chnge = CheckChange();
		if( chnge != 1 ){
			db->ExecuteSQL( GetQueryDropOnly("Sn_SEQ","syz_tokurei") );		// 特例帳票
		}

		CreateShininfo(db);
		CTime CurTime = CTime::GetCurrentTime();
		CString NowDay = CurTime.Format( "%Y%m%d" );

		CString sq;
		sq += "declare @Sn_Seq int \n";
		sq += "set @Sn_Seq  = 1 \n";

		// 履歴保存分のみzvol関係やszvolの要素を保存する （※複数レコードをもたないテーブルのため）
		CString ind_type,S_SGN2,S_SGN4;
		ind_type.Format(",%d",m_pZmSub->zvol->ind_type);
//		S_SGN2.Format(",%d",Vol1.S_SGN2);
		S_SGN2.Format(",%d",m_pSnHeadData->m_s_sgn2);
//		S_SGN4.Format(",%d",Vol1.S_SGN4);
		S_SGN4.Format(",%d",m_pSnHeadData->m_s_sgn4);

		CString SVsasw,SVmzsw,SVhksw,SVd5sw,SVkojn,SVzchg,SVkazei,SVsacq,SVzisq,SVsign,SVzuri;
		/*SVsasw.Format(",%d",m_pSnHeadData->SVsasw);
		SVmzsw.Format(",%d",m_pSnHeadData->SVmzsw);
		SVhksw.Format(",%d",m_pSnHeadData->SVhksw);
		SVd5sw.Format(",%d",m_pSnHeadData->SVd5sw);
		SVkojn.Format(",%d",m_pSnHeadData->SVkojn);
		SVzchg.Format(",%d",m_pSnHeadData->SVzchg);
		SVkazei.Format(",%d",m_pSnHeadData->SVkazei);
		SVsacq.Format(",%d",m_pSnHeadData->SVsacq);
		SVzisq.Format(",%d",m_pSnHeadData->SVzisq);
		SVsign.Format(",%d",m_pSnHeadData->SVsign);*/
		// 履歴データは、前回申告区分の情報から作成する（区分切替時の変更分を保存しないように）
		SVsasw.Format(",%d",pastset.SVsasw);
		SVmzsw.Format(",%d",pastset.SVmzsw);
		SVhksw.Format(",%d",pastset.SVhksw);
		SVd5sw.Format(",%d",pastset.SVd5sw);
		SVkojn.Format(",%d",pastset.SVkojn);
		SVzchg.Format(",%d",pastset.SVzchg);
		SVkazei.Format(",%d",pastset.SVkazei);
		SVsacq.Format(",%d",pastset.SVsacq);
		SVzisq.Format(",%d",pastset.SVzisq);
		SVsign.Format(",%d",pastset.SVsign);
			
		char money[128] = {};
		m_Arith.l_print( money, m_pSnHeadData->SVzuri, "SSSSSSSSSSSSS9" );
		SVzuri.Format(",'%s'",money);

		sq += "update ShinInfo set seq = seq + 1 \n";
		sq += "INSERT INTO ShinInfo SELECT Sn_Seq, "+ NowDay + ind_type + S_SGN2 + S_SGN4 + SVsasw + SVmzsw + SVhksw + SVd5sw + SVkojn + SVzchg + SVkazei + SVsacq + SVzisq + SVsign + SVzuri + " \n";
		sq += "	from    szsnk \n";
		sq += "	where   Sn_Seq = @Sn_Seq \n";

		sq += "update szsnk set sn_sign4 = ( select sn_sign4|129 from szsnk where sn_seq = @Sn_Seq ) where sn_seq = @Sn_Seq  \n";

		db->ExecuteSQL( sq );

	}catch(CDBException* dbe){
		ICSMessageBox("更新に失敗しました！\nデータは変更されません。",0, 0, 0, this);
		db->Rollback();
		dbe->Delete();
		return -1;
	}

	db->CommitTrans();

	if( m_pMainFrame->CloseSzsnk( 0, ErMs, FALSE ) == ERR ){
		return 1;
	}

	//-->追加 harino ('14.03.26)
	// 今回の基本情報の変更点を履歴に反映させないように、前情報で上書きする
	unsigned char tumiag = m_pSnHeadData->Sn_TUMIAG;
	unsigned char sign2 = m_pSnHeadData->Sn_Sign2;
	unsigned char sign4 = m_pSnHeadData->Sn_Sign4;
	unsigned char svmzsw = m_pSnHeadData->SVmzsw;
	unsigned char special = m_pSnHeadData->Sn_SPECIAL;
//--> '15.08.21 INS START
	long	svDispTabSgn = m_pSnHeadData->m_DispTabSgn;
//<-- '15.08.21 INS END

	m_pSnHeadData->Sn_TUMIAG = pastset.Sn_TUMIAG;
	m_pSnHeadData->Sn_Sign2 = pastset.Sn_Sign2;
	m_pSnHeadData->Sn_Sign4 = pastset.Sn_Sign4 | 129;
	m_pSnHeadData->SVmzsw = pastset.SVmzsw;
	if(pastset.Sn_SPECIAL&0x01) m_pSnHeadData->Sn_SPECIAL |= 0x01;
	else m_pSnHeadData->Sn_SPECIAL &= (~0x01);
//--- '15.11.18 ---
// この情報（m_DispTabSgn）は、次回申告に進む段階では変更されないので今の情報を履歴に保存する
//--> '15.08.21 INS START
//	m_pSnHeadData->m_DispTabSgn = pastset.m_DispTabSgn;
//<-- '15.08.21 INS END
	m_pSnHeadData->m_DispTabSgn = pastset.m_DispTabSgn;	// 次回申告に進む段階で履歴が書き変わってしまうので、書き変わらないようにコメント復活（特定課税仕入れ） ('19.08.02)
//-----------------
	
	m_pSnHeadData->SetData( m_pZmSub, 1 );

	m_pSnHeadData->Sn_TUMIAG = tumiag;
	m_pSnHeadData->Sn_Sign2 = sign2;
	m_pSnHeadData->Sn_Sign4 = sign4;
	m_pSnHeadData->SVmzsw = svmzsw;
	m_pSnHeadData->Sn_SPECIAL = special;
//--> '15.08.21 INS START
	m_pSnHeadData->m_DispTabSgn = svDispTabSgn;
//<-- '15.08.21 INS END
	//<--追加 harino ('14.03.26)

	return 0;
}

//-- '14.09.10 --
//int CDBSyzShinMainView::DropHistoryAll()
//{
//	if( ICSMessageBox("最終申告分のみを残し履歴関連情報をすべて削除します。\n",(MB_YESNO | MB_DEFBUTTON2)) != IDYES) return 1;
//
//	CDatabase* db = m_pZmSub->m_database;
//
//	db->BeginTrans();
//	try {
//	
//		db->ExecuteSQL( GetQueryAllDrop("Sn_Seq","szsnk") ); 			// 消費税申告書
//		db->ExecuteSQL( GetQueryAllDrop("seq","szskj") ); 				// 仕入控除税額に関する明細書
//		db->ExecuteSQL( GetQueryAllDrop("SkjSaveDay","skjrec") ); 		// 仕入控除税額に関する明細書（次葉）
//		db->ExecuteSQL( GetQueryAllDrop("KPSequ","refundvol") ); 		// 消費税額の還付申告に関する明細書
//		db->ExecuteSQL( GetQueryAllDrop("KPSequ","refundrec") ); 		// 消費税額の還付申告に関する明細書（次葉）
//		db->ExecuteSQL( GetQueryAllDrop("KSkksw","reask") );			// 消費税及び地方消費税の更正の請求書
//		db->ExecuteSQL( GetQueryAllDrop("kssgn5","Kjsnk") ); 			// 課税取引金額計算表
//		db->ExecuteSQL( GetQueryAllDrop("NFSequ","ShinPayment") );		// 納付
//		db->ExecuteSQL( GetQueryAllDrop("spseq","splist") );			// 特定収入計算表（非営利のみ）
//		db->ExecuteSQL( GetQueryAllDrop("sn_seq","SYZ_SNK_LST_VAL") );	// 消費税申告書(金額)
//		db->ExecuteSQL( GetQueryAllDrop("sn_seq","SYZ_SNK_LST_RATE") );	// 消費税申告書(割合)
//		db->ExecuteSQL("update szsnk set sn_sign4 = ( select sn_sign4&254 from szsnk where sn_seq = 0 ) where sn_seq = 0 ");
//		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shininfo'),'ISTABLE') IS NOT NULL drop table shininfo");
//		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shinminfo'),'ISTABLE') IS NOT NULL drop table shinminfo");
//		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shinkinfo'),'ISTABLE') IS NOT NULL drop table shinkinfo");
//
//	}catch(CDBException* dbe){
//		dbe->Delete();
//	}
//
//	db->CommitTrans();
//	CleateShinMininfo();
//
//	return 0;
//}
//---------------
//--- '15.01.13 ---
//int CDBSyzShinMainView::DropHistoryAll( CString& msg )
//{
//	if( ICSMessageBox("最終申告分のみを残し履歴関連情報をすべて削除します。\n",(MB_YESNO | MB_DEFBUTTON2)) != IDYES) return 1;
//
//	CDatabase* db = m_pZmSub->m_database;
//
//	db->BeginTrans();
//	try {
//	
//		db->ExecuteSQL( GetQueryAllDrop("Sn_Seq","szsnk") ); 			// 消費税申告書
//		db->ExecuteSQL( GetQueryAllDrop("seq","szskj") ); 				// 仕入控除税額に関する明細書
//		db->ExecuteSQL( GetQueryAllDrop("SkjSaveDay","skjrec") ); 		// 仕入控除税額に関する明細書（次葉）
//		db->ExecuteSQL( GetQueryAllDrop("KPSequ","refundvol") ); 		// 消費税額の還付申告に関する明細書
//		db->ExecuteSQL( GetQueryAllDrop("KPSequ","refundrec") ); 		// 消費税額の還付申告に関する明細書（次葉）
//		db->ExecuteSQL( GetQueryAllDrop("KSkksw","reask") );			// 消費税及び地方消費税の更正の請求書
//		db->ExecuteSQL( GetQueryAllDrop("kssgn5","Kjsnk") ); 			// 課税取引金額計算表
//		db->ExecuteSQL( GetQueryAllDrop("NFSequ","ShinPayment") );		// 納付
//		db->ExecuteSQL( GetQueryAllDrop("spseq","splist") );			// 特定収入計算表（非営利のみ）
//		db->ExecuteSQL( GetQueryAllDrop("sn_seq","SYZ_SNK_LST_VAL") );	// 消費税申告書(金額)
//		db->ExecuteSQL( GetQueryAllDrop("sn_seq","SYZ_SNK_LST_RATE") );	// 消費税申告書(割合)
//		db->ExecuteSQL("update szsnk set sn_sign4 = ( select sn_sign4&254 from szsnk where sn_seq = 0 ) where sn_seq = 0 ");
//		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shininfo'),'ISTABLE') IS NOT NULL drop table shininfo");
//		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shinminfo'),'ISTABLE') IS NOT NULL drop table shinminfo");
//		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shinkinfo'),'ISTABLE') IS NOT NULL drop table shinkinfo");
//
//	}catch(CDBException* dbe){
//		db->Rollback();
//		msg.Format( _T("履歴関連情報の削除に失敗しましたので処理を中止します。\n%s"), dbe->m_strError );
//		dbe->Delete();
//		return -1;
//	}
//
//	db->CommitTrans();
//	CleateShinMininfo();
//
//	return 0;
//}
//-----------------
int CDBSyzShinMainView::DropHistoryAll( CString& msg, BOOL isExistHistory )
{
	if( isExistHistory ){
		if( ICSMessageBox( "最終申告分のみを残し履歴関連情報をすべて削除します。\n",(MB_YESNO | MB_DEFBUTTON2)) != IDYES){
			return 1;
		}
	}
	else{
		ICSMessageBox( "最終申告分のみを残し関連情報をすべて削除します。\n", MB_OK );
	}

	ASSERT( m_pZmSub );
	CDatabase* db = m_pZmSub->m_database;
	ASSERT( db );
	if( db == NULL ){
		msg.Format( _T("ＤＢがオープンされていませんでしたので処理を中止します。") );
		return -1;
	}

	db->BeginTrans();
	try {
	
		db->ExecuteSQL( GetQueryAllDrop("Sn_Seq","szsnk") ); 			// 消費税申告書
		db->ExecuteSQL( GetQueryAllDrop("seq","szskj") ); 				// 仕入控除税額に関する明細書
		db->ExecuteSQL( GetQueryAllDrop("SkjSaveDay","skjrec") ); 		// 仕入控除税額に関する明細書（次葉）
		db->ExecuteSQL( GetQueryAllDrop("KPSequ","refundvol") ); 		// 消費税額の還付申告に関する明細書
		db->ExecuteSQL( GetQueryAllDrop("KPSequ","refundrec") ); 		// 消費税額の還付申告に関する明細書（次葉）
		db->ExecuteSQL( GetQueryAllDrop("KSkksw","reask") );			// 消費税及び地方消費税の更正の請求書
		db->ExecuteSQL( GetQueryAllDrop("kssgn5","Kjsnk") ); 			// 課税取引金額計算表
		db->ExecuteSQL( GetQueryAllDrop("NFSequ","ShinPayment") );		// 納付
		db->ExecuteSQL( GetQueryAllDrop("spseq","splist") );			// 特定収入計算表（非営利のみ）
		db->ExecuteSQL( GetQueryAllDrop("sn_seq","SYZ_SNK_LST_VAL") );	// 消費税申告書(金額)
		db->ExecuteSQL( GetQueryAllDrop("sn_seq","SYZ_SNK_LST_RATE") );	// 消費税申告書(割合)
//2018.03.13 INSERT START
		db->ExecuteSQL( GetQueryAllDrop("Sn_SEQ","fhyo60rec") );		// 付表６
		db->ExecuteSQL( GetQueryAllDrop("Sn_SEQ","syz_sozoku") );		// 付表６　相続人
//2018.03.13 INSERT END

		db->ExecuteSQL( GetQueryAllDrop("Sn_SEQ","syz_tokurei") );		// 特例帳票

		db->ExecuteSQL("update szsnk set sn_sign4 = ( select sn_sign4&254 from szsnk where sn_seq = 0 ) where sn_seq = 0 ");
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shininfo'),'ISTABLE') IS NOT NULL drop table shininfo");
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shinminfo'),'ISTABLE') IS NOT NULL drop table shinminfo");
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('shinkinfo'),'ISTABLE') IS NOT NULL drop table shinkinfo");

		// 10日間営業日のテーブルもクリア
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('simku_eigyobi'),'ISTABLE') IS NOT NULL drop table simku_eigyobi");
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('simku_kyujitsu'),'ISTABLE') IS NOT NULL drop table simku_kyujitsu");
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('simku_syukeitenki'),'ISTABLE') IS NOT NULL drop table simku_syukeitenki");
		db->ExecuteSQL("IF OBJECTPROPERTY ( object_id('simku_syukujitsu'),'ISTABLE') IS NOT NULL drop table simku_syukujitsu");
		// simku_settingholidayはクリアしない


	}catch(CDBException* dbe){
		db->Rollback();
		msg.Format( _T("履歴関連情報の削除に失敗しましたので処理を中止します。\n%s"), dbe->m_strError );
		dbe->Delete();
		return -1;
	}

	db->CommitTrans();
	CleateShinMininfo();

	return 0;
}
//-----------------
//---------------

int CDBSyzShinMainView::DropHistory()
{
	if( !IsExistsHistory() ) return 0;

	if( IsHistoryMode() ) HistoryModeOff();

	CHistoryReport::RecType rec = HistoryVec[0];

	CString mes;
	mes.Format( "現在申告分を削除後、履歴保存されている「%s : %s」に戻ります。\n",rec.str_kubun, rec.str_kikan );
	mes += "よろしいですか？（入力ロックは解除されます。）";

	if( ICSMessageBox(mes,(MB_YESNO | MB_DEFBUTTON2)) != IDYES) return 0;

	SetOldData();

	CDatabase* db = m_pZmSub->m_database;

	db->BeginTrans();
	try{
		db->ExecuteSQL( GetQueryDrop("Sn_Seq","szsnk") ); 				// 消費税申告書
		db->ExecuteSQL( GetQueryDrop("seq","szskj") ); 					// 仕入控除税額に関する明細書
		db->ExecuteSQL( GetQueryDrop("SkjSaveDay","skjrec") ); 			// 仕入控除税額に関する明細書（次葉）
		db->ExecuteSQL( GetQueryDrop("KPSequ","refundvol") ); 			// 消費税額の還付申告に関する明細書
		db->ExecuteSQL( GetQueryDrop("KPSequ","refundrec") ); 			// 消費税額の還付申告に関する明細書（次葉）
		db->ExecuteSQL( GetQueryDrop("KSkksw","reask") );				// 消費税及び地方消費税の更正の請求書
		db->ExecuteSQL( GetQueryDrop("kssgn5","Kjsnk") ); 				// 課税取引金額計算表
		db->ExecuteSQL( GetQueryDrop("NFSequ","ShinPayment") ); 		// 納付
		db->ExecuteSQL( GetQueryDrop("spseq","splist") );				// 特定収入計算表（非営利のみ）
		db->ExecuteSQL( GetQueryDrop("sn_seq","SYZ_SNK_LST_VAL") );		// 消費税申告書(金額)
		db->ExecuteSQL( GetQueryDrop("sn_seq","SYZ_SNK_LST_RATE") );	// 消費税申告書(割合)
//2018.03.13 INSERT START
		db->ExecuteSQL( GetQueryDrop("Sn_SEQ","fhyo60rec") );			// 付表６
		db->ExecuteSQL( GetQueryDrop("Sn_SEQ","syz_sozoku") );			// 付表６　相続人
//2018.03.13 INSERT END

		db->ExecuteSQL( GetQueryDrop("Sn_SEQ","syz_tokurei") );			// 特例帳票


		db->ExecuteSQL("update ShinInfo set seq = seq - 1 ");
		db->ExecuteSQL("delete from shininfo where seq = 0 ");

		db->ExecuteSQL("update szsnk set sn_sign4 = ( select sn_sign4&254 from szsnk where sn_seq = 0 ) where sn_seq = 0 ");

		db->ExecuteSQL("IF( 0 = ( select count(*) from shininfo ) ) drop table shininfo");

	}catch(CDBException* dbe){
		ICSMessageBox("更新に失敗しました！\nデータは変更されません。",0, 0, 0, this);
		db->Rollback();
		dbe->Delete();
		return -1;
	}

	db->CommitTrans();

	//--> '14.03.10 INS START
	// 入力ロック解除
	rec.GetSnset().Sn_Sign4 &= ~0x01;
	//<-- '14.03.10 INS END

//--> '15.07.25 INS START
	if( m_pMainFrame ){
		CString	msg;
		m_pSnHeadData->TakeInMynumber( m_pZmSub, m_pMainFrame->GetTgData(), msg, NTBMOD_GYFLG_SYOHI );
	}
//<-- '15.07.25 INS END

//--> '14.03.19 INS START
	// 当期データ再読み込み
	m_pSnHeadData->GetData( m_pZmSub, 0/*当期シーケンス*/ );
//<-- '14.03.19 INS START

	rec.GetSnset().GetSnData(*m_pSnHeadData );

	m_Drophistorysw = TRUE;//19.11.07

	HistoryReportInit();

	m_Drophistorysw = FALSE;//19.11.07

	unsigned char newver;
	if (CheckKaiseiVER(newver)) {
		m_pMainFrame->KikanChange(newver);
	}

//	SyzShin_UpDate();
	if( m_pMainFrame ){
		m_pMainFrame->UpdateSyzShin();
		// データ再読込み
		m_pMainFrame->ReReadData();
		m_pMainFrame->SetCompChangeFlg( TRUE );
	}
	

//--> '15.07.21 INS START
	InitMynumberControl();
//<-- '15.07.21 INS END

//	memmove( &SNHeadEx, &Snh, sizeof( SH_SN_HEAD ) );
//	memmove( &SNDataEx, &Snd, sizeof( SH_SN_DATA ) );
	return 0;
}

int CDBSyzShinMainView::SetKikan()
{
	//unsigned char Sn_KDAYS[3] = {0};
	//unsigned char Sn_KDAYE[3] = {0};
	//unsigned char Sn_MDAYS[3] = {0};
	//unsigned char Sn_MDAYE[3] = {0};
	unsigned char Sn_KDAYS[4] = {0};
	unsigned char Sn_KDAYE[4] = {0};
	unsigned char Sn_MDAYS[4] = {0};
	unsigned char Sn_MDAYE[4] = {0};

	TimePair k = GetCurrentKessanKikan();
	if( k == TimePair() )	return -1;
	CTimeControl::SetDateYMD(Sn_KDAYS, Sn_KDAYE, k);

	if( curset.IsTyukanShinkoku() ){
		TimePair t = GetCurrentTyukanKikan();
		if( t == TimePair() )	return -1;
		CTimeControl::SetDateYMD(Sn_MDAYS, Sn_MDAYE, t );
	}
	//m_pSnHeadData->SetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, 0, Sn_KDAYS[0] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, 1, Sn_KDAYS[1] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, 2, Sn_KDAYS[2] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_KAZEIKIKAN_TO, 0, Sn_KDAYE[0] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_KAZEIKIKAN_TO, 1, Sn_KDAYE[1] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_KAZEIKIKAN_TO, 2, Sn_KDAYE[2] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_CHUUKAN_FROM, 0, Sn_MDAYS[0] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_CHUUKAN_FROM, 1, Sn_MDAYS[1] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_CHUUKAN_FROM, 2, Sn_MDAYS[2] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_CHUUKAN_TO, 0, Sn_MDAYE[0] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_CHUUKAN_TO, 1, Sn_MDAYE[1] );
	//m_pSnHeadData->SetYmdData( ID_ICSSH_CHUUKAN_TO, 2, Sn_MDAYE[2] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, 0, Sn_KDAYS[1] , Sn_KDAYS[0] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, 1, Sn_KDAYS[2] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, 2, Sn_KDAYS[3] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, 0, Sn_KDAYE[1], Sn_KDAYE[0] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, 1, Sn_KDAYE[2] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, 2, Sn_KDAYE[3] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, 0, Sn_MDAYS[1], Sn_MDAYS[0] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, 1, Sn_MDAYS[2] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, 2, Sn_MDAYS[3] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_CHUUKAN_TO, 0, Sn_MDAYE[1], Sn_MDAYE[0] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_CHUUKAN_TO, 1, Sn_MDAYE[2] );
	m_pSnHeadData->SetYmdDataGen( ID_ICSSH_CHUUKAN_TO, 2, Sn_MDAYE[3] );

	return 0;
}

int RenewSkip(int pastkubun,int curkubun)
{
//2018.03.13 UPDATE START
//	if( pastkubun == kakutei && ( curkubun == kousei || curkubun == kakutei_syuuse ) ) return 1;
	if( (pastkubun == kakutei||pastkubun == junkakutei) && ( curkubun == kousei || curkubun == kakutei_syuuse ) ) return 1;
//2018.03.13 UPDATE END
	if( pastkubun == kakutei_syuuse && curkubun == kousei )  return 1;
//2022.07.26 19-0032 INSERT START
	if( pastkubun == kousei && curkubun == kakutei_syuuse )  return 1;
//2022.07.26 19-0032 INSERT END
	if( pastkubun == tyuukan && curkubun == tyuukan_syuuse )  return 1;
//----> 20-0846 add
	if( pastkubun == kakutei && curkubun == junkakutei )  return 1;
//<---- 20-0846 add
	return 0;
}

int	CDBSyzShinMainView::MakeJikaiShinkokuData(int save)
{
	if( SetKikan() ) return -1;

//--> '14.03.15 INS START
	long	symd=0;
	//---->特定課税仕入対応150713
	long	eymd=0;
	//<-----------------------------
	switch( m_pSnHeadData->Sn_SKKBN&0xff ){
		case 1 : case 3 :	// 確定・確定修正
			symd = m_pSnHeadData->Sn_KDAYS;
			//---->特定課税仕入対応150713
			eymd = m_pSnHeadData->Sn_KDAYE;
			//<-----------------------------
			break;
		default:			//中間・中間修正
			symd = m_pSnHeadData->Sn_MDAYS;
			//---->特定課税仕入対応150713
			eymd = m_pSnHeadData->Sn_MDAYE;
			//<-----------------------------
			break;
	}

//--> '15.08.27 INS START
	// 特定課税仕入
	SetTkkzSign( eymd );
	// 改正様式
	BOOL IsNeedReRead = FALSE;
	EnumIdIcsShRevType tmpShRevType = m_ShRevType;
//-- '15.11.17 --
////	m_ShRevType = GetShRevType();
//	m_ShRevType = GetShRevTypeFromSnset( curset );
//---------------
	m_ShRevType = GetShRevType();
//---------------
	if( tmpShRevType != m_ShRevType ){
		if( m_pMainFrame ){
			m_pMainFrame->SetShRevType( m_ShRevType );
			IsNeedReRead = TRUE;
		}
	}
//<-- '15.08.27 INS END

	BOOL	tmpSixKindFlg = FALSE;
//-- '15.05.01 --
//	if( symd >= ICS_SH_SIXKIND_REFORM_DAY ){
//		tmpSixKindFlg = TRUE;
//	}
//	else{
//		tmpSixKindFlg = FALSE;
//	}
//---------------
	tmpSixKindFlg = ((CMainFrame*)GetParentOwner())->IsSixKindKani(symd);
//---------------

	if( m_SixKindFlg != tmpSixKindFlg ){
		m_SixKindFlg = tmpSixKindFlg;
		if( m_pMainFrame ){
			m_pMainFrame->SetSixKindFlg( m_SixKindFlg );
//-- '15.08.27 --
//			m_pMainFrame->ReReadData();
//---------------
			IsNeedReRead = TRUE;
//---------------
		}
	}
//<-- '14.03.15 INS END

//--> '15.08.27 INS START
	if( IsNeedReRead == TRUE ){
		if( m_pMainFrame ){
			m_pMainFrame->ReReadData();
		}
	}
//<-- '15.08.27 INS END

	// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する
//2018.03.13 UPDATE START
//	if( (pastset.GetKubun() == kakutei ) && (curset.GetKubun() == kakutei_syuuse ) || (pastset.GetKubun() == tyuukan ) && (curset.GetKubun() == tyuukan_syuuse ) )	{
	if( ((pastset.GetKubun() == kakutei )||(pastset.GetKubun() == junkakutei )) && (curset.GetKubun() == kakutei_syuuse ) || (pastset.GetKubun() == tyuukan ) && (curset.GetKubun() == tyuukan_syuuse ) )	{
//2018.03.13 UPDATE END

//★//[13'05.21]///
//		if( l_test( Snd.Sn_1F14C ) < 0 )	{
//			// 過不足
//			memmove( Snd.Sn_KAKTIZ, Snd.Sn_1F14C, 6 );
//			memset( WORK0, '\0', 6 );
//			l_input( WORK0, "250" );
//			memset( WORK1, '\0', 6 );
//			pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)Snd.Sn_1F12B, (unsigned char *)WORK0, 1, 0 );
//			l_neg( WORK1 );
//			memmove( Snd.Sn_TKAKTZ, WORK1, 6 );
//		}
//		else	{
//			// 納税
//			memmove( Snd.Sn_KAKTIZ, Snd.Sn_EDNOFZ, 6 );
//			memmove( Snd.Sn_TKAKTZ, Snd.Sn_TEDNOF, 6 );
//		}
///////////////////
		// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備

		// 前回申告区分のチェック
		CString	ymdStr;
		int pastEymd = 0;
//2018.03.13 UPDATE START
//		if( pastset.GetKubun() == kakutei ){
		if(( pastset.GetKubun() == kakutei )||( pastset.GetKubun() == junkakutei )){
//2018.03.13 UPDATE END			
			ymdStr = pastset.GetSetKessanKikan().second.Format("%Y%m%d");
			pastEymd = atoi( ymdStr );
		}
		else if( pastset.GetKubun() == tyuukan ){
			ymdStr = pastset.GetSetTyukanKikan().second.Format("%Y%m%d");
			pastEymd = atoi( ymdStr );
		}
		int type = 1;
		if( pastEymd && (pastEymd<20140401) ){
			type = 0;
		}
		AlreadyDecisionCalc( type );
///////////////////
	}

	// 確定申告時 中間納税額の転記
/*- '14.04.11 -*/
//	if( ( curset.IsKakuteiShinkoku() && pastset.IsTyukanShinkoku() ) || ( pastset.IsKakuteiShinkoku() && curset.IsKakuteiShinkoku() ))	{
/*-------------*/
	if( ( curset.IsKakuteiShinkoku() && pastset.IsTyukanShinkoku() ) ){
/*-------------*/
		CArith arith(0x16);
		char KMPvalTotal[6] = {};
		char TMPvalTotal[6] = {};
		vector<MPAYMENTVAL> mpvec = MiddlePaymentRead();
		if( mpvec.size() ) {
			for(UINT i = 0;i<mpvec.size();i++){
				arith.l_add(KMPvalTotal,KMPvalTotal,mpvec[i].KMPval);
				arith.l_add(TMPvalTotal,TMPvalTotal,mpvec[i].TMPval);
			}
/*- '14.02.21 -*/
//			memmove( Snd.Sn_TYNOFZ,KMPvalTotal,6 );
//			memmove( Snd.Sn_TTYWAR,TMPvalTotal,6 );
/*-------------*/
			CString	tag, chtag;
			if (m_ShRevType < ID_ICSSH_10PER_REVTYPE) {
				tag = _T("AAB00110");
				SetMony(tag, chtag, KMPvalTotal);
				tag = _T("Sn_TTYWAR");
				SetMony(tag, chtag, TMPvalTotal);
			}
			else {
				// 31
				tag = _T("AAJ00110");
				SetMony(tag, chtag, KMPvalTotal);
				tag = _T("AAK00070");
				SetMony(tag, chtag, TMPvalTotal);
			}
/*-------------*/
		}
	}

	// 前回が確定で今回が更正の請求の場合
//2018.03.13 UPDATE START
//	if( (pastset.GetKubun() == kakutei && ( curset.GetKubun() == kousei || curset.GetKubun() == kakutei_syuuse ) ||
	if( (  (pastset.GetKubun() == kakutei||pastset.GetKubun() == junkakutei) && ( curset.GetKubun() == kousei || curset.GetKubun() == kakutei_syuuse ) ||
//2018.03.13 UPDATE END
		(pastset.GetKubun() == kakutei_syuuse && curset.GetKubun() == kousei ) )){
		if( m_pMainFrame ){
//			ShzBillValSet( 0, 0 );
			m_pMainFrame->CalqSyzForKsk( 0 );
		}
	}

	// 前回：更正→今回：修正確定の場合（19-0032/160231）
	if( ( pastset.GetKubun() == kousei ) && ( curset.GetKubun() == kakutei_syuuse ) ){
		
		// 本表からではなく更正の請求から金額を取得する（非連動で更正の請求の金額を変更していた場合を考慮して）
		if ( GetActViewVer() == ID_VER_SYZ31 ) {
			CH31HyoView	*pH31HyoView = NULL;
			pH31HyoView = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
			if (pH31HyoView != NULL) {
				pH31HyoView->CalqSyzFromKsk();
			}
		}
	}

	//----->('24.07.23 Add)
	// 前回：予定の場合、今回分の本表 提出日をクリアする[23-0203]
	if( ( pastset.GetKubun() == yotei ) ) {
		if ( GetActViewVer() == ID_VER_SYZ31 ) {
			CH31HyoView	*pH31HyoView = NULL;
			pH31HyoView = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
			if (pH31HyoView != NULL) {
				pH31HyoView->ChgPastYoteiSgn( TRUE );
			}
		}
	}
	//<-----

	// 次の申告区分を作成
	int Flg = 0;
	if( !(m_pSnHeadData->Sn_SKKBN%2) ){
		Flg = m_pSnHeadData->Sn_MCOUNT;
	}
	else{
		// 期間短縮
		switch( m_pSnHeadData->Sn_KZTNSK&0x0f ) {
			case 0x01 :	Flg = 3;		break;	// ３ヶ月
			case 0x02 :	Flg = 11;		break;	// １ヶ月
			default   :	Flg = 0;		break;	// 無し
		}
	}

//--> '17.01.19 INS START
	// ﾛｯｸ解除
	curset.Sn_Sign4 &= 0xfe;
	m_pSnHeadData->Sn_Sign4 &= 0xfe;
//<-- '17.01.19 INS END

	// 付表6 令和2年4月1日改正前の様式で出力のチェックをOFF
	m_pSnHeadData->Sn_GenericSgn &= ~0x20;

	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		// 既確定税額のクリア
		if( ((pastset.GetKubun() == tyuukan_syuuse ) && (curset.GetKubun() == tyuukan )) ||
//2018.03.13 UPDATE START
//		    ((pastset.GetKubun() == kakutei_syuuse ) && (curset.GetKubun() == kakutei )) ){
		    ((pastset.GetKubun() == kakutei_syuuse ) && ((curset.GetKubun() == kakutei )||(curset.GetKubun() == junkakutei ))) ){
//2018.03.13 UPDATE END
//			memset(Snd.Sn_KAKTIZ,0,6);
//			memset(Snd.Sn_TKAKTZ,0,6);
//--> '15.01.28 INS START
			ClearAlreadyDecision();
//<-- '15.01.28 INS END
		}

		// 次の申告へ
//		Snd.Sn_BSMON = 0x00;	// 月換算	分子
//		Snd.Sn_BBMON = 0x00;	// 〃　　　 分母
//--> '15.01.28 INS START
		ClearYoteiMonth();
//<-- '15.01.28 INS END
		if( save == 0 )	return 0;	// 申告区分の変更
	}
	else if( curset.Sn_Sign4&0x80 ){
		// 既確定税額のクリア
		if( ((pastset.GetKubun() == tyuukan_syuuse ) && (curset.GetKubun() == tyuukan )) ||
//2018.03.13 UPDATE START
//		    ((pastset.GetKubun() == kakutei_syuuse ) && (curset.GetKubun() == kakutei )) ){
		    ((pastset.GetKubun() == kakutei_syuuse ) && ((curset.GetKubun() == kakutei )||(curset.GetKubun() == junkakutei ))) ){
//2018.03.13 UPDATE END
			ClearAlreadyDecision();
		}
	}

	if( save && ( curset.Sn_Sign4 & 0x80 ) ) {
		if( RenewSkip( pastset.GetKubun(), curset.GetKubun() ) ) return 0;
	}

///--> '17.01.19 CUT START
////2016.06.22 UPDATE START
//	//curset.Sn_Sign4 &= 0x7e; // ﾛｯｸ＆非連動解除
//	//m_pSnHeadData->Sn_Sign4 &= 0x7e;
//	curset.Sn_Sign4 &= 0xfe; // ﾛｯｸ解除
//	m_pSnHeadData->Sn_Sign4 &= 0xfe;
////2016.06.22 UPDATE END
///<-- '17.01.19 CUT END

	return RenewSyzShin();
}

void CDBSyzShinMainView::HistoryReportInitCur( void )
{
	if( !(m_pSnHeadData->SVsasw&0x01) ) {
		GetDlgItem(IDC_STATIC_I06)->SetWindowText("なし");
		return;
	}

	TimePair date;
	if( curset.IsTyukanShinkoku() )
		date = curset.GetSetTyukanKikan();
	else
		date = curset.GetSetKessanKikan();

	CString sq;
	CTime CurTime = CTime::GetCurrentTime();
	CString NowDay = CurTime.Format( "%Y%m%d" );

	CString str;
	str.Format( "%s\r\n%s", curset.GetKubunStr(), CTimeControl::GetDateStrPair(date) );
	
	CString	tmpStr;
	GetDlgItem(IDC_STATIC_I06)->GetWindowText( tmpStr );
	if( tmpStr != str ){
		GetDlgItem(IDC_STATIC_I06)->SetWindowText(str);
	}
}

int CDBSyzShinMainView::CheckShinMinfo()
{
	TimePairVector shin = GetShinMininfovec();
	if( !shin.size() ){
		CleateShinMininfo();
		return 0;
	}
	TimePairVector org  = GetMinfovecOrg();
	return org != shin;
}

TimePairVector CDBSyzShinMainView::GetMinfovecOrg()
{
	TimePairVector minfovec;
	m_pZmSub->minfo->MoveFirst();
//-- '18.12.17 --
//	for(int i = 0; i != m_pZmSub->minfo->GetRecordCount(); i++ ) {
//		if( !m_pZmSub->minfo->kes_sgn ) minfovec.push_back( TimePair( CTimeControl::GetCTime(m_pZmSub->minfo->smd), CTimeControl::GetCTime(m_pZmSub->minfo->emd)));
//		if( m_pZmSub->minfo->emd == m_pZmSub->zvol->ee_ymd ) break;
//		m_pZmSub->minfo->MoveNext();
//	}
//---------------
	for( int i=0;; i++ ){
		if( m_pZmSub->minfo->st==-1 )	break;
		if( !m_pZmSub->minfo->kes_sgn ) minfovec.push_back( TimePair( CTimeControl::GetCTime(m_pZmSub->minfo->smd), CTimeControl::GetCTime(m_pZmSub->minfo->emd)));
		if( m_pZmSub->minfo->emd == m_pZmSub->zvol->ee_ymd ) break;
		m_pZmSub->minfo->MoveNext();
	}
//---------------

	return minfovec;
}

void CDBSyzShinMainView::CleateShinMininfo()
{
	TimePairVector minfovec = GetMinfovecOrg();

	CString sq;
	sq += "IF OBJECTPROPERTY ( object_id('shinminfo'),'ISTABLE') IS NOT NULL BEGIN\n";
	sq += "	DROP TABLE shinminfo \n";
	sq += "END  \n";
	sq += "CREATE TABLE [dbo].[shinminfo]( ";
	sq += "		 [seq] [int] NOT NULL, [smd] [int] NOT NULL, [emd] [int] NOT NULL, ";
	sq += "		 [esmd] [int] NOT NULL, [eemd] [int] NOT NULL ";
	sq += " ) \n";
	for(int i = 0; i != minfovec.size(); i++ ) {
		CString seq,smd,emd;
		seq.Format("%d",i);
		smd = minfovec[i].first.Format("%Y%m%d");
		emd = minfovec[i].second.Format("%Y%m%d");
		sq += "INSERT INTO shinminfo SELECT "+ seq +", " + smd + ", " + emd + ", " + smd + ", " + emd + " \n";
	}

	CDatabase *db = m_pZmSub->m_database;
	try {
		db->ExecuteSQL( sq );
	}catch(CDBException* dbe){
		dbe->Delete();
	}
}

CTime CDBSyzShinMainView::FindTansyukuKaishi()
{
	CTime tansyuku;
	if( curset.IsKakuteiShinkoku() && curset.IsMinashi() ){
		TimePair kessan = curset.GetSetKessanKikan();
		if( kessan.first == CTimeControl::GetCTime(m_pZmSub->zvol->ss_ymd) ) 
			tansyuku = kessan.second + CTimeSpan(1,0,0,0); 
	}

	if( tansyuku != CTime() ) return tansyuku;

	for(UINT i = 0; i < HistoryVec.size();i++){
		CSnset snset = HistoryVec[i].GetSnset();
		if( snset.IsKakuteiShinkoku() && snset.IsMinashi() ){
			TimePair kessan = snset.GetSetKessanKikan();
			if( kessan.first == CTimeControl::GetCTime(m_pZmSub->zvol->ss_ymd) ) 
				return kessan.second + CTimeSpan(1,0,0,0); 
		}
	}
	return CTime();
}

void CDBSyzShinMainView::SetTansyukuKaishi()
{
	CTime tansyuku = FindTansyukuKaishi();
	if( tansyuku != CTime() ) {
		CString mes;
		mes.Format("期間短縮年月日\n%s",CTimeControl::GetDateStr(tansyuku,"%2d年%2d月%2d日") );

		CString	tmpStr;
		GetDlgItem(IDC_STATIC_B59)->GetWindowText(tmpStr);
		if( tmpStr != mes ){
			GetDlgItem(IDC_STATIC_B59)->SetWindowText(mes);
		}
	}
	else{
		CString	tmpStr;
		GetDlgItem(IDC_STATIC_B59)->GetWindowText(tmpStr);
		if( tmpStr.GetLength() ){
			GetDlgItem(IDC_STATIC_B59)->SetWindowText("");
		}
	}
}

void CDBSyzShinMainView::HistoryReportInit( void )
{
	HistoryVec.clear();
	m_CheckTokusyu.SetCheck(0);//期間短縮変更許可チェックボックス初期化
	m_PastReport.Init(GetFont());

	CRecordset dbrec(m_pZmSub->m_database);
	try {
	
		CString sq;
//-- '15.11.16 --
//		sq += "SELECT Sn_MDAYS,Sn_MDAYE,Sn_KDAYS,Sn_KDAYE,Sn_SKKBN,Sn_KZTNSK,Sn_MCOUNT,Sn_JCOUNT,Sn_ZeiHou,Sn_Syuusei,Sn_Sign1,Sn_Sign2,Sn_Sign3,Sn_Sign4,Sn_SPECIAL,Sn_TUMIAG,SVsasw,SVmzsw,SVhksw,SVd5sw,SVkojn,SVzchg,SVkazei,SVsacq,SVzisq,SVsign,SVzuri,ind_type,S_SGN2,S_SGN4,updateday \n";
//---------------
		sq += "SELECT Sn_MDAYS,Sn_MDAYE,Sn_KDAYS,Sn_KDAYE,Sn_SKKBN,Sn_KZTNSK,Sn_MCOUNT,Sn_JCOUNT,Sn_ZeiHou,Sn_Syuusei,Sn_Sign1,Sn_Sign2,Sn_Sign3,Sn_Sign4,Sn_SPECIAL,Sn_TUMIAG, Sn_ADRD8, SVsasw,SVmzsw,SVhksw,SVd5sw,SVkojn,SVzchg,SVkazei,SVsacq,SVzisq,SVsign,SVzuri,ind_type,S_SGN2,S_SGN4,updateday \n";
//---------------
		sq += "FROM szsnk, shininfo \n";
		sq += "WHERE seq = Sn_Seq ORDER BY Sn_Seq \n";
		
		dbrec.Open( CRecordset::forwardOnly, sq );
		while ( dbrec.IsEOF() == FALSE ){
		
			CString Sn_MDAYS,Sn_MDAYE,Sn_KDAYS,Sn_KDAYE,Sn_SKKBN,Sn_KZTNSK,Sn_MCOUNT,Sn_JCOUNT,Sn_ZeiHou,Sn_Syuusei,Sn_Sign1,Sn_Sign2,Sn_Sign3,Sn_Sign4,Sn_SPECIAL,Sn_TUMIAG,SVsasw,SVmzsw,SVhksw,SVd5sw,SVkojn,SVzchg,SVkazei,SVsacq,SVzisq,SVsign,SVzuri,ind_type,S_SGN2,S_SGN4,updateday;
//--> '15.11.16 INS START
			CString	strSn_ADRD8;
//<-- '15.11.16 INS END
	
			dbrec.GetFieldValue( "Sn_MDAYS", Sn_MDAYS );
			dbrec.GetFieldValue( "Sn_MDAYE", Sn_MDAYE );
			dbrec.GetFieldValue( "Sn_KDAYS", Sn_KDAYS );
			dbrec.GetFieldValue( "Sn_KDAYE", Sn_KDAYE );

			dbrec.GetFieldValue( "Sn_SKKBN", Sn_SKKBN );
			dbrec.GetFieldValue( "Sn_KZTNSK", Sn_KZTNSK );
			dbrec.GetFieldValue( "Sn_MCOUNT", Sn_MCOUNT );
			dbrec.GetFieldValue( "Sn_JCOUNT", Sn_JCOUNT );
			dbrec.GetFieldValue( "Sn_ZeiHou", Sn_ZeiHou );
			dbrec.GetFieldValue( "Sn_Syuusei", Sn_Syuusei );
			dbrec.GetFieldValue( "Sn_Sign1", Sn_Sign1 );
			dbrec.GetFieldValue( "Sn_Sign2", Sn_Sign2 );
			dbrec.GetFieldValue( "Sn_Sign3", Sn_Sign3 );
			dbrec.GetFieldValue( "Sn_Sign4", Sn_Sign4 );

			dbrec.GetFieldValue( "Sn_SPECIAL", Sn_SPECIAL );
			dbrec.GetFieldValue( "Sn_TUMIAG", Sn_TUMIAG );
//--> '15.11.16 INS START
			dbrec.GetFieldValue( "Sn_ADRD8", strSn_ADRD8 );
//<-- '15.11.16 INS END

			dbrec.GetFieldValue("SVsasw",SVsasw);
			dbrec.GetFieldValue("SVmzsw",SVmzsw);
			dbrec.GetFieldValue("SVhksw",SVhksw);
			dbrec.GetFieldValue("SVd5sw",SVd5sw);
			dbrec.GetFieldValue("SVkojn",SVkojn);
			dbrec.GetFieldValue("SVzchg",SVzchg);
			dbrec.GetFieldValue("SVkazei",SVkazei);
			dbrec.GetFieldValue("SVsacq",SVsacq);
			dbrec.GetFieldValue("SVzisq",SVzisq);
			dbrec.GetFieldValue("SVsign",SVsign);
			dbrec.GetFieldValue("SVzuri", SVzuri );

			dbrec.GetFieldValue( "ind_type", ind_type );
			dbrec.GetFieldValue( "S_SGN2", S_SGN2 );
			dbrec.GetFieldValue( "S_SGN4", S_SGN4 );
			dbrec.GetFieldValue( "updateday", updateday );

			CSnset snset;
			snset.GetSetTyukanKikan() = TimePair( CTimeControl::GetCTime(Sn_MDAYS),CTimeControl::GetCTime(Sn_MDAYE) );
			snset.GetSetKessanKikan() = TimePair( CTimeControl::GetCTime(Sn_KDAYS),CTimeControl::GetCTime(Sn_KDAYE) );

			snset.GetSn_SKKBN()  = atoi(Sn_SKKBN);	// 申告区分
			snset.Sn_KZTNSK = atoi(Sn_KZTNSK);		// 課税期間短縮
			snset.Sn_MCOUNT = atoi(Sn_MCOUNT);		// 中間申告　年間回数
			snset.Sn_JCOUNT = atoi(Sn_JCOUNT);		// 中間申告　現在申告状態
			snset.Sn_ZeiHou = atoi(Sn_ZeiHou);		// 特殊処理サイン
			snset.Sn_Syuusei= atoi(Sn_Syuusei);		// 〃
			snset.Sn_Sign1  = atoi(Sn_Sign1);		// 〃
			snset.Sn_Sign2  = atoi(Sn_Sign2);		// 〃
			snset.Sn_Sign3  = atoi(Sn_Sign3);		// 〃
			snset.Sn_Sign4  = atoi(Sn_Sign4);		// 〃
			snset.Sn_SPECIAL= atoi(Sn_SPECIAL);		// 特殊サイン
			snset.Sn_TUMIAG = atoi(Sn_TUMIAG);		// 規則２２条１項の適用＆法第３０条第１項の適用（積み上げ方式）
//--> '15.11.16 INS START
			SepStrToSign( &snset.m_DispTabSgn, strSn_ADRD8 );
//<-- '15.11.16 INS END

			snset.SVsasw	= atoi(SVsasw);
			snset.SVmzsw	= atoi(SVmzsw);
			snset.SVhksw	= atoi(SVhksw);
			snset.SVd5sw	= atoi(SVd5sw);
			snset.SVkojn	= atoi(SVkojn);
			snset.SVzchg	= atoi(SVzchg);
			snset.SVkazei	= atoi(SVkazei);
			snset.SVsacq	= atoi(SVsacq);
			snset.SVzisq	= atoi(SVzisq);
			snset.SVsign	= atoi(SVsign);
			m_Arith.l_input(snset.SVzuri,(LPSTR)(LPCSTR)SVzuri );
	
			snset.ind_type = atoi(ind_type);
			snset.S_SGN2 = atoi(S_SGN2);
			snset.S_SGN4 = atoi(S_SGN4);

			snset.SetMcount();
			HistoryVec.push_back( HistoryData( snset, CTimeControl::GetCTime(atoi(updateday)) ) );
			dbrec.MoveNext();

		}
	}catch(CDBException* dbe){
//		dbrec.Close();
		dbe->Delete();
	}
	dbrec.Close();
	
	m_PastReport.SetUnion(CHistoryReport::kubun,CHistoryReport::saisyu,"履歴保存");
	m_PastReport.SetHeaderHeight(m_PastReport.GetHeaderHeight() * 2 );
	m_PastReport.AddRecordArray(HistoryVec);

	if( !IsExistsHistory() ){
		HistoryModeOff();
	}
	else{
		SetCtrl();
	}

	SetTansyukuKaishi();
}

//-----------------------------------------------------------------------------
// 文字列データをサインに分解（'15.11.16）
//-----------------------------------------------------------------------------
// 引数	pDispTabSgn	：	分解先データ
//		srcStr		：	分解元文字列
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SepStrToSign( long *pDispTabSgn, CString &srcStr )
{
	const	int	EVERY_OPTION_STRLEN	=	8;
	// AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDD(16進数データを文字列化して保存)

	CString	dStr;
	int		max = srcStr.GetLength() / EVERY_OPTION_STRLEN;

	for( int i=0; i<max; i++ ){
		dStr = srcStr.Mid( i*EVERY_OPTION_STRLEN, EVERY_OPTION_STRLEN );
		if( i==0 ){
			// 税理士区分
			;
		}
		else if( i==1 ){
			// 税理士出力サイン
			;
		}
		else if( i==2 ){
			m_Util.AscToBcd( (char*)pDispTabSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN );
		}
	}

	return 0;
}

TimePairVector CDBSyzShinMainView::GetShinMininfovec()
{
	TimePairVector minfovec;

	CString sq;
	sq.Format("select smd,emd from shinminfo order by seq");
	CRecordset rec( m_pZmSub->m_database );
	try {
		rec.Open( CRecordset::forwardOnly, sq );
		while ( rec.IsEOF() == FALSE ) {
			CDBVariant smd,emd;
			rec.GetFieldValue((SHORT)0, smd, SQL_C_SLONG);
			rec.GetFieldValue((SHORT)1, emd, SQL_C_SLONG);
			minfovec.push_back( TimePair( CTimeControl::GetCTime((int)smd.m_lVal), CTimeControl::GetCTime((int)emd.m_lVal)) );
			rec.MoveNext();
		}
	}
	catch ( CDBException* dbe ) {
//		rec.Close();
		dbe->Delete();
	}
	rec.Close();
	return minfovec;
}

int CDBSyzShinMainView::CheckChange()
{
	if( pastset.GetKubun() != curset.GetKubun() ) return 1;

	if( curset.IsTyukanShinkoku() ) {
		TimePair t1 = pastset.GetSetTyukanKikan();
		TimePair t2 = GetCurrentTyukanKikan();
		if(t1 != t2) return 2;
	}

	if( curset.IsKakuteiShinkoku() ) {
		TimePair t1 = pastset.GetSetKessanKikan();
		TimePair t2 = GetCurrentKessanKikan();
		if(t1 != t2) return 3;
	}

	return 0;
}

#ifdef _20150312_EXTEND_
CString CDBSyzShinMainView::GetNextShinkokuMessage()
{
	int chnge = CheckChange();

	CString mes;
	if( chnge == 1 ){
		CString str;
		str += "申告区分が変更されました。\n";
		str += "\n";
		str += "　　【現在申告】%s\n";
		str += "　　　　 ↓\n";
		str += "　　【次回申告】%s\n";
		mes.Format(str,pastset.GetKubunStr(),curset.GetKubunStr());
	}

	if( chnge == 2 ){
		CString str;
		str += "中間申告期間が変更されました。\n";
		str += "\n";
		str += "　　【現在申告】%s：%s\n";
		str += "　　　　 ↓\n";
		str += "　　【次回申告】%s：%s\n";
		TimePair t1 = pastset.GetSetTyukanKikan();
		TimePair t2 = GetCurrentTyukanKikan();
		mes.Format(str,pastset.GetKubunStr(),CTimeControl::GetDateStrPair(t1),curset.GetKubunStr(),CTimeControl::GetDateStrPair(t2));
	}

	if( chnge == 3 ){
		CString str;
		str += "課税期間が変更されました。\n";
		str += "\n";
		str += "　　【現在申告】%s：%s\n";
		str += "　　　　 ↓\n";
		str += "　　【次回申告】%s：%s\n";
		TimePair t1 = pastset.GetSetKessanKikan();
		TimePair t2 = GetCurrentKessanKikan();
		mes.Format(str,pastset.GetKubunStr(),CTimeControl::GetDateStrPair(t1),curset.GetKubunStr(),CTimeControl::GetDateStrPair(t2));
	}

	if( mes.GetLength() ) {
		if(!IsExistsHistory()){
			mes += "\n現在申告分を保存し、次の申告に進みますか？";
			mes += "\n現在申告分を変更する場合は、「申告区分の変更」を選択してください。";
		}else{
			if( ( ( pastset.GetKubun() == tyuukan && curset.GetKubun() == yotei  ) || ( pastset.GetKubun() == yotei   && curset.GetKubun() == tyuukan) ) && pastset.GetSetTyukanKikan() ==  GetCurrentTyukanKikan() )
				mes += "\n申告区分を変更しますか？";
			else
				mes += "\n現在申告分を保存し、次の申告に進みますか？";
		}
		return mes;
	}

	return CString();
}
#else
CString CDBSyzShinMainView::GetNextShinkokuMessage()
{
	int chnge = CheckChange();

//2018.03.13 INSERT START
	if( chnge == 3 ){
		//マイナンバー非対応に変更された場合、準確定は解除
		TimePair kikan = GetCurrentKessanKikan();
		//int seireki = CTimeControl::GetSeirekiYMD( kikan.first );
		int wareki = CTimeControl::GetWarekiYMD( kikan.first );
		//if( GetShRevTypeFromSnset( curset ) < ID_ICSSH_MYNUMBER_REVTYPE ){
		//if( wareki < 280101 ){
		//---->yoshida200520
		//令和の場合、準確定解除されてしまう。元号をチェックし、平成のみチェックするようにした（平成以前は未考慮）
		if ((wareki / 1000000) == 4) {//平成のみ
			if ((wareki % 1000000) < 280101) {
				curset.Sn_Syuusei &= ~0x10;
			}
		}
		//-----変更前
//		if( (wareki%1000000) < 280101 ){
//			curset.Sn_Syuusei &= ~0x10;
//		}
		//<---------
	}
//2018.03.13 INSERT END

	CString mes;
	if( chnge == 1 ){
		CString str;
		str += "申告区分を切り替えます。現在申告データを保存しますか？\n";
		str += "\n";
		str += "　　【現在申告】%s\n";
		str += "　　　　 ↓\n";
		str += "　　【次回申告】%s\n";
		mes.Format(str,pastset.GetKubunStr(),curset.GetKubunStr());

		mes += _T("\n　◎「保存する」\n　　現在の申告データを履歴保存して、次の申告（申告区分）に進みます。\n");
		mes += _T("\n　◎「保存しない」\n　　現在の申告データを履歴保存せずに、次の申告（申告区分）に進みます。\n");
	}

	if( chnge == 2 ){
		CString str;
		str += "中間申告期間を切り替えます。現在申告データを保存しますか？\n";
		str += "\n";
		str += "　　【現在申告】%s：%s\n";
		str += "　　　　 ↓\n";
		str += "　　【次回申告】%s：%s\n";
		TimePair t1 = pastset.GetSetTyukanKikan();
		TimePair t2 = GetCurrentTyukanKikan();
		mes.Format(str,pastset.GetKubunStr(),CTimeControl::GetDateStrPair(t1),curset.GetKubunStr(),CTimeControl::GetDateStrPair(t2));

		mes += _T("\n　◎「保存する」\n　　現在の申告データを履歴保存して、次の申告（中間申告期間）に進みます。\n");
		mes += _T("\n　◎「保存しない」\n　　現在の申告データを履歴保存せずに、次の申告（中間申告期間）に進みます。\n");
	}

	if( chnge == 3 ){
		CString str;
		str += "課税期間を切り替えます。現在申告データを保存しますか？\n";
		str += "\n";
		str += "　　【現在申告】%s：%s\n";
		str += "　　　　 ↓\n";
		str += "　　【次回申告】%s：%s\n";
		TimePair t1 = pastset.GetSetKessanKikan();
		TimePair t2 = GetCurrentKessanKikan();
		mes.Format(str,pastset.GetKubunStr(),CTimeControl::GetDateStrPair(t1),curset.GetKubunStr(),CTimeControl::GetDateStrPair(t2));

		mes += _T("\n　◎「保存する」\n　　現在の申告データを履歴保存して、次の申告（課税期間）に進みます。\n");
		mes += _T("\n　◎「保存しない」\n　　現在の申告データを履歴保存せずに、次の申告（課税期間）に進みます。\n");
	}

	if( mes.GetLength() ) {
		return mes;
	}

	return CString();
}
#endif

int CDBSyzShinMainView::NextShinkoku( int save )
{
	unsigned char newver, nowver;
	int st = 0;

//2018.03.13 INSERT START
	if( IsJunkakutei(save) ){
		curset.Sn_Syuusei |= 0x10;
	}
//2018.03.13 INSERT END

	if( !CheckChange() ){
		curset.GetSnData( *m_pSnHeadData );

		newver = 0;
		if (CheckKaiseiVER(newver)) {
			m_pMainFrame->KikanChange(newver);
		}

		return SetKikan();
	}

	newver = 0;
	st = CheckKaiseiVER(newver);

	bool bDelH26 = false;
	if( save ){
		if( st && (GetKaiseiVer()==ID_VER_SYZ26) ){
			// versionが１のデータを、強制削除
			bDelH26 = true;
		}

		SavePastShinkokuData(st);

		if( m_pMainFrame->IsNeedReOpenZmtbl() ){
			m_pMainFrame->ReOpenZmtbl();
			m_pMainFrame->SetReOpenFlg( FALSE );
		}

		// 新規データ読込み
		if( m_pMainFrame ){
			m_pMainFrame->ReReadData();
		}
	}

	curset.GetSnData(*m_pSnHeadData);

	if (st) {
		nowver = GetKaiseiVer();
		m_pMainFrame->KikanChange(newver);
		m_pMainFrame->TakeOverData( nowver, newver );
	}

	if( MakeJikaiShinkokuData(save) ) return 1;

	// 移行完了につき、旧データの削除
	if( bDelH26 ){
		if( DeleteH26LstVal() ){
			return 1;
		}
	}

	if(	!IsExistsHistory() && curset.IsKakuteiShinkoku() && !save ){
/*- '14.02.21 -*/
//		l_clear( Snd.Sn_TYNOFZ );					//  中間納付税額
//		l_clear( Snd.Sn_TTYWAR );					//  中間納付税額		(地方消費税用)
/*-------------*/
		// 中間納付税額
		char	mony[MONY_BUF_SIZE]={0};
		CString	tag, chtag;
		if (m_ShRevType < ID_ICSSH_10PER_REVTYPE) {
			tag = _T("AAB00110");
			SetMony(tag, chtag, mony);
			tag = _T("Sn_TTYWAR");
			SetMony(tag, chtag, mony);
		}
		else {
			tag = _T("AAJ00110");
			SetMony(tag, chtag, mony);
			tag = _T("AAK00070");
			SetMony(tag, chtag, mony);
		}
/*-------------*/
	}

	return 0;
}

int CDBSyzShinMainView::GetSkkbnSel()
{
	return m_ComboSkkbn.GetCurSel();
}

int CDBSyzShinMainView::GetMonthSel()
{
	return m_ComboMonthCnt.GetCurSel();
}

void CDBSyzShinMainView::SetCurrentSkkbn()
{
	map<int,int> kubnmap = GetMapKubun();
	curset.SetKubun( kubnmap[GetSkkbnSel()] );
}

int CDBSyzShinMainView::GetMonthCnt()
{
	map<int,int> McountMap = GetMapMcount();
	return McountMap[GetMonthSel()];
}

void CDBSyzShinMainView::SetCurrentMcount()
{
	curset.GetSetMcount() = GetMonthCnt();

	curset.Sn_MCOUNT = IsTyukanAri() ? max( curset.GetSetMcount(), 1 ) : 0;
//	Snh.Sn_MCOUNT = curset.Sn_MCOUNT;

	if( IsKikanTansyuku() ) {
		switch( curset.GetSetMcount() ) {
		case 3 :
			curset.Sn_KZTNSK = 0x01;
//			Snh.Sn_KZTNSK = 0x01;
			break;	// ３ヶ月
		default:
		case 11 :
			curset.Sn_KZTNSK = 0x02;
//			Snh.Sn_KZTNSK = 0x02;
			break;	// １ヶ月
		}
	}
	else{
		curset.Sn_KZTNSK = 0;
//		Snh.Sn_KZTNSK = 0;
	}
}

map<int,int> CDBSyzShinMainView::GetMapKubun(int swap_sw)
{
	return GetMapOfsKubun( swap_sw );
}

vector<pair<int,CString>>  CDBSyzShinMainView::GetVectorSkbnName()
{
	map<int,int> kubnmap = GetMapKubun();
	map<int,CString> cbitm = GetMapSkbnName();
	vector<pair<int,CString>> SkbnNameVector;

	for(UINT i = 0;i<kubnmap.size();i++)
		SkbnNameVector.push_back(pair<int,CString>(kubnmap[i],cbitm[kubnmap[i]]));

	return SkbnNameVector;
}

int CDBSyzShinMainView::IsKityuuKakutei()
{
//2018.03.13 UPDATE START
//	return IsExistsHistory() && pastset.IsTyukanShinkoku() && curset.GetKubun() == kakutei;
	return IsExistsHistory() && pastset.IsTyukanShinkoku() && (curset.GetKubun() == kakutei||curset.GetKubun() == junkakutei);
//2018.03.13 UPDATE END
}

int CDBSyzShinMainView::IsKikanTansyuku()
{
	return m_ComboKikanTansyuku.GetCurSel();
}

int CDBSyzShinMainView::IsTyukanAri()
{
	return m_ComboTyukanAri.GetCurSel();
}

//2018.03.13 INSERT START
int CDBSyzShinMainView::IsKojin()
{
	return m_pSnHeadData->IsSoleProprietor();
}
int CDBSyzShinMainView::IsMynumber()
{
	return (m_ShRevType>=ID_ICSSH_MYNUMBER_REVTYPE);
}
int CDBSyzShinMainView::IsJunkakutei( int sgn )
{

	if( sgn ){ //履歴の追加
		return m_pSnHeadData->Sn_Syuusei&0x10;
	}

	//申告区分の変更or追加も変更もなし
	unsigned char syuusei = 0x00;
	int rec_cnt = 0;
	CStringArray syuusei_ary;

	CRecordset dbrec(m_pZmSub->m_database);
	try {
	
		CString sq;
		sq += "SELECT Sn_Syuusei\n";
		sq += "FROM szsnk ORDER BY Sn_Seq";
		
		dbrec.Open( CRecordset::forwardOnly, sq );
		while ( dbrec.IsEOF() == FALSE ){
		
			CString Sn_Syuusei;
	
			dbrec.GetFieldValue( "Sn_Syuusei", Sn_Syuusei );

			syuusei_ary.Add(Sn_Syuusei);

			dbrec.MoveNext();
		}
	}catch(CDBException* dbe){
		dbe->Delete();
	}
	dbrec.Close();

	if( syuusei_ary.GetCount() >= 2 ){
		syuusei = atoi(syuusei_ary.GetAt(1));
		return syuusei&0x10;
	}

	return 0;
}
//2018.03.13 INSERT END

int CDBSyzShinMainView::GetHistoryCurSel()
{
	pair<int,int> rc = m_PastReport.GetCurrentRowCoulmn();
	return rc.first;
}

int CDBSyzShinMainView::GetHistorySeq()
{
	return GetHistoryCurSel() + 1;
}

int CDBSyzShinMainView::ChangeLockMode(CWnd* wnd)
{
	if( m_pSnHeadData->Sn_Sign4&0x01 ){
		if( m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE ){
			CString str;
			str += "入力ロックを解除しますか？\n";
			str += "※ロック解除後は、自動集計が行われます。\n";
			str += "（非連動処理又は履歴確認時は、自動集計しません。）\n";
			if( ICSMessageBox( str, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0, 0, this ) != IDYES )	return -1;

		}
		m_pSnHeadData->Sn_Sign4 &= 0xfe;
	
	}else{

		m_pSnHeadData->Sn_Sign4 |= 0x01;
	}

	if(!IsHistoryMode()){
		curset.Sn_Sign4 = m_pSnHeadData->Sn_Sign4;
		return 0;
	}

	CDatabase* db = m_pZmSub->m_database;
	CTime CurTime = CTime::GetCurrentTime();
	CString NowDay = CurTime.Format( "%Y%m%d " );

	// ロック解除時 最終更新日の更新
	CString w;
	w.Format("where seq = %d", GetHistorySeq() );
	CString sq = "update ShinInfo set updateday = "+ NowDay + w +"  \n";

	db->ExecuteSQL(sq);
	m_PastReport.SetRecordText(GetHistoryCurSel(), CHistoryReport::saisyu,CTimeControl::GetDateStr( CurTime, "%2d年%2d月%2d日" ));
	return 0;
}

int CDBSyzShinMainView::SyokaiCheck()
{
	if( !(m_pSnHeadData->SVsasw&0x01) ) {
/*- '14.02.22 -*/
//		curset.SetSnh( *m_pSnHeadData );
/*-------------*/
		curset.GetSnData( *m_pSnHeadData );
		m_pSnHeadData->SVsasw |= 0x01;
/*-------------*/
//		SyzShin_UpDate();
		m_pMainFrame->UpdateSyzShin();
		curset.GetSetTyukanKikan() = GetCurrentTyukanKikan();
		curset.GetSetKessanKikan() = GetCurrentKessanKikan();
		pastset = curset;
		return 1;
	}
	return 0;
}

CStdioFile *    DebugFp = NULL;
static  void    CloseDebugLog(void)
{
#ifdef _DEBUG
        delete DebugFp;
        DebugFp = NULL;
#endif
}

static  void    OpenDebugLog(CString path)
{
#ifdef _DEBUG
        UINT    mode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
        DebugFp = new CStdioFile(path, mode);
#endif
}

int     WriteDebugLog(LPCSTR text)
{
#ifdef _DEBUG
        DebugFp->WriteString(text);
        DebugFp->WriteString("\n");
#endif
        return 0;
}

class PassClass
{ 
	CString pas;
	CString cur;
	CString op;
	int exists;

public:
	PassClass(CString p,CString c,CString o,int e) : pas(p),cur(c),op(o),exists(e),tugi(0),syusei(0)
	{
	}

	PassClass(char* p,char* c,char* o,int e,int t,int s) : pas(p),cur(c),op(o),exists(e),tugi(t),syusei(s)
	{
	}

	bool operator ==( const PassClass& b ){
		return pas == b.pas && cur == b.cur && op == b.op && exists == b.exists;
	}

	int tugi;
	int syusei;
};

pair<int,int> GetPass(CString pas,CString cur,CString op,int IsExistsHistory)
{
	// 通常は基本パターン (0,1) 次へ × 変更 ○ を返す 
	// それ以外の組み合わせになるものをリスト
	vector<PassClass> v;
	v.push_back( PassClass("確定","確定","<",1,1,0) );
	v.push_back( PassClass("確定","中間","<",1,1,0) );
	v.push_back( PassClass("確定","予定(26号)","<",1,1,0) );
//2016.10.20 UPDATE START
	//v.push_back( PassClass("確定","確定修正","=",1,1,0) );
	//v.push_back( PassClass("確定","確定修正",">",1,1,0) );
	//v.push_back( PassClass("確定","中間修正",">",1,1,1) );
	v.push_back( PassClass("確定","修正確定","=",1,1,0) );
	v.push_back( PassClass("確定","修正確定",">",1,1,0) );
	v.push_back( PassClass("確定","修正中間",">",1,1,1) );
//2016.10.20 UPDATE END
	v.push_back( PassClass("確定","更正の請求","=",1,1,0) );
	v.push_back( PassClass("中間","確定","<",1,1,0) );
	v.push_back( PassClass("中間","中間","<",1,1,0) );
	v.push_back( PassClass("中間","予定(26号)","<",1,1,0) );
//2016.10.20 UPDATE START
//	v.push_back( PassClass("中間","中間修正","=",1,1,0) );
	v.push_back( PassClass("中間","修正中間","=",1,1,0) );
//2016.10.20 UPDATE END
	v.push_back( PassClass("予定(26号)","確定","<",1,1,0) );
	v.push_back( PassClass("予定(26号)","中間","<",1,1,0) );
	v.push_back( PassClass("予定(26号)","予定(26号)","<",1,1,0) );
//2016.10.20 UPDATE START
	//v.push_back( PassClass("確定修正","確定","<",1,1,0) );
	//v.push_back( PassClass("確定修正","中間","<",1,1,0) );
	//v.push_back( PassClass("確定修正","予定(26号)","<",1,1,0) );
	//v.push_back( PassClass("確定修正","確定修正","<",1,1,0) );
	//v.push_back( PassClass("確定修正","中間修正",">",1,1,0) );
	//v.push_back( PassClass("確定修正","更正の請求","<",1,1,0) );
	//v.push_back( PassClass("確定修正","更正の請求","=",1,1,0) );
	//v.push_back( PassClass("中間修正","確定","<",1,1,0) );
	//v.push_back( PassClass("中間修正","中間","<",1,1,0) );
	//v.push_back( PassClass("中間修正","予定(26号)","<",1,1,0) );
	v.push_back( PassClass("修正確定","確定","<",1,1,0) );
	v.push_back( PassClass("修正確定","中間","<",1,1,0) );
	v.push_back( PassClass("修正確定","予定(26号)","<",1,1,0) );
	v.push_back( PassClass("修正確定","修正確定","<",1,1,0) );
	v.push_back( PassClass("修正確定","修正中間",">",1,1,0) );
	v.push_back( PassClass("修正確定","更正の請求","<",1,1,0) );
	v.push_back( PassClass("修正確定","更正の請求","=",1,1,0) );
	v.push_back( PassClass("修正中間","確定","<",1,1,0) );
	v.push_back( PassClass("修正中間","中間","<",1,1,0) );
	v.push_back( PassClass("修正中間","予定(26号)","<",1,1,0) );
//2016.10.20 UPDATE END
	v.push_back( PassClass("更正の請求","確定","<",1,1,0) );
	v.push_back( PassClass("更正の請求","更正の請求","<",1,1,0) );
	v.push_back( PassClass("確定","確定","<",0,1,1) );
	v.push_back( PassClass("確定","中間","<",0,1,1) );
	v.push_back( PassClass("確定","予定(26号)","<",0,1,1) );
//2016.10.20 UPDATE START
	//v.push_back( PassClass("確定","確定修正","<",0,1,1) );
	//v.push_back( PassClass("確定","確定修正","=",0,1,1) );
	v.push_back( PassClass("確定","修正確定","<",0,1,1) );
	v.push_back( PassClass("確定","修正確定","=",0,1,1) );
//2016.10.20 UPDATE END
	v.push_back( PassClass("確定","更正の請求","<",0,1,1) );
	v.push_back( PassClass("確定","更正の請求","=",0,1,1) );
	v.push_back( PassClass("確定","更正の請求",">",0,1,1) );
	v.push_back( PassClass("中間","確定","<",0,1,1) );
	v.push_back( PassClass("中間","中間","<",0,1,1) );
	v.push_back( PassClass("中間","予定(26号)","<",0,1,1) );
//2016.10.20 UPDATE START
	//v.push_back( PassClass("中間","中間修正","=",0,1,1) );
	//v.push_back( PassClass("中間","中間修正",">",0,1,1) );
	v.push_back( PassClass("中間","修正中間","=",0,1,1) );
	v.push_back( PassClass("中間","修正中間",">",0,1,1) );
//2016.10.20 UPDATE END
	v.push_back( PassClass("予定(26号)","確定","<",0,1,1) );
	v.push_back( PassClass("予定(26号)","中間","<",0,1,1) );
	v.push_back( PassClass("予定(26号)","予定(26号)","<",0,1,1) );
//2016.10.20 UPDATE START
	//v.push_back( PassClass("確定修正","確定","<",0,1,1) );
	//v.push_back( PassClass("確定修正","確定修正","<",0,1,1) );
	//v.push_back( PassClass("確定修正","確定修正","=",0,1,1) );
	//v.push_back( PassClass("確定修正","更正の請求","<",0,1,1) );
	//v.push_back( PassClass("確定修正","更正の請求","=",0,1,1) );
	//v.push_back( PassClass("中間修正","確定","<",0,1,1) );
	//v.push_back( PassClass("中間修正","中間","<",0,1,1) );
	//v.push_back( PassClass("中間修正","予定(26号)","<",0,1,1) );
	//v.push_back( PassClass("中間修正","確定修正","<",0,1,1) );
	//v.push_back( PassClass("中間修正","確定修正","=",0,1,1) );
	//v.push_back( PassClass("中間修正","中間修正","<",0,1,1) );
	//v.push_back( PassClass("中間修正","更正の請求","<",0,1,1) );
	//v.push_back( PassClass("中間修正","更正の請求","=",0,1,1) );
	v.push_back( PassClass("修正確定","確定","<",0,1,1) );
	v.push_back( PassClass("修正確定","修正確定","<",0,1,1) );
	v.push_back( PassClass("修正確定","修正確定","=",0,1,1) );
	v.push_back( PassClass("修正確定","更正の請求","<",0,1,1) );
	v.push_back( PassClass("修正確定","更正の請求","=",0,1,1) );
	v.push_back( PassClass("修正中間","確定","<",0,1,1) );
	v.push_back( PassClass("修正中間","中間","<",0,1,1) );
	v.push_back( PassClass("修正中間","予定(26号)","<",0,1,1) );
	v.push_back( PassClass("修正中間","修正確定","<",0,1,1) );
	v.push_back( PassClass("修正中間","修正確定","=",0,1,1) );
	v.push_back( PassClass("修正中間","修正中間","<",0,1,1) );
	v.push_back( PassClass("修正中間","更正の請求","<",0,1,1) );
	v.push_back( PassClass("修正中間","更正の請求","=",0,1,1) );
//2016.10.20 UPDATE END
	v.push_back( PassClass("更正の請求","更正の請求","<",0,1,1) );
//2018.03.13 INSERT START
	v.push_back( PassClass("準確定","準確定","<",1,1,0) );
	v.push_back( PassClass("準確定","中間","<",1,1,0) );
	v.push_back( PassClass("準確定","予定(26号)","<",1,1,0) );
	v.push_back( PassClass("準確定","修正確定","=",1,1,0) );
	v.push_back( PassClass("準確定","修正確定",">",1,1,0) );
	v.push_back( PassClass("準確定","修正中間",">",1,1,1) );
	v.push_back( PassClass("準確定","更正の請求","=",1,1,0) );
	v.push_back( PassClass("中間","準確定","<",1,1,0) );
	v.push_back( PassClass("予定(26号)","準確定","<",1,1,0) );
	v.push_back( PassClass("修正確定","準確定","<",1,1,0) );
	v.push_back( PassClass("修正中間","準確定","<",1,1,0) );
	v.push_back( PassClass("更正の請求","準確定","<",1,1,0) );
	v.push_back( PassClass("準確定","準確定","<",0,1,1) );
	v.push_back( PassClass("準確定","中間","<",0,1,1) );
	v.push_back( PassClass("準確定","予定(26号)","<",0,1,1) );
	v.push_back( PassClass("準確定","修正確定","<",0,1,1) );
	v.push_back( PassClass("準確定","修正確定","=",0,1,1) );
	v.push_back( PassClass("準確定","更正の請求","<",0,1,1) );
	v.push_back( PassClass("準確定","更正の請求","=",0,1,1) );
	v.push_back( PassClass("準確定","更正の請求",">",0,1,1) );
	v.push_back( PassClass("中間","準確定","<",0,1,1) );
	v.push_back( PassClass("予定(26号)","準確定","<",0,1,1) );
	v.push_back( PassClass("修正確定","準確定","<",0,1,1) );
	v.push_back( PassClass("修正中間","準確定","<",0,1,1) );
//2018.03.13 INSERT END
//2022.06.10 19-0032 INSERT START
	v.push_back( PassClass("更正の請求","修正確定","=",0,1,1) );
	v.push_back( PassClass("更正の請求","修正確定","=",1,1,1) );
//2022.06.10 19-0032 INSERT END
//2023.01.17 20-0846 INSERT START
	v.push_back( PassClass("確定","準確定","<",1,1,1) );	// 前回申告区分/次の申告区分/期間の比較？/履歴があるか/保存する/保存しない
	v.push_back( PassClass("確定","準確定","<",0,1,1) );
	v.push_back( PassClass("確定","準確定","=",1,1,1) );
	v.push_back( PassClass("確定","準確定","=",0,1,1) );
//2023.01.17 20-0846 INSERT END

	vector<PassClass>::iterator it = find(v.begin(),v.end(), PassClass( pas, cur, op, IsExistsHistory) );
	if( it == v.end() ) return pair<int,int>(0,1);

	return pair<int,int>(it->tugi,it->syusei);
}


int GetPass(int new_kubun,int old_kubun,TimePair t1, TimePair t2, int IsExistsHistory,CString& mes)
{
	CString op;
	if( t1.second < t2.second )
		op = "<";
	if( t1.second == t2.second )
		op = "=";
	if( t1.second > t2.second )
		op = ">";

	CString pas = GetMapSkbnName()[old_kubun];
	CString cur = GetMapSkbnName()[new_kubun];

	pair<int,int> next = GetPass(pas,cur,op,IsExistsHistory);
	int ret = 0;

	if( next.first )   ret |= 0x01;
	if( next.second )  ret |= 0x02;

	return ret;
}

#include "NextShinkokuDlg.h"

enum{
tugi = 1,
syuse = 2,
mukou = 4,
};

int ExtBox(CString mes,int flg)
{
	CNextShinkokuDlg dlg(mes,flg);
	return (int)dlg.DoModal();
}

int CDBSyzShinMainView::NextShinkokuCheck(CString& mes)
{
	TimePair t1,t2;

	int new_kubun = curset.GetKubun();
	int old_kubun = pastset.GetKubun();

	if( IsTyukanShinkoku( old_kubun ) )
		t1 = pastset.GetSetTyukanKikan();
	else			
		t1 = pastset.GetSetKessanKikan();

	if( IsTyukanShinkoku( new_kubun ) )
		t2 = GetCurrentTyukanKikan();
	else
		t2 = GetCurrentKessanKikan();

//--> TEST
//	CString	day = t1.second.Format( _T("%Y%m%d") );
//	AfxMessageBox( day );
//	day = t2.second.Format( _T("%Y%m%d") );
//	AfxMessageBox( day );
//<-- TEST

	int ret = GetPass( new_kubun, old_kubun, t1, t2, IsExistsHistory() ? TRUE : FALSE, mes );

	// ↓以下パターンから外れた分

	// 同一開始日かつ処理月単位が異なる
	if( ( curset.IsTyukanShinkoku() == pastset.IsTyukanShinkoku() ) && curset.GetSetMcount() != pastset.GetSetMcount() ){
		if( t1.first == t2.first)
			ret &= ~0x01;
	}

	// 期間を飛ばした
	if( curset.IsTyukanShinkoku() && (t1.first < t2.first) ){
		if( t1.second + CTimeSpan(1, 0, 0, 0) != t2.first ) 
			ret &= ~0x01;
	}

	// 中間を規定回数行わず確定（notみなし）に変更した
	if( !curset.IsMinashi() && curset.IsKakuteiShinkoku() && pastset.IsTyukanShinkoku() ){
		int mcount = pastset.GetSetMcount();
		int jcont = GetTimeToJcount( mcount, pastset.GetSetTyukanKikan() ) + 1;
//2016.06.22 UPDATE START
//		if( mcount != jcont) 

		int kitei_cnt = Tyukan_Kitei( mcount );
		if( kitei_cnt != jcont) 
//2016.06.22 UPDATE END
			ret &= ~0x01;
	}

	// 確定申告グループ間の同一期間の移行を制御（ex 確定申告と確定修正で同一期間時に変更を許可しない）
	if( curset.IsKakuteiShinkoku() ){
		for(CHistoryReport::VectorType::iterator it = HistoryVec.begin();it != HistoryVec.end();it++){
			if( ( it->GetSnset().GetKubun() == new_kubun ) && (it->GetSnset().GetSetKessanKikan() == t2 ) ){
				ret &= ~0x02;
				break;
			}
//2018.03.13 INSERT START
			// 確定→(修正確定)→準確定などの変更を許可しない
			if( it->GetSnset().GetSetKessanKikan() == t2 ){
				if((( it->GetSnset().GetKubun() == kakutei )&&( new_kubun == junkakutei ))||
					(( it->GetSnset().GetKubun() == junkakutei )&&( new_kubun == kakutei ))){
						ret &= ~0x02;
						break;
				}
			}
//2018.03.13 INSERT END
		}
	}

	// 中間修正から同一期間の中間への移行を制御 harino ('14.03.27)
	if( (new_kubun == tyuukan) && (old_kubun == tyuukan_syuuse) ) {
		if( t1.first == t2.first ) {
			if( HistoryVec.size() > 0 ) {
				CHistoryReport::RecType rec = HistoryVec[0];
				CSnset& snset = rec.GetSnset();
				TimePair t3;
				int hist_kubun = snset.GetKubun();

				if( IsTyukanShinkoku( hist_kubun ) ) {
					t3 = snset.GetSetTyukanKikan();
					if( t1.first == t3.first ) {
						ret &= ~0x02;
					}
				}
			}
		}
	}

	// ８％期間の場合は更正→修正の履歴保存は行えないようにする（19-0032/160362）
	if( (new_kubun == kakutei_syuuse) && (old_kubun == kousei) ){
		if( GetActViewVer() == ID_VER_SYZ26 ) {
			ret &= ~0x01;
		}
	}

	return ExtBox( mes, ret );
}

//2016.06.22 INSERT START
int CDBSyzShinMainView::Tyukan_Kitei( int mcount )
{
	//固定値(1,3,11)を根本から修正したかったが、影響範囲が大きいので関数で対応

	int ret = 0;
	TimePairVector minfovec = GetShinMininfovec();

	switch( mcount ){
		case 1: //6ヶ月
			if( (int)minfovec.size() >= 6 ){
				if( (int)minfovec.size()%6 ){
					ret = ((int)minfovec.size()/6);
				}else{
					ret = ((int)minfovec.size()/6) - 1;
				}
			}else{
				ret = 0;
			}

			break;
		case 3: //3ヶ月
			if( (int)minfovec.size() >= 3 ){
				if( (int)minfovec.size()%3 ){
					ret = ((int)minfovec.size()/3);
				}else{
					ret = ((int)minfovec.size()/3) - 1;
				}
			}else{
				ret = 0;
			}

			break;
		case 11: //1ヶ月
			ret = ((int)minfovec.size()/1) - 1;
			break;
		default:
			break;
	}

	return ret;
}
//2016.06.22 INSERT END

//-----------------------------------------------------------------------------
// 申告不可能期間？
//-----------------------------------------------------------------------------
// 引数		snset	：	チェック対象情報
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	申告不可期間
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsNotOutKikan( CSnset &snset )
{
	BOOL	bRt = TRUE;

	TimePair t1;
	if( snset.IsTyukanShinkoku() ){
		t1 = snset.GetSetTyukanKikan();
	}
	else if( snset.IsKakuteiShinkoku() ){
		t1 = snset.GetSetKessanKikan();
	}
	else{
		return bRt;
	}

	CString	strEmd = t1.second.Format("%Y%m%d");

	int	inEmd = atoi(strEmd);

	if( ICS_SH_8PER_REFORM_DAY > inEmd ){
		bRt = FALSE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 申告不可能期間？('15.02.23)
//-----------------------------------------------------------------------------
// 引数		snset	：	チェック対象情報
//			msg		：	表示メッセージ
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	申告不可期間
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsImpossibleKikan( CSnset &snset, CString &msg )
{


	//---->yoshida190716
	// 消費税不正仕訳チェック
	//決算期間をまずチェック。期末が2019/10/01を超えているか
	//超えていた場合には課税期間の至日が2019/10/01以前かをチェックする(過去はしていないのでいる？)
	//上記２つの条件に合致する際にcheckwrongdata処理にて不正仕訳のチェックを行う

	//---->'19.08.28
	//課税期間内でチェックするよう変更
	//かつ１０％改正以降の課税期間であれば10月1日以前で１０％とかあっても申告書をたちあげれるようにする。以前の申告書がそうだった（修正依頼で依頼有り）
	TimePair t1;

	if (snset.IsTyukanShinkoku()) {
		t1 = snset.GetSetTyukanKikan();
	}
	else if (snset.IsKakuteiShinkoku()) {
		t1 = snset.GetSetKessanKikan();
	}


	CString	strSmd = t1.first.Format("%Y%m%d");
	CString	strEmd = t1.second.Format("%Y%m%d");

	long	inSmd = atoi(strSmd);
	long	inEmd = atoi(strEmd);

	//230816

	
	//---->yoshida190926
	//特例適用時の制御を変更
	if (m_pSnHeadData->m_DispTabSgn & 0x80) {//特例チェックONの時
		//特例適用時には、課税期間開始年月日が2019/10/01以前の時までチェックを行う必要あり。（特例に金額あがらないので）
		if (!(m_pSnHeadData->Sn_Sign4 & 0x80)) {//連動の場合のみチェック。非連動ではチェックしない
			if ((inSmd < ICS_SH_10PER_REFORM_DAY)) {
				if (curset.GetKubun() == yotei) {
					// 予定申告では仕訳チェックはしない！
					;
				}
				else {
					CZmGen8 ZmGen;
					int ver = ZmGen.GetZgValiableVersion(m_pZmSub->m_database);
					if (ver == 2) {
						CSyzMakeSub SyzMkSub;
						if (SyzMkSub.CheckWrongData(m_pZmSub, inSmd, (ICS_SH_10PER_REFORM_DAY - 1))) {
							msg = SyzMkSub.GetErrorMsg();
							return TRUE;
						}
					}
				}
			}


			//別記仕訳制御yoshida190927
			//令和1年10月1日～令和5年09月30日の期間内で別記仕訳ある場合には制御する
			if ((ICS_SH_10PER_REFORM_DAY <= inSmd && inSmd <= URIAGETOKUREI_EDAY) || (ICS_SH_10PER_REFORM_DAY <= inEmd && inEmd <= URIAGETOKUREI_EDAY)) {
				if (curset.GetKubun() == yotei) {
					// 予定申告では仕訳チェックはしない！
					;
				}
				else {
					CZmGen8 ZmGen;
					int ver = ZmGen.GetZgValiableVersion(m_pZmSub->m_database);
					if (ver == 2) {
						CSyzMakeSub SyzMkSub;

						long check_s = 0, check_e = 0;

						if (inSmd < ICS_SH_10PER_REFORM_DAY) {
							check_s = ICS_SH_10PER_REFORM_DAY;
						}
						else {
							check_s = inSmd;
						}

						if (inEmd > URIAGETOKUREI_EDAY) {
							check_e = URIAGETOKUREI_EDAY;
						}
						else {
							check_e = inEmd;
						}


						if (SyzMkSub.CheckSepParaData(m_pZmSub, check_s, check_e)) {
							msg = SyzMkSub.GetErrorMsg();
							return TRUE;
						}
					}
				}
			}
		}
	}
	//<-------

	//yoshida220202  ここに入れるのはインボイス本チャン出荷時、3月分では出さない
	//----->インボイスマスターかどうかチェックする
	//CZmGen8 ZmGen;
	//if (ZmGen.IsInvoiceVupMaster(m_pZmSub) == TRUE) {//インボイスバージョンアップマスターかどうか
	//	CSyzMakeSub SyzMkSub;
	//	if (SyzMkSub.CheckWrongData(m_pZmSub, inSmd, inEmd)) {//不正な仕訳が存在していないかチェック
	//		msg = SyzMkSub.GetErrorMsg();
	//		return TRUE;
	//	}
	//}


	//------>'23.04.04　申告書起動後に仕訳入力で10月以降の仕訳打たれた際に止める為の制御
	//インボイス出力出荷（9月予定分）前までは起動時チェックで止める。出力対応後はこの処理をカット予定
	//m_msgdisp = TRUE;//'23.04.06
	CSyzMakeSub SyzMkSub;
	//if (SyzMkSub.CheckWrongData(m_pZmSub)) {
	//	msg = SyzMkSub.GetErrorMsg();
	//	ICSMessageBox(msg, MB_ICONSTOP);//注意メッセージがでてたのを警告（×）メッセージに変更'23.04.06
	//	m_msgdisp = FALSE;//関数戻りのメッセージを出さない形に//'23.04.06
	//	return TRUE;
	//}
	//<--------	

	//---->'23.04.04
	m_msgdisp = TRUE;
	if (SyzMkSub.CheckWrongData(m_pZmSub, inSmd, inEmd)) {
		msg = SyzMkSub.GetErrorMsg();
		//int st = ICSMessageBox(msg, (MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1), 0, 0, this);
		int st = ICSMessageBox( msg, ( MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2 ), 0, 0, this );
		//if (st == IDYES) {
		if (st == IDNO) {
			m_msgdisp = FALSE;//関数戻りのメッセージを出さない形に
			return TRUE;
		}
	}
	//<----

	//<-------


	if ((inEmd < ICS_SH_10PER_REFORM_DAY)) {

		if (curset.GetKubun() == yotei) {
			// 予定申告では仕訳チェックはしない！
			;
		}
		else{

			//---->19.09.03
			if (!(m_pSnHeadData->Sn_Sign4 & 0x80)) {//連動の場合のみチェック。非連動ではチェックしない
				CZmGen8 ZmGen;
				int ver = ZmGen.GetZgValiableVersion(m_pZmSub->m_database);
				if (ver == 2) {//１０％改正バージョンがアップされてるマスターのみ対象とさせるyoshida190903
					CSyzMakeSub SyzMkSub;
					if (SyzMkSub.CheckWrongData(m_pZmSub, inSmd, inEmd)) {
						msg = SyzMkSub.GetErrorMsg();
						return TRUE;
					}
				}
			}
		}
	}
	else {
		if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){	// 連動時のみチェック通す
			CZmGen8 ZmGen;
			int ver = ZmGen.GetZgValiableVersion(m_pZmSub->m_database);
			if (ver == 2) {//１０％改正バージョンがアップされてるマスターのみ対象とさせる
				CSyzMakeSub SyzMkSub;
				if (SyzMkSub.Check3or5ImportData(m_pZmSub, inSmd, inEmd)) {
					msg = SyzMkSub.GetErrorMsg();
					return TRUE;
				}
			}
		}
	}


	

	//-----
	/*

	if ((m_pZmSub->zvol->ee_ymd > ICS_SH_10PER_REFORM_DAY)) {
		CSyzMakeSub SyzMkSub;
			if (SyzMkSub.CheckWrongData(m_pZmSub, m_pZmSub->zvol->ss_ymd, (ICS_SH_10PER_REFORM_DAY - 1))) {
				//ICSMessageBox(SyzMkSub.GetErrorMsg());
				msg = SyzMkSub.GetErrorMsg();
				return TRUE;
			}
	}
	*/
	//<----
	//<-----------



	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI ){

		//--->'19.08.28
		//処理をifの外に出した。先に必要となった為
		/*
		TimePair t1;
//-- '15.04.30 --
//		if( snset.IsTyukanShinkoku() ){
//			t1 = snset.GetSetTyukanKikan();
//		}
//		else if( snset.IsKakuteiShinkoku() ){
//			t1 = snset.GetSetKessanKikan();
//		}
//		else{
//			return FALSE;
//		}
//---------------
		t1 = snset.GetSetKessanKikan();
//---------------

		CString	strSmd = t1.first.Format("%Y%m%d");
		CString	strEmd = t1.second.Format("%Y%m%d");

		long	inSmd = atoi(strSmd);
		long	inEmd = atoi(strEmd);
		*/
		//<------

//--> '15.03.12 INS START
//		// 申告書を対応した時には外す！
//		if( (curset.GetKubun()!=yotei) && (inSmd>=ICS_SH_SIXKIND_REFORM_DAY) ){
//			msg = _T("平成27年4月1日以後開始課税期間分（簡易課税用）の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。");
//			return TRUE;
//		}
//<-- '15.03.12 INS END


		if( m_pZmSub->zvol->ss_ymd < ICS_SH_SIXKIND_REFORM_DAY ){
			CSyzMakeSub SyzMkSub;
			int st = SyzMkSub.CheckWrongKaniData( m_pZmSub, m_pZmSub->zvol->ss_ymd, (ICS_SH_SIXKIND_REFORM_DAY-1) );
			if( (st==-1) || (st==1) ){
				msg = _T("平成27年3月31日以前に、第六種事業の仕訳が入力されています。\n仕訳の見直しを行ってください。");
				return TRUE;
			}
		}

		if( curset.GetKubun() == yotei ){
			// 予定申告では仕訳チェックはしない！
			;
		}
		else if( inSmd < ICS_SH_SIXKIND_REFORM_DAY ){
			CSyzMakeSub SyzMkSub;
			int st = SyzMkSub.CheckWrongKaniData( m_pZmSub, inSmd, inEmd );
			if( (st==-1) || (st==1) ){
				msg = _T("平成27年3月31日以前開始の課税期間では、第六種事業の金額は集計されません。\n仕訳の見直しを行ってください。");
				return TRUE;
			}
		}
	}

//-- '15.04.04 --
//	if( m_pMainFrame && (m_pMainFrame->GetsgSpc()==1) ){
//---------------
	if( m_pMainFrame && (*(m_pMainFrame->GetsgSpc())==1) ){
//---------------
		if( m_pZmSub->zvol->ss_ymd < ICS_SH_8PER_REFORM_DAY ){
			CSyzMakeSub SyzMkSub;
			int st = SyzMkSub.CheckWrongSpcData( m_pZmSub, m_pZmSub->zvol->ss_ymd, (ICS_SH_8PER_REFORM_DAY-1) );
			if( (st==-1) || (st==1) ){
				msg = _T("平成26年3月31日以前に、特定収入(8%)・不特定収入(8%)の仕訳が入力されています。\n仕訳の見直しを行ってください。");
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CDBSyzShinMainView::SyzEdit()
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("■■■SyzEdit_START\n");
	DbgViewTrace(hoge);
#endif

// 改良依頼No.20-0560 add -->
	char	zenarith[MONY_BUF_SIZE] = {0};
	char	wkbf[MONY_BUF_SIZE]     = {0};

	CString tag, chtag;
	int actVer = 0;
// 改良依頼No.20-0560 add <--

	CString ErMs;

	// 入力年月　開始・最終設定
	if( m_pMainFrame ){
		m_pMainFrame->GetMofs();
	}

	//---->非連動時、Sn_EXP100のサイン落とす6
	if( m_pSnHeadData ){
		if( m_pSnHeadData->Sn_Sign4&0x80 ){
			if( m_pMainFrame && (m_pMainFrame->GetSnSeq()==0) ){
				m_pSnHeadData->Sn_EXP100 &= ~0x01;
			}
		}
	}
	//<------------------------------------------

	


	// マイナンバー履歴
	CBaseHyoView *pBaseView = m_pMainFrame->GetHonHyoView();
	if( pBaseView == NULL ){
		return 0;
	}
	pBaseView->SetMyNumBrowseSgn(m_MyNumBrowse);

	if (!IsHistoryMode()) {
		//---->230822インボイス
		int	inSmd = 0;
		int inEmd = 0;
		GetKazeiKikanfromSnset(curset, inSmd, inEmd);

	//	if (IsInvoiceKikan(curset)) {
		if (inEmd >= ICS_SH_INVOICE_DAY) {
			//未対応制御
			//0912Del
			//if ((m_pSnHeadData->m_apno & 0xf0) > 0x30 || (m_pSnHeadData->IsSoleProprietor())) {
// 24/02/05_特定収入対応 del -->
			//if ((m_pSnHeadData->m_apno & 0xf0) > 0x30) {	// 11月リリース時点では特定収入計算表は未対応のため止める
			//	CString	guideMsg;
			//	guideMsg = _T("令和５年10月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。");
			//	ICSMessageBox(guideMsg, 0, 0, 0, this);
			//	return 0;
			//}
// 24/02/05_特定収入対応 del <--

			//積み上げ割り戻し混合判定
			CDBSyzAccess SyzAcc;
			bool uriage = 0;
			bool siire = 0;
			SyzAcc.CheckSyzZeiCalqSireKonzai(m_pZmSub, inSmd, inEmd, &uriage, &siire);
			if (siire) {
				CString	guideMsg;
				guideMsg = _T("課税期間内の税額計算（仕入）について「積上げ計算」と「割戻し計算」が混在しています。\n併用は認められていないため、新規会社登録・修正・削除の消費税情報にて、\n税額計算（仕入）の設定を「積上げ計算」または「割戻し計算」にしてください！");
				ICSMessageBox(guideMsg, 0, 0, 0, this);
				return 0;
			}
		}

		//<----

		if( !IsEnableNextShinkoku() ) {
			CString str;
			str += "選択された申告区分・期間には変更できません。\n";
			str += "申告区分・期間をご確認ください。\n";
			ICSMessageBox(str);		
			return 0;
		}
		
		int ret = IDINVALID_ICS;

#ifdef _DBG_VIEW_START_
		hoge.Format("■■■SyzEdit_IsHistoryMode_001\n");
		DbgViewTrace(hoge);
#endif

		actVer = GetActViewVer();
		if(actVer == ID_VER_SYZ31) {
			if(curset.GetKubun() == yotei) {
				// 前年度の消費税額を税額情報より取得
				tag = _T("AAF00030");
				m_pMainFrame->GetMony( tag, chtag, zenarith, actVer );
				m_Arith.l_input( wkbf, _T("480000") );
				if(m_Arith.l_cmp(zenarith,wkbf) <= 0) {
					if(ICSMessageBox(_T("前年度の消費税額が48万円以下ですが、予定申告を作成しますか？"),(MB_YESNO|MB_DEFBUTTON2)) == IDNO) {
						return 0;
					}
				}
			}

			// 登録番号チェック
			CString errMsg;
			int rt = 0;
			if( (rt = CheckWrongInvoiceData( errMsg )) != 0 ) {
				if( rt == -1 ){
					ICSMessageBox( errMsg, MB_ICONEXCLAMATION, 0, 0, this );
					return 0;
				}
				else{
					if( ICSMessageBox( errMsg, ( MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2 ), 0, 0, this ) == IDNO ){
						return 0;
					}
				}
			}


			if(( curset.m_DispTabSgn & 0x2000 ) != ( m_SvDispTabSgn & 0x2000 )) {
				if(( curset.GetKubun() == kakutei_syuuse ) || ( curset.GetKubun() == kousei )) {
					CString str;
					str.Format( _T( "当初申告において、2割特例を適用して（または2割特例を適用しないで）申告を行った場合、\n"
									"更正の請求や修正申告において、2割特例を適用しないこととする（または2割特例を適用する）\n"
									"ことはできませんのでご注意ください。\n"
									"処理を継続してもよろしいですか？" ) );

					if( ICSMessageBox( str, ( MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ), 0, 0, this ) == IDNO ) {
						return 0;
					}
				}
			}
		}

		BOOL	IsFirst = FALSE;
		if( !SyokaiCheck() ){
/*// 改良依頼No.20-0560 add -->
			actVer = GetActViewVer();
			if(actVer == ID_VER_SYZ31) {
				if(curset.GetKubun() == yotei) {
					if( pastset.GetKubun() != curset.GetKubun() ) { 
						// 前年度の消費税額を税額情報より取得
						tag = _T("AAF00030");
						m_pMainFrame->GetMony( tag, chtag, zenarith, actVer );
						m_Arith.l_input( wkbf, _T("480000") );
						if(m_Arith.l_cmp(zenarith,wkbf) <= 0) {
							if(ICSMessageBox(_T("前年度の消費税額が48万円以下ですが、予定申告を作成しますか？"),(MB_YESNO|MB_DEFBUTTON2)) == IDNO) {
								return 0;
							}
						}
					}
				}
		}
// 改良依頼No.20-0560 add <--*/
//--> '15.11.12 INS START
			// 変更可能？
			if( CheckChange() ){

#ifdef _DBG_VIEW_START_
				hoge.Format("■■■SyzEdit_CheckChange_001:Kaisei_VER:%d\n", m_pSnHeadData->Kaisei_VER);
				DbgViewTrace(hoge);
#endif
				// 特定課税仕入の残高確認
				//--->yoshida190802
				CBaseHyoView *pChkView = m_pMainFrame->GetHonHyoView(GetKaiseiVer());
				if (pChkView != NULL) {
					if (pChkView->ChkTkkzAndKzwari(0, 3)) {
						return 0;
					}
				}

				////---
				//if ( m_pSnHeadData->Kaisei_VER == ID_VER_SYZ26 ){
				//	CBaseHyoView *pChkView = m_pMainFrame->GetHonHyoView(m_pSnHeadData->Kaisei_VER);
				//	if( pChkView != NULL ){
				//		CH26HyoView	*pH26View = (CH26HyoView *)pChkView;
				//		if( pH26View->ChkTkkzAndKzwari(0, 3) ){
				//			return 0;
				//		}
				//	}
				//}
				//else if( m_pSnHeadData->Kaisei_VER == ID_VER_SYZ31 ){
				//	// 10％用の実装が必要
				//}
				//<------
			}
//<-- '15.11.12 INS END

			CString mes = GetNextShinkokuMessage();
			if( mes.GetLength() ){
				ret = NextShinkokuCheck( mes );
				if( ret == IDINVALID_ICS ) return 0;
			}
		}
		else{
/*// 改良依頼No.20-0560 add -->
			if(GetActViewVer() == ID_VER_SYZ31) {
				if(curset.GetKubun() == yotei) {
					// 前年度の消費税額を取得
					m_pMainFrame->GetZenSyz(zenarith);
					m_Arith.l_input( wkbf, _T("480000") );
					if(m_Arith.l_cmp(zenarith,wkbf) <= 0) {
						if(ICSMessageBox(_T("前年度の消費税額が48万円以下ですが、予定申告を作成しますか？"),(MB_YESNO|MB_DEFBUTTON2)) == IDNO) {
							return 0;
						}
					}
				}
			}
// 改良依頼No.20-0560 add <--*/
			IsFirst = TRUE;
		}

#ifdef _DBG_VIEW_START_
		hoge.Format("■■■SyzEdit_IsHistoryMode_002\n");
		DbgViewTrace(hoge);
#endif

		// 変更あり？
		BOOL IsChanged = FALSE;
		// 手入力サインをクリアする必要あり？
		BOOL IsNeedClearMInput = FALSE;
		// 経過税率変化有り？
		BOOL IsChgKzritu = FALSE;

		//連動・非連動切替チェックyoshida190909
		BOOL IsCngRendou = FALSE;

		if( CheckChange() ){
			IsChanged = TRUE;
//2018.03.13 UPDATE START
//			if( ((pastset.GetKubun()==kakutei)&&(curset.GetKubun()==kakutei_syuuse)) || ((pastset.GetKubun()==tyuukan)&&(curset.GetKubun()==tyuukan_syuuse)) ){
			if( ((pastset.GetKubun()==kakutei||pastset.GetKubun()==junkakutei)&&(curset.GetKubun()==kakutei_syuuse)) ||
				((pastset.GetKubun()==tyuukan)&&(curset.GetKubun()==tyuukan_syuuse)) ||
//2018.03.13 UPDATE START
//2022.08.17 UPDATE START
				((pastset.GetKubun()==kakutei)&&(curset.GetKubun()==kousei)) ||
				((pastset.GetKubun()==kousei)&&(curset.GetKubun()==kakutei_syuuse)) ||
				((pastset.GetKubun()==kakutei_syuuse)&&(curset.GetKubun()==kousei)) ){
//2022.08.17 UPDATE END
				;
			}
			else{
				IsNeedClearMInput = TRUE;
			}
		}
		else{
/*- '14.05.16 -*/
//			if( curset.Sn_Sign4 != pastset.Sn_Sign4 ){
//				if( (curset.Sn_Sign4&0x80) != (pastset.Sn_Sign4&0x80) ){
//					IsNeedClearMInput = TRUE;
//				}
//				IsChanged = TRUE;
//			}
//			else if( curset.SVmzsw != pastset.SVmzsw ){
//				IsChanged = TRUE;
//				IsNeedClearMInput = TRUE;
//			}
/*-------------*/
			if( curset.SVmzsw != m_SvSVmzsw ){
				IsChanged = TRUE;
				IsNeedClearMInput = TRUE;
				IsChgKzritu = TRUE;

				//yoshida190912
				if ((curset.Sn_Sign4 & 0x80) != (m_SvSn_Sign4 & 0x80)) {
					IsCngRendou = TRUE;
				}

			}
			else if( (curset.Sn_Sign4&0x81) != (m_SvSn_Sign4&0x81) ){
				// 状態の変更は、ロック切替・非連動切替を意味する
				if( (curset.Sn_Sign4&0x80) != (m_SvSn_Sign4&0x80) ){
					IsNeedClearMInput = TRUE;
					IsCngRendou = TRUE;//yoshida190909
				}
				IsChanged = TRUE;
			}
//--> '15.08.21 INS START
			else if( (curset.m_DispTabSgn&0x01) != (m_SvDispTabSgn&0x01) ){
				IsNeedClearMInput = TRUE;
				IsChanged = TRUE;
			}

			else if ((curset.m_DispTabSgn & 0xf0) != (m_SvDispTabSgn & 0xf0)) {
				IsNeedClearMInput = TRUE;
				IsChanged = TRUE;
			}
			else if ( ( curset.m_DispTabSgn & 0x1000 ) != ( m_SvDispTabSgn & 0x1000 ) ) {
				IsNeedClearMInput = TRUE;
				IsChanged = TRUE;
			}
			//----->('23.05.01 Add)
			else if ( ( curset.m_DispTabSgn & 0x2000 ) != ( m_SvDispTabSgn & 0x2000 ) ) {
				IsNeedClearMInput = TRUE;
				IsChanged = TRUE;
			}
			//<-----
			else{
				// 改正様式
				EnumIdIcsShRevType tmpShRevType = GetShRevTypeFromSnset( curset );
				if( tmpShRevType != m_ShRevType ){
					IsNeedClearMInput = TRUE;
					IsChanged = TRUE;
				}
			}
//<-- '15.08.21 INS END
/*-------------*/

#ifdef _DBG_VIEW_START_
			hoge.Format("■■■SyzEdit_IsHistoryMode_003\n");
			DbgViewTrace(hoge);
#endif

		}

//--> '14.05.16 MOVE START
//		// 手入力サインのクリア
//		if( IsNeedClearMInput ){
//			m_pMainFrame->ClearManualInputSign();
//		}
//<-- '14.05.16 MOVE END

#ifdef _DBG_VIEW_START_
		hoge.Format("■■■SyzEdit_IsHistoryMode_004\n");
		DbgViewTrace(hoge);
#endif

		if( (IsChanged||IsFirst) && m_pMainFrame ){
			m_pMainFrame->SetCompChangeFlg( TRUE );
		}

		// このタイミングで、Kaisei_VER は切り替えられる
		if( NextShinkoku( ret == IDYES ) ) return 0;

//--> '14.05.16 MOVE START
		// 手入力サインのクリア
		if( IsNeedClearMInput ){
			m_pMainFrame->ClearManualInputSign( IsChgKzritu );
		}
//<-- '14.05.16 MOVE END

		//---->yoshida190909
		//非連動チェックつけられた際、付表２の仕入税額の部分の手入力サインをONとする。連動と非連動で計算方法が違い、金額に差異がでる為yoshida190909
		//３１の場合のみ処理を通る用作成する
		if (IsCngRendou == TRUE) {
			if ( ((curset.Sn_Sign4 & 0x80) == 0x80) && GetKaiseiVer() == ID_VER_SYZ31) {//チェックついてる場合のみ
				CH31HyoView *pH31View = (CH31HyoView*)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
				CString tagID;//今は何もセットしていない。今後用
				pH31View->WriteDBManualInputSign(tagID);
			}
		}
		//<-------



		// 不要になった帳票のデータをクリア ('19.04.16)
		if ( ( ( IsNeedClearMInput ) || ( m_pSnHeadData->SVmzsw == 0 ) ) && ( GetKaiseiVer() == ID_VER_SYZ31 ) ) {
			m_pMainFrame->ClearHyoData();
		}

		pastset = curset;
//		m_Middle = curset.GetKubun() == yotei ? 1 : 0;

		
//--> '15.08.28 INS START
		// 改正様式の再取得
		m_ShRevType = GetShRevType();
//<-- '15.08.28 INS END

#ifdef _DBG_VIEW_START_
		hoge.Format("■■■SyzEdit_IsHistoryMode_005\n");
		DbgViewTrace(hoge);
#endif

		if( SyzShinWrite() )
			return 0;

//		SyzShin_UpDate();
		m_pMainFrame->UpdateSyzShin();



		HistoryReportInit();


#ifdef _20190830_SYUKA_ //yoshida190820
		if (IsSpc31OutKikan(curset)) {
			CString	guideMsg;
			guideMsg = _T("令和１年10月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。");
			ICSMessageBox(guideMsg, 0, 0, 0, this);
			return 0;

		}
#else

		// 本表・付表は表示する必要があるため、メッセージを表示しない
		// 特定収入計算表タブを非表示にする
		//--->yoshida190728
		// 特定収入計算表未対応。期間を見て対象期間の場合処理制御
//		if(m_Util.IsSpcMaster(m_pZmSub)){
//			if (IsSpc31OutKikan(curset)) {
//				CString	guideMsg;
//				guideMsg = _T("令和１年10月改正の特定収入計算表申告書は、現在未対応です。\n対応までしばらくお待ちください。");
//				ICSMessageBox(guideMsg, 0, 0, 0, this);
//				return 0;
//			}
//		}
		//<-------
#endif


#ifdef _20140314_SYUKA_
		if( IsNotOutKikan(curset) ){
			CString	guideMsg;
			guideMsg = _T("平成26年4月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。");
			ICSMessageBox( guideMsg, 0, 0, 0, this );
			return 0;
		}
#endif

#ifdef _KOU_TAIOU//更正の請求対応'14.07.22
		// 更正の請求の場合
		if( ChkCallProgramType() == 1 ){//26年度版のときだけチェック。過年度呼び出しの場合にはチェックしない

			if(curset.GetKubun() == kousei){
				 CString str;
				str += "平成26年4月改正の更正の請求書は、現在未対応です。\n";
				str += "対応までしばらくお待ちください。\n";
				ICSMessageBox( str, MB_OK, 0, 0, this );
				return 0;
			}
		}
#endif

//--> '15.02.23 INS START
		// 申告不正マスターチェック
		CString	msg;
		if( IsImpossibleKikan(curset, msg) ){
			if (m_msgdisp == TRUE) {	//'23.04.04
				ICSMessageBox(msg, 0, 0, 0, this);
			}
			return 0;
		}
//<-- '15.02.23 INS END

		if( SyzShinCalq() ) return 0;


		// 中間申告の場合はここで2割特例のサインを落とす
		if( curset.IsTyukanShinkoku() ){
			if ( !InLockInput() ) {
				curset.m_DispTabSgn &= ~0x2000;
				m_pSnHeadData->m_DispTabSgn &= ~0x2000;
			}
		}
	}

	else{
	
		CHistoryReport::RecType rec = HistoryVec[GetHistoryCurSel()];

		//---->230822インボイス
		//積み上げ割り戻し混合判定
		int	inSmd = 0;
		int inEmd = 0;
		GetKazeiKikanfromSnset(rec.GetSnset(), inSmd, inEmd);

		//if (IsInvoiceKikan(rec.GetSnset())) {
		if (inEmd >= ICS_SH_INVOICE_DAY) {
			//int	inSmd = 0;
			//int inEmd = 0;
			//GetKazeiKikanfromSnset(rec.GetSnset(), inSmd, inEmd);

			CDBSyzAccess SyzAcc;
			bool uriage = 0;
			bool siire = 0;
			SyzAcc.CheckSyzZeiCalqSireKonzai(m_pZmSub, inSmd, inEmd, &uriage, &siire);
			if (siire) {
				CString	guideMsg;
				guideMsg = _T("課税期間内の税額計算（仕入）について「積上げ計算」と「割戻し計算」が混在しています。\n併用は認められていないため、新規会社登録・修正・削除の消費税情報にて、\n税額計算（仕入）の設定を「積上げ計算」または「割戻し計算」にしてください！");
				ICSMessageBox(guideMsg, 0, 0, 0, this);
				return 0;
			}
		}
		//<----


		if( rec.GetSnset().ind_type != m_pZmSub->zvol->ind_type ){
		    CString str;
			str += "履歴保存時と事業区分が異なるため、当履歴は選択できません。\n";
			str += "履歴の参照が必要な場合はマスターコピー後、前回と同じ事業区分に変更して処理を行ってください。\n";
			ICSMessageBox( str, MB_OK, 0, 0, this );
			return 0;
		}
	
//		if( rec.GetSnset().S_SGN2 != Vol1.S_SGN2 ){
		if( rec.GetSnset().S_SGN2 != m_pZmSub->zvol->s_sgn2 ){
		    CString str;
			str += "履歴保存時と課税方式が異なるため、当履歴は選択できません。\n";
			str += "履歴の参照が必要な場合はマスターコピー後、前回と同じ課税方式に変更して処理を行ってください。\n";
			ICSMessageBox( str, MB_OK, 0, 0, this );
			return 0;
		}

#ifdef _20190830_SYUKA_ //yoshida190820
		if (IsSpc31OutKikan(rec.GetSnset())) {
			CString	guideMsg;
			guideMsg = _T("令和１年10月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。");
			ICSMessageBox(guideMsg, 0, 0, 0, this);
			return 0;

		}
#else

		// 本表・付表は表示する必要があるため、メッセージを表示しない
		// 特定収入計算表タブを非表示にする
		//--->yoshida190728
		// 特定収入計算表未対応。期間を見て対象期間の場合処理制御
//		if (m_Util.IsSpcMaster(m_pZmSub)) {
//			if(IsSpc31OutKikan(rec.GetSnset())){
//				CString	guideMsg;
//				guideMsg = _T("令和１年10月改正の特定収入計算表申告書は、現在未対応です。\n対応までしばらくお待ちください。");
//				ICSMessageBox(guideMsg, 0, 0, 0, this);
//				return 0;
//			}
//		}
		//<------
#endif

#ifdef _20140314_SYUKA_
		if( IsNotOutKikan(rec.GetSnset()) ){
			CString	guideMsg;
			guideMsg = _T("平成26年4月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。");
			ICSMessageBox( guideMsg, 0, 0, 0, this );
			return 0;
		}
#endif

#ifdef _KOU_TAIOU//更正の請求対応'14.07.22
		// 更正の請求の場合
		if( ChkCallProgramType() == 1 ){//26年度版のときだけチェック。過年度呼び出しの場合にはチェックしない
			if(rec.GetSnset().GetKubun() == kousei){
				 CString str;
				str += "平成26年4月改正の更正の請求書は、現在未対応です。\n";
				str += "対応までしばらくお待ちください。\n";
				ICSMessageBox( str, MB_OK, 0, 0, this );
				return 0;
			}
		}
#endif

//--> '15.02.23 INS START
		// 申告不正マスターチェック
		CString	msg;
		if( IsImpossibleKikan(rec.GetSnset(), msg) ){
			if (m_msgdisp == TRUE) {	//'23.04.04
				ICSMessageBox(msg, 0, 0, 0, this);
			}
			return 0;
		}
//<-- '15.02.23 INS END

		CString str;
		str.Format("履歴保存されている「%s : %s」を呼び出します。\nよろしいですか？",rec.str_kubun,rec.str_kikan);
		if( ICSMessageBox( str, MB_YESNO|MB_ICONQUESTION, 0, 0, this ) != IDYES )
			return 0;

		unsigned char newver = 0;
		if (CheckKaiseiVER(newver)) {
			m_pMainFrame->KikanChange(newver);
		}

		// 他表履歴切替
		CString	ErMs;
		if( m_pMainFrame ){
			m_pMainFrame->OpenSyzShin( GetHistorySeq(), ErMs );
			m_pMainFrame->ChangeViewSnSeq( GetHistorySeq() );
		}

/*- TEST -*/
//		CString	hoge;
//		hoge.Format( _T("seq=%d"), GetHistorySeq() );
//		AfxMessageBox( hoge );
/*---------*/

//		if( rec.GetSnset().S_SGN4 & 0x80 )	Anbun = 1;	// 個別対応
//		else								Anbun = 0;	// 比例配分
		if( rec.GetSnset().S_SGN4 & 0x80 )	m_Anbun = 1;	// 個別対応
		else								m_Anbun = 0;	// 比例配分
	
//		Vol1.S_SGN2 = rec.GetSnset().S_SGN2;
//		Vol1.S_SGN4 = rec.GetSnset().S_SGN4;
		m_pSnHeadData->m_s_sgn2 = rec.GetSnset().S_SGN2;
		m_pSnHeadData->m_s_sgn4 = rec.GetSnset().S_SGN4;

		// 履歴呼出時には強制切替
		m_pMainFrame->SetCompChangeFlg( TRUE );

//2015.09.16 INSERT START
		m_ShRevType = GetShRevType();
//2015.09.16 INSERT END
	}
	
	//★９５％ルール改正対応期間チェック [13'11.20]
//	if( (Vol1.S_SGN2 & 0x0f) < 2 )	TaxReformsInitCheck();
	if( (m_pSnHeadData->m_s_sgn2 & 0x0f) < 2 ){
		m_pMainFrame->Chk_H24Reform_InInit();
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("■■■SyzEdit_END\n");
	DbgViewTrace(hoge);
#endif

	return 1;
}

// for remove_if functor
class pred
{
public:
	enum op{
		not,	// !=
		equal, 	// ==
		over,	// >
		andover,// >= 
		less,	// <
		orless,	// <=
	};

private:
	CTime m_time;
	op m_op;
	int flg;
public:

	pred(CTime t,op o,int f) : m_time(t), m_op(o), flg(f)
	{
	}

	bool operator()(TimePair ti) const 
	{ 
		CTime t = flg ? ti.second : ti.first;
		switch(m_op){
		case not:
			return t != m_time;
		case equal:
			return t == m_time;
		case over:
			return t >  m_time;
		case andover: 
			return t >= m_time;
		case less:
			return t <  m_time;
		case orless:
			return t <= m_time;
		};
		return false; 
	}
};

CSnset* CDBSyzShinMainView::GetRirekiSnset(int (CSnset::*pFunc)())
{
	CSnset* snset = 0;

	if( (pastset.*pFunc)() )
		snset = &pastset;
	else{
		for(CHistoryReport::VectorType::iterator it = HistoryVec.begin();it != HistoryVec.end();it++){
			if( (it->GetSnset().*pFunc)() )
				return &it->GetSnset();
		}
	}
	return snset;
}

TimePairVector CDBSyzShinMainView::GetTimeVector(int mcount)
{
	TimePairVector minfovec = GetShinMininfovec();

	int span = 12 / ( mcount + 1 );
	TimePairVector vec;

	for(int i = 0;i < (int)minfovec.size(); i+= span ){
		int target = min( (int)minfovec.size() - 1, i + span - 1 );
		vec.push_back( TimePair( minfovec[i].first , minfovec[ target ].second ) );
	}
	return vec;
}

/*
	期間情報の取得関係関数
	TimePairVector CDBSyzShinMainView::GetXXXXXXXX()
	基本的な考え　デフォルトのリストから、不要なものをカットして返す

	ポイント 至日 ＋ 1日が次の期間の開始日！
*/

TimePairVector CDBSyzShinMainView::GetTyukanTimeVector()
{
	TimePairVector vec = GetTimeVector( GetMonthCnt() );
	if( !vec.size() ) return vec;

	vec.erase( remove_if( vec.begin(), vec.end(), pred( CTimeControl::GetCTime(m_pZmSub->zvol->ee_ymd), pred::equal,TRUE) ));

	CTime tyukan_end = pastset.GetSetTyukanKikan().second;
	if( curset.IsMinashi() && pastset.IsKakuteiShinkoku() )
		tyukan_end = pastset.GetSetKessanKikan().second;
	else
		if( !IsExistsHistory() ) return vec;

	TimePairVector::iterator it;
//2018.03.13 UPDATE START
//	if( pastset.GetKubun() == kakutei && curset.IsMinashi() )
	if( (pastset.GetKubun() == kakutei||pastset.GetKubun() == junkakutei) && curset.IsMinashi() )
//2018.03.13 UPDATE END
		it = remove_if( vec.begin(), vec.end(), pred(tyukan_end,pred::orless,FALSE) );
	else
		it = remove_if( vec.begin(), vec.end(), pred(tyukan_end,pred::less,TRUE) );

	vec.erase( it, vec.end() );

	CTime erase = tyukan_end + CTimeSpan(1, 0, 0, 0);
	vec.erase( remove_if( vec.begin(), vec.end(), pred(erase,pred::over,FALSE) ), vec.end() );
 
	return vec;
}

/*
	みなし課税期間関係
	TimePairVector CDBSyzShinMainView::GetMinashiKazeiKikan() // ss_ymd ～ 期中
	TimePairVector CDBSyzShinMainView::GetMinashiTyuukanKikan() // 期中 ～ ee_ymd

	基本的な考え
	現在の申告区分より、期首 ～ 期中　のみなしか
				  　　　期中 ～ 期末　のみなしか の判断を行い どちらかを選択

	ポイント
	「みなしを選択する」ときと「自身がみなし」のときで対処がちがう
	・みなしを選択するとき　これから選択可能な期間を導出して返す
	・自身がみなし			自身の期間を返す
	・結果が空の時 -> みなし課税期間の選択不可

	TODO:履歴情報が存在せず拠り所になる情報がないときでも
	　　 総当たりの組み合わせを提示しユーザに選択させる。
	     -> どちらをどのタイミングで提示する？ユーザーの操作は？
		 
	ex)
	TimePairVector vec;
	TimePairVector minfovec = GetShinMininfovec();
	for(TimePairVector::iterator it = minfovec.begin();it != minfovec.end();it++)
		vec.push_back( TimePair( CTimeControl::GetCTime(m_pZmSub->zvol->ss_ymd), (*it).second ) );

	for(TimePairVector::iterator it = minfovec.begin();it != minfovec.end();it++)
		vec.push_back( TimePair( (*it).first, CTimeControl::GetCTime(m_pZmSub->zvol->ee_ymd) );
*/

TimePairVector CDBSyzShinMainView::GetMinashiKazeiKikan()
{
	TimePairVector vec;
	CSnset* snset = GetRirekiSnset(&CSnset::IsKakuteiShinkoku);
	if( snset ){
		if( snset->IsMinashi() )
			vec.push_back( TimePair( snset->GetSetKessanKikan()) );
		else{
			CTime smd = snset->GetSetKessanKikan().second + CTimeSpan(1, 0, 0, 0);
			CTime emd = CTimeControl::GetCTime(m_pZmSub->zvol->ee_ymd);
			if( smd > emd ) return vec;
			vec.push_back( TimePair( smd,emd ));
		}
		return vec;
	}

	snset = GetRirekiSnset(&CSnset::IsTyukanShinkoku);
	if( !snset ) return vec;

	if( snset->IsMinashi() ){
		vec.push_back( TimePair(snset->GetSetKessanKikan()) );
		return vec;
	}

	TimePair t1 = snset->GetSetTyukanKikan();
	vec = GetTimeVector( snset->GetSetMcount() );
	vec.erase( remove_if( vec.begin(), vec.end(), pred(t1.first,pred::orless,FALSE) ), vec.end() );

	if( !vec.size() ) return vec;

	TimePairVector ret;
	ret.push_back( TimePair( CTimeControl::GetCTime(m_pZmSub->zvol->ss_ymd), vec[0].second) );
	return ret;
}

TimePairVector CDBSyzShinMainView::GetMinashiTyuukanKikan()
{
	TimePairVector vec;
	CSnset* snset = GetRirekiSnset(&CSnset::IsTyukanShinkoku);
	if( snset ){
		if( snset->IsMinashi() )
			vec.push_back( TimePair( snset->GetSetKessanKikan()) );
		else{
			snset = GetRirekiSnset(&CSnset::IsKakuteiShinkoku);
			if(!snset) return vec;
			CTime smd = snset->GetSetKessanKikan().second + CTimeSpan(1, 0, 0, 0);
			CTime emd = CTimeControl::GetCTime(m_pZmSub->zvol->ee_ymd);
			if( smd > emd ) return vec;
			vec.push_back( TimePair( smd,emd ));
		}
		return vec;
	}

	snset = GetRirekiSnset(&CSnset::IsKakuteiShinkoku);
	if( !snset ) return vec;

	CTime t1 = snset->GetSetKessanKikan().second;
	vec = GetTimeVector( snset->GetSetMcount() );
	vec.erase( remove_if( vec.begin(), vec.end(), pred(t1 + CTimeSpan(1,0,0,0),pred::less,FALSE) ), vec.end() );

	if( !vec.size() ) return vec;

	TimePairVector ret;
	ret.push_back( TimePair( vec[0].first, CTimeControl::GetCTime(m_pZmSub->zvol->ee_ymd) ) );
	return ret;
}

TimePairVector CDBSyzShinMainView::GetKessanTimeVector()
{
	TimePairVector vec;

	if( curset.IsMinashi() ){
		if( curset.IsTyukanShinkoku() ) return GetMinashiTyuukanKikan();
		if( curset.IsKakuteiShinkoku() ) return GetMinashiKazeiKikan();
	}

	if( curset.IsKakuteiShinkoku() && IsKikanTansyuku() ){
		vec = GetTimeVector( GetMonthCnt() );
		if( !vec.size() ) return vec;
		if( !IsExistsHistory() ) return vec;
		CSnset* snset = GetRirekiSnset(&CSnset::IsKakuteiShinkoku);
		TimePairVector::iterator it;
		if( snset && snset->IsMinashi() )
			it = remove_if( vec.begin(), vec.end(), pred(pastset.GetSetKessanKikan().second,pred::orless,TRUE) );
		else
			it = remove_if( vec.begin(), vec.end(), pred(pastset.GetSetKessanKikan().second,pred::less,TRUE) );
		vec.erase( it, vec.end() );
		CTime erase = pastset.GetSetKessanKikan().second + CTimeSpan(1, 0, 0, 0);
		vec.erase( remove_if( vec.begin(), vec.end(), pred(erase,pred::over,FALSE) ), vec.end() );
	}
	else
		vec.push_back( TimePair(CTimeControl::GetCTime(m_pZmSub->zvol->ss_ymd), CTimeControl::GetCTime(m_pZmSub->zvol->ee_ymd)) );

	return vec;
}

TimePair CDBSyzShinMainView::GetCurrentTyukanKikan()
{
	TimePairVector timevector = GetTyukanTimeVector();
	int cur = m_ComboTyukanKikan.GetCurSel();
	if( cur < 0 || !timevector.size()) return TimePair();
	return timevector[ min( cur, (int)timevector.size() - 1) ];
}

TimePair CDBSyzShinMainView::GetCurrentKessanKikan()
{
	TimePairVector timevector = GetKessanTimeVector();
	int cur = m_ComboKessanKikan.GetCurSel();
	if(cur < 0 || !timevector.size()) return TimePair();
	return timevector[ min( cur, (int)timevector.size() - 1) ];
}

void CDBSyzShinMainView::ChangeComboSkkbn()
{
	std::vector< std::pair<int,CString> > ComboVector = GetVectorSkbnName();
	m_ComboSkkbn.ResetContent();

	for(UINT i = 0;i<ComboVector.size();i++)
		m_ComboSkkbn.AddString(ComboVector[i].second);

	m_ComboSkkbn.EnableWindow( !IsHistoryMode() );
}

void CDBSyzShinMainView::ChangeComboTyukanKikan()
{
	TimePairVector timevector = GetTyukanTimeVector();

	m_ComboTyukanKikan.ResetContent();
	for(UINT i = 0;i<timevector.size();i++)
		m_ComboTyukanKikan.AddString( CTimeControl::GetDateStrPair( timevector[i] ) );

	if( !timevector.size() || !curset.IsTyukanShinkoku() ){
		m_ComboTyukanKikan.EnableWindow(FALSE);
		return;
	}

	m_ComboTyukanKikan.EnableWindow( timevector.size() > 1 && !IsHistoryMode() );
	SetComboTyukanKikan();
}

UINT CDBSyzShinMainView::GetKikanOfs( TimePairVector& v, TimePair t )
{
	TimePairVector::iterator it = find(v.begin(),v.end(), t);
	if( it == v.end() ) return 0;
	return UINT( it - v.begin() );
}

void CDBSyzShinMainView::SetComboKessanKikan()
{
	TimePair t = curset.GetSetKessanKikan();

	if( t == TimePair() ) {

		m_ComboKessanKikan.SetCurSel(0);

	}else{
		UINT ofs = GetKikanOfs(GetKessanTimeVector(),t);
		m_ComboKessanKikan.SetCurSel(ofs);
	}
}

void CDBSyzShinMainView::SetComboTyukanKikan()
{
	TimePair t = curset.GetSetTyukanKikan();

	if( t == TimePair() ) {

		m_ComboTyukanKikan.SetCurSel(0);

	}else{
		UINT ofs = GetKikanOfs(GetTyukanTimeVector(),t);
		m_ComboTyukanKikan.SetCurSel(ofs);
	}
}

void CDBSyzShinMainView::ChangeComboKessanKikan()
{
	TimePairVector timevector = GetKessanTimeVector();
	
	m_ComboKessanKikan.ResetContent();
	for(UINT i = 0;i<timevector.size();i++)
		m_ComboKessanKikan.AddString( CTimeControl::GetDateStrPair( timevector[i]) );

	m_ComboKessanKikan.GetSetColorChange() = curset.IsMinashi();

	if(	!timevector.size() ){
		m_ComboKessanKikan.EnableWindow(FALSE);
		return;
	}

	m_ComboKessanKikan.EnableWindow( timevector.size() > 1 && !IsHistoryMode() );
	SetComboKessanKikan();
}

int CDBSyzShinMainView::IsEnableNextShinkoku()
{
	if( curset.IsTyukanShinkoku() && ( !m_ComboKessanKikan.GetCount() || !m_ComboTyukanKikan.GetCount() ) )
		return FALSE;
	
	if( curset.IsKakuteiShinkoku() && !m_ComboKessanKikan.GetCount() )
		return FALSE;

	return TRUE;
}

int CDBSyzShinMainView::GetTimeToJcount(int mcount, TimePair time)
{
	TimePairVector minf = GetTimeVector( mcount );
	TimePairVector::iterator it = find(minf.begin(),minf.end(), time );
	if( minf.end() != it )
		return (int)(it - minf.begin());

	return 0;
}

void CDBSyzShinMainView::GetComboTyukanKikan()
{
	if( GetMonthSel() >= 0 &&  curset.IsTyukanShinkoku() ){
		curset.Sn_JCOUNT = GetTimeToJcount( GetMonthCnt(), GetCurrentTyukanKikan() ) + 1;
//		Snh.Sn_JCOUNT = curset.Sn_JCOUNT;
	}
	else{
		curset.Sn_JCOUNT = 0;
//		Snh.Sn_JCOUNT = 0;
	}
}

void CDBSyzShinMainView::SetMonthCnt()
{
	map<int,int> McountMap = GetMapMcount();
	m_ComboMonthCnt.ResetContent();

	if( curset.IsTyukanShinkoku() || ( curset.IsKakuteiShinkoku() ) && IsKikanTansyuku() ){
		map<int,CString> NameMap = GetMcountName( curset.GetKubun() );
		for(map<int,int>::iterator it = McountMap.begin();it != McountMap.end();it++){
			m_ComboMonthCnt.AddString(NameMap[(*it).second]);
		}
	}

	McountMap = GetMapMcount(TRUE);
	map<int,int>::iterator it = McountMap.find( curset.GetSetMcount() );
	if( it != McountMap.end() )
		m_ComboMonthCnt.SetCurSel((*it).second);
	else{
		m_ComboMonthCnt.SetCurSel(0);
		SetCurrentMcount();
	}
}

void CDBSyzShinMainView::ChangeTyukanKikanTansyuku()
{
	SetMonthCnt();

	m_ComboTyukanAri.EnableWindow( !m_ComboKikanTansyuku.GetCurSel() && !IsExistsHistory() || m_CheckTokusyu.GetCheck() );
	m_ComboKikanTansyuku.EnableWindow( !m_ComboTyukanAri.GetCurSel() && !IsExistsHistory() || m_CheckTokusyu.GetCheck() );

	if( curset.IsTyukanShinkoku() || ( curset.IsKakuteiShinkoku() && IsKikanTansyuku()) )
		m_ComboMonthCnt.EnableWindow( !IsExistsHistory() && !IsHistoryMode() || m_CheckTokusyu.GetCheck() );
	else
		m_ComboMonthCnt.EnableWindow(  FALSE );
}

void CDBSyzShinMainView::SetComboKikanTansyuku()
{
	if( curset.Sn_KZTNSK )
		m_ComboKikanTansyuku.SetCurSel(1);
	else
		m_ComboKikanTansyuku.SetCurSel(0);
}

void CDBSyzShinMainView::SetComboTyukanAri()
{
	if( curset.Sn_MCOUNT )
		m_ComboTyukanAri.SetCurSel(1);
	else
		m_ComboTyukanAri.SetCurSel(0);
}

void CDBSyzShinMainView::SetComboSkkbn()
{
	map<int,int> kubnmap = GetMapKubun( TRUE );
	map<int,int>::iterator it = kubnmap.find( curset.GetKubun() );
	if( it != kubnmap.end() )
		m_ComboSkkbn.SetCurSel((*it).second);
	else{
		m_ComboSkkbn.SetCurSel(0);
		curset.SetKubun(kakutei);
	}
}

int CDBSyzShinMainView::IsHistoryMode()
{
	return m_ButtonRirekiSel.GetCheck();
}

int CDBSyzShinMainView::IsMinashi()
{
	return m_CheckMinashi.GetCheck();
}

void CDBSyzShinMainView::ChangeRirekiSel()
{
	m_ButtonRirekiSel.EnableWindow( IsExistsHistory() );
	m_PastReport.EnableWindow(IsHistoryMode() && IsExistsHistory());

	((ICSReportPaintManagerEX*)m_PastReport.GetPaintManager())->GetSetHideSelection() = !m_PastReport.IsWindowEnabled();
	m_PastReport.RedrawControl();
}

void SetComboStr(CComboBox& box,CString str)
{
	box.ResetContent();
	box.AddString(str);
	box.SetCurSel(0);
}

int CDBSyzShinMainView::SetCurHistoryCheck()
{
	CHistoryReport::RecType rec = HistoryVec[GetHistoryCurSel()];
	CSnset& snset = rec.GetSnset();

	m_Check1.SetCheck( snset.Sn_TUMIAG&0x01 );
	m_Check2.SetCheck( snset.Sn_TUMIAG&0x02 );
	m_Check3.SetCheck( snset.Sn_SPECIAL&0x01 );
	m_Check4.SetCheck( snset.Sn_Sign2&0x02 );
	m_Check5.SetCheck( snset.SVmzsw&0x01 );
	m_Check21.SetCheck( snset.Sn_Sign4&0x80 );
//	m_Check8.SetCheck( !(snset.Sn_ZeiHou&0x80) );
	m_CheckMinashi.SetCheck( snset.Sn_Sign4&0x40 );
//--> '15.08.21 INS START
	m_Check6.SetCheck( snset.m_DispTabSgn&0x01 );
//<-- '15.08.21 INS END

	//-->特例帳票追加
	//基本画面チェック情報
	m_Check7.SetCheck(snset.m_DispTabSgn & 0x80);
	//<-- 

	m_Check9.SetCheck(snset.m_DispTabSgn & 0x1000);

	SetComboStr( m_ComboSkkbn, GetMapSkbnName()[snset.GetKubun()] );

	if( snset.IsTyukanShinkoku() ){
		SetComboStr( m_ComboTyukanKikan ,CTimeControl::GetDateStrPair(snset.GetSetTyukanKikan()) );
		SetComboStr( m_ComboKessanKikan ,CTimeControl::GetDateStrPair(snset.GetSetKessanKikan()) );
	}else{
		m_ComboTyukanKikan.ResetContent();
		SetComboStr( m_ComboKessanKikan ,CTimeControl::GetDateStrPair(snset.GetSetKessanKikan()));
	}
	SetComboStr( m_ComboMonthCnt,GetMcountName(snset.GetKubun())[snset.GetSetMcount()] );

//	m_Middle = snset.GetKubun() == yotei ? 1 : 0;
	
	ASSERT( m_pMainFrame );
	CString ErMs;
//	SyzShinOpen( GetHistorySeq() , ErMs );
	m_pMainFrame->OpenSyzShin( GetHistorySeq() , ErMs );
	snset.GetSnData(*m_pSnHeadData);

//	SetActViewVer(&snset);
	SetActViewVer(m_pSnHeadData);

	// 他表履歴切替
	m_pMainFrame->ChangeViewSnSeq( GetHistorySeq() );

	m_ComboKessanKikan.GetSetColorChange() = snset.IsMinashi();
	m_ComboKessanKikan.Invalidate(TRUE);

	m_pMainFrame->ReadOfficeInfo( m_pSnHeadData->SVzisq, m_pSnHeadData->SubInf );

//--> '15.07.21 INS START
	InitMynumberControl();
//<-- '15.07.21 INS END

	Static_Update( m_pSnHeadData->Sn_Sign4 );
	
	return 0;
}

void CDBSyzShinMainView::SetHistoryCtrl(int ishistory)
{
	if( !ishistory ) return;

	BOOL	isRirekiChg = FALSE;
	CString	tmpStr;
	GetDlgItem(IDC_CHECK_RIREKISEL)->GetWindowText( tmpStr );
	if( tmpStr != "現在申告に戻る" ){
		isRirekiChg = TRUE;
	}

	if( isRirekiChg ){
		GetDlgItem(IDC_CHECK_RIREKISEL)->SetWindowText("現在申告に戻る");
	}
//	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("<<履歴選択中>>");
//	m_InfoGroup.SetWindowText("<<履歴選択中>>");
	m_PastReport.SetFocus();
	m_PastReport.SetFocusedRow(max(GetHistoryCurSel(),0));

	SetCurHistoryCheck();
	m_ComboTyukanAri.EnableWindow(FALSE);
	m_ComboKikanTansyuku.EnableWindow(FALSE);

	m_ComboSkkbn.EnableWindow(FALSE);
	m_ComboTyukanKikan.EnableWindow(FALSE);
	m_ComboKessanKikan.EnableWindow(FALSE);
	m_ComboMonthCnt.EnableWindow(FALSE);

	m_Check1.EnableWindow(FALSE);
	m_Check2.EnableWindow(FALSE);
	m_Check3.EnableWindow(FALSE);
	m_Check4.EnableWindow(FALSE);
	m_Check5.EnableWindow(FALSE);
//--> '15.08.21 INS START
	m_Check6.EnableWindow(FALSE);
//<-- '15.08.21 INS END

	//-->特例計算チェック追加
	m_Check7.EnableWindow(FALSE);

	m_Check9.EnableWindow ( FALSE );

	m_Check10.EnableWindow(FALSE);

	m_IsAbleTkrei = FALSE;

	m_btn1.ShowWindow(SW_HIDE);
	//<------------------

	m_Check21.EnableWindow(FALSE);
//	m_Check8.EnableWindow(FALSE);

	m_CheckTokusyu.EnableWindow(FALSE);
	m_CheckMinashi.EnableWindow(FALSE);

//--> '14.01.29 INS START
	if( isRirekiChg ){
		m_InfoGroup.EnableWindow( FALSE );
		m_InfoGroup.ShowWindow( SW_HIDE );

		m_RInfoGroup.EnableWindow( TRUE );
		m_RInfoGroup.ShowWindow( SW_SHOW );

		m_RInfoGroup.ShowWindow( SW_HIDE );
		m_RInfoGroup.ShowWindow( SW_SHOW );

		if( m_pMainFrame ){
//			m_pMainFrame->OpenSyzShin( GetHistorySeq() , ErMs );
//			snset.GetSnData(*m_pSnHeadData);
//			// 他表履歴切替
//			m_pMainFrame->ChangeViewSnSeq( GetHistorySeq() );
		}
	}
//<-- '14.01.29 INS END

	if( m_pSnHeadData ){
		Static_Update( m_pSnHeadData->Sn_Sign4 );
	}
}

void CDBSyzShinMainView::SetCurCtrl( int ishistory )
{
	if( ishistory ) return;

	ASSERT( m_pMainFrame );
	if( m_pMainFrame == NULL ){
		return;
	}

//--> '14.02.24 INS START
// 会社切替で不正になる為に、閉じましたが履歴呼び出しでキチンと動かないので、保留。
//	if( m_pMainFrame ){
//		if( m_pMainFrame->IsCalledFork() ){
//			return;
//		}
//	}
//<-- '14.02.24 INS END

	BOOL	isRirekiChg = FALSE;
	CString	tmpStr;
	GetDlgItem(IDC_CHECK_RIREKISEL)->GetWindowText( tmpStr );
	if( tmpStr != "履歴選択" ){
		isRirekiChg = TRUE;
	}

	if( isRirekiChg ){
		GetDlgItem(IDC_CHECK_RIREKISEL)->SetWindowText("履歴選択");
////	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("<<申告選択>>");
//	m_InfoGroup.SetWindowText("<<申告選択>>");

//--> '14.01.29 INS START
		m_RInfoGroup.EnableWindow( FALSE );
		m_RInfoGroup.ShowWindow( SW_HIDE );

		m_InfoGroup.EnableWindow( TRUE );
		m_InfoGroup.ShowWindow( SW_SHOW );
//<-- '14.01.29 INS END

//--> '14.03.15 INS START
		if( m_pMainFrame->IsNeedReOpenZmtbl() ){
			m_pMainFrame->ReOpenZmtbl();
			m_pMainFrame->SetReOpenFlg( FALSE );
		}
//<-- '14.03.15 INS END
	}

	m_Check1.EnableWindow(TRUE);
	m_Check2.EnableWindow(TRUE);
	m_Check3.EnableWindow(TRUE);
	m_Check4.EnableWindow(TRUE);
	m_Check5.EnableWindow(TRUE);
//--> '15.08.21 INS START
	if( m_IsAbleTkkz ){
		m_Check6.EnableWindow(TRUE);
	}
	else{
		m_Check6.EnableWindow(FALSE);
	}

	//-->特例計算チェック追加

#ifdef _20190830_SYUKA_ //yoshida190820
	//特例チェック非表示
	m_Check7.ShowWindow(SW_HIDE);
	m_btn1.ShowWindow(SW_HIDE);
	m_IsAbleTkrei = FALSE;

#else
	// SetTkreiChk()の中で判定するように変更
	/*if ((m_pSnHeadData->Sn_KDAYE < TOKUREI_KIKAN_S)	|| (m_pSnHeadData->Sn_KDAYS > TOKUREI_KIKAN_E)) {	// 20231025 9月30日をまたぐ間は適用可能なため、開始日が超えているかどうかで判定する
	//if ((m_pSnHeadData->Sn_KDAYE < TOKUREI_KIKAN_S)	|| (m_pSnHeadData->Sn_KDAYE > TOKUREI_KIKAN_E)) {	// 20231025 del
		m_Check7.EnableWindow(FALSE);
		m_IsAbleTkrei = FALSE;
	}
	else {
		//m_Check7.EnableWindow(TRUE);
		//m_IsAbleTkrei = TRUE;
		SetTkreiChk();
	}*/
#endif
	//<--------------------------------

	if ( m_IsAbleR02Reform ) {
		m_Check9.EnableWindow ( TRUE );
	}
	else {
		m_Check9.EnableWindow ( FALSE );
	}

//<-- '15.08.21 INS END
	m_Check21.EnableWindow(TRUE);
//	m_Check8.EnableWindow(TRUE);
	m_CheckTokusyu.EnableWindow(TRUE);
	m_CheckMinashi.EnableWindow(TRUE);

	CString ErMs;
/*	ZaimuVolume_Cnv();
	SyzVolume_Cnv();*/
//	SyzShinOpen( 0, ErMs );
	m_pMainFrame->OpenSyzShin( 0, ErMs );
	// 他表履歴切替
	m_pMainFrame->ChangeViewSnSeq( 0 );
	// 次回切替時強制再描画
	if( isRirekiChg ){
		m_pMainFrame->SetCompChangeFlg( TRUE );
	}

	pastset = curset = CSnset(*m_pSnHeadData);
//	m_Middle = curset.GetKubun() == yotei ? 1 : 0;

	SetComboTyukanAri();
	SetComboKikanTansyuku();
	ChangeTyukanKikanTansyuku();

	ChangeComboSkkbn();
	SetComboSkkbn();

	ChangeComboTyukanKikan();
	ChangeComboKessanKikan();
	
//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTkreiChk();//特例チェック

	SetR02Chk();

	SetActViewVer(m_pSnHeadData);

//	SyzShinEntryIniz();
	InitDisp();

//--> '14.01.29 INS START
/*	GetDlgItem(IDC_STATIC_B06)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B06)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B51)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B51)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B52)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B52)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B53)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B53)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B54)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B54)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B55)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B55)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B56)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B56)->ShowWindow( SW_SHOW );

	GetDlgItem(IDC_STATIC_B57)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_B57)->ShowWindow( SW_SHOW );*/

//--> 再描画用に移動
//	GetDlgItem(IDC_STATIC_I06)->ShowWindow( SW_HIDE );
//	GetDlgItem(IDC_STATIC_I06)->ShowWindow( SW_SHOW );
//<--

	if( isRirekiChg ){
		m_InfoGroup.ShowWindow( SW_HIDE );
		m_InfoGroup.ShowWindow( SW_SHOW );
	}
//<-- '14.01.29 INS END

	m_PastReport.EnableWindow(FALSE);
	HistoryReportInitCur();

//--> 再描画用に移動
	GetDlgItem(IDC_STATIC_I06)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_STATIC_I06)->ShowWindow( SW_SHOW );
//<--

	m_pMainFrame->ReadOfficeInfo( m_pSnHeadData->SVzisq, m_pSnHeadData->SubInf );
}

int CDBSyzShinMainView::SetCtrl()
{
	ChangeRirekiSel();
	SetHistoryCtrl( IsHistoryMode() );
	SetCurCtrl(	IsHistoryMode() );

	CMainFrame* frm = (CMainFrame *)GetParentOwner();
	frm->UpdateRirekiIndicator( IsHistoryMode() );
//--> '15.07.21 INS START
	InitMynumberControl();
//<-- '15.07.21 INS END
	return 0;
}

void CDBSyzShinMainView::HistoryModeOff()
{
	m_ButtonRirekiSel.SetCheck(0);
	SetCtrl();
}

void CDBSyzShinMainView::SetOldData()
{
	old_Sn_SKKBN  = m_pSnHeadData->Sn_SKKBN;

	if( curset.IsKakuteiShinkoku() && curset.IsMinashi() ){
		old_tansyuku = CTime(-1);
		old_Sn_KZTNSK = 0xff;
		old_Sn_MCOUNT = 0xff;
	}else{
		old_tansyuku  = FindTansyukuKaishi();
		old_Sn_KZTNSK = curset.IsKakuteiShinkoku() ? m_pSnHeadData->Sn_KZTNSK : 0;
		old_Sn_MCOUNT = curset.IsTyukanShinkoku() ? m_pSnHeadData->Sn_MCOUNT : 0;
	}
}

vector<MPAYMENTVAL> CDBSyzShinMainView::ShinKinfoRead()
{
	CArith arith(0x16);
	vector<MPAYMENTVAL> vec;

	CString sq;
	sq += "select Sn_EDNOFZ,Sn_TEDNOF from shinkinfo order by seq";
	
	CRecordset dbrec(m_pZmSub->m_database);
	try {
		dbrec.Open( CRecordset::forwardOnly, sq );
		while ( dbrec.IsEOF() == FALSE ){

			CString kubun,Sn_EDNOFZ,Sn_TEDNOF;
			dbrec.GetFieldValue( "Sn_EDNOFZ", Sn_EDNOFZ );
			dbrec.GetFieldValue( "Sn_TEDNOF", Sn_TEDNOF );

			MPAYMENTVAL val;
			arith.l_input( val.KMPval, (LPSTR)(LPCSTR)Sn_EDNOFZ );
			arith.l_input( val.TMPval, (LPSTR)(LPCSTR)Sn_TEDNOF );

			vec.push_back(val);
			dbrec.MoveNext();
		}
	}catch(CDBException* dbe){
//		dbrec.Close();
		dbe->Delete();
	}
	dbrec.Close();
	return vec;
}

int CDBSyzShinMainView::ShinKinfoWrite(vector<MPAYMENTVAL>& vec)
{
	CArith arith(0x16);

	CString sq;
	sq += "IF OBJECTPROPERTY ( object_id('shinkinfo'),'ISTABLE') IS NOT NULL drop table shinkinfo \n";

	sq += "CREATE TABLE [dbo].[shinkinfo]( ";
	sq += "		 [seq] [int] NOT NULL,  [Sn_EDNOFZ] [decimal](14) NOT NULL, [Sn_TEDNOF] [decimal](14) NOT NULL ";
	sq += " ) \n";

	for(int i = 0; i != vec.size(); i++ ) {
		if( !vec[i].edit ) continue;
		CString seq;
		seq.Format("%d",i);
		char ednofz[32] = {};
		char tednof[32] = {};
		arith.l_print( ednofz, vec[i].KMPval, "SSSSSSSSSSS9" );
		arith.l_print( tednof, vec[i].TMPval, "SSSSSSSSSSS9" );
		sq += "INSERT INTO shinkinfo SELECT "+ seq +", " + CString(ednofz) + ", " + CString(tednof) + " \n";
	}

	CDatabase *db = m_pZmSub->m_database;
	db->ExecuteSQL( sq ); 
	return 0;
}

vector<MPAYMENTVAL> CDBSyzShinMainView::OldKinfoRead()
{
	vector<MPAYMENTVAL> vec; 
	switch( m_pSnHeadData->Sn_MCOUNT ){
	case 3:
		{
			MPAYMENTVAL val;
			memmove( val.KMPval, m_pSnHeadData->Sn_TYNF1 ,6 );
			memmove( val.TMPval, m_pSnHeadData->Sn_TTYNF1, 6 );
			vec.push_back(val);

			memmove( val.KMPval, m_pSnHeadData->Sn_TYNF2 ,6 );
			memmove( val.TMPval, m_pSnHeadData->Sn_TTYNF2, 6 );
			vec.push_back(val);

			memmove( val.KMPval, m_pSnHeadData->Sn_TYNF3 ,6 );
			memmove( val.TMPval, m_pSnHeadData->Sn_TTYNF3, 6 );
			vec.push_back(val);

			memmove( val.KMPval, m_pSnHeadData->Sn_TYNF4 ,6 );
			memmove( val.TMPval, m_pSnHeadData->Sn_TTYNF4, 6 );
			vec.push_back(val);
		}
		break;
	case 11:
		for(int i = 0;i<11;i++){
			MPAYMENTVAL val;
			memmove( val.KMPval, m_pSnHeadData->Sn_TYNF[i] ,6 );
			memmove( val.TMPval, m_pSnHeadData->Sn_TTNF[i], 6 );
			vec.push_back(val);
		}
		break;
	default:
		break;
	}
	return vec;
}

int GetMonthSpan(int date_start, int date_end, TimePairVector mvec)
{
	CTime smd = CTimeControl::GetCTime(date_start);
	CTime emd = CTimeControl::GetCTime(date_end);

	int span = 0;
	for(int i = 0; i != mvec.size(); i++ ) {
		if( mvec[i].first  >= smd )
			span++;
		if( mvec[i].second >= emd )
			break;
	}
	return span;
}

vector<MPAYMENTVAL> CDBSyzShinMainView::MiddlePaymentRead(int old_dataread)
{
	// TODO:  ここに初期化を追加してください
	CArith arith(0x16);
	vector<MPAYMENTVAL> kinfo = !old_dataread ? ShinKinfoRead() : OldKinfoRead();

	CString sq;
	sq += "if object_id('tempdb..#temp_midpay', 'u') is not null drop table #temp_midpay \n";

	sq += ";with \n";
	sq += "szsnk_row(Sn_SKKBN,Sn_Syuusei,Sn_EDNOFZ,Sn_TEDNOF,Sn_MDAYS,Sn_MDAYE,rownum)  \n";
	sq += "as( \n";
	sq += "	SELECT Sn_SKKBN,Sn_Syuusei,Sn_EDNOFZ,Sn_TEDNOF,Sn_MDAYS,Sn_MDAYE, ROW_NUMBER()OVER(PARTITION BY Sn_MDAYS,Sn_MDAYE ORDER BY Sn_SKKBN DESC) \n";
	sq += "	FROM   szsnk  \n";
	sq += "	WHERE  Sn_SKKBN = 2 OR Sn_SKKBN = 4 \n";
	sq += ") \n";
	sq += "select * into #temp_midpay from szsnk_row where rownum = 1 \n";
	m_pZmSub->m_database->ExecuteSQL(sq);

	vector<MPAYMENTVAL> vec;
	CRecordset dbrec(m_pZmSub->m_database);
	try {
	
		CString sq;
		sq = "select Sn_SKKBN,Sn_Syuusei,Sn_EDNOFZ,Sn_TEDNOF,Sn_MDAYS,Sn_MDAYE from #temp_midpay";
		
		dbrec.Open( CRecordset::forwardOnly, sq );
		while ( dbrec.IsEOF() == FALSE ){
		
			// unfinished
			CString Sn_SKKBN,Sn_Syuusei,Sn_EDNOFZ,Sn_TEDNOF,Sn_MDAYS,Sn_MDAYE;
		
			dbrec.GetFieldValue( "Sn_SKKBN", Sn_SKKBN );
			dbrec.GetFieldValue( "Sn_Syuusei", Sn_Syuusei );
			dbrec.GetFieldValue( "Sn_EDNOFZ", Sn_EDNOFZ );
			dbrec.GetFieldValue( "Sn_TEDNOF", Sn_TEDNOF );
			dbrec.GetFieldValue( "Sn_MDAYS", Sn_MDAYS );
			dbrec.GetFieldValue( "Sn_MDAYE", Sn_MDAYE );

			int kubun = Sn_SKKBN == CString("2") ? atoi(Sn_Syuusei)&0x08 ? yotei : tyuukan: tyuukan_syuuse;

			char ednofz[6],tednof[6];
			arith.l_input( ednofz, (LPSTR)(LPCSTR)Sn_EDNOFZ );
			arith.l_input( tednof, (LPSTR)(LPCSTR)Sn_TEDNOF );

			vec.push_back(MPAYMENTVAL(kubun,atoi(Sn_MDAYS),atoi(Sn_MDAYE),ednofz,tednof));
			dbrec.MoveNext();
		}

	}catch(CDBException* dbe){
//		dbrec.Close();
		dbe->Delete();
	}
	dbrec.Close();

	if( vec.size() ){
		int span = GetMonthSpan( vec[0].smd, vec[0].emd, GetShinMininfovec() );
		int count = 0;
		switch( span ){
		case 3:	count = 3; break;
		case 6:	count = 1; break;
		default:count = 11; break;
		}

		TimePairVector minf = GetTimeVector( count );

		int jcont = GetTimeToJcount( count, TimePair( CTimeControl::GetCTime(vec[0].smd), CTimeControl::GetCTime(vec[0].emd) ) );
		if( jcont > 0 ){
		
			for(int i = 0;i<jcont;i++)
				vec.insert(vec.begin(),MPAYMENTVAL());
			
			for(int i = 0;i<min( jcont, (int)kinfo.size() ) ;i++)
				vec[i] = kinfo[i];
	
			for(int i = 0;i<jcont;i++){
				vec[i].smd = CTimeControl::GetIntTime(minf[i].first);
				vec[i].emd = CTimeControl::GetIntTime(minf[i].second);
			}
		}
	}
	return vec;
}

int CDBSyzShinMainView::WriteNofu()
{
	ASSERT( m_pMainFrame );
	if( m_pMainFrame == NULL ){
		return -1;
	}

	char Sn_ZZURI[32]  = {}, Sn_ZZNOFZ[32] = {}, Sn_ZURI[32] = {}, Sn_ZNOFZ[32] = {};

	char	tmpMony[MONY_BUF_SIZE] = {0};
	CString	tag, chtag;

	int ActViewVer = GetActViewVer();

	// 履歴ボタン押下時にサインをセットし税額情報の引継ぎを行わないように対応したため、以下をカット
	//int otherViewVer = -1;
	//BOOL isOtherView = FALSE;
	//if ( ( m_pZmSub->zvol->ss_ymd < ICS_SH_10PER_REFORM_DAY ) && ( m_pZmSub->zvol->ee_ymd >= ICS_SH_10PER_REFORM_DAY ) ) {
	//	isOtherView = TRUE;
	//}

	// 10%改正対応
	//tag = _T("AAF00000");
	if ( ActViewVer == ID_VER_SYZ31 ) {
		tag = _T("AAM00030");
		//if ( isOtherView ) {
		//	otherViewVer = ID_VER_SYZ26;
		//}
	}
	else {
		tag = _T("AAF00000");
		//if ( isOtherView ) {
		//	otherViewVer = ID_VER_SYZ31;
		//}
	}

	m_pMainFrame->GetMony( tag, chtag, tmpMony, ActViewVer );
	m_Arith.l_print( Sn_ZZURI,  tmpMony, _T("SSSSSSSSSSSSS9") );
	tag = _T("AAF00020");
	m_pMainFrame->GetMony( tag, chtag, tmpMony, ActViewVer );
	m_Arith.l_print( Sn_ZZNOFZ, tmpMony, _T("SSSSSSSSSSSSS9") );
	tag = _T("AAF00010");
	m_pMainFrame->GetMony( tag, chtag, tmpMony, ActViewVer );
	m_Arith.l_print( Sn_ZURI,   tmpMony, _T("SSSSSSSSSSSSS9") );
	tag = _T("AAF00030");
	m_pMainFrame->GetMony( tag, chtag, tmpMony, ActViewVer );
	m_Arith.l_print( Sn_ZNOFZ, tmpMony, _T("SSSSSSSSSSSSS9") );

	int seq = IsHistoryMode() ? GetHistorySeq() : 0;

	CString sq;
	sq.Format( _T("update szsnk set Sn_ZZURI = %s, Sn_ZZNOFZ = %s, Sn_ZURI = %s, Sn_ZNOFZ = %s where Sn_SEQ = %d"), 
		Sn_ZZURI,Sn_ZZNOFZ,Sn_ZURI,Sn_ZNOFZ, seq );

	try{
		m_pZmSub->m_database->ExecuteSQL(sq);
	}
	catch( CDBException *pDe ){
		pDe->Delete();
		return -1;
	}

	m_pMainFrame->WriteSepData( 0x01 );

	//if ( otherViewVer != -1 ) {
	//	// 不正データに対しても税額情報をセットしておく
	//	m_pMainFrame->TakeOverData ( ActViewVer, otherViewVer );
	//}

	return 0;
}

//-----------------------------------------------------------------------------
// 中間及び期間短縮の更新
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
//int CDBSyzShinMainView::SyzShinRenew()
int CDBSyzShinMainView::RenewSyzShin()
{
	int rt = -1;

	// 区分
	int past_SKKBN = pastset.GetSn_SKKBN();
	int past_MCOUNT = pastset.GetSetMcount();
	int past_MINASHI = pastset.IsMinashi();

	SH_PASTINFO	pastInfo = {0};
	pastInfo.SKKBN = past_SKKBN;
	pastInfo.MCOUNT = past_MCOUNT;
	pastInfo.MINASHI = past_MINASHI;

	if( m_pMainFrame ){
		rt = m_pMainFrame->RenewSyzShin(&pastInfo);
		// 区分切替時に経過措置、特例のチェックが落ちてしまうので削除 ('19.08.05)
/*		curset.SVmzsw = 0;
		m_pSnHeadData->SVmzsw = 0;
//--> '15.08.21 INS START
		curset.m_DispTabSgn = 0;
		m_pSnHeadData->m_DispTabSgn = 0;
//<-- '15.08.21 INS END
*/
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 本表ビューの取得
//-----------------------------------------------------------------------------
// 引数		vtype		：	呼び出しビュータイプ（-1：Kaisei_VERによる自動判定、それ以外：呼び出すビューのタイプ）
//-----------------------------------------------------------------------------
// 返送値	NULL以外	：	現時点での本表・付表ビューのポインタ
//			NULL		：	エラー
//-----------------------------------------------------------------------------
CBaseHyoView* CDBSyzShinMainView::GetHonHyoView(int vtype/*=-1*/)
{
	if( m_pMainFrame ){
		return m_pMainFrame->GetHonHyoView( vtype );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 消費税申告書集計
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SyzShinWrite()
{
	CString ErMs;

	if( !IsHistoryMode() ) {
		curset.MargeSnData( *m_pSnHeadData );
	}

	CBaseHyoView *pBaseView = (CBaseHyoView *)GetHonHyoView();
	if( pBaseView != NULL ) {
		pBaseView->WriteData(FALSE, FALSE);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 履歴関係の情報セット
//-----------------------------------------------------------------------------
// 引数	Snh	：	セット対象データ
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetSnHeadToCSnset( const CSnHeadData& Snh )
{
	curset.SetSnh( Snh );
}

//-----------------------------------------------------------------------------
// 過去データ更新
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetPastData()
{
	int	sel = GetHistoryCurSel();
//-- '15.04.04 --
//	if( (0<=sel) && (sel<HistoryVec.size()) ){
//---------------
	if( (0<=sel) && (sel<(int)HistoryVec.size()) ){
//---------------
		CHistoryReport::RecType rec = HistoryVec[sel];
		CSnset& snset = rec.GetSnset();
		snset.SetSnh( *m_pSnHeadData );

		HistoryVec[sel] = HistoryData( snset, rec.GetUpdate() );
	}
	//--->shimizu140319
	else{
		curset.SetSnh( *m_pSnHeadData );
	}
	//<------------

	return 0;
}


//-----------------------------------------------------------------------------
// 26年プログラムに行くときにkjsnkの25年度の０のデータを削除
//-----------------------------------------------------------------------------
//int CDBSyzShinMainView::DeleteOldKjsnkData()
//{
//	CDatabase* db = m_pZmSub->m_database;
//
//	db->BeginTrans();
//	try{
//		db->ExecuteSQL( GetQuerySelectDrop("kssgn5","Kjsnk") );		// 課税取引金額計算表
//	}catch(CDBException* dbe){
//		ICSMessageBox("更新に失敗しました！\nデータは変更されません。",0, 0, 0, this);
//		db->Rollback();
//		dbe->Delete();
//		return -1;
//	}
//
//	db->CommitTrans();
//
//	return 0;
//}

//-----------------------------------------------------------------------------
// 特定課税仕入有りチェックの状態をセット('15.11.14)
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetTkkzChk()
{
	if( curset.IsHirendou() == 0 ){
		return;
	}

	long	eymd = 0;
	if( curset.IsTyukanShinkoku() ) {
		TimePair t1 = GetCurrentTyukanKikan();
		eymd = CTimeControl::GetIntTime( t1.second );
	}
	else if( curset.IsKakuteiShinkoku() ) {
		TimePair t1 = GetCurrentKessanKikan();
		eymd = CTimeControl::GetIntTime( t1.second );
	}
	if( eymd == 0 ){
		return;
	}

	if( m_pZmSub == NULL ){
		return;
	}
	else if( m_pZmSub->zvol == NULL ){
		return;
	}

	bool bIsAbleTkkz = false;
	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) && (eymd>=ICS_SH_TOKUTEI_REFORM_DAY) ){
		bIsAbleTkkz = true;
	}

	if( m_IsAbleTkkz && (bIsAbleTkkz==false) ){
		SetTkkzChk( 0 );
		m_Check6.EnableWindow( FALSE );
		SetTkkzSign( eymd );
	}
	else if( !m_IsAbleTkkz && (bIsAbleTkkz==true) ){
		m_Check6.EnableWindow( TRUE );
		SetTkkzSign( eymd );
	}
}


//-----------------------------------------------------------------------------
// DBSyz26/31切り替え？
//-----------------------------------------------------------------------------
// 引数		newver	：	変更後のバージョン（切り替えが必要な場合のみセット）
//-----------------------------------------------------------------------------
// 返送値	1	：	切り替えが必要
//			0	：	　　　　　不要
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::CheckKaiseiVER(unsigned char& newver)
{
	int seireki;

	if (!IsHistoryMode()) {
		if (curset.IsTyukanShinkoku()) {
			TimePair t1 = GetCurrentTyukanKikan();
			seireki = CTimeControl::GetIntTime(t1.second);
		}
		if (curset.IsKakuteiShinkoku()) {
			TimePair t1 = GetCurrentKessanKikan();
			seireki = CTimeControl::GetIntTime(t1.second);
		}
	}
	else {
		CHistoryReport::RecType rec = HistoryVec[GetHistoryCurSel()];
		if (rec.GetSnset().IsTyukanShinkoku()) {
			TimePair t1 = rec.GetSnset().GetSetTyukanKikan();
			seireki = CTimeControl::GetIntTime(t1.second);
		}
		if (rec.GetSnset().IsKakuteiShinkoku()) {
			TimePair t1 = rec.GetSnset().GetSetKessanKikan();
			seireki = CTimeControl::GetIntTime(t1.second);
		}
	}

	int	Kver = GetKaiseiVer();
//	if( (m_pMainFrame->m_nowTabver == ID_VER_SYZ26) && (seireki >= ICS_SH_10PER_REFORM_DAY) ) {
	if ((Kver == ID_VER_SYZ26) && (seireki >= ICS_SH_10PER_REFORM_DAY)) {
		newver = ID_VER_SYZ31;
		return 1;
	}

//	if( (m_pMainFrame->m_nowTabver == ID_VER_SYZ31) && (seireki < ICS_SH_10PER_REFORM_DAY) ) {
	if ((Kver == ID_VER_SYZ31) && (seireki < ICS_SH_10PER_REFORM_DAY)) {
		newver = ID_VER_SYZ26;
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特例計算チェックの状態をセット
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetTkreiChk()
{
	// 20231106 kasai
	// 修正依頼168266で履歴選択中かを見るように修正した。
	// 他にも同じような箇所あるが、管理課アップが近いため一旦ここだけ修正する。
	// GetSelectKazeiKikan()の処理を以下のように変更して、それを呼び出す形で問題ないか確認する。元々呼び出している箇所も要確認。
	long	eymd , symd = 0;
	if (!IsHistoryMode()) {
		if (curset.IsTyukanShinkoku()) {
			TimePair t1 = GetCurrentTyukanKikan();
			eymd = CTimeControl::GetIntTime(t1.second);
			symd = CTimeControl::GetIntTime(t1.first);
		}
		else if (curset.IsKakuteiShinkoku()) {
			TimePair t1 = GetCurrentKessanKikan();
			eymd = CTimeControl::GetIntTime(t1.second);
			symd = CTimeControl::GetIntTime(t1.first);
		}
	}
	else {
		CHistoryReport::RecType rec = HistoryVec[GetHistoryCurSel()];
		if (rec.GetSnset().IsTyukanShinkoku()) {
			TimePair t1 = rec.GetSnset().GetSetTyukanKikan();
			eymd = CTimeControl::GetIntTime(t1.second);
			symd = CTimeControl::GetIntTime(t1.first);
		}
		if (rec.GetSnset().IsKakuteiShinkoku()) {
			TimePair t1 = rec.GetSnset().GetSetKessanKikan();
			eymd = CTimeControl::GetIntTime(t1.second);
			symd = CTimeControl::GetIntTime(t1.first);
		}
	}
	if (eymd == 0) {
		return;
	}

	if (m_pZmSub == NULL) {
		return;
	}
	else if (m_pZmSub->zvol == NULL) {
		return;
	}

	if (symd > SIIRETOKUREI_EDAY) {
		m_pSnHeadData->m_DispTabSgn &= ~0x40;
		curset.m_DispTabSgn &= ~0x40;
	}

	if (symd > URIAGETOKUREI_EDAY) {	// 20231025 9月30日をまたぐ間は適用可能なため、開始日が超えているかどうかで判定する
	//if (eymd > URIAGETOKUREI_EDAY) {	// 20231025 del
		m_pSnHeadData->m_DispTabSgn &= ~0x70;
		curset.m_DispTabSgn &= ~0x70;
	}

	if( eymd < TOKUREI_KIKAN_S || symd > TOKUREI_KIKAN_E ){	// 20231025 9月30日をまたぐ間は適用可能なため、開始日が超えているかどうかで判定する
	//if( eymd > TOKUREI_KIKAN_E ){	// 20231025 del
		m_pSnHeadData->m_DispTabSgn &= ~0x80;
		curset.m_DispTabSgn &= ~0x80;
		m_Check7.EnableWindow(FALSE);
		m_Check7.SetCheck(BST_UNCHECKED);
		m_IsAbleTkrei = FALSE;
		m_btn1.ShowWindow(SW_HIDE);
		return;
	}

	// 令和5年10月1日以降開始の課税期間の場合は無効にする
	if( symd >= ICS_SH_INVOICE_DAY ){
		m_pSnHeadData->m_DispTabSgn &= ~0x80;
		curset.m_DispTabSgn &= ~0x80;
		m_Check7.EnableWindow(FALSE);
		m_Check7.SetCheck(BST_UNCHECKED);
		m_IsAbleTkrei = FALSE;
		m_btn1.ShowWindow(SW_HIDE);
		return;		
	}

	//----> 20231025
	if( IsCrossOverR050930( symd, eymd ) && !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
		m_pSnHeadData->m_DispTabSgn &= ~0x80;
		curset.m_DispTabSgn &= ~0x80;
		m_Check7.SetCheck(BST_UNCHECKED);
		m_IsAbleTkrei = FALSE;
		return;
	}

	if( curset.m_DispTabSgn & 0x2000 ){
		m_pSnHeadData->m_DispTabSgn &= ~0x80;
		curset.m_DispTabSgn &= ~0x80;
		m_Check7.EnableWindow(FALSE);
		m_Check7.SetCheck(BST_UNCHECKED);
		m_IsAbleTkrei = FALSE;
		return;
	}
	//<---- 20231025

	//チェック自体を無効にと思ったが、有効状態にし、
	//合併マスターの場合、特例選択は可能だが、特例画面は常に非連動とし、集計ボタンを無効とさせるyoshida190728
//	if (m_pZmSub->zvol->m_type == 0x01) {
//		SetTkreiChk(0);
//		m_Check7.EnableWindow(FALSE);
//		m_IsAbleTkrei = FALSE;
//		return;
//	}

	//  連動時、合併マスターの場合は特例を選択できないようにする
	if( m_pZmSub->zvol->m_type & 0x01 ){
		if ( !(m_pSnHeadData->Sn_Sign4 & 0x80) ) {
			m_Check7.EnableWindow(FALSE);
			m_btn1.ShowWindow(SW_HIDE);
			m_IsAbleTkrei = FALSE;
			m_pSnHeadData->m_DispTabSgn &= ~0x80;
			curset.m_DispTabSgn &= ~0x80;

			//---->yoshida190926
			if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {//一括税抜
				m_Check1.EnableWindow(TRUE);
				m_Check2.EnableWindow(TRUE);
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
					m_Check2.EnableWindow(FALSE);
				}
			}

			if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {
				m_Check3.EnableWindow(TRUE);
			}
			//<------

			return;
		}
		else {
			m_Check7.EnableWindow(TRUE);
			m_IsAbleTkrei = TRUE;
			return;
		}
	}


	bool bIsAbleTkrei = false;
	if ( eymd >= ICS_SH_10PER_REFORM_DAY) {
		bIsAbleTkrei = true;
	}

	if (m_IsAbleTkrei && (bIsAbleTkrei == false)) {
		SetTkreiChk(0);
		m_Check7.EnableWindow(FALSE);
		m_IsAbleTkrei = FALSE;

	}
	else if (!m_IsAbleTkrei && (bIsAbleTkrei == true)) {
		m_Check7.EnableWindow(TRUE);
		m_IsAbleTkrei = TRUE;

	}

	//---->yoshida190926
	//連動の場合のみ
	if (!(m_pSnHeadData->Sn_Sign4 & 0x80)) {
		if ((m_pSnHeadData->m_DispTabSgn & 0x80)) {
			//積上げ計算及び簡易計算のチェックボックスを無効化する。特例使用時は選択不可に
			//簡易計算OFF
			if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {//非連動ｄｂじゃない時。非連動ｄｂでは無効化されてる
				m_Check3.EnableWindow(FALSE);

				m_SimpSw = 0;
				curset.Sn_SPECIAL &= 0xfe;
				m_pSnHeadData->Sn_SPECIAL &= 0xfe;

				m_Check3.SetCheck(m_SimpSw);
			}

			//積上げ計算OFF
			if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {

				m_Check1.EnableWindow(FALSE);
				m_Check2.EnableWindow(FALSE);

				m_K221sw = m_H301sw = 0;
				curset.Sn_TUMIAG &= 0xfc;
				m_pSnHeadData->Sn_TUMIAG &= 0xfc;
				m_Check1.SetCheck(m_K221sw);
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
					// 法第２０条第１項の適用
					m_Check2.SetCheck(m_H301sw);
				}
			}
		}
		else {

			if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {
				m_Check1.EnableWindow(TRUE);
				m_Check2.EnableWindow(TRUE);
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
					m_Check2.EnableWindow(FALSE);
				}
			}

			if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {
				m_Check3.EnableWindow(TRUE);
			}
		}
	}
	else {//非連動は今まで通り
		if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {
			m_Check1.EnableWindow(TRUE);
			m_Check2.EnableWindow(TRUE);
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
				m_Check2.EnableWindow(FALSE);
			}
		}

		if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {
			m_Check3.EnableWindow(TRUE);
		}

	}
	//<------


	//yoshida190725
	//原則課税か簡易課税かを確認し、関係ないチェックがついてればここで落とす。
	//例えば原則で５－（２）号チェックし、簡易に変えたときにチェック残るなど
	if ((m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01) {//簡易課税
		//５－（２）号落とす
		curset.m_DispTabSgn &= ~0x20;
		m_pSnHeadData->m_DispTabSgn &= ~0x20;

		//５－（３）号落とす
		curset.m_DispTabSgn &= ~0x40;
		m_pSnHeadData->m_DispTabSgn &= ~0x40;

		//全てのサイン落ちてる場合には、本体チェックも外す
		if ((!(m_pSnHeadData->m_DispTabSgn & 0x10)) && (!(m_pSnHeadData->m_DispTabSgn & 0x20)) && (!(m_pSnHeadData->m_DispTabSgn & 0x40))) {
			curset.m_DispTabSgn &= ~0x80;
			m_pSnHeadData->m_DispTabSgn &= ~0x80;
			m_Check7.SetCheck(BST_UNCHECKED);
			m_btn1.ShowWindow(SW_HIDE);
		}
	}
	else {//原則課税　yoshida190911
		//「仕入税額の按分法が個別対応」かつ「連動」なら、５－（３）は無効
		if ((m_pSnHeadData->m_s_sgn4 & 0x80) && !(m_pSnHeadData->Sn_Sign4 & 0x80)) {
			//５－（３）号落とす
			curset.m_DispTabSgn &= ~0x40;
			m_pSnHeadData->m_DispTabSgn &= ~0x40;
		}

		//全てのサイン落ちてる場合には、本体チェックも外す
		if ((!(m_pSnHeadData->m_DispTabSgn & 0x10)) && (!(m_pSnHeadData->m_DispTabSgn & 0x20)) && (!(m_pSnHeadData->m_DispTabSgn & 0x40))) {
			curset.m_DispTabSgn &= ~0x80;
			m_pSnHeadData->m_DispTabSgn &= ~0x80;
			m_Check7.SetCheck(BST_UNCHECKED);
			m_btn1.ShowWindow(SW_HIDE);
		}
	}
}

//-----------------------------------------------------------------------------
// 特定課税仕入有りチェックの状態をセット('15.09.09)
//-----------------------------------------------------------------------------
// 引数	onoff	：	0:オフ, 1:オン
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetTkreiChk(int onoff)
{
	if (onoff == 0) {
		m_Check7.SetCheck(BST_UNCHECKED);
		m_btn1.ShowWindow(SW_HIDE);

		curset.m_DispTabSgn &= ~0x80;
		m_pSnHeadData->m_DispTabSgn &= ~0x80;

	}
	else if (onoff == 1) {
		m_Check7.SetCheck(BST_CHECKED);
		m_btn1.ShowWindow(SW_SHOW);

		curset.m_DispTabSgn |= 0x80;
		m_pSnHeadData->m_DispTabSgn |= 0x80;
	}
}

//-----------------------------------------------------------------------------
// ８％データの削除
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::DeleteH26LstVal()
{
	CDatabase* db = m_pZmSub->m_database;

	db->BeginTrans();
	try{
		db->ExecuteSQL(GetQueryDropOnlyH26("sn_seq", "SYZ_SNK_LST_VAL", "version"));	// 消費税申告書(金額)
		db->ExecuteSQL(GetQueryDropOnlyH26("sn_seq", "SYZ_SNK_LST_RATE", "version"));	// 消費税申告書(割合)
	}
	catch( CDBException *pDbe ){
		pDbe->Delete();
		db->Rollback();
		return -1;
	}
	db->CommitTrans();

	return 0;
}


//=============================================================================
// 令和2年4月1日改正様式を使用するのチェックの状態をセット
//=============================================================================
void CDBSyzShinMainView::SetR02Chk()
{
	long eymd = 0;
	if ( curset.IsTyukanShinkoku() ) {
		TimePair t1 = GetCurrentTyukanKikan();
		eymd = CTimeControl::GetIntTime ( t1.second );
	}
	else if ( curset.IsKakuteiShinkoku() ) {
		TimePair t1 = GetCurrentKessanKikan();
		eymd = CTimeControl::GetIntTime ( t1.second );
	}
	if ( eymd == 0 ) {
		return;
	}
	// 令和3年1月1日以降の課税期間では、令和2年4月1日改正様式を使用する
	else if ( eymd >= R03_SDAY ) {
		SetR02Chk ( 1 );
		m_Check9.EnableWindow ( FALSE );
		SetR02Sign ( eymd );
		return;
	}

	if ( m_pZmSub == NULL ) {
		return;
	}
	else if ( m_pZmSub->zvol == NULL ) {
		return;
	}

	bool bisAbleR02Reform = false;
	EnumIdIcsShKazeihoushiki kzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );
	if ( kzHoushiki == ID_ICSSH_GENSOKU ) {
		if ( eymd >= ICS_SH_KYOJU_REFORM_DAY ) {
			bisAbleR02Reform = true;
		}
	}
	else {
		if ( eymd >= ICS_SH_10PER_REFORM_DAY) {
			bisAbleR02Reform = true;
		}
	}


	if ( m_IsAbleR02Reform && ( bisAbleR02Reform == false ) ) {
		SetR02Chk ( 0 );
		m_Check9.EnableWindow ( FALSE );
		SetR02Sign ( eymd );
	}
	else if ( !m_IsAbleR02Reform && ( bisAbleR02Reform == true ) ) {
		m_Check9.EnableWindow( TRUE );
		SetR02Sign ( eymd );
	}
}

//=============================================================================
// 令和2年4月1日改正様式を使用するのチェックの状態をセット
//-----------------------------------------------------------------------------
// 引数
//		onoff		:		0 = off, 1 = on
//=============================================================================
void CDBSyzShinMainView::SetR02Chk ( int onoff )
{
	if ( onoff == 0 ) {
		m_Check9.SetCheck ( BST_UNCHECKED );
		curset.m_DispTabSgn &= ~0x1000;
		//m_pSnHeadData->m_DispTabSgn &= ~0x1000;

	}
	else if ( onoff == 1 ) {
		m_Check9.SetCheck ( BST_CHECKED );
		curset.m_DispTabSgn |= 0x1000;
		//m_pSnHeadData->m_DispTabSgn |= 0x1000;
	}
}

//=============================================================================
// 令和2年4月1日改正様式を使用するのサインセット
//-----------------------------------------------------------------------------
// 引数
//		eymd		:		末日
//=============================================================================
void CDBSyzShinMainView::SetR02Sign( long eymd )
{
	ASSERT ( m_pZmSub );
	ASSERT ( m_pZmSub->zvol );

	// etaxが令和3年1月4日以降に決定
	// 終了日はetaxリリース時に指定
	if ( eymd >= R03_SDAY ) {
		m_IsAbleR02Reform = FALSE;
	}
	else {
		if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
			if ( eymd >= ICS_SH_KYOJU_REFORM_DAY ) {
				m_IsAbleR02Reform = TRUE;
			}
			else {
				m_IsAbleR02Reform = FALSE;
			}
		}
		else {
			if ( eymd >= ICS_SH_10PER_REFORM_DAY ) {
				m_IsAbleR02Reform = TRUE;
			}
			else {
				m_IsAbleR02Reform = FALSE;
			}
		}
	}
}

//yoshida190728
//-----------------------------------------------------------------------------
// 特定収入計算表未対応。期間を見て対象期間の場合処理制御
//-----------------------------------------------------------------------------
// 引数		snset	：	チェック対象情報
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	申告不可期間
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsSpc31OutKikan(CSnset &snset)
{
	BOOL	bRt = TRUE;

	TimePair t1;
	if (snset.IsTyukanShinkoku()) {
		t1 = snset.GetSetTyukanKikan();
	}
	else if (snset.IsKakuteiShinkoku()) {
		t1 = snset.GetSetKessanKikan();
	}
	else {
		return bRt;
	}

	CString	strEmd = t1.second.Format("%Y%m%d");

	int	inEmd = atoi(strEmd);

	if (ICS_SH_10PER_REFORM_DAY > inEmd) {
		bRt = FALSE;
	}

	return bRt;
}

//=====================================================================
// 課税期間の取得
//---------------------------------------------------------------------
// 引数
//		symd		:		取得した課税期間　自
//		eymd		:		取得した課税期間　至
//=====================================================================
void CDBSyzShinMainView::GetKazeiKikan( long &symd, long &eymd )
{
	// 初期化
	symd = 0;
	eymd = 0;

	if( m_pMainFrame ) {
		m_pMainFrame->GetKazeiKikan( symd, eymd );
	}
}

//=====================================================================
// 課税期間の取得（23,8）
//---------------------------------------------------------------------
// 引数
//		symd		:		取得した課税期間　自
//		eymd		:		取得した課税期間　至
//=====================================================================
void CDBSyzShinMainView::GetKazeiKikanfromSnset(CSnset &snset, int &symd, int &eymd)
{
	symd = 0;
	eymd = 0;

	TimePair t1;

	CString	strSmd;
	CString	strEmd;

	if (IsHistoryMode()) { //履歴選択時
		if (snset.IsTyukanShinkoku()) {
			t1 = snset.GetSetTyukanKikan();
		}
		//else if (snset.Sn_KZTNSK) {//期間短縮
		else if (snset.IsKakuteiShinkoku()) {
			t1 = snset.GetSetKessanKikan(); 
		}
		else {
			return;
		}
	}
	else {
		if (snset.IsTyukanShinkoku()) {
			t1 = GetCurrentTyukanKikan();
		}
		//else if (snset.Sn_KZTNSK) {//期間短縮
		else if (snset.IsKakuteiShinkoku()) {
			t1 = GetCurrentKessanKikan();
		}
		else {
			return;
		}
	}

	strSmd = t1.first.Format("%Y%m%d");
	strEmd = t1.second.Format("%Y%m%d");
	symd = atoi(strSmd);
	eymd = atoi(strEmd);
}

//============================================================================
// 仕訳に紐づいている登録番号のチェック
//----------------------------------------------------------------------------
// 引数
//			errMsg			:			エラーメッセージ
//----------------------------------------------------------------------------
// 返送値
//			0				：			インボイス登録番号データ不正なし
//			1				：			インボイス登録番号データ不正あり
//		   -1				：			エラー
//============================================================================
int CDBSyzShinMainView::CheckWrongInvoiceData( CString &errMsg )
{
	int ret = 0;
	int symd = 0, eymd = 0;

	errMsg.Empty();

	// 非連動・入力ロック中はチェックしない
	if(( curset.Sn_Sign4 & 0x80 ) || ( !CheckChange() && InLockInput() )) {
		return 0;
	}

	// 予定申告の場合もチェックしない
	if( curset.GetKubun() == yotei ) {
		return 0;
	}

	// 会社登録で登録番号を表示しないに設定している場合もチェックしない
	CZmGen8 zmGen;
	if( !zmGen.IsDispInvno( m_pZmSub )) {
		return 0;
	}

	// 簡易課税もしくは２割特例の場合はチェックしない
	// 仕入を集計しないため
	if(( m_Util.GetKazeihoushiki( m_pZmSub->zvol ) != ID_ICSSH_GENSOKU ) || ( curset.m_DispTabSgn & 0x2000 )) {
		return 0;
	}

	// 課税期間取得
	GetKazeiKikanfromSnset( curset, symd, eymd );

	if( eymd >= ICS_SH_INVOICE_DAY ) {
		// チェック！
		CSyzMakeSub syzMakeSub;
		SYZ_WrongInvoiceDataList wrongData;
		ret = syzMakeSub.CheckWrongInvoiceData( m_pZmSub, symd, eymd, wrongData, this );
		if( ret == -1 ) {
			errMsg = syzMakeSub.GetErrorMsg();
			return -1;
		}
		else if( ret == 1 ) {
			bool bNetError = false;
			bool bKojin = false;

			if( m_pSnHeadData->IsSoleProprietor() ) {
				bKojin = true;
			}

			for( int i = 0; i < wrongData.CntNG; i++ ) {
				if( bKojin ) {
					if( wrongData.WrongData[i].errStatusKojin == -4 ) {
						bNetError = true;
						break;
					}
				}
				else {
					if( wrongData.WrongData[i].errStatusHojin == -4 ) {
						bNetError = true;
						break;
					}
				}
			}

			if( bNetError ) {
				errMsg.Format( _T( "インターネットに接続されていないため、登録番号の確認が行えません。\nネットワーク回線等を確認してください。\n処理を継続しますか？" ));
			}
			else {
				errMsg.Format( _T( "入力された仕訳に不正な登録番号が存在します。\n入力済み仕訳を確認してください。\n処理を継続しますか？" ));
			}
		}
	}
	else {
		// 令和5年10月1日より前の課税期間の場合はチェックしない
		ret = 0;
	}

	return ret;
}