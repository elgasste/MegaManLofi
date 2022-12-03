#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IHighResolutionClock.h>

class mock_HighResolutionClock : public MegaManLofi::IHighResolutionClock
{
public:
   MOCK_METHOD( long long, Now, ( ), ( override ) );
};
