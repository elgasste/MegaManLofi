#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/EntityFactory.h>

class mock_EntityFactory : public MegaManLofi::EntityFactory
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::Entity>, CreateBullet, ( MegaManLofi::Coordinate<float>, MegaManLofi::Direction ), ( const, override ) );
};
