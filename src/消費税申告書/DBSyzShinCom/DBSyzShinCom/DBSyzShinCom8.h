
#pragma	once

#ifdef _SYZSHINCOM_EXPORT_CHANGE_
	#define SYZSHINCOM_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZSHINCOM_CLASS_EXPORT __declspec(dllimport)
#endif

#include <SyzShinComDef.h>
#include "SyzUtil.h"
#include "SnHeadData.h"
