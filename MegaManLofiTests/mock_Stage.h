#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/Stage.h>

class mock_Stage : public MegaManLofi::Stage
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyArena>, GetArena, ( int ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyArena>, GetActiveArena, ( ), ( const, override ) );
   MOCK_METHOD( std::optional<std::reference_wrapper<const MegaManLofi::ArenaPortal>>, GetArenaPortal, ( MegaManLofi::Direction, int ), ( const, override ) );

   MOCK_METHOD( const std::shared_ptr<MegaManLofi::Arena>, GetMutableActiveArena, ( ), ( const, override ) );
   MOCK_METHOD( void, AddArena, ( std::shared_ptr<MegaManLofi::Arena> ), ( override ) );
   MOCK_METHOD( void, AddArenaPortal, ( MegaManLofi::Direction, MegaManLofi::ArenaPortal ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( void, SetActiveArena, ( int ), ( override ) );
};
