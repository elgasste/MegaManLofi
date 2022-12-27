#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntityFactory.h>

class mock_EntityFactory : public MegaManLofi::IEntityFactory
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IEntity>, CreateBullet, ( MegaManLofi::Coordinate<long long>, MegaManLofi::Direction ), ( const, override ) );
};
