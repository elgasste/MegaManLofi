#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IPlayerInfoProvider.h>

class mock_PlayerInfoProvider : public MegaManLofi::IPlayerInfoProvider
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IReadOnlyPlayer>, GetPlayer, ( ), ( const, override ) );
};
