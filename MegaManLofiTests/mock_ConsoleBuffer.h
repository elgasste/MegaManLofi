#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IConsoleBuffer.h>

class mock_ConsoleBuffer : public MegaManLofi::IConsoleBuffer
{
public:
   MOCK_METHOD( void, Initialize, ( ), ( override ) );
   MOCK_METHOD( void, CleanUp, ( ), ( override ) );

   MOCK_METHOD( void, SetDefaultForegroundColor, ( MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, SetDefaultBackgroundColor, ( MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Clear, ( ), ( override ) );

   MOCK_METHOD( void, Draw, ( int, int, char ), ( override ) );
   MOCK_METHOD( void, Draw, ( int, int, char, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( int, int, char, MegaManLofi::ConsoleColor, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( int, int, const std::string& ), ( override ) );
   MOCK_METHOD( void, Draw, ( int, int, const std::string&, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( int, int, const std::string&, MegaManLofi::ConsoleColor, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( int, int, const MegaManLofi::ConsoleSprite& ), ( override ) );

   MOCK_METHOD( void, Flip, ( ), ( override ) );
};
