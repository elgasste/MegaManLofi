#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IFrameRateProvider.h>

class mock_FrameRateProvider : public MegaManLofi::IFrameRateProvider
{
public:
   MOCK_METHOD( int, GetFramesPerSecond, ( ), ( const, override ) );
};
