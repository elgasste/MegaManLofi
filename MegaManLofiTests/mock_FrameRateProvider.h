#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IFrameRateProvider.h>

class mock_FrameRateProvider : public MegaManLofi::IFrameRateProvider
{
public:
   MOCK_METHOD( long long, GetCurrentFrame, ( ), ( const, override ) );
   MOCK_METHOD( double, GetFrameSeconds, ( ), ( const, override ) );
};
