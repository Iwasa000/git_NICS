#ifndef __PICTURE_H__
#define __PICTURE_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

/////////////////////////////////////////////////////////////////////////////
// CPicture ラップ クラス

class CPicture : public COleDispatchDriver
{
public:
	CPicture() {}		// COleDispatchDriver デフォルト コンストラクション
	CPicture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPicture(const CPicture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// アトリビュート
public:
	long GetHandle();
	long GetHPal();
	void SetHPal(long);
	short GetType();
	long GetWidth();
	long GetHeight();

// オペレーション
public:
	// メソッド 'Render' は戻り値の型またはパラメータ型が無効なため表示できません。
};

#endif // __PICTURE_H__
