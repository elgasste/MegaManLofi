#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/PlayerPhysics.h>

class mock_PlayerPhysics : public MegaManLofi::PlayerPhysics
{
public:
   MOCK_METHOD( void, AssignTo, ( std::shared_ptr<MegaManLofi::Player> ), ( override ) );
   MOCK_METHOD( void, Tick, (), ( override ) );
   MOCK_METHOD( void, PointTo, ( MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, PushTo, ( MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, Jump, ( ), ( override ) );
   MOCK_METHOD( void, ExtendJump, ( ), ( override ) );
};
