#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IGameInputHandler.h>

class mock_GameInputHandler : public MegaManLofi::IGameInputHandler
{
public:
   MOCK_METHOD( void, HandleInput, ( ), ( override ) );
};
