#pragma once

#include <gmock/gmock.h>
#include <string>

#include <MegaManLofi/IPlayerPhysics.h>

class mock_PlayerPhysics : public MegaManLofi::IPlayerPhysics
{
public:
   MOCK_METHOD( void, ApplyFriction, (), ( const, override ) );
   MOCK_METHOD( void, ApplyGravity, (), ( const, override ) );
   MOCK_METHOD( void, Point, ( MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, Push, ( MegaManLofi::Direction ), ( const, override ) );
   MOCK_METHOD( void, Jump, ( ), ( const, override ) );
   MOCK_METHOD( void, ExtendJump, ( ), ( const, override ) );
};
