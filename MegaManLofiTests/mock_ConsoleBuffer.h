#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ConsoleBuffer.h>

class mock_ConsoleBuffer : public MegaManLofi::ConsoleBuffer
{
public:
   MOCK_METHOD( void, LoadRenderDefs, ( std::shared_ptr<MegaManLofi::IGameRenderDefs> ), ( override ) );
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
   MOCK_METHOD( void, Draw, ( short, short, const MegaManLofi::ConsoleImage& ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, const std::shared_ptr<MegaManLofi::ConsoleSprite> ), ( override ) );
   MOCK_METHOD( void, Draw, ( short, short, const std::shared_ptr<MegaManLofi::IEntityConsoleSprite> ), ( override ) );
   MOCK_METHOD( void, Flip, ( ), ( override ) );
};
