#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IPlayerInfoProvider.h>

class mock_PlayerInfoProvider : public MegaManLofi::IPlayerInfoProvider
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyPlayer>, GetPlayer, ( ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyEntity>, GetPlayerEntity, ( ), ( const, override ) );
};
