#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ISleeper.h>

class mock_Sleeper : public MegaManLofi::ISleeper
{
public:
   MOCK_METHOD( void, Sleep, ( long long ), ( override ) );
};
