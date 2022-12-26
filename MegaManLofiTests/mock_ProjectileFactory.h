#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IProjectileFactory.h>

class mock_ProjectileFactory : public MegaManLofi::IProjectileFactory
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IEntity>, CreateBullet, ( MegaManLofi::Coordinate<long long>, MegaManLofi::Direction ), ( const, override ) );
};
