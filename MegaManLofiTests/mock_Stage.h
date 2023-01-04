#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/Stage.h>

class mock_Stage : public MegaManLofi::Stage
{
public:
   MOCK_METHOD( void, AddArena, ( std::shared_ptr<MegaManLofi::Arena> ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( void, SetActiveArena, ( int ), ( override ) );
};
