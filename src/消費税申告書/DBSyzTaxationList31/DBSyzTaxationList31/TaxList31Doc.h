// DBSyzShinMainDoc.h : CTaxlist31Doc クラスのインターフェイス
//

#pragma once

#ifdef _SYZ_TAXATIONLIST31_EXPORT_CHANGE_
#define SYZ_TAXATIONLIST31_CLASS_EXPORT __declspec(dllexport)
#else
#define SYZ_TAXATIONLIST31_CLASS_EXPORT
#endif

class SYZ_TAXATIONLIST31_CLASS_EXPORT CTaxList31Doc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CTaxList31Doc();
	DECLARE_DYNCREATE(CTaxList31Doc)

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
	virtual ~CTaxList31Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


