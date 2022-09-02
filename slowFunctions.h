#pragma once

void SafeClose(HANDLE handle)
{
	if (handle)
	{
		CloseHandle(handle);
		handle = NULL;
	}
}


class sHFILE
{
	HANDLE hFile = NULL;

public:
	sHFILE(HANDLE initHFile)
	{
		hFile = initHFile;
	}
	~sHFILE()
	{
		SafeClose(hFile);
	}
	HANDLE operator=(HANDLE newHFile)
	{
		SafeClose(hFile);
		return hFile = newHFile;
	}
	operator HANDLE()
	{
		return hFile;
	}
};

HANDLE createTempFile()
{
	wchar_t tempFileName[MAX_PATH] = {};
	GetTempFileName(L".\\temp\\", L"raw", 0, tempFileName);
	HANDLE hTempFile = CreateFile(tempFileName, GENERIC_READ | GENERIC_WRITE, /*FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE*/ NULL, NULL,
		OPEN_EXISTING, /*FILE_ATTRIBUTE_TEMPORARY | /* Might want this, but not sure*/ FILE_FLAG_DELETE_ON_CLOSE, NULL); // GetTempFileName creates the file, so this only needs to open it
	return hTempFile;
}

HANDLE reverse(HANDLE hFile)
{
	// Returns a file that is reversed of the one given (does not delete or overide the original. Starts at the previous position of the file pointer)
	HANDLE hTempFile = createTempFile();
	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		return INVALID_HANDLE_VALUE;
	}
	char buffer[copyBufferSize];
	char buffer2[copyBufferSize];
	DWORD bytesRead;
	DWORD bytesWritten;
	LARGE_INTEGER fileSize;
	LARGE_INTEGER li = {};
	GetFileSizeEx(hFile, &fileSize);
	SetFilePointerEx(hFile, li, &li, FILE_CURRENT);
	li.QuadPart = fileSize.QuadPart - li.QuadPart;

	SetFilePointerEx(hTempFile, li, NULL, FILE_CURRENT);
	while (ReadFile(hFile, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
	{
		// Write the contents of the file in another file in reverse
		for (size_t i = 0; i < bytesRead; ++i)
		{
			buffer2[bytesRead - 1 - i] = buffer[i];
		}
		li.QuadPart -= bytesRead; // Litterly just -bytesRead, but done in a way to avoid unsigned errors
		SetFilePointerEx(hTempFile, li, NULL, FILE_BEGIN);
		WriteFile(hTempFile, &buffer2, bytesRead, &bytesWritten, NULL);

	}
	SetFilePointer(hTempFile, 0, 0, FILE_BEGIN);
	return hTempFile;
}

// TODO: add the efficiencies that I added to the other reverse function
HANDLE reverse(HANDLE hFile, unsigned long long bytesToReverse) // maybe create a third one where you choose the location of the reverse (Not needed though because I can just use the append function)
{
	// Returns a file that is reversed of the one given (does not delete or overide the original. Starts at the previous position of the file pointer)
	HANDLE hTempFile = createTempFile();
	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		return INVALID_HANDLE_VALUE;
	}
	char buffer[copyBufferSize];
	char buffer2[copyBufferSize];
	DWORD bytesRead;
	DWORD bytesWritten;
	LARGE_INTEGER fileSize;
	LARGE_INTEGER li = {};
	GetFileSizeEx(hFile, &fileSize);
	SetFilePointerEx(hFile, li, &li, FILE_CURRENT);
	li.QuadPart = min(fileSize.QuadPart - li.QuadPart, bytesToReverse);

	SetFilePointerEx(hTempFile, li, NULL, FILE_CURRENT);
	while (ReadFile(hFile, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0 && bytesToReverse > 0)
	{
		// Write the contents of the file in another file in reverse
		for (size_t i = 0; i < bytesRead; ++i)
		{
			buffer2[bytesRead - 1 - i] = buffer[i];
		}
		li.QuadPart -= bytesRead;
		SetFilePointerEx(hTempFile, li, NULL, FILE_BEGIN);
		WriteFile(hTempFile, &buffer2, bytesRead, &bytesWritten, NULL);
		bytesToReverse -= bytesRead;
	}
	SetFilePointer(hTempFile, 0, 0, FILE_BEGIN);
	return hTempFile;
}

void append(HANDLE mainFile, HANDLE fileToAppend)
{
	// appends bytesToAppend bytes of fileToAppend from it's current file pointer, to the position of the file pointer on mainFile
	char buffer[copyBufferSize];
	DWORD bytesRead;
	DWORD bytesWritten;
	while (ReadFile(fileToAppend, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
	{
		WriteFile(mainFile, &buffer, bytesRead, &bytesWritten, NULL);
	}
}

void append(HANDLE mainFile, HANDLE fileToAppend, unsigned long long bytesToAppend)
{
	// appends bytesToAppend bytes of fileToAppend from it's current file pointer, to the position of the file pointer on mainFile
	char buffer[copyBufferSize];
	DWORD bytesRead;
	DWORD bytesWritten;
	while (ReadFile(fileToAppend, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0 && bytesToAppend > 0)
	{
		WriteFile(mainFile, &buffer, bytesRead, &bytesWritten, NULL);
		bytesToAppend -= bytesRead;
	}
}

HANDLE makeCopy(HANDLE fileToCopy) // Currently, if you want more control, use append
{
	HANDLE tempFile = createTempFile();
	SetFilePointer(fileToCopy, 0, 0, FILE_BEGIN);
	append(tempFile, fileToCopy);
	SetFilePointer(tempFile, 0, 0, FILE_BEGIN);
	return tempFile;
}

HANDLE noiseDistort(HANDLE fileToDistort, WORD amount)
{
	HANDLE hTempFile = createTempFile();
	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		return INVALID_HANDLE_VALUE;
	}
	char buffer[copyBufferSize];
	DWORD bytesRead;
	DWORD bytesWritten;

	while (ReadFile(fileToDistort, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
	{
		for (size_t i = 0; i < bytesRead; ++i)
		{
			buffer[i] = buffer[i] + ((((double)rand() / (double)RAND_MAX) * 2 - 1) * amount);
		}
		WriteFile(hTempFile, &buffer, bytesRead, &bytesWritten, NULL);
	}
	SetFilePointer(hTempFile, 0, 0, FILE_BEGIN);
	return hTempFile;
}

double maxVolume(WAVEFORMATEX wfx)
{
	switch (wfx.wBitsPerSample)
	{
	case 8:
		return 127.5;
	case 16:
		return 32767.5;
	case 32:
		return 2147483647.5;
	default:
		return 1;
	}
}

BOOL writeByte(HANDLE hFile, BYTE byteToWrite, LPDWORD numberOfBytesWritten)
{
	return WriteFile(hFile, &byteToWrite, 1, numberOfBytesWritten, NULL);
}

BOOL writeSoundFrame(WAVEFORMATEX wfx, HANDLE hFile, double soundFrame, LPDWORD bytesWritten)
{
	switch (wfx.wBitsPerSample)
	{
		// look into rounding at .5 rather than just rounding down
	case 8:
	{
		DWORD dwWrite = soundFrame * maxVolume(wfx) + 127.5;
		return writeByte(hFile, dwWrite, bytesWritten);
	}
	break;
	case 16:
	{
		DWORD dwWrite = soundFrame * maxVolume(wfx);
		return WriteFile(hFile, &dwWrite, 2, bytesWritten, NULL);
	}
	break;
	case 32:
	{
		DWORD dwWrite = soundFrame * maxVolume(wfx);
		return WriteFile(hFile, &dwWrite, sizeof(DWORD), bytesWritten, NULL);
	}
	break;
	default:
		return FALSE;
	}
}

BOOL writeSoundFrame(WAVEFORMATEX wfx, HANDLE hFile, double soundFrame)
{
	DWORD bytesWritten = 0;
	return writeSoundFrame(wfx, hFile, soundFrame, &bytesWritten); // Calls the other one, but without you needing to give it a bytesWritten pointer
}

void dblFile(HANDLE hFile)
{
	HANDLE copy = makeCopy(hFile);
	SetFilePointer(hFile, 0, 0, FILE_END);
	append(hFile, copy);
	CloseHandle(copy);
}

void generateSquareWave(HANDLE hFile, int dataLength, double swapsPerSecond, double baseAmp = 0.04, double slopeAmp = 1, double minAmp = 0, double maxAmp = 0.04)
{
	double swapsPerSample = swapsPerSecond / samplesPerSec;
	int swapsDone = 0;
	DWORD bytesWritten = 0;
	DWORD dwWriteValue;
	bool upper = false;
	for (size_t i = 0; i < dataLength; ++i)
	{
		if (swapsPerSample * i >= (1 + swapsDone))
		{
			upper = !upper;
			++swapsDone;
		}
		if (upper)
		{
			dwWriteValue = (baseAmp * 127.5) + 127.5;
		}
		else
		{
			dwWriteValue = -(baseAmp * 127.5) + 127.5;
		}
		writeByte(hFile, dwWriteValue, &bytesWritten);

		baseAmp *= slopeAmp;
		if (baseAmp > maxAmp)
			baseAmp = maxAmp;
		if (baseAmp < minAmp)
			baseAmp = minAmp;
	}
}

void generateSinWave(HANDLE hFile, int dataLength, double swapsPerSecond, double baseAmp = 0.08, double slopeAmp = 1, double minAmp = 0, double maxAmp = 0.16)
{
	double swapsPerSample = swapsPerSecond / samplesPerSec;
	DWORD bytesWritten = 0;
	char cWriteValue;
	for (size_t i = 0; i < dataLength; ++i)
	{
		cWriteValue = sin(M_PI * swapsPerSample * i) * 127.5 * baseAmp + 127.5;
		WriteFile(hFile, &cWriteValue, sizeof(char), &bytesWritten, NULL);
		baseAmp *= slopeAmp;
		if (baseAmp > maxAmp)
			baseAmp = maxAmp;
		if (baseAmp < minAmp)
			baseAmp = minAmp;
	}
}

void generateSilence(WAVEFORMATEX wfx, HANDLE hFile, int dataLength)
{
	dataLength *= wfx.wBitsPerSample / 8;
	LONGLONG ll = 0;
	if (wfx.wBitsPerSample == 8)
		ll = 0x7F7F7F7F7F7F7F7F; // 7F is the position of the middle of the speaker (127). It is repeated for a whole 64-bit number. I think this makes setting the bits to this value faster because it does the most it can at a time (64-bit cpu)
	else
		ll = 0;
	DWORD bytesWritten = 0;
	for (size_t i = 0; i < dataLength / 8; ++i)
	{
		WriteFile(hFile, &ll, sizeof(LONGLONG), &bytesWritten, NULL);
	}
	for (size_t i = 0; i < dataLength % 8; ++i)
		writeSoundFrame(wfx, hFile, 0.0, &bytesWritten);
}

// Adds the values of two sounds together
HANDLE addSounds(HANDLE hFile1, HANDLE hFile2)
{
	char buffer[copyBufferSize];
	char buffer2[copyBufferSize];
	DWORD bytesRead;
	DWORD bytesWritten;
	DWORD bytesRead2;
	DWORD bytesWritten2;
	HANDLE hTempFile = createTempFile();

	while (true)
	{
		if (!(ReadFile(hFile1, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0))
		{
			while (ReadFile(hFile2, &buffer2, sizeof(buffer2), &bytesRead2, NULL) && bytesRead2 > 0)
			{
				WriteFile(hTempFile, &buffer2, bytesRead2, &bytesWritten2, NULL);
			}
			break;
		}
		if (!(ReadFile(hFile2, &buffer2, sizeof(buffer2), &bytesRead2, NULL) && bytesRead2 > 0))
		{
			while (ReadFile(hFile1, &buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
			{
				WriteFile(hTempFile, &buffer, bytesRead, &bytesWritten, NULL);
			}
			break;
		}
		for (size_t i = 0; i < bytesRead; ++i)
		{
			buffer[i] += ((int)buffer2[i]) - 127;
		}
		WriteFile(hTempFile, &buffer, bytesRead, &bytesWritten, NULL);
	}
	return hTempFile;
}

BOOL mySetFilePointer(HANDLE hFile, LONGLONG offset, DWORD dwMoveMethod)
{
	LARGE_INTEGER li = { offset };
	return SetFilePointerEx(hFile, li, NULL, dwMoveMethod);
}

BOOL setToStart(HANDLE hFile)
{
	return mySetFilePointer(hFile, 0, FILE_BEGIN);
}

BOOL setToEnd(HANDLE hFile)
{
	return mySetFilePointer(hFile, 0, FILE_END);
}

