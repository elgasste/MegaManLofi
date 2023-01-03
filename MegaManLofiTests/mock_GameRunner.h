#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/GameRunner.h>

class mock_Game : public MegaManLofi::GameRunner
{
public:
   MOCK_METHOD( void, Run, ( ), ( override ) );
};
