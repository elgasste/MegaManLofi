#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ArenaPhysics.h>

class mock_ArenaPhysics : public MegaManLofi::ArenaPhysics
{
public:
   MOCK_METHOD( void, AssignTo, ( const std::shared_ptr<MegaManLofi::Arena> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
};
