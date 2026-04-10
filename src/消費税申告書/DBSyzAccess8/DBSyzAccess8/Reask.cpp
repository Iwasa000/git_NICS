// \‘@X³‚Ì¿‹
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CReask

IMPLEMENT_DYNAMIC(CReask, CRecordset)

CReask::CReask( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CReask)

	pr.KStype = 0;
	pr.KSdata = 0;		// ’ño”NŒŽ“ú										(•½¬ŒÅ’è)
	pr.KSktdt = 0;		// X³Œˆ’è’Ê’m‘“ú•t								(•½¬ŒÅ’è)
	pr.KSktsw = 0;		// 0=\A1=X³A2=Œˆ’è
	pr.KSstdt = 0;		// C³\’ño”NŒŽ“ú(–”‚ÍX³Œˆ’è’Ê’m‘Žó—”NŒŽ“ú)	(•½¬ŒÅ’è)
	pr.KSjrdt = 0;		// X³Œˆ’è’Ê’m‘Žó—”NŒŽ“ú							(•½¬ŒÅ’è)

	pr.KSreas1 = "";	// X³‚Ì¿‹‚ð‚·‚é——R“™(40) ‚Ps–Ú
	pr.KSreas2 = "";	// V						  ‚Qs–Ú
	pr.KSreas3 = "";	// V						  ‚Rs–Ú
	pr.KSreas4 = "";	// V						  —\@”õ
	pr.KSdocu1 = "";	// “Y•t‘—Þ(15)	‚Ps–Ú
	pr.KSdocu2 = "";	// V			‚Qs–Ú
	pr.KSdocu3 = "";	// V			—\@”õ
	pr.KVvflg1 = 0;		//  1 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval1 = "";	//  1 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval1 = "";	//  1 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg2 = 0;		//  2 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval2 = "";	//  2 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval2 = "";	//  2 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg3 = 0;		//  3 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval3 = "";	//  3 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval3 = "";	//  3 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg4 = 0;		//  4 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval4 = "";	//  4 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval4 = "";	//  4 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg5 = 0;		//  5 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval5 = "";	//  5 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval5 = "";	//  5 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg6 = 0;		//  6 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval6 = "";	//  6 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval6 = "";	//  6 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg7 = 0;		//  7 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval7 = "";	//  7 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval7 = "";	//  7 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg8 = 0;		//  8 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval8 = "";	//  8 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval8 = "";	//  8 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg9 = 0;		//  9 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval9 = "";	//  9 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval9 = "";	//  9 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg10 = 0;	// 10 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval10 = "";	// 10 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval10 = "";	// 10 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg11 = 0;	// 11 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval11 = "";	// 11 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval11 = "";	// 11 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg12 = 0;	// 12 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval12 = "";	// 12 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval12 = "";	// 12 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg13 = 0;	// 13 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval13 = "";	// 13 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval13 = "";	// 13 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg14 = 0;;	// 14 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval14 = "";	// 14 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval14 = "";	// 14 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg15 = 0;	// 15 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval15 = "";	// 15 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval15 = "";	// 15 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg16 = 0;	// 16 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval16 = "";	// 16 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval16 = "";	// 16 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg17 = 0;	// 17 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval17 = "";	// 17 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval17 = "";	// 17 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg18 = 0;	// 18 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval18 = "";	// 18 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval18 = "";	// 18 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg19 = 0;	// 19 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval19 = "";	// 19 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval19 = "";	// 19 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.KVvflg20 = 0;	// 20 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	pr.KVsval20 = "";	// 20 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	pr.KVkval20 = "";	// 20 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	pr.Bname = "";		// ‹âs“™–¼Ì	(40)
	pr.Bkbn = 0;		// ‹âs“™‹æ•ª(0:‹âs/1:‹àŒÉ/2:‘g‡/3:‹™‹¦/4:”_‹¦)
	pr.Sname = "";		// –{Žx“X–¼Ì	(40)
	pr.Skbn = 0;		// –{Žx“X‹æ•ª(1:–{“X/2:Žx“X/3:–{Š/4:ŽxŠ)
	pr.Dname = "",		// —a‹à			(30)
	pr.Accnt = "";		// ŒûÀ”Ô†		(20)
	pr.Pname = "";		// —X•Ö‹Ç–¼Ì	(50)
	pr.Acnt1 = "";		// ŒûÀ”Ô†‚P	(20)
	pr.Acnt2 = "";		// ŒûÀ”Ô†‚Q	(20)
	pr.KSbpsw = 0;		// ‹âs–”‚Í—X•Ö‹Ç‘I‘ðƒTƒCƒ“
	pr.KSthsw = 0;		// “K—p–@—ß‚Ì‹æ•ªƒTƒCƒ“
	pr.KSkksw = 0;		// Šm’èŠz‹æ•ªƒTƒCƒ“ƒTƒCƒ“
	pr.Kikakutei1 = "";	// 13 ‚±‚Ì¿‹‘O‚ÌŠùŠm’èÅŠz@X³‚Ì¿‹/V—lŽ®‘Î‰ž
	pr.Kikakutei2 = "";	// 21 ‚±‚Ì¿‹‘O‚ÌŠùŠm’è÷“nŠ„Šz@X³‚Ì¿‹/V—lŽ®‘Î‰ž
	m_nFields = 87;		// 85->87@Kikakutei1/Kikakutei2’Ç‰Á‚Ì‚½‚ß
	//}}AFX_FIELD_INIT

