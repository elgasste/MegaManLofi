#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IConsoleBuffer.h>

class mock_ConsoleBuffer : public MegaManLofi::IConsoleBuffer
{
public:
   MOCK_METHOD( void, LoadRenderConfig, ( std::shared_ptr<MegaManLofi::IGameRenderConfig> ), ( override ) );
   MOCK_METHOD( void, CleanUp, ( ), ( override ) );

   MOCK_METHOD( void, SetDefaultForegroundColor, ( MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, SetDefaultBackgroundColor, ( MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Clear, ( ), ( override ) );

   MOCK_METHOD( void, Draw, ( short, short, char ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, char, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, char, MegaManLofi::ConsoleColor, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, const std::string& ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, const std::string&, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, const std::string&, MegaManLofi::ConsoleColor, MegaManLofi::ConsoleColor ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, const MegaManLofi::ConsoleSprite& ), ( override ) );

   MOCK_METHOD( void, Flip, ( ), ( override ) );
};
