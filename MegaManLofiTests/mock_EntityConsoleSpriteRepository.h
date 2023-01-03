#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/EntityConsoleSpriteRepository.h>

class mock_EntityConsoleSpriteRepository : public MegaManLofi::EntityConsoleSpriteRepository
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::EntityConsoleSprite>, GetSprite, ( int ), ( const, override ) );
   MOCK_METHOD( int, GetSpriteCount, ( ), ( const, override ) );
};
