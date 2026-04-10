// CShinCalculate

#ifndef  __ICS_SHINCALQLATE__
#define  __ICS_SHINCALQLATE__

class CShinCalculateW;
class CShinCalculate
{
	CShinCalculateW	*m_pShincalculate;
public:
	__declspec(dllexport) CShinCalculate();
	__declspec(dllexport) virtual ~CShinCalculate();

	////////////////////////
	// 課税売上高チェック //
	////////////////////////
	// 財務クラス
	__declspec(dllexport) int DBShinCheck( int, long, long, CDBZmSub* );
	// 公益クラス
	__declspec(dllexport) int NPShinCheck( int, long, long, CDBNpSub* );
	// エラーメッセージの取得
	__declspec(dllexport) CString& ShinCheckErrMsg( void );
	// チェックメッセージの取得
	__declspec(dllexport) CString& ShinCheckGetMsg( void );

	////////////////////////
	// 消費税申告書　集計 //
	////////////////////////
	// 財務クラス
	__declspec(dllexport) int DBShinCalculate( long, long, SUMINFO*, SUMLIST*, CDBZmSub* );
	// 公益クラス
	__declspec(dllexport) int NPShinCalculate( long, long, SUMINFO*, SUMLIST*, CDBNpSub* );
	// エラーメッセージの取得
	__declspec(dllexport) CString& ShinCalculateErrMsg( void );

	char	Rsv1[512];

private:

	char	Rsv2[256];
};

#endif			// end of __ICS_SHINCALQLATE__
