#pragma once
#include "headers.h"
#include "soundsMelodyTest2.h"

HANDLE melodyTest2(WAVEFORMATEX wfx, HFILE pitchWave = NULL)
{

#define bpm (60)

#define tempoRatio (60.0 / bpm)

	HANDLE hReturn = createTempFile();
	PitchShifter SndVS(wfx, sinWave, 0, 1);
	//vibrotoSquare SndVS(wfx, 1.0, .5);
	double dWrite;

#define forlen(length) for (size_t i = 0; i < (length); ++i)

#define note(seconds, PitchRatio, buildUp)\
{ size_t length = (seconds) * (tempoRatio) * wfx.nSamplesPerSec; SndVS.HzFrequency = (PitchRatio);\
forlen(length)\
{dWrite = SndVS.nextFrame() * (1.0 / length * (length - i));\
writeSoundFrame(wfx, hReturn, dWrite); } SndVS.reset(); }



	note(0.5, 300);

	note(0.5 / 2, 300);
	note(0.5 / 2, 300);

	note(0.5 / 2, 300);
	note(0.5 / 2, 300);

	note(0.5, 300);

	note(0.5, 300);
	
	note(0.5 / 2, 300);
	note(0.5 / 2, 300);

	note(0.5, 300);

	note(0.5 / 2, 300);
	note(0.5 / 2, 300);

	note(0.5 / 2, 300);
	note(0.5 / 2, 300);

	note(0.5, 300);

	note(0.5, 300);

	note(0.5 / 2, 300);
	note(0.5 / 2, 300);

	note(2, 300);
	/*
	forlen(samplesPerSec * 30)
	{
		dWrite = pow(SndVS.nextFrame() * 2, 3) * 0.1;
		writeSoundFrame(wfx, hReturn, dWrite);
		SndVS.HzFrequency = 300;
	}*/
	/*
	POINT pt = {};
	int height = GetSystemMetrics(SM_CYSCREEN);

	forlen(samplesPerSec * 3)
	{
		GetCursorPos(&pt);
		SndVS.HzFrequency = height - pt.y;
		dWrite = SndVS.nextFrame();
		writeSoundFrame(wfx, hReturn, dWrite);
	}
	*/
	return hReturn;
}