#pragma once
#include "headers.h"
#include "soundsMelodyTest2.h"

HANDLE melodyTest2(WAVEFORMATEX wfx, HFILE pitchWave = NULL)
{

#define bpm (124)

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


	// not perfectly in tune notes, but close enough for my purposes
	double d = 300;
	double a = std::pow(7 / 6.0, 3) * 300;
	double g = a * 6.0 / 7;
	double b = a * 7.0 / 6;
	double e = d * 7.0 / 6;
	double c = b * 8.0 / 7;


	note(1, b);
	note(1, b);

	note(0.5, 0);
	note(0.5, b);
	note(1, 0);

	note(1, b);
	note(0.5, 0);
	note(0.5, c * 7.0 / 6);
	note(1, 0);

	note(1, b);
	note(1, b);

	note(0.5, 0);
	note(0.5, b);
	note(1, 0);

	note(1, b);
	note(0.5, 0);
	note(0.5, c * 7.0 / 6);

	note(0.5, 0);
	note(0.5, c * std::pow(8.0 / 7, 3));
	note(0.5, c * 7.0 / 6);
	note(0.5, b);

	note(1, 0);
	note(1, b);

	note(0.5, 0);
	note(0.5, b);
	note(0.5, c * std::pow(8.0 / 7, 3));
	note(0.5, c * std::pow(8.0 / 7, 2));


	/*
	note(0.5, d);


	note(0.5 / 2, a);
	note(0.5 / 2, g);

	note(0.5 / 2, a);
	note(0.5 / 2, g);

	note(0.5, d);

	note(0.5, e);
	
	note(0.5 / 2, b);
	note(0.5 / 2 + 0.5, a);

	//note(0.5, 300); // included in the previous note (because slur)

	note(0.5 / 2, a);
	note(0.5 / 2, g);

	note(0.5 / 2, a);
	note(0.5 / 2, g);

	note(0.5, d);

	note(0.5, e);

	note(0.5 / 2, b);
	note(0.5 / 2 + 2, c);
	*/

	//note(2, 300); // slur

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