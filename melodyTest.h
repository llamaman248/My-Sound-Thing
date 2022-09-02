#pragma once

#include "Headers.h"


HANDLE melodyTest(WAVEFORMATEX wfx)
{
	HANDLE hReturn = createTempFile();
	vibrotoSquare SndVS(wfx, 1, 0.2);
	size_t length = wfx.nSamplesPerSec * 1;
	double dWrite;
	for (size_t i = 0; i < length; ++i)
	{
		dWrite = SndVS.nextFrame() * (abs((double)length - (i + length / 2)) * (1.0 / length));
		writeSoundFrame(wfx, hReturn, dWrite);
	}
	SndVS.waveOffset = 0;
	SndVS.pitchRatio *= 7.0 / 6.0;
	length *= 0.5;
	for (size_t i = 0; i < length; ++i)
	{
		dWrite = SndVS.nextFrame() * (((double)length - i) * (1.0 / length));
		writeSoundFrame(wfx, hReturn, dWrite);
	}
	SndVS.waveOffset = 0;
	SndVS.pitchRatio *= 7.0 / 6.0;
	for (size_t i = 0; i < length; ++i)
	{
		dWrite = SndVS.nextFrame() * (abs((double)length - (i + length / 2)) * (1.0 / length));
		writeSoundFrame(wfx, hReturn, dWrite);
	}
	SndVS.waveOffset = 0;
	SndVS.pitchRatio *= 7.0 / 6.0;
	length *= 2;
	for (size_t i = 0; i < length; ++i)
	{
		SndVS.waveOffset = 0;
		dWrite = SndVS.nextFrame() * (abs((double)length - (i + length / 2)) * (1.0 / length));
		writeSoundFrame(wfx, hReturn, dWrite);
	}
	SndVS.pitchRatio *= 6.0 / 7.0;
	for (size_t i = 0; i < length; ++i)
	{
		dWrite = SndVS.nextFrame();
		writeSoundFrame(wfx, hReturn, dWrite);
	}
	SndVS.waveOffset = 0;
	SndVS.pitchRatio *= 7.0 / 6.0;
	vibrotoSquare SndVS2(wfx, 1, 0.2);
	for (size_t i = 0; i < length; ++i)
	{
		dWrite = SndVS.nextFrame() * (abs((double)length - (i + length / 2)) * (1.0 / length)) + SndVS2.nextFrame() * (abs((double)length - (i + length / 2)) * (1.0 / length));
		writeSoundFrame(wfx, hReturn, dWrite);
	}

	/*
	* macro example
#define dynamicNote(length)\
{\
	for (size_t i = 0; i < (length); ++i)\
	{\
		dWrite = SndVS.nextFrame() * (abs((double)length - (i + (length) / 2)) * (1.0 / (length))) + SndVS2.nextFrame() * (abs((double)(length) - (i + (length) / 2)) * (1.0 / (length)));\
		writeSoundFrame(wfx, hReturn, dWrite);\
	}\
}

	dynamicNote(samplesPerSec * 1);
	*/

	return hReturn;
}
