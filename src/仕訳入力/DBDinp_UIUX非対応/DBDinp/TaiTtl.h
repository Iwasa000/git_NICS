// TaiTtl.h : ヘッダー ファイル
//
#include "TaiData.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiTtl

class CTaiTtl
{
// コンストラクション
public:
	CTaiTtl();

	BOOL IsEndCalq();
	void end_taiclq();
	void kmktbl_sort();
	void init_taiclq();

// アトリビュート
public:
protected:
	STaiData m_TaiData,m_TaiData2;

	int		tai_size;

// オペレーション
public:
	void taidata_inz( int kmcnt );
	void free_kmktbl();

	STaiData* get_taidata();
	void add_taidata( CDBINPDataRec *drec );
	void sub_taidata( CDBINPDataRec *drec );

// インプリメンテーション
public:
	virtual ~CTaiTtl();
};
