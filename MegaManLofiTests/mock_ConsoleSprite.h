#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ConsoleSprite.h>

class mock_ConsoleSprite : public MegaManLofi::ConsoleSprite
{
public:
   MOCK_METHOD( void, AddImage, ( MegaManLofi::ConsoleImage ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( short, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( short, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( float, GetTotalTraversalSeconds, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ConsoleImage&, GetCurrentImage, ( ), ( const, override ) );
};
