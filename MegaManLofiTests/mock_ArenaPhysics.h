#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArenaPhysics.h>

class mock_ArenaPhysics : public MegaManLofi::IArenaPhysics
{
public:
   MOCK_METHOD( void, AssignTo, ( std::shared_ptr<MegaManLofi::IArena>,
                                  std::shared_ptr<MegaManLofi::IPlayer> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
};
