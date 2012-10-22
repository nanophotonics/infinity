#pragma once
#include "infinity.h"
#pragma pack(2)

struct GetNumberParams
{
	double result;
};

struct GetStringParams
{
	Handle result;
};

namespace Functions
{
	long Monochrome(GetNumberParams* p);
	long MaxWidth(GetNumberParams* p);
	long MaxHeight(GetNumberParams* p);
	long Model(GetStringParams* p);
	long Exposure(GetNumberParams* p);
	long Gain(GetNumberParams* p);
	long TargetLeft(GetNumberParams* p);
	long TargetTop(GetNumberParams* p);
	long TargetWidth(GetNumberParams* p);
	long TargetHeight(GetNumberParams* p);

	long Register(long index);
}

#pragma pack()