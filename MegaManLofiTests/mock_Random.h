#pragma once
#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IRandom.h>

class mock_Random : public MegaManLofi::IRandom
{
public:
   MOCK_METHOD( unsigned int, GetUnsignedInt, ( unsigned int, unsigned int ), ( const, override ) );
};
