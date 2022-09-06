#pragma once

#include "Headers.h"

class vibrotoSquare : public sound
{
	PitchShifter pitchPitchSinWave;
	PitchShifter pitchSinWave;
	PitchShifter playingSquareWave;
public:
	vibrotoSquare(WAVEFORMATEX initWfx, double initPitchRatio, double initVolume)
		: // Initializers of PitchShifters:
		pitchSinWave(initWfx, sinWave, 0, 4),
		playingSquareWave(initWfx, squareWave, 300, 1),
		pitchPitchSinWave(initWfx, sinWave, 1, 2)
	{
		wfx = initWfx;
		HzFrequency = initPitchRatio;
		volume = initVolume;
	}

	double nextFrame(double nextFrameDistance = 1)
	{
		pitchSinWave.HzFrequency = (pitchPitchSinWave.nextFrame() + 3) * HzFrequency;
		playingSquareWave.HzFrequency = (pitchSinWave.nextFrame(nextFrameDistance) + 300) * HzFrequency;
		return playingSquareWave.nextFrame(nextFrameDistance) * volume;
	}
	void reset()
	{
		firstTime = true;
		pitchSinWave.reset();
		pitchPitchSinWave.reset();
		playingSquareWave.reset();
	}
};