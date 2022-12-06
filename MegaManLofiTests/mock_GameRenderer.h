#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IGameRenderer.h>

class mock_GameRenderer : public MegaManLofi::IGameRenderer
{
public:
   MOCK_METHOD( void, Render, ( ), ( override ) );
   MOCK_METHOD( bool, HasFocus, ( ), ( const, override ) );
};
