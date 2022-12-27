#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntityConsoleSprite.h>

class mock_EntityConsoleSprite : public MegaManLofi::IEntityConsoleSprite
{
public:
   MOCK_METHOD( void, AddSprite, ( MegaManLofi::MovementType, MegaManLofi::Direction, const std::shared_ptr<MegaManLofi::IConsoleSprite> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( short, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( short, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( double, GetTotalTraversalSeconds, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ConsoleImage&, GetCurrentImage, ( ), ( const, override ) );
   MOCK_METHOD( void, SetMovementType, ( MegaManLofi::MovementType ), ( override ) );
   MOCK_METHOD( void, SetDirection, ( MegaManLofi::Direction ), ( override ) );
};
