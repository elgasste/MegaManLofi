#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IReadOnlyPlayer.h>

class mock_ReadOnlyPlayer : public MegaManLofi::IReadOnlyPlayer
{
public:
   MOCK_METHOD( unsigned int, GetLivesRemaining, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsJumping, ( ), ( const, override ) );
};
