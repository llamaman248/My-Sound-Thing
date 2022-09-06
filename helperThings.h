#pragma once

#define InterpolArrayIndexer(ArrayToIndex, IndexVariable) customPitchWave([&]\
		{\
		++IndexVariable;\
		if (IndexVariable == ((double)sizeof(ArrayToIndex) / sizeof(ArrayToIndex[0])))\
		{\
			IndexVariable = 0;\
		}\
		ArrayToIndex[IndexVariable].value;\
		return ArrayToIndex[IndexVariable];\
		}