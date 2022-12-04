#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArenaPhysics.h>

class mock_ArenaPhysics : public MegaManLofi::IArenaPhysics
{
public:
   MOCK_METHOD( void, MovePlayer, ( ), ( override ) );
};
