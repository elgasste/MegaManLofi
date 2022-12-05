#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IGame.h>

class mock_Game : public MegaManLofi::IGame
{
public:
   MOCK_METHOD( void, Tick, ( ), ( override ) );
};
