#pragma once
#include "Headers.h"

double sinWave(WAVEFORMATEX wfx, double HzFrequency, double requestedFrame, double volume = 0.2)
{
	return sin((requestedFrame / wfx.nSamplesPerSec) / ((1 / HzFrequency) / (M_PI * 2))) * volume;
}

double squareWave(WAVEFORMATEX wfx, double HzFrequency, double requestedFrame, double volume = 0.2)
{
	if (sinWave(wfx, HzFrequency, requestedFrame, volume) >= 0)
		return volume;
	else
		return -volume;
}

//typedef dPoint range; // x-coord (time) is min, y-coord (value) is max	(typedefed to make casting convienient (without needing to make a class)) (actualy for readablility, I did not do this)
typedef struct tag_range
{
	double min = 0;
	double max = 0;
} range;

double map(range oldRange, range newRange, double valOld)
{
	// Maps a point from one range onto another. Same as the map node in Gbj
	// I might get rid of the range struct and just have this function have more parameters instead
	return (valOld - oldRange.min) * ((newRange.max - newRange.min) / (oldRange.max - oldRange.min)) + newRange.min;
}

