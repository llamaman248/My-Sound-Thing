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


class violin : public sound
{
	// Not actualy a violin, yet!
public:
	PointInterpolator soundWave;
	loopingVector<weightedPoint> lV;

	violin(WAVEFORMATEX initWfx, double initHzFrequency, double initVolume) : soundWave([this]
		{
			return lV.next();
		}, initWfx, HzFrequency, volume, weightedInterpolator)
	{
		HzFrequency = initHzFrequency;
		volume = initVolume;
		wfx = initWfx;
		lV.currentPiece->value = weightedPoint(1, -1, 1);
		lV.pushToNext(weightedPoint(1,1,2));
		//lV.selectNext(); // loops back to start
	}
	double nextFrame(double nextFrameDistance = 1)
	{
		soundWave.HzFrequency = HzFrequency;
		return soundWave.nextFrame(nextFrameDistance) * volume;
	}
	void reset()
	{
		firstTime = true;
		soundWave.reset();
	}
};
