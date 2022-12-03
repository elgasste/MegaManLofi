#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "ThreadWrapper.h"

using namespace MegaManLofi;

bool ThreadWrapper::SetThisThreadToHighestPriority() const
{
   return SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
}
