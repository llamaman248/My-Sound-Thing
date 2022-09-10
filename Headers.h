#pragma once
#include <iostream>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <functional>

#include "BaseSoundClass.h"
#include "slowFunctions.h"
#include "sFunctions.h"

#define forwardWriter for (size_t i = 0; i < dataLength; ++i)
#define reverseWriter for (size_t i = dataLength; i > 0; --i)



#include "specialSoundClasses.h"
#include "helperThings.h" // this include was moved to be inside of specialSoundClasses.h because it needed to be in the middle of it

#include "Sounds.h"
#include "melodyTest2.h"