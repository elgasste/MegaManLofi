#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IGameCommandExecutor.h>

class mock_GameCommandExecutor : public MegaManLofi::IGameCommandExecutor
{
public:
   MOCK_METHOD( void, ExecuteCommand, ( MegaManLofi::GameCommand ), ( override ) );
   MOCK_METHOD( void, ExecuteCommand, ( MegaManLofi::GameCommand, const std::shared_ptr<MegaManLofi::GameCommandArgs> ), ( override ) );
};
