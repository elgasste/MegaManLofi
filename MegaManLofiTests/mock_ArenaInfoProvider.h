#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArenaInfoProvider.h>

class mock_ArenaInfoProvider : public MegaManLofi::IArenaInfoProvider
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyArena>, GetActiveArena, ( ), ( const, override ) );
};
