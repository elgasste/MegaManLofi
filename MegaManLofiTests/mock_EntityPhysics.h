#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/EntityPhysics.h>

class mock_EntityPhysics : public MegaManLofi::EntityPhysics
{
public:
   MOCK_METHOD( void, AssignTo, ( const std::shared_ptr<MegaManLofi::Stage> ), ( override ) );
};