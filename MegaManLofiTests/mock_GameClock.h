#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IGameClock.h>

class mock_GameClock : public MegaManLofi::IGameClock
{
public:
   MOCK_METHOD( void, StartFrame, ( ), ( override ) );
   MOCK_METHOD( void, WaitForNextFrame, ( ), ( override ) );
   MOCK_METHOD( unsigned int, GetFramesPerSecond, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetTotalFrameCount, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetLagFrameCount, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetCurrentFrame, ( ), ( const, override ) );
   MOCK_METHOD( double, GetSecondsPerFrame, ( ), ( const, override ) );
};
