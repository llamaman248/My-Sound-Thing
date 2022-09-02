#pragma once

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