
#pragma	once

//-----------------------------------------------------------------------------
// 消費税申告書データ（平成26年度版）
//-----------------------------------------------------------------------------

#ifdef _SYZSHIN26DATA_EXPORT_CHANGE_
	#define SYZSHIN26DATA_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZSHIN26DATA_CLASS_EXPORT __declspec(dllimport)
#endif

// 各帳票データ
#pragma	pack(push,8)
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
//2018.03.13 INSERT START
#include "H28SnFhyo60Data.h"
//2018.03.13 INSERT END
#pragma	pack(pop)


