
#pragma	once

//-----------------------------------------------------------------------------
// 消費税申告書データ（消費税１０％対応版）
//-----------------------------------------------------------------------------

#ifdef _SYZSHIN31DATA_EXPORT_CHANGE_
	#define SYZSHIN31DATA_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZSHIN31DATA_CLASS_EXPORT __declspec(dllimport)
#endif

// 各帳票データ
#pragma	pack(push,8)

#include "H31SnHonpyoData.h"
#include "H31SnHonpyo2Data.h"
#include "H31SnFhyo2_01Data.h"
#include "H31SnFhyo2_02Data.h"
#include "H31SnFhyo1_01Data.h"
#include "H31SnFhyo1_02Data.h"
#include "H31SnFhyo4_01Data.h"
#include "H31SnFhyo4_02Data.h"
#include "H31SnFhyo5_01Data.h"
#include "H31SnFhyo5_012Data.h"
#include "H31SnFhyo5_02Data.h"
#include "H31SnFhyo5_022Data.h"
#include "H31SnFhyo6Data.h"
#include "H31SnFhyo6_2wariData.h"	//('23.05.01 Add)
#include "H31SnKskData.h"
#include "H31SnChskData.h"
#include "H31SnTanaData.h"


/*
#include "H26SnHonpyoData.h"
#include "H26SnFhyo10Data.h"
#include "H26SnFhyo20Data.h"
#include "H26SnFhyo50Data.h"
#include "H26SnTanaData.h"
#include "H26SnChskData.h"
#include "H27SnHonpyoData.h"
#include "H27SnFhyo10Data.h"
#include "H27SnFhyo50Data.h"
#include "H28SnFhyo10Data.h"
#include "H28SnFhyo20Data.h"
#include "H28SnHonpyoData.h"
#include "H28SnFhyo60Data.h"

*/
#pragma	pack(pop)


