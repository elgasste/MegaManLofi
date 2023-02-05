#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IBehavior.h>

class mock_Behavior : public MegaManLofi::IBehavior
{
public:
   MOCK_METHOD( void, SetInstructions, ( std::vector<mbc_instruction> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
};
