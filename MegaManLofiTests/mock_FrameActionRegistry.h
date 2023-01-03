#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/FrameActionRegistry.h>

class mock_FrameActionRegistry : public MegaManLofi::FrameActionRegistry
{
public:
   MOCK_METHOD( void, Clear, ( ), ( override ) );
   MOCK_METHOD( void, FlagAction, ( MegaManLofi::FrameAction ), ( override ) );
   MOCK_METHOD( bool, ActionFlagged, ( MegaManLofi::FrameAction ), ( const, override ) );
};