//	m_pCDBPrtblW = NULL;
}


void CReask::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CReask)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX, _T("KStype"), pr.KStype );
	RFX_Long( pFX, _T("KSdata"), pr.KSdata );						// ’ño”NŒŽ“ú										(•½¬ŒÅ’è)
	RFX_Long( pFX, _T("KSktdt"), pr.KSktdt );						// X³Œˆ’è’Ê’m‘“ú•t								(•½¬ŒÅ’è)
	RFX_Int( pFX, _T("KSktsw"), pr.KSktsw );						// 0=\A1=X³A2=Œˆ’è
	RFX_Long( pFX, _T("KSstdt"), pr.KSstdt );						// C³\’ño”NŒŽ“ú(–”‚ÍX³Œˆ’è’Ê’m‘Žó—”NŒŽ“ú)	(•½¬ŒÅ’è)
	RFX_Long( pFX, _T("KSjrdt"), pr.KSjrdt );						// X³Œˆ’è’Ê’m‘Žó—”NŒŽ“ú							(•½¬ŒÅ’è)

	RFX_Text( pFX, _T("KSreas1"), pr.KSreas1 );						// X³‚Ì¿‹‚ð‚·‚é——R“™(40) ‚Ps–Ú
	RFX_Text( pFX, _T("KSreas2"), pr.KSreas2 );						// V						  ‚Qs–Ú
	RFX_Text( pFX, _T("KSreas3"), pr.KSreas3 );						// V						  ‚Rs–Ú
	RFX_Text( pFX, _T("KSreas4"), pr.KSreas4 );						// V						  —\@”õ
	RFX_Text( pFX, _T("KSdocu1"), pr.KSdocu1 );						// “Y•t‘—Þ(15)	‚Ps–Ú
	RFX_Text( pFX, _T("KSdocu2"), pr.KSdocu2 );						// V			‚Qs–Ú
	RFX_Text( pFX, _T("KSdocu3"), pr.KSdocu3 );						// V			—\@”õ
	RFX_Int( pFX, _T("KVvflg1"),  pr.KVvflg1 );						//  1 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval1"), pr.KVsval1, 16, SQL_DECIMAL );	//  1 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval1"), pr.KVkval1, 16, SQL_DECIMAL );	//  1 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg2"),  pr.KVvflg2 );						//  2 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval2"), pr.KVsval2, 16, SQL_DECIMAL );	//  2 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval2"), pr.KVkval2, 16, SQL_DECIMAL );	//  2 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg3"),  pr.KVvflg3 );						//  3 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval3"), pr.KVsval3, 16, SQL_DECIMAL );	//  3 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval3"), pr.KVkval3, 16, SQL_DECIMAL );	//  3 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg4"),  pr.KVvflg4 );						//  4 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval4"), pr.KVsval4, 16, SQL_DECIMAL );	//  4 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval4"), pr.KVkval4, 16, SQL_DECIMAL );	//  4 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg5"),  pr.KVvflg5 );						//  5 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval5"), pr.KVsval5, 16, SQL_DECIMAL );	//  5 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval5"), pr.KVkval5, 16, SQL_DECIMAL );	//  5 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg6"),  pr.KVvflg6 );						//  6 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval6"), pr.KVsval6, 16, SQL_DECIMAL );	//  6 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval6"), pr.KVkval6, 16, SQL_DECIMAL );	//  6 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg7"),  pr.KVvflg7 );						//  7 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval7"), pr.KVsval7, 16, SQL_DECIMAL );	//  7 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval7"), pr.KVkval7, 16, SQL_DECIMAL );	//  7 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg8"),  pr.KVvflg8 );						//  8 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval8"), pr.KVsval8, 16, SQL_DECIMAL );	//  8 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval8"), pr.KVkval8, 16, SQL_DECIMAL );	//  8 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg9"),  pr.KVvflg9 );						//  9 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval9"), pr.KVsval9, 16, SQL_DECIMAL );	//  9 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval9"), pr.KVkval9, 16, SQL_DECIMAL );	//  9 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg10"),  pr.KVvflg10 );					// 10 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval10"), pr.KVsval10, 16, SQL_DECIMAL );	// 10 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval10"), pr.KVkval10, 16, SQL_DECIMAL );	// 10 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg11"),  pr.KVvflg11 );					// 11 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval11"), pr.KVsval11, 16, SQL_DECIMAL );	// 11 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval11"), pr.KVkval11, 16, SQL_DECIMAL );	// 11 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg12"),  pr.KVvflg12 );					// 12 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval12"), pr.KVsval12, 16, SQL_DECIMAL );	// 12 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval12"), pr.KVkval12, 16, SQL_DECIMAL );	// 12 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg13"),  pr.KVvflg13 );					// 13 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval13"), pr.KVsval13, 16, SQL_DECIMAL );	// 13 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval13"), pr.KVkval13, 16, SQL_DECIMAL );	// 13 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg14"),  pr.KVvflg14 );					// 14 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval14"), pr.KVsval14, 16, SQL_DECIMAL );	// 14 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval14"), pr.KVkval14, 16, SQL_DECIMAL );	// 14 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg15"),  pr.KVvflg15 );					// 15 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval15"), pr.KVsval15, 16, SQL_DECIMAL );	// 15 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval15"), pr.KVkval15, 16, SQL_DECIMAL );	// 15 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg16"),  pr.KVvflg16 );					// 16 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval16"), pr.KVsval16, 16, SQL_DECIMAL );	// 16 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval16"), pr.KVkval16, 16, SQL_DECIMAL );	// 16 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg17"),  pr.KVvflg17 );					// 17 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval17"), pr.KVsval17, 16, SQL_DECIMAL );	// 17 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval17"), pr.KVkval17, 16, SQL_DECIMAL );	// 17 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg18"),  pr.KVvflg18 );					// 18 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval18"), pr.KVsval18, 16, SQL_DECIMAL );	// 18 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval18"), pr.KVkval18, 16, SQL_DECIMAL );	// 18 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg19"),  pr.KVvflg19 );					// 19 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval19"), pr.KVsval19, 16, SQL_DECIMAL );	// 19 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval19"), pr.KVkval19, 16, SQL_DECIMAL );	// 19 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Int( pFX, _T("KVvflg20"),  pr.KVvflg20 );					// 20 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
	RFX_Text( pFX, _T("KVsval20"), pr.KVsval20, 16, SQL_DECIMAL );	// 20 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
	RFX_Text( pFX, _T("KVkval20"), pr.KVkval20, 16, SQL_DECIMAL );	// 20 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
	RFX_Text( pFX, _T("Bname"), pr.Bname );							// ‹âs“™–¼Ì	(40)
	RFX_Int( pFX, _T("Bkbn"),	pr.Bkbn );							// ‹âs“™‹æ•ª(0:‹âs/1:‹àŒÉ/2:‘g‡/3:‹™‹¦/4:”_‹¦)
	RFX_Text( pFX, _T("Sname"), pr.Sname );							// –{Žx“X–¼Ì	(40)
	RFX_Int( pFX, _T("Skbn"),	pr.Skbn );							// –{Žx“X‹æ•ª(1:–{“X/2:Žx“X/3:–{Š/4:ŽxŠ)
	RFX_Text( pFX, _T("Dname"),	pr.Dname );							// —a‹à			(30)
	RFX_Text( pFX, _T("Accnt"), pr.Accnt );							// ŒûÀ”Ô†		(20)
	RFX_Text( pFX, _T("Pname"), pr.Pname );							// —X•Ö‹Ç–¼Ì	(50)
	RFX_Text( pFX, _T("Acnt1"), pr.Acnt1 );							// ŒûÀ”Ô†‚P	(20)
	RFX_Text( pFX, _T("Acnt2"), pr.Acnt2 );							// ŒûÀ”Ô†‚Q	(20)
	RFX_Int( pFX, _T("KSbpsw"),	pr.KSbpsw );						// ‹âs–”‚Í—X•Ö‹Ç‘I‘ðƒTƒCƒ“
	RFX_Int( pFX, _T("KSthsw"),	pr.KSthsw );						// “K—p–@—ß‚Ì‹æ•ªƒTƒCƒ“
	RFX_Int( pFX, _T("KSkksw"),	pr.KSkksw );						// Šm’èŠz‹æ•ªƒTƒCƒ“
	RFX_Text( pFX, _T("Kikakutei1"), pr.Kikakutei1, 16, SQL_DECIMAL );	// 13 ‚±‚Ì¿‹‘O‚ÌŠùŠm’èÅŠz@X³‚Ì¿‹/V—lŽ®‘Î‰ž
	RFX_Text( pFX, _T("Kikakutei2"), pr.Kikakutei2, 16, SQL_DECIMAL );	// 21 ‚±‚Ì¿‹‘O‚ÌŠùŠm’è÷“nŠ„Šz@X³‚Ì¿‹/V—lŽ®‘Î‰ž
	//}}AFX_FIELD_MAP
}				  
				  
