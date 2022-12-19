#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IConsoleAnimation.h>

class mock_ConsoleAnimation : public MegaManLofi::IConsoleAnimation
{
public:
   MOCK_METHOD( void, Start, ( MegaManLofi::Coordinate<short>, MegaManLofi::Coordinate<short> ), ( override ) );
   MOCK_METHOD( bool, IsRunning, ( ), ( const override ) );
   MOCK_METHOD( void, Draw, ( ), ( override ) );
   MOCK_METHOD( void, Tick, ( int ), ( override ) );
};
