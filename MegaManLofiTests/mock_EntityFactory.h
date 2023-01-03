#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntityFactory.h>

class mock_EntityFactory : public MegaManLofi::IEntityFactory
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::Entity>, CreateBullet, ( MegaManLofi::Coordinate<float>, MegaManLofi::Direction ), ( const, override ) );
};
