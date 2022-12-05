#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IPlayerPhysics.h>

class mock_PlayerPhysics : public MegaManLofi::IPlayerPhysics
{
public:
   MOCK_METHOD( void, AssignTo, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( override ) );
   MOCK_METHOD( void, Tick, (), ( override ) );
   MOCK_METHOD( void, PointTo, ( MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, PushTo, ( MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, Jump, ( ), ( override ) );
   MOCK_METHOD( void, ExtendJump, ( ), ( override ) );
};
