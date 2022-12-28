#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IUniqueNumberGenerator.h>

class mock_UniqueNumberGenerator : public MegaManLofi::IUniqueNumberGenerator
{
public:
   MOCK_METHOD( int, GetNext, ( ), ( override ) );
};
