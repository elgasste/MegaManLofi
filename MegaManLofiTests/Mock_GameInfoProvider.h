#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IGameInfoProvider.h>

class mock_GameInfoProvider : public MegaManLofi::IGameInfoProvider
{
public:
   MOCK_METHOD( MegaManLofi::GameState, GetGameState, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsPaused, ( ), ( const, override ) );
};
