#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>

#include <chrono>

#include "HighResolutionClockWrapper.h"

using namespace std;
using namespace MegaManLofi;

HighResolutionClockWrapper::HighResolutionClockWrapper()
{
   // According to documentation, this sets the system's minimum clock resolution to
   // 1 millisecond. Without it, higher frame rates have unpredictable results in Windows.
   timeBeginPeriod( 1 );
}

HighResolutionClockWrapper::~HighResolutionClockWrapper()
{
   timeEndPeriod( 1 );
}

long long HighResolutionClockWrapper::Now()
{
   return chrono::steady_clock::now().time_since_epoch().count();
}
