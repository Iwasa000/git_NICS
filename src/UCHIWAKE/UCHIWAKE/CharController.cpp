#include "StdAfx.h"
#include "CharController.h"
#include "DBSubFnc8.h"				// 漢字逆変換クラス


// 文字列sourceを先頭からlengthバイト目で分割し、resultに格納
// 戻り値	0:成功 -1:失敗
int CharController::Divide(CString source, CString& result, int length/*必ずname[]の半分以下*/)
{
	//int one_line_char_num = length*2;
	//unsigned char name[128] = {0};
	//int max_size = (sizeof(name) / sizeof(name[0])) - 1/*NULL文字を入れる為の-1*/;

	//if(one_line_char_num > max_size)	return -1;

	//int strBaseLen = source.GetLength();
	//if(strBaseLen < max_size)	max_size = strBaseLen;	// 小さいほうのサイズを採用

	//memmove(name,(LPCSTR)source, max_size);

	//result.Empty();


	result.Empty();

	int divide_len = GetDivideLength(source, length);
	if(divide_len == -1)	return -1;

	//if( _ismbstrail(name, &name[length*2]) ){
	//	one_line_char_num--;
	//}

	result = source.Left( divide_len );

	return 0;
}

// 文字列sourceを先頭からlengthバイト目で分割し、result1にセット
// 残りをresult2にセット
// 戻り値	0:成功 -1:失敗
int CharController::Divide(CString source, CString& result1, CString& result2, int length/*必ずname[]の半分以下*/)
{
	//int one_line_char_num = length*2;
	//unsigned char name[128] = {0};
	//int max_size = (sizeof(name) / sizeof(name[0])) - 1/*NULL文字を入れる為の-1*/;
	//if(one_line_char_num > max_size)	return -1;
	//int strBaseLen = source.GetLength();
	//if(strBaseLen < max_size)	max_size = strBaseLen;	// 小さいほうのサイズを採用

	//memmove(name,(LPCSTR)source, max_size);

	//result1.Empty();
	//result2.Empty();

	//if( _ismbstrail(name, &name[length*2]) ){
	//	one_line_char_num--;
	//}

	result1.Empty();
	result2.Empty();

	int divide_len = GetDivideLength(source, length);
	if(divide_len == -1)	return -1;

	

	//Divide(source, result1, length);

	result1 = source.Left( divide_len );
	CString dmy_source = source;
	dmy_source.Delete( 0, divide_len );
	result2 = dmy_source;

	return 0;
}

int CharController::GetDivideLength(const CString& source, int length)
{
	int char_num = length*2;
	unsigned char name[128] = {0};
	int max_size = (sizeof(name) / sizeof(name[0])) - 1/*NULL文字を入れる為の-1*/;

	if(char_num > max_size)	return -1;

	int strBaseLen = source.GetLength();
	if(strBaseLen < max_size)	max_size = strBaseLen;	// 小さいほうのサイズを採用

	memmove(name,(LPCSTR)source, max_size);

	if( _ismbstrail(name, &name[length*2]) ){
		char_num--;
	}

	return char_num;
}