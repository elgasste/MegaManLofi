#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IPhysics.h>

class mock_PhysicsEngine : public MegaManLofi::IPhysics
{
public:
   MOCK_METHOD( void, PlayerApplyFriction, ( std::shared_ptr<MegaManLofi::IPlayer> ), ( const, override ) );
};
