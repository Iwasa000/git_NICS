// ICS Application1Doc.h : CSimulateKuriwariDriverDoc クラスのインターフェイス
//


#pragma once

class CSimulateKuriwariDriverDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CSimulateKuriwariDriverDoc();
	DECLARE_DYNCREATE(CSimulateKuriwariDriverDoc)

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
	virtual ~CSimulateKuriwariDriverDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