CString CReask::GetDefaultSQL()
{				  
	return _T("reask");
}

/////////////////////////////////////////////////////////////////////////////
// CReask ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CReask::AssertValid() const
{
	CRecordset::AssertValid();
}

void CReask::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CREASK
CREASK::CREASK( CReask *preask )
{
	m_preask = preask;
	nodata = 0;
}

void CREASK::data()
{
	if( KStype == -1 ){	// ŒvŽZ•\@”Ô†
		st = -1;
		KSdata = 0;		// ’ño”NŒŽ“ú										(•½¬ŒÅ’è)
		KSktdt = 0;		// X³Œˆ’è’Ê’m‘“ú•t								(•½¬ŒÅ’è)
		KSktsw = 0;		// 0=\A1=X³A2=Œˆ’è
		KSstdt = 0;		// C³\’ño”NŒŽ“ú(–”‚ÍX³Œˆ’è’Ê’m‘Žó—”NŒŽ“ú)	(•½¬ŒÅ’è)
		KSjrdt = 0;		// X³Œˆ’è’Ê’m‘Žó—”NŒŽ“ú							(•½¬ŒÅ’è)

		KSreas1 = "";	// X³‚Ì¿‹‚ð‚·‚é——R“™(40) ‚Ps–Ú
		KSreas2 = "";	// V						  ‚Qs–Ú
		KSreas3 = "";	// V						  ‚Rs–Ú
		KSreas4 = "";	// V						  —\@”õ
		KSdocu1 = "";	// “Y•t‘—Þ(15)	‚Ps–Ú
		KSdocu2 = "";	// V			‚Qs–Ú
		KSdocu3 = "";	// V			—\@”õ
		KVvflg1 = 0;		//  1 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval1 = "";	//  1 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval1 = "";	//  1 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg2 = 0;		//  2 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval2 = "";	//  2 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval2 = "";	//  2 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg3 = 0;		//  3 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval3 = "";	//  3 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval3 = "";	//  3 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg4 = 0;		//  4 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval4 = "";	//  4 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval4 = "";	//  4 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg5 = 0;	//  5 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval5 = "";	//  5 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval5 = "";	//  5 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg6 = 0;	//  6 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval6 = "";	//  6 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval6 = "";	//  6 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg7 = 0;	//  7 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval7 = "";	//  7 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval7 = "";	//  7 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg8 = 0;	//  8 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval8 = "";	//  8 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval8 = "";	//  8 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg9 = 0;	//  9 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval9 = "";	//  9 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval9 = "";	//  9 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg10 = 0;	// 10 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval10 = "";	// 10 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval10 = "";	// 10 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg11 = 0;	// 11 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval11 = "";	// 11 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval11 = "";	// 11 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg12 = 0;	// 12 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval12 = "";	// 12 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval12 = "";	// 12 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg13 = 0;	// 13 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval13 = "";	// 13 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval13 = "";	// 13 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg14 = 0;;	// 14 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval14 = "";	// 14 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval14 = "";	// 14 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg15 = 0;	// 15 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval15 = "";	// 15 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval15 = "";	// 15 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg16 = 0;	// 16 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval16 = "";	// 16 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval16 = "";	// 16 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg17 = 0;	// 17 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval17 = "";	// 17 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval17 = "";	// 17 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg18 = 0;	// 18 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval18 = "";	// 18 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval18 = "";	// 18 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg19 = 0;	// 19 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval19 = "";	// 19 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval19 = "";	// 19 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		KVvflg20 = 0;	// 20 ŽÀŠzƒTƒCƒ“@D0:KVsval D1:KVkval
		KVsval20 = "";	// 20 ‚±‚Ì¿‹‘O‚Ì‹àŠz(Šm’èŠz)
		KVkval20 = "";	// 20 X³‚Ì¿‹‹àŠz(³“–‚Æ‚·‚éŠz)
		Bname = "";		// ‹âs“™–¼Ì	(40)
		Bkbn = 0;		// ‹âs“™‹æ•ª(0:‹âs/1:‹àŒÉ/2:‘g‡/3:‹™‹¦/4:”_‹¦)
		Sname = "";		// –{Žx“X–¼Ì	(40)
		Skbn = 0;		// –{Žx“X‹æ•ª(1:–{“X/2:Žx“X/3:–{Š/4:ŽxŠ)
		Dname = "",		// —a‹à			(30)
		Accnt = "";		// ŒûÀ”Ô†		(20)
		Pname = "";		// —X•Ö‹Ç–¼Ì	(50)
		Acnt1 = "";		// ŒûÀ”Ô†‚P	(20)
		Acnt2 = "";		// ŒûÀ”Ô†‚Q	(20)
		KSbpsw = 0;		// ‹âs–”‚Í—X•Ö‹Ç‘I‘ðƒTƒCƒ“
		KSthsw = 0;		// “K—p–@—ß‚Ì‹æ•ªƒTƒCƒ“
		KSkksw = 0;		// Šm’èŠz‹æ•ªƒTƒCƒ“
		Kikakutei1 = "";// 13 ‚±‚Ì¿‹‘O‚ÌŠùŠm’èÅŠz@X³‚Ì¿‹/V—lŽ®‘Î‰ž
		Kikakutei2 = "";// 21 ‚±‚Ì¿‹‘O‚ÌŠùŠm’è÷“nŠ„Šz@X³‚Ì¿‹/V—lŽ®‘Î‰ž
		return;
	}
	st = 0;
}

