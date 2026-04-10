// SyogBrn.h : ヘッダー ファイル
//
#ifndef INCLUDE_SYOGBRN_H_
#define INCLUDE_SYOGBRN_H_

#include "BrnTkyDef.h"
#include "DBana8.h"


class CANAHYOITMdata
{
public:
	CANAHYOITMdata() {};
	~CANAHYOITMdata() {};

	CANAHYOITMdata& operator=(const CANAHYOITMdata& obj)
	{
		if (this != &obj) {

			hyono	= obj.hyono;
			seq		= obj.seq;
			itmno	= obj.itmno;
			itmcd	= obj.itmcd;
			kriksi	= obj.kriksi;
			nmatt	= obj.nmatt;
			itmsgn.Copy(obj.itmsgn);
			mdfnmkbn = obj.mdfnmkbn;
			regnm = obj.regnm;
			mdfnm = obj.mdfnm;
		}
		return *this;
	}

	void clear()
	{
		hyono = 0;
		seq = 0;
		itmno = 0;
		itmcd = 0;
		kriksi = 0;
		nmatt = 0;
		itmsgn.RemoveAll();
		mdfnmkbn = 0;
		regnm.Empty();
		mdfnm.Empty();
	}

public:
	BYTE	hyono;
	BYTE	seq;
	BYTE	itmno;
	long	itmcd;
	BYTE	kriksi;
	BYTE	nmatt;
	CByteArray itmsgn;		// 09.29 /06 追加
	BYTE	mdfnmkbn;
	CString	regnm;
	CString	mdfnm;
};



class CBrnTkySub;

//
// クラス定義
class CSyogBrn
{
//friend  class CBrnTkySub;

// コンストラクション
public:
	CSyogBrn();
	virtual ~CSyogBrn();

// アトリビュート
public:

private:
	/*-------------------------------------------
		アプリケーション単位別諸口枝番テーブル
	---------------------------------------------*/
	BRN_TB m_pApSyogBrnTBL;
	/*-------------------------------------------
		アプリケーション単位別諸口枝番テーブル
		実レコード数
	---------------------------------------------*/
	int m_ApSyogBrnNUM;

public:
	int			syogbrn_iniz( CDBZmSub* pZm );
	void		syogbrn_free();
	BRN_TB*		get_syogbrtb( BOOL full );
	BRN_NAME*	get_syogbrnmsg( CString& code, int brn, BOOL full );

	BOOL&		IsKmkcodeDisp();
	int			make_syogbrn();

// オペレーション
protected:
	void syogbrn_tbliniz();
	BOOL syogbrn_check( BOOL full );
	int MakeApSyogBrn( BRN_TB *tbl, BOOL full );
	// 10.16 /08
	BOOL syogbrn_master();

// インプリメンテーション
public:


private:
	CDBZmSub*	m_pZm;
	CDBAna*		m_pAna;

	BOOL		m_bKmcod;

	CArray<CANAHYOITMdata, CANAHYOITMdata&>	m_itmData;
};

#endif	// End of INCLUDE_SYOGBRN_H_
