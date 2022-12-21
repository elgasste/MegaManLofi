#include "GameClock.h"
#include "IHighResolutionClock.h"
#include "ISleeper.h"

using namespace std;
using namespace MegaManLofi;

GameClock::GameClock( const shared_ptr<IHighResolutionClock> highResolutionClock,
                      const shared_ptr<ISleeper> sleeper,
                      int framesPerSecond ) :
   _highResolutionClock( highResolutionClock ),
   _sleeper( sleeper ),
   _framesPerSecond( framesPerSecond ),
   _totalFrameCount( 0 ),
   _lagFrameCount( 0 ),
   _frameStartTimeNano(),
   _nanoSecondsPerFrame( 1'000'000'000ll / framesPerSecond ),
   _frameScalar( 1 / (double)framesPerSecond )
{
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
