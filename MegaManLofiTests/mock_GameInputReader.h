#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IGameInputReader.h>

class mock_GameInputReader : public MegaManLofi::IGameInputReader
{
public:
   MOCK_METHOD( void, ReadInput, ( ), ( override ) );
   MOCK_METHOD( bool, WasButtonPressed, ( MegaManLofi::GameButton ), ( const, override ) );
   MOCK_METHOD( bool, IsButtonDown, ( MegaManLofi::GameButton ), ( const, override ) );
   MOCK_METHOD( bool, WasAnyButtonPressed, ( ), ( const, override ) );
};
