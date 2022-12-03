#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IPhysics.h>

class mock_Physics : public MegaManLofi::IPhysics
{
public:
   MOCK_METHOD( void, PlayerApplyFriction, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
   MOCK_METHOD( void, PlayerApplyGravity, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
   MOCK_METHOD( void, PlayerPush, ( std::shared_ptr<MegaManLofi::IPlayer>, MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, PlayerJump, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
};
