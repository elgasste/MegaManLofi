#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ReadOnlyStage.h>

class mock_ReadOnlyStage : public MegaManLofi::ReadOnlyStage
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyArena>, GetArena, ( int ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyArena>, GetActiveArena, ( ), ( const, override ) );
   MOCK_METHOD( std::optional<std::reference_wrapper<const MegaManLofi::ArenaPortal>>, GetArenaPortal, ( MegaManLofi::Direction, int ), ( const, override ) );
};