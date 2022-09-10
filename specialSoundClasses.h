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

class dPoint
{
	// 2D point made of doubles, where x is time and y is value
public:
	double time = 0;
	double value = 0;
	dPoint(double initTime, double initValue) : time(initTime), value(initValue) {}
	dPoint() {}
};

class weightedPoint : public dPoint
{
public:
	double weight = 1;
	weightedPoint(dPoint initDp, double initWeight) : dPoint::dPoint(initDp), weight(initWeight) {}
	weightedPoint(double initTime, double initValue, double initWeight) : dPoint::dPoint(initTime, initValue), weight(initWeight) {}
	weightedPoint() {}

};

#include "helperThings.h"

class PointInterpolator : public sound
{
	weightedPoint prevPoint = weightedPoint(0, 0, 1); // initialized at {0, 0} with weight of 1
	weightedPoint nextPoint = weightedPoint(0, 0, 0);
public:
	double waveOffset = 0.0;
	std::function<weightedPoint(void)> fToGiveNextPoint;
	std::function<double(weightedPoint, weightedPoint, double)> interpolationFunction;

	PointInterpolator(std::function<weightedPoint(void)> funcToGiveNextPoint, WAVEFORMATEX initWfx, double initHzFrequency, double initVolume, std::function<double(weightedPoint,weightedPoint,double)> initInterpolationFunc = linearInterpolator) :
		fToGiveNextPoint(funcToGiveNextPoint),
		interpolationFunction(initInterpolationFunc)
	{
		wfx = initWfx;
		volume = initVolume;
		HzFrequency = initHzFrequency;
	}

	double nextFrame(double nextFrameDistance = 1.0)
	{
		if (firstTime)
		{
			nextPoint = fToGiveNextPoint();
			if (nextPoint.time == 0.0)
			{
				prevPoint.value = nextPoint.value;
				//double temp = nextPoint.time; (these two commented lines would be useless because they are for when time is not 0.0 on nextPoint)
				nextPoint = fToGiveNextPoint();
				//nextPoint.time += temp; // To make the Time in the function a releative offset
			}

			firstTime = false; // Setting firstTime back to true manualy may not result in expected results because the fToGiveNextPoint is not reset
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


		return interpolationFunction(prevPoint, nextPoint, waveOffset / wfx.nSamplesPerSec);


		/*
		// Linear interpolation, I plan on adding more interpolation options
		double diffTime = (nextPoint.time - prevPoint.time);
		if (diffTime == 0.0) // if the prevPoint and nextPoint are at the same time and that time is requested, return the value of nextPoint (without this, this case would be a divide by zero)
			return nextPoint.value;
		
		double ret = prevPoint.value + ((nextPoint.value - prevPoint.value) / diffTime) * ((waveOffset / wfx.nSamplesPerSec) - prevPoint.time);

		return ret * volume;
		*/
	}
};
