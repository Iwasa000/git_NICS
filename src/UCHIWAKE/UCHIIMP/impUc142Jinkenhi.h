#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc142Jinkenhi.h"

class CimpUc142Jinkenhi :
	public CimpUc000Common
{
public:
	CimpUc142Jinkenhi(void);
	~CimpUc142Jinkenhi(void);

public:		// •Ï”

public:		// ŠÖ”
	// Še’ •[ŒÅ—L‚Ìˆ—
	virtual int Init(CDatabase* pDB);
	virtual int SetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 150905 add -->
	virtual int	SetCsvNullLineData142(void);
// midori 150905 add <--
// midori 180404 add -->
	virtual int ImpCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add <--
	virtual int GetCsvOneLineData(CStringArray* pstrCsvDataAry);

protected:	// •Ï”
	// ƒe[ƒuƒ‹
	CdbUc142Jinkenhi*		m_pUcData;				// “à–ó‘ƒe[ƒuƒ‹

protected:	// ŠÖ”


// –ðˆõ•ñVŽè“–/]‹Æˆõ‹‹—¿Žè“–/]‹Æˆõ’À‹àŽè“–‚Ìs”»’f—p
private:
	BOOL fYakuinHousyu;		// –ðˆõ•ñVŽè“–
	BOOL fJugyoinKyuyo;		// ]‹Æˆõ‹‹—¿Žè“–
	BOOL fJugyoinChingin;	// ]‹Æˆõ’À‹àŽè“–
};
