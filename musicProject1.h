#pragma once

#include "Headers.h"

HANDLE musicProject1()
{
	HANDLE hReturn = soundProject1();
	sHFILE hTemp = soundProject1(2);
	setToEnd(hReturn);
	append(hReturn, hTemp);
	hTemp = soundProject1(0.5);    // make setToStart happen auto from the sound project and setToEnd after append (done)
	append(hReturn, hTemp);



	return hReturn;
}