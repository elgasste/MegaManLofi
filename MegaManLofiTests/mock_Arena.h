#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArena.h>

class mock_Arena : public MegaManLofi::IArena
{
public:
   MOCK_METHOD( double, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( double, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( double, GetPlayerPositionX, ( ), ( const, override ) );
   MOCK_METHOD( double, GetPlayerPositionY, ( ), ( const, override ) );
   MOCK_METHOD( void, SetPlayerPositionX, ( double ), ( override ) );
   MOCK_METHOD( void, SetPlayerPositionY, ( double ), ( override ) );
   MOCK_METHOD( void, MovePlayer, ( ), ( override ) );
};
