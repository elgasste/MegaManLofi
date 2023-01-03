#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArenaPhysics.h>

class mock_ArenaPhysics : public MegaManLofi::IArenaPhysics
{
public:
   MOCK_METHOD( void, AssignTo, ( const std::shared_ptr<MegaManLofi::Arena> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
};
