#include <chrono>
#include <thread>

#include "SleeperWrapper.h"

using namespace std;
using namespace MegaManLofi;

void SleeperWrapper::Sleep( long long nanoseconds )
{
   this_thread::sleep_for( chrono::nanoseconds( nanoseconds ) );
}
