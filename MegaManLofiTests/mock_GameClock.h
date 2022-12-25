#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IGameClock.h>

class mock_GameClock : public MegaManLofi::IGameClock
{
public:
   MOCK_METHOD( long long, GetCurrentFrame, ( ), ( const, override ) );
   MOCK_METHOD( double, GetFrameSeconds, ( ), ( const, override ) );
   MOCK_METHOD( void, StartFrame, ( ), ( override ) );
   MOCK_METHOD( void, EndFrame, ( ), ( override ) );
   MOCK_METHOD( long long, GetElapsedNanoseconds, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetAverageFrameRate, ( ), ( const, override ) );
};
