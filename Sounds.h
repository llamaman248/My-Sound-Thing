#pragma once
#include "Headers.h"

HANDLE soundProject1(WAVEFORMATEX wfx, double pitchRatio = 1)
{
	HANDLE hReturn = createTempFile();
	for (size_t i = 0; i < 200; ++i)
	{
		sHFILE hfile = createTempFile();
		sHFILE hfile2 = createTempFile();
		sHFILE hfile3 = createTempFile();
		generateSinWave(wfx, hfile, wfx.nSamplesPerSec * 0.03, wfx.nSamplesPerSec * 0.0004 * i * pitchRatio, 0.16, 0.99999);
		generateSinWave(wfx, hfile2, wfx.nSamplesPerSec * 0.03, wfx.nSamplesPerSec * 0.0005 * i * pitchRatio, 0.00006, 0.99999);
		generateSinWave(wfx, hfile3, wfx.nSamplesPerSec * 0.03, wfx.nSamplesPerSec * 0.00035 * i * pitchRatio, 0.9993);
		mySetFilePointer(hfile, 0, FILE_BEGIN);
		mySetFilePointer(hfile2, 0, FILE_BEGIN);
		mySetFilePointer(hfile3, 0, FILE_BEGIN);
		sHFILE h12 = addSounds(hfile, hfile2);
		mySetFilePointer(h12, 0, FILE_BEGIN);
		sHFILE h123 = addSounds(h12, hfile3);
		mySetFilePointer(h123, 0, FILE_BEGIN);
		append(hReturn, h123);
		//HANDLE hReturn = addSounds(h12, hfile3);
		//mySetFilePointer(hReturn, 0, FILE_BEGIN);
	}

	mySetFilePointer(hReturn, 0, FILE_BEGIN);
	sHFILE hFile = createTempFile();
	generateSinWave(wfx, hFile, wfx.nSamplesPerSec * 4, wfx.nSamplesPerSec * 0.09 * pitchRatio, 0.08, 0.9999);
	mySetFilePointer(hFile, 0, FILE_BEGIN);
	hReturn = addSounds(hReturn, hFile);
	mySetFilePointer(hReturn, 0, FILE_BEGIN);
	//hReturn = reverse(hReturn);
	return hReturn;
}

HANDLE efficientSoundSinShift(WAVEFORMATEX wfx, double pitchRatio = 1, double volumeRatio = 1)
{
	HANDLE hFile = createTempFile();

	DWORD bytesWritten = 0;
	double dWrite = 0;
	size_t length = wfx.nSamplesPerSec * 2;
	for (size_t i=0;i<length;++i)
	{
		dWrite = sinWave(wfx, (double)i / length * 500, i);
		writeSoundFrame(wfx, hFile, dWrite, &bytesWritten);
	}
	
	return hFile;
}

HANDLE randPitchTest(WAVEFORMATEX wfx, double pitchRatio = 1, double volumeRatio = 1)
{
	HANDLE hFile = createTempFile();

	DWORD bytesWritten = 0;
	double dWrite = 0;
	size_t length = wfx.nSamplesPerSec * 2;
	for (size_t i = 0; i < length; ++i)
	{
		dWrite = sinWave(wfx, rand(), i);
		writeSoundFrame(wfx, hFile, dWrite, &bytesWritten);
	}

	return hFile;
}

HANDLE sinPitchTest(WAVEFORMATEX wfx, double pitchRatio = 1, double volumeRatio = 1, HANDLE testH = 0)
{
	HANDLE hFile = createTempFile();

	DWORD bytesWritten = 0;
	double dWrite = 0;
	double test = 0;
	size_t length = wfx.nSamplesPerSec * 10;
	PitchShifter sinWave1(wfx, sinWave, 300, 0.1);
	for (size_t i = 0; i < length; ++i)
	{
		sinWave1.HzFrequency = sinWave(wfx, 10, i, 200) + 500;
		dWrite = sinWave1.nextFrame();
		writeSoundFrame(wfx, hFile, dWrite, &bytesWritten);
		if (testH)
			writeSoundFrame(wfx, testH, sinWave(wfx, 10, i, 1));
	}

	return hFile;
}

class instramentTest
{
	bool firstTime = true;
	PitchShifter pitchPitchSinWave;
	PitchShifter pitchSinWave;
	PitchShifter playingSquareWave;
public:
	double waveOffset = 0; // waveOffset in samples (increase rate depends on the HzFrequency value)

	WAVEFORMATEX wfx = {}; // This class is not currently designed to have this change.
	double pitchRatio = 1.0;
	double volume = 1.0;
	instramentTest(WAVEFORMATEX initWfx, double initPitchRatio, double initVolume, double initWaveOffset = 0.0)
		: // Initializers of PitchShifters:
		pitchSinWave(initWfx, sinWave, 0, 4),
		playingSquareWave(initWfx, squareWave, 300, 1),
		pitchPitchSinWave(initWfx, sinWave, 1, 2)
	{
		waveOffset = initWaveOffset;
		wfx = initWfx;
		pitchRatio = initPitchRatio;
		volume = initVolume;
	}

	double nextFrame(double nextFrameDistance = 1)
	{
		pitchSinWave.HzFrequency = (pitchPitchSinWave.nextFrame() + 3) * pitchRatio;
		playingSquareWave.HzFrequency = (pitchSinWave.nextFrame(nextFrameDistance) + 300) * pitchRatio;
		return playingSquareWave.nextFrame(nextFrameDistance) * volume;
	}
};


#include "soundsMelodyTest2.h"
HANDLE customWaveTest(WAVEFORMATEX wfx, HANDLE hPitch)
{
	HANDLE hReturn = createTempFile();

	violin sViolin(wfx, 300, 1);

	for(size_t i=0;i<wfx.nSamplesPerSec * 5;++i)
	{
		writeSoundFrame(wfx, hReturn, sViolin.nextFrame());
	}

	return hReturn;
}