int CREASK::MoveFirst()
{
	KStype = -1;
	if( !nodata ){
		m_preask->MoveFirst();
	}
	data();
	return st;
}

int CREASK::MoveNext()
{
	KStype = -1;
	if( ! m_preask->IsEOF() ){
		m_preask->MoveNext();
	}
	data();
	return st;
}

int CREASK::MovePrev()
{
	KStype = -1;
	if( ! m_preask->IsBOF() ){
		m_preask->MovePrev();
	}
	data();
	return st;
}

int CREASK::MoveLast()
{
	KStype = -1;
	if( ! nodata ){
		m_preask->MoveLast();
	}
	data();
	return st;
}

int CREASK::Move( int nrows )
{
	KStype = -1;
	if( nrows > 0 ) {
		if( ! m_preask->IsEOF() ){
			m_preask->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_preask->IsBOF() ){
			m_preask->Move(nrows);
		}
	}
	else{
		m_preask->Move(nrows);
	}
	data();
	return st;
}

int CREASK::SetAbsolutePosition( int nrows )
{
	KStype = -1;
	if( ! nodata ){
		m_preask->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CREASK::Requery( CString filter )
{
	KStype = -1;
	m_preask->m_strFilter = filter;
	m_preask->m_strSort =  _T("kslist,KStype,ksitem");
	m_preask->Requery();
	if( m_preask->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CREASK::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_preask->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_preask->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_preask->MoveFirst();
		}
		for( ; ! m_preask->IsEOF(); ){
			m_preask->MoveNext();
		}
		if( cpos >= 0 ){
			m_preask->SetAbsolutePosition(cpos+1);
		}
		return( m_preask->GetRecordCount() );
	}
}

void CREASK::Edit()
{
	m_preask->m_pDatabase->BeginTrans();
	m_preask->Edit();
}

void CREASK::AddNew()
{
	m_preask->m_pDatabase->BeginTrans();
	m_preask->AddNew();
}

int CREASK::Update()
{
	try{
		st = m_preask->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_preask->Requery();
			if( m_preask->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_preask->m_pDatabase->Rollback();
//		if( m_preask->m_pCDBPrtblW ){
//			m_preask->m_pCDBPrtblW->SetErrMsg( DB_FATAL_ERROR, dbe->m_strError.GetBuffer(), dbe->m_nRetCode );
//		}
		dbe->Delete();
		return -1;
	}

	m_preask->m_pDatabase->CommitTrans();
	
	return st;
}

void CREASK::Delete()
{
	if( ! nodata ) {
		m_preask->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_preask->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_preask->Requery();
					if( m_preask->IsBOF() ) {
						m_preask->m_pDatabase->Rollback();
						return;
					}
					nodata=0;
					continue;
				}
				m_preask->m_pDatabase->Rollback();
//				if( m_preask->m_pCDBPrtblW ){
//					m_preask->m_pCDBPrtblW->SetErrMsg( DB_FATAL_ERROR, dbe->m_strError.GetBuffer(), dbe->m_nRetCode );
//				}
				dbe->Delete();
				return;
			}
			break;
		}
		m_preask->m_pDatabase->CommitTrans();
		m_preask->MoveNext();
	}
}
