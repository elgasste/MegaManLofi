#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntityConsoleSpriteRepository.h>

class mock_EntityConsoleSpriteRepository : public MegaManLofi::IEntityConsoleSpriteRepository
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IEntityConsoleSprite>, GetSprite, ( int ), ( const, override ) );
   MOCK_METHOD( int, GetSpriteCount, ( ), ( const, override ) );
};
