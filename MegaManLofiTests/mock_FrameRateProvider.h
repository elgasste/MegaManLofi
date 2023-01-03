#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IFrameRateProvider.h>

class mock_FrameRateProvider : public MegaManLofi::IFrameRateProvider
{
public:
   MOCK_METHOD( long long, GetMinimumFrameRate, ( ), ( const, override ) );
   MOCK_METHOD( bool, HasMinimumFrameRate, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetCurrentFrame, ( ), ( const, override ) );
   MOCK_METHOD( float, GetFrameSeconds, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetLagFrameCount, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetElapsedNanoseconds, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetAverageFrameRate, ( ), ( const, override ) );
};
