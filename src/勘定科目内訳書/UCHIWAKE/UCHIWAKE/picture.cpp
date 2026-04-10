// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "picture.h"


/////////////////////////////////////////////////////////////////////////////
// CPicture プロパティ

long CPicture::GetHandle()
{
	long result;
	GetProperty(0x0, VT_I4, (void*)&result);
	return result;
}

long CPicture::GetHPal()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CPicture::SetHPal(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

short CPicture::GetType()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

long CPicture::GetWidth()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

long CPicture::GetHeight()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CPicture オペレーション
