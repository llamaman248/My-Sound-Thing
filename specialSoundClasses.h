#pragma once
// PitchShifter provides an elegant way to change the pitch of a wave for every time the next sample is obtained.
// Using the functions alone gives a rough result when the pitch changes due to the wave compressing at it's origin and not at the
// current sample position, which is required for a smooth result.
class PitchShifter : public sound
{
public:
	double (*sFunc)(WAVEFORMATEX wfx, double HzFrequency, double requestedFrame, double volume) = 0;
	
	PitchShifter(WAVEFORMATEX initWfx, 
		double soundFunc(WAVEFORMATEX wfx, double HzFrequency,double requestedFrame, double volume), 
		double initHzFrequency, double initVolume, double initWaveOffset = 0.0)
	{
		sFunc = soundFunc;
		waveOffset = initWaveOffset;
		wfx = initWfx;
		HzFrequency = initHzFrequency;
		volume = initVolume;
	}

	double nextFrame(double nextFrameDistance = 1)
	{
		if (!firstTime)
			waveOffset += HzFrequency * nextFrameDistance;
		else
			firstTime = false;
		return sFunc(wfx, 1.0, waveOffset, volume);
	}

	void reset()
	{
		waveOffset = 0.0;
		firstTime = true;
	}
};

