#pragma once

class sound
{
	// Base class all sound classes should inherit from
protected:
	bool firstTime = true;
public:
	
	double waveOffset = 0.0;
	WAVEFORMATEX wfx = {}; // This class is not currently designed to have this change. If it needs to change, make sure to edit it inversely proportional to the waveOffset (so divide waveOffset by the value the samplesPerSecond is multiplied by)
	double HzFrequency = 1.0;
	double volume = 1.0;

	virtual double nextFrame(double nextFrameDistance = 1) = 0;
	virtual void reset()
	{
		waveOffset = 0;
		firstTime = true;
	}
};