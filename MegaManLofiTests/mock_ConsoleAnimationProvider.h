#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IConsoleAnimationProvider.h>

class mock_ConsoleAnimationProvider : public MegaManLofi::IConsoleAnimationProvider
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IConsoleAnimation>, GetAnimation, ( MegaManLofi::ConsoleAnimationType ), ( const, override ) );
};
