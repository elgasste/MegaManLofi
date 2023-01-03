#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/GameEventAggregator.h>

class mock_GameEventAggregator : public MegaManLofi::GameEventAggregator
{
public:
   MOCK_METHOD( void, RegisterEventHandler, ( MegaManLofi::GameEvent, std::function<void()> ), ( override ) );
   MOCK_METHOD( void, RaiseEvent, ( MegaManLofi::GameEvent ), ( const, override ) );
};
