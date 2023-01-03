#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntityConsoleSprite.h>

class mock_EntityConsoleSprite : public MegaManLofi::IEntityConsoleSprite
{
public:
   MOCK_METHOD( void, AssignTo, ( const std::shared_ptr<MegaManLofi::IReadOnlyEntity> ), ( override ) );
   MOCK_METHOD( void, AddSprite, ( MegaManLofi::MovementType, MegaManLofi::Direction, const std::shared_ptr<MegaManLofi::IConsoleSprite> ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( short, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( short, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( float, GetTotalTraversalSeconds, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ConsoleImage&, GetCurrentImage, ( ), ( const, override ) );
};
