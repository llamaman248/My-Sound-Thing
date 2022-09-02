// WaveOutputTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once

#include <iostream>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

#define samplesPerSec (double)16000  // Has to be an integer, only marked as double to fix rounding errors
#define copyBufferSize 4096

#include "headers.h"

void saveSoundFile(WAVEFORMATEX, HANDLE, const wchar_t*);

//#include "audioFunctions.cpp"
int main()
{
	
	

	DWORD dwWriteValue;
	DWORD bytesWritten = NULL;
	LARGE_INTEGER fileSize = {};
	WAVEFORMATEX wfx = {};
	
	
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = samplesPerSec; // 8000 / 1000 = 8.0kHz
	wfx.wBitsPerSample = 32;
	wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.wBitsPerSample * wfx.nChannels) / 8; // 8 bits in a byte
	wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;  // size of a single frame of audio
	wfx.cbSize = 0; // Not used. Excluded from file.
	

	sHFILE pitchWave = createTempFile();
	sHFILE hTempDataFile = melodyTest2(wfx, (HFILE)(HANDLE)pitchWave);

	if (hTempDataFile == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}
	/*
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, samplesPerSec / 32, 0.03, 0.99996);
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, samplesPerSec / 16, 0.03, 0.99996);
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, samplesPerSec / 12, 0.03, 0.99996);
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, samplesPerSec / 10, 0.03, 0.99996);
	generateSquareWave(hTempDataFile, samplesPerSec * 1.5, samplesPerSec / 9, 0.03, 0.99996);
	*/
	

	saveSoundFile(wfx, hTempDataFile, L"NewSound.wav");
	saveSoundFile(wfx, pitchWave, L"pitchWave.wav");
}

void saveSoundFile(WAVEFORMATEX wfx, HANDLE rawSoundFile, const wchar_t* fileName)
{
	HANDLE hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE | NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed";
		return;
	}

	DWORD dwWriteValue;
	DWORD bytesWritten;
	dwWriteValue = fourccRIFF;
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);
	dwWriteValue = 0;
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);
	dwWriteValue = fourccWAVE;
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);
	dwWriteValue = fourccFMT;
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);
	dwWriteValue = 16; // sizeof WAVEFORMATEX without cbsize
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);
	WriteFile(hFile, &wfx, dwWriteValue, &bytesWritten, NULL);
	dwWriteValue = fourccDATA;
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);


	LARGE_INTEGER lI = {};
	GetFileSizeEx(rawSoundFile, &lI);
	dwWriteValue = lI.QuadPart;
	WriteFile(hFile, &dwWriteValue, sizeof(DWORD), &bytesWritten, NULL);

	char dataBuffer[copyBufferSize];
	DWORD bytesRead;
	SetFilePointer(rawSoundFile, 0, 0, FILE_BEGIN);
	while (ReadFile(rawSoundFile, dataBuffer, sizeof(dataBuffer), &bytesRead, NULL) && bytesRead > 0)
	{
		if (!WriteFile(hFile, dataBuffer, bytesRead, &bytesWritten, NULL))
		{
			std::cout << "fail write";
			break;
		}
	}

	SetFilePointer(hFile, 4, 0, FILE_BEGIN);



	GetFileSizeEx(hFile, &lI);

	if (lI.HighPart != 0)
	{
		lI.QuadPart -= sizeof(DWORD) * 2;
		if (lI.HighPart != 0)
			std::cout << "File is too large";
	}
	else
	{
		if (lI.LowPart > sizeof(DWORD) * 2)
			lI.LowPart -= sizeof(DWORD) * 2;
		else
			std::cout << "File is too small";
	}
	WriteFile(hFile, &lI.LowPart, sizeof(DWORD), &bytesWritten, NULL);

	// Extend the data to fill the nearest WOORD boundry

	if (lI.QuadPart % 2 == 1)
	{
		SetFilePointer(hFile, 0, 0, FILE_END);
		dwWriteValue = 0;
		WriteFile(hFile, &dwWriteValue, 1, &bytesWritten, NULL);
	}

}