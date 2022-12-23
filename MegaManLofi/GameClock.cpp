#include <stdexcept>

#include "GameClock.h"
#include "IHighResolutionClock.h"
#include "ISleeper.h"

using namespace std;
using namespace MegaManLofi;

GameClock::GameClock( const shared_ptr<IHighResolutionClock> highResolutionClock,
                      const shared_ptr<ISleeper> sleeper,
                      unsigned int framesPerSecond ) :
   _highResolutionClock( highResolutionClock ),
   _sleeper( sleeper ),
   _framesPerSecond( framesPerSecond ),
   _totalFrameCount( 0 ),
   _lagFrameCount( 0 ),
   _frameStartTimeNano()
{
   if ( framesPerSecond == 0 )
   {
      throw invalid_argument( "Frame rate cannot be zero" );
   }

   _nanoSecondsPerFrame = 1'000'000'000ll / framesPerSecond;
   _secondsPerFrame = 1 / (double)framesPerSecond;
}

void GameClock::StartFrame()
{
   _frameStartTimeNano = _highResolutionClock->Now();
}

void GameClock::WaitForNextFrame()
{
   auto frameEndTimeNano = _highResolutionClock->Now();

   auto elapsedFrameTimeNano = frameEndTimeNano - _frameStartTimeNano;
   auto remainingFrameTimeNano = _nanoSecondsPerFrame - elapsedFrameTimeNano;

   if ( remainingFrameTimeNano > 0 )
   {
      _sleeper->Sleep( remainingFrameTimeNano );
   }
   else if ( remainingFrameTimeNano < 0 )
   {
      _lagFrameCount++;
   }

   _totalFrameCount++;
}
