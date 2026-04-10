// DBSyzShinMainDoc.h : CH26HyoDoc クラスのインターフェイス
//

#pragma once

#ifdef _SYZ_H26_HYO_EXPORT_CHANGE_
	#define SYZ_H26_HYO_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZ_H26_HYO_CLASS_EXPORT 
#endif

class SYZ_H26_HYO_CLASS_EXPORT CH26HyoDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CH26HyoDoc();
	DECLARE_DYNCREATE(CH26HyoDoc)

// 属性
public:

// 操作
public:

// オーバーライド
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 実装
public:
	virtual ~CH26HyoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


