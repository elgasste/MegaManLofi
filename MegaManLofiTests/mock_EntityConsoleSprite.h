#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/EntityConsoleSprite.h>

class mock_EntityConsoleSprite : public MegaManLofi::EntityConsoleSprite
{
public:
   MOCK_METHOD( void, AssignTo, ( const std::shared_ptr<MegaManLofi::ReadOnlyEntity> ), ( override ) );
   MOCK_METHOD( void, AddSprite, ( MegaManLofi::MovementType, MegaManLofi::Direction, const std::shared_ptr<MegaManLofi::ConsoleSprite> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( short, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( short, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( float, GetTotalTraversalSeconds, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ConsoleImage&, GetCurrentImage, ( ), ( const, override ) );
};
