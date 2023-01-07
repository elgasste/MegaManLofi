#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ReadOnlyPlayer.h>

class mock_ReadOnlyPlayer : public MegaManLofi::ReadOnlyPlayer
{
public:
   MOCK_METHOD( unsigned int, GetLivesRemaining, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsJumping, ( ), ( const, override ) );
};
