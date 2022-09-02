#pragma once

#include "headers.h"

HANDLE musicTest(WAVEFORMATEX wfx)
{
#define samplesPerSec ((double)wfx.nSamplesPerSec)
	HANDLE hTempDataFile = createTempFile();
	if (hTempDataFile == INVALID_HANDLE_VALUE)
		return INVALID_HANDLE_VALUE;
	int t = samplesPerSec / 32;
	double slope = .1;
	double slope2 = 0.997;
	double slope3 = 1;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);
	t *= slope + 1;
	slope *= slope2;
	slope2 *= slope3;
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, t * (1.0 / 2.0), 0.03, 0.99996);

	SetFilePointer(hTempDataFile, 0, 0, FILE_BEGIN);
	HANDLE temp = reverse(hTempDataFile);
	append(hTempDataFile, temp);
	return hTempDataFile;
}