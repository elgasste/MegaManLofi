#pragma once
#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IRandom.h>

class mock_Random : public MegaManLofi::IRandom
{
public:
   MOCK_METHOD( int, GetInt, ( int, int ), ( const, override ) );
};
