#include <stdexcept>

#include "GameClock.h"
#include "IHighResolutionClock.h"

using namespace std;
using namespace MegaManLofi;

GameClock::GameClock( const shared_ptr<IHighResolutionClock> highResolutionClock ) :
   _highResolutionClock( highResolutionClock ),
   _minimumFrameRate( -1 ),
   _minNanoSecondsPerFrame( -1 ),
   _hasMinimumFrameRate( false ),
   _wasLagFrame( false ),
   _totalFrameCount( 0 ),
   _lagFrameCount( 0 ),
   _absoluteStartTimeNano( 0 ),
   _frameStartTimeNano( 0 ),
   _lastFrameDurationNano( 0 ),
   _totalDurationNano( 0 )
{
   _absoluteStartTimeNano = _highResolutionClock->Now();
}

void GameClock::SetMinimumFrameRate( long long frameRate )
{
   if ( frameRate == 0 )
   {
      throw invalid_argument( "Minimum frame rate cannot be zero" );
   }
   else if ( frameRate < 0 )
   {
      _minimumFrameRate = -1;
      _minNanoSecondsPerFrame = -1;
      _hasMinimumFrameRate = false;
   }
   else
   {
      _minimumFrameRate = frameRate;
      _minNanoSecondsPerFrame = (long long)( ( 1 / (double)frameRate ) * 1'000'000'000 );
      _hasMinimumFrameRate = true;
   }
}

void GameClock::StartFrame()
{
   _frameStartTimeNano = _highResolutionClock->Now();
}

void GameClock::EndFrame()
{
   _totalFrameCount++;
   auto now = _highResolutionClock->Now();
   _lastFrameDurationNano = now - _frameStartTimeNano;
   _totalDurationNano = now - _absoluteStartTimeNano;
   _wasLagFrame = _hasMinimumFrameRate && ( _lastFrameDurationNano > _minNanoSecondsPerFrame );

   if ( _wasLagFrame )
   {
      _lagFrameCount++;
   }
}

long long GameClock::GetAverageFrameRate() const
{
   return _totalFrameCount == 0 ? 0 : ( 1'000'000'000 / ( _totalDurationNano / _totalFrameCount ) );
}

double GameClock::GetFrameSeconds() const
{
   return _wasLagFrame ? _minNanoSecondsPerFrame / 1'000'000'000. : _lastFrameDurationNano / 1'000'000'000.;
}
