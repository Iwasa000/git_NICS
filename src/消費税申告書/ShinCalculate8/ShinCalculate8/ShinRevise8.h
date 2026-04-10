
// ShinRevise.h
#ifndef  __ICS_SHINREVISE__
#define  __ICS_SHINREVISE__

// 改正チェック関数	財務クラス
__declspec(dllexport) int DBShinReviseCheck( int, long, long, CDBZmSub*, CString& );
// 改正チェック関数 非営利クラス
__declspec(dllexport) int NPShinReviseCheck( int, long, long, CDBNpSub*, CString& );
// シミュレーション集計　財務クラス
__declspec(dllexport) int DBShinReviseCalcurate( long, long, SUMINFO*, SUMLIST*, CDBZmSub*, CString& );
// シミュレーション集計　非営利クラス
__declspec(dllexport) int NPShinReviseCalcurate( long, long, SUMINFO*, SUMLIST*, CDBNpSub*, CString& );

#endif	//__ICS_SHINREVISE__ 