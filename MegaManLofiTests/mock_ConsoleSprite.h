#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IConsoleSprite.h>

class mock_ConsoleSprite : public MegaManLofi::IConsoleSprite
{
public:
   MOCK_METHOD( void, AddImage, ( MegaManLofi::ConsoleImage ), ( override ) );
   MOCK_METHOD( void, Tick, ( int ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( short, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( short, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( double, GetTotalTraversalSeconds, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ConsoleImage&, GetCurrentImage, ( ), ( const, override ) );
};
