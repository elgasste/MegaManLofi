#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/UniqueNumberGenerator.h>

class mock_UniqueNumberGenerator : public MegaManLofi::UniqueNumberGenerator
{
public:
   MOCK_METHOD( int, GetNext, ( ), ( override ) );
};
