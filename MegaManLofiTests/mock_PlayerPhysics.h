#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IPlayerPhysics.h>

class mock_PlayerPhysics : public MegaManLofi::IPlayerPhysics
{
public:
   MOCK_METHOD( void, ApplyFriction, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
   MOCK_METHOD( void, ApplyGravity, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
   MOCK_METHOD( void, Push, ( std::shared_ptr<MegaManLofi::IPlayer>, MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, Jump, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
};
