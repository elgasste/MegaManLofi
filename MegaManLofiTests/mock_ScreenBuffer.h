#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IScreenBuffer.h>

class mock_ScreenBuffer : public MegaManLofi::IScreenBuffer
{
public:
   MOCK_METHOD( void, LoadRenderDefs, ( const std::shared_ptr<MegaManLofi::IGameRenderDefs> ), ( override ) );
   MOCK_METHOD( void, CleanUp, ( ), ( override ) );
   MOCK_METHOD( void, Clear, ( ), ( override ) );
   MOCK_METHOD( void, Flip, ( ), ( override ) );
};
