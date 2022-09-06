#pragma once
// PitchShifter provides an elegant way to change the pitch of a wave for every time the next sample is obtained.
// Using the functions alone gives a rough result when the pitch changes due to the wave compressing at it's origin and not at the
// current sample position, which is required for a smooth result.
class PitchShifter : public sound
{
public:
	double waveOffset = 0.0;
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

typedef struct tag_dPoint
{
	// 2D point made of doubles
	double time = 0;
	double value = 0;
} dPoint;



class PointInterpolator : public sound
{
	dPoint prevPoint = {}; // initialized at {0, 0}
	dPoint nextPoint = {};
public:
	double waveOffset = 0.0;
	std::function<dPoint(void)> fToGiveNextPoint;

	PointInterpolator(std::function<dPoint(void)> funcToGiveNextPoint, WAVEFORMATEX initWfx, double initHzFrequency, double initVolume) :
		fToGiveNextPoint(funcToGiveNextPoint)
	{
		wfx = initWfx;
		volume = initVolume;
		HzFrequency = initHzFrequency;
		nextPoint = fToGiveNextPoint();
		if (nextPoint.time == 0.0)
		{
			prevPoint.value = nextPoint.value;
			//double temp = nextPoint.time; (these two commented lines would be useless because they are for when time is not 0.0 on nextPoint)
			nextPoint = fToGiveNextPoint();
			//nextPoint.time += temp; // To make the Time in the function a releative offset
		}

	}

	double nextFrame(double nextFrameDistance = 1.0)
	{
		if (firstTime)
		{
			firstTime = false;
			return prevPoint.value;
		}
		waveOffset += HzFrequency * nextFrameDistance;
	
	checkIfWaveOffsetIsInDomain:

		// If waveOffset is not in current domain, get the next one and check again
		if (waveOffset > (nextPoint.time * wfx.nSamplesPerSec))
		{
			prevPoint = nextPoint;
			double temp = nextPoint.time;
			nextPoint = fToGiveNextPoint();
			nextPoint.time += temp; // To make the function return a relative time offset
			goto checkIfWaveOffsetIsInDomain;
		}

		// Linear interpolation, I plan on adding more interpolation options
		double diffTime = (nextPoint.time - prevPoint.time);
		if (diffTime == 0.0) // if the prevPoint and nextPoint are at the same time and that time is requested, return the value of nextPoint (without this, this case would be a divide by zero)
			return nextPoint.value;
		
		double ret = prevPoint.value + ((nextPoint.value - prevPoint.value) / diffTime) * ((waveOffset / wfx.nSamplesPerSec) - prevPoint.time);

		return ret * volume;
	}
};
